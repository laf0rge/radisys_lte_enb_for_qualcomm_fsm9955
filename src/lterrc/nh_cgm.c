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
  
     Desc:     RRC Lower Layer Configuration Handlers. 

     File:     nh_cgm.c

     Sid:      ni_cgm.c@@/main/3 - Fri Jul  1 01:13:13 2011

     Prg:      mc 

**********************************************************************/

/** @defgroup cgm_group Configuration Module
 *  @{
 */



/* header include files (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=84;
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
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
#include "cm5.x"           /* Timer services */
#include "cm_lib.x"        /* Common library definitions */
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_llist.x"      /* Common Link list definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_lte.x"        /* Common LTE includes */
#include "lnh.x"           /* LNH Interface includes */
#include "nhu.x"           /* NHU Interface includes */
#include "nh.x"            /* RRC includes */

/* RRC Control block structure */
PUBLIC NhCb nhCb;

PRIVATE Void nhCGMUpdTransCbRlsRbCfgMask ARGS ((TransCb *transCtrlBlk,
                                               NhActvUeCb *ueCb));  
PRIVATE Void nhCGMUpdTransCbModRbCfgMask ARGS ((TransCb *transCtrlBlk));
PRIVATE Void nhCGMUpdTransCbAddRbCfgMask ARGS ((TransCb *transCtrlBlk));
PRIVATE Void nhCGMUpdTransCbSecCfgMask   ARGS ((TransCb *transCtrlBlk));
PRIVATE S16 nhCGMUpdCellCfgInOrdrdCfg ARGS (( NhuCellCfgReqSdus *cellCfgSdu,
                                              NhCellOrdrdCfg    *ordrdCfg));
PRIVATE S16 nhCGMUpdPdcpSduStaUeCbExpCfg ARGS ((TransCb *transCtrlBlk,
                                              NhuUeCfgInfo *ueCfg));
PUBLIC S16 nhCGMUpdTransCbResetMacCfgMask ARGS(( TransCb *transCtrlBlk));
PUBLIC S16 nhCGMUpdTransCbRestbSrb1CfgMask ARGS(( TransCb *transCtrlBlk));
PRIVATE S16 nhCGMUpdUeCbExpCfg ARGS(( TransCb        *transCtrlBlk,
                                   NhuUeCfgInfo   *ueCfg));
PRIVATE Void nhCGMUpdTransCbUeCfgMask ARGS(( TransCb *transCtrlBlk,
                                          NhActvUeCb        *ueCb));
#ifdef RRC_PHY_CFG_SUPP
PRIVATE Void nhCGMUpdTransCbPhyCfgMask ARGS(( TransCb *transCtrlBlk,
                                           NhActvUeCb        *ueCb));
#endif
PRIVATE Void nhCGMUpdTransCbRestbRbCfgMask ARGS(( TransCb *transCtrlBlk,
                                               NhActvUeCb *ueCb));
/**
 * @brief Handler for updating the UE control block configuration.
 *
 * @details
 *
 *     This function updates the UE control block configuration with
 *     the UE configuration information received from the RRC user.
 *           
 *  @param[in] transCtrlBlk  Transaction control block 
 *  @param[in] ueCfg    Explicit configuration
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 nhCGMUpdUeCbExpCfg
(
TransCb        *transCtrlBlk, 
NhuUeCfgInfo   *ueCfg
)
#else /* ANSI */
PRIVATE S16 nhCGMUpdUeCbExpCfg(transCtrlBlk, ueCfg)
TransCb        *transCtrlBlk;
NhuUeCfgInfo   *ueCfg;
#endif /* ANSI */
{
   U8         noRbToCfg;
   
   TRC2(nhCGMUpdUeCbExpCfg)

   RLOG0(L_DEBUG, "Updating Explicit Configuration in Trans Control Block ");

   if (ueCfg->ueCfgBitMsk & NHU_EXP_CFG_UE_CFG)
   { 
      RLOG0(L_DEBUG, "Updating UE CFG in ordered cfg ");
      /* transCtrlBlk->ordrdCfg.ueInfo is updated by CCM module during
      * connection setup and connection establishment case.  */
      if(transCtrlBlk->cause == NH_TRANSCB_CFG_REQ)
      {
         transCtrlBlk->ordrdCfg.actionMask |= NH_ORD_CFG_UEID;
      }
   } /* if (ueCfg->ueCfgBitMsk & NHU_EXP_CFG_UE_CFG) */

   if (ueCfg->ueCfgBitMsk & NHU_EXP_CFG_RB_CFG)
   { 
      RLOG0(L_DEBUG, "Updating Rb CFG in ordered cfg ");
      if (ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_ADD)
      {
         RLOG0(L_DEBUG, "Updating Rb ADD CFG in ordered cfg ");

         transCtrlBlk->ordrdCfg.rbToAddCfgList.numOfRbCfgInfo = 
              ueCfg->rbCfgAllList.rbToAddCfgList.numOfRbCfgInfo;

         transCtrlBlk->ordrdCfg.rbToAddCfgList.isHO = 
                           ueCfg->rbCfgAllList.rbToAddCfgList.isHO;
          
         noRbToCfg = transCtrlBlk->ordrdCfg.rbToAddCfgList.numOfRbCfgInfo;
         /* Alocate memory for the RbCfg */
         NH_ALLOC(NH_REG, NH_POOL, 
                  &transCtrlBlk->ordrdCfg.rbToAddCfgList.rbCfgInfoList, 
                  noRbToCfg * sizeof(NhuRbCfgInfo));
         
         if (NULLP == transCtrlBlk->ordrdCfg.rbToAddCfgList.rbCfgInfoList)
         {
            RLOG0(L_FATAL, "Failed to allocate memory for Add RB list.");
            RETVALUE(RFAILED);
         }

         NH_MEM_COPY(transCtrlBlk->ordrdCfg.rbToAddCfgList.rbCfgInfoList, 
                     ueCfg->rbCfgAllList.rbToAddCfgList.rbCfgInfoList, 
                     noRbToCfg * sizeof(NhuRbCfgInfo));
                  
         transCtrlBlk->ordrdCfg.actionMask |= NH_ORD_CFG_RB_ADD;
      }
      if(ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_MODIFY)
      {
         RLOG0(L_DEBUG, "Updating Rb MOD CFG in ordered cfg ");

         transCtrlBlk->ordrdCfg.rbToModCfgList.numOfRbCfgInfo = 
                 ueCfg->rbCfgAllList.rbToModCfgList.numOfRbCfgInfo;
          
         noRbToCfg = transCtrlBlk->ordrdCfg.rbToModCfgList.numOfRbCfgInfo;
         /* Alocate memory for the RbCfg */
         NH_ALLOC(nhCb.init.region, nhCb.init.pool,
                  &transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList, 
                  noRbToCfg * sizeof(NhuRbCfgInfo));
         
         if (NULLP == transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList)
         {
            RLOG0(L_FATAL, "Failed to allocate memory for Modified RB list.");
            RETVALUE(RFAILED);
         }
 
         NH_MEM_COPY(transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList, 
                     ueCfg->rbCfgAllList.rbToModCfgList.rbCfgInfoList,
                     noRbToCfg * sizeof(NhuRbCfgInfo));

         /* Update HO PDCP SDU information (ulBitMap) for the RBs */
         if(ROK != nhCGMUpdPdcpSduStaUeCbExpCfg(transCtrlBlk, ueCfg))
         {
            RLOG0(L_ERROR, "nhCGMUpdPdcpSduStaUeCbExpCfg function failed");
          /* purify report Fix*/
            NH_FREE(NH_REG, NH_POOL, transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList,
                                noRbToCfg * sizeof(NhuRbCfgInfo));
            RETVALUE(RFAILED);
         }

         transCtrlBlk->ordrdCfg.actionMask |= NH_ORD_CFG_RB_MOD;
     } /*  if(ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_MODIFY) */
      
     if(ueCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_RLS)
     {
        RLOG0(L_DEBUG, "Updating Rb RLS CFG in ordered cfg ");
        transCtrlBlk->ordrdCfg.rbToRlsCfgList.numOfRbCfgInfo =
                   ueCfg->rbCfgAllList.rbToRlsCfgList.numOfRbCfgInfo;

        noRbToCfg = transCtrlBlk->ordrdCfg.rbToRlsCfgList.numOfRbCfgInfo;
        /* Alocate memory for the RbCfg */
#if 0
#else
        NH_ALLOC(nhCb.init.region, nhCb.init.pool,
                 &transCtrlBlk->ordrdCfg.rbToRlsCfgList.rbsToRel,
                 noRbToCfg * sizeof(NhuRbId));
#endif

        if (NULLP == transCtrlBlk->ordrdCfg.rbToRlsCfgList.rbsToRel)
        {
           RLOG0(L_FATAL, "Failed to allocate memory for release RB list.");
           RETVALUE(RFAILED);
        }

        NH_MEM_COPY(transCtrlBlk->ordrdCfg.rbToRlsCfgList.rbsToRel,
                    ueCfg->rbCfgAllList.rbToRlsCfgList.rbsToRel,
                    noRbToCfg * sizeof(NhuRbId));

        transCtrlBlk->ordrdCfg.actionMask |= NH_ORD_CFG_RB_RLS;
     } /* if(expCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_RLS) */

   
   }/* End of if (expCfg->expCfgBitMsk & NHU_EXP_CFG_RB_CFG) */

   if (ueCfg->conReCfgCause & NHU_CONN_RECFG_AFTER_RESTB)
   {
      transCtrlBlk->ordrdCfg.actionMask |= NH_ORD_CFG_RB_RESTB;
   } /* if(expCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_RESTB) */

   if (ueCfg->conReCfgCause & NHU_CONN_RESTB_EVNT)
   {
      /*Set ordered config action mask flags related to re-establishment
        SRB1 and MAC reset.*/
      transCtrlBlk->ordrdCfg.actionMask |= NH_ORD_CFG_RESTB_SRB1;
      transCtrlBlk->ordrdCfg.actionMask |= NH_ORD_CFG_RESET_MAC;
   } /* if(expCfg->rbCfgAllList.rbCfgTypBitMsk & NHU_CFG_RB_RESTB) */


   if(ueCfg->ueCfgBitMsk & NHU_EXP_CFG_TRCH_CFG)
   {
      RLOG0(L_DEBUG, "Updating TrCh CFG in ordered cfg");
      NH_MEM_COPY(&transCtrlBlk->ordrdCfg.trChCfg,
                  &ueCfg->trChCfg,
                  sizeof(NhuTrChCfg));
      transCtrlBlk->ordrdCfg.actionMask |= NH_ORD_CFG_TRCH;
   }

   if (ueCfg->ueCfgBitMsk & NHU_EXP_CFG_SEC_CFG)
   { 
      RLOG0(L_DEBUG, "Updating Security CFG in ordered cfg");

      NH_MEM_COPY(&transCtrlBlk->ordrdCfg.secCfg,
                  &ueCfg->secCfg, sizeof(NhuSecurityCfg));

      transCtrlBlk->ordrdCfg.actionMask |= NH_ORD_CFG_SEC;
   }

#ifdef RRC_PHY_CFG_SUPP
   if (ueCfg->ueCfgBitMsk & NHU_EXP_CFG_PHY_CFG)
   {
      RLOG0(L_DEBUG, "Updating Physical layer CFG in ordered cfg");

      NH_MEM_COPY(&transCtrlBlk->ordrdCfg.phyCfg,
                  &ueCfg->phyCfg, sizeof(NhuPhyCfg));

      transCtrlBlk->ordrdCfg.actionMask |= NH_ORD_CFG_PHYCH;
   }
#endif /* RRC_PHY_CFG_SUPP */

   RETVALUE(ROK);
} /* end of nhCGMUpdUeCbExpCfg */


/**
 * @brief Handler for Configuration Request from the RRC user.
 *
 * @details
 *
 *     This function is the handler function for Configuration Request from the RRC user.
 *     This in turn calls the nhCGMLowerLyrCfgReq to configure the lower layers.
 *           
 *  @param[in]  cellAndUeCb  Structure containing both CellCb and UeCb pointers
 *  @param[in]  transCtrlBlk      Transaction control block
 *  @param[in]  cfgInfo      NhuUeCfgInfo containing the explicit configuration info
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 nhCGMCfgReq
(
NhCellAndUeCb   *cellAndUeCb,
TransCb         *transCtrlBlk,
NhuUeCfgInfo    *cfgInfo
)
#else /* ANSI */
PUBLIC S16 nhCGMCfgReq(cellAndUeCb, transCtrlBlk, cfgInfo)
NhCellAndUeCb   *cellAndUeCb;
TransCb         *transCtrlBlk;
NhuUeCfgInfo    *cfgInfo;
#endif /* ANSI */
{
   S16 retVal;
   
   TRC2(nhCGMCfgReq)

   retVal = nhCGMLowerLyrCfgReq(cellAndUeCb, cfgInfo, transCtrlBlk, TRUE);

   if (ROK != retVal)
   {
      RLOG0(L_ERROR, "Cannot perform lower layer config");
      RETVALUE(retVal);
   }

   RETVALUE(ROK);

}  /* end of nhCGMCfgReq */

/**
 *
 * @brief Handler for Cancel UE Request.
 *
 * @details
 *
 *    This function handles the Cancel UE Request
 *    from the RRC user by taking steps to remove configuration
 *    at the lower layers and then removing the associated
 *    data structures.
 *            
 *  @param[in] cellCb  Cell control block
 *  @param[in] ueCb    UE control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 nhCGMCnclUe
(
NhCellCb    *cellCb,
NhActvUeCb  *ueCb
)
#else /* ANSI */
PUBLIC S16 nhCGMCnclUe(cellCb, ueCb)
NhCellCb    *cellCb;
NhActvUeCb  *ueCb;
#endif /* ANSI */
{
   NhId nhIdentity;

   TRC2(nhCGMCnclUe)

   RLOG0(L_DEBUG, "Release lower layer Cfg.");

   /* Update the General Statistics only when RRC Connection establishment is
    * not completed */
   if(!(ueCb->rbSecCfgBitMask & NH_SRB1_CFGD))
   {
      nhUTLUpdGenSts(NH_STS_CAUSE_CON_FAIL);
   }
   nhIdentity.ueId = ueCb->ueCbKey.crnti;
   nhIdentity.cellId = cellCb->cellId;

   /* Call LIM module to release the lower layer configuration */
   if(RFAILED == nhLIMRlsLowerLyrCfg(ueCb))
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,nhIdentity.cellId,"nhLIMRlsLowerLyrCfg "
         "failed for UEID [%d]", nhIdentity.ueId);
       RETVALUE(RFAILED);
   }

   RLOG0(L_DEBUG, "delete UE CB.");
   /* call UDM module to delete the UE control block */
   if (RFAILED == nhDBMDeleteUeCb(cellCb, &ueCb))
   {
      RLOG0(L_ERROR, "nhDBMDeleteUeCb failed ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}/* end of nhCGMCnclUe */

/**
 * @brief Handler for security related configuration of PDCP layer.
 *
 * @details
 *
 *      This function updates the transaction control block security configuration with
 *      the explicit configuration information received.
 *      Sends configuration requests to the PDCP layer. The configuration can be
 *      integrity or ciphering.
 *            
 *            
 *  @param[in] cellAndUeCb    Cell and UE control blocks
 *  @param[in] transCtrlBlk  Transaction control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 nhCGMSecCfgReq
(
NhCellAndUeCb   *cellAndUeCb,
TransCb     *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhCGMSecCfgReq(cellAndUeCb, transCtrlBlk)
NhCellAndUeCb   *cellAndUeCb;
TransCb     *transCtrlBlk;
#endif /* ANSI */
{
   S16 ret = ROK;
   
   TRC2(nhCGMSecCfgReq)
 
   transCtrlBlk->cfgMask |= NH_PDCP_CONFIGURED;
   transCtrlBlk->secCfgMapTbl[NH_INTG_CFG].status = NH_STATUS_USED;
   transCtrlBlk->secCfgMapTbl[NH_INTG_CFG].action = NH_PDCP_CFG_ESTABLISH;
   transCtrlBlk->secCfgMapTbl[NH_CIPH_CFG].status = NH_STATUS_USED;
   transCtrlBlk->secCfgMapTbl[NH_CIPH_CFG].action = NH_PDCP_CFG_ESTABLISH;

   /* Copy the cfgMask in transCtrlBlk to cfmMask */
   transCtrlBlk->cfmMask = transCtrlBlk->cfgMask;

   ret = nhLIMSendCfgMsg(cellAndUeCb, transCtrlBlk);
   if(ret != ROK)
   {
      RLOG0(L_ERROR, "LIM Send Cfg Faied");
      RETVALUE(ret);
   }
      
   RETVALUE(ret);

} /* end of nhCGMSecCfgReq */

/**
 * @brief Handler for configuration of the lower layer, if the last parameter is TRUE.
 *
 * @details
 *
 *      This function updates the UE control block configuration with
 *      the explicit configuration information received.
 *      Fills the default configuration, if specified.
 *      Configuration requests to the lower layers are issued, 
 *      if indicated as TRUE in the last parameter.
 *            
 *  @param[in] cellAndUeCb  Structure containing both CellCb and UeCb.
 *  @param[in] cfgInfo      NhuUeCfgInfo containg the explicit configuration info
 *  @param[in] transCtrlBlk      Transaction control block
 *  @param[in] cfgReqd      Configuration Required (TRUE # sends configuration to lower layer)
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 nhCGMLowerLyrCfgReq
(
NhCellAndUeCb   *cellAndUeCb,
NhuUeCfgInfo    *cfgInfo,
TransCb         *transCtrlBlk,
Bool             cfgReqd
)
#else /* ANSI */
PUBLIC S16 nhCGMLowerLyrCfgReq(cellAndUeCb, cfgInfo, transCtrlBlk, cfgReqd)
NhCellAndUeCb   *cellAndUeCb;
NhuUeCfgInfo    *cfgInfo;
TransCb         *transCtrlBlk;
Bool             cfgReqd;
#endif /* ANSI */
{
   S16  ret = ROK;
   NhUeCbKey   *ueCbKey;
   
   TRC2(nhCGMLowerLyrCfgReq)

   /* Removed for Warning fix*/
   /* Check for Default Configuration */
   nhUTLFillDfltConfig(cellAndUeCb, cfgInfo);
   /* Update the Ordered Configuration if Present */
   ret = nhCGMUpdUeCbExpCfg(transCtrlBlk, cfgInfo);
   if (ROK != ret)
   {
      RLOG0(L_ERROR, "Update of ordered configuration in UeCb failed.");
      RETVALUE(ret);
   }

   /* Configuration is updated in the Ordered Configuration
    * Table. Now, the respective configuration requests must be sent 
    * to the lower layers.
    */
   if(cfgReqd)
   {
      /* Update the transCtrlBlk to inform LIM about the configurations to be done */
      nhCGMUpdTransCbCfgMask(transCtrlBlk, cellAndUeCb->ueCb);
      
      if(transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_UEID &&
         transCtrlBlk->ordrdCfg.ueInfo.crntiCfgType == NH_UE_INFO_MOD_CRNTI) 
      {
         /* Add UE CB to Cell Hash List */
         cellAndUeCb->ueCb->newUeCbKey.crnti = 
                              transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;
         ueCbKey = &(cellAndUeCb->ueCb->newUeCbKey);
         ret = nhHashListInsert(&cellAndUeCb->cellCb->newUeHlCp, 
                                (PTR)(cellAndUeCb->ueCb),
                                (U8*)(ueCbKey), sizeof(NhUeCbKey));
         if(ROK != ret)
         {
            RLOG0(L_ERROR, "UE addition to cell hash list failed.");
            RETVALUE(ret);
         }
      }
 
      /* Send configuration requests to the lower layers */
      ret = nhLIMSendCfgMsg(cellAndUeCb, transCtrlBlk);
      if(ROK != ret)
      {
         RLOG0(L_ERROR, "LIM Send Cfg Failed");
         RETVALUE(ret);
      }
   }

   RETVALUE(ROK);
}/* end of nhCGMLowerLyrCfgReq */

/**
 *
 * @brief Handler for updating the configuration mask in cell
 *        transaction control block.
 *
 * @details
 *
 *     This function updates the configuration mask in the cell
 *     transaction control block while processing the cell configuration
 *     request received from the RRC user.
 *
 *  @param[in] *cellCfgSdu  NhuCellCfgReqSdus containing the cell configurations
 *  @param[in] *cellCb      Cell control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 nhCGMUpdCellTransCbCfgMask
(
NhuCellCfgReqSdus *cellCfgSdu,
NhCellCb          *cellCb
)
#else /* ANSI */
PUBLIC S16 nhCGMUpdCellTransCbCfgMask(cellCfgSdu, cellCb)
NhuCellCfgReqSdus *cellCfgSdu;
NhCellCb          *cellCb;
#endif /* ANSI */
{
   NhuCellCfgInfo *cellCfgInfo;
   NhuRbCfgList   *rbToAddCfgList;
   NhCellTransCb  *transCb;
   S16            ret;
   S16            i;
   S16            rlcMapCnt;
   S16            macMapCnt;
   Bool           cfgMacForCell;
   NhuRbCfgInfo   *rbCfgInfo;
   /* nh002.201 - correcting cellrecfg handling */
   NhuCellReCfgInfo *cellReCfgInfo;

   TRC2(nhCGMUpdCellTransCbCfgMask)

   ret = ROK;
   rlcMapCnt = 0;
   macMapCnt = 0;
   cfgMacForCell = FALSE;

   /* Initialize the pointers */
   /* nh002.201 - correcting cellrecfg handling */
   transCb = &(cellCb->cellTransCb);

   /* Update the transCb as occupied and populate it */
   transCb->status = NH_USED;
   transCb->nxTransId = cellCfgSdu->hdr.transId;

   switch(cellCfgSdu->sdu.cfgType)
   {
      case NHU_CFGTYPE_SETUP:
      {
         /* nh002.201 - correcting cellrecfg handling */
         cellCfgInfo = &(cellCfgSdu->sdu.u.cfgInfo);
         /* Update the main action */
         transCb->cfgAction = NH_CFG_ACTN_ADD;

         transCb->cfgMask |= NH_MAC_CONFIGURED;
         transCb->macMapTbl[macMapCnt].status = NH_STATUS_USED;
         transCb->macMapTbl[macMapCnt].logChId = NH_ZERO_VAL;
         macMapCnt++;

         /* Update the RLC and MAC configurations */
         rbToAddCfgList = &(cellCfgInfo->rbToAddCfgList);

         /* Update the RLC map table */
         for(i=0; i<(rbToAddCfgList->numOfRbCfgInfo); i++)
         {
            transCb->cfgMask |= NH_RLC_CONFIGURED;

            /* Update the RLC map table */
            transCb->rlcMapTbl[rlcMapCnt].status = NH_STATUS_USED;
            transCb->rlcMapTbl[rlcMapCnt].rbId = 
                              rbToAddCfgList->rbCfgInfoList[i].rbId.rbId;
            rlcMapCnt++;
   
            /* Update the MAC map table */
            transCb->cfgMask |= NH_MAC_CONFIGURED;
            rbCfgInfo = &rbToAddCfgList->rbCfgInfoList[i];
            transCb->macMapTbl[macMapCnt].status = NH_STATUS_USED;
            if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
               (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL))
            {
               transCb->macMapTbl[macMapCnt].logChId =
                                   rbCfgInfo->lcId.lcId;
               transCb->macMapTbl[macMapCnt].dir = NH_LCID_DIR_RX;
            }
            if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) &&
                    (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_LOGCH_MAP_DL))
            {
               transCb->macMapTbl[macMapCnt].logChId =
                                   rbCfgInfo->lcId.lcId;
               transCb->macMapTbl[macMapCnt].dir = NH_LCID_DIR_TX;
            }
            if (transCb->macMapTbl[macMapCnt].dir)
            {
               macMapCnt++;
            }
         } /* end of for loop */ 

#ifdef RRC_PHY_CFG_SUPP
         /* Update mask and table for PHY config */
         if (cellCfgInfo->cellCfgBitMsk & NHU_CELL_PHY_CFG)
         {
            transCb->cfgMask |= NH_PHY_CONFIGURED;
            transCb->phyMapInfo.status = NH_STATUS_USED;
         }
#endif /* RRC_PHY_CFG_SUPP */
      }
      break;
      case NHU_CFGTYPE_MODIFY:
      {
         /* nh002.201 - correcting cellrecfg handling */
         cellReCfgInfo = &(cellCfgSdu->sdu.u.reCfgInfo);
         /* Only few parameters of MAC and PHY config can be modified.
          * These have been validated in Validate function. So need to repeat
          * the check here. */
         /* Update the main action */
         transCb->cfgAction = NH_CFG_ACTN_MODIFY;

         if (cellReCfgInfo->cellCfgBitMsk & NHU_CELL_CFG_MAC_CMN)
         {
            /* Update mask and table for MAC config */
            transCb->cfgMask |= NH_MAC_CONFIGURED;
            transCb->macMapTbl[macMapCnt].status = NH_STATUS_USED;
            transCb->macMapTbl[macMapCnt].logChId = NH_ZERO_VAL;
         }

#ifdef RRC_PHY_CFG_SUPP
         /* Update mask and table for PHY config */
         if (cellReCfgInfo->cellCfgBitMsk & NHU_CELL_PHY_CFG)
         {
            transCb->cfgMask |= NH_PHY_CONFIGURED;
            transCb->phyMapInfo.status = NH_STATUS_USED;
         }
#endif /* RRC_PHY_CFG_SUPP */
      }
      break;
      case NHU_CFGTYPE_RELEASE:
      {
         /* Update the main action */
         transCb->cfgAction = NH_CFG_ACTN_RLS;

         /* Update mask and table for MAC config */
         transCb->cfgMask |= NH_MAC_CONFIGURED;
         transCb->macMapTbl[macMapCnt].status = NH_STATUS_USED;
         transCb->macMapTbl[macMapCnt].logChId = NH_ZERO_VAL;
         transCb->macMapTbl[macMapCnt].dir = NH_ZERO_VAL;
      
         /* Update mask and table for RLC config */
         transCb->cfgMask |= NH_RLC_CONFIGURED;
         transCb->rlcMapTbl[rlcMapCnt].status = NH_STATUS_USED;
         transCb->rlcMapTbl[rlcMapCnt].rbId = NH_ZERO_VAL;

#ifdef RRC_PHY_CFG_SUPP
         /* Update mask and table for PHY config if PHY was configured */
         if (cellCb->crntCellCfgInfo.cfgBitMsk & NHU_CELL_PHY_CFG)
         {
            transCb->cfgMask |= NH_PHY_CONFIGURED;
            transCb->phyMapInfo.status = NH_STATUS_USED;
         }
#endif /* RRC_PHY_CFG_SUPP */
      }
      break;
      default:
      {
         /* Must never be here, as validation is passed by this time */
         RLOG0(L_ERROR, "Incorrect cell config type detected after validation");
         RETVALUE(RFAILED);
      }
   }

   /* Set the confirm mask to configuration mask indicating that
    * confirm is expected from all the lower layers for which 
    * a configuration is required
    */
   transCb->cfmMask = transCb->cfgMask;

   RETVALUE(ROK);
} /*  end of nhCGMUpdCellTransCbCfgMask */

/**
 *
 * @brief Handler for configuring the lower layers with Cell configuration.
 *
 * @details
 *
 *     This function updates the Cell configuration information received 
 *     from the RRC user in cellTransCb and sends configuration requests to the lower
 *     layers.
 *
 *  @param[in] cellCfgSdu  NhuCellCfgReqSdus containing the cell Configuration
 *  @param[in] ordrdCfg    Ordered Configuration information 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 nhCGMUpdCellCfgInOrdrdCfg
(
NhuCellCfgReqSdus *cellCfgSdu,
NhCellOrdrdCfg    *ordrdCfg
)
#else /* ANSI */
PRIVATE S16 nhCGMUpdCellCfgInOrdrdCfg(cellCfgSdu, ordrdCfg)
NhuCellCfgReqSdus *cellCfgSdu;
NhCellOrdrdCfg    *ordrdCfg;
#endif /* ANSI */
{
   S16 ret;
   NhuCellCfgInfo *cellCfgInfo=NULLP;
   NhuCellCfgInfo *cellCfgFrmSdu=NULLP;
   NhuCellReCfgInfo *cellRecfgFrmSdu=NULLP; 

   TRC2(nhCGMUpdCellCfgInOrdrdCfg)

   ret = ROK;
   cellCfgInfo = &(ordrdCfg->cellCfgInfo);

   switch (cellCfgSdu->sdu.cfgType)
   {
      case NHU_CFGTYPE_SETUP:
      {
         /* initialize the SDU cell config */
         cellCfgFrmSdu = &(cellCfgSdu->sdu.u.cfgInfo);

         /* copy the ordered configuration with cell config */
         NH_MEM_COPY((cellCfgInfo), (cellCfgFrmSdu),
                              sizeof(NhuCellCfgInfo));

         cellCfgInfo->rbToAddCfgList.rbCfgInfoList = NULLP;

         if (cellCfgFrmSdu->cellCfgBitMsk & NHU_CELL_CFG_RB)
         {
            NH_ALLOC(NH_REG, NH_POOL,
                     &(cellCfgInfo->rbToAddCfgList.rbCfgInfoList),
                     ((cellCfgFrmSdu->rbToAddCfgList.numOfRbCfgInfo) 
                        * sizeof(NhuRbCfgInfo)));

            if (NULLP == cellCfgInfo->rbToAddCfgList.rbCfgInfoList)
            {
               RLOG0(L_FATAL, "Failed to allocate memory for Add RB config.");
               RETVALUE(RFAILED);
            }

            /* copy the ordered configuration with cell configuration */
            NH_MEM_COPY((cellCfgInfo->rbToAddCfgList.rbCfgInfoList),
                        (cellCfgFrmSdu->rbToAddCfgList.rbCfgInfoList),
                        ((cellCfgFrmSdu->rbToAddCfgList.numOfRbCfgInfo)
                              * sizeof(NhuRbCfgInfo)));
         } /* if (cellCfgFrmSdu->cellCfgBitMsk & NHU_CELL_CFG_RB) */
      }
      break;
      case NHU_CFGTYPE_MODIFY:
      {
         /* Initialize the reconfig info from sdu pointer */
         cellRecfgFrmSdu = &(cellCfgSdu->sdu.u.reCfgInfo);

         /* copy the activation time */
         NH_MEM_COPY(&(ordrdCfg->reCfgActvnTime), 
                     &(cellRecfgFrmSdu->actvnTime), 
                     sizeof(CmLteTimingInfo));

         /* copy the reconfig IEs */
         if (cellRecfgFrmSdu->cellCfgBitMsk & NHU_CELL_CFG_MAC_CMN)
         {
            NH_MEM_COPY(&(cellCfgInfo->macCmnCfg), 
                        &(cellRecfgFrmSdu->macCmnCfg), sizeof(NhuCmnMacCfg));
         }

#ifdef RRC_PHY_CFG_SUPP
         if (cellRecfgFrmSdu->cellCfgBitMsk & NHU_CELL_PHY_CFG)
         {
            if (cellRecfgFrmSdu->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PRACH)
            {
               NH_MEM_COPY(&(cellCfgInfo->phyCellCfg.prachCfg), 
                           &(cellRecfgFrmSdu->phyCellCfg.prachCfg), 
                           sizeof(NhuPrachCfg));
            }
            if (cellRecfgFrmSdu->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PDSCH)
            {
               NH_MEM_COPY(&(cellCfgInfo->phyCellCfg.cmnPdschCfg), 
                           &(cellRecfgFrmSdu->phyCellCfg.cmnPdschCfg), 
                           sizeof(NhuCmnPdschCfg));
            }
            if (cellRecfgFrmSdu->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PUSCH)
            {
               NH_MEM_COPY(&(cellCfgInfo->phyCellCfg.cmnPuschCfg), 
                           &(cellRecfgFrmSdu->phyCellCfg.cmnPuschCfg), 
                           sizeof(NhuCmnPuschCfg));
            }
            if (cellRecfgFrmSdu->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PHICH)
            {
               NH_MEM_COPY(&(cellCfgInfo->phyCellCfg.cmnPhichCfg), 
                           &(cellRecfgFrmSdu->phyCellCfg.cmnPhichCfg), 
                           sizeof(NhuPhichCfg));
            }
            if (cellRecfgFrmSdu->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_PUCCH)
            {
               NH_MEM_COPY(&(cellCfgInfo->phyCellCfg.cmnPucchCfg), 
                           &(cellRecfgFrmSdu->phyCellCfg.cmnPucchCfg), 
                           sizeof(NhuPucchCfg));
            }
            if (cellRecfgFrmSdu->phyCellCfg.bitmask & NHU_CMN_PHY_CFG_UL_SRS)
            {
               NH_MEM_COPY(&(cellCfgInfo->phyCellCfg.srsUlCfg), 
                           &(cellRecfgFrmSdu->phyCellCfg.srsUlCfg), 
                           sizeof(NhuCmnSrsUlCfg));
            }
            cellCfgInfo->phyCellCfg.bitmask = 
                           cellRecfgFrmSdu->phyCellCfg.bitmask;
            cellCfgInfo->cellCfgBitMsk = cellRecfgFrmSdu->cellCfgBitMsk;
         }
#endif /* RRC_PHY_CFG_SUPP */
      }
      break;
      case NHU_CFGTYPE_RELEASE:
         break;
      default:
      {
         /* Should not come here, as this check has happened in 
            Validate function for cell config */
         RLOG1(L_ERROR, "Invalid config type [%d] in cell cfg",
            cellCfgSdu->sdu.cfgType);
         RETVALUE(RFAILED);
      }
      break;
   }

   RETVALUE(ROK);
}/* end of nhCGMUpdCellCfgInOrdrdCfg */

/**
 *
 * @brief Handler for configuring the lower layers with Cell configuration.
 *
 * @details
 *
 *     This function updates the Cell configuration information received 
 *     from the RRC user in cellTransCb and sends configuration requests to the lower
 *     layers.
 *
 *  @param[in] cellCfgSdu  NhuCellCfgReqSdus containing the cell Configuration
 *  @param[in] cellCb      Cell control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 nhCGMLowerLyrCellCfgReq
(
NhuCellCfgReqSdus *cellCfgSdu,
NhCellCb          *cellCb
)
#else /* ANSI */
PUBLIC S16 nhCGMLowerLyrCellCfgReq(cellCfgSdu, cellCb)
NhuCellCfgReqSdus *cellCfgSdu;
NhCellCb          *cellCb;
#endif /* ANSI */
{
   NhCellOrdrdCfg *cellOrdrdCfg;
   NhuCellCfgInfo *cellInfoFrmSdu;
   S16 ret;

   TRC2(nhCGMLowerLyrCellCfgReq)

   NH_SET_ZERO(&cellCb->cellTransCb, sizeof(NhCellTransCb));
   ret = ROK;

   /* Allocate memory for interior structures, this must be freed when 
    * configuration confirms are received
    */
   NH_ALLOC(NH_REG, NH_POOL, 
         &(cellCb->cellTransCb.cellOrdrdCfg), sizeof(NhCellOrdrdCfg));
   if (NULLP == cellCb->cellTransCb.cellOrdrdCfg)
   {
      RLOG0(L_FATAL, "Failed to allocate memory for Ordered configuration.");
      RETVALUE(RFAILED);
   }

   cellOrdrdCfg = cellCb->cellTransCb.cellOrdrdCfg;
   /* nh002.201 - correcting cellrecfg handling */

   ret = nhCGMUpdCellCfgInOrdrdCfg(cellCfgSdu, cellOrdrdCfg);
   if (ret != ROK)
   {
      RLOG0(L_ERROR,"start timer failed");
      NH_FREE(NH_REG, NH_POOL, cellCb->cellTransCb.cellOrdrdCfg, 
                  sizeof(NhCellOrdrdCfg));
      RETVALUE(ret);
   }

   /* Update the ordrdcfg configuration mask for use by LIM */
   nhCGMUpdCellTransCbCfgMask(cellCfgSdu, cellCb);

   /* Configuration is updated in the Ordered configuration
    * in the cell control block. Now, the respective configuration requests 
    * must be sent to the lower layers.
    */
   ret = nhLIMSendCellCfg(cellCb);
   if (ROK != ret)
   {
      /* Handle the error/cleanup */
      /* nh002.201 - correcting cellrecfg handling */
      if ((cellCfgSdu->sdu.cfgType == NHU_CFGTYPE_SETUP) && 
          (cellCfgSdu->sdu.u.cfgInfo.cellCfgBitMsk & NHU_CELL_CFG_RB))
      {
         cellInfoFrmSdu = &(cellCfgSdu->sdu.u.cfgInfo);
         NH_FREE(NH_REG, NH_POOL, 
                 cellOrdrdCfg->cellCfgInfo.rbToAddCfgList.rbCfgInfoList,
                 ((cellInfoFrmSdu->rbToAddCfgList.numOfRbCfgInfo) *  
                 sizeof(NhuRbCfgInfo)));

         NH_FREE(NH_REG, NH_POOL, cellCb->cellTransCb.cellOrdrdCfg, 
                     sizeof(NhCellOrdrdCfg));
      }

      RETVALUE(ret);
   }

   RETVALUE(ROK);
}/* end of nhCGMLowerLyrCellCfgReq */

/**
 *
 * @brief Handler for determining the layers impacted by the configuration to
 * update the configuration mask in the ordered configuration.
 *
 * @details
 *
 *     This function determines which layers are affected by 
 *     the configuration and updates the configuration mask in the ordered 
 *     configuration of the transaction Cb, this information is used by LIM 
 *     while configuraing the lower layers.
 *
 *  @param[in] transCtrlBlk  Transaction control block
 *  @param[in] ueCb     UE control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void nhCGMUpdTransCbUeCfgMask
(
TransCb           *transCtrlBlk,
NhActvUeCb        *ueCb
)
#else /* ANSI */
PRIVATE Void nhCGMUpdTransCbUeCfgMask(transCtrlBlk, ueCb)
TransCb           *transCtrlBlk;
NhActvUeCb        *ueCb;
#endif /* ANSI */
{
   /* nh004.201 - Correction for TRC functionality. */
   TRC2(nhCGMUpdTransCbUeCfgMask)

   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_UEID)
   {
      if(transCtrlBlk->ordrdCfg.ueInfo.crntiCfgType == NH_UE_INFO_NEW_CRNTI)
      {
         transCtrlBlk->cfgMask |= NH_MAC_CONFIGURED;
         transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].status = NH_STATUS_USED;
         transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].action = NH_MAC_CFG_ADD;
      }
      else if(transCtrlBlk->ordrdCfg.ueInfo.crntiCfgType == 
                      NH_UE_INFO_MOD_CRNTI) 
      {
#ifdef RRC_PHY_CFG_SUPP
         if (ueCb->crntCfg.bitMskPres & NH_CRNT_CFG_PHYCH)
         {
            transCtrlBlk->cfgMask |= NH_PHY_CONFIGURED;
            transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].status = 
                                     NH_STATUS_USED;
            transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].action = 
                                     NH_PHY_CFG_UEID_CHNG;
         }
#endif /* RRC_PHY_CFG_SUPP */

         transCtrlBlk->cfgMask |= NH_MAC_CONFIGURED;
         transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].status = NH_STATUS_USED;
         transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].action = 
                                     NH_MAC_CFG_MODIFY;
         
         transCtrlBlk->cfgMask |= NH_RLC_CONFIGURED;
         transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_RLC].status = NH_STATUS_USED;
         transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_RLC].action = 
                                     NH_RLC_CFG_UEID_CHNG;
         
         transCtrlBlk->cfgMask |= NH_PDCP_CONFIGURED;
         transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PDCP].status = NH_STATUS_USED;
         transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PDCP].action = 
                                                    NH_PDCP_CFG_UEID_CHNG;
      }
      else
      {
         /* Even for release case, it does not come here, as the release
            now happens directly via PIM and does not use CGM(during
            RRC connection release and cancel UE. */
         RLOG0(L_ERROR, "Invalid ue cfgtype");
         RETVOID;
      }
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_UEID) */
   else
   {
      if ((transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_TRCH) ||
          (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_SCHED))
      {
          transCtrlBlk->cfgMask |= NH_MAC_CONFIGURED;
          transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].status = NH_STATUS_USED;
          transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].action =
                                                        NH_MAC_CFG_MODIFY;
      }
   }

   RETVOID;
}/* End of nhCGMUpdTransCbUeCfgMask */
/**
 *
 * @brief Handler for determining the layers impacted by the configuration to
 * update the config mask in ordered config.
 *
 * @details
 *
 *     This function determines which layers will be affected by 
 *     the configuration and updates the config mask in ordered 
 *     config of the transaction Cb with this information for use 
 *     by LIM while configuraing the lower layers.
 *
 *  @param[in] transCtrlBlk  Transaction control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void nhCGMUpdTransCbAddRbCfgMask
(
TransCb           *transCtrlBlk
)
#else /* ANSI */
PRIVATE Void nhCGMUpdTransCbAddRbCfgMask(transCtrlBlk)
TransCb           *transCtrlBlk;
#endif /* ANSI */
{
   U8           cfgMask = 0;
   U8           macCfgMsk = 0;
   NhuRbCfgInfo *rbCfgInfo = NULLP;
   U8            rbCount;

   /* nh004.201 - Correction for TRC functionality. */
   TRC2(nhCGMUpdTransCbAddRbCfgMask)

   for (rbCount = 0; 
        rbCount < transCtrlBlk->ordrdCfg.rbToAddCfgList.numOfRbCfgInfo; 
        rbCount++)
   {
      rbCfgInfo = (NhuRbCfgInfo *)
         &(transCtrlBlk->ordrdCfg.rbToAddCfgList.rbCfgInfoList[rbCount]);

      if (((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) && 
         (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)) ||
         ((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) &&
         (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_LOGCH_MAP_DL)))
      {
         if(FALSE == macCfgMsk)
         {
            transCtrlBlk->cfgMask |= NH_MAC_CONFIGURED;
            cfgMask |= NH_MAC_CONFIGURED;
            transCtrlBlk->cfgMask |= NH_RLC_CONFIGURED;
            cfgMask |= NH_RLC_CONFIGURED;
         }
         if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
            (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)) 
         {
            transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToAdd |= 
                                       (1 << (rbCfgInfo->lcId.lcId - 1));
         }
         if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) &&
            (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_LOGCH_MAP_DL))
         {
            transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToAdd |= 
                                       (1 << (rbCfgInfo->lcId.lcId - 1));
         }

         macCfgMsk = TRUE;
      } /* if (((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) && */

      if (((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
          (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_RLC_MODE_UL)) ||
          ((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) &&
          (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_DL)))
      {
         transCtrlBlk->cfgMask |= NH_RLC_CONFIGURED;
         cfgMask |= NH_RLC_CONFIGURED;
      }

      if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
      {
         transCtrlBlk->cfgMask |= NH_PDCP_CONFIGURED;
         cfgMask |= NH_PDCP_CONFIGURED;
      }
      
      if(cfgMask & NH_RLC_CONFIGURED)
      {
         if(rbCfgInfo->rbId.rbTyp == CM_LTE_SRB)
         {
            transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_ADD] |= 
                             (1 <<(rbCfgInfo->rbId.rbId-1));
         }
         else
         {
            transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_ADD] |= 
                             (1 <<(rbCfgInfo->rbId.rbId-1));
         }
      }
      if (cfgMask & NH_PDCP_CONFIGURED)
      {
         if(rbCfgInfo->rbId.rbTyp == CM_LTE_SRB)
         {
            transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_ADD] |= 
                             (1 <<(rbCfgInfo->rbId.rbId-1));
         }
         else 
         {
            transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_ADD] |= 
                             (1 <<(rbCfgInfo->rbId.rbId-1));
         }
      }
      cfgMask = 0;
   }/* end of for loop */
   transCtrlBlk->macLchMapTbl[NH_UL_LC].addCfmStatus = 
                     transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToAdd;
   transCtrlBlk->macLchMapTbl[NH_DL_LC].addCfmStatus = 
                     transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToAdd;
   transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_ADD] = 
                     transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_ADD];
   transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_ADD] = 
                     transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_ADD];
   transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_ADD] = 
                     transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_ADD];
   transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_ADD] = 
                     transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_ADD];

   /* nh004.201 - Correction for TRC functionality. */
   RETVOID;
}/* End of nhCGMUpdTransCbAddRbCfgMask */
/**
 *
 * @brief Handler for determining the layers impacted by the configuration to
 * update the config mask in ordered config.
 *
 * @details
 *
 *     This function determines which layers will be affected by 
 *     the configuration and updates the config mask in ordered 
 *     config of the transaction Cb with this information for use 
 *     by LIM while configuraing the lower layers.
 *
 *  @param[in] transCtrlBlk  Transaction control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void nhCGMUpdTransCbModRbCfgMask
(
TransCb           *transCtrlBlk
)
#else /* ANSI */
PRIVATE Void nhCGMUpdTransCbModRbCfgMask(transCtrlBlk)
TransCb           *transCtrlBlk;
#endif /* ANSI */
{
   S16          i = 0;
   U8           cfgMask = 0;
   U8           macCfgMsk = 0;
   NhuRbCfgInfo *rbCfgInfo = NULLP;

   /* nh004.201 - Correction for TRC functionality. */   
   TRC2(nhCGMUpdTransCbModRbCfgMask)
   
   for (i = 0; i < transCtrlBlk->ordrdCfg.rbToModCfgList.numOfRbCfgInfo; i++)
   {
      rbCfgInfo = (NhuRbCfgInfo*)
              &(transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList[i]);

      if (((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
         (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)))
      {
         if(FALSE == macCfgMsk)
         {
            transCtrlBlk->cfgMask |= NH_MAC_CONFIGURED;
            cfgMask |= NH_MAC_CONFIGURED;
         }
         if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
            (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)) 
         {
            transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToMod |= 
                                     (1 << (rbCfgInfo->lcId.lcId - 1));
         }

         macCfgMsk = TRUE;
      } /* if (((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) && */

      if (((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
         (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_RLC_MODE_UL)) ||
         ((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) &&
         (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_DL)))
      {
         transCtrlBlk->cfgMask |= NH_RLC_CONFIGURED;
         cfgMask |= NH_RLC_CONFIGURED;
      }

      if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
      {
         transCtrlBlk->cfgMask |= NH_PDCP_CONFIGURED;
         cfgMask |= NH_PDCP_CONFIGURED;
      }
      
      if(cfgMask & NH_RLC_CONFIGURED)
      {
         if(rbCfgInfo->rbId.rbTyp == CM_LTE_SRB)
         {
            transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_MOD] |= 
                               (1 <<(rbCfgInfo->rbId.rbId-1));
         }
         else
         {
            transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_MOD] |= 
                               (1 <<(rbCfgInfo->rbId.rbId-1));
         }
      }
      if (cfgMask & NH_PDCP_CONFIGURED)
      {
         if(rbCfgInfo->rbId.rbTyp == CM_LTE_SRB)
         {
            transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_MOD] |= 
                               (1 <<(rbCfgInfo->rbId.rbId-1));
         }
         else
         {
            transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_MOD] |= 
                               (1 <<(rbCfgInfo->rbId.rbId-1));
         }
      }
      cfgMask = 0;
   } /* end of for loop */
   transCtrlBlk->macLchMapTbl[NH_UL_LC].modCfmStatus = 
                     transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToMod;
   transCtrlBlk->macLchMapTbl[NH_DL_LC].modCfmStatus = 
                     transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToMod;
   transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_MOD] = 
                     transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_MOD];
   transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_MOD] = 
                     transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_MOD];
   transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_MOD] = 
                     transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_MOD];
   transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_MOD] = 
                     transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_MOD];

   /* nh004.201 - Correction for TRC functionality. */
   RETVOID;
}/* End of nhCGMUpdTransCbModRbCfgMask */
/**
 *
 * @brief Handler for determining the layers impacted by the configuration to
 * update the config mask in ordered config.
 *
 * @details
 *
 *     This function determines which layers will be affected by 
 *     the configuration and updates the config mask in ordered 
 *     config of the transaction Cb with this information for use 
 *     by LIM while configuraing the lower layers.
 *
 *  @param[in] transCtrlBlk  Transaction control block
 *  @param[in] ueCb     UE control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void nhCGMUpdTransCbRlsRbCfgMask
(
TransCb           *transCtrlBlk,
NhActvUeCb        *ueCb
)
#else /* ANSI */
PRIVATE Void nhCGMUpdTransCbRlsRbCfgMask(transCtrlBlk, ueCb)
TransCb           *transCtrlBlk;
NhActvUeCb        *ueCb;
#endif /* ANSI */
{
   U8           cfgMask = 0;
   U8           macCfgMsk = 0;
   NhuRbId      rbId;
   NhuRbCfgInfo *rbCfgInfo = NULLP;
   U8            rbCount;
   CmLList      *tmpNode;
  
   TRC2(nhCGMUpdTransCbRlsRbCfgMask)
      
   for (rbCount = 0; 
        rbCount < transCtrlBlk->ordrdCfg.rbToRlsCfgList.numOfRbCfgInfo; 
        rbCount++)
   {
      if(FALSE == macCfgMsk)
      {
         transCtrlBlk->cfgMask |= NH_MAC_CONFIGURED;
         cfgMask |= NH_MAC_CONFIGURED;
         transCtrlBlk->cfgMask |= NH_RLC_CONFIGURED;
         cfgMask |= NH_RLC_CONFIGURED;
      }
      macCfgMsk = TRUE;

      rbId = transCtrlBlk->ordrdCfg.rbToRlsCfgList.rbsToRel[rbCount];
      if(rbId.rbTyp == CM_LTE_SRB)
      {
         transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_RLS] |= (1 <<(rbId.rbId-1));
      }
      else
      {
         transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_RLS] |= (1 <<(rbId.rbId-1));
      }
      
      CM_LLIST_FIRST_NODE(&(ueCb->crntCfg.rbCfgList.rbCfgInfoList), tmpNode);
      while(NULLP != tmpNode)
      {
         if ((NULLP != tmpNode->node) &&
             (((NhuRbCfgInfo*)(tmpNode->node))->rbId.rbId == rbId.rbId && 
             ((NhuRbCfgInfo*)(tmpNode->node))->rbId.rbTyp == rbId.rbTyp))
         {
            rbCfgInfo = (NhuRbCfgInfo*)(tmpNode->node);
            if(rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
            {
               transCtrlBlk->cfgMask |= NH_PDCP_CONFIGURED;
               if(rbCfgInfo->rbId.rbTyp == CM_LTE_SRB)
               {
                  transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_RLS] |= 
                                        (1 <<(rbCfgInfo->rbId.rbId-1));
               }
               else
               {
                  transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_RLS] |= 
                                        (1 <<(rbCfgInfo->rbId.rbId-1));
               }
            }
            if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
               (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL))
            {
               transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToRls |= 
                                     (1 << (rbCfgInfo->lcId.lcId - 1));
            }
         if((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) &&
               (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_LOGCH_MAP_DL))
            {
               transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToRls |= 
                                     (1 << (rbCfgInfo->lcId.lcId - 1));
            }

            break;
         } /*  if ((NULLP != tmpNode->node) && */
         
         CM_LLIST_NEXT_NODE(&ueCb->crntCfg.rbCfgList.rbCfgInfoList, tmpNode);
      } /*  while(NULLP != tmpNode) */
      cfgMask = 0;
   } /* end of for loop */
   transCtrlBlk->macLchMapTbl[NH_UL_LC].rlsCfmStatus = 
                     transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToRls;
   transCtrlBlk->macLchMapTbl[NH_DL_LC].rlsCfmStatus = 
                     transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToRls;
   transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_RLS] = 
                     transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_RLS];
   transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_RLS] = 
                     transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_RLS];
   transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_RLS] = 
                     transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_RLS];
   transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_RLS] = 
                     transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_RLS];

   /* nh004.201 - Correction for TRC functionality. */
   RETVOID;
}/* End of nhCGMUpdTransCbRlsRbCfgMask */
/**
 *
 * @brief Handler for determining the layers impacted by the configuration to
 * update the config mask in ordered config.
 *
 * @details
 *
 *     This function determines which layers will be affected by 
 *     the configuration and updates the config mask in ordered 
 *     config of the transaction Cb with this information for use 
 *     by LIM while configuraing the lower layers.
 *
 *  @param[in] transCtrlBlk  Transaction control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void nhCGMUpdTransCbSecCfgMask
(
TransCb           *transCtrlBlk
)
#else /* ANSI */
PRIVATE Void nhCGMUpdTransCbSecCfgMask(transCtrlBlk)
TransCb           *transCtrlBlk;
#endif /* ANSI */
{
   /* nh004.201 - Correction for TRC functionality. */
   TRC2(nhCGMUpdTransCbSecCfgMask)

   transCtrlBlk->cfgMask |= NH_PDCP_CONFIGURED;

   transCtrlBlk->secCfgMapTbl[NH_INTG_CFG].status = NH_STATUS_USED;
   transCtrlBlk->secCfgMapTbl[NH_INTG_CFG].action = NH_PDCP_CFG_ESTABLISH;
   transCtrlBlk->secCfgMapTbl[NH_CIPH_CFG].status = NH_STATUS_USED;
   transCtrlBlk->secCfgMapTbl[NH_CIPH_CFG].action = NH_PDCP_CFG_ESTABLISH;

   /* nh004.201 - Correction for TRC functionality. */
   RETVOID;
}/* End of nhCGMUpdTransCbSecCfgMask */

#ifdef RRC_PHY_CFG_SUPP
/**
 *
 * @brief Handler to update the config mask in the transaction control block 
 *        for PHY config elements.
 *
 * @details
 *
 *     This function determines which config elements will be affected by 
 *     the PHY configuration and updates the config mask in ordered 
 *     config of the transaction Cb with this information for use 
 *     by LIM while configuraing the lower layers.
 *
 *  @param[in] transCtrlBlk  Transaction control block
 *  @param[in] ueCb     UE control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void nhCGMUpdTransCbPhyCfgMask
(
TransCb           *transCtrlBlk,
NhActvUeCb        *ueCb
)
#else /* ANSI */
PRIVATE Void nhCGMUpdTransCbPhyCfgMask(transCtrlBlk, ueCb)
TransCb           *transCtrlBlk;
NhActvUeCb        *ueCb;
#endif /* ANSI */
{
   /* nh004.201 - Correction for TRC functionality. */
   TRC2(nhCGMUpdTransCbPhyCfgMask)

   transCtrlBlk->cfgMask |= NH_PHY_CONFIGURED;

   transCtrlBlk->phyMapInfo.status = NH_STATUS_USED;
   if(!(ueCb->crntCfg.bitMskPres & NH_CRNT_CFG_PHYCH))
   {
      /* PHY config setup case */
      transCtrlBlk->phyMapInfo.action = NH_PHY_CFG_ADD;
   }
   else
   {
      /* PHY config modify case */
      transCtrlBlk->phyMapInfo.action = NH_PHY_CFG_MODIFY;
   }

   /* nh004.201 - Correction for TRC functionality. */
   RETVOID;
} /* nhCGMUpdTransCbPhyCfgMask */
#endif /* RRC_PHY_CFG_SUPP */

/**
 *
 * @brief Handler for determining the layers impacted by the configuration to
 * update the config mask in ordered config.
 *
 * @details
 *
 *     This function determines which layers will be affected by 
 *     the configuration and updates the config mask in ordered 
 *     config of the transaction Cb with this information for use 
 *     by LIM while configuraing the lower layers.
 *
 *  @param[in] transCtrlBlk  Transaction control block
 *  @param[in] ueCb     UE control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE Void nhCGMUpdTransCbRestbRbCfgMask
(
TransCb           *transCtrlBlk,
NhActvUeCb        *ueCb
)
#else /* ANSI */
PRIVATE Void nhCGMUpdTransCbRestbRbCfgMask(transCtrlBlk, ueCb)
TransCb           *transCtrlBlk;
NhActvUeCb        *ueCb;
#endif /* ANSI */
{
  
   TRC2(nhCGMUpdTransCbRestbRbCfgMask)
  
   nhUTLGetRbToRestb(transCtrlBlk, ueCb->crntCfg.rbCfgList);    
 
   if(transCtrlBlk->ordrdCfg.rbToRestbCfgList.numOfRbCfgInfo > 0)
   {
      transCtrlBlk->cfgMask |= NH_RLC_CONFIGURED;
      transCtrlBlk->cfgMask |= NH_PDCP_CONFIGURED;
      transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_REST] = 
                   transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_REST];
      transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_REST] = 
                   transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_REST];
      transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_REST] = 
                   transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_REST];
      transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_REST] = 
                   transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_REST];

   }

   /* nh004.201 - Correction for TRC functionality. */
   RETVOID;
}/* End of nhCGMUpdTransCbRestbRbCfgMask */
/**
 *
 * @brief Handler for determining the layers impacted by the configuration to
 * update the config mask in ordered config.
 *
 * @details
 *
 *     This function determines which layers will be affected by 
 *     the configuration and updates the config mask in ordered 
 *     config of the transaction Cb with this information for use 
 *     by LIM while configuraing the lower layers.
 *
 *  @param[in] transCtrlBlk  Transaction control block
 *  @param[in] ueCb     UE control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 nhCGMUpdTransCbCfgMask
(
TransCb           *transCtrlBlk,
NhActvUeCb        *ueCb
)
#else /* ANSI */
PUBLIC S16 nhCGMUpdTransCbCfgMask(transCtrlBlk, ueCb)
TransCb           *transCtrlBlk;
NhActvUeCb        *ueCb;
#endif /* ANSI */
{

   TRC2(nhCGMUpdTransCbCfgMask)

   /* Initialize transCtrlBlk */
   transCtrlBlk->cfgMask = 0;

   /* If New Urnti/Crnti is present in ordered cfg info */
   nhCGMUpdTransCbUeCfgMask(transCtrlBlk, ueCb);

   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_ADD)
   {
      nhCGMUpdTransCbAddRbCfgMask(transCtrlBlk);
   } /*  if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_ADD) */

   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_MOD)
   {
      nhCGMUpdTransCbModRbCfgMask(transCtrlBlk);
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_MOD) */

   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS)
   {
      nhCGMUpdTransCbRlsRbCfgMask(transCtrlBlk, ueCb);
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS) */

   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RESTB)
   {
      nhCGMUpdTransCbRestbRbCfgMask(transCtrlBlk, ueCb);
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RESTB) */

   /* Security */
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_SEC)
   {
      nhCGMUpdTransCbSecCfgMask(transCtrlBlk);
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_SEC) */

   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RESTB_SRB1)
   {
      nhCGMUpdTransCbRestbSrb1CfgMask(transCtrlBlk);
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RESTB_SRB1) */

   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RESET_MAC)
   {
      nhCGMUpdTransCbResetMacCfgMask(transCtrlBlk);
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RESET_MAC) */

#ifdef RRC_PHY_CFG_SUPP
   /* PHY config */
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_PHYCH)
   {
      nhCGMUpdTransCbPhyCfgMask(transCtrlBlk, ueCb);
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_PHYCH) */
#endif /* RRC_PHY_CFG_SUPP */

   /* Copy the cfgMask in transCtrlBlk to cfmMask */
   transCtrlBlk->cfmMask = transCtrlBlk->cfgMask;

   RETVALUE(ROK);

}/*  end of nhCGMUpdTransCbCfgMask */

/**
 *
 * @brief Handler for Delete Transaction Request from the RRC user.
 *
 * @details
 *
 *      This function handles the Delete Transaction Request
 *      from the RRC user by taking steps to remove the configuration
 *      at the lower layers and then removing the associated
 *      data structures.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] transCtrlBlk  Transaction control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 nhCGMDeleteTrans
(
NhActvUeCb    *ueCb,
TransCb       *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhCGMDeleteTrans(ueCb, transCtrlBlk)
NhActvUeCb    *ueCb;
TransCb       *transCtrlBlk;
#endif /* ANSI */
{
   NhId           nhIdentity;
   NhuErrIndSdus  errInd;
   NhuHdr         hdr;
   NhuErrorInfo   *errInfo;
   S16            ret = ROK;

   TRC2(nhCGMDeleteTrans)

   NH_SET_ZERO(&hdr, sizeof(NhuHdr));
   NH_SET_ZERO(&errInd, sizeof(NhuErrIndSdus));

   nhIdentity.ueId = ueCb->ueCbKey.crnti;
   nhIdentity.cellId = ueCb->cellCb->cellId;

   RLOG0(L_DEBUG, "Release lower layer Cfg.");

   errInfo = &(errInd.sdu);
   /* If the previously ordered configuration is complete
    * then take action to revert the entire configuration
   */
   if (transCtrlBlk->ordrdCfg.actionMask)
   {
      if (transCtrlBlk->cfmStatus == transCtrlBlk->cfgMask)
      {
         /* Revert the configuration */
         ret = nhLIMHndlRvrtCfg(&nhIdentity, ueCb, transCtrlBlk);
         if(ROK != ret)
         { 
             RLOG0(L_ERROR, "nhLIMHndlRvrtCfg Failed");
             RETVALUE(RFAILED); 
         }
          
      }
      else
      {
         RLOG0(L_ERROR, "Ongoing configuration, cannot delete transCtrlBlk");
         NH_FORM_NHU_HDR(hdr, nhIdentity);
         hdr.transId = (U32)transCtrlBlk->nxTransId;
       
         NH_FILL_NHU_ERROR(*errInfo, NHU_ERRTYPE_INV_DEL_TRANS_REQ,
                                     NHU_ERRCAUSE_CFG_IN_PROGRESS); 
         NH_SEND_ERROR_IND(errInd, hdr);
         RETVALUE(RFAILED);
      }
   } 
   /* Revert is now done or transCtrlBlk is created just for sending
    * the UE message. So delete the transaction control block.
    */
   RLOG0(L_ERROR, "delete Trans CB");

   /* call UDM module to delete trans control block */
   if (RFAILED == nhDBMDelTransCb(ueCb, &transCtrlBlk))
   {
      RLOG0(L_ERROR, "nhUDMDelTransInfoBlock Failed");
      RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);
} /* end of nhCGMDeleteTrans */

/**
 * @brief Handler for configuration of PDCP layer for security configuration
 * and RB re-establishment.
 *
 * @details
 *
 *      This function updates the transaction control block security configuration with
 *      the explicit configuration information received.
 *      Sends configuration requests to the PDCP layer. The configuration can be
 *      integrity or ciphering.
 *            
 *            
 *  @param[in] transCtrlBlk  Transaction control block
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 nhCGMUpdTransCbRestbSrb1CfgMask 
(
TransCb        *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhCGMUpdTransCbRestbSrb1CfgMask(transCtrlBlk)
TransCb        *transCtrlBlk;
#endif /* ANSI */
{
   NhuRbCfgInfo *rbInfo = NULLP;
   CmLList      *tmpNode= NULLP;
   
   TRC2(nhCGMUpdTransCbRestbSrb1CfgMask)


   /******************************************************
    *PDCP specific configuration for new security keys and
    *re-estbalishement of SRB1.
    ******************************************************/
   /* Security */
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_SEC)
   {
      /*As there is a single primitive at PDPC to accept new
        security keys and re-establish SRB1. We reset NH_ORD_CFG_SEC
        flag and set NH_ORD_CFG_PDCP_RESTB_SRB1 flag*/
      transCtrlBlk->ordrdCfg.actionMask &= ~NH_ORD_CFG_SEC;
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_SEC) */
   transCtrlBlk->restbTbl[NH_RESTB_MAPTBL_IDX_PDCP_SRB1].status = 
      NH_STATUS_USED;
   transCtrlBlk->restbTbl[NH_RESTB_MAPTBL_IDX_PDCP_SRB1].action = 
      NH_PDCP_CFG_RESTB_SRB1;

   /******************************************************
    *RLC specific configuration for for re-estbalishement 
    *of SRB1.
    ******************************************************/
    NH_ALLOC(NH_REG, NH_POOL, &tmpNode, sizeof(CmLList));
    if(NULLP == tmpNode)
    {
       RLOG0(L_FATAL, "Failed to allocate memory for adding CmLList.");
       RETVALUE(ROUTRES);
    }
    NH_ALLOC(NH_REG, NH_POOL, &rbInfo, sizeof(NhuRbCfgInfo));
    if(NULLP == rbInfo)
    {
       RLOG0(L_FATAL, "Failed to allocate memory for adding radio bearer.");
       NH_FREE(NH_REG, NH_POOL, tmpNode, sizeof(CmLList));
       RETVALUE(ROUTRES);
    }

    rbInfo->rbId.rbId = NH_RB_ID_SRB1;
    rbInfo->rbId.rbTyp = CM_LTE_SRB;
    rbInfo->rbCfgBitMsk = (NHU_RB_CFG_DL | NHU_RB_CFG_UL);
    rbInfo->dlRbCfg.dlRbBitMsk = (NHU_RB_CFG_RLC_MODE_DL |NHU_RB_CFG_RLC_MODE_UL);

    tmpNode->node = (PTR)rbInfo;
    cmLListAdd2Tail(&transCtrlBlk->ordrdCfg.rbToRestbCfgList.rbCfgInfoList, 
                    tmpNode);
    transCtrlBlk->ordrdCfg.rbToRestbCfgList.numOfRbCfgInfo++;


   transCtrlBlk->cfgMask |= NH_RLC_CONFIGURED;
   transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_REST] |= 
                                  (1 << (NH_RB_ID_SRB1 - 1));
   transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_REST] = 
                        transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_REST];

   RETVALUE(ROK);
}/* End of nhCGMUpdTransCbRestbSrb1CfgMask */

#ifdef ANSI
PUBLIC S16 nhCGMUpdTransCbResetMacCfgMask 
(
TransCb        *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhCGMUpdTransCbResetMacCfgMask(transCtrlBlk)
TransCb        *transCtrlBlk;
#endif /* ANSI */
{

   TRC2(nhCGMUpdTransCbResetMacCfgMask)

   transCtrlBlk->cfgMask |= NH_MAC_CONFIGURED;
   transCtrlBlk->rstTbl[NH_RESET_MAPTBL_IDX_MAC].status = NH_STATUS_USED;
   transCtrlBlk->rstTbl[NH_RESET_MAPTBL_IDX_MAC].action = NH_MAC_CFG_RESET;

   RETVALUE(ROK);
} /* end of nhCGMUpdTransCbResetMacCfgMask */

/**
 * @brief This function fills and sends the request to PDCP for discReqd 
 *  info modification.
 *
 * @details
 *
 * This function sends request to PDCP to set discReqd flag to TRUE as 
 * security has been activated.
 *
 * @param[in] transCtrlBlk       Transaction context
 * @param[in] cellAndUeCb   Cell and UE control block
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhCGMPdcpDiscReqdCfgReq
(
TransCb       *transCtrlBlk,
NhCellAndUeCb *cellAndUeCb
)
#else /* ANSI */
PUBLIC S16 nhCGMPdcpDiscReqdCfgReq(transCtrlBlk, cellAndUeCb)
TransCb       *transCtrlBlk;
NhCellAndUeCb *cellAndUeCb;
#endif /* ANSI */
{
   S16 ret = ROK;

   TRC2(nhCGMPdcpDiscReqdCfgReq)

   RLOG0(L_DEBUG, "nhCGMPdcpDiscReqdCfgReq called");

   /* Request to be sent only to PDCP. */
   transCtrlBlk->cfgMask = NH_PDCP_CONFIGURED;


   /* Reset the security Map Table */
   transCtrlBlk->secCfgMapTbl[NH_INTG_CFG].status = NH_STATUS_UNUSED;
   transCtrlBlk->secCfgMapTbl[NH_CIPH_CFG].status = NH_STATUS_UNUSED;

   /* Copy the cfgMask in transCtrlBlk to cfmMask */
   transCtrlBlk->cfmMask = transCtrlBlk->cfgMask;

   /* Request for only SRB1 RB modification */
   transCtrlBlk->ordrdCfg.actionMask = NH_ORD_CFG_RB_MOD;
   transCtrlBlk->ordrdCfg.rbToModCfgList.numOfRbCfgInfo = 1;

   /* Alocate memory for the RbCfg -only for SRB1  */
   NH_ALLOC(NH_REG, NH_POOL,
            &transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList,
            1 * sizeof(NhuRbCfgInfo));

   if (NULLP == transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList)
   {
      RLOG0(L_FATAL, "Failed to allocate memory for Mod RB list.");
      RETVALUE(RFAILED);
   }

   /* Fill required RB info */
   transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList->rbId.rbId = 
                    NH_RB_ID_SRB1;
   transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList->rbId.rbTyp = CM_LTE_SRB;
   transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList->rbCfgBitMsk = 
                                                     NHU_RB_CFG_PDCP_INFO;

   /* Update PDCP RB map table */
   /* Resetting the PDCP bitmask as the request is being sent only to *
    * discReqd field modification.                                    */
   transCtrlBlk->ordrdCfg.rbToModCfgList.rbCfgInfoList->pdcpCfg.pdcpCfgBitMsk =
                                                     NH_ZERO_VAL;

   ret = nhLIMSendCfgMsg(cellAndUeCb, transCtrlBlk);
   if(ret != ROK)
   {
      RLOG0(L_ERROR, "LIM Send Cfg Faied");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of nhCGMPdcpDiscReqdCfgReq */

/**
 * @brief This function updates PDCP SDU status information in transCtrlBlk
 *        from ueCfg info given by the RRC user.
 *
 * @details
 *
 * This function  allocates memory for ulBitMap in NhuHoPdcpSduInfo
 * of transCtrlBlk and updates the ulBitMap from ueCfg info given by the 
 * RRC user.
 *
 * @param[in] transCtrlBlk        Transaction control block
 * @param[in] ueCfg          Explicit configuration
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PRIVATE S16 nhCGMUpdPdcpSduStaUeCbExpCfg
(
TransCb         *transCtrlBlk,
NhuUeCfgInfo    *ueCfg
)
#else /* ANSI */
PRIVATE S16 nhCGMUpdPdcpSduStaUeCbExpCfg(transCtrlBlk, ueCfg)
TransCb         *transCtrlBlk;
NhuUeCfgInfo    *ueCfg;
#endif /* ANSI */
{
   U8         noRbToCfg;
   U8         rbCnt;
   Size       ulBitMapSize;
   NhuHoPdcpSduInfo *hoPdcpSduStaInfo;
   U8         pdcpCfgBitMsk;

   TRC2(nhCGMUpdPdcpSduStaUeCbExpCfg)

   RLOG0(L_DEBUG, "Updating Explicit Configuration in Trans Control Block for"
      "HO PDCP SDU status info");

   noRbToCfg = transCtrlBlk->ordrdCfg.rbToModCfgList.numOfRbCfgInfo;

   /* Allocate and fill ulBitMap Info for each RB if bitmask is set for that */
   for(rbCnt = 0; rbCnt < noRbToCfg; rbCnt++)
   {
      pdcpCfgBitMsk = transCtrlBlk->ordrdCfg.rbToModCfgList.\
                      rbCfgInfoList[rbCnt].pdcpCfg.pdcpCfgBitMsk;
      if (pdcpCfgBitMsk & NHU_PDCP_SDU_STA_INFO)
      {
         hoPdcpSduStaInfo = &transCtrlBlk->ordrdCfg.rbToModCfgList.\
                            rbCfgInfoList[rbCnt].pdcpCfg.hoPdcpSduStaInfo;

         if( (NHU_HO_UL & hoPdcpSduStaInfo->isHoPres) &&
             (hoPdcpSduStaInfo->numBits > 0) )
         {
            /* Get the number of octets required to allocate memory for *
             * ulBitMap */
            NH_GET_UL_BITMAP_SIZE(hoPdcpSduStaInfo->numBits,ulBitMapSize);

            NH_ALLOC(nhCb.init.region, nhCb.init.pool,
                     &hoPdcpSduStaInfo->ulBitMap, ulBitMapSize);
            if(NULLP == hoPdcpSduStaInfo->ulBitMap)
            {
               RLOG0(L_FATAL, "Failed to allocate memory for ulBitMap in "
                  "ordered config");
               /* In case of failure, release the memory of ulBitMap allocated
                * previously for other RBs */
               nhUTLFreeUlBitMapFrmOrdCfg(transCtrlBlk, rbCnt);
               RETVALUE(RFAILED);
            }
            /* copy the ulBitMap to transCtrlBlk from ueCfg provided by RRC user */
            NH_MEM_COPY(hoPdcpSduStaInfo->ulBitMap,
                 ueCfg->rbCfgAllList.rbToModCfgList.rbCfgInfoList[rbCnt].\
                                        pdcpCfg.hoPdcpSduStaInfo.ulBitMap,
                 ulBitMapSize);
         }/* if( (NHU_HO_UL & hoPdcpSduStaInfo->isHoPres) ) */
      } /* if (pdcpCfgBitMsk & NHU_PDCP_SDU_STA_INFO) */
   }/* end of for loop of rbCnt */

   RETVALUE(ROK);
} /* end of nhCGMUpdPdcpSduStaUeCbExpCfg */

/** @} */
/**********************************************************************
  
         End of file:     ni_cgm.c@@/main/3 - Fri Jul  1 01:13:13 2011
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     rer              1. LTE-RRC Initial Release.
/main/2      ---     ch               1. RRC Release 2.1.
          nh002.201 vkulkarni         1. Correcting cellrecfg handling.
          nh004.201 vkulkarni   1. Correction for TRC functionality.
/main/3      ---     sbalakrishna     1. Updated for Release of 3.1.
/main/4   nh001.301  njha             1. Memory leaked fixed.
*********************************************************************91*/
