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
               

     File:     wr_smm_rrc.c

     Sid:      fc_smm_rrc_brcm.c@@/main/TeNB_Main_BR/3 - Mon Aug 11 16:52:26 2014

     Prg:      Sriky 

*********************************************************************21*/
/* header include files */ 
static const char* RLOG_MODULE_NAME="SMM";

static int RLOG_FILE_ID=48;

static int RLOG_MODULE_ID=2;

#include "wr.h"
#include "wr_cmn.h"
#include "lnh.h"
#include "lnh.x"
#include "wr_lwr.h"
#include "wr_smm_init.h"

#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */

PRIVATE U16 wrNhCfg = 0;
PRIVATE U16 numBinds = 0;
PRIVATE Bool bndInPrg = 1;

#define MAXNHMI  2         /* The size of the LC/TC matrix */

#ifndef LCSMNHMILNH
#define PTSMMILNH   1      /* Portable LNH Interface */
#else
#ifndef SM
#define PTSMMILNH   1      /* Portable LNH Interface */
#endif /* SM */
#endif /* LCSMNHMILNH */

#ifdef PTSMMILNH
/* forward references */
PRIVATE S16 PtMiLnhCfgReq   (Pst *pst, NhMngmt *cfg );
PRIVATE S16 PtMiLnhCntrlReq (Pst *pst, NhMngmt *cntrl );
#endif /* PTSMMILNH */

/*WR_DIP*/
PRIVATE  U32 wrNhKwuSaps = 0;
PRIVATE  U32 wrNhPjuSaps = 0;

#ifdef WR_RSYS_OAM  
EXTERN S16 smStartTmr ARGS((PTR cb, S16 tmrEvnt,U32 delay));
EXTERN S16 smStopTmr ARGS((PTR cb, S16 tmrEvnt));
EXTERN Void smStaticCfgFailAlarm ARGS((Void));
#endif

PRIVATE LnhCfgReq SmMiLnhCfgReqMt[MAXNHMI] =
{
#ifdef LCSMNHMILNH
   cmPkLnhCfgReq,          /* 0 - loosely coupled */
#else
   PtMiLnhCfgReq,          /* 0 - tightly coupled, portable */
#endif /* LCSMNHMILNH */
#ifdef NH
   NhMiLnhCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLnhCfgReq,          /* 1 - tightly coupled, portable */
#endif /* NH */
};

PRIVATE LnhCntrlReq SmMiLnhCntrlReqMt[MAXNHMI] =
{
#ifdef LCSMNHMILNH
   cmPkLnhCntrlReq,          /* 0 - loosely coupled */
#else
   PtMiLnhCntrlReq,          /* 0 - tightly coupled, portable */
#endif /* LCSMNHMILNH */
#ifdef NH
   NhMiLnhCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLnhCntrlReq,          /* 1 - tightly coupled, portable */
#endif /* NH */
};

#ifdef __cplusplus
}
#endif /* cplusplus */

/*
*
*       Fun:   Initialize RRC task.
*
*       Desc:  Invoked to create RRC TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smNhInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smNhInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smNhInit);

   /* Register RRC TAPA Task */
   if (SRegTTsk((Ent)ENTNH, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR2,
            nhActvInit, (ActvTsk)nhActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Attach RRC TAPA Task */
   if (SAttachTTsk((Ent)ENTNH, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */
   RLOG1(L_DEBUG, "RRC Tapa Task successfully registered and attached to %d",
      sysTskId);

   RETVALUE(ROK);
} /* end of smNhInit */

/*
 *
 *       Fun:    wrSmNhCfg - configure NH
 *
 *       Desc:
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_sm_nh_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void wrSmNhCfg
(
Void
)
#else
PUBLIC Void wrSmNhCfg(Void)
#endif /* ANSI */
{

   TRC2(wrSmNhCfg)

 wrNhCfg = 0;
 numBinds = 0;
 bndInPrg = 1;
   wrNhKwuSaps = 0;
   wrNhPjuSaps = 0;

#ifdef WR_RSYS_OAM  
   smStartTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR, SM_TMR_STATIC_CFG_TMR_VAL);
#endif

   smBuildNhGenCfg();
   smBuildNhProtCfg();
   smBuildNhUsapCfg();
   smBuildNhLsapCfg(STNHKWUSAP, WR_SM_KWUL_INST);
   smBuildNhLsapCfg(STNHCKWSAP, WR_SM_KWUL_INST);
   smBuildNhLsapCfg(STNHCRGSAP, WR_SM_KWUL_INST);
   smBuildNhLsapCfg(STNHCPJSAP, WR_SM_KWUL_INST);
   smBuildNhLsapCfg(STNHPJUSAP, WR_SM_KWUL_INST);
   smBuildNhLsapCfg(STNHKWUSAP, WR_SM_KWDL_INST);
   smBuildNhLsapCfg(STNHPJUSAP, WR_SM_KWDL_INST);

   RETVOID;
} /* end of wrSmNhCfg */



/*
*
*       Fun:   smBuildNhGenCfg
*
*       Desc:  Invoked to build RRC GEN CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhGenCfg
(
)
#else
PUBLIC S16 smBuildNhGenCfg(Void)
#endif
{
   
   NhMngmt   nhMngmt;
   NhGenCfg  *genCfg = NULLP;
   Pst       pst;
   U32 idx;

   TRC2(smBuildNhGenCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(nhMngmt.u.cfg.u.gen);

   /*----------- Fill General Configuration Parameters ---------*/
   genCfg->maxCell      =  1;
   genCfg->msecTmrRes   = SM_DFLT_MSEC_TMR_CFG;   /* 10 Resolution i.e 1sec unit */
   genCfg->cfgTmr.enb = FALSE;
   genCfg->cfgTmr.val  = 60;
   genCfg->nmbPjuSaps = 2;
   genCfg->nmbKwuSaps = 2;
   genCfg->nmbCkwSaps = 1;
   genCfg->nmbCrgSaps = 1;
   genCfg->nmbCpjSaps = 1;
   genCfg->minTransLstLen = 2;
   genCfg->minMuiLstLen = 2;
   genCfg->portNumber = smCb.portNumber;
   for(idx=0; idx <16; idx++)
   {
      genCfg->ipAddress[idx]  = smCb.ipAddress[idx];
   }
   genCfg->rrcLogEnable = smCb.rrcLogEnable;
   genCfg->lmPst.dstProcId = SM_SM_PROC;
   genCfg->lmPst.srcProcId = SM_NH_PROC;
   genCfg->lmPst.dstEnt    = ENTSM;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTNH;
   genCfg->lmPst.srcInst   = 0;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = NH_MEM_REGION;
   genCfg->lmPst.pool      = NH_POOL;
   genCfg->lmPst.selector  = SM_SELECTOR_LC;
   

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.msgLen              = 0;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHGEN;
   nhMngmt.hdr.elmId.elmntInst1    = 0;
   nhMngmt.hdr.elmId.elmntInst2    = 0;
   nhMngmt.hdr.elmId.elmntInst3    = 0;
   nhMngmt.hdr.seqNmb              = 0;
   nhMngmt.hdr.version             = 0;
   nhMngmt.hdr.transId             = 0;
  
   nhMngmt.hdr.response.prior      = PRIOR0;
   nhMngmt.hdr.response.route      = RTESPEC;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;
   nhMngmt.hdr.response.selector   = SM_SELECTOR_LC;
   
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRC Gen Cfg Req sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBuildNhGenCfg */

/*
*
*       Fun:   smBuildNhProtCfg
*
*       Desc:  Invoked to build RRC Protocol CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhProtCfg
(
Void
)
#else
PUBLIC S16 smBuildNhProtCfg(Void)
#endif
{

   NhMngmt    nhMngmt;
   NhProtCfg  *protCfg = NULLP;
   Pst       pst;

   TRC2(smBuildNhProtCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protCfg   = &(nhMngmt.u.cfg.u.prot);

   /* Fill Prot Config*/
   protCfg->maxConn = 1000;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHPROT;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRC Prot Cfg Req sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhProtCfg */

/*
*
*       Fun:   smBuildNhUsapCfg
*
*       Desc:  Invoked to build USAP CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhUsapCfg
(
Void
)
#else
PUBLIC S16 smBuildNhUsapCfg(Void)
#endif
{

   NhMngmt    nhMngmt;
   NhUSapCfg  *uSap = NULLP;
   Pst       pst;

   TRC2(smBuildNhUsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   uSap   = &(nhMngmt.u.cfg.u.uSap);

   uSap->selector     = SM_SELECTOR_TC; /* Between RRC-NX Tight coupled */
   uSap->mem.region   = NH_MEM_REGION;
   uSap->mem.pool     = NH_POOL;
   uSap->prior        = PRIOR0;
   uSap->procId       = SM_NX_PROC;
   uSap->ent          = ENTNX;
   uSap->inst         = SM_INST_ZERO;
   uSap->route        = RTESPEC;
   uSap->spId         = SM_ZERO_VAL;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHUSAP;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "RRC Nhu USap Cfg Req sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhUsapCfg */


/*
*
*       Fun:   smBuildNhKwuLsapCfg
*
*       Desc:  Invoked to build LSAP KWU CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhKwuLsapCfg
(
Inst  inst
)
#else
PUBLIC S16 smBuildNhKwuLsapCfg(inst)
Inst inst;
#endif
{
   
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   TRC2(smBuildNhKwuLsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and RLC Loosely Coupled */
   lSap->mem.region   = NH_MEM_REGION;
   lSap->mem.pool     = NH_POOL;
   lSap->prior        = PRIOR0;
   lSap->procId       = SM_KW_PROC;
   lSap->ent          = ENTKW;
   lSap->inst         = inst;
   lSap->route        = RTESPEC;
   lSap->spId         = 0;
   lSap->suId         = (inst == WR_SM_KWUL_INST)? 
                        WR_SM_KWDL_INST: WR_SM_KWUL_INST;
   lSap->bndRetryCnt  = 0;
   lSap->bndTmr.enb   = TRUE;
   lSap->bndTmr.val   = 1000;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHKWUSAP;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "RRC Kwu LSap Cfg Req sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhKwuLsapCfg */

/*
*
*       Fun:   smBuildNhCkwLsapCfg
*
*       Desc:  Invoked to build LSAP CKW CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhCkwLsapCfg
(
Void
)
#else
PUBLIC S16 smBuildNhCkwLsapCfg(Void)
#endif
{
   
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   TRC2(smBuildNhCkwLsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and RLC Loosely Coupled */
   lSap->mem.region   = NH_MEM_REGION;
   lSap->mem.pool     = NH_POOL;
   lSap->prior        = PRIOR0;
   lSap->procId       = SM_KW_PROC;
   lSap->ent          = ENTKW;
   lSap->inst         = SM_INST_ZERO;
   lSap->route        = RTESPEC;
   lSap->spId         = 0;
   lSap->suId         = SM_ZERO_VAL;
   lSap->bndRetryCnt  = 0;
   lSap->bndTmr.enb   = TRUE;
   lSap->bndTmr.val   = 1000;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCKWSAP;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "RRC Ckw LSap Cfg Req sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhCkwLsapCfg */

/*
*
*       Fun:   smBuildNhCrgLsapCfg
*
*       Desc:  Invoked to build LSAP CRG CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhCrgLsapCfg
(
Void
)
#else
PUBLIC S16 smBuildNhCrgLsapCfg(Void)
#endif
{
   
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   TRC2(smBuildNhCrgLsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and MAC Loosely Coupled */
   lSap->mem.region   = NH_MEM_REGION;
   lSap->mem.pool     = NH_POOL;
   lSap->prior        = PRIOR0;
   lSap->procId       = SM_RG_PROC;
   lSap->ent          = ENTRG;
   lSap->inst         = SM_INST_ZERO;
   lSap->route        = RTESPEC;
   lSap->spId         = 0;
   lSap->suId         = 0;
   lSap->bndRetryCnt  = 0;
   lSap->bndTmr.enb   = TRUE;
   lSap->bndTmr.val   = 1000;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCRGSAP;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "RRC Crg LSap Cfg Req sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* end of smBuildNhCrgLsapCfg */

/*
*
*       Fun:   smBuildNhCpjLsapCfg
*
*       Desc:  Invoked to build LSAP CPJ CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhCpjLsapCfg
(
Void
)
#else
PUBLIC S16 smBuildNhCpjLsapCfg(Void)
#endif
{
   
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   TRC2(smBuildNhCpjLsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and PDCP Loosely Coupled */
   lSap->mem.region   = NH_MEM_REGION;
   lSap->mem.pool     = NH_POOL;
   lSap->prior        = PRIOR0;
   lSap->procId       = SM_PJ_PROC;
   lSap->ent          = ENTPJ;
   lSap->inst         = SM_INST_ZERO;
   lSap->route        = RTESPEC;
   lSap->spId         = 0;
   lSap->suId         = SM_ZERO_VAL;
   lSap->bndRetryCnt  = 0;
   lSap->bndTmr.enb   = TRUE;
   lSap->bndTmr.val   = 100;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCPJSAP;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "RRC Cpj LSap Cfg Req sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhCpjLsapCfg */

/*
*
*       Fun:   smBuildNhPjuLsapCfg
*
*       Desc:  Invoked to build LSAP PJU CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhPjuLsapCfg
(
Inst inst
)
#else
PUBLIC S16 smBuildNhPjuLsapCfg(inst)
Inst inst;
#endif
{
   
   NhLSapCfg  *lSap=NULLP;
   NhMngmt    nhMngmt;
   Pst        pst;

   TRC2(smBuildNhPjuLsapCfg)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   lSap   = &(nhMngmt.u.cfg.u.lSap);

   lSap->selector     = SM_SELECTOR_LC; /* RRC and RLC Loosely Coupled */
   lSap->mem.region   = NH_MEM_REGION;
   lSap->mem.pool     = NH_POOL;
   lSap->prior        = PRIOR0;
   lSap->procId       = SM_PJ_PROC;
   lSap->ent          = ENTPJ;
   lSap->inst         = inst;
   lSap->route        = RTESPEC;
   lSap->spId         = 0;   /* Vamsee */
   lSap->suId         = (inst == WR_SM_KWUL_INST) ? 
                         WR_SM_KWDL_INST : WR_SM_KWUL_INST;
   lSap->bndRetryCnt  = 0;
   lSap->bndTmr.enb   = TRUE;
   lSap->bndTmr.val   = 100;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCFG;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHPJUSAP;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "RRC Pju LSap Cfg Req sent");

   /* Send the request to the RRC */
   SmMiLnhCfgReq(&pst, &nhMngmt);
   
   RETVALUE(ROK);
} /* smBuildNhPjuLsapCfg */

/*
*
*       Fun:   smBuildNhLsapCfg
*
*       Desc:  Invoked to build LSAP CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhLsapCfg
(
U8  elemId,
Inst inst
)
#else
PUBLIC S16 smBuildNhLsapCfg (elemId,inst)
U8  elemId;
Inst inst;
#endif
{
   S16 ret = ROK;

   TRC2(smBuildNhLsapCfg)

   switch (elemId)
   {
      case STNHKWUSAP:
      {
         smBuildNhKwuLsapCfg(inst);
         break;
      }
      case STNHCKWSAP:
      {
         smBuildNhCkwLsapCfg();
         break;
      }
      case STNHCRGSAP:
      {
         smBuildNhCrgLsapCfg();
         break;
      }
      case STNHCPJSAP:
      {
         smBuildNhCpjLsapCfg();
         break;
      }
      case STNHPJUSAP:
      {
         smBuildNhPjuLsapCfg(inst);
         break;
      }
      default:
      {
         RLOG0(L_ERROR, "smBindNhToLsap: Invalid element");
         ret = RFAILED;
         break;
      }
   
   } /* end of switch */
   
   RETVALUE(ret);
} /* end of smBuildNhLsapCfg */


/*
*
*       Fun:   smBindUnBindNhToKwuSap
*
*       Desc:  Invoked to bind and unbind RRC to KWU Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrc.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindNhToKwuSap
(
Inst inst,
U8 action
)
#else
PUBLIC S16 smBindUnBindNhToKwuSap(inst,action)
Inst inst;
U8   action;
#endif
{

   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   
   TRC2(smBindUnBindNhToKwuSap)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  action;
   cntrl->u.sap.sapId   =  inst;
   cntrl->subAction     =  SM_ZERO_VAL;


   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHKWUSAP;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   if (action == ABND_ENA)
   {
   RLOG0(L_DEBUG, "RRC Cntrl Req to nh for Kwu Bind");
   }
   else
   {
      RLOG0(L_DEBUG, "RRC Cntrl Req to nh for Kwu UnBind");
   }

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);

} /* smBindUnBindNhToKwuSap */


/*
*
*       Fun:   smBindUnBindNhToCkwSap
*
*       Desc:  Invoked to bind and unbind RRC to CKW Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrc.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindNhToCkwSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindNhToCkwSap(U8 action)
#endif
{

   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;

   TRC2(smBindUnBindNhToCkwSap)
   
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  action;
   cntrl->u.sap.sapId   =  0;
   cntrl->subAction     =  SM_ZERO_VAL;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCKWSAP;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   if (action == ABND_ENA)
   {
   RLOG0(L_DEBUG, "RRC Cntrl Req to nh for Ckw Bind");
   }
   else
   {
      RLOG0(L_DEBUG, "RRC Cntrl Req to nh for Ckw UnBind");
   }

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBindUnBindNhToCkwSap */


/*
*
*       Fun:   smBindUnBindNhToCrgSap
*
*       Desc:  Invoked to bind and Unbind RRC to CRG Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrc.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindNhToCrgSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindNhToCrgSap(U8 action)
#endif
{

   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;

   TRC2(smBindUnBindNhToCrgSap)
   
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  action;
   cntrl->u.sap.sapId   =  0;
   cntrl->subAction     =  SM_ZERO_VAL;


   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCRGSAP;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   if (action == ABND_ENA)
   {
   RLOG0(L_DEBUG, "RRC Cntrl Req to nh for Crg Bind");
   }
   else
   {
      RLOG0(L_DEBUG, "RRC Cntrl Req to nh for Crg UnBind");
   }

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBindUnBindNhToCrgSap */


/*
*
*       Fun:   smBindUnBindNhToCpjSap
*
*       Desc:  Invoked to bind and Unbind RRC to CPJ Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrc.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindNhToCpjSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindNhToCpjSap(U8 action)
#endif
{

   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   
   TRC2(smBindUnBindNhToCpjSap)
   
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  action;
   cntrl->u.sap.sapId   =  0;
   cntrl->subAction     =  SM_ZERO_VAL;


   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHCPJSAP;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   if (action == ABND_ENA)
   {
   RLOG0(L_DEBUG, "RRC Cntrl Req to nh for Cpj Bind");
   }
   else
   {
      RLOG0(L_DEBUG, "RRC Cntrl Req to nh for Cpj UnBind");
   }

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBindUnBindNhToCpjSap */


/*
*
*       Fun:   smBindUnBindNhToPjuSap
*
*       Desc:  Invoked to bind and unbind RRC to PJU Sap
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_rrc.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindUnBindNhToPjuSap
(
Inst inst,
U8 action
)
#else
PUBLIC S16 smBindUnBindNhToPjuSap(inst,action)
Inst inst;
U8 action;
#endif
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   
   TRC2(smBindUnBindNhToPjuSap)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  action;
   cntrl->u.sap.sapId   =  inst;
   cntrl->subAction     =  SM_ZERO_VAL;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHPJUSAP;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   if (action == ABND_ENA)
   {
   RLOG0(L_DEBUG, "RRC Cntrl Req to nh for Pju Bind");
   }
   else
   {
      RLOG0(L_DEBUG, "RRC Cntrl Req to nh for Pju UnBind");
   }

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBindUnBindNhToPjuSap */

/*
*
*       Fun:   smBindNhToLsap
*
*       Desc:  Invoked to bind RRC to LSAP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBindNhToLsap
(
U8  elemId
)
#else
PUBLIC S16 smBindNhToLsap (elemId)
U8  elemId;
#endif
{
   S16   ret = ROK;

   TRC2(smBindNhToLsap)

   switch (elemId)
   {
      case STNHKWUSAP:
         smBindUnBindNhToKwuSap(0,ABND_ENA);
         break;

      case STNHCKWSAP:
         smBindUnBindNhToCkwSap(ABND_ENA);
         break;

      case STNHCRGSAP:
         smBindUnBindNhToCrgSap(ABND_ENA);
         break;

      case STNHCPJSAP:
         smBindUnBindNhToCpjSap(ABND_ENA);
         break;

      case STNHPJUSAP:
         smBindUnBindNhToPjuSap(0,ABND_ENA);
         break;

      default:
         ret = RFAILED;
         RLOG0(L_ERROR, "smBindNhToLsap: Invalid element");
         break;
   } /* end of switch statement */

   RETVALUE(ret);
} /* end of smBindNhToLsap */

/*
*
*       Fun:   smBuildNhGenCntrl
*
*       Desc:  Invoked to Enable Alarms at RRC
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhGenCntrl
(
Void
)
#else
PUBLIC S16 smBuildNhGenCntrl(Void)
#endif
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
  
   TRC2(smBuildNhGenCntrl)
 
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  AENA;
   cntrl->u.sap.sapId   =  0;
   cntrl->subAction     =  SAUSTA;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHGEN;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "RRC Gen Cntrl Req to nh");

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBuildNhGenCntrl */
 
/**
 * @brief This function is used to send enable/disable control request to 
 * the RRC layers.
 *
 * @details
 *
 * Function: smBuildNhLogCntrl
 *
 * @param[in] actType
 * @param[in] mask
 *
 * @return Void
 */

#ifdef ANSI
PUBLIC S16 smBuildNhLogCntrl
(
 Bool actType,
 U32  mask
)
#else
PUBLIC S16 smBuildNhLogCntrl(actType, mask)
 Bool actType;
 U32  mask;
#endif
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;

   TRC2(smBuildNhLogCntrl)

   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   if(actType == WR_ON)
   {
      cntrl->action        =  AENA;
   }
   else
   {
      cntrl->action        =  ADISIMM;
   }
   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHGEN;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBuildNhLogCntrl */
 
/*
*
*       Fun:   smBuildNhDbgCntrl
*
*       Desc:  Invoked to Enable debugs at RRC
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhDbgCntrl
(
Void
)
#else
PUBLIC S16 smBuildNhDbgCntrl(Void)
#endif
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
   
   TRC2(smBuildNhDbgCntrl)
 
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  AENA;
   cntrl->subAction     =  SADBG;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHGEN;
   nhMngmt.hdr.response.mem.region = NH_MEM_REGION;
   nhMngmt.hdr.response.mem.pool   = NH_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;
   
   RLOG0(L_DEBUG, "RRC Dbg Cntrl Req to nh");

   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBuildNhDbgCntrl */

/* IPSec Recovery */

/*
*
*       Fun:   smBuildNhShutDownCntrl
*
*       Desc:  Invoked to shut down RRC
*
*       Ret:   ROK
*       Notes: None
*
*       File:  wr_sm_nh_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNhShutDownCntrl
(
Void
)
#else
PUBLIC S16 smBuildNhShutDownCntrl(Void)
#endif
{
   NhCntrl *cntrl = NULLP;
   NhMngmt  nhMngmt;
   Pst      pst;
  
   TRC2(smBuildNhShutDownCntrl)
 
   SM_SET_ZERO(&nhMngmt, sizeof(NhMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nhMngmt.u.cntrl);

   cntrl->action        =  ASHUTDOWN;
   /* cntrl->u.sap.sapId   =  0; */ /* Needed? */
   cntrl->subAction     =  NULLD;

   /* Fill Header */
   nhMngmt.hdr.msgType             = TCNTRL;
   nhMngmt.hdr.entId.ent           = ENTNH;
   nhMngmt.hdr.entId.inst          = SM_INST_ZERO;
   nhMngmt.hdr.elmId.elmnt         = STNHGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNH;
   pst.dstProcId = SM_NH_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "RRC shut down Cntrl Req to nh");
   /* Send the request to the RRC */
   SmMiLnhCntrlReq(&pst, &nhMngmt);

   RETVALUE(ROK);
} /* end of smBuildNhShutDownCntrl */
 
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
*       File:  smnhexms.c
*
*/

#ifdef ANSI
PUBLIC S16 smNhActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smNhActvInit(ent, inst, region, reason)
Ent ent;                      /* entity */
Inst inst;                    /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
{
   TRC3(smNhActvInit)

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
*       Desc:   Processes received event from FP User layer
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   smrrexms.c
*
*/

#ifdef ANSI
PUBLIC S16 smNhActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smNhActvTsk(pst, mBuf)
Pst *pst;                   /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16 ret;

   TRC3(smNhActvTsk)

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCSMNHMILNH
      case EVTLNHCFGCFM:             /* Config confirm */
         ret = cmUnpkLnhCfgCfm(SmMiLnhCfgCfm, pst, mBuf);
         break;
      case EVTLNHCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLnhCntrlCfm(SmMiLnhCntrlCfm, pst, mBuf);
         break;
      case EVTLNHSTAIND:           /* Status Ind */
         ret = cmUnpkLnhStaInd(SmMiLnhStaInd, pst, mBuf);
         break;
#endif
      default:
         WR_FREEMBUF(mBuf);
         ret = RFAILED;
         RLOG1(L_ERROR, "Invalid event [%d] received", pst->event);
         break;
   }

   RETVALUE(ret);

} /* end of smNhActvTsk */

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
 *      File:  wr_sm_nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 SmMiLnhCfgReq
(
Pst *pst,                 /* post structure */
NhMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLnhCfgReq(pst, cfg)
Pst *pst;                 /* post structure */   
NhMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLnhCfgReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLnhCfgReqMt[pst->selector])(pst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLnhCfgReq */


/**************************************************************************
 *
 *      Fun:   Control request
 *
 *      Desc:  This function is used to send the control request RRC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_sm_nh_ptmi.c
 *
 *************************************************************************/
 
#ifdef ANSI
PUBLIC S16 SmMiLnhCntrlReq
(
Pst *pst,                 /* post structure */
NhMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLnhCntrlReq(pst, cntrl)
Pst *pst;                 /* post structure */   
NhMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLnhCntrlReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLnhCntrlReqMt[pst->selector])(pst, cntrl); 
   RETVALUE(ROK);
} /* end of SmMiLnhCntrlReq */

#ifdef PTSMMILNH
/**************************************************************************
 *
 *      Fun:   Portable Configuration request
 *
 *      Desc:  This function is used to send the configuration request RRC
 *             (Portable)
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_sm_nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PRIVATE S16 PtMiLnhCfgReq
(
Pst *pst,                   /* post structure */
NhMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLnhCfgReq(pst, cfg)
Pst *pst;                   /* post structure */
NhMngmt *cfg;               /* configure */
#endif
{
  TRC3(PtMiLnhCfgReq);

  UNUSED(cfg);
  UNUSED(pst);
  RETVALUE(ROK);
} /* end of PtMiLnhCfgReq */


/**************************************************************************
 *
 *      Fun:   Portable Control request
 *
 *      Desc:  This function is used to send the control request RRC
 *             (Portable)
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_sm_nh_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtMiLnhCntrlReq
(
Pst *pst,                   /* post structure */
NhMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLnhCntrlReq(pst, cntrl)
Pst *pst;                   /* post structure */
NhMngmt *cntrl;             /* control */
#endif
{
  TRC3(PtMiLnhCntrlReq);

  UNUSED(cntrl);
  UNUSED(pst);

  RETVALUE(ROK);
} /* end of PtMiLnhCntrlReq */

#endif /* PTSMMILNH */


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
*       File:  wr_sm_nh_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLnhCfgCfm
(
Pst     *pst,          /* post structure */
NhMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLnhCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
NhMngmt *cfm;          /* configuration */
#endif
{
  Pst      nhPst;

   TRC2(SmMiLnhCfgCfm);

   SM_SET_ZERO(&nhPst,  sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
       switch (cfm->hdr.elmId.elmnt)
       {
          case STNHGEN:
               wrNhCfg |= WR_NH_GEN_CFG;
               break;      
          case STNHPROT:
               wrNhCfg |= WR_NH_PROT_CFG;
               break;
          case STNHUSAP:
               wrNhCfg |= WR_NH_USAP_CFG;
               break;
          case STNHCKWSAP:
               wrNhCfg |= WR_NH_CKW_SAP_CFG;
               break;
          case STNHKWUSAP:
               wrNhCfg |= WR_NH_KWU_SAP_CFG;
               wrNhKwuSaps++;
               break;
          case STNHCRGSAP:
               wrNhCfg |= WR_NH_CRG_SAP_CFG;
               break;
          case STNHCPJSAP:
               wrNhCfg |= WR_NH_CPJ_SAP_CFG;
               break;
          case STNHPJUSAP:
               wrNhCfg |= WR_NH_PJU_SAP_CFG;
               wrNhPjuSaps++;
               break;
          default:
               RLOG0(L_ERROR, "RRC Cfg Cfm received with invalid element");
               break;
       } /* end of switch statement */
       RLOG1(L_DEBUG, "RRC Cfg Cfm received for the element %d",
          cfm->hdr.elmId.elmnt);

       if ((wrNhCfg == WR_SM_NH_CONFIGURED)
           && (wrNhKwuSaps == 2) && (wrNhPjuSaps == 2) 
          )
       {
          smCb.smState = WR_SM_STATE_RRC_CFG_DONE;
          RLOG0(L_ALWAYS, "RRC CONFIGURATION SUCCESSFULLY PERFORMED.");
#ifdef WR_RSYS_OAM  
          smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
#endif
          WR_FILL_PST(nhPst, SM_NH_PROC, ENTSM, ENTWR, EVTWRPDCPCFG, NH_POOL, SM_SELECTOR_LC);

          if(wrSmSelfPstMsg(&nhPst) != ROK)
          {
             RLOG0(L_FATAL, "Self Pst failed after RRC Configuration.");
          }
          else
          {
             RLOG0(L_DEBUG, "Self Pst success after RRC Configuration.");
          } /* end of else part */

       }  /* end of wrNhCfg == WR_SM_NH_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
      RLOG0(L_ERROR, "Configuration Confirm not OK ofr RRC.");
#ifdef WR_RSYS_OAM  
      /*timer stop :: Raised an alarm */
      smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
      smCb.cfgCb.alarm.severity  = SM_ALARM_SEVERITY_MAJOR;
      smCb.cfgCb.alarm.causeType = SM_ALARM_CAUSE_TYPE_ERR_IND;
      smCb.cfgCb.alarm.causeVal  = SM_ALARM_CAUSE_VAL_INIT_FAIL;
      smStaticCfgFailAlarm();
#endif
   } /* end of else part */

   RETVALUE(ROK);
} /* end of SmMiLnhCfgCfm */

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
*       File:  wr_sm_nh_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnhCntrlCfm
(
Pst     *pst,          /* post structure */
NhMngmt *cntrl         /* control */
)
#else
PUBLIC S16 SmMiLnhCntrlCfm(pst, cntrl)
Pst     *pst;          /* post structure */
NhMngmt *cntrl;        /* control */
#endif
{
   Pst wrPst;

   /* wr002.102: Added trace macro */
   TRC2(SmMiLnhCntrlCfm)

   SM_SET_ZERO(&wrPst, sizeof(Pst));
   /* IPSec */ 
   if(cntrl->cfm.status == LCM_PRIM_OK)
   {
//      if (smCb.smState == WR_SM_STATE_APP_SHUTDWN_DONE)
      if (smCb.smState == WR_SM_STATE_SON_SHUTDWN_DONE)
      {
         smCb.smState = WR_SM_STATE_RRC_SHUTDWN_DONE;
         RLOG0(L_ALWAYS, "RRC SHUTDOWN SUCCESSFULLY PERFORMED.");
         WR_FILL_PST(wrPst, SM_NH_PROC, ENTSM, ENTWR, EVTWRPDCPSHUT, NH_POOL, SM_SELECTOR_LC);

         if(wrSmSelfPstMsg(&wrPst) != ROK)
         {
            RLOG0(L_FATAL, "Self Pst failed after RRC Shutdown Configuration.");
         }
         else
         {
            RLOG0(L_DEBUG, "Self Pst success after RRC Shutdown Configuration.");
         } /* end of else part */
         RETVALUE(ROK);
      }
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */


   if(cntrl->cfm.status == LCM_PRIM_OK)
   {
      switch (cntrl->hdr.elmId.elmnt)
      {
         case STNHGEN:
            {
               RLOG0(L_INFO, "Enabling RRC Alarms is successfully performed.");
               if((smCfgCb.nhDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_NH_DBG_MASK))
               {
                  /* Update DbgMsk */
                  smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | WR_SM_NH_DBG_MASK);
                  /* wr001.102: Trigger Debug Cntrl */
                  wrSmHdlEnbDebugs(EVTWRENBDEBUGS);
               }
            }
            break;
         case STNHCRGSAP:
            {
               /* Due to unbind */
              if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
              {  
                 if (cntrl->cfm.status == LCM_PRIM_OK)
                 {
                    RLOG0(L_DEBUG,"UNBIND OF RRC WITH MAC-control SUCCESSFUL ");
                    /* Unbind RRC to PDCP UL */
                    smBindUnBindNhToPjuSap(WR_SM_PJUL_INST,AUBND);
                 }
              }
            }
            break;
         case STNHCPJSAP:
            {
               /* Due to unbind */
              if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
              {  
                 if (cntrl->cfm.status == LCM_PRIM_OK)
                 {
                    RLOG0(L_DEBUG,"UNBIND OF RRC WITH PDCP-control SUCCESSFUL ");
                    /* Unbind RRC to RLC-control */
                    smBindUnBindNhToCkwSap(AUBND);
                 }
              }
            }
            break;
         case STNHPJUSAP:
            {
               /* Due to unbind */
              if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
              {  
                 if (cntrl->cfm.status == LCM_PRIM_OK)
                 {
                    if (pst->srcInst == WR_SM_PJDL_INST)
                    {
                       RLOG0(L_DEBUG,"UNBIND OF RRC WITH PDCP DL SUCCESSFUL ");
                       /* Unbind RRC to RLC UL */
                       smBindUnBindNhToKwuSap(WR_SM_KWUL_INST,AUBND);
                    }
                    else if (pst->srcInst == WR_SM_PJUL_INST)
                    {
                       RLOG0(L_DEBUG,"UNBIND OF RRC WITH PDCP UL SUCCESSFUL ");
                       /* Unbind RRC to PDCP-control */
                       smBindUnBindNhToCpjSap(AUBND);
                    }
                 }
              }
            }
            break;
            /* wr003.102: Fix for CRId- 115717 */
            /* Handling cntrlCfm for RRC CKW SAP */
         case STNHCKWSAP:
            {
               /* Due to unbind */
              if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
              {  
                 if (cntrl->cfm.status == LCM_PRIM_OK)
                 {
                    RLOG0(L_DEBUG,"UNBIND OF RRC WITH RLC-control SUCCESSFUL ");
                    /* Unbind PDCP DL to RLC DL */
                    smBindUnBindPjKwuSap(WR_SM_PJDL_INST,AUBND);
                 }
              }
            }
            break;
         case STNHKWUSAP:
            {
               /* Due to unbind */
              if (smCb.smState == WR_SM_STATE_RRM_STK_UNBND_DONE)
              {  
                 if (cntrl->cfm.status == LCM_PRIM_OK)
                 {
                    if (pst->srcInst == WR_SM_KWDL_INST)
                    {
                       RLOG0(L_DEBUG,"UNBIND OF RRC WITH RLC DL SUCCESSFUL ");
                       /* Unbind RRC to PDCP DL */
                       smBindUnBindNhToPjuSap(WR_SM_PJDL_INST,AUBND);
                    }
                    else if (pst->srcInst == WR_SM_KWUL_INST)
                    {
                       RLOG0(L_DEBUG,"UNBIND OF RRC WITH RLC UL SUCCESSFUL ");
                       /* Unbind RRC to MAC control */
                       smBindUnBindNhToCrgSap(AUBND);
                    }
                 }
              }
            }
            break;

         default:
            {
               RLOG1(L_ERROR, "Unknown CntrlCfm %d", cntrl->hdr.elmId.elmnt);
            }
            break;
      }
   } /* end of if cntrl->cfm.status == LCM_PRIM_OK */

   RETVALUE(ROK);
} /* end of nhAcSmMiLnhCntrlCfm */

/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used by to present status confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_sm_nh_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnhStaCfm
(
Pst     *pst,          /* post structure */
NhMngmt *cfm           /* control */
)
#else
PUBLIC S16 SmMiLnhStaCfm(pst, cfm)
Pst     *pst;          /* post structure */
NhMngmt *cfm;          /* control */
#endif
{
   TRC2(SmMiLnhStaCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
} /* end of SmMiLnhStaCfm */

/*
*
*       Fun:   Sts Confirm
*
*       Desc:  This function is used by to present sts confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_sm_nh_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnhStsCfm
(
Pst       *pst,         /* post structure */
NhMngmt   *sts          /* confirmed statistics */
)
#else
PUBLIC S16 SmMiLnhStsCfm(pst, sts)
Pst       *pst;         /* post structure */
NhMngmt   *sts;         /* confirmed statistics */
#endif
{
   TRC2(SmMiLnhStsCfm)

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of SmMiLnhStsCfm */

/*
*
*       Fun:   Status Indication
*
*       Desc:  This function is used by to present status Indication
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_sm_nh_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnhStaInd
(
Pst     *pst,           /* post structure */
NhMngmt *usta           /* unsolicited status */
)
#else
PUBLIC S16 SmMiLnhStaInd(pst, usta)
Pst     *pst;           /* post structure */
NhMngmt *usta;          /* unsolicited status */
#endif
{
   NhUstaDgn *dgn;

   TRC2(SmMiLnhStaInd)

   dgn = &usta->u.usta.dgn;

   switch(usta->u.usta.alarm.category)
   {
      case LCM_CATEGORY_INTERFACE:
         {
            switch(usta->u.usta.alarm.event)
            {
               case LCM_EVENT_BND_OK:
                  {
                     if (bndInPrg)
                     {
                        numBinds ++;
                     }
                     else
                     {
                        RLOG1(L_DEBUG, "BIND OF RRC (NHU) Usap id[%d] IS "
                           "SUCCESSFUL", dgn->u.sapInfo.sapId);
                        break;
                     }
                     switch (numBinds)
                     {
                        case 1:
                        {
                           RLOG1(L_DEBUG, "BIND OF RRC CKW lsap id[%d] IS "
                              "SUCCESSFUL", dgn->u.sapInfo.sapId);
                           smBindUnBindNhToCpjSap(ABND_ENA); 
                          break;
                        }
                        case 2:
                        {
                           RLOG1(L_DEBUG, "BIND OF RRC CPJ lsap id[%d] IS "
                              "SUCCESSFUL", dgn->u.sapInfo.sapId);
                           smBindUnBindNhToPjuSap(WR_SM_PJUL_INST,ABND_ENA);
                          break;
                        }
                        case 3:
                        {
                           RLOG1(L_DEBUG, "BIND OF RRC PJU lsap id[%d] IS "
                              "SUCCESSFUL", dgn->u.sapInfo.sapId);
                           smBindUnBindNhToCrgSap(ABND_ENA); 
                          break;
                        }
                        case 4:
                        {
                           RLOG1(L_DEBUG, "BIND OF RRC CRG lsap id[%d] IS "
                              "SUCCESSFUL", dgn->u.sapInfo.sapId);
                           /* Initiate Bind request for KWU Sap btw NH and KW */
                           smBindUnBindNhToKwuSap(WR_SM_KWUL_INST,ABND_ENA); 
                           
                          break;
                        }
                        case 5:
                        {
                           RLOG1(L_DEBUG, "BIND OF RRC CPJ lsap id[%d] IS "
                              "SUCCESSFUL", dgn->u.sapInfo.sapId);
                           
                           smBindUnBindNhToPjuSap(WR_SM_PJDL_INST,ABND_ENA); 
                          break;
                        }
                        case 6:
                        {
                           RLOG1(L_DEBUG, "BIND OF RRC CRG lsap id[%d] IS "
                              "SUCCESSFUL", dgn->u.sapInfo.sapId);
                           /* Initiate Bind request for KWU Sap btw NH and KW */
                           smBindUnBindNhToKwuSap(WR_SM_KWDL_INST,ABND_ENA); 
                           
                          break;
                        }
                        case 7:
                        {
                           RLOG1(L_DEBUG, "BIND OF RRC KWU lsap id[%d] IS "
                              "SUCCESSFUL", dgn->u.sapInfo.sapId);
                           bndInPrg = FALSE;
                          smBindUnBindWrToCtfSap(ABND_ENA);  
                         break;
                        }
                        default:
                          RLOG1(L_ERROR, "Multiple Alarms received %d",
                             dgn->u.sapInfo.sapId);
                          break;
                     }
                  }
                  break;
               case LCM_EVENT_BND_FAIL:
                  {
                     RLOG1(L_FATAL, "BIND OF RRC lsap id[%d] FAILED",
                        dgn->u.sapInfo.sapId);
                  }
                  break;
               default:
                  {
                  }
            }
         }
         break;
      default:
         {
         }
   }

   RETVALUE(ROK);
} /* end of SmMiLnhStaInd */

/********************************************************************30**

           End of file:     fc_smm_rrc_brcm.c@@/main/TeNB_Main_BR/3 - Mon Aug 11 16:52:26 2014

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
