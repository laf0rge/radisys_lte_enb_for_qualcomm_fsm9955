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

     Sid:      eq_ul_utl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:53 2015

     Prg:      nm 

**********************************************************************/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=243;

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

EXTERN U32 numOutStandingCryptoJobs[2]; /*PJ_QCOM_MAX_CRYPTO_CORE == 2*/
#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

PRIVATE S16 pjUtlUlHdlSecInitCfm ARGS((PjCb *gCb,PjUlUeCb *ueCb, 
              U16 txIdx,PjAsyncCfm *asyncCfm, CpjSecCfgCfmInfo *secCfgCfm,CpjReEstCfmInfo *reEstCfm));
PRIVATE S16 pjUtlUlHdlCmpInitCfm ARGS((PjCb *gCb,PjUlUeCb *ueCb, 
              U16 txIdx,PjAsyncCfm *asyncCfm, CpjCfgCfmInfo *cfgCfm));

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
PUBLIC S16 pjUtlUlCmpInit
(
PjCb       *gCb,
PjUlRbCb   *pjRbCb                  /* PDCP RbCb */
)
#else
PUBLIC S16 pjUtlUlCmpInit(pjRbCb)
PjCb       *gCb;
PjUlRbCb   *pjRbCb;                 /* PDCP RbCb */
#endif
{
   TRC3(pjUtlUlCmpInit)

   RLOG2(L_DEBUG, "pjUtlUlCmpInit(pjRbCb(%d,%d))",
                    pjRbCb->rbId, pjRbCb->rbType);
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
PUBLIC S16 pjUtlUlIntInit
(
PjCb     *gCb,
PjUlUeCb   *ueCb                   /* UE CB Ptr */
)
#else
PUBLIC S16 pjUtlUlIntInit(ueCb)
PjCb     *gCb;
PjUlUeCb   *ueCb;                   /* UE CB Ptr */
#endif
{

#ifndef PJ_SEC_ASYNC
   Void *ctxId = NULL;   /* KW_FIX */
#else
   U8  txIdx;
   U32 *libInitBitMask = NULLP;
#endif
   S32 ret=ROK;

   TRC3(pjUtlUlIntInit)

   RLOG2(L_DEBUG, "pjUtlUlIntInit(ueCb(%d,%d))",
                    ueCb->key.ueId, ueCb->key.cellId);

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
PUBLIC S16 pjUtlUlCpInit
(
PjCb     *gCb,
PjUlUeCb   *ueCb                    /* UE CB Ptr */
)
#else
PUBLIC S16 pjUtlUlCpInit(ueCb)
PjCb     *gCb;
PjUlUeCb   *ueCb;                   /* UE CB Ptr */
#endif
{

#ifndef PJ_SEC_ASYNC
   Void *ctxId = NULL;       /* KW_FIX */
#else
   U8  txIdx;
   U32 *libInitBitMask = NULLP;
#endif
   S16   ret = ROK;

   TRC3(pjUtlUlCpInit)

   RLOG2(L_DEBUG, "pjUtlCpInit(ueCb(%d,%d))",
                    ueCb->key.ueId, ueCb->key.cellId);

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
                  ueCb->secInfo.cipherInfo.cpKey, &ctxId);  /*KW_FIX*/
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
PUBLIC S16 pjUtlUlUpInit
(
PjCb     *gCb,
PjUlUeCb   *ueCb                    /* UE CB Ptr */
)
#else
PUBLIC S16 pjUtlUpInit(gCb,ueCb)
PjCb     *gCb;
PjUlUeCb   *ueCb;                   /* UE CB Ptr */
#endif
{

#ifndef PJ_SEC_ASYNC
   Void* ctxId = NULL;  /* KW_FIX */
#else
   U8  txIdx;
   U32 *libInitBitMask = NULLP;
#endif

   TRC3(pjUtlUlUpInit)

   RLOG2(L_DEBUG, "pjUtlUpInit(ueCb(%d,%d))",
                    ueCb->key.ueId, ueCb->key.cellId);

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
                              ueCb->secInfo.cipherInfo.upKey, &ctxId); /*KW_FIX*/
   ueCb->secInfo.upCxtId = ctxId;
#endif /* PJ_SEC_ASYNC */

   RETVALUE(ROK);

} /* end of pjUtlUpInit */


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
PUBLIC S16 pjUtlUlCmpReset
(
PjCb        *gCb,
PjUlRbCb      *pjRbCb                 /* Context to be reset for compression */
)
#else
PUBLIC S16 pjUtlUlCmpReset(pjRbCb)
PjCb        *gCb;
PjUlRbCb      *pjRbCb;                 /* Context to be reset for compression */
#endif
{
   TRC3(pjUtlUlCmpReset)

   RLOG2(L_DEBUG, "pjUtlUlCmpReset(pjRbCb(%d,%d))",
                    pjRbCb->rbId, pjRbCb->rbType);

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
 *
 *        Handler for closing the UL context with the Ciphering unit (either 
 *        synchronous or asynchronous) for SRBs of an UE.
 *
 * @b Description:
 *
 *        This function closes an existing UL context with the security algo unit per UE
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
PUBLIC S16 pjUtlUlCipherClose
(
PjCb     *gCb,
Void      *cpCxtId                  /* Context Id for Ciphering to be closed */
)
#else
PUBLIC S16 pjUtlUlCipherClose(cpCxtId)
PjCb     *gCb;
Void     *cpCxtId;                 /* Context Id for Ciphering to be closed */
#endif
{
   TRC3(pjUtlUlCipherClose)
   RLOG0(L_DEBUG, "pjUtlUlCipherClose");
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
 *        Handler for closing the UL context with the Integration unit (either 
 *        synchronous or asynchronous) for all RBs of an UE.
 *
 * @b Description:
 *
 *        This function closes an existing UL context with the security algo unit per UE
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
PUBLIC S16 pjUtlUlIntClose
(
PjCb     *gCb,
Void      *intCxtId                 /* Integration CxtId to be closed */
)
#else
PUBLIC S16 pjUtlUlIntClose(intCxtId)
PjCb     *gCb;
Void     *intCxtId;                /* Integration CxtId to be closed */
#endif
{
   TRC3(pjUtlUlIntClose)
   RLOG0(L_DEBUG, "pjUtlUlIntClose");
   
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
 *        Handler for redirecing deciphering request to either synchronous
 *        or asynchronous module.
 *
 * @b Description:
 *
 *        This function sends deciphering request as either synchronous or
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
/* Uday */
#ifdef TENB_AS_SECURITY
#ifdef ANSI
PUBLIC S16 pjUtlDecipherReq
(
PjCb       *gCb,
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
U32        count,
PjRxEnt   *rxEnt,                   /* Data to be deciphered */
Buffer   **opSdu                  /* Deciphered SDU */
)
#else
PUBLIC S16 pjUtlDecipherReq(gCb, pjRbCb, count, mBuf, opSdu)
PjCb       *gCb;
PjUlRbCb   *pjRbCb;                 /* PDCP RbCb */
U32        count;
Buffer     *mBuf;                   /* Data to be deciphered */
Buffer   **opSdu;                  /* Deciphered SDU */
#endif
{
   S16 ret = ROK;
   PjUlUeCb *ulUeCb;
   PjLibTrans *libTrans; /* Transaction Id for deciphering */
   Void      *cxtId = NULLP;    /* Context Identifier */
   TRC3(pjUtlDecipherReq)

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
      RETVALUE(RFAILED);
   }
   
   ulUeCb = pjRbCb->ueCb;
   libTrans->pdcpInstance = gCb->init.inst;
   libTrans->count = count;
   libTrans->rbId = pjRbCb->rbId - 1;
   libTrans->dir = PJ_SEC_DIR_UL;
   libTrans->rbType = pjRbCb->rbType;
   libTrans->snLen = pjRbCb->snLen;
   libTrans->ciphAlgoType = ulUeCb->secInfo.cipherInfo.algoType;
   libTrans->ueCb = (PTR)ulUeCb;
   libTrans->ueId = ulUeCb->key.ueId;
   libTrans->cellId = ulUeCb->key.cellId;
   libTrans->rxEnt  = (PTR)rxEnt;
   if(PJ_SRB == pjRbCb->rbType)
   {
      libTrans->key = ulUeCb->secInfo.cipherInfo.cpKey;
   }
   else
   {
      libTrans->key = ulUeCb->secInfo.cipherInfo.upKey;
   }
   
#ifdef PJ_SEC_ASYNC
   /* Start the timer if it is not started already */
   if((pjChkTmr(gCb,(PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
   {
      pjRbCb->ulCb.obdPdu   =  secInp.count;
      pjStartTmr(gCb,(PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }
   ret = PjLibObdDecipherReq(&(gCb->pjGenCfg.obdPst.secPst), 
                                 cxtId, secInp, libTrans, rxEnt->mBuf);  
#else
   ret = PjLibObdDecipherReq(libTrans, rxEnt->mBuf, opSdu);
#endif
   if (RFAILED == ret)
   {
       PJ_FREE(gCb,libTrans, sizeof(PjLibTrans));
   }
   RETVALUE(ret);
} /* end of pjUtlDecipherReq */
#else
#ifdef ANSI
PUBLIC S16 pjUtlDecipherReq
(
PjCb       *gCb,		   /* Cell Cb */
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
U32        count,
Buffer   *mBuf,                   /* Data to be deciphered */
Buffer   **opSdu                  /* Deciphered SDU */
)
#else
PUBLIC S16 pjUtlDecipherReq(gCb, pjRbCb, count, mBuf, opSdu)
PjCb       *gCb;
PjUlRbCb   *pjRbCb;                 /* PDCP RbCb */
U32        count;
Buffer   *mBuf;                   /* Data to be deciphered */
Buffer   **opSdu;                 /* Deciphered SDU */
#endif
{
   S16 ret = ROK;
#ifdef PJ_SEC_ASYNC
   PjLibTrans libTrans; /* Transaction Id for deciphering */
#else
   PjSecInp   secInp;
#endif /* PJ_SEC_ASYNC */
   PTR        cxtId;    /* Context Identifier */
   TRC3(pjUtlDecipherReq)
   if (pjRbCb->rbType == PJ_SRB)
   {
      cxtId = (PTR)pjRbCb->ueCb->secInfo.cpCxtId;
   }
   else
   {
      cxtId = (PTR)pjRbCb->ueCb->secInfo.upCxtId;
   } 
#ifdef PJ_SEC_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = count;
   /* kw005.201 ccpu00114955 corrected the RB ID calculation */
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;
   /* Start the timer if it is not started already */
   if((pjChkTmr(gCb,(PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
   {
      pjRbCb->ulCb.obdPdu   =  secInp.count;
      pjStartTmr(gCb,(PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }
   ret = PjLibObdDecipherReq(&gCb->pjGenCfg.obdPst.secPst, 
                                 cxtId, secInp, libTrans, mBuf);  
#else
   secInp.count = count;
   secInp.rbId  = pjRbCb->rbId - 1;
   secInp.dir = 0;
   ret = pjLibDecipherReq(cxtId, secInp, mBuf, opSdu);
#endif
   RETVALUE(ret);
} /* end of pjUtlDecipherReq */
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
 *        This function sends Intergrity verification request as either synchronous or
 *        asynchronous function calls with relevant input parameters.
 *
 *  @param[in]   pjRbCb        PDCP RbCb 
 *  @param[in]   secInp        Input parameters for integrity 
 *  @param[in]   mBuf          SDU to be compressed 
 *  @param[out]  status        Integrity verification status 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef TENB_AS_SECURITY
#ifdef ANSI
PUBLIC S16 pjUtlIntVerReq
(
PjCb     *gCb,
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for integrity */ 
Buffer   *mBuf,                   /* SDU to be compressed */
U32      macI,                    /* MAC-I to be verified with */
Status   *status                  /* Integrity verification status */
)
#else
PUBLIC S16 pjUtlIntVerReq(pjRbCb, secInp, mBuf, macI, status)
PjCb     *gCb;
PjUlRbCb   *pjRbCb;                 /* PDCP RbCb */
PjSecInp secInp;                 /* Input parameters for integrity */ 
Buffer   *mBuf;                   /* SDU to be compressed */
U32      macI;                    /* MAC-I to be verified with */
Status   *status;                 /* Integrity verification status */
#endif
{
   S16 ret = ROK;
   PjUlUeCb *ulUeCb;
   PjLibTrans *libTrans; /* Transaction Id for deciphering */
   Void       *cxtId;    /* Context Identifier */
   TRC3(pjUtlIntVerReq)

   cxtId = pjRbCb->ueCb->secInfo.intCxtId;
   PJ_ALLOC(gCb, libTrans , sizeof(PjLibTrans));
   if(libTrans == NULLP)
   {
      RsysSHMLog("PjLibObdIntProtReq failed line %d \n", __LINE__);
      RETVALUE(RFAILED);
   }
   
   ulUeCb = pjRbCb->ueCb;
   libTrans->pdcpInstance = gCb->init.inst;
   libTrans->count = secInp.count;
   libTrans->rbId = pjRbCb->rbId - 1;
   libTrans->dir = secInp.dir;
   libTrans->rbType = pjRbCb->rbType;
   libTrans->intAlgoType = ulUeCb->secInfo.intInfo.algoType;
   libTrans->ueCb = (PTR)ulUeCb;
   libTrans->ueId = ulUeCb->key.ueId;
   libTrans->cellId = ulUeCb->key.cellId;
   libTrans->key = (U8*)&ulUeCb->secInfo.intInfo.intKey[0];
   PJ_SEC_FILL_FRESH(libTrans->fresh, libTrans->rbId);

#ifdef PJ_SEC_ASYNC
   /* Start the timer if it is not started already */
   if((pjChkTmr(gCb, (PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
   {
      pjRbCb->ulCb.obdPdu   =  secInp.count;
      pjStartTmr(gCb, (PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }
   ret = PjLibObdIntVerReq(&(gCb->pjGenCfg.obdPst.secPst), cxtId, 
                                       secInp, libTrans, mBuf, macI);  
#else
   ret = PjLibObdIntVerReq(libTrans, mBuf, macI);
#endif
   if (RFAILED == ret)
   {
       PJ_FREE(gCb,libTrans, sizeof(PjLibTrans));
   }

   RETVALUE(ret);

} /* end of pjUtlIntVerReq */
#else
#ifdef ANSI
PUBLIC S16 pjUtlIntVerReq
(
PjCb     *gCb,
PjUlRbCb   *pjRbCb,                 /* PDCP RbCb */
PjSecInp secInp ,                 /* Input parameters for integrity */ 
Buffer   *mBuf,                   /* SDU to be compressed */
U32      macI,                    /* MAC-I to be verified with */
Status   *status                  /* Integrity verification status */
)
#else
PUBLIC S16 pjUtlIntVerReq(gCb, pjRbCb, secInp, mBuf, macI, status)
PjCb     *gCb;
PjUlRbCb   *pjRbCb;                 /* PDCP RbCb */
PjSecInp secInp;                 /* Input parameters for integrity */ 
Buffer   *mBuf;                   /* SDU to be compressed */
U32      macI;                    /* MAC-I to be verified with */
Status   *status;                 /* Integrity verification status */
#endif
{
   S16 ret = ROK;
#ifdef PJ_SEC_ASYNC
   PjLibTrans libTrans; /* Transaction Id for deciphering */
#endif /* PJ_SEC_ASYNC */
   PTR cxtId;    /* Context Identifier */

   TRC3(pjUtlIntVerReq)

   cxtId = (PTR)pjRbCb->ueCb->secInfo.intCxtId;

#ifdef PJ_SEC_ASYNC
   /* Assign transId and cxtId */
   libTrans.count  = secInp.count;
   libTrans.rbId   = pjRbCb->rbId - 1;
   libTrans.rbType = pjRbCb->rbType;
   libTrans.ueCb   = pjRbCb->ueCb;

   /* Start the timer if it is not started already */
   if((pjChkTmr(gCb,(PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
   {
      pjRbCb->ulCb.obdPdu   =  secInp.count;
      pjStartTmr(gCb,(PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }

   ret = PjLibObdIntVerReq(&gCb->pjGenCfg.obdPst.secPst, cxtId, 
                                       secInp, libTrans, mBuf, macI);  
#else
   /* ccpu00142570 */
   ret = pjLibIntVerReq(cxtId, secInp, mBuf, macI, status);
#endif

   RETVALUE(ret);

} /* end of pjUtlIntVerReq */
#endif




/**
 *
 * @brief 
 *
 *        Utility function that processes the CfgReq with type as
 *        REESTABLISH sent from RRC.
 *
 * @b Description:
 *
 *        This function 
 *        1. Marks the state of the RBs as PJ_STATE_REEST.
 *        2. Calculates the number of RBs undergoing reestablishment.
 *
 *  @param[in] ueCb     UE Control Block Pointer.
 *  @param[in] numEnt   Number of entities undergoing reestablishment
 *  @param[in] cfgEnt   List of entities configured.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlUlReEstSrb1
(
PjCb       *gCb,
PjUlRbCb   *pjRbCb                    /* PDCP Control Block Pointer */
)
#else
PUBLIC S16 pjUtlUlReEstSrb1(pjRbCb)
PjCb       *gCb;
PjUlRbCb   *pjRbCb;                   /* PDCP Control Block Pointer */
#endif
{
   PjUlUeCb   *ueCb;

   TRC2(pjUtlReEstSrb1)

   RLOG2(L_DEBUG, "pjUtlReEstSrb1(pjRbCb(%d,%d))",
                    pjRbCb->rbId, pjRbCb->rbType);

   ueCb     = pjRbCb->ueCb;

   ueCb->libInfo.state  =  PJ_STATE_REEST;

   /* SRB Reconfiguration we can update the control Plane keys and
      the Integrity Keys in the OBD module */

   pjUtlUlUpdCpSecKeys(gCb,ueCb);

   /* Perform  uplink reestablishment of SRB1 */
   pjUlmReEstSrb(gCb,pjRbCb);

   /* Start security reconfiguration if SRB1 is the only RB */
   if ((ueCb->numSrbs == 1) && (ueCb->numDrbs == 0))
   {
     pjUtlUlUpdUpSecKeys(gCb,ueCb);
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
PUBLIC S16 pjUtlUlReEstHO
(
PjCb          *gCb,
PjUlUeCb      *ueCb           /* Number of RBs undergoing reestablishment*/ 
)
#else
PUBLIC S16 pjUtlUlReEstHO(gCb,ueCb) 
PjCb          *gCb;
PjUlUeCb      *ueCb;           /* Number of RBs undergoing reestablishment*/ 
#endif
{
   U8             rbCnt;
   PjUlRbCb     **rbCbLst;
   PjUlRbCb      *pjRbCb;

   TRC2(pjUtlUlReEstHO); 

#ifdef ALIGN_64BIT
   RLOG2(L_DEBUG, "pjUtlReEstHOStart(ueCb(%d,%d)))",
                ueCb->key.ueId, ueCb->key.cellId);
#else
   RLOG2(L_DEBUG, "pjUtlReEstHOStart(ueCb(%d,%d))",
                ueCb->key.ueId, ueCb->key.cellId);
#endif

   /* Perform initialisations on the HO Info structure */
   PJ_ALLOC(gCb,ueCb->hoInfo, sizeof(PjUlHoInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ueCb->hoInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   PJ_ALLOC(gCb,ueCb->hoInfo->hoCfmInfo, 
              sizeof(PjUlHoCfmInfo) *  PJ_MAX_DRB_PER_UE );
 
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ueCb->hoInfo->hoCfmInfo == NULLP )
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      /*ccpu00136858 */      
      PJ_PST_FREE(gCb->u.ulCb->cpjSap.pst.region, gCb->u.ulCb->cpjSap.pst.pool, ueCb->hoInfo, sizeof(PjUlHoInfo));
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
      ueCb->libInfo.numReEstDrb++;

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
       pjRbCb->ulCb.obdCnt   = 0;
       pjRbCb->ulCb.transCmp = 0;
       if(pjRbCb->mode == PJ_DRB_AM)
       {
          pjRbCb->ulCb.firstReEstCnt   =   pjRbCb->ulCb.lastSubCnt + 1;
       }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */

   }/* for(rbCnt .. */ 

   RETVALUE(ROK);

} /* pjUtlReEstHOStart */

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
PUBLIC S16 pjUtlUlUpdUpSecKeys
(
PjCb       *gCb,
PjUlUeCb   *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlUlUpdUpSecKeys(gCb,ueCb)
PjCb         *gCb;
PjUlUeCb     *ueCb;           /* UE Control Block */ 
#endif
{
   PjSec    *secInfo;
   Void     *tmpUpCxt;

   TRC2(pjUtlUlUpdUpSecKeys);

   RLOG2(L_DEBUG, "pjUtlUlUpdUpSecKeys(ueCb(%d,%d))",
                ueCb->key.ueId, ueCb->key.cellId);

   secInfo  =  &ueCb->secInfo;

   if(ueCb->secInfo.secAct == TRUE)
   {
      /* Initialisation is done first followed by closure since we
       * do not want the same ctxId to be returned for the new init.
       * In ASYNC case, we drop packets if they are returned from a
       * different ctxId. */

      tmpUpCxt  = ueCb->secInfo.upCxtId;    

      pjUtlUlUpInit(gCb,ueCb);
      pjUtlUlCipherClose(gCb,tmpUpCxt);
   }

   RETVALUE(ROK);
} /* pjUtlUlUpdUpSecKeys */

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
PUBLIC S16 pjUtlUlUpdCpSecKeys
(
PjCb       *gCb,
PjUlUeCb   *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlUlUpdCpSecKeys(gCb,ueCb)
PjCb         *gCb;
PjDlUeCb     *ueCb;           /* UE Control Block */ 
#endif
{
   PjSec    *secInfo;
   Void     *tmpIntCxt;
   Void     *tmpCpCxt;

   TRC2(pjUtlDlUpdCpSecKeys);

   RLOG2(L_DEBUG, "pjUtlUlUpdCpSecKeys(ueCb(%d,%d))",
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

      pjUtlUlCpInit(gCb,ueCb);
      pjUtlUlIntInit(gCb,ueCb);

      pjUtlUlIntClose(gCb,tmpIntCxt);
      pjUtlUlCipherClose(gCb,tmpCpCxt);
   }

   RETVALUE(ROK);
} /* pjUtlDlUpdCpSecKeys */

/**
 *
 * @brief 
 *
 *        Function to handle the REESTABLISHMENT COMPLETE 
 *        for each uplink RB
 *
 * @b Description:
 *
 *        This function 
 *
 *  @param[in] ueCb   UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUtlUlHdlRbReEstComplete
(
PjCb         *gCb,
PjUlRbCb     *pjRbCb           /*RB Control Block */ 
)
#else
PUBLIC S16 pjUtlUlHdlRbReEstComplete(ueCb, pjRbCb)
PjCb         *gCb;
PjUlRbCb     *pjRbCb;          /* RB Control Block */ 
#endif
{
   PjUlUeCb      *ueCb;

   TRC2(pjUtlUlHdlRbReEstComplete);

   RLOG1(L_DEBUG, "pjUtlUlHdlRbReEstComplete(rbCb(%d)",
                pjRbCb->rbId);

   ueCb = pjRbCb->ueCb;
   if (ueCb->libInfo.numReEstDrb == 0)
   {
      ueCb->libInfo.numReEstCmp++;
      RETVALUE(ROK);
   }
   else
   {
      ueCb->libInfo.numReEstDrb--;  
   }

   if (ueCb->libInfo.numReEstDrb == 0)
   {
      if (ueCb->libInfo.state == PJ_STATE_REEST)
      {
         /* Since we have received all the packets in uplink
            we can update the user keys */
         pjUtlUlUpdUpSecKeys(gCb, ueCb);

#ifdef PJ_ASYNC_CFM
        RETVALUE(ROK);
#endif
        pjUtlUlSndReEstCfgCfm(gCb,ueCb);
      }
      else if (ueCb->libInfo.state == PJ_STATE_REEST_HO)
      {
         pjUtlUlSndSduStaCfm(gCb,ueCb);
      }
   }
   RETVALUE(ROK);

} /* pjUtlUlHdlRbReEstComplete */

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
PUBLIC S16 pjUtlUlSndReEstCfgCfm
(
PjCb         *gCb,
PjUlUeCb       *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlUlSndReEstCfgCfm(ueCb)
PjCb         *gCb;
PjUlUeCb       *ueCb;           /* UE Control Block */ 
#endif
{
   CpjCfgCfmInfo *cfgCfmInfo;
   U8             txIdx;
   PjAsyncCfm    *asyncCfm    = NULLP;
   PjCpjSapCb    *cpjSap;

   TRC2(pjUtlSndReEstCfgCfm);

   RLOG2(L_DEBUG, "pjUtlSndReEstCfgCfm(ueCb(%d,%d))",
                ueCb->key.ueId, ueCb->key.cellId);

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
   /* Memory leak fix ccpu00135359 */
   cpjSap = &(gCb->u.ulCb->cpjSap);
   if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,(Data **)&cfgCfmInfo,
      sizeof (CpjCfgCfmInfo)) != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   PJ_FILL_REEST_CFG_CFM_INFO(cfgCfmInfo, asyncCfm);
   PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

   PjUiCpjCfgCfm(&gCb->u.ulCb->cpjSap.pst, gCb->u.ulCb->cpjSap.suId, cfgCfmInfo);         

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
PUBLIC S16 pjUtlUlSaveCfmInfo
(
PjCb        *gCb,
PjUlUeCb      *ueCb,           /* Pointer to UeCb */ 
U8          cfmType,         /* Confirm type */
Bool        startTmr,        /* Start Timer */
U8          entity,          /* Confirm type */
PTR         cfmPtr,          /* Pointer to the structure for confirm */
PTR         cfgPtr           /* Pointer to the structure for request */
)
#else
PUBLIC S16 pjUtlUlSaveCfmInfo(ueCb, cfmType, startTmr, entity, cfmPtr, cfgPtr)
PjCb        *gCb;
PjUlUeCb      *ueCb;           /* Pointer to UeCb */ 
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


   TRC3(pjUtlSaveCfmInfo)

   RLOG_ARG4(L_DEBUG,DBG_UEID,  ueCb->key.ueId,
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
        (U16)((ueCb->libInfo.nxtAvlbTxId + 1) % PJ_MAX_ASYNC_CFM); /*KW_FIX*/

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
PUBLIC S16 pjUtlUlHdlObdInitCfm
(
PjCb        *gCb,
PjUlUeCb      *ueCb,           /* Pointer to UeCb */ 
U16         txIdx,           /* Transaction Index for UeCb */
U8          cfmType,         /* Confirm type */
U8          maskVal          /* mask value */
)
#else
PUBLIC S16 pjUtlUlHdlObdInitCfm(ueCb, txIdx, cfmType, maskVal)
PjCb        *gCb;
PjUlUeCb      *ueCb;           /* Pointer to UeCb */ 
U16         txIdx;           /* Transaction Index for UeCb */
U8          cfmType;         /* Confirm type */
U8          maskVal;         /* mask value */
#endif
{
   CpjSecCfgCfmInfo *secCfgCfm;   /* Security config confirm */
   CpjReEstCfmInfo  *reEstCfm;    /* Reest config confirm */
   PjAsyncCfm       *asyncCfm;
   CpjCfgCfmInfo    *cfgCfm;
   U8               idx;
   S16              ret;
   TRC3(pjUtlUlHdlObdInitCfm)

   RLOG_ARG4(L_DEBUG,DBG_UEID,  ueCb->key.ueId, 
         "pjUtlHdlUlObdInitCfm(ueCb(%d,%d),txIdx(%d),cfmType(%d),maskVal(%d))",
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
      ret = pjUtlUlHdlSecInitCfm(gCb,ueCb,txIdx,asyncCfm,secCfgCfm,reEstCfm);
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

      ret = pjUtlUlHdlCmpInitCfm(gCb,ueCb,txIdx,asyncCfm,cfgCfm);
   }

   RETVALUE(ret);
}/* pjUtlHdlUlObdInitCfm */

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
PUBLIC S16 pjUtlUlHdlInitObdTmrExp
(
PjCb     *gCb,
PjUlUeCb *ueCb
)
#else
PUBLIC S16 pjUtlUlHdlInitObdTmrExp(ueCb)
PjCb     *gCb;
PjUlUeCb *ueCb;
#endif
{
   S16               ret;           /* Return Value */
   CpjSecCfgCfmInfo  *secCfgCfm;    /* Security Cfg Confirm Info */
   CpjCfgCfmInfo     *cfgCfm;       /* Config Confirm Info */
   CpjReEstCfmInfo   *reEstCfm;     /* Reest config confirm */
   CpjCfgCfmInfo     *pjCfgCfm;
   PjLibInfo         *libInfo;      /* Off-board Info */
   PjAsyncCfm        *asyncCfm;     /* Async Cfm Info */
   PjCpjSapCb        *cpjSap;       /* CPJ SAP Control Block */
   U16               txIdx;         /* Tx Idx */
   U16               idx;           /* Index for looping */
   U16               cfgIdx;

   TRC3(pjUtlHdlInitObdTmrExp)

   RLOG2(L_DEBUG, "pjUtlHdlInitObdTmrExp(ueCb(%d,%d))",
                ueCb->key.ueId, ueCb->key.cellId);

   ret       = ROK;
   cpjSap    = &(gCb->u.ulCb->cpjSap);
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
      PJ_ALLOC(gCb,secCfgCfm, sizeof(CpjSecCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (secCfgCfm == NULLP)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
      PJ_FILL_SEC_CFM_INFO(secCfgCfm, asyncCfm);
      secCfgCfm->status = CPJ_CFG_CFM_NOK;
      secCfgCfm->reason = CPJ_CFG_REAS_OBD_TIMEOUT;

      PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

      PjUiCpjSecCfgCfm(&(cpjSap->pst), cpjSap->suId, secCfgCfm);
   }

   /* Configuration confirms */
   else if ((asyncCfm->cfmType & PJ_CFG_ASYNC_CFM) ||
            (asyncCfm->cfmType & PJ_CFG_REEST_ASYNC_CFM))
   {
      PJ_FILL_TIMEOUT_CFG_CFM_INFO(gCb,cfgCfm, asyncCfm);

      PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

      PjUiCpjCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
   }
   else if (asyncCfm->cfmType & PJ_REEST_ASYNC_CFM)
   {
      /* Send ReEstCfm */
      PJ_ALLOC(gCb,reEstCfm, sizeof(CpjReEstCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (reEstCfm == NULLP)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
      PJ_FILL_REEST_CFM_INFO(reEstCfm, asyncCfm, LCM_PRIM_NOK);

      PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

      PjUiCpjReEstCfm(&gCb->u.ulCb->cpjSap.pst, 
                       gCb->u.ulCb->cpjSap.suId, reEstCfm);

      /* Send the config confirm also if any exists */
      cfgIdx   =  (U16)(txIdx + 1);  /*KW_FIX*/

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

      PjUiCpjCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
   }
   else if(asyncCfm->cfmType & PJ_CFG_UEDEL_ASYNC_CFM)
   {
      /* Delete ueCb entry from ueLstCp */
      pjStopTmr(gCb,(PTR)ueCb, PJ_EVT_OBD_WAIT_TMR);
      ret = cmHashListDelete(&(gCb->u.ulCb->ueLstCp), (PTR) ueCb);
      if (ret != ROK)
      {

#ifdef DEBUGP
         RLOG_ARG0(L_DEBUG,DBG_UEID,ueCb->key.ueId, 
               "pjUtlHdlInitObdTmrExp: cmHashListDelete Failed for ueCb.");
#endif
      }

      if ( asyncCfm->entity == ENTPJ )
      {
         PJ_ALLOC(gCb,cfgCfm, sizeof(CpjCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (cfgCfm == NULLP)
         {
            RLOG0(L_FATAL, "Memory Allocation failed.");
            RETVALUE(RFAILED);
         }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

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

         PJ_FREE(gCb,ueCb, sizeof(PjUlUeCb));
         PjUiCpjCfgCfm(&(cpjSap->pst), cpjSap->suId, cfgCfm);
      }
      else if ( asyncCfm->entity == ENTPJ )
      {
         PJ_ALLOC(gCb,pjCfgCfm, sizeof(CpjCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (pjCfgCfm == NULLP)
         {
            RLOG0(L_FATAL, "Memory Allocation failed.");
            RETVALUE(RFAILED);
         }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
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

         PJ_FREE(gCb,ueCb, sizeof(PjUlUeCb));
         PjUiCpjCfgCfm(&(cpjSap->pst), cpjSap->suId, pjCfgCfm);
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
PUBLIC S16 pjUtlUlSndSduStaCfm
(
PjCb         *gCb,
PjUlUeCb     *ueCb           /* UE Control Block */ 
)
#else
PUBLIC S16 pjUtlUlSndSduStaCfm(gCb, ueCb)  
PjCb         *gCb;
PjUlUeCb     *ueCb;           /* UE Control Block */ 
#endif
{
   U8       rbCnt;
   CpjSduStaCfmInfo  *cfmInfo;
   UdxSduStaCfmInfo  *udxCfmInfo;
   UdxSduStaInfo tempDlStaInfo[PJ_MAX_DRB_PER_UE] = {{0}};
   U8 rbId;
   U8 numRb = 0;
   PjUlHoCfmInfo *hoCfmInfo;
   CpjSduStaInfo  *staInfo;
   Bool rbPres;
   PjCpjSapCb     *cpjSap;
   PjUdxUlSapCb   *udxSap;
   

   TRC2(pjUtlUlSndSduStaCfm); 

   RLOG2(L_DEBUG, "pjUtlUlSndSduStaCfm(ueCb(%d,%d))",
                ueCb->key.ueId, ueCb->key.cellId);

   cpjSap = &(gCb->u.ulCb->cpjSap);
   if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,(Data **)&cfmInfo,
      sizeof (CpjSduStaCfmInfo)) != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   cfmInfo->ueId = ueCb->key.ueId;
   cfmInfo->cellId = ueCb->key.cellId;
   cfmInfo->transId = ueCb->hoInfo->transId;
   cfmInfo->reason = CPJ_CFG_REAS_NONE;
   udxCfmInfo      = ueCb->hoInfo->staCfm;

   for(rbCnt = 0; rbCnt < udxCfmInfo->numRb; rbCnt++)
   {
      /*
      - Check for rbId and copy the info to index=(rbId)
      */
      rbId = udxCfmInfo->sduStaInfo[rbCnt].rbId;
      if(udxCfmInfo->sduStaInfo[rbCnt].dlSduStaInfo.hoPres == TRUE)
      {
         tempDlStaInfo[rbId].rbId = udxCfmInfo->sduStaInfo[rbCnt].rbId;
         tempDlStaInfo[rbId].dir = udxCfmInfo->sduStaInfo[rbCnt].dir;
         tempDlStaInfo[rbId].dlSduStaInfo.count = udxCfmInfo->sduStaInfo[rbCnt].dlSduStaInfo.count;
         tempDlStaInfo[rbId].dlSduStaInfo.hoPres = TRUE;
      }
   } 

   for(rbCnt = 0;
      ((rbCnt < PJ_MAX_DRB_PER_UE) && (numRb < CPJ_MAX_DRB)); rbCnt++)
   {
      rbPres     = FALSE;
      hoCfmInfo   =  &ueCb->hoInfo->hoCfmInfo[rbCnt];
      staInfo     =  &cfmInfo->sduStaInfo[numRb];
      memset((void *)staInfo,0,sizeof(CpjSduStaInfo));
      if(hoCfmInfo->pres == TRUE)
      {                          
         rbPres = TRUE;
         staInfo->rbId  =  hoCfmInfo->rbId;
         staInfo->dir   |= hoCfmInfo->dir;
         staInfo->snLen = ueCb->drbCb[hoCfmInfo->rbId]->snLen;
         staInfo->ulSduStaInfo.numBits   =  hoCfmInfo->numBits;
         if (hoCfmInfo->numBits > 0) 
         {
            /* numBits is always filled as multiple's of 8 */
            cpjSap = &(gCb->u.ulCb->cpjSap);
            if(SGetSBuf(cpjSap->pst.region,cpjSap->pst.pool,
                        (Data **)&staInfo->ulSduStaInfo.ulBitMap,
                        sizeof (U8) * (hoCfmInfo->numBits / 8)) != ROK)
            {
               RLOG0(L_FATAL, "Memory Allocation failed.");
               RETVALUE(RFAILED);
            }
            PJ_MEM_CPY(staInfo->ulSduStaInfo.ulBitMap, hoCfmInfo->ulBitMap, (hoCfmInfo->numBits / 8));
            PJ_FREE(gCb, hoCfmInfo->ulBitMap, sizeof(U8) * (hoCfmInfo->numBits / 8));
         }
         staInfo->ulSduStaInfo.count     =  hoCfmInfo->count;
      }
      if(tempDlStaInfo[rbCnt].dlSduStaInfo.hoPres == TRUE)
      { 
         rbPres = TRUE;
         staInfo->rbId  =  tempDlStaInfo[rbCnt].rbId;
         staInfo->dir   |=  tempDlStaInfo[rbCnt].dir;
         staInfo->snLen = ueCb->drbCb[staInfo->rbId]->snLen;
         staInfo->dlSduStaInfo.count = tempDlStaInfo[rbCnt].dlSduStaInfo.count;
      }
      if(rbPres != TRUE)
      {
         continue;
      }
      else
      {
        numRb++;
      }
   }

   cfmInfo->numRb = numRb; 
   cfmInfo->status = ROK;
   cfmInfo->reason = CPJ_CFG_REAS_NONE;
   PJ_FREE(gCb,ueCb->hoInfo->hoCfmInfo,
              (PJ_MAX_DRB_PER_UE * sizeof(PjUlHoCfmInfo)));
   /* Memory leak fix ccpu00135359 */
   udxSap = PJ_GET_UL_UDX_SAP(gCb);
   PJ_PST_FREE(udxSap->pst.region, udxSap->pst.pool,ueCb->hoInfo->staCfm,
                sizeof (UdxSduStaCfmInfo));
   PJ_FREE(gCb,ueCb->hoInfo, sizeof(PjUlHoInfo));
   
   PjUiCpjSduStaCfm(&gCb->u.ulCb->cpjSap.pst, 
                     gCb->u.ulCb->cpjSap.suId, cfmInfo);
    
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
PUBLIC S16 pjUtlUlShutdown
(
PjCb  *gCb
)
#else
PUBLIC S16 pjUtlUlShutdown()
PjCb   *gCb;
#endif
{
   S16         ret;              /* Return Value */
   PjUlUeCb      *ueCb;            /* UE Control Block */
   PjUlUeCb      *prevUeCb;        /* Previos UE Control Block */
   PjUlRbCb        *rbCb;            /* RB Control Block */
   PjUlRbCb        **rbCbLst;        /* RB Control Block List */
   PjSec       *secInfo;          /* Security info of UE */
   U8          idx;              /* Index for RBs */
#ifdef LTE_L2_MEAS_COMMENT
   PjL2MeasEvtCb  *measEvtCb = NULLP;
#endif
   TRC3(pjUtlShutdown)

   RLOG0(L_DEBUG, "pjUtlShutdown()");

   ret      = ROK;
   ueCb     = NULLP;
   prevUeCb = NULLP;
   rbCb     = NULLP;
   rbCbLst  = NULLP;
   idx      = 0;

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (cmHashListGetNext(&(gCb->u.ulCb->ueLstCp), (PTR) prevUeCb, (PTR *)&ueCb) == ROK)
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
               pjUtlUlFreeRb(gCb,rbCb);
             }
         }
         rbCbLst = ueCb->drbCb;
         for (idx = 0; idx < PJ_MAX_DRB_PER_UE; idx++)
         {
             rbCb = rbCbLst[idx];
             if (rbCb != NULLP)
             {
                pjUtlUlFreeRb(gCb,rbCb);
             }
         }
         /* Close all the context info for offboarding */
         secInfo = &(ueCb->secInfo);
         pjUtlUlCipherClose(gCb,secInfo->cpCxtId);
         pjUtlUlCipherClose(gCb,secInfo->upCxtId);
         pjUtlUlIntClose(gCb,secInfo->intCxtId);

         /* Delete hoInfo if present */
         if (ueCb->hoInfo != NULLP)
         {
            for (idx = 0; idx < PJ_MAX_DRB_PER_UE; idx++)
            {
               if (ueCb->hoInfo->hoCfmInfo[idx].pres == TRUE)
               {
                  PJ_FREE(gCb,ueCb->hoInfo->hoCfmInfo[idx].ulBitMap,
                        (ueCb->hoInfo->hoCfmInfo[idx].numBits % 8)?
                        ((ueCb->hoInfo->hoCfmInfo[idx].numBits / 8) + 1):
                        (ueCb->hoInfo->hoCfmInfo[idx].numBits / 8 ));
               }
            }

            PJ_FREE(gCb,ueCb->hoInfo->hoCfmInfo,                         \
                           (PJ_MAX_DRB_PER_UE * sizeof(PjUlHoCfmInfo)));
            PJ_FREE(gCb,ueCb->hoInfo, sizeof(PjUlHoInfo));                 
         }
      }
      prevUeCb = ueCb;
   }
#ifdef LTE_L2_MEAS_COMMENT
   for(idx = 0; idx < PJ_MAX_L2MEAS_EVT; idx++)
   {
      if(pjCb.pjL2Cb.pjMeasEvtCb[idx] != NULLP)
      {
         measEvtCb = pjCb.pjL2Cb.pjMeasEvtCb[idx];
         pjStopTmr(gCb,(PTR)measEvtCb, PJ_EVT_L2_TMR);
         gCb.pjL2Cb.pjNumMeas--;
         PJ_FREE(gCb,measEvtCb, sizeof(PjL2MeasEvtCb));
         pjCb.pjL2Cb.pjMeasEvtCb[idx] = NULLP;
      }
   }
#endif
   ret = pjDbmUlDeInit(gCb);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      RLOG0(L_FATAL, "PDCP UL DeInitialization Failed.\n");
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */
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
PUBLIC Void pjUtlUlFreeRb
(
PjCb     *gCb,
PjUlRbCb *pjRbCb
)
#else
PUBLIC Void pjUtlUlFreeRb(pjRbCb)
PjCb     *gCb;
PjUlRbCb *pjRbCb;
#endif
{
   TRC3(pjUtlFreeUlRb)

   RLOG0(L_DEBUG, "pjUtlFreeRb()");

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   if (pjRbCb->ulCb.obdTmr.tmrEvnt == PJ_EVT_UL_OBD_TMR)
   {
       pjStopTmr(gCb,(PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
   }
#endif
   pjDbmRxDeInit(gCb,&(pjRbCb->ulCb.recBuf));
   
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
PUBLIC S16 pjUtlUlSndDatFwdInd
(
PjCb             *gCb,
PjUlRbCb           *pjRbCb,
PjuDatFwdIndInfo *datFwdInd
)
#else
PUBLIC S16 pjUtlUlSndDatFwdInd(pjRbCb, datFwdInd)
PjCb             *gCb;
PjUlRbCb           *pjRbCb;
PjuDatFwdIndInfo *datFwdInd;
#endif
{
   CmLtePdcpId  *pdcpId;
   PjPjuSapCb   *pjuSap;
   CmLtePdcpId   pdcpIdTmp;

   TRC3(pjUtlUlSndDatFwdInd)
   
   RLOG0(L_DEBUG, "pjUtlSndDatFwdInd()");

   pjuSap   = &(gCb->u.ulCb->pjuSap[PJ_DRB_SAP]);
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
PUBLIC S16 pjUtlUlSndUlStaRep
(
PjCb             *gCb,
PjUlRbCb         *pjRbCb,
Buffer           *sta
)
#else
PUBLIC S16 pjUtlUlSndUlStaRep(pjRbCb, sta)
PjCb             *gCb;
PjUlRbCb         *pjRbCb;
Buffer           *sta;
#endif
{
   PjUdxUlSapCb *udxSap;
   UdxUlStaRepInfo   *staRep;
   
   udxSap = PJ_GET_UL_UDX_SAP(gCb);

   TRC3(pjUtlUlSndUlStaRep)
   
   RLOG0(L_DEBUG, "pjUtlUlSndUlStaRep()");

   /* Memory leak fix ccpu00135359 */
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&staRep,
      sizeof (UdxUlStaRepInfo)) != ROK)
   {
     staRep = NULLP;
   }

   if(staRep != NULLP)
   {
      staRep->pdcpId.ueId   =  pjRbCb->ueCb->key.ueId;
      staRep->pdcpId.cellId =  pjRbCb->ueCb->key.cellId;
      staRep->pdcpId.rbId   =  pjRbCb->rbId;
      staRep->pdcpId.rbType =  pjRbCb->rbType;
      staRep->sta = sta;

      PjUlUdxUlStaRep(&(udxSap->pst), udxSap->spId, staRep);
   }
   else
   {
      PJ_FREE_BUF(sta);
   }

   RETVALUE(ROK);
}/* end of pjUtlSndDatFwdInd */


/**
 * 
 * @brief 
 *
 *        Handler to send Feedback packet to DL-PDCP
 *
 * @b Description:
 *      This function sends Feedback packet to DL-PDCP 
 *
 *  @param[in]  pjCb       PDCP global control block
 *  @param[in]  pjRbCb     PDCP RbCb
 *  @param[in]  fbPkt      Feedback packet
 *
 *
 *  @return  S16
 *      -# ROK 
 *
 */

#ifdef ANSI
PUBLIC S16 pjUtlUlSndUlRohcFdbk
(
PjCb             *gCb,
PjUlRbCb         *pjRbCb,
Buffer           *fbPkt
)
#else
PUBLIC S16 pjUtlUlSndUlRohcFdbk(gCb, pjRbCb, fbPkt)
PjCb             *gCb;
PjUlRbCb         *pjRbCb;
Buffer           *fbPkt;
#endif
{
   PjUdxUlSapCb *udxSap;
   UdxUlFdbkPktInfo   *fdbkPktInfo;
   
   udxSap = PJ_GET_UL_UDX_SAP(gCb);

   TRC3(pjUtlUlSndUlRohcFdbk)
   
   RLOG0(L_DEBUG, "pjUtlUlSndUlRohcFdbk()");

   /* Memory leak fix ccpu00135359 */
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&fdbkPktInfo,
      sizeof (UdxUlFdbkPktInfo)) != ROK)
   {
      fdbkPktInfo= NULLP;
   }

   if(fdbkPktInfo != NULLP)
   {
      fdbkPktInfo->pdcpId.ueId   =  pjRbCb->ueCb->key.ueId;
      fdbkPktInfo->pdcpId.cellId =  pjRbCb->ueCb->key.cellId;
      fdbkPktInfo->pdcpId.rbId   =  pjRbCb->rbId;
      fdbkPktInfo->pdcpId.rbType =  pjRbCb->rbType;
      fdbkPktInfo->fbPkt = fbPkt;

      PjUlUdxUlFdbkPktInfo(&(udxSap->pst), udxSap->spId, fdbkPktInfo);
   }
   else
   {
      PJ_FREE_BUF(fbPkt);
   }

   RETVALUE(ROK);
}/* end of pjUtlUlSndUlRohcFdbk */

/*
 * @brief: This function will reset the rxEnt->count of 
 * forwarded buffered packets
 *
 * @b Description: Retrieve the buffered rxEnt.
 *                 Store its contents into a new rxEnt.
 *                 Store the new Count Value in rxEnt.
 *                 Delete the old retrieved rxEnt &
 *                 Insert the rxEnt with new Count.
 *
 */
#ifdef ANSI 
PUBLIC S16 pjUtlUlResetBuffrdPkts
(
PjCb           *gCb,
PjUlRbCb       *pjRbCb
)
#else
PUBLIC S16 pjUtlUlResetBuffrdPkts(gCb, pjRbCb)
PjCb           *gCb;
PjUlRbCb       *pjRbCb;
#endif
{
   PjRxEnt       *oldRxEnt  = NULLP; /* Old Rx Entry pointer */
   PjRxEnt       *rxEnt  = NULLP;    /* Rx Entry pointer */
   U32            pktCount = 0;      /* Number of Packets Buffered in datPktQ */ /*KW_FIX*/
   U16            fms = 0;

   TRC2(pjUtlUlHdlDatFwdReq);

   fms = pjRbCb->ulCb.nxtRxSn; 
   pktCount = pjRbCb->ulCb.recBuf.datPktQ.count;

   cmLListFirst(&pjRbCb->ulCb.recBuf.datPktQ);
   while((cmLListCrnt(&pjRbCb->ulCb.recBuf.datPktQ)) && (pktCount > 0))
   {
      /* Create and insert the entry in the buffer */
      PJ_ALLOC(gCb,rxEnt, sizeof(PjRxEnt));

#if (ERRCLASS & ERRCLS_DEBUG)
      if(rxEnt == NULLP)
      {
         RLOG_ARG0(L_DEBUG,DBG_UEID,pjRbCb->ueCb->key.ueId, 
               "pjUlmHdlDatFwdReq: Failure in allocating an rxEnt");
         RETVALUE(RFAILED);
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      oldRxEnt = (PjRxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->ulCb.recBuf.datPktQ));
      /* Update the Count Value based on the HFN received */
      if (pjRbCb->snLen == PJ_12_BIT_SN)
      {
         if((oldRxEnt->count & 0x0fff) > fms)
         {
            /* SN is greater than FMS, so this SN belongs to the HFN sent by Source */
            rxEnt->count = (((pjRbCb->ulCb.rxHfn) << (pjRbCb->snLen)) | (oldRxEnt->count & 0x0fff));
            pktCount--;
         }
         else
         {
            /* SN is lesser than FMS, so this SN belong to the HFN + 1 sent by Source 
             * The Max can be only till HFN + 1 because BitMap length sent from Source
             * cannot exceed 4096 */
            rxEnt->count = (((pjRbCb->ulCb.rxHfn + 1) << (pjRbCb->snLen)) | (oldRxEnt->count & 0x0fff));
            pktCount--;
         }
      }
      else if (pjRbCb->snLen == PJ_15_BIT_SN)
      {
         if((oldRxEnt->count & 0x7fff) > fms)
         {
            /* SN is greater than FMS, so this SN belongs to the HFN sent by Source */
            rxEnt->count = (((pjRbCb->ulCb.rxHfn) << (pjRbCb->snLen)) | (oldRxEnt->count & 0x7fff));
            pktCount--;
         }
         else
         {
            /* SN is lesser than FMS, so this SN belong to the HFN + 1 sent by Source 
             * The Max can be only till HFN + 1 because BitMap length sent from Source
             * cannot exceed 4096 */
            rxEnt->count = (((pjRbCb->ulCb.rxHfn + 1) << (pjRbCb->snLen)) | (oldRxEnt->count & 0x7fff));
            pktCount--;
         }
      }

      rxEnt->state     = PJ_RDY_TO_SUBMIT;
      SCpyMsgMsg(oldRxEnt->mBuf, gCb->init.region, gCb->init.pool, &rxEnt->mBuf);
      rxEnt->isOutSeq     = oldRxEnt->isOutSeq;

      /* Insert the entry in the buffer */
      pjDbmInsRxEnt(gCb, &pjRbCb->ulCb.recBuf, rxEnt, FALSE);
      /* Delete the old RxEnt entry */
      pjDbmDelRxEnt(gCb, &pjRbCb->ulCb.recBuf, oldRxEnt->count);
      cmLListNext(&pjRbCb->ulCb.recBuf.datPktQ);
   }

   RETVALUE(ROK);

}/* End - pjUtlUlResetBuffrdPkts() */


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
S16 pjUtlL2MeasUlInit(PjCb *gCb)
{
   U16   cntr;

   gCb->u.ulCb->pjL2Cb.pjNumMeas=0;
   for(cntr = 0; cntr < LPJ_MAX_L2MEAS; cntr++)
   {
      cmMemset((U8 *)&(gCb->u.ulCb->pjL2Cb.pjL2EvtCb[cntr]), 0, sizeof(PjL2MeasEvtCb));
   }
   gCb->u.ulCb->pjL2Cb.pjL2EvtCb[PJ_L2MEAS_UL_LOSS].measCb.measType = LPJ_L2MEAS_UL_LOSS;
   gCb->u.ulCb->pjL2Cb.pjL2EvtCb[PJ_L2CPU_PERCORE_STATS].measCb.measType = LPJ_L2CPU_PERCORE_STATS;
   gCb->u.ulCb->pjL2Cb.pjL2EvtCb[PJ_L2MEM_PERPOOL_STATS].measCb.measType = LPJ_L2MEM_PERPOOL_STATS;
   /* initialize the timer value for memory and CPU utilization */
   gCb->u.ulCb->pjL2Cb.measTmrCfg.enb = TRUE;
   gCb->u.ulCb->pjL2Cb.measTmrCfg.val = CM_MEM_CPU_UITL_INFO_TMR_VAL;
   cmInitTimers(&gCb->u.ulCb->pjL2Cb.measTmr,PJ_L2_MAX_TIMERS); 
   
   RETVALUE(ROK);
}
/**
 *
 * @brief Handler for resetting the DL measurement counters 
 *
 *
 * @b Description
 *
 * @param[in] measCb    Measurement Control Block.
 *
 *
 * @return  Void
 */
#ifdef ANSI

PUBLIC Void pjUtlResetUlL2MeasCntr
(
PjCb         *tPjCb,
PjL2MeasCb   *measCb,
U8           measType
)
#else
PUBLIC Void pjUtlResetUlL2MeasCntr(tPjCb, measCb, measType)
PjCb         *tPjCb;
PjL2MeasCb   *measCb;
U8           measType;
#endif
{
   if (measCb->measType & LPJ_L2MEAS_UL_LOSS)
   {
      U32 qciIdx;
      for(qciIdx = 1; qciIdx < LPJ_MAX_QCI; qciIdx++)
      {
         U8 qci = measCb->qci[qciIdx];

         measCb->measData[qci].ulLoss.val = 0;
         measCb->measData[qci].ulLoss.numPkts = 0;
         tPjCb->u.ulCb->pjL2Cb.measOn[qci] &= ~LPJ_L2MEAS_UL_LOSS;
      }
   }
   if (measCb->measType & LPJ_L2MEM_PERPOOL_STATS)
   {
     tPjCb->u.ulCb->pjL2Cb.memUtilizationMask = 0;
     /* Clear Memory Statistics */
     cmClearMemUtilizationCounter(&(measCb->memInfo));
   }
   if(measCb->measType & LPJ_L2CPU_PERCORE_STATS )
   {
     tPjCb->u.ulCb->pjL2Cb.cpuUtilizationMask = 0;
   }

}


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
PUBLIC S16 pjUtlSndUlL2MeasNCfm
(
PjCb            *gCb,
PjL2MeasCfmEvt  *measCfmEvt
)
#else
PUBLIC S16 pjUtlSndUlL2MeasNCfm(gCb, measCfmEvt)
PjCb            *gCb;
PjL2MeasCfmEvt  *measCfmEvt;
#endif
{

   TRC3(pjUtlSndL2MeasNCfm)

   PjMiLpjL2MeasCfm(&gCb->pjGenCfg.lmPst, measCfmEvt);

   RETVALUE(ROK);
} /* pjUtlSndL2MeasNCfm */

/**
 *
 * @brief Handler for storing address of MeasData in rbCb at right index
 *
 *
 * @b Description
 *        Handler for storing address of MeasData in rbCb at right index.
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
PUBLIC Void pjUtlPlcMeasDatInUlL2Sts
(
PjL2Cntr       *measData, 
PjL2MeasRbCb   *rbL2Cb,
U8             measType
)
#else
PUBLIC Void pjUtlPlcMeasDatInUlL2Sts(measData, rbL2Cb, measType)
PjL2Cntr       *measData; 
PjL2MeasRbCb   *rbL2Cb;
U8             measType;
#endif
{
   TRC3(pjUtlPlcMeasDatInUlL2Sts)
   
   /* We should check the number of measType in the request. This can be done 
    * by looking at each bit in the measType. Also store the measData in the 
    * correct index of l2Sts in RbCb.
    * */

    if(measType & LPJ_L2MEAS_UL_LOSS)
    {
         rbL2Cb->l2Sts[PJ_L2MEAS_UL_LOSS] = measData;
    }

}/* End of pjUtlPlcMeasDatInUlL2Sts */
#endif /* LTE_L2_MEAS */


/**
@brief 
This function processes Data Resume request received from the Application.
 *  @param[in] gCb      PDCP Instance Control block 
 *  @param[in] ueCb     Uplink UeCb 
 *  @return S16
 *      -# Success : ROK
 *      -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 pjUtlUlPrcsDatResume
(
PjCb                 *gCb,
CpjDatResumeReqInfo  *datResReq
)
#else
PUBLIC S16 pjUtlUlPrcsDatResume(gCb, datResReq)
PjCb                 *gCb;
CpjDatResumeReqInfo  *datResReq;
#endif
{
   U8 rbCnt;
   PjUlRbCb *pjRbCb = NULLP;
   PjUlUeCb *ueCb= NULLP;
   
   pjDbmFetchUlUeCb(gCb, datResReq->ueId, datResReq->cellId, &ueCb);
   if(!ueCb)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID, datResReq->cellId,"UeId[%u] not found",
            datResReq->ueId);
      RETVALUE(RFAILED);
   }   

   for(rbCnt = 0; rbCnt < PJ_MAX_DRB_PER_UE; rbCnt ++)
   {
      /* Check of rbCb available */
      if( (pjRbCb = ueCb->drbCb[rbCnt]) == NULLP)
      {
         continue;
      }
#ifdef LTE_L2_MEAS
      pjRbCb->ulCb.nxtSubSn = pjRbCb->ulCb.nxtRxSn;
#endif

      /* Process the packets in dlPktQ */
      pjUlmProcessUlPktQ(gCb, pjRbCb);
      pjRbCb->state = PJ_STATE_NORMAL;
   }
   ueCb->libInfo.state = PJ_STATE_NORMAL;
   RETVALUE(ROK);
}

PUBLIC Void pjUtlEmptyUlPktList(PjCb *gCb, PjUlRbCb *pjRbCb)
{
   PjUlPkt *ulPkt;
   CmLListCp *ulPktLst = &pjRbCb->ulCb.ulPktLst;
   while(ulPktLst->first)
   {
      ulPkt = (PjUlPkt *)(ulPktLst->first->node);
      cmLListDelFrm(ulPktLst, ulPktLst->first);
      PJ_FREE_BUF(ulPkt->pdu);
      PJ_FREE(gCb, ulPkt, sizeof(PjUlPkt));
   }
   RETVOID;
}

void dumpPDCPUlRbInformation(PjUlRbCb* ulRbCb, U16 ueId)
{
   PjUlCb* ulCb = &ulRbCb->ulCb;

   QU_CRITLOG(QU_LOG_MESSAGE,"UE [%u] PDCP UL RB numEntries = %lu\n",ueId, ulCb->recBuf.numEntries);
}


void DumpPDCPUlDebugInformation(void)
{
   PjCb* ulInst = pjCb[0]; /* TODO: check whether UL is 0 or 1 */

   PjUlgCb* ulgCb = ulInst->u.ulCb;

   PjUlUeCb *ueCb = NULLP; 
   QU_CRITLOG(QU_LOG_MESSAGE,"PDCP UL Information\n");
   
   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&ulgCb->ueLstCp, 
                                   (PTR) ueCb, 
                                   (PTR *)&ueCb))
   {
      U32 i;
      for(i = 0; i < PJ_MAX_SRB_PER_UE; i++)
      {
         PjUlRbCb* rbCb = ueCb->srbCb[i];
         if(rbCb != NULLP)
         {
            dumpPDCPUlRbInformation(rbCb, ueCb->key.ueId);
         }
      }
      for(i = 0; i < PJ_MAX_DRB_PER_UE; i++)
      {
         PjUlRbCb* rbCb = ueCb->drbCb[i];
         if(rbCb != NULLP)
         {
            dumpPDCPUlRbInformation(rbCb, ueCb->key.ueId);
         }
      }
   }/* end while */   
}

#ifdef ANSI
PRIVATE S16 pjUtlUlHdlSecInitCfm
(
PjCb        *gCb,              /* Pointer to PJ DL Control Block */
PjUlUeCb      *ueCb,           /* Pointer to UeCb */ 
U16         txIdx,             /* Transaction Index for UeCb */
PjAsyncCfm       *asyncCfm,    /* Async Confirm */
CpjSecCfgCfmInfo *secCfgCfm,   /* Security config confirm */
CpjReEstCfmInfo  *reEstCfm     /* Reest config confirm */
)
#else
PRIVATE S16 pjUtlUlHdlSecInitCfm(gCb,ueCb,txIdx, asyncCfm,secCfgCfm,reEstCfm)
(                              
PjCb        *gCb;              /* Pointer to PJ DL Control Block */
PjUlUeCb      *ueCb;           /* Pointer to UeCb */ 
U16         txIdx;             /* Transaction Index for UeCb */
PjAsyncCfm       *asyncCfm;    /* Async Confirm */
CpjSecCfgCfmInfo *secCfgCfm;   /* Security config confirm */
CpjReEstCfmInfo  *reEstCfm;    /* Reest config confirm */
)
#endif   
{
   TRC3(pjUtlUlHdlSecInitCfm)

   if(asyncCfm->libInitBitMask == 0)
   {
      PJ_CHK_RESTART_OBD_TIMER(gCb,ueCb, txIdx);
      if(ueCb->libInfo.state == PJ_STATE_NORMAL)
      {
         /* Send security config confirm */
         PJ_ALLOC(gCb,secCfgCfm, sizeof(CpjSecCfgCfmInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
         if (secCfgCfm == NULLP)
         {
            RLOG0(L_FATAL, "Memory Allocation failed.");
            RETVALUE(RFAILED);
         }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
         PJ_FILL_SEC_CFM_INFO(secCfgCfm, asyncCfm);

         PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

         PjUiCpjSecCfgCfm(&gCb->u.ulCb->cpjSap.pst, gCb->u.ulCb->cpjSap.suId, secCfgCfm);
      }
      else if(ueCb->libInfo.state == PJ_STATE_REEST)
      {
         /* Send ReEstCfm */
         PJ_ALLOC(gCb,reEstCfm, sizeof(CpjReEstCfmInfo));
#if (ERRCLASS & ERRCLS_DEBUG)
         if (reEstCfm == NULLP)
         {
            RLOG0(L_FATAL, "Memory Allocation failed.");
            RETVALUE(RFAILED);
         }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

         PJ_FILL_REEST_CFM_INFO(reEstCfm, asyncCfm, LCM_PRIM_OK);

         /* Some house keeping work */
         PJ_CLEAN_AND_UPD_ASYNCINFO(gCb,ueCb, txIdx);

#ifdef PJ_SEC_ASYNC            
         /*Change the state from REEST to NORMAL*/
         ueCb->libInfo.reEstCfmSent = TRUE;
#endif
         PjUiCpjReEstCfm(&gCb->u.ulCb->cpjSap.pst, gCb->u.ulCb->cpjSap.suId, reEstCfm);
      }
   }

 RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 pjUtlUlHdlCmpInitCfm
(
PjCb           *gCb,            /* Pointer to PJ DL Control Block */
PjUlUeCb       *ueCb,           /* Pointer to UeCb */ 
U16            txIdx,           /* Transaction Index for UeCb */
PjAsyncCfm     *asyncCfm,       /* Async Confirm */
CpjCfgCfmInfo  *cfgCfm         /* UDX Config Confirm */
)
#else
PRIVATE S16 pjUtlUlHdlCmpInitCfm(gCb,ueCb,txIdx,asyncCfm,cfgCfm)
(                              
PjCb           *gCb;            /* Pointer to PJ DL Control Block */
PjUlUeCb       *ueCb;           /* Pointer to UeCb */ 
U16            txIdx;           /* Transaction Index for UeCb */
PjAsyncCfm     *asyncCfm;       /* Async Confirm */
CpjCfgCfmInfo  *cfgCfm;         /* UDX Config Confirm */
)
#endif   
{
   U32               idx;

   TRC3(pjUtlUlHdlCmpInitCfm)

      if(asyncCfm->cmpInitBitMask == 0)
      {
         asyncCfm->libInitBitMask ^= (PJ_LIB_COMP_BIT_MASK);

         if((asyncCfm->libInitBitMask == 0) &&
               !(asyncCfm->cfmType & PJ_CFG_REEST_ASYNC_CFM))
         {

            /* Send config confirm */
            PJ_ALLOC(gCb,cfgCfm, sizeof(CpjCfgCfmInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
            if (cfgCfm == NULLP)
            {
               RLOG0(L_FATAL, "Memory Allocation failed.");
               RETVALUE(RFAILED);
            }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
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
            PjUiCpjCfgCfm(&(gCb->u.ulCb->cpjSap.pst), gCb->u.ulCb->cpjSap.suId, cfgCfm);
         }
      }
   RETVALUE(ROK);
}   

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
PjLibTrans *libTrans,              /* Transaction Id */
Status   status                   /* Integrity verification status */
)
#else
PUBLIC S16 PjLibObdIntVerCfm(pst, cxtId, libTrans, status)
Pst      *pst;                    /* post structure */
PjLibTrans *libTrans;              /* Transaction Id */
Status   status;                  /* Integrity verification status */
#endif
{
   PjCb     *tPjCb;
   PjUlRbCb   *pjRbCb;
   PjUlCb   *ulCb;
   PjRxEnt  *rxEnt;
   PjSn     sn;
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */

   TRC2(PjLibObdIntVerCfm)


   tPjCb = pjCb[PJ_SEC_DIR_UL]; 


   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      RETVALUE(RFAILED);
   }

   /* Initialisations */
   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   pjDbmFetchPjUlRbCb((PjUlUeCb*)libTrans->ueCb, (libTrans->rbId + 1), libTrans->rbType,&pjRbCb);
   if(pjRbCb == NULLP)
   {
      PJLOGERROR(tPjCb, ERRCLS_ADD_RES, EPJXXX, (ErrVal)libTrans->rbId,
            "PjLibObdIntVerCfm: PjRbCb is NULLP \n");
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      PJ_STS_INC_GEN_CNT(tPjCb, numIntgVrfFails);
      RETVALUE(RFAILED);
   }
   ulCb  =  &pjRbCb->ulCb;

   /* For reestablishment cases, exit */
   if(pjRbCb->state != PJ_STATE_NORMAL)
   {
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      RETVALUE(ROK);
   }

   /* Get the rxEnt */
   rxEnt =  pjDbmGetRxEnt(tPjCb,&ulCb->recBuf, libTrans->count);
   if(rxEnt == NULLP)
   {
      PJLOGERROR(tPjCb, ERRCLS_ADD_RES, EPJXXX, (ErrVal)libTrans->rbId,
            "PjLibObdIntVerCfm: RxEnt is NULLP \n");
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      PJ_STS_INC_GEN_CNT(tPjCb, numIntgVrfFails);
      RETVALUE(RFAILED);
   }
   PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
   /* Validate status */
   if(status != ROK)
   {
      PJLOGERROR(tPjCb, ERRCLS_ADD_RES, EPJXXX, (ErrVal)rxEnt->count,
            "PjLibObdIntVerCfm: Integrity Failed \n");
      PJ_SND_PJU_STA_IND(tPjCb,pjRbCb, rxEnt);
      PJ_STS_INC_GEN_CNT(tPjCb,numIntgVrfFails);
      pjDbmDelRxEnt(tPjCb, &ulCb->recBuf, rxEnt->count); 
      RETVALUE(RFAILED);
   }

   /* Check if next in line for delivery */
   rxEnt->state   =  PJ_RDY_TO_SUBMIT;
   /*if(rxEnt->count != ulCb->nxtSubCnt)
   {
      RETVALUE(ROK);
   }*/

   /*RsysSHMLog("####### PjLibObdIntVerCfm() pjUlmDeliverSrb");*/

   /* Remove the SN from the mBuf. */
   SRemPreMsg((Data *)&sn, rxEnt->mBuf);

   /* Send for delivery */
   pjUlmDeliverSrb(tPjCb,pjRbCb, rxEnt);

   RETVALUE(ROK);

} /* end of PjLibObdIntVerCfm */

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
PjLibTrans  *libTrans,             /* Transaction Id */
Buffer      *opPdu                /* deciphered SDU */
)
#else
PUBLIC S16 PjLibObdDecipherCfm(pst, cxtId, libTrans, opPdu)
Pst         *pst;                 /* post structure */
PjLibTrans  *libTrans;             /* Transaction Id */
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

   TRC2(PjLibObdDecipherCfm);


   tPjCb = pjCb[PJ_SEC_DIR_UL]; 

   rxEnt = (PjRxEnt *)libTrans->rxEnt;
   if(rxEnt == NULL)
   {
      RETVALUE(RFAILED);
   }

   if(rxEnt->state == PJ_PDU_TO_DELETE)
   {
     /*Free libTrans and rxEnt*/
     PJ_FREE(tPjCb, rxEnt, sizeof(PjRxEnt));
     PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
     PJ_FREE_BUF(opPdu);

     RETVALUE(ROK);
   }

#ifdef ALIGN_64BIT
   PJDBGP(tPjCb,(PJ_DBGMASK_UTL | PJ_DBGMASK_BRIEF),
      (tPjCb->init.prntBuf, "PjLibObdDecipherCfm(pst, cxtId(%ld), libTrans(%d,%d,%d))\n",\
       cxtId, libTrans->rbId, libTrans->rbType, libTrans->count));
#else
   PJDBGP(tPjCb,(PJ_DBGMASK_UTL | PJ_DBGMASK_BRIEF),
      (tPjCb->init.prntBuf, "PjLibObdDecipherCfm(pst, libTrans(%d,%d,%ld))\n",\
       libTrans->rbId, libTrans->rbType, libTrans->count));
#endif

   /* If PDCP Layer is not configured and recieved message from Off-board, then
    * clean the resources recieved and return RFAILED.
    * Else proceed with processing.*/
   /*QU_CRITLOG(QU_LOG_MESSAGE,"PjLibObdDecipherCfm called 1\n" );*/
   if (!(tPjCb->init.cfgDone & PJ_LMM_GENCFG_DONE))
   {
      PJ_FREE_BUF(opPdu);
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      RETVALUE(RFAILED);
   }

   /* pj005.201 ccpu00114955 corrected the RB ID calculation */
   /* 1. Initialisations */
   /*QU_CRITLOG(QU_LOG_MESSAGE,"PjLibObdDecipherCfm called 2\n" );*/
    pjDbmFetchPjUlRbCb((PjUlUeCb*)libTrans->ueCb, (libTrans->rbId + 1), libTrans->rbType,&pjRbCb);
   if(pjRbCb == NULLP)
   {
      PJLOGERROR(tPjCb,ERRCLS_ADD_RES, EPJXXX, (ErrVal)libTrans->rbId,
            "PjLibObdDecipherCfm: PjRbCb is NULLP \n");
      PJ_FREE_BUF(opPdu);
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      PJ_STS_INC_GEN_CNT(tPjCb,numDeciphFails);
      RETVALUE(RFAILED);
   }

   ulCb  =  &pjRbCb->ulCb;

   /*QU_CRITLOG(QU_LOG_MESSAGE,"PjLibObdDecipherCfm called 3\n" );*/
   /* Validating rxEnt will be done later in code */
   /*rxEnt =  pjDbmGetRxEnt(tPjCb,&ulCb->recBuf, libTrans->count);*/

    /* 2. check decipher status */
   if(libTrans->res == 9)
   {
      //QU_CRITLOG(QU_LOG_MESSAGE, "DecCfm: state=%d, count=%d", rxEnt->state, rxEnt->count);
      if ( rxEnt != NULLP )
      {
         PJ_SND_PJU_STA_IND(tPjCb,pjRbCb, rxEnt);
         pjUlmHdlErrUpdates(tPjCb,pjRbCb, rxEnt->count);
         pjDbmDelRxEnt(tPjCb,&ulCb->recBuf, rxEnt->count);
         //QU_CRITLOG(QU_LOG_MESSAGE, "DecCfm: state=%d, count=%d", rxEnt->state, rxEnt->count);
      }
      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      PJ_FREE_BUF(opPdu);
      RETVALUE(RFAILED);
   }


   /*QU_CRITLOG(QU_LOG_MESSAGE,"PjLibObdDecipherCfm called 4\n" );*/
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
         PJLOGERROR(tPjCb,ERRCLS_ADD_RES, EPJXXX, (ErrVal)libTrans->rbId,
               "PjLibObdDecipherCfm: RxEnt is NULLP \n");
         PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
         PJ_FREE_BUF(opPdu);
         PJ_STS_INC_GEN_CNT(tPjCb,numDeciphFails);
         RETVALUE(RFAILED);
      }

      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      /* 3.3 Post deciphering updations */
      rxEnt->state   =  PJ_RDY_TO_INTVER;
      PJ_FREE_BUF(rxEnt->mBuf);
      rxEnt->mBuf  =  opPdu;
      /*SPrntMsg(rxEnt->mBuf,0,0);*/

      /*QU_CRITLOG(QU_LOG_MESSAGE,"PjLibObdDecipherCfm called 6\n" );*/
      /* 3.4 Send for IntVer. This will be ASYNC  */
      ret = pjUlmHdlIntVer(tPjCb,pjRbCb, rxEnt);

      RETVALUE(ret);
   }
   else  /*  DRB */
   {
       numOutStandingCryptoJobs[pjQcomGetCryptoCoreId(libTrans)]--;

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
            if(pjUtlDeCmpReq(tPjCb,pjRbCb, libTrans->count, opPdu, &outPdu) != ROK)
            {
               PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
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

         PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
         RETVALUE(ROK);
      }


      /* 5. Post deciphering updations */
      rxEnt->state   =  PJ_RDY_TO_SUBMIT;
      PJ_FREE_BUF(rxEnt->mBuf);
      rxEnt->mBuf  =  opPdu;

      PJ_FREE(tPjCb,libTrans, sizeof(PjLibTrans));
      /* 6. Submit it for decompression */
      pjUlmSubmitForDecmp(tPjCb,pjRbCb, rxEnt);

   }/* if(pjRbCb->rbType == PJ_DRB) */

   RETVALUE(ROK);

} /* end of PjLibObdDecipherCfm */
#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**
         End of file:     eq_ul_utl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:53 2015
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

