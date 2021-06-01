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
  
        Name:    RLC service user (PDCP) lower interface
    
        Type:    C file
  
        Desc:    This file Contains the RLC service user lower interface
                  primitive implementain
 
        File:    kw_ptlib.c

        Sid:      eq_ptlib.c@@/main/TeNB_Main_BR/12 - Mon Aug 11 16:42:51 2014
  
        Prg:     chakrapani
  
*********************************************************************21*/
#ifdef TENB_AS_SECURITY
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_FILE_ID=248;
static int RLOG_MODULE_ID=1024;
#endif

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
#include "kwu.h"           /* KWU defines */
#include "pju.h"           /* PJU defines */
/*#include "rgu.h"*/           /* RGU defines */
#include "cpj.h"           /* CPJ defines */
#include "lpj.h"           /* LPJ defines */
#include "pju.h"           /* PJU defines */
#include "pj_env.h"        /* RLC environment options */
#include "pj.h"            /* RLC defines */
#include "pj_dl.h"

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
#include "kwu.x"           /* KWU */
#include "pju.x"           /* PJU */
/*#include "rgu.x" */          /* RGU */
#include "cpj.x"           /* CPJ */
#include "lpj.x"           /* LPJ */
#include "pju.x"           /* PJU */
#include "pj.x"
#include "pj_udx.h"          /* LIB */
#include "pj_udx.x"          /* LIB */
#include "pj_dl.x"
#include "pj_ul.x"
#include "pj_lib.x"     /* LIB */
#ifdef TENB_AS_SECURITY
#include "pj_ptsec.h"
#include "pj_ptsec.x"
#endif
#include "tfu.h"
#include "tfu.x"
#include "pj_ptsec.h"
#include "pj_ptsec.x"
#ifdef LTE_TDD
#define PJ_MAX_SDUS_TO_WAIT_FOR_ACK 1100 /* Maximum number of SDUs PDCP 
                                       will buffer waiting for ACKS */
#else
#define PJ_MAX_SDUS_TO_WAIT_FOR_ACK 900 /* Maximum number of SDUs PDCP 
                                       will buffer waiting for ACKS */
#endif

PUBLIC U32 admCntrPktsDrop=0;
PUBLIC U32 pdcpPktsDrop=0;
PUBLIC U32 pdcpPktsRcvd=0;
#ifdef TENB_AS_SECURITY
PUBLIC U32 pjQcomHdlOpenReq(PTR *fd);
PUBLIC U32 pjQcomHdlCloseReq(PTR *fd);
PUBLIC U32 pjQcomPrepPacketForCiphDeciph(PjLibTrans *libTrans, Buffer *mBuf,Buffer *opSdu);
PUBLIC U32 pjQcomIntegOperation(PjLibTrans *libTrans, Buffer *mBuf);

PUBLIC U8 pjQcomIntKeyReg(U16 ueId, U8 *key);

EXTERN U32 numDeciphReq, numDeciphRsp, numCiphReq, numCiphRsp;

EXTERN U32 pjQcomBatchAndDeCipherPackets ARGS ((Void));
EXTERN Void pjQcomInitUlQue ARGS ((Void));

#ifdef TENB_T2K3K_SPECIFIC_CHANGES
PRIVATE Void pjMsSpaccProcessDlBatchOutput ARGS ((Void));
PRIVATE Void pjMsSpaccProcessUlBatchOutput ARGS((Void));
PUBLIC U32 pjMsCheckSpaccQueue(Bool isUl);
PUBLIC Void pjSpaccDeInitCiphQ(Void);
PUBLIC Void pjSpaccDeInitDeCipherQ(Void);
#endif
#endif

#ifdef PTPJLIB
#ifdef PJ_CMP_ASYNC
/************************************************************************
                             PJU Interface Matrices
************************************************************************/

/****************************************************************************
 *                         PJU Interface Mt functions
 ***************************************************************************/

#ifdef ANSI
PUBLIC S16 PjLibObdCmpInitReq
(
Pst * pst,
PjCmpCxt cmpCxt,
PjRohc rohc
)
#else
PUBLIC S16 PjLibObdCmpInitReq(pst, cmpCxt, rohc)
Pst * pst;
PjCmpCxt cmpCxt;
PjRohc rohc;
#endif
{

   TRC3(PjLibObdCmpInitReq)

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjLibObdCmpReq
(
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * mBuf
)
#else
PUBLIC S16 PjLibObdCmpReq(pst, cmpCxtId, libTrans, mBuf)
Pst * pst;
PTR cmpCxtId;
PjLibTrans libTrans;
Buffer * mBuf;
#endif
{

   TRC3(PjLibObdCmpReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdDecmpReq
(
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer * mBuf
)
#else
PUBLIC S16 PjLibObdDecmpReq(pst, cmpCxtId, libTrans, mBuf)
Pst * pst;
PTR cmpCxtId;
PjLibTrans libTrans;
Buffer * mBuf;
#endif
{

   TRC3(PjLibObdDecmpReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCmpCloseReq
(
Pst * pst,
PTR cmpCxtId
)
#else
PUBLIC S16 PjLibObdCmpCloseReq(pst, cmpCxtId)
Pst * pst;
PTR cmpCxtId;
#endif
{

   TRC3(PjLibObdCmpCloseReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCmpResetReq
(
Pst * pst,
PTR cmpCxtId
)
#else
PUBLIC S16 PjLibObdCmpResetReq(pst, cmpCxtId)
Pst * pst;
PTR cmpCxtId;
#endif
{

   TRC3(PjLibObdCmpResetReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PjLibObdCmpFbReq
(
Pst * pst,
PTR cmpCxtId,
PjLibTrans libTrans,
Buffer *feedback
)
#else
PUBLIC S16 PjLibObdCmpFbReq(pst, cmpCxtId, libTrans, feedback)
Pst * pst;
PTR cmpCxtId;
PjLibTrans libTrans;
Buffer *feedback;
#endif
{

   TRC3(PjLibObdCmpFbReq)

   RETVALUE(ROK);

}

#else
#ifdef ANSI
PUBLIC S16 PjLibObdCipherCloseReq
(
Void *cpCxtId
)
#else
PUBLIC S16 PjLibObdCipherCloseReq(cpCxtId)
Void *cpCxtId
#endif
{
   TRC3(PjLibObdCipherCloseReq)

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 pjLibCmpInitReq 
(
PjCmpCxt cmpCxt,                  /* Context for ROHC */
PjRohc   rohc,                    /* ROHC Config Info */
PTR      *cmpCxtId                /* Compression Context ID */
)
#else
PUBLIC S16 pjLibCmpInitReq(cmpCxt, rohc, cmpCxtId)
PjCmpCxt cmpCxt;                  /* Context for ROHC */
PjRohc   rohc;                    /* ROHC Config Info */
PTR      *cmpCxtId;               /* Compression Context ID */
#endif
{
   TRC3(pjLibCmpInitReq)

   RETVALUE(ROK);

} /* end of pjLibCmpInitReq */

#ifdef ANSI
PUBLIC S16 pjLibCmpReq 
(
PTR      cmpCxtId,                /* Context Id for compression */
Buffer   *mBuf,                   /* SDU to be compressed */
Buffer   **opSdu                  /* Compressed SDU */
)
#else
PUBLIC S16 pjLibCmpReq(cmpCxtId, mBuf, opSdu)
PTR      cmpCxtId;                /* Context Id for compression */
Buffer   *mBuf;                   /* SDU to be compressed */
Buffer   **opSdu;                 /* Compressed SDU */
#endif
{
   TRC3(pjLibCmpReq)

   RETVALUE(ROK);

} /* end of pjLibCmpReq */

#ifdef ANSI
PUBLIC S16 pjLibDecmpReq 
(
PTR      cmpCxtId,                /* Context Id for compression */
Buffer   *mBuf,                   /* SDU to be decompressed */
Buffer   **feedback,                 /* Decompressed SDU */
Buffer   **opSdu               /* ROHC feedback buffer */
)
#else
PUBLIC S16 pjLibDecmpReq(cmpCxtId, mBuf, feedback, opSdu)
PTR      cmpCxtId;                /* Context Id for compression */
Buffer   *mBuf;                   /* SDU to be decompressed */
Buffer   **feedback;                 /* Decompressed SDU */
Buffer   **opSdu;              /* ROHC feedback buffer */
#endif
{
   TRC3(pjLibDecmpReq)

   RETVALUE(ROK);

} /* end of pjLibDecmpReq */

#ifdef ANSI
PUBLIC S16 pjLibCmpCloseReq
(
PTR      cmpCxtId                 /* ROHC Context to be closed */
)
#else
PUBLIC S16 pjLibCmpCloseReq(cmpCxtId)
PTR      cmpCxtId;                /* ROHC Context to be closed */
#endif
{
   TRC3(pjLibCmpCloseReq)

   RETVALUE(ROK);

} /* end of pjLibCmpCloseReq */

#ifdef ANSI
PUBLIC S16 pjLibCmpResetReq
(
PTR      cmpCxtId                 /* ROHC Context to be closed */
)
#else
PUBLIC S16 pjLibCmpResetReq(cmpCxtId)
PTR      cmpCxtId;                /* ROHC Context to be closed */
#endif
{
   TRC3(pjLibCmpResetReq)

   RETVALUE(ROK);

} /* end of pjLibCmpResetReq */

#ifdef ANSI
PUBLIC S16 pjLibCmpFbReq
(
PTR      cmpCxtId,                /* ROHC Context to be closed */
Buffer   *feedback                /* Feed back */
)
#else
PUBLIC S16 pjLibCmpFbReq(cmpCxtId, feedback)
PTR      cmpCxtId;                /* ROHC Context to be closed */
Buffer   *feedback;               /* Feed back */
#endif
{
   TRC3(pjLibCmpFbReq)

   RETVALUE(ROK);

} /* end of pjLibCmpFbReq */

#endif /* PJ_CMP_ASYNC */

#ifdef ANSI
PUBLIC S16 PjLibObdIntCloseReq
(
Void *intCxtId
)
#else
PUBLIC S16 PjLibObdIntCloseReq(*intCxtId)
Void *intCxtId;
#endif
{
   TRC3(PjLibObdIntCloseReq)
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 pjLibIntInitReq 
(
PjSecCxt secCxt,                  /* Context for Intg protection/verification */
PjIntInfo intInfo,                /* Integrity Cfg Info */
Void      **cxtId                 /* FD */
)
#else
PUBLIC S16 pjLibIntInitReq(secCxt, intInfo, cxtId)
PjSecCxt secCxt;                  /* Context for Intg protection/verification */
PjIntInfo intInfo;                /* Integrity Cfg Info */
Void      **cxtId;                /* FD */
#endif
{
   S16   ret = ROK;

   TRC3(pjLibIntInitReq)

   RETVALUE(ret);

} /* end of pjLibIntInitReq */

#ifdef ANSI
PUBLIC S16 pjLibCpInitReq
(
PjSecCxt secCxt,                  /* Context for Intg protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *cpKey,                  /* Control plane ciphering key */
Void     **cxtId                  /* FD */
)
#else
PUBLIC S16 pjLibCpInitReq(secCxt, algoType, cpKey, cxtId)
PjSecCxt secCxt;                  /* Context for Intg protection/verificatino */
U8       algoType;                /* Type of the algorithm */
U8       *cpKey;                  /* Control plane ciphering key */
Void     **cxtId;                 /* FD */
#endif
{
   S16   ret = ROK;

   TRC3(pjLibCpInitReq)

   RETVALUE(ret);

} /* end of pjLibCpInitReq */

#ifdef ANSI
PUBLIC S16 pjLibUpInitReq
(
PjSecCxt secCxt,                  /* Context for Intg protection/verificatino */
U8       algoType,                /* Type of the algorithm */
U8       *upKey,                  /* User plane ciphering key */
Void     **cxtId                  /* FD */
)
#else
PUBLIC S16 pjLibUpInitReq(secCxt, algoType, upKey, cxtId)
PjSecCxt secCxt;                  /* Context for Intg protection/verificatino */
U8       algoType;                /* Type of the algorithm */
U8       *upKey;                  /* User plane ciphering key */
Void     **cxtId;                 /* FD */
#endif
{
   S16   ret = ROK;

   TRC3(pjLibUpInitReq)


   RETVALUE(ret);

} /* end of pjLibUpInitReq */



#ifdef ANSI
PUBLIC S16 PjLibObdCpInitReq
(
PjSec *secInp
)
#else
PUBLIC S16 PjLibObdCpInitReq(*secInp)
PjSec *secInp;
#endif
{
   TRC3(PjLibObdCpInitReq)
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 PjLibObdUpInitReq
(
PjSec *secInp
)
#else
PUBLIC S16 PjLibObdUpInitReq(*secInp)
PjSec *secInp
#endif
{
   TRC3(PjLibObdUpInitReq)
   RETVALUE(ROK);
}

#ifdef TENB_AS_SECURITY

#ifdef TENB_T2K3K_SPECIFIC_CHANGES
/**
 *
 * @brief Extracting gCb, rbCb, txEnt from SPAcc Mapping Information.
 *       
 *
 * @b Description: This function extracts gCb, rbCb, txEnt from SPAcc mapping
 * info which is passed as one of the input params.
 *
 *
 *  @param[in] spaccInfo : Information required to map o/p PDU from SPACc to
 *  PDCP TxEnt.
 *  @param[in] gCb       : PDCP Control Block
 *  @param[in] pjRbCb    : PDCP rbCb
 *  @param[in] txEnt     : PDCP TxEnt
 *
 * 
 *  @return  Void
 */

#ifdef ANSI
PRIVATE S16 pjExtractTxEntRbCbUsingSpaccMappingInfo 
(
PjMsSPACCOutputMapInfo *spaccInfo,
PjCb   **gCb,
PjDlRbCb **pjRbCb,
PjTxEnt  **txEnt
)
#else
PRIVATE S16 pjExtractTxEntRbCbUsingSpaccMappingInfo(spaccInfo, gCb, pjRbCb, txEnt)
PjMsSPACCOutputMapInfo *spaccInfo;
PjCb   **gCb;
PjDlRbCb **pjRbCb;
PjTxEnt  **txEnt;
#endif
{
   PjDlUeCb   *ueCb = NULLP;

   *gCb = PJ_GET_PJCB(spaccInfo->pdcpInstance);

   pjDbmFetchDlUeCb(*gCb, spaccInfo->pdcpId.ueId,spaccInfo->pdcpId.cellId, &ueCb);

   if(ueCb  == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_UEID,spaccInfo->pdcpId.ueId,"UECB not found !!!! ");
      RETVALUE(RFAILED);
   }


   pjDbmFetchPjDlRbCb(ueCb, spaccInfo->pdcpId.rbId, spaccInfo->pdcpId.rbType, pjRbCb);
   if(*pjRbCb  == NULLP)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,spaccInfo->pdcpId.ueId, 
            "CellId[%u]:RbId[%d] not found",
            spaccInfo->pdcpId.cellId, spaccInfo->pdcpId.rbId);
      RETVALUE(RFAILED);
   }

   *txEnt = (PjTxEnt *)pjDbmGetTxEnt(*gCb, &((*pjRbCb)->dlCb.txBuf),
         spaccInfo->count);

   if(*txEnt == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_UEID,spaccInfo->pdcpId.ueId,"!!!!rxEnt is NULL!!!!");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/**
 *
 * @brief Handler for Processing Ciphered PDU's from SPAcc.
 *       
 *
 * @b Description: This function processes and extracts the required control
 * blocks for the ciphered PDU  and calls the function for to be processed
 * further.
 *
 *  @param[in] spaccOpInfo : Information required to process PDU
 *  @param[in] opSdu    : Ciphered buffer
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */

#ifdef ANSI
PUBLIC S16 pjProcCiphPdu 
(
Buffer *opSdu,
PjMsSPACCOutputMapInfo *spaccOpInfo
)
#else
PUBLIC S16 pjProcCiphPdu(opSdu, spaccOpInfo)
Buffer *opSdu;
PjMsSPACCOutputMapInfo *spaccOpInfo;
#endif
{
   U32 ret;
   PjCb   *gCb = NULLP;
   PjDlRbCb *pjRbCb = NULLP;
   PjTxEnt  *txEnt = NULLP;

   /* Extract gCb, rbCb and txEnt for further processing */
   ret = pjExtractTxEntRbCbUsingSpaccMappingInfo(spaccOpInfo, &gCb, &pjRbCb, &txEnt);

   if(ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId,
            "pjExtractTxEntRbCbUsingSpaccMappingInfo: returned failure ");
      PJ_FREE_BUF(opSdu);
      RETVALUE(RFAILED);
   }
   else
   {
      if(pjRbCb->rbType == PJ_SRB)
      {
        if(txEnt->pdu != NULLP)
        {
            PJ_FREE_BUF(txEnt->pdu);
        }
      }
      /* In case of Reestablish/HO , dropping the ciphered packets. These packets
       * are processed again when events EVTCPJDATRESUMEREQ/EVTPJUDATFWDREQ are 
       * received from upper layer */ 
      if((pjRbCb->ueCb->libInfo.state != PJ_STATE_NORMAL) && (pjRbCb->rbType != PJ_SRB))
      {
	      RLOG_ARG0(L_INFO,DBG_UEID,spaccOpInfo->pdcpId.ueId,
               "Dropping the packet in reestablish state "); 
	      PJ_FREE_BUF(opSdu);
	      RETVALUE(RFAILED);
      }
      txEnt->pdu = opSdu;

      /* Call to deliver PDU to RLC */
      ret = pjDlmDeliverPdu(gCb, pjRbCb, txEnt);
      if(ret != ROK)
      {
         RETVALUE(RFAILED);
         RLOG_ARG0(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId,
               "pjProcCiphPdu: pjDlmDeliverPdu failed !!!!");   
      }
   }

   RETVALUE(ROK);
}


/**
 *
 * @brief Handler for Ciphering failure 
 *       
 *
 * @b Description: This function handles the steps to be taken on failure of
 *                 ciphering 
 *
 *
 *  @param[in] procInfo : Information required to process PDU
 *
 * 
 *  @return  Void
 */

#ifdef ANSI
PUBLIC S16 pjHndlCiphFail 
(
PjMsSPACCOutputMapInfo *spaccInfo
)
#else
PUBLIC S16 pjHndlCiphFail(spaccInfo)
PjMsSPACCOutputMapInfo *spaccInfo;
#endif
{
   PjCb   *gCb = NULLP;
   PjDlRbCb *pjRbCb = NULLP;
   PjTxEnt  *txEnt = NULLP;

   /* Extract gCb, rbCb and txEnt for further processing */
   if((pjExtractTxEntRbCbUsingSpaccMappingInfo(spaccInfo, &gCb, &pjRbCb, &txEnt)) != ROK)
   {
      RETVALUE(RFAILED);
   }
   else
   {
       PJ_UPD_DL_VAR(gCb, pjRbCb, txEnt->count);
       if ( pjRbCb->dlCb.cfmReqd)
       {
          PJ_SND_CFM(gCb,pjRbCb, txEnt,
                     PJU_CIPHER_FAILED);
       }
       else
       {
          pjDbmDelTxEnt(gCb, &(pjRbCb->dlCb.txBuf), txEnt->count);   
       }
       gCb->pjGenSts.numCiphFails++;
   }

   RETVALUE(ROK);
}
#ifdef SPACC_THREADED
PRIVATE U32 globPollCount = 50;

/**
 *
 * @brief Handler for SPACC Reset 
 *       
 *
 * @b Description: This is triggeres when the processing of batch at SPACC
 *                 is delayed
 *
 *
 * 
 *  @return  Void
 */


#ifdef ANSI
PRIVATE Void pjMsSpaccReset(Void)
#else
PRIVATE Void pjMsSpaccReset()
#endif
{
   U32 ret=ROK;
   U32 jobNum=1;
   Buffer *opSdu=NULLP;
   PjMsSPACCOutputMapInfo spaccInfo;

   RLOG0(L_DEBUG,"Restarting SPACC");
//   PjLibObdDlCloseReq();
   pjMsSPAccRestart();
   RLOG0(L_DEBUG,"SPACC Restarted");

   for(;;)
   {
      ret = pjMsSpaccFetchDlPduInfo(&spaccInfo, &opSdu, jobNum, RFAILED);
      if(ret == ROKDNA)
      {
         RLOG1(L_ERROR, "NUM DL jobs at failure:%ld",jobNum);
         break;
      }
      pjHndlCiphFail(&spaccInfo);
      jobNum++;
   }

   pjMsSetQFreeDl();

   jobNum = 1;

   for(;;)
   {
      ret = pjMsSpaccFetchUlPduInfo(&spaccInfo, &opSdu, jobNum, RFAILED);
      if(ret == ROKDNA)
      {
         RLOG1(L_ERROR,"NUM UL jobs at failure:%ld",jobNum);
         break;
      }
      pjHdlDecipherFailure(&spaccInfo);
      jobNum++;
   }

   pjMsSetQFreeUl();

   //RLOG0(L_DEBUG, "pjMsSetQFreeUl done");
   //RLOG0(L_DEBUG, "Reopening SPACC DL");
   //pjMsSpaccHdlDlfdOpenReq();
  // RLOG0(L_DEBUG, "Reopened SPACC DL");

   RETVOID;
}
#endif
/**
 *
 * @brief Handler for NRT TTI Indication to PDCP for DL batch processing 
 *       
 *
 * @b Description: Triggers batch processing to SPAcc. Fetches the ciphered PDUs
 *                 from SPAcc and triggers sending the PDU's RLC.
 *
 *
 * 
 *  @return  Void
 */


#ifdef ANSI
PUBLIC Void PjDlSpaccTrigger(Void)
#else
PUBLIC Void PjDlSpaccTrigger()
#endif
{

   U32 ret=ROK;
   U32 jobNum=1;
   Buffer *opSdu=NULLP;
   PjMsSPACCOutputMapInfo spaccInfo;
   static U32 dlPollCount = 0;

#ifdef SPACC_NONBLOCKING

   S32 pollop;
   pollop = pjMsSpaccPollDlQueue();
   if(PJ_MS_POLL_NOT_REQUIRED == pollop)
   {
      ret = pjMsSpaccBatchAndCipherPackets();
//      pjMsSpaccSetResetDlQueueFree(0);  /* Set to FALSE */
   }
   else
   {
      if(PJ_MS_POLL_COMPLETE == pollop)
      {
         dlPollCount = 0;
         /* Process the o/p from SPAcc */
         pjMsSpaccProcessDlBatchOutput();
//         pjMsSpaccSetResetDlQueueFree(1);  /* Set to TRUE */

         /* Send next set of data to SPAcc after processing */
         ret = pjMsSpaccBatchAndCipherPackets();
//         pjMsSpaccSetResetDlQueueFree(0);  /* Set to FALSE */
      }
      else
      {
#ifdef SPACC_THREADED
         /*Workaround for the SPACC issue of "unexpected job"*/
         dlPollCount++;
         if(dlPollCount >= globPollCount)
         {
            U32 numUnexpJobs = 0;
            dlPollCount = 0;
            numUnexpJobs = pjMsCheckUnExpJobs();
            if(numUnexpJobs != 0)
            {
               pjMsSpaccReset();
            }
         }
#endif 
         /* Do nothing as SPAcc is not done with processing */
      }
   }

   /* This condition is executed when SPAcc returns failure for whatever
   * reason.So once failure is returned, we fetch all the PDU's we had sent
   * for ciphering and process them as failure */
   if(ret != ROK)
   {
      RLOG0(L_ERROR,"Error leg in DL Spacc trigger hit ");
      for(;;)
      {
         ret = pjMsSpaccFetchDlPduInfo(&spaccInfo, &opSdu, jobNum, ret);
         if(ret == ROKDNA)
         {
            break;
         }
         pjHndlCiphFail(&spaccInfo);
         jobNum++;
      }
   }

#else

   ret = pjMsSpaccBatchAndCipherPackets();

   /* This condition is executed when SPAcc returns failure for whatever
    * reason.So once failure is returned, we fetch all the PDU's we had sent
    * for ciphering and process them as failure */
   if(ret != ROK)
   {
      for(;;)
      {
         ret = pjMsSpaccFetchDlPduInfo(&spaccInfo, &opSdu, jobNum, ret);
         if(ret == ROKDNA)
         {
            break;
         }
         pjHndlCiphFail(&spaccInfo);
         RETVOID;
      }
   }

   pjMsSpaccProcessDlBatchOutput();

#endif

   RETVOID;
}

/**
 *
 * @brief Handler for processing of Decipher Failure
 *       
 *
 * @b Description: This function handles decipher failures by deleting the
 * RxEnt,  send STA_IND to upper layer and updation of general statistics.
 *
 *
 *  @param[in] spaccOpInfo : Information required to process PDU
 *
 * 
 *  @return  Void
 */

#ifdef ANSI
PUBLIC S16 pjHdlDecipherFailure 
(
PjMsSPACCOutputMapInfo *spaccOpInfo
)
#else
PUBLIC S16 pjHdlDecipherFailure()
PjMsSPACCOutputMapInfo *spaccOpInfo;
#endif
{
   PjCb   *gCb;
   PjUlUeCb   *ueCb = NULLP;
   PjUlRbCb *pjRbCb = NULLP;
   PjRxEnt  *rxEnt = NULLP; 

   gCb = PJ_GET_PJCB(spaccOpInfo->pdcpInstance);

   pjDbmFetchUlUeCb(gCb, 
                    spaccOpInfo->pdcpId.ueId,
                    spaccOpInfo->pdcpId.cellId, 
                    &ueCb);

   if(ueCb  == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId,"UECB not found !!!! ");
      RETVALUE(RFAILED);
   }

   /*     RLOG_ARG4(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId,
    *     "At PDCP: : RBTYPE:%d, CELLID:%d, RBID:%d, 
         COUNT:%d", spaccOpInfo->pdcpId.rbType, spaccOpInfo->pdcpId.cellId,  
         spaccOpInfo->pdcpId.rbId, spaccOpInfo->count);*/
   /*   RLOG_ARG2(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId,
          "SNLEN:%d, numjobs %d "spaccOpInfo->pdcpId.ueId,  \
          spaccOpInfo->snLen,  numJobs); */

   pjDbmFetchPjUlRbCb(ueCb, spaccOpInfo->pdcpId.rbId, spaccOpInfo->pdcpId.rbType, &pjRbCb);
   if(pjRbCb  == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId,"RBCB not found !!!!");
      RETVALUE(RFAILED);
   }

   rxEnt = (PjRxEnt *)pjDbmGetRxEnt(gCb, 
                                    &(pjRbCb->ulCb.recBuf),
                                    spaccOpInfo->count);

   if(rxEnt == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId,"!!!!rxEnt is NULL!!!!");
      RETVALUE(RFAILED);
   }

   PJ_SND_PJU_STA_IND(gCb,pjRbCb, rxEnt);
   pjDbmDelRxEnt(gCb, &(pjRbCb->ulCb.recBuf), rxEnt->count);
   PJ_STS_INC_GEN_CNT(gCb,numDeciphFails);

   RETVALUE(ROK);
}

/**
 *
 * @brief Handler for processing deciphered PDU 
 *       
 *
 * @b Description: This function processes and sends the ciphered PDU to RLC
 *
 *
 *  @param[in] spaccOpInfo : Information required to process PDU
 *  @param[in] opSdu    : Deciphered buffer
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */

#ifdef ANSI
PUBLIC S16 pjProcDeCipherPdu 
(
PjMsSPACCOutputMapInfo *spaccOpInfo,
Buffer *opSdu
)
#else
PUBLIC S16 pjProcDeCipherPdu()
PjMsSPACCOutputMapInfo *spaccOpInfo;
Buffer *opSdu;
#endif
{
   U32 ret;
   PjCb   *gCb;
   PjUlUeCb   *ueCb = NULLP;
   PjUlRbCb *pjRbCb = NULLP;
   PjRxEnt  *rxEnt = NULLP; 

   gCb = PJ_GET_PJCB(spaccOpInfo->pdcpInstance);

   pjDbmFetchUlUeCb(gCb, 
                    spaccOpInfo->pdcpId.ueId,
                    spaccOpInfo->pdcpId.cellId, 
                    &ueCb);
   
   if(ueCb  == NULLP)
   {
      RLOG_ARG0(L_WARNING,DBG_UEID,spaccOpInfo->pdcpId.ueId,"UECB not found !!!! ");
      RETVALUE(RFAILED);
   }

/*     RLOG_ARG4(L_ERROR, DBG_UEID,spaccOpInfo->pdcpId.ueId,
       "UDAY: At PDCP: : RBTYPE:%d, CELLID:%d, RBID:%d,  COUNT:%d", 
       spaccOpInfo->pdcpId.rbType, spaccOpInfo->pdcpId.cellId, 
       spaccOpInfo->pdcpId.rbId, spaccOpInfo->count,);*/
/*     RLOG_ARG2(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId,
       " SNLEN:%d, numjobs %d ",spaccOpInfo->snLen,  numJobs); */

   pjDbmFetchPjUlRbCb(ueCb, spaccOpInfo->pdcpId.rbId, spaccOpInfo->pdcpId.rbType, &pjRbCb);
   if(pjRbCb  == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId,"RBCB not found !!!! ");
      RETVALUE(RFAILED);
   }

   rxEnt = (PjRxEnt *)pjDbmGetRxEnt(gCb, &(pjRbCb->ulCb.recBuf),
         spaccOpInfo->count);

/* To be used for debugging. Will be removed once verified */

   if(rxEnt == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId,"!!!!rxEnt is NULL!!!!");
      RETVALUE(RFAILED);
   }  

   /*Free the source buffer here*/
   if(rxEnt->mBuf)
   {
      PJ_FREE_BUF(rxEnt->mBuf);
   }
 
   rxEnt->mBuf = opSdu;


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
      RLOG_ARG0(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId," 3. Deleting RxEnt");
      /* duplicate entry */
      PJ_FREE_BUF(rxEnt->mBuf);
      PJ_FREE(gCb,rxEnt, sizeof(PjRxEnt));
   }
   else if(rxEnt->discFlag == TRUE)
   {
      RLOG_ARG0(L_ERROR,DBG_UEID,spaccOpInfo->pdcpId.ueId,"4. Deleting RxEnt");
      /* duplicate entry */
      pjDbmDelRxEnt(gCb, &pjRbCb->ulCb.recBuf, rxEnt->count);
   }   
   RETVALUE(ROK);

}
#endif

/**
 *
 * @brief Handler for NRT TTI Indication to PDCP for UL batch processing 
 *       
 *
 * @b Description: Triggers batch processing of packets lined up for 
 *                 deciphering. Fetches the ciphered PDUs
 *                 and triggers further processing at PDCP. 
 *
 *
 * 
 *  @return  Void
 */


#ifdef ANSI
PUBLIC Void PjUlSpaccTrigger(Void)
#else
PUBLIC Void PjUlSpaccTrigger(Void)
#endif
{

   pjQcomBatchAndDeCipherPackets();

   RETVOID;
}



/**
*@brief
* This function calls the function which interfaces with SPAcc
* for opening of the fd. Also, it initializes the SPAcc DL Queues.
*
*@inparam[in] gCb  PDCP Instance Control Block
*@return S16
*/

#ifdef ANSI
PUBLIC S16 PjLibObdDlInitReq
(
PjCb    *gCb
)
#else
PUBLIC S16 PjLibObdDlInitReq(gCb)
PjCb    *gCb;
#endif
{
   TRC3(PjLibObdDlInitReq)

   RETVALUE(ROK);
}

/**
*@brief
* This function calls the function which interfaces with SPAcc
* for opening of the fd. Also, it initializes the SPAcc UL Queues.
*
*@inparam[in] gCb  PDCP Instance Control Block
*@return S16
*
*/

#ifdef ANSI
PUBLIC S16 PjLibObdUlInitReq
(
PjCb    *gCb
)
#else
PUBLIC S16 PjLibObdUlInitReq(gCb)
PjCb    *gCb;
#endif
{
   TRC3(PjLibObdUlInitReq)

   /*Call to open file descriptors for interfacing with SPACC*/
   pjQcomInitUlQue();

   RETVALUE(ROK);
}


/**
 *
 * @brief Handler for invoking key derivation function for KeNB*. This is used
 * for Handover cases.
 *       
 *
 * @Description: When the eNB decides to perform an intra-eNB handover it
 * shall derive KeNB* using target PCI, its frequency EARFCN-DL, and 
 * either NH or the current KeNB.
 *
 * @input: gCb  - Control Block
 *         kenbInf - Input structure for KeNB* derivation.
 *         kdfCfm  - Output filled into this structure after derivation.
 *
 *  @return  S16
 */
#ifdef ANSI
PUBLIC S16 pjLibObdPrcKenbStarCfg
(
PjCb                   *gCb,
CpjKenbStarInfo*       kenbInf,
CpjKenbStarCfmInfo     *kdfCfm
)
#else
PUBLIC S16 pjLibObdPrcKenbStarCfg(gCb, kenbInf, kdfCfm)
PjCb                   *gCb;
CpjKenbStarInfo*       kenbInf;
CpjKenbStarCfmInfo     *kdfCfm;
#endif
{

   TRC2(pjLibObdPrcKenbStarCfg)

#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   PJ_ALLOC(gCb, pSecInfo , sizeof(PjMsSecInfo));
   if(pSecInfo == NULLP)
   {
      RLOG1(L_ERROR, "YsMsUlmPrcKenbStarCfg failed line %d ", __LINE__);
      RETVALUE(RFAILED);
   }


   cmMemcpy(pSecInfo->hash_key, kenbInf->secKey, CPJ_SEC_KEY_LEN);

   /* Building the String S = FC || P0 || L0 || P1 || L1 */

   /*Values are initialized as per section A7 33.401*/
   pSecInfo->strS[0] = 0x13; 
   pSecInfo->strS[3] = 0x00;
   pSecInfo->strS[4] = 0x02;
   pSecInfo->strS[7] = 0x00;
   pSecInfo->strS[8] = 0x02;


   RLOG2(L_DEBUG,"Value of Cellid, dl Freq is %d %d ", 
         kenbInf->physCellId, kenbInf->dlCarrFreq);
   /*******************************************
    *************kENB* from kENB*************
    *******************************************/

   /*PCI*/
   pSecInfo->strS[1] = kenbInf->physCellId >> 8;
   pSecInfo->strS[2] = (U8)kenbInf->physCellId;

   /*DL frequency*/
   pSecInfo->strS[5] = kenbInf->dlCarrFreq >> 8;
   pSecInfo->strS[6] = (U8)kenbInf->dlCarrFreq;

   pSecInfo->strSlen = 9;

   if((pjSendToKeyDeriveFunc(pSecInfo, kdfCfm->secKey)) != ROK)
   {
      RLOG0(L_ERROR, "In pjLibObdPrcKenbStarCfg: pjMsKDF failed");
      ret = RFAILED;
   }/*End of if(ret!=ROK)*/

   PJ_FREE(gCb, pSecInfo, sizeof(PjMsSecInfo));

   RETVALUE(ret);
#endif
   RETVALUE(ROK);
}/*End of function*/

/**
 *
 * @brief Handler for invoking key derivation function for KeNB. KeNB is Key
 * derivation funtion. This is normal Ciphering/Integrity Key derivation at
 * eNB.
 *       
 *
 * @Description: For Security to be enabled, Integrity and Ciphering keys need
 * to be derived. These are applied on SRB/DRB data and the Ciphered/Integrity
 * protected PDU are sent to UE.
 *
 * @input: gCb  - Control Block
 *         kenbInf - Input structure for KeNB* derivation.
 *         kdfCfm  - Output filled into this structure after derivation.
 *
 *  @return  S16
 */
#ifdef ANSI
PUBLIC S16 pjLibObdPrcKenbCfg
(
PjCb                   *gCb,
CpjAsKeyInfo*       kenbInf,
CpjAskeyCfmInfo     *kdfCfm
)
#else
PUBLIC S16 pjLibObdPrcKenbCfg(kenbInf)
PjCb                   *gCb;
CpjAsKeyInfo*       kenbInf;
CpjAskeyCfmInfo     *kdfCfm;
#endif
{

   TRC2(pjLibObdPrcKenbCfg)

   RETVALUE(ROK);
}/*End of function*/


/**
 *
 * @brief 
 *
 *        Provides input params like Keys, RbId, direction which would be used
 *        as input for creation of SPAcc i/p structure..
 *
 * @b Description:
 *
 *        This function sends Intergrity protection request with elevant input parameters.
 *
 *  @param[in]   pjRbCb       PDCP RbCb 
 *  @param[in]   intCxtId       Input parameters for integrity 
 *  @param[in]   mBuf         SDU to be compressed 
 *  @param[out]  libTrans      Input to integrity algo (RbId, Count, Direction) 
 *
 *  @return  S16
 *      -# ROK 
 *
 */

#ifdef ANSI
PUBLIC S16 PjLibObdIntProtReq
(
PjLibTrans  *libTrans,
Buffer      **mBuf
)
#else
PUBLIC S16 PjLibObdIntProtReq(libTrans, mBuf)
PjLibTrans libTrans;
Buffer      **mBuf;
#endif
{

   TRC3(PjLibObdIntProtReq)

   RETVALUE(pjQcomIntegOperation(libTrans, *mBuf));
   }


/**
 *
 * @brief 
 *
 *        Handler for redirecing Integration request to SPAcc module.
 *
 * @b Description:
 *
 *        This function sends Intergrity verification request along with relevant input parameters.
 *
 *  @param[in]   PjCb        PDCP Cb 
 *  @param[in]   PTR        Handle for Integrity verification (handle opened during spacc_dev_open)  
 *  @param[in]   PjLibTrans        Input parameters for integrity 
 *  @param[in]   mBuf          SDU to be compressed 
 *  @param[out]  macI        MAC-I 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdIntVerReq
(
PjLibTrans  *libTrans,
Buffer      *mBuf,
U32         macI
)
#else
PUBLIC S16 PjLibObdIntVerReq(libTrans, mBuf, macI)
PjLibTrans libTrans;
Buffer      *mBuf;
U32         macI;
#endif
{
   TRC3(PjLibObdIntVerReq)

   libTrans->macI = macI;

   RETVALUE(pjQcomIntegOperation(libTrans, mBuf));

}


/**
*@brief
* This function calls the function which interfaces with SPAcc
* for closing the DL fd. 
*
*@return S16
*        -ROK
*
*/
#ifdef ANSI
PUBLIC S16 PjLibObdDlCloseReq(Void)
#else
PUBLIC S16 PjLibObdDlCloseReq(Void)
#endif
{

   TRC3(PjLibObdDlCloseReq)
   /*pjMsSPAccDlfdCloseReq();*/
   RETVALUE(ROK);

}

/**
*@brief
* This function calls the function which interfaces with SPAcc
* for closing the UL fd. 
*
*@return S16
*        -ROK
*
*/
#ifdef ANSI
PUBLIC S16 PjLibObdUlCloseReq(Void)
#else
PUBLIC S16 PjLibObdUlCloseReq(Void)
#endif
{

   TRC3(PjLibObdUlCloseReq)
   /*pjMsSPAccUlfdCloseReq();*/
   RETVALUE(ROK);

}


/**
 *
 * @brief 
 *
 *        Handler for redirecing ciphering request to SPAcc Module.
 *
 * @b Description:
 *
 *        This function sends ciphering protection request along with relevant input parameters.
 *
 *  @param[in]   PjCb           PDCP Cb 
 *  @param[in]   PjLibTrans      libTrans( Input parameters for deciphering )
 *  @param[in]   mBuf             Data to be deciphered 
 *  @param[in]   opSdu            Deciphered SDU 
 *  @param[in]   cxtId          Handle for ciphering either for User/Control plane.
 *
 *  @return  S16
 *      -# ROK 
 *
 */

#ifdef ANSI
PUBLIC S16 PjLibObdCipherReq
(
PjLibTrans  *libTrans,
Buffer      *mBuf,
Buffer      **opSdu
)
#else
PUBLIC S16 PjLibObdCipherReq(libTrans, mBuf, opSdu)
PjLibTrans  *libTrans;
Buffer      *mBuf;
Buffer      **opSdu;
#endif
{
   U32 ret = ROK;
   MsgLen len;
   Data *dBuf;

   TRC3(PjLibObdCipherReq)


   /*QU_CRITLOG(QU_LOG_MESSAGE,"SCpyMsgMsg reg %d pool %d ops %p\n", gCb->init.region, gCb->init.pool, *opSdu);*/
   SFndLenMsg(mBuf, &len); 
   SGetPayloadFlatBuf(len + 4, &dBuf,4);
   SPayloadPtrFlush((U8 *)dBuf);
   SGetMsgUsingPtr(opSdu,(dBuf - 4), len);

   ret = pjQcomPrepPacketForCiphDeciph(libTrans, mBuf,*opSdu);

   RETVALUE(ret);
}


#ifdef FLAT_BUFFER_OPT
/**
 *
 * @brief 
 *
 *        Handler for redirecing ciphering request to SPAcc Module.
 *
 * @b Description:
 *
 *        This function sends ciphering protection request along with relevant input parameters.
 *
 *  @param[in]   PjCb           PDCP Cb 
 *  @param[in]   PjLibTrans     libTrans( Input parameters for deciphering )
 *  @param[in]   mBuf           Flat Buffer to be deciphered 
 *  @param[in]   opSdu          Deciphered SDU 
 *  @param[in]   cxtId          Handle for ciphering either for User/Control plane.
 *
 *  @return  S16
 *      -# ROK 
 *
 */

#ifdef ANSI
PUBLIC S16 PjLibObdCipherReqFB
(
PjCb     *gCb,
Void *cxtId,
PjLibTrans libTrans,
FlatBuffer  *mBuf,
Buffer      **opSdu
)
#else
PUBLIC S16 PjLibObdCipherReqFB(gCb, cxtId, libTrans, mBuf, opSdu)
PjCb     *gCb;
Void *cxtId;
PjLibTrans libTrans;
FlatBuffer  *mBuf;
Buffer      **opSdu;
#endif
{

   U32 ret = ROK;
   PjQcomSecInfo pSecInfo;
   PjDlUeCb *dlUeCb;

   TRC3(PjLibObdCipherReqFB)

   dlUeCb = (PjDlUeCb *)libTrans.ueCb;

   pjMsCopyCipherParams(gCb,
         &libTrans,
         &(dlUeCb->secInfo), 
         &(dlUeCb->key),
         &pSecInfo);

   ret = pjQcomPrepareAndQPacketForCiphFB((Void*)&pSecInfo, mBuf);

   RETVALUE(ret);
}
#endif


/**
 *
 * @brief 
 *
 *        Handler for redirecing deciphering request to SPAcc module.
 *
 * @b Description:
 *
 *        This function sends deciphering request along with relevant input parameters.
 *
 *  @param[in]   PjCb           PDCP Cb 
 *  @param[in]   PTR            Handle for Deciphering (stored during spacc_dev_open) 
 *  @param[in]   PjLibTrans           Input parameters for deciphering 
 *  @param[in]   mBuf             Data to be deciphered 
 *  @param[in]   opSdu            Deciphered SDU 
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 PjLibObdDecipherReq
(
PjLibTrans  *libTrans,
Buffer      *mBuf,
Buffer      **opSdu
)
#else
PUBLIC S16 PjLibObdDecipherReq(libTrans, mBuf, opSdu)
PjLibTrans  *libTrans;
Buffer      *mBuf;
Buffer      **opSdu;
#endif
{
   U32 ret = ROK;

   TRC3(PjLibObdDecipherReq)

   if(libTrans->rbType != PJ_DRB)
   {
      MsgLen len;
      Data *dBuf;

      SFndLenMsg(mBuf, &len);
      SGetPayloadFlatBuf(len, &dBuf,0);
      SPayloadPtrFlush((U8 *)dBuf);
      SGetMsgUsingPtr(opSdu,dBuf, len);
   }
   ret = pjQcomPrepPacketForCiphDeciph(libTrans, mBuf, *opSdu);
   if(ret == RFAILED)
   {
       if(libTrans->rbType != PJ_DRB)
       {
          PJ_FREE_BUF(*opSdu);
       }
//       PJ_FREE(gCb,libTrans, sizeof(PjLibTrans));
   }

   RETVALUE(ret);
}
#else

#ifdef ANSI
PUBLIC S16 pjLibObdPrcKenbCfg
(
PjCb                   *gCb,
CpjAsKeyInfo*       kenbInf,
CpjAskeyCfmInfo     *kdfCfm
)
#else
PUBLIC S16 pjLibObdPrcKenbCfg(gCb, kenbInf, kdfCfm)
PjCb                   *gCb;
CpjAsKeyInfo*       kenbInf;
CpjAskeyCfmInfo     *kdfCfm;
#endif
{
   TRC2(pjLibObdPrcKenbCfg);
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 pjLibObdPrcKenbStarCfg
(
PjCb                   *gCb,
CpjKenbStarInfo*       kenbInf,
CpjKenbStarCfmInfo     *kdfCfm
)
#else
PUBLIC S16 pjLibObdPrcKenbStarCfg(gCb, kenbInf, kdfCfm)
PjCb                   *gCb;
CpjKenbStarInfo*       kenbInf;
CpjKenbStarCfmInfo     *kdfCfm;
#endif
{

   TRC2(pjLibObdPrcKenbStarCfg);
   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 pjLibDecipherReq
(
PTR         intCxtId,           /* Context Id for compression */
PjSecInp    secInp,             /* Input for Deciphering */
Buffer      *mBuf,              /* SDU to be deciphered */
Buffer      **opSdu             /* deciphered SDU */
)
#else
PUBLIC S16 pjLibDecipherReq(intCxtId, secInp, mBuf, opSdu)
PTR         intCxtId;           /* Context Id for Deciphering */
PjSecInp    secInp;             /* Input for Deciphering */
Buffer      *mBuf;              /* SDU to be Deciphering */
Buffer      **opSdu;            /* Deciphering SDU */
#endif
{
   TRC3(pjLibDecipherReq)
   RETVALUE(ROK);

} /* end of pjLibDecipherReq */


#ifdef ANSI
PUBLIC S16 pjLibIntVerReq 
(
PTR        intCxtId,                /* Context Id for compression */
PjSecInp   secInp,                  /* Input for Integrity protection */                  
Buffer     *mBuf,                   /* SDU to be compressed */
U32        macI,                    /* Message authentication code for the SDU */
Status     *status                  /* Status of the Integrity verification */
)
#else
PUBLIC S16 pjLibIntVerReq(intCxtId, secInp, mBuf, macI, status)
PTR        intCxtId;                /* Context Id for compression */
PjSecInp   secInp;                  /* Input for Integrity protection */
Buffer     *mBuf;                   /* SDU to be compressed */
U32        macI;                    /* Message authentication code for the SDU */
Status     *status;                 /* Status of the Integrity verification */
#endif
{
   TRC3(pjLibIntVerReq)
   RETVALUE(ROK);

} /* end of pjLibIntVerReq */

#ifdef ANSI
PUBLIC S16 PjLibObdIntProtReq
(
PjCb     *gCb,
Void    *intCxtId,
PjLibTrans libTrans,
Buffer      **mBuf
)
#else
PUBLIC S16 PjLibObdIntProtReq(gCb, intCxtId, libTrans, mBuf)
PjCb     *gCb;
Void    *intCxtId;
PjLibTrans libTrans;
Buffer      **mBuf;
#endif
{
   TRC3(PjLibObdIntProtReq);
   RETVALUE(ROK);
}


#ifdef ANSI
PUBLIC S16 PjLibObdCipherReq
(
PjCb     *gCb,
Void *cxtId,
PjLibTrans libTrans,
Buffer      *mBuf,
Buffer      **opSdu
)
#else
PUBLIC S16 PjLibObdCipherReq(gCb, cxtId, libTrans, mBuf, opSdu)
PjCb     *gCb;
Void *cxtId;
PjLibTrans libTrans;
Buffer      *mBuf;
Buffer      **opSdu;
#endif
{

   TRC3(PjLibObdCipherReq);
   RETVALUE(ROK);

}
#endif

#ifdef TENB_T2K3K_SPECIFIC_CHANGES
PUBLIC U32 dbgSpaccThreshHoldDropCnt = 0;
PUBLIC U32 dbgPdcpQSizeThreshHoldDropCnt = 0;
PUBLIC U32 dbgPdcpMemCount = 0;
PUBLIC U32 pdcpDropMemTh = 0;
PUBLIC U32 dbgICCMemThreshHoldDropCnt = 0;
PUBLIC U32 dbgSSIMemThreshHoldDropCnt = 0;
PUBLIC U32 dbgICCMemAlarm;
PUBLIC U32 dbgPdcpQMsSpaccQFull = 0;
PUBLIC U32 dbgPdcpRbOverload = 0;
#endif
/* kw003.201 This function is for admission control at packet level */
#ifdef ANSI
PUBLIC S16 pjLibPktAdmCtrl
(
PjDlRbCb        *pjRbCb,
Buffer        *pkt
) 
#else
PUBLIC S16 pjLibPktAdmCtrl(pjRbCb, pkt)
PjDlRbCb        *pjRbCb;
Buffer        *pkt;
#endif 
{
   U32 admitPacket;
   TRC3(pjLibPktAdmCtrl)
   /* Patch kw004.201 */
#ifdef LTE_QCOM
      /*Without this check the entried in case of UDP goes upto 4000-10000.
        This is resulting in HO failure with UDP data*/
   if(pjRbCb->dlCb.txBuf.numEntries > 1500) /*As of CD6 no limit is set*/
      {
#ifdef TENB_STATS
      if (pjRbCb->ueCb->tenbStats)
      {
         pjRbCb->ueCb->tenbStats->stats.pdcp.dlPdcpAckWaitDropCnt++;
      }
#endif
      RETVALUE(RFAILED);
   }
#endif

#ifdef SS_CAVIUM
      if(pjRbCb->dlCb.txBuf.numEntries > 450)
      {
         RETVALUE(RFAILED);
      }
#endif
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   if(pjRbCb->dlCb.txBuf.numEntries > PJ_MAX_SDUS_TO_WAIT_FOR_ACK)
   {
      dbgPdcpQSizeThreshHoldDropCnt++;
      /*This is for AM bearers only.  FC for UDP on AM will also
        be taken care here. */
#ifdef TENB_STATS
      if (pjRbCb->ueCb->tenbStats)
      {
         pjRbCb->ueCb->tenbStats->stats.pdcp.dlPdcpAckWaitDropCnt++;
      }
#endif
      RETVALUE(RFAILED);
   }
   /* Check the SPAcc queue too as we need to start dropping the PDUs in case
    * if SPAcc queue is waiting more number of PDUs
    */
   if(FALSE == (pjMsCheckSpaccQueue(FALSE)))
   {
      dbgSpaccThreshHoldDropCnt++;
      dbgPdcpQMsSpaccQFull++;
      RETVALUE(RFAILED);
   }
   /* Memory threshold is reached then drop the packet  */
   if(isMemThreshReached(pjCb[1]->init.region) != ROK)
   {
      {
         extern U32 pdcpdrop;
         pdcpdrop++;
         //MSPD_LOG("at PDCPdrop %d new %d\n",pdcpdrop);
      }
      RETVALUE(RFAILED);
   }
#endif
#ifdef LTE_QCOM
   admitPacket = SSPktAdmCtrl();
   if(admitPacket == 0)
   {
      admCntrPktsDrop++;
      RETVALUE(RFAILED);
   }
#endif

   if ((pjRbCb->dropOnlyOne) ||
       ((pjRbCb->pktAdmitCnt) && (pjRbCb->pktCnt > pjRbCb->pktAdmitCnt)))
   {
#ifdef TENB_STATS
      /*flowCntrlAdmit cnt reached for RB. Drop the packets.*/
      if (pjRbCb->ueCb->tenbStats)
      {
         pjRbCb->ueCb->tenbStats->stats.pdcp.dlPdcpDropCnt++;
      }

#endif

#ifdef TENB_T2K3K_SPECIFIC_CHANGES
      dbgPdcpMemCount++;
#endif
      pjRbCb->dropOnlyOne = 0;
      RETVALUE(RFAILED);
   }
   /* Pkt admitted. Counting it in.*/
   pjRbCb->pktCnt++;
   pdcpPktsRcvd++;
   RETVALUE(ROK);

} /* end of pjLibUpCloseReq */


#endif /* PTPJLIB */
  
/********************************************************************30**
         End of file:     eq_ptlib.c@@/main/TeNB_Main_BR/12 - Mon Aug 11 16:42:51 2014
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
/main/1      ---         cp    1. Initial release.
/main/1      ---         gk    1. LTERLC Release 2.1
/main/2      kw001.201   vp    1. Patch  for HENB, changes for max RBs
                                    in cell and avoidance of float variables.
/main/5      kw003.201   vp    1. Fixes for re-segmentation, segement offset
                               value and optimization 
/main/6      kw004.201   ap    1. Added support for SS_CAVIUM in the function 
                                  pjLibPktAdmCtrl
*********************************************************************91*/
