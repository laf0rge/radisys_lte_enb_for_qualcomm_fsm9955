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

     Name:     LTE-X2AP Layer 
  
     Type:     C Source file
  
     Desc:     This file handles the node management procedure.

     File:     cz_nmm.c

     Sid:      tr_nmm.c@@/main/2 - Tue Aug 30 18:36:13 2011

     Prg:       

**********************************************************************/
/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_mblk.h"       /* common memory */
#include "cm5.h"           /* common structs 3 */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* Common Token defines */
#include "cm_tpt.h"        /* common transport */
#include "cm_pasn.h"       /* Common Asn.1 library           */
#include "sct.h"           /* SCTP Layer */
#include "lcz.h"           /* LCZ Interface defines */
#include "czt.h"           /* CZT Interface defines */
#include "cz.h"            /* X2AP defines */

/* Header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_mblk.x"       /* common memory */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
#include "cm_pasn.x"       /* Common ASn.1 Library           */
#include "sct.x"           /* SCTP Layer */
#include "cm5.x"           /* Common timer definitions */
#include "lcz.x"           /* LCZ Interface includes */
#include "czt.x"           /* CZT Interface includes */
#include "cz.x"            /* X2AP includes */

/*Private Functions */

/* Declaration of Global CzCb */
#ifdef SS_MULTIPLE_PROCS
   PUBLIC CzCb czCbLst[CZ_MAX_INSTANCES];
   PUBLIC CzCb *czCbPtr;
#else /* SS_MULTIPLE_PROCS */
   PUBLIC CzCb czCb;
#endif /* SS_MULTIPLE_PROCS */

/*This function compares the IP Addresses */
PRIVATE Bool  czNmmCmpIpAddr ( CmNetAddr    *peerAddr, CmNetAddr    *dstAddr );


PRIVATE S16 czNmmHdlProcErrInd ARGS((CzPeerCb  *peerCb,
                                     CztEvnt   *evnt,
                                     U8       msgDirectn));

/** @defgroup nmmmodule X2AP Node Managment Module
 *  @{
 */

/**
     @brief The function finds peer based on peer Id

     @details This function searches the internal hash for finding the PeerCb. 
      
     @param[in]   peerId      The peerId for the PeerCb. 
     @param[out]   peer       The pointer to peer Control Block 
      
     @return 
         - ROK             - If the Procedure was successful.
         - RFAILED         - If the Peer Could not be found. 
*/


#ifdef ANSI
PUBLIC S16 czNmmFndPeer
(
U32      peerId,
CzPeerCb **peer
)
#else
PUBLIC S16 czNmmFndPeer (peerId, peer)
U32     peerId;
CzPeerCb **peer;
#endif
{
   CzPeerCb *peerCb;
   S16 found = RFAILED;

   CZ_TRC2(czNmmFndPeer);
#ifdef ALIGN_64BIT
   CZ_DBG_INFO((CZ_PRNT_BUF, "Finding peer with peerId (%d)", peerId));
#else
   CZ_DBG_INFO((CZ_PRNT_BUF, "Finding peer with peerId (%ld)", peerId));
#endif


#if (ERRCLASS & ERRCLS_DEBUG)
   /* Check the Pointer */
   if(peer == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmFndPeer: Null peer Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
#endif
   *peer = NULLP;

   found = cmHashListFind(&czCb.nodeCb.peerCbLst, (U8 *)&peerId, 
         sizeof (U32), 0, (PTR *)peer);
   if (found == ROK) 
   {
      peerCb = *peer;
      CZ_DBG_INFO((CZ_PRNT_BUF, "Peer Cb Found.\n"));
      CZ_PRNT_PEERID;
      CZ_RETVALUE(ROK);
   }

   CZ_RETVALUE(RFAILED);
} /* czNmmFndPeer */              


/**
     @brief   The function initializes the Peer List 
     @details The function initializes the Peer List and Dynamic Peer List 

     @return 
         - ROK     - If the Procedure was successful.
         - RFAILED - Suitable error code otherwise.
*/


#ifdef ANSI
PUBLIC S16 czNmmInitPeerLst
(
)
#else
PUBLIC S16 czNmmInitPeerLst(Void)
#endif
{
   S16 ret = RFAILED;
 
   CZ_TRC2(czNmmInitPeerLst);
   CZ_DBG_INFO((CZ_PRNT_BUF, "Initializing Peerid list.\n"));

   ret = cmHashListInit(&czCb.nodeCb.peerCbLst, /* hash list Cp */
            CZ_HASH_LIST_SIZE,                  /* HL bins */
            0,                                  /* Offset of HL Entry */
            FALSE,                              /* Allow dup. keys ? */
            CZ_HASH_KEYTYPE,                    /* HL key type */
            czCb.init.region,                   /* Mem region for HL */
            czCb.init.pool);                    /* Mem pool for HL */

   if (ret == ROK)
   {
      ret = cmHashListInit(&czCb.nodeCb.dynamPeerAssocLst,   /* hash list Cp */
            CZ_HASH_LIST_SIZE,                     /* HL bins */
            0,                                     /* Offset of HL Entry */
            FALSE,                                 /* Allow dup. keys ? */
            CZ_HASH_KEYTYPE,                       /* HL key type */
            czCb.init.region,                      /* Mem region for HL */
            czCb.init.pool);                       /* Mem pool for HL */

      if(ret != ROK)
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF, "dynamPeerAssocLst Hash List failed\
                  to initialize. "));
         cmHashListDeinit(&czCb.nodeCb.peerCbLst);
      }
   }
   else
   {
         CZ_DBG_FATAL((CZ_PRNT_BUF, "peerCbLst Hash List failed\
                  to initialize. "));
   }
   

   CZ_RETVALUE(ret);
}

/**
     @brief The function allocates the Memory for the Peer.

     @details Additionaly it initializes the Control Blocks, hash List and Timers. 
      
     @param[in]  peerId    The peerId for the PeerCb. 
     @param[out] peer    The allocated peerCb Pointer to be returned
      
     @return 
         - ROK           - If the function was successful.
         - RFAILED       - If the function was unsuccessful due to any failure.
         - ROUTRES       - If the procedure was out of resource.
     
*/



#ifdef ANSI
PUBLIC S16 czNmmAllocPeerCb
(
U32 peerId,
CzPeerCb **peer
)
#else
PUBLIC S16 czNmmAllocPeerCb(peerId, peer)
U32 peerId;
CzPeerCb **peer;
#endif
{
   CzPeerCb *peerCb = NULLP;
   S16 ret = RFAILED;

   CZ_TRC2(czNmmAllocPeerCb);

   CZ_DBG_INFO((CZ_PRNT_BUF, "Allocating PeerCb.\n"));
   /* cz001.301 : Removed redundent code.
    * No need of NULL check for peer, we know that we are assigning
    * NULL to peer before passing to czNmmAllocPeerCb */ 

     *peer = NULLP;

   /* Allocate peer control block */
   CZ_ALLOC(peerCb, sizeof(CzPeerCb));

   /* Check the Memory */
   if(peerCb == NULLP)
   {
      /* cz001.101 : Debug Prints added */
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");

      CZ_DBG_FATAL((CZ_PRNT_BUF,
           "czNmmAllocPeerCb: Couldn't allocate memory for peerCB\n"));
      CZ_RETVALUE(ROUTRES);
   }

   /* Allocate Assoc control block in peer cb */
   CZ_ALLOC(peerCb->assocCb,sizeof(CzAssocCb));
   if(peerCb->assocCb == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF,
           "czNmmAllocPeerCb: Couldn't allocate memory for assocCB\n"));
      CZ_FREE(peerCb,sizeof(CzPeerCb));
      CZ_RETVALUE(RFAILED);
   }

   /* Allocate assoc config in assoc control block */
   CZ_ALLOC(peerCb->assocCb->assocCfg,sizeof(CzAssocCfg));

   if(peerCb->assocCb->assocCfg == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF,
           "czNmmAllocPeerCb: Couldn't allocate memory for assocCfg\n"));
      CZ_FREE(peerCb->assocCb,sizeof(CzAssocCb));
      CZ_FREE(peerCb,sizeof(CzPeerCb));
      CZ_RETVALUE(RFAILED);
   }
   /* Intialize the Hash Lists */
   ret = cmHashListInit(&peerCb->ueCbHlCp,          /* hash list Cp */
            CZ_HASH_LIST_SIZE,                  /* HL bins */
            0,                                  /* Offset of HL Entry */
            FALSE,                              /* Allow dup. keys ? */
            CZ_HASH_KEYTYPE,                    /* HL key type */
            czCb.init.region,                   /* Mem region for HL */
            czCb.init.pool);                    /* Mem pool for HL */

   if(ret == ROK)
   {
      ret = cmHashListInit(&peerCb->resrcCbHlCp,    /* hash list Cp */
            CZ_HASH_LIST_SIZE,                  /* HL bins */
            0,                                  /* Offset of HL Entry */
            FALSE,                              /* Allow dup. keys ? */
            CZ_HASH_KEYTYPE,                    /* HL key type */
            czCb.init.region,                   /* Mem region for HL */
            czCb.init.pool);                    /* Mem pool for HL */

      if (ret == ROK)
      {
         peerCb->peerId = peerId;
         CZ_PRNT_PEERID;

         peerCb->assocCb->peer = peerCb;

         /* Set the State of the Peer to the Idle */
         peerCb->state = CZ_GP_IDLE;
         peerCb->gpmCb.wait=FALSE;
         peerCb->gpmCb.procPending=0;
         peerCb->gpmCb.retxBuf=NULLP;
         peerCb->assocCb->assocState = CZ_ASSOC_DOWN;

         ret = cmHashListInsert (&czCb.nodeCb.peerCbLst, (PTR)peerCb,
               (U8 *)&peerCb->peerId, sizeof (U32));
         if(ret != ROK)
         {
            cmHashListDeinit(&peerCb->ueCbHlCp);
            cmHashListDeinit(&peerCb->resrcCbHlCp);
         }

      }
      else
      {
         CZ_DBG_FATAL((CZ_PRNT_BUF, "resrcCbHlCp Hash List failed\
                  to initialize. "));
         cmHashListDeinit(&peerCb->ueCbHlCp);
      }
   }
   else
   {
         CZ_DBG_FATAL((CZ_PRNT_BUF, "ueCbHlCp Hash List failed\
                  to initialize. "));
   }

   if (ret != ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmAllocPeerCb: Failure in the Hash List Operation\n"));

      /* Clean the Allocated Memory */
      CZ_FREE(peerCb->assocCb->assocCfg,sizeof(CzAssocCfg));
      CZ_FREE(peerCb->assocCb,sizeof(CzAssocCb));
      CZ_FREE(peerCb,sizeof(CzPeerCb));
      CZ_RETVALUE(RFAILED);
   }

   CZ_DBG_INFO((CZ_PRNT_BUF, "Initializing Timers.\n"));
   /* Initialize the Timers */
   cmInitTimers(&peerCb->gpmCb.gpTimer, 1);
   *peer = peerCb;

   CZ_RETVALUE(ROK);   
}

/**
     @brief    The function czNmmDeallocPeerCb deallocates the Memory for the Peer.

     @details  The function czNmmDeallocPeerCb deallocates the Memory for the Peer.
               It also deallocates the resource Control Block and Ue Control Blocks.   
      
     @param[in] peerCb  The peerCb to be deallocated. 
      
     @return 
         - ROK        - If the Procedure was successful.
         - RFAILED    - If the procedure was unsuccessful due to any failure.
     
*/


#ifdef ANSI
PUBLIC S16 czNmmDeallocPeerCb
(
CzPeerCb *peerCb
)
#else
PUBLIC S16 czNmmDeallocPeerCb(peerCb)
CzPeerCb *peerCb;
#endif
{
   S16 found = RFAILED;
   S16 ret = RFAILED;
   CzPeerCb *locPeerCb = NULL;

   CZ_TRC2(czNmmDeallocPeerCb);

#if (ERRCLASS & ERRCLS_DEBUG)
   /* Check the Pointer */
   if(peerCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmDeallocPeerCb: Null Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
#endif
   CZ_PRNT_PEERID;
   CZ_DBG_INFO((CZ_PRNT_BUF, "Deallocating PeerCb.\n"));
   found = cmHashListFind(&czCb.nodeCb.peerCbLst, (U8 *)&peerCb->peerId, 
                    sizeof (U32), 0, (PTR *)&locPeerCb);

   if(found != ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmDeallocPeerCb: peer not found in Hash list\n"));
      CZ_RETVALUE(RFAILED);
   }

   /* Function to remove all Ue related Control Blocks*/
   ret = cmHashListDelete (&czCb.nodeCb.peerCbLst,(PTR)locPeerCb);
   if(ret != ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
          "czNmmDeallocPeerCb:Peer Could not be deleted from czCb HashList\n"));
      CZ_RETVALUE(RFAILED);
   }
   /* Will not fail as the only fail return point is NULL Check which we already
    * done*/
   czNmmResetPeer(peerCb);

   cmHashListDeinit(&peerCb->ueCbHlCp);
   cmHashListDeinit(&peerCb->resrcCbHlCp);

   CZ_FREE(peerCb->assocCb->assocCfg,sizeof(CzAssocCfg));
   CZ_FREE(peerCb->assocCb,sizeof(CzAssocCb));
   CZ_FREE(peerCb,sizeof(CzPeerCb));

   CZ_RETVALUE(ROK);
}

/**
       @brief   The function czNmmDisablePeer disables the Peer.

       @details  This function removes transient peer and assoc control block from
                 the hash list. Also it sends the TermReq to Transport Layer terminating
                 association if assocTerm is passed as TRUE.

       
       @param[in] peerCb The peerCb to be disabled. 
       @param[in] assocTerm Association to be terminated or not. 

       @return   
         - ROK        - If the Procedure was successful.
         - RFAILED    - If the procedure was unsuccessful due to any failure.

*/
#ifdef ANSI
PUBLIC S16 czNmmDisablePeer
(
CzPeerCb *peerCb,
Bool  assocTerm
)
#else
PUBLIC S16 czNmmDisablePeer(peerCb, assocTerm)
CzPeerCb *peerCb;
Bool  assocTerm;
#endif
{
   S16 ret = RFAILED;

   CZ_TRC2(czNmmDisablePeer);
#if (ERRCLASS & ERRCLS_DEBUG)
   /*Check the Pointers*/
   if(peerCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmDisablePeer: Null Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
#endif
   CZ_PRNT_PEERID;
   CZ_DBG_INFO((CZ_PRNT_BUF, "Disabling peerCb.\n"));

   /*Based on the Upper API Argument*/
   /* If the AssocTerm is TRUE. Then deletion will be taken care in the 
    * TermCfm API. then the same function can be invoked with the 
    * assocTerm = FALSE */
   if(assocTerm)
   {
      /* If the Assoc Term failed that means the Association could be down 
       * so it is better to Reset the PeerCB.
         Bool value True = Forceful. FALSE= Graceful*/ 
      ret = czTpmTermAssoc(peerCb->assocCb, FALSE);
      if (ret == ROK)
      {
         CZ_RETVALUE(ROK);
      }

      CZ_DBG_FATAL((CZ_PRNT_BUF,
            "czNmmDisablePeer: Call to czLiTermAssoc Failed Assoc is down\n"));
   }
   /* Do not check the ret value as it returns FAIL only when peerCb is NULL Check */
   czNmmResetPeer(peerCb);

   CZ_RETVALUE(ROK);
}/*czNmmDisablePeer */





/**
     @brief    The function czNmmAddUeCb allocates and adds a new UE CB into the hash 
               list within peerCb using ueCbKey as the key 
     
     @details  The function czNmmAddUeCb allocates and adds a new UE CB into the hash 
               list within peerCb using ueCbKey as the key.Additionally it initialize the
               state and peerCb pointers.
      
     @param[in] peerCb      Peer Control Block. 
     @param[in] ueCbKey     Key for the UeCb. 
     @param[out] ueCb       Pointer to UE Control Block to be  returned. 
     @param[in] oldUEX2APId Old UE X2AP ID. 
      
     @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
        - ROUTRES - If the procedure was out of resource.
     
*/

#ifdef ANSI
PUBLIC S16  czNmmAddUeCb
(
CzPeerCb *peerCb,
U32 ueCbKey,
CzUeCb **ueCb,
U16 oldUEX2APId
 )
#else
PUBLIC S16 czNmmAddUeCb(peerCb, ueCbKey,ueCb, oldUEX2APId)
CzPeerCb *peerCb;
U32 ueCbKey;
CzUeCb **ueCb;
U16 oldUEX2APId;
#endif
{
   CzUeCb      *tmpUeCb = NULLP;
   S16         ret = RFAILED;
   U8         keyPrefix; /* window's compilation fix changed data type to u8 frm u16 */

   CZ_TRC2(czNmmAddUeCb);

   CZ_DBG_INFO((CZ_PRNT_BUF, "Adding UeCb oldUEX2APId = %d.\n",oldUEX2APId));
   /* Check the Pointer */
   if(peerCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "Null Peer Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
   CZ_PRNT_PEERID;

   if(ueCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "Null ueCb Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }

   *ueCb = NULLP;

   CZ_ALLOC(tmpUeCb,sizeof(CzUeCb));

   if(tmpUeCb == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF,
           "Couldn't allocate memory for ueCb\n"));
      CZ_RETVALUE(ROUTRES);
   }
   
   /* Initialize The TImers */
   cmInitTimers(tmpUeCb->bmpTimers, CZ_BMP_MAX_TIMERS);

   /* Set the Peer CB*/
   tmpUeCb->peerCb = peerCb;
   tmpUeCb->oldUEX2APId = oldUEX2APId;
   tmpUeCb->newUEX2APId =  CZ_UNKNOWN_UE_X2AP_ID;
   tmpUeCb->ueCbKey = ueCbKey;
   CZ_BMP_GET_UECB_KEY_PREFIX(ueCbKey, keyPrefix);
   /*kworks fix :type cast to U8 */
   tmpUeCb->eNbType = (U8) keyPrefix;
   
   /*Insert into the Hash List*/
   ret = cmHashListInsert (&peerCb->ueCbHlCp, (PTR)tmpUeCb,
         (U8 *)&tmpUeCb->ueCbKey, sizeof (U32));

   if(ret != ROK)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,
           "Couldn't Insert ueCb in the peerCb->ueCbHlCp\n"));
      CZ_FREE(tmpUeCb, sizeof(CzUeCb));
      CZ_RETVALUE(RFAILED);
   }
   
   /* Initialize the State to the IDLE */
   tmpUeCb->state = CZ_BMP_HO_IDLE;

   *ueCb = tmpUeCb;

   CZ_RETVALUE(ROK);   
}

/**
     @brief    The function czNmmFindUeCb finds a UE CB from the hash 
               list within peerCb using ueCbKey as the key 

     @details  The function czNmmFindUeCb finds a UE CB from the hash 
               list within peerCb using ueCbKey as the key 
      
     @param[in]  peerCb      PeerCb to get the Hash List. 
     @param[in]  ueCbKey     The key to extract ueCb from the Hash.
     @param[out] ueCb        The pointer to ueCb from the Hash.
      
     @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
     
*/


#ifdef ANSI
PUBLIC S16  czNmmFindUeCb
(
CzPeerCb *peerCb,
U32 ueCbKey,
CzUeCb  **ueCb
 )
#else
PUBLIC S16 czNmmFindUeCb (peerCb,  ueCbKey, ueCb) 
CzPeerCb *peerCb;
U32 ueCbKey;
CzUeCb  **ueCb;
#endif
{
   S16 found = RFAILED;

   CZ_TRC2(czNmmFindUeCb);
#ifdef ALIGN_64BIT
   CZ_DBG_INFO((CZ_PRNT_BUF, "Finding UeCb. Key = %d\n", ueCbKey));
#else
   CZ_DBG_INFO((CZ_PRNT_BUF, "Finding UeCb. Key = %ld\n", ueCbKey));
#endif

#if (ERRCLASS & ERRCLS_DEBUG)
   /* Check the Pointers */
   if((!peerCb) || (!ueCb))
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmFindUeCb: Null peerCb/ueCb Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
#endif
   *ueCb = NULLP;

   found = cmHashListFind(&peerCb->ueCbHlCp, (U8 *)&ueCbKey,   
         sizeof(U32), 0, (PTR *)ueCb);

   if (found == ROK)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "UE found.\n"));
      CZ_RETVALUE(ROK);
   }

   CZ_RETVALUE(RFAILED);
}

/**
     @brief    The function czNmmDelUeCb deletes the ueCb  

     @details  The function czNmmDelUeCb deletes the ueCb.Additionally, it removes
               the ueCb from the hash List.
      
     @param[in] ueCb  The ueCb Pointer to be deleted.
      
     @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
     
*/

#ifdef ANSI
PUBLIC S16  czNmmDelUeCb
(
CzUeCb *ueCb
)
#else
PUBLIC S16 czNmmDelUeCb (ueCb)
CzUeCb *ueCb;
#endif
{
   S16 found = RFAILED;
   S16 ret = RFAILED;
   CzUeCb  *locUeCb=NULLP;

   CZ_TRC2(czNmmDelUeCb);
   
#if (ERRCLASS & ERRCLS_DEBUG)
   /* Check the Pointer */
   if(ueCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmDelUeCb: Null peerCb Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
#endif

#ifdef ALIGN_64BIT
   CZ_DBG_INFO((CZ_PRNT_BUF, "Deleting UE. UE Key = %d\n", ueCb->ueCbKey));
#else
   CZ_DBG_INFO((CZ_PRNT_BUF, "Deleting UE. UE Key = %ld\n", ueCb->ueCbKey));
#endif
   found = cmHashListFind(&ueCb->peerCb->ueCbHlCp, (U8 *)&ueCb->ueCbKey,
         sizeof (U32), 0, (PTR *)&locUeCb);
   if (found != ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
            "czNmmDelUeCb: UeCb not found in HAsh list\n"));
      CZ_RETVALUE(RFAILED);
   }
    ret = cmHashListDelete (&ueCb->peerCb->ueCbHlCp,(PTR)locUeCb); 
   /* ret = cmHashListDelete (&czCb.nodeCb.peerCbLst,(PTR)locUeCb); */
   if(ret != ROK)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,
            "czNmmDelUeCb: HAsh list Delete Failed\n"));
   }
   
   /* Stop All Timers related to UE*/
   if(ueCb->eNbType == CZ_SRC)
      czStopUeTmrs(locUeCb);

   /*Free the POinter*/
   CZ_FREE(locUeCb, sizeof(CzUeCb));

   CZ_RETVALUE(ROK);
}


/**
     @brief    The function czNmmAddRsrcCb allocates resource CB.

     @details  The function czNmmAddRsrcCb allocates resource CB.Additionally,
               it addds resource CB into the hash list within peerCb using the key.
      
     @param[in]  peerCb      Peer Control Block. 
     @param[in]  rsrcCbKey   Key to get the rsrcCb. 
     @param[out] rsrcCb      Pointer to RSRC Control Block to be  returned. 
     @param[in]  eNb1MeasId  eNodeB Measurement Id. 
      
     @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
        - ROUTRES - If the procedure was out of resource.
     
*/

#ifdef ANSI
PUBLIC S16  czNmmAddRsrcCb
(
CzPeerCb *peerCb,
U32 rsrcCbKey,
CzRsrcCb **rsrcCb,
U16 eNb1MeasId 
 )
#else
PUBLIC S16 czNmmAddRsrcCb(peerCb, rsrcCbKey,rsrcCb, eNb1MeasId)
CzPeerCb *peerCb;
U32 rsrcCbKey;
CzRsrcCb **rsrcCb;
U16 eNb1MeasId;
#endif
{
   CzRsrcCb   *rsCb = NULLP;
   S16         ret = RFAILED;

   CZ_TRC2(czNmmAddRsrcCb);

#if (ERRCLASS & ERRCLS_DEBUG)
   /* Check the Pointer */
   if(peerCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           " Null Peer Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
   CZ_PRNT_PEERID;

   if(rsrcCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           " Null rsrcCb Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
#endif

   CZ_DBG_INFO((CZ_PRNT_BUF, "Adding RsrcCb. eNb1MeasId = %d\n", eNb1MeasId));
   *rsrcCb = NULLP;
   
   /* cz001.101 : Debug Prints added */
   CZ_ALLOC(rsCb, sizeof(CzRsrcCb));
   if(rsCb == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF,
           "czNmmAddRsrcCb: Couldn't allocate memory for ueCb\n"));
      CZ_RETVALUE(ROUTRES);
   }

   /* Set the Peer CB*/
   rsCb->peerCb = peerCb;
   rsCb->eNb1MeasId = eNb1MeasId;
   rsCb->eNb2MeasId = CZ_UNKNOWN_MEAS_ID;
   rsCb->rsrcCbKey = rsrcCbKey;
   rsCb->state = CZ_RSRC_IDLE;
   rsCb->retxBuf = NULLP;

   /*Insert into the Hash List*/
   ret = cmHashListInsert (&peerCb->resrcCbHlCp, (PTR)rsCb,
         (U8 *)&rsCb->rsrcCbKey, sizeof (U32));
   if(ret != ROK)
   {
      CZ_DBG_FATAL((CZ_PRNT_BUF,
           "czNmmAddRsrcCb: Couldn't Insert the rsCb into the Hash List\n"));
      CZ_RETVALUE(RFAILED);
   }

   *rsrcCb = rsCb;

   CZ_RETVALUE(ROK);   
}

/**
     @brief    The function czNmmFindRsrcCb finds a Resource CB. 

     @details  The function czNmmFindRsrcCb finds a Resource CB from the hash 
               list resrcCbHlCp within peerCb using resCbKey as the key  
      
     @param[in]  peerCb     The Peer Control Block. 
     @param[in]  rsCbKey    The key to extract rsrcCB from the Hash.
     @param[out] rsCb       The rsCb Pointer to be searched.
      
     @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/

#ifdef ANSI
PUBLIC S16  czNmmFindRsrcCb
(
CzPeerCb *peerCb,
U32 rsCbKey,
CzRsrcCb **rsCb
 )
#else
PUBLIC S16 czNmmFindRsrcCb (peerCb,  rsCbKey, rsCb) 
CzPeerCb *peerCb;
U32 rsCbKey;
CzRsrcCb **rsCb;
#endif
{
   S16 found = RFAILED;

   CZ_TRC2(czNmmFindRsrcCb);
#ifdef ALIGN_64BIT
   CZ_DBG_INFO((CZ_PRNT_BUF, "Finding RsrcCb. Key = %d\n", rsCbKey));
#else
   CZ_DBG_INFO((CZ_PRNT_BUF, "Finding RsrcCb. Key = %ld\n", rsCbKey));
#endif


#if (ERRCLASS & ERRCLS_DEBUG)
   /* Check the Pointer */
   if(peerCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmFindRsrcCb: Null peerCb Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
   CZ_PRNT_PEERID;

   if(rsCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmFindRsrcCb: Null rsCb Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
#endif

   *rsCb = NULLP;
   found = cmHashListFind(&peerCb->resrcCbHlCp, (U8 *)&rsCbKey,   
         sizeof (U32), 0, (PTR *)rsCb);

   if (found == ROK)
   {
      CZ_DBG_INFO((CZ_PRNT_BUF, "Rsrc Cb Found.\n"));
      CZ_RETVALUE(ROK);
   }

   CZ_RETVALUE(RFAILED);
}

/**
     @brief    The function czNmmDelRsrcCb deletes the rsCb  

     @details  The function czNmmDelRsrcCb deletes the rsCb.Additionally, it removes
               the rsCb from the hash List.
      
     @param[in] rsCb  The rsCb Pointer to be deleted.
      
     @return
        - ROK     - If the procedure was successful.
        - RFAILED - If the procedure was unsuccessful due to any failure.
*/

#ifdef ANSI
PUBLIC S16  czNmmDelRsrcCb
(
CzRsrcCb *rsCb
)
#else
PUBLIC S16 czNmmDelRsrcCb ( rsCb)
CzRsrcCb *rsCb;
#endif
{
   S16 found = RFAILED;
   S16 ret = RFAILED;
   CzRsrcCb  *locrsCb=NULLP;

   CZ_TRC2(czNmmDelRsrcCb);
   CZ_DBG_INFO((CZ_PRNT_BUF, "Deleting RsrcCb.\n"));
   
#if (ERRCLASS & ERRCLS_DEBUG)
   /* Check for the NULL rsCb */
   if(rsCb == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmDelRsrcCb: Null rsCb Pointer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
#endif
#ifdef ALIGN_64BIT
   CZ_DBG_INFO((CZ_PRNT_BUF, "rsrcCbKey = %d.\n", rsCb->rsrcCbKey));
#else
   CZ_DBG_INFO((CZ_PRNT_BUF, "rsrcCbKey = %ld.\n", rsCb->rsrcCbKey));
#endif
   found = cmHashListFind(&rsCb->peerCb->resrcCbHlCp, (U8 *)&rsCb->rsrcCbKey,   
         sizeof (U32), 0, (PTR *)&locrsCb);
   if (found != ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
            "czNmmDelRsrcCb: rsCb not found in Hash list\n"));
      CZ_RETVALUE(RFAILED);
   }
   ret = cmHashListDelete (&rsCb->peerCb->resrcCbHlCp,(PTR)locrsCb);
   if (ret != ROK )
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmDelRsrcCb: Deletion from the peerCb->resrcCbHlCp failed\n"));
      CZ_RETVALUE(RFAILED);
   }

   /* Free the Buffer*/
   CZ_FREEMBUF(rsCb->retxBuf);

   CZ_FREE(rsCb, sizeof(CzRsrcCb));

   CZ_RETVALUE(ROK);
}

/**
     @brief    The function czNmmMatchPeer searches the Peer 

     @details  The function czNmmMatchPeer searches the Peer based on 
               the transport Address.
      
     @param[in] dstNAddrLst The address to match 
     @param[in] port The port to match in case the Peers are runnning at the same Ip and MultiPort 
      
     @return
        - peerCb     - If the peer  was found.
        - NULLP      - If the peer was could not be found 
*/

#ifdef ANSI
PUBLIC CzPeerCb* czNmmMatchPeer
(
CztNetAddrLst *dstNAddrLst,
SctPort        port
)
#else
PUBLIC CzPeerCb* czNmmMatchPeer(dstNAddrLst, port)
CztNetAddrLst *dstNAddrLst;
SctPort        port;
#endif
{
   S16 idx = 0;
   S16 nmbPeer = 0;
   CzPeerCb *nextCb = NULLP;
   CzPeerCb *peerCb = NULLP;
   CmNetAddr *peerLstAddr = NULLP;
   CmNetAddr *dstPeerAddr = NULLP;

   CZ_TRC2(czNmmMatchPeer);

#if (ERRCLASS & ERRCLS_DEBUG)
   /* Check for the NULL rsCb */
   if(dstNAddrLst == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmMatchPeer: Null dstNAddrLst Pointer as the Argument\n"));
      CZ_RETVALUE(NULLP);
   }
#endif

   while (cmHashListGetNext(&czCb.nodeCb.peerCbLst,
            (PTR)nextCb,
            (PTR *)&peerCb) == ROK)
   {
      if(peerCb != NULLP)
      {
         for(idx = 0; idx < peerCb->assocCb->assocCfg->\
                       dstAddrLst.nmb; idx++)
         {
            peerLstAddr = &(peerCb->assocCb->assocCfg->\
                                          dstAddrLst.nAddr[idx]);
            for(nmbPeer = 0; nmbPeer < dstNAddrLst->nmb; nmbPeer++)
            {
               dstPeerAddr = &(dstNAddrLst->nAddr[nmbPeer]);
               if(czNmmCmpIpAddr(peerLstAddr, dstPeerAddr) == TRUE)
               {
                  if(peerCb->assocCb->assocCfg->dstPort == port)
                  {
                     CZ_DBG_INFO((CZ_PRNT_BUF, "Peer Matched.\n"));
                     CZ_RETVALUE(peerCb);
                  }
               }
            }
         }
      }
      nextCb = peerCb;
      peerCb = NULLP;
   }

   CZ_DBG_INFO((CZ_PRNT_BUF, "Peer Failed to Matched.\n"));
   CZ_RETVALUE(NULLP);
}

/**
     @brief    The function czNmmCmpIpAddr compares the IP Address

     @details  The function czNmmCmpIpAddr compares the IP Address.
               It uses the match flag and switches to IPV4 and IPV6 
               address type and match the corresponding structure.
      
     @param[in] peerAddr The peer address to match 
     @param[in] dstAddr The address received to match 
      
     @return
        - TRUE           - If the match was found.
        - FALSE       - If the match was not successful 
*/


#ifdef ANSI
PRIVATE Bool  czNmmCmpIpAddr
(
CmNetAddr    *peerAddr,      /* Address to be compared with */
CmNetAddr    *dstAddr        /* Address to be compared */
)
#else
PRIVATE Bool czNmmCmpIpAddr (peerAddr, dstAddr)
CmNetAddr    *peerAddr;      /* Address to be compared with */
CmNetAddr    *dstAddr;       /* Address to be compared */
#endif
{
   Bool matchFlag = FALSE;
   U32  ip6Cze = 0;

   CZ_TRC2(czNmmCmpIpAddr);

   /* Compare the address type. If they are of same 
    * type, compare the address else return FALSE
   */
   if((peerAddr->type == dstAddr->type))
   {
      switch (peerAddr->type)
      {
         case CM_NETADDR_IPV4:
         {  
            if (peerAddr->u.ipv4NetAddr == dstAddr->u.ipv4NetAddr)
            {
               matchFlag = TRUE;
            }
            else
            {
               matchFlag = FALSE;
            }
         } /* end of CM_NETADDR_IPV4 */
         break;
         case CM_NETADDR_IPV6:
         {
            matchFlag = TRUE;
            if (peerAddr->u.ipv6NetAddr[ip6Cze] !=
                  dstAddr->u.ipv6NetAddr[ip6Cze])
            {
               matchFlag = FALSE;
            }
         } /* end of CM_NETADDR_IPV6 */
         break;
         default:
            matchFlag = FALSE;
            CZ_DBG_INFO((CZ_PRNT_BUF,
                "czNmmCmpIpAddr: IP Address is other than the IPV4/IPV6\n"));
            break;
            
      } /* end of switch */
   } /* end of addrType is not same */
   else
   {
      /* Set the matchFlag to FALSE */
      matchFlag = FALSE;
   }
   CZ_RETVALUE(matchFlag);
}


/*******************************************************/
/* Handling X2AP Error Indication using Node Management
   module in UI or LI Transport module. */
/*******************************************************/

/**
    * @brief This function is the entry point for handling X2AP Error Indication
      message received from user or from peer.

    * @details This function delegates handling X2AP Error Indication
      to either of GPM, BMP based on the procedure code present in 
      Criticality Diagnostics IE when this message is received from user
      or from peer.
    *  
    * @param[in] peerCb      The active peerCb from this eNodeB with which Error
                             Indication is sent or received.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received from 
                             user or from peer.
    * @param[in] msgDirectn  The Message direction: 
                            -CZ_DIR_OUT: If from user, 
                            -CZ_DIR_IN: If from peer. 
    *  
    * @return 
    * - ROK on success.
    * - Suitable error code otherwise.
    * 
    */

#ifdef ANSI
PUBLIC S16 czNmmHdlErrInd
(
CzPeerCb  *peerCb,
CztEvnt   *evnt,                    
U8        msgDirectn                    
)
#else
PUBLIC S16 czNmmHdlErrInd(peerCb, evnt, msgDirectn)
CzPeerCb  *peerCb;                
CztEvnt   *evnt;                   
U8        msgDirectn;
#endif
{
   S16 ret;

   CZ_TRC2(czNmmHdlErrInd);

   ret = czNmmHdlProcErrInd(peerCb, evnt, msgDirectn);

   /* Assuming nothing can be done more on this message, simply delegate
    * to next processing module */
   if(ROKPENDING == ret)
   {
      /* Error Indication from peer through TPM */
      if(CZ_DIR_IN == msgDirectn)
      {
#ifdef ALIGN_64BIT
         CZ_DBG_INFO((CZ_PRNT_BUF, "Delivering Error Indication from peer(%d)"\
                "to user through NMM \n",(peerCb->peerId)));
#else
         CZ_DBG_INFO((CZ_PRNT_BUF, "Delivering Error Indication from peer(%ld)"\
                "to user through NMM \n",(peerCb->peerId)));
#endif
         /* Increment the incoming counter for this message */
         czUtlUpdInSts(peerCb, CZT_MSG_ERR_IND);

         ret = czUimSndGpErrInd(peerCb, evnt);
         /* No need to inform the peer based on return check, since this is
         * received from peer. Peer should have taken appropriate action
         * while sending this message itself. */
         if(ROK != ret)
         {
#ifdef ALIGN_64BIT
            CZ_DBG_ERROR((CZ_PRNT_BUF, "Sending Error Indication "\
                   "to user(%d) failed: cause(%d)",\
                    peerCb->peerId, ret));
#else
            CZ_DBG_ERROR((CZ_PRNT_BUF, "Sending Error Indication "\
                   "to user(%ld) failed: cause(%d)",\
                    peerCb->peerId, ret));
#endif
            /* Increment the error outgoing counter for this message */
            czUtlUpdInErrSts(peerCb, CZT_MSG_ERR_IND);
            CZ_FREEEVNT(evnt);

         }
      }
      /* Error Indication from user through UIM */
      else
      {
         /* Should be simply sent across to peer */  
#ifdef ALIGN_64BIT
         CZ_DBG_INFO((CZ_PRNT_BUF, "Sending Error Indication through NMM"\
                "to peer(%d) on stream(%d)",\
               (peerCb->peerId), peerCb->peerCfg.globalStreamId)); 
#else
         CZ_DBG_INFO((CZ_PRNT_BUF, "Sending Error Indication through NMM"\
                "to peer(%ld) on stream(%d)",\
               (peerCb->peerId), peerCb->peerCfg.globalStreamId)); 
#endif
         /* Increment the outgoing counter for this message */
         czUtlUpdOutSts(peerCb, CZT_MSG_ERR_IND);

         ret = czTpmSndToLower(peerCb, &evnt->pdu, 
                               peerCb->peerCfg.globalStreamId); 

         /* Nothing to roll back */
         if(ROK != ret)
         {
#ifdef ALIGN_64BIT
            CZ_DBG_ERROR((CZ_PRNT_BUF, "Sending Error Indication "\
                   "to peer(%d) failed: cause(%d)",\
                    peerCb->peerId, ret));
#else
            CZ_DBG_ERROR((CZ_PRNT_BUF, "Sending Error Indication "\
                   "to peer(%ld) failed: cause(%d)",\
                    peerCb->peerId, ret));
#endif
            /* Increment the error outgoing counter for this message */
            czUtlUpdOutErrSts(peerCb, CZT_MSG_ERR_IND);

            /* Inform the user too that event has occurred w.r.t peer
            Invoke a certain function in UIM to generate this */
            CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_MSG_SEND_FAILED, 
                                ret,
                                CZT_MSG_ERR_IND,
                                evnt,
                                NULLP) 
         }
         else
         {
            CZ_FREEEVNT(evnt);
         } 
      }
   }
   /* Dont need to peek into old UE X2AP ID and new UE X2AP ID IE Values
      as this is handled by BMP module if required */ 

   /* Dont need to cross check the return value as it is sent to upper interface
      / to transport module from function handlers within BMP / GP module if not
      from here */

   CZ_RETVALUE(ret);
}


/**
    * @brief This function delegates handling X2AP Error Indication
      to either of GPM, BMP based on the procedure code present in 
      Criticality Diagnostics IE when this message is received from user
      or from peer.

    * @details 
    **************************************************************
    ERROR INDICATION
    **************************************************************
    protocolIEs              Presence
    **************************************************************
    Old-eNB-UE-X2AP-ID       Optional
    New-eNB-UE-X2AP-ID       Optional
    Cause                    Optional
    CriticalityDiagnostics   Optional
    **************************************************************
    This function internally makes use of:
    * -Criticality Diagnostics IE -> procedure code value
    * to distinguish on procedure and handling of it with GP or BMP 
    * procedure code value present.
    *  
    * @param[in] peerCb      The active peerCb from this eNodeB with which Error
                             Indication is sent or received.
    * @param[in] evnt        The pointer with X2AP ASN PDU structure received from 
                             user or from peer.
    * @param[in] msgDirectn  The Message direction: 
                            -CZ_DIR_OUT: If from user, 
                            -CZ_DIR_IN: If from peer. 
    *  
    * @return 
    * -# ROKPENDING if X2AP procedure is not distinguished yet.
    * -# Else suitable return value of the function handlers from 
    * GPM or BMP.
    * 
    */

#ifdef ANSI
PRIVATE S16 czNmmHdlProcErrInd
(
CzPeerCb  *peerCb,
CztEvnt   *evnt,                    
U8        msgDirectn                    
)
#else
PRIVATE S16 czNmmHdlProcErrInd(peerCb, evnt, msgDirectn)
CzPeerCb  *peerCb;                
CztEvnt   *evnt;                   
U8        msgDirectn;
#endif
{
   CztCriticalityDiag *criticalityDiag = NULLP;
   /*kworks fix:changed type S16 to U8*/
   U8  procCode = CZ_PROC_UNKNOWN;
   S16  procType;
   U8   msgChoice  = 0xff;
   U8   msgType = CZT_MSG_UNKNOWN;
   S16  ret = ROKPENDING;

   CZ_TRC2(czNmmHdlProcErrInd);
   
#ifdef ALIGN_64BIT
   CZ_DBG_INFO((CZ_PRNT_BUF, "Handling Error Indication message from (%s)"\
                "through NMM for peer(%d)\n", \
   ((msgDirectn == CZ_DIR_OUT) ? ("User"):("Peer")), (peerCb->peerId)));
#else
   CZ_DBG_INFO((CZ_PRNT_BUF, "Handling Error Indication message from (%s)"\
                "through NMM for peer(%ld)\n", \
   ((msgDirectn == CZ_DIR_OUT) ? ("User"):("Peer")), (peerCb->peerId)));
#endif

   if(CZ_DIR_OUT == msgDirectn || CZ_DIR_IN == msgDirectn)
   {
      /* Start with Criticality Diagnostics IE value to get message 
         procedure code value */
      czUtlGetIE(&evnt->pdu, Cztid_CriticalityDiag, (TknU8 **)&criticalityDiag);
      if(NULLP != criticalityDiag)
      {
         if(PRSNT_NODEF == criticalityDiag->procedureCode.pres)
         {
            /*kworks fix: type cast*/
            procCode = (U8)criticalityDiag->procedureCode.val;
            if(PRSNT_NODEF == criticalityDiag->triggeringMsg.pres)
            {
               /*kworks fix: type cast*/
               msgChoice =  (U8)criticalityDiag->triggeringMsg.val;
               if(msgChoice <= X2AP_PDU_UNSUCCESSFULOUTCOME)
                  msgType = czX2MsgMap[procCode][msgChoice];

               /* Get the Procedure Group from procedure code value */
               procType = czUtlGetProcTypeVal(procCode);
               switch(procType) 
               {
                  /* For BMP, special handling can occur only based on
                   * oldUEX2APId if present.
                   * Internally BMP should take care of it */
                  /* Error Indication for Basic Mobility Management Procedure */
                  case CZ_BMP: 
                  {
                     ret = czBmpHdlErrInd(peerCb, evnt, msgType, msgDirectn);
                     CZ_RETVALUE(ret);
                  }
                  case CZ_GP: /* Error Indication is for Global Procedure */
                  {
                     /* Should be able to interpret based on required causeIE type
                         & values */
                     ret = czGpmHdlErrInd(peerCb, msgType, evnt, msgDirectn);
                     CZ_RETVALUE(ret);
                  }
                  case CZ_ERR: /* Error Indication itself */
                  case CZ_PRIV: /* Private Message */
                  {
#ifdef ALIGN_64BIT
                     CZ_DBG_INFO((CZ_PRNT_BUF, "Error Indication on"\
                     "Error Indication / Private Message for peer(%d)",\
                     (peerCb->peerId)));
#else
                     CZ_DBG_INFO((CZ_PRNT_BUF, "Error Indication on"\
                     "Error Indication / Private Message for peer(%ld)",\
                     (peerCb->peerId)));
#endif
                     CZ_FREEEVNT(evnt);

                     break;
                  }   
                  default: /* Unknown Procedure */
                  {
                     if(CZ_DIR_OUT == msgDirectn)
                     {
#ifdef ALIGN_64BIT
                        CZ_DBG_ERROR((CZ_PRNT_BUF, "Error Indication received "\
                         "from user on unknown procedure for peer(%d)\n", \
                                     (peerCb->peerId)));
#else
                        CZ_DBG_ERROR((CZ_PRNT_BUF, "Error Indication received "\
                         "from user on unknown procedure for peer(%ld)\n", \
                                     (peerCb->peerId)));
#endif

                        /* Increment the error outgoing counter for this message */
                        czUtlUpdOutErrSts(peerCb, CZT_MSG_ERR_IND);
                        /* Inform the user that event has occurred
                        Invoke a certain function in UIM to generate this */
                        CZ_SEND_LCL_ERR_ALL(peerCb, CZT_ERRTYPE_INTERNAL, 
                                            CZT_ERRCAUSE_UNEXP_MSG,
                                            CZT_MSG_UNKNOWN,
                                            evnt,
                                            NULLP); 
                        CZ_RETVALUE(ROK);
                     }
                     break; /* This is required if any arrives from peer badly. */
                  }
               }/* Complete based on procType */
            }/* Complete based on triggering message */
         }/* Complete based on Criticality Diagnostics IE -> procedure code */
      }
   }
   CZ_RETVALUE(ret);
}   


/*******************************************************/
/* End of handling X2AP Error Indication in Basic mobility
   management procedure module. */
/*******************************************************/



/**

      @brief    czNmmDeallocAllPeerCb to deallocate all the PeerCb

      @details  This function is called to deallocate all the peers in czCb.
                If the operation is not shutdown then send Term only when the suId
                matches.If suId does not match continue. For shutdown irrespective of
                suId send Term for all.The peerCb will be eventually deleted on receipt of
                TermCfm.
                  
      @param[in] sapId The sapId  to be dealloacted.
      @param[in] opn The operation which causes the DeAllocation.

      @return 
        - ROK           - If the match was found.
        - RFAILED       - If the match was not successful 


*/
#ifdef ANSI
PUBLIC S16 czNmmDeallocAllPeerCb 
(
SpId   sapId,
U8     opn
)
#else
PUBLIC S16 czNmmDeallocAllPeerCb(sapId,  opn)
SpId   sapId;
U8     opn;
#endif
{
   CztStaInd czSta;
   CzPeerCb *crnt = NULLP;
   CzPeerCb *next = NULLP; 

   CZ_TRC2(czNmmDeallocAllPeerCb);

   while (cmHashListGetNext(&czCb.nodeCb.peerCbLst,
            (PTR)crnt,
            (PTR *)&next) == ROK)
   {
      /* If the operation is not shutdown then send Term only when the suId
       * matches.If suId does not match continue. For shutdown irrespective of
       * suId send Term for all*/
      if (( opn != CZ_SHUT_DOWN) && 
         (( next->sctSapCb != NULL) &&(next->sctSapCb->suId != sapId)))
      {
         crnt = next;
         next = NULLP;
         continue;
      }

      next->delPcb = (opn == CZ_SHUT_DOWN) ? TRUE : FALSE;
      /* Terminate the association */
      czTpmTermAssoc(next->assocCb,TRUE);

      /* Fill czSta for the user */
      czSta.type = CZT_STA_TYPE_ASSOC;
      czSta.status = CZT_STATUS_ASSOC_DOWN;
      czSta.reason = CZT_REASON_UBND_SAP;
      czSta.peerId = next->peerId;

      /* Send StaInd to the user */
      if ( (next->cztSapCb != NULL) && 
            (next->cztSapCb->state == LCZ_SAP_ISBOUND) )
      {
      (Void) czUimSndStaInd(next->cztSapCb, &czSta);
      }

      crnt = next;
      next = NULLP;
   }
   CZ_RETVALUE(ROK);
} /* czNmmDeallocAllPeerCb */

/************ Start of DYNAMIC CCONFIG FUNCTIONS *************/

/**

       @brief   czNmmAllocDynamPeerCb

       @details  Allocates a transient peer and assoc control block

       @param[in] suId    The  suId of the SAP.
       @param[out] peer   The  POinter to peer Control Block to be  returned. 
     
       @return
       - ROK     - If the procedure was successful.
       - ROUTRES - If the procedure was out of resource.
       - RFAILED - If the procedure was unsuccessful due to any other failure.

*/

#ifdef ANSI
PUBLIC S16 czNmmAllocDynamPeerCb
(
SuId      suId,
CzPeerCb **peer
)
#else
PUBLIC S16 czNmmAllocDynamPeerCb(suId, peer)
SuId      suId;
CzPeerCb **peer;
#endif
{
   CzPeerCb   *peerCb = NULLP;
   S16         ret;

   CZ_TRC2(czNmmAllocDynamPeerCb);

   /* cz001.301 : Removed redundent code.
    * No need of NULL check for peer, we know that we are assigning
    * NULL to peer before passing to czNmmAllocPeerCb */ 

   *peer = NULLP;

   /* Allocate peer control block */
   CZ_ALLOC(peerCb, sizeof(CzPeerCb));
   if(peerCb == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF,
           "czNmmAllocDynamPeerCb: Couldn't allocate memory for peerCB\n"));
      CZ_RETVALUE(ROUTRES);
   }

   /* Allocate Assoc control block in peer cb */
   CZ_ALLOC(peerCb->assocCb, sizeof(CzAssocCb));
   if(peerCb->assocCb == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF,
           "czNmmAllocDynamPeerCb: Couldn't allocate memory for assocCB\n"));
      CZ_FREE(peerCb, sizeof(CzPeerCb));
      CZ_RETVALUE(RFAILED);
   }

   /* Allocate assoc config in assoc control block */
   CZ_ALLOC(peerCb->assocCb->assocCfg, sizeof(CzAssocCfg));
   if(peerCb->assocCb->assocCfg == NULLP)
   {
      SLogError(0, 0, 0,
            __FILE__, __LINE__, (ErrCls)ERRCLS_ADD_RES,
            (ErrVal)ECZT015, (ErrVal)0, "Memory allocaiton failed");
      CZ_DBG_FATAL((CZ_PRNT_BUF,
           "czNmmAllocDynamPeerCb: Couldn't allocate memory for assocCfg\n"));
      CZ_FREE(peerCb->assocCb, sizeof(CzAssocCb));
      CZ_FREE(peerCb, sizeof(CzPeerCb));
      CZ_RETVALUE(RFAILED);
   }

   peerCb->assocCb->peer = peerCb;

   peerCb->peerCfg.isDynamCfgd = TRUE;
   peerCb->peerCfg.globalStreamId = 0;
   peerCb->sctSapCb = czCb.sctSapLst[suId];
   peerCb->cztSapCb = czCb.cztSapLst[czCb.nodeCb.protCfg.defDPeerUSapId];
   
   /* 
    * Update the Czt SAP as the default SAP 
    * This shall be finally updated when the
    * response to the dynamic Peer
    */

   /* Intialize the Hash Lists */
   ret = cmHashListInit(&peerCb->ueCbHlCp,          /* hash list Cp */
            CZ_HASH_LIST_SIZE,                  /* HL bins */
            0,                                  /* Offset of HL Entry */
            FALSE,                              /* Allow dup. keys ? */
            CZ_HASH_KEYTYPE,                    /* HL key type */
            czCb.init.region,                   /* Mem region for HL */
            czCb.init.pool);                    /* Mem pool for HL */

   if(ret == ROK)
   {
      ret = cmHashListInit(&peerCb->resrcCbHlCp,    /* hash list Cp */
            CZ_HASH_LIST_SIZE,                  /* HL bins */
            0,                                  /* Offset of HL Entry */
            FALSE,                              /* Allow dup. keys ? */
            CZ_HASH_KEYTYPE,                    /* HL key type */
            czCb.init.region,                   /* Mem region for HL */
            czCb.init.pool);                    /* Mem pool for HL */

      if (ret == ROK)
      {
         peerCb->assocCb->peer = peerCb;

         /* Set the State of the Peer to the Idle */
         peerCb->state = CZ_GP_IDLE;
         peerCb->gpmCb.wait=FALSE;
         peerCb->gpmCb.procPending=0;
         peerCb->gpmCb.retxBuf=NULLP;

      }
      else
      {
         cmHashListDeinit(&peerCb->ueCbHlCp);
      }
   }

   if (ret != ROK)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmDynamAllocPeerCb: Failure in the Hash List Operation\n"));
      CZ_FREE(peerCb->assocCb->assocCfg, sizeof(CzAssocCfg));
      CZ_FREE(peerCb->assocCb, sizeof(CzAssocCb));
      CZ_FREE(peerCb, sizeof(CzPeerCb));

      CZ_RETVALUE(RFAILED);
   }

   /* Set the State of the Peer to the Idle */
   cmInitTimers(&peerCb->gpmCb.gpTimer, 1);

   *peer = peerCb;

   CZ_RETVALUE(ROK);   
}


/**
       @brief   czNmmDeallocDynamPeerCb

       @details  It deallocates the transient peer and assoc control block
     
       @param[in] peer The peer  to be dealloacted.


       @return   
        - ROK           - If the match was found.
        - RFAILED       - If the match was not successful 

*/
#ifdef ANSI
PUBLIC S16 czNmmDeallocDynamPeerCb
(
CzPeerCb *peer
)
#else
PUBLIC S16 czNmmDeallocDynamPeerCb(peer)
CzPeerCb *peer;
#endif
{
   CZ_TRC2(czNmmDeallocDynamPeerCb);

   cmHashListDeinit(&peer->ueCbHlCp);
   cmHashListDeinit(&peer->resrcCbHlCp);

   CZ_FREE(peer->assocCb->assocCfg, sizeof(CzAssocCfg));
   CZ_FREE(peer->assocCb ,sizeof(CzAssocCb));
   CZ_FREE(peer, sizeof(CzPeerCb));

   CZ_RETVALUE(ROK);
}

/**
      @brief   czNmmFindDynamPeerAssocCb

      @details  finds transient peerAssoc control block from the hash list

      @param[in] dstAddrLst The Transport Address of the Dynamic Peer.
      @param[in] dstPort    The port of the Dynamic Peer 
      @param[out] peerAssoc The peer  to be returned.

      @return 
        - ROK           - If it is successful 
        - RFAILED       - If it was not successful 

*/
#ifdef ANSI
PUBLIC S16 czNmmFindDynamPeerAssocCb
(
CztNetAddrLst      *dstAddrLst,
CztPort             dstPort,
CzDynamPeerAssocCb **peerAssoc
)
#else
PUBLIC S16 czNmmFindDynamPeerAssocCb(dstAddrLst, dstPort, peerAssoc)
CztNetAddrLst      *dstAddrLst;
CztPort             dstPort;
CzDynamPeerAssocCb **peerAssoc;
#endif
{
   CzDynamPeerAssocEntry  peerAssocEntry;
   CzDynamPeerAssocCb    *peerAssocCb;
   U16                    idx;
   S16                    ret;

   CZ_TRC2(czNmmFindDynamPeerAssocCb);

   /* Check the Pointer */
   if(peerAssoc == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
        "czNmmFindDynamPeerAssocCb: Null peerAssoc as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }

   *peerAssoc  = NULLP;
   peerAssocCb = NULLP;

   for (idx = 0; idx < dstAddrLst->nmb; idx++)
   {
      CZ_SET_ZERO(&peerAssocEntry, sizeof(CzDynamPeerAssocEntry));

      peerAssocEntry.peerAddr.type = dstAddrLst->nAddr[idx].type;

      if (peerAssocEntry.peerAddr.type == CM_NETADDR_IPV4)
      {
         peerAssocEntry.peerAddr.u.ipv4NetAddr = 
            dstAddrLst->nAddr[idx].u.ipv4NetAddr;
      }
      else
         (Void) cmMemcpy((U8 *)&(peerAssocEntry.peerAddr.u.ipv6NetAddr),
                         (U8 *)&(dstAddrLst->nAddr[idx].u.ipv6NetAddr),
                         (PTR) CM_IPV6ADDR_SIZE);

      peerAssocEntry.port = dstPort;

      ret = cmHashListFind(&(czCb.nodeCb.dynamPeerAssocLst), 
            (U8 *) &(peerAssocEntry),
            sizeof(CzDynamPeerAssocEntry), 0, (PTR *) &peerAssocCb);
      if (ret == ROK)
      {
         break;
      }
   }

   if (idx == dstAddrLst->nmb)
   {
      CZ_RETVALUE(RFAILED);
   }
   
   *peerAssoc = peerAssocCb;

   CZ_RETVALUE(ROK);
}

/**

      @brief   czNmmRemDynamPeerAssoc

      @details It removes transient peer and assoc control block from the hash list

      @param[in] peer The peer  to be removed.

      @return  
        - ROK           - If it is successful 
        - RFAILED       - If it was not successful 

*/
#ifdef ANSI
PUBLIC S16 czNmmRemDynamPeerAssoc
(
CzPeerCb *peer
)
#else
PUBLIC S16 czNmmRemDynamPeerAssoc(peer)
CzPeerCb *peer;
#endif
{
   SctNetAddrLst         *dstAddrLst;
   CzDynamPeerAssocEntry  peerAssocEntry;
   CzDynamPeerAssocCb    *peerAssocCb;
   Bool                   allFound;
   U16                    idx;
   S16                    ret = RFAILED;

   CZ_TRC2(czNmmRemDynamPeerAssoc);

    /* Check the Pointer */
   if(peer == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmRemDynamPeerAssoc: Null  peer as the Argument\n"));
      CZ_RETVALUE(RFAILED);
   }
  

   allFound = TRUE;
   dstAddrLst = &peer->assocCb->assocCfg->dstAddrLst;

   for (idx = 0; idx < dstAddrLst->nmb; idx++)
   {
      CZ_SET_ZERO(&peerAssocEntry, sizeof(CzDynamPeerAssocEntry));

      peerAssocEntry.peerAddr.type = dstAddrLst->nAddr[idx].type;

      if (peerAssocEntry.peerAddr.type == CM_NETADDR_IPV4)
      {
         peerAssocEntry.peerAddr.u.ipv4NetAddr = 
            dstAddrLst->nAddr[idx].u.ipv4NetAddr;
      }
      else
      {
         (Void) cmMemcpy((U8 *)&(peerAssocEntry.peerAddr.u.ipv6NetAddr),
                         (U8 *)&(dstAddrLst->nAddr[idx].u.ipv6NetAddr),
                         (PTR) CM_IPV6ADDR_SIZE);
      }

      peerAssocEntry.port = peer->assocCb->assocCfg->dstPort;

      ret = cmHashListFind(&(czCb.nodeCb.dynamPeerAssocLst), 
            (U8 *) &(peerAssocEntry),
            sizeof(CzDynamPeerAssocEntry), 0, (PTR *) &peerAssocCb);
      if (ret != ROK)
      {
         allFound = FALSE;
         continue;
      }
      
      cmHashListDelete(&(czCb.nodeCb.dynamPeerAssocLst), (PTR)(peerAssocCb));
      CZ_FREE(peerAssocCb, sizeof(CzDynamPeerAssocCb));
   }

   if (allFound == FALSE)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
           "czNmmRemDynamPeerAssoc: No  peer found  \n"));
      CZ_RETVALUE(RFAILED);
   }
   
   CZ_RETVALUE(ROK);
}

/************ End of DYNAMIC CCONFIG FUNCTIONS *************/

/**
       @brief   czNmmResetPeer

       @details  This function removes transient data and control block from
                 the hash list of the PeerCb
       
       @param[in] peer The peerCb to be used. 

       @return   
         - Void       

*/
#ifdef ANSI
PUBLIC Void czNmmResetPeer
(
CzPeerCb *peer
)
#else
PUBLIC Void czNmmResetPeer(peer )
CzPeerCb *peer;
#endif
{
   S16 ret = RFAILED;
   CzUeCb   *ueCb = NULLP;
   CzRsrcCb   *rsCb = NULLP;

   CZ_TRC2(czNmmResetPeer);

#if (ERRCLASS & ERRCLS_DEBUG)
   if(peer == NULLP)
   {
      CZ_DBG_ERROR((CZ_PRNT_BUF,
               "czNmmResetPeer: Null pointer of peer passed\n"));
      RETVOID; 
   }
#endif

   /* The Entry for all the UEs should be removed */
   while (cmHashListGetNext(&peer->ueCbHlCp, (PTR) NULLP,
               (PTR *) &ueCb) == ROK)
   {
      ret = czNmmDelUeCb(ueCb);
      if(ret != ROK)
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF,
               "czNmmResetPeer: UE Could not be deleted form ueCbHlCp\n"));
      }
      else
      {
         ueCb = NULLP;
      }
   }

   /* The Entry for all the Resource CBs should be removed */
   while (cmHashListGetNext(&peer->resrcCbHlCp, (PTR) NULLP,
               (PTR *) &rsCb) == ROK)
   {
      ret = czNmmDelRsrcCb(rsCb);
       if(ret != ROK)
      {
         CZ_DBG_ERROR((CZ_PRNT_BUF,
              "czNmmResetPeer: ResourCb Could not be deleted form rsrcbLst\n"));
      }
       else
       {
          rsCb=NULLP;
       }
   }

   /* Stop The Timers */
   czStopTmr(CZ_TMR_TIME_TO_WAIT , peer);

   /*reset the Statistics*/
   CZ_SET_ZERO(&peer->peerCfg.peerSts, sizeof(CzPeerNodeSts));

   /* Deletes the Buffer */
   CZ_FREEMBUF(peer->gpmCb.retxBuf);

   /* Set the State of the Peer to the Idle */
   peer->state = CZ_GP_IDLE;
   peer->gpmCb.wait=FALSE;
   peer->gpmCb.procPending = 0;

}


/** @} */


/**********************************************************************
  
         End of file:     tr_nmm.c@@/main/2 - Tue Aug 30 18:36:13 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        aj              1. LTE-X2AP Initial Release.
            cz001.101   mm              1. CR Fixes, Memory Leak fixes.
/main/2      ---        pt              1. LTE-X2AP 3.1 release.
/main/2     cz001.301  akaranth         1. Added a new function to deallocate dynamPeerCb.
*********************************************************************91*/
