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


File:     wr_smm_s1ap.c

     Sid:      fc_smm_s1ap_brcm.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:52:29 2014

Prg:      Sriky 

 *********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="SMM";

static int RLOG_FILE_ID=50;

static int RLOG_MODULE_ID=2;

#include "wr.h"
#include "wr_cmn.h"
#include "sct.h"
#include "lsz.h"
#include "lsz.x"
#include "wr_lwr.h"
#include "wr_smm_init.h"
#include "wr_kpi.h"
#include "wr_utils.h"

#ifdef RM_INTF 
#include "lrm.x"
#endif

#ifdef WR_RSYS_OAM  
EXTERN S16 smStartTmr ARGS((PTR cb, S16 tmrEvnt,U32 delay));
EXTERN S16 smStopTmr ARGS((PTR cb, S16 tmrEvnt));
EXTERN Void smStaticCfgFailAlarm ARGS((Void));
#endif
EXTERN U16 wrSmSctpPort;
EXTERN Bool wrEmmGetMmeStatus(U32 mmeId);


PRIVATE U8 wrSzCfg = 0;

/*
 *
 *       Fun:   Initialize S1AP task.
 *
 *       Desc:  Invoked to create S1AP Tapa task.
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *
 *       File:  wr_sm_sz_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smSzInit
(
 SSTskId    sysTskId
 )
#else
PUBLIC S16 smSzInit(sysTskId)
   SSTskId    sysTskId;
#endif
{
   TRC2(smSzInit)

      /* Register S1AP TAPA Task */
   if (SRegTTsk((Ent)ENTSZ, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR2,
               szActvInit, (ActvTsk)szActvTsk) != ROK) 
      {
         RETVALUE(RFAILED);
      }

   /* Attach S1AP TAPA Task */
   if (SAttachTTsk((Ent)ENTSZ, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   }

   RLOG1(L_INFO, "S1AP Tapa Task successfully registered and attached to %d",
         sysTskId);

   RETVALUE(ROK);
} /* end of smSzInit */

/*
 *
 *       Fun:    wrSmSzCfg - configure SZ 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_sm_sz_cntrl.c
 *
 */

#ifdef ANSI
PUBLIC Void wrSmSzCfg
(
 Void
 )
#else
PUBLIC Void wrSmSzCfg()
#endif /* ANSI */
{
   TRC2(wrSmSzCfg)
      wrSzCfg = 0;

#ifdef WR_RSYS_OAM  
   smStartTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR, SM_TMR_STATIC_CFG_TMR_VAL);
#endif

   smBuildSzGenCfg();
   smBuildSzProtCfg();
   smBuildSzLSapCfg(SZ_SB_SPID, SZ_SB_SUID);
   smBuildSzUSapCfg(WR_SZ_SPID);
   smBuildSzPeerCfg();

   RETVOID;
} /* end of wrSmSzCfg */


/*
 *
 *       Fun:   smBuildSzGenCfg
 *
 *       Desc:  fill in genCfg for SZ
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *              
 *       File:  wr_sm_sz_cntrl.c 
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildSzGenCfg
(
 Void
 )
#else
PUBLIC S16 smBuildSzGenCfg()
#endif
{
   SzMngmt  szMgt;
   SzGenCfg    *cfg = NULLP;
   Pst       pst;

   TRC2(smBuildSzGenCfg)

      SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(szMgt.u.cfg.u.gen);

   /*----------- Fill General Configuration Parameters ---------*/
   cfg->nmbSztSaps = (S16)WRSM_LSZ_MAX_SZTSAP;
   cfg->nmbSctSaps = (S16)WRSM_LSZ_MAX_SCTSAP;
   cfg->nmbConn = (U32)(WRSM_LSZ_MAX_PEER * (smCfgCb.maxUeSupp + WR_MAX_SR_CQI_NOT_CONFIGURED_UES));
   cfg->nmbPeer = (U32)WRSM_LSZ_MAX_PEER;
   cfg->timeRes = (S16)WRSM_LSZ_TIMERES; /* wr004.102 : klock warning fixed */

   cfg->lmPst.srcProcId = SM_SZ_PROC;
   cfg->lmPst.dstProcId = SM_SM_PROC;
   cfg->lmPst.srcEnt = (Ent)ENTSZ;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;
   cfg->lmPst.prior = (Prior)WRSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = SZ_MEM_REGION;
   cfg->lmPst.pool =  SZ_POOL;
   cfg->lmPst.selector = (Selector)WRSM_SZSMSEL;

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZGEN;
   szMgt.hdr.elmId.elmntInst1    = 0;
   szMgt.hdr.elmId.elmntInst2    = 0;
   szMgt.hdr.elmId.elmntInst3    = 0;
   szMgt.hdr.seqNmb              = 0;
   szMgt.hdr.version             = 0;
   szMgt.hdr.transId             = 0;

   szMgt.hdr.response.prior      = PRIOR0;
   szMgt.hdr.response.route      = RTESPEC;
   szMgt.hdr.response.mem.region = SZ_MEM_REGION;
   szMgt.hdr.response.mem.pool   = SZ_POOL;
   szMgt.hdr.response.selector   = SM_SELECTOR_LC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "S1AP Gen Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);

   RETVALUE(ROK);
}/* smBuildSzGenCfg */

/*
 *
 *       Fun:   smBuildSzLSapCfg
 *
 *       Desc:  Call handler for TSAP Configuration Request
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *              
 *       File:  wr_sm_sz_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildSzLSapCfg
(
 SuId        suId,             /* S1AP Sap ID */
 SpId        spId              /* Service Provider Sap ID */
 )
#else
PUBLIC S16 smBuildSzLSapCfg(suId, spId)
   SuId        suId;             /* S1AP Sap ID */
   SpId        spId;             /* Service Provider Sap ID */
#endif
{
   SzMngmt  szMgt;
   SzSctSapCfg  *cfg = NULLP;
   Pst       pst;

   TRC2(smBuildSzLSapCfg)

      SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg   = &(szMgt.u.cfg.u.sctSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->selector     = WRSM_SZSBSEL;
   cfg->mem.region   = SZ_MEM_REGION;
   cfg->mem.pool     = SZ_POOL;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->dstProcId    = SM_SB_PROC;
   cfg->dstEntId     = ENTSB;
   cfg->dstInstId    = (Inst)0;

   cfg->tBndTmr.enb = TRUE;
   cfg->tBndTmr.val = WRSM_LSZ_VAL_10;
   cfg->maxBndRetry = WRSM_LSZ_VAL_3;

#ifdef LSZV1
   /* wr004.102: Fill source address info */
   cfg->srcAddr.sctPort = wrSmSctpPort;
   WR_COPY_TPT_TO_NETADDR_ARR(cfg->srcAddr.ipAddr.nAddr,cfg->srcAddr.ipAddr.nmb,
      smCfgCb.enbIpAddr);
#endif /* LSZV1 */

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZSCTSAP;
   szMgt.hdr.response.mem.region = SZ_MEM_REGION;
   szMgt.hdr.response.mem.pool   = SZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "S1AP LSap Cfg sent");

   pst.region = smCb.init.region;
   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);

   RETVALUE(ROK);
}/* smBuildSzLSapCfg */


/*
 *
 *       Fun:   smBuildSzUSapCfg
 *
 *       Desc:  Call handler for SSAP Configuration Request
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *              
 *       File:  wr_sm_sz_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildSzUSapCfg
(
 SpId        spId              /* Service Provider Sap ID */
 )
#else
PUBLIC S16 smBuildSzUSapCfg(spId)
   SpId        spId;             /* Service Provider Sap ID */
#endif
{
   SzMngmt  szMgt;
   SzSztSapCfg    *cfg = NULLP;
   Pst      pst;

   TRC2(smBuildSzUSapCfg)

      SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(szMgt.u.cfg.u.sztSap);

   cfg->spId   = spId;
   cfg->selector     = WRSM_SZWRSEL;
   cfg->mem.region   = SZ_MEM_REGION;
   cfg->mem.pool     = SZ_POOL;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZSZTSAP;
   szMgt.hdr.response.mem.region = SZ_MEM_REGION;
   szMgt.hdr.response.mem.pool   = SZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "S1AP USap Cfg sent");

   pst.region = smCb.init.region;
   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);

   RETVALUE(ROK);
}/* smBuildSzUSapCfg */


/*
 *
 *       Fun:   smBuildSzProtCfg
 *
 *       Desc:  Call handler for Protocol Configuration Request
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *              
 *       File:  wr_sm_sz_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildSzProtCfg
(
 Void
 )
#else
PUBLIC S16 smBuildSzProtCfg()
#endif
{
   SzMngmt  szMgt;
   SzProtCfg *protCfg = NULLP;
   Pst      pst;

   TRC2(smBuildSzProtCfg)

      SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   protCfg = &(szMgt.u.cfg.u.prot);

#ifndef LSZV1   
   /* wr004.102: Compilation error fix */
   WR_COPY_TPT_TO_NETADDR_ARR(protCfg->srcAddr.ipAddr.nAddr,
     protCfg->srcAddr.ipAddr.nmb, smCfgCb.enbIpAddr); 
   protCfg->srcAddr.sctPort =   wrSmSctpPort;
#endif /* LSZV1 */
#ifdef SZ_ENB   
   protCfg->nodeType =   LSZ_NODE_ENB;
   protCfg->u.eNbId     =   WRSM_LSZ_ENB_PRC0;
#endif /* SZ_ENB */   

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZPROT;
   szMgt.hdr.response.mem.region = SZ_MEM_REGION;
   szMgt.hdr.response.mem.pool   = SZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "S1AP Protocol Cfg sent");

   pst.region = smCb.init.region;
   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);

   RETVALUE(ROK);
}/* smBuildSzProtCfg */

/*
 *
 *       Fun:   smBuildSzPeerCfg
 *
 *       Desc:  Call handler for Peer Configuration Request
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *              
 *       File:  wr_sm_sz_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildSzPeerCfg
(
 Void
 )
#else
PUBLIC S16 smBuildSzPeerCfg()
#endif
{
   SzMngmt  szMgt;
   SzPeerCfg *cfg = NULLP;
   SctNetAddrLst *addrLst = NULLP;
   LwrSmMmeCfg *mmeCfg = NULLP;
   U16      cnt;
   Pst      pst;
   U8       idx; 
   TRC2(smBuildSzPeerCfg)

      SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(szMgt.u.cfg.u.peer);

   cfg->nmbPeers = (U32)smCfgCb.noOfCfg;
   if(WR_VAL_ZERO == cfg->nmbPeers)
   {
      wrSzCfg |= WR_SZ_PEER_CFG;
      RLOG0(L_WARNING, "There are no S1AP Peer Cfg present now.");
      RETVALUE(ROK);
   }

   if(cfg->nmbPeers > LWR_SM_MAX_MMES)
   {
      wrSzCfg |= WR_SZ_PEER_CFG;
      RLOG0(L_WARNING, "There are more than cfged S1AP Peers present now.");
      RETVALUE(ROK);
   }

   WRSM_ALLOC(&cfg->peerInfo, sizeof(SzPeerInfo)* cfg->nmbPeers);
   /* wr004.102 : klock warning fixed */
   if(cfg->peerInfo == NULLP)
   {
      wrSzCfg |= WR_SZ_PEER_CFG;
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   } /* end of if statement */

   for (cnt=0; cnt < cfg->nmbPeers; cnt++)
   {
#ifdef SZ_ENB
      mmeCfg = smCfgCb.mmeCfg[cnt];    
      cfg->peerInfo[cnt].type      =  LSZ_NODE_MME;   
      cfg->peerInfo[cnt].peerId.pres   =  TRUE; 
      cfg->peerInfo[cnt].peerId.val    =  mmeCfg->mmeId; 
      cfg->peerInfo[cnt].uSapId     =  WR_SZ_SPID;  
      cfg->peerInfo[cnt].sapId     =  WR_SZ_SUID;  
      cfg->peerInfo[cnt].assocCfg.locOutStrms        =  WRSM_LSZ_OUT_STRMS;
      
      wrCpyTptToNetAddr(&cfg->peerInfo[cnt].assocCfg.priDstAddr, 
         &mmeCfg->mmeAddr[0]);
      addrLst = &cfg->peerInfo[cnt].assocCfg.dstAddrLst;
      addrLst->nmb     = mmeCfg->noOfIps;
      for(idx = 0; idx < (mmeCfg->noOfIps); idx++)
      {
         wrCpyTptToNetAddr(&addrLst->nAddr[idx], &mmeCfg->mmeAddr[idx]);
      }
      cfg->peerInfo[cnt].assocCfg.dstPort            =  wrSmSctpPort;
      cfg->peerInfo[cnt].assocCfg.nmbRetry           =  WRSM_LSZ_VAL_3;
      cfg->peerInfo[cnt].assocCfg.tmrAssocRecov.enb  =  TRUE;
      cfg->peerInfo[cnt].assocCfg.tmrAssocRecov.val  =  WRSM_LSZ_VAL_50;

#endif /* SZ_ENB */
   } 

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZPEER;
   szMgt.hdr.response.mem.region = SZ_MEM_REGION;
   szMgt.hdr.response.mem.pool   = SZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "S1AP Peer Cfg sent");

   pst.region = smCb.init.region;
   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);

   RETVALUE(ROK);
}/* smBuildSzPeerCfg */

/*
 *
 *       Fun:   smBuildDynSzPeerCfg
 *
 *       Desc:  Call handler for Peer Configuration Request
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *              
 *       File:  wr_sm_sz_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildDynSzPeerCfg
(
 U8 mmeIndex,
 U8 dbIndex
 )
#else
PUBLIC S16 smBuildDynSzPeerCfg(mmeIndex,dbIndex)
   U8 mmeIndex;
   U8 dbIndex;
#endif
{
   SzMngmt  szMgt;
   SzPeerCfg *cfg = NULLP;
   U16      cnt;
   Pst      pst;
   U8       idx = 0;

   TRC2(smBuildDynSzPeerCfg);

      SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cfg = &(szMgt.u.cfg.u.peer);

   cfg->nmbPeers = 1;

   WRSM_ALLOC(&cfg->peerInfo, sizeof(SzPeerInfo)* cfg->nmbPeers);
   /* wr004.102 : klock warning fixed */
   if(cfg->peerInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   } /* end of if statement */

   cnt = 0;

#ifdef SZ_ENB   
   cfg->peerInfo[cnt].type      =  LSZ_NODE_MME;   
   cfg->peerInfo[cnt].peerId.pres   =  TRUE; 
   cfg->peerInfo[cnt].peerId.val    =  smCfgCb.mmeCfg[mmeIndex -1]->mmeId; 
   cfg->peerInfo[cnt].uSapId     =  WR_SZ_SPID;  
   cfg->peerInfo[cnt].sapId     =  WR_SZ_SUID;  
   cfg->peerInfo[cnt].assocCfg.locOutStrms        =  WRSM_LSZ_OUT_STRMS;

   wrCpyTptToNetAddr(&cfg->peerInfo[cnt].assocCfg.priDstAddr, 
      &smCfgCb.mmeCfg[mmeIndex -1]->mmeAddr[0]);
   cfg->peerInfo[cnt].assocCfg.dstAddrLst.nmb     =  smCfgCb.mmeCfg[mmeIndex -1]->noOfIps;
   for (idx = 0; idx < cfg->peerInfo[cnt].assocCfg.dstAddrLst.nmb; idx++)
   {
      wrCpyTptToNetAddr(&cfg->peerInfo[cnt].assocCfg.dstAddrLst.nAddr[idx],
         &smCfgCb.mmeCfg[mmeIndex -1]->mmeAddr[idx]);
   }

   cfg->peerInfo[cnt].assocCfg.dstPort            =  wrSmSctpPort;
   cfg->peerInfo[cnt].assocCfg.nmbRetry           =  WRSM_LSZ_VAL_3;
   cfg->peerInfo[cnt].assocCfg.tmrAssocRecov.enb  =  TRUE;
   cfg->peerInfo[cnt].assocCfg.tmrAssocRecov.val  =  WRSM_LSZ_VAL_50;

#endif /* SZ_ENB */

   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZPEER;
   szMgt.hdr.response.mem.region = SZ_MEM_REGION;
   szMgt.hdr.response.mem.pool   = SZ_POOL;

   /* Filling OamDb index as trans Id */
   szMgt.hdr.transId = dbIndex;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "S1AP Peer Cfg  for Dynamic MME Configuration sent");

   pst.region = smCb.init.region;
   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&pst, &szMgt);

   RETVALUE(ROK);
}/* smBuildDynSzPeerCfg */

/*
 *      FUN:   smBldSzPeerDelCntrlReq
 *
 *      Desc:  Builds the delete peer control request and sends it to
 *             S1AP
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  wr_smm_s1ap.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBldSzPeerDelCntrlReq
(
 U8    peerId,
 U8    dbIndex
 )
#else
PUBLIC S16 smBldSzPeerDelCntrlReq()
   U8    peerId;
   U8    dbIndex;
#endif /* ANSI */
{
   SzMngmt  cntrl;
   Pst      pst;

   TRC2(smBldSzPeerDelCntrlReq)

      SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.u.cntrl.action        = ADEL;
   cntrl.u.cntrl.subAction    = SAELMNT;
   /* At present we are deleting only one peer at a time */
   cntrl.u.cntrl.u.peer.nmbPeers = 1;
   WRSM_ALLOC(&(cntrl.u.cntrl.u.peer.peerId), ((sizeof(TknU32)) * cntrl.u.cntrl.u.peer.nmbPeers));
   if(cntrl.u.cntrl.u.peer.peerId == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Fill the peerid value which needs to be deleted */
   cntrl.u.cntrl.u.peer.peerId[0].pres = PRSNT_NODEF;
   cntrl.u.cntrl.u.peer.peerId[0].val = peerId;

   /* Fill the index which will be used when we receive the confirmation
    * for the delete peer request */
   cntrl.hdr.transId             = dbIndex;
   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZPEER;
   cntrl.hdr.response.mem.region = SZ_MEM_REGION;
   cntrl.hdr.response.mem.pool   = SZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "Delete S1AP Peer Cfg sent for Dynamic MME Delete");

   /* Send the control request to the S1AP */
   (Void)SmMiLszCntrlReq(&pst, &cntrl);
   RETVALUE(ROK);
} /* end of smBldSzPeerDelCntrlReq */



/*
 *      FUN:   smBindUnBindSzToLSap
 *
 *      Desc:  Bind and unbind S1AP to lower SAP
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  wr_smm_s1ap.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 smBindUnBindSzToLSap
(
U8 action
 )
#else
PUBLIC S16 smBindUnBindSzToLSapp(action)
U8 action;
#endif /* ANSI */
{
   SzMngmt  cntrl;
   Pst      pst;
   S16      ret = ROK;

   TRC2(smBindUnBindSzToLSap)

      SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.u.cntrl.action        = action;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sap.id       = WR_SZ_SUID; 

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZSCTSAP;
   cntrl.hdr.response.mem.region = SZ_MEM_REGION;
   cntrl.hdr.response.mem.pool   = SZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   if (action == ABND_ENA)
   {
      RLOG0(L_DEBUG, "S1AP Cntrl Req to bind with lower sap");
   }
   else
   {
      RLOG0(L_DEBUG, "S1AP Cntrl Req to unbind with lower sap");
   }

   /* Send the control request to the S1AP */
   (Void)SmMiLszCntrlReq(&pst, &cntrl);

   RETVALUE(ret);
} /* end of smBindUnBindSzToLSap */

/*
 *
 *       Fun:   smBuildSzGenCntrl
 *
 *       Desc:  Invoked to Enable Alarms at S1AP
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *
 *       File:  wr_sm_sz_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildSzGenCntrl
(
)
#else
PUBLIC S16 smBuildSzGenCntrl(Void)
#endif
{
   SzMngmt cntrl;
   Pst      pst;

   TRC2(smBuildSzGenCntrl)

      SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.u.cntrl.action        =  AENA;
   cntrl.u.cntrl.u.sap.id      =  0;
   cntrl.u.cntrl.subAction     =  SAUSTA;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZGEN;
   cntrl.hdr.response.mem.region = SZ_MEM_REGION;
   cntrl.hdr.response.mem.pool   = SZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "S1AP Gen Cntrl Req to sz sent");

   /* Send the request to the S1AP */
   SmMiLszCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildSzGenCntrl */

/**
 * @brief This function is used to send enable/disable control request to 
 * the S1AP layers.
 *
 * @details
 *
 * Function: smBuildSzLogCntrl
 *
 * @param[in] actType
 * @param[in] mask
 *
 * @return Void
 */

#ifdef ANSI
PUBLIC S16 smBuildSzLogCntrl
(
 Bool actType,
 U32 mask
 )
#else
PUBLIC S16 smBuildSzLogCntrl(actType, mask)
   Bool actType;
   U32 mask;
#endif
{
   SzMngmt cntrl;
   Pst      pst;

   TRC2(smBuildSzLogCntrl)

      SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   if (actType == WR_ON)
   {
      cntrl.u.cntrl.action        =  AENA;
   }
   else
   {
      cntrl.u.cntrl.action        =  ADISIMM;
   }
   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZGEN;
   cntrl.hdr.response.mem.region = SZ_MEM_REGION;
   cntrl.hdr.response.mem.pool   = SZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   /* Send the request to the S1AP */
   SmMiLszCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildSzLogCntrl */ 


/*
 *
 *       Fun:   smBuildSzDbgCntrl
 *
 *       Desc:  Invoked to Enable Debugs at S1AP
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *
 *       File:  wr_sm_sz_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildSzDbgCntrl
(
)
#else
PUBLIC S16 smBuildSzDbgCntrl(Void)
#endif
{
   SzMngmt cntrl;
   Pst      pst;

   TRC2(smBuildSzDbgCntrl)

      SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.u.cntrl.action        =  AENA;
   cntrl.u.cntrl.subAction     =  SADBG;
#ifdef DEBUGP
   cntrl.u.cntrl.u.dbg.dbgMask =  0xffffffff;
#endif

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZGEN;
   cntrl.hdr.response.mem.region = SZ_MEM_REGION;
   cntrl.hdr.response.mem.pool   = SZ_POOL;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.region = smCb.init.region;

   RLOG0(L_DEBUG, "S1AP Dbg Cntrl Req to sz sent");

   /* Send the request to the S1AP */
   SmMiLszCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildSzGenCntrl */

/* IPSec Recovery */
/*
 *
 *       Fun:   smBuildSzShutDownCntrl
 *
 *       Desc:  Invoked to shut down S1AP
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *
 *       File:  wr_sm_sz_cntrl.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildSzShutDownCntrl
(
)
#else
PUBLIC S16 smBuildSzShutDownCntrl(Void)
#endif
{
   SzMngmt cntrl;
   Pst      pst;

   TRC2(smBuildSzShutDownCntrl)

      SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl.u.cntrl.action        =  ASHUTDOWN;
   /* cntrl.u.cntrl.u.sap.id      =  0; */ /* Needed? */
   cntrl.u.cntrl.subAction     =  NULLD;

   /* Fill Header */
   cntrl.hdr.msgType             = TCNTRL;
   cntrl.hdr.entId.ent           = ENTSZ;
   cntrl.hdr.entId.inst          = SM_INST_ZERO;
   cntrl.hdr.elmId.elmnt         = STSZGEN;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "S1AP shut down Cntrl Req to sz sent");

   /* Send the request to the S1AP */
   SmMiLszCntrlReq(&pst, &cntrl);

   RETVALUE(ROK);
} /* end of smBuildSzGenCntrl */

/* IPSec Recovery */


/*
 *
 *       Fun:    activate task
 *
 *       Desc:   Processes received event from SZ
 *
 *       Ret:    ROK  - ok
 *
 *       Notes:  None
 *
 *       File:   wr_sm_sz_exms.c
 *
 */

#ifdef ANSI
PUBLIC S16 smSzActvTsk
(
 Pst *pst,                   /* post */
 Buffer *mBuf                /* message buffer */
 )
#else
PUBLIC S16 smSzActvTsk(pst, mBuf)
   Pst *pst;                   /* post */
   Buffer *mBuf;               /* message buffer */
#endif
{
   S16 ret;

   TRC2(smSzActvTsk)

      ret = ROK;

   switch(pst->event)
   {
#ifdef LCSMMILSZ
      case EVTLSZCFGCFM:             /* Config confirm */
         ret = cmUnpkLszCfgCfm(SmMiLszCfgCfm, pst, mBuf);
         break;
      case EVTLSZCNTRLCFM:           /* Control confirm */
         ret = cmUnpkLszCntrlCfm(SmMiLszCntrlCfm, pst, mBuf);
         break;
      case EVTLSZSTAIND:             /* Status Indication */
         ret = cmUnpkLszStaInd(SmMiLszStaInd, pst, mBuf);
         break;
      case EVTLSZSTSCFM: 
         ret = cmUnpkLszStsCfm(SmMiLszStsCfm, pst, mBuf);
         break;
#endif
      default:
         WR_FREEMBUF(mBuf);
         ret = RFAILED;
         RLOG1(L_ERROR, "  Received invalid event %d ", pst->event);
         break;
   }

   RETVALUE(ret);

} /* end of smSzActvTsk */

/*local defines */
#define SM_MAX_SZ_SEL 4


/*local typedefs */

/*forward references */
PUBLIC S16 PtMiLszCfgReq      (Pst *post, SzMngmt *cfg);
PUBLIC S16 PtMiLszStsReq      (Pst *post, Action action, SzMngmt *sts);
PUBLIC S16 PtMiLszStaReq      (Pst *post, SzMngmt *ssta);
PUBLIC S16 PtMiLszCntrlReq    (Pst *post, SzMngmt *cntrl);

/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */

/* Following matrices define the mapping between the primitives called by the
 * layer manager (as SmMiLszXXXReq) and the corresponding primitives of the 
 * S1AP layer (layer management service provider) (as SzMiLszXXXReq).
 * Each primitive is mapped to one of SM_MAX_SZ_SEL functions in the array.
 * The mapping is based on post->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCSMMILSZ    loose         cmPkMiLszXXXReq
 * 1                 MG          tight         SzMiLszXXXReq
 * 2+                            tight         PtMiLszXXXReq
 */

PRIVATE LszCfgReq smMiLszCfgReqMt[] =
{
#ifdef LCSMMILSZ
   cmPkLszCfgReq,
#else
   PtMiLszCfgReq,
#endif

#ifdef SZ
   SzMiLszCfgReq,
#else
   PtMiLszCfgReq,
#endif

};


PRIVATE LszStsReq smMiLszStsReqMt[] =
{
#ifdef LCSMMILSZ
   cmPkLszStsReq,
#else
   PtMiLszStsReq,
#endif

#ifdef SZ
   SzMiLszStsReq,
#else
   PtMiLszStsReq,
#endif

};


PRIVATE LszStaReq smMiLszStaReqMt[] =
{
#ifdef LCSMMILSZ
   cmPkLszStaReq,
#else
   PtMiLszStaReq,
#endif

#ifdef SZ
   SzMiLszStaReq,
#else
   PtMiLszStaReq,
#endif

};


PRIVATE LszCntrlReq smMiLszCntrlReqMt[] =
{
#ifdef LCSMMILSZ
   cmPkLszCntrlReq,
#else
   PtMiLszCntrlReq,
#endif

#ifdef SZ
   SzMiLszCntrlReq,
#else
   PtMiLszCntrlReq,
#endif

};


/*
 *
 *       Fun:    SmMiLszCfgReq - Configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszCfgReq
(
 Pst     *post,       /* post structure */
 SzMngmt *cfg        /* management structure */
 )
#else
PUBLIC S16 SmMiLszCfgReq(post, cfg)
   Pst     *post;       /* post structure */
   SzMngmt *cfg;       /* management structure */
#endif
{

   TRC3(SmMiLszCfgReq)

#if (ERRCLASS & ERRCLS_DEBUG)
      if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
      {
         RETVALUE(RFAILED);
      }
#endif

   RETVALUE((*(smMiLszCfgReqMt[post->selector]))(post, cfg));
} /* end of SmMiLszCfgReq() */

/*
 *
 *       Fun:    SmMiLszStsReq - Statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStsReq
(
 Pst     *post,       /* post structure */
 Action action,
 SzMngmt *sts        /* management structure */
 )
#else
PUBLIC S16 SmMiLszStsReq(post, action, sts)
   Pst     *post;       /* post structure */
   Action action;
   SzMngmt *sts;       /* management structure */
#endif
{
   TRC3(SmMiLszStsReq)

#if (ERRCLASS & ERRCLS_DEBUG)
      if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
      {
         RETVALUE(RFAILED);
      }
#endif

   RETVALUE((*(smMiLszStsReqMt[post->selector]))(post, action, sts));
} /* end of SmMiLszStsReq() */

/*
 *
 *       Fun:    SmMiLszStaReq - Status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStaReq
(
 Pst     *post,       /* post structure */
 SzMngmt *ssta        /* management structure */
 )
#else
PUBLIC S16 SmMiLszStaReq(post, ssta)
   Pst     *post;       /* post structure */
   SzMngmt *ssta;       /* management structure */
#endif
{
   TRC3(SmMiLszStaReq)

#if (ERRCLASS & ERRCLS_DEBUG)
      if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
      {
         RETVALUE(RFAILED);
      }
#endif

   RETVALUE((*(smMiLszStaReqMt[post->selector]))(post, ssta));
} /* end of SmMiLszStaReq() */

/*
 *
 *       Fun:    SmMiLszCntrlReq - Control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszCntrlReq
(
 Pst     *post,       /* post structure */
 SzMngmt *cntrl        /* management structure */
 )
#else
PUBLIC S16 SmMiLszCntrlReq(post, cntrl)
   Pst     *post;       /* post structure */
   SzMngmt *cntrl;       /* management structure */
#endif
{
   TRC3(SmMiLszCntrlReq)

#if (ERRCLASS & ERRCLS_DEBUG)
      if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
      {
         RETVALUE(RFAILED);
      }
#endif

   RETVALUE((*(smMiLszCntrlReqMt[post->selector]))(post, cntrl));
} /* end of SmMiLszCntrlReq() */

/*
 *
 *       Fun:    PtMiLszCfgReq - portable configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszCfgReq
(
 Pst     *post,       /* post structure */
 SzMngmt *cfg        /* management structure */
 )
#else
PUBLIC S16 PtMiLszCfgReq(post, cfg)
   Pst     *post;       /* post structure */
   SzMngmt *cfg;       /* management structure */
#endif
{
   TRC3(PtMiLszCfgReq)

      UNUSED(post);
   UNUSED(cfg);
   RETVALUE(RFAILED);
} /* end of PtMiLszCfgReq() */

/*
 *
 *       Fun:    PtMiLszStsReq - portable statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszStsReq
(
 Pst     *post,       /* post structure */
 Action action,
 SzMngmt *sts        /* management structure */
 )
#else
PUBLIC S16 PtMiLszStsReq(post, action, sts)
   Pst     *post;       /* post structure */
   Action action;
   SzMngmt *sts;       /* management structure */
#endif
{
   TRC3(PtMiLszStsReq)

      UNUSED(post);
   UNUSED(action);
   UNUSED(sts);
   RETVALUE(RFAILED);
} /* end of PtMiLszStsReq() */

/*
 *
 *       Fun:    PtMiLszStaReq - portable status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszStaReq
(
 Pst     *post,       /* post structure */
 SzMngmt *ssta        /* management structure */
 )
#else
PUBLIC S16 PtMiLszStaReq(post, ssta)
   Pst     *post;       /* post structure */
   SzMngmt *ssta;       /* management structure */
#endif
{
   TRC3(PtMiLszStaReq)

      UNUSED(post);
   UNUSED(ssta);
   RETVALUE(RFAILED);
} /* end of PtMiLszStaReq() */

/*
 *
 *       Fun:    PtMiLszCntrlReq - portable control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszCntrlReq
(
 Pst     *post,       /* post structure */
 SzMngmt *cntrl        /* management structure */
 )
#else
PUBLIC S16 PtMiLszCntrlReq(post, cntrl)
   Pst     *post;       /* post structure */
   SzMngmt *cntrl;       /* management structure */
#endif
{
   TRC3(PtMiLszCntrlReq)

      UNUSED(post);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
} /* end of PtMiLszCntrlReq() */

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
 *       File:  wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszCfgCfm
(
 Pst     *pst,          /* post structure */
 SzMngmt *cfm           /* configuration */
 )
#else
PUBLIC S16 SmMiLszCfgCfm(pst, cfm)
   Pst     *pst;          /* post structure */
   SzMngmt *cfm;          /* configuration */
#endif
{
   Pst      szPst;

   TRC2(SmMiLszCfgCfm);

   SM_SET_ZERO(&szPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STSZGEN:
            wrSzCfg |= WR_SZ_GEN_CFG;
            break;      
         case STSZPROT:
            wrSzCfg |= WR_SZ_PROT_CFG;
            break;
         case STSZSZTSAP:
            wrSzCfg |= WR_SZ_SZT_SAP_CFG;
            break;
         case STSZSCTSAP:
            wrSzCfg |= WR_SZ_SCT_SAP_CFG;
            break;
         case STSZPEER:
            wrSzCfg |= WR_SZ_PEER_CFG;
            break;
         default:
            break;
      } /* end of switch statement */

   RLOG1(L_DEBUG, "S1AP Cfg Cfm received for the element %d",
         cfm->hdr.elmId.elmnt);

      if (wrSzCfg == WR_SM_SZ_CONFIGURED)
      {
#ifdef WR_RSYS_OAM
         if(smCb.smState == WR_SM_STATE_OAM_DYN_START)
         {
            wrSendDynAddMmeCfg(cfm->hdr.transId);
#ifdef RM_INTF /*clear mme context at rrm*/
            smSndRmuProtDynMmeCfg(cfm->hdr.transId, LRM_CFG_ADD);
#endif
            RETVALUE(ROK);
         }
#endif
            smCb.smState = WR_SM_STATE_S1AP_CFG_DONE;
         RLOG0(L_ALWAYS, "S1AP CONFIGURATION SUCCESSFULLY PERFORMED.");
#ifdef WR_RSYS_OAM  
            smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
#endif
            WR_FILL_PST(szPst, SM_SZ_PROC, ENTSM, ENTWR, EVTWREGTPCFG, WR_POOL, SM_SELECTOR_LC);

            if(wrSmSelfPstMsg(&szPst) != ROK)
            {
            RLOG0(L_FATAL, "Self Pst failed after S1AP Configuration.");
            }
            else
            {
            RLOG0(L_DEBUG, "Self Pst success after S1AP Configuration.");
            } /* end of else part */
         }
      }  /* end of wrSzCfg == WR_SM_SZ_CONFIGURED */
   else
   {
      RLOG0(L_ERROR," S1AP Configuration confirm not OK");

#ifdef WR_RSYS_OAM  
      smStopTmr((PTR)&(smCb),SM_TMR_STATIC_CFG_TMR);
      smCb.cfgCb.alarm.severity  = SM_ALARM_SEVERITY_MAJOR;
      smCb.cfgCb.alarm.causeType = SM_ALARM_CAUSE_TYPE_ERR_IND;
      smCb.cfgCb.alarm.causeVal  = SM_ALARM_CAUSE_VAL_INIT_FAIL;
      smStaticCfgFailAlarm();
#endif
   }

   RETVALUE(ROK);
} /* end of SmMiLszCfgCfm */

/*
 *
 *       Fun:    SmMiLszCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszCntrlCfm
(
 Pst         *pst,
 SzMngmt     *cntrl
 )
#else
PUBLIC S16 SmMiLszCntrlCfm(pst, cntrl)
   Pst         *pst;
   SzMngmt     *cntrl;
#endif /* ANSI */
{
   Pst  szPst;

   TRC2(SmMiLszCntrlCfm);

   SM_SET_ZERO(&szPst, sizeof(Pst));
   /* IPSec */
   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
      if (smCb.smState == WR_SM_STATE_X2AP_SHUTDWN_DONE)
      {
         smCb.smState = WR_SM_STATE_S1AP_SHUTDWN_DONE;
         RLOG0(L_ALWAYS, "S1AP SHUTDOWN SUCCESSFULLY PERFORMED.");
         WR_FILL_PST(szPst, SM_SZ_PROC, ENTSM, ENTWR, EVTWRSCTPSHUT, WR_POOL, SM_SELECTOR_LC);

         if(wrSmSelfPstMsg(&szPst) != ROK)
         {
            RLOG0(L_FATAL, "Self Pst failed after S1AP SHUTDOWN Configuration.");
         }
         else
         {
            RLOG0(L_DEBUG, "Self Pst success after S1AP SHUTDOWN Configuration.");
         } /* end of else part */
         RETVALUE(ROK);
      }
   }

   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
      if(cntrl->hdr.elmId.elmnt == STSZGEN)
      {
         if((smCfgCb.szDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_SZ_DBG_MASK))
         {
            /* Update DbgMsk */
            smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | WR_SM_SZ_DBG_MASK);
            /* Trigger Debug Cntrl */
            wrSmHdlEnbDebugs(EVTWRENBDEBUGS);
         }
      }

   } /* end of if statement */
   /* Initiate control request for binding App to S1AP */
   if(cntrl->hdr.elmId.elmnt == STSZSCTSAP)
   {
      /* Due to unbind */
      if (smCb.smState == WR_SM_STATE_EGTP_STK_UNBND_DONE)
      {
         if (cntrl->cfm.status == LCM_PRIM_OK)
         {
            RLOG0(L_DEBUG,"UNBIND OF S1AP WITH SCTP SUCCESSFUL");
            /* Unbind SCTP with TUCL */
            smBindUnBindSbToLSap(SB_HI_SPID,AUBND);
         }
         RETVALUE(ROK);
      }
      else
      {
         RLOG0(L_INFO, "Bind of S1AP LSap with SCTP is in progress");
      }
   } /* end of if statement */

   if ( STSZPEER == cntrl->hdr.elmId.elmnt)
   {
#ifdef WR_RSYS_OAM
      if((smCb.smState == WR_SM_STATE_OAM_DYN_START) && (LCM_PRIM_OK == cntrl->cfm.status))
      {
         /*clean mme context at enb application*/
         wrSendDynMmeDelcfg(cntrl->hdr.transId);
#ifdef RM_INTF /*clear mme context at rrm*/
         smSndRmuProtDynMmeCfg(cntrl->hdr.transId, LRM_CFG_DEL);
#endif
         RETVALUE(ROK);
      }
#endif/*end of WR_RSYS_OAM*/
   }

   RETVALUE(ROK);
} /* end of SmMiLszCntrlCfm() */

/*
 *
 *       Fun:    SmMiLszStaInd - status indication
 *
 *       Desc:   Received the Status Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStaInd
(
 Pst         *pst,
 SzMngmt      *staInd
 )
#else
PUBLIC S16 SmMiLszStaInd(pst, staInd)
   Pst         *pst;
   SzMngmt      *staInd;
#endif /* ANSI */
{
   Pst  szPst;
   SzUsta  *usta= NULLP;
   SzUstaDgn *dgn;

   TRC2(SmMiLszStaInd)

      SM_SET_ZERO(&szPst, sizeof(Pst));
   usta = &(staInd->u.usta);
   dgn = &(usta->dgn);

   if (usta->alarm.event == LSZ_EVENT_LI_ASSOC_CFM  
         && usta->alarm.cause == LSZ_CAUSE_ASSOC_UP) 
   {
#ifdef WR_RSYS_OAM
      if(smCb.smState != WR_SM_STATE_OAM_DYN_START)
      {
         /* CNM_DEV */
         smCb.smState = WR_SM_STATE_AWAIT_SYNC;
         //smCb.smState = WR_SM_STATE_AWAIT_CELL_UP;
      }
#else
      {
         /* CNM_DEV */
         smCb.smState = WR_SM_STATE_AWAIT_SYNC;
         //smCb.smState = WR_SM_STATE_AWAIT_CELL_UP;
      }
#endif /* WR_RSYS_OAM */
      RLOG0(L_ALWAYS, "S1AP ASSOCIATION SUCCESSFULLY ESTABLISHED.");
   } /* end of if statement */

   else if((usta->alarm.event == LSZ_EVENT_LI_TERM_CFM) &&
         (usta->dgn.type == LSZ_USTA_DGNVAL_PEER) &&
         (usta->alarm.cause == LSZ_CAUSE_PEER_DELETED))
   {
      RLOG1(L_DEBUG, "Term Cfm Recvd. peer id[%ld]", dgn->u.peerId.val);
      /*127802*/
      RLOG1(L_DEBUG, "Received the ALARM for del peer for ID %ld",
            usta->dgn.u.peerId.val);
#ifdef WR_RSYS_OAM
      if(smCb.smState == WR_SM_STATE_OAM_DYN_START)
      {
         wrSendDynMmeDelcfg(usta->dgn.u.peerId.val - 1);
         RETVALUE(ROK);
      }
#endif
      /*Check if any add request is pending and send the ADD Request*/
   }
   else if (usta->alarm.category == LCM_CATEGORY_INTERFACE)
   {
      if (usta->alarm.event == LCM_EVENT_BND_OK)
      {
         RLOG1(L_DEBUG, "BIND OF S1AP lsap id[%d] IS SUCCESSFUL", dgn->u.sapId);
         smBindUnBindWrToSztSap(ABND_ENA); 
      }
      else if (usta->alarm.event == LCM_EVENT_BND_FAIL) 
      {
         RLOG1(L_FATAL, "BIND OF S1AP lsap id[%d] FAILED", dgn->u.sapId);
      }
      else if (usta->alarm.event == LSZ_EVENT_LI_TERM_IND)
      {
         RLOG1(L_ERROR, "Term Indication Recvd. lsap id[%d] FAILED",
               dgn->u.sapId);
         /* Updating Stack Manager's state */
         smCb.smState = WR_SM_STATE_AWAIT_S1_CON;
      }
   } /* end of LCM_CATEGORY_INTERFACE */



   RETVALUE(ROK);
} /* end of SmMiLszStaInd() */

/*
 *
 *       Fun:    SmMiLszTrcInd - trace indication
 *
 *       Desc:   Received the Trace Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszTrcInd
(
 Pst         *pst,
 SzMngmt      *trcInd,
 Buffer      *mBuf
 )
#else
PUBLIC S16 SmMiLszTrcInd(pst, trcInd, mBuf)
   Pst         *pst;
   SzMngmt     *trcInd;
   Buffer      *mBuf;
#endif /* ANSI */
{
   TRC2(SmMiLszTrcInd);
   RLOG0(L_DEBUG, "Received S1AP TrcInd");
   RETVALUE(ROK);
} /* end of SmMiLszTrcInd */

/*
 *
 *       Fun:    SmMiLszStaCfm - status confirm
 *
 *       Desc:   Received the Status Confirm
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStaCfm
(
 Pst         *pst,
 SzMngmt      *staCfm
 )
#else
PUBLIC S16 SmMiLszStaCfm(pst, staCfm)
   Pst         *pst;
   SzMngmt      *staCfm;
#endif /* ANSI */
{
   TRC2(SmMiLszStaCfm)

      RLOG0(L_DEBUG, "Received S1AP StaCfm");

   RETVALUE(ROK);
} /* end of SmMiLszStaCfm() */
/*
 *
 *       Fun:   smBuildSzStatReq
 *
 *       Desc:  fill in genCfg for SZ
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *              
 *       File:  wr_sm_sz_cntrl.c 
 *
 */
#ifdef ANSI
PRIVATE S16 smBuildSzStatReq 
(
 U32 peerId
 )
#else
PRIVATE S16 smBuildSzStatReq(peerId)
   U32 peerId
#endif
{
   SzMngmt        szMgt;
   SzPeerNodeSts *node;
   Pst            pst;

   TRC2(smBuildSzGenCfg)

      SM_SET_ZERO(&szMgt, sizeof(SzMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   node = &szMgt.u.sts.u.node;

   node->peerId.pres   =  TRUE; 
   node->peerId.val    =  peerId; 
   /* Fill Header */
   szMgt.hdr.msgType             = TCFG;
   szMgt.hdr.msgLen              = 0;
   szMgt.hdr.entId.ent           = ENTSZ;
   szMgt.hdr.entId.inst          = SM_INST_ZERO;
   szMgt.hdr.elmId.elmnt         = STSZPEER;
   szMgt.hdr.elmId.elmntInst1    = 0;
   szMgt.hdr.elmId.elmntInst2    = 0;
   szMgt.hdr.elmId.elmntInst3    = 0;
   szMgt.hdr.seqNmb              = 0;
   szMgt.hdr.version             = 0;
   szMgt.hdr.transId             = 0;

   szMgt.hdr.response.prior      = PRIOR0;
   szMgt.hdr.response.route      = RTESPEC;
   szMgt.hdr.response.mem.region = DFLT_REGION;
   szMgt.hdr.response.mem.pool   = SZ_POOL;
   szMgt.hdr.response.selector   = SM_SELECTOR_LC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTSZ;
   pst.dstProcId = SM_SZ_PROC;
   pst.srcProcId = SM_SM_PROC;

   RLOG0(L_DEBUG, "S1AP Gen Cfg sent");

   /* Send the request to the LM */
   (Void) SmMiLszStsReq(&pst, ZEROSTS, &szMgt);

   RETVALUE(ROK);
}/* smBuildSzStatReq*/

/*
 *
 *       Fun:    SmMiLszStsCfm - statistic indication
 *
 *       Desc:   Received the statistic Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_sz_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStsCfm
(
 Pst         *pst,
 SzMngmt      *staInd
 )
#else
PUBLIC S16 SmMiLszStsCfm(pst, staInd)
   Pst         *pst;
   SzMngmt      *staInd;
#endif /* ANSI */
{
   SzErrSts *errst;
   errst = &staInd->u.sts.u.node.cfm.errSts;
   U32 cause = 0;
   U32 val = 0;

   if(errst->protErrSts.transferSyntaxErr != 0)
   {
      cause = SztCauseRadioNwunknown_mme_ue_s1ap_idEnum;
      val = errst->protErrSts.transferSyntaxErr;
      WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, cause, val);
   }
   if(errst->protErrSts.absSyntaxErrRej != 0)
   {
      cause = SztCauseProtabstract_syntax_error_rejectEnum;
      val = errst->protErrSts.absSyntaxErrRej;
      WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, cause, val);
   }
   if(errst->protErrSts.absSyntaxErrIgnAndNtfy != 0)
   {
      cause = SztCauseProtabstract_syntax_error_ignore_and_notifyEnum;
      val = errst->protErrSts.absSyntaxErrIgnAndNtfy;
      WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, cause, val);
   }
   if(errst->protErrSts.msgNotCompWithRcvrState != 0)
   {
      cause = SztCauseProtmessage_not_compatible_with_receiver_stateEnum;
      val = errst->protErrSts.msgNotCompWithRcvrState;
      WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, cause, val);
   }
   if(errst->protErrSts.semanticErr != 0)
   {
      cause = SztCauseProtsemantic_errorEnum;
      val = errst->protErrSts.semanticErr;
      WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, cause, val);
   }
   if(errst->protErrSts.absSyntaxErrFalslyConsMsg != 0)
   {
      cause = SztCauseProtabstract_syntax_error_falsely_constructed_messageEnum;
      val = errst->protErrSts.absSyntaxErrFalslyConsMsg;
      WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, cause, val);
   }
   if(errst->protErrSts.protunspecified != 0)
   {
      cause = SztCauseProtunspecifiedEnum;
      val = errst->protErrSts.protunspecified;
      WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, cause, val);
   }

   if(errst->radioNwErrSts.unknownMmeUeS1apId != 0)
   {
      cause = SztCauseRadioNwunknown_mme_ue_s1ap_idEnum;
      val = errst->radioNwErrSts.unknownMmeUeS1apId;
      WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, cause, val);
   }
   if(errst->radioNwErrSts.unknownEnbUeS1apId != 0)
   {
      cause = SztCauseRadioNwunknown_enb_ue_s1ap_idEnum;
      val = errst->radioNwErrSts.unknownEnbUeS1apId;
      WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, cause, val);
   }
   if(errst->radioNwErrSts.unknownPairUeS1apId != 0)
   {
      cause = SztCauseRadioNwunknown_pair_ue_s1ap_idEnum;
      val = errst->radioNwErrSts.unknownPairUeS1apId;
      WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW, cause, val);
   }

   RETVALUE(ROK);
}
/*
 *
 *       Fun:    smGetS1apStats- statistic indication
 *
 *       Desc:   Received the statistic Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_sm_sz_ptmi.c
 *
 */

PUBLIC Void smGetS1apStats()
{
  U16 idx = 0;
  for(idx = 0; idx < smCfgCb.noOfCfg; idx++)
  {
    if(wrEmmGetMmeStatus(smCfgCb.mmeCfg[idx]->mmeId) == TRUE)
    {
      smBuildSzStatReq(smCfgCb.mmeCfg[idx]->mmeId);
    }
  }
}/*smGetS1apStats*/



/********************************************************************30**

           End of file:     fc_smm_s1ap_brcm.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:52:29 2014

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
