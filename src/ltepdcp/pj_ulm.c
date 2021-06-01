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
  
     Desc:     Source code for PDCP Uplink module.
               This file contains following functions
                  -- pjUlmHdlSrbPkt
                  -- pjUlmHdlDrbPkt
                  -- pjUlmProcessSrb
                  -- pjUlmProcessDrbUm
                  -- pjUlmProcessDrbAm
                  -- pjUlmDeliverSrb
                  -- pjUlmDeliverDrbUm
                  -- pjUlmDeliverDrbAm
                  -- pjUlmReEstSrb
                  -- pjUlmReEstDrbAm
                  -- pjUlmBldStaRep
                  -- pjUlmHdlDatFwdReq
                  -- pjUlmHdlObdTmrExpiry

     File:     gp_pj_ulm.c

     Sid:      eq_ulm.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:53 2015

     Prg:      mn 

**********************************************************************/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=244;
/** @file gp_pj_ulm.c
@brief PDCP Uplink module
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
#include "lpj.h"                /* RRC layer */
#include "pj_env.h"             /* RLC environment options */
#include "pj.h"                 /* RLC layer */
#include "pj_ul.h"
#include "pj_err.h"
#include "pj_ptsec.h"



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
#include "lpj.x"                /* LM Interface */
#include "pj.h"                /* LM Interface */
#include "pj.x"                 /* RLC layer */
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_dl.x"
#include "pj_ul.x"
 

/* local defines */

/* local externs */
#ifdef TENB_AS_SECURITY
EXTERN U8 isSecBatchMode;
#endif

/* forward references */
PUBLIC S16 pjUtlChekTxEnqReq(PjCb *gCb, PjDlRbCb *pjRbCb, PjTxEnt *txEnt);
PUBLIC S16 pjUtlChekRxEnqReq(PjCb *gCb, PjUlRbCb *pjRbCb, PjRxEnt *rxEnt);

/* public variable declarations */
#ifdef QCOM_STATS
PUBLIC U32 pjUlrate_pju;
PUBLIC U32 pjUlrate_kwu;
#endif

/* This structure holds all the global structs we need. */

/* private variable declarations */

/* private function declarations */

PRIVATE S16 pjUlmDrbFetchSn 
(
PjCb     *gCb,
PjUlRbCb *pjRbCb,               /* !< PDCP Control Block */      
Buffer   *pdu,                    /* !< PDU Buffer */ 
U8        hdrByte,               /* !< one byte extracted from pdu */
PjSn     *sn                     /* PDCP SN*/
);

/** @addtogroup uldata */
/*@{*/

/*****************************************************************************
 *    HEADER PARSING FUNCTIONS
 ****************************************************************************/

PRIVATE Void pjUlmEnqueueUlPkt ARGS(( PjCb *gCb, PjUlRbCb   *pjRbCb,PjSn      sn, Buffer   *pdu));



PUBLIC S16 pjUlmHndlDlFdbk ARGS ((
PjCb     *gCb,
PjUlRbCb   *pjRbCb,
Buffer   *mBuf
));
/**
 *
 * @brief 
 *
 *        Function to extract the SRB header.
 *
 * @b Description: 
 *
 *        This function extracts the SN and the MAC-I from the pdu buffer and
 *        places them in the SrbHdr structure.
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] pdu      PDU to be processed
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */

#ifdef ANSI
PUBLIC S16 pjUlmHdlSrbPkt
(
PjCb   *gCb,
PjUlRbCb *pjRbCb,               /* !< PDCP Control Block */  
Buffer *pdu                   /* !< PDU Buffer */ 
)
#else
PUBLIC S16 pjUlmHdlSrbPkt(gCb, pjRbCb, pdu)
PjCb    *gCb;
PjUlRbCb  *pjRbCb;               /* !< PDCP Control Block */      
Buffer  *pdu;                  /* !< PDU Buffer */ 
#endif
{
   S16    ret     = ROK;    /* Return Value */
   PjSn   sn;               /* SN value */
   U8     hdrByte;          /* First byte storing hdr values */
   U8     res;              /* Reserved values */

   TRC2(pjUlmHdlSrbPkt);


   /* Initialistaions */

   /* Get the first byte */
   ret = SRemPreMsg(&hdrByte, pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR, DBG_UEID,pjRbCb->ueCb->key.ueId," SRemPreMsg Failed ");
      PJ_FREE_BUF(pdu);
      PJ_STS_INC_GEN_CNT(gCb,errorPdusRecv);
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   /* Verify that reserved values are zero */
   res = (hdrByte >> PJ_SRB_SN_LEN);
   if(res != 0)
   {
      RLOG_ARG0(L_ERROR,DBG_UEID,pjRbCb->ueCb->key.ueId,
                     "Reserved Values Non-zero ");
      PJ_FREE_BUF(pdu);
      PJ_STS_INC_GEN_CNT(gCb,errorPdusRecv);
      RETVALUE(RFAILED);
   }

   /* Directly assigning since the reserved values are zero */
   sn = hdrByte;

   /* Start processing */
   ret =  pjUlmProcessSrb(gCb, pjRbCb, sn, pdu);

   RETVALUE(ret);
}/* pjUlmHdlSrbPkt */

/**
 *
 * @brief 
 *
 *        Handler to forward the status report to PDCP-DL.
 *
 * @b Description: 
 *
 *        1. This function is called when a status report is received from the
 *        peer.    @n
 *        2. This function forwards the status report to PDCP-DL.  @n
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
PUBLIC S16 pjUlmHndlDlStaRep
(
PjCb     *gCb,
PjUlRbCb   *pjRbCb,
PjSn     fms,
Buffer   *staPdu
)
#else
PUBLIC S16 pjUlmHndlDlStaRep(gCb, pjRbCb, fms, staPdu)
PjCb     *gCb;
PjUlRbCb   *pjRbCb;
PjSn     fms,
Buffer   *staPdu;
#endif
{
   UdxDlStaRepInfo      *staRep;
   PjUdxUlSapCb         *udxSap;
   
   TRC3(pjUlmHndlDlStaRep)

   udxSap = PJ_GET_UL_UDX_SAP(gCb);

   /* Memory leak fix ccpu00135359 */
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&staRep,
      sizeof (UdxDlStaRepInfo)) != ROK)
   {
      staRep = NULLP;
   }

   if (NULLP != staRep)
   {
      staRep->pdcpId.cellId = pjRbCb->ueCb->key.cellId;
      staRep->pdcpId.ueId = pjRbCb->ueCb->key.ueId;
      staRep->pdcpId.rbId = pjRbCb->rbId;
      staRep->pdcpId.rbType = pjRbCb->rbType;

      staRep->fms = fms;
      staRep->staPdu = staPdu;
      
      PjUlUdxDlStaRep(&(udxSap->pst), udxSap->spId, staRep);
   }
   else
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Handler to forward the ROHC feedback packet received from UE to PDCP-DL.
 *
 * @b Description: 
 *
 *        1. This function is called when a ROHC feedback packet received from
 *        peer.    @n
 *        2. This function forwards the ROHC feedback packet to PDCP-DL.  @n
 *            
 *  @param[in] gCb           global control block.
 *  @param[in] pjRbCb      PDCP control block. 
 *  @param[in] mBuf         ROHC feedback packet.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */

#ifdef ANSI
PUBLIC S16 pjUlmHndlDlFdbk
(
PjCb     *gCb,
PjUlRbCb   *pjRbCb,
Buffer   *mBuf
)
#else
PUBLIC S16 pjUlmHndlDlFdbk(gCb, pjRbCb, mBuf)
PjCb     *gCb;
PjUlRbCb   *pjRbCb;
Buffer   *mBuf;
#endif
{
   UdxDlFdbkPktInfo *fbPkt;
   PjUdxUlSapCb     *udxSap;
   
   TRC3(pjUlmHndlDlFdbk)
   
   udxSap = PJ_GET_UL_UDX_SAP(gCb);

   /* Memory leak fix ccpu00135359 */
   if(SGetSBuf(udxSap->pst.region,udxSap->pst.pool,(Data **)&fbPkt,
      sizeof (UdxDlFdbkPktInfo)) != ROK)
   {
      fbPkt = NULLP;
   }

   if (NULLP != fbPkt)
   {
      fbPkt->pdcpId.cellId = pjRbCb->ueCb->key.cellId;
      fbPkt->pdcpId.ueId = pjRbCb->ueCb->key.ueId;
      fbPkt->pdcpId.rbId = pjRbCb->rbId;
      fbPkt->pdcpId.rbType = pjRbCb->rbType;

      fbPkt->fbPkt= mBuf;
      
      PjUlUdxDlFdbkPktInfo(&(udxSap->pst), udxSap->spId, fbPkt);
   }
   else
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Function to extract the DRB SN from hdr.
 *
 * @b Description: 
 *
 *        This function extracts the SN in case of a data packet 
 *
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] pdu      PDU to be processed
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PRIVATE S16 pjUlmDrbFetchSn 
(
PjCb     *gCb,
PjUlRbCb *pjRbCb,               /* !< PDCP Control Block */      
Buffer   *pdu,                    /* !< PDU Buffer */ 
U8        hdrByte,               /* !< one byte extracted from pdu */
PjSn     *sn                     /* PDCP SN*/
)
#else
PRIVATE S16 pjUlmDrbFetchSn(gCb, pjRbCb, pdu, hdrByte, sn)
PjCb     *gCb;
PjUlRbCb *pjRbCb;               /* !< PDCP Control Block */      
Buffer   *pdu;                    /* !< PDU Buffer */ 
U8        hdrByte;              /* !< one byte extracted from pdu */
PjSn     *sn;                   /* PDCP SN*/
#endif
{
   U8     res;              /* Reserved values */
   S16    ret     = ROK;    /* Return Value */
   /* Pack the SN */
   if(pjRbCb->snLen == PJ_7_BIT_SN)
   {
      *sn   =  (U16)(hdrByte & (0x7f)); /*KW_FIX*/
   }
   else if (pjRbCb->snLen == PJ_12_BIT_SN) /* Its 12 bit */ 
   {
      /* Verify that reserved values are zero */
      res = (hdrByte >> 4);
      if (res != 8)
      {
         PJDBGP(gCb,(PJ_DBGMASK_ULM | PJ_DBGMASK_ERROR),
               (gCb->init.prntBuf," Reserved Values Non-zero \n"));
         RETVALUE(RFAILED);
      }

      *sn = (U16)(hdrByte & PJ_12_BIT_SN_MSB_MASK); /*KW_FIX*/
      *sn = *sn << PJ_BYTE_SIZE;
      ret = SRemPreMsg(&hdrByte, pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
      if (ret != ROK)
      {
         PJDBGP(gCb,(PJ_DBGMASK_ULM | PJ_DBGMASK_ERROR),
               (gCb->init.prntBuf," SRemPreMsg Failed \n"));
         RETVALUE(RFAILED);
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      *sn |= hdrByte;
   }
   else if (pjRbCb->snLen == PJ_15_BIT_SN) /* Its 15 bit */ 
   {
      *sn = (U16)(hdrByte & PJ_15_BIT_SN_MSB_MASK); /*KW_FIX*/
      *sn = *sn << PJ_BYTE_SIZE;
      ret = SRemPreMsg(&hdrByte, pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
      if (ret != ROK)
      {
         PJDBGP(gCb,(PJ_DBGMASK_ULM | PJ_DBGMASK_ERROR),
               (gCb->init.prntBuf," SRemPreMsg Failed \n"));
         RETVALUE(RFAILED);
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      *sn |= hdrByte;
   }
   RETVALUE(ROK);
}

/**
 *
 * @brief 
 *
 *        Function to extract the DRB header.
 *
 * @b Description: 
 *
 *        This function extracts the SN in case of a data packet 
 *        or feedback packet and the FMS and Bitmap incase of a 
 *        PDCP status packet places them in the DrbHdr structure.
 *
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] pdu      PDU to be processed
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmHdlDrbPkt
(
PjCb   *gCb,
PjUlRbCb *pjRbCb,               /* !< PDCP Control Block */      
Buffer *pdu,                    /* !< PDU Buffer */ 
Bool    isOutOfSeq              /*!< To indicate whether this packet is in-sequence or not */
)
#else
PUBLIC S16 pjUlmHdlDrbPkt(gCb, pjRbCb, pdu, isOutOfSeq)
PjCb   *gCb;
PjUlRbCb *pjRbCb;               /* !< PDCP Control Block */      
Buffer *pdu;                    /* !< PDU Buffer */ 
Bool    isOutOfSeq;             /* !< To indicate whether this packet is in-sequence or not */
#endif
{
   S16    ret     = ROK;    /* Return Value */
   PjSn   sn = 0;               /* SN value */
   U8     hdrByte;          /* First byte storing hdr values */
   U8     pduType;          /* Type of PDU */
   U8     res;              /* Reserved values */

   TRC2(pjUlmHdlDrbPkt)

#ifndef QCOM_STATS
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
extern U32 ulrate_kwu;
MsgLen len;
SFndLenMsg(pdu, &len);
ulrate_kwu += len;
#endif
#endif
#endif
#ifdef QCOM_STATS
   {
      MsgLen len;
      SFndLenMsg(pdu, &len);
      pjUlrate_kwu += len;
      //RsysSHMLog("\nData at PDCP len %d \n", len);
   }
#endif
   /* Get the first byte */
   ret = SRemPreMsg(&hdrByte, pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      RLOG_ARG0(L_DEBUG, DBG_UEID,pjRbCb->ueCb->key.ueId," SRemPreMsg Failed ");
      PJ_FREE_BUF(pdu);
      PJ_STS_INC_GEN_CNT(gCb,errorPdusRecv);
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   /* check if its a data PDU */
   if((hdrByte & PJ_TYPE_DRB_DATA) == PJ_TYPE_DRB_DATA)
   {
      ret = pjUlmDrbFetchSn (gCb, pjRbCb, pdu, hdrByte, &sn);
#if (ERRCLASS & ERRCLS_DEBUG)
      if (ret != ROK)
      {
         PJDBGP(gCb,(PJ_DBGMASK_ULM | PJ_DBGMASK_ERROR),
               (gCb->init.prntBuf," Failed to extract SN from PDU\n"));
         PJ_FREE_BUF(pdu);
         PJ_STS_INC_GEN_CNT(gCb,errorPdusRecv);
         RETVALUE(RFAILED);
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      /* Call the processing function */
      if(pjRbCb->mode == PJ_DRB_AM)
      {
         if (pjRbCb->state == PJ_STATE_HO)
         {
             pjUlmEnqueueUlPkt(gCb,pjRbCb,sn,pdu);
         }
         else
         {
           ret = pjUlmProcessDrbAm(gCb, pjRbCb, sn, pdu, isOutOfSeq);
         }
      }
      else /* (pjRbCb->type == PJ_DRB_UM) */
      {
         ret = pjUlmProcessDrbUm(gCb, pjRbCb, sn, pdu);
      }
   }
   else  /* Its a control PDU */
   {

      pduType =  (U8)((hdrByte & PJ_PDU_BIT_MASK) >> 4);  /*KW_FIX*/

      if(pduType == PJ_TYPE_STATUS_REPORT)
      {
         if (pjRbCb->snLen == PJ_12_BIT_SN) /* Its 12 bit */ 
         {
            /* Copy the FMS */
            sn = (U16)(hdrByte & PJ_12_BIT_SN_MSB_MASK); /*KW_FIX*/
            sn   =  sn << PJ_BYTE_SIZE;
            ret = SRemPreMsg(&hdrByte, pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
            if(ret != ROK)
            {
               RLOG_ARG0(L_DEBUG, DBG_UEID,pjRbCb->ueCb->key.ueId,"SRemPreMsg Failed");
               PJ_FREE_BUF(pdu);
               RETVALUE(RFAILED);
            }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
            sn   |=  hdrByte;
         }
         else if (pjRbCb->snLen == PJ_15_BIT_SN) /* Its 15 bit */ 
         {
            /* Copy the FMS */
            ret = SRemPreMsg(&hdrByte, pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
            if(ret != ROK)
            {
               RLOG_ARG0(L_DEBUG, DBG_UEID,pjRbCb->ueCb->key.ueId,"SRemPreMsg Failed");
               PJ_FREE_BUF(pdu);
               RETVALUE(RFAILED);
            }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
            sn = (U16)(hdrByte & PJ_15_BIT_SN_MSB_MASK); /*KW_FIX*/
            sn   =  sn << PJ_BYTE_SIZE;
            ret = SRemPreMsg(&hdrByte, pdu);
#if (ERRCLASS & ERRCLS_DEBUG)
            if(ret != ROK)
            {
               RLOG_ARG0(L_DEBUG, DBG_UEID,pjRbCb->ueCb->key.ueId,"SRemPreMsg Failed");

               PJ_FREE_BUF(pdu);
               RETVALUE(RFAILED);
            }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
            sn   |=  hdrByte;
         }
         ret = pjUlmHndlDlStaRep(gCb, pjRbCb, sn, pdu);
         if (ret != ROK)
         {
            RLOG_ARG0(L_DEBUG,DBG_UEID,pjRbCb->ueCb->key.ueId,
                          " pjUlmHndlDlStaRep() returned Failed ");
            PJ_FREE_BUF(pdu);
            RETVALUE(RFAILED);
         }
      }
      else if(pduType == PJ_TYPE_ROHC_FEEDBACK)
      {

         /* validate that reserved values are 0*/
         res  =  (U8)(hdrByte & PJ_CPDU_RES_VAL_MASK);  /*KW_FIX*/
         if(res != 0)
         {
            RLOG_ARG0(L_DEBUG,DBG_UEID,pjRbCb->ueCb->key.ueId,
                        " Reserved values non-null ");
            PJ_FREE_BUF(pdu);
            PJ_STS_INC_GEN_CNT(gCb,errorPdusRecv);
            RETVALUE(RFAILED);
         }

         ret = pjUlmHndlDlFdbk(gCb, pjRbCb, pdu);
         if (ret != ROK)
         {
            RLOG_ARG0(L_DEBUG,DBG_UEID,pjRbCb->ueCb->key.ueId,
                  " pjUlmHndlDlStaRep() returned Failed ");
            PJ_FREE_BUF(pdu);
            RETVALUE(RFAILED);
         }
      }
      else
      {
         /* Invalid PDU type */
         RLOG_ARG0(L_DEBUG,DBG_UEID,pjRbCb->ueCb->key.ueId,
                         " Invalid PDU Type ");
         PJ_FREE_BUF(pdu);
         PJ_STS_INC_GEN_CNT(gCb,errorPdusRecv);
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ret);
}/* pjUlmHdlDrbPkt */

/*****************************************************************************
 *          PROCESSING FUNCTIONS
 ****************************************************************************/

/**
 *
 * @brief 
 *
 *        This function is called after header extraction to process the SRB pdu.
 *
 * @b Description: 
 *
 *        This function performs the follwing functions:                   @n
 *        1. Create and Insert the entry in the reception buffer.          @n
 *        2. Update the nxtRxSn and rxHfn values.                          @n
 *        3. Call pjUlmHdlDecipher to perform deciphering.                 @n
 *        4. Call pjUlmHdlIntVer to perform integrity verification.        @n
 *        5. Call pjUlmDeliverSrb for performing state updations and 
 *           submitting to upper layers.                                   @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] sn       SN value of the pdu
 *  @param[in] pdu      PDU message buffer.
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmProcessSrb
(
PjCb     *gCb,
PjUlRbCb   *pjRbCb,           /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu              /* !< PDU message buffer */           
)
#else
PUBLIC S16 pjUlmProcessSrb(gCb, pjRbCb, sn, pdu)
PjCb      *gCb;
PjUlRbCb    *pjRbCb;          /* !< PDCP Control Block */     
PjSn      sn;              /* !< SN value of PDU */           
Buffer   *pdu;             /* !< PDU message buffer */           
#endif
{
   PjUlCb   *ulCb;          /* PTR to UL Control Block        */
   PjRxEnt  *rxEnt;         /* Ptr to Rx Entry                */
   S16       ret     = ROK; /* Return value                   */
   U32       count;         /* Count value */

   TRC2(pjUlmProcessSrb)


   /* 1. Initialisations */
   ulCb  = &pjRbCb->ulCb;

   /* 2. Check for wrap around and update the Hfn */
   /* Set the nxtSubCnt value */
   PJ_GET_COUNT(count, PJ_SRB_SN_LEN, sn, (ulCb->rxHfn));

   /* 3. Check for wrap around and update the Hfn */
   PJ_ULM_UPD_RX_VAR(sn, pjRbCb);

   /* 4. Check for wrap around and update the Hfn */
   PJ_ULM_UPD_NXT2SUB(ulCb, count);


   /* 5. Create and insert the entry in the buffer */
   PJ_ALLOC(gCb,rxEnt, sizeof(PjRxEnt));
#if (ERRCLASS & ERRCLS_DEBUG)
   if(rxEnt == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   rxEnt->count   =  count;
   rxEnt->state   =  PJ_RDY_TO_DCIPHER;
   rxEnt->mBuf    =  pdu;
   /*Insert the entry in the buffer */
   ret = pjDbmInsRxEnt(gCb, &ulCb->recBuf, rxEnt, FALSE); 

   /* 4. Process for deciphering */
   ret = pjUlmHdlDecipher(gCb, pjRbCb, rxEnt);

   RETVALUE(ret);
}/*pjUlmProcessSrb  */


/**
 *
 * @brief 
 *
 *        Function to process the DRB UM Pdu. It is called from pjUlmHdlDrbPkt.
 *
 * @b Description: 
 *
 *        This function performs the follwing steps:                       @n
 *        1. Create and Insert the entry in the reception buffer.          @n
 *        2. Check for firstSn in reestablish state.                       @n
 *        3. Update nxtSubCnt and nxtSubDeCmp.                             @n
 *        4. Call pjUtlDecipher to perform deciphering.                    @n
 *        5. Call pjUtlDecomp to perform integrity verification.           @n
 *        6. Call pjUlmDeliverDrbUm for performing state updations and 
 *           submitting to upper layers.                                   @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] sn       SN value of the pdu
 *  @param[in] pdu      PDU message buffer.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmProcessDrbUm
(
PjCb     *gCb,
PjUlRbCb   *pjRbCb,          /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu              /* !< PDU message buffer */           
)
#else
PUBLIC S16 pjUlmProcessDrbUm(gCb, pjRbCb, sn, pdu)
PjCb      *gCb;
PjUlRbCb    *pjRbCb;         /* !< PDCP Control Block */     
PjSn      sn;              /* !< SN value of PDU */           
Buffer   *pdu;             /* !< PDU message buffer */           
#endif
{
   PjUlCb       *ulCb;            /* PTR to UL Control Block     */
   PjRxEnt      *rxEnt;           /* Ptr to Rx Entry             */
   S16          ret      = ROK;   /* Return value                */
   U32          count;           /* Count value */
   U32          oldRxCnt;        /* Old Count value used for decomp */

   TRC2(pjUlmProcessDrbUm)

   /* 1. Initialisations */
   sn    =  sn;
   ulCb  = &pjRbCb->ulCb;
   PJ_GET_COUNT(oldRxCnt, (pjRbCb->snLen), (ulCb->nxtRxSn), (ulCb->rxHfn));

   /* 2. Check for wrap around */
   PJ_ULM_UPD_RX_VAR(sn, pjRbCb);
   PJ_GET_COUNT(count, (pjRbCb->snLen), sn, (ulCb->rxHfn));
   PJ_ULM_UPD_NXT2DCOMP(ulCb, (pjRbCb->snLen), sn, oldRxCnt);
   PJ_ULM_UPD_NXT2SUB(ulCb, count); 

   /* 3. Create the entry */
   PJ_ALLOC(gCb,rxEnt, sizeof(PjRxEnt));
#if (ERRCLASS & ERRCLS_DEBUG)
   if(rxEnt == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   /* 4. Fill values and Insert into hash list */
   rxEnt->count   =  count;
   rxEnt->state   =  PJ_RDY_TO_DCIPHER;
   rxEnt->mBuf    =  pdu;
   ret = pjDbmInsRxEnt(gCb, &ulCb->recBuf, rxEnt, FALSE); 

   /* 5. Update OBD count */
   PJ_INC_OBD_COUNT(pjRbCb, rxEnt->count);

   /* 6. Send for deciphering */
   ret = pjUlmHdlDecipher(gCb, pjRbCb, rxEnt);

   RETVALUE(ret);
}/* pjUlmProcessDrbUm */


/**
 *
 *        Function to process the DRB AM Pdu. It is called from pjUlmHdlDrbPkt.
 *
 * @b Description: 
 *
 *        This function performs the follwing functions:                                            @n
 *        1. Create and Insert the entry in the reception buffer.                                   @n
 *        2. Perform checks to see if its a valid PDU and calculate RX_HFN                          @n
 *        3. Check for firstSn in reestablish state.                                                @n
 *        4. Update nxtSubCnt and nxtSubDeCmp.                                                      @n
 *        5. Call pjUtlDecipher to perform deciphering.                                             @n
 *        6. Call pjUtlDecomp to perform integrity verification.                                    @n
 *        7. Call pjUlmDeliverDrbUm for performing state updations and submitting to upper layers.  @n
 * @brief 
 *
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] sn       SN value of the pdu
 *  @param[in] pdu      PDU message buffer.
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmProcessDrbAm
(
PjCb     *gCb,
PjUlRbCb   *pjRbCb,           /* !< PDCP Control Block */   
PjSn     sn,               /* !< SN value of PDU */           
Buffer   *pdu,              /* !< PDU message buffer */           
Bool     isOutOfSeq         /* !< To indicate whether packet received is in-sequence or not */
)
#else
PUBLIC S16 pjUlmProcessDrbAm(gCb, pjRbCb, sn, pdu, isOutOfSeq)
PjCb     *gCb;
PjUlRbCb   *pjRbCb;          /* !< PDCP Control Block */     
PjSn     sn;              /* !< SN value of PDU */           
Buffer   *pdu;             /* !< PDU message buffer */     
Bool     isOutOfSeq;       /* !< To indicate whether packet received is in-sequence or not */
#endif
{
   PjUlCb       *ulCb;            /* PTR to UL Control Block     */
   PjRxEnt      *rxEnt;           /* Ptr to Rx Entry             */
   S16          ret      = ROK;   /* Return value                */
   U32          ctxHfn;           /* Context Hfn for lib fn      */  /*KW_FIX*/
   U32          oldRxCnt;        /* Old Count value used for decomp */
   Bool         discFlag;         /* Tmp Flag to store the value from rxEnt */
   Bool         dupEntry;         /* Tmp Flag to store the value from rxEnt */

   TRC2(pjUlmProcessDrbAm);

   /* 1. Initialisations */
   ulCb   = &pjRbCb->ulCb;
   ctxHfn = ulCb->rxHfn;
   PJ_GET_COUNT(oldRxCnt, (pjRbCb->snLen), (ulCb->nxtRxSn), (ulCb->rxHfn));

   /* 2. Allocate memory for the rxEnt */
   PJ_ALLOC_WC(gCb,rxEnt, sizeof(PjRxEnt));
#if (ERRCLASS & ERRCLS_DEBUG)
   if(rxEnt == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   rxEnt->lstEnt.next = NULLP;
   rxEnt->lstEnt.prev = NULLP;
   rxEnt->lstEnt.node = NULLP;
   rxEnt->discFlag = rxEnt->dupEntry = 0;
   rxEnt->isOutSeq = isOutOfSeq;

   /* 3. Perform validations */
   /* 3.1 A SN already delivered to the upper layer has come */
   if(PJ_CHK_SN_OUTSIDE_WIN(ulCb, sn, pjRbCb->snLen))
   {
      rxEnt->discFlag = TRUE;
      if(sn > ulCb->nxtRxSn)
      {
         ctxHfn --;
      }
   }

   /* 3.2 Wrap around case */
   else if((ulCb->nxtRxSn - sn) > PJ_DRB_AM_WINSIZE (pjRbCb->snLen))
   {
      ulCb->rxHfn ++;
      ctxHfn ++;
      ulCb->nxtRxSn = (U16)(sn + 1);  /*KW_FIX*/
      PJ_ULM_UPD_NXT2DCOMP(ulCb, (pjRbCb->snLen), sn, oldRxCnt);

   }
   /* 3.3 A missed out packet arriving after wrap around */
   else if((sn - ulCb->nxtRxSn) > PJ_DRB_AM_WINSIZE(pjRbCb->snLen))
   {
         ctxHfn --;
   }
   /* 3.4 normal scenario */
   else if(sn >=  ulCb->nxtRxSn)
   {

      ulCb->nxtRxSn = (U16)(sn + 1); /*KW_FIX*/
      if(ulCb->nxtRxSn ==  (1 << pjRbCb->snLen))
      {
         ulCb->nxtRxSn = 0;
         ulCb->rxHfn ++;
      }
   }
  
    /* Check whether the received packet is in-sequence or not
    * For every inSeq packet store the FMS Count value corresponding
    * to the FMS + HFN now */
   if(isOutOfSeq == FALSE)
   {
      PJ_GET_COUNT(pjRbCb->ulCb.fmsCount, pjRbCb->snLen, pjRbCb->ulCb.nxtRxSn, pjRbCb->ulCb.rxHfn);
   }

   /* 4. Fill up the structures and insert into hashlist */
   rxEnt->count   =  (ctxHfn << pjRbCb->snLen) | sn;
   rxEnt->state   =  PJ_RDY_TO_DCIPHER;
   rxEnt->mBuf    =  pdu;

   PJ_ULM_UPD_NXT2SUB(ulCb, rxEnt->count); 
   ret = pjDbmInsRxEnt(gCb, &ulCb->recBuf, rxEnt, TRUE); 
   if(ret == ROKDUP)
   {
      /* Duplicates are not to be inserted */
      /* marked to be freed up later */
      rxEnt->dupEntry =  TRUE;
   }

   /* 5. Update OBD count */
   if((rxEnt->dupEntry != TRUE) &&
      (rxEnt->discFlag != TRUE))
   {
      PJ_INC_OBD_COUNT(pjRbCb, rxEnt->count);
   }
   discFlag =  rxEnt->discFlag;
   dupEntry =  rxEnt->dupEntry;
#ifndef TENB_ACC
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   PJ_QUE_CHECK_FOR_MS(rxEnt,ulCb,gCb)
#endif
#endif
   /* 7. Cleanup the duplicates */
   if(dupEntry == TRUE)
   {
      /* duplicate entry, Need to add a counter to print it on console periodically */
      //QU_CRITLOG(QU_LOG_MESSAGE,"Received duplicate packet in PDCP");
      PJ_FREE_BUF(rxEnt->mBuf);
      PJ_FREE(gCb,rxEnt, sizeof(PjRxEnt));
      RETVALUE(ROK);
   }
   else if(discFlag == TRUE)
   {
      /* discard entry, Need to add a counter to print it on console periodically */
      /*QU_CRITLOG(QU_LOG_MESSAGE,"Received packet in PDCP outside win");*/
      pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
      RETVALUE(ROK);
   }

   /* 6. Send for deciphering */
   ret = pjUlmHdlDecipher(gCb, pjRbCb, rxEnt);

   RETVALUE(ROK);
}/* pjUlmProcessDrbAm */

/**
 *
 * @brief 
 *
 *        Function is called when the offboard timer expires.
 *
 * @b Description: 
 *
 *    The function performs the following
 *    1. If the SDU associated with the timer is not delivered, we 
 *       delete the entry .                                              @n
 *    2. A status indication is sent to the user with error cause.       @n
 *    3. We associate the nextToSubmit value to the next SDU that has 
 *       to be submitted.                                                @n
 *    4. The timer is associated with the nextToSubmit SDU and restarted.@n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    Rx Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmHdlDecipher
(
PjCb       *gCb,
PjUlRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
)
#else
PUBLIC S16 pjUlmHdlDecipher(gCb, pjRbCb, rxEnt)
PjCb       *gCb;
PjUlRbCb     *pjRbCb;        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt;         /* !< Rx Entry */
#endif
{
   PjUlCb   *ulCb;            /* ULM Control Block        */
   PjSecInp  inParam;         /* Input parameters for Lib */
   S16       ret     = ROK;   /* Return values            */
   Buffer   *mBuf    = NULLP; /* Output Buffer            */
   PjSec     *secInfo;

   TRC2(pjUlmHdlDecipher);

   /* 1. Initialistions */
   ulCb  =  &pjRbCb->ulCb;

   secInfo = &(pjRbCb->ueCb->secInfo); 
   /* 2. Process for Deciphering */
#ifndef QCOM 
   if(secInfo->secAct == TRUE && secInfo->cipherInfo.algoType != 0)
#endif
   {
      /* 2.1 Check that it is not the first pkt, with selectively enabled */
      if((pjRbCb->rbType == PJ_SRB) && 
         (pjRbCb->ueCb->secInfo.firstMsg && pjRbCb->ueCb->secInfo.selSecAct))
      {
         /* first pkt */
         pjRbCb->ueCb->secInfo.firstMsg   =  FALSE;
      }
      else
      {
         /* 2.2 Prepare the input Parameters */
         inParam.dir    =  PJ_SEC_DIR_UL;
         inParam.rbId   =  pjRbCb->rbId;
         inParam.count  =  rxEnt->count;
         rxEnt->state = PJ_SENT_TO_DCIPHER;
#ifndef QCOM 
#ifndef TENB_ACC 
#ifndef LTE_PAL_ENB         
         pjUtlChekRxEnqReq(gCb, pjRbCb, rxEnt);
#endif         
#endif         
#endif         
#ifdef SS_RBUF
         if(rxEnt->rbCb)
         {
            RETVALUE(ROK);
         }
#endif
         /* 2.3 call deciphering hook */
         if( pjUtlDecipherReq(gCb, pjRbCb, rxEnt->count, rxEnt, &mBuf) != ROK)
         {
            RLOG1(L_ERROR, "Deciphering Req failed Rx Count [%lu]", rxEnt->count);
            /*PJ_SND_PJU_STA_IND(gCb,pjRbCb, rxEnt);*/
            pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
            PJ_STS_INC_GEN_CNT(gCb,numDeciphFails);
            RETVALUE(RFAILED);
         }

         
         /* 2.4 wait for output before processing further */
    /* Batch Mode processing, Packets queued will be sent to spacc 
       together upon trigger */
#ifdef TENB_AS_SECURITY
         /*if(isSecBatchMode)
         {*/
                /*State set on call back from Crypto*/ 
         	/*rxEnt->state   =  PJ_SENT_TO_DCIPHER;*/
         	RETVALUE(ROK);		 
	      /*}*/
#endif

#ifdef PJ_SEC_ASYNC
         /* 2.4 wait for output before processing further */
         rxEnt->state   =  PJ_SENT_TO_DCIPHER;
         RETVALUE(ROK);
#else   /* PJ_SEC_ASYNC */
         /* 2.3 Validate output of deciphering */
         if(mBuf == NULLP)
         {
            RLOG1(L_ERROR, "Deciphering Req failed Rx Count [%lu]", rxEnt->count);
            PJ_SND_PJU_STA_IND(gCb,pjRbCb, rxEnt);
            pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
            PJ_STS_INC_GEN_CNT(gCb,numDeciphFails);
            RETVALUE(RFAILED);
         }

         /* 2.5 copy output buffer */
#ifdef TENB_AS_SECURITY
         /* If not batch mode, free PDU. If batch mode, free will be done upon
         SPAcc return */
         if(!(isSecBatchMode))
         {
            PJ_FREE_BUF(rxEnt->mBuf);
         }
#endif
         rxEnt->mBuf     =  mBuf;
#endif
      }
   }
   /* 3. Post ciphering updations */
   if(pjRbCb->rbType == PJ_DRB)
   {
      rxEnt->state   =  PJ_RDY_TO_DCOMP;
      ret = pjUlmHdlDeCmp(gCb, pjRbCb, rxEnt);
   }
   else
   {
      rxEnt->state   =  PJ_RDY_TO_INTVER;
      ret = pjUlmHdlIntVer(gCb, pjRbCb, rxEnt);
   }

   RETVALUE(ret);
}/* end of pjUlmHdlDecipher */


/**
 *
 * @brief 
 *
 *        Function is called to perform integrity verification.
 *
 * @b Description: 
 *
 *    The function performs the following
 *    1. Extract the mac-I.
 *    2. Add sn to the buffer.
 *    3. Call Integrity Verification hook.
 *    4. Return in case of Async.
 *    5. Validate output and submit for delivery.
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    Rx Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmHdlIntVer
(
PjCb       *gCb,
PjUlRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
)
#else
PUBLIC S16 pjUlmHdlIntVer(gCb, pjRbCb, rxEnt)
PjCb       *gCb;
PjUlRbCb     *pjRbCb;        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt;         /* !< Rx Entry */
#endif
{
   PjUlCb  *ulCb;          /* ULM Control Block */
   PjSn     sn;            /* Sn Value */    /*KW_FIX*/
   S16      ret = ROK;     /* Return value */
   U32      macI = 0;      /* Mac-I value */
   PjSecInp inParam;       /* Input Params */
   Status   status = ROK;        /* Status of IntVer */

   TRC2(pjUlmHdlIntVer);

   /* 1. Initialistions */
   ulCb  =  &pjRbCb->ulCb;

   /* 2. Xtract the last four bits irrescpective of whether you 
    * are going to do integrity verification or not */
   PJ_UNPK_MACI(rxEnt->mBuf, macI);

   /* 3. Process for Integrity Verification */
   if(pjRbCb->ueCb->secInfo.secAct == TRUE && pjRbCb->ueCb->secInfo.intInfo.algoType != 0)
   {

      /* 3.1 Prepare the input Parameters */
      inParam.dir    =  PJ_SEC_DIR_UL;
      inParam.rbId   =  pjRbCb->rbId;
      inParam.count  =  rxEnt->count;

      /* 3.2. Add the SN to the buffer */
      sn  =  (U16)(rxEnt->count % (1 << PJ_SRB_SN_LEN));  /*KW_FIX*/
      ret = SAddPreMsg((Data)sn, rxEnt->mBuf);
#if (ERRCLASS & ERRCLS_DEBUG)
      if (ret != ROK)
      {
         RLOG_ARG0(L_ERROR, DBG_UEID,pjRbCb->ueCb->key.ueId,
               " pjUlmProcessSrb: SRemPreMsgMult Failed ");
         pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
         RETVALUE(RFAILED);
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      /* 3.3 call deciphering hook */
      if( pjUtlIntVerReq(gCb, pjRbCb, inParam, rxEnt->mBuf, macI, &status) != ROK)
      {
         RLOG1(L_ERROR, "Integrity Verification Req failed Rx Count[%lu]", rxEnt->count);
         ret = SRemPreMsg((Data *)&sn, rxEnt->mBuf);
         if (ret != ROK)
         {
         }
         PJ_SND_PJU_STA_IND(gCb,pjRbCb, rxEnt);
         rxEnt->state = PJ_RX_INTVER_FAIL;
         pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
         PJ_STS_INC_GEN_CNT(gCb,numIntgVrfFails);
         RETVALUE(RFAILED);
      }

      /* Sequence number will be removed from mBuf, on Integrity-Verification Cfm */

      /*Return and wait for call back*/
      /*RsysSHMLog("pjUtlIntVerReq RETURNNNNNN\n"); */
      RETVALUE(ROK);
#ifdef PJ_SEC_ASYNC
      /* 3.5 wait for output before processing further */
      rxEnt->state   =  PJ_SENT_TO_INTVER;
      RETVALUE(ROK);
#else
      /* 3.6 Post integrity verification updations */
      if(status != ROK)
      {
         RLOG1(L_ERROR, "Integrity Verification Req failed Rx Count[%lu]", rxEnt->count);
         PJ_SND_PJU_STA_IND(gCb,pjRbCb, rxEnt);
         pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
         PJ_STS_INC_GEN_CNT(gCb,numIntgVrfFails);
         RETVALUE(RFAILED);
      }

#endif /* PJ_SEC_ASYNC */

   }

   /* 4. Update state and send for delivery */
   rxEnt->state   =  PJ_RDY_TO_SUBMIT;
   ret = pjUlmDeliverSrb(gCb, pjRbCb, rxEnt);

   RETVALUE(ret);
}/* end of pjUlmHdlIntVer */


/**
 *
 * @brief 
 *
 *        Function is called to handle decompression.
 *
 * @b Description: 
 *
 *    The function performs the following
 *    1. Call decompression handler function.
 *    2. For sync, validate output and submit for delivery.
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    Rx Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmHdlDeCmp
(
PjCb       *gCb,
PjUlRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* !< Rx Entry */
)
#else
PUBLIC S16 pjUlmHdlDeCmp(gCb, pjRbCb, rxEnt)
PjCb       *gCb;
PjUlRbCb     *pjRbCb;        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt;         /* !< Rx Entry */
#endif
{
   PjUlCb  *ulCb;          /* ULM Control Block */
   Buffer  *mBuf = NULLP;  /* Output Buffer */

   TRC2(pjUlmHdlDeCmp);


   /* 1. Initialistions */
   ulCb  =  &pjRbCb->ulCb;

   /* 2. process for decompression */
   if(pjRbCb->rohc.hdrCmpUsed == TRUE)
   {
      /* 2.1 call decompression hook */
      if(pjUtlDeCmpReq(gCb, pjRbCb,rxEnt->count, rxEnt->mBuf, &mBuf) != ROK)
      {
         RLOG1(L_ERROR, "DeCompression Req failed Rx Count[%lu]", rxEnt->count);
         PJ_SND_PJU_STA_IND(gCb,pjRbCb, rxEnt);
         pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
         PJ_STS_INC_GEN_CNT(gCb,numDecmpFails);
         RETVALUE(RFAILED);
      }

#ifdef PJ_CMP_ASYNC
      /* 2.2 Wait for output before processing further */
      rxEnt->state   =  PJ_SENT_TO_DCOMP;
      RETVALUE(ROK);
#else /* PJ_CMP_ASYNC */

      /* 2.3 Validate output of decomp */
      if(mBuf == NULLP)
      {
         RLOG1(L_ERROR, "DeCompression Req failed Rx Count[%lu]", rxEnt->count);
         PJ_SND_PJU_STA_IND(gCb,pjRbCb, rxEnt);
         pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
         PJ_STS_INC_GEN_CNT(gCb,numDecmpFails);
         RETVALUE(RFAILED);
      }

      /* 2.4 Copy output buffer */
      PJ_FREE_BUF(rxEnt->mBuf);
      rxEnt->mBuf     =  mBuf;
#endif
   }

   /* 3. Return for discardable entries */
   if((rxEnt->discFlag == TRUE) ||
      (rxEnt->dupEntry == TRUE))
   {
      RLOG2(L_ERROR,"Dropping packets...discflag is [%d], dupEntry is [%d]", rxEnt->discFlag, rxEnt->dupEntry);
      RETVALUE(ROK);
   }

   /* 4. Post decomp updations */
   rxEnt->state   =  PJ_RDY_TO_SUBMIT;
   PJ_DEC_OBD_COUNT(gCb, pjRbCb, rxEnt->count);
   PJ_ULM_DELIVER_DRB(gCb,pjRbCb, rxEnt);

   RETVALUE(ROK);
}/* end of pjUlmHdlDeCmp */

/**
 *
 * @brief 
 *
 *        Function to perform updations and deliver the SDU to the upper layer. 
 *        It is called for SRBs .
 *
 * @b Description: 
 *
 *        This function performs the following functions,                    @n
 *        1. Call PjUiPjuDatInd to deliver the SDU to the upper layer.       @n
 *        2. In async mode, check if any subsequent SDUs can also be sent up.@n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEn     reception entry for the PDU
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmDeliverSrb
(
PjCb   *gCb,
PjUlRbCb *pjRbCb,          /* !< PDCP Control Block */     
PjRxEnt *rxEnt           /* !< Recption Buffer Entry   */            
)
#else
PUBLIC S16 pjUlmDeliverSrb(gCb, pjRbCb, rxEnt)
PjCb   *gCb;
PjUlRbCb *pjRbCb;          /* !< PDCP Control Block */                  
PjRxEnt *rxEnt;          /* !< Recption Buffer Entry   */             
#endif
{

   U32      count;        /* Count for looping through the entries */
#ifdef PJ_SEC_ASYNC
   U32      nxtRxCnt;     /* Count for looping through the entries */
   PjRxEnt  *tmpEnt;      /* Temp var for looping through the entries */
#endif
   PjUlCb   *ulCb;        /* Pointer to Uplink Control Block */

   TRC2(pjUlmDeliverSrb);

   /* Post integrity verification updations */
   count          =  rxEnt->count;
   ulCb           =  &pjRbCb->ulCb;

   /* call the PJU Primitive to deliver it to upper layers */
   PJ_SND_PJU_DAT_IND(gCb,pjRbCb, rxEnt);

   /* cleanup the entry */
   pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);

#ifdef PJ_SEC_ASYNC
   tmpEnt = NULLP;
   /* Search and see if any of the successive SDUs can 
    * also be sent to the upper layer */
   count++;
   nxtRxCnt  =  (ulCb->rxHfn << PJ_SRB_SN_LEN) | ulCb->nxtRxSn;

   /* Check till nxtRxCnt */
   while(count < nxtRxCnt) 
   {
      /* Get the next node */
      tmpEnt   =  (PjRxEnt *)pjDbmGetRxEnt(gCb, &ulCb->recBuf, count);
      count ++;

      if(tmpEnt == NULLP)
      {
         /* Such an entry does not exist, search for the next */
         continue;
      }
      if(tmpEnt->state != PJ_RDY_TO_SUBMIT)
      {
         /* Integrity verification not yet done so we have to wait */
         ulCb->nxtSubCnt =  tmpEnt->count;
         /* Cant send anymore messages up, break*/
         break;
      }
      else
      {
         /* call the PJU Primitive to deliver it to upper layers */
         PJ_SND_PJU_DAT_IND(gCb,pjRbCb, tmpEnt);

         /* cleanup the entry */
         pjDbmDelRxEnt(gCb, &ulCb->recBuf, tmpEnt->count);
      }
   }

   if( ((count == nxtRxCnt) &&(tmpEnt != NULLP) &&(tmpEnt->state == PJ_RDY_TO_SUBMIT)) ||
       ((count == nxtRxCnt) &&(tmpEnt == NULLP)) )
   {
     if((pjChkTmr(gCb, (PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == TRUE) 
     {                                                            
         pjStopTmr(gCb, (PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);          
     }
   }
#endif /* PJ_SEC_ASYNC */


   RETVALUE(ROK);
}/* pjUlmDeliverSrb */


/**
 *
 * @brief 
 *
 *        Function to deliver the SDU to the upper layer. 
 *        It is called for DRB UM
 *
 * @b Description: 
 *
 *        This function performs the following functions.                  @n
 *        1. Deliver the SDU to the upper layer                            @n
 *        2. Clean up the hash list entry for this SDU.                    @n
 *        3. For asynchronous mode, it checks if any subsequent 
 *           messages can also be sent up.                                 @n
 *        4. Mark the next SDU to be submitted to the upper layers.        @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    reception entry for the PDU
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmDeliverDrbUm 
(
PjCb   *gCb,
PjUlRbCb *pjRbCb,        /* !< PDCP Control Block */                  
PjRxEnt *rxEnt         /* !< Recption Buffer Entry   */             
)
#else
PUBLIC S16 pjUlmDeliverDrbUm(gCb, pjRbCb, rxEnt)
PjCb   *gCb;
PjUlRbCb *pjRbCb;        /* !< PDCP Control Block */                  
PjRxEnt *rxEnt;        /* !< Recption Buffer Entry   */             
#endif
{

   PjUlCb   *ulCb;     /* Uplink Cb Ptr */
#ifdef PJ_CMP_ASYNC
   U32      nxtRxCnt;  /* Count for looping through the entries */
   PjRxEnt  *tmpEnt;   /* Temp var for looping through the entries */
   U32      count;     /* count varaible */ 
#endif

   TRC2(pjUlmDeliverDrbUm)

   /* Initialisations */
   ulCb  =  &pjRbCb->ulCb;

   /* Update state */
   rxEnt->state   =  PJ_RDY_TO_SUBMIT;

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   if(rxEnt->count != ulCb->nxtSubCnt)
   {
      RETVALUE(ROK);
   }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
#ifndef PJ_CMP_ASYNC
#endif
#ifndef QCOM_STATS
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
   EXTERN U32 ulrate_pju;
      MsgLen len;
      SFndLenMsg(rxEnt->mBuf, &len);
      ulrate_pju += len;
#endif      
#endif      
#endif 
#ifdef QCOM_STATS
   {
      MsgLen len;
      SFndLenMsg(rxEnt->mBuf, &len);
      pjUlrate_pju += len;
   }
#endif     
   /* call the PJU Primitive to deliver it to upper layers */
   PJ_SND_PJU_DAT_IND(gCb,pjRbCb, rxEnt);

   /* cleanup the entry */
   pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);

#ifdef PJ_CMP_ASYNC

   /* Search and see if any of the succeeding SNs can 
    * also be sent to the upper layer */
   nxtRxCnt  =  (ulCb->rxHfn << pjRbCb->snLen) | ulCb->nxtRxSn;

   /* Search till nextRxCnt */
   for(count = ulCb->nxtSubCnt + 1; count < nxtRxCnt; count++) 
   {
      /* Get the next node */
      tmpEnt   =  (PjRxEnt *)pjDbmGetRxEnt(gCb, &(ulCb->recBuf), count);

      if(tmpEnt == NULLP)
      {
         /* pj005.201 added support for L2 Measurement */
         /* Such an entry does not exist, search for the next */
         continue;
      }
      if(tmpEnt->state != PJ_RDY_TO_SUBMIT)
      {
         /* Cant send anymore messages up, break*/
         break;
      }
      else
      {
         /* pj005.201 added support for L2 Measurement */
         /* call the PJU Primitive to deliver it to upper layers */
         PJ_SND_PJU_DAT_IND(gCb,pjRbCb, tmpEnt);

         /* cleanup the entry */
         pjDbmDelRxEnt(gCb, &ulCb->recBuf, tmpEnt->count);
      }
   }/* end of while */

   /* Update  nxtSubCnt */
   ulCb->nxtSubCnt =  count;
#endif /* PJ_CMP_ASYNC */

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   if(pjRbCb->state != PJ_STATE_NORMAL)
   {
      /* start reestablishment procedures if last message has been processed */
      if((ulCb->transCmp == TRUE) && 
            (ulCb->obdCnt == 0))
      {
         /* It has received all messages from RLC and finished
          * processing them. Possibility only when the last last message
          * from deciphering fails. */
         PJ_ULM_DRBUM_REEST(gCb, pjRbCb);
      }
   }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */

   RETVALUE(ROK);
}/* pjUlmDeliverDrbUm */

/**
 *
 * @brief 
 *
 *        Function to deliver the SDU to the upper layer. It is called for DRB AM
 *
 * @b Description: 
 *
 *        This function performs the following functions.                                  @n
 *        1. Delete duplicates and SDUs marked for discard.                                @n
 *        2. Deliver the SDU to the upper layer                                            @n
 *        3. Perform sequential delivery for SDUs received during reestablishment.         @n
 *        4. Clean up the hash list entry for this SDU.                                    @n
 *        5. For asynchronous mode, check if any subsequent messages can also be sent up.  @n
 *        6. Mark the next SDU to be submitted to the upper layers.                        @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    Rx Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmDeliverDrbAm
(
PjCb   *gCb,
PjUlRbCb *pjRbCb,           /* !< PDCP Control Block */                  
PjRxEnt *rxEnt            /* !< Recption Buffer Entry   */             
)
#else
PUBLIC S16 pjUlmDeliverDrbAm(gCb, pjRbCb, rxEnt)
PjCb   *gCb;
PjUlRbCb *pjRbCb;           /* !< PDCP Control Block */                  
PjRxEnt *rxEnt;           /* !< Recption Buffer Entry   */             
#endif
{
   PjUlCb  *ulCb;        /* RB uplink Control Block */
   U32      nxtSubCnt;   /* Delivery count */
   S16      ret;         /* Return value */
   PjRxEnt *tmpEnt;      /* Temp Entry - loop counter */

   TRC2(pjUlmDeliverDrbAm)

   /* Initialisations */
   ret         = ROK;
   ulCb        = &pjRbCb->ulCb;
   nxtSubCnt   = ulCb->lastSubCnt + 1;

#ifndef QCOM_STATS
#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
extern U32 ulrate_pju;
MsgLen len;
SFndLenMsg(rxEnt->mBuf, &len);
ulrate_pju += len;
#endif
#endif
#endif
      if(rxEnt->state != PJ_RDY_TO_SUBMIT)
      {
         /* PDU still under processing, return */
         RETVALUE(ROK);
      }

      /* pj005.201 added support for L2 Measurement */

      /* submit to upper layers and delete entry */
      /* submit only the in-sequence Pkts, out-of-seq Pkts shall 
       * not be sent and be buffered in datQ 
       * This is Only in the case of either Re-Estb or HandOver */
      if(rxEnt->isOutSeq == FALSE) 
      {
         cmLListFirst(&pjRbCb->ulCb.recBuf.datPktQ);
         if(cmLListCrnt(&pjRbCb->ulCb.recBuf.datPktQ))
         {
            /* Do the following only during Handover or Re-Estb */
            tmpEnt = (PjRxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->ulCb.recBuf.datPktQ));
            if((rxEnt->count) > (tmpEnt->count))
            {
               /* There are some Out-Of-Seq packets buffered in the datPktQ
                * These packets should be delivered before received
                * in-sequence  packet can be delivered */
               while((rxEnt->count) > (tmpEnt->count))
               {
                  PJ_SND_PJU_DAT_IND(gCb,pjRbCb, tmpEnt);
                  pjDbmDelRxEnt(gCb, &ulCb->recBuf, tmpEnt->count);
                  cmLListFirst(&pjRbCb->ulCb.recBuf.datPktQ);              
                  tmpEnt = (PjRxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->ulCb.recBuf.datPktQ));
               }
               PJ_SND_PJU_DAT_IND(gCb,pjRbCb, rxEnt);
               pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
            }
            else
            {
               /* There are no Out-Of-Seq packets buffered in datPktQ
                * So deliver the received in-sequence rxEnt packet */
               PJ_SND_PJU_DAT_IND(gCb,pjRbCb, rxEnt);
               pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
            }
         }
      }
      else
      {
         /* Fix for L2 Crash due to UlMemFailCnt */
        /* if(pjRbCb->state == PJ_STATE_NORMAL)*/
         {
            /* During Normal State do not buffer the Out-Of-Seq Packets */
            PJ_SND_PJU_DAT_IND(gCb,pjRbCb, rxEnt);
            pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
         }
      }
      /* do updations */
      ulCb->lastSubCnt   =  nxtSubCnt;

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   /* start reestablishment procedures if last message has been processed */
   if(pjRbCb->state != PJ_STATE_NORMAL)
   {
      if((ulCb->transCmp == TRUE) && 
         (ulCb->obdCnt == 0))
      {
         /* It has received all messages from RLC and finished
          * processing them. Possibility only when the last last message
          * from deciphering fails. */
         pjUlmReEstDrbAm(gCb, pjRbCb);
      }
   }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */

   RETVALUE(ret);
}/* pjUlmDeliverDrbAm */

/*****************************************************************************
 *                      REESTABLISHMENT FUNCTIONS
 ****************************************************************************/

/**
 *
 * @brief 
 *
 *        Function to is called to reestablish the SRB. It is called as when the 
 *        reestablishment request  is received.
 *
 * @b Description: 
 *
 *        This function performes the following.      @n
 *        1. Reset NEXT_PDCP_RX_SN and RX_HFN         @n
 *        2. Deinitialise the hashlist.               @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmReEstSrb
(
PjCb   *gCb,
PjUlRbCb *pjRbCb       /*!< PDCP control block  */
)
#else
PUBLIC S16 pjUlmReEstSrb(gCb,pjRbCb)
PjCb   *gCb;
PjUlRbCb *pjRbCb;      /*!< PDCP control block  */
#endif
{
   TRC2(pjUlmReEstSrb);

   /* Reset NEXT_PDCP_RX_SN and RX_HFN */
   pjRbCb->ulCb.nxtRxSn  =  0;
   pjRbCb->ulCb.rxHfn  =  0;

   /* Deinitialise the hashlist */
   pjDbmRxDelAll(gCb, &pjRbCb->ulCb.recBuf);

   RETVALUE(ROK);
}  /* pjUlmResetSrb */



/**
 *
 * @brief 
 *
 *        Function to is called to reestablish the DRB .
 *
 * @b Description: 
 *
 *        This function is called after the last pdu is received from RLC
 *        as part of reestablishment procedures. This function builds the staPdu
 *        and store it
 *                                               
 *  @param[in] pjCb     PDCP Instance control block 
 *  @param[in] pjRbCb   PDCP control block 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmReEstDrbAm
(
PjCb       *gCb,
PjUlRbCb   *pjRbCb   
)
#else
PUBLIC S16 pjUlmReEstDrbAm(gCb, pjRbCb)
PjCb       *gCb;
PjUlRbCb   *pjRbCb;   
#endif
{
   U16               fms;               /* First Missing SN                 */
   PjUlCb           *ulCb;              /* RB uplink Control Block          */
   PjSn              sn;                /* temp sn                          */
   PjRxEnt          *tmpEnt    = NULLP; /* Temporary entity used in loop    */
   U32               count;             /* Counter variable                 */
   U32               idx;
   U16               byteIdx = 0;
   U8                bitIdx;
   U8                numBits = 0;   /*KW_FIX */
   Bool              nxtSnNotFnd;
   PjUlHoCfmInfo     *hoInfo = NULLP;   /* To store the numBits and bitMap  */
   U8                packByte[512] = {0};

   TRC2(pjUlmReEstDrbAm);

   RLOG2(L_DEBUG,"pjUlmReEstDrbAm(pjRbCb(%d,%d))",pjRbCb->rbId,pjRbCb->rbType);

   /* Initialisations */
   ulCb  =  &pjRbCb->ulCb;
   PJ_MEM_SET(&(packByte), 0, (sizeof(packByte)));       /*KW_FIX : ccpu00136902*/
   /* Its DRB AM from now on */
   if(pjRbCb->state == PJ_STATE_REEST)
   {
      /* if status report generation not necessary, quit */
      if(ulCb->staRepReqd == FALSE)
      {
         RETVALUE(ROK);
      }
   }

   /* If out-of-seq Pkts are stored in datPktQ then process the
    * bitMap the following way */
   fms = (U16)(pjRbCb->ulCb.fmsCount & (0x0fff));   /*KW_FIX*/
   /* Setting the bitIdx to start from 8 */
   bitIdx = PJ_BYTE_SIZE;
   /* Initialize idx as 1 since Bitmap starts from FMS + 1 */
   idx = 1;
   cmLListFirst(&pjRbCb->ulCb.recBuf.datPktQ);
   while(cmLListCrnt(&pjRbCb->ulCb.recBuf.datPktQ))
   {
      nxtSnNotFnd = TRUE;
      tmpEnt = (PjRxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->ulCb.recBuf.datPktQ));
      /* Retrieve the SN from Count of RxEnt */
      sn = (U16)((tmpEnt->count) & (0x0fff)); /*KW_FIX*/
      if(tmpEnt->isOutSeq == FALSE)
      {
         /* This Packet is In-Seq so no need to compute this for Bitmap
          * As this Packet should have SN less than FMS. */
         cmLListNext(&pjRbCb->ulCb.recBuf.datPktQ);
         continue;
      }
      while((nxtSnNotFnd == TRUE) && (byteIdx < 512))
      {
         /* BitMap start from FMS + 1 */
         count = fms + idx;
         if(count >= (1 << pjRbCb->snLen))
         {
            count = count % (1 << pjRbCb->snLen);
         }
         if(count != sn)
         {
            /* This SN is missing so set bitMap value to 0 */
            packByte[byteIdx] |= (0 << (--bitIdx));
            numBits++;
            idx++;
         }
         else
         {
            /* This SN is present so set bitMap value to 1 */
            packByte[byteIdx] |= (1 << (--bitIdx));
            numBits++;
            idx++;
            nxtSnNotFnd = FALSE;
            cmLListNext(&pjRbCb->ulCb.recBuf.datPktQ);               
         }
         if(numBits % PJ_BYTE_SIZE == 0)
         {
            byteIdx++;
            bitIdx = PJ_BYTE_SIZE;
         }
      }
      if(byteIdx >= 512)
         break;
   }
   /* if bitmap is not byte aligned, then append/pad 
    * with 0s (to indicate from here Packets not received)
    * to make it byte aligned and send to App */
   while(numBits % 8 != 0)
   {
      packByte[byteIdx] |= (0 << (--bitIdx));
      numBits++;
   }

   /* Store the numOfBits and bitMap in ueCb */
   if(pjRbCb->state == PJ_STATE_REEST_HO)
   {
      hoInfo = &pjRbCb->ueCb->hoInfo->hoCfmInfo[pjRbCb->rbId];
      hoInfo->pres = TRUE;
      hoInfo->rbId = pjRbCb->rbId;
      hoInfo->dir |= PJ_DIR_UL;
      /* numBits sent from PDCP is always a multiple of 8 */
      hoInfo->numBits = numBits;
      if (numBits > 0) 
      {
         RLOG_ARG1(L_DEBUG,DBG_UEID,pjRbCb->ueCb->key.ueId, 
               "pjUlmReEstDrbAm : OutOfSeq UL Pkts are present for rbId = %d ",
               pjRbCb->rbId);
      /* ByteAlign the computed Bits, numBits is always a multiple of 8 */ 
      PJ_ALLOC(gCb, hoInfo->ulBitMap, sizeof(U8) * (numBits / 8));
#if (ERRCLASS & ERRCLS_DEBUG)
   if(hoInfo->ulBitMap == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      PJ_MEM_CPY(hoInfo->ulBitMap, packByte, (numBits / 8));
      }
      hoInfo->count = pjRbCb->ulCb.fmsCount;
   }
   else
   {
      /* Send the status report only in case of Re-Establishment 
       * not during Handover as this part is hit when eNB is Source */
      RLOG_ARG0(L_DEBUG,DBG_UEID,pjRbCb->ueCb->key.ueId, 
            "pjUlmReEstDrbAm : Sending PDCP Status Report ");
      pjUlmBldStaRep(gCb, pjRbCb, fms, packByte, numBits);
   }
      /*No error check required here*/

   RETVALUE(ROK);
}  /* pjUlmReEstDrbAm */


/**
 *
 * @brief 
 *
 *        Function is called to build and send the SDU Status report.
 *
 * @b Description: 
 *
 *        This function is called to build the SDU status report and send it to the 
 *        lower layer after reestablishment.
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] fms      First Missing sequence number
 *  @param[in] staReq   Buffer containing status report bitmap
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmBldStaRep
(
PjCb       *gCb,
PjUlRbCb   *pjRbCb,         
PjSn       fms,            
U8         *bitMap,
U16        bMapLen
)
#else
PUBLIC S16 pjUlmBldStaRep(gCb, pjRbCb, fms, bitMap, bMapLen)
PjCb       *gCb;
PjUlRbCb   *pjRbCb;      
PjSn       fms;         
U8         *bitMap;
U16        bMapLen;
#endif
{
   Buffer          *staRep = NULLP;
   U8              packByte = 0;         /* Temp Var for packing bytes */
   S16             ret      =  ROK;  /* Return value */
   U16             byteLen = 0;

   TRC2(pjUlmBldStaRep);

   RLOG2(L_DEBUG, "pjUlmBldStaRep(pjRbCb(%d,%d))",pjRbCb->rbId, pjRbCb->rbType);

   PJ_ALLOC_BUF(gCb, staRep);
#if (ERRCLASS & ERRCLS_DEBUG)
   if(staRep == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   if (pjRbCb->snLen == PJ_12_BIT_SN)
   {
      /* pduType and control bit are zeros */
      packByte = (U8)(fms >> PJ_BYTE_SIZE);  /*KW_FIX*/
      SAddPstMsg(packByte, staRep);

      /* Pack the 8 LSB  bits of fms directly  */
      packByte = (U8)(fms & 0x00ff); /*KW_FIX*/
      SAddPstMsg(packByte, staRep);
   }
   else if (pjRbCb->snLen == PJ_15_BIT_SN)
   {
      /* pduType and control bit are zeros */
      SAddPstMsg(packByte, staRep);
      /* one bit reserved in second byte */
      packByte = (U8)((fms & 0x7f00) >> PJ_BYTE_SIZE);
      SAddPstMsg(packByte, staRep);

      /* Pack the 8 LSB  bits of fms directly  */
      packByte = (U8)(fms & 0x00ff);
      SAddPstMsg(packByte, staRep);
   }
   /* Pack the bitmap */
   if(bMapLen != 0)
   {
      /* bMapLen will always be a multiple of 8 */
      byteLen = (U16)(bMapLen / 8);  /*KW_FIX*/
      SAddPstMsgMult(bitMap, byteLen, staRep);
   }

   /*send the status report to DLPDCP */
   pjUtlUlSndUlStaRep(gCb, pjRbCb, staRep);

   RETVALUE(ret);

}/*pjUlmBldStaRep*/


/**
 *
 * @brief 
 *
 *        Function is called during handover to transfer the undeliverd SDUs
 *        at the target side.
 *
 * @b Description: 
 *
 *        This function performs the following.                           @n
 *        1. Calculate lastSubCnt.                                        @n
 *        2. Compute count for each SDU.                                  @n
 *        3. Create an rxEnt for each SDU and insert it into the hashlist.@n
 *                                               
 *  @param[in] pjRbCb      PDCP control block 
 *  @param[in] datFwdReq   Data Forward Request info
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmHdlDatFwdReq  
(
PjCb           *gCb,
PjUlRbCb         *pjRbCb,                       /* !< PDCP ID */        
PjuDatFwdReqInfo   *datFwdReq                 /* !< DatFwdReq Info */
)
#else
PUBLIC S16 pjUlmHdlDatFwdReq(gCb, pjRbCb, datFwdReq)
PjCb           *gCb;
PjUlRbCb         *pjRbCb;                       /* !< PDCP ID */        
PjuDatFwdReqInfo   *datFwdReq;                /* !< DatFwdReq Info */ 
#endif
{
   U32            datFwdCnt;         /* Temp value to store SDU count    */
   PjRxEnt       *rxEnt     = NULLP; /* Rx Entry pointer                 */
   PjuDatFwdInfo *info;              /* Loop var - pointer to DatFwdInfo */
   PjUlCb        *ulCb      = NULLP; /* UL Control block                 */
   U8             sduIndex;          /* loop index                       */

   TRC2(pjUlmHdlDatFwdReq);

   RLOG2(L_DEBUG, "pjUlmHdlDatFwdReq(pjRbCb(%d,%d), datFwdReq) ",
         pjRbCb->rbId, pjRbCb->rbType);

   /* Initialisations */
   ulCb  =  &pjRbCb->ulCb;

   /* Loop through all SDUs */
   for( sduIndex = 0; sduIndex < datFwdReq->numSdus; sduIndex++)
   {
      info  =  &datFwdReq->datFwdInfo[sduIndex];
      PJ_STS_INC_GEN_CNT(gCb,numPktsRcvd);

      /* Compute the count from the SN value  */
      PJ_GET_DATFWD_CNT(pjRbCb, (info->sn), datFwdCnt); 
      /* PJ_ULM_UPD_RX_VAR(info->sn, pjRbCb); */

      /* Create and insert the entry in the buffer */
      PJ_ALLOC(gCb,rxEnt, sizeof(PjRxEnt));

#if (ERRCLASS & ERRCLS_DEBUG)
      if(rxEnt == NULLP)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         RETVALUE(RFAILED);
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      rxEnt->count     = datFwdCnt;
      rxEnt->state     = PJ_RDY_TO_SUBMIT;
      rxEnt->mBuf      = info->sdu;

      if(pjRbCb->state == PJ_STATE_HO) 
      {
         /* If RbCb State = PJ_STATE_HO, then this is Target and Forwarded Packets
          * are received before Status Transfer so buffer this Packet */
         rxEnt->isOutSeq = TRUE;
      }
      /* Insert the entry in the buffer */
      pjDbmInsRxEnt(gCb, &ulCb->recBuf, rxEnt, FALSE); 
   }

   RETVALUE(ROK);
}/*pjUlmHdlDatFwdReq*/

/**
 *
 * @brief 
 *
 *        Function is called when 
 *        1. The offboard timer expires.
 *        2. Deciphering returns. 
 *
 * @b Description: 
 *
 *    The function is called when the pdu is next in line for decompression
 *    It performs the following.                                           @n
 *    1. Submit the pdu for decompression.                                 @n
 *    2. Check if any successive messages can be sent to for decompression @n
 *    3. Update the nxtSubDeCmp value appropriately .                      @n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *  @param[in] rxEnt    Reception Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC S16 pjUlmSubmitForDecmp
(
PjCb       *gCb,
PjUlRbCb     *pjRbCb,        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt          /* Ptr To Rx Entry */
)
#else
PUBLIC S16 pjUlmSubmitForDecmp(gCb, pjRbCb, rxEnt)
PjCb       *gCb;
PjUlRbCb     *pjRbCb;        /* !< PDCP Control Block */  
PjRxEnt    *rxEnt;         /* Ptr To Rx Entry */
#endif
{
   PjUlCb  *ulCb;          /* ULM Control Block */
#ifdef PJ_SEC_ASYNC
   U32     count;          /* count to go through the list and check */
   U32     nxtRxCnt;       /* count of Sdu to receive next */
   PjRxEnt *tmpEnt;        /* Temporary variable for looping */
#endif /* PJ_SEC_ASYNC */

   TRC2(pjUlmSubmitForDecmp);

#ifndef ALIGN_64BIT
   RLOG3(L_DEBUG, "pjUlmSubmitForDecmp(pjRbCb(%d,%d),rxEnt(%ld)) ",
         pjRbCb->rbId, pjRbCb->rbType, rxEnt->count);
#else
   RLOG3(L_DEBUG, "pjUlmSubmitForDecmp(pjRbCb(%d,%d),rxEnt(%d)) ",
         pjRbCb->rbId, pjRbCb->rbType, rxEnt->count);
#endif

   /* 1. Initialisations */
   ulCb  =  &pjRbCb->ulCb;
   rxEnt->state = PJ_RDY_TO_DCOMP;

   /* 2. If decomp is disabled, start delivery procedures */
   if(pjRbCb->rohc.hdrCmpUsed != TRUE)
   {
      rxEnt->state   =  PJ_RDY_TO_SUBMIT;
      PJ_DEC_OBD_COUNT(gCb, pjRbCb, rxEnt->count);
      PJ_ULM_DELIVER_DRB(gCb,pjRbCb, rxEnt);
      RETVALUE(ROK);
   }

   /* 3. If not the next in line for decompression, wait.. */
   if(ulCb->nxtSubDeCmp != rxEnt->count)
   {
      RETVALUE(ROK);
   }

   /* 4. Calling decompression hook */
   if(pjUlmHdlDeCmp(gCb, pjRbCb, rxEnt) != ROK)
   {
      RETVALUE(RFAILED);
   }

#ifdef PJ_SEC_ASYNC
   /* 5. cycle through the subsequent entries to see if any of them are also 
    * available for decompression */
   PJ_GET_COUNT(nxtRxCnt, (pjRbCb->snLen), (ulCb->nxtRxSn), (ulCb->rxHfn));

   for(count =  ulCb->nxtSubDeCmp + 1; count < nxtRxCnt; count ++)
   {
      /* 5.1 Get the next node */
      tmpEnt   =  pjDbmGetRxEnt(gCb, &ulCb->recBuf, count);

      /* 5.2 If such an entry does not exist, search for the next */
      if(tmpEnt == NULLP)
      {
         continue;
      }

      /* 5.3 During post reestablishment phase, there might me some PDUs in 
       * the Rx Buffer that were received prior to reestablishment and 
       * have been fully processed but are not sent up because they are 
       * waiting for the arrival of a previous PDU. We can  safely skip 
       * such an entry */
      if(tmpEnt->state == PJ_RDY_TO_SUBMIT)
      {
         continue;
      }

      /* 5.4 Deciphering not yet done so we have to wait */
      if(tmpEnt->state != PJ_RDY_TO_DCOMP)
      {
         break;
      }

      /* 5.5 Send it to decompression */
      if(pjUlmHdlDeCmp(gCb, pjRbCb, tmpEnt) != ROK)
      {
         RETVALUE(RFAILED);
      }

   } /* while(!((sn == ulCb->nxtRxSn)... */

   /* 6. Update nxtSubDeCmp */
   ulCb->nxtSubDeCmp = count;

#endif /* PJ_SEC_ASYNC */

   RETVALUE(ROK);

}/* end of pjUlmSubmitForDecmp */

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
/**
 *
 * @brief 
 *
 *        Function is called when the offboard timer expires.
 *
 * @b Description: 
 *
 *    The function performs the following
 *    1. If the SDU associated with the timer is not delivered, we 
 *       delete the entry .                                              @n
 *    2. A status indication is sent to the user with error cause.       @n
 *    3. In case of failure call pjUlmHdlErrUpdates to recalculate
 *       nxtSubCnt and if necessary nxtSubDeCmp.                         @n
 *    4. The timer is associated with the nextToSubmit SDU and restarted.@n
 *                                               
 *  @param[in] pjRbCb   PDCP control block 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 */
#ifdef ANSI
PUBLIC Void pjUlmHdlObdTmrExpiry
(
PjCb       *gCb,
PjUlRbCb     *pjRbCb         /* !< PDCP Control Block */  
)
#else
PUBLIC Void pjUlmHdlObdTmrExpiry(gCb,pjRbCb)
PjCb       *gCb;
PjUlRbCb     *pjRbCb;        /* !< PDCP Control Block */  
#endif
{
   PjUlCb  *ulCb;          /* ULM Control Block */
   PjRxEnt *rxEnt;         /* Ptr To Rx Entry */
   U32      subCnt;        /* Count that was/will be submitted */

   TRC2(pjUlmHdlObdTmrExpiry);

   RLOG2(L_DEBUG, "pjUlmHdlObdTmrExpiry(pjRbCb(%d,%d)) ",
         pjRbCb->rbId, pjRbCb->rbType);

   /* 1. Initialistions */
   ulCb  =  &pjRbCb->ulCb;
   PJ_STS_INC_GEN_CNT(gCb,numPdusDiscObdTmrExp);


   /* 2. For SDUs that have not yet been delivered */
   PJ_ULM_GET_SUBCNT(pjRbCb, subCnt, gCb);
   if(ulCb->obdPdu == subCnt)
   {
      /* 2.1 Send failure indication  and delete the entry */
      rxEnt =  (PjRxEnt *)pjDbmGetRxEnt(gCb, &(ulCb->recBuf), ulCb->obdPdu);
      if(rxEnt == NULLP)
      {
         if( ulCb->recBuf.numEntries == 0)
         {
            RETVOID;
         }
#if (ERRCLASS & ERRCLS_DEBUG)
         RLOG1(L_ERROR, "rxEnt not found for subCnt [%lu]", subCnt);
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
         RETVOID;
      }
      else
      {
         RLOG1(L_ERROR, "Obd Timer expires without reply for subCnt [%lu]", subCnt);
         PJ_SND_PJU_STA_IND(gCb,pjRbCb, rxEnt);
         pjDbmDelRxEnt(gCb, &ulCb->recBuf, rxEnt->count);
      }

      /* 2.2 Update nxtSubCnt */
      pjUlmHdlErrUpdates(gCb, pjRbCb, ulCb->obdPdu);

      /* 2.3 Restart timer if it has not been restarted already */
      if( ulCb->recBuf.numEntries != 0)
      {
         if((pjChkTmr(gCb, (PTR)pjRbCb, PJ_EVT_UL_OBD_TMR)) == FALSE)
         {
            PJ_ULM_GET_SUBCNT(pjRbCb, (ulCb->obdPdu), gCb);
            if(!PJ_DRBAM_ALL_PDU_RECVD(pjRbCb))
            {
               pjStartTmr(gCb, (PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
            }
         }
      }
   }
   /* 3. For SDUs that have been delivered */
   else
   {
      if( ulCb->recBuf.numEntries != 0)
      {
         /* 3.1 Associate the nxtToSub with the timer */
         PJ_ULM_GET_SUBCNT(pjRbCb, (ulCb->obdPdu), gCb);
         if(!PJ_DRBAM_ALL_PDU_RECVD(pjRbCb))
         {
            pjStartTmr(gCb, (PTR)pjRbCb, PJ_EVT_UL_OBD_TMR);
         }
      }
   }

   RETVOID;
}/* end of pjUlmHdlObdTmrExpiry */
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */


/**
 *
 * @brief 
 *
 *        Function is called when 
 *        1. The offboard timer expires 
 *        2. The library functions fail.
 *
 * @b Description: 
 *
 *    The function performs the following
 *    1. Update nxtSubCnt if necessary.
 *    2. Update nxtSubDeCmp if necessary.
 *    3. Check if reestablishment has to be triggered.
 *                                               
 *  @param[in] pjRbCb   PDCP control block.
 *  @param[in] errCnt   Count of the error PDU.
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmHdlErrUpdates
(
PjCb       *gCb,
PjUlRbCb     *pjRbCb,        /* !< PDCP Control Block */  
U32         errCnt         /* !< Count of PDU that raises the error */
)
#else
PUBLIC Void pjUlmHdlErrUpdates(gCb, pjRbCb, errCnt)
PjCb       *gCb;
PjUlRbCb     *pjRbCb;        /* !< PDCP Control Block */  
U32         errCnt;        /* !< Count of PDU that raises the error */
#endif
{
   PjUlCb  *ulCb;          /* ULM Control Block */
   U32      nxtRxCnt;
#ifdef PJ_SEC_ASYNC
   U32      nxtToDeCmp;
#endif /* PJ_SEC_ASYNC */

#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   U32      nxtToSub;
   PjRxEnt *rxEnt = NULLP;   /* Ptr To Rx Entry */
#endif /* (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)) */

   TRC2(pjUlmHdlErrUpdates);

   /* 1. Initialistions */
   ulCb       =  &pjRbCb->ulCb;
   PJ_GET_COUNT(nxtRxCnt, (pjRbCb->snLen), (ulCb->nxtRxSn), (ulCb->rxHfn));

   /* 2. Decrement obd counter */
   PJ_DEC_OBD_COUNT(gCb, pjRbCb, errCnt);

#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   /* 3. For DRB AM , check if we can deliver anything  */
   if((pjRbCb->rbType == PJ_DRB) &&  (pjRbCb->mode == PJ_DRB_AM))
   {
      /* if any successive entries can be sent up , send it */
      for(nxtToSub  =  ulCb->lastSubCnt + 1; nxtToSub < nxtRxCnt; nxtToSub++)
      {
         if( (rxEnt = pjDbmGetRxEnt(gCb, &ulCb->recBuf, nxtToSub)) != NULLP)
         {
            /* As soon as we get an entry, we break */
            if(rxEnt->state == PJ_RDY_TO_SUBMIT)
            {
               pjUlmDeliverDrbAm(gCb, pjRbCb, rxEnt);
            }
            break;
         }

      }
      if(nxtToSub == nxtRxCnt)
      {
         /* This situation occurs, if this is the only pdu in the hashlist 
          * and obd fails for it. In this case, we have to trigger reestablishment
          * procedures */
         if(pjRbCb->state != PJ_STATE_NORMAL)
         {
            if((ulCb->transCmp == TRUE) && 
                  (ulCb->obdCnt == 0))
            {
               PJ_ULM_DRBUM_REEST(gCb, pjRbCb);
            }
         }
      }
   }
   /* 4. For SRB/DRB UM Update nxtSubCnt */
   else
   {
      if(errCnt ==  ulCb->nxtSubCnt)
      {
         for(nxtToSub  =  ulCb->nxtSubCnt + 1; nxtToSub < nxtRxCnt; nxtToSub++)
         {
            if( (rxEnt = pjDbmGetRxEnt(gCb, &ulCb->recBuf, nxtToSub)) != NULLP)
            {
               ulCb->nxtSubCnt = nxtToSub;
               break;
            }
         }
         if(nxtToSub == ulCb->nxtSubCnt )
         {
            if(rxEnt->state == PJ_RDY_TO_SUBMIT)
            {
               PJ_ULM_DELIVER_PDU(gCb, pjRbCb, rxEnt);
            }
         }
         else
         {
            /* Update nxtSubCnt to point to nxtRxCnt so that 
             * updations at pjUlmProcessDrbUm/Srb can happen properly */
            ulCb->nxtSubCnt   =  nxtRxCnt;

            /* Check if its necessary to reestablish */
            if(pjRbCb->state != PJ_STATE_NORMAL)
            {
               if((ulCb->transCmp == TRUE) && 
                     (ulCb->obdCnt == 0))
               {
                  PJ_ULM_DRBUM_REEST(gCb, pjRbCb);
               }
            }
         }
      }
   }

#endif /* (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC)) */

   /* Notes : Update nxSubCnt first and then nxtSubDeCmp because we dont want a case
    * where nxtSubDeCmp becomes greater that nxtSubCnt */
#ifdef PJ_SEC_ASYNC
   /* 5. Update nxtSubDeCmp */
   if((pjRbCb->rbType == PJ_DRB) && (errCnt == ulCb->nxtSubDeCmp))
   {
      nxtToDeCmp  =  ulCb->nxtSubDeCmp + 1;

      for(; nxtToDeCmp < nxtRxCnt; nxtToDeCmp++)
      {
         rxEnt = pjDbmGetRxEnt(gCb, &ulCb->recBuf, nxtToDeCmp);

         if(rxEnt == NULLP)
         {
            continue;
         }

         /* A pdu existing after reestablishment */
         if(rxEnt->state == PJ_RDY_TO_SUBMIT)
         {
            continue;
         }

         ulCb->nxtSubDeCmp = nxtToDeCmp;
         break;
      }

      if(nxtToDeCmp == ulCb->nxtSubDeCmp)
      {
         if(rxEnt && (rxEnt->state == PJ_RDY_TO_DCOMP))
         {
            pjUlmSubmitForDecmp(gCb, pjRbCb, rxEnt);
         }
      }
      else
      {
         /* Update nxtSubDeCmp to point to nxtRxCnt so that 
          * updations at pjUlmProcessDrbUm/Am can happen properly */
         ulCb->nxtSubDeCmp =  nxtRxCnt;

      }
   }
#endif /* PJ_SEC_ASYNC */

   RETVOID;
}/* end of pjUlmHdlErrUpdates */

/**
 *
 * @brief 
 *
 *        Function is called when 
 *        1.  PjLiKwuReEstCmpInd received from RLC in case of Re-establishment and HO
 *
 * @b Description: 
 *
 *    The function performs the following
 *    1. Checks the RB state, if it is not equal to PJ_STATE_NORMAL calls
 *        PJ_ULM_DRBUM_REEST or pjUlmReEstDrbAm
 *                                               
 *  @param[in] tPjCb   PDCP control block.
 *  @param[in] tRbCb  RB control block.
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmReEstCmpInd
(
PjCb   *gCb,
PjUlRbCb *tRbCb        /* !< RB control block */  
)
#else
PUBLIC Void pjUlmReEstCmpInd(tPjCb, tRbCb)
PjCb       *gCb;
PjUlRbCb     *tRbCb;        /* !< PDCP Control Block */  
#endif
{

   TRC3(pjUlmReEstCmpInd)

   /* No need to update state for SRB */
   if(tRbCb->ueCb->libInfo.state == PJ_STATE_NORMAL)
   {
      RETVOID;
   }

   if(tRbCb->rbType == PJ_SRB)
   {
      RETVOID;
   }

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   /* If Async mode, set variable to true and return */
   tRbCb->ulCb.transCmp = TRUE;
   if(tRbCb->ulCb.obdCnt == 0)
   {
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */
      /* In synchronous mode, we can proceed with reestablishment */
      if (tRbCb->mode  ==  PJ_DRB_UM)
      {
         tRbCb->ulCb.nxtRxSn  =  0; 
         tRbCb->ulCb.rxHfn    =  0;
#ifdef LTE_L2_MEAS
         tRbCb->ulCb.nxtSubSn =  0; 
#endif
      }
      else
      {
         (Void)pjUlmReEstDrbAm(gCb, tRbCb);
      }

      pjUtlUlHdlRbReEstComplete(gCb,tRbCb);

#if (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC))
   }
#endif /* (defined(PJ_SEC_ASYNC) || defined (PJ_CMP_ASYNC)) */

   RETVOID;
}


/**
 *
 * @brief
 *
 *        Handler to forward the uplink data to the upper layer.
 *
 * @b Description:
 *
 *        1. This function is used to forward the uplink data to the upper layer
 *        during handover.  @n
 *        2. The out-of-sequene data SDUs are sent to the upper
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
PUBLIC S16 pjUlmStartDataFrwdPerRb
(
PjCb       *gCb,
PjUlRbCb    *pjRbCb 
)
#else
PUBLIC S16 pjUlmStartDataFrwdPerRb(gCb, pjRbCb)
PjCb       *gCb;
PjUlRbCb    *pjRbCb;
#endif
{

   U32              numSdu =0;
   U16              count;
   U32              numSduCnt =0,numSduTx = 0;
   PjRxEnt  *tmpRxEnt = NULLP;              /* temporary Rx Entity */
   PjuDatFwdIndInfo *datFwdInd = NULLP;
   U32      numEntries = 0;
   U16      sn = 0;
   PjPjuSapCb   *pjuSap;

   TRC3(pjUlmStartDataFrwdPerRb)

   RLOG1(L_DEBUG, "pjUlmStartDataFrwdPerRb (%d )) ", pjRbCb->rbId);

   pjuSap   = &(gCb->u.ulCb->pjuSap[PJ_DRB_SAP]);
   if((pjRbCb->ulCb.nxtRxSn == 0) && (pjRbCb->ulCb.rxHfn == 0))
   {
      if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,(Data **)&datFwdInd,
      sizeof (PjuDatFwdIndInfo)) != ROK)
      {
          datFwdInd = NULLP;
      }
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (datFwdInd == NULLP)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
      datFwdInd->dir = PJ_DIR_UL;
      datFwdInd->numSdus = 0;
      datFwdInd->isLastDatFwdInd = TRUE;

      /* sending DatFwdInd even if numSdu is zero */
      pjUtlUlSndDatFwdInd(gCb,pjRbCb, datFwdInd);

      RETVALUE(ROK);
   }

   /* Find the total number of RxEnts present */
   numEntries = pjRbCb->ulCb.recBuf.numEntries;
   /* Find the total count of the txEnts present */
   numSduCnt = numSdu = numEntries;

   /* Traverse the datPktQ and send the UL Packets to DAM */
   cmLListFirst(&pjRbCb->ulCb.recBuf.datPktQ);

while(numSduCnt >0)
{
   count = 0;
   if(numSduCnt > PJ_FWD_MAX_SDU_CNT)
   {
      numSduTx  = PJ_FWD_MAX_SDU_CNT;
      numSduCnt = numSduCnt - PJ_FWD_MAX_SDU_CNT;
   }
   else
   {
      numSduTx = numSduCnt;
      numSduCnt = 0;
   }

      if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,(Data **)&datFwdInd,
      sizeof (PjuDatFwdIndInfo)) != ROK)
      {
          datFwdInd = NULLP;
      }
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (datFwdInd == NULLP)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

      if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,
         (Data **)&datFwdInd->datFwdInfo, (sizeof (PjuDatFwdInfo)* numSduTx)) != ROK)
      {
          datFwdInd->datFwdInfo = NULLP;
      }
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (datFwdInd->datFwdInfo == NULLP)
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   while (numSduTx>0)
   {
      /* Retrieve the Packet Info by typecasting to RxEnt */
      tmpRxEnt = (PjRxEnt *) cmLListNode(cmLListCrnt(&pjRbCb->ulCb.recBuf.datPktQ)); 
      sn = (U16)((tmpRxEnt->count) & (0x0fff)); /*KW_FIX*/

      datFwdInd->datFwdInfo[count].sn = sn;
      /* Copy the SDU Buffer as it is into datFwdInfo */
#ifdef SS_RBUF
     datFwdInd->datFwdInfo[count].sdu = tmpRxEnt->mBuf;
#else
      SCpyMsgMsg(tmpRxEnt->mBuf, 0, 0,
            &datFwdInd->datFwdInfo[count].sdu);
#endif
      cmLListNext(&pjRbCb->ulCb.recBuf.datPktQ);
      numSduTx--;
      count++;
   }
      /* Fill the datFwdInd struct and send it to DAM */
      datFwdInd->dir = PJ_DIR_UL;
      datFwdInd->numSdus = count;
      datFwdInd->isLastDatFwdInd = FALSE;
      /* Send the datFwdInd to DAM */
      pjUtlUlSndDatFwdInd(gCb,pjRbCb, datFwdInd);

}

   if(SGetSBuf(pjuSap->pst.region,pjuSap->pst.pool,(Data **)&datFwdInd,
      sizeof (PjuDatFwdIndInfo)) != ROK)
   {
       datFwdInd = NULLP;
   }
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (datFwdInd == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */
   datFwdInd->dir = PJ_DIR_UL;
   datFwdInd->numSdus = 0;
   datFwdInd->isLastDatFwdInd = TRUE;

   /* sending DatFwdInd even if numSdu is zero */
   pjUtlUlSndDatFwdInd(gCb,pjRbCb, datFwdInd);

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
 *  @param[in] pjUlRbCb    Uplink Rb control block. 
 *  @param[in] datFwdReq   Data Forward Info.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 *
 */
#ifdef ANSI
PUBLIC S16 pjUlmHndlDatFwdReq
(
PjCb               *gCb,
PjUlRbCb           *pjRbCb,
PjuDatFwdReqInfo   *datFwdReq
)
#else
PUBLIC S16 pjUlmHndlDatFwdReq(gCb,pjRbCb,datFwdReq)
PjCb               *gCb;
PjUlRbCb           *pjRbCb;
PjuDatFwdReqInfo   *datFwdReq;
#endif
{

   U32      sduId;               /* SDU ID */
   U32      ctxHfn;               /* TX_HFN of a SDU */
   PjRxEnt  *rxEnt;              /* Transmission Entity */ 
   PjSn     sn;
   int      ret;
   U32      numSdus=0;

   TRC3(pjUlmHdlDatFwdReq)

   RLOG2(L_DEBUG, "pjDlmHndlDatFwdReq(pjRbCb (%d),datFwdReq(%d)) ", 
      pjRbCb->rbId, datFwdReq->numSdus);

   sduId    = 0;
   ret      = ROK;
   sn       =datFwdReq->datFwdInfo[numSdus].sn;
   gCb->pjGenSts.numPktsRcvd += datFwdReq->numSdus;
   /* Process each of the SDUs with received SN and sduId */ 
   for ( numSdus = 0; numSdus < datFwdReq->numSdus; numSdus++ )
   {
      /* 
         The variables nxtTxSn and COUNT are assumed to be 
         already updated in dlCb to continue with the 
         transmission ( in the target eNodeB ).
      */
      ctxHfn = pjRbCb->ulCb.rxHfn;

      /* 2. Allocate memory for the rxEnt */
      PJ_ALLOC(gCb,rxEnt, sizeof(PjRxEnt));
#if (ERRCLASS & ERRCLS_DEBUG)
      if(rxEnt == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failure");
         RETVALUE(RFAILED);
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      /* 3. Perform validations */
      /* 3.1 A SN already delivered to the upper layer has come */
      if(PJ_CHK_SN_OUTSIDE_WIN((&pjRbCb->ulCb), sn, pjRbCb->snLen))
      {
         rxEnt->discFlag = TRUE;
         if(sn > pjRbCb->ulCb.nxtRxSn)
         {
            ctxHfn --;
         }
      }

      /* 3.2 Wrap around case */
      else if((pjRbCb->ulCb.nxtRxSn - sn) > PJ_DRB_AM_WINSIZE(pjRbCb->snLen))
      {
         pjRbCb->ulCb.rxHfn ++;
         ctxHfn ++;
         pjRbCb->ulCb.nxtRxSn = (U16)(sn + 1); /*KW_FIX*/
         PJ_ULM_UPD_NXT2DCOMP(pjRbCb->ulCb, (pjRbCb->snLen), sn, oldRxCnt);

      }
      /* 3.3 A missed out packet arriving after wrap around */
      else if((sn - pjRbCb->ulCb.nxtRxSn) > PJ_DRB_AM_WINSIZE (pjRbCb->snLen))
      {
         ctxHfn --;
      }
      /* 3.4 normal scenario */
      else if(sn >=  pjRbCb->ulCb.nxtRxSn)
      {

         pjRbCb->ulCb.nxtRxSn = (U16)(sn + 1);  /*KW_FIX*/
         if(pjRbCb->ulCb.nxtRxSn ==  (1 << pjRbCb->snLen))
         {
            pjRbCb->ulCb.nxtRxSn = 0;
            pjRbCb->ulCb.rxHfn ++;
         }
      }

      /* 4. Fill up the structures and insert into hashlist */
      rxEnt->count   =  (ctxHfn << pjRbCb->snLen) |  sn;
      rxEnt->state   =  PJ_RDY_TO_DCIPHER;
      rxEnt->mBuf    =  datFwdReq->datFwdInfo[numSdus].sdu;

      PJ_ULM_UPD_NXT2SUB(pjRbCb->ulCb, rxEnt->count); 
      ret = pjDbmInsRxEnt(gCb, &pjRbCb->ulCb.recBuf, rxEnt, TRUE); 
      if(ret == ROKDUP)
      {
         /* Duplicates are not to be inserted */
         /* marked to be freed up later */
         rxEnt->dupEntry =  TRUE;
      }

      if(pjRbCb->rbType == PJ_DRB)
      {
         rxEnt->state   =  PJ_RDY_TO_DCOMP;
         ret = pjUlmHdlDeCmp(gCb, pjRbCb, rxEnt);
      }
      else
      {
         rxEnt->state   =  PJ_RDY_TO_INTVER;
         ret = pjUlmHdlIntVer(gCb, pjRbCb, rxEnt);
      }

      if(rxEnt->dupEntry == TRUE)
      {
         /* duplicate entry */
         PJ_FREE_BUF(rxEnt->mBuf);
         PJ_FREE(gCb,rxEnt, sizeof(PjRxEnt));
      }
      else if(rxEnt->discFlag == TRUE)
      {
         /* duplicate entry */
         pjDbmDelRxEnt(gCb, &pjRbCb->ulCb.recBuf, rxEnt->count);
      }   
   }
   RETVALUE(ROK);
}
/**
 *
 * @brief It processes the packets queued up in ULPktQ
 *
 * @Description
 *      This ulPktQ queues the forwarded message at targe enodeb 
 *      during handover. 
 *
 *  @param[in] pjCb     PDCP Instance control block.
 *  @param[in] pjUlRbCb  Uplink Rb Control block 
 *
 *  @return  Void
 */
#ifdef ANSI
PUBLIC Void pjUlmProcessUlPktQ
(
PjCb       *gCb,
PjUlRbCb   *pjRbCb           /* !< PDCP Control Block */
)
#else
PUBLIC Void pjUlmProcessUlPktQ(gCb, pjRbCb)
PjCb       *gCb;
PjUlRbCb   *pjRbCb;          /* !< PDCP Control Block */
#endif
{
   PjUlPkt *pkt;
   CmLList *node;

   TRC2(pjUlmProcessUlPktQ);

   CM_LLIST_FIRST_NODE(&(pjRbCb->ulCb.ulPktLst), node);
   while (node != NULLP)
   {
      pkt = (PjUlPkt *) node->node;

      cmLListDelFrm(&(pjRbCb->ulCb.ulPktLst), node);
      pkt->lnk.node = NULLP;

      pjUlmProcessDrbAm(gCb,pjRbCb, pkt->sn, pkt->pdu, TRUE);
      PJ_FREE(gCb,pkt, sizeof (PjUlPkt));
      
      CM_LLIST_FIRST_NODE(&(pjRbCb->ulCb.ulPktLst), node);
   }

   RETVOID;
}/* end of pjDlmProcessDlPktQ */

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
PRIVATE Void pjUlmEnqueueUlPkt
(
PjCb     *gCb,
PjUlRbCb   *pjRbCb,           /* !< PDCP Control Block */
PjSn      sn,               /* !< SN value of PDU */
Buffer   *pdu              /* !< PDU message buffer */
)
#else
PRIVATE Void pjUlmEnqueueUlPkt(gCb,pjRbCb, sn, pdu)
PjCb     *gCb;
PjUlRbCb   *pjRbCb;          /* !< PDCP Control Block */
PjSn      sn;              /* !< SN value of PDU */
Buffer   *pdu;             /* !< PDU message buffer */
#endif
{
   PjUlPkt    *pkt;

   TRC2(pjUlmEnqueueUlPkt)

   PJ_ALLOC(gCb,pkt, sizeof(PjUlPkt));
   if (pkt != NULLP)
   {
      pkt->sn = sn;
      pkt->pdu = pdu;
      pkt->type = PJ_DATA_NRM_PKT;
      pkt->lnk.node = (PTR)pkt;
      cmLListAdd2Tail (&pjRbCb->ulCb.ulPktLst, &pkt->lnk);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failure");
      PJ_FREE_BUF(pdu);
   }

   RETVOID;
}
                                                        
/*@}*/

/********************************************************************30**
  
         End of file:     eq_ulm.c@@/main/TeNB_Main_BR/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Tue Jan 13 11:31:53 2015
  
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
/main/1      ---      mn   1. LTERLC Release 2.1
/main/2     pj005.201 ap   1. Added support for L2 Measurment.
                           2. Removed macro and written function 
                              to send DatFwdInd message
*********************************************************************91*/

