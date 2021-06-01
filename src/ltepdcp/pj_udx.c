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
  
        Name:    LTE PDCP Interface  
    
        Type:    C file
  
        Desc:    This file contains the pack/unpack functions for the
                 data structures at UDX Interface.
 
        File:    pj_udx.c

        Sid:      eq_udx.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:52 2016
  
        Prg:      
  
*********************************************************************21*/
#if(defined(LCUDX) || defined(LWLCUDX)) 
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=224;
#endif

/* header include files (.h) */
  
#include "envopt.h"             /* Environment options */  
#include "envdep.h"             /* Environment dependent */
#include "envind.h"             /* Environment independent */
#include "gen.h"                /* General */
#include "ssi.h"                /* System services interface */
#include "cm_lte.h"             /* Common LTE header file */
#include "cm_llist.h"           /* Linked List header file */
#include "cm_tkns.h"            /* Common tokens header file */
#include "cm_hash.h"            /* Hash List header file */
#include "cpj.h"                /* CPJ Interface header file */
#include "lpj.h"
#include "pj_udx.h"



/* header/extern include files (.x) */

#include "gen.x"                /* General */
#include "ssi.x"                /* System services interface */
#include "cm_lib.x"             /* Common lib header file */
#include "cm_hash.x"            /* Hash List header file */
#include "cm_llist.x"           /* Linked List header file */
#include "cm_tkns.x"            /* Common tokens header file */
#include "cm_lte.x"             /* Common LTE header file */
#include "cpj.x"                /* CPJ Interface header file */
#include "pj.h"
#include "lpj.x"
#include "pj_udx.x"

#if(defined(LCUDX) || defined(LWLCUDX)) 
/* Packing/Unpacking Macros */

#ifdef ANSI
PUBLIC S16 cmPkUdxBndReq
(
Pst * pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 cmPkUdxBndReq(pst, suId, spId)
Pst * pst;
SuId suId;
SpId spId;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxBndReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX001, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX002, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX003, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTUDXBNDREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxBndReq
(
UdxBndReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxBndReq(func, pst, mBuf)
UdxBndReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId;
   SpId spId;
   
   TRC3(cmUnpkUdxBndReq)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX004, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX005, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, spId));
}

#ifdef ANSI
PUBLIC S16 cmPkUdxBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 cmPkUdxBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxBndCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ006, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SPkU8(status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ007, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ008, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTCPJBNDCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxBndCfm
(
UdxBndCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxBndCfm(func, pst, mBuf)
UdxBndCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SuId suId;
   U8 status;
   
   TRC3(cmUnpkUdxBndCfm)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ009, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkU8(&status, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ010, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, status));
}

#ifdef ANSI
PUBLIC S16 cmPkUdxUbndReq
(
Pst * pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 cmPkUdxUbndReq(pst, spId, reason)
Pst * pst;
SpId spId;
Reason reason;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxUbndReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX011, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

   if (SPkS16(reason, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX012, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX013, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTUDXUBNDREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxUbndReq
(
UdxUbndReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUbndReq(func, pst, mBuf)
UdxUbndReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   SpId spId;
   Reason reason;
   
   TRC3(cmUnpkUdxUbndReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX014, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkS16(&reason, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX015, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, reason));
}

#ifdef ANSI
PUBLIC S16 cmPkUdxCfgReq
(
Pst * pst,
SpId spId,
UdxCfgReqInfo * cfgReq
)
#else
PUBLIC S16 cmPkUdxCfgReq(pst, spId, cfgReq)
Pst * pst;
SpId spId;
UdxCfgReqInfo * cfgReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxCfgReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX016, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)cfgReq, sizeof(UdxCfgReqInfo));
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {

            if (cmPkUdxCfgReqInfo(pst, cfgReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDX017, (ErrVal)0, "Packing failed");
#endif
               SPutSBuf(pst->region, pst->pool, (Data *)cfgReq, sizeof(UdxCfgReqInfo));
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }

            /* Free Request Structure after packing */
            SPutSBuf(pst->region, pst->pool, (Data *)cfgReq, sizeof(UdxCfgReqInfo));


            break;
         }
      case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)cfgReq,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }

    if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX019, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)cfgReq, sizeof(UdxCfgReqInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   pst->event = (Event) EVTUDXCFGREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmPkUdxUlStaRep
(
Pst * pst,
SpId spId,
UdxUlStaRepInfo *staRep
)
#else
PUBLIC S16 cmPkUdxUlStaRep(pst, spId, staRep)
Pst * pst;
SpId spId;
UdxUlStaRepInfo *staRep;
#endif
{
   S16 ret;
   Buffer *mBuf;
   mBuf = NULLP;

   TRC3(cmPkUdxUlStaRep)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX016, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      SPutSBuf(pst->region, pst->pool, (Data *)staRep, sizeof(UdxUlStaRepInfo));

      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            ret = cmPkUdxUlStaRepInfo( (staRep), pst, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
            if(ret != ROK)
            {
               PJ_FREE_BUF(mBuf);
               PJ_FREE_BUF(staRep->sta);
               SPutSBuf(pst->region, pst->pool, (Data *)staRep, sizeof(UdxUlStaRepInfo));
               
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDXXXX, (ErrVal)ret, "Packing failure");
               RETVALUE( ret );
            }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
            PJ_FREE_BUF(staRep->sta);
            if (SPutSBuf(pst->region, pst->pool, (Data *)staRep,
                     sizeof(UdxUlStaRepInfo)) != ROK)
            {
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }

            break;
         }
       case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)staRep,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }
   
   CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
   pst->event = (Event) EVTUDXULSTAREP;

   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmPkUdxDlStaRep
(
Pst * pst,
SpId spId,
UdxDlStaRepInfo *staRep
)
#else
PUBLIC S16 cmPkUdxDlStaRep(pst, spId, staRep)
Pst * pst;
SpId spId;
UdxDlStaRepInfo *staRep;
#endif
{
   S16 ret;
   Buffer *mBuf;
   mBuf = NULLP;

   TRC3(cmPkUdxDlStaRep)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) 
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX016, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(staRep->staPdu);
      SPutSBuf(pst->region, pst->pool, (Data *)staRep, sizeof(UdxDlStaRepInfo));

      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            ret = cmPkUdxDlStaRepInfo( (staRep), pst, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
            if(ret != ROK)
            {
               PJ_FREE_BUF(mBuf);
               PJ_FREE_BUF(staRep->staPdu);
               SPutSBuf(pst->region, pst->pool, (Data *)staRep, sizeof(UdxDlStaRepInfo));
               
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDXXXX, (ErrVal)ret, "Packing failure");
               RETVALUE( ret );
            }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
            PJ_FREE_BUF(staRep->staPdu);
            if (SPutSBuf(pst->region, pst->pool, (Data *)staRep,
                     sizeof(UdxDlStaRepInfo)) != ROK)
            {
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }

            break;
         }
       case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)staRep,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }
   
   CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
   pst->event = (Event) EVTUDXDLSTAREP;

   RETVALUE(SPstTsk(pst,mBuf));
}


#ifdef ANSI
PUBLIC S16 cmUnpkUdxCfgReq
(
UdxCfgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCfgReq(func, pst, mBuf)
UdxCfgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   UdxCfgReqInfo *cfgReq;
   
   TRC3(cmUnpkUdxCfgReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX021, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {

            if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&cfgReq,\
                        sizeof(UdxCfgReqInfo))) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               RETVALUE(ret1);
            }

            if (cmUnpkUdxCfgReqInfo(pst, cfgReq, mBuf) != ROK) {
               SPutSBuf(pst->region, pst->pool, (Data *)cfgReq, sizeof(UdxCfgReqInfo));
               PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDX023, (ErrVal)0, "Packing failed");
#endif
               RETVALUE(RFAILED);
            }
            break;
         }
      case UDX_SEL_LWLC:
         {
            CMCHKUNPK(cmUnpkPtr,(PTR *) &cfgReq, mBuf);
            break;
         }
#endif /* LCUDX */
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, cfgReq));
}

#ifdef ANSI
PUBLIC S16 cmPkUdxCfgCfm
(
 Pst * pst,
 SuId suId,
 UdxCfgCfmInfo * cfgCfm
 )
#else
PUBLIC S16 cmPkUdxCfgCfm(pst, suId, cfgCfm)
        Pst * pst;
        SuId suId;
        UdxCfgCfmInfo * cfgCfm;
#endif
{
        Buffer *mBuf = NULLP;
        TRC3(cmPkUdxCfgCfm)

                if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ024, (ErrVal)0, "Packing failed");
#endif
                        SPutSBuf(pst->region, pst->pool, (Data *)cfgCfm, sizeof(UdxCfgCfmInfo));
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
           case UDX_SEL_LC:
              {

                 if (cmPkUdxCfgCfmInfo(cfgCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                    SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                          __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                          (ErrVal)ECPJ025, (ErrVal)0, "Packing failed");
#endif
                    SPutSBuf(pst->region, pst->pool, (Data *)cfgCfm, sizeof(UdxCfgCfmInfo));
                    PJ_FREE_BUF(mBuf);
                    RETVALUE(RFAILED);
                 }
                 /* cpj_c_001.main_3 */
                 /* Free Confirm Structure after packing */
                 SPutSBuf(pst->region, pst->pool, (Data *)cfgCfm, sizeof(UdxCfgCfmInfo));


                 break;
              }
           case UDX_SEL_LWLC:
              {
                 CMCHKPKLOG(cmPkPtr,(PTR)cfgCfm,mBuf,EUDXXXX,pst);
                 break;
              }
#endif /* LCUDX */

        }

        if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                (ErrVal)ECPJ027, (ErrVal)0, "Packing failed");
#endif
                SPutSBuf(pst->region, pst->pool, (Data *)cfgCfm, sizeof(UdxCfgCfmInfo));
                PJ_FREE_BUF(mBuf);
                RETVALUE(RFAILED);
        }

        pst->event = (Event) EVTCPJCFGCFM;
        RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxCfgCfm
(
 UdxCfgCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkUdxCfgCfm(func, pst, mBuf)
        UdxCfgCfm func;
        Pst *pst;
        Buffer *mBuf;
#endif
{
        S16   ret1;
        SuId suId;
        UdxCfgCfmInfo *cfgCfm;

        TRC3(cmUnpkUdxCfgCfm)

                if (SUnpkS16(&suId, mBuf) != ROK) {
                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ029, (ErrVal)0, "Packing failed");
#endif
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        {


                                if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&cfgCfm,\
                                                                sizeof(UdxCfgCfmInfo))) != ROK)
                                {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                        RETVALUE(ret1);
                                }

                                if (cmUnpkUdxCfgCfmInfo(cfgCfm, mBuf) != ROK) {
                                        SPutSBuf(pst->region, pst->pool, (Data *)cfgCfm, sizeof(UdxCfgCfmInfo));
                                        PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ031, (ErrVal)0, "Packing failed");
#endif
                                        RETVALUE(RFAILED);
                                }
                                break;
                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKUNPK(cmUnpkPtr,(PTR *) &cfgCfm, mBuf);
                                break;
                        }
#endif /* LCUDX */
        }
        PJ_FREE_BUF(mBuf);
        RETVALUE((*func)(pst, suId, cfgCfm));
}


#ifdef ANSI
PUBLIC S16 cmPkUdxReEstReq
(
Pst * pst,
SpId spId,
UdxReEstReqInfo * reEstReq
)
#else
PUBLIC S16 cmPkUdxReEstReq(pst, spId, reEstReq)
Pst * pst;
SpId spId;
UdxReEstReqInfo * reEstReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxReEstReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX032, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)reEstReq, sizeof(UdxReEstReqInfo));
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   { 
                           if (cmPkUdxReEstReqInfo(reEstReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX033, (ErrVal)0, "Packing failed");
#endif
                                   SPutSBuf(pst->region, pst->pool, (Data *)reEstReq, sizeof(UdxReEstReqInfo));
                                   PJ_FREE_BUF(mBuf);
                                   RETVALUE(RFAILED);
                           }
                           /* Free Request Structure after packing */
                           SPutSBuf(pst->region, pst->pool, (Data *)reEstReq, sizeof(UdxReEstReqInfo));

                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKPKLOG(cmPkPtr,(PTR)reEstReq,mBuf,EUDXXXX,pst);
                           break;
                   }
#endif /* LCUDX */
   }

   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX035, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)reEstReq, sizeof(UdxReEstReqInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTUDXREESTREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxReEstReq
(
UdxReEstReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxReEstReq(func, pst, mBuf)
UdxReEstReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   UdxReEstReqInfo *reEstReq;
   
   TRC3(cmUnpkUdxReEstReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX037, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   { 

                           if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&reEstReq,\
                                                           sizeof(UdxReEstReqInfo))) != ROK)
                           {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                   RETVALUE(ret1);
                           }

                           if (cmUnpkUdxReEstReqInfo(reEstReq, mBuf) != ROK) {
                                   SPutSBuf(pst->region, pst->pool, (Data *)reEstReq, sizeof(UdxReEstReqInfo));
                                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX039, (ErrVal)0, "Packing failed");
#endif
                                   RETVALUE(RFAILED);
                           }
                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKUNPK(cmUnpkPtr,(PTR *) &reEstReq, mBuf);
                           break;
                   }
#endif /* LCUDX */
}

   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, reEstReq));
}


#ifdef ANSI
PUBLIC S16 cmPkUdxReEstCfm
(
 Pst * pst,
 SuId suId,
 UdxReEstCfmInfo * reEstCfm
 )
#else
PUBLIC S16 cmPkUdxReEstCfm(pst, suId, reEstCfm)
        Pst * pst;
        SuId suId;
        UdxReEstCfmInfo * reEstCfm;
#endif
{
        Buffer *mBuf = NULLP;
        TRC3(cmPkUdxReEstCfm)

                if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ040, (ErrVal)0, "Packing failed");
#endif
                        SPutSBuf(pst->region, pst->pool, (Data *)reEstCfm, sizeof(UdxReEstCfmInfo));
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        { 

                                if (cmPkUdxReEstCfmInfo(reEstCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ041, (ErrVal)0, "Packing failed");
#endif
                                        SPutSBuf(pst->region, pst->pool, (Data *)reEstCfm, sizeof(UdxReEstCfmInfo));
                                        PJ_FREE_BUF(mBuf);
                                        RETVALUE(RFAILED);
                                }

                                /* Free Confirm Structure after packing */
                                SPutSBuf(pst->region, pst->pool, (Data *)reEstCfm, sizeof(UdxReEstCfmInfo));
                                break;
                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKPKLOG(cmPkPtr,(PTR)reEstCfm,mBuf,EUDXXXX,pst);
                                break;
                        }
#endif /* LCUDX */
        }
        if (SPkS16(suId, mBuf) != ROK)
        {
#if (ERRCLASS & ERRCLS_ADD_RES)
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                (ErrVal)ECPJ043, (ErrVal)0, "Packing failed");
#endif
                SPutSBuf(pst->region, pst->pool, (Data *)reEstCfm, sizeof(UdxReEstCfmInfo));
                PJ_FREE_BUF(mBuf);
                RETVALUE(RFAILED);
        }


        pst->event = (Event) EVTCPJREESTCFM;
        RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxReEstCfm
(
 UdxReEstCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkUdxReEstCfm(func, pst, mBuf)
        UdxReEstCfm func;
        Pst *pst;
        Buffer *mBuf;
#endif
{
        S16   ret1;
        SuId suId;
        UdxReEstCfmInfo *reEstCfm;

        TRC3(cmUnpkUdxReEstCfm)

                if (SUnpkS16(&suId, mBuf) != ROK) {
                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ045, (ErrVal)0, "Packing failed");
#endif
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        { 


                                if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&reEstCfm,\
                                                                sizeof(UdxReEstCfmInfo))) != ROK)
                                {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                        RETVALUE(ret1);
                                }

                                if (cmUnpkUdxReEstCfmInfo(reEstCfm, mBuf) != ROK) {
                                        SPutSBuf(pst->region, pst->pool, (Data *)reEstCfm, sizeof(UdxReEstCfmInfo));
                                        PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ047, (ErrVal)0, "Packing failed");
#endif
                                        RETVALUE(RFAILED);
                                }
                                break;
                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKUNPK(cmUnpkPtr,(PTR *) &reEstCfm, mBuf);
                                break;
                        }
#endif /* LCUDX */
        }

        PJ_FREE_BUF(mBuf);
        RETVALUE((*func)(pst, suId, reEstCfm));
}


#ifdef ANSI
PUBLIC S16 cmPkUdxSecCfgReq
(
Pst * pst,
SpId spId,
UdxSecCfgReqInfo * secCfgReq
)
#else
PUBLIC S16 cmPkUdxSecCfgReq(pst, spId, secCfgReq)
Pst * pst;
SpId spId;
UdxSecCfgReqInfo * secCfgReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxSecCfgReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX048, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)secCfgReq, sizeof(UdxSecCfgReqInfo));
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if (cmPkUdxSecCfgReqInfo(secCfgReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX049, (ErrVal)0, "Packing failed");
#endif
                                   SPutSBuf(pst->region, pst->pool, (Data *)secCfgReq, sizeof(UdxSecCfgReqInfo));
                                   PJ_FREE_BUF(mBuf);
                                   RETVALUE(RFAILED);
                           }  
                           /* Free Request Structure after packing */
                           SPutSBuf(pst->region, pst->pool, (Data *)secCfgReq, sizeof(UdxSecCfgReqInfo));
                           break;

                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKPKLOG(cmPkPtr,(PTR)secCfgReq,mBuf,EUDXXXX,pst);
                           break;
                   }
#endif /* LCUDX */
   }

   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX051, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)secCfgReq, sizeof(UdxSecCfgReqInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

    pst->event = (Event) EVTUDXSECCFGREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxSecCfgReq
(
UdxSecCfgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxSecCfgReq(func, pst, mBuf)
UdxSecCfgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   UdxSecCfgReqInfo *secCfgReq;
   
   TRC3(cmUnpkUdxSecCfgReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX053, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&secCfgReq,\
                                                           sizeof(UdxSecCfgReqInfo))) != ROK)
                           {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                   RETVALUE(ret1);
                           }

                           if (cmUnpkUdxSecCfgReqInfo(secCfgReq, mBuf) != ROK) {
                                   SPutSBuf(pst->region, pst->pool, (Data *)secCfgReq, sizeof(UdxSecCfgReqInfo));
                                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX055, (ErrVal)0, "Packing failed");
#endif
                                   RETVALUE(RFAILED);
                           }
                           break;

                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKUNPK(cmUnpkPtr,(PTR *) &secCfgReq,mBuf);
                           break;
                   }
#endif /* LCUDX */
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, secCfgReq));
}


#ifdef ANSI
PUBLIC S16 cmPkUdxSecCfgCfm
(
 Pst * pst,
 SuId suId,
 UdxSecCfgCfmInfo * secCfgCfm
 )
#else
PUBLIC S16 cmPkUdxSecCfgCfm(pst, suId, secCfgCfm)
        Pst * pst;
        SuId suId;
        UdxSecCfgCfmInfo * secCfgCfm;
#endif
{
        Buffer *mBuf = NULLP;
        TRC3(cmPkUdxSecCfgCfm)

                if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ056, (ErrVal)0, "Packing failed");
#endif
                        SPutSBuf(pst->region, pst->pool, (Data *)secCfgCfm, sizeof(UdxSecCfgCfmInfo));
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        {


                                if (cmPkUdxSecCfgCfmInfo(secCfgCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ057, (ErrVal)0, "Packing failed");
#endif
                                        SPutSBuf(pst->region, pst->pool, (Data *)secCfgCfm, sizeof(UdxSecCfgCfmInfo));
                                        PJ_FREE_BUF(mBuf);
                                        RETVALUE(RFAILED);
                                }
                                /* Free Confirm Structure after packing */
                                SPutSBuf(pst->region, pst->pool, (Data *)secCfgCfm, sizeof(UdxSecCfgCfmInfo));


                                break;

                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKPKLOG(cmPkPtr,(PTR)secCfgCfm,mBuf,EUDXXXX,pst);
                                break;
                        }
#endif /* LCUDX */
        }



        if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                (ErrVal)ECPJ059, (ErrVal)0, "Packing failed");
#endif
                SPutSBuf(pst->region, pst->pool, (Data *)secCfgCfm, sizeof(UdxSecCfgCfmInfo));
                PJ_FREE_BUF(mBuf);
                RETVALUE(RFAILED);
        }

        pst->event = (Event) EVTCPJSECCFGCFM;
        RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxSecCfgCfm
(
 UdxSecCfgCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkUdxSecCfgCfm(func, pst, mBuf)
        UdxSecCfgCfm func;
        Pst *pst;
        Buffer *mBuf;
#endif
{
        S16   ret1;
        SuId suId;
        UdxSecCfgCfmInfo *secCfgCfm;

        TRC3(cmUnpkUdxSecCfgCfm)

                if (SUnpkS16(&suId, mBuf) != ROK) {
                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ061, (ErrVal)0, "Packing failed");
#endif
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        {


                                if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&secCfgCfm,\
                                                                sizeof(UdxSecCfgCfmInfo))) != ROK)
                                {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                        RETVALUE(ret1);
                                }

                                if (cmUnpkUdxSecCfgCfmInfo(secCfgCfm, mBuf) != ROK) {
                                        SPutSBuf(pst->region, pst->pool, (Data *)secCfgCfm, sizeof(UdxSecCfgCfmInfo));
                                        PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ063, (ErrVal)0, "Packing failed");
#endif
                                        RETVALUE(RFAILED);
                                }
                                break;

                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKUNPK(cmUnpkPtr,(PTR *)&secCfgCfm,mBuf);
                                break;
                        }
#endif /* LCUDX */
        }
        PJ_FREE_BUF(mBuf);
        RETVALUE((*func)(pst, suId, secCfgCfm));
}


#ifdef ANSI
PUBLIC S16 cmPkUdxUeIdChgReq
(
Pst * pst,
SpId spId,
U32 transId,
UdxUeInfo * oldUeInfo,
UdxUeInfo * newUeInfo
)
#else
PUBLIC S16 cmPkUdxUeIdChgReq(pst, spId, transId, oldUeInfo, newUeInfo)
Pst * pst;
SpId spId;
U32 transId;
UdxUeInfo * oldUeInfo;
UdxUeInfo * newUeInfo;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxUeIdChgReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX064, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)oldUeInfo, sizeof(UdxUeInfo));
      SPutSBuf(pst->region, pst->pool, (Data *)newUeInfo, sizeof(UdxUeInfo));
      RETVALUE(RFAILED);
   }
   if (cmPkUdxUeInfo(newUeInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EUDX065, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)oldUeInfo, sizeof(UdxUeInfo));
         SPutSBuf(pst->region, pst->pool, (Data *)newUeInfo, sizeof(UdxUeInfo));
         PJ_FREE_BUF(mBuf);
         RETVALUE(RFAILED);
   }

   if (cmPkUdxUeInfo(oldUeInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EUDX067, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)oldUeInfo, sizeof(UdxUeInfo));
         SPutSBuf(pst->region, pst->pool, (Data *)newUeInfo, sizeof(UdxUeInfo));
         PJ_FREE_BUF(mBuf);
         RETVALUE(RFAILED);
   }

   if (SPkU32(transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX069, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)oldUeInfo, sizeof(UdxUeInfo));
      SPutSBuf(pst->region, pst->pool, (Data *)newUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX070, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)oldUeInfo, sizeof(UdxUeInfo));
      SPutSBuf(pst->region, pst->pool, (Data *)newUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Request Structure after packing */
   SPutSBuf(pst->region, pst->pool, (Data *)oldUeInfo, sizeof(UdxUeInfo));
   SPutSBuf(pst->region, pst->pool, (Data *)newUeInfo, sizeof(UdxUeInfo));

   pst->event = (Event) EVTUDXUEIDCHGREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxUeIdChgReq
(
UdxUeIdChgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUeIdChgReq(func, pst, mBuf)
UdxUeIdChgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16 ret1;
   SpId spId;
   U32 transId;
   UdxUeInfo *oldUeInfo;
   UdxUeInfo *newUeInfo;
   
   TRC3(cmUnpkUdxUeIdChgReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX073, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkU32(&transId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX074, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&oldUeInfo,\
                sizeof(UdxUeInfo))) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
             (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }

   if (cmUnpkUdxUeInfo(oldUeInfo, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)oldUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX076, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

   if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&newUeInfo,\
               sizeof(UdxUeInfo))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   if (cmUnpkUdxUeInfo(newUeInfo, mBuf) != ROK)
   {
      SPutSBuf(pst->region, pst->pool, (Data *)oldUeInfo, sizeof(UdxUeInfo));
      SPutSBuf(pst->region, pst->pool, (Data *)newUeInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX078, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, transId, oldUeInfo, newUeInfo));
}


#ifdef ANSI
PUBLIC S16 cmPkUdxUeIdChgCfm
(
Pst * pst,
SuId suId,
U32 transId,
UdxUeInfo  * ueInfo,
CmStatus status
)
#else
PUBLIC S16 cmPkUdxUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst * pst;
SuId suId;
U32 transId;
UdxUeInfo  * ueInfo;
CmStatus status;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxUeIdChgCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ079, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)ueInfo, sizeof(UdxUeInfo));
      RETVALUE(RFAILED);
   }
   if (cmPkCmStatus(&status, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ080, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)ueInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (cmPkUdxUeInfo(ueInfo, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ081, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)ueInfo, sizeof(UdxUeInfo));
         PJ_FREE_BUF(mBuf);
         RETVALUE(RFAILED);
   }

   if (SPkU32(transId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ083, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)ueInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ084, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)ueInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Confirm Structure after packing */
   SPutSBuf(pst->region, pst->pool, (Data *)ueInfo, sizeof(UdxUeInfo));

   pst->event = (Event) EVTCPJUEIDCHGCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxUeIdChgCfm
(
UdxUeIdChgCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUeIdChgCfm(func, pst, mBuf)
UdxUeIdChgCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   U32 transId;
   UdxUeInfo *ueInfo;
   CmStatus status;
   
   TRC3(cmUnpkUdxUeIdChgCfm)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ086, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (SUnpkU32(&transId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ087, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

   if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&ueInfo,\
               sizeof(UdxUeInfo))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   if (cmUnpkUdxUeInfo(ueInfo, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)ueInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ089, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   if (cmUnpkCmStatus(&status, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)ueInfo, sizeof(UdxUeInfo));
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ090, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, transId, ueInfo, status));
}

#ifdef ANSI
PUBLIC S16 cmPkUdxCountReq
(
Pst * pst,
SpId spId,
UdxCountReqInfo * countReq
)
#else
PUBLIC S16 cmPkUdxCountReq(pst, spId, countReq)
Pst * pst;
SpId spId;
UdxCountReqInfo * countReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxCountReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX091, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)countReq, sizeof(UdxCountReqInfo));
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if (cmPkUdxCountReqInfo(countReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX092, (ErrVal)0, "Packing failed");
#endif
                                   SPutSBuf(pst->region, pst->pool, (Data *)countReq, sizeof(UdxCountReqInfo));
                                   PJ_FREE_BUF(mBuf);
                                   RETVALUE(RFAILED);
                           }
                           /* Free Request Structure after packing */
                           SPutSBuf(pst->region, pst->pool, (Data *)countReq, sizeof(UdxCountReqInfo));

                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKPKLOG(cmPkPtr,(PTR)countReq,mBuf,EUDXXXX,pst);
                           break;
                   }
#endif /* LCUDX */
   }

   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX094, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)countReq, sizeof(UdxCountReqInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTUDXCOUNTREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxCountReq
(
UdxCountReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCountReq(func, pst, mBuf)
UdxCountReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   UdxCountReqInfo *countReq;
   
   TRC3(cmUnpkUdxCountReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX096, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&countReq,\
                                                           sizeof(UdxCountReqInfo))) != ROK)
                           {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                   RETVALUE(ret1);
                           }

                           if (cmUnpkUdxCountReqInfo(countReq, mBuf) != ROK) {
                                   SPutSBuf(pst->region, pst->pool, (Data *)countReq, sizeof(UdxCountReqInfo));
                                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX098, (ErrVal)0, "Packing failed");
#endif
                                   RETVALUE(RFAILED);
                           }
                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKUNPK(cmUnpkPtr,(PTR *) &countReq,mBuf);
                           break;
                   }
#endif /* LCUDX */
}

PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, countReq));
}


#ifdef ANSI
PUBLIC S16 cmPkUdxCountCfm
(
Pst * pst,
SuId suId,
UdxCountCfmInfo * countCfm
)
#else
PUBLIC S16 cmPkUdxCountCfm(pst, suId, countCfm)
Pst * pst;
SuId suId;
UdxCountCfmInfo * countCfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxCountCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ099, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)countCfm, sizeof(UdxCountCfmInfo));
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

   if (cmPkUdxCountCfmInfo(countCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
         SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ100, (ErrVal)0, "Packing failed");
#endif
         SPutSBuf(pst->region, pst->pool, (Data *)countCfm, sizeof(UdxCountCfmInfo));
         PJ_FREE_BUF(mBuf);
         RETVALUE(RFAILED);
    }
   /* Free Confirm Structure after packing */
   SPutSBuf(pst->region, pst->pool, (Data *)countCfm, sizeof(UdxCountCfmInfo));

   break;
  }
  case UDX_SEL_LWLC:
                   {
                           CMCHKPKLOG(cmPkPtr,(PTR)countCfm,mBuf,EUDXXXX,pst);
                           break;
                   }
#endif /* LCUDX */
   }

   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ102, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)countCfm, sizeof(UdxCountCfmInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTCPJCOUNTCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxCountCfm
(
UdxCountCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCountCfm(func, pst, mBuf)
UdxCountCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   UdxCountCfmInfo *countCfm;
   
   TRC3(cmUnpkUdxCountCfm)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ104, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {


   if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&countCfm,\
               sizeof(UdxCountCfmInfo))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   if (cmUnpkUdxCountCfmInfo(countCfm, mBuf) != ROK) {
      SPutSBuf(pst->region, pst->pool, (Data *)countCfm, sizeof(UdxCountCfmInfo));
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)ECPJ106, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
         break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKUNPK(cmUnpkPtr,(PTR *) &countCfm,mBuf);
                           break;
                   }
#endif /* LCUDX */
}

PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, countCfm));
}


#ifdef ANSI
PUBLIC S16 cmPkUdxSduStaReq
(
Pst * pst,
SpId spId,
UdxSduStaReqInfo * staReq
)
#else
PUBLIC S16 cmPkUdxSduStaReq(pst, spId, staReq)
Pst * pst;
SpId spId;
UdxSduStaReqInfo * staReq;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxSduStaReq)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX107, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)staReq, sizeof(UdxSduStaReqInfo));
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if (cmPkUdxSduStaReqInfo(staReq, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX108, (ErrVal)0, "Packing failed");
#endif
                                   SPutSBuf(pst->region, pst->pool, (Data *)staReq, sizeof(UdxSduStaReqInfo));
                                   PJ_FREE_BUF(mBuf);
                                   RETVALUE(RFAILED);
                           }
                           /* Free Request Structure after packing */
                           SPutSBuf(pst->region, pst->pool, (Data *)staReq, sizeof(UdxSduStaReqInfo));


                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKPKLOG(cmPkPtr,(PTR)staReq,mBuf,EUDXXXX,pst);
                           break;
                   }
#endif /* LCUDX */
}
   if (SPkS16(spId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX110, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)staReq, sizeof(UdxSduStaReqInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   pst->event = (Event) EVTUDXSDUSTAREQ;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxSduStaReq
(
UdxSduStaReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxSduStaReq(func, pst, mBuf)
UdxSduStaReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId spId;
   UdxSduStaReqInfo *staReq;
   
   TRC3(cmUnpkUdxSduStaReq)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX112, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
           case UDX_SEL_LC:
                   {

                           if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&staReq,\
                                                           sizeof(UdxSduStaReqInfo))) != ROK)
                           {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                                   RETVALUE(ret1);
                           }

                           if (cmUnpkUdxSduStaReqInfo(staReq, mBuf) != ROK) {
                                   SPutSBuf(pst->region, pst->pool, (Data *)staReq, sizeof(UdxSduStaReqInfo));
                                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                                   SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                   __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                   (ErrVal)EUDX114, (ErrVal)0, "Packing failed");
#endif
                                   RETVALUE(RFAILED);
                           }
                           break;
                   }
           case UDX_SEL_LWLC:
                   {
                           CMCHKUNPK(cmUnpkPtr,(PTR *) &staReq,mBuf);
                           break;
                   }
#endif /* LCUDX */
}
PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, spId, staReq));
}


#ifdef ANSI
PUBLIC S16 cmPkUdxSduStaCfm
(
 Pst * pst,
 SuId suId,
 UdxSduStaCfmInfo * staCfm
 )
#else
PUBLIC S16 cmPkUdxSduStaCfm(pst, suId, staCfm)
        Pst * pst;
        SuId suId;
        UdxSduStaCfmInfo * staCfm;
#endif
{
        Buffer *mBuf = NULLP;
        TRC3(cmPkUdxSduStaCfm)

                if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ115, (ErrVal)0, "Packing failed");
#endif
                        SPutSBuf(pst->region, pst->pool, (Data *)staCfm, sizeof(UdxSduStaCfmInfo));
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                        {


                                if (cmPkUdxSduStaCfmInfo(pst, staCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                                        (ErrVal)ECPJ116, (ErrVal)0, "Packing failed");
#endif
                                        SPutSBuf(pst->region, pst->pool, (Data *)staCfm, sizeof(UdxSduStaCfmInfo));
                                        PJ_FREE_BUF(mBuf);
                                        RETVALUE(RFAILED);
                                }
                                break;
                        }
                case UDX_SEL_LWLC:
                        {
                                CMCHKPKLOG(cmPkPtr,(PTR)staCfm,mBuf,EUDXXXX,pst);
                                break;
                        }
#endif /* LCUDX */
        }

        if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                (ErrVal)ECPJ118, (ErrVal)0, "Packing failed");
#endif
                SPutSBuf(pst->region, pst->pool, (Data *)staCfm, sizeof(UdxSduStaCfmInfo));
                PJ_FREE_BUF(mBuf);
                RETVALUE(RFAILED);
        }

        /* Free Confirm Structure after packing */
        /*SPutSBuf(pst->region, pst->pool, (Data *)staCfm, sizeof(UdxSduStaCfmInfo));*/

        pst->event = (Event) EVTUDXSDUSTACFM;
        RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxSduStaCfm
(
 UdxSduStaCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkUdxSduStaCfm(func, pst, mBuf)
        UdxSduStaCfm func;
        Pst *pst;
        Buffer *mBuf;
#endif
{
        S16   ret1;
        SuId suId;
        UdxSduStaCfmInfo *staCfm;

        TRC3(cmUnpkUdxSduStaCfm)

                if (SUnpkS16(&suId, mBuf) != ROK) {
                   PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                        __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                        (ErrVal)ECPJ120, (ErrVal)0, "Packing failed");
#endif
                        RETVALUE(RFAILED);
                }
        switch(pst->selector)
        {
#ifdef LCUDX
                case UDX_SEL_LC:
                {
                    if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&staCfm,\
                                        sizeof(UdxSduStaCfmInfo))) != ROK)
                    {
#if (ERRCLASS & ERRCLS_ADD_RES)
                        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                  __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                 (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
                        RETVALUE(ret1);
                    }

                    if (cmUnpkUdxSduStaCfmInfo(pst, staCfm, mBuf) != ROK) {
                            SPutSBuf(pst->region, pst->pool, (Data *)staCfm, sizeof(UdxSduStaCfmInfo));
                            PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
                            SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                                            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                                            (ErrVal)ECPJ122, (ErrVal)0, "Packing failed");
#endif
                            RETVALUE(RFAILED);
                    }
                }
                break;

                case UDX_SEL_LWLC:
                {
                        CMCHKUNPK(cmUnpkPtr,(PTR *) &staCfm,mBuf);
                }
                break;
        }
#endif /* LCUDX */


        PJ_FREE_BUF(mBuf);
        RETVALUE((*func)(pst, suId, staCfm));
}

#ifdef ANSI
PUBLIC S16 cmPkUdxDatResumeCfm
(
 Pst * pst,
 SuId suId,
 UdxDatResumeCfmInfo * datResumeCfm
 )
#else
PUBLIC S16 cmPkUdxDatResumeCfm(pst, suId, datResumeCfm)
   Pst * pst;
   SuId suId;
   UdxDatResumeCfmInfo * datResumeCfm;
#endif
{
   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxDatResumeCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ115, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)datResumeCfm, sizeof(UdxDatResumeCfmInfo));
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            if (cmPkUdxDatResumeCfmInfo(pst, datResumeCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ116, (ErrVal)0, "Packing failed");
#endif
               SPutSBuf(pst->region, pst->pool, (Data *)datResumeCfm, sizeof(UdxDatResumeCfmInfo));
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }
            break;
         }
      case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)datResumeCfm,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }

   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ118, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)datResumeCfm, sizeof(UdxDatResumeCfmInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Confirm Structure after packing */
   /*SPutSBuf(pst->region, pst->pool, (Data *)datResumeCfm, sizeof(UdxSduStaCfmInfo));*/

   pst->event = (Event) EVTUDXDATRESUMECFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

/*
*
*    Fun:    cmPkUdxDatForwardCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxDatForwardCfm
(
 Pst * pst,
 SuId suId,
 UdxDatFwdCfmInfo  * datFwdCfm
 )
#else
PUBLIC S16 cmPkUdxDatForwardCfm(pst, suId, datFwdCfm)
   Pst * pst;
   SuId suId;
   UdxDatFwdCfmInfo  * datFwdCfm;
#endif
{

   Buffer *mBuf = NULLP;
   TRC3(cmPkUdxDatForwardCfm)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ115, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdCfm, sizeof(UdxDatFwdCfmInfo));
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            if (cmPkUdxDatForwardCfmInfo(pst, datFwdCfm, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ116, (ErrVal)0, "Packing failed");
#endif
               SPutSBuf(pst->region, pst->pool, (Data *)datFwdCfm, sizeof(UdxDatFwdCfmInfo));
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }
            break;
         }
      case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)datFwdCfm,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }

   if (SPkS16(suId, mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ118, (ErrVal)0, "Packing failed");
#endif
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdCfm, sizeof(UdxDatFwdCfmInfo));
      PJ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free Confirm Structure after packing */
   /*SPutSBuf(pst->region, pst->pool, (Data *)datFwdCfm, sizeof(UdxDatFwdCfmInfo));*/

   pst->event = (Event) EVTUDXDATFRWDCFM;
   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxDatResumeCfm
(
 UdxDatResumeCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkUdxDatResumeCfm(func, pst, mBuf)
   UdxDatResumeCfm func;
   Pst *pst;
   Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   UdxDatResumeCfmInfo *datResumeCfm;

   TRC3(cmUnpkUdxDatResumeCfm)

   RLOG0(L_DEBUG," Unpacking UdxDatResumeCfm");
   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ120, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&datResumeCfm,\
                        sizeof(UdxDatResumeCfmInfo))) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               RETVALUE(ret1);
            }

            if (cmUnpkUdxDatResumeCfmInfo(pst, datResumeCfm, mBuf) != ROK) {
               SPutSBuf(pst->region, pst->pool, (Data *)datResumeCfm, sizeof(UdxDatResumeCfmInfo));
               PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ122, (ErrVal)0, "Packing failed");
#endif
               RETVALUE(RFAILED);
            }
         }
         break;

      case UDX_SEL_LWLC:
         {
            CMCHKUNPK(cmUnpkPtr,(PTR *) &datResumeCfm,mBuf);
         }
         break;
   }
#endif /* LCUDX */


   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, datResumeCfm));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxDatForwardCfm
(
 UdxDatForwardCfm func,
 Pst *pst,
 Buffer *mBuf
 )
#else
PUBLIC S16 cmUnpkUdxDatForwardCfm(func, pst, mBuf)
   UdxDatForwardCfm func;
   Pst *pst;
   Buffer *mBuf;
#endif
{
   S16   ret1;
   SuId suId;
   UdxDatFwdCfmInfo *datFwdCfm;

   TRC3(cmUnpkUdxDatForwardCfm)

   if (SUnpkS16(&suId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECPJ120, (ErrVal)0, "Packing failed");
#endif
      RETVALUE(RFAILED);
   }
   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&datFwdCfm,\
                        sizeof(UdxDatFwdCfmInfo))) != ROK)
            {
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
               RETVALUE(ret1);
            }

            if (cmUnpkUdxDatForwardCfmInfo(pst, datFwdCfm, mBuf) != ROK) {
               SPutSBuf(pst->region, pst->pool, (Data *)datFwdCfm, sizeof(UdxDatFwdCfmInfo));
               PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)ECPJ122, (ErrVal)0, "Packing failed");
#endif
               RETVALUE(RFAILED);
            }
         }
         break;

      case UDX_SEL_LWLC:
         {
            CMCHKUNPK(cmUnpkPtr,(PTR *) &datFwdCfm,mBuf);
         }
         break;
   }
#endif /* LCUDX */

   PJ_FREE_BUF(mBuf);
   RETVALUE((*func)(pst, suId, datFwdCfm));
}



/***********************************************************
*
*     Func : cmPkUdxRlcAm
*
*
*     Desc :  * RLC parameter information in AM mode.
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
PUBLIC S16 cmPkUdxRlcAm
(
UdxRlcAm *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxRlcAm(param, mBuf)
UdxRlcAm *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxRlcAm)

   CMCHKPK(SPkU8, param->statusRepReqd, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxRlcAm
*
*
*     Desc :  * RLC parameter information in AM mode.
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
PUBLIC S16 cmUnpkUdxRlcAm
(
UdxRlcAm *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxRlcAm(param, mBuf)
UdxRlcAm *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxRlcAm)

   CMCHKUNPK(SUnpkU8, &param->statusRepReqd, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxRlcUm
*
*
*     Desc : * RLC parameter information in UM mode.
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
PUBLIC S16 cmPkUdxRlcUm
(
UdxRlcUm *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxRlcUm(param, mBuf)
UdxRlcUm *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxRlcUm)

   CMCHKPK(SPkU8, param->pdcpSNSize, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxRlcUm
*
*
*     Desc : * RLC parameter information in UM mode.
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
PUBLIC S16 cmUnpkUdxRlcUm
(
UdxRlcUm *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxRlcUm(param, mBuf)
UdxRlcUm *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxRlcUm)

   CMCHKUNPK(SUnpkU8, &param->pdcpSNSize, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxRlcInfo
*
*
*     Desc : * Mapped RLC entity information in RLC for the PDCP entity
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
PUBLIC S16 cmPkUdxRlcInfo
(
UdxRlcInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxRlcInfo(param, mBuf)
UdxRlcInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxRlcInfo)

      switch(param->mode) {
         case CM_LTE_MODE_UM:
            CMCHKPK(cmPkUdxRlcUm, &param->m.rlcUm, mBuf);
            break;
         case CM_LTE_MODE_AM:
            CMCHKPK(cmPkUdxRlcAm, &param->m.rlcAm, mBuf);
            break;
         default :
            RETVALUE(RFAILED);
      }
   CMCHKPK(SPkU8, param->mode, mBuf);
   CMCHKPK(SPkU8, param->dir, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxRlcInfo
*
*
*     Desc : * Mapped RLC entity information in RLC for the PDCP entity
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
PUBLIC S16 cmUnpkUdxRlcInfo
(
UdxRlcInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxRlcInfo(param, mBuf)
UdxRlcInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxRlcInfo)

   CMCHKUNPK(SUnpkU8, &param->dir, mBuf);
   CMCHKUNPK(SUnpkU8, &param->mode, mBuf);
      switch(param->mode) {
         case CM_LTE_MODE_AM:
            CMCHKUNPK(cmUnpkUdxRlcAm, &param->m.rlcAm, mBuf);
            break;
         case CM_LTE_MODE_UM:
            CMCHKUNPK(cmUnpkUdxRlcUm, &param->m.rlcUm, mBuf);
            break;
         default :
            RETVALUE(RFAILED);
      }
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxCipherInfo
*
*
*     Desc : * Ciphering information to be configured per UE in PDCP.
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
PUBLIC S16 cmPkUdxCipherInfo
(
UdxCipherInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxCipherInfo(param, mBuf)
UdxCipherInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkUdxCipherInfo)

   CMCHKPK(SPkU8, param->algoType, mBuf);
   for (i=UDX_CIPHER_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->usrplnKey[i], mBuf);
   }
   for (i=UDX_CIPHER_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->ctrlplnKey[i], mBuf);
   }
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxCipherInfo
*
*
*     Desc : * Ciphering information to be configured per UE in PDCP.
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
PUBLIC S16 cmUnpkUdxCipherInfo
(
UdxCipherInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCipherInfo(param, mBuf)
UdxCipherInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkUdxCipherInfo)

   for (i=0; i<UDX_CIPHER_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->ctrlplnKey[i], mBuf);
   }
   for (i=0; i<UDX_CIPHER_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->usrplnKey[i], mBuf);
   }
   CMCHKUNPK(SUnpkU8, &param->algoType, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxIntegInfo
*
*
*     Desc : * Integrity protection paramater information.
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
PUBLIC S16 cmPkUdxIntegInfo
(
UdxIntegInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxIntegInfo(param, mBuf)
UdxIntegInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkUdxIntegInfo)

   for (i=UDX_INTG_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->intKey[i], mBuf);
   }
   CMCHKPK(SPkU8, param->algoType, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxIntegInfo
*
*
*     Desc : * Integrity protection paramater information.
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
PUBLIC S16 cmUnpkUdxIntegInfo
(
UdxIntegInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxIntegInfo(param, mBuf)
UdxIntegInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkUdxIntegInfo)

   CMCHKUNPK(SUnpkU8, &param->algoType, mBuf);
   for (i=0; i<UDX_INTG_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->intKey[i], mBuf);
   }
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxSecCfg
*
*
*     Desc : * PDCP Security configuration parameter.
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
PUBLIC S16 cmPkUdxSecCfg
(
UdxSecCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxSecCfg(param, mBuf)
UdxSecCfg *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxSecCfg)

   CMCHKPK(cmPkUdxCipherInfo, &param->cipherInfo, mBuf);
   CMCHKPK(cmPkUdxIntegInfo, &param->integInfo, mBuf);
   CMCHKPK(SPkU8, param->selSecAct, mBuf);
   CMCHKPK(SPkU8, param->isHo, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxSecCfg
*
*
*     Desc : * PDCP Security configuration parameter.
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
PUBLIC S16 cmUnpkUdxSecCfg
(
UdxSecCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxSecCfg(param, mBuf)
UdxSecCfg *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxSecCfg)

   CMCHKUNPK(SUnpkU8, &param->isHo, mBuf);
   CMCHKUNPK(SUnpkU8, &param->selSecAct, mBuf);
   CMCHKUNPK(cmUnpkUdxIntegInfo, &param->integInfo, mBuf);
   CMCHKUNPK(cmUnpkUdxCipherInfo, &param->cipherInfo, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxRohcInfo
*
*
*     Desc : * ROHC compression protocol information structure.
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
PUBLIC S16 cmPkUdxRohcInfo
(
UdxRohcInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxRohcInfo(param, mBuf)
UdxRohcInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkUdxRohcInfo)

   for (i=UDX_MAX_ROHC_PROF_LIST-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->profLst[i], mBuf);
   }
   CMCHKPK(SPkU16, param->maxCid, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxRohcInfo
*
*
*     Desc : * ROHC compression protocol information structure.
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
PUBLIC S16 cmUnpkUdxRohcInfo
(
UdxRohcInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxRohcInfo(param, mBuf)
UdxRohcInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkUdxRohcInfo)

   CMCHKUNPK(SUnpkU16, &param->maxCid, mBuf);
   for (i=0; i<UDX_MAX_ROHC_PROF_LIST; i++) {
      CMCHKUNPK(SUnpkU8, &param->profLst[i], mBuf);
   }
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxHdrCompCfg
*
*
*     Desc : * ROHC configuration information for PDCP entity.
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
PUBLIC S16 cmPkUdxHdrCompCfg
(
UdxHdrCompCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxHdrCompCfg(param, mBuf)
UdxHdrCompCfg *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxHdrCompCfg)

   CMCHKPK(cmPkUdxRohcInfo, &param->rohcInfo, mBuf);
   CMCHKPK(SPkU8, param->hdrCompUsed, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxHdrCompCfg
*
*
*     Desc : * ROHC configuration information for PDCP entity.
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
PUBLIC S16 cmUnpkUdxHdrCompCfg
(
UdxHdrCompCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxHdrCompCfg(param, mBuf)
UdxHdrCompCfg *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxHdrCompCfg)

   CMCHKUNPK(SUnpkU8, &param->hdrCompUsed, mBuf);
   CMCHKUNPK(cmUnpkUdxRohcInfo, &param->rohcInfo, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxHoInfo
*
*
*     Desc : * Handover Information for RB.
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
PUBLIC S16 cmPkUdxHoInfo
(
Pst *pst,
UdxHoInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxHoInfo(pst, param, mBuf)
Pst *pst;
UdxHoInfo *param;
Buffer *mBuf;
#endif
{
   S16   cnt, idx;

   TRC3(cmPkUdxHoInfo)

   if (param->numBits % 8 == 0)
      cnt = param->numBits/8;
   else
      cnt = param->numBits/8 + 1;

   for (idx = cnt-1; idx >= 0; idx--)
   {
      CMCHKPK(SPkU8, param->ulBitMap[idx], mBuf);
   }

   /* Free ulBitMap after packing */
   if (cnt)
      SPutSBuf(pst->region, pst->pool, (Data *)param->ulBitMap, cnt * sizeof(U8));

   CMCHKPK(SPkU8, param->numBits, mBuf);
   CMCHKPK(SPkU32, param->ulCount, mBuf);
   CMCHKPK(SPkU32, param->dlCount, mBuf);
   CMCHKPK(SPkU8, param->isHoPres, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxHoInfo
*
*
*     Desc : * Handover Information for RB.
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
PUBLIC S16 cmUnpkUdxHoInfo
(
Pst *pst,
UdxHoInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxHoInfo(pst, param, mBuf)
Pst *pst;
UdxHoInfo *param;
Buffer *mBuf;
#endif
{
   S16   cnt, idx;
   S16   ret1 = ROK;

   TRC3(cmUnpkUdxHoInfo)

   CMCHKUNPK(SUnpkU8, &param->isHoPres, mBuf);
   CMCHKUNPK(SUnpkU32, &param->dlCount, mBuf);
   CMCHKUNPK(SUnpkU32, &param->ulCount, mBuf);
   CMCHKUNPK(SUnpkU8, &param->numBits, mBuf);

   if (param->numBits % 8 == 0)
      cnt = param->numBits/8;
   else
      cnt = param->numBits/8 + 1;

   if(cnt && (ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&param->ulBitMap,\
               cnt * sizeof(U8))) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
      RETVALUE(ret1);
   }

   if (cnt == 0)
      param->ulBitMap = NULLP;

   for (idx = 0; idx < cnt; idx++)
   {
      CMCHKUNPK(SUnpkU8, &param->ulBitMap[idx], mBuf);
   }

   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxCfgEnt
*
*
*     Desc : * Configuration structure for PDCP entity.
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
PUBLIC S16 cmPkUdxCfgEnt
(
Pst *pst,
UdxCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxCfgEnt(pst, param, mBuf)
Pst *pst;
UdxCfgEnt *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkUdxCfgEnt)

      switch(param->cfgType) {
         case UDX_CFG_MODIFY:
            if (cmPkUdxModCfgEnt(pst, &param->m.modCfg, mBuf) != ROK)
               RETVALUE(RFAILED);
            break;
         case UDX_CFG_ADD:
            CMCHKPK(cmPkUdxAddCfgEnt, &param->m.addCfg, mBuf);
            break;
      }
   CMCHKPK(SPkU8, param->cfgType, mBuf);
   CMCHKPK(SPkU8, param->rbType, mBuf);
   CMCHKPK(SPkU8, param->rbId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxCfgEnt
*
*
*     Desc : * Configuration structure for PDCP entity.
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
PUBLIC S16 cmUnpkUdxCfgEnt
(
Pst *pst,
UdxCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCfgEnt(pst, param, mBuf)
Pst *pst;
UdxCfgEnt *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkUdxCfgEnt)

   CMCHKUNPK(SUnpkU8, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->rbType, mBuf);
   CMCHKUNPK(SUnpkU8, &param->cfgType, mBuf);
      switch(param->cfgType) {
         case UDX_CFG_ADD:
            CMCHKUNPK(cmUnpkUdxAddCfgEnt, &param->m.addCfg, mBuf);
            break;
         case UDX_CFG_MODIFY:
            if (cmUnpkUdxModCfgEnt(pst, &param->m.modCfg, mBuf) != ROK)
               RETVALUE(RFAILED);
            break;
      }
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmPkUdxAddCfgEnt
(
UdxAddCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxAddCfgEnt(param, mBuf)
UdxAddCfgEnt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxAddCfgEnt)

   CMCHKPK(cmPkUdxHdrCompCfg, &param->hdrCompCfg, mBuf);
   CMCHKPK(SPkS16, param->discardTmr, mBuf);
   CMCHKPK(cmPkUdxRlcInfo, &param->rlcInfo, mBuf);
   /* cpj_c_001.main_4 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   CMCHKPK(SPkU8, param->qci, mBuf);
#endif /* LTE_L2_MEAS */
   CMCHKPK(SPkU8, param->cfmReqd, mBuf);
   CMCHKPK(SPkU8, param->discReqd, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmUnpkUdxAddCfgEnt
(
UdxAddCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxAddCfgEnt(param, mBuf)
UdxAddCfgEnt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxAddCfgEnt)

   CMCHKUNPK(SUnpkU8, &param->discReqd, mBuf);
   CMCHKUNPK(SUnpkU8, &param->cfmReqd, mBuf);
   /* cpj_c_001.main_4 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   CMCHKUNPK(SUnpkU8, &param->qci, mBuf);
#endif /* LTE_L2_MEAS */
   CMCHKUNPK(cmUnpkUdxRlcInfo, &param->rlcInfo, mBuf);
   CMCHKUNPK(SUnpkS16, &param->discardTmr, mBuf);
   CMCHKUNPK(cmUnpkUdxHdrCompCfg, &param->hdrCompCfg, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmPkUdxModCfgEnt
(
Pst *pst,
UdxModCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxModCfgEnt(pst, param, mBuf)
Pst *pst;
UdxModCfgEnt *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkUdxModCfgEnt)

   if (cmPkUdxHoInfo(pst, &param->hoInfo, mBuf) != ROK)
      RETVALUE(RFAILED);
   CMCHKPK(cmPkUdxHdrCompCfg, &param->hdrCompCfg, mBuf);
   CMCHKPK(SPkU8, param->cfmReqd, mBuf);
   CMCHKPK(SPkU8, param->discReqd, mBuf);
   CMCHKPK(SPkU8, param->bitFlag, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmUnpkUdxModCfgEnt
(
Pst *pst,
UdxModCfgEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxModCfgEnt(pst, param, mBuf)
Pst *pst;
UdxModCfgEnt *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkUdxModCfgEnt)

   CMCHKUNPK(SUnpkU8, &param->bitFlag, mBuf);
   CMCHKUNPK(SUnpkU8, &param->discReqd, mBuf);
   CMCHKUNPK(SUnpkU8, &param->cfmReqd, mBuf);
   CMCHKUNPK(cmUnpkUdxHdrCompCfg, &param->hdrCompCfg, mBuf);
   if (cmUnpkUdxHoInfo(pst, &param->hoInfo, mBuf) != ROK)
      RETVALUE(RFAILED);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxCfgReqInfo
*
*
*     Desc : * PDCP Configuration structure. Parameters are filled by RRC 
   layer while configuring the PDCP layer.
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
PUBLIC S16 cmPkUdxCfgReqInfo
(
Pst *pst,
UdxCfgReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxCfgReqInfo(pst, param, mBuf)
Pst *pst;
UdxCfgReqInfo *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmPkUdxCfgReqInfo)

   for (i=param->numEnt-1; i >= 0; i--) {
      if (cmPkUdxCfgEnt(pst, &param->cfgEnt[i], mBuf) != ROK)
         RETVALUE(RFAILED);
   }
   CMCHKPK(SPkU8, param->numEnt, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}

/*
*
*    Fun:    cmPkUdxUlStaRepInfo
*
*    Desc:    pack the structure UdxUlStaRepInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     pj_udx.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxUlStaRepInfo
(
UdxUlStaRepInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmPkUdxUlStaRepInfo(param, pst, mBuf)
UdxUlStaRepInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   S16 msgLen;
   TRC3(cmPkUdxUlStaRepInfo)

   CMCHKPK(SPkU8, param->pdcpId.rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->pdcpId.rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->pdcpId.ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(param->sta, &msgLen) != ROK)
      RETVALUE(RFAILED);
   if (SCatMsg(mBuf, param->sta, M1M2) != ROK)
      RETVALUE(RFAILED);
   PJ_FREE_BUF(mBuf);
   CMCHKPK(SPkS16, msgLen, mBuf);

   RETVALUE(ROK);
} /* cmPkUdxUlStaRepInfo */

#ifdef ANSI
PUBLIC S16 cmUnpkUdxUlStaRepInfo
(
Pst *pst,
UdxUlStaRepInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUlStaRepInfo(pst, param, mBuf)
Pst *pst;
UdxUlStaRepInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen, totalMsgLen;

   TRC3(cmUnpkUdxUlStaRepInfo);

   CMCHKUNPK(SUnpkU8, &param->pdcpId.rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->pdcpId.rbId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->pdcpId.ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);

   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->sta) != ROK)
      RETVALUE(RFAILED);   

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxUlFdbkPktInfo
(
Pst *pst,
UdxUlFdbkPktInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUlFdbkPktInfo(pst, param, mBuf)
Pst *pst;
UdxUlFdbkPktInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen, totalMsgLen;

   TRC3(cmUnpkUdxUlFdbkPktInfo);

   CMCHKUNPK(SUnpkU8, &param->pdcpId.rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->pdcpId.rbId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->pdcpId.ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);

   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->fbPkt) != ROK)
      RETVALUE(RFAILED);   

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxDlFdbkPktInfo
(
Pst *pst,
UdxDlFdbkPktInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxDlFdbkPktInfo(pst, param, mBuf)
Pst *pst;
UdxDlFdbkPktInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen, totalMsgLen;

   TRC3(cmUnpkUdxDlFdbkPktInfo);

   CMCHKUNPK(SUnpkU8, &param->pdcpId.rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->pdcpId.rbId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->pdcpId.ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);

   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->fbPkt) != ROK)
      RETVALUE(RFAILED);   

   RETVALUE(ROK);
}

/*
*
*    Fun:    cmPkUdxDlStaRepInfo
*
*    Desc:    pack the structure UdxDlStaRepInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     pj_udx.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxDlStaRepInfo
(
UdxDlStaRepInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmPkUdxDlStaRepInfo(param, pst, mBuf)
UdxDlStaRepInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   S16 msgLen;
   TRC3(cmPkUdxDlStaRepInfo)

   CMCHKPK(SPkU8, param->pdcpId.rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->pdcpId.rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->pdcpId.ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->pdcpId.cellId, mBuf);
   CMCHKPK(SPkU16, param->fms, mBuf);
   
   if (SFndLenMsg(param->staPdu, &msgLen) != ROK)
      RETVALUE(RFAILED);
   if (SCatMsg(mBuf, param->staPdu, M1M2) != ROK)
      RETVALUE(RFAILED);
   PJ_FREE_BUF(mBuf);
   CMCHKPK(SPkS16, msgLen, mBuf);

   RETVALUE(ROK);
} /* cmPkUdxUlStaRepInfo */

#ifdef ANSI
PUBLIC S16 cmUnpkUdxDlStaRepInfo
(
Pst *pst,
UdxDlStaRepInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxDlStaRepInfo(pst, param, mBuf)
Pst *pst;
UdxDlStaRepInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen, totalMsgLen;

   TRC3(cmUnpkUdxDlStaRepInfo);

   CMCHKUNPK(SUnpkU8, &param->pdcpId.rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->pdcpId.rbId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->pdcpId.ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->pdcpId.cellId, mBuf);
   CMCHKPK(SUnpkU16, &param->fms, mBuf);

   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);

   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->staPdu) != ROK)
      RETVALUE(RFAILED);   

   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxCfgReqInfo
*
*
*     Desc : * PDCP Configuration structure. Parameters are filled by RRC 
   layer while configuring the PDCP layer.
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
PUBLIC S16 cmUnpkUdxCfgReqInfo
(
Pst *pst,
UdxCfgReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCfgReqInfo(pst, param, mBuf)
Pst *pst;
UdxCfgReqInfo *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmUnpkUdxCfgReqInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->numEnt, mBuf);
   for (i=0; i<param->numEnt; i++) {
      if (cmUnpkUdxCfgEnt(pst, &param->cfgEnt[i], mBuf) != ROK)
         RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxCfmEnt
*
*
*     Desc : * PDCP confirm parameter structure for PDCP entity.
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
PUBLIC S16 cmPkUdxCfmEnt
(
UdxCfmEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxCfmEnt(param, mBuf)
UdxCfmEnt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxCfmEnt)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   CMCHKPK(SPkU8, param->rbType, mBuf);
   CMCHKPK(SPkU8, param->rbId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxCfmEnt
*
*
*     Desc : * PDCP confirm parameter structure for PDCP entity.
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
PUBLIC S16 cmUnpkUdxCfmEnt
(
UdxCfmEnt *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCfmEnt(param, mBuf)
UdxCfmEnt *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxCfmEnt)

   CMCHKUNPK(SUnpkU8, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->rbType, mBuf);
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxCfgCfmInfo
*
*
*     Desc : * PDCP configuration confirm structure.
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
PUBLIC S16 cmPkUdxCfgCfmInfo
(
UdxCfgCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxCfgCfmInfo(param, mBuf)
UdxCfgCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkUdxCfgCfmInfo)

   for (i=param->numEnt-1; i >= 0; i--) {
      CMCHKPK(cmPkUdxCfmEnt, &param->cfmEnt[i], mBuf);
   }
   CMCHKPK(SPkU8, param->numEnt, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxCfgCfmInfo
*
*
*     Desc : * PDCP configuration confirm structure.
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
PUBLIC S16 cmUnpkUdxCfgCfmInfo
(
UdxCfgCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCfgCfmInfo(param, mBuf)
UdxCfgCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkUdxCfgCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->numEnt, mBuf);
   for (i=0; i<param->numEnt; i++) {
      CMCHKUNPK(cmUnpkUdxCfmEnt, &param->cfmEnt[i], mBuf);
   }
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxReEstReqInfo
*
*
*     Desc : * Reestabishment request structure for SRB1 of an UE.
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
PUBLIC S16 cmPkUdxReEstReqInfo
(
UdxReEstReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxReEstReqInfo(param, mBuf)
UdxReEstReqInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkUdxReEstReqInfo)

   for (i=UDX_CIPHER_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->usrplnKey[i], mBuf);
   }
   for (i=UDX_CIPHER_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->ctrlplnKey[i], mBuf);
   }
   for (i=UDX_INTG_KEY_LEN-1; i >= 0; i--) {
      CMCHKPK(SPkU8, param->intKey[i], mBuf);
   }
   CMCHKPK(SPkU32, param->transId, mBuf);

   CMCHKPK(cmPkLtePdcpId, &param->pdcpId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxReEstReqInfo
*
*
*     Desc : * Reestabishment request structure for SRB1 of an UE.
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
PUBLIC S16 cmUnpkUdxReEstReqInfo
(
UdxReEstReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxReEstReqInfo(param, mBuf)
UdxReEstReqInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkUdxReEstReqInfo)

   CMCHKUNPK(cmUnpkLtePdcpId, &param->pdcpId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   for (i=0; i<UDX_INTG_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->intKey[i], mBuf);
   }
   for (i=0; i<UDX_CIPHER_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->ctrlplnKey[i], mBuf);
   }
   for (i=0; i<UDX_CIPHER_KEY_LEN; i++) {
      CMCHKUNPK(SUnpkU8, &param->usrplnKey[i], mBuf);
   }
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxReEstCfmInfo
*
*
*     Desc : * Reestablishment confirmation structure for an UE.
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
PUBLIC S16 cmPkUdxReEstCfmInfo
(
UdxReEstCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxReEstCfmInfo(param, mBuf)
UdxReEstCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxReEstCfmInfo)

   CMCHKPK(SPkS16, param->status, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxReEstCfmInfo
*
*
*     Desc : * Reestablishment confirmation structure for an UE.
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
PUBLIC S16 cmUnpkUdxReEstCfmInfo
(
UdxReEstCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxReEstCfmInfo(param, mBuf)
UdxReEstCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxReEstCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxSecCfgReqInfo
*
*
*     Desc : * PDCP Security Configuration structure. Parameters are filled by RRC
 * while configuring the security parameters.
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
PUBLIC S16 cmPkUdxSecCfgReqInfo
(
UdxSecCfgReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxSecCfgReqInfo(param, mBuf)
UdxSecCfgReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxSecCfgReqInfo)

   CMCHKPK(cmPkUdxSecCfg, &param->secCfg, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxSecCfgReqInfo
*
*
*     Desc : * PDCP Security Configuration structure. Parameters are filled by RRC
 * while configuring the security parameters.
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
PUBLIC S16 cmUnpkUdxSecCfgReqInfo
(
UdxSecCfgReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxSecCfgReqInfo(param, mBuf)
UdxSecCfgReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxSecCfgReqInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(cmUnpkUdxSecCfg, &param->secCfg, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxSecCfgCfmInfo
*
*
*     Desc : * PDCP Security configuration confirm structure.
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
PUBLIC S16 cmPkUdxSecCfgCfmInfo
(
UdxSecCfgCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxSecCfgCfmInfo(param, mBuf)
UdxSecCfgCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxSecCfgCfmInfo)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxSecCfgCfmInfo
*
*
*     Desc : * PDCP Security configuration confirm structure.
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
PUBLIC S16 cmUnpkUdxSecCfgCfmInfo
(
UdxSecCfgCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxSecCfgCfmInfo(param, mBuf)
UdxSecCfgCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxSecCfgCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxUeInfo
*
*
*     Desc : *  UE ID information
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
PUBLIC S16 cmPkUdxUeInfo
(
UdxUeInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxUeInfo(param, mBuf)
UdxUeInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxUeInfo)

   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxUeInfo
*
*
*     Desc : *  UE ID information
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
PUBLIC S16 cmUnpkUdxUeInfo
(
UdxUeInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUeInfo(param, mBuf)
UdxUeInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxUeInfo)

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxCountReqInfo
*
*
*     Desc : *  PDCP Count Request Information from RRC.
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
PUBLIC S16 cmPkUdxCountReqInfo
(
UdxCountReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxCountReqInfo(param, mBuf)
UdxCountReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxCountReqInfo)

   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxCountReqInfo
*
*
*     Desc : *  PDCP Count Request Information from RRC.
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
PUBLIC S16 cmUnpkUdxCountReqInfo
(
UdxCountReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCountReqInfo(param, mBuf)
UdxCountReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxCountReqInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxCountInfo
*
*
*     Desc : *  PDCP Count Information for a DRB.
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
PUBLIC S16 cmPkUdxCountInfo
(
UdxCountInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxCountInfo(param, mBuf)
UdxCountInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxCountInfo)

   CMCHKPK(SPkU32, param->dlCount, mBuf);
   CMCHKPK(SPkU32, param->ulCount, mBuf);
   CMCHKPK(SPkU8, param->dir, mBuf);
   CMCHKPK(SPkU8, param->rbId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxCountInfo
*
*
*     Desc : *  PDCP Count Information for a DRB.
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
PUBLIC S16 cmUnpkUdxCountInfo
(
UdxCountInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCountInfo(param, mBuf)
UdxCountInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxCountInfo)

   CMCHKUNPK(SUnpkU8, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->dir, mBuf);
   CMCHKUNPK(SUnpkU32, &param->ulCount, mBuf);
   CMCHKUNPK(SUnpkU32, &param->dlCount, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxCountCfmInfo
*
*
*     Desc : *  PDCP Count Request Confirmation to RRC.
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
PUBLIC S16 cmPkUdxCountCfmInfo
(
UdxCountCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxCountCfmInfo(param, mBuf)
UdxCountCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmPkUdxCountCfmInfo)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   for (i=param->numRb-1; i >= 0; i--) {
      CMCHKPK(cmPkUdxCountInfo, &param->countInfo[i], mBuf);
   }
   CMCHKPK(SPkU8, param->numRb, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxCountCfmInfo
*
*
*     Desc : *  PDCP Count Request Confirmation to RRC.
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
PUBLIC S16 cmUnpkUdxCountCfmInfo
(
UdxCountCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCountCfmInfo(param, mBuf)
UdxCountCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkUdxCountCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->numRb, mBuf);
   for (i=0; i<param->numRb; i++) {
      CMCHKUNPK(cmUnpkUdxCountInfo, &param->countInfo[i], mBuf);
   }
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxSduStaReqInfo
*
*
*     Desc : *  PDCP SDU Status Request Information from RRC - Used for reestablishment
 *  during handover.
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
PUBLIC S16 cmPkUdxSduStaReqInfo
(
UdxSduStaReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxSduStaReqInfo(param, mBuf)
UdxSduStaReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxSduStaReqInfo)

   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxSduStaReqInfo
*
*
*     Desc : *  PDCP SDU Status Request Information from RRC - Used for reestablishment
 *  during handover.
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
PUBLIC S16 cmUnpkUdxSduStaReqInfo
(
UdxSduStaReqInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxSduStaReqInfo(param, mBuf)
UdxSduStaReqInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxSduStaReqInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxDlSduStaInfo
*
*
*     Desc : *  PDCP DL SDU Status Information for a DRB mapped to RLC AM.
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
PUBLIC S16 cmPkUdxDlSduStaInfo
(
UdxDlSduStaInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxDlSduStaInfo(param, mBuf)
UdxDlSduStaInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkUdxDlSduStaInfo)
                                 
   CMCHKPK(cmPkBool, param->hoPres, mBuf);  
   CMCHKPK(SPkU32, param->count, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxDlSduStaInfo
*
*
*     Desc : *  PDCP DL SDU Status Information for a DRB mapped to RLC AM.
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
PUBLIC S16 cmUnpkUdxDlSduStaInfo
(
UdxDlSduStaInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxDlSduStaInfo(param, mBuf)
UdxDlSduStaInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxDlSduStaInfo)
   
   CMCHKUNPK(cmUnpkBool, &param->hoPres, mBuf); 
   CMCHKUNPK(SUnpkU32, &param->count, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxSduStaInfo
*
*
*     Desc : *  PDCP SDU Status Information for a DRB mapped to RLC AM.
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
PUBLIC S16 cmPkUdxSduStaInfo
(
Pst *pst,
UdxSduStaInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxSduStaInfo(pst, param, mBuf)
Pst *pst;
UdxSduStaInfo *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkUdxSduStaInfo)

   CMCHKPK(cmPkUdxDlSduStaInfo, &param->dlSduStaInfo, mBuf);
   CMCHKPK(SPkU8, param->dir, mBuf);
   CMCHKPK(SPkU8, param->rbId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkUdxSduStaInfo
*
*
*     Desc : *  PDCP SDU Status Information for a DRB mapped to RLC AM.
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
PUBLIC S16 cmUnpkUdxSduStaInfo
(
Pst *pst,
UdxSduStaInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxSduStaInfo(pst, param, mBuf)
Pst *pst;
UdxSduStaInfo *param;
Buffer *mBuf;
#endif
{
   TRC3(cmUnpkUdxSduStaInfo)

   CMCHKUNPK(SUnpkU8, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->dir, mBuf);

   CMCHKUNPK(cmUnpkUdxDlSduStaInfo, &param->dlSduStaInfo, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkUdxSduStaCfmInfo
*
*
*     Desc : *  PDCP SDU Status Request Confirmation for an UE.
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
PUBLIC S16 cmPkUdxSduStaCfmInfo
(
Pst *pst,
UdxSduStaCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxSduStaCfmInfo(pst, param, mBuf)
Pst *pst;
UdxSduStaCfmInfo *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmPkUdxSduStaCfmInfo)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   for (i=param->numRb-1; i >= 0; i--) {
      if (cmPkUdxSduStaInfo(pst, &param->sduStaInfo[i], mBuf) != ROK)
         RETVALUE(RFAILED);
   }
   CMCHKPK(SPkU8, param->numRb, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmPkUdxDatResumeCfmInfo
*
*
*     Desc : *  PDCP SDU Status Request Confirmation for an UE.
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
PUBLIC S16 cmPkUdxDatResumeCfmInfo
(
Pst *pst,
UdxDatResumeCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkUdxDatResumeCfmInfo(pst, param, mBuf)
Pst *pst;
UdxDatResumeCfmInfo *param;
Buffer *mBuf;
#endif
{
   S32 i;

   TRC3(cmPkUdxDatResumeCfmInfo)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   CMCHKPK(SPkU32, param->transId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmPkUdxDatForwardCfmInfo
(
Pst *pst,
UdxDatFwdCfmInfo *param,
Buffer *mBuf
 )
#else
PUBLIC S16 cmPkUdxDatForwardCfmInfo(pst, param, mBuf)
Pst *pst;
UdxDatFwdCfmInfo *param;
Buffer *mBuf;
#endif
{
   TRC3(cmPkUdxDatForwardCfmInfo)

   CMCHKPK(SPkS16, param->reason, mBuf);
   CMCHKPK(SPkS16, param->status, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   RETVALUE(ROK);
}
/*
*
*    Fun:    cmPkUdxDlFdbkPktInfo
*
*    Desc:    pack the structure UdxDlFdbPktInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     pj_udx.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxDlFdbkPktInfo
(
UdxDlFdbkPktInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmPkUdxDlFdbkPktInfo(param, pst, mBuf)
UdxDlFdbkPktInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   S16 msgLen;
   TRC3(cmPkUdxDlFdbkPktInfo)

   CMCHKPK(SPkU8, param->pdcpId.rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->pdcpId.rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->pdcpId.ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(param->fbPkt, &msgLen) != ROK)
      RETVALUE(RFAILED);
   if (SCatMsg(mBuf, param->fbPkt, M1M2) != ROK)
      RETVALUE(RFAILED);
   PJ_FREE_BUF(mBuf);
   CMCHKPK(SPkS16, msgLen, mBuf);

   RETVALUE(ROK);
} /* cmPkUdxDlFdbkPktInfo */


/***********************************************************
*
*     Func : cmUnpkUdxSduStaCfmInfo
*
*
*     Desc : *  PDCP SDU Status Request Confirmation for an UE.
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
PUBLIC S16 cmUnpkUdxSduStaCfmInfo
(
Pst *pst,
UdxSduStaCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxSduStaCfmInfo(pst, param, mBuf)
Pst *pst;
UdxSduStaCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkUdxSduStaCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->numRb, mBuf);
   for (i=0; i<param->numRb; i++) {
      if (cmUnpkUdxSduStaInfo(pst, &param->sduStaInfo[i], mBuf) != ROK)
         RETVALUE(RFAILED);
   }
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}


/***********************************************************
*
*     Func : cmUnpkUdxDatResumeCfmInfo
*
*
*     Desc : *  PDCP SDU Status Request Confirmation for an UE.
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
PUBLIC S16 cmUnpkUdxDatResumeCfmInfo
(
Pst *pst,
UdxDatResumeCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxDatResumeCfmInfo(pst, param, mBuf)
Pst *pst;
UdxDatResumeCfmInfo *param;
Buffer *mBuf;
#endif
{

   S32 i;
   TRC3(cmUnpkUdxDatResumeCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkU32, &param->transId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}
/***********************************************************
*
*     Func : cmUnpkUdxDatForwardCfm
*
*
*     Desc : *  PDCP Data Forward Confirmation for an UE.
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
PUBLIC S16 cmUnpkUdxDatForwardCfmInfo
(
Pst *pst,
UdxDatFwdCfmInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxDatForwardCfmInfo(pst, param, mBuf)
Pst *pst;
UdxDatFwdCfmInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkUdxDatForwardCfmInfo)

   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(SUnpkS16, &param->status, mBuf);
   CMCHKUNPK(SUnpkS16, &param->reason, mBuf);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmPkUdxUlFdbkPkt
(
Pst * pst,
SpId spId,
UdxUlFdbkPktInfo *fdbkPktInfo
)
#else
PUBLIC S16 cmPkUdxUlFdbkPkt(pst, spId, fdbkPktInfo)
Pst * pst;
SpId spId;
UdxUlFdbkPktInfo *fdbkPktInfo;
#endif
{
   S16 ret;
   Buffer *mBuf;
   mBuf = NULLP;

   TRC3(cmPkUdxUlFdbkPkt)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX016, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      SPutSBuf(pst->region, pst->pool, (Data *)fdbkPktInfo, sizeof(UdxUlFdbkPktInfo));

      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            ret = cmPkUdxUlFdbkPktInfo( (fdbkPktInfo), pst, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
            if(ret != ROK)
            {
               PJ_FREE_BUF(mBuf);
               PJ_FREE_BUF(fdbkPktInfo->fbPkt);
               SPutSBuf(pst->region, pst->pool, (Data *)fdbkPktInfo, sizeof(UdxUlFdbkPktInfo));
               
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDXXXX, (ErrVal)ret, "Packing failure");
               RETVALUE( ret );
            }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
            PJ_FREE_BUF(fdbkPktInfo->fbPkt);
            if (SPutSBuf(pst->region, pst->pool, (Data *)fdbkPktInfo,
                     sizeof(UdxUlFdbkPktInfo)) != ROK)
            {
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }

            break;
         }
       case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)fdbkPktInfo,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }
   
   CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
   pst->event = (Event) EVTUDXULROHCFDB;

   RETVALUE(SPstTsk(pst,mBuf));
}

#ifdef ANSI
PUBLIC S16 cmPkUdxDlFdbkPkt
(
Pst * pst,
SpId spId,
UdxDlFdbkPktInfo *fdbkPktInfo
)
#else
PUBLIC S16 cmPkUdxDlFdbkPkt(pst, spId, fdbkPktInfo)
Pst * pst;
SpId spId;
UdxDlFdbkPktInfo *fdbkPktInfo;
#endif
{
   S16 ret;
   Buffer *mBuf;
   mBuf = NULLP;

   TRC3(cmPkUdxDlFdbkPkt)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX016, (ErrVal)0, "Packing failed");
#endif
      PJ_FREE_BUF(mBuf);
      SPutSBuf(pst->region, pst->pool, (Data *)fdbkPktInfo, sizeof(UdxUlFdbkPktInfo));

      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
      case UDX_SEL_LC:
         {
            ret = cmPkUdxDlFdbkPktInfo( (fdbkPktInfo), pst, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
            if(ret != ROK)
            {
               PJ_FREE_BUF(mBuf);
               PJ_FREE_BUF(fdbkPktInfo->fbPkt);
               SPutSBuf(pst->region, pst->pool, (Data *)fdbkPktInfo, sizeof(UdxDlFdbkPktInfo));
               
               SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                     (ErrVal)EUDXXXX, (ErrVal)ret, "Packing failure");
               RETVALUE( ret );
            }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
            PJ_FREE_BUF(fdbkPktInfo->fbPkt);
            if (SPutSBuf(pst->region, pst->pool, (Data *)fdbkPktInfo,
                     sizeof(UdxDlFdbkPktInfo)) != ROK)
            {
               PJ_FREE_BUF(mBuf);
               RETVALUE(RFAILED);
            }

            break;
         }
       case UDX_SEL_LWLC:
         {
            CMCHKPKLOG(cmPkPtr,(PTR)fdbkPktInfo,mBuf,EUDXXXX,pst);
            break;
         }
#endif /* LCUDX */
   }
   
   CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
   pst->event = (Event) EVTUDXDLROHCFDB;

   RETVALUE(SPstTsk(pst,mBuf));
}

/*
*
*    Fun:    cmPkUdxUlFdbkPktInfo
*
*    Desc:    pack the structure UdxUlFdbPktInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     pj_udx.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxUlFdbkPktInfo
(
UdxUlFdbkPktInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmPkUdxUlFdbkPktInfo(param, pst, mBuf)
UdxUlFdbkPktInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   S16 msgLen;
   TRC3(cmPkUdxUlFdbkPktInfo)

   CMCHKPK(SPkU8, param->pdcpId.rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->pdcpId.rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->pdcpId.ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(param->fbPkt, &msgLen) != ROK)
      RETVALUE(RFAILED);
   if (SCatMsg(mBuf, param->fbPkt, M1M2) != ROK)
      RETVALUE(RFAILED);
   PJ_FREE_BUF(param->fbPkt);
   CMCHKPK(SPkS16, msgLen, mBuf);

   RETVALUE(ROK);
} /* cmPkUdxUlFdbkPktInfo */


#ifdef ANSI
PUBLIC S16 cmUnpkUdxDlFdbkPkt
(
UdxDlFdbkPkt func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxDlFdbkPkt(func, pst, mBuf)
UdxDlFdbkPkt func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId  spId;
   UdxDlFdbkPktInfo *fdbkPkt;

   TRC3(cmUnpkUdxDlFdbkPkt)
   
   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX021, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
       case UDX_SEL_LC:
       {        
          if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&fdbkPkt,\
                      sizeof(UdxDlFdbkPktInfo))) != ROK)
          {
#if (ERRCLASS & ERRCLS_ADD_RES)
              SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                    __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                    (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

              PJ_FREE_BUF(mBuf);
             RETVALUE(ret1);
          }

          if (cmUnpkUdxDlFdbkPktInfo(pst, fdbkPkt, mBuf) != ROK) 
          {
             SPutSBuf(pst->region, pst->pool, (Data *)fdbkPkt, sizeof(UdxDlFdbkPktInfo));
             PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
             SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)EUDX023, (ErrVal)0, "Unpacking failed");
#endif
             RETVALUE(RFAILED);
          }

          break;
       }

       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &fdbkPkt, mBuf);
          break;
       }
#endif /* LCUDX */
   }

   PJ_FREE_BUF(mBuf);

   RETVALUE((*func)(pst, spId, fdbkPkt));  
}


#ifdef ANSI
PUBLIC S16 cmUnpkUdxDlFdbPktInfo
(
Pst *pst,
UdxDlFdbkPktInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxDlFdbPktInfo(pst, param, mBuf)
Pst *pst;
UdxDlFdbkPktInfo *param;
Buffer *mBuf;
#endif
{
   MsgLen msgLen, totalMsgLen;

   TRC3(cmUnpkUdxDlFdbPktInfo);

   CMCHKUNPK(SUnpkU8, &param->pdcpId.rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->pdcpId.rbId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->pdcpId.ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->pdcpId.cellId, mBuf);

   if (SFndLenMsg(mBuf, &totalMsgLen) != ROK)
      RETVALUE(RFAILED);

   if (SSegMsg(mBuf, totalMsgLen-msgLen, &param->fbPkt) != ROK)
      RETVALUE(RFAILED);   

   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 cmUnpkUdxUlStaRep
(
UdxUlStaRep func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUlStaRep(func, pst, mBuf)
UdxUlStaRep func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId  spId;
   UdxUlStaRepInfo *staRep;
   
   TRC3(cmUnpkUdxUlStaRep)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX021, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
       {
          if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&staRep,\
                      sizeof(UdxUlStaRepInfo))) != ROK)
          {
#if (ERRCLASS & ERRCLS_ADD_RES)
              SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                    __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                    (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

              PJ_FREE_BUF(mBuf);
             RETVALUE(ret1);
          }

          if (cmUnpkUdxUlStaRepInfo(pst, staRep, mBuf) != ROK) 
          {
             SPutSBuf(pst->region, pst->pool, (Data *)staRep, sizeof(UdxUlStaRepInfo));
             PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
             SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)EUDX023, (ErrVal)0, "Unpacking failed");
#endif
             RETVALUE(RFAILED);
          }

          break;
       }

       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &staRep, mBuf);
          break;
       }
#endif /* LCUDX */
   }

    PJ_FREE_BUF(mBuf);

   RETVALUE((*func)(pst, spId, staRep));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxDlStaRep
(
UdxDlStaRep func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxDlStaRep(func, pst, mBuf)
UdxDlStaRep func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId  spId;
   UdxDlStaRepInfo *staRep;
   
   TRC3(cmUnpkUdxDlStaRep)

   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX021, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
       {
          if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&staRep,\
                      sizeof(UdxDlStaRepInfo))) != ROK)
          {
#if (ERRCLASS & ERRCLS_ADD_RES)
              SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                    __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                    (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

              PJ_FREE_BUF(mBuf);
             RETVALUE(ret1);
          }

          if (cmUnpkUdxDlStaRepInfo(pst, staRep, mBuf) != ROK) 
          {
             SPutSBuf(pst->region, pst->pool, (Data *)staRep, sizeof(UdxDlStaRepInfo));
             PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
             SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)EUDX023, (ErrVal)0, "Unpacking failed");
#endif
             RETVALUE(RFAILED);
          }

          break;
       }

       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &staRep, mBuf);
          break;
       }
#endif /* LCUDX */
   }

    PJ_FREE_BUF(mBuf);

   RETVALUE((*func)(pst, spId, staRep));
}

#ifdef ANSI
PUBLIC S16 cmUnpkUdxUlFdbkPkt
(
UdxUlFdbkPkt func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUlFdbkPkt(func, pst, mBuf)
UdxUlFdbkPkt func;
Pst *pst;
Buffer *mBuf;
#endif
{
   S16   ret1;
   SpId  spId;
   UdxUlFdbkPktInfo *fdbkPkt;

   TRC3(cmUnpkUdxUlFdbkPkt)
   
   if (SUnpkS16(&spId, mBuf) != ROK) {
      PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
      SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
         __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
         (ErrVal)EUDX021, (ErrVal)0, "Unpacking failed");
#endif
      RETVALUE(RFAILED);
   }

   switch(pst->selector)
   {
#ifdef LCUDX
       case UDX_SEL_LC:
       {        
          if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&fdbkPkt,\
                      sizeof(UdxUlFdbkPktInfo))) != ROK)
          {
#if (ERRCLASS & ERRCLS_ADD_RES)
              SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                    __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                    (ErrVal)EUDX040, (ErrVal)0, "SGetMsg() failed");
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

              PJ_FREE_BUF(mBuf);
             RETVALUE(ret1);
          }

          if (cmUnpkUdxUlFdbkPktInfo(pst, fdbkPkt, mBuf) != ROK) 
          {
             SPutSBuf(pst->region, pst->pool, (Data *)fdbkPkt, sizeof(UdxUlFdbkPktInfo));
             PJ_FREE_BUF(mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
             SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)EUDX023, (ErrVal)0, "Unpacking failed");
#endif
             RETVALUE(RFAILED);
          }

          break;
       }

       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &fdbkPkt, mBuf);
          break;
       }
#endif /* LCUDX */
   }

   PJ_FREE_BUF(mBuf);

   RETVALUE((*func)(pst, spId, fdbkPkt));  
}

#ifdef LTE_L2_MEAS
/*
*
*    Fun:    cmPkUdxL2MeasReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxL2MeasReq
(
Pst               *pst,
PjL2MeasReqEvt    *measReqEvt 
)
#else
PUBLIC S16 cmPkUdxL2MeasReq(pst, measReqEvt)
Pst               *pst;
PjL2MeasReqEvt    *measReqEvt; 
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkUdxL2MeasReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       RETVALUE(ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
         CMCHKPK(cmPkPtr,(PTR) measReqEvt, mBuf);
         break;
       }
    }
    pst->event = (Event) UDX_EVT_L2MEAS_REQ;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkUdxStaUpdReq */

/*
*
*    Fun:    cmPkUdxL2MeasReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxL2MeasSendReq
(
Pst              *pst,
U8               measType
)
#else
PUBLIC S16 cmPkUdxL2MeasSendReq(pst, measReqEvt)
Pst               *pst;
U8                measType
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkUdxL2MeasSendReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       RETVALUE(ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKPKLOG(SPkU8, measType, mBuf, EUDXXXX, pst);
          break;
       }
    }
    pst->event = (Event) UDX_EVT_L2MEAS_SEND_REQ;

    RETVALUE(SPstTsk(pst, mBuf));

} 

/*
*
*    Fun:    cmPkUdxL2MeasStopReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxL2MeasStopReq
(
Pst              *pst,
U8               measType
)
#else
PUBLIC S16 cmPkUdxL2MeasStopReq(pst, measType)
Pst               *pst;
U8                measType
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkUdxL2MeasStopReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       RETVALUE(ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKPKLOG(SPkU8, measType, mBuf, EUDXXXX, pst);
          break;
       }
    }
    pst->event = (Event) UDX_EVT_L2MEAS_STOP_REQ;

    RETVALUE(SPstTsk(pst, mBuf));

} 

/*

 
*    Fun:    cmUnpkUdxL2MeasReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxL2MeasReq
(
UdxMeasReq     func,
Pst              *pst,
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkUdxL2MeasReq(func, pst, mBuf)
UdxMeasReq     func;
Pst              *pst;
Buffer           *mBuf;
#endif
{
    S16    ret1;
    PjL2MeasReqEvt    *measReqEvt; 

    TRC3(cmUnpkUdxL2MeasReq)

    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
         CMCHKUNPK(cmUnpkPtr, (PTR *)&measReqEvt,mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, measReqEvt));
} /* cmUnpkUdxL2MeasReq */

/*
*
*    Fun:    cmUnpkUdxL2MeasSendReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxL2MeasSendReq
(
UdxStaPduReq     func,
Pst              *pst,
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkUdxL2MeasSendReq(func, pst, mBuf)
UdxStaPduReq     func;
Pst              *pst;
Buffer           *mBuf;
#endif
{
    S16    ret1;
    U8     measType;

    TRC3(cmUnpkUdxL2MeasSendReq)

    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(SUnpkU8, &measType, mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, measType));
} /* cmUnpkUdxL2MeasReq */

/*
*
*    Fun:    cmUnpkUdxL2MeasStopReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxL2MeasStopReq
(
UdxStaPduReq     func,
Pst              *pst,
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkUdxL2MeasStopReq(func, pst, mBuf)
UdxStaPduReq     func;
Pst              *pst;
Buffer           *mBuf;
#endif
{
    U8     measType;

    TRC3(cmUnpkUdxL2MeasStopReq)

    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(SUnpkU8, &measType, mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, measType));
} /* cmUnpkUdxL2MeasStopReq */
#endif

/*
*
*    Fun:    cmPkUdxDatResumeReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxDatResumeReq
(
Pst           *pst,
CmLteCellId   cellId,
CmLteRnti     ueId
)
#else
PUBLIC S16 cmPkUdxDatResumeReq(pst, cellId, ueId)
Pst           *pst;
CmLteCellId   cellId;
CmLteRnti     ueId;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkUdxDatResume)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       RETVALUE(ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKPK(cmPkLteRnti, ueId, mBuf);
          CMCHKPK(cmPkLteCellId, cellId, mBuf);
          break;
       }
    }
   pst->event = (Event) EVTUDXDATRESUME;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmUnpkUdxDatResumeReq */

/*
*
*    Fun:    cmUnpkUdxDatResumeReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxDatResumeReq
(
UdxStaPduReq     func,
Pst              *pst,
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkUdxDatResumeReq(func, pst, mBuf)
UdxStaPduReq     func;
Pst              *pst;
Buffer           *mBuf;
#endif
{
   CmLteCellId   cellId;
   CmLteRnti     ueId;

    TRC3(cmUnpkUdxDatResumeReq)

    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkLteCellId, &cellId, mBuf);
          CMCHKUNPK(cmUnpkLteRnti, &ueId, mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, cellId, ueId));
} /* cmUnpkUdxDatResumeReq */

/*
*
*    Fun:    cmPkUdxDatForwardReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxDatForwardReq
(
Pst           *pst,
CmLteCellId   cellId,
CmLteRnti     ueId
)
#else
PUBLIC S16 cmPkUdxDatForwardReq(pst, cellId, ueId)
Pst           *pst;
CmLteCellId   cellId;
CmLteRnti     ueId;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkUdxDatForwardReq)

    RLOG0(L_DEBUG,"Packing UdxDatForwadReq");
    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)EUDXXXX, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */

       RETVALUE(ret1);
    }
    
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKPK(cmPkLteRnti, ueId, mBuf);
          CMCHKPK(cmPkLteCellId, cellId, mBuf);
          break;
       }
    }
   pst->event = (Event) EVTUDXDATFRWD;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmUnpkUdxDatForwardReq */

/*
*
*    Fun:    cmUnpkUdxDatForwardReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxDatForwardReq
(
UdxStaPduReq     func,
Pst              *pst,
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkUdxDatForwardReq(func, pst, mBuf)
UdxStaPduReq     func;
Pst              *pst;
Buffer           *mBuf;
#endif
{
   CmLteCellId   cellId;
   CmLteRnti     ueId;

    TRC3(cmUnpkUdxDatForwardReq)

    RLOG0(L_DEBUG,"Unpacking UdxDatForwadReq");
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkLteCellId, &cellId, mBuf);
          CMCHKUNPK(cmUnpkLteRnti, &ueId, mBuf);
         break;
       }
    }

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, cellId, ueId));
} /* cmUnpkUdxDatForwardReq */
#endif /* end of #if(defined(LCUDX) || defined(LWLCUDX)) */

/********************************************************************30**
  
         End of file:     eq_udx.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:52 2016
  
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
/main/1      ---      dm   1. LTE RRC Initial Release.
/main/3      ---      gk   1. LTE PDCP Release 2.1.
/main/4      ---      cpj_c_001.main_3 nm   1. Updated the size for cfgCfm.
/main/5      ---      cpj_c_001.main_4 ap   1. Added support for L2 Measurement.
*********************************************************************91*/

