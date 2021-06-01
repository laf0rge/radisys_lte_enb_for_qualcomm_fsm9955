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
               

     File:     wr_smm_rlc.c

     Sid:      fc_smm_rlc_brcm.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:52:24 2014

     Prg:      Sriky 

*********************************************************************21*/
/* header include files */
static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_MODULE_ID=2;
static int RLOG_FILE_ID=42;

#include "wr.h"
#include "wr_cmn.h"
#include "lkw.h"
#include "lkw.x"
#include "lpj.h"
#include "lpj.x"
#ifdef TENB_STATS
#include "cm_tenb_stats.x"
#endif

#include "wr_lwr.h"
#include "wr_smm_init.h"
#include "wr_kpiIds.h"
#include "wr_kpi.h"
PRIVATE U8 wrKwDlCfg = 0;
PRIVATE  U8 wrNumKwuDlSaps; 
PRIVATE U8 wrKwUlCfg = 0;
PRIVATE U8 wrNumKwuUlSaps = 0;

/* wr004.102: g++ compilation fix */
#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */

EXTERN S16 cmUnpkLpjCfgCfm ARGS((
LpjCfgCfm func,
Pst *pst,
Buffer *mBuf
));

EXTERN S16 cmUnpkLpjCntrlCfm ARGS((
LpjCntrlCfm func,
Pst *pst,
Buffer *mBuf
));
#ifdef LTE_L2_MEAS
EXTERN S16 cmUnpkLpjL2MeasCfm ARGS((
LpjL2MeasCfm func,
Pst *pst,
Buffer *mBuf
));
EXTERN S16 cmUnpkLpjL2MeasStopCfm ARGS((
LpjL2MeasStopCfm func,
Pst *pst,
Buffer *mBuf
));
#endif /*LTE_L2_MEAS*/

#ifdef WR_RSYS_OAM  
EXTERN S16 smStartTmr ARGS((PTR cb, S16 tmrEvnt,U32 delay));
EXTERN S16 smStopTmr ARGS((PTR cb, S16 tmrEvnt));
EXTERN Void smStaticCfgFailAlarm ARGS((Void));
#endif
#define MAXNHMI  2         /* The size of the LC/TC matrix */

/* local defines */
#ifndef LCKWMILKW
#define PTSMLKW
#endif

#ifndef KW
#define PTSMLKW
#endif

#ifndef SM 
#define PTSMLKW
#endif

/* local typedefs */

/* local externs */

#ifdef PTSMLKW
/* forward references */
PRIVATE S16 PtMiLkwCfgReq   (Pst *pst, KwMngmt *cfg );
PRIVATE S16 PtMiLkwCntrlReq   (Pst *pst, KwMngmt *cfg );
#endif

PRIVATE LkwCfgReq smMiLkwCfgReqMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwCfgReq,          /* 0 - loosely coupled - fc */
#else
   PtMiLkwCfgReq,          /* 0 - loosely coupled portable */
#endif /* LCKWMILKW */
#ifdef KW
   KwMiLkwCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLkwCfgReq,          /* 1 - tightly coupled portable */
#endif /* KW */
};

/* control request primitive */
PRIVATE LkwCntrlReq smMiLkwCntrlReqMt[] =
{
#ifdef LCKWMILKW 
   cmPkLkwCntrlReq,          /* 0 - loosely coupled - fc */
#else
   PtMiLkwCntrlReq,          /* 0 - tightly coupled, portable */
#endif /* LCKWMILKW */
#ifdef KW 
   KwMiLkwCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLkwCntrlReq,          /* 1 - tightly coupled, portable */
#endif /* RL */
};

#ifdef __cplusplus
}
#endif /* cplusplus */

PRIVATE S16 smProcKwDlCfgCfm (Pst *pst, KwMngmt *cfm );
PRIVATE S16 smProcKwUlCfgCfm ( Pst     *pst, KwMngmt *cfm );

/*
*
*       Fun:   Initialize RLC task.
*
*       Desc:  Invoked to create RLC/PDCP TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_enb_init.c
*
*/
#ifdef ANSI
PUBLIC S16 smKwDlInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smKwDlInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smKwDlInit)


   /* Attach RLC TAPA Task */
   if (SAttachTTsk((Ent)ENTKW, (Inst)WR_SM_KWDL_INST, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

#ifdef DEBUGP
   WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, "RLC DL Tapa Task successfully\
            registered and attached to %d\n",sysTskId));
#endif

   RETVALUE(ROK);
} /* end of smPjUlInit */


/*
*
*       Fun:   Initialize RLC task.
*
*       Desc:  Invoked to create RLC/PDCP TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_enb_init.c
*
*/
#ifdef ANSI
PUBLIC S16 smKwUlInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smKwUlInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smKwUlInit)

   /* Register RLC TAPA Task */

   /* Attach RLC TAPA Task */
   if (SAttachTTsk((Ent)ENTKW, (Inst)WR_SM_KWUL_INST, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

#ifdef DEBUGP
   WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, "RLC UL Tapa Task successfully\
            registered and attached to %d\n",sysTskId));
#endif

   RETVALUE(ROK);
} /* end of smPjUlInit */


/*
 *
 *       Fun:    wrSmKwCfg - configure KW
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_sm_kw_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void wrSmKwDlCfg
(
Void
)
#else
PUBLIC Void wrSmKwDlCfg(Void)
#endif /* ANSI */
{
   TRC2(wrSmKwCfg)

   wrKwDlCfg = 0;
   wrNumKwuDlSaps = 0; 
   wrKwUlCfg = 0;
   wrNumKwuUlSaps = 0;

#ifdef WR_RSYS_OAM  
   smStartTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR, SM_TMR_STATIC_CFG_TMR_VAL);
#endif
   smBuildKwGenCfg((Inst) WR_SM_KWDL_INST);
   smBuildKwUsapCfg(STKWUSAP, ENTNH, (Inst)WR_SM_KWDL_INST);
   smBuildKwUsapCfg(STKWUSAP, ENTPJ, (Inst)WR_SM_KWDL_INST);
   smBuildKwUsapCfg(STUDXSAP, ENTKW, (Inst)WR_SM_KWDL_INST);
   smBuildKwLsapCfg(ENTRG,(Inst)WR_SM_KWDL_INST);

   RETVOID;
} /* end of wrSmKwCfg */

/*
 *
 *       Fun:    wrSmKwCfg - configure KW
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_sm_kw_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void wrSmKwUlCfg
(
Void
)
#else
PUBLIC Void wrSmKwUlCfg(Void)
#endif /* ANSI */
{
   TRC2(wrSmKwCfg)

#ifdef WR_RSYS_OAM  
   smStartTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR, SM_TMR_STATIC_CFG_TMR_VAL);
#endif
   smBuildKwGenCfg((Inst) WR_SM_KWUL_INST);
   RLOG0(L_DEBUG, "Building CKW Cfg Req");
   smBuildKwUsapCfg(STCKWSAP, ENTNH,(Inst) WR_SM_KWUL_INST);
   smBuildKwUsapCfg(STKWUSAP, ENTNH,(Inst) WR_SM_KWUL_INST);
   smBuildKwUsapCfg(STKWUSAP, ENTPJ,(Inst) WR_SM_KWUL_INST);
   smBuildKwLsapCfg(ENTRG,(Inst) WR_SM_KWUL_INST);
   smBuildKwLsapCfg(ENTKW,(Inst) WR_SM_KWUL_INST);

   RETVOID;
} /* end of wrSmKwCfg */

/*
*
*       Fun:   smBuildKwGenCfg
*
*       Desc:  Invoked to build RLC GEN CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildKwGenCfg
(
Inst inst
)
#else
PUBLIC S16 smBuildKwGenCfg(inst)
Inst inst;
#endif
{
   KwMngmt   kwMngmt;
   KwGenCfg  *genCfg = NULLP;
   Pst       pst;

   TRC2(smBuildKwGenCfg)

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(kwMngmt.t.cfg.s.gen);

   /*----------- Fill General Configuration Parameters ---------*/
   genCfg->maxUe       = smCfgCb.maxUeSupp;
   genCfg->maxKwuSaps  = 2;
   genCfg->maxUdxSaps  = 1; /* Changed the resolution from 10 to 1 */
   genCfg->rlcMode     = (inst == WR_SM_KWUL_INST) ? 
                         LKW_RLC_MODE_UL : LKW_RLC_MODE_DL;
   /*wr004.102: Platform changes */
   genCfg->timeRes     = 1; /* Changed the resolution from 10 to 1 */
   
   /*----------- Fill lmPst Parameters ---------*/
   genCfg->lmPst.dstProcId = SM_SM_PROC;
   genCfg->lmPst.srcProcId = SM_KW_PROC;
   genCfg->lmPst.dstEnt    = ENTSM;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTKW;
   genCfg->lmPst.srcInst   = inst;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = (inst == WR_SM_KWUL_INST) ?
                              KW_UL_MEM_REGION:KW_DL_MEM_REGION;
   genCfg->lmPst.pool      = KW_POOL;
   genCfg->lmPst.selector  = SM_SELECTOR_LC;
   
   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.msgLen              = 0;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STGEN;
   kwMngmt.hdr.seqNmb              = 0;
   kwMngmt.hdr.version             = 0;
   kwMngmt.hdr.transId             = 0;
   kwMngmt.hdr.response.prior      = PRIOR0;
   kwMngmt.hdr.response.route      = RTESPEC;
   kwMngmt.hdr.response.mem.region = (inst == WR_SM_KWUL_INST) ?
                                      KW_UL_MEM_REGION:KW_DL_MEM_REGION;
   kwMngmt.hdr.response.mem.pool   = KW_POOL;
   kwMngmt.hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstInst   = inst;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG,"RLC Gen Cfg Req sent");

   /* Send the request to the RLC */
   SmMiLkwCfgReq(&pst, &kwMngmt);

   RETVALUE(ROK);
} /* smBuildKwGenCfg */

/*
*
*       Fun:   smBuildCkwUsapCfg
*
*       Desc:  Invoked to build USAP CFG.(CKW)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCkwUsapCfg
(
Inst inst
)
#else
PUBLIC S16 smBuildCkwUsapCfg(inst)
Inst inst;
#endif
{
   KwMngmt    kwMngmt;
   KwSapCfg   *uSap = NULLP;
   Pst       pst;

   TRC2(smBuildCkwUsapCfg)

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(kwMngmt.t.cfg.s.sap);
  
   uSap->selector =  SM_SELECTOR_LC;
   uSap->mem.region = (inst == WR_SM_PJUL_INST) ?
                       KW_UL_MEM_REGION:KW_DL_MEM_REGION;
   uSap->mem.pool = KW_POOL;
   uSap->mem.spare = 0;
   uSap->procId = SM_NH_PROC;
   uSap->ent = ENTNH;
   uSap->inst = SM_INST_ZERO;
   uSap->sapId = 0;
   uSap->bndTmrIntvl = 1000;
   uSap->priority = PRIOR0;
   uSap->route = RTESPEC;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STCKWSAP;
   kwMngmt.hdr.response.mem.region = (inst == WR_SM_PJUL_INST) ? 
                                      KW_UL_MEM_REGION:KW_DL_MEM_REGION;

   kwMngmt.hdr.response.mem.pool   = PJ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG,"RLC Ckw Upper Sap Cfg Req sent");

   /* Send the request to the RLC */
   SmMiLkwCfgReq(&pst, &kwMngmt);
   
   RETVALUE(ROK);
} /* smBuildCkwUsapCfg */

/*
*
*       Fun:   smBuildUdxUsapCfg
*
*       Desc:  Invoked to build USAP CFG.(KWU -- NH/PJ)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildUdxUsapCfg
(
Inst inst 
)
#else
PUBLIC S16 smBuildUdxUsapCfg (ent,inst)
Inst inst;
#endif
{
   KwMngmt    kwMngmt;
   KwSapCfg   *uSap = NULLP;
   Pst        pst;

   TRC2(smBuildUdxUsapCfg)

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(kwMngmt.t.cfg.s.sap);

   uSap->selector   = 1;
//   uSap->selector   = SM_SELECTOR_LC;
   uSap->mem.region = (inst == WR_SM_PJUL_INST) ?
                      KW_UL_MEM_REGION:KW_DL_MEM_REGION;
   uSap->mem.pool = KW_POOL;
   uSap->mem.spare = 0;
   
   uSap->procId = SM_KW_PROC;
   uSap->ent = ENTKW;
   uSap->sapId = 0;
   
   uSap->inst = (inst == WR_SM_KWUL_INST) ? 
                 WR_SM_KWDL_INST : WR_SM_KWUL_INST;
   uSap->bndTmrIntvl = 1000;
   uSap->priority = PRIOR0;
   uSap->route = RTESPEC;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STUDXSAP;
   kwMngmt.hdr.response.mem.region = (inst == WR_SM_PJUL_INST) ? 
                                      KW_UL_MEM_REGION:KW_DL_MEM_REGION;

   kwMngmt.hdr.response.mem.pool   = PJ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.dstInst = inst;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RLC Kwu Upper Sap Cfg Req sent");

   /* Send the request to the RLC */
   SmMiLkwCfgReq(&pst, &kwMngmt);
   
   RETVALUE(ROK);
} /* end of smBuildUdxUsapCfg*/


/*
*
*       Fun:   smBuildKwuUsapCfg
*
*       Desc:  Invoked to build USAP CFG.(KWU -- NH/PJ)
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildKwuUsapCfg
(
U8  ent,
Inst inst 
)
#else
PUBLIC S16 smBuildKwuUsapCfg (ent,inst)
U8  ent;
Inst inst;
#endif
{
   KwMngmt    kwMngmt;
   KwSapCfg   *uSap = NULLP;
   Pst        pst;

   TRC2(smBuildKwuUsapCfg)

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(kwMngmt.t.cfg.s.sap);

      /*Interface with RRC is LC */
   /* Selector value for RLC Upper Sap 
      0 -> Loosely Coupled
      1 -> Tightly Coupled with RRC
      2 -> Tightly Coupled with PDCP
      3 -> Lightweight Loosely Coupled */
   uSap->selector   = SM_SELECTOR_LC;
   uSap->mem.region = (inst == WR_SM_PJUL_INST) ?
                      KW_UL_MEM_REGION:KW_DL_MEM_REGION;
   uSap->mem.pool = KW_POOL;
   uSap->mem.spare = 0;
   
   if (ent == ENTNH)
   {
      uSap->procId = SM_NH_PROC;
      uSap->ent = ENTNH;
      uSap->sapId = 0;
   }
   else
   {
      /*Interface with PDPC is either TC or LWLC*/
      /* 2 is for Tight coupling (PDCP<-->RLC  UL) *
       * 3 is for LWLC (PDCP <-> RLC DL) */
      uSap->selector  = (inst == WR_SM_PJUL_INST) ? 2 : 3;
      uSap->procId = SM_PJ_PROC;
      uSap->ent = ENTPJ;
      uSap->sapId = 1;
   }
   
   uSap->inst = SM_INST_ZERO;
   uSap->bndTmrIntvl = 1000;
   uSap->priority = PRIOR0;
   uSap->route = RTESPEC;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STKWUSAP;
   kwMngmt.hdr.response.mem.region = (inst == WR_SM_PJUL_INST) ? 
                                      KW_UL_MEM_REGION:KW_DL_MEM_REGION;

   kwMngmt.hdr.response.mem.pool   = PJ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.dstInst = inst;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG,"RLC Kwu Upper Sap Cfg Req sent");

   /* Send the request to the RLC */
   SmMiLkwCfgReq(&pst, &kwMngmt);
   
   RETVALUE(ROK);
} /* end of smBuildKwuUsapCfg */

/*
*
*       Fun:   smBuildKwUsapCfg
*
*       Desc:  Invoked to build USAP CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildKwUsapCfg
(
U8  elemId,
U8  ent,
Inst inst
)
#else
PUBLIC S16 smBuildKwUsapCfg (elemId, ent,inst)
U8  elemId;
U8  ent;
Inst inst;
#endif
{
   S16 ret = ROK;

   TRC2(smBuildKwUsapCfg)

   switch (elemId)
   {
      case STCKWSAP:
      {
         smBuildCkwUsapCfg(inst);
         break;
      }
      case STKWUSAP:
      {
         /* entity can be NH or PJ */
         smBuildKwuUsapCfg(ent, inst);
         break;
      }

      case STUDXSAP:
      {
         smBuildUdxUsapCfg(inst);
         break;
      }
      default:
      {
         RLOG0(L_ERROR, "Invalid elementId ");
         ret = RFAILED;
         break;
      }
   } /* end of switch statement */

   RETVALUE(ret);
} /* end of smBuildKwUsapCfg */

/*
*
*       Fun:   smBuildKwRguLsapCfg
*
*       Desc:  Invoked to build LSAP RGU CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildKwLsapCfg
(
U8   ent,
Inst inst
)
#else
PUBLIC S16 smBuildKwLsapCfg(ent,inst)
U8   ent;
Inst inst;
#endif
{
   
   KwMngmt    kwMngmt;
   KwSapCfg   *lSap = NULLP;
   Pst        pst;

   TRC2(smBuildKwRguLsapCfg)

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(kwMngmt.t.cfg.s.sap);
  
   lSap->mem.region = (inst == WR_SM_PJUL_INST) ?
                      KW_UL_MEM_REGION:KW_DL_MEM_REGION;
   lSap->mem.pool    = KW_POOL;
   lSap->mem.spare   = 0;
   if (ent == ENTRG)
   {
   lSap->procId      = SM_RG_PROC;
   lSap->ent         = ENTRG;
   lSap->inst        = SM_INST_ZERO;
      lSap->selector    = (inst == WR_SM_KWUL_INST) ? 2 : SM_SELECTOR_TC;
      kwMngmt.hdr.elmId.elmnt  = STRGUSAP; 
   }
   else
   {
      lSap->procId    = SM_KW_PROC;
      lSap->ent       = ENTKW;
      lSap->inst      = (inst == WR_SM_KWUL_INST) ? 
                        WR_SM_KWDL_INST : WR_SM_KWUL_INST;
#if 0
      lSap->selector = SM_SELECTOR_LC;
#else
      lSap->selector = 1;
#endif
      kwMngmt.hdr.elmId.elmnt  = STUDXSAP; 
   }
   lSap->sapId       = 0;
   lSap->bndTmrIntvl = 10;
   lSap->priority    = PRIOR0;
   lSap->route       = RTESPEC;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCFG;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.response.mem.region = (inst == WR_SM_PJUL_INST) ? 
                                      KW_UL_MEM_REGION:KW_DL_MEM_REGION;

   kwMngmt.hdr.response.mem.pool   = PJ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.dstInst   = inst;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG,"RLC Rgu Lower Sap Cfg Req sent");
   /* Send the request to the RRC */
   SmMiLkwCfgReq(&pst, &kwMngmt);
   
   RETVALUE(ROK);
} /* end of smBuildKwLsapCfg */

/*
*
*       Fun:   smBindKwUdxSap
*
*       Desc:  Invoked to bind RLC to UDX Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindKwUdxSap
(
Inst inst
)
#else
PUBLIC S16 smBindKwUdxSap(inst)
Inst inst;
#endif
{
   KwCntrl  *cntrl = NULLP;
   KwMngmt  kwMngmt;
   Pst      pst;
  
   TRC2(smBindKwUdxSap)
 
   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(kwMngmt.t.cntrl);

   cntrl->action            =  ABND;
   cntrl->subAction         =  SM_ZERO_VAL;
   cntrl->s.sapCntrl.suId   =  SM_ZERO_VAL;
   cntrl->s.sapCntrl.spId   =  SM_ZERO_VAL;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCNTRL;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = inst;
   kwMngmt.hdr.elmId.elmnt         = STUDXSAP; 
   kwMngmt.hdr.response.mem.region = (inst == WR_SM_KWUL_INST) ?
                                      KW_UL_MEM_REGION:KW_DL_MEM_REGION;
   kwMngmt.hdr.response.mem.pool   = KW_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.dstInst   = inst;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "RLC Cntrl Req to rg for Rgu sap");
   /* Send the request to the RRC */
   SmMiLkwCntrlReq(&pst, &kwMngmt);

   RETVALUE(ROK);
} /* smBindKwUdxSap */
 
/*
*
*       Fun:   smBindUnBindKwToRguSap
*
*       Desc:  Invoked to bind and Unbind RLC to RGU Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rlc.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindKwToRguSap
(
Inst inst,
U8   action
)
#else
PUBLIC S16 smBindUnBindKwToRguSap(inst,action)
Inst inst;
U8   action;
#endif
{
   KwCntrl  *cntrl = NULLP;
   KwMngmt  kwMngmt;
   Pst      pst;
  
   TRC2(smBindUnBindKwToRguSap)
 
   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(kwMngmt.t.cntrl);

   cntrl->action            =  action;
   cntrl->subAction         =  SM_ZERO_VAL;
   cntrl->s.sapCntrl.suId   =  SM_ZERO_VAL;
   cntrl->s.sapCntrl.spId   =  inst;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCNTRL;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = inst;
   kwMngmt.hdr.elmId.elmnt         = 186; /* ambiguous defines in lkw.h and lrg.h so direct hardcoded*/
   kwMngmt.hdr.response.mem.region = (inst == WR_SM_KWUL_INST) ?
                                      KW_UL_MEM_REGION:KW_DL_MEM_REGION;
   kwMngmt.hdr.response.mem.pool   = KW_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.dstInst   = inst;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   if (action == ABND)
   {
      RLOG0(L_DEBUG, "Cntrl Req to RLC to bind Rgu sap");
   }
   else
   {
      RLOG0(L_DEBUG, "Cntrl Req to RLC to unbind Rgu sap");
   }

   /* Send the request to the RRC */
   SmMiLkwCntrlReq(&pst, &kwMngmt);

   RETVALUE(ROK);
} /* smBindUnBindKwToRguSap */
 
/**
 * @brief This function is used to send enable/disable control request to 
 * the RLC layers.
 *
 * @details
 *
 * Function: smBuildKwLogCntrl
 *
 * @param[in] actType
 * @param[in] mask
 *
 * @return Void
 */

#ifdef ANSI
PUBLIC S16 smBuildKwLogCntrl
(
 Bool actType,
 U32  mask
)
#else
PUBLIC S16 smBuildKwLogCntrl(actType, mask)
 Bool actType;
 U32  mask;
#endif
{
   KwCntrl  *cntrl = NULLP;
   KwMngmt  kwMngmt;
   Pst      pst;

   TRC2(smBuildKwLogCntrl)

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(kwMngmt.t.cntrl);

   if(actType == WR_ON)
   {
      cntrl->action            =  AENA;
   }
   else
   {
      cntrl->action            =  ADISIMM;
   }

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCNTRL;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STGEN; 
   kwMngmt.hdr.response.mem.pool   = KW_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   /* Send the request to the RRC */
   SmMiLkwCntrlReq(&pst, &kwMngmt);

   RETVALUE(ROK);

} /* smBuildKwLogCntrl */
 
/*
*
*       Fun:   smBuildKwDbgCntrl
*
*       Desc:  Invoked to enable debug prints
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildKwDbgCntrl
(
Void
)
#else
PUBLIC S16 smBuildKwDbgCntrl(Void)
#endif
{
   KwCntrl  *cntrl = NULLP;
   KwMngmt  kwMngmt;
   Pst      pst;
  
   TRC2(smBuildKwDbgCntrl)
 
   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(kwMngmt.t.cntrl);

   cntrl->action            =  AENA;
   cntrl->subAction         =  SADBG;
   /*wr004.102: Platform changes */
   cntrl->s.dbgCntrl.dbgMask = 0;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCNTRL;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STGEN; 
   kwMngmt.hdr.response.mem.pool   = KW_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "RLC Debug Cntrl Req sent to rg");
   /* Send the request to the RRC */
   SmMiLkwCntrlReq(&pst, &kwMngmt);

   RETVALUE(ROK);

} /* smBuildKwDbgCntrl */

/* IPSec Recovery */

/*
*
*       Fun:   smBuildKwShutDownCntrl
*
*       Desc:  Invoked to shut down RLC
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_kw_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildKwShutDownCntrl
(
Inst inst
)
#else
PUBLIC S16 smBuildKwShutDownCntrl(inst)
Inst inst;
#endif
{
   KwCntrl  *cntrl = NULLP;
   KwMngmt  kwMngmt;
   Pst      pst;
  
   TRC2(smBuildKwShutDownCntrl)
 
   wrKwDlCfg = 0;
   //wrNumKwuDlSaps; 
   wrKwUlCfg = 0;
   wrNumKwuUlSaps = 0;

   SM_SET_ZERO(&kwMngmt, sizeof(KwMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(kwMngmt.t.cntrl);

   cntrl->action            =  ASHUTDOWN;
   cntrl->subAction         =  NULLD;

   /* Fill Header */
   kwMngmt.hdr.msgType             = TCNTRL;
   kwMngmt.hdr.entId.ent           = ENTKW;
   kwMngmt.hdr.entId.inst          = SM_INST_ZERO;
   kwMngmt.hdr.elmId.elmnt         = STGEN; 
   kwMngmt.hdr.response.mem.pool   = KW_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   pst.dstInst   = inst;
   
   RLOG0(L_DEBUG,"RLC shut down Cntrl Req sent to rg");
   /* Send the request to the RRC */
   SmMiLkwCntrlReq(&pst, &kwMngmt);

   RETVALUE(ROK);

} /* smBuildKwDbgCntrl */

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
*       File:  smkwexms.c
*
*/

#ifdef ANSI
PUBLIC S16 smKwActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smKwActvInit(ent, inst, region, reason)
Ent ent;                      /* entity */
Inst inst;                    /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
{
   TRC3(smKwActvInit)

   UNUSED(ent);
   UNUSED(inst);
   UNUSED(region);
   UNUSED(reason);
   RETVALUE(ROK);
}


/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from KW
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   wr_sm_kw_exms.c
*
*/

#ifdef ANSI
PUBLIC S16 smKwActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smKwActvTsk(pst, mBuf)
Pst *pst;                   /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16 ret;

   TRC2(smKwActvTsk)

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCKWMILKW
      case LKW_EVT_CFG_CFM:             /* Config confirm */
         ret = cmUnpkLkwCfgCfm(SmMiLkwCfgCfm, pst, mBuf);
         break;
      case LKW_EVT_CNTRL_CFM:           /* Control confirm */
         ret = cmUnpkLkwCntrlCfm(SmMiLkwCntrlCfm, pst, mBuf);
         break;
      case LKW_EVT_STA_IND:           /* Status Indication*/
         ret = cmUnpkLkwStaInd(SmMiLkwStaInd , pst, mBuf);
         break;
#ifdef LTE_L2_MEAS 
      case LKW_EVT_L2MEAS_CFM:
         ret = cmUnpkLkwL2MeasCfm(SmMiLkwL2MeasCfm, pst, mBuf);
         break;
      case LKW_EVT_L2MEAS_STOP_CFM:
         ret = cmUnpkLkwL2MeasStopCfm(SmMiLkwL2MeasStopCfm, pst, mBuf);
         break;
#endif
#endif

#ifdef LCPJMILPJ
      case LPJ_EVT_CFG_CFM:             /* Config confirm */
         ret = cmUnpkLpjCfgCfm(SmMiLpjCfgCfm, pst, mBuf);
         break;
      case LPJ_EVT_CNTRL_CFM:           /* Control confirm */
         ret = cmUnpkLpjCntrlCfm(SmMiLpjCntrlCfm, pst, mBuf);
         break;
      case LPJ_EVT_STA_IND:           /* Status Indication*/
         break;

#ifdef LTE_L2_MEAS
      case LPJ_EVT_L2MEAS_CFM:
         ret = cmUnpkLpjL2MeasCfm(SmMiLpjL2MeasCfm,pst,mBuf);
         break;
      case LPJ_EVT_L2MEAS_STOP_CFM:
         ret = cmUnpkLpjL2MeasStopCfm(SmMiLpjL2MeasStopCfm,pst,mBuf);
         break;
#endif /*LTE_L2_MEAS*/
#ifdef TENB_STATS
      case  EVTTENBL2UESTATS:
         TSInfUnpkL2UeStats(TSInfHdlL2UeStatsInd, pst, mBuf);
         break;
      case  EVTTENBL2CELLSTATS:
         TSInfUnpkL2CellStats(TSInfHdlL2CellStatsInd, pst, mBuf);
         break;
#endif
#endif

      default:
         WR_FREEMBUF(mBuf);
         ret = RFAILED;
         RLOG1(L_ERROR,"Received invalid event[%d]", pst->event);
         break;
   } /* end of switch statement */

   RETVALUE(ret);

} /* end of smKwActvTsk */


/**************************************************************************
 *
 *      Fun:   Configuration request
 *
 *      Desc:  This function is used to configure RRC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_sm_kw_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLkwCfgReq
(
Pst *pst,                 /* post structure */
KwMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLkwCfgReq(pst, cfg)
Pst *pst;                 /* post structure */
KwMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLkwCfgReq)
   /* jump to specific primitive depending on configured selector */
   (*smMiLkwCfgReqMt[pst->selector])(pst, cfg);
   RETVALUE(ROK);

} /* end SmMiLkwCfgReq */

/**************************************************************************
 *
 *      Fun:   Control request
 *
 *      Desc:  This function is used to send the control request RLC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_sm_kw_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLkwCntrlReq
(
Pst *pst,                 /* post structure */
KwMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLkwCntrlReq(pst, cntrl)
Pst *pst;                 /* post structure */
KwMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLkwCntrlReq)
   /* jump to specific primitive depending on configured selector */
   (*smMiLkwCntrlReqMt[pst->selector])(pst, cntrl);
   RETVALUE(ROK);
} /* end of SmMiLkwCntrlReq */


#ifdef PTSMLKW
/**************************************************************************
 *
 *      Fun:   Portable Configuration request
 *
 *      Desc:  This function is used to send the configuration request RLC
 *             (Portable)
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_sm_kw_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtMiLkwCfgReq
(
Pst *pst,                   /* post structure */
KwMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLkwCfgReq(pst, cfg)
Pst *pst;                   /* post structure */
KwMngmt *cfg;               /* configure */
#endif
{
  TRC3(PtMiLkwCfgReq);
  UNUSED(cfg);
  UNUSED(pst);
  /* wr002.102: Removed TRC2 macro */
  RETVALUE(ROK);
} /* end of PtMiLkwCfgReq */

/**************************************************************************
 *
 *      Fun:   Portable Control request
 *
 *      Desc:  This function is used to send the control request RLC
 *             (Portable)
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_sm_kw_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtMiLkwCntrlReq
(
Pst *pst,                   /* post structure */
KwMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLkwCntrlReq(pst, cntrl)
Pst *pst;                   /* post structure */
KwMngmt *cntrl;             /* control */
#endif
{
  TRC3(PtMiLkwCntrlReq);

  UNUSED(cntrl);
  UNUSED(pst);
  /* wr002.102: Removed TRC2 macro */
  RETVALUE(ROK);
} /* end of PtMiLkwCntrlReq */

#ifdef LTE_L2_MEAS
#ifdef ANSI
PRIVATE S16 PtMiLkwL2MeasReq 
(
Pst *pst,                   /* post structure */
KwL2MeasReqEvt *measReqEvt
)
#else
PRIVATE S16 PtMiLkwL2MeasReq(pst, measReqEvt)
Pst *pst,                   /* post structure */
KwL2MeasReqEvt *measReqEvt
#endif
{
  TRC3(PtMiLkwL2MeasReq);

  UNUSED(measReqEvt);
  UNUSED(pst);
  /* wr002.102: Removed TRC2 macro */
  RETVALUE(ROK);
} /* end of PtMiLkwCntrlReq */

#ifdef ANSI
PRIVATE S16 PtMiLkwL2MeasStopReq 
(
Pst *pst,                   /* post structure */
U8  measType
)
#else
PRIVATE S16 PtMiLkwL2MeasStopReq(pst, cntrl)
Pst *pst;                   /* post structure */
U8  measType;
#endif
{
  TRC3(PtMiLkwL2MeasStopReq);

  UNUSED(measType);
  UNUSED(pst);
  /* wr002.102: Removed TRC2 macro */
  RETVALUE(ROK);
} /* end of PtMiLkwCntrlReq */

#ifdef ANSI
PRIVATE S16 PtMiLkwL2MeasSendReq
(
Pst *pst,                   /* post structure */
U8 measType
)
#else
PRIVATE S16 PtMiLkwL2MeasSendReq(pst, cntrl)
Pst *pst;                   /* post structure */
U8  measType;
#endif
{
  TRC3(PtMiLkwL2MeasSendReq);

  UNUSED(measType);
  UNUSED(pst);
  /* wr002.102: Removed TRC2 macro */
  RETVALUE(ROK);
} /* end of PtMiLkwL2MeasSendReq */
#endif/*LTE_L2_MEAS*/ 

#endif /* PTSMLKW */
 
 
/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Managemer.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_sm_kw_ptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 smProcKwUlCfgCfm
(
Pst     *pst,          /* post structure */
KwMngmt *cfm           /* configuration */
)
#else
PRIVATE S16 smProcKwUlCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
KwMngmt *cfm;          /* configuration */
#endif
{
   Pst      kwPst;
   PRIVATE U8 wrKwCfg = 0;
   PRIVATE U8 wrNumKwuSaps = 0;

   TRC2(smProcKwUlCfgCfm);

   SM_SET_ZERO(&kwPst, sizeof(Pst));

   RLOG1(L_DEBUG, "RLC UL Cfg status %d",cfm->cfm.status);
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            wrKwCfg |= WR_KW_GEN_CFG;
            break;      
         case STCKWSAP:
            wrKwCfg |= WR_KW_CKW_SAP_CFG;
            break;
         case STKWUSAP:
            wrKwCfg |= WR_KW_KWU_SAP_CFG;
            wrNumKwuSaps ++;
            break;
         case STRGUSAP: 
            wrKwCfg |= WR_KW_RGU_SAP_CFG;
            break;
         case STUDXSAP:
            wrKwCfg |= WR_KW_UDX_SAP_CFG;
            break;
         default:
            break;
      } /* end of switch statement */
      RLOG2(L_DEBUG, "RLC UL Cfg Cfm received for the element %d  wrKwCfg %x",
         cfm->hdr.elmId.elmnt,wrKwCfg);

     // if ((wrKwCfg == 0xf /*chetan WR_SM_KW_UL_CONFIGURED*/) && (wrNumKwuSaps == 2))
      if ((wrKwCfg == WR_SM_KW_UL_CONFIGURED) && (wrNumKwuSaps == 2))
	  {
         smCb.smState = WR_SM_STATE_RLC_UL_CFG_DONE;
         RLOG0(L_ALWAYS, "RLC UL CONFIGURATION SUCCESSFULLY PERFORMED");
#ifdef WR_RSYS_OAM  
         smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
#endif
         wrKwCfg = 0;
         wrNumKwuSaps = 0;
         WR_FILL_PST(kwPst, SM_KW_PROC, ENTSM, ENTWR, EVTWRRLCCFG, WR_POOL, SM_SELECTOR_LC);

         if(wrSmSelfPstMsg(&kwPst) != ROK)
         {
            RLOG0(L_FATAL, "Self Pst failed after RLC UL Configuration");
         }
         else
         {
            RLOG0(L_DEBUG, "Self Pst success after RLC UL Configuration");
         } /* end of else part */

      }  /* end of wrKwCfg == WR_SM_KW_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
      RLOG1(L_FATAL,"CONFIGURATION CONFIRM NOK for RLC UL. wrKwCfg %x", wrKwCfg);
#ifdef WR_RSYS_OAM  
      /*timer stop :: Raised an alarm */
      smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
      smCb.cfgCb.alarm.severity  = SM_ALARM_SEVERITY_MAJOR;
      smCb.cfgCb.alarm.causeType = SM_ALARM_CAUSE_TYPE_ERR_IND;
      smCb.cfgCb.alarm.causeVal  = SM_ALARM_CAUSE_VAL_INIT_FAIL;
      smStaticCfgFailAlarm();
#endif
   }

   RETVALUE(ROK);
} /* end of smProcKwUlCfgCfm */

/*
 *
 *       Fun:   Configuration Confirm
 *
 *       Desc:  This function is used by to present configuration confirm
 *              information to Layer Managemer.
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  wr_sm_kw_ptmi.c
 *
 */

#ifdef ANSI
PRIVATE S16 smProcKwDlCfgCfm
(
 Pst     *pst,          /* post structure */
 KwMngmt *cfm           /* configuration */
 )
#else
PRIVATE S16 smProcKwDlCfgCfm(pst, cfm)
   Pst     *pst;          /* post structure */
   KwMngmt *cfm;          /* configuration */
#endif
{
   Pst      kwPst;
   PRIVATE U8 wrKwCfg = 0;
   PRIVATE  U8 wrNumKwuSaps = 0;
   TRC2(smProcKwDlCfgCfm);

   SM_SET_ZERO(&kwPst, sizeof(Pst));

   RLOG2(L_DEBUG, "RLC DL Cfg status %d wrKwCfg %x",cfm->cfm.status,wrKwCfg);
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            wrKwCfg |= WR_KW_GEN_CFG;
            break;      
         case STKWUSAP:
            wrKwCfg |= WR_KW_KWU_SAP_CFG;
            wrNumKwuSaps ++;
            break;
         case STRGUSAP: 
            wrKwCfg |= WR_KW_RGU_SAP_CFG;
            break;
         case STUDXSAP:
            wrKwCfg |= WR_KW_UDX_SAP_CFG;
            break;
         default:
            break;
      } /* end of switch statement */
      RLOG2(L_DEBUG, "RLC DL Cfg Cfm received for the element %d  wrKwCfg %x",
            cfm->hdr.elmId.elmnt,wrKwCfg);

      //if ((wrKwCfg == 0x19 /*chetan WR_SM_KW_DL_CONFIGURED*/) && (wrNumKwuSaps == 2))
      if ((wrKwCfg == WR_SM_KW_DL_CONFIGURED) && (wrNumKwuSaps == 2))
      {
         smCb.smState = WR_SM_STATE_RLC_DL_CFG_DONE;
         RLOG0(L_ALWAYS, "RLC DL CONFIGURATION SUCCESSFULLY PERFORMED.");
#ifdef WR_RSYS_OAM  
         smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
#endif
         wrKwCfg = 0;
         wrNumKwuSaps = 0;
         WR_FILL_PST(kwPst, SM_KW_PROC, ENTSM, ENTWR, EVTWRMACCFG, WR_POOL, SM_SELECTOR_LC);

         if(wrSmSelfPstMsg(&kwPst) != ROK)
         {
            RLOG0(L_FATAL, "Self Pst failed after RLC DL Configuration.");
         }
         else
         {
            RLOG0(L_DEBUG, "Self Pst success after RLC DL Configuration.");
         } /* end of else part */

      }  /* end of wrKwCfg == WR_SM_KW_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
      RLOG1(L_ERROR, "CONFIGURATION CONFIRM NOK  for RLC DL wrKwCfg %x",wrKwCfg);
#ifdef WR_RSYS_OAM  
      /*timer stop :: Raised an alarm */
      smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
      smCb.cfgCb.alarm.severity  = SM_ALARM_SEVERITY_MAJOR;
      smCb.cfgCb.alarm.causeType = SM_ALARM_CAUSE_TYPE_ERR_IND;
      smCb.cfgCb.alarm.causeVal  = SM_ALARM_CAUSE_VAL_INIT_FAIL;
      smStaticCfgFailAlarm();
#endif
   }

   RETVALUE(ROK);
} /* end of smProcKwDlCfgCfm */

#ifdef LTE_L2_MEAS 
/*
*
*       Fun:   Sm build L2 Meas Req 
*
*       Desc:  This function is used build and send L2 Meas Req 
*              to RLC layer
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_rlc.c
*
*/

#ifdef ANSI 
PUBLIC S16 smSndKwL2MeasStartReq
(
U8              measType
)
#else
PUBLIC S16 smSndKwL2MeasStartReq(measType)
U8   measType;
#endif
{
   Pst  pst;
   KwL2MeasReqEvt l2MeasEvt;
   KwL2MeasReqInfo  *measReq;

   TRC3(smBuildKwL2MeasStartReq);
   RLOG1(L_DEBUG,"smSndKwL2MeasStartReq received with measType[%d]",measType);
   SM_SET_ZERO(&l2MeasEvt, sizeof(KwL2MeasReqEvt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   /*filling pst structure*/
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;

   /*filling L2 counters meas info*/
   l2MeasEvt.transId = (kpiInfo.transId)++;
   l2MeasEvt.measPeriod = 0; /*collection Period value*/
   measReq= &(l2MeasEvt.measReq);
   measReq->measType = measType;

   if(measType == LKW_L2MEAS_ACT_UE)
   {    
      measReq->val.nonIpThMeas.numSamples = 3;
   }/*For Non Ip Throughput L2 cntrs*/

   SmMiLkwL2MeasReq(&pst,&l2MeasEvt);
   RETVALUE(ROK);
}/*smSndKwL2MeasStartReq*/

/*
*  
*       Fun:   Sm build L2 Meas Stop Req 
*  
*       Desc:  This function is used build and send L2 Meas Stop Req 
*              to RLC layer
* 
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_rlc.c
*
*/
#ifdef ANSI 
PUBLIC S16 smSndKwL2MeasStopReq
(
U8              measType
)
#else
PUBLIC S16 smSndKwL2MeasStopReq(measType)
U8   measType;
#endif
{
   Pst  pst;

   TRC3(smSndKwL2MeasStopReq);

   RLOG1(L_DEBUG,"smSndKwL2MeasStartReq received with measType[%d]",measType);
   SM_SET_ZERO(&pst, sizeof(Pst));
   /*filling pst structure*/
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;

   /*filling L2 counters meas info*/
   SmMiLkwL2MeasStopReq(&pst,measType);

   RETVALUE(ROK);
}/* smSndKwL2MeasStopReq */

/*
*  
*       Fun:   Sm build L2 Meas Stop Req 
*  
*       Desc:  This function is used build and send L2 Meas Stop Req 
*              to RLC layer
* 
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_rlc.c
*
*/
#ifdef ANSI 
PUBLIC S16 smSndKwL2MeasSendReq
(
U8              measType
)
#else
PUBLIC S16 smSndKwL2MeasSendReq(measType)
U8   measType;
#endif
{
   Pst  pst;

   TRC3(smSndKwL2MeasSendReq);

   RLOG1(L_DEBUG, "smSndKwL2MeasSendReq received with measType[%d]",measType);
   SM_SET_ZERO(&pst, sizeof(Pst));
   /*filling pst structure*/
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTKW;
   pst.dstProcId = SM_KW_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;

   /*filling L2 counters meas info*/
   SmMiLkwL2MeasSendReq(&pst,measType);

   RETVALUE(ROK);
}/* smSndKwL2MeasSendReq */


/*
*
*       Fun:   SmMiLkwL2MeasCfm
*
*       Desc:  This function recevies L2 cntrs Cfm from
*               Rlc layer
*
*       Ret:   S16 ret 
*
*       Notes:
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLkwL2MeasCfm
(
Pst     *pst,           /* post structure */
KwL2MeasCfmEvt *measEvt /* measuremnet */
)
#else
PUBLIC S16 SmMiLkwL2MeasCfm(pst, action, sts)
Pst     *pst;         /* post structure */
KwL2MeasCfmEvt *measEvt; /* measuremnet */
#endif
{
   S16 ret = ROK;
   U32  kpiId = INVALID_KPI;
   U16 numOfQci;
   U8  measType;
   U8  qciVal;

   TRC2(SmMiLkwL2MeasCfm);

   /*This function updates L2 cntrs info comes from RLC in OAM*/ 
   measType = measEvt->measType;
   kpiInfo.numOfRcvdL2MeasCfm++;

   if(measType & LKW_L2MEAS_DL_DISC)
   {
      kpiInfo.measConfigRecv |= WR_MEAS_RLC_UULOSS_CFG;
      RLOG0(L_DEBUG,"Received Confirmation for RLC Discard");
   }
   else if ( (measType & LKW_L2MEAS_UL_IP) || (measType & LKW_L2MEAS_DL_IP) )
   {
      kpiInfo.measConfigRecv |= WR_MEAS_RLC_DLULIP_CFG;
      RLOG0(L_DEBUG,"Received Confirmation for RLC DL/UL IP TPT");
   }
   else
   {
      /*Other Meas types are not handled*/
   }
   RLOG3(L_INFO,"SmMiLkwL2MeasCfm {%d} measType{%d} NbQci[%d]",
      kpiInfo.numOfRcvdL2MeasCfm,measType, measEvt->val.nonIpThMeas.numCfm);

   if(measType & LKW_L2MEAS_ACT_UE || measType & LKW_L2MEAS_UU_LOSS || measType & LKW_L2MEAS_DL_DISC || measType & LKW_L2MEAS_DL_DELAY)
   {
      U8 indx;      
      numOfQci = measEvt->val.nonIpThMeas.numCfm;      
      for(indx = 0;indx < numOfQci;indx++)
      {
         qciVal = measEvt->val.nonIpThMeas.measCfm[indx].qci;

         if( (qciVal < 1) || (qciVal > 9) ) 
         {
            continue;
         }

         if(measType & LKW_L2MEAS_ACT_UE)
         {

         }/*Updates Active Dl Ues per QCI L2 cntrs*/

         if( measType & LKW_L2MEAS_DL_DISC )
         {
            kpiId = KPI_ID_LTE_DRB_PDCPSDUDROPRATEDL_QCI1;
            WR_INC_KPI((kpiId + qciVal - 1),KPI_VALUE_TYPE_INTEGER,
                  measEvt->val.nonIpThMeas.measCfm[indx].val.nonIpThrput.dlDiscRate);
            WR_INC_KPI(KPI_ID_LTE_DRB_PDCPSDUDROPRATEDL_QCI_SUM,
                  KPI_VALUE_TYPE_INTEGER,measEvt->val.nonIpThMeas.measCfm[indx].val.nonIpThrput.dlDiscRate); 
            RLOG2(L_INFO,"L2_MEAS RESULT:NonIP: DL DISC QCI[%d] Rate[%ld]",
               qciVal,measEvt->val.nonIpThMeas.measCfm[indx].val.nonIpThrput.dlDiscRate);
         }
         if(measType & LKW_L2MEAS_UU_LOSS )
         {
            kpiId = KPI_ID_LTE_DRB_PDCPSDUAIRLOSSRATEDL_QCI1;
            WR_INC_KPI((kpiId + qciVal - 1),KPI_VALUE_TYPE_INTEGER,
                  measEvt->val.nonIpThMeas.measCfm[indx].val.nonIpThrput.uuLoss);      
            WR_INC_KPI(KPI_ID_LTE_DRB_PDCPSDUAIRLOSSRATEDL_QCI_SUM,
                  KPI_VALUE_TYPE_INTEGER,measEvt->val.nonIpThMeas.measCfm[indx].val.nonIpThrput.uuLoss); 
         }/*Updates PDCP UL LOSS per QCI L2 cntrs*/
         if(measType & LKW_L2MEAS_DL_DELAY)
         {
            kpiId = KPI_ID_LTE_DRB_PDCPSDUDELAYDL_QCI1;
            WR_INC_KPI((kpiId + qciVal - 1),KPI_VALUE_TYPE_INTEGER,
                  measEvt->val.nonIpThMeas.measCfm[indx].val.nonIpThrput.dlSduDelay);
            WR_INC_KPI(KPI_ID_LTE_DRB_PDCPSDUDELAYDL_QCI_SUM,
                  KPI_VALUE_TYPE_INTEGER,measEvt->val.nonIpThMeas.measCfm[indx].val.nonIpThrput.dlSduDelay); 
         }
      }
   }
   else
   {
      U32 qciUlTh[WR_KPI_MAX_QCI] = {0};
      U32 qciDlTh[WR_KPI_MAX_QCI] = {0};
      U8  cntr2;
      U8  cntr3;
      U32 printInt;

      U8  cntr;
      U8  cntr1;
      for(cntr = 0;cntr < measEvt->val.ipThMeas.numUes;cntr++) 
      {
         numOfQci = measEvt->val.ipThMeas.ueInfoLst[cntr].numCfm;
         for(cntr1 = 0;cntr1< numOfQci;cntr1++)
         {
            if(measType & LKW_L2MEAS_UL_IP)
            {          
               qciUlTh[measEvt->val.ipThMeas.ueInfoLst[cntr].measCfm[cntr1].qci] += 
                  measEvt->val.ipThMeas.ueInfoLst[cntr].measCfm[cntr1].val.ipThrput.ulIpThPut;
               RLOG3(L_INFO,"L2_MEAS RESULT:IP: UE %d QCI %d UL_TPT  %d",
                  measEvt->val.ipThMeas.ueInfoLst[cntr].ueId,
                  measEvt->val.ipThMeas.ueInfoLst[cntr].measCfm[cntr1].qci,
                  printInt = measEvt->val.ipThMeas.ueInfoLst[cntr].measCfm[cntr1].val.ipThrput.ulIpThPut);
            }
            if(measType & LKW_L2MEAS_DL_IP)
            {
               qciDlTh[measEvt->val.ipThMeas.ueInfoLst[cntr].measCfm[cntr1].qci] +=
                  measEvt->val.ipThMeas.ueInfoLst[cntr].measCfm[cntr1].val.ipThrput.dlIpThPut;

               RLOG3(L_INFO, " L2_MEAS RESULT:IP: UE %d QCI %d DL_TPT %f ",
                  measEvt->val.ipThMeas.ueInfoLst[cntr].ueId,
                  measEvt->val.ipThMeas.ueInfoLst[cntr].measCfm[cntr1].qci,
                  printInt = measEvt->val.ipThMeas.ueInfoLst[cntr].measCfm[cntr1].val.ipThrput.dlIpThPut);
            }         
         }
      }   

      if(measType & LKW_L2MEAS_DL_IP)
      {
         for(cntr2 = 1;cntr2 < WR_KPI_MAX_QCI;cntr2++)
         {      
            if(qciDlTh[cntr2] != 0 )
            {       
               kpiId = KPI_ID_LTE_DRB_IPTHPDL_QCI1;
               WR_INC_KPI((kpiId + cntr2 - 1),KPI_VALUE_TYPE_INTEGER,
                     qciDlTh[cntr2]);
            }
         } 
      }    
      if(measType & LKW_L2MEAS_UL_IP)
      {
         for(cntr3 = 1;cntr3 < WR_KPI_MAX_QCI;cntr3++)
         {      
            if(qciUlTh[cntr3] != 0 )
            {        
               kpiId = KPI_ID_LTE_DRB_IPTHPUL_QCI1;
               WR_INC_KPI((kpiId + cntr3 - 1),KPI_VALUE_TYPE_INTEGER,
                     qciUlTh[cntr3]);
            }
         }/*Updates UP throughput per QCi L2 cntrs*/
      }
   }

   /* Push Rlc layer counter*/
   //if(kpiInfo.numOfTrggrL2MeasReq == kpiInfo.numOfRcvdL2MeasCfm)
   if( kpiInfo.measConfigRecv == WR_MEAS_CFG_DONE )
   {
#ifdef WR_RSYS_OAM
      KpisInfoSentNotification();
#endif
      kpiInfo.measConfigRecv = 0;
      kpiInfo.numOfTrggrL2MeasReq = WR_KPI_SET_TO_ZERO;
      kpiInfo.numOfRcvdL2MeasCfm  = WR_KPI_SET_TO_ZERO;
   }

   RETVALUE(ret);

} /*SmMiLkwL2MeasCfm*/

/*
*
*       Fun:   SmMiLkwL2MeasStopCfm
*
*       Desc:  This function recevies L2 cntrs Cfm from
*               Rlc layer
*
*       Ret:   S16 ret 
*
*       Notes:
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLkwL2MeasStopCfm
(
Pst     *pst,           /* post structure */
U8      measType,
U8      status
)
#else
PUBLIC S16 SmMiLkwL2MeasStopCfm(pst, measType, status)
Pst     *pst;         /* post structure */
U8      measType;
U8      status;
#endif
{
   S16 ret = ROK;

   TRC2(SmMiLkwL2MeasStopCfm);

   kpiInfo.numOfL2MeasStopCfm++;
   RLOG2(L_INFO, "numOfL2MeasStopCfm[%d] numOfL2MeasStopCfm[%d]",
      kpiInfo.numOfL2MeasStopReq,kpiInfo.numOfL2MeasStopCfm);
   if((status == LCM_PRIM_OK) && (kpiInfo.numOfL2MeasStopReq == kpiInfo.numOfL2MeasStopCfm))
   {
      /*KpiCollection is stopped*/
      wrKpiStopKpiCollecPrcCfm();
      kpiInfo.numOfL2MeasStopReq = WR_KPI_SET_TO_ZERO;
      kpiInfo.numOfL2MeasStopCfm = WR_KPI_SET_TO_ZERO;
   }
   else
   {
      /*could not stop l2 measurement in rlc*/
   }

   RETVALUE(ret);
} /* SmMiLkwL2MeasStopCfm */
 
PRIVATE CONSTANT LkwL2MeasReq SmMiLkwL2MeasStartReqMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwL2MeasReq,
#else
   PtMiLkwL2MeasReq,
#endif
#ifdef PJ
   KwMiLkwL2MeasReq,
#else
#ifdef PTSMLKW
   PtMiLkwL2MeasReq,
#endif
#endif
};
PRIVATE CONSTANT LkwL2MeasStopReq SmMiLkwL2MeasStopReqMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwL2MeasStopReq,
#else
   PtMiLkwL2MeasStopReq,
#endif
#ifdef PJ
   KwMiLkwL2MeasStopReq,
#else
#ifdef PTSMLKW
   PtMiLkwL2MeasStopReq,
#endif
#endif
};
PRIVATE CONSTANT LkwL2MeasSendReq SmMiLkwL2MeasSendReqMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwL2MeasSendReq,
#else
   PtMiLkwL2MeasSendReq,
#endif
#ifdef PJ
   KwMiLkwL2MeasSendReq,
#else
#ifdef PTSMLKW
   PtMiLkwL2MeasSendReq,
#endif
#endif
};
 
/*
*
*       Fun: SmMiLkwL2MeasStopReq 
*
*       Desc:  This function is used to build L2 Meas Stop Req 
*              to send  RLC layer
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_rlc.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLkwL2MeasSendReq
(
Pst            *pst,
U8             measType
)
#else
PUBLIC S16 SmMiLkwL2MeasSendReq (pst, measType)
Pst            *pst;
U8             measType;
#endif
{
   TRC3(SmMiLkwL2MeasSendReq )

   (*SmMiLkwL2MeasSendReqMt[pst->selector])(pst,measType);

   RETVALUE(ROK);
}/* SmMiLkwL2MeasSendReq */
 
/*
*
*       Fun: SmMiLkwL2MeasStopReq 
*
*       Desc:  This function is used to build L2 Meas Stop Req 
*              to send  RLC layer
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_rlc.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLkwL2MeasStopReq
(
Pst            *pst,
U8             measType
)
#else
PUBLIC S16 SmMiLkwL2MeasStopReq (pst, measType)
Pst            *pst;
U8             measType;
#endif
{
   TRC3(SmMiLkwL2MeasStopReq )

   (*SmMiLkwL2MeasStopReqMt[pst->selector])(pst,measType);

   RETVALUE(ROK);
}/*SmMiLkwL2MeasStopReq*/
 
/*
*
*       Fun: SmMiLkwL2MeasReq 
*
*       Desc:  This function is used to build L2 Meas Req 
*              to send  RLC layer
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_rlc.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLkwL2MeasReq
(
Pst            *pst,
KwL2MeasReqEvt *measReqEvt
)
#else
PUBLIC S16 SmMiLkwL2MeasReq (pst, measReqEvt)
Pst            *pst;
KwL2MeasReqEvt *measReqEvt;
#endif
{
   TRC3(SmMiLkwL2MeasReq )

   (*SmMiLkwL2MeasStartReqMt[pst->selector])(pst,measReqEvt);

   RETVALUE(ROK);
}/*SmMiLkwL2MeasReq*/ 
#endif/*LTE_L2_MEAS*/ 

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Managemer.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_sm_kw_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLkwCfgCfm
(
Pst     *pst,          /* post structure */
KwMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLkwCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
KwMngmt *cfm;          /* configuration */
#endif
{
   /*Pst      kwPst;*/

   TRC2(SmMiLkwCfgCfm);

   if (pst->srcInst == WR_SM_KWUL_INST)
      {
      smProcKwUlCfgCfm(pst, cfm);
         }
         else
         {
      smProcKwDlCfgCfm(pst, cfm);
   }

   RETVALUE(ROK);
} /* end of SmMiLkwCfgCfm */

/*
 *
 *       Fun:   Control Confirm
 *
 *       Desc:  This function is used by to present control confirm
 *              information to Layer Management.
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  wr_sm_kw_ptmi.c
 *
 */
#ifdef ANSI
PUBLIC S16 SmMiLkwCntrlCfm
(
 Pst     *pst,          /* post structure */
 KwMngmt *cntrl         /* control */
 )
#else
PUBLIC S16 SmMiLkwCntrlCfm(pst, cntrl)
   Pst     *pst;          /* post structure */
   KwMngmt *cntrl;        /* control */
#endif
{
   Pst  kwPst;

   TRC2(SmMiLkwCntrlCfm);

   SM_SET_ZERO(&kwPst, sizeof(Pst));
   /* IPSec */
   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
//         if (smCb.smState == WR_SM_STATE_PDCP_DL_SHUTDWN_DONE)
      if (smCb.smState == WR_SM_STATE_PDCP_UL_SHUTDWN_DONE)
      {
         if (pst->srcInst == WR_SM_KWDL_INST)
         {
            smCb.smState = WR_SM_STATE_RLC_DL_SHUTDWN_DONE;
            RLOG0(L_ALWAYS, "RLC DL SHUTDOWN SUCCESSFULLY PERFORMED.");
            WR_FILL_PST(kwPst, SM_KW_PROC, ENTSM, ENTWR, EVTWRRLCSHUT, WR_POOL, SM_SELECTOR_LC);

            if(wrSmSelfPstMsg(&kwPst) != ROK)
            {
               RLOG0(L_FATAL, "Self Pst failed after RLC DL SHUTDOWN Configuration.");
            }
            else
            {
               RLOG0(L_DEBUG, "Self Pst success after RLC DL SHUTDOWN Configuration.");
            } /* end of else part */
         }
         else
         {
            RLOG1(L_ERROR, " Invalid DL Instance ID %d ", pst->srcInst);
         }
         RETVALUE(ROK);
      }
      else if (smCb.smState == WR_SM_STATE_RLC_DL_SHUTDWN_DONE)
      {
         if (pst->srcInst == WR_SM_KWUL_INST)
      {
            smCb.smState = WR_SM_STATE_RLC_UL_SHUTDWN_DONE;
            RLOG0(L_ALWAYS, "RLC UL SHUTDOWN SUCCESSFULLY PERFORMED.");
         WR_FILL_PST(kwPst, SM_KW_PROC, ENTSM, ENTWR, EVTWRMACSCHSHUT, WR_POOL, SM_SELECTOR_LC);

         if(wrSmSelfPstMsg(&kwPst) != ROK)
         {
               RLOG0(L_ERROR, "Self Pst failed after RLC UL SHUTDOWN Configuration.");
         }
         else
         {
               RLOG0(L_DEBUG, "Self Pst success after RLC UL SHUTDOWN Configuration.");
         } /* end of else part */
         }
         else
         {
            RLOG1(L_ERROR, " Invalid DL Instance ID %d ", pst->srcInst);
         }
         RETVALUE(ROK);
      }
   }  /* end of wrKwCfg == WR_SM_KW_CONFIGURED */
   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
      if (smCb.smState == WR_SM_STATE_PDCP_UL_SHUTDWN_DONE)
      {
         if (pst->srcInst == WR_SM_KWDL_INST)
         {
            smCb.smState = WR_SM_STATE_RLC_DL_SHUTDWN_DONE;
            RLOG0(L_ALWAYS, "RLC DL SHUTDOWN SUCCESSFULLY PERFORMED.");
            WR_FILL_PST(kwPst, SM_KW_PROC, ENTSM, ENTWR, EVTWRRLCSHUT, WR_POOL, SM_SELECTOR_LC);

            if(wrSmSelfPstMsg(&kwPst) != ROK)
   {
               RLOG0(L_FATAL, "Self Pst failed after RLC DL SHUTDOWN Configuration.");
            }
            else
            {
               RLOG0(L_DEBUG, "Self Pst success after RLC DL SHUTDOWN Configuration.");
            } /* end of else part */
         }
         else
         {
            RLOG1(L_ERROR, " Invalid DL Instance ID %d ", pst->srcInst);
         }
         RETVALUE(ROK);
      }
   }  /* end of wrKwCfg == WR_SM_KW_CONFIGURED */

   if(cntrl->cfm.status == LCM_PRIM_OK)
   {
      switch (cntrl->hdr.elmId.elmnt)
      {
         case  STGEN:
            {
      if((smCfgCb.kwDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_KW_DBG_MASK))
      {
         /* Update DbgMsk */
         smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | WR_SM_KW_DBG_MASK);
         /* Trigger Debug Cntrl */
         wrSmHdlEnbDebugs(EVTWRENBDEBUGS);
      }
               break;
            }
         case  STRGUSAP:
            {
               /* Due to unbind */
               if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
               {
                  if (cntrl->cfm.status == LCM_PRIM_OK)
                  {
                     if (pst->srcInst == WR_SM_KWDL_INST)
                     {
                        RLOG0(L_DEBUG,"UNBIND OF RLC DL WITH MAC DL SUCCESSFUL");
                        /* Unbind RLC UL to MAC UL */
                        smBindUnBindKwToRguSap(WR_SM_KW_INST,AUBND);
                     }
                     else if (pst->srcInst == WR_SM_KW_INST)
                     {
                        RLOG0(L_DEBUG,"UNBIND OF RLC UL WITH MAC UL SUCCESSFUL");
                        /* Unbind SCH to CL */
                        smBindUnBindRgSchToTfuSap(AUBND);
                     }
                  }
                  RETVALUE(ROK);
               }

               if (pst->srcInst == WR_SM_KWDL_INST)
               {
                  RLOG0(L_DEBUG, "BIND OF RLC DL TO MAC DL (RGU) SAP SUCCESSFUL");
                  /* Initiate control request for binding PJ to RLC */
                  smBindUnBindPjKwuSap((Inst)WR_SM_KWUL_INST, ABND); 
   }
   else
   {
                  RLOG0(L_DEBUG, "BIND OF RLC UL TO MAC UL (RGU) SAP SUCCESSFUL");
                  /* Initiate control request for binding RRC to RLC */
                  smBindKwUdxSap((Inst)WR_SM_KWUL_INST); 

               }
               break;
            }
         case  STUDXSAP:
            {
               if (pst->srcInst == WR_SM_KWUL_INST)
               {
                  RLOG0(L_DEBUG, "BIND OF RLC UL TO RLC UL (UDX) SAP SUCCESSFUL");
                  /* Initiate control request for binding RRC to RLC */
                  smBindUnBindKwToRguSap((Inst) WR_SM_KWDL_INST,ABND); 

               }
               break;
            }
         default:
            break;
      }
   }

   RETVALUE(ROK);
} /* end of SmMiLkwCntrlCfm */


/*
 *
 *       Fun:   Sts Confirm
 *
 *       Desc:  This function is used by to present Sts confirm
 *              information to Layer Management.
 *
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_sm_kw_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLkwStsCfm 
(
Pst     *pst,          /* post structure */
Action  action,        /* Action */
KwMngmt *sts           /* statistics */
)
#else
PUBLIC S16 SmMiLkwStsCfm(pst, action, sts)
Pst     *pst;          /* post structure */
Action  action;        /* Action */
KwMngmt *sts;          /*  statistics */
#endif
{
   TRC2(SmMiLkwStsCfm)

   UNUSED(pst);
   UNUSED(action);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of SmMiLkwStsCfm */


/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used by to present Status confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_sm_kw_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLkwStaCfm
(
Pst     *pst,          /* post structure */
KwMngmt *sta           /* Status */
)
#else
PUBLIC S16 SmMiLkwStaCfm(pst,sta)
Pst     *pst;          /* post structure */
KwMngmt *sta;          /* Status */
#endif  
{
   TRC2(SmMiLkwStaCfm)

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of SmMiLkwStaCfm */


/*
*
*       Fun:   Status Ind
*
*       Desc:  This function is used by to present Status Ind
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_sm_kw_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLkwStaInd
(
Pst     *pst,          /* post structure */
KwMngmt *usta           /* configuration */
)
#else
PUBLIC S16 SmMiLkwStaInd(pst, usta)
Pst     *pst;          /* post structure */
KwMngmt *usta;          /* configuration */
#endif
{
   TRC2(SmMiLkwStaInd)

   UNUSED(pst);
   UNUSED(usta);

   RETVALUE(ROK);
} /* end of SmMiLkwStaInd */


/*
*
*       Fun:   Trc Ind
*
*       Desc:  This function is used by to present Trc Ind
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_sm_kw_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLkwTrcInd
(
Pst      *pst,          /* post structure */
KwMngmt  *trc,          /* trace indication */
Buffer   *mBuf          /* message buffer */
)
#else
PUBLIC S16 SmMiLkwTrcInd(pst,trc, mBuf)
Pst      *pst;          /* post structure */
KwMngmt  *trc;          /* trace indication */
Buffer   *mBuf;         /* message buffer */
#endif
{
   TRC2(SmMiLkwTrcInd)

   UNUSED(pst);
   UNUSED(trc);
   UNUSED(mBuf);

   RETVALUE(ROK);
} /* end of SmMiLkwTrcInd */


/********************************************************************30**

           End of file:     fc_smm_rlc_brcm.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:52:24 2014

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
SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
