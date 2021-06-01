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
                  -- pjUtlCipherReq
                  -- pjUtlIntProtReq
  
     File:     gp_pj_utl.c

     Sid:      eq_dl_utl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:49 2015

     Prg:      nm 

**********************************************************************/
 static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=218;

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
#include "pj_dl.h"
#include "pj_err.h"        /* Error defines */
#include "pj_udx.h"
#include "pj_ptsec.h"

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
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_dl.x"
#include "pj_lib.x"          /* LIB */


EXTERN CmLListCp pjMsCiphQ;
EXTERN CmLListCp pjMsDeCiphQ;
EXTERN U32 numOutStandingCryptoJobs[2]; /*PJ_QCOM_MAX_CRYPTO_CORE == 2*/
EXTERN U32 dbgPdcpMemCount;
EXTERN U32 dbgPdcpQSizeThreshHoldDropCnt;

EXTERN U32 pjMsDlSpaccQCnt;
EXTERN U32 pjMsUlSpaccQCnt;
#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */


PUBLIC S16 pjUtlReEstDl 
(
PjCb        *gCb,
PjDlUeCb       *ueCb           /* UE Control Block */ 
);

PRIVATE S16 pjUtlDlHdlSecInitCfm ARGS((PjCb *gCb,PjDlUeCb *ueCb, 
              U16 txIdx,PjAsyncCfm *asyncCfm, UdxSecCfgCfmInfo *secCfgCfm,UdxReEstCfmInfo *reEstCfm));
PRIVATE S16 pjUtlDlHdlCmpInitCfm ARGS((PjCb *gCb,PjDlUeCb *ueCb, 
              U16 txIdx,PjAsyncCfm *asyncCfm, UdxCfgCfmInfo *cfgCfm));

/**
 *
 * @brief 
 *
 *        Handler to apply the new security algorithms and 
 *        reset the compression unit.
 *
 * @b Description:
 *
 *        This function 
 *        1. Creates new contexts for Control Plane Ciphering and Integrity.
 *        2. Closes the old contexts for Control Plane Ciphering and Integrity.
 *
 *  @param[in] ueCb   UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDlUpdUpSecKeys
(
PjCb       *gCb,
PjDlUeCb   *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlDlUpdUpSecKeys(gCb,ueCb)
PjCb         *gCb;
PjDlUeCb     *ueCb;           /* UE Control Block */ 
#endif
{
   PjSec    *secInfo;
   Void     *tmpUpCxt;

   TRC2(pjUtlDlUpdUpSecKeys);

   RLOG2(L_DEBUG, "pjUtlReEstReconfig(ueCb(%d,%d))",
         ueCb->key.ueId, ueCb->key.cellId);

   secInfo  =  &ueCb->secInfo;

   if(ueCb->secInfo.secAct == TRUE)
   {
      /* Initialisation is done first followed by closure since we
       * do not want the same ctxId to be returned for the new init.
       * In ASYNC case, we drop packets if they are returned from a
       * different ctxId. */

      tmpUpCxt  = ueCb->secInfo.upCxtId;    

      pjUtlDlUpInit(gCb,ueCb);
      pjUtlDlCipherClose(gCb,tmpUpCxt);
   }

   RETVALUE(ROK);
} /* pjUtlDlUpdUpSecKeys */

/**
 *
 * @brief 
 *
 *        Handler to apply the new security algorithms and 
 *        reset the compression unit.
 *
 * @b Description:
 *
 *        This function 
 *        1. Creates new contexts for Control Plane Ciphering and Integrity.
 *        2. Closes the old contexts for Control Plane Ciphering and Integrity.
 *
 *  @param[in] ueCb   UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDlUpdCpSecKeys
(
PjCb       *gCb,
PjDlUeCb   *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlDlUpdCpSecKeys(gCb,ueCb)
PjCb         *gCb;
PjDlUeCb     *ueCb;           /* UE Control Block */ 
#endif
{
   PjSec    *secInfo;
   Void     *tmpIntCxt;
   Void     *tmpCpCxt;

   TRC2(pjUtlDlUpdCpSecKeys);

   RLOG2(L_DEBUG, "pjUtlReEstReconfig(ueCb(%d,%d))",
         ueCb->key.ueId, ueCb->key.cellId);

   secInfo  =  &ueCb->secInfo;

   if(ueCb->secInfo.secAct == TRUE)
   {
      /* Initialisation is done first followed by closure since we
       * do not want the same ctxId to be returned for the new init.
       * In ASYNC case, we drop packets if they are returned from a
       * different ctxId. */

      tmpIntCxt = ueCb->secInfo.intCxtId;  
      tmpCpCxt  = ueCb->secInfo.cpCxtId;    

      pjUtlDlCpInit(gCb,ueCb);
      pjUtlDlIntInit(gCb,ueCb);

      pjUtlDlIntClose(gCb,tmpIntCxt);
      pjUtlDlCipherClose(gCb,tmpCpCxt);

   }

   RETVALUE(ROK);

} /* pjUtlDlUpdCpSecKeys */

/********************************************************************
 *    Utility Handler for Sending to Offboarding unit               *
 *******************************************************************/

/**
 *
 * @brief 
 *
 *        Handler for opening the context with the compression unit (either 
 *        synchronous or asynchronous) for RB within an UE.
 *
 * @b Description:
 *
 *        This function opens the context with the compression unit per RB per
 *        UE, by sending ROHC configuration elements using synchronous / asynchronous 
 *        function calls with relevant input parameters.
 *
 *
 *  @param[in]  pjRbCb   PDCP RbCb
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDlCmpInit
(
PjCb       *gCb,
PjDlRbCb   *pjRbCb                  /* PDCP RbCb */
)
#else
PUBLIC S16 pjUtlDlCmpInit(pjRbCb)
PjCb       *gCb;
PjDlRbCb   *pjRbCb;                 /* PDCP RbCb */
#endif
{
   TRC3(pjUtlDlCmpInit)

   RLOG2(L_DEBUG, "pjUtlDlCmpInit(pjRbCb(%d,%d))",pjRbCb->rbId, pjRbCb->rbType);
#ifdef PJ_CMP_ASYNC

   pjRbCb->ueCb->libInfo.tCmpInitBitMask |= (1 << (pjRbCb->rbId - 1));
   pjRbCb->ueCb->libInfo.tLibInitBitMask |= PJ_LIB_COMP_BIT_MASK;
   pjRbCb->cmpCxt.cfgTxId  =  pjRbCb->ueCb->libInfo.nxtAvlbTxId;

   PjLibObdCmpInitReq(&gCb->pjGenCfg.obdPst.cmpPst, 
                                    pjRbCb->cmpCxt, pjRbCb->rohc);
#else
   pjLibCmpInitReq(pjRbCb->cmpCxt, pjRbCb->rohc, &(pjRbCb->cmpCxtId));
#endif  /* PJ_CMP_ASYNC */

   RETVALUE(ROK);

} /* end of pjUtlCmpInit */



/**
 *
 * @brief 
 *
 *        Handler for opening the context with the Integration unit (either 
 *        synchronous or asynchronous) for all SRBs of an UE.
 *
 * @b Description:
 *
 *        This function opens the context with the security algo unit per UE
 *        for Integration protection/verification by sending the configured
 *        Integrity Info using synchronous / asynchronous function calls with 
 *        relevant input parameters.
 *
 *  @param[in]  pjRbCb     Pointer to RB control Block
 *
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDlIntInit
(
PjCb     *gCb,
PjDlUeCb   *ueCb                   /* UE CB Ptr */
)
#else
PUBLIC S16 pjUtlDlIntInit(*gCb, *ueCb)
PjCb     *gCb;
PjDlUeCb   *ueCb;                   /* UE CB Ptr */
#endif
{

   S16 ret=ROK;
   Void *ctxId = NULLP;   
#ifdef PJ_SEC_ASYNC
   U8  txIdx;
   U32 *libInitBitMask = NULLP;
#endif
   
   TRC3(pjUtlDlIntInit)

   RLOG2(L_DEBUG, "pjUtlDlIntInit(ueCb(%d,%d))",ueCb->key.ueId, ueCb->key.cellId);

#ifdef PJ_SEC_ASYNC

   if(ueCb->libInfo.state == PJ_STATE_REEST)
   {
      for(txIdx = 0; txIdx < PJ_MAX_ASYNC_CFM; txIdx++)
      {
         if((ueCb->libInfo.asyncCfm[txIdx] != NULLP) &&
               (ueCb->libInfo.asyncCfm[txIdx]->cfmType == PJ_REEST_ASYNC_CFM))
         {
            libInitBitMask =  &ueCb->libInfo.asyncCfm[txIdx]->libInitBitMask;
            break;
         }
      }
   }
   else if(ueCb->libInfo.state == PJ_STATE_NORMAL)
   {
      libInitBitMask =  &ueCb->libInfo.tLibInitBitMask;
   }

   if(libInitBitMask)
   {
      (*libInitBitMask) = (*libInitBitMask) | PJ_LIB_INT_BIT_MASK;
   }

   ret = PjLibObdIntInitReq(&gCb->pjGenCfg.obdPst.secPst, 
                           ueCb->secInfo.secCxt, ueCb->secInfo.intInfo);
#else
   pjLibIntInitReq(ueCb->secInfo.secCxt, ueCb->secInfo.intInfo,&(ctxId));
   ueCb->secInfo.intCxtId    =  ctxId;
#endif /* PJ_SEC_ASYNC */

   RETVALUE(ret);

} /* end of pjUtlIntInit */

/**
 *
 * @brief 
 *
 *        Handler for opening the context with the Ciphering unit (either 
 *        synchronous or asynchronous) for SRBs of an UE.
 *
 * @b Description:
 *
 *        This function opens the context with the security algo unit per UE
 *        for ciphering / deciphering by key by sending the configured
 *        Ciphering Info with control plane key using synchronous / asynchronous 
 *        function calls with relevant input parameters.
 *
 *  @param[in]  pjRbCb     Pointer to RB control Block
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDlCpInit
(
PjCb     *gCb,
PjDlUeCb   *ueCb                    /* UE CB Ptr */
)
#else
PUBLIC S16 pjUtlDlCpInit(*gCb, *ueCb)
PjCb     *gCb;
PjDlUeCb   *ueCb;                   /* UE CB Ptr */
#endif
{
   Void *ctxId = NULLP;
#ifdef PJ_SEC_ASYNC
   U8  txIdx;
   U32 *libInitBitMask = NULLP;
#endif
   S16   ret = ROK;

   TRC3(pjUtlDlCpInit)

   RLOG2(L_DEBUG, "pjUtlCpInit(ueCb(%d,%d))",ueCb->key.ueId, ueCb->key.cellId);

#ifdef PJ_SEC_ASYNC

   if(ueCb->libInfo.state == PJ_STATE_REEST)
   {
      for(txIdx = 0; txIdx < PJ_MAX_ASYNC_CFM; txIdx++)
      {
         if((ueCb->libInfo.asyncCfm[txIdx] != NULLP) &&
               (ueCb->libInfo.asyncCfm[txIdx]->cfmType == PJ_REEST_ASYNC_CFM))
         {
            libInitBitMask =  &ueCb->libInfo.asyncCfm[txIdx]->libInitBitMask;
            break;
         }
      }
   }
   else if(ueCb->libInfo.state == PJ_STATE_NORMAL)
   {
      libInitBitMask =  &ueCb->libInfo.tLibInitBitMask;
   }

   if(libInitBitMask)
   {
      (*libInitBitMask) = (*libInitBitMask) | PJ_LIB_CP_CIP_BIT_MASK;
   }
   ret = PjLibObdCpInitReq(&gCb->pjGenCfg.obdPst.secPst, ueCb->secInfo.secCxt, 
         ueCb->secInfo.cipherInfo.algoType, ueCb->secInfo.cipherInfo.cpKey);
#else
   ret = pjLibCpInitReq(ueCb->secInfo.secCxt, (U8)ueCb->secInfo.cipherInfo.algoType,
                  ueCb->secInfo.cipherInfo.cpKey, &ctxId);    /*KW_FIX*/
   ueCb->secInfo.cpCxtId = ctxId;
#endif /* PJ_SEC_ASYNC */

   RETVALUE(ret);

} /* end of pjUtlCpInit */

/**
 *
 * @brief 
 *
 *        Handler for opening the context with the Ciphering unit (either 
 *        synchronous or asynchronous) for DRBs of an UE.
 *
 * @b Description:
 *
 *        This function opens the context with the security algo unit per UE
 *        for ciphering / deciphering by sending the configured
 *        Ciphering Info with user plane key using synchronous / asynchronous 
 *        function calls with relevant input parameters.
 *
 *  @param[in]  pjRbCb     Pointer to RB control Block
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDlUpInit
(
PjCb     *gCb,
PjDlUeCb   *ueCb                    /* UE CB Ptr */
)
#else
PUBLIC S16 pjUtlDlUpInit(*gCb,*ueCb) 
PjCb     *gCb;
PjDlUeCb   *ueCb;                   /* UE CB Ptr */
#endif
{

   Void  *ctxId = NULLP;
#ifdef PJ_SEC_ASYNC
   U8  txIdx;
   U32 *libInitBitMask = NULLP;
#endif
   S16 ret = ROK;

   TRC3(pjUtlDlUpInit)

   RLOG2(L_DEBUG, "pjUtlUpInit(ueCb(%d,%d))",ueCb->key.ueId, ueCb->key.cellId);

#ifdef PJ_SEC_ASYNC

   if(ueCb->libInfo.state == PJ_STATE_REEST)
   {
      for(txIdx = 0; txIdx < PJ_MAX_ASYNC_CFM; txIdx++)
      {
         if((ueCb->libInfo.asyncCfm[txIdx] != NULLP) &&
               (ueCb->libInfo.asyncCfm[txIdx]->cfmType == PJ_REEST_ASYNC_CFM))
         {
            libInitBitMask =  &ueCb->libInfo.asyncCfm[txIdx]->libInitBitMask;
            break;
         }
      }
   }
   else if(ueCb->libInfo.state == PJ_STATE_NORMAL)
   {
      libInitBitMask =  &ueCb->libInfo.tLibInitBitMask;
   }

   if(libInitBitMask)
      (*libInitBitMask) = (*libInitBitMask) | PJ_LIB_UP_CIP_BIT_MASK;

   ret = PjLibObdUpInitReq(&gCb->pjGenCfg.obdPst.secPst, ueCb->secInfo.secCxt, 
         ueCb->secInfo.cipherInfo.algoType, ueCb->secInfo.cipherInfo.upKey);
#else
   pjLibUpInitReq(ueCb->secInfo.secCxt, (U8)ueCb->secInfo.cipherInfo.algoType, 
                              ueCb->secInfo.cipherInfo.upKey, &ctxId);   /*KW_FIX*/
   ueCb->secInfo.upCxtId = ctxId;
#endif /* PJ_SEC_ASYNC */
   RETVALUE(ret);
} /* end of pjUtlUpInit */



/**
 *
 * @brief 
 *
 *        Handler for redirecing ciphering request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends ciphering protection request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb           PDCP RbCb 
 *  @param[in]   secInp           Input parameters for deciphering 
 *  @param[in]   mBuf             Data to be deciphered 
 *  @param[in]   opSdu            Deciphered SDU 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
//#ifdef TENB_AS_SECURITY
#ifdef ANSI
PUBLIC S16 pjUtlCipherReq
(
PjCb     *gCb,
PjDlRbCb   *pjRbCb,              /* PDCP RbCb */
U32      count,                  /* Input parameters for deciphering */ 
PjTxEnt   *txEnt,                   /* Data to be deciphered */
Buffer   **opSdu                 /* Deciphered SDU */
)
#else
PUBLIC S16 pjUtlCipherReq(gCb, pjRbCb, count, txEnt, opSdu)
PjCb     *gCb,
PjDlRbCb   *pjRbCb;               /* PDCP RbCb */
U32      count;                   /* Input parameters for deciphering */ 
PjTxEnt   *txEnt;                   /* Data to be deciphered */
Buffer   **opSdu;                 /* Deciphered SDU */
#endif
{
   S16 ret = ROK;
   PjDlUeCb *dlUeCb;
   PjLibTrans *libTrans; /* Transaction Id for deciphering */

   Void   *cxtId;    /* Context Identifier */

   TRC3(pjUtlCipherReq)

   if (pjRbCb->rbType == PJ_SRB)
   {
      cxtId = pjRbCb->ueCb->secInfo.cpCxtId;
   }
   else
   {
      cxtId = pjRbCb->ueCb->secInfo.upCxtId;
   } 

   PJ_ALLOC(gCb, libTrans , sizeof(PjLibTrans));
   if(libTrans == NULLP)
   {
      RsysSHMLog("PjLibObdIntProtReq failed line %d \n", __LINE__);
      QU_CRITLOG(QU_LOG_MESSAGE,"PjLibObdIntProtReq failed line %d ", __LINE__);
      RETVALUE(RFAILED);
   }
   
   dlUeCb = pjRbCb->ueCb;
   libTrans->pdcpInstance = gCb->init.inst;
   libTrans->count  = count;

   libTrans->rbId   = pjRbCb->rbId - 1;
   libTrans->dir = PJ_SEC_DIR_DL;
   libTrans->rbType = pjRbCb->rbType;
   libTrans->snLen = pjRbCb->snLen;
   libTrans->ciphAlgoType = dlUeCb->secInfo.cipherInfo.algoType;
   libTrans->ueCb = (PTR)dlUeCb;
   libTrans->ueId = dlUeCb->key.ueId;
   libTrans->cellId = dlUeCb->key.cellId;
   libTrans->txEnt  = (PTR)txEnt;
   if(PJ_SRB == pjRbCb->rbType)
   {
      libTrans->key = dlUeCb->secInfo.cipherInfo.cpKey;
   }
   else
   {
      libTrans->key = dlUeCb->secInfo.cipherInfo.upKey;
   }

#ifdef PJ_SEC_ASYNC
   /* Start the timer if it is not started already */
   if((pjChkTmr(gCb, (PTR)pjRbCb, PJ_EVT_DL_OBD_TMR)) == FALSE)
   {
      pjStartTmr(gCb, (PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }

   ret = PjLibObdCipherReq(&(gCb->pjGenCfg.obdPst.secPst), cxtId, 
                                             secInp, libTrans, txEnt->pdu);  

#else
      //QU_CRITLOG(QU_LOG_MESSAGE,"PDCP:Sending Data For Ciphering");
      ret = PjLibObdCipherReq(libTrans, txEnt->pdu, opSdu);
#endif

   RETVALUE(ret);

}


#ifdef FLAT_BUFFER_OPT
/**
 *
 * @brief 
 *
 *        Handler for redirecing ciphering request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends ciphering protection request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb           PDCP RbCb 
 *  @param[in]   secInp           Input parameters for deciphering 
 *  @param[in]   mBuf             Flat Buffer to be deciphered 
 *  @param[in]   opSdu            Deciphered SDU 
 *
 *  @return  S16
 *      -# ROK 
 *
 */

#ifdef ANSI
PUBLIC S16 pjUtlCipherReqFB
(
PjCb       *gCb,
PjDlRbCb   *pjRbCb,                /* PDCP RbCb */
U32        count,                  /* Input parameters for deciphering */ 
FlatBuffer *mBuf,                  /* Data to be deciphered */
Buffer     **opSdu                 /* Deciphered SDU */
)
#else
PUBLIC S16 pjUtlCipherReqFB(gCb, pjRbCb, count, mBuf, opSdu)
PjCb       *gCb,
PjDlRbCb   *pjRbCb;                /* PDCP RbCb */
U32        count;                  /* Input parameters for deciphering */ 
FlatBuffer *mBuf;                  /* Data to be deciphered */
Buffer     **opSdu;                /* Deciphered SDU */
#endif
{
   S16 ret = ROK;
   Void   *cxtId;    /* Context Identifier */
   PjDlUeCb *dlUeCb;
   PjLibTrans libTrans;

   TRC3(pjUtlCipherReqFB)

   cxtId = pjRbCb->ueCb->secInfo.upCxtId;

   dlUeCb = pjRbCb->ueCb;
   libTrans.pdcpInstance = gCb->init.inst;
   libTrans.count = count;

   libTrans.rbId = pjRbCb->rbId - 1;
   libTrans.dir = PJ_SEC_DIR_DL;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.snLen = pjRbCb->snLen;
   libTrans.ciphAlgoType = dlUeCb->secInfo.cipherInfo.algoType;
   libTrans.ueCb = (PTR)dlUeCb;
   libTrans.ueId = dlUeCb->key.ueId;
   libTrans.cellId = dlUeCb->key.cellId;
   if(PJ_SRB == pjRbCb->rbType)
   {
      libTrans.key = dlUeCb->secInfo.cipherInfo.cpKey;
   }
   else
   {
      libTrans.key = dlUeCb->secInfo.cipherInfo.upKey;
   }

#ifdef PJ_SEC_ASYNC
   /* Start the timer if it is not started already */
   if((pjChkTmr(gCb, (PTR)pjRbCb, PJ_EVT_DL_OBD_TMR)) == FALSE)
   {
      pjStartTmr(gCb, (PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }

   ret = PjLibObdCipherReq(&(gCb->pjGenCfg.obdPst.secPst), cxtId, 
                                             secInp, libTrans, mBuf);  

#else
      ret = PjLibObdCipherReqFB(&libTrans, mBuf, opSdu);
#endif

   RETVALUE(ret);

}
#endif


/**
 *
 * @brief 
 *
 *        Handler for redirecing Integration request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends Intergrity protection request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb       PDCP RbCb 
 *  @param[in]   secInp       Input parameters for integrity 
 *  @param[in]   mBuf         SDU to be compressed 
 *  @param[out]  macI         Message authentication code for the SDU 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlIntProtReq
(
PjCb     *gCb,
PjDlRbCb   *pjRbCb,             /* PDCP RbCb */
PjSecInp secInp,              /* Input parameters for integrity */ 
Buffer   **mBuf               /* SDU to be compressed */
)
#else
PUBLIC S16 pjUtlIntProtReq(gCb, pjRbCb, secInp, mBuf)
PjCb     *gCb;
PjDlRbCb   *pjRbCb;             /* PDCP RbCb */
PjSecInp secInp;              /* Input parameters for integrity */ 
Buffer   **mBuf;               /* SDU to be compressed */
#endif
{
   S16 ret = ROK;
   PjDlUeCb *dlUeCb;
   PjLibTrans *libTrans; /* Transaction Id for deciphering */
   Void   *cxtId;    /* Context Identifier */

   TRC3(pjUtlIntProtReq)

   cxtId = pjRbCb->ueCb->secInfo.intCxtId;

   PJ_ALLOC(gCb, libTrans , sizeof(PjLibTrans));
   if(libTrans == NULLP)
   {
      RsysSHMLog("PjLibObdIntProtReq failed line %d \n", __LINE__);
      RETVALUE(RFAILED);
   }
   
   dlUeCb = pjRbCb->ueCb;
   libTrans->pdcpInstance = gCb->init.inst;
   libTrans->count = secInp.count;
   libTrans->rbId = pjRbCb->rbId - 1;
   libTrans->dir = secInp.dir;
   libTrans->rbType = pjRbCb->rbType;
   /*pSecInfo.snLen = pjRbCb->snLen;*/
   libTrans->intAlgoType = dlUeCb->secInfo.intInfo.algoType;
   libTrans->ueCb = (PTR)dlUeCb;
   libTrans->ueId = dlUeCb->key.ueId;
   libTrans->cellId = dlUeCb->key.cellId;
   libTrans->key = &(dlUeCb->secInfo.intInfo.intKey[0]);
   PJ_SEC_FILL_FRESH(libTrans->fresh, libTrans->rbId);
#ifdef PJ_SEC_ASYNC
   /* Start the timer if it is not started already */
   if((pjChkTmr(gCb, (PTR)pjRbCb, PJ_EVT_DL_OBD_TMR)) == FALSE)
   {
      pjStartTmr(gCb, (PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }

   ret = PjLibObdIntProtReq(&(gCb->pjGenCfg.obdPst.secPst), cxtId, 
                                 secInp, libTrans, mBuf);  

#else
   ret = PjLibObdIntProtReq(libTrans, mBuf);
#endif

   RETVALUE(ret);

} /* end of pjUtlIntProtReq */

//#endif


/**
 *
 * @brief 
 *
 *        Handler for closing the DL context with the Ciphering unit (either 
 *        synchronous or asynchronous) for SRBs of an UE.
 *
 * @b Description:
 *
 *        This function closes an existing DL context with the security algo unit per UE
 *        for ciphering / deciphering with control plane key using synchronous / asynchronous 
 *        function calls with relevant input parameters.
 *
 *
 *  @param[in] cpCxtId          Context Id for Ciphering to be closed 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI 
PUBLIC S16 pjUtlDlCipherClose
(
PjCb     *gCb,
Void     *cpCxtId                  /* Context Id for Ciphering to be closed */
)
#else
PUBLIC S16 pjUtlDlCipherClose(cpCxtId)
PjCb     *gCb;
Void     *cpCxtId;                 /* Context Id for Ciphering to be closed */
#endif
{
   TRC3(pjUtlDlCipherClose)

   RLOG0(L_DEBUG, "pjUtlDlCipherClose");

#ifndef PJ_SEC_ASYNC
   PjLibObdCipherCloseReq(cpCxtId);
#else
   PjLibObdCipherCloseReq(&gCb->pjGenCfg.obdPst.secPst, cpCxtId);
#endif

   RETVALUE(ROK);

} /* end of pjUtlCipherClose */


/**
 *
 * @brief 
 *
 *        Handler for closing the DL context with the Integration unit (either 
 *        synchronous or asynchronous) for all RBs of an UE.
 *
 * @b Description:
 *
 *        This function closes an existing DL context with the security algo unit per UE
 *        for Integration protection/verification using synchronous / asynchronous function 
 *        calls with relevant input parameters.
 *
 *
 *  @param[in] intCxtId        Integration CxtId to be closed 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI 
PUBLIC S16 pjUtlDlIntClose
(
PjCb     *gCb,
Void      *intCxtId                 /* Integration CxtId to be closed */
)
#else
PUBLIC S16 pjUtlDlIntClose(intCxtId)
PjCb     *gCb;
Void      *intCxtId;                /* Integration CxtId to be closed */
#endif
{
   TRC3(pjUtlDlIntClose)

   RLOG0(L_DEBUG, "pjUtlDlIntClose");

#ifdef PJ_SEC_ASYNC
   PjLibObdIntCloseReq(&gCb->pjGenCfg.obdPst.secPst,intCxtId);
#else   
   PjLibObdIntCloseReq(intCxtId);
#endif

   RETVALUE(ROK);

} /* end of pjUtlIntClose */


/**
 *
 * @brief 
 *
 *        Handler for resetting the context with the compression unit (either 
 *        synchronous or asynchronous) for RB within an UE.
 *
 * @b Description:
 *
 *        This function resets the context with the compression unit per RB per
 *        UE by using synchronous / asynchronous function calls.
 *
 *
 *  @param[in] cmpCxtId     Context to be reset for compression 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDlCmpReset
(
PjCb        *gCb,
PjDlRbCb      *pjRbCb                 /* Context to be reset for compression */
)
#else
PUBLIC S16 pjUtlDlCmpReset(pjRbCb) 
PjCb        *gCb;
PjDlRbCb      *pjRbCb;                 /* Context to be reset for compression */
#endif
{
   TRC3(pjUtlDlCmpReset)  

   RLOG2(L_DEBUG, "pjUtlCmpReset(pjRbCb(%d,%d))",pjRbCb->rbId, pjRbCb->rbType);

#ifdef PJ_CMP_ASYNC
   PjLibObdCmpResetReq(&gCb->pjGenCfg.obdPst.cmpPst, pjRbCb->cmpCxtId);
#else
   pjLibCmpResetReq(pjRbCb->cmpCxtId);
#endif

   RETVALUE(ROK);

} /* end of pjUtlCmpReset */


/**
 *
 * @brief 
 *        Handle re-establishment on SRB1. 
 *
 * @b Description:
 *        This function 
 *        1. Marks the state of the RBs as PJ_STATE_REEST.
 *        2. Calculates the number of RBs undergoing reestablishment.
 *
 *  @param[in] gCb     PDCP Instance control block
 *  @param[in] pjRbCb  Downlink Rb control block 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDlReEstSrb1
(
PjCb       *gCb,
PjDlRbCb   *pjRbCb                    /* PDCP Control Block Pointer */
)
#else
PUBLIC S16 pjUtlDlReEstSrb1(gCb, pjRbCb)
PjCb       *gCb;
PjDlRbCb   *pjRbCb;                   /* PDCP Control Block Pointer */
#endif
{
   PjDlUeCb   *ueCb;

   TRC2(pjUtlDlReEstSrb1)

   RLOG2(L_DEBUG, "pjUtlReEstSrb1(pjRbCb(%d,%d))",pjRbCb->rbId, pjRbCb->rbType);

   ueCb = pjRbCb->ueCb;

   ueCb->libInfo.state  =  PJ_STATE_REEST;

   /* SRB Reconfiguration we can update the control Plane keys and
      the Integrity Keys in the OBD module */

   pjUtlDlUpdCpSecKeys(gCb,ueCb);

   /* Perform downlink reestablishment of SRB1 */
   pjDlmReEstSrb(gCb,pjRbCb);

   /*  If Only SRB1 configured then we can update userplane 
       keys also */
   if ((ueCb->numSrbs == 1) && (ueCb->numDrbs == 0))
   {
      pjUtlDlUpdUpSecKeys(gCb,ueCb);
   }

   RETVALUE(ROK);
} /* pjUtlReEstSrb1 */


/**
 *
 * @brief 
 *
 *        Handler to start reestablishment processing
 *
 * @b Description:
 *
 *        This function 
 *        1. Marks the state of the RBs as PJ_STATE_REEST_HO.
 *        2. Calculates the number of RBs undergoing reestablishment.
 *
 *  @param[in] ueCb    Number of RBs undergoing reestablishment
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDlReEstHO
(
PjCb              *gCb,
PjDlUeCb          *ueCb           /* Number of RBs undergoing reestablishment*/ 
)
#else
PUBLIC S16 pjUtlDlReEstHO(gCb,ueCb)    
PjCb              *gCb;
PjDlUeCb          *ueCb;           /* Number of RBs undergoing reestablishment*/ 
#endif
{
   U8             rbCnt;
   PjDlRbCb     **rbCbLst;
   PjDlRbCb     * pjRbCb;

   TRC2 (pjUtlDlReEstHO); 

#ifdef ALIGN_64BIT
   RLOG2(L_DEBUG, "pjUtlReEstHO(ueCb(%d,%d))",ueCb->key.ueId, ueCb->key.cellId);
#else
   RLOG2(L_DEBUG, "pjUtlReEstHO(ueCb(%d,%d))",ueCb->key.ueId, ueCb->key.cellId);
#endif

   /* Initialisations */
   ueCb->libInfo.numReEstDrb = 0;
   ueCb->libInfo.state = PJ_STATE_REEST_HO;

   /* Perform initialisations on the HO Info structure */
   PJ_ALLOC(gCb,ueCb->hoInfo, sizeof(PjDlHoInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ueCb->hoInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   PJ_ALLOC(gCb,ueCb->hoInfo->hoCfmInfo, sizeof(PjDlHoCfmInfo) *  PJ_MAX_DRB_PER_UE );
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ueCb->hoInfo->hoCfmInfo == NULLP )
   {
      /*ccpu00136858 */      
      PJ_PST_FREE(gCb->u.dlCb->udxDlSap[0].pst.region,
                  gCb->u.dlCb->udxDlSap[0].pst.pool, 
                           ueCb->hoInfo, sizeof(PjDlHoInfo));
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   /* Initialise the reestablishment states for the RBs */
   rbCbLst      = ueCb->drbCb;
   for(rbCnt = 0; rbCnt < PJ_MAX_DRB_PER_UE; rbCnt ++)
   {
      /* Check of rbCb available */
      if( (pjRbCb = rbCbLst[rbCnt]) == NULLP)
      {
         continue;
      }

      /* Fetch the rbCb and intialise the states */
      pjRbCb->state  =  PJ_STATE_REEST_HO;
      
      if(pjRbCb->mode == PJ_DRB_AM)
      {
         pjDlmReEstHoDrbAm(gCb, pjRbCb);
      }

   }/* for(rbCnt .. */ 

   RETVALUE(ROK);
} /* pjUtlReEstHO */


/**
 *
 * @brief 
 *
 *        Called after the new security algorithms have been applied
 *        and header compression/decompression reset. It starts
 *        reestablishment procedures for downlink DRBs.
 *
 * @b Description:
 *
 *        This function 
 *        1. Creates new contexts for Ciphering and Integrity.
 *        2. Closes the old contexts for Ciphering and Integrity.
 *        3. Resets the ROHC unit.
 *
 *  @param[in] ueCb   UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlReEstDl
(
PjCb       *gCb,
PjDlUeCb   *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlReEstDl(gCb, ueCb)
PjCb       *gCb;
PjDlUeCb   *ueCb;           /* UE Control Block */ 
#endif
{
   TRC2(pjUtlReEstDl);

   RLOG2(L_DEBUG, "pjUtlReEstDl(ueCb(%d,%d))",ueCb->key.ueId, ueCb->key.cellId);


   /* Send Cfms and update the state  only 
      when there is some cfgreq cfm is present for other RBs*/
   if(ueCb->libInfo.state == PJ_STATE_REEST)
   {
      pjUtlDlSndReEstCfgCfm(gCb, ueCb);
   }
   else if(ueCb->libInfo.state == PJ_STATE_REEST_HO)
   {
      pjUtlDlSndSduStaCfm(gCb, ueCb);
   }
   ueCb->libInfo.state = PJ_STATE_NORMAL;

   RETVALUE(ROK);

} /* pjUtlReEstDl */




/**
 *
 * @brief 
 *
 *        Function to sent the REESTABLISHMENT COMPPLETE 
 *        to the RRC.
 *
 * @b Description:
 *
 *        This function 
 *        1. Sends a ReEstablishment Confirm for normal reestablishment.
 *        2. Sends a SDU status confirm for handover reestablishment.
 *
 *  @param[in] ueCb   UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDlSndReEstCfgCfm
(
PjCb         *gCb,
PjDlUeCb       *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlDlSndReEstCfgCfm(ueCb)
PjCb         *gCb;
PjDlUeCb       *ueCb;           /* UE Control Block */ 
#endif
{
   U8             rbCnt;
   PjDlRbCb        *tRbCb;
   CpjCfgCfmInfo *cfgCfmInfo;
   U8             txIdx;
   PjAsyncCfm    *asyncCfm    = NULLP;
   PjUdxDlSapCb *udxSap;

   TRC2(pjUtlDlSndReEstCfgCfm);

   RLOG2(L_DEBUG, "pjUtlSndReEstCfgCfm(ueCb(%d,%d))",ueCb->key.ueId, ueCb->key.cellId);

   ueCb->libInfo.state  =  PJ_STATE_NORMAL;

   /* Pick up the confirm info */
   for(txIdx = 0; txIdx < PJ_MAX_ASYNC_CFM; txIdx++)
   {
      if((ueCb->libInfo.asyncCfm[txIdx] != NULLP) &&
         (ueCb->libInfo.asyncCfm[txIdx]->cfmType & PJ_CFG_REEST_ASYNC_CFM))
      {
         asyncCfm =  ueCb->libInfo.asyncCfm[txIdx];
         break;
      }
   }

   /* its a reestablishment with only srb1 configured */
   if(asyncCfm == NULLP)
   {
      RETVALUE(ROK);
   }

   for(rbCnt = 0; rbCnt < PJ_MAX_SRB_PER_UE; rbCnt ++)
   {
      if( ((tRbCb = ueCb->srbCb[rbCnt]) == NULLP)    ||
          (tRbCb->state ==  PJ_STATE_NORMAL))
      {
         continue;
      }

      /* Fetch the rbCb and intialise the states */
      tRbCb->state  =  PJ_STATE_NORMAL;
   } 
   
   /* Memory leak fix ccpu00135359 */
   udxSap    = &(gCb->u.dlCb->udxDlSap[0]);
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&cfgCfmInfo,
      sizeof (CpjCfgCfmInfo)) != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   PJ_FILL_REEST_CFG_CFM_INFO(cfgCfmInfo, asyncCfm);
   PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

   PjDlUdxCfgCfm(&gCb->u.dlCb->udxDlSap[0].pst, 
                  gCb->u.dlCb->udxDlSap[0].suId, 
                   (UdxCfgCfmInfo *)cfgCfmInfo);         

   RETVALUE(ROK);
} /* pjUtlSndReEstCfgCfm */


/**
 *
 * @brief Handler to store the confirms while we wait for the init
 *        confirms from the off-board.
 *       
 *
 * @b Description
 *        This function is invoked during security config request,
 *        reestablishment request and config request if the libraries
 *        are off-boarded. This stores the entries in a confirm list
 *        while waiting for the confirm from the offboard.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/

#ifdef ANSI
PUBLIC S16 pjUtlDlSaveCfmInfo
(
PjCb        *gCb,
PjDlUeCb      *ueCb,           /* Pointer to UeCb */ 
U8          cfmType,         /* Confirm type */
Bool        startTmr,        /* Start Timer */
U8          entity,          /* Confirm type */
PTR         cfmPtr,          /* Pointer to the structure for confirm */
PTR         cfgPtr           /* Pointer to the structure for request */
)
#else
PUBLIC S16 pjUtlDlSaveCfmInfo(ueCb, cfmType, startTmr, entity, cfmPtr, cfgPtr)
PjCb        *gCb;
PjDlUeCb      *ueCb;           /* Pointer to UeCb */ 
U8          cfmType;         /* Confirm type */
Bool        startTmr;        /* Start Timer */
U8          entity;          /* Confirm type */
PTR         cfmPtr;          /* Pointer to the structure for confirm */
PTR         cfgPtr;          /* Pointer to the structure for request */
#endif
{
   S16               ret;        /* Return value            */
   CpjSecCfgCfmInfo  *secCfm;    /* Security confirm        */
   CpjReEstCfmInfo   *reEstCfm;  /* Reestablishment confirm */
   CpjCfgCfmInfo     *cfgCfm;    /* Config confirm          */
   U16               txId;       /* Transaction Id          */
   PjAsyncCfm        *tAsyncCfm; /* Pointer to the async Cfm*/
   CpjCfgReqInfo     *cfgReq;    /* Pointer to the config request */
   U8                cfmIdx;     /* index for loopoing */


   TRC3(pjUtlDlSaveCfmInfo) 

   RLOG_ARG4(L_DEBUG,DBG_UEID,ueCb->key.ueId, 
         "pjUtlSaveCfmInfo cellId(%d),cfmType(%d),startTmr(%d),entity(%d))",
         ueCb->key.cellId, cfmType, startTmr, entity);

   /* Initialisations */
   ret      = ROK;
   secCfm   = NULLP;
   reEstCfm = NULLP;
   cfgCfm   = NULLP;
   cfgReq   = NULLP;
   txId     = ueCb->libInfo.nxtAvlbTxId;

   if(txId == PJ_MAX_ASYNC_CFM)
   {
      /* The entire list is full, have to return negative confirms */
      RETVALUE(RFAILED);
   }

   /* is entry already present */
   if(ueCb->libInfo.asyncCfm[txId] != NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Allocate the structure */
   PJ_ALLOC(gCb,ueCb->libInfo.asyncCfm[txId],  sizeof(PjAsyncCfm));
   if(ueCb->libInfo.asyncCfm[txId]== NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Initialise the structure */
   tAsyncCfm   =   ueCb->libInfo.asyncCfm[txId];
   tAsyncCfm->libInitBitMask   =   ueCb->libInfo.tLibInitBitMask;
   tAsyncCfm->cmpInitBitMask   =   ueCb->libInfo.tCmpInitBitMask;
   tAsyncCfm->entity           =   entity;
   tAsyncCfm->startTmr         =   startTmr;

   ueCb->libInfo.tLibInitBitMask =  0; 
   ueCb->libInfo.tCmpInitBitMask =  0; 
   
   /* Assign the values */
   if(cfmType  ==   PJ_SEC_ASYNC_CFM)
   {
      secCfm   =  (CpjSecCfgCfmInfo *)cfmPtr;
      tAsyncCfm->cfmType =  cfmType;
      tAsyncCfm->ueId    = secCfm->ueId;
      tAsyncCfm->cellId  = secCfm->cellId;
      tAsyncCfm->transId = secCfm->transId;
   }
   else if(cfmType  ==   PJ_REEST_ASYNC_CFM)
   {
      reEstCfm   =  (CpjReEstCfmInfo *)cfmPtr;
      tAsyncCfm->cfmType =  cfmType;
      tAsyncCfm->ueId    = reEstCfm->ueId;
      tAsyncCfm->cellId  = reEstCfm->cellId;
      tAsyncCfm->transId = reEstCfm->transId;
   }
   else if((cfmType  & PJ_CFG_ASYNC_CFM)  ||
           (cfmType  & PJ_CFG_REEST_ASYNC_CFM)||
           (cfmType  & PJ_CFG_UEDEL_ASYNC_CFM))
   {
      if(entity   == ENTPJ)
      {
         cfgCfm   =  (CpjCfgCfmInfo *)cfmPtr;
         cfgReq   =  (CpjCfgReqInfo *)cfgPtr;

         tAsyncCfm->cfmType = cfmType;
         tAsyncCfm->ueId    = cfgCfm->ueId;
         tAsyncCfm->cellId  = cfgCfm->cellId;
         tAsyncCfm->transId = cfgCfm->transId;
         tAsyncCfm->numEnt  = cfgReq->numEnt;

         for ( cfmIdx = 0; cfmIdx < cfgReq->numEnt; cfmIdx++ )
         {
            tAsyncCfm->cfmEnt[cfmIdx].status  = cfgCfm->cfmEnt[cfmIdx].status;
            tAsyncCfm->cfmEnt[cfmIdx].reason  = cfgCfm->cfmEnt[cfmIdx].reason;
            tAsyncCfm->cfmEnt[cfmIdx].rbId    = cfgCfm->cfmEnt[cfmIdx].rbId;
            tAsyncCfm->cfmEnt[cfmIdx].rbType  = cfgCfm->cfmEnt[cfmIdx].rbType;
            tAsyncCfm->cfmEnt[cfmIdx].cfgType = cfgReq->cfgEnt[cfmIdx].cfgType;
         }
      }
      else if(entity == ENTPJ)
      {
         CpjCfgEnt     *pjCfg;
         CpjCfgCfmInfo  *pjCfgCfm;       /* Configuraiton Confirm */

         pjCfgCfm   =  (CpjCfgCfmInfo *)cfmPtr;
         pjCfg      =  (CpjCfgEnt *)cfgPtr;

         tAsyncCfm->cfmType = cfmType;
         tAsyncCfm->transId = pjCfgCfm->transId;
         tAsyncCfm->ueId    = pjCfgCfm->ueId;
         tAsyncCfm->cellId  = pjCfgCfm->cellId;
         tAsyncCfm->numEnt  = pjCfgCfm->numEnt;

         for ( cfmIdx = 0; cfmIdx < pjCfgCfm->numEnt; cfmIdx++ )
         {
            tAsyncCfm->cfmEnt[cfmIdx].status  = 
                                 pjCfgCfm->cfmEnt[cfmIdx].status;
            tAsyncCfm->cfmEnt[cfmIdx].reason  = 
                                 pjCfgCfm->cfmEnt[cfmIdx].reason;
            tAsyncCfm->cfmEnt[cfmIdx].rbId    = 
                                 pjCfgCfm->cfmEnt[cfmIdx].rbId;
            tAsyncCfm->cfmEnt[cfmIdx].rbType  = 
                                 pjCfgCfm->cfmEnt[cfmIdx].rbType;
         }
      }
   }

   /* Start timer */
   /* Timer should not be started when SEC is SYNC for ReEst Req
    * and for the subsequent config req for REEST. The config req
    * after reest req can still start the timer in the case comp being
    * async and sec being sync if it also adds a RB and a compInit has
    * to be sent . 
    * */

   if(tAsyncCfm->startTmr  == TRUE)
   {
      if((pjChkTmr(gCb,(PTR)ueCb, PJ_EVT_OBD_WAIT_TMR)) == FALSE)
      {
         pjStartTmr(gCb,(PTR)ueCb, PJ_EVT_OBD_WAIT_TMR);
         ueCb->libInfo.crntTmrTxId  =  ueCb->libInfo.nxtAvlbTxId;
      }
   }

   /* update nxtAvlbTxId to the next empty slot */
   for(cfmIdx = 0; cfmIdx < PJ_MAX_ASYNC_CFM; cfmIdx++)
   {
      ueCb->libInfo.nxtAvlbTxId = 
        (U16)((ueCb->libInfo.nxtAvlbTxId + 1) % PJ_MAX_ASYNC_CFM);  /*KW_FIX*/

      if(ueCb->libInfo.asyncCfm[ueCb->libInfo.nxtAvlbTxId] == NULLP)
      {
         break;
      }
   }

   /* Cant find an empty slot ? */
   if(cfmIdx == PJ_MAX_ASYNC_CFM)
   {
      ueCb->libInfo.nxtAvlbTxId = PJ_MAX_ASYNC_CFM;
   }

   RETVALUE(ret);
}/* pjUtlSaveCfmInfo */

/**
 *
 * @brief Handler to store update the bitmaks and send confirms if necessary.
 *       
 *
 * @b Description
 *        This function is invoked when receiving a InitCfm from Offboard.
 *        It updates the bit masks and checks if it is necesary to send a 
 *        confirm.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/

#ifdef ANSI
PUBLIC S16 pjUtlDlHdlObdInitCfm
(
PjCb        *gCb,
PjDlUeCb      *ueCb,           /* Pointer to UeCb */ 
U16         txIdx,           /* Transaction Index for UeCb */
U8          cfmType,         /* Confirm type */
U8          maskVal          /* mask value */
)
#else
PUBLIC S16 pjUtlDlHdlObdInitCfm(ueCb, txIdx, cfmType, maskVal)
PjCb        *gCb;
PjDlUeCb      *ueCb;           /* Pointer to UeCb */ 
U16         txIdx;           /* Transaction Index for UeCb */
U8          cfmType;         /* Confirm type */
U8          maskVal;         /* mask value */
#endif
{
   UdxSecCfgCfmInfo *secCfgCfm;   /* Security config confirm */
   UdxReEstCfmInfo  *reEstCfm;    /* Reest config confirm */
   PjAsyncCfm       *asyncCfm;
   UdxCfgCfmInfo    *cfgCfm;
   U8               idx;
   S16              ret;

   TRC3(pjUtlDlHdlObdInitCfm)

   RLOG_ARG4(L_DEBUG,DBG_UEID,ueCb->key.ueId, 
         "pjUtlHdlDlObdInitCfm cellId(%d), txIdx(%d), cfmType(%d), maskVal(%d))",
          ueCb->key.cellId, txIdx, cfmType, maskVal);

   /* Initialisations */
   secCfgCfm   =  NULLP;
   reEstCfm    =  NULLP;
   cfgCfm      =  NULLP;
   asyncCfm    =  NULLP;
   ret         =  ROK;

   if((txIdx  == PJ_MAX_ASYNC_CFM) ||
      (ueCb->libInfo.asyncCfm[txIdx] == NULLP))
   {
      RETVALUE(RFAILED);
   }

   asyncCfm =  ueCb->libInfo.asyncCfm[txIdx];

   if(cfmType & PJ_SEC_INIT_CFM)
   {
      /* Update bit mask and check if all confirms have been received */
      asyncCfm->libInitBitMask ^= (maskVal);
      ret = pjUtlDlHdlSecInitCfm(gCb,ueCb,txIdx,asyncCfm,secCfgCfm,reEstCfm);
   }
   else if(cfmType  & PJ_CMP_INIT_CFM)
   {
      /* Check if all confirms have been received */
      asyncCfm->cmpInitBitMask &= ~(1 << (maskVal - 1));

      /* Update the status and reason for the received CmpInitCfm */
      for ( idx = 0; idx < asyncCfm->numEnt; idx++ )
      {
         if ( asyncCfm->cfmEnt[idx].rbId == maskVal)
         {
            asyncCfm->cfmEnt[idx].status = CPJ_CFG_CFM_OK;
            asyncCfm->cfmEnt[idx].reason = CPJ_CFG_REAS_NONE;
            break;
         }
      }

      /* Check if we can send confirm */
      if(asyncCfm->cmpInitBitMask == 0)
      {
         ret = pjUtlDlHdlCmpInitCfm(gCb,ueCb,txIdx,asyncCfm,cfgCfm);
      }
   }
   RETVALUE(ret);
}/* pjUtlHdlDlObdInitCfm */

/**
 *
 * @brief Handler for init off-board timer expiry.
 *
 *
 * @b Description
 *        This function is called when the off-board timer expires for
 *        a Init Req of a channel (either for ciphering/integrity/compression).
 *        This function sends a confirm with failure for SecCfgReq if the
 *        state of the RB is normal. It sends a confirm with failure
 *        for config request(only for RBs with Re-establishment Req, a failure 
 *        is stored in the confirm) if the state of the RB is re-establishment.
 *
 *  @param[in] ueCb    UE control block.
 *
 *
 *  @return  S16
 *      -# ROK
 */

#ifdef ANSI
PUBLIC S16 pjUtlDlHdlInitObdTmrExp
(
PjCb     *gCb,
PjDlUeCb *ueCb
)
#else
PUBLIC S16 pjUtlDlHdlInitObdTmrExp(ueCb)
PjCb     *gCb;
PjDlUeCb *ueCb;
#endif
{
   S16               ret;           /* Return Value */
   UdxSecCfgCfmInfo  *secCfgCfm;    /* Security Cfg Confirm Info */
   UdxCfgCfmInfo     *cfgCfm;       /* Config Confirm Info */
   UdxReEstCfmInfo   *reEstCfm;     /* Reest config confirm */
   UdxCfgCfmInfo     *pjCfgCfm;
   PjLibInfo         *libInfo;      /* Off-board Info */
   PjAsyncCfm        *asyncCfm;     /* Async Cfm Info */
   PjUdxDlSapCb      *udxSap;
   U16               txIdx;         /* Tx Idx */
   U16               idx;           /* Index for looping */
   U16               cfgIdx;

   TRC3(pjUtlDlHdlInitObdTmrExp)

   RLOG2(L_DEBUG, "pjUtlDlHdlInitObdTmrExp(ueCb(%d,%d))",
         ueCb->key.ueId, ueCb->key.cellId);

   ret       = ROK;
   udxSap    = &(gCb->u.dlCb->udxDlSap[0]);
   secCfgCfm = NULLP;
   libInfo   = &ueCb->libInfo;
   txIdx     = libInfo->crntTmrTxId;

   /* Restart the timer */
   PJ_CHK_RESTART_OBD_TIMER(gCb,ueCb, txIdx);

   /* Shouldnt happen, nevertheless, check */
   if( txIdx >= PJ_MAX_ASYNC_CFM || libInfo->asyncCfm[txIdx] == NULLP)
   {
      RETVALUE(ROK);
   }

   asyncCfm =  libInfo->asyncCfm[txIdx];

   /* Security confirm */
   if(asyncCfm->cfmType & PJ_SEC_ASYNC_CFM)
   {
      /* Memory leak fix ccpu00135359 */
      if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&secCfgCfm,
          sizeof (UdxSecCfgCfmInfo)) != ROK)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         RETVALUE(RFAILED);
      }
      PJ_FILL_SEC_CFM_INFO(secCfgCfm, asyncCfm);
      secCfgCfm->status = CPJ_CFG_CFM_NOK;
      secCfgCfm->reason = CPJ_CFG_REAS_OBD_TIMEOUT;

      PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

      PjDlUdxSecCfgCfm((&udxSap->pst), udxSap->suId, secCfgCfm);
   }

   /* Configuration confirms */
   else if ((asyncCfm->cfmType & PJ_CFG_ASYNC_CFM) ||
            (asyncCfm->cfmType & PJ_CFG_REEST_ASYNC_CFM))
   {
      PJ_FILL_TIMEOUT_CFG_CFM_INFO(gCb,cfgCfm, asyncCfm);

      PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

      PjDlUdxCfgCfm(&(gCb->u.dlCb->udxDlSap[0].pst), 
                         gCb->u.dlCb->udxDlSap[0].suId, 
                        cfgCfm);
   }
   else if (asyncCfm->cfmType & PJ_REEST_ASYNC_CFM)
   {
      /* Send ReEstCfm */
      /* Memory leak fix ccpu00135359 */
      if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&reEstCfm,
          sizeof (UdxReEstCfmInfo)) != ROK)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         RETVALUE(RFAILED);
      }
      PJ_FILL_REEST_CFM_INFO(reEstCfm, asyncCfm, LCM_PRIM_NOK);

      PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

      PjDlUdxReEstCfm(&udxSap->pst, 
                       udxSap->suId, reEstCfm);

      /* Send the config confirm also if any exists */
      cfgIdx   = (U16)( txIdx + 1); /*KW_FIX*/

      for(idx = 0; idx < PJ_MAX_ASYNC_CFM; idx++, cfgIdx++)
      {
         cfgIdx %=   PJ_MAX_ASYNC_CFM;
         if((libInfo->asyncCfm[cfgIdx] != NULLP) &&
            (libInfo->asyncCfm[cfgIdx]->cfmType & PJ_CFG_REEST_ASYNC_CFM))
         {
            break;
         }
      }

      /* This scenario occurs when only SRB1 is configured */
      if(idx == PJ_MAX_ASYNC_CFM)
      {
         RETVALUE(ROK);
      }

      PJ_CHK_RESTART_OBD_TIMER(gCb,ueCb, cfgIdx);
      asyncCfm =  libInfo->asyncCfm[cfgIdx];

      PJ_FILL_TIMEOUT_CFG_CFM_INFO(gCb,cfgCfm, asyncCfm);

      PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

      PjDlUdxCfgCfm(&gCb->u.dlCb->udxDlSap[0].pst, 
                  gCb->u.dlCb->udxDlSap[0].suId, cfgCfm);         
   }
   else if(asyncCfm->cfmType & PJ_CFG_UEDEL_ASYNC_CFM)
   {
      /* Delete ueCb entry from ueLstCp */
      pjStopTmr(gCb,(PTR)ueCb, PJ_EVT_OBD_WAIT_TMR);
      ret = cmHashListDelete(&(gCb->u.ulCb->ueLstCp), (PTR) ueCb);
      if (ret != ROK)
      {

#ifdef DEBUGP
         RLOG_ARG0(L_ERROR, DBG_UEID,ueCb->key.ueId, 
               "Hash List Delete Failed for DL ueCb");
#endif
      }

      if ( asyncCfm->entity == ENTPJ )
      {
         /* Memory leak fix ccpu00135359 */
         if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&cfgCfm,
             sizeof (UdxCfgCfmInfo)) != ROK)
         {
            RLOG0(L_FATAL, "Memory Allocation failed.");
            RETVALUE(RFAILED);
         }

         for ( idx = 0; idx < asyncCfm->numEnt; idx++)
         {
            cfgCfm->cfmEnt[idx].status = asyncCfm->cfmEnt[idx].status;
            cfgCfm->cfmEnt[idx].reason = asyncCfm->cfmEnt[idx].reason;
            cfgCfm->cfmEnt[idx].rbId   = asyncCfm->cfmEnt[idx].rbId;
            cfgCfm->cfmEnt[idx].rbType = asyncCfm->cfmEnt[idx].rbType;
         }
         cfgCfm->ueId    = asyncCfm->ueId;
         cfgCfm->cellId  = asyncCfm->cellId;
         cfgCfm->transId = asyncCfm->transId;
         cfgCfm->numEnt  = asyncCfm->numEnt;

         /* delete all confirms present there */
         for(idx = 0; idx < PJ_MAX_ASYNC_CFM; idx++)
         {
            if(libInfo->asyncCfm[idx] != NULLP)
            {
               PJ_FREE(gCb,ueCb->libInfo.asyncCfm[idx], sizeof(PjAsyncCfm)); 
               ueCb->libInfo.asyncCfm[idx] = NULLP; 
            }
         }

         PJ_FREE(gCb,ueCb, sizeof(PjDlUeCb));
         PjDlUdxCfgCfm(&(udxSap->pst), udxSap->suId, cfgCfm);
      }
      else if ( asyncCfm->entity == ENTPJ )
      {
         /* Memory leak fix ccpu00135359 */
         if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&pjCfgCfm,
             sizeof (UdxCfgCfmInfo)) != ROK)
         {
            RLOG0(L_FATAL, "Memory Allocation failed.");
            RETVALUE(RFAILED);
         }
         for ( idx = 0; idx < asyncCfm->numEnt; idx++)
         {
            pjCfgCfm->cfmEnt[idx].status = asyncCfm->cfmEnt[idx].status;
            pjCfgCfm->cfmEnt[idx].reason = asyncCfm->cfmEnt[idx].reason;
            pjCfgCfm->cfmEnt[idx].rbId          = asyncCfm->cfmEnt[idx].rbId;
            pjCfgCfm->cfmEnt[idx].rbType        = asyncCfm->cfmEnt[idx].rbType;
         }
         pjCfgCfm->ueId    = asyncCfm->ueId;
         pjCfgCfm->cellId  = asyncCfm->cellId;
         pjCfgCfm->transId = asyncCfm->transId;
         pjCfgCfm->numEnt  = asyncCfm->numEnt;

         /* delete all confirms present there */
         for(idx = 0; idx < PJ_MAX_ASYNC_CFM; idx++)
         {
            if(libInfo->asyncCfm[idx] != NULLP)
            {
               PJ_FREE(gCb,ueCb->libInfo.asyncCfm[idx], sizeof(PjAsyncCfm)); 
               ueCb->libInfo.asyncCfm[idx] = NULLP; 
            }
         }

         PJ_FREE(gCb,ueCb, sizeof(PjDlUeCb));
         PjDlUdxCfgCfm(&(udxSap->pst), udxSap->suId, pjCfgCfm);
      }
   }

   RETVALUE(ret);
} /* pjHdlUeDelWaitTmrExp */


/**
 *
 * @brief 
 *
 *        Function to sent the REESTABLISHMENT COMPPLETE 
 *        to the RRC.
 *
 * @b Description:
 *
 *        This function 
 *        1. Sends a ReEstablishment Confirm for normal reestablishment.
 *        2. Sends a SDU status confirm for handover reestablishment.
 *
 *  @param[in] ueCb   UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlDlSndSduStaCfm
(
PjCb         *gCb,
PjDlUeCb       *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlDlSndSduStaCfm(gCb, ueCb) 
PjCb         *gCb;
PjDlUeCb       *ueCb;           /* UE Control Block */ 
#endif
{
   UdxSduStaCfmInfo  *cfmInfo; 
   PjUdxDlSapCb     *udxSap;

   TRC2(pjUtlDlSndSduStaCfm); 

   RLOG2(L_DEBUG, "pjUtlSndSduStaCfm(ueCb(%d,%d))",ueCb->key.ueId, ueCb->key.cellId);

   /* Perform processing for filling up cfm structures */
   /* Memory leak fix ccpu00135359 */
   udxSap    = &(gCb->u.dlCb->udxDlSap[0]);
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&cfmInfo,
      sizeof (UdxSduStaCfmInfo)) != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
   PJ_FILL_DL_SDU_CFM_INFO(gCb,cfmInfo, ueCb);

   PjDlUdxSduStaCfm(&gCb->u.dlCb->udxDlSap[0].pst, 
                     gCb->u.dlCb->udxDlSap[0].suId, 
                     cfmInfo); 

   RETVALUE(ROK);

} /* pjUtlSndSduStaCfm */

/**
 *
 * @brief Handler to clean up all the PDCP Control Blocks.
 *       
 *
 * @b Description
 *        This function is invoked by LMM to shutdown the
 *        layer. This cleans up recBuf and txBuf of all the
 *        PDCP RBs in all the UEs.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/

#ifdef ANSI
PUBLIC S16 pjUtlDlShutdown
(
PjCb  *gCb
)
#else
PUBLIC S16 pjUtlDlShutdown()
PjCb   *gCb;
#endif
{
   S16         ret;              /* Return Value */
   PjDlUeCb      *ueCb;            /* UE Control Block */
   PjDlUeCb      *prevUeCb;        /* Previos UE Control Block */
   PjDlRbCb        *rbCb;            /* RB Control Block */
   PjDlRbCb        **rbCbLst;        /* RB Control Block List */
   PjSec       secInfo;          /* Security info of UE */
   U8          idx;              /* Index for RBs */
#ifdef LTE_L2_MEAS_COMMENT
   PjL2MeasEvtCb  *measEvtCb = NULLP;
#endif
   TRC3(pjUtlDlShutdown) 

   RLOG0(L_DEBUG, "pjUtlShutdown()");

   ret      = ROK;
   ueCb     = NULLP;
   prevUeCb = NULLP;
   rbCb     = NULLP;
   rbCbLst  = NULLP;
   idx      = 0;

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (cmHashListGetNext(&(gCb->u.dlCb->ueLstCp), (PTR) prevUeCb, (PTR *)&ueCb) == ROK)
   {
      if ( ueCb != NULLP )
      {
#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
         if (ueCb->libInfo.obdTmr.tmrEvnt == PJ_EVT_OBD_TMR)
         {
            pjStopTmr(gCb,(PTR)ueCb, PJ_EVT_OBD_TMR);
         }
#endif
         rbCbLst = ueCb->srbCb;
         for (idx = 0; idx < PJ_MAX_SRB_PER_UE; idx++)
         {
             rbCb = rbCbLst[idx];
             if (rbCb != NULLP)
             {
               /* Free the Buffers of RbCb */
               pjUtlDlFreeRb(gCb,rbCb);
             }
         }
         rbCbLst = ueCb->drbCb;
         for (idx = 0; idx < PJ_MAX_DRB_PER_UE; idx++)
         {
             rbCb = rbCbLst[idx];
             if (rbCb != NULLP)
             {
                pjUtlDlFreeRb(gCb,rbCb);
             }
         }
         /* Close all the context info for offboarding */
         secInfo = ueCb->secInfo;
         pjUtlDlCipherClose(gCb,secInfo.cpCxtId);
         pjUtlDlCipherClose(gCb,secInfo.upCxtId);
         pjUtlDlIntClose(gCb,secInfo.intCxtId);

         /* Delete hoInfo if present */
         if (ueCb->hoInfo != NULLP)
         {
            PJ_FREE(gCb,ueCb->hoInfo->hoCfmInfo,
                           (PJ_MAX_DRB_PER_UE * sizeof(PjDlHoCfmInfo)));
            PJ_FREE(gCb,ueCb->hoInfo, sizeof(PjDlHoInfo));                 
         }
      }
      prevUeCb = ueCb;
   }
#ifdef LTE_L2_MEAS_COMMENT
   for(idx = 0; idx < PJ_MAX_L2MEAS_EVT; idx++)
   {
      if(gCb->u.dlCb->pjL2Cb.pjMeasEvtCb[idx] != NULLP)
      {
         measEvtCb = gCb->u.dlCb->pjL2Cb.pjMeasEvtCb[idx];
         pjStopTmr((PTR)measEvtCb, PJ_EVT_L2_TMR);
         gCb->u.dlCb->pjL2Cb.pjNumMeas--;
         PJ_FREE(gCb,measEvtCb, sizeof(PjL2MeasEvtCb));
         gCb->u.dlCb->pjL2Cb.pjMeasEvtCb[idx] = NULLP;
      }
   }
#endif
   ret = pjDbmDlDeInit(gCb);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      RLOG0(L_FATAL, "PDCP DL DeInitialization Failed.");
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */


   /* commented this to use in deregister timer and kwusap */
   /* gCb->init.cfgDone &= ~PJ_LMM_GENCFG_DONE; */
   RETVALUE(ret);
}

/**
 * 
 * @brief 
 *
 *        Handler to free a PDCP Control Block.
 *
 * @b Description:
 * 
 *        This function frees the txBuf and rxBuf in a PDCP control block.
 *        and closes the compression channel opened. It stops the timers 
 *        of this control block if they are running.
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
PUBLIC Void pjUtlDlFreeRb
(
PjCb     *gCb,
PjDlRbCb *pjRbCb
)
#else
PUBLIC Void pjUtlDlFreeRb(pjRbCb)
PjCb     *gCb;
PjDlRbCb *pjRbCb;
#endif
{
   TRC3(pjUtlDlFreeRb)
   RLOG0(L_DEBUG, "pjUtlFreeRb()");


#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   if (pjRbCb->dlCb.obdTmr.tmrEvnt == PJ_EVT_DL_OBD_TMR)
   {
       pjStopTmr(gCb,(PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }
#endif
   
   pjUtlCmpClose(gCb,pjRbCb->cmpCxtId);
   /*pj004.201 Adding of Missing Trace in LTE RLC PDCP*/
   RETVOID;
}

/* pj005.201 added function to send data forward indication message */
/**
 * 
 * @brief 
 *
 *        Handler to send Data Fwd Indication
 *
 * @b Description:
 *      This function sends Data Fwd Indication message to PDCP user 
 *
 *  @param[in]  pjRbCb     PDCP RbCb
 *  @param[in]  datFwdInd  Data forward indication
 *
 *
 *  @return  S16
 *      -# ROK 
 *
 */

#ifdef ANSI
PUBLIC S16 pjUtlDlSndDatFwdInd
(
PjCb             *gCb,
PjDlRbCb           *pjRbCb,
PjuDatFwdIndInfo *datFwdInd
)
#else
PUBLIC S16 pjUtlDlSndDatFwdInd(pjRbCb, datFwdInd) 
PjCb             *gCb;
PjDlRbCb           *pjRbCb;
PjuDatFwdIndInfo *datFwdInd;
#endif
{
   CmLtePdcpId  *pdcpId;
   PjPjuSapCb   *pjuSap;

   TRC3(pjUtlDlSndDatFwdInd) 
   
   RLOG0(L_DEBUG, "pjUtlSndDatFwdInd()");

   pjuSap   = &(gCb->u.dlCb->pjuSap[PJ_DRB_SAP]);
   CmLtePdcpId pdcpIdTmp;
   pdcpId = &pdcpIdTmp;

   if(pdcpId != NULLP)
   {
      pdcpId->ueId   =  pjRbCb->ueCb->key.ueId;
      pdcpId->cellId =  pjRbCb->ueCb->key.cellId;
      pdcpId->rbId   =  pjRbCb->rbId;
      pdcpId->rbType =  pjRbCb->rbType;
      
      gCb->pjGenSts.numPktsFrwd += datFwdInd->numSdus;
      /* If trace flag is enabled send the trace indication */
      if(gCb->init.trc == TRUE)
      {                       
         /* Populate the trace params */
         pjLmmSendTrc(gCb,EVTPJUDATFWDIND, NULLP);
      }
      PjUiPjuDatFwdInd(&(pjuSap->pst), pjuSap->suId, pdcpId, datFwdInd);
   }

  RETVALUE(ROK);

}/* end of pjUtlSndDatFwdInd */

/* pj005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
/**
 *
 * @brief Function to initialise measurement
 *
 * @b Description
 *
 * @param[in]  gCb     PDCP Instance Control Block
 *
 *  @return  Void
 *
 */
S16 pjUtlL2MeasDlInit(PjCb *gCb)
{
   U16   cntr;

   gCb->u.dlCb->pjL2Cb.pjNumMeas=0;
   for(cntr = 0; cntr < LPJ_MAX_L2MEAS; cntr++)
   {
      cmMemset((U8 *)&(gCb->u.dlCb->pjL2Cb.pjL2EvtCb[cntr]), 0, sizeof(PjL2MeasEvtCb));
   }
   gCb->u.dlCb->pjL2Cb.pjL2EvtCb[0].measCb.measType = LPJ_L2MEAS_DL_DISC;

   RETVALUE(ROK);
}

/**
 *
 * @brief Handler for sending L2 Measurement report 
 *
 * @b Description
 *
 *  @param[in] gCb          PDCP Instance Control Block. 
 *  @param[in] measEvtCb    Measurement Event Control Block.
 *
 *
 *  @return  S16
 *      -# ROK
 */

#ifdef ANSI
PUBLIC S16 pjUtlSndDlL2MeasCfm
(
PjCb            *gCb,
PjL2MeasEvtCb   *measEvtCb
)
#else
PUBLIC S16 pjUtlSndDlL2MeasCfm(measEvtCb)
PjCb            *gCb;
PjL2MeasEvtCb   *measEvtCb;
#endif
{
   PjL2MeasCb     *measCb;
   PjL2MeasCfmEvt measCfmEvt;
   U8             qciIdx;

   TRC3(pjUtlSndDlL2MeasCfm)

   measCb = &measEvtCb->measCb;
   cmMemset((U8*)&measCfmEvt, 0, sizeof(PjL2MeasCfmEvt));
   
   measCfmEvt.measType = measCb->measType;
   measCfmEvt.status.status = LCM_PRIM_OK;
   measCfmEvt.status.reason = LCM_REASON_NOT_APPL;
   for(qciIdx = 0; qciIdx < (LPJ_MAX_QCI-1); qciIdx++)
   {
      measCfmEvt.measCfm[measCfmEvt.numQciCfm].qci = measCb->qci[qciIdx];
      if(measCfmEvt.measCfm[measCfmEvt.numQciCfm].qci > 0)
      {
         if((measCb->measType & LPJ_L2MEAS_DL_DISC) &&
               (measCb->measData[measCb->qci[qciIdx]].dlDisc.numPkts > 0))
         {
            /* dlDisc = num of Pkts Discarded * 10^6 / total no of pkts. */
            measCfmEvt.measCfm[measCfmEvt.numQciCfm].dlDisc = 
               measCb->measData[measCb->qci[qciIdx]].dlDisc.val  * (1000000) /
               measCb->measData[measCb->qci[qciIdx]].dlDisc.numPkts;
            /* Reset the counters */
            measCb->measData[measCb->qci[qciIdx]].dlDisc.val = 0; 
            measCb->measData[measCb->qci[qciIdx]].dlDisc.numPkts =0;
         }
         measCfmEvt.numQciCfm++;
      }
   }

   /* Send Measurement confirmation to layer manager */
   PjMiLpjL2MeasCfm(&gCb->pjGenCfg.lmPst,&measCfmEvt);

   RETVALUE(ROK);
}


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
/**
 *
 * @brief Handler for resetting the DL measurement counters 
 *
 *     Function :pjUtlResetDlL2MeasCntr 
 *
 * @b Description
 *
 * @param[in] measCb    Measurement Control Block.
 *
 *
 * @return  Void
 */
#ifdef ANSI

PUBLIC Void pjUtlResetDlL2MeasCntr
(
PjCb         *gCb,
PjL2MeasCb   *measCb,
U8           measType
)
#else
PUBLIC Void pjUtlResetDlL2MeasCntr(gCb, measCb, measType)
PjCb         *gCb;
PjL2MeasCb   *measCb;
U8           measType;
#endif
{
   /* for now the only meas should be DL discard in this case */
   if (measCb->measType & LPJ_L2MEAS_DL_DISC)
   {
      U32 i;
      for(i = 0; i < (LPJ_MAX_QCI - 1); i++)
      {
         U8 qciVal = measCb->qci[i];

         measCb->measData[qciVal].dlDisc.val = 0;
         measCb->measData[qciVal].dlDisc.numPkts  = 0;
         gCb->u.dlCb->pjL2Cb.measOn[qciVal] &= ~LPJ_L2MEAS_DL_DISC;
      }

      measCb->numQci = 0;
   }
}

/**
 *
 * @brief Handler for storing address of MeasData in rbCb at right index
 *
 *
 * @b Description
 *        This function is called when LM sends measReq message to RLC.
 *
 *  @param[in] 
 *  @param[out] 
 *  @param[in] 
 *
 *
 *  @return  S16
 *      -# ROK
 */
#ifdef ANSI
PUBLIC Void pjUtlPlcMeasDatInDlL2Sts
(
PjL2Cntr       *measData, 
PjL2MeasRbCb   *rbL2Cb,
U8             measType
)
#else
PUBLIC Void pjUtlPlcMeasDatInDlL2Sts(measData, rbL2Cb, measType)
PjL2Cntr       *measData; 
PjL2MeasRbCb   *rbL2Cb;
U8             measType;
#endif
{
   TRC3(pjUtlPlcMeasDatInDlL2Sts)
   
   /* We should check the number of measType in the request. This can be done 
    * by looking at each bit in the measType. Also store the measData in the 
    * correct index of l2Sts in RbCb.
    * */

    if(measType & LPJ_L2MEAS_DL_DISC)
    {
         rbL2Cb->l2Sts[PJ_L2MEAS_DL_DISC] = measData;
    }

}/* End of pjUtlPlcMeasDatInDlL2Sts */
#endif /* LTE_L2_MEAS */

void dumpPDCPDlRbInformation(PjDlRbCb* dlRbCb, U16 ueId)
{
   PjDlCb* dlCb = &dlRbCb->dlCb;
   RLOG_ARG1(L_INFO,DBG_UEID,ueId, "PDCP DL RB numEntries = %lu",
                    dlCb->txBuf.numEntries);
   RTLIN_DUMP_DEBUG("UE [%u] PDCP DL RB numEntries = %lu",
                  ueId, dlCb->txBuf.numEntries);
   QU_CRITLOG(QU_LOG_MESSAGE,"UE [%u] PDCP DL RB numEntries = %lu\n",
                  ueId, dlCb->txBuf.numEntries);
}

void DumpPDCPDlDebugInformation(void)
{
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   void *iccHdl = NULLP;
   U32 poolZeroFreeCnt;
   U32 poolOneFreeCnt;
   U32 poolTwoFreeCnt;
   U32 poolThreeFreeCnt;
#endif   
   PjCb* dlInst = pjCb[1]; /* TODO: check whether DL is 0 or 1 */

   PjDlgCb* dlgCb = dlInst->u.dlCb;

   PjDlUeCb *ueCb = NULLP; 
   RLOG0(L_INFO,"PDCP DL Information");
   RLOG0(L_INFO,"===================");
   QU_CRITLOG(QU_LOG_MESSAGE,"PDCP DL Information\n");
   QU_CRITLOG(QU_LOG_MESSAGE,"===================\n");
   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&dlgCb->ueLstCp, 
                                   (PTR) ueCb, 
                                   (PTR *)&ueCb))
   {
      U32 i;
      for(i = 0; i < PJ_MAX_SRB_PER_UE; i++)
      {
         PjDlRbCb* rbCb = ueCb->srbCb[i];
         if(rbCb != NULLP)
         {
            dumpPDCPDlRbInformation(rbCb, ueCb->key.ueId);
         }
      }
      for(i = 0; i < PJ_MAX_DRB_PER_UE; i++)
      {
         PjDlRbCb* rbCb = ueCb->drbCb[i];
         if(rbCb != NULLP)
         {
            dumpPDCPDlRbInformation(rbCb, ueCb->key.ueId);
         }
      }
   }/* end while */
   /*printing ICC memory statistics*/
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   iccHdl = ssGetIccHdl(dlInst->init.region);
   if(NULLP != iccHdl)
   {
      poolZeroFreeCnt = TL_GetFreeBlocks(iccHdl, ICC_POOL_ZERO_SIZE);
      poolOneFreeCnt = TL_GetFreeBlocks(iccHdl, ICC_POOL_ONE_SIZE);
      poolTwoFreeCnt = TL_GetFreeBlocks(iccHdl, ICC_POOL_TWO_SIZE);
      poolThreeFreeCnt = TL_GetFreeBlocks(iccHdl, ICC_POOL_THREE_SIZE);
      RLOG1(L_ALWAYS,"ICC pool zero free count = %lu", poolZeroFreeCnt);
      RLOG1(L_ALWAYS,"ICC pool one free count = %lu", poolOneFreeCnt);
      RLOG1(L_ALWAYS,"ICC pool two free count = %lu", poolTwoFreeCnt);
      RLOG1(L_ALWAYS,"ICC pool three free count = %lu", poolThreeFreeCnt);
      RTLIN_DUMP_DEBUG("ICC pool zero free count = %lu\n", poolZeroFreeCnt);
      RTLIN_DUMP_DEBUG("ICC pool one free count = %lu\n", poolOneFreeCnt);
      RTLIN_DUMP_DEBUG("ICC pool two free count = %lu\n", poolTwoFreeCnt);
      RTLIN_DUMP_DEBUG("ICC pool three free count = %lu\n", poolThreeFreeCnt);
   }
      /*ccpu00136858 */      
   RLOG1(L_ALWAYS,"SPACC cipher Q num of entry = %lu ", pjMsCiphQ.count);
   RLOG1(L_ALWAYS,"SPACC decipher Q num of entry = %lu ", pjMsDeCiphQ.count);
   RTLIN_DUMP_DEBUG("SPACC cipher Q num of entry = %lu \n", pjMsCiphQ.count);
   RTLIN_DUMP_DEBUG("SPACC decipher Q num of entry = %lu \n", pjMsDeCiphQ.count);
#endif
}

PUBLIC Void pjUtlEmptyDlPktList(PjCb *gCb, PjDlRbCb *pjRbCb)
{
   PjDlPkt *ulPkt;
   CmLListCp *ulPktLst = &pjRbCb->dlCb.dlPktQ;
   while(ulPktLst->first)
   {
      ulPkt = (PjDlPkt *)(ulPktLst->first->node);
      cmLListDelFrm(ulPktLst, ulPktLst->first);
      PJ_FREE_BUF(ulPkt->pdu);
#ifdef FLAT_BUFFER_OPT
      PJ_FREE_FLAT_BUF(gCb, &(ulPkt->fb))
#endif
      PJ_FREE(gCb, ulPkt, sizeof(PjDlPkt));
   }
   RETVOID;
}

void ResetPDCPStats(void)
{
   cmMemset((U8*)&gPdcpStats, 0, sizeof(PDCPStats));
}

void PrintPDCPStats(void)
{
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   /* UDAY */
   RLOG3(L_INFO,"PDCP Stats: SDUs Dropped = (%ld),SDUs Queued for SPAcc Cipher = (%ld),"
            "Decipher = (%ld)",
           gPdcpStats.numPdcpSdusDiscarded, pjMsDlSpaccQCnt, pjMsUlSpaccQCnt);
   RTLIN_DUMP_DEBUG("PDCP Stats: SDUs Dropped = (%ld),"
         "SDUs Queued for SPAcc Cipher = (%ld), Decipher = (%ld) MEM DROP = (%ld) dbgPdcpQSizeThreshHoldDropCnt is (%ld) \n",
           gPdcpStats.numPdcpSdusDiscarded, pjMsDlSpaccQCnt, pjMsUlSpaccQCnt,dbgPdcpMemCount,dbgPdcpQSizeThreshHoldDropCnt); /* UDAY */
   dbgPdcpMemCount =0;
   dbgPdcpQSizeThreshHoldDropCnt=0;
   //pdcpDropMemTh=0;
#endif
}

#ifdef ANSI
PRIVATE S16 pjUtlDlHdlSecInitCfm
(
PjCb        *gCb,              /* Pointer to PJ DL Control Block */
PjDlUeCb      *ueCb,           /* Pointer to UeCb */ 
U16         txIdx,             /* Transaction Index for UeCb */
PjAsyncCfm       *asyncCfm,
UdxSecCfgCfmInfo *secCfgCfm,   /* Security config confirm */
UdxReEstCfmInfo  *reEstCfm     /* Reest config confirm */
)
#else
PRIVATE S16 pjUtlDlHdlSecInitCfm(gCb,ueCb,txIdx,secCfgCfm,reEstCfm)
(                              
PjCb        *gCb;              /* Pointer to PJ DL Control Block */
PjDlUeCb      *ueCb;           /* Pointer to UeCb */ 
U16         txIdx;             /* Transaction Index for UeCb */
PjAsyncCfm       *asyncCfm;
UdxSecCfgCfmInfo *secCfgCfm;   /* Security config confirm */
UdxReEstCfmInfo  *reEstCfm;    /* Reest config confirm */
)
#endif   
{
   PjUdxDlSapCb    *udxSap;
   
   TRC3(pjUtlDlHdlSecInitCfm)

   if(asyncCfm->libInitBitMask == 0)
   {
      PJ_CHK_RESTART_OBD_TIMER(gCb,ueCb, txIdx);
      if(ueCb->libInfo.state == PJ_STATE_NORMAL)
      {
         /* Send security config confirm */
         /* Memory leak fix ccpu00135359 */
         udxSap    = &(gCb->u.dlCb->udxDlSap[0]);
         if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&secCfgCfm,
                  sizeof (UdxSecCfgCfmInfo)) != ROK)
         {
            RLOG0(L_FATAL, "Memory Allocation failed.");
            RETVALUE(RFAILED);
         }
         PJ_FILL_SEC_CFM_INFO(secCfgCfm, asyncCfm);

         PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

         PjDlUdxSecCfgCfm(&gCb->u.dlCb->udxDlSap[0].pst, 
               gCb->u.dlCb->udxDlSap[0].suId, 
               (UdxSecCfgCfmInfo *) secCfgCfm);
      }
      else if(ueCb->libInfo.state == PJ_STATE_REEST)
      {
         /* Send ReEstCfm */
         /* Memory leak fix ccpu00135359 */
         udxSap    = &(gCb->u.dlCb->udxDlSap[0]);
         if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&reEstCfm,
                  sizeof (UdxReEstCfmInfo)) != ROK)
         {
            RLOG0(L_FATAL, "Memory Allocation failed.");
            RETVALUE(RFAILED);
         }
         PJ_FILL_REEST_CFM_INFO(reEstCfm, asyncCfm, LCM_PRIM_OK);

         /* Some house keeping work */
         PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

         PjDlUdxReEstCfm(&gCb->u.dlCb->udxDlSap[0].pst, 
               gCb->u.dlCb->udxDlSap[0].suId, reEstCfm);

         pjUtlReEstDl(gCb, ueCb);
      }
   }
   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 pjUtlDlHdlCmpInitCfm
(
PjCb           *gCb,            /* Pointer to PJ DL Control Block */
PjDlUeCb       *ueCb,           /* Pointer to UeCb */ 
U16            txIdx,           /* Transaction Index for UeCb */
PjAsyncCfm     *asyncCfm,       /* Async Confirm */
UdxCfgCfmInfo  *cfgCfm         /* UDX Config Confirm */
)
#else
PRIVATE S16 pjUtlDlHdlCmpInitCfm(gCb,ueCb,txIdx,asyncCfm,cfgCfm)
(                              
PjCb           *gCb;            /* Pointer to PJ DL Control Block */
PjDlUeCb       *ueCb;           /* Pointer to UeCb */ 
U16            txIdx;           /* Transaction Index for UeCb */
PjAsyncCfm     *asyncCfm;       /* Async Confirm */
UdxCfgCfmInfo  *cfgCfm;         /* UDX Config Confirm */
)
#endif   
{
   PjUdxDlSapCb    *udxSap;
   U32             idx;
   TRC3(pjUtlDlHdlCmpInitCfm)

      asyncCfm->libInitBitMask ^= (PJ_LIB_COMP_BIT_MASK);

   if((asyncCfm->libInitBitMask == 0) &&
         !(asyncCfm->cfmType & PJ_CFG_REEST_ASYNC_CFM))
   {

      /* Send config confirm */
      /* Memory leak fix ccpu00135359 */
      udxSap    = &(gCb->u.dlCb->udxDlSap[0]);
      if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&cfgCfm,
               sizeof (UdxCfgCfmInfo)) != ROK)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         RETVALUE(RFAILED);
      }
      cfgCfm->transId = asyncCfm->transId;
      cfgCfm->ueId    = asyncCfm->ueId;
      cfgCfm->cellId  = asyncCfm->cellId;
      cfgCfm->numEnt  = asyncCfm->numEnt; 
      for ( idx = 0; idx < asyncCfm->numEnt; idx++ )
      {
         cfgCfm->cfmEnt[idx].status = asyncCfm->cfmEnt[idx].status;  
         cfgCfm->cfmEnt[idx].reason = asyncCfm->cfmEnt[idx].reason;  
         cfgCfm->cfmEnt[idx].rbId   = asyncCfm->cfmEnt[idx].rbId;
         cfgCfm->cfmEnt[idx].rbType = asyncCfm->cfmEnt[idx].rbType;
      }

      /* Some house keeping work */
      PJ_CHK_RESTART_OBD_TIMER(gCb,ueCb, txIdx);
      PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

      /* Send confirmtion to the User */
      PjDlUdxCfgCfm(&(gCb->u.dlCb->udxDlSap[0].pst), 
            gCb->u.dlCb->udxDlSap[0].suId, 
            (UdxCfgCfmInfo *)cfgCfm);
   }
   
   RETVALUE(ROK); 
}

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
PjLibTrans *libTrans,              /* Transaction Id */
U32      macI                     /* Message authentication code */
)
#else
PUBLIC S16 PjLibObdIntProtCfm(pst, libTrans, macI)
Pst      *pst;                    /* post structure */
PjLibTrans *libTrans;              /* Transaction Id */
U32      macI;                    /* Message authentication code */
#endif
{
   PjCb    *tPjCb;
   U8         hdr;
   PjDlRbCb   *pjRbCb;
   S16      ret=ROK;
   PjTxEnt  *txEnt=NULLP;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   Buffer *pdu;

   TRC3(PjLibObdIntProtCfm)

   tPjCb = pjCb[PJ_SEC_DIR_DL];

   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   pjDbmFetchPjDlRbCb((PjDlUeCb*)libTrans->ueCb, (libTrans->rbId + 1), libTrans->rbType,&pjRbCb);
   if ( pjRbCb == NULLP )
   {
      PJLOGERROR(tPjCb, ERRCLS_ADD_RES, EPJXXX, (ErrVal)libTrans->count,
         "PjLibObdIntProtCfm: pjRbCb is NULLP \n");
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      RETVALUE(RFAILED);
   }
   if ( pjRbCb->state == PJ_STATE_REEST || pjRbCb->state == PJ_STATE_REEST_HO)
   {
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      RETVALUE(ret);
   }


   txEnt = pjDbmGetTxEnt(tPjCb,&(pjRbCb->dlCb.txBuf), libTrans->count);
   if ( txEnt == NULLP )
   {
      PJLOGERROR(tPjCb,ERRCLS_ADD_RES, EPJXXX, (ErrVal)libTrans->count,
        "PjLibObdCmpCfm: txEnt is NULLP \n");
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      RETVALUE(RFAILED);
   }
  
   PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
   pdu = txEnt->sdu; 

   PJ_PACK_MACI(pdu, macI);

   txEnt->pdu = pdu;

   /* Remove the header and then only send for ciphering */
   ret = SRemPreMsg(&hdr, txEnt->pdu);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( ret != ROK )
   {
      PJLOGERROR(gCb, ERRCLS_ADD_RES, EPJXXX, (ErrVal)txEnt->count,
         "pjDlmHdlIntProt: SRemPreMsg for txEnt \n");
      PJ_UPD_DL_VAR(gCb, pjRbCb, txEnt->count);
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(gCb,pjRbCb, txEnt, PJ_REM_MSG_FAILED);
      }
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */
   /* If rbCb flag is set => firstDL msg. So skip ciphering */
   if ( pjRbCb->firstDlMsg == TRUE  && pjRbCb->firstSn == txEnt->sn)
   {
      pjRbCb->firstDlMsg = FALSE;
      /*PJ_FREE_BUF(txEnt->sdu);*/
      txEnt->sdu = NULLP;
   }
   else
   {
      PjSec *secInfo;
      txEnt->state  = PJ_WAIT_FOR_CIPHER;
      secInfo = &(pjRbCb->ueCb->secInfo);
      {
         ret = pjDlmHdlCiph(tPjCb, pjRbCb, txEnt);
         if ( ret != ROK )
         {
            PJLOGERROR(tPjCb, ERRCLS_ADD_RES, EPJXXX, (ErrVal)txEnt->count,
               "pjDlmHdlIntProc: Ciphering failed for txEnt \n");
            RETVALUE(RFAILED);
         }
         /*Return, Batch mode in place so packet will be ciphered on trigger*/
#ifdef TENB_AS_SECURITY
         /*if(isSecBatchMode)
         {
              RETVALUE(ret);
         }*/
         /*QU_CRITLOG(QU_LOG_MESSAGE,"pjDlmHdlCiph RET #####################\n" );*/
         RETVALUE(ROK);
#endif
      }
   }
   ret = pjDlmDeliverPdu(tPjCb, pjRbCb, txEnt);

   RETVALUE(ret);
} /* end of PjLibObdCmpCfm */

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
PjLibTrans *libTrans,              /* Transaction Id */
Buffer   *opSdu                   /* ciphered SDU */
)
#else
PUBLIC S16 PjLibObdCipherCfm(pst, libTrans, opSdu)
Pst      *pst;                    /* post structure */
PjLibTrans *libTrans;              /* Transaction Id */
Buffer   *opSdu;                  /* ciphered SDU */
#endif
{
   PjCb     *tPjCb;
   PjDlRbCb   *pjRbCb;
   S16      ret=ROK;
   PjTxEnt  *txEnt=NULLP;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */

   TRC3(PjLibObdCipherCfm)

   tPjCb = pjCb[PJ_SEC_DIR_DL];

   /*QU_CRITLOG(QU_LOG_MESSAGE,"PjLibObdCipherCfm called 1\n" );*/
#ifdef ALIGN_64BIT
   PJDBGP(tPjCb,(PJ_DBGMASK_UTL | PJ_DBGMASK_BRIEF),
          (tPjCb->init.prntBuf, "PjLibObdCipherCfm(pst, libTrans(%d,%d,%d))\n",\
           libTrans->rbId, libTrans->rbType, libTrans->count));
#else
   PJDBGP(tPjCb,(PJ_DBGMASK_UTL | PJ_DBGMASK_BRIEF),
         (tPjCb->init.prntBuf, "PjLibObdCipherCfm(pst, libTrans(%d,%d,%ld))\n",\
           libTrans->rbId, libTrans->rbType, libTrans->count));
#endif
     if(libTrans->rbType == PJ_DRB)
     {
        numOutStandingCryptoJobs[pjQcomGetCryptoCoreId(libTrans)]--;
     }
   txEnt = (PjTxEnt*)libTrans->txEnt;

   if(txEnt->state == PJ_PDU_TO_DELETE)
   {
     /*Free libTrans and rxEnt*/
     PJ_FREE(tPjCb, txEnt, sizeof(PjTxEnt));
     PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
     PJ_FREE_BUF(opSdu);

     RETVALUE(ROK);
   }


   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      PJ_FREE_BUF(opSdu);
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      RETVALUE(RFAILED);
   }

   /*QU_CRITLOG(QU_LOG_MESSAGE,"PjLibObdCipherCfm called 2\n" );*/
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   pjDbmFetchPjDlRbCb((PjDlUeCb*)libTrans->ueCb, (libTrans->rbId + 1), libTrans->rbType,&pjRbCb);
   if ( pjRbCb == NULLP )
   {
      PJLOGERROR(tPjCb,ERRCLS_ADD_RES, EPJXXX, (ErrVal)libTrans->count,
         "PjLibObdCipherCfm: pjRbCb is NULLP \n");
      QU_CRITLOG(QU_LOG_MESSAGE, "Double Free: rbCb null");
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
   if ( pjRbCb->state == PJ_STATE_REEST || pjRbCb->state == PJ_STATE_REEST_HO)
   {
      QU_CRITLOG(QU_LOG_MESSAGE, "Double Free: state is reest");
      PJ_FREE_BUF(opSdu);
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      RETVALUE(ret);
   }
   /*QU_CRITLOG(QU_LOG_MESSAGE,"PjLibObdCipherCfm called 3\n" );*/
   if ( opSdu == NULLP )
   {
      PJLOGERROR(tPjCb,ERRCLS_ADD_RES, EPJXXX, (ErrVal)libTrans->count,
        "PjLibObdCipherCfm: opSdu is NULLP:Ciphering Failed \n");
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
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      RETVALUE(RFAILED);
   }

   //QU_CRITLOG(QU_LOG_MESSAGE,"PjLibObdCipherCfm called 5\n" );
   txEnt->pdu = opSdu;
   //txEnt->state = PJ_PDU_SUBMITTED;
   ret = pjDlmDeliverPdu(tPjCb,pjRbCb, txEnt);
   PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));

   RETVALUE(ret);

} /* end of PjLibObdCipherCfm */




#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file:     eq_dl_utl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:49 2015
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

