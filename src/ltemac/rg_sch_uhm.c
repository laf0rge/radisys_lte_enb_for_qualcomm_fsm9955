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




/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_sch_uhm.c
  
     Sid:      gk_sch_uhm.c@@/main/2 - Sat Jul 30 02:21:52 2011
  
     Prg:      sd
  
**********************************************************************/

/** @file rg_sch_uhm.c
@brief This module handles uplink harq related functionality in MAC.
*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */

#include "cm_lte.h"        /* Common LTE */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* memory management */

#include "rg_env.h"        /* MAC Environment Defines */
#include "tfu.h"           /* TFU Interface defines */
#include "rgr.h"           /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg_sch.h"            /* Scheduler defines */
#include "rg_sch_inf.h"            /* Scheduler defines */
#include "rg_sch_err.h"        /* MAC error defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* system services */
#include "cm_lte.x"        /* Common LTE */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* memory management */

#include "tfu.x"           /* TFU Interface defines */
#include "rgr.x"           /* RGR Interface includes */
#include "lrg.x"           /* LRG Interface includes */
#include "rgm.x"
#include "rg_sch_inf.x"            /* Scheduler defines */
#include "rg_sch.x"            /* Scheduler includes */
#include "rg_sch_cmn.h"
#include "rg_sch_cmn.x"

/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */


PUBLIC U8 rgRvIdxTable[] = {0, 3, 1, 2}; /* This gives rvIdx for a given rv */
PUBLIC U8 rgRvTable[] = {0, 2, 3 ,1};    /* This gives rv for a given rvIdx */

/**
 * @brief Handler for HARQ processing on recieving Data indication from PHY.
 *
 * @details
 *
 *     Function: rgSCHUhmProcDatInd
 *     
 *     Invoked by: rgSCHTomTfuDatInd of  TOM
 *
 *     Processing Steps:
 *      - Set rcvdCrcInd variable to TRUE
 *           
 *  @param[in] *cell
 *  @param[in] *ue
 *  @param[in] frm
 *  @return  Void
 **/
#ifndef MAC_SCH_STATS
#ifdef ANSI
PUBLIC Void rgSCHUhmProcDatInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      frm
)
#else
PUBLIC Void rgSCHUhmProcDatInd(cell, ue, frm)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      frm;
#endif
#else  /* MAC_SCH_STATS */
#ifdef ANSI
PUBLIC Void rgSCHUhmProcDatInd
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      frm,
U8                   cqi
)
#else
PUBLIC Void rgSCHUhmProcDatInd(cell, ue, frm, cqi)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      frm;
U8                   cqi;
#endif
#endif /* MAC_SCH_STATS */
{
   RgSchUlHqProcCb   *hqProc;
#ifdef UL_LA
   RgSchCmnUlUe   *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   S32            iTbs;
   U8             maxiTbs = rgSchCmnUlCqiToTbsTbl[cell->isCpUlExtend]
                                                 [ueUl->maxUlCqi];
#endif

   TRC2(rgSCHUhmProcDatInd);

   rgSCHUtlUlHqProcForUe(cell, frm, ue, &hqProc);
#ifdef UL_LA
#ifdef LTEMAC_SPS
   if((hqProc->isSpsActvnHqP != TRUE)  &&
      (hqProc->isSpsOccnHqP != TRUE))
#endif
   {
      ueUl->ulLaCb.deltaiTbs += UL_LA_STEPUP;
      iTbs = (ueUl->ulLaCb.cqiBasediTbs + ueUl->ulLaCb.deltaiTbs)/100;

      if (iTbs > maxiTbs)
      {
         ueUl->ulLaCb.deltaiTbs = (maxiTbs * 100) - ueUl->ulLaCb.cqiBasediTbs;
      }

   }
#endif
#ifdef MAC_SCH_STATS
   /** Stats update over here 
    */
   {
      hqFailStats.ulCqiStat[cqi - 1].numOfAcks++;
   }
#endif

   hqProc->rcvdCrcInd = TRUE;
#ifdef TENB_STATS
   cell->tenbStats->sch.ulAckNack[rgRvTable[hqProc->rvIdx]]++;
   ue->tenbStats->stats.sch.ulAckNackCnt++;
   if(hqProc->alloc)
   {
      ue->tenbStats->stats.sch.ulTpt += (hqProc->alloc->grnt.datSz << 3);
      cell->tenbStats->sch.ulTtlTpt +=(hqProc->alloc->grnt.datSz << 3);//pverma
   }
#endif
#ifdef LTE_TDD
   RGSCH_UPD_PHICH(cell->ulDlCfgIdx, frm.subframe, hqProc);
#endif
   
   RETVOID;
}  /* rgSCHUhmProcDatInd */

/**
 * @brief Handler for HARQ processing on recieving Data indication from PHY.
 *
 * @details
 *
 *     Function: rgSCHUhmProcMsg3DatInd
 *     
 *     Invoked by: rgSCHTomTfuDatInd of  TOM
 *
 *     Processing Steps:
 *      - Set rcvdCrcInd variable to TRUE  
 *           
 *  @param[in,out] *hqProc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUhmProcMsg3DatInd
(
RgSchUlHqProcCb *hqProc
)
#else
PUBLIC Void rgSCHUhmProcMsg3DatInd(hqProc)
RgSchUlHqProcCb *hqProc;
#endif
{
   TRC2(rgSCHUhmProcMsg3DatInd);
   hqProc->rcvdCrcInd = TRUE;
   RETVOID;
}  /* rgSCHUhmProcMsg3DatInd */

/**
 * @brief Handler for HARQ processing on recieving Data indication from PHY.
 *
 * @details
 *
 *     Function: rgSCHUhmProcMsg3Failure
 *     
 *     Invoked by: rgSCHTomTfuDatInd of  TOM
 *
 *     Processing Steps:
 *      - Set rcvdCrcInd variable to TRUE  
 *           
 *  @param[in,out] *hqProc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUhmProcMsg3Failure
(
RgSchUlHqProcCb *hqProc
)
#else
PUBLIC Void rgSCHUhmProcMsg3Failure(hqProc)
RgSchUlHqProcCb *hqProc;
#endif
{
   TRC2(rgSCHUhmProcMsg3Failure);

   hqProc->rcvdCrcInd = FALSE;
   RETVOID;
}  /* rgSCHUhmProcMsg3Failure */

/**
 * @brief Handler for HARQ processing on recieving Decode failure from PHY.
 *
 * @details
 *
 *     Function: rgSCHUhmProcHqFailure
 *     
 *     Invoked by: rgSCHTomTfuDecFailInd of TOM
 *
 *     Processing Steps: 
 *      - Update NACK information in harq info.
 *      - Update RV index of received RV from PHY in harq info.
 *      - Set PhichInfo in DlSf
 *           
 *  @param[in] *cell
 *  @param[in] *ue
 *  @param[in] frm
 *  @param[in] rv
 *  @return  Void
 **/
#ifndef MAC_SCH_STATS
#ifdef ANSI
PUBLIC Void rgSCHUhmProcHqFailure
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      frm,
TknU8                rv
)
#else
PUBLIC Void rgSCHUhmProcHqFailure(cell, ue, frm, rv)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      frm;
TknU8                rv;
#endif
#else /* MAC_SCH_STATS */
#ifdef ANSI
PUBLIC Void rgSCHUhmProcHqFailure
(
RgSchCellCb          *cell,
RgSchUeCb            *ue,
CmLteTimingInfo      frm,
TknU8                rv,
U8                   cqi
)
#else
PUBLIC Void rgSCHUhmProcHqFailure(cell, ue, frm, rv, cqi)
RgSchCellCb          *cell;
RgSchUeCb            *ue;
CmLteTimingInfo      frm;
TknU8                rv;
U8                   cqi;
#endif
#endif /* MAC_SCH_STATS */
{
   RgSchUlHqProcCb   *hqProc;
#ifdef UL_LA
   RgSchCmnUlUe   *ueUl = RG_SCH_CMN_GET_UL_UE(ue);
   S32               iTbs;
#endif
   TRC2(rgSCHUhmProcHqFailure);

   rgSCHUtlUlHqProcForUe(cell, frm, ue, &hqProc);
#ifdef UL_LA
#ifdef LTEMAC_SPS
   if((hqProc->isSpsActvnHqP != TRUE)  &&
      (hqProc->isSpsOccnHqP != TRUE))
#endif
   {
      ueUl->ulLaCb.deltaiTbs -= UL_LA_STEPDOWN;
      iTbs = (ueUl->ulLaCb.cqiBasediTbs + ueUl->ulLaCb.deltaiTbs)/100;

      if (iTbs < 0)
      {
         ueUl->ulLaCb.deltaiTbs = -(ueUl->ulLaCb.cqiBasediTbs);
      }

   } 
#endif
#ifdef MAC_SCH_STATS
   /** Stats update over here */
   {
      static U32 retxCnt = 0;
      ++retxCnt;
      hqFailStats.ulCqiStat[cqi - 1].numOfNacks++;
   }
#endif /* MAC_SCH_STATS */

   hqProc->rcvdCrcInd = FALSE;
#ifdef TENB_STATS
   cell->tenbStats->sch.ulAckNack[rgRvTable[hqProc->rvIdx]]++;
   ue->tenbStats->stats.sch.ulAckNackCnt++;
   cell->tenbStats->sch.ulNack[rgRvTable[hqProc->rvIdx]]++;
   ue->tenbStats->stats.sch.ulNackCnt++;
#endif
   hqProc->rvIdxPhy.pres = rv.pres;
   if(rv.pres)
   {
      hqProc->rvIdxPhy.val = rgRvIdxTable[rv.val];
   }
#ifdef LTE_TDD
   RGSCH_UPD_PHICH(cell->ulDlCfgIdx, frm.subframe, hqProc);
#endif
   RETVOID;
} /* rgSCHUhmProcHqFailure */

/**
 * @brief Handler for identifying the HARQ process cb associated with the
 * index.
 *
 * @details
 *
 *     Function: rgSCHUhmGetUlHqProc
 *     
 *     Processing Steps: 
 *      - Return pointer to uplink harq process corresponding to the timing
 *        information passed.
 *           
 *  @param[in]  *ue
 *  @param[in]  idx
 *  @return  RgSchUlHqProcCb*
 *      -# Pointer to harq process corresponding to index
 *      -# NULL
 **/
#ifdef ANSI
PUBLIC RgSchUlHqProcCb* rgSCHUhmGetUlHqProc
(
RgSchUeCb        *ue, 
U8               idx
)
#else
PUBLIC RgSchUlHqProcCb* rgSCHUhmGetUlHqProc(ue, idx)
RgSchUeCb        *ue; 
U8               idx;
#endif
{
   TRC2(rgSCHUhmGetUlHqProc);

#if (ERRCLASS & ERRCLS_DEBUG)
   if(idx >= ue->ul.hqEnt.numHqPrcs)
   {
      RETVALUE(NULLP);
   }
#endif
   RETVALUE(&(ue->ul.hqEnt.hqProcCb[idx]));
}  /* rgSCHUhmGetUlHqProc */

/**
 * @brief Handler for HARQ processing on recieving new trasmission indication 
 * from USM.
 *
 * @details
 *
 *     Function: rgSCHUhmNewTx
 *     
 *     Invoked by: USM
 *
 *     Processing Steps: 
 *      - Update harq info with values indicating new HARQ transmission.
 *           
 *  @param[in,out]  *hqProc
 *  @param[in]      *alloc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUhmNewTx
(
RgSchUlHqProcCb *hqProc,
U8              maxHqRetx,
RgSchUlAlloc    *alloc
)
#else
PUBLIC Void rgSCHUhmNewTx(hqProc, maxHqRetx, alloc)
RgSchUlHqProcCb *hqProc;
U8              maxHqRetx;
RgSchUlAlloc    *alloc;
#endif
{
   TRC2(rgSCHUhmNewTx);

   hqProc->ndi ^= 1;
   hqProc->alloc = alloc;
   hqProc->remTx = maxHqRetx;
   hqProc->rcvdCrcInd = FALSE;
   hqProc->rvIdx = 0;
   hqProc->rvIdxPhy.pres = FALSE;
#ifdef LTE_L2_MEAS
   if (hqProc->alloc->ue)
   {
      hqProc->alloc->ue->ul.hqEnt.numBusyHqProcs++;
   }
#endif
   RETVOID;
}  /* rgSCHUhmNewTx */

/**
 * @brief Free an uplink HARQ process.
 *
 * @details
 *
 *     Function: rgSCHUhmFreeProc
 *     
 *     Invoked by: USM
 *
 *     Processing Steps: 
 *      - Set alloc pointer to NULLP
 *           
 *  @param[in]  RgSchUlHqProcCb   *hqProc
 *  @param[in]  RgSchCellCb      *cell
 *  @return  Void
 **/
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC Void rgSCHUhmFreeProc
(
RgSchUlHqProcCb *hqProc,
RgSchCellCb      *cell
)
#else
PUBLIC Void rgSCHUhmFreeProc(hqProc, cell)
RgSchUlHqProcCb *hqProc;
RgSchCellCb      *cell;
#endif
#else
#ifdef ANSI
PUBLIC Void rgSCHUhmFreeProc
(
RgSchUlHqProcCb *hqProc
)
#else
PUBLIC Void rgSCHUhmFreeProc(hqProc)
RgSchUlHqProcCb *hqProc;
#endif
#endif
{
#ifdef LTEMAC_SPS
   RgSchCmnUlUeSpsInfo   *ulSpsUe = NULLP;
#endif
#ifdef LTE_L2_MEAS
   RgSchUeCb         *ueCb;
   U8 qci = 1;
#endif
   TRC2(rgSCHUhmFreeProc);
#ifdef LTEMAC_SPS
   /*For SPS allocations, harq proc will not be linked to
    * alloc at the time of allocation from 2nd occassion onwards and hence harq
    * proc passed to this function may be NULLP*/
   if(hqProc == NULLP)
   {
      RETVOID;
   }
   else
   {
      RgSchUeCb         *ue;
      /* Check if this Harq proc was used for sending UL SPS
         Actvn PDCCH. In this case set pdcchToRetx to TRUE
         to ensure resending of UL SPS Actvn PDCCH */
      if (hqProc->alloc)
      {
         ue = hqProc->alloc->ue;
         if (ue && (TRUE == hqProc->isSpsActvnHqP))
         {
            ulSpsUe = RG_SCH_CMN_GET_UL_SPS_UE(ue);
            if (ulSpsUe->state == RGSCH_SPS_ACTV_PDCCH_SENT)
            {
               ulSpsUe->pdcchToRetx = TRUE;
               ulSpsUe->actPdcchCrcFailCount++;
            }
         }
      }
   }
   hqProc->isSpsActvnHqP = FALSE;
   hqProc->isSpsOccnHqP = FALSE;
#endif

#ifdef LTE_L2_MEAS
   if (hqProc->alloc && hqProc->alloc->ue)
   {
      ueCb = hqProc->alloc->ue;
      if (ueCb && cell)
      {
         U32 nonLcg0ReportedBs = ((RgSchCmnLcg *)(ueCb->ul.lcgArr[1].sch))->reportedBs + \
                               ((RgSchCmnLcg *)(ueCb->ul.lcgArr[2].sch))->reportedBs + \
                               ((RgSchCmnLcg *)(ueCb->ul.lcgArr[3].sch))->reportedBs;
         ueCb->ul.hqEnt.numBusyHqProcs--;
         if (!ueCb->ul.hqEnt.numBusyHqProcs && !(nonLcg0ReportedBs))
         {
            while (ueCb->ulActiveLCs)
            {
               if (ueCb->ulActiveLCs & 0x1)
               {
                  cell->qciArray[qci].ulUeCount--;
               }
               qci++;
               ueCb->ulActiveLCs >>= 1;
            }
         }
      }
   }
#endif
   hqProc->alloc = NULLP;
   hqProc->ulSfIdx = RGSCH_INVALID_INFO;
   /*ccpu00116293 - Correcting relation between UL subframe and DL subframe based on RG_UL_DELTA*/
   hqProc->isRetx  = FALSE;
   RETVOID;
}  /* rgSCHUhmFreeProc */

/**
 * @brief Handler for HARQ processing on recieving re-trasmission 
 * indication from USM.
 *
 * @details
 *
 *     Function: rgSCHUhmRetx
 *     
 *     Invoked by: USM
 *
 *     Processing Steps: 
 *      - Update harq info with values corresponding to
 *        re-transmission. 
 *           
 *  @param[in,out]  *hqProc
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUhmRetx
(
RgSchUlHqProcCb *hqProc,
RgSchUlAlloc    *alloc
)
#else
PUBLIC Void rgSCHUhmRetx(hqProc, alloc) 
RgSchUlHqProcCb *hqProc;
RgSchUlAlloc    *alloc;
#endif
{
   TRC2(rgSCHUhmRetx);

   hqProc->alloc = alloc;
   --hqProc->remTx;
   hqProc->rvIdx = (hqProc->rvIdx + 1) % 4;
   hqProc->rvIdxPhy.pres = FALSE;
   RETVOID;
}  /* rgSCHUhmRetx */


/**
 * @brief Handler for initializing the HARQ entity.
 *
 * @details
 *
 *     Function: rgSCHUhmRgrUeCfg
 *     
 *     Invoked by: RGR
 *
 *     Processing Steps: 
 *      -  Initialize maxHqRetx
 *           
 *  @param[in]      *cellCb
 *  @param[in,out]  *ueCb
 *  @param[in]      *ueCfg
 *  @param[out]     *err
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUhmRgrUeCfg
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb,
RgrUeCfg          *ueCfg
)
#else
PUBLIC Void rgSCHUhmRgrUeCfg(cellCb, ueCb, ueCfg) 
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
RgrUeCfg          *ueCfg;
#endif
{
   U8 i;

   TRC2(rgSCHUhmRgrUeCfg);

   ueCb->ul.hqEnt.maxHqRetx = (ueCfg->ueUlHqCfg.maxUlHqTx - 1); 
#ifdef TFU_UPGRADE
   /* Storing the delta HARQ offset for HARQ + PUSCH */
   ueCb->ul.betaHqOffst = ueCfg->puschDedCfg.bACKIdx;
#endif
   for(i=0; i < ueCb->ul.hqEnt.numHqPrcs; i++)
   {
      ueCb->ul.hqEnt.hqProcCb[i].procId = i;
      ueCb->ul.hqEnt.hqProcCb[i].ulSfIdx = RGSCH_INVALID_INFO;
      ueCb->ul.hqEnt.hqProcCb[i].alloc = NULLP;
#ifdef LTEMAC_SPS
      /* ccpu00139513- Initializing SPS flags*/
      ueCb->ul.hqEnt.hqProcCb[i].isSpsActvnHqP = FALSE;
      ueCb->ul.hqEnt.hqProcCb[i].isSpsOccnHqP = FALSE;
#endif
   }

   RETVOID;
}  /* rgSCHUhmRgrUeCfg */

/**
 * @brief Handler for re-initializing the HARQ entity.
 *
 * @details
 *
 *     Function: rgSCHUhmRgrUeRecfg
 *     
 *     Invoked by: RGR
 *
 *     Processing Steps: 
 *      -  Re-initialize maxHqRetx
 *           
 *  @param[in]      *cellCb
 *  @param[in,out]  *ueCb
 *  @param[in]      *ueCfg
 *  @param[out]     *err
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUhmRgrUeRecfg
(
RgSchCellCb       *cellCb,
RgSchUeCb         *ueCb,
RgrUeRecfg        *ueRecfg
)
#else
PUBLIC Void rgSCHUhmRgrUeRecfg(cellCb, ueCb, ueRecfg) 
RgSchCellCb       *cellCb;
RgSchUeCb         *ueCb;
RgrUeRecfg        *ueRecfg;
#endif
{
   TRC2(rgSCHUhmRgrUeRecfg);

   /* [ccpu00123958]-ADD- Check for HARQ Reconfig from the bit mask  */
   if(RGR_UE_ULHARQ_RECFG & ueRecfg->ueRecfgTypes)
   {
      ueCb->ul.hqEnt.maxHqRetx = (ueRecfg->ueUlHqRecfg.maxUlHqTx - 1); 
   }

   RETVOID;
}  /* rgSCHUhmRgrUeRecfg */

/**
 * @brief Handler for de-initializing the HARQ entity.
 *
 * @details
 *
 *     Function: rgSCHUhmFreeUe
 *     
 *     Invoked by: RGR
 *
 *     Processing Steps: 
 *      - 
 *           
 *  @param[in,out]  *ueCb
 *  @return  Void
 **/
#ifdef ANSI
PUBLIC Void rgSCHUhmFreeUe
(
RgSchUeCb         *ueCb
)
#else
PUBLIC Void rgSCHUhmFreeUe(ueCb) 
RgSchUeCb         *ueCb;
#endif
{
   TRC2(rgSCHUhmFreeUe);
#ifdef LTE_TDD
   /* ccpu00117052 - MOD - Passing double pointer
   for proper NULLP assignment*/
   rgSCHUtlFreeSBuf(ueCb->cell->instIdx, 
                  (Data **)(&(ueCb->ul.hqEnt.hqProcCb)),
                  ueCb->ul.hqEnt.numHqPrcs * sizeof(RgSchUlHqProcCb));
#endif

   RETVOID;
}  /* rgSCHUhmFreeUe */


/**
* @brief Handler for appending the PHICH information in to the dlSf.
*
* @details
*
*     Function: rgSCHUhmAppendPhich
*     
*     Invoked by: TOM
*
*     Processing Steps:
*      - Set PhichInfo in DlSf for each Hq
*
*  @param[in] *RgSchCellCb
*  @param[in] frm
*  @return  S16
*      -# ROK 
*      -# RFAILED
*/
#ifdef LTE_TDD
#ifdef ANSI
PUBLIC S16 rgSCHUhmAppendPhich
(
RgSchCellCb            *cellCb,
CmLteTimingInfo        frm,
U8                     idx
)
#else
PUBLIC S16 rgSCHUhmAppendPhich (cellCb, frm, idx)
RgSchCellCb            *cellCb;
CmLteTimingInfo        frm;
U8                     idx;
#endif
#else /* LTE TDD */
#ifdef ANSI
PUBLIC S16 rgSCHUhmAppendPhich
(
RgSchCellCb            *cellCb,
CmLteTimingInfo        frm
)
#else
PUBLIC S16 rgSCHUhmAppendPhich (cellCb, frm)
RgSchCellCb            *cellCb;
CmLteTimingInfo        frm;
#endif
#endif
{
   U8              nDmrs;
   U8              rbStart;
#ifdef LTE_TDD
   /* Changes for storing and passing iPhich value at
    * TFU interface*/
   U8              iPhich;
#else
   U8              idx=0;
#endif
   RgSchUlAlloc    *ulAlloc;
#ifdef LTEMAC_HDFDD
   Bool            allwNack = TRUE;
#endif /* LTEMAC_HDFDD */
   RgSchCmnUlCell  *cellUl = RG_SCH_CMN_GET_UL_CELL(cellCb);

   TRC2(rgSCHUhmAppendPhich)

   if(cellUl->hqFdbkIdx[idx] != RGSCH_INVALID_INFO)
   {
      ulAlloc = rgSCHUtlFirstHqFdbkAlloc (cellCb, idx);
   while (ulAlloc)
   {
      /*ccpu00106104 MOD added check for ACKNACK rep*/
      /*added check for acknack so that adaptive retx considers ue 
       inactivity due to ack nack repetition*/
      if((ulAlloc->ue != NULLP) && ((TRUE != ulAlloc->forMsg3) &&
                ((ulAlloc->ue->measGapCb.isMeasuring == TRUE) ||
                        (ulAlloc->ue->ackNakRepCb.isAckNakRep == TRUE))))
      {
         /* Mark the UE for retransmission */
      /* If UE is measuring then we should not be sending PHICH unless msg3 */
      /*UE assumes ack, if nack then do adaptive re-transmission*/
         /*ulAlloc->hqProc->rcvdCrcInd = FALSE;--*/
            ulAlloc = rgSCHUtlNextHqFdbkAlloc (cellCb, ulAlloc, idx);
         continue;
      }
#ifdef LTE_TDD
      if (rgSCHUtlGetPhichInfo (ulAlloc->hqProc, &rbStart, &nDmrs, &iPhich) != ROK)
#else
      if (rgSCHUtlGetPhichInfo (ulAlloc->hqProc, &rbStart, &nDmrs) != ROK)
#endif
      {
         RETVALUE (RFAILED);
      }
      if(nDmrs != RGSCH_INVALID_NDMRS)
      {
         if(cellCb->dynCfiCb.switchOvrInProgress)
         {
           ulAlloc->hqProc->rcvdCrcInd = TRUE;
         }    

         if(ulAlloc->hqProc->rcvdCrcInd) 
         {
#ifdef LTE_TDD
            rgSCHUtlAddPhich (cellCb, frm, TRUE, nDmrs, rbStart, iPhich);
#else
            rgSCHUtlAddPhich (cellCb, frm, TRUE, nDmrs, rbStart, ulAlloc->forMsg3);
#endif
         }
           /* Sending NACK in PHICH for failed UL TX */
         else
         {
#ifdef LTE_TDD
            rgSCHUtlAddPhich (cellCb, frm, FALSE, nDmrs, rbStart, iPhich);
#else
#ifdef LTEMAC_HDFDD
            if (ulAlloc->ue != NULLP && ulAlloc->ue->hdFddEnbld)
            {
               rgSCHCmnHdFddChkNackAllow( cellCb, ulAlloc->ue, frm, &allwNack);
               /* Present implementaion of non-HDFDD does not send phich req
                  incase of NACK. So commented this part to maintain same right
                  now.*/

               if (allwNack)
               {
                  rgSCHUtlAddPhich (cellCb, frm, FALSE, nDmrs, rbStart, ulAlloc->forMsg3);
               }
               else
               {
                  rgSCHUtlAddPhich (cellCb, frm, TRUE, nDmrs, rbStart, ulAlloc->forMsg3);
               }
            }
            else
            {
               rgSCHUtlAddPhich (cellCb, frm, FALSE, nDmrs, rbStart, ulAlloc->forMsg3);
            }
#else
            rgSCHUtlAddPhich (cellCb, frm, FALSE, nDmrs, rbStart, ulAlloc->forMsg3);
#endif/* LTEMAC_HDFDD */
#endif
         }
      }
         ulAlloc = rgSCHUtlNextHqFdbkAlloc (cellCb, ulAlloc, idx);
      }
   }
   RETVALUE(ROK);
} /* rgSCHUhmAppendPhich */

/**
 * @brief This function initializes the DL HARQ Entity of UE.
 *
 * @details
 *
 *     Function: rgSCHUhmHqEntInit
 *     Purpose:  This function initializes the UL HARQ Processes of 
 *               UE control block. This is performed at the time
 *               of creating UE control block.
 *     
 *     Invoked by: configuration module
 *     
 *  @param[in]  RgSchUeCb*    ueCb
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSCHUhmHqEntInit
(
RgSchUeCb           *ueCb
)
#else
PUBLIC S16 rgSCHUhmHqEntInit(ueCb)
RgSchUeCb           *ueCb;
#endif
{
#ifdef LTE_TDD   
   Inst              inst = ueCb->cell->instIdx;
#endif
   TRC2(rgSCHUhmHqEntInit)

#ifndef LTE_TDD
   /* Init the HARQ processes */
   ueCb->ul.hqEnt.numHqPrcs = RGSCH_NUM_UL_HQ_PROC;
#else
   /* Init the HARQ processes */
   ueCb->ul.hqEnt.numHqPrcs = 
                        rgSchTddUlNumHarqProcTbl[ueCb->cell->ulDlCfgIdx];
   if (rgSCHUtlAllocSBuf(inst, (Data **)&ueCb->ul.hqEnt.hqProcCb, 
                           ueCb->ul.hqEnt.numHqPrcs * \
                           sizeof(RgSchUlHqProcCb)) != ROK)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);
} /* rgSCHUhmHqEntInit */



/**********************************************************************
 
         End of file:     gk_sch_uhm.c@@/main/2 - Sat Jul 30 02:21:52 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     sd    1. LTE MAC 2.1 release
           rg003.201 ns    1. Trace added to functions.
           rg005.201 sm    1. Renamed rcvdDatInd to rcvdCrcInd
           rg006.201 ms    1. ccpu00111742: Sending NACK in PHICH for failed UL TX
           rg008.201 rsharon    2. Added support for SPS.
           rg009.201 dv   1. Added changes of TFU_UPGRADE.
                          2. ccpu00116309:Changes for storing and passing iPhich at TFU
                          interface.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---     rt      1. LTE MAC 4.1 release
*********************************************************************91*/
