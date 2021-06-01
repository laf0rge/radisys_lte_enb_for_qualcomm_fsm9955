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
  
        Name:    LTE-RLC Layer - Upper Interface
    
        Type:    C file
  
        Desc:    C source code for the upper interface of LTE-RLC
 
        File:    pj_ptui.c

        Sid:      eq_udx_ul.c@@/main/TeNB_Main_BR/10 - Mon Aug 11 16:42:57 2014
  
        Prg:     gk
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=226;
/** @file gp_ptui.c
@brief RLC Upper Interface
*/

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000371     LTE-RLC
 */


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
#include "cpj.h"           /* UDX defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"           /* LPJ defines */

#include "pj_err.h"
#include "pj_env.h"        /* RLC environment options */
#include "pj.h"
#include "pj_ul.h"
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
#include "cpj.x"           /* UDX defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#include "pj.x"
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_ul.x"

PRIVATE S16 pjHdlCpjUlCfgReq ARGS((PjCb  *gCb,PjCfgInfo *cfgInfo,
              UdxCfgCfmInfo *cfmInfo, CpjCfgCfmInfo *cfgCfm, Bool *sndCfgCfm));



/**
  * @brief
  * UDX APIs
  */
/**
 *
 * @brief 
 *
 *        Handler for bind confirmation from DL.
 *
 * @b Description:
 *
 *        This function handles the bind confirmation received
 *        from MAC. 
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service User ID
 */


  
/****************************************************************************
 *                         UDX Interface Mt functions
 ***************************************************************************/
#ifdef ANSI
PUBLIC S16 PjUlUdxBndCfm
(
Pst * pst,
SuId suId,
U8 status
)
#else
PUBLIC S16 PjUlUdxBndCfm(pst, suId, status)
Pst * pst;
SuId suId;
U8 status;
#endif
{
   U16             event;              /* Event */
   U16             cause;              /* Cause */
   PjUdxUlSapCb      *udxSap;            /* RGU SAP Control Block */
   PjCb            *tPjCb;

   TRC3(PjUlUdxBndCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef DEBUGP
   RLOG2(L_DEBUG, "PjUlUdxBndCfm(post, suId(%d), status(%d)", suId, status);
#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      /* INNC: ERROR CLASS */
      RLOG0(L_FATAL, "General configuration not done");
      PJ_SEND_SAPID_ALARM(tPjCb,suId, 
                          LPJ_EVENT_LI_BND_CFM, LCM_CAUSE_INV_STATE);

      RETVALUE(RFAILED);
   }

   if (suId < 0)
   {
      /* INNC: ERROR CLASS */
      RLOG0(L_ERROR, "Invalid suId");
      PJ_SEND_SAPID_ALARM(tPjCb,suId, 
                           LPJ_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SUID);

      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   udxSap = tPjCb->u.ulCb->udxUlSap + suId;

#ifdef DEBUGP
   RLOG1(L_DEBUG, "PjLiRguBndCfm: For RGU SAP state=%d",
       udxSap->state);
#endif

   /* Check rguSap state */
   switch (udxSap->state)
   {
      case PJ_SAP_BINDING:
      {
         pjStopTmr (tPjCb,(PTR)udxSap, PJ_EVT_WAIT_UDX_BNDCFM);

         udxSap->retryCnt = 0;
          
         if (status == CM_BND_OK)
         {
            udxSap->state = PJ_SAP_BND;
            event = LCM_EVENT_BND_OK;
            cause = LPJ_CAUSE_SAP_BNDENB;
         }
         else
         {
            udxSap->state = PJ_SAP_CFG;
            event = LCM_EVENT_BND_FAIL;
            cause = LPJ_CAUSE_UNKNOWN;
         }
      }
      break;

     default:
        event = LPJ_EVENT_RGU_BND_CFM;
        cause = LCM_CAUSE_INV_STATE;
        break;
   }

   /* Send an alarm with proper event and cause */
   /*PJ_SEND_SAPID_ALARM(tPjCb,suId, event, cause);*/
#ifdef LTE_L2_MEAS
   pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, event, cause, suId, 0,0); 
#else
   pjLmmSendAlarm(tPjCb,LCM_CATEGORY_INTERFACE, event, cause, suId, 0); 
#endif

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUlUdxCfgCfm
(
Pst * pst,
SuId suId,
UdxCfgCfmInfo * cfmInfo
)
#else
PUBLIC S16 PjUlUdxCfgCfm(pst, suId, cfgCfm)
Pst * pst;
SuId suId;
UdxCfgCfmInfo * cfmInfo;
#endif
{
   S16              ret;
   PjUdxUlSapCb   *udxSap;            /* RGU SAP Control Block */
   PjCpjSapCb     *cpjSap;
   PjCb            *tPjCb;
   CpjCfgCfmInfo   *cfgCfm;
   Bool            sndCfgCfm;
   PjCfgInfo       *cfgInfo;

   TRC3(PjUlUdxCfgCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef DEBUGP
   RLOG1(L_DEBUG, "PjUlUdxBndCfm(post, suId(%d), )", suId);
#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      /* INNC: ERROR CLASS */
      RLOG0(L_ERROR, "Invalid suId");
      PJ_SEND_SAPID_ALARM(tPjCb,suId, 
                           LPJ_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SUID);

      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   udxSap = tPjCb->u.ulCb->udxUlSap + suId;

   ret = pjDbmFindUlTransIdLst(tPjCb,cfmInfo->transId, &cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Invalid transId");
      PJ_PST_FREE(pst->region, pst->pool,cfmInfo, sizeof (UdxCfgCfmInfo));
      RETVALUE (RFAILED);
   }
   /* Allocate memory and memset to 0 for cfmInfo */
   cpjSap = &(tPjCb->u.ulCb->cpjSap);
   if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,(Data **)&cfgCfm, 
      sizeof (CpjCfgCfmInfo)) != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      /* pj002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, cfmInfo, sizeof(UdxCfgCfmInfo));
      PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgInfo->cfgInfo, sizeof(CpjCfgReqInfo));
      PJ_FREE(tPjCb,cfgInfo,sizeof(PjCfgInfo));
      RETVALUE(RFAILED);
   }

   sndCfgCfm = TRUE;
   ret = pjHdlCpjUlCfgReq(tPjCb,cfgInfo, cfmInfo, cfgCfm, &sndCfgCfm);
/* AGHOSH */
   /* Free the cfg confirm allocated from DL, this should actually
      be a sharable buffer and not a static buffer */
   PJ_PST_FREE(pst->region, pst->pool, cfmInfo, sizeof(UdxCfgCfmInfo));
   PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgInfo->cfgInfo, sizeof(CpjCfgReqInfo));
   PJ_FREE(tPjCb,cfgInfo,sizeof(PjCfgInfo));
/* AGHOSH */
   
   if (sndCfgCfm)
   {
       cpjSap = &(tPjCb->u.ulCb->cpjSap);

       PjUiCpjCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
   }

   
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUlUdxReEstCfm
(
Pst               *pst,
SuId              suId,
UdxReEstCfmInfo   *reEstCfm
)
#else
PUBLIC S16 PjUlUdxReEstCfm(pst, suId, reEstCfm)
Pst               *pst;
SuId              suId;
UdxReEstCfmInfo   *reEstCfm;
#endif
{
   S16              ret;
   PjUdxUlSapCb    *udxSap;            /* RGU SAP Control Block */
   PjCpjSapCb      *cpjSap;
   PjCb            *tPjCb;
   PjCfgInfo       *cfgInfo;
   CpjReEstCfmInfo *cpjReEstCfm;
    
   TRC3(PjUlUdxReEstCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      PJ_PST_FREE(pst->region, pst->pool,reEstCfm, sizeof (UdxReEstCfmInfo));

      RETVALUE (RFAILED);
   }
#endif
   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef DEBUGP
   RLOG1(L_DEBUG, "PjUlUdxReEstCfm(post, suId(%d), ", suId);
#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      /* INNC: ERROR CLASS */
      RLOG0(L_ERROR, "Invalid suId");
      PJ_PST_FREE(pst->region, pst->pool,reEstCfm, sizeof (UdxReEstCfmInfo));
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   udxSap = tPjCb->u.ulCb->udxUlSap + suId;

   ret = pjDbmFindUlTransIdLst(tPjCb, reEstCfm->transId, &cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Invalid transId");
      PJ_PST_FREE(pst->region, pst->pool,reEstCfm, sizeof (UdxReEstCfmInfo));
      RETVALUE (RFAILED);
   }

   ret = ROK;

   cpjSap = &(tPjCb->u.ulCb->cpjSap);
   if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,(Data **)&cpjReEstCfm, 
      sizeof (CpjReEstCfmInfo)) != ROK)
   {
      /* Free memory for CfgReqInfo */
      /* kw002.201 Freeing from proper region */  
      PJ_PST_FREE(pst->region, pst->pool,reEstCfm, sizeof (UdxReEstCfmInfo));
      PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgInfo->reEstReq, sizeof(CpjReEstReqInfo));
      PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   cpjReEstCfm->ueId = cfgInfo->reEstReq->pdcpId.ueId;
   cpjReEstCfm->cellId = cfgInfo->reEstReq->pdcpId.cellId;
   /* Assign number of entity configuraitons and suId */
   cpjReEstCfm->transId = cfgInfo->uprLyrTransId;
   cpjReEstCfm->status = reEstCfm->status;
   
   if (CPJ_CFG_CFM_OK == reEstCfm->status)
   {
      /* handle ReEstablish here */
      pjCfgUlReEstReq(tPjCb, cfgInfo->reEstReq, cpjReEstCfm);
   }

#ifndef PJ_ASYNC_CFM
   PjUiCpjReEstCfm(&(cpjSap->pst), cpjSap->suId, cpjReEstCfm);
#endif
   /* Configuration information needs to be freed from proper region */
   PJ_PST_FREE(pst->region, pst->pool,reEstCfm, sizeof (UdxReEstCfmInfo));
   PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgInfo->reEstReq, sizeof(CpjReEstReqInfo));
   PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));

   RETVALUE(ROK);
}



#ifdef ANSI
PUBLIC S16 PjUlUdxSecCfgCfm
(
Pst * pst,
SuId suId,
UdxSecCfgCfmInfo * secCfgCfm
)
#else
PUBLIC S16 PjUlUdxSecCfgCfm(pst, suId, secCfgCfm)
Pst * pst;
SuId suId;
UdxSecCfgCfmInfo * secCfgCfm;
#endif
{
   S16         ret;
   CpjSecCfgCfmInfo  *cfgCfm;       /* Security Cfg Confirm structure */
   PjCpjSapCb          *cpjSap;
   PjCb            *tPjCb;
   PjCfgInfo       *cfgInfo;
   CpjSecCfgReqInfo *secCfg;
#ifdef PJ_SEC_ASYNC
   PjUlUeCb               *ueCb = NULLP;
#endif

   TRC3(PjUlUdxSecCfgCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef DEBUGP
   RLOG1(L_DEBUG, "PjUlUdxUeIdChgCfm(post, suId(%d), ", suId);
#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      /* INNC: ERROR CLASS */
      RLOG0(L_ERROR, "Invalid suId");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   cpjSap = &(tPjCb->u.ulCb->cpjSap);

   ret = pjDbmFindUlTransIdLst(tPjCb,secCfgCfm->transId, &cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Invalid transId");
      PJ_PST_FREE(pst->region, pst->pool,secCfgCfm, sizeof (UdxSecCfgCfmInfo));
      RETVALUE (RFAILED);
   }

   secCfg = cfgInfo->secCfgInfo;


   if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,(Data **)&cfgCfm,
      sizeof (CpjSecCfgCfmInfo)) != ROK)
   {
      /* Free memory for CfgReqInfo */
      /* kw002.201 Freeing from proper region */  
      PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, secCfg, sizeof(CpjSecCfgReqInfo));
      PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   if ( pjCfgPrcSecUlCfg(tPjCb,secCfg,cfgCfm) != ROK )
   {
      RLOG_ARG2(L_ERROR,DBG_UEID,secCfg->ueId, 
          "PjUiCpjSecCfgReq  cellId(%d) pjCfgSecCfg Failed: reason (%d)",
            secCfg->cellId, cfgCfm->reason);

   }
   else
   {
        if (secCfgCfm->status != CPJ_CFG_CFM_OK)
        {
            PJ_MEM_CPY(cfgCfm, secCfgCfm, sizeof (CpjSecCfgCfmInfo));             
        }
   }
   
   cfgCfm->transId = cfgInfo->uprLyrTransId;
#ifdef PJ_SEC_ASYNC
   ret = pjDbmFetchUlUeCb(tPjCb,secCfg->ueId, secCfg->cellId, &ueCb);
   if ( ueCb != NULLP )
   {
      pjUtlUlSaveCfmInfo(tPjCb, ueCb, PJ_SEC_ASYNC_CFM, TRUE,  ENTPJ, (PTR)cfgCfm, NULLP);
      PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgCfm, sizeof(CpjSecCfgCfmInfo));
   }
   else
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, secCfg->cellId,"UeId[%u] not found",
            secCfg->ueId);
      PjUiCpjSecCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
   }
#else
   PjUiCpjSecCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
#endif 

  /* Configuration information needs to be freed from proper region */
   PJ_PST_FREE(pst->region, pst->pool, secCfgCfm, sizeof (UdxSecCfgCfmInfo));
   PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, secCfg, sizeof(CpjSecCfgReqInfo));
   PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUlUdxUeIdChgCfm
(
Pst * pst,
SuId suId,
U32 transId,
UdxUeInfo  * ueInfo,
CmStatus status
)
#else
PUBLIC S16 PjUlUdxUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst * pst;
SuId suId;
U32 transId;
UdxUeInfo  * ueInfo;
CmStatus status;
#endif
{
   S16         ret;
   CmStatus    tStatus;
   PjUdxUlSapCb      *udxSap;            /* RGU SAP Control Block */
   PjCpjSapCb          *cpjSap;
   PjCb            *tPjCb;
   PjCfgInfo       *cfgInfo;

   TRC3(PjUlUdxUeIdChgCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef DEBUGP
   RLOG1(L_DEBUG, "PjUlUdxUeIdChgCfm(post, suId(%d), ", suId);
#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      /* INNC: ERROR CLASS */
      RLOG0(L_ERROR, "Invalid suId");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   udxSap = tPjCb->u.ulCb->udxUlSap + suId;

   ret = pjDbmFindUlTransIdLst(tPjCb, transId, &cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Invalid transId");
      PJ_PST_FREE(pst->region, pst->pool,ueInfo, sizeof (UdxUeInfo));
      RETVALUE (RFAILED);
   }

   ret = ROK;
   tStatus.reason = CPJ_CFG_REAS_NONE;
   tStatus.status = CPJ_CFG_CFM_OK;

   if ( (ret = pjCfgUlUeIdChng(tPjCb,cfgInfo->ueInfo,
                      cfgInfo->newUeInfo, &tStatus)) != ROK)
   {
#ifdef DEBUGP
      RLOG_ARG4(L_ERROR,DBG_UEID,cfgInfo->ueInfo->ueId, "Cpj UeId Chg Req"
            " old-ueId(%d), new-ueId(%d), cellId(%d))Failed: reason (%d)",
             cfgInfo->ueInfo->ueId, cfgInfo->newUeInfo->ueId,
             cfgInfo->newUeInfo->cellId, status.reason);
#endif
   }

   cpjSap = &(tPjCb->u.ulCb->cpjSap);

   if ((status.status == CPJ_CFG_CFM_OK) || 
         (tStatus.status == CPJ_CFG_CFM_OK))
   {
      tStatus.reason = CPJ_CFG_REAS_NONE;
      tStatus.status = CPJ_CFG_CFM_OK;
   }
 
   PjUiCpjUeIdChgCfm(&cpjSap->pst,cpjSap->suId,cfgInfo->uprLyrTransId,(CpjUeInfo *)ueInfo,tStatus);

   /* Free the buffers allocated before returning */
   PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgInfo->ueInfo, sizeof(CpjUeInfo));
   PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgInfo->newUeInfo, sizeof(CpjUeInfo));
   PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));


   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUlUdxCountCfm
(
Pst * pst,
SuId suId,
UdxCountCfmInfo *cntCfm
)
#else
PUBLIC S16 PjUlUdxCountCfm(pst, suId, cntCfm) 
Pst * pst;
SuId suId;
UdxCountCfmInfo *cntCfm;
#endif
{

   S16         ret;
   PjUdxUlSapCb      *udxSap;            /* RGU SAP Control Block */
   PjCpjSapCb          *cpjSap;
   PjCb            *tPjCb;
   PjCfgInfo       *cfgInfo;
   CpjCountReqInfo   *countReq;
   CpjCountCfmInfo *countCfm;

   TRC3(PjUlUdxUeIdChgCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef DEBUGP
   RLOG1(L_DEBUG, "PjUlUdxUeIdChgCfm(post, suId(%d), ", suId);
#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      /* INNC: ERROR CLASS */
      RLOG0(L_ERROR, "Invalid suId");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   udxSap = tPjCb->u.ulCb->udxUlSap + suId;

   ret = pjDbmFindUlTransIdLst(tPjCb, cntCfm->transId, &cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Invalid transId");
      PJ_PST_FREE(pst->region, pst->pool,cntCfm, sizeof (UdxCountCfmInfo));
      RETVALUE (RFAILED);
   }
   cpjSap = &tPjCb->u.ulCb->cpjSap;   
   countReq = cfgInfo->cntReq;
   if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,(Data **)&countCfm,
      sizeof (CpjCountCfmInfo)) != ROK)
   {   
      /* Free memory for CfgReqInfo */
      /* kw002.201 Freeing from proper region */
     /* ccpu00136902 */
   /*   PJ_PST_FREE(pst->region, pst->pool, countReq, sizeof(CpjCountReqInfo)); */
      PJ_PST_FREE(pst->region, pst->pool, cntCfm,sizeof(UdxCountCfmInfo)); /*MEM_LEAK_FIX */   
      PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, countReq, sizeof(CpjCountReqInfo));    
      PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));  /*MEM_LEAK_FIX */
      RLOG0(L_FATAL, "Memory Allocation failed.");

      RETVALUE(RFAILED);
   }
   PJ_MEM_CPY(countCfm, cntCfm, sizeof(CpjCountCfmInfo));      /* KW_FIX */
   if ( pjCfgPrcUlCount(tPjCb, countReq, countCfm, cntCfm) != ROK )
   {
      RLOG_ARG2(L_ERROR,DBG_UEID,countReq->ueId, 
            "PjUiCpjCountReq cellId(%d)  pjCfgSecCfg Failed: reason (%d)",
                countReq->cellId, countCfm->reason);

   }

   countCfm->transId = cfgInfo->uprLyrTransId;
   /* Ned to consolidate confirms */
   /* Freeing from proper region */
   
   PJ_PST_FREE(pst->region, pst->pool, cntCfm,sizeof(UdxCountCfmInfo)); /*MEM_LEAK_FIX */   
   PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, countReq, sizeof(CpjCountReqInfo));    
   PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));  /*MEM_LEAK_FIX */


   PjUiCpjCountCfm(&(cpjSap->pst), cpjSap->suId,countCfm);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUlUdxSduStaCfm
(
Pst              *pst,
SuId              suId,
UdxSduStaCfmInfo *staCfm
)
#else
PUBLIC S16 PjUlUdxSduStaCfm(pst, suId, staCfm)
Pst              *pst;
SuId              suId;
UdxSduStaCfmInfo *staCfm;
#endif
{
   S16                ret;
   PjUdxUlSapCb      *udxSap;            /* UDX SAP Control Block */
   PjCpjSapCb        *cpjSap;
   PjCb              *tPjCb;
   PjCfgInfo         *cfgInfo;
   CpjSduStaReqInfo  *staReq;  
   CpjSduStaCfmInfo  *sduStaCfm;

   TRC3(PjUlUdxSduStaCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef DEBUGP
   RLOG1(L_DEBUG, "PjUlUdxSduStaCfm(post, suId(%d), ", suId); 
#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      /* INNC: ERROR CLASS */
      RLOG0(L_ERROR, "Invalid suId");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   udxSap = tPjCb->u.ulCb->udxUlSap + suId;
   cpjSap = &(tPjCb->u.ulCb->cpjSap);
   ret = pjDbmFindUlTransIdLst(tPjCb, staCfm->transId, &cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Invalid transId");
      PJ_PST_FREE(pst->region, pst->pool,staCfm, sizeof (UdxSduStaCfmInfo));
      RETVALUE (RFAILED);
   }

   staReq = cfgInfo->staReq;
   
   if (ROK == staCfm->status)
   {
      if (pjCfgPrcUlSduSta(tPjCb, staReq, staCfm) != ROK )
      {
         RLOG_ARG1(L_ERROR,DBG_UEID,staReq->ueId, 
             "PjUlUdxSduStaReq ,cellId(%d) pjCfgSecCfg Failed",staReq->cellId);
         /* Memory leak fix ccpu00135359 */
         PJ_PST_FREE(pst->region, pst->pool,staCfm, sizeof (UdxSduStaCfmInfo));
         ret = RFAILED;
      }
   }
   else
   {
      /* Memory leak fix ccpu00135359 */
      if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,(Data **)&sduStaCfm, 
         sizeof (CpjSduStaCfmInfo)) != ROK)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         PJ_PST_FREE(pst->region, pst->pool,staCfm, sizeof (UdxSduStaCfmInfo));
         PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgInfo->staReq, sizeof(CpjSduStaReqInfo));
         PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));
         RETVALUE(RFAILED);
      }
      sduStaCfm->ueId   = staCfm->ueId; 
      sduStaCfm->cellId = staCfm->cellId;
      sduStaCfm->transId = cfgInfo->uprLyrTransId;
      sduStaCfm->status = RFAILED;
      sduStaCfm->reason = staCfm->reason; 
      sduStaCfm->numRb = staCfm->numRb; 

      RLOG_ARG1(L_DEBUG,DBG_UEID,staCfm->ueId, 
            "PjUlUdxSduStaCfm- UdxSduStaCfm fail - cellId(%d)",staCfm->cellId);

      PjUiCpjSduStaCfm(&(cpjSap->pst), cpjSap->suId, sduStaCfm); 
      PJ_PST_FREE(pst->region, pst->pool,staCfm, sizeof (UdxSduStaCfmInfo));
   }

   PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgInfo->staReq, sizeof(CpjSduStaReqInfo));
   PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjUlUdxDatResumeCfm
(
Pst                   *pst,
SuId                  suId,
UdxDatResumeCfmInfo   *udxDatResumeCfm
)
#else
PUBLIC S16 PjUlUdxDatResumeCfm(pst, suId, udxDatResumeCfm)
Pst                   *pst;
SuId                  suId;
UdxDatResumeCfmInfo   *udxDatResumeCfm;
#endif
{
   S16                ret;
   PjUdxUlSapCb      *udxSap;            /* UDX SAP Control Block */
   PjCpjSapCb        *cpjSap;
   PjCb              *tPjCb;
   PjCfgInfo         *cfgInfo;
   CpjDatResumeReqInfo  *datResReq;  
   CpjDatResumeCfmInfo  *datResumeCfmInfo;

   TRC3(PjUlUdxDatResumeCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef DEBUGP
   RLOG1(L_DEBUG, "PjUlUdxDatResumeCfm(post, suId(%d), ", suId); 
#endif

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      /* INNC: ERROR CLASS */
      RLOG0(L_ERROR, "Invalid suId");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   udxSap = tPjCb->u.ulCb->udxUlSap + suId;

   ret = pjDbmFindUlTransIdLst(tPjCb, udxDatResumeCfm->transId, &cfgInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Invalid transId");
      PJ_PST_FREE(pst->region, pst->pool,udxDatResumeCfm, sizeof (UdxDatResumeCfmInfo));
      RETVALUE (RFAILED);
   }

   datResReq = cfgInfo->datResReq;
   
   pjUtlUlPrcsDatResume (tPjCb, datResReq);


   /* Perform processing for filling up cfm structures */
   cpjSap = &(tPjCb->u.ulCb->cpjSap);
   if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,(Data **)&datResumeCfmInfo, 
      sizeof (CpjDatResumeCfmInfo)) != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool,udxDatResumeCfm, sizeof (UdxDatResumeCfmInfo));
      PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgInfo->datResReq, sizeof(CpjDatResumeReqInfo));
      PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));
      RETVALUE(RFAILED);
   }
   datResumeCfmInfo->ueId = datResReq->ueId;
   datResumeCfmInfo->cellId = datResReq->cellId;
   datResumeCfmInfo->transId = datResReq->transId;
   datResumeCfmInfo->status = CPJ_CFG_CFM_OK;
   PjUiCpjDatResumeCfm(&(cpjSap->pst), cpjSap->suId, datResumeCfmInfo);

   PJ_PST_FREE(pst->region, pst->pool,udxDatResumeCfm, sizeof (UdxDatResumeCfmInfo));
   PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgInfo->datResReq, sizeof(CpjDatResumeReqInfo));
   PJ_FREE(tPjCb, cfgInfo, sizeof (PjCfgInfo));

   RETVALUE(ROK);
}

/**
 * @brief
 * This function processes Data Forwarding Confirm received from DL.
 *
 * Now the UL Module can start forwardng the UL Packets is Application 
 * has requested for UL Data Forwarding.
 *
 *  @param[in] pst      post structure
 *  @param[in] measType meas Type
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
 */
#ifdef ANSI 
PUBLIC S16 PjUlUdxDatForwardCfm
(
 Pst               *pst,
 SuId              suId,
 UdxDatFwdCfmInfo  *datFwdCfm
 )
#else
PUBLIC S16 PjUlUdxDatForwardCfm(pst, suId, datFwdCfm)
 Pst               *pst;
 SuId              suId;
 UdxDatFwdCfmInfo  *datFwdCfm;
#endif
{
   PjCb        *tPjCb;
   PjUlUeCb    *ueCb;
   PjUlRbCb    *pjRbCb;
   S16          ret;
   U8           rbCnt = 0;

   tPjCb = PJ_GET_PJCB(pst->dstInst);

   ret = pjDbmFetchUlUeCb(tPjCb,datFwdCfm->ueId,
         datFwdCfm->cellId, &ueCb);

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, datFwdCfm->cellId,"UeId[%u] not found",
            datFwdCfm->ueId);
      PJ_PST_FREE(pst->region, pst->pool, datFwdCfm, sizeof(UdxDatFwdCfmInfo));
      RETVALUE(ret);
   }

   /* Forward UL Packets only if Application has requested */
   if(ueCb->isUlDatFwdReqd == TRUE)
   {
      /* Call the UL Pkt Fwd Func for each Rab */
      for(rbCnt = 0; rbCnt < PJ_MAX_DRB_PER_UE; rbCnt++)
      {
         if((pjRbCb = ueCb->drbCb[rbCnt]) == NULLP)
         {
            continue;
         }
         /* UL Data Forwarding done only for AM Mode Bearers 
          * datQ is in Buffer State and only during Handover */
         if((pjRbCb->mode == PJ_DRB_AM) && (ueCb->libInfo.state == PJ_STATE_REEST_HO))
         {
            pjUlmStartDataFrwdPerRb(tPjCb, pjRbCb);
         }
      }
   }

   PJ_PST_FREE(pst->region, pst->pool, datFwdCfm, sizeof(UdxDatFwdCfmInfo));
   RETVALUE(ROK);
}


/**
 *
 * @brief 
 *
 *        Handler for Configuration Request on UL
 *
 * @b Description:
 *
 *        This function handles the configuration on UL after receiving 
 *        the configuration confirm from DL.
 *
 *  @param[in] gCb     PDCP Instance Control block
 *  @param[in] cfgInfo UL Configuration Info 
 *  @param[in] cfmInfo Configuration Confirm Info 
 *  @param[in] cfgCfm  Configuration Confirm Info 
 *  @param[in] sndCfgCfm  Configuration Confirm Info 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PRIVATE S16 pjHdlCpjUlCfgReq
(
PjCb         *gCb,
PjCfgInfo    *cfgInfo,
UdxCfgCfmInfo *cfmInfo,
CpjCfgCfmInfo *cfgCfm,
Bool           *sndCfgCfm
)
#else
PRIVATE S16 pjHdlCpjUlCfgReq(gCb, cfgInfo, cfmInfo, cfgCfm, sndCfgCfm)
PjCb         *gCb;
PjCfgInfo    *cfgInfo;
UdxCfgCfmInfo *cfmInfo;
CpjCfgCfmInfo *cfgCfm;
Bool           *sndCfgCfm;
#endif
{
   CpjCfgReqInfo       *cfg;
   U8                   idx;
   Bool                 reEstPres;     /* ReEstablishment present */
   Bool                 cfmWait;       /* Add present */
   Bool                 cfmType = 0;
   Bool                 ueDelWait;     /* Add present */
   Bool                 startTmr;
   PjUlUeCb            *ueCb;       /* UE Control Block for storing the
                                          cfg structure in case of ReEstablishment */ 
   PjCpjSapCb          *cpjSap;

   cfg = cfgInfo->cfgInfo;
   reEstPres = FALSE;
   cfmWait = FALSE;
   ueDelWait = FALSE;
   startTmr  = FALSE;
   ueCb      = NULLP;

   /* Fetch UeCb */
   pjDbmFetchUlUeCb(gCb, cfg->ueId, cfg->cellId, &ueCb);
  /* KW_FIX : CPJ_MAX_CFG,CPJ_MAX_CFM,UDX_MAX_CFM have the same value , hence checking for only one of them */
   for (idx = 0; idx < cfg->numEnt && idx < CPJ_MAX_CFG; idx++)
   {
      CpjCfmEnt  *entCfgCfm;
      UdxCfmEnt  *entDlCfgCfm;
      CpjCfgEnt  *entCfg;

      entCfg  = &(cfg->cfgEnt[idx]);
      entCfgCfm   = &(cfgCfm->cfmEnt[idx]);
      entDlCfgCfm = &(cfmInfo->cfmEnt[idx]);

      if (NULL != ueCb)
      {
            /* Fill the detault values here */
         PJ_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType, 
                   CPJ_CFG_CFM_OK,CPJ_CFG_REAS_NONE);
      }
      else
      {
         RLOG_ARG1(L_INFO, DBG_CELLID, cfg->cellId,"UeId[%u] not found",
            cfg->ueId);
         PJ_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType, 
                       CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_UE_UNKWN);
         if (entCfg->cfgType !=  CPJ_CFG_ADD)
         {
            /* all other requests require ueCb mandatorily */
            continue;
         }
       }
            
      switch (entCfg->cfgType)
      {
         case CPJ_CFG_ADD:
            {
               if (entCfg->m.addCfg.rlcInfo.dir & PJ_DIR_UL)
               {
                  U8 isHo;
                  isHo = cfg->isHo;
                  /* Add a new RB entity configuration */
                  if (pjCfgAddUlRb(gCb,cfg->ueId, cfg->cellId, isHo,
                              entCfg, &ueCb, entCfgCfm) != ROK)
                  {
#ifdef DEBUGP
                      RLOG_ARG3(L_ERROR,DBG_UEID,cfg->ueId,"UL RB Addition for"
                            "cellId(%d),rbId(%d) Failed: reason (%d)",
                            cfg->cellId, cfg->cfgEnt[idx].rbId, entCfgCfm->reason);
#endif
                      PJ_CFG_FILL_CFG_CFM(entCfgCfm,entCfg->rbId,entCfg->rbType, 
                            CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_UE_UNKWN);
                  }
                  /* Only In case of DRB only we need to set this flag */
#ifdef PJ_CMP_ASYNC
                  else if(PJ_DRB == entCfg->rbType)
                  {
                     if(entCfg->m.addCfg.hdrCompCfg.hdrCompUsed)
                     {
                        cfmType  |=  PJ_CFG_ASYNC_CFM;
                        startTmr =  TRUE;
                     }
                  }
#endif /* PJ_CMP_ASYNC */
                  if (entCfg->m.addCfg.rlcInfo.dir & PJ_DIR_DL)
                  {
                      if (entDlCfgCfm->status != CPJ_CFG_CFM_OK)
                      {
                        PJ_MEM_CPY(entCfgCfm, entDlCfgCfm, 
                                         sizeof(CpjCfmEnt));      /* KW_FIX */
                      }
                  }
               }
               else if (entCfg->m.addCfg.rlcInfo.dir & PJ_DIR_DL)
               {
                 PJ_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(CpjCfmEnt));       /* KW_FIX */
               }
               break;
            }

         case CPJ_CFG_MODIFY:
            {
               if ((entCfg->m.modCfg.bitFlag & 0x01) ||
                   (entCfg->m.modCfg.hoInfo.isHoPres & CPJ_HO_UL))
               {
                  /* Re-configure the existing RB entity configuration */
                  if (pjCfgReCfgUlRb(gCb,ueCb, entCfg, entCfgCfm) != ROK)
                  {
#ifdef DEBUGP
                     RLOG_ARG3(L_ERROR,DBG_UEID,cfg->ueId,"RB Config Modify HO"
                        "for cellId(%d), rbId(%d) Failed :reason (%d)",
                        cfg->cellId, cfg->cfgEnt[idx].rbId, entCfgCfm->reason);
#endif
                  }
#ifdef PJ_CMP_ASYNC
                  else if(PJ_DRB == entCfg->rbType)
                  {
                     if(entCfg->m.modCfg.hdrCompCfg.hdrCompUsed)
                     {
                        cfmType  |=  PJ_CFG_ASYNC_CFM;
                        startTmr =  TRUE;
                     }
                  }
#endif /* PJ_CMP_ASYNC */
                  if ((entCfg->m.modCfg.bitFlag & 0x02) ||
                     (entCfg->m.modCfg.hdrCompCfg.hdrCompUsed) ||
                     (entCfg->m.modCfg.hoInfo.isHoPres & CPJ_HO_DL))
                  {
                      if (entDlCfgCfm->status != CPJ_CFG_CFM_OK)
                      {
                        PJ_MEM_CPY(entCfgCfm, entDlCfgCfm, 
                                         sizeof(CpjCfmEnt)); 
                      }
                  }
               }
               else  if (entCfg->m.modCfg.hdrCompCfg.hdrCompUsed) 
               {
                  /* Re-configure the existing RB entity configuration */
                  if (pjCfgReCfgUlRb(gCb,ueCb, entCfg, entCfgCfm) != ROK)
                  {
#ifdef DEBUGP
                     RLOG_ARG3(L_ERROR,DBG_UEID,cfg->ueId,"UL RB Modify Normal"
                        " for cellId(%d), rbId(%d) Failed: reason (%d)",
                        cfg->cellId, cfg->cfgEnt[idx].rbId, entCfgCfm->reason);
#endif
                     if (entCfgCfm->reason == CPJ_CFG_REAS_PDCPID_UNKWN)
                     {
                       PJ_MEM_CPY(entCfgCfm, entDlCfgCfm,sizeof(CpjCfmEnt)); 
                     }
                  }
#ifdef PJ_CMP_ASYNC
                  else if(PJ_DRB == entCfg->rbType)
                  {
                     if(entCfg->m.modCfg.hdrCompCfg.hdrCompUsed)
                     {
                        cfmType  |=  PJ_CFG_ASYNC_CFM;
                        startTmr =  TRUE;
                     }
                  }
#endif /* PJ_CMP_ASYNC */

                  if ((entDlCfgCfm->reason != CPJ_CFG_REAS_PDCPID_UNKWN) &&
                     (entDlCfgCfm->status != CPJ_CFG_CFM_OK))
                  {
                    /* send configuration to DL instance */
                    PJ_MEM_CPY(entCfgCfm, entDlCfgCfm,sizeof(CpjCfmEnt));     /* KW_FIX */
                  }
               }
               else  if ((entCfg->m.modCfg.bitFlag & 0x02) ||
                     (entCfg->m.modCfg.hoInfo.isHoPres & CPJ_HO_DL))
               {
                  /* send configuration to DL instance */
                  PJ_MEM_CPY(entCfgCfm, entDlCfgCfm,sizeof(CpjCfmEnt));  /* KW_FIX */
               }
               break;
            }
         case CPJ_CFG_DELETE:
            {
               /* Delete the existing RB entity configuration */
               if (pjCfgDelUlRb(gCb,ueCb,entCfg, entCfgCfm) != ROK)
               {
#ifdef DEBUGP
                  RLOG_ARG3(L_ERROR,DBG_UEID,cfg->ueId, "Ul RB Delete for"
                            "cellId(%d), rbId(%d) Failed: reason (%d)",
                      cfg->cellId, cfg->cfgEnt[idx].rbId, entCfgCfm->reason);
#endif
               }
               if ((entDlCfgCfm->status != CPJ_CFG_CFM_OK) || (entCfgCfm->status != CPJ_CFG_CFM_OK))
               {
                   PJ_MEM_CPY(entCfgCfm, entDlCfgCfm, 
                                    sizeof(CpjCfmEnt));   /* KW_FIX */
               }
               break;
            }

         case CPJ_CFG_REESTABLISH:
            {
               /* Re-establish the existing RB entity configuration */
               if (pjCfgReEstUlRb(gCb,ueCb,entCfg, entCfgCfm, entDlCfgCfm) != ROK)
               {
                  RLOG_ARG3(L_ERROR,DBG_UEID,cfg->ueId, "UL RB Re-establish "
                        "for cellId(%d), rbId(%d) Failed: reason (%d)",
                         cfg->cellId, cfg->cfgEnt[idx].rbId, entCfgCfm->reason);

               }
               else
               {
                  cfmType  |=  PJ_CFG_REEST_ASYNC_CFM;
                  startTmr = FALSE;
               }
               
               break;
            }
         case CPJ_CFG_DELETE_UE:
            {
               /* Delete all RB entity configuration under UE */
               if (pjCfgDelUlUe(gCb,ueCb,entCfg, entCfgCfm) != ROK)
               {
#ifdef DEBUGP
                  RLOG_ARG3(L_ERROR,DBG_UEID,cfg->ueId, "UL UE Delete for "
                       "cellId(%d), rbId(%d) Failed: reason (%d)",
                      cfg->cellId, cfg->cfgEnt[idx].rbId, entCfgCfm->reason);
#endif
               }
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
               else
               {
                  /* Wait for any messages in the Demand q to be processed */
                  cfmType  |=  PJ_CFG_UEDEL_ASYNC_CFM;
                  startTmr =  TRUE;
               }
#endif /* (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)) */
               if (entDlCfgCfm->status == CPJ_CFG_CFM_OK)
               {
                   PJ_MEM_CPY(entCfgCfm, entDlCfgCfm,
                                    sizeof(CpjCfmEnt));    /* KW_FIX */
               }
               break;
            }
         default:
            {
               PJ_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType,
                           CPJ_CFG_CFM_NOK, CPJ_CFG_REAS_INVALID_CFG);

#ifdef DEBUGP
               RLOG_ARG4(L_ERROR,DBG_UEID,cfg->ueId, "Cfg Req with Invalid Cfg"
                     "Type cellId(%d), cfgType(%d), rbId(%d)) Failed: reason (%d)",
                     cfg->cellId, entCfg->cfgType,
                     cfg->cfgEnt[idx].rbId, CPJ_CFG_REAS_INVALID_CFG);
#endif
            }
      }
     /* ccpu00137221 : KW Fix */
      if (NULLP == ueCb)
      {
         PJ_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType, 
               CPJ_CFG_CFM_NOK,CPJ_CFG_REAS_UE_UNKWN);
      }
   }

   /* Assign number of entity configuraitons and suId */
   cfgCfm->transId = cfgInfo->uprLyrTransId;
   cfgCfm->ueId = cfg->ueId;
   cfgCfm->cellId = cfg->cellId;
   cfgCfm->numEnt = cfg->numEnt;
   /* ccpu00137221 : KW Fix */
   if (NULLP == ueCb)
   {
      RETVALUE(ROK);
   }

   if (cfmType & PJ_CFG_REEST_ASYNC_CFM)
   {
      if (ueCb->libInfo.numReEstCmp > 0)
      {
         if (ueCb->libInfo.numReEstCmp > ueCb->libInfo.numReEstDrb)
         {
            /* Error Case */
            ueCb->libInfo.numReEstDrb = 0;
            ueCb->libInfo.numReEstCmp = 0;
         } 
         else
         {
            ueCb->libInfo.numReEstDrb -= ueCb->libInfo.numReEstCmp;
            ueCb->libInfo.numReEstCmp = 0;
         }
      }
      if (ueCb->libInfo.numReEstDrb == 0)
      {
#ifdef PJ_ASYNC_CFM
         pjUtlUlSaveCfmInfo(gCb, ueCb, cfmType, startTmr, ENTPJ, (PTR)cfgCfm, (PTR)cfg);
#endif
         pjUtlUlUpdUpSecKeys(gCb, ueCb);
#ifdef PJ_ASYNC_CFM
         /* Memory leak fix ccpu00135359 */
        cpjSap = &(tPjCb->u.ulCb->cpjSap);
        PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgCfm, sizeof(CpjCfgCfmInfo));
        *sndCfgCfm = FALSE;
#endif
      }
      else
      {
         pjUtlUlSaveCfmInfo(gCb, ueCb, cfmType, startTmr, ENTPJ, (PTR)cfgCfm, (PTR)cfg);
         cpjSap = &(gCb->u.ulCb->cpjSap);
         PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgCfm, sizeof(CpjCfgCfmInfo));
         *sndCfgCfm = FALSE;
      }
   }
   else
   {
      if (cfmType != 0)
      {
         pjUtlUlSaveCfmInfo(gCb, ueCb, cfmType, startTmr, ENTPJ, (PTR)cfgCfm, (PTR)cfg);
         /* Memory leak fix ccpu00135359 */
         cpjSap = &(gCb->u.ulCb->cpjSap);
         PJ_PST_FREE(cpjSap->pst.region, cpjSap->pst.pool, cfgCfm, sizeof(CpjCfgCfmInfo));
         *sndCfgCfm = FALSE;
      }
   }

   RETVALUE(ROK);
}


/********************************************************************30**
         End of file:     eq_udx_ul.c@@/main/TeNB_Main_BR/10 - Mon Aug 11 16:42:57 2014
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
/main/1      ---      gk   1. initial release.
/main/2      ---      gk   1. LTERLC Release 2.1
/main/3      pj005.201 ap  1. Added support for L2 Measurement feature.
*********************************************************************91*/
