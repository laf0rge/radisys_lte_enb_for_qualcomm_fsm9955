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

        Name:    LTE - RLC - Layer Manager file

        Type:    C source file

        Desc:    It contains the following common  functions for processing
                 the all the external interfaces.
                    -- KwMiLkwCfgReq
                    -- KwMiLkwCfgCfm 
                    -- KwMiLkwCntrlReq
                    -- KwMiLkwCntrlCfm
                    -- KwMiLkwStaReq
                    -- KwMiLkwStaCfm
                    -- KwMiLkwStaInd
                    -- KwMiLkwStsReq
                    -- KwMiLkwStsCfm
                    -- KwMiLkwTrcInd 

        File:     gp_lmm.c

        Sid:      gp_lmm.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:01 2015

        Prg:     cp 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="LMM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=197;
/** @file gp_lmm.c
@brief RLC Layer Manager Module
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
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_dl.h"
#include "kw_ul.h"
#include "kw_udx.h"

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
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_udx.x"
#include "kw_dl.x"
#include "kw_ul.x"

#define KW_MODULE KW_DBGMASK_LMM

RLCStats gRlcStats;

/*********************************************************************
 *             Forward Declaration of LKW Porting Functions
 ********************************************************************/
PUBLIC KwCb *kwCb[KW_MAX_RLC_INSTANCES];
EXTERN S16 kwActvTmr ARGS ((Ent ent, Inst inst));

PRIVATE Void kwLmmSendCfm ARGS ((KwCb *gCb,Pst *pst,KwMngmt *cfm,U8 type,
                                 Header *hdr));
PRIVATE S16 kwLmmGenCfg ARGS ((KwCb *gCb, KwGenCfg *cfg));
PRIVATE S16 kwLmmCfgKwuSap ARGS ((KwCb *gCb,KwSapCfg *cfg));
PRIVATE S16 kwLmmCfgUdxSap ARGS ((KwCb *gCb,KwSapCfg *cfg));
PRIVATE S16 kwLmmCfgCkwSap ARGS ((KwCb *gCb,KwSapCfg   *cfg));
PRIVATE S16 kwLmmCfgRguSap ARGS ((KwCb *gCb,KwSapCfg   *cfg));
PRIVATE S16 kwLmmGenCntrl ARGS ((KwCb *gCb,KwMngmt *cntrl));
PRIVATE S16 kwLmmUdxSapCntrl ARGS ((KwCb *gCb,KwMngmt *cntrl));
PRIVATE S16 kwLmmLSapCntrl ARGS ((KwCb *gCb,KwMngmt *cntrl));
PRIVATE S16 kwLmmGetKwuSapSta ARGS ((KwCb *gCb,KwKwuSapSta *sta));
PRIVATE S16 kwLmmGetRguSapSta ARGS ((KwCb *gCb,KwRguSapSta *sta));
PRIVATE S16 kwLmmGetCkwCntSapSta ARGS ((KwCb *gCb,KwCkwCntSapSta *sta));
PRIVATE S16 kwLmmGetGenSts ARGS ((KwCb *gCb,KwGenSts *sts,Action action));
PRIVATE S16 kwLmmGetSapSts ARGS ((KwCb *gCb,KwMngmt *sts,Elmnt elmnt,Action 
                                  action));


/*********************************************************************
 *             Primitives for RGU interface 
 ********************************************************************/
/**
 * @brief
 *    Stores the general RLC configuration. 
 * @details
 *    It calculates the memory requirement of the Saps  
 *    It also initializes the control blocks, the hash lists and registers
 *    the timers. In case of error it rolls back and reports error in the 
 *    confirm.If the general configuration is already done it is treated as 
 *    reconfiguration, but right now reconfiguration is not supported.
 * 
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  cfm   RLC LM general config structure
 * 
 * @return  S16
 *    LCM_REASON_NOT_APPL  (SUCCESS) 
 *    LCM_REASON_MEM_NOAVAIL
 *    LCM_REASON_REGTMR_FAIL
 *    LCM_REASON_INVALID_PAR_VAL
 *    LCM_REASON_HASHING_FAILED
 */
#ifdef ANSI
PRIVATE S16 kwLmmGenCfg
(
KwCb       *gCb,
KwGenCfg   *cfg
)
#else
PRIVATE S16 kwLmmGenCfg(gCB,cfg)
KwCb       *gCb;
KwGenCfg   *cfg;
#endif
{
   Size     kwSapSize;
   Size     kwUdxSapSize;
   S16      ret;
   CmTqCp   *kwTqCp;

   TRC2(kwLmmGenCfg)

   if(cfg->maxUe > KW_MAX_UE) 
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      RLOG2(L_ERROR, "Invalid maxUe : Max [%lu] Received [%lu]", 
            KW_MAX_UE, 
            cfg->maxUe);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if(cfg->maxKwuSaps > KW_MAX_KWUSAPS)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      RLOG2(L_ERROR, "Invalid maxKwuSaps : Max [%lu] Received [%lu]", 
            KW_MAX_KWUSAPS, 
            cfg->maxKwuSaps);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if(cfg->maxUdxSaps > KW_MAX_UDXSAPS)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      RLOG2(L_ERROR, "Invalid maxUduSaps : Max [%lu] Received [%lu]", 
            KW_MAX_UDXSAPS, 
            cfg->maxUdxSaps);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   
   if(gCb->init.cfgDone == TRUE)
   {
      /* reconfiguration not supported */
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }

   gCb->genCfg = *cfg;

   /* Timer Queue Control point initialization */
   kwTqCp         = &(gCb->kwTqCp);
   kwTqCp->tmrLen = KW_TMR_LEN;
   kwTqCp->nxtEnt = 0;

   if(gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      KW_ALLOC(gCb,gCb->u.dlCb, sizeof (KwDlCb));
      if (gCb->u.dlCb == NULLP)
      {  
         RLOG0(L_FATAL,"Memory Allocation failed");   
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      
      }
      /* allocate memory to the KWU sap's */
      kwSapSize = (Size)((Size) gCb->genCfg.maxKwuSaps *
                   (Size)sizeof(KwKwuSapCb));

      KW_ALLOC(gCb,gCb->u.dlCb->kwuDlSap, kwSapSize);

#if (ERRCLASS & ERRCLS_INT_PAR)
      if(gCb->u.dlCb->kwuDlSap == NULLP)
      {
         KW_FREE(gCb,gCb->u.dlCb, sizeof (KwDlCb));
         RLOG0(L_FATAL,"Memory Allocation failed");   
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      gCb->u.dlCb->numKwuSaps = gCb->genCfg.maxKwuSaps;

      kwUdxSapSize = (Size)((Size) gCb->genCfg.maxUdxSaps *
                   (Size)sizeof(KwUdxDlSapCb));

      KW_ALLOC(gCb,gCb->u.dlCb->udxDlSap, kwUdxSapSize);
#if (ERRCLASS & ERRCLS_INT_PAR)
      if(gCb->u.dlCb->udxDlSap == NULLP)
      {
         KW_FREE(gCb,gCb->u.dlCb->kwuDlSap, kwSapSize);
         KW_FREE(gCb,gCb->u.dlCb, sizeof (KwDlCb));
         RLOG0(L_FATAL,"Memory Allocation failed");   
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      
      /* Initialize the Ue and Cell hash list */
      ret = kwDbmDlInit(gCb);
      if (ret != ROK)
      {
         KW_FREE(gCb,gCb->u.dlCb->udxDlSap, kwSapSize);
         KW_FREE(gCb,gCb->u.dlCb->kwuDlSap, kwSapSize);
         KW_FREE(gCb,gCb->u.dlCb, sizeof (KwDlCb));
         RLOG0(L_FATAL,"RLC DL Initialization failed");   
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }
      /* Register the timer */
/*Pradeep: changing the SRegTmrMt() to SRegTmr()*/
      if(SRegTmrMt(gCb->init.ent, gCb->init.inst, (U16)cfg->timeRes,
              kwActvTmr) != ROK)
      {
         KW_FREE(gCb,gCb->u.dlCb->udxDlSap, kwUdxSapSize);
         KW_FREE(gCb,gCb->u.dlCb->kwuDlSap, kwSapSize);
         KW_FREE(gCb,gCb->u.dlCb, sizeof (KwDlCb));

         RETVALUE(LCM_REASON_REGTMR_FAIL);
      }

      /* initializations for background processing of freeing memory */
      kwUtlInitToBeFreed(gCb, &(gCb->u.dlCb->toBeFreed));
      kwUtlInitializeSelfPst(gCb);

      if(SGetMsg(gCb->init.region, 
                 gCb->init.pool,
                 &(gCb->u.dlCb->selfPstMBuf)) != ROK)
      {
         KW_FREE(gCb,gCb->u.dlCb->udxDlSap, kwSapSize);
         KW_FREE(gCb,gCb->u.dlCb->kwuDlSap, kwSapSize);
         KW_FREE(gCb,gCb->u.dlCb, sizeof (KwDlCb));

         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      
      }
#ifdef LTE_L2_MEAS
      kwUtlL2MeasDlInit(gCb);
#endif
   }
   else if(gCb->genCfg.rlcMode == LKW_RLC_MODE_UL)
   {
      KW_ALLOC(gCb,gCb->u.ulCb, sizeof (KwUlCb));
      if (gCb->u.ulCb == NULLP)
      {     
         RLOG0(L_FATAL,"Memory Allocation failed");   
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      
      }
      /* allocate memory to the KWU sap's */
      kwSapSize = (Size)((Size) gCb->genCfg.maxKwuSaps *
                   (Size)sizeof(KwKwuSapCb));

      KW_ALLOC(gCb,gCb->u.ulCb->kwuUlSap, kwSapSize);

#if (ERRCLASS & ERRCLS_INT_PAR)
      if(gCb->u.ulCb->kwuUlSap == NULLP)
      {
         KW_FREE(gCb,gCb->u.ulCb, sizeof (KwUlCb));
         RLOG0(L_FATAL,"Memory Allocation failed");   
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      gCb->u.ulCb->numKwuSaps = gCb->genCfg.maxKwuSaps;

      /* allocate memory to the KWU sap's */
      kwUdxSapSize = (Size)((Size) gCb->genCfg.maxUdxSaps *
                   (Size)sizeof(KwUdxUlSapCb));

      KW_ALLOC(gCb,gCb->u.ulCb->udxUlSap, kwUdxSapSize);

#if (ERRCLASS & ERRCLS_INT_PAR)
      if(gCb->u.ulCb->kwuUlSap == NULLP)
      {
         KW_FREE(gCb,gCb->u.ulCb->kwuUlSap, kwSapSize);
         KW_FREE(gCb,gCb->u.ulCb, sizeof (KwUlCb));
         RLOG0(L_FATAL,"Memory Allocation failed");   
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */


      /* Initialize the Ue and Cell hash list */
      ret = kwDbmUlInit(gCb);
      if (ret != ROK)
      {
         KW_FREE(gCb,gCb->u.ulCb->udxUlSap, kwUdxSapSize);
         KW_FREE(gCb,gCb->u.ulCb->kwuUlSap, kwSapSize);
         KW_FREE(gCb,gCb->u.ulCb, sizeof (KwUlCb));
         RLOG0(L_FATAL,"RLC DL Initialization failed");   
      }

      /* Register the timer */

      if(SRegTmrMt(gCb->init.ent, gCb->init.inst, (U16)cfg->timeRes,
              kwActvTmr) != ROK)
      {
         KW_FREE(gCb,gCb->u.ulCb->udxUlSap, kwUdxSapSize);
         KW_FREE(gCb,gCb->u.ulCb->kwuUlSap, kwSapSize);
         KW_FREE(gCb,gCb->u.ulCb, sizeof (KwUlCb));

         RETVALUE(LCM_REASON_REGTMR_FAIL);
      }
#ifdef LTE_L2_MEAS
      kwUtlL2MeasUlInit(gCb);
#endif
   }
   else
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      RLOG0(L_ERROR, "Received Invalid RLC Mode");
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   /* Timer Initialization  */
   gCb->kwTqCp.tmrLen = KW_TMR_LEN;

   cmMemset((U8*)gCb->kwTq, NULLP, sizeof(CmTqType) * KW_TMR_LEN);

   KW_MEM_CPY(&(gCb->init.lmPst), &cfg->lmPst, sizeof(Pst));

   gCb->init.lmPst.srcProcId = gCb->init.procId;
   gCb->init.lmPst.srcEnt = gCb->init.ent;
   gCb->init.lmPst.srcInst = gCb->init.inst;
   gCb->init.lmPst.event = EVTNONE;

   /* kw002.201 For multi core and multi region,no need to reinitiailize the 
    * region again . This would be done with kwActvInit from SSI */
#if !defined(SS_MULTICORE_SUPPORT) && !defined(SS_M_PROTO_REGION)
   gCb->init.region = cfg->lmPst.region;
#endif /* !defined(SS_MULTICORE_SUPPORT) && !defined(SS_M_PROTO_REGION) */
   gCb->init.pool   = cfg->lmPst.pool;

   gCb->init.cfgDone = TRUE;

   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *     Sends confirmation message to LM Called by KwMiLkwCfgReq function
 * 
 * @param[in] gCb    RLC Instance Control Block
 * @param[in] pst    Post structure
 * @param[in] cfm    RLC LM structure 
 * @param[in] type   Type of LM message
 * @param[in] hdr    Message header
 * 
 * @return  S16
 *     -# ROK
 */
#ifdef ANSI
PRIVATE Void kwLmmSendCfm
(
KwCb      *gCb,
Pst       *pst,                
KwMngmt   *cfm,               
U8        type,              
Header    *hdr              
)
#else
PRIVATE Void kwLmmSendCfm(gCb,pst, cfm, type, hdr)
KwCb      gCb;
Pst       *pst;            
KwMngmt   *cfm;           
U8        type;          
Header    *hdr;         
#endif
{
   Pst   rPst;   /* Reply post structure */

   TRC2(kwLmmSendCfm);

   KW_MEM_SET(&rPst, 0, sizeof(Pst));   
 
   /* reply post structure for confirmation */
   if (gCb)
   {
      rPst.srcEnt = gCb->init.ent;
      rPst.srcInst = gCb->init.inst;
      cfm->hdr.entId.ent = gCb->init.ent;
      cfm->hdr.entId.inst = gCb->init.inst;
   }
   else
   {
      rPst.srcEnt = pst->dstEnt;
      rPst.srcInst = pst->dstInst;
      cfm->hdr.entId.ent = pst->dstEnt;
      cfm->hdr.entId.inst = pst->dstInst;
   }
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
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.transId = hdr->transId;

   switch(type)
   {
      case TCFG:
         KwMiLkwCfgCfm(&rPst,cfm);
         break;
      case TCNTRL:
         SGetDateTime(&(cfm->t.cntrl.dt));
         KwMiLkwCntrlCfm(&rPst,cfm);
         break;
      case TSTS:
         SGetDateTime(&(cfm->t.sts.dt));
         KwMiLkwStsCfm(&rPst,0,cfm);
         break;
      case TSSTA:
         SGetDateTime(&(cfm->t.ssta.dt));
         KwMiLkwStaCfm(&rPst,cfm);
         break;
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         RLOG0(L_ERROR, "Received Invalid Message Type");
         if(!gCb)
         {
            fflush(stdout);
         }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         break;
   }

   RETVOID; 
} 


/**
 * @brief
 *    This function processes configuration requests received from the layer 
 *    manager.
 *
 * @details
 *    The configuration requests can be for general configuration, or 
 *    configuration of RGU, KWU, and CKW SAPs. The type of the configuration 
 *    request can be determined from the elmId field in the header of the layer
 *    manager message.This function is called by the layer manager to configure
 *    RLC. 
 *
 *    This function implements the following logic: 
 *
 *    - if genCfg is not done, send negative Cfm to the layer manager; 
 *
 *    - switch (cfg->hdr.elmId.elmnt)
 *       - case STGEN 
 *          - update the genCfg field in RlCb; 
 *          - allocate the maximum static memory required by the RLC product; 
 *          - register the timer service by calling SReqTmr; 
 *          - set CfgDone for not configuring again; 
 *       - case STKWUSAP 
 *          - if all the parameters are valid, then allocate the KwuSap control 
 *            block; 
 *          - update the SAP control block with the information provided in the 
 *          - configuration request; 
 *          - send configuration confirm with the status, success; 
 *       - case STCKWSAP 
 *          - if all the parametes are valid, then allocate the CkwSap control 
 *            block; 
 *          - update the SAP control block with the information provided in the
 *            configuration request; 
 *          - send configuration confirm with the status, success; 
 *       - case STRGUSAP 
 *          - if all the parametes are valid, then allocate the rguSap control 
 *            block; 
 *          - update the SAP control block with the information provided in the
 *            configuration request; 
 *          - send configuration confirm with the status, success; 
 *       - case STUDXSAP 
 *          - if all the parametes are valid, then allocate the udxSap control 
 *            block; 
 *          - update the SAP control block with the information provided in the
 *            configuration request; 
 *          - send configuration confirm with the status, success; 
 *    - end switch;
 *     
 * @param[in] pst   post structure 
 * @param[in] cfg   LM management structure 
 * 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */

#ifdef ANSI
PUBLIC S16 KwMiLkwCfgReq 
(
Pst       *pst, 
KwMngmt   *cfg
)
#else
PUBLIC S16 KwMiLkwCfgReq (pst, cfg)
Pst       *pst;
KwMngmt   *cfg;
#endif
{
   Reason   reason;   /* failure reason */
   KwCb     *tKwCb=NULLP;

   TRC3(KwMiLkwCfgReq);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check if the instance is configured */
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      cfg->cfm.status = LCM_PRIM_NOK;
      cfg->cfm.reason = LCM_REASON_INVALID_INSTANCE;
      kwLmmSendCfm(tKwCb,pst, cfg, TCFG, &cfg->hdr);     
      RETVALUE(RFAILED);
   }
#endif
   
   tKwCb =  KW_GET_KWCB(pst->dstInst);

   if (!tKwCb)
   {
      cfg->cfm.status = LCM_PRIM_NOK;
      cfg->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      kwLmmSendCfm(tKwCb,pst, cfg, TCFG, &cfg->hdr);     
      RETVALUE(RFAILED);
   }
   /* check configuration is done or not */
   if ((tKwCb->init.cfgDone != TRUE) && 
       (cfg->hdr.elmId.elmnt != STGEN))
   {
      /*
       * if general config is not over then use pst structure
       * in primitive to communicate to stack manager
       */

      cfg->cfm.status = LCM_PRIM_NOK;
      cfg->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      kwLmmSendCfm(tKwCb,pst, cfg, TCFG, &cfg->hdr);     
      RETVALUE(RFAILED);
   }

   RLOG1(L_DEBUG, "KwMiLkwCfgReq elmId(%d)", cfg->hdr.elmId.elmnt);

   switch(cfg->hdr.elmId.elmnt)
   {
      case STGEN:
      {
         reason = kwLmmGenCfg(tKwCb,&cfg->t.cfg.s.gen);
         break;
      }
      case STKWUSAP:
      {
         reason = kwLmmCfgKwuSap(tKwCb,&cfg->t.cfg.s.sap);
         break;
      }
      case STCKWSAP:
      {
         reason = kwLmmCfgCkwSap(tKwCb,&cfg->t.cfg.s.sap);
         break;
      }
      case STRGUSAP:
      {
         reason = kwLmmCfgRguSap(tKwCb,&cfg->t.cfg.s.sap);
         break;
      }
      case STUDXSAP:
      {
         reason = kwLmmCfgUdxSap(tKwCb,&cfg->t.cfg.s.sap);
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
      cfg->cfm.status = LCM_PRIM_OK;
      cfg->cfm.reason = LCM_REASON_NOT_APPL;
      kwLmmSendCfm(tKwCb,pst, cfg, TCFG, &cfg->hdr);
      RETVALUE(ROK);
   }
   else
   {
      cfg->cfm.status = LCM_PRIM_NOK;
      cfg->cfm.reason = reason;
      kwLmmSendCfm(tKwCb,pst, cfg, TCFG, &cfg->hdr);
      RETVALUE(RFAILED);
   }
} 

/**
 * @brief 
 *    The layer manager initiates and uses the management-control procedure to 
 *    control RLC elements.The RLC control request primitive (KwMiLkwCntrlReq) 
 *    can be called more than once and at any time after the 
 *    management-configuration procedure.The control request primitive 
 *    is confirmed by a KwMiLkwCntrlCfm primitive.
 *
 * @details
 *    This function implements the following logic: 
 *  
 *    - if(cfgDone = FALSE) 
 *       - send negative confirmation to the layer manager; 
 *       - return; 
 *    - else 
 *    - switch(cntrl->hdr.elmId.elmnt) 
 *       -  case STGEN 
 *           - switch (action) 
 *           - case AENA 
 *                - switch(subAction) 
 *                - case SAUSTA 
 *                     - enable the unsolicited status flag; 
 *                     - send the control Cfm with success; 
 *                - case SADBG 
 *                     - set the debug mask; 
 *                     - send the control Cfm with success; 
 *                - case SATRC 
 *                     - enable the trace flag; 
 *                     - send the control Cfm with success; 
 *            - case ADISIMM 
 *               - switch(subAction) 
 *               - case SAUSTA 
 *                    - disable the unsolicited status flag; 
 *                    - send the control Cfm with success; 
 *               - case SATRC 
 *                    - disable the trace flag; 
 *                    - send the control Cfm with success; 
 *          - case ASHUTDOWN 
 *     - case STRGUSAP 
 *          - Call LSapCntrl
 *     - case STUDXSAP 
 *          - Call UdxSapCntrl
 *     - default  
 *    - end else  
 * 
 * @param[in] pst     post structure
 * @param[in] cntrl   LM management structure
 * 
 * @return S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwMiLkwCntrlReq
(
Pst       *pst, 
KwMngmt   *cntrl
)
#else
PUBLIC S16 KwMiLkwCntrlReq(pst, cntrl)
Pst       *pst;
KwMngmt   *cntrl;
#endif
{
   Reason   reason;   /* failure reason */
   KwCb     *tKwCb=NULLP;

   TRC3(KwMiLkwCntrlReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check if the instance is configured */
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      cntrl->cfm.status = LCM_PRIM_NOK;
      cntrl->cfm.reason =  LCM_REASON_INVALID_INSTANCE;

      kwLmmSendCfm(tKwCb,pst, cntrl, TCNTRL, &cntrl->hdr);
      RETVALUE(RFAILED);
   }
#endif
   
   tKwCb =  KW_GET_KWCB(pst->dstInst);

   if(!tKwCb)
   {
      cntrl->cfm.status = LCM_PRIM_NOK;
      cntrl->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      kwLmmSendCfm(tKwCb,pst, cntrl, TCNTRL, &cntrl->hdr);
      RETVALUE(RFAILED);
   }

   if (!(tKwCb->init.cfgDone))
   {
      cntrl->cfm.status = LCM_PRIM_NOK;
      cntrl->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      kwLmmSendCfm(tKwCb,pst, cntrl, TCNTRL, &cntrl->hdr);
      RETVALUE(RFAILED);
   }
   RLOG1(L_DEBUG, "KwMiLkwCntrlReq(elmId(%d))", cntrl->hdr.elmId.elmnt);

   /* In normal cases, LCM_REASON_NOT_APPL is returned in cfm.
    * In all error cases appropriate reason is returned
    * by the functions below
    */

   switch (cntrl->hdr.elmId.elmnt)
   {
      case STGEN:
      {
         /* general control */
         reason = kwLmmGenCntrl(tKwCb,cntrl);
         break;
      }
      case STRGUSAP:
      {
         /* Lower SAP control */
         reason = kwLmmLSapCntrl(tKwCb,cntrl);
         break;
      }
      case STUDXSAP:
      {
         /* Udx Sap cntrl */
         reason = kwLmmUdxSapCntrl(tKwCb,cntrl);
         break;
      }
      default:
      {
         reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end switch */
   
   if (reason == LCM_REASON_NOT_APPL)
   {
      cntrl->cfm.status = LCM_PRIM_OK;
      cntrl->cfm.reason = LCM_REASON_NOT_APPL;
   }
   else
   {
      cntrl->cfm.status = LCM_PRIM_NOK;
      cntrl->cfm.reason = reason;
   }

   kwLmmSendCfm(tKwCb,pst, cntrl, TCNTRL, &cntrl->hdr);

   RETVALUE(ROK);
} 

/**
 * @brief 
 *    This function processes solicited status requests received from the layer
 *    manager. The layer manager can request status information regarding the
 *    system ID, RGUSAP,KWUSAP, or CKWSAP.
 *
 * @param[in] pst   post structure
 * @param[in] sta   LM management structure
 * @return S16
 *    -# ROK (SUCCESS)
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwMiLkwStaReq
(
Pst       *pst, 
KwMngmt   *sta
)
#else
PUBLIC S16 KwMiLkwStaReq(pst, sta)
Pst       *pst;
KwMngmt   *sta;
#endif
{
   KwMngmt   rSta;     /* Status */
   Reason    reason;   /* Failure reason */
   KwCb      *tKwCb=NULLP;

   TRC3(KwMiLkwStaReq);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check if the instance is configured */
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      sta->cfm.status = LCM_PRIM_NOK;
      sta->cfm.reason = LCM_REASON_INVALID_INSTANCE;

      kwLmmSendCfm(tKwCb,pst, sta, TSSTA, &sta->hdr);
      RETVALUE(RFAILED);
   }
#endif
   
   tKwCb =  KW_GET_KWCB(pst->dstInst);
   if (!tKwCb)
   {
      sta->cfm.status = LCM_PRIM_NOK;
      sta->cfm.reason = LCM_REASON_GENCFG_NOT_DONE;

      kwLmmSendCfm(tKwCb,pst, sta, TSSTA, &sta->hdr);
      RETVALUE(RFAILED);
   }

   RLOG1(L_DEBUG, "Status request for elmId(%d))", sta->hdr.elmId.elmnt);

   reason = LCM_REASON_NOT_APPL;

   KW_MEM_SET(&rSta, 0, sizeof(KwMngmt));
   switch (sta->hdr.elmId.elmnt)
   {
      case STSID:
      {
         reason = LCM_REASON_NOT_APPL;
         kwGetSId(&rSta.t.ssta.s.sysId);
         break;
      }
      case STKWUSAP:
      {
         KW_MEM_CPY (&rSta.t.ssta.s.kwuSap, 
                     &sta->t.ssta.s.kwuSap, 
                     sizeof (KwKwuSapSta));

         reason = kwLmmGetKwuSapSta (tKwCb,&rSta.t.ssta.s.kwuSap);
         break;
      }
      case STRGUSAP:
      {
         KW_MEM_CPY (&rSta.t.ssta.s.rguSap, 
                     &sta->t.ssta.s.rguSap,
                     sizeof (KwRguSapSta));

         reason = kwLmmGetRguSapSta (tKwCb,&rSta.t.ssta.s.rguSap);
         break;
      }
      case STCKWSAP:
      {
         KW_MEM_CPY (&rSta.t.ssta.s.ckwSap, 
                     &sta->t.ssta.s.ckwSap,
                     sizeof (KwCkwCntSapSta));

         reason = kwLmmGetCkwCntSapSta (tKwCb,&rSta.t.ssta.s.ckwSap);
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
   }
   else
   {
      rSta.cfm.status = LCM_PRIM_NOK;
      rSta.cfm.reason = reason;
   }
   kwLmmSendCfm(tKwCb,pst, &rSta, TSSTA, &sta->hdr);

   RETVALUE(ROK);
} 

/**
 * @brief 
 *    This function processes statistics requests received from the layer
 *    manager.
 *
 * @details  
 *    After collecting the statistics, ir calls the statistics confirm function
 *    to send the statistics to the layer manager.
 *
 *    - switch(sts->hdr.elmId.elmnt) 
 *    - case STGEN 
 *       - get the general statistics from the KW control block; 
 *       - if (action = RESET) 
 *          - reset the general statistic field in the RlCb; 
 *          - call KwMiLkwStsCfm to send statistics back to layer manager; 
 *    - case STCKWSAP 
 *    - case STKWUSAP 
 *    - case STRGUSAP 
 *        - get the SAP specific statistics from KW control block; 
 *        - if (action = RESET) 
 *       - reset the general statistic field in the RlCb; 
 *       - call KwMiLkwStsCfm to send statistics to the layer manager; 
 *  -end switch;
 *
 * @param[in] pst      post structure
 * @param[in] action   action 
 * @param[in] sts      LM management structure
 * 
 * @return S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwMiLkwStsReq 
(
Pst      *pst, 
Action    action, 
KwMngmt   *sts
)
#else
PUBLIC S16 KwMiLkwStsReq (pst, action, sts)
Pst       *pst;
Action    action;
KwMngmt   *sts;
#endif
{
   KwMngmt   rSts;     /* Statistics */
   Reason    reason;   /* Reason for failure */
   KwCb     *tKwCb=NULLP;

   TRC3(KwMiLkwStsReq);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check if the instance is configured */
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      rSts.cfm.status = LCM_PRIM_NOK;
      rSts.cfm.reason = LCM_REASON_INVALID_INSTANCE;

      kwLmmSendCfm(tKwCb,pst, &rSts, TCNTRL, &sts->hdr);
      RETVALUE(RFAILED);
   }
#endif
   
   tKwCb =  KW_GET_KWCB(pst->dstInst);
   if (!tKwCb)
   {
      rSts.cfm.status = LCM_PRIM_NOK;
      rSts.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;

      kwLmmSendCfm(tKwCb,pst, &rSts, TCNTRL, &sts->hdr);
      RETVALUE(RFAILED);
   }

   RLOG2(L_DEBUG, "KwMiLkwStsReq(elmId(%d),action(%d))", 
                       action, 
                       sts->hdr.elmId.elmnt);

   KW_MEM_SET(&rSts, 0, sizeof(KwMngmt));

   switch (sts->hdr.elmId.elmnt)
   {
      case STGEN:
      {
         reason = kwLmmGetGenSts(tKwCb,&rSts.t.sts.s.gen, action);
         break;
      }
      case STKWUSAP:
      case STCKWSAP:
      {
         if( sts->hdr.elmId.elmnt == STKWUSAP)
         {
            /* kw005.201, modified the element of kwuSap from suId to spId */
            rSts.t.sts.s.kwuSap.spId = sts->t.sts.s.kwuSap.spId;
         }
         reason = kwLmmGetSapSts(tKwCb,&rSts, sts->hdr.elmId.elmnt, action);
         break;
      }
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
   }
   else
   {
      rSts.cfm.status = LCM_PRIM_NOK;
      rSts.cfm.reason = reason;
   }
   kwLmmSendCfm(tKwCb,pst, &rSts, TSTS, &sts->hdr);

   RETVALUE(ROK);
} 

/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
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
PUBLIC S16 KwMiLkwL2MeasReq 
(
Pst            *pst, 
KwL2MeasReqEvt *measReqEvt 
)
#else
PUBLIC S16 KwMiLkwL2MeasReq (pst, measReqEvt)
Pst            *pst; 
KwL2MeasReqEvt *measReqEvt;
#endif
{
   S16            ret = ROK;
#if 0 /* L2 MEAS AGHOSH */   
   KwL2MeasEvtCb *measEvtCb = NULLP;
   KwL2MeasCb    *measCb    = NULLP;
#endif   
   U16            cntr;
   U8             measType;
   KwL2MeasCfmEvt measCfmEvt;

#if 0 /* L2 MEAS AGHOSH */
   KwUlUeCb         *ueCb = NULL;
   U16             rbIdx = 0;
   U16             qciIdx = 0;
#endif


   KwCb     *tKwCb;

   TRC3(KwMiLkwL2MeasReq);

   tKwCb =  KW_GET_KWCB(pst->dstInst);

   /* Initialize measCfmEvt */
   KW_MEM_ZERO(&measCfmEvt, sizeof(KwL2MeasCfmEvt));
   /* validate the received measReqEvt */
   /*LTE_L2_MEAS_PHASE2*/
   if(measReqEvt->measPeriod != 0)
   {
      /* L2 MEAS AGHOSH */
      if((measReqEvt->measReq.measType & LKW_L2MEAS_DL_IP)|| 
            (measReqEvt->measReq.measType & LKW_L2MEAS_UL_IP))
      {
         ret = kwUtlValidateIpThL2Meas(measReqEvt, &measCfmEvt);
      }

      if(ret != ROK)
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
#endif /* ERRCLASS & ERRCLS_ADD_RES */
         kwUtlSndUlL2MeasNCfm(tKwCb, measReqEvt, &measCfmEvt);
         KW_FREE(tKwCb, measReqEvt, sizeof(KwL2MeasReqEvt))
         RETVALUE(RFAILED);
      }
   }

   measType = measReqEvt->measReq.measType;

   if((measType != LKW_L2MEAS_DL_IP) &&
      (measType !=  LKW_L2MEAS_ACT_UE) &&
      (measType !=  LKW_L2MEAS_DL_DISC) &&
      (measType != LKW_L2MEAS_DL_DELAY) && 
      (measType != LKW_L2MEAS_UU_LOSS) &&
      (measType !=  LKW_L2MEAS_UL_IP))/* || (qci > LKW_MAX_QCI)*/
   {
      measCfmEvt.transId = measReqEvt->transId;
      measCfmEvt.measType = measType;
      measCfmEvt.status.status = LCM_PRIM_NOK;
      measCfmEvt.status.reason = LKW_CAUSE_INVALID_MEASTYPE;
      kwUtlSndDlL2MeasNCfm(tKwCb, measReqEvt, &measCfmEvt);
      KW_FREE(tKwCb, measReqEvt, sizeof(KwL2MeasReqEvt))
      RETVALUE(ROK);
   }

   /* for UL IP throughput meas enable for all QCIs */
   if(measType & LKW_L2MEAS_UL_IP)
   {
      for(cntr = 0; cntr < LKW_MAX_QCI; cntr++)
      {
         tKwCb->u.ulCb->kwL2Cb.measOn[cntr] |= LKW_L2MEAS_UL_IP;
      }
   }
  
   if(measType & 
      (LKW_L2MEAS_DL_IP | LKW_L2MEAS_DL_DISC | 
      LKW_L2MEAS_DL_DELAY | LKW_L2MEAS_UU_LOSS))
   {
      KwL2MeasReqEvt *measEvt;
      Pst      *udxPst;

      udxPst = &(KW_GET_UDX_SAP(tKwCb)->pst);
      
      KW_ALLOC_SHRABL_BUF(udxPst->region, 
			  udxPst->pool,
			  measEvt, 
			  sizeof(KwL2MeasReqEvt));

#if (ERRCLASS & ERRCLS_ADD_RES) /* KW_FIX */ 
      if(measEvt == NULLP)
      {
         RLOG0(L_FATAL,"Memory Allocation failed");   
         RETVALUE(RFAILED);
      }   
#endif      
      cmMemcpy((Void*)measEvt, (Void*)measReqEvt, sizeof(KwL2MeasReqEvt));
      /*Redirect the request to DL task */
      /* NOTE:As of today, there are no cases where the Req will fail at DL
         as long as it reached the DL, so we don't wait for a confirmation from
         DL to send the confirmation to LM*/
      /* The interface for sending a confirmation back does not exist today;
         it needs to be created when the need arises */
      KwUlUdxL2MeasReq(&(KW_GET_UDX_SAP(tKwCb)->pst),measEvt);
   }

   /* We need to copy the transId for sending back confirms later */
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      KwL2MeasEvtCb* measEvtCb = &(tKwCb->u.ulCb->kwL2Cb.kwL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         measEvtCb->transId= measReqEvt->transId;
      }
   }
   /*KW_FREE(tKwCb, measReqEvt, sizeof(KwL2MeasReqEvt));*/

   RETVALUE(ret);
} /* KwMiLkwL2MeasReq */

/**
@brief 
This function processes L2 Measurement stop request received from the layer manager.
After receving this request, RLC stops L2 Measurement
 *  @param[in] pst      post structure
 *  @param[in] measType meas Type 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

#ifdef ANSI
PUBLIC S16 KwMiLkwL2MeasStopReq
(
Pst            *pst,
U8             measType
)
#else
PUBLIC S16 KwMiLkwL2MeasStopReq (pst, measType)
Pst            *pst;
U8             measType;
#endif
{
   S16 ret = ROK;
   KwL2MeasEvtCb *measEvtCb = NULLP;

   U16            cntr;
   U8             status = ROK;
   KwCb     *tKwCb;

   TRC3(KwMiLkwL2MeasStopReq);

   tKwCb =  KW_GET_KWCB(pst->dstInst);

   /* reset the counter values for the measurement that is stopped */
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(tKwCb->u.ulCb->kwL2Cb.kwL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         kwUtlResetUlL2MeasInKwRb(tKwCb, &measEvtCb->measCb, measType);

      }
   }   

   /* for UL IP throughput meas disable for all QCIs */
   if(measType & LKW_L2MEAS_UL_IP)
   {
      for(cntr = 0; cntr < LKW_MAX_QCI; cntr++)
      {
         tKwCb->u.ulCb->kwL2Cb.measOn[cntr] &= ~LKW_L2MEAS_UL_IP;
      }
   }
   
   if((measType & LKW_L2MEAS_DL_IP) || (measType & LKW_L2MEAS_DL_DISC)
       || (measType & LKW_L2MEAS_DL_DELAY))
   {
      /*Redirect the request to DL task */
      KwUlUdxL2MeasStopReq(&(KW_GET_UDX_SAP(tKwCb)->pst),measType);
      /*RETVALUE(ROK);*/
   }
   /*cmMemset((U8*)&measCfmEvt, 0, sizeof(KwL2MeasCfmEvt)); */

   status = LCM_PRIM_OK; 
   KwMiLkwL2MeasStopCfm(&tKwCb->genCfg.lmPst, measType,status); 
   
   RETVALUE(ret);
}
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
PUBLIC S16 KwMiLkwL2MeasSendReq
(
Pst            *pst,
U8             measType
)
#else
PUBLIC S16 KwMiLkwL2MeasSendReq (pst, measType)
Pst            *pst;
U8             measType;
#endif
{
   /*S16 ret = ROK;*/
   KwL2MeasEvtCb *measEvtCb = NULLP;
   U16            cntr;
   KwCb     *tKwCb;
   TRC3(KwMiLkwUlL2MeasSendReq);

   tKwCb =  KW_GET_KWCB(pst->dstInst);
   
   /* In case of addition of any new measType here ,appropriate handling 
    * has to be done in RLC DL (kwUtlSndDlL2MeasCfm)*/
   if(measType & 
      (LKW_L2MEAS_DL_DISC | LKW_L2MEAS_DL_DELAY 
       | LKW_L2MEAS_UU_LOSS| LKW_L2MEAS_DL_IP))
   {
      /*Redirect the request to DL task */
      KwUlUdxL2MeasSendReq(&(KW_GET_UDX_SAP(tKwCb)->pst),measType);
      /* L2 MEAS AGHOSH */
      /*RETVALUE(ROK);*/
   }

   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(tKwCb->u.ulCb->kwL2Cb.kwL2EvtCb[cntr]);
      /* L2 MEAS AGHOSH */
      if(measEvtCb->measCb.measType & measType)
      {
         kwUtlHdlL2TmrExp(tKwCb, measEvtCb);
      }
   }

   RETVALUE(ROK);
}
#endif /* LTE_L2_MEAS */

/**
 * @brief
 *    This function configures the RLC data sap
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  cfg   RLC LM Sap configuration structure
 *
 * @return  S16
 *    -# LCM_REASON_NOT_APPL  (SUCCESS)
 *    -# LCM_REASON_INVALID_SAP 
 *    -# LCM_REASON_MEM_NOAVAIL 
 *    -# LCM_REASON_RECONFIG_FAIL 
 */
#ifdef ANSI
PRIVATE S16 kwLmmCfgKwuSap
(
KwCb       *gCb,
KwSapCfg   *cfg                
)
#else
PRIVATE S16 kwLmmCfgKwuSap(gCb,cfg)
KwCb       *gCb;
KwSapCfg   *cfg;               
#endif
{
   KwKwuSapCb   *kwuSapCb;
   TRC2(kwLmmCfgKwuSap)

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate the protocol parameters */
   if((cfg->sapId >= (S16)gCb->genCfg.maxKwuSaps) || (cfg->sapId < 0))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   kwuSapCb = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
              (KwKwuSapCb*)(gCb->u.dlCb->kwuDlSap + cfg->sapId):
              (KwKwuSapCb*)(gCb->u.ulCb->kwuUlSap + cfg->sapId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if (kwuSapCb->state != KW_SAP_NOT_CFG)
   {
      RLOG2(L_ERROR,"RLC Mode [%d] : Invalid kwuSap State [%d]",
            gCb->genCfg.rlcMode, kwuSapCb->state);
      /* reconfiguration not allowed */
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* Fill the parameters */
   KW_FILL_SAP_HELPER(kwuSapCb, cfg, gCb);

   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    This function configures the RLC control sap
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  cfg   RLC LM Sap configuration structure
 * 
 * @return  S16
 *    -# LCM_REASON_NOT_APPL (SUCCESS)
 *    -# LCM_REASON_INVALID_SAP
 *    -# LCM_REASON_INVALID_SAP 
 *    -# LCM_REASON_RECONFIG_FAIL 
 */
#ifdef ANSI
PRIVATE S16 kwLmmCfgCkwSap 
(
KwCb       *gCb,
KwSapCfg   *cfg             
)
#else
PRIVATE S16 kwLmmCfgCkwSap(gCb,cfg)
KwCb       *gCb;
KwSapCfg   *cfg;             
#endif
{
   KwCkwSapCb  *ckwSap;
   
   TRC2(kwLmmCfgCkwSap)

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate config parameters */
   if ((cfg->sapId >= KW_MAX_CKWSAPS) || (cfg->sapId < 0) )
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      /* Get Sap control block */
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
#endif

   ckwSap = &(gCb->u.ulCb->ckwSap);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if(ckwSap->state  != KW_SAP_NOT_CFG)
   {
      RLOG1(L_ERROR,"Invalid kwuSap State [%d]",ckwSap->state);
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   
   /* Fill the parameters */
   KW_FILL_SAP_HELPER(ckwSap, cfg, gCb);

   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    This function configures the UDX sap
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  cfg   RLC LM Sap configuration structure
 *
 * @return  S16
 *    -# LCM_REASON_NOT_APPL   (SUCCESS)
 *    -# LCM_REASON_INVALID_SAP
 *    -# LCM_REASON_INVALID_SAP
 *    -# LCM_REASON_RECONFIG_FAIL
 */
#ifdef ANSI
PRIVATE S16 kwLmmCfgUdxSap 
(
KwCb       *gCb,
KwSapCfg   *cfg         
)
#else
PRIVATE S16 kwLmmCfgUdxSap(gCb,cfg)
KwCb       *gCb;
KwSapCfg   *cfg;       
#endif
{
   KwUdxDlSapCb   *udxDlSap;
   KwUdxUlSapCb   *udxUlSap;

   TRC2(kwLmmCfgUdxSap);
   
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate the protocol parameters */
   if((cfg->sapId >= KW_MAX_UDXSAPS) || (cfg->sapId < 0))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
#endif
   /* Get Sap control block */
   if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      udxDlSap = (gCb->u.dlCb->udxDlSap + cfg->sapId);
#if (ERRCLASS & ERRCLS_INT_PAR)
      /* Check for reconfiguration */
      if(udxDlSap->state != KW_SAP_NOT_CFG)
      {
         RLOG1(L_ERROR,"Invalid udxDlSap State [%d]",udxDlSap->state);
         RETVALUE(LCM_REASON_RECONFIG_FAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      /* Fill the parameters */
      KW_FILL_SAP_HELPER(udxDlSap, cfg, gCb);
   }
   else
   {
      udxUlSap = (gCb->u.ulCb->udxUlSap + cfg->sapId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
      if(udxUlSap->state != KW_SAP_NOT_CFG)
      {
         RLOG1(L_ERROR,"Invalid udxUlSap State [%d]", udxUlSap->state);
         RETVALUE(LCM_REASON_RECONFIG_FAIL);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      /* Fill the parameters */
      KW_FILL_SAP_HELPER(udxUlSap, cfg, gCb);

      udxUlSap->bndTmrInt = cfg->bndTmrIntvl;
      udxUlSap->retryCnt = 0;
      cmInitTimers(&(udxUlSap->bndTmr), 1);
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    This function configures the RGU sap
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  cfg   RLC LM Sap configuration structure
 * @return  S16
 *    -# LCM_REASON_NOT_APPL    (SUCCESS)
 *    -# LCM_REASON_INVALID_SAP
 *    -# LCM_REASON_INVALID_SAP
 *    -# LCM_REASON_RECONFIG_FAIL
 */
#ifdef ANSI
PRIVATE S16 kwLmmCfgRguSap 
(
KwCb       *gCb,
KwSapCfg   *cfg               
)
#else
PRIVATE S16 kwLmmCfgRguSap(gCb,cfg)
KwCb       *gCb;
KwSapCfg   *cfg;             
#endif
{
   KwRguSapCb   *rguSap;

   TRC2(kwLmmCfgRguSap);
   
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate the protocol parameters */
   if((cfg->sapId >= KW_MAX_RGUSAPS) || (cfg->sapId < 0))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
#endif

   rguSap = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            &(gCb->u.dlCb->rguDlSap): &(gCb->u.ulCb->rguUlSap);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Check for reconfiguration */
   if(rguSap->state != KW_SAP_NOT_CFG)
   {
      RLOG2(L_ERROR,"RLC Mode [%d]: Invalid rguSap State [%d]",
            gCb->genCfg.rlcMode, rguSap->state);
      RETVALUE(LCM_REASON_RECONFIG_FAIL);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* Fill the parameters */
   KW_FILL_SAP_HELPER(rguSap, cfg, gCb);
   rguSap->suId = cfg->sapId;
   rguSap->bndTmrInt = cfg->bndTmrIntvl;
   rguSap->retryCnt = 0;
   rguSap->state = KW_SAP_CFG;

   cmInitTimers(&(rguSap->bndTmr), 1);

   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Validates the RLC general control parameters 
 *
 * @param[in] cntrl   RLC LM structure 
 * 
 * @return  S16
 *    -# LCM_REASON_NOT_APPL (SUCCESS)
 *    -# LCM_REASON_INVALID_SUBACTION
 *    -# LCM_REASON_INVALID_ACTION
 */
#ifdef ANSI
PRIVATE S16 kwLmmValidateGenCntrl
(
KwMngmt   *cntrl                 
)
#else
PRIVATE S16 kwLmmValidateGenCntrl(cntrl)
KwMngmt   *cntrl;                
#endif
{
   S16   reason;    /* reason for failure */
   U8    sAction;   /* subaction field */

   TRC2(kwLmmValidateGenCntrl);

   reason = LCM_REASON_NOT_APPL;
   sAction = cntrl->t.cntrl.subAction;

   switch (cntrl->t.cntrl.action)
   {
      case AENA:
      case ADISIMM:
         if ((sAction != SAUSTA) && 
             (sAction != SADBG) && 
             (sAction != SATRC))
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
} 

/**
 * @brief
 *    it deregisters the timers and deregisters the kwuSap Control blocks 
 *
 * @param[in]  gCb   RLC Instance Control Block
 * 
 * @return  Void
 */
#ifdef ANSI
PRIVATE Void kwLmmCleanGblRsrcs
(
KwCb   *gCb
)
#else
PRIVATE Void kwLmmCleanGblRsrcs(gCb)
KwCb   *gCb;
#endif
{
   Size   kwSapSize;
   Size   kwUdxSapSize;

   TRC2(kwLmmCleanGblRsrcs)   
 
   if (gCb->init.cfgDone)
   {
      /* Deregister the timers */
/*Pradeep: changing the SDeregTmrMt() to SDeregTmr*/
      (Void) SDeregTmrMt(gCb->init.ent, gCb->init.inst,
                        (S16)gCb->genCfg.timeRes, kwActvTmr);
      /*(Void) SDeregTmr(gCb->init.ent, gCb->init.inst,
                        (S16)gCb->genCfg.timeRes, kwActvTmr);*/

      kwSapSize = (Size)(gCb->genCfg.maxKwuSaps * sizeof(KwKwuSapCb));

      if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
      {
         kwUdxSapSize = (Size)(gCb->genCfg.maxUdxSaps * sizeof(KwUdxDlSapCb));
         if (gCb->u.dlCb)
         {
            gCb->u.dlCb->shutdownReceived = TRUE;
            kwUtlFreeDlMemory(gCb); 
            if (gCb->u.dlCb->kwuDlSap != NULLP)
            {
               KW_FREE(gCb,gCb->u.dlCb->kwuDlSap, kwSapSize);
            }
            
            if(gCb->u.dlCb->udxDlSap != NULLP)
            {
               KW_FREE(gCb,gCb->u.dlCb->udxDlSap, kwUdxSapSize);
            }
            if (gCb->u.dlCb->shutdownReceived) 
            {
               if (gCb->u.dlCb->selfPstMBuf != NULL)
               {
                  SPutMsg(gCb->u.dlCb->selfPstMBuf);
               }
               KW_FREE(gCb,gCb->u.dlCb, sizeof (KwDlCb));
            }
         }
         
      }
      else
      {
         kwUdxSapSize = (Size)(gCb->genCfg.maxUdxSaps * sizeof(KwUdxUlSapCb));
         if (gCb->u.ulCb)
         {
            if (gCb->u.ulCb->kwuUlSap != NULLP)
            {
               KW_FREE(gCb,gCb->u.ulCb->kwuUlSap, kwSapSize);
            }
            
            if(gCb->u.ulCb->udxUlSap != NULLP)
            {
               KW_FREE(gCb,gCb->u.ulCb->udxUlSap, kwUdxSapSize);
            }
         
            KW_FREE(gCb,gCb->u.ulCb, sizeof (KwUlCb));
         }
      }

      gCb->init.cfgDone = FALSE;
      gCb->init.acnt = FALSE;
      gCb->init.trc = FALSE;

      gCb->init.usta = FALSE;
   }

   RETVOID;
}

/**
 * @brief
 *    Delete all SAPs and Control Blocks
 *
 * @details
 *    Shutdown of RLC happens at modules level, tm, um and am modules will be
 *    shutdown using utility functin and dbm shutdown will clean up the SAPs,
 *    control blocks and lists. 
 *
 * @params[in] gCb   RLC instance control block   
 *
 * @return  S16
 *    -# LCM_REASON_NOT_APPL (SUCCESS)
 *    -# LCM_REASON_HASHING_FAILED 
 */
#ifdef ANSI
PRIVATE S16 kwLmmShutdown
(
KwCb   *gCb
)
#else
PRIVATE S16 kwLmmShutdown(gCb)
KwCb   *gCb;
#endif
{
   KwRguSapCb   *rguSap;

   TRC2(kwLmmShutdown)

   if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      rguSap = &(gCb->u.dlCb->rguDlSap); 
   }
   else
   {
      rguSap = &(gCb->u.ulCb->rguUlSap); 
   }
   
   KwLiRguUbndReq(&(rguSap->pst), rguSap->spId, 0);

   if(gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      kwDbmDlShutdown(gCb); 
   }
   else
   {
      kwDbmUlShutdown(gCb);
   }

   kwLmmCleanGblRsrcs(gCb);

   KW_MEM_SET (&(gCb->genSts), 0, sizeof (KwGenSts));

   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Function processes the general control request 
 *
 * @param[in] gCb     RLC instance control block
 * @param[in] cntrl   RLC LM structure
 *
 * @return  S16
 *    -# LCM_REASON_NOT_APPL (SUCCESS)
 *    -# LCM_REASON_INVALID_SUBACTION
 *    -# LCM_REASON_INVALID_ACTION
 */
#ifdef ANSI
PRIVATE S16 kwLmmGenCntrl
(
KwCb *gCb,
KwMngmt *cntrl       
)
#else
PRIVATE S16 kwLmmGenCntrl(gCb,cntrl)
KwCb    *gCb;
KwMngmt *cntrl;     
#endif
{
   KwTrcCntrl *trcCntrl; /* trace */
#ifdef DEBUGP
   KwDbgCntrl *dbgCntrl; /* debug */
#endif /* DEBUGP */
   S16   reason;         /* reason for failure */
   
   TRC2(kwLmmGenCntrl);

   /* Validate control parameters */
   reason = kwLmmValidateGenCntrl (cntrl);

   if (reason != LCM_REASON_NOT_APPL)
   {
      RETVALUE(reason);
   }

   switch(cntrl->t.cntrl.action)
   {
      case AENA:
      {
         switch(cntrl->t.cntrl.subAction)
         {
            case SAUSTA:
            {
               KW_SET_USTA_FLAG(gCb, TRUE);
               break;
            }
            case SADBG:
            {
#ifdef DEBUGP
               dbgCntrl = &cntrl->t.cntrl.s.dbgCntrl;
               KW_GET_DBG_MASK(gCb) |= dbgCntrl->dbgMask;
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
         switch(cntrl->t.cntrl.subAction)
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
               KW_GET_DBG_MASK(gCb) &= ~(dbgCntrl->dbgMask);
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
         reason = kwLmmShutdown(gCb);
         break;
      }
   }
   RETVALUE(reason);
} 

/**
 * @brief
 *    Bind/Unbind RLC UDX sap 
 *
 * @param[in]  gCb     RLC Instance Control Block
 * @param[in]  cntrl   RLC LM structure
 * 
 * @return  S16
 *    -# LCM_REASON_NOT_/PPL (SUCCESS)    
 *    -# LCM_REASON_INVALID_SAP 
 *    -# LCM_REASON_INVALID_STATE
 *    -# LCM_REASON_INVALID_ACTION 
 */
#ifdef ANSI
PRIVATE S16 kwLmmUdxSapCntrl
(
KwCb      *gCb,
KwMngmt   *cntrl               
)
#else
PRIVATE S16 kwLmmUdxSapCntrl(gCb,cntrl)
KwCb      *gCb;
KwMngmt   *cntrl;               
#endif
{
   TRC2(kwLmmUdxSapCntrl)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)
   {
      RETVALUE(LCM_REASON_INVALID_SAP);
   }
   /* validate SuId */
   if((cntrl->t.cntrl.s.sapCntrl.suId < 0) || 
      (cntrl->t.cntrl.s.sapCntrl.suId >= KW_MAX_UDXSAPS))
   {
      RETVALUE(LCM_REASON_INVALID_SAP);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

#define UDX_SAP  gCb->u.ulCb->udxUlSap[cntrl->t.cntrl.s.sapCntrl.suId]
#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate SAP */
   if(UDX_SAP.state == KW_SAP_NOT_CFG)
   {
      RLOG0(L_ERROR,"udxUlSap not configured yet");
      RETVALUE(LCM_REASON_INVALID_STATE);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   switch(cntrl->t.cntrl.action)
   {
      case ABND:
      {
         if(UDX_SAP.state != KW_SAP_BND)
         {
            /* start timer to wait for bind confirm */
            kwStartTmr(gCb,(PTR)(&UDX_SAP), KW_EVT_WAIT_BNDCFM);
            UDX_SAP.state = KW_SAP_BINDING;
            KwUlUdxBndReq(&(UDX_SAP.pst), UDX_SAP.suId, UDX_SAP.spId);
         }
         else
         {
            /* control request received for an already bound SAP */
            RETVALUE(LCM_REASON_INVALID_STATE);
         }
      }
      break;
      case AUBND:
      {
         /* make the state of UDXSAP configured but not bound */
         UDX_SAP.state = KW_SAP_CFG;
         KwUlUdxUbndReq(&(UDX_SAP.pst), UDX_SAP.spId, 0);
      }
      break;
      default:
         RETVALUE(LCM_REASON_INVALID_ACTION);
         break;
   } /* end of switch */
#undef UDX_SAP
   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Bind/Unbind RLC lower sap 
 *
 * @param[in]  gCb     RLC Instance Control Block
 * @param[in]  cntrl   RLC LM structure
 * @return  S16
 *    -# LCM_REASON_NOT_APPL       (SUCCESS)
 *    -# LCM_REASON_INVALID_SAP 
 *    -# LCM_REASON_INVALID_STATE
 *    -# LCM_REASON_INVALID_ACTION 
 */
#ifdef ANSI
PRIVATE S16 kwLmmLSapCntrl
(
KwCb      *gCb,
KwMngmt   *cntrl 
)
#else
PRIVATE S16 kwLmmLSapCntrl(gCb,cntrl)
KwCb      *gCb;
KwMngmt   *cntrl; 
#endif
{
   KwRguSapCb   *rguSap;   /* rgu sap pointer */

   TRC2(kwLmmLSapCntrl)

   rguSap = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            &(gCb->u.dlCb->rguDlSap): &(gCb->u.ulCb->rguUlSap);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* validate SuId */
   if((cntrl->t.cntrl.s.sapCntrl.suId < 0) || 
      (cntrl->t.cntrl.s.sapCntrl.suId >= KW_MAX_RGUSAPS))
   {
      RETVALUE(LCM_REASON_INVALID_SAP);
   }

   /* validate SAP */
   if(rguSap->state == KW_SAP_NOT_CFG)
   {
      RLOG1(L_ERROR,"RLC Mode [%d]:rguSap not configured yet", gCb->genCfg.rlcMode);
      RETVALUE(LCM_REASON_INVALID_STATE);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   switch(cntrl->t.cntrl.action)
   {
      case ABND:
      {
         if(rguSap->state != KW_SAP_BND)
         {
            /* start timer to wait for bind confirm */
            kwStartTmr(gCb,(PTR)(rguSap), KW_EVT_WAIT_BNDCFM);
            rguSap->state = KW_SAP_BINDING;
            KwLiRguBndReq(&(rguSap->pst), rguSap->suId, rguSap->spId);
         }
         else
         {
            /* control request received for an already bound SAP */
            RETVALUE(LCM_REASON_INVALID_STATE);
         }
      }
      break;
      case AUBND:
      {
         /* make the state of RGUSAP is configure but not bound */
         rguSap->state = KW_SAP_CFG;
         KwLiRguUbndReq(&(rguSap->pst), rguSap->spId, 0);
      }
      break;
      default:
         RETVALUE(LCM_REASON_INVALID_ACTION);
         break;
   } /* end of switch */

   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Function gather the general KWU SAP status 
 *
 * @param[in] gCb   RLC Instance Control Block
 * @param[in] sta   LM KWU Sap Status structure 
 * 
 * @return  S16
 *    -# LCM_REASON_NOT_APPL       (SUCCESS)
 *    -# LCM_REASON_INVALID_PAR_VAL(FAILURE)
 */
#ifdef ANSI
PRIVATE S16 kwLmmGetKwuSapSta 
(
KwCb          *gCb,
KwKwuSapSta   *sta         
)
#else
PRIVATE S16 kwLmmGetKwuSapSta(gCb,sta)
KwCb          *gCb;
KwKwuSapSta   *sta;         
#endif
{
   KwKwuSapCb   *kwSapCb;
   
   TRC2(kwLmmGetKwuSapSta);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate the protocol parameters */
   if ((sta->spId >= (S16)gCb->genCfg.maxKwuSaps)||
       (sta->spId < 0))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
#endif

   /* Get Sap control block */
   kwSapCb = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)? 
             (KwKwuSapCb*)(gCb->u.dlCb->kwuDlSap + sta->spId):
             (KwKwuSapCb*)(gCb->u.ulCb->kwuUlSap + sta->spId);

   sta->state = kwSapCb->state;

   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Function gather the general RGU SAP status
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  sta   LM RGU Sap Status structure
 * @return  S16
 *    -# LCM_REASON_NOT_APPL       (SUCCESS) 
 *    -# LCM_REASON_INVALID_PAR_VAL(FAILURE) 
 */
#ifdef ANSI
PRIVATE S16 kwLmmGetRguSapSta 
(
KwCb          *gCb,
KwRguSapSta   *sta         
)
#else
PRIVATE S16 kwLmmGetRguSapSta(gCb,sta)
KwCb          *gCb;
KwRguSapSta   *sta;       
#endif
{
   TRC2(kwLmmGetRguSapSta);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate the protocol parameters */
   if((sta->suId >= KW_MAX_RGUSAPS) || (sta->suId < 0))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
#endif

   sta->state = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ? 
                 gCb->u.dlCb->rguDlSap.state : 
                 gCb->u.ulCb->rguUlSap.state;

   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Function gather the general CKW SAP status
 *
 * @param[in]  gCb   RLC Instance Control Block
 * @param[in]  sta   LM CKW Sap Status structure
 * @return  S16
 *    -# LCM_REASON_NOT_APPL       (SUCCESS)
 *    -# LCM_REASON_INVALID_PAR_VAL(FAILURE)
 */
#ifdef ANSI
PRIVATE S16 kwLmmGetCkwCntSapSta
(
KwCb             *gCb,
KwCkwCntSapSta   *sta     
)
#else
PRIVATE S16 kwLmmGetCkwCntSapSta(gCb,sta)
kwCb             *gCb,
KwCkwCntSapSta   *sta;   
#endif
{
   TRC2(kwLmmGetCkwCntSapSta);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate config parameters */
   if ((sta->spId >= KW_MAX_CKWSAPS ) ||
       (sta->spId < 0 ) ||
       (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
#endif
   sta->state =  gCb->u.ulCb->ckwSap.state;

   RETVALUE(LCM_REASON_NOT_APPL);

} 

/**
 * @brief
 *    Gather the general statistics 
 *
 * @param[in]  gCb      RLC Instance Control Block
 * @param[in]  sts      LM general statistics structure
 * @param[in]  action   action
 * @return  S16
 *    -# LCM_REASON_NOT_APPL       (SUCCESS)
 *    -# LCM_REASON_INVALID_PAR_VAL(FAILURE)
 */
#ifdef ANSI
PRIVATE S16 kwLmmGetGenSts
(
KwCb       *gCb,
KwGenSts   *sts,       
Action     action         
)
#else
PRIVATE S16 kwLmmGetGenSts(gCb,sts, action)
KwCb       *gCb;
KwGenSts   *sts;          
Action     action;         
#endif
{
   TRC2(kwLmmGetGenSts);

#if (ERRCLASS & ERRCLS_INT_PAR)
   /* Validate protocol parameters */
   if ((action != LKW_ZEROSTS) && (action != LKW_NOZEROSTS))
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
#endif

   *sts = gCb->genSts;
   
   if(action == LKW_ZEROSTS)
   {
      KW_MEM_SET (&(gCb->genSts), 0, sizeof (KwGenSts));
   }

   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    Gather the SAP statistics
 *
 * @param[in]  gCb      RLC Instance Control Block
 * @param[in]  sts      LM general statistics structure
 * @param[in]  elmnt    element 
 * @param[in]  action   action 
 * 
 * @return  S16
 *    -# LCM_REASON_NOT_APPL (SUCCESS)
 *    -# LCM_REASON_INVALID_PAR_VAL (FAIL)
 */
#ifdef ANSI
PRIVATE S16 kwLmmGetSapSts
(
KwCb      *gCb,
KwMngmt   *sts,               
Elmnt     elmnt,               
Action    action             
)
#else
PRIVATE S16 kwLmmGetSapSts(sts, elmnt, action)
KwCb      *gCb;
KwMngmt   *sts;            
Elmnt     elmnt;            
Action    action;         
#endif
{
   TRC2(kwLmmGetSapSts);

   /* Validate protocol parameters */
   if (action != LKW_ZEROSTS && action != LKW_NOZEROSTS)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   switch(elmnt)
   {
      case STKWUSAP:
      {
         KwKwuSapSts  *kwuSap; 
         KwKwuSapCb   *kwuSapCb;    
         kwuSap = &sts->t.sts.s.kwuSap;

         /* Get Sap control block */
         /* kw005.201, modified the element of kwuSap from suId to spId */
         kwuSapCb = (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL)? 
              (KwKwuSapCb*)(gCb->u.dlCb->kwuDlSap + kwuSap->spId):
              (KwKwuSapCb*)(gCb->u.ulCb->kwuUlSap + kwuSap->spId);


         *kwuSap = kwuSapCb->sts;

         if (action == LKW_ZEROSTS)
         {
            KW_MEM_SET (&kwuSapCb->sts, 0, sizeof (KwKwuSapSts));
         }
         break;
      }
      /* kw005.201 removed RGUSAP statistics support */
      case STCKWSAP:
      {
         KwCkwCntSts  *ckwSap;  
         ckwSap = &sts->t.sts.s.ckwSap;
         if (gCb->genCfg.rlcMode == LKW_RLC_MODE_DL) 
         {
            RETVALUE(LCM_REASON_INVALID_PAR_VAL);
         }

         ckwSap->statMsgs = gCb->u.ulCb->ckwSap.sts.statMsgs;

         if (action == LKW_ZEROSTS)
         {
            KW_MEM_SET (&(gCb->u.ulCb->ckwSap.sts), 0, sizeof (KwCkwCntSts));
         }
         break;
      }
      default:
         RETVALUE(LCM_REASON_INVALID_ELMNT);
   }

   SGetDateTime(&sts->t.sts.dt);

   RETVALUE(LCM_REASON_NOT_APPL);
} 

/**
 * @brief
 *    This function sends Unsolicited Status Indication to the Layer Management
 *    Entity. 
 *
 * @param[in]  gCb        RLC Instance Control Block
 * @param[in]  category   Category 
 * @param[in]  event      event
 * @param[in]  cause      cause
 * @param[in]  UeId       ueId 
 * @param[in]  Qci        qci 
 * 
 * @return  Void
 */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC Void kwLmmSendAlarm
(
KwCb   *gCb,
U16    category,  
U16    event,    
U16    cause,   
SuId   suId,   
U32    ueId,  
U8     qci   
)
#else
PUBLIC Void kwLmmSendAlarm(category, event, cause, suId, ueId, qci)
KwCb   *gCb;
U16    category; 
U16    event;   
U16    cause;  
SuId   suId;  
U32    ueId; 
U8     qci;    
#endif
#else /* LTE_L2_MEAS */
#ifdef ANSI
PUBLIC Void kwLmmSendAlarm
(
KwCb   *gCb,
U16    category, 
U16    event,   
U16    cause,  
SuId   suId,  
U32    ueId  
)
#else
PUBLIC Void kwLmmSendAlarm(category, event, cause, suId, ueId)
KwCb   *gCb;
U16    category; 
U16    event;   
U16    cause;  
SuId   suId;  
U32    ueId; 
#endif
#endif /* LTE_L2_MEAS */
{
   KwMngmt   usta;   /* Rlc Management Structure */

   TRC2(kwLmmSendAlarm);

   if(gCb->init.usta == FALSE)
   {
      RETVOID;
   }

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
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   if(event == LKW_EVT_MEAS_HALT)
   {
      usta.t.usta.qci = qci;
   }
#endif /* LTE_L2_MEAS */
   /* update the date and time */
   (Void) SGetDateTime(&usta.t.usta.alarm.dt);

   KwMiLkwStaInd(&(gCb->init.lmPst), &usta);

   RETVOID;

} 

/**
 * @brief
 *    This function sends trace indication to LM 
 *
 * @param[in]  gCb     RLC Instance Control Block
 * @param[in]  event   event
 * @param[in]  mBuf    meessage buffer
 *
 * @return S16
 *    -# ROK 
 *    -# RFAILED 
 */
#ifdef ANSI
PUBLIC S16 kwLmmSendTrc
(
KwCb     *gCb,
Event    event,     
Buffer   *mBuf     
)
#else
PUBLIC S16 kwLmmSendTrc(gCb,event, mBuf)
KwCb     *gCb;
Event    event;   
Buffer   *mBuf;  
#endif
{
   KwMngmt   trc;              /* RLC management control block */
   Buffer    *dstMbuf;   
   MsgLen    bufLen;
   Data      *tempBuf;
   MsgLen    tempCnt;
   Pst       pst;

   TRC2(kwLmmSendTrc)

   RLOG2(L_DEBUG, "kwLmmSendTrc(): Trace for event=%d, gCb->trcLen=%d",
                     event,
                     gCb->trcLen);

   cmMemset((U8 *)&trc, 0, sizeof(KwMngmt));

   pst = gCb->init.lmPst;

   trc.t.trc.event = event;
   SGetDateTime(&trc.t.trc.dt);
   trc.cfm.status = LCM_PRIM_OK;
   trc.cfm.reason = LCM_REASON_NOT_APPL;
         
   if(mBuf != NULLP)
   {
     /* Check if the whole buffer is to be sent in Trace indication */
     if(gCb->trcLen == LKW_FULL_TRACE)
     {
        if (SCpyMsgMsg(mBuf, pst.region, pst.pool, &dstMbuf)
            != ROK)
        {
           /* rg005.201 removed SPutSBuf on error */
           RLOG0(L_ERROR,"SCpyMsgMsg Failed");   
           RETVALUE(RFAILED);
        }
        /* Send Trace Indication to Layer manager */
        KwMiLkwTrcInd(&pst, &trc, dstMbuf);
     }
     /* check if only a specified number of bytes are to be sent */
     else if(gCb->trcLen > 0)
     {
        /* Get the length of the recvd message buffer */
        if (SFndLenMsg(mBuf, &bufLen) != ROK)
        {
           RLOG0(L_ERROR,"SFndLenMsg Failed");   
           RETVALUE(RFAILED);
        }
        /* Check if the recvd buffer size is less than request trace len */
        if(bufLen < gCb->trcLen)
        {
           /* Copy the whole of the recvd buffer in trace indication */
      
           if (SCpyMsgMsg(mBuf, pst.region, pst.pool, &dstMbuf)
              != ROK)
           {
              RLOG0(L_ERROR,"SCpyMsgMsg Failed");   
              RETVALUE(RFAILED);
           }
         
           /* Send Trace Indication to Layer manager */
           KwMiLkwTrcInd(&pst, &trc, dstMbuf);
        }
        else
        {
           /* if the recvd buffer size is greater than request trace len */
           /* Get a temporary buffer to store the msg */
           KW_ALLOC(gCb,tempBuf, gCb->trcLen);

#if (ERRCLASS & ERRCLS_INT_PAR)
           if(tempBuf == NULLP)
           {
              (Void) SPutSMem(KW_GET_MEM_REGION(gCb), KW_GET_MEM_POOL(gCb));
      
               RLOG0(L_FATAL,"Memory Allocation failed");   
               RETVALUE(LCM_REASON_MEM_NOAVAIL);
           }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         
           /* Copy trcLen nos of bytes from the recvd message */
           if (SCpyMsgFix(mBuf,0,gCb->trcLen,tempBuf,&tempCnt) != ROK)   
           {
              RLOG0(L_ERROR,"SCpyMsgFix Failed");   
              RETVALUE(RFAILED);
           }

           if (SGetMsg(pst.region, pst.pool, &dstMbuf) != ROK)
           {
              RLOG0(L_FATAL,"Memory Allocation failed");   
              RETVALUE(RFAILED);
           }
           /* Copy the tempBuf data to dst mBuf */
           if (SCpyFixMsg(tempBuf,dstMbuf,0,gCb->trcLen,&tempCnt) != ROK)
           {
              RLOG0(L_ERROR,"SCpyMsgFix Failed");   
              RETVALUE(RFAILED);
           }
  
           /* Free the memory allocated for tempBuf */
           KW_FREE(gCb,tempBuf, gCb->trcLen);
           /* Send Trace Indication to Layer manager */
           KwMiLkwTrcInd(&pst, &trc, dstMbuf);
        }
     }
   }
   else
   {
      KwMiLkwTrcInd(&pst, &trc, mBuf);
   }

   RETVALUE(ROK);
} 

  
/*
 * @brief
 *    Activate Task - timer
 *
 * @details     
 *    Invoked by system services to activate a task with a timer tick.
 *
 * @param[in] ent    entity
 * @param[in] inst   instance
 *
 * @return S16
 *    -# ROK
 *    -# RFAILED
 *
 */
 
#ifdef ANSI
PUBLIC S16 kwActvTmr
(
Ent    ent,
Inst   inst
)
#else
PUBLIC S16 kwActvTmr(ent,inst)
Ent    ent;
Inst   inst;
#endif
{
   KwCb   *gCb; 
   TRC2(kwActvTmr)

   if (inst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
   gCb = KW_GET_KWCB(inst); 
   cmPrcTmr(&(gCb->kwTqCp), gCb->kwTq, (PFV) kwTmrExpiry);

   RETVALUE(ROK);

} /* end of kwActvTmr */

 
/********************************************************************30**

         End of file:     gp_lmm.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:01 2015

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
/main/1      ---       gk   1. initial release.
*********************************************************************91*/
