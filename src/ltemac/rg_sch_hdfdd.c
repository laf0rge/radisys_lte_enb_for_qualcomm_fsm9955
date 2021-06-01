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
 
     Desc:     C source code for HD-FDD functions
 
     File:     rg_sch_hdfdd.c
 
     Sid:      gk_sch_hdfdd.c@@/main/1 - Sat Jul 30 02:21:42 2011
 
     Prg:      rbabu
 
**********************************************************************/

/** @file rg_sch_hdfdd.c
@brief This module handles the  Periodic CQI/PMI/RI, SRS, SR and Half Duplex 
       functionality 
*/
#ifdef LTEMAC_HDFDD
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=165;
#endif

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch_err.h"
#include  "rgr.h"
#include  "rgm.h"
#include "rg_sch.h"
#include "rg_sch_cmn.h"
#include "rl_interface.h"
#include "rl_common.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common tokens */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"    /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */
#include "rg_sch_cmn.x"


#ifdef LTEMAC_HDFDD
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* @details
 *
 *     Function : rgSCHHdFddUeCfg 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at UE Re/configuration. 
 *        
 *     Processing Steps:
 *     - For UE-specific Half Duplex  
 *      - Allocate the memory and place the UE in cellCb->hdUeLstCp 
 *        - Update subframes information state to defualt
 *        - Update subframes information sfn to defualt
 *     - Return ROK
 *
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *  @param[in]  Bool         *hdFddEnbl
 *
 *  @RETVALUE S16
 *      -# ROK
 *      -# RFAILED
*/ 

#ifdef ANSI
PUBLIC S16 rgSCHHdFddUeCfg 
(
RgSchCellCb  *cellCb,
RgSchUeCb *ueCb,
Bool hdFddEnbl
)
#else /* ANSI */
PUBLIC S16 rgSCHHdFddUeCfg (cellCb, ueCb, hdFddEnbl)
RgSchCellCb  *cellCb;
RgSchUeCb *ueCb;
Bool hdFddEnbl;
#endif /* ANSI */
{
   U8 sfi;
   TRC3(rgSCHHdFddUeCfg)

   RLOG_ARG2(L_DEBUG,DBG_CELLID,cellCb->cellId,
               "rgSCHHdFddUeCfg(): UeId =%d hdFddEnbl=%d",
               ueCb->ueId, hdFddEnbl);
   if(ueCb->hdFddEnbld == TRUE)
   {
      if (hdFddEnbl == FALSE)
      {
         /* Do not allow switch from HD-FDD to FD-FDD configuration */
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
               "rgSCHHdFddUeCfg(): HD-FDD  to FD-FDD Configuration is not allowed"
               "CRNTI:%d",ueCb->ueId);
      }
      else
      {
         /* If already enabled then it can be second reconfiguration */
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
                  "rgSCHHdFddUeCfg(): HD-FDD already enabled for this UE"
                  "CRNTI:%d",ueCb->ueId);
      }
      RETVALUE(RFAILED);
   }

#ifdef LTEMAC_SPS
   /* Check is SPS enabled for this UE */
   if(hdFddEnbl == TRUE &&
       (ueCb->ul.ulSpsCfg.isUlSpsEnabled == TRUE ||
        ueCb->dl.dlSpsCfg.isDlSpsEnabled == TRUE))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
      "rgSCHHdFddUeCfg(): Could'nt do HDFDD cfg, SPS already configured"
               "CRNTI:%d",ueCb->ueId);
      RETVALUE(RFAILED);
   }
#endif

   ueCb->hdFddEnbld = hdFddEnbl;
   if( hdFddEnbl == TRUE)
   {
      rgSCHUtlAllocSBuf(cellCb->instIdx,(Data **) &ueCb->hdFddCb,
             sizeof(RgSchUeHdFddCb));
      if (ueCb->hdFddCb != NULLP)
      {
         for (sfi = 0; sfi < RG_SCH_HDFDD_NUMSFINFO; sfi++)
         {
            ueCb->hdFddCb->subfrm[sfi].subFrmDir = RG_SCH_HDFDD_NOSCHD;
            ueCb->hdFddCb->subfrm[sfi].sfn      = RG_SCH_HDFDD_INVSFN;
         }
         /* Add this UE to list maintained in CellCb */
         /* cmLListAdd2Tail(&cellCb->hdFddLst,&ueCb->hdFddCb->hdFddLstEnt); */
      }
      else
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
               "rgSCHHdFddUeCfg(): Could not allocate memory for hd-fdd ueCb"
               "CRNTI:%d",ueCb->ueId);
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}/*rgSCHHdFddUeCfg*/


/* @brief Frees Half Duplex  related data structures
 *
 * @details
 *
 *     Function : rgSCHHdFddUeDel
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by SCH_GOM at Ue deletion.
 *
 *     Processing Steps:
 *       - if Half Duplex is enabled
 *       - if (ueCb->hdFddCb != NULL)
 *        - Remove ue from  cellCb->hdUeLstCp;
 *        - Dellocate memory 
 *       - else
 *        - Nothing to do
 *      - Return ROK
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *
 *  @RETVALUE( S16
 *      -# ROK
*
*/
#ifdef ANSI
PUBLIC S16 rgSCHHdFddUeDel 
(
RgSchCellCb *cellCb,
RgSchUeCb   *ueCb
)
#else /* ANSI */
PUBLIC S16 rgSCHHdFddUeDel(cellCb, ueCb)
RgSchCellCb *cellCb;
RgSchUeCb   *ueCb;
#endif /* ANSI */
{
 TRC3(rgSCHHdFddUeDel)

   RLOG_ARG2(L_DEBUG,DBG_CELLID,cellCb->cellId,
             " rgSCHHdFddUeDel(): UeId =%d hdFdd=%x",
             ueCb->ueId, ueCb->hdFddEnbld);


    if (ueCb->hdFddCb)
    {
       /* ccpu00117052 - MOD - Passing double pointer
       for proper NULLP assignment*/
       rgSCHUtlFreeSBuf(cellCb->instIdx, (Data **)(&(ueCb->hdFddCb)),
             sizeof(RgSchUeHdFddCb));   
       ueCb->hdFddEnbld = FALSE;
    }

    RETVALUE(ROK);
} /* rgSCHHdFddUeDel */



#ifdef TFU_UPGRADE
/* @brief Mark the subframes as uplink for HD FDD if CQI/RI or SRS or RI is
 * expecting .
 *
 * @details
 *
 *     Function: rgSCHCmnHdFddPtUlMrk 
 *     Purpose:  Updation of Periodic CQI/PMI, SRS and SR tranmission
 *               instance updates
 *               for HD FDD UEs
 *  @param[in]  RgSchCellCb *cell
 *  @RETVALUE   None
 */

#ifdef ANSI
PUBLIC Void rgSCHCmnHdFddPtUlMrk 
(
RgSchCellCb *cellCb
)
#else /* ANSI */
PUBLIC Void rgSCHCmnHdFddPtUlMrk (cellCb)
RgSchCellCb *cellCb;
#endif /* ANSI */
{
   U16                    sfn; /* System Frame Number */
   U32                    pti; /* Index into Periodic table */
   U16                    sfi; /* Index into HDFDD state table */
   CmLListCp              *cqiLst;
   CmLListCp              *srsLst;
   CmLListCp              *srLst;
   CmLListCp              *riLst;
   CmLList                *cqiNode;
   CmLList                *srsNode;
   CmLList                *srNode;
   CmLList                *riNode;
   CmLteTimingInfo        timeInfo;

   TRC3(rgSCHCmnHdFddPtUlMrk)

   timeInfo = cellCb->crntTime;

   /* Determine indexes */
   pti = RG_SCH_HDFDD_GETPTI(timeInfo);
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_HDFDD_DELTA);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_HDFDD_DELTA);

   /* Get PT entries for */
   cqiLst = &cellCb->pCqiSrsSrLst[pti].cqiLst;
   srsLst = &cellCb->pCqiSrsSrLst[pti].srsLst;
   srLst  = &cellCb->pCqiSrsSrLst[pti].srLst;
   riLst  = &cellCb->pCqiSrsSrLst[pti].riLst;

   /* Get first node in each list */
   CM_LLIST_FIRST_NODE(cqiLst, cqiNode);
   CM_LLIST_FIRST_NODE(srsLst, srsNode);
   CM_LLIST_FIRST_NODE(riLst,  riNode);
   CM_LLIST_FIRST_NODE(srLst,  srNode);

   /* Mark corresponding the subframe as uplink control */
   while ((NULLP != cqiNode ) &&
         (NULLP != srsNode  ) &&
         (NULLP != srNode   )  &&
         (NULLP != riNode   ))
   {
      /* CQI/PMI Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK((RgSchUeCb*)cqiNode->node,
                     RG_SCH_HDFDD_UL, sfn, sfi);
      /* SRS Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK((RgSchUeCb*)srsNode->node,
                     RG_SCH_HDFDD_UL, sfn, sfi);
      /* SR Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK((RgSchUeCb*)srNode->node,
                     RG_SCH_HDFDD_UL, sfn, sfi);
      /* RI Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK((RgSchUeCb*)riNode->node,
                     RG_SCH_HDFDD_UL, sfn, sfi);

      /* Get next UeCb for all lists */
      CM_LLIST_NEXT_NODE(cqiLst, cqiNode);
      CM_LLIST_NEXT_NODE(srsLst, srsNode);
      CM_LLIST_NEXT_NODE(srLst, srNode);
      CM_LLIST_NEXT_NODE(riLst, riNode);
   }

   while ( NULLP != cqiNode)
   {
      /* CQI/PMI Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK(((RgSchUeCb*)cqiNode->node),
                     RG_SCH_HDFDD_UL, sfn, sfi);
      CM_LLIST_NEXT_NODE(cqiLst, cqiNode);
   }
   while( NULLP != srsNode)
   {
      /* SRS Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK(((RgSchUeCb*)srsNode->node),
                     RG_SCH_HDFDD_UL, sfn, sfi);
      CM_LLIST_NEXT_NODE(srsLst, srsNode);
   }
   while( NULLP != srNode)
   {
      /* SR Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK(((RgSchUeCb*)srNode->node),
                     RG_SCH_HDFDD_UL, sfn, sfi);
      CM_LLIST_NEXT_NODE(srLst, srNode);
   }
   while( NULLP != riNode)
   {
      /* RI Transmission instances */
      RG_SCH_HDFDD_VLDTANDMARK(((RgSchUeCb*)riNode->node),
                     RG_SCH_HDFDD_UL, sfn, sfi);
      CM_LLIST_NEXT_NODE(riLst, riNode);
   }

   RETVOID;
} /* rgSCHCmnHdFddPtUlMrk */
#endif /* ifdef TFU_UPGRADE */



/* @brief Decides whether UE can be allowed for DL in given subframe
 *
 * @details
 *
 *     Function : rgSCHCmnHdFddChkUlAllow 
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by schedulars before allocating  UL grants .
 *
 *     Processing Steps:
 *       - if Half Duplex is enabled
 *       - If ue->sf[reqsf].state is "DONWLINK"
 *            set alloweUlSch=FALSE
 *       - else
 *            set alloweUlSch=TRUE
 *        This function Marking for BCCH/PCCH occasions is also done
 *      - Return ROK
 *  @param[in]  RgSchCellCb  *cell
 *  @param[in]  RgSchUeCb    *ue
 *
 *  @RETVALUE   None
 *
 */
#ifdef ANSI
PUBLIC Void rgSCHCmnHdFddChkUlAllow
( 
 RgSchCellCb *cellCb,
 RgSchUeCb   *ueCb,
 U8          *allow
)
#else /* ANSI */
PUBLIC Void rgSCHCmnHdFddChkUlAllow ( cellCb, ueCb, allow)
RgSchCellCb *cellCb;
RgSchUeCb   *ueCb;
U8          *allow;
#endif /* ANSI */
{
   U16              sfn;
   U16              sfi;
   CmLteTimingInfo  timeInfo;
   RgSchDlSf        *sf = NULLP; /* Dl subframe info */
   U8               ulOffset

   TRC3(rgSCHCmnHdFddChkUlAllow)
   
   RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId, 
            " rgSCHCmnHdFddChkUlAllow: ueId=%d ", ueCb->ueId);

   *allow = FALSE;

   timeInfo = cellCb->crntTime;

   ulOffset = RGSCH_PDCCH_PUSCH_DELTA - 
          TFU_CRCIND_ULDELTA + RGSCH_PDCCH_PUSCH_DELTA;
   RG_SCH_ADD_TO_CRNT_TIME(cellCb->crntTime, timeInfo, ulOffset);

   /* Set default value */
   *allow = FALSE;

   /* Validate condition 1 */
   /* For (curretn time + DL_DELTA)th sf */

   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, timeInfo);

   sfn = timeInfo.sfn;
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, 0);

   /* Validate condition 2 */
   if (RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
         /* Common channel scheduled */
         /* Mark the BCCH/PCCH occasion */
         RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, sfn, sfi);
         RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,  
            "rgSCHCmnHdFddChkUlAllow: Already marked for Cmn DL, ueId = %d ",
            ueCb->ueId);
   }
   if ((ueCb->hdFddCb->subfrm[sfi].sfn == sfn) &&
         (ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_DLDATA ||
          ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_DLCNTRL))
   {
      /* Downlink scheduled */
      *allow = FALSE;
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,  
               "rgSCHCmnHdFddChkUlAllow: Already marked for DL, ueId = %d ",
               ueCb->ueId);
      RETVOID;
   }

   /* Validate condition 3 */
   /* For (curretn time + DL_DELTA + HRQ_DELTA)th sf
      - i.e. next HARQ Feedback occasion */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
       ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_UL)
   {
      /* No place for HARQ feedback */
      *allow = FALSE;
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,  
         "rgSCHCmnHdFddChkUlAllow: No Place for HARQ, ueId = %d ",
         ueCb->ueId);
      RETVOID;

   }
   /* Validate condition 4 */
   /* For (curretn time + DL_DELTA - HRQ_DELTA)th sf
      - i.e. previous HARQ Feedback occasion */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, (-RG_SCH_CMN_HARQ_INTERVAL));
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, (-RG_SCH_CMN_HARQ_INTERVAL));
   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
       ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_UL)
   {
      *allow = FALSE;
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,  
               " rgSCHCmnHdFddChkUlAllow: No Place for UL grant, ueId = %d ",
               ueCb->ueId);
      RETVOID;

   }
   /* Validate condition 5 */
   /* For (curretn time + DL_DELTA - 1)th sf -i.e. Guard time */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, (-RG_SCH_HDFDD_GRDTIM_DUR));
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, (-RG_SCH_HDFDD_GRDTIM_DUR));
   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
       (ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_DLDATA))
   {
      /* This subframe may be a switching gaurd time */
      *allow = FALSE;
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,  
             " rgSCHCmnHdFddChkUlAllow: No Place for Guard time, ueId = %d ",
             ueCb->ueId);
      RETVOID;

   }
   /* Adition guard time rule check: Above check is only for PDSCH, lets check
      is there is any BCCH/PCCH data scheduled */
   RG_SCH_ADD_TO_CRNT_TIME(cellCb->crntTime, timeInfo, 
                      (ulOffset - RG_SCH_HDFDD_GRDTIM_DUR));
   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, timeInfo);
   if (RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
      /* Common channel scheduled */
      /* Mark the BCCH/PCCH occasion */
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, timeInfo.sfn, sfi);
      *allow = FALSE;
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,  
         "rgSCHCmnHdFddChkUlAllow: Already marked for Cmn DL, ueId = %d ",
         ueCb->ueId);
      RETVOID;

   }

   /* All validation done. Safe to for UL */
   *allow = TRUE;
   RETVOID;
} /* rgSCHCmnHdFddChkUlAllow */


 /* @brief Decides whether UE can be allowed for UL in given subframe
 *
 * @details
 *
 *     Function : rgSCHCmnHdFddChkDlAllow
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by schedulars before allocating for DL.
 *
 *     Processing Steps:
 *          Condition 1:  subframe n + DL_DELTA should not be uplink
 *          Condition 2:  subframe n+ DL_DELTA + 1 should meet guard time
 *                        creation rule. For more
 *                        information refer to section "2.25.7.1 Guard time
 *                        creation rule"
 *          Condition 3:  subframe n + DL_DELTA + HRQ_DELTA should not be
 *                        downlink so that downlink data (HARQ Feedback)
 *                        can be received in next 4 subframe. {n + 7} Above
 *                        conditions have to
 *                        be validated by taking SFN number into consideration.
 *          if all conditions are met then *allow is set to TRUE or lese to
 *          FALSE.
 *          if hd-fdd is not anabled for this UE, then *allow is always TRUE.
 *   
 *   Returns None
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgSchUeCb    *ueCb
 *  @param[in]  CmLteTimingInfo    *timeInfo
 *  @param[out] U8 *allow -- TRUE is allowed or FALSE if no allowedi.
 *                   Valdity of this pointer is not done in this function
 *
 */
#ifdef ANSI
PUBLIC Void rgSCHCmnHdFddChkDlAllow 
(
RgSchCellCb *cellCb,
RgSchUeCb *ueCb,
Bool *allow /* Valdity of this pointer is not done in this function */
)
#else /* ANSI */
PUBLIC Void rgSCHCmnHdFddChkDlAllow ( cellCb, ueCb, allow)
RgSchCellCb *cellCb;
RgSchUeCb *ueCb;
Bool *allow; /* Valdity of this pointer is not done in this function */
#endif /* ANSI */
{
   U16     sfn;
   U16     sfi;
   RgSchDlSf *sf = NULLP; /* Dl subframe info */
   CmLteTimingInfo timeInfo;
   CmLteTimingInfo tempTimeInfo;

   TRC3(rgSCHCmnHdFddChkDlAllow)

   *allow = FALSE;

   timeInfo = cellCb->crntTime;
   RGSCH_INCR_SUB_FRAME(timeInfo, RG_SCH_CMN_DL_DELTA);

   RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,  
            "rgSCHCmnHdFddDlSchAll (): ueId=%d ", ueCb->ueId);

   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, timeInfo);

   /* Validate condition 1 */
   /* For (curretn time + DL_DELTA)th sf */
   sfn = timeInfo.sfn;
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, 0);

   if ((ueCb->hdFddCb->subfrm[sfi].sfn == sfn) &&
        (ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_UL))
   {
      /* Uplink scheduled */
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,  
               "rgSCHCmnHdFddChkDlAllow: sf is UL, ueId=%d ", ueCb->ueId);
      *allow = FALSE;
      RETVOID;
   }

   /* It is not validation, but BCCH/PCCH marking is done here */
   if (RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
      /* Common channel scheduled */
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, sfn, sfi); /* NOT_HIT */
   }

   /* Validate condition 2 */
   /* For (curretn time + DL_DELTA + 1)th sf -i.e. Guard time */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_HDFDD_GRDTIM_DUR);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_HDFDD_GRDTIM_DUR);
   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
          (ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_UL))
   {
      /* This subframe may be a switching guard time */
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,  
              " rgSCHCmnHdFddChkDlAllow: Guard time rule not met, ueId=%d ",
              ueCb->ueId);
      *allow = FALSE;
      RETVOID;
   }

   /* Validate condition 3 */
   /* For (curretn time + DL_DELTA + HRQ_DELTA)th sf - i.e. next HARQ
      Feedback occasion */

   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);

   /* First check for any Common channel info is scheduled */
   RG_SCH_ADD_TO_CRNT_TIME(timeInfo, tempTimeInfo, RG_SCH_CMN_HARQ_INTERVAL)
   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, tempTimeInfo);
   if (RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
         /* Common channel scheduled */
      /* Do the marking for this subframe */
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, tempTimeInfo.sfn, sfi);
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,  
              "rgSCHCmnHdFddChkDlAllow: Possible systemInfo, ueId=%d ",
              ueCb->ueId);
   }

   /* Check for actual validation condition 3 */
   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
       ueCb->hdFddCb->subfrm[sfi].subFrmDir != RG_SCH_HDFDD_UL)
   {
      /* No place for HARQ feedback */
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,  
            "rgSCHCmnHdFddChkDlAllow: No place for HARQ feedback, ueId=%d ",
            ueCb->ueId);
      *allow = FALSE;

      /* Mark this sf as DLCNTRL */
      ueCb->hdFddCb->subfrm[sfi].subFrmDir =RG_SCH_HDFDD_DLCNTRL;
      RETVOID;
   }


   /* If we are here then, subframe at HARQth location can be UL.
      But check if Guard violation is done */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_CMN_HARQ_INTERVAL-1);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_CMN_HARQ_INTERVAL-1);
   /* check for any Common channel info is scheduled */
   RG_SCH_ADD_TO_CRNT_TIME(timeInfo, tempTimeInfo, (RG_SCH_CMN_HARQ_INTERVAL-1))
   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, tempTimeInfo);
   if (RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
         /* Common channel scheduled */
      /* Do the marking for this subframe */
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, tempTimeInfo.sfn, sfi);
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,
              "rgSCHCmnHdFddChkDlAllow: (GT) Possible systemInfo, ueId=%d ",
              ueCb->ueId);
   }

   if (ueCb->hdFddCb->subfrm[sfi].sfn == sfn &&
       ueCb->hdFddCb->subfrm[sfi].subFrmDir == RG_SCH_HDFDD_DLDATA)
   {
      /* No place for HARQ feedback */
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,
            "rgSCHCmnHdFddChkDlAllow: (GT) No place for HARQ feedback,"
             "ueId=%d ",ueCb->ueId);

      *allow = FALSE;
      RETVOID;
   }
   /* First check for any Common channel info is scheduled */

   *allow = TRUE;
   /* All validation done. Safe to for DL */
   RETVOID;
} /* rgSCHCmnHdFddChkDlAllow */



/* @brief Decides whether NACK can be sent in a given subrame
 *
 * @details
 *
 *     Function : rgSCHCmnHdFddChkNackAllow
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by schedulars.
 *
 *  @param[in]  RgSchUeCb    *ue
 *
 *  @RETVALUE None
 *
 */

#ifdef ANSI
PUBLIC Void rgSCHCmnHdFddChkNackAllow
(
RgSchCellCb *cellCb,
RgSchUeCb   *ueCb,
CmLteTimingInfo  timeInfo,
Bool *sndNACK
)
#else /* ANSI */
PUBLIC Void rgSCHCmnHdFddChkNackAllow(cellCb, ueCb, timeInfo, sndNACK)
RgSchCellCb *cellCb;
RgSchUeCb   *ueCb;
CmLteTimingInfo  timeInfo;
Bool *sndNACK;
#endif /* ANSI */
{
   RgSchDlSf *sf;
   CmLteTimingInfo  tempTimeInfo;

   TRC3(rgSCHCmnHdFddChkNackAllow)

   /* Information in timeInfo contains (n+DL_DELTA) th subframe info*/

   *sndNACK = FALSE;

   /* Determine SFN and sf index for current subframe.
      Note: Round function used as example */
   tempTimeInfo = timeInfo;
   RGSCH_INCR_SUB_FRAME(tempTimeInfo, RG_SCH_CMN_HARQ_INTERVAL);

   /* Also get subframe pointer to fetch Common Ch allocation */
   sf = rgSCHUtlSubFrmGet(cellCb, tempTimeInfo);

   /* Check is this subframe has any Common Channel info scheduled */
   if(RG_SCH_HDFDD_ISCMN_SCHED(sf))
   {
      /* Yes, Cannot send NACK */
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,
               "rgSCHCmnHdFddChkNackAllow: Cannot send NACK, ueId = %d ",
               ueCb->ueId);
      *sndNACK = FALSE;
   }
   else
   {
      /* safe, Send NACK */
      RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,
               "rgSCHCmnHdFddChkNackAllow: NACk can be sent, ueId = %d ",
               ueCb->ueId);
      *sndNACK = TRUE;
   }

   RETVOID;
} /* rgSCHCmnHdFddChkNackAllow */




 /* @brief makes final marking for HD-FDD UL allocations
 *
 * @details
 *
 *     Function : rgSCHCmnHdFddUpdULMark
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by schedulars at the time of UL allocation
 *        finalization.
 *
 *     Processing Steps:
 *   
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgSchUeCb    *ueCb
 *  @param[in]  CmLteTimingInfo    *timeInfo
 *  @param[out] U8 *allow -- TRUE is allowed or FALSE if no allowedi.
 *             Valdity of this pointer is not done in this function.
 *
 *  @RETVALUE None
 */
#ifdef ANSI
PUBLIC Void rgSCHCmnHdFddUpdULMark
(
RgSchCellCb *cellCb,
RgSchUeCb   *ueCb
)
#else /* ANSI */
PUBLIC Void rgSCHCmnHdFddUpdULMark ( cellCb, ueCb)
RgSchCellCb *cellCb;
RgSchUeCb   *ueCb;
#endif /* ANSI */
{

   U16 sfn;
   U16 sfi;
   CmLteTimingInfo  timeInfo;
   U8   ulOffset;

   TRC3(rgSCHCmnHdFddUpdULMark)


   ulOffset = RGSCH_PDCCH_PUSCH_DELTA - 
              TFU_CRCIND_ULDELTA + RGSCH_PDCCH_PUSCH_DELTA;
   RG_SCH_ADD_TO_CRNT_TIME(cellCb->crntTime, timeInfo, ulOffset)


   /* Mark (n + UL_DELTA)th subframe as UL */
   sfn = timeInfo.sfn;
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, 0);

   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_UL, sfn, sfi);

   /* Mark (n + UL_DELTA + HARQ_DELTA)th subframe as DL */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLCNTRL, sfn, sfi);

   /* Mark (n + UL_DELTA - HARQ_DELTA)th subframe as DL */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, (0-RG_SCH_CMN_HARQ_INTERVAL));
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, (0-RG_SCH_CMN_HARQ_INTERVAL));
   if (ueCb->hdFddCb->subfrm[sfi].subFrmDir != RG_SCH_HDFDD_DLDATA)
   {
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLCNTRL, sfn, sfi);
   }

   /* Mark (n + UL_DELTA - 1)th subframe as DL_CNTRL */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, -RG_SCH_HDFDD_GRDTIM_DUR);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, -RG_SCH_HDFDD_GRDTIM_DUR);
   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLCNTRL, sfn, sfi);

   /* Remove marking for older subframes */

   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, (S16)(ulOffset * -1));
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, (ulOffset * -1));
   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_NOSCHD, RG_SCH_HDFDD_INVSFN, sfi);

   RETVOID;
} /* rgSCHCmnHdFddUpdULMark */




 /* @brief makes final marking for HD-FDD DL allocations
 *
 * @details
 *
 *     Function : rgSCHCmnHdFddUpdDLMark
 *
 *     Invoking Module Processing:
 *      - This shall be invoked by schedulars at the time of DL allocation
 *          finalization.
 *
 *     Processing Steps:
 *   
 *  @param[in]  RgSchCellCb  *cellCb
 *  @param[in]  RgSchUeCb    *ueCb
 *  @param[in]  CmLteTimingInfo    *timeInfo
 *  @param[out] U8 *allow -- TRUE is allowed or FALSE if no allowed.
 *                Valdity of this pointer is not done in this function
 *
 *  @RETVALUE None
 */

#ifdef ANSI
PUBLIC Void rgSCHCmnHdFddUpdDLMark
(
RgSchCellCb *cellCb,
RgSchUeCb   *ueCb
)
#else /* ANSI */
PUBLIC Void rgSCHCmnHdFddUpdDLMark (cellCb, ueCb)
RgSchCellCb *cellCb;
RgSchUeCb   *ueCb;
#endif /* ANSI */
{

   U16 sfn;
   U16 sfi;
   CmLteTimingInfo  timeInfo;

   TRC3(rgSCHCmnHdFddUpdDLMark)

   timeInfo = cellCb->crntTime;
   RGSCH_INCR_SUB_FRAME(timeInfo, RG_SCH_CMN_DL_DELTA);

   /* Mark (n + DL_DELTA)th subframe as DL */
   sfn = timeInfo.sfn;
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, 0);

   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLDATA, sfn, sfi);

   /* Mark (n + 1)th subframe as DL_CNTRL */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_HDFDD_GRDTIM_DUR);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_HDFDD_GRDTIM_DUR);
   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLCNTRL, sfn, sfi);

   /* Mark (n + DL_DELTA + HARQ_DELTA )th subframe as UL */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo, RG_SCH_CMN_HARQ_INTERVAL);
   RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_UL, sfn, sfi);

   /* Mark (n + DL_DELTA + HARQ_DELTA - 1)th subframe
      as DL control for Guard period */
   RG_SCH_HDFDD_GETSFN(sfn, timeInfo,
          (RG_SCH_CMN_HARQ_INTERVAL - RG_SCH_HDFDD_GRDTIM_DUR));
   RG_SCH_HDFDD_GETSFI(sfi, timeInfo,
         (RG_SCH_CMN_HARQ_INTERVAL - RG_SCH_HDFDD_GRDTIM_DUR));
   if (ueCb->hdFddCb->subfrm[sfi].subFrmDir != RG_SCH_HDFDD_UL)
   {
      RG_SCH_HDFDD_MARKSTATE(ueCb, RG_SCH_HDFDD_DLCNTRL, sfn, sfi);
   }

   RETVOID;
} /* rgSCHCmnHdFddUpdDLMark */


 /* @brief determines effective SFN
 *
 * @details
 *
 *     Function : rgSCHHdFddGetSfn
 *
 *     Invoking Module Processing:
 *       Any HD-FDD module can invoke this function.
 *
 *     Processing Steps:
 *   
 *  @param[out]  *sfn U32
 *  @param[in]  timeInfo timing information subframe of interest 
 *  @param[in]  offsest  Offest with w.r.t which SFN has to be determined
 *
 *  @RETVALUE None
 */

#ifdef ANSI
PUBLIC Void rgSCHHdFddGetSfn
(
 U16 *sfn,
 CmLteTimingInfo  timeInfo,
 S16 offset
)
#else /* ANSI */
PUBLIC Void rgSCHHdFddGetSfn (sfn, timeInfo, offset)
 U16 *sfn;
 CmLteTimingInfo  timeInfo;
 S16 offset;
#endif /* ANSI */
{
   U16 tempSfn;
   S16 tempSfCount;

   TRC3(rgSCHHdFddGetSfn)
   if(((S16)(timeInfo.subframe) + offset) >= RGSCH_NUM_SUB_FRAMES)
   {
      /* Increament to number of times of SFNs that can be possible
         with this offset */
      tempSfn = (timeInfo.sfn +
                ((timeInfo.subframe + offset) / RGSCH_NUM_SUB_FRAMES))
                & (RGSCH_MAX_SFN -1); /* Mod with MAX SFN supported */
   }
   else
   {
      if(((S16)(timeInfo.subframe) + offset) < 0)
      {
         /* If negative, then definitely at least previous SFN */
         tempSfn = (timeInfo.sfn - 1) & (RGSCH_MAX_SFN -1);

         /* Now find how many more times we need to decreament */
         tempSfCount = timeInfo.subframe + offset;
         RG_SCH_HDFDD_ROLLSFN(tempSfCount, tempSfn);
      }
      else
      {
         /* No change in sfn */
         tempSfn = timeInfo.sfn;
      }
   }
   *sfn = tempSfn;

   RETVOID;
} /* End of rgSCHHdFddGetSfn */


#ifdef __cplusplus
}
 /* extern C */
#endif /* __cplusplus */

#endif /* LTEMAC_HDFDD */




/**********************************************************************
 
         End of file:     gk_sch_hdfdd.c@@/main/1 - Sat Jul 30 02:21:42 2011
 
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
/main/1      ---         gvj 1. Created for LTE MAC Rel 3.1
            rg002.301  asehgal 1.ccpu00118351: seperate UL & DL deltas.
$SID$        ---        rt   1. LTE MAC 4.1 release
*********************************************************************91*/
