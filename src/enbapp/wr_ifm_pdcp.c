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

     Name:     EnodeB Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_ifm_pdcp.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

/* header (.h) include files */
static const char* RLOG_MODULE_NAME="IFM";

static int RLOG_FILE_ID=23;

static int RLOG_MODULE_ID=4;

#include "wr.h"        /* Environment options */
#include "wr_emm.h"        /* Environment options */
#include "wr_cmn.h"        /* Environment options */
#include "wr_ifm_pdcp.h"
#include "wr_lwr.h"
#include "wr_dam.h"
#include "wr_smm_init.h"
#include "cpj.h"
#include "ctf.h"
#include "cpj.x"
#include "ctf.x"
#include "pju.x"
#include "wr_dam_ifm_app.h"/*ccpu00138576*/ 

#define WR_CPJ_KDF_CFG      0
#ifdef KW_PDCP


EXTERN S16 PtUiPjuBndReq ARGS(( Pst* , SuId , SpId));
EXTERN S16 PtUiPjuUbndReq ARGS(( Pst*, SpId, Reason));
EXTERN S16 PtUiPjuDatReq ARGS(( Pst*, SpId, CmLtePdcpId *, PjuSduId, Buffer*));
EXTERN S16 PtUiPjuDatFwdReq ARGS((Pst*, SpId, CmLtePdcpId *, PjuDatFwdReqInfo *));


/************************************************************************
                             PJU Interface Matrices
************************************************************************/
PRIVATE CONSTANT PjuBndReq WrIfmPdcpBndReqMt[] =
{
   cmPkPjuBndReq,          /* 0 - loosely coupled */
#ifdef PJ
   PjUiPjuBndReq           /* 1 - tightly coupled */
#else
   PtUiPjuBndReq           /* 1 - Portable Tightly Coupled */
#endif
};


PRIVATE CONSTANT PjuUbndReq WrIfmPdcpUbndReqMt[] =
{
   cmPkPjuUbndReq,         /* 0 - loosely coupled */
#ifdef PJ
   PjUiPjuUbndReq          /* 1 - tightly coupled */
#else
   PtUiPjuUbndReq           /* 1 - Portable Tightly Coupled */
#endif
};


PRIVATE CONSTANT PjuDatReq WrIfmPdcpDatReqMt[] =
{
#ifdef SS_RBUF
   cmPkFpPjuDatReq,        /* 0 - loosely coupled */
#else
   cmPkPjuDatReq,          /* 0 - loosely coupled */
#endif
#ifdef PJ
   PjUiPjuDatReq           /* 1 - tightly coupled */
#else
   PtUiPjuDatReq           /* 1 - Portable Tightly Coupled */
#endif
};

#ifdef FLAT_BUFFER_OPT
PRIVATE CONSTANT PjuDatReqFB WrIfmPdcpDatReqFBMt[] =
{
   cmPkPjuDatReqFB,          /* 0 - loosely coupled */
#ifndef WR_ACC
   PjUiPjuDatReqFB           /* 1 - tightly coupled */
#endif
};
#endif


PRIVATE CONSTANT PjuDatFwdReq WrIfmPdcpDatFwdReqMt[] =
{
   cmPkPjuDatFwdReq,       /* 0 - loosely coupled */
#ifdef PJ
   PjUiPjuDatFwdReq        /* 1 - tightly coupled */
#else
   PtUiPjuDatFwdReq           /* 1 - Portable Tightly Coupled */
#endif
};
#ifdef TENB_AS_SECURITY 

/** UE ID change Request from RRC to PHY. */
PRIVATE CONSTANT CpjKdfReq WrIfmPdcpKdfReqMt[] =
{
   cmPkCpjKdfReq,      /* 0 - loosely coupled */
   NULLP,              /* 1 - - tightly coupled, INVALID */
   NULLP                   /* 2 - tightly coupled, INVALID */
};

#ifndef LTE_PAL_ENB
/** @brief This function is responsible for Updating the transaction id.
 *
 * @details
 *
 * Function:wrIfmPdcpSetTrans
 *
 *   Processing steps:
 *   - Update trans id.
 *
 * @param[in]  transID: transaction Identifier
 * @param[in]  transId: transaction Identifier
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */

PRIVATE Void  wrIfmPdcpSetTrans
(
CpjCfgTransId                *transID,
U32                          transId
)
{
   transID->trans[7] = transId & 0x000000ff; transId >>= 8;
   transID->trans[6] = transId & 0x000000ff; transId >>= 8;
   transID->trans[5] = transId & 0x000000ff; transId >>= 8;
   transID->trans[4] = transId & 0x000000ff; transId >>= 8;
}

#endif


/** @brief This function is responsible for Sending KDF request 
 *
 * @details
 *
 * Function:wrIfmPdcpKdfReq
 *
 *
 *  @param[in] kdfReqInf: Parameters for key generation 
 *  @param[in]  transId: transaction Id
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrIfmPdcpKdfReq
(
CpjKdfReqInfo                *kdfReqInf, 
U32                          transId
)
{
   Pst                       pst;
   CpjCfgTransId             transID;

   TRC2(wrIfmPdcpKdfReq);

   RLOG0(L_DEBUG,"Triggering KDF Req over the CPJ Interface");

   /* Copying the pst structure */
   cmMemset((U8 *)&transID, '\0', sizeof(CpjCfgTransId));
   transID.trans[0] = WR_CPJ_KDF_CFG;
   /**/
   pst.event       = EVTCPJKDFREQ;
   pst.dstEnt      = ENTPJ;
   pst.dstInst     = 0;
   pst.dstProcId   = SM_PJ_PROC;
   pst.srcEnt      = ENTWR;
   pst.srcInst     = 0;
   pst.srcProcId   = SM_WR_PROC;
   pst.region      = WR_MEM_REGION;
   pst.pool        = WR_MEM_POOL;
   pst.route       = RTESPEC;
   pst.selector    = 0;
   pst.prior       = PRIOR0;
   wrIfmPdcpSetTrans(&transID, transId);

//   RETVALUE((*WrIfmL1KdfReqMt[pst->selector])(pst, wrCb.ctfSap[0]->spId, transID, kdfReqInf));
   /* For Key derivation, we are tight coupling App with CL call. Ket derivation is done in the App thread
      context to avoid TTI stretch issue seen during re-establishment */
   RETVALUE((*WrIfmPdcpKdfReqMt[0])(&pst, wrCb.ctfSap[0]->spId, transID, kdfReqInf));

}/*wrIfmPhyUeCfg*/


#ifdef TENB_AS_SECURITY
/** @brief This function is responsible for Sending Ue Id change confirm
 * message
 * 
 * @details
 *
 * Function:wrCpyCpjToCtf 
 *
 *   Processing steps: Copies CPJ paramters to KDF
 *
 *  @param[in]  kdfCfmInf: Pointer to CPJ KDF info 
 *  @param[in]  ctfKdfCfmInf: Pointer to CTF KDF info 
 *  @return S16
 *       SUCCESS : ROK
 */
PUBLIC Void wrCpyCpjToCtf 
(
 CpjKdfCfmInfo                *kdfCfmInf, 
 CtfKdfCfmInfo                *ctfKdfCfmInf
)
{
   ctfKdfCfmInf->keyDerType = kdfCfmInf->keyDerType;

   switch(ctfKdfCfmInf->keyDerType)
   {
      case CPJ_KDF_TYPE1:
         cmMemcpy(&(ctfKdfCfmInf->u.kEnbInf.intKey[0]),
            &(kdfCfmInf->u.kEnbInf.intKey[0]), (sizeof(U8) * CTF_SEC_KEY_LEN));
         cmMemcpy(&(ctfKdfCfmInf->u.kEnbInf.upCiphKey[0]),
            &(kdfCfmInf->u.kEnbInf.upCiphKey[0]), (sizeof(U8) * CTF_SEC_KEY_LEN));
         cmMemcpy(&(ctfKdfCfmInf->u.kEnbInf.cpCiphKey[0]),
            &(kdfCfmInf->u.kEnbInf.cpCiphKey[0]), (sizeof(U8) * CTF_SEC_KEY_LEN));
         break;

      case CPJ_KDF_TYPE2:
         cmMemcpy(&(ctfKdfCfmInf->u.kEnbStarInf.secKey[0]),
            &(kdfCfmInf->u.kEnbStarInf.secKey[0]), (sizeof(U8) * CTF_SEC_KEY_LEN));
         break;

      case CPJ_KDF_TYPE3:
      RLOG0(L_ERROR, "CPJ_KDF_TYPE3 not used");
         break;

      default:
      RLOG0(L_ERROR,"Invalid Key Type");
         break;
   }
   RETVOID;
}
#endif
/** @brief This function is responsible for Sending Ue Id change confirm
 * message
 * 
 * @details
 *
 * Function: WrIfmPdcpKdfCfm
 *
 *   Processing steps:
 *    - Process ue id change message.
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suId: service user Id
 *  @param[in]  transId: transaction identifier
 *  @param[in]  kdfCfmInf: Pointer to KDF info 
 *  @param[in]  status: Status of KDF Cfg 
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 WrIfmPdcpKdfCfm
(
Pst                          *pst,            
SuId                         suId,
CpjCfgTransId                transId,
CpjKdfCfmInfo                *kdfCfmInf, 
U8 status
)
{
   U32                       lTransId;
   CtfKdfCfmInfo             ctfKdfCfmInf; 

   TRC3(WrIfmPdcpKdfCfm);
   
   lTransId = transId.trans[4];
   lTransId = (lTransId << 8) | transId.trans[5];
   lTransId = (lTransId << 8) | transId.trans[6];
   lTransId = (lTransId << 8) | transId.trans[7];

   RLOG_ARG0(L_DEBUG,DBG_TRNSID,(U32)lTransId,"CPJ Cfg Cfm");

   if(status != CTF_CFG_CFM_OK)
   {
      RLOG1(L_ERROR,"Status failed:[%d]",wrEmmCb.cellCb[0]->sib1.cellIdentity);
      RETVALUE(RFAILED);
   }
   /*KDF paramters on CTF and CPJ are similar, application will process it as
    * CTF*/
   wrCpyCpjToCtf(kdfCfmInf, &ctfKdfCfmInf);
   if(status != CTF_CFG_CFM_OK)
   {
      wrUmmPrcKdfCfm(lTransId, WR_CFG_NOK, &ctfKdfCfmInf);
   }
   else
   {
      wrUmmPrcKdfCfm(lTransId, WR_CFG_OK, &ctfKdfCfmInf);
   }

   RETVALUE(ROK);
}  /* end of WrIfmL1UeIdChgCfm */
#endif /* TENB_AS_SECURITY */

/****************************************************************************
 *                         PJU Interface Mt functions
 ***************************************************************************/
#ifdef WR
/** @brief This function is responsible for Sending Bind request.
 *
 * @details
 *
 * Function:WrIfmPdcpBndReq
 *
 *   Processing steps:
 *    - based on selector present in Post structure return appropriate function
 *       pointer.
 *
 *  @param[in]  pst: Pointer to the Post structure
 *  @param[in]  suId: Service user Id
 *  @param[in]  spId: Service Provider Id
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 WrIfmPdcpBndReq
(
Pst*                         pst,
SuId                         suId,
SpId                         spId
)
{
   (*WrIfmPdcpBndReqMt[pst->selector])(pst, suId, spId);

   RETVALUE(ROK);
}

/** @brief This function is responsible for Sending Unbind request.
 *
 * @details
 *
 * Function:WrIfmPdcpUbndReq
 *
 *   Processing steps:
 *    - based on selector present in Post structure return appropriate function
 *       pointer.
 *
 *  @param[in]  pst: Pointer to the Post structure
 *  @param[in]  spId: Service Provider Id
 *  @param[in]  reason: Reason
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 WrIfmPdcpUbndReq
(
Pst*                         pst,
SpId                         spId,
Reason                       reason
)
{
   (*WrIfmPdcpUbndReqMt[pst->selector])(pst, spId, reason);

   RETVALUE(ROK);
}

/** @brief This function is responsible for Sending Pdcp Data request.
 *
 * @details
 *
 * Function:WrIfmPdcpDatReq
 *
 *   Processing steps:
 *    - based on selector present in Post structure return appropriate function
 *       pointer.
 *
 *  @param[in]  pdcpId: Pointer to the CmLtePdcpId structure
 *  @param[in]  sduId: PjuSduId
 *  @param[in]  mBuf: Pointer to buffer
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 WrIfmPdcpDatReq
(
CmLtePdcpId                  *pdcpId,
PjuSduId                     sduId,
U8                           priority,
Buffer                       *mBuf
)
{
   Pst                       *pst = &wrDamCb.pjuSap[0]->pst;
   SpId                      spId =  wrDamCb.pjuSap[0]->spId;

   pst->prior = priority;

   (*WrIfmPdcpDatReqMt[pst->selector])(pst, spId, pdcpId, sduId, mBuf);

   RETVALUE(ROK);
}

#ifdef FLAT_BUFFER_OPT
/** @brief This function is responsible for Sending Pdcp Data request.
 *
 * @details
 *
 * Function:WrIfmPdcpDatReqFB
 *
 *   Processing steps:
 *    - based on selector present in Post structure return appropriate function
 *       pointer.
 *
 *  @param[in]  pdcpId: Pointer to the CmLtePdcpId structure
 *  @param[in]  sduId: PjuSduId
 *  @param[in]  mBuf: Pointer to Flat buffer
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */

PUBLIC S16 WrIfmPdcpDatReqFB
(
CmLtePdcpId                  *pdcpId,
PjuSduId                     sduId,
U8                           priority,
FlatBuffer                   *mBuf
)
{
   Pst                       *pst = &wrDamCb.pjuSap[0]->pst;
   SpId                      spId =  wrDamCb.pjuSap[0]->spId;

   pst->prior = priority;

   (*WrIfmPdcpDatReqFBMt[pst->selector])(pst, spId, pdcpId, sduId, mBuf);

   RETVALUE(ROK);
}
#endif

/** @brief This function is responsible for Sending Data forward request.
 *
 * @details
 *
 * Function:WrIfmPdcpDatFwdReq
 *
 *   Processing steps:
 *    - based on selector present in Post structure return appropriate function
 *       pointer.
 *
 *  @param[in]  pdcpId: Pointer to the CmLtePdcpId structure
 *  @param[in]  datFwdReq: Pointer to PjuDatFwdReqInfo structure
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 WrIfmPdcpDatFwdReq
(
CmLtePdcpId                  *pdcpId,
PjuDatFwdReqInfo             *datFwdReq
)
{
   Pst      *pst;
   SpId      spId;
   if(datFwdReq->dir == CM_LTE_DIR_DL)
   {
      pst = &wrDamCb.pjuSap[0]->pst;
      spId =  wrDamCb.pjuSap[0]->spId;
   }
   else
   {
      pst = &wrDamCb.pjuSap[1]->pst;
      spId =  wrDamCb.pjuSap[1]->spId;
   }

   (*WrIfmPdcpDatFwdReqMt[pst->selector])(pst, spId, pdcpId, datFwdReq);

   RETVALUE(ROK);
}
#endif /*--ifdef NX--*/

  
/*************************************************************
    PJU interface handler functions
**************************************************************/
/** @brief This function is responsible for Sending PJU Bind confirm.
 *
 * @details
 *
 * Function:WrIfmPdcpBndCfm
 *
 *   Processing steps:
 *    - check the status 
 *    - process the alarm
 *
 *  @param[in]  pst: Pointer to the Post structure
 *  @param[in]  suId: SuId structure
 *  @param[in]  status: Status
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 WrIfmPdcpBndCfm
(
Pst                          *pst,
SuId                         suId,
U8                           status
)
{
   TRC2(WrIfmPdcpBndCfm);

   RLOG0(L_DEBUG, "WrIfmPdcpBndCfm: PJU Bind Cfm.");

   /* Send alarm */
   if (CM_BND_OK == status)
   {
      RLOG0(L_INFO, "[ALARM] Sending PJU_SAP_BND_OK to OAM");
      wrIfmDamWrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, 
                                                LWR_CAUSE_PJU_SAP_BOUND);
   }
   else
   {
      RLOG0(L_FATAL, "[ALARM] Sending PJU_SAP_BND_FAIL to OAM");
      wrIfmDamWrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                                                     LCM_CAUSE_UNKNOWN);
   }

   wrDamStopTmr((PTR)wrDamCb.pjuSap[suId],WR_TMR_PJU_SAP_BND);

   RETVALUE(ROK);
} /* WrIfmPdcpBndCfm */

/** @brief This function is responsible for Sending PJU Data confirm.
 *
 * @details
 *
 * Function:WrIfmPdcpDatCfm
 *
 *   Processing steps:
 *    - process the message
 *
 *  @param[in]  pst: Pointer to the Post structure
 *  @param[in]  suId: SuId structure
 *  @param[in]  pdcpId: pointer to CmLtePdcpId structure
 *  @param[in]  datCfm: Pointer to PjuDatCfmInfo structure
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 WrIfmPdcpDatCfm
(
Pst                          *pst,
SuId                         suId,
CmLtePdcpId                  *pdcpId,
PjuDatCfmInfo                *datCfm
)
{
/* RLC_DL_MAX_RETX fix */

   TRC2(WrIfmPdcpDatCfm);
   RLOG2(L_DEBUG, "PJU Data Cfm UE[%d] rbId [%d]", pdcpId->ueId, pdcpId->rbId);
   /* As of now it is assumed that DatCfm for DRBs comes only
    * for RLC DL Max Retx exceeding case and handled in the same 
    * way of Inactivity Timer Expiry case*/
   if(wrDamPdcpDatCfm(*pdcpId) != ROK)
   {
      RETVALUE(RFAILED);
   }
   /* DatCfm for other cases are Ignored */
   
   RETVALUE(ROK);
} /* WrIfmPdcpDatCfm */

/** @brief This function is responsible for Sending PJU Status indication
 *
 * @details
 *
 * Function:WrIfmPdcpStaInd
 *
 *   Processing steps:
 *    - process Status indication message
 *
 *  @param[in]  pst: Pointer to the Post structure
 *  @param[in]  suId: SuId structure
 *  @param[in]  pdcpId: pointer to CmLtePdcpId structure
 *  @param[in]  staInd: Pointer to PjuStaIndInfo structure
 *  @param[in]  mBuf: Pointer to Buffer
 *  @return S16
 *       SUCCESS : ROK
*/
PRIVATE S16 WrIfmPdcpStaInd
(
Pst                          *pst,
SuId                         suId,
CmLtePdcpId                  *pdcpId,
PjuStaIndInfo                *staInd,
Buffer                       *mBuf
)
{
   TRC2(WrIfmPdcpStaInd);
   RLOG0(L_DEBUG, "WrIfmPdcpStaInd: PJU Status Ind");
   if (mBuf != NULL)
   {
      WR_FREEMBUF(mBuf);
      mBuf = NULL;
   }
   /* Free SatInd  */
   SPutSBuf(pst->region, pst->pool, (Data *) staInd, sizeof(PjuStaIndInfo));
   
   RETVALUE(ROK);
} /* WrIfmPdcpStaInd */

/*************************************************************
    PJU interface wrapper functions
**************************************************************/
/** @brief This function is responsible for Sending PJU Bind confirm
 *
 * @details
 *
 * Function:PxLiPjuBndCfm
 *
 *   Processing steps:
 *    - process Bind Confirm message
 *
 *  @param[in]  pst: Pointer to the Post structure
 *  @param[in]  suId: SuId structure
 *  @param[in]  status: status
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 PxLiPjuBndCfm
(
Pst                          *pst,
SuId                         suId,
U8                           status
)
{
   TRC2(PxLiPjuBndCfm);
   RETVALUE(WrIfmPdcpBndCfm(pst,suId,status));
} /* PxLiPjuBndCfm */

/** @brief This function is responsible for Sending PJU Bind Indication
 *
 * @details
 *
 * Function:PxLiPjuDatInd
 *
 *   Processing steps:
 *    - process Bind Indication message
 *
 *  @param[in]  pst, Pointer to the Post structure
 *  @param[in]  suId, SuId structure
 *  @param[in]  pdcpId, Pointer to CmLtePdcpId structure
 *  @param[in]  mBuf, Pointer to buffer
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 PxLiPjuDatInd
(
Pst                          *pst,
SuId                         suId,
CmLtePdcpId                  *pdcpId,
Buffer                       *mBuf
)
{
   TRC2(PxLiPjuDatInd);
   wrDamPdcpDatInd(pdcpId, mBuf);
   RETVALUE(ROK);
} /* PxLiPjuDatInd */

/** @brief This function is responsible for Sending PJU Data Confirm.
 *
 * @details
 *
 * Function:PxLiPjuDatCfm
 *
 *   Processing steps:
 *    - process Data Confirm message
 *
 *  @param[in]  pst, Pointer to the Post structure
 *  @param[in]  suId, SuId structure
 *  @param[in]  pdcpId, pointer to CmLtePdcpId structure
 *  @param[in]  datCfm, Pointer to PjuDatCfmInfo structure
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 PxLiPjuDatCfm
(
Pst                          *pst,
SuId                         suId,
CmLtePdcpId                  *pdcpId,
PjuDatCfmInfo                *datCfm
)
{
   TRC2(PxLiPjuDatCfm);
   RETVALUE(WrIfmPdcpDatCfm(pst, suId, pdcpId, datCfm));
} /* PxLiPjuDatCfm */

/** @brief This function is responsible for Sending PJU Status indication.
 *
 * @details
 *
 * Function:PxLiPjuStaInd
 *
 *   Processing steps:
 *    - process Status Indication message
 *
 *  @param[in]  pst, Pointer to the Post structure
 *  @param[in]  suId, SuId structure
 *  @param[in]  pdcpId, pointer to CmLtePdcpId structure
 *  @param[in]  staInd, Pointer to PjuStaIndInfo structure
 *  @param[in]  mBuf, Pointer to Buffer
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 PxLiPjuStaInd
(
Pst                          *pst,
SuId                         suId,
CmLtePdcpId                  *pdcpId,
PjuStaIndInfo                *staInd,
Buffer                       *mBuf
)
{
   PjuStaIndInfo *staIndMsg = NULLP;
   TRC2(PxLiPjuStaInd);

   /* Allocting mem for staInd
    * In TC case, pdcp passes stack variable
    */
   if((ROK != SGetSBuf(pst->region, pst->pool, (Data **)&staIndMsg,\
                  sizeof(PjuStaIndInfo))))
   {
      if (mBuf != NULL)
      {
         WR_FREEMBUF(mBuf);
         mBuf = NULL;
      }
      RETVALUE(RFAILED);
   }
   *staIndMsg = *staInd;
   RETVALUE(WrIfmPdcpStaInd(pst, suId, pdcpId, staIndMsg, mBuf));
} /* PxLiPjuStaInd */

/** @brief This function is responsible for Sending PJU Data forward indication.
 *
 * @details
 *
 * Function:PxLiPjuDatFwdInd
 *
 *   Processing steps:
 *    - process the message
 *
 *  @param[in]  pst, Pointer to the Post structure
 *  @param[in]  suId, SuId structure
 *  @param[in]  pdcpId, pointer to CmLtePdcpId structure
 *  @param[in]  datFwdInd, Pointer to PjuDatFwdIndInfo structure
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 PxLiPjuDatFwdInd
(
Pst                          *pst,
SuId                         suId,
CmLtePdcpId                  *pdcpId,
PjuDatFwdIndInfo             *datFwdInd
)
{
   TRC2(PxLiPjuDatFwdInd);
   wrDamPdcpDatFwdInd(pst, pdcpId, datFwdInd);
   /* fix for ccpu00129521:pdcpId is an object,freeing not required*/
   RETVALUE(ROK);
} /* PxLiPjuDatFwdInd */

#ifndef PJ
/**
 *
 * @brief 
 *
 *        Portable Handler for binding the PDCP upper layer service user with 
 *        the PDCP layer.
 *
 * @b Description:
 *
 *        1. This function is used by PDCP user to request for binding to 
 *        PDCP. @n
 *        2. This function is called by the PJU interface to bind 
 *        PDCP's SAP (identified by spId) with the service user's
 *        SAP (identified by suId). 
 *
 *  @param[in] pst   Post structure  
 *  @param[in] suId  Service user SAP ID 
 *  @param[in] spId  Service provider ID
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
PUBLIC S16 PtUiPjuBndReq 
(
Pst* pst,
SuId suId,
SpId spId
)
{
   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Portable Handler for unbinding the PDCP upper layer service user with 
 *        the PDCP layer.
 *
 * @b Description:
 *
 *        1. This function is used by PDCP user to request for unbinding  
 *        with PDCP. @n
 *        2. This function is called by the PJU interface to 
 *        unbind with PDCP. 
 *
 *  @param[in] pst     Post structure  
 *  @param[in] suId    Service user SAP ID 
 *  @param[in] reason  Reason for Unbinding 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
PUBLIC S16 PtUiPjuUbndReq
(
Pst      *pst,   
SpId     spId,    
Reason   reason     
)
{
   RETVALUE(ROK);
}

/**
 * @details
 * This portable primitive is used by the PDCP service user to request PDCP
 * for data transfer.
 *
 * @param[in] pst    - Pointer to Pst structure.
 * @param[in] spId  - Service provider ID. This is the reference number
 * used by PDCP service user (Relay) to identify its SAP. This reference number
 * will be provided by PDCP in any further primitives sent by PDCP
 * to the service user.
 * @param[in] pdcpId - Pointer to CmLtePdcpId structure, PDCP id is used to 
 * uniquely identify an PDCP entity. 
 * @param[in] sduId  - SDU ID. 
 * @param[in] mBuf   - Pointer to data buffer.
 *
 * @return ROK
 */
PUBLIC S16 PtUiPjuDatReq
(
Pst            *pst,            
SpId           spId,               
CmLtePdcpId    *pdcpId,         
PjuSduId       sduId,           
Buffer         *mBuf          
)
{
   RETVALUE(ROK);
}
/**
 * 
 * @details
 * This portable primitive is used by PDCP user to forward the SDUs along with its SN
 * as part of handover (SDUs forwarded from PDCP on source eNB via X2-U 
 * Interface to the PDCP on target eNB)
 *
 * @param[in] pst    - Pointer to Pst structure.
 * @param[in] spId   - Service provider ID. This is the reference number
 * used by PDCP service user (Relay) to identify its SAP. This reference number
 * will be provided by PDCP in any further primitives sent by PDCP
 * to the service user.
 * @param[in] pdcpId - Pointer to CmLtePdcpId structure, PDCP id is used to 
 * uniquely identify a PDCP entity. 
 * @param[in] datFwdReq - Pointer to data forward information structure.
 *
 *  @return ROK.
 */
PUBLIC S16 PtUiPjuDatFwdReq 
(
Pst              *pst, 
SpId             spId, 
CmLtePdcpId      *pdcpId, 
PjuDatFwdReqInfo *datFwdReq
)
{
   RETVALUE(ROK);
}
#endif /* PJ */
#endif /* KW_PDCP */



/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
