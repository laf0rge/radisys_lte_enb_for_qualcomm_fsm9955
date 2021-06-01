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

     Name:     EnodeB Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_emm_cell.c

     Sid:      fc_emm_cell_t2k.c@@/main/T2K_Merge_BR/Br_Tenb_GA2.0_Intg/2 - Wed Oct 16 14:44:40 2013

     Prg:      Sriky 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=4;

/* header includes */
#include "wr.h" 
#include "wr_cmn.h" 
#include "wr_smm_smallcell.h"
#include "wr_emm.h" 
#include "wr_ifm_dam.h"
#include "wr_emm_pbm.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_l1.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_rrm.h"
#include "wr_smm_init.h"

extern SmCb   smCb;

/* LTE_ADV starts */
PRIVATE S16  wrEmmBuildInitRgrLteAdvancedConfig(RgrLteAdvancedCellConfig *rgrLteAdvCfg, WrCellCb  *cellCb);
PRIVATE S16  wrEmmBuildRgrLteAdvancedConfig(RgrLteAdvancedCellConfig *rgrLteAdvCfg, LwrLteAdvancedConfigTrans  *lteAdvancedTrans);
/* LTE_ADV ends */

PUBLIC S16 wrFillDlRlcModeInfo( NhuDlRlcMode  *dlRlcMode, U8   rabId, U16 cellId);
PUBLIC S16 wrFillUlRlcModeInfo( NhuUlRlcMode  *ulRlcMode, U8   rabId, U16 cellId);
PUBLIC S16 wrFillCellRbCfg( NhuRbCfgList  **rbCfgList, U16 cellId);
/*PUBLIC S16 wrEmmCellInit( WrCellCb   *cellCb);*/
PUBLIC U8  wrFillBwCfg(U8  dlBwIn);
PUBLIC S16 wrFillRgrDlHqCfg( RgrDlHqCfg *cfg, U16 cellId);
PUBLIC S16 wrFillRgrRntiCfg( RgrRntiCfg *cfg, U16 cellId);
PUBLIC S16 wrFillRgrCfiCfg( RgrCfiCfg *cfg, U16 cellId);
PUBLIC S16 wrFillRgrUlTrgCqiCfg( RgrUlTrgCqiCfg *cfg, U16 cellId);
PUBLIC S16 wrFillRgrDlCmnCodeRateCfg( RgrDlCmnCodeRateCfg *cfg, U16 cellId);
PUBLIC S16 wrEmmGetBwInPrb (WrCellBw dlBw,WrCellBw *dlBwInPrbs);
PUBLIC S16 wrEmmGetNumPHICHGrps(WrPhichGrps phichGrps,U16 *numPHICHGrps);
PUBLIC S16 wrFillRgrPuschSubBandCfg( RgrPuschSubBandCfg *cfg,WrCellCb   *cellCb);
PUBLIC S16 wrFillRgrUlCmnCodeRateCfg( RgrUlCmnCodeRateCfg *cfg, U16 cellId);
PUBLIC S16 wrFillRgrDlfsCfg( RgrDlfsCfg *cfg, U16 cellId);
PUBLIC S16 wrFillRgrPucchCfg( RgrPucchCfg   *cfg,WrCellCb  *cellCb );
PUBLIC S16 wrFillRgrPhichCfg( RgrPhichCfg   *cfg,WrCellCb  *cellCb );
PUBLIC S16 wrFillCtfVndrCellCfg(WrCellCb *cellCb,CtfCfgReqInfo *cellCfgReq);
PUBLIC S16 wrEmmCellDedCrntiRange(U8 cellId,U16 *minVal,U16 *maxVal);


/** @brief This function fills Uplink Logical Channel Configuration.
 *
 * @details This function fills Uplink Logical Channel Configuration for a
 *          specific bearer.
 *
 *     Function: wrEmmCellFillUlLogChCfg
 *
 *         Processing steps:
 *         - This function populates the RRC NhuUlLogChMap structure. 
 *
 * @param[out] ulLogChMap: Uplink Logical Channel Mapping information
 * @param[in]       rbId: RAB ID. 
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmCellFillUlLogChCfg
(
NhuUlLogChMap                *ulLogChMap,
U8                           rbId
)
{

   TRC2(wrEmmCellFillUlLogChCfg);

   RLOG1(L_DEBUG, "Filling uplink logical channel configuration.RabId = %d",rbId);

   ulLogChMap->lcCfg.logChCfgpres = PRSNT_NODEF;
   ulLogChMap->lcCfgType = NHU_CFG_TYPE_EXPLICIT;

   RETVALUE(ROK);
} /* End of wrEmmCellFillUlLogChCfg */

/** @brief This function fills required configuration for common logical
 *         channels.
 *
 * @details This function fills required configuration for common logical
 *          channels.
 *
 *     Function: wrFillRgrCmnLcCfg
 *
 *         Processing steps:
 *         - This function populate RgrCellCfg structure.
 *
 * @param[out] cfg:  cell configuration parameters at MAC.
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PRIVATE S16 wrFillRgrCmnLcCfg
(
RgrCellCfg                   *cfg
)
{
   TRC2(wrFillRgrCmnLcCfg);

   cfg->numCmnLcs = WR_DFLT_NUM_CMN_LCS;

   if(cfg->numCmnLcs > RGR_MAX_CMN_LC_PER_CELL)
   {
      RLOG1(L_ERROR,"Invalid number of logical channels [%d]",cfg->numCmnLcs);
      RETVALUE(RFAILED);
   }

   {
      cfg->cmnLcCfg[0].lcId = WR_BCCH_SIB1_ID;
      cfg->cmnLcCfg[0].lcType = CM_LTE_LCH_BCCH;
      cfg->cmnLcCfg[0].dir = RGR_DIR_TX;
      cfg->cmnLcCfg[0].dlTrchType = CM_LTE_TRCH_DL_SCH;
      cfg->cmnLcCfg[0].ulTrchType = WR_VAL_ZERO;
   }
   {
      cfg->cmnLcCfg[1].lcId = WR_PCCH_ID;
      cfg->cmnLcCfg[1].lcType = CM_LTE_LCH_PCCH;
      cfg->cmnLcCfg[1].dir = RGR_DIR_TX;
      cfg->cmnLcCfg[1].dlTrchType = WR_VAL_ZERO;
      cfg->cmnLcCfg[1].ulTrchType = WR_VAL_ZERO;
   }
   {
      cfg->cmnLcCfg[2].lcId = WR_BCCH_BCH_ID;
      cfg->cmnLcCfg[2].lcType = CM_LTE_LCH_BCCH;
      cfg->cmnLcCfg[2].dir = RGR_DIR_TX;
      cfg->cmnLcCfg[2].dlTrchType = CM_LTE_TRCH_BCH;
      cfg->cmnLcCfg[2].ulTrchType = WR_VAL_ZERO;
   }

   {
      cfg->cmnLcCfg[3].lcId = WR_CCCH_UL_ID;
      cfg->cmnLcCfg[3].lcType = CM_LTE_LCH_CCCH;
      cfg->cmnLcCfg[3].dir = RGR_DIR_RX;
      cfg->cmnLcCfg[3].dlTrchType = CM_LTE_TRCH_DL_SCH;
      cfg->cmnLcCfg[3].ulTrchType = CM_LTE_TRCH_UL_SCH;
   }

   {
      cfg->cmnLcCfg[4].lcId = WR_CCCH_DL_ID;
      cfg->cmnLcCfg[4].lcType = CM_LTE_LCH_CCCH;
      cfg->cmnLcCfg[4].dir = RGR_DIR_TX;
      cfg->cmnLcCfg[4].dlTrchType = CM_LTE_TRCH_DL_SCH;
      cfg->cmnLcCfg[4].ulTrchType = CM_LTE_TRCH_UL_SCH;
   }

   {
      cfg->cmnLcCfg[5].lcId = WR_BCCH_SIB_OTHERS_ID;
      cfg->cmnLcCfg[5].lcType = CM_LTE_LCH_BCCH;
      cfg->cmnLcCfg[5].dir = RGR_DIR_TX;
      cfg->cmnLcCfg[5].dlTrchType = CM_LTE_TRCH_DL_SCH;
      cfg->cmnLcCfg[5].ulTrchType = WR_VAL_ZERO;
   }

   RETVALUE(ROK);

} /* end of wrFillRgrCmnLcCfg */


/** @brief This function is used to create the new cell control block and
 *         fill the parameters.    
 *
 * @details This function create a new cell control block and initialise the
 *          necessary values. 
 *
 *     Function: wrEmmCellAdd
 *
 *         Processing steps:
 *         - Allocate memory for Cell control block.
 *         - Assign the state of the cell control blcok to WR_INIT_STATE.
 *         - Fill the cell Id in cell control block.
 *
 * @param[in] cellId: Cell ID.
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrEmmCellAdd
(
WrCellId                     cellId
)
{
   WrCellCb                  *cellCb = NULLP;
   U32                       idx;
   
   /* allocate Cell Cb */
   for (idx = 0; idx < WR_MAX_CELLS; idx++)
   {
      if (wrEmmCb.cellCb[idx] == NULLP)
      {
         break;
      }
   }
   /*Allocate the memory for this cell */
   WR_ALLOC(&wrEmmCb.cellCb[idx], sizeof(WrCellCb));
   if(NULLP == wrEmmCb.cellCb[idx])
   {
      RLOG0(L_FATAL, "Failed to allocate memory for cell control block while adding cell.");
      RETVALUE(RFAILED);
   }

   /*Assign to the local pointer*/
   cellCb = wrEmmCb.cellCb[idx];

   /*Update the CellId in the cellCb*/
   cellCb->cellId = cellId;
   cellCb->cellState  = WR_CELL_STATE_INIT;

   RLOG_ARG0(L_DEBUG,DBG_CELLID,cellCb->cellId,
      "Cell state = INIT, Reason = Addition of New Cell");

#if 0   /*Initilizing the Cell CB for MIB and SIB*/
   if(ROK!= wrEmmCellInit(cellCb))
   {
      RLOG0(L_ERROR, "Initialisation of EMM Cell failed");
      RETVALUE(RFAILED);
   }
#endif
   if(ROK != wrEmmPbmCellCfg(cellCb))
   {
      RLOG0(L_ERROR,"EMM PBM cell configuration failed while adding cell");
      RETVALUE(RFAILED);
   }

   /*Reset Statistics*/
   cmMemset((U8 *) &cellCb->wrCellStats,0x00,sizeof(cellCb->wrCellStats));

   RETVALUE(ROK);
} /* end of wrEmmCellAdd */



/***********************************************************************
                    NHU related functions
************************************************************************/

/** @brief This function fills Downlink RLC Mode information.
 *
 * @details This function fills Downlink RLC Mode information.
 *
 *     Function: wrFillDlRlcModeInfo 
 *
 *         Processing steps:
 *         - This function fills the DL RLC Mode inforamtion based on RAB ID and
 *           RLC Mode type.
 *
 * @param[out] dlRlcMode: Fill the DL RLC Mode information based on RAB ID.
 * @param[in]      rabId: RAB ID.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillDlRlcModeInfo
(
NhuDlRlcMode  *dlRlcMode,
U8            rabId,
U16      cellId
)
{
   WrCellCb    *cellCb = NULLP;
   TRC2(wrFillDlRlcModeInfo);

   RLOG2(L_DEBUG, "Filling RLC DL information for rabId [%d] and cellId [%d]",rabId,cellId);

   WR_GET_CELLCB(cellCb, cellId); 
  if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Cell Id is invalid for filling RLC DL information");
      RETVALUE(RFAILED);
   }
   if(dlRlcMode->mode == NHU_RLC_MODE_AM)
   {
      if((rabId == WR_PDCP_ID_SRB1) || (rabId == WR_PDCP_ID_SRB2))
      {
         /* Using default values specified in 36.331 */
         dlRlcMode->m.amRlc.maxRetxThresh        = cellCb->rlcCfg.dlModeInfo.amRlcSrbmaxRetxThresh;
         dlRlcMode->m.amRlc.pollByte             = cellCb->rlcCfg.dlModeInfo.amRlcSrbmaxpollByte;
         dlRlcMode->m.amRlc.pollPdu              = cellCb->rlcCfg.dlModeInfo.amRlcSrbpollPdu;
         dlRlcMode->m.amRlc.tPollRetx            = cellCb->rlcCfg.dlModeInfo.amRlcSrbtPollRetx; 
      }
      else
      {
         dlRlcMode->m.amRlc.maxRetxThresh        = cellCb->rlcCfg.dlModeInfo.amRlcDrbmaxRetxThresh;
         dlRlcMode->m.amRlc.pollByte             = cellCb->rlcCfg.dlModeInfo.amRlcDrbmaxpollByte;
         dlRlcMode->m.amRlc.pollPdu              = cellCb->rlcCfg.dlModeInfo.amRlcDrbpollPdu;
         dlRlcMode->m.amRlc.tPollRetx            = cellCb->rlcCfg.dlModeInfo.amRlcDrbtPollRetx;
      }
   }
   else if(dlRlcMode->mode == NHU_RLC_MODE_UM)
   {
      dlRlcMode->m.umRlc.snFieldLen              = cellCb->rlcCfg.dlModeInfo.umRlcsnFieldLen;
   }

   RETVALUE(ROK);
} /* End of wrFillDlRlcModeInfo */

/** @brief This function fills Uplink RLC Mode information.
 *
 * @details This function fills Uplink RLC Mode information.
 *
 *     Function: wrFillUlRlcModeInfo
 *
 *         Processing steps:
 *         - This function fills the DL RLC Mode inforamtion based on RAB ID and
 *           RLC Mode type.
 *
 * @param[out] ulRlcMode: Fill the UL RLC Mode information based on RAB ID.
 * @param[in]      rabId: RAB ID.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillUlRlcModeInfo
(
NhuUlRlcMode  *ulRlcMode,
U8            rabId,
U16      cellId
)
{
   WrCellCb  *cellCb = NULLP;
   TRC2(wrFillUlRlcModeInfo);

   RLOG2(L_DEBUG, "Fill uplink RLC mode info.CellId = %d,RabId = %d",rabId,cellId);

   WR_GET_CELLCB(cellCb, cellId); 
  if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid cellId received while filling Ul RLC mode info");
      RETVALUE(RFAILED);
   }
   if(ulRlcMode->mode == NHU_RLC_MODE_AM)
   {
      if((rabId == WR_PDCP_ID_SRB1) || (rabId == WR_PDCP_ID_SRB2))
      {
         /* Using default values specified in 36.331 */
         ulRlcMode->m.amRlc.tStatusProh = cellCb->rlcCfg.ulModeInfo.amRlcSrbtStatusProh;
         ulRlcMode->m.amRlc.tReordring =  cellCb->rlcCfg.ulModeInfo.srbtReordring;
      }
      else
      {
         ulRlcMode->m.amRlc.tStatusProh = cellCb->rlcCfg.ulModeInfo.amRlcDrbtStatusProh;
         ulRlcMode->m.amRlc.tReordring = cellCb->rlcCfg.ulModeInfo.drbtReordring;
      }
   }
   else if (ulRlcMode->mode == NHU_RLC_MODE_UM)
   {
      ulRlcMode->m.umRlc.snFieldLen = cellCb->rlcCfg.ulModeInfo.umRlcsnFieldLen;
      ulRlcMode->m.umRlc.tReordring = cellCb->rlcCfg.ulModeInfo.drbtReordring;
   }

   RETVALUE(ROK);
} /* End of wrFillUlRlcModeInfo */

/** @brief This function fills the Radio bearer configuration information to be
 *         configured at lower layers. 
 *
 *     Function: wrFillCellRbCfg
 *
 *         Processing steps:
 *         - This function fills the st of configuration information for the
 *           radio bearers to be configured at the lower layers.
 *
 *
 * @param[out] rbCfgList: Fill the list of configuration information for the 
 *                        radio bearers to be configured at the lower layers.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillCellRbCfg
(
NhuRbCfgList  **rbCfgList,
U16        cellId
)
{
   NhuRbCfgList *lclRbCfgList = NULLP;
   U8 dlrlcCfgType;
   U8 ulrlcCfgType;

   TRC2(wrFillCellRbCfg);

   RLOG0(L_INFO,"Configuring cell RBs");

   lclRbCfgList = *rbCfgList;

   /* Filling RB for BCCH (SIB1) to DL-SCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[0].rbId.rbId =  WR_BCCH_SIB1_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[0].rbCfgBitMsk = NHU_RB_CFG_DL;
      /* TODO Hard coding the CQI to 1 as of now */
#ifdef LTE_L2_MEAS
      lclRbCfgList->rbCfgInfoList[0].qci = 1;
#endif

      /* DlRbCfg */
      {
         lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRbBitMsk =
            (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* dlLogChMapInfo */
         {
            /* Logical Channel ID and type */
            lclRbCfgList->rbCfgInfoList[0].lcId.lcId = 
               WR_BCCH_SIB1_ID;
            lclRbCfgList->rbCfgInfoList[0].lcId.logChType = 
               CM_LTE_LCH_BCCH;
            /* Transport Channel Type */
            lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlLogChMap.trChType = 
               CM_LTE_TRCH_DL_SCH;
         }
         /* DL-RLC mode */
         {
            dlrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRlcMode.rlcCfgType =
               dlrlcCfgType;
            lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRlcMode.mode = \
                                                                    NHU_RLC_MODE_TM;
            /* SET dlRlcMode.mode HERE */
            wrFillDlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[0].dlRbCfg.dlRlcMode), WR_BCCH_SIB1_ID, cellId);
         }
      }
   }

   /* Filling RB for PCCH to PCH  Mapping */
   {
      lclRbCfgList->rbCfgInfoList[1].rbId.rbId = WR_PCCH_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[1].rbCfgBitMsk = NHU_RB_CFG_DL;
      /* TODO Hard coding the CQI to 1 as of now */
#ifdef LTE_L2_MEAS
      lclRbCfgList->rbCfgInfoList[1].qci = 1;
#endif

      /* DlRbCfg */
      {
         lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRbBitMsk 
            = (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* dlLogChMapInfo */
         {
            /* Logical Channel ID and type */
            lclRbCfgList->rbCfgInfoList[1].lcId.lcId = 
               WR_PCCH_ID; 
            lclRbCfgList->rbCfgInfoList[1].lcId.logChType = 
               CM_LTE_LCH_PCCH;
            /* Transport Channel Type */
            lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlLogChMap.trChType =
               CM_LTE_TRCH_PCH;
         }
         /* DL-RLC mode */
         {
            dlrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRlcMode.rlcCfgType =
               NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRlcMode.mode =\
                                                                   NHU_RLC_MODE_TM;
            wrFillDlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRlcMode), WR_PCCH_ID, cellId);
         }
      }
   }

   /* Filling RB for BCCH to BCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[2].rbId.rbId =  WR_BCCH_BCH_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[2].rbCfgBitMsk = NHU_RB_CFG_DL;
      /* TODO Hard coding the CQI to 1 as of now */
#ifdef LTE_L2_MEAS
      lclRbCfgList->rbCfgInfoList[2].qci = 1;
#endif

      /* DlRbCfg */
      {
         lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRbBitMsk = 
            (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* dlLogChMapInfo */
         {
            /* Logical Channel ID and type */
            lclRbCfgList->rbCfgInfoList[2].lcId.lcId = 
               WR_BCCH_BCH_ID; 
            lclRbCfgList->rbCfgInfoList[2].lcId.logChType = 
               CM_LTE_LCH_BCCH;
            /* Transport Channel Type */
            lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlLogChMap.trChType =
               CM_LTE_TRCH_BCH;
         }
         /* DL-RLC mode */
         {
            dlrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRlcMode.rlcCfgType = 
               dlrlcCfgType;
            lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRlcMode.mode \
               = NHU_RLC_MODE_TM;
            wrFillDlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[2].dlRbCfg.dlRlcMode), WR_BCCH_BCH_ID, cellId);
         }
      }
   }

   /* Filling RB for CCCH to UL-SCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[3].rbId.rbId =  WR_CCCH_UL_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[3].rbCfgBitMsk = NHU_RB_CFG_UL;
      /* TODO Hard coding the CQI to 1 as of now */
#ifdef LTE_L2_MEAS
      lclRbCfgList->rbCfgInfoList[3].qci = 1;
#endif

      /* UlRbCfg */
      {
         /* Bit mask calculate properly */
         lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRbBitMsk = 
            (NHU_RB_CFG_LOGCH_MAP_UL | NHU_RB_CFG_RLC_MODE_UL);

         /* ulLogChMapInfo */
         {
            /* Transport Channel type */
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulLogChMap.trChType\
               = CM_LTE_TRCH_UL_SCH;

            /* Logical Channel Id and Type */
            lclRbCfgList->rbCfgInfoList[3].lcId.lcId =
               WR_CCCH_UL_ID; 
            lclRbCfgList->rbCfgInfoList[3].lcId.logChType = 
               CM_LTE_LCH_CCCH;

            /* Fill Logical Channel Cfg */
            wrEmmCellFillUlLogChCfg(
                  &(lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulLogChMap), 
                                                        WR_CCCH_UL_ID);
         }
         /* ulRlcMode */
         {
            ulrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRlcMode.rlcCfgType = 
               ulrlcCfgType;
            lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRlcMode.mode \
               = NHU_RLC_MODE_TM;
            wrFillUlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[3].ulRbCfg.ulRlcMode), WR_CCCH_UL_ID, cellId);
         } /* ulRlcMode */
      }
   }

   /* Filling RB for CCCH to DL-SCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[4].rbId.rbId =  WR_CCCH_DL_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[4].rbCfgBitMsk = NHU_RB_CFG_DL;
      /* TODO Hard coding the CQI to 1 as of now */
#ifdef LTE_L2_MEAS
      lclRbCfgList->rbCfgInfoList[4].qci = 1;
#endif

      /* DlRbCfg */
      {
         lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRbBitMsk |= 
            (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* dlLogChMapInfo */
         {
            /* Logical Channel ID and type */
            lclRbCfgList->rbCfgInfoList[4].lcId.lcId = 
               WR_CCCH_DL_ID; 
            lclRbCfgList->rbCfgInfoList[4].lcId.logChType = 
               CM_LTE_LCH_CCCH;
            /* Transport Channel Type */
            lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlLogChMap.trChType = 
               CM_LTE_TRCH_DL_SCH;
         }
         /* DL-RLC mode */
         {
            dlrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRlcMode.rlcCfgType = 
               dlrlcCfgType;
            lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRlcMode.mode\
               = NHU_RLC_MODE_TM;
            /* set lclRbCfgList->rbCfgInfoList[1].dlRbCfg.dlRlcMode.mode */
            wrFillDlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[4].dlRbCfg.dlRlcMode), WR_CCCH_DL_ID, cellId);
         }
      }
   }

   /* Filling RB for BCCH (non-SIB1) to DL-SCH Mapping */
   {
      lclRbCfgList->rbCfgInfoList[5].rbId.rbId =  WR_BCCH_SIB_OTHERS_ID;
      /* Bit Mask (UL, DL Cfg present) */ 
      lclRbCfgList->rbCfgInfoList[5].rbCfgBitMsk = NHU_RB_CFG_DL;
      /* TODO Hard coding the CQI to 1 as of now */
#ifdef LTE_L2_MEAS
      lclRbCfgList->rbCfgInfoList[5].qci = 1;
#endif

      /* DlRbCfg */
      {
         lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRbBitMsk =
            (NHU_RB_CFG_LOGCH_MAP_DL | NHU_RB_CFG_RLC_MODE_DL);

         /* dlLogChMapInfo */
         {
            /* Logical Channel ID and type */
            lclRbCfgList->rbCfgInfoList[5].lcId.lcId = 
               WR_BCCH_SIB_OTHERS_ID; 
            lclRbCfgList->rbCfgInfoList[5].lcId.logChType = 
               CM_LTE_LCH_BCCH;
            /* Transport Channel Type */
            lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlLogChMap.trChType = 
               CM_LTE_TRCH_DL_SCH;
         }
         /* DL-RLC mode */
         {
            dlrlcCfgType = NHU_CFG_TYPE_EXPLICIT;
            lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRlcMode.rlcCfgType =
               dlrlcCfgType;
            lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRlcMode.mode = \
                                                                    NHU_RLC_MODE_TM;
            /* set lclRbCfgList->rbCfgInfoList[3].dlRbCfg.dlRlcMode.mode HERE */
            wrFillDlRlcModeInfo(
                  &(lclRbCfgList->rbCfgInfoList[5].dlRbCfg.dlRlcMode), (WR_BCCH_SIB_OTHERS_ID), cellId);
         }
      }
   }

   RETVALUE(ROK);
} /* End of wrFillCellRbCfg */

/** @brief This function fills the Band Width parameter.
 *
 * @details This function fills the Band Width parameter. 
 *
 *     Function: wrFillBwCfg
 *
 *         Processing steps:
 *         - This function derives the Down link Band width paramter.
 *
 * @param[in]   bwIn:  Bandwidth input value.
 * @return U8 
 *        -# Success : a valid  BW value.
 */
PUBLIC U8 wrFillBwCfg
(
U8                bwIn
)
{
   
   U8                bwOut;

   TRC2(wrFillBwCfg);
   /*Converting ASN datatypes to RGR values*/
   switch(bwIn)
   {
      case LWR_BANDWIDTH_6:
         bwOut = 6;
         break;
      case LWR_BANDWIDTH_15:
         bwOut = 15;
         break;
      case LWR_BANDWIDTH_25:
         bwOut = 25;
         break;
      case LWR_BANDWIDTH_50:
         bwOut = 50;
         break;
      case LWR_BANDWIDTH_75:
         bwOut = 75;
         break;
      case LWR_BANDWIDTH_100:
         bwOut = 100;
         break;
      default:
         /* Default value is currenlty considered as least value */
         RLOG1(L_DEBUG,"Invalid bandwidth [%d] received while filling bandwidth configuration", bwIn);
         bwOut = 6;
         break;
   }   
   RETVALUE(bwOut);
} /* end of wrFillBwCfg */
/** @brief This function fills and send RRC cell configuation Request 
 *         to the RRC IFM. 
 *
 * @details
 *
 *     Function: wrFillNhuCellCfgReq
 *
 *         Processing steps:
 *         - This function allocates memory for RRC cell configuration Req.
 *         - Fills the RRC cell confguration Req with necesary values.
 *         - Invokes the RRC IFM Module function wrSndNhuCellCfgReq to send 
 *           to RRC layer.
 *
 * @param[in] cellCb: cell control block. 
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrFillNhuCellCfgReq
(
WrCellCb          *cellCb
)
{
   NhuCellCfgReqSdus         *nhuCellCfgReqSdus = NULLP;
   NhuRbCfgList              *rbCfgList = NULLP;
   NhuRachCfg                *rachCfg = NULLP;
   WrMibParams               *mib  = &(cellCb->mib);
   WrSib2Params              *sib2 = &(cellCb->sib2);

   TRC2(wrFillNhuCellCfgReq);

   /* Alloc event structure */
   WR_ALLOCEVNT(&nhuCellCfgReqSdus, sizeof(NhuCellCfgReqSdus));
   if(NULLP == nhuCellCfgReqSdus)
   {
      RLOG0(L_FATAL,"Memory allocation failed while sending cell configuration to RRC.");
      RETVALUE(RFAILED);
   }
   /* Filling Header */
   nhuCellCfgReqSdus->hdr.cellId       = cellCb->cellId;
   nhuCellCfgReqSdus->hdr.ueId         = WR_VAL_ZERO;
   nhuCellCfgReqSdus->hdr.transId      = WR_VAL_ONE;

   nhuCellCfgReqSdus->sdu.cfgType      = NHU_CFGTYPE_SETUP;

   nhuCellCfgReqSdus->sdu.u.cfgInfo.duplexMode = 
           (cellCb->duplexMode == NHU_MODE_FDD) ?
           NHU_MODE_FDD : NHU_MODE_TDD; 

   nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.ulSysBw =  wrFillBwCfg(sib2->ulBw);
   nhuCellCfgReqSdus->sdu.u.cfgInfo.sysBwCfg.dlSysBw =  wrFillBwCfg(mib->dlBw);

   /* cellCfgBitMask - to indicate presence of Cell Config Info */
   nhuCellCfgReqSdus->sdu.u.cfgInfo.cellCfgBitMsk = (NHU_CELL_CFG_RB |\
         NHU_CELL_CFG_MAC_CMN);

   /* 
    * Fill RB Configuration 
    */
   rbCfgList = &(nhuCellCfgReqSdus->sdu.u.cfgInfo.rbToAddCfgList);
   /* 5 cellRbs are configured by default */
   rbCfgList->numOfRbCfgInfo = WR_CELL_CFG_DFLT_RBS;    
   WR_GET_MEM(nhuCellCfgReqSdus, sizeof(NhuRbCfgInfo)*rbCfgList->numOfRbCfgInfo,
         &(rbCfgList->rbCfgInfoList));
   /* filling RB Cfg. maapping */
   wrFillCellRbCfg(&rbCfgList, cellCb->cellId);

   /* 
    * MAC common configuration 
    * Fill RACH Configuration 
    */
   rachCfg = &(nhuCellCfgReqSdus->sdu.u.cfgInfo.macCmnCfg.rachCfg);
   rachCfg->maxMsg3Tx = (U8)sib2->rachCfg.maxMsg3HarqTrans;

  if(ROK != wrSndNhuCellCfgReq(&nhuCellCfgReqSdus))
   {
      RLOG0(L_ERROR,"Failure received while sending cell configuration request"
             "to RRC Layer");      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /*end of  wrFillNhuCellCfgReq */

/** @brief This function fills and send RRC cell delete configuation Request 
 *         to the RRC IFM. 
 *
 * @details
 *
 *     Function: wrFillNhuCellDelCfgReq
 *
 *         Processing steps:
 *         - This function allocates memory for RRC cell Delete configuration Req.
 *         - Fills the RRC cell delete confguration Req with necesary values.
 *         - Invokes the RRC IFM Module function wrSndNhuCellCfgReq to send 
 *           to RRC layer.
 *
 * @param[in] cellCb: cell control block. 
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrFillNhuCellDelCfgReq
(
U16                          cellId
)
{
   NhuCellCfgReqSdus         *nhuCellDelCfgReqSdus = NULLP;
   WrCellCb *cellCb = NULL;

   TRC2(wrFillNhuCellDelCfgReq);

   WR_GET_CELLCB(cellCb, cellId);
  if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid CellId received while"
                "sending cell delete configuration request to RRC");
      RETVALUE(RFAILED);
   }

   RLOG_ARG0(L_INFO,DBG_CELLID,cellCb->cellId, "Filling of cell configuration request over "
      "the NHU Interface ");

   /* Alloc event structure */
   WR_ALLOCEVNT(&nhuCellDelCfgReqSdus, sizeof(NhuCellCfgReqSdus));
   if(nhuCellDelCfgReqSdus == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed while sending delete cell request to RRC.");
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Filling Header */
   nhuCellDelCfgReqSdus->hdr.cellId       = cellCb->cellId;
   nhuCellDelCfgReqSdus->hdr.ueId         = WR_VAL_ZERO;
   nhuCellDelCfgReqSdus->hdr.transId      = WR_VAL_TWO_CELL_RELEASE;

   nhuCellDelCfgReqSdus->sdu.cfgType      = NHU_CFGTYPE_RELEASE;
   nhuCellDelCfgReqSdus->sdu.u.rlsInfo.cellId = cellCb->cellId; 
   if(ROK != wrSndNhuCellCfgReq(&nhuCellDelCfgReqSdus))
   {
      RLOG0(L_ERROR,"Failed to send delete cell request to RRC");      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}
/***********************************************************************
                    CTF related functions
************************************************************************/
/** @brief This function fills and send  the cell configuration for PHY layer. 
 *
 * @details This function prepares and send  the cell configuration for PHY layer.
 *
 *     Function: wrFillCtfCellCfgReq
 *
 *         Processing steps:
 *         - Fill the PHY related configuration parameters.
 *         - Invoke the function wrIfmPhyCellCfg to send the filled confguration
 *           to PHY layer.
 *
 * @param[in] cellId: Cell ID.  
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrFillCtfCellCfgReq
(
U16  cellId
)
{
   CtfCfgReqInfo    *cellCfgReq = NULLP;
   WrCellCb          *cellCb = NULLP;

   TRC2(wrFillCtfCellCfgReq);

   WR_GET_CELLCB(cellCb,cellId);
   if(cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid cellId received while filling"
                "phy related configuration parameters");
      RETVALUE(RFAILED);
   } /* end of if statement */

   WR_ALLOC(&cellCfgReq, sizeof(CtfCfgReqInfo));

   if(cellCfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed while filling phy cell configuration");
      RETVALUE(RFAILED);
   } /* end of if statement */

   cellCfgReq->cfgType = CTF_CONFIG;
   cellCfgReq->u.cfg.cfgElem = CTF_CELL_CFG;
   cellCfgReq->u.cfg.u.cellCfg.cellId = (U8)cellId; 
   cellCfgReq->u.cfg.u.cellCfg.cellIdGrpId = (cellCb->physCellId / 3);

   /*cellCfgReq->u.cfg.u.cellCfg.physCellIdPres = PRSNT_NODEF;*/
   cellCfgReq->u.cfg.u.cellCfg.physCellId = (cellCb->physCellId % 3);
   /* Bandwidth Config */
   cellCfgReq->u.cfg.u.cellCfg.bwCfg.pres = PRSNT_NODEF; 

   cellCfgReq->u.cfg.u.cellCfg.bwCfg.dlBw = cellCb->mib.dlBw;
   cellCfgReq->u.cfg.u.cellCfg.bwCfg.ulBw = cellCb->sib2.ulBw;
   cellCfgReq->u.cfg.u.cellCfg.bwCfg.eUtraBand \
      = cellCb->sib1.freqBandIndicator;

   /* Basic transmission scheme */
   cellCfgReq->u.cfg.u.cellCfg.txCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.txCfg.duplexMode = 
      (cellCb->duplexMode == CTF_DUPMODE_FDD) ? 
      CTF_DUPMODE_FDD : CTF_DUPMODE_TDD;
   cellCfgReq->u.cfg.u.cellCfg.txCfg.scSpacing = cellCb->ctfCfg.dfltScSpacing;
   cellCfgReq->u.cfg.u.cellCfg.txCfg.cycPfx = cellCb->dlCyclicprefix;
   /* Antenna config */
   if ( wrEmmCb.numTxAntenna == 1)
   {
      cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt =  CTF_AP_CNT_1;
   }
   else if ( wrEmmCb.numTxAntenna == 2)
   {
      cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt =  CTF_AP_CNT_2;
   }
   else
   {
      cellCfgReq->u.cfg.u.cellCfg.antennaCfg.antPortsCnt =  CTF_AP_CNT_4;
   }
   
   /* PRACH config */
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.rootSequenceIndex \
      = cellCb->sib2.prachCfg.rootSeqIdx;

   cellCfgReq->u.cfg.u.cellCfg.prachCfg.prachCfgIndex = 
         cellCb->sib2.prachCfg.prachCfgIdx;
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.zeroCorrelationZoneCfg \
      = cellCb->sib2.prachCfg.zeroCorrZone; 
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.highSpeedFlag = 
      cellCb->sib2.prachCfg.highSpeedFlag;
   cellCfgReq->u.cfg.u.cellCfg.prachCfg.prachFreqOffset \
      = cellCb->sib2.prachCfg.prachFreqOffset;
   cellCfgReq->u.cfg.u.cellCfg.pdschCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.pdschCfg.refSigPwr = 
      ((-(cellCb->sib2.pdschCfg.refSigPower)) * 4);

   cellCfgReq->u.cfg.u.cellCfg.pdschCfg.p_b = 
      cellCb->sib2.pdschCfg.pB;
   /* PUSCH config */
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.pres = PRSNT_NODEF;

   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.noOfsubBands \
      = cellCb->sib2.puschCfg.nSB;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.hoppingMode \
      = cellCb->sib2.puschCfg.hoppingMode;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.hoppingOffset\
      = cellCb->sib2.puschCfg.hoppingOffset;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschBasicCfg.enable64QAM\
      = cellCb->sib2.puschCfg.qam64Enabled;

   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.grpHopEnabled\
      = cellCb->sib2.puschCfg.grpHoppingEnabled;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.seqHopEnabled\
      = cellCb->sib2.puschCfg.seqHoppingEnabled;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.grpNum\
      = cellCb->sib2.puschCfg.grpAssignPusch;
   cellCfgReq->u.cfg.u.cellCfg.puschCfg.puschUlRS.cycShift\
      = cellCb->sib2.puschCfg.cyclicShift;
   /* PHICH config */
   cellCfgReq->u.cfg.u.cellCfg.phichCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.phichCfg.duration = cellCb->mib.phichDur;
   cellCfgReq->u.cfg.u.cellCfg.phichCfg.resource = cellCb->mib.phichGrps;
   /* PUCCH config */
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.pres = PRSNT_NODEF;
   /* wr004.102 : klock warning fixed */
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.nRB =   cellCb->sib2.pucchCfg.nRBCqi;
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.nCS = cellCb->sib2.pucchCfg.nCSAN;
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.n1PUCCH = cellCb->sib2.pucchCfg.n1PucchAn;
   cellCfgReq->u.cfg.u.cellCfg.pucchCfg.deltaShift = cellCb->sib2.pucchCfg.deltaPucchShift;
   /* SRS UL config, setup case */
   if(TRUE == wrSmEnableSrs)
   {
      cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.pres =  PRSNT_NODEF;
   }
   else
   {
      cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.pres =  NOTPRSNT;
   }
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.srsCfgType = SOUNDINGRS_UL_CONFIGCMN_SETUP;

   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.srsSetup.srsBw         = cellCb->sib2.srsCfg.srsBandwidthCfg; 
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.srsSetup.sfCfg         = cellCb->sib2.srsCfg.srsSubfrmCfg;
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.srsSetup.srsANSimultTx = cellCb->sib2.srsCfg.ackNackSrsSimulTx;
   cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.srsSetup.srsMaxUpPts   = NOTPRSNT; 
/* ACC-TDD */
#ifdef LTE_TDD
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.sfAssignment = cellCb->sib1.tddCfg.subframeCfg;
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.spclSfPatterns = cellCb->sib1.tddCfg.specialSubframeCfg;
#else
   /* TDD Configuration is not present*/
   cellCfgReq->u.cfg.u.cellCfg.tddSfCfg.pres = NOTPRSNT;
#endif
   /* Unused parameters by MSPD PHY*/
   cellCfgReq->u.cfg.u.cellCfg.syncSigPowOsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.syncSigPowOs = WR_SIG_POWOS;

   cellCfgReq->u.cfg.u.cellCfg.cfiPowOsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.cfiPowOs = WR_CFI_POWOS;
   
   cellCfgReq->u.cfg.u.cellCfg.dciPowOsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.dciPowOs = WR_DCI_POWOS;

   cellCfgReq->u.cfg.u.cellCfg.extWinMarginPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.extWinMargin = WR_EXT_WIN_MARGIN;

   cellCfgReq->u.cfg.u.cellCfg.pucchNoiseGammaPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.pucchNoiseGamma = WR_PUCCH_NOISE_GAMA;

   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio4Pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio4 = WR_PRACH_RATIO_4;

   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio0Pres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.prachPkRatio0 = WR_PRACH_RATIO_0;

   cellCfgReq->u.cfg.u.cellCfg.srsDopEstFactorPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.srsDopEstFactor = WR_SRS_DOP_EST_FACTOR;

   cellCfgReq->u.cfg.u.cellCfg.puschProbDtxAckPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.puschProbDtxAck = WR_PUSCH_DTX_ACK;

   cellCfgReq->u.cfg.u.cellCfg.pucchProbDtxAckPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.pucchProbDtxAck = WR_PUCCH_DTX_ACK;

   cellCfgReq->u.cfg.u.cellCfg.txAntennaPortsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.txAntennaPorts = WR_TX_ANTENA_PORTS;


   cellCfgReq->u.cfg.u.cellCfg.rxAntennaPortsPres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.rxAntennaPorts = WR_RX_ANTENA_PORTS;

   cellCfgReq->u.cfg.u.cellCfg.phySyncModePres= PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.phySyncMode = WR_PHY_SYNC_MODE;

   cellCfgReq->u.cfg.u.cellCfg.dataRepModePres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.dataRepMode = WR_DATA_REP_MODE;

   cellCfgReq->u.cfg.u.cellCfg.rachSrRepModePres = PRSNT_NODEF;
   cellCfgReq->u.cfg.u.cellCfg.rachSrRepMode = WR_RACH_SR_REP_MODE ;

   cellCfgReq->u.cfg.u.cellCfg.dlEarfcn = cellCb->dlCarrierFreq; 
   cellCfgReq->u.cfg.u.cellCfg.ulEarfcn = cellCb->sib2.ulCarrierFreq;
   /* Added this parameter to rgu dynamic delta*/
   cellCfgReq->u.cfg.u.cellCfg.numDlUePerTti = cellCb->schdCfg.genCfg.maxDlUeNewTxPerTti;

   cellCfgReq->u.cfg.u.cellCfg.startRntiVal = cellCb->rgrCfg.dfltStartRnti;
   cellCfgReq->u.cfg.u.cellCfg.maxNumUe = cellCb->rgrCfg.dfltRntiSize + cellCb->wrDfltRrmRntisize;

   printf("Before Vendor Params\n");
/*
TODO_T2K:Once PHY supports Test Mode operations
         this needs to be removed
*/
   /* Fill vendor specific configurations */
   #ifndef TENB_ACC
   if(ROK != wrFillCtfVndrCellCfg(cellCb, cellCfgReq))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"VendorSpecific configuration failed");   
      WR_FREE(cellCfgReq, sizeof(CtfCfgReqInfo));
      RETVALUE(RFAILED);
   }
   #endif
   if(ROK != wrIfmPhyCellCfg(cellCfgReq,cellId))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failure received while sending PHY cell configuration.");      
      RETVALUE(RFAILED);
   }


   RETVALUE(ROK);
} /* end of wrSndCtfCellCfgReq */

/***********************************************************************
                    CTF related functions
************************************************************************/
/** @brief This function fills and send  the cell re-configuration for PHY layer. 
 *
 * @details This function prepares and send  the cell re-configuration for PHY layer.
 *
 *     Function: wrFillCtfCellReCfgReq
 *
 *         Processing steps:
 *         - Fill the PHY related re-configuration parameters.
 *         - Invoke the function wrIfmPhyCellCfg to send the filled re-confguration
 *           to PHY layer.
 *
 * @param[in] cellId: Cell ID. 
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrFillCtfCellReCfgReq
(
U32  cellId
)
{
   CtfCfgReqInfo      *cellCfgReq = NULLP;
   WrCellCb           *cellCb     = NULLP;
   CtfCellRecfgInfo   *cellReCfg  = NULLP;
   TRC2(wrFillCtfCellCfgReq);

   RLOG0(L_INFO, "Trigger cell reconfiguration request over CTF interface");

   WR_GET_CELLCB(cellCb,cellId);
   if(cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid cellId received while filling"
                "phy reconfig parameters");
      RETVALUE(RFAILED);
   } /* end of if statement */

   WR_ALLOC(&cellCfgReq, sizeof(CtfCfgReqInfo));

   if(cellCfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed while filling phy reconfig parameters.");
      RETVALUE(RFAILED);
   } /* end of if statement */

  
   cellCfgReq->cfgType = CTF_RECONFIG;
   cellCfgReq->u.reCfg.cfgElem = CTF_CELL_CFG;
   cellReCfg = &(cellCfgReq->u.reCfg.u.cellRecfg); 

   cellReCfg->ctfReCfgType = CTF_MAJOR_RECFG; 
   cellReCfg->cellId = (U16)cellId; 
   cellReCfg->cellIdGrpId = (cellCb->physCellId / 3);
   cellReCfg->physCellIdPres = PRSNT_NODEF ;
   cellReCfg->physCellId = (cellCb->physCellId % 3);
   /* Bandwidth Config */
   cellReCfg->bwCfg.pres = PRSNT_NODEF ; 

   cellReCfg->bwCfg.dlBw = cellCb->mib.dlBw;
   cellReCfg->bwCfg.ulBw = cellCb->sib2.ulBw;
   cellReCfg->bwCfg.eUtraBand = cellCb->sib1.freqBandIndicator;

   /* Basic transmission scheme */
   cellReCfg->txCfg.pres = PRSNT_NODEF ;
   cellReCfg->txCfg.duplexMode = 
      (cellCb->duplexMode == CTF_DUPMODE_FDD) ? 
      CTF_DUPMODE_FDD : CTF_DUPMODE_TDD;
   cellReCfg->txCfg.scSpacing = cellCb->ctfCfg.dfltScSpacing;
   cellReCfg->txCfg.cycPfx = cellCb->dlCyclicprefix;
   /* Antenna config */
   if ( wrEmmCb.numTxAntenna == 1)
   {
      cellReCfg->antennaCfg.antPortsCnt =  CTF_AP_CNT_1;
   }
   else if ( wrEmmCb.numTxAntenna == 2)
   {
      cellReCfg->antennaCfg.antPortsCnt =  CTF_AP_CNT_2;
   }
   else
   {
      cellReCfg->antennaCfg.antPortsCnt =  CTF_AP_CNT_4;
   }

   /* PRACH config */
   cellReCfg->prachCfg.pres = PRSNT_NODEF;
   cellReCfg->prachCfg.rootSequenceIndex =
         cellCb->sib2.prachCfg.rootSeqIdx;

   cellReCfg->prachCfg.prachCfgIndex = 
         cellCb->sib2.prachCfg.prachCfgIdx;
   cellReCfg->prachCfg.zeroCorrelationZoneCfg  = 
         cellCb->sib2.prachCfg.zeroCorrZone; 
   cellReCfg->prachCfg.highSpeedFlag = 
         cellCb->sib2.prachCfg.highSpeedFlag;
   cellReCfg->prachCfg.prachFreqOffset = 
         cellCb->sib2.prachCfg.prachFreqOffset;
   
   cellReCfg->pdschCfg.pres = PRSNT_NODEF;
   cellReCfg->pdschCfg.refSigPwr = cellCb->sib2.pdschCfg.refSigPower;
   cellReCfg->pdschCfg.p_b = cellCb->sib2.pdschCfg.pB;
   /* PUSCH config */
   cellReCfg->puschCfg.pres = PRSNT_NODEF;

   cellReCfg->puschCfg.puschBasicCfg.noOfsubBands =
        cellCb->sib2.puschCfg.nSB;
   cellReCfg->puschCfg.puschBasicCfg.hoppingMode  =
        cellCb->sib2.puschCfg.hoppingMode;
   cellReCfg->puschCfg.puschBasicCfg.hoppingOffset =
        cellCb->sib2.puschCfg.hoppingOffset;
   cellReCfg->puschCfg.puschBasicCfg.enable64QAM   =
        cellCb->sib2.puschCfg.qam64Enabled;

   cellReCfg->puschCfg.puschUlRS.grpHopEnabled     =
        cellCb->sib2.puschCfg.grpHoppingEnabled;
   cellReCfg->puschCfg.puschUlRS.seqHopEnabled     =
        cellCb->sib2.puschCfg.seqHoppingEnabled;
   cellReCfg->puschCfg.puschUlRS.grpNum            =
        cellCb->sib2.puschCfg.grpAssignPusch;
   cellReCfg->puschCfg.puschUlRS.cycShift          =
        cellCb->sib2.puschCfg.cyclicShift;
   /* PHICH config */
   cellReCfg->phichCfg.pres = PRSNT_NODEF;
   cellReCfg->phichCfg.duration = cellCb->mib.phichDur;
   cellReCfg->phichCfg.resource = cellCb->mib.phichGrps;
   /* PUCCH config */
   cellReCfg->pucchCfg.pres = PRSNT_NODEF;
   /* wr004.102 : klock warning fixed */
   cellReCfg->pucchCfg.nRB =   cellCb->sib2.pucchCfg.nRBCqi;
   cellReCfg->pucchCfg.nCS = cellCb->sib2.pucchCfg.nCSAN;
   cellReCfg->pucchCfg.n1PUCCH = cellCb->sib2.pucchCfg.n1PucchAn;
   cellReCfg->pucchCfg.deltaShift = cellCb->sib2.pucchCfg.deltaPucchShift;
   /* SRS UL config, setup case */
   if(TRUE == wrSmEnableSrs)
   {
      cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.pres =  PRSNT_NODEF;
   }
   else
   {
      cellCfgReq->u.cfg.u.cellCfg.srsUlCfg.pres =  NOTPRSNT;
   }
   cellReCfg->srsUlCfg.srsCfgType = SOUNDINGRS_UL_CONFIGCMN_SETUP;

   cellReCfg->srsUlCfg.srsSetup.srsBw         = cellCb->sib2.srsCfg.srsBandwidthCfg; 
   cellReCfg->srsUlCfg.srsSetup.sfCfg         = cellCb->sib2.srsCfg.srsSubfrmCfg;
   cellReCfg->srsUlCfg.srsSetup.srsANSimultTx = cellCb->sib2.srsCfg.ackNackSrsSimulTx;
   cellReCfg->srsUlCfg.srsSetup.srsMaxUpPts   = NOTPRSNT; 
/* ACC-TDD */
#ifdef LTE_TDD
   cellReCfg->tddSfCfg.pres = PRSNT_NODEF;
   cellReCfg->tddSfCfg.sfAssignment = cellCb->sib1.tddCfg.subframeCfg;
   cellReCfg->tddSfCfg.spclSfPatterns = cellCb->sib1.tddCfg.specialSubframeCfg;
#else
   /* TDD Configuration is not present*/
   cellReCfg->tddSfCfg.pres = NOTPRSNT;
#endif
   /* Unused parameters by MSPD PHY*/
   cellReCfg->syncSigPowOsPres = PRSNT_NODEF;
   cellReCfg->syncSigPowOs = WR_SIG_POWOS;

   cellReCfg->cfiPowOsPres = PRSNT_NODEF;
   cellReCfg->cfiPowOs = WR_CFI_POWOS;
   
   cellReCfg->dciPowOsPres = PRSNT_NODEF;
   cellReCfg->dciPowOs = WR_DCI_POWOS;

   cellReCfg->extWinMarginPres = PRSNT_NODEF;
   cellReCfg->extWinMargin = WR_EXT_WIN_MARGIN;

   cellReCfg->pucchNoiseGammaPres = PRSNT_NODEF;
   cellReCfg->pucchNoiseGamma = WR_PUCCH_NOISE_GAMA;

   cellReCfg->prachPkRatio4Pres = PRSNT_NODEF;
   cellReCfg->prachPkRatio4 = WR_PRACH_RATIO_4;

   cellReCfg->prachPkRatio0Pres = PRSNT_NODEF;
   cellReCfg->prachPkRatio0 = WR_PRACH_RATIO_0;

   cellReCfg->srsDopEstFactorPres = PRSNT_NODEF;
   cellReCfg->srsDopEstFactor = WR_SRS_DOP_EST_FACTOR;

   cellReCfg->puschProbDtxAckPres = PRSNT_NODEF;
   cellReCfg->puschProbDtxAck = WR_PUSCH_DTX_ACK;

   cellReCfg->pucchProbDtxAckPres = PRSNT_NODEF;
   cellReCfg->pucchProbDtxAck = WR_PUCCH_DTX_ACK;

   cellReCfg->txAntennaPortsPres = PRSNT_NODEF;
   cellReCfg->txAntennaPorts = WR_TX_ANTENA_PORTS;


   cellReCfg->rxAntennaPortsPres = PRSNT_NODEF;
   cellReCfg->rxAntennaPorts = WR_RX_ANTENA_PORTS;

   cellReCfg->phySyncModePres= PRSNT_NODEF;
   cellReCfg->phySyncMode = WR_PHY_SYNC_MODE;

   cellReCfg->dataRepModePres = PRSNT_NODEF;
   cellReCfg->dataRepMode = WR_DATA_REP_MODE;

   cellReCfg->rachSrRepModePres = PRSNT_NODEF;
   cellReCfg->rachSrRepMode = WR_RACH_SR_REP_MODE ;
   cellReCfg->pilotSigPwr = cellCb->shutdwnPwrCfg.hiPwrCfg.highPilotSigPwr ;
   cellReCfg->priSigPwr = cellCb->shutdwnPwrCfg.hiPwrCfg.highPriSigPwr ;
   cellReCfg->secSigPwr = cellCb->shutdwnPwrCfg.hiPwrCfg.highSecSigPwr ;
   cellReCfg->ctfCellStatus = CTF_CELL_UP;
   RLOG_ARG0(L_INFO,DBG_CELLID,cellId,"Bringing transmission power UP"); 
   if(ROK != wrIfmPhyCellCfg(cellCfgReq,cellId))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failure received while sending phy reconfiguration.");      
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of wrFillCtfCellReCfgReq */

/** @brief This function fills and send  the cell delete  configuration for PHY layer. 
 *
 * @details This function prepares and send  the cell delete configuration for PHY layer.
 *
 *     Function: wrFillCtfCellDelCfgReq
 *
 *         Processing steps:
 *         - Fill the PHY related configuration parameters.
 *         - Invoke the function wrIfmPhyCellCfg to send the filled confguration
 *           to PHY layer.
 *
 * @param[in] cellId: Cell ID.  
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrFillCtfCellDelCfgReq
(
U16  cellId
)
{
   CtfCfgReqInfo    *cellDelCfgReq = NULLP;
   WrCellCb          *cellCb = NULLP;

   TRC2(wrFillCtfCellDelCfgReq);

   RLOG_ARG0(L_INFO,DBG_CELLID,cellId,"Trigger cell delete request over CTF Interface");

   WR_GET_CELLCB(cellCb,cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid cellid received in cell delete request to PHY");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&cellDelCfgReq, sizeof(CtfCfgReqInfo));

   if(cellDelCfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed while sending cell delete request to PHY.");
      RETVALUE(RFAILED);
   } /* end of if statement */



   cellDelCfgReq->cfgType = CTF_DELETE;
   cellDelCfgReq->u.release.cfgElem = CTF_CELL_CFG;
   cellDelCfgReq->u.release.u.cellRel.cellId = (CmLteCellId)cellId; 
   if(ROK != wrIfmPhyCellCfg(cellDelCfgReq,cellId))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failure received while sending cell"
                "delete request to PHY.");      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}
/***********************************************************************
                    RGR related functions
************************************************************************/


/** @brief This function fills the downlink HARQ configuration per Cell. 
 *
 * @details This function populate RgrDlHqCfg structure.
 *
 *     Function: wrFillRgrDlHqCfg
 *
 *         Processing steps:
 *         - This function populate RgrDlHqCfg structure. 
 *
 * @param[out] cfg: Downlink HARQ configuration per Cell.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillRgrDlHqCfg
(
RgrDlHqCfg *cfg,
U16       cellId
)
{
   WrCellCb *cellCb = NULLP;
   TRC2(wrFillRgrDlHqCfg);

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invaild cellid received while filling"
                "invalid harq configuration");
      RETVALUE(RFAILED);
   }
   cfg->maxDlHqTx = cellCb->rgrCfg.dfltMaxDlHqTx;
   cfg->maxMsg4HqTx = cellCb->rgrCfg.dfltMaxMsg4HqTx;
   RETVALUE(ROK);
} /* end of wrFillRgrDlHqCfg */ 

/** @brief This function fills the Range of RNTIs need to mange by MAC.
 *
 * @details This function fills the Range of RNTIs need to mange by MAC.
 *
 *     Function: wrFillRgrRntiCfg
 *
 *         Processing steps:
 *         - This function populate RgrRntiCfg structure. 
 *
 * @param[out] cfg: Range of RNTIs managed by MAC 
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillRgrRntiCfg
(
RgrRntiCfg *cfg,
U16     cellId
)
{
   WrCellCb  *cellCb = NULLP;
   TRC2(wrFillRgrRntiCfg);
   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid Cellid while filling range"
                 "of RNTIs");
      RETVALUE(RFAILED);
   }

   cfg->startRnti = cellCb->rgrCfg.dfltStartRnti;
   cfg->size = cellCb->rgrCfg.dfltRntiSize;
   RETVALUE(ROK);
} /* end of wrFillRgrRntiCfg */ 

/** @brief This function fills the CFI configuration per cell for configuring th
 *          MAC.
 *
 * @details This function populate RgrCfiCfg structure.
 *
 *     Function: wrFillRgrCfiCfg 
 *
 *         Processing steps:
 *         - This function populate RgrCfiCfg structure. 
 *
 * @param[out] cfg: Control Format Indicator (CFI) configuration per cell. 
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillRgrCfiCfg
(
RgrCfiCfg *cfg,
U16   cellId
)
{
   WrCellCb    *cellCb = NULLP;
   TRC2(wrFillRgrCfiCfg);
   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid cellid received while"
                "filling cfi configuration");
      RETVALUE(RFAILED);
   }

   cfg->cfi = cellCb->rgrCfg.dfltCfiCfg;
   RETVALUE(ROK);
} /*end of  wrFillRgrCfiCfg */


/** @brief This function fills the target Uplink CQI to achieve through group
 * power control configured per cell.
 *
 * @details This function populate RgrUlTrgCqiCfg structure.
 *
 *     Function: wrFillRgrUlTrgCqiCfg
 *
 *         Processing steps:
 *         - This function populate RgrUlTrgCqiCfg structure. 
 *
 * @param[out] cfg: Target Uplink CQI inoramtion configured per cell
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillRgrUlTrgCqiCfg
(
RgrUlTrgCqiCfg *cfg,
U16      cellId
)
{
   WrCellCb       *cellCb = NULLP;
   TRC2(wrFillRgrUlTrgCqiCfg);
   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid cellid received while"
                "filling Ul CQI configuration");
      RETVALUE(RFAILED);
   }

   cfg->trgCqi = cellCb->rgrCfg.dfltTrgUlCqi;
   RETVALUE(ROK);
} /* end of wrFillRgrUlTrgCqiCfg */

/** @brief This function fills the downlink common channel code rate
 *         configuration per cell used to configure the MAC.
 *
 * @details This fiunction populate RgrDlCmnCodeRateCfg structure.
 *
 *     Function: wrFillRgrDlCmnCodeRateCfg
 *
 *         Processing steps:
 *         - This fiunction populate RgrDlCmnCodeRateCfg structure.
 *
 * @param[out] cfg:  
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillRgrDlCmnCodeRateCfg
(
RgrDlCmnCodeRateCfg *cfg,
U16 cellId
)
{
   WrCellCb    *cellCb = NULLP;
   TRC2(wrFillRgrDlCmnCodeRateCfg);
   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid CellId received while"
                "filling dl common code rate configuration");
      RETVALUE(RFAILED);
   }
   cfg->bcchPchRaCodeRate = cellCb->rgrCfg.dfltBcchPchRaCodeRate;
   cfg->pdcchCodeRate = cellCb->rgrCfg.dfltPdcchCodeRate;
   cfg->ccchCqi = cellCb->rgrCfg.dfltDLCmnCodRateCcchCqi;
   RETVALUE(ROK);
} /* wrFillRgrDlCmnCodeRateCfg */

PUBLIC S16 wrEmmGetBwInPrb (WrCellBw dlBw,WrCellBw *dlBwInPrbs)
{
   switch (dlBw)
   {
      case CTF_BW_RB_15:
      		*dlBwInPrbs = 15;
		break;
      case CTF_BW_RB_25:
      		*dlBwInPrbs = 25;
		break;
      case CTF_BW_RB_50:
      		*dlBwInPrbs = 50;
		break;
      case CTF_BW_RB_75:
      		*dlBwInPrbs = 75;
		break;
      case CTF_BW_RB_100:   
      		*dlBwInPrbs = 100;
		break;
      default:
		RLOG1(L_ERROR,"Invalid bandwidth configuration received [%d]",dlBw);
      		break;
   }
   
   RETVALUE (ROK);
}
PUBLIC S16 wrEmmGetNumPHICHGrps(WrPhichGrps phichGrps,U16 *numPHICHGrps)
{
    WrCellBw dlBw = 0;
    
    wrEmmGetBwInPrb(wrEmmCb.cellCb[0]->mib.dlBw,&dlBw);
    /*  N_PHICH_GROUPS = Ng*(ceil(Nrb_dl/8)) */
    switch (phichGrps)
    {
       case WrPhichOneSixth:
            
	    *numPHICHGrps = ((dlBw - 1)/(6 * 8)) + 1; 
       break;
       case WrPhichHalf:
	    *numPHICHGrps = ((dlBw - 1)/(2 * 8)) + 1; 
       break;
       case WrPhichOne:
	    *numPHICHGrps = ((dlBw - 1)/8) + 1; 
       break;
       case WrPhichTwice:
	    *numPHICHGrps = ((dlBw - 1)/ 4) + 1; 
       break;
       default:
          RLOG1(L_ERROR,"Wrong Ng Value [%d]",phichGrps);
          break;
    }
   
    RETVALUE (ROK);
}
/** @brief This function fills the PUSCH sub-band configuration per cell used
 *         to configure the Scheduler. 
 *
 * @details This function populate RgrPuschSubBandCfg structure.
 *
 *     Function: wrFillRgrPuschSubBandCfg
 *
 *         Processing steps:
 *         - This function populate RgrPuschSubBandCfg structure.
 *
 * @param[out]   cfg: PUSCH sub-band configuration per cell.
 * @param[in] cellCb: cell control block 
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrFillRgrPuschSubBandCfg
(
RgrPuschSubBandCfg *cfg,
WrCellCb          *cellCb
)
{
   U8 dmrsCnt = 0;
   U16 numPHICHGrps = 0;
   U16 prbLowIndex = 0;
   U16 nDmrs = 0;
   U32 phichGrpSeqTbl[28][8]={{0}};  /* giving Maximum size to the table considering Ng=2 and BW=20Mhz */
   U32 grp=0,seq = 0;

   TRC2(wrFillRgrPuschSubBandCfg);

   cfg->subbandStart = cellCb->sib2.puschCfg.puschSubBndStrt;
   cfg->numSubbands = cellCb->sib2.puschCfg.nSB;
   cfg->numSubbands = cellCb->sib2.puschCfg.puschNoSubbnds;
   cfg->size = cellCb->sib2.puschCfg.puschSubBandSz;/*TODO Calculation*/
   wrEmmGetNumPHICHGrps(cellCb->mib.phichGrps,&numPHICHGrps);

   if(cfg->size > RGR_MAX_SUBBANDS)
   {
      RLOG1(L_ERROR,"Number of equal sized sub-bands invalid [%d]",cfg->size);
      RETVALUE(RFAILED);
   }

   for(dmrsCnt = 0; dmrsCnt < cfg->numSubbands; dmrsCnt++)
   {
	   prbLowIndex = (cfg->subbandStart + ( dmrsCnt * cellCb->sib2.puschCfg.puschSubBandSz));      
	   for (nDmrs=0;nDmrs < 8;nDmrs++)
	   {
		   grp=(prbLowIndex + nDmrs)% numPHICHGrps;
		   seq = ((prbLowIndex / numPHICHGrps)+ nDmrs)%8;
		   if (!phichGrpSeqTbl[grp][seq])
		   {
			   phichGrpSeqTbl[grp][seq] = 1;
			   break;		 
		   }
	   }
	   cfg->dmrs[dmrsCnt] = nDmrs%8;
   }

   RETVALUE(ROK);
} /* wrFillRgrPuschSubBandCfg */

/** @brief This function fills the uplink common channel code rate configuration
 *         per cell nedd to configure the Scheduler.
 *
 * @details This function populate RgrUlCmnCodeRateCfg structure.
 *
 *     Function:wrFillRgrUlCmnCodeRateCfg 
 *
 *         Processing steps:
 *         - This function populate RgrUlCmnCodeRateCfg structure. 
 *
 * @param[out] cfg: Uplink common channel code rate configuration per cell.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillRgrUlCmnCodeRateCfg
(
RgrUlCmnCodeRateCfg *cfg,
U16      cellId
)
{
   WrCellCb       *cellCb = NULLP;
   TRC2(wrFillRgrUlCmnCodeRateCfg);
   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid cellid received while"
                "filling Rgr Common Code Rate Configuration");
      RETVALUE(RFAILED);
   }
   cfg->ccchCqi = cellCb->rgrCfg.dfltULCmnCodRateCcchCqi;
   RETVALUE(ROK);
} /* end of wrFillRgrUlCmnCodeRateCfg */

/** @brief This function fills the MAC configuration for DLFS scheduler. 
 *
 * @details This function fills the MAC configuration for DLFS scheduler.
 *
 *     Function: wrFillRgrDlfsCfg
 *
 *         Processing steps:
 *         - This function populate RgrDlfsCfg structure. 
 *
 * @param[out] cfg: DLFS scheduler configuration.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillRgrDlfsCfg
(
RgrDlfsCfg *cfg,
U16 cellId
)
{
   WrCellCb   *cellCb = NULLP;
   TRC2(wrFillRgrDlfsCfg);

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid CellId received while "
                "filling DLFS configuration");
      RETVALUE(RFAILED);
   }
   cfg->isDlFreqSel = cellCb->rgrCfg.dfltIsDlFreqSel;
   cfg->thresholdCqi = cellCb->rgrCfg.dfltThresholdCqi;
   RETVALUE(ROK);
} /* end of wrFillRgrDlfsCfg */

/** @brief This function fills the PHICH configuration per cell for scheduler
 *         configuration.
 *
 * @details This function fills the PHICH configuration per cell for scheduler
 *          configuration.        
 *
 *     Function: wrFillRgrPhichCfg 
 *
 *         Processing steps:
 *         -This function populate RgrPhichCfg structure. 
 *
 * @param[out]   cfg: PHICH configuration per cell.
 * @param[in] cellCb: cell control block 
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillRgrPhichCfg
(
RgrPhichCfg       *cfg,
WrCellCb          *cellCb
)
{
   TRC2(wrFillRgrPhichCfg);

   switch (cellCb->mib.phichGrps)
   {
      case WR_PHICH_RES_ONE_SIXTH:
         cfg->ngEnum = RGR_NG_ONESIXTH;
         break;
      case WR_PHICH_RES_ONE_HALF:
         cfg->ngEnum = RGR_NG_HALF;
         break;
      case WR_PHICH_RES_ONE:
         cfg->ngEnum = RGR_NG_ONE;
         break;
      case WR_PHICH_RES_TWO:
         cfg->ngEnum = RGR_NG_TWO;
         break;
    } 

   /*Converting into RGR parameters*/
   switch(cellCb->mib.phichDur)
   {
      case WR_PHICH_DURATION_NORMAL:
         cfg->isDurExtend = FALSE; 
         break;
      case WR_PHICH_DURATION_EXTENDED:
         cfg->isDurExtend = TRUE;
         break;
   }  


   RETVALUE(ROK);
} /* end of wrFillRgrPhichCfg */

/** @brief This function fills the PUCCH configuration. 
 *
 * @details This function fills the PUCCH configuration in MAC cell 
 *          configuration buffer.
 *
 *     Function: wrFillRgrPucchCfg
 *
 *         Processing steps:
 *         - This function populate RgrPucchCfg structure. 
 *
 * @param[out]   cfg: PUCCH configuration per cell.
 * @param[in] cellCb: cell control block 
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrFillRgrPucchCfg
(
RgrPucchCfg       *cfg,
WrCellCb          *cellCb
)
{
   TRC2(wrFillRgrPucchCfg);

   cfg->n1PucchAn = cellCb->sib2.pucchCfg.n1PucchAn;
   /* RGR expects values like {1,2,3} */
   cfg->deltaShift = (cellCb->sib2.pucchCfg.deltaPucchShift + 1);
   cfg->cyclicShift = cellCb->sib2.pucchCfg.nCSAN;
   cfg->resourceSize = cellCb->sib2.pucchCfg.nRBCqi;
   cfg->maxPucchRb = cellCb->rgrCfg.dfltMaxPucchRbs;
   
   RETVALUE(ROK);
} /* end of wrFillRgrPucchCfg */

/** @brief This function fills the SRS configuration for schedular configuration.
 *
 * @details This function fills the SRS configuration for schedular
 *          configuration.
 *
 *     Function: wrEmmCellSchdSrsCfg
 *
 *         Processing steps:
 *         - This function populate RgrSrsCfg structure. 
 *
 * @param[out]   cfg: MAC configuration for SRS configuration per cell.
 * @param[in] cellCb: cell control block. 
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmCellSchdSrsCfg
(
RgrSrsCfg                    *cfg,
WrCellCb                     *cellCb
)
{
   TRC2(wrEmmCellSchdSrsCfg);

   switch(cellCb->sib2.srsCfg.srsBandwidthCfg)
   {
      case WR_SIB2_BWCFG0:
         cfg->srsBwEnum = RGR_SRS_BWCFG_0;
         break;
      case WR_SIB2_BWCFG1:
         cfg->srsBwEnum = RGR_SRS_BWCFG_1;
         break;
      case WR_SIB2_BWCFG2:
         cfg->srsBwEnum = RGR_SRS_BWCFG_2;
         break;
      case WR_SIB2_BWCFG3:
         cfg->srsBwEnum = RGR_SRS_BWCFG_3;
         break;
      case WR_SIB2_BWCFG4:
         cfg->srsBwEnum = RGR_SRS_BWCFG_4;
         break;
      case WR_SIB2_BWCFG5:
         cfg->srsBwEnum = RGR_SRS_BWCFG_5;
         break;
      case WR_SIB2_BWCFG6:
         cfg->srsBwEnum = RGR_SRS_BWCFG_6;
         break;
      case WR_SIB2_BWCFG7:
         cfg->srsBwEnum = RGR_SRS_BWCFG_7;
         break;
   }   
   /*ccpu00130768 - ADD - cell specific SRS enable/disable flag*/
   cfg->isSrsCfgSetup = TRUE;
   
   cfg->srsSubFrameCfg = cellCb->sib2.srsCfg.srsSubfrmCfg;
   cfg->srsCfgPrdEnum =(RgrSrsCfgPrd) (cellCb->schdCfg.prdEnum);
   /* WORKAROUND */
   cfg->srsBwEnum = RGR_SRS_BWCFG_0;
   cfg->srsSubFrameCfg = 1;
   cfg->srsCfgPrdEnum = 0;

   RETVALUE(ROK);
} /* end of wrEmmCellSchdSrsCfg */

/** @brief This function derives the Si-Window Size from RRC enum. 
 *
 * @details This function peform SI window mapping structure.
 *
 *     Function: wrEmCellSchdSiWinSzMap
 *
 *         Processing steps:
 *         - This function peform SI window mapping structure.
 *
 * @param[in] window: RRC ENUM value for SI-window.  
 * @param[out] siWindowSize: SI window value.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrEmCellSchdSiWinSzMap
(
U16                          window,
U8                           *siWindowSize
)
{
   TRC2(wrEmCellSchdSiWinSzMap);
   switch(window)
   {
      case WR_SI_WINDOWLEN_1:
         *siWindowSize  = 1;
      break;
      case WR_SI_WINDOWLEN_2:
         *siWindowSize = 2;
      break;
      case WR_SI_WINDOWLEN_5:
         *siWindowSize = 5;
      break;
      case WR_SI_WINDOWLEN_10:
         *siWindowSize = 10;
      break;
      case WR_SI_WINDOWLEN_15:
         *siWindowSize = 15;
      break;
      case WR_SI_WINDOWLEN_20:
         *siWindowSize = 20;
      break;
      case WR_SI_WINDOWLEN_40:
         *siWindowSize = 40;
      break;
   }
   RETVALUE(ROK);
} /* end of wrEmCellSchdSiWinSzMap */

/** @brief This function derives the Si-Periodicity from RRC enum. 
 *
 * @details This function peform SI period mapping structure.
 *
 *     Function: wrEmCellSchdSiPeriodMap
 *
 *         Processing steps:
 *         - This function peform SI period mapping structure.
 *
 * @param[in] period: RRC ENUM value for SI-period.  
 * @param[out] siPeriodicity: SI periodicity value.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrEmCellSchdSiPeriodMap
(
U16                          period,
RgrSiPeriodicity             *siPeriodicity
)
{
   TRC2(wrEmCellSchdSiPeriodMap);
   switch(period)
   {
      case NhuSchedulingInfosi_Periodicityrf8Enum:
         *siPeriodicity  = RGR_SI_PERD_8;
      break;
      case NhuSchedulingInfosi_Periodicityrf16Enum:
         *siPeriodicity = RGR_SI_PERD_16;
      break;
      case NhuSchedulingInfosi_Periodicityrf32Enum:
         *siPeriodicity = RGR_SI_PERD_32;

      break;
      case NhuSchedulingInfosi_Periodicityrf64Enum:
         *siPeriodicity = RGR_SI_PERD_64;
      break;
      case NhuSchedulingInfosi_Periodicityrf128Enum:
         *siPeriodicity = RGR_SI_PERD_128;
      break;
      case NhuSchedulingInfosi_Periodicityrf256Enum:
         *siPeriodicity = RGR_SI_PERD_256;
      break;
      case NhuSchedulingInfosi_Periodicityrf512Enum:
         *siPeriodicity = RGR_SI_PERD_512;
      break;
      default:
         /* Default value is currenlty considered as least value */
         RLOG1(L_ERROR,"Unknown period[%d]", period);
         *siPeriodicity  = RGR_SI_PERD_8;
   }
   RETVALUE(ROK);
} /* end of wrEmCellSchdSiPeriodMap */

/** @brief This function fills the SI confgiration structure.
 *
 * @details This function populate RgrSiCfg structure
 *
 *     Function:wrEmmCellSchdSiCfg 
 *
 *         Processing steps:
 *         - This function populate RgrSiCfg structure.
 *
 * @param[out]   cfg: SI Confgiuration buffer.
 * @param[in] cellCb: cell control block. 
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrEmmCellSchdSiCfg
(
RgrSiCfg                     *cfg,
WrCellCb                     *cellCb
)
{
   S32 idx = 0;
   U16      dfltDrxCycle;
   U8       dfltModPrdCoeff;
   TRC2(wrEmmCellSchdSiCfg);
   
   wrEmCellSchdSiWinSzMap(cellCb->sib1.siWindowLen, &cfg->siWinSize);
   cfg->numSi  = cellCb->sib1.numSis;
   for(idx = 0;idx < (cfg->numSi);idx++)
   {  
      /* PH04_CMAS : Added if check in order to skip configuration of warning SI
       * as those will be configured when write replace warning request comes
       * from MME.*/
      if((idx != WR_PBM_SIB10_SI_INDEX) &&
         (idx != WR_PBM_SIB11_SI_INDEX) &&
         (idx != WR_PBM_SIB12_SI_INDEX))
      {
         if((idx >= WR_PBM_MAX_NUM_SIS) || (idx < 0))
         {
             RETVALUE(RFAILED);
         }

         wrEmCellSchdSiPeriodMap(cellCb->sib1.siSchdInfo[idx].period, \
                 &cfg->siPeriodicity[idx]);
      }
   }
   WR_GET_PAGING_CYCLE(wrSmDfltDefaultPagCycle, dfltDrxCycle);
   WR_GET_MODIFI_COEFF(wrSmDfltModPeriodCoeff, dfltModPrdCoeff);

   cfg->modPrd = dfltDrxCycle * dfltModPrdCoeff;
   cfg->retxCnt = cellCb->schdCfg.retxCnt;
   RETVALUE(ROK);
} /* end of wrEmmCellSchdSiCfg */

/** @brief This function fills the cell-specific power configuration parameters.
 *
 * @details This function fills the cell-specific power configuration
 *          parameters.
 *
 *     Function: wrEmmCellSchdUlPwrCfg
 *
 *         Processing steps:
 *         - This function populate RgrUlPwrCfg Structure. 
 *
 * @param[out] cfg: Cell-specific power configuration buffer.
 * @param[in] cellCb: cell control block. 
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmCellSchdUlPwrCfg
(
RgrUlPwrCfg                  *cfg,
WrCellCb                     *cellCb
)
{
   TRC2(wrEmmCellSchdUlPwrCfg);

   cfg->p0NominalPusch = cellCb->sib2.ulPwrCtrlCfg.p0NominalPusch;
   cfg->p0NominalPusch = 1;// WORKAROUND cellCb->sib2.ulPwrCtrlCfg.p0NominalPusch;
   switch(cellCb->sib2.ulPwrCtrlCfg.alpha)
   {   
         case WR_SIB2_ALPHAL0:
         cfg->alpha = RGR_PWR_ALPHA0;
         break;
         case WR_SIB2_ALPHAL04:
         cfg->alpha = RGR_PWR_ALPHA4;
         break;
         case WR_SIB2_ALPHAL05:
         cfg->alpha = RGR_PWR_ALPHA5;
         break;
         case WR_SIB2_ALPHAL06:
         cfg->alpha = RGR_PWR_ALPHA6;
         break;
         case WR_SIB2_ALPHAL07:
         cfg->alpha = RGR_PWR_ALPHA7;
         break;
         case WR_SIB2_ALPHAL08:
         cfg->alpha = RGR_PWR_ALPHA8;
         break;
         case WR_SIB2_ALPHAL09:
         cfg->alpha = RGR_PWR_ALPHA9;
         break;
         case WR_SIB2_ALPHAL11:
         cfg->alpha = RGR_PWR_ALPHAALL;
         break;
   }
   cfg->p0NominalPucch = 1; /* WORKAROUND: cellCb->sib2.ulPwrCtrlCfg.p0NominlPucch;*/
   cfg->deltaPreambleMsg3 = 1; /* WORKAROUND : cellCb->sib2.ulPwrCtrlCfg.deltaPeambleMsg3; */
    cfg->alpha = RGR_PWR_ALPHA0; // WORKAROUND
   cfg->pucchPwrFmt3.startTpcRnti = cellCb->schdCfg.pwrCfg.pucchPwrFmt3Rnti;
   cfg->pucchPwrFmt3.size = cellCb->schdCfg.pwrCfg.pucchPwrFmt3Siz;
   cfg->pucchPwrFmt3a.startTpcRnti = cellCb->schdCfg.pwrCfg.pucchPwrFmt3aRnti;
   cfg->pucchPwrFmt3a.size = cellCb->schdCfg.pwrCfg.pucchPwrFmt3aSiz;
   cfg->puschPwrFmt3.startTpcRnti = cellCb->schdCfg.pwrCfg.puschPwrFmt3Rnti;
   cfg->puschPwrFmt3.size = cellCb->schdCfg.pwrCfg.puschPwrFmt3Siz;
   cfg->puschPwrFmt3a.startTpcRnti = cellCb->schdCfg.pwrCfg.puschPwrFmt3aRnti;
   cfg->puschPwrFmt3a.size = cellCb->schdCfg.pwrCfg.puschPwrFmt3aSiz;
   RETVALUE(ROK);
} /* end of wrEmmCellSchdUlPwrCfg */

/** @brief This function fills the cell-specific hopping configuration
 *         parameters used to configure MAC. 
 *
 * @details This function fills the cell-specific hopping configuration
 *         parameters used to configure MAC. 
 *         
 *     Function: wrEmmCellSchdRgrPuschCfg
 *
 *         Processing steps:
 *         - Populate RgrPuschCfg Structure.
 *
 * @param[out] cfg: cell-specific hopping configuration buffer.
 * @param[in] cellCb: cell control block. 
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmCellSchdRgrPuschCfg
(
RgrPuschCfg                  *cfg,
WrCellCb                     *cellCb
)
{
   TRC2(wrEmmCellSchdRgrPuschCfg);

   cfg->numSubBands = cellCb->sib2.puschCfg.nSB;
   cfg->hopOffst = 0; /* WORKAROUND : cellCb->sib2.puschCfg.hoppingOffset; */
   cfg->isIntraHop = 
      (cellCb->sib2.puschCfg.hoppingMode == WR_INTRA_SUBFRAME_HOPPING)?
      TRUE:FALSE;
   RETVALUE(ROK);
} /* end of wrEmmCellSchdRgrPuschCfg */

/** @brief This function fills the CSG parameters.
 *
 * @details This function fills the CSG parameters in RgrCellCfg structure
 *
 *     Function: wrEmmSchdFillCsgParam
 *
 *         Processing steps:
 *
 * @param[out] cfg: fill the CSG parameters in RgrCellCfg structure
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmSchdFillCsgParam
(
RgrCellCfg            *cfg,
U16                  cellId
)
{
   WrCellCb       *cellCb = NULLP;
   TRC2(wrEmmSchdFillCsgParam);

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid CellId received while"
                "filling CSG parameters");
      RETVALUE(RFAILED);
   }

   cfg->accsMode = cellCb->csgAccessMode;
   cfg->csgParamCfg.minDlResNonCsg = cellCb->minDlResNonCsg;
   cfg->csgParamCfg.minUlResNonCsg = cellCb->minUlResNonCsg;
   RETVALUE(ROK);
} /* wrEmmSchdFillCsgParam */

/** @brief This function fills the range of PDCCH Order Preamble Set.
 *
 * @details This function fills the range of PDCCH Order Preamble Set.
 *
 *     Function: wrEmmCellSchdPreambleSetCfg
 *
 *         Processing steps:
 *         - This function populate RgrPreambleSetCfg Structure. 
 *
 * @param[out] cfg: fill the range of PDCCH Order Preamble Set.
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmCellSchdPreambleSetCfg
(
RgrPreambleSetCfg            *cfg,
U16      cellId
)
{
   WrCellCb       *cellCb = NULLP;
   TRC2(wrEmmCellSchdPreambleSetCfg);

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid CellId received while"
                "filling preamble set configuration");
      RETVALUE(RFAILED);
   }
   cfg->pres = cellCb->schdCfg.preamPres;
   cfg->start = cellCb->schdCfg.preamStart;
   cfg->size = cellCb->schdCfg.preamSize;
   RETVALUE(ROK);
} /* wrEmmCellSchdPreambleSetCfg */

/** @brief This function derive the Number of preamble
 *
 * @details This function converts the RRC enum for number of preambles for
 *           to equvalent value need to configure scheduler.
 *
 *     Function: wrEmmCellSchdNumPreambleCfg
 *
 * @param[in]       cfgVal: Number of Preamble configuration value.
 * @param[out] numPrble :  number of preambles derived value.
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmCellSchdNumPreambleCfg
(
U8                           cfgVal,
U8                           *numPrble
)
{
   TRC2(wrEmmCellSchdNumPreambleCfg);

   switch(cfgVal)
   {

      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn4Enum:
         *numPrble = 4;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn8Enum:
         *numPrble = 8;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn12Enum:
         *numPrble = 12;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn16Enum:
         *numPrble = 16;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn20Enum:
         *numPrble = 20;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn24Enum:
         *numPrble = 24;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn28Enum:
         *numPrble = 28;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn32Enum:
         *numPrble = 32;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn36Enum:
         *numPrble = 36;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn40Enum:
         *numPrble = 40;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn44Enum:
         *numPrble = 44;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn48Enum:
         *numPrble = 48;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn52Enum:
         *numPrble = 52;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn56Enum:
         *numPrble = 56;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn60Enum:
         *numPrble = 60;
            break;
      case NhuRACH_ConfigCmnpreambleInfonumberOfRA_Preamblesn64Enum:
         *numPrble = 64;
         break;
   }

   RETVALUE(ROK);
} /* end of wrEmmCellSchdNumPreambleCfg */


/** @brief This function derive the preamble number.
 *
 * @details This function converts the RRC enum for number of preambles for
 *          GroupA value to equvalent value need to configure scheduler.
 *
 *     Function: wrEmmCellSchdPreambleCfg 
 *
 * @param[in]       cfgVal: Number of Preamble configuration value.
 * @param[out] grpASizeVal: Group A number of preambles derived value.
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmCellSchdPreambleCfg
(
U8                           cfgVal,
U8                           *grpASizeVal
)
{
   TRC2(wrEmmCellSchdPreambleCfg);

   switch(cfgVal)
   {
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn4Enum:
         *grpASizeVal = 4;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn8Enum:
         *grpASizeVal = 8;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn12Enum:
         *grpASizeVal = 12;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn16Enum:
         *grpASizeVal = 16;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn20Enum:
         *grpASizeVal = 20;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn24Enum:
         *grpASizeVal = 24;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn28Enum:
         *grpASizeVal = 28;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn32Enum:
         *grpASizeVal = 32;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn36Enum:
         *grpASizeVal = 36;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn40Enum:
         *grpASizeVal = 40;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn44Enum:
         *grpASizeVal = 44;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn48Enum:
         *grpASizeVal = 48;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn52Enum:
         *grpASizeVal = 52;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn56Enum:
         *grpASizeVal = 56;
         break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigsizeOfRA_PreamblesGroupAn60Enum:
         *grpASizeVal = 60;
         break;
   }

   RETVALUE(ROK);
} /* end of wrEmmCellSchdPreambleCfg */

/** @brief This function derive the contention resolution timer value. 
 *
 * @details This function converts the RRC enum for contention resolution timer
 *          value to equvalent value need to configure scheduler.
 *
 *     Function: wrEmmCellSchdContResTmr
 *
 * @param[in]   cfgVal: configuration value.
 * @param[out] cTmrVal: derived contention resoulution timer value.
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmCellSchdContResTmr
(
U8                           cfgVal,
U8                           *cTmrVal
)
{
   TRC2(wrEmmCellSchdContResTmr);

   switch(cfgVal)
   {
      case NhuRACH_ConfigCmnra_SupervisionInfomac_ContentionResolutionTimersf8Enum:
         *cTmrVal = 8;
         break;
      case NhuRACH_ConfigCmnra_SupervisionInfomac_ContentionResolutionTimersf16Enum:
         *cTmrVal = 16;
         break;
      case NhuRACH_ConfigCmnra_SupervisionInfomac_ContentionResolutionTimersf24Enum:
         *cTmrVal = 24;
         break;
      case NhuRACH_ConfigCmnra_SupervisionInfomac_ContentionResolutionTimersf32Enum:
         *cTmrVal = 32;
         break;
      case NhuRACH_ConfigCmnra_SupervisionInfomac_ContentionResolutionTimersf40Enum:
         *cTmrVal = 40;
         break;
      case NhuRACH_ConfigCmnra_SupervisionInfomac_ContentionResolutionTimersf48Enum:
         *cTmrVal = 48;
         break;
      case NhuRACH_ConfigCmnra_SupervisionInfomac_ContentionResolutionTimersf56Enum:
         *cTmrVal = 56;
         break;
      case NhuRACH_ConfigCmnra_SupervisionInfomac_ContentionResolutionTimersf64Enum:
         *cTmrVal = 64;
         break;
   }

   RETVALUE(ROK);
} /* end of wrEmmCellSchdContResTmr */

/** @brief This function derive the RA Group A preamble size.
 *
 * @details This function converts the RRC enum for Random Access Group-A size
 *          to equvalent value need to configure scheduler.
 *
 *     Function: wrEmmCellSchdRAGrpAMsgSize
 *
 * @param[in] cfgVal: configuration value in RRC enums.
 * @param[out] grpAVal: Equlent value in decimal.
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmCellSchdRAGrpAMsgSize
(
U8                           cfgVal,
U16                           *grpAVal
)
{
   TRC2(wrFillRAGrpAMsgSize);

   switch(cfgVal)
   {
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessageSizeGroupAb56Enum:
         *grpAVal = 56;
      break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessageSizeGroupAb144Enum:
         *grpAVal = 144;
      break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessageSizeGroupAb208Enum:
         *grpAVal = 208;
      break;
      case NhuRACH_ConfigCmnpreambleInfopreamblesGroupAConfigmessageSizeGroupAb256Enum:
         *grpAVal = 256;   /* TODO: need to ask, since U8 can have 255 */
      break;
   }

   RETVALUE(ROK);
} /* end of wrFillRAGrpAMsgSize */

/** @brief This function derive the RA Window Size.
 *
 * @details This function converts the RRC enum for Random Access Window size
 *          to equvalent value need to configure scheduler.
 *
 *     Function: wrEmmCellSchRaWinSize
 *
 * @param[in] raWinSzEnum: configuration value in RRC enums.
 * @param[out] raWinSzVal: Equlent value in decimal.
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmCellSchRaWinSize
(
U8                           raWinSzEnum,
U8                           *raWinSzVal
)
{
   switch(raWinSzEnum)
   {
      case NhuRACH_ConfigCmnra_SupervisionInfora_RspWindowSizesf2Enum:
         *raWinSzVal = 2;
      break;
      case NhuRACH_ConfigCmnra_SupervisionInfora_RspWindowSizesf3Enum:
         *raWinSzVal = 3;
      break;
      case NhuRACH_ConfigCmnra_SupervisionInfora_RspWindowSizesf4Enum:
         *raWinSzVal = 4;
      break;
      case NhuRACH_ConfigCmnra_SupervisionInfora_RspWindowSizesf5Enum:
         *raWinSzVal = 5;
      break;
      case NhuRACH_ConfigCmnra_SupervisionInfora_RspWindowSizesf6Enum:
         *raWinSzVal = 6;
      break;
      case NhuRACH_ConfigCmnra_SupervisionInfora_RspWindowSizesf7Enum:
         *raWinSzVal = 7;
      break;
      case NhuRACH_ConfigCmnra_SupervisionInfora_RspWindowSizesf8Enum:
         *raWinSzVal = 8;
      break;
      case NhuRACH_ConfigCmnra_SupervisionInfora_RspWindowSizesf10Enum:
         *raWinSzVal = 10;
      break;
   }
   RETVALUE(ROK);
}

/** @brief This function derive the T300.
 *
 * @details This function converts the RRC enum for T300 
 *          to equvalent value need to configure scheduler.
 *
 *     Function: wrEmmCellSchdT300Tmr 
 *
 * @param[in] raWinSzEnum: configuration value in RRC enums.
 * @param[out] raWinSzVal: Equlent value in decimal.
 * @return S16 
 *        -# Success : ROK
 */
PRIVATE S16 wrEmmCellSchdT300Tmr 
(
U8                            t300Enum,
U16                           *t300SchVal
)
{
   switch(t300Enum)
   {
      case 0x00:
         *t300SchVal = 10;
      break;
      case 0x01:
         *t300SchVal = 20;
      break;
      case 0x02:
         *t300SchVal = 30;
      break;
      case 0x03:
         *t300SchVal = 40;
      break;
      case 0x04:
         *t300SchVal = 60;
      break;
      case 0x05:
         *t300SchVal = 100;
      break;
      case 0x06:
         *t300SchVal = 150;
      break;
      case 0x07:
         *t300SchVal = 200;
      break;
   }
   RETVALUE(ROK);
}

/** @brief This function fills the RACH Configuration Parameter.
 *
 * @details This function fills the Scheduler RACH Configuration Parameters.
 *
 *     Function: wrEmmCellSchdRachCfg 
 *
 *         Processing steps:
 *         - This function populate RgrRachCfg Structure. 
 *
 * @param[out]    cfg: RACH configuration parameters.
 * @param[in]  cellCb: Cell Control Block 
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrEmmCellSchdRachCfg
(
RgrRachCfg                   *cfg,
WrCellCb                     *cellCb
)
{
   U8 idx = 0;

   TRC2(wrEmmCellSchdRachCfg);
   
   /*TODO Calculation from Config Index*/
   cfg->preambleFormat = cellCb->schdCfg.rachCfg.preambleFormat;

   cfg->raOccasion.size = cellCb->schdCfg.rachCfg.raOccSize;
   cfg->raOccasion.sfnEnum = cellCb->schdCfg.rachCfg.raOccSfnEnum;
   if(cfg->raOccasion.size > RGR_MAX_SUBFRAME_NUM)
   {
      RLOG1(L_ERROR,"Number of subframe numbers invalid [%d] ",cfg->raOccasion.size);
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < cfg->raOccasion.size; idx++)
   {
      cfg->raOccasion.subFrameNum[idx] = idx + 1;
   }

   cfg->maxMsg3Tx = cellCb->sib2.rachCfg.maxMsg3HarqTrans;
   wrEmmCellSchdNumPreambleCfg(cellCb->sib2.rachCfg.noRaPreambles, 
                                                    &cfg->numRaPreamble);
   wrEmmCellSchdPreambleCfg(cellCb->sib2.rachCfg.noGrpAPreambles, 
                                               &cfg->sizeRaPreambleGrpA);
   wrEmmCellSchdRAGrpAMsgSize(cellCb->sib2.rachCfg.grpAMsgSize, 
                                                  &cfg->msgSizeGrpA);
   wrEmmCellSchRaWinSize(cellCb->sib2.rachCfg.raWindowSize,
                                                      &cfg->raWinSize);
   wrEmmCellSchdContResTmr(cellCb->sib2.rachCfg.contResTimer, &cfg->contResTmr);
   cfg->prachResource = cellCb->schdCfg.rachCfg.prachResource;
   RETVALUE(ROK);
} /* end of wrEmmCellSchdRachCfg */


/** @brief This function prepares and send the scheduler/MAC cell configuration 
 *         Request.
 *
 * @details This function allocates memory for Scheduler cell configuration
 *          Request and fills the requred parametrs properly and finally call
 *          the MAC IFM function wrEmmCellSchdCfg to send to the MAC layer.
 *
 *     Function: wrEmmCellSchdCfg
 *
 *         Processing steps:
 *         - Fill the required Schduler or MAC configuration parameters.
 *         - Invoke the MAC Interface Module function wrEmmCellSchdCfg to send
 *           the config request for this cell.
 *
 * @param[in] cellId: Cell ID.
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrEmmCellSchdCfg
(
U16                          cellId
)
{
   RgrCfgReqInfo             *cfgReq = NULLP;
   RgrCellCfg                *cfg = NULLP;
   WrCellCb                  *cellCb = NULLP;
   U8                        idx;

   TRC2(wrEmmCellSchdCfg);

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid CellId received while"
                "filling scheduler cell configuration");
      RETVALUE(RFAILED);
   }

   /* Allocate RGR Cfg struct */
   WR_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
   if(cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed while filling scheduler cell"
            "configuration");
      RETVALUE(RFAILED);
   }

   /* Populate the cfg. values */
   cfgReq->action = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_CELL_CFG;
   cfg = &(cfgReq->u.cfgInfo.u.cellCfg);

   /* Populate RgrCellCfg */
   cfg->cellId = (U16)cellId;
   /* MAC instance */
   cfg->macInst              = cellCb->schdCfg.genCfg.macInst;
   /* Maximum UEs that may be scheduled UL and DL */ 
   cfg->maxUePerUlSf         = cellCb->schdCfg.genCfg.maxUePerUlSf;
   cfg->maxUePerDlSf         = cellCb->schdCfg.genCfg.maxUePerDlSf;

   /* Maximum uplink & downlink bandwidth */
   cfg->maxUlBwPerUe         = cellCb->schdCfg.genCfg.maxUlBwPerUe;
   cfg->maxDlBwPerUe         = (U8)cellCb->schdCfg.genCfg.maxDlBwPerUe;

   /* Maximum DL retransmissions B/W in RBs */
   cfg->maxDlRetxBw          = cellCb->schdCfg.genCfg.maxDlRetxBw;

   /* Number UE's per TTI for new transmissions */
   cfg->maxDlUeNewTxPerTti   = cellCb->schdCfg.genCfg.maxDlUeNewTxPerTti;

   /* Number of UEs per TTI for UL
     - correcting the assignment with actual configured */
   cfg->maxUlUeNewTxPerTti   = cellCb->schdCfg.genCfg.maxUlUeNewTxPerTti; 
   
   /* SPS changes starts */
   if (cellCb->schdCfg.spsCfg.bIsSpsEnabled == TRUE)
   {
      cfg->spsCfg.maxSpsUePerDlSf = cellCb->schdCfg.spsCfg.maxDlSpsUePerTti;
      cfg->spsCfg.maxSpsUePerUlSf = cellCb->schdCfg.spsCfg.maxUlSpsUePerTti;
      cfg->spsCfg.maxSpsDlBw      = cellCb->schdCfg.spsCfg.maxDlBwInRbs;
   }
   /* SPS changes ends */

   cfg->isDynCfiEnb = cellCb->schdCfg.isDynCfiEnb;
   cfg->isAutoCfgModeEnb = cellCb->isAutoCfgModeEnb;

   /* Number of Transmission antenna ports */
   cfg->numTxAntPorts        = cellCb->numAntPorts;  
   /* Cyclic prefix */
   cfg->isCpUlExtend         = (Bool)cellCb->sib2.ulCyclicPrefix;
   cfg->isCpDlExtend         = (Bool)cellCb->dlCyclicprefix;

   /* Cell Modulation Scheme */
   cfg->cellModSchm          = (Bool)cellCb->schdCfg.genCfg.cellModSchm;
   /* Cell Uplink transmission power */
   cfg->pMax                 = (S8)cellCb->sib1.pMax;

   /* DL scheduler to be used */
   /*SCH Type|FullyFair|MaxCI|QoS-RR|QoS-PFS|
    *TpFact  |   0     | 10  |  0   |  10   |
    *FairFact|  10     |  0  |  0   |  10   |
    */
   cfg->dlSchdType           = cellCb->schCfg.dlSchType;
   cfg->dlSchInfo.cellDlPfs.tptCoeffi = cellCb->schCfg.pfsDlTptCoefficient;
   cfg->dlSchInfo.cellDlPfs.fairCoeffi = cellCb->schCfg.pfsDlFairCoefficient;
   for(idx = 0; idx < RGR_MAX_NUM_QCI; idx++)
   {
      cfg->dlSchInfo.cellDlPfs.qciWgt[idx] = cellCb->schCfg.pfsDlQciWgt[idx];
   }   

#ifdef LTE_TDD
   cfg->ulDlCfgIdx =  cellCb->sib1.tddCfg.subframeCfg;
   cfg->spclSfCfgIdx = cellCb->sib1.tddCfg.specialSubframeCfg;
#endif
   /* UL scheduler to be used */
   /*SCH Type|FullyFair|MaxCI|QoS-RR|QoS-PFS|
    *TpFact  |   0     | 10  |  0   |  10   |
    *FairFact|  10     |  0  |  0   |  10   |
    */
   cfg->ulSchdType           = cellCb->schCfg.ulSchType;
   cfg->ulSchInfo.cellUlPfs.tptCoeffi= cellCb->schCfg.pfsUlTptCoefficient;
   cfg->ulSchInfo.cellUlPfs.fairCoeffi= cellCb->schCfg.pfsUlFairCoefficient;
   for(idx = 0; idx < RGR_MAX_NUM_QCI; idx++)
   {
      cfg->ulSchInfo.cellUlPfs.qciWgt[idx] = cellCb->schCfg.pfsUlQciWgt[idx];
   }   
   /* DLFS scheduler to use. Only 1 DLFS schd implementation
    * supported */
   cfg->dlfsSchdType         = 0; 

   /* HARQ related configuration */
   if (wrFillRgrDlHqCfg(&cfg->dlHqCfg, cellId) != ROK) 
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill downlink HARQ configuration");
      WR_FREE(cfg, sizeof(*cfg));
      RETVALUE(RFAILED);
   }

   /* Range of RNTIs to be managed by MAC */
   if (wrFillRgrRntiCfg(&cfg->macRnti, cellId) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill the Range of RNTIs");
      RETVALUE(RFAILED);
   }

   /* CFI for PDCCH */
   if (wrFillRgrCfiCfg(&cfg->cfiCfg, cellId) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill the CFI configuration");
      RETVALUE(RFAILED);
   }

   /* Target UL CQI */
   if (wrFillRgrUlTrgCqiCfg(&cfg->trgUlCqi, cellId) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill the target Uplink CQI configuration");
      RETVALUE(RFAILED);
   }

   /* Coding rate for common DL channels */
   if (wrFillRgrDlCmnCodeRateCfg(&cfg->dlCmnCodeRate, cellId) != ROK) 
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill the downlink common channel code rate configuration");
      RETVALUE(RFAILED);
   }

   /* UL sub-band information */
   if (wrFillRgrPuschSubBandCfg(&cfg->puschSubBand,cellCb) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fills the PUSCH sub-band configuration");
      RETVALUE(RFAILED);
   }

   /* Coding rate for common UL channels */
   if (wrFillRgrUlCmnCodeRateCfg(&cfg->ulCmnCodeRate, cellId) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill the uplink common channel code rate configuration");
      RETVALUE(RFAILED);
   }

   /* Configuration for DLFS scheduler */
   if (wrFillRgrDlfsCfg(&cfg->dlfsCfg, cellId) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill the MAC configuration for DLFS scheduler");
      RETVALUE(RFAILED);
   }

   /* Bandwidth configuration */ 
   cfg->bwCfg.ulTotalBw = wrFillBwCfg(cellCb->sib2.ulBw);
   cfg->bwCfg.dlTotalBw = wrFillBwCfg(cellCb->mib.dlBw);

   RLOG2(L_INFO,"ul bw = %d %d ", cellCb->sib2.ulBw, cfg->bwCfg.ulTotalBw);
   RLOG2(L_INFO,"dl bw = %d %d",cellCb->mib.dlBw, cfg->bwCfg.dlTotalBw);
   /* PHICH configuration information */
   if (wrFillRgrPhichCfg(&cfg->phichCfg,cellCb) != ROK)
   {
       RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill the PHICH configuration");
      RETVALUE(RFAILED);
   }

   /* PUCCH configuration information */
   if (wrFillRgrPucchCfg(&cfg->pucchCfg,cellCb) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill PUCCH configuration");
      RETVALUE(RFAILED);
   }

   /* SRS configuration information */ 
   if (wrEmmCellSchdSrsCfg(&cfg->srsCfg,cellCb) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill SRS configuration");
      RETVALUE(RFAILED);
   }

   /* RACH configuration */
   if (wrEmmCellSchdRachCfg(&cfg->rachCfg,cellCb) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill RACH configuration");
      RETVALUE(RFAILED);
   }

   wrEmmCellSchdT300Tmr(cellCb->sib2.t300, &cfg->t300TmrVal);
   /* SI configuration */
   if (wrEmmCellSchdSiCfg(&cfg->siCfg,cellCb) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill SI configuration");
      RETVALUE(RFAILED);
   }

   /* Cell-specific power configuration */ 
   if (wrEmmCellSchdUlPwrCfg(&cfg->pwrCfg,cellCb) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill the cell-specific power configuration");
      RETVALUE(RFAILED);
   }

   /* Cell specific hopping configuration  */
   if (wrEmmCellSchdRgrPuschCfg(&cfg->puschCfg,cellCb) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill the cell-specific hopping configuration");
      RETVALUE(RFAILED);
   }

   /* PDCCH Order Preamble Id's */
   if(wrEmmCellSchdPreambleSetCfg(&cfg->macPreambleSet, cellId) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill the PDCCH Order Preamble Set configuration");
      RETVALUE(RFAILED);
   }

   /* Configuration for common logical channels */
   if(wrFillRgrCmnLcCfg(cfg) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to fill common logical channels configuration");
      RETVALUE(RFAILED);
   }

/* LTE_ADV starts */
   wrEmmBuildInitRgrLteAdvancedConfig(&cfgReq->u.cfgInfo.u.cellCfg.rgrLteAdvCfg,cellCb);
/* LTE_ADV ends */

   cfg->rrmTtiIndPrd          = 1;

   wrEmmSchdFillCsgParam(cfg, cellId);
   RLOG2(L_INFO," ul bw = %d %d ", cellCb->sib2.ulBw, cfg->bwCfg.ulTotalBw);
   RLOG2(L_INFO,"dl bw = %d %d",cellCb->mib.dlBw, cfg->bwCfg.dlTotalBw); 
   if(ROK != WrIfmSchdCfgCell(cfgReq, (U32)cellId))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to send cell configuration request to MAC.cellId ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of  wrEmmCellSchdCfg */

/**
 *  @brief This function returns dedicated crnti range
 *
 *      Function: wrEmmCellDedCrntiRange
 *
 *          Processing steps:
 *          - get cellCb from cellId
 *          - return range of the dedicated crnti
 *
 *
 *  @param [in]  cellId  : cell ID of cell for which Dedicated CRNTI is
 *  requested
 *  @return S16
 *   -# Success : ROK
 *   -# Failure : RFAILED
 */
PUBLIC S16 wrEmmCellDedCrntiRange
(
U8                          cellId,
U16                         *minVal,
U16                         *maxVal
)
{
   WrCellCb *cellCb = NULLP;

   TRC2(wrEmmCellDedCrntiRange);

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Dedicated CRNTI requested for invalid cell");
      RETVALUE(RFAILED);
   }

   *minVal = cellCb->rgrCfg.dfltStartRnti + cellCb->rgrCfg.dfltRntiSize;
   *maxVal = (cellCb->rgrCfg.dfltStartRnti + cellCb->rgrCfg.dfltRntiSize) + cellCb->wrDfltRrmRntisize;

   RETVALUE(ROK);
}/* end of wrEmmCellDedCrntiRange */

/** @brief This function sends PRB configuration request to scheduler.
 *
 * @details This function prepare the Cell Edge UE , cell centre UE, 
 *          PRB information configured from SON which is stored in cell 
 *          control block and send to scheduler.
 *
 *
 * @param[in]  cellCb: Cell Control Block
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED.
 */

#ifdef ANSI
PUBLIC S16 wrEmmSonSchdPrbCfg
(
WrCellCb   *cellCb
)
#else
PUBLIC S16 wrEmmSonSchdPrbCfg(cellCb)
WrCellCb   *cellCb;
#endif
{
   U32                       count;
   RgrCfgReqInfo             *cfgReq;
   RgrPrbCfg                 *prbCfg;
   LwrSonSchdCfg             *schdCfg;
   RLOG0(L_INFO,"Filling SON PRB configuration over RGR");

   WR_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
   if(cfgReq == NULLP)
   {
      RLOG0(L_ERROR,"Memory allocation failure for SON PRB configuration request ");
      RETVALUE(RFAILED);
   }

   /* Populate the cfg. values */
   cfgReq->action = RGR_SON_CFG;
   cfgReq->u.sonCfg.cfgType = RGR_SON_PRB_CFG;
   prbCfg = &(cfgReq->u.sonCfg.u.prbCfg);
   prbCfg->numCellEdgeUEs = 0;
   prbCfg->numCellCentreUEs = 0;
   prbCfg->isSonIcicEnable = TRUE;
   schdCfg = &(cellCb->sonCfg.schdCfg);
   for(count = 0; count < schdCfg->numUeCfg; count ++)
   {
      if(TRUE == schdCfg->ueCfg[count].isCellEdge)
      {
         prbCfg->cellEdgeUe[prbCfg->numCellEdgeUEs] = 
            schdCfg->ueCfg[count].crnti;
         prbCfg->numCellEdgeUEs++;
      }
      else
      {
         prbCfg->cellCentreUe[prbCfg->numCellCentreUEs] = 
            schdCfg->ueCfg[count].crnti;
         prbCfg->numCellCentreUEs++;
      }
   }
   /*Fill the PRB level*/
   for(count = 0; count < RGR_SCH_MAX_PA_PER_PRB; count ++)
   {

      prbCfg->paLevelPerPrb[count] = schdCfg->paLevelPerPrb[count];

      if((U32)prbCfg->paLevelPerPrb[count] == (U32)LNL_SON_CFG_PA_LEVEL_LOW)
      {
         RLOG2(L_INFO,"PA Configuration to MAC: PRB %d has PA value %d\n",
               count, prbCfg->paLevelPerPrb[count]);
      }

   }

   RLOG2(L_INFO,"Number of Cell Edge UE's %d and Cell Center UE's %d\n",
               prbCfg->numCellEdgeUEs, prbCfg->numCellCentreUEs);

   if(ROK != WrIfmSchdCfgCell(cfgReq, (U32)cellCb->cellId))
   {
      RLOG0(L_ERROR, "Failed to send SON configuration over RGR interface to MAC \n");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}


#if WR_RECFG
PUBLIC S16 wrEmmCellSchdReCfg
(
U16                          cellId
)
{
   RgrCfgReqInfo             *cfgReq = NULLP;
   RgrCellRecfg              *reCfg = NULLP;
   WrCellCb                  *cellCb = NULLP;

   TRC2(wrEmmCellSchdReCfg);

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for cell reconfiguration");
      RETVALUE(RFAILED);
   }

   /* Allocate RGR Cfg struct */
   WR_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
   if(cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed during cell reconfiguration.");
      RETVALUE(RFAILED);
   }

   /* Populate the cfg. values */
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_CELL_CFG;
   reCfg = &(cfgReq->u.recfgInfo.u.cellRecfg);

   cmMemcpy((U8 *)reCfg, (U8 *)&(cellCb->pendRecfg.schReCfg) , sizeof(RgrCellRecfg));

   /* Populate RgrCellCfg */
   reCfg->cellId = (U16)cellId;
   
   if(ROK != WrIfmSchdCfgCell(cfgReq, (U32)cellId))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failed to send cell configuration to scheduler.");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of  wrEmmCellSchdReCfg */
#endif               
/* TODO: write function header */
PUBLIC S16 wrEmmCellSchdCmdCpuOvrldReCfg
(
U16                          cellId,
U8                           instruction
)
{

   RgrCfgReqInfo             *cfgReq = NULLP;
   RgrCellRecfg              *reCfg = NULLP;
   /* Allocate RGR Cfg struct */
   WR_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
   if(cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Populate the cfg. values */
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_CELL_CFG;

   reCfg = &(cfgReq->u.recfgInfo.u.cellRecfg);
   reCfg->cellId = cellId;
   reCfg->recfgTypes = RGR_CELL_CNTRL_CMD_RECFG; 
   reCfg->cntrlCmdCfg.cmdType = RGR_CNTRL_CMD_CPU_OVRLD;
   reCfg->cntrlCmdCfg.cmdDesc.cpuOvrLd.instruction = instruction;  

   if(ROK != WrIfmSchdCfgCell(cfgReq, (U32)cellId))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"WrSndRgrCfgReq failed. ");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of  wrEmmCellSchdCmdCpuOvrldReCfg */

/* LTE_ADV starts */
PRIVATE S16  wrEmmBuildRgrLteAdvancedConfig(RgrLteAdvancedCellConfig *rgrLteAdvCfg, LwrLteAdvancedConfigTrans  *lteAdvancedTrans)
{
/* LTE_ADV_ABS starts */
   cmMemcpy((U8 *)&rgrLteAdvCfg->absCfg,(U8 *)&lteAdvancedTrans->lteAdvancedConfig.absCfg, sizeof(RgrAbsConfig));
/* LTE_ADV_ABS ends */
   cmMemcpy((U8 *)&rgrLteAdvCfg->sfrCfg,(U8 *)&lteAdvancedTrans->lteAdvancedConfig.sfrCfg, sizeof(RgrSfrConfig));

   /*LTE_ADV_DSFR*/
   cmMemcpy((U8 *)&rgrLteAdvCfg->dsfrCfg,(U8 *)&lteAdvancedTrans->lteAdvancedConfig.dsfrCfg, sizeof(RgrDsfrConfig)); 
   rgrLteAdvCfg->pres = lteAdvancedTrans->pres;
   RETVALUE(ROK);
}

PRIVATE S16  wrEmmBuildInitRgrLteAdvancedConfig(RgrLteAdvancedCellConfig *rgrLteAdvCfg, WrCellCb  *cellCb)
{
/* LTE_ADV_ABS starts */
   cmMemset((U8 *)rgrLteAdvCfg, 0 , sizeof(RgrLteAdvancedCellConfig));
   if((cellCb->lteAdvCb.lteAdvCfg.absCfg.status) &&  (0 == cellCb->lteAdvCb.lteAdvCfg.absCfg.absLoadPeriodicity))
   {
      cmMemcpy((U8 *)&rgrLteAdvCfg->absCfg,(U8 *)&cellCb->lteAdvCb.lteAdvCfg.absCfg, sizeof(RgrAbsConfig));
      rgrLteAdvCfg->pres |= RGR_ABS;
   }
/* LTE_ADV_ABS ends */
   if(cellCb->lteAdvCb.lteAdvCfg.sfrCfg.status)
   {
      cmMemcpy((U8 *)&rgrLteAdvCfg->sfrCfg,(U8 *)&cellCb->lteAdvCb.lteAdvCfg.sfrCfg, sizeof(RgrSfrConfig));
      rgrLteAdvCfg->pres |= RGR_SFR;
   }
   
   /*LTE_ADV_DSFR*/
   if(cellCb->lteAdvCb.lteAdvCfg.dsfrCfg.status)
   {
      cmMemcpy((U8 *)&rgrLteAdvCfg->dsfrCfg,(U8 *)&cellCb->lteAdvCb.lteAdvCfg.dsfrCfg, sizeof(RgrDsfrConfig));
      rgrLteAdvCfg->pres |= RGR_DSFR;
   }

   RETVALUE(ROK);
}

PUBLIC S16 wrEmmCellSchdReCfgLteAdvFeatures
(
CmLteCellId                cellId,
LwrLteAdvancedConfigTrans  *lteAdvancedTrans
)                             /* KW fix for LTE_ADV */    
{
   RgrCfgReqInfo             *cfgReq = NULLP;
   RgrCellRecfg              *reCfg = NULLP;
   WrCellCb                  *cellCb = NULLP;

   TRC2(wrEmmCellSchdReCfgLteAdvFeatures);

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received for"
                "sending Lte Advance Features reconfiguration to scheduler");
      RETVALUE(RFAILED);
   }

   if (lteAdvancedTrans == NULLP)
   {
      RLOG0(L_WARNING, "Transaction Control Block is NULL for sending LTE"
            "advance feature reconfiguration to scheduler");
      RETVALUE(RFAILED);
   }

   /* Allocate RGR Cfg struct */
   WR_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
   if(cfgReq == NULLP)
   {
      RLOG1(L_ERROR, "Memory allocation failed for sending Lte advance features"
            "reconfiguration request.Cellid is %d ", cellId);
      RETVALUE(RFAILED);
   }

   /* Populate the cfg. values */
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_CELL_CFG;
   reCfg = &(cfgReq->u.recfgInfo.u.cellRecfg);

   reCfg->recfgTypes = RGR_CELL_LTEA_FEATURE_RECFG;
   wrEmmBuildRgrLteAdvancedConfig(&reCfg->rgrLteAdvCfg, lteAdvancedTrans);

   /* Populate RgrCellCfg */
   reCfg->cellId = cellCb->cellId;

   if(ROK != WrIfmSchdCfgCell(cfgReq, cellCb->cellId))
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellCb->cellId, "Failed to send cell configuration request to MAC");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* wrEmmCellSchdReCfgLteAdvFeatures */
/* LTE_ADV ends */

/** @brief This function sends the DCFI reconfiguration request to scheduler.
 *
 * @details This function sends the DCFI reconfiguration request to scheduler.
 *
 *     Function: wrEmmCellSchdReCfgDynCfi 
 *
 * @param[in]  cellCb: Cell Control Block 
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrEmmCellSchdReCfgDynCfi
(
WrCellCb                   *cellCb
)                             /* KW fix for LTE_ADV */    
{
   RgrCfgReqInfo             *cfgReq = NULLP;
   RgrCellRecfg              *reCfg = NULLP;

   TRC2(wrEmmCellSchdReCfgDynCfi);

   /* Allocate RGR Cfg struct */
   WR_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
   if(cfgReq == NULLP)
   {
      RLOG1(L_ERROR, "Memory allocation failed while sending the DCFI reconfiguration request to scheduler."
            "CellId is %d ", cellCb->cellId);
      RETVALUE(RFAILED);
   }

   /* Populate the cfg. values */
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_CELL_CFG;
   reCfg = &(cfgReq->u.recfgInfo.u.cellRecfg);

   reCfg->recfgTypes = RGR_CELL_DYN_CFI_RECFG;

   reCfg->isDynCfiEnb = cellCb->schdCfg.isDynCfiEnb;

   /* Populate RgrCellCfg */
   reCfg->cellId = cellCb->cellId;

   if(ROK != WrIfmSchdCfgCell(cfgReq, cellCb->cellId))
   {
      RLOG1(L_ERROR, " Failed to send cell configuration to scheduler.CellId %d ", cellCb->cellId);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function sends the Tm Auto Mode reconfiguration request to scheduler.
 *
 * @details This function sends the TM Mode Auto reconfig request to scheduler.
 *
 *     Function: wrEmmCellSchdReCfgTmMode
 *
 * @param[in]  cellCb: Cell Control Block 
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrEmmCellSchdReCfgTmMode
(
WrCellCb                   *cellCb
)                             /* KW fix for LTE_ADV */    
{
   RgrCfgReqInfo             *cfgReq = NULLP;
   RgrCellRecfg              *reCfg = NULLP;

   TRC2(wrEmmCellSchdReCfgTmMode);

   /* Allocate RGR Cfg struct */
   WR_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
   if(cfgReq == NULLP)
   {
      RLOG1(L_ERROR, "Memory allocation failed for sending the TM Mode Auto reconfiguration request."
            "CellId is %d ", cellCb->cellId);
      RETVALUE(RFAILED);
   }

   /* Populate the cfg. values */
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_CELL_CFG;
   reCfg = &(cfgReq->u.recfgInfo.u.cellRecfg);

   reCfg->recfgTypes = RGR_CELL_AUTO_CFG_MODE_RECFG;

   reCfg->isAutoCfgModeEnb = cellCb->isAutoCfgModeEnb;

   /* Populate RgrCellCfg */
   reCfg->cellId = cellCb->cellId;

   if(ROK != WrIfmSchdCfgCell(cfgReq, cellCb->cellId))
   {
      RLOG1(L_ERROR, "Failure received while sending TM automode"
            "reconfiguration request to scheduler.cellId %d ", cellCb->cellId);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
/** @brief This function prepares and send the scheduler/MAC cell delete configuration 
 *         Request.
 *
 * @details This function allocates memory for Scheduler cell delete configuration
 *          Request and fills the requred parametrs properly and finally call
 *          the MAC IFM function wrEmmCellSchdCfg to send to the MAC layer.
 *
 *     Function: wrEmmCellDelSchdCfg
 *
 *         Processing steps:
 *         - Fill the required Schduler or MAC configuration parameters.
 *         - Invoke the MAC Interface Module function wrEmmCellSchdCfg to send
 *           the config request for this cell.
 *
 * @param[in] cellId: Cell ID.
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrEmmCellDelSchdCfg
(
U16                          cellId
)
{
   RgrCfgReqInfo             *delCfgReq = NULLP;
   WrCellCb                  *cellCb = NULLP;

   TRC2(wrEmmCellDelSchdCfg);

   RLOG0(L_DEBUG, "Filling Cell delete Cfg ");

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received while"
                "filling cell delete configuration parameters");
      RETVALUE(RFAILED);
   }

   /* Allocate RGR Cfg struct */
   WR_ALLOC(&delCfgReq, sizeof(RgrCfgReqInfo));
   if(delCfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed while filling cell delete"
            "configuration parameters for SCH/MAC.");
      RETVALUE(RFAILED);
   }

   /* Populate the cfg. values */
   delCfgReq->action = RGR_DELETE;
   delCfgReq->u.delInfo.delType = RGR_CELL_CFG;
   delCfgReq->u.delInfo.u.cellDel.cellId = cellId;
   
   if(ROK != WrIfmSchdCfgCell(delCfgReq, (U32)cellId))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failure received while sending cell"
                "delete configuration request.");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}

/** @brief This function configures the Data Application Module(DAM).
 *
 * @details This function configures the Data Application Module(DAM) 
 *          to create a new cell with start RNTI and max RNTIs. 
 *
 *     Function: wrEmmCellCfgDam
 *
 *         Processing steps:
 *         - Fill the required DAM cell Information from cell confguration. 
 *         - call the DAM interface function with required cell Info to 
 *           create a new Cell in DAM Module. 
 *
 * @param[in] cellId: Cell ID. 
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrEmmCellCfgDam
(
U16                          cellId
)
{
   WrCellCb                  *cellCb;
   WrDamCellInfo             *cellInfo;

   /* Configure the Data application module */
   WR_GET_CELLCB(cellCb, cellId);
   if (NULLP == cellCb)/* Klockwork P1 warning fix */
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid cellId received for DAM"
                "configuration");
      RETVALUE(RFAILED);
   } 

   WR_ALLOC_SHAREBLE_BUF(&cellInfo,sizeof(WrDamCellInfo));
   if (NULLP == cellInfo)/* Klockwork P1 warning fix */
   {
      RLOG0(L_FATAL, "Memory allocation failed for filling DAM cell information"
            "from cell confguration.");
      RETVALUE(RFAILED);
   }
   cellInfo->cellId    = cellId;
   cellInfo->startRnti = cellCb->rgrCfg.dfltStartRnti;
   cellInfo->maxRntis  = (cellCb->rgrCfg.dfltRntiSize) +
      (cellCb->wrDfltRrmRntisize);
   wrIfmDamCellAddReq(cellId, cellInfo);
   RETVALUE(ROK);
} /* end of wrEmmCellCfgDam */

/** @brief This function get the PLMN ID based on cell ID and 
 *         selected PLMN Index.
 *
 * @details This function get the PLMN ID information based on 
 *          cell ID and selected PLMN Index.
 *
 *     Function: wrEmmCellGetSelPlmn
 *
 *         Processing steps:
 *         - Get the cell control block based on cell ID.
 *         - Get the PLMN ID information SIB1 in cell control block. 
 *
 * @param[in]      cellId: Cell ID. 
 * @param[in]  selPlmnIdx: selected PLMN Index
 * @param[out]     plmnId: PLMN ID.
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED. 
 */
PUBLIC S16 wrEmmCellGetSelPlmn
(
WrCellId                     cellId,
U8                           selPlmnIdx,
WrPlmnId                     **plmnId
)
{
   WrCellCb                  *cellCb;
   U8                        idx;
   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG1(L_ERROR,"Invalid CellId received for getting the PLMNID info[%d]"
            ,cellId);
      RETVALUE(RFAILED);
   }
   if (cellCb->sib1.numPlmnIds < selPlmnIdx)
   {
      RLOG2(L_DEBUG,"Selected PLMN Index [%d] is greater than numOfPlmnIds[%d]",
            selPlmnIdx,cellCb->sib1.numPlmnIds);
      RETVALUE(RFAILED);
   }
   for(idx = 0; idx < cellCb->sib1.numPlmnIds; idx++)
   {
      if(TRUE == cellCb->sib1.plmns[idx].activated)
      {
        selPlmnIdx--;
      }
      else
      {
        continue;
      }
      if(!selPlmnIdx)
      {
         *plmnId = &cellCb->sib1.plmns[idx].plmnId;
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
} /* end of wrEmmCellGetSelPlmn */

/* Starts - Fix for CR ccpu00123185 */
/**
 *  @brief This function build and send the cell re-Configuration           message
 *
 *      Function: wrEmmCellTxPwrReCfg 
 *
 *          Processing steps:
 *          - Allocate memory for cell reCfg message 
 *          - update the structure
 *          - call the interface function to send the message to               CL over CTF interface
 *
 *
 *  @param [in]  txType : To set low or normal Tx Power
 *  @param [in]  cellId  : cell ID of cell for which Tx Power should be modified
 *  @param [in]  transId : transid
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmCellTxPwrReCfg 
(
U16                          txType,
U8                          cellId,
U32                          transId
)
{
   CtfCfgReqInfo             *reCfgInfo = NULLP;
   CtfCellRecfgInfo          *reCellCfgParam = NULLP;
   WrCellCb *cellCb = NULLP;

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid cellId received for cell Tx"
                "Power reConfiguration");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&reCfgInfo, sizeof(CtfCfgReqInfo));
   if(reCfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed for Tx Power reconfiguration.");
      RETVALUE(RFAILED);
   }
   reCfgInfo->cfgType 	       = CTF_RECONFIG;

   if(smCb.prem == TRUE) 
   { 
      reCfgInfo->u.reCfg.cfgElem = CTF_CELL_STOP; 
   } 
   else 
   { 
      reCfgInfo->u.reCfg.cfgElem  = CTF_TX_PWR_CFG; 
   } 

   reCellCfgParam = &reCfgInfo->u.reCfg.u.cellRecfg;

   reCellCfgParam->cellId        = cellId;
   /* No S1 Connection available so setting Low tx power 
      at PHY */
   if(WR_TX_LOW == txType)
   {
      RLOG_ARG0(L_INFO,DBG_CELLID,cellId,"Bringing transmission power DOWN"); 
      reCellCfgParam->pdschCfg.refSigPwr = 
            cellCb->shutdwnPwrCfg.lwPwrCfg.lowRefSigPwr;
      reCellCfgParam->pilotSigPwr        = 
            cellCb->shutdwnPwrCfg.lwPwrCfg.lowPilotSigPwr;
      reCellCfgParam->priSigPwr          = 
            cellCb->shutdwnPwrCfg.lwPwrCfg.lowPriSigPwr;
      reCellCfgParam->secSigPwr          = 
            cellCb->shutdwnPwrCfg.lwPwrCfg.lowSecSigPwr;
      reCellCfgParam->ctfCellStatus      = CTF_CELL_DOWN;
   }
   else
   {
      RLOG_ARG0(L_INFO,DBG_CELLID,cellId,"Bringing transmission power UP"); 
      /* One S1 connection is available now so setting defalut tx power */
      reCellCfgParam->pdschCfg.refSigPwr =  
            cellCb->shutdwnPwrCfg.hiPwrCfg.highRefSigPwr;
      reCellCfgParam->pilotSigPwr        =  
            cellCb->shutdwnPwrCfg.hiPwrCfg.highPilotSigPwr;
      reCellCfgParam->priSigPwr          =  
            cellCb->shutdwnPwrCfg.hiPwrCfg.highPriSigPwr;
      reCellCfgParam->secSigPwr          =  
            cellCb->shutdwnPwrCfg.hiPwrCfg.highSecSigPwr;
      reCellCfgParam->ctfCellStatus      = CTF_CELL_UP;
   }

   if(ROK != wrIfmPhyCellCfg(reCfgInfo, transId))
   {
      RLOG0(L_ERROR,"Failed to send cell configuration to scheduler.");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}/* end of wrEmmCellTxPwrReCfg*/




/* Ends - Fix for CR ccpu00123185 */

#ifndef RM_INTF
/* @brief This function is responsible for informing RRM about the Cell
 * configuration.
 * 
 * @details
 *
 *     Function: wrEmmCellRrmCfg
 *
 *         Processing steps:
 *         - Update the Structure
 *
 * @param[in] cellCb     : Pointer to the cell Cb
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED 
 */
PUBLIC S16 wrEmmCellRrmCfg
(
WrCellCb                      *cellCb
)
{
   RrmWrCellCfgReqInfo        *rrmWrCellCfgReq = NULLP;

   TRC3 (wrEmmCellRrmCfg);

   WR_ALLOC (&rrmWrCellCfgReq, sizeof(RrmWrCellCfgReqInfo));
   if (rrmWrCellCfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed for RRM Cell configuration.");
      RETVALUE(RFAILED);
   }

   rrmWrCellCfgReq->addModOrDel           = RRM_WR_CFG_ADD;
   rrmWrCellCfgReq->cellId                = cellCb->cellId;
   rrmWrCellCfgReq->cellCfg.dlBw          = cellCb->mib.dlBw;
   rrmWrCellCfgReq->cellCfg.n2Rb          = cellCb->sib2.pucchCfg.nRBCqi;
   rrmWrCellCfgReq->cellCfg.n1Cs          = cellCb->sib2.pucchCfg.nCSAN;
   rrmWrCellCfgReq->cellCfg.isDlfsSchd    = cellCb->schdCfg.genCfg.dlfsSchdType;
   rrmWrCellCfgReq->cellCfg.maxNumUes     = cellCb->maxUeSupp;
   rrmWrCellCfgReq->cellCfg.moDataAcbTime = 
      cellCb->sib2.barringInfo.dataCall.acBarringTime;
   rrmWrCellCfgReq->cellCfg.moSigAcbTime  = 
      cellCb->sib2.barringInfo.signalling.acBarringTime;

   if (ROK != wrIfmRrmCellCfgReq(rrmWrCellCfgReq))
   {
      RLOG1(L_ERROR, "Failure received while sending RRM cell add"
            "request.Cellid is %d",rrmWrCellCfgReq->cellId);
      RETVALUE(RFAILED);
   }
   RLOG0(L_INFO, "Cell ADD configuration is successfull at RRM.");
   RETVALUE(ROK);
}/* wrEmmCellRrmCfg */

/* @brief This function is responsible for informing RRM about the Cell
 * delete.
 * 
 * @details
 *
 *     Function: wrEmmCellDelRrmCfg
 *
 *         Processing steps:
 *         - Update the Structure
 *
 * @param[in] cellCb     : Pointer to the cell Cb
 * @return S16
 *     -# Success : ROK
 *     -# Failure : RFAILED 
 */
PUBLIC S16 wrEmmCellDelRrmCfg
(
WrCellCb                      *cellCb
)
{
   RrmWrCellCfgReqInfo        *rrmWrCellDelReq = NULLP;

   TRC3 (wrEmmCellDelRrmCfg);

   WR_ALLOC(&rrmWrCellDelReq, sizeof(RrmWrCellCfgReqInfo));
   if (rrmWrCellDelReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed for cell delete RRM Cfg.");
      RETVALUE(RFAILED);
   }

   rrmWrCellDelReq->addModOrDel = RRM_WR_CFG_DEL;
   rrmWrCellDelReq->cellId      = cellCb->cellId;

   if (ROK != wrIfmRrmCellCfgReq(rrmWrCellDelReq))
   {
      RLOG1(L_ERROR,"Failure received while sending cell delete to RRM."
            "Cell id is %d",rrmWrCellDelReq->cellId);
      RETVALUE(RFAILED);
   }
    RLOG0(L_INFO,"Sent cell delete to RRM");
   RETVALUE(ROK);
}/* wrEmmCellDelRrmCfg */
#endif /* RM_INTF */

/** @brief This function is used to send MAC cell recconfiguration request to MAC
 *         scheduler layer for csg parameters
 *
 * @details
 *
 *     Function: wrEmmSndSchdReCfgReq
 *
 *         Processing steps:
 *
 *        -# Success : ROK
 *        -# Failure : RFAILED 
 */

PUBLIC S16 wrEmmSndSchdReCfgReq
(
U32  cellId,
U32  minDlResNonCsg,
U32  minUlResNonCsg
)
{
   RgrCfgReqInfo             *cfgReq = NULLP;
   RgrCellRecfg              *reCfg = NULLP;
   WrCellCb                  *cellCb = NULLP;

   TRC2(wrEmmSndSchdReCfgReq);

   RLOG0(L_INFO,"Filling Cell ReCfg over RGR");

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Invalid cellId received for MAC cell"
                "reconfiguration request");
      RETVALUE(RFAILED);
   }

   /* Allocate RGR Cfg struct */
   WR_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
   if(cfgReq == NULLP)
   {
      RLOG_ARG0(L_FATAL,DBG_CELLID,cellId,"Memory allocation failed for MAC cell reconfiguration request.");
      RETVALUE(RFAILED);
   }

   /* Populate the cfg. values */
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_CELL_CFG;
   reCfg = &(cfgReq->u.recfgInfo.u.cellRecfg);
   reCfg->recfgTypes = RGR_CELL_CSG_PARAM_RECFG;
   reCfg->csgParamCfg.minDlResNonCsg = minDlResNonCsg;
   reCfg->csgParamCfg.minUlResNonCsg = minUlResNonCsg;
   /* Populate RgrCellCfg */
   reCfg->cellId = (U16)cellId;
   
   if(ROK != WrIfmSchdCfgCell(cfgReq, (U32)cellId))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Failure received while sending cell"
                "reconfiguration request to MAC");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of WrIfmSchdCfgCell */


/********************************************************************30**

           End of file:     fc_emm_cell_t2k.c@@/main/T2K_Merge_BR/Br_Tenb_GA2.0_Intg/2 - Wed Oct 16 14:44:40 2013

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
SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
