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
  
        Name:    UDX RRC Control Interface
    
        Type:    C file
  
        Desc:    This file Contains the packing and unpacking functions for
                 UDX Interface
 
        File:    ckw.c

        Sid:      kw_udx.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:26 2014
  
        Prg:     chakrapani
  
*********************************************************************21*/

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
#include "lkw.h"
#include "kw_udx.h"           /* UDX defines */

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
#include "kw.h"
#include "lkw.x"
#include "kw_udx.x"
#include <stdlib.h>


#ifdef LCUDX
#if 0
PRIVATE S16 cmPkUdxTmInfo ARGS((UdxTmInfo   *param,Buffer  *mBuf));
PRIVATE S16 cmPkUdxUmInfo ARGS((UdxEntCfgInfo *param, Buffer *mBuf));
PRIVATE S16 cmPkUdxAmInfo ARGS((UdxAmInfo *param, Buffer *mBuf));
PRIVATE S16 cmPkUdxLChInfo ARGS((UdxLChInfo *param, Buffer *mBuf));
PRIVATE S16 cmPkUdxEntCfgInfo ARGS((UdxEntCfgInfo *param, Buffer *mBuf));
PRIVATE S16 cmPkUdxCfgInfo ARGS((UdxCfgInfo *param, Pst *pst, Buffer *mBuf));
PRIVATE S16 cmPkUdxEntCfgCfmInfo ARGS((UdxEntCfgCfmInfo *param, Buffer *mBuf));

PRIVATE S16 cmUnpkUdxTmInfo ARGS((UdxTmInfo   *param,Buffer  *mBuf));
PRIVATE S16 cmUnpkUdxUmInfo ARGS((UdxEntCfgInfo *param, Buffer *mBuf));
PRIVATE S16 cmUnpkUdxAmInfo ARGS((UdxAmInfo *param, Buffer *mBuf));
PRIVATE S16 cmUnpkUdxLChInfo ARGS((UdxLChInfo *param, Buffer *mBuf));
PRIVATE S16 cmUnpkUdxEntCfgInfo ARGS((UdxEntCfgInfo *param, Buffer *mBuf));
PRIVATE S16 cmUnpkUdxCfgInfo ARGS((UdxCfgInfo *param, Pst *pst, Buffer *mBuf));
PRIVATE S16 cmUnpkUdxEntCfgCfmInfo ARGS((UdxEntCfgCfmInfo *param, Buffer *mBuf));
PRIVATE S16 cmUnpkUdxUeInfo ARGS((UdxUeInfo *param, Buffer *mBuf));
PRIVATE S16 cmPkUdxUeInfo ARGS((CkwUeInfo *param, Buffer *mBuf));
#endif
/*
*
*    Fun:    cmPkUdxBndReq
*
*    Desc:    pack the primitive UdxBndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else
PUBLIC S16 cmPkUdxBndReq(pst, suId, spId)
Pst *pst;
SuId suId;
SpId spId;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkUdxBndReq)

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
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    CMCHKPKLOG(SPkS16, suId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_BND_REQ;

    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkUdxBndReq*/

/*
*
*    Fun:    cmPkUdxUbndReq
*
*    Desc:    pack the primitive UdxUbndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else
PUBLIC S16 cmPkUdxUbndReq(pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkUdxUbndReq)

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
    CMCHKPKLOG(SPkS16, reason, mBuf, EUDXXXX, pst);
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_UBND_REQ;

    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkUdxUbndReq*/

/*
*
*    Fun:    cmPkUdxBndCfm
*
*    Desc:    pack the primitive UdxBndCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxBndCfm
(
Pst *pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 cmPkUdxBndCfm(pst, suId, status)
Pst *pst;
SuId suId;
U8 status;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkUdxBndCfm)

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
    CMCHKPKLOG(SPkU8, status, mBuf, EUDXXXX, pst);
    CMCHKPKLOG(SPkS16, suId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_BND_CFM;

    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkUdxBndCfm*/

#if 0

/*
*
*    Fun:    cmPkUdxTmInfo
*
*    Desc:    pack the structure UdxTmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmPkUdxTmInfo
(
UdxTmInfo      *param,
Buffer         *mBuf
)
#else
PRIVATE S16 cmPkUdxTmInfo(param, mBuf)
UdxTmInfo      *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmPkUdxTmInfo)

    CMCHKPK(SPkU32, param->dl.buffSize, mBuf);

    RETVALUE(ROK);
} /* cmPkUdxTmInfo */


/*
*
*    Fun:    cmPkUdxUmInfo
*
*    Desc:    pack the structure UdxUmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmPkUdxUmInfo
(
UdxEntCfgInfo  *param,
Buffer         *mBuf
)
#else
PRIVATE S16 cmPkUdxUmInfo(param, mBuf)
UdxEntCfgInfo  *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmPkUdxUmInfo)

    switch(param->dir)
    {
       case UDX_CFG_DIR_DL:
          {
             CMCHKPK(SPkU8, param->m.umInfo.dl.snLen, mBuf);
             break;
          }
       case UDX_CFG_DIR_UL:
          {
             CMCHKPK(SPkU8, param->m.umInfo.ul.reOrdTmr, mBuf);
             CMCHKPK(SPkU8, param->m.umInfo.ul.snLen, mBuf);
             break;
          }
       /* Patch ckw_c_001.main_3 */
       case UDX_CFG_DIR_BOTH:
          {
             CMCHKPK(SPkU8, param->m.umInfo.dl.snLen, mBuf);
             CMCHKPK(SPkU8, param->m.umInfo.ul.reOrdTmr, mBuf);
             CMCHKPK(SPkU8, param->m.umInfo.ul.snLen, mBuf);
             break;
          }
    }

    RETVALUE(ROK);
} /* cmPkUdxUmInfo */


/*
*
*    Fun:    cmPkUdxAmInfo
*
*    Desc:    pack the structure UdxAmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmPkUdxAmInfo
(
UdxAmInfo      *param,
Buffer         *mBuf
)
#else
PRIVATE S16 cmPkUdxAmInfo(param, mBuf)
UdxAmInfo      *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmPkUdxAmInfo)

    /* UP LINK */
    CMCHKPK(SPkU8, param->ul.reOrdTmr, mBuf);
    CMCHKPK(SPkU16, param->ul.staProhTmr, mBuf);

    /* DOWN LINK */
    CMCHKPK(SPkU8, param->dl.maxRetx, mBuf);
    CMCHKPK(SPkS32, param->dl.pollByte, mBuf);
    CMCHKPK(SPkS16, param->dl.pollPdu, mBuf);
    CMCHKPK(SPkU16, param->dl.pollRetxTmr, mBuf);

    RETVALUE(ROK);
} /* cmPkUdxAmInfo */


/*
*
*    Fun:    cmPkUdxLInfo
*
*    Desc:    pack the structure UdxLChInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmPkUdxLChInfo
(
UdxLChInfo     *param,
Buffer         *mBuf
)
#else
PRIVATE S16 cmPkUdxLChInfo(param, mBuf)
UdxLChInfo     *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmPkUdxLChInfo)

    CMCHKPK(SPkU8, param->type, mBuf);
    CMCHKPK(SPkU8, param->lChId, mBuf);

    RETVALUE(ROK);
} /* cmPkUdxLChInfo */


/*
*
*    Fun:    cmPkUdxEntCfgInfo
*
*    Desc:    pack the structure UdxEntCfgInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmPkUdxEntCfgInfo
(
UdxEntCfgInfo  *param,
Buffer         *mBuf
)
#else
PRIVATE S16 cmPkUdxEntCfgInfo(param, mBuf)
UdxEntCfgInfo  *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmPkUdxEntCfgInfo)

    switch(param->entMode)
    {
       case CM_LTE_MODE_TM:
          {
             CMCHKPK(cmPkUdxTmInfo, &(param->m.tmInfo), mBuf);
             break;
          }
       case CM_LTE_MODE_UM:
          {
             CMCHKPK(cmPkUdxUmInfo, param, mBuf);
             break;
          }
       case CM_LTE_MODE_AM:
          {
             CMCHKPK(cmPkUdxAmInfo, &(param->m.amInfo), mBuf);
             break;
          }
    }

    /* Logical Channel Information */
    switch(param->entMode)
    {
       case CM_LTE_MODE_TM:
       case CM_LTE_MODE_UM:
          {
             CMCHKPK(cmPkUdxLChInfo, &(param->lCh[0]), mBuf);
             break;
          }
       case CM_LTE_MODE_AM:
          {
             CMCHKPK(cmPkUdxLChInfo, &(param->lCh[1]), mBuf);
             CMCHKPK(cmPkUdxLChInfo, &(param->lCh[0]), mBuf);
             break;
          }
    }

    CMCHKPK(SPkU8, param->dir, mBuf);
    CMCHKPK(cmPkLteRlcMode, param->entMode, mBuf);
    /* ckw_c_001.main_4 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
    CMCHKPK(SPkU8, param->qci, mBuf);
#endif /* LTE_L2_MEAS */
    CMCHKPK(SPkU8, param->rbType, mBuf);
    CMCHKPK(SPkU8, param->rbId, mBuf);
    CMCHKPK(SPkU8, param->cfgType, mBuf);

    RETVALUE(ROK);
} /* cmPkUdxEntCfgInfo */


/*
*
*    Fun:    cmPkUdxCfgInfo
*
*    Desc:    pack the structure UdxCfgInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmPkUdxCfgInfo
(
UdxCfgInfo     *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PRIVATE S16 cmPkUdxCfgInfo(param, pst, mBuf)
UdxCfgInfo     *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
   S16 idx;

   TRC3(cmPkUdxCfgInfo)

    for (idx = param->numEnt-1; idx >= 0; idx--)
    {
       CMCHKPK(cmPkUdxEntCfgInfo, &(param->entCfg[idx]), mBuf);
    }

    CMCHKPK(SPkU8, param->numEnt, mBuf);
    CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
    CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
    CMCHKPKLOG(SPkU32, param->transId, mBuf, EUDXXXX, pst);

    RETVALUE(ROK);
} /* cmPkUdxCfgInfo */
#endif

/*
*
*    Fun:    cmPkUdxCfgReq
*
*    Desc:    pack the primitive KwUiUdxCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxCfgReq
(
Pst               *pst,
SpId              spId,
CkwCfgInfo        *cfgInfo
)
#else
PUBLIC S16 cmPkUdxCfgReq(pst, spId, cfgInfo)
Pst               *pst;
SpId              spId;
CkwCfgInfo        *cfgInfo;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkUdxCfgReq)

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

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
#if 0
          {
             ret1 = cmPkUdxCfgInfo( (cfgInfo), pst, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EUDXXXX, (ErrVal)ret1, "Packing failure");
                RETVALUE( ret1 );
             }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             if (SPutSBuf(pst->region, pst->pool, (Data *)cfgInfo,
                      sizeof(UdxCfgInfo)) != ROK)
             {
                SPutMsg(mBuf);
                RETVALUE(RFAILED);
             }

             break;
          }
#endif
        case UDX_SEL_LWLC:
          {
             CMCHKPKLOG(cmPkPtr,(PTR)cfgInfo,mBuf,EUDXXXX,pst);
             break;
          }
#endif /* LCUDX */
    }

    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_CFG_REQ;

    RETVALUE(SPstTsk(pst,mBuf));
} /* cmPkUdxCfgReq */

#if 0

/*
*
*    Fun:    cmPkUdxEntCfgCfmInfo
*
*    Desc:    pack the structure UdxEntCfgInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmPkUdxEntCfgCfmInfo
(
UdxEntCfgCfmInfo  *param,
Buffer            *mBuf
)
#else
PRIVATE S16 cmPkUdxEntCfgCfmInfo(param, mBuf)
UdxEntCfgCfmInfo  *param;
Buffer            *mBuf;
#endif
{
    TRC3(cmPkUdxEntCfgCfmInfo)

    CMCHKPK(cmPkCmStatus, &(param->status), mBuf);
    CMCHKPK(SPkU8, param->rbType, mBuf);
    CMCHKPK(SPkU8, param->rbId, mBuf);

    RETVALUE(ROK);
} /* cmPkUdxEntCfgCfmInfo */


/*
*
*    Fun:    cmPkUdxCfgCfmInfo
*
*    Desc:    pack the structure UdxCfgCfmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmPkUdxCfgCfmInfo
(
UdxCfgCfmInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PRIVATE S16 cmPkUdxCfgCfmInfo(param, pst, mBuf)
UdxCfgCfmInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    S16         idx;

    TRC3(cmPkUdxCfgCfmInfo)

    for (idx = param->numEnt-1; idx >= 0; idx--)
    {
       CMCHKPK(cmPkUdxEntCfgCfmInfo, &(param->entCfgCfm[(U8)idx]), mBuf);
    }

    CMCHKPK(SPkU8, param->numEnt, mBuf);
    CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
    CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
    CMCHKPKLOG(SPkU32, param->transId, mBuf, EUDXXXX, pst);

    RETVALUE(ROK);
} /* cmPkUdxCfgCfmInfo */
#endif

/*
*
*    Fun:    cmPkUdxCfgCfm
*
*    Desc:    pack the primitive KwUiUdxCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxCfgCfm
(
Pst               *pst,
SuId              suId,
CkwCfgCfmInfo     *cfgCfmInfo
)
#else
PUBLIC S16 cmPkUdxCfgCfm(pst, suId, cfgCfmInfo)
Pst               *pst;
SuId              suId;
CkwCfgCfmInfo     *cfgCfmInfo;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkUdxCfgCfm)

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

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
#if 0         
          {
             ret1 = cmPkUdxCfgCfmInfo( (cfgCfmInfo), pst, mBuf);
#if (ERRCLASS & ERRCLS_ADD_RES)
             if(ret1 != ROK)
             {
                SPutMsg(mBuf);
                SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                      __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                      (ErrVal)EUDXXXX, (ErrVal)ret1, "Packing failure");
                RETVALUE( ret1 );
             }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
             if (SPutSBuf(pst->region, pst->pool, (Data *)cfgCfmInfo,
                      sizeof(UdxCfgCfmInfo)) != ROK)
             {
                SPutMsg(mBuf);
                RETVALUE(RFAILED);
             }

             break;
          }
#endif
        case UDX_SEL_LWLC:
          {
             CMCHKPKLOG(cmPkPtr,(PTR)cfgCfmInfo,mBuf,EUDXXXX,pst);
             break;
          }
#endif /* LCUDX */
    }

    CMCHKPKLOG(SPkS16, suId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_CFG_CFM;

    RETVALUE(SPstTsk(pst,mBuf));
} /* cmPkUdxCfgCfm */


/*
*
*    Fun:    cmPkUdxUeInfo
*
*    Desc:   pack the primitive KwUiUdxUeInfo
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#if 0
#ifdef ANSI
PRIVATE S16 cmPkUdxUeInfo
(
CkwUeInfo *param,
Buffer    *mBuf
)
#else
PRIVATE S16 cmPkUdxUeInfo(param, mBuf)
CkwUeInfo *param;
Buffer    *mBuf;
#endif
{
   TRC2(cmPkUdxUeInfo)

   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);

   RETVALUE(ROK);
}
#endif
/*
*
*    Fun:    cmPkUdxUeIdChgReq
*
*    Desc:   pack the primitive KwUiUdxUeIdChgReq
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxUeIdChgReq
(
Pst               *pst,
SpId              spId,
U32               transId,
CkwUeInfo         *ueInfo,
CkwUeInfo         *newUeInfo
)
#else
PUBLIC S16 cmPkUdxUeIdChgReq(pst, spId, transId, ueInfo, newUeInfo)
Pst               *pst;
SpId              spId;
U32               transId;
CkwUeInfo         *ueInfo;
CkwUeInfo         *newUeInfo;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkUdxUeIdChgReq)

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

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
       case UDX_SEL_LWLC:
          {
             CMCHKPKLOG(cmPkPtr,(PTR)newUeInfo,mBuf,EUDXXXX,pst);
             CMCHKPKLOG(cmPkPtr,(PTR)ueInfo,mBuf,EUDXXXX,pst);
             break;
          }
       default:
       {
          printf("cmPkUdxUeIdChgReq()- selector =%d not supported \n", pst->selector);
          exit(1);
       }
#endif
    }
    CMCHKPKLOG(SPkU32, transId, mBuf, EUDXXXX, pst);
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_UEIDCHG_REQ;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkUdxUeIdChgReq */

/*
*
*    Fun:    cmPkUdxUeIdChgCfm
*
*    Desc:   pack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxUeIdChgCfm
(
Pst               *pst,
SuId              suId,
U32               transId,
CmStatus          status
)
#else
PUBLIC S16 cmPkUdxUeIdChgCfm(pst, suId, transId, status)
Pst               *pst;
SuId              suId;
U32               transId;
CmStatus          status;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;
    TRC3(cmPkUdxUeIdChgCfm)

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
    
    CMCHKPK(cmPkCmStatus, &status, mBuf); 
    CMCHKPKLOG(SPkU32, transId, mBuf, EUDXXXX, pst);
    CMCHKPKLOG(SPkS16, suId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_UEIDCHG_CFM;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkUdxUeIdChgCfm */

/*
*
*    Fun:    cmPkUdxStaUpdCfm
*
*    Desc:   pack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxStaUpdCfm
(
Pst               *pst,
SuId              suId,
CmLteRlcId        *rlcId,
KwUdxBufLst       *pStaPdu
)
#else
PUBLIC S16 cmPkUdxStaUpdCfm(pst, suId, rlcId,pStaPdu)
Pst               *pst;
SuId              suId;
CmLteRlcId        *rlcId;
KwUdxBufLst       *pStaPdu;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkUdxStaUpdCfm)

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
         CMCHKPK(cmPkPtr,(PTR) pStaPdu, mBuf);
         CMCHKPK(cmPkPtr,(PTR) rlcId, mBuf); 
         break;
       }
    }
    CMCHKPKLOG(SPkS16, suId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_STA_UPD_CFM;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkUdxStaUpdCfm */

/*
*
*    Fun:    cmPkUdxStaProhTmrStart
*
*    Desc:   pack the primitive
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   Kw_udx.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxStaProhTmrStart
(
Pst               *pst,
SpId              spId,
CmLteRlcId        *rlcId
)
#else
PUBLIC S16 cmPkUdxStaProhTmrStart(pst, suId, rlcId)
Pst               *pst;
SpId              spId;
CmLteRlcId        *rlcId;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkUdxStaProhTmrStart)

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
       case UDX_SEL_LWLC:
       {
         CMCHKPK(cmPkPtr,(PTR) rlcId, mBuf); 
         break;
       }
    }
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_STA_PHBT_TMR_START;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkUdxStaProhTmrStart */

/*
*
*    Fun:    cmPkUdxStaUpdReq
*
*    Desc:   pack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxStaUpdReq
(
Pst               *pst,
SpId              spId,
CmLteRlcId        *rlcId,
KwUdxStaPdu      *pStaPdu
)
#else
PUBLIC S16 cmPkUdxStaUpdCfm(pst, suId, rlcId,pStaPdu)
Pst               *pst;
SpId              spId;
CmLteRlcId        *rlcId;
KwUdxStaPdu       *pStaPdu;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkUdxStaUpdCfm)

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
         CMCHKPK(cmPkPtr,(PTR) pStaPdu, mBuf);
         CMCHKPK(cmPkPtr,(PTR) rlcId, mBuf); 
         break;
       }
    }
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_STA_UPD_REQ;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkUdxStaUpdReq */

/*
*
*    Fun:    cmPkUdxStaPduReq
*
*    Desc:   pack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkUdxStaPduReq
(
Pst               *pst,
SpId              spId,
CmLteRlcId        *rlcId,
KwUdxDlStaPdu     *pStaPdu
)
#else
PUBLIC S16 cmPkUdxStaPduReq(pst, suId, rlcId,pStaPdu)
Pst               *pst;
SpId              spId;
CmLteRlcId        *rlcId;
KwUdxDlStaPdu     *pStaPdu;
#endif
{
    S16    ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkUdxStaUpdReq)

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
         CMCHKPK(cmPkPtr,(PTR) pStaPdu, mBuf);
         CMCHKPK(cmPkPtr,(PTR) rlcId, mBuf); 
         break;
       }
    }
    CMCHKPKLOG(SPkS16, spId, mBuf, EUDXXXX, pst);
    pst->event = (Event) UDX_EVT_STA_PDU_REQ;

    RETVALUE(SPstTsk(pst, mBuf));

} /* cmPkUdxStaUpdReq */

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
KwL2MeasReqEvt    *measReqEvt 
)
#else
PUBLIC S16 cmPkUdxL2MeasReq(pst, measReqEvt)
Pst               *pst;
KwL2MeasReqEvt    *measReqEvt; 
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
#endif
/******************************************************************************
 *                            UNPACK FUNCTIONS
 *****************************************************************************/

/*
*
*    Fun:    cmUnpkUdxBndReq
*
*    Desc:    unpack the primitive UdxBndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxBndReq
(
UdxBndReq      func,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmUnpkUdxBndReq(func, pst, mBuf)
UdxBndReq      func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    SuId       suId = 0;
    SpId       spId = 0;
    
    TRC3(cmUnpkUdxBndReq)

    CMCHKUNPKLOG(SUnpkS16, &suId, mBuf, EUDXXXX, pst);
    CMCHKUNPKLOG(SUnpkS16, &spId, mBuf, EUDXXXX, pst);
    SPutMsg(mBuf);

    RETVALUE((*func)(pst, suId, spId));
} /*end of function cmUnpkUdxBndReq*/

/*
*
*    Fun:    cmUnpkUdxUbndReq
*
*    Desc:    unpack the primitive UdxUbndReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxUbndReq
(
UdxUbndReq     func,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUbndReq(func, pst, mBuf)
UdxUbndReq     func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    SpId       spId = 0;
    Reason     reason = 0;
    
    TRC3(cmUnpkUdxUbndReq)

    CMCHKUNPKLOG(SUnpkS16, &spId, mBuf, EUDXXXX, pst);
    CMCHKUNPKLOG(SUnpkS16, &reason, mBuf, EUDXXXX, pst);
    SPutMsg(mBuf);
    RETVALUE((*func)(pst, spId, reason));
} /*end of function cmUnpkUdxUbndReq*/

/*
*
*    Fun:    cmUnpkUdxBndCfm
*
*    Desc:    unpack the primitive UdxBndCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxBndCfm
(
UdxBndCfm      func,
Pst            *pst,
Buffer         *mBuf
)
#else
PUBLIC S16 cmUnpkUdxBndCfm(func, pst, mBuf)
UdxBndCfm      func;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    SuId       suId = 0;
    U8         status = 0;
    
    TRC3(cmUnpkUdxBndCfm)

    CMCHKUNPKLOG(SUnpkS16, &suId, mBuf, EUDXXXX, pst);
    CMCHKUNPKLOG(SUnpkU8, &status, mBuf, EUDXXXX, pst);
    SPutMsg(mBuf);

    RETVALUE((*func)(pst, suId, status));
} /*end of function cmUnpkUdxBndCfm*/

#if 0

/*
*
*    Fun:    cmUnpkUdxTmInfo
*
*    Desc:    pack the structure UdxTmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmUnpkUdxTmInfo
(
UdxTmInfo      *param,
Buffer         *mBuf
)
#else
PRIVATE S16 cmUnpkUdxTmInfo(param, mBuf)
UdxTmInfo      *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmUnpkUdxTmInfo)

    CMCHKUNPK(SUnpkU32, &(param->dl.buffSize), mBuf);

    RETVALUE(ROK);
} /* cmUnpkUdxTmInfo */


/*
*
*    Fun:    cmUnpkUdxUmInfo
*
*    Desc:    pack the structure UdxUmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmUnpkUdxUmInfo
(
UdxEntCfgInfo  *param,
Buffer         *mBuf
)
#else
PRIVATE S16 cmUnpkUdxUmInfo(param, mBuf)
UdxEntCfgInfo  *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmUnpkUdxUmInfo)

    switch(param->dir)
    {
       case UDX_CFG_DIR_DL:
          {
             CMCHKUNPK(SUnpkU8, &(param->m.umInfo.dl.snLen), mBuf);
             break;
          }
       case UDX_CFG_DIR_UL:
          {
             CMCHKUNPK(SUnpkU8, &(param->m.umInfo.ul.snLen), mBuf);
             CMCHKUNPK(SUnpkU8, &(param->m.umInfo.ul.reOrdTmr), mBuf);
             break;
          }
       /* Patch ckw_c_001.main_3 */
       case UDX_CFG_DIR_BOTH:
          {
             CMCHKUNPK(SUnpkU8, &(param->m.umInfo.ul.snLen), mBuf);
             CMCHKUNPK(SUnpkU8, &(param->m.umInfo.ul.reOrdTmr), mBuf);
             CMCHKUNPK(SUnpkU8, &(param->m.umInfo.dl.snLen), mBuf);
             break;
          }
    }

    RETVALUE(ROK);
} /* cmUnpkUdxUmInfo */


/*
*
*    Fun:    cmUnpkUdxAmInfo
*
*    Desc:    pack the structure UdxAmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmUnpkUdxAmInfo
(
UdxAmInfo      *param,
Buffer         *mBuf
)
#else
PRIVATE S16 cmUnpkUdxAmInfo(param, mBuf)
UdxAmInfo      *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmUnpkUdxAmInfo)

    /* DOWN LINK */
    CMCHKUNPK(SUnpkU16, &(param->dl.pollRetxTmr), mBuf);
    CMCHKUNPK(SUnpkS16, &(param->dl.pollPdu), mBuf);
    CMCHKUNPK(SUnpkS32, &(param->dl.pollByte), mBuf);
    CMCHKUNPK(SUnpkU8, &(param->dl.maxRetx), mBuf);

    /* UP LINK */
    CMCHKUNPK(SUnpkU16, &(param->ul.staProhTmr), mBuf);
    CMCHKUNPK(SUnpkU8, &(param->ul.reOrdTmr), mBuf);

    RETVALUE(ROK);
} /* cmUnpkUdxAmInfo */


/*
*
*    Fun:    cmUnpkUdxLChInfo
*
*    Desc:    pack the structure UdxLChInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmUnpkUdxLChInfo
(
UdxLChInfo     *param,
Buffer         *mBuf
)
#else
PRIVATE S16 cmUnpkUdxLChInfo(param, mBuf)
UdxLChInfo     *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmUnpkUdxLChInfo)

    CMCHKUNPK(SUnpkU8, &(param->lChId), mBuf);
    CMCHKUNPK(SUnpkU8, &(param->type), mBuf);

    RETVALUE(ROK);
} /* cmUnpkUdxLChInfo */


/*
*
*    Fun:    cmUnpkUdxEntCfgCfmInfo
*
*    Desc:    unpack the structure UdxEntCfgInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmUnpkUdxEntCfgCfmInfo
(
UdxEntCfgCfmInfo  *param,
Buffer            *mBuf
)
#else
PRIVATE S16 cmUnpkUdxEntCfgCfmInfo(param, mBuf)
UdxEntCfgCfmInfo  *param;
Buffer            *mBuf;
#endif
{
    TRC3(cmUnpkUdxEntCfgCfmInfo)

    CMCHKUNPK(SUnpkU8, &(param->rbId), mBuf);
    CMCHKUNPK(SUnpkU8, &(param->rbType), mBuf);
    CMCHKUNPK(cmUnpkCmStatus, &(param->status), mBuf);

    RETVALUE(ROK);
} /* cmUnpkUdxEntCfgCfmInfo */


/*
*
*    Fun:    cmUnpkUdxCfgCfmInfo
*
*    Desc:    pack the structure UdxCfgCfmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmUnpkUdxCfgCfmInfo
(
CkwCfgCfmInfo  *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PRIVATE S16 cmUnpkUdxCfgCfmInfo(param, pst, mBuf)
CkwCfgCfmInfo  *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    U8         idx;

    TRC3(cmUnpkUdxCfgCfmInfo)

    CMCHKUNPKLOG(SUnpkU32, &(param->transId), mBuf, EUDXXXX, pst);
    CMCHKUNPK(cmUnpkLteRnti, &(param->ueId), mBuf);
    CMCHKUNPK(cmUnpkLteCellId, &(param->cellId), mBuf);
    CMCHKUNPK(SUnpkU8, &(param->numEnt), mBuf);

    for (idx = 0; idx < param->numEnt; idx++)
    {
       CMCHKUNPK(cmUnpkUdxEntCfgCfmInfo, &(param->entCfgCfm[idx]), mBuf);
    }

    RETVALUE(ROK);
} /* cmUnpkUdxCfgCfmInfo */
#endif
#if 0

/*
*
*    Fun:    cmUnpkUdxEntCfgInfo
*
*    Desc:    pack the structure UdxEntCfgCfmInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmUnpkUdxEntCfgInfo
(
UdxEntCfgInfo  *param,
Buffer         *mBuf
)
#else
PRIVATE S16 cmUnpkUdxEntCfgInfo(param, mBuf)
UdxEntCfgInfo  *param;
Buffer         *mBuf;
#endif
{
    TRC3(cmUnpkUdxEntCfgInfo)

    CMCHKUNPK(SUnpkU8, &(param->cfgType), mBuf);
    CMCHKUNPK(SUnpkU8, &(param->rbId), mBuf);
    CMCHKUNPK(SUnpkU8, &(param->rbType), mBuf);
    /* ckw_c_001.main_4 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
    CMCHKUNPK(SUnpkU8, &(param->qci), mBuf);
#endif /* LTE_L2_MEAS */
    CMCHKUNPK(cmUnpkLteRlcMode, &(param->entMode), mBuf);
    CMCHKUNPK(SUnpkU8, &(param->dir), mBuf);

    /* Logical Channel Information */
    switch(param->entMode)
    {
       case CM_LTE_MODE_TM:
       case CM_LTE_MODE_UM:
          {
             CMCHKUNPK(cmUnpkUdxLChInfo, &(param->lCh[0]), mBuf);
             break;
          }
       case CM_LTE_MODE_AM:
          {
             CMCHKUNPK(cmUnpkUdxLChInfo, &(param->lCh[0]), mBuf);
             CMCHKUNPK(cmUnpkUdxLChInfo, &(param->lCh[1]), mBuf);
             break;
          }
    }

    /* TM/UM/AM Info */
    switch(param->entMode)
    {
       case CM_LTE_MODE_TM:
          {
             CMCHKUNPK(cmUnpkUdxTmInfo, &(param->m.tmInfo), mBuf);
             break;
          }
       case CM_LTE_MODE_UM:
          {
             CMCHKUNPK(cmUnpkUdxUmInfo, param, mBuf);
             break;
          }
       case CM_LTE_MODE_AM:
          {
             CMCHKUNPK(cmUnpkUdxAmInfo, &(param->m.amInfo), mBuf);
             break;
          }
    }

    RETVALUE(ROK);
} /* cmUnpkUdxEntCfgInfo */


/*
*
*    Fun:    cmUnpkUdxCfgInfo
*
*    Desc:    unpack the structure UdxCfgInfo
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PRIVATE S16 cmUnpkUdxCfgInfo
(
UdxCfgInfo     *param,
Pst            *pst,
Buffer         *mBuf
)
#else
PRIVATE S16 cmUnpkUdxCfgInfo(param, pst, mBuf)
UdxCfgInfo     *param;
Pst            *pst;
Buffer         *mBuf;
#endif
{
    U8         idx;

    TRC3(cmUnpkUdxCfgInfo)

    CMCHKUNPKLOG(SUnpkU32, &(param->transId), mBuf, EUDXXXX, pst);
    CMCHKUNPK(cmUnpkLteRnti, &(param->ueId), mBuf);
    CMCHKUNPK(cmUnpkLteCellId, &(param->cellId), mBuf);
    CMCHKUNPK(SUnpkU8, &(param->numEnt), mBuf);

    for (idx = 0; idx < param->numEnt; idx++)
    {
       CMCHKUNPKLOG(cmUnpkUdxEntCfgInfo,&(param->entCfg[idx]), mBuf, EUDXXXX, pst);
    }

    RETVALUE(ROK);
} /* cmUnpkUdxCfgInfo */
#endif

/*
*
*    Fun:    cmUnpkUdxCfgReq
*
*    Desc:    unpack the primitive KwUiUdxCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxCfgReq
(
UdxCfgReq         func,
Pst               *pst,
Buffer            *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCfgReq(func, pst, mBuf)
UdxCfgReq         func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
#if 0    
    S16           ret1;
#endif    
    SpId          spId = 0;
    CkwCfgInfo    *cfgInfo = NULLP;
    
    TRC3(cmUnpkUdxCfgReq)

    CMCHKUNPK(SUnpkS16, &(spId), mBuf);
    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
#if 0         
       {
          if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&cfgInfo,\
                       sizeof(UdxCfgInfo))) != ROK)
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

          cmMemset((U8 *)cfgInfo, 0, sizeof(UdxCfgInfo));

          ret1 = cmUnpkUdxCfgInfo( (cfgInfo), pst, mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
          if(ret1 != ROK)
          {
             SPutMsg(mBuf);
             SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                   __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                  (ErrVal)EUDXXXX, (ErrVal)ret1, "Unpacking failure");
             RETVALUE( ret1 );
          }
#endif /* ERRCLASS & ERRCLS_DEBUG */
          break;
       }
#endif
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &cfgInfo, mBuf);
          break;
       }
#endif /* LCUDX */
    }
    SPutMsg(mBuf);

    RETVALUE((*func)(pst, spId, cfgInfo));
} /* cmUnpkUdxCfgReq */


/*
*
*    Fun:    cmUnpkUdxCfgCfm
*
*    Desc:    unpack the primitive KwUiUdxCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxCfgCfm
(
UdxCfgCfm         func,
Pst               *pst,
Buffer            *mBuf
)
#else
PUBLIC S16 cmUnpkUdxCfgCfm(func, pst, mBuf)
UdxCfgCfm         func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
#if 0    
    S16 ret1;
#endif   
    SuId             suId = 0;
    CkwCfgCfmInfo    *cfgCfmInfo = NULLP;
    
    TRC3(cmUnpkUdxCfgCfm)


    CMCHKUNPK(SUnpkS16, &suId, mBuf);

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
#if 0         
       {
           if((ret1 = SGetSBuf(pst->region, pst->pool, (Data **)&cfgCfmInfo,\
                       sizeof(UdxCfgCfmInfo))) != ROK)
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

           cmMemset((U8 *)cfgCfmInfo, 0, sizeof(UdxCfgCfmInfo));

          ret1 = cmUnpkUdxCfgCfmInfo( (cfgCfmInfo), pst, mBuf);
#if(ERRCLASS & ERRCLS_DEBUG)
          if(ret1 != ROK)
          {
             SPutMsg(mBuf);
             SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                   __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                  (ErrVal)EUDXXXX, (ErrVal)ret1, "Unpacking failure");
             RETVALUE( ret1 );
          }
#endif /* ERRCLASS & ERRCLS_DEBUG */
          break;
       }
#endif
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &cfgCfmInfo, mBuf);
          break;
       }
#endif /* LCUDX */
    }
    SPutMsg(mBuf);

    RETVALUE((*func)(pst, suId, cfgCfmInfo));
} /* cmUnpkUdxCfgCfm */

#if 0
/*
*
*    Fun:    cmUnpkUdxUeInfo
*
*    Desc:   Unpack the primitive KwUiUdxUeInfo
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxUeInfo
(
CkwUeInfo *param,
Buffer    *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUeInfo(param, mBuf)
CkwUeInfo *param;
Buffer    *mBuf;
#endif
{
   TRC2(cmUnpkUdxUeInfo)

   CMCHKUNPK(cmUnpkLteRnti, &(param->ueId), mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &(param->cellId), mBuf);

   RETVALUE(ROK);
}
#endif
/*
*
*    Fun:    cmUnpkUdxUeIdChgReq
*
*    Desc:    unpack the primitive KwUiUdxUeIdChgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxUeIdChgReq
(
UdxUeIdChgReq     func,
Pst               *pst,
Buffer            *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUeIdChgReq(func, pst, mBuf)
UdxUeIdChgReq     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    SpId      spId = 0;
    U32       transId = 0;
    CkwUeInfo *ueInfo = NULLP;
    CkwUeInfo *newUeInfo = NULLP;
    
    TRC3(cmUnpkUdxUeIdChgReq)


    CMCHKUNPK(SUnpkS16, &(spId), mBuf);
    CMCHKUNPKLOG(SUnpkU32, &transId, mBuf, EUDXXXX, pst);

    switch(pst->selector)
    {
#ifdef LCUDX
       case UDX_SEL_LC:
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr,(PTR *) &ueInfo, mBuf);
          CMCHKUNPK(cmUnpkPtr,(PTR *) &newUeInfo, mBuf);
          break;
       }
       default:
       {
          printf("cmUnpkUdxUeIdChgReq()- selector =%d not supported \n", pst->selector);
          exit(1);
       }

#endif /* LCUDX */
    }
    SPutMsg(mBuf);

    RETVALUE((*func)(pst, spId, transId, ueInfo, newUeInfo));

} /* cmUnpkUdxUeIdChgReq */

/*
*
*    Fun:     cmUnpkUdxUeIdChgCfm
*
*    Desc:    unpack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:     ROK  -ok
*
*    Notes:   None
*
*    File:    ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxUeIdChgCfm
(
UdxUeIdChgCfm     func,
Pst               *pst,
Buffer            *mBuf
)
#else
PUBLIC S16 cmUnpkUdxUeIdChgCfm(func, pst, mBuf)
UdxUeIdChgCfm     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    SuId      suId = 0;
    U32       transId = 0;
    CmStatus  status;
    
    TRC3(cmUnpkUdxUeIdChgCfm)

    cmMemset((U8 *)&status, (U8)0, (PTR)sizeof(CmStatus));

    CMCHKUNPK(SUnpkS16, &suId, mBuf);
    CMCHKUNPKLOG(SUnpkU32, &transId, mBuf, EUDXXXX, pst);

    CMCHKUNPK(cmUnpkCmStatus, &status, mBuf);

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, suId, transId, status));

} /* cmUnpkUdxUeIdChgCfm */

/*
*
*    Fun:     cmUnpkUdxStaUpdCfm
*
*    Desc:    unpack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:     ROK  -ok
*
*    Notes:   None
*
*    File:    ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxStaUpdCfm
(
UdxStaUpdCfm     func,
Pst               *pst,
Buffer            *mBuf
)
#else
PUBLIC S16 cmUnpkUdxStaUpdCfm(func, pst, mBuf)
UdxStaUpdCfm     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    SuId      suId = 0;
    CmLteRlcId *rlcId = NULLP;  /* KW_FIX */
    KwUdxBufLst *pBufLst = NULLP; /* KW_FIX*/
    
    TRC3(cmUnpkUdxStaUpdCfm)

    CMCHKUNPK(SUnpkS16, &suId, mBuf);
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
	  break;
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr, (PTR *)&rlcId,mBuf);
          CMCHKUNPK(cmUnpkPtr, (PTR *)&pBufLst,mBuf);
          break;
       }
    } 

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, suId, rlcId, pBufLst));

} /* cmUnpkUdxUeIdChgCfm */

/*
*
*    Fun:     cmUnpkUdxStaUpdReq
*
*    Desc:    unpack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:     ROK  -ok
*
*    Notes:   None
*
*    File:    ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxStaUpdReq
(
UdxStaUpdReq     func,
Pst               *pst,
Buffer            *mBuf
)
#else
PUBLIC S16 cmUnpkUdxStaUpdReq(func, pst, mBuf)
UdxStaUpdReq     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    SpId      spId = 0;
    CmLteRlcId *rlcId = NULLP; /* KW_FIX */ 
    KwUdxStaPdu *pStaPdu = NULLP; /* KW_FIX */
    
    TRC3(cmUnpkUdxStaUpdCfm)

    CMCHKUNPK(SUnpkS16, &spId, mBuf);
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr, (PTR *)&rlcId,mBuf);
          CMCHKUNPK(cmUnpkPtr, (PTR *)&pStaPdu,mBuf);
          break;
       }
    } 

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, spId, rlcId, pStaPdu));

} /* cmUnpkUdxUeIdChgCfm */

/*
*
*    Fun:     cmUnpkUdxStaPduReq
*
*    Desc:    unpack the primitive KwUiUdxUeIdChgCfm
*
*    Ret:     ROK  -ok
*
*    Notes:   None
*
*    File:    ckw.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxStaPduReq
(
UdxStaPduReq     func,
Pst               *pst,
Buffer            *mBuf
)
#else
PUBLIC S16 cmUnpkUdxStaPduReq(func, pst, mBuf)
UdxStaPduReq     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    SpId      spId = 0;
    CmLteRlcId *rlcId = NULLP;  /* KW_FIX */
    KwUdxDlStaPdu *pStaPdu = NULLP; /* KW_FIX */
    
    TRC3(cmUnpkUdxStaUpdCfm)

    CMCHKUNPK(SUnpkS16, &spId, mBuf);
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr, (PTR *)&rlcId,mBuf);
          CMCHKUNPK(cmUnpkPtr, (PTR *)&pStaPdu,mBuf);
          break;
       }
    } 

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, spId, rlcId, pStaPdu));

} /* cmUnpkUdxUeIdChgCfm */

/*
*
*    Fun:     cmUnpkUdxStaProhTmrStart
*
*    Desc:    unpack the Status prohibit timer start Msg
*
*    Ret:     ROK  -ok
*
*    Notes:   None
*
*    File:    kw_udx.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkUdxStaProhTmrStart
(
UdxStaProhTmrStart     func,
Pst               *pst,
Buffer            *mBuf
)
#else
PUBLIC S16 cmUnpkUdxStaProhTmrStart(func, pst, mBuf)
UdxStaProhTmrStart     func;
Pst               *pst;
Buffer            *mBuf;
#endif
{
    SpId      spId = 0;
    CmLteRlcId *rlcId = NULLP; /* KW_FIX */
    
    TRC3(cmUnpkUdxStaProhTmrStart)

    CMCHKUNPK(SUnpkS16, &spId, mBuf);
    switch (pst->selector)
    {
       case UDX_SEL_LC:
       {
       }
       case UDX_SEL_LWLC:
       {
          CMCHKUNPK(cmUnpkPtr, (PTR *)&rlcId,mBuf);
          break;
       }
    } 

    SPutMsg(mBuf);

    RETVALUE((*func)(pst, spId, rlcId));

} /* cmUnpkUdxStaProhTmrStart */

#ifdef LTE_L2_MEAS
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
UdxL2MeasReq     func,
Pst              *pst,
Buffer           *mBuf
)
#else
PUBLIC S16 cmUnpkUdxL2MeasReq(func, pst, mBuf)
UdxL2MeasReq     func;
Pst              *pst;
Buffer           *mBuf;
#endif
{
    KwL2MeasReqEvt    *measReqEvt = NULLP;  /* KW_FIX */

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
UdxL2MeasSendReq   func,
Pst                *pst,
Buffer             *mBuf
)
#else
PUBLIC S16 cmUnpkUdxL2MeasSendReq(func, pst, mBuf)
UdxL2MeasSendReq   func;
Pst                *pst;
Buffer             *mBuf;
#endif
{
    U8     measType = 0;  /* KW_FIX */

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
UdxL2MeasStopReq   func,
Pst                *pst,
Buffer             *mBuf
)
#else
PUBLIC S16 cmUnpkUdxL2MeasStopReq(func, pst, mBuf)
UdxL2MeasSendReq     func;
Pst                  *pst;
Buffer               *mBuf;
#endif
{
    U8     measType = 0; /* KW_FIX */

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
#endif /* LCUDX */

  
/********************************************************************30**
         End of file:     kw_udx.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:26 2014
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
/main/1      ---      cp   1. initial release.
*********************************************************************91*/
