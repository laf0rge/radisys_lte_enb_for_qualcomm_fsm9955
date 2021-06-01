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

     Name:     LTE-PDCP Layer 
  
     Type:     C file
  
     Desc:     Source code for PDCP Downlink module.
               This file contains following functions

                  --pjDlmProcessSdus
                  --pjDlmBldPdu
                  --pjDlmReEstSrb
                  --pjDlmReEstDrbUm
                  --pjDlmReEstDrbAm
                  --pjDlmProcessCfm
                  --pjDlmHndlStaRep
                  --pjDlmDiscSdu
                  --pjDlmDeliverPdu
                  --pjDlmReEstHoDrbAm
                  --pjDlmHndlDatFwdReq
                  --pjDlmProcSrb
                  --pjDlmProcDrb

     File:     gp_pj_dlm.c

     Sid:      eq_dlm.c@@/main/TeNB_Main_BR/14 - Mon Aug 11 16:42:47 2014

     Prg:      vp 

**********************************************************************/

static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_FILE_ID=241;
static int RLOG_MODULE_ID=1024;
/** @file gp_pj_dlm.c
@brief PDCP Downlink module
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
#include "cm_lte.h"             /* common LTE header file */
#include "cm_hash.h"            /* common hash module  file */
#include "cm_llist.h"           /* common list header file */
#include "cpj.h"                /* RRC layer */
#include "pju.h"                /* PDCP service user */
#include "lpj.h"                /* LPJ defines */
#include "pj_env.h"             /* RLC environment options */
#include "pj.h"                 /* RLC layer */
#include "pj_dl.h"
#include "pj_err.h"
#include "pj_ptsec.h"
#ifndef TENB_T2K3K_SPECIFIC_CHANGES 
void prc_trace_format_string(U32 group_mask, U16 level, const char *format, ...);
#endif


/* header/extern include files (.x) */
#include "gen.x"                /* general */
#include "ssi.x"                /* system services interface */
#include "cm_lib.x"             /* common library */
#include "cm5.x"                /* Timer Functions */
#include "cm_hash.x"            /* common hash module */
#include "cm_lte.x"             /* common LTE file */
#include "cm_llist.x"           /* common list header file */
#include "cpj.x"                /* RRC layer */
#include "pju.x"                /* PDCP service user */
#include "lpj.x"                /* LPJ defines */
#include "pj.x"                 /* RLC layer */
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_dl.x"
#include "pj_ul.x"

EXTERN U32 pjTotDlPckCntPerCell;
#ifdef TENB_AS_SECURITY 
EXTERN U8 isSecBatchMode;
#endif

#ifdef QCOM_STATS
PUBLIC U32 pjDlrate_pju;
#endif

#ifdef TENB_T2K3K_SPECIFIC_CHANGES
EXTERN Ticks SGetT2KTtiCount(Void);
#endif
/* kw005.201 Moved definition of PJ_ASYNC_WIN to kw.h file */
/** @addtogroup dldata */
/*@{*/
EXTERN S16 pjUtlChekTxEnqReq ARGS(( PjCb *gCb, PjDlRbCb *pjRbCb, PjTxEnt *txEnt));

PRIVATE S16 pjDlmBldPdu(PjCb *gCb,PjDlRbCb *pjRbCb,PjTxEnt *txEnt);
PRIVATE Void pjDlmDelTxEntUptoFms ARGS ((PjCb *gCb, PjDlRbCb *pjRbCb, U32 fms, PjuDatCfmInfo *datCfm));

/**
 *
 * @brief 
 *
 *        Handler to free all sdus present in datCfmQ till the First Missing
 *        Segment. 
 *
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] fms      First Missing Segment
 *  @param[in] datCfm   datCfm 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PRIVATE Void pjDlmDelTxEntUptoFms
(
PjCb            *gCb,
PjDlRbCb        *pjRbCb,
U32             fms,
PjuDatCfmInfo   *datCfm
)
#else
PRIVATE Void pjDlmDelTxEntUptoFms(gCb, pjRbCb, fms, datCfm)
PjCb            *gCb;
PjDlRbCb        *pjRbCb;
U32             fms;
PjuDatCfmInfo   *datCfm;
#endif
{
   PjTxEnt   *txEnt;
   U32       count;

   PJ_UPD_COUNT(pjRbCb,fms,pjRbCb->dlCb.txHfn, count);

   cmLListFirst(&pjRbCb->dlCb.txBuf.datCfmQ);
   while (cmLListCrnt(&pjRbCb->dlCb.txBuf.datCfmQ))
   {
     txEnt = (PjTxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->dlCb.txBuf.datCfmQ));
     if (txEnt != NULLP)
     {
        if (txEnt->count < count)
        { 
           if (pjRbCb->dlCb.cfmReqd)
           {
              /* As of now, DATCFM is sent only for the 1st 25 PDUs. Once we      */
              /* enable cfmReqd, we need to add code to send for all PDUs in loop */
              if(datCfm->numSdus < PJU_MAX_SDU_CFM) 
              {
                 datCfm->cfmSta[datCfm->numSdus].sduId  = txEnt->sduId;
                 datCfm->cfmSta[datCfm->numSdus].status = PJ_CFM_OK;
                 datCfm->numSdus++;
              }
           }
           pjDbmDelTxEnt(gCb, &(pjRbCb->dlCb.txBuf), txEnt->count);
        }
        else
        {
           /* fms reached break out of the loop */
           break;
        }
     } 
     cmLListNext(&pjRbCb->dlCb.txBuf.datCfmQ);
   }

   RETVOID;
}

/**
 *
 * @brief 
 *
 *        Handler to process the SDU received from upper layer, form a PDU
 *        and send the PDU to the lower layer.
 *
 * @b Description: 
 *
 *        1. This function places the SDU in the transmission buffer and performs
 *        Compression for DRB SDUs and Integrity Protection for SRB SDUs.  @n
 *        2. After compression/integrity protection, it performs ciphering and then 
 *        constructs the PDU and sends it to the lower layer. @n
 *        3. The first DL message of SRBs is not ciphered and is just integrity 
 *        protected.  @n 
 *        4. This function checks the UE CB control block to check the flag for the 
 *        first DL message and performs only integrity protection and unsets 
 *        the flag.       @n 
 *            
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] sdu      SDU to be processed.
 *  @param[in] sduId    SDU ID. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */

U32 pjTxSdu;

#ifdef ANSI
PUBLIC S16 pjDlmProcessSdus
(
PjCb       *gCb,
PjDlRbCb   *pjRbCb,                   /* !< PDCP Control Block */
Buffer     *sdu,                      /* !< SDU to be processed */
U32        sduId,                      /* !< PDCP SDU ID */
PjSn       sn,                        /* !< PDCP SN to be assigned */
U32        txHfn                      /* !< TX_HFN to be assigned */
)
#else 
PUBLIC S16 pjDlmProcessSdus(gCb, pjRbCb, sdu, sduId,sn, txHfn)
PjCb       *gCb;
PjDlRbCb   *pjRbCb;                   /* !< PDCP Control Block */
Buffer     *sdu;                      /* !< SDU to be processed */
U32        sduId;                     /* !< PDCP SDU ID */
PjSn       sn;                        /* !< PDCP SN to be assigned */ 
U32        txHfn;                     /* !< TX_HFN to be assigned */
#endif
{
   S16      ret;                  /* Return Value */
   PjTxEnt  *txEnt;               /* Transmission Entity for sdu*/
   U32      count;                /* Count Value for this txEnt */

   TRC2(pjDlmProcessSdus)

   ret = ROK;
   
   /* Allocate the transmission entity */
   PJ_ALLOC(gCb,txEnt, sizeof(PjTxEnt));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (txEnt == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_FREE_BUF(sdu);
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   /* Update COUNT to the current count in dlCb*/
   PJ_UPD_COUNT(pjRbCb, sn, txHfn, count);
   pjRbCb->dlCb.count = count;

   /* Fill TxEnt values */
   txEnt->sn = sn;
   txEnt->count = count;
   txEnt->txHfn = txHfn;
   txEnt->state = PJ_SDU_RECEIVED;
   txEnt->sduId = sduId;
   /* The received buffer is stored in the SDU and the output from 
    * Compression/Ciphering is stored in the PDU.
    */
   txEnt->sdu   = sdu;
   txEnt->pdu   = NULLP;
   /* Insert TxEnt into the transmission buffer */
   pjDbmInsTxEnt(gCb, &(pjRbCb->dlCb.txBuf), txEnt);

   /* Process the SDU based on rbType */
   if (pjRbCb->rbType == PJ_SRB)
   {
      //QU_CRITLOG(QU_LOG_MESSAGE,"PDCP:Processing SRB message");
      ret = pjDlmProcSrb(gCb, pjRbCb, txEnt);
   }
   else
   {
#ifndef QCOM_STATS
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
      {      
      EXTERN U32 dlrate_pju;
   MsgLen len;
   SFndLenMsg(sdu, (MsgLen *) &len);
   dlrate_pju += len;
      }
#endif 
#endif 
#endif 
#ifdef QCOM_STATS
      {
         MsgLen len;                 
         SFndLenMsg(sdu, (MsgLen *) &len);
         pjDlrate_pju += len;
      }
#endif
      ret = pjDlmProcDrb(gCb, pjRbCb, txEnt);
   }

   RETVALUE(ret);
}

/**
 *
 * @brief 
 *
 *        Handler to Re-establish a SRB.
 *
 * @b Description: 
 *
 *        1. This function is called when re-establishment request is
 *        received for a SRB.   @n
 *        2. The stored PDUs and SDUs are freed and the variables are reset. @n
 *            
 *  @param[in] pjRbCb   PDCP control block.
 *
 */

#ifdef ANSI
PUBLIC Void pjDlmRbDataResume
(
PjCb   *gCb,
PjDlRbCb *pjRbCb
)
#else
PUBLIC Void pjDlmRbDataResume(gCb, pjRbCb)
PjCb   *gCb;
PjDlRbCb *pjRbCb;
#endif
{
   S16      ret;
   PjTxEnt *txEnt;
   TRC3(pjDlmRbDataResume)

   RLOG_ARG0(L_DEBUG,DBG_RBID,pjRbCb->rbId, "pjDlmRbDataResume(pjRbCb ()) ");
                       
   cmLListFirst(&pjRbCb->dlCb.txBuf.reEstPktQ);
   while (cmLListCrnt(&pjRbCb->dlCb.txBuf.reEstPktQ))
   {
      txEnt = (PjTxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->dlCb.txBuf.reEstPktQ));
      if (txEnt != NULLP)
         {
         /* Free the existing PDU and
          * cpy the SDU to PDU */
#ifdef FLAT_BUFFER_OPT
         if(txEnt->sdu == NULLP)
         {
            PJ_FREE_BUF(txEnt->pdu);
         }
         else
#endif
         if ( txEnt->pdu != txEnt->sdu )
         {
            PJ_FREE_BUF(txEnt->pdu);
         }
         else
         {
            txEnt->pdu = NULLP;
         }

         txEnt->state = PJ_SDU_RECEIVED;
         /* Fix for ccpu00135798 */
         txEnt->reEstPktEnt.node = NULLP;
         cmLListDelFrm(&pjRbCb->dlCb.txBuf.reEstPktQ, &txEnt->reEstPktEnt);
         
         txEnt->sduSubmitEnt.node = (PTR)txEnt;
         cmLListAdd2Tail(&pjRbCb->dlCb.txBuf.sduSubmitQ, &txEnt->sduSubmitEnt);

         /*This is a defensive check. Not sure why this coming NULL*/
         if(txEnt->pdu != NULLP || txEnt->sdu !=NULL)
         {
         ret = pjDlmProcDrb(gCb, pjRbCb, txEnt);
         if ( ret != ROK )
         {
             RLOG_ARG0(L_ERROR,DBG_RBID,pjRbCb->rbId, 
                   "pjDlmRbDataResume: pjDlmProcDrb Failed ");
         }
         }
      }
      cmLListFirst(&pjRbCb->dlCb.txBuf.reEstPktQ);
   }

   pjDlmProcessDlPktQ(gCb,pjRbCb);
   
   RETVOID;
}

#ifdef FLAT_BUFFER_OPT

/**
 *
 * @brief 
 *
 *        Handler to process the SDU received from upper layer, form a PDU
 *        and send the PDU to the lower layer.
 *
 * @b Description: 
 *
 *        1. This function places the SDU in the transmission buffer and performs
 *        Compression for DRB SDUs and Integrity Protection for SRB SDUs.  @n
 *        2. After compression/integrity protection, it performs ciphering and then 
 *        constructs the PDU and sends it to the lower layer. @n
 *        3. The first DL message of SRBs is not ciphered and is just integrity 
 *        protected.  @n 
 *        4. This function checks the UE CB control block to check the flag for the 
 *        first DL message and performs only integrity protection and unsets 
 *        the flag.       @n 
 *            
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] sdu      SDU to be processed.
 *  @param[in] sduId    SDU ID. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */

#ifdef ANSI
PUBLIC S16 pjDlmProcessSdusFB
(
PjCb   *gCb,                        /* !< Global control Block*/
PjDlRbCb *pjRbCb,                   /* !< PDCP Control Block */
FlatBuffer *sdu,                    /* !< SDU to be processed */
U32    sduId,                       /* !< PDCP SDU ID */
PjSn       sn,                      /* !< PDCP SN to be assigned */
U32        txHfn                    /* !< TX_HFN to be assigned */
)
#else 
PUBLIC S16 pjDlmProcessSdusFB(gCb, pjRbCb, sdu, sduId, sn, txHfn)
PjCb   *gCb;                        /* !< Global control Block*/
PjDlRbCb *pjRbCb;                   /* !< PDCP Control Block */
FlatBuffer *sdu;                    /* !< SDU to be processed */
U32    sduId;                       /* !< PDCP SDU ID */
PjSn       sn;                      /* !< PDCP SN to be assigned */
U32        txHfn;                   /* !< TX_HFN to be assigned */
#endif
{
   S16      ret;                  /* Return Value */
   PjTxEnt  *txEnt;               /* Transmission Entity for sdu*/

   TRC2(pjDlmProcessSdusFB)
      
#ifndef ALIGN_64BIT
   RLOG_ARG2(L_DEBUG,DBG_RBID,pjRbCb->rbId, 
         "pjDlmProcessSdusFB(pjRbCb(), sdu, sduId(%ld),sn(%u), txHfn) )",
                         sduId, sn);
#else
   RLOG_ARG2(L_DEBUG,DBG_RBID,pjRbCb->rbId, 
         "pjDlmProcessSdusFB(pjRbCb(), sdu, sduId(%d),sn(%u), txHfn) )",
                         sduId, sn);
#endif


   ret = ROK;
   
   /* Allocate the transmission entity */
   PJ_ALLOC(gCb,txEnt, sizeof(PjTxEnt));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (txEnt == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      PJ_FREE_FLAT_BUF(gCb,sdu);
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   /* Update COUNT to the current count in dlCb*/

   PJ_UPD_COUNT(pjRbCb, pjRbCb->dlCb.nxtTxSn,  pjRbCb->dlCb.txHfn,  pjRbCb->dlCb.count);
   PJ_DLM_UPD_VAR(pjRbCb,  txEnt->sn, txEnt->txHfn);      
   /* Fill TxEnt values */
   txEnt->count = pjRbCb->dlCb.count;
   txEnt->state = PJ_SDU_RECEIVED;
   txEnt->sduId = sduId;
   /* The received buffer is stored in the SDU and the output from 
    * Compression/Ciphering is stored in the PDU.
    */
   txEnt->fb   = *sdu;
   txEnt->pdu   = NULLP;
   txEnt->sdu   = NULLP;
   /* Insert TxEnt into the transmission buffer */
   pjDbmInsTxEnt(gCb, &(pjRbCb->dlCb.txBuf), txEnt);
   /* kw005.201 added support for L2 Measurement */

   /* Process the SDU based on rbType */
   {
#ifndef QCOM_STATS
#ifdef MSPD
      {
         extern U32 dlrate_pju;
         dlrate_pju += sdu->len;
      }
#endif
#endif
      ret = pjDlmProcDrb(gCb, pjRbCb, txEnt);
   }

   RETVALUE(ret);
}
#endif

/**
*
 * @brief 
 *
 *        Handler to construct a data PDU/Control PDU.
 *
 * @b Description: 
 *
 *        1. This function constructs the PDU header based on the SN length
 *        configured.    @n
 *        2. The header is inserted at the start of the PDU and
 *        the constructed PDU is returned to the calling function. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PRIVATE S16 pjDlmBldPdu
(
PjCb    *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
)
#else
PRIVATE S16 pjDlmBldPdu(gCb,pjRbCb,txEnt)
PjCb    *gCb;
PjDlRbCb  *pjRbCb;
PjTxEnt *txEnt;
#endif
{
   U16        hdr;                /* Header field */
   S16        ret;                /* Return Value */
   U32        numBytes;           /* Number of bytes to be added to the PDU */

   TRC3(pjDlmBldPdu)
  
   hdr       = txEnt->sn;
   ret       = ROK;
   numBytes  = 1; /* for SRB it is 1 byte and even for 7 bit SN  on DRB it is 1 byte */

   /* Build a Data PDU */
   /* 5 bit SN is the only header field */
   if ( pjRbCb->rbType != PJ_SRB )
   {
      /* The one byte header contains the MSB as '1' */
      if ( pjRbCb->snLen == PJ_7_BIT_SN )
      {
         hdr |= PJ_DRB_7BIT_SN_HDR;
      }
      /* The two byte header has the SN and MSB as '1' */
      else if (pjRbCb->snLen == PJ_12_BIT_SN) 
      {
         hdr |= PJ_DRB_12BIT_SN_HDR;
         numBytes = 2;
      }
      else if (pjRbCb->snLen == PJ_15_BIT_SN)
      {
         hdr |= PJ_DRB_15BIT_SN_HDR;
         numBytes = 2;
      }
   }

   /* Add the hdr(based on numBytes) at the start of the PDU */
   PJ_ADD_PRE_MSG( txEnt->pdu, hdr, numBytes, ret);

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (ret != ROK)
   {
      RLOG_ARG2(L_ERROR, DBG_RBID, pjRbCb->rbId, 
            "SAddPreMsg failed SN [%u], HFN [%lu]",
            txEnt->sn ,  txEnt->txHfn);
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */
   RETVALUE(ret);
}

/**
 *
 * @brief 
 *
 *        Handler to Re-establish a SRB.
 *
 * @b Description: 
 *
 *        1. This function is called when re-establishment request is
 *        received for a SRB.   @n
 *        2. The stored PDUs and SDUs are freed and the variables are reset. @n
 *            
 *  @param[in] pjRbCb   PDCP control block.
 *
 */

#ifdef ANSI
PUBLIC Void pjDlmReEstSrb
(
PjCb   *gCb,
PjDlRbCb *pjRbCb
)
#else
PUBLIC Void pjDlmReEstSrb(gCb, pjRbCb)
PjCb   *gCb;
PjDlRbCb *pjRbCb;
#endif
{

   TRC3(pjDlmResetSrb)

   RLOG_ARG0(L_DEBUG,DBG_RBID,pjRbCb->rbId, "pjDlmResetSrb(pjRbCb ()) ");
  

   /* Stop the off-board and discard timer if running */
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   if (pjRbCb->dlCb.obdTmr.tmrEvnt == PJ_EVT_DL_OBD_TMR)
   {
       pjStopTmr(gCb, (PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }
#endif
   /* Clean up all the SDUs present */ 
   pjDbmTxDelAll(gCb, &(pjRbCb->dlCb.txBuf));

   /* Reset the variables in rbCb */
   PJ_RESET_ALL_VAR(pjRbCb);
   /*kw004.201 Adding of Missing Trace in LTE RLC PDCP*/
   RETVOID;
}

/**
 *
 * @brief 
 *
 *        Handler to Re-establish a UM DRB.
 *
 * @b Description: 
 *
 *        1. This function is called when re-establishment request is
 *           received for a UM DRB.   @n
 *        2. The stored PDUs are freed and the variables are reset.  @n
 *        3. The PDUs which were associated with a SN and were not sent 
 *           to the lower layer are now sent with the new ciphering 
 *           algorithm and keys. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjDlmReEstDrbUm
(
PjCb   *gCb,
PjDlRbCb *pjRbCb
)
#else
PUBLIC S16 pjDlmReEstDrbUm(gCb,pjRbCb)
PjCb   *gCb;
PjDlRbCb *pjRbCb;
#endif
{
   S16            ret;                     /* Return Value */ 
   U32            newCount;                /* New Count of a PDU */
   PjTxEnt        *txEnt;                  /* Transmission Entity */
   CmLListCp      *datQ;                   /* Data Queue storing the txEnts */
   PjBuf          tmpTxBuf;                /* Tx Buffer */ 
 
   TRC3(pjDlmReEstDrbUm)

   RLOG_ARG0(L_DEBUG,DBG_RBID,pjRbCb->rbId, "pjDlmReEstDrbUm(pjRbCb ()) ");
   ret      = ROK; /* KW_FIX */
   newCount =  0;
   txEnt    =  NULLP;
   datQ     = NULLP;

   /* Reset all the variable */
   PJ_RESET_ALL_VAR(pjRbCb);
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   /* Stop the off board timer if running */
   if (pjRbCb->dlCb.obdTmr.tmrEvnt == PJ_EVT_DL_OBD_TMR)
   {
       pjStopTmr(gCb, (PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   } 
#endif

   if(pjRbCb->dlCb.txBuf.numEntries == 0)
   {
      RETVALUE(ROK);
   }

   /* Create the new buffer list  and store the old pointer in datQ */
   PJ_CREATE_NEW_LIST(gCb,pjRbCb, tmpTxBuf, ret);
#if (ERRCLASS & ERRCLS_ADD_RES) 
   if (ret != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   datQ = tmpTxBuf.datQ;
   /* Process all the existing PDUs as received from above layer */
   cmLListFirst(&tmpTxBuf.sduSubmitQ);
   while (cmLListCrnt(&tmpTxBuf.sduSubmitQ))
   {
      U8   hashKey = 0;
      txEnt = (PjTxEnt *) cmLListNode(cmLListCrnt(&tmpTxBuf.sduSubmitQ));
      /* Assign the new SN, TX_HFN and insert in the new transmission
       * buffer.
       */
      if ( txEnt != NULLP )
      {
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
         if ( txEnt->pdu != txEnt->sdu )
         {
            PJ_FREE_BUF(txEnt->pdu);
         }
         else
         {
            txEnt->pdu = NULLP;
         }
#endif
         hashKey = (U8)PJ_HASH_FN((&tmpTxBuf), txEnt->count);
         PJ_UPD_COUNT(pjRbCb, pjRbCb->dlCb.nxtTxSn,  pjRbCb->dlCb.txHfn,  pjRbCb->dlCb.count);
         PJ_DLM_UPD_VAR(pjRbCb,  txEnt->sn, txEnt->txHfn);      
         txEnt->state = PJ_SDU_RECEIVED;
         txEnt->pdu = NULLP;

         cmLListDelFrm(&tmpTxBuf.datQ[hashKey], &txEnt->lstEnt);
         cmLListDelFrm(&tmpTxBuf.sduSubmitQ, &txEnt->sduSubmitEnt);

         tmpTxBuf.numEntries --;

         pjDbmInsTxEnt(gCb, &(pjRbCb->dlCb.txBuf), txEnt);

         txEnt->reEstPktEnt.node = (PTR)txEnt;
         cmLListAdd2Tail(&pjRbCb->dlCb.txBuf.reEstPktQ, &txEnt->reEstPktEnt);

         txEnt->sduSubmitEnt.node=NULLP;
         cmLListDelFrm(&pjRbCb->dlCb.txBuf.sduSubmitQ, &txEnt->sduSubmitEnt);
      }
      cmLListFirst(&tmpTxBuf.sduSubmitQ);
   }
   /* Free up the old transmission buffer */
   pjDbmTxDeInit(gCb,&tmpTxBuf);
   RETVALUE(ret);    
}

/**
 *
 * @brief 
 *
 *        Handler to Re-establish a AM DRB.
 *
 * @b Description: 
 *
 *        1. This function is called when re-establishment request is
 *        received for a AM DRB.     @n
 *        2. The stored PDUs are freed.   @n
 *        3. The PDUs which were not submitted to the lower
 *        layer and for which a confirm has not been received
 *        are now sent with the new ciphering algorithm and keys. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjDlmReEstDrbAm
(
PjCb   *gCb,
PjDlRbCb *pjRbCb
)
#else
PUBLIC S16 pjDlmReEstDrbAm(gCb,pjRbCb)
PjCb   *gCb;
PjDlRbCb *pjRbCb;
#endif
{
   PjTxEnt        *txEnt;

   TRC3(pjDlmReEstDrbAm)

   RLOG_ARG0(L_DEBUG,DBG_RBID,pjRbCb->rbId, "pjDlmReEstDrbAm(pjRbCb ()) ");
 
  
   if (pjRbCb->dlCb.txBuf.numEntries == 0)
   {
      RETVALUE(ROK);
   }
 
   cmLListFirst(&pjRbCb->dlCb.txBuf.datCfmQ);
   while (cmLListCrnt(&pjRbCb->dlCb.txBuf.datCfmQ))
   {
      txEnt = (PjTxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->dlCb.txBuf.datCfmQ));
#if (ERRCLASS & ERRCLS_DEBUG)  /* KW_FIX */
         if(txEnt == NULLP)
         {
            RLOG_ARG0(L_ERROR,DBG_RBID,pjRbCb->rbId, 
                  "pjDlmReEstDrbAm:Transmission Entity is NULL ");
            RETVALUE(RFAILED);
         }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      if (txEnt != NULLP)
      {
          txEnt->reEstPktEnt.node = (PTR)txEnt;
          cmLListAdd2Tail(&pjRbCb->dlCb.txBuf.reEstPktQ, &txEnt->reEstPktEnt);
          pjTotDlPckCntPerCell--;          
      }
      txEnt->datCfmEnt.node = NULLP;
      cmLListDelFrm(&pjRbCb->dlCb.txBuf.datCfmQ, &txEnt->datCfmEnt);
      cmLListFirst(&pjRbCb->dlCb.txBuf.datCfmQ);
   }

   cmLListFirst(&pjRbCb->dlCb.txBuf.sduSubmitQ);
   while (cmLListCrnt(&pjRbCb->dlCb.txBuf.sduSubmitQ))
   {
      txEnt = (PjTxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->dlCb.txBuf.sduSubmitQ));
      if (txEnt != NULLP)      /*KW_FIX : ccpu00136902*/
      {
         txEnt->reEstPktEnt.node = (PTR)txEnt;
         cmLListAdd2Tail(&pjRbCb->dlCb.txBuf.reEstPktQ, &txEnt->reEstPktEnt);
         txEnt->sduSubmitEnt.node=NULLP;
         cmLListDelFrm(&pjRbCb->dlCb.txBuf.sduSubmitQ, &txEnt->sduSubmitEnt);
         cmLListFirst(&pjRbCb->dlCb.txBuf.sduSubmitQ);
      }
      else
      {
         RLOG_ARG0(L_ERROR,DBG_RBID,pjRbCb->rbId, "This should not be hit ");
         break;
      }
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Handler to send data confirmation to RRC.
 *
 * @b Description: 
 *
 *        1. This function is called when a status indication or a confirm is 
 *        received from the lower layer.  @n
 *        2. The confirm type can be a success or failure. @n 
 *        3. The corresponding SDUs and PDUs are cleaned up and
 *        if cfmReqd is configured, a confirm is sent to the upper layer. @n
 *            
 *  @param[in] pjRbCb    PDCP control block. 
 *  @param[in] staInd  Status Indication Information.
 *  @param[in] cfmType Confirm Type can be SUCCESS or FAILURE.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

U32 pjRxCfm;

#ifdef ANSI
PUBLIC S16 pjDlmProcessCfm
(
PjCb          *gCb,
PjDlRbCb        *pjRbCb,
PjDatCfm      *pjDatCfm,
U8            cfmType
)
#else
PUBLIC S16 pjDlmProcessCfm(gCb,pjRbCb,pjDatCfm,cfmType)
PjCb          *gCb;
PjDlRbCb        *pjRbCb;
PjDatCfm      *pjDatCfm;
U8            cfmType;
#endif
{
   PjuDatCfmInfo  *datCfm;        /* Data Cfm to be sent to upper layer */
   U32            cnt;            /* Counter for number of SDUs in datCfm*/
   U16            datCfmCnt;      /* Counter for number of SDUs in datCfm*/ /*KW_FIX*/
   PjPjuSapCb     *pjuSap;        /* Upper Sap of PDCP User */
   CmLtePdcpId    *pdcpId;        /* PDCP ID */
   PjDlCb         *dlCb;           /* Downlink control block */
   PjTxEnt        *txEnt;          /* Transmission entity */

   TRC3(pjDlmProcessCfm)

   gCb->pjPerfSts.pjSduRcvCnt += pjDatCfm->numSdu;

   dlCb     = &pjRbCb->dlCb;
   pdcpId   = NULLP;
   cnt      = 0;
   pjuSap   = NULLP;
   datCfm   = NULLP;
   txEnt    = NULLP;

   /* If cfmReqd flag is sent a confirmation is sent to the above layer.
    * For each SDU, get the sduId and status and fill in the datCfm
    * structure and discard the txEnt. Also update cfmExp count.
    */
   if ( (pjRbCb->dlCb.cfmReqd) || (PJ_CFM_NOK == cfmType))
   {
      PjuDatCfmInfo datCfmTmp;
      CmLtePdcpId pdcpIdTmp;
      datCfm = &datCfmTmp;
      pdcpId = &pdcpIdTmp;
      pdcpId->rbId   = pjRbCb->rbId;
      pdcpId->rbType = pjRbCb->rbType;
      pdcpId->ueId   = pjRbCb->ueCb->key.ueId;
      pdcpId->cellId = pjRbCb->ueCb->key.cellId;

      if (pjRbCb->rbType == PJ_SRB)
         pjuSap = &(gCb->u.dlCb->pjuSap[PJ_SRB_SAP]);
      else
         pjuSap = &(gCb->u.dlCb->pjuSap[PJ_DRB_SAP]);

      datCfmCnt = 0;
      for ( cnt = 0; cnt < pjDatCfm->numSdu; cnt++)
      {
         txEnt = (PjTxEnt *)pjDbmGetTxEnt(gCb,&(pjRbCb->dlCb.txBuf), 
                                       pjDatCfm->sduId[cnt]);
         if ( txEnt != NULLP )
         {
            datCfm->cfmSta[datCfmCnt].sduId  = txEnt->sduId;
            /* RLC_DL_MAX_RETX fix */
            if (PJ_CFM_OK != cfmType)
            {
               datCfm->cfmSta[datCfmCnt].status = PJU_RLC_TRANS_FAIL;
            }
            else
            {
               datCfm->cfmSta[datCfmCnt].status = cfmType;
            }

            datCfm->cfmSta[datCfmCnt].status = cfmType;
            datCfmCnt ++;
            pjDbmDelTxEnt(gCb, &(pjRbCb->dlCb.txBuf), txEnt->count);
         }
      }
      datCfm->numSdus = datCfmCnt;
      if(datCfm->numSdus)
      {
         /* If trace flag is enabled send the trace indication */
         if(gCb->init.trc == TRUE)
         {
            /* Populate the trace params */
            pjLmmSendTrc(gCb, EVTPJUDATCFM, NULLP);
         }

         PjUiPjuDatCfm(&pjuSap->pst, pjuSap->suId, pdcpId, datCfm);
      }
   }
   /* If cfmReqd is not set, discard the txEnts and update the cfmExp count */
   else
   {
      for ( cnt = 0; cnt < pjDatCfm->numSdu; cnt++)
      {
         pjDbmDelTxEnt(gCb, &(pjRbCb->dlCb.txBuf), pjDatCfm->sduId[cnt]);
      }
   
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Handler to send process the status report.
 *
 * @b Description: 
 *
 *        1. This function is called when a status report is received from the
 *        peer.    @n
 *        2. This function process the status report and sends a confirmation
 *        to the above layer and removes the PDUs and SDUs for which positive 
 *        acknowledgement is received.  @n
 *            
 *  @param[in] pjRbCb      PDCP control block. 
 *  @param[in] staPdu      Status report.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjDlmHndlStaRep
(
PjCb     *gCb,
PjDlRbCb   *pjRbCb,
PjSn     fms,
Buffer   *staPdu
)
#else
PUBLIC S16 pjDlmHndlStaRep(gCb,pjRbCb,fms,staPdu)
PjCb     *gCb;
PjDlRbCb   *pjRbCb;
PjSn     fms;
Buffer   *staPdu;
#endif
{
   PjuDatCfmInfo *datCfm;       /* Dat Cfm to be sent to the upper layer */
   PjuDatCfmInfo datCfmTmp;     /* Tmp variable allocated to assign local memory to the datCfm pointer */
   CmLtePdcpId pdcpIdTmp;       /* Tmp variable allocated to assign local memory to the pdcpId pointer */
   S16           ret;           /* Return Value */
   U8            cnt;           /* Counter for the bits in a byte */
   U32           bitPos;        /* Bit position in the Status PDU */
   PjPjuSapCb    *pjuSap;       /* PJU SAP Control Block */
   CmLtePdcpId   *pdcpId;       /* PDCP ID */
#ifdef PJ_HNDL_BITMAP
   U8            sduSta;        /* SDU Status of reception at the Peer */
   PjTxEnt       *txEnt;        /* Transmission entity */
   U8            byte;          /* One Byte of a Status Pdu */
   MsgLen        bMapLen;       /* StaPdu Length in bytes */
   PjSn          sn;            /* SN of the PDU */
#endif

   TRC3(pjDlmHndlStaRep)

   RLOG_ARG1(L_DEBUG,DBG_RBID,pjRbCb->rbId, "pjDlmHndlStaRep(pjRbCb (), fms (%u), staPdu) ",fms);

   datCfm  = NULLP;
   ret     = ROK;
   cnt     = 0;
   bitPos  = 1;
   pjuSap  = NULLP;
   pdcpId  = NULLP;

   
   /* Allocate for datCfm structure is cfmReqd is set */
   if ( pjRbCb->dlCb.cfmReqd )
   {
    datCfm = &datCfmTmp;
   }

   /* Fills the confirm information for all the PDUs whose COUNT < COUNT(FMS) 
    * and sets cfmExp to fms
    */
   pjDlmDelTxEntUptoFms(gCb, pjRbCb, fms, datCfm);

#ifdef PJ_HNDL_BITMAP

   byte    = 0;
   bMapLen = 0;
   SFndLenMsg(staPdu, &bMapLen);

   /* Read one byte at a time from Status PDU */
   while ( bMapLen != 0 )
   {
      ret = SRemPreMsg(&byte,staPdu);
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (ret != ROK)
      {
         RLOG0(L_ERROR, "SRemPreMsg Failed for staPdu");
         PJ_FREE_BUF(staPdu);
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_RES */

      /* Check if each bit in a byte is set or not.
       * If cfmReqd is set fill in the cfm info.
       * Discard the txEnt if the bit is set to 1.
       */
      for ( cnt = 0; cnt < PJ_BYTE_LEN; cnt++, bitPos++ )
      {
         sduSta  = (byte << cnt) & PJ_FIRST_BIT;
         if (sduSta)
         {
            sn      = (fms + bitPos) % PJ_TX_BUF_LEN;
            txEnt   = (PjTxEnt *)pjDbmGetTxEntSn(gCb, &(pjRbCb->dlCb.txBuf), sn );
            if ( txEnt != NULLP )
            {
               if ((pjRbCb->dlCb.cfmReqd)&& (datCfm->numSdus < PJU_MAX_SDU_CFM))
               {
                   
                  datCfm->cfmSta[datCfm->numSdus].sduId  = txEnt->sduId;
                  datCfm->cfmSta[datCfm->numSdus].status = sduSta;
                  datCfm->numSdus++;;
               }
               pjDbmDelTxEnt(gCb, &(pjRbCb->dlCb.txBuf), txEnt->count); 
            }
         }
      }
      bMapLen--;
   }
#endif
   /* Send a confirmation to the upper layer */
   if ((pjRbCb->dlCb.cfmReqd) && (datCfm->numSdus > 0))
   {
     pdcpId = &pdcpIdTmp;

     pdcpId->rbId   = pjRbCb->rbId;
     pdcpId->rbType = pjRbCb->rbType;
     pdcpId->ueId   = pjRbCb->ueCb->key.ueId;
     pdcpId->cellId = pjRbCb->ueCb->key.cellId;

     pjuSap   =  &gCb->u.dlCb->pjuSap[PJ_DRB_SAP];
     /* If trace flag is enabled send the trace indication */
     if(gCb->init.trc == TRUE)
     {
       /* Populate the trace params */
       pjLmmSendTrc(gCb, EVTPJUDATCFM, NULLP);
     }
     PjUiPjuDatCfm(&pjuSap->pst, pjuSap->suId, pdcpId, datCfm);
   }

   PJ_FREE_BUF(staPdu);

   RETVALUE(ret); 
}

/**
 *
 * @brief 
 *
 *        Handler to forward the downlink data to the upper layer.
 *
 * @b Description: 
 *
 *        1. This function is used to send the downlink data to the upper layer
 *        during handover.  @n
 *        2. The unacknowledged data SDUs are sent to the upper
 *        layer.    @n
 *            
 *  @param[in] pjRbCb      PDCP control block. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjDlmReEstHoDrbAm
(
PjCb   *gCb,
PjDlRbCb *pjRbCb
)
#else
PUBLIC S16 pjDlmReEstHoDrbAm(gCb,pjRbCb)
PjCb    *gCb;
PjDlRbCb  *pjRbCb;
#endif
{
   TRC3(pjDlmReEstHoDrbAm)
   
   RLOG_ARG0(L_DEBUG,DBG_RBID,pjRbCb->rbId, "pjDlmReEstHoDrbAm(pjRbCb ()) ");


   pjRbCb->pktAdmitCnt = 0;
   pjRbCb->ueCb->hoInfo->hoCfmInfo[pjRbCb->rbId].pres = TRUE;
   pjRbCb->ueCb->hoInfo->hoCfmInfo[pjRbCb->rbId].rbId = pjRbCb->rbId;
   pjRbCb->ueCb->hoInfo->hoCfmInfo[pjRbCb->rbId].dir  = PJ_DIR_DL;
   pjRbCb->ueCb->hoInfo->hoCfmInfo[pjRbCb->rbId].count = pjRbCb->dlCb.count + 1;

   RETVALUE(ROK);
}


/**
 *
 * @brief 
 *
 *        Handler to forward the downlink data to the upper layer.
 *
 * @b Description: 
 *
 *        1. This function is used to send the downlink data to the upper layer
 *        during handover.  @n
 *        2. The unacknowledged data SDUs are sent to the upper
 *        layer.    @n
 *            
 *  @param[in] pjRbCb      PDCP control block. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

PRIVATE Pst gself2Pdcp =
{
   0,
   0,
   ENTPJ,
   0/*DL INST*/,
   ENTPJ,
   0,
   0,
   0,
   0x31/*EVTCPJSTARTDATAFWDREQ*/,
   0, /* region */
   0, /* pool */
   0,
   0
};


#ifdef ANSI
PUBLIC S16 pjDlmStartDataFrwdPerRb
(
PjCb       *gCb,
PjDlRbCb   *pjRbCb
)
#else
PUBLIC S16 pjDlmStartDataFrwdPerRb(gCb,pjRbCb)
PjCb       *gCb;
PjDlRbCb   *pjRbCb;
#endif
{
#define MAX_PKTS_FWD_SEND_AT_ONCE 32
   U32              numSdu =0;
   U16              count;   /*KW_FIX*/
   U32              numSduCnt =0,numSduTx = 0;
   PjTxEnt          *txEnt = NULLP;       
   PjuDatFwdIndInfo *datFwdInd = NULLP;  
   PjPjuSapCb   *pjuSap;
   PjDlPkt    *pkt = NULLP;
   U32        numFwdedPkts=0;
   /*Bool       brekFrmLoop=FALSE;*/

   TRC3(pjDlmStartDataFrwdPerRb)
   
   RLOG_ARG0(L_DEBUG,DBG_RBID,pjRbCb->rbId, "pjDlmStartDataFrwdPerRb(pjRbCb ()) ");

   pjuSap   = &(gCb->u.dlCb->pjuSap[PJ_DRB_SAP]);
   /* Update hoCfmInfo, used to fill SduStaCfm */
   if((pjRbCb->dlCb.nxtTxSn == 0) && ( pjRbCb->dlCb.txHfn == 0))
   {
    //  PJ_ALLOC(gCb,datFwdInd, sizeof(PjuDatFwdIndInfo));
   if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,(Data **)&datFwdInd, 
            sizeof (PjuDatFwdIndInfo)) != ROK)
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (datFwdInd == NULLP)
      {
         /*ccpu00136858 */      
         RLOG0(L_FATAL, "Memory Allocation failed.");
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
         RETVALUE(RFAILED);
   }
      datFwdInd->dir = PJ_DIR_DL;
      datFwdInd->numSdus = 0;
      datFwdInd->isLastDatFwdInd = TRUE;

      /* sending DatFwdInd even if numSdu is zero */
      pjUtlDlSndDatFwdInd(gCb,pjRbCb, datFwdInd);
      RETVALUE(ROK);
   }

   /* Find the total count of the txEnts present */
   numSdu = pjRbCb->dlCb.txBuf.numEntries + pjRbCb->dlCb.dlPktQ.count;

   /* This was added from Hotfix branch when Data Fwding was not supported */

   /*lastSntPktdatCfmQ is NULL when this funtion is called for 1st time
    for a given pjRbCb. In the next run it will be pointing to the node
    from which we need to resume data forwarding*/
   if(pjRbCb->dlCb.txBuf.lastSntPktdatCfmQ == NULL)
   {
   cmLListFirst(&pjRbCb->dlCb.txBuf.datCfmQ);
   }
   else
   {
     pjRbCb->dlCb.txBuf.datCfmQ.crnt = pjRbCb->dlCb.txBuf.lastSntPktdatCfmQ;
   }
   while (cmLListCrnt(&pjRbCb->dlCb.txBuf.datCfmQ) &&
    (numFwdedPkts < MAX_PKTS_FWD_SEND_AT_ONCE))
   {
      /*Since we are not emptying list and if we have already parsed
        through this list then just break*/
      if(pjRbCb->dlCb.txBuf.datCfmQRead == TRUE)
   {
         break;
   }

      txEnt = (PjTxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->dlCb.txBuf.datCfmQ));
      if(txEnt->sdu == NULLP)
   {
         QU_CRITLOG(QU_LOG_MESSAGE," pjDlmStartDataFrwdPerRb: SDU is NULL \n");
         cmLListNext(&pjRbCb->dlCb.txBuf.datCfmQ);
         continue;
   }

   if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,(Data **)&datFwdInd, 
            sizeof (PjuDatFwdIndInfo)) != ROK)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

   if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,(Data **)&datFwdInd->datFwdInfo, 
            (sizeof(PjuDatFwdInfo) )) != ROK)
   {
      /* Free memory for DatFwdInfo */
      PJ_PST_FREE(pjuSap->pst.region, pjuSap->pst.pool,datFwdInd,  
               sizeof(PjuDatFwdIndInfo));
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }

      datFwdInd->dir = PJ_DIR_DL;
      datFwdInd->numSdus = 1;
      datFwdInd->datFwdInfo[0].sduId = txEnt->sduId;
      datFwdInd->datFwdInfo[0].sn    = txEnt->sn;
#ifdef FLAT_BUFFER_OPT
         if(txEnt->sdu == NULLP)
         {
         pjUtlCopyFbToBuf(gCb, &(txEnt->fb), &(datFwdInd->datFwdInfo[0].sdu));
         }
         else
#endif
         {
         datFwdInd->datFwdInfo[0].sdu = txEnt->sdu;
         }
      pjUtlDlSndDatFwdInd(gCb,pjRbCb, datFwdInd);
      txEnt->sdu= NULLP;

         cmLListNext(&pjRbCb->dlCb.txBuf.datCfmQ);
      numFwdedPkts++;
      pjRbCb->dlCb.txBuf.lastSntPktdatCfmQ = cmLListCrnt(&pjRbCb->dlCb.txBuf.datCfmQ);
      }

   /*Counter limit is set to 4. This means for a given pjRbCb from txBuf list a 
     maximum of 4x32= 128 packets will be sent. If maximum limit is reached then
     numFwdedPkts is made zero indicating that last PDCP PDU indication needs to be sent*/

   /*if numFwdedPkts is less than MAX_PKTS_FWD_SEND_AT_ONCE it definetly means the list is parsed*/
   if(numFwdedPkts < MAX_PKTS_FWD_SEND_AT_ONCE)
   {
      pjRbCb->dlCb.txBuf.datCfmQRead = TRUE;
   }
   /*if numFwdedPkts == MAX_PKTS_FWD_SEND_AT_ONCE it could mean that list is completely read 
     or some more packets need to be read*/
   else if (numFwdedPkts == MAX_PKTS_FWD_SEND_AT_ONCE)
   {
      if((pjRbCb->dlCb.txBuf.sndCnt*MAX_PKTS_FWD_SEND_AT_ONCE) == pjRbCb->dlCb.txBuf.datCfmQ.count)
      {
         pjRbCb->dlCb.txBuf.datCfmQRead = TRUE;
      }
   }
  
   if(pjRbCb->dlCb.txBuf.lastSntPktsduSubmitQ == NULL)
   {
     cmLListFirst(&pjRbCb->dlCb.txBuf.sduSubmitQ);
   }
   else
   {
      pjRbCb->dlCb.txBuf.sduSubmitQ.crnt = pjRbCb->dlCb.txBuf.lastSntPktsduSubmitQ;
   }
   while (cmLListCrnt(&pjRbCb->dlCb.txBuf.sduSubmitQ) &&
          (numFwdedPkts < MAX_PKTS_FWD_SEND_AT_ONCE))
   {
      /*Since we are not emptying list and if we have already parsed
        through this list then just break*/
      if(pjRbCb->dlCb.txBuf.sduSubmitQRead == TRUE)
      {
         break;
      }

      if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,(Data **)&datFwdInd, 
               sizeof (PjuDatFwdIndInfo)) != ROK)
      {
         /*PJLOGERROR(ERRCLS_ADD_RES, EKW062, (ErrVal) RFAILED,
               "pjDlmStartDataFrwdPerRb: PJ_ALLOC Failed.");*/
         RETVALUE(RFAILED);
      }
         txEnt = (PjTxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->dlCb.txBuf.sduSubmitQ));

      if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,(Data **)&datFwdInd->datFwdInfo, 
         (sizeof(PjuDatFwdInfo) )) != ROK)
      if (datFwdInd->datFwdInfo == NULLP)
      {
         /* Free memory for DatFwdInfo */
         /*PJ_FREE(datFwdInd->datFwdInfo,  numSdu * sizeof(PjuDatFwdIndInfo));
         PJLOGERROR(ERRCLS_ADD_RES, EKW063, (ErrVal) RFAILED,
               "pjDlmStartDataFrwdPerRb: PJ_ALLOC Failed.");*/
         RETVALUE(RFAILED);
      }

      datFwdInd->dir = PJ_DIR_DL;
      datFwdInd->numSdus = 1;
      datFwdInd->datFwdInfo[0].sduId = txEnt->sduId;
      datFwdInd->datFwdInfo[0].sn    = txEnt->sn;
#ifdef FLAT_BUFFER_OPT
         if(txEnt->sdu == NULLP)
         {
         pjUtlCopyFbToBuf(gCb, &(txEnt->fb), &(datFwdInd->datFwdInfo[0].sdu));
         }
         else
#endif
         {
         if((SCpyMsgMsg(txEnt->sdu, 1, 0, &datFwdInd->datFwdInfo[0].sdu))== RFAILED)
         {
            QU_CRITLOG(QU_LOG_MESSAGE,"Failed to Copy msg\n");
            RETVALUE(RFAILED);
         }
      }
      pjUtlDlSndDatFwdInd(gCb,pjRbCb, datFwdInd);

         cmLListNext(&pjRbCb->dlCb.txBuf.sduSubmitQ);

      numFwdedPkts++;
      pjRbCb->dlCb.txBuf.lastSntPktsduSubmitQ = cmLListCrnt(&pjRbCb->dlCb.txBuf.sduSubmitQ);
   }

   if(numFwdedPkts < MAX_PKTS_FWD_SEND_AT_ONCE)
   {
      pjRbCb->dlCb.txBuf.sduSubmitQRead = TRUE;
   }
   else if (numFwdedPkts == MAX_PKTS_FWD_SEND_AT_ONCE)
   {
      /*if((pjRbCb->dlCb.txBuf.sndCnt*MAX_PKTS_FWD_SEND_AT_ONCE) == pjRbCb->dlCb.txBuf.datCfmQ.count)
      {
         pjRbCb->dlCb.txBuf.datCfmQRead = TRUE;
      }*/
      }


   if(pjRbCb->dlCb.lastSntPktdlPktQ == NULL)
   {
      cmLListFirst(&pjRbCb->dlCb.dlPktQ);
   }
   else
      {
      pjRbCb->dlCb.dlPktQ.crnt = pjRbCb->dlCb.lastSntPktdlPktQ;
   }
   /* TODO: Forward data from dlPktQ */
   while (cmLListCrnt(&pjRbCb->dlCb.dlPktQ) &&
         (numFwdedPkts < MAX_PKTS_FWD_SEND_AT_ONCE))
   {
      /*Since we are not emptying list and if we have already parsed
        through this list then just break*/
      if(pjRbCb->dlCb.dlPktQRead == TRUE)
      {
         break;
      }
      if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,(Data **)&datFwdInd, 
               sizeof (PjuDatFwdIndInfo)) != ROK)
      {
         /*PJLOGERROR(ERRCLS_ADD_RES, EKW062, (ErrVal) RFAILED,
               "pjDlmStartDataFrwdPerRbFB: PJ_ALLOC Failed.");*/
         RETVALUE(RFAILED);
      }
         pkt = (PjDlPkt *) cmLListNode(cmLListCrnt(&pjRbCb->dlCb.dlPktQ));


      if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,(Data **)&datFwdInd->datFwdInfo, 
         (sizeof(PjuDatFwdInfo) )) != ROK)
      {
         /* Free memory for DatFwdInfo */
         /*PJ_FREE(datFwdInd->datFwdInfo,  numSdu * sizeof(PjuDatFwdIndInfo));
         PJLOGERROR(ERRCLS_ADD_RES, EKW063, (ErrVal) RFAILED,
               "pjDlmStartDataFrwdPerRbFB: PJ_ALLOC Failed.");*/
         RETVALUE(RFAILED);
      }

      datFwdInd->dir = PJ_DIR_DL;
      datFwdInd->numSdus = 1;
      datFwdInd->datFwdInfo[0].sduId = pkt->sduId;
      datFwdInd->datFwdInfo[0].sn    = pkt->sn;
#ifdef FLAT_BUFFER_OPT
         if(pkt->pdu == NULLP)
         {
         pjUtlCopyFbToBuf(gCb, &(pkt->fb), &(datFwdInd->datFwdInfo[0].sdu));
         }
         else
#endif
         {
         if((SCpyMsgMsg(pkt->pdu, 1, 0, &datFwdInd->datFwdInfo[0].sdu))== RFAILED)
         {
            QU_CRITLOG(QU_LOG_MESSAGE,"Failed to Copy msg\n");
            RETVALUE(RFAILED);
         }
         }

      pjUtlDlSndDatFwdInd(gCb,pjRbCb, datFwdInd);
         cmLListNext(&pjRbCb->dlCb.dlPktQ);
      numFwdedPkts++;
      pjRbCb->dlCb.lastSntPktdlPktQ = cmLListCrnt(&pjRbCb->dlCb.dlPktQ);
      }

   if(numFwdedPkts < MAX_PKTS_FWD_SEND_AT_ONCE)
   {
      pjRbCb->dlCb.dlPktQRead = TRUE;
   }
   else if (numFwdedPkts == MAX_PKTS_FWD_SEND_AT_ONCE)
   {
      /*if((pjRbCb->dlCb.txBuf.sndCnt*MAX_PKTS_FWD_SEND_AT_ONCE) == pjRbCb->dlCb.txBuf.datCfmQ.count)
      {
         pjRbCb->dlCb.txBuf.datCfmQRead = TRUE;
      }*/
   }


   if(numFwdedPkts >= MAX_PKTS_FWD_SEND_AT_ONCE)
   {
     /*self post*/
      CpjDataFwdReqInfo *dataFwdReq;
      SpId               spId=0; /*NH_PJ_SPID is 0*/

      PJ_ALLOC(gCb, dataFwdReq, sizeof(CpjDataFwdReqInfo) );
      if (dataFwdReq == NULLP)
      {
         /*PJLOGERROR(ERRCLS_ADD_RES, EKW062, (ErrVal) RFAILED,
              "pjDlmStartDataFrwdPerRb: PJ_ALLOC Failed.");*/
         RETVALUE(RFAILED);
      }

      dataFwdReq->cellId = pjRbCb->ueCb->key.cellId;
      dataFwdReq->ueId = pjRbCb->ueCb->key.ueId;
      dataFwdReq->transId = 0xFEFEABAB;
      cmPkCpjStartDataFwdReq(&gself2Pdcp, spId, dataFwdReq); 
   }
   else
   {
      QU_CRITLOG(QU_LOG_MESSAGE,"All forwarded List %d UE %d\n",pjRbCb->dlCb.txBuf.datCfmQ.count, pjRbCb->ueCb->key.ueId);
     if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,(Data **)&datFwdInd, 
               sizeof (PjuDatFwdIndInfo)) != ROK)
     {
      /*PJLOGERROR(ERRCLS_ADD_RES, EKW062, (ErrVal) RFAILED,
             "pjDlmStartDataFrwdPerRb: PJ_ALLOC Failed.");*/
      RETVALUE(RFAILED);
   }
   datFwdInd->dir = PJ_DIR_DL;
   datFwdInd->numSdus = 0;
   datFwdInd->isLastDatFwdInd = TRUE;

   /* sending DatFwdInd even if numSdu is zero */
   pjUtlDlSndDatFwdInd(gCb,pjRbCb, datFwdInd);
   }
   RETVALUE(ROK);

}


/**
 *
 * @brief 
 *
 *        Handler to process the forwarded data received from upper layer.
 *
 * @b Description: 
 *
 *        1. This function is used to process the SDUs received from the upper
 *        layer as part of handover.   @n
 *        2. This function calls pjDlmProcessSdus function with the correct 
 *        SN and HFN values.     @n
 *            
 *  @param[in] gCb         PDCP Instance control block. 
 *  @param[in] pjRbCb      Rb control block. 
 *  @param[in] datFwdReq   Data Forward Info.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjDlmHndlDatFwdReq
(
PjCb               *gCb,
PjDlRbCb           *pjRbCb,
PjuDatFwdReqInfo   *datFwdReq
)
#else
PUBLIC S16 pjDlmHndlDatFwdReq(gCb,pjRbCb,datFwdReq)
PjCb               *gCb;
PjDlRbCb           *pjRbCb;
PjuDatFwdReqInfo   *datFwdReq;
#endif
{

   U32      sduCount;            /* Number of received SDUs */
   U32      numSdus;             /* Counter for SDUs */
   U32      sduId;               /* SDU ID */
   U32      txHfn;               /* TX_HFN of a SDU */
   U32      count;               /* COUNT value of the SDU */
   PjTxEnt  *txEnt;              /* Transmission Entity */ 

   TRC3(pjDlmHdlDatFwdReq)

   RLOG_ARG1(L_DEBUG,DBG_RBID,pjRbCb->rbId, "pjDlmHndlDatFwdReq(pjRbCb (),datFwdReq(%d)) ", 
                             datFwdReq->numSdus);

   sduCount = datFwdReq->numSdus;
   sduId    = 0;
   txHfn    = pjRbCb->dlCb.txHfn;
   numSdus  = 0;
   count    = 0;
   
   /* FIXME is this required , TC 11.10 */
   /* FIXME NOT a proper fix also */
   if((pjRbCb->dlCb.nxtTxSn == 0) && ( pjRbCb->dlCb.txHfn == 0))
   {
      pjRbCb->dlCb.nxtTxSn =  datFwdReq->datFwdInfo[sduCount - 1].sn;
      pjRbCb->dlCb.count   =  datFwdReq->datFwdInfo[sduCount - 1].sn;
   }

   gCb->pjGenSts.numPktsRcvd += sduCount;
   /* Process each of the SDUs with received SN and sduId */ 
   for ( numSdus = 0; numSdus < sduCount; numSdus++ )
   {
      /* 
         The variables nxtTxSn and COUNT are assumed to be 
         already updated in dlCb to continue with the 
         transmission ( in the target eNodeB ).
      */
      PJ_ALLOC(gCb,txEnt, sizeof(PjTxEnt)); 

#if (ERRCLASS & ERRCLS_ADD_RES)
      if (txEnt == NULLP)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         PJ_FREE_BUF(datFwdReq->datFwdInfo[numSdus].sdu);
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_RES */


      /* Fill TxEnt values */
      txEnt->sn = datFwdReq->datFwdInfo[numSdus].sn;
      txEnt->sduId = datFwdReq->datFwdInfo[numSdus].sduId;
      txEnt->state = PJ_SDU_RECEIVED;
      txEnt->sdu   = datFwdReq->datFwdInfo[numSdus].sdu;
      txEnt->pdu   = NULLP;

      if ( txEnt->sn  > pjRbCb->dlCb.nxtTxSn )
      {
         txHfn = pjRbCb->dlCb.txHfn -1;
      }
      else
      {
         txHfn = pjRbCb->dlCb.txHfn;
      }   

      PJ_UPD_COUNT(pjRbCb, txEnt->sn, txHfn, txEnt->count);
      pjDbmInsTxEnt(gCb, &(pjRbCb->dlCb.txBuf), txEnt);
      pjDlmProcDrb(gCb,pjRbCb,txEnt);
   }
 

   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Handler to construct a PDU and send it to the lower layer.
 *
 * @b Description: 
 *
 *        1. This function is used to deliver a PDU to the lower layer.@n
 *        2. It calls pjDlmBldPdu function and sends the PDU to 
 *        the lower layer using pjDlmSendDatReq function. @n
 *        3. The txBuf is freed up if the cfmReqd is not configured or 
 *        if the rbType is UM.@n
 *            
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] txEnt    Transmission Entity.
 *  @param[in] mBuf     The PDU to be transmitted.
 *  @param[in] pduType  Type of the PDU.
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */

#ifdef ANSI
PUBLIC S16 pjDlmDeliverPdu
(
PjCb       *gCb,
PjDlRbCb     *pjRbCb,
PjTxEnt    *txEnt
)
#else
PUBLIC S16 pjDlmDeliverPdu(gCb,pjRbCb,txEnt)
PjCb       *gCb;
PjDlRbCb     *pjRbCb;
PjTxEnt    *txEnt;
#endif
{
   PjDlCb         *dlCb;           /* Downlink Control Block */
   S16            ret;            /* Return Value */
   S16            ret1;            /* Return Value */
   U32            count;          /* COUNT value */
   PjTxEnt    *txFirstEntity;


   TRC3(pjDlmDeliverPdu)


   dlCb   = &pjRbCb->dlCb;
   ret    = ROK;
   count  = 0;


   /* Constructs a PDU */
   if ( txEnt->state != PJ_PDU_CONSTRUCTED )
   {
      ret = pjDlmBldPdu(gCb, pjRbCb, txEnt);
   }

   if ( ret != ROK )
   {
      RLOG_ARG3(L_ERROR, DBG_RBID, pjRbCb->rbId, "PDCP DL Build PDU Failed SN [%u], HFN [%lu] , Count [%lu]",  
         txEnt->sn , txEnt->txHfn, txEnt->count);
      PJ_UPD_DL_VAR(gCb, pjRbCb, txEnt->count);
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(gCb,pjRbCb, txEnt, PJ_PDU_CONSTRUCT_FAILED);
      }
      RETVALUE(RFAILED);
   }
   txEnt->state = PJ_PDU_CONSTRUCTED;

   /* Deliver the PDU to the lower layer only if it is nxtToSub.
    * Also deliver the consecutive PDUs if they are constructed.
    * If not just store the PDU.
    */
   cmLListFirst(&pjRbCb->dlCb.txBuf.sduSubmitQ);

   while (cmLListCrnt(&pjRbCb->dlCb.txBuf.sduSubmitQ))
   {
      txFirstEntity = (PjTxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->dlCb.txBuf.sduSubmitQ));
      if (txFirstEntity->state == PJ_PDU_CONSTRUCTED )
      {
     ret1 =
         pjDlmSendDatReq(gCb, pjRbCb, txFirstEntity->count, 
               txFirstEntity->pdu);
         gCb->pjGenSts.txPdus++;
         txFirstEntity->pdu   = NULLP;
         txFirstEntity->state = PJ_PDU_SUBMITTED;

         txFirstEntity->sduSubmitEnt.node = (PTR)NULLP;
         cmLListDelFrm(&pjRbCb->dlCb.txBuf.sduSubmitQ, &txFirstEntity->sduSubmitEnt);

         if ((pjRbCb->mode == PJ_DRB_UM ) 
               || (!(pjRbCb->dlCb.cfmReqd) && pjRbCb->rbType == PJ_SRB)
              || (ret1 != ROK))
         {
              pjDbmDelTxEnt(gCb, &(pjRbCb->dlCb.txBuf), txFirstEntity->count);
         }
         else
         {
            /* Add to the DatCfm Q */
            txFirstEntity->datCfmEnt.node = (PTR)txEnt;
            cmLListAdd2Tail(&pjRbCb->dlCb.txBuf.datCfmQ, &txFirstEntity->datCfmEnt);
            pjTotDlPckCntPerCell++;
         }
         cmLListFirst(&pjRbCb->dlCb.txBuf.sduSubmitQ);
      }
      else
      {
         break;
      }
   }
   
   RETVALUE(ret);
}
/**
*
 * @brief  
 * 
 *        Handler to construct a process a SRB SDU.
 * 
 * @b Description: 
 * 
 *        1. This function performs integrity protection if it is
 *        configured.    @n
 *        2. The first DL message of SRBs is not ciphered and is just integrity 
 *        protected.  @n 
 *        3. This function checks the UE CB control block to check the flag for the 
 *        first DL message and performs only integrity protection and unsets 
 *        the flag.       @n 
 *        4. Ciphering is performed if applicable.
 *        5. The PDU is constructed and then delivered to the lower
 *        layer. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */


#ifdef ANSI
PUBLIC S16 pjDlmProcSrb
(
PjCb    *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
)
#else
PUBLIC S16 pjDlmProcSrb(gCb, pjRbCb, txEnt)
PjCb    *gCb;
PjDlRbCb  *pjRbCb;
PjTxEnt *txEnt;
#endif
{
   S16             ret;           /* Return Value */
   U8              hdr;           /* Header Value */
   U32             macI;          /* MAC-I value to be padded to the PDU */
   Buffer          *pdu;          /* Pointer for the PDU in txEnt */

   TRC3(pjDlmProcSrb)
#ifndef ALIGN_64BIT
   RLOG_ARG1(L_DEBUG,DBG_RBID,pjRbCb->rbId, "pjDlmProcSrb(pjRbCb (), txEnt(%ld)) ", 
                              txEnt->count);
#else
   RLOG_ARG1(L_DEBUG,DBG_RBID,pjRbCb->rbId, "pjDlmProcSrb(pjRbCb (), txEnt(%d)) ",
                              txEnt->count);
#endif


   pdu    = txEnt->sdu;
   macI   = 0;
   ret    = ROK;
   hdr    = 0;

   /* Perform security operation only if configured */
   if ( pjRbCb->ueCb->secInfo.secAct && pjRbCb->ueCb->secInfo.intInfo.algoType != 0)
   {
      /* Perform integrity/ciphering */
      ret = pjDlmHdlIntProt(gCb, pjRbCb, txEnt);
      if ( ret != ROK )
      {
         RLOG_ARG2(L_ERROR, DBG_RBID, pjRbCb->rbId, "Integrity Prot failed SN [%u] HFN [%lu]", txEnt->sn, txEnt->txHfn);
      }
   }
   else
   {
      /* If security is not configured, pad the PDU with 4 bytes 
       * and deliver to the lower layer
       */
      //QU_CRITLOG(QU_LOG_MESSAGE,"PDCP:Skipping Integration");
      PJ_PACK_MACI(pdu, macI);
      ret = pjDlmHdlCiph(gCb, pjRbCb, txEnt);
/*      ret = pjDlmDeliverPdu(gCb, pjRbCb, txEnt);*/
   }

   RETVALUE(ret);
}

/**
 *
 * @brief 
 * 

 *        Handler to construct a process a DRB SDU.
 * 
 * @b Description: 
 * 
 *        1. This function performs header compression if it is
 *        configured.    @n
 *        2. If security is configured, ciphering is performed. @n
 *        3. The PDU is constructed and then delivered to the lower
 *        layer. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjDlmProcDrb
(
PjCb     *gCb,
PjDlRbCb   *pjRbCb,
PjTxEnt  *txEnt
)
#else
PUBLIC S16 pjDlmProcDrb(gCb,pjRbCb,txEnt)
PjCb     *gCb;
PjDlRbCb   *pjRbCb;
PjTxEnt  *txEnt;
#endif
{
   TRC3(pjDlmProcDrb)
#ifndef ALIGN_64BIT
   RLOG_ARG1(L_UNUSED,DBG_RBID,pjRbCb->rbId, 
         "pjDlmProcDrb(pjRbCb (), txEnt(%ld)) ", txEnt->count);
#else
   RLOG_ARG1(L_UNUSED,DBG_RBID,pjRbCb->rbId, 
         "pjDlmProcDrb(pjRbCb (), txEnt(%d)) ", txEnt->count);
#endif

   RETVALUE(pjDlmHdlCmp(gCb, pjRbCb, txEnt));
}

/**
 *
 * @brief 
 * 
 *        Handler to process the Integrity Protection Request
 * 
 * @b Description: 
 * 
 *        1. This function performs integrity protection
 *        of a SDU by prepending the header.    @n
 *        2. If ciphering is applicable, it is performed after removing
 *        the header. @n
 *        3. The PDU is constructed and then delivered to the lower
 *        layer. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjDlmHdlIntProt
(
PjCb    *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
)
#else
PUBLIC S16 pjDlmHdlIntProt(gCb,pjRbCb,txEnt)
PjCb    *gCb;
PjDlRbCb  *pjRbCb;
PjTxEnt *txEnt;
#endif
{
   S16             ret;           /* Return Value */
   U8              hdr;           /* Header Value */
   U32             macI;          /* MAC-I value to be padded to the PDU */
   Buffer          *pdu = NULLP;          /* Pointer for the PDU in txEnt */
   PjSecInp        secInp;        /* Security Input for Ciphering and Int Prot */

   TRC3(pjDlmHdlIntProt)
#ifndef ALIGN_64BIT
   RLOG_ARG1(L_DEBUG,DBG_RBID,pjRbCb->rbId, 
         "pjDlmHdlIntProt(pjRbCb (), txEnt (%ld)) ",   txEnt->count);
#else
   RLOG_ARG1(L_DEBUG,DBG_RBID,pjRbCb->rbId, 
         "pjDlmHdlIntProt(pjRbCb (), txEnt (%d)) ",  txEnt->count);
#endif

   pdu    = txEnt->sdu;
   macI   = 0;
   ret    = ROK;
   hdr    = 0;

   /* If firstDL msg after secCfg, unset the ueCb flag ans set
    * rbCb flag and set the SN
    */
   if (pjRbCb->ueCb->secInfo.selSecAct == TRUE &&
            pjRbCb->ueCb->secInfo.firstMsg == TRUE )
   {
      pjRbCb->firstDlMsg = TRUE;
      pjRbCb->firstSn    = txEnt->sn;
      pjRbCb->ueCb->secInfo.firstMsg = FALSE;
   }
   secInp.dir  = PJ_SEC_DIR_DL;
   secInp.rbId = pjRbCb->rbId;
   secInp.count= txEnt->count;

   /* Add the header and then send it for Integrity Protection */
   hdr = (U8)txEnt->sn;
   ret = SAddPreMsg((Data)hdr, pdu);
#ifndef TENB_T2K3K_SPECIFIC_CHANGES
   txEnt->sdu = pdu;
#endif

#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( ret != ROK )
   {
      RLOG1(L_ERROR, "SAddPreMsg failed  Tx Count[%lu]", txEnt->count);
      PJ_UPD_DL_VAR(gCb, pjRbCb, txEnt->count);
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(gCb,pjRbCb, txEnt, PJ_REM_MSG_FAILED);
      }
      RETVALUE(RFAILED);
   }
#endif

   txEnt->state = PJ_WAIT_FOR_INTPROT;
   ret = pjUtlIntProtReq(gCb, pjRbCb, secInp, &pdu);


   if ( ret != ROK )
   {
      RLOG1(L_ERROR, "Integrity Protection Req failed Tx Count[%lu]", txEnt->count);
      PJ_UPD_DL_VAR(gCb, pjRbCb, txEnt->count);
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(gCb,pjRbCb, txEnt, PJU_INTPROT_FAILED);
      }
      gCb->pjGenSts.numIntgPrtFails++; 
      RETVALUE(RFAILED);
   }

   /*Return and wait for call back*/
   RETVALUE(ROK);
#ifndef PJ_SEC_ASYNC
   /* Append the macI at the end */
#ifdef TENB_AS_SECURITY
   /*PJ_PACK_MACI(pdu, macI);*/
#else
   PJ_PACK_MACI(pdu, macI);
#endif
   txEnt->pdu = pdu;
#ifndef TENB_ACC
#ifndef TENB_T2K3K_SPECIFIC_CHANGES 
#ifdef SS_RBUF
//   prc_trace_format_string(0x40,3,"[%ld] ",macI);
#endif
#endif
#endif

   /* Remove the header and then only send for ciphering */
   ret = SRemPreMsg(&hdr, txEnt->pdu);
#if (ERRCLASS & ERRCLS_ADD_RES)
   if ( ret != ROK )
   {
      RLOG1(L_ERROR, "SRemPreMsg failed  Tx Count [%lu]", txEnt->count);
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
#ifndef SS_RBUF
      PJ_FREE_BUF(txEnt->sdu);
#endif 
      txEnt->sdu = NULLP;
      ret = pjDlmDeliverPdu(gCb, pjRbCb, txEnt);
   }
   else
   {
      ret = pjDlmHdlCiph(gCb, pjRbCb, txEnt);
   }
#endif
   RETVALUE(ret);
}


/**
 *
 * @brief 
 *
 *        Handler to process NULL Ciphering. 
 *
 * @b Description: 
 *        This function handles the PDU for NULL ciphering.
 *            
 *  @param[in] gCb      Global control block.
 *  @param[in] pjRbCb   RB control block.
 *  @param[in] txEnt    Transmission Entity.
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */

#ifdef ANSI
PUBLIC S16 pjDlmHdlNullCiph
(
PjCb       *gCb,
PjDlRbCb     *pjRbCb,
PjTxEnt    *txEnt
)
#else
PUBLIC S16 pjDlmHdlNullCiph(gCb,pjRbCb,txEnt)
PjCb       *gCb;
PjDlRbCb     *pjRbCb;
PjTxEnt    *txEnt;
#endif
{
   S16            ret;            /* Return Value */

   TRC3(pjDlmHdlNullCiph);

   ret=ROK;

   /* If Ciphering is enabled, txEnt->pdu will point to a mBuf */
#ifdef FLAT_BUFFER_OPT
   if(txEnt->sdu == NULLP)
   {
      ret = pjUtlCopyFbToBuf(gCb, &(txEnt->fb), &(txEnt->pdu));
      if (ret != ROK)
      {
         RLOG1(L_ERROR, "Cpoying to Flat Buf Failed Tx Count [%lu]", txEnt->count);
         PJ_UPD_DL_VAR(gCb, pjRbCb, txEnt->count);
         if ( pjRbCb->dlCb.cfmReqd)
         {
            PJ_SND_CFM(gCb,pjRbCb, txEnt, PJ_CPY_MSG_FAILED);
         }
         RETVALUE(RFAILED);
      }
   }
#endif

   if (txEnt->pdu == NULLP )
   {
      if ( pjRbCb->mode == PJ_DRB_AM
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
            || pjRbCb->mode == PJ_DRB_UM
#endif
         )
      {
         PJ_CPY_MSG(gCb,txEnt->sdu, &(txEnt->pdu), ret);
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (ret != ROK)
         {
            RLOG1(L_ERROR, "SAddMsgRef Failed Tx Count [%lu]", txEnt->count);

            PJ_UPD_DL_VAR(gCb, pjRbCb, txEnt->count);
            if ( pjRbCb->dlCb.cfmReqd)
            {
               PJ_SND_CFM(gCb,pjRbCb, txEnt, PJ_CPY_MSG_FAILED);
            }
            RETVALUE(RFAILED);
         }
#endif
      }
      else
      {
         txEnt->pdu = txEnt->sdu;
         txEnt->sdu = NULLP;
      }
   }

   RETVALUE(ret);
}

/**
 *
 * @brief  
 * 
 *        Handler to process the Compression Request.
 * 
 * @b Description: 
 * 
 *        1. This function performs header compression if configured. @n
 *        2. If ciphering is applicable, it is performed. @n
 *        3. The PDU is constructed and then delivered to the lower
 *        layer. @n
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */


#ifdef ANSI
PUBLIC S16 pjDlmHdlCmp
(
PjCb    *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
)
#else
PUBLIC S16 pjDlmHdlCmp(gCb,pjRbCb,txEnt)
PjCb    *gCb;
PjDlRbCb  *pjRbCb;
PjTxEnt *txEnt;
#endif
{
   S16            ret;         /* Return Value */
   Buffer         *opSdu;      /* Pointer for Output of Compression */

   TRC3(pjDlmHdlCmp)
#ifndef ALIGN_64BIT
   RLOG_ARG1(L_UNUSED,DBG_RBID,pjRbCb->rbId, "pjDlmHdlCmp(pjRbCb (), txEnt (%ld)) ", 
            txEnt->count);
#else
   RLOG_ARG1(L_UNUSED,DBG_RBID,pjRbCb->rbId, "pjDlmHdlCmp(pjRbCb (), txEnt (%d)) ",
            txEnt->count);
#endif

   ret     = ROK;
   opSdu   = NULLP;

   /* Perform compression if ocnfigured */
   if ( pjRbCb->rohc.hdrCmpUsed )
   {
      txEnt->pdu = txEnt->sdu;
      txEnt->state = PJ_WAIT_FOR_CMP;
      ret =   pjUtlCmpReq(gCb, pjRbCb, txEnt->pdu, &opSdu, txEnt->count);
      if ( ret != ROK )
      {

         RLOG1(L_ERROR, "Compression Req Failed Tx Count [%lu]", txEnt->count);
         PJ_UPD_DL_VAR(gCb, pjRbCb, txEnt->count);
         if ( pjRbCb->dlCb.cfmReqd)
         {
            PJ_SND_CFM(gCb,pjRbCb, txEnt, PJU_COMP_FAILED);
         }
         gCb->pjGenSts.numCmpFails++;
         RETVALUE(RFAILED);
      }
#ifdef PJ_CMP_ASYNC
      RETVALUE(ret);
#else
      txEnt->pdu = opSdu;
#endif
   }
   /* Perform ciphering if security is configured */
   ret = pjDlmHdlCiph(gCb, pjRbCb, txEnt);
   RETVALUE(ret);
}

#ifdef LTE_PAL_ENB
/**
 *  @brief This fn cheks whether the RING buffer is used and therefore DL pkts
 *         need to be enqueued before forwarding it for ciphering.
 *         Note: It acts as a dummy fn so far in case of Mindspeed pltfm
 *
 *  @details
 *      Function : pjUtlChekTxEnqReq
 *
 *          Processing Steps:
 *
 *
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 pjUtlChekTxEnqReq
(
PjCb       *gCb,
PjDlRbCb   *pjRbCb,
PjTxEnt    *txEnt
)
#else
PUBLIC S16 pjUtlChekTxEnqReq(gCb,pjRbCb, txEnt)
PjCb       *gCb;
PjDlRbCb   *pjRbCb;
PjTxEnt    *txEnt;
#endif
{
  RETVALUE(ROK);
}
#endif
/**
 * 
 * @brief  
 * 
 *        Handler to process the Ciphering Request.
 * 
 * @b Description: 
 * 
 *        1. This function performs ciphering.  @n
 *        2. If asynchronous flag is defined, the function returns . @n
 *        3. Else the function updates the PDU field in txEnt and returns. @n 
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PUBLIC S16 pjDlmHdlCiph
(
PjCb    *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
)
#else
PUBLIC S16 pjDlmHdlCiph(gCb,pjRbCb,txEnt)
PjCb    *gCb;
PjDlRbCb  *pjRbCb;
PjTxEnt *txEnt;
#endif
{
   S16            ret;         /* Return Value */
   Buffer         *ciphSdu;    /* Pointer for Output of Ciphering */
   PjSec *secInfo;

   TRC3(pjDlmHdlCiph)
#ifndef QCOM
#ifndef SS_RBUF
#ifndef TENB_ACC
   pjUtlChekTxEnqReq(gCb, pjRbCb, txEnt);
#endif   
#if CIPH_BATCH_PROC
   RETVALUE(ROK);
#endif
#endif
#endif

#ifndef ALIGN_64BIT
   RLOG_ARG1(L_UNUSED,DBG_RBID,pjRbCb->rbId, "pjDlmHdlCiph(pjRbCb (), txEnt (%ld)) ", 
            txEnt->count);
#else
   RLOG_ARG1(L_UNUSED,DBG_RBID,pjRbCb->rbId, "pjDlmHdlCiph(pjRbCb (), txEnt (%d)) ",
            txEnt->count);
#endif
   ret     = ROK;
   ciphSdu = NULLP;

   txEnt->state  = PJ_WAIT_FOR_CIPHER;


   secInfo = &(pjRbCb->ueCb->secInfo);
#ifndef QCOM
   if ( secInfo->secAct && secInfo->cipherInfo.algoType != 0)   /* If its not NULL Ciphering and Security is enabled */
#endif
   {
      if ( txEnt->pdu == NULLP )
      {
         txEnt->pdu = txEnt->sdu;
      }
      /* Note: FB is only for DRB data in DL direction. SRB data after  */
      /* integrity protection will use the NON-FB version of code.      */
      /* The below "if" check is to identify whether it is for SRB data */
      /* or DRB data.                                                   */
#ifdef CIPH_BATCH_PROC
#ifndef TENB_ACC
   pjUtlChekTxEnqReq(gCb, pjRbCb, txEnt);
#endif  
   RETVALUE(ROK);
#endif

#ifdef FLAT_BUFFER_OPT
      if (txEnt->sdu == NULLP)
      {
         ret = pjUtlCipherReqFB(gCb, pjRbCb, txEnt->count, &(txEnt->fb), &ciphSdu);
      }
      else
#endif
      {
         ret = pjUtlCipherReq(gCb, pjRbCb, txEnt->count, txEnt, &ciphSdu);
      }

      /*There can be failures in creating the input for SPACC*/
      if ( ret != ROK )
      {
         RLOG1(L_ERROR, "Ciphering Req failed Tx Count [%lu]", txEnt->count);
         PJ_UPD_DL_VAR(gCb, pjRbCb, txEnt->count);
         if ( pjRbCb->dlCb.cfmReqd)
         {
            PJ_SND_CFM(gCb,pjRbCb, txEnt, PJU_CIPHER_FAILED);
         }
         gCb->pjGenSts.numCiphFails++;
         RETVALUE(RFAILED);
      }
#ifdef TENB_AS_SECURITY
      if(isSecBatchMode)
      {
         /*This state is set on reception of call back from Cryto engine*/
         //txEnt->state = PJ_PDU_SUBMITTED;
         RETVALUE(ret);
      }
      else
#endif
      {
         txEnt->pdu = ciphSdu;
      }

   }
#ifndef QCOM
   else
   {
#ifdef SS_RBUF
      txEnt->pdu = NULLP;
#endif
      QU_CRITLOG(QU_LOG_MESSAGE,"PDCP: Handling NULL Ciphering");
      ret = pjDlmHdlNullCiph(gCb, pjRbCb, txEnt);
   }
#endif

   txEnt->state = PJ_PDU_SUBMITTED;
   ret = pjDlmDeliverPdu(gCb, pjRbCb, txEnt);


   RETVALUE(ROK);
}

#ifdef FLAT_BUFFER_OPT
/**
 * 
 * @brief  
 * 
 *        Handler to process the Ciphering Request.
 * 
 * @b Description: 
 * 
 *        1. This function performs ciphering.  @n
 *        2. If asynchronous flag is defined, the function returns . @n
 *        3. Else the function updates the PDU field in txEnt and returns. @n 
 *            
 *  @param[in] pjRbCb   PDCP control block. 
 *  @param[in] txEnt    Transmission Entity.
 *                 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */

#ifdef ANSI
PUBLIC S16 pjDlmHdlCiphFB
(
PjCb    *gCb,
PjDlRbCb  *pjRbCb,
PjTxEnt *txEnt
)
#else
PUBLIC S16 pjDlmHdlCiphFB(gCb,pjRbCb,txEnt)
PjCb    *gCb;
PjDlRbCb  *pjRbCb;
PjTxEnt *txEnt;
#endif
{
   PjSecInp       secInp;      /* Security Input for Ciphering */
   S16            ret;         /* Return Value */
   Buffer         *ciphSdu;    /* Pointer for Output of Ciphering */

   TRC3(pjDlmHdlCiphFB)
   
   ret     = ROK;
   ciphSdu = NULLP;

   secInp.dir  = PJ_SEC_DIR_DL;
   secInp.rbId = pjRbCb->rbId;
   secInp.count= txEnt->count;
   txEnt->state  = PJ_WAIT_FOR_CIPHER;

   ret = pjUtlCipherReqFB(gCb, pjRbCb, txEnt->count, &(txEnt->fb), &ciphSdu);

   /*There can be failures in creating the input for SPACC*/
   if ( ret != ROK )
   {
      RLOG1(L_ERROR, "Ciphering Req failed Tx Count [%lu]", txEnt->count);
      PJ_UPD_DL_VAR(gCb, pjRbCb, txEnt->count);
      if ( pjRbCb->dlCb.cfmReqd)
      {
         PJ_SND_CFM(gCb,pjRbCb, txEnt, PJU_CIPHER_FAILED);
      }
      gCb->pjGenSts.numCiphFails++;
      RETVALUE(RFAILED);
   }

   
   txEnt->state = PJ_PDU_SUBMITTED;


   RETVALUE(ROK);
}
#endif


/**
 *
 * @brief
 *
 *  @param[in] pjRbCb   PDCP control block.
    @param[in] PjRxEnt  *rxEnt
 *
 *  @return  Void
 */

#ifdef ANSI
PUBLIC Void pjDlmEnqueueDlPkt
(
PjCb               *gCb, 
PjDlRbCb           *pjRbCb,          /* !< PDCP Control Block */
U8                 datType,
PjuDatFwdReqInfo   *datFwd,          /* !< SN value of PDU */
PjuSduId           sduId,
Buffer             *pdu              /* !< PDU message buffer */
)
#else
PUBLIC Void pjDlmEnqueueDlPkt(gCb,pjRbCb,datType,datFwd,suId,pdu)
PjCb               *gCb; 
PjDlRbCb           *pjRbCb;          /* !< PDCP Control Block */
U8                 datType;
PjuDatFwdReqInfo   *datFwd;               /* !< SN value of PDU */
PjuSduId           sduId;
Buffer             *pdu;             /* !< PDU message buffer */
#endif
{
   PjDlPkt    *pkt;
   U32          numSdu;

   TRC2(pjDlmEnqueueDlPkt)

   if (datType == PJ_DATA_FWD_PKT)
   {
      for (numSdu=0; numSdu < datFwd->numSdus; numSdu++)
      {
          PJ_ALLOC(gCb, pkt, sizeof(PjDlPkt));
          if (pkt != NULLP)
          {
             pkt->type     = datType;
             pkt->sn       = datFwd->datFwdInfo[numSdu].sn; 
             pkt->sduId    = datFwd->datFwdInfo[numSdu].sduId;
             pkt->pdu      = datFwd->datFwdInfo[numSdu].sdu;
             pkt->lnk.node = (PTR)pkt;
             cmLListAdd2Tail (&pjRbCb->dlCb.dlPktQ, &pkt->lnk);
          }
      }
   }
   else
   {
      PJ_ALLOC(gCb, pkt, sizeof(PjDlPkt));
      if (pkt != NULLP)
      {
         pkt->type = datType;
         pkt->pdu = pdu;
         pkt->sduId = sduId;
         pkt->lnk.node = (PTR)pkt;
         cmLListAdd2Tail (&pjRbCb->dlCb.dlPktQ, &pkt->lnk);
      }
      else
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         PJ_FREE_BUF(pdu);
      }
   }

   RETVOID;
}/* end of pjDlmEnqueueHoPkt */

#ifdef FLAT_BUFFER_OPT
/**
 *
 * @brief
 *
 *  @param[in] pjRbCb   PDCP control block.
    @param[in] PjRxEnt  *rxEnt
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjDlmEnqueueDlPktFB
(
PjCb               *gCb, 
PjDlRbCb           *pjRbCb,          /* !< PDCP Control Block */
U8                 datType,
PjuDatFwdReqInfo   *datFwd,          /* !< SN value of PDU */
PjuSduId           sduId,
FlatBuffer         *pdu             /* !< Flat Buffer PDU */ 
)
#else
PUBLIC Void pjDlmEnqueueDlPktFB(gCb,pjRbCb,datType,datFwd,suId,pdu)
PjCb               *gCb; 
PjDlRbCb           *pjRbCb;          /* !< PDCP Control Block */
U8                 datType;
PjuDatFwdReqInfo   *datFwd;               /* !< SN value of PDU */
PjuSduId           sduId;
FlatBuffer         *pdu;             /* !< Flat Buffer PDU */ 
#endif
{
   PjDlPkt    *pkt;

   TRC2(pjDlmEnqueueDlPktFB)

   if (datType == PJ_DATA_FWD_PKT)
   {
      /* printf("pjDlmEnqueueDlPktFB: datType = PJ_DATA_FWD_PKT. TODO !!!!! "); */
#if 0  /* TODO: Uday. While doing HO changes, take care */
      for (numSdu=0; numSdu < datFwd->numSdus; numSdu++)
      {
          PJ_ALLOC(gCb, pkt, sizeof(PjDlPkt));
          if (pkt != NULLP)
          {
             pkt->type     = datType;
             pkt->sn       = datFwd->datFwdInfo[numSdu].sn; 
             pkt->sduId    = datFwd->datFwdInfo[numSdu].sduId;
             pkt->pdu      = datFwd->datFwdInfo[numSdu].sdu;
             pkt->lnk.node = (PTR)pkt;
             cmLListAdd2Tail (&pjRbCb->dlCb.dlPktQ, &pkt->lnk);
          }
      }
#endif
   }
   else
   {
      PJ_ALLOC(gCb, pkt, sizeof(PjDlPkt));
      if (pkt != NULLP)
      {
         pkt->type = datType;
         pkt->fb.startAddr = pdu->startAddr;
         pkt->fb.ptr = pdu->ptr;
         pkt->fb.len = pdu->len;
         pkt->pdu = NULLP;
         pkt->sduId = sduId;
         pkt->lnk.node = (PTR)pkt;
         cmLListAdd2Tail (&pjRbCb->dlCb.dlPktQ, &pkt->lnk);
         /*printf("pjDlmEnqueueDlPktFB: Enqueue DL Flat Buffer Packets. Sdu Id is %d ",
 pkt->sduId);*/
      }
      else
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         PJ_FREE_FLAT_BUF(gCb, pdu);
      }
   }

   RETVOID;
}/* end of pjDlmEnqueueDlPktFB */
#endif

/**
 *
 * @brief It processes the packets queued up in DLPktQ
 *
 * @Description
 *      This DlPktQ queues the Normal and forwarded message during
 *      reestablishment/handover. 
 *
 *  @param[in] pjCb     PDCP Instance control block.
 *  @param[in] pjRbCb   Rb Control block 
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjDlmProcessDlPktQ
(
PjCb       *gCb,
PjDlRbCb   *pjRbCb           /* !< PDCP Control Block */
)
#else
PUBLIC Void pjDlmProcessDlPktQ(gCb, pjRbCb)
PjCb       *gCb;
PjDlRbCb   *pjRbCb;          /* !< PDCP Control Block */
#endif
{
   PjDlPkt *pkt;
   CmLList *node;
   S16     ret;   /*KW_FIX*/

   TRC2(pjDlmProcessDlPktQ)

   /* first send the status report if any enqueued */

   if (pjRbCb->dlCb.staRep != NULL)
   {
      pjDlmSendDatReq(gCb, pjRbCb, 0xffffffff, pjRbCb->dlCb.staRep);
      pjRbCb->dlCb.staRep = NULL;
   }

   CM_LLIST_FIRST_NODE(&(pjRbCb->dlCb.dlPktQ), node);
   while (node != NULLP)
   {
      pkt = (PjDlPkt *) node->node;
      if (pkt->type == PJ_DATA_FWD_PKT)
      {
         PjuDatFwdReqInfo datFwdReq;
         PjuDatFwdInfo     datFwd;
         datFwdReq.numSdus = 1;
         datFwd.sn =  pkt->sn;
         datFwd.sdu = pkt->pdu;
         datFwd.sduId = pkt->sduId;
         datFwdReq.datFwdInfo = &datFwd;
         pjDlmHndlDatFwdReq(gCb, pjRbCb, &datFwdReq);
      }
      else
      {
#ifdef FLAT_BUFFER_OPT
         if((pkt->fb.len !=0) && (pkt->fb.ptr != NULLP))
         {
            ret = pjDlmProcessSdusFB(gCb, pjRbCb, &(pkt->fb), pkt->sduId, pjRbCb->dlCb.nxtTxSn,  pjRbCb->dlCb.txHfn);
         }
         else
#endif
         {
            ret = pjDlmProcessSdus(gCb, pjRbCb, pkt->pdu, pkt->sduId, pjRbCb->dlCb.nxtTxSn,  pjRbCb->dlCb.txHfn);
         }
         if ( ret != ROK )
         {
#if (ERRCLASS & ERRCLS_INT_PAR)
             RLOG_ARG1(L_ERROR, DBG_RBID, pjRbCb->rbId, "Processing of SDU [%lu] failed", pkt->sduId);
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         }
      }

      cmLListDelFrm(&(pjRbCb->dlCb.dlPktQ), node);
      PJ_FREE(gCb, pkt, sizeof (PjDlPkt));
      node = NULLP;
      CM_LLIST_FIRST_NODE(&(pjRbCb->dlCb.dlPktQ), node);
  }

   RETVOID;
}/* end of pjDlmProcessDlPktQ */




#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
/**
 *
 * @brief Handler for  downlink off-board timer expiry.
 *       
 *
 * @b Description
 *        This function is called when the off-board timer expires.
 *        This function discards the txEnt of dlCb.obdCount if it is 
 *        is not submitted and sends the constructed PDUs to the lower
 *        layer and start the timer for the next valid txEnt.
 *
 *  @param[in] pjRbCb    PDCP control block.
 *
 * 
 *  @return  S16
 *      -# ROK 
 */
#ifdef ANSI
PUBLIC S16 pjDlmObdTmrExp
(
PjCb   *gCb,
PjDlRbCb *pjRbCb
)
#else
PUBLIC S16 pjDlmObdTmrExp(gCb, pjRbCb)
PjCb    *gCb;
PjDlRbCb  *pjRbCb;
#endif
{
   U32           count;
   U32           curCount;
   PjTxEnt       *txEnt;
   PjuDatCfmInfo *datCfm;
   S16           ret;

   TRC3(pjDlmObdTmrExp)
   
   RLOG_ARG0(L_DEBUG,DBG_RBID,pjRbCb->rbId, "pjDlmObdTmrExp(pjRbCb()) ");

   count      = pjRbCb->dlCb.obdCount;
   curCount   = pjRbCb->dlCb.count;
   txEnt      = pjDbmGetTxEnt(gCb, &(pjRbCb->dlCb.txBuf), count);
   datCfm     = NULLP;
   ret        = ROK;

   if ( txEnt && txEnt->state != PJ_PDU_SUBMITTED )
   {
#ifndef ALIGN_64BIT
      RLOG_ARG1(L_DEBUG,DBG_RBID,pjRbCb->rbId, 
            "pjDlmObdTmrExp(pjRbCb()) : discarding txEnt withcount(%ld)",
                         txEnt->count);
#else
      RLOG_ARG1(L_DEBUG,DBG_RBID,pjRbCb->rbId, 
            "pjDlmObdTmrExp(pjRbCb()) : discarding txEnt withcount(%d)",
                         txEnt->count);
#endif
      PJ_UPD_DL_VAR(gCb, pjRbCb, txEnt->count);
      PJ_SND_CFM(gCb,pjRbCb, txEnt, PJ_OBD_TIMEOUT);
      gCb->pjGenSts.numSdusDiscObdTmrExp++;
      count++;
      while ( count <= curCount )
      {
         txEnt = pjDbmGetTxEnt(gCb, &(pjRbCb->dlCb.txBuf), count);
         if ( txEnt != NULLP )
         {
            if ( txEnt->state == PJ_PDU_CONSTRUCTED )
            {
               ret = pjDlmDeliverPdu(gCb, pjRbCb, txEnt);
               count = pjRbCb->dlCb.nxtToSub;
            }
            break;
         }
         else
         {
            PJ_UPD_DL_VAR(gCb, pjRbCb, count);
         }
         count++;
      }
   }
   else
   {
      count = pjRbCb->dlCb.nxtToSub;
   }
   if ( pjDbmGetTxEnt(gCb, &(pjRbCb->dlCb.txBuf), count) != NULLP )
   {
      pjRbCb->dlCb.obdCount = count;
      pjStartTmr(gCb, (PTR)pjRbCb, PJ_EVT_DL_OBD_TMR);
   }

   RETVALUE(ret);
}

#endif

/*@}*/
/********************************************************************30**
 *          End of file:     eq_dlm.c@@/main/TeNB_Main_BR/14 - Mon Aug 11 16:42:47 2014
 **********************************************************************31*/

/********************************************************************40**
 *   
 *           Notes:
 *             
 *********************************************************************41*/

/********************************************************************50**
 *
 * *********************************************************************51*/

/********************************************************************60**
 *   
 *           Revision history:
 *             
 **********************************************************************61*/

/********************************************************************90**
 * ver       pat    init                  description
 * ------------ -------- ---- ----------------------------------------------
 * /main/1   ---       vp   1. LTERLC Release 2.1
/main/2      kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/3      kw004.201 av   1.Adding of Missing Trace in LTE RLC PDCP
/main/4      kw005.201 ap  1. Moved definition of PJ_ASYNC_WIN to
                              kw.h file
                           2. Added support for L2 Measurement.
                           3. Sending Data forward indication when numSdu=0
*********************************************************************91*/

