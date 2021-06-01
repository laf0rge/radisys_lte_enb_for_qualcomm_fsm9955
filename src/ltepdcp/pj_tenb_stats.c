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
 
    Name:  
 
    Type:   C include file
 
    Desc:   
 
    File:  pj_tenb_stats.c
 
    Sid:      pj_tenb_stats.c@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:53:14 2016
 
    Prg:   sm
 
**********************************************************************/
 
/** @file pj_tenb_stats.c
@brief This file contains 
*/

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*
*/
/* header include files (.h) */
  
#include "envopt.h"             /* Environment options */  
#include "envdep.h"             /* Environment dependent */
#include "envind.h"             /* Environment independent */
#include "gen.h"                /* General */
#include "ssi.h"                /* System services interface */
#include "cm_llist.h"           /* Linked list header file */
#include "cm_tkns.h"            /* Common tokens header file */


/* header/extern include files (.x) */

#include "gen.x"                /* General */
#include "ssi.x"                /* System services interface */
#include "cm_lib.x"             /* Common lib header file */
#include "cm_llist.x"           /* Linked list header file */
#include "cm_tkns.x"            /* Common tokens header file */
#include "ss_queue.h"
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"
#include "pj_tenb_stats.x"    /* Total EnodeB Stats declarations */

#ifdef TENB_STATS
PUBLIC TSL2CellStatsCb* l2CellStats[L2_STATS_MAX_CELLS];
PUBLIC TSL2UeStatsCb*   l2UeStats[L2_STATS_MAX_UES];
PUBLIC CmLListCp        freeL2UeStatsLst; /*!< Free Pool of UE stats Blocks */
PUBLIC CmLListCp        inUseL2UeStatsLst;/*!< In Use Pool of UE stats Blocks */

/*
*
*       Fun:   TSL2AllocStatsMem
*
*       Desc:  Pre-Allocate Memory for L2 stats BLOCKs 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC Void TSL2AllocStatsMem 
(
 Region region,
 Pool   pool 
)
#else
PUBLIC Void TSL2AllocStatsMem(region, pool)
 Region region;
 Pool   pool;
#endif
{
   U32 cnt=0;

   TRC2(TSL2AllocStatsMem)

   cmLListInit(&inUseL2UeStatsLst);     
   cmLListInit(&freeL2UeStatsLst);      
   for (cnt=0; cnt < L2_STATS_MAX_CELLS; cnt++)
   {
      if (SGetSBuf(region, pool, (Data **)&l2CellStats[cnt],
               (Size)sizeof (TSL2CellStatsCb)) != ROK)
      {
         printf("\n STATS Unexpected MEM Alloc Failure\n");
      }
      cmMemset((U8 *)l2CellStats[cnt], 0x00, (Size)sizeof(TSL2CellStatsCb));
   }

   for (cnt=0; cnt < L2_STATS_MAX_UES; cnt++)
   {
      TSL2UeStatsCb *statsCb = NULLP;
      if (SGetSBuf(region, pool, (Data **)&statsCb,
               (Size)sizeof (TSL2UeStatsCb)) != ROK)
      {
         printf("\n STATS Unexpected MEM Alloc Failure at %d\n", (int)cnt);
      }
      cmMemset((U8 *)statsCb, 0x00, (Size)sizeof(TSL2UeStatsCb));
      statsCb->lnk.node = (PTR)statsCb;
      cmLListAdd2Tail(&freeL2UeStatsLst, &statsCb->lnk);
      l2UeStats[cnt] = statsCb;
   }

   RETVOID;
}

/*
*
*       Fun:   TSL2AllocUeStatsBlk
*
*       Desc:  Assign Stats Block for this UE[RNTI] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC TSL2UeStatsCb* TSL2AllocUeStatsBlk 
(
 U16   rnti
)
#else
PUBLIC TSL2UeStatsCb* TSL2AllocUeStatsBlk(rnti)
 U16   rnti;
#endif
{
   CmLList          *tmp = NULLP;
   TSL2UeStatsCb  *statsCb = NULLP;

   TRC2(TSL2AllocUeStatsBlk)

   tmp = freeL2UeStatsLst.first;
   if (tmp == NULLP)
   {
      printf("\n STATS Unexpected Mem BLK unavailable for UE %d\n", rnti);
   }
   cmLListDelFrm(&freeL2UeStatsLst, tmp);
   statsCb = (TSL2UeStatsCb *)(tmp->node);
   cmLListAdd2Tail(&inUseL2UeStatsLst, tmp);

   statsCb->stats.rnti = (U32)rnti;
   statsCb->inUse = TRUE;

   RETVALUE(statsCb);
}

/*
*
*       Fun:   TSL2DeallocUeStatsBlk
*
*       Desc:  Deassign Stats Block for this UE[RNTI] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC Void TSL2DeallocUeStatsBlk 
(
 U16              rnti,
 TSL2UeStatsCb  *statsCb
)
#else
PUBLIC Void TSL2DeallocUeStatsBlk(rnti, statsCb)
 U16              rnti;
 TSL2UeStatsCb  *statsCb;
#endif
{
   TRC2(TSL2DeallocUeStatsBlk)

   statsCb->inUse = FALSE;
   cmLListDelFrm(&inUseL2UeStatsLst, &statsCb->lnk);
   freeL2UeStatsLst.crnt = freeL2UeStatsLst.first;
   cmLListInsAfterCrnt(&freeL2UeStatsLst, &statsCb->lnk);

   RETVOID;
}

/*
*
*       Fun:   TSL2AllocCellStatsBlk
*
*       Desc:  Assign Stats Block for this CELL[CELLID] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC TSL2CellStatsCb* TSL2AllocCellStatsBlk 
(
 U32 cellId
)
#else
PUBLIC TSL2CellStatsCb* TSL2AllocCellStatsBlk(cellId)
 U32 cellId;
#endif
{
   TRC2(TSL2AllocCellStatsBlk)

   if (cellId != 1)
   {
      printf("\n STATS Unexpected CellID = %d\n", (int)cellId);
   }

   RETVALUE(l2CellStats[cellId-1]);
}

/*
*
*       Fun:   TSL2DeallocCellStatsBlk
*
*       Desc:  Deassign Stats Block for this CELL[CELLID] 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC Void TSL2DeallocCellStatsBlk 
(
 U32 cellId
)
#else
PUBLIC Void TSL2DeallocCellStatsBlk(cellId)
 U32 cellId;
#endif
{
   TRC2(TSL2DeallocCellStatsBlk)

   RETVOID;
}

/*
*
*       Fun:   TSL2SendStatsToApp
*
*       Desc:  Collates and Sends STATS to Application 
*              Send UE STATS first. 10 UEs are grouped in one message.
*              Followed by CELL Stats. All CELLS are grouped in one msg.
*              At Reception of CELL stats APP assumes STATS reception cycle is complete.
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC Void TSL2SendStatsToApp
(
 Pst    *pst,
 SuId   suId
 )
#else
PUBLIC Void TSL2SendStatsToApp(pst, suId)
 Pst    *pst;
 SuId   suId;
#endif
{
   U32 idx;

   TRC2(TSL2SendStatsToApp)

   for (idx = 0; idx < L2_STATS_MAX_UES; idx++)
   {
      TSL2UeStatsCb *statsCb = l2UeStats[idx];
      U32 rnti;
      if (statsCb->inUse != TRUE)
      {
         continue;
      }
      if (pst->selector == 0)
      {
         /* Loose Coupling */
         TSInfPkSndL2UeStats(pst, suId, &statsCb->stats);
      }
      else
      {
#ifdef PX
         /* Tight Coupling */
         TSInfHdlL2UeStatsInd(pst, suId, &statsCb->stats);
#endif
      }
      rnti = statsCb->stats.rnti;
      cmMemset((U8 *)&statsCb->stats, 0x00, (Size)sizeof(TSInfL2UeStats));
      statsCb->stats.rnti = rnti;
   }

   /* Allocate mBuf for CELLSTATS */
   for (idx = 0; idx < L2_STATS_MAX_CELLS; idx++)
   {
      if (pst->selector == 0)
      {
         /* Loose Coupling */
         TSInfPkSndL2CellStats(pst, suId, l2CellStats[idx]);
      }
      else
      {
#ifdef PX
         /* Tight Coupling */
         TSInfHdlL2CellStatsInd(pst, suId, l2CellStats[idx]);
#endif
      }
      cmMemset((U8 *)l2CellStats[idx], 0x00, (Size)sizeof(TSInfL2CellStats));
   }

   RETVOID;
}

/*
*
*       Fun:   TSL2MapUeStatsBlk
*
*       Desc:  UE Creation at SCH would have allocated ueStatsCb
*       for a given RNTI.
*       Other Layers, shall invoke this API to GET the previously created*
*       ueStatsCb.
*       RETURN value can also be NULLP, hence other layers should validate
*       before upadting stats.
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC TSL2UeStatsCb* TSL2MapUeStatsBlk 
(
 U16   rnti
)
#else
PUBLIC TSL2UeStatsCb* TSL2MapUeStatsBlk(rnti)
 U16   rnti;
#endif
{
   CmLList   *node;

   TRC2(TSL2MapUeStatsBlk)

   node = inUseL2UeStatsLst.first;
   while(node)
   {
      TSL2UeStatsCb  *ueStats = (TSL2UeStatsCb *)(node->node);
      if (ueStats->stats.rnti == rnti)
      {
         RETVALUE(ueStats);
      }
      node = node->next;
   }
   RETVALUE(NULLP);
}
#endif /* TENB_STATS */
/**********************************************************************
         End of file:     pj_tenb_stats.c@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:53:14 2016
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------

*********************************************************************91*/
