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




/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Layer Manager Interface Module 
  
     File:     rg_lmm.c 
  
     Sid:      gk_lmm.c@@/main/3 - Sat Jul 30 02:21:30 2011
  
     Prg:      sm
  
**********************************************************************/

/** @file rg_lmm.c
@brief This file contains the Layer Management interface module implementation.
       The functions for the configuration, control, status and statistics 
       request primitives are defined here.
*/
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=234;

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE Defines */
#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "tfu.h"           /* RGU Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */
#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* system services */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE Defines */
#include "crg.x"           /* CRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* RGU Interface includes */
#include "rg_sch_inf.x"           /* RGR Interface includes */
#include "lrg.x"           /* LRG Interface includes */
#include "rg.x"            /* MAC includes */
#ifdef SS_DIAG
#include "ss_diag.h"        /* Common log file */
#endif
#ifdef SS_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
EXTERN Void rgGetSId ARGS((SystemId *s));
#ifdef __cplusplus
}
#endif /* __cplusplus */


/* forward references */
PRIVATE U16 rgLMMGenCfg ARGS((
   RgCfg          *cfg           
));

PRIVATE U16 rgLMMSapCfg ARGS((
   RgCfg          *cfg,
   Elmnt          sapType
));

PRIVATE Void rgLMMShutdown ARGS((Void
));

PRIVATE Void rgLMMFillCfmPst ARGS((
   Pst           *reqPst,
   Pst           *cfmPst,
   RgMngmt       *cfm
));

PRIVATE Void rgLMMGenCntrl ARGS((
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
));

PRIVATE Void rgLMMSapCntrl ARGS((
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
));

#if 1

PUBLIC Void rgPrintfSubFrameInfo(RgDlSf *dlSf)
{

   if(dlSf->txDone == FALSE)
   {
      printf("Processing subframe\n");

      if (dlSf->bch.tb != NULLP)
      {
         printf("BCH TB PRESENT\n");
      }

      if (dlSf->bcch.tb != NULLP)
      {
         printf("BCCH TB PRESENT\n");
      }
      if (dlSf->pcch.tb !=
            NULLP)
      {
         printf("PCCH TB PRESENT\n");
      }

      printf("%d RAR present\n",dlSf->numRaRsp);
      if(dlSf->tbs.count)
      {
         printf("%lu TBs present\n", dlSf->tbs.count);
      }
   }
}


PUBLIC void printMacCellInfo(void)
{
   RgCellCb   *cellCbPrev = NULLP;
   RgCellCb   *cellCbCurr = NULLP;
   U8 idx;
   U8 cellIdx = 0;


   while (NULLP != (cellCbCurr = rgDBMGetNextCellCb(cellCbPrev)))
   {
      cellIdx++;
      printf("CELL :%d\n",cellIdx);


      for(idx=0;idx < RG_NUM_SUB_FRAMES; idx++)
      {
         rgPrintfSubFrameInfo(&cellCbCurr->subFrms[idx]);
      }

      printf("NUM UEs present:%d\n",cellCbCurr->ueLst.nmbEnt);
       cellCbPrev = cellCbCurr;
   }

   RETVOID;

}



#endif

/**
 * @brief Task Initiation callback function. 
 *
 * @details
 *
 *     Function : rgActvInit
 *     
 *     This function is supplied as one of parameters during MAC's 
 *     task registration. SSI will invoke this function once, after
 *     it creates and attaches this TAPA Task to a system task.
 *     
 *  @param[in]  Ent entity, the entity ID of this task.     
 *  @param[in]  Inst inst, the instance ID of this task.
 *  @param[in]  Region region, the region ID registered for memory 
 *              usage of this task.
 *  @param[in]  Reason reason.
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 rgActvInit(entity, inst, region, reason)
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
{
   TRC2(rgActvInit);

   /* Initialize the MAC TskInit structure to zero */
   cmMemset ((U8 *)&rgCb, 0, sizeof(RgCb));

   /* Initialize the MAC TskInit with received values */
   rgCb.rgInit.ent = entity;
   rgCb.rgInit.inst = inst;
   rgCb.rgInit.region = region;
   rgCb.rgInit.pool = 0;
   rgCb.rgInit.reason = reason;
   rgCb.rgInit.cfgDone = FALSE;
   rgCb.rgInit.acnt = FALSE;
   rgCb.rgInit.usta = FALSE;
   rgCb.rgInit.trc = FALSE;
   rgCb.trcLen = 0; 
#ifdef DEBUGP
#ifdef RG_DEBUG
   /* disabling debugs by default */
   /* rgCb.rgInit.dbgMask = 0xffffffff; */
#endif
#endif /* DEBUGP */
#ifdef SS_DIAG
   rgCb.rgInit.logMask = 0x0;
#endif
   rgCb.rgInit.procId = SFndProcId();

   rgCb.tfuSap.numBndRetries = 0;

   /* Initialize Sap state */
   rgCb.tfuSap.sapSta.sapState = LRG_NOT_CFG;
   rgCb.rguSap[0].sapSta.sapState = LRG_NOT_CFG;
   rgCb.rguSap[1].sapSta.sapState = LRG_NOT_CFG;
   rgCb.crgSap.sapSta.sapState = LRG_NOT_CFG;

#ifdef SS_RBUF
   SAttachSRngBuf(SS_RNG_BUF_ULMAC_TO_ULRLC, SS_RBUF_ENT_ULMAC,SS_RNG_TX);
   SAttachSRngBuf(SS_RNG_BUF_ULMAC_TO_ULRLC, SS_RBUF_ENT_ULRLC,SS_RNG_RX);
#endif
   RETVALUE(ROK);

} /* rgActvInit */

/**
 * @brief Layer Manager Configuration request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgCfgReq
 *     
 *     This function handles the configuration
 *     request received from the Layer Manager.
 *     -# Based on the cfg->hdr.elmId.elmnt value it invokes one of the
 *        functions rgHdlGenCfg() or rgHdlSapCfg().
 *     -# Invokes RgMiLrgCfgCfm() to send back the confirmation to the LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cfg, the configuration parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgCfgReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cfg     /* config structure  */
)
#else
PUBLIC S16 RgMiLrgCfgReq(pst, cfg)
Pst      *pst;    /* post structure  */
RgMngmt  *cfg;    /* config structure  */
#endif    
{
   U16       ret = LCM_PRIM_OK;
   U16       reason = LCM_REASON_NOT_APPL;
   RgMngmt   cfm;
   Pst       cfmPst;

   TRC2(RgMiLrgCfgReq)
   

  RG_DIAG_LVL0(0x0a0b0001, RG_DIAG_NA, SS_DIAG_INV_ARG,
              "Received CfgReq for MAC layer, Entity = %d, Instance = %d\n",
              pst->srcEnt, pst->srcInst,0,0);

   /* Fill the post structure for sending the confirmation */
   rgLMMFillCfmPst(pst, &cfmPst, cfg);

   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));

#ifdef LMINT3
   cfm.hdr.transId = cfg->hdr.transId;
#endif


   cfm.hdr.elmId.elmnt = cfg->hdr.elmId.elmnt;
   switch(cfg->hdr.elmId.elmnt)
   {
      case STGEN:
         reason = rgLMMGenCfg(&cfg->t.cfg); 
         break;
      case STRGUSAP:
      case STCRGSAP:
      case STTFUSAP:
         reason = rgLMMSapCfg(&cfg->t.cfg, cfg->hdr.elmId.elmnt);
         break;
      default:
         ret = LCM_PRIM_NOK;
         reason = LCM_REASON_INVALID_ELMNT;
         RLOG1(L_ERROR, "Invalid Elmnt=%d",
                  cfg->hdr.elmId.elmnt);
         break;
   }

   if (reason != LCM_REASON_NOT_APPL)
   {
      ret = LCM_PRIM_NOK;
   }

   cfm.cfm.status = ret;
   cfm.cfm.reason = reason;

   RgMiLrgCfgCfm(&cfmPst, &cfm);
   
   RETVALUE(ROK);
}/*-- RgMiLrgCfgReq --*/


/**
 * @brief Layer Manager Statistics request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgStsReq
 *     
 *     This function handles the statistics
 *     request received from the Layer Manager.
 *      -# Based on sts->hdr.elmId.elmnt, it retrieves either general or SAP
 *      statistics from the rgCb global control block.
 *      -# If action=ARST, it will reset the statistics parameters in rgCb to 0.
 *      -# Invokes the RgMiLrgStsCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *sts, the statistics parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgStsReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *sts     /* statistics structure  */
)
#else
PUBLIC S16 RgMiLrgStsReq(pst, sts)
Pst      *pst;    /* post structure  */
RgMngmt  *sts;    /* statistics structure  */
#endif    
{
   Pst       cfmPst;
   RgMngmt   cfm;

   TRC2(RgMiLrgStsReq)
   
   /* Fill the post structure for sending the confirmation */
   rgLMMFillCfmPst(pst, &cfmPst, sts);

   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));

#ifdef LMINT3
   cfm.hdr.transId = sts->hdr.transId;
#endif
   SGetDateTime(&cfm.t.sts.dt);
   cfm.cfm.status = LCM_PRIM_OK;
   cfm.cfm.reason = LCM_REASON_NOT_APPL;
   cfm.hdr.elmId.elmnt = sts->hdr.elmId.elmnt;
   cfm.t.sts.action = sts->t.sts.action;

   /* Check if General Config Done */
   if(rgCb.rgInit.cfgDone != TRUE) 
   {
      cfm.cfm.status = LCM_PRIM_NOK;
      cfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      RgMiLrgStsCfm(&cfmPst,&cfm);
      RLOG0(L_ERROR, "Gen Cfg not done");
      RETVALUE(ROK);
   }

   switch(sts->hdr.elmId.elmnt)
   {
      case STGEN:
         {
            cmMemcpy((U8 *)&(cfm.t.sts.s.genSts), (U8 *)&rgCb.genSts,
                  sizeof(RgGenSts));
            /* check if action is read and reset */
            if(sts->t.sts.action == ARST)
            {
               rgCb.genSts.numHarqFail = 0;
            }
            /* L2 statistics */
#ifdef MAC_SCH_STATS
            {
               RgGenSts *genSts = &(cfm.t.sts.s.genSts);
               U8       cqi = 0;
               for(cqi=0; cqi <= 14; cqi++)
               {
                  /* Filling DL ACK/NACK stats */
                  genSts->nackAckStats.dlCqiStat[cqi].mcs = \
                     hqFailStats.dlCqiStat[cqi].mcs;
                  genSts->nackAckStats.dlCqiStat[cqi].numOfNacks = \
                     hqFailStats.dlCqiStat[cqi].numOfNacks;
                  genSts->nackAckStats.dlCqiStat[cqi].numOfAcks = 
                     hqFailStats.dlCqiStat[cqi].numOfAcks;

                  /* Filling UL ACK/NACK stats */
                  genSts->nackAckStats.ulCqiStat[cqi].mcs = \
                     hqFailStats.ulCqiStat[cqi].mcs;
                  genSts->nackAckStats.ulCqiStat[cqi].numOfNacks = \
                     hqFailStats.ulCqiStat[cqi].numOfNacks;
                  genSts->nackAckStats.ulCqiStat[cqi].numOfAcks = \
                     hqFailStats.ulCqiStat[cqi].numOfAcks;

                  /* Filling DL HQ Retx stats */
                  genSts->hqRetxStats.dlCqiStat[cqi].mcs = \
                     hqRetxStats.dlCqiStat[cqi].mcs;
                  genSts->hqRetxStats.dlCqiStat[cqi].numOfHQ_1 = \
                     hqRetxStats.dlCqiStat[cqi].numOfHQ_1;
                  genSts->hqRetxStats.dlCqiStat[cqi].numOfHQ_2 = \
                     hqRetxStats.dlCqiStat[cqi].numOfHQ_2;
                  genSts->hqRetxStats.dlCqiStat[cqi].numOfHQ_3 = \
                     hqRetxStats.dlCqiStat[cqi].numOfHQ_3;
                  genSts->hqRetxStats.dlCqiStat[cqi].numOfHQ_4 = \
                     hqRetxStats.dlCqiStat[cqi].numOfHQ_4;
                  genSts->hqRetxStats.dlCqiStat[cqi].totalTx = \
                     hqRetxStats.dlCqiStat[cqi].totalTx;

                  /* Filling UL HQ Retx stats */
                  genSts->hqRetxStats.ulCqiStat[cqi].mcs = \
                     hqRetxStats.ulCqiStat[cqi].mcs;
                  genSts->hqRetxStats.ulCqiStat[cqi].numOfHQ_1 = \
                     hqRetxStats.ulCqiStat[cqi].numOfHQ_1;
                  genSts->hqRetxStats.ulCqiStat[cqi].numOfHQ_2 = \
                     hqRetxStats.ulCqiStat[cqi].numOfHQ_2;
                  genSts->hqRetxStats.ulCqiStat[cqi].numOfHQ_3 = \
                     hqRetxStats.ulCqiStat[cqi].numOfHQ_3;
                  genSts->hqRetxStats.ulCqiStat[cqi].numOfHQ_4 = \
                     hqRetxStats.ulCqiStat[cqi].numOfHQ_4;
                  genSts->hqRetxStats.ulCqiStat[cqi].totalTx = \
                     hqRetxStats.ulCqiStat[cqi].totalTx;
               }
               /* Reset statistics */
               if(sts->t.sts.action == ZEROSTS)
               {
                  cmMemset((U8 *)&hqRetxStats, 0, \
                        sizeof(RgSchHqRetxStats));
                  cmMemset((U8 *)&hqFailStats, 0, \
                        sizeof(RgSchNackAckStats));
               }
            }
#endif /* MAC_SCH_STATS*/
         }
         break;
      case STRGUSAP:
         cmMemcpy((U8 *)&(cfm.t.sts.s.rguSts), (U8 *)&rgCb.rguSap[sts->t.sts.sapInst].sapSts,
                  sizeof(RgSapSts));

         /* check if action is read and reset */
         if(sts->t.sts.action == ARST)
            cmMemset((U8 *)&rgCb.rguSap[sts->t.sts.sapInst].sapSts, 0, sizeof(RgSapSts));

         break;
      case STCRGSAP:
         cmMemcpy((U8 *)&(cfm.t.sts.s.crgSts), (U8 *)&rgCb.crgSap.sapSts,
                  sizeof(RgSapSts));

         /* check if action is read and reset */
         if(sts->t.sts.action == ARST)
            cmMemset((U8 *)&rgCb.crgSap.sapSts, 0, sizeof(RgSapSts));

         break;
      case STTFUSAP:
         cmMemcpy((U8 *)&(cfm.t.sts.s.tfuSts), (U8 *)&rgCb.tfuSap.sapSts,
                  sizeof(RgSapSts));

         /* check if action is read and reset */
         if(sts->t.sts.action == ARST)
            cmMemset((U8 *)&rgCb.tfuSap.sapSts, 0, sizeof(RgSapSts));

         break;
      default:
         cfm.cfm.status = LCM_PRIM_NOK;
         cfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
         RLOG1(L_ERROR, "Invalid Elmnt = %d",sts->hdr.elmId.elmnt);
         break;     
   }
   RgMiLrgStsCfm(&cfmPst,&cfm);
   RETVALUE(ROK);
}/*-- RgMiLrgStsReq --*/


/**
 * @brief Layer Manager Status request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgStaReq
 *     
 *     This function handles the solicited status
 *     request received from the Layer Manager.
 *      -# Based on sta->hdr.elmId.elmnt, it retrieves the status of a
 *      particular SAP from the rgCb global control block.
 *      -# Invokes the RgMiLrgStaCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *sta, the status parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgStaReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *sta     /* status structure  */
)
#else
PUBLIC S16 RgMiLrgStaReq(pst, sta)
Pst      *pst;    /* post structure  */
RgMngmt  *sta;    /* status structure  */
#endif    
{
   Pst       cfmPst;
   RgMngmt   cfm;

   TRC2(RgMiLrgStaReq)
   
   /* Fill the post structure for sending the confirmation */
   rgLMMFillCfmPst(pst, &cfmPst, sta);

   if (sta->t.ssta.s.sysId.ptNmb != NULLP)
   {
      SPutSBuf(pst->region, pst->pool, (Data *)sta->t.ssta.s.sysId.ptNmb, LRG_MAX_PT_NUM_SIZE);
   }
   
   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));
   cfm.hdr.elmId.elmnt = sta->hdr.elmId.elmnt;

#ifdef LMINT3
   cfm.hdr.transId = sta->hdr.transId;
#endif
   /* Check if General Config Done */
   if(rgCb.rgInit.cfgDone != TRUE) 
   {
      SGetDateTime(&cfm.t.ssta.dt);
      if (SGetSBuf(cfmPst.region, cfmPst.pool, 
          (Data **)&(cfm.t.ssta.s.sysId.ptNmb), LRG_MAX_PT_NUM_SIZE)
         != ROK)
      {
         RLOG0(L_ERROR, "Memory Unavailable for Confirmation");
         RETVALUE(ROK);
      } 
      cmMemset((U8 *)(cfm.t.ssta.s.sysId.ptNmb), 0, LRG_MAX_PT_NUM_SIZE);
      rgGetSId(&cfm.t.ssta.s.sysId);
      cfm.cfm.status = LCM_PRIM_NOK;
      cfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cfm.hdr.elmId.elmnt = sta->hdr.elmId.elmnt;
      RgMiLrgStaCfm(&cfmPst, &cfm);
      RLOG0(L_ERROR, "Gen Cfg not done");
      RETVALUE(ROK);
   }

   switch(sta->hdr.elmId.elmnt)
   {
      case STGEN:
         SGetDateTime(&cfm.t.ssta.dt);
         if (SGetSBuf(cfmPst.region, cfmPst.pool, 
             (Data **)&(cfm.t.ssta.s.sysId.ptNmb), LRG_MAX_PT_NUM_SIZE)
            != ROK)
         {
            RLOG0(L_ERROR, "Memory Unavailable for Confirmation");
            RETVALUE(ROK);
         } 
         cmMemset((U8 *)(cfm.t.ssta.s.sysId.ptNmb), 0, LRG_MAX_PT_NUM_SIZE);
         rgGetSId(&cfm.t.ssta.s.sysId);
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      case STRGUSAP:
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm.t.ssta.dt);
         cmMemcpy((U8 *)&(cfm.t.ssta.s.rguSapSta), 
            (U8 *)&rgCb.rguSap[sta->t.ssta.sapInst].sapSta,
            sizeof(RgSapSta));
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      case STCRGSAP:
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm.t.ssta.dt);
         cmMemcpy((U8 *)&(cfm.t.ssta.s.crgSapSta), (U8 *)&rgCb.crgSap.sapSta,
         sizeof(RgSapSta));
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      case STTFUSAP:
         cfm.cfm.status = LCM_PRIM_OK;
         cfm.cfm.reason = LCM_REASON_NOT_APPL;
         SGetDateTime(&cfm.t.ssta.dt);
         cmMemcpy((U8 *)&(cfm.t.ssta.s.tfuSapSta), (U8 *)&rgCb.tfuSap.sapSta,
         sizeof(RgSapSta));
         RgMiLrgStaCfm(&cfmPst, &cfm);
         break;
      default:
         cfm.cfm.status = LCM_PRIM_NOK;
         cfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
         RgMiLrgStaCfm(&cfmPst, &cfm);
         RLOG1(L_ERROR, "Invalid elmnt=%d",sta->hdr.elmId.elmnt);
         break;     
   }
   RETVALUE(ROK);
}/*-- RgMiLrgStaReq --*/


/**
 * @brief Layer Manager Control request handler. 
 *
 * @details
 *
 *     Function : RgMiLrgCntrlReq
 *     
 *     This function handles the control
 *     request received from the Layer Manager.
 *      -# Based on cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action
 *      and cntrl->t.cntrl.subAction, it performs the appropriate control action
 *      of SAP (enable/disable), Debug (enable/disable), Trace (enable/disable)
 *      and layer shutdown.
 *      -# Invokes the RgMiLrgCntrlCfm to send back the confirmation to LM.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgMngmt *cntrl, the control parameter's structure
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 RgMiLrgCntrlReq
(
Pst      *pst,    /* post structure  */
RgMngmt  *cntrl   /* control structure  */
)
#else
PUBLIC S16 RgMiLrgCntrlReq(pst, cntrl)
Pst      *pst;    /* post structure  */
RgMngmt  *cntrl;  /* control structure  */
#endif    
{
   S16       ret = ROK;            /* return value */
   Pst       cfmPst;
   RgMngmt   cfm;
   
   TRC2(RgMiLrgCntrlReq)
   
   /* Fill the post structure for sending the confirmation */
   rgLMMFillCfmPst(pst, &cfmPst, cntrl);

   cmMemset((U8 *)&cfm, 0, sizeof(RgMngmt));
#ifdef LMINT3
   cfm.hdr.transId = cntrl->hdr.transId;
#endif
   cfm.hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
   cfm.t.cntrl.action = cntrl->t.cntrl.action;
   cfm.t.cntrl.subAction = cntrl->t.cntrl.subAction;

   /* Check if General Config Done*/
   if(rgCb.rgInit.cfgDone != TRUE)
   {
      cfm.cfm.status = LCM_PRIM_NOK;
      cfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      cfm.hdr.elmId.elmnt = cntrl->hdr.elmId.elmnt;
      RgMiLrgCntrlCfm(&cfmPst, &cfm);
      RLOG0(L_ERROR, "Gen Cfg not done");
      RETVALUE(ROK);
   }
 
   /* General Config done, process the Control request */   
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STGEN:
         rgLMMGenCntrl(cntrl, &cfm, &cfmPst);
         break;
      case STTFUSAP:
      case STRGUSAP:
      case STCRGSAP:
         rgLMMSapCntrl(cntrl, &cfm, &cfmPst);
         break;
      default:
         cfm.cfm.status = LCM_PRIM_NOK;
         cfm.cfm.reason = LCM_REASON_INVALID_PAR_VAL;
         RgMiLrgCntrlCfm(&cfmPst, &cfm);
         RLOG1(L_ERROR, "invalid elmnt=%d",cntrl->hdr.elmId.elmnt);
         break;
   }
   RETVALUE(ret);
}/*-- RgMiLrgCntrlReq --*/


/**
 * @brief SAP Configuration Handler. 
 *
 * @details
 *
 *     Function : rgLMMSapCfg
 *     
 *     This function in called by RgMiLrgCfgReq(). It handles the
 *     interface SAP configuration of the LTE MAC layer. It 
 *     initializes the sapState to LRG_UNBND. Returns
 *     reason for success/failure of this function.
 *     
 *  @param[in]  RgCfg *cfg, the Configuaration information 
 *  @return  U16
 *      -# LCM_REASON_GENCFG_NOT_DONE
 *      -# LCM_REASON_INVALID_SAP
 *      -# LCM_REASON_NOT_APPL
 **/
#ifdef ANSI
PRIVATE U16 rgLMMSapCfg
(
RgCfg *cfg,            /* Configuaration information */
Elmnt sapType             /* Sap Type */
)
#else
PRIVATE U16 rgLMMSapCfg(cfg,sapType)
RgCfg *cfg;            /* Configuaration information */
Elmnt sapType;            /* Sap Type */
#endif
{
   U16               ret = LCM_REASON_NOT_APPL;
   RgLowSapCfgInfo   *lowSapCfg = NULLP;
   RgUpSapCfgInfo    *upSapCfg = NULLP;
    RgUpSapCb          *upSapCb  = NULLP;

   TRC2(rgLMMSapCfg)

   /* Check if Gen Config has been done */
   if(rgCb.rgInit.cfgDone != TRUE)
      RETVALUE(LCM_REASON_GENCFG_NOT_DONE);

   switch(sapType)
   {   
      case STRGUSAP:
         if ((cfg->s.rguSap.spId > LRG_MAX_RGU_SAPS) &&
             (cfg->s.rguSap.selector != RGU_SEL_TC) &&
             (cfg->s.rguSap.selector != RGU_SEL_LC))
         {
            ret = LCM_REASON_INVALID_PAR_VAL;
            RLOG0(L_ERROR, "unsupported Selector value for RGU");
            break;
         }
         upSapCb = &(rgCb.rguSap[cfg->s.rguSap.spId]);
         if(upSapCb->sapSta.sapState == LRG_NOT_CFG)
         { 
            upSapCb->sapSta.sapState = LRG_UNBND;
         }
         upSapCfg = &(upSapCb->sapCfg);
         upSapCfg->sapPst.dstEnt = cfg->s.rguSap.ent;
         upSapCfg->sapPst.dstInst = cfg->s.rguSap.inst;
         upSapCfg->sapPst.dstProcId = cfg->s.rguSap.procId;
         upSapCfg->sapPst.srcEnt = rgCb.rgInit.ent;
         upSapCfg->sapPst.srcInst = rgCb.rgInit.inst;
         upSapCfg->sapPst.srcProcId = rgCb.rgInit.procId;
         upSapCfg->sapPst.region = cfg->s.rguSap.mem.region;
         upSapCfg->sapPst.pool = cfg->s.rguSap.mem.pool;
         upSapCfg->sapPst.selector = cfg->s.rguSap.selector;
         upSapCfg->sapPst.route = cfg->s.rguSap.route;
         upSapCfg->sapPst.intfVer = 0; 
         upSapCfg->sapPst.prior = cfg->s.rguSap.prior;
         upSapCfg->suId = cfg->s.rguSap.suId;
         upSapCfg->spId = cfg->s.rguSap.spId;
         /*T2K uses 2 saps, T3K uses 1 sap. change the rgRguDlSap to 1 only if
          * there is cfg request with sap is 1*/
         if(cfg->s.rguSap.spId == 1)
         {
            rgRguDlSap = 1;
         }
         break;
      case STCRGSAP:
         if ((cfg->s.crgSap.selector != CRG_SEL_TC) &&
             (cfg->s.crgSap.selector != CRG_SEL_LC))
         {
            ret = LCM_REASON_INVALID_PAR_VAL;
            RLOG0(L_ERROR, "unsupported Selector value for CRG");
            break;
         }
         if(rgCb.crgSap.sapSta.sapState == LRG_NOT_CFG)
         { 
            rgCb.crgSap.sapSta.sapState = LRG_UNBND;
         }
         upSapCfg = &rgCb.crgSap.sapCfg;

         upSapCfg->sapPst.dstEnt = cfg->s.crgSap.ent;
         upSapCfg->sapPst.dstInst = cfg->s.crgSap.inst;
         upSapCfg->sapPst.dstProcId = cfg->s.crgSap.procId;
         upSapCfg->sapPst.srcEnt = rgCb.rgInit.ent;
         upSapCfg->sapPst.srcInst = rgCb.rgInit.inst;
         upSapCfg->sapPst.srcProcId = rgCb.rgInit.procId;
         upSapCfg->sapPst.region = cfg->s.crgSap.mem.region;
         upSapCfg->sapPst.pool = cfg->s.crgSap.mem.pool;
         upSapCfg->sapPst.selector = cfg->s.crgSap.selector;
         upSapCfg->sapPst.route = cfg->s.crgSap.route;
         upSapCfg->sapPst.intfVer = 0; 
         upSapCfg->sapPst.prior = cfg->s.crgSap.prior;
         upSapCfg->suId = cfg->s.crgSap.suId;
         upSapCfg->spId = cfg->s.crgSap.spId;
         break;
      case STTFUSAP:
#ifndef CL_MAC_LWLC 
         if ((cfg->s.tfuSap.selector != TFU_SEL_TC) &&
             (cfg->s.tfuSap.selector != TFU_SEL_LC))
         {
            ret = LCM_REASON_INVALID_PAR_VAL;
            RLOG0(L_ERROR, "unsupported Selector value for TFU");
            break;
         }
#endif
         if (rgCb.tfuSap.sapSta.sapState == LRG_NOT_CFG) 
         { 
            rgCb.tfuSap.sapSta.sapState = LRG_UNBND;
         }
         lowSapCfg = &rgCb.tfuSap.sapCfg;

         lowSapCfg->sapPst.dstEnt = cfg->s.tfuSap.ent;
         lowSapCfg->sapPst.dstInst = cfg->s.tfuSap.inst;
         lowSapCfg->sapPst.dstProcId = rgCb.rgInit.procId;
         lowSapCfg->sapPst.srcEnt = rgCb.rgInit.ent;
         lowSapCfg->sapPst.srcInst = rgCb.rgInit.inst;
         lowSapCfg->sapPst.srcProcId = rgCb.rgInit.procId;
         lowSapCfg->sapPst.region = cfg->s.tfuSap.mem.region;
         lowSapCfg->sapPst.pool = cfg->s.tfuSap.mem.pool;
         lowSapCfg->sapPst.selector = cfg->s.tfuSap.selector;
         lowSapCfg->sapPst.route = cfg->s.tfuSap.route;
         lowSapCfg->sapPst.intfVer = 0; 
         lowSapCfg->sapPst.prior = cfg->s.tfuSap.prior;
         lowSapCfg->suId = cfg->s.tfuSap.suId;
         lowSapCfg->spId = cfg->s.tfuSap.spId;
         cmMemcpy((U8 *)&lowSapCfg->bndTmr, (U8 *)&cfg->s.tfuSap.bndTmr,
                   sizeof(TmrCfg));
         break;
      default:
         /* would never reach here */
         break;
   }
   RETVALUE(ret);
}


/**
 * @brief General Configuration Handler. 
 *
 * @details
 *
 *     Function : rgLMMGenCfg
 *     
 *     This function in called by RgMiLrgCfgReq(). It handles the
 *     general configuration of the LTE MAC layer. It initializes 
 *     the hash lists of RgCb. Returns
 *     reason for success/failure of this function.
 *     
 *  @param[in]  RgCfg *cfg, the Configuaration information 
 *  @return  U16
 *      -# LCM_REASON_NOT_APPL 
 *      -# LCM_REASON_INVALID_MSGTYPE
 *      -# LCM_REASON_MEM_NOAVAIL
 **/
#ifdef ANSI
PRIVATE U16 rgLMMGenCfg
(
RgCfg *cfg            /* Configuaration information */
)
#else
PRIVATE U16 rgLMMGenCfg(cfg)
RgCfg *cfg;            /* Configuaration information */
#endif
{
   U16    ret = LCM_REASON_NOT_APPL;

   TRC2(rgLMMGenCfg)

   /* Check if General Configuration is done already */
   if (rgCb.rgInit.cfgDone == TRUE)
   {
      RETVALUE(LCM_REASON_INVALID_MSGTYPE);
   }
   if ((cfg->s.genCfg.lmPst.selector != LRG_SEL_TC) &&
       (cfg->s.genCfg.lmPst.selector != LRG_SEL_LC))
   {
      RLOG0(L_ERROR, "unsupported Selector value for RGU");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   /* Update the Pst structure for LM interface */
   cmMemcpy((U8 *)&rgCb.rgInit.lmPst, (U8 *)&cfg->s.genCfg.lmPst,
             sizeof(Pst));

   rgCb.rgInit.lmPst.srcProcId = rgCb.rgInit.procId;
   rgCb.rgInit.lmPst.srcEnt = rgCb.rgInit.ent;
   rgCb.rgInit.lmPst.srcInst = rgCb.rgInit.inst;
   rgCb.rgInit.lmPst.event = EVTNONE;

   rgCb.rgInit.region = cfg->s.genCfg.mem.region;
   rgCb.rgInit.pool = cfg->s.genCfg.mem.pool;
   rgCb.genCfg.tmrRes = cfg->s.genCfg.tmrRes;
   /* Initialize SAP States */
   rgCb.crgSap.sapSta.sapState = LRG_NOT_CFG;
   rgCb.rguSap[0].sapSta.sapState = LRG_NOT_CFG;
   rgCb.rguSap[1].sapSta.sapState = LRG_NOT_CFG;
   rgCb.tfuSap.sapSta.sapState = LRG_NOT_CFG;
   /* Initialize the timer blocks */
   cmInitTimers(rgCb.tmrBlk, RG_MAX_TIMER);
   /* Initialzie the timer queue */   
   cmMemset((U8 *)&rgCb.tmrTq, 0, sizeof(CmTqType)*RG_TQ_SIZE);
   /* Initialize the timer control point */
   cmMemset((U8 *)&rgCb.tmrTqCp, 0, sizeof(CmTqCp));
   rgCb.tmrTqCp.tmrLen = RG_TQ_SIZE;
   /* Timer Registration request to SSI */
   if (SRegTmrMt(rgCb.rgInit.ent, rgCb.rgInit.inst,
            (S16)rgCb.genCfg.tmrRes, rgActvTmr) != ROK)
   {
      RLOG0(L_ERROR, "Failed to register timer");
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }   
   /* cellCb hashList initialization */
   if (rgDBMInitCellLst(RG_NMB_CELL_HASHBIN) != ROK)
   {
      RLOG0(L_ERROR, "rgCb.cellLst Initialization failed");
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   /* Inactive cellCb hashList initialization */
   if (rgDBMInitInactvCellLst(RG_NMB_CELL_HASHBIN) != ROK)
   {
      RLOG0(L_ERROR, "rgCb.inactvCellLst Initialization failed");
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   rgCb.prevCell = NULLP;
   /* Set Config done in TskInit */
   rgCb.rgInit.cfgDone = TRUE;

   RETVALUE(ret);
}


/***********************************************************
 *
 *     Func : rgLMMShutdown
 *        
 *
 *     Desc : Handles the MAC layer shutdown request. Calls 
 *     rgCFGFreeCellCb(RgCellCb*) to handle each cellCb deallocation.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : rg_lmm.c 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgLMMShutdown
(
)
#else
PRIVATE Void rgLMMShutdown()
#endif
{
   RgCellCb   *cellCbPrev = NULLP;
   RgCellCb   *cellCbCurr = NULLP;
   U8 idx;

   TRC2(rgLMMShutdown)


   /* Unbind the TFU Sap */
   if(rgCb.tfuSap.sapSta.sapState == LRG_WAIT_BNDCFM)
   {
      rgLIMTfuUbndReq(rgCb.tfuSap.sapCfg.spId, LRG_UNBND);
      if (rgCb.tfuSap.sapCfg.bndTmr.enb == TRUE)
      {
         rgLMMStopTmr(RG_BNDREQ_TMR, (PTR)&rgCb.tfuSap); 
      }
      rgCb.tfuSap.sapSta.sapState = LRG_UNBND;
   }
   if(rgCb.tfuSap.sapSta.sapState == LRG_BND)
   {
      rgLIMTfuUbndReq(rgCb.tfuSap.sapCfg.spId, LRG_UNBND);
      rgCb.tfuSap.sapSta.sapState = LRG_UNBND;
   }

   /* Free the memory held by the Layer */
   while (NULLP != (cellCbCurr = rgDBMGetNextCellCb(cellCbPrev)))
   {
      rgDBMDelCellCb(cellCbCurr);

      for(idx=0;idx < RG_NUM_SUB_FRAMES; idx++)
      {
         rgTOMRlsSf(&cellCbCurr->subFrms[idx]);
      }

      rgCFGFreeCellCb(cellCbCurr);
   }  
   rgDBMDeInitCellLst();
   rgCb.prevCell = NULLP;

   cellCbPrev = NULLP;
   while (NULLP != (cellCbCurr = rgDBMGetNextInactvCellCb(cellCbPrev)))
   {
      rgDBMDelInactvCellCb(cellCbCurr);
      rgCFGFreeInactvCellCb(cellCbCurr);
   }  
   rgDBMDeInitInactvCellLst();

   /* De-register the Timer Service */
   (Void) SDeregTmrMt(rgCb.rgInit.ent, rgCb.rgInit.inst,
                     (S16)rgCb.genCfg.tmrRes, rgActvTmr); 

   /* call back the task initialization function to intialize
    * the global RgCb Struct */
   rgActvInit(rgCb.rgInit.ent, rgCb.rgInit.inst, rgCb.rgInit.region, 
              rgCb.rgInit.reason);

   RETVOID;
}


/***********************************************************
 *
 *     Func : rgLMMGenCntrl 
 *        
 *
 *     Desc : Processes the LM control request for STGEN elmnt.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : rg_lmm.c 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgLMMGenCntrl 
(
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
)
#else
PRIVATE Void rgLMMGenCntrl(cntrl, cfm, cfmPst)
RgMngmt       *cntrl;
RgMngmt       *cfm;
Pst           *cfmPst;
#endif
{
   TRC2(rgLMMGenCntrl)

   cfm->cfm.status = LCM_PRIM_OK;
   cfm->cfm.reason = LCM_REASON_NOT_APPL;
   

   switch(cntrl->t.cntrl.action)
   {
      case AENA:
         /* Action is Enable */
         switch(cntrl->t.cntrl.subAction)
         {
            case SATRC:
            /* Enable Traces */
               rgCb.rgInit.trc = TRUE;
               rgCb.trcLen = cntrl->t.cntrl.s.trcLen;
               /*Store the response and TransId for sending the Traces */
               cmMemcpy((U8 *)&rgCb.genCfg.trcResp.response, 
               (U8 *)&cntrl->hdr.response, sizeof(Resp));
               rgCb.genCfg.trcResp.transId = cntrl->hdr.transId;
               
               break;
            case SAUSTA:   
            /* Enable Unsolicited Status (alarms) */
               rgCb.rgInit.usta = TRUE;
               /*Store the response and TransId for sending the Alarms */
               cmMemcpy((U8 *)&rgCb.genCfg.ustaResp.response, 
               (U8 *)&cntrl->hdr.response, sizeof(Resp));
               rgCb.genCfg.ustaResp.transId = cntrl->hdr.transId;
               break;
            case SADBG:
            /* Enable Debug Printing */
#ifdef DEBUGP
               rgCb.rgInit.dbgMask |= cntrl->t.cntrl.s.rgDbgCntrl.dbgMask;
#endif
               break;
#ifdef SS_DIAG
            case SALOG:
               rgCb.rgInit.logMask = cntrl->t.cntrl.s.logMask;
               break;
#endif

            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               RLOG1(L_ERROR, "invalid subaction=%d",cntrl->t.cntrl.subAction);
               break;
         }
         break;
      case ADISIMM:
         /* Action is Diable immidiately */
         switch(cntrl->t.cntrl.subAction)
         {
            case SATRC:
            /* Disable Traces */
               rgCb.rgInit.trc = FALSE;
               break;
            case SAUSTA:
            /* Disable Unsolicited Status (alarms) */
               rgCb.rgInit.usta = FALSE;
               break;
            case SADBG:
            /* Disable Debug Printing */
#ifdef DEBUGP
               rgCb.rgInit.dbgMask &=~cntrl->t.cntrl.s.rgDbgCntrl.dbgMask;
#endif
               break;
#ifdef SS_DIAG
            case SALOG:
               rgCb.rgInit.logMask = cntrl->t.cntrl.s.logMask;
                break;
#endif

            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               RLOG1(L_ERROR, "invalid subaction=%d",cntrl->t.cntrl.subAction);
               break;
         }
         break;
      case ASHUTDOWN:
         /* Free all the memory dynamically allocated by MAC */
         rgLMMShutdown();
         break;
      default:
         cfm->cfm.status = LCM_PRIM_NOK;
         cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
         RLOG1(L_ERROR, "invalid action=%d",cntrl->t.cntrl.action);
         break;
   }
   RgMiLrgCntrlCfm(cfmPst, cfm);
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgLMMSapCntrl 
 *        
 *
 *     Desc : Processes the LM control request for STxxxSAP elmnt.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : rg_lmm.c 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgLMMSapCntrl 
(
RgMngmt       *cntrl,
RgMngmt       *cfm,
Pst           *cfmPst
)
#else
PRIVATE Void rgLMMSapCntrl(cntrl, cfm, cfmPst)
RgMngmt       *cntrl;
RgMngmt       *cfm;
Pst           *cfmPst;
#endif
{
   TRC2(rgLMMSapCntrl)

   /* Only TFU Sap can be controlled by LM */
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STTFUSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ABND:
            /* Bind Enable Request */
               if ((rgCb.tfuSap.sapSta.sapState == LRG_NOT_CFG) ||
                   (rgCb.tfuSap.sapSta.sapState == LRG_BND))
               {
                  cfm->cfm.status = LCM_PRIM_NOK;
                  cfm->cfm.reason = LCM_REASON_INVALID_SAP;
               }
               else
               {
                  if (rgCb.tfuSap.sapCfg.bndTmr.enb == TRUE)
                  {
                     rgLMMStartTmr(RG_BNDREQ_TMR, rgCb.tfuSap.sapCfg.bndTmr.val, 
                     (PTR)&rgCb.tfuSap);
                  }
                  /* Change SAP state */
                  rgCb.tfuSap.sapSta.sapState = LRG_WAIT_BNDCFM;
                  rgCb.tfuSap.numBndRetries++;
                  /* Store the response and TransId for sending 
                   * the Control confirm */
                  cmMemcpy((U8 *)&rgCb.genCfg.bndCfmResp.response,
                           (U8 *)&cntrl->hdr.response, sizeof(Resp));
                  rgCb.genCfg.bndCfmResp.transId = cntrl->hdr.transId;

                  /* Sending Status Indication to Layer Manager */
                  cfm->cfm.status = LCM_PRIM_OK_NDONE;
                  cfm->cfm.reason = LCM_REASON_NOT_APPL;
                  RgMiLrgCntrlCfm(cfmPst, cfm);

                  rgLIMTfuBndReq(rgCb.tfuSap.sapCfg.suId, rgCb.tfuSap.sapCfg.spId);
                  RETVOID;
               }
               break;
            case AUBND:
            /* Unbind request */

               /* Check if the SAP is configured */
               if( (rgCb.tfuSap.sapSta.sapState == LRG_NOT_CFG) ||
                     (rgCb.tfuSap.sapSta.sapState == LRG_UNBND))
               {
                  cfm->cfm.status = LCM_PRIM_NOK;
                  cfm->cfm.reason = LCM_REASON_INVALID_MSGTYPE;
               }
               else
               {
                  rgLIMTfuUbndReq(rgCb.tfuSap.sapCfg.spId, TFU_UBNDREQ_MNGMT);
                  if (rgCb.tfuSap.sapCfg.bndTmr.enb == TRUE)
                  {
                     rgLMMStopTmr(RG_BNDREQ_TMR, (PTR)&rgCb.tfuSap);
                  }
                  /* Change SAP state */
                  rgCb.tfuSap.sapSta.sapState = LRG_UNBND;
                  cfm->cfm.status = LCM_PRIM_OK;
                  cfm->cfm.reason = LCM_REASON_NOT_APPL;
               }
               break;
            case ADEL:
               /* Delete SAP, does initialization of SAP */
               if ((rgCb.tfuSap.sapSta.sapState == LRG_WAIT_BNDCFM) ||
                   (rgCb.tfuSap.sapSta.sapState == LRG_BND))
               {
                  rgLIMTfuUbndReq(rgCb.tfuSap.sapCfg.spId, TFU_UBNDREQ_MNGMT);
                  if (rgCb.tfuSap.sapCfg.bndTmr.enb == TRUE)
                  {
                     rgLMMStopTmr(RG_BNDREQ_TMR, (PTR)&rgCb.tfuSap);
                  }
               }
               cmMemset((U8 *)&rgCb.tfuSap, 0, sizeof(RgLowSapCb));
               rgCb.tfuSap.sapSta.sapState = LRG_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               RLOG1(L_ERROR, "invalid action=%d",cntrl->t.cntrl.action);
               break;
         }
         break;
      case STRGUSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ADEL:
               cmMemset((U8 *)&rgCb.rguSap, 0, sizeof(RgUpSapCb));
               rgCb.rguSap[cntrl->t.cntrl.instId].sapSta.sapState = LRG_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               RLOG1(L_ERROR, "invalid action=%d",cntrl->t.cntrl.action);
               break;
         }
         break;
      case STCRGSAP:
         switch(cntrl->t.cntrl.action)
         {
            case ADEL:
               cmMemset((U8 *)&rgCb.crgSap, 0, sizeof(RgUpSapCb));
               rgCb.crgSap.sapSta.sapState = LRG_NOT_CFG;
               cfm->cfm.status = LCM_PRIM_OK;
               cfm->cfm.reason = LCM_REASON_NOT_APPL;
               break;
            default:
               cfm->cfm.status = LCM_PRIM_NOK;
               cfm->cfm.reason = LCM_REASON_INVALID_PAR_VAL;
               RLOG1(L_ERROR, "invalid action=%d",cntrl->t.cntrl.action);
               break;
         }
         break;
      default:
         /* Would never here. */
         RETVOID;
   }
   RgMiLrgCntrlCfm(cfmPst, cfm);
   RETVOID;
}


/***********************************************************
 *
 *     Func : rgLMMFillCfmPst 
 *        
 *
 *     Desc : Fills the Confirmation Post Structure cfmPst using the reqPst 
 *            and the cfm->hdr.response.
 *            
 *
 *     Ret  : Void
 *
 *     Notes: 
 *
 *     File : rg_lmm.c 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgLMMFillCfmPst
(
Pst           *reqPst,
Pst           *cfmPst,
RgMngmt       *cfm
)
#else
PRIVATE Void rgLMMFillCfmPst(reqPst, cfmPst, cfm)
Pst           *reqPst;
Pst           *cfmPst;
RgMngmt       *cfm;
#endif
{
   TRC2(rgLMMFillCfmPst)

   cfmPst->srcEnt    = rgCb.rgInit.ent;
   cfmPst->srcInst   = rgCb.rgInit.inst;
   cfmPst->srcProcId = rgCb.rgInit.procId;
   cfmPst->dstEnt    = reqPst->srcEnt;
   cfmPst->dstInst   = reqPst->srcInst;
   cfmPst->dstProcId = reqPst->srcProcId;

   cfmPst->selector  = cfm->hdr.response.selector;
   cfmPst->prior     = cfm->hdr.response.prior;
   cfmPst->route     = cfm->hdr.response.route;
   cfmPst->region    = cfm->hdr.response.mem.region;
   cfmPst->pool      = cfm->hdr.response.mem.pool;

   RETVOID;
}


/**
 * @brief Timer start handler. 
 *
 * @details
 *
 *     Function : rgLMMStartTmr
 *     
 *     This function based on the input parameters starts the timer for 
 *     "tmrVal" duration. As of now MAC uses the timer functionality for 
 *     BndReq only. Hence there is no conditional code based on "tmrEvnt".
 *     
 *  @param[in]  S16   tmrEvnt, the Timer Event    
 *  @param[in]  U32   tmrVal,  the Wait Time
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgLMMStartTmr
(
S16                tmrEvnt,            /* Timer Event */
U32                tmrVal,             /* Wait Time */
PTR                cb                  /* Entry for which Timer Expired */
)
#else
PUBLIC S16 rgLMMStartTmr(tmrEvnt, tmrVal, cb)
S16                tmrEvnt;            /* Timer Event */
U32                tmrVal;             /* Wait Time */
PTR                cb;                 /* Entry for which Timer Expired */
#endif
{
   CmTmrArg    arg;

   TRC2(rgLMMStartTmr)

   UNUSED(tmrEvnt);

   /* Initialize the arg structure */
   cmMemset((U8 *)&arg, 0, sizeof(CmTmrArg));

   arg.tqCp = &rgCb.tmrTqCp;
   arg.tq = rgCb.tmrTq;
   arg.timers = rgCb.tmrBlk;
   arg.cb = cb;
   arg.tNum = 0;
   arg.max = RG_MAX_TIMER;
   arg.evnt = RG_BNDREQ_TMR;
   arg.wait = tmrVal;      
   cmPlcCbTq(&arg);

   RETVALUE(ROK);
}


/**
 * @brief Timer stop handler. 
 *
 * @details
 *
 *     Function : rgLMMStopTmr
 *     
 *     This function based on the input parameters stops the timer for 
 *     "tmrEvnt". As of now MAC uses the timer functionality for 
 *     BndReq only. Hence there is no conditional code based on "tmrEvnt".
 *     Once the bind happens and this timer is stopped, the timer functionality
 *     is deregistered with SSI. As there is no further use of timer processing.
 *     
 *  @param[in]  S16   tmrEvnt, the Timer Event    
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgLMMStopTmr
(
S16                tmrEvnt,            /* Timer Event */
PTR                cb                  /* Entry for which Timer Expired */
)
#else
PUBLIC S16 rgLMMStopTmr(tmrEvnt, cb)
S16                tmrEvnt;            /* Timer Event */
PTR                cb;                 /* Entry for which Timer Expired */
#endif
{
   CmTmrArg   arg;
   U8         i;
   S16        ret; 

   TRC2(rgLMMStopTmr)

   ret = RFAILED;

   for(i=0;i<RG_MAX_TIMER;i++)
   {
      /* Search for the Timer Blocks */
      if(rgCb.tmrBlk[i].tmrEvnt == tmrEvnt)
      {
         /* Initialize the arg structure */
         cmMemset((U8 *)&arg, 0, sizeof(CmTmrArg));

         arg.tqCp = &rgCb.tmrTqCp;
         arg.tq = rgCb.tmrTq;
         arg.timers = rgCb.tmrBlk;
         arg.cb = cb;
         arg.max = RG_MAX_TIMER;
         arg.evnt = NOTUSED;
         arg.wait = NOTUSED;

         arg.tNum = i;   
         cmRmvCbTq(&arg);
         ret = ROK;
         break;
      }

   }


   RETVALUE(ret);
}


/**
 * @brief Timer Expiry handler. 
 *
 * @details
 *
 *     Function : rgLMMTmrExpiry
 *     
 *     This is a callback function used as an input parameter to cmPrcTmr()
 *     to check expiry of any timer. In this function, the only concern is
 *     about tmrEvnt=Bind timer.
 *     
 *  @param[in]  PTR   cb,  Entry for which Timer expired
 *  @param[in]  S16   tmrEvnt, the Timer Event    
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgLMMTmrExpiry
(
PTR cb,               /* Pointer to timer control block */
S16 tmrEvnt           /* Timer Event */
)
#else
PUBLIC S16 rgLMMTmrExpiry(cb,tmrEvnt)
PTR cb;               /* Pointer to timer control block */
S16 tmrEvnt;          /* Timer Event */
#endif
{
   S16        ret = ROK;
   RgLowSapCb *tfuSap = (RgLowSapCb *)cb;

   TRC2(rgLMMTmrExpiry)

   
   switch(tmrEvnt)
   {
      case RG_BNDREQ_TMR:
         tfuSap->numBndRetries++;
         if(tfuSap->numBndRetries > RG_MAX_BNDRETRY)
         {
            rgLMMStaInd(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                        LCM_CAUSE_TMR_EXPIRED, NULLP);
         }
         else
         {
            /* Restart the bind timer */
            if (tfuSap->sapCfg.bndTmr.enb == TRUE)
            {
               ret = rgLMMStartTmr(RG_BNDREQ_TMR, tfuSap->sapCfg.bndTmr.val,
               cb);
            }

            /* Send bind request */
            rgLIMTfuBndReq(tfuSap->sapCfg.suId, tfuSap->sapCfg.spId);
         }
         break;
      default:
         RLOG1(L_ERROR, "Invalid tmrEvnt=%d",tmrEvnt);
         ret = RFAILED;
         break;
   }
   RETVALUE(ret);
}



/**
 * @brief Layer Manager Unsolicited Status Indication generation. 
 *
 * @details
 *
 *     Function : rgLMMStaInd 
 *     
 *     This API is used by the other modules of MAC to send a unsolicited
 *     status indication to the Layer Manager.
 *     
 *  @param[in]  U16 category, the Alarm category
 *  @param[in]  U16 event, the Alarm event
 *  @param[in]  U16 cause, the cause of the Alarm
 *  @param[in]  RgUstaDgn *dgn, Alarm Diagonostics
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgLMMStaInd
(
U16 category,
U16 event,
U16 cause,
RgUstaDgn *dgn
)
#else
PUBLIC S16 rgLMMStaInd(category, event, cause, dgn) 
U16 category;
U16 event;
U16 cause;
RgUstaDgn *dgn;
#endif
{
   RgMngmt    usta;

   TRC2(rgLMMStaInd)

   if(rgCb.rgInit.usta == FALSE)
   {
      RETVALUE(ROK);
   }

   cmMemset((U8 *)&usta, 0, sizeof(RgMngmt));

   SGetDateTime(&usta.t.usta.cmAlarm.dt);
   usta.t.usta.cmAlarm.category = category;
   usta.t.usta.cmAlarm.event = event;
   usta.t.usta.cmAlarm.cause = cause;
   if (dgn != NULLP)
   {
      cmMemcpy((U8 *)&usta.t.usta.dgn, (U8 *)dgn, sizeof(RgUstaDgn));
   }

   rgCb.rgInit.lmPst.selector = rgCb.genCfg.ustaResp.response.selector;
   rgCb.rgInit.lmPst.prior = rgCb.genCfg.ustaResp.response.prior;
   rgCb.rgInit.lmPst.route = rgCb.genCfg.ustaResp.response.route;
   rgCb.rgInit.lmPst.region = rgCb.genCfg.ustaResp.response.mem.region;
   rgCb.rgInit.lmPst.pool = rgCb.genCfg.ustaResp.response.mem.pool;
   usta.hdr.transId = rgCb.genCfg.ustaResp.transId;

   RETVALUE(RgMiLrgStaInd(&rgCb.rgInit.lmPst, &usta));
}


/**
 * @brief Layer Manager Trace Indication generation. 
 *
 * @details
 *
 *     Function : rgLMMTrcInd 
 *     
 *     This API is used by the other modules of MAC to send a 
 *     Trace indication to the Layer Manager.
 *     
 *  @param[in]   Buffer *srcMbuf, the Message Buffer .
 *  @param[in]   U8 event, the trace event.
 *  @return Void 
 **/
#ifdef ANSI
PUBLIC Void rgLMMTrcInd
(
Buffer *srcMbuf,    /* Message Buffer */
U8 event            /* event */
)
#else
PUBLIC Void rgLMMTrcInd(srcMbuf,event)
Buffer *srcMbuf;    /* Message Buffer */
U8 event;           /* event */
#endif
{
   Buffer   *dstMbuf = NULLP;   
   MsgLen   bufLen  = 0;
   Data     *tempBuf;
   MsgLen   tempCnt;
   RgMngmt  trc;
   Pst      pst;

   TRC2(rgLMMTrcInd)


   if ((rgCb.trcLen == LRG_NO_TRACE) || (srcMbuf == NULLP))
   {
      RLOG0(L_ERROR, "Trace Disabled.");
      RETVOID;
   }
   
   cmMemset((U8 *)&trc, 0, sizeof(RgMngmt));

   pst = rgCb.rgInit.lmPst;
   pst.selector = rgCb.genCfg.trcResp.response.selector;
   pst.prior = rgCb.genCfg.trcResp.response.prior;
   pst.route = rgCb.genCfg.trcResp.response.route;
   pst.region = rgCb.genCfg.trcResp.response.mem.region;
   pst.pool = rgCb.genCfg.trcResp.response.mem.pool;

   trc.hdr.transId = rgCb.genCfg.trcResp.transId;

   SGetDateTime(&trc.t.trc.dt);
      
   /* Check if the whole buffer is to be sent in Trace indication */
   if(rgCb.trcLen == LRG_FULL_TRACE)
   {
      if (SCpyMsgMsg(srcMbuf, pst.region, pst.pool, &dstMbuf)
         != ROK)
      {
         RLOG0(L_ERROR, "SCpyMsgMsg Failed.");
         RETVOID;
      }
      trc.cfm.status = LCM_PRIM_OK;
      trc.cfm.reason = LCM_REASON_NOT_APPL;
      trc.t.trc.evnt = event;
         
      /* Send Trace Indication to Layer manager */
      RgMiLrgTrcInd(&pst, &trc, dstMbuf);
   }
   /* check if only a specified number of bytes are to be sent */
   else if(rgCb.trcLen > 0)
   {
      /* Get the length of the recvd message buffer */
      if (SFndLenMsg(srcMbuf, &bufLen) != ROK)
      {
         RLOG0(L_ERROR, "SFndLenMsg Failed.");
         RETVOID;
      }
      /* Check if the recvd buffer size is less than request trace len */
      if(bufLen < rgCb.trcLen)
      {
         /* Copy the whole of the recvd buffer in trace indication */
      
         if (SCpyMsgMsg(srcMbuf, pst.region, pst.pool, &dstMbuf)
            != ROK)
         {
            RLOG0(L_ERROR, "SCpyMsgMsg Failed.");
            RETVOID;
         }
         
         trc.cfm.status = LCM_PRIM_OK;
         trc.cfm.reason = LCM_REASON_NOT_APPL;
         trc.t.trc.evnt = event;

         /* Send Trace Indication to Layer manager */
         RgMiLrgTrcInd(&pst, &trc, dstMbuf);
      }
      /* if the recvd buffer size is greater than request trace len */
      if(bufLen >= rgCb.trcLen)
      {
         /* Get a temporary buffer to store the msg */
         if (rgAllocSBuf(&tempBuf, rgCb.trcLen) != ROK)
         {
            RLOG0(L_ERROR, "rgAllocSBuf Failed.");
            RETVOID;
         }
         
         /* Copy trcLen nos of bytes from the recvd message */
         if (SCpyMsgFix(srcMbuf,0,rgCb.trcLen,tempBuf,&tempCnt) != ROK)   
         {
            RLOG0(L_ERROR, "SCpyMsgFix Failed.");
            RETVOID;
         }

         if (SGetMsg(pst.region, pst.pool, &dstMbuf) != ROK)
         {
            RLOG0(L_ERROR, "dstMbuf Allocation Failed");
            RETVOID;
         }
         /* Copy the tempBuf data to dst mBuf */
         if (SCpyFixMsg(tempBuf,dstMbuf,0,rgCb.trcLen,&tempCnt) != ROK)
         {
            RLOG0(L_ERROR, "SCpyFixMsg Failed.");
            RETVOID;
         }

         /*ccpu00117052 - MOD - Passing double pointer for proper NULLP 
                               assignment */
         /* Free the memory allocated for tempBuf */
         rgFreeSBuf(&tempBuf, rgCb.trcLen);
               
         trc.cfm.status = LCM_PRIM_OK;
         trc.cfm.reason = LCM_REASON_NOT_APPL;
         trc.t.trc.evnt = event;
      
         /* Send Trace Indication to Layer manager */
         RgMiLrgTrcInd(&pst, &trc, dstMbuf);
      }
   }
   RETVOID;
}


/**
 * @brief Layer Manager Control Confirm generation handler
 *        for Bind Confirm reception at TFU interface.
 *        RgLiTfuBndCfm() forwards the confirmation to this 
 *        function. All SAP state related handling is restricted
 *        to LMM modules, hence the cfm forwarding.
 *
 * @details
 *
 *     Function : rgLMMBndCfm 
 *     
 *     This API is used by the LIM module of MAC to forward
 *     the Bind Confirm it receives over the TFU interface.
 *     
 *  @param[in]   Pst *pst, Post Structure
 *  @param[in]   SuId suId, Service user ID
 *  @param[in]   U8 status, Status
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgLMMBndCfm
(
Pst *pst,               /* Post Structure */
SuId suId,              /* Service user ID */
U8 status               /* Status */
)
#else
PUBLIC S16 rgLMMBndCfm(pst,suId,status)
Pst *pst;               /* Post Structure */
SuId suId;              /* Service user ID */
U8 status;              /* Status */
#endif
{
   S16       ret = ROK;
   RgMngmt   cntrlCfm;
   Pst       cfmPst;

   TRC3(rgLMMBndCfm)

   UNUSED(pst);

   /* Check if the suId is valid */
   if(rgCb.tfuSap.sapCfg.suId != suId)
   {
      RLOG0(L_ERROR, "Invalid SuId");
      RETVALUE(RFAILED);
   }

   /* check the Sap State */
   switch(rgCb.tfuSap.sapSta.sapState)
   {
      case LRG_WAIT_BNDCFM:
         break;
      case LRG_BND:
         /* SAP is already bound */
         RETVALUE(ROK);
      default:
         RETVALUE(RFAILED);
   }

   cfmPst = rgCb.rgInit.lmPst;
   cfmPst.selector = rgCb.genCfg.bndCfmResp.response.selector;
   cfmPst.prior = rgCb.genCfg.bndCfmResp.response.prior;
   cfmPst.route = rgCb.genCfg.bndCfmResp.response.route;
   cfmPst.region = rgCb.genCfg.bndCfmResp.response.mem.region;
   cfmPst.pool = rgCb.genCfg.bndCfmResp.response.mem.pool;
   
   cmMemset((U8 *)&cntrlCfm, 0, sizeof(RgMngmt));

   switch(status)
   {
      case CM_BND_OK: /* status is OK */
         /* Change SAP state to Bound */
         rgCb.tfuSap.sapSta.sapState = LRG_BND;
         if (rgCb.tfuSap.sapCfg.bndTmr.enb == TRUE)
         {
            ret = rgLMMStopTmr(RG_BNDREQ_TMR, (PTR)&rgCb.tfuSap);
         }
         /* Send Control Confirm with status as OK to Layer Manager */
         cntrlCfm.cfm.status = LCM_PRIM_OK;
         cntrlCfm.cfm.reason = LCM_REASON_NOT_APPL;
         break;

      default:
         /* Change SAP state to UnBound */
         rgCb.tfuSap.sapSta.sapState = LRG_UNBND;
         if (rgCb.tfuSap.sapCfg.bndTmr.enb == TRUE)
         {
            ret = rgLMMStopTmr(RG_BNDREQ_TMR, (PTR)&rgCb.tfuSap);
         }
         /* Send Control Confirm with status as NOK to Layer Manager */
         cntrlCfm.cfm.status = LCM_PRIM_NOK;
         cntrlCfm.cfm.reason = LCM_REASON_NEG_CFM;
         break;
   }
   rgCb.tfuSap.numBndRetries = 0;
   cntrlCfm.hdr.elmId.elmnt = STTFUSAP;
   cntrlCfm.hdr.transId = rgCb.genCfg.bndCfmResp.transId;

   ret = RgMiLrgCntrlCfm(&cfmPst, &cntrlCfm);

   RETVALUE(ret);
}


/**
 * @brief LTE MAC timer call back function registered with SSI. 
 *
 * @details
 *
 *     Function :  rgActvTmr
 *     
 *     This function is invoked by SSI for every timer activation
 *     period expiry.
 *     
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgActvTmr
(
 Ent     ent,
 Inst    inst
)
#else
PUBLIC S16 rgActvTmr(ent, inst)
Ent     ent;
Inst    inst;
#endif
{
   TRC3(rgActvTmr)

   /* Check if any MAC timer has expired */ 
   cmPrcTmr(&rgCb.tmrTqCp, rgCb.tmrTq, (PFV) rgLMMTmrExpiry);
 
   RETVALUE(ROK);
 
} /* end of rgActvTmr */


/**********************************************************************
 
         End of file:     gk_lmm.c@@/main/3 - Sat Jul 30 02:21:30 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     sm                1. Initial Release.
/main/2      ---     sm                1. LTE MAC 2.1 release
          rg003.201  ns                1. Ccpu00108135 and Trace added to functions
          rg005.201  sm                1. Disabling debugs by default.
                                       2. Removed SPutBuf 
          rg009.201 gj                 1. [ccpu00116647] Added transId filling for
                                          confirm messages
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---        rt   1. LTE MAC 4.1 release
*********************************************************************91*/
