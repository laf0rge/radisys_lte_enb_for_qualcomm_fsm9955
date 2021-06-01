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
  
     Desc:     Source code for RLC Unacknowledged mode assembly and
               reassembly.This file contains following functions

                  --kwUmmQSdu
                  --kwUmmProcessSdus
                  --kwUmmProcessPdus
                  --kwUmmReAssembleSdus
                  --kwUmmReEstablish 

     File:     gp_umm_dl.c

     Sid:      gp_umm_dl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:03 2015

     Prg:      vp 

**********************************************************************/
static const char* RLOG_MODULE_NAME="RLC";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=239;
/** 
 * @file gp_umm_dl.c
 * @brief RLC Unacknowledged Mode downlink module
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
#include "envopt.h"             /* environment options */
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

#include "gen.h"                /* general */
#include "ssi.h"                /* system services interface */
#include "cm5.h"                /* Timer Functions */
#include "cm_lte.h"             /* common umts header file */
#include "cm_hash.h"            /* common hash module  file */
#include "cm_llist.h"           /* common list header file */
#include "ckw.h"                /* RRC layer */
#include "lkw.h"                /* RRC layer */
#include "kwu.h"                /* RLC service user */
#include "lkw.h"                /* LM Interface */
#include "rgu.h"                /* MAC layer */
#include "kw_env.h"             /* RLC environment options */

#include "kw.h"                 /* RLC layer */
#include "kw_err.h"
#include "kw_udx.h"
#include "kw_dl.h"


/* header/extern include files (.x) */

#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */
#include "cm_lib.x"             /* common library */
#include "cm5.x"                /* Timer Functions */
#include "cm_hash.x"            /* common hash module */
#include "cm_lte.x"             /* common umts file */
#include "cm_llist.x"           /* common list header file */
#include "ckw.x"                /* RRC layer */
#include "kwu.x"                /* RLC service user */
#include "lkw.x"                /* LM Interface */
#include "rgu.x"                /* MAC later */

#include "kw.x"                 /* RLC layer */
#include "kw_udx.x"
#include "kw_dl.x"

#define KW_MODULE (KW_DBGMASK_UM | KW_DBGMASK_DL)

/* variables for logging :declared in BRDCM cl */
#ifndef QCOM_STATS
#ifndef TENB_ACC
extern U32 dldrops_kwu_um;
extern U32 dlpkt_um;
extern U32 buffer_occ;
extern U32 dlrate_kwu;
#endif
#else
extern U32 kwDlrate_kwu;
#endif

PRIVATE Void kwUmmEstHdrSz ARGS ((KwUmDl *umUl));

PRIVATE Void kwUmmCreatePdu ARGS ((KwCb *gCb,
                                  KwDlRbCb *rbCb, 
                                  Buffer *pdu,
                                  U8 fi,
                                  KwPduInfo *datReqPduInfo));

/** @addtogroup ummode */
/*@{*/

/**
 * @brief  Handler to queue a SDU in the SDU queue, update BO and report 
 *         it to the lower layer.
 *       
 * @details
 *    This function is used to queue the received SDU in the 
 *    SDU queue maintained in the radio bearer control block.
 *    After queuing the SDU, BO is updated and is reported
 *    to the lower layer. 
 *
 * @param[in] gCb      RLC Instance control block
 * @param[in] rbCb     RB control block 
 * @param[in] datReq   Ptr to the datReq sent from PDCP
 * @param[in] mBuf     Sdu data
 *
 * @return  Void
*/  
#ifdef ANSI
PUBLIC Void kwUmmQSdu       
(
KwCb            *gCb,
KwDlRbCb        *rbCb, 
KwuDatReqInfo   *datReq,  
Buffer          *mBuf 
)
#else
PUBLIC Void kwUmmQSdu(gCb,rbCb,datReq,mBuf)
KwCb            *gCb;
KwDlRbCb        *rbCb;
KwuDatReqInfo   *datReq; 
Buffer          *mBuf;
#endif
{
   MsgLen   len;    /* SDU buffer length */
   KwSdu    *sdu;   /* SDU */

   TRC2(kwUmmQSdu)

   KW_UPD_L2_DL_TOT_SDU_STS(gCb,rbCb);

   KW_ALLOC_WC(gCb, sdu, (Size)sizeof(KwSdu));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( sdu == NULLP )
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      SPutMsg(mBuf);
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

/* Discard new changes starts */
   kwUtlGetCurrTime(&sdu->arrTime);
/* Discard new changes ends */
   SFndLenMsg(mBuf,&len);

   sdu->mBuf = mBuf;
   sdu->sduSz = len;
   sdu->actSz = len;
   sdu->mode.um.sduId = datReq->sduId;
   sdu->mode.um.isSegmented = FALSE;

#ifndef QCOM_STATS
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
   {
         dlrate_kwu += len;
   }
#endif   
#endif
#else
   kwDlrate_kwu += len;
#endif
   rbCb->m.umDl.bo += len;
   
   cmLListAdd2Tail(&(rbCb->m.umDl.sduQ), &sdu->lstEnt);
   sdu->lstEnt.node = (PTR)sdu;
   
   kwUmmEstHdrSz(&rbCb->m.umDl);

   if(!kwDlUtlIsReestInProgress(rbCb))
   {
      kwUtlSndDStaRsp(gCb,rbCb,rbCb->m.umDl.bo,rbCb->m.umDl.estHdrSz,FALSE);
   }
   
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_RLC
   /* Update numActUe if it is not active */
   if((rbCb->rbL2Cb.measOn & LKW_L2MEAS_ACT_UE) &&
      (rbCb->ueCb->numActRb[rbCb->qci]++ == 0))
   {
     kwCb.kwL2Cb.numActUe[rbCb->qci]++;
   }
#endif

   RETVOID;    
}


/**
 * @brief   Handler to form PDU(s) and update the BO. 
 *       
 * @details
 *     -# This function forms pdu(s) from the SDU(s) in the
 *        SDU queue and returns them.
 *     -# This function also updates the BO along with the 
 *        along with the estimated Header size.
 *
 * @param[in]  rbCb     RB control block 
 * @param[out] pduInfo  Pdu Info to be filled and the PDU size to be 
 *                      formed and the updated BO
 * @param[in]  pduSz    The size for which PDUs have to constructed
 *
 * @return  S16
 *    -# ROK       In case of success
 *    -# RFAILED   If allocation of Sdu fails
*/  
#ifdef ANSI
PUBLIC Void kwUmmProcessSdus
(
KwCb       *gCb,
KwDlRbCb   *rbCb,   
KwDatReq   *datReq   
)
#else
PUBLIC Void kwUmmProcessSdus(gCb, rbCb, datReq)
KwCb       *gCb;
KwDlRbCb   *rbCb; 
KwDatReq   *datReq;
#endif
{
   CmLList     *firstNode;   /* First Node in SDU queue */
   U8          fi=0;           /* Framing Info */
   Buffer      *pdu;         /* Buffer for holding the formed PDU */
   KwPduInfo   *pduInfo;     /* PDU Info pointer */
   S16         pduSz;        /* PDU Size to be constructed */
   
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwContSduLst         contSduLst;  /*Contained sduLst */
   S32                  dataVol    = rbCb->m.umDl.bo;
   U32*                 totMacGrant= &(datReq->totMacGrant);
   KwL2MeasDlIpTh       *dlIpThPut = &rbCb->l2MeasIpThruput.dlIpTh;
   U8                   *sduIdx    = &dlIpThPut->lastSduIdx;
   Bool                 newIdx = FALSE;
   S32                  oldBo;
   KwlchInfo            lchInfo = {0};
   U32                  segSduCnt = 0;
#endif
   Ticks                curTime  = 0;
   S16                  timeDiff = 0;
   KwSdu                *sdu;


   TRC2(kwUmmProcessSdus)


   pdu = NULLP;

   pduInfo = &(datReq->pduInfo);
   pduSz = datReq->pduSz;
   
#ifdef LTE_L2_MEAS   
   contSduLst.numSdus = 0;
   contSduLst.lcId = rbCb->lch.lChId;
   oldBo = rbCb->m.umDl.bo; 
   lchInfo.lcId = rbCb->lch.lChId;
   lchInfo.numSdus = 0;
#endif

   /* Discard new changes starts */
   kwUtlGetCurrTime(&curTime);

   /* ccpu00143043 */
   while ((pduSz > 0) && (rbCb->m.umDl.sduQ.count > 0) &&
           (rbCb->m.umDl.numLi < KW_MAX_DL_LI) && (pduInfo->numPdu <= KW_MAX_PDU))
   {
      CM_LLIST_FIRST_NODE(&rbCb->m.umDl.sduQ,firstNode);
      sdu = (KwSdu *)(firstNode->node);

      if ((sdu->mode.um.isSegmented == FALSE) && (rbCb->discTmrInt > 0) && 
            (rbCb->rlcId.rbType == CM_LTE_DRB))
      {
         timeDiff = KW_TIME_DIFF(curTime,sdu->arrTime); 

         if (timeDiff >= rbCb->discTmrInt)
         {
#ifdef LTE_L2_MEAS 
            KW_UPD_L2_DL_DISC_SDU_STS(gCb, rbCb);
#endif
            rbCb->m.umDl.bo -= sdu->sduSz;
            KW_RMV_SDU(gCb,&rbCb->m.umDl.sduQ,sdu);
            continue;
         }
      }
      /* When forming a new PDU, pdu == NULLP
           -# Eliminate MAC header size for each pdu 
           -# Initialize the li array to 0 
           -# Substract the fixed header length based on SN length
      */
#ifdef LTE_L2_MEAS
      newIdx = FALSE;
#endif
      if (!pdu)
      {
         KW_RMV_MAC_HDR_SZ(pduSz);

         /* account for the RLC header size */
         pduSz -= rbCb->m.umDl.snLen;

         /* kw005.201 fixing pduSz <= 0 problem, ccpu00119417 */
         if(pduSz <= 0)
         {
            break;
         }         
         
         rbCb->m.umDl.numLi = 0;
         if (sdu->mode.um.isSegmented == TRUE)
         {
            fi = 2;
         }
         else
         {
            fi = 0;
         }
      }

      kwUtlCalcLiForSdu(gCb,rbCb->m.umDl.numLi,sdu->sduSz,&pduSz);
     
      /* Exact fit scenario :
         If the SDU size matches with the PDU size
           -# Allocate memory equal to PDU size;
           -# update BO
           -# Remove SDu from queue
           -# Append to already existing PDU portion if present .
           -# Add Header and create complete PDU and place it in
              pduInfo and return
      */ 
      if (sdu->sduSz == pduSz)
      {
         if (!pdu)
         {
            pdu = sdu->mBuf;
            sdu->mBuf = NULLP;
         }
         else
         {
            SCatMsg(pdu, sdu->mBuf, M1M2);    
         }
         
         rbCb->m.umDl.bo -= pduSz;
         pduSz = 0;

#ifdef LTE_L2_MEAS
        if(KW_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(gCb,rbCb))
        {
           if(sdu->mode.um.isSegmented)
           {
              *sduIdx    = dlIpThPut->lastSduIdx;
           }
           else
           {
              KW_GETSDUIDX(*sduIdx);
              newIdx = TRUE;
           }
           kwUtlUpdateContainedSduLst(*sduIdx, &contSduLst);
           kwUtlUpdateOutStandingSduLst(dlIpThPut, *sduIdx, sdu->actSz, 
                 sdu->mode.um.sduId, newIdx);
           /* ccpu00143043 */
           if ( lchInfo.numSdus < KW_L2MEAS_SDUIDX)
           {
              lchInfo.sduInfo[lchInfo.numSdus].arvlTime = sdu->arrTime; 
              lchInfo.sduInfo[lchInfo.numSdus].isRetxPdu = FALSE;
              lchInfo.numSdus++;
           }
        }
#endif
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_RLC
         kwUtlUpdSduSnMap(rbCb, sdu, datReq, TRUE);
         if((rbCb->rbL2Cb.measOn & LKW_L2MEAS_DL_DELAY) ||
                 (rbCb->rbL2Cb.measOn & LKW_L2MEAS_UU_LOSS))
          {
             /* ccpu00143043 */
             if ( lchInfo.numSdus < KW_L2MEAS_SDUIDX)
             {
                lchInfo.arvlTime[lchInfo.numSdus] = sdu->arrTime; 
                lchInfo.numSdus++;
             }
          }
#endif /*  LTE_L2_MEAS */
         KW_RMV_SDU(gCb,&(rbCb->m.umDl.sduQ),sdu); /* kw003.201 */
         kwUtlIncrementKwuStsSduTx(gCb->u.dlCb->kwuDlSap + rbCb->kwuSapId);

         kwUmmCreatePdu(gCb,rbCb,pdu,fi,pduInfo);
         pdu = NULLP;
         
      }
      /* Concatenation scenario :
         If SDU size is less than the requested PDU size
           -# Allocate memory and copy SDU into it.
           -# Update BO
           -# Remove SDU from the Queue.
           -# Append to already existing PDU portion if present .
           -# If the SDU size is greater than 2047 or the number of i
                 LIs reaches max, place it as a separate PDU in pduInfo and 
                 set pdu to NULL
              else 
                 place the msglen in li array and continue with the next SDU.
           -# If the number of PDUs is more than KW_MAX_PDU, return from 
              the function even if pduSize > 0.
      */
      else if (sdu->sduSz < pduSz)
      {
         if (!pdu)
         {
            pdu = sdu->mBuf;
            sdu->mBuf = NULLP;
         }
         else
         {
            SCatMsg(pdu, sdu->mBuf ,M1M2);
         }
         rbCb->m.umDl.bo -= sdu->sduSz;

         pduSz -= sdu->sduSz;
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_RLC
          kwUtlUpdSduSnMap(rbCb, sdu, datReq, TRUE);
#endif /*  LTE_L2_MEAS */
         if (sdu->sduSz < 2048 && rbCb->m.umDl.numLi < KW_MAX_DL_LI)
         {
            rbCb->m.umDl.li[(rbCb->m.umDl.numLi)++] = sdu->sduSz;
         }
         else 
         {
            kwUmmCreatePdu(gCb, rbCb, pdu, fi, pduInfo);
            pdu = NULLP;

            if ( pduInfo->numPdu == KW_MAX_PDU)
            {
                /* Could not transmit what MAC asked for because the number 
                 * of PDUs to be transmitted has reached maximum. */
               RLOG_ARG2(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                        "Maximum Pdu limit has been reached UEID:%d CELLID:%d",
                        rbCb->rlcId.ueId,
                        rbCb->rlcId.cellId);
               break;
            }
         }
#ifdef LTE_L2_MEAS
        if(KW_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(gCb,rbCb) )
        {
           if(sdu->mode.um.isSegmented)
           {
              *sduIdx    = dlIpThPut->lastSduIdx;
           }
           else
           {
              KW_GETSDUIDX(*sduIdx);
              newIdx = TRUE;
           }
           kwUtlUpdateContainedSduLst(*sduIdx, &contSduLst);
           kwUtlUpdateOutStandingSduLst(dlIpThPut, *sduIdx, sdu->actSz, 
                 sdu->mode.um.sduId, newIdx);
           /* ccpu00143043 */
           if ( lchInfo.numSdus < KW_L2MEAS_SDUIDX)
           {
              lchInfo.sduInfo[lchInfo.numSdus].arvlTime = sdu->arrTime; 
              lchInfo.sduInfo[lchInfo.numSdus].isRetxPdu = FALSE;
              lchInfo.numSdus++;
           }
        }
#endif
         KW_RMV_SDU(gCb,&(rbCb->m.umDl.sduQ),sdu);
         /* kw005.201 ccpu00117318, updating the statistics */
         kwUtlIncrementKwuStsSduTx(gCb->u.dlCb->kwuDlSap + rbCb->kwuSapId);         
      }
      /* Segmentation scenario :
         If size of SDU is greater than PDU size 
           -# Allocate memory and Segment the Sdu.
           -# Update BO
           -# Append to already existing PDU if any.
           -# Set the second bit of the framing info.
           -# Create the complete PDU and place in pduInfo.
      */ 
      else 
      {
         Buffer *remSdu;
       
         SSegMsg(sdu->mBuf,pduSz,&remSdu);
        
#ifdef LTE_L2_MEAS
        if(KW_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(gCb, rbCb))
        {
           if(sdu->mode.um.isSegmented)
           {
              *sduIdx    = dlIpThPut->lastSduIdx;
           }
           else
           {
              KW_GETSDUIDX(*sduIdx);
              newIdx = TRUE;
           }
           kwUtlUpdateContainedSduLst(*sduIdx, &contSduLst);
           kwUtlUpdateOutStandingSduLst(dlIpThPut, *sduIdx, sdu->actSz, 
                 sdu->mode.um.sduId, newIdx);
        }
        if(KW_MEAS_IS_DL_UU_LOSS_MEAS_ON_FOR_RB(gCb,rbCb))
        {
           if(sdu->actSz == sdu->sduSz)
           {
              segSduCnt++;
           }
        }
#endif
         if (!pdu)
         {
            pdu = sdu->mBuf;
         }
         else 
         {
            SCatMsg(pdu, sdu->mBuf, M1M2);
            KW_FREE_BUF_WC(sdu->mBuf);
         }

         sdu->sduSz -= pduSz;
         rbCb->m.umDl.bo -= pduSz;
         sdu->mode.um.isSegmented = TRUE;
         sdu->mBuf = remSdu;
         pduSz = 0;
         
         fi |= 1;
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS_RLC
         kwUtlUpdSduSnMap(rbCb, sdu, datReq, FALSE);
#endif /*  LTE_L2_MEAS */

         kwUmmCreatePdu(gCb,rbCb,pdu,fi,pduInfo);
         pdu = NULLP;
      }
/* kw005.201 added support for L2 Measurement */
   }
#ifdef LTE_L2_MEAS_RLC
   if((rbCb->rbL2Cb.measOn) && 
      (rbCb->m.umDl.sduQ.count == 0) && 
      (dataWasPrsnt))
   {
      if(--(rbCb->ueCb->numActRb[rbCb->qci]) == 0)
      {
         kwCb.kwL2Cb.numActUe[rbCb->qci]--;
      }
   }
#endif /* LTE_L2_MEAS */
#ifdef LTE_L2_MEAS
   kwUtlUpdateBurstSdus(gCb, rbCb, &contSduLst, dataVol, *totMacGrant);
   /* Need to check into optimizing this code : TODO */
   if(KW_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(gCb,rbCb) && (lchInfo.numSdus != 0))
   {
      KwL2MeasTb *l2MeasTb = kwUtlGetCurMeasTb(gCb, rbCb);
      /* ccpu00143043 */
      /* Fix Klock warning */
      if ((lchInfo.numSdus != 0) && (l2MeasTb != NULLP) &&
          (l2MeasTb->numLchInfo < KW_MAX_ACTV_DRB))
      {   
         cmMemcpy((U8 *) &l2MeasTb->lchInfo[l2MeasTb->numLchInfo], (U8 *) &lchInfo, sizeof(KwlchInfo));
         l2MeasTb->numLchInfo++;
      }
      l2MeasTb->txSegSduCnt += segSduCnt;
   }
   *totMacGrant -= (oldBo - rbCb->m.umDl.bo);
#endif 

   /* If we have a situation wherein the size requested is greater than the total size of SDUs
      and a pdu buffer which is not null, this if loop helps to send 
      a non null PDU to the lower layer. 
   */
   if (pduSz > 0 && pdu)
   {
      if (pduInfo->numPdu != KW_MAX_PDU)
      {
         rbCb->m.umDl.numLi--;         
         kwUmmCreatePdu(gCb,rbCb,pdu,fi,pduInfo);   
         pdu = NULLP;
      }
      else
      {
         KW_FREE_BUF_WC(pdu);
      }
   }
   
   kwUmmEstHdrSz(&rbCb->m.umDl);
   datReq->boRep.bo = rbCb->m.umDl.bo;
   datReq->boRep.estHdrSz = rbCb->m.umDl.estHdrSz;
   datReq->boRep.staPduPrsnt = FALSE;
   if (rbCb->m.umDl.sduQ.count > 0)
   {
      datReq->boRep.oldestSduArrTime = 
        ((KwSdu *)(rbCb->m.umDl.sduQ.first->node))->arrTime;
   }
   RETVOID; 
}

/**
 * @brief   Handler to process the re-establishment request received from i
 *          the upper layer. 
 *       
 * @details
 *     This function does the following functions : 
 *         Remove all the SDUs in the SDU queue.
 *
 * @param[in] gCb        RLC Instance control block
 * @param[in] rlcID      Identity of the RLC entity for which 
 *                       re-establishment is to be done
 * @param[in] sendReEst  Whether to send re-establishment complete 
 *                       indication to  upper layer or not
 * @param[in] rbCb       RB control block for which re-establishment 
 *                       is to be done
 *
 * @return  Void
*/ 
#ifdef ANSI
PUBLIC Void kwDlUmmReEstablish
(
KwCb         *gCb,
CmLteRlcId   rlcId,
Bool         sendReEst,
KwDlRbCb     *rbCb
)
#else
PUBLIC Void kwDlUmmReEstablish(gCb, rlcId, rbCb)
KwCb         *gCb;
CmLteRlcId   rlcId;
Bool         sendReEst;
KwDlRbCb       *rbCb;
#endif
{
   /* The re-establishment indication is sent from the UL only */
   TRC2(kwDlUmmReEstablish)


   kwUmmFreeDlRbCb(gCb, rbCb);

   rbCb->m.umDl.vtUs = 0;

   /* this would have been set when re-establishment was triggered
      for SRB 1 */
   kwDlUtlResetReestInProgress(rbCb);
   
   RETVOID;
}
#if 1
/**
 * @brief   Handler to create the header and complete a PDU.
 *       
 * @details
 *     This function is used to create the header of a PDU and concatenate  it
 *     with the data part of the PDU.
 *     Also updates the statistics
 *     Sets the passed pdu to NULLP
 *
 * @param[in]     gCb            RLC instance control block
 * @param[in,out] rbCb           RB control block 
 * @param[in]     pdu            PDU  
 * @param[in]     fi             Framing Info field
 * @param[out]    datReqPduInfo  Holder in which to copy the created PDU pointer
 *
 * @return  Void
*/ 
#ifdef ANSI
PRIVATE Void kwUmmCreatePdu
(
KwCb        *gCb,
KwDlRbCb    *rbCb,           
Buffer      *pdu,           
U8          fi, 
KwPduInfo   *datReqPduInfo
)
#else
PRIVATE Void kwUmmCreatePdu(gCb, rbCb, pdu, fi, datReqPduInfo)
KwCb        *gCb;
KwDlRbCb    *rbCb;          
Buffer      *pdu;           
U8          fi;
KwPduInfo   *datReqPduInfo
#endif
{
   KwSn   sn;        /*  Sequence Number */
   U32    liCount;   /*  LI count */
   U8     e = 0;     /* Extension Bit */
   U32    count;     /* Loop Counter */
   U32    hdrSz;

   /* create a big array to store the header, assuming 3 bytes per 2 L1s 
    * (2 bytes if only a single LI) and 2 bytes for the 
    *  FI and SN
    * size of header = ( NumLi /2 ) * 3 + (NumLi % 2) * 2 + 2;
    * where NumLi = Number of Length Indicators to be sent
   */
   U8 hdr[((KW_MAX_DL_LI >> 1) * 3) + ((KW_MAX_DL_LI & 0x01) << 1) + 2];
   U32 idx = 0; /* To index to the hdr array */
   
   /* Note: idx is not checked against crossing the hdr array bound as 
    * liCount will be < KW_MAX_DL_LI and as per the size calculated above; 
    * idx cannot cross the array
    */

   TRC2(kwUmmCreatePdu) 


   /* stats updated before for bytes sent before adding RLC headers */
   kwUtlIncrementGenStsBytesAndPdusSent(&gCb->genSts, pdu);
         
   sn = rbCb->m.umDl.vtUs;
   liCount = rbCb->m.umDl.numLi;
   
   if(liCount > KW_MAX_DL_LI)
      liCount = KW_MAX_DL_LI;

   /* if there are any LI's then set the first E bit */
   if(liCount)
   {
      e = 1;
   }
   
   if (rbCb->m.umDl.snLen == 1) 
   {
      hdr[idx++] = (fi << 6) | (e << 5) | sn;
   }
   else /* SN length is 2 */
   {
      /* SN length is 10 bits */
      hdr[idx] = (fi << 3) | (e << 2) | (sn >> 8);
      hdr[++idx] = sn & 0xff;
      ++idx;
   }

   hdrSz = sizeof(hdr); 
   for (count = 0;count < liCount;count++)
   {
      /* In each iteration we try and encode 2 LIs */
      /* if this is the last LI then e should be 0 */
      if(count == liCount - 1)
      {
         e = 0;
      }
      
      /* ccpu00135170  Fixing KLOCK warning */  
      if((idx + 1)>= hdrSz)
      {
	      break;
      }
      /* odd LI, 1st , 3rd etc */
      hdr[idx] = (e << 7) | (rbCb->m.umDl.li[count] >> 4);
      hdr[++idx] = (rbCb->m.umDl.li[count] & 0xf) << 4;
      
      count++;
      if(count == liCount - 1)
      {
         e = 0;
      }
      else if(count >= liCount)
      {
         break;
      }
      /* ccpu00135170  Fixing KLOCK warning */  
      if((idx + 1)>= hdrSz)
      {
	      break;
      }
      /* even one, 2nd , 4th etc LI's, count starts at 0 */
      hdr[idx] |= ((e << 3) | (rbCb->m.umDl.li[count] >> 8));
      hdr[++idx] = rbCb->m.umDl.li[count] & 0xff;
      ++idx;
   }

   /* if odd number of L1s increment idx */
   if(liCount & 0x1)
   {
      ++idx;
   }

   /* increment VT(US) */
   rbCb->m.umDl.vtUs = (rbCb->m.umDl.vtUs + 1) & rbCb->m.umDl.modBitMask;

   /* add the header to the beginning of the pdu */
   SAddPreMsgMultInOrder(hdr, idx, pdu);

   datReqPduInfo->mBuf[datReqPduInfo->numPdu++] = pdu;
   RETVOID;
}
#endif

/**
 * @brief  Handler to estimate the header size of the RLC SDUs 
 *         present in the SDU queue.
 *       
 * @details
 *     This function is used to update the estimated header size variable in RB.
 *     This function is called when a SDU is queued and when a PDU is formed and
 *     sent to the lower layer.
 *
 * @param[in] umDl  UM mode downlink control block
 *
 * @return  Void
*/ 
#ifdef ANSI
PRIVATE Void kwUmmEstHdrSz
(
KwUmDl *umDl          
)
#else
PRIVATE Void kwUmmEstHdrSz(umDl)
KwUmDl *umDl;          
#endif
{
   /* The header size is estimated as :
          If sdu count = 0 then 0
          else sdu count * 2 + 1; the 1 is added for the FI and SN byte; 
          2 for one LI and E
    */   
   umDl->estHdrSz = (umDl->sduQ.count)?((umDl->sduQ.count << 1) + 1) : 0;
   
   RETVOID;
}

/**
 * @brief   Handler to discard a SDU.
 *       
 * @details
 *     This function is used to discard a SDU after receiving
 *     the Discard Request from the upper layer.The SDU is discarded if 
 *     it is present and is not mapped to any PDU yet. 
 *     The discards coming from the upper layer would be coming in 
 *     sequence according to the sduId, so we should find the sduId at the 
 *     head of the sduQ. Discards if there is a match else does nothing.
 *
 * @param[in] rbCb   RB control block 
 * @param[in] sduId  SDU ID of the SDU to be discarded
 *
 * @return  Void
*/
#ifdef ANSI
PUBLIC Void kwUmmDiscSdu
(
KwCb       *gCb,
KwDlRbCb   *rbCb,                
U32        sduId                
)
#else
PUBLIC Void kwUmmDiscSdu(gCb,rbCb,sduId)
KwCb       *gCb;
KwDlRbCb   *rbCb;                
U32        sduId;                
#endif
{
   CmLList *tmpNode;  /* Temporary Node in SDU queue */
   
   TRC2(kwUmmDiscSdu)

  
   CM_LLIST_FIRST_NODE(&rbCb->m.umDl.sduQ,tmpNode);

   if (tmpNode)
   {
      KwSdu *sdu = (KwSdu *)tmpNode->node;
      
      if (sdu->mode.um.sduId == sduId && sdu->mode.um.isSegmented == FALSE)
      {
/* kw005.201 added support for L2 Measurement */
         KW_RMV_SDU(gCb,&rbCb->m.umDl.sduQ,sdu);
         gCb->genSts.numSduDisc++;         
      }
   }

   RETVOID;
}

/*
 *
 * @brief
 *    function to free/release the UnAcknowledged mode RBCB buffers
 *
 * @details
 *    This primitive Frees the Unacknowldged Mode RbCb sdu queue
 *
 * @param [in]   gCb    - RLC instance control block
 * @param [in]   rbCb   - RB Control Block
 *
 * @return Void
 */
#ifdef ANSI
PUBLIC Void kwUmmFreeDlRbCb
(
KwCb       *gCb,
KwDlRbCb   *rbCb
)
#else
PUBLIC Void kwUmmFreeDlRbCb(gCb,rbCb)
KwCb       *gCb;
KwDlRbCb   *rbCb;
#endif
{
   TRC2(kwUmmFreeDlRbCb)


   /* cat the SDU queue to the to be freed list */
   cmLListCatLList(&(gCb->u.dlCb->toBeFreed.sduLst),&(rbCb->m.umDl.sduQ));
   kwUtlRaiseDlCleanupEvent(gCb);

   RETVOID;
} /* kwUmmFreeDlRbCb */
















/********************************************************************30**
         End of file:     gp_umm_dl.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:32:03 2015
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
/main/1      ---       vp   1. initial release.
*********************************************************************91*/
/*@}*/
