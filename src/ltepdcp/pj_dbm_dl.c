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
  
        Name:    PDCP - Database module file
    
        Type:    C source file
  
        Desc:    Source code for Database Module functions such as, 

                  -  pjDbmBufInit
                  -  pjDbmInsTxEnt
                  -  pjDbmGetTxEnt
                  -  pjDbmGetTxEntSn
                  -  pjDbmDelTxEnt
                  -  pjDbmTxDeInit
                  -  pjDbmInsRxEnt
                  -  pjDbmGetRxEnt
                  -  pjDbmDelRxEnt
                  -  pjDbmRxDeInit

        File:    gp_pj_dbm.c

        Sid:      eq_dbm_dl.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:46:44 2014
 
        Prg:     mn
  
*********************************************************************21*/
  
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=215;

/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

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
#include "pj_dl.h"
#include "pj_err.h"
#include "pj_ptsec.h"
/* extern (.x) include files */
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
#include "pj.x"                 /* RLC layer */
#include "pj_udx.h"
#include "pj_udx.x"
#include "pj_dl.x"
#include "pj_ul.x"
U32 pjTotDlPckCntPerCell;

/* local defines */

/* local externs */

/* forward references */

/* public variable declarations */
PUBLIC S16 pjCiphStateChek ARGS ((PjCb *gCb, PjTxEnt *txEnt));
/* This structure holds all the global structs we need. */

/* private variable declarations */

/* private function declarations */

/** @file gp_pj_dbm.c
@brief PDCP DBM Module
**/
/*****************************************************************************
 *                  HANDLER FOR THE TRANSMISSION BUFFER
 ****************************************************************************/

/**
 *
 * @brief Handler to initialize  Buffer
 *
 * @b Description
 *        This function is invoked by config to initialize the Buffer
 *        hash List
 *
 *  @param[in]    buf       Rx/Tx Buffer Control Point
 *  @param[in]    numBins   number of Bins
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDlBufInit
(
PjCb        *gCb,
PjBuf       *buf,            /* !< Rx/Tx Buffer */
U8          numBins          /* !< number of Bins */
)
#else
PUBLIC S16 pjDbmDlBufInit(gCb, buf, numBins)
PjCb        *gCb;
PjBuf       *buf;            /* !< Rx/Tx Buffer */ 
U8          numBins;         /* !< number of Bins */             
#endif
{
   U8       hashIndex;        /* HashIndex of array */

   TRC3(pjDbmDlBufInit)

   RLOG1(L_DEBUG, "pjDbmBufInit(buf, numBins(%d)", numBins);

   /* Initialize CmLListCps*/
   PJ_ALLOC(gCb, buf->datQ, (sizeof(CmLListCp) * numBins));
#if (ERRCLASS & ERRCLS_DEBUG)
   if (buf->datQ == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   for(hashIndex = 0; hashIndex < numBins; hashIndex++)
   {
      cmLListInit(&buf->datQ[hashIndex]);
   }

   /* Initialistations of buf */
   buf->numEntries = 0;
   buf->numBins    = numBins;
   
   cmLListInit(&buf->datCfmQ);
   cmLListInit(&buf->sduSubmitQ);
   cmLListInit(&buf->reEstPktQ);

   RETVALUE(ROK);
} /* pjDbmBufInit */


/**
 *
 * @brief Handler to insert into a TX Buffer
 *
 * @b Description
 *        This function is invoked to insert into
 *        the transmission buffer hash List
 *
 *  @param[in]    buf         Tx Buffer 
 *  @param[in]    txEnt       Transmission Entry
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmInsTxEnt
(
PjCb        *gCb,
PjBuf       *buf,             /* !< Tx Buffer */  
PjTxEnt     *txEnt            /* !< Tx Entry */              
)
#else
PUBLIC S16 pjDbmInsTxEnt(gCb, buf, txEnt)
PjCb        *gCb;
PjBuf       *buf;             /* !< Tx Buffer */              
PjTxEnt     *txEnt;           /* !< Tx Entry */               
#endif
{
   U8       hashKey;          /* Computed HashKey */

   TRC3(pjDbmInsTxEnt)

//   pjTotDlPckCntPerCell++; 
   /* Get the hash index */
   hashKey = (U8)PJ_HASH_FN(buf, txEnt->count); /*KW_FIX*/

   txEnt->lstEnt.node = (PTR)txEnt;
   cmLListAdd2Tail(&buf->datQ[hashKey], &txEnt->lstEnt);
   buf->numEntries ++;
 
   txEnt->sduSubmitEnt.node = (PTR)txEnt;
   cmLListAdd2Tail(&buf->sduSubmitQ, &txEnt->sduSubmitEnt);

   RETVALUE(ROK);
} /* pjDbmTxBufIns */


/**
 *
 * @brief Handler to fetch from a TX Buffer
 *
 * @b Description
 *        This function is invoked to fetch from
 *         the transmission buffer hash List
 *
 *  @param[in]    buf         Tx Buffer 
 *  @param[in]    count       Count of the PjTxEnt to retrieve
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC PjTxEnt* pjDbmGetTxEnt
(
PjCb        *gCb,
PjBuf       *buf,        /* !< Tx Buffer  */                    
U32          count       /* !< Count of the PjTxEnt to retrieve */
)
#else
PUBLIC PjTxEnt* pjDbmGetTxEnt(gCb, buf, count)
PjCb        *gCb;
PjBuf       *buf;        /* !<Tx Buffer  */                     
U32         count;       /* !<Count of the PjTxEnt to retrieve */ 
#endif
{
   U8          hashKey;     /* Computed Hash Key */
   CmLListCp   *datQ;       /* Pointer to the ListCp */
   PjTxEnt     *tmpEnt;     /* Loop Var - Pointer to PjTxEnt */

   TRC3(pjDbmGetTxEnt) 
 
   /* Call the hash function */
   hashKey = (U8)PJ_HASH_FN(buf, count); /*KW_FIX*/

   datQ = &buf->datQ[hashKey];

   cmLListFirst(datQ);
   while(cmLListCrnt(datQ))
   {
      tmpEnt = (PjTxEnt *) cmLListNode(cmLListCrnt(datQ));
      if(tmpEnt->count  == count)
      {
         RETVALUE(tmpEnt);
      }
      cmLListNext(datQ);
   }
   RETVALUE(NULLP);
} /* pjDbmGetTxEnt */


/**
 *
 * @brief Handler to fetch from a TX Buffer
 *
 * @b Description
 *        This function is fetch the node based on the SN.
 *        This function will be called only for the case of a 
 *        PDCP STATUS REPORT being received by the DRB AM. 
 *
 *  @param[in]    buf         Tx Buffer 
 *  @param[in]    sn          SN to get the Ent
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC PjTxEnt* pjDbmGetTxEntSn
(
PjCb        *gCb,
PjBuf       *buf,        /* !< Tx Buffer  */                     
U16          sn          /* !< SN of the PjTxEnt to retrieve */ 
)
#else
PUBLIC PjTxEnt* pjDbmGetTxEntSn(gCb, buf, sn)
PjCb        *gCb;
PjBuf       *buf;        /* !< Tx Buffer  */                      
U16         sn;          /* !< SN of the PjTxEnt to retrieve */     
#endif
{
   U8          hashKey;  /* Computed hash index */
   CmLListCp   *datQ;    /* Pointer to the ListCP */
   PjTxEnt     *tmpEnt;  /* Loop Var - pointer to PjTxEnt */

   TRC3(pjDbmGetTxEntSn) 

   RLOG1(L_DEBUG, "pjDbmGetTxEnt(buf, sn(%d)", sn);

   /* Call the hash function */
   hashKey = (U8)PJ_HASH_FN(buf, sn); /*KW_FIX*/

   datQ = &buf->datQ[hashKey];

   /* Loop through to find the entry that matches */
   cmLListFirst(datQ);
   while(cmLListCrnt(datQ))
   {
      tmpEnt = (PjTxEnt *) cmLListNode(cmLListCrnt(datQ));
      if(tmpEnt->sn == sn)
      {
         RETVALUE(tmpEnt);
      }
      cmLListNext(datQ);
   }
   RETVALUE(NULLP);

} /* pjDbmGetTxEntSn */
  
/* ccpu00136902 */
#if (defined (LTE_PAL_ENB) || defined (TENB_ACC))
/**
 *  @brief This fn is called to chek the state of the PDCP pkts in DL before Ciphering operation
 *         and appropriate action is taken accordingly
 *
 *  @details
 *      Function : pjCiphStateChek
 *
 *  Processing Steps:
 *
 *
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
*/
#ifdef ANSI
PUBLIC S16 pjCiphStateChek
(
PjCb        *gCb,
PjTxEnt     *txEnt
)
#else
PUBLIC S16 pjCiphStateChek(gCb, txEnt)
PjCb        *gCb;
PjTxEnt     *txEnt;
#endif
{
  TRC3(pjCiphStateChek);
  PJ_FREE_BUF(txEnt->sdu);
  PJ_FREE_BUF(txEnt->pdu);
#ifdef FLAT_BUFFER_OPT
   if(txEnt->fb.startAddr)
   {
       PJ_FREE_FLAT_BUF(gCb, (&(txEnt->fb)));
   }
#endif

   /*This txEnt is deleted in checkAndSendPacketsToCrypto() in
    * pj_ptsec_qcom.cpp*/
   if(txEnt->state == PJ_WAIT_FOR_CIPHER)
   {
      txEnt->state = PJ_PDU_TO_DELETE;
      RETVALUE(ROK);
   }

  PJ_FREE(gCb, txEnt, sizeof(PjTxEnt));
  RETVALUE(ROK);
}
#endif


/**
 *
 * @brief Handler to delete from a TX Buffer
 *
 * @b Description
 *        This function is invoked by to insert into
 *         the transaction buffer hash List
 *
 *  @param[in]    buf         Tx Buffer 
 *  @param[in]    count       count to get the Ent
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDelTxEnt
(
PjCb        *gCb,
PjBuf       *buf,     /* !< Tx Buffer  */                     
U32         count     /* !< Count of the PjTxEnt to delete */ 
)
#else
PUBLIC S16 pjDbmDelTxEnt(gCb, buf, count)
PjCb        *gCb;
PjBuf       *buf;     /* !< Tx Buffer  */                      
U32         count;    /* !< Count of the PjTxEnt to delete */    
#endif
{
   U8          hashKey;      /* Computed Hash Key */
   CmLListCp   *datQ;        /* Pointer to the ListCP */
   PjTxEnt     *delEnt;      /* Loop Var - pointer to PjTxEnt */

   TRC3(pjDbmDelTxEnt)

   /* search for the entry */
   delEnt   =  pjDbmGetTxEnt(gCb, buf, count);
   if(delEnt == NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Compute the hash index */
   hashKey = (U8)PJ_HASH_FN(buf, delEnt->count); /*KW_FIX*/
   datQ  =  &buf->datQ[hashKey];

   /* delete the entry */
   cmLListDelFrm(datQ, &delEnt->lstEnt);

   if (delEnt->sduSubmitEnt.node != NULLP)
   {
      delEnt->sduSubmitEnt.node = NULLP;
      cmLListDelFrm(&buf->sduSubmitQ,&delEnt->sduSubmitEnt);
   }
   if(delEnt->datCfmEnt.node != NULLP)
   {
      delEnt->datCfmEnt.node = NULLP;
      cmLListDelFrm(&buf->datCfmQ, &delEnt->datCfmEnt);
      pjTotDlPckCntPerCell--;
   }

   if(delEnt->reEstPktEnt.node != NULLP)
   {
      delEnt->reEstPktEnt.node = NULLP;
      cmLListDelFrm(&buf->reEstPktQ, &delEnt->reEstPktEnt);
   }
   /* NULLing off to prevent double dealloc */
   if(delEnt->pdu == delEnt->sdu)
   {
      delEnt->pdu =  NULLP;
   }
/* ccpu00136902 */
   pjCiphStateChek(gCb, delEnt);
   buf->numEntries --;

   RETVALUE(ROK);
} /* pjDbmDelTxEnt */


/**
 *
 * @brief Handler to Delete all the elements in the hashlist
 *       
 *
 * @b Description
 *        This function is invoked to Delete all the elements in the
 *        hash List
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmTxDelAll
(
PjCb  *gCb,
PjBuf *buf        /* !< Tx Buffer  */                     
)
#else
PUBLIC S16 pjDbmTxDelAll(gCb,buf)
PjCb  *gCb;
PjBuf *buf;       /* !< Tx Buffer  */                     
#endif
{
   U8         i;         /* index counter */
   CmLListCp *datQ;      /* CmLListcp Ptr */
   CmLList   *lstEnt;   /* CmLList Ptr - Loop var */
   PjTxEnt   *txEnt;     /* Tx Entry Ptr - Loop var */

   TRC3(pjDbmTxDelAll);

   RLOG0(L_DEBUG, "pjDbmTxDelAll(buf)");

   /* cleanup the entries */
   for(i = 0; i< buf->numBins; i++)
   {
      datQ = &buf->datQ[i];
      while(cmLListFirst(datQ))
      {
         lstEnt = cmLListDelFrm(datQ, cmLListCrnt(datQ));
         if(lstEnt != NULLP)
         {
            txEnt =  (PjTxEnt *)cmLListNode(lstEnt);
            if(txEnt->datCfmEnt.node != NULLP)
            {
               txEnt->datCfmEnt.node = NULLP;
               cmLListDelFrm(&buf->datCfmQ, &txEnt->datCfmEnt);
               pjTotDlPckCntPerCell--;
            }

            if (txEnt->sduSubmitEnt.node != NULLP)
            {
               txEnt->sduSubmitEnt.node = NULLP;
               cmLListDelFrm(&buf->sduSubmitQ,&txEnt->sduSubmitEnt);
            }

            if(txEnt->reEstPktEnt.node != NULLP)
            {
               txEnt->reEstPktEnt.node = NULLP;
               cmLListDelFrm(&buf->reEstPktQ, &txEnt->reEstPktEnt);
            }

            if ( txEnt->sdu == txEnt->pdu )
            {
               txEnt->sdu = NULLP;
            }
            /* ccpu00136902 */
            pjCiphStateChek(gCb, txEnt);
         }
      }
      cmLListInit(datQ);
   }

   RETVALUE(ROK);
} /* pjDbmTxDelAll */

/**
 *
 * @brief Handler to De initialize hash list
 *       
 *
 * @b Description
 *        This function is invoked by LMM to De initialize the 
 *        hash List
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmTxDeInit
(
PjCb  *gCb,
PjBuf *buf        /* !< Tx Buffer  */                     
)
#else
PUBLIC S16 pjDbmTxDeInit(gCb,buf)
PjCb  *gCb;
PjBuf *buf;       /* !< Tx Buffer  */                     
#endif
{

   TRC3(pjDbmTxDeInit);

   RLOG0(L_DEBUG, "pjDbmTxDeInit(buf)");

   /* cleanup the entries */
   pjDbmTxDelAll(gCb,buf);
   
   /* Cleanup the listCps */
   PJ_FREE(gCb, buf->datQ, (sizeof(CmLListCp) * buf->numBins));
   buf->numBins   =  NULLP;

   RETVALUE(ROK);
} /* pjDbmDeInit */



/**
 *
 * @brief Handler to initialize hash list
 *       
 *
 * @b Description
 *        This function is invoked by LMM to initialize the UeCb and CellCb
 *        hash List
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDlInit  
(
PjCb *gCb
)
#else
PUBLIC S16 pjDbmDlInit(gCb)
PjCb *gCb;
#endif
{
   S16      ret;              /* Return Value */

   TRC3(pjDbmDlInit)

#ifdef DEBUGP
   RLOG0(L_DEBUG, "pjDbmInit()");
#endif

   /* Initialize ueCb Hash List */
   ret = cmHashListInit(&(gCb->u.dlCb->ueLstCp), (U16) PJ_UE_LIST_BUCKET_SIZE,
         (U16) 0, (Bool) FALSE, (U16) CM_HASH_KEYTYPE_DEF,
         gCb->init.region, gCb->init.pool);
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG0(L_DEBUG, "pjDbmInit: cmHashListInit Failed for gCb.ueLstCp.");
#endif
      RETVALUE(ret);
   }

   RETVALUE(ROK);
} /* pjDbmInit */


/**
 *
 * @brief Handler to De initialize hash list
 *       
 *
 * @b Description
 *        This function is invoked by LMM to De initialize the UeCb and CellCb
 *        hash List
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDlDeInit
(
PjCb *gCb
)
#else
PUBLIC S16 pjDbmDlDeInit(gCb)
PjCb *gCb;
#endif
{
   S16      ret;                       /* Return Value */

   TRC3(pjDbmDlDeInit);

#ifdef DEBUGP
   RLOG0(L_DEBUG, "pjDbmDeInit()");
#endif

   /* Initialize ueCb Hash List */
   ret = cmHashListDeinit(&(gCb->u.dlCb->ueLstCp));
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG0(L_ERROR, "pjDbmDeInit: cmHashListDeinit Failed for gCb.ueLstCp.");
#endif
      RETVALUE(ret);
   }
   RETVALUE(ROK);
} /* pjDbmDeInit */


/**
 *
 * @brief Handler to fetch rbCb in the Upper Interface
 *       
 *
 * @b Description
 *        This function is invoked by CFG to fetch rbCb from the cellCb/ueCb in
 *        the upper interface (CPJ/PJU).
 *
 *  @param[in]    rlcId    RLC Identifier 
 *  @param[out]   rbCb     RB Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmFetchDlRbCb
(
PjCb           *gCb,
CmLteRlcId     rlcId,      /* RLC Identifier */
PjDlRbCb       **rbCb       /* RB Cb */
)
#else
PUBLIC S16 pjDbmFetchDlRbCb(gCb, rlcId, rbCb)
PjCb           *gCb;
CmLteRlcId     rlcId;      /* RLC Identifier */
PjDlRbCb         **rbCb;      /* RB Cb */
#endif
{
   PjDlUeCb      *ueCb;                  /* UE Control Block */
   PjDlRbCb        **rbCbLst;            /* RB Control Block List */
   PjDlRbCb        *tRbCb;                 /* Local RB CB */
   S16         ret;                    /* Return Value */

   TRC3(pjDbmFetchDlRbCb)

#ifdef DEBUGP
   RLOG3(L_DEBUG, "pjDbmFetchRbCbForLi(rlcId(ueId(%d), cellId(%d), rbId(%d)),   rbCb)",
             rlcId.ueId, rlcId.cellId, rlcId.rbId);
#endif

   ueCb = NULLP;
   rbCbLst = NULLP;
   ret = ROK;

   /* Validate the RBID in case of TM/UM/AM */
   PJ_VALIDATE_RBID(rlcId.ueId, rlcId.rbId, rlcId.rbType, ret);
   if (ret != ROK)
   {
#ifdef DEBUGP
      /* DEBUG_PRINT EROR */
      RLOG_ARG2(L_ERROR,DBG_UEID,rlcId.ueId, 
            "PJ_VALIDATE_RBID Failed -- rbId(%d), cellId(%d)",
               rlcId.rbId, rlcId.cellId);

#endif
      RETVALUE(ret);
   }
   if (pjDbmFetchDlUeCb(gCb, rlcId.ueId, rlcId.cellId, &ueCb) != ROK)
   {
         RLOG_ARG1(L_ERROR, DBG_CELLID, rlcId.cellId,"UeId[%u] not found",
            rlcId.ueId);
         RETVALUE(RFAILED);

   }

   PJ_DBM_FETCH_DL_RBCB(rlcId.rbId, rlcId.rbType, ueCb, tRbCb);

   if (tRbCb == NULLP)
   {
      RLOG_ARG2(L_ERROR, DBG_UEID,rlcId.ueId, "CellId[%u]:RbId[%d] not found",
            rlcId.cellId, rlcId.rbId);
      RETVALUE(RFAILED);
   }

   *rbCb = tRbCb;

   RETVALUE(ROK);
} /* pjDbmFetchRbCb */


/**
 *
 * @brief Handler to delete RbCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to remove RbCb from Ue/Cell hashlist 
 *
 *
 *  @param[in] rbCbLst   - Rb Cb List
 *  @param[in] numRbCb   - Number of rbCbs
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDelAllDlRb
(
PjCb             *gCb,
PjDlRbCb         **rbCbLst,              /* RB Cb list */
U8             numRbCb                 /* Number of rbCbs */
)
#else
PUBLIC S16 pjDbmDelAllDlRb(gCb,rbCbLst, numRbCb)
PjCb             *gCb;
PjDlRbCb         **rbCbLst;              /* RB Cb list */
U8             numRbCb;                /* Number of rbCbs */
#endif
{
   U8          idx;                    /* Index */
   PjDlRbCb        *rbCb;                  /* RB Control Block */

   TRC3(pjDbmDelAllDlRb)

#ifdef DEBUGP
   RLOG1(L_DEBUG, "pjDbmDelAllRb(rbCbLst, numRbCb(%d))", numRbCb);
#endif

   for (idx = 0; idx < numRbCb; idx++)
   {
      rbCb = rbCbLst[idx];
      if (rbCb != NULLP)
      {
         PJ_DBM_DELETE_DL_RBCB(gCb, rbCb);
         rbCbLst[idx] = NULLP;
      }
   }

   RETVALUE(ROK);
} /* pjDbmDelAllRb */


/**
 *
 * @brief Handler to create an UeCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to create UeCb and insert into the Ue hashlist 
 *        of KwCb.
 *
 *
 *  @param[in] ueId     UE Identifier 
 *  @param[in] cellId   Cell Identifier 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmCreateDlUeCb
(
PjCb              *gCb,
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
PjDlUeCb            **ueCb       /* UE Control Block */
)
#else
PUBLIC S16 pjDbmCreateDlUeCb(gCb, ueId, cellId, ueCb)
PjCb              *gCb; 
CmLteRnti         ueId;       /* UE Identifier */
CmLteCellId       cellId;     /* Cell Identifier */
PjDlUeCb            **ueCb;     /* UE Control Block */
#endif
{
   S16      ret;              /* Return Value */
   PjDlUeCb   *tUeCb;           /* UE Control Block */

   TRC3(PjDbmCreateDlUeCb)

#ifdef DEBUGP
   RLOG2(L_DEBUG, "pjDbmCreateUeCb(ueId(%d), cellId(%d))", ueId, cellId);
#endif

   PJ_ALLOC(gCb, *ueCb, sizeof(PjDlUeCb));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (*ueCb == NULLP)
   {
      ret = RFAILED;
      RLOG0(L_FATAL, "Memory Allocation failed.");
      RETVALUE(ret);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   tUeCb = *ueCb;
   tUeCb->key.ueId = ueId;
   tUeCb->key.cellId = cellId;

   ret = cmHashListInsert(&(gCb->u.dlCb->ueLstCp), (PTR)tUeCb, (U8 *)&(tUeCb->key),
         (U16) sizeof(PjUeKey));
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG_ARG1(L_ERROR,DBG_UEID,ueId, 
            "DL UE CB Hash Insert Failed for cellId (%d)", cellId);
#endif
      RETVALUE(ret);
   }

   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->u.dlCb->pjGenSts.numUe++;
   (*ueCb)->libInfo.state =  PJ_STATE_NORMAL;
   cmInitTimers(&((*ueCb)->libInfo.obdTmr), 1);
   RETVALUE(ret);
} /* kwDbmCreateUeCb */


/**
 *
 * @brief Handler to Fetch an UeCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to fetch UeCb from the Ue hashlist 
 *        of KwCb.
 *
 *
 *  @param[in]    ueId     UE Identifier 
 *  @param[in]    cellId   Cell Identifier 
 *  @param[out]   ueCb     UE Control Block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmFetchDlUeCb
(
PjCb              *gCb,
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
PjDlUeCb            **ueCb       /* UE Control Block */
)
#else
PUBLIC S16 pjDbmFetchDlUeCb(gCb, ueId, cellId, ueCb)
PjCb              *gCb;
CmLteRnti         ueId;       /* UE Identifier */
CmLteCellId       cellId;     /* Cell Identifier */
PjDlUeCb            **ueCb;      /* UE Control Block */
#endif
{
   S16      ret;              /* Return Value */
   PjUeKey  key;              /* Key for UE Hash List */

   TRC3(pjDbmFetchDlUeCb)


   key.ueId = ueId;
   key.cellId = cellId;

   ret = cmHashListFind(&(gCb->u.dlCb->ueLstCp), (U8 *)&(key), sizeof(PjUeKey),
         PJ_DEF_SEQ_NUM, (PTR *) ueCb);
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG_ARG0(L_WARNING,DBG_UEID,ueId, 
            "pjDbmFetchUeCb: cmHashListFind Failed for ueCb.");
#endif
      RETVALUE(ret);
   }

   RETVALUE(ROK);
} /* pjDbmFetchUeCb */


/**
 *
 * @brief Handler to delete an UeCb
 *       
 *
 * @b Description
 *        This function is invoked by CFG to delete UeCb from the Ue hashlist 
 *        of KwCb.
 *
 *
 *  @param[in] ueCb     UE Control Block
 *  @param[in] abrtFlag Abort Flag
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDelDlUeCb
(
PjCb        *gCb,
PjDlUeCb      *ueCb,       /* UE Identifier */
Bool        abortFlag    /* Abort Flag */
)
#else
PUBLIC S16 pjDbmDelDlUeCb(gCb, ueCb, abortFlag)
PjCb        *gCb;
PjDlUeCb      *ueCb;       /* UE Identifier */
Bool        abortFlag;   /* Abort Flag */
#endif
{
   S16         ret;      /* Return Value */
   U8          idx;      /* Index */

   TRC3(pjDbmDelDlUeCb)

#ifdef DEBUGP
   RLOG2(L_DEBUG, "pjDbmDelUeCb(ueId(%d), cellId(%d))",
            ueCb->key.ueId, ueCb->key.cellId);
#endif

/* kw005.201 Check to see if there is atleast one logical channel */

   for ( idx = 0; idx< PJ_MAX_SRB_PER_UE; idx++)
       pjCfgDelPjDlRbCb(gCb, ueCb, ueCb->srbCb[idx]);

   for ( idx = 0; idx< PJ_MAX_DRB_PER_UE; idx++) 
       pjCfgDelPjDlRbCb(gCb, ueCb, ueCb->drbCb[idx]);

   /* Close the Integrity/Ciphering channels */
   if(ueCb->secInfo.secAct)
   {
      pjUtlDlIntClose(gCb,ueCb->secInfo.intCxtId);
      pjUtlDlCipherClose(gCb,ueCb->secInfo.cpCxtId);
      pjUtlDlCipherClose(gCb,ueCb->secInfo.upCxtId);
   }

   /* Delete hoInfo if present */
   if (ueCb->hoInfo != NULLP)
   {
      for (idx = 0; idx < PJ_MAX_DRB_PER_UE; idx++)
      {
         if (ueCb->hoInfo->hoCfmInfo[idx].pres == TRUE)
         { /* should these be dl? */
   
         }
      }

      PJ_FREE(gCb, ueCb->hoInfo->hoCfmInfo, (PJ_MAX_DRB_PER_UE * sizeof(PjDlHoCfmInfo)));
      PJ_FREE(gCb, ueCb->hoInfo, sizeof(PjDlHoInfo));  
   }

   /* Delete ueCb entry from ueLstCp */
   ret = cmHashListDelete(&(gCb->u.dlCb->ueLstCp), (PTR) ueCb); 
   if (ret != ROK)
   {

#ifdef DEBUGP
      RLOG_ARG0(L_ERROR,DBG_UEID,ueCb->key.ueId,"Hash Delete Failed for ueCb");
#endif
   }
   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->pjGenSts.numUe--; 
   /* Deallocate ueCb */
   PJ_FREE(gCb, ueCb, sizeof(PjDlUeCb));

   RETVALUE(ret);
} /* pjDbmDelUeCb */


/**
 *
 * @brief Handler to delete all UeCbs
 *       
 *
 * @b Description
 *        This function is invoked by CFG to delete all UeCbs from the Ue
 *        hashlist of KwCb.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 PjDbmDelAllDlUe
(
PjCb  *gCb
)
#else
PUBLIC S16 PjDbmDelAllDlUe(gCb)
PjCb  *gCb;
#endif
{
   S16         ret;
   PjDlUeCb      *ueCb;            /* UE Control Block */

   TRC3(pjDbmDelAllDlUe)

#ifdef DEBUGP
   RLOG0(L_DEBUG, "pjDbmDelAllUe()");
#endif

   ret = ROK;
   ueCb = NULLP;

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (cmHashListGetNext(&(gCb->u.dlCb->ueLstCp), (PTR) ueCb, (PTR *)&ueCb) == ROK)
   {
      /* Delete ueCb */
      ret = pjDbmDelDlUeCb(gCb,ueCb, TRUE);
      if (ret != ROK)
      {
#ifdef DEBUGP
         RLOG_ARG0(L_ERROR,DBG_UEID,ueCb->key.ueId,"DL Ue Cb Deletion Failed");
#endif
         RETVALUE(ret);
      }
      ueCb = NULLP;
   }

   RETVALUE(ret);
} /* pjDbmDelAllUe */

#ifdef ANSI
PUBLIC S16 pjDbmDlShutdown 
(
PjCb *gCb
)
#else
PUBLIC S16 pjDbmDlShutdown(gCb)
PjCb *gCb;
#endif
{
   S16 ret;

   TRC3(pjDbmDlShutdown)

   ret = pjDbmDlDeInit(gCb);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      RLOG0(L_FATAL, "DL DeInitialization Failed");
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* pjDbmShutdown */

/**
 *
 * @brief Handler to create a RB control block.
 *       
 *
 * @b Description
 *        This function is called to create a RLC control block or PDCP 
 *        control block based on rb flag and update the pointers in RbCb.
 *        If the RbCb is already allocated, the rbId is updated in KwRbCb
 *        or PjDlRbCb based on rb. If the RbCb has not been allocated earlier,
 *        a new RbCb is created and the pointers are updated along with the
 *        rbIds.
 *
 *  @param[in] rbId      RB ID of the entity
 *  @param[in] rbCbLst   List of the RBs in the UeCb
 *  @param[in] rb        The RB to be created. This can be
 *                       PJ_CFG_PDCP when PDCP is created
 *                       or PJ_CFG_RLC when RLC is being
 *                       created. 
 *  
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC PjDlRbCb* pjDbmCreateDlRbCb
(
PjCb    *gCb,
U8      rbId,
U8      rbType, 
PjDlUeCb  *ueCb, 
U8      rb
)
#else
PUBLIC PjDlRbCb* pjDbmCreateDlRbCb(gCb,rbId,rbType,ueCb,rb)
PjCb    *gCb;
U8      rbId; 
U8      rbType;
PjDlUeCb  *ueCb; 
U8      rb;
#endif
{
   PjDlRbCb   *rbCb;
   PjDlRbCb   **rbCbLst;

   TRC2(pjDbmCreateDlRbCb)

   rbCb = NULLP;
   rbCbLst = NULLP;

   PJ_DBM_FETCH_DL_RBCB(rbId, rbType, ueCb, rbCb);

   if(rbCb == NULLP)
   {
      PJ_ALLOC(gCb, rbCb, sizeof(PjDlRbCb));
      if ( rbCb == NULLP )
      {
         RLOG0(L_FATAL, "Memory Allocation failed.");
         RETVALUE(rbCb);
      }
   }

   rbCb->rbId   = rbId;
   rbCb->rbType = rbType;

   rbCbLst = ((rbType == CM_LTE_SRB)? ueCb->srbCb:ueCb->drbCb); 
   rbCbLst[rbId] = rbCb;
   /* kw005.201 ccpu00117318, updating the statistics */   
   PJ_LMM_RB_STS_INC(gCb);                                    
   
   RETVALUE(rbCb); 
   
} /* pjDbmCreateRbCb */

/**
 *
 * @brief Handler to fetch a  PDCP RB control block.
 *       
 *
 * @b Description
 *        This function is used to fetch the PDCP RB control block based on 
 *        the RB id. The pointer to PjDlRbCb is returned.
 *       
 *  @param[in]  ueCb    UE control block of the PDCP RB
 *  @param[in]  rbId    RB ID of the required PDCP entity.
 *  @param[out] pjRbCb  PDCP RB Control Block.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PUBLIC S16 pjDbmFetchPjDlRbCb
(
PjDlUeCb  *ueCb,
U8       rbId,
U8       rbType,
PjDlRbCb  **pjRbCb 
)
#else
PUBLIC S16 pjDbmFetchPjDlRbCb(ueCb,rbId,rbType,pjRbCb)
PjDlUeCb  *ueCb;      
U8       rbId;
U8       rbType;
PjDlRbCb  **pjRbCb;
#endif
{
   PjDlRbCb    *rbCb;

   TRC2(PjDbmFetchPjDlRbCb)

   *pjRbCb = NULLP;

   PJ_DBM_FETCH_DL_RBCB(rbId, rbType, ueCb, rbCb);
   if ( rbCb != NULLP )
   {
      *pjRbCb = rbCb;
   }

   RETVALUE(ROK);

} /* pjDbmFetchPjDlRbCb */

/**
 *
 * @brief Handler to delete a PDCP/RLC RB control block.
 *       
 *
 * @b Description
 *        This function is used to delete the PDCP/RLC RB control block based 
 *        on RB id and the RB type (PDCP/RLC). If the corresponding RB is 
 *        present, the cfgStat flag is unset. If the cfgStat flag is zero,
 *        the RBCB is freed.
 *
 *  @param[in]  rbId    RB ID of the to be deleted RB
 *  @param[in]  rbCbLst RbCb list in UE CB
 *  @param[in]  rb      The RB to be deleted. This can be
 *                      PJ_CFG_PDCP when PDCP is created
 *                      or PJ_CFG_RLC when RLC is being
 *                      created.
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PUBLIC S16 pjDbmDelDlRbCb
(
PjCb   *gCb,
U8      rbId,
U8      rbType,
PjDlRbCb   **rbCb, 
U8      rb,
PjDlUeCb  *ueCb
)
#else
PUBLIC S16 pjDbmDelDlRbCb(gCb,rbId,rbType,rbCb,rb,ueCb)
PjCb   *gCb;
U8      rbId;
U8      rbType;
PjDlRbCb   **rbCb;
U8      rb;
PjDlUeCb  *ueCb;
#endif
{
   S16      ret;
   PjDlRbCb   **rbCbLst;
   rbCbLst = NULLP;
   
   TRC2(pjDbmDelDlRbCb)

   ret   = RFAILED;
   /*updating rbCbList */
   rbCbLst = ((rbType == CM_LTE_SRB)? ueCb->srbCb:ueCb->drbCb);

   if ( (*rbCb) != NULLP )
   {
      PJ_DBM_DELETE_DL_RBCB(gCb,*rbCb);
      ret = ROK;
      /*updating rbCbList */
      rbCbLst[rbId] = NULLP;
   }
   RETVALUE(ret);

} /* pjDbmDelRbCb */




/********************************************************************30**
  
         End of file:     eq_dbm_dl.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:46:44 2014
  
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
*********************************************************************91*/
