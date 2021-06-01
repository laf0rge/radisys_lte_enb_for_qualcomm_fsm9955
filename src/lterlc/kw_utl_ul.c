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
  
     Desc:     Source code for RLC Utility Module
               This file contains following functions

                  --kwUtlSndToLi
                  --kwUtlRcvFrmLi
                  --kwUtlEmptySduQ
                  --kwUtlSndDatInd 
                  --kwUtlShutDown
     File:     gp_utl_ul.c

     Sid:      gp_utl_ul.c@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:55:23 2014

     Prg:      vp 

**********************************************************************/
static const char* RLOG_MODULE_NAME="UTL";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=210;

/** @file gp_utl_ul.c
@brief RLC Utility Module 
*/


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
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#include <sys/time.h>
#endif

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "lkw.h"           /* LKW defines */
#include "rgu.h"           /* RGU defines */

#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#include "kw_err.h"        /* Error defines */
#include "kw_ul.h"         /* RLC Uplink defines */

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
#include "ckw.x"           /* CKW includes */
#include "kwu.x"           /* KWU includes */
#include "lkw.x"           /* LKW includes */
#include "rgu.x"           /* RGU includes */

#include "kw.x"            /* RLC inlcudes */
#include "kw_ul.x"         /* RLC uplink includes */
#ifdef SS_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
PUBLIC S16 SMrkUlPkt(Buffer *mbuf);
#endif
#define KW_MODULE (KW_DBGMASK_DUT | KW_DBGMASK_UL) /* for debugging purpose */


/**
 *
 * @brief 
 *        Handler for receiving data for multiple logical channels from MAC.
 *
 * @details
 *        This function receives the data sent by MAC for one or more 
 *        logical channels.It calls the UMM or AMM functions to process
 *        the PDUs and send them to the uppper layer.  
 *
 * @param[in] gCb          - RLC instance control block 
 * @param[in] datIndInfo   - Data Indication Information containing the PDU(s)
 *                            for one or more logical channels  
 *
 * @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 kwUtlRcvFrmLi
(
KwCb           *gCb,                              
KwDatIndInfo   *datIndInfo                       
)
#else
PUBLIC S16 kwUtlRcvFrmLi(gCb,datIndInfo)  
KwCb           *gCb;                     
KwDatIndInfo   *datIndInfo;             
#endif
{
   U32         count;      /* Loop Counter */
   KwPduInfo   *pduInfo;   /* PDU Information */
   KwUlRbCb    *rbCb;      /* RB Control Block */
   KwUlUeCb    *ueCb;      /* UE Control Block */
/* kw005.201 added support for L2 Measurement */

   TRC2(kwUtlRcvFrmLi)


   ueCb = NULLP;
   
   /* fetch UeCb  */
   if( ROK != kwDbmFetchUlUeCb(gCb,datIndInfo->rnti,datIndInfo->cellId,&(ueCb)))
   {
      /* Fetch UeCb failed */
      RLOG_ARG1(L_ERROR,DBG_CELLID,datIndInfo->cellId,
               "UEID:%d UeCb not found",
               datIndInfo->rnti);
      /* free the buffers inside the datIndInfo */
      U32 i,j;
      for(i = 0; i< datIndInfo->numLch; i++)
      {
         for(j = 0; j < datIndInfo->lchData[i].pdu.numPdu; j++)
         {
            if(datIndInfo->lchData[i].pdu.mBuf[j])
            {
               KW_FREE_BUF_WC(datIndInfo->lchData[i].pdu.mBuf[j]);
            }
         }
      }
      
      RETVALUE(RFAILED);
   }

#ifdef LTE_L2_MEAS

   if (RGU_L2M_UL_BURST_START == datIndInfo->burstInd)
   {
      ueCb->isUlBurstActive = TRUE;
   }
   else
   {
      ueCb->firstPacketTTI = 0;
      ueCb->isUlBurstActive = FALSE;
   }
#endif
   for ( count = 0;count < datIndInfo->numLch; count++ )
   {
      rbCb = ueCb->lCh[datIndInfo->lchData[count].lcId - 1].ulRbCb;
      /* kw002.201 Removed allocation of pduInfo */ 
      pduInfo = &(datIndInfo->lchData[count].pdu);
      /* Fix for CR ccpu00138374,sometimes rbCb is NULL in UL path,
       * So inorder to avoid the crash, added this preventive check
       */
      if(rbCb == NULLP)
      {
         U32 j;
         for(j = 0; j < pduInfo->numPdu; j++)
         {
            if(pduInfo->mBuf[j])
            {
               KW_FREE_BUF_WC(pduInfo->mBuf[j]);
            }
         }
         continue;
      }

#ifdef SS_RBUF
      SMrkUlPkt(pduInfo->mBuf[0]);
#endif
      if ( rbCb->mode == CM_LTE_MODE_UM )
      {
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
         kwUmmProcessPdus(gCb,rbCb, pduInfo, datIndInfo->ttiCnt);
#else
         kwUmmProcessPdus(gCb,rbCb,pduInfo);
#endif
      }
      else if (rbCb->mode == CM_LTE_MODE_AM )
      {
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
         kwAmmProcessPdus(gCb,rbCb,  pduInfo, datIndInfo->ttiCnt);
#else
         kwAmmProcessPdus(gCb,rbCb,pduInfo);
#endif
      }
   }
   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *    Handler for sending Data Indication to the upper layer. 
 * 
 * @Details
 *    This function is used to send re-assembled SDU to the upper layer.
 *
 * @param[in]  gCb    - RLC instance Control Block 
 * @param[in]  rbCb   - RB Control Block 
 * @param[in]  sdu    - SDU to be sent to upper layer 
 *
 * @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 kwUtlSndDatInd
(
KwCb       *gCb,
KwUlRbCb   *rbCb,                   
Buffer     *sdu                    
)
#else
PUBLIC S16 kwUtlSndDatInd(gCb,rbCb,sdu)
KwCb       *gCb;
KwUlRbCb   *rbCb;                  
Buffer     *sdu;                    
#endif
{
#ifndef KW_PDCP
   KwKwuSapCb      *kwKwSap;     /* KWU SAP Information */
   KwuDatIndInfo   *datIndInfo;   /* Data Indication Information */
   KwuDatIndInfo datIndInfoTmp;
#endif

   TRC3(kwUtlSndDatInd)


#ifndef KW_PDCP
   kwKwSap = gCb->u.ulCb->kwuUlSap + KW_UI_PDCP;
   /* Creating static memory for KwuDatIndInfo. #else will be 
    * removed once the testing is done on all platforms */
#if 1
   datIndInfo = &datIndInfoTmp;
#else
   KW_SHRABL_STATIC_BUF_ALLOC(kwKwSap->pst.region, kwKwSap->pst.pool, datIndInfo,sizeof(KwuDatIndInfo));
#endif

#if (ERRCLASS & ERRCLS_ADD_RES )
   if ( datIndInfo == NULLP  )
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      KW_FREE_BUF(sdu);
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   KW_MEM_CPY(&(datIndInfo->rlcId),&(rbCb->rlcId),sizeof(CmLteRlcId));
   /* Set the "isOutofSeq" flag for each packet 
    * If packets are in-sequence set flag as TRUE else FALSE */
   datIndInfo->isOutOfSeq = rbCb->m.amUl.isOutOfSeq; 
#endif /* KW_PDCP */
   
   /* If trace flag is enabled send the trace indication */
   if(gCb->init.trc == TRUE)
   {
      /* Populate the trace params */
      kwLmmSendTrc(gCb,KWU_EVT_DAT_IND, sdu);
   }
#ifndef KW_PDCP

   KwUiKwuDatInd(&kwKwSap->pst, kwKwSap->suId, datIndInfo, sdu);
#endif   
   RETVALUE(ROK);
} /* kwUtlSndDatInd */


PRIVATE Void dumpRLCUlRbInformation(KwUlRbCb* ulRbCb)
{
   if(ulRbCb->mode == CM_LTE_MODE_UM)
   {
      U32 i;
      U32 pdusInReceptionBuffer = 0;
      U32 windSz  = ulRbCb->m.umUl.umWinSz << 1;

      for(i = 0; i< windSz; i++)
      {
         if(ulRbCb->m.umUl.recBuf[i] != NULLP)
         {
            pdusInReceptionBuffer++;
         }
      }
      
      RLOG_ARG3(L_DEBUG,DBG_RBID,ulRbCb->rlcId.rbId,
               "UM UL UEID:%d CELLID:%d Reception Buffer size = %d", 
                       (int)ulRbCb->rlcId.ueId,
                       (int)ulRbCb->rlcId.cellId,
                       (int)pdusInReceptionBuffer);
   }
   else if(ulRbCb->mode == CM_LTE_MODE_AM)
   {
      U32 i;
      U32 pdusInReceptionBuffer = 0;
      U32 totalSegs = 0;
      U32 windSz  = KW_AM_WIN_SZ << 1;
      
      for(i = 0; i< windSz; i++)
      {
         if(ulRbCb->m.amUl.recBuf[i] != NULLP)
         {
            pdusInReceptionBuffer++;
            totalSegs += ulRbCb->m.amUl.recBuf[i]->segLst.count;
         }
      }

      RLOG_ARG4(L_DEBUG,DBG_RBID,ulRbCb->rlcId.rbId,
                "AM UL UEID:%d CELLID:%d Reception Buf size = %d"
                "total segs = %d", 
                       (int)ulRbCb->rlcId.ueId,
                       (int)ulRbCb->rlcId.cellId,
                       (int)pdusInReceptionBuffer, 
                       (int)totalSegs);
   }
}

Void DumpRLCUlDebugInformation(Void)
{
   KwCb* ulInst = kwCb[0]; /* TODO : Check whether UL is 0 or 1 */
   KwUlCb* ulCb = ulInst->u.ulCb;
   KwUlUeCb *ueCb = NULLP; 

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (ROK == cmHashListGetNext(&ulCb->ueLstCp, 
                                   (PTR) ueCb, 
                                   (PTR *)&ueCb))
   {
      U32 i;
      for(i = 0; i< KW_MAX_SRB_PER_UE; i++)
      {
         KwUlRbCb* ulRbCb = ueCb->srbCb[i]; 
         if(ulRbCb != NULLP)
         {
            dumpRLCUlRbInformation(ulRbCb);
         }
      }
      for(i = 0; i< KW_MAX_DRB_PER_UE; i++)
      {
         KwUlRbCb* ulRbCb = ueCb->drbCb[i]; 
         if(ulRbCb != NULLP)
         {
            dumpRLCUlRbInformation(ulRbCb);
         }
      }
   }
}


/*
 * kwUtlFreeUlRbCb() function is split into two functions 
 *  -  kwAmmFreeUlRbCb() ---> gp_amm_ul.c 
 *  -  kwUmmFreeUlRbCb() ---> gp_umm_ul.c
 * and placed in respective files mentioned above
 */



/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS

/**
 *
 * @brief 
 *    Handler for initialisation of measurement 
 * 
 * @param[in]  gCb    - RLC instance Control Block 
 *
 * @return  S16
 *      -# ROK 
 */
S16 kwUtlL2MeasUlInit(KwCb *gCb)
{
   U16             cntr;

   gCb->u.ulCb->kwL2Cb.kwNumMeas=0;
   for(cntr = 0; cntr < LKW_MAX_L2MEAS; cntr++)
   {
      cmMemset((U8 *)&(gCb->u.ulCb->kwL2Cb.kwL2EvtCb[cntr]), 0, sizeof(KwL2MeasEvtCb));
   }
   gCb->u.ulCb->kwL2Cb.kwL2EvtCb[KW_L2MEAS_UL_IP].measCb.measType = LKW_L2MEAS_UL_IP;
   RETVALUE(ROK);
}
/**
 *
 * @brief
 *
 *        Handler to calculate the Ul Ip throughput for a LCH
 *
 * @b Description:
 *
 *
 * @param[in] rbCb         RB control block
 * @param[in] pdu          Pdu of LCH
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PUBLIC  Void kwUtlCalUlIpThrPutIncTTI
(
KwCb                  *gCb,
KwUlRbCb              *rbCb,
U32                   ttiCnt
)
#else
PUBLIC Void kwUtlCalUlIpThrPutIncTTI(gCb, rbCb, ttiCnt)
KwCb                  *gCb;
KwUlRbCb              *rbCb;
U32                   ttiCnt;
#endif
{
   VOLATILE U32     startTime = 0;
   TRC2(kwUtlCalUlIpThrPutIncTTI)

      /*starting Task*/
      SStartTask(&startTime, PID_RLC_IP_TPT_INCTTI);
#ifndef ALIGN_64BIT
   RLOG_ARG4(L_UNUSED, DBG_RBID,rbCb->rlcId.rbId,"Log for ul ip throughput:"
         "RB_MeasOn:%d ttiCnt :%ld UEID:%d CELLID:%d", 
         rbCb->rbL2Cb.measOn,ttiCnt,
         rbCb->rlcId.ueId,
         rbCb->rlcId.cellId);
#else
   RLOG_ARG4(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, "Log for ul ip throughput:"
         "RB_MeasOn:%d ttiCnt :%d UEID:%d CELLID:%d", 
         rbCb->rbL2Cb.measOn,ttiCnt,
         rbCb->rlcId.ueId,
         rbCb->rlcId.cellId);
#endif

   /*Check if UL IP throughput measurement is ON for this RB or not*/
   if(KW_MEAS_IS_UL_IP_MEAS_ON_FOR_RB(gCb,rbCb))              
   {
      if (TRUE  == rbCb->ueCb->isUlBurstActive)
      {
         if (ttiCnt < rbCb->l2MeasIpThruput.prevTtiCnt)
         {
            /*Removed Error Print*/
         }
         if (rbCb->l2MeasIpThruput.prevTtiCnt != 0)
         {
            rbCb->rbL2Cb.l2Sts[KW_L2MEAS_UL_IP]->ulIpThruput.timeSummation += 
               (ttiCnt - rbCb->l2MeasIpThruput.prevTtiCnt);
         }
         else
         {
            rbCb->ueCb->firstPacketTTI = ttiCnt;
         }
         rbCb->l2MeasIpThruput.prevTtiCnt = ttiCnt;
      }
      else
      {
         rbCb->l2MeasIpThruput.prevTtiCnt = 0;
      }
   }   

   /*stopping Task*/
   SStopTask(startTime, PID_RLC_IP_TPT_INCTTI);
} /* kwUtlCalUlIpThrPutIncTTI */


/**
 *
 * @brief
 *
 *        Handler to calculate the Ul Ip throughput for a LCH
 *
 * @b Description:
 *
 *
 * @param[in] rbCb         RB control block
 * @param[in] pdu          Pdu of LCH
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PUBLIC  Void kwUtlCalUlIpThrPut
(
KwCb                  *gCb,
KwUlRbCb              *rbCb,
Buffer                *pdu,
U32                   ttiCnt
)
#else
PUBLIC Void kwUtlCalUlIpThrPut(gCb, rbCb, pdu, ttiCnt)
   KwCb                  *gCb;
   KwUlRbCb              *rbCb;
   Buffer                *pdu;
   U32                   ttiCnt;
#endif
{
   MsgLen        rlcSduSz = 0;  /*Holds length of Rlc Sdu*/
   VOLATILE U32     startTime = 0;
   TRC2(kwUtlCalUlIpThrPut)


   /*starting Task*/
   SStartTask(&startTime, PID_RLC_IP_TPT_INCVOL);

   /*Check if UL IP throughput measurement is ON for this RB or not*/
   if(KW_MEAS_IS_UL_IP_MEAS_ON_FOR_RB(gCb, rbCb) &&              
         (TRUE  == rbCb->ueCb->isUlBurstActive) &&
         (rbCb->ueCb->firstPacketTTI) &&
         (ttiCnt != rbCb->ueCb->firstPacketTTI))
   {
      SFndLenMsg(pdu, &rlcSduSz);

      rbCb->rbL2Cb.l2Sts[KW_L2MEAS_UL_IP]->ulIpThruput.volSummation += rlcSduSz;

   }
   /*stopping Task*/
   SStopTask(startTime, PID_RLC_IP_TPT_INCVOL);
} /* kwUtlCalUlIpThrPut */


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
PUBLIC S16 kwUtlHdlL2TmrExp
(
KwCb          *gCb,
KwL2MeasEvtCb *measEvtCb
)
#else
PUBLIC S16 kwUtlHdlL2TmrExp(measEvtCb)
KwCb          *gCb;
KwL2MeasEvtCb *measEvtCb;
#endif
{
   TRC3(kwUtlHdlL2TmrExp)

#ifdef LTE_L2_MEAS_RLC
   U16             qciIdx;
   KwL2MeasCb     *measCb;
   
   /* Clean up the RB data structures */
   if((measEvtCb->measCb.measType & LKW_L2MEAS_ACT_UE) &&
      (measEvtCb->measCb.val.nonIpThMeas.numSamples))
   {
      measCb = &measEvtCb->measCb;

      for(qciIdx = 0; qciIdx < measCb->val.nonIpThMeas.numQci;qciIdx++)
      {
         measCb->val.nonIpThMeas.measData[measCb->val.nonIpThMeas.qci[qciIdx]].actUe.numActvUe   +=   
                     kwCb.kwL2Cb.numActUe[measCb->val.nonIpThMeas.qci[qciIdx]];
         measCb->val.nonIpThMeas.measData[measCb->val.nonIpThMeas.qci[qciIdx]].actUe.sampOc++;
      }
      measEvtCb->val.nonIpThMeas.measCb.numSamples--;
      kwStartTmr(gCb, (PTR)measEvtCb, KW_EVT_L2_TMR); 
      RETVALUE(ROK);
   }
#endif

   kwUtlSndUlL2MeasCfm(gCb, measEvtCb);

   RETVALUE(ROK);
} /* kwUtlHdlL2TmrExp */
/**
 *
 * @brief Handler for Sending L2 Measurement confirm.
 *
 *
 * @b Description
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
PUBLIC S16 kwUtlSndUlL2MeasCfm
(
KwCb                  *gCb,
KwL2MeasEvtCb         *measEvtCb
)
#else
PUBLIC S16 kwUtlSndUlL2MeasCfm(gCb, measEvtCb)
KwCb                  *gCb;
KwL2MeasEvtCb         *measEvtCb;
#endif
{
   U32                     qciIdx;
   KwL2MeasCb              *measCb;
   KwL2MeasCfmEvt          measCfmEvt;

   U64                     ulDataVol;
   U64                     ulTime;
   U16                     cntr;
   /* Discard new changes starts */
   U8                      qci = 0;
   U32                     cfmIdx =0;
   /* Discard new changes ends */

   TRC3(kwUtlSndUlL2MeasCfm)

   /* kw006.201 ccpu00120058 emoved 64 bit compilation warning */
#ifndef ALIGN_64BIT
   RLOG1(L_DEBUG,"kwUtlSndUlL2MeasCfm(transId(%ld))", measEvtCb->transId);
#else
   RLOG1(L_DEBUG,"kwUtlSndUlL2MeasCfm(transId(%d))", measEvtCb->transId);
#endif

   /* Clean up the RB data structures */
   measCb = &measEvtCb->measCb;
   
   cmMemset((U8*)&measCfmEvt, 0, sizeof(KwL2MeasCfmEvt));
   measCfmEvt.transId = measEvtCb->transId;

   measCfmEvt.measType = measCb->measType;
   measCfmEvt.status.status = LCM_PRIM_OK;
   measCfmEvt.status.reason = LCM_REASON_NOT_APPL;
   
   if( measCb->measType & LKW_L2MEAS_UL_IP)
   {
      KwL2MeasCbUeMeasInfo *pUeInfoLstCb  = measCb->val.ipThMeas.ueInfoLst;
      KwL2MeasCfmUeInfoLst *pUeInfoLstCfm = measCfmEvt.val.ipThMeas.ueInfoLst;
      for(cntr = 0;(cntr < measCb->val.ipThMeas.numUes) && (cntr < gCb->genCfg.maxUe);cntr++)        
      {
         pUeInfoLstCfm[cfmIdx].numCfm = 0;
         if (pUeInfoLstCb[cntr].isValid == TRUE)
         {
            pUeInfoLstCfm[cfmIdx].ueId = pUeInfoLstCb[cntr].ueId;
            pUeInfoLstCfm[cfmIdx].cellId = pUeInfoLstCb[cntr].cellId;

            for(qciIdx = 0; qciIdx < pUeInfoLstCb[cntr].numQci; qciIdx++)
            {
               qci = pUeInfoLstCb[cntr].qci[qciIdx];
               pUeInfoLstCfm[cfmIdx].measCfm[pUeInfoLstCfm[cfmIdx].numCfm].qci = qci;

               if(measCb->measType & LKW_L2MEAS_UL_IP)
               {
                  ulDataVol = pUeInfoLstCb[cntr].measData[qci].ulIpThruput.volSummation;
                  ulTime = pUeInfoLstCb[cntr].measData[qci].ulIpThruput.timeSummation;
                  if(0 == ulTime)
                  {
                     pUeInfoLstCfm[cfmIdx].measCfm[pUeInfoLstCfm[cfmIdx].numCfm].val.ipThrput.ulIpThPut = 0;
                  }
                  else
                  {
                     pUeInfoLstCfm[cfmIdx].measCfm[pUeInfoLstCfm[cfmIdx].numCfm].val.ipThrput.ulIpThPut = (ulDataVol / ulTime);

                     /* Converting it to kbps */
                     pUeInfoLstCfm[cfmIdx].measCfm[pUeInfoLstCfm[cfmIdx].numCfm].val.ipThrput.ulIpThPut *= 8;

                  }
                  /* Reset the values after reporting to Application */
                  pUeInfoLstCb[cntr].measData[qci].ulIpThruput.volSummation = 0;
                  pUeInfoLstCb[cntr].measData[qci].ulIpThruput.timeSummation = 0;
               }
               pUeInfoLstCfm[cfmIdx].numCfm++;
            }
            cfmIdx++;
         }
      }
      measCfmEvt.val.ipThMeas.numUes = cfmIdx; 
   }
#if 0   /*KW_FIX */
   if(measEvtCb->l2TmrCfg.enb == TRUE)
   {
      KwMiLkwL2MeasStopReq(NULLP,measEvtCb->measCb.measType);
   } 
#endif   
   KwMiLkwL2MeasCfm(&gCb->genCfg.lmPst, &measCfmEvt);
   RETVALUE(ROK);
} /* kwUtlSndUlL2MeasCfm */
/**
 *
 * @brief Handler for Sending Negative confirm .
 *
 *
  @b Description
 *        This function is called when the l2 measurement cannot be started
 *        This function sends  negative confirm for all the requests
 *
 * @param[in] gCb - RLC instance control block
 * @param[in] measReqEvt    Measurement Req Structure
 * @param[in] measCfmEvt  Confirmation to be sent to layer manager
 *
 *
 *  @return  S16
 *      -# ROK
 */

#ifdef ANSI
PUBLIC S16 kwUtlSndUlL2MeasNCfm
(
KwCb           *gCb,
KwL2MeasReqEvt *measReqEvt,
KwL2MeasCfmEvt *measCfmEvt
)
#else
PUBLIC S16 kwUtlSndUlL2MeasNCfm(gCb, measReqEvt, measCfmEvt)
KwCb           *gCb;
KwL2MeasReqEvt *measReqEvt;
KwL2MeasCfmEvt *measCfmEvt;
#endif
{
   TRC3(kwUtlSndUlL2MeasNCfm)

   KwMiLkwL2MeasCfm(&gCb->genCfg.lmPst, measCfmEvt);
   RETVALUE(ROK);
} /* kwUtlSndL2MeasNCfm */

#ifdef LTE_L2_MEAS_RLC
/**
 * @brief  Validates the measurement request parameters. 
 *
 * @details
 *
 *     Function :kwUtlValidateL2Meas 
 *
 *  @param[in]  measReqEvt L2 measurement request received from layer manager.
 *  @param[out] measCfmEvt L2 measurement confirm to be prepared.
 *  @param[out] lChId      List of LCh for the given Ue corresponding to QCIs
                           given in measurement request.
 *  @param[out] numLCh     Number of LCh in array lChId.
 **/

#ifdef ANSI
PUBLIC S16 kwUtlValidateL2Meas
(
KwL2MeasReqEvt *measReqEvt,
KwL2MeasCfmEvt *measCfmEvt,
CmLteLcId      *lChId,
U8             *numLCh
)
#else
PUBLIC S16 kwUtlValidateL2Meas(measReqEvt, measCfmEvt, lChId, numLCh)
KwL2MeasReqEvt *measReqEvt;
KwL2MeasCfmEvt *measCfmEvt;
CmLteLcId      *lChId;
U8             *numLCh;
#endif
{
   U8      measType;
   S16     ret;
   U8      qciIdx;
   U8      qci;
   U8      idx;
   U8      *qciVal;
   U8      numQci;
   KwRbCb  *rbCb;

   KwUeCb     *ueCb;
   RbCb       **rbCbLst;
   U8         rbIdx;
   U8         lsbNibble = 0;
   U8         msbNibble = 0;
   U8         numFaild = 0;


   TRC3(kwUtlValidateL2Meas)
   
   idx = 0;
   rbCb = NULLP;
   ret = ROK;
   measType = measReqEvt->measReq.measType;
   /* Check for the range of measType */
   /* LKW_L2MEAS_DL_IP+ LKW_L2MEAS_UL_IP = 0x0030*/
   if((measType == 0x00) ||
       measType > 0x30)
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_MEASTYPE;
      RETVALUE(RFAILED);
   }
   /*User can either request for Active UE,*
    *Dl delay, Dl discard, Uu Loss OR Dl ip throughput, Ul ip throughput. */
   lsbNibble = measType & 0x0F;
   msbNibble = measType & 0xF0;

   if( (lsbNibble != 0) && (msbNibble != 0) )
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_MEASTYPE;
      RETVALUE(RFAILED);
   }

   /* Check for total maximum number of Measurement Control Block */
   if(kwCb.kwL2Cb.kwNumMeas >= LKW_MAX_L2MEAS )
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_EXCEED_NUMMEAS;
      RETVALUE(RFAILED);
   }

   /* Check that number of samples should be a non-zero value */
   if(((measType & LKW_L2MEAS_ACT_UE) &&
      (measReqEvt->measReq.val.nonIpThMeas.numSamples == 0)))
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_ZERO_NUMSAM;
      RETVALUE(RFAILED);
   }
   /* Check that measurement period  should be completely divisible *
    * number of sample.                                             */
   if(((measType & LKW_L2MEAS_ACT_UE) &&
      ((measReqEvt->measPeriod % 
        measReqEvt->measReq.val.nonIpThMeas.numSamples) != 0)))
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_NUMSAM;
      RETVALUE(RFAILED);
   }
   {
      numQci = measReqEvt->measReq.val.nonIpThMeas.numQci;
      qciVal = measReqEvt->measReq.val.nonIpThMeas.qci;
   }
   /* Check whether qci is configured or not */
   for(qciIdx = 0; qciIdx < numQci; qciIdx++)
   {
      qci = qciVal[qciIdx];
      ret = cmHashListFind(&(kwCb.kwL2Cb.qciHlCp), 
            (U8 *)&qci, (U16)sizeof(qci), 0, (PTR *)&rbCb);
      if(ret != ROK)
      {
         measCfmEvt->val.nonIpThMeas.measCfm[measCfmEvt->val.nonIpThMeas.numCfm].qci = qci;
         measCfmEvt->val.nonIpThMeas.numCfm++;

      }
   }
   if(measCfmEvt->val.nonIpThMeas.numCfm > 0)
   {
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_QCI;
      measCfmEvt->measType = measType;
      measCfmEvt->transId = measReqEvt->transId;
      RETVALUE(RFAILED);
   }
   {
      for(qciIdx = 0; qciIdx < numQci; qciIdx++)
      {
         if(kwCb.kwL2Cb.measOn[qci] & measReqEvt->measReq.measType)
         {
            /* measurement is already ongoing */
            measCfmEvt->status.status = LCM_PRIM_NOK;
            measCfmEvt->status.reason = LKW_CAUSE_MEAS_ALREADY_ENA;
            measCfmEvt->val.nonIpThMeas.measCfm[measCfmEvt->val.nonIpThMeas.numCfm].qci = qci;
            measCfmEvt->measType = measType;
            measCfmEvt->val.nonIpThMeas.numCfm++;
         }
      }
   }
   if(measCfmEvt->val.nonIpThMeas.numCfm > 0)
   {
      measCfmEvt->transId = measReqEvt->transId;
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* kwUtlValidateL2Meas */
#endif

#ifdef ANSI
PUBLIC S16 kwUtlValidateIpThL2Meas
(
KwL2MeasReqEvt *measReqEvt,
KwL2MeasCfmEvt *measCfmEvt
)
#else
PUBLIC S16 kwUtlValidateIpThL2Meas(measReqEvt, measCfmEvt)
KwL2MeasReqEvt *measReqEvt;
KwL2MeasCfmEvt *measCfmEvt;
#endif
{
   U8      measType;
#if 0 /* L2 MEAS AGHOSH */  
   KwUeCb     *ueCb;
#endif /* L2 MEAS AGHOSH */
   U8         lsbNibble = 0;
   U8         msbNibble = 0;
#if 0 /* L2 MEAS AGHOSH */   
   U8         numFaild = 0;
   U16        cntr;
#endif   

   TRC3(kwUtlValidateIpThL2Meas)
   
   measType = measReqEvt->measReq.measType;
   /* Check for the range of measType */
   /* LKW_L2MEAS_DL_IP+ LKW_L2MEAS_UL_IP = 0x0030*/
   if((measType == 0x00) ||
       measType > 0x30)
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_MEASTYPE;
      RETVALUE(RFAILED);
   }
   /*User can either request for Active UE,*
    *Dl delay, Dl discard, Uu Loss OR Dl ip throughput, Ul ip throughput. */
   lsbNibble = measType & 0x0F;
   msbNibble = measType & 0xF0;

   if( (lsbNibble != 0) && (msbNibble != 0) )
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_INVALID_MEASTYPE;
      RETVALUE(RFAILED);
   }
#if 0 /* AGHOSH not required */
   /* AGHOSH TODO -- this check is required for max measurements */
   /* Check for total maximum number of Measurement Control Block */
   if(kwCb.kwL2Cb.kwNumMeas >= LKW_MAX_L2MEAS )
   {
      measCfmEvt->transId = measReqEvt->transId;
      measCfmEvt->measType = measType;
      measCfmEvt->status.status = LCM_PRIM_NOK;
      measCfmEvt->status.reason = LKW_CAUSE_EXCEED_NUMMEAS;
      RETVALUE(RFAILED);
   }
   for(cntr = 0;cntr< measReqEvt->measReq.val.ipThMeas.numUes;cntr++)
    {
      /* Check for number of QCIs in the measurement request */
      if((measReqEvt->measReq.val.ipThMeas.ueInfoLst[cntr].numQci > LKW_MAX_QCI) /*||
         (measReqEvt->measReq.val.ueInfoLst.ipThMeas[cntr].numQci == 0)*/)
      {
         measCfmEvt->val.ipThMeas.ueInfoLst[cntr].numCfm++; 
         measCfmEvt->transId = measReqEvt->transId;
         measCfmEvt->measType = measType;
         measCfmEvt->status.status = LCM_PRIM_NOK;
         measCfmEvt->status.reason = LKW_CAUSE_INVALID_NUMQCI;
         measCfmEvt->val.ipThMeas.ueInfoLst[numFaild].ueId = measReqEvt->measReq.val.ipThMeas.ueInfoLst[cntr].ueId;
         measCfmEvt->val.ipThMeas.ueInfoLst[numFaild].cellId = measReqEvt->measReq.val.ipThMeas.ueInfoLst[cntr].cellId;
         numFaild++;
         measReqEvt->measReq.val.ipThMeas.ueInfoLst[cntr].ueId = 0;
         measCfmEvt->val.ipThMeas.numUes = numFaild;
      }
    }
    if( numFaild > 0)
    {          
       if(numFaild == measReqEvt->measReq.val.ipThMeas.numUes)
       {
         RETVALUE(RFAILED);
       }
       else
       {
          kwUtlSndL2MeasNCfm(measReqEvt, measCfmEvt);
          numFaild = 0;
       }
   }  
    for(cntr = 0;cntr< measReqEvt->measReq.val.ipThMeas.numUes;cntr++)
    {
      if(measReqEvt->measReq.val.ipThMeas.ueInfoLst[cntr].ueId == 0)
      {
         continue;
      }      
      /* Check whether UE is valid or not */
      if (kwDbmFetchUlUeCb(gCb, measReqEvt->measReq.val.ipThMeas.ueInfoLst[cntr].ueId, 
                         measReqEvt->measReq.val.ipThMeas.ueInfoLst[cntr].cellId, &ueCb) != ROK)
      {
#ifdef DEBUGP
         KWDBGP((KW_DBGMASK_DUT | KW_DBGMASK_ERROR), (_kwPBuf,
                  "kwUtlValidateL2Meas: kwDbmFetchUeCb Failed.\n"));
#endif
         measCfmEvt->val.ipThMeas.ueInfoLst[cntr].numCfm++;
         measCfmEvt->transId = measReqEvt->transId;
         measCfmEvt->measType = measType;
         measCfmEvt->status.status = LCM_PRIM_NOK;
         measCfmEvt->status.reason = LKW_CAUSE_INVALID_UE;
         measCfmEvt->val.ipThMeas.ueInfoLst[numFaild].ueId = measReqEvt->measReq.val.ipThMeas.ueInfoLst[cntr].ueId;
         measCfmEvt->val.ipThMeas.ueInfoLst[numFaild].cellId = measReqEvt->measReq.val.ipThMeas.ueInfoLst[cntr].cellId;
         numFaild++;
         measReqEvt->measReq.val.ipThMeas.ueInfoLst[cntr].ueId = 0;
         measCfmEvt->val.ipThMeas.numUes = numFaild;
         if(numFaild == measReqEvt->measReq.val.ipThMeas.numUes)
           {
             RETVALUE(RFAILED);
           }
          continue;
      }
    } /* for(qciIdx) ends */

    if( numFaild > 0 ) 
    { 
      if(numFaild == measReqEvt->measReq.val.ipThMeas.numUes)
      {
        RETVALUE(RFAILED);
     }
    else
    {
        kwUtlSndL2MeasNCfm(measReqEvt, measCfmEvt);
        numFaild = 0;
    }
   }
#endif    
   RETVALUE(ROK);
}/* kwUtlValidateL2Meas */

/**
 *
 * @brief Handler for resetting the RB data structures
 *
 *
 * @b Description
 *        This function resets the RB data structure after the expiry of 
 *        measurement timer.
 *
 *  @param[in] measCb    Measurement Control Block.
 *
 *
 *  @return  Void
 */
#ifdef ANSI

PUBLIC Void kwUtlResetUlL2MeasInKwRb
(
KwCb       *gCb,
KwL2MeasCb *measCb,
U8             measType
)
#else
PUBLIC Void kwUtlResetUlL2MeasInKwRb(measCb, measType)
KwCb       *gCb;
KwL2MeasCb *measCb;
U8             measType;
#endif
{
   U32           rbIdx;
   U32           ueIdx;
   U32           qciIdx;
   KwUlUeCb         *ueCb = NULL;



   if (measCb->measType & LKW_L2MEAS_UL_IP)
   {
      for(ueIdx = 0; ueIdx < measCb->val.ipThMeas.numUes; ueIdx++)
      {           
         if (measCb->val.ipThMeas.ueInfoLst[ueIdx].isValid == TRUE)
         {
            for (qciIdx =0; qciIdx < measCb->val.ipThMeas.ueInfoLst[ueIdx].numQci; qciIdx++)
            {
               if (measType & LKW_L2MEAS_UL_IP)
               {
                  measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[qciIdx].ulIpThruput.volSummation = 0;
                  measCb->val.ipThMeas.ueInfoLst[ueIdx].measData[qciIdx].ulIpThruput.timeSummation = 0;
               }
            }

            if(ROK  != kwDbmFetchUlUeCb(gCb, measCb->val.ipThMeas.ueInfoLst[ueIdx].ueId,
                     measCb->val.ipThMeas.ueInfoLst[ueIdx].cellId, &ueCb))
            {
               continue;
            }

            for (rbIdx = 0; rbIdx < KW_MAX_DRB_PER_UE; rbIdx++)
            {
               if (ueCb->drbCb[rbIdx])
               {
                  ueCb->drbCb[rbIdx]->rbL2Cb.measOn &= ~measType;
               }
            }
         }
      }
   }
} /* kwUtlResetUlL2MeasInKwRb */

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
PUBLIC Void kwUtlPlcMeasDatInL2Sts
(
KwL2Cntr       *measData, 
KwL2MeasRbCb   *rbL2Cb,
U8             measType
)
#else
PUBLIC Void kwUtlPlcMeasDatInL2Sts(measData, rbL2Cb, measType)
KwL2Cntr       *measData; 
KwL2MeasRbCb   *rbL2Cb;
U8             measType;
#endif
{
   TRC3(kwUtlPlcMeasDatInL2Sts)
   
   /* We should check the number of measType in the request. This can be done 
    * by looking at each bit in the measType. Also store the measData in the 
    * correct index of l2Sts in RbCb.
    * */

    if(measType & LKW_L2MEAS_ACT_UE)
    {
         rbL2Cb->l2Sts[KW_L2MEAS_ACT_UE] = measData;
    }
    if(measType & LKW_L2MEAS_UU_LOSS)
    {
         rbL2Cb->l2Sts[KW_L2MEAS_UU_LOSS] = measData;
    }
    if(measType & LKW_L2MEAS_DL_IP )
    {
         rbL2Cb->l2Sts[KW_L2MEAS_DL_IP] = measData;
    }
    if(measType & LKW_L2MEAS_UL_IP)
    {
         rbL2Cb->l2Sts[KW_L2MEAS_UL_IP] = measData;
    }
    if(measType & LKW_L2MEAS_DL_DISC)
    {
         rbL2Cb->l2Sts[KW_L2MEAS_DL_DISC] = measData;
    }
    if(measType & LKW_L2MEAS_DL_DELAY)
    {
         rbL2Cb->l2Sts[KW_L2MEAS_DL_DELAY] = measData;
    }
}/* End of kwUtlPlcMeasDatInL2Sts */
#endif /* LTE_L2_MEAS */



/********************************************************************30**
         End of file:     gp_utl_ul.c@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:55:23 2014
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
/main/2      ---       nm   1. LTERLC Release 2.1
/main/3      kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/5      kw003.201 vp  1.  Fixes for re-segmentation, segement offset
                               value and optimization 
/main/6      kw004.201 ap  1.  Used SSI function to initialize the variable.
                           2.  Added fn call cmHashListDelete for deleting
                       rd  3.  Code added for DL and UL IP throughput 
                               measurement under flag LTE_L2_MEAS.
                              the entry from tbHlCp.  
                           4.  Added code for DL/UL IP Meas in fn 
                               kwUtlSndL2MeasCfm().
/main/7      kw005.201 ap  1. Added support for L2 Measurement.                               
*********************************************************************91*/

