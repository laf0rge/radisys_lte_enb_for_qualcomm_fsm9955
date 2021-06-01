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

        Sid:      eq_dbm_ul.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:46:45 2014
 
        Prg:     mn
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=216;


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
#include "pj_ul.h"                 /* RLC layer */
#include "pj_err.h"
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


/* local defines */

/* local externs */

/* forward references */

/* public variable declarations */
PUBLIC S16 pjDeciphStateChek ARGS ((PjCb *gCb, PjRxEnt *delEnt));
/* This structure holds all the global structs we need. */

/* private variable declarations */

/* private function declarations */

/** @file gp_pj_dbm.c
@brief PDCP DBM Module
**/

/*****************************************************************************
 *                  HANDLER FOR THE TRANSMISSION BUFFER
 ****************************************************************************/

/*****************************************************************************
 *                  HANDLERS FOR THE RECEPTION BUFFER
 ****************************************************************************/

/**
 * @brief Handler to insert packets In-Sequence into RX Buffer
 *
 *
 */
#ifdef ANSI
PRIVATE S16 pjDbmInsInSeqRxEnt
(
PjBuf       *buf,          /* !< Rx Buffer  */
PjRxEnt     *rxEnt         /* !< Rx Entry   */              
)
#else
PRIVATE S16 pjDbmInsInSeqRxEnt(buf, rxEnt)
PjBuf       *buf;
PjRxEnt     *rxEnt;
#endif
{

   PjRxEnt     *tmpEnt = NULLP;

   tmpEnt = (PjRxEnt *) cmLListNode(cmLListFirst(&buf->datPktQ));
   if((rxEnt->count) < (tmpEnt->count))
   {         
      rxEnt->datPktEnt.node = (PTR) rxEnt;
      cmLListInsCrnt(&buf->datPktQ, &rxEnt->datPktEnt);
   }
   else
   {
      tmpEnt = (PjRxEnt *) cmLListNode(cmLListLast(&buf->datPktQ));
      while((rxEnt->count) < (tmpEnt->count))
      {
         cmLListPrev(&buf->datPktQ);
         tmpEnt = (PjRxEnt *) cmLListNode(cmLListCrnt(&buf->datPktQ));
      }
      rxEnt->datPktEnt.node = (PTR) rxEnt;
      cmLListInsAfterCrnt(&buf->datPktQ, &rxEnt->datPktEnt);
   }
   RETVALUE(ROK);
}



/**
 *
 * @brief Handler to insert into a RX Buffer
 *
 * @b Description
 *        This function is invoked by to insert into
 *         the reception buffer hash List
 *
 *  @param[in]  buf      Rx Buffer 
 *  @param[in]  rxEnt    Reception Entry
 *  @param[in]  dupFlag  Search for duplicates(Should be TRUE for AM)
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmInsRxEnt
(
PjCb        *gCb,
PjBuf       *buf,          /* !< Rx Buffer  */
PjRxEnt     *rxEnt,        /* !< Rx Entry   */              
Bool        dupFlag        /* !< Boolean to check for duplicate  entries */
)
#else
PUBLIC S16 pjDbmInsRxEnt(gCb, buf, rxEnt, dupFlag)
PjCb        *gCb;
PjBuf       *buf;          /* !< Rx Buffer  */
PjRxEnt     *rxEnt;        /* !< Rx Entry   */                              
Bool        dupFlag;       /* !< Boolean to check for duplicate  entries */ 
#endif
{
   U8       hashKey;          /* Computed hash key */
   PjRxEnt  *tmpEnt = NULLP;  /* Rx Entry Ptr */

   TRC3(pjDbmInsRxEnt)

   if(dupFlag)
   {
      /* Search for duplicates */
      tmpEnt = pjDbmGetRxEnt(gCb, buf, rxEnt->count);
      if(tmpEnt != NULLP)
      {
         RETVALUE(ROKDUP);
      }
   }

   /* Compute the hash index */
   hashKey = (U8)PJ_HASH_FN(buf, rxEnt->count);  /*KW_FIX*/

   rxEnt->lstEnt.node   =  (PTR) rxEnt;
   cmLListAdd2Tail(&buf->datQ[hashKey], &rxEnt->lstEnt);
   buf->numEntries ++;

   if(buf->datPktQ.count == 0)
   {
      rxEnt->datPktEnt.node = (PTR) rxEnt;
      cmLListAdd2Tail(&buf->datPktQ, &rxEnt->datPktEnt);
   }
   else
   {
      pjDbmInsInSeqRxEnt(buf, rxEnt);
   }

   RETVALUE(ROK);
} /* pjDbmRxBufIns */


/**
 *
 * @brief Handler to fetch from a RX Buffer
 *
 * @b Description
 *        This function is invoked to fetch an entry from
 *         the transaction buffer hash List
 *
 *  @param[in]    buf         Rx Buffer 
 *  @param[in]    count       Count value of the entry to retrieve
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC PjRxEnt* pjDbmGetRxEnt
(
PjCb   *gCb,
PjBuf  *buf,       /* !< Rx Buffer  */                               
U32    count       /* !< Count value of the entry to retrieve */                               
)
#else
PUBLIC PjRxEnt* pjDbmGetRxEnt(gCb, buf, count)
PjCb   *gCb;
PjBuf  *buf;       /* !< Rx Buffer  */                               
U32    count;      /* !< Count value of the entry to retrieve */   
#endif
{
   U8         hashKey;   /* Computed hashKey */
   CmLListCp *datQ;      /* Ptr to the CmLListCp */
   PjRxEnt   *tmpEnt = NULLP;  /* Loop var - Rx Entr Ptr */

   TRC3(pjDbmGetRxEnt)
   /* Compute the hask key*/
   hashKey = (U8)PJ_HASH_FN(buf, count); /*KW_FIX*/

   datQ = &buf->datQ[hashKey];

   /* Loop through and search */
   cmLListFirst(datQ);
   while(cmLListCrnt(datQ))
   {
      tmpEnt = (PjRxEnt *)cmLListNode(cmLListCrnt(datQ));
      if((tmpEnt->count) == count)
      {
         RETVALUE(tmpEnt);
      }
      cmLListNext(datQ);
   }
   RETVALUE(NULLP);
} /* pjDbmGetRxEnt */
/**
 *  @brief This fn is called to chek the state of the PDCP pkts in UL before Deciphering operation
 *         and appropriate action is taken accordingly
 *
 *  @details
 *      Function : pjDeciphStateChek
 *
 *  Processing Steps:
 *
 *
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
*/

/* ccpu00136902 */
#if (defined (LTE_PAL_ENB) || defined (TENB_ACC))
#ifdef ANSI
PUBLIC S16 pjDeciphStateChek
(
PjCb        *gCb,
PjRxEnt     *delEnt
)
#else
PUBLIC S16 pjDeciphStateChek(gCb, delEnt)
PjCb        *gCb;
PjRxEnt     *delEnt;
#endif
{
  TRC3(pjDeciphStateChek);
  PJ_FREE_BUF(delEnt->mBuf);
  PJ_FREE(gCb, delEnt, sizeof(PjRxEnt));
  RETVALUE(ROK);
}
#endif


/**
 *
 * @brief Handler to delete from a RX Buffer
 *
 * @b Description
 *        This function is invoked by to delete from
 *         the transaction buffer hash List
 *
 *  @param[in]    buf         Rx Buffer Control Point
 *  @param[in]    count       Count value of the Rx Entry to delete
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDelRxEnt
(
PjCb   *gCb,
PjBuf  *buf,        /* !< Rx Buffer  */                                                          
U32    count        /* !< Count value of the entry to retrieve */                                
)
#else
PUBLIC S16 pjDbmDelRxEnt(gCb, buf, count)
PjCb   *gCb;
PjBuf  *buf;        /* !< Rx Buffer  */                                                          
U32    count;       /* !< Count value of the entry to retrieve */                                
#endif
{
   U8          hashKey;      /* Computed hash value */
   CmLListCp   *datQ;        /* Pointer to the the ListCp */
   PjRxEnt     *delEnt;      /* Loop var - entry to be deleted */

   TRC3(pjDbmDelRxEnt)

   /* Get the node */
   delEnt   =  pjDbmGetRxEnt(gCb, buf, count);

   if(delEnt == NULLP)
   {
      RETVALUE(ROK);
   }

   hashKey = (U8)PJ_HASH_FN(buf, count); /*KW_FIX*/
   datQ = &buf->datQ[hashKey];

   /* delete the entity and free it*/
   cmLListDelFrm(datQ, &delEnt->lstEnt);
   if(delEnt->datPktEnt.node != NULLP)
   {
      cmLListDelFrm(&buf->datPktQ, &delEnt->datPktEnt);
      delEnt->datPktEnt.node = NULLP;
   }
   /* ccpu00136902 */
   pjDeciphStateChek(gCb, delEnt);
   buf->numEntries --;

   RETVALUE(ROK);
} /* pjDbmDelRxEnt */


/**
 *
 * @brief Handler to Delete all the elements in the hashlist
 *       
 *
 * @b Description
 *        This function is invoked by delete all the elements in th3
 *        hash List
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmRxDelAll
(
PjCb  *gCb,
PjBuf *buf        /* !< Rx Buffer  */                                                          
)                 
#else
PUBLIC S16 pjDbmRxDelAll(gCb,buf)
PjCb  *gCb;
PjBuf *buf;       /* !< Rx Buffer  */                                                          
#endif
{
   U8         listInd;   /* loop index */
   CmLListCp *datQ;      /* CmLListCp Ptr */
   CmLList   *lstEnt;   /* loop ptr to CmLlistEnt */
   PjRxEnt   *delEnt;    /* loop ptr to entry to be deleted */

   TRC3(pjDbmRxDelAll);

   RLOG0(L_DEBUG, "pjDbmRxDelAll(buf)");

   for(listInd = 0; listInd< buf->numBins; listInd++)
   {
      datQ = &buf->datQ[listInd];
      while(cmLListFirst(datQ))
      {
         lstEnt = cmLListDelFrm(datQ, cmLListCrnt(datQ));
         if(lstEnt != NULLP)
         {
            delEnt   =  (PjRxEnt *)cmLListNode(lstEnt);
            if(delEnt->datPktEnt.node != NULLP)
            {
               cmLListDelFrm(&buf->datPktQ, &delEnt->datPktEnt);
               delEnt->datPktEnt.node = NULLP;
            }
            /* ccpu00136902 */
            pjDeciphStateChek(gCb, delEnt);
         }
      }
      cmLListInit(datQ);
   }

   RETVALUE(ROK);
} /* pjDbmRxDelAll */

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
PUBLIC S16 pjDbmRxDeInit
(
PjCb  *gCb,
PjBuf *buf        /* !< Rx Buffer  */                                                          
)                 
#else
PUBLIC S16 pjDbmRxDeInit(gCb,buf)
PjCb  *gCb;
PjBuf *buf;       /* !< Rx Buffer  */                                                          
#endif
{

   TRC3(pjDbmRxDeInit);

   RLOG0(L_DEBUG, "pjDbmRxDeInit(buf)");

   /* Cleanup all entries */
   pjDbmRxDelAll(gCb,buf);

   /* Cleanup hashlist */
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
PUBLIC S16 pjDbmUlInit  
(
PjCb *gCb
)
#else
PUBLIC S16 pjDbmUlInit(gCb)
PjCb *gCb;
#endif
{
   S16      ret;              /* Return Value */

   TRC3(pjDbmUlInit)

#ifdef DEBUGP
   RLOG0(L_DEBUG, "pjDbmInit()");
#endif

   /* Initialize ueCb Hash List */
   ret = cmHashListInit(&(gCb->u.ulCb->ueLstCp), (U16) PJ_UE_LIST_BUCKET_SIZE,
         (U16) 0, (Bool) FALSE, (U16) CM_HASH_KEYTYPE_DEF,
         gCb->init.region, gCb->init.pool);
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG0(L_ERROR, "cmHashListInit Failed for gCb.ueLstCp.");
#endif
      RETVALUE(ret);
   }

   ret = cmHashListInit(&(gCb->u.ulCb->transIdLstCp), 
                        (U16) PJ_TRANS_ID_LST_BKT_SIZE,
         (U16) 0, (Bool) FALSE, (U16) CM_HASH_KEYTYPE_DEF,
         PJ_GET_MEM_REGION(gCb), PJ_GET_MEM_POOL(gCb));
   if (ret != ROK)
   {
      cmHashListDeinit(&gCb->u.ulCb->ueLstCp);
#ifdef DEBUGP
      RLOG0(L_ERROR, "cmHashListInit Failed for pjCb.transIdLstCp.");
#endif
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
PUBLIC S16 pjDbmUlDeInit
(
PjCb *gCb
)
#else
PUBLIC S16 pjDbmUlDeInit(gCb)
PjCb *gCb;
#endif
{
   S16      ret;                       /* Return Value */

   TRC3(pjDbmUlDeInit);

#ifdef DEBUGP
   RLOG0(L_DEBUG, "pjDbmDeInit()");
#endif

   /* Initialize ueCb Hash List */
   ret = cmHashListDeinit(&(gCb->u.ulCb->ueLstCp));
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG0(L_ERROR, "cmHashListDeinit Failed for gCb.ueLstCp.");
#endif
      RETVALUE(ret);
   }
   /* Initialize transId Hash List */
   /*MEM_LEAK_FIX*/
   ret = cmHashListDeinit(&(gCb->u.ulCb->transIdLstCp));
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG0(L_ERROR, "cmHashListDeinit Failed for gCb.transIdLstCp.");
#endif
      RETVALUE(ret);
   }

   ret = cmHashListDeinit(&(gCb->u.ulCb->transIdLstCp));
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG0(L_ERROR, "cmHashListDeinit Failed for gCb->u.ulCb->transIdLstCp");
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
PUBLIC S16 pjDbmFetchUlRbCb
(
PjCb           *gCb,
CmLteRlcId     rlcId,      /* RLC Identifier */
PjUlRbCb       **rbCb       /* RB Cb */
)
#else
PUBLIC S16 pjDbmFetchUlRbCb(gCb, rlcId, rbCb)
PjCb           *gCb;
CmLteRlcId     rlcId;      /* RLC Identifier */
PjUlRbCb         **rbCb;      /* RB Cb */
#endif
{
   PjUlUeCb      *ueCb;                  /* UE Control Block */
   PjUlRbCb        **rbCbLst;            /* RB Control Block List */
   PjUlRbCb        *tRbCb;                 /* Local RB CB */
   S16         ret;                    /* Return Value */

   TRC3(pjDbmFetchUlRbCb)

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
            "PJ_VALIDATE_RBID Failed rbId(%d), cellId(%d)",
                       rlcId.rbId, rlcId.cellId);
#endif
      RETVALUE(ret);
   }
   if (pjDbmFetchUlUeCb(gCb, rlcId.ueId, rlcId.cellId, &ueCb) != ROK)
   {
#ifdef DEBUGP
      RLOG_ARG0(L_ERROR,DBG_UEID,rlcId.ueId, "Failed to Fetch UE Cb");
#endif
      RETVALUE(RFAILED);
   }
   else
   {
      PJ_DBM_FETCH_UL_RBCB(rlcId.rbId, rlcId.rbType, ueCb, tRbCb);
   }

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
PUBLIC S16 pjDbmDelAllUlRb
(
PjCb             *gCb,
PjUlRbCb         **rbCbLst,              /* RB Cb list */
U8             numRbCb                 /* Number of rbCbs */
)
#else
PUBLIC S16 pjDbmDelAllUlRb(gCb,rbCbLst, numRbCb)
PjCb             *gCb;
PjUlRbCb         **rbCbLst;              /* RB Cb list */
U8             numRbCb;                /* Number of rbCbs */
#endif
{
   U8          idx;                    /* Index */
   PjUlRbCb        *rbCb;                  /* RB Control Block */

   TRC3(pjDbmDelAllUlRb)

#ifdef DEBUGP
   RLOG1(L_DEBUG, "pjDbmDelAllRb(rbCbLst, numRbCb(%d))", numRbCb);
#endif

   for (idx = 0; idx < numRbCb; idx++)
   {
      rbCb = rbCbLst[idx];
      if (rbCb != NULLP)
      {
         PJ_DBM_DELETE_UL_RBCB(gCb,rbCb);
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
 *        of PjCb.
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
PUBLIC S16 pjDbmCreateUlUeCb
(
PjCb              *gCb,
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
PjUlUeCb            **ueCb       /* UE Control Block */
)
#else
PUBLIC S16 pjDbmCreateUlUeCb(gCb, ueId, cellId, ueCb)
PjCb              *gCb; 
CmLteRnti         ueId;       /* UE Identifier */
CmLteCellId       cellId;     /* Cell Identifier */
PjUlUeCb            **ueCb;     /* UE Control Block */
#endif
{
   S16      ret;              /* Return Value */
   PjUlUeCb   *tUeCb;           /* UE Control Block */

   TRC3(pjDbmCreateUlUeCb)

#ifdef DEBUGP
   RLOG2(L_DEBUG, "pjDbmCreateUeCb(ueId(%d), cellId(%d))", ueId, cellId);
#endif

   PJ_ALLOC(gCb, *ueCb, sizeof(PjUlUeCb));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (*ueCb == NULLP)
   {
      ret = RFAILED;
      RLOG0(L_FATAL, "Memory Allocation failure");
      RETVALUE(ret);
   }
#endif /* ERRCLASS & ERRCLS_ADD_RES */

   tUeCb = *ueCb;
   tUeCb->key.ueId = ueId;
   tUeCb->key.cellId = cellId;

   ret = cmHashListInsert(&(gCb->u.ulCb->ueLstCp), (PTR)tUeCb, (U8 *)&(tUeCb->key),
         (U16) sizeof(PjUeKey));
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG_ARG1(L_ERROR,DBG_UEID,ueId,"Ul UeCb Hash Insert Failed : cellId(%d)",
                            cellId);
#endif
      RETVALUE(ret);
   }

   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->pjGenSts.numUe++;

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
 *        of PjCb.
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
PUBLIC S16 pjDbmFetchUlUeCb
(
PjCb              *gCb,
CmLteRnti         ueId,       /* UE Identifier */
CmLteCellId       cellId,     /* Cell Identifier */
PjUlUeCb            **ueCb       /* UE Control Block */
)
#else
PUBLIC S16 pjDbmFetchUlUeCb(gCb, ueId, cellId, ueCb)
PjCb              *gCb;
CmLteRnti         ueId;       /* UE Identifier */
CmLteCellId       cellId;     /* Cell Identifier */
PjUlUeCb            **ueCb;      /* UE Control Block */
#endif
{
   S16      ret;              /* Return Value */
   PjUeKey  key;              /* Key for UE Hash List */

   TRC3(pjDbmFetchUlUeCb)


   key.ueId = ueId;
   key.cellId = cellId;

   ret = cmHashListFind(&(gCb->u.ulCb->ueLstCp), (U8 *)&(key), sizeof(PjUeKey),
         PJ_DEF_SEQ_NUM, (PTR *) ueCb);
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG_ARG0(L_INFO,DBG_UEID,ueId, "Hash Find Failed for fetching ueCb");
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
 *        of PjCb.
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
PUBLIC S16 pjDbmDelUlUeCb
(
PjCb        *gCb,
PjUlUeCb      *ueCb,       /* UE Identifier */
Bool        abortFlag    /* Abort Flag */
)
#else
PUBLIC S16 pjDbmDelUlUeCb(gCb, ueCb, abortFlag)
PjCb        *gCb;
PjUlUeCb      *ueCb;       /* UE Identifier */
Bool        abortFlag;   /* Abort Flag */
#endif
{
   S16         ret;      /* Return Value */
   U8          idx;      /* Index */

   TRC3(pjDbmDelUlUeCb)

#ifdef DEBUGP
   RLOG2(L_DEBUG, "pjDbmDelUeCb(ueId(%d), cellId(%d))",
            ueCb->key.ueId, ueCb->key.cellId);
#endif

/* kw005.201 Check to see if there is atleast one logical channel */

   for ( idx = 0; idx< PJ_MAX_SRB_PER_UE; idx++)
       pjCfgDelPjUlRbCb(gCb, ueCb, ueCb->srbCb[idx]);

   for ( idx = 0; idx< PJ_MAX_DRB_PER_UE; idx++) 
       pjCfgDelPjUlRbCb(gCb, ueCb, ueCb->drbCb[idx]);

   /* Close the Integrity/Ciphering channels */
   if(ueCb->secInfo.secAct)
   {
      pjUtlUlIntClose(gCb,ueCb->secInfo.intCxtId); 
      pjUtlUlCipherClose(gCb,ueCb->secInfo.cpCxtId);
      pjUtlUlCipherClose(gCb,ueCb->secInfo.upCxtId);
   }

   /* Delete hoInfo if present */
   if (ueCb->hoInfo != NULLP)
   {
      for (idx = 0; idx < PJ_MAX_DRB_PER_UE; idx++)
      {
         if (ueCb->hoInfo->hoCfmInfo[idx].pres == TRUE)
         {
            PJ_FREE(gCb, ueCb->hoInfo->hoCfmInfo[idx].ulBitMap,
                  (ueCb->hoInfo->hoCfmInfo[idx].numBits % 8)?
                  ((ueCb->hoInfo->hoCfmInfo[idx].numBits / 8) + 1):
                  (ueCb->hoInfo->hoCfmInfo[idx].numBits / 8 ));
         }
      }

      PJ_FREE(gCb, ueCb->hoInfo->hoCfmInfo, 
                 (PJ_MAX_DRB_PER_UE * sizeof(PjUlHoCfmInfo)));
      PJ_FREE(gCb, ueCb->hoInfo, sizeof(PjUlHoInfo));  
   }

   /* Delete ueCb entry from ueLstCp */
   ret = cmHashListDelete(&(gCb->u.ulCb->ueLstCp), (PTR) ueCb);
   if (ret != ROK)
   {

#ifdef DEBUGP
      RLOG_ARG0(L_ERROR,DBG_UEID,ueCb->key.ueId, 
            "Hash Delete Failed for deleting ueCb.");
#endif
   }
   /* kw005.201 ccpu00117318, updating the statistics */
   gCb->pjGenSts.numUe--;

   /* Deallocate ueCb */
   PJ_FREE(gCb, ueCb, sizeof(PjUlUeCb));

   RETVALUE(ret);
} /* pjDbmDelUeCb */


/**
 *
 * @brief Handler to delete all UeCbs
 *       
 *
 * @b Description
 *        This function is invoked by CFG to delete all UeCbs from the Ue
 *        hashlist of PjCb.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDelAllUlUe
(
PjCb  *gCb
)
#else
PUBLIC S16 pjDbmDelAllUlUe(gCb)
PjCb  *gCb;
#endif
{
   S16         ret;
   PjUlUeCb      *ueCb;            /* UE Control Block */

   TRC3(pjDbmDelAllUlUe)

#ifdef DEBUGP
   RLOG0(L_DEBUG, "pjDbmDelAllUe()");
#endif

   ret = ROK;
   ueCb = NULLP;

   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (cmHashListGetNext(&(gCb->u.ulCb->ueLstCp), (PTR) ueCb, (PTR *)&ueCb) == ROK)
   {
      /* Delete ueCb */
      ret = pjDbmDelUlUeCb(gCb,ueCb, TRUE);
      if (ret != ROK)
      {
#ifdef DEBUGP
         RLOG_ARG0(L_ERROR,DBG_UEID,ueCb->key.ueId, "UE deletion Failed");
#endif
         RETVALUE(ret);
      }
      ueCb = NULLP;
   }

   RETVALUE(ret);
} /* pjDbmDelAllUe */

/**
 *
 * @brief Handler to Shutdown all UeCbs
 *       
 *
 * @b Description
 *        This function is invoked by CFG to shutdown UeCbs from the PjCb.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmUlShutdown 
(
PjCb *gCb
)
#else
PUBLIC S16 pjDbmUlShutdown(gCb)
PjCb *gCb;
#endif
{
   S16 ret;

   TRC3(pjDbmUlShutdown)

   ret = pjDbmUlDeInit(gCb);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      RLOG0(L_FATAL, "UL DeInitialization Failed");
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
 *        If the RbCb is already allocated, the rbId is updated in PjRbCb
 *        or PjUlRbCb based on rb. If the RbCb has not been allocated earlier,
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
PUBLIC PjUlRbCb* pjDbmCreateUlRbCb
(
PjCb    *gCb,
U8      rbId,
U8      rbType, 
PjUlUeCb  *ueCb, 
U8      rb
)
#else
PUBLIC PjUlRbCb* pjDbmCreateUlRbCb(gCb,rbId,rbType,ueCb,rb)
PjCb    *gCb;
U8      rbId; 
U8      rbType;
PjUlUeCb  *ueCb; 
U8      rb;
#endif
{
   PjUlRbCb   *rbCb;
   PjUlRbCb   **rbCbLst;

   TRC2(pjDbmCreateUlRbCb)

   rbCb = NULLP;
   rbCbLst = NULLP;
   PJ_ALLOC(gCb, rbCb, sizeof(PjUlRbCb) );
   if ( rbCb == NULLP )
   {
      RLOG0(L_FATAL, "Memory Allocation failed");
      RETVALUE(rbCb);
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
 *        the RB id. The pointer to PjUlRbCb is returned.
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
PUBLIC S16 pjDbmFetchPjUlRbCb
(
PjUlUeCb  *ueCb,
U8       rbId,
U8       rbType,
PjUlRbCb  **pjRbCb 
)
#else
PUBLIC S16 pjDbmFetchPjUlRbCb(ueCb,rbId,rbType,pjRbCb)
PjUlUeCb  *ueCb;      
U8       rbId;
U8       rbType;
PjUlRbCb  **pjRbCb;
#endif
{
   PjUlRbCb    *rbCb;

   TRC2(pjDbmFetchPjUlRbCb)

   *pjRbCb = NULLP;

   PJ_DBM_FETCH_UL_RBCB(rbId, rbType, ueCb, rbCb);
   if ( rbCb != NULLP )
   {
      *pjRbCb = rbCb;
   }

   RETVALUE(ROK);

} /* pjDbmFetchPjUlRbCb */

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
PUBLIC S16 pjDbmDelUlRbCb
(
PjCb   *gCb,
U8      rbId,
U8      rbType,
PjUlRbCb   **rbCb, 
U8      rb,
PjUlUeCb  *ueCb
)
#else
PUBLIC S16 pjDbmDelUlRbCb(gCb,rbId,rbType,rbCb,rb)
PjCb   *gCb;
U8      rbId;
U8      rbType;
PjUlRbCb   **rbCb;
U8      rb;
PjUlUeCb  *ueCb;
#endif
{
   S16      ret;
   PjUlRbCb   **rbCbLst;
   rbCbLst = NULLP;     
 
   TRC2(pjDbmDelUlRbCb)

   ret   = RFAILED;
   /*updating rbCbList */                              
   rbCbLst = ((rbType == CM_LTE_SRB)? ueCb->srbCb:ueCb->drbCb); 
   if ( (*rbCb) != NULLP )
   {
      PJ_DBM_DELETE_UL_RBCB(gCb,(*rbCb));
      ret = ROK;
      /*updating rbCbList */
      rbCbLst[rbId] = NULLP;         
   }
   RETVALUE(ret);

} /* pjDbmDelRbCb */

/**
 *
 * @brief Handler to add UL Transaction Id to List 
 *       
 *
 * @b Description
 *        This function is invoked by CFG to add all itransaction IDs to
 *        hashlist of PjCb.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmAddUlTransIdLst
(
PjCb              *gCb,
PjCfgInfo         *cfg       /* UE Control Block */
)
#else
PUBLIC S16 pjDbmAddUlTransIdLst(gCb,cfg)
PjCb              *gCb;
PjCfgInfo         *cfg;     /* UE Control Block */
#endif
{
   S16      ret;              /* Return Value */

   TRC3(pjDbmAddUlTransIdLst)

#ifdef DEBUGP
#ifndef ALIGN_64BIT
   RLOG1(L_UNUSED, "pjDbmAddUlTransIdLst(transId(%ld))", cfg->transId);
#else
   RLOG1(L_UNUSED, "pjDbmAddUlTransIdLst(transId(%d))", cfg->transId);
#endif
#endif

   ret = cmHashListInsert(&(gCb->u.ulCb->transIdLstCp), (PTR)cfg, 
         (U8 *)&(cfg->transId), (U16) sizeof(cfg->transId));
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG0(L_ERROR, "Hash Insert Failed for Inserting UL transId");
#endif
      RETVALUE(ret);
   }

   RETVALUE(ret);
} /* pjDbmAddUlTransIdLst */

/**
 *
 * @brief Handler to Find UL Transaction Id in the List 
 *       
 *
 * @b Description
 *        This function is invoked by CFG to Find transaction IDs in the
 *        hashlist of PjCb.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmFindUlTransIdLst
(
PjCb              *gCb,
U32               transId,
PjCfgInfo         **cfg       /* UE Control Block */
)
#else
PUBLIC S16 pjDbmFindUlTransIdLst(gCb,cfg)
PjCb              *gCb;
U32                transId;
PjCfgInfo         *cfg;     /* UE Control Block */
#endif
{
   S16      ret;              /* Return Value */

   TRC3(pjDbmFindUlTransIdLst)

#ifdef DEBUGP
#ifndef ALIGN_64BIT
   RLOG1(L_DEBUG, "pjDbmFindUlTransIdLst(transId(%ld))", transId);
#else
   RLOG1(L_DEBUG, "pjDbmFindUlTransIdLst(transId(%d))", transId);
#endif
#endif

   ret = cmHashListFind(&(gCb->u.ulCb->transIdLstCp),(U8 *) &transId, 
                  sizeof (transId), PJ_DEF_SEQ_NUM,(PTR *) cfg); 
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG1(L_ERROR,"Hash Find Failed Fetch for transId [%d]", transId);
#endif
   }
   ret = pjDbmDelUlTransIdLst(gCb, cfg);
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG0(L_ERROR,"Deleting Trans ID Failed");
#endif
      RETVALUE(ret);
   }

   RETVALUE(ret);
} /* pjDbmAddUlTransIdLst */

/**
 *
 * @brief Handler to Delete UL Transaction Id to List 
 *       
 *
 * @b Description
 *        This function is invoked by CFG to Delete transaction IDs From the
 *        hashlist of PjCb.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDelUlTransIdLst
(
PjCb              *gCb,
PjCfgInfo         **cfg       /* UE Control Block */
)
#else
PUBLIC S16 pjDbmDelUlTransIdLst(gCb,cfg)
PjCb              *gCb;
PjCfgInfo         **cfg;     /* UE Control Block */
#endif
{
   S16      ret;              /* Return Value */

   TRC3(pjDbmDelUlTransIdLst)

#ifdef DEBUGP
   RLOG0(L_DEBUG, "pjDbmFindUlTransIdLst(transId()" );
#endif

   ret = cmHashListDelete(&(gCb->u.ulCb->transIdLstCp),(PTR) (*cfg)); 
   if (ret != ROK)
   {
#ifdef DEBUGP
      RLOG1(L_ERROR," Hash Delete Failed for transId(%d)",(*cfg)->transId);
#endif
   }

   RETVALUE(ret);
} /* pjDbmAddUlTransIdLst */

/**
 *
 * @brief Handler to Delete All  UL Transaction Ids from the List 
 *       
 *
 * @b Description
 *        This function is invoked by CFG to Delete all itransaction IDs from the
 *        hashlist of PjCb.
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
*/
#ifdef ANSI
PUBLIC S16 pjDbmDelAllUlTransIdLst
(
PjCb              *gCb
)
#else
PUBLIC S16 pjDbmDelAllUlTransIdLst(gCb)
PjCb              *gCb;
#endif
{
   S16      ret;              /* Return Value */
   PjCfgInfo *cfg;

   TRC3(pjDbmDelAllUlTransIdLst)

#ifdef DEBUGP
   RLOG0(L_DEBUG, "pjDbmFindUlTransIdLst");
#endif
   ret = ROK;
   cfg = NULLP; /*KW_FIX*/
   /* Until no more ueCb is ueLstCp hash list get and delete ueCb */
   while (cmHashListGetNext(&(gCb->u.ulCb->transIdLstCp), 
               (PTR) cfg, (PTR *)&cfg) == ROK)

   {
      /* Delete ueCb */
      ret = pjDbmDelUlTransIdLst(gCb,&cfg);
      if (ret != ROK)
      {
#ifdef DEBUGP
         RLOG0(L_ERROR, " pjDbmDelTransIdLst Failed.");
#endif
         RETVALUE(ret);
      }
      
      cfg = NULLP;
   }

   RETVALUE(ret);
} /* pjDbmAddUlTransIdLst */






/********************************************************************30**
  
         End of file:     eq_dbm_ul.c@@/main/TeNB_Main_BR/tenb_brcm_4.0_tdd_dlfs_IT/2 - Wed Dec 10 12:46:45 2014
  
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
