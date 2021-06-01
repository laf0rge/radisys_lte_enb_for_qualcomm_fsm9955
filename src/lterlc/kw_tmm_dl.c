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
  
     Desc:     Source code for RLC Transparent mode assembly and
               reassembly.This file contains following functions
                
                  --kwTmmQSdu
                  --kwTmmSndToLi
                  --kwTmmRcvFrmLi
                  --kwTmmReEstablish 

     File:     gp_tmm_dl.c

     Sid:      gp_tmm_dl.c@@/main/TeNB_Main_BR/tenb_br_tdd_e1.2_4.0.0_cr/1 - Thu Nov 20 22:41:31 2014

     Prg:      vp 

**********************************************************************/
static const char* RLOG_MODULE_NAME="TMM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=200;
/** 
 * @file gp_tmm_dl.c
 * @brief RLC Transparent Mode module
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
#include "kw_err.h"            /* RLC defines */
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

#define KW_MODULE (KW_DBGMASK_TM | KW_DBGMASK_DL)

PRIVATE Void kwTmmSndStaRsp ARGS((KwCb *gCb, KwDlRbCb *rbCb, 
                                 MsgLen bo, KwuDatReqInfo *datReqInfo));
extern U32 rgMacGT ;  
/** @addtogroup tmmode */
/*@{*/

/**
 * @brief 
 *    Handler to queue the SDU in the SDU queue and update BO and report it to
 *    the lower layer.
 *
 * @details 
 *    This function is used to queue the received SDU in the SDU queue 
 *    maintained in the radio bearer control block. After queuing the SDU, BO 
 *    is updated and is reported to the lower layer. 
 *            
 *  @param[in] rbCb         RB control block. 
 *  @param[in] datReqInfo   Data Request Information.
 *  @param[in] mBuf         SDU Buffer.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PUBLIC Void kwTmmQSdu
(
KwCb            *gCb,
KwDlRbCb        *rbCb,      
KwuDatReqInfo   *datReqInfo, 
Buffer          *mBuf       
)
#else
PUBLIC Void kwTmmQSdu(gCb,rbCb,datReqInfo,mBuf)
KwCb            *gCb;
KwDlRbCb        *rbCb;       
KwuDatReqInfo   *datReqInfo;  
Buffer          *mBuf;         
#endif
{
   KwSdu   *sdu;              
 
   TRC2(kwTmmQSdu) 


   KW_ALLOC(gCb,sdu,sizeof(KwSdu));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( sdu == NULLP )
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
            "Memory Allocation failed UEID:%d CELLID:%d",   
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);   
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
#ifdef CCPU_OPT   
   if ( rbCb->lch.lChType == CM_LTE_LCH_BCCH || 
        rbCb->lch.lChType == CM_LTE_LCH_PCCH )
   {
      sdu->mode.tm.sfn = datReqInfo->tm.tmg.sfn;
      sdu->mode.tm.subframe = datReqInfo->tm.tmg.subframe;
   }
   else
   {
      sdu->mode.tm.rnti = datReqInfo->tm.rnti;
   }
#endif
   sdu->arrTime = rgMacGT;
   SFndLenMsg(mBuf,&sdu->sduSz); 
   sdu->mBuf = mBuf;
   
   cmLListAdd2Tail(&(rbCb->m.tm.sduQ), &(sdu->lstEnt));  
   sdu->lstEnt.node = (PTR)sdu; 

   kwTmmSndStaRsp(gCb, rbCb, sdu->sduSz, datReqInfo); 
   RETVOID;
}

/**
* @brief 
*    Handler to form a pdu and send it to the lower layer.
* 
* @details 
*    This function forms one pdu from the first SDU in the SDU queue and sends 
*    it to the lower layer.
*             
* @param[in] gCb     RLC Instance Control Block
* @param[in] rbCb    RB control block. 
* @param[in] staInd  Status Indication of common logical channel 
*
* @return  S16
*    -# ROK 
*    -# RFAILED         
*/
#ifdef ANSI
PUBLIC Void kwTmmSndToLi
(
KwCb             *gCb,
KwDlRbCb         *rbCb,              
RguCStaIndInfo   *staInd
)
#else
PUBLIC Void kwTmmSndToLi(gCb,rbCb, staInd)
KwCb             *gCb;
KwDlRbCb         *rbCb;             
RguCStaIndInfo   *staInd;
#endif
{
   CmLList          *node;          /* Current Link List Node */
   KwSdu            *sdu;           /* SDU */
   RguCDatReqInfo   *cDatReqInfo;   /* Data Request Information */
   S16   timeDiff = 0;
   Ticks curTime  = 0;

   TRC2(kwTmmSndToLi)


   CM_LLIST_FIRST_NODE(&(rbCb->m.tm.sduQ), 
                       node);

   /* (Sfn,subframe) at which the message should be transmitted is 
    * validated with alloted (sfn,subframe)in the MAC layer */
   while (node != NULLP)
   {
      sdu = (KwSdu *)(node->node);
      if ( rbCb->lch.lChType == CM_LTE_LCH_BCCH ||
            rbCb->lch.lChType == CM_LTE_LCH_PCCH )
      {
         U16 sfn, subframe;
         /* MS_FIX: syed sfn is of 10 bytes rather than 8 */
         sfn = (staInd->transId >> 8) & 0x3FF;
         subframe = staInd->transId & 0xFF;
         /* Table
          * tm.subframe - current subframe 
          * 0,sfn        7,sfn-1
          * 4,sfn        1,sfn
          * 5,sfn        2,sfn
          * 9,sfn        6,sfn
          */
         /* MS_FIX: syed Incorrect sfn determination. 
          * Take care of SFN wraparound. TODO: It is better for RLC
          * not to be aware of SCH DELTAs. So we should look for 
          * sending actual transmission time to RLC. */
         if ((subframe + TFU_DELTA) >= 10)
         {
            sfn = (sfn + 1)%1024;
         }

         if ((sdu->mode.tm.sfn != sfn) ||
               (sdu->mode.tm.subframe != ((subframe+TFU_DELTA)%10)))
         {
            node = node->next;
            RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                  "Releasing SDU of RNTI = %d for RNTI = %d UEID:%d CELLID:%d",
                  sdu->mode.tm.rnti, 
                  staInd->rnti,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);   
            RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                  "sfn %d subframe %d  UEID:%d CELLID:%d",
                  sfn, 
                  subframe,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);   
            cmLListDelFrm(&(rbCb->m.tm.sduQ), &sdu->lstEnt);
            KW_FREE_BUF(sdu->mBuf);
            KW_FREE(gCb, sdu, sizeof(KwSdu));
         }
         else
         {
            break;
         }
      }
      else
      {
         curTime = rgMacGT;
         if (curTime < sdu->arrTime)
         {
            timeDiff = (10240 - sdu->arrTime) + curTime;
         }
         else
         {
            timeDiff = curTime - sdu->arrTime;
         }
         RLOG_ARG4(L_DEBUG, DBG_RBID,rbCb->rlcId.rbId,
               "TMM: TmSdu Sta Indication received for Rnti %d Sdu Rnti %d "
               " UEID:%d CELLID:%d", 
               staInd->rnti, 
               sdu->mode.tm.rnti,
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);   
         RLOG_ARG4(L_DEBUG, DBG_RBID,rbCb->rlcId.rbId,
               "TMM: TmSdu Sta Indication received : timeDiff %d SduQCnt %lu"
               " UEID:%d CELLID:%d", 
               timeDiff, 
               rbCb->m.tm.sduQ.count,
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);   
         if (timeDiff > 40)
         {
            /* Memory leak needs to be fixed */
            node = node->next;
            RLOG_ARG3(L_DEBUG, DBG_RBID,rbCb->rlcId.rbId,
                  " timeDiff greater than 40, so deleting the Sdu %u "
                  " UEID:%d CELLID:%d", 
                  sdu->mode.tm.rnti,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);   
            cmLListDelFrm(&(rbCb->m.tm.sduQ), &sdu->lstEnt);
            KW_FREE_BUF(sdu->mBuf);
            KW_FREE(gCb, sdu, sizeof(KwSdu));
            continue;
         }

         if (sdu->mode.tm.rnti != staInd->rnti)
         {
            /* Memory leak needs to be fixed */
            node = node->next;
            RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                  "TMM: Searching for Rnti %d Skipping Sdu for Rnti %d"
                  " UEID:%d CELLID:%d", 
                  staInd->rnti,
                  sdu->mode.tm.rnti, 
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);   
            RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                  " timeDiff %d sdu->arrTime %d"
                  " UEID:%d CELLID:%d", 
                  timeDiff, 
                  sdu->arrTime,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);   
            RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                  "curTime %d SduQCnt %lu and continuing"
                  " UEID:%d CELLID:%d", 
                   curTime, 
                   rbCb->m.tm.sduQ.count,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);   
            continue;
         }
         else
         {
            RLOG_ARG3(L_DEBUG, DBG_RBID,rbCb->rlcId.rbId,
                  "TMM: TmSdu found %u UEID:%d CELLID:%d",
                  sdu->mode.tm.rnti,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);   
            break;
         }
      }

   }
   if ( node == NULLP )
   {
      RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
               "SDU not found TM Queue is empty UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);   
      RETVOID;
   }
   sdu = (KwSdu *)node->node;

    KW_ALLOC_SHRABL_BUF(gCb->u.dlCb->rguDlSap.pst.region,
                        gCb->u.dlCb->rguDlSap.pst.pool,
                        cDatReqInfo,(Size)sizeof(RguCDatReqInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( cDatReqInfo == NULLP )
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
            "Memory Allocation failed UEID:%d CELLID:%d",   
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);   
      RETVOID; 
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
#ifdef CCPU_OPT
   
   if ( rbCb->lch.lChType == CM_LTE_LCH_BCCH ||  
        rbCb->lch.lChType == CM_LTE_LCH_PCCH )
   {
      cDatReqInfo->u.timeToTx.sfn = sdu->mode.tm.sfn;
      cDatReqInfo->u.timeToTx.subframe = sdu->mode.tm.subframe; 
   }
   else
   {
      cDatReqInfo->u.rnti = sdu->mode.tm.rnti;
   }
#endif 
   cDatReqInfo->pdu = sdu->mBuf; 
   cDatReqInfo->transId = rbCb->transId;
   cDatReqInfo->cellId  = rbCb->rlcId.cellId;
   cDatReqInfo->lcId   = rbCb->lch.lChId; 
   cDatReqInfo->lcType = rbCb->lch.lChType; 

   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->genSts.bytesSent += sdu->sduSz;
   gCb->genSts.pdusSent++;

   kwUtlIncrementKwuStsSduTx(gCb->u.dlCb->kwuDlSap + rbCb->kwuSapId);   

   /* remove SDU from queue */ 
   sdu->mBuf = NULLP;
   cmLListDelFrm(&(rbCb->m.tm.sduQ),
                 &sdu->lstEnt); 
   KW_FREE(gCb,sdu, sizeof(KwSdu));

   /* If trace flag is enabled send the trace indication */
   if(gCb->init.trc == TRUE)
   {
      /* Populate the trace params */
      kwLmmSendTrc(gCb,EVTRGUCDATREQ, NULLP);
   }
   KwLiRguCDatReq (&(gCb->u.dlCb->rguDlSap.pst), 
                   gCb->u.dlCb->rguDlSap.spId, 
                   cDatReqInfo);
   
   RETVOID;
}

/**
 *
 * @brief 
 *    Handler to process the re-establishment request received from the upper 
 *    layer. 
 *
 * @details
 *    This function empties the SDU queue for the RB in the downlink.
 *            
 * @param[in] gCb   RLC Instance Control Block 
 * @param[in] rbCb  RB control block. 
 *
 * @return  S16
 *    -# ROK 
 */
#ifdef ANSI
PUBLIC Void kwDlTmmReEstablish
(
KwCb       *gCb,
KwDlRbCb   *rbCb    
)
#else
PUBLIC Void kwDlTmmReEstablish(gCb,rbCb)
KwCb       *gCb;
KwDlRbCb   *rbCb;     
#endif
{
   TRC2(kwDlTmmReEstablish)


#ifdef LTE_L2_MEAS_RLC
   kwUtlEmptySduQ(gCb, rbCb, &rbCb->m.tm.sduQ);
#else
   kwUtlEmptySduQ(gCb,&rbCb->m.tm.sduQ);
#endif
   
   RETVOID;
}
/**
 *
 * @brief 
 *    Handler to send Status Response to the lower layer.
 *
 * @details
 *    This function is used to the BO to the lower layer after receiving a data
 *    request from the upper layer. 
 *            
 * @param[in] gCb          RLC Instance Control Block 
 * @param[in] rbCb         RB control block. 
 * @param[in] bo           Buffer Occupancy 
 * @param[in] datReqInfo   Data Request Information.
 *
 * @return  S16
 *    -# ROK 
 *    -# RFAILED 
 */

#ifdef ANSI
PRIVATE Void kwTmmSndStaRsp
(
KwCb            *gCb,
KwDlRbCb        *rbCb,                 
MsgLen          bo,                    
KwuDatReqInfo   *datReqInfo         
)
#else
PRIVATE Void kwTmmSndStaRsp(rbCb,bo,datReqInfo)
KwCb            *gCb;
KwDlRbCb        *rbCb;               
MsgLen          bo;                
KwuDatReqInfo   *datReqInfo;   
#endif
{
   RguCStaRspInfo   *staRspInfo;   /* Status Response Information */
   KwRguSapCb       *rguSap;       /* SAP Information */

   TRC3(kwTmmSndStaRsp)


   rguSap = &(gCb->u.dlCb->rguDlSap);

   KW_ALLOC_SHRABL_BUF(gCb->u.dlCb->rguDlSap.pst.region,
                       gCb->u.dlCb->rguDlSap.pst.pool,
                       staRspInfo,sizeof(RguCStaRspInfo));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( staRspInfo == NULLP )
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
            "Memory Allocation failed UEID:%d CELLID:%d",
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);   
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   staRspInfo->bo = bo;
   staRspInfo->cellId = rbCb->rlcId.cellId;
   staRspInfo->lcId   = rbCb->lch.lChId;
   staRspInfo->lcType = rbCb->lch.lChType;
#ifdef CCPU_OPT    
   if ( rbCb->lch.lChType == CM_LTE_LCH_BCCH ||
        rbCb->lch.lChType == CM_LTE_LCH_PCCH )
   {
      staRspInfo->u.timeToTx.sfn      = datReqInfo->tm.tmg.sfn;
      staRspInfo->u.timeToTx.subframe = datReqInfo->tm.tmg.subframe;
   }
   else if ( rbCb->lch.lChType == CM_LTE_LCH_CCCH )
   {
       staRspInfo->u.rnti = datReqInfo->tm.rnti;
   }
#endif

   /* If trace flag is enabled send the trace indication */
   if(gCb->init.trc == TRUE)
   {
      /* Populate the trace params */
      kwLmmSendTrc(gCb,EVTRGUCSTARSP, NULLP);
   }

   KwLiRguCStaRsp(&rguSap->pst,rguSap->spId,staRspInfo);

   RETVOID;
} 

#ifdef _cplusplus
}
#endif
/*@}*/
/********************************************************************30**
         End of file:     gp_tmm_dl.c@@/main/TeNB_Main_BR/tenb_br_tdd_e1.2_4.0.0_cr/1 - Thu Nov 20 22:41:31 2014
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
