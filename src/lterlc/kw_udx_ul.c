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
 
    File:   kw_udx_ul.c
 
    Sid:      kw_udx_ul.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:29 2014
 
    Prg:   mraj
 
**********************************************************************/
static const char* RLOG_MODULE_NAME="UDX";
static int RLOG_MODULE_ID=262144;
static int RLOG_FILE_ID=204;

/** @file kw_udx_ul.c
@brief  UDX Uplink Module 
*/
	 

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
#include "kw_ul.h"
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
#include "kw_ul.x"


#define KW_MODULE KW_DBGMASK_UDX
/* local defines */

/* local externs */

/* forward references */

/* public variable declarations */
EXTERN S16 kwHdlCkwUlCfgReq ARGS((KwCb  *gCb,KwUlCfgTmpData *cfgTmpData,
                                  CkwCfgCfmInfo *cfmInfo, CkwCfgCfmInfo *cfgCfm));

/**
  * @brief
  * UDX APIs
  */
/**
 * @brief 
 *    Handler for bind confirmation from DL.
 *
 * @details
 *    This function handles the bind confirmation received
 *    from DL. 
 *
 * @param[in] pst     Post structure  
 * @param[in] suId    Service User ID
 * @param[in] status  Status of confirmation 
 * 
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */

#ifdef ANSI
PUBLIC S16 KwUlUdxBndCfm
(
Pst    *pst,  
SuId   suId, 
U8     status 
)
#else
PUBLIC S16 KwUlUdxBndCfm (pst, suId, status)
Pst    *pst; 
SuId   suId;   
U8     status; 
#endif
{
   U16            event;        
   U16            cause;       
   KwUdxUlSapCb   *udxSap;   /* RGU SAP Control Block */
   KwCb           *tKwCb;

   TRC3(KwUlUdxBndCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tKwCb = KW_GET_KWCB(pst->dstInst);

   KWDBGP_BRIEF(tKwCb, "KwUlUdxBndCfm(post, suId(%d), status(%d)\n", 
                suId, status);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (tKwCb->init.cfgDone != TRUE)
   {
      RLOG0(L_FATAL, "General configuration not done");
      KW_SEND_SAPID_ALARM(tKwCb,suId, 
                          LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_STATE);

      RETVALUE(RFAILED);
   }

   if (suId < 0)
   {
      RLOG0(L_ERROR, "Invalid suId");
      KW_SEND_SAPID_ALARM(tKwCb,suId, 
                           LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SUID);

      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   udxSap = tKwCb->u.ulCb->udxUlSap + suId;

   KWDBGP_DETAIL(tKwCb, "KwLiRguBndCfm: For RGU SAP state=%d\n", 
                 udxSap->state);

   /* Check rguSap state */
   switch (udxSap->state)
   {
      case KW_SAP_BINDING:
      {
         if(TRUE == kwChkTmr(tKwCb,(PTR)udxSap,KW_EVT_WAIT_BNDCFM))
         {
             kwStopTmr (tKwCb,(PTR)udxSap, KW_EVT_WAIT_BNDCFM);
         }
         udxSap->retryCnt = 0;
          
         if (status == CM_BND_OK)
         {
            udxSap->state = KW_SAP_BND;
            event = LCM_EVENT_BND_OK;
            cause = LKW_CAUSE_SAP_BNDENB;
         }
         else
         {
            udxSap->state = KW_SAP_CFG;
            event = LCM_EVENT_BND_FAIL;
            cause = LKW_CAUSE_UNKNOWN;
         }
      }
      break;

     default:
        event = LKW_EVENT_RGU_BND_CFM;
        cause = LCM_CAUSE_INV_STATE;
        break;
   }
   /* Send an alarm with proper event and cause */
   KW_SEND_SAPID_ALARM(tKwCb,suId, event, cause);

   RETVALUE(ROK);
} 

/**
 * @brief 
 *    Handles UDX Configuration Confirm 
 *
 * @details
 *    This function handles the UDX configuration Confirm from DL Instance
 *
 * @param[in] pst     Post structure  
 * @param[in] suId    Service User ID
 * @param[in] cfmInfo Confirm Information 
 * 
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUlUdxCfgCfm
(
Pst             *pst,  
SuId            suId, 
CkwCfgCfmInfo   *cfmInfo  
)
#else
PUBLIC S16 KwUlUdxCfgCfm (pst, suId, cfmInfo)
Pst             *pst;   
SuId            suId;  
CkwCfgCfmInfo   *cfmInfo;  
#endif
{
   KwCb             *tKwCb;
   CkwCfgCfmInfo    *cfgCfm;
   KwUlCfgTmpData   *cfgTmpData;

   TRC3(KwUlUdxCfgCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      KW_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(CkwCfgCfmInfo));
      RETVALUE (RFAILED);
   }
#endif
   tKwCb = KW_GET_KWCB(pst->dstInst);
   KWDBGP_BRIEF(tKwCb, " suId(%d)\n", suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      RLOG0(L_ERROR, "Invalid suId");
      KW_SEND_SAPID_ALARM(tKwCb,suId, 
                           LKW_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SUID);
      KW_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(CkwCfgCfmInfo));
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   if(ROK != kwDbmFindUlTransaction(tKwCb,cfmInfo->transId, &cfgTmpData))
   {
      RLOG0(L_ERROR, "Invalid transId");
      KW_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(CkwCfgCfmInfo));
      RETVALUE (RFAILED);
   }

   if(ROK != kwDbmDelUlTransaction(tKwCb, cfgTmpData))
   {
      KW_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(CkwCfgCfmInfo));
       RETVALUE(RFAILED);
   }
      /* Allocate memory and memset to 0 for cfmInfo */
   KW_ALLOC(tKwCb,cfgCfm, sizeof(CkwCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (cfgCfm == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      KW_FREE_SHRABL_BUF(pst->region,
                         pst->pool,
	                 cfmInfo,
                         sizeof(CkwCfgCfmInfo));
       RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   kwHdlCkwUlCfgReq(tKwCb,cfgTmpData, cfmInfo, cfgCfm);
   KwUiCkwCfgCfm(&(tKwCb->u.ulCb->ckwSap.pst), 
                 tKwCb->u.ulCb->ckwSap.suId , cfgCfm);

   /* free the memory from DL */
   KW_FREE_SHRABL_BUF(pst->region,
                      pst->pool,
		      cfmInfo,
		      sizeof(CkwCfgCfmInfo));

   /* free the cfgInfo that came from LM */
   KW_PST_FREE(pst->region, pst->pool, cfgTmpData->cfgInfo, sizeof(CkwCfgInfo));
   KW_FREE(tKwCb,cfgTmpData,sizeof(KwUlCfgTmpData));
   
   RETVALUE(ROK);
} 

/**
 * @brief 
 *    Handler for UeId change confirm 
 *
 * @details
 *    This function handles the UeId Change Confirm from DL Instance 
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service User ID
 *  @param[in] transId Transaction Id 
 *  @param[in] status  Status of confirmation 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */

#ifdef ANSI
PUBLIC S16 KwUlUdxUeIdChgCfm
(
Pst        *pst,          
SuId       suId,           
U32        transId,
CmStatus   status
)
#else
PUBLIC S16 KwUlUdxUeIdChgCfm (pst, suId, cfmInfo)
Pst        *pst;         
SuId       suId;        
U32        transId;
CmStatus   status;
#endif
{
   KwCb             *tKwCb;
   KwUlCfgTmpData   *cfgTmpData;

   TRC3(KwUlUdxUeIdChgCfm)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tKwCb = KW_GET_KWCB(pst->dstInst);

   KWDBGP_BRIEF(tKwCb, " suId(%d) \n", suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (suId < 0)
   {
      RLOG0(L_ERROR, "Invalid suId");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   if(ROK != kwDbmFindUlTransaction(tKwCb, transId, &cfgTmpData))
   {
      RLOG0(L_ERROR, "Invalid transId");
      RETVALUE (RFAILED);
   }

   if(ROK != kwDbmDelUlTransaction(tKwCb, cfgTmpData))
   {
      RETVALUE(RFAILED);
   }

   if(status.status == CKW_CFG_CFM_OK)
   {
       if(cfgTmpData->ueCb != NULLP) 
       {
      kwCfgApplyUlUeIdChng(tKwCb, cfgTmpData->ueInfo, cfgTmpData->newUeInfo, cfgTmpData);
   }
   }
   KwUiCkwUeIdChgCfm(&(tKwCb->u.ulCb->ckwSap.pst),
                     tKwCb->u.ulCb->ckwSap.suId,
                     transId,cfgTmpData->ueInfo,status);
   /* only newUeInfo needs to be freed here, ueInfo would be freed at the 
      interface or by he receipient in case of tight coupling */
   KW_PST_FREE(pst->region, pst->pool, cfgTmpData->newUeInfo, sizeof(CkwUeInfo));
   KW_FREE_WC(tKwCb, cfgTmpData, sizeof (KwUlCfgTmpData));
   RETVALUE(ROK);
} 

/**
 * @brief 
 *    Udx Status Prohibit Timer Start
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service User ID
 *  @param[in] rlcId   rlc Id 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
PUBLIC S16  KwUlUdxStaProhTmrStart
(
Pst*         pst,
SuId         suId,
CmLteRlcId   *rlcId
)
{
   KwCb       *tKwCb= NULLP;
   KwUlRbCb   *rbCb;        

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE (RFAILED);
   }
#endif
   tKwCb = KW_GET_KWCB(pst->dstInst);

   kwDbmFetchUlRbCbByRbId(tKwCb, rlcId, &rbCb);
   if (rbCb == NULLP)
   {    
      RLOG_ARG2(L_ERROR, DBG_UEID,rlcId->ueId, "CellId [%u]:RbId[%d] not found",
            rlcId->cellId,rlcId->rbId);
      RETVALUE(RFAILED);
   }

   /* Start staProhTmr */
   kwStartTmr(tKwCb,(PTR)rbCb, KW_EVT_AMUL_STA_PROH_TMR);

   RETVALUE (ROK);
} 

/**
 * @brief 
 *    Handler for configuration confirm from DL.
 *
 * @param[in] gCb        - RLC Instance Control Block
 * @param[in] cfgTmpData - Configuration Temporary Data 
 * @param[in] cfmInfo    - DL Configuration Confirm Info
 * @param[out] cfgCfm    - Configuration Confirm to be sent to RRC
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 kwHdlCkwUlCfgReq
(
KwCb             *gCb,
KwUlCfgTmpData   *cfgTmpData,
CkwCfgCfmInfo    *cfmInfo,
CkwCfgCfmInfo    *cfgCfm
)
#else
PUBLIC S16 kwHdlCkwUlCfgReq(gCb,cfgTmpData,cfmInfo,cfmInfo)
KwCb             *gCb;
KwUlCfgTmpData   *cfgTmpData;
CkwCfgCfmInfo    *cfmInfo;
CkwCfgCfmInfo    *cfgCfm;
#endif
{
   CkwCfgInfo   *cfg;
   U32          idx;
   U32          maxEnt;
   
   cfg = cfgTmpData->cfgInfo;
   maxEnt = (cfg->numEnt < CKW_MAX_ENT_CFG)? cfg->numEnt:CKW_MAX_ENT_CFG;

   for (idx = 0; idx < maxEnt; idx++)
   {
      CkwEntCfgCfmInfo   *entCfgCfm;
      CkwEntCfgCfmInfo   *entDlCfgCfm;
      CkwEntCfgInfo      *entCfg;

      entCfg  = &(cfg->entCfg[idx]);
      entCfgCfm   = &(cfgCfm->entCfgCfm[idx]);
      entDlCfgCfm = &(cfmInfo->entCfgCfm[idx]);
      switch (entCfg->cfgType)
      {
         case CKW_CFG_ADD:
         case CKW_CFG_MODIFY:
         case CKW_CFG_DELETE:
            {
               if (entCfg->dir == KW_DIR_UL)
               {
                  KW_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(CkwEntCfgCfmInfo)); 
               }
               else if (entCfg->dir == KW_DIR_DL)
               {
                  KW_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(CkwEntCfgCfmInfo)); 
               }
               else if(entCfg->dir == KW_DIR_BOTH)
               {
                  if (entDlCfgCfm->status.status != CKW_CFG_CFM_OK)
                  {
                     kwCfgRollBackUlRb(gCb,
                                       cfg->ueId,
                                       &cfg->entCfg[idx], 
                                       &cfgTmpData->cfgEntData[idx]);
                  }
                  else
                  {
                     kwCfgApplyUlRb(gCb,  
                                    &cfg->entCfg[idx], 
                                    &cfgTmpData->cfgEntData[idx],
                                    cfgTmpData);
                  }
                  KW_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(CkwEntCfgCfmInfo)); 
               }
               else
               {
		            KW_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType,
				           CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_DIR);
                  RLOG_ARG2(L_ERROR,DBG_UEID,cfg->ueId,"RBID[%d] direction[%d] is invalid",
                         entCfg->rbId,entCfg->dir);
               }
               break;
            }

         case CKW_CFG_REESTABLISH:
            {
               if (entCfg->dir == KW_DIR_UL)
               {
                  KW_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(CkwEntCfgCfmInfo)); 
               }
               else if (entCfg->dir == KW_DIR_DL)
               {
                  KW_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(CkwEntCfgCfmInfo)); 
               }
               else
               {
                  if (entCfg->dir & KW_DIR_UL)
                  {
                     /* Reestablish indication is sent from UL Instance only*/
                     if (entDlCfgCfm->status.status == CKW_CFG_CFM_OK)
                     {
                        kwCfgApplyReEstUlRb (gCb, cfg->ueId, 
                                             cfg->cellId, TRUE, 
                                             &cfgTmpData->cfgEntData[idx]);
                     }
                     KW_MEM_CPY(entCfgCfm, entDlCfgCfm, sizeof(CkwEntCfgCfmInfo)); 
                  }
               }
               break;
            }
         case CKW_CFG_DELETE_UE:
            {
               if (cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status == CKW_CFG_CFM_OK)
               {
                  kwCfgApplyDelUlUe(gCb, cfgTmpData);
                  KW_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(CkwEntCfgCfmInfo)); 
               }
               else
               {
                   KW_MEM_CPY(entCfgCfm, entDlCfgCfm,
                              sizeof(CkwEntCfgCfmInfo)); 
               }
               break;
            }
         case CKW_CFG_DELETE_CELL:
            {
               if (cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status == CKW_CFG_CFM_OK)
               {
                  kwCfgApplyDelUlCell(gCb, cfgTmpData);
                  KW_MEM_CPY(entCfgCfm, 
                             &cfgTmpData->cfgEntData[idx].entUlCfgCfm, 
                             sizeof(CkwEntCfgCfmInfo)); 
               }
               else
               {
                   KW_MEM_CPY(entCfgCfm, entDlCfgCfm,
                              sizeof(CkwEntCfgCfmInfo)); 
               }
               break;
            }
         default:
            {
               KW_CFG_FILL_CFG_CFM(entCfgCfm, entCfg->rbId, entCfg->rbType,
                                   CKW_CFG_CFM_NOK, CKW_CFG_REAS_INVALID_CFG);

               RLOG0(L_ERROR,"Invalid configuration type");
            }
      }
   }

   /* Assign number of entity configurations and suId */
   cfgCfm->transId = cfgTmpData->uprLyrTransId;
   cfgCfm->ueId = cfg->ueId;
   cfgCfm->cellId = cfg->cellId;
   cfgCfm->numEnt = cfg->numEnt;

   RETVALUE(ROK);
}
#ifdef __cplusplus
}
#endif /* __cplusplus */


/**********************************************************************
         End of file:     kw_udx_ul.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:29 2014
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       mraj   1. LTE RLC Initial Release
*********************************************************************91*/
