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



/**********************************************************************
 
    Name:   LTE RLC layer
 
    Type:   C include file
 
    Desc:   Defines required by LTE MAC
 
    File:   kw_udx_dl.c
 
    Sid:      kw_udx_dl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:08 2015
 
    Prg:   mraj
 
**********************************************************************/
static const char* RLOG_MODULE_NAME="UDX";
static int RLOG_MODULE_ID=262144;
static int RLOG_FILE_ID=203;


/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*
*/
 

/* header include files (.h) */
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
#include "kw_udx.h"
#include "kw_dl.h"

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


#define KW_MODULE KW_DBGMASK_UDX
/* local defines */

EXTERN S16 kwDlmHndlStaRsp ARGS (( KwCb  *gCb,KwDlRbCb  *rbCb,
                KwUdxStaPdu *pStaPdu, KwUdxBufLst  *rlsPduLst));




/**
  * @brief
  * UDX APIs
  */

/**
 *
 * @brief 
 *    Handler to bind the DL with UL. 
 *
 * @param[in] pst   Post structure  
 * @param[in] suId  Service user SAP ID 
 * @param[in] spId  Service provider ID
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwDlUdxBndReq 
(
Pst    *pst,  
SuId   suId, 
SpId   spId 
)
#else
PUBLIC S16 KwDlUdxBndReq (pst, suId, spId)
Pst    *pst;   
SuId   suId;  
SpId   spId; 
#endif
{
   KwUdxDlSapCb   *udxSap;            /* pointer to session SAP */
   KwCb           *tKwCb;

   TRC3(KwDlUdxBndReq);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
#endif
   tKwCb = KW_GET_KWCB(pst->dstInst);

   KWDBGP_BRIEF(tKwCb, "KwDlUdxBndReq(spId(%d), suId(%d))\n", 
                spId, suId);

   udxSap = (tKwCb->u.dlCb->udxDlSap + spId);

   /* Verify CKW SAP State */
   switch(udxSap->state)
   {
      /* SAP is configured but not bound */
      case KW_SAP_CFG:
      case KW_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         udxSap->suId = suId;
         udxSap->pst.dstProcId = pst->srcProcId;
         udxSap->pst.dstEnt = pst->srcEnt;
         udxSap->pst.dstInst = pst->srcInst;

         /* Update the State */
         udxSap->state = KW_SAP_BND;

         RLOG1(L_INFO, "UDX SAP state [%d]", udxSap->state);
         break;
      }
      /* SAP is already bound */
      case KW_SAP_BND:
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
            KW_SEND_SAPID_ALARM(tKwCb, spId, 
                                LKW_EVENT_UDX_BND_REQ, LCM_CAUSE_INV_PAR_VAL);

            RLOG0(L_ERROR, "UDX SAP already Bound");
            KwDlUdxBndCfm(&(udxSap->pst), udxSap->suId, CM_BND_NOK);
         }
         break;
      }

     default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         KW_SEND_SAPID_ALARM(tKwCb,spId, 
                             LKW_EVENT_CKW_BND_REQ, LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         RLOG0(L_ERROR, "Invalid UDX SAP State in Bind Req");
         KwDlUdxBndCfm(&(udxSap->pst), udxSap->suId, CM_BND_NOK);
         break;
      }
   }
   KwDlUdxBndCfm(&(udxSap->pst), udxSap->suId, CM_BND_OK);
   RETVALUE(ROK);
} 


/**
 * @brief 
 *    Handler for unbinding the DL from UL. 
 *
 *  @param[in] pst     Post structure  
 *  @param[in] spId    Service provider SAP ID 
 *  @param[in] reason  Reason for Unbinding 
 *
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 KwDlUdxUbndReq
(
Pst      *pst,    
SpId     spId,   
Reason   reason 
)
#else
PUBLIC S16 KwDlUdxUbndReq(pst, spId, reason)
Pst      *pst;   
SpId     spId;   
Reason   reason; 
#endif
{
   KwUdxDlSapCb   *udxSap; 
   KwCb           *tKwCb;

   TRC3(KwDlUdxUbndReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
#endif

   tKwCb = KW_GET_KWCB(pst->dstInst);

   RLOG2(L_DEBUG,"Unbind Req for spId[%d], reason[%d]", 
                spId, reason);
   UNUSED(reason);
   /* disable upper sap (CKW) */
   udxSap = (tKwCb->u.dlCb->udxDlSap + spId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   KW_GET_AND_VALIDATE_UDXSAP(tKwCb,udxSap, EKW208, "KwUiDlUdxndReq");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   udxSap->state = KW_SAP_CFG;
   RETVALUE(ROK);
}


/**
 * @brief 
 *    Handler for configuring RLC entities.
 *
 * @details
 *    This function is used by RRC to configure(add/delete/modify)
 *    one or more RLC entities. 
 *        - CKW_CFG_ADD          => kwCfgAddRb
 *        - CKW_CFG_MODIFY       => kwCfgReCfgRb
 *        - CKW_CFG_DELETE       => kwCfgDelRb
 *        - CKW_CFG_REESTABLISH  => kwCfgReEstRb
 *        - CKW_CFG_DELETE_UE    => kwCfgDelUe
 *
 * @param[in] pst   -  Post structure  
 * @param[in] spId  -  Serive Provider ID 
 * @param[in] cfg   -  Configuration information for one or more RLC entities. 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwDlUdxCfgReq
(
Pst          *pst,
SpId         spId,
CkwCfgInfo   *cfg
)
#else
PUBLIC S16 KwDlUdxCfgReq(pst, spId, cfg)
Pst          *pst;
SpId         spId;
CkwCfgInfo   *cfg;
#endif
{
   CkwCfgCfmInfo   *cfgCfm; 
   U8              idx;    
   S16             ret = ROK;
   KwCb            *tKwCb;
   Pst             *pstUdxCfm;

   TRC3(KwDlUdxCfgReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
#endif

   tKwCb = KW_GET_KWCB(pst->dstInst);

   pstUdxCfm = &(tKwCb->u.dlCb->udxDlSap[spId].pst);
   KWDBGP_BRIEF(tKwCb,"spId(%d)\n", spId);

   /* Validate SAP ID under ERRORCLS */
   KW_VALDATE_SAP(tKwCb,spId, 
                  (&tKwCb->u.dlCb->udxDlSap[spId]), 
                  ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      /* only RLC UL will free it KW_PST_FREE(pst->region, pst->pool, cfg, sizeof(CkwCfgInfo)); */
      RETVALUE(RFAILED);
   }

   /* Allocate memory and memset to 0 for cfmInfo */
   KW_ALLOC_SHRABL_BUF_WC(pstUdxCfm->region,
                          pstUdxCfm->pool,
                          cfgCfm,
                          sizeof(CkwCfgCfmInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (cfgCfm == NULLP)
   {
      RLOG0(L_FATAL,"Memory Allocation Failed.");
      /* kw002.201 Freeing from proper region */
      /* KW_PST_FREE(pst->region, pst->pool, cfg, sizeof(CkwCfgInfo)); */
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   /* For every entity configuration process by cfgType */
   for (idx = 0; idx < cfg->numEnt; idx++)
   {
      CkwEntCfgCfmInfo   *entCfgCfm;
      CkwEntCfgInfo      *entCfg;

      entCfg  = (CkwEntCfgInfo *)&(cfg->entCfg[idx]);
      entCfgCfm   = (CkwEntCfgCfmInfo *)&(cfgCfm->entCfgCfm[idx]);

      switch (entCfg->cfgType)
      {
         case CKW_CFG_ADD:
            {
               if (entCfg->dir & KW_DIR_DL)
               { 
                  /* Add a new RB entity configuration */
                  if (kwCfgAddDlRb(tKwCb,cfg->ueId, cfg->cellId,\
                              entCfg, entCfgCfm) != ROK)
                  {
                     RLOG_ARG1(L_ERROR,DBG_RBID,cfg->entCfg[idx].rbId,"Addition Failed due to[%d]",
                           entCfgCfm->status.reason);
                  }
               }
               break;
            }
         case CKW_CFG_MODIFY:
            {
               if (entCfg->dir & KW_DIR_DL)
               {
                  /* Re-configure the existing RB entity configuration */
                  if (kwCfgReCfgDlRb(tKwCb,cfg->ueId, cfg->cellId,\
                           entCfg, entCfgCfm) != ROK)
                  {
                     RLOG_ARG1(L_ERROR,DBG_RBID,cfg->entCfg[idx].rbId,"ReCfg Failed due to[%d]",
                           entCfgCfm->status.reason);
                  }
               }
               break;
            }

         case CKW_CFG_DELETE:
            {
               if (entCfg->dir & KW_DIR_DL)
               {
                  /* Delete the existing RB entity configuration */
                  if (kwCfgDelDlRb(tKwCb,cfg->ueId, cfg->cellId,\
                        entCfg, entCfgCfm) != ROK)
                  {
                     RLOG_ARG1(L_ERROR,DBG_RBID,cfg->entCfg[idx].rbId,"Deletion Failed due to[%d]",
                           entCfgCfm->status.reason);
                  } 
               }
               break;
            }

         case CKW_CFG_REESTABLISH:
            {
               if (entCfg->dir & KW_DIR_DL)
               {
                  /*if direction is both then, re-establishment end indication
                   * should be sent only from the UL instance, only if DIR is
                   * DL only then DL instance will send indication.*/
                  Bool sndReEst = TRUE;
                  if (entCfg->dir & KW_DIR_UL)
                  {
                     sndReEst = FALSE;
                  }
                  /* Re-establish the existing RB entity configuration */
                  if (kwCfgReEstDlRb(tKwCb,cfg->ueId, cfg->cellId,
                                     sndReEst,entCfg, entCfgCfm) != ROK)
                  {
                     RLOG_ARG1(L_ERROR,DBG_RBID,cfg->entCfg[idx].rbId,"Reest Failed due to[%d]",
                           entCfgCfm->status.reason);
                  }
               }
               break;
            }

         case CKW_CFG_DELETE_UE:
            {
               /* Delete all RB entity configuration under UE */
               if (kwCfgDelDlUe(tKwCb,cfg->ueId, cfg->cellId,
                                entCfg, entCfgCfm) != ROK)
               {
                  RLOG_ARG1(L_ERROR,DBG_UEID,cfg->ueId,"deletion Failed due to[%d]",
                           entCfgCfm->status.reason);
               }
               break;
            }
         case CKW_CFG_DELETE_CELL:
            {
               if (kwCfgDelDlCell(tKwCb,cfg->cellId,entCfg,entCfgCfm) 
                                                                != ROK )
               {
                  RLOG_ARG1(L_ERROR,DBG_CELLID,cfg->cellId,"deletion Failed due to[%d]",
                           entCfgCfm->status.reason);
               } 
               break;
            }

         default:
            {
               KW_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType,\
                                   CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_CFG);
               RLOG0(L_ERROR, "Invalid CfgType");
            }
      }
   }

   /* Assign number of entity configuraitons and suId */
   cfgCfm->transId = cfg->transId;
   cfgCfm->ueId = cfg->ueId;
   cfgCfm->cellId = cfg->cellId;
   cfgCfm->numEnt = cfg->numEnt;

   /* kw002.201 Freeing from proper region */
   /* KW_PST_FREE(pst->region, pst->pool, cfg, sizeof(CkwCfgInfo)); */
   /* Send Configuration confirm primitive */
   KwDlUdxCfgCfm(&(tKwCb->u.dlCb->udxDlSap[spId].pst),
                 tKwCb->u.dlCb->udxDlSap[spId].suId, 
                 cfgCfm);

   RETVALUE(ROK);
} 

/**
 *@brief 
 *   This primitive is used by RRC to change the UeId for the existing UE
 *   context.
 *
 * @param pst     -  Pointer to the pst structure
 * @param spId    -  The ID of the service provider SAP in the RLC layer 
 * @param transId -  Transaction ID. This field uniquily identifies
 *                   transaction between RRC and RLC
 * @param ueInfo    -  Old UE Id Info for which the change request has come 
 * @param newUeInfo -  New UE Id Info for existing UE context 
 * 
 * @return 
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwDlUdxUeIdChgReq
(
Pst         *pst, 
SpId        spId, 
U32         transId, 
CkwUeInfo   *ueInfo,
CkwUeInfo   *newUeInfo
)
#else
PUBLIC S16 KwDlUdxUeIdChgReq(pst,spId,transId,ueInfo,newUeInfo)
Pst         *pst;
SpId        spId;
U32         transId;
CkwUeInfo   *ueInfo;
CkwUeInfo   *newUeInfo;
#endif
{
   CmStatus       status;
   KwCb           *tKwCb;

   TRC3(KwDlUdxUeIdChgReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
#endif

   tKwCb = KW_GET_KWCB(pst->dstInst);
#ifndef ALIGN_64BIT
   KWDBGP_BRIEF(tKwCb, "(spId(%d), transId(%ld))\n", 
                spId, transId);
#else
   KWDBGP_BRIEF(tKwCb, "(spId(%d), transId(%d))\n", 
                spId, transId);
#endif

   status.reason = CKW_CFG_REAS_NONE;
   status.status = CKW_CFG_CFM_OK;
   
   if (kwCfgDlUeIdChng(tKwCb, ueInfo, newUeInfo, &status) != ROK)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,newUeInfo->cellId,"Failure due to[%d]",
             status.reason);
   }
   KwDlUdxUeIdChgCfm(&(tKwCb->u.dlCb->udxDlSap[spId].pst),
                     tKwCb->u.dlCb->udxDlSap[spId].suId, 
                     transId, 
                     status);

   RETVALUE(ROK);
} 

/**
* @brief 
*    Request for status PDU from  ULM to DLM.
*
* @param[in]   pst   -  Post Structure
* @param[in]   spId  -  Service Provider Id
* @param[in]   rlcId -  Rlc Information Id
* @param[in]   pStaPdu - Status PDU 
*  
* @return   S16
*    -# ROK
*    -# RFAILED
**/
#ifdef ANSI
PUBLIC S16  KwDlUdxStaPduReq
(
Pst             *pst,
SpId            spId,
CmLteRlcId      *rlcId,
KwUdxDlStaPdu   *pStaPdu
)
#else
PUBLIC S16  KwDlUdxStaPduReq(pst, spId, rlcId, pStaPdu)
Pst             *pst;
SpId            spId;
CmLteRlcId      *rlcId;
KwUdxDlStaPdu   *pStaPdu;
#endif
{
   KwDlRbCb   *rbCb;
   KwCb       *tKwCb;

   tKwCb =  KW_GET_KWCB (pst->dstInst);

   kwDbmFetchDlRbCbByRbId(tKwCb, rlcId, &rbCb); /* Fetch DBM RbCb */
   if (!rbCb)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,rlcId->ueId, "CellId [%u]:RbId[%d] not found",
            rlcId->cellId,rlcId->rbId);
      KW_FREE_SHRABL_BUF(pst->region, 
			 pst->pool, 
			 pStaPdu, 
			 sizeof(KwUdxDlStaPdu));
      RETVALUE(RFAILED);
   }

   AMDL.cntrlBo = pStaPdu->controlBo;
   /* If there already exists a STAUS PDU, free it and take the new one
      into account */
   if(AMDL.pStaPdu)
   {
      KW_FREE_SHRABL_BUF(pst->region, 
			 pst->pool, 
			 AMDL.pStaPdu, 
			 sizeof(KwUdxDlStaPdu));
   }
   
   AMDL.pStaPdu = pStaPdu;
#if 0
   RLOG1(L_ERROR,"##### Status PDU BO : %d #####",AMDL.cntrlBo);         
#endif
   kwAmmSendDStaRsp(tKwCb, rbCb, &AMDL);             

   RETVALUE (ROK);
}

/**
* @brief 
*    It handles the status update recieved from ULM.
*
* @param[in]   pst   -  Post Structure
* @param[in]   spId  -  Service Provider Id
* @param[in]   rlcId -  Rlc Information Id
* @param[in]   pStaPdu - Status PDU 
*  
* @return   S16
*    -# ROK
*    -# RFAILED
**/
#ifdef ANSI
PUBLIC S16  KwDlUdxStaUpdReq
(
Pst*          pst,
SpId          spId,
CmLteRlcId    *rlcId,
KwUdxStaPdu   *pStaPdu
)
#else
PUBLIC S16 KwDlUdxStaUpdReq(pst, spId, rlcId,pStaPdu)
Pst*          pst;
SpId          spId;
CmLteRlcId    *rlcId;
KwUdxStaPdu   *pStaPdu;
#endif
{
   KwCb          *tKwCb;
   KwDlRbCb      *rbCb;

   tKwCb = KW_GET_KWCB(pst->dstInst);

   kwDbmFetchDlRbCbByRbId(tKwCb, rlcId, &rbCb);
   if (!rbCb)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,rlcId->ueId, "CellId [%u]:RbId[%d] not found",
            rlcId->cellId,rlcId->rbId);
      RETVALUE(RFAILED);
   }

   kwAmmDlHndlStatusPdu(tKwCb, rbCb, pStaPdu);

   KW_FREE_SHRABL_BUF(pst->region,
		      pst->pool, 
		      pStaPdu, 
		      sizeof(KwUdxStaPdu));

   RETVALUE(ROK);
}

#ifdef LTE_L2_MEAS
/**
*/
#ifdef ANSI
PUBLIC S16 KwDlUdxL2MeasReq 
(
Pst            *pst, 
KwL2MeasReqEvt *measReqEvt 
)
#else
PUBLIC S16 KwDlUdxL2MeasReq (pst, measReqEvt)
Pst            *pst; 
KwL2MeasReqEvt *measReqEvt;
#endif
{
   U32            cntr;
   U8             measType;
#if 0 /* L2 MEAS AGHOSH */
   KwL2MeasCfmEvt measCfmEvt;

   KwDlUeCb         *ueCb = NULL;
   U16             rbIdx = 0;
   U16             qciIdx = 0;
#endif
   VOLATILE U32     startTime = 0;
   KwCb     *tKwCb;

   TRC3(KwDlUdxL2MeasReq);

   /*starting Task*/
   SStartTask(&startTime, PID_RLC_MEAS_START);

   tKwCb =  KW_GET_KWCB(pst->dstInst);

   /* Initialize measCfmEvt */

  /* validate the received measReqEvt */
 /*LTE_L2_MEAS_PHASE2*/

   measType = measReqEvt->measReq.measType;


#if 0
   /* Check if already an event for the requested Meas type already exists */
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      if (tKwCb->u.dlCb->kwL2Cb.kwL2EvtCb[cntr].measCb.measType & measType)
      {
         measEvtCb = &(tKwCb->u.dlCb->kwL2Cb.kwL2EvtCb[cntr]);
         cmMemset((U8 *) measEvtCb, 0, sizeof(KwL2MeasEvtCb));
         break;
      }        
   }

   /* If no Event CB of the requested type already exists, create a new one */
   if (cntr == LKW_MAX_L2MEAS)
   {
      /* Allocate and intialise the Event Control Block */
         /* measCfmEvt.numCfm  = 1;*/

      for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
      {
         if(tKwCb->u.dlCb->kwL2Cb.kwL2EvtCb[cntr].measCb.measType == LKW_L2MEAS_NONE)
         {
            /* TODO : Need to see for multiple Measurements here */
            tKwCb->u.dlCb->kwL2Cb.kwL2EvtCb[cntr].cbIdx = cntr;
            tKwCb->u.dlCb->kwL2Cb.kwL2EvtCb[cntr].measCb.measType = measType;
            tKwCb->u.dlCb->kwL2Cb.kwNumMeas++;
            break;
         }
      }
   }
   
#endif

   if(measType & LKW_L2MEAS_DL_IP) 
   {
      /* if measurement is for DL IP enable for all QCI */
      for(cntr = 0; cntr < LKW_MAX_QCI; cntr++)
      {
         tKwCb->u.dlCb->kwL2Cb.measOn[cntr] |= LKW_L2MEAS_DL_IP;
      }
   }
   else
   {
      /* for nonIpThroughput meas, enable only for the sent QCIs */
      U32 i;
#if 0
      for(i = 0; i < measReqEvt->measReq.val.nonIpThMeas.numQci; i++)
      {
         U8 qciValue = measReqEvt->measReq.val.nonIpThMeas.qci[i];
         tKwCb->u.dlCb->kwL2Cb.measOn[qciValue] |= measType;
      }
#else
      for(i = 0; i < LKW_MAX_QCI; i++)
      {
         tKwCb->u.dlCb->kwL2Cb.measOn[i] |= measType;
      }
#endif
   }

   /* We need to copy the transId for sending back confirms later */
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      KwL2MeasEvtCb* measEvtCb = &(tKwCb->u.dlCb->kwL2Cb.kwL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         measEvtCb->transId= measReqEvt->transId;
      }
   }
   KW_FREE_SHRABL_BUF(pst->region, 
		      pst->pool, 
		      measReqEvt, 
		      sizeof(KwL2MeasReqEvt));

   /*stopping Task*/
   SStopTask(startTime, PID_RLC_MEAS_START);
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
PUBLIC S16 KwDlUdxL2MeasStopReq
(
Pst            *pst,
U8             measType
)
#else
PUBLIC S16 KwDlUdxL2MeasStopReq (pst, measType)
Pst            *pst;
U8             measType;
#endif
{
  /* S16 ret = ROK;*/
   KwL2MeasEvtCb *measEvtCb = NULLP;
   U16            cntr;
   U8             status = ROK;
/*   KwL2MeasCfmEvt          measCfmEvt;  */
   VOLATILE U32     startTime = 0;
   KwCb     *tKwCb=NULLP;
   TRC3(KwDlUdxMeasStopReq);
   
   /*starting Task*/
   SStartTask(&startTime, PID_RLC_MEAS_STOP);

   tKwCb =  KW_GET_KWCB(pst->dstInst);
/*   cmMemset((U8*)&measCfmEvt, 0, sizeof(KwL2MeasCfmEvt)); */
   /* reset the counters for the measurement type passed */
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(tKwCb->u.dlCb->kwL2Cb.kwL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         kwUtlResetDlL2MeasInKwRb(tKwCb, &measEvtCb->measCb, measType);

#if 0
         kwCb.kwL2Cb.kwL2EvtCb[measEvtCb->cbIdx] = NULLP;
         kwCb.kwL2Cb.kwNumMeas--;
         KW_FREE(measEvtCb, sizeof(KwL2MeasEvtCb));
#endif
      }
   }

   /* switch off the measurements for the type passed */
   for(cntr = 0; cntr < LKW_MAX_QCI; cntr++)
   {
      tKwCb->u.dlCb->kwL2Cb.measOn[cntr] &= ~measType;
   }
   
   status = LCM_PRIM_OK;
   /* Stop confirm is removed as UL thread is already sending it */
   
   /*stopping Task*/
   SStopTask(startTime, PID_RLC_MEAS_STOP);

   RETVALUE(ROK);
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
PUBLIC S16 KwDlUdxL2MeasSendReq
(
Pst            *pst,
U8             measType
)
#else
PUBLIC S16 KwDlUdxL2MeasSendReq (pst, measType)
Pst            *pst;
U8             measType;
#endif
{
   KwL2MeasEvtCb *measEvtCb;
   U16            cntr;
   
   VOLATILE U32     startTime = 0;
   KwCb     *tKwCb;
   TRC3(KwDlUdxMeasSendReq);

   tKwCb =  KW_GET_KWCB(pst->dstInst);
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      measEvtCb = &(tKwCb->u.dlCb->kwL2Cb.kwL2EvtCb[cntr]);
      if(measEvtCb->measCb.measType & measType)
      {
         /*starting Task*/
         SStartTask(&startTime, PID_RLC_MEAS_REPORT);

         kwUtlSndDlL2MeasCfm(tKwCb, measEvtCb);

         /*stopping Task*/
         SStopTask(startTime, PID_RLC_MEAS_REPORT);
      }
   }

   RETVALUE(ROK);
}
#endif /* LTE_L2_MEAS */

#ifdef __cplusplus
}
#endif /* __cplusplus */


/**********************************************************************
         End of file:     kw_udx_dl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:08 2015
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       mraj   1. LTE RLC Initial Release
*********************************************************************91*/
