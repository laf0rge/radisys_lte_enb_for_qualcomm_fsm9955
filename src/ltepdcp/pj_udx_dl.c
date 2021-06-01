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
  
        Name:    LTE-PDCP Layer - Upper Interface Functions
    
        Type:    C file
  
        Desc:    Source code for PDCP Upper Interface Module
                 This file contains following functions
      
        --PjDlUdxBndReq
        --PjDlUdxUbndReq
        --PjDlUdxCfgReq
        --PjDlUdxUeIdChgReq
        --PjDlUdxReEstReq
        --PjDlUdxSecCfgReq
        --PjDlUdxSduStaReq
     

     File:     pj_udx.c

     Sid:      eq_udx_dl.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:46:47 2014

     Prg:      vp

**********************************************************************/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=225;
/** @file gp_pj_udx.c
@brief PDCP Upper Interface Module 
*/


/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000372     LTE-MAC
 *      1000369     LTE-RRC
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
#include "lpj.h"           /* LPJ define */
#include "cpj.h"           /* RRC layer */
#include "pju.h"           /* PDCP service user */
#include "pj_env.h"        /* RLC environment options */
#include "pj.h"            /* RLC defines */
#include "pj_dl.h"
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
#include "lpj.x"           /* LM(PDCP) */
#include "cpj.x"           /* RRC layer */
#include "pju.x"           /* PDCP service user */
#include "pj_err.h"
#include "pj.x"
#include "pj_udx.h"          /* LIB */
#include "pj_udx.x"          /* LIB */
#include "pj_dl.x"
#include "pj_udx.x"

/* kw004.201 Assigned build issue in LTE RLC pj_uim.c */
#include "pj_lib.x"


/*****************************************************************************
 *                          UDX INTERFACE
 ****************************************************************************/
/**
 *
 * @brief 
 *
 *        Handler for binding the PDCP upper layer service user with 
 *        the PDCP layer.
 *
 * @b Description:
 *
 *        1. This function is used by PDCP user to request for binding to 
 *        PDCP. @n
 *        2. This function is called by the UDX interface to bind 
 *        PDCP's SAP (identified by spId) with the service user's
 *        SAP (identified by suId). @n
 *
 *  @param[in] pst   Post structure  
 *  @param[in] suId  Service user SAP ID 
 *  @param[in] spId  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjDlUdxBndReq 
(
Pst  *pst,   
SuId suId,  
SpId spId   
)
#else
PUBLIC S16 PjDlUdxBndReq (pst, suId, spId)
Pst *pst;    
SuId suId;   
SpId spId;   
#endif
{
   PjUdxDlSapCb      *udxSap;            /* pointer to session SAP */
   S16             ret;                /* return value */
   PjCb            *tPjCb;

   TRC3(PjDlUdxBndReq)

   ret = ROK;

   tPjCb = PJ_GET_PJCB(pst->dstInst);

   RLOG2(L_DEBUG, "PjDlUdxBndReq(pst, spId(%d), suId(%d))", spId, suId);

   udxSap = (tPjCb->u.dlCb->udxDlSap + spId);

   /* Verify UDX SAP State */
   switch(udxSap->state)
   {
      /* SAP is configured but not bound */
      case PJ_SAP_CFG:
      case PJ_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         udxSap->suId = suId;
         udxSap->pst.dstProcId = pst->srcProcId;
         udxSap->pst.dstEnt = pst->srcEnt;
         udxSap->pst.dstInst = pst->srcInst;

         /* Update the State */
         udxSap->state = PJ_SAP_BND;

         RLOG1(L_DEBUG, "PjDlUdxBndReq: state (%d)", udxSap->state);
         break;
      }

      /* SAP is already bound */
      case PJ_SAP_BND:
      {
         /* 
          * Sap is already bound check source, destination Entity and 
          * Proc Id
          */
         if (udxSap->pst.dstProcId != pst->srcProcId 
               || udxSap->pst.dstEnt != pst->srcEnt
               || udxSap->pst.dstInst != pst->srcInst
               || udxSap->suId != suId)
         {
#if 0 /*PRERIT */
            PJ_SEND_SAPID_ALARM(tPjCb,spId, LPJ_EVENT_UDX_BND_REQ, LCM_CAUSE_INV_PAR_VAL);
#endif

            RLOG1(L_DEBUG, "PjDlUdxBndReq: SAP already bound -- state(%d)", 
                         udxSap->state);
            ret = RFAILED;
         }
         break;
      }

     default:
      {
         RLOG0(L_ERROR, "Invalid UDX DL SAP State in Bind Req");
         ret = RFAILED;
         break;
      }
   }

   if (ret == ROK)
   {
      PjDlUdxBndCfm(&(udxSap->pst), udxSap->suId, CM_BND_OK);
   }
   else
   {
      PjDlUdxBndCfm(&(udxSap->pst), udxSap->suId, CM_BND_NOK);
   }

   RETVALUE(ROK);
} /* PjDlUdxBndReq */


/**
 *
 * @brief 
 *
 *        Handler for unbinding the RLC upper layer service user UDX with 
 *        the RLC layer.
 *
 * @b Description:
 *
 *        1. This function is used by RLC user to request for unbinding  
 *        with RLC. @n
 *        2. This function is called by the UDX interface to 
 *        unbind with RLC. @n
 *
 *  @param[in] pst     Post structure  
 *  @param[in] spId    Service provider SAP ID 
 *  @param[in] reason  Reason for Unbinding 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjDlUdxUbndReq
(
Pst      *pst,            
SpId     spId,           
Reason   reason           
)
#else
PUBLIC S16 PjDlUdxUbndReq(pst, spId, reason)
Pst      *pst;       
SpId     spId;      
Reason   reason;      
#endif
{
   PjUdxDlSapCb  *udxSap;            /* UDX SAP control block */
   S16         ret;                /* Return Value */
   PjCb   *tPjCb;

   TRC3(PjDlUdxUbndReq)

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   RLOG2(L_DEBUG, "PjDlUdxUbndReq(pst, spId(%d), reason(%d))", spId, reason);

   UNUSED(pst);
   UNUSED(reason);

   /* disable upper sap (UDX) */
   udxSap = &(tPjCb->u.dlCb->udxDlSap[0]);
   ret    = ROK;

   udxSap->state = PJ_SAP_CFG;

   RETVALUE(ROK);
} /* PjDlUdxUbndReq */


/**
 *
 * @brief 
 *
 *        Handler for Sending the PDCP Status Report.
 *
 * @b Description:
 *
 *        This function is used by PDCP-DL to send PDCP Status Report. 
 *
 *  @param[in] pst   -  Post structure  
 *  @param[in] spId  -  Serive Provider ID 
 *  @param[in] staRep   -  PDCP Status Report. 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjDlUdxUlStaRep
(
Pst             *pst,
SpId            spId,
UdxUlStaRepInfo   *staRep
)
#else
PUBLIC S16 PjDlUdxUlStaRep(pst, spId, staRep)
Pst            *pst;
SpId           spId;
UdxUlStaRepInfo  *staRep;
#endif
{
   PjCb                 *tPjCb;
   PjDlUeCb             *ueCb;
   PjDlRbCb             *tRbCb;
   PjUdxDlSapCb         *udxSap;       /* UDX SAP Control Block */
   S16                  ret;           /* Return Value */
   
   TRC3(PjDlUdxUlStaRep);

   ret = ROK;

   tPjCb = PJ_GET_PJCB(pst->dstInst);
#ifndef ALIGN_64BIT
   RLOG1(L_DEBUG, "PjDlUdxUlStaRep(pst, spId(%d))", spId);
#else
   RLOG1(L_DEBUG, "PjDlUdxUlStaRep(pst, spId(%d))", spId);
#endif

   /* Get the udxSap */
   udxSap = (tPjCb->u.dlCb->udxDlSap + spId);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb,spId, udxSap, ret);
   if (ret != ROK)
   {
      /* pj002.201 Freeing from proper region */
      PJ_FREE_BUF(staRep->sta);
      PJ_PST_FREE(pst->region, pst->pool, staRep, sizeof(UdxUlStaRepInfo));
      RETVALUE(RFAILED);
   }

   /* Fetch Ue Cb */
   ret = pjDbmFetchDlUeCb(tPjCb, staRep->pdcpId.ueId, staRep->pdcpId.cellId, &ueCb);

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, staRep->pdcpId.cellId,"UeId[%u] not found",
            staRep->pdcpId.ueId);
      PJ_FREE_BUF(staRep->sta);
      PJ_PST_FREE(pst->region, pst->pool, staRep, sizeof(UdxUlStaRepInfo));
      RETVALUE(ret);
   }

   /* Fetch Rb Cb */
   PJ_DBM_FETCH_DL_RBCB(staRep->pdcpId.rbId, staRep->pdcpId.rbType, ueCb, tRbCb);

   /* Rb Cb is not present or Rb Cb is not configured for PDCP */
   if( NULLP == tRbCb )
   {
      RLOG_ARG2(L_WARNING, DBG_UEID,staRep->pdcpId.ueId, "CellId[%u]:RbId[%d] not found",
            staRep->pdcpId.cellId, staRep->pdcpId.rbId);
      PJ_FREE_BUF(staRep->sta);
      PJ_PST_FREE(pst->region, pst->pool, staRep, sizeof(UdxUlStaRepInfo));         
      RETVALUE(RFAILED);
   }
   
   /* This will go to the RLC as a datReq. So we will get a confirm for it
       * we have to make sure that the count does not clash with the existing
       * counts. So pick a value that is least expected to clash. */
   if (tRbCb->state != PJ_STATE_NORMAL)
   {
      if (tRbCb->dlCb.staRep != NULL)
      {
        PJ_FREE_BUF(tRbCb->dlCb.staRep); 
      }
      tRbCb->dlCb.staRep = staRep->sta;
   }
   else
   {
      pjDlmSendDatReq(tPjCb,tRbCb,0xffffffff, staRep->sta);
   }
   
   PJ_PST_FREE(pst->region, pst->pool, staRep, sizeof(UdxUlStaRepInfo));

   RETVALUE(ROK);
} /* PjDlUdxUlStaRep */

/**
 *
 * @brief 
 *
 *        Handler for Handling the PDCP Status Report.
 *
 * @b Description:
 *
 *        This function is used by PDCP-DL to handle the PDCP Status Report
 *        from peer PDCP entity. 
 *
 *  @param[in] pst   -  Post structure  
 *  @param[in] spId  -  Serive Provider ID 
 *  @param[in] staRep   -  PDCP Status Report. 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjDlUdxDlStaRep
(
Pst             *pst,
SpId            spId,
UdxDlStaRepInfo   *staRep
)
#else
PUBLIC S16 PjDlUdxDlStaRep(pst, spId, staRep)
Pst            *pst;
SpId           spId;
UdxDlStaRepInfo  *staRep;
#endif
{
   PjCb                 *tPjCb;
   PjDlUeCb             *ueCb;
   PjDlRbCb             *tRbCb;
   PjUdxDlSapCb         *udxSap;       /* UDX SAP Control Block */
   S16                  ret;           /* Return Value */
   
   TRC3(PjDlUdxDlStaRep);

   ret = ROK;
   
   tPjCb = PJ_GET_PJCB(pst->dstInst);
#ifndef ALIGN_64BIT
   RLOG1(L_DEBUG, "PjDlUdxDlStaRep(pst, spId(%d))", spId);
#else
   RLOG1(L_DEBUG, "PjDlUdxDlStaRep(pst, spId(%d))", spId);
#endif

   /* Get the udxSap */
   udxSap = (tPjCb->u.dlCb->udxDlSap + spId);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb,spId, udxSap, ret);
   if (ret != ROK)
   {
      /* pj002.201 Freeing from proper region */
      PJ_FREE_BUF(staRep->staPdu);
      PJ_PST_FREE(pst->region, pst->pool, staRep, sizeof(UdxDlStaRepInfo));
      RETVALUE(RFAILED);
   }

   /* Fetch Ue Cb */
   ret = pjDbmFetchDlUeCb(tPjCb, staRep->pdcpId.ueId, staRep->pdcpId.cellId, &ueCb);

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, staRep->pdcpId.cellId,"UeId[%u] not found",
            staRep->pdcpId.ueId);
      PJ_FREE_BUF(staRep->staPdu);
      PJ_PST_FREE(pst->region, pst->pool, staRep, sizeof(UdxDlStaRepInfo));
      RETVALUE(ret);
   }

   /* Fetch Rb Cb */
   PJ_DBM_FETCH_DL_RBCB(staRep->pdcpId.rbId, staRep->pdcpId.rbType, ueCb, tRbCb);

   /* Rb Cb is not present or Rb Cb is not configured for PDCP */
   if( NULLP == tRbCb )
   {
      RLOG_ARG2(L_WARNING, DBG_UEID,staRep->pdcpId.ueId, "CellId[%u]:RbId[%d] not found",
            staRep->pdcpId.cellId, staRep->pdcpId.rbId);
      PJ_FREE_BUF(staRep->staPdu);
      PJ_PST_FREE(pst->region, pst->pool, staRep, sizeof(UdxDlStaRepInfo));         
      RETVALUE(RFAILED);
   }
   
   ret = pjDlmHndlStaRep(tPjCb, tRbCb, staRep->fms, staRep->staPdu);

   PJ_PST_FREE(pst->region, pst->pool, staRep, sizeof(UdxDlStaRepInfo));

   RETVALUE(ret);
} /* PjUdxDlStaRep */    



/**
 *
 * @brief 
 *
 *        Handler for configuring Downlink PDCP entities.
 *
 * @b Description:
 *
 *        This function is used by RRC to configure(add/delete/modify/
 *        reestalish/delete all RB in a UE) one or more PDCP entities. 
 *
 *        - UDX_CFG_ADD          => pjCfgAddRb
 *        - UDX_CFG_MODIFY       => pjCfgReCfgRb
 *        - UDX_CFG_DELETE       => pjCfgDelRb
 *        - UDX_CFG_REESTABLISH  => pjCfgReEstRb
 *        - UDX_CFG_DELETE_UE    => pjCfgDelUe
 *
 *  @param[in] pst   -  Post structure  
 *  @param[in] spId  -  Service Provider ID 
 *  @param[in] cfg   -  Configuration information for one or more PDCP entities. 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjDlUdxCfgReq
(
Pst             *pst,
SpId            spId,
UdxCfgReqInfo   *cfg
)
#else
PUBLIC S16 PjDlUdxCfgReq(pst, spId, cfg)
Pst            *pst;
SpId           spId;
UdxCfgReqInfo  *cfg;
#endif
{

   UdxCfgCfmInfo        *cfgCfm;       /* Configuraiton Confirm */
   U8                   idx;           /* Index */
   PjUdxDlSapCb         *udxSap;       /* UDX SAP Control Block */
   Bool                 reEstPres;     /* ReEstablishment present */
   Bool                 startTmr;      /* Add present */
   S16                  ret;           /* Return Value */
   PjDlUeCb            *ueCb;         /* UE Control Block for storing the
                                    cfg structure in case of ReEstablishment */
   U8                   cfmType;
   PjCb                *tPjCb;

   TRC3(PjDlUdxCfgReq);

   reEstPres = FALSE;
   cfmType   = 0;
   ret       = ROK;
   cfgCfm    = NULLP;
   idx       = 0;
   ueCb      = NULLP;
   startTmr  = 0;

   tPjCb = PJ_GET_PJCB(pst->dstInst);
#ifndef ALIGN_64BIT
   RLOG2(L_DEBUG, "PjDlUdxCfgReq(pst, spId(%d), TransId(%ld))", 
         spId, cfg->transId);
#else
   RLOG2(L_DEBUG, "PjDlUdxCfgReq(pst, spId(%d), TransId(%d))", 
         spId, cfg->transId);
#endif

   /* Get the udxsap */
   udxSap = (tPjCb->u.dlCb->udxDlSap + spId);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb,spId, udxSap, ret);
   if (ret != ROK)
   {
      /* pj002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(UdxCfgReqInfo));
      RETVALUE(RFAILED);
   }

   /* Allocate memory and memset to 0 for cfmInfo */
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&cfgCfm, 
      sizeof (UdxCfgCfmInfo)) != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      /* pj002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(UdxCfgReqInfo));
      RETVALUE(RFAILED);
   }

   /* Fetch UeCb */
   pjDbmFetchDlUeCb(tPjCb, cfg->ueId, cfg->cellId, &ueCb);
  /* KW_FIX : UDX_MAX_CFG , UDX_MAX_CFM have the same value , hence checking for only one */
   /* For every entity configuration process by cfgType */
   for (idx = 0; idx < cfg->numEnt && idx < UDX_MAX_CFG; idx++)
   {
      UdxCfmEnt         *entCfgCfm;
      UdxCfgEnt         *entCfg;

      entCfg  = (UdxCfgEnt *)&(cfg->cfgEnt[idx]);
      entCfgCfm   = (UdxCfmEnt *)&(cfgCfm->cfmEnt[idx]);

      if (NULL != ueCb)
      {
         /* Fill the detault values here */
         PJ_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType, UDX_CFG_CFM_OK,\
               UDX_CFG_REAS_NONE);
      }
      else
      {
         RLOG_ARG1(L_WARNING, DBG_CELLID,cfg->cellId,"UeId[%u] not found",
               cfg->ueId);
         PJ_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType, CPJ_CFG_CFM_NOK,\
            CPJ_CFG_REAS_UE_UNKWN);

         RLOG_ARG1(L_WARNING,DBG_UEID,cfg->ueId, 
           "Dl RB Config Req- UE CB is not found for cellid (%d)",cfg->cellId);
         if (entCfg->cfgType !=  UDX_CFG_ADD)
         {
            /* all other requests require ueCb mandatorily */
            continue;
         }
      }

      switch (entCfg->cfgType)
      {
         case UDX_CFG_ADD:
            {
               if (entCfg->m.addCfg.rlcInfo.dir & PJ_DIR_DL)
               {
                  U8 isHo;

                  isHo = cfg->isHo;
                  /* Add a new RB entity configuration */
                  if (pjCfgAddDlRb(tPjCb,cfg->ueId, cfg->cellId, isHo,
                                     entCfg, &ueCb,entCfgCfm) != ROK)
                  {
                     RLOG_ARG3(L_ERROR,DBG_UEID,cfg->ueId, "Dl Cfg RB Add Req"
                           " for cellId(%d), rbId(%d) Failed: reason (%d)",
                       cfg->cellId, cfg->cfgEnt[idx].rbId, entCfgCfm->reason);
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
               }
               break;
            }

         case UDX_CFG_MODIFY:
            {
               /* Re-configure the existing RB entity configuration */
               if ((entCfg->m.modCfg.bitFlag & 0x02) ||
                  (entCfg->m.modCfg.hdrCompCfg.hdrCompUsed) ||
                  (entCfg->m.modCfg.hoInfo.isHoPres & CPJ_HO_DL))
               {
                  if (pjCfgReCfgDlRb(tPjCb,ueCb, entCfg, entCfgCfm) != ROK)
                  {
                     RLOG_ARG3(L_ERROR,DBG_UEID,cfg->ueId,"Dl RB Modify Req"
                            "for cellId(%d),rbId(%d) not done as RbCb not found: reason(%d)",
                        cfg->cellId,cfg->cfgEnt[idx].rbId, entCfgCfm->reason);
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
             }
               break;
            }

         case UDX_CFG_DELETE:
            {
               /* Delete the existing RB entity configuration */
               if (pjCfgDelDlRb(tPjCb, ueCb, entCfg, entCfgCfm) != ROK)
               {
                  RLOG_ARG3(L_WARNING,DBG_UEID,cfg->ueId, "Dl RB Delete Req for"
                        "for cellId(%d), rbId(%d) not done as RbCb not found: reason (%d)",
                    cfg->cellId, cfg->cfgEnt[idx].rbId, entCfgCfm->reason);
               }
               break;
            }

         case UDX_CFG_REESTABLISH:
            {
               if (pjCfgReEstDlRb(tPjCb, ueCb, entCfg, entCfgCfm) != ROK)
               {
                   RLOG_ARG3(L_ERROR,DBG_UEID,cfg->ueId, "Dl RB Re-establih Req"
                         "for cellId(%d), rbId(%d) Failed: reason (%d)",
                      cfg->cellId, cfg->cfgEnt[idx].rbId, entCfgCfm->reason);
               }
               else
               {
                  cfmType  |=  PJ_CFG_REEST_ASYNC_CFM;
                  reEstPres= TRUE;
               }

               break;
            }

         case UDX_CFG_DELETE_UE:
            {
               /* Delete all RB entity configuration under UE */
               if (pjCfgDelDlUe(tPjCb, ueCb,entCfg, entCfgCfm) != ROK)
               {

                  RLOG_ARG3(L_ERROR,DBG_UEID,cfg->ueId, "Dl UE Delete Req for"
                     "cellId(%d), rbId(%d) Failed: reason (%d)",
                      cfg->cellId, cfg->cfgEnt[idx].rbId, entCfgCfm->reason);
               }
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
               else
               {
                  /* Wait for any messages in the Demand q to be processed */
                  cfmType  |=  PJ_CFG_UEDEL_ASYNC_CFM;
                  startTmr =  TRUE;
               }
#endif /* (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)) */
               break;
            }

         default:
            {
               PJ_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType, UDX_CFG_CFM_NOK,\
                     UDX_CFG_REAS_INVALID_CFG);

               RLOG_ARG3(L_ERROR,DBG_UEID,cfg->ueId, "Dl Cfg Req with Invalid"
                      "config type,cellId(%d),cfgType(%d),rbId(%d) ",
                     cfg->cellId, entCfg->cfgType, cfg->cfgEnt[idx].rbId);
            }
      }
   }

   /* Assign number of entity configuraitons and suId */
   cfgCfm->transId = cfg->transId;
   cfgCfm->ueId = cfg->ueId;
   cfgCfm->cellId = cfg->cellId;
   cfgCfm->numEnt = cfg->numEnt;

   if (cfmType != 0)
   {
      if (ueCb != NULLP)
      {
         if(TRUE == reEstPres )
         {
#ifdef PJ_ASYNC_CFM
            pjUtlDlSaveCfmInfo(tPjCb, ueCb, cfmType, startTmr, ENTPJ, (PTR)cfgCfm, (PTR)cfg);
#endif
            pjUtlDlUpdUpSecKeys(tPjCb, ueCb); 
#ifdef PJ_ASYNC_CFM
            PJ_PST_FREE(udxSap->pst.region, udxSap->pst.pool, cfgCfm, sizeof(UdxCfgCfmInfo));
#else
            PjDlUdxCfgCfm(&(udxSap->pst), udxSap->suId, cfgCfm);
#endif
          
         }
         else
         {
            pjUtlDlSaveCfmInfo(tPjCb, ueCb, cfmType, startTmr, ENTPJ, (PTR)cfgCfm, (PTR)cfg);
            PJ_PST_FREE(udxSap->pst.region, udxSap->pst.pool, cfgCfm, sizeof(UdxCfgCfmInfo));
         }
         /* Free the cfg */
         PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(UdxCfgReqInfo));
      }
      else
      {
         PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(UdxCfgReqInfo));
         PjDlUdxCfgCfm(&(udxSap->pst), udxSap->suId, cfgCfm);
      }
   }
   else
   {
      PJ_PST_FREE(pst->region, pst->pool, cfg, sizeof(UdxCfgReqInfo));
      PjDlUdxCfgCfm(&(udxSap->pst), udxSap->suId, cfgCfm);
   }

   RETVALUE(ret);
} /* PjDlUdxCfgReq */

/**
 *@details This primitive is used by RRC to change the UeId for the existing UE
           context.

 * @param pst     -  Point to the pst structure
 * @param spId    -  The ID of the service provider SAP in the PDCP layer 
 * @param transId -  Transaction ID. This field uniquily identifies
 *                   transaction between RRC and PDCP.
 * @param ueInfo    -  Old UE Id Info for which the change request has come 
 * @param newUeInfo -  New UE Id Info for existing UE context 
 * @return ROK
**/
#ifdef ANSI
PUBLIC S16 PjDlUdxUeIdChgReq
(
Pst        *pst, 
SpId       spId, 
U32        transId, 
UdxUeInfo  *ueInfo,
UdxUeInfo  *newUeInfo
)
#else
PUBLIC S16 PjDlUdxUeIdChgReq(pst,spId,transId,ueInfo,newUeInfo)
Pst        *pst;
SpId       spId;
U32        transId;
UdxUeInfo  *ueInfo;
UdxUeInfo  *newUeInfo;
#endif
{
   S16         ret;           /* Return Value */
   CmStatus    status;        /* Status of the request */
   PjUdxDlSapCb  *udxSap;       /* UDX SAP Control Block */
   UdxUeInfo   *tUeInfo;      /* Temporary UE info */
   PjCb         *tPjCb;

   TRC3(PjDlUdxUeIdChgReq)

   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifndef ALIGN_64BIT
   RLOG2(L_DEBUG, "PjDlUdxUeIdChgReq(pst, spId(%d), transId(%ld))", spId, transId);
#else
   RLOG2(L_DEBUG, "PjDlUdxUeIdChgReq(pst, spId(%d), transId(%d))", spId, transId);
#endif

   ret = ROK;
   tUeInfo = NULLP;
   udxSap = &(tPjCb->u.dlCb->udxDlSap[0]);
   status.reason = UDX_CFG_REAS_NONE;
   status.status = UDX_CFG_CFM_OK;
   
   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb,spId, udxSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(UdxUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(UdxUeInfo));
      RETVALUE(RFAILED);
   }

   if (pjCfgDlUeIdChng(tPjCb, ueInfo,
                   newUeInfo, &status) != ROK)
   {
      RLOG_ARG4(L_ERROR,DBG_UEID,ueInfo->ueId,"Dl UeId Change Req old-ueId(%d),"
            "new-ueId(%d), cellId(%d) Failed: reason (%d)",
             ueInfo->ueId, newUeInfo->ueId, newUeInfo->cellId, status.reason);
   }

   /* Allocate memory for UdxUeInfo */
   /* Memory leak fix ccpu00135359 */
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&tUeInfo,
        sizeof (UdxUeInfo)) != ROK)
   {
      /* Free memory for UdxUeInfo */
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(UdxUeInfo));
      PJ_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(UdxUeInfo));

      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   tUeInfo->ueId = ueInfo->ueId;
   tUeInfo->cellId = ueInfo->cellId;

   /* Free memory for CkwUeInfo */
   /* kw002.201 Freeing from proper region */
   PJ_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(UdxUeInfo));
   PJ_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(UdxUeInfo));

   PjDlUdxUeIdChgCfm(&(udxSap->pst), udxSap->suId, transId, tUeInfo, status);

   RETVALUE(ROK);
} /* PjDlUdxUeIdChgReq */


/**
 *@details  
 * 1. RRC uses this primitive to configure PDCP security parameters.
 * Integrity protection and/or Ciphering are configured by RRC based on the
 * selSecAct flag.  @n
 * 2. Integirty Protection/Ciphering are configured per UE and applicable
 * to all pdcp entities in that UE. Both Integrity protection and ciphering are
 * re-configured during re-establishment.  @n
 *
 * @param pst     -  Point to the pst structure
 * @param spId    -  The ID of the service provider SAP in the PDCP layer 
 * @param secCfg  -  Security Configuration Info per UE.
 *
 * @return 
 *       -# ROK
 *       -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 PjDlUdxSecCfgReq
(
Pst              *pst,
SpId             spId,
UdxSecCfgReqInfo *secCfg
)
#else
PUBLIC S16 PjDlUdxSecCfgReq(pst,spId,secCfg)
Pst              *pst;
SpId             spId;
UdxSecCfgReqInfo *secCfg;
#endif
{
   S16               ret;           /* Return Value */
   UdxSecCfgCfmInfo  *cfgCfm;       /* Security Cfg Confirm structure */
   PjUdxDlSapCb        *udxSap;       /* UDX SAP Control Block */
#ifdef PJ_SEC_ASYNC
   PjDlUeCb            *ueCb;         /* UE Control Block */
#endif
   PjCb   *tPjCb;

   TRC2(PjDlUdxSecCfgReq)
   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifndef ALIGN_64BIT 
   RLOG2(L_DEBUG, "PjDlUdxSecCfgReq(pst, spId(%d), TransId(%ld))", spId,
               secCfg->transId);
#else
   RLOG2(L_DEBUG, "PjDlUdxSecCfgReq(pst, spId(%d), TransId(%d))", spId,
               secCfg->transId);
#endif

   ret = ROK;
   cfgCfm = NULLP;
   udxSap = NULLP;

   udxSap = &(tPjCb->u.dlCb->udxDlSap[0]);

#ifdef PJ_SEC_ASYNC
   ueCb   = NULLP;
#endif
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&cfgCfm, 
      sizeof (UdxSecCfgCfmInfo)) != ROK)
   {
      /* Free memory for CfgReqInfo */
      /* kw002.201 Freeing from proper region */  
      PJ_PST_FREE(pst->region, pst->pool, secCfg, sizeof(UdxSecCfgReqInfo));

      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   if ( pjCfgPrcSecDlCfg(tPjCb,secCfg,
                            cfgCfm) != ROK )
   {
      RLOG_ARG2(L_ERROR,DBG_UEID,secCfg->ueId, " Dl Sec Cfg Req for cellId(%d)"
            " Failed: reason (%d)", secCfg->cellId, cfgCfm->reason);
   }

#ifdef PJ_SEC_ASYNC
   ret = pjDbmFetchDlUeCb(tPjCb,secCfg->ueId, secCfg->cellId, &ueCb);
   if ( ueCb != NULLP )
   {
      pjUtlDlSaveCfmInfo(tPjCb, ueCb, PJ_SEC_ASYNC_CFM, TRUE,  ENTPJ, (PTR)cfgCfm, NULLP);
      PJ_FREE(tPjCb, cfgCfm, sizeof(UdxSecCfgCfmInfo));
   }
   else
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, secCfg->cellId,"UeId[%u] not found",
            secCfg->ueId);
      PjDlUdxSecCfgCfm(&(udxSap->pst), udxSap->suId, cfgCfm);
   }
#else
   PjDlUdxSecCfgCfm(&(udxSap->pst), udxSap->suId, cfgCfm);
#endif 

  /* Configuration information needs to be freed from proper region */
   PJ_PST_FREE(pst->region, pst->pool, secCfg, sizeof(UdxSecCfgReqInfo));

   RETVALUE(ret); 
}



/**
 * @details
 * 1. RRC uses this primitive to re-establish the PDCP entity. @n
 * 2. This primitive is initiation of re-establishment procedure. @n
 * 3. This prmitive and corresponding confirmation shall be followed by
 *    PjDlUdxCfgReq procedure for entire re-establishment procedure to complete.
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used by
 * PDCP to identify its SAP.
 * @param[in] reEstReq - PDCP re-establishment request structure. 
 *
 */
#ifdef ANSI
PUBLIC S16 PjDlUdxReEstReq 
(
Pst             *pst, 
SpId            spId, 
UdxReEstReqInfo *reEstReq
)
#else 
PUBLIC S16 PjDlUdxReEstReq(pst,spId,reEstReq)
Pst             *pst;
SpId            spId;
UdxReEstReqInfo *reEstReq;
#endif
{
   S16               ret;           /* Return Value */
   UdxReEstCfmInfo   *reEstCfm;     /* Re-Establish Confirm */
   PjUdxDlSapCb        *udxSap;       /* UDX SAP Control Block */
   PjCb   *tPjCb;

   TRC2(PjDlUdxReEstReq);

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   RLOG1(L_DEBUG, "PjDlUdxReEstReq(pst, spId(%d))", spId);

   ret = ROK;
   udxSap = &(tPjCb->u.dlCb->udxDlSap[0]);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb,spId, udxSap, ret);
   if (ret != ROK)
   {
      /* pj002.201 Freeing from proper region */  
      PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(UdxReEstReqInfo));
      RETVALUE(RFAILED);
   }
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&reEstCfm, 
      sizeof (UdxReEstCfmInfo)) != ROK)
   {
      /* Free memory for CfgReqInfo */
      /* kw002.201 Freeing from proper region */  
      PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(UdxReEstReqInfo));

      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   if ( pjCfgDlReEstReq(tPjCb,(UdxReEstReqInfo *)reEstReq,
                              (UdxReEstCfmInfo *)reEstCfm) != ROK )
   {
      RLOG_ARG2(L_ERROR,DBG_UEID,reEstReq->pdcpId.ueId, "Dl Re-Est Req Cfg for"
            " cellId(%d)  Failed: reason (%u)",
         reEstReq->pdcpId.cellId, reEstCfm->status);

       PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(UdxReEstReqInfo));
       PjDlUdxReEstCfm(&(udxSap->pst), udxSap->suId, reEstCfm); 
       RETVALUE (ROK);
   }

   /* kw002.201 Freeing from proper region */  
   PJ_PST_FREE(pst->region, pst->pool, reEstReq, sizeof(UdxReEstReqInfo));

#ifdef PJ_ASYNC_CFM   
   /* If Async is enabled then we need to wait for confirmation */
   RETVALUE(ROK);
#endif
   PjDlUdxReEstCfm(&(udxSap->pst), udxSap->suId, reEstCfm); 
   RETVALUE(ROK); 
}


/**
 * @details
 * 1. RRC uses this primitive to request count value for all DRBs in a UE. @n
 * 2. PDCP sends the UL and DL count values in the confirmation of this request. 
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] countReq - PDCP Count Request Information. 
 *
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjDlUdxCountReq 
(
Pst             *pst, 
SpId            spId, 
UdxCountReqInfo *countReq
)
#else
PUBLIC S16 PjDlUdxCountReq(pst,spId,countReq)
Pst             *pst;
SpId            spId;
UdxCountReqInfo *countReq;
#endif
{

   S16               ret;           /* Return Value */
   UdxCountCfmInfo   *countCfm;     /* Count Confirm structure */
   PjUdxDlSapCb        *udxSap;       /* UDX SAP Control Block */
   PjCb   *tPjCb;

   TRC2(PjDlUdxCountReq);

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   RLOG1(L_DEBUG, "PjDlUdxCountReq(pst, spId(%d))", spId);

   ret = ROK;
   countCfm = NULLP;
   udxSap = &(tPjCb->u.dlCb->udxDlSap[0]);
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&countCfm, 
      sizeof (UdxCountCfmInfo)) != ROK)
   {
      /* Free memory for CfgReqInfo */
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, countReq, sizeof(UdxCountReqInfo));

      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
   if ( pjCfgPrcDlCount(tPjCb, countReq, 
                             countCfm) != ROK )
   {
      RLOG_ARG2(L_ERROR,DBG_UEID,countReq->ueId, "Dl Count Req for cellId(%d)"
            " Failed: reason (%d)",
          countReq->cellId, countCfm->reason);

   }

   /* Freeing from proper region */
   PJ_PST_FREE(pst->region, pst->pool, countReq, sizeof(UdxCountReqInfo));

   PjDlUdxCountCfm(&(udxSap->pst), udxSap->suId,countCfm);

   RETVALUE(ROK);

}

/**
 * 
 * @details
 * 1. RRC uses this primitive to request the status of the SDUs for DRBs in an UE. @n
 * 2. Its used as a reestablishment request during handover.  @n
 * 3. This primitive is used to forward the handover control Information to the target
 * eNodeB.
 *
 * @param[in] pst    - Pointer to post structure.
 * @param[in] spId   - Service provider ID. This is the reference number used
 * by PDCP to identify its SAP.
 * @param[in] sduStaReq - PDCP SDU Status Request Information. 
 *
 * @return ROK
 */
#ifdef ANSI
PUBLIC S16 PjDlUdxSduStaReq 
(
Pst              *pst, 
SpId             spId, 
UdxSduStaReqInfo *staReq
)
#else
PUBLIC S16 PjDlUdxSduStaReq(pst,spId,staReq)
Pst              *pst;
SpId             spId;
UdxSduStaReqInfo *staReq;
#endif
{

   S16               ret;          /* Return Value */
   PjUdxDlSapCb     *udxSap;       /* UDX SAP Control Block */
   PjCb             *tPjCb;

   TRC2(PjDlUdxSduStaReq);

   ret = ROK;
   udxSap = NULLP;

   tPjCb = PJ_GET_PJCB(pst->dstInst);
   udxSap = &(tPjCb->u.dlCb->udxDlSap[0]);

    RLOG1(L_DEBUG, "PjDlUdxSduStaReq(pst, spId(%d))", spId);


   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb,spId, udxSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      PJ_PST_FREE(pst->region, pst->pool, staReq, sizeof(UdxSduStaReqInfo));
      RETVALUE(RFAILED);
   }

   if (pjCfgPrcDlSduSta(tPjCb,(UdxSduStaReqInfo *) staReq) != ROK )
   {
      RLOG_ARG1(L_ERROR,DBG_UEID,staReq->ueId, 
            "Dl Sdu Sta Req for cellId(%d) Failed",staReq->cellId);
      ret = RFAILED;

   }
   /* kw002.201 Freeing from proper region */
   PJ_PST_FREE(pst->region, pst->pool, staReq, sizeof(UdxSduStaReqInfo));

   RETVALUE(ret);
}

/**
 *
 * @brief 
 *
 *        Handler for Sending the Feed back Packet Info to DL-RLC.
 *
 * @b Description:
 *
 *        This function is used by PDCP-DL to send Feedback Packet Info to DL-RLC. 
 *
 *  @param[in] pst   -  Post structure  
 *  @param[in] spId  -  Serive Provider ID 
 *  @param[in] fdbkPktInfo   -  Feedback Packet Info. 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjDlUdxUlFdbkPktInfo
(
Pst             *pst,
SpId            spId,
UdxUlFdbkPktInfo   *fdbkPktInfo
)
#else
PUBLIC S16 PjDlUdxUlFdbkPktInfo(pst, spId, fdbkPktInfo)
Pst            *pst;
SpId           spId;
UdxUlFdbkPktInfo  *fdbkPktInfo;
#endif
{
   PjCb                 *tPjCb;
   PjDlUeCb             *ueCb;
   PjDlRbCb             *tRbCb;
   PjUdxDlSapCb         *udxSap;       /* UDX SAP Control Block */
   S16                  ret;           /* Return Value */
   U32                  count;
   Data                 hdr = 0;
   Buffer               *rohcFdbPdu = NULLP;

   TRC3(PjDlUdxUlFdbkPktInfo);

   ret = ROK;
   tPjCb = PJ_GET_PJCB(pst->dstInst);
#ifndef ALIGN_64BIT
   RLOG1(L_DEBUG, "PjDlUdxUlFdbkPktInfo(pst, spId(%d))", spId);
#else
   RLOG1(L_DEBUG, "PjDlUdxUlFdbkPktInfo(pst, spId(%d))", spId);
#endif

   /* Get the udxSap */
   udxSap = (tPjCb->u.dlCb->udxDlSap + spId);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb,spId, udxSap, ret);
   if (ret != ROK)
   {
      /* pj002.201 Freeing from proper region */
      PJ_FREE_BUF(fdbkPktInfo->fbPkt);
      PJ_PST_FREE(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxDlFdbkPktInfo));
      RETVALUE(RFAILED);
   }

   /* Fetch Ue Cb */
   ret = pjDbmFetchDlUeCb(tPjCb, fdbkPktInfo->pdcpId.ueId, fdbkPktInfo->pdcpId.cellId, &ueCb);

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, fdbkPktInfo->pdcpId.cellId,"UeId[%u] not found",
            fdbkPktInfo->pdcpId.ueId);
      PJ_FREE_BUF(fdbkPktInfo->fbPkt);
      PJ_PST_FREE(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxDlFdbkPktInfo));
      RETVALUE(ret);
   }

   /* Fetch Rb Cb */
   PJ_DBM_FETCH_DL_RBCB(fdbkPktInfo->pdcpId.rbId, fdbkPktInfo->pdcpId.rbType, ueCb, tRbCb);

   /* Rb Cb is not present or Rb Cb is not configured for PDCP */
   if( NULLP == tRbCb )
   {
      RLOG_ARG2(L_WARNING, DBG_UEID,fdbkPktInfo->pdcpId.ueId, "CellId[%u]:RbId[%d] not found",
            fdbkPktInfo->pdcpId.cellId, fdbkPktInfo->pdcpId.rbId);
      PJ_FREE_BUF(fdbkPktInfo->fbPkt);
      PJ_PST_FREE(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxDlFdbkPktInfo));         
      RETVALUE(RFAILED);
   }
   
   hdr |= PJ_ROHC_FEEDBACK_HDR;
   rohcFdbPdu = fdbkPktInfo->fbPkt;

   ret = SAddPreMsg((Data) hdr, rohcFdbPdu);
   if ( ret != ROK )                                      
   {                                                       
       RLOG0(L_ERROR, "SAddPreMsg failed for ROHC Feedbck pdu");
       PJ_FREE_BUF(rohcFdbPdu);
       RETVALUE(RFAILED);                                    
   }                                                       
   
   if(tRbCb->mode == PJ_DRB_UM)
   {
       count = tRbCb->dlCb.nxtToSub - 1; 
   }
   else
   {
       count = tRbCb->dlCb.cfmExp - 1;
   }
   
   pjDlmSendDatReq(tPjCb, tRbCb, count, rohcFdbPdu);
   
   PJ_PST_FREE(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxDlFdbkPktInfo));

   RETVALUE(ROK);
} /* PjUlUdxUlFdbkPktInfo*/

/**
 *
 * @brief 
 *
 *        Handler for Sending the Feed back Packet Info to Compression entity.
 *
 * @b Description:
 *
 *        This function is used by PDCP-DL to send Feedback Packet Info to Compression entity. 
 *
 *  @param[in] pst   -  Post structure  
 *  @param[in] spId  -  Serive Provider ID 
 *  @param[in] fdbkPktInfo   -  Feedback Packet Info. 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjDlUdxDlFdbkPktInfo
(
Pst             *pst,
SpId            spId,
UdxDlFdbkPktInfo   *fdbkPktInfo
)
#else
PUBLIC S16 PjDlUdxDlFdbkPktInfo(pst, spId, fdbkPktInfo)
Pst            *pst;
SpId           spId;
UdxDlFdbkPktInfo  *fdbkPktInfo;
#endif
{
   PjCb                 *tPjCb;
   PjDlUeCb             *ueCb;
   PjDlRbCb             *tRbCb;
   PjUdxDlSapCb         *udxSap;       /* UDX SAP Control Block */
   S16                  ret;           /* Return Value */

#ifdef PJ_CMP_ASYNC
   PjLibTrans libTrans; /* Transaction Id for compression */
#endif /* PJ_SEC_ASYNC */

   TRC3(PjDlUdxDlFdbkPktInfo);

   ret = ROK;
   tPjCb = PJ_GET_PJCB(pst->dstInst);
#ifndef ALIGN_64BIT
   RLOG1(L_DEBUG, "PjDlUdxDlFdbkPktInfo(pst, spId(%d))", spId);
#else
   RLOG1(L_DEBUG, "PjDlUdxDlFdbkPktInfo(pst, spId(%d))", spId);
#endif

   /* Get the udxSap */
   udxSap = (tPjCb->u.dlCb->udxDlSap + spId);

   /* Validate SAP ID under ERRORCLS */
   PJ_VALDATE_SAP(tPjCb,spId, udxSap, ret);
   if (ret != ROK)
   {
      /* pj002.201 Freeing from proper region */
      PJ_FREE_BUF(fdbkPktInfo->fbPkt);
      PJ_PST_FREE(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxDlFdbkPktInfo));
      RETVALUE(RFAILED);
   }

   /* Fetch Ue Cb */
   ret = pjDbmFetchDlUeCb(tPjCb, fdbkPktInfo->pdcpId.ueId, fdbkPktInfo->pdcpId.cellId, &ueCb);

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, fdbkPktInfo->pdcpId.cellId,"UeId[%u] not found",
            fdbkPktInfo->pdcpId.ueId);
      PJ_FREE_BUF(fdbkPktInfo->fbPkt);
      PJ_PST_FREE(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxDlFdbkPktInfo));
      RETVALUE(ret);
   }

   /* Fetch Rb Cb */
   PJ_DBM_FETCH_DL_RBCB(fdbkPktInfo->pdcpId.rbId, fdbkPktInfo->pdcpId.rbType, ueCb, tRbCb);

   /* Rb Cb is not present or Rb Cb is not configured for PDCP */
   if( NULLP == tRbCb )
   {
      RLOG_ARG2(L_WARNING, DBG_UEID,fdbkPktInfo->pdcpId.ueId, "CellId[%u]:RbId[%d] not found",
            fdbkPktInfo->pdcpId.cellId, fdbkPktInfo->pdcpId.rbId);
      PJ_FREE_BUF(fdbkPktInfo->fbPkt);
      PJ_PST_FREE(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxDlFdbkPktInfo));         
      RETVALUE(RFAILED);
   }

#ifdef PJ_CMP_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = tRbCb->dlCb.nxtToSub;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = tRbCb->rbId - 1;
   libTrans.rbType = tRbCb->rbType;
   libTrans.ueCb   = (PTR)tRbCb->ueCb;

   PjLibObdCmpFbReq(&(tPjCb->pjGenCfg.obdPst.cmpPst), tRbCb->cmpCxtId, libTrans,
         fdbkPktInfo->fbPkt);
#else
   pjLibCmpFbReq(tRbCb->cmpCxtId, fdbkPktInfo->fbPkt);
#endif

   PJ_PST_FREE(pst->region, pst->pool, fdbkPktInfo, sizeof(UdxDlFdbkPktInfo));         

   RETVALUE(ROK);
} /* PjDlUdxDlFdbkPktInfo*/


#ifdef LTE_L2_MEAS
/**
*/
#ifdef ANSI
PUBLIC S16 PjDlUdxL2MeasReq 
(
Pst            *pst, 
PjL2MeasReqEvt *measReqEvt 
)
#else
PUBLIC S16 PjDlUdxL2MeasReq (pst, measReqEvt)
Pst            *pst; 
PjL2MeasReqEvt *measReqEvt;
#endif
{
   U16            cntr;
   U8             measType;

   PjCb     *tPjCb=NULLP;
   TRC3(PjDlUdxL2MeasReq);

   tPjCb =  PJ_GET_PJCB(pst->dstInst);
   /* Initialize measCfmEvt */


   measType = measReqEvt->measReq.measType;

   RLOG1(L_DEBUG, "L2_MEAS START Measurement type is %u",
             measType);

  for(cntr = 0; cntr < LPJ_MAX_QCI; cntr++)
  {
     tPjCb->u.dlCb->pjL2Cb.measOn[cntr] |= measType;
  }
#ifdef SS_LOCKLESS_MEMORY
      PJ_FREE_SHRABL_BUF(tPjCb,measReqEvt, sizeof(PjL2MeasReqEvt));
#else
      PJ_FREE(tPjCb,measReqEvt, sizeof(PjL2MeasReqEvt));
#endif
      /*stoping Task*/

  RETVALUE(ROK);
} /* KwDlUdxMeasReq */

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
PUBLIC S16 PjDlUdxL2MeasStopReq
(
Pst   *pst,
U8    measType
)
#else
PUBLIC S16 PjDlUdxL2MeasStopReq (pst, measType)
Pst   *pst;
U8    measType;
#endif
{
   S16 ret = ROK;
   PjL2MeasEvtCb   *measEvtCb = NULLP;
   PjL2MeasCb      *measCb    = NULLP;
   U16             cntr;
   U8              status = ROK;
   PjL2MeasCfmEvt  measCfmEvt;  
   PjCb     *tPjCb=NULLP;
   TRC3(PjDlUdxMeasStopReq);
   tPjCb =  PJ_GET_PJCB(pst->dstInst);
   RLOG0(L_DEBUG, "L2_MEAS Received stopReq\n");
   cmMemset((U8*)&measCfmEvt, 0, sizeof(PjL2MeasCfmEvt)); 
   for(cntr = 0; cntr < LPJ_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(tPjCb->u.dlCb->pjL2Cb.pjL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         measCb = &measEvtCb->measCb;
         pjUtlResetDlL2MeasCntr(tPjCb, measCb, measType);

      }
   }
   status = LCM_PRIM_OK; 
   PjMiLpjL2MeasStopCfm(&tPjCb->pjGenCfg.lmPst, measType,status); 
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
PUBLIC S16 PjDlUdxL2MeasSendReq
(
Pst            *pst,
U8             measType
)
#else
PUBLIC S16 PjDlUdxL2MeasSendReq (pst, measType)
Pst            *pst;
U8             measType;
#endif
{
   S16 ret = ROK;
   PjL2MeasEvtCb *measEvtCb = NULLP;
   U16            cntr;
   PjCb     *tPjCb=NULLP;
   TRC3(PjDlUdxMeasSendReq);

   tPjCb =  PJ_GET_PJCB(pst->dstInst);
   RLOG1(L_DEBUG, "L2_MEAS: Sending Meas for meastype %u",
             measType);

   for(cntr = 0; cntr < LPJ_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(tPjCb->u.dlCb->pjL2Cb.pjL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         pjUtlSndDlL2MeasCfm(tPjCb, measEvtCb);

      }
   }

   RETVALUE(ret);
}
#endif /* LTE_L2_MEAS */
/**
@brief 
This function processes Data Resume request received from the Application.
 *  @param[in] pst      post structure
 *  @param[in] measType meas Type 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

#ifdef ANSI
PUBLIC S16 PjDlUdxDatResumeReq
(
Pst               *pst,
UdxDatResumeInfo  *datResumeInfo
)
#else
PUBLIC S16 PjDlUdxDatResumeReq (pst, *datResumeInfo)
Pst               *pst;
UdxDatResumeInfo  *datResumeInfo;
#endif
{
   U8 rbCnt;
   PjDlUeCb *ueCb;
   PjDlRbCb *pjRbCb;
   PjCb     *tPjCb;
   S16       ret;
   UdxDatResumeCfmInfo  *cfmInfo;
   PjUdxDlSapCb   *udxSap;

   tPjCb =  PJ_GET_PJCB(pst->dstInst);

   /* Perform processing for filling up cfm structures */
   /* Memory leak fix ccpu00135359 */
   udxSap = &(tPjCb->u.dlCb->udxDlSap[0]);
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&cfmInfo,
        sizeof (UdxDatResumeCfmInfo)) != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, datResumeInfo, sizeof(UdxDatResumeInfo));         
      RETVALUE(RFAILED);
   }

   cfmInfo->ueId = datResumeInfo->ueId;
   cfmInfo->cellId = datResumeInfo->cellId;
   cfmInfo->transId = datResumeInfo->transId;
   cfmInfo->status = CPJ_CFG_CFM_OK; 
   cfmInfo->reason = CPJ_CFG_REAS_NONE;
   
   ret = pjDbmFetchDlUeCb(tPjCb, datResumeInfo->ueId, 
                    datResumeInfo->cellId, &ueCb);

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, datResumeInfo->cellId,"UeId[%u] not found",
            datResumeInfo->ueId);
      cfmInfo->status = CPJ_CFG_CFM_NOK; 
      cfmInfo->reason = CPJ_CFG_REAS_UE_UNKWN;
      PJ_PST_FREE(pst->region, pst->pool, datResumeInfo, sizeof(UdxDatResumeInfo));         
      /* Send Failure SduStaCfm */
      PjDlUdxDatResumeCfm(&tPjCb->u.dlCb->udxDlSap[0].pst, 
                        tPjCb->u.dlCb->udxDlSap[0].suId, cfmInfo);
      RETVALUE(ret);
   }

   for(rbCnt = 0; rbCnt < PJ_MAX_DRB_PER_UE; rbCnt ++)
   {
      /* Check of rbCb available */
      if( (pjRbCb = ueCb->drbCb[rbCnt]) == NULLP)
      {
         continue;
      }
  
      pjDlmRbDataResume(tPjCb,pjRbCb);
      pjRbCb->state = PJ_STATE_NORMAL;
   }

   ueCb->libInfo.state = PJ_STATE_NORMAL;
   PJ_PST_FREE(pst->region, pst->pool, datResumeInfo, sizeof(UdxDatResumeInfo));         
   
   PjDlUdxDatResumeCfm(&tPjCb->u.dlCb->udxDlSap[0].pst, 
                     tPjCb->u.dlCb->udxDlSap[0].suId, cfmInfo);
   RETVALUE(ROK);
}



/**
@brief 
This function processes Start Data Forwarding request received from UL 
 *  @param[in] pst      post structure
 *  @param[in] measType meas Type 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/

#ifdef ANSI
PUBLIC S16 PjDlUdxDatForwardReq
(
Pst                 *pst,
UdxDatForwardInfo   *datFwdInfo
)
#else
PUBLIC S16 PjDlUdxDatForwardReq (pst, *datFwdInfo)
Pst                 *pst;
UdxDatForwardInfo   *datFwdInfo;
#endif
{
   S16 ret;
   U8 rbCnt;
   PjDlUeCb *ueCb =NULLP;
   PjDlRbCb *pjRbCb = NULLP;
   PjCb     *tPjCb;
   UdxDatFwdCfmInfo  *cfmInfo = NULLP;
   PjUdxDlSapCb      *udxSap;

   tPjCb =  PJ_GET_PJCB(pst->dstInst);

   udxSap = &(tPjCb->u.dlCb->udxDlSap[0]);
   /* Allocate MEM and fill the DatFwd Cfm Struct */
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&cfmInfo, 
      sizeof (UdxDatFwdCfmInfo)) != ROK)
   {
      cfmInfo = NULLP;
#if (ERRCLASS & ERRCLS_DEBUG)
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_PST_FREE(pst->region, pst->pool, datFwdInfo, 
                  sizeof(UdxDatForwardInfo));         
      RETVALUE(RFAILED);
#endif
   }

   cfmInfo->ueId = datFwdInfo->ueId;
   cfmInfo->cellId = datFwdInfo->cellId;
   cfmInfo->status = CPJ_CFG_CFM_OK;

   ret = pjDbmFetchDlUeCb(tPjCb,datFwdInfo->ueId, 
         datFwdInfo->cellId, &ueCb);

   /* Ue Cb is not present */
   if( ROK != ret || NULLP == ueCb)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, datFwdInfo->cellId,"UeId[%u] not found",
            datFwdInfo->ueId);
      cfmInfo->status = CPJ_CFG_CFM_NOK;
      cfmInfo->reason = CPJ_CFG_REAS_UE_UNKWN;
      /* Send Cfm to UL so that it can fwd the UL Pkts */
      PjDlUdxDatFwdCfm(&tPjCb->u.dlCb->udxDlSap[0].pst,
                       tPjCb->u.dlCb->udxDlSap[0].suId, cfmInfo);
      PJ_PST_FREE(pst->region, pst->pool, datFwdInfo, sizeof(UdxDatForwardInfo));         
      RETVALUE(ret);
   }

   for(rbCnt = 0; rbCnt < PJ_MAX_DRB_PER_UE; rbCnt ++)
   {
      /* Check of rbCb available */
      if( (pjRbCb = ueCb->drbCb[rbCnt]) == NULLP)
      {
         continue;
      }

      if(pjRbCb->mode == PJ_DRB_AM)
      {
         pjDlmStartDataFrwdPerRb(tPjCb,pjRbCb);
      }
   }
   /* Send Data Fwd Cfm to the UL PDCP Module
    * The UL PDCP Module will forward UL Pkts
    * after receiving this cfm Info */
   PjDlUdxDatFwdCfm(&tPjCb->u.dlCb->udxDlSap[0].pst,
         tPjCb->u.dlCb->udxDlSap[0].suId, cfmInfo);

   PJ_PST_FREE(pst->region, pst->pool, datFwdInfo, sizeof(UdxDatForwardInfo));         
   RETVALUE(ROK);

}



/********************************************************************30**
         End of file:     eq_udx_dl.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:46:47 2014
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
/main/1      ---       vp   1. LTERLC Release 2.1
/main/2      kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/3      kw003.201 vp   1. Fixes for re-segmentation, segement offset
                               value and optimization 
/main/4      kw004.201 av   1. Fix for  build issue in LTE RLC pj_uim.c
/main/5      kw005.201 ap   1. Updated missing statistics in the code. 
*********************************************************************91*/
