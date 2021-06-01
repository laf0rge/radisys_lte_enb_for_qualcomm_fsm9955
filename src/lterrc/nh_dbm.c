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

     Name:     LTE-RRC Layer 
  
     Type:     C Source file
  
     Desc:     Provides the APIs to access/modify the RRC
               internal data structures.

     File:     nh_dbm.c

     Sid:      ni_dbm.c@@/main/3 - Fri Jul  1 01:13:17 2011

     Prg:      gm

**********************************************************************/

/** @defgroup dbm_group Database Module
  * @{
  */

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=86;
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* timer services */
#include "cm_tkns.h"       /* Common Token defines */
#include "cm_llist.h"      /* Common Link list defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "lnh.h"           /* LNH Interface defines */
#include "nhu.h"           /* NHU Interface defines */
#include "nh.h"            /* RRC defines */

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_lte.x"        /* Common LTE includes */
#include "lnh.x"           /* LNH Interface includes */
#include "nhu.x"           /* NHU Interface includes */
#include "nh.x"            /* RRC includes */

/* local defines */

/* local typedefs */

/* local externs */

/* forward references */

PRIVATE S16 nhDBMCommitRbAddCfg ARGS(( NhOrdrdCfgInfo    *ordCfgInfo,
                                   NhCrntCfgInfo     *crntCfgInfo));
PRIVATE S16 nhDBMCommitRbModCfg ARGS (( NhOrdrdCfgInfo    *ordCfgInfo,
                                   NhCrntCfgInfo     *crntCfgInfo));
PRIVATE S16 nhDBMCommitRbRlsCfg ARGS (( NhOrdrdCfgInfo    *ordCfgInfo,
                                   NhCrntCfgInfo     *crntCfgInfo));
PRIVATE S16 nhDBMCommitUeCfg    ARGS (( NhActvUeCb    *ueCb,
                                   TransCb     *transCtrlBlk));

PRIVATE S16 nhDBMFillLogchPdcpInfo ARGS (( NhRbToLchMap   *dstCfgInfo,
                                   NhuRbCfgInfo   *srcRbCfgInfo,
                                   U8             action));

PRIVATE S16 nhDBMUpdRbToLchOrPdcp ARGS (( NhCrntCfgInfo *crntCfg,
                                   NhuRbCfgInfo  *rbCfgInfo,
                                   U8            action));

PRIVATE S16 nhInitUeCb ARGS (( NhActvUeCb   *ueCb ));

PRIVATE S16 nhDBMCommitCellOrdrdCfgAdd ARGS (( NhCellCb   *cellCb));


/**
 * @brief Creates transaction control block in the UE context for a 
          transaction triggered by the RRC user.
 * 
 * @details 
 * 
 *
 * This function creates new transaction in the UE context, if the pre-existing
 * transaction contexts are not free. If the context is free, one of 
 * the existing contexts is allocated.
 *
 * @param[in] ueCb          UE context
 * @param[out] transCtrlBlk      UE Transaction context
 * @param[in] msgTransKey   Message Transaction Key
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMAddTransCb
(
NhActvUeCb  *ueCb,
TransCb     **transCtrlBlk,
U32         msgTransKey
)
#else /* ANSI */
PUBLIC S16 nhDBMAddTransCb(ueCb, transCtrlBlk, msgTransKey)
NhActvUeCb  *ueCb;
TransCb     **transCtrlBlk;
U32         msgTransKey;
#endif /* ANSI */
{
   U8        i;
   S16       ret;
   CmLList   *tmp = NULLP;
   TransCb   *locTransCb=NULLP;

   TRC2(nhDBMAddTransCb)

   RLOG0(L_DEBUG, "Addition of New Transaction Info Block.");

   /* There is already a link list existing in the UE control block
    * having the minimum number of transaction info blocks.
    */
   /* Search must be done initially amongst the already available
    * transaction info blocks. If free entry is not available,
    * a new entry is created and appended to the already existing
    * transaction info blocks.
    */
   CM_LLIST_FIRST_NODE(&(ueCb->transCbLst), tmp);
   for (i=0; i < nhCb.genCfg.minTransLstLen; i++)
   {
      locTransCb = (TransCb *)tmp->node;

      if (NH_STATUS_UNUSED == locTransCb->status)
      {
         locTransCb->msgTransKey = msgTransKey;
         /* Insert the Entry in the Message Transaction HashList */
         ret = nhHashListInsert(&ueCb->msgTransHlCp, (PTR)locTransCb,
                           (U8 *)&locTransCb->msgTransKey, sizeof(U32));
         if (ROK != ret)
         {
            RETVALUE(ret);
         }
         /* CR-ccpu00136443 Changes, Changing the Status only 
          * if Hash inset is success */
         locTransCb->status = NH_STATUS_USED;
         *transCtrlBlk = locTransCb;
         RETVALUE(ROK);
      }
      CM_LLIST_NEXT_NODE(&(ueCb->transCbLst), tmp);
   }

   /* Get a new block and add it in the TransCbLst. */
   NH_ALLOC(NH_REG, NH_POOL, &tmp, sizeof(CmLList));
   if (NULLP == tmp)
   {
      RLOG0(L_FATAL, "Failed to allocate memory.");
      RETVALUE(ROUTRES);
   }

   NH_ALLOC(NH_REG, NH_POOL, &locTransCb, sizeof(TransCb));
   if (NULLP == locTransCb)
   {
      NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
      RLOG0(L_FATAL, "Failed to allocate memory.");
      RETVALUE(ROUTRES);
   }

   /* Reserve the locTransCb to the caller */
   locTransCb->status = NH_STATUS_USED;
   /* Set the transId, first set it to invalid so that
    * the macro updates with valid value */
   locTransCb->transId = NH_RESERVED_TRANS_ID;
   NH_ALLOC_TRANS_ID(locTransCb, &(locTransCb->transId))
   /* store the msgTransKey into locTransCb */
   locTransCb->msgTransKey = msgTransKey;

   tmp->node = (PTR)(locTransCb);


   /* Insert the entry in the Message Transaction Hashlist */
   ret = nhHashListInsert(&ueCb->msgTransHlCp, (PTR)locTransCb,
                           (U8 *)&locTransCb->msgTransKey, sizeof(U32));

   if (ROK != ret)
   {
      NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
      NH_FREE(NH_REG, NH_POOL, locTransCb, sizeof(TransCb));
      RETVALUE(ret);
   }
   else /* CR-ccpu00136443 Changes, Inserting to list only if Hash inset is success */
   {
      /* Add the node to the TransCb list */
      cmLListAdd2Tail(&(ueCb->transCbLst), tmp);
   }

   *transCtrlBlk = locTransCb;

   RETVALUE(ROK);
} /* nhDBMAddTransCb */

/**
 * @brief Deletes the transaction control block from the UE context.
 * 
 * @details 
 * 
 *
 * This function deletes the transaction context created in the UE context.
 *
 * @param[in] ueCb      UE context
 * @param[in] transCtrlBlk   UE Transaction context
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMDelTransCb
(
NhActvUeCb  *ueCb,
TransCb     **transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhDBMDelTransCb(ueCb, transCtrlBlk)
NhActvUeCb  *ueCb;
TransCb     **transCtrlBlk;
#endif /* ANSI */
{
   U8       i;
   CmLList  *tmp = NULLP;
   TransCb  *node = NULLP;
   U32      tmpTrId;

   TRC2(nhDBMDelTransCb)

   RLOG0(L_DEBUG, "Deleting TransCb Block.");
   /* There is already a link list existing in the UE control block
    * having the minimum number of transaction info blocks. If the
    * transaction is anyone of the nodes in this default list, then
    * the requested node is not physically deleted. Rather, only
    * the status entry is set to NH_STATUS_UNUSED.
    * Moreover, if the requested entry is not a part of the default
    * list, then that entry is physically deleted.
    *
    * Search must be done initially amongst the already available
    * transaction info blocks.
    */
   if ((*transCtrlBlk)->transId < nhCb.genCfg.minTransLstLen)
   {
      tmpTrId = (*transCtrlBlk)->transId;

      /* Delete the entry from the Message Transaction Hash list */
      nhHashListDelete(&ueCb->msgTransHlCp, (PTR) *transCtrlBlk);

      /* Delete the memory allocated for NhuRbCfgList */
      if ((*transCtrlBlk)->ordrdCfg.actionMask & NH_ORD_CFG_RB_ADD)
      {
         NH_FREE(NH_REG, NH_POOL, 
                  (*transCtrlBlk)->ordrdCfg.rbToAddCfgList.rbCfgInfoList,
                  ((*transCtrlBlk)->ordrdCfg.rbToAddCfgList.numOfRbCfgInfo
                        *sizeof(NhuRbCfgInfo)));
      }
      if ((*transCtrlBlk)->ordrdCfg.actionMask & NH_ORD_CFG_RB_MOD)
      {
         /* Delete UL bitmap of HO info from ordered cfg if present */
         nhUTLFreeUlBitMapFrmOrdCfg(*transCtrlBlk,
                     (*transCtrlBlk)->ordrdCfg.rbToModCfgList.numOfRbCfgInfo);
         NH_FREE(NH_REG, NH_POOL, 
                  (*transCtrlBlk)->ordrdCfg.rbToModCfgList.rbCfgInfoList,
                  ((*transCtrlBlk)->ordrdCfg.rbToModCfgList.numOfRbCfgInfo
                        *sizeof(NhuRbCfgInfo)));
      }
      if ((*transCtrlBlk)->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS)
      {
         NH_FREE(NH_REG, NH_POOL, 
                  (*transCtrlBlk)->ordrdCfg.rbToRlsCfgList.rbsToRel,
                  ((*transCtrlBlk)->ordrdCfg.rbToRlsCfgList.numOfRbCfgInfo
                        *sizeof(NhuRbId)));
      }
      if ((*transCtrlBlk)->ordrdCfg.actionMask & NH_ORD_CFG_RB_RESTB ||
          (*transCtrlBlk)->ordrdCfg.actionMask & NH_ORD_CFG_RESTB_SRB1)
      {
         CM_LLIST_FIRST_NODE(&(*transCtrlBlk)->ordrdCfg.rbToRestbCfgList.rbCfgInfoList, tmp);
        
         while(NULLP != tmp)
         {
            cmLListDelFrm(&(*transCtrlBlk)->ordrdCfg.rbToRestbCfgList.rbCfgInfoList, tmp);
            NH_FREE(NH_REG, NH_POOL, tmp->node, sizeof(NhuRbCfgInfo));
            NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
            CM_LLIST_FIRST_NODE(&(*transCtrlBlk)->ordrdCfg.rbToRestbCfgList.rbCfgInfoList, tmp);
         }
      }

      /* Delete store message and lowerLyrParam strcture */
      if ((*transCtrlBlk)->storedMsg)
      {
         NH_FREEMBUF((*transCtrlBlk)->storedMsg);
      }

      if ((*transCtrlBlk)->lowerLyrParam)
      {
         NH_FREE(NH_REG, NH_POOL, (*transCtrlBlk)->lowerLyrParam,
                  sizeof(NhDatReqParam));
      }

      NH_SET_ZERO((U8 *)*transCtrlBlk, sizeof(TransCb));

      /* Default list element */
      (*transCtrlBlk)->status = NH_STATUS_UNUSED;

      /* Restoring the transId value */
      (*transCtrlBlk)->transId = tmpTrId;

      RETVALUE(ROK);
   }
   else
   {
      /* Entry is not a part of Default List */

      /* Traverse the default list */
      CM_LLIST_FIRST_NODE(&(ueCb->transCbLst), tmp);
      for (i=0; i < nhCb.genCfg.minTransLstLen; i++)
      {
         CM_LLIST_NEXT_NODE(&(ueCb->transCbLst), tmp);
      }

      while(NULLP != tmp)
      {
         node = (TransCb *)(tmp->node);

         if ((*transCtrlBlk)->transId == node->transId)
         {
            /* Delete the Entry from the Message Transaction Hash list */
            nhHashListDelete(&ueCb->msgTransHlCp, (PTR)node);
            /* Block entry found - It must be deleted */
            cmLListDelFrm(&(ueCb->transCbLst), tmp);

            /* Delete the memory allocated for NhuRbCfgList */
            if ((*transCtrlBlk)->ordrdCfg.actionMask & NH_ORD_CFG_RB_ADD)
            {
               NH_FREE(NH_REG, NH_POOL, 
                  (*transCtrlBlk)->ordrdCfg.rbToAddCfgList.rbCfgInfoList,
                  ((*transCtrlBlk)->ordrdCfg.rbToAddCfgList.numOfRbCfgInfo
                        *sizeof(NhuRbCfgInfo)));
            }
            if ((*transCtrlBlk)->ordrdCfg.actionMask & NH_ORD_CFG_RB_MOD)
            {
               /* Delete UL bitmap of HO info from ordered cfg if present */
               nhUTLFreeUlBitMapFrmOrdCfg(*transCtrlBlk,
                     (*transCtrlBlk)->ordrdCfg.rbToModCfgList.numOfRbCfgInfo);

               NH_FREE(NH_REG, NH_POOL, 
                  (*transCtrlBlk)->ordrdCfg.rbToModCfgList.rbCfgInfoList,
                  ((*transCtrlBlk)->ordrdCfg.rbToModCfgList.numOfRbCfgInfo
                        *sizeof(NhuRbCfgInfo)));
            }
            if ((*transCtrlBlk)->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS)
            {
               NH_FREE(NH_REG, NH_POOL, 
                  (*transCtrlBlk)->ordrdCfg.rbToRlsCfgList.rbsToRel,
                  ((*transCtrlBlk)->ordrdCfg.rbToRlsCfgList.numOfRbCfgInfo
                        *sizeof(NhuRbId)));
            }

            /* Delete the store message and the lowerLyrParam strcture */
            if ((*transCtrlBlk)->storedMsg)
            {
               NH_FREEMBUF((*transCtrlBlk)->storedMsg);
            }

            if ((*transCtrlBlk)->lowerLyrParam)
            {
               NH_FREE(NH_REG, NH_POOL, (*transCtrlBlk)->lowerLyrParam,
                        sizeof(NhDatReqParam));
            }

            /* Free the cellUpdCfmRpt if available */
            NH_FREE(NH_REG, NH_POOL, tmp->node, sizeof(TransCb));
            NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
            (*transCtrlBlk) = NULLP;
            break;
         }
         /* Find out the next node */
         CM_LLIST_NEXT_NODE(&(ueCb->transCbLst), tmp);
      }
   }

   RETVALUE(ROK);
} /* nhDBMDelTransCb */


/**
 * @brief Adds MUI control block in the UE context.
 * 
 * @details 
 * 
 *
 * This function returns a MUI information block. If it
 * does not find an pre-existing free entry, it creates a new
 * MUI info block and adds it to the exisiting list. An
 * MUI info block is used to track the transmission of a
 * UE message in AM mode.
 *
 * @param[in] ueCb      This is a pointer to UE control block.
 * @param[out] muiCb    This is a pointer to MUI control block.
 * @param[in] nxTransId This is a RRC user Transaction ID.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMAddMuiCb
(
NhActvUeCb *ueCb,
MuiInfo    **muiCb,
U32        nxTransId
)
#else /* ANSI */
PUBLIC S16 nhDBMAddMuiCb(ueCb, muiCb, nxTransId)
NhActvUeCb *ueCb;
MuiInfo    **muiCb;
U32        nxTransId;
#endif /* ANSI */
{
   U8          i;
   CmLList     *tmp = NULLP;
   MuiInfo     *muiInfo = NULLP;

   TRC2(nhDBMAddMuiCb)
   
   RLOG0(L_DEBUG, "Adding MUI Control Block.");
   
   CM_LLIST_FIRST_NODE(&(ueCb->muiInfoLst), tmp);

   for (i=0; i < nhCb.genCfg.minMuiLstLen; i++)
   {
      if (NULLP == tmp)
         break;

      muiInfo = (MuiInfo *)tmp->node;
      if ((muiInfo != NULLP ) && (NH_STATUS_UNUSED == muiInfo->status))
      {
         muiInfo->status = NH_STATUS_USED;
         muiInfo->nxTransId = nxTransId;
         *muiCb  = muiInfo;
         RETVALUE(ROK);
      }
      CM_LLIST_NEXT_NODE(&(ueCb->muiInfoLst), tmp); 
   }

   /* Get a new block and add it in the MuiInfoLst */
   NH_ALLOC(NH_REG, NH_POOL, &tmp, sizeof(CmLList));
   if (NULLP == tmp)
   {
      RLOG0(L_FATAL, "Failed to allocate memory.");
      RETVALUE(ROUTRES);
   }

   NH_ALLOC(NH_REG, NH_POOL, &muiInfo, sizeof(MuiInfo));
   if (NULLP == muiInfo)
   {
      RLOG0(L_FATAL, "Failed to allocate memory.");
      NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
      RETVALUE(ROUTRES);
   }

   muiInfo->status    = NH_STATUS_USED;
   muiInfo->nxTransId = nxTransId;
   muiInfo->mui       = NH_RESERVED_TRANS_ID;

   NH_ALLOC_MUI_ID(muiInfo, &(muiInfo->mui));

   tmp->node = (PTR)(muiInfo);
   
   *muiCb  = muiInfo;         
   
   /* Add the node to the MuiInfo list */
   cmLListAdd2Tail(&(ueCb->muiInfoLst), tmp);

   RETVALUE(ROK);
} /* nhDBMAddMuiCb */


/**
 * @brief To delete MUI block from the UE context.
 * 
 * @details 
 * 
 *
 * This function deletes the MUI block from the UE context.
 *
 * @param[in] ueCb   UE context
 * @param[in] muiCb  Message Unit Identifier control block
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMDelMuiCb
(
NhActvUeCb *ueCb,
MuiInfo    **muiCb
)
#else /* ANSI */
PUBLIC S16 nhDBMDelMuiCb(ueCb, muiCb)
NhActvUeCb *ueCb;
MuiInfo    **muiCb;
#endif /* ANSI */
{
   U8          i;
   CmLList     *tmp = NULLP;
   MuiInfo     *node = NULLP;

   TRC2(nhDBMDelMuiCb)

   RLOG0(L_DEBUG, "Deletion of Mui Info Block.");

   /* There is already a link list existing in the UE control block
    * having the minimum number of MUI info blocks. If the
    * muiaction is anyone of the nodes in this default list, then
    * the requested node is not physically deleted. Rather, only
    * the status entry is set to NH_STATUS_UNUSED.
    * Moreover, if the requested entry is not a part of the default
    * list, then that entry is physically deleted.
    *
    * Search must be done initially amongst the already available
    * muiaction info blocks.
    */
   if ((*muiCb)->mui < nhCb.genCfg.minMuiLstLen)
   {
      /* Default list element */
      (*muiCb)->status = NH_STATUS_UNUSED;
      (*muiCb)->nxTransId = 0;
      RETVALUE(ROK);
   }
   else
   {
      /* Entry is not a part of default list */

      /* Traverse the default list */
      CM_LLIST_FIRST_NODE(&(ueCb->muiInfoLst), tmp);
      for (i=0; i < nhCb.genCfg.minMuiLstLen; i++)
      {
         CM_LLIST_NEXT_NODE(&(ueCb->muiInfoLst), tmp);
      }

      while(NULLP != tmp)
      {
         node = (MuiInfo *)(tmp->node);

         if ((*muiCb)->mui == node->mui)
         {
            /* Block entry found - it must be deleted */
            cmLListDelFrm(&(ueCb->muiInfoLst), tmp);
            NH_FREE(NH_REG, NH_POOL, tmp->node, sizeof(MuiInfo));
            NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
            (*muiCb) = NULLP;
            break;
         }
         /* Find out the next node */
         CM_LLIST_NEXT_NODE(&(ueCb->muiInfoLst), tmp);
      }
   }

   RETVALUE(ROK);
} /* nhDBMDelMuiCb */




/**
 * @brief Fetches the MUI control block from the UE context.
 * 
 * @details 
 * 
 *
 * This function returns the MUI control block from 
 * a given UE control block based on the MUI ID.
 *
 * @param[in] ueCb   This identifies the UE.
 * @param[in] mui    This identifies the MUI control block.
 *
 * @return 
 *  -# Pointer to MUI control block, if found
 *  -# NULLP - If not found
*/
#ifdef ANSI
PUBLIC MuiInfo* nhDBMGetMuiCb
(
NhActvUeCb *ueCb,
U32        mui
)
#else /* ANSI */
PUBLIC MuiInfo* nhDBMGetMuiCb(ueCb, mui)
NhActvUeCb *ueCb;
U32        mui;
#endif /* ANSI */
{
   MuiInfo        *muiCb=NULLP;
   CmLList        *tmp = NULLP;
   U8             i;

   TRC2(nhDBMGetMuiCb)

#ifndef ALIGN_64BIT   
      RLOG1(L_INFO, "Fetch the Mui control Block: mui [%ld]", mui);
#else /* ALIGN_64BIT */
      RLOG1(L_INFO, "Fetch the Mui control Block: mui [%d]", mui);
#endif /* ALIGN_64BIT */


   if (mui <= nhCb.genCfg.minMuiLstLen)
   {
      /* Default list element */
      CM_LLIST_FIRST_NODE(&(ueCb->muiInfoLst), tmp);
      for (i=1; i <= mui; i++)
      {
         CM_LLIST_NEXT_NODE(&(ueCb->muiInfoLst), tmp);
      }
      if (NULLP != tmp)
      {
         muiCb = (MuiInfo *)(tmp->node);
         /* Check the status to ensure that its really being used */
         if ((NULLP != muiCb) && (muiCb->status != NH_STATUS_USED))
         {
            /* MUI control block does not exist */
            RLOG0(L_ERROR, "Invalid Mui ID.");
            muiCb = NULLP;
         }
      }
      else
         muiCb = NULLP;
   }
   else
   {
      /* Entry is not a part of default list */
   
      /* Traverse the default list */
      CM_LLIST_FIRST_NODE(&(ueCb->muiInfoLst), tmp);
      for (i=1; i <= nhCb.genCfg.minMuiLstLen; i++)
      {
         CM_LLIST_NEXT_NODE(&(ueCb->muiInfoLst), tmp);
      }
   
      while(NULLP != tmp)
      {
         muiCb = (MuiInfo *)(tmp->node);
   
         if (muiCb->mui == mui)
         {
            break;
         }
         /* Find out the next node */
         CM_LLIST_NEXT_NODE(&(ueCb->muiInfoLst), tmp);
      }
   }

   RETVALUE(muiCb);
} /* nhDBMGetMuiCb */

/**
 * @brief Creates a Cell control block/context.
 * 
 * @details 
 * 
 *
 * This function is used by the RRC modules to Create a Cell context
 * with a provided cell ID.
 *
 * @param[in] cellId    This identifies the cell uniquely.
 * @param[out] cellCb   This is a pointer to the Cell control block.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMCreateCellCb
(
U16        cellId,
NhCellCb   **cellCb  
)
#else /* ANSI */
PUBLIC S16 nhDBMCreateCellCb(cellId, cellCb)
U16        cellId;
NhCellCb   **cellCb;   
#endif /* ANSI */
{
   CmLList  *tmp = NULLP;
   S16   ret;

   TRC2(nhDBMCreateCellCb);

   RLOG0(L_DEBUG, "Creating Cell control block.");

   ret = ROK;

   /* Allocate memory for the node */
   NH_ALLOC(NH_REG, NH_POOL, &tmp, sizeof(CmLList));
   if (NULLP == tmp)
   {
      /* Handle memory alloc error */
      RLOG0(L_FATAL, "Failed to allocate memory for CmLList in Cell.");
      RETVALUE(RFAILED);
   }

   /* Allocate memory for NhCellCb structure */
   NH_ALLOC(NH_REG, NH_POOL, cellCb, sizeof(NhCellCb));
   if (NULLP == *cellCb)
   {
      RLOG0(L_FATAL, "Failed to allocate memory for cellCb in Cell.");
      NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
      RETVALUE(RFAILED);
   }

   (*cellCb)->cellId = cellId;
   /* Initialize the UeCb hash list */
   ret = nhHashListInit(&(*cellCb)->actvUeHlCp, NH_UECB_BINS, 
                        NH_UECB_OFFSET, FALSE,
                        CM_HASH_KEYTYPE_DEF, NH_REG, NH_POOL);

   if(ROK != ret)
   {
      NH_FREE(NH_REG, NH_POOL, *cellCb, sizeof(NhCellCb));
      RETVALUE(ret);
   }
   /* Initialize the new UeCb hash list. This is required for ueId change
    * request handling. We will insert the same ueCb in this hashlist so that
    * during the cfgcfm handling we can use the new ueid to get the UeCb. 
    */
   ret = nhHashListInit(&(*cellCb)->newUeHlCp, NH_NEW_UECB_BINS, 
                        sizeof(CmHashListEnt), FALSE,
                        CM_HASH_KEYTYPE_DEF, NH_REG, NH_POOL);

   if(ROK != ret)
   {
      NH_FREE(NH_REG, NH_POOL, *cellCb, sizeof(NhCellCb));
      RETVALUE(ret);
   }

   cmInitTimers((*cellCb)->cfgTimers, (U8)NH_MAX_CFG_TMRS);

   tmp->node = (PTR)(*cellCb);

   nhCb.eNodeBCb.nhCellCbList.numOfCellCb++;
   /* Add the node to the TransInfo list */
   cmLListAdd2Tail(&(nhCb.eNodeBCb.nhCellCbList.cellCbList), tmp);

   RETVALUE(ROK);
} /* nhDBMCreateCellCb */

/**
 * @brief Deletes a Cell control block.
 * 
 * @details 
 * 
 *
 * This function is used by the LTE RRC modules to Delete a Cell context. 
 *
 * @param[in] cellCb    This identifies the Cell control block to be deleted.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMDeleteCellCb
(
NhCellCb   **cellCb  
)
#else /* ANSI */
PUBLIC S16 nhDBMDeleteCellCb(cellCb)
NhCellCb   **cellCb;   
#endif /* ANSI */
{
   CmLList  *tmp = NULLP;
   CmLList  *cellTmp = NULLP;
   CmLListCp   *cellCbList = NULLP;
   CmLListCp   *rbCfgInfoList = NULLP;
   NhActvUeCb *ueCb = NULLP;
   NhLLyrCfgTmrCb    *tmrCb = NULLP;
   U8                tmrNum;
   CmTmrArg          arg;
   S16 ret;
   U8                i;

   TRC2(nhDBMDeleteCellCb);

   RLOG0(L_DEBUG, "Deleting Cell control Block.");
   
   /* Delete the CellCb */

   /* Free memory allocated for cell configuration */
   if (nhCb.eNodeBCb.nhCellCbList.numOfCellCb)
   {
      cellCbList = &(nhCb.eNodeBCb.nhCellCbList.cellCbList);
      i = 1;
      CM_LLIST_FIRST_NODE(cellCbList, cellTmp);
      while (NULLP != cellTmp)
      {
         if((((NhCellCb *)cellTmp->node)->cellId) == (*cellCb)->cellId)
         {
            cmLListDelFrm(cellCbList, cellTmp);
            break;
         }
         i++;
         CM_LLIST_NEXT_NODE(cellCbList, cellTmp);
      }
      /* cell not found in list so return */
      if (i > nhCb.eNodeBCb.nhCellCbList.numOfCellCb)
         RETVALUE(RFAILED);
   }
   else
      RETVALUE(RFAILED);

   /* Free memory allocated for cell configuration */
   if ((*cellCb)->crntCellCfgInfo.rbCfgList.numOfRbCfgInfo)
   {
      rbCfgInfoList = &((*cellCb)->crntCellCfgInfo.rbCfgList.rbCfgInfoList);

      CM_LLIST_FIRST_NODE(rbCfgInfoList, tmp);
      while (NULLP != tmp)
      {
         cmLListDelFrm(rbCfgInfoList, tmp);
         NH_FREE(NH_REG, NH_POOL, 
                 tmp->node, sizeof(NhuRbCfgInfo));
         NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
         CM_LLIST_FIRST_NODE(rbCfgInfoList, tmp);
      }
   }

   /* Delete Ordered Configuration */
   if(NULLP != (*cellCb)->cellTransCb.cellOrdrdCfg)
   {
      if((*cellCb)->cellTransCb.cellOrdrdCfg->cellCfgInfo.rbToAddCfgList.numOfRbCfgInfo)
      {
         NH_FREE(NH_REG, NH_POOL,
               (*cellCb)->cellTransCb.cellOrdrdCfg->cellCfgInfo.rbToAddCfgList.rbCfgInfoList, 
               (*cellCb)->cellTransCb.cellOrdrdCfg->cellCfgInfo.rbToAddCfgList.numOfRbCfgInfo *
               sizeof(NhuRbCfgInfo));
      }
      NH_FREE(NH_REG, NH_POOL, (*cellCb)->cellTransCb.cellOrdrdCfg, 
               sizeof(NhuCellCfgInfo));
   }

   /* stop any running timers before buffer release. */
   for (tmrNum = 0; tmrNum < NH_MAX_CFG_TMRS; tmrNum++)
   {
      if ((*cellCb)->cfgTimers[tmrNum].tmrEvnt != TMR_NONE)
      {
         tmrCb = (NhLLyrCfgTmrCb *)((*cellCb)->cfgTimers[tmrNum].cb);
         /* fill the argument */
         arg.tq     = nhCb.nhLLyrCfgTq;
         arg.tqCp   = &(nhCb.nhLLyrCfgTqCp);
         arg.timers = (*cellCb)->cfgTimers;
         arg.max    = NH_MAX_CFG_TMRS;
         arg.cb     = (PTR)tmrCb;
         arg.evnt   = NH_TMR_UE_LLYR_CFGCFM;
         arg.wait   = NOTUSED;
         arg.tNum   = tmrNum;
         /* Stop the Lower layer Cfg timer */
         cmRmvCbTq(&arg);
         /* Deallocate the lower layer config control block */
         NH_FREE(NH_REG, NH_POOL, tmrCb, sizeof(NhLLyrCfgTmrCb));
      }
   }

   /* Delete UEs in newUeHlCp */
   for(;;)
   {
      ret = nhHashListGetNext(&(*cellCb)->newUeHlCp, NULLP,
                  (PTR *) &ueCb);
      if ((ret != ROK) || (NULLP == (NhActvUeCb *)ueCb))
      {
         break;
      }  
      else
      {
         nhHashListDelete(&(*cellCb)->newUeHlCp, (PTR)ueCb);
      }
   }/* End of for(;;) */
   /* Delete UEs in actvUeHlCp */
   for(;;)
   {
      ret = nhHashListGetNext(&(*cellCb)->actvUeHlCp, NULLP,
                  (PTR *) &ueCb);
      if ((ret != ROK) || (NULLP == (NhActvUeCb *)ueCb))
      {
         break;
      }  
      else
      {
         nhHashListDelete(&(*cellCb)->actvUeHlCp, (PTR)ueCb);
         /* Delete UeCb */
         ret = nhDBMDeleteUeCb(*cellCb, &ueCb);
         /* 
          * Not checking the ret of the above function since it always 
          * returns ROK. If in future any changes are done in this function 
          * then return value validation might be required.
          */
      }
   }/* End of for(;;) */



   /* De-Init uecb Hash list */
   nhHashListDeInit(&((*cellCb)->actvUeHlCp));
   nhHashListDeInit(&((*cellCb)->newUeHlCp));

   /* Free memory allocated to cellCb */
   NH_FREE(NH_REG, NH_POOL, *cellCb, sizeof(NhCellCb));
   NH_FREE(NH_REG, NH_POOL, cellTmp, sizeof(CmLList));
   nhCb.eNodeBCb.nhCellCbList.numOfCellCb--;

   RETVALUE(ROK);
} /* nhDBMDeleteCellCb */

/**
 * @brief Commit Ordered Configuration for a Cell.
 * 
 * @details 
 * 
 *
 * This function is used by LTE RRC modules to commit the ordered configuration onto
 * Current configuration for a Cell for action type add, after getting configuraton confirm 
 * from the lower layers. As a result, the ordered configuration gets deleted.
 *
 * @param[in] cellCb    This is a pointer to the Cell control block.
 *
 * @return S16
 *  -# ROK
 *  -# ROUTRES
*/
#ifdef ANSI
PRIVATE S16 nhDBMCommitCellOrdrdCfgAdd
(
NhCellCb   *cellCb  
)
#else /* ANSI */
PRIVATE S16 nhDBMCommitCellOrdrdCfgAdd(cellCb)
NhCellCb   *cellCb;   
#endif /* ANSI */
{
   NhCellOrdrdCfg *ordrdCfgInfo = NULLP;
   NhCellCfgInfo  *crntCfgInfo = NULLP;
   CmLList        *tmp = NULLP; /* Remove */
   NhuRbCfgInfo   *rbCfgInfo = NULLP; /* Remove */
   S16            i;

   TRC2(nhDBMCommitCellOrdrdCfgAdd);

   ordrdCfgInfo = cellCb->cellTransCb.cellOrdrdCfg;
   crntCfgInfo = &(cellCb->crntCellCfgInfo);
   tmp = NULLP;
   rbCfgInfo = NULLP;

   /* Fill the general elements */
   crntCfgInfo->cfgBitMsk = ordrdCfgInfo->cellCfgInfo.cellCfgBitMsk;
   /* Fill the duplex mode  -FFD/TDD */
   crntCfgInfo->duplexMode = ordrdCfgInfo->cellCfgInfo.duplexMode;

   /* Copy the RB configuration list */
   if (ordrdCfgInfo->cellCfgInfo.cellCfgBitMsk & NHU_CELL_CFG_RB)
   {
      crntCfgInfo->rbCfgList.numOfRbCfgInfo = 
         ordrdCfgInfo->cellCfgInfo.rbToAddCfgList.numOfRbCfgInfo;

      for(i=0; i<ordrdCfgInfo->cellCfgInfo.rbToAddCfgList.numOfRbCfgInfo; i++)
      {
         /* Allocate memory for the node */
         NH_ALLOC(NH_REG, NH_POOL, &tmp, sizeof(CmLList));
         if (NULLP == tmp)
         {
            /* Handle memory alloc error */
            RLOG0(L_FATAL, "Failed to allocate memory for CmLList in Cell.");
            NH_FREE(NH_REG, NH_POOL,
                  cellCb->cellTransCb.cellOrdrdCfg, sizeof(NhuCellCfgInfo));
            RETVALUE(ROUTRES);

         }
      /* Allocate memory for NhuRbCfgInfo structure */
         NH_ALLOC(NH_REG, NH_POOL, &rbCfgInfo, sizeof(NhuRbCfgInfo));
         if (NULLP == rbCfgInfo)
         {
            RLOG0(L_FATAL, "Failed to allocate memory for RbCfg in Cell.");
            NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
            NH_FREE(NH_REG, NH_POOL,
                  cellCb->cellTransCb.cellOrdrdCfg, sizeof(NhuCellCfgInfo));
            RETVALUE(ROUTRES);
         }
         NH_MEM_COPY(rbCfgInfo, 
          &(ordrdCfgInfo->cellCfgInfo.rbToAddCfgList.rbCfgInfoList[i]),
               sizeof(NhuRbCfgInfo));

         tmp->node = (PTR)(rbCfgInfo);
         /* Add the node to the TransInfo list */
         cmLListAdd2Tail(&(crntCfgInfo->rbCfgList.rbCfgInfoList), tmp);
      }  
   }

   /* Copy rest of the items */
   if (ordrdCfgInfo->cellCfgInfo.cellCfgBitMsk & NHU_CELL_CFG_MAC_CMN)
   {
      NH_MEM_COPY(&(crntCfgInfo->macCfg.rachCfg), 
         &(ordrdCfgInfo->cellCfgInfo.macCmnCfg.rachCfg),
            sizeof(NhuRachCfg));
   }

#ifdef RRC_PHY_CFG_SUPP
   if (ordrdCfgInfo->cellCfgInfo.cellCfgBitMsk & NHU_CELL_PHY_CFG)
   {
      NH_MEM_COPY(&(crntCfgInfo->phyCfg), &(ordrdCfgInfo->cellCfgInfo.phyCellCfg), 
            sizeof(NhuCmnPhyCfg));
   }
#endif /* RRC_PHY_CFG_SUPP */

   /* free up the memory allocated for cellOrdrdCfg in cellTransCb */
   if(ordrdCfgInfo->cellCfgInfo.rbToAddCfgList.numOfRbCfgInfo)
   {
      NH_FREE(NH_REG, NH_POOL, 
            ordrdCfgInfo->cellCfgInfo.rbToAddCfgList.rbCfgInfoList,
            (sizeof(NhuRbCfgInfo)*(ordrdCfgInfo->cellCfgInfo.rbToAddCfgList.numOfRbCfgInfo)));
      ordrdCfgInfo->cellCfgInfo.rbToAddCfgList.numOfRbCfgInfo = 0;
   }

   NH_FREE(NH_REG, NH_POOL,
         cellCb->cellTransCb.cellOrdrdCfg, sizeof(NhCellOrdrdCfg));
   RETVALUE(ROK);
}/* End of nhDBMCommitCellOrdrdCfgAdd */


/**
 * @brief Commit Ordered Configuration for a Cell.
 * 
 * @details 
 * 
 *
 * This function is used by LTE RRC modules to commit the ordered configuration onto
 * Current configuration for a Cell, after getting configuraton confirm 
 * from the lower layers. As a result, the ordered configuration gets deleted.
 *
 * @param[in] cellCb    This is a pointer to the Cell control block.
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMCommitCellOrdrdCfg
(
NhCellCb   *cellCb  
)
#else /* ANSI */
PUBLIC S16 nhDBMCommitCellOrdrdCfg(cellCb)
NhCellCb   *cellCb;   
#endif /* ANSI */
{
   NhCellOrdrdCfg *ordrdCfgInfo = NULLP;
   NhCellCfgInfo  *crntCfgInfo = NULLP;
   S16            ret;

   TRC2(nhDBMCommitCellOrdrdCfg);

   ordrdCfgInfo = cellCb->cellTransCb.cellOrdrdCfg;
   crntCfgInfo = &(cellCb->crntCellCfgInfo);
   ret = ROK;

   switch(cellCb->cellTransCb.cfgAction)
   {
      case NH_CFG_ACTN_ADD:
         ret = nhDBMCommitCellOrdrdCfgAdd(cellCb);
         if(ROK != ret)
         {
             RETVALUE(RFAILED);
         }
         else
         {
             RETVALUE(ROK);
         }
         break;
      case NH_CFG_ACTN_MODIFY:
         {
            /* Overwrite the old info with the ordered info */
            if (ordrdCfgInfo->cellCfgInfo.cellCfgBitMsk & NHU_CELL_CFG_MAC_CMN)
            {
               NH_MEM_COPY(&(crntCfgInfo->macCfg.rachCfg), &(ordrdCfgInfo->cellCfgInfo.macCmnCfg.rachCfg),
                     sizeof(NhuRachCfg));
            }

#ifdef RRC_PHY_CFG_SUPP
            if (ordrdCfgInfo->cellCfgInfo.cellCfgBitMsk & NHU_CELL_PHY_CFG)
            {
               if (ordrdCfgInfo->cellCfgInfo.phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PRACH)
               {
                  NH_MEM_COPY(&(crntCfgInfo->phyCfg.prachCfg), 
                              &(ordrdCfgInfo->cellCfgInfo.phyCellCfg.prachCfg),
                              sizeof(NhuPrachCfg));
               }

               if (ordrdCfgInfo->cellCfgInfo.phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PDSCH)
               {
                  NH_MEM_COPY(&(crntCfgInfo->phyCfg.cmnPdschCfg), 
                              &(ordrdCfgInfo->cellCfgInfo.phyCellCfg.cmnPdschCfg),
                              sizeof(NhuCmnPdschCfg));
               }

               if (ordrdCfgInfo->cellCfgInfo.phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PUSCH)
               {
                  NH_MEM_COPY(&(crntCfgInfo->phyCfg.cmnPuschCfg), 
                              &(ordrdCfgInfo->cellCfgInfo.phyCellCfg.cmnPuschCfg),
                              sizeof(NhuCmnPuschCfg));
               }

               if (ordrdCfgInfo->cellCfgInfo.phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PHICH)
               {
                  NH_MEM_COPY(&(crntCfgInfo->phyCfg.cmnPhichCfg), 
                              &(ordrdCfgInfo->cellCfgInfo.phyCellCfg.cmnPhichCfg),
                              sizeof(NhuPhichCfg));
               }

               if (ordrdCfgInfo->cellCfgInfo.phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PUCCH)
               {
                  NH_MEM_COPY(&(crntCfgInfo->phyCfg.cmnPucchCfg), 
                              &(ordrdCfgInfo->cellCfgInfo.phyCellCfg.cmnPucchCfg),
                              sizeof(NhuPucchCfg));
               }

               if (ordrdCfgInfo->cellCfgInfo.phyCellCfg.bitmask & NHU_CMN_PHY_CFG_UL_SRS)
               {
                  NH_MEM_COPY(&(crntCfgInfo->phyCfg.srsUlCfg), 
                              &(ordrdCfgInfo->cellCfgInfo.phyCellCfg.srsUlCfg),
                              sizeof(NhuCmnSrsUlCfg));
               }

               if (ordrdCfgInfo->cellCfgInfo.phyCellCfg.bitmask & NHU_CMN_PHY_CFG_TDD_SF)
               {
                  NH_MEM_COPY(&(crntCfgInfo->phyCfg.tddSfCfg), 
                              &(ordrdCfgInfo->cellCfgInfo.phyCellCfg.tddSfCfg),
                              sizeof(NhuTddSfCfgInfo));
               }
            }
#endif /* RRC_PHY_CFG_SUPP */

         /* Free up the memory allocated for the cellOrdrdCfg in cellTransCb */
            NH_FREE(NH_REG, NH_POOL,
                  cellCb->cellTransCb.cellOrdrdCfg, sizeof(NhuCellCfgInfo));
         }
         break;
      case NH_CFG_ACTN_RLS:
         {
            NH_FREE(NH_REG, NH_POOL,
                  cellCb->cellTransCb.cellOrdrdCfg, sizeof(NhuCellCfgInfo));
         }
         break;
      default:
         {
            /* Must never enter here */
            RLOG0(L_ERROR, "Unexpected config action.");
            RETVALUE(RFAILED);
         }
   } /* switch(cellCb->cellTransCb.cfgAction) */

   RETVALUE(ROK);
} /* nhDBMCommitCellOrdrdCfg */


/**
 * @brief Fills the mapping of RB to logCh and PDCP.
 * 
 * @details 
 * 
 *
 * This function updates the PDCP configuration based on the configuration action.
 *
 *
 *
 * @param[in] dstCfgInfo      Radio Bearer, PDCP, Logical Channel Map
 * @param[in] srcRbCfgInfo    Raio Bearer Configuration
 * @param[in] action          Action: Add or Modify
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PRIVATE S16 nhDBMFillLogchPdcpInfo
(
NhRbToLchMap   *dstCfgInfo,
NhuRbCfgInfo   *srcRbCfgInfo,
U8             action
)
#else /* ANSI */
PRIVATE S16 nhDBMFillLogchPdcpInfo(dstCfgInfo, srcRbCfgInfo, action)
NhRbToLchMap   *dstCfgInfo;
NhuRbCfgInfo   *srcRbCfgInfo;
U8             action;
#endif /* ANSI */
{
   TRC2(nhDBMFillLogchPdcpInfo)

   switch (action)
   {
      case NH_CFG_ACTN_ADD:
      {
         dstCfgInfo->rbId.rbId = srcRbCfgInfo->rbId.rbId;
         dstCfgInfo->rbId.rbTyp = srcRbCfgInfo->rbId.rbTyp;
         NH_MEM_COPY(&(dstCfgInfo->lchId),
                     &(srcRbCfgInfo->lcId),
                     sizeof(NhuLogChId));
         if(srcRbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL)
         {
            if(srcRbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)
            {
               dstCfgInfo->logChCfg.lChCfgBitMsk |= NH_LOGCH_CFG_LCH;
               NH_MEM_COPY(&(dstCfgInfo->logChCfg.logChCfg),
                           &(srcRbCfgInfo->ulRbCfg.ulLogChMap.lcCfg),
                           sizeof(NhuLogChCfg));
               dstCfgInfo->logChCfg.lChCfgBitMsk |= NH_LOGCH_CFG_UL_TRCHTYP;
               dstCfgInfo->logChCfg.ulTrchType =
                  srcRbCfgInfo->ulRbCfg.ulLogChMap.trChType;
               dstCfgInfo->dir = NH_LCID_DIR_RX;

            }
         }
         if(srcRbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL)
         {
            if(srcRbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_LOGCH_MAP_DL)
            {
               dstCfgInfo->logChCfg.lChCfgBitMsk |= NH_LOGCH_CFG_DL_TRCHTYP;
               dstCfgInfo->logChCfg.dlTrchType =
                  srcRbCfgInfo->dlRbCfg.dlLogChMap.trChType;

               if(dstCfgInfo->dir == NH_LCID_DIR_RX)
                  dstCfgInfo->dir = NH_LCID_DIR_TXRX;
               else
                  dstCfgInfo->dir = NH_LCID_DIR_TX;

            }
         }
         if (srcRbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
            dstCfgInfo->isPdcpConfigured = TRUE;
      }
      break;
      case NH_CFG_ACTN_MODIFY:
      {
         if(srcRbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL)
         {
            if(srcRbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)
            {
               dstCfgInfo->logChCfg.lChCfgBitMsk |= NH_LOGCH_CFG_LCH;
               dstCfgInfo->logChCfg.logChCfg.logChGrp = 
                           srcRbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChGrp;
            }
         }
      }
      break;
      default:
      break;
   }

   RETVALUE(ROK);
} /* nhDBMFillLogchPdcpInfo */

/**
 * @brief Updates current configuration for an UE with RB, Logch, and PDCP mapping.
 * 
 * @details 
 *
 * This function commits the ordered configuration onto
 * the current configuration for a RbId, stores LogChCfg for UL/DL, and PdcpCfg 
 * after getting configuraton confirm from the lower layers.  
 *
 * @param[out] crntCfg  Current configuration
 * @param[in] rbCfgInfo RB configuration info to be used for filling
 * @param[in] action    Configuration action
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PRIVATE S16 nhDBMUpdRbToLchOrPdcp
(
NhCrntCfgInfo *crntCfg,                
NhuRbCfgInfo  *rbCfgInfo,
U8            action
)
#else /* ANSI */
PRIVATE S16 nhDBMUpdRbToLchOrPdcp(crntCfg, rbCfgInfo, action)
NhCrntCfgInfo *crntCfg;                
NhuRbCfgInfo  *rbCfgInfo;
U8            action;
#endif /* ANSI */
{
   CmLList              *tmpNode =  NULLP;
   NhRbToLchMap         *tmpCfgInfo = NULLP;
   U8                   rbId;
   U8                   rbTyp;

   TRC2(nhDBMUpdRbToLchOrPdcp)

   switch(action)
   {
      case NH_CFG_ACTN_ADD:
      {
         crntCfg->bitMskPres |= NH_CRNT_CFG_LOGCHCB;
         NH_ALLOC(NH_REG, NH_POOL, &tmpNode, sizeof(CmLList));
         if(NULLP == tmpNode)
         {
            RLOG0(L_FATAL, "Failed to allocate memory for adding radio bearer");
            RETVALUE(ROUTRES);
         }
         NH_ALLOC(NH_REG, NH_POOL, &tmpCfgInfo, sizeof(NhRbToLchMap));
         if(NULLP == tmpCfgInfo)
         {
             RLOG0(L_FATAL, "Failed to allocate memory for adding Logical "
                "ChannelCfg.");
             NH_FREE(NH_REG, NH_POOL, tmpNode, sizeof(CmLList));
             RETVALUE(ROUTRES);
         }
         /* Fill the parameter in NnhRbToLchOrPdcpMap */ 
         nhDBMFillLogchPdcpInfo(tmpCfgInfo,rbCfgInfo, action);
         tmpNode->node = (PTR) tmpCfgInfo;

         cmLListAdd2Tail(&crntCfg->logChCbLst, tmpNode);
       }         
         break;

      case NH_CFG_ACTN_MODIFY:
      {
         rbId = rbCfgInfo->rbId.rbId;
         rbTyp = rbCfgInfo->rbId.rbTyp;
         CM_LLIST_FIRST_NODE(&crntCfg->logChCbLst, tmpNode);

         while(NULLP != tmpNode)
         {
            if(((NhRbToLchMap *) tmpNode->node)->rbId.rbId == rbId &&
              ((NhRbToLchMap *) tmpNode->node)->rbId.rbTyp == rbTyp )
            {
               tmpCfgInfo = (NhRbToLchMap *)tmpNode->node;
               /* Update the informations in logChCbLst */ 
               nhDBMFillLogchPdcpInfo(tmpCfgInfo,rbCfgInfo, action);
               break;
            }
            /* ccpu00130981 - Fix */
            CM_LLIST_NEXT_NODE(&crntCfg->logChCbLst, tmpNode);
         }
         
      }
      break;
      case NH_CFG_ACTN_RLS:
      {
         CM_LLIST_FIRST_NODE(&crntCfg->logChCbLst, tmpNode);

         while(NULLP != tmpNode)
         {
            if((((NhRbToLchMap *)tmpNode->node)->rbId.rbId) 
                                             == rbCfgInfo->rbId.rbId &&
               (((NhRbToLchMap *)tmpNode->node)->rbId.rbTyp) 
                                             == rbCfgInfo->rbId.rbTyp)
            {
               cmLListDelFrm(&crntCfg->logChCbLst, tmpNode);
               NH_FREE(NH_REG, NH_POOL, tmpNode->node,
                                      sizeof(NhRbToLchMap));
               NH_FREE(NH_REG, NH_POOL, tmpNode, sizeof(CmLList));
               break;
            }
            /* ccpu00130981 - Fix */
            CM_LLIST_NEXT_NODE(&crntCfg->logChCbLst, tmpNode);
         }

      }
      break;
      default :
      break;
   }

   RETVALUE(ROK);
} /* nhDBMUpdRbToLchOrPdcp */

/**
 * @brief Updates Modified RB Configuration for an UE.
 * 
 * @details 
 * 
 * This function updates the rbCfg in present in the crnt cfg from the ordered
 * cfg during Rb Modify case.
 *
 * @param[in] dstRbCfgInfo   Destination where RB information needs to be updated
 * @param[in] srcRbCfgInfo   Source containing RB information to be used to update
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMUpdModifiedRb
(
NhuRbCfgInfo *dstRbCfgInfo,
NhuRbCfgInfo *srcRbCfgInfo
)
#else /* ANSI */
PUBLIC S16 nhDBMUpdModifiedRb(dstRbCfgInfo, srcRbCfgInfo)
NhuRbCfgInfo *dstRbCfgInfo;
NhuRbCfgInfo *srcRbCfgInfo;
#endif /* ANSI */
{
   /* nh004.201 - Correction to TRC functionality. */
   TRC2(nhDBMUpdModifiedRb)

   if(srcRbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL)
   {
      nhDBMUpdModifiedRbCfgUl(dstRbCfgInfo, srcRbCfgInfo);
   }
   if(srcRbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL)
   {
      nhDBMUpdModifiedRbCfgDl(dstRbCfgInfo, srcRbCfgInfo);
   }
   if(srcRbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
   {

      RETVALUE(nhDBMUpdModifiedRbPdcpInfo(dstRbCfgInfo, srcRbCfgInfo));
   }

   RETVALUE(ROK);

}/* end of nhDBMUpdModifiedRb */
/**
 * @brief Commits Ordered Config  for a UE.
 * 
 * @details 
 * 
 * This function commits the ordered configuration for TrchCfg, SchedCfg, and
 * NewCrnti Cfg.
 * from Lower layers. As a result, ordered configuration gets deleted.
 *
 * @param[in] ueCb      UE Context
 * @param[in] transCtrlBlk   UE Transaction Context
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PRIVATE S16 nhDBMCommitUeCfg
(
NhActvUeCb *ueCb,
TransCb  *transCtrlBlk
)
#else /* ANSI */
PRIVATE S16 nhDBMCommitUeCfg(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;
TransCb  *transCtrlBlk;
#endif /* ANSI */
{
   NhCrntCfgInfo     *crntCfgInfo = NULLP;
   NhOrdrdCfgInfo    *ordCfgInfo = NULLP;
   S16               retVal = ROK;
   NhCellCb          *cellCb = NULLP;

   TRC2(nhDBMCommitUeCfg)

   crntCfgInfo = (NhCrntCfgInfo *)&ueCb->crntCfg;
   ordCfgInfo  = (NhOrdrdCfgInfo *)&transCtrlBlk->ordrdCfg;

   if(ordCfgInfo->actionMask & NH_ORD_CFG_TRCH)
   {
      crntCfgInfo->bitMskPres |= NH_CRNT_CFG_TRCH;
      crntCfgInfo->trChCfg.trChBitMsk = ordCfgInfo->trChCfg.trChBitMsk;

      if(ordCfgInfo->trChCfg.trChBitMsk & NHU_TRCH_CFG_UL_HARQ)
      {
         NH_MEM_COPY(&crntCfgInfo->trChCfg.ulHarqCfg,
                     &ordCfgInfo->trChCfg.ulHarqCfg,
                     sizeof(NhuUlHarqCfg));
      }
   }

   if(ordCfgInfo->actionMask & NH_ORD_CFG_SEC)
   {
      crntCfgInfo->bitMskPres |= NH_CRNT_CFG_SEC;
      crntCfgInfo->secCfg.secCfgBitMsk = ordCfgInfo->secCfg.secCfgBitMsk;

      NH_MEM_COPY(&crntCfgInfo->secCfg.intgCfgInfo,
                  &ordCfgInfo->secCfg.intgCfgInfo,
                  sizeof(NhuIntgKeyAlgoInfo));
      NH_MEM_COPY(&crntCfgInfo->secCfg.cipherCfgInfo,
                  &ordCfgInfo->secCfg.cipherCfgInfo,
                  sizeof(NhuCiphKeyAlgoInfo));
   }

   if(ordCfgInfo->actionMask & NH_ORD_CFG_RESTB_SRB1)
   {
      crntCfgInfo->bitMskPres |= NH_CRNT_CFG_SEC;
      crntCfgInfo->secCfg.secCfgBitMsk = ordCfgInfo->secCfg.secCfgBitMsk;

      if(ordCfgInfo->secCfg.secCfgBitMsk & NHU_SEC_INTG_ALGO)
      {
         NH_MEM_COPY(&crntCfgInfo->secCfg.intgCfgInfo.intgKey, 
                     &ordCfgInfo->secCfg.intgCfgInfo.intgKey,
                     NHU_INTG_KEY_LEN * sizeof(U8));
      }

      if(ordCfgInfo->secCfg.secCfgBitMsk & NHU_SEC_CIPHER_ALGO)
      {
         NH_MEM_COPY(&crntCfgInfo->secCfg.cipherCfgInfo.cipherCpKey, 
                     &ordCfgInfo->secCfg.cipherCfgInfo.cipherCpKey,
                     NHU_CIPH_KEY_LEN * sizeof(U8));

         NH_MEM_COPY(&crntCfgInfo->secCfg.cipherCfgInfo.cipherUpKey, 
                     &ordCfgInfo->secCfg.cipherCfgInfo.cipherUpKey,
                     NHU_CIPH_KEY_LEN * sizeof(U8));
      }
   }

   if(ordCfgInfo->actionMask & NH_ORD_CFG_UEID)
   {
      if(NH_UE_INFO_MOD_CRNTI == ordCfgInfo->ueInfo.crntiCfgType)
      {
         cellCb = ueCb->cellCb;

         retVal = nhHashListDelete(&(cellCb->newUeHlCp), (PTR)ueCb);
         if (ROK != retVal)
         {
            RLOG0(L_ERROR, "Could not delete from hash list.");
         }

         retVal = nhHashListDelete(&(cellCb->actvUeHlCp), (PTR)ueCb);
         if (ROK != retVal)
         {
            RLOG0(L_ERROR, "Could not delete from hash list.");
         }
         ueCb->ueCbKey.crnti = ordCfgInfo->ueInfo.u.modCrnti;

         /* Insert the UE Control Block in the ueCb Hash List 
            with new CRNTI */
         retVal = nhHashListInsert (&(cellCb->actvUeHlCp), (PTR)ueCb,
               (U8 *)&(ueCb->ueCbKey), sizeof (NhUeCbKey));
         if (ROK != retVal)
         {
            RLOG0(L_ERROR, "Could not insert from hash list.");
            RETVALUE(retVal);
         }
      }
   }

#ifdef RRC_PHY_CFG_SUPP
   if(ordCfgInfo->actionMask & NH_ORD_CFG_PHYCH)
   {
      crntCfgInfo->bitMskPres |= NH_CRNT_CFG_PHYCH;
      NH_MEM_COPY(&crntCfgInfo->phyCfg, &ordCfgInfo->phyCfg,
                  sizeof(NhuPhyCfg));
   }
#endif /* RRC_PHY_CFG_SUPP */

   RETVALUE(ROK);
} /* nhDBMCommitUeCfg */

/**
 * @brief Commits RB release for a UE.
 * 
 * @details 
 * 
 * This function updates the Released Rb Cfg from Lower layers. 
 *
 * @param[in] ordCfgInfo  Ordered Config
 * @param[in] crntCfgInfo Current Config Info
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PRIVATE S16 nhDBMCommitRbRlsCfg
(
NhOrdrdCfgInfo    *ordCfgInfo,
NhCrntCfgInfo     *crntCfgInfo
)
#else /* ANSI */
PRIVATE S16 nhDBMCommitRbRlsCfg(ordCfgInfo, crntCfgInfo)
NhOrdrdCfgInfo    *ordCfgInfo;
NhCrntCfgInfo     *crntCfgInfo;
#endif /* ANSI */
{
   CmLList           *tmpNode =  NULLP;
   S16               i;

   /* nh004.201 - Correction to TRC functionality. */
   TRC2(nhDBMCommitRbRlsCfg)

   for(i=0; i < ordCfgInfo->rbToRlsCfgList.numOfRbCfgInfo; i++)
   {
      CM_LLIST_FIRST_NODE(&crntCfgInfo->rbCfgList.rbCfgInfoList, tmpNode);

      while(NULLP != tmpNode)
      {
         if((((NhuRbCfgInfo *) tmpNode->node)->rbId.rbId) 
                   == ordCfgInfo->rbToRlsCfgList.rbsToRel[i].rbId 
           && (((NhuRbCfgInfo *) tmpNode->node)->rbId.rbTyp 
                   == ordCfgInfo->rbToRlsCfgList.rbsToRel[i].rbTyp))
         {
            cmLListDelFrm(&crntCfgInfo->rbCfgList.rbCfgInfoList, tmpNode);
            crntCfgInfo->rbCfgList.numOfRbCfgInfo--;
            nhDBMUpdRbToLchOrPdcp(crntCfgInfo, 
                                  ((NhuRbCfgInfo *) tmpNode->node),
                                   NH_CFG_ACTN_RLS);
            NH_FREE(NH_REG, NH_POOL, tmpNode->node, sizeof(NhuRbCfgInfo));
            NH_FREE(NH_REG, NH_POOL, tmpNode, sizeof(CmLList));
            break;
         }
         CM_LLIST_NEXT_NODE(&crntCfgInfo->rbCfgList.rbCfgInfoList, tmpNode);
      }
   }
   if(crntCfgInfo->rbCfgList.numOfRbCfgInfo == NH_ZERO_VAL)
   {
      crntCfgInfo->bitMskPres &= ~NH_CRNT_CFG_RB;
   }

#ifdef LTERRC_REL9
   /* R9 Upgrade:NH_NW_90: MobilityFromEutra procedure is initiated only when 
    * AS-security has been activated, and SRB2 with at least one DRB are setup
    * and not suspended. Reset the RB mask if SRB2 or DRBs released or not 
    *established yet
    */
   if ((crntCfgInfo->rbCfgList.numOfRbCfgInfo) < NH_RB_ID_SRB2)
   {
      /*cellAndUeCb->ueCb->rbSecCfgBitMask &= ~NH_CRNT_CFG_SRB2;
      cellAndUeCb->ueCb->rbSecCfgBitMask &= ~NH_CRNT_CFG_DRBS;*/
      crntCfgInfo->bitMskPres &= ~NH_CRNT_CFG_SRB2;
      crntCfgInfo->bitMskPres &= ~NH_CRNT_CFG_DRBS;
   }
   else if ((crntCfgInfo->rbCfgList.numOfRbCfgInfo) == NH_RB_ID_SRB2)
   {
      /*cellAndUeCb->ueCb->rbSecCfgBitMask &= ~NH_CRNT_CFG_DRBS;*/
      crntCfgInfo->bitMskPres &= ~NH_CRNT_CFG_DRBS;
   }
#endif /*LTERRC_REL9*/

   RETVALUE(ROK); 
} /* nhDBMCommitRbRlsCfg */

/**
 * @brief Updates the Modified Rb Config  for a UE.
 * 
 * @details 
 * 
 * This function commits the ordered configuration given for a modified Rb.
 *
 * @param[in] ordCfgInfo  Orered Config Info
 * @param[in] crntCfgInfo Current Config Info
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PRIVATE S16 nhDBMCommitRbModCfg
(
NhOrdrdCfgInfo    *ordCfgInfo,
NhCrntCfgInfo     *crntCfgInfo
)
#else /* ANSI */
PRIVATE S16 nhDBMCommitRbModCfg(ordCfgInfo, crntCfgInfo)
NhOrdrdCfgInfo    *ordCfgInfo;
NhCrntCfgInfo     *crntCfgInfo;
#endif /* ANSI */
{
   CmLList           *tmpNode =  NULLP;
   NhuRbCfgInfo      *srcRbCfgInfo = NULLP;
   NhuRbCfgInfo      *dstRbCfgInfo = NULLP;
   S16               i;
   S16               retVal;

   /* nh004.201 - Correction to TRC functionality. */   
   TRC2(nhDBMCommitRbModCfg)

   for(i=0; i < ordCfgInfo->rbToModCfgList.numOfRbCfgInfo; i++)
   {
      CM_LLIST_FIRST_NODE(&crntCfgInfo->rbCfgList.rbCfgInfoList, tmpNode);

      while(NULLP != tmpNode)
      {
         if(((NhuRbCfgInfo *) tmpNode->node)->rbId.rbId 
                   == ordCfgInfo->rbToModCfgList.rbCfgInfoList[i].rbId.rbId 
           && (((NhuRbCfgInfo *) tmpNode->node)->rbId.rbTyp == ordCfgInfo->rbToModCfgList.rbCfgInfoList[i].rbId.rbTyp))
         {
   /* HO_T2200: FIX */
/*            NH_MEM_COPY((U8*) tmpNode->node, 
                        (U8*) &ordCfgInfo->rbToModCfgList.rbCfgInfoList[i],
                        sizeof(NhuRbCfgInfo)); */
   /* HO_T2200: END*/
            dstRbCfgInfo =  (NhuRbCfgInfo *)tmpNode->node;
            srcRbCfgInfo = &ordCfgInfo->rbToModCfgList.rbCfgInfoList[i];
            
            retVal = nhDBMUpdModifiedRb(dstRbCfgInfo, srcRbCfgInfo);
            if(ROK != retVal)
            {
               RLOG0(L_ERROR, "nhDBMUpdModifiedRb failed");
               RETVALUE(RFAILED);
            }
            nhDBMUpdRbToLchOrPdcp(crntCfgInfo,
                            &ordCfgInfo->rbToModCfgList.rbCfgInfoList[i], 
                            NH_CFG_ACTN_MODIFY);
            break;
         }
         CM_LLIST_NEXT_NODE(&crntCfgInfo->rbCfgList.rbCfgInfoList, tmpNode);
      } /* while */
   }/* end of for loop */

   RETVALUE(ROK);
} /* nhDBMCommitRbModCfg */

/**
 * @brief Commits Ordered Config  for a UE.
 * 
 * @details 
 * 
 * This function commits the ordered configuration given for added RBs.
 *
 * @param[in] ordCfgInfo  Ordered Config
 * @param[in] crntCfgInfo Current config Info
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PRIVATE S16 nhDBMCommitRbAddCfg
(
NhOrdrdCfgInfo    *ordCfgInfo,
NhCrntCfgInfo     *crntCfgInfo
)
#else /* ANSI */
PRIVATE S16 nhDBMCommitRbAddCfg(ordCfgInfo, crntCfgInfo)
NhOrdrdCfgInfo    *ordCfgInfo;
NhCrntCfgInfo     *crntCfgInfo;
#endif /* ANSI */
{
   CmLList           *tmpNode =  NULLP;
   NhuRbCfgInfo      *tmpRbCfgInfo = NULLP;
   U8                 i;

   /* nh004.201 - Correction to TRC functionality. */   
   TRC2(nhDBMCommitRbAddCfg)
   
   for(i=0; i < ordCfgInfo->rbToAddCfgList.numOfRbCfgInfo; i++)
   {
      NH_ALLOC(NH_REG, NH_POOL, &tmpNode, sizeof(CmLList));
      if(NULLP == tmpNode)
      {
         RLOG0(L_FATAL, "Failed to allocate memory for adding radio bearer.");
         RETVALUE(ROUTRES);
      }

      NH_ALLOC(NH_REG, NH_POOL, &tmpRbCfgInfo, sizeof(NhuRbCfgInfo));
      if(NULLP == tmpRbCfgInfo)
      {
         RLOG0(L_FATAL, "Failed to allocate memory for adding radio bearer.");
         NH_FREE(NH_REG, NH_POOL, tmpNode, sizeof(CmLList));
         RETVALUE(ROUTRES);
      }

      NH_MEM_COPY((U8*) tmpRbCfgInfo, 
                  (U8*) &ordCfgInfo->rbToAddCfgList.rbCfgInfoList[i],
                  sizeof(NhuRbCfgInfo));

      tmpNode->node = (PTR) tmpRbCfgInfo;

      cmLListAdd2Tail(&crntCfgInfo->rbCfgList.rbCfgInfoList, tmpNode);
      
      nhDBMUpdRbToLchOrPdcp(crntCfgInfo, tmpRbCfgInfo, NH_CFG_ACTN_ADD);

      /* Increment the RB count */
      crntCfgInfo->rbCfgList.numOfRbCfgInfo++;
   }

#ifdef LTERRC_REL9
   /* R9 Upgrade:NH_NW_90: MobilityFromEutra procedure is initiated only when 
    * AS-security has been activated, and SRB2 with at least one DRB are setup
    * and not suspended. Set the RB mask if SRB2 and one DRB is established
    */
   if ((crntCfgInfo->rbCfgList.numOfRbCfgInfo) == NH_RB_ID_SRB2)
   {
      /*cellAndUeCb->ueCb->rbSecCfgBitMask |= NH_CRNT_CFG_SRB2;*/
      crntCfgInfo->bitMskPres |= NH_CRNT_CFG_SRB2;
   }
   else if ((crntCfgInfo->rbCfgList.numOfRbCfgInfo) > NH_RB_ID_SRB2)
   {
     /* cellAndUeCb->ueCb->rbSecCfgBitMask |= NH_CRNT_CFG_DRBS;*/
      crntCfgInfo->bitMskPres |= NH_CRNT_CFG_SRB2;
      crntCfgInfo->bitMskPres |= NH_CRNT_CFG_DRBS;
   }
#endif /*LTERRC_REL9*/

   RETVALUE(ROK);
} /* nhDBMCommitRbAddCfg */

/**
 * @brief Commits Ordered Config  for a UE.
 * 
 * @details 
 * 
 * This function commits the ordered configuration onto
 * from Lower layers. As a result, ordered configuration gets deleted.
 *
 * @param[in] ueCb      UE Context
 * @param[in] transCtrlBlk   UE Transaction Context
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMCommitOrdrdCfg
(
NhActvUeCb *ueCb,                
TransCb  *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhDBMCommitOrdrdCfg(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;                
TransCb  *transCtrlBlk;    
#endif /* ANSI */
{
   NhCrntCfgInfo     *crntCfgInfo = NULLP;
   NhOrdrdCfgInfo    *ordCfgInfo = NULLP;
   S16               retVal = ROK;
 
   TRC2(nhDBMCommitOrdrdCfg)

   RLOG0(L_DEBUG, "nhDBMCommitOrdrdCfg");

   crntCfgInfo = (NhCrntCfgInfo *)&ueCb->crntCfg;
   ordCfgInfo  = (NhOrdrdCfgInfo *)&transCtrlBlk->ordrdCfg;

   if(ordCfgInfo->actionMask & NH_ORD_CFG_RB_ADD)
   {
      crntCfgInfo->bitMskPres |= NH_CRNT_CFG_RB;
      retVal = nhDBMCommitRbAddCfg(ordCfgInfo, crntCfgInfo);
      if (ROK != retVal)
      {
         /* Debug Prints are already there in the calling function */
         RETVALUE(retVal);
      }
   }

   if(ordCfgInfo->actionMask & NH_ORD_CFG_RB_MOD)
   {
      crntCfgInfo->bitMskPres |= NH_CRNT_CFG_RB;
      retVal = nhDBMCommitRbModCfg(ordCfgInfo, crntCfgInfo);
      if(ROK != retVal)
      {
         nhUTLFreeUlBitMapFrmCrntRbCfg(crntCfgInfo); 
         /* Debug Prints are already there in the calling function */
         RETVALUE(retVal);
      }
   }

   if(ordCfgInfo->actionMask & NH_ORD_CFG_RB_RLS)
   {
      retVal = nhDBMCommitRbRlsCfg(ordCfgInfo, crntCfgInfo);

      if (ROK != retVal)
      {
         /* Debug Prints are already there in the calling function */
         RETVALUE(retVal);
      }
   }

   /* return the ROK/RFAILED after nhDBMCommitUeCfg */
   RETVALUE(nhDBMCommitUeCfg(ueCb, transCtrlBlk));

} /* nhDBMCommitOrdrdCfg */



/**
 * @brief Fetches the Cell control block.
 * 
 * @details 
 * 
 *
 * This function finds the cell context based on the cell ID by looking
 * into the hash list.
 *
 * @param[in] cellId Cell identity
 *
 * @return 
 *  -# Pointer to cell context, if found 
 *  -# NULLP, if not found
*/
#ifdef ANSI
PUBLIC NhCellCb* nhDBMGetCellCb
(
U16    cellId
)
#else /* ANSI */
PUBLIC NhCellCb* nhDBMGetCellCb(cellId)
U16    cellId;
#endif /* ANSI */
{
   CmLList   *tmpNode = NULLP;
   NhCellCb  *cellCb = NULLP;
   NhCellCb  *tmpCellCb = NULLP;

   TRC2(nhDBMGetCellCb)

   /* Find Cell Cb*/
   CM_LLIST_FIRST_NODE(&(nhCb.eNodeBCb.nhCellCbList.cellCbList), tmpNode);
   while(NULLP != tmpNode)
   {
      if (NULLP == tmpNode->node)
      {
         RLOG0(L_ERROR, "NULL logChCb");
         RETVALUE(ROK);
      }
      tmpCellCb = (NhCellCb *)(tmpNode->node);
      if(cellId == tmpCellCb->cellId)
      {
         cellCb = tmpCellCb;
         break;
      }
      CM_LLIST_NEXT_NODE(&(nhCb.eNodeBCb.nhCellCbList.cellCbList), tmpNode);
   }

   RETVALUE(cellCb);
} /* nhDBMGetCellCb */




/**
 * @brief Fetches the UE context.
 * 
 * @details 
 * 
 *
 * This function finds UE context based on the cell ID and the UE ID by
 * looking into the hash list.
 *
 * @param[in] cellCb Cell context
 * @param[in] ueId   UE identity (CRNTI)
 *
 * @return 
 *  -# Pointer to UE context, if found.
 *  -# NULLP, if not found.
*/
#ifdef ANSI
PUBLIC NhActvUeCb* nhDBMGetUeCb
(
NhCellCb    *cellCb,
U16         ueId                
)
#else /* ANSI */
PUBLIC NhActvUeCb* nhDBMGetUeCb(cellCb, ueId)
NhCellCb    *cellCb;
U16         ueId;                
#endif /* ANSI */
{
   NhActvUeCb  *ueCb = NULLP;
   S16         ret;

   TRC2(nhDBMGetUeCb)

   /* Find UE in the hash list */
   ret = nhHashListFind(&cellCb->actvUeHlCp, (U8 *)&ueId, sizeof(U16),
                        (PTR *)&ueCb);

   if(ROK != ret)
   {
      RLOG_ARG1(L_INFO, DBG_CELLID,(U32)cellCb->cellId,"UE [%d] not found.",ueId); 
      RETVALUE(NULLP);
   }

   RETVALUE(ueCb);
} /* nhDBMGetUeCb */

/**
 * @brief Fetches the UE context.
 * 
 * @details 
 * 
 *
 * This function finds UE context based on cell ID and UE ID by
 * looking into the newUeHlCp hash list present in CellCb.
 *
 * @param[in] cellCb Cell Context
 * @param[in] ueId   UE Identity (CRNTI)
 *
 * @return 
 *  -# Pointer to UE context if found.
 *  -# NULLP if not found.
*/
#ifdef ANSI
PUBLIC NhActvUeCb* nhDBMGetNewUeCb
(
NhCellCb    *cellCb,
U16          ueId                
)
#else /* ANSI */
PUBLIC NhActvUeCb* nhDBMGetNewUeCb(cellCb, ueId)
NhCellCb    *cellCb;
U16          ueId;                
#endif /* ANSI */
{
   NhActvUeCb  *ueCb = NULLP;
   S16          ret;
   NhUeCbKey    ueCbKey;

   TRC2(nhDBMGetNewUeCb)

   NH_SET_ZERO(&ueCbKey, sizeof(NhUeCbKey));
   ueCbKey.crnti = ueId;
   /* Find UE in the hash list */
   ret = nhHashListFind(&cellCb->newUeHlCp, (U8 *)&ueCbKey, sizeof(NhUeCbKey),
                        (PTR *)&ueCb);

   if(ROK != ret)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellCb->cellId,"UE [%d] not found.",ueId); 
      RETVALUE(NULLP);
   }

   RETVALUE(ueCb);
} /* nhDBMGetNewUeCb */
/**
 * @brief Initialises the UE context
 * 
 * @details 
 * 
 *
 * This function intialises the UE context and its subfields such as
 * MUI list and transCtrlBlk list. This is called by nhDBMCreateUeCb.
 *
 * @param[in,out] ueCb   UE context 
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PRIVATE S16 nhInitUeCb
(
NhActvUeCb   *ueCb
)
#else /* ANSI */
PRIVATE S16 nhInitUeCb(ueCb)
NhActvUeCb   *ueCb;
#endif /* ANSI */
{
   S16       ret;
   U8        i;
   TransCb   *transCtrlBlk=NULLP;
   MuiInfo   *muiInfo=NULLP;
   CmLList   *tempNode=NULLP;

   TRC2(nhInitUeCb);

   RLOG0(L_DEBUG, "Initialising UE control block.");
   /* Initialize the link lists */
   cmLListInit(&(ueCb->muiInfoLst));
   cmLListInit(&(ueCb->transCbLst));
   cmLListInit(&(ueCb->crntCfg.logChCbLst));

   /* Initialize the hash list */
   ret = nhHashListInit(&ueCb->msgTransHlCp, NH_TRANSCB_BINS, NH_TRANSCB_OFFSET,
                  FALSE, CM_HASH_KEYTYPE_DEF, NH_REG,NH_POOL);

   if(ROK != ret)
   {
      RETVALUE(ret);
   }

   /* Since this is the first time UE control block is getting
    * initialized, it creates the minimum number of nodes in the
    * transaction link list and initializes them.
    */
   for (i=0; i < nhCb.genCfg.minTransLstLen; i++)
   {
      NH_ALLOC(NH_REG, NH_POOL, &tempNode, sizeof(CmLList));
      if (NULLP == tempNode)
      {
         RLOG0(L_FATAL, "Failed to allocate memory.");
         RETVALUE(ROUTRES);
      }

      NH_ALLOC(NH_REG, NH_POOL, &transCtrlBlk, sizeof(TransCb));
      if (NULLP == transCtrlBlk)
      {
         RLOG0(L_FATAL, "Failed to allocate memory.");
         NH_FREE(NH_REG, NH_POOL, tempNode, sizeof(CmLList));
         RETVALUE(ROUTRES);
      }

      /* Initialize the structure for transId */
      /* This node is going to exist for the lifetime of ueCb */
      NH_SET_ZERO((U8 *)transCtrlBlk, sizeof(TransCb));
      transCtrlBlk->transId = i;

      tempNode->node = (PTR)(transCtrlBlk);

      /* Add the node to the TransCb list */
      cmLListAdd2Tail(&(ueCb->transCbLst), tempNode);
   }
   /* Since this is the first time UE control block is getting
    * initialized, it creates the minimum number of nodes in the
    * MUI link list and initializes them.
    */
   for (i=0; i < nhCb.genCfg.minMuiLstLen; i++)
   {
      NH_ALLOC(NH_REG, NH_POOL, &tempNode, sizeof(CmLList));
      if (NULLP == tempNode)
      {
         RLOG0(L_FATAL, "Failed to allocate memory.");
         RETVALUE(ROUTRES);
      }

      NH_ALLOC(NH_REG, NH_POOL, &muiInfo, sizeof(MuiInfo));
      if (NULLP == muiInfo)
      {
         RLOG0(L_FATAL, "Failed to allocate memory.");
         NH_FREE(NH_REG, NH_POOL, tempNode, sizeof(CmLList));
         RETVALUE(ROUTRES);
      }

      /* Initialize the structure for MUI */
      /* This node is going to exist for the lifetime of ueCb */
      NH_SET_ZERO((U8 *)muiInfo, sizeof(MuiInfo));
      muiInfo->mui = i;

      tempNode->node = (PTR)(muiInfo);

      /* Add the node to the TransCb list */
      cmLListAdd2Tail(&(ueCb->muiInfoLst), tempNode);
   }

   RETVALUE(ROK);
}/* nhInitUeCb */

/**
 * @brief Creates an UE context.
 * 
 * @details 
 * 
 *
 * This function creates and initialises the UE context.
 * @param[in] cellCb     Cell context
 * @param[in] crnti      UE identity (CRNTI)
 * @param[out] ueCb      UE context
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMCreateUeCb
(
NhCellCb       *cellCb,
U16            crnti,
NhActvUeCb     **ueCb       
)
#else /* ANSI */
PUBLIC S16 nhDBMCreateUeCb(cellCb, crnti, ueCb)
NhCellCb       *cellCb;
U16            crnti;
NhActvUeCb     **ueCb;      
#endif /* ANSI */
{
   S16   ret;

   TRC2(nhDBMCreateUeCb);

   RLOG0(L_DEBUG, "Creating UE control Block.");

   /* Allocating the memory for the New UE control Block */
   NH_ALLOC(NH_REG, NH_POOL, ueCb, sizeof(NhActvUeCb));
   if (NULLP == *ueCb)
   {
      RLOG0(L_FATAL, "Failed to allocate memory.");
      RETVALUE(ROUTRES);
   }

   /* Set UE IE and store pointer of Cell CB */
   (*ueCb)->cellCb = cellCb;
   (*ueCb)->ueCbKey.crnti = crnti;

   ret = nhInitUeCb(*ueCb);
   if(ROK != ret)
   {
      RLOG0(L_ERROR, "UE initialisation failed.");
      NH_FREE(NH_REG, NH_POOL, *ueCb, sizeof(NhActvUeCb));
      RETVALUE(ret);
   }

   /* This timestamp contains the information when
    * this UE control block was created
    */
   SGetSysTime(&((*ueCb)->tmStmp));

   /* Add UE CB to Cell Hash List */
   ret = nhHashListInsert(&cellCb->actvUeHlCp, (PTR) (*ueCb), 
                          (U8*) &((*ueCb)->ueCbKey),
                          sizeof (NhUeCbKey));
   if(ROK != ret)
   {
      RLOG0(L_ERROR, "UE addition to cell hash list failed.");
      nhDBMDeleteUeCb(cellCb, ueCb);
      RETVALUE(ret);
   }

   /* Update the number of Active UE connections */
   nhCb.eNodeBCb.numActUEs++;

#ifndef NH_NO_STS
   cellCb->cellSts.numUEs++;
#endif /* NH_NO_STS */

   RLOG0(L_DEBUG, "UE control Block Created and Initialized successfully.");

   RETVALUE(ROK);
} /* nhDBMCreateUeCb */

/**
 * @brief Deletes the UE context.
 * 
 * @details 
 * 
 *
 * This function deletes the UE context from the cell context.
 *
 * @param[in] cellCb  Cell context
 * @param[in] ueCb    UE context
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMDeleteUeCb
(
NhCellCb      *cellCb,
NhActvUeCb    **ueCb       
)
#else /* ANSI */
PUBLIC S16 nhDBMDeleteUeCb(cellCb, ueCb)
NhCellCb      *cellCb;
NhActvUeCb    **ueCb;       
#endif /* ANSI */
{
   CmLList    *tmp = NULLP;
   TransCb    *transCtrlBlk = NULLP;
   NhLLyrCfgTmrCb    *tmrCb = NULLP;
   CmTmrArg          arg;
   U8                tmrNum;

   TRC2(nhDBMDeleteUeCb);

   RLOG0(L_DEBUG, "Deleting UE control Block.");

   tmrNum = 0;
   tmrCb = NULLP;

   nhHashListDelete(&cellCb->actvUeHlCp, (PTR)*ueCb);
   nhHashListDelete(&cellCb->newUeHlCp, (PTR)*ueCb);

   /* Traverse the Message Transaction Link list and delete all the
    * entries from the link list and hashlist
    */
   CM_LLIST_FIRST_NODE(&((*ueCb)->transCbLst), tmp);

   while(NULLP != tmp)
   {
      /* Delete the entry from the hashlist */
      transCtrlBlk = (TransCb*)(tmp->node);
      nhHashListDelete(&(*ueCb)->msgTransHlCp, (PTR) transCtrlBlk);

      /* Delete the Entry from the Link List */
      cmLListDelFrm(&((*ueCb)->transCbLst), tmp);

      /* Delete the ordered configuration from transCtrlBlk */
      if(transCtrlBlk->ordrdCfg.rbToAddCfgList.numOfRbCfgInfo)
      {
         NH_FREE(NH_REG, NH_POOL, transCtrlBlk->ordrdCfg.rbToAddCfgList.rbCfgInfoList,
               transCtrlBlk->ordrdCfg.rbToAddCfgList.numOfRbCfgInfo * sizeof(NhuRbCfgInfo));
      }
      if(transCtrlBlk->ordrdCfg.rbToModCfgList.numOfRbCfgInfo)
      {
         /* Delete UL bitmap of HO info from ordered cfg if present */
         nhUTLFreeUlBitMapFrmOrdCfg(transCtrlBlk, 
                     transCtrlBlk->ordrdCfg.rbToModCfgList.numOfRbCfgInfo);

         NH_FREE(NH_REG, NH_POOL, transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList,
               transCtrlBlk->ordrdCfg.rbToModCfgList.numOfRbCfgInfo * sizeof(NhuRbCfgInfo));
      }
      if(transCtrlBlk->ordrdCfg.rbToRlsCfgList.numOfRbCfgInfo)
      {
         NH_FREE(NH_REG, NH_POOL, transCtrlBlk->ordrdCfg.rbToRlsCfgList.rbsToRel,
               transCtrlBlk->ordrdCfg.rbToRlsCfgList.numOfRbCfgInfo * sizeof(NhuRbId));
      }
      /* Delete store message and lowerLyrParam strcture */
      if (transCtrlBlk->storedMsg)
      {
         NH_FREEMBUF(transCtrlBlk->storedMsg);
      }

      if (transCtrlBlk->lowerLyrParam)
      {
         NH_FREE(NH_REG, NH_POOL, transCtrlBlk->lowerLyrParam,
                  sizeof(NhDatReqParam));
      }
      
      /* Delete the transCtrlBlk */
      NH_FREE(NH_REG, NH_POOL, transCtrlBlk, sizeof(TransCb));
      NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
      CM_LLIST_FIRST_NODE(&((*ueCb)->transCbLst), tmp);
   }

   /* De-Init Transaction Hash list */
   /* nh005.201 124486-RRC Crash Fix */
   nhHashListDeInit(&((*ueCb)->msgTransHlCp));

   /* Update the number of active UEs */
   if (nhCb.eNodeBCb.numActUEs)
   {
      nhCb.eNodeBCb.numActUEs--;
#ifndef NH_NO_STS
      cellCb->cellSts.numUEs--;
#endif /* NH_NO_STS */
   }

   /* Traverse the MUI Link list and delete all the
    * entries from the link list and hashlist
    */
   CM_LLIST_FIRST_NODE(&((*ueCb)->muiInfoLst), tmp);

   while (NULLP != tmp)
   {
      /* Delete the Entry from the Link List */
      cmLListDelFrm(&((*ueCb)->muiInfoLst), tmp);
      NH_FREE(NH_REG, NH_POOL, tmp->node, sizeof(MuiInfo));
      NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
      CM_LLIST_FIRST_NODE(&((*ueCb)->muiInfoLst), tmp);
   }

   /* Delete logical channel and RB Map from Current Configuration */
   CM_LLIST_FIRST_NODE(&(*ueCb)->crntCfg.logChCbLst, tmp);

   while(NULLP != tmp)
   {
      cmLListDelFrm(&(*ueCb)->crntCfg.logChCbLst, tmp);
      NH_FREE(NH_REG, NH_POOL, tmp->node, sizeof(NhRbToLchMap));
      NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
      CM_LLIST_FIRST_NODE(&(*ueCb)->crntCfg.logChCbLst, tmp);
   }

   /* Delete ulBitMap from HO info if present */
   nhUTLFreeUlBitMapFrmCrntRbCfg(&(*ueCb)->crntCfg);

   /* Delete RB Configuration list from Current Configuration */
   CM_LLIST_FIRST_NODE(&(*ueCb)->crntCfg.rbCfgList.rbCfgInfoList, tmp);

   while(NULLP != tmp)
   {
      cmLListDelFrm(&(*ueCb)->crntCfg.rbCfgList.rbCfgInfoList, tmp);
      NH_FREE(NH_REG, NH_POOL, tmp->node, sizeof(NhuRbCfgInfo));
      NH_FREE(NH_REG, NH_POOL, tmp, sizeof(CmLList));
      CM_LLIST_FIRST_NODE(&(*ueCb)->crntCfg.rbCfgList.rbCfgInfoList, tmp);
   }

   /* stop any running timers before buffer release. */
   for (tmrNum = 0; tmrNum < NH_MAX_CFG_TMRS; tmrNum++)
   {
      tmrCb = (NhLLyrCfgTmrCb *)((*ueCb)->cellCb->cfgTimers[tmrNum].cb);
      if ((NULLP != tmrCb) && (tmrCb->ueCb == (*ueCb)))
      {
         /* fill the argument */
         arg.tq     = nhCb.nhLLyrCfgTq;
         arg.tqCp   = &(nhCb.nhLLyrCfgTqCp);
         arg.timers = (*ueCb)->cellCb->cfgTimers;
         arg.max    = NH_MAX_CFG_TMRS;
         arg.cb     = (PTR)tmrCb;
         arg.evnt   = NH_TMR_UE_LLYR_CFGCFM;
         arg.wait   = NOTUSED;
         arg.tNum   = tmrNum;

         /* Stop the Lower layer Cfg timer */
         cmRmvCbTq(&arg);

         /* Deallocate the lower layer config control block */
         NH_FREE(NH_REG, NH_POOL, tmrCb, sizeof(NhLLyrCfgTmrCb));
      }
   }

   /* Free memory allocated to UeCb */
   NH_FREE(NH_REG, NH_POOL, *ueCb, sizeof(NhActvUeCb));

   RETVALUE(ROK);
} /* nhDBMDeleteUeCb */

/* ccpu00142868 : eNB does not send RRC_CONN_RLS in case of RRC_REEST with cause other */
/**
 * @brief Deletes all the transaction blocks of a UE.
 * 
 * @details 
 * 
 *
 * This function deletes all the transaction blocks of a UE.
 *
 * @param[in] ueCb    UE context
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMDelTransCbs
(
NhActvUeCb    *ueCb       
)
#else /* ANSI */
PUBLIC S16 nhDBMDelTransCbs(ueCb)
NhActvUeCb    *ueCb;       
#endif /* ANSI */
{
   CmLList    *tmp = NULLP;
   TransCb    *transCtrlBlk = NULLP;

   TRC2(nhDBMDelTransCbs);

   RLOG0(L_DEBUG, "Deleting all transactions of UE.");

   /* Traverse the default list */
   CM_LLIST_FIRST_NODE(&(ueCb->transCbLst), tmp);
   while(NULLP != tmp)
   {
      transCtrlBlk = (TransCb*)(tmp->node);
      /*Deleting Onebyone using existing Function*/
      nhDBMDelTransCb(ueCb, &transCtrlBlk);
      CM_LLIST_NEXT_NODE(&(ueCb->transCbLst), tmp);
   }

   RETVALUE(ROK);
}/* nhDBMDelTransCbs */
   

/**
 * @brief Updates Modified RB Configuration (Uplink Configuration) for an UE.
 * 
 * @details 
 * 
 * This function updates the rbCfg in present in the crnt cfg from the ordered
 * cfg during Rb Modify case (Uplink Configuration).
 *
 * @param[in] dstRbCfgInfo   Destination where RB information needs to be updated
 * @param[in] srcRbCfgInfo   Source containing RB information to be used to update
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMUpdModifiedRbCfgUl
(
NhuRbCfgInfo *dstRbCfgInfo,
NhuRbCfgInfo *srcRbCfgInfo
)
#else /* ANSI */
PUBLIC S16 nhDBMUpdModifiedRbCfgUl(dstRbCfgInfo, srcRbCfgInfo)
NhuRbCfgInfo *dstRbCfgInfo;
NhuRbCfgInfo *srcRbCfgInfo;
#endif /* ANSI */
{
   /* nh004.201 - Correction to TRC functionality. */
   TRC2(nhDBMUpdModifiedRbCfgUl)

   if(srcRbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)
   {
      if(srcRbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChCfgpres &
                                          NHU_UL_LOGCH_MAP_GRP)
         dstRbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChGrp = 
                  srcRbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChGrp;

   }
   if(srcRbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_RLC_MODE_UL)
   {
      if(srcRbCfgInfo->ulRbCfg.ulRlcMode.mode == NHU_RLC_MODE_UM)
         NH_MEM_COPY((U8*)&dstRbCfgInfo->ulRbCfg.ulRlcMode.m.umRlc, 
                  (U8*) &srcRbCfgInfo->ulRbCfg.ulRlcMode.m.umRlc,
                  sizeof(NhuUlUmRlcModeInfo));
      else if(srcRbCfgInfo->ulRbCfg.ulRlcMode.mode == NHU_RLC_MODE_AM)
         NH_MEM_COPY((U8*)&dstRbCfgInfo->ulRbCfg.ulRlcMode.m.amRlc, 
                  (U8*) &srcRbCfgInfo->ulRbCfg.ulRlcMode.m.amRlc,
                  sizeof(NhuUlAmRlcModeInfo));

   }

   RETVALUE(ROK);
}/* End of nhDBMUpdModifiedRbCfgUl */

/**
 * @brief Updates Modified RB Configuration (Downlink Configuration) for an UE.
 * 
 * @details 
 * 
 * This function updates the rbCfg in present in the crnt cfg from the ordered
 * cfg during Rb Modify case (Downlink Configuration).
 *
 * @param[in] dstRbCfgInfo   Destination where RB information needs to be updated
 * @param[in] srcRbCfgInfo   Source containing RB information to be used to update
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMUpdModifiedRbCfgDl
(
NhuRbCfgInfo *dstRbCfgInfo,
NhuRbCfgInfo *srcRbCfgInfo
)
#else /* ANSI */
PUBLIC S16 nhDBMUpdModifiedRbCfgDl(dstRbCfgInfo, srcRbCfgInfo)
NhuRbCfgInfo *dstRbCfgInfo;
NhuRbCfgInfo *srcRbCfgInfo;
#endif /* ANSI */
{
   /* nh004.201 - Correction to TRC functionality. */
   TRC2(nhDBMUpdModifiedRbCfgDl)

   /* TM mode not expected, nothing to do */
   if(srcRbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_DL)
   {
      if(srcRbCfgInfo->dlRbCfg.dlRlcMode.mode == NHU_RLC_MODE_UM)
         NH_MEM_COPY((U8*)&dstRbCfgInfo->dlRbCfg.dlRlcMode.m.umRlc,
                  (U8*) &srcRbCfgInfo->dlRbCfg.dlRlcMode.m.umRlc,
                  sizeof(NhuDlUmRlcModeInfo));
      else if(srcRbCfgInfo->dlRbCfg.dlRlcMode.mode == NHU_RLC_MODE_AM)
         NH_MEM_COPY((U8*)&dstRbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc,
                  (U8*) &srcRbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc,
                  sizeof(NhuDlAmRlcModeInfo));
   }

   RETVALUE(ROK);
}/* End of nhDBMUpdModifiedRbCfgDl */

/**
 * @brief Updates Modified RB Configuration (PDCP Inforamtion) for an UE.
 * 
 * @details 
 * 
 * This function updates the rbCfg in present in the crnt cfg from the ordered
 * cfg during Rb Modify case (PDCP Inforamtion).
 *
 * @param[in] dstRbCfgInfo   Destination where RB information needs to be updated
 * @param[in] srcRbCfgInfo   Source containing RB information to be used to update
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhDBMUpdModifiedRbPdcpInfo
(
NhuRbCfgInfo *dstRbCfgInfo,
NhuRbCfgInfo *srcRbCfgInfo
)
#else /* ANSI */
PUBLIC S16 nhDBMUpdModifiedRbPdcpInfo(dstRbCfgInfo, srcRbCfgInfo)
NhuRbCfgInfo *dstRbCfgInfo;
NhuRbCfgInfo *srcRbCfgInfo;
#endif /* ANSI */
{
   Size  ulBitMapSize = 0;

   /* nh004.201 - Correction to TRC functionality. */
   TRC2(nhDBMUpdModifiedRbPdcpInfo)

   if(srcRbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_DISCARD_TMR)
      dstRbCfgInfo->pdcpCfg.discardTmr = srcRbCfgInfo->pdcpCfg.discardTmr;
   if(srcRbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_FLUSH_TMR)
      dstRbCfgInfo->pdcpCfg.flushTmr = srcRbCfgInfo->pdcpCfg.flushTmr;
   if(srcRbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_SN_SIZE)
      dstRbCfgInfo->pdcpCfg.snSize = srcRbCfgInfo->pdcpCfg.snSize;
   if(srcRbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_STATUS_RPT)
      dstRbCfgInfo->pdcpCfg.statusRptReq = 
                               srcRbCfgInfo->pdcpCfg.statusRptReq;
   if(srcRbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_HDR_COMPR_INFO)
      NH_MEM_COPY((U8*)&dstRbCfgInfo->pdcpCfg.hdrComprInfo, 
                 (U8*)&srcRbCfgInfo->pdcpCfg.hdrComprInfo,
                  sizeof(NhuPdcpHdrComprInfo));
   /* Update HO information */
   if(srcRbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_SDU_STA_INFO)
   {
      NH_MEM_COPY((U8*)&dstRbCfgInfo->pdcpCfg.hoPdcpSduStaInfo, 
                 (U8*)&srcRbCfgInfo->pdcpCfg.hoPdcpSduStaInfo,
                  sizeof(NhuHoPdcpSduInfo)); 

      if(NH_ZERO_VAL != srcRbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.numBits)
      {
         /* Get number of octets to allocate from numBits */
         NH_GET_UL_BITMAP_SIZE(
               srcRbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.numBits,
               ulBitMapSize); 

         /* Allocate memory for ulBitMap */
         NH_ALLOC(NH_REG, NH_POOL, 
                  &dstRbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.ulBitMap, 
                  sizeof(U8) * ulBitMapSize);

         if(NULLP == dstRbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.ulBitMap)
         {
             RLOG0(L_FATAL, "Failed to allocate memory for ulBitMap.");
             RETVALUE(RFAILED);
         }
         NH_MEM_COPY((U8*)dstRbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.ulBitMap,
                     (U8*)srcRbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.ulBitMap,
                     sizeof(U8) * ulBitMapSize);
      }
   }
   if(srcRbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_CFM_REQD)
   {
      dstRbCfgInfo->pdcpCfg.cfmReqd = srcRbCfgInfo->pdcpCfg.cfmReqd;
   }

   dstRbCfgInfo->pdcpCfg.pdcpCfgBitMsk |=
                      srcRbCfgInfo->pdcpCfg.pdcpCfgBitMsk;

   RETVALUE(ROK);
}/* End of nhDBMUpdModifiedRbPdcpInfo */
/** @} */

/**********************************************************************
  
         End of file:     ni_dbm.c@@/main/3 - Fri Jul  1 01:13:17 2011
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1.
           nh004.201 vkulkarni        1. Correction to TRC functionality.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
/main/4    nh005.201  njha             1.Crash fix
*********************************************************************91*/
