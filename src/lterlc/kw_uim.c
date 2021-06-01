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
  
        Name:    LTE-RLC Layer - Upper Interface Functions
    
        Type:    C file
  
     Desc:     Source code for RLC Upper Interface Module
               This file contains following functions
      
        --KwUiCkwBndReq
        --KwUiCkwUbndReq
        --KwUiCkwCfgReq

        --KwUiKwuBndReq
        --KwUiKwuUbndReq
        --KwUiKwuDatReq
        --KwUiKwuDiscSduReq 

     File:     kw_uim.c

     Sid:      gp_uim.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:19 2014

     Prg:      gk 

**********************************************************************/
static const char* RLOG_MODULE_NAME="UIM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=205;

/** 
 * @file gp_uim.c
 * @brief RLC Upper Interface Module
*/

#define KW_MODULE KW_DBGMASK_INF

/*
 *      This software may be combined with the following TRILLIUM
 *      software:
 *
 *      part no.                      description
 *      --------    ----------------------------------------------
 *      1000241     LTE-MAC
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
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
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
#include "kw_err.h"
#include "kw.x"
#include "kw_udx.x"
#include "kw_dl.x"
#include "kw_ul.x"


/*****************************************************************************
 *                          CKW INTERFACE
 ****************************************************************************/

/**
 * @brief 
 *    Handler for binding the RLC upper layer service user with 
 *    the RLC layer.
 *
 * @details
 *    This function is used by RLC user to request for binding to 
 *    RLC. This function is called by the CKW interface to bind 
 *    RLC's SAP (identified by spId) with the service user's
 *    SAP (identified by suId). 
 *
 * @param[in] pst     Post structure
 * @param[in] suId    Service User ID
 * @param[in] spId    Service provider ID
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 *
*/
#ifdef ANSI
PUBLIC S16 KwUiCkwBndReq 
(
Pst    *pst,
SuId   suId,
SpId   spId 
)
#else
PUBLIC S16 KwUiCkwBndReq (pst, suId, spId)
Pst    *pst; 
SuId   suId;
SpId   spId;
#endif
{
   KwCkwSapCb   *ckwSap;  
   KwCb         *tKwCb;

   TRC3(KwUiCkwBndReq);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
#endif

   tKwCb = KW_GET_KWCB(pst->dstInst);

   RLOG2(L_DEBUG, "spId(%d), suId(%d)", spId, suId);
   ckwSap = &(tKwCb->u.ulCb->ckwSap);
   /* Take action based on the current state of the SAP */
   switch(ckwSap->state)
   {
      /* SAP is configured but not bound */
      case KW_SAP_CFG:
      case KW_SAP_UBND:
      {
         /* copy bind configuration parameters in SSAP sap */
         ckwSap->suId = suId;
         ckwSap->pst.dstProcId = pst->srcProcId;
         ckwSap->pst.dstEnt = pst->srcEnt;
         ckwSap->pst.dstInst = pst->srcInst;

         /* Update the State */
         ckwSap->state = KW_SAP_BND;

         RLOG1(L_DEBUG, "KwUiCkwBndReq: state (%d)", ckwSap->state);
         break;
      }
      case KW_SAP_BND:
      {
         /* Sap is already bound check source, destination Entity and  
          * Proc Id  */
         if (ckwSap->pst.dstProcId != pst->srcProcId ||
             ckwSap->pst.dstEnt != pst->srcEnt ||
             ckwSap->pst.dstInst != pst->srcInst ||
             ckwSap->suId != suId)
         {
            KW_SEND_SAPID_ALARM(tKwCb,
                                spId, 
                                LKW_EVENT_CKW_BND_REQ, 
                                LCM_CAUSE_INV_PAR_VAL);

            RLOG0(L_ERROR, "CKW SAP already Bound");
            KwUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_NOK);
            RETVALUE(RFAILED);
         }
         break;
      }
      default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         RLOG0(L_ERROR, "Invalid CKW SAP State in Bind Req");
         KW_SEND_SAPID_ALARM(tKwCb,
                             spId, 
                             LKW_EVENT_CKW_BND_REQ, 
                             LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         KwUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_NOK);
         RETVALUE(RFAILED);
         break;
      }
   }

   KwUiCkwBndCfm(&(ckwSap->pst), ckwSap->suId, CM_BND_OK);
   RETVALUE(ROK);
} 


/**
 * @brief 
 *    Handler for unbinding the RLC upper layer service user CKW with 
 *    the RLC layer.
 *
 * @details
 *    This function is used by RLC user to request for unbinding  
 *    with RLC.This function is called by the CKW interface to 
 *    unbind with RLC.  
 *
 * @param[in] pst     Post structure
 * @param[in] spId    Service provider SAP ID
 * @param[in] reason  Reason for Unbinding 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
*/ 
#ifdef ANSI
PUBLIC S16 KwUiCkwUbndReq
(
Pst      *pst,   
SpId     spId,  
Reason   reason 
)
#else
PUBLIC S16 KwUiCkwUbndReq(pst, spId, reason)
Pst      *pst; 
SpId     spId; 
Reason   reason;
#endif
{
   KwCb *tKwCb;

   TRC3(KwUiCkwUbndReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   tKwCb = KW_GET_KWCB(pst->dstInst);

   RLOG2(L_DEBUG,"spId(%d), reason(%d)", 
                spId, 
                reason);

   UNUSED(reason);

#if (ERRCLASS & ERRCLS_INT_PAR)
   KW_GET_AND_VALIDATE_CKWSAP(tKwCb, 
                              (&(tKwCb->u.ulCb->ckwSap)), 
                              EKW208, 
                              "KwUiCkwUbndReq");
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   /* disable upper sap (CKW) */
   tKwCb->u.ulCb->ckwSap.state = KW_SAP_CFG;
   RETVALUE(ROK);
} 


/**
 * @brief  
 *    Handler for configuring RLC entities.
 *
 * @details
 *    This function is used by RRC to configure(add/delete/modify)
 *    one or more RLC entities. 
 *
 * @param[in] pst   -  Post structure  
 * @param[in] spId  -  Serive Provider ID 
 * @param[in] cfg   -  Configuration information for one or more RLC entities. 
 *
 * @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiCkwCfgReq
(
Pst          *pst,
SpId         spId,
CkwCfgInfo   *cfg
)
#else
PUBLIC S16 KwUiCkwCfgReq(pst, spId, cfg)
Pst          *pst;
SpId         spId;
CkwCfgInfo   *cfg;
#endif
{
   S16         ret = ROK;
   KwCb        *tKwCb;
   KwUlCfgTmpData   *cfgTmpData;

   static U32  transCount;


   TRC3(KwUiCkwCfgReq)
      
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      KW_PST_FREE(pst->region, pst->pool, cfg, sizeof(CkwCfgInfo));
      RETVALUE(RFAILED);
   }
#endif
   tKwCb = KW_GET_KWCB(pst->dstInst);

   RLOG1(L_DEBUG, "spId(%d)", spId);

   /* Validate SAP ID under ERRORCLS */
   KW_VALDATE_SAP(tKwCb, spId, (&(tKwCb->u.ulCb->ckwSap)), ret);
   if (ret != ROK)
   {
      KW_PST_FREE(pst->region, pst->pool, cfg, sizeof(CkwCfgInfo));
      RETVALUE(RFAILED);
   }

   KW_ALLOC(tKwCb, cfgTmpData, sizeof (KwUlCfgTmpData));

   if (cfgTmpData == NULLP)
   {
      KW_PST_FREE(pst->region, pst->pool, cfg, sizeof(CkwCfgInfo));
      RETVALUE(RFAILED);
   }


   cfgTmpData->uprLyrTransId = cfg->transId; /*Save User TransId*/
   cfgTmpData->transId = ++transCount;       /*Generate new TransId*/
   cfg->transId = cfgTmpData->transId;
   cfgTmpData->cfgInfo  = cfg;
 
   if (kwDbmAddUlTransaction(tKwCb, cfgTmpData) != ROK)
   {
      RLOG0(L_ERROR, "Addition to UL transId Lst Failed");
      KW_PST_FREE(pst->region, pst->pool, cfg, sizeof(CkwCfgInfo));
      
      RETVALUE(RFAILED);
   }
   
   kwHdlUiCkwUlCfgReq(tKwCb, cfgTmpData, cfg);
   
   KwUlUdxCfgReq(&(KW_GET_UDX_SAP(tKwCb)->pst),KW_GET_UDX_SAP(tKwCb)->spId,cfg); 

   RETVALUE(ROK);
} 

/**
 * @brief 
 *    Handler to change the UeId
 *
 * @details 
 *    This primitive is used by RRC to change the UeId for the existing UE
 *    context.
 *     
 * @param[in] pst     -  Point to the pst structure
 * @param[in] spId    -  The ID of the service provider SAP in the RLC layer 
 * @param[in] transId -  Transaction ID. This field uniquily identifies
 *                       transaction between RRC and RLC
 * @param[in] ueInfo    -  Old UE Id Info for which the change request has come 
 * @param[in] newUeInfo -  New UE Id Info for existing UE context 
 * 
 * @return S16
 *    -# ROK
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiCkwUeIdChgReq
(
Pst         *pst, 
SpId        spId, 
U32         transId, 
CkwUeInfo   *ueInfo,
CkwUeInfo   *newUeInfo
)
#else
PUBLIC S16 KwUiCkwUeIdChgReq(pst, spId, transId, ueInfo, newUeInfo)
Pst         *pst;
SpId        spId;
U32         transId;
CkwUeInfo   *ueInfo;
CkwUeInfo   *newUeInfo;
#endif
{
   S16              ret = ROK;
   KwCb             *tKwCb;
   KwUlCfgTmpData   *cfgTmpData = NULLP;

   TRC3(KwUiCkwUeIdChgReq)

   do
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
      {
         ret = RFAILED;
         break;
      }
#endif

      tKwCb = KW_GET_KWCB(pst->dstInst);
#ifndef ALIGN_64BIT
      RLOG_ARG2(L_DEBUG,DBG_CELLID,newUeInfo->cellId,
                   "KwUiCkwUeIdChgReq(pst, spId(%d), transId(%ld))", 
                   spId, 
                   transId);
#else
      RLOG_ARG2(L_DEBUG,DBG_CELLID,newUeInfo->cellId, 
                   "KwUiCkwUeIdChgReq(pst, spId(%d), transId(%d))\n", 
                   spId, 
                   transId);
#endif

      KW_ALLOC(tKwCb, cfgTmpData, sizeof (KwUlCfgTmpData));
      if (!cfgTmpData)
      {
         ret = RFAILED;
         break;
      }

      cfgTmpData->transId = transId;
      cfgTmpData->ueInfo  = ueInfo;
      cfgTmpData->newUeInfo  = newUeInfo; 

      if (kwDbmAddUlTransaction(tKwCb, cfgTmpData))
      {
         RLOG0(L_ERROR, "Addition to UL transId Lst Failed");
         ret = RFAILED;
         break;
      }
   }while(0);

   if(ret)
   {
      /* there was an error in the processing, free up all the memory
       * that was passed and could have been allocated in this function
       */
      /* Freeing from proper region */
      KW_PST_FREE(pst->region, pst->pool, newUeInfo, sizeof(CkwUeInfo));
      KW_PST_FREE(pst->region, pst->pool, ueInfo, sizeof(CkwUeInfo));

      if(cfgTmpData)
      {
         KW_FREE(tKwCb, cfgTmpData, sizeof (KwUlCfgTmpData));
      }
      RETVALUE(RFAILED);
   }

   if(ROK != kwCfgValidateUeIdChng(tKwCb,ueInfo,newUeInfo,cfgTmpData))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cfgTmpData->ueInfo->cellId, 
            "Validation Failure for UeId change");
   }

   KwUlUdxUeIdChgReq(&(KW_GET_UDX_SAP(tKwCb)->pst),
                     KW_GET_UDX_SAP(tKwCb)->spId,
                     transId, 
                     ueInfo,
                     newUeInfo);

   RETVALUE(ROK);
} 


/**
 * @brief 
 *    Handler for Configuration Request 
 *
 * @param[in] gCb      RLC Instance Control Block  
 * @param[in] cfgTmpData  Configuration stored in Transaction Block  
 * @param[in] cfg      Configuration block
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC Void kwHdlUiCkwUlCfgReq
(
KwCb             *gCb,
KwUlCfgTmpData   *cfgTmpData,
CkwCfgInfo       *cfg
)
#else
PUBLIC Void kwHdlUiCkwUlCfgReq(gCb, cfgTmpData, cfg)
KwCb             *gCb;
KwUlCfgTmpData   *cfgTmpData;
CkwCfgInfo       *cfg;
#endif
{
   U8   idx;  

   TRC3(kwHdlUiCkwUlCfgReq)

   cfgTmpData->ueId = cfg->ueId;
   cfgTmpData->cellId = cfg->cellId;
   for (idx = 0; idx < cfg->numEnt; idx++)
   {
      cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status = CKW_CFG_CFM_OK;
      cfgTmpData->cfgEntData[idx].entUlCfgCfm.rbId = cfg->entCfg[idx].rbId; 
      cfgTmpData->cfgEntData[idx].entUlCfgCfm.rbType = cfg->entCfg[idx].rbType;
      switch(cfg->entCfg[idx].cfgType)
      {
         case CKW_CFG_ADD:
         case CKW_CFG_MODIFY:
         case CKW_CFG_DELETE:
         {
         
            if(cfg->entCfg[idx].dir & KW_DIR_UL)
            {
               /* Configuration is for UL , thus validating */
               if(ROK != kwCfgValidateUlRb(gCb,
                                           &cfg->entCfg[idx], 
                                           &cfgTmpData->cfgEntData[idx],
                                           cfgTmpData))
               {
                  RLOG_ARG2(L_ERROR,DBG_UEID, cfgTmpData->ueId,
                        "CELLID [%u]:Validation Failure for UL RB [%d]",
                         cfg->cellId,cfg->entCfg[idx].rbId);
                  cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status = CKW_CFG_CFM_NOK;
                  /*Validation is getting failed so no need to do configuration at DL.
                   *Set dir as UL, so that no configuration is done at DL */
                  cfg->entCfg[idx].dir = KW_DIR_UL;
               }
            }
            if(cfg->entCfg[idx].dir == KW_DIR_UL)
            {
               /*If the configuration is for UL only then apply it */   
               if (cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status == CKW_CFG_CFM_OK)
               {
                  kwCfgApplyUlRb(gCb, 
                                 &cfg->entCfg[idx], 
                                 &cfgTmpData->cfgEntData[idx],
                                 cfgTmpData);
               }
            }
            break;
         }
         case CKW_CFG_REESTABLISH:   
         {
            if(cfg->entCfg[idx].dir & KW_DIR_UL)
            {
               if(ROK != kwCfgValidateReEstRb(gCb,
                                              cfg->ueId, 
                                              cfg->cellId, 
                                              &cfg->entCfg[idx], 
                                              &cfgTmpData->cfgEntData[idx]))
               {
                  RLOG_ARG2(L_ERROR,DBG_UEID,cfg->ueId,
                        "CellID [%u]:Validation Failure for Reest UL RB [%d]",
                        cfg->cellId,cfg->entCfg[idx].rbId);
                  cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status = CKW_CFG_CFM_NOK;
                  /* Setting dir as UL, so that no configuration is done at DL */
                  cfg->entCfg[idx].dir = KW_DIR_UL;
                  break;
               }
            }
            if(cfg->entCfg[idx].dir == KW_DIR_UL)
            {
               /*If the configuration is for UL only then apply it */   
               if (cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status == CKW_CFG_CFM_OK)
               {
                  kwCfgApplyReEstUlRb(gCb,  
                                      cfg->ueId, 
                                      cfg->cellId,
                                      TRUE,
                                      &cfgTmpData->cfgEntData[idx]);
               }
            }
            break;
         }
         case CKW_CFG_DELETE_UE :   
         {
            if(ROK != kwCfgValidateDelUlUe(gCb,  
                                           &cfg->entCfg[idx], 
                                           &cfgTmpData->cfgEntData[idx],
                                           cfgTmpData))
            {
               RLOG_ARG1(L_ERROR,DBG_CELLID,cfg->cellId,
                     "UL UEID [%d]:Validation Failure",
                     cfgTmpData->ueId);
               cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status = CKW_CFG_CFM_NOK;
               /* Setting dir as UL, so that no configuration is done at DL */
               cfg->entCfg[idx].dir = KW_DIR_UL;
            }
            break;
         }
         case CKW_CFG_DELETE_CELL :   
         {
            if(ROK != kwCfgValidateDelUlCell(gCb,
                                             cfg->cellId, 
                                             &cfg->entCfg[idx], 
                                             &cfgTmpData->cfgEntData[idx],
                                             cfgTmpData))
            {
               RLOG_ARG0(L_ERROR,DBG_CELLID,cfg->cellId,
                     "Del UL Cell Validation Failure");
               cfgTmpData->cfgEntData[idx].entUlCfgCfm.status.status = CKW_CFG_CFM_NOK;
               /* Setting dir as UL, so that no configuration is done at DL */
               cfg->entCfg[idx].dir = KW_DIR_UL;
            }
            break;
         }
      }/* switch end */
   }/* for end */
   RETVOID;
}


/*****************************************************************************
 *                          KWU INTERFACE
 ****************************************************************************/
/**
 * @brief  
 *    Handler for binding the RLC upper layer service user with 
 *    the RLC layer.
 *
 * @details
 *    This function is used by RLC user to request for binding to 
 *    RLC.This function is called by the KWU interface to bind 
 *    RLC's SAP (identified by spId) with the service user's
 *    SAP (identified by suId). 
 *
 * @param[in] pst   Post structure  
 * @param[in] suId  Service user SAP ID 
 * @param[in] spId  Service provider ID
 *
 * @return  S16
 *    -# ROK
 *    -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuBndReq 
(
Pst    *pst,  
SuId   suId, 
SpId   spId 
)
#else
PUBLIC S16 KwUiKwuBndReq (pst, suId, spId)
Pst    *pst;  
SuId   suId; 
SpId   spId; 
#endif
{
   KwKwuSapCb   *kwuSap;     /* SAP Config Block */
   KwCb         *tKwCb;
   
   TRC3(KwUiKwuBndReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pst->dstInst >= KW_MAX_RLC_INSTANCES)
   {
      RETVALUE(RFAILED);
   }
#endif
   tKwCb = KW_GET_KWCB(pst->dstInst);
   RLOG2(L_DEBUG, "KwUiKwuBndReq(pst, spId(%d), suId(%d))", spId, suId);

    /* Validation of input parameters */
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(!((spId < (S16) tKwCb->genCfg.maxKwuSaps) && (spId >=0))) 
   {
      RLOG0(L_ERROR,"Invalid spId");
      KW_SEND_SAPID_ALARM(tKwCb,spId, LKW_EVENT_KWU_BND_REQ, LCM_CAUSE_INV_SAP);
      RETVALUE(RFAILED); 
   }
#endif

   /* Get Sap control block */
   kwuSap = (tKwCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            (tKwCb->u.dlCb->kwuDlSap + spId):
            (tKwCb->u.ulCb->kwuUlSap + spId);

   /* Take action based on the current state of the SAP */
   switch(kwuSap->state)
   {
      /* SAP is configured but not bound */
      case KW_SAP_CFG:
      case KW_SAP_UBND:
      {
         /* copy bind configuration parameters in sap */
         kwuSap->suId          = suId;
         kwuSap->pst.dstProcId = pst->srcProcId;
         kwuSap->pst.dstEnt    = pst->srcEnt;
         kwuSap->pst.dstInst   = pst->srcInst;

         /* Update the State */
         kwuSap->state = KW_SAP_BND;

         RLOG1(L_DEBUG, "KwUiKwuBndReq: state (%d)", kwuSap->state);
         break;
      }
      case KW_SAP_BND:
      {
         /* Sap is already bound check source, destination Entity and Proc Id */
         if (kwuSap->pst.dstProcId != pst->srcProcId ||
             kwuSap->pst.dstEnt != pst->srcEnt ||
             kwuSap->pst.dstInst != pst->srcInst ||
             kwuSap->suId != suId)
         {
            KW_SEND_SAPID_ALARM(tKwCb,
                                spId, 
                                LKW_EVENT_KWU_BND_REQ, 
                                LCM_CAUSE_INV_PAR_VAL);
            RLOG1(L_ERROR,"RLC Mode [%d] : KWU SAP already Bound",
                  tKwCb->genCfg.rlcMode);
            KwUiKwuBndCfm(&(kwuSap->pst), kwuSap->suId, CM_BND_NOK);
            RETVALUE(RFAILED);
         }
         break;
      }

     default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         RLOG1(L_ERROR,"RLC Mode [%d]:Invalid KWU SAP State in Bind Req",
               tKwCb->genCfg.rlcMode);
         KW_SEND_SAPID_ALARM(tKwCb,
                             spId,
                             LKW_EVENT_KWU_BND_REQ, 
                             LCM_CAUSE_INV_STATE);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         KwUiKwuBndCfm(&(kwuSap->pst), kwuSap->suId, CM_BND_NOK);
         RETVALUE(RFAILED);
      }
   }
   KwUiKwuBndCfm(&(kwuSap->pst), kwuSap->suId, CM_BND_OK);
   RETVALUE(ROK);
} 


/**
 * @brief 
 *    Handler for unbinding the RLC upper layer service user with 
 *    the RLC layer.
 *
 * @details
 *    This function is used by RLC user to request for unbinding  
 *    with RLC.This function is called by the KWU interface to 
 *    unbind with RLC. 
 *
 * @param[in] pst     Post structure  
 * @param[in] spId    Service provider SAP ID 
 * @param[in] reason  Reason for Unbinding 
 *
 * @return  S16
 *     -# ROK 
 *     -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuUbndReq
(
Pst      *pst,  
SpId     spId, 
Reason   reason 
)
#else
PUBLIC S16 KwUiKwuUbndReq(pst, spId, reason)
Pst      *pst; 
SpId     spId; 
Reason   reason; 
#endif
{
   KwKwuSapCb   *kwuSap;   /* KWU SAP control block */
   KwCb         *tKwCb;

   TRC3(KwUiKwuUbndReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((pst->dstInst >= KW_MAX_RLC_INSTANCES) ||
       (spId >= (S16) kwCb[pst->dstInst]->genCfg.maxKwuSaps) ||
       (spId < 0))
   {
      RETVALUE (RFAILED);
   }
#endif

   tKwCb = KW_GET_KWCB(pst->dstInst);

   RLOG2(L_DEBUG, "spId(%d), reason(%d)", 
                spId, 
                reason);

   /* Get Sap control block */
   kwuSap = (tKwCb->genCfg.rlcMode == LKW_RLC_MODE_DL) ?
            (tKwCb->u.dlCb->kwuDlSap + spId):
            (tKwCb->u.ulCb->kwuUlSap + spId);

   kwuSap->state = KW_SAP_CFG;

   RETVALUE(ROK);
} 

/**
 * @brief Handler for receiving the data(SDU) from upper layer. 
 *
 * @details
 *    This function is used by RLC service user (PDCP) to 
 *    transfer data (SDU) to RLC.
 *
 * @param[in] pst         Post structure  
 * @param[in] spId        Service Provider SAP ID 
 * @param[in] datreq  Data Request Information
 * @param[in] mBuf        Data Buffer (SDU) 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDatReq
(
Pst             *pst,   
SpId            spId,  
KwuDatReqInfo   *datReq, 
Buffer          *mBuf   
)
#else
PUBLIC S16 KwUiKwuDatReq(pst, spId, datReq, mBuf)
Pst             *pst;  
SpId            spId; 
KwuDatReqInfo   *datReq; 
Buffer          *mBuf;  
#endif
{
   S16          ret = ROK;   /* Return Value */
   KwDlRbCb     *rbCb;       /* RB Control Block */
   KwKwuSapCb   *kwuSap;     /* SAP Config Block */
   KwCb         *tKwCb;

   TRC3(KwUiKwuDatReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((pst->dstInst >= KW_MAX_RLC_INSTANCES) ||
       (spId >= (S16) kwCb[pst->dstInst]->genCfg.maxKwuSaps) ||
       (spId < 0))
   {
      SPutMsg(mBuf);
      RETVALUE(RFAILED);
   }
#endif

   tKwCb = KW_GET_KWCB(pst->dstInst);

   /* Get Sap control block */
   kwuSap = tKwCb->u.dlCb->kwuDlSap + spId;

   /* Validate SAP ID under ERRORCLS */
   KW_VALDATE_SAP(tKwCb,spId, kwuSap, ret);
   if (ret != ROK)
   {
      KW_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Fetch the RbCb */
   kwDbmFetchDlRbCbByRbId(tKwCb, &datReq->rlcId, &rbCb);
   if(!rbCb)
   {
      RLOG_ARG2(L_WARNING, DBG_UEID,datReq->rlcId.ueId, "CellId[%u]:DL RbId [%d] not found",
            datReq->rlcId.cellId,datReq->rlcId.rbId);
      KW_FREE_BUF(mBuf);

      RETVALUE(RFAILED);
   }

   /* kw005.201 update the spId received in datReq to update statistics */
   rbCb->kwuSapId = spId;
   /* Dispatch according to mode of the rbCb */
   switch (rbCb->mode)
   {
      case CM_LTE_MODE_TM:
      {
         /* Verify the user */
         if (pst->srcEnt != ENTNH)
         {
            /* kw002.201 Freeing from proper region */
            KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, datReq, 
                        sizeof(KwuDatReqInfo));
            KW_FREE_BUF(mBuf);
             
            RETVALUE(RFAILED);
         }

         kwTmmQSdu(tKwCb,rbCb, datReq, mBuf);
         /* kw005.201 ccpu00117318, updated statistics */
         kwuSap->sts.sduRx++;
         break;
      }
      case CM_LTE_MODE_UM:
      {
         kwUmmQSdu(tKwCb,rbCb, datReq, mBuf);

         /* kw005.201 ccpu00117318, updated statistics */
         kwuSap->sts.sduRx++;
         break;
      }
      case CM_LTE_MODE_AM:
      {
         kwAmmQSdu(tKwCb,rbCb, mBuf, datReq);
         /* kw005.201 ccpu00117318, updated statistics */
         kwuSap->sts.sduRx++;
         break;
      }
      default:
      {
         RLOG0(L_ERROR, "Invalid RB Mode");
         break;
      }
   }
   RETVALUE(ret);
} 


/**
 * @brief 
 *    Handler for discarding a SDU. 
 *
 * @details
 *    This function is used by RLC AM  and RLC UM entities. 
 *    This function is called by the service user to discard a particular
 *    RLC SDU if it is present in the SDU queue of the RB control block 
 *    and if it is not mapped to any PDU. 
 *
 * @param[in] pst            Post structure  
 * @param[in] spId           Service Provider SAP ID 
 * @param[in] discSdu        SDU discard Information 
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 KwUiKwuDiscSduReq 
(
Pst              *pst,   
SpId             spId,  
KwuDiscSduInfo   *discSdu 
)
#else
PUBLIC S16 KwUiKwuDiscSduReq(pst, spId, discSdu)
Pst              *pst;   
SpId             spId;  
KwuDiscSduInfo   *discSdu; 
#endif
{
#if 1
   KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, discSdu, sizeof(KwuDiscSduInfo));
#else
   S16          ret = ROK;   /* Return Value */
   KwDlRbCb     *rbCb;       /* RB Control Block */
   KwKwuSapCb   *kwuSap;     /* SAP Config Block */
   KwCb         *tKwCb;

   TRC3(KwUiKwuDiscSduReq)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if ((pst->dstInst >= KW_MAX_RLC_INSTANCES) ||
       (spId >= (S16) kwCb[pst->dstInst]->genCfg.maxKwuSaps) ||
       (spId < 0))
   {
      KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, discSdu, sizeof(KwuDiscSduInfo));
      RETVALUE(RFAILED);
   }
#endif

   tKwCb = KW_GET_KWCB(pst->dstInst);

   RLOG_ARG3(L_DEBUG,DBG_UEID,discSdu->rlcId.ueId, 
         "spId(%d) CellId[%u]:DL RbId [%d]",
         spId,discSdu->rlcId.cellId,discSdu->rlcId.rbId);

   /* Get Sap control block */
   kwuSap = tKwCb->u.dlCb->kwuDlSap + spId;

   /* Validate SAP ID under ERRORCLS */
   KW_VALDATE_SAP(tKwCb,spId, kwuSap, ret);
   if (ret != ROK)
   {
      /* kw002.201 Freeing from proper region */
      KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, discSdu, sizeof(KwuDiscSduInfo));
      RETVALUE(RFAILED);
   }

   /* Fetch the rbCb */
   kwDbmFetchDlRbCbByRbId(tKwCb, &discSdu->rlcId, &rbCb);
   if(!rbCb)
   {
      RLOG_ARG2(L_WARNING, DBG_UEID,discSdu->rlcId.ueId, 
            "CellId[%u]:DL RbId [%d] not found",
            discSdu->rlcId.cellId,discSdu->rlcId.rbId);
      /* kw002.201 Freeing from proper region */
      KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, discSdu, sizeof(KwuDiscSduInfo));
      RETVALUE(RFAILED);
   }

   /* DIspatch according to mode */
   switch (rbCb->mode)
   {
      case CM_LTE_MODE_UM:
      {
         kwUmmDiscSdu(tKwCb,rbCb, discSdu->sduId);
         break;
      }
      case CM_LTE_MODE_AM:
      {
         if ((ret = kwAmmDiscSdu(tKwCb,rbCb, discSdu->sduId)))
         {
            RLOG_ARG1(L_WARNING,DBG_RBID,rbCb->rlcId.rbId,
                  "Req for disc sduId [%lu] failed",
                  discSdu->sduId);
         }
         break;
      }
      case CM_LTE_MODE_TM:
      default:
      {
         RLOG0(L_ERROR, "Invalid RB Mode");
         break;
      }
   }
   KW_SHRABL_STATIC_BUF_FREE(pst->region, pst->pool, discSdu, sizeof(KwuDiscSduInfo));
   RETVALUE(ret);
#endif
   RETVALUE(ROK);
} 

/********************************************************************30**
         End of file:     gp_uim.c@@/main/TeNB_Main_BR/5 - Mon Aug 11 16:43:19 2014
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
