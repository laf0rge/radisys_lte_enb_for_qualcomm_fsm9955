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
  
     Desc:     C source code for  L2 Measurements in MAC
  
     File:     rg_l2m.c
  
     Sid:      gk_l2m.c@@/main/1 - Sat Jul 30 02:21:29 2011
  
     Prg:      AP
  
**********************************************************************/

/** @file rg_l2m.c
@brief This file implements the schedulers main access to MAC layer code.
*/

#ifdef LTE_L2_MEAS
static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_FILE_ID=183;
static int RLOG_MODULE_ID=4096;
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
#include "crg.h"        
#include "rgr.h"
#include "rgu.h"           
#include "tfu.h"
#include "rg_env.h"
#include "rg_err.h"
#include "rg_sch_inf.h"
#include "rg.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* TFU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"         /* typedefs for Scheduler */
#include "rg.x"            /* MAC types */

/* local defines */
#ifdef LTE_L2_MEAS
PRIVATE S16 rgL2mInsertMeasCb ARGS((
         RgCellCb       *cell,
         RgL2MeasCb     *measCb,
         RgInfL2MeasReq *measInfo ));

PRIVATE RgL2MeasCb * rgL2mAllocMeasCb ARGS((
         RgCellCb       *cell,
         RgInfL2MeasReq *measInfo,
         RgErrInfo      *err));

/* Function definitions */

/** @brief This function creates the measCb
 *
 * @details
 *
 *     Function: rgL2mCreateMeasCb
 *         Processing steps:
 *         - Check the measType
 *         - Create measCb for every qci
 *
 * @param  [in] RgCellCb       *cell
 * @param  [in] RgInfL2MeasReq *measInfo
 * @param  [in] U8             measType
 * @param  [out] RgErrInfo      *err
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 */
#ifdef ANSI
PUBLIC S16 rgL2mCreateMeasCb 
(
RgCellCb       *cell,
RgInfL2MeasReq *measInfo, 
U8              measType,
RgErrInfo      *err
)
#else
PUBLIC S16 rgL2mCreateMeasCb(cell, measInfo, measType, err)
RgCellCb       *cell;
RgInfL2MeasReq *measInfo; 
U8              measType;
RgErrInfo      *err;
#endif    
{
   U32     idx;
   RgL2MeasCb       *measCb = NULLP;
   U8            qciVal = 0;

   UNUSED(measType);
   UNUSED(err);

   TRC3(rgL2mCreateMeasCb)

      if ((measCb = rgL2mAllocMeasCb(cell, measInfo, err)) == NULLP)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Allocation of RgL2MeasCb failed");
         RETVALUE(RFAILED);
      }
   //Memcpy is already done in rgL2mAllocMeasCb
   /*cmMemcpy((U8 *)&measCb->measReq, (CONSTANT U8 *)measInfo,\
     sizeof(RgInfL2MeasReq));*/
   rgL2mInsertMeasCb(cell, measCb, measInfo);
   measCb->measReq.timePrd = measInfo->timePrd;

   if(measInfo->timePrd == 0)
   {
      cell->sndL2Meas = FALSE;
   }

   for(idx = 0; idx < measInfo->t.prbReq.numQci; idx++)
   {
      if(measInfo->timePrd == 0)
      {
         qciVal = measInfo->t.prbReq.qci[idx];
         cell->qciArray[qciVal].qci = qciVal;
      }
      cell->qciArray[measInfo->t.prbReq.qci[idx]].mask = TRUE;
   }
   RETVALUE(ROK);
} /* rgL2mCreateMeasCb */


/**
 * @brief Layer Manager Measurement request handler. 
 *
 * @details
 *
 *     Function : rgL2mMeasReq
 *     
 *     This function handles  measurement request received at MAC
 *     from the Scheduler.
 *     -# Measurement request will be stored in the list in descending order of
 *     there time period.
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgInfL2MeasReq *measInfo, the measurement request structure
 *  @param[out] RgErrInfo   *err, error information
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgL2mMeasReq 
(
RgCellCb       *cell,
RgInfL2MeasReq *measInfo,
RgErrInfo      *err
)
#else
PUBLIC S16 rgL2mMeasReq(cell, measInfo, err)
RgCellCb       *cell;
RgInfL2MeasReq *measInfo; 
RgErrInfo      *err;
#endif    
{
   S16  ret=RFAILED;

   TRC3(rgL2mMeasReq)
   /* Creaet MeasCb Insert in cell->l2mList and return*/
   if ( (ret = rgL2mCreateMeasCb(cell, measInfo,
               LRG_L2MEAS_AVG_PRB_PER_QCI_UL, err)) != ROK)
   {
      /* Clear Downlink MeasCb created Above If exists*/
      RETVALUE(ret);
   }
   RETVALUE(ROK);
} /* rgL2mMeasReq */
/** @brief This function sends the measurement confirm
 *  from mac to scheduler
 *
 * @details
 *
 *     Function: rgSndL2MeasCfm
 *
 * @param  [in] RgCellCb          *cell
 * @param  [in] RgInfL2MeasCfm    *measCfm
 */
#ifdef ANSI
PRIVATE Void rgSndL2MeasCfm
(
RgCellCb          *cell, 
RgInfL2MeasCfm    *measCfm
)
#else
PRIVATE Void rgSndL2MeasCfm (cell, measCfm)
RgCellCb          *cell; 
RgInfL2MeasCfm    *measCfm;   
#endif
{
   Pst             pst;

   TRC3(rgSndL2MeasCfm)

      cmMemset((U8 *)&pst, 0, sizeof(Pst));
   rgGetPstToInst(&pst, cell->schInstMap.schInst);
   RgMacSchL2Meas(&pst, measCfm);

   RETVOID;
}/* rgSndL2MeasCfm */

/** @brief This function sends the measurement stop confirm
 *  from mac to scheduler
 *
 * @details
 *
 *     Function: rgSndL2MeasStopCfm
 *
 * @param  [in] RgCellCb          *cell
 * @param  [in] RgInfL2MeasCfm    *measCfm
 */
#ifdef ANSI
PRIVATE Void rgSndL2MeasStopCfm
(
RgCellCb          *cell,
RgInfL2MeasCfm    *measCfm
)
#else
PRIVATE Void rgSndL2MeasStopCfm (cell, measCfm)
RgCellCb          *cell;
RgInfL2MeasCfm    *measCfm;
#endif
{
   Pst             pst;

   TRC3(rgSndL2MeasStopCfm)
      cmMemset((U8 *)&pst, 0, sizeof(Pst));
   rgGetPstToInst(&pst, cell->schInstMap.schInst);
   RgMacSchL2MeasStop(&pst, measCfm);

   RETVOID;
}/* rgSndL2MeasStopCfm */

/**
 * @brief  L2 Measurement request handler.This function shall be called by
 *  scheduler to calculate average PRB usage Per Qci in Uplink
 *
 * @details
 *
 *     Function : RgSchMacL2MeasReq
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgInfL2MeasReq *measInfo, L2 Measurement req structure
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 RgSchMacL2MeasReq
(
Pst               *pst,          /* post structure  */
RgInfL2MeasReq    *measInfo      /* Meas Req Info */
)
#else
PUBLIC S16 RgSchMacL2MeasReq(pst, measInfo)
Pst               *pst;          /* post structure  */
RgInfL2MeasReq    *measInfo;      /* Meas Req Info */
#endif    
{
   RgCellCb        *cellCb;
   RgErrInfo       err;
   S16             ret = ROK;
   RgInfL2MeasCfm   measCfm;

   TRC3(RgSchMacL2MeasReq)


   /* Get the  cell using cellId */
   cellCb = rgDBMGetCellCb (measInfo->cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,measInfo->cellId,"unable to get the cellCb");
      RETVALUE(RFAILED);
   }
   /* Call L2M Function to store Meas req */
   ret = rgL2mMeasReq(cellCb, measInfo, &err);
   if (ret != ROK)
   {
      cmMemset((U8 *)&measCfm, 0, sizeof(RgInfL2MeasCfm));
      measCfm.transId     = measInfo->transId;
      measCfm.cellId      = measInfo->cellId;
      measCfm.measType    = measInfo->measType;
      measCfm.cfm.reason   = LCM_REASON_INVALID_PAR_VAL;
      measCfm.cfm.status  = LCM_PRIM_NOK;
      rgSndL2MeasCfm(cellCb, &measCfm);
      RLOG_ARG2(L_ERROR,DBG_CELLID,measInfo->cellId,
               "Meas req Failed  errType(%d) errCause(%d)",
               err.errType, err.errCause);
      RETVALUE(RFAILED);
   }
   RETVALUE(ret);
} /* -- RgSchMacL2MeasReq-- */

/**
 * @brief  L2 Measurement request handler.This function shall be called by
 *         sch to to stop l2 measurement in MAC,
 *
 * @details
 *
 *     Function : RgSchMacL2MeasStopReq
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgInfL2MeasReq *measInfo, L2 Measurement req structure
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 RgSchMacL2MeasStopReq
(
Pst               *pst,          /* post structure  */
RgInfL2MeasStopReq *measInfo      /* Meas Req Info */
)
#else
PUBLIC S16 RgSchMacL2MeasStopReq(pst, measInfo)
Pst               *pst;          /* post structure  */
RgInfL2MeasStopReq *measInfo;      /* Meas Req Info */
#endif
{
   S16             ret = ROK;   
   RgCellCb        *cellCb = NULLP;
   CmLList         *node   = NULLP;
   RgL2MeasCb      *measCb = NULLP;
   U8               idx;
   U8               qciVal;

   RgInfL2MeasCfm  measCfm;

   TRC3(RgSchMacL2MeasStopReq)
      /* Get the  cell using cellId */
      cellCb = rgDBMGetCellCb (measInfo->cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,measInfo->cellId,
            "Unable to get the cellCb");
      RETVALUE(RFAILED);
   }
   node = cellCb->l2mList.first; 
   while(node != NULLP)
   {
      measCb = (RgL2MeasCb *)(node)->node;
      node = (node)->next;
      /*L2 Meas off for qci in cell */
      for(idx = 0; idx < measCb->measReq.t.prbReq.numQci; idx++)
      {
         qciVal = measCb->measReq.t.prbReq.qci[idx];
         cellCb->qciArray[qciVal].mask = FALSE;
      }
      cmLListDelFrm(&cellCb->l2mList, &measCb->measLnk);
      rgFreeSBuf((Data**)&measCb, sizeof(RgL2MeasCb));
   }
   cmMemset((U8 *)&measCfm, 0, sizeof(RgInfL2MeasCfm));
   measCfm.transId     = measInfo->transId;
   measCfm.cellId      = measInfo->cellId;
   measCfm.measType    = measInfo->measType;
   measCfm.cfm.status  = LCM_PRIM_OK;
   rgSndL2MeasStopCfm(cellCb, &measCfm);
   RETVALUE(ret);
} /* -- RgSchMacL2MeasStopReq-- */

/**
 * @brief  L2 Measurement request handler.This function shall be called by
 *  scheduler for  sending L2 meas 
 *
 * @details
 *
 *     Function : RgSchMacL2MeasSendReq
 *     
 *  @param[in]  Pst *pst, the post structure     
 *  @param[in]  RgInfL2MeasReq *measInfo, L2 Measurement req structure
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 RgSchMacL2MeasSendReq
(
Pst               *pst,          /* post structure  */
RgInfL2MeasSndReq *measInfo      /* Meas Req Info */
)
#else
PUBLIC S16 RgSchMacL2MeasSendReq(pst, measInfo)
Pst               *pst;          /* post structure  */
RgInfL2MeasSndReq *measInfo;      /* Meas Req Info */
#endif
{
   S16             ret = ROK;
   RgCellCb        *cellCb = NULLP;

   TRC3(RgSchMacL2MeasSendReq)
      /* Get the  cell using cellId */
      cellCb = rgDBMGetCellCb (measInfo->cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,measInfo->cellId,
            "Unable to get the cellCb");
      RETVALUE(RFAILED);
   }
   /*set sndL2Meas as applicatoin sent l2 meas info request*/
   cellCb->sndL2Meas = TRUE;

   RETVALUE(ret);
}/*RgSchMacL2MeasSendReq*/ 

/** @brief This function inserts the MeasCb in to data base
 *
 * @details
 *
 *     Function: rgL2mInsertMeasCb
 *
 * @param  [in] RgCellCb       *cell
 * @param  [in] RgL2MeasCb     *measCb
 * @param  [in] RgInfMeasReq   *measInfo
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgL2mInsertMeasCb
(
RgCellCb       *cell,
RgL2MeasCb     *measCb,
RgInfL2MeasReq *measInfo
)
#else
PRIVATE S16 rgL2mInsertMeasCb(cell, measCb, measInfo)
RgCellCb       *cell;
RgL2MeasCb     *measCb;
RgInfL2MeasReq *measInfo;
#endif
{
   CmLList   *lnk, *node;
   RgL2MeasCb  *oldMeasCb;
   U16         diffTime;

   TRC3(rgL2mInsertMeasCb)

      /* 
       * 1. Check if l2mList has any entries.
       * 2. If yes 
       *       1. Take the first entrie's time period and find the diff with
       *       cell->crntTime.
       *       2. If the diff is > measInfo->timePeriod then insert before this
       *       entry.
       *       3. Else take the next entry in list
       *       4. If reached without adding to list . Append at the end of list.
       * 3. If no entries in l2mList add at the first.
       */
      lnk = cell->l2mList.first;

   node = &measCb->measLnk;
   node->node = (PTR)measCb;
   while(lnk != NULLP )
   {
      oldMeasCb = (RgL2MeasCb *)lnk->node;
      diffTime = (oldMeasCb->measReq.timePrd - 
            (RG_CALC_SF_DIFF(cell->crntTime, oldMeasCb->startTime)));
      if (diffTime > measInfo->timePrd)
      {
         cell->l2mList.crnt = lnk;
         cmLListInsCrnt(&(cell->l2mList), node);
         RETVALUE(ROK);
      }
      else
      {
         lnk = lnk->next;
      }
   }  /* End of While */

   cmLListAdd2Tail(&(cell->l2mList), node);
   RETVALUE(ROK);
} /* rgL2mInsertMeasCb */

/** @brief This function allocates memory from the heap
 *
 * @details
 *
 *     Function: rgL2mAllocMeasCb
 *
 * @param  [in] RgCellCb       *cell
 * @param  [in] RgInfL2MeasReq *measInfo
 * @param  [out] RgErrInfo      *err
 * @return  RgSchL2MeasCb *
 */
#ifdef ANSI
PRIVATE RgL2MeasCb * rgL2mAllocMeasCb
(
RgCellCb       *cell,
RgInfL2MeasReq *measInfo,
RgErrInfo      *err
)
#else
PRIVATE RgL2MeasCb * rgL2mAllocMeasCb(cell, measInfo, err)
RgCellCb       *cell;
RgInfL2MeasReq *measInfo;
RgErrInfo      *err;
#endif
{
   RgL2MeasCb       *measCb = NULLP;

   TRC3(rgL2mAllocMeasCb)

      if((rgAllocSBuf((Data **)&(measCb),
                  sizeof(RgL2MeasCb))) == RFAILED)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,
                   "Allocation of RgL2MeasCb failed");
         err->errType  = RGERR_L2M_MEASREQ;
         err->errCause = RGERR_RAM_MEM_EXHAUST;
         RETVALUE(NULLP);
      }
   cmMemcpy((U8 *)&measCb->measReq, (U8 *)measInfo, sizeof(RgInfL2MeasReq));
   RGCPYTIMEINFO(cell->crntTime, measCb->startTime);

   RETVALUE(measCb);
} /* rgL2mAllocMeasCb */


/**
 * @brief This function calculates the measurement for measType 
 * LRG_L2MEAS_AVG_PRB_PER_QCI_UL and send the end result to the 
 * MAC Scheduler.
 *
 * @details
 *
 *  Function : rgL2Meas
 *     
 *  @param[in] RgCellCb  *cell
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgL2Meas
(
RgCellCb  *cell
)
#else
PUBLIC S16 rgL2Meas(cell)
RgCellCb  *cell;
#endif
{
   CmLList         *node   = NULLP;
   RgL2MeasCb      *measCb = NULLP;
   RgInfL2MeasCfm  measCfm;
   U8              idx = 0;
   U8              qciVal = 0;
   U32             measPrd; /*LTE_L2_MEAS_PHASE2*/
   CmLteTimingInfo  crntTime;
   
   TRC3(rgL2Meas)

   node = cell->l2mList.first;

   while(node != NULLP)
   {
      measCb = (RgL2MeasCb *)node->node;
      node = node->next;
      crntTime = cell->crntTime;

      if(cell->crntTime.sfn == 0 && cell->crntTime.subframe == 0)
      {
         measCb->sfnCycle++;
      }

      measPrd = RG_CALC_SFN_SF_DIFF(cell->crntTime, 
                        measCb->sfnCycle, measCb->startTime);
      
      /*LTE_L2_MEAS_PHASE2*/
      if (cell->sndL2Meas || measPrd == measCb->measReq.timePrd)
      {
         cmMemset((U8 *)&measCfm, 0, sizeof(RgInfL2MeasCfm));
         for(idx = 0; idx < measCb->measReq.t.prbReq.numQci; idx++)
         {
            qciVal = measCb->measReq.t.prbReq.qci[idx];
            measCfm.u.prbCfm.prbUsage[idx].qciValue = qciVal;

            measCfm.transId  = measCb->measReq.transId;
            measCfm.measType = measCb->measReq.measType;
            measCfm.cellId    = measCb->measReq.cellId;

            measCfm.u.prbCfm.prbUsage[idx].prbUsage = 
               cell->qciArray[qciVal].prbCount;

            cell->qciArray[qciVal].prbCount = 0;
            measCfm.u.prbCfm.numQci++;
            if(measCb->measReq.timePrd > 0)
            {
               cell->qciArray[qciVal].mask = FALSE;
            }
         }
         rgSndL2MeasCfm(cell, &measCfm);

         if(measCb->measReq.timePrd > 0) 
         {
            cmLListDelFrm(&cell->l2mList, &measCb->measLnk);
            rgFreeSBuf((Data**)&measCb, sizeof(RgL2MeasCb));
         }
         else /*if meas period is 0 then do not delette meascb , just reset l2 cntrs value to 0*/
         { 
            measCb->startTime = crntTime;
            measCb->measReq.timePrd = 0;
            cell->sndL2Meas = FALSE;
         }
         continue;
      } 
   }
   RETVALUE(ROK);
} /* rgL2MEas */

#endif /* LTE_L2_MEAS */
/**********************************************************************
 
         End of file:     gk_l2m.c@@/main/1 - Sat Jul 30 02:21:29 2011
 
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
/main/1      ---        ap  1. Created for LTE MAC Rel 3.1
$SID$        ---        rt  1. LTE MAC 4.1 release
*********************************************************************91*/
