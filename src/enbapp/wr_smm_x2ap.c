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
               

     File:     wr_smm_x2ap.c

     Sid:      fc_smm_x2ap_brcm.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:52:37 2014

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_FILE_ID=202;


static int RLOG_MODULE_ID=2;

#include "wr.h"
#include "wr_cmn.h"
#include "sct.h"
#include "lcz.h"
#include "lcz.x"
#include "wr_lwr.h"
#include "wr_smm_init.h"
#include "wr_smm_smallcell.h"
#include "wr_utils.h"

PRIVATE U8 wrCzCfg = 0;

#ifdef WR_RSYS_OAM  
EXTERN S16 smStartTmr ARGS((PTR cb, S16 tmrEvnt,U32 delay));
EXTERN S16 smStopTmr ARGS((PTR cb, S16 tmrEvnt));
EXTERN Void smStaticCfgFailAlarm ARGS((Void));
#endif

/*
*
*       Fun:   Initialize X2AP task.
*
*       Desc:  Invoked to create X2AP Tapa task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smCzInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smCzInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smCzInit)
 
   /* Register X2AP TAPA Task */
   if (SRegTTsk((Ent)ENTCZ, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
                 czActvInit, (ActvTsk)czActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   }

   /* Attach X2AP TAPA Task */
   if (SAttachTTsk((Ent)ENTCZ, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }

   RLOG1(L_DEBUG, "X2AP Tapa Task successfully registered and attached to %d",
      sysTskId);

   RETVALUE(ROK);
} /* end of smCzInit */

/*
 *
 *       Fun:    wrSmCzCfg - configure CZ 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_sm_cz_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void wrSmCzCfg
(
Void
)
#else
PUBLIC Void wrSmCzCfg()
#endif /* ANSI */
{
   TRC2(wrSmCzCfg)
   wrCzCfg = 0;

#ifdef WR_RSYS_OAM  
   smStartTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR, SM_TMR_STATIC_CFG_TMR_VAL);
#endif
   smBuildCzGenCfg();
   smBuildCzProtCfg();
   smBuildCzLSapCfg(CZ_SB_SUID,CZ_SB_SPID);
   smBuildCzUSapCfg(WR_CZ_SPID);
#ifndef WR_RSYS_OAM
   smBuildCzPeerCfg();
#endif

   RETVOID;
} /* end of wrSmCzCfg */


/*
*
*       Fun:   smBuildCzGenCfg
*
*       Desc:  fill in genCfg for CZ
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_cz_cntrl.c 
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzGenCfg
(
Void
)
#else
PUBLIC S16 smBuildCzGenCfg()
#endif
{
   /* ccpu00129256 */
   CzMngmt  *czMgt = NULLP;
   CzGenCfg    *cfg = NULLP;
   Pst       pst;

   TRC2(smBuildCzGenCfg)

   WRSM_ALLOC(&czMgt,sizeof(CzMngmt));
   if(NULLP == czMgt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(NULLP != czMgt)
   {
   cfg   = &(czMgt->t.cfg.u.gen);
   /*----------- Fill General Configuration Parameters ---------*/
   cfg->nmbCztSaps = (S16)WRSM_LCZ_MAX_CZTSAP;
   cfg->nmbSctSaps = (S16)WRSM_LCZ_MAX_SCTSAP;
#if 1
   cfg->memUpperThr = DFLT_CZ_RES_THR_UP;
   cfg->memLowerThr = DFLT_CZ_RES_THR_LOW;
#endif
   cfg->maxNmbPeer = (U32) smCfgCb.maxX2Peers;
   cfg->timeRes = (Ticks)WRSM_LCZ_TIMERES; /* resolution 10 i.e 1sec unit */

   cfg->lmPst.srcProcId = SM_CZ_PROC;
   cfg->lmPst.dstProcId = SM_SM_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTCZ;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)WRSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = CZ_MEM_REGION;
   cfg->lmPst.pool =  CZ_POOL;
   cfg->lmPst.selector = (Selector)WRSM_CZSMSEL;

   /* Fill Header */
   czMgt->hdr.msgType             = TCFG;
   czMgt->hdr.msgLen              = 0;
   czMgt->hdr.entId.ent           = ENTCZ;
   czMgt->hdr.entId.inst          = SM_INST_ZERO;
   czMgt->hdr.elmId.elmnt         = STCZGEN;
   czMgt->hdr.elmId.elmntInst1    = 0;
   czMgt->hdr.elmId.elmntInst2    = 0;
   czMgt->hdr.elmId.elmntInst3    = 0;
   czMgt->hdr.seqNmb              = 0;
   czMgt->hdr.version             = 0;
   czMgt->hdr.transId             = 0;

   czMgt->hdr.response.prior      = PRIOR0;
   czMgt->hdr.response.route      = RTESPEC;
   czMgt->hdr.response.mem.region = DFLT_REGION;
   czMgt->hdr.response.mem.pool   = CZ_POOL;
   czMgt->hdr.response.selector   = SM_SELECTOR_LC;
   czMgt->hdr.msgType             = TCFG;
   czMgt->hdr.msgLen              = 0;
   czMgt->hdr.entId.ent           = ENTCZ;
   czMgt->hdr.entId.inst          = SM_INST_ZERO;
   czMgt->hdr.elmId.elmnt         = STCZGEN;
   czMgt->hdr.elmId.elmntInst1    = 0;
   czMgt->hdr.elmId.elmntInst2    = 0;
   czMgt->hdr.elmId.elmntInst3    = 0;
   czMgt->hdr.seqNmb              = 0;
   czMgt->hdr.version             = 0;
   czMgt->hdr.transId             = 0;

   czMgt->hdr.response.prior      = PRIOR0;
   czMgt->hdr.response.route      = RTESPEC;
   czMgt->hdr.response.mem.region = CZ_MEM_REGION;

    /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;

   RLOG0(L_DEBUG, "X2AP Gen Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, czMgt);
}
else
{
      RLOG0(L_FATAL, "Memory allocation failed.");
}
   RETVALUE(ROK);
}/* smBuildCzGenCfg */

/*
*
*       Fun:   smBuildCzLSapCfg
*
*       Desc:  Call handler for TSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzLSapCfg
(
SuId        suId,             /* X2AP Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildCzLSapCfg(suId, spId)
SuId        suId;             /* X2AP Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
#if 1
   /* ccpu00129256 */
   CzMngmt  *czMgt = NULLP;
   CzSctSapCfg  *cfg = NULLP;
   Pst       pst;
 
   TRC2(smBuildCzLSapCfg)

   WRSM_ALLOC(&czMgt,sizeof(CzMngmt));
   if(NULLP == czMgt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(NULLP != czMgt)
   {
   cfg   = &(czMgt->t.cfg.u.sctSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->selector     = WRSM_CZSBSEL;
   cfg->mem.region   = CZ_MEM_REGION;
   cfg->mem.pool     = CZ_POOL;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->dstProcId    = SM_SB_PROC;
   cfg->dstEntId     = ENTSB;
   cfg->dstInstId    = (Inst)0;

   cfg->tBndTmr.enb = TRUE;
   cfg->tBndTmr.val = WRSM_LCZ_VAL_10;
   cfg->maxBndRetry = WRSM_LCZ_VAL_3;

   /* Fill Header */
   czMgt->hdr.msgType             = TCFG;
   czMgt->hdr.msgLen              = 0;
   czMgt->hdr.entId.ent           = ENTCZ;
   czMgt->hdr.entId.inst          = SM_INST_ZERO;
   czMgt->hdr.elmId.elmnt         = STCZSCTSAP;
   czMgt->hdr.response.mem.region = CZ_MEM_REGION;
   czMgt->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "X2AP LSap Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, czMgt);
}
else
{
   RLOG0(L_FATAL, "Memory allocation failed.");
}
#endif
   RETVALUE(ROK);
}/* smBuildCzLSapCfg */


/*
*
*       Fun:   smBuildCzUSapCfg
*
*       Desc:  Call handler for SSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzUSapCfg
(
SpId        spId              /* Service Provider Sap ID */
)
#else
PUBLIC S16 smBuildCzUSapCfg(spId)
SpId        spId;             /* Service Provider Sap ID */
#endif
{
#if 1
   /* ccpu00129256 */
   CzMngmt  *czMgt = NULLP;
   CzCztSapCfg    *cfg = NULLP;
   Pst      pst;
 
   TRC2(smBuildCzUSapCfg)

   WRSM_ALLOC(&czMgt,sizeof(CzMngmt));
   if(NULLP == czMgt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(NULLP != czMgt)
   {
   cfg = &(czMgt->t.cfg.u.cztSap);

   cfg->spId   = spId;
   cfg->selector     = WRSM_CZWRSEL;
   cfg->mem.region   = CZ_MEM_REGION;
   cfg->mem.pool     = CZ_POOL;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;

   /* Fill Header */
   czMgt->hdr.msgType             = TCFG;
   czMgt->hdr.msgLen              = 0;
   czMgt->hdr.entId.ent           = ENTCZ;
   czMgt->hdr.entId.inst          = SM_INST_ZERO;
   czMgt->hdr.elmId.elmnt         = STCZCZTSAP;
   czMgt->hdr.response.mem.region = CZ_MEM_REGION;
   czMgt->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = smCb.init.region;
   
   RLOG0(L_DEBUG, "X2AP USap Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, czMgt);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
#endif
   RETVALUE(ROK);
}/* smBuildCzUSapCfg */


/*
*
*       Fun:   smBuildCzProtCfg
*
*       Desc:  Call handler for Protocol Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzProtCfg
(
Void
)
#else
PUBLIC S16 smBuildCzProtCfg()
#endif
{
#if 1
   /* ccpu00129256 */
   CzMngmt  *czMgt = NULLP;
   CzProtCfg *protCfg = NULLP;
   Pst      pst;
 
   TRC2(smBuildCzProtCfg)

   WRSM_ALLOC(&czMgt,sizeof(CzMngmt));
   if(NULLP == czMgt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(NULLP != czMgt)
   {
   protCfg = &(czMgt->t.cfg.u.prot);
   WR_COPY_TPT_TO_NETADDR_ARR(protCfg->srcAddr.ipAddr.nAddr,protCfg->srcAddr.ipAddr.nmb,smCfgCb.enbIpAddr);
   protCfg->tRELOCprep = (U16)smCfgCb.x2PrepTimerVal;
   protCfg->tX2RELOCOverall = (U16)smCfgCb.x2OvrAllTimerVal;
   protCfg->timeToWait = (U16)smCfgCb.x2TimeToWaitTimerVal;
   protCfg->dynamPeerFlag = 1;
   protCfg->defDPeerUSapId = 0;
#ifdef CZ_ENB   
   protCfg->eNbId     =   WRSM_LCZ_ENB_PRC0;
   protCfg->srcAddr.sctPort =   WRSM_LCZ_ENB_SCTPORT;
#endif /* CZ_ENB */   

   /* Fill Header */
   czMgt->hdr.msgType             = TCFG;
   czMgt->hdr.msgLen              = 0;
   czMgt->hdr.entId.ent           = ENTCZ;
   czMgt->hdr.entId.inst          = SM_INST_ZERO;
   czMgt->hdr.elmId.elmnt         = STCZPROT;
   czMgt->hdr.response.mem.region = CZ_MEM_REGION;
   czMgt->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = smCb.init.region;
 
      RLOG0(L_DEBUG, "X2AP Protocol Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, czMgt);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
#endif 
   RETVALUE(ROK);
}/* smBuildCzProtCfg */

/*
*
*       Fun:   smBuildCzPeerCfg
*
*       Desc:  Call handler for Peer Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzPeerCfg
(
Void
)
#else
PUBLIC S16 smBuildCzPeerCfg()
#endif
{
   /* ccpu00129256 */
   CzMngmt  *czMgt = NULLP;
   CzPeerLstCfg *cfg = NULLP;
   U16      cnt;
   Pst      pst;
 
   TRC2(smBuildCzPeerCfg)

   WRSM_ALLOC(&czMgt,sizeof(CzMngmt));
   if(NULLP == czMgt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(NULLP != czMgt)
   {
   cfg = &(czMgt->t.cfg.u.peerLst);
   
   /* Due a bug in X2AP , noOfNghCfg must always be maxX2Peers -1 */
   cfg->nmbPeers = (U8)(smCfgCb.noOfNghCfg);
   /* TODO */
   if(WR_VAL_ZERO == cfg->nmbPeers)
   {
      wrCzCfg |= WR_CZ_PEER_CFG;
      RLOG0(L_WARNING, "There are no X2AP Peer Cfg present now.");
      RETVALUE(ROK);
   }

   if(cfg->nmbPeers > WR_SMM_MAX_NBR_ENB)
   {
      wrCzCfg |= WR_CZ_PEER_CFG;
      RLOG0(L_WARNING, "There are more X2AP Peers than Cfged present now.");
      RETVALUE(ROK);
   }
   
   WRSM_ALLOC(&cfg->peerCfg, sizeof(CzPeerCfg)* cfg->nmbPeers);

   if(NULLP == cfg->peerCfg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   for (cnt=0; cnt < cfg->nmbPeers; cnt++)
   {
#ifdef CZ_ENB   
      cfg->peerCfg[cnt].peerId   =  smCfgCb.wrNghCellCfg[cnt]->peerId;
      /* TODO */
      cfg->peerCfg[cnt].tSapId   =  0; 
      cfg->peerCfg[cnt].uSapId   =  0;  
      cfg->peerCfg[cnt].eAValue  =  UNKNOWN_VALUE;
      /*cfg->peerCfg[cnt].iPAValue =  0;*/
      cfg->peerCfg[cnt].iPAValue =  UNKNOWN_VALUE;
      cfg->peerCfg[cnt].assocCfg.locOutStrms =  WRSM_LCZ_OUT_STRMS;
      wrCpyTptToNetAddr(&cfg->peerCfg[cnt].assocCfg.priDstAddr, &smCfgCb.wrNghCellCfg[cnt]->enbAddr);
      cfg->peerCfg[cnt].assocCfg.dstAddrLst.nmb     =  1;
      wrCpyTptToNetAddr(&cfg->peerCfg[cnt].assocCfg.dstAddrLst.nAddr[0],&smCfgCb.wrNghCellCfg[cnt]->enbAddr);
      cfg->peerCfg[cnt].assocCfg.dstPort            =  WRSM_LCZ_ENB_SCTPORT;/*Peer eNB's port number.*/

#endif /* CZ_ENB */
  }

   /* Fill Header */
   czMgt->hdr.msgType             = TCFG;
   czMgt->hdr.msgLen              = 0;
   czMgt->hdr.entId.ent           = ENTCZ;
   czMgt->hdr.entId.inst          = SM_INST_ZERO;
   czMgt->hdr.elmId.elmnt         = STCZPEER;
   czMgt->hdr.response.mem.region = CZ_MEM_REGION;
   czMgt->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = smCb.init.region;
 
   RLOG0(L_DEBUG, "X2AP Peer Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, czMgt);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
}/* smBuildCzPeerCfg */
#ifdef WR_RSYS_OAM
/*
*
*       Fun:   smBuildDynCzPeerCfg
*
*       Desc:  Call handler for Peer Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildDynCzPeerCfg
(
LwrNeighAddModCfg *nghEnbCfg
)
#else
PUBLIC S16 smBuildDynCzPeerCfg(nghEnbCfg)
LwrNeighAddModCfg *nghEnbCfg;
#endif
{
   CzMngmt  *czMgt;
   CzPeerLstCfg *cfg = NULLP;
   U16      cnt = 0;
   Pst      pst;
 
   TRC2(smBuildDynCzPeerCfg)

   WRSM_ALLOC(&czMgt,sizeof(CzMngmt));
   if(NULLP == czMgt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(czMgt->t.cfg.u.peerLst);
   
   cfg->nmbPeers = 1;

   WRSM_ALLOC(&cfg->peerCfg, sizeof(CzPeerCfg)* cfg->nmbPeers);

   if(NULLP == cfg->peerCfg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

#ifdef CZ_ENB   
      cfg->peerCfg[cnt].peerId   =  nghEnbCfg->peerId; 
      cfg->peerCfg[cnt].tSapId   =  0; 
      cfg->peerCfg[cnt].uSapId   =  0;  
      cfg->peerCfg[cnt].eAValue  =  UNKNOWN_VALUE;
      /* cfg->peerCfg[cnt].iPAValue =  0; */
      cfg->peerCfg[cnt].iPAValue =  UNKNOWN_VALUE;
      cfg->peerCfg[cnt].assocCfg.locOutStrms =  WRSM_LCZ_OUT_STRMS;
      wrCpyTptToNetAddr(&cfg->peerCfg[cnt].assocCfg.priDstAddr, 
       &nghEnbCfg->ipAddr);
      cfg->peerCfg[cnt].assocCfg.dstAddrLst.nmb     =  1;
      wrCpyTptToNetAddr(&cfg->peerCfg[cnt].assocCfg.dstAddrLst.nAddr[0], 
       &nghEnbCfg->ipAddr);

      cfg->peerCfg[cnt].assocCfg.dstPort            =  WRSM_LCZ_ENB_SCTPORT;/*Peer eNB's port number.*/

#endif /* CZ_ENB */

   /* Fill Header */
   czMgt->hdr.msgType             = TCFG;
   czMgt->hdr.msgLen              = 0;
   czMgt->hdr.entId.ent           = ENTCZ;
   czMgt->hdr.entId.inst          = SM_INST_ZERO;
   czMgt->hdr.elmId.elmnt         = STCZPEER;
   czMgt->hdr.transId             = nghEnbCfg->enbId;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
 
   RLOG0(L_DEBUG, "X2AP Peer Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, czMgt);
   RETVALUE(ROK);
}/* smBuildDynCzPeerCfg */



#else
/*
*
*       Fun:   smBuildDynCzPeerCfg
*
*       Desc:  Call handler for Peer Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildDynCzPeerCfg
(
U32 nbrIndex,
U8  dbIndex
)
#else
PUBLIC S16 smBuildDynCzPeerCfg(nbrIndex,dbIndex)
U32 nbrIndex;
U8  dbIndex;
#endif
{
   /* ccpu00129256 */
   CzMngmt  *czMgt = NULLP;
   CzPeerLstCfg *cfg = NULLP;
   U16      cnt = 0;
   Pst      pst;
 
   TRC2(smBuildDynCzPeerCfg)

   WRSM_ALLOC(&czMgt,sizeof(CzMngmt));
   if(NULLP == czMgt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(NULLP != czMgt)
   {
   cfg = &(czMgt->t.cfg.u.peerLst);
   
   /* Due a bug in X2AP , noOfNghCfg must always be maxX2Peers -1 */
   cfg->nmbPeers = 1;
   /* kworks fix */
   /* below check is not required */

   WRSM_ALLOC(&cfg->peerCfg, sizeof(CzPeerCfg)* cfg->nmbPeers);

   if(NULLP == cfg->peerCfg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

#ifdef CZ_ENB   
      cfg->peerCfg[cnt].peerId   =  smCfgCb.wrNghCellCfg[nbrIndex -1]->peerId; 
      cfg->peerCfg[cnt].tSapId   =  0; 
      cfg->peerCfg[cnt].uSapId   =  0;  
      cfg->peerCfg[cnt].eAValue  =  UNKNOWN_VALUE;
      /* cfg->peerCfg[cnt].iPAValue =  0; */
      cfg->peerCfg[cnt].iPAValue =  UNKNOWN_VALUE;
      cfg->peerCfg[cnt].assocCfg.locOutStrms =  WRSM_LCZ_OUT_STRMS;

      wrCpyTptToNetAddr(&cfg->peerCfg[cnt].assocCfg.priDstAddr, 
       &smCfgCb.wrNghCellCfg[nbrIndex -1]->enbAddr);
      cfg->peerCfg[cnt].assocCfg.dstAddrLst.nmb     =  1;
      wrCpyTptToNetAddr(&cfg->peerCfg[cnt].assocCfg.dstAddrLst.nAddr[0], 
       &smCfgCb.wrNghCellCfg[nbrIndex - 1]->enbAddr);


      cfg->peerCfg[cnt].assocCfg.dstPort            =  WRSM_LCZ_ENB_SCTPORT;/*Peer eNB's port number.*/

#endif /* CZ_ENB */

   /* Fill Header */
   czMgt->hdr.msgType             = TCFG;
   czMgt->hdr.msgLen              = 0;
   czMgt->hdr.entId.ent           = ENTCZ;
   czMgt->hdr.entId.inst          = SM_INST_ZERO;
   czMgt->hdr.elmId.elmnt         = STCZPEER;
   czMgt->hdr.transId             = dbIndex;
   czMgt->hdr.response.mem.region = CZ_MEM_REGION;
   czMgt->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = smCb.init.region;
 
   RLOG0(L_DEBUG, "X2AP Peer Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, czMgt);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
}/* smBuildDynCzPeerCfg */
#endif
 /*
 *      FUN:   smBldrCzPeerDelCntrlReq
 *
 *      Desc:  Builds the delete peer control request and sends it to
 *             S1AP
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  wr_smm_x2ap.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBldrCzPeerDelCntrlReq
(
U32    peerId,
U32     dbIndex
)
#else
PUBLIC S16 smBldrCzPeerDelCntrlReq()
U32    peerId;
U32     dbIndex;
#endif /* ANSI */
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;
   
   TRC2(smBldrCzPeerDelCntrlReq)
   
   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        = ADEL;
   cntrl->t.cntrl.subAction    = SAELMNT;
   /* Fill the peerid value which needs to be deleted */
   cntrl->t.cntrl.u.peer.peerId = peerId;

   /* Fill the index which will be used when we receive the confirmation
    * for the delete peer request */
   cntrl->hdr.transId             = dbIndex;
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZPEER;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = SM_MEM_REGION;

   RLOG0(L_DEBUG, "Delete X2AP Peer Cfg for Dynamic Neighbour EnodeB Sent");

    /* Send the control request to the S1AP */
   (Void)SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smBldrCzPeerDelCntrlReq */
 

/*
 *      FUN:   smBindUnBindCzToLSap
 *
 *      Desc:  Bind and Unbind X2AP to lower SAP
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  wr_smm_x2ap.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindUnBindCzToLSap
(
U8 action
)
#else
PUBLIC S16 smBindUnBindCzToLSap(action)
U8 action;
#endif /* ANSI */
{
#if 1
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;
   S16      ret = ROK;
   
   TRC2(smBindUnBindCzToLSap)
   
   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   
   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        = action;
   cntrl->t.cntrl.subAction     = SAELMNT;
   cntrl->t.cntrl.u.sap.id      = WR_CZ_SUID; 

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZSCTSAP;

   cntrl->hdr.response.mem.pool   = CZ_POOL;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;

   if (action == ABND_ENA)
   {
      RLOG0(L_DEBUG, "X2AP Bind Req to LSap sent");
   }
   else
   {
      RLOG0(L_DEBUG, "X2AP UnBind Req to LSap sent");
   }

    /* Send the control request to the X2AP */
   (Void)SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ret);
#endif
   RETVALUE(ROK);
} /* end of smBindUnBindCzToLSap */

/*
*
*       Fun:   smBuildCzGenCntrl
*
*       Desc:  Invoked to enable Alarm at X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzGenCntrl
(
)
#else
PUBLIC S16 smBuildCzGenCntrl(Void)
#endif
{
#if 1
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;
   
   TRC2(smBuildCzGenCntrl)
   
   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  AENA;
   cntrl->t.cntrl.u.sap.id      =  0;
   cntrl->t.cntrl.subAction     =  SAUSTA;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZGEN;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;
   
   RLOG0(L_DEBUG, "X2AP Gen Cntrl Req to cz sent");

   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
#endif
   RETVALUE(ROK);
} /* end of smBuildCzGenCntrl */
 
/**
 * @brief This function is used to send enable/disable control request to 
 * the X2AP layers.
 *
 * @details
 *
 * Function: smBuildCzLogCntrl
 *
 * @param[in] actType
 * @param[in] mask
 *
 * @return Void
 */

#ifdef ANSI
PUBLIC S16 smBuildCzLogCntrl
(
 Bool actType,
 U32  mask
)
#else
PUBLIC S16 smBuildCzLogCntrl(actType, mask)
 Bool actType;
 U32  mask;
#endif
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smBuildCzLogCntrl)

   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(NULLP != cntrl)
   {
   if(actType == WR_ON)
   {
      cntrl->t.cntrl.action        =  AENA;
   }
   else
   {
      cntrl->t.cntrl.action        =  ADISIMM;
   }

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZGEN;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = smCb.init.region;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;

   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smBuildCzLogCntrl */
 
/*
*
*       Fun:   smBuildCzDbgCntrl
*
*       Desc:  Invoked to Enable Debugs at X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzDbgCntrl
(
)
#else
PUBLIC S16 smBuildCzDbgCntrl(Void)
#endif
{
#if 1
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smBuildCzDbgCntrl)
   
   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  AENA;
   cntrl->t.cntrl.subAction     =  SADBG;

#ifdef DEBUGP
   cntrl->t.cntrl.u.dbg.dbgMask =  0xffffffff;
#endif
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZGEN;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;
   
   RLOG0(L_DEBUG, "X2AP Dbg Cntrl Req to cz sent");

   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
#endif
   RETVALUE(ROK);
} /* end of smBuildCzDbgCntrl */


/*
*
*       Fun:   smBuildCzShutDownCntrl
*
*       Desc:  Invoked to shut down the X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzShutDownCntrl
(
)
#else
PUBLIC S16 smBuildCzShutDownCntrl(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smBuildCzShutDownCntrl)

   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  ASHUTDOWN;
   cntrl->t.cntrl.subAction     =  NULLD;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZGEN;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "X2AP ShutDown Cntrl Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smBuildCzShutDownCntrl */    

/*
*
*       Fun:   smUbindCzFrmUSap
*
*       Desc:  Invoked to disable CZT SAP at X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smUbindCzFrmUSap
(
)
#else
PUBLIC S16 smUbindCzFrmUSap(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smUbindCzFrmUSap)

   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  AUBND_DIS;
   cntrl->t.cntrl.subAction     =  NULLD;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZCZTSAP;
      cntrl->hdr.response.mem.region = CZ_MEM_REGION;
      cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
      pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "Upper SAP Disable+Unbind Cntrl Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smUbindCzFrmUSap */

/*
*
*       Fun:   smDelCzUSap
*
*       Desc:  Invoked to delete a CZT SAP at X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smDelCzUSap
(
)
#else
PUBLIC S16 smDelCzUSap(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smDelCzUSap)

   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  ADEL;
   cntrl->t.cntrl.subAction     =  NULLD;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZCZTSAP;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "Upper SAP Delete Cntrl Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smDelCzUSap */

/*
*
*       Fun:   smBuildCzLSapTrcCntrl
*
*       Desc:  Invoked to Enable\Disable Trace for a SCT SAP at X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzLSapTrcCntrl
(
U16 action
)
#else
PUBLIC S16 smBuildCzLSapTrcCntrl(action)
U16 action;
#endif
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smBuildCzLSapTrcCntrl)

   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  action;
   cntrl->t.cntrl.subAction     =  SATRC;
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZSCTSAP;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "Lower SAP Trace Cntrl Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smBuildCzLSapTrcCntrl */


/*
*
*       Fun:   smUbindCzFrmLSap
*
*       Desc:  Invoked to  CZT SAP at X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smUbindCzFrmLSap
(
)
#else
PUBLIC S16 smUbindCzFrmLSap(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smUbindCzFrmLSap)

   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  AUBND_DIS;
   cntrl->t.cntrl.subAction     =  SAELMNT;

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZSCTSAP;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "Lower SAP Disable+Unbind Cntrl Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smUbindCzFrmLSap */

/*
*
*       Fun:   smDelCzLSap
*
*       Desc:  Invoked to delete a SCT SAP at X2AP
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smDelCzLSap
(
)
#else
PUBLIC S16 smDelCzLSap(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smDelCzLSap)

   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  ADEL;
   cntrl->t.cntrl.subAction     =  SAELMNT;
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZSCTSAP;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "Upper SAP Delete Cntrl Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;
   pst.region    = smCb.init.region;
   RETVALUE(ROK);
} /* end of smDelCzLSap */

/*
*
*       Fun:   smBuildCzPeerTrcCntrl
*
*       Desc:  Invoked to Enable\Disable Trace at a particular Peer.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzPeerTrcCntrl
(
)
#else
PUBLIC S16 smBuildCzPeerTrcCntrl(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smBuildCzPeerTrcCntrl)

   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  AENA;
   cntrl->t.cntrl.subAction     =  SATRC;
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZPEER;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "PeerTrc Cntrl Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smBuildCzPeerTrcCntrl */


/*
*
*       Fun:   smDelCzPeer
*
*       Desc:  Invoked to delete a particular Peer.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smDelCzPeer
(
)
#else

PUBLIC S16 smDelCzPeer(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smDelCzPeer)

   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));/*suresh*/
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  ADEL;
   cntrl->t.cntrl.subAction     =  SAELMNT;
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STCZPEER;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "Peer Delete Cntrl Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smDelCzPeer */


/*
*
*       Fun:   smBuildCzEndPtCntrl
*
*       Desc:  Invoked to Enable\Disable SCTP End Point.
*
*       Ret:   ROK
*

*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzEndPtCntrl
(
)
#else

PUBLIC S16 smBuildCzEndPtCntrl(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smBuildCzEndPtCntrl)

   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  AENA;
   cntrl->t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STTPTSRV;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "End Point Cntrl Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smBuildCzEndPtCntrl */

/*
*
*       Fun:   smBuildCzAssocCntrl
*
*       Desc:  Invoked to change the primary destination IP Address.
*
*       Ret:   ROK
*

*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzAssocCntrl
(
)
#else

PUBLIC S16 smBuildCzAssocCntrl(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt  *cntrl = NULLP;
   Pst      pst;

   TRC2(smBuildCzAssocCntrl)

   WRSM_ALLOC(&cntrl,sizeof(CzMngmt));
   if(NULLP == cntrl)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != cntrl)
   {
   cntrl->t.cntrl.action        =  AMODIFY;
   cntrl->t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTCZ;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STASSOC;
   cntrl->hdr.response.mem.region = CZ_MEM_REGION;
   cntrl->hdr.response.mem.pool   = CZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
   pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "Associaion Cntrl Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczCntrlReq(&pst, cntrl);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smBuildCzAssocCntrl */

/*
*
*       Fun:   smBuildCzGenStsReq
*
*       Desc:  Invoked to send the General statistic request to X2AP.
*
*       Ret:   ROK
*

*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzGenStsReq
(
)
#else

PUBLIC S16 smBuildCzGenStsReq(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt *sts = NULLP;
   Pst      pst;
   Action action;

   TRC2(smBuildCzGenStsReq)

   WRSM_ALLOC(&sts,sizeof(CzMngmt));
   if(NULLP == sts)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   action = NOZEROSTS;
   if(NULLP != sts)
   {
   sts->t.cntrl.action        =  NULLD;
   sts->t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sts->hdr.msgType             = TSTS;
   sts->hdr.entId.ent           = ENTCZ;
   sts->hdr.entId.inst          = SM_INST_ZERO;
   sts->hdr.elmId.elmnt         = STCZGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;

   RLOG0(L_DEBUG, "General Statistics Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczStsReq(&pst, action, sts);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   pst.region    = smCb.init.region;
   RETVALUE(ROK);
} /* end of smBuildCzGenStsReq */


/*
*
*       Fun:   smBuildCzPeerStsReq
*
*       Desc:  Invoked to send the Peer statistic request to X2AP.
*
*       Ret:   ROK
*

*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzPeerStsReq
(
)
#else

PUBLIC S16 smBuildCzPeerStsReq(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt *sts = NULLP;
   Pst      pst;
   Action action;

   TRC2(smBuildCzPeerStsReq)

   WRSM_ALLOC(&sts,sizeof(CzMngmt));
   if(NULLP == sts)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   action = NOZEROSTS;
   if(NULLP != sts)
   {
   sts->t.cntrl.action        =  NULLD;
   sts->t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sts->hdr.msgType             = TSTS;
   sts->hdr.entId.ent           = ENTCZ;
   sts->hdr.entId.inst          = SM_INST_ZERO;
   sts->hdr.elmId.elmnt         = STCZPEER;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;

    RLOG0(L_DEBUG, "General Statistics Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczStsReq(&pst, action, sts);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   pst.region    = smCb.init.region;
   RETVALUE(ROK);
} /* end of smBuildCzStsReq */


/*
*
*       Fun:   smBuildCzGenStaReq
*
*       Desc:  Invoked to send the General status request to X2AP.
*
*       Ret:   ROK
*

*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzGenStaReq
(
)
#else

PUBLIC S16 smBuildCzGenStaReq(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt *sta = NULLP;
   Pst      pst;

   TRC2(smBuildCzGenStaReq)

   WRSM_ALLOC(&sta,sizeof(CzMngmt));
   if(NULLP == sta)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != sta)
   {
   sta->t.cntrl.action        =  NULLD;
   sta->t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sta->hdr.msgType             = TSSTA;
   sta->hdr.entId.ent           = ENTCZ;
   sta->hdr.entId.inst          = SM_INST_ZERO;
   sta->hdr.elmId.elmnt         = STCZGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
      pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "General Status Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczStaReq(&pst, sta);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smBuildCzGenStaReq */

/*
*
*       Fun:   smBuildCzSidStaReq
*
*       Desc:  Invoked to send the Sysytem Id request to X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzSidStaReq
(
)
#else

PUBLIC S16 smBuildCzSidStaReq(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt *sta = NULLP;
   Pst      pst;

   TRC2(smBuildCzSidStaReq)

   WRSM_ALLOC(&sta,sizeof(CzMngmt));
   if(NULLP == sta)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != sta)
   {
   sta->t.cntrl.action        =  NULLD;
   sta->t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sta->hdr.msgType             = TSSTA;
   sta->hdr.entId.ent           = ENTCZ;
   sta->hdr.entId.inst          = SM_INST_ZERO;
   sta->hdr.elmId.elmnt         = STCZSID;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
      pst.region    = smCb.init.region;

    RLOG0(L_DEBUG, "System Id Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczStaReq(&pst, sta);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smBuildCzSidStaReq */

/*
*
*       Fun:   smBuildCzUSapStaReq
*
*       Desc:  Invoked to send Upper Sap statistic request to X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzUSapStaReq
(
)
#else

PUBLIC S16 smBuildCzUSapStaReq(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt *sta = NULLP;
   Pst      pst;

   TRC2(smBuildCzUSapStaReq)

   WRSM_ALLOC(&sta,sizeof(CzMngmt));
   if(NULLP == sta)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != sta)
   {
   sta->t.cntrl.action        =  NULLD;
   sta->t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sta->hdr.msgType             = TSSTA;
   sta->hdr.entId.ent           = ENTCZ;
   sta->hdr.entId.inst          = SM_INST_ZERO;
   sta->hdr.elmId.elmnt         = STCZCZTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
      pst.region    = smCb.init.region;

   RLOG0(L_DEBUG, "Upper SAP Statistics Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczStaReq(&pst, sta);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   RETVALUE(ROK);
} /* end of smBuildCzUSapStaReq */

/*
*
*       Fun:   smBuildCzLSapStaReq
*
*       Desc:  Invoked to send Lower Sap statistic request to X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzLSapStaReq
(
)
#else

PUBLIC S16 smBuildCzLSapStaReq(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt *sta = NULLP;
   Pst      pst;

   TRC2(smBuildCzLSapStaReq)

   WRSM_ALLOC(&sta,sizeof(CzMngmt));
   if(NULLP == sta)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != sta)
   {
   sta->t.cntrl.action        =  NULLD;
   sta->t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sta->hdr.msgType             = TSSTA;
   sta->hdr.entId.ent           = ENTCZ;
   sta->hdr.entId.inst          = SM_INST_ZERO;
   sta->hdr.elmId.elmnt         = STCZSCTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;

   RLOG0(L_DEBUG, "Lower SAP Statistics Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczStaReq(&pst, sta);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }
   pst.region    = smCb.init.region;
   RETVALUE(ROK);
} /* end of smBuildCzLSapStaReq */

/*
*
*       Fun:   smBuildCzPeerStaReq
*
*       Desc:  Invoked to send Peer statistic request to X2AP.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildCzPeerStaReq
(
)
#else

PUBLIC S16 smBuildCzPeerStaReq(Void)
#endif
{
   /* ccpu00129256 */
   CzMngmt *sta = NULLP;
   Pst      pst;

   TRC2(smBuildCzPeerStaReq)

   WRSM_ALLOC(&sta,sizeof(CzMngmt));
   if(NULLP == sta)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));
   if(NULLP != sta)
   {
   sta->t.cntrl.action        =  NULLD;
   sta->t.cntrl.subAction     =  NULLD;
   /* Fill Header */
   sta->hdr.msgType             = TSSTA;
   sta->hdr.entId.ent           = ENTCZ;
   sta->hdr.entId.inst          = SM_INST_ZERO;
   sta->hdr.elmId.elmnt         = STCZSCTSAP;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;

   RLOG0(L_DEBUG, "Lower SAP Statistics Req to cz sent");
   /* Send the request to the X2AP */
   SmMiLczStaReq(&pst, sta);
   }
else
{
      RLOG0(L_FATAL, "Memory allocation failed.");
}

   pst.region    = smCb.init.region;
   RETVALUE(ROK);
} /* end of smBuildCzPeerStaReq */


/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from CZ
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   wr_sm_cz_exms.c
*
*/

#ifdef ANSI
PUBLIC S16 smCzActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smCzActvTsk(pst, mBuf)
Pst *pst;                   /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16 ret;

   TRC2(smCzActvTsk)

   ret = ROK;

   switch(pst->event)
   {
#ifdef LCSMMILCZ
      case EVTLCZCFGCFM:             /* Config confirm */
         ret = cmUnpkLczCfgCfm(SmMiLczCfgCfm, pst, mBuf);
         break;
      case EVTLCZCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLczCntrlCfm(SmMiLczCntrlCfm, pst, mBuf);
         break;
      case EVTLCZSTAIND:             /* Status Indication */
         ret = cmUnpkLczStaInd(SmMiLczStaInd, pst, mBuf);
         break;
#endif
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         RLOG1(L_ERROR, "Invalid event [%d] received", pst->event);
         break;
   }

   RETVALUE(ret);

} /* end of smCzActvTsk */

/*local defines */
#define SM_MAX_CZ_SEL 4

/*local typedefs */

/*forward references */
PUBLIC S16 PtMiLczCfgReq      (Pst *post, CzMngmt *cfg);
PUBLIC S16 PtMiLczStsReq      (Pst *post, Action action, CzMngmt *sts);
PUBLIC S16 PtMiLczStaReq      (Pst *post, CzMngmt *ssta);
PUBLIC S16 PtMiLczCntrlReq    (Pst *post, CzMngmt *cntrl);

/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */

/* Following matrices define the mapping between the primitives called by the
 * layer manager (as SmMiLczXXXReq) and the corresponding primitives of the 
 * X2AP layer (layer management service provider) (as CzMiLczXXXReq).
 * Each primitive is mapped to one of SM_MAX_CZ_SEL functions in the array.
 * The mapping is based on post->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCSMMILCZ    loose         cmPkMiLczXXXReq
 * 1                 MG          tight         CzMiLczXXXReq
 * 2+                            tight         PtMiLczXXXReq
 */

PRIVATE LczCfgReq smMiLczCfgReqMt[] =
{
#ifdef LCSMMILCZ
   cmPkLczCfgReq,
#else
   PtMiLczCfgReq,
#endif

#ifdef CZ
   CzMiLczCfgReq,
#else
   PtMiLczCfgReq,
#endif

};


PRIVATE LczStsReq smMiLczStsReqMt[] =
{
#ifdef LCSMMILCZ
   cmPkLczStsReq,
#else
   PtMiLczStsReq,
#endif

#ifdef CZ
   CzMiLczStsReq,
#else
   PtMiLczStsReq,
#endif

};


PRIVATE LczStaReq smMiLczStaReqMt[] =
{
#ifdef LCSMMILCZ
   cmPkLczStaReq,
#else
   PtMiLczStaReq,
#endif

#ifdef CZ
   CzMiLczStaReq,
#else
   PtMiLczStaReq,
#endif

};


PRIVATE LczCntrlReq smMiLczCntrlReqMt[] =
{
#ifdef LCSMMILCZ
   cmPkLczCntrlReq,
#else
   PtMiLczCntrlReq,
#endif

#ifdef CZ
   CzMiLczCntrlReq,
#else
   PtMiLczCntrlReq,
#endif

};


/*
 *
 *       Fun:    SmMiLczCfgReq - Configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczCfgReq
(
Pst     *post,       /* post structure */
CzMngmt *cfg        /* management structure */
)
#else
PUBLIC S16 SmMiLczCfgReq(post, cfg)
Pst     *post;       /* post structure */
CzMngmt *cfg;       /* management structure */
#endif
{

   TRC3(SmMiLczCfgReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_CZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLczCfgReqMt[post->selector]))(post, cfg));
} /* end of SmMiLczCfgReq() */

/*
 *
 *       Fun:    SmMiLczStsReq - Statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczStsReq
(
Pst     *post,       /* post structure */
Action action,
CzMngmt *sts        /* management structure */
)
#else
PUBLIC S16 SmMiLczStsReq(post, action, sts)
Pst     *post;       /* post structure */
Action action;
CzMngmt *sts;       /* management structure */
#endif
{
   TRC3(SmMiLczStsReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_CZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLczStsReqMt[post->selector]))(post, action, sts));
} /* end of SmMiLczStsReq() */

/*
 *
 *       Fun:    SmMiLczStaReq - Status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczStaReq
(
Pst     *post,       /* post structure */
CzMngmt *ssta        /* management structure */
)
#else
PUBLIC S16 SmMiLczStaReq(post, ssta)
Pst     *post;       /* post structure */
CzMngmt *ssta;       /* management structure */
#endif
{
   TRC3(SmMiLczStaReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_CZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLczStaReqMt[post->selector]))(post, ssta));
} /* end of SmMiLczStaReq() */

/*
 *
 *       Fun:    SmMiLczCntrlReq - Control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczCntrlReq
(
Pst     *post,       /* post structure */
CzMngmt *cntrl        /* management structure */
)
#else
PUBLIC S16 SmMiLczCntrlReq(post, cntrl)
Pst     *post;       /* post structure */
CzMngmt *cntrl;       /* management structure */
#endif
{
   TRC3(SmMiLczCntrlReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_CZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLczCntrlReqMt[post->selector]))(post, cntrl));
} /* end of SmMiLczCntrlReq() */

/*
 *
 *       Fun:    PtMiLczCfgReq - portable configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLczCfgReq
(
Pst     *post,       /* post structure */
CzMngmt *cfg        /* management structure */
)
#else
PUBLIC S16 PtMiLczCfgReq(post, cfg)
Pst     *post;       /* post structure */
CzMngmt *cfg;       /* management structure */
#endif
{
   TRC3(PtMiLczCfgReq)

   UNUSED(post);
   UNUSED(cfg);
   RETVALUE(RFAILED);
} /* end of PtMiLczCfgReq() */

/*
 *
 *       Fun:    PtMiLczStsReq - portable statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLczStsReq
(
Pst     *post,       /* post structure */
Action action,
CzMngmt *sts        /* management structure */
)
#else
PUBLIC S16 PtMiLczStsReq(post, action, sts)
Pst     *post;       /* post structure */
Action action;
CzMngmt *sts;       /* management structure */
#endif
{
   TRC3(PtMiLczStsReq)

   UNUSED(post);
   UNUSED(action);
   UNUSED(sts);
   RETVALUE(RFAILED);
} /* end of PtMiLczStsReq() */

/*
 *
 *       Fun:    PtMiLczStaReq - portable status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLczStaReq
(
Pst     *post,       /* post structure */
CzMngmt *ssta        /* management structure */
)
#else
PUBLIC S16 PtMiLczStaReq(post, ssta)
Pst     *post;       /* post structure */
CzMngmt *ssta;       /* management structure */
#endif
{
   TRC3(PtMiLczStaReq)

   UNUSED(post);
   UNUSED(ssta);
   RETVALUE(RFAILED);
} /* end of PtMiLczStaReq() */

/*
 *
 *       Fun:    PtMiLczCntrlReq - portable control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLczCntrlReq
(
Pst     *post,       /* post structure */
CzMngmt *cntrl        /* management structure */
)
#else
PUBLIC S16 PtMiLczCntrlReq(post, cntrl)
Pst     *post;       /* post structure */
CzMngmt *cntrl;       /* management structure */
#endif
{
   TRC3(PtMiLczCntrlReq)

   UNUSED(post);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
} /* end of PtMiLczCntrlReq() */


/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_sm_cz_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLczCfgCfm
(
Pst     *pst,          /* post structure */
CzMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLczCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
CzMngmt *cfm;          /* configuration */
#endif
{
   Pst      czPst;
   U32       dbIndex;
   S16      retVal;

   TRC2(SmMiLczCfgCfm);

   SM_SET_ZERO(&czPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
       switch (cfm->hdr.elmId.elmnt)
       {
          case STCZGEN:
               wrCzCfg |= WR_CZ_GEN_CFG;
               break;      
          case STCZPROT:
               wrCzCfg |= WR_CZ_PROT_CFG;
               break;
          case STCZCZTSAP:
               wrCzCfg |= WR_CZ_CZT_SAP_CFG;
               break;
          case STCZSCTSAP:
               wrCzCfg |= WR_CZ_SCT_SAP_CFG;
               break;
#ifndef WR_RSYS_OAM
          case STCZPEER:
               wrCzCfg |= WR_CZ_PEER_CFG;
               break;
#endif
          default:
               break;
       } /* end of switch statement */

      RLOG1(L_DEBUG,"X2AP Cfg Cfm received for the element %d",
         cfm->hdr.elmId.elmnt);

       /* Since for the first eNB that is getting deployed, there may not be
          any Peers. Hence there is no Peer config present. So to avoid 
          failure, we are checking for another condition of if CZ Configured
          without peers. */

      if (((wrCzCfg == WR_SM_CZ_CONFGRD_WO_PEER) && (WR_VAL_ZERO == smCfgCb.noOfNghCfg)) 
            || wrCzCfg == WR_SM_CZ_CONFIGURED)
         {
            {
#if WR_RSYS_OAM
               if(((smCb.smState == WR_SM_STATE_CELL_UP) ||
                  (smCb.smState == WR_SM_STATE_OAM_DYN_START)) &&
                  (cfm->hdr.elmId.elmnt == STCZPEER))
#else
               if(smCb.smState == WR_SM_STATE_CELL_UP)
#endif
               {
                  /* Send Newly addedd Nbr Enb Cfg to Enbapp*/
             /* if (retVal == ROK) if enbapp sends ROK then
                send X2Init Cntrl req to application */
                  dbIndex = cfm->hdr.transId;
                  /*127802*/
                  RLOG1(L_DEBUG, "Received X2AP Confirm for Dynamic detected "
                     "Neighbour Enodeb: dbIndex=%d", dbIndex);

                  retVal = wrSmmSendDynNbrEnbCfg(dbIndex);
               }
               else
               {
						{
                  smCb.smState = WR_SM_STATE_X2AP_CFG_DONE;
                  RLOG0(L_ALWAYS, "X2AP CONFIGURATION SUCCESSFULLY PERFORMED.");
#ifdef WR_RSYS_OAM  
                  smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
#endif
#ifdef RM_INTF
                  WR_FILL_PST(czPst, SM_CZ_PROC, ENTSM, ENTWR, EVTWRRRMCFG, WR_POOL, SM_SELECTOR_LC);
#else
                  WR_FILL_PST(czPst, SM_CZ_PROC, ENTSM, ENTWR, EVTWRAPPCFG, WR_POOL, SM_SELECTOR_LC);
#endif /*RM_INTF*/
						}
                  if(wrSmSelfPstMsg(&czPst) != ROK)
                  {
                     RLOG0(L_FATAL, "Self Pst failed after X2AP Configuation.");
                  }
                  else
                  {
                     RLOG0(L_DEBUG, "Self Pst success after X2AP Configuation.");
                  } /* end of else part */
               }

            }
         }  /* end of wrCzCfg == WR_SM_CZ_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
      RLOG0(L_ERROR, "Configuration confirm for X2Ap is not OK.");
#ifdef WR_RSYS_OAM  
      /*timer stop :: Raise an Alarm */
      smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
      smCb.cfgCb.alarm.severity  = SM_ALARM_SEVERITY_MAJOR;
      smCb.cfgCb.alarm.causeType = SM_ALARM_CAUSE_TYPE_ERR_IND;
      smCb.cfgCb.alarm.causeVal  = SM_ALARM_CAUSE_VAL_INIT_FAIL;
      smStaticCfgFailAlarm();
#endif
   }
   /* MEM LEAK FIX: */
   WR_FREE(cfm, sizeof(CzMngmt));

   RETVALUE(ROK);
} /* end of SmMiLczCfgCfm */

/*
 *
 *       Fun:    SmMiLczCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczCntrlCfm
(
 Pst         *pst,
 CzMngmt     *cntrl
 )
#else
PUBLIC S16 SmMiLczCntrlCfm(pst, cntrl)
   Pst         *pst;
   CzMngmt     *cntrl;
#endif /* ANSI */
{
   Pst  czPst;

   TRC2(SmMiLczCntrlCfm);

   SM_SET_ZERO(&czPst, sizeof(Pst));

   /* IPSec */
   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
      if (smCb.smState == WR_SM_STATE_EGTP_SHUTDWN_DONE)
      {
         smCb.smState = WR_SM_STATE_X2AP_SHUTDWN_DONE;
         RLOG0(L_ALWAYS, "X2AP SHUTDOWN SUCCESSFULLY PERFORMED.");
#ifdef RM_INTF
         WR_FILL_PST(czPst, SM_CZ_PROC, ENTSM, ENTWR, EVTWRS1APSHUT, WR_POOL, SM_SELECTOR_LC);
#else
         WR_FILL_PST(czPst, SM_CZ_PROC, ENTSM, ENTWR, EVTWRS1APSHUT, WR_POOL, SM_SELECTOR_LC);
#endif /*RM_INTF*/

         if(wrSmSelfPstMsg(&czPst) != ROK)
         {
            RLOG0(L_FATAL, "Self Pst failed after X2AP SHUTDOWN Configuration.");
         }
         else
         {
            RLOG0(L_DEBUG, "Self Pst success after X2AP SHUTDOWN Configuration.");
         } /* end of else part */
         WR_FREE(cntrl, sizeof(CzMngmt));
         RETVALUE(ROK);
      }
   }
   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
      if(cntrl->hdr.elmId.elmnt == STCZGEN)
      {
         if((smCfgCb.czDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_CZ_DBG_MASK))
         {
            /* Update DbgMsk */
            smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | WR_SM_CZ_DBG_MASK);
            /* Trigger Debug Cntrl */
            wrSmHdlEnbDebugs(EVTWRENBDEBUGS);
         }
      }

   } /* end of if statement */
   /* Initiate control request for binding App to X2AP */
   if(cntrl->hdr.elmId.elmnt == STCZSCTSAP)
   {
      /* Due to unbind */
      if (smCb.smState == WR_SM_STATE_RRC_STK_UNBND_DONE)
      {
         if (cntrl->cfm.status == LCM_PRIM_OK)
   {
            RLOG0(L_DEBUG,"UNBIND OF X2AP WITH SCTP SUCCESSFUL");

            smCb.smState = WR_SM_STATE_X2AP_STK_UNBND_DONE;
            WR_FILL_PST(czPst, SM_WR_PROC, ENTSM, ENTWR, EVTWREGTPSTKUNBND,
               WR_POOL, SM_SELECTOR_TC);
            if(wrSmSelfPstMsg(&czPst) != ROK)
            {
               RLOG1(L_ERROR,"Failure at Self Pst.[%d]", smCb.smState);
            }
         }
         WR_FREE(cntrl, sizeof(CzMngmt));
         RETVALUE(ROK);
   } 
      RLOG0(L_DEBUG, "Bind of X2AP LSap with SCTP is in progress");
   } /* end of if statement */

   /* MEM LEAK FIX:  */
   WR_FREE(cntrl, sizeof(CzMngmt));

   RETVALUE(ROK);
} /* end of SmMiLczCntrlCfm() */

/*
 *
 *       Fun:    SmMiLczStaInd - status indication
 *
 *       Desc:   Received the Status Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczStaInd
(
Pst         *pst,
CzMngmt      *staInd
)
#else
PUBLIC S16 SmMiLczStaInd(pst, staInd)
Pst         *pst;
CzMngmt      *staInd;
#endif /* ANSI */
{
   Pst  czPst;
   CzUsta  *usta= NULLP;
   CzUstaDgn *dgn;

   TRC2(SmMiLczStaInd)

   SM_SET_ZERO(&czPst, sizeof(Pst));
   usta = &(staInd->t.usta);
   dgn = &(usta->dgn);

   if (usta->alarm.event == LCZ_EVENT_LI_ASSOC_CFM  
      && usta->alarm.cause == LCZ_CAUSE_ASSOC_UP) 
   {
      RLOG0(L_ALWAYS, "X2AP ASSOCIATION SUCCESSFULLY ESTABLISHED.");
   } /* end of if statement */
   else if (usta->alarm.category == LCM_CATEGORY_INTERFACE)
   {
      if (usta->alarm.event == LCM_EVENT_BND_OK)
      {
         RLOG1(L_DEBUG,"BIND OF X2AP lsap id[%d] IS SUCCESSFUL", dgn->u.sapId);
         smBindUnBindWrToCztSap(ABND_ENA); 
      }
      else if (usta->alarm.event == LCM_EVENT_BND_FAIL) 
      {
         RLOG1(L_ERROR, "BIND OF X2AP lsap id[%d] FAILED", dgn->u.sapId);
      }
      else if((usta->alarm.event == LCZ_EVENT_LI_TERM_CFM) &&
            (usta->dgn.alarmType == LCZ_USTA_DGNVAL_PEER) &&
            (usta->alarm.cause == LCZ_CAUSE_PEER_DELETED))
      {
         RLOG1(L_DEBUG, "Term Cfm Recvd. peer id[%ld]", dgn->u.peerInfo.peerId); 
         /*127802*/
         RLOG1(L_DEBUG, "Received the ALARM for del peer for ID %ld",
               usta->dgn.u.peerInfo.peerId);
         /*Check if any add request is pending and send the ADD Request*/
      }
      else if (usta->alarm.event == LCZ_EVENT_LI_TERM_IND)
      {
         RLOG1(L_DEBUG, "Term Indication Recvd. lsap id[%d] FAILED",
               dgn->u.sapId);
      }
   } /* end of LCM_CATEGORY_INTERFACE */

 
   /* MEM LEAK FIX:  */
   WR_FREE(staInd, sizeof(CzMngmt));
   
   RETVALUE(ROK);
} /* end of SmMiLczStaInd() */

/*
 *
 *       Fun:    SmMiLczTrcInd - trace indication
 *
 *       Desc:   Received the Trace Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczTrcInd
(
Pst         *pst,
CzMngmt      *trcInd,
Buffer      *mBuf
)
#else
PUBLIC S16 SmMiLczTrcInd(pst, trcInd, mBuf)
Pst         *pst;
CzMngmt     *trcInd;
Buffer      *mBuf;
#endif /* ANSI */
{
   TRC2(SmMiLczTrcInd);

   RLOG0(L_DEBUG, "Received X2AP TrcInd");

   RETVALUE(ROK);
} /* end of SmMiLczTrcInd */

/*
 *
 *       Fun:    SmMiLczStaCfm - status confirm
 *
 *       Desc:   Received the Status Confirm
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczStaCfm
(
Pst         *pst,
CzMngmt      *staCfm
)
#else
PUBLIC S16 SmMiLczStaCfm(pst, staCfm)
Pst         *pst;
CzMngmt      *staCfm;
#endif /* ANSI */
{
   TRC2(SmMiLczStaCfm)

   RLOG0(L_DEBUG, "Received X2AP StaCfm");

   RETVALUE(ROK);
} /* end of SmMiLczStaCfm() */

/*
 *
 *       Fun:    SmMiLczStsCfm - statistics confirm
 *
 *       Desc:   Received the Statistics Confirm
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_cz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLczStsCfm
(
Pst         *pst,
CzMngmt      *stsCfm
)
#else
PUBLIC S16 SmMiLczStsCfm(pst, stsCfm)
Pst         *pst;
CzMngmt      *stsCfm;
#endif /* ANSI */
{
   TRC2(SmMiLczStsCfm);

   RLOG0(L_DEBUG, "Received X2AP StsCfm");

   RETVALUE(ROK);
} /* end of SmMiLczStsCfm() */

/*
*
*       Fun:   smBuildCzPeerCfg
*
*       Desc:  Call handler for Peer Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  wr_sm_cz_cntrl.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildAddNeighCfg
(
LwrMngmt *lwrMngmt
)
#else
PUBLIC S16 smBuildAddNeighCfg(lwrMngmt)
LwrMngmt *lwrMngmt; 
#endif
{
   /* ccpu00129256 */
   CzMngmt  *czMgt = NULLP;
   CzPeerLstCfg *cfg = NULLP;
   U16      cnt;
   Pst      pst;
 
   TRC2(smBuildAddNeighCfg)

   WRSM_ALLOC(&czMgt,sizeof(CzMngmt));
   if(NULLP == czMgt)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SM_SET_ZERO(&pst, sizeof(Pst));

   if(NULLP != czMgt)
   {
   cfg = &(czMgt->t.cfg.u.peerLst); 
   /* Due a bug in X2AP , noOfNghCfg must always be maxX2Peers -1 */
   cfg->nmbPeers = (U8)(smCfgCb.noOfNghCfg);
   /* TODO */
   cfg->nmbPeers = 1;
   /* kworks fix */
   /* below check is not required */
   WRSM_ALLOC(&cfg->peerCfg, sizeof(CzPeerCfg)* cfg->nmbPeers);

      if(NULLP == cfg->peerCfg)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

   for (cnt=0; cnt < cfg->nmbPeers; cnt++)
   {
#ifdef CZ_ENB   
      /*cfg->peerCfg[cnt].peerId   =  smCfgCb.wrNghCellCfg[cnt]->peerId; */
      /* TODO */
      cfg->peerCfg[cnt].peerId   =  1;/*TBD*/
      cfg->peerCfg[cnt].tSapId   =  0; 
      cfg->peerCfg[cnt].uSapId   =  0;  
      cfg->peerCfg[cnt].eAValue  =  UNKNOWN_VALUE;
      /* cfg->peerCfg[cnt].iPAValue =  0; */
      cfg->peerCfg[cnt].iPAValue =  UNKNOWN_VALUE;
      cfg->peerCfg[cnt].assocCfg.locOutStrms =  WRSM_LCZ_OUT_STRMS;
      wrCpyTptToNetAddr(&cfg->peerCfg[cnt].assocCfg.priDstAddr,
       &lwrMngmt->t.nbrAddReq.nbIpAddress);
      /*cfg->peerCfg[cnt].assocCfg.priDstAddr.u.ipv4NetAddr   =  smCfgCb.wrNghCellCfg[cnt]->enbAddr;*/
      cfg->peerCfg[cnt].assocCfg.dstAddrLst.nmb     =  1;
      wrCpyTptToNetAddr(&cfg->peerCfg[cnt].assocCfg.dstAddrLst.nAddr[0], 
       &lwrMngmt->t.nbrAddReq.nbIpAddress);
    
      /*cfg->peerCfg[cnt].assocCfg.dstAddrLst.nAddr[0].u.ipv4NetAddr   =  smCfgCb.wrNghCellCfg[cnt]->enbAddr;*/


      cfg->peerCfg[cnt].assocCfg.dstPort            =  WRSM_LCZ_ENB_SCTPORT;/*Peer eNB's port number.*/

#endif /* CZ_ENB */
  }

   /* Fill Header */
   czMgt->hdr.msgType             = TCFG;
   czMgt->hdr.msgLen              = 0;
   czMgt->hdr.entId.ent           = ENTCZ;
   czMgt->hdr.entId.inst          = SM_INST_ZERO;
   czMgt->hdr.elmId.elmnt         = STCZPEER;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTCZ;
   pst.dstProcId = SM_CZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region    = DFLT_REGION;
   pst.pool      = DFLT_POOL;
 
   RLOG0(L_DEBUG, "X2AP Peer Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLczCfgReq(&pst, czMgt);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
   }

   RETVALUE(ROK);
}/* smBuildAddNeighCfg*/ 


/********************************************************************30**

           End of file:     fc_smm_x2ap_brcm.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:52:37 2014

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
