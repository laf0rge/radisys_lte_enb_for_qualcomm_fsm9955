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

        Name:    LTE - PDCP - Layer Manager file

        Type:    C source file

        Desc:    It contains the following common  functions for processing
                 the all the external interfaces.
                    -- PjMiLpjCfgReq
                    -- PjMiLpjCfgCfm 
                    -- PjMiLpjCntrlReq
                    -- PjMiLpjCntrlCfm
                    -- PjMiLpjStaReq
                    -- PjMiLpjStaCfm
                    -- PjMiLpjStaInd
                    -- PjMiLpjStsReq
                    -- PjMiLpjStsCfm
                    -- PjMiLpjTrcInd 

        File:     gp_lmm.c

        Sid:      eq_lmm.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:46:46 2014
        Sid:      eq_lmm.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:46:46 2014

        Prg:     cp 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=3;

/** @file pj_lmm.c
@brief PDCP Layer Manager Module
**/


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
#include "kwu.h"
#include "lpj.h"           /* LPJ defines */
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"           
#include "pj_err.h"        /* Err defines */
#include "pj_env.h"        /* PDCP environment options */
#include "pj.h"            /* PDCP defines */
#include "pj_udx.h"

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
#include "kwu.x"
#include "lpj.x"           /* LPJ */
#include "cpj.x"           /* CPJ */
#include "pju.x"           /* PJU */

#include "pj.x"
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_ul.x"
#include "pj_dl.x"
#include "pj_lib.x"

#ifdef __cplusplus
EXTERN "C" {
#endif /* CPLUSPLUS */

EXTERN S16 pjGetSId ARGS((SystemId *s));
#ifdef __cplusplus
}
#endif /* CPLUSPLUS */

U8 *gPoolId; 
PDCPStats gPdcpStats;

#ifdef TENB_AS_SECURITY
U8 isSecBatchMode;
#endif


/*********************************************************************
 *             Forward Declaration of LPJ Porting Functions
 ********************************************************************/
PUBLIC PjCb  *pjCb[PJ_MAX_PDCP_INSTANCES];
EXTERN S16 pjActvTmr ARGS ((Ent ent, Inst inst));

PRIVATE S16 pjLmmSendCfm ARGS ((Pst *pst,PjMngmt *cfm,U8 type,Header *hdr));
PRIVATE S16 pjLmmGenCfg ARGS ((PjCb *gCb,PjGenCfg *cfg));
PRIVATE S16 pjLmmPjuSapCfg ARGS ((PjCb *gCb,PjSapCfg *cfg));
PRIVATE S16 pjLmmKwuSapCfg ARGS ((PjCb *gCb,PjSapCfg *cfg));
PRIVATE S16 pjLmmUdxSapCfg ARGS ((PjCb *gCb,PjSapCfg *cfg));
PRIVATE S16 pjLmmCpjSapCfg ARGS ((PjCb *gCb,PjSapCfg   *cfg));
PRIVATE S16 pjLmmGenCntrl ARGS ((PjCb *gCb,PjMngmt *cntrl));
PRIVATE S16 pjLmmUdxSapCntrl ARGS ((PjCb *gCb,PjMngmt *cntrl));
PRIVATE S16 pjLmmLSapCntrl ARGS ((PjCb *gCb,PjMngmt *cntrl));
PRIVATE S16 pjLmmFormLmCfm ARGS ((Pst *pst, Header *hdr, Reason reason));
PRIVATE S16 pjLmmGetPjuSapSta ARGS ((PjCb *gCb,PjPjuSapSta *sta));
PRIVATE S16 pjLmmGetKwuSapSta ARGS ((PjCb *gCb,PjKwuSapSta *sta));
PRIVATE S16 pjLmmGetUdxSapSta ARGS ((PjCb *gCb,PjUdxSapSta *sta));
PRIVATE S16 pjLmmGetCpjSapSta ARGS ((PjCb *gCb,PjCpjSapSta *sta));
PRIVATE S16 pjLmmGetGenSts ARGS ((PjCb *gCb,PjGenSts *sts,Action action));
PRIVATE S16 pjLmmGetSapSts ARGS ((PjCb *gCb,PjMngmt *sts,Elmnt elmnt,Action action));
PRIVATE Void pjLmmCleanGblRsrcs ARGS ((PjCb *gCb));

/*********************************************************************
 *             Primitives for LPJ interface 
 ********************************************************************/
/**
 * @brief
      Stores the general PDCP configuration. It calculates the
      memory requirement and reserves it. It also initializes the
      control blocks, the hash lists and registers the timers. In
      case of error it rolls back and reports error in the confirm.
      If the general configuration is already done it is treated as
      reconfiguration, but right now reconfiguration is not supported.
 
 *  @param[in] cfm         PDCP LM general config structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL 
 *      -# Failure : LCM_REASON_MEM_NOAVAIL
 *                   LCM_REASON_REGTMR_FAIL
 *                   LCM_REASON_INVALID_PAR_VAL
 *                   LCM_REASON_HASHING_FAILED
 */
#ifdef ANSI
PRIVATE S16 pjLmmGenCfg
(
PjCb        *gCb,
PjGenCfg    *cfg
)
#else
PRIVATE S16 pjLmmGenCfg(gCb, cfg)
PjCb        *gCb;
PjGenCfg    *cfg;
#endif
{
   Size       pjMemSize;
   Size       pjSapSize;
   PjPjuSapCb *pjSapCb;
   CmTqCp     *pjTqCp;
   U16        cnt;
   S16        ret;

   TRC2(pjLmmGenCfg)

   ret       =   ROK;
   pjMemSize =   0;
   pjSapSize =   0;
   pjSapCb   =   NULLP;

   if(cfg->maxUe > PJ_MAX_UE) 
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      RLOG2(L_ERROR, "Invalid maxUe : Max [%lu] Received [%lu]", 
                        PJ_MAX_UE, cfg->maxUe);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if(cfg->maxPjuSaps > PJ_MAX_PJUSAPS)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      RLOG2(L_ERROR, "Invalid maxPjuSaps : Max [%lu] Received [%lu]", 
                       PJ_MAX_PJUSAPS, cfg->maxPjuSaps);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if(cfg->maxKwuSaps > PJ_MAX_KWUSAPS)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      RLOG2(L_ERROR, "Invalid maxKwuSaps : Max [%lu] Received [%lu]", 
                        PJ_MAX_KWUSAPS, cfg->maxKwuSaps);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if(cfg->maxUdxSaps > PJ_MAX_UDXSAPS)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      RLOG2(L_ERROR, "Invalid maxUduSaps : Max [%lu] Received [%lu]", 
                       PJ_MAX_UDXSAPS, cfg->maxUdxSaps);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }   

   if(gCb->init.cfgDone & PJ_LMM_GENCFG_DONE)
   {
      /* reconfiguration is supported only for obdDiscTmrInt */
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      gCb->pjGenCfg.obdTmrInt = cfg->obdTmrInt;
      gCb->pjGenCfg.obdWtTmrInt = cfg->obdWtTmrInt;
#endif /* PJ_SEC_ASYNC || PJ_CMP_ASYNC */
      RETVALUE(LCM_REASON_NOT_APPL);
   }

   PJ_MEM_CPY(&(gCb->pjGenCfg), cfg, sizeof(PjGenCfg));
#ifdef TENB_AS_SECURITY
   isSecBatchMode = TRUE;
#endif

   /* Timer Queue Control point initialization */
   pjTqCp         = &(gCb->pjTqCp);
   pjTqCp->tmrLen = PJ_TMR_LEN;
   pjTqCp->nxtEnt = 0;

   /* calculate the memory requirements */
   pjMemSize += (Size)SBUFSIZE(PJ_UE_LIST_BUCKET_SIZE * sizeof(CmListEnt));

   /* Cell Hash List Size */
   pjMemSize += (Size)SBUFSIZE(PJ_CELL_LIST_BUCKET_SIZE * sizeof(CmListEnt));

   /* Reserve the memory */
   if(SGetSMem(PJ_GET_MEM_REGION(gCb), pjMemSize,
              PJ_GET_MEM_POOL_ADDRESS(gCb)) != ROK)
   {
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }
   cmMemset((U8 *)&gCb->pjTq, 0, sizeof(CmTqType)*PJ_TMR_LEN);
   cmMemset((U8 *)&gCb->pjTqCp, 0, sizeof(CmTqCp));
   gCb->pjTqCp.tmrLen = PJ_TMR_LEN;
   /* Register the timer */
   if(SRegTmrMt(gCb->init.ent, gCb->init.inst, (U16)cfg->timeRes,
              pjActvTmr) != ROK)
   {
      (Void) SPutSMem(PJ_GET_MEM_REGION(gCb), PJ_GET_MEM_POOL(gCb));

      RETVALUE(LCM_REASON_REGTMR_FAIL);
   }

   /* Timer Initialization  */
   gCb->pjTqCp.tmrLen = PJ_TMR_LEN;
   for(cnt = 0; cnt < PJ_TMR_LEN; cnt++)
   {
      gCb->pjTq[cnt].first = NULLP;
      gCb->pjTq[cnt].tail = NULLP;
   }


   if (cfg->mode == LPJ_MODE_PDCP_UL)
   {
      PJ_ALLOC(gCb, gCb->u.ulCb, sizeof(PjUlgCb)); 
      if (gCb->u.ulCb == NULLP)
      {  
        RLOG0(L_FATAL, "Memory Allocation failed.");
        /* Do clean up */
        RETVALUE(RFAILED);
      }
      
      /* Initialize the Ue and Cell hash list */
      ret = pjDbmUlInit(gCb); /*How would DbmDlInit be called? */
      if (ret != ROK)
      {
         /* Do Clean up */
         RETVALUE(LCM_REASON_HASHING_FAILED);
      }
    
      /* Initialize the PJU SAP control blocks */
      PJ_MEM_SET(&(gCb->u.ulCb->pjuSap[PJ_SRB_SAP]), 0, sizeof(PjPjuSapCb));
      PJ_MEM_SET(&(gCb->u.ulCb->pjuSap[PJ_DRB_SAP]), 0, sizeof(PjPjuSapCb));

      /* Initialize the KWU SAP Control blocks */
      PJ_MEM_SET(&(gCb->u.ulCb->kwuSap), 0, (PJ_MAX_KWUSAPS * sizeof(PjPjuSapCb)));
      PJ_MEM_SET(&(gCb->u.ulCb->udxUlSap), 0, (PJ_MAX_UDXSAPS * sizeof(PjUdxUlSapCb)));

      /* Initialize CPJ SAP control block */
      PJ_MEM_SET(&(gCb->u.ulCb->cpjSap), 0, sizeof(PjCpjSapCb)); 
#ifdef LTE_L2_MEAS
      pjUtlL2MeasUlInit(gCb);
#endif
   }
   else
   {
      PJ_ALLOC(gCb, gCb->u.dlCb, sizeof(PjDlgCb)); 
      if (gCb->u.dlCb == NULLP)
      {
        RLOG0(L_FATAL, "Memory Allocation failed.");
        /* Do clean up */
        RETVALUE(RFAILED);
      }
      
      /* Initialize the Ue and Cell hash list */
      ret = pjDbmDlInit(gCb);
      if (ret != ROK)
      {
         RETVALUE(LCM_REASON_HASHING_FAILED);
      }
    

      /* Initialize the PJU SAP control blocks */
      PJ_MEM_SET(&(gCb->u.dlCb->pjuSap[PJ_SRB_SAP]), 0, sizeof(PjPjuSapCb));
      PJ_MEM_SET(&(gCb->u.dlCb->pjuSap[PJ_DRB_SAP]), 0, sizeof(PjPjuSapCb));

      /* Initialize the KWU SAP Control blocks */
      PJ_MEM_SET(&(gCb->u.dlCb->kwuSap), 0, (PJ_MAX_KWUSAPS * sizeof(PjPjuSapCb)));
      PJ_MEM_SET(&(gCb->u.dlCb->udxDlSap), 0, (PJ_MAX_UDXSAPS * sizeof(PjUdxDlSapCb)));
#ifdef LTE_L2_MEAS
      pjUtlL2MeasDlInit(gCb);
#endif
   }

   PJ_MEM_CPY(&(gCb->init.lmPst), &cfg->lmPst,
                      sizeof(Pst));

   gCb->init.lmPst.srcProcId = gCb->init.procId;
   gCb->init.lmPst.srcEnt = gCb->init.ent;
   gCb->init.lmPst.srcInst = gCb->init.inst;
   gCb->init.lmPst.event = EVTNONE;

   /* pj002.201 For multi core and multi region, no need to reinitiailize the region again .
      This would be done with pjActvInit from SSI */
#if !defined(SS_MULTICORE_SUPPORT) && !defined(SS_M_PROTO_REGION)
   gCb->init.region = cfg->lmPst.region;
#endif
   gCb->init.pool   = cfg->lmPst.pool;

   gCb->init.cfgDone |= PJ_LMM_GENCFG_DONE;

   gPoolId = &(gCb->u.dlCb->kwuSap->pst.pool);

#ifdef TENB_AS_SECURITY
   /* Initialize UL fd for interfacing with SPAcc */
   if (cfg->mode == LPJ_MODE_PDCP_UL)
   {
      if(PjLibObdUlInitReq(gCb) != ROK)
      {
         RETVALUE(LCM_REASON_MISC_FAILURE);
      }
   }
   else
   {
      /* Initialize DL fd for interfacing with SPAcc */
      if(PjLibObdDlInitReq(gCb) != ROK)
      {
         RETVALUE(LCM_REASON_MISC_FAILURE);
      }
   }
#endif


   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmGenCfg */

/**
 *
 * @brief
 *     Sends confirmation message to LM Called by Request function
 *  @param[in] pst         Post structure
 *  @param[in] cfm         PDCP LM structure 
 *  @param[in] type        type of message
 *  @param[in] hdr         message header
 *  @return  S16
 *      -# ROK
 */
#ifdef ANSI
PRIVATE S16 pjLmmSendCfm
(
Pst *pst,                    /* Post structure */
PjMngmt *cfm,                /* PDCP LM structure */
U8 type,                     /* type of message */
Header *hdr                  /* message header */
)
#else
PRIVATE S16 pjLmmSendCfm(pst, cfm, type, hdr)
Pst *pst;                    /* Post structure */
PjMngmt *cfm;                /* PDCP LM structure */
U8 type;                     /* type of message */
Header *hdr;                 /* message header */
#endif
{
   Pst rPst;                 /* Reply post structure */
   PjCb   *tPjCb;
   TRC2(pjLmmSendCfm);

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   PJ_MEM_SET(&rPst, 0, sizeof(Pst));   
 
   /* reply post structure for confirmation */
   rPst.srcEnt = tPjCb->init.ent;
   rPst.srcInst = tPjCb->init.inst;
   rPst.srcProcId = SFndProcId();
   rPst.dstEnt = pst->srcEnt;
   rPst.dstInst = pst->srcInst;
   rPst.dstProcId = pst->srcProcId;
   rPst.selector = hdr->response.selector;
   rPst.prior = hdr->response.prior;
   rPst.route = hdr->response.route;
   rPst.region = hdr->response.mem.region;
   rPst.pool= hdr->response.mem.pool;

   /* Fill the reply header */
   PJ_MEM_SET (&cfm->hdr, '\0', sizeof(cfm->hdr));
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.entId.ent = tPjCb->init.ent;
   cfm->hdr.entId.inst = tPjCb->init.inst;
   cfm->hdr.transId = hdr->transId;

   switch(type)
   {
      case TCFG:
         PjMiLpjCfgCfm(&rPst,cfm);
         break;
      case TCNTRL:
         SGetDateTime(&(cfm->t.cntrl.dt));
         PjMiLpjCntrlCfm(&rPst,cfm);
         break;
      case TSTS:
         SGetDateTime(&(cfm->t.sts.dt));
         PjMiLpjStsCfm(&rPst,0,cfm);
         break;
      case TSSTA:
         SGetDateTime(&(cfm->t.ssta.dt));
         PjMiLpjStaCfm(&rPst,cfm);
         break;
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         RLOG0(L_ERROR, "Invalid message type");
          
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         break;
   }

   RETVALUE(ROK);
} /* pjLmmSendCfm */


/**
@brief
This function processes configuration requests received from the layer manager.
The configuration requests can be for general configuration, or configuration
of PJU, and CPJ SAPs. The type of the configuration request can be determined
from the elmId field in the header of the layer manager message.This function
is called by the layer manager to configure PDCP. 

This function implements the following logic: \n

- switch (cfg->hdr.elmId.elmnt)
   - case STGEN 
      - update the genCfg field in RlCb; 
      - allocate the maximum static memory required by the PDCP product; 
      - register the timer service by calling SReqTmr; 
      - set CfgDone for not configuring again; 
   - case STPJUSAP 
      - if genCfg is not done, send negative Cfm to the layer manager; 
      - if all the parameters are valid, then allocate the RlSap control block;
      - update the SAP control block with the information provided in the 
      - configuration request; 
      - send configuration confirm with the status, success; 
   - case STCPJSAP 
      - if genCfg is not done, send negative Cfm to the layer manager; 
      - if all the parametes are valid, then allocate the CrSap control block; 
      - update the SAP control block with the information provided in the
         configuration request; 
      - send configuration confirm with the status, success; 
- end switch;
 
 *  @param[in] pst      post structure 
 *  @param[in] cfg      LM management structure 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

#ifdef ANSI
PUBLIC S16 PjMiLpjCfgReq 
(
Pst *pst, 
PjMngmt *cfg
)
#else
PUBLIC S16 PjMiLpjCfgReq (pst, cfg)
Pst  *pst;
PjMngmt *cfg;
#endif
{
   S16   ret;
   Elmnt elmnt;          /* header element */
   PjMngmt rCfm;         /* Reply cfm */
   Reason reason;        /* failure reason */
   PjCb   *tPjCb;

   TRC3(PjMiLpjCfgReq);

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)     /* KW_FIX */
   {
      RLOG1(L_DEBUG, "PjMiLpjCfgReq(post, elmId(%d))", cfg->hdr.elmId.elmnt);
      QU_CRITLOG(QU_LOG_MESSAGE, "PjMiLpjCfgReq(post, dstInst(%d))", pst->dstInst);
       RETVALUE(RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);


   ret = ROK;

   /* check configuration is done or not */
   if ((!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE)) &&
         (cfg->hdr.elmId.elmnt != STGEN))
   {
      /*
       * if general config is not over then use pst structure
       * in primitive to communicate to stack manager
       */

      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      pjLmmSendCfm(pst, &rCfm, TCFG, &cfg->hdr);     
      RETVALUE(RFAILED);
   }

   elmnt = cfg->hdr.elmId.elmnt;

   switch(elmnt)
   {
      case STGEN:
      {
         reason = pjLmmGenCfg(tPjCb,&cfg->t.cfg.s.gen);
         break;
      }
      case STPJUSAP:
      {
         reason = pjLmmPjuSapCfg(tPjCb,&cfg->t.cfg.s.sap);
         break;
      }
      case STKWUSAP:
      {
         reason = pjLmmKwuSapCfg(tPjCb,&cfg->t.cfg.s.sap);
         break;
      }
      case STCPJSAP:
      {
         reason = pjLmmCpjSapCfg(tPjCb,&cfg->t.cfg.s.sap);
         break;
      }
      case STUDXSAP:
      {
         reason = pjLmmUdxSapCfg(tPjCb,&cfg->t.cfg.s.sap);
         break;
      }
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }   
   }

   if (reason == LCM_REASON_NOT_APPL)
   {
      rCfm.cfm.status = LCM_PRIM_OK;
      rCfm.cfm.reason = LCM_REASON_NOT_APPL;

      ret = ROK;
   }
   else
   {
      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = reason;

      ret = RFAILED;
   }

   RLOG2(L_DEBUG, "Cfg  Req processed : status(%d) reason(%d)",
            rCfm.cfm.status, rCfm.cfm.reason);

   ret = pjLmmSendCfm(pst, &rCfm, TCFG, &cfg->hdr);

   RETVALUE(ret);
} /* PjMiLpjCfgReq */

/**
@brief 
The layer manager initiates and uses the management-control procedure to
control PDCP elements.The PDCP control request primitive (PjMiLpjCntrlReq)
can be called more than once and at any time after the management-configuration
procedure.The control request primitive is confirmed by a PjMiLpjCntrlCfm
primitive.

This function implements the following logic: \n

- if(cfgDone = FALSE) 
   - send negative confirmation to the layer manager; 
   - return; 
- else 
- switch(cntrl->hdr.elmId.elmnt) 
   -  case STGEN 
        - switch (action) 
        - case AENA 
             - switch(subAction) 
             - case SAUSTA 
                  - enable the unsolicited status flag; 
                  - send the control Cfm with success; 
             - case SADBG 
                  - set the debug mask; 
                  - send the control Cfm with success; 
             - case SATRC 
                  - enable the trace flag; 
                  - send the control Cfm with success; 
        - case ADISIMM 
             - switch(subAction) 
             - case SAUSTA 
                  - disable the unsolicited status flag; 
                  - send the control Cfm with success; 
             - case SATRC 
                  - disable the trace flag; 
                  - send the control Cfm with success; 
        - case ASHUTDOWN 
   - case STMKSAP 
        - switch (action) 
        - case ABND 
        - case AUBND 
   - default  
- end else  
 *  @param[in] pst      post structure
 *  @param[in] cntrl    LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 PjMiLpjCntrlReq
(
Pst *pst, 
PjMngmt *cntrl
)
#else
PUBLIC S16 PjMiLpjCntrlReq(pst, cntrl)
Pst     *pst;
PjMngmt *cntrl;
#endif
{
   Elmnt elmnt;          /* header element */
   PjMngmt rCfm;         /* Reply cfm */
   Reason reason;        /* failure reason */

   S16 ret;
   PjCb   *tPjCb;
 
   TRC3(PjMiLpjCntrlReq)

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)     /* KW_FIX */
   {
     RETVALUE(RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

   RLOG1(L_DEBUG,  "Cntrl Req Received (post, elmId(%d))", 
                       cntrl->hdr.elmId.elmnt);

   ret = ROK;

   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;

      pjLmmSendCfm(pst, &rCfm, TCNTRL, &cntrl->hdr);
      RETVALUE(RFAILED);
   }

   /* In normal cases, LCM_REASON_NOT_APPL is returned in cfm.
    * In all error cases appropriate reason is returned
    * by the functions below
    */

   elmnt = cntrl->hdr.elmId.elmnt;

   reason = LCM_REASON_NOT_APPL;


   switch (elmnt)
   {
      case STGEN:
      {
         /* general control */
         reason = pjLmmGenCntrl(tPjCb,cntrl);
         break;
      }

      case STKWUSAP:
      {
         /* general control */
         reason = pjLmmLSapCntrl(tPjCb,cntrl);
         break;
      }
      case STUDXSAP:
      {
         /* general control */
         reason = pjLmmUdxSapCntrl(tPjCb,cntrl);
         break;
      }

      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end switch */

   RLOG1(L_DEBUG, "Cntrl Req  Processed sending cfm: reason(%d)", reason);

   ret = pjLmmFormLmCfm(pst, &cntrl->hdr, reason);
   RETVALUE(ret);
} /* PjMiLpjCntrlReq */

/**
@brief 
This function processes solicited status requests received from the layer
manager. The layer manager can request status information regarding the
system ID, PJUSAP, or CPJSAP.

 *  @param[in] pst      post structure
 *  @param[in] cfm    LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 PjMiLpjStaReq
(
Pst *pst, 
PjMngmt *sta
)
#else
PUBLIC S16 PjMiLpjStaReq(pst, sta)
Pst     *pst;
PjMngmt *sta;
#endif
{
   S16      ret;
   Elmnt    elmnt;            /* Header element */
   PjMngmt  rSta;             /* Status */
   Reason   reason;           /* Failure reason */
   PjCb   *tPjCb;

   TRC3(PjMiLpjStaReq);

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   RLOG1(L_DEBUG, "Status  Req Received (post, elmId(%d))", 
                         sta->hdr.elmId.elmnt);

   PJ_MEM_SET(&rSta, 0, sizeof(PjMngmt));

   reason = LCM_REASON_NOT_APPL;

   elmnt = sta->hdr.elmId.elmnt;

   switch (elmnt)
   {
      case STSID:
      {
         reason = LCM_REASON_NOT_APPL;
         pjGetSId(&rSta.t.ssta.s.sysId);
         break;
      }
      case STGEN:
      {
         break;
      }
      case STKWUSAP:
      {
         PJ_MEM_CPY (&rSta.t.ssta.s.kwuSap, &sta->t.ssta.s.kwuSap,
               sizeof (PjKwuSapSta));
         reason = pjLmmGetKwuSapSta (tPjCb,&rSta.t.ssta.s.kwuSap);
         break;
      }
      case STUDXSAP:
      {
         PJ_MEM_CPY (&rSta.t.ssta.s.udxSap, &sta->t.ssta.s.udxSap,
               sizeof (PjUdxSapSta));
         reason = pjLmmGetUdxSapSta (tPjCb,&rSta.t.ssta.s.udxSap);
         break;
      }
      case STPJUSAP:
      {
         PJ_MEM_CPY (&rSta.t.ssta.s.pjuSap, &sta->t.ssta.s.pjuSap,
               sizeof (PjPjuSapSta));
         reason = pjLmmGetPjuSapSta (tPjCb,&rSta.t.ssta.s.pjuSap);
         break;
      }
      case STCPJSAP:
      {
         PJ_MEM_CPY (&rSta.t.ssta.s.cpjSap, &sta->t.ssta.s.cpjSap,
               sizeof (PjCpjSapSta));
         reason = pjLmmGetCpjSapSta (tPjCb,&rSta.t.ssta.s.cpjSap);
         break;
      }
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end of switch */

   if (reason == LCM_REASON_NOT_APPL)
   {
      rSta.cfm.status = LCM_PRIM_OK;
      rSta.cfm.reason = LCM_REASON_NOT_APPL;

      ret = ROK;
   }
   else
   {
      rSta.cfm.status = LCM_PRIM_NOK;
      rSta.cfm.reason = reason;

      ret = RFAILED;
   }

   RLOG2(L_DEBUG, "Sta Req processed - sending cfm: status(%d) reason(%d)",
             rSta.cfm.status, rSta.cfm.reason);

   ret = pjLmmSendCfm(pst, &rSta, TSSTA, &sta->hdr);

   RETVALUE(ret);
} /* PjMiLpjStaReq */

/**
@brief 
This function processes statistics requests received from the layer manager.
After collecting the statistics, this function calls the statistics confirm
function to send the statistics to the layer manager.

- switch(sts->hdr.elmId.elmnt) 
  - case STGEN 
     - get the general statistics from the PJ control block; 
     - if (action = RESET) 
        - reset the general statistic field in the RlCb; 
        - call PjMiLpjStsCfm to send statistics back to layer manager; 
  - case STCPJSAP 
  - case STPJUSAP 
      - get the SAP specific statistics from PJ control block; 
      - if (action = RESET) 
      - reset the general statistic field in the RlCb; 
      - call PjMiLpjStsCfm to send statistics to the layer manager; 
-end switch;

 *  @param[in] pst      post structure
 *  @param[in] action   action 
 *  @param[in] cfg      LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 PjMiLpjStsReq 
(
Pst *pst, 
Action action, 
PjMngmt *sts
)
#else
PUBLIC S16 PjMiLpjStsReq (pst, action, sts)
Pst     *pst;
Action  action;
PjMngmt *sts;
#endif
{
   S16      ret;
   PjMngmt  rSts;             /* Statistics */
   Reason   reason;           /* Reason for failure */
   Elmnt    elmnt;            /* Element Id */
   PjCb   *tPjCb;

   TRC3(PjMiLpjStsReq);

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   RLOG2(L_DEBUG,  "Sts Req (post, elmId(%d),action(%d))",
            action, sts->hdr.elmId.elmnt);

   PJ_MEM_SET(&rSts, 0, sizeof(PjMngmt));
   elmnt = sts->hdr.elmId.elmnt;

   switch (elmnt)
   {
      case STGEN:
      {
         reason = pjLmmGetGenSts(tPjCb,&rSts.t.sts.s.gen, action);
         break;
      }
      case STPJUSAP:
      {
         reason = pjLmmGetSapSts(tPjCb,sts, elmnt, action);
         PJ_MEM_CPY(&rSts.t.sts.s.pjuSap, &sts->t.sts.s.pjuSap, 
                    sizeof(PjPjuSts));
         break;
      }
      case STCPJSAP: /* No CPJ SAP statistics available */
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end switch */

   if (reason == LCM_REASON_NOT_APPL)
   {
      rSts.cfm.status = LCM_PRIM_OK;
      rSts.cfm.reason = LCM_REASON_NOT_APPL;

      ret = ROK;
   }
   else
   {
      rSts.cfm.status = LCM_PRIM_NOK;
      rSts.cfm.reason = reason;

      ret = RFAILED;
   }

   RLOG2(L_DEBUG, "Sts Req Processed - sending cfm: status(%d) reason(%d)",
            rSts.cfm.status, rSts.cfm.reason);

   ret = pjLmmSendCfm(pst, &rSts, TSTS, &sts->hdr);

   RETVALUE(ret);
} /* PjMiLpjStsReq */
/* pj005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
/**
 * @brief  Validates the measurement request parameters. 
 *
 * @details
 *
 *     Function :pjUtlValidateL2Meas 
 *
 *  @param[in]  PjL2MeasReqEvt measReqEvt
 *  @param[out] PjL2MeasCfmEvt measCfmEvt
 **/
#ifdef ANSI
PRIVATE S16 pjUtlValidateL2Meas
(
PjCb            *tPjCb, 
PjL2MeasReqEvt  *measReqEvt,
PjL2MeasCfmEvt  *measCfmEvt
)
#else
PRIVATE S16 pjUtlValidateL2Meas(tPjCb, measReqEvt, measCfmEvt)
PjCb             *tPjCb;   
PjL2MeasReqEvt  *measReqEvt;
PjL2MeasCfmEvt  *measCfmEvt;
#endif
{
   U16      measType;
   S16     ret;
   U8      qciIdx, qciVal;

   TRC3(pjUtlValidateL2Meas)
   
   ret = ROK;
   measType = measReqEvt->measReq.measType;

   if(measType == 0)
   {
      measCfmEvt->numQciCfm = 1;
      measCfmEvt->measCfm[0].qci = measReqEvt->measReq.qci[0];
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LPJ_CAUSE_INVALID_MEASTYPE;
      RETVALUE(RFAILED);
   }
   if(((measReqEvt->measReq.measType & LPJ_L2CPU_PERCORE_STATS) || 
       (measReqEvt->measReq.measType & LPJ_L2MEM_PERPOOL_STATS)) &&
       (~(measReqEvt->measReq.measType & LPJ_L2MEAS_UL_LOSS))) 

   {
      RETVALUE(ROK);
   }
   if(measReqEvt->measReq.numQci > LPJ_MAX_QCI || measReqEvt->measReq.numQci == 0)
   {
      measCfmEvt->numQciCfm = 1;
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LPJ_CAUSE_EXCEED_NUMQCI;
      RETVALUE(RFAILED);
   }
   if(measReqEvt->measPeriod != 0)
   {
      measCfmEvt->numQciCfm = 1;
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LPJ_CAUSE_INVALID_MEASTYPE;
      RETVALUE(RFAILED);
   }
   for(qciIdx = 0; qciIdx < measReqEvt->measReq.numQci; qciIdx++)
   {
      if(tPjCb->u.ulCb->pjL2Cb.measOn[measReqEvt->measReq.qci[qciIdx]] & 
            measReqEvt->measReq.measType)
      {
        /* Measurement is already ongoing */
        measCfmEvt->measCfm[measCfmEvt->numQciCfm].qci = qciIdx;
        measCfmEvt->measType = measType;
        measCfmEvt->numQciCfm++;
      }
   }
   if(measCfmEvt->numQciCfm > 0)
   {
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LPJ_CAUSE_MEAS_ALREADY_ENA;
      measCfmEvt->transId = measReqEvt->transId;
      RETVALUE(RFAILED);
   }
   for(qciIdx = 0; qciIdx < measReqEvt->measReq.numQci; qciIdx++)
   {
      qciVal = measReqEvt->measReq.qci[qciIdx];
      if(qciVal == 0 || qciVal > 9)
      {
         measCfmEvt->numQciCfm = 1;
         measCfmEvt->measCfm[0].qci = measReqEvt->measReq.qci[qciIdx];
         measCfmEvt->transId = measReqEvt->transId;
         measCfmEvt->measType = measReqEvt->measReq.measType;
         measCfmEvt->status.status = LCM_PRIM_NOK;
         measCfmEvt->status.reason = LPJ_CAUSE_INVALID_QCI;
         RETVALUE(RFAILED);
      }
   }
   
   
   RETVALUE(ROK);
}/* pjUtlValidateL2Meas */

/**
@brief 
This function processes L2 Measurement requests received from the layer manager.
The L2 Measurement is start for a time period and after the timer expiry, the 
measurement confirm is sent.

-  Accept only one set of measurements.
-  Allocate and initialise KwL2MeasEvtCb.
-  Validate if the measurement is already in progress.
   -  Loop through the existing measEvtCb with matching measType
      -  Check if measurement is running for any qci present in the measReq
      -  If present, move the qCi to list of invalid qCIs.
-  Set KwL2MeasEvtCb.measOn to TRUE for QCIs to be measured.
-  For the number of measurements requested.
   -  For all Ues with RBs matching qCI.
      -  Add RbCb to the RB linked list.
      -  Set kwRbCb.measOn to measurement type.
      -  If Meas type is DL_DELAY
         -  Update COUNT to startCount.
-  Start l2Timer

 *  @param[in] pst      post structure
 *  @param[in] action   action 
 *  @param[in] cfg      LM management structure
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 PjMiLpjL2MeasReq 
(
Pst            *pst, 
PjL2MeasReqEvt *measReqEvt 
)
#else
PUBLIC S16 PjMiLpjL2MeasReq (pst, measReqEvt)
Pst            *pst; 
PjL2MeasReqEvt *measReqEvt;
#endif
{
   S16            ret = ROK;
   U16             measType;
   PjL2MeasCfmEvt measCfmEvt;
   PjL2Cb   *pjL2Cb = NULLP;
   PjCb     *tPjCb = PJ_GET_PJCB(pst->dstInst);

   TRC3(PjMiLpjL2MeasReq);

   tPjCb =  PJ_GET_PJCB(pst->dstInst);
   
   measType = measReqEvt->measReq.measType;
   
   RLOG1(L_DEBUG, "L2_MEAS START Measurement type is %u\n", measType);
   /* Initialize measCfmEvt */
   memset(&measCfmEvt,0, sizeof(PjL2MeasCfmEvt));

   ret = pjUtlValidateL2Meas(tPjCb, measReqEvt, &measCfmEvt);
   if(ret != ROK)
   {
      pjUtlSndUlL2MeasNCfm(tPjCb, &measCfmEvt);
      RETVALUE(RFAILED);
   }

   RLOG1(L_DEBUG,"Measurement Start Req type is [%lu]", measType);
   if (measType & LPJ_L2MEAS_UL_LOSS)
   {
      U32      cntr    = 0;

      pjL2Cb = &tPjCb->u.ulCb->pjL2Cb;

      for(cntr = 1; cntr < LPJ_MAX_QCI; cntr++)
      {
         pjL2Cb->measOn[cntr] |= measType;
      }
   }
   if ((measType & LPJ_L2MEM_PERPOOL_STATS) || (measType & LPJ_L2CPU_PERCORE_STATS) )
   {
      /* starting measurement timer */
      ret = pjStartTmr(tPjCb, (PTR)tPjCb, PJ_EVT_L2_MEAS_TMR); 
      if(ret == RFAILED) 
      {
         RETVALUE(ret);
      }  
   }
   if(measType & LPJ_L2MEM_PERPOOL_STATS)
   {   
      pjL2Cb->memUtilizationMask |= LPJ_L2MEM_PERPOOL_STATS;
   }
   if(measType & LPJ_L2CPU_PERCORE_STATS)
   {
      pjL2Cb->cpuUtilizationMask |= LPJ_L2CPU_PERCORE_STATS;
   }
   /* Use else for DL in future */
   RETVALUE(ret);
} /* PjMiLpjL2MeasReq */

/**
@brief 
This function processes L2 Measurement stop request received from the layer manager.
After receving this request, PDCP stops L2 Measurement
 *  @param[in] pst      post structure
 *  @param[in] measType meas Type 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

#ifdef ANSI
PUBLIC S16 PjMiLpjL2MeasStopReq
(
Pst            *pst,
U16            measType
)
#else
PUBLIC S16 PjMiLpjL2MeasStopReq (pst, measType)
Pst            *pst;
U16             measType;
#endif
{
   U16            cntr;
   PjCb             *tPjCb     = PJ_GET_PJCB(pst->dstInst);
   PjL2MeasEvtCb    *measEvtCb = NULLP;
   PjL2MeasCfmEvt          measCfmEvt;  
   TRC3(PjMiLpjL2MeasStopReq);


   RLOG1(L_DEBUG,"Meas Stop Req type is [%lu]", measType);
   /* For UL counters only */
   cmMemset((U8*)&measCfmEvt, 0, sizeof(PjL2MeasCfmEvt)); 
   
   if(measType & LPJ_L2MEM_PERPOOL_STATS || 
      measType & LPJ_L2CPU_PERCORE_STATS)
   {
   	/* stopping timer */
   	pjStopTmr(tPjCb, (PTR)tPjCb, PJ_EVT_L2_MEAS_TMR);
   }
   if(measType & LPJ_L2MEM_PERPOOL_STATS)
   {   
      tPjCb->u.ulCb->pjL2Cb.memUtilizationMask = 0;
   }
   if(measType & LPJ_L2CPU_PERCORE_STATS)
   {
      tPjCb->u.ulCb->pjL2Cb.cpuUtilizationMask = 0;
   }
 
   for(cntr = 0; cntr < LPJ_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(tPjCb->u.ulCb->pjL2Cb.pjL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         pjUtlResetUlL2MeasCntr(tPjCb, &measEvtCb->measCb, measType);
      }
   }
   PjMiLpjL2MeasStopCfm(&tPjCb->pjGenCfg.lmPst, measType,LCM_PRIM_OK); 
   RETVALUE(ROK);
}


/**
 *
 * @brief For reporting measurement.
 *
 *
 * @b Description
 *        This function is called when the l2 measurement report send request is received. 
 *        This function sends a consolidates the mesaurements taken during
 *        this time and sends the confirm .
 *
 *  @param[in] measEvtCb    Measurement Event Control Block.
 *
 *
 *  @return  S16
 *      -# ROK
 */

#ifdef ANSI
PRIVATE S16 PjUtlPrepNSndL2MeasSendCfm
(
PjCb          *tPjCb,
PjL2MeasEvtCb *measEvtCb
)
#else
PRIVATE S16 PjUtlPrepNSndL2MeasSendCfm(tPjCb, measEvtCb)
PjCb          *tPjCb;
PjL2MeasEvtCb *measEvtCb;
#endif
{
   PjL2MeasCb     *measCb;
   PjL2MeasCfmEvt measCfmEvt;
   U8             qciIdx;
   U8             coreIdx;
   S16            ret;
   TRC3(PjUtlPrepNSndL2MeasSendCfm);

   measCb = &measEvtCb->measCb;
   /* Initialize measCfmEvt */
   cmMemset((U8 *) &measCfmEvt, 0, sizeof(PjL2MeasCfmEvt));
   measCfmEvt.measType = measCb->measType;
   measCfmEvt.status.status = LCM_PRIM_OK;
   measCfmEvt.status.reason = LCM_REASON_NOT_APPL;

   /* Filling Memory Utilzation structure */
   if(measCb->measType &  LPJ_L2MEM_PERPOOL_STATS)
   {
      if(tPjCb->u.ulCb->pjL2Cb.memUtilizationMask & LPJ_L2MEM_PERPOOL_STATS)
      {
         cmFillMemUtilizationMeas(&(measCfmEvt.memoryInfo),&(measCb->memInfo));
         cmClearMemUtilizationCounter(&(measCb->memInfo));
         if(FALSE == (pjChkTmr(tPjCb, (PTR)tPjCb, PJ_EVT_L2_MEAS_TMR)))
         {
            /* starting measurement timer */
            ret = pjStartTmr(tPjCb, (PTR)tPjCb, PJ_EVT_L2_MEAS_TMR); 
            if(ret == RFAILED) 
            {
               RETVALUE(ret);
            }
         }
      }
      else
      {
         RLOG0(L_DEBUG,"Meas Send  Req without configuring Memory meas");
      }
   }
   if(measCb->measType &  LPJ_L2MEAS_UL_LOSS)
   {
      /* Fill the measCfmEvt structure */
      for(qciIdx = 1; qciIdx < LPJ_MAX_QCI; qciIdx++)
      {
         U8 qci = measCb->qci[qciIdx];
         if(qci > 0)
         {   
            if(measCb->measType &  LPJ_L2MEAS_UL_LOSS)
            {
               U64 totPkts, lostPkts;

               lostPkts = (U64)(measCb->measData[qci].ulLoss.val);
               totPkts = (U64)(measCb->measData[qci].ulLoss.numPkts);

               measCfmEvt.measCfm[measCfmEvt.numQciCfm].qci = qci;
               measCfmEvt.measCfm[measCfmEvt.numQciCfm].ulLoss = 0;
               if (totPkts > 0)
               {
                  /* ul Loss  = num of Pkts lost * 10^6 / total no of pkts. */
                  measCfmEvt.measCfm[measCfmEvt.numQciCfm].ulLoss = (((U64)(lostPkts  * ((U64)1000000))) / totPkts);

                  measCb->measData[qci].ulLoss.val     = 0;
                  measCb->measData[qci].ulLoss.numPkts = 0;
               }
            }
            if (measCb->measData[qci].totDrbsPerQci == 0)
            {
               measCb->qci[qciIdx] = 0;
            }
            measCfmEvt.numQciCfm++;
         }
      }
   }
   if((measCb->measType &  LPJ_L2CPU_PERCORE_STATS) &&(tPjCb->u.ulCb->pjL2Cb.cpuUtilizationMask & LPJ_L2CPU_PERCORE_STATS))
   {  
      CmCpuStatsInfo *cpuInfo = &measCb->cpuInfo;
      for(coreIdx = 0; (coreIdx < cpuInfo->numCores) && (coreIdx < CM_MAX_CPU_CORES); coreIdx++)
      {
         measCfmEvt.cmLteCpuInfo[coreIdx].avgCpuUtil = cpuInfo->cpuUtil[coreIdx].totCpuUtil / cpuInfo->cpuUtil[coreIdx].numSamples ;  
         measCfmEvt.cmLteCpuInfo[coreIdx].maxCpuUtil = cpuInfo->cpuUtil[coreIdx].maxCpuUtil ;
         cpuInfo->cpuUtil[coreIdx].totCpuUtil = 0;
         cpuInfo->cpuUtil[coreIdx].maxCpuUtil = 0; 
         cpuInfo->cpuUtil[coreIdx].numSamples = 0;
      }
      measCfmEvt.numCores = cpuInfo->numCores;
      cpuInfo->numCores = 0;
      if(FALSE == (pjChkTmr(tPjCb, (PTR)tPjCb, PJ_EVT_L2_MEAS_TMR)))
      {
         /* starting measurement timer */
         ret = pjStartTmr(tPjCb, (PTR)tPjCb, PJ_EVT_L2_MEAS_TMR); 
         if(ret == RFAILED) 
         {
            RETVALUE(ret);
         }
      }
   } 
   /* Send Measurement confirmation to layer manager */
   pjUtlSndUlL2MeasNCfm(tPjCb, &measCfmEvt);

   RETVALUE(ROK);
} /* PjUtlPrepNSndL2MeasSendCfm */

/**
@brief 
This function processes L2 Measurement Send request received from the layer manager.
After receving this request, RLC sends L2 Measurement
 *  @param[in] pst      post structure
 *  @param[in] measType meas Type 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

#ifdef ANSI
PUBLIC S16 PjMiLpjL2MeasSendReq
(
Pst            *pst,
U16            measType
)
#else
PUBLIC S16 PjMiLpjL2MeasSendReq (pst, measType)
Pst            *pst;
U16            measType;
#endif
{
   TRC3(PjMiLpjL2MeasSendReq);

   if((measType & LPJ_L2MEAS_UL_LOSS) || 
      (measType & LPJ_L2MEM_PERPOOL_STATS) || 
      (measType & LPJ_L2CPU_PERCORE_STATS))
   {
      U8             cntr;
      PjCb           *tPjCb     = PJ_GET_PJCB(pst->dstInst);
      PjL2MeasEvtCb  *measEvtCb = NULLP;

      if((measType & LPJ_L2MEM_PERPOOL_STATS) && 
         (tPjCb->u.ulCb->pjL2Cb.memUtilizationMask & LPJ_L2MEM_PERPOOL_STATS))
      {
         if(TRUE == (pjChkTmr(tPjCb, (PTR)tPjCb, PJ_EVT_L2_MEAS_TMR)))
         {
   	   /* stopping timer */
   	   pjStopTmr(tPjCb, (PTR)tPjCb, PJ_EVT_L2_MEAS_TMR);
         }
      }
      if((measType & LPJ_L2CPU_PERCORE_STATS) && 
         (tPjCb->u.ulCb->pjL2Cb.cpuUtilizationMask & LPJ_L2CPU_PERCORE_STATS))
      {
         if(TRUE == (pjChkTmr(tPjCb, (PTR)tPjCb, PJ_EVT_L2_MEAS_TMR)))
         {
   	   /* stopping timer */
   	   pjStopTmr(tPjCb, (PTR)tPjCb, PJ_EVT_L2_MEAS_TMR);
         }
 
      }
      for(cntr = 0; cntr < PJ_MAX_L2MEAS_EVT; cntr++)
      {
         measEvtCb = &tPjCb->u.ulCb->pjL2Cb.pjL2EvtCb[cntr];
         if(measEvtCb->measCb.measType & measType)
         {
            PjUtlPrepNSndL2MeasSendCfm(tPjCb, measEvtCb);
         } 
      }
   }
   RETVALUE(ROK);
}
#ifdef ANSI
PUBLIC Void UpdateMemInfo
(
PjL2Cb *tPjL2Cb
)
#else
PUBLIC Void UpdateMemInfo(tPjL2Cb)
PjL2Cb *tPjL2Cb;
#endif
{
   U8             cntr;
   PjL2MeasEvtCb  *measEvtCb = NULLP;
   for(cntr = 0; cntr < PJ_MAX_L2MEAS_EVT; cntr++)
   {
      measEvtCb = &tPjL2Cb->pjL2EvtCb[cntr];
      if(measEvtCb->measCb.measType & LPJ_L2MEM_PERPOOL_STATS)
      {
         /* Update Memory Info for SSI Memory */
         cmUpdateSsiMemInfo(&(measEvtCb->measCb.memInfo));

         //UpdateSocMemInfo(CM_L2_MEM_UTIL_AREAIDX, &(measEvtCb->measCb.memInfo));
         break;
      } 
   }
}

#ifdef ANSI
PUBLIC S16 UpdateCpuInfo
(
PjL2Cb *tPjL2Cb
)
#else
PUBLIC S16 UpdateCpuInfo(tPjL2Cb)
PjL2Cb *tPjL2Cb;
#endif
{
   U8             cntr;
   PjL2MeasEvtCb  *measEvtCb = NULLP;
   for(cntr = 0; cntr < PJ_MAX_L2MEAS_EVT; cntr++)
   {
      measEvtCb = &tPjL2Cb->pjL2EvtCb[cntr];
      if(measEvtCb->measCb.measType & LPJ_L2CPU_PERCORE_STATS)
      {
         /* Update CpuUtilization Info */
         //UpdateSocCpuInfo(&(measEvtCb->measCb.cpuInfo), CM_L2_CPU_UTIL);
         break;
      } 
   }
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 pjUtlMemCpuHdlTmrExp 
(
PjCb          *tPjCb,
PjL2Cb *tPjL2Cb
)
#else
PUBLIC S16 pjUtlMemCpuHdlTmrExp(tPjCb,tPjL2Cb)
PjCb   *tPjCb;
PjL2Cb *tPjL2Cb;
#endif
{
   S16 ret = ROK;
   if(tPjCb->u.ulCb->pjL2Cb.memUtilizationMask & LPJ_L2MEM_PERPOOL_STATS)
   {
      /* calling memory Info */
      UpdateMemInfo(tPjL2Cb);
   }
   if(tPjCb->u.ulCb->pjL2Cb.cpuUtilizationMask & LPJ_L2CPU_PERCORE_STATS)
   {
      UpdateCpuInfo(tPjL2Cb);
   }
   /*TODO: call cpu util */
   /* starting measurement timer */
   ret = pjStartTmr(tPjCb, (PTR)tPjCb, PJ_EVT_L2_MEAS_TMR);
   if(ret == RFAILED) 
   {
      RETVALUE(ret);
   }
   RETVALUE(ROK);
}


#endif /* LTE_L2_MEAS */
/**
 * @brief
      This function configures the PDCP data sap
 *
 *  @param[in] cfg         PDCP LM Sap structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP
 *                   LCM_REASON_INVALID_SAP 
 *                   LCM_REASON_RECONFIG_FAIL 
 */
#ifdef ANSI
PRIVATE S16 pjLmmPjuSapCfg
(
PjCb       *gCb,
PjSapCfg   *cfg               /* SAP control block */
)
#else
PRIVATE S16 pjLmmPjuSapCfg(gCb, cfg)
PjCb       *gCb;
PjSapCfg   *cfg;              /* SAP control block */
#endif
{
   PjPjuSapCb  *pjuSap;
   
   TRC2(pjLmmPjuSapCfg)

   /* Validate config parameters */
   if (cfg->sapId >= PJ_MAX_PJUSAPS )
   {
      QU_CRITLOG(QU_LOG_MESSAGE,"PjuSap sapId [%d] >= [2]",cfg->sapId);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      pjuSap = &(gCb->u.ulCb->pjuSap[cfg->sapId]);
   }
   else
   {
      pjuSap = &(gCb->u.dlCb->pjuSap[cfg->sapId]);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if(pjuSap->state  != PJ_SAP_NOT_CFG)
   {
      RLOG0(L_ERROR, "Invalid pjuSap State");
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   
   /* Fill the parameters */
   pjuSap->pst.selector = cfg->selector;
   pjuSap->pst.route = cfg->route;
   pjuSap->pst.prior =  cfg->priority;
   /* pj002.201 With multicore support layer shall use the assigned region
      and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   pjuSap->pst.region = gCb->init.region; 
   pjuSap->pst.pool = gCb->init.pool;
#else
   pjuSap->pst.region = cfg->mem.region; 
   pjuSap->pst.pool = cfg->mem.pool;
#endif
   pjuSap->pst.dstProcId = cfg->procId;
   pjuSap->pst.dstEnt = cfg->ent;
   pjuSap->pst.dstInst = cfg->inst;
   pjuSap->pst.srcProcId = gCb->init.procId;
   pjuSap->pst.srcEnt = gCb->init.ent;
   pjuSap->pst.srcInst = gCb->init.inst;
   pjuSap->pst.event = EVTNONE;
   pjuSap->spId = cfg->sapId;
   /* QCOM_WORKAROUND: Fix for UL data from PDCP to DAM */
   /* Remove this fix once fix is done in L3 */
   pjuSap->pst.dstProcId = 1;
   pjuSap->pst.dstEnt = ENTEU;

   /* make sap unbound initially */
   pjuSap->state = PJ_SAP_CFG;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmPjuSapCfg */

/**
 * @brief
      This function configures the PDCP / RLC data sap
 *
 *  @param[in] cfg         PDCP LM Sap structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP
 *                   LCM_REASON_INVALID_SAP 
 *                   LCM_REASON_RECONFIG_FAIL 
 */
#ifdef ANSI
PRIVATE S16 pjLmmKwuSapCfg
(
PjCb       *gCb,
PjSapCfg   *cfg               /* SAP control block */
)
#else
PRIVATE S16 pjLmmKwuSapCfg(gCb, cfg)
PjCb       *gCb;
PjSapCfg   *cfg;              /* SAP control block */
#endif
{
   PjKwuSapCb  *kwuSap;
   PjPjuSapCb  *pjuSap;
   
   TRC2(pjLmmKwuSapCfg)
   
   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
          pjuSap = &(gCb->u.ulCb->pjuSap[cfg->sapId]);
   }
   else
   {
          pjuSap = &(gCb->u.dlCb->pjuSap[cfg->sapId]);
   }

   /* Validate config parameters */
   if (cfg->sapId >= PJ_MAX_KWUSAPS )
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      kwuSap = &(gCb->u.ulCb->kwuSap[cfg->sapId]);
   }
   else
   {
      kwuSap = &(gCb->u.dlCb->kwuSap[cfg->sapId]);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if(kwuSap->state  != PJ_SAP_NOT_CFG)
   {
      RLOG0(L_ERROR, "Invalid kwuSap State");
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   
   /* Fill the parameters */
   kwuSap->pst.selector = cfg->selector;
   kwuSap->pst.route = cfg->route;
   kwuSap->pst.prior =  cfg->priority;
   /* pj002.201 With multicore support layer shall use the assigned region
      and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   kwuSap->pst.region = gCb->init.region; 
   kwuSap->pst.pool = gCb->init.pool;
#else
   kwuSap->pst.region = cfg->mem.region; 
   kwuSap->pst.pool = cfg->mem.pool;
#endif
   kwuSap->pst.dstProcId = cfg->procId;
   kwuSap->pst.dstEnt = cfg->ent;
   kwuSap->pst.dstInst = cfg->inst;
   kwuSap->pst.srcProcId = gCb->init.procId;
   kwuSap->pst.srcEnt = gCb->init.ent;
   kwuSap->pst.srcInst = gCb->init.inst;
   kwuSap->pst.event = EVTNONE;
   kwuSap->suId = cfg->sapId;

   /* make sap unbound initially */
   kwuSap->bndTmrInt = cfg->bndTmrIntvl;
   cmInitTimers(&kwuSap->bndTmr,PJ_MAX_KWUSAP_TMR); 
   kwuSap->state = PJ_SAP_CFG;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmKwuSapCfg */

/**
 * @brief
      This function configures the PDCP / RLC data sap
 *
 *  @param[in] cfg         PDCP LM Sap structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP
 *                   LCM_REASON_INVALID_SAP 
 *                   LCM_REASON_RECONFIG_FAIL 
 */
#ifdef ANSI
PRIVATE S16 pjLmmUdxSapCfg
(
PjCb       *gCb,
PjSapCfg   *cfg               /* SAP control block */
)
#else
PRIVATE S16 pjLmmUdxSapCfg(gCb, cfg)
PjCb       *gCb;
PjSapCfg   *cfg;              /* SAP control block */
#endif
{
   PjPjuSapCb  *pjuSap;

   TRC2(pjLmmUdxSapCfg)
   
   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
        pjuSap = &(gCb->u.ulCb->pjuSap[cfg->sapId]);
   }
   else
   {
        pjuSap = &(gCb->u.dlCb->pjuSap[cfg->sapId]);
   }

   /* Validate config parameters */
   if (cfg->sapId >= PJ_MAX_UDXSAPS )
   {
      QU_CRITLOG(QU_LOG_MESSAGE,"UdxSap sapId [%d] >= [1]",cfg->sapId);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      PjUdxUlSapCb *udxSap;
      udxSap = (gCb->u.ulCb->udxUlSap + cfg->sapId);
#if (ERRCLASS & ERRCLS_INT_PAR)
      /* Check for reconfiguration */
      if(udxSap->state  != PJ_SAP_NOT_CFG)
      {
         RLOG0(L_ERROR, "Invalid udxSap State");
         RETVALUE(LCM_REASON_RECONFIG_FAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   
      /* Fill the parameters */
      udxSap->pst.selector = cfg->selector;
      udxSap->pst.route = cfg->route;
      udxSap->pst.prior =  cfg->priority;
      /* pj002.201 With multicore support layer shall use the assigned region
         and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
      udxSap->pst.region = gCb->init.region; 
      udxSap->pst.pool = gCb->init.pool;
#else
      udxSap->pst.region = cfg->mem.region; 
      udxSap->pst.pool = cfg->mem.pool;
#endif
      udxSap->pst.dstProcId = cfg->procId;
      udxSap->pst.dstEnt = cfg->ent;
      udxSap->pst.dstInst = cfg->inst;
      udxSap->pst.srcProcId = gCb->init.procId;
      udxSap->pst.srcEnt = gCb->init.ent;
      udxSap->pst.srcInst = gCb->init.inst;
      udxSap->pst.event = EVTNONE;
      udxSap->suId = cfg->sapId;

      /* make sap unbound initially */
      udxSap->bndTmrInt = cfg->bndTmrIntvl;
      cmInitTimers(&udxSap->bndTmr,PJ_MAX_UDXSAP_TMR); 
      udxSap->state = PJ_SAP_CFG;

   }
   else
   {
      PjUdxDlSapCb *udxSap;
      udxSap = (gCb->u.dlCb->udxDlSap + cfg->sapId);
#if (ERRCLASS & ERRCLS_INT_PAR)
      /* Check for reconfiguration */
      if(udxSap->state  != PJ_SAP_NOT_CFG)
      {
         RLOG0(L_ERROR, "Invalid udxSap State");
         RETVALUE(LCM_REASON_RECONFIG_FAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   
      /* Fill the parameters */
      udxSap->pst.selector = cfg->selector;
      udxSap->pst.route = cfg->route;
      udxSap->pst.prior =  cfg->priority;
      /* pj002.201 With multicore support layer shall use the assigned region
         and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
      udxSap->pst.region = gCb->init.region; 
      udxSap->pst.pool = gCb->init.pool;
#else
      udxSap->pst.region = cfg->mem.region; 
      udxSap->pst.pool = cfg->mem.pool;
#endif
      udxSap->pst.dstProcId = cfg->procId;
      udxSap->pst.dstEnt = cfg->ent;
      udxSap->pst.dstInst = cfg->inst;
      udxSap->pst.srcProcId = gCb->init.procId;
      udxSap->pst.srcEnt = gCb->init.ent;
      udxSap->pst.srcInst = gCb->init.inst;
      udxSap->pst.event = EVTNONE;
      udxSap->suId = cfg->sapId;
      udxSap->state = PJ_SAP_CFG;
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmUdxSapCfg */



/**
 * @brief
      This function configures the PDCP control sap
 *
 *  @param[in] cfg         PDCP LM Sap structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP
 *                   LCM_REASON_INVALID_SAP 
 *                   LCM_REASON_RECONFIG_FAIL 
 */
#ifdef ANSI
PRIVATE S16 pjLmmCpjSapCfg
(
PjCb       *gCb,
PjSapCfg   *cfg               /* SAP control block */
)
#else
PRIVATE S16 pjLmmCpjSapCfg(gCb,cfg)
PjCb       *gCb;
PjSapCfg   *cfg;              /* SAP control block */
#endif
{
   PjCpjSapCb  *cpjSap;
   
   TRC2(pjLmmCpjSapCfg)

   /* Validate config parameters */
   if (cfg->sapId >= PJ_MAX_CPJSAPS )
   {
      QU_CRITLOG(QU_LOG_MESSAGE,"CpjSap sapId [%d] >= [1]",cfg->sapId);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      cpjSap = &(gCb->u.ulCb->cpjSap);
   }
   else
   {
      QU_CRITLOG(QU_LOG_MESSAGE,"CpjSap Mode [%d] != [1]",gCb->pjGenCfg.mode);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if(cpjSap->state  != PJ_SAP_NOT_CFG)
   {
      RLOG0(L_ERROR, "Invalid cpjSap State");
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   
   /* Fill the parameters */
   cpjSap->pst.selector = cfg->selector;
   cpjSap->pst.route = cfg->route;
   cpjSap->pst.prior =  cfg->priority;
   /* pj002.201 With multicore support layer shall use the assigned region
      and pool from SSI */
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   cpjSap->pst.region = gCb->init.region; 
   cpjSap->pst.pool = gCb->init.pool;
#else
   cpjSap->pst.region = cfg->mem.region; 
   cpjSap->pst.pool = cfg->mem.pool;
#endif
   cpjSap->pst.dstProcId = cfg->procId;
   cpjSap->pst.dstEnt = cfg->ent;
   cpjSap->pst.dstInst = cfg->inst;
   cpjSap->pst.srcProcId = gCb->init.procId;
   cpjSap->pst.srcEnt = gCb->init.ent;
   cpjSap->pst.srcInst = gCb->init.inst;
   cpjSap->pst.event = EVTNONE;
   cpjSap->spId = cfg->sapId;

   /* make sap unbound initially */
   cpjSap->state = PJ_SAP_CFG;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmCpjSapCfg */

/**
 * @brief
     Validates the PDCP general control parameters 
 *
 *  @param[in] cntrl PDCP LM structure 
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SUBACTION
 *                   LCM_REASON_INVALID_ACTION
 */
#ifdef ANSI
PRIVATE S16 pjLmmValidateGenCntrl
(
PjMngmt *cntrl                 /* PDCP LM structure */
)
#else
PRIVATE S16 pjLmmValidateGenCntrl(cntrl)
PjMngmt *cntrl;                /* PDCP LM structure */
#endif
{
   S16 reason;                 /* reason for failure */
   U8 action;                  /* action parameter */
   U8 sAction;                 /* subaction field */


   reason = LCM_REASON_NOT_APPL;

   TRC2(pjLmmValidateGenCntrl);

   action = cntrl->t.cntrl.action;
   sAction = cntrl->t.cntrl.subAction;

   switch (action)
   {
      case AENA:
      case ADISIMM:
         if ((sAction != SAUSTA)
            && (sAction != SADBG)
            && (sAction != SATRC))
         {
            reason = LCM_REASON_INVALID_SUBACTION;
         }
         break;
      case ASHUTDOWN:
         break;
      default:
         reason = LCM_REASON_INVALID_ACTION;
         break;
   }
   
   RETVALUE(reason);
} /* pjLmmValidateGenCntrl */

/**
 * @brief
      Delete all SAPs and Control Blocks
 *
 * @b Description
 *       Shutdown of PDCP happens at modules level, tm, um and am modules will
 *       be shutdown using utility functin and dbm shutdown will clean up the
 *       SAPs, control blocks and lists. 
 *
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_HASHING_FAILED 
 */
#ifdef ANSI
PRIVATE S16 pjLmmShutdown
(
PjCb   *gCb
)
#else
PRIVATE S16 pjLmmShutdown(gCb)
PjCb   *gCb;
#endif
{
   TRC2(pjLmmShutdown)
   S16 ret;
   /* Call PDCP DBM Cleanup function.
    * RLC LM shutdown takes care of cleanup of Gloabal and RLC resources */
   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      ret = pjUtlUlShutdown(gCb);
    
#ifdef TENB_AS_SECURITY
      PjLibObdUlCloseReq();
#endif
   }
   else
   {
      ret = pjUtlDlShutdown(gCb);
      
#ifdef TENB_AS_SECURITY
      PjLibObdDlCloseReq();
#endif
   }
   pjLmmCleanGblRsrcs(gCb);

   if(ret != ROK)
   {
      RETVALUE(ret);
   }
   else
   {   
   RETVALUE(LCM_REASON_NOT_APPL);
   }
} /* end of pjLmmShutdown */

/**
 * @brief
     Function processes the general control request 
 *
 *  @param[in] cntrl PDCP LM structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SUBACTION
 *                   LCM_REASON_INVALID_ACTION
 */
#ifdef ANSI
PRIVATE S16 pjLmmGenCntrl
(
PjCb    *gCb,
PjMngmt *cntrl                 /* PDCP LM structure */
)
#else
PRIVATE S16 pjLmmGenCntrl(gCb,cntrl)
PjCb    *gCb;
PjMngmt *cntrl;                /* PDCP LM structure */
#endif
{
   U8 action;            /* action parameter */
   U8 sAction;           /* subaction field */
   PjTrcCntrl *trcCntrl; /* trace */

#ifdef DEBUGP
   PjDbgCntrl *dbgCntrl; /* debug */
#endif /* DEBUGP */

   S16   reason;         /* reason for failure */
   
   TRC2(pjLmmGenCntrl);

   reason = LCM_REASON_NOT_APPL;

   /* Validate control parameters */
   reason = pjLmmValidateGenCntrl (cntrl);

   action = cntrl->t.cntrl.action;
   sAction = cntrl->t.cntrl.subAction;

   if (reason != LCM_REASON_NOT_APPL)
   {
      RETVALUE(reason);
   }

   switch(action)
   {
      case AENA:
      {
         switch(sAction)
         {
            case SAUSTA:
            {
               PJ_SET_USTA_FLAG(gCb, TRUE);
               break;
            }
            case SADBG:
            {
#ifdef DEBUGP
               dbgCntrl = &cntrl->t.cntrl.s.dbgCntrl;
               PJ_GET_DBG_MASK(gCb) |= dbgCntrl->dbgMask;
#endif /* DEBUGP */
               break;
            }
            case SATRC:
            {
               trcCntrl = &cntrl->t.cntrl.s.trcCntrl;
               gCb->init.trc = TRUE;
               gCb->trcLen = trcCntrl->trcLen;
               (gCb->trcMask) |= trcCntrl->trcMask;
                break;
            }
         }
         break;
      }
      case ADISIMM:
      {
         switch(sAction)
         {
            case SAUSTA:
            {
               gCb->init.usta = FALSE;
            }
            break;
            case SADBG:
            {
#ifdef DEBUGP
               dbgCntrl = &cntrl->t.cntrl.s.dbgCntrl;
               PJ_GET_DBG_MASK(gCb) &= ~(dbgCntrl->dbgMask);
#endif /* DEBUGP */
            }
            break;
            case SATRC:
            {
               trcCntrl = &cntrl->t.cntrl.s.trcCntrl;
               gCb->init.trc = FALSE;
               gCb->trcMask &= ~(trcCntrl->trcMask);
            }
            break;
         }
         break;
      }
      case ASHUTDOWN:
      {
         reason = pjLmmShutdown(gCb);
         break;
      }
   }
   RETVALUE(reason);
} /* pjLmmGenCntrl */

/**
 * @brief
     Bind/Unbind RLC upper sap 
 *
 *  @param[in] cntrl RLC LM structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP 
 *                   LCM_REASON_INVALID_STATE
 *                   LCM_REASON_INVALID_ACTION 
 */
#ifdef ANSI
PRIVATE S16 pjLmmUdxSapCntrl
(
PjCb    *gCb,
PjMngmt *cntrl                 /* RLC LM structure */
)
#else
PRIVATE S16 pjLmmUdxSapCntrl(gCb,cntrl)
PjCb    *gCb;
PjMngmt *cntrl;                /* RLC LM structure */
#endif
{
   U8         action;                  /* action parameter */
   S16        reason;                 /* reason for failure */
   PjUdxUlSapCb *udxSap;                /* udx sap pointer */
   
   TRC2(kwLmmUdxSapCntrl)
   

   reason = LCM_REASON_NOT_APPL;
   action = cntrl->t.cntrl.action;

   /* validate SuId */
   if((cntrl->t.cntrl.s.sapCntrl.suId < 0)
   || (cntrl->t.cntrl.s.sapCntrl.suId >= PJ_MAX_KWUSAPS))
   {
      reason = LCM_REASON_INVALID_SAP;
      RETVALUE(reason);
   }


   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      /*udxSap = &(gCb->u.ulCb->udxUlSap[cntrl->t.cntrl.s.sapCntrl.suId]);*/
      udxSap = (gCb->u.ulCb->udxUlSap + cntrl->t.cntrl.s.sapCntrl.suId);
   }
   else
   {
      reason = LCM_REASON_INVALID_SAP;
      RETVALUE(reason);
   }


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate SAP */
   if(udxSap->state == PJ_SAP_NOT_CFG)
   {
      RLOG0(L_ERROR, "Invalid udxSap State");
      reason = LCM_REASON_INVALID_STATE;
      RETVALUE(reason);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   switch(action)
   {
      case ABND:
      {
         if(udxSap->state != PJ_SAP_BND)
         {
            S16 ret;
            /* start timer to wait for bind confirm */
            ret = pjStartTmr(gCb, (PTR)udxSap, PJ_EVT_WAIT_UDX_BNDCFM); 
            if(ret == RFAILED) 
            {
               reason = LCM_REASON_REGTMR_FAIL; 
            } 
            else
            {
            udxSap->state = PJ_SAP_BINDING;
           /* udxSap->spId = cntrl->t.cntrl.s.sapCntrl.spId; */
            PjUlUdxBndReq(&(udxSap->pst), udxSap->suId, udxSap->spId);
         }
         }
         else
         {
            /* control request received for an already bound SAP */
            reason = LCM_REASON_INVALID_STATE;
         }
      }
      break;
      case AUBND:
      {
         /* make the state of RGUSAP is configure but not bound */
         udxSap->state = PJ_SAP_CFG;
         PjUlUdxUbndReq(&(udxSap->pst), udxSap->spId, 0);
      }
      break;
      default:
         reason = LCM_REASON_INVALID_ACTION;
         break;
   } /* end of switch */

   RETVALUE(reason);
} /* pjLmmUdxSapCntrl */


/**
 * @brief
     Bind/Unbind RLC upper sap 
 *
 *  @param[in] cntrl RLC LM structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_SAP 
 *                   LCM_REASON_INVALID_STATE
 *                   LCM_REASON_INVALID_ACTION 
 */
#ifdef ANSI
PRIVATE S16 pjLmmLSapCntrl
(
PjCb    *gCb,
PjMngmt *cntrl                 /* RLC LM structure */
)
#else
PRIVATE S16 pjLmmLSapCntrl(gCb,cntrl)
PjCb    *gCb;
PjMngmt *cntrl;                /* RLC LM structure */
#endif
{
   U8         action;                  /* action parameter */
   S16        reason;                 /* reason for failure */
   PjKwuSapCb *kwuSap;                /* rgu sap pointer */

   TRC2(kwLmmLSapCntrl)

   reason = LCM_REASON_NOT_APPL;
   action = cntrl->t.cntrl.action;

   /* validate SuId */
   if((cntrl->t.cntrl.s.sapCntrl.suId < 0)
   || (cntrl->t.cntrl.s.sapCntrl.suId >= PJ_MAX_KWUSAPS))
   {
      reason = LCM_REASON_INVALID_SAP;
      RETVALUE(reason);
   }

   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      kwuSap = &(gCb->u.ulCb->kwuSap[cntrl->t.cntrl.s.sapCntrl.suId]);
   }
   else  
   {
      kwuSap = &(gCb->u.dlCb->kwuSap[cntrl->t.cntrl.s.sapCntrl.suId]);
   }


#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate SAP */
   if(kwuSap->state == PJ_SAP_NOT_CFG)
   {
      RLOG0(L_ERROR, "Invalid kwuSap State");
      reason = LCM_REASON_INVALID_STATE;
      RETVALUE(reason);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   switch(action)
   {
      case ABND:
      {
         if(kwuSap->state != PJ_SAP_BND)
         {
            S16 ret;
            /* start timer to wait for bind confirm */
            ret = pjStartTmr(gCb, (PTR)kwuSap, PJ_EVT_WAIT_KWU_BNDCFM);
            if(ret == RFAILED) 
            {
               reason = LCM_REASON_REGTMR_FAIL; 
            } 
            else
            {
            kwuSap->state = PJ_SAP_BINDING;
            kwuSap->spId = cntrl->t.cntrl.s.sapCntrl.spId;
            PjLiKwuBndReq(&(kwuSap->pst), kwuSap->suId, kwuSap->spId);
         }
         }
         else
         {
            /* control request received for an already bound SAP */
            reason = LCM_REASON_INVALID_STATE;
         }
      }
      break;
      case AUBND:
      {
         /* make the state of RGUSAP is configure but not bound */
         kwuSap->state = PJ_SAP_CFG;
         PjLiKwuUbndReq(&(kwuSap->pst), kwuSap->spId, 0);
      }
      break;
      default:
         reason = LCM_REASON_INVALID_ACTION;
         break;
   } /* end of switch */

   RETVALUE(reason);
} /* pjLmmLSapCntrl */


/**
 * @brief
     forms Lm Cfm based on the return values 
 *
 *  @param[in] cntrl PDCP LM structure
 *  @return  S16
 *      -# Success : ROK 
 *      -# Failure : RFAILED
 */
#ifdef ANSI
PRIVATE S16 pjLmmFormLmCfm
(
Pst      *pst,
Header   *hdr,
Reason   reason
)
#else
PRIVATE S16 pjLmmFormLmCfm(pst, hdr, reason)
Pst      *pst;
Header   *hdr;
Reason   reason;
#endif
{
   PjMngmt  rCfm;
   S16      ret;
   TRC3(pjLmmFormLmCfm)

   ret = ROK;

   if (reason == LCM_REASON_NOT_APPL)
   {
      rCfm.cfm.status = LCM_PRIM_OK;
      rCfm.cfm.reason = LCM_REASON_NOT_APPL;

      ret = ROK;
   }
   else
   {
      rCfm.cfm.status = LCM_PRIM_NOK;
      rCfm.cfm.reason = reason;

      ret = RFAILED;
   }

   pjLmmSendCfm(pst, &rCfm, TCNTRL, hdr);

   RETVALUE(ret);
} /* pjLmmFormLmCfm */

/**
 * @brief
     Function gather the general PJU SAP status 
 *
 *  @param[in] sta LM PJU Sap Status structure 
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 pjLmmGetPjuSapSta 
(
PjCb        *gCb,
PjPjuSapSta *sta                /* PJU SAP status */
)
#else
PRIVATE S16 pjLmmGetPjuSapSta(gCb,sta)
PjCb        *gCb;
PjPjuSapSta *sta;               /* PJU SAP status */
#endif
{
   PjPjuSapCb *pjuSap;
   TRC2(pjLmmGetPjuSapSta);

   /* Validate config parameters */
   if (sta->spId >= PJ_MAX_PJUSAPS )
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      pjuSap = &(gCb->u.ulCb->pjuSap[sta->spId]);
   }
   else
   {
      pjuSap = &(gCb->u.dlCb->pjuSap[sta->spId]);
   }


   /* Get Sap control block */
   sta->state = pjuSap->state;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmGetPjuSapSta */

/**
 * @brief
     Function gather the general PJU SAP status 
 *
 *  @param[in] sta LM PJU Sap Status structure 
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 pjLmmGetKwuSapSta 
(
PjCb        *gCb,
PjKwuSapSta *sta                /* PJU SAP status */
)
#else
PRIVATE S16 pjLmmGetKwuSapSta(gCb,sta)
PjCb        *gCb;
PjKwuSapSta *sta;               /* PJU SAP status */
#endif
{
   PjKwuSapCb *kwuSap;
   TRC2(pjLmmGetKwuSapSta);

   /* Validate config parameters */
   if (sta->spId >= PJ_MAX_PJUSAPS )
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      kwuSap = &(gCb->u.ulCb->kwuSap[sta->spId]);
   }
   else
   {
      kwuSap = &(gCb->u.dlCb->kwuSap[sta->spId]);
   }

   /* Get Sap control block */
   sta->state = kwuSap->state;

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmGetKwuSapSta */

/**
 * @brief
     Function gather the general PJU SAP status 
 *
 *  @param[in] sta LM PJU Sap Status structure 
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 pjLmmGetUdxSapSta 
(
PjCb        *gCb,
PjUdxSapSta *sta                /* PJU SAP status */
)
#else
PRIVATE S16 pjLmmGetUdxSapSta(gCb,sta)
PjCb        *gCb;
PjUdxSapSta *sta;               /* PJU SAP status */
#endif
{
   PjUdxUlSapCb *udxUlSap;   
   PjUdxDlSapCb *udxDlSap;   
   TRC2(pjLmmGetUdxSapSta);

   /* Validate config parameters */
   if (sta->spId >= PJ_MAX_UDXSAPS )   /* KW_FIX */
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      udxUlSap = &(gCb->u.ulCb->udxUlSap[sta->spId]);
      /* Get Sap control block */
      sta->state = udxUlSap->state;
   }
   else
   {
      udxDlSap = &(gCb->u.dlCb->udxDlSap[sta->spId]);
      /* Get Sap control block */
      sta->state = udxDlSap->state;
   }


   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmGetKwuSapSta */


/**
 * @brief
     Function gather the general CPJ SAP status
 *
 *  @param[in] sta LM CPJ Sap Status structure
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 pjLmmGetCpjSapSta
(
PjCb        *gCb,
PjCpjSapSta *sta              /* RLU SAP status */
)
#else
PRIVATE S16 pjLmmGetCpjSapSta(sta)
PjCb        *gCb;
PjCpjSapSta *sta;             /* RLU SAP status */
#endif
{
   PjCpjSapCb *cpjSap;
   TRC2(pjLmmGetCpjSapSta);

   if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      cpjSap = &(gCb->u.ulCb->cpjSap);
   }
   else
   {
      RETVALUE(LCM_REASON_INVALID_SAP);
   }


   sta->state = cpjSap->state;

   RETVALUE(LCM_REASON_NOT_APPL);

} /* pjLmmGetCpjSapSta */

/**
 * @brief
     Gather the general statistics 
 *
 *  @param[in] sts       LM general statistics structure
 *  @param[in] action    action
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 pjLmmGetGenSts
(
PjCb     *gCb,
PjGenSts *sts,              /* general statistics structure */
Action action               /* Action */
)
#else
PRIVATE S16 pjLmmGetGenSts(gCb, sts, action)
PjCb     *gCb;
PjGenSts *sts;              /* general statistics structure */
Action action;              /* Action */
#endif
{
   PjGenSts *genSts;        /* General statistics */

   TRC2(pjLmmGetGenSts);

   /* Validate protocol parameters */
   if ((action != LPJ_ZEROSTS) && (action != LPJ_NOZEROSTS))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   PJ_MEM_CPY (sts, &(gCb->pjGenSts), sizeof (PjGenSts));
   
   if(action == LPJ_ZEROSTS)
   {
      genSts = &(gCb->pjGenSts);
      PJ_MEM_SET (genSts, 0, sizeof (PjGenSts));
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmGetGenSts */

/**
 * @brief
     Gather the SAP statistics
 *
 *  @param[in] sts     LM general statistics structure
 *  @param[in] elmnt   element 
 *  @param[in] action  action 
 *  @return  S16
 *      -# Success : LCM_REASON_NOT_APPL
 *      -# Failure : LCM_REASON_INVALID_PAR_VAL
 */
#ifdef ANSI
PRIVATE S16 pjLmmGetSapSts
(
PjCb    *gCb,
PjMngmt *sts,               /* PDCP layer management */
Elmnt elmnt,                /* element */
Action action               /* Action */
)
#else
PRIVATE S16 pjLmmGetSapSts(gCb, sts, elmnt, action)
PjCb    *gCb;
PjMngmt *sts;               /* PDCP layer management */
Elmnt elmnt;                /* element */
Action action;              /* Action */
#endif
{
   PjPjuSapCb   *pjuSapCb;      /* PDCP User SAP control block */
   PjPjuSts     *pjuSap;        /* place holder for SAP statistics */

   TRC2(pjLmmGetSapSts);

   /* Validate protocol parameters */
   if (action != LPJ_ZEROSTS && action != LPJ_NOZEROSTS)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   pjuSap = &sts->t.sts.s.pjuSap;

   /* Get Sap control block */
   /* pj005.201, modified suId to spId */
   
   pjuSapCb = (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_DL)?               
       (PjPjuSapCb*)(gCb->u.dlCb->pjuSap + pjuSap->spId):          
       (PjPjuSapCb*)(gCb->u.ulCb->pjuSap + pjuSap->spId);          


   pjuSap->rxSdus = pjuSapCb->sts.rxSdus;
   pjuSap->txSdus = pjuSapCb->sts.txSdus;

   if (action == LPJ_ZEROSTS)
   {
      PJ_MEM_SET (&pjuSapCb->sts, 0, sizeof (PjPjuSts));
   }

   SGetDateTime(&sts->t.sts.dt);

   RETVALUE(LCM_REASON_NOT_APPL);
} /* pjLmmGetSapSts */


/**
 * @brief
    This function sends Unsolicited Status Indication to the Layer Management
    Entity. 
 *
 *  @param[in] category  Category 
 *  @param[in] event     event
 *  @param[in] cause     cause
 *  @param[in] cause     ueId 
 *  @return  Void
 */
/* pj005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC Void pjLmmSendAlarm
(
PjCb     *gCb,
U16      category,  /* Category */
U16      event,     /* Alarm event */
U16      cause,     /* Alarm cause */
SuId     suId,      /* suId */
U32      ueId,      /* ueId */
U8       qci        /* qci */
)
#else
PUBLIC Void pjLmmSendAlarm(gCb, category, event, cause, suId, ueId, qci)
PjCb     *gCb;
U16      category;  /* Category */
U16      event;     /* Alarm event */
U16      cause;     /* Alarm cause */
SuId     suId;      /* suId */
U32      ueId;      /* ueId */
U8       qci;       /* qci */
#endif
#else
#ifdef ANSI
PUBLIC Void pjLmmSendAlarm
(
PjCb     *gCb,
U16      category,  /* Category */
U16      event,     /* Alarm event */
U16      cause,     /* Alarm cause */
SuId     suId,      /* suId */
U32      ueId       /* ueId */
)
#else
PUBLIC Void pjLmmSendAlarm(gCb, category, event, cause, suId, ueId)
PjCb     *gCb;
U16      category;  /* Category */
U16      event;     /* Alarm event */
U16      cause;     /* Alarm cause */
SuId     suId;      /* suId */
U32      ueId;      /* ueId */
#endif
#endif
{

   PjMngmt usta;    /* Rlc Management Structure */

   TRC2(pjLmmSendAlarm);

   if(gCb->init.usta == FALSE)
   {
      RETVOID;
   }

   /* initialize the management structure */
   PJ_MEM_SET(&usta, 0, sizeof(PjMngmt));

   usta.hdr.elmId.elmnt = STGEN;
   usta.hdr.entId.ent = gCb->init.ent;
   usta.hdr.entId.inst = gCb->init.inst;

   /* fill in the event and category */
   usta.t.usta.alarm.category = category;
   usta.t.usta.alarm.event = event;
   usta.t.usta.alarm.cause = cause;

   /* set the suId and ueId */
   usta.t.usta.ueId = ueId;
   usta.t.usta.suId = suId;
   /* pj005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   if(event == LPJ_EVT_MEAS_HALT)
   {
      usta.t.usta.qci = qci;
   }
#endif

   /* update the date and time */
   (Void) SGetDateTime(&usta.t.usta.alarm.dt);

   PjMiLpjStaInd(&(gCb->init.lmPst), &usta);

   RETVOID;

} /* pjLmmSendAlarm */

/**
 * @brief
    This function sends trace indication to LM 
 *
 *  @param[in] event    event
 *  @param[in] mBuf     meessage buffer
 *  @return S16
 *      -# Success : ROK 
 *      -# Failure : RFAILED 
 */
#ifdef ANSI
PUBLIC S16 pjLmmSendTrc
(
PjCb  *gCb,
Event event,                 /* event */
Buffer *mBuf                 /* message buffer */
)
#else
PUBLIC S16 pjLmmSendTrc(gCb, event, mBuf)
PjCb  *gCb;
Event event;                 /* event */
Buffer *mBuf;                /* message buffer */
#endif
{
   /* patch pj004.201 Corrected pjLmmSendTrc function definition */
   PjMngmt trc;              /* PDCP management control block */
   Buffer   *dstMbuf;   
   MsgLen   bufLen;
   Data     *tempBuf;
   MsgLen   tempCnt;
   Pst      pst;

   TRC2(pjLmmSendTrc);

   PJ_MEM_SET(&trc, 0, sizeof(PjMngmt));

   cmMemset((U8 *)&trc, 0, sizeof(PjMngmt));

   pst = gCb->init.lmPst;

   trc.t.trc.event = event;
   SGetDateTime(&trc.t.trc.dt);
   trc.cfm.status = LCM_PRIM_OK;
   trc.cfm.reason = LCM_REASON_NOT_APPL;
    
   if(mBuf != NULLP)
   {
     /* Check if the whole buffer is to be sent in Trace indication */
     if(gCb->trcLen == LPJ_FULL_TRACE)
     {
        if (SCpyMsgMsg(mBuf, pst.region, pst.pool, &dstMbuf)
            != ROK)
        {
           /* rg005.201 removed SPutSBuf on error */
           RLOG0(L_DEBUG,"SCpyMsgMsg Failed.");
           RETVALUE(RFAILED);
        }
        /* Send Trace Indication to Layer manager */
        PjMiLpjTrcInd(&pst, &trc, dstMbuf);
     }
     /* check if only a specified number of bytes are to be sent */
     else if(gCb->trcLen > 0)
     {
        /* Get the length of the recvd message buffer */
        if (SFndLenMsg(mBuf, &bufLen) != ROK)
        {
           RLOG0(L_DEBUG, "SFndLenMsg Failed.");
           RETVALUE(RFAILED);
        }
        /* Check if the recvd buffer size is less than request trace len */
        if(bufLen < gCb->trcLen)
        {
           /* Copy the whole of the recvd buffer in trace indication */
      
           if (SCpyMsgMsg(mBuf, pst.region, pst.pool, &dstMbuf)
              != ROK)
           {
              RLOG0(L_DEBUG, "pjLmmSendTrc(): SCpyMsgMsg Failed.");
              RETVALUE(RFAILED);
           }
         
           /* Send Trace Indication to Layer manager */
           PjMiLpjTrcInd(&pst, &trc, dstMbuf);
        }
        /* if the recvd buffer size is greater than request trace len */
        if(bufLen >= gCb->trcLen)
        {
           /* Get a temporary buffer to store the msg */
           PJ_ALLOC(gCb, tempBuf, gCb->trcLen);

#if (ERRCLASS & ERRCLS_INT_PAR)
           if(tempBuf == NULLP)
           {
              (Void) SPutSMem(PJ_GET_MEM_REGION(gCb), PJ_GET_MEM_POOL(gCb));
               RLOG0(L_FATAL, "Memory Allocation failed.");
               RETVALUE(LCM_REASON_MEM_NOAVAIL);
           }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         
           /* Copy trcLen nos of bytes from the recvd message */
           if (SCpyMsgFix(mBuf,0,gCb->trcLen,tempBuf,&tempCnt) != ROK)   
           {
              RLOG0(L_DEBUG, "SCpyMsgFix Failed.");
              RETVALUE(RFAILED);
           }

           if (SGetMsg(pst.region, pst.pool, &dstMbuf) != ROK)
           {
              RLOG0(L_FATAL, "Memory Allocation failed.");
              RETVALUE(RFAILED);
           }
           /* Copy the tempBuf data to dst mBuf */
           if (SCpyFixMsg(tempBuf,dstMbuf,0,gCb->trcLen,&tempCnt) != ROK)
           {
              RLOG0(L_DEBUG, "SCpyFixMsg Failed.");
              RETVALUE(RFAILED);
           }
  
           /* Free the memory allocated for tempBuf */
           PJ_FREE(gCb, tempBuf, gCb->trcLen);
           /* Send Trace Indication to Layer manager */
           PjMiLpjTrcInd(&pst, &trc, dstMbuf);
        }
     }
   }
   else
   {
      PjMiLpjTrcInd(&pst, &trc, mBuf);
   }

   RETVALUE(ROK);

} /* end of pjLmmSendTrc */


/*
*
*       Fun:   Activate Task - timer
*
*       Desc:  Invoked by system services to activate a task with
*              a timer tick.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  gp_lmm.c
*
*/
#ifdef ANSI
PUBLIC S16 pjActvTmr
(
Ent     ent,
Inst    inst
)
#else
PUBLIC S16 pjActvTmr(ent,inst)
Ent     ent;
Inst    inst;
#endif
{
   PjCb  *gCb; 
   TRC2(pjActvTmr)

   if (inst >= PJ_MAX_PDCP_INSTANCES) 
   {
      RETVALUE (RFAILED);
   }
   gCb = PJ_GET_PJCB(inst); 
   cmPrcTmr(&(gCb->pjTqCp), gCb->pjTq, (PFV) pjTmrExpiry);

   RETVALUE(ROK);

} /* end of pjActvTmr */

/**
 * @brief
 it deregisters the timers and deregisters the kwuSap Control blocks
 *
 *  @return  Void
 */
#ifdef ANSI
PRIVATE Void pjLmmCleanGblRsrcs
(
PjCb *gCb
)
#else
PRIVATE Void pjLmmCleanGblRsrcs(gCb)
PjCb *gCb;
#endif
{
    Size    kwSapSize;

    TRC2(pjLmmCleanGblRsrcs)

        if (gCb->init.cfgDone)
        {
            /* Deregister the timers */
            (Void) SDeregTmrMt(gCb->init.ent, gCb->init.inst,
                    (S16)gCb->pjGenCfg.timeRes, pjActvTmr);
            /*(Void) SDeregTmr(gCb->init.ent, gCb->init.inst,
              (S16)gCb->genCfg.timeRes, kwActvTmr);*/

            kwSapSize = (Size)((Size)gCb->pjGenCfg.maxKwuSaps * (Size)sizeof(PjKwuSapCb));
            if (gCb->pjGenCfg.mode == LPJ_MODE_PDCP_DL)
            {
                if (gCb->u.dlCb)
                {
                    if (gCb->u.dlCb->kwuSap != (PjKwuSapCb *)NULLP)
                    {
                        /*PJ_FREE(gCb,gCb->u.dlCb->kwuSap, kwSapSize); */
                    }

                    PJ_FREE(gCb,gCb->u.dlCb, sizeof (PjDlgCb));
                }

            }
            else
            {
                if (gCb->u.ulCb)
                {
                    if (gCb->u.ulCb->kwuSap != (PjKwuSapCb *)NULLP)
                    {
                        /*PJ_FREE(gCb,gCb->u.ulCb->kwuSap, kwSapSize);*/
                    }

                    PJ_FREE(gCb,gCb->u.ulCb, sizeof (PjUlgCb));
                }
            }

            (Void) SPutSMem(gCb->init.region, gCb->init.pool);
            gCb->init.cfgDone = FALSE;
            gCb->init.acnt = FALSE;
            gCb->init.trc = FALSE;
            gCb->init.usta = FALSE;
        }
    
    RETVOID;
} /* pjLmmCleanGblRsrcs */

/********************************************************************30**

         End of file:     eq_lmm.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:46:46 2014
         End of file:     eq_lmm.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:46:46 2014

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
/main/1      ---       gk   1. LTERLC Release 2.1
/main/2      pj002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/3      pj004.201 ap   1. Corrected pjLmmSendTrc function definition
/main/4      pj005.201 ap   1. Modified suId to spId in PjPjuSts.
                            2. Added support for L2 Measurement.
*********************************************************************91*/
