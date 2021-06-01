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
               

     File:     wr_lwr.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="ENBAPP";
static int RLOG_MODULE_ID=64;
static int RLOG_FILE_ID=106;
/* header include files */
#include "wr.h"
#include "wr_lwr.h"
#include "wr_cmn.h"

#ifdef LCLWR 

/*
*
*    Fun:    cmPkWrGenCfg
*
*    Desc:    pack the structure WrGenCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkWrGenCfg
(
LwrGenCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkWrGenCfg(param ,mBuf)
LwrGenCfg *param;
Buffer *mBuf;
#endif
{
   S16 idx = 0;
   S16 loopCnt = 0;
   S16 loopCnt1 = 0;
   TRC3(cmPkWrGenCfg);
#ifdef LTE_HO_SUPPORT
   CMCHKPK(SPkU32, param->inactvTmrVal, mBuf);
   CMCHKPK(SPkU16, param->expiryCnt, mBuf);
   CMCHKPK(SPkU32, param->ueHoAttachTimerVal, mBuf);
   CMCHKPK(SPkU8, param->noOfguGrp, mBuf);
   CMCHKPK(SPkU16, param->noOfNghInfo, mBuf);
   CMCHKPK(SPkU32, param->eNBType, mBuf);
#endif
   CMCHKPK(cmPkCmTptAddr, &param->dataAppAddr, mBuf);
#ifdef WR_RELAY
#ifdef LTE_HO_SUPPORT 
    for (loopCnt = (param->cellCfg.noOfBrdCstPlmn - 1); loopCnt >= 0; loopCnt--)
    {
       for (loopCnt1 = 2; loopCnt1 >= 0; loopCnt1--)
       {
          //cmPkTknU8(&param->cellCfg.plmnId[loopCnt][loopCnt1], mBuf);  
           SUnpkU8(&param->cellCfg.plmnId[loopCnt][loopCnt1], mBuf);
       }
    }/* End of for() */
    CMCHKPK(SPkU8, param->cellCfg.noOfBrdCstPlmn, mBuf);
    CMCHKPK(SPkU8, param->cellCfg.tddCfg.ulCycPrefix, mBuf);
    CMCHKPK(SPkU8, param->cellCfg.tddCfg.dlCycPrefix, mBuf);
    CMCHKPK(SPkU8, param->cellCfg.tddCfg.splSfPatrn, mBuf);
    CMCHKPK(SPkU8, param->cellCfg.tddCfg.sfAssgnmnt, mBuf);
    CMCHKPK(SPkU8, param->cellCfg.tddCfg.txBndWdth, mBuf);
    CMCHKPK(SPkU32, param->cellCfg.tddCfg.earfcn, mBuf);
    CMCHKPK(SPkU8, param->cellCfg.fddCfg.dlTxBndWdth, mBuf);
    CMCHKPK(SPkU8, param->cellCfg.fddCfg.ulTxBndWdth, mBuf);
    CMCHKPK(SPkU32, param->cellCfg.fddCfg.dlEarfcn, mBuf);
    CMCHKPK(SPkU32, param->cellCfg.fddCfg.ulEarfcn, mBuf);
    CMCHKPK(SPkU32, param->cellCfg.rsrpTrshldRange, mBuf);
    CMCHKPK(SPkU8, param->cellCfg.numOfMacPrm, mBuf);
    CMCHKPK(SPkU16, param->cellCfg.maxMacRntis, mBuf);
    CMCHKPK(SPkU16, param->cellCfg.macRntiStart, mBuf);
    CMCHKPK(SPkU16, param->cellCfg.maxRrmPrmbls, mBuf);
    CMCHKPK(SPkU16, param->cellCfg.rrmPrmblStart, mBuf);
    CMCHKPK(SPkU16, param->cellCfg.maxRrmRntis, mBuf);
    CMCHKPK(SPkU16, param->cellCfg.rrmRntiStart, mBuf);
    CMCHKPK(SPkU16, param->cellCfg.physCellId, mBuf);
#endif
   CMCHKPK(SPkU8, param->cellCfg.genUeCfg.ciphAlgo, mBuf);
   CMCHKPK(SPkU8, param->cellCfg.genUeCfg.intgAlgo, mBuf);
   CMCHKPK(SPkU8, param->cellCfg.freqBandInd, mBuf);
   CMCHKPK(SPkU16, param->cellCfg.trackAreaCode, mBuf);
   for(idx = 2; idx >= 0; idx--)
   {
     cmPkTknU8(&param->cellCfg.mnc[(U8)idx], mBuf);
     cmPkTknU8(&param->cellCfg.mcc[(U8)idx], mBuf);
   }
   CMCHKPK(SPkU8, param->cellCfg.maxUeSupp, mBuf);
   CMCHKPK(SPkU16, param->cellCfg.duplexMode, mBuf);
   CMCHKPK(SPkU8, param->cellCfg.modType, mBuf);
   CMCHKPK(SPkU16, param->cellCfg.cellId, mBuf);
#endif /* end of WR_RELAY */
   CMCHKPK(cmPkPst, &param->lmPst,mBuf);
   RETVALUE(ROK);
} /*end of function cmPkWrGenCfg*/

/*
*
*    Fun:    cmUnpkWrNghCfg
*
*    Desc:    Unpack the structure WrGenCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkWrNghCfg
(
LwrNeighCellCfg *param,
Buffer *mBuf,
Pst *pst
)
#else
PUBLIC S16 cmUnpkWrNghCfg(param, mBuf, pst)
LwrNeighCellCfg  *param;
Buffer *mBuf;
Pst *pst;
#endif
{
   RETVALUE(ROK);
}/* End of cmUnpkWrNghCfg() */


/*
*
*    Fun:    cmPkWrMmeCfg
*
*    Desc:    pack the structure WrGenCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkWrMmeCfg
(
LwrMmeCfg  *param,
Buffer       *mBuf
)
#else
PUBLIC S16 cmPkWrMmeCfg(param ,mBuf)
LwrMmeCfgLst  *param;
Buffer       *mBuf;
#endif
{
   RETVALUE(ROK);
}/* End of cmPkWrMmeCfg() */

/*
*
*    Fun:    cmUnpkWrMmeCfg
*
*    Desc:    Unpack the structure WrGenCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkWrMmeCfg
(
LwrMmeCfg  *param,
Buffer       *mBuf,
Pst          *pst
)
#else
PUBLIC S16 cmUnpkWrMmeCfg(param, mBuf, pst)
LwrMmeCfg    *param;
Buffer       *mBuf;
Pst          *pst;
#endif
{
   RETVALUE(ROK);
}

#ifdef WR_RELAY
/*
*
*    Fun:    cmPkWrUstaDgn
*
*    Desc:    pack the structure WrUstaDgn
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkWrUstaDgn
(
LwrUstaDgn *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkWrUstaDgn(param ,mBuf)
LwrUstaDgn *param;
Buffer *mBuf;
#endif
{
    TRC3(cmPkWrUstaDgn)

    switch( param->type )
    {
       case  LWR_USTA_DGNVAL_SAP :
          CMCHKPK(SPkS16, param->u.sapId,mBuf);
          break;
       case LWR_USTA_DGNVAL_CELLUEID :
          CMCHKPK(SPkU16, param->u.s.cellId,mBuf);
          CMCHKPK(SPkU16, param->u.s.udId,mBuf);
          break; 
       default:
          break;
    }
    CMCHKPK(SPkU8, param->type,mBuf);
    RETVALUE(ROK);
} /*end of function cmPkWrUstaDgn*/
#endif 
/*
*
*    Fun:    cmPkWrUsta
*
*    Desc:    pack the structure WrUsta
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkWrUsta
(
LwrUsta *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkWrUsta(param ,mBuf)
LwrUsta *param;
Buffer *mBuf;
#endif
{
    TRC3(cmPkWrUsta)

#ifdef WR_RELAY
    CMCHKPK(cmPkWrUstaDgn, &param->dgn,mBuf);
#endif
    CMCHKPK(cmPkCmAlarm, &param->alarm,mBuf);
    RETVALUE(ROK);
} /*end of function cmPkWrUsta*/

/*
*
*    Fun:    cmPkWrCntrl
*
*    Desc:    pack the structure WrCntrl
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkWrCntrl
(
LwrCntrl *param,
U16 elmnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkWrCntrl(param ,elmnt, mBuf)
LwrCntrl *param;
U16 elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmPkWrCntrl)

    switch( elmnt )
    {
       case  STWRGEN :
          if (param->action != ASHUTDOWN)
          {
             switch (param->subAction)
             {
                case  SADBG :
                   break;

                case SAUSTA:
                   break;  

                default:
                   RETVALUE(RFAILED);
             }
          }
          break;
#ifdef EU_DAT_APP
       case  STWREGTSAP :
          CMCHKPK(SPkS16, param->u.sapId,mBuf);
          break;
       case  STWRPJUSAP :
          CMCHKPK(SPkS16, param->u.sapId,mBuf);
          break;
#endif
       case STWRRGRSAP:
       case STWRCTFSAP:
       case STWRNHUSAP: 
       case STWRSZTSAP: 
#ifdef RM_INTF
       case STWRRMUSAP:  /*RRM->FSM sck changes*/
#endif /*RM_INTF*/
          CMCHKPK(SPkS16, param->u.sapId,mBuf);
          break;
       default:
          break;
    }
    CMCHKPK(SPkS16, param->subAction,mBuf);
    CMCHKPK(SPkS16, param->action,mBuf);

    RETVALUE(ROK);
} /*end of function cmPkWrCntrl*/
/*
*
*    Fun:    cmPkWrLSapCfg
*
*    Desc:    pack the structure WrLSapCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkWrLSapCfg
(
LwrLSapCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkWrLSapCfg(param ,mBuf)
LwrLSapCfg *param;
Buffer *mBuf;
#endif
{
    TRC3(cmPkWrLSapCfg)
    CMCHKPK(cmPkTmrCfg, &param->connTmr,mBuf);
    CMCHKPK(cmPkTmrCfg, &param->bndTmr,mBuf);
    CMCHKPK(SPkU8, param->maxBndRetry,mBuf);
    CMCHKPK(SPkU8, param->mem.pool,mBuf);
    CMCHKPK(SPkU8, param->mem.region,mBuf);
    CMCHKPK(SPkU8, param->dstSel,mBuf);
    CMCHKPK(SPkU8, param->dstRoute,mBuf);
    CMCHKPK(SPkU8, param->dstPrior,mBuf);
    CMCHKPK(SPkU8, param->dstInst,mBuf);
    CMCHKPK(SPkU8, param->dstEnt,mBuf);
    CMCHKPK(SPkU16, param->dstProcId,mBuf);
    CMCHKPK(SPkU8, param->srcInst,mBuf);
    CMCHKPK(SPkU8, param->srcEnt,mBuf);
    CMCHKPK(SPkS16, param->spId,mBuf);
    CMCHKPK(SPkS16, param->suId,mBuf);
    RETVALUE(ROK);
} /*end of function cmPkWrLSapCfg*/
/*
*
*    Fun:    cmPkWrCfg
*
*    Desc:    pack the structure WrCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkWrCfg
(
LwrCfg *param,
U16 elmnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkWrCfg(param ,elmnt, mBuf)
LwrCfg *param;
U16 elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmPkWrCfg)

    RETVALUE(ROK);
} /*end of function cmPkWrCfg*/
/*
*
*    Fun:    cmPkWrMngmt
*
*    Desc:    pack the structure WrMngmt
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkWrMngmt
(
LwrMngmt *param,
U8 evnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkWrMngmt(param ,evnt, mBuf)
LwrMngmt *param;
U8 evnt;
Buffer *mBuf;
#endif
{
    S16 ret1= ROK;
    TRC3(cmPkWrMngmt)

    switch( evnt )
    {
       case  EVTLWRCFGREQ :
          ret1 = cmPkWrCfg(&param->t.cfg, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLWRCNTRLREQ :
          ret1 = cmPkWrCntrl(&param->t.cntrl, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLWRSTAIND :
          CMCHKPK(cmPkWrUsta, &param->t.usta,mBuf);
          break;
       default:
          break;
    }
    if ((evnt != EVTLWRCFGREQ) && (evnt != EVTLWRCNTRLREQ) && (evnt != EVTLWRSTAIND ))
    {
    CMCHKPK(cmPkCmStatus, &param->cfm,mBuf);
    }
    CMCHKPK(cmPkHeader, &param->hdr,mBuf);
    RETVALUE(ROK);
} /*end of function cmPkWrMngmt*/
#endif
/*
*
*    Fun:    cmPkLwrCfgReq
*
*    Desc:    pack the primitive LveCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLwrCfgReq
(
Pst *pst,
LwrMngmt *cfg
)
#else
PUBLIC S16 cmPkLwrCfgReq(pst, cfg)
Pst *pst;
LwrMngmt *cfg;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLwrCfgReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ELWR001, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
       
    switch(pst->selector)
    {
#ifdef LCLWR 
       case LWR_SEL_LC:
    ret1 = cmPkWrMngmt(cfg, EVTLWRCFGREQ ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELWR002, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
          break;
#endif
#ifdef LWLCLWR
       case LWR_SEL_LWLC:
          CMCHKPKLOG(cmPkPtr, (PTR)cfg, mBuf, ELWR003, pst);
          break;
#endif
    }
    pst->event = (Event) EVTLWRCFGREQ;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLwrCfgReq*/


/*
*
*    Fun:    cmPkLwrDynCfgReq
*
*    Desc:   This primitive packs the Dynamic configuration
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLwrDynCfgReq
(
Pst   *pst,
LwrDynMngmt *dynCfg
)
#else
PUBLIC S16 cmPkLwrDynCfgReq(pst, dynCfg) 
Pst    *pst;
LwrDynMngmt *dynCfg;
#endif
{
    S16 ret1;
    Buffer *mBuf = NULLP;

    TRC3(cmPkLwrDynCfgReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELWR004, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
       
    switch(pst->selector)
    {
#ifdef LCLWR 
       case LWR_SEL_LC:
    ret1 = cmPkWrMngmt((LwrMngmt *)dynCfg, EVTLWRDYNCFGREQ, mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELWR005, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
          break;
#endif
#ifdef LWLCLWR
       case LWR_SEL_LWLC:
          CMCHKPKLOG(cmPkPtr, (PTR)dynCfg, mBuf, ELWR0029, pst);
          break;
#endif
    }
    pst->event = (Event) EVTLWRDYNCFGREQ;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLwrDynCfgReq*/ 


/*
*
*    Fun:    cmPkLwrLoadInd
*
*    Desc:    pack the primitive LveCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLwrLoadInd
(
Pst *pst,
LwrLoadIndInfo *loadIndInfo
)
#else
PUBLIC S16 cmPkLwrLoadInd(pst, loadIndInfo)
Pst *pst;
LwrLoadIndInfo *loadIndInfo;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLwrLoadInd)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ELWR007, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }

    CMCHKPK(SPkU8, loadIndInfo->cpuLoad, mBuf);
    pst->event = (Event) EVTLWRLOADIND;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLwrLoadInd*/
/*
*
*    Fun:    cmPkLwrCntrlReq
*
*    Desc:    pack the primitive LveCntrlReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLwrCntrlReq
(
Pst *pst,
LwrMngmt *cntrl
)
#else
PUBLIC S16 cmPkLwrCntrlReq(pst, cntrl)
Pst *pst;
LwrMngmt *cntrl;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLwrCntrlReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ELWR008, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
   switch(pst->selector)
   {

#ifdef LCLWR 
      case LWR_SEL_LC:
    ret1 = cmPkWrMngmt(cntrl, EVTLWRCNTRLREQ ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELWR009, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         break;
#endif
#ifdef LWLCLWR
      case LWR_SEL_LWLC:
         CMCHKPKLOG(cmPkPtr, (PTR)cntrl, mBuf, ELWR010, pst);
         break;
#endif 
   }
    pst->event = (Event) EVTLWRCNTRLREQ;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLwrCntrlReq*/
#if (defined(LCWRMILWR)|| defined (LWLCSMMILWR))
/*
*
*    Fun:    cmPkLwrCfgCfm
*
*    Desc:    pack the primitive LwrCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLwrCfgCfm
(
Pst *pst,
LwrMngmt *cfm
)
#else
PUBLIC S16 cmPkLwrCfgCfm(pst, cfm)
Pst *pst;
LwrMngmt *cfm;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLwrCfgCfm)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ELWR011, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }

    switch(pst->selector)
    {
       case LWR_SEL_LC:
#ifdef LCLWR 
    ret1 = cmPkWrMngmt(cfm, EVTLWRCFGCFM ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELWR012, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
    break;
#endif
#ifdef LWLCLWR
       case LWR_SEL_LWLC:
          CMCHKPKLOG(cmPkPtr, (PTR)cfm, mBuf, ELWR013, pst);
          break;
#endif 
    }

    pst->event = (Event) EVTLWRCFGCFM;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLwrCfgCfm*/

/*
*
*    Fun:    cmPkLwrUpDateNhgCellCfgReq 
*
*    Desc:    unpack the primitive  
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLwrUpDateNhgCellCfgReq 
(
Pst *pst,
LwrMngmt *cfg
)
#else
PUBLIC S16 cmPkLwrUpDateNhgCellCfgReq(pst, cfg)
Pst *pst;
LwrMngmt *cfg;
#endif
{
  S16 ret1;
  Buffer *mBuf;
  mBuf = NULLP;
  TRC3(cmPkLwrUpDateNhgCellCfgReq)

  if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
  {
     RLOG0(L_FATAL, "Memory allocation failed.");
#if (ERRCLASS & ERRCLS_ADD_RES)
     if(ret1 != ROK)
     {
        SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
              __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
              (ErrVal)ELWR001, (ErrVal)0, "SGetMsg() failed");
     }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
    switch(pst->selector)
    {
#ifdef LWLCLWR
       case LWR_SEL_LWLC:
          CMCHKPKLOG(cmPkPtr, (PTR)cfg, mBuf, ELWR003, pst);
          break;
#endif
    }
    pst->event = (Event) EVTLWRADDNGHCELLCFGREQ;
    RETVALUE(SPstTsk(pst,mBuf));

}/*end of cmPkLwrUpDateNhgCellCfgReq*/

/*
*
*    Fun:    cmPkLwrStaInd
*
*    Desc:    pack the primitive LwrStaInd
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLwrStaInd
(
Pst *pst,
LwrMngmt *sta
)
#else
PUBLIC S16 cmPkLwrStaInd(pst, sta)
Pst *pst;
LwrMngmt *sta;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLwrStaInd)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ELWR014, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
   switch(pst->selector)
   {
      case LWR_SEL_LC:
#ifdef LCLWR 
    ret1 = cmPkWrMngmt(sta, EVTLWRSTAIND ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELWR015, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         break;
#endif
#ifdef LWLCLWR
      case LWR_SEL_LWLC:
         CMCHKPKLOG(cmPkPtr, (PTR)sta, mBuf, ELWR016, pst);
         break;
#endif 
   } 
    pst->event = (Event) EVTLWRSTAIND;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLwrStaInd*/


/*
*
*    Fun:    cmPkLwrCntrlCfm
*
*    Desc:    pack the primitive LveCntrlCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLwrCntrlCfm
(
Pst *pst,
LwrMngmt *cfm
)
#else
PUBLIC S16 cmPkLwrCntrlCfm(pst, cfm)
Pst *pst;
LwrMngmt *cfm;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLwrCntrlCfm)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ELWR017, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }

   switch(pst->selector)
   {
#ifdef LCLWR 
      case LWR_SEL_LC:
    ret1 = cmPkWrMngmt(cfm, EVTLWRCNTRLCFM ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELWR018, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
         break;
#endif
#ifdef LWLCLWR
      case LWR_SEL_LWLC:
         CMCHKPKLOG(cmPkPtr, (PTR)cfm, mBuf, ELWR019, pst);
         break;
#endif 
   } 
    pst->event = (Event) EVTLWRCNTRLCFM;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLwrCntrlCfm*/


/*
*
*    Fun:    cmPkLwrDynCfgCfm
*
*    Desc:    pack the primitive LveCntrlCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/


#ifdef ANSI
PUBLIC S16 cmPkLwrDynCfgCfm
(
Pst         *pst,
LwrDynMngmt *cfgCfm
)
#else
PUBLIC S16 cmPkLwrDynCfgCfm(pst, cfgCfm)
Pst         *pst;
LwrDynMngmt *cfgCfm;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLwrCntrlCfm)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ELWR017, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }

   switch(pst->selector)
   {
#ifdef LCLWR 
      case LWR_SEL_LC:
#endif
#ifdef LWLCLWR
      case LWR_SEL_LWLC:

         CMCHKPKLOG(cmPkPtr, (PTR)cfgCfm, mBuf, ELWR019, pst);

         break;
#endif 
   } 
    pst->event = (Event) EVTLWRDYNCFGCFM;
    RETVALUE(SPstTsk(pst,mBuf));
} /*end of function cmPkLwrCntrlCfm*/


#endif

#ifdef LCLWR 
/*
*
*    Fun:    cmUnpkWrGenCfg
*
*    Desc:    unpack the structure wrGenCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkWrGenCfg
(
LwrGenCfg *param,
Buffer *mBuf,
Pst *pst
)
#else
PUBLIC S16 cmUnpkWrGenCfg(param ,mBuf, pst)
LwrGenCfg *param;
Buffer *mBuf;
Pst *pst;
#endif
{
    U8 idx = 0;
    S16 loopCnt = 0;
    S16 loopCnt1 = 0;
    TRC3(mUnpkWrGenCfg)

    CMCHKUNPK(cmUnpkPst, &param->lmPst,mBuf);
#ifdef WR_RELAY
    CMCHKUNPK(SUnpkU16, &param->cellCfg.cellId, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.modType, mBuf);
    CMCHKUNPK(SUnpkU16, &param->cellCfg.duplexMode, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.maxUeSupp, mBuf);
    for (idx = 0; idx < 3; idx++)
    {
       cmUnpkTknU8(&param->cellCfg.mcc[idx], mBuf);
       cmUnpkTknU8(&param->cellCfg.mnc[idx], mBuf);
    }
    CMCHKUNPK(SUnpkU16, &param->cellCfg.trackAreaCode, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.freqBandInd, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.genUeCfg.intgAlgo, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.genUeCfg.ciphAlgo, mBuf);
#ifdef LTE_HO_SUPPORT
    CMCHKUNPK(SUnpkU16, &param->cellCfg.physCellId, mBuf);
    CMCHKUNPK(SUnpkU16, &param->cellCfg.rrmRntiStart, mBuf);
    CMCHKUNPK(SUnpkU16, &param->cellCfg.maxRrmRntis, mBuf);
    CMCHKUNPK(SUnpkU16, &param->cellCfg.rrmPrmblStart, mBuf);
    CMCHKUNPK(SUnpkU16, &param->cellCfg.maxRrmPrmbls, mBuf);
    CMCHKUNPK(SUnpkU16, &param->cellCfg.macRntiStart, mBuf);
    CMCHKUNPK(SUnpkU16, &param->cellCfg.maxMacRntis, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.numOfMacPrm, mBuf);
    CMCHKUNPK(SUnpkU32, &param->cellCfg.rsrpTrshldRange, mBuf);
    CMCHKUNPK(SUnpkU32, &param->cellCfg.fddCfg.ulEarfcn, mBuf);
    CMCHKUNPK(SUnpkU32, &param->cellCfg.fddCfg.dlEarfcn, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.fddCfg.ulTxBndWdth, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.fddCfg.dlTxBndWdth, mBuf);
    CMCHKUNPK(SUnpkU32, &param->cellCfg.tddCfg.earfcn, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.tddCfg.txBndWdth, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.tddCfg.sfAssgnmnt, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.tddCfg.splSfPatrn, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.tddCfg.dlCycPrefix, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.tddCfg.ulCycPrefix, mBuf);
    CMCHKUNPK(SUnpkU8, &param->cellCfg.noOfBrdCstPlmn, mBuf);
    for (loopCnt = 0; loopCnt < param->cellCfg.noOfBrdCstPlmn; loopCnt++)
    {
       for (loopCnt1 = 0; loopCnt1 < 3; loopCnt1++)
       {
          //cmUnpkTknU8(&param->cellCfg.plmnId[loopCnt][loopCnt1], mBuf);  
          SUnpkU8(&param->cellCfg.plmnId[loopCnt][loopCnt1], mBuf);
       }
    }/* End of for() */
#endif

#endif
   CMCHKUNPK(cmUnpkCmTptAddr, &param->dataAppAddr, mBuf);
#ifdef LTE_HO_SUPPORT
   CMCHKUNPK(SUnpkU8, &param->eNBType, mBuf);
   CMCHKUNPK(SUnpkU16, &param->noOfNghInfo, mBuf);
   CMCHKUNPK(SUnpkU8, &param->noOfguGrp, mBuf);
   CMCHKUNPK(SUnpkU32, &param->ueHoAttachTimerVal, mBuf);
   CMCHKUNPK(SUnpkU16, &param->expiryCnt, mBuf);
   CMCHKUNPK(SUnpkU32, &param->inactvTmrVal, mBuf);
#endif
   RETVALUE(ROK);
} /*end of function cmUnpkWrGenCfg*/
#ifdef WR_RELAY
/*
*
*    Fun:    cmUnpkWrUstaDgn
*
*    Desc:    unpack the structure wrUstaDgn
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkWrUstaDgn
(
LwrUstaDgn *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkWrUstaDgn(param ,mBuf)
LwrUstaDgn *param;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkWrUstaDgn)

    CMCHKUNPK(SUnpkU8, &param->type,mBuf);
    switch( param->type )
    {
       case LWR_USTA_DGNVAL_SAP :
          CMCHKUNPK(SUnpkS16, &param->u.sapId,mBuf);
          break;
       case LWR_USTA_DGNVAL_CELLUEID :
          CMCHKUNPK(SUnpkU16, &param->u.s.udId,mBuf);
          CMCHKUNPK(SUnpkU16, &param->u.s.cellId,mBuf);
          break; 
       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmUnpkWrUstaDgn*/
#endif 

/*
*
*    Fun:    cmUnpkWrUsta
*
*    Desc:    unpack the structure wrUsta
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkWrUsta
(
LwrUsta *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkWrUsta(param ,mBuf)
LwrUsta *param;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkWrUsta)

    CMCHKUNPK(cmUnpkCmAlarm, &param->alarm,mBuf);
#ifdef WR_RELAY
    CMCHKUNPK(cmUnpkWrUstaDgn, &param->dgn,mBuf);
#endif
    RETVALUE(ROK);
} /*end of function cmUnpkWrUsta*/
#endif
/*
*
*    Fun:    cmUnpkWrCntrl
*
*    Desc:    unpack the structure WrCntrl
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkWrCntrl
(
LwrCntrl *param,
U16 elmnt,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkWrCntrl(param ,elmnt, mBuf)
LwrCntrl *param;
U16 elmnt;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkWrCntrl)

    CMCHKUNPK(SUnpkS16, &param->action,mBuf);
    CMCHKUNPK(SUnpkS16, &param->subAction,mBuf);
    switch( elmnt )
    {
       case  STWRGEN :
          if (param->action != ASHUTDOWN)
          {
             switch (param->subAction)
             {
                case  SADBG :
                   break;

               case SAUSTA:
                  break;  

                default:
                   RETVALUE(RFAILED);
             }
          }
          break;
#ifdef EU_DAT_APP
       case  STWREGTSAP :
          CMCHKUNPK(SUnpkS16, &param->u.sapId,mBuf);
          break;
       case  STWRPJUSAP :
          CMCHKUNPK(SUnpkS16, &param->u.sapId,mBuf);
          break;
#endif

       case STWRRGRSAP:
       case STWRCTFSAP:
       case STWRNHUSAP: 
       case STWRSZTSAP: 
#ifdef RM_INTF
       case STWRRMUSAP:  /*RRM->FSM sck changes*/
#endif /*RM_INTF*/
          CMCHKUNPK(SUnpkS16, &param->u.sapId,mBuf);
          break;
       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmUnpkWrCntrl*/

#ifdef LCLWR 
/*
*
*    Fun:    cmUnpkWrLSapCfg
*
*    Desc:    unpack the structure wrLSapCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkWrLSapCfg
(
LwrLSapCfg *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkWrLSapCfg(param ,mBuf)
LwrLSapCfg *param;
Buffer *mBuf;
#endif
{
    TRC3(cmUnpkWrLSapCfg)
    CMCHKUNPK(SUnpkS16, &param->suId,mBuf);
    CMCHKUNPK(SUnpkS16, &param->spId,mBuf);
    CMCHKUNPK(SUnpkU8, &param->srcEnt,mBuf);
    CMCHKUNPK(SUnpkU8, &param->srcInst,mBuf);
    CMCHKUNPK(SUnpkU16, &param->dstProcId,mBuf);
    CMCHKUNPK(SUnpkU8, &param->dstEnt,mBuf);
    CMCHKUNPK(SUnpkU8, &param->dstInst,mBuf);
    CMCHKUNPK(SUnpkU8, &param->dstPrior,mBuf);
    CMCHKUNPK(SUnpkU8, &param->dstRoute,mBuf);
    CMCHKUNPK(SUnpkU8, &param->dstSel,mBuf);
    CMCHKUNPK(SUnpkU8, &param->mem.region,mBuf);
    CMCHKUNPK(SUnpkU8, &param->mem.pool,mBuf);
    CMCHKUNPK(SUnpkU8, &param->maxBndRetry,mBuf);
    CMCHKUNPK(cmUnpkTmrCfg, &param->bndTmr,mBuf);
    CMCHKUNPK(cmUnpkTmrCfg, &param->connTmr,mBuf);
    RETVALUE(ROK);
} /*end of function cmUnpkWrLSapCfg*/
/*
*
*    Fun:    cmUnpkWrCfg
*
*    Desc:    unpack the structure WrCfg
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkWrCfg
(
LwrCfg *param,
U16 elmnt,
Buffer *mBuf,
Pst *pst
)
#else
PUBLIC S16 cmUnpkWrCfg(param ,elmnt, mBuf, pst)
LwrCfg *param;
U16 elmnt;
Buffer *mBuf;
Pst *pst;
#endif
{
    TRC3(cmUnpkWrCfg)

    switch( elmnt )
    {
       case STWRRGRSAP:
       case STWRCTFSAP:
       case STWRNHUSAP: 
       case STWRSZTSAP:
#ifdef RM_INTF
       case STWRRMUSAP: /*RRM->FSM sck changes*/
#endif /*RM_INTF*/
#ifdef LTE_HO_SUPPORT
       case STWRCZTSAP:
#endif
          CMCHKUNPK(cmUnpkWrLSapCfg, &param->t.lSap,mBuf);
          break;
       case STWRGEN:
          /* since pst needs to be passed to cmUnpkWrGenCfg() and 
             CMCHKUNPK does not take more than 3 args, we are 
             directly invoking cmUnpkWrGenCfg() */
          cmUnpkWrGenCfg(&param->t.genCfg,mBuf, pst);
          break;
       case STWRNGH:
          /* since pst needs to be passed to cmUnpkWrNghCfg() and 
             CMCHKUNPK does not take more than 3 args, we are 
             directly invoking cmUnpkWrNghCfg() 
          cmUnpkWrNghCfg(&param->t.NghCellCfgLst, mBuf, pst);*/
          break;
       case STWRMME:
          /* since pst needs to be passed to cmUnpkWrMmeCfg() and 
             CMCHKUNPK does not take more than 3 args, we are 
             directly invoking cmUnpkWrMmeCfg() *
          cmUnpkWrMmeCfg(&param->t.mmeCfgLst, mBuf, pst);*/
          break;
#ifdef EU_DAT_APP
       case  STWREGTSAP :
          CMCHKUNPK(cmUnpkWrLSapCfg, &param->t.lSap,mBuf);
          break;
       case  STWRPJUSAP :
          CMCHKUNPK(cmUnpkWrLSapCfg, &param->t.lSap,mBuf);
          break;
#endif
       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmUnpkWrCfg*/
/*
*
*    Fun:    cmUnpkWrMngmt
*
*    Desc:    unpack the structure WrMngmt
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkWrMngmt
(
LwrMngmt *param,
U8 evnt,
Ptr ptr,
Buffer *mBuf,
Pst *pst
)
#else
PUBLIC S16 cmUnpkWrMngmt(param ,evnt, ptr, mBuf, pst)
LwrMngmt *param;
U8 evnt;
Ptr ptr;
Buffer *mBuf;
Pst *pst;
#endif
{
    S16 ret1;
    TRC3(cmUnpkWrMngmt)

    UNUSED(ptr);

    CMCHKUNPK(cmUnpkHeader, &param->hdr,mBuf);
    if( ( evnt != EVTLWRCFGREQ) && (evnt != EVTLWRCNTRLREQ) && (evnt != EVTLWRSTAIND ))
    {
    CMCHKUNPK(cmUnpkCmStatus, &param->cfm,mBuf);
    }
    switch( evnt )
    {
       case  EVTLWRCFGREQ :
          ret1 = cmUnpkWrCfg(&param->t.cfg, param->hdr.elmId.elmnt ,mBuf, pst);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLWRCNTRLREQ :
          ret1 = cmUnpkWrCntrl(&param->t.cntrl, param->hdr.elmId.elmnt ,mBuf);
          if (ret1 != ROK)
          {
             RETVALUE(RFAILED);
          }
          break;
       case  EVTLWRSTAIND :
          CMCHKUNPK(cmUnpkWrUsta, &param->t.usta,mBuf);
          break;
       default:
          break;
    }
    RETVALUE(ROK);
} /*end of function cmUnpkWrMngmt*/
#endif
#if (defined(LCWRMILWR)|| defined (LWLCSMMILWR))
/*
*
*    Fun:   cmUnpkLwrNghCfgInd 
*
*    Desc:    unpack the primitive LwrNghCfgInd
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLwrNghCfgInd 
(
LwrCfgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLwrNghCfgInd(func, pst, mBuf)
LwrCfgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
#ifdef LWLCLWR
   LwrMngmt *cfg = NULLP;
#else
   LwrMngmt cfg;
#endif
    
    TRC3(cmUnpkLwrNghCfgInd)

      switch(pst->selector)
      {
#ifdef LCLWR 
         case LWR_SEL_LC:
    ret1 = cmUnpkWrMngmt((LwrMngmt *)&cfg, EVTLWRNGHCFGREQ, NULLP ,mBuf, pst);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELWR020, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
            break;
#endif
#ifdef LWLCLWR
         case  LWR_SEL_LWLC:
            CMCHKUNPKLOG(cmUnpkPtr, (PTR*) &cfg, mBuf, (ErrVal)ELWR021, pst);

            break;
#endif
      }
    SPutMsg(mBuf);
#ifdef LWLCLWR   
   ret1 = (*func)(pst, cfg);
   RETVALUE(ret1);
#else
    RETVALUE((*func)(pst, &cfg));
#endif
} /*end of function cmUnpkLwrNghCfgInd*/

/*
*
*    Fun:    cmUnpkLwrCfgReq
*
*    Desc:    unpack the primitive LwrCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLwrCfgReq
(
LwrCfgReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLwrCfgReq(func, pst, mBuf)
LwrCfgReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
#ifdef LWLCLWR
   LwrMngmt *cfg = NULLP;
#else
   LwrMngmt cfg;
#endif
    
    TRC3(cmUnpkLwrCfgReq)

      switch(pst->selector)
      {
#ifdef LCLWR 
         case LWR_SEL_LC:
    ret1 = cmUnpkWrMngmt((LwrMngmt *)&cfg, EVTLWRCFGREQ , NULLP ,mBuf, pst);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELWR020, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
            break;
#endif
#ifdef LWLCLWR
         case  LWR_SEL_LWLC:
            CMCHKUNPKLOG(cmUnpkPtr, (PTR*) &cfg, mBuf, (ErrVal)ELWR021, pst);

            break;
#endif
      }
    SPutMsg(mBuf);
#ifdef LWLCLWR   
   ret1 = (*func)(pst, cfg);
   RETVALUE(ret1);
#else
    RETVALUE((*func)(pst, &cfg));
#endif
} /*end of function cmUnpkLwrCfgReq*/


/*
*
*    Fun:    cmUnpkLwrDynCfgReq
*
*    Desc:    unpack the primitive LwrCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLwrDynCfgReq
(
LwrDynCfgReq func,
Pst    *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLwrDynCfgReq(func, pst, mBuf)
LwrDynCfgReq func;
Pst    *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
#ifdef LWLCLWR
   LwrDynMngmt *dynCfg = NULLP;
#else
   Void dynCfg;
#endif
    
    TRC3(cmUnpkLwrDynCfgReq)

      switch(pst->selector)
      {
#ifdef LCLWR 
         case LWR_SEL_LC:
    ret1 = cmUnpkWrMngmt((LwrMngmt *)&dynCfg, EVTLWRDYNCFGREQ, NULLP ,mBuf, pst);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELWR020, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
            break;
#endif
#ifdef LWLCLWR
         case  LWR_SEL_LWLC:
            CMCHKUNPKLOG(cmUnpkPtr, (PTR*) &dynCfg, mBuf, (ErrVal)ELWR021, pst);
            break;
#endif
      }
    SPutMsg(mBuf);
#ifdef LWLCLWR   
   ret1 = (*func)(pst,dynCfg);
   RETVALUE(ret1);
#else
    RETVALUE((*func)(pst, &dynCfg));
#endif
} /*end of function cmUnpkLwrDynCfgReq */

/*
*
*       Fun:   smUnPackStartStopStackReq
*
*       Desc:  un packing and sending  stack start and stop request
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_lwr.c
*
*/ 
   
PUBLIC S16 smUnPackStartStopStackReq(LwrStartStopStackReq func, Pst *pst, Buffer *mBuf)
{
   S16  ret = ROK;
   U8 isPerdREMReq,isStackStart,isImmediate;

   TRC3(smUnPackStartStopStackReq);

   SRemPreMsgMult((Data *)&(isImmediate), (MsgLen) sizeof(U8), mBuf);
   SRemPreMsgMult((Data *)&(isStackStart), (MsgLen) sizeof(U8), mBuf);
   SRemPreMsgMult((Data *)&(isPerdREMReq), (MsgLen) sizeof(U8), mBuf);

   SPutMsg(mBuf);
   (*func)(isPerdREMReq,isStackStart,isImmediate);

   RETVALUE(ret);
} /* end of smUnPackStartStopStackReq */
 
/*
*
*       Fun:   cmUnPackSendStaticCfg
*
*       Desc:  un packing and sending static configuration to APP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_lwr.c
*
*/ 

PUBLIC S16 cmUnPackSendStaticCfg(LwrStaticCfgReqSend func, Pst *pst, Buffer *mBuf)
{
   S16  ret = ROK;

   TRC3(cmUnPackSendStaticCfg);

   SPutMsg(mBuf);
   ret = (*func)();

   RETVALUE(ret);
} /* end of cmUnPackSendStaticCfg */ 

/*
*
*       Fun:   cmUnPackSendDynCfg
*
*       Desc:  un packing and sending dynamic configuration to APP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_lwr.c
*
*/ 

PUBLIC S16 cmUnPackSendDynCfg(LwrDynCfgReqSend func, Pst *pst, Buffer *mBuf)
{
   S16  ret = ROK;
   U32  len = 0;
   U32  cfgType = 0;
   void *cfg = NULLP;
   U8   *pkArray;

   TRC3(cmUnPackSendDynCfg);

   SRemPreMsgMult((Data *)&cfgType, (MsgLen) sizeof(U32), mBuf);
   SRemPreMsgMult((Data *)&len, (MsgLen) sizeof(U32), mBuf);

   WR_ALLOC(&pkArray, len);
   SRemPreMsgMult((Data *)&pkArray[0], (MsgLen)len, mBuf);
   cfg = &pkArray[0];

   SPutMsg(mBuf);
   ret = (*func)(cfg, cfgType, 0);

   RETVALUE(ret);
} /* end of cmUnPackSendDynCfg */
 


/*
*
*    Fun:    cmUnpkLwrCntrlReq
*
*    Desc:    unpack the primitive LwrCntrlReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLwrCntrlReq
(
LwrCntrlReq func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLwrCntrlReq(func, pst, mBuf)
LwrCntrlReq func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
#ifdef LWLCLWR
   LwrMngmt *cntrl = NULLP;
#else
   LwrMngmt cntrl;
#endif
    
    TRC3(cmUnpkLwrCntrlReq)
      switch(pst->selector)
      {
#ifdef LCLWR 
         case LWR_SEL_LC:
    ret1 = cmUnpkWrMngmt((LwrMngmt *)&cntrl, EVTLWRCNTRLREQ , NULLP ,mBuf, pst);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELWR022, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
            break;
#endif
#ifdef LWLCLWR
         case  LWR_SEL_LWLC:
            CMCHKUNPKLOG(cmUnpkPtr, (PTR*)&cntrl, mBuf, (ErrVal)ELWR023, pst);
            break;
#endif
      }   
    SPutMsg(mBuf);
#ifdef LWLCLWR   
   ret1 = (*func)(pst, cntrl);
/*   SPutSBuf(pst->region, pst->pool, (Data *) cntrl, sizeof(LwrMngmt));*/
   RETVALUE(ret1);
#else
    RETVALUE((*func)(pst, &cntrl));
#endif 
} /*end of function cmUnpkLwrCntrlReq*/



/*
*
*    Fun:    cmUnpkLwrLoadInd
*
*    Desc:    unpack the primitive LwrLoadInd
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLwrLoadInd
(
LwrLoadInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLwrLoadInd(func, pst, mBuf)
LwrLoadInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
    LwrLoadIndInfo loadInfo;

    TRC3(cmUnpkLwrLoadInd)

    CMCHKUNPK(SUnpkU8, &(loadInfo.cpuLoad), mBuf);

    SPutMsg(mBuf);
    RETVALUE((*func)(pst, &loadInfo));
} /*end of function cmUnpkLwrLoadInd*/

#endif /* LCWRMILWR */


/*
*
*    Fun:    cmUnpkLwrCfgCfm
*
*    Desc:    unpack the primitive LwrCfgCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLwrCfgCfm
(
LwrCfgCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLwrCfgCfm(func, pst, mBuf)
LwrCfgCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
#ifdef LWLCLWR
   LwrMngmt *cfm = NULLP;
#else
   LwrMngmt cfm;
#endif
    
    TRC3(cmUnpkLwrCfgCfm)

      switch(pst->selector)
      {
#ifdef LCLWR 
         case LWR_SEL_LC:

    ret1 = cmUnpkWrMngmt((LwrMngmt *)&cfm, EVTLWRCFGCFM , NULLP ,mBuf, pst);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELWR024, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
            break;
#endif
#ifdef LWLCLWR
         case  LWR_SEL_LWLC:
            CMCHKUNPKLOG(cmUnpkPtr, (PTR*) &cfm, mBuf, (ErrVal)ELWR025, pst);
            break;
#endif 
      }

    SPutMsg(mBuf);
#ifdef LWLCLWR
   ret1 = (*func)(pst, cfm);
   SPutSBuf(pst->region, pst->pool, (Data *) cfm, sizeof(LwrMngmt));
   RETVALUE(ret1);
#else
    RETVALUE((*func)(pst, &cfm));
#endif
} /*end of function cmUnpkLwrCfgCfm*/

/*
*
*    Fun:     cmUnpkLwrUpDateNhgCellCfgReq 
*
*    Desc:    unpack the primitive  
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLwrUpDateNhgCellCfgReq 
(
LwrUpDateNhgCellCfg func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLwrUpDateNhgCellCfgReq(func, pst, mBuf)
LwrUpDateNhgCellCfg func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
#ifdef LWLCLWR
   LwrMngmt *cfg = NULLP;
#else
   LwrMngmt cfg;
#endif
    
    TRC3(cmUnpkLwrUpDateNhgCellCfgReq)

      switch(pst->selector)
      {
#ifdef LWLCLWR
         case  LWR_SEL_LWLC:
            CMCHKUNPKLOG(cmUnpkPtr, (PTR*) &cfg, mBuf, (ErrVal)ELWR025, pst);
            break;
#endif 
      }
    SPutMsg(mBuf);
#ifdef LWLCLWR
   ret1 = (*func)(pst, cfg);
   RETVALUE(ret1);
#else
    RETVALUE((*func)(pst, &cfg));
#endif
}/*end of cmUnpkLwrUpDateNhgCellCfgReq*/
/*
*
*    Fun:    cmUnpkLwrStaInd
*
*    Desc:    unpack the primitive LwrStaInd
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLwrStaInd
(
LwrStaInd func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLwrStaInd(func, pst, mBuf)
LwrStaInd func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
#ifdef LWLCLWR
    LwrMngmt *sta = NULLP;
#else
    LwrMngmt sta;
#endif
    
    TRC3(cmUnpkLwrStaInd)
    switch(pst->selector)
    {
#ifdef LCLWR 
       case LWR_SEL_LC:
          ret1 = cmUnpkWrMngmt((LwrMngmt *)&sta, EVTLWRSTAIND, NULLP ,mBuf, pst);
#if(ERRCLASS & ERRCLS_DEBUG)
          if(ret1 != ROK)
          {
             SPutMsg(mBuf);
             SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                   __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                   (ErrVal)ELWR024, (ErrVal)ret1, "Unpacking failure");
             RETVALUE( ret1 );
          }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
          break;
#endif
#ifdef LWLCLWR
       case  LWR_SEL_LWLC:
          CMCHKUNPKLOG(cmUnpkPtr, (PTR*) &sta, mBuf, (ErrVal)ELWR025, pst);
          break;
#endif 
    }

    SPutMsg(mBuf);
#ifdef LWLCLWR
   ret1 = (*func)(pst, sta);
   SPutSBuf(pst->region, pst->pool, (Data *) sta, sizeof(LwrMngmt));
   RETVALUE(ret1);
#else
    RETVALUE((*func)(pst, &cfm));
#endif 
} /*end of function cmUnpkLwrStaInd*/


/*
*
*    Fun:    cmUnpkLwrDynCfgCfm
*
*    Desc:   unpack the primitive LwrCntrlCfm
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLwrDynCfgCfm
(
LwrDynCfgCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLwrDynCfgCfm(func, pst, mBuf)
LwrDynCfgCfm func;
Pst *pst;
Buffer *mBuf;
#endif 
{

   S16 ret1;
#ifdef LWLCLWR
   LwrDynMngmt *dynCfgCfm = NULLP;
#endif
   LwrMngmt cfm;  

   TRC3(cmUnpkLwrDynCfgCfm)

      switch(pst->selector)
      {
#ifdef LCLWR 
         case LWR_SEL_LC:
            ret1 = cmUnpkWrMngmt(&cfm, EVTLWRCFGCFM , NULLP ,mBuf, pst);
#if(ERRCLASS & ERRCLS_DEBUG)
            if(ret1 != ROK)
            {
               SPutMsg(mBuf);
               SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
                     __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
                     (ErrVal)ELWR024, (ErrVal)ret1, "Unpacking failure");
               RETVALUE( ret1 );
            }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
            break;
#endif
#ifdef LWLCLWR
         case  LWR_SEL_LWLC:
            CMCHKUNPKLOG(cmUnpkPtr, (PTR*) &dynCfgCfm, mBuf, (ErrVal)ELWR025, pst);
            break;
#endif 
      }

   SPutMsg(mBuf);
#ifdef LWLCLWR
   ret1 = (*func)(pst, dynCfgCfm);
   RETVALUE(ret1);
#else
   RETVALUE((*func)(pst, &cfm));
#endif 

}/* end of cmUnpkLwrDynCfgCfm */


/*
*
*    Fun:    cmUnpkLwrCntrlCfm
*
*    Desc:    unpack the primitive LwrCntrlCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLwrCntrlCfm
(
LwrCntrlCfm func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLwrCntrlCfm(func, pst, mBuf)
LwrCntrlCfm func;
Pst *pst;
Buffer *mBuf;
#endif
{
    S16 ret1;
#ifdef LWLCLWR
   LwrMngmt *cfm = NULLP;
#else
   LwrMngmt cfm;
#endif
    
    TRC3(cmUnpkLwrCntrlCfm)
      switch(pst->selector)

      {
#ifdef LCLWR 
    ret1 = cmUnpkWrMngmt((LwrMngmt *)&cfm, EVTLWRCNTRLCFM , NULLP ,mBuf, pst);
#if(ERRCLASS & ERRCLS_DEBUG)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_DEBUG,
            (ErrVal)ELWR027, (ErrVal)ret1, "Unpacking failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_DEBUG   */
         break;
#endif
#ifdef LWLCLWR
         case  LWR_SEL_LWLC:
         CMCHKUNPKLOG(cmUnpkPtr, (PTR*) &cfm, mBuf, (ErrVal)ELWR028, pst);
         break;
#endif 
      }

   SPutMsg(mBuf); 
#ifdef LWLCLWR 
   ret1 = (*func)(pst, cfm);
   SPutSBuf(pst->region, pst->pool, (Data *) cfm, sizeof(LwrMngmt));
   RETVALUE(ret1);
#else
    RETVALUE((*func)(pst, &cfm));
#endif
} /*end of function cmUnpkLwrCntrlCfm*/

/*
*
*    Fun:    cmPkLwrAddNeighCfgReq
*
*    Desc:    pack the primitive LveCntrlCfm
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLwrAddNeighCfgReq
(
Pst *pst,
LwrMngmt *cfg
)
#else
PUBLIC S16 cmPkLwrAddNeighCfgReq(pst, cfg)
Pst *pst;
LwrMngmt *cfg;
#endif
{
    S16 ret1;
    Buffer *mBuf;
    mBuf = NULLP;
    TRC3(cmPkLwrCfgReq)

    if((ret1 = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret1 != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
                (ErrVal)ELWR001, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret1);
    }
       
    switch(pst->selector)
    {
#ifdef LCLWR 
       case LWR_SEL_LC:
    ret1 = cmPkWrMngmt(cfg, EVTLWRCFGREQ ,mBuf);
#if(ERRCLASS & ERRCLS_ADD_RES)
    if(ret1 != ROK)
    {
       SPutMsg(mBuf);
       SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
             __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ELWR002, (ErrVal)ret1, "Packing failure");
       RETVALUE( ret1 );
    }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
          break;
#endif
#ifdef LWLCLWR
       case LWR_SEL_LWLC:
          CMCHKPKLOG(cmPkPtr, (PTR)cfg, mBuf, ELWR003, pst);
          break;
#endif
    }
    pst->event = (Event) EVTLWRCFGREQ;
    RETVALUE(SPstTsk(pst,mBuf));

} /*end of function cmPkLwrAddNeighCfgReq*/
/*
*
*    Fun:    cmPkLwrAddNeighCfm
*
*    Desc:    pack the primitive LveCfgReq
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLwrAddNeighCfm
(
Pst *pst,
LwrMngmt *cfg
)
#else
PUBLIC S16 cmPkLwrAddNeighCfm(pst, cfg)
Pst *pst;
LwrMngmt *cfg;
#endif
{                            
    TRC3(cmPkLwrAddNeighCfm)
    RETVALUE(ROK);
}   



/*
*
*    Fun:    cmPkLwrNghCellInd
*
*    Desc:   This primitive packs the neighbour cell indication
*
*    Ret:    ROK  -ok
*
*    Notes:    None
*
*    File:     wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmPkLwrNghCellInd
(
Pst   *pst,
LwrNghCellInd *nghCellInd
)
#else
PUBLIC S16 cmPkLwrNghCellInd(pst, nghCellInd) 
Pst    *pst;
LwrNghCellInd *nghCellInd;
#endif
{
    S16 ret;
    Buffer *mBuf = NULLP;

    TRC3(cmPkLwrNghCellInd)

    if((ret = SGetMsg(pst->region, pst->pool, &mBuf)) != ROK)
    {
#if (ERRCLASS & ERRCLS_ADD_RES)
       if(ret != ROK)
       {
          SLogError(pst->srcEnt, pst->srcInst, pst->srcProcId,
                __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
               (ErrVal)ELWR004, (ErrVal)0, "SGetMsg() failed");
       }
#endif /*  ERRCLASS & ERRCLS_ADD_RES  */
       RETVALUE(ret);
    }
       
    switch(pst->selector)
    {
       case LWR_SEL_LWLC:
          CMCHKPKLOG(cmPkPtr, (PTR)nghCellInd, mBuf, ELWR029, pst);
          break;
       default:
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
    }
    pst->event = (Event) EVTNGHCELLIND;
    RETVALUE(SPstTsk(pst,mBuf));
} 


/*
*
*    Fun:    cmUnpkLwrDynCfgCfm
*
*    Desc:   unpack the neighbour cell indication
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   wr_lwr.c
*
*/
#ifdef ANSI
PUBLIC S16 cmUnpkLwrNghCellInd
(
LwrNghCellIndToOam func,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLwrNghCellInd(func, pst, mBuf)
LwrNghCellIndToOam func;
Pst *pst;
Buffer *mBuf;
#endif 
{

   S16 ret = ROK;
   LwrNghCellInd *nghCellInd = NULLP; 

   TRC3(cmUnpkLwrNghCellInd)

   switch(pst->selector)
   {
      case  LWR_SEL_LWLC:
         CMCHKUNPKLOG(cmUnpkPtr, (PTR*) &nghCellInd, mBuf, (ErrVal)ELWR025, pst);
         break;
      default:
         ret = RFAILED;
   }
   SPutMsg(mBuf);
   if(ret == ROK)
   {
      ret = (*func)(pst, nghCellInd);
   }
   RETVALUE(ret);
}


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
