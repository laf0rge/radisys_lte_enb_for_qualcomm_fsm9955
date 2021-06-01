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
  
     File:     rg_sch_gom.c 
  
     Sid:      gk_sch_gom.c@@/main/2 - Sat Jul 30 02:21:41 2011
  
     Prg:     ns 
  
**********************************************************************/

/** @file rg_sch_gom.c 
@brief This module does processing related to handling of upper interface APIs 
invoked by RRM towards MAC.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=164;

/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm_hash.h"       /* common hash list */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "rgr.h"
#include "tfu.h"
#include "rg_env.h"
#include "rg_sch_inf.h"
#include "rg_sch.h"
#include "rg_sch_err.h"
#include "rl_interface.h"
#include "rl_common.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system service interface */
#include "cm5.x"           /* common timers */
#include "cm_lib.x"        /* common library */
#include "cm_hash.x"       /* common hash list */
#include "cm_llist.x"      /* common linked list library */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common LTE */
#include "lrg.x"
#include "rgr.x"
#include "tfu.x"
#include "rg_sch_inf.x"
#include "rg_sch.x"

/* local defines */
PRIVATE S16 rgSCHGomHndlCfgReq ARGS((RgSchCb *instCb, SpId spId,
         RgrCfg  *cfg, RgSchErrInfo *errInfo));
PRIVATE S16 rgSCHGomHndlRecfgReq ARGS((RgSchCb *instCb, SpId spId,
         RgrRecfg  *recfg, RgSchErrInfo *errInfo));
PRIVATE S16 rgSCHGomHndlResetReq ARGS((RgSchCb *instCb,SpId spId,RgrRst *reset,
         RgSchErrInfo  *errInfo));
PRIVATE S16 rgSCHGomGetCellIdFrmCfgReq ARGS((RgrCfgReqInfo *rgrCfgReq,
         CmLteCellId *cellId));
PRIVATE S16 rgSCHGomCfgReq ARGS((Region reg, Pool pool, RgSchCb *instCb,
         SpId spId, RgrCfgTransId transId, RgrCfgReqInfo *cfgReqInfo));
PRIVATE S16 rgSCHGomEnqCfgReq ARGS((Region reg, Pool pool, RgSchCellCb *cell,
         RgrCfgTransId transId, RgrCfgReqInfo *rgrCfgReq));
PRIVATE S16 rgSCHGomHndlDelReq ARGS((RgSchCb  *instCb,SpId spId, 
         RgrDel  *del,RgSchErrInfo  *errInfo));

/* local typedefs */
 
/* local externs */
 
/* forward references */



/**
 * @brief Handler for config request from RRM to Schedular.
 *
 * @details
 *
 *     Function: rgSCHGomHndlCfg
 *     
 *     This API is called from schedulers UIM and it handles config request
 *     from RRM to Scheduler.
 *     
 *     Processing Steps:
 *      - If the request is for the inactive cell, 
 *       - Handle request.Call rgSCHGomCfgReq.
 *      - Else,
 *       - Enqueue the request. Call rgSCHGomEnqCfgReq.
 *         
 *  @param[in]  Region        reg
 *  @param[in]  Poll          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHGomHndlCfg
(
Region        reg,
Pool          pool,
RgSchCb       *instCb,
SpId          spId,
RgrCfgTransId transId,
RgrCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 rgSCHGomHndlCfg(reg, pool, instCb, spId, transId, cfgReqInfo)
Region        reg;
Pool          pool;
RgSchCb       *instCb;
SpId          spId;
RgrCfgTransId transId;
RgrCfgReqInfo *cfgReqInfo;
#endif
{
   S16           ret;
   CmLteCellId   cellId;
   RgSchCellCb   *cell = NULLP;    
   U8            cfmStatus = RGR_CFG_CFM_NOK;
#ifdef DEBUGP
   Inst          inst = (instCb->rgSchInit.inst );
#endif

   TRC2(rgSCHGomHndlCfg);
   /* Apply the configuration for Cell Configuration or Delete */
   if (cfgReqInfo->action != RGR_RECONFIG)
   {
      ret = rgSCHGomCfgReq (reg, pool, instCb, spId, transId, cfgReqInfo);
      RETVALUE(ret);
   }

   /* Fetch the cell Id for the recieved request */
   if((rgSCHGomGetCellIdFrmCfgReq(cfgReqInfo, &cellId)) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst, "Action.Config Type Error");

      SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
      cfgReqInfo = NULLP;
      rgSCHUtlRgrCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }
   /* Extract the cell and Enquee Config Request */
   if(NULLP != instCb->rgrSap[spId].cell)
   {
      if(cellId != instCb->rgrSap[spId].cell->cellId) 
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cellId, "Cell with Id %d already exists "
           "on sap %d", instCb->rgrSap[spId].cell->cellId, spId);  

         SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
         cfgReqInfo = NULLP;
         rgSCHUtlRgrCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
         RETVALUE(RFAILED);
      }
      cell = instCb->rgrSap[spId].cell;

      /* Enqueue the configuration */
      ret = rgSCHGomEnqCfgReq(reg, pool, cell, transId, cfgReqInfo);
      if (ret != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cellId, "rgSCHGomHndlCfg: Enqueuing CfgReq "
              "Failed ");

         SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
         cfgReqInfo = NULLP;
         rgSCHUtlRgrCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
         RETVALUE(RFAILED);
      }

      RETVALUE(ROK);
   }
   SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
   cfgReqInfo = NULLP;
   rgSCHUtlRgrCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
   RETVALUE(RFAILED);

}/* rgSCHGomHndlCfg */


/**
 * @brief Handler to handle config request from RRM to Scheduler.
 *
 * @details
 *
 *     Function: rgSCHGomCfgReq
 *     
 *     This API handles processing for config request from RRM to Scheduler.
 *     
 *     Processing Steps: 
 *      - If Configuration request, call rgSCHGomHndlCfgReq.
 *      - Else if Reconfiguration request, call rgSCHGomHndlRecfgReq.
 *      - If successful, send configuration confirm to RRM.
 *        Call rgSCHUtlRgrCfgCfm else FAIL.
 *
 *  @param[in]  Region        reg
 *  @param[in]  Poll          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomCfgReq
(
Region        reg,
Pool          pool,
RgSchCb       *instCb,
SpId          spId,
RgrCfgTransId transId,
RgrCfgReqInfo *cfgReqInfo
)
#else
PRIVATE S16 rgSCHGomCfgReq(reg, pool, instCb, spId, transId, cfgReqInfo)
Region        reg;
Pool          pool;
RgSchCb       *instCb;
SpId          spId;
RgrCfgTransId transId;
RgrCfgReqInfo *cfgReqInfo;
#endif
{
   U8              cfmStatus = RGR_CFG_CFM_OK;
   S16             ret;
   RgSchErrInfo    errInfo;
#ifdef DEBUGP
   Inst            inst = (instCb->rgSchInit.inst );
#endif
   TRC2(rgSCHGomCfgReq);

   /* Process Config/Reconfig/Delete request from RRM */
   switch (cfgReqInfo->action)
   {
      case RGR_CONFIG:
         {
            ret = rgSCHGomHndlCfgReq(rgSchCb, spId, 
                             &cfgReqInfo->u.cfgInfo, &errInfo);
            break;
         }
      case RGR_RECONFIG:
         {
            ret = rgSCHGomHndlRecfgReq(rgSchCb, spId, 
                             &cfgReqInfo->u.recfgInfo, &errInfo);
            break;
         }
      case RGR_RESET:
         {
            ret = rgSCHGomHndlResetReq(rgSchCb, spId, 
                             &cfgReqInfo->u.rstInfo, &errInfo);
            break;
         }
      case RGR_DELETE:
         {
            ret = rgSCHGomHndlDelReq(rgSchCb, spId,
                             &cfgReqInfo->u.delInfo, &errInfo);
            break;
         } 
      default:
         {
            RLOG_ARG1(L_ERROR,DBG_INSTID,inst, "Invalid configuration "
               "action %d", cfgReqInfo->action);
            ret = RFAILED;
         }
   } /* End of switch */

   if (ret != ROK)
   {
      cfmStatus = RGR_CFG_CFM_NOK;
      QU_CRITLOG(QU_LOG_MESSAGE,"Sch Cfg Fail ErrTyp %d , ErrCause %d",
         errInfo.errType, errInfo.errCause);
   }

   SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
   cfgReqInfo = NULLP;
   /* Send back confirmation status to RRM */   
   rgSCHUtlRgrCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
   RETVALUE(ret);
}  /* rgSCHGomCfgReq */


/**
 * @brief Handler to enqueuing config request from RRM to Scheduler.
 *
 * @details
 *
 *     Function: rgSCHGomEnqCfgReq
 *     
 *     This API enqueues config request from RRM to MAC. 
 *     
 *     Processing Steps:
 *      - Allocate the configuration request element.
 *      - Copy the contents of the recieved configuration to config request
 *        element and free the recieved configuration pointer.
 *      - If the configuration is without activation time,
 *         - Enqueue the request in crntRgrCfgLst of the cell at the end of 
 *           the list.
 *      - Else
 *         - Enqueue the request in pndngRgrCfgLst of the cell.
 *         
 *  @param[in]  Region        reg,
 *  @param[in]  Pool          pool
 *  @param[in]  RgSchCellCb   *cell
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrCfgReqInfo *rgrCfgReq
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomEnqCfgReq
(
Region        reg,
Pool          pool,
RgSchCellCb   *cell,
RgrCfgTransId transId,
RgrCfgReqInfo *rgrCfgReq
)
#else
PRIVATE S16 rgSCHGomEnqCfgReq(reg, pool, cell, transId, rgrCfgReq)
Region        reg;
Pool          pool;
RgSchCellCb   *cell;
RgrCfgTransId transId;
RgrCfgReqInfo *rgrCfgReq;
#endif
{
   S16                ret;
   U32                sfDiff;
   RgSchCfgElem       *rgrCfgElem  = NULLP;
   CmLteTimingInfo    actvTime; 
   Inst               inst = cell->instIdx;

   TRC2(rgSCHGomEnqCfgReq);

   /* Allocate memory for config Element */
   ret = rgSCHUtlAllocSBuf(inst, (Data **)&rgrCfgElem, sizeof(RgSchCfgElem));
   if ((ret != ROK) || ((U8 *)rgrCfgElem == NULLP))
   {
      RETVALUE(RFAILED);
   }

   /* Initialize the configuration element */
   cmMemcpy((U8*)rgrCfgElem->rgrCfg.transId.trans,(U8*)transId.trans,
         sizeof(transId.trans));
   rgrCfgElem->rgrCfg.reg = reg;
   rgrCfgElem->rgrCfg.pool = pool;
   rgrCfgElem->rgrCfg.rgrCfgReq = rgrCfgReq;
   rgrCfgElem->cfgReqLstEnt.prev = NULLP;
   rgrCfgElem->cfgReqLstEnt.next = NULLP;
   rgrCfgElem->cfgReqLstEnt.node = (PTR )rgrCfgElem;

   /* Add configuration element to current/pending cfgLst */
   if (((rgrCfgReq->action == RGR_RECONFIG) &&
            (rgrCfgReq->u.recfgInfo.recfgType == RGR_CELL_CFG) &&
            (rgrCfgReq->u.recfgInfo.u.cellRecfg.recfgActvTime.pres == TRUE)))

   {
      actvTime = 
         rgrCfgReq->u.recfgInfo.u.cellRecfg.recfgActvTime.actvTime;

      /* Check if the activation time is valid */
      if (actvTime.sfn >= RGSCH_MAX_SFN 
            || actvTime.subframe >= RGSCH_NUM_SUB_FRAMES)
      {
         RLOG_ARG4(L_ERROR,DBG_CELLID,cell->cellId, "Invalid activation time for RGR "
           "config request: activation sfn %d activation subframe %d current "
           "sfn %d current subframe %d", actvTime.sfn, actvTime.subframe, 
           cell->crntTime.sfn, cell->crntTime.subframe);
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)&rgrCfgElem, sizeof(*rgrCfgElem));
         RETVALUE(RFAILED);
      }

      sfDiff = RGSCH_CALC_SF_DIFF(actvTime, cell->crntTime);

      if (sfDiff > (RGR_ACTV_WIN_SIZE * RGSCH_NUM_SUB_FRAMES))
      {
         RLOG_ARG4(L_ERROR,DBG_CELLID,cell->cellId,"Invalid activation time for RGR"
              " config request: activation sfn %d activation subframe %d "
              "current sfn %d current subframe %d", actvTime.sfn,
              actvTime.subframe, cell->crntTime.sfn, cell->crntTime.subframe);
         /* ccpu00117052 - MOD - Passing double pointer
         for proper NULLP assignment*/
         rgSCHUtlFreeSBuf(inst, (Data **)&rgrCfgElem, sizeof(*rgrCfgElem));
         RETVALUE(RFAILED);
      }

      if (sfDiff)
      {
         /* Add to pending cfgReqLst */
         rgrCfgElem->actvTime = actvTime; 
         rgSCHDbmInsPndngRgrCfgElem(cell, rgrCfgElem);
         /* Cfm to be sent only after applying request */
         RETVALUE(ROK);
      }
   }

   /* Add to current cfgReq list */
   rgSCHDbmInsCrntRgrCfgElem(cell, rgrCfgElem);
   /* Cfm to be sent only after applying request */
   RETVALUE(ROK);
}  /* rgSCHGomEnqCfgReq */


/**
 * @brief Handler for TTI processing for configurations recieved from RRM.
 *
 * @details
 *
 *     Function: rgSCHGomTtiHndlr
 *     
 *     This API does TTI processing for configurations recieved from RRM.
 *     
 *     Processing Steps:
 *      - It dequeues config request from the current configuration list.
 *        For each config request in the list: 
 *        - Processes the request. Call rgSCHGomCfgReq.
 *      - It dequeues config request for the current tti from the pending 
 *        configuration list. For each config request in the list:
 *        - Processes the request. Call rgSCHGomCfgReq.
 *
 *  @param[in]  RgSchCellCb *cell
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHGomTtiHndlr
(
RgSchCellCb      *cell,
SpId             spId
)
#else
PUBLIC S16 rgSCHGomTtiHndlr(cell, spId)
RgSchCellCb      *cell;
SpId             spId;
#endif
{
   RgSchCfgElem       *cfgElem;
   Inst               inst= cell->instIdx;
   TRC2(rgSCHGomTtiHndlr);

   /* Dequeue from current config list */
   while ((cfgElem = rgSCHDbmGetNextCrntRgrCfgElem(cell, NULLP)) != NULLP)
   {
      rgSCHDbmDelCrntRgrCfgElem(cell, cfgElem);
      rgSCHGomCfgReq(cfgElem->rgrCfg.reg,cfgElem->rgrCfg.pool,
            &rgSchCb[inst], spId, cfgElem->rgrCfg.transId, 
                                          cfgElem->rgrCfg.rgrCfgReq);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&cfgElem, sizeof(*cfgElem));
   }

   /* Handle config requests from pending config list */
   while((cfgElem = rgSCHDbmGetPndngRgrCfgElemByKey(cell, cell->crntTime)) != NULLP)
   {
      rgSCHDbmDelPndngRgrCfgElem(cell, cfgElem);
      rgSCHGomCfgReq(cfgElem->rgrCfg.reg, cfgElem->rgrCfg.pool, 
            &rgSchCb[inst], spId, cfgElem->rgrCfg.transId, 
                                          cfgElem->rgrCfg.rgrCfgReq);
      /* ccpu00117052 - MOD - Passing double pointer
      for proper NULLP assignment*/
      rgSCHUtlFreeSBuf(inst, (Data **)&cfgElem, sizeof(*cfgElem));
   } 

   RETVALUE(ROK);
}


/**
 * @brief Handler to handle configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlCfgReq
 *     
 *     This API handles processing for configuration request from RRM to MAC.
 *     
 *     - Processing Steps: 
 *        - Validate configuration request parameters at CFG module. 
 *          Call rgSCHCfgVldtRgrCellCfg for cell configuration.
 *        - If validated successfully, send configuration request to CFG.
 *          Call rgSCHCfgRgrCellCfg else FAIL.
 *          
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgrCfg       *cfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomHndlCfgReq
(
RgSchCb        *instCb,
SpId           spId,
RgrCfg         *cfg,
RgSchErrInfo   *errInfo
)
#else
PRIVATE S16 rgSCHGomHndlCfgReq(instCb, spId, cfg, errInfo)
RgSchCb        *instCb;
SpId           spId;
RgrCfg         *cfg;
RgSchErrInfo   *errInfo;
#endif
{
   S16          ret;
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   Inst         inst = (instCb->rgSchInit.inst );
   RgSchUeCb    *ue;

   TRC2(rgSCHGomHndlCfgReq);

   errInfo->errType = RGSCHERR_GOM_CFG_REQ;
   
   /* Validate and process the configuration request */ 
   switch (cfg->cfgType)
   {
      case RGR_CELL_CFG:
      {
         ret = rgSCHCfgVldtRgrCellCfg(inst, &cfg->u.cellCfg, cell, errInfo);
            if (ret != ROK)
            {
               RLOG1(L_ERROR,"Rgr Cell configuration "
                  "validation FAILED: Cell %d", cfg->u.cellCfg.cellId);
               RETVALUE(RFAILED);
            }
         ret = rgSCHCfgRgrCellCfg(instCb, spId, &cfg->u.cellCfg, errInfo);
            if (ret != ROK)
            {
               QU_CRITLOG(QU_LOG_MESSAGE,"Rgr Cell configuration "
                  "validation FAILED: Cell %d 2\n", cfg->u.cellCfg.cellId);
               RETVALUE(RFAILED);
            }
         break;
      }
      case RGR_UE_CFG:
      {
         ret = rgSCHCfgVldtRgrUeCfg(inst, &cfg->u.ueCfg, &cell, errInfo);
         if (ret != ROK)
         {
            RLOG1(L_ERROR,"Ue configuration validation"
               " FAILED: CRNTI:%d", cfg->u.ueCfg.crnti);
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrUeCfg(cell, &cfg->u.ueCfg, errInfo);
         break;
      }
      case RGR_LCH_CFG:
      {
         ret = rgSCHCfgVldtRgrLcCfg(inst, &cfg->u.lchCfg, &cell, &ue, errInfo);
         if (ret != ROK)
         {
            RLOG1(L_ERROR,"LC configuration validation "
              "FAILED: LCID:%d", cfg->u.lchCfg.lcId);
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrLchCfg(cell, ue, &cfg->u.lchCfg, errInfo); 
         break;
      }
      case RGR_LCG_CFG:
      {
         ret = rgSCHCfgVldtRgrLcgCfg(inst, &cfg->u.lcgCfg, &cell, &ue, errInfo);
         if (ret != ROK)
         {
            RLOG1(L_ERROR,"LCG configuration validation "
              "FAILED: LCGID:%d", cfg->u.lcgCfg.ulInfo.lcgId);
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrLcgCfg(cell, ue, &cfg->u.lcgCfg, errInfo); 
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
            RLOG1(L_ERROR,"Should never come here: "
                "cfgType %d", cfg->cfgType);
#endif
            RETVALUE(RFAILED);
      }
   }
   
   RETVALUE(ret);
}  /* rgSCHGomHndlCfgReq */


/**
 * @brief Handler to handle re-configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlRecfgReq
 *     
 *     This API handles processing for re-configuration request from RRM to MAC.
 *     
 *     - Processing Steps: 
 *        - Validate re-configuration request parameters at CFG module. 
 *          Call rgSCHCfgVldtRgrCellRecfg for cell re-configuration.
 *        - If validated successfully, send configuration request to CFG.
 *          Call rgSCHCfgRgrCellRecfg else FAIL.
 *
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgrRecfg     *recfg
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomHndlRecfgReq
(
RgSchCb       *instCb,
SpId          spId,
RgrRecfg      *recfg,
RgSchErrInfo  *errInfo
)
#else
PRIVATE S16 rgSCHGomHndlRecfgReq(instCb, spId, recfg, errInfo)
RgSchCb       *instCb;
SpId          spId;
RgrRecfg      *recfg;
RgSchErrInfo  *errInfo;
#endif
{
   RgSchUeCb    *ue = NULLP;
   RgSchDlLcCb  *dlLc = NULLP; /* PURIFY_FIX:UMR */
   S16          ret; 
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   Inst         inst = (instCb->rgSchInit.inst );

   TRC2(rgSCHGomHndlRecfgReq);

   errInfo->errType = RGSCHERR_GOM_RECFG_REQ;
   
   /* Validate and process the re-configuration request */
   switch (recfg->recfgType)
   {
      case RGR_CELL_CFG:
      {
         ret = rgSCHCfgVldtRgrCellRecfg(inst, &recfg->u.cellRecfg, &cell,
               errInfo);
         if (ret != ROK) 
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,recfg->u.cellRecfg.cellId,"Rgr Cell Recfg Validation "
                     "FAILED");
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrCellRecfg(cell, &recfg->u.cellRecfg, errInfo);
         break;
      }
      case RGR_UE_CFG:
      {
         ret = rgSCHCfgVldtRgrUeRecfg(inst, &recfg->u.ueRecfg, &cell, &ue, errInfo);
         if ( ret != ROK)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,recfg->u.ueRecfg.cellId,"Ue Recfg Validation FAILED"
                      "OLD CRNTI:%d",recfg->u.ueRecfg.oldCrnti);
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrUeRecfg(cell, ue, &recfg->u.ueRecfg, errInfo);
         break;
      }
      case RGR_LCH_CFG:
      {
         ret = rgSCHCfgVldtRgrLchRecfg(inst, &recfg->u.lchRecfg, &cell, &ue, 
               &dlLc, errInfo);
         if (ret != ROK)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,recfg->u.lchRecfg.cellId,"Lc Recfg Validation FAILED"
                      "LCID:%d",recfg->u.lchRecfg.lcId);
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrLchRecfg(cell, ue, dlLc, &recfg->u.lchRecfg, errInfo);
         break;
      }
      case RGR_LCG_CFG:
      {
         ret = rgSCHCfgVldtRgrLcgRecfg(inst, &recfg->u.lcgRecfg, cell, &ue,
               errInfo);
         if (ret != ROK)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,recfg->u.lcgRecfg.cellId, "Lcg Recfg Validation FAILED"
                      "LCGID:%d",recfg->u.lcgRecfg.ulRecfg.lcgId);
            RETVALUE(RFAILED);
         }
         ret = rgSCHCfgRgrLcgRecfg(cell, ue, &recfg->u.lcgRecfg, errInfo);
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
         RLOG1(L_ERROR,"Should never come here: recfgType %d", recfg->recfgType);
#endif
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ret);
}  /* rgSCHGomHndlRecfgReq */

/**
 * @brief Handler to handle UE reset request from RRM to Scheduler.
 *
 * @details
 *
 *     Function: rgSCHGomHndlResetReq
 *     
 *     This API handles processing for UE reset request from RRM to Scheduler.
 *     
 *     - Processing Steps: 
 *        - Validate UE reset request parameters at CFG module. 
 *          Call rgSCHCfgVldtRgrUeReset for UE reset.
 *        - If validated successfully, send UE reset request to CFG.
 *          Call rgSCHCfgRgrUeReset else FAIL.
 *
 *  @param[in]  RgrRst   *rstInfo
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomHndlResetReq
(
RgSchCb       *instCb,
SpId          spId,
RgrRst        *reset,
RgSchErrInfo  *errInfo
)
#else
PRIVATE S16 rgSCHGomHndlResetReq(instCb, spId, reset, errInfo)
RgSchCb       *instCb;
SpId          spId;
RgrRst        *reset;
RgSchErrInfo  *errInfo;
#endif
{
   S16          ret; 
   RgSchCellCb  *cell= instCb->rgrSap[spId].cell;
   Inst         inst = (instCb->rgSchInit.inst );
   RgSchUeCb    *ue = NULLP;

   TRC2(rgSCHGomHndlResetReq);


   errInfo->errType = RGSCHERR_GOM_RESET_REQ;
   
   /* Validate and process the UE reset request */
   ret = rgSCHCfgVldtRgrUeReset(inst, reset, cell, &ue, errInfo);
   if (ret != ROK) 
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,reset->cellId,"Rgr UE Reset Validation FAILED"
               "CRNTI:%d",reset->crnti);
      RETVALUE(RFAILED);
   }
   
   ret = rgSCHCfgRgrUeReset(cell, ue, reset, errInfo);
   if (ret != ROK) 
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,reset->cellId,"Rgr UE Reset FAILED"
               "CRNTI:%d",reset->crnti);
      RETVALUE(RFAILED);
   }

   RETVALUE(ret);
}  /* rgSCHGomHndlResetReq */


/**
 * @brief Handler for processing Cell/Ue/Logical channel delete request
 * recieved from RRM.
 *
 * @details
 *
 *     Function: rgSCHGomHndlDelReq
 *     
 *     This API handles processing of delete request from RRM to MAC. 
 *     
 *     Processing Steps:
 *        - Fetch corresponding control block and pass it to CFG module.
 *        - If control block does not exist, FAIL.
 *
 *  @param[in]  RgSchCb      *instCb
 *  @param[in]  SpId         spId
 *  @param[in]  RgrDel       *del
 *  @param[out] RgSchErrInfo *errInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgSCHGomHndlDelReq
(
RgSchCb       *instCb,
SpId          spId,
RgrDel        *del,
RgSchErrInfo  *errInfo
)
#else
PRIVATE S16 rgSCHGomHndlDelReq(instCb, spId, del, errInfo)
RgSchCb       *instCb;
SpId          spId;
RgrDel        *del;
RgSchErrInfo  *errInfo;
#endif
{

   S16       ret;
#ifdef DEBUGP
   Inst      inst = (instCb->rgSchInit.inst);
#endif
   VOLATILE U32     startTime=0;

   TRC2(rgSCHGomHndlDelReq);
   
   errInfo->errType = RGSCHERR_GOM_DEL_REQ;

   if(instCb->rgrSap[spId].cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_INSTID,inst,"Cell doesnt exist");
      RETVALUE(RFAILED); 
   }
   
   /* Process the delete request */ 
   switch (del->delType)
   {
      case RGR_CELL_CFG:
      {
         ret = rgSCHCfgRgrCellDel(instCb->rgrSap[spId].cell, del, errInfo);
         if(ret == ROK)
         {
            instCb->rgrSap[spId].cell = NULLP;
            instCb->tfuSap[spId].cell = NULLP;
         }
         break;
      }
      case RGR_UE_CFG:
      {

         /*starting Task*/
         SStartTask(&startTime, PID_SCH_UE_DEL);

         ret = rgSCHCfgRgrUeDel(instCb->rgrSap[spId].cell, del, errInfo);

         /*stoping Task*/
         SStopTask(startTime, PID_SCH_UE_DEL);

         break;
      }
      case RGR_LCH_CFG:
      {
         ret = rgSCHCfgRgrLcDel(instCb->rgrSap[spId].cell, del, errInfo);
         break;
      }
      case RGR_LCG_CFG:
      {
         ret = rgSCHCfgRgrLcgDel(instCb->rgrSap[spId].cell, del, errInfo);
         break;
      }
      default:
      {
#if(ERRCLASS & ERRCLS_INT_PAR)
         RLOG1(L_ERROR,"Should never come here: delType %d", del->delType);
#endif
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ret);
}  /* rgSCHGomHndlDelReq */




/***********************************************************
 *
 *     Func : rgSCHGomGetCellIdFrmCfgReq
 *        
 *
 *     Desc : 
 *     - Processing Steps: 
 *        - Retrieves the cell Id for a config request.
 *
 *  @param[in]  RgrCfgReqInfo *rgrCfgReq
 *  @param[out] CmLteCellId   *cellId
 *     Ret  : ROK on fetching cellId
 *            RFAILED on failure
 *
 *     Notes: 
 *
 *     File : rg_sch_gom.c 
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgSCHGomGetCellIdFrmCfgReq
(
RgrCfgReqInfo *rgrCfgReq,
CmLteCellId   *cellId
)
#else
PRIVATE S16 rgSCHGomGetCellIdFrmCfgReq(rgrCfgReq, cellId)
RgrCfgReqInfo *rgrCfgReq;
CmLteCellId   *cellId;
#endif
{

   TRC2(rgSCHGomGetCellIdFrmCfgReq);


   /* Extract CellId depending on the action and Config Type in the Request 
    * As of now this function is called for only re configuration so removed
    * othe CASES below if needed we can add them*/
   switch (rgrCfgReq->action)
   {
      case RGR_RECONFIG:
         {
            if (rgrCfgReq->u.recfgInfo.recfgType ==RGR_CELL_CFG)
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.cellRecfg.cellId;
            }
            else if (rgrCfgReq->u.recfgInfo.recfgType == RGR_UE_CFG)
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.ueRecfg.cellId;
            }
            else if (rgrCfgReq->u.recfgInfo.recfgType == RGR_LCH_CFG)
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.lchRecfg.cellId;
            }
            else if (rgrCfgReq->u.recfgInfo.recfgType == RGR_LCG_CFG) 
            {
               *cellId = rgrCfgReq->u.recfgInfo.u.lcgRecfg.cellId;
            }
            else
            {
               RETVALUE(RFAILED);
            }
            break;
         }
      default:
      {
         RETVALUE(RFAILED);
      }
   }  /* End of Switch */

   RETVALUE(ROK);
}  /* rgSCHGomGetCellIdFrmCfgReq */

#ifdef RGR_SI_SCH
/**
 * @brief Handler to handle SI configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlSiCfg
 *     
 *     This API handles processing for SI configuration request from RRM to MAC.
 *     
 *     - Processing Steps: 
 *        - Validate SI configuration request parameters at CFG module. 
 *          Call rgSCHCfgVldtSiCfg for SI configuration.
 *        - If validated successfully, send configuration request to CFG.
 *          Call rgSCHCfgRgrCellCfg else FAIL.
 *          
 *  @param[in]  Region        reg
 *  @param[in]  Pool          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  SpId          spId
 *  @param[in]  RgrCfgTransId transId
 *  @param[in]  RgrSiCfgReqInfo *cfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHGomHndlSiCfg
(
Region        reg,
Pool          pool,
RgSchCb       *instCb,
SpId          spId,
RgrCfgTransId transId,
RgrSiCfgReqInfo *cfgReqInfo
)
#else
PUBLIC S16 rgSCHGomHndlSiCfg(reg, pool, instCb, spId, transId, cfgReqInfo)
Region        reg;
Pool          pool;
RgSchCb       *instCb;
SpId          spId;
RgrCfgTransId transId;
RgrSiCfgReqInfo *cfgReqInfo;
#endif
{
   S16          ret;
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   Inst         inst = (instCb->rgSchInit.inst );
   RgSchErrInfo    errInfo;
   U8           cfmStatus = RGR_CFG_CFM_NOK;
   MsgLen       msgLen = 0, pduLen;
   S32          tbSz   = 0;
   U8           nPrb   = 0; 
   U8           mcs    = 0;

   TRC2(rgSCHGomHndlSiCfg);


   /* check if cell does not exists */
   if (((U8 *)cell == NULLP) || (cell->cellId != cfgReqInfo->cellId))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cfgReqInfo->cellId,"Cell Control block does not exist"
               );
      RGSCH_FREE_MSG(cfgReqInfo->pdu);
      SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
      cfgReqInfo = NULLP;
      rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }

   /*Validate the received SI configuration */
   ret = rgSCHCfgVldtRgrSiCfg(inst, cfgReqInfo, cell, &errInfo);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cfgReqInfo->cellId,"Rgr SI configuration "
               "validation FAILED");
      RGSCH_FREE_MSG(cfgReqInfo->pdu);
      SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
      cfgReqInfo = NULLP;
      rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 
      RETVALUE(RFAILED);
   }
   /*ccpu00140789: Stopping SI scheduling*/
   if(RGR_SI_STOP == cfgReqInfo->cfgType)
   {
      if((cell->siCb.crntSiInfo.siInfo[cfgReqInfo->siId-1].si != NULLP)&&
                      (cell->siCb.siArray[cfgReqInfo->siId-1].si != NULLP))
      {
         cell->siCb.crntSiInfo.siInfo[cfgReqInfo->siId-1].si = NULLP;
         RGSCH_FREE_MSG(cell->siCb.siArray[cfgReqInfo->siId-1].si);
         cell->siCb.siArray[cfgReqInfo->siId-1].si = NULLP;
         if(cell->siCb.newSiInfo.siInfo[cfgReqInfo->siId-1].si != NULLP)
         {
            RGSCH_FREE_MSG(cell->siCb.newSiInfo.siInfo[cfgReqInfo->siId-1].si);
            cell->siCb.newSiInfo.siInfo[cfgReqInfo->siId-1].si = NULLP;
         }
         SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
         cfgReqInfo = NULLP;   
         cfmStatus = RGR_CFG_CFM_OK;
         rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus);
         RETVALUE(ROK); 
       }
       else
       {
          SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo));
          cfgReqInfo = NULLP;
          rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus);
          RETVALUE(RFAILED); 
       }
   }

   /* Check if the pdu sent from application
    * matches a transport block size. if not,
    * add padding bytes. This is usually done
    * by RRC but since we are bypassing RRC,
    * MAC is taking over that responsibility
    */

   if ( RGR_SI_CFG_TYPE_MIB != cfgReqInfo->cfgType)
   {
      SFndLenMsg(cfgReqInfo->pdu, &msgLen);

      /* check if the application pdu matches a tb size */
      tbSz = rgSCHUtlGetAllwdCchTbSz(msgLen*8, &nPrb, &mcs);

      if ( tbSz != (msgLen*8) )
      {
         MsgLen  nmPadBytes = 0;
         Data*   padding    = NULLP;

         /* need to add padding bytes */
         nmPadBytes = (tbSz - (msgLen*8))/8;

         if ( SGetSBuf(reg,pool,&padding,nmPadBytes) != ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cfgReqInfo->cellId,"Rgr SI configuration "
                     "SGetSBuf failed for padding failed");
            SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            cfgReqInfo = NULLP;
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                                                           cfmStatus); 
            RETVALUE(RFAILED);
         }

         cmMemset((U8*)padding,(U8)0,nmPadBytes);

#ifdef MS_MBUF_CORRUPTION 
   MS_BUF_ADD_ALLOC_CALLER();
#endif 
         if ( SAddPstMsgMult((Data*)padding,nmPadBytes,cfgReqInfo->pdu) != ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cfgReqInfo->cellId,"Rgr SI configuration "
                     "Failed to add padding bytes");
            SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            cfgReqInfo = NULLP;
            SPutSBuf(reg, pool, (Data* )padding,(Size)nmPadBytes);
            padding = NULLP;
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                                                            cfmStatus); 
            RETVALUE(RFAILED);
         }
         SPutSBuf(reg, pool, (Data* )padding,(Size)nmPadBytes);
         padding = NULLP;
      }/* if (tbSz != ...*/
   }/* if (RGR_SI_CFG_TYPE_SI...*/

   /*Set the received pdu at the appropriate place */
   switch(cfgReqInfo->cfgType)
   {
      case RGR_SI_CFG_TYPE_MIB:   /* SI CFG Type MIB */
         RGSCHCHKNUPDSIPDU(cell->siCb.crntSiInfo.mib, 
               cell->siCb.newSiInfo.mib, 
               cfgReqInfo->pdu, cell->siCb.siBitMask, 
               RGSCH_SI_MIB_UPD);
         break;

      case RGR_SI_CFG_TYPE_SIB1_PWS:
         SFndLenMsg(cfgReqInfo->pdu, &pduLen);
         ret = rgSCHUtlCalMcsAndNPrb(cell, cfgReqInfo->cfgType, pduLen,0);
         if (ret != ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cfgReqInfo->cellId,"Failed to get MCS and NPRB" 
                     "value");
            RGSCH_FREE_MSG(cfgReqInfo->pdu);
            SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            cfgReqInfo = NULLP;
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                                                                 cfmStatus); 
            RETVALUE(RFAILED);
         }

         RGSCHCHKNUPDSIPDU(cell->siCb.crntSiInfo.sib1Info.sib1, 
               cell->siCb.newSiInfo.sib1Info.sib1, 
               cfgReqInfo->pdu, cell->siCb.siBitMask, 
               RGSCH_SI_SIB1_PWS_UPD);
         break;

      case RGR_SI_CFG_TYPE_SIB1:
         SFndLenMsg(cfgReqInfo->pdu, &pduLen);
         ret = rgSCHUtlCalMcsAndNPrb(cell, cfgReqInfo->cfgType, pduLen,0);
         if (ret != ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cfgReqInfo->cellId,"Failed to get MCS and NPRB" 
                     "value");
            RGSCH_FREE_MSG(cfgReqInfo->pdu);
            SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            cfgReqInfo = NULLP;
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                                                                 cfmStatus); 
            RETVALUE(RFAILED);
         }
         RGSCHCHKNUPDSIPDU(cell->siCb.crntSiInfo.sib1Info.sib1, 
               cell->siCb.newSiInfo.sib1Info.sib1, 
               cfgReqInfo->pdu, cell->siCb.siBitMask, 
               RGSCH_SI_SIB1_UPD);
         break;

      case RGR_SI_CFG_TYPE_SI:    /* SI CFG TYPE SI */
         SFndLenMsg(cfgReqInfo->pdu, &pduLen);
         ret = rgSCHUtlCalMcsAndNPrb(cell, cfgReqInfo->cfgType, pduLen, 
                                                     cfgReqInfo->siId);
         if (ret != ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cfgReqInfo->cellId,"Failed to get MCS and NPRB" 
                     "value");
            RGSCH_FREE_MSG(cfgReqInfo->pdu);
            SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            cfgReqInfo = NULLP;
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                                                                 cfmStatus); 
            RETVALUE(RFAILED);
         }
         /* Si recfg, where numSi changes */
         if (cell->siCb.siBitMask & RGSCH_SI_SICFG_UPD) 
         {
            Buffer **newSiPdu = &cell->siCb.newSiInfo.siInfo[cfgReqInfo->siId-1].si;
            if(*newSiPdu != NULLP)
            {
               RGSCH_FREE_MSG(*newSiPdu);
            }
            *newSiPdu = (Buffer *)cfgReqInfo->pdu;
            cell->siCb.siBitMask |= RGSCH_SI_SI_UPD;
         }
         else /* Initial Si cfg  or si recfg where numSi did not change */
         {
            U8 bitMask;
            /* Initial Si cfg */
            if (cell->siCb.crntSiInfo.siInfo[cfgReqInfo->siId-1].si == NULLP)
            {
               cell->siCb.siArray[cfgReqInfo->siId-1].si = cfgReqInfo->pdu;
               cell->siCb.siArray[cfgReqInfo->siId-1].isWarningSi = FALSE;
               bitMask = RGSCH_SI_DFLT;
            }
            else
            {
               bitMask = RGSCH_SI_SI_UPD;
            }

            RGSCHCHKNUPDSIPDU(cell->siCb.crntSiInfo.siInfo[cfgReqInfo->siId-1].si, 
                cell->siCb.newSiInfo.siInfo[cfgReqInfo->siId-1].si, 
                cfgReqInfo->pdu, 
                cell->siCb.siBitMask, bitMask);
         }
         break;

      case RGR_SI_CFG_TYPE_SIB8_CDMA:    /* SI CFG TYPE SIB 8 CDMA */
         SFndLenMsg(cfgReqInfo->pdu, &pduLen);
         ret = rgSCHUtlCalMcsAndNPrb(cell, cfgReqInfo->cfgType, pduLen, 
                                                     cfgReqInfo->siId);
         if (ret != ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cfgReqInfo->cellId,"Failed to get MCS and NPRB" 
                     "value");
            RGSCH_FREE_MSG(cfgReqInfo->pdu);
            SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
            cfgReqInfo = NULLP;
            rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
                                                                 cfmStatus); 
            RETVALUE(RFAILED);
         }
         /* No need to wait for Modification period boundary */
         cell->siCb.siArray[cfgReqInfo->siId-1].si = cfgReqInfo->pdu;
         RGSCH_SET_SI_INFO(cell->siCb.crntSiInfo.siInfo[cfgReqInfo->siId-1].si,
               cfgReqInfo->pdu);
         cell->siCb.siArray[cfgReqInfo->siId-1].isWarningSi = FALSE;
         break;

      default:
         RLOG_ARG0(L_ERROR,DBG_CELLID,cfgReqInfo->cellId,"Invalid cfgType "
                  "parameter value");
         RGSCH_FREE_MSG(cfgReqInfo->pdu);
         SPutSBuf(reg, pool, (Data *)cfgReqInfo, 
               (Size)sizeof(*cfgReqInfo)); 
         cfgReqInfo = NULLP;
         rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, 
               cfmStatus); 
         RETVALUE(RFAILED);
   }

   SPutSBuf(reg, pool, (Data *)cfgReqInfo, (Size)sizeof(*cfgReqInfo)); 
   cfgReqInfo = NULLP;
   cfmStatus = RGR_CFG_CFM_OK;
   rgSCHUtlRgrSiCfgCfm(instCb->rgSchInit.inst, spId, transId, cfmStatus); 


   RETVALUE(ROK);
}  /* rgSCHGomHndlSiCfg */


/**
 * @brief Handler to handle Warning SI configuration request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlWarningSiCfg
 *     
 *     This API handles processing for Warning SI configuration request from 
 *     RRM to MAC.
 *     
 *          
 *  @param[in]  Region        reg
 *  @param[in]  Pool          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  SpId          spId
 *  @param[in]  RgrWarningSiCfgReqInfo  *warningSiCfgReqInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgSCHGomHndlWarningSiCfg
(
Region        reg,
Pool          pool,
RgSchCb       *instCb,
SpId          spId,
RgrCfgTransId transId,
RgrWarningSiCfgReqInfo *warningSiCfgReqInfo
)
#else
PUBLIC S16 rgSCHGomHndlWarningSiCfg(reg, pool, instCb, spId, transId, 
warningSiCfgReqInfo)
Region        reg;
Pool          pool;
RgSchCb       *instCb;
SpId          spId;
RgrCfgTransId transId;
RgrWarningSiCfgReqInfo *warningSiCfgReqInfo;
#endif
{
   RgSchCellCb       *cell = instCb->rgrSap[spId].cell;
   U8                 cfmStatus = RGR_CFG_CFM_NOK;
   U16                idx;
   U8                 siId = warningSiCfgReqInfo->siId; 
   U8                 j, mcs=0, nPrb=0;
   RgSchWarningSiSeg  *warningSiMsg;
   RgSchWarningSiPdu  *pduNode;
   CmLList            *node;
   MsgLen             msgLen = 0;
   Bool               freeNodeFound = FALSE;

   TRC2(rgSCHGomHndlWarningSiCfg);


   /* check if cell does not exists */
   if (((U8 *)cell == NULLP) || 
         (cell->cellId != warningSiCfgReqInfo->cellId) ||
         (warningSiCfgReqInfo->siId > 
          ((cell->siCfg.minPeriodicity * 10)/cell->siCfg.siWinSize)))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,warningSiCfgReqInfo->cellId,"Warning SI Cfg Failed for siId = %d"
                "warning cellID:%d",warningSiCfgReqInfo->siId,warningSiCfgReqInfo->cellId);
      rgSCHUtlFreeWarningSiSeg(reg, pool, &warningSiCfgReqInfo->siPduLst);
      SPutSBuf(reg, pool, (Data *)warningSiCfgReqInfo, 
            sizeof(RgrWarningSiCfgReqInfo)); 
      warningSiCfgReqInfo = NULLP;
      rgSCHUtlRgrWarningSiCfgCfm(instCb->rgSchInit.inst, spId, siId, transId, 
            cfmStatus); 
      RETVALUE(RFAILED);
   }

   /* Search for free index in WarningSi */
   for(idx = 0; idx < RGR_MAX_NUM_WARNING_SI; idx++)
   {
      if((cell->siCb.warningSi[idx].siId == 0 ||
               cell->siCb.warningSi[idx].siId == warningSiCfgReqInfo->siId))
      {
         warningSiMsg = (RgSchWarningSiSeg *)&cell->siCb.warningSi[idx].warningSiMsg; 

         /* Search for free SI node */
         /* ccpu00136659: CMAS ETWS design changes */
         if (warningSiMsg->segLstCp.first == NULLP) /* Free SI Node */
         {
            warningSiMsg->transId = transId;
            pduNode = (RgSchWarningSiPdu *)&cell->siCb.warningSi[idx].
               warningSiMsg.pduNode;
            CM_LLIST_FIRST_NODE(&warningSiCfgReqInfo->siPduLst, node);
            j = 0;

            /* Get the PDUs one by one from the received pduLst of warning 
             * message and calculate the MCS and nPrb of each pdu once.
             * Store the pdu in warningSiMsg pduLst, which will be scheduled 
             * later while sending warning message as part of SIB11/SIB12 
             */   
            while((node != NULLP) && (j < RGR_MAX_WARNING_SI_SEG))

            {
               pduNode[j].pdu = (Buffer *)node->node;
               if(pduNode[j].pdu != NULLP)
               {
                  SFndLenMsg(pduNode[j].pdu, &msgLen);
                  /*Get the nPrb and mcs parametr values */
                  if (rgSCHUtlGetAllwdCchTbSz(msgLen*8, &nPrb, &mcs) != 
                        (msgLen*8))
                  {
                     RGSCHLOGERROR(cell->instIdx,ERRCLS_INT_PAR,ERG011,
                           (ErrVal)msgLen,
                           "rgSCHGomHndlWarningSiCfg():msgLen does not match\
                           any valid TB Size.");
                     RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"Warning SI Cfg Failed" 
                           "for siId = %d", warningSiCfgReqInfo->siId);
                     rgSCHUtlFreeWarningSiSeg(reg, pool, 
                           &warningSiCfgReqInfo->siPduLst);
                     SPutSBuf(reg, pool, (Data *)warningSiCfgReqInfo, 
                           sizeof(RgrWarningSiCfgReqInfo)); 
                     warningSiCfgReqInfo = NULLP;
                     rgSCHUtlRgrWarningSiCfgCfm(instCb->rgSchInit.inst, spId,
                           siId, transId,cfmStatus); 
                     RETVALUE(RFAILED);

                  }
               }
               pduNode[j].mcs = mcs;
               pduNode[j].nPrb = nPrb;
               pduNode[j].msgLen = msgLen;
               /* ccpu00136659: CMAS ETWS design changes */
               cmLListAdd2Tail(&warningSiMsg->segLstCp, &pduNode[j].lnk);
               pduNode[j].lnk.node = (PTR)&pduNode[j];
               j++;
               node = node->next;
            }

            /* ccpu00132385-  nodes in received SI config linked list should 
             * be freed after processing the config.*/
            while(warningSiCfgReqInfo->siPduLst.first != NULLP)
            {
               node = warningSiCfgReqInfo->siPduLst.first;
               cmLListDelFrm(&(warningSiCfgReqInfo->siPduLst), node);
               SPutSBuf(reg, pool, (Data *)node,sizeof(CmLList));
               node = NULLP;
            } 

            cell->siCb.warningSi[idx].siId = warningSiCfgReqInfo->siId;
            cell->siCb.warningSi[idx].idx = idx;
            cell->siCb.siArray[warningSiCfgReqInfo->siId-1].si = 
               &cell->siCb.warningSi[idx];
            cell->siCb.siArray[warningSiCfgReqInfo->siId-1].isWarningSi =
               TRUE;
            freeNodeFound = TRUE;
            break;
         }
      }
   }

   if (freeNodeFound == FALSE)
   {
      RLOG_ARG0(L_DEBUG,DBG_CELLID,cell->cellId,"No SI Index is free");
      rgSCHUtlFreeWarningSiSeg(reg, pool, &warningSiCfgReqInfo->siPduLst);
      SPutSBuf(reg, pool, (Data *)warningSiCfgReqInfo, 
            sizeof(RgrWarningSiCfgReqInfo)); 
      warningSiCfgReqInfo = NULLP;
      rgSCHUtlRgrWarningSiCfgCfm(instCb->rgSchInit.inst, spId, siId, transId, 
            cfmStatus); 
      RETVALUE(RFAILED);
   }

   SPutSBuf(reg, pool, (Data *)warningSiCfgReqInfo, 
         sizeof(RgrWarningSiCfgReqInfo)); 
   warningSiCfgReqInfo = NULLP;
   RETVALUE(ROK);
}


/**
 * @brief Handler to handle SI Stop request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlWarningSiStopReq
 *     
 *     This API handles processing for SI stop request from RRM to MAC.
 *     
 *  @param[in]  Region        reg
 *  @param[in]  Pool          pool
 *  @param[in]  RgSchCb       *instCb
 *  @param[in]  SpId          siId
 *  @return  void
 **/
#ifdef ANSI
PUBLIC Void rgSCHGomHndlWarningSiStopReq 
(
Region        reg,
Pool          pool,
RgSchCb       *instCb,
U8            siId,
RgrCfgTransId transId,
SpId          spId
)
#else
PUBLIC Void rgSCHGomHndlWarningSiStopReq(reg, pool, instCb, siId, transId, spId)
Region        reg;
Pool          pool;
RgSchCb       *instCb;
U8            siId;
RgrCfgTransId transId;
SpId          spId;
#endif
{
   RgSchCellCb        *cell = instCb->rgrSap[spId].cell;
   U16                idx;
   CmLList            *node;
   RgSchWarningSiPdu  *warningSiPdu;
   Buffer             *pdu;

   TRC3(rgSCHGomHndlWarningSiStopReq)

   for(idx = 0; idx < RGR_MAX_NUM_WARNING_SI; idx++)
   {
      if(cell->siCb.warningSi[idx].siId == siId)
      {
         if ((cmMemcmp ((U8 *)&cell->siCb.warningSi[idx].warningSiMsg.transId, 
                     (U8 *)&transId, sizeof(RgrCfgTransId))) == 0)
         {
            /* ccpu00136659: CMAS ETWS design changes */
            CM_LLIST_FIRST_NODE(&cell->siCb.warningSi[idx].warningSiMsg.segLstCp, node);
            while(node != NULLP)
            {
               /* On receiving the warning stop message, remove one by one
                * each PDU from the warning SI list
                */  
               /* ccpu00136659: CMAS ETWS design changes */
               node = (CmLList *)&cell->siCb.warningSi[idx].warningSiMsg.segLstCp.first;
               warningSiPdu = (RgSchWarningSiPdu *)node->node;
               pdu = warningSiPdu->pdu;
               cmLListDelFrm(&cell->siCb.warningSi[idx].warningSiMsg.segLstCp, node);
               RGSCH_FREE_MSG(pdu);
               node = node->next;
            }
         }
      }
   }
   RETVOID;
}

#endif/*RGR_SI_SCH */

/* LTE_ADV_FLAG_REMOVED_START */

/**
 * @brief This function sets the Phigh range for CC users corresponding to the CC Pool
 * @details
 *
 *     Function: rgSchUpdtRNTPInfo
 *
 *     Invoked by: rgSCHGomHndlLoadInf
 *
 *  @param[in]  RgSchCellCb*     cell
 *  @param[in]  RgSubFrm*     subFrm
 *  @param[in]  RgrLoadInfReqInfo   *loadInfReq
 *  @return  S16
 *
 **/
#ifdef ANSI
PUBLIC S16 rgSchUpdtRNTPInfo
(
 RgSchCellCb             *cell,
 RgSchDlSf               *sf,
 RgrLoadInfReqInfo       *loadInfReq
 )
#else
PRIVATE Void rgSchUpdtRNTPInfo(cell, sf)
   RgSchCellCb             *cell;
   RgSchDlSf               *sf;
   RgrLoadInfReqInfo       *loadInfReq;

#endif
{
   /*  Initialise the variables */
   RgSchSFRPoolInfo *sfrCCPool;
   CmLListCp   *l;
   CmLList     *n;
   S16 ret = RFAILED;

   TRC2(rgSchUpdtRNTPInfo);

   l = &sf->sfrTotalPoolInfo.ccPool;

   /*Get the first node from the CC Pool*/
   n = cmLListFirst(l);
   while(n)
   {
      sfrCCPool = (RgSchSFRPoolInfo*)n->node;
      if (sfrCCPool->poolendRB == loadInfReq->rgrCcPHighEndRb)
      {
         sfrCCPool->pwrHiCCRange.endRb   = loadInfReq->rgrCcPHighEndRb;
         sfrCCPool->pwrHiCCRange.startRb = loadInfReq->rgrCcPHighStartRb;
         RETVALUE(ROK);
      }
      else
      {
         n = cmLListNext(l);
      }
   }
   RETVALUE(ret);
}
/**
 * @brief Handler to handle LOAD INF request from RRM to MAC.
 *
 * @details
 *
 *     Function: rgSCHGomHndlLoadInf
 *
 *     This API handles processing for LOAD INF request from RRM to MAC.
 *
 *     - Processing Steps:
 *        - Validate LOAD INF request parameters at CFG module.
 *          Call rgSCHCfgVldtRgrLoadInf for SI configuration.
 *        - If validated successfully, send configuration request.
 *
 *  @param[in]  Region            reg
 *  @param[in]  Pool              pool
 *  @param[in]  RgSchCb           *instCb
 *  @param[in]  SpId              spId
 *  @param[in]  RgrCfgTransId     transId
 *  @param[in]  RgrLoadInfReqInfo *loadInfReq
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgSCHGomHndlLoadInf
(
 Region            reg,
 Pool              pool,
 RgSchCb           *instCb,
 SpId              spId,
 RgrCfgTransId     transId,
 RgrLoadInfReqInfo *loadInfReq
 )
#else
PUBLIC S16 rgSCHGomHndlLoadInf(reg, pool, instCb, spId, transId, loadInfReq)
   Region            reg;
   Pool              pool;
   RgSchCb           *instCb;
   SpId              spId;
   RgrCfgTransId     transId;
   RgrLoadInfReqInfo *loadInfReq;
#endif
{
   S16          ret;
   RgSchCellCb  *cell = instCb->rgrSap[spId].cell;
   Inst         inst  = (instCb->rgSchInit.inst );
   RgSchErrInfo errInfo;
   U16 i;

   TRC2(rgSCHGomHndlLoadInf);


   /* check if cell does not exists */
   if (((U8 *)cell == NULLP) || (cell->cellId != loadInfReq->cellId))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,loadInfReq->cellId,"Cell Control block does not exist"
             "for load cellId:%d",loadInfReq->cellId);
      SPutSBuf(reg, pool, (Data *)loadInfReq, (Size)sizeof(*loadInfReq));
      RETVALUE(RFAILED);
   }

   if (cell->lteAdvCb.dsfrCfg.status == RGR_DISABLE)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId, "rgSCHGomHndlLoadInf(): DSFR Feature not enabled");
      SPutSBuf(reg, pool, (Data *)loadInfReq, (Size)sizeof(*loadInfReq));
      RETVALUE(RFAILED);
   }
   /* Validate the received LOAD INF Configuration */
   ret = rgSCHCfgVldtRgrLoadInf(inst, loadInfReq, cell, &errInfo);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Rgr LOAD INF Configuration "
               "validation FAILED");
      SPutSBuf(reg, pool, (Data *)loadInfReq, (Size)sizeof(*loadInfReq));
      RETVALUE(RFAILED);
   }
   /* Update the RNTP info rcvd in the respective cell centre pool so that Phigh can be
      sent for the UEs scheduled in that particular RB range*/
   for(i = 0; i < RGSCH_NUM_DL_SUBFRAMES; i++)
   {
      if((rgSchUpdtRNTPInfo(cell, cell->subFrms[i], loadInfReq) != ROK))
      {
         RETVALUE(RFAILED);
      }
   }

   SPutSBuf(reg, pool, (Data *)loadInfReq, (Size)sizeof(*loadInfReq));


   RETVALUE(ROK);
}  /* rgSCHGomHndlLoadInf */
/* LTE_ADV_FLAG_REMOVED_END */

/**********************************************************************
 
         End of file:     gk_sch_gom.c@@/main/2 - Sat Jul 30 02:21:41 2011
 
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
/main/1      ---     sd   1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments. 
             rg003.201 ns 1. Trace added to functions & Removed extra comments.
             rg004.201 rnt 1. Modified for SI Enhancement
             rg009.201 adi 1. ccpu00114920:add paddying bytes if application
                              provided PDU does not match a pre-defined TB sz
                    nudupi 1.Added the code for Insure fixes.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1
$SID$        ---     rt      1. LTE MAC 4.1 release
*********************************************************************91*/
