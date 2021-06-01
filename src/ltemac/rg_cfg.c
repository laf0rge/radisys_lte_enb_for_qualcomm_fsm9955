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
 
     File:     rg_cfg.c
 
     Sid:      gk_cfg.c@@/main/3 - Sat Jul 30 02:21:24 2011
 
     Prg:     sd
 
**********************************************************************/

/** @file rg_cfg.c
@brief This module handles the configuration of MAC by RRC and RRM.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_FILE_ID=180;
static int RLOG_MODULE_ID=4096;

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
#include "crg.h"
#include "rgu.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"
#include "rg.h"
#include "rg_err.h"

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
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"           /* layer management typedefs for MAC */
#include "rg.x"            /* typedefs for MAC */

/* LTE-MAC Control Block Structure */
PUBLIC RgCb rgCb;

/* local defines */
PRIVATE S16 rgCFGVldtCrgDedLcCfg ARGS((CrgLchCfg *lcCfg, RgCellCb **cell,
         RgUeCb **ue, RgErrInfo *errInfo));
PRIVATE S16 rgCFGVldtCrgCmnLcCfg ARGS((CrgLchCfg *lcCfg, RgCellCb **cell,
         RgErrInfo *errInfo));
PRIVATE S16 rgCFGCrgDedLcCfg ARGS((RgCellCb *cell, RgUeCb *ue,
         CrgLchCfg *lcCfg, RgErrInfo *errInfo));
PRIVATE S16 rgCFGCrgCmnLcCfg ARGS((RgCellCb *cell, CrgLchCfg *lcCfg,
         RgErrInfo *errInfo));

PRIVATE Void rgCFGFreeCmnLcLst ARGS((RgCellCb *cell));
PRIVATE Void rgCFGFreeUeLst ARGS((RgCellCb *cell));
/* Added support for SPS*/
#ifdef LTEMAC_SPS
PRIVATE Void rgCFGFreeSpsUeLst ARGS((RgCellCb *cell));
#endif /* LTEMAC_SPS */
#ifdef LTE_L2_MEAS /*LTE_L2_MEAS_PHASE2*/
PRIVATE S16 rgAddToL2MeasPerQci ARGS ((RgCellCb  *cell,U8 qci));
#endif

/* local typedefs */
 
/* local externs */
 
/* forward references */




/**
 * @brief Validates the cell configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgCellCfg
 *
 *     Processing Steps:
 *      - Validate the cell configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  CrgCellCfg  *cellCfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgCellCfg
(
CrgCellCfg  *cellCfg,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgCellCfg(cellCfg, errInfo)
CrgCellCfg  *cellCfg;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgCellCfg);

   errInfo->errCause = RGERR_CFG_INVALID_CRG_CELL_CFG;

   if ((rgDBMGetCellCb(cellCfg->cellId) != NULLP)
         || rgDBMGetInactvCellCb(cellCfg->cellId) != NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Cell already exists");
      RETVALUE(RFAILED);
   }
   if ((cellCfg->bwCfg.dlTotalBw < RG_MIN_DL_BW
            || cellCfg->bwCfg.dlTotalBw > RG_MAX_DL_BW)
         || (cellCfg->bwCfg.ulTotalBw < RG_MIN_UL_BW
            || cellCfg->bwCfg.ulTotalBw > RG_MAX_UL_BW))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID,cellCfg->cellId, 
            "Invalid Bandwidth configuration: ul %d dl %d",
            cellCfg->bwCfg.ulTotalBw, cellCfg->bwCfg.dlTotalBw);
      RETVALUE(RFAILED);
   }
   if (cellCfg->rachCfg.maxMsg3Tx < RG_MIN_HQ_TX)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellCfg->cellId,
                "Invalid RACH configuration: maxMsg3Tx %d",cellCfg->rachCfg.maxMsg3Tx);
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGVldtCrgCellCfg */


/**
 * @brief Validates the UE configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgUeCfg
 *
 *     Processing Steps:
 *      - Validate the UE configuration request from RRC to MAC at CFG:
 *        validate the value range for the configured values.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell of UE.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  CrgUeCfg  *ueCfg
 *  @param[out] RgCellCb  **cell
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgUeCfg
(
CrgUeCfg  *ueCfg,
RgCellCb  **cell,
RgErrInfo *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgUeCfg(ueCfg, cell, errInfo)
CrgUeCfg  *ueCfg;
RgCellCb  **cell;
RgErrInfo *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgUeCfg);

   errInfo->errCause = RGERR_CFG_INVALID_CRG_UE_CFG;
   if ((ueCfg->txMode.pres == PRSNT_NODEF) && 
       (ueCfg->txMode.tm == CRG_UE_TM_5))
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti,"Transmission Mode=%d not supported",
            ueCfg->txMode.tm);
      RETVALUE(RFAILED);
   }
   
   /* Fetch the Active cell */
   if ((*cell = rgDBMGetCellCb(ueCfg->cellId)) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti,"Active Cell does not exist for cellId%d",
            ueCfg->cellId);
      RETVALUE(RFAILED);
   }
   /* Check if Ue already configured */
   if (rgDBMGetUeCb(*cell, ueCfg->crnti) != NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CRNTI,ueCfg->crnti,"Ue already exists");
      RETVALUE(RFAILED);
   }

   if (ueCfg->ueUlHqCfg.maxUlHqTx < RG_MIN_HQ_TX)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCfg->crnti, "Invalid Uplink HARQ config %d ",
            ueCfg->ueUlHqCfg.maxUlHqTx);
      RETVALUE(RFAILED);
   }
   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGVldtCrgUeCfg */


/**
 * @brief Validates the logical channel configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgLcCfg
 *
 *     Processing Steps:
 *      - Validate the logical channel configuration request from RRC to
 *        MAC at CFG: validate if configured values are within the range.
 *      - If validated successfully,
 *        - Return ROK and pointer to the cell for common channels. Return
 *          pointer to cell and UE for dedicated logical channels.
 *      - Else 
 *        - Return RFAILED.
 *
 *  @param[in]  CrgLchCfg  *lcCfg
 *  @param[out] RgCellCb   **cell
 *  @param[out] RgUeCb     **ue
 *  @param[out] RgErrInfo  *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgLcCfg
(
CrgLchCfg  *lcCfg,
RgCellCb   **cell,
RgUeCb     **ue,
RgErrInfo  *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgLcCfg(lcCfg, cell, ue, errInfo)
CrgLchCfg  *lcCfg;
RgCellCb   **cell;
RgUeCb     **ue;
RgErrInfo  *errInfo;
#endif
{

   TRC2(rgCFGVldtCrgLcCfg);

   if (lcCfg->lcType == CM_LTE_LCH_DTCH || lcCfg->lcType == CM_LTE_LCH_DCCH)
   {
      /* Dedicated logical channels */
      if ((rgCFGVldtCrgDedLcCfg(lcCfg, cell, ue, errInfo)) != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CRNTI,lcCfg->crnti,"Validation for dedicated LC failed");
         RETVALUE(RFAILED);
      }
   }
   else if (lcCfg->lcType == CM_LTE_LCH_BCCH
         || lcCfg->lcType == CM_LTE_LCH_PCCH
         || lcCfg->lcType == CM_LTE_LCH_CCCH)
   {
      if ((rgCFGVldtCrgCmnLcCfg(lcCfg, cell, errInfo)) != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CRNTI,lcCfg->crnti,"Validation for common logical channels failed");
         RETVALUE(RFAILED);
      }
   }
   else
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,lcCfg->crnti,"Invalid logical channel type %d",
                lcCfg->lcType);
      RETVALUE(RFAILED);
   }
#ifdef LTE_L2_MEAS
   if ( lcCfg->qci <  RG_QCI_MIN ||
        lcCfg->qci >  RG_QCI_MAX
      )
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,lcCfg->crnti,"Invalid qci %x",lcCfg->qci);
      RETVALUE(RFAILED);
   }
   /*validate qci */
#endif /*LTE_L2_MEAS */

   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGVldtCrgLcCfg */


/**
 * @brief Validates the cell re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgCellRecfg
 *
 *     Processing Steps:
 *      - Retrieve the cell control block.
 *      - If successful,
 *        - Validate the range of re-configured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  CrgCellRecfg  *cellRecfg
 *  @param[out] RgCellCb      **cell
 *  @param[out] RgErrInfo     *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgCellRecfg
(
CrgCellRecfg  *cellRecfg,
RgCellCb      **cell,
RgErrInfo     *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgCellRecfg(cellRecfg, cell, errInfo)
CrgCellRecfg  *cellRecfg;
RgCellCb      **cell;
RgErrInfo     *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgCellRecfg);

   errInfo->errCause = RGERR_CFG_INVALID_CRG_CELL_RECFG;
   
   if (((*cell = rgDBMGetCellCb(cellRecfg->cellId)) == NULLP)
         && ((*cell = rgDBMGetInactvCellCb(cellRecfg->cellId)) == NULLP))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellRecfg->cellId,"Cell does not exist");
      RETVALUE(RFAILED);
   }
   if (cellRecfg->rachRecfg.maxMsg3Tx < RG_MIN_HQ_TX)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cellRecfg->cellId,
                "Invalid RACH configuration: maxMsg3Tx %d",cellRecfg->rachRecfg.maxMsg3Tx);
      RETVALUE(RFAILED);
   }
   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGVldtCrgCellRecfg */


/**
 * @brief Validates the UE re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgUeRecfg
 *
 *     Processing Steps:
 *      - Retrieve the UE control block.
 *      - If successful,
 *        - Validate the range of re-configured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell and ue.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  CrgUeRecfg *ueRecfg
 *  @param[out] RgCellCb   **cell
 *  @param[out] RgUeCb     **ue
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgUeRecfg
(
CrgUeRecfg  *ueRecfg,
RgCellCb    **cell,
RgUeCb      **ue,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgUeRecfg(ueRecfg, cell, ue, errInfo)
CrgUeRecfg  *ueRecfg;
RgCellCb    **cell;
RgUeCb      **ue;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgUeRecfg);

   errInfo->errCause = RGERR_CFG_INVALID_CRG_UE_RECFG;
   
   if ((ueRecfg->txMode.pres == PRSNT_NODEF) && 
       (ueRecfg->txMode.tm == CRG_UE_TM_5))
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Transmission Mode=%d not supported",
                ueRecfg->txMode.tm);
      RETVALUE(RFAILED);
   }

   /* Fetch the Active cell */
   if ((*cell = rgDBMGetCellCb(ueRecfg->cellId)) == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Active Cell does not exist");
      RETVALUE(RFAILED);
   }

   /* Fix : syed UE ID change at MAC will now be controlled
    * by SCH. */
   if ((*ue = rgDBMGetUeCb(*cell, ueRecfg->oldCrnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,"[%d]Old Ue does not exist", ueRecfg->oldCrnti);
      RETVALUE(RFAILED);
   }
   if (ueRecfg->ueUlHqRecfg.maxUlHqTx < RG_MIN_HQ_TX)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,ueRecfg->cellId,"Invalid Uplink HARQ config for UE %d",
            ueRecfg->ueUlHqRecfg.maxUlHqTx);
      RETVALUE(RFAILED);
   }
   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGVldtCrgUeRecfg */


/**
 * @brief Validates the logical channel re-configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgLcRecfg
 *
 *     Processing Steps:
 *      - Retrieve the uplink and downlink logical channel control block.
 *      - If successful,
 *        - Validate the range of re-configured values recieved in
 *          re-configuration request.
 *        - If validated successfully,
 *          - Return ROK and pointer to the cell, UE and logical channel.
 *        - Else 
 *          - Return RFAILED.
 *      - Else return RFAILED.
 *
 *  @param[in]  CrgLchRecfg  *lcRecfg
 *  @param[out] RgCellCb     **cell
 *  @param[out] RgUeCb       **ue
 *  @param[out] RgUlLcCb     **ulLc
 *  @param[out] RgDlLcCb     **dlLc
 *  @param[out] RgErrInfo    *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgLcRecfg
(
CrgLchRecfg *lcRecfg,
RgCellCb    **cell,
RgUeCb      **ue,
RgUlLcCb    **ulLc,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgLcRecfg(lcRecfg, cell, ue, ulLc, errInfo)
CrgLchRecfg  *lcRecfg;
RgCellCb     **cell;
RgUeCb       **ue;
RgUlLcCb     **ulLc;
RgErrInfo    *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgLcRecfg);

   errInfo->errCause = RGERR_CFG_INVALID_CRG_LC_RECFG;

   /* Fetch the cell */
   if ((*cell = rgDBMGetCellCb(lcRecfg->cellId)) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,lcRecfg->crnti,"Active Cell does not exist %d",lcRecfg->cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue for dedicated channels */
   if ((*ue = rgDBMGetUeCb(*cell, lcRecfg->crnti)) == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CRNTI,lcRecfg->crnti,"Ue does not exist for dedicated logical channel");
      RETVALUE(RFAILED);
   }

   if ((*ulLc = rgDBMGetUlDedLcCb((*ue), lcRecfg->lcId)) == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CRNTI,lcRecfg->crnti,"Dedicated UL LC does not exist %d",lcRecfg->lcId);
      RETVALUE(RFAILED);
   }

   if (lcRecfg->ulRecfg.lcgId > (RG_MAX_LCG_PER_UE - 1))
   {
      RLOG_ARG2(L_ERROR,DBG_CRNTI,lcRecfg->crnti,"Invalid lcgId for uplink logical channel lcg %d lc %d",
                lcRecfg->ulRecfg.lcgId, lcRecfg->lcId);
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGVldtCrgLcRecfg */

/* Start: LTEMAC_2.1_DEV_CFG */
/**
 * @brief Validates the UE Reset request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGVldtCrgUeReset
 *
 *     Processing Steps:
 *      - Retrieve the CELL control block.
 *      - If Failue,
 *          - Return RFAILED.
 *      - Retrieve the UE control block.
 *      - If Failue,
 *          - Return RFAILED.
 *
 *  @param[in]  CrgRst     *reset,
 *  @param[out] RgCellCb   **cell
 *  @param[out] RgUeCb     **ue
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGVldtCrgUeReset
(
CrgRst     *reset,
RgCellCb    **cell,
RgUeCb      **ue,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGVldtCrgUeReset(reset, cell, ue, errInfo)
CrgRst     *reset;
RgCellCb    **cell;
RgUeCb      **ue;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGVldtCrgUeReset);

   errInfo->errCause = RGERR_CFG_INVALID_CRG_UE_RESET;
   
   /* Fetch the Active cell */
   if ((*cell = rgDBMGetCellCb(reset->cellId)) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,reset->crnti,"Active Cell does not exist %d",reset->cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((*ue = rgDBMGetUeCb(*cell, reset->crnti)) == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CRNTI,reset->crnti,"UE does not exist");
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGVldtCrgUeReset*/
/* End: LTEMAC_2.1_DEV_CFG */



/**
 * @brief Handler for the cell configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgCellCfg
 *
 *     Processing Steps:
 *      - Allocate and create cell control block.
 *      - Update cell control block with the values recieved in the
 *        configuration.
 *      - Add the control block to hash list of cells.
 *      - Update the statistics.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  CrgCellCfg  *cellCfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgCellCfg
(
CrgCellCfg  *cellCfg,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgCellCfg(cellCfg, errInfo)
CrgCellCfg  *cellCfg;
RgErrInfo   *errInfo;
#endif
{
   S16        ret;
   RgCellCb   *cell = NULLP;
   U8         idx;
   /* RLC SAP to allocate flowCntrlInd buffer*/
   Pst        *pst = &rgCb.rguSap[rgRguDlSap].sapCfg.sapPst;

   TRC2(rgCFGCrgCellCfg);

   errInfo->errCause = RGERR_CFG_CRG_CELL_CFG;
   
   /* Allocate the cell control block */
   if((ret = rgAllocSBuf((Data**)&cell, sizeof(RgCellCb))) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"Memory allocation FAILED for cell");
      RETVALUE(RFAILED);
   }
   if (cell == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId, "Memory allocation FAILED for cell");
      RETVALUE(RFAILED);
   }

   /* Initialize the cell */
   cell->cellId  = cellCfg->cellId;
   cell->rachCfg = cellCfg->rachCfg;
   cell->bwCfg   = cellCfg->bwCfg;

   /* Initialize UL and DL CCCH logical channels */
   cell->ulCcchId = RG_INVALID_LC_ID;
   cell->dlCcchId = RG_INVALID_LC_ID;

   
   /* Initialize the lists of the cell */
   ret = rgDBMInitCell(cell);
   if (ret != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCfg->cellId,"DBM initialization for cell failed");
      rgCFGFreeInactvCellCb(cell);
      RETVALUE(RFAILED);
   }

   for(idx=0;idx < RG_NUM_SUB_FRAMES; idx++)
   {
      cell->subFrms[idx].txDone = TRUE;
   }
   
   /* Insert cell in the incative cell list */
   rgDBMInsInactvCellCb(cell);
#ifdef LTE_L2_MEAS
   cmLListInit(&cell->l2mList);
   for(idx = 0; idx < RG_NUM_UL_SUB_FRAMES; idx++)
   {
      cmMemset((U8 *)&cell->ulSf[idx], 0, sizeof(RgUlSf));
   }

   cell->ttiCycle = (U32)RG_TTI_CYCLE_INVLD;   
#endif
   /* Update Statistics */
   rgUpdtCellCnt(RG_CFG_ADD);
   errInfo->errCause = RGERR_NONE;
  
   /* Allocate a buffer for flowCntrlInd.*/
   SGetSBuf(pst->region, pst->pool, (Data **)&cell->flowCntrlInd, 
              sizeof(RguFlowCntrlInd));

   RETVALUE(ROK);
}  /* rgCFGCrgCellCfg */


/**
 * @brief Handler for the UE configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeCfg
 *
 *     Processing Steps:
 *      - Allocate and create UE control block.
 *      - Update UE control block with the values recieved in the
 *        configuration.
 *      - Invoke RAM, SCH, UHM and DHM with created UE control block, to
 *        update random access, scheduler, uplink harq and downlink harq
 *        specific information respectively.
 *      - If successful, add the control block to hash list of UEs for the cell
 *        else Rollback and FAIL.
 *
 *  @param[in]  RgCellCb  *cell
 *  @param[in]  CrgUeCfg  *ueCfg
 *  @param[out] RgErrInfo *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgUeCfg
(
RgCellCb  *cell,
CrgUeCfg  *ueCfg,
RgErrInfo *errInfo
)
#else
PUBLIC S16 rgCFGCrgUeCfg(cell, ueCfg, errInfo)
RgCellCb  *cell;
CrgUeCfg  *ueCfg;
RgErrInfo *errInfo;
#endif
{
   RgUeCb    *ue = NULLP;
   Bool      handover = FALSE;

   TRC2(rgCFGCrgUeCfg);

   errInfo->errCause = RGERR_CFG_CRG_UE_CFG;

/* Start: LTEMAC_2.1_DEV_CFG */
   if ((ue = rgDBMGetUeCbFromRachLst(cell, ueCfg->crnti)) == NULLP)
   {
      /* Create UeCb and Insert in Rach List */
      if((ue=rgRAMCreateUeCb(cell, ueCfg->crnti, FALSE, errInfo)) == NULLP)
      {
         RLOG_ARG0(L_ERROR,DBG_CRNTI,ueCfg->crnti,"UeCb creation failed");
         RETVALUE(RFAILED);
      }
      if(rgDHMHqEntInit(&ue->dl.hqEnt, cell->maxDlHqProcPerUe) != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CRNTI,ueCfg->crnti,"UeCb Harq Entity Initialization failed");
         RETVALUE(RFAILED);
      }
      handover = TRUE;
   }
/* End: LTEMAC_2.1_DEV_CFG */

   if(handover == FALSE)
   {
      /* Remove from the rachLst */
      rgDBMDelUeCbFromRachLst(cell, ue);
   }

   /* Initialize uplink HARQ related information for UE */
   rgUHMCrgUeCfg(cell, ue, ueCfg);

   rgDBMInsUeCb(cell, ue);

   /* Update satistics */
   rgUpdtUeCnt(RG_CFG_ADD);
   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGCrgUeCfg */


/**
 * @brief Handler for the logical channel configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgLcCfg
 *
 *     Processing Steps:
 *      - Check if the configuration is for dedicated or common logical channel.
 *      - For Dedicated logical channels:
 *        - Update the dedicated logical channel Cb with the configured values.
 *        - Invoke SCH will cell, UE and logical channel Cb to update scheduler
 *          specific information.
 *      - For Common logical channels:
 *        - Update the common logical channel Cb with the configured values.
 *        - Move cell to active list of cells if cell becomes ACTIVE.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgCellCb    *cell
 *  @param[in]  RgUeCb      *ue
 *  @param[in]  CrgLchCfg   *lcCfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgLcCfg
(
RgCellCb    *cell,
RgUeCb      *ue,
CrgLchCfg   *lcCfg,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgLcCfg(cell, ue, lcCfg, errInfo)
RgCellCb    *cell;
RgUeCb      *ue;
CrgLchCfg   *lcCfg;
RgErrInfo   *errInfo;
#endif
{

   TRC2(rgCFGCrgLcCfg);
   
   /* Handle Config for dedicated/common logical channels */
   if (lcCfg->lcType == CM_LTE_LCH_DTCH || lcCfg->lcType == CM_LTE_LCH_DCCH)
   {
      if ((rgCFGCrgDedLcCfg(cell, ue, lcCfg, errInfo)) != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CRNTI,lcCfg->crnti,
               "Dedicated logical channel configuration failed %d",lcCfg->lcId);
         RETVALUE(RFAILED);
      }
   }
   else
   {
      if ((rgCFGCrgCmnLcCfg(cell, lcCfg, errInfo)) != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CRNTI,lcCfg->crnti,
                  "Common logical channel configuration failed %d",lcCfg->lcId);
         RETVALUE(RFAILED);
      }
   }

   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGCrgLcCfg */


/**
 * @brief Handler for the cell re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgCellRecfg
 *
 *     Processing Steps:
 *      - Invoke SCH with updated Cell Cb to update scheduler specific
 *        parameters.
 *      - Update the cell Cb with the reconfigured values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgCellCb      *cell
 *  @param[in]  CrgCellRecfg  *cellRecfg
 *  @param[out] RgErrInfo     *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgCellRecfg
(
RgCellCb      *cell,
CrgCellRecfg  *cellRecfg,
RgErrInfo     *errInfo
)
#else
PUBLIC S16 rgCFGCrgCellRecfg(cell, cellRecfg, errInfo)
RgCellCb      *cell;
CrgCellRecfg  *cellRecfg;
RgErrInfo     *errInfo;
#endif
{
   TRC2(rgCFGCrgCellRecfg);

   /* Store the given rachCfg */
   cell->rachCfg = cellRecfg->rachRecfg;

   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGCrgCellRecfg */


/**
 * @brief Handler for the UE re-configuration request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeRecfg
 *
 *     Processing Steps:
 *      - If rnti changes,
 *        - Invoke RAM for UE reconfiguration.
 *        - Delete old UE from the list.
 *        - Update the new rnti and re-insert the UE in the list.
 *      - Update the UE control block with the reconfigured values.
 *      - Invoke SCH, UHM and DHM with updated UE control block to 
 *        update scheduler, uplink HARQ and downlink HARQ specific
 *        parameters.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgCellCb    *cell
 *  @param[in]  RgUeCb      *ue
 *  @param[in]  CrgUeRecfg  *ueRecfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgUeRecfg
(
RgCellCb    *cell,
RgUeCb      *ue,
CrgUeRecfg  *ueRecfg,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgUeRecfg(cell, ue, ueRecfg, errInfo)
RgCellCb    *cell;
RgUeCb      *ue;
CrgUeRecfg  *ueRecfg;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGCrgUeRecfg);

   errInfo->errCause = RGERR_CFG_CRG_UE_RECFG;

   /* Fix : syed UE ID change at MAC will now be controlled
    * by SCH. */

   /* Update uplink HARQ related information for UE */
   rgUHMCrgUeRecfg(cell, ue, ueRecfg);

   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGCrgUeRecfg */


/**
 * @brief Handler for the logical channel re-configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgLcRecfg
 *
 *     Processing Steps:
 *      - Invoke scheduler to update scheduler specific information.
 *      - Update the dedicated logical channel Cb with the re-configured
 *        values.
 *      - If successful, return ROK else RFAILED.
 *
 *  @param[in]  RgUlCellCb  *cell
 *  @param[in]  RgUlUeCb    *ue
 *  @param[in]  RgUlLcCb    *ulLc
 *  @param[in]  RgDlLcCb    *dlLc
 *  @param[in]  CrgLchRecfg *lcRecfg
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgLcRecfg
(
RgCellCb    *cell,
RgUeCb      *ue,
RgUlLcCb    *ulLc,
CrgLchRecfg *lcRecfg,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgLcRecfg(cell, ue, ulLc, lcRecfg, errInfo)
RgCellCb    *cell;
RgUeCb      *ue;
RgUlLcCb    *ulLc;
CrgLchRecfg *lcRecfg;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGCrgLcRecfg);

   if (ulLc->lcgId != lcRecfg->ulRecfg.lcgId)
   {
      rgDBMUpdUlDedLcCb(ue, ulLc, lcRecfg->ulRecfg.lcgId);
   }

   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGCrgLcRecfg */

/* Start: LTEMAC_2.1_DEV_CFG */
/**
 * @brief Handler for the logical channel re-configuration request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeReset
 *
 *     Processing Steps:
 *
 *  @param[in]  RgUlCellCb  *cell
 *  @param[in]  RgUlUeCb    *ue
 *  @param[in]  CrgRst     *reset
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgUeReset
(
RgCellCb    *cell,
RgUeCb      *ue,
CrgRst     *reset,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgUeReset(cell, ue, reset, errInfo)
RgCellCb    *cell;
RgUeCb      *ue;
CrgRst     *reset;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGCrgUeReset);

   rgDHMUeReset(&ue->dl.hqEnt);

   errInfo->errCause = RGERR_NONE;

   RETVALUE(ROK);
}  /* rgCFGCrgUeReset */
/* End: LTEMAC_2.1_DEV_CFG */

/**
 * @brief Handler for the cell delete request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgCellDel
 *
 *     Processing Steps:
 *      - Fetch the cell control block.
 *      - Remove the cell control block from the hash list of cells.
 *      - Free the cell control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  CrgDel      *cellDelInfo
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgCellDel
(
CrgDel      *cellDelInfo,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgCellDel(cellDelInfo, errInfo)
CrgDel      *cellDelInfo;
RgErrInfo   *errInfo;
#endif
{
   RgCellCb      *cell;
   U8 idx;

   TRC2(rgCFGCrgCellDel);

   errInfo->errCause = RGERR_CFG_CRG_CELL_DEL;

   if ((cell = rgDBMGetCellCb(cellDelInfo->u.cellDel.cellId)) == NULLP)
   {
      if ((cell = rgDBMGetInactvCellCb(cellDelInfo->u.cellDel.cellId))
            == NULLP)
      {

         RLOG_ARG0(L_ERROR,DBG_CELLID,cellDelInfo->u.cellDel.cellId,"Cell does not exist");
         RETVALUE(RFAILED);
      }

      /* Delete cell from inactive list */
      rgDBMDelInactvCellCb(cell);

      /* Free the inactive cell */
      rgCFGFreeInactvCellCb(cell);

      errInfo->errCause = RGERR_NONE;
      RETVALUE(ROK);
   }

   /* Delete from the cell list */
   rgDBMDelCellCb(cell);

   for(idx=0;idx < RG_NUM_SUB_FRAMES; idx++)
   {
      rgTOMRlsSf(&cell->subFrms[idx]);
   }

   /* Free the active cell */
   rgCFGFreeCellCb(cell);
   rgCb.prevCell = NULLP;

   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGCrgCellDel */


/**
 * @brief Handler for the UE delete request from RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgUeDel
 *
 *     Processing Steps:
 *      - Fetch the UE control block.
 *      - Remove the UE control block from the hash list of UEs for the cell.
 *      - Free the UE control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  CrgDel      *ueDelInfo
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgUeDel
(
CrgDel      *ueDelInfo,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgUeDel(ueDelInfo, errInfo)
CrgDel      *ueDelInfo;
RgErrInfo   *errInfo;
#endif
{
   TRC2(rgCFGCrgUeDel);

   errInfo->errCause = RGERR_CFG_CRG_UE_DEL;

   if (rgDBMGetCellCb(ueDelInfo->u.ueDel.cellId) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,ueDelInfo->u.ueDel.crnti,"Cell does not exist %d",
                ueDelInfo->u.ueDel.cellId);
      RETVALUE(RFAILED);
   }
   errInfo->errCause = RGERR_NONE;
   /* Fix: syed Context Deletion is relied upon SCH indication */
   RETVALUE(ROK);
}  /* rgCFGCrgUeDel */


/**
 * @brief Handler for the logical channel delete request from
 * RRC to MAC.
 *
 * @details
 *
 *     Function : rgCFGCrgLcDel
 *
 *     Processing Steps:
 *      - Fetch the logical channel control block.
 *      - Free the logical channel control block.
 *      - If successful, return ROK else return RFAILED.
 *
 *  @param[in]  CrgDel      *lcDelInfo
 *  @param[out] RgErrInfo   *errInfo
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 **/
#ifdef ANSI
PUBLIC S16 rgCFGCrgLcDel
(
CrgDel      *lcDelInfo,
RgErrInfo   *errInfo
)
#else
PUBLIC S16 rgCFGCrgLcDel(lcDelInfo, errInfo)
CrgDel      *lcDelInfo;
RgErrInfo   *errInfo;
#endif
{
   Bool      dirVld = FALSE;
   RgCellCb  *cell;
   RgUeCb    *ue;
   RgUlLcCb  *ulLc;
   RgDlLcCb  *dlLc;

   TRC2(rgCFGCrgLcDel);

   errInfo->errCause = RGERR_CFG_CRG_LC_DEL;

   /* Fetch the Active cell */
   if ((cell = rgDBMGetCellCb(lcDelInfo->u.lchDel.cellId)) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,lcDelInfo->u.lchDel.crnti,"Cell does not exist %d",
                lcDelInfo->u.lchDel.cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((ue = rgDBMGetUeCb(cell, lcDelInfo->u.lchDel.crnti)) == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CRNTI,lcDelInfo->u.lchDel.crnti,
                "UE does not exist for dedicated logical channel");
      RETVALUE(RFAILED);
   }

   /* Validate downlink info */
   if (lcDelInfo->u.lchDel.dir & CRG_DIR_TX)
   {
      if ((dlLc = rgDBMGetDlDedLcCb(ue, lcDelInfo->u.lchDel.lcId))
            == NULLP)
      {
         RLOG_ARG1(L_ERROR,DBG_CRNTI,lcDelInfo->u.lchDel.crnti,"DL LC %d does not exist",
                   lcDelInfo->u.lchDel.lcId);
         RETVALUE(RFAILED);
      }
      rgDBMDelDlDedLcCb(ue, dlLc);
      dirVld = TRUE;
   }

   /* Validate uplink info */
   if (lcDelInfo->u.lchDel.dir & CRG_DIR_RX)
   {
      if ((ulLc = rgDBMGetUlDedLcCb(ue, lcDelInfo->u.lchDel.lcId))
            == NULLP)
      {
         RLOG_ARG1(L_ERROR,DBG_CRNTI,lcDelInfo->u.lchDel.crnti,"UL LC %d does not exist",
                   lcDelInfo->u.lchDel.lcId);
         RETVALUE(RFAILED);
      }
      rgDBMDelUlDedLcCb(ue, ulLc);
      dirVld = TRUE;
   }

   if (!dirVld)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,lcDelInfo->u.lchDel.crnti,"Invalid direction %d for LC Delete",
            lcDelInfo->u.lchDel.dir);
      RETVALUE(RFAILED);
   }

   errInfo->errCause = RGERR_NONE;
   RETVALUE(ROK);
}  /* rgCFGCrgLcDel */

/***********************************************************
 *
 *     Func : rgCFGVldtCrgDedLcCfg
 *
 *
 *     Desc : Validates dedicated logical channel configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgCFGVldtCrgDedLcCfg
(
CrgLchCfg     *lcCfg,
RgCellCb      **cell,
RgUeCb        **ue,
RgErrInfo     *errInfo
)
#else
PRIVATE S16 rgCFGVldtCrgDedLcCfg(lcCfg, cell, ue, errInfo)
CrgLchCfg     *lcCfg;
RgCellCb      **cell;
RgUeCb        **ue;
RgErrInfo     *errInfo;
#endif
{
   U8         dirVld   = FALSE;
   TRC2(rgCFGVldtCrgDedLcCfg);

   errInfo->errCause = RGERR_CFG_INVALID_CRG_DED_LC_CFG;

   /* Fetch the Active cell */
   if ((*cell = rgDBMGetCellCb(lcCfg->cellId)) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,lcCfg->crnti,"Active Cell does not exist: Cell %d",
                lcCfg->cellId);
      RETVALUE(RFAILED);
   }

   /* Fetch the Ue */
   if ((*ue = rgDBMGetUeCb(*cell, lcCfg->crnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,lcCfg->crnti,"UE  does not exist for dedicated logical channel %d",
                lcCfg->lcId);
      RETVALUE(RFAILED);
   }

   /* Validate logical channel Id */
   if ((lcCfg->lcId < RG_DEDLC_MIN_LCID)
            ||(lcCfg->lcId > RG_DEDLC_MAX_LCID))
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,lcCfg->crnti,"Invalid logical channel Id %d",
                lcCfg->lcId);
      RETVALUE(RFAILED);
   }

   /* Validate downlink info */
   if (lcCfg->dir & CRG_DIR_TX)
   {
      if (rgDBMGetDlDedLcCb((*ue), lcCfg->lcId) != NULLP)
      {
         RLOG_ARG1(L_ERROR,DBG_CRNTI,lcCfg->crnti,"UE: Dedicated DL LC %d already configured",
                    lcCfg->lcId);
         RETVALUE(RFAILED);
      }
      dirVld = TRUE;
   }

   /* Validate uplink info */
   if (lcCfg->dir & CRG_DIR_RX)
   {
      if (lcCfg->ulInfo.lcgId > (RG_MAX_LCG_PER_UE - 1))
      {
         RLOG_ARG1(L_ERROR,DBG_CRNTI,lcCfg->crnti,"UE: Invalid lcgId for uplink logical channel %d",
                   lcCfg->ulInfo.lcgId);
         RETVALUE(RFAILED);
      }
      if (rgDBMGetUlDedLcCb((*ue), lcCfg->lcId) != NULLP)
      {
         RLOG_ARG1(L_ERROR,DBG_CRNTI,lcCfg->crnti,"UE: Dedicated UL LC %d already configured",
                   lcCfg->lcId);
         RETVALUE(RFAILED);
      }
      dirVld = TRUE;
   }

   if (!dirVld)
   {
      RLOG_ARG1(L_ERROR,DBG_CRNTI,lcCfg->crnti,"Invalid Direction %d",
               lcCfg->dir);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgCFGVldtCrgDedLcCfg */


/***********************************************************
 *
 *     Func : rgCFGVldtCrgCmnLcCfg
 *
 *
 *     Desc : Validates common logical channel configuration recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgCFGVldtCrgCmnLcCfg
(
CrgLchCfg     *lcCfg,
RgCellCb      **cell,
RgErrInfo     *errInfo
)
#else
PRIVATE S16 rgCFGVldtCrgCmnLcCfg(lcCfg, cell, errInfo)
CrgLchCfg     *lcCfg;
RgCellCb      **cell;
RgErrInfo     *errInfo;
#endif
{
   U8         dirVld  = FALSE;

   TRC2(rgCFGVldtCrgCmnLcCfg);

   errInfo->errCause = RGERR_CFG_INVALID_CRG_CMN_LC_CFG;
   
   /* Ensure cell is not in the active list */
   if ((*cell = rgDBMGetCellCb(lcCfg->cellId)) != NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,lcCfg->cellId,"Active Cell exists for common channels");
      RETVALUE(RFAILED);
   }

   /* Fetch the inactive cell for common logical channels */
   if ((*cell = rgDBMGetInactvCellCb(lcCfg->cellId)) == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,lcCfg->cellId,"Inactive Cell does not exist for common channels");
      RETVALUE(RFAILED);
   }

   /* Validate downlink info */
   if (lcCfg->dir & CRG_DIR_TX)
   {
      if (lcCfg->lcType == CM_LTE_LCH_BCCH)
      {
         if (lcCfg->dlInfo.dlTrchType == CM_LTE_TRCH_DL_SCH)
         {
            if (rgDBMGetBcchOnDlsch(*cell,lcCfg->lcId) != NULLP)
            {
               RLOG_ARG0(L_ERROR,DBG_CELLID,lcCfg->cellId,"BCCH on DLSCH already configured for cell");
               RETVALUE(RFAILED);
            }
         }
         else if (lcCfg->dlInfo.dlTrchType == CM_LTE_TRCH_BCH)
         {
            if (rgDBMGetBcchOnBch(*cell) != NULLP)
            {
               RLOG_ARG0(L_ERROR,DBG_CELLID,lcCfg->cellId,"BCCH on BCH already configured for cell %d");
               RETVALUE(RFAILED);
            }
         }
         else
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,lcCfg->cellId,"Invalid transport channel %d for cell",
                  lcCfg->dlInfo.dlTrchType);
            RETVALUE(RFAILED);
         }
      }
      else if (lcCfg->lcType == CM_LTE_LCH_PCCH)
      {
         if (rgDBMGetPcch(*cell) != NULLP)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,lcCfg->cellId,"PCCH already configured for cell");
            RETVALUE(RFAILED);
         }
      }
      else if (RG_DLCCCH_ISCFGD(*cell))
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,lcCfg->cellId,"DL CCCH already configured for cell %d");
         RETVALUE(RFAILED);
      }
      dirVld = TRUE;
   }

   /* Validate uplink info */
   if (lcCfg->dir & CRG_DIR_RX)
   {
      /* Uplink CCCH */
      if (lcCfg->lcType != CM_LTE_LCH_CCCH)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,lcCfg->cellId,"Invalid UL common lcType %d for cell ",
                  lcCfg->lcType);
         RETVALUE(RFAILED);
      }
      if (RG_ULCCCH_ISCFGD(*cell))
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,lcCfg->cellId,"UL CCCH already configured for cell ");
         RETVALUE(RFAILED);
      }
      dirVld = TRUE;
   }

   /* Invalid direction */
   if (!dirVld)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,lcCfg->cellId,"Invalid Direction %d", lcCfg->dir);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgCFGVldtCrgCmnLcCfg */

/***********************************************************
 *
 *     Func : rgCFGCrgDedLcCfg
 *
 *
 *     Desc : Handles dedicated logical channel configuration 
 *     recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgCFGCrgDedLcCfg
(
RgCellCb      *cell,
RgUeCb        *ue,
CrgLchCfg     *lcCfg,
RgErrInfo     *errInfo
)
#else
PRIVATE S16 rgCFGCrgDedLcCfg(cell, ue, lcCfg, errInfo)
RgCellCb      *cell;
RgUeCb        *ue;
CrgLchCfg     *lcCfg;
RgErrInfo     *errInfo;
#endif
{
   TRC2(rgCFGCrgDedLcCfg);

   errInfo->errCause = RGERR_CFG_CRG_DED_LC_CFG;

   /* Uplink/Bi-directional logical channel */
   if (lcCfg->dir & CRG_DIR_RX)
   {
#ifdef LTE_L2_MEAS
      rgDBMInsUlDedLcCb(ue, lcCfg->lcId, lcCfg->ulInfo.lcgId, lcCfg->qci);
      cell->qciArray[lcCfg->qci].qci = lcCfg->qci;
      if(lcCfg->lcType == CM_LTE_LCH_DTCH)
      {
        rgAddToL2MeasPerQci(cell,lcCfg->qci);/*LTE_L2_MEAS_PHASE2*/ 
      }
#else
      rgDBMInsUlDedLcCb(ue, lcCfg->lcId, lcCfg->ulInfo.lcgId);
#endif
   }

   /* Downlink/Bi-directional logical channel */
   if (lcCfg->dir & CRG_DIR_TX)
   {
      rgDBMInsDlDedLcCb(ue, lcCfg->lcId);
   }
   RETVALUE(ROK);
}  /* rgCFGCrgDedLcCfg */


/***********************************************************
 *
 *     Func : rgCFGCrgCmnLcCfg
 *
 *
 *     Desc : Handles dedicated logical channel configuration 
 *     recieved from RRC.
 *
 *     Ret  : S16
 *            ROK - Success
 *            RFAILED - Failed
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE S16 rgCFGCrgCmnLcCfg
(
RgCellCb      *cell,
CrgLchCfg     *lcCfg,
RgErrInfo     *errInfo
)
#else
PRIVATE S16 rgCFGCrgCmnLcCfg(cell, lcCfg, errInfo)
RgCellCb      *cell;
CrgLchCfg     *lcCfg;
RgErrInfo     *errInfo;
#endif
{
   TRC2(rgCFGCrgCmnLcCfg);

   errInfo->errCause = RGERR_CFG_CRG_CMN_LC_CFG;

   /* Handle configuration for CCCH/BCCH/PCCH */
   if (lcCfg->lcType == CM_LTE_LCH_CCCH)
   {
      /* UL and DL CCCH configuration */
      if (lcCfg->dir & CRG_DIR_TX)
      {
         cell->dlCcchId = lcCfg->lcId;
         cell->cellActvState |= RG_DL_CCCH_CFG_DONE;
      }

      if (lcCfg->dir & CRG_DIR_RX)
      {
         cell->ulCcchId = lcCfg->lcId;
         cell->cellActvState |= RG_UL_CCCH_CFG_DONE;
      }
   }
   else
   {
      if (lcCfg->lcType == CM_LTE_LCH_BCCH)
      {
         /* BCCH on BCH and DLSCH configuration */
         if (lcCfg->dlInfo.dlTrchType == CM_LTE_TRCH_DL_SCH)
         {
            rgDBMInsBcchOnDlsch(cell, lcCfg->lcId);
            
            if(cell->cellActvState & RG_BCCH_DLSCH_CFG1_DONE)
            {
               cell->cellActvState |= RG_BCCH_DLSCH_CFG2_DONE;
            }
            else
            {
               cell->cellActvState |= RG_BCCH_DLSCH_CFG1_DONE;
            }
         }
         else
         {
            rgDBMInsBcchOnBch(cell, lcCfg->lcId);
            cell->cellActvState |= RG_BCCH_BCH_CFG_DONE;
         }
      }
      else  /* PCCH configuration */
      {
         rgDBMInsPcch(cell, lcCfg->lcId);
         cell->cellActvState |= RG_PCCH_CFG_DONE;
      }
   }

   /* Add to active cell list if cell is active */
   if (cell->cellActvState == RG_CELL_ACTIVE)
   {
      /* Delete cell from inactive list */
      rgDBMDelInactvCellCb(cell);
      /* Insert cell in the active list */
      rgDBMInsCellCb(cell);
   }

   RETVALUE(ROK);
}  /* rgCFGCrgCmnLcCfg */
#ifdef LTE_L2_MEAS
/***********************************************************
 *
 *     Func : rgCFGFreeUeUlAlloc 
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgCFGFreeUeUlAlloc
(
RgCellCb      *cell
)
#else
PRIVATE Void rgCFGFreeUeUlAlloc(cell)
RgCellCb      *cell;
#endif
{
   U8    sfIdx;
   
   TRC2(rgCFGFreeUeUlAlloc);

   for(sfIdx = 0; sfIdx < RG_NUM_UL_SUB_FRAMES; sfIdx++)
   {
      if(cell->ulSf[sfIdx].ueUlAllocInfo != NULLP)
      {
         /*ccpu00117052 - MOD- Passing double pointer for proper
          *                    NULLP assignment */
         rgFreeSBuf((Data **)&(cell->ulSf[sfIdx].ueUlAllocInfo), 
               (cell->ulSf[sfIdx].numUe * sizeof(RgUeUlAlloc)));
      }
   }
}/* rgCFGFreeUeUlAlloc */
#endif
/***********************************************************
 *
 *     Func : rgCFGFreeCellCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgCFGFreeCellCb
(
RgCellCb      *cell
)
#else
PUBLIC Void rgCFGFreeCellCb(cell)
RgCellCb      *cell;
#endif
{
   TRC2(rgCFGFreeCellCb);

   /* Free lists of the cell */
#ifdef LTEMAC_SPS
   rgCFGFreeSpsUeLst(cell);
#endif /* LTEMAC_SPS */
   rgCFGFreeUeLst(cell);
   rgRAMFreeCell(cell);
   rgCFGFreeCmnLcLst(cell);
#ifdef LTE_L2_MEAS
   rgCFGFreeUeUlAlloc(cell);
#endif
   /* ccpu00117052 - MOD - Passing double pointer for proper NULLP 
                           assignment */
   /* De-allocate the Cell */
   rgFreeSBuf((Data **)&cell, sizeof(*cell));
 
   /* Update satistics */
   rgUpdtCellCnt(RG_CFG_DEL);


  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  RETVOID; 
}  /* rgCFGFreeCellCb */


/***********************************************************
 *
 *     Func : rgCFGFreeInactvCellCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees inactive cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgCFGFreeInactvCellCb
(
RgCellCb      *cell
)
#else
PUBLIC Void rgCFGFreeInactvCellCb(cell)
RgCellCb      *cell;
#endif
{
   TRC2(rgCFGFreeInactvCellCb);
   
#if 1
   /* De-initialize the Ue list */
   rgDBMDeInitUeCbLst(cell);
#endif
#ifdef LTEMAC_SPS
   rgDBMDeInitSpsUeCbLst(cell);
#endif

   rgCFGFreeCmnLcLst(cell);

   /*ccpu00117052 - MOD- Passing double pointer for proper
                        NULLP assignment */
   /* De-allocate the Cell */
   rgFreeSBuf((Data **)&cell, sizeof(*cell));

   /* Update satistics */
   rgUpdtCellCnt(RG_CFG_DEL);


  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  RETVOID; 
}  /* rgCFGFreeInactvCellCb */


/***********************************************************
 *
 *     Func : rgCFGFreeUeCb
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgCFGFreeUeCb
(
RgCellCb    *cell,
RgUeCb      *ue
)
#else
PUBLIC Void rgCFGFreeUeCb(cell, ue)
RgCellCb    *cell;
RgUeCb      *ue;
#endif
{

   TRC2(rgCFGFreeUeCb);

   rgDHMFreeUe(&ue->dl.hqEnt);

   /* ccpu00117052 - MOD - Passing double pointer for proper NULLP
                          assignment */
   /* De-allocate the Ue */
   rgFreeSBuf((Data **)&ue, sizeof(*ue));

   /* Update Statistics */
   rgUpdtUeCnt(RG_CFG_DEL);


  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  RETVOID; 
}  /* rgCFGFreeUeCb */

/***********************************************************
 *
 *     Func : rgCFGFreeCmnLcLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees common logical channels in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgCFGFreeCmnLcLst
(
RgCellCb      *cell
)
#else
PRIVATE Void rgCFGFreeCmnLcLst(cell)
RgCellCb      *cell;
#endif
{
   TRC2(rgCFGFreeCmnLcLst);

   rgDBMFreeCmnLcLst(cell);


  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  RETVOID; 
}  /* rgCFGFreeCmnLcLst */


/***********************************************************
 *
 *     Func : rgCFGFreeUeLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees UE list in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgCFGFreeUeLst
(
RgCellCb      *cell
)
#else
PRIVATE Void rgCFGFreeUeLst(cell)
RgCellCb      *cell;
#endif
{
   RgUeCb     *ue;

   TRC2(rgCFGFreeUeLst);

   /* Free Ues in the list */
   while ((ue = rgDBMGetNextUeCb(cell, NULLP)) != NULLP)
   {
      rgDBMDelUeCb(cell, ue);
      rgCFGFreeUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgDBMDeInitUeCbLst(cell);


  /* Stack Crash Problem for TRACE5 Changes. Added return below */
  RETVOID; 
}  /* rgCFGFreeUeLst */

#ifdef LTEMAC_SPS
/***********************************************************
 *
 *     Func : rgCFGFreeSpsUeLst
 *
 *
 *     Desc :
 *     - Processing Steps:
 *        - Frees Sps UE list in cell control block.
 *
 *     Ret  : Void
 *
 *     Notes:
 *
 *     File :
 *
 **********************************************************/
#ifdef ANSI
PRIVATE Void rgCFGFreeSpsUeLst
(
RgCellCb      *cell
)
#else
PRIVATE Void rgCFGFreeSpsUeLst(cell)
RgCellCb      *cell;
#endif
{
   RgUeCb     *ue;

   TRC2(rgCFGFreeSpsUeLst);

   /* Free Ues in the list */
   while ((ue = rgDBMGetNextSpsUeCb(cell, NULLP)))
   {
      rgDBMDelSpsUeCb(cell, ue);
   }

   /* De-initialize the Ue list */
   rgDBMDeInitSpsUeCbLst(cell);

}  /* rgCFGFreeSpsUeLst */

#endif /* LTEMAC_SPS */

/**
 * @brief Function for registering cell- scheduler instance mapping
 *
 * @details
 *
 *     Function : RgSchMacCellRegReq
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     cell configuration successfully.
 *     This shall create a mapping of the cell, scheduler instance that
 *     is serving the cell and the unique identifier of the cell on the 
 *     scheduler at MAC. This mapping shall be used for further 
 *     communication to the scheduler instance for this cell.
 *     
 *           
 *  @param[in] Pst*                pst,
 *  @param[in] CmLteCellId         cellId,
 *  @param[in] RaRespReqInfo       raRespReq
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgSchMacCellRegReq
(
Pst*                pst,
RgInfCellReg*       regReq 
)
#else
PUBLIC S16 RgSchMacCellRegReq(pst, regReq)
Pst*                pst;
RgInfCellReg*       regReq;
#endif
{
   RgCellCb *cell;

   TRC3(RgSchMacCellRegReq)

   if(NULLP == regReq)
   {
      RETVALUE(RFAILED);
   }
      
   if((cell = rgDBMGetCellCb(regReq->cellId)) == NULLP)
   {
      RETVALUE(RFAILED);
   }
   if(regReq->maxDlHqProcPerUe > RG_MAX_DL_HARQ_NUM) 
   {
      RETVALUE(RFAILED);
   }
   /* Initialize */
   cell->schInstMap.cellSapId = regReq->cellSapId;
   cell->schInstMap.schInst   = pst->srcInst;
   cell->maxDlHqProcPerUe = regReq->maxDlHqProcPerUe;

   RETVALUE(ROK);

} /* end of RgSchMacCellRegReq */

/*Added Ue for Onging L2 Meas*/
#ifdef LTE_L2_MEAS
/*LTE_L2_MEAS_PHASE2*/
PRIVATE S16 rgAddToL2MeasPerQci(RgCellCb  *cell,U8 qci)
{
 S16      ret = ROK;   
 CmLList   *lnk;
 RgL2MeasCb  *measCb;
 U16          idx;
 
 lnk = cell->l2mList.first;
  while(lnk != NULLP )
   {
      measCb = (RgL2MeasCb *)lnk->node;
      if(measCb->measReq.measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
      {
         for(idx = 0;idx< measCb->measReq.t.prbReq.numQci;idx++)
         {
            if(measCb->measReq.t.prbReq.qci[idx] == qci)
            {
               break; /*exit from for loop*/
            } 
         }   
         if(idx == measCb->measReq.t.prbReq.numQci)
         {
            cell->qciArray[qci].mask = TRUE; 
            measCb->measReq.t.prbReq.qci[measCb->measReq.t.prbReq.numQci++] = qci;
         }      
      }      
      lnk = lnk->next;
   }  /* End of While*/
 
       
 RETVALUE(ret);
}


#endif

/**********************************************************************
 
         End of file:     gk_cfg.c@@/main/3 - Sat Jul 30 02:21:24 2011
 
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
/main/1      ---       sd   1. Initial Release.
/main/2      ---       sd   1. LTE MAC 2.1 release
           rg001.201   sm          1. Removed Transport channel 
                                   types from CRG
           rg003.201   ns   1. Trace added for functions.
           rg004.201   ns   1. ccpu00105882: Releasing HARQ processes of old
                               UE when ue reconfig with new crnti 
           rg007.201   ap  1. Added support for MIMO
           rg008.201   sd  1. Removed dependncy on MIMO compile-time flag
                           2. Added support for SPS.
/main/3      ---         gvj 1. Updated for LTE MAC Rel 3.1
           rg001.301   nudupi  1.Changes for the CR ccpu00119113. 
           rg002.301  asehgal 1. ccpu00120621:validations for qci introduced.
$SID$        ---        rt    1. LTE MAC 4.1 release
*********************************************************************91*/
