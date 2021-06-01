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

     Name:     LTE-RLC Layer 
  
     Type:     C file
  
     Desc:     Source code for PDCP Utility Module

               This file contains following functions:

                  --PjLibObdCmpCfm
                  --PjLibObdIntProtCfm
                  --PjLibObdCipherCfm
                  --PjLibObdDecipherCfm
                  --PjLibObdIntVerCfm
                  --PjLibObdDecmpCfm
                  -- pjUtlCmpReq
                  -- pjUtlIntProtReq
  
     File:     gp_pj_utl.c

     Sid:      eq_utl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:54 2015

     Prg:      nm 

**********************************************************************/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=231;
/** @file gp_pj_utl.c
@brief PDCP Utility Module 
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
#include "cpj.h"                /* CPJ defines */
#include "pju.h"                /* PJU defines */
#include "lpj.h"                /* LPJ defines */

#include "pj_env.h"        /* RLC environment options */
#include "pj.h"            /* RLC defines */
#include "pj_ul.h"
#include "pj_dl.h"
#include "pj_err.h"        /* Error defines */

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
#include "cpj.x"                /* CPJ */
#include "pju.x"                /* PJU */
#include "lpj.x"                /* LPJ */
#include "pj.x"
#include "pj_udx.h"          /* LIB */
#include "pj_udx.x"          /* LIB */
#include "pj_ul.x"
#include "pj_dl.x"

#include "pj_lib.x"          /* LIB */

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

/** @addtogroup dldata */
/*@{*/

#ifdef PJ_CMP_ASYNC
/********************************************************************
 *    Input Handler from Offboarding unit                           *
 *******************************************************************/
/**
 *
 * @brief 
 *
 *        Asynchronous handler to received that Init Confirm for 
 *        compression for the  offboarding unit.
 *
 * @b Description:
 *
 *        This function gets the context Id passed as the parameter and stores it
 *        in the RbCb.
 *
 *  @param[in]  pst      Post structure 
 *  @param[in]  cmpCxt   Context for ROHC
 *  @param[in]  ctxId    Context Id
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdCmpInitCfm
(
Pst      *pst,                    /* post structure */
PjCmpCxt cmpCxt,                  /* Context for ROHC */
PTR      ctxId                    /* Compression Context ID */
)
#else
PUBLIC S16 PjLibObdCmpInitCfm(pst, cmpCxt, ctxId)
Pst      *pst;                    /* post structure */
PjCmpCxt cmpCxt;                  /* Context for ROHC */
PTR      ctxId;                   /* Compression Context ID */
#endif
{                                 
   PjCb     *tPjCb;

   TRC3(PjLibObdCmpInitCfm);
   
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

   RLOG4(L_DEBUG, "PjLibObdCmpInitCfm(pst, cmpCxt(%d,%d,%d,%d)"
           cmpCxt.ueId, cmpCxt.rbId, cmpCxt.rbType, cmpCxt.cfgTxId);
   RLOG1(L_DEBUG ," ctxId(%ld))",ctxId);

   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }

   if (tPjCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      PjUlUeCb   *ueCb = NULLP;         /* UeCb control block */
      PjUlRbCb   *pjRbCb = NULLP;       /* PDCP RB control block */

      pjDbmFetchUlUeCb(tPjCb,cmpCxt.ueId, cmpCxt.cellId, &ueCb);
      if(ueCb  == NULLP)
      {
         RLOG_ARG1(L_ERROR, DBG_CELLID, cmpCxt.cellId,"UL UeId[%u] not found",
            cmpCxt.ueId);
         RETVALUE(RFAILED);
      }
      
      pjDbmFetchPjUlRbCb(ueCb, cmpCxt.rbId, cmpCxt.rbType,&pjRbCb); 
      if(pjRbCb  == NULLP)
      {
         RLOG_ARG2(L_ERROR, DBG_UEID,cmpCxt.ueId,"CellId[%u]: UL RbId[%d] not found",
            cmpCxt.cellId, cmpCxt.rbId);
         RETVALUE(RFAILED);
      }

      pjRbCb->cmpCxtId  =  ctxId;

      /* Check if all confirms have been received */
      pjUtlUlHdlObdInitCfm(tPjCb,ueCb, cmpCxt.cfgTxId, PJ_CMP_INIT_CFM,  cmpCxt.rbId);
   }
   else
   {
      PjDlUeCb   *ueCb = NULLP;         /* UeCb control block */
      PjDlRbCb   *pjRbCb = NULLP;       /* PDCP RB control block */

      pjDbmFetchDlUeCb(tPjCb,cmpCxt.ueId, cmpCxt.cellId, &ueCb);
      if(ueCb  == NULLP)
      {
         RLOG_ARG1(L_ERROR, DBG_CELLID, cmpCxt.cellId,"DL UeId[%u] not found",
                cmpCxt.ueId);
         RETVALUE(RFAILED);
      }

      pjDbmFetchPjDlRbCb(ueCb, cmpCxt.rbId, cmpCxt.rbType,&pjRbCb);
      if(pjRbCb  == NULLP)
      {
         RLOG_ARG2(L_ERROR, DBG_UEID,cmpCxt.ueId, "CellId[%u]: DL RbId[%d] not found",
                cmpCxt.cellId, cmpCxt.rbId);
         RETVALUE(RFAILED);
      }

      pjRbCb->cmpCxtId  =  ctxId;

      /* Check if all confirms have been received */
      pjUtlDlHdlObdInitCfm(tPjCb,ueCb, cmpCxt.cfgTxId, PJ_CMP_INIT_CFM,  cmpCxt.rbId);
   }


   RETVALUE(ROK);

} /* end of PjLibObdCmpInitCfm */
#endif /* PJ_SEC_ASYNC */

#ifdef PJ_SEC_ASYNC
/**
 *
 * @brief 
 *
 *        Asynchronous handler to received that Init Confirm for 
 *        Integrity for the  offboarding unit.
 *
 * @b Description:
 *
 *        This function gets the context Id passed as the parameter and stores it
 *        in the UeCb.
 *
 *  @param[in]  pst        Post structure 
 *  @param[in]  secCxt     Context for Integrity protection/verification 
 *  @param[in]  ctxId      Integrity Context Id
 *
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdIntInitCfm
(
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Integrity Context ID */
)
#else
PUBLIC S16 PjLibObdIntInitCfm(pst, secCxt, ctxId)
Pst      *pst;                    /* post structure */
PjSecCxt secCxt;                  /* Secutriy Context */
PTR      ctxId;                   /* Integrity Context ID */
#endif
{                                 
   PjCb     *tPjCb;

   TRC3(PjLibObdIntInitCfm)

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);


   RLOG4(L_DEBUG, "PjLibObdCmpInitCfm(pst, secCxt(%d,%d,%d), ctxId(%ld))",
           secCxt.cellId, secCxt.ueId, secCxt.cfgTxId, ctxId);

   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }

   if (tPjCb->pjGenCfg.mode  == LPJ_MODE_PDCP_UL)
   {
      PjUlUeCb   *ueCb = NULLP;         /* UeCb control block */
 
      /* Fetch UeCb */
      pjDbmFetchUlUeCb(tPjCb, secCxt.ueId, secCxt.cellId, &ueCb);
      if(ueCb  == NULLP)
      {
         RLOG_ARG1(L_ERROR, DBG_CELLID, secCxt.cellId,"UL UeId[%u] not found",
            secCxt.ueId);
         RETVALUE(RFAILED);
      }

      /* Update context Id */
      ueCb->secInfo.intCxtId = ctxId;

      pjUtlUlHdlObdInitCfm(tPjCb, ueCb, secCxt.cfgTxId, PJ_SEC_INIT_CFM,  
                                             PJ_LIB_INT_BIT_MASK);
   }
   else
   {
      PjDlUeCb   *ueCb = NULLP;         /* UeCb control block */
 
      /* Fetch UeCb */
      pjDbmFetchDlUeCb(tPjCb, secCxt.ueId, secCxt.cellId, &ueCb);
      if(ueCb  == NULLP)
      {
         RLOG_ARG1(L_ERROR, DBG_CELLID, secCxt.cellId,"DL UeId[%u] not found",
            secCxt.ueId);
         RETVALUE(RFAILED);
      }

      /* Update context Id */
      ueCb->secInfo.intCxtId = ctxId;

      pjUtlDlHdlObdInitCfm(tPjCb, ueCb, secCxt.cfgTxId, PJ_SEC_INIT_CFM,  
                                             PJ_LIB_INT_BIT_MASK);
   }

   RETVALUE(ROK);

} /* end of PjLibObdIntInitCfm */

/**
 *
 * @brief 
 *
 *        Asynchronous handler to received that Init Confirm for 
 *        Ciphering Cp Key  for the  offboarding unit.
 *
 * @b Description:
 *
 *        This function gets the context Id passed as the parameter and stores it
 *        in the UeCb.
 *
 *  @param[in]  pst        Post structure 
 *  @param[in]  secCxt     Context for Ciphering 
 *  @param[in]  ctxId      Ciphering CP Context Id
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdCpInitCfm
(
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Ciphering Context ID */
)
#else
PUBLIC S16 PjLibObdCpInitCfm(pst, secCxt, ctxId)
Pst      *pst;                    /* post structure */
PjSecCxt secCxt;                  /* Secutriy Context */
PTR      ctxId;                   /* Ciphering Context ID */
#endif
{
   PjCb     *tPjCb;

   TRC3(PjLibObdCpInitCfm);

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);


   RLOG4(L_DEBUG, "PjLibObdCpInitCfm(pst, secCxt(%d,%d,%d), ctxId(%ld))",
           secCxt.cellId, secCxt.ueId, secCxt.cfgTxId, ctxId);

   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }

   if (tPjCb->pjGenCfg.mode  == LPJ_MODE_PDCP_UL)
   {
      PjUlUeCb   *ueCb = NULLP;         /* UeCb control block */
 
      pjDbmFetchUlUeCb(tPjCb, secCxt.ueId, secCxt.cellId, &ueCb);
      if(ueCb  == NULLP)
      {
         RLOG_ARG1(L_ERROR, DBG_CELLID, secCxt.cellId,"UL UeId[%u] not found",
            secCxt.ueId);
         RETVALUE(RFAILED);
      }

      ueCb->secInfo.cpCxtId =  ctxId;

      /* Update bit mask and check if all confirms have been received */
      pjUtlUlHdlObdInitCfm(tPjCb, ueCb, secCxt.cfgTxId, PJ_SEC_INIT_CFM,  
                           (U8)PJ_LIB_CP_CIP_BIT_MASK);
   }
   else
   {
      PjDlUeCb   *ueCb = NULLP;         /* UeCb control block */
 
      pjDbmFetchDlUeCb(tPjCb, secCxt.ueId, secCxt.cellId, &ueCb);
      if(ueCb  == NULLP)
      {
         RLOG_ARG1(L_ERROR, DBG_CELLID, secCxt.cellId,"DL UeId[%u] not found",
            secCxt.ueId);
         RETVALUE(RFAILED);
      }

      ueCb->secInfo.cpCxtId =  ctxId;

      /* Update bit mask and check if all confirms have been received */
      pjUtlDlHdlObdInitCfm(tPjCb, ueCb, secCxt.cfgTxId, PJ_SEC_INIT_CFM,  
                           (U8)PJ_LIB_CP_CIP_BIT_MASK);
   }

   RETVALUE(ROK);
} /* end of PjLibObdCpInitCfm */

/**
 *
 * @brief 
 *
 *        Asynchronous handler to received that Init Confirm for 
 *        Ciphering Up Key  for the  offboarding unit.
 *
 * @b Description:
 *
 *        This function gets the context Id passed as the parameter and stores it
 *        in the UeCb.
 *
 *  @param[in]  pst        Post structure 
 *  @param[in]  secCxt     Context for Ciphering 
 *  @param[in]  ctxId      Ciphering UP Context Id
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdUpInitCfm
(
Pst      *pst,                    /* post structure */
PjSecCxt secCxt,                  /* Secutiry Context */
PTR      ctxId                    /* Ciphering Context ID */
)
#else
PUBLIC S16 PjLibObdUpInitCfm(pst, secCxt, ctxId)
Pst      *pst;                    /* post structure */
PjSecCxt secCxt;                  /* Secutriy Context */
PTR      ctxId;                   /* Ciphering Context ID */
#endif
{
   PjCb     *tPjCb;

   TRC3(PjLibObdUpInitCfm);

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

   RLOG4(L_DEBUG, "PjLibObdUpInitCfm(pst, secCxt(%d,%d,%d), ctxId(%ld))",
           secCxt.cellId, secCxt.ueId, secCxt.cfgTxId, ctxId);

   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }

   if (tPjCb->pjGenCfg.mode == LPJ_MODE_PDCP_UL)
   {
      PjUlUeCb   *ueCb = NULLP;         /* UeCb control block */

      pjDbmFetchUlUeCb(tPjCb, secCxt.ueId, secCxt.cellId, &ueCb);
      if(ueCb  == NULLP)
      {
         RLOG_ARG1(L_ERROR, DBG_CELLID, secCxt.cellId,"UL UeId[%u] not found",
            secCxt.ueId);
         RETVALUE(RFAILED);
      }

      ueCb->secInfo.upCxtId =  ctxId;

      /* Update bit mask and check if all confirms have been received */
      pjUtlUlHdlObdInitCfm(tPjCb, ueCb, secCxt.cfgTxId, PJ_SEC_INIT_CFM,  
                        (U8)PJ_LIB_UP_CIP_BIT_MASK);
   }
   else
   {
      PjDlUeCb   *ueCb = NULLP;         /* UeCb control block */

      pjDbmFetchDlUeCb(tPjCb, secCxt.ueId, secCxt.cellId, &ueCb);
      if(ueCb  == NULLP)
      {
         RLOG_ARG1(L_ERROR, DBG_CELLID, secCxt.cellId,"DL UeId[%u] not found",
            secCxt.ueId);
         RETVALUE(RFAILED);
      }

      ueCb->secInfo.upCxtId =  ctxId;

      /* Update bit mask and check if all confirms have been received */
      pjUtlDlHdlObdInitCfm(tPjCb, ueCb, secCxt.cfgTxId, PJ_SEC_INIT_CFM,  
                        (U8)PJ_LIB_UP_CIP_BIT_MASK);
   }

   RETVALUE(ROK);

} /* end of PjLibObdUpInitCfm */

#endif /* PJ_SEC_ASYNC */

#ifdef PJ_CMP_ASYNC
/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the compressed SDU from off-board
 *        entity.
 *
 * @b Description:
 *
 *        This function receives an output from compression unit and 
 *        sends the same for ciphering with relevant input paramenters.
 *
 *  @param[in]   pst          Post structure 
 *  @param[in]   cmpCxtId     Context Id for compression 
 *  @param[in]   libTrans     Transaction Id 
 *  @param[in]   opSdu        compressed SDU 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdCmpCfm
(
Pst        *pst,        /* post structure             */
PTR         cmpCxtId,   /* Context Id for compression */
PjLibTrans  libTrans,   /* Transaction Id             */
Buffer     *opSdu       /* compressed SDU             */
)
#else
PUBLIC S16 PjLibObdCmpCfm(pst, cmpCxtId, libTrans, opSdu)
Pst        *pst;        /* post structure             */
PTR         cmpCxtId;   /* Context Id for compression */
PjLibTrans  libTrans;   /* Transaction Id             */
Buffer     *opSdu;      /* compressed SDU             */
#endif
{
   PjCb        *tPjCb;
   PjDlRbCb    *pjRbCb;
   PjTxEnt     *txEnt;
   S16         ret;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;

   TRC3(PjLibObdCmpCfm)

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef ALIGN_64BIT
   RLOG4(L_DEBUG, "PjLibObdCmpCfm(pst, cmpCxtId(%ld), libTrans(%d,%d,%d))",
           cmpCxtId, libTrans.rbId, libTrans.rbType, libTrans.count);
#else
   RLOG4(L_DEBUG, "PjLibObdCmpCfm(pst, cmpCxtId(%ld), libTrans(%d,%d,%ld))",
           cmpCxtId, libTrans.rbId, libTrans.rbType, libTrans.count);
#endif

   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }

   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   rbId =  libTrans.rbId + 1;
   pjDbmFetchPjDlRbCb((PjDlUeCb*)libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb);
   if ( pjRbCb == NULLP )
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,libTrans.ueId, "CellId[%u]: DL RbId[%d] not found",
            libTrans.cellId, rbId);
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
   
   if ( pjRbCb->state == PJ_STATE_REEST || pjRbCb->state == PJ_STATE_REEST_HO)
   {
      PJ_FREE_BUF(opSdu);
      RETVALUE(ret);
   }

   txEnt = pjDbmGetTxEnt(tPjCb,&(pjRbCb->dlCb.txBuf), libTrans.count);
   if ( txEnt == NULLP )
   {
      RLOG1(L_ERROR, "txEnt not found for Cnt [%lu]", libTrans.count);
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
   if ( opSdu == NULLP )
   {
      RLOG_ARG0(L_ERROR, DBG_RBID, rbId ,"Compression Failed , Count [%lu]",  libTrans.count);
      PJ_UPD_DL_VAR(tPjCb, pjRbCb, txEnt->count);
      txEnt->sdu = NULLP;
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(tPjCb,pjRbCb, txEnt, PJU_COMP_FAILED);
      }
      else
      {
          pjDbmDelTxEnt( tPjCb,&(pjRbCb->dlCb.txBuf), txEnt->count);
      }
      tPjCb->pjGenSts.numCmpFails++;
      RETVALUE(RFAILED);
   }

   txEnt->pdu    = opSdu;
   if ( pjRbCb->ueCb->secInfo.secAct )
   {
      ret = pjDlmHdlCiph(tPjCb,pjRbCb, txEnt);
      if ( ret != ROK )
      {
         RLOG_ARG0(L_ERROR, DBG_RBID, rbId ,"CipherReq Failed , Count [%lu]",  libTrans.count);
         RETVALUE(RFAILED);
      }
#ifdef PJ_SEC_ASYNC
      RETVALUE(ret);
#endif
   }
   ret = pjDlmDeliverPdu(tPjCb,pjRbCb, txEnt);
   
   RETVALUE(ret);
} /* end of PjLibObdCmpCfm */

/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the decompressed SDU from off-board
 *        entity.
 *
 * @b Description:
 *
 *        This function receives an output from decompression unit and 
 *        sends the SDU to upper layer in sequence. 
 *
 *  @param[in] pst         post structure
 *  @param[in] cmpCxtId    Context Id for decompression
 *  @param[in] libTrans    Transaction Id
 *  @param[in] opSdu       decompressed SDU
 *  @param[in] fbPkt       Feedback Pkt
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdDecmpCfm
(
Pst         *pst,                 /* post structure */
PTR         cmpCxtId,             /* Context Id for decompression */
PjLibTrans  libTrans,             /* Transaction Id */
Buffer      *opSdu,               /* decompressed SDU */
Buffer      *fbPkt                /* Feedback pkt */
)
#else
PUBLIC S16 PjLibObdDecmpCfm(pst, cmpCxtId, libTrans, opSdu, fbPkt)
Pst         *pst;                 /* post structure */
PTR         cmpCxtId;             /* Context Id for decompression */
PjLibTrans  libTrans;             /* Transaction Id */
Buffer      *opSdu;               /* decompressed SDU */
Buffer      *fbPkt;               /* feedback Pkt */
#endif
{
   PjCb       *tPjCb;
   PjUlRbCb   *pjRbCb;
   PjUlCb     *ulCb;
   PjRxEnt    *rxEnt;

   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;

   TRC2(PjLibObdDecmpCfm);
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef ALIGN_64BIT
   RLOG4(L_DEBUG, "PjLibObdDecmpCfm(pst, cmpCxtId(%ld), libTrans(%d,%d,%d))",
        cmpCxtId, libTrans.rbId, libTrans.rbType, libTrans.count);
#else
   RLOG4(L_DEBUG, "PjLibObdDecmpCfm(pst, cmpCxtId(%ld), libTrans(%d,%d,%ld))",
       cmpCxtId, libTrans.rbId, libTrans.rbType, libTrans.count);
#endif

   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      PJ_FREE_BUF(opSdu);
      PJ_FREE_BUF(fbPkt);
      RETVALUE(RFAILED);
   }

   /* 1. Initialisations */
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   rbId =  libTrans.rbId + 1;
   pjDbmFetchPjUlRbCb((PjUlUeCb*)libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb);
   if(pjRbCb == NULLP)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,libTrans.ueId, "CellId[%u]: UL RbId[%d] not found",
            libTrans.cellId, rbId);
      RETVALUE(RFAILED);
   }
   ulCb  =  &pjRbCb->ulCb;

   /* 2. Send the feedback packet */
   if(fbPkt != NULLP)
   {
      pjUtlUlSndUlRohcFdbk(tPjCb,pjRbCb,fbPkt);
   }

   
   /* 3. Get the rxEnt */
   rxEnt =  pjDbmGetRxEnt(tPjCb,&ulCb->recBuf, libTrans.count );
   if((rxEnt == NULLP) || (rxEnt->state != PJ_SENT_TO_DCOMP))
   {
      /* We  have got a duplicate . 
       * It is NULLP if the original has been sent up,
       * Or, the original is in another state 
       * (if both are in the same state, we accept the 
       * first output) */
      PJ_FREE_BUF(opSdu);
      RETVALUE(ROK);
   }

   /* 4. check decompression status */
   if(opSdu == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_RBID,rbId,"opSdu is NULLP");
      PJ_FREE_BUF(opSdu);
      PJ_SND_PJU_STA_IND(tPjCb,pjRbCb, rxEnt);
      PJ_STS_INC_GEN_CNT(tPjCb,numDecmpFails);
      pjUlmHdlErrUpdates(tPjCb,pjRbCb, rxEnt->count);
      pjDbmDelRxEnt(tPjCb,&ulCb->recBuf, rxEnt->count);
      RETVALUE(RFAILED);
   }

   /* 5. check for discardable entries */
   if(rxEnt->discFlag == TRUE)
   {
      pjDbmDelRxEnt(tPjCb,&ulCb->recBuf, rxEnt->count);
      PJ_FREE_BUF(opSdu);
      RETVALUE(ROK);
   }

   /* 6. Post decomp updations */
   rxEnt->state   =  PJ_RDY_TO_SUBMIT;
   PJ_FREE_BUF(rxEnt->mBuf);
   rxEnt->mBuf  =  opSdu;

   /* 7. Call the function to deliver the pdu*/
   PJ_DEC_OBD_COUNT(tPjCb,pjRbCb, rxEnt->count);
   PJ_ULM_DELIVER_DRB(tPjCb,pjRbCb, rxEnt);

   RETVALUE(ROK);
} /* end of PjLibObdDecmpCfm */

#endif /* PJ_CMP_ASYNC */

#ifdef PJ_SEC_ASYNC 
/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the MAC-I from off-board
 *        entity.
 *
 * @b Description:
 *
 *        This function receives an output, MAC-I from Integration unit and 
 *        sends the data for ciphering with relevant input paramenters.
 *
 *  @param[in]   pst         Post structure 
 *  @param[in]   cxtId       Context Id for Integrity protection
 *  @param[in]   libTrans    Transaction Id 
 *  @param[in]   macI        Message authentication code 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdIntProtCfm
(
Pst      *pst,                    /* post structure */
PTR      cxtId,                   /* Context Id for Integrity protection*/
PjLibTrans libTrans,              /* Transaction Id */
U32      macI                     /* Message authentication code */
)
#else
PUBLIC S16 PjLibObdIntProtCfm(pst, cxtId, libTrans, macI)
Pst      *pst;                    /* post structure */
PTR      cxtId;                   /* Context Id for Integrity protection */
PjLibTrans libTrans;              /* Transaction Id */
U32      macI;                    /* Message authentication code */
#endif
{
   PjCb    *tPjCb;
   U8         hdr;
   PjDlRbCb   *pjRbCb;
   Buffer   *ciphSdu;
   S16      ret;
   U32      count;
   PjTxEnt  *txEnt;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;
  
   TRC3(PjLibObdIntProtCfm)
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef ALIGN_64BIT
   RLOG4(L_DEBUG, "PjLibObdIntProtCfm(pst, cxtId(%ld), libTrans(%d,%d,%d))",
           cxtId, libTrans.rbId, libTrans.rbType, libTrans.count);
#else
   RLOG4(L_DEBUG, "PjLibObdIntProtCfm(pst, cxtId(%ld), libTrans(%d,%d,%ld))",
           cxtId, libTrans.rbId, libTrans.rbType, libTrans.count);
#endif


   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }
   
   count    = libTrans.count;
   ret      = ROK;
   txEnt    = NULLP;
   ciphSdu  = NULLP;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   rbId =  libTrans.rbId + 1;
   pjDbmFetchPjDlRbCb((PjDlUeCb*)libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb);
   if ( pjRbCb == NULLP )
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,libTrans.ueId, "CellId[%u]: DL RbId[%d] not found",
            libTrans.cellId, rbId);
      RETVALUE(RFAILED);
   }
   if ( pjRbCb->state == PJ_STATE_REEST || pjRbCb->state == PJ_STATE_REEST_HO)
   {
      RETVALUE(ret);
   }

   txEnt = pjDbmGetTxEnt(tPjCb,&(pjRbCb->dlCb.txBuf), count);
   if ( txEnt == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_RBID,rbId,"txEnt is NULLP, Count[%lu]",
            count);
      RETVALUE(RFAILED);
   }


   PJ_PACK_MACI(txEnt->sdu, macI);
   
   ret = SRemPreMsg(&hdr, txEnt->sdu);
   if ( ret != ROK )
   {
      RLOG_ARG1(L_ERROR,DBG_RBID,rbId,"SRemPreMsg failed , Count [%lu]",
            count);
      PJ_UPD_DL_VAR(tPjCb,pjRbCb, txEnt->count);
      PJ_SND_CFM(tPjCb,pjRbCb, txEnt, PJ_REM_MSG_FAILED);
      pjDbmDelTxEnt(tPjCb,&(pjRbCb->dlCb.txBuf), txEnt->count);
      RETVALUE(RFAILED);
   }
    
   txEnt->pdu = txEnt->sdu;

   if (pjRbCb->firstDlMsg == TRUE && pjRbCb->firstSn == txEnt->sn)
   {
      pjRbCb->firstDlMsg = FALSE; 
      txEnt->sdu = NULLP;
      ret = pjDlmDeliverPdu(tPjCb,pjRbCb, txEnt);
   }
   else
   {
      ret = pjDlmHdlCiph(tPjCb,pjRbCb, txEnt);
      if ( ret != ROK )
      {    
         RLOG_ARG1(L_ERROR,DBG_RBID,rbId,"CipherReq failed, Count[%lu]",
                libTrans.count);
      } 
   }
   
   RETVALUE(ret);

} /* end of PjLibObdIntProtCfm */

/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the ciphered SDU from off-board
 *        entity.
 *
 * @b Description:
 *
 *        This function receives an output from ciphering unit and 
 *        sends the PDU to lower layer in sequence.
 *
 *  @param[in]   pst              Post structure
 *  @param[in]   cxtId            Context Id for Ciphering 
 *  @param[in]   libTrans         Transaction Id 
 *  @param[in]   opSdu            ciphered SDU 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdCipherCfm
(
Pst      *pst,                    /* post structure */
PTR      cxtId,                   /* Context Id for Ciphering */
PjLibTrans libTrans,              /* Transaction Id */
Buffer   *opSdu                   /* ciphered SDU */
)
#else
PUBLIC S16 PjLibObdCipherCfm(pst, cxtId, libTrans, opSdu)
Pst      *pst;                    /* post structure */
PTR      cxtId;                   /* Context Id for Integrity protection */
PjLibTrans libTrans;              /* Transaction Id */
Buffer   *opSdu;                  /* ciphered SDU */
#endif
{
   PjCb     *tPjCb;
   PjDlRbCb   *pjRbCb;
   S16      ret;
   PjTxEnt  *txEnt;
   U32      count;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;

   TRC3(PjLibObdCipherCfm)
   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef ALIGN_64BIT
   RLOG4(L_DEBUG, "PjLibObdCipherCfm(pst, cxtId(%ld), libTrans(%d,%d,%d))",
           cxtId, libTrans.rbId, libTrans.rbType, libTrans.count);
#else
   RLOG4(L_DEBUG, "PjLibObdCipherCfm(pst, cxtId(%ld), libTrans(%d,%d,%ld))",
           cxtId, libTrans.rbId, libTrans.rbType, libTrans.count);
#endif

   count  = libTrans.count;
   ret    = ROK;

   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }

   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   rbId =  libTrans.rbId + 1;
   pjDbmFetchPjDlRbCb((PjDlUeCb*)libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb);
   if ( pjRbCb == NULLP )
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,libTrans.ueId, "CellId[%u]: DL RbId[%d] not found",
            libTrans.cellId, rbId);
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
   if ( pjRbCb->state == PJ_STATE_REEST || pjRbCb->state == PJ_STATE_REEST_HO)
   {
      PJ_FREE_BUF(opSdu);
      RETVALUE(ret);
   }
   txEnt = pjDbmGetTxEnt(tPjCb,&(pjRbCb->dlCb.txBuf), count);
   if ( txEnt == NULLP )
   {
      /* Log an error and return */
      RLOG_ARG1(L_ERROR,DBG_RBID,rbId,"txEnt is NULLP, Count[%lu]",
            count);
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
   if ( opSdu == NULLP )
   {
      RLOG_ARG0(L_ERROR,DBG_RBID,rbId,"opSdu is NULLP");
      PJ_UPD_DL_VAR(tPjCb,pjRbCb, txEnt->count);
      if (!pjRbCb->rohc.hdrCmpUsed)
      {
         txEnt->sdu = NULLP;
      }
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(tPjCb,pjRbCb, txEnt, PJU_CIPHER_FAILED);
      }
      else
      {
         pjDbmDelTxEnt(tPjCb, &(pjRbCb->dlCb.txBuf), txEnt->count);
      }
      tPjCb->pjGenSts.numCiphFails++;
      RETVALUE(RFAILED);
   }

   if ( pjRbCb->rohc.hdrCmpUsed )
   {
      PJ_FREE_BUF(txEnt->pdu);
   }
   txEnt->pdu = opSdu;
   ret = pjDlmDeliverPdu(tPjCb,pjRbCb, txEnt);

   RETVALUE(ret);

} /* end of PjLibObdCipherCfm */
/*@}*/

/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the deciphered SDU from off-board
 *        entity.
 *
 * @b Description:
 *
 *        This function receives an output from deciphering unit and 
 *        sends the same for Integrity verification or decompression
 *        based on the rbType.
 * 
 *  @param[in]  pst            Post structure 
 *  @param[in]  cxtId          Context Id for Integrity protection
 *  @param[in]  libTrans       Transaction Id 
 *  @param[in]  opPdu          Deciphered SDU 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdDecipherCfm
(
Pst         *pst,                 /* post structure */
PTR         cxtId,                /* Context Id for Integrity protection*/
PjLibTrans  libTrans,             /* Transaction Id */
Buffer      *opPdu                /* deciphered SDU */
)
#else
PUBLIC S16 PjLibObdDecipherCfm(pst, cxtId, libTrans, opPdu)
Pst         *pst;                 /* post structure */
PTR         cxtId;                /* Context Id for Integrity protection*/
PjLibTrans  libTrans;             /* Transaction Id */
Buffer      *opPdu;               /* deciphered SDU */
#endif
{
   PjCb       *tPjCb;
   PjUlCb     *ulCb;
   PjRxEnt    *rxEnt;
   PjUlRbCb     *pjRbCb;
   Buffer     *outPdu = NULLP;
   S16         ret   =  ROK;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;

   TRC2(PjLibObdDecipherCfm);

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef ALIGN_64BIT
   RLOG4(L_DEBUG, "PjLibObdDecipherCfm(pst, cxtId(%ld), libTrans(%d,%d,%d))",
       cxtId, libTrans.rbId, libTrans.rbType, libTrans.count);
#else
   RLOG4(L_DEBUG, "PjLibObdDecipherCfm(pst, cxtId(%ld), libTrans(%d,%d,%ld))",
       cxtId, libTrans.rbId, libTrans.rbType, libTrans.count);
#endif

   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      PJ_FREE_BUF(opPdu);
      RETVALUE(RFAILED);
   }

   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   rbId =  libTrans.rbId + 1;
   /* 1. Initialisations */
    pjDbmFetchPjUlRbCb((PjUlUeCb*)libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb); 
   if(pjRbCb == NULLP)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,libTrans.ueId, "CellId[%u]: UL RbId[%d] not found",
            libTrans.cellId, rbId);
      PJ_FREE_BUF(opPdu);
      PJ_STS_INC_GEN_CNT(tPjCb,numDeciphFails);
      RETVALUE(RFAILED);
   }

   ulCb  =  &pjRbCb->ulCb;

   /* Validating rxEnt will be done later in code */
   rxEnt =  pjDbmGetRxEnt(tPjCb,&ulCb->recBuf, libTrans.count);

    /* 2. check decipher status */
   if( opPdu == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_RBID, rbId,"opPdu is NULL");
      PJ_FREE_BUF(opPdu);
      PJ_STS_INC_GEN_CNT(tPjCb,numDecmpFails);
      if ( rxEnt != NULLP )
      {
         PJ_SND_PJU_STA_IND(tPjCb,pjRbCb, rxEnt);
         pjUlmHdlErrUpdates(tPjCb,pjRbCb, rxEnt->count);
         pjDbmDelRxEnt(tPjCb,&ulCb->recBuf, rxEnt->count);
      }
      RETVALUE(RFAILED);
   }


   /* 3. Check if it is SRB */
   if(pjRbCb->rbType == PJ_SRB)
   {
      /* 3.1 For reestablishment scenarios, discard */
      if(pjRbCb->state != PJ_STATE_NORMAL )
      {
         /* Discard the mBuf */
         PJ_FREE_BUF(opPdu);
         RETVALUE(ROK);
      }
      
      /* 3.2 Validate the rxEnt */
      if(rxEnt == NULLP)
      {
         RLOG_ARG0(L_ERROR, DBG_RBID,rbId,"rxEnt not found");
         PJ_FREE_BUF(opPdu);
         PJ_STS_INC_GEN_CNT(tPjCb,numDeciphFails);
         RETVALUE(RFAILED);
      }
         
      /* 3.3 Post deciphering updations */
      rxEnt->state   =  PJ_RDY_TO_INTVER;
      PJ_FREE_BUF(rxEnt->mBuf);
      rxEnt->mBuf  =  opPdu;

      /* 3.4 Send for IntVer. This will be ASYNC  */
      ret = pjUlmHdlIntVer(tPjCb,pjRbCb, rxEnt);

      RETVALUE(ret);
   }
   else  /*  DRB */
   {

      /* 4. Special case, 
       * Duplicate entries, can be sent out of order */
      if((rxEnt  == NULLP)||
         (rxEnt->discFlag == TRUE) ||
         (rxEnt->state != PJ_SENT_TO_DCIPHER))
      {
         /* 4.1 is decompression applicable */
         if(pjRbCb->rohc.hdrCmpUsed == TRUE)
         {

            /* 4.1.1 Calling decompression hook */
            if(pjUtlDeCmpReq(tPjCb,pjRbCb, libTrans.count, opPdu, &outPdu) != ROK)
            {
               RETVALUE(RFAILED);
            }

#ifdef PJ_CMP_ASYNC
            PJ_FREE_BUF(opPdu);
            RETVALUE(ROK);
#endif /* PJ_CMP_ASYNC */
         }

         /* 4.1.2 discard output and exit */
         PJ_FREE_BUF(opPdu);
         PJ_FREE_BUF(outPdu);
         if((rxEnt != NULLP) &&
            (rxEnt->discFlag == TRUE))
         {
            pjDbmDelRxEnt(tPjCb,&ulCb->recBuf, rxEnt->count);
         }

         RETVALUE(ROK);
      }
      

      /* 5. Post deciphering updations */
      rxEnt->state   =  PJ_RDY_TO_SUBMIT;
      PJ_FREE_BUF(rxEnt->mBuf);
      rxEnt->mBuf  =  opPdu;

      /* 6. Submit it for decompression */
      pjUlmSubmitForDecmp(tPjCb,pjRbCb, rxEnt);

   }/* if(pjRbCb->rbType == PJ_DRB) */

   RETVALUE(ROK);

} /* end of PjLibObdDecipherCfm */

/**
 *
 * @brief 
 *
 *        Asynchronous handler to receive the status for Integrity verification 
 *        from off-board entity.
 *
 * @b Description:
 *
 *        This function receives status, success or failure for Integrity
 *        verification and sends the data for deciphering with relevant input paramenters.
 *
 *  @param[in] pst        post structure 
 *  @param[in] cxtId      Context Id for Integrity protection
 *  @param[in] libTrans   Transaction Id 
 *  @param[in] status     Integrity verification status 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdIntVerCfm
(
Pst      *pst,                    /* post structure */
PTR      cxtId,                   /* Context Id for Integrity protection*/
PjLibTrans libTrans,              /* Transaction Id */
Status   status                   /* Integrity verification status */
)
#else
PUBLIC S16 PjLibObdIntVerCfm(pst, cxtId, libTrans, status)
Pst      *pst;                    /* post structure */
PTR      cxtId;                   /* Context Id for Integrity protection */
PjLibTrans libTrans;              /* Transaction Id */
Status   status;                  /* Integrity verification status */
#endif
{
   PjCb     *tPjCb;
   PjUlRbCb   *pjRbCb;
   PjUlCb   *ulCb;
   PjRxEnt  *rxEnt;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   CmLteRbId rbId;

   TRC2(PjLibObdIntVerCfm)

   if (pst->dstInst >= PJ_MAX_PDCP_INSTANCES)
   {
      RETVALUE (RFAILED);
   }

   tPjCb = PJ_GET_PJCB(pst->dstInst);

#ifdef ALIGN_64BIT
   RLOG4(L_DEBUG, "PjLibObdIntVerCfm(cxtId(%ld), libTrans(%d,%d,%d))",
          cxtId, libTrans.rbId, libTrans.rbType, libTrans.count);
#else
   RLOG4(L_DEBUG, "PjLibObdIntVerCfm(cxtId(%ld), libTrans(%d,%d,%ld))",
          cxtId, libTrans.rbId, libTrans.rbType, libTrans.count, status);
#endif
   RLOG1(L_DEBUG," status(%d)",status);

   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }

   /* Initialisations */
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   rbId =  libTrans.rbId + 1;
   pjDbmFetchPjUlRbCb((PjUlUeCb*)libTrans.ueCb, rbId, libTrans.rbType,&pjRbCb); 
   if(pjRbCb == NULLP)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,libTrans.ueId, "CellId[%u]: UL RbId[%d] not found",
            libTrans.cellId, rbId);
      PJ_STS_INC_GEN_CNT(tPjCb, numIntgVrfFails);
      RETVALUE(RFAILED);
   }
   ulCb  =  &pjRbCb->ulCb;

   /* For reestablishment cases, exit */
   if(pjRbCb->state != PJ_STATE_NORMAL)
   {
      RETVALUE(ROK);
   }

   /* Get the rxEnt */
   rxEnt =  pjDbmGetRxEnt(tPjCb,&ulCb->recBuf, libTrans.count);
   if(rxEnt == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_RBID, rbId, "rxEnt not found for Count [%lu]", libTrans.count);
      PJ_STS_INC_GEN_CNT(tPjCb, numIntgVrfFails);
      PJ_PERFORM_ERR_UPDATES(tPjCb, pjRbCb, libTrans.count);
      RETVALUE(RFAILED);
   }

   /* Validate status */
   if(status != ROK)
   {
      RLOG_ARG1(L_ERROR, DBG_RBID, rbId, "Integrity Verification Failed , Count [%lu]"
            , rxEnt->count);
      PJ_SND_PJU_STA_IND(tPjCb,pjRbCb, rxEnt);
      PJ_STS_INC_GEN_CNT(tPjCb,numIntgVrfFails);
      PJ_PERFORM_ERR_UPDATES(tPjCb,pjRbCb, rxEnt->count);
/*      pjDbmDelRxEnt(&ulCb->recBuf, rxEnt->count); */
      RETVALUE(RFAILED);
   }

   /* Check if next in line for delivery */
   rxEnt->state   =  PJ_RDY_TO_SUBMIT;
   if(rxEnt->count != ulCb->nxtSubCnt)
   {
      RETVALUE(ROK);
   }

   /* Send for delivery */
   pjUlmDeliverSrb(tPjCb,pjRbCb, rxEnt);

   RETVALUE(ROK);

} /* end of PjLibObdIntVerCfm */
#endif /* PJ_SEC_ASYNC */


/**
 *
 * @brief 
 *
 *        Handler for redirecing compression request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends compression request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb         PDCP RbCb 
 *  @param[in]   mBuf           SDU to be compressed 
 *  @param[in]   opSdu          Compressed SDU 
 *  @param[in]   count          COUNT - transaction Id 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlCmpReq
(
PjCb       *gCb,
PjDlRbCb   *pjRbCb,                 /* PDCP RbCb */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu,                 /* Compressed SDU */
U32      count                    /* COUNT - transaction Id */
)
#else
PUBLIC S16 pjUtlCmpReq(gCb,pjRbCb, mBuf, opSdu, count)
PjCb       *gCb;
PjDlRbCb   *pjRbCb;                 /* PDCP RbCb */
Buffer   *mBuf;                   /* SDU to be compressed */
Buffer   **opSdu;                 /* Compressed SDU */
U32      count;                   /* COUNT - transaction Id */
#endif
{
#ifdef PJ_CMP_ASYNC
   PjLibTrans libTrans; /* Transaction Id for compression */
#endif
   S16        ret;

   ret = ROK;

   TRC3(pjUtlCmpReq)


#ifdef PJ_CMP_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = count;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = (PTR)pjRbCb->ueCb;

   /* Start the timer if it is not started already */
   if((pjChkTmr(gCb, (PTR)pjRbCb, PJ_EVT_DL_OBD_TMR)) == FALSE)
   {
      pjStartTmr(gCb, (PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }

   /* Send to the library */
   ret = PjLibObdCmpReq(&(gCb->pjGenCfg.obdPst.cmpPst), pjRbCb->cmpCxtId, 
                                                      libTrans, mBuf);  
#else
   ret = pjLibCmpReq(pjRbCb->cmpCxtId, mBuf, opSdu);
#endif

   RETVALUE(ret);

} /* end of pjUtlCmpReq */



/**
 *
 * @brief 
 *
 *        Handler for redirecing decompression request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends decompression protection request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb         PDCP RbCb 
 *  @param[in]   mBuf           SDU to be compressed 
 *  @param[in]   opSdu          Compressed SDU 
 *  @param[in]   count          COUNT - transaction Id 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDeCmpReq
(
PjCb       *gCb,
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
U32      count,                   /* COUNT - transaction Id */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu                  /* Compressed SDU */
)
#else
PUBLIC S16 pjUtlDeCmpReq(pjRbCb, count, mBuf, opSdu)
PjCb    *gCb;
PjUlRbCb   *pjRbCb;                 /* PDCP RbCb */
U32      count;                   /* COUNT - transaction Id */
Buffer   *mBuf;                   /* SDU to be compressed */
Buffer   **opSdu;                 /* Compressed SDU */
#endif
{
   S16 ret = ROK;
#ifdef PJ_CMP_ASYNC
   PjLibTrans libTrans; /* Transaction Id for compression */
#else
   Buffer *feedback  = NULLP;
#endif /* PJ_CMP_ASYNC */

   TRC3(pjUtlDeCmpReq)

#ifdef PJ_CMP_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = count;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = (PTR)pjRbCb->ueCb;

   /* Start the timer if it is not started already */
   if((pjChkTmr(gCb, (PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
   {
      pjRbCb->ulCb.obdPdu   =  count;
      pjStartTmr(gCb, (PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }

   ret = PjLibObdDecmpReq(&(gCb->pjGenCfg.obdPst.cmpPst), 
                                      pjRbCb->cmpCxtId, libTrans, mBuf);  
#else
   ret = pjLibDecmpReq(pjRbCb->cmpCxtId, mBuf, &feedback, opSdu);

   if(feedback != NULLP)
   {
      pjUtlUlSndUlRohcFdbk(gCb,pjRbCb,feedback);
   }
#endif

   RETVALUE(ret);

} /* end of pjUtlDeCmpReq */


/**
 *
 * @brief 
 *
 *        Handler for redirecing compression request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends compression request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb           PDCP RbCb
 *  @param[in]   feedback         ROHC Feedback
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlSndFeedback
(
PjCb     *gCb,
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
Buffer   *feedback                /* ROHC Feedback */
)
#else
PUBLIC S16 pjUtlSndFeedback(pjRbCb, feedback)
PjCb     *gCb;
PjUlRbCb   *pjRbCb;                 /* PDCP RbCb */
Buffer   *feedback;               /* Feedback */
#endif
{
#ifdef PJ_CMP_ASYNC
   PjLibTrans libTrans; /* Transaction Id for compression */
#endif /* PJ_SEC_ASYNC */

   TRC3(pjUtlSndFeedback)

   RLOG2(L_DEBUG, "pjUtlCmpReq(pjRbCb(%d,%d))",
                    pjRbCb->rbId, pjRbCb->rbType);

#ifdef PJ_CMP_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = pjRbCb->ulCb.lastSubCnt;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = (PTR)pjRbCb->ueCb;

   PjLibObdCmpFbReq(&(gCb->pjGenCfg.obdPst.cmpPst), pjRbCb->cmpCxtId, libTrans,
         feedback);
#else
   pjLibCmpFbReq(pjRbCb->cmpCxtId, feedback);
#endif

   RETVALUE(ROK);

} /* end of pjUtlSndFeedback */

/**
 *
 * @brief 
 *
 *        Handler for closing the context with the compression unit (either 
 *        synchronous or asynchronous) for RB within an UE.
 *
 * @b Description:
 *
 *        This function closes an existing context with the compression unit per RB per
 *        UE, either using synchronous / asynchronous function calls with relevant input parameters.
 *
 *
 *  @param[in] cmpCxtId         ROHC Context to be closed
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlCmpClose
(
PjCb     *gCb,
PTR      cmpCxtId                 /* ROHC Context to be closed */
)
#else
PUBLIC S16 pjUtlCmpClose(cmpCxtId)
PjCb     *gCb;
PTR      cmpCxtId;                /* ROHC Context to be closed */
#endif
{
   TRC3(pjUtlCmpClose)

   RLOG1(L_DEBUG, "pjUtlCmpClose(cmpCxtId(%ld))", cmpCxtId);

#ifndef PJ_CMP_ASYNC
   pjLibCmpCloseReq(cmpCxtId);
#else
   PjLibObdCmpCloseReq(&gCb->pjGenCfg.obdPst.cmpPst, cmpCxtId);
#endif

   RETVALUE(ROK);

} /* end of pjUtlCmpClose */



/* pj005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_COMMENT

/**
 *
 * @brief Handler for L2 Measurement timer expiry.
 *
 *
 * @b Description
 *        This function is called when the l2 measurement timer expires. 
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
PUBLIC S16 pjUtlHdlL2TmrExp
(
PjL2MeasEvtCb *measEvtCb
)
#else
PUBLIC S16 pjUtlHdlL2TmrExp(measEvtCb)
PjL2MeasEvtCb *measEvtCb;
#endif
{
   CmLList        *node;
   U32            idx;
   PjL2MeasCb     *measCb;
   PjL2MeasCfmEvt measCfmEvt;
   PjRbCb         *pjRbCb;
   PjTxEnt        *txEnt      = NULLP;
   U8             qciIdx;

   TRC3(pjUtlHdlL2TmrExp)

   RLOG1,"pjUtlHdlL2TmrExp(transId(%ld))", measEvtCb->transId);

   measCb = &measEvtCb->measCb;
   while((node = cmLListFirst(&measCb->rbList)))
   {
      node = cmLListDelFrm(&measCb->rbList, node);
      pjRbCb = (PjRbCb *) node->node;

      /* Cleanup any pending buffers which were not acked */
      if(measCb->measType & LPJ_L2MEAS_DL_DELAY)
      {
         for(idx = pjRbCb->rbL2Cb.startCount; idx < pjRbCb->dlCb.count; idx++)
         {
            if((txEnt = pjDbmGetTxEnt( &(pjRbCb->dlCb.txBuf), idx)) == NULLP)
            {
               continue;
            }
            if(txEnt->state != PJ_PDU_SUBMITTED)
            {
               break;
            }
            pjDbmDelTxEnt( &(pjRbCb->dlCb.txBuf), idx);
         }
         pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_DL_DELAY]   =  NULLP;
      }
      if(measCb->measType & PJ_L2MEAS_ACT_UE)
      {
         pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_ACT_UE]   =  NULLP;
      }
      if(measCb->measType & PJ_L2MEAS_DL_DISC )
      {
         pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_DL_DISC]   =  NULLP;
      }
      if(measCb->measType & PJ_L2MEAS_UL_LOSS)
      {
         pjRbCb->rbL2Cb.l2Sts[PJ_L2MEAS_UL_LOSS]   =  NULLP;
      }
    }/* End of while loop */
   /* Initialize measCfmEvt */
   cmMemset((U8 *) &measCfmEvt, 0, sizeof(PjL2MeasCfmEvt));

   measCfmEvt.transId = measEvtCb->transId;
   measCfmEvt.numQciCfm = 0;
   /* Fill the measCfmEvt structure */
   for(qciIdx = 0; qciIdx < measCb->numQci; qciIdx++)
   {
      pjCb.pjL2Cb.measOn[measCb->qci[qciIdx]] &= ~measCb->measType;
      measCfmEvt.measType = measCb->measType;
      measCfmEvt.measCfm[measCfmEvt.numQciCfm].qci = measCb->qci[qciIdx];
      measCfmEvt.status.status = LCM_PRIM_OK;
      measCfmEvt.status.reason = LCM_REASON_NOT_APPL;
      if((measCb->measType &  LPJ_L2MEAS_DL_DELAY) &&
         (measCb->measData[measCb->qci[qciIdx]].dlDelay.numPkts > 0))
      {
         /* Packed delay is in ms */
         measCfmEvt.measCfm[measCfmEvt.numQciCfm].dlDelay = 
         (measCb->measData[measCb->qci[qciIdx]].dlDelay.val / 
         measCb->measData[measCb->qci[qciIdx]].dlDelay.numPkts);
      }
      if((measCb->measType & LPJ_L2MEAS_DL_DISC) &&
         (measCb->measData[measCb->qci[qciIdx]].dlDisc.numPkts > 0))
      {
         /* dlDisc = num of Pkts Discarded * 10^6 / total no of pkts. */
         measCfmEvt.measCfm[measCfmEvt.numQciCfm].dlDisc = 
         measCb->measData[measCb->qci[qciIdx]].dlDisc.val  * (1000000) /
         measCb->measData[measCb->qci[qciIdx]].dlDisc.numPkts;
      }
      if((measCb->measType &  LPJ_L2MEAS_UL_LOSS) &&
         (measCb->measData[measCb->qci[qciIdx]].ulLoss.numPkts > 0))
      {
         /* ul Loss  = num of Pkts lost * 10^6 / total no of pkts. */
         measCfmEvt.measCfm[measCfmEvt.numQciCfm].ulLoss  =
         measCb->measData[measCb->qci[qciIdx]].ulLoss.val  * (1000000) /
         measCb->measData[measCb->qci[qciIdx]].ulLoss.numPkts;
      }
      measCfmEvt.numQciCfm++;
   }

   pjCb.pjL2Cb.pjMeasEvtCb[measEvtCb->cbIdx] = NULLP;
   PJ_FREE(measEvtCb, sizeof(PjL2MeasEvtCb));
   pjCb.pjL2Cb.pjNumMeas--;

   /* Send Measurement confirmation to layer manager */
   PjMiLpjL2MeasCfm(&pjCb.pjGenCfg.lmPst,&measCfmEvt);

   RETVALUE(ROK);
} /* pjUtlHdlL2TmrExp */

/**
 *
 * @brief Handler for Sending Negative confirm .
 *
 *
 * @b Description
 *        This function is called when the l2 measurement cannot be started
 *        This function sends  negative confirm for all the requests
 *
 *  @param[in] measReqEvt    Measurement Req Structure
 *
 *
 *  @return  S16
 *      -# ROK
 */

#ifdef ANSI
PUBLIC S16 pjUtlSndL2MeasNCfm
(
PjL2MeasReqEvt  *measReqEvt,
PjL2MeasCfmEvt  *measCfmEvt
)
#else
PUBLIC S16 pjUtlSndL2MeasNCfm(measReqEvt, measCfmEvt)
PjL2MeasReqEvt  *measReqEvt;
PjL2MeasCfmEvt  *measCfmEvt;
#endif
{

   TRC3(pjUtlSndL2MeasNCfm)

   RLOG1,"pjUtlSndL2MeasNCfm(transId(%ld))", measReqEvt->transId);

   PjMiLpjL2MeasCfm(&pjCb.pjGenCfg.lmPst, measCfmEvt);

   RETVALUE(ROK);
} /* pjUtlSndL2MeasNCfm */
#endif

#ifdef FLAT_BUFFER_OPT
/**
 *
 * @brief 
 *
 *       Handler to move the 'FlatBuffer' into 'Buffer' 
 *
 * @b Description: 
 *
 *            
 *  @param[in] gCb         Global PDCP control block. 
 *  @param[in] FlatBuffer  'FlatBuffer' ptr to be converted. 
 *  @param[in_out] Buffer  Output mBuf. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjUtlConvFbToBuf
(
PjCb       *gCb,
FlatBuffer *fb,
Buffer     **mBuf
)
#else
PUBLIC S16 pjUtlConvFbToBuf(gCb, fb, mBuf)
PjCb       *gCb;
FlatBuffer *fb;
Buffer     **mBuf;
#endif
{
   S16 ret;

   TRC3(pjUtlConvFbToBuf);
#ifdef T2K_MEM_LEAK_DBG
   ret = SAttachPtrToBufNew(gCb->init.region,
         gCb->init.pool,
         fb->ptr,
         fb->len,
         mBuf,
         __FILE__,
         __LINE__);
#else
   ret = SAttachPtrToBuf(gCb->init.region,
         gCb->init.pool,
         fb->ptr,
         fb->len,
         mBuf);

#endif
   if(ret != ROK)
   {
      RLOG0(L_ERROR,"SAttachPtrToBuf failed !!!!!");
   }

   RETVALUE(ret);
}


/**
 *
 * @brief 
 *
 *       Handler to copy a 'FlatBuffer' to 'Buffer' 
 *
 * @b Description: 
 *
 *            
 *  @param[in] gCb         Global PDCP control block. 
 *  @param[in] FlatBuffer  'FlatBuffer' to be converted. 
 *  @param[in_out] Buffer  Output mBuf. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjUtlCopyFbToBuf
(
PjCb       *gCb,
FlatBuffer *fb,
Buffer     **mBuf
)
#else
PUBLIC S16 pjUtlCopyFbToBuf(gCb, fb, mBuf)
PjCb       *gCb;
FlatBuffer *fb;
Buffer     **mBuf;
#endif
{
   U8 *opPdu;
   S16 ret;

   TRC3(pjUtlCopyFbToBuf);

   if((SGetStaticBuffer(gCb->init.region,
               gCb->init.pool,
               (Data
                **)&(opPdu),
               fb->len,
               SS_SHARABLE_MEMORY)
            != ROK))
   {
      RLOG0(L_ERROR,"SGetStaticBuffer failed !!!!!");
      RETVALUE(RFAILED);
   }

   cmMemcpy(opPdu, fb->ptr, fb->len);
   /*RLOG1(L_ERROR,"Data forwarding: PDCP SN %d is converted to mBuf",
    * txEnt->sn); */
#ifdef T2K_MEM_LEAK_DBG
   ret = SAttachPtrToBufNew(gCb->init.region,
         gCb->init.pool,
         opPdu,
         fb->len,
         mBuf,
         __FILE__,
         __LINE__);
#else
   ret = SAttachPtrToBuf(gCb->init.region,
         gCb->init.pool,
         opPdu,
         fb->len,
         mBuf);

#endif
   if(ret != ROK)
   {
      RLOG0(L_ERROR,"SAttachPtrToBuf failed !!!!!");
   }

   RETVALUE(ret);
}
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file:     eq_utl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:54 2015
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
/main/1      ---      nm   1. LTERLC Release 2.1
/main/2      pj004.201 av   1. Adding missing trace
/main/3      pj005.201 ap   1. Corrected the RB ID calculation.
                            2. Added support for L2 Measurement.
                            3. Added function to send the data forward indication instead of macro.
*********************************************************************91*/

