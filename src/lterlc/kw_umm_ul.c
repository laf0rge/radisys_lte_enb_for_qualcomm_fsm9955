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

     File:     gp_umm_ul.c

     Sid:      gp_umm_ul.c@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:43:20 2014
     Prg:      vp 

**********************************************************************/
static const char* RLOG_MODULE_NAME="RLC";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=240;

/** 
 * @file gp_umm_ul.c
 * @brief RLC Unacknowledged Mode uplink module
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
#include "kw_ul.h"


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
#include "kw_ul.x"

#define KW_MODULE (KW_DBGMASK_UM | KW_DBGMASK_UL)

PRIVATE S16 kwUmmExtractHdr ARGS ((KwCb *gCb, 
                                   KwUlRbCb *rbCb,
                                   Buffer *pdu,
                                   KwUmHdr *umHdr));

PRIVATE Void kwUmmReAssembleSdus ARGS ((KwCb *gCb,
                                        KwUlRbCb *rbCb,
                                        KwUmRecBuf *umRecBuf));

#ifdef QCOM_STATS
PUBLIC U32  kwUlrate_rgu;
#endif


#ifndef QCOM_STATS
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
extern U32 isMemThreshReached(Region region);
#endif
#endif
#endif
/**
 * @brief  Finds and sets the next VR(UR) depending on the 
 *         passed sequence number
 *       
 * @details
 *    Finds the next VR(UR) depending on the passed SN. Updates VR(UR) to 
 *    the SN of the first UMD PDU with SN >= _nextSn that has not been received
 *
 * @param[in] umUl      pointer to Um mode uplink control block
 * @param[in] nextSn    Sequence number after which the VR(UR) is to set to
 *
 * @return  Void
*/ 
PRIVATE Void kwUmmFindNextVRUR (KwUmUl* umUl, KwSn nextSn)
{
   KwSn ur = KW_UM_GET_VALUE(umUl->vrUr, *umUl);
   
   KwSn nextSnToCompare = KW_UM_GET_VALUE(nextSn,*umUl);
   
   while (ur < nextSnToCompare)
   {
      if (!(umUl->recBuf[nextSn])) /* if the buffer is empty, SN not received */
      {
         umUl->vrUr = nextSn;
         break;
      }
      nextSn = (nextSn + 1) & umUl->modBitMask; 
      nextSnToCompare = KW_UM_GET_VALUE(nextSn,*umUl);
   }
}

/**
 * @brief  Checks whether a sequence number is within the 
 *         re-ordering window or not
 *       
 * @param[in] sn        Sequence Number to be checked
 * @param[in] umUl      pointer to Um mode uplink control block
 *
 * @return  S16
 *      -# TRUE 
 *      -# FALSE
 *
 * @return  Void
*/
PRIVATE S16 kwUmmCheckSnInReordWindow (KwSn sn, 
                                       CONSTANT KwUmUl* CONSTANT umUl)  
{
   return (KW_UM_GET_VALUE(sn, *umUl) < KW_UM_GET_VALUE(umUl->vrUh, *umUl)); 
}

/**
 * @brief  Handler to process the Data Indication from the lower layer 
 *         and send the PDUs to re-assembly unit. 
 *       
 * @details
 *    This function processes the PDUs received from the lower layer
 *    re-orders them and sends them one after the other in sequence 
 *    to the re-assembly unit.
 *
 * @param[in] gCb      RLC Instance control block
 * @param[in] rbCb     RB control block 
 * @param[in] pduInfo  Pdu information 
 *
 *  @return Void
*/
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

#ifdef ANSI
PUBLIC Void kwUmmProcessPdus
(
KwCb      *gCb,
KwUlRbCb  *rbCb,                   /* Rb Control Block */
KwPduInfo *pduInfo,                 /* Pdu  data and related information */
U32       ttiCnt                  /* ttiCnt received from MAC */
)
#else
PUBLIC Void kwUmmProcessPdus(rbCb,pduInfo,ttiCnt)
KwCb      *gCb;
KwUlRbCb  *rbCb;                   /* Rb Control Block */
KwPduInfo *pduInfo;                /* Pdu  data and related information */
U32       ttiCnt;                  /* ttiCnt received from MAC */
#endif
#else
#ifdef ANSI
PUBLIC Void kwUmmProcessPdus
(
KwCb      *gCb,
KwUlRbCb  *rbCb,                /* Rb Control Block */
KwPduInfo *pduInfo              /* Pdu  data and related information */
)
#else
PUBLIC Void kwUmmProcessPdus(rbCb,pduInfo)
KwCb      *gCb;
KwUlRbCb  *rbCb;                /* Rb Control Block */
KwPduInfo *pduInfo;             /* Pdu  data and related information */
#endif
#endif
{
   U16          *vrUh;      /* vr(uh) */
   U16          *vrUr;      /* vr(ur) */
   KwSn         *vrUx;      /* vr(ux) */
   U16          curSn;      /* Current Sequence Number */
   U32          pduCount;   /* PDU count */
   U32          count;      /* Loop counter */
   KwUmRecBuf   **recBuf;   /* UM Reception Buffer */

   Bool         tmrRunning;   /* Boolean for checking Tmr */
/* kw005.201 added support for L2 Measurement */

   TRC2(kwUmmProcessPdus)


   count = 0;

   /* pduCount should be the min of RGU_MAX_PDU and pduInfo->numPdu */
   pduCount = (pduInfo->numPdu < RGU_MAX_PDU)? pduInfo->numPdu : RGU_MAX_PDU;
   
   vrUh   = &(rbCb->m.umUl.vrUh);
   vrUr   = &(rbCb->m.umUl.vrUr);
   vrUx   = &(rbCb->m.umUl.vrUx);
   recBuf = (rbCb->m.umUl.recBuf);

   while (count < pduCount)
   {
      KwSn   ur; 
      KwSn   uh; 
      KwSn   seqNum;
      Buffer *pdu = pduInfo->mBuf[count];
      KwUmRecBuf *tmpRecBuf;
      gCb->genSts.pdusRecv++;
#ifndef QCOM_STATS
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
      extern U32 ulrate_rgu;
      MsgLen len;
      SFndLenMsg(pdu, &len);
      ulrate_rgu += len;
#endif
#endif      
#endif      
      /* create a buffer to be later inserted into the reception buffer */
      KW_ALLOC_WC(gCb, tmpRecBuf, sizeof(KwUmRecBuf));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (tmpRecBuf == NULLP)
      {   
         RLOG_ARG2(L_FATAL, DBG_RBID,rbCb->rlcId.rbId,
                  "Memory allocation failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         SPutMsg(pdu);

         RETVOID;
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */      
      /* ccpu00142274 - UL memory based flow control*/ 
#ifndef QCOM_STATS
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
    /* Changed the condition to TRUE from ROK  */
     if(isMemThreshReached(kwCb[0]->init.region) == TRUE)
     {
        extern U32 rlculdrop;
        rlculdrop++;
        KW_FREE_BUF(pdu);
        KW_FREE_WC(gCb, tmpRecBuf, sizeof(KwUmRecBuf));
        /*Fix for CR ccpu00144030: If threshhold is hit then also count
         *should be incrmented */
        count++;
        continue;
     }
#endif
#endif
#endif
      /* get the pdu header */
      if (kwUmmExtractHdr(gCb, rbCb, pdu, &(tmpRecBuf->umHdr)))  
      {
         RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                  "Header Extraction Failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         /* Header extraction is a problem. 
          * log an error and free the allocated memory */
         /* ccpu00136940 */
         KW_FREE_WC(gCb, tmpRecBuf, sizeof(KwUmRecBuf));
         SPutMsg(pdu);
         count++;
         /* kw005.201 ccpu00117318, updating the statistics */
         gCb->genSts.errorPdusRecv++;
         continue;
      }
      curSn = tmpRecBuf->umHdr.sn;

      /* Check if the PDU should be discarded or not */
      ur = KW_UM_GET_VALUE(KW_UMUL.vrUr, KW_UMUL); 
      uh = KW_UM_GET_VALUE(KW_UMUL.vrUh, KW_UMUL); 
      seqNum = KW_UM_GET_VALUE(curSn, KW_UMUL); 

      if (((ur < seqNum) && (seqNum < uh) && (KW_UMUL.recBuf[curSn])) || 
          (seqNum < ur)) 
      {
         /* PDU needs to be discarded */
         RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                  "Received a duplicate pdu with sn %d UEID:%d CELLID:%d",
                  curSn,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         KW_FREE_BUF(pdu);
         KW_FREE_WC(gCb, tmpRecBuf, sizeof(KwUmRecBuf));
         count++;
         /* kw005.201 ccpu00117318, updating the statistics */
         gCb->genSts.unexpPdusRecv++;
         continue;
      } 

      /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

      /* kw006.201 ccpu00120058, reduced code complexity by adding new function */
      kwUtlCalUlIpThrPut(gCb,rbCb, pdu, ttiCnt);

#endif 

      recBuf[curSn] = tmpRecBuf;

      recBuf[curSn]->pdu = pdu;
      SFndLenMsg(pdu,&(recBuf[curSn]->pduSz));
      /* kw005.201 ccpu00117318, updating the statistics */
      gCb->genSts.bytesRecv += recBuf[curSn]->pduSz;
      
      if (!kwUmmCheckSnInReordWindow(curSn,&KW_UMUL))
      {  /* currSn is outside re-ordering window */
         *vrUh  = (curSn + 1) & KW_UMUL.modBitMask;

         /* re-assemble all pdus outside the modified re-ordering window */
         /* the first SN is VR(UR) */
         if (!kwUmmCheckSnInReordWindow(*vrUr,&KW_UMUL))
         {
            /* TODO : should it be VR(UR) + 1 ?... check, earlier it was so */
            KwSn sn = *vrUr; /* SN's which need to be re-assembled */
            KwSn lowerEdge;  /* to hold the lower-edge of the 
                                re-ordering window */

            /* The new value ov VR(UR) is the lower end of the window i
             * and SN's still this value need to be re-assembled */
            
            *vrUr = (*vrUh - KW_UMUL.umWinSz) &  KW_UMUL.modBitMask;         
            lowerEdge = KW_UM_GET_VALUE(*vrUr ,KW_UMUL);
            
            while (KW_UM_GET_VALUE(sn, KW_UMUL) < lowerEdge)
            {
               if (recBuf[sn])
               {
                  kwUmmReAssembleSdus(gCb,rbCb,recBuf[sn]);
                  KW_FREE_WC(gCb,recBuf[sn],sizeof(KwUmRecBuf));
                  recBuf[sn] = NULLP;
               }
               sn = (sn + 1) & KW_UMUL.modBitMask;
            }
         }
      }
      if (recBuf[*vrUr])
      {
         KwSn sn       = *vrUr;
         KwSn tSn      = KW_UM_GET_VALUE(sn,KW_UMUL); 
         KwSn tVrUr;       

         /* set VR(UR) to next SN > current VR(UR) which is not received */
         KwSn nextVrUr = (*vrUr + 1) & KW_UMUL.modBitMask;
         kwUmmFindNextVRUR(&KW_UMUL, nextVrUr);

         /* re-assemble SDUs with SN < Vr(UR) */
         tVrUr = KW_UM_GET_VALUE(*vrUr,KW_UMUL);
         while (recBuf[sn] && tSn < tVrUr)
         {
            kwUmmReAssembleSdus(gCb,rbCb,recBuf[sn]);
            KW_FREE_WC(gCb,recBuf[sn],sizeof(KwUmRecBuf));
            recBuf[sn] = NULLP;
            sn = (sn + 1) & KW_UMUL.modBitMask;
            tSn = KW_UM_GET_VALUE(sn, KW_UMUL);
         }
      }

      tmrRunning = kwChkTmr(gCb,(PTR)rbCb, KW_EVT_UMUL_REORD_TMR);

      if (tmrRunning) 
      {
         KwSn tVrUx = KW_UM_GET_VALUE(*vrUx, KW_UMUL);
         KwSn tVrUr = KW_UM_GET_VALUE(*vrUr ,KW_UMUL);

         KwSn tVrUh = KW_UM_GET_VALUE(*vrUh, KW_UMUL);

         S16 ret = kwUmmCheckSnInReordWindow(*vrUx, &KW_UMUL);

         if ( (tVrUx <= tVrUr) || ((!ret) && (tVrUx != tVrUh))) 
         {
            kwStopTmr(gCb,(PTR)rbCb,KW_EVT_UMUL_REORD_TMR);
            tmrRunning = FALSE;
         }
      }

      if (!tmrRunning)
      {
         if (KW_UM_GET_VALUE(*vrUh, KW_UMUL) > KW_UM_GET_VALUE(*vrUr, KW_UMUL))
         {
            kwStartTmr(gCb,(PTR)rbCb,KW_EVT_UMUL_REORD_TMR); 
            *vrUx = *vrUh;
         }
      }
      count++;
   }/* end while count < pduCount */
#ifdef LTE_L2_MEAS
   kwUtlCalUlIpThrPutIncTTI(gCb, rbCb,ttiCnt);
#endif /* LTE_L2_MEAS */
   RETVOID;   
}

/**
 * @brief  Handler to reassemble the SDUs and send them to the upper layer.
 *       
 * @details
 *    This function processes the received in-sequence PDU and
 *    re-assembles the SDUs and sends them to the upper layer.
 *
 * @param[in] gCb        RLC Instance control block
 * @param[in] rbCb       RB control block
 * @param[in] umRecBuf   Reception Buffer to be Re-Assembled 
 *
 * @return  Void
*/
#ifdef ANSI
PRIVATE Void kwUmmReAssembleSdus
(
KwCb         *gCb,
KwUlRbCb     *rbCb,   
KwUmRecBuf   *umRecBuf 
)
#else
PRIVATE Void kwUmmReAssembleSdus(gCb,rbCb,umRecBuf)
KwCb         *gCb;
KwUlRbCb     *rbCb;     
KwUmRecBuf   *umRecBuf; 
#endif
{
   U32      liCount;        /* LI count */
   U32      count;          /* Loop counter */
   U8       fi;             /* Framing Info */
   U16      sn;             /* Sequence Number of current PDU */
   MsgLen   len;            /* PDU Length */
   Buffer   *sdu;           /* SDU to be sent to upper layer */
   Buffer   *remPdu;        /* Remaining PDU */
   Buffer   **partialSdu;   /* Partial SDU */

   TRC2(kwUmmReAssembleSdus)  


   liCount = umRecBuf->umHdr.numLi;
   fi = umRecBuf->umHdr.fi;
   sn =  umRecBuf->umHdr.sn;
 
   for (count = 0; (count <= liCount);count++)
   {
      if (count < liCount )
         len = umRecBuf->umHdr.li[count];
      else
      {
         if (!(umRecBuf->pdu))
         {
            RETVOID;
         }
         SFndLenMsg(umRecBuf->pdu,&len);
      }
         
      /* get the sdu out of the pdu */
      SSegMsg(umRecBuf->pdu,len,&remPdu);
      sdu = umRecBuf->pdu;
      umRecBuf->pdu = remPdu;
      
      partialSdu = &(rbCb->m.umUl.partialSdu);
      /* While re-assembling the SDUs, consider the first LI and perform 
       * the following steps.
          -# If the first bit of FI(Framing Info of 2  bits) is set => 
                -# The current Data field in the PDU is a segment. 
                   So form a SDU only if the 
                       rbCb->m.um.umUl.partialSdu exists and the SNs are 
                       in-sequence.
                -# If there are no LIs and the second bit of LI is 1 
                   then a partial SDU is formed which would not be sent 
                   to the upper layer.
          -# else
                -# If rbCb->partialSdu is not NULL then flush it off.
                -# If LI count > 0 or LI count is 0 and second bit 
                   of FI is not 1 
                         The SDU is complete.So send it to upper layer.
                -# else
                         The SDU is partial and is placed 
                         in rbCb->m.um.umUl.partialSdu;
      */
             
      if (0 == count )
      {
         if (fi & 2)
         {
            if ((*partialSdu) && 
                (sn == ((rbCb->m.umUl.sn + 1) & rbCb->m.umUl.modBitMask)))
            {
               SCatMsg(*partialSdu,sdu,M1M2);
               KW_FREE_BUF(sdu);
               if (liCount > 0 || !(fi & 1))
               {
                  kwUtlSndDatInd(gCb,rbCb,*partialSdu);
                  *partialSdu = NULLP;
               }
            }
            else
            {
                /* Partial Sdu stored is not valid now.So free it */
               if (*partialSdu)
               {
                  KW_FREE_BUF(*partialSdu);
                  *partialSdu = NULLP;
               }
                  
               KW_FREE_BUF(sdu);
               sdu = NULLP;
            }
         }
         else
         {
            if (*partialSdu)
            {
               KW_FREE_BUF(*partialSdu);  /* RLC mem leak fix */
               *partialSdu = NULLP;
            }
            
            if (liCount > 0 || !( fi & 1))
            {
               kwUtlSndDatInd(gCb,rbCb,sdu);
            }
            else
            {
                  *partialSdu = sdu; 
            }
         }
      }
      /*
          If the SDU pointer has the last Data field of the PDU
            -# If FI is 1,place the SDU in rbCb->m.um.umDl.partialSdu
            -# else send the SDU to upper layer.
      */
       else if (count == liCount)
      {
         if (fi & 1)
         {
            *partialSdu = sdu;
         }
         else
         {
            kwUtlSndDatInd(gCb, rbCb, sdu);
         }
      }
      /*  
         If the LI is something other than the first one, 
         just send the SDU to the upper layer */
      else
      {
         kwUtlSndDatInd(gCb, rbCb, sdu);
      }
   }
   rbCb->m.umUl.sn = sn;

   RETVOID;
}

/**
 * @brief  Handler to process the re-establishment request received 
 *         from the upper layer.
 *       
 * @details
 *    This function does the following functions : 
 *       - If direction of the RB is downlink : 
 *         Remove all the SDUs in the SDU queue.
 *       - If direction of the RB is uplink   : 
 *         Call kwUmmReAssembleSdus() for each PDU with SN < VR(UH)
 *
 * @param[in] gCb        RLC Instance control block
 * @param[in] rlcID      Identity of the RLC entity for which 
 *                       re-establishment is to be done
 * @param[in] rbCb       RB control block for which re-establishment 
 *                       is to be done
 *
 * @return  Void
*/ 
#ifdef ANSI
PUBLIC Void kwUmmUlReEstablish
(
KwCb         *gCb,
CmLteRlcId   *rlcId,
KwUlRbCb     *rbCb
)
#else
PUBLIC Void kwUmmUlReEstablish(gCb, rlcId, rbCb)
KwCb         *gCb;
CmLteRlcId   *rlcId;
KwUlRbCb     *rbCb;
#endif
{
   KwSn         curSn;
   KwSn         vrUh;
   KwUmRecBuf   **recBuf;   /* UM Reception Buffer */
   KwKwuSapCb   *kwKwSap;   /* KWU SAP Information */

   TRC2(kwUmmUlReEstablish)


   curSn = rbCb->m.umUl.vrUr;
   vrUh  = KW_UM_GET_VALUE(rbCb->m.umUl.vrUh,rbCb->m.umUl);
   recBuf =  rbCb->m.umUl.recBuf;

   if(TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_UMUL_REORD_TMR))
   {
       kwStopTmr(gCb,(PTR)rbCb,KW_EVT_UMUL_REORD_TMR);
   }
   
   while (KW_UM_GET_VALUE(curSn,rbCb->m.umUl) < vrUh)
   {
      if ( recBuf[curSn] != NULLP )
      {
         kwUmmReAssembleSdus(gCb,rbCb,recBuf[curSn]);
         KW_FREE_WC(gCb,recBuf[curSn],sizeof(KwUmRecBuf));
         recBuf[curSn] = NULLP;
      } 
      curSn = (curSn + 1) & rbCb->m.umUl.modBitMask;
   }
   rbCb->m.umUl.vrUr = 0;
   rbCb->m.umUl.vrUh = 0;
   rbCb->m.umUl.vrUx = 0;

   kwKwSap = gCb->u.ulCb->kwuUlSap + KW_UI_PDCP;

   /* In the UM Mode always send reestablish-indication to Upper Latyer*/
   KwUiKwuReEstCmpInd(&kwKwSap->pst, kwKwSap->suId, *rlcId);

   RETVOID;
}

/**
 * @brief  Handler to extract the header from a PDU
 *       
 * @details
 *    This function is used to extract the header of a PDU and store it 
 *    along with the PDU buffer.The sequence number,framing info 
 *    and LIs are extracted by this function.
 *
 * @param[in] gCb      RLC Instance control block
 * @param[in] rbCb     Rb Control block for which the pdu is received
 * @param[in] pdu      PDU buffer
 * @param[out] umHdr   UM header to be filled after extraction
 *
 * @return  S16
 *      -# TRUE 
 *      -# FALSE
*/
#ifdef ANSI
PRIVATE S16 kwUmmExtractHdr
(
KwCb       *gCb,
KwUlRbCb   *rbCb, 
Buffer     *pdu, 
KwUmHdr    *umHdr 
)
#else
PRIVATE S16 kwUmmExtractHdr(gCb, rbCb, pdu, umHdr)
KwCb       *gCb;
KwUlRbCb   *rbCb; 
Buffer     *pdu; 
KwUmHdr    *umHdr; 
#endif
{
   U8        e;         /* Extension Bit */
   Data      dst[2];    /* Destination Buffer */
   S32       totalSz;   /* Sum of LIs */
   MsgLen    pduSz;     /* PDU size */
   S16       ret;       /* Return Value */

   TRC3(kwUmmExtractHdr)


   SFndLenMsg(pdu,&pduSz);
 
   if ( rbCb->m.umUl.snLen == 1)
   {
      ret = SRemPreMsg(dst,pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
      if (ret != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                  "SRemPreMsg Failed for 5 bit SN UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         RETVALUE(RFAILED);
      }
#endif
      pduSz--;
      umHdr->sn = (dst[0]) & 0x1F; 
      umHdr->fi = (dst[0]) >> 6;
      e       = (dst[0]>>5) & 0x01;
   }
   else
   {
      /* snLen - sequnce length will be 10 bits requiring 2 bytes */ 
      ret = SRemPreMsgMult(dst,2,pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
      if (ret != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                  "SRemPreMsgMult Failed for 10 bits SN UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         RETVALUE(RFAILED);
      }
#endif
      pduSz -= 2;
   
      /* kw005.201 R9 Upgrade 3gpp spec 36.322 ver9.3.0 CR0082      *
       * Removed the "if" condition for checking the reserved field *
       * Added mask 0x03 for extracting the FI field.          */

      umHdr->fi = ( (dst[0] ) >> 3) & 0x03;
      e       = ( (dst[0] ) >> 2) & 0x01;
      umHdr->sn = (  dst[0] & 0x03) << 8;
      umHdr->sn  |= dst[1];
   }

   umHdr->numLi = 0;
   
   totalSz = 0;
   while(e && umHdr->numLi < KW_MAX_UL_LI )
   {
      ret = SRemPreMsgMult(dst,2,pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
      if (ret != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                  "SRemPreMsgMult Failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         RETVALUE(RFAILED);
      }
#endif
      umHdr->li[umHdr->numLi] = ((dst[0]) & 0x7F) << 4;
      umHdr->li[umHdr->numLi] |= dst[1] >> 4;
      if ( 0 == umHdr->li[umHdr->numLi] )
      {
         RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                  "Received LI as 0 UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         RETVALUE(RFAILED); 
      }
      totalSz += umHdr->li[umHdr->numLi];
      if ( pduSz <=  totalSz )
      {
         RLOG_ARG3(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                  "SN [%d]: UEID:%d CELLID:%d",
                  umHdr->sn, 
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         RLOG_ARG4(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                  "Corrupted PDU as TotSz[%lu] PduSz[%lu] UEID:%d CELLID:%d ",
                  totalSz, 
                  pduSz,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         RETVALUE(RFAILED); /* the situation where in the PDU size 
                            is something that does not match with 
                            the size in LIs*/
      }
      umHdr->numLi++;
      pduSz -= 2;

      e = ((dst[0]) & 0x80) >> 7;
   
      if ( e && umHdr->numLi < KW_MAX_UL_LI)
      {
         U8 tmp = ((dst[1]) & 0x08) >> 3;
         umHdr->li[umHdr->numLi] = ( dst[1] & 0x07) << 8;

         ret = SRemPreMsg(dst,pdu);

#if (ERRCLASS & ERRCLS_DEBUG)
         if (ret != ROK)
         {
            RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                     "SRemPreMsg Failed UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            RETVALUE(RFAILED);
         }
#endif
         umHdr->li[umHdr->numLi] |= ( dst[0] );    /* The first byte lies in 
                                                   the first 8 bits.We want 
                                                   them in the last 8 bits */
         if ( 0 == umHdr->li[umHdr->numLi] )
         {
            RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                     "Received LI as 0 UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            RETVALUE(RFAILED); 
         }
         totalSz += umHdr->li[umHdr->numLi];
         pduSz--;
         umHdr->numLi++;

         if (pduSz < totalSz)
         {
            RETVALUE(RFAILED); /* the situation where in the PDU size is 
                               something that does not match with the 
                               size in LIs*/
         }

         e = tmp;
      }
   } /* while(e && umHdr->numLi < KW_MAX_LI ) */
   if (e)
   {
      /* PDU was constructed with LIs that exceeded KW_MAX_LI */
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK); 
}
   
/**
 * @brief Handles expiry of re-ordering timer
 *
 * @param[in] gCb     RLC Instance control block
 * @param[in] rbCb    Rb Control block for which re-order timer expired
 *
 * @return  Void
*/
#ifdef ANSI
PUBLIC Void kwUmmReOrdTmrExp
(
KwCb       *gCb,
KwUlRbCb   *rbCb     
)
#else
PUBLIC Void kwUmmReOrdTmrExp(gCb, rbCb)
KwCb       *gCb;
KwUlRbCb   *rbCb;   
#endif
{
   KwSn prevVrUr;   /* prevVrUr */

   TRC3(kwUmmReOrdTmrExp)


   prevVrUr = KW_UMUL.vrUr;

   /* set VR(UR) to SN >= VR(UX) that has not been received */
   kwUmmFindNextVRUR(&KW_UMUL, KW_UMUL.vrUx);

   while (KW_UM_GET_VALUE(prevVrUr,KW_UMUL) < 
          KW_UM_GET_VALUE(KW_UMUL.vrUr,KW_UMUL))
   {
      if (KW_UMUL.recBuf[prevVrUr])
      {
         kwUmmReAssembleSdus(gCb, rbCb, KW_UMUL.recBuf[prevVrUr]);
         if(KW_UMUL.recBuf[prevVrUr]->pdu != NULLP) /* RLC mem leak fix */
         {
            KW_FREE_BUF(KW_UMUL.recBuf[prevVrUr]->pdu);
         }
         KW_FREE_WC(gCb, KW_UMUL.recBuf[prevVrUr], sizeof(KwUmRecBuf));
         KW_UMUL.recBuf[prevVrUr] = NULLP;
      }

      prevVrUr = (prevVrUr + 1) & rbCb->m.umUl.modBitMask;
   }

   if (KW_UM_GET_VALUE(KW_UMUL.vrUh, KW_UMUL) > 
       KW_UM_GET_VALUE(KW_UMUL.vrUr, KW_UMUL))
   {
      kwStartTmr(gCb,(PTR)rbCb,KW_EVT_UMUL_REORD_TMR);
      KW_UMUL.vrUx = KW_UMUL.vrUh;
   }
}

/**
 * @brief
 *   Function to release/free the UnAcknowledged Mode Module  RbCb buffers
 *
 * @details
 *   This primitive Frees the UM RbCb transmission Buffer, retransmission
 *   Buffer and reciption Buffers
 *
 * @param [in]   gCb   - RLC instance Control Block
 * @param [in]   rbCb  - RB Control Block
 *
 * @return   void
 */

#ifdef ANSI
PUBLIC Void kwUmmFreeUlRbCb
(
KwCb       *gCb,
KwUlRbCb   *rbCb
)
#else
PUBLIC Void kwUmmFreeUlRbCb(gCb,rbCb)
KwCb       *gCb;
KwUlRbCb   *rbCb;
#endif
{
   KwSn         curSn = 0;           /* sequence number of PDU */
   KwSn         windSz;              /* PDU window size */
   KwUmRecBuf   **umRecBuf;          /* UM module receive buffer */

   TRC2(kwUmmFreeUlRbCb)


   windSz  = rbCb->m.umUl.umWinSz << 1;

   umRecBuf =  rbCb->m.umUl.recBuf;

   if(TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_UMUL_REORD_TMR))
   {
      kwStopTmr(gCb,(PTR)rbCb,KW_EVT_UMUL_REORD_TMR);
   }
   while (curSn < windSz)
   {
      if (umRecBuf[curSn] != NULLP)
      {
         KW_FREE_BUF_WC(umRecBuf[curSn]->pdu);
         umRecBuf[curSn]->pdu = NULLP;

         KW_FREE_WC(gCb, umRecBuf[curSn], sizeof(KwUmRecBuf));
         umRecBuf[curSn] = NULLP;
      }
      curSn++;
   }
   KW_FREE_WC(gCb,rbCb->m.umUl.recBuf, (windSz ) * sizeof(KwUmRecBuf*));
   rbCb->m.umUl.recBuf = NULLP;
   RETVOID;
} 


/********************************************************************30**
         End of file:     gp_umm_ul.c@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:43:20 2014
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
