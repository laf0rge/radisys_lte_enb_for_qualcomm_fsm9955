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
               

     File:     wr_smm_tucl.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_MODULE_ID=2;
static int RLOG_FILE_ID=18;

#include "wr.h"
#include "wr_cmn.h"
#include "lhi.h"
#include "lhi.x"
#include "wr_lwr.h"
#include "wr_smm_init.h"
PRIVATE U8 wrHiCfg = 0;
PRIVATE U8 wrHiTSapCnt = 0;

#ifdef WR_RSYS_OAM  
EXTERN S16 smStartTmr ARGS((PTR cb, S16 tmrEvnt,U32 delay));
EXTERN S16 smStopTmr ARGS((PTR cb, S16 tmrEvnt));
EXTERN Void smStaticCfgFailAlarm ARGS((Void));
#endif

/*
*
*       Fun:   Initialize TUCL task.
*
*       Desc:  Invoked to create TUCL Tapa task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_hi_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smHiInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smHiInit(sysTskId)
SSTskId    sysTskId;
#endif
{

   TRC2(smHiInit)

   /* Register TUCL TAPA Task */
   if (SRegTTsk((Ent)ENTHI, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 hiActvInit, (ActvTsk)hiActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach TUCL TAPA Task */
   if (SAttachTTsk((Ent)ENTHI, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }
   RLOG1(L_DEBUG,"TUCL Tapa Task successfully registered and attached to [%d]",
      sysTskId);

   RETVALUE(ROK);
} /* end of smHiInit */

/*
 *
 *       Fun:    wrSmHiCfg - configure HI
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_sm_hi_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void wrSmHiCfg
(  
Void
)
#else
PUBLIC Void wrSmHiCfg()
#endif /* ANSI */ 
{  
   TRC2(wrSmHiCfg)

    wrHiCfg = 0;
    wrHiTSapCnt = 0;

#ifdef WR_RSYS_OAM  
   smStartTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR, SM_TMR_STATIC_CFG_TMR_VAL);
#endif
   smBuildHiGenCfg();
   smBuildHiTSapCfg(SB_HI_SPID);
   smBuildHiTSapCfg(EG_HI_SPID);

   RETVOID;
} /* end of wrSmHiCfg */

/*
 *
 *       Fun:    smBuildHiGenCfg - fill in default genCfg for HI
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  wr_sm_hi_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildHiGenCfg
(
Void
)
#else
PUBLIC Void smBuildHiGenCfg()
#endif /* ANSI */
{

   HiMngmt     hiMgt;
   HiGenCfg    *cfg = NULLP;
   Pst         pst;

   TRC2(smBuildHiGenCfg)

   SM_SET_ZERO(&hiMgt, sizeof(HiMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &hiMgt.t.cfg.s.hiGen;

   cfg->numSaps = WRSM_HI_NUMBSAPS;
   cfg->numCons = WRSM_HI_NMBCONS;
   cfg->numFdsPerSet = WRSM_HI_FDS;
   cfg->numFdBins = WRSM_HI_FDBINS;

   cfg->selTimeout = WRSM_HI_SELTIMEOUT;
   cfg->numRawMsgsToRead = 80;

#if (defined(HI_LKSCTP) && defined(HIAC_LKSCTP))
   cfg->numAssocBins   =  5;
   cfg->maxInitReTx    =  5; 
   cfg->maxAssocReTx   =  8;
   cfg->maxPathReTx    =  4; 
#endif

   cfg->permTsk = WRSM_HI_PERM_TSK_FLAG;
   cfg->schdTmrVal = WRSM_HI_SCHDTMR_VAL;
   cfg->poolStrtThr = WRSM_HI_STRT_THRESH;
   cfg->poolDropThr = WRSM_HI_DRP_THRESH;
   cfg->poolStopThr = WRSM_HI_STP_THRESH;
   cfg->timeRes = WRSM_HI_TIME_RES;
   
   /* Madhur Adding pool configuration */
   cfg->initPool = 1;

#ifdef HI_TLS
   cfg->initOpenSSL = TRUE;
   cfg->numContexts = 2;
#ifdef HI_ACC_TLS_PRNG_FILE
   cfg->prng.option = LHI_OSSL_PRNG_FILE;
   cmMemcpy((U8 *)cfg.t.cfg.s.hiGen.prng.u.file, (U8 *)"/dev/urandom", 13);
#else
   cfg->prng.option =  LHI_OSSL_PRNG_SEED;
   for (i = 0;  i < LHI_OSSL_PRNG_SEED_LEN;  i++)
   {
      SRandom(&rnd);
      cfg->prng.u.seed[i] = (U8)rnd;
   }
#endif /* HI_ACC_TLS_PRNG_FILE */
#endif

   cfg->lmPst.srcProcId = SM_HI_PROC;
   cfg->lmPst.dstProcId = SM_SM_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTHI;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)WRSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = DFLT_REGION;
   cfg->lmPst.pool = HI_POOL;
   cfg->lmPst.selector = (Selector)WRSM_HISMSEL;

 /* Fill Header */
   hiMgt.hdr.msgType             = TCFG;
   hiMgt.hdr.msgLen              = 0;
   hiMgt.hdr.entId.ent           = ENTHI;
   hiMgt.hdr.entId.inst          = SM_INST_ZERO;
   hiMgt.hdr.elmId.elmnt         = STGEN;
   hiMgt.hdr.elmId.elmntInst1    = 0;
   hiMgt.hdr.elmId.elmntInst2    = 0;
   hiMgt.hdr.elmId.elmntInst3    = 0;
   hiMgt.hdr.seqNmb              = 0;
   hiMgt.hdr.version             = 0;
   hiMgt.hdr.transId             = 0;

   hiMgt.hdr.response.prior      = PRIOR0;
   hiMgt.hdr.response.route      = RTESPEC;
   hiMgt.hdr.response.mem.region = DFLT_REGION;
   hiMgt.hdr.response.mem.pool   = HI_POOL;
   hiMgt.hdr.response.selector   = SM_SELECTOR_LC;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.srcProcId = SM_SM_PROC;
   pst.dstEnt    = ENTHI;
   pst.dstProcId = SM_HI_PROC;

   RLOG0(L_DEBUG,"TUCL Gen Cfg Req sent");
   (Void)SmMiLhiCfgReq(&pst, &hiMgt);
  
   RETVOID;
} /* end of smBuildHiGenCfg() */


/*
 *
 *       Fun:    smBuildHiTSapCfg - fill in default HI SAP
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  wr_sm_hi_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildHiTSapCfg
(
SpId        spId
)
#else
PUBLIC Void smBuildHiTSapCfg(spId)
SpId        spId;
#endif /* ANSI */
{
   HiMngmt     hiMgt;
   HiSapCfg    *cfg;
   Pst         pst;

   TRC2(smBuildHiTSapCfg)

   SM_SET_ZERO(&hiMgt, sizeof(HiMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &hiMgt.t.cfg.s.hiSap;

   cfg->spId = spId;
   cfg->uiSel = 0;
   cfg->flcEnb = FALSE;
   cfg->txqCongStrtLim = WRSM_HI_CONG_STRT;
   cfg->txqCongDropLim  = WRSM_HI_CONG_DRP;
   cfg->txqCongStopLim = WRSM_HI_CONG_STP;
   cfg->numBins = WRSM_HI_NMB_HLBINS;
     
   cfg->uiMemId.region =  DFLT_REGION;
   cfg->uiMemId.pool   =  HI_POOL;
   /* Fix: Reducing the priority of data plane packets to 
    * reduce control plane packet delay */
   if (spId == EG_HI_SPID)
   {
      cfg->uiPrior        = PRIOR3;
   }
   else
   {
      cfg->uiPrior        = PRIOR0;
   }
   cfg->uiRoute        = RTESPEC;

    /* profile_1 : srvcType value = 3 (GTP header format) */
   cfg->hdrInf[0].hdrLen = 20;
   cfg->hdrInf[0].offLen = 2;
   cfg->hdrInf[0].lenLen = 2;
   cfg->hdrInf[0].flag   = 0;

   /* profile_2 : srvcType value = 4 (LDP header format) */
   cfg->hdrInf[1].hdrLen = 10;
   cfg->hdrInf[1].offLen = 2;
   cfg->hdrInf[1].lenLen = 2;
   cfg->hdrInf[1].flag   = 0;

#if (defined(HI_LKSCTP) && defined(HIAC_LKSCTP))
   cfg->type = HI_SCT_SAP;        
#endif

    /* Fill Header */
   hiMgt.hdr.msgType             = TCFG;
   hiMgt.hdr.msgLen              = 0;
   hiMgt.hdr.entId.ent           = ENTHI;
   hiMgt.hdr.entId.inst          = SM_INST_ZERO;
   hiMgt.hdr.elmId.elmnt         = STTSAP;
 
      /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTHI;
   pst.srcProcId = SM_SM_PROC;
   pst.dstProcId = SM_HI_PROC;

   RLOG0(L_DEBUG,"TUCL TSap Cfg Req sent");
   (Void)SmMiLhiCfgReq(&pst, &hiMgt);

   RETVOID;
} /* end of smBuildHiTSapCfg() */
 
/**
 * @brief This function is used to send enable/disable control request to 
 * the tucl layers.
 *
 * @details
 *
 * Function: smBuildHiLogCntrl
 *
 * @param[in] actType
 * @param[in] mask
 *
 * @return Void
 */ 

#ifdef ANSI
PUBLIC Void smBuildHiLogCntrl
(
 Bool actType,
 U32  mask
)
#else
PUBLIC Void smBuildHiLogCntrl(actType, mask)
 Bool actType;
 U32  mask;
#endif /* ANSI */
{
   HiMngmt              hiMgt;
   Pst                  pst;

   TRC2(smBuildHiLogCntrl)

   SM_SET_ZERO(&hiMgt, sizeof(HiMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   hiMgt.hdr.msgType             = TCNTRL;
   hiMgt.hdr.entId.ent           = ENTHI;
   hiMgt.hdr.entId.inst          = SM_INST_ZERO;
   hiMgt.hdr.elmId.elmnt         = STGEN;

   if(actType == WR_ON)
   {
      hiMgt.t.cntrl.action          =  AENA;
   }
   else
   {
      hiMgt.t.cntrl.action          =  ADISIMM;
   }
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTHI;
   pst.dstProcId = SM_HI_PROC;
   pst.srcProcId = SM_SM_PROC;

   (Void)SmMiLhiCntrlReq(&pst, &hiMgt);

   RETVOID;
} /* end of smBuildHiLogCntrl*/
 
/*
 *
 *       Fun:    smBuildHiDbgCntrl
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   wr_sm_eg_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildHiDbgCntrl
(
Void
)
#else
PUBLIC Void smBuildHiDbgCntrl()
#endif /* ANSI */
{
   HiMngmt              hiMgt;
   Pst                  pst;

   TRC2(smBuildHiDbgCntrl)

   SM_SET_ZERO(&hiMgt, sizeof(HiMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   hiMgt.hdr.msgType             = TCNTRL;
   hiMgt.hdr.entId.ent           = ENTHI;
   hiMgt.hdr.entId.inst          = SM_INST_ZERO;
   hiMgt.hdr.elmId.elmnt         = STGEN;

   hiMgt.t.cntrl.action          =  AENA;
   hiMgt.t.cntrl.subAction       =  SADBG;
#ifdef DEBUGP
   hiMgt.t.cntrl.ctlType.hiDbg.dbgMask = 0xffffffff;
#endif
    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTHI;
   pst.dstProcId = SM_HI_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG,"TUCL TSap Cntrl Req sent");

   (Void)SmMiLhiCntrlReq(&pst, &hiMgt);

   RETVOID;
} /* end of smBuildHiDbgCntrl*/

/* IPSec Recovery */
/*
 *
 *       Fun:    smBuildHiShutDownCntrl
 *
 *       Desc:   Invoked to shut down TUCL
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   wr_sm_eg_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void smBuildHiShutDownCntrl
(
Void
)
#else
PUBLIC Void smBuildHiShutDownCntrl()
#endif /* ANSI */
{
   HiMngmt              hiMgt;
   Pst                  pst;

   TRC2(smBuildHiShutDownCntrl)

   SM_SET_ZERO(&hiMgt, sizeof(HiMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   hiMgt.hdr.msgType             = TCNTRL;
   hiMgt.hdr.entId.ent           = ENTHI;
   hiMgt.hdr.entId.inst          = SM_INST_ZERO;
   hiMgt.hdr.elmId.elmnt         = STGEN;

   hiMgt.t.cntrl.action          =  ASHUTDOWN;
   hiMgt.t.cntrl.subAction       =  NULLD;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTHI;
   pst.dstProcId = SM_HI_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG,"TUCL shut down Cntrl Req sent");

   (Void)SmMiLhiCntrlReq(&pst, &hiMgt);

   RETVOID;
} /* end of smBuildHiDbgCntrl*/



/* IPSec Recovery */

/*
*
*       Fun:   Activate Task - initialize
*
*       Desc:  Invoked by system services to initialize a task.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smhiexms.c
*
*/

#ifdef ANSI
PUBLIC S16 smHiActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smHiActvInit(ent, inst, region, reason)
Ent ent;                      /* entity */
Inst inst;                    /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
{
   TRC3(smHiActvInit)

   UNUSED(ent);
   UNUSED(inst);
   UNUSED(region);
   UNUSED(reason);
   RETVALUE(ROK);
}

  
/* local defines */

/* local typedefs */
  
/* local externs */
  
/* forward references */
  

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from TUCL
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:     wr_sm_hi_exms.c
*
*/
  
#ifdef ANSI
PUBLIC S16 smHiActvTsk
(
Pst    *spst,               /* post structure */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smHiActvTsk(spst, mBuf)
Pst    *spst;               /* post structure */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16           ret = ROK; /* return value from the function */

   /* wr002.102: Added trace macro */
   TRC2(smHiActvTsk)

   switch(spst->event)
   {
#ifdef LCSMHIMILHI
      case EVTLHICFGCFM:             /* Configuration Confirm */
         if ((ret = cmUnpkLhiCfgCfm(SmMiLhiCfgCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;
      
      case EVTLHISTSCFM:             /* Statistics Confirm */
         if ((ret = cmUnpkLhiStsCfm(SmMiLhiStsCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHICNTRLCFM:           /* Conntrol Confirm */
         if ((ret = cmUnpkLhiCntrlCfm(SmMiLhiCntrlCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHISTACFM:             /* Status Confirm */
         if ((ret = cmUnpkLhiStaCfm(SmMiLhiStaCfm, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

     case EVTLHISTAIND:             /* Status Indication */
         if ((ret = cmUnpkLhiStaInd(SmMiLhiStaInd, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;

      case EVTLHITRCIND:             /* Trace Indication */
         if ((ret = cmUnpkLhiTrcInd(SmMiLhiTrcInd, spst, mBuf)) != ROK)
         {
            RETVALUE(ret);
         }
         break;
#endif
      default:
        (Void)SPutMsg(mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
        SLogError(spst->dstEnt, spst->dstInst, spst->dstProcId, __FILE__, \
                   __LINE__, (ErrCls) ERRCLS_DEBUG, (ErrVal)WR_EWR001, 
                   (ErrVal)spst->event, "smHiActvTsk () Failed"); 
#endif
         break;
   }

   SExitTsk();

   RETVALUE(ret);
}/* end of smHiActvTsk() */

#define  MAXHIMI  2

#ifndef  LCSMHIMILHI
#define  PTSMHIMILHI
#else
#ifndef   HI
#define  PTSMHIMILHI
#endif
#endif


#ifdef PTSMHIMILHI
PRIVATE S16 PtMiLhiCfgReq    (Pst *pst, HiMngmt *cfg);
PRIVATE S16 PtMiLhiStsReq    (Pst *pst, Action action, HiMngmt *sts);
PRIVATE S16 PtMiLhiCntrlReq  (Pst *pst, HiMngmt *cntrl);
PRIVATE S16 PtMiLhiStaReq    (Pst *pst, HiMngmt *sta);
#endif


/*
the following matrices define the mapping between the primitives
called by the layer management interface of TCP UDP Convergence Layer
and the corresponding primitives in TUCL
 
The parameter MAXHIMI defines the maximum number of layer manager
entities on top of TUCL . There is an array of functions per primitive
invoked by TCP UDP Conbvergence Layer. Every array is MAXHIMI long
(i.e. there are as many functions as the number of service users).

The dispatching is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMHIMILHI) 2 - Lhi (#define HI)

*/


/* Configuration request primitive */
 
PRIVATE LhiCfgReq SmMiLhiCfgReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiCfgReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiCfgReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Control request primitive */
 
PRIVATE LhiCntrlReq SmMiLhiCntrlReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiCntrlReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Statistics request primitive */
 
PRIVATE LhiStsReq SmMiLhiStsReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiStsReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiStsReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiStsReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiStsReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Status request primitive */
 
PRIVATE LhiStaReq SmMiLhiStaReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiStaReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiStaReq,          /* 1 - tightly coupled, portable */
#endif
};


/*
*     layer management interface functions 
*/
 
/*
*
*       Fun:   Configuration request
*
*       Desc:  This function is used to configure  TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     wr_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiCfgReq
(
Pst *spst,                /* post structure */
HiMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLhiCfgReq(spst, cfg)
Pst *spst;                /* post structure */   
HiMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLhiCfgReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiCfgReqMt[spst->selector])(spst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLhiCfgReq */



/*
*
*       Fun:   Statistics request
*
*       Desc:  This function is used to request statistics from 
*              TUCL
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     wr_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiStsReq
(
Pst *spst,                /* post structure */
Action action,
HiMngmt *sts              /* statistics */
)
#else
PUBLIC S16 SmMiLhiStsReq(spst, action, sts)
Pst *spst;                /* post structure */   
Action action;
HiMngmt *sts;             /* statistics */
#endif
{
   TRC3(SmMiLhiStsReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiStsReqMt[spst->selector])(spst, action, sts); 
   RETVALUE(ROK);
} /* end of SmMiLhiStsReq */


/*
*
*       Fun:   Control request
*
*       Desc:  This function is used to send control request to 
*              TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     wr_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiCntrlReq
(
Pst *spst,                 /* post structure */
HiMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLhiCntrlReq(spst, cntrl)
Pst *spst;                 /* post structure */   
HiMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLhiCntrlReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiCntrlReqMt[spst->selector])(spst, cntrl); 
   RETVALUE(ROK);
} /* end of SmMiLhiCntrlReq */


/*
*
*       Fun:   Status request
*
*       Desc:  This function is used to send a status request to 
*              TUCL
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     wr_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiStaReq
(
Pst *spst,                /* post structure */
HiMngmt *sta              /* status */
)
#else
PUBLIC S16 SmMiLhiStaReq(spst, sta)
Pst *spst;                /* post structure */   
HiMngmt *sta;             /* status */
#endif
{
   TRC3(SmMiLhiStaReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiStaReqMt[spst->selector])(spst, sta); 
   RETVALUE(ROK);
} /* end of SmMiLhiStaReq */

#ifdef PTSMHIMILHI

/*
 *             Portable Functions
 */

/*
*
*       Fun:   Portable configure Request- TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     wr_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLhiCfgReq
(
Pst *spst,                  /* post structure */
HiMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLhiCfgReq(spst, cfg)
Pst *spst;                  /* post structure */
HiMngmt *cfg;               /* configure */
#endif
{
  TRC3(PtMiLhiCfgReq) 

  UNUSED(spst);
  UNUSED(cfg);

  RETVALUE(ROK);
} /* end of PtMiLhiCfgReq */



/*
*
*       Fun:   Portable statistics Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     wr_sm_hi_ptmi.c
*
*/
#ifdef ANSI
PRIVATE S16 PtMiLhiStsReq
(
Pst *spst,                  /* post structure */
Action action,
HiMngmt *sts                /* statistics */
)
#else
PRIVATE S16 PtMiLhiStsReq(spst, action, sts)
Pst *spst;                  /* post structure */
Action action;
HiMngmt *sts;               /* statistics */
#endif
{
  TRC3(PtMiLhiStsReq) 

  UNUSED(spst);
  UNUSED(action);
  UNUSED(sts);

  RETVALUE(ROK);
} /* end of PtMiLhiStsReq */


/*
*
*       Fun:   Portable control Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     wr_sm_hi_ptmi.c
*
*/
#ifdef ANSI
PRIVATE S16 PtMiLhiCntrlReq
(
Pst *spst,                  /* post structure */
HiMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLhiCntrlReq(spst, cntrl)
Pst *spst;                  /* post structure */
HiMngmt *cntrl;             /* control */
#endif
{
  TRC3(PtMiLhiCntrlReq)

  UNUSED(spst);
  UNUSED(cntrl);

  RETVALUE(ROK);
} /* end of PtMiLhiCntrlReq */


/*
*
*       Fun:   Portable status Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     wr_sm_hi_ptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLhiStaReq
(
Pst *spst,                  /* post structure */
HiMngmt *sta                /* status */
)
#else
PRIVATE S16 PtMiLhiStaReq(spst, sta)
Pst *spst;                  /* post structure */
HiMngmt *sta;               /* status */
#endif
{
  TRC3(PtMiLhiStaReq);

  UNUSED(spst);
  UNUSED(sta);

  RETVALUE(ROK);
} /* end of PtMiLhiStaReq */

#endif /* PTSMHIMILHI */

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiCfgCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLhiCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
#endif
{
   Pst  hiPst;

   TRC2(SmMiLhiCfgCfm)
  
  /*-- Update Event --*/
   UNUSED(pst);

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch(cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            wrHiCfg |= WR_HI_GEN_CFG;
            break;
         case STTSAP:
            wrHiCfg |= WR_HI_TSAP_CFG;
            wrHiTSapCnt++;
            break;
         default:
            RLOG1(L_ERROR,"TUCL Cfg Cfm received with invalid element [%d]",
               cfm->hdr.elmId.elmnt);
            break;
      } /* end of switch statement */
      RLOG1(L_DEBUG,"TUCL Cfg Cfm received for the element [%d]",
         cfm->hdr.elmId.elmnt);

      /* Wait for two Tsap confirms one for SCTP provider Sap
         and the other egtp provider Sap */
      if (wrHiCfg == WR_SM_HI_CONFIGURED && wrHiTSapCnt == 2)
      {
         /* Initiate SCTP configuration after TUCL configuration. 
            So send EVTWRSCTPCFG */
         smCb.smState = WR_SM_STATE_TUCL_CFG_DONE;
         RLOG0(L_ALWAYS,"TUCL CONFIGURATION SUCCESSFULLY PERFORMED.");
#ifdef WR_RSYS_OAM  
         smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
#endif
         WR_FILL_PST(hiPst, SM_HI_PROC, ENTSM, ENTWR, EVTWRSCTPCFG, WR_POOL, SM_SELECTOR_LC);
         if(wrSmSelfPstMsg(&hiPst) != ROK)
         {
            RLOG0(L_FATAL, "Self Pst failed after TUCL Configuration.");
         }
         else
         {
            RLOG0(L_DEBUG, "Self Pst success after TUCL Configuration.");
         } /* end of else part */

      } /* end of if statement  wrHiCfg == WR_SM_HI_CONFIGURED */

   }
#ifdef WR_RSYS_OAM  
   else
   {
      smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
      smCb.cfgCb.alarm.severity  = SM_ALARM_SEVERITY_MAJOR;
      smCb.cfgCb.alarm.causeType = SM_ALARM_CAUSE_TYPE_ERR_IND;
      smCb.cfgCb.alarm.causeVal  = SM_ALARM_CAUSE_VAL_INIT_FAIL;
      smStaticCfgFailAlarm();
   }
#endif
   /* end of if statement  cfm->cfm.status == LCM_PRIM_OK */

   RETVALUE(ROK);
} /* end of SmMiLhiCfgCfm */


/*
 *
 *       Fun:   Control Confirm
 *
 *       Desc:  This function is used by to present control confirm
 *              information to Layer Manager.
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smhiptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLhiCntrlCfm
(
 Pst     *pst,          /* post structure */
 HiMngmt *cfm           /* configuration */
 )
#else
PUBLIC S16 SmMiLhiCntrlCfm(pst, cfm)
   Pst     *pst;          /* post structure */
   HiMngmt *cfm;          /* configuration */
#endif
{
   Pst  hiPst;
   TRC2(SmMiLhiCntrlCfm);
   /* IPSec */
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      if (smCb.smState == WR_SM_STATE_SCTP_SHUTDWN_DONE)
      {
         smCb.smState = WR_SM_STATE_TUCL_SHUTDWN_DONE;
         RLOG0(L_ALWAYS, "TUCL SHUTDOWN SUCCESSFULLY PERFORMED.");
         WR_FILL_PST(hiPst, SM_SZ_PROC, ENTSM, ENTWR, EVTWRTUCLSHUT, WR_POOL, SM_SELECTOR_LC);

         if(wrSmSelfPstMsg(&hiPst) != ROK)
         {
            RLOG0(L_FATAL, "Self Pst failed after TUCL SHUTDOWN Configuration.");
         }
         else
         {
            RLOG0(L_DEBUG, "Self Pst success after TUCL SHUTDOWN Configuration.");
         } /* end of else part */
         RETVALUE(ROK);
      }
   }

   if(cfm->cfm.status == LCM_PRIM_OK)
   {
      if((smCfgCb.hiDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_HI_DBG_MASK))
      {
         /* Update DbgMsk */
         smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | WR_SM_HI_DBG_MASK);
         /* Trigger Debug Cntrl */
         wrSmHdlEnbDebugs(EVTWRENBDEBUGS);
      }
   }
   else
   {
      RLOG0(L_ERROR,"SmMiLhiCntrlCfm failed!");

   }

   RETVALUE(ROK);
} /* end of SmMiLhiCntrlCfm */

/*
*
*       Fun:   Statistics Confirm
*
*       Desc:  This function is used by to present statistics confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiStsCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLhiStsCfm(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
#endif
{
    TRC2(SmMiLhiStsCfm)

    RETVALUE(ROK);
} /* end of SmMiLhiStsCfm */

/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used by to present status confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiStaCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLhiStaCfm(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
#endif
{
    TRC2(SmMiLhiStaCfm)

    RETVALUE(ROK);
} /* end of SmMiLhiStaCfm */

/*
*
*       Fun:   Status Indication
*
*       Desc:  This function is used by to present status indication
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiStaInd
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLhiStaInd(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
#endif
{
    TRC2(SmMiLhiStaInd)

    RETVALUE(ROK);
} /* end of SmMiLhiStaInd*/

/*
*
*       Fun:   Trace Indication
*
*       Desc:  This function is used by to present trace indication
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiTrcInd
(
Pst     *pst,          /* post structure */
HiMngmt *cfm,           /* configuration */
Buffer  *mBuf
)
#else
PUBLIC S16 SmMiLhiTrcInd(pst, cfm, mBuf)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
Buffer  *mBuf;
#endif
{
    TRC2(SmMiLhiStaInd)

    RETVALUE(ROK);
} /* end of SmMiLhiTrcInd*/



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
