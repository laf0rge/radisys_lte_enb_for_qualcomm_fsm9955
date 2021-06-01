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

     Name:     LTE-RRC Lower Interface Portable module
  
     Type:     C Source file
  
     Desc:     RRC Portable Lower Interface (outgoing) Primitives. 

     Ret :     ROK - success
               RFAILED - failure

     File:     nh_pim.c

     Sid:      ni_pim.c@@/main/3 - Fri Jul  1 01:13:20 2011

     Prg:      dm

**********************************************************************/
/*
*     This software may be combined with the following TRILLIUM
*     software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*     1000369     LTE-RRC
*
*/
/** @ingroup lim_group 
 * @{
 */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=91;

#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_pasn.h"       /* Common Pasn defines */

#include "nhu.h"           /* NHU interface defines */
#include "lnh.h"           /* Layer Manager interface defines */
#include "kwu.h"           /* RLC data Interface defines */
#include "ckw.h"           /* RLC control interface defines */
#include "crg.h"           /* MAC control interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "ctf.h"           /* PHY interface defines */

#include "nh.h"            /* RRC defines */

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_pasn.x"       /* Common Pasn includes */
#include "cm_lib.x"        /* Common memory allocation library */

#include "kwu.x"           /* KWU Interface includes */
#include "ckw.x"           /* CKW Interface includes */
#include "nhu.x"           /* NHU Interface includes */
#include "lnh.x"           /* LNH Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "ctf.x"           /* PHY Interface includes */

#include "nh.x"            /* RRC includes */

PRIVATE S16 nhPIMFillRlcDatReqParams ARGS((
                                    KwuDatReqInfo   *kwDatReqParam,
                                    NhTmRlcDatReqParam *rlcParam));

PRIVATE S16 nhPIMFillRlcRvrtCfgParam ARGS(( 
                      CkwCfgInfo     *ckwCfgParam,
                      NhuRbCfgInfo   *rbCfgInfo,
                      U8             cfgType)); 

PRIVATE S16 nhPIMFillRlcCfgParam ARGS((
                        CkwCfgInfo     *ckwCfgParam,
                        NhuRbCfgList   *rbCfgListInfo,
                        U8             cfgType ));
PRIVATE S16 nhPIMFillPdcpRvrtCfgParam ARGS((
                       NhActvUeCb    *ueCb,
                       CpjCfgReqInfo *cpjCfgReq,
                       NhuRbCfgInfo  *rbCfgInfo,
                       U8            cfgType )); 
PRIVATE S16 nhPIMFillPdcpRvrtAddCfgParam ARGS((
                       CpjCfgReqInfo *cpjCfgReq,
                       NhuRbCfgInfo  *rbCfgInfo,
                       NhActvUeCb    *ueCb));
PRIVATE S16 nhPIMFillPdcpRvrtModCfgParam ARGS((
                       CpjCfgReqInfo *cpjCfgReq,
                       NhuRbCfgInfo  *rbCfgInfo,
                       NhActvUeCb    *ueCb));
PRIVATE S16 nhPIMFillAddPdcpCfgParam ARGS((
                       NhActvUeCb    *ueCb,
                       CpjCfgReqInfo *cpjCfgReq,
                       NhuRbCfgList  *rbCfgListInfo)); 
PRIVATE S16 nhPIMFillModPdcpCfgParam ARGS((
                       NhActvUeCb *ueCb,
                       CpjCfgReqInfo *cpjCfgReq,
                       NhuRbCfgList  *rbCfgListInfo)); 
PRIVATE S16 nhPIMFillPdcpSecCfg ARGS((CpjSecCfgReqInfo  *cpjSecConfigReqInfo,
                                  NhuSecurityCfg    *secCfg,
                                  U8                selSecMode));

PRIVATE S16 nhPIMFillRlcCellCfgParam ARGS((
                       CkwCfgInfo     *ckwCfgParam,
                       NhuRbCfgList   *rbCfgListInfo,
                       U8             cfgType ));

PRIVATE S16 nhPIMFillMacLchReCfgParam ARGS((
                       CrgLchRecfg     *crgLchReConfig,
                       NhuRbCfgInfo *rbCfgInfo,
                       U8 *lcId,
                       U8 *dir ));

PRIVATE S16 nhPIMFillPdcpCfgParam ARGS((
                        NhActvUeCb    *ueCb,
                        TransCb       *transCtrlBlk,
                        CpjCfgReqInfo *cpjCfgReq));

PRIVATE S16 nhPIMFillMacLchCfgParam ARGS((
                       CrgLchCfg     *crgLchConfig,
                       NhuRbCfgInfo  *rbCfgInfo,
                       U8            *lcId,
                       U8            *dir));

PRIVATE S16 nhPIMFillMacRvrtLchReCfgParam ARGS((
                       CrgLchRecfg     *crgLchReConfig,
                       NhLogChCfg     *logChInfo ));

PRIVATE S16 nhPIMFillMacRvrtLchRlsCfgParam ARGS((
                       CrgLchCfg     *crgLchConfig,
                       NhLogChCfg     *logChInfo));

PRIVATE S16 nhPIMFillMacUeCfgTrchParam ARGS((
                      CrgUeCfg      *crgUeConfig,  
                      NhuTrChCfg      *trChCfg));

PRIVATE S16 nhPIMFillMacCellCfgParam ARGS((
                     CrgCellCfg    *crgCellConfig,
                     NhuCellCfgInfo  *cellCfgInfo));

PRIVATE S16 nhPIMFillMacRvrtCellReCfgParam ARGS((
                     CrgCellRecfg      *crgCellReConfig,
                     NhCellCfgInfo  *cellCfgInfo));

PRIVATE S16 nhPIMFillMacCellReCfgParam ARGS((
                     CrgCellRecfg      *crgCellReConfig,
                     NhuCellCfgInfo  *cellCfgInfo));


PRIVATE S16 nhPIMFillRvrtMacModUeCfg ARGS((
CrgCfgReqInfo  *crgCfgParam,
NhActvUeCb *ueCb,
TransCb *transCtrlBlk));

PRIVATE S16 nhPIMRvrtMacModLchCfg ARGS((
NhActvUeCb *ueCb,
TransCb *transCtrlBlk,
NhLSapCb *lSap));

PRIVATE S16 nhPIMRvrtMacModRXLch ARGS((
NhActvUeCb *ueCb,
NhLSapCb *lSap,
U8 lcId)); 

PRIVATE NhRbToLchMap* nhPIMGetRvrtMacLch ARGS((
NhActvUeCb *ueCb,
U8  dir,
U8 lcId)); 

PRIVATE S16 nhPIMFillRvrtRlcAddParam ARGS((
CkwCfgInfo  *ckwCfgParam,
TransCb *transCtrlBlk));


PRIVATE S16 nhPIMFillRvrtRlcModParam ARGS((
CkwCfgInfo  *ckwCfgParam,
NhActvUeCb *ueCb,
TransCb *transCtrlBlk));

PRIVATE S16 nhPIMFillRvrtRlcRlsParam ARGS((
CkwCfgInfo  *ckwCfgParam,
NhActvUeCb *ueCb,
TransCb *transCtrlBlk));


PRIVATE NhuRbCfgInfo* nhPIMGetRvrtRlcRb ARGS((
NhActvUeCb *ueCb,
NhuRbId  rbId));


PRIVATE S16 nhPIMRvrtMacRlsLchCfg ARGS((
NhActvUeCb *ueCb,
TransCb *transCtrlBlk,
NhLSapCb *lSap));


PRIVATE S16 nhPIMRvrtMacRlsTXRXLch ARGS((
NhActvUeCb *ueCb,
NhLSapCb *lSap,
U8 lcId));


PRIVATE S16 nhPIMRvrtMacRlsRXLch ARGS((
NhActvUeCb *ueCb,
NhLSapCb *lSap,
U8 lcId));


PRIVATE S16 nhPIMRvrtMacRlsTXLch ARGS((
NhActvUeCb *ueCb,
NhLSapCb *lSap,
U8 lcId));


PRIVATE S16 nhPIMSendRvrtMacRlsLchCfg ARGS((
NhActvUeCb *ueCb,
NhRbToLchMap  *crntLchCfg,
NhLSapCb *lSap));


PRIVATE S16 nhPIMSendRvrtMacModLchCfg ARGS((
NhActvUeCb *ueCb,
NhRbToLchMap  *crntLchCfg,
NhLSapCb *lSap));

#ifdef RRC_PHY_CFG_SUPP
PRIVATE S16 nhPIMFillRvrtPhyModUeCfg ARGS((
CtfCfgReqInfo  *ctfCfgParam,
NhActvUeCb *ueCb,
TransCb *transCtrlBlk
));
#endif /* RRC_PHY_CFG_SUPP */
/*********************/

/* Dir_Fix: Added direction value as RLC works depending on Direction
 *         Provided by RRC
 */
PRIVATE S16 nhPIMFillRlcRlsCfg ARGS((CmLListCp *rbCfgInfoList,
                                      TransCb        *transCtrlBlk,
                                      CkwCfgInfo     *ckwCfgParam));
PRIVATE S16 nhPIMFillPdcpRlsCfg ARGS((NhActvUeCb    *ueCb,
                                      TransCb       *transCtrlBlk,
                                      CpjCfgReqInfo *cpjCfgReq,
                                      U8            *rbCount));
PRIVATE S16 nhPIMRvrtPdcpUeIdChgReq ARGS((NhLSapCb   *lSap,
                                          NhActvUeCb *ueCb,
                                          TransCb    *transCtrlBlk));
PRIVATE S16 nhPIMRvrtPdcpAddCfg ARGS((CpjCfgReqInfo   *cpjCfgReq,
                                      NhActvUeCb *ueCb,
                                      TransCb    *transCtrlBlk));
PRIVATE S16 nhPIMRvrtPdcpModCfg ARGS((CpjCfgReqInfo   *cpjCfgReq,
                                      NhActvUeCb *ueCb,
                                      TransCb    *transCtrlBlk));
PRIVATE S16 nhPIMRvrtPdcpRlsCfg ARGS((CpjCfgReqInfo   *cpjCfgReq,
                                      NhActvUeCb *ueCb,
                                      TransCb    *transCtrlBlk));

PRIVATE S16 nhPIMFillRlcRestbCfg ARGS((CmLListCp *rbCfgInfoList,
                                       CkwCfgInfo *ckwCfgParam,
                                       Bool skipSrb));

PRIVATE S16 nhPIMFillPdcpRestbSRB1Cfg ARGS((CpjReEstReqInfo  *cpjCfgParam,
                                            NhuSecurityCfg   *secCfg));
PRIVATE S16 nhPIMUpdPdcpSduStaRlcMapTbl ARGS((TransCb        *transCtrlBlk,
                                             CmLListCp      *rbCfgInfoList));

PRIVATE S16 nhPIMFillPdcpRestbCfg ARGS((TransCb *transCtrlBlk,
                                      CpjCfgReqInfo *cpjCfgReq,
                                      U8            *rbCount));

PRIVATE S16 nhPIMFillRlcDlCfgParam ARGS((CkwCfgInfo *ckwCfgParam,
                                       NhuRbCfgInfo   *rbCfgInfo,
                                       U8              rbCount));

PRIVATE S16 nhPIMFillRlcUlCfgParam ARGS((CkwCfgInfo     *ckwCfgParam,
                                       NhuRbCfgInfo   *rbCfgInfo,
                                       U8              rbCount));

#ifdef RRC_PHY_CFG_SUPP
PRIVATE S16 nhPIMFillPhyCfgParam ARGS((CtfDedCfgInfo  *ueCfgParam,
                                     NhuPhyCfg     *uePhyCfg));

PRIVATE S16 nhPIMFillPhyReCfgParam ARGS((CtfDedRecfgInfo  *ueReCfgParam,
                                       NhuPhyCfg     *uePhyCfg));

PRIVATE S16 nhPIMFillPhyCellReCfgParam ARGS((CtfCellRecfgInfo  *ctfCellRecfg,
                                           NhuCmnPhyCfg      *cellCfgInfo));

PRIVATE S16 nhPIMFillPhyCellCfgParam ARGS((CtfCellCfgInfo  *ctfCellCfg,
                                         NhuCellCfgInfo  *cellCfgInfo));
#endif

PRIVATE S16 nhPIMSendMacCellCfgAdd ARGS(( NhCellCb *cellCb));


/**
 *  @brief Handler to send encoded message to the RLC layer.
 *
 *  @details 
 *  This function is used to send encoded message 
 *  to the RLC and the PDCP layers.
 *            
 *  @param[in] cellAndUeCb    Cell and UE Control block
 *  @param[in] nhDataReqParam  This is a pointer to Data Request param.
 *  @param[in] mBuf           This is a pointer to message buffer.
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendUeMsg
(
NhCellAndUeCb  *cellAndUeCb,
NhDatReqParam  *nhDataReqParam, /* RLC or PDCP Data Request parameters */
Buffer         *mBuf           /* Encoded buffer */
)
#else /* ANSI */
PUBLIC S16 nhPIMSendUeMsg(cellAndUeCb, nhDataReqParam, mBuf)
NhCellAndUeCb  *cellAndUeCb;
NhDatReqParam  *nhDataReqParam; /* RLC or PDCP Data Request parameters */
Buffer         *mBuf;          /* Encoded buffer */
#endif /* ANSI */
{
   NhId           *nhIdentity;                      /* Cell ID or UE ID */
   KwuDatReqInfo  *kwuDatReqParam = NULLP;    /* RLC Data Request parameter */
   CmLtePdcpId    pdcpId;                     /* PDCP ID */
   NhLSapCb       *lSap = NULLP;              /* Lower SAP control block */
   Pst            tmpPst;
   SuId           suId = NH_ZERO_VAL;

   TRC2(nhPIMSendUeMsg)

   RLOG0(L_DEBUG, "nhPIMSendUeMsg");
   
   nhIdentity = &(nhDataReqParam->nhId);

   if (nhDataReqParam->isPdcpDatReq)
   {
#ifdef NHU_MULT_SAP_SUPPRT
      NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, cellAndUeCb->cellCb, STNHPJUSAP);
#endif /* NHU_MULT_SAP_SUPPRT */       
      
      /* get lower SAP */
      lSap = nhUTLFindLlyrSapFrmSuId(STNHPJUSAP, suId);
      if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
      {
         RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
         NH_FREEMBUF(mBuf);
         RETVALUE(RFAILED);
      }

      tmpPst = lSap->pst;
/*
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
*/
      pdcpId.rbId = nhIdentity->rbId;
      pdcpId.ueId = nhIdentity->ueId;
      pdcpId.cellId = nhIdentity->cellId;
      pdcpId.rbType = CM_LTE_SRB;
/*
#else
 As part of the Common Code Merge commenting this part.This was earlier part of the T3K code base.
   Need to be removed once it is tested with common code base

      pdcpId.rbId = nhIdentity->rbId;
      pdcpId.ueId = nhIdentity->ueId;
      pdcpId.cellId = nhIdentity->cellId;
      pdcpId.rbType = CM_LTE_SRB;

#endif
*/
#ifdef NH_TRACE_ENABLED
   NH_GEN_TRC(suId, STNHPJUSAP, LNH_MSG_SENT, mBuf)
#endif /* NH_TRACE_ENABLED */
 /*
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
*/
      if(ROK != NhLiPjuDatReq(&tmpPst, lSap->spId, &pdcpId,
                nhDataReqParam->u.pdcpParam.mui, mBuf))
/*
#else     
 As part of the Common Code Merge commenting this part.This was earlier part of the T3K code base.
   Need to be removed once it is tested with common code base
      if(ROK != NhLiPjuDatReq(&tmpPst, lSap->spId, &pdcpId,
                nhDataReqParam->u.pdcpParam.mui, mBuf))
#endif
*/
      {
         RLOG0(L_ERROR,"Failed to send data to lower layer");
         RETVALUE(RFAILED);
      }
   }
   else
   {
#ifdef NHU_MULT_SAP_SUPPRT
      NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, cellAndUeCb->cellCb, STNHKWUSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       
      /* Get lower SAP */
      lSap = nhUTLFindLlyrSapFrmSuId(STNHKWUSAP,suId);
      if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
      {
         RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
         NH_FREEMBUF(mBuf);
         RETVALUE(RFAILED);
      }

      tmpPst = lSap->pst;

      NH_ALLOC_SHARABLE_BUF(NH_REG, NH_POOL, &kwuDatReqParam, sizeof(KwuDatReqInfo));
      if (kwuDatReqParam == NULLP)
      {
         RLOG0(L_FATAL, "Could not allocate mem for kwuDatReqParam");
         NH_FREEMBUF(mBuf);
         RETVALUE(RFAILED);
      }
      kwuDatReqParam->rlcId.rbId = nhIdentity->rbId;
      kwuDatReqParam->rlcId.ueId = nhIdentity->ueId;
      kwuDatReqParam->rlcId.cellId = nhIdentity->cellId;

      nhPIMFillRlcDatReqParams(kwuDatReqParam, &nhDataReqParam->u.rlcParam);
#ifdef NH_TRACE_ENABLED
   NH_GEN_TRC(suId, STNHKWUSAP, LNH_MSG_SENT, mBuf)
#endif /* NH_TRACE_ENABLED */
      
      if (ROK != NhLiKwuDatReq(&tmpPst, lSap->spId, kwuDatReqParam, mBuf))
      {
         RLOG0(L_ERROR,"Failed to send data to lower layer");
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}/* End of nhPIMSendUeMsg */


/**
 *  @brief Handler to send configuration to RLC for specified RB ID.
 *
 *  @details 
 *  This function is used to send configuration request   
 *  to RLC layer. It checks the action Mask and fills all types (ADD/MOD/DELETE)
 *  of configuration parameter and sends in one request.
 *            
 *  @param[in] ueCb     This is a pointer to UE Cb.
 *  @param[in] transCtrlBlk  This is a pointer to transaction Cb.
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendRlcCfg
(
NhActvUeCb   *ueCb,
TransCb      *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhPIMSendRlcCfg(ueCb, transCtrlBlk)
NhActvUeCb   *ueCb;
TransCb      *transCtrlBlk;
#endif /* ANSI */
{
   CkwCfgInfo     *ckwCfgParam = NULLP;
   Pst            tmpPst;
   NhLSapCb       *lSap = NULLP;    /* Lower SAP Cb */
   SuId           suId = NH_ZERO_VAL;
   NhuRbCfgList   *rbCfgListInfo = NULLP;   
   CkwUeInfo       *oldUeInfo = NULLP;
   CkwUeInfo       *newUeInfo = NULLP;
   U16             ueId = NH_ZERO_VAL;     /* UeId */
   
   TRC2(nhPIMSendRlcCfg)
   RLOG0(L_DEBUG, "nhPIMSendRlcCfg");
   
   /* Get lower SAP*/
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCKWSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCKWSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;


   if ((NH_STATUS_USED == 
                 transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_RLC].status) &&
       (NH_RLC_CFG_UEID_CHNG == 
                 transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_RLC].action))
   {
      NH_ALLOC(NH_REG, NH_POOL, &oldUeInfo, sizeof(CkwUeInfo));
      if(NULLP == oldUeInfo)
      {
         RLOG0(L_FATAL, "Could not allocate mem for Ordrd config");
         RETVALUE(RFAILED);
      }
      NH_ALLOC(NH_REG, NH_POOL, &newUeInfo, sizeof(CkwUeInfo));
      if(NULLP == newUeInfo)
      {
         RLOG0(L_FATAL, "Could not allocate mem for Ordrd config");
         NH_FREE(NH_REG, NH_POOL, oldUeInfo, sizeof(CkwUeInfo));
         RETVALUE(RFAILED);
      }
               
      oldUeInfo->ueId = ueCb->ueCbKey.crnti;
      oldUeInfo->cellId = ueCb->cellCb->cellId;

      newUeInfo->ueId = transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;
      newUeInfo->cellId = ueCb->cellCb->cellId;
      ueId = transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;

      transCtrlBlk->numOfRlcCfms++;
      if (ROK != NhLiCkwUeIdChgReq(&tmpPst, lSap->spId, transCtrlBlk->nxTransId,
                                                     oldUeInfo, newUeInfo))
      {
         transCtrlBlk->numOfRlcCfms--;
         RLOG0(L_ERROR,"Failed to send UEID Change Req to RLC");
         RETVALUE(RFAILED);
      }
      RETVALUE(ROK);
   }
   else
   {
      ueId = ueCb->ueCbKey.crnti;
   }
                
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS ||
       transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_MOD ||
       transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_ADD ||
       transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RESTB || 
       transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RESTB_SRB1 ||
       transCtrlBlk->cause == NH_TRANSCB_SDU_STA_RESTAB)
   {
      /* allocate memory for cfg Parameter */
      NH_ALLOC(NH_REG, NH_POOL, &ckwCfgParam, sizeof(CkwCfgInfo));
      if (NH_CHK_INPUT_PARAM(ckwCfgParam))
      {
         RLOG0(L_FATAL, "GOT NULL ckwCfgParam");
         RETVALUE(RFAILED);
      }

      ckwCfgParam->transId = transCtrlBlk->nxTransId;
      ckwCfgParam->ueId = ueId;
      ckwCfgParam->cellId = ueCb->cellCb->cellId;
   }
   else
   {
      RLOG0(L_DEBUG,"No Rbs to Configure ");
      RETVALUE(ROK);
   }
  
   /* Action add */
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_ADD)
   {
      
      rbCfgListInfo = &(transCtrlBlk->ordrdCfg.rbToAddCfgList);   
      /* Fill RLC Config parameter to CKW Cfg structure */
      if (ROK != nhPIMFillRlcCfgParam(ckwCfgParam, rbCfgListInfo, 
                             CKW_CFG_ADD))
      {
         RLOG0(L_ERROR, "Failed to fill data to lower interface structure");
         NH_FREE(NH_REG, NH_POOL, ckwCfgParam, sizeof(CkwCfgInfo));
         RETVALUE(RFAILED);
      }

   }
   /* Action modify */
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_MOD)
   {
      
      rbCfgListInfo = &(transCtrlBlk->ordrdCfg.rbToModCfgList);   
      /* Fill RLC Config parameter to CKW Cfg structure */
      if (ROK != nhPIMFillRlcCfgParam(ckwCfgParam, rbCfgListInfo,
                      CKW_CFG_MODIFY))
      {
         RLOG0(L_ERROR, "Failed to fill data to lower interface structure");
         NH_FREE(NH_REG, NH_POOL, ckwCfgParam, sizeof(CkwCfgInfo));
         RETVALUE(RFAILED);
      }

   }
   /* Action release */
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS)
   {
      /* Dir_Fix: Added direction value as RLC works depending on Direction
       *         Provided by RRC
       */
      nhPIMFillRlcRlsCfg(&ueCb->crntCfg.rbCfgList.rbCfgInfoList, transCtrlBlk,
            ckwCfgParam);
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS) */

  /* Action re-establish */
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RESTB ||
       transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RESTB_SRB1)
   {
      nhPIMFillRlcRestbCfg(
                 &transCtrlBlk->ordrdCfg.rbToRestbCfgList.rbCfgInfoList, 
                 ckwCfgParam, FALSE );
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS) */

   /* Re-establish in case of PDCP SDU STATUS */
   if (transCtrlBlk->cause == NH_TRANSCB_SDU_STA_RESTAB)
   {
      /* Fill the rbsToRestab and restbCfmStatus in rlcMapTbl od transCtrlBlk. *
       * This reqd to validate when confirm is received.                  */
      nhPIMUpdPdcpSduStaRlcMapTbl(transCtrlBlk, 
                                  &ueCb->crntCfg.rbCfgList.rbCfgInfoList);

      /* Filling rbIds and rbType in ckwCfgParam structure */ 
      nhPIMFillRlcRestbCfg(&ueCb->crntCfg.rbCfgList.rbCfgInfoList, 
                           ckwCfgParam, TRUE );
   }

   if(ckwCfgParam->numEnt > 0)
   {
      transCtrlBlk->numOfRlcCfms++;
      /* Send Configuration to RLC layer */
      if (ROK != NhLiCkwCfgReq(&tmpPst, lSap->spId, ckwCfgParam))
      {
         transCtrlBlk->numOfRlcCfms--;
         RLOG0(L_ERROR,"Failed to send data to lower layer");
         RETVALUE(RFAILED);
      }
   }
   
   RETVALUE(ROK);

} /* nhPIMSendRlcCfg */

PUBLIC S16 nhPIMRlcSndRestbCfg(TransCb *transCtrlBlk, NhActvUeCb *ueCb)
{
   CkwCfgInfo     *ckwCfgParam = NULLP;
   Pst            tmpPst;
   SuId           suId = NH_ZERO_VAL;
   NhLSapCb       *lSap = NULLP;    /* Lower SAP Cb */

      /* allocate memory for cfg Parameter */
   NH_ALLOC(NH_REG, NH_POOL, &ckwCfgParam, sizeof(CkwCfgInfo));
   if (NH_CHK_INPUT_PARAM(ckwCfgParam))
   {
     RLOG0(L_FATAL, " GOT NULL ckwCfgParam ");
     RETVALUE(RFAILED);
   }

   ckwCfgParam->transId = transCtrlBlk->nxTransId;
   ckwCfgParam->ueId =  transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;
   ckwCfgParam->cellId = ueCb->cellCb->cellId;
   nhPIMFillRlcRestbCfg(
                 &transCtrlBlk->ordrdCfg.rbToRestbCfgList.rbCfgInfoList, 
                 ckwCfgParam, FALSE );
   if(ckwCfgParam->numEnt > 0)
   {
     /* Get lower SAP*/
#ifdef NHU_MULT_SAP_SUPPRT
     NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCKWSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

     lSap = nhUTLFindLlyrSapFrmSuId(STNHCKWSAP,suId);
     if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
     {
       RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound ");
       RETVALUE(RFAILED);
     }

     tmpPst = lSap->pst;
     transCtrlBlk->numOfRlcCfms++;
     /* Send Configuration to RLC layer */
     if (ROK != NhLiCkwCfgReq(&tmpPst, lSap->spId, ckwCfgParam))
     {
       transCtrlBlk->numOfRlcCfms--;
       RLOG0(L_ERROR, " Failed to send data to lower layer ");
       RETVALUE(RFAILED);
     }
   }
   RETVALUE(ROK);
}

/**
 *
 *   @brief Handler to send Cell Configuration to RLC.
 *
 *   @details
 *   This function is used to send cell configuration to RLC layer.
 *            
 *  @param[in] cellCb  Cell control block.
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendRlcCellCfg
(
NhCellCb       *cellCb        /* Cell control block */
)
#else /* ANSI */
PUBLIC S16 nhPIMSendRlcCellCfg(cellCb)
NhCellCb       *cellCb;        /* Cell control block */
#endif /* ANSI */
{
   CkwCfgInfo      *ckwCfgParam = NULLP;   /* CKW Config parameter */
   Pst             tmpPst;                 /* Pst structure */
   NhLSapCb        *lSap = NULLP;          /* Lower SAP Cb */
   SuId            suId = NH_ZERO_VAL;     /* RRC lower SAP ID */
  
   NhCellOrdrdCfg  *cellCfgInfo = NULLP;

   TRC2(rrPIMSendRlcCellCfg)
   
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, cellCb, STNHCKWSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCKWSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
       
   }

   /* Allocate memory for cfg parameter */
   NH_ALLOC(NH_REG, NH_POOL, &ckwCfgParam, sizeof(CkwCfgInfo));
   if (NH_CHK_INPUT_PARAM(ckwCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL ckwCfgParam");
      RETVALUE(RFAILED);
   }

   ckwCfgParam->transId = cellCb->cellTransCb.nxTransId;
   
   ckwCfgParam->cellId = cellCb->cellId;
  
   /* Get ordered cell config */
   cellCfgInfo = cellCb->cellTransCb.cellOrdrdCfg;

   /* Check for config action */
   switch(cellCb->cellTransCb.cfgAction)
   {
      case NH_CFG_ACTN_ADD: 
         if (cellCfgInfo->cellCfgInfo.cellCfgBitMsk & NHU_CELL_CFG_RB)
         {
            /* Fill cell config parameter */
            if (ROK != nhPIMFillRlcCellCfgParam(ckwCfgParam,
                    &cellCfgInfo->cellCfgInfo.rbToAddCfgList, CKW_CFG_ADD))
            {
               RLOG0(L_ERROR,"Failed to fill data for Cell");
               NH_FREE(NH_REG, NH_POOL, ckwCfgParam, sizeof(CkwCfgInfo));
               RETVALUE(RFAILED);
            }
            cellCb->cellTransCb.numOfRlcCfms += ckwCfgParam->numEnt;
         }
         break;

      case NH_CFG_ACTN_RLS:
         ckwCfgParam->numEnt = 1; 
         ckwCfgParam->entCfg[0].cfgType = CKW_CFG_DELETE_CELL;
         cellCb->cellTransCb.numOfRlcCfms += ckwCfgParam->numEnt;
         break;

      default:
         RLOG0(L_ERROR,"No Action in Ordered config for cell");
         NH_FREE(NH_REG, NH_POOL, ckwCfgParam, sizeof(CkwCfgInfo));
         RETVALUE(RFAILED);
   }
  
   tmpPst = lSap->pst;

   /* Send Configuration to RLC layer */
   if (ROK != NhLiCkwCfgReq(&tmpPst, lSap->spId, ckwCfgParam))
   {
      RLOG0(L_ERROR,"Failed to send data to lower layer");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* end of nhPIMSendRlcCellCfg */


/**
 *
 *   @brief Handler to revert successful configuration at RLC.
 *
 *   @details
 *   This function is used to revert successful
 *   cell configuration at RLC layer.
 *            
 *
 *   @param[in] cellCb  Cell control block
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRvrtRlcCellCfg
(
NhCellCb *cellCb                         /* Cell control block */
)
#else /* ANSI */
PUBLIC S16 nhPIMRvrtRlcCellCfg(cellCb)
NhCellCb *cellCb;                        /* Cell control block */
#endif /* ANSI */
{
   NhCellOrdrdCfg  *cellOrdrdCfg;        /* Ordered Configuration */
   NhCellTransCb   *cellTransCb;
   CkwCfgInfo      *ckwCfgParam = NULLP; /* CKW configuration parameter */
   Pst             tmpPst;               /* Pst structure */ 
   U8              rvrtAction = 0x00;    /* Revert action */
   NhLSapCb        *lSap = NULLP;        /* Lower SAP Cb */
   
   TRC2(nhPIMRvrtRlcCellCfg)
   RLOG0(L_DEBUG, "nhPIMRvrtRlcCellCfg");

   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   SuId            suId = NH_ZERO_VAL;   /* suId */
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, cellCb, STNHCKWSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
       
   }

   tmpPst = lSap->pst;
   /* Allocate memory for cfg Parameter */
   NH_ALLOC(NH_REG, NH_POOL, &ckwCfgParam, sizeof(CkwCfgInfo));
   if (NH_CHK_INPUT_PARAM(ckwCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL ckwCfgParam");
      RETVALUE(RFAILED);
   }

   /* Fill transaction ID */
   ckwCfgParam->transId = cellCb->cellTransCb.nxTransId;
   /* Fill Cell ID */
   ckwCfgParam->cellId = cellCb->cellId;
 
  /* Get Cell Trans Cb */ 
   cellTransCb = &(cellCb->cellTransCb);
      if (cellTransCb->cfgAction == NH_CFG_ACTN_ADD)
          rvrtAction = NH_CFG_ACTN_RLS;

   cellOrdrdCfg = cellTransCb->cellOrdrdCfg;
   if(rvrtAction == NH_CFG_ACTN_RLS)
   {
      ckwCfgParam->numEnt = 1; 
      ckwCfgParam->entCfg[0].cfgType = CKW_CFG_DELETE_CELL;
   }

   /* Send Configuration to RLC layer */
   if (ROK != NhLiCkwCfgReq(&tmpPst, lSap->spId, ckwCfgParam))
   {
      RLOG0(L_ERROR,"Failed to send data to lower layer");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* end of nhPIMRvrtRlcCellCfg */


/**
 *
 *   @brief Handler to modify configuration at MAC.
 *
 *   @details
 *   This function is used to modify configuration   
 *   at MAC.
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Transaction control block
 *   @param[in] cfgType   Configuration type
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendMacModCfg
(
NhActvUeCb *ueCb,        /* UE control block */
TransCb *transCtrlBlk,   /* Transaction control block */
U8 cfgType               /* Config type */
)
#else /* ANSI */
PUBLIC S16 nhPIMSendMacModCfg(ueCb, transCtrlBlk, cfgType)
NhActvUeCb *ueCb;        /* Ue control block */
TransCb *transCtrlBlk;        /* Trans Cb */
U8 cfgType;              /* Configuration type */
#endif /* ANSI */
{
   CrgCfgReqInfo     *crgCfgParam = NULLP;   /* MAC Configuration Request parameter */
   CrgCfgTransId     crgCfgTransId;          /* MAC Transaction ID */
   Pst               tmpPst;                 /* Post structure */ 
   NhLSapCb          *lSap = NULLP;          /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL;     /* suId */
   NhuRbCfgList      *rbCfgList = NULLP;     /* RB Configuration List */
   U8                i = NH_ZERO_VAL;        /* Iterator for FOR loop */ 
   NhuRbCfgInfo      *rbCfgInfo = NULLP;     /* Configuration info */
   U16               ueId = NH_ZERO_VAL;     /* UeId */
   U8                lcId = NH_ZERO_VAL;     /* Logical ID */
   U8                dir = NH_ZERO_VAL;      /* Direction UL/DL */

   TRC2(nhPIMSendMacModCfg)
   RLOG0(L_DEBUG, "nhPIMSendMacModCfg");
  
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCRGSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
       
   }

   tmpPst = lSap->pst;
   /* Reterive UE ID */ 
   if(transCtrlBlk->ordrdCfg.ueInfo.crntiCfgType == NH_UE_INFO_MOD_CRNTI)
   {
      ueId = transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;
   }
   else
   {
      ueId = ueCb->ueCbKey.crnti;
   }
   /* Reconfiguration to MAC */
   if (cfgType & NH_CRG_UE_CFG)
   {
      /* Allocate memory for cfg Parameter */
      NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
      if (NH_CHK_INPUT_PARAM(crgCfgParam))
      {
         RLOG0(L_FATAL, "GOT NULL crgCfgParam");
         RETVALUE(RFAILED);
      }
      /* Set zero */
      NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
      /* Re-configuration */
      crgCfgParam->action = CRG_RECONFIG;
      crgCfgParam->u.recfgInfo.recfgType = CRG_UE_CFG;
      crgCfgParam->u.recfgInfo.u.ueRecfg.cellId = ueCb->cellCb->cellId;
      crgCfgParam->u.recfgInfo.u.ueRecfg.oldCrnti = ueCb->ueCbKey.crnti;
      crgCfgParam->u.recfgInfo.u.ueRecfg.newCrnti = ueId;
     
      if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_TRCH )
      {
         /* Fill Trch parameter */
         crgCfgParam->u.recfgInfo.u.ueRecfg.ueUlHqRecfg.maxUlHqTx =
              transCtrlBlk->ordrdCfg.trChCfg.ulHarqCfg.ulMaxHarqTx;
      }
      /* Construct transaction ID */
      NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, ueCb->cellCb->cellId,
         ueCb->ueCbKey.crnti, transCtrlBlk->nxTransId, NH_CRG_UE_CFG, 
         NH_CFG_ACTN_MODIFY, NH_ZERO_VAL, NH_ZERO_VAL);

      transCtrlBlk->numOfMacCfms++;
      /* Send Configuration to MAC layer */
      if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, crgCfgParam))
      {
         transCtrlBlk->numOfMacCfms--;
         RLOG0(L_ERROR,"Failed to send data to lower layer");
         RETVALUE(RFAILED);
      }
   }
   /* Logical channel configuration */
   if (cfgType & NH_CRG_LOGCH_MAP)
   {
      rbCfgList = &transCtrlBlk->ordrdCfg.rbToModCfgList;
      /* traverse all rb config list */
      for (i = 0; i < rbCfgList->numOfRbCfgInfo; i++)
      {
         /* Allocate memory for cfg parameter */
         NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
         if (NH_CHK_INPUT_PARAM(crgCfgParam))
         {
            RLOG0(L_FATAL, "GOT NULL crgCfgParam");
            RETVALUE(RFAILED);
         }

         /* Initialize transaction ID to zero */
         NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
      
         /* Re-configuration */
         crgCfgParam->action = CRG_RECONFIG;
         crgCfgParam->u.recfgInfo.recfgType = CRG_LCH_CFG;
         crgCfgParam->u.recfgInfo.u.lchRecfg.cellId = ueCb->cellCb->cellId;
         crgCfgParam->u.recfgInfo.u.lchRecfg.crnti = ueId;
         
         /* Get rbConfig Info */
         rbCfgInfo = &rbCfgList->rbCfgInfoList[i];
        
         /* Fill Mac logical channel parameter */
         nhPIMFillMacLchReCfgParam(&crgCfgParam->u.recfgInfo.u.lchRecfg,
               rbCfgInfo, &lcId, &dir);

         if (dir)
         {
            /* Construct transaction id */
            NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, 
                  ueCb->cellCb->cellId, ueCb->ueCbKey.crnti, 
                  transCtrlBlk->nxTransId, NH_CRG_LOGCH_MAP, 
                  NH_CFG_ACTN_MODIFY,lcId, dir);

            transCtrlBlk->numOfMacCfms++;
            /* Send Configuration to MAC layer */
            if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, 
                                     crgCfgParam))
            {
               transCtrlBlk->numOfMacCfms--;
               RLOG0(L_ERROR, "Failed to send data to lower layer");
               RETVALUE(RFAILED);
            }
         }
         else
         {
            /* free the allocated memory */
            NH_FREE(NH_REG, NH_POOL, crgCfgParam, sizeof(CrgCfgReqInfo));
         }

         lcId = 0;
         dir = 0;
      }
   }

   RETVALUE(ROK);
}  /* end of nhPIMSendMacModCfg */


/**
 *
 *   @brief Handler to release specific configuration at MAC.
 *
 *   @details
 *   This function is used to release specific   
 *   configuration at MAC.
 *            
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Transaction control block
 *   @param[in] cfgType   Configuration Type
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendMacRlsCfg
(
NhActvUeCb *ueCb,       /* UE Cb */
TransCb *transCtrlBlk,       /* Transaction Cb */
U8 cfgType              /* configuration type */
)
#else /* ANSI */
PUBLIC S16 nhPIMSendMacRlsCfg(ueCb, transCtrlBlk, cfgType)
NhActvUeCb *ueCb;       /* UE Cb */
TransCb *transCtrlBlk;       /* Transaction Cb */ 
U8 cfgType;             /* configuration type */
#endif /* ANSI */
{
   NhRbToLchMap          *crntLchCfg = NULLP;
   CrgCfgReqInfo         *crgCfgParam = NULLP;   /* MAC Configuration Request parameter */
   CrgCfgTransId         crgCfgTransId;          /* MAC Transaction ID */
   Pst                   tmpPst;                 /* Post structure */ 
   NhLSapCb              *lSap = NULLP;          /* Lower SAP Cb */
   SuId                  suId = NH_ZERO_VAL;     /* Service user ID */ 
   NhuRbRlsCfgList       *rbToRlsCfgList;        /* RB release list */
   NhuRbId               *rbIdPtr = NULLP;       /* Pointer to RB ID */
   U8                    i = NH_ZERO_VAL;        /* Iterator for FOR loop */
   CmLList               *tmpNode = NULLP;       /* Temp node */
   U8                    found = FALSE;          /* Found flag */

   TRC2(nhPIMSendMacRlsCfg)

   RLOG0(L_DEBUG,"Rlsing Cfg at MAC  ");

   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCRGSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;

   /* logical channel delete */
   if (cfgType & NH_CRG_LOGCH_MAP)
   {
      rbToRlsCfgList = &(transCtrlBlk->ordrdCfg.rbToRlsCfgList); 
      for (i = 0; i < rbToRlsCfgList->numOfRbCfgInfo; i++)
      {
         /* Allocate memory for cfg parameter */
         NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
         if (NH_CHK_INPUT_PARAM(crgCfgParam))
         {
            RLOG0(L_FATAL, "GOT NULL crgCfgParam");
            RETVALUE(RFAILED);
         }
         /* Initialize to zero */
         NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
        
         /* Delete configuration */    
         crgCfgParam->action = CRG_DELETE;
         crgCfgParam->u.delInfo.delType = CRG_LCH_CFG;
         crgCfgParam->u.delInfo.u.lchDel.cellId = ueCb->cellCb->cellId;
         crgCfgParam->u.delInfo.u.lchDel.crnti = ueCb->ueCbKey.crnti;
      
         found = FALSE;

         /* Get rbId from release cfg list*/
         rbIdPtr = &rbToRlsCfgList->rbsToRel[i];
       
         /* Find logical channel ID */
         CM_LLIST_FIRST_NODE(&(ueCb->crntCfg.logChCbLst), tmpNode);
         while(NULLP != tmpNode)
         {
            crntLchCfg = (NhRbToLchMap *)(tmpNode->node);
            /* Check for rbId */
            if( (*rbIdPtr).rbId == crntLchCfg->rbId.rbId &&
                (*rbIdPtr).rbTyp == crntLchCfg->rbId.rbTyp)
            {
               found = TRUE;
               break;
            }
            CM_LLIST_NEXT_NODE(&ueCb->crntCfg.logChCbLst, tmpNode);
         }
          
         if(found)
         {

            /* Fill found lclId */
            crgCfgParam->u.delInfo.u.lchDel.lcId = crntLchCfg->lchId.lcId;
            crgCfgParam->u.delInfo.u.lchDel.dir = crntLchCfg->dir;
         
            /* Construct transaction ID */
            NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, 
               ueCb->cellCb->cellId, ueCb->ueCbKey.crnti, 
               transCtrlBlk->nxTransId, NH_CRG_LOGCH_MAP, NH_CFG_ACTN_RLS,
               crntLchCfg->lchId.lcId, crntLchCfg->dir);

            transCtrlBlk->numOfMacCfms++;
            /* Send Configuration to MAC layer */
            if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, 
                                    crgCfgParam))
            {
              transCtrlBlk->numOfMacCfms--;
              RLOG0(L_ERROR, "Failed to send data to lower layer");
              RETVALUE(RFAILED);
            }
        }/* found */
      }/* for rbCfgList */
   } /* cfgType */

   RETVALUE(ROK);
}  /* end of nhPIMSendMacRlsCfg */


/**
 *
 *   @brief Handler to add/mod/rls cell cfg at MAC.
 *
 *   @details
 *   This function is used to add cell cfg at MAC.
 *            
 *
 *   @param[in] cellCb  Cell control block.
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMSendMacCellCfgAdd
(
NhCellCb *cellCb             /* Cell control block */
)
#else /* ANSI */
PRIVATE S16 nhPIMSendMacCellCfgAdd(cellCb)
NhCellCb *cellCb;             /* Cell control block */
#endif /* ANSI */
{
   CrgCfgReqInfo     *crgCfgParam = NULLP;  /* MAC Configuration Request parameter */
   CrgCfgTransId     crgCfgTransId;         /* MAC Transaction ID */
   Pst               tmpPst;                /* Post structure */ 
   NhLSapCb          *lSap = NULLP;         /* Lower SAP Cb */
   NhCellOrdrdCfg    *cellCfgInfo = NULLP;  /* RRC cell configuration info */
   CrgLchCfg         *crgLchConfig = NULLP;    /* logical channel configuration info */
   CrgCellCfg        *crgCellConfig = NULLP;   /* RMAC cell cfg */ 
   NhuRbCfgInfo      *rbCfgInfo  = NULLP;   /* RRB Configuration info */
   SuId              suId = NH_ZERO_VAL;    /* It is hard coded for only one SAP
                                               configuration */    
   U8                rbCount;               /* RCounter for RBs */
   U8                lcId = NH_ZERO_VAL;    /* RLogical channel ID */ 
   U16               ueId = NH_ZERO_VAL;    /* UE ID */
   U8                dir;                   /* RDirection UL/DL */
   
   TRC2(nhPIMSendMacCellCfg)
   RLOG0(L_DEBUG, "nhPIMSendMacCellCfg");

   /* Get lower SAP*/
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, cellCb, STNHCRGSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;
   
   /* Initialize transid structure to zero */
   NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));

   /* Get ordered cell config */
   cellCfgInfo = cellCb->cellTransCb.cellOrdrdCfg;
  
   if (cellCfgInfo->cellCfgInfo.cellCfgBitMsk & (NHU_CELL_CFG_MAC_CMN))
   {
      /* Allocate memory for cfg parameter */
      NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
      if (NH_CHK_INPUT_PARAM(crgCfgParam))
      {
         RLOG0(L_FATAL, "GOT NULL crgCfgParam");
         RETVALUE(RFAILED);
      }
      
      /* Add configuration to MAC */
      crgCfgParam->action = CRG_CONFIG;
      /* Cell configuration */
      crgCfgParam->u.cfgInfo.cfgType = CRG_CELL_CFG;
      crgCellConfig = &crgCfgParam->u.cfgInfo.u.cellCfg;
      crgCellConfig->cellId = cellCb->cellId;
      nhPIMFillMacCellCfgParam(crgCellConfig, 
                              &(cellCfgInfo->cellCfgInfo));
     
      /* Construct transaction ID */
      NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, 
            crgCellConfig->cellId, ueId, cellCb->cellTransCb.nxTransId, 
            NH_CRG_CELL_CFG, NH_CFG_ACTN_ADD, NH_ZERO_VAL, NH_ZERO_VAL);
      cellCb->cellTransCb.numOfMacCfms++;
      /* Send Configuration to MAC layer */
      if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, 
                              crgCfgParam))
      {
         cellCb->cellTransCb.numOfMacCfms--;
         RLOG0(L_ERROR, "Failed to send data to lower layer");
         RETVALUE(RFAILED);
      }
   }
   if (cellCfgInfo->cellCfgInfo.cellCfgBitMsk & NHU_CELL_CFG_RB)
   {
      /* get rb from the list */
      for (rbCount = 0; rbCount <
            cellCfgInfo->cellCfgInfo.rbToAddCfgList.numOfRbCfgInfo; 
            rbCount++)
      {   
         /* Allocate memory for cfg Parameter */
         NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
         if (NH_CHK_INPUT_PARAM(crgCfgParam))
         {
            RLOG0(L_FATAL, "GOT NULL crgCfgParam");
            RETVALUE(RFAILED);
         }
        
      /* Add configuration to MAC */
         crgCfgParam->action = CRG_CONFIG;
         
      /* Logical channel configuration */
         crgCfgParam->u.cfgInfo.cfgType = CRG_LCH_CFG;
         crgLchConfig = &crgCfgParam->u.cfgInfo.u.lchCfg;
         crgLchConfig->cellId = cellCb->cellId;
         rbCfgInfo = 
         &(cellCfgInfo->cellCfgInfo.rbToAddCfgList.rbCfgInfoList[rbCount]);
         /* Fill logical channel info for cell configuration */
         nhPIMFillMacLchCfgParam(crgLchConfig, rbCfgInfo, &lcId, &dir);
       
         /* Construct transaction ID */
         NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans,
               cellCb->cellId, ueId,
               cellCb->cellTransCb.nxTransId, NH_CRG_CELL_LOGCH_MAP, 
               NH_CFG_ACTN_ADD, lcId, NH_ZERO_VAL);
        
         /* Increment the number of confirms expected from MAC layer */
         cellCb->cellTransCb.numOfMacCfms++;
         /* Send Configuration to MAC layer */
         if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, 
                                 crgCfgParam))
         {
            cellCb->cellTransCb.numOfMacCfms--;
            RLOG0(L_ERROR, "Failed to send data to lower layer");
            RETVALUE(RFAILED);
         }

      } /* for (rbCount) */
   } /* NHU_CELL_CFG_RB */
   RETVALUE(ROK);
}/* End of nhPIMSendMacCellCfgAdd */



/**
 *
 *   @brief Handler to add/mod/rls cell cfg at MAC.
 *
 *   @details
 *   This function is used to add/mod/rls cell cfg at MAC.
 *            
 *
 *   @param[in] cellCb  Cell control block.
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendMacCellCfg
(
NhCellCb *cellCb             /* Cell control block */
)
#else /* ANSI */
PUBLIC S16 nhPIMSendMacCellCfg(cellCb)
NhCellCb *cellCb;             /* Cell control block */
#endif /* ANSI */
{
   CrgCfgReqInfo     *crgCfgParam = NULLP;  /* MAC Configuration Request parameter */
   CrgCfgTransId     crgCfgTransId;         /* MAC Transaction ID */
   Pst               tmpPst;                /* Post structure */ 
   NhLSapCb          *lSap = NULLP;         /* Lower SAP Cb */
   NhCellOrdrdCfg    *cellCfgInfo = NULLP;  /* RRC cell configuration info */
   SuId              suId = NH_ZERO_VAL;    /* It is hard coded for only one SAP
                                               configuration */    
   U16               ueId = NH_ZERO_VAL;    /* UE ID */
   S16            ret;
   
   TRC2(nhPIMSendMacCellCfg)
   RLOG0(L_DEBUG, "nhPIMSendMacCellCfg");

   /* Get lower SAP*/
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, cellCb, STNHCRGSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;
   ret = ROK;
   
   /* Initialize transid structure to zero */
   NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));

   /* Get ordered cell config */
   cellCfgInfo = cellCb->cellTransCb.cellOrdrdCfg;
  
   switch(cellCb->cellTransCb.cfgAction)
   {
      case NH_CFG_ACTN_ADD: 
         ret = nhPIMSendMacCellCfgAdd(cellCb);
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
         /* Allocate memory for cfg parameter */
         NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
         if (NH_CHK_INPUT_PARAM(crgCfgParam))
         {
            RLOG0(L_FATAL, "GOT NULL crgCfgParam");
            RETVALUE(RFAILED);
         }
         /* Re-configuration to MAC */
         crgCfgParam->action = CRG_RECONFIG;
         /* Cell configuration */
         crgCfgParam->u.recfgInfo.recfgType = CRG_CELL_CFG;
         crgCfgParam->u.recfgInfo.u.cellRecfg.cellId = cellCb->cellId;
         /* Fill re-configuration parameter */
         nhPIMFillMacCellReCfgParam(&crgCfgParam->u.recfgInfo.u.cellRecfg, 
                       &(cellCfgInfo->cellCfgInfo));
         /* Construct transaction ID */
         NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans,
                     cellCb->cellId,ueId,
                     cellCb->cellTransCb.nxTransId, NH_CRG_CELL_CFG, 
                     NH_CFG_ACTN_MODIFY, NH_ZERO_VAL, NH_ZERO_VAL);
         /* Increment the no of confirm expected from MAC layer */
          
         cellCb->cellTransCb.numOfMacCfms++;
         /* Send Configuration to MAC layer */
         if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, 
                                 crgCfgParam))
         {
            RLOG0(L_ERROR,"Failed to send data to lower layer");
            cellCb->cellTransCb.numOfMacCfms--;
            RETVALUE(RFAILED);
         }
         break;

      case NH_CFG_ACTN_RLS:
         /* Allocate memory for cfg Parameter */
         NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
         if (NH_CHK_INPUT_PARAM(crgCfgParam))
         {
            RLOG0(L_FATAL, "GOT NULL crgCfgParam");
            RETVALUE(RFAILED);
         }
         
         /* Delete configuration to MAC */
         crgCfgParam->action = CRG_DELETE;
         /* Cell delete */
         crgCfgParam->u.delInfo.delType = CRG_CELL_CFG;
         crgCfgParam->u.delInfo.u.cellDel.cellId = cellCb->cellId;
         /* Construct transaction ID */
         NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans,
                     cellCb->cellId, ueId,
                     cellCb->cellTransCb.nxTransId, NH_CRG_CELL_CFG, 
                     NH_CFG_ACTN_RLS, NH_ZERO_VAL, NH_ZERO_VAL);
         
         cellCb->cellTransCb.numOfMacCfms++;
         
         /* Send Configuration to MAC layer */
         if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, 
                                 crgCfgParam))
         {
            RLOG0(L_ERROR,"Failed to send data to lower layer");
            cellCb->cellTransCb.numOfMacCfms--;
            RETVALUE(RFAILED);
         }

         break;
      
      default:
         RLOG0(L_ERROR,"Invalid Cfg Action ");
         RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);

} /* end of nhPIMSendMacCellCfg */


/**
 *
 *   @brief Handler to revert cell cfg at MAC.
 *
 *   @details
 *   This function is used to revert cell cfg at MAC.
 *            
 *
 *   @param[in] cellCb  Cell control block
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRvrtMacCellCfg
(
NhCellCb *cellCb             /* Cell control block */
)
#else /* ANSI */
PUBLIC S16 nhPIMRvrtMacCellCfg(cellCb)
NhCellCb *cellCb;             /* Cell control block */
#endif /* ANSI */
{
   CrgCfgReqInfo    *crgCfgParam = NULLP;   /* MAC Config Request Parameter */
   CrgCfgTransId    crgCfgTransId;          /* MAC Transaction Id */
   NhCellTransCb    *cellTransCb = NULLP;   /* Cell TransCb */
   NhCellCfgInfo    *cellCfgInfo = NULLP;   /* Cell config Info */ 
   U8               rvrtAction = 0x00;      /* Revert cction */ 
   Pst              tmpPst;                 /* Post structure */ 
   NhLSapCb         *lSap = NULLP;          /* Lower SAP Cb */
   SuId             suId = NH_ZERO_VAL;     /* service user id */
   U8               i = NH_ZERO_VAL;        /* Iterator for loop */
   U16              ueId = NH_ZERO_VAL;     /* UE ID  */
   U32              rvrtTransId = NH_RESERVED_TRANS_ID;  
      
   TRC2(nhPIMRvrtMacCellCfg)
   RLOG0(L_DEBUG, "nhPIMRvrtMacCellCfg");

   /* Get lower Sap */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, cellCb, STNHCRGSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;
   
   /* Reterive cell trans Cb */
   cellTransCb = &(cellCb->cellTransCb);
   if (cellTransCb->cfgAction == NH_CFG_ACTN_ADD)
       rvrtAction = NH_CFG_ACTN_RLS;
   if (cellTransCb->cfgAction == NH_CFG_ACTN_MODIFY)
       rvrtAction = NH_CFG_ACTN_MODIFY;
  
   /* Check for Cell Cfg */
   if ((cellTransCb->macMapTbl[0].status ==  NH_STATUS_USED) &&
       (cellTransCb->macMapTbl[0].cfmStatus != NH_CFG_FAILED))
   {
      /* Allocate memory for crg configuration */
      NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
      if (NH_CHK_INPUT_PARAM(crgCfgParam))
      {
         RLOG0(L_FATAL, "GOT NULL crgCfgParam");
         RETVALUE(RFAILED);
      }
      /* Initialize transaction id */
      NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
      
      switch(rvrtAction)
      {
         case NH_CFG_ACTN_RLS:
            /* If revert action is release, then original action was add */
            /* send delete configuration */

            crgCfgParam->action = CRG_DELETE;
            /* Cell delete */
            crgCfgParam->u.delInfo.delType = CRG_CELL_CFG;
            crgCfgParam->u.delInfo.u.cellDel.cellId = 
                               cellCb->cellId;

            NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans,
                     cellCb->cellId, ueId,
                     rvrtTransId, NH_CRG_CELL_CFG, 
                     NH_CFG_ACTN_RLS, NH_ZERO_VAL, NH_ZERO_VAL);
            
            /* Send Configuration to MAC layer */
            if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, 
                                      crgCfgTransId, crgCfgParam))
            {
                RLOG0(L_ERROR, "Failed to configuration to MAC layer");
                RETVALUE(RFAILED);
            }
            RETVALUE(ROK);
         
         case NH_CFG_ACTN_MODIFY:
            {
               /* If revert action is add, then original action was Rls */
               /* Get Cfg info from crntcfg and add it now */
                cellCfgInfo = &(cellCb->crntCellCfgInfo);
                crgCfgParam->action = CRG_RECONFIG;
                crgCfgParam->u.recfgInfo.recfgType = CRG_CELL_CFG;
                crgCfgParam->u.recfgInfo.u.cellRecfg.cellId = 
                                       cellCb->cellId;
                nhPIMFillMacRvrtCellReCfgParam(
                    &crgCfgParam->u.recfgInfo.u.cellRecfg, cellCfgInfo); 
                
                NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans,
                      cellCb->cellId, ueId, 
                      rvrtTransId, NH_CRG_CELL_CFG, 
                      NH_CFG_ACTN_MODIFY, NH_ZERO_VAL, NH_ZERO_VAL);
                
                /* Send Configuration to MAC layer */
                if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, 
                                          crgCfgTransId, crgCfgParam))
                {
                   RLOG0(L_ERROR, "Failed to configuration to MAC layer");
                   RETVALUE(RFAILED);
                }

            }
            break;
         default:
            RLOG0(L_ERROR, "Invalid case of revert config for MAC layer");
            NH_FREE(NH_REG, NH_POOL, crgCfgParam, sizeof(CrgCfgReqInfo));
            RETVALUE(RFAILED);

      }/* Switch case */
   }

   /* Check for RBs */
   for (i = 1; i< NH_MAX_RB_PER_CELL_CFG; i++)
   {
      /* If status is used and logical channel configuration is not failed */
      /* Revert those ID */
      if ((cellTransCb->macMapTbl[i].status == NH_STATUS_USED) &&
        (cellTransCb->macMapTbl[i].cfmStatus != NH_CFG_FAILED) && 
        ( rvrtAction == NH_CFG_ACTN_RLS ))
      {
         /* Allocate config parameter */
         NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
         if (NH_CHK_INPUT_PARAM(crgCfgParam))
         {
            RLOG0(L_FATAL, "GOT NULL crgCfgParam");
            RETVALUE(RFAILED);
         }
         NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
         
         /* Delete configuration */
         crgCfgParam->action = CRG_DELETE;
         crgCfgParam->u.delInfo.delType = CRG_LCH_CFG;
         crgCfgParam->u.delInfo.u.lchDel.cellId = 
                           cellCb->cellId;
         crgCfgParam->u.delInfo.u.lchDel.lcId = 
                    cellTransCb->macMapTbl[i].logChId;  
         crgCfgParam->u.delInfo.u.lchDel.dir = 
                        cellTransCb->macMapTbl[i].dir;  
       
         /* Construct transaction id */
         NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans,
               cellCb->cellId, ueId, 
               rvrtTransId, NH_CRG_CELL_LOGCH_MAP, 
               NH_CFG_ACTN_RLS,  crgCfgParam->u.cfgInfo.u.lchCfg.lcId,
               NH_ZERO_VAL);
          
         /* Send Configuration to MAC layer */
         if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, 
                                   crgCfgTransId, crgCfgParam))
         {
            RLOG0(L_ERROR, "Failed to configuration to MAC layer");
            RETVALUE(RFAILED);
         }

      } /* if ((cellTransCb->macMapTbl[i].status == NH_STATUS_USED) ... */
   } /* for (i = 1; i< NH_MAX_RB_PER_CELL_CFG; i++) */
                                                  
   RETVALUE(ROK);

} /* end of nhPIMRvrtMacCellCfg */


/**
 *
 *   @brief Handler to add new configuration to MAC.
 *
 *   @details
 *   This function is used to add new configuration   
 *   to MAC.
 *            
 *  @param[in] ueCb      UE control block 
 *  @param[in] transCtrlBlk   Transaction control block
 *  @param[in] cfgType   Configuration type
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendMacAddCfg
(
NhActvUeCb *ueCb,         /* UE control block */
TransCb    *transCtrlBlk,      /* Transaction control block */
U8 cfgType                /* Configuration type */
)
#else /* ANSI */
PUBLIC S16 nhPIMSendMacAddCfg(ueCb, transCtrlBlk, cfgType)
NhActvUeCb *ueCb;         /* UE control block */
TransCb    *transCtrlBlk;      /* Transaction control block */
U8 cfgType;               /* configuration type */
#endif  /* ANSI */
{
   CrgCfgReqInfo     *crgCfgParam = NULLP;    /* MAC Configuration Request parameter */
   CrgCfgTransId     crgCfgTransId;           /* MAC Transaction ID */
   Pst               tmpPst;                  /* Post structure */ 
   NhLSapCb          *lSap = NULLP;           /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL;      /* Service user ID */ 
   NhOrdrdCfgInfo    *ordrdCfg = NULLP;       /* Ordered configuration */
   NhuRbCfgList      *rbCfgList = NULLP;      /* Configuration list */
   NhuRbCfgInfo      *rbCfgInfo = NULLP;      /* RB configuration info */ 
   U8                lcId = NH_ZERO_VAL;      /* Logical channel ID */ 
   U8                i = NH_ZERO_VAL;       
   U8                dir = NH_ZERO_VAL;       
   
   TRC2(nhPIMSendMacAddCfg)
   RLOG0(L_DEBUG, "nhPIMSendMacAddCfg");
   
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCRGSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;

   ordrdCfg = &transCtrlBlk->ordrdCfg;
   
   if (cfgType & NH_CRG_UE_CFG)
   {
      /* Allocate memory for cfg Parameter */
      NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
      if (NH_CHK_INPUT_PARAM(crgCfgParam))
      {
         RLOG0(L_FATAL, "GOT NULL crgCfgParam");
         RETVALUE(RFAILED);
      }
      NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
      /* configuration to MAC */
      crgCfgParam->action = CRG_CONFIG;
      crgCfgParam->u.cfgInfo.cfgType = CRG_UE_CFG;
      crgCfgParam->u.cfgInfo.u.ueCfg.cellId = ueCb->cellCb->cellId;
      crgCfgParam->u.cfgInfo.u.ueCfg.crnti = ueCb->ueCbKey.crnti;
     
      if ((ordrdCfg->actionMask & NH_ORD_CFG_TRCH) ==NH_ZERO_VAL)
      {
         RLOG0(L_ERROR, "Mandatory TRCH config not found");
         RETVALUE(RFAILED);
      }
      else 
      {
         /* Fill Trch param */
         nhPIMFillMacUeCfgTrchParam(&crgCfgParam->u.cfgInfo.u.ueCfg,
            &transCtrlBlk->ordrdCfg.trChCfg);
      }
      
      NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, ueCb->cellCb->cellId,
         ueCb->ueCbKey.crnti, transCtrlBlk->nxTransId, NH_CRG_UE_CFG, 
         NH_CFG_ACTN_ADD, NH_ZERO_VAL, NH_ZERO_VAL);

      transCtrlBlk->numOfMacCfms++;

      /* Send Configuration to MAC layer */
      if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, crgCfgParam))
      {
         RLOG0(L_ERROR,"Failed to send data to lower layer");
         transCtrlBlk->numOfMacCfms--;
         RETVALUE(RFAILED);
      }
   }
   if (cfgType & NH_CRG_LOGCH_MAP)
   {
      if (ordrdCfg->actionMask & NH_ORD_CFG_RB_ADD)
      {
         rbCfgList = &ordrdCfg->rbToAddCfgList;
         for (i = 0; i < rbCfgList->numOfRbCfgInfo; i++)
         {
            /* Allocate memory for cfg parameter */
            NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
            if (NH_CHK_INPUT_PARAM(crgCfgParam))
            {
               RLOG0(L_FATAL, "GOT NULL crgCfgParam");
               RETVALUE(RFAILED);
            }
            NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
            /* Configuration to MAC */
            crgCfgParam->action = CRG_CONFIG;
            crgCfgParam->u.cfgInfo.cfgType = CRG_LCH_CFG;
            crgCfgParam->u.cfgInfo.u.lchCfg.cellId = ueCb->cellCb->cellId;
            crgCfgParam->u.cfgInfo.u.lchCfg.crnti = ueCb->ueCbKey.crnti;
            rbCfgInfo = &(rbCfgList->rbCfgInfoList[i]);
            nhPIMFillMacLchCfgParam(&crgCfgParam->u.cfgInfo.u.lchCfg, rbCfgInfo,
                  &lcId, &dir);
            NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, 
               ueCb->cellCb->cellId, ueCb->ueCbKey.crnti, 
               transCtrlBlk->nxTransId, NH_CRG_LOGCH_MAP, 
               NH_CFG_ACTN_ADD,lcId, dir);

            transCtrlBlk->numOfMacCfms++;

            /* Send configuration to MAC layer */
            if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, 
                                    crgCfgParam))
            {
               RLOG0(L_ERROR, "Failed to send data to lower layer");
               transCtrlBlk->numOfMacCfms--;
               RETVALUE(RFAILED);
            }
         }
      }
   }

   RETVALUE(ROK);
}  /* end of nhPIMSendMacAddCfg */

/**
 *
 *   @brief Handler to add configuration to PDCP for specified RB Id.
 *
 *   @details
 *   This function is used to add/mod/delete configuration   
 *   to PDCP for specified RB Id.
 *            
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Trans control block
 *   @param[out] cpjCfgReq  CPJ  Configuration Request information
 *   @param[in] rbCount   No. of RbCount
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillPdcpRlsCfg
(
NhActvUeCb    *ueCb,
TransCb       *transCtrlBlk,
CpjCfgReqInfo *cpjCfgReq,
U8            *rbCount
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPdcpRlsCfg(ueCb, transCtrlBlk, cpjCfgReq, rbCount)
NhActvUeCb    *ueCb;
TransCb       *transCtrlBlk;
CpjCfgReqInfo *cpjCfgReq;
U8            *rbCount;
#endif /* ANSI */
{
   NhRbToLchMap          *crntLchCfg = NULLP;
   CmLList               *tmpNode = NULLP;
   NhuRbId               *rbIdPtr = NULLP;
   U8                     i = NH_ZERO_VAL; 
   Bool                   found = FALSE; 

   TRC2(nhPIMFillPdcpRlsCfg)

   RLOG0(L_DEBUG, "nhPIMFillPdcpRlsCfg");

   for ( i=0; i < transCtrlBlk->ordrdCfg.rbToRlsCfgList.numOfRbCfgInfo; i++)
   {
      rbIdPtr = &transCtrlBlk->ordrdCfg.rbToRlsCfgList.rbsToRel[i];
      if (rbIdPtr != NULLP)
      {
        /* Find logical channel id */
        CM_LLIST_FIRST_NODE(&(ueCb->crntCfg.logChCbLst), tmpNode);
        while(NULLP != tmpNode)
        {
           crntLchCfg = (NhRbToLchMap *)(tmpNode->node);

           if((*rbIdPtr).rbId == crntLchCfg->rbId.rbId &&
              (*rbIdPtr).rbTyp == crntLchCfg->rbId.rbTyp &&
              (crntLchCfg->isPdcpConfigured == TRUE))
           {
              found = TRUE;
              break;
           }
           CM_LLIST_NEXT_NODE(&ueCb->crntCfg.logChCbLst, tmpNode);
        }/* end of while */
    
        if(found)
        {
           cpjCfgReq->cfgEnt[*rbCount].cfgType = CPJ_CFG_DELETE;
           cpjCfgReq->cfgEnt[*rbCount].rbId = (*rbIdPtr).rbId;
           cpjCfgReq->cfgEnt[*rbCount].rbType = (*rbIdPtr).rbTyp;
           (*rbCount)++;
           found = FALSE;
        }

      } /* if (rbIdPtr != NULLP) */
   } /* end of for loop */
   RETVALUE(ROK);
}/* End of nhPIMFillPdcpRlsCfg */

/**
 *
 *   @brief Handler to re-establish configuration to PDCP for specified RB Id list.
 *
 *   @details
 *   This function is used to re-establish configuration   
 *   to PDCP for specified RB Id.
 *            
 *
 *   @param[in] transCtrlBlk   Trans control block
 *   @param[out] cpjCfgReq  CPJ  Configuration Request information
 *   @param[in] rbCount   No. of RbCount
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillPdcpRestbCfg
(
TransCb       *transCtrlBlk,
CpjCfgReqInfo *cpjCfgReq,
U8            *rbCount
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPdcpRestbCfg(transCtrlBlk, cpjCfgReq, rbCount)
TransCb         *transCtrlBlk;
CpjCfgReqInfo   *cpjCfgReq;
U8              *rbCount;
#endif /* ANSI */
{
   CmLList               *tmpNode = NULLP;

   TRC2(nhPIMFillPdcpRestbCfg)

   CM_LLIST_FIRST_NODE(&(transCtrlBlk->ordrdCfg.rbToRestbCfgList.rbCfgInfoList),
                         tmpNode);
   while(tmpNode)
   {
      cpjCfgReq->cfgEnt[*rbCount].cfgType = CPJ_CFG_REESTABLISH;
      cpjCfgReq->cfgEnt[*rbCount].rbId = 
              ((NhuRbCfgInfo *) tmpNode->node)->rbId.rbId;
      cpjCfgReq->cfgEnt[*rbCount].rbType = 
              ((NhuRbCfgInfo *) tmpNode->node)->rbId.rbTyp;
      (*rbCount)++;
      CM_LLIST_NEXT_NODE(
              &(transCtrlBlk->ordrdCfg.rbToRestbCfgList.rbCfgInfoList), 
                           tmpNode);
   }

   RETVALUE(ROK);
}/* End of nhPIMFillPdcpRestbCfg */

/**
 *
 *   @brief Handler to add configuration to PDCP for specified RB Id.
 *
 *   @details
 *   This function is used to add/mod/delete configuration   
 *   to PDCP for specified RB ID.
 *            
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Transaction control block
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendPdcpCfg
(
NhActvUeCb *ueCb,
TransCb *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhPIMSendPdcpCfg(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
#endif /* ANSI */
{
   CpjCfgReqInfo     *cpjCfgReq = NULLP;     /* PDCP Configuration Request parameter */
   Pst               tmpPst;                 /* Post structure */ 
   NhLSapCb          *lSap = NULLP;          /* Lower SAP Cb */
   NhuSecurityCfg    *secCfg = NULLP;
   SuId              suId = NH_ZERO_VAL;
   CpjUeInfo         *oldUeInfo = NULLP;
   CpjUeInfo         *newUeInfo = NULLP;
   U16               ueId;
   CpjReEstReqInfo   *restbReq = NULLP;
   CpjSecCfgReqInfo  *cpjSecCfgInfo = NULLP;
   
   TRC2(nhPIMSendPdcpCfg)

   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCPJSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;

   
   if ((NH_STATUS_USED == 
                   transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PDCP].status) &&
       (NH_PDCP_CFG_UEID_CHNG == 
                   transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PDCP].action))
   {
      NH_ALLOC(NH_REG, NH_POOL, &oldUeInfo, sizeof(CpjUeInfo));
      if(NULLP == oldUeInfo)
      {
         RLOG0(L_FATAL, "Could not allocate mem for Ue Info");
         RETVALUE(RFAILED);
      }
      NH_ALLOC(NH_REG, NH_POOL, &newUeInfo, sizeof(CpjUeInfo));
      if(NULLP == newUeInfo)
      {
         RLOG0(L_FATAL, "Could not allocate mem for New Ue Info");
         NH_FREE(NH_REG, NH_POOL, oldUeInfo, sizeof(CpjUeInfo));
         RETVALUE(RFAILED);
      }
      oldUeInfo->ueId = ueCb->ueCbKey.crnti;
      oldUeInfo->cellId = ueCb->cellCb->cellId;

      newUeInfo->ueId = transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;
      newUeInfo->cellId = ueCb->cellCb->cellId;
      ueId = transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;

      transCtrlBlk->numOfPdcpCfms++;
      if (ROK != NhLiCpjUeIdChgReq(&tmpPst, lSap->spId, transCtrlBlk->nxTransId,
                                                     oldUeInfo, newUeInfo))
      {
         transCtrlBlk->numOfPdcpCfms--;
         RLOG0(L_ERROR,"Failed to send UEID Change Req to RLC");
         RETVALUE(RFAILED);
      }
      
   }
   else
   {
      ueId = ueCb->ueCbKey.crnti;
   }
   /* If Security configuration is present and PDCP re-establishment 
      enabled then do this*/

   if ((NH_STATUS_USED == 
           transCtrlBlk->restbTbl[NH_RESTB_MAPTBL_IDX_PDCP_SRB1].status) &&
       (NH_PDCP_CFG_RESTB_SRB1 == 
           transCtrlBlk->restbTbl[NH_RESTB_MAPTBL_IDX_PDCP_SRB1].action))
   {
      /* allocate memory for cfg Parameter */
      NH_ALLOC(NH_REG, NH_POOL, &restbReq, sizeof(CpjReEstReqInfo));
      if(NULLP == restbReq)
      {
         RLOG0(L_FATAL, "Could not allocate mem for Re-Estb Req");
         RETVALUE(RFAILED);
      }

      /*R9 upgrade: Kworks Error: Duplicate NULL check is deleted*/

      restbReq->pdcpId.cellId   = ueCb->cellCb->cellId;
      restbReq->pdcpId.ueId     = ueId;
      restbReq->pdcpId.rbId     = NH_RB_ID_SRB1;
      restbReq->pdcpId.rbType   = CM_LTE_SRB;
      restbReq->transId  = transCtrlBlk->nxTransId;

       /*Fill and call PDPC related funtion*/
       secCfg = &transCtrlBlk->ordrdCfg.secCfg;
       nhPIMFillPdcpRestbSRB1Cfg(restbReq, secCfg);
      transCtrlBlk->numOfPdcpCfms++;
      if (ROK != NhLiCpjReEstReq(&tmpPst, lSap->spId, restbReq))
      {
         transCtrlBlk->numOfPdcpCfms--;
         RLOG0(L_ERROR, "Failed to send Re-Establish Req to PDCP");
         RETVALUE(RFAILED);
      }
   }


   /* If both security & PDCP entity configuration are present then security
    * will always be prensent in the zeroth index of the CpjCfgReqInfo. 
    * It should not be moved after filling the pdcp entity configuration. 
    * The handling of pdcp cfgcfm in LIM depends on this logic. */


   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_ADD ||
       transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_MOD ||
       transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS ||
       transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RESTB)
   {
      /* allocate memory for cfg Parameter */
      NH_ALLOC(NH_REG, NH_POOL, &cpjCfgReq, sizeof(CpjCfgReqInfo));
      if (NH_CHK_INPUT_PARAM(cpjCfgReq))
      {
         RLOG0(L_FATAL, "GOT NULL cpjCfgReq");
         RETVALUE(RFAILED);
      }

      cpjCfgReq->cellId   = ueCb->cellCb->cellId;
      cpjCfgReq->ueId     = ueId;
      cpjCfgReq->transId  = transCtrlBlk->nxTransId;

      if (ROK != nhPIMFillPdcpCfgParam(ueCb, transCtrlBlk, cpjCfgReq))
      {
         RLOG0(L_ERROR, " Failed to fill Pdcp Cfg Info ");
         RETVALUE(RFAILED);
      }

      if(cpjCfgReq->numEnt > 0)
      {
         transCtrlBlk->numOfPdcpCfms++;
         /* Send Configuration to PDCP layer */
         if (ROK != NhLiCpjCfgReq(&tmpPst, lSap->spId, cpjCfgReq))
         {
            transCtrlBlk->numOfPdcpCfms--;
            RLOG0(L_ERROR, " Failed to send data to lower layer ");
            RETVALUE(RFAILED);
         }
      }  
   }

   if ((transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_SEC))
   {
      secCfg = &transCtrlBlk->ordrdCfg.secCfg;
      NH_ALLOC(NH_REG, NH_POOL, &cpjSecCfgInfo, sizeof(CpjSecCfgReqInfo));
      if (NH_CHK_INPUT_PARAM(cpjSecCfgInfo))
      {
         RLOG0(L_FATAL, " GOT NULL cpjSecCfgInfo ");
         RETVALUE(RFAILED);
      }

      cpjSecCfgInfo->cellId   = ueCb->cellCb->cellId;
      cpjSecCfgInfo->ueId     = ueId;
      cpjSecCfgInfo->transId  = transCtrlBlk->nxTransId;
      
      /* R9 upgrade:kworks error: ret val check is deleted since this func
       * always returns ROK. in future if somebody changes the return val
       * then he has to add the validation for ret val
       */
      (Void)nhPIMFillPdcpSecCfg(cpjSecCfgInfo, secCfg, 
                                transCtrlBlk->enblSelSecModeCfg);

      cpjSecCfgInfo->secCfg.isHo = transCtrlBlk->ordrdCfg.rbToAddCfgList.isHO;

      transCtrlBlk->numOfPdcpCfms++;
      if (ROK != NhLiCpjSecCfgReq(&tmpPst, lSap->spId, cpjSecCfgInfo))
      {
         transCtrlBlk->numOfPdcpCfms--;
         RLOG0(L_ERROR, " Failed to send sec cfg data to lower layer ");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
} /* end of nhPIMSendPdcpCfg */


/**
 *
 *   @brief Handler to a to PDCP for specified UEID.
 *
 *   @details
 *   This function is used to a
 *   to PDCP for specified RB ID.
 *            
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Transaction control block
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendStartPdcpDataFwdReq 
(
NhActvUeCb *ueCb,
Bool  isUlDatFwdReq
)
#else /* ANSI */
PUBLIC S16 nhPIMSendStartPdcpDataFwdReq(ueCb, isUlDatFwdReq)
NhActvUeCb *ueCb;
Bool  isUlDatFwdReq;
#endif /* ANSI */
{
   Pst               tmpPst;                 /* Post structure */ 
   NhLSapCb          *lSap = NULLP;          /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL;
   U16               ueId;
   CpjDataFwdReqInfo *cpjDataFwdReqInfo = NULLP;

   TRC2(nhPIMSendStartPdcpDataFwdReq)

   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCPJSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, " Failed to find lower layer or SAP not bound ");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;
   ueId = ueCb->ueCbKey.crnti;

   /* allocate memory for cfg Parameter */
   NH_ALLOC(NH_REG, NH_POOL, &cpjDataFwdReqInfo, sizeof(CpjDataFwdReqInfo));
   if (NH_CHK_INPUT_PARAM(cpjDataFwdReqInfo))
   {
      RLOG0(L_FATAL, " GOT NULL cpjDataFwdReqInfo ");
      RETVALUE(RFAILED);
   }
   
   cpjDataFwdReqInfo->cellId   = ueCb->cellCb->cellId;
   cpjDataFwdReqInfo->ueId     = ueId;
   cpjDataFwdReqInfo->transId  = 0;
   cpjDataFwdReqInfo->isUlDatFwdReq  = isUlDatFwdReq;

   /* Send Configuration to PDCP layer */
   if (ROK != NhLiCpjStartDataFwdReq(&tmpPst, lSap->spId, cpjDataFwdReqInfo))
   {
      RLOG0(L_ERROR, " Failed to send data forward request to PDCP layer ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of nhPIMSendStartPdcpDataFwdReq*/


/**
 *
 *   @brief Handler to a to PDCP for specified UEID.
 *
 *   @details
 *   This function is used to a
 *   to PDCP for specified RB ID.
 *            
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Transaction control block
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendPdcpDatResumeReq 
(
NhActvUeCb *ueCb,
NhTransId transId   
)
#else /* ANSI */
PUBLIC S16 nhPIMSendPdcpDatResumeReq(ueCb, transId)
NhActvUeCb *ueCb;
NhTransId transId;   
#endif /* ANSI */
{
   Pst               tmpPst;                 /* Post structure */ 
   NhLSapCb          *lSap = NULLP;          /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL;
   U16               ueId;
   CpjDatResumeReqInfo *cpjDatResumeReqInfo = NULLP;

   TRC2(nhPIMSendPdcpDatResumeReq)

   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCPJSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, " Failed to find lower layer or SAP not bound ");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;
   ueId = ueCb->ueCbKey.crnti;

   /* allocate memory for cfg Parameter */
   NH_ALLOC(NH_REG, NH_POOL, &cpjDatResumeReqInfo, sizeof(CpjDatResumeReqInfo));
   if (NH_CHK_INPUT_PARAM(cpjDatResumeReqInfo))
   {
      RLOG0(L_FATAL, " GOT NULL cpjDatResumeReqInfo ");
      RETVALUE(RFAILED);
   }
   
   cpjDatResumeReqInfo->cellId   = ueCb->cellCb->cellId;
   cpjDatResumeReqInfo->ueId     = ueId;
   cpjDatResumeReqInfo->transId  = transId;

   /* Send Configuration to PDCP layer */
   if (ROK != NhLiCpjDatResumeReq(&tmpPst, lSap->spId, cpjDatResumeReqInfo))
   {
      RLOG0(L_ERROR, " Failed to send data resume request to PDCP layer ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of nhPIMSendPdcpDatResumeReq*/


/**
 *
 *   @brief Handler to fill PDCP revert configuration info.
 *
 *   @details
 *   This function is used to fill PDCP revert configuration   
 *   data to PDCP interface structure.
 *            
 *
 *   @param[in] ueCb        UE control block
 *   @param[out] cpjCfgReq   Configuration request structure
 *   @param[in] rbCfgInfo   RB configuration info
 *   @param[in] cfgType     Configuration type
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillPdcpRvrtCfgParam
(
NhActvUeCb    *ueCb,
CpjCfgReqInfo *cpjCfgReq,
NhuRbCfgInfo  *rbCfgInfo,
U8            cfgType 
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPdcpRvrtCfgParam(ueCb, cpjCfgReq, rbCfgInfo, cfgType)
NhActvUeCb    *ueCb;
CpjCfgReqInfo *cpjCfgReq;
NhuRbCfgInfo  *rbCfgInfo;
U8            cfgType; 
#endif /* ANSI */
{
   U8   rbCount = NH_ZERO_VAL;
   CpjCfgEnt  *cpjParam = NULLP;
   
   TRC2(nhPIMFillPdcpRvrtCfgParam)
   RLOG0(L_DEBUG, "nhPIMFillPdcpRvrtCfgParam");

   rbCount = cpjCfgReq->numEnt;

   cpjParam = &cpjCfgReq->cfgEnt[rbCount];

   cpjParam->cfgType = cfgType;
   cpjParam->rbId = rbCfgInfo->rbId.rbId;
   cpjParam->rbType = rbCfgInfo->rbId.rbTyp;

   if(cfgType == CPJ_CFG_ADD)
   {
      nhPIMFillPdcpRvrtAddCfgParam(cpjCfgReq, rbCfgInfo, ueCb);
   }
   else if(cfgType == CPJ_CFG_MODIFY)
   {
      nhPIMFillPdcpRvrtModCfgParam(cpjCfgReq, rbCfgInfo, ueCb);
   }
   RETVALUE(ROK);
}/* End of nhPIMFillPdcpRvrtCfgParam */

/**
 *
 *   @brief Handler to fill PDCP revert Add configuration info in release 
 *          failure.
 *
 *   @details
 *   This function is used to fill PDCP revert Add configuration   
 *   data to PDCP interface structure in case release failure.
 *            
 *
 *   @param[in] cpjCfgReq   Configuration request structure
 *   @param[in] rbCfgInfo   RB configuration info
 *   @param[in] ueCb        UE control block
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillPdcpRvrtAddCfgParam
(
CpjCfgReqInfo *cpjCfgReq,
NhuRbCfgInfo  *rbCfgInfo,
NhActvUeCb    *ueCb
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPdcpRvrtAddCfgParam(cpjCfgReq, rbCfgInfo, ueCb)
CpjCfgReqInfo *cpjCfgReq;
NhuRbCfgInfo  *rbCfgInfo;
NhActvUeCb    *ueCb;
#endif /* ANSI */
{
   U8   rbCount = NH_ZERO_VAL;
   S16  rohcProfIndex;
   CpjCfgEnt  *cpjParam = NULLP;

   TRC2(nhPIMFillPdcpRvrtAddCfgParam)
   RLOG0(L_DEBUG, "nhPIMFillPdcpRvrtAddCfgParam");

   rbCount = cpjCfgReq->numEnt;
   cpjParam = &cpjCfgReq->cfgEnt[rbCount];
   
   if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL)
   {
      cpjParam->m.addCfg.rlcInfo.dir |= NHU_RB_CFG_UL;
      cpjParam->m.addCfg.rlcInfo.mode = rbCfgInfo->ulRbCfg.ulRlcMode.mode;
   }
   if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL)
   {
      cpjParam->m.addCfg.rlcInfo.dir |= NHU_RB_CFG_DL;
      cpjParam->m.addCfg.rlcInfo.mode = rbCfgInfo->dlRbCfg.dlRlcMode.mode;
   }
   if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
   {
      /* Fill Discard Timer Value */
      if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_DISCARD_TMR)
      {
            cpjParam->m.addCfg.discardTmr =  rbCfgInfo->pdcpCfg.discardTmr;
      }
      if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk &  NHU_PDCP_SN_SIZE)
      {
         if (cpjParam->m.addCfg.rlcInfo.mode == NHU_RLC_MODE_UM)
         {
            /* Fill PDCP sequence size parameter */
            cpjParam->m.addCfg.rlcInfo.m.rlcUm.pdcpSNSize = 
               rbCfgInfo->pdcpCfg.snSize;
         }
         /* Adding support for PDCP 15 bit SN in AM mode */
         else if (cpjParam->m.addCfg.rlcInfo.mode == NHU_RLC_MODE_AM)
         {
            /* Fill PDCP sequence size parameter */
            cpjParam->m.addCfg.rlcInfo.m.rlcAm.pdcpSNSize = 
               rbCfgInfo->pdcpCfg.snSize;
         }
      }
      if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_STATUS_RPT)
      {
         if (cpjParam->m.addCfg.rlcInfo.mode == NHU_RLC_MODE_AM)
         {
            /* Fill status report required parameter */
            cpjParam->m.addCfg.rlcInfo.m.rlcAm.statusRepReqd = 
                rbCfgInfo->pdcpCfg.statusRptReq;
         }
      }
      /* Fill Header compression parameter */
      if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk &  NHU_PDCP_HDR_COMPR_INFO)
      {
         cpjParam->m.addCfg.hdrCompCfg.rohcInfo.maxCid = 
            rbCfgInfo->pdcpCfg.hdrComprInfo.rohcInfo.maxCid;

         for (rohcProfIndex = 0; rohcProfIndex < NHU_MAX_ROHC_PROF_LST_LEN; 
                                 rohcProfIndex++)
         {
             cpjParam->m.addCfg.hdrCompCfg.rohcInfo.profLst[rohcProfIndex] = 
                   rbCfgInfo->pdcpCfg.hdrComprInfo.rohcInfo.rohcProfileList[
                                 rohcProfIndex];
         }
      }

      /* Requesting PDCP to discard PDUs if Integrity Verification fails.*/
      cpjParam->m.addCfg.discReqd = TRUE;

      /* If RB is SRB1 and security activation is not completed,requesting *
       * PDCP not to discard PDU by setting discReqd flag to FALSE.        */ 
      if(!( ueCb->rbSecCfgBitMask &  NH_SEC_CMPLTD) && 
          (NH_RB_ID_SRB1 == rbCfgInfo->rbId.rbId) && 
          (CM_LTE_DRB != rbCfgInfo->rbId.rbTyp) ) 
      {
         cpjParam->m.addCfg.discReqd = FALSE;
      }

      /* Fill CfmReqd parameter */
      if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_CFM_REQD)
      {
         cpjParam->m.addCfg.cfmReqd = rbCfgInfo->pdcpCfg.cfmReqd;
      }
   }
   cpjCfgReq->numEnt++; 
   RETVALUE(ROK);
}/* End of nhPIMFillPdcpRvrtAddCfgParam */

/**
 *
 *   @brief Handler to fill PDCP revert configuration info in case of 
 *          modification failure.
 *
 *   @details
 *   This function is used to fill PDCP revert configuration   
 *   data to PDCP interface structure in modification failure.
 *            
 *
 *   @param[in] cpjCfgReq   Configuration request structure
 *   @param[in] rbCfgInfo   RB configuration info
 *   @param[in] ueCb        UE control block
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillPdcpRvrtModCfgParam
(
CpjCfgReqInfo *cpjCfgReq,
NhuRbCfgInfo  *rbCfgInfo,
NhActvUeCb    *ueCb
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPdcpRvrtModCfgParam(cpjCfgReq, rbCfgInfo, ueCb)
CpjCfgReqInfo *cpjCfgReq;
NhuRbCfgInfo  *rbCfgInfo;
NhActvUeCb    *ueCb;
#endif /* ANSI */
{
   U8   rbCount = NH_ZERO_VAL;
   U32  ulBitMapSize = 0;
   S16  rohcProfIndex;
   CpjCfgEnt  *cpjParam = NULLP;

   TRC2(nhPIMFillPdcpRvrtModCfgParam)
   RLOG0(L_DEBUG, "nhPIMFillPdcpRvrtModCfgParam");

   rbCount = cpjCfgReq->numEnt;
   cpjParam = &cpjCfgReq->cfgEnt[rbCount];

   if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
   {
      /* If RB is SRB1 and security activation is completed, setting  *
       * discReqd to TRUE requesting PDCP to discard PDU if integrity *
       * fails.                                                       */
      if( (NH_RB_ID_SRB1 == rbCfgInfo->rbId.rbId) && 
          (CM_LTE_DRB != rbCfgInfo->rbId.rbTyp) &&
          (ueCb->rbSecCfgBitMask &  NH_SEC_CMPLTD) ) 
      {
         cpjParam->m.modCfg.bitFlag |= CPJ_RECFG_DISRQD;
         cpjParam->m.modCfg.discReqd = TRUE;
      }

      /* Fill CfmReqd parameter */
      if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_CFM_REQD)
      {
         cpjParam->m.modCfg.bitFlag |= CPJ_RECFG_CFMREQ;
         cpjParam->m.modCfg.cfmReqd = rbCfgInfo->pdcpCfg.cfmReqd;
      }
      /* Fill Header compression parameter */
      if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk &  NHU_PDCP_HDR_COMPR_INFO)
      {
         cpjParam->m.modCfg.hdrCompCfg.rohcInfo.maxCid = 
            rbCfgInfo->pdcpCfg.hdrComprInfo.rohcInfo.maxCid;

         for (rohcProfIndex = 0; rohcProfIndex < NHU_MAX_ROHC_PROF_LST_LEN; 
                                 rohcProfIndex++)
         {
             cpjParam->m.modCfg.hdrCompCfg.rohcInfo.profLst[rohcProfIndex] = 
                   rbCfgInfo->pdcpCfg.hdrComprInfo.rohcInfo.rohcProfileList[
                                 rohcProfIndex];
         }
      }
      /* Fill HO Information */
      if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_SDU_STA_INFO)
      {
         if(rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.isHoPres & NHU_HO_DL)
         {
            cpjParam->m.modCfg.hoInfo.isHoPres |= CPJ_HO_DL;
            cpjParam->m.modCfg.hoInfo.dlCount = 
                     rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.dlCountVal;
         }
         if(rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.isHoPres & NHU_HO_UL)
         {
            cpjParam->m.modCfg.hoInfo.isHoPres |= CPJ_HO_UL;
            cpjParam->m.modCfg.hoInfo.ulCount = 
                     rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.ulCountVal;

            ulBitMapSize = rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.numBits / 
                           NH_OCTET_VAL;

            if(ulBitMapSize % NH_OCTET_VAL)
            {
                ulBitMapSize++;
            }

            /* Allocate memory and store ul bitmap */
            NH_ALLOC(NH_REG, NH_POOL, &cpjParam->m.modCfg.hoInfo.ulBitMap, 
                     sizeof(U8)*ulBitMapSize);

            if(NULLP != cpjParam->m.modCfg.hoInfo.ulBitMap)
            {
               cpjParam->m.modCfg.hoInfo.numBits = 
                            rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.numBits;
               NH_MEM_COPY((U8*)cpjParam->m.modCfg.hoInfo.ulBitMap,
                           (U8*)rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.ulBitMap,
                           ulBitMapSize);
            }
            else
            {
               RLOG0(L_FATAL, "Memory Allocation failed for ulBitMap."); 
               RETVALUE(RFAILED);
            }
         } /* end of cpjParam->m.modCfg.hoInfo.isHoPres & CPJ_HO_UL */
      } /* end of rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_SDU_STA_INFO */
   }
   cpjCfgReq->numEnt++; 
   RETVALUE(ROK);
}/* End of nhPIMFillPdcpRvrtModCfgParam */

/**
 *
 *   @brief Handler to fill PDCP configuration info for PDCP AddCfgEnt.
 *
 *   @details
 *   This function is used to fill PDCP configuration   
 *   data to PDCP interface structure. This is used to fill
 *   PDCP configuration data to be added.
 *            
 *   @param[in] ueCb  UE Control block
 *   @param[out] cpjCfgReq      PDCP configuration parameter
 *   @param[in] rbCfgListInfo  RB info list
 *
 *   @return  S16
 *    -# ROK 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillAddPdcpCfgParam
(
NhActvUeCb *ueCb,
CpjCfgReqInfo *cpjCfgReq,
NhuRbCfgList  *rbCfgListInfo
)
#else /* ANSI */
PRIVATE S16 nhPIMFillAddPdcpCfgParam(ueCb, cpjCfgReq, rbCfgListInfo)
NhActvUeCb *ueCb;
CpjCfgReqInfo *cpjCfgReq;
NhuRbCfgList  *rbCfgListInfo;
#endif /* ANSI */
{
   U8   rbCount = NH_ZERO_VAL;
   U8   i = NH_ZERO_VAL;
   U8   rohcProfIndex = NH_ZERO_VAL;
   NhuRbCfgInfo *rbCfgInfo = NULLP;
   CpjCfgEnt  *cpjParam = NULLP;
   
   TRC2(nhPIMFillAddPdcpCfgParam)

   RLOG0(L_DEBUG, "nhPIMFillAddPdcpCfgParam");

   rbCount = cpjCfgReq->numEnt;
   for ( i=0; i < rbCfgListInfo->numOfRbCfgInfo; i++)
   {
      rbCfgInfo = &rbCfgListInfo->rbCfgInfoList[i];
      if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
      {      
          cpjParam = &cpjCfgReq->cfgEnt[rbCount];
     
          cpjParam->cfgType = CPJ_CFG_ADD;
          cpjParam->rbId = rbCfgInfo->rbId.rbId;
          cpjParam->rbType = rbCfgInfo->rbId.rbTyp;
      
          if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL)
          {
             cpjParam->m.addCfg.rlcInfo.dir |= NHU_RB_CFG_UL;
             cpjParam->m.addCfg.rlcInfo.mode = 
                       rbCfgInfo->ulRbCfg.ulRlcMode.mode;
          }
          if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL)
          {
             cpjParam->m.addCfg.rlcInfo.dir |= NHU_RB_CFG_DL;
             cpjParam->m.addCfg.rlcInfo.mode = 
                       rbCfgInfo->dlRbCfg.dlRlcMode.mode;
          }
         /* Fill Discard Timer value */
         if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk &
             NHU_PDCP_DISCARD_TMR)
         {
            cpjParam->m.addCfg.discardTmr =  rbCfgInfo->pdcpCfg.discardTmr;
         }
         if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk &  NHU_PDCP_SN_SIZE)
         {
            if (cpjParam->m.addCfg.rlcInfo.mode == NHU_RLC_MODE_UM)
            {
               /* Fill PDCP sequence size parameter */
               cpjParam->m.addCfg.rlcInfo.m.rlcUm.pdcpSNSize = 
                rbCfgInfo->pdcpCfg.snSize;
            }
            else if (cpjParam->m.addCfg.rlcInfo.mode == NHU_RLC_MODE_AM)
            {
               /* Fill PDCP sequence size parameter */
               cpjParam->m.addCfg.rlcInfo.m.rlcAm.pdcpSNSize = 
                rbCfgInfo->pdcpCfg.snSize;
            }
         }
         if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_STATUS_RPT)
         {
            if (cpjParam->m.addCfg.rlcInfo.mode == NHU_RLC_MODE_AM)
            {
               /* Fill status report required parameter */
               cpjParam->m.addCfg.rlcInfo.m.rlcAm.statusRepReqd = 
                rbCfgInfo->pdcpCfg.statusRptReq;
            }
         }

         /* Fill Header compression parameter */
         if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk &  NHU_PDCP_HDR_COMPR_INFO)
         {
            cpjParam->m.addCfg.hdrCompCfg.hdrCompUsed = NH_USED;
            cpjParam->m.addCfg.hdrCompCfg.rohcInfo.maxCid = 
               rbCfgInfo->pdcpCfg.hdrComprInfo.rohcInfo.maxCid;

             for (rohcProfIndex = 0; rohcProfIndex < NHU_MAX_ROHC_PROF_LST_LEN;
                  rohcProfIndex++)
             {
                cpjParam->m.addCfg.hdrCompCfg.rohcInfo.profLst[rohcProfIndex] = 
                   rbCfgInfo->pdcpCfg.hdrComprInfo.rohcInfo.rohcProfileList[
                                                             rohcProfIndex];
             }

         } /* if(rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_HDR_COMPR_INFO)*/

         /* Requesting PDCP to discard PDUs if Integrity Verification fails */
         cpjParam->m.addCfg.discReqd = TRUE;

         /* If RB is SRB1 and security activation is not completed, requesting *
          * PDCP not to discard PDU by setting discReqd flag to FALSE         */
         if(!(ueCb->rbSecCfgBitMask &  NH_SEC_CMPLTD) &&
             (NH_RB_ID_SRB1 == rbCfgInfo->rbId.rbId) && 
             (CM_LTE_DRB != rbCfgInfo->rbId.rbTyp) ) 
         {
            cpjParam->m.addCfg.discReqd = FALSE;
         }

         /* Fill CfmReqd parameter */
         if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk &  NHU_PDCP_CFM_REQD)
         {
            cpjParam->m.addCfg.cfmReqd = rbCfgInfo->pdcpCfg.cfmReqd;
         }
         /*nh003.301: Filling the L2 Measurement related parameters*/
#ifdef LTE_L2_MEAS
         cpjParam->m.addCfg.qci = rbCfgInfo->qci;
#endif /* LTE_L2_MEAS */
         rbCount++;
      } /* if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO) */
   }   /* for ( i=0; i < rbCfgListInfo->numOfRbCfgInfo; i++) */
   cpjCfgReq->numEnt =  rbCount;
   if ( rbCount != NH_ZERO_VAL )
   {
      cpjCfgReq->isHo  = rbCfgListInfo->isHO;
   }
   RETVALUE(ROK);
}/* End of nhPIMFillAddPdcpCfgParam */

/**
 *
 *   @brief Handler to fill PDCP configuration info for PDCP ModCfgEnt.
 *
 *   @details
 *   This function is used to fill PDCP configuration   
 *   data to PDCP interface structure. This is used to
 *   fill PDCP configuration data to be modified.
 *            
 *   @param[in] ueCb  UE Control block
 *   @param[out] cpjCfgReq      PDCP configuration parameter
 *   @param[in] rbCfgListInfo  RB info list
 *
 *   @return  S16
 *    -# ROK 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillModPdcpCfgParam
(
NhActvUeCb *ueCb,
CpjCfgReqInfo *cpjCfgReq,
NhuRbCfgList  *rbCfgListInfo
)
#else /* ANSI */
PRIVATE S16 nhPIMFillModPdcpCfgParam(ueCb, cpjCfgReq, rbCfgListInfo)
NhActvUeCb *ueCb;
CpjCfgReqInfo *cpjCfgReq;
NhuRbCfgList  *rbCfgListInfo;
#endif /* ANSI */
{
   U8   rbCount = NH_ZERO_VAL;
   U8   i = NH_ZERO_VAL;
   U8   rohcProfIndex = NH_ZERO_VAL;
   NhuRbCfgInfo *rbCfgInfo = NULLP;
   CpjCfgEnt  *cpjParam = NULLP;
   Size       ulBitMapSize;
   
   TRC2(nhPIMFillModPdcpCfgParam)
  
   RLOG0(L_DEBUG, "nhPIMFillModPdcpCfgParam");


   rbCount = cpjCfgReq->numEnt;
   for ( i=0; i < rbCfgListInfo->numOfRbCfgInfo; i++)
   {
      rbCfgInfo = &rbCfgListInfo->rbCfgInfoList[i];
      if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO)
      {      
          cpjParam = &cpjCfgReq->cfgEnt[rbCount];
    
          cpjParam->cfgType = CPJ_CFG_MODIFY;
          cpjParam->rbId = rbCfgInfo->rbId.rbId;
          cpjParam->rbType = rbCfgInfo->rbId.rbTyp; 
         /* Fill Header compression parameter */
         if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk &  NHU_PDCP_HDR_COMPR_INFO)
         {
            cpjParam->m.modCfg.hdrCompCfg.hdrCompUsed = NH_USED;
            cpjParam->m.modCfg.hdrCompCfg.rohcInfo.maxCid = 
               rbCfgInfo->pdcpCfg.hdrComprInfo.rohcInfo.maxCid;

             for (rohcProfIndex = 0; rohcProfIndex < NHU_MAX_ROHC_PROF_LST_LEN;
                                     rohcProfIndex++)
             {
                cpjParam->m.modCfg.hdrCompCfg.rohcInfo.profLst[rohcProfIndex] = 
                   rbCfgInfo->pdcpCfg.hdrComprInfo.rohcInfo.rohcProfileList[
                                     rohcProfIndex];
             }

         } /* if(rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_HDR_COMPR_INFO)*/

         /* Fill HO Information */
         if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_SDU_STA_INFO)
         {
            if(rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.isHoPres & NHU_HO_DL)
            {
               cpjParam->m.modCfg.hoInfo.isHoPres |= CPJ_HO_DL;
               cpjParam->m.modCfg.hoInfo.dlCount = 
                                rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.dlCountVal;
               cpjParam->m.modCfg.hoInfo.snLen = rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.snLen;
            }
            if(rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.isHoPres & NHU_HO_UL)
            {
               cpjParam->m.modCfg.hoInfo.isHoPres |= CPJ_HO_UL;
               cpjParam->m.modCfg.hoInfo.ulCount = 
                                rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.ulCountVal;
               cpjParam->m.modCfg.hoInfo.snLen = rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.snLen;
                
               /*nh002.302:121151: fixed zero memory alloc issue, numBits
                * can be zero since "hoPdcpSduStaInfo" is a optinal parameter
                */ 
               if (rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.numBits)
               {
                  ulBitMapSize = rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.numBits / 
                     NH_OCTET_VAL;

                  if(ulBitMapSize % NH_OCTET_VAL)
                  {
                     ulBitMapSize++;
                  }

                  /* Allocate memory and store ul bitmap */
                  NH_ALLOC(NH_REG, NH_POOL, &cpjParam->m.modCfg.hoInfo.ulBitMap, 
                        sizeof(U8)*ulBitMapSize);

                  if(NULLP != cpjParam->m.modCfg.hoInfo.ulBitMap)
                  {
                     cpjParam->m.modCfg.hoInfo.numBits = 
                        rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.numBits;
                     NH_MEM_COPY((U8*)cpjParam->m.modCfg.hoInfo.ulBitMap,
                           (U8*)rbCfgInfo->pdcpCfg.hoPdcpSduStaInfo.ulBitMap,
                           ulBitMapSize);
                  }
                  else
                  {
                     RLOG0(L_FATAL, "Memory Allocation failed for ulBitMap."); 
                     RETVALUE(RFAILED);
                  }
               } /* 0 != */
            } /* end of cpjParam->m.modCfg.hoInfo.isHoPres & CPJ_HO_UL */
         } /* end of rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_SDU_STA_INFO */

         /* If RB is SRB1 and security activation is completed, setting  *
          * discReqd to TRUE requesting PDCP to discard PDU if integrity *
          * fails.                                                       */
         if( (NH_RB_ID_SRB1 == rbCfgInfo->rbId.rbId) && 
             (CM_LTE_DRB != rbCfgInfo->rbId.rbTyp) &&
             (ueCb->rbSecCfgBitMask &  NH_SEC_CMPLTD) ) 
         {
            cpjParam->m.modCfg.bitFlag |= CPJ_RECFG_DISRQD;
            cpjParam->m.modCfg.discReqd = TRUE;
         }

         /* Fill CfmReqd parameter */
         if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk &  NHU_PDCP_CFM_REQD)
         {
            cpjParam->m.modCfg.bitFlag |= CPJ_RECFG_CFMREQ;
            cpjParam->m.modCfg.cfmReqd = rbCfgInfo->pdcpCfg.cfmReqd;
         }
         rbCount++;
      } /* if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO) */
   }   /* for ( i=0; i < rbCfgListInfo->numOfRbCfgInfo; i++) */
   cpjCfgReq->numEnt =  rbCount;

   RETVALUE(ROK);
}/* End of nhPIMFillModPdcpCfgParam */


/**
 *
 *   @brief Handler to release complete configuration at MAC, RLC, and PDCP
 *   for an UE.
 *
 *   @details
 *   This function is used to release complete        
 *   configuration at the lower layer (MAC/RLC/PDCP) for an UE.
 *            
 *
 *  @param[in] ueCb  UE control block
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRlcPdcpMacRlsCfg 
(
NhActvUeCb *ueCb
)
#else /* ANSI */
PUBLIC S16 nhPIMRlcPdcpMacRlsCfg(ueCb)
NhActvUeCb *ueCb;
#endif /* ANSI */
{
   CrgCfgReqInfo       *crgCfgParam = NULLP; /* MAC Configuration Request parameter */
   CkwCfgInfo          *ckwCfgParam = NULLP; /* RLC Configuration request parameter */
   CpjCfgReqInfo       *cpjCfgReq = NULLP;   /* PDCP Configuration Request parameter */
   CrgCfgTransId       crgCfgTransId;        /* MAC Transaction ID */
   Pst                 tmpPst;               /* Post structure */ 
   NhLSapCb            *lSap = NULLP;        /* Lower SAP Cb */
   SuId                suId = NH_ZERO_VAL; 
   U32                 transId = NH_RESERVED_TRANS_ID;

   TRC2(nhPIMRlcPdcpMacRlsCfg)
   RLOG0(L_DEBUG, "nhPIMRlcPdcpMacRlsCfg");

   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCRGSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
       
   }

   tmpPst = lSap->pst;
   
   /* Allocate memory for cfg Parameter */
   NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
   if (NH_CHK_INPUT_PARAM(crgCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL crgCfgParam");
      RETVALUE(RFAILED);
   }
   NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));

   /* Delete configuration */
   crgCfgParam->action = CRG_DELETE;
   crgCfgParam->u.delInfo.delType = CRG_UE_CFG;
   crgCfgParam->u.delInfo.u.ueDel.cellId = ueCb->cellCb->cellId;
   crgCfgParam->u.delInfo.u.ueDel.crnti = ueCb->ueCbKey.crnti;

   /* Construct transaction ID */
   NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, ueCb->cellCb->cellId,
      ueCb->ueCbKey.crnti, transId, NH_CRG_UE_CFG, 
      NH_CFG_ACTN_RLS, NH_ZERO_VAL, NH_ZERO_VAL);

   /* Send Configuration to MAC layer */
   if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, crgCfgParam))
   {
      RLOG0(L_ERROR,"Failed to send data to lower layer");
      RETVALUE(RFAILED);
   }
   
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCKWSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCKWSAP, suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
       
   }

   tmpPst = lSap->pst;

   /* Allocate memory for cfg Parameter */
   NH_ALLOC(NH_REG, NH_POOL, &ckwCfgParam, sizeof(CkwCfgInfo));
   if (NH_CHK_INPUT_PARAM(ckwCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL ckwCfgParam");
      RETVALUE(RFAILED);
   }

   ckwCfgParam->transId = transId;
   ckwCfgParam->ueId = ueCb->ueCbKey.crnti;
   ckwCfgParam->cellId = ueCb->cellCb->cellId;
   ckwCfgParam->entCfg[0].cfgType = CKW_CFG_DELETE_UE;
   ckwCfgParam->numEnt += 1;
   
   /* Send Configuration to RLC layer */
   if (ROK != NhLiCkwCfgReq(&tmpPst, lSap->spId, ckwCfgParam))
   {
      RLOG0(L_ERROR,"Failed to send data to lower layer");
      RETVALUE(RFAILED);
   }
   
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCPJSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;

   /* Allocate memory for cfg Parameter */
   NH_ALLOC(NH_REG, NH_POOL, &cpjCfgReq, sizeof(CpjCfgReqInfo));
   if (NH_CHK_INPUT_PARAM(cpjCfgReq))
   {
      RLOG0(L_FATAL, "GOT NULL cpjCfgReq");
      RETVALUE(RFAILED);
   }
   
   cpjCfgReq->cellId  = ueCb->cellCb->cellId; 
   cpjCfgReq->ueId  = ueCb->ueCbKey.crnti; 
   cpjCfgReq->transId  = transId; 
   cpjCfgReq->cfgEnt[0].cfgType = CPJ_CFG_DELETE_UE;
   cpjCfgReq->numEnt += 1;
   
   /* Send Configuration to PDCP layer */
   if (ROK != NhLiCpjCfgReq(&tmpPst, lSap->spId, cpjCfgReq))
   {
      RLOG0(L_ERROR,"Failed to send data to lower layer");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of nhPIMRlcPdcpMacRlsCfg */

/**
 *
 *   @brief Handler to fill PDCP re-establishmenr/security 
 *   parameter for SRB1.
 *
 *   @details
 *   This function is used to fills new security keys. 
 *            
 *
 *   @param[out] cpjCfgParam  PDCP configuration parameter  
 *   @param[in]  secCfg       Security configuration info 
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillPdcpRestbSRB1Cfg 
(
CpjReEstReqInfo     *cpjCfgParam, 
NhuSecurityCfg      *secCfg
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPdcpRestbSRB1Cfg( cpjCfgParam, secCfg)
CpjReEstReqInfo     *cpjCfgParam;
NhuSecurityCfg      *secCfg;
#endif /* ANSI */
{
   
   TRC2(nhPIMFillPdcpRestbSRB1Cfg)
   
   NH_MEM_COPY(cpjCfgParam->intKey,secCfg->intgCfgInfo.intgKey,
                   CPJ_INTG_KEY_LEN);
   NH_MEM_COPY(cpjCfgParam->ctrlplnKey,secCfg->cipherCfgInfo.cipherCpKey,
                   CPJ_CIPHER_KEY_LEN);
   NH_MEM_COPY(cpjCfgParam->usrplnKey,secCfg->cipherCfgInfo.cipherUpKey,
                   CPJ_CIPHER_KEY_LEN);

   RETVALUE(ROK);
}  /* end of nhPIMFillPdcpRestbSRB1Cfg */

/**
 *
 *   @brief Handler to send Security Cfg request to lower layers
 *
 *   @details
 *   This function is used to send Security Cfg request to lower layers (PDCP)
 *
 *   @param[in] cpjSecConfigReqInfo Cpj Cfg paramters
 *   @param[in] secCfg   Nhu Sec cfg paramters
 *   @param[in] selSecMode  Selective Security Mode Configuration enbl/disbl info
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 nhPIMFillPdcpSecCfg 
(
CpjSecCfgReqInfo    *cpjSecConfigReqInfo,
NhuSecurityCfg      *secCfg,
U8 selSecMode
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPdcpSecCfg (cpjSecConfigReqInfo, secCfg, selSecMode)
CpjSecCfgReqInfo    *cpjSecConfigReqInfo;
NhuSecurityCfg      *secCfg;
U8 selSecMode;
#endif /* ANSI */
{
   TRC2(nhPIMFillPdcpSecCfg)

   RLOG0(L_DEBUG, "nhPIMFillPdcpSecCfg");

   /* Fill ordered Integrity Cfg Information */
   switch(secCfg->intgCfgInfo.intgAlgo)
   {
#ifdef LTERRC_REL9
      case NHU_INTG_ALGO_EIA0: /* R9 upgrade: Added to support NULL IP */
         cpjSecConfigReqInfo->secCfg.integInfo.algoType = CPJ_SEC_ALG_NULL;
         break;
#endif /* LTERRC_REL9 */
      case NHU_INTG_ALGO_EIA1:
         cpjSecConfigReqInfo->secCfg.integInfo.algoType = CPJ_SEC_ALG_SNOW3G;
         break;
      case NHU_INTG_ALGO_EIA2:
         cpjSecConfigReqInfo->secCfg.integInfo.algoType = CPJ_SEC_ALG_AES;
         break;
/* ZUC_ALGO*/
      case NHU_INTG_ALGO_EIA3:
         cpjSecConfigReqInfo->secCfg.integInfo.algoType = CPJ_SEC_ALG_ZUC;
         break;
/* ZUC_ALGO*/
      default:
         RLOG0(L_DEBUG, "Invalid intg Algo");
         RETVALUE(RFAILED);
         break;
   }
   NH_MEM_COPY(cpjSecConfigReqInfo->secCfg.integInfo.intKey,
               secCfg->intgCfgInfo.intgKey, CPJ_INTG_KEY_LEN);

   /* Fill ordered Ciphering Cfg Information */
   switch(secCfg->cipherCfgInfo.ciphAlgo)
   {
      case NHU_CIPHER_ALGO_EEA0:
         cpjSecConfigReqInfo->secCfg.cipherInfo.algoType = CPJ_SEC_ALG_NULL;
         break;
      case NHU_CIPHER_ALGO_EEA1:
         cpjSecConfigReqInfo->secCfg.cipherInfo.algoType = CPJ_SEC_ALG_SNOW3G;
         break;
      case NHU_CIPHER_ALGO_EEA2:
         cpjSecConfigReqInfo->secCfg.cipherInfo.algoType = CPJ_SEC_ALG_AES;
         break;
/* ZUC_ALGO */
      case NHU_CIPHER_ALGO_EEA3:
         cpjSecConfigReqInfo->secCfg.cipherInfo.algoType = CPJ_SEC_ALG_ZUC;
         break;
/* ZUC_ALGO */
      default:
         RLOG0(L_DEBUG, "Invalid ciph Algo");
         RETVALUE(RFAILED);
         break;
   }

   NH_MEM_COPY(cpjSecConfigReqInfo->secCfg.cipherInfo.ctrlplnKey,
               secCfg->cipherCfgInfo.cipherCpKey, CPJ_CIPHER_KEY_LEN);
   NH_MEM_COPY(cpjSecConfigReqInfo->secCfg.cipherInfo.usrplnKey,
               secCfg->cipherCfgInfo.cipherUpKey, CPJ_CIPHER_KEY_LEN);

   /* Selective Security Mode Configuration */
   cpjSecConfigReqInfo->secCfg.selSecAct = FALSE;
   if(selSecMode == NH_SEL_SEC_MOD_CFG_ENABLE)
   {
      cpjSecConfigReqInfo->secCfg.selSecAct = TRUE;
   }

   RETVALUE(ROK);

} /* nhPIMFillPdcpSecCfg */

/**
 *
 *   @brief Function to fill RLC Revert configuration parameter.
 *
 *   @details
 *   This function is used to fill RLC Revert configuration parameter.
 *            
 *
 *  @param[out] ckwCfgParam  RLC configuration parameter
 *  @param[in]  rbCfgInfo    RB configuration info
 *  @param[in]  cfgType      Configuration type
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillRlcRvrtCfgParam 
(
CkwCfgInfo     *ckwCfgParam,
NhuRbCfgInfo   *rbCfgInfo,
U8             cfgType 
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRlcRvrtCfgParam(ckwCfgParam, rbCfgInfo, cfgType ) 
CkwCfgInfo     *ckwCfgParam;
NhuRbCfgInfo   *rbCfgInfo;
U8             cfgType;
#endif /* ANSI */
{
   U8   rbCount = NH_ZERO_VAL;

   TRC2(nhPIMFillRlcRvrtCfgParam)

   
   rbCount = ckwCfgParam->numEnt;
   
   ckwCfgParam->entCfg[rbCount].cfgType = cfgType;
   ckwCfgParam->entCfg[rbCount].rbId = rbCfgInfo->rbId.rbId;
   ckwCfgParam->entCfg[rbCount].rbType = rbCfgInfo->rbId.rbTyp;
  
   /* Up Link information */
   if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL)
   {
      ckwCfgParam->entCfg[rbCount].dir = CKW_CFG_DIR_UL;
      if (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_RLC_MODE_UL)
      {
         ckwCfgParam->entCfg[rbCount].entMode = 
                      rbCfgInfo->ulRbCfg.ulRlcMode.mode;
         
         switch(rbCfgInfo->ulRbCfg.ulRlcMode.mode)
         {
            case NHU_RLC_MODE_AM:
                 ckwCfgParam->entCfg[rbCount].m.amInfo.ul.staProhTmr =
                 rbCfgInfo->ulRbCfg.ulRlcMode.m.amRlc.tStatusProh;
                 ckwCfgParam->entCfg[rbCount].m.amInfo.ul.reOrdTmr =
                 rbCfgInfo->ulRbCfg.ulRlcMode.m.amRlc.tReordring;
                 if ( rbCfgInfo->ulRbCfg.ulRbBitMsk &
                       NHU_RB_CFG_LOGCH_MAP_UL)
                 {
                    ckwCfgParam->entCfg[rbCount].lCh[1].lChId =
                    rbCfgInfo->lcId.lcId;
                    ckwCfgParam->entCfg[rbCount].lCh[1].type =
                    rbCfgInfo->lcId.logChType;
                 }
               break;

            case NHU_RLC_MODE_UM:
                 ckwCfgParam->entCfg[rbCount].m.umInfo.ul.snLen =
                 rbCfgInfo->ulRbCfg.ulRlcMode.m.umRlc.snFieldLen;
                 /* nh001.301 :Corrected filling of UM mode Rerodering Timer */
                 ckwCfgParam->entCfg[rbCount].m.umInfo.ul.reOrdTmr =
                 rbCfgInfo->ulRbCfg.ulRlcMode.m.umRlc.tReordring;
                 if ( rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)
                 {
                    ckwCfgParam->entCfg[rbCount].lCh[0].lChId =
                    rbCfgInfo->lcId.lcId;
                    ckwCfgParam->entCfg[rbCount].lCh[0].type =
                    rbCfgInfo->lcId.logChType;
                 }
               
               break;

            case NHU_RLC_MODE_TM:
                 if ( rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)
                 {
                    ckwCfgParam->entCfg[rbCount].lCh[0].lChId =
                    rbCfgInfo->lcId.lcId;
                    ckwCfgParam->entCfg[rbCount].lCh[0].type =
                    rbCfgInfo->lcId.logChType;
                 }
               break;

            default:
                 RLOG0(L_ERROR,"No default mode of RLC");
                 RETVALUE(RFAILED);
        }

      }
   }
   /* Down Link information */
   if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL)
   {
      ckwCfgParam->entCfg[rbCount].dir = CKW_CFG_DIR_DL;
      if (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_DL)
      {
         ckwCfgParam->entCfg[rbCount].entMode = 
                      rbCfgInfo->dlRbCfg.dlRlcMode.mode;
         
         switch(rbCfgInfo->dlRbCfg.dlRlcMode.mode)
         {
            case NHU_RLC_MODE_AM:
                 ckwCfgParam->entCfg[rbCount].m.amInfo.dl.pollRetxTmr =
                 rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.tPollRetx;
                 /* nh004.201 - Correction to pollPdu and pollByte values
                                as expected at CKW interface. */
                 if (NHU_POLL_PDU_INF == rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollPdu)
                 {
                    ckwCfgParam->entCfg[rbCount].m.amInfo.dl.pollPdu = (S16)0xFFFF;
                 }
                 else
                 {
                    ckwCfgParam->entCfg[rbCount].m.amInfo.dl.pollPdu =
                    rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollPdu;
                 }

                 /* RLC expects pollByte in bytes. NHU gives it in KBytes.
                    Multiple by 1024. */
                 if (NHU_POLL_BYTE_INF == rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollByte)
                 {
                    ckwCfgParam->entCfg[rbCount].m.amInfo.dl.pollByte = 0xFFFFFFFF;
                 }
                 else
                 {
                    ckwCfgParam->entCfg[rbCount].m.amInfo.dl.pollByte =
                    (rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollByte << 10);
                 }
                 ckwCfgParam->entCfg[rbCount].m.amInfo.dl.maxRetx =
                 rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.maxRetxThresh;
                 if ( rbCfgInfo->dlRbCfg.dlRbBitMsk &
                       NHU_RB_CFG_LOGCH_MAP_DL)
                 {
                    ckwCfgParam->entCfg[rbCount].lCh[0].lChId =
                    rbCfgInfo->lcId.lcId;
                    ckwCfgParam->entCfg[rbCount].lCh[0].type =
                    rbCfgInfo->lcId.logChType;
                 }
               break;

            case NHU_RLC_MODE_UM:
                 ckwCfgParam->entCfg[rbCount].m.umInfo.dl.snLen =
                 rbCfgInfo->dlRbCfg.dlRlcMode.m.umRlc.snFieldLen;
                 if ( rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_LOGCH_MAP_DL)
                 {
                    ckwCfgParam->entCfg[rbCount].lCh[0].lChId =
                    rbCfgInfo->lcId.lcId;
                    ckwCfgParam->entCfg[rbCount].lCh[0].type =
                    rbCfgInfo->lcId.logChType;
                 }
               
               break;

            case NHU_RLC_MODE_TM:
                 ckwCfgParam->entCfg[rbCount].m.tmInfo.dl.buffSize =
                 rbCfgInfo->dlRbCfg.dlRlcMode.m.tmRlc.bufferSize;
                 if ( rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_LOGCH_MAP_DL)
                 {
                    ckwCfgParam->entCfg[rbCount].lCh[0].lChId =
                    rbCfgInfo->lcId.lcId;
                    ckwCfgParam->entCfg[rbCount].lCh[0].type =
                    rbCfgInfo->lcId.logChType;
                 }
               break;

            default:
                 RLOG0(L_ERROR,"No default mode of RLC");
                 RETVALUE(RFAILED);
         }
      }
   }
  
   /* Increment the number of entity */
   ckwCfgParam->numEnt++;
   
   RETVALUE(ROK);
}/* End of nhPIMFillRlcRvrtCfgParam */

/**
 *
 *   @brief Function to fill RLC configuration parameter.
 *
 *   @details
 *   This function is used to fill RLC configuration parameter.
 *            
 *  @param[out] ckwCfgParam   RLC configuration parameter
 *  @param[in]  rbCfgInfo  RB configuration information
 *  @param[in]  rbCount     Number of RBs
 *
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillRlcDlCfgParam 
(
CkwCfgInfo     *ckwCfgParam,
NhuRbCfgInfo   *rbCfgInfo,
U8              rbCount
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRlcDlCfgParam(ckwCfgParam, rbCfgInfo, rbCount) 
CkwCfgInfo     *ckwCfgParam;
NhuRbCfgInfo   *rbCfgInfo;
U8              rbCount;
#endif /* ANSI */
{
   TRC2(nhPIMFillRlcDlCfgParam)

   RLOG0(L_DEBUG, "nhPIMFillRlcDlCfgParam");
   switch(rbCfgInfo->dlRbCfg.dlRlcMode.mode)
   {
      case NHU_RLC_MODE_AM:
           ckwCfgParam->entCfg[rbCount].m.amInfo.dl.pollRetxTmr =
           rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.tPollRetx;
           /* nh004.201 - Correction to pollPdu and pollByte values
                          as expected at CKW interface. */
           if (NHU_POLL_PDU_INF == rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollPdu)
           {
              rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollPdu = (NhuPollPdu)0xFFFF;
           }
           else
           {
              ckwCfgParam->entCfg[rbCount].m.amInfo.dl.pollPdu =
              rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollPdu;
           }
           /* RLC expects the pollByte in KiloBytes. NHU gives it in Bytes.
              Multiply by 1024. */
           if (NHU_POLL_BYTE_INF == rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollByte)
           {
              ckwCfgParam->entCfg[rbCount].m.amInfo.dl.pollByte = 0xFFFFFFFF;
           }
           else
           {
              ckwCfgParam->entCfg[rbCount].m.amInfo.dl.pollByte =
              (rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.pollByte << 10);
           }
           ckwCfgParam->entCfg[rbCount].m.amInfo.dl.maxRetx =
           rbCfgInfo->dlRbCfg.dlRlcMode.m.amRlc.maxRetxThresh;
           if ( rbCfgInfo->dlRbCfg.dlRbBitMsk &
                 NHU_RB_CFG_LOGCH_MAP_DL)
           {
              ckwCfgParam->entCfg[rbCount].lCh[0].lChId =
              rbCfgInfo->lcId.lcId;
              ckwCfgParam->entCfg[rbCount].lCh[0].type =
              rbCfgInfo->lcId.logChType;
           }
         break;

      case NHU_RLC_MODE_UM:
           ckwCfgParam->entCfg[rbCount].m.umInfo.dl.snLen =
           rbCfgInfo->dlRbCfg.dlRlcMode.m.umRlc.snFieldLen;
           if ( rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_LOGCH_MAP_DL)
           {
              ckwCfgParam->entCfg[rbCount].lCh[0].lChId =
              rbCfgInfo->lcId.lcId;
              ckwCfgParam->entCfg[rbCount].lCh[0].type =
              rbCfgInfo->lcId.logChType;
           }
         
         break;

      case NHU_RLC_MODE_TM:
           ckwCfgParam->entCfg[rbCount].m.tmInfo.dl.buffSize =
           rbCfgInfo->dlRbCfg.dlRlcMode.m.tmRlc.bufferSize;
           if ( rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_LOGCH_MAP_DL)
           {
              ckwCfgParam->entCfg[rbCount].lCh[0].lChId =
              rbCfgInfo->lcId.lcId;
              ckwCfgParam->entCfg[rbCount].lCh[0].type =
              rbCfgInfo->lcId.logChType;
           }
         break;

      default:
           RLOG0(L_ERROR,"No default mode of RLC");
           RETVALUE(RFAILED);

   } /*  switch(rbCfgInfo->dlRbCfg.dlRlcMode.mode) */

   RETVALUE(ROK);
}/* End of nhPIMFillRlcDlCfgParam */
/**
 *
 *   @brief Function to fill RLC Cell configuration parameter.
 *
 *   @details
 *   This function is used to fill RLC cell configuration parameter.
 *            
 *  @param[out] ckwCfgParam   RLC config parameter.
 *  @param[in]  rbCfgInfo RB config information.
 *  @param[in]  rbCount       RB Index
 *
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillRlcUlCfgParam 
(
CkwCfgInfo     *ckwCfgParam,
NhuRbCfgInfo   *rbCfgInfo,
U8              rbCount
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRlcUlCfgParam(ckwCfgParam, rbCfgInfo, rbCount) 
CkwCfgInfo     *ckwCfgParam;
NhuRbCfgInfo   *rbCfgInfo;
U8              rbCount;
#endif /* ANSI */
{
   TRC2(nhPIMFillRlcUlCfgParam)
   RLOG0(L_DEBUG, "nhPIMFillRlcUlCfgParam");

   switch(rbCfgInfo->ulRbCfg.ulRlcMode.mode)
   {
      case NHU_RLC_MODE_AM:
           ckwCfgParam->entCfg[rbCount].m.amInfo.ul.staProhTmr =
           rbCfgInfo->ulRbCfg.ulRlcMode.m.amRlc.tStatusProh;
           ckwCfgParam->entCfg[rbCount].m.amInfo.ul.reOrdTmr =
           rbCfgInfo->ulRbCfg.ulRlcMode.m.amRlc.tReordring;
           if ( rbCfgInfo->ulRbCfg.ulRbBitMsk &
                 NHU_RB_CFG_LOGCH_MAP_UL)
           {
              ckwCfgParam->entCfg[rbCount].lCh[1].lChId =
              rbCfgInfo->lcId.lcId;
              ckwCfgParam->entCfg[rbCount].lCh[1].type =
              rbCfgInfo->lcId.logChType;
           }
         break;

      case NHU_RLC_MODE_UM:
           ckwCfgParam->entCfg[rbCount].m.umInfo.ul.snLen =
           rbCfgInfo->ulRbCfg.ulRlcMode.m.umRlc.snFieldLen;
           /* nh001.301 :Corrected filling of UM mode Rerodering Timer */
           ckwCfgParam->entCfg[rbCount].m.umInfo.ul.reOrdTmr =
           rbCfgInfo->ulRbCfg.ulRlcMode.m.umRlc.tReordring;
           if ( rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)
           {
              ckwCfgParam->entCfg[rbCount].lCh[1].lChId =
              rbCfgInfo->lcId.lcId;
              ckwCfgParam->entCfg[rbCount].lCh[1].type =
              rbCfgInfo->lcId.logChType;
           }
         
         break;

      case NHU_RLC_MODE_TM:
           if ( rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)
           {
              ckwCfgParam->entCfg[rbCount].lCh[0].lChId =
              rbCfgInfo->lcId.lcId;
              ckwCfgParam->entCfg[rbCount].lCh[0].type =
              rbCfgInfo->lcId.logChType;
           }
         break;

      default:
           RLOG0(L_ERROR,"No default mode of RLC");
           RETVALUE(RFAILED);
  }
  RETVALUE(ROK);

}/* End of nhPIMFillRlcUlCfgParam */
/**
 *
 *   @brief Function to fill RLC configuration parameter.
 *
 *   @details
 *   This function is used to fill RLC config parameter.
 *            
 *  @param[in]  transCtrlBlk       Transaction control block
 *  @param[out] ckwCfgParam   RLC config parameter.
 *
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillRlcRlsCfg
(
CmLListCp      *rbCfgInfoList,
TransCb        *transCtrlBlk,
CkwCfgInfo     *ckwCfgParam
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRlcRlsCfg(rbCfgInfoList, transCtrlBlk, ckwCfgParam) 
CmLListCp      *rbCfgInfoList;
TransCb        *transCtrlBlk;
CkwCfgInfo     *ckwCfgParam;
#endif /* ANSI */
{
   U8           rbCount = NH_ZERO_VAL;
   U8           i = NH_ZERO_VAL;
   NhuRbId        *rbIdPtr = NULLP;
   CmLList      *tmpNode =  NULLP;

   TRC2(nhPIMFillRlcRlsCfg)
   
   rbCount = ckwCfgParam->numEnt;
   for ( i=0; i < transCtrlBlk->ordrdCfg.rbToRlsCfgList.numOfRbCfgInfo; i++, 
                       rbCount++)
   {
      ckwCfgParam->entCfg[rbCount].cfgType = CKW_CFG_DELETE;
      rbIdPtr = &transCtrlBlk->ordrdCfg.rbToRlsCfgList.rbsToRel[i];
      if (rbIdPtr != NULLP)
      {
        ckwCfgParam->entCfg[rbCount].rbId = (*rbIdPtr).rbId;
        ckwCfgParam->entCfg[rbCount].rbType = (*rbIdPtr).rbTyp;
        ckwCfgParam->numEnt ++; 
        /* Dir_Fix: Added direction value as RLC works depending on Direction
         *         Provided by RRC
         * NOTE : TENB_SPLIT_ARCH_SUPPORT flag was present.Has been removed 
         * as part of  Common Code merge changes.*/
        CM_LLIST_FIRST_NODE(rbCfgInfoList, tmpNode);
        while(tmpNode)
        {
           if((((NhuRbCfgInfo *)tmpNode->node)->rbId.rbId == (*rbIdPtr).rbId) 
             &&(((NhuRbCfgInfo *)tmpNode->node)->rbId.rbTyp == rbIdPtr->rbTyp))
           {
              if ((((NhuRbCfgInfo *)tmpNode->node)->rbCfgBitMsk & NHU_RB_CFG_DL) &&
                    (((NhuRbCfgInfo *)tmpNode->node)->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_DL))
              {
                 ckwCfgParam->entCfg[rbCount].dir |= CKW_CFG_DIR_DL;
                 ckwCfgParam->entCfg[rbCount].entMode = 
                    ((NhuRbCfgInfo *)tmpNode->node)->dlRbCfg.dlRlcMode.mode;
              }
              if ((((NhuRbCfgInfo *)tmpNode->node)->rbCfgBitMsk & NHU_RB_CFG_UL) &&
                    (((NhuRbCfgInfo *)tmpNode->node)->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_UL))
              {
                 ckwCfgParam->entCfg[rbCount].dir |= CKW_CFG_DIR_UL;
                 ckwCfgParam->entCfg[rbCount].entMode = 
                    ((NhuRbCfgInfo *)tmpNode->node)->ulRbCfg.ulRlcMode.mode;
              }
           }
           CM_LLIST_NEXT_NODE(rbCfgInfoList, tmpNode);
        } /* while*/
      } /* if */
   } /* for */

   RETVALUE(ROK);
}/* End of nhPIMFillRlcRlsCfg */

/**
 *
 *   @brief Function to fill RLC re-establish configuration parameter.
 *
 *   @details
 *   This function is used to fill RLC config parameter.
 *            
 *  @param[in]  rbCfgInfoList RB config info list.
 *  @param[out] ckwCfgParam   RLC config parameter.
 *  @param[in]  skipSrb       Skip RBs.
 *
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillRlcRestbCfg
(
CmLListCp      *rbCfgInfoList,
CkwCfgInfo     *ckwCfgParam,
Bool           skipSrb
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRlcRestbCfg(rbCfgInfoList, ckwCfgParam, skipSrb) 
CmLListCp      *rbCfgInfoList;
CkwCfgInfo     *ckwCfgParam;
Bool           skipSrb;
#endif /* ANSI */
{
   U8           rbCount = NH_ZERO_VAL;
   CmLList      *tmpNode =  NULLP;
   Bool         isDrb = FALSE;

   TRC2(nhPIMFillRlcRestbCfg)

  /* get number of entity */
   rbCount = ckwCfgParam->numEnt;

   CM_LLIST_FIRST_NODE(rbCfgInfoList, tmpNode);
   while(tmpNode)
   { 
       ckwCfgParam->entCfg[rbCount].cfgType = CKW_CFG_REESTABLISH;
       isDrb = ((NhuRbCfgInfo *)tmpNode->node)->rbId.rbTyp; 
       if(((TRUE == skipSrb) && (CM_LTE_DRB == isDrb)) || (FALSE == skipSrb) )
       {
          ckwCfgParam->entCfg[rbCount].rbId = 
                  ((NhuRbCfgInfo *)tmpNode->node)->rbId.rbId;
          ckwCfgParam->entCfg[rbCount].rbType = 
                  ((NhuRbCfgInfo *)tmpNode->node)->rbId.rbTyp;
          /* HO_FIX: Added direction value as RLC works depending on Direction
           *         Provided by RRC
           */
          if ((((NhuRbCfgInfo *)tmpNode->node)->rbCfgBitMsk & NHU_RB_CFG_DL) &&
              (((NhuRbCfgInfo *)tmpNode->node)->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_DL))
          {
               ckwCfgParam->entCfg[rbCount].dir |= CKW_CFG_DIR_DL;
               ckwCfgParam->entCfg[rbCount].entMode = 
                  ((NhuRbCfgInfo *)tmpNode->node)->dlRbCfg.dlRlcMode.mode;
          }
          if ((((NhuRbCfgInfo *)tmpNode->node)->rbCfgBitMsk & NHU_RB_CFG_UL) &&
              (((NhuRbCfgInfo *)tmpNode->node)->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_UL))
          {
               ckwCfgParam->entCfg[rbCount].dir |= CKW_CFG_DIR_UL;
               ckwCfgParam->entCfg[rbCount].entMode = 
                  ((NhuRbCfgInfo *)tmpNode->node)->ulRbCfg.ulRlcMode.mode;
          }
     
          rbCount++; 
       }

       CM_LLIST_NEXT_NODE(rbCfgInfoList, tmpNode);
   }
   ckwCfgParam->numEnt = rbCount; 

   RETVALUE(ROK);
}/* End of nhPIMFillRlcRestbCfg */

/**
 *
 *   @brief Function to fill RLC configuration parameter.
 *
 *   @details
 *   This function is used to fill RLC config parameter.
 *            
 *  @param[out] ckwCfgParam   RLC config parameter.
 *  @param[in]  rbCfgListInfo RB config info list.
 *  @param[in]  cfgType       Configuration type.
 *
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillRlcCfgParam 
(
CkwCfgInfo     *ckwCfgParam,
NhuRbCfgList   *rbCfgListInfo,
U8             cfgType 
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRlcCfgParam(ckwCfgParam, rbCfgListInfo, cfgType ) 
CkwCfgInfo     *ckwCfgParam;
NhuRbCfgList   *rbCfgListInfo;
U8             cfgType;
#endif /* ANSI */
{
   U8           rbCount = NH_ZERO_VAL;
   U8           i = NH_ZERO_VAL;
   NhuRbCfgInfo *rbCfgInfo = NULLP;

   TRC2(nhPIMFillRlcCfgParam)
   RLOG0(L_DEBUG, "nhPIMFillRlcCfgParam");

  /* get number of entity */ 
   rbCount = ckwCfgParam->numEnt;
   for ( i=0; i < rbCfgListInfo->numOfRbCfgInfo; i++, rbCount++)
   {
      rbCfgInfo = &rbCfgListInfo->rbCfgInfoList[i];
      if (NULLP == rbCfgInfo)
      {
         RLOG0(L_ERROR,"NULLP for rbCfgInfo");
         RETVALUE(RFAILED);
      }
      ckwCfgParam->entCfg[rbCount].cfgType = cfgType;
      ckwCfgParam->entCfg[rbCount].rbId = rbCfgInfo->rbId.rbId;
      ckwCfgParam->entCfg[rbCount].rbType = rbCfgInfo->rbId.rbTyp;
/*
  As part of Common Code Merge Chnages removed flag TENB_SPLIT_ARCH_SUPPORT
*/
      /* if discard timer is configured for PDCP pass it to RLC also */
      if (rbCfgInfo->pdcpCfg.pdcpCfgBitMsk & NHU_PDCP_DISCARD_TMR)
      {
         ckwCfgParam->entCfg[rbCount].discardTmr = rbCfgInfo->pdcpCfg.discardTmr;
      }
      else
      {
         ckwCfgParam->entCfg[rbCount].discardTmr = -1;
      }
     /* nh003.301: added support for L2 Measurement */
#ifdef LTE_L2_MEAS
      ckwCfgParam->entCfg[rbCount].qci = rbCfgInfo->qci;
#endif /* LTE_L2_MEAS */

      /* for Up link Rb */
      if ((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL) &&
          (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_RLC_MODE_UL))
      {
         ckwCfgParam->entCfg[rbCount].dir |= CKW_CFG_DIR_UL;
         ckwCfgParam->entCfg[rbCount].entMode = 
                         rbCfgInfo->ulRbCfg.ulRlcMode.mode;
            
         nhPIMFillRlcUlCfgParam(ckwCfgParam, rbCfgInfo, rbCount);
      }
      /* for down link Rb */
      if ((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) &&
          (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_DL))
      {
         /* Oring with the direction */
         ckwCfgParam->entCfg[rbCount].dir |= CKW_CFG_DIR_DL;
         ckwCfgParam->entCfg[rbCount].entMode = 
                          rbCfgInfo->dlRbCfg.dlRlcMode.mode;
            
         nhPIMFillRlcDlCfgParam(ckwCfgParam, rbCfgInfo, rbCount);
      } /* if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) */
   } /*  for ( i=0; i < rbCfgListInfo->numOfRbCfgInfo; i++, rbCount++) */
  
   /* increment the number of configured entity */
   ckwCfgParam->numEnt += rbCfgListInfo->numOfRbCfgInfo;
   
   RETVALUE(ROK);
}/* End of nhPIMFillRlcCfgParam */

/**
 *
 *   @brief Function to fill RLC Cell configuration parameter.
 *
 *   @details
 *   This function is used to fill RLC cell config parameter.
 *            
 *
 *   @param[out] ckwCfgParam     RLC configuration patameter
 *   @param[in]  rbCfgListInfo   RB config info list
 *   @param[in]  cfgType         Config type
 *   @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillRlcCellCfgParam 
(
CkwCfgInfo     *ckwCfgParam,
NhuRbCfgList   *rbCfgListInfo,
U8             cfgType 
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRlcCellCfgParam(ckwCfgParam, rbCfgListInfo, cfgType ) 
CkwCfgInfo     *ckwCfgParam;
NhuRbCfgList   *rbCfgListInfo;
U8             cfgType;
#endif /* ANSI */
{
   U8   rbCount = 0;
   NhuRbCfgInfo *rbCfgInfo = NULLP;

   TRC2(nhPIMFillRlcCellCfgParam)

   RLOG0(L_DEBUG, "nhPIMFillRlcCellCfgParam");
   
   ckwCfgParam->numEnt = rbCfgListInfo->numOfRbCfgInfo; 
   for (rbCount = 0; rbCount < rbCfgListInfo->numOfRbCfgInfo; rbCount++)
   {
      rbCfgInfo = &rbCfgListInfo->rbCfgInfoList[rbCount];
      if (NULLP == rbCfgInfo)
      {
         RLOG0(L_ERROR,"NULLP for rbCfgInfo");
         RETVALUE(RFAILED);
      }
      ckwCfgParam->entCfg[rbCount].cfgType = cfgType;
      ckwCfgParam->entCfg[rbCount].rbId = rbCfgInfo->rbId.rbId;
      ckwCfgParam->entCfg[rbCount].rbType = rbCfgInfo->rbId.rbTyp;
      
      if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL &&
          rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_RLC_MODE_UL)
      {
         ckwCfgParam->entCfg[rbCount].dir |= CKW_CFG_DIR_UL;
         ckwCfgParam->entCfg[rbCount].entMode = 
                          rbCfgInfo->ulRbCfg.ulRlcMode.mode;
         
         if(NHU_RLC_MODE_TM == rbCfgInfo->ulRbCfg.ulRlcMode.mode)
         {
             ckwCfgParam->entCfg[rbCount].lCh[0].lChId =
                                             rbCfgInfo->lcId.lcId;
             ckwCfgParam->entCfg[rbCount].lCh[0].type =
                                        rbCfgInfo->lcId.logChType;
         } /* if (NHU_RLC_MODE_TM == rbCfgInfo->ulRbCfg.ulRlcMode.mode) */
         else
         {
             RLOG1(L_ERROR, "Invalid Rlc Mode for Cell [%d]",
                rbCfgInfo->ulRbCfg.ulRlcMode.mode);
         }
      } /* if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL && ... */

      if ((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) &&
          (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_RLC_MODE_DL))
      {
         ckwCfgParam->entCfg[rbCount].dir |= CKW_CFG_DIR_DL;
         ckwCfgParam->entCfg[rbCount].entMode = 
                       rbCfgInfo->dlRbCfg.dlRlcMode.mode;
         
         if(NHU_RLC_MODE_TM == rbCfgInfo->dlRbCfg.dlRlcMode.mode)
         {
              ckwCfgParam->entCfg[rbCount].m.tmInfo.dl.buffSize =
              rbCfgInfo->dlRbCfg.dlRlcMode.m.tmRlc.bufferSize;
              ckwCfgParam->entCfg[rbCount].lCh[0].lChId =
                                         rbCfgInfo->lcId.lcId;
              ckwCfgParam->entCfg[rbCount].lCh[0].type =
                                    rbCfgInfo->lcId.logChType;
         } /* if(NHU_RLC_MODE_TM == rbCfgInfo->dlRbCfg.dlRlcMode.mode) */
         else
         {
             RLOG1(L_ERROR, "Invalid Rlc Mode for Cell [%d]", 
                rbCfgInfo->dlRbCfg.dlRlcMode.mode);
         }
      } /* if ((rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL) && ... */
   } /* end of for loop */
   RETVALUE(ROK);
} /* end of nhPIMFillRlcCellCfgParam */

/**
 *
 *   @brief Function to fill MAC revert release configuration parameter.
 *
 *   @details
 *   This function is used to fill revert MAC logical
 *   channel.
 *            
 *
 *  @param[out] crgLchConfig  MAC logical channel configuration  
 *  @param[in]  logChInfo  Logical channel configuration info
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillMacRvrtLchRlsCfgParam 
(
CrgLchCfg     *crgLchConfig,
NhLogChCfg     *logChInfo
)
#else /* ANSI */
PRIVATE S16 nhPIMFillMacRvrtLchRlsCfgParam(crgLchConfig, logChInfo) 
CrgLchCfg    *crgLchConfig;
NhLogChCfg     *logChInfo;
#endif /* ANSI */
{
   TRC2(nhPIMFillMacRvrtLchRlsCfgParam)
   
   /* if logChCfg is present, it means UL is active */
   if (logChInfo->lChCfgBitMsk & NH_LOGCH_CFG_LCH)
   {
      if (logChInfo->logChCfg.logChCfgpres)
      {
          crgLchConfig->ulInfo.lcgId = logChInfo->logChCfg.logChGrp;
      }
      crgLchConfig->dir = CRG_DIR_RX;
      if (logChInfo->lChCfgBitMsk & NH_LOGCH_CFG_UL_TRCHTYP)
      {
         crgLchConfig->ulInfo.ulTrchType = logChInfo->ulTrchType;
      }
   }

   if (logChInfo->lChCfgBitMsk & NH_LOGCH_CFG_DL_TRCHTYP)
   {
      crgLchConfig->dlInfo.dlTrchType = logChInfo->dlTrchType;

      if (crgLchConfig->dir == CRG_DIR_RX)
      {
         crgLchConfig->dir = CRG_DIR_TX_RX;
      }
      else
      {
         crgLchConfig->dir = CRG_DIR_TX;
      }
   }

   RETVALUE(ROK);
}/* End of nhPIMFillMacRvrtLchRlsCfgParam */

/**
 *
 *   @brief Function to fill MAC revert re-configuration parameter.
 *
 *   @details
 *   This function is used to fill revert MAC logical 
 *   channel re-configuration parameter.
 *            
 *   @param[out] crgLchReConfig  MAC logical channel re-configuration 
 *   @param[in]  logChInfo    Logical channel configuration
 *
 *   @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillMacRvrtLchReCfgParam 
(
CrgLchRecfg     *crgLchReConfig,
NhLogChCfg    *logChInfo
)
#else /* ANSI */
PRIVATE S16 nhPIMFillMacRvrtLchReCfgParam(crgLchReConfig, logChInfo) 
CrgLchRecfg    *crgLchReConfig;
NhLogChCfg    *logChInfo;
#endif /* ANSI */
{
   TRC2(nhPIMFillMacRvrtLchReCfgParam)
  
   /* Logical channel group */
   if (logChInfo->lChCfgBitMsk & NH_LOGCH_CFG_LCH)
   {
      if (logChInfo->logChCfg.logChCfgpres)
      {
         crgLchReConfig->ulRecfg.lcgId = logChInfo->logChCfg.logChGrp;
      }
   }

   RETVALUE(ROK);
}/* End of nhPIMFillMacRvrtLchReCfgParam */
/**
 *
 *   @brief Function to fill MAC re-configuration parameter.
 *
 *   @details
 *   This function is used to fill MAC logical channel re-configuration parameter.
 *            
 *
 *   @param[out] crgLchReConfig  MAC logical channel re-configuration
 *   @param[in]  rbCfgInfo    RB Configuration info
 *   @param[out] lcId         Logical channel ID
 *   @param[out] dir          Direction
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillMacLchReCfgParam 
(
CrgLchRecfg     *crgLchReConfig,
NhuRbCfgInfo    *rbCfgInfo,
U8              *lcId,
U8              *dir
)
#else /* ANSI */
PRIVATE S16 nhPIMFillMacLchReCfgParam(crgLchReConfig, rbCfgInfo, lcId, dir) 
CrgLchRecfg    *crgLchReConfig;
NhuRbCfgInfo   *rbCfgInfo;
U8             *lcId; 
U8             *dir;
#endif /* ANSI */
{
   TRC2(nhPIMFillMacLchReCfgParam)
   
   if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL)
   {
      if (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)
      {
         crgLchReConfig->lcId = rbCfgInfo->lcId.lcId;
         *lcId = crgLchReConfig->lcId;
         *dir = NH_LCID_DIR_RX;
         if (rbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChCfgpres)
         {
            crgLchReConfig->ulRecfg.lcgId =
                  rbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChGrp;
         }
      }
   }      

   RETVALUE(ROK);
}/* End of nhPIMFillMacLchReCfgParam */

/**
 *
 *   @brief Function to fill MAC configuration parameter.
 *
 *   @details
 *   This function is used to fill MAC logical configuration parameter.
 *            
 *
 *   @param[out] crgLchConfig  MAC logical channel configuration
 *   @param[out] lcId       Logical Channel ID
 *   @param[out] dir        Direction UL/DL
 *   @param[in]  rbCfgInfo  RB configuration info
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillMacLchCfgParam 
(
CrgLchCfg     *crgLchConfig,
NhuRbCfgInfo *rbCfgInfo,
U8            *lcId,
U8            *dir
)
#else /* ANSI */
PRIVATE S16 nhPIMFillMacLchCfgParam(crgLchConfig, rbCfgInfo, lcId, dir) 
CrgLchCfg    *crgLchConfig;
NhuRbCfgInfo *rbCfgInfo;
U8           *lcId;
U8           *dir;
#endif /* ANSI */
{
   TRC2(nhPIMFillMacLchCfgParam)

   /* Up Link info */   
   if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_UL)
   {
      if (rbCfgInfo->ulRbCfg.ulRbBitMsk & NHU_RB_CFG_LOGCH_MAP_UL)
      {
         crgLchConfig->lcId = rbCfgInfo->lcId.lcId;
         *lcId = crgLchConfig->lcId;
         crgLchConfig->lcType = rbCfgInfo->lcId.logChType;
         *dir = NH_LCID_DIR_RX;
         crgLchConfig->dir = CRG_DIR_RX;  
         crgLchConfig->ulInfo.ulTrchType = 
                 rbCfgInfo->ulRbCfg.ulLogChMap.trChType;
         /* ICNC Needs to revist the logic */
         if ( rbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChCfgpres)
         {
            crgLchConfig->ulInfo.lcgId =
               rbCfgInfo->ulRbCfg.ulLogChMap.lcCfg.logChGrp;
         }
      }
   }      
   /* Down Link info */
   if (rbCfgInfo->rbCfgBitMsk & NHU_RB_CFG_DL)
   {
      if (rbCfgInfo->dlRbCfg.dlRbBitMsk & NHU_RB_CFG_LOGCH_MAP_DL)
      {
         crgLchConfig->lcId = rbCfgInfo->lcId.lcId;
         *lcId = rbCfgInfo->lcId.lcId;
         crgLchConfig->lcType = rbCfgInfo->lcId.logChType;
         if ( crgLchConfig->dir == CRG_DIR_RX)
         {
            *dir = NH_LCID_DIR_TXRX;
            crgLchConfig->dir = CRG_DIR_TX_RX;  
         }
         else
         {
            *dir = NH_LCID_DIR_TX;
            crgLchConfig->dir = CRG_DIR_TX;  
         }
         crgLchConfig->dlInfo.dlTrchType = 
                       rbCfgInfo->dlRbCfg.dlLogChMap.trChType;
      }
   }  
/*nh003.301: Filling the L2 Measurement related parameters*/
#ifdef LTE_L2_MEAS
   crgLchConfig->qci = rbCfgInfo->qci;
#endif /* LTE_L2_MEAS */    

   RETVALUE(ROK);
}/* End of nhPIMFillMacLchCfgParam */

/**
 *
 *   @brief Function to fill MAC configuration parameter.
 *
 *   @details
 *   This function is used to fill MAC UE configuration parameter.
 *            
 *
 *   @param[out] crgUeConfig  MAC UE configuration
 *   @param[in]  trChCfg   Trch configuration
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillMacUeCfgTrchParam 
(
CrgUeCfg      *crgUeConfig,  
NhuTrChCfg      *trChCfg
)
#else /* ANSI */
PRIVATE S16 nhPIMFillMacUeCfgTrchParam(crgUeConfig, trChCfg) 
CrgUeCfg      *crgUeConfig;  
NhuTrChCfg      *trChCfg;
#endif /* ANSI */
{
   TRC2(nhPIMFillMacUeCfgTrchParam)
   
   if (trChCfg->trChBitMsk & NHU_TRCH_CFG_UL_HARQ)
   {
      crgUeConfig->ueUlHqCfg.maxUlHqTx =trChCfg->ulHarqCfg.ulMaxHarqTx; 
   }
    
   RETVALUE(ROK);

}/* End of nhPIMFillMacUeCfgTrchParam */

/**
 *
 *   @brief Function to fill MAC configuration parameter.
 *
 *   @details
 *   This function is used to fill MAC Cell configuration parameter.
 *            
 *
 *   @param[out] crgCellConfig   MAC cell configuration
 *   @param[in]  cellCfgInfo  Cell configration info
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillMacCellCfgParam 
(
CrgCellCfg    *crgCellConfig,
NhuCellCfgInfo  *cellCfgInfo
)
#else /* ANSI */
PRIVATE S16 nhPIMFillMacCellCfgParam(crgCellConfig, cellCfgInfo) 
CrgCellCfg      *crgCellConfig;
NhuCellCfgInfo  *cellCfgInfo;
#endif /* ANSI */
{
   
   TRC2(nhPIMFillMacCellCfgParam)

   if (cellCfgInfo->cellCfgBitMsk & NHU_CELL_CFG_MAC_CMN)
   {
      crgCellConfig->rachCfg.maxMsg3Tx = 
                    cellCfgInfo->macCmnCfg.rachCfg.maxMsg3Tx;
      /* nh002.201 - filling sysbw to MAC */
      crgCellConfig->bwCfg.dlTotalBw = (U8)cellCfgInfo->sysBwCfg.dlSysBw;
      crgCellConfig->bwCfg.ulTotalBw = (U8)cellCfgInfo->sysBwCfg.ulSysBw;
   }

   RETVALUE(ROK);
}/* End of nhPIMFillMacCellCfgParam */

/**
 *
 *   @brief Function to fill MAC Cell Revert configuration parameter.
 *
 *   @details
 *   This function is used to fill MAC Cell Revert configuration parameter.
 *            
 *
 *   @param[out] crgCellReConfig  MAC Cell re-configuration 
 *   @param[in]  cellCfgInfo   Cell configuration info
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillMacRvrtCellReCfgParam 
(
CrgCellRecfg   *crgCellReConfig,
NhCellCfgInfo  *cellCfgInfo
)
#else /* ANSI */
PRIVATE S16 nhPIMFillMacRvrtCellReCfgParam(crgCellReConfig, cellCfgInfo) 
CrgCellRecfg   *crgCellReConfig;
NhCellCfgInfo  *cellCfgInfo;
#endif /* ANSI */
{
   
   TRC2(nhPIMFillMacRvrtCellReCfgParam)

   if (cellCfgInfo->cfgBitMsk & NHU_CELL_CFG_MAC_CMN)
   {
      crgCellReConfig->rachRecfg.maxMsg3Tx = 
                       cellCfgInfo->macCfg.rachCfg.maxMsg3Tx;
   }

   RETVALUE(ROK);
}/* End of nhPIMFillMacRvrtCellReCfgParam */
/**
 *
 *   @brief Function to fill MAC configuration parameter.
 *
 *   @details
 *   This function is used to fill MAC Cell re-configuration parameter.
 *            
 *
 *   @param[out] crgCellReConfig  Mac cell re-configuration parameter
 *   @param[in]  cellCfgInfo   Cell configuration info
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillMacCellReCfgParam 
(
CrgCellRecfg      *crgCellReConfig,
NhuCellCfgInfo  *cellCfgInfo
)
#else /* ANSI */
PRIVATE S16 nhPIMFillMacCellReCfgParam(crgCellReConfig, cellCfgInfo) 
CrgCellRecfg      *crgCellReConfig;
NhuCellCfgInfo  *cellCfgInfo;
#endif /* ANSI */
{
   
   TRC2(nhPIMFillMacCellReCfgParam)

   if (cellCfgInfo->cellCfgBitMsk & NHU_CELL_CFG_MAC_CMN)
   {
      /* Fill RACH parameter */
      crgCellReConfig->rachRecfg.maxMsg3Tx = 
                       cellCfgInfo->macCmnCfg.rachCfg.maxMsg3Tx;
   }

   RETVALUE(ROK);
}/* End of nhPIMFillMacCellReCfgParam */

#ifdef RRC_PHY_CFG_SUPP
/**
 *
 *   @brief Function to fill PHY configuration parameter.
 *
 *   @details
 *   This function is used to fill PHY Ue configuration parameter.
 *            
 *
 *   @param[out] ueCfgParam  PHY Ue configuration
 *   @param[in]  uePhyCfg    NHU Ue Phy Cfg
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      

#ifdef ANSI
PRIVATE S16 nhPIMFillPhyCfgParam 
(
CtfDedCfgInfo  *ueCfgParam,
NhuPhyCfg     *uePhyCfg
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPhyCfgParam(ueCfgParam, uePhyCfg) 
CtfDedCfgInfo  *ueCfgParam;
NhuPhyCfg     *uePhyCfg;
#endif /* ANSI */
{
#ifdef NHU_VER2
   U8 idx;
#endif /* NHU_VER2 */
   TRC2(nhPIMFillPhyCfgParam)

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_UE_CAT)
   {
      ueCfgParam->ueCatCfg.pres = TRUE;
      ueCfgParam->ueCatCfg.ueCategory = uePhyCfg->ueCategory;
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_PDSCH)
   {
      ueCfgParam->pdschCfg.pres = TRUE;
      ueCfgParam->pdschCfg.pA = (CtfPdschPaParam) uePhyCfg->pdschCfg.pa;
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_PUCCH)
   {
      ueCfgParam->pucchCfg.pres = TRUE;
      ueCfgParam->pucchCfg.dedPucchCfgType = uePhyCfg->pucchCfg.choice;
      ueCfgParam->pucchCfg.pucchSetup.repFact 
                   = (CtfPucchRepFactor) uePhyCfg->pucchCfg.setup.factor;
      ueCfgParam->pucchCfg.pucchSetup.n1PUCCHRep 
                   = uePhyCfg->pucchCfg.setup.n1PUCCH_AN_Rep;
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_PUSCH)
   {
      ueCfgParam->puschCfg.pres = TRUE;
      ueCfgParam->puschCfg.betaOffsetAckIdx = 
                   uePhyCfg->puschCfg.betaOffsetAckIdx;
      ueCfgParam->puschCfg.betaOffsetRiIdx = 
                   uePhyCfg->puschCfg.betaOffsetRiIdx;
      ueCfgParam->puschCfg.betaOffsetCqiIdx = 
                   uePhyCfg->puschCfg.betaOffsetCqiIdx;
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_CQI)
   {
      ueCfgParam->cqiRptCfg.pres = TRUE;
#ifndef NHU_VER2
      ueCfgParam->cqiRptCfg.reportingMode = uePhyCfg->cqiCfg.reportingMode;
#endif /* NHU_VER2 */

#ifdef NHU_VER2
      if (uePhyCfg->cqiCfg.reportMode.aPeriodicRpt.pres != FALSE)
      {
          ueCfgParam->cqiRptCfg.reportMode.aPeriodicRpt.pres = TRUE;
          ueCfgParam->cqiRptCfg.reportMode.aPeriodicRpt.aPeriodicRptMode =
             (CtfCqiRptModeAperiodic)uePhyCfg->cqiCfg.reportMode.\
             aPeriodicRpt.aPeriodicRptMode;
      }
#else /* NHU_VER2 */
      if (NHU_CQI_RPTMODE_APERIODIC == uePhyCfg->cqiCfg.reportingMode)
      {
         ueCfgParam->cqiRptCfg.reportMode.aPeriodicRpt 
             = (CtfCqiRptModeAperiodic)uePhyCfg->cqiCfg.reportMode.aPeriodicRpt;
      }
#endif /* NHU_VER2 */


#ifdef NHU_VER2
      if (uePhyCfg->cqiCfg.reportMode.periodicRpt.pres != FALSE)
#else /* NHU_VER2 */
      if (NHU_CQI_RPTMODE_PERIODIC == uePhyCfg->cqiCfg.reportingMode)
#endif /* NHU_VER2 */
      {
#ifdef NHU_VER2
         ueCfgParam->cqiRptCfg.reportMode.periodicRpt.pres = TRUE;
#endif /* NHU_VER2 */
         ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiPeriodicCfgType 
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.cqiPeriodicCfgType;
         ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPUCCHRsrcIndx 
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.cqiPUCCHRsrcIndx;
         ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPmiCfgIndx 
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.cqiPmiCfgIndx;
         ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.formatIndicator 
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.formatIndicator;
         ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.subbandCqi.k 
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.subbandCqi.k;
         ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgPres
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.riCfgPres;
         ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgIndx
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.riCfgIndx;
         ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.\
                                     simultaneousAckNack
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.simultaneousAckNack;
      }
   }
   
   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_UL_SRS)
   {
      ueCfgParam->srsUlCfg.pres = TRUE;
      ueCfgParam->srsUlCfg.dedSrsUlCfgType = uePhyCfg->srsUlCfg.choice;
      ueCfgParam->srsUlCfg.dedSrsSetup.srsBw = 
              uePhyCfg->srsUlCfg.setup.srsBwCfg;
      ueCfgParam->srsUlCfg.dedSrsSetup.srsHopngBw = 
              uePhyCfg->srsUlCfg.setup.srsHopBwCfg;
      ueCfgParam->srsUlCfg.dedSrsSetup.freqDmnPos = 
              uePhyCfg->srsUlCfg.setup.freqDomPos;
      ueCfgParam->srsUlCfg.dedSrsSetup.duration = 
              uePhyCfg->srsUlCfg.setup.duration;
      ueCfgParam->srsUlCfg.dedSrsSetup.srsCfgIdx = 
              uePhyCfg->srsUlCfg.setup.srsCfgIndx;
      ueCfgParam->srsUlCfg.dedSrsSetup.txComb = 
              uePhyCfg->srsUlCfg.setup.transComb;
      ueCfgParam->srsUlCfg.dedSrsSetup.cyclicShift = 
              uePhyCfg->srsUlCfg.setup.cycleshft;
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_SR)
   {
      ueCfgParam->dedSRCfg.pres = TRUE;
      ueCfgParam->dedSRCfg.dedSRCfgType = uePhyCfg->srCfg.dedSRCfgType;
      ueCfgParam->dedSRCfg.dedSrSetup.srPUCCHRi = 
              uePhyCfg->srCfg.setup.srPUCCHRi;
      ueCfgParam->dedSRCfg.dedSrSetup.srCfgIdx = uePhyCfg->srCfg.setup.srCfgIdx;
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_ANT)
   {
      ueCfgParam->antInfo.pres = TRUE;
      ueCfgParam->antInfo.txMode = (CtfUeTxAntMode)uePhyCfg->antCfg.txMode;
      ueCfgParam->antInfo.ueTxAntSelection.cfgType = uePhyCfg->antCfg.txAntSelectchoice;
      ueCfgParam->antInfo.ueTxAntSelection.txAntSelect = (CtfUeTxAntSelectType) uePhyCfg->antCfg.txAntSelect;
#ifdef NHU_VER2
      /* Added codeBookSubsetRestriction in PHY config */
      ueCfgParam->antInfo.codeBookSubsetRes.ctfCdBkRestType = 
         (CtfCdBkSubsetRestnType)uePhyCfg->antCfg.codeBookSubsetRes.\
         nhuCdBkRestType;
      switch(uePhyCfg->antCfg.codeBookSubsetRes.nhuCdBkRestType)
      {
          case N2TX_ANTENNA_TM3:
              ueCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n2TxAntTm3 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n2TxAntTm3;
          break;

          case N4TX_ANTENNA_TM3:
              ueCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n4TxAntTm3 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n4TxAntTm3;
          break;

          case N2TX_ANTENNA_TM4:
              ueCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n2TxAntTm4 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n2TxAntTm4;
          break;

          case N4TX_ANTENNA_TM4:
              for (idx = 0; idx < 8; idx++)
                  ueCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                              n4TxAntTm4[idx] = uePhyCfg->antCfg.codeBookSubsetRes.
                          codeBookSubsetRestn.n4TxAntTm4[idx];
          break;

          case N2TX_ANTENNA_TM5:
              ueCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n2TxAntTm5 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n2TxAntTm5;
          break;

          case N4TX_ANTENNA_TM5:
              ueCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n4TxAntTm5 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n4TxAntTm5;
          break;

          case N2TX_ANTENNA_TM6:
              ueCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n2TxAntTm6 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n2TxAntTm6;
          break;

          case N4TX_ANTENNA_TM6:
              ueCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n4TxAntTm6 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n4TxAntTm6;
          break;

          default:
              break;
      }
#endif /* NHU_VER2 */
   }
   RETVALUE(ROK);
}/* End of nhPIMFillPhyCfgParam */
/**
 *
 *   @brief Function to fill PHY configuration parameter.
 *
 *   @details
 *   This function is used to fill PHY Ue Re-Configuration parameter.
 *            
 *
 *   @param[out] ueReCfgParam  PHY Ue configuration
 *   @param[in]  uePhyCfg    NHU Ue Phy Cfg
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      

#ifdef ANSI
PRIVATE S16 nhPIMFillPhyReCfgParam 
(
CtfDedRecfgInfo  *ueReCfgParam,
NhuPhyCfg     *uePhyCfg
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPhyReCfgParam(ueReCfgParam, uePhyCfg) 
CtfDedRecfgInfo  *ueReCfgParam;
NhuPhyCfg     *uePhyCfg;
#endif /* ANSI */
{
#ifdef NHU_VER2
   U8 idx;
#endif /* NHU_VER2 */
   
   TRC2(nhPIMFillPhyReCfgParam)

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_UE_CAT)
   {
     ueReCfgParam->ueCatCfg.pres = TRUE;
     ueReCfgParam->ueCatCfg.ueCategory = uePhyCfg->ueCategory;
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_PDSCH)
   {
     ueReCfgParam->pdschCfg.pres = TRUE;
     ueReCfgParam->pdschCfg.pA = (CtfPdschPaParam)uePhyCfg->pdschCfg.pa;
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_PUCCH)
   {
     ueReCfgParam->pucchCfg.pres = TRUE;
     ueReCfgParam->pucchCfg.dedPucchCfgType = uePhyCfg->pucchCfg.choice;
     ueReCfgParam->pucchCfg.pucchSetup.repFact 
                 =(CtfPucchRepFactor) uePhyCfg->pucchCfg.setup.factor;
     ueReCfgParam->pucchCfg.pucchSetup.n1PUCCHRep 
                 = uePhyCfg->pucchCfg.setup.n1PUCCH_AN_Rep;
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_PUSCH)
   {
     ueReCfgParam->puschCfg.pres = TRUE;
     ueReCfgParam->puschCfg.betaOffsetAckIdx = 
             uePhyCfg->puschCfg.betaOffsetAckIdx;
     ueReCfgParam->puschCfg.betaOffsetRiIdx = 
             uePhyCfg->puschCfg.betaOffsetRiIdx;
     ueReCfgParam->puschCfg.betaOffsetCqiIdx = 
             uePhyCfg->puschCfg.betaOffsetCqiIdx;
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_UL_SRS)
   {
     ueReCfgParam->srsUlCfg.pres = TRUE;
     ueReCfgParam->srsUlCfg.dedSrsUlCfgType = uePhyCfg->srsUlCfg.choice;
     ueReCfgParam->srsUlCfg.dedSrsSetup.srsBw = 
             uePhyCfg->srsUlCfg.setup.srsBwCfg;
     ueReCfgParam->srsUlCfg.dedSrsSetup.srsHopngBw = 
             uePhyCfg->srsUlCfg.setup.srsHopBwCfg;
     ueReCfgParam->srsUlCfg.dedSrsSetup.freqDmnPos = 
             uePhyCfg->srsUlCfg.setup.freqDomPos;
     ueReCfgParam->srsUlCfg.dedSrsSetup.duration = 
             uePhyCfg->srsUlCfg.setup.duration;
     ueReCfgParam->srsUlCfg.dedSrsSetup.srsCfgIdx = 
             uePhyCfg->srsUlCfg.setup.srsCfgIndx;
     ueReCfgParam->srsUlCfg.dedSrsSetup.txComb = 
             uePhyCfg->srsUlCfg.setup.transComb;
     ueReCfgParam->srsUlCfg.dedSrsSetup.cyclicShift = 
             uePhyCfg->srsUlCfg.setup.cycleshft;
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_SR)
   {
     ueReCfgParam->dedSRCfg.pres = TRUE;
     ueReCfgParam->dedSRCfg.dedSRCfgType = uePhyCfg->srCfg.dedSRCfgType;
     ueReCfgParam->dedSRCfg.dedSrSetup.srPUCCHRi = 
             uePhyCfg->srCfg.setup.srPUCCHRi;
     ueReCfgParam->dedSRCfg.dedSrSetup.srCfgIdx = 
             uePhyCfg->srCfg.setup.srCfgIdx;
   }

   /*nh001.201*/
   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_CQI)
   {
      ueReCfgParam->cqiRptCfg.pres = TRUE;
#ifndef NHU_VER2
      ueReCfgParam->cqiRptCfg.reportingMode = uePhyCfg->cqiCfg.reportingMode;
#endif /* NHU_VER2 */

#ifdef NHU_VER2
      if(uePhyCfg->cqiCfg.reportMode.aPeriodicRpt.pres != FALSE)
      {
          ueReCfgParam->cqiRptCfg.reportMode.aPeriodicRpt.pres = TRUE;
          ueReCfgParam->cqiRptCfg.reportMode.aPeriodicRpt.aPeriodicRptMode =
             (CtfCqiRptModeAperiodic)uePhyCfg->cqiCfg.reportMode.\
             aPeriodicRpt.aPeriodicRptMode;
      }
#else /* NHU_VER2 */
      if (NHU_CQI_RPTMODE_APERIODIC == uePhyCfg->cqiCfg.reportingMode)
      {
         ueReCfgParam->cqiRptCfg.reportMode.aPeriodicRpt 
             = (CtfCqiRptModeAperiodic)uePhyCfg->cqiCfg.reportMode.aPeriodicRpt;
      }
#endif  /* NHU_VER2 */

#ifdef NHU_VER2
      if (uePhyCfg->cqiCfg.reportMode.periodicRpt.pres != FALSE)
#else /* NHU_VER2 */
      if (NHU_CQI_RPTMODE_PERIODIC == uePhyCfg->cqiCfg.reportingMode)
#endif /* NHU_VER2 */
      {
#ifdef NHU_VER2
         ueReCfgParam->cqiRptCfg.reportMode.periodicRpt.pres = TRUE;
#endif /* NHU_VER2 */
         ueReCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiPeriodicCfgType 
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.cqiPeriodicCfgType;
         ueReCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.\
                                            cqiPUCCHRsrcIndx 
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.cqiPUCCHRsrcIndx;
         ueReCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPmiCfgIndx 
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.cqiPmiCfgIndx;
         ueReCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.\
                                            formatIndicator 
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.formatIndicator;
         ueReCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.subbandCqi.k 
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.subbandCqi.k;
         ueReCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgPres
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.riCfgPres;
         ueReCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgIndx
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.riCfgIndx;
         ueReCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.\
                                            simultaneousAckNack
            = uePhyCfg->cqiCfg.reportMode.periodicRpt.setup.simultaneousAckNack;
      }
   }

   if (uePhyCfg->bitMask & NHU_DED_PHY_CFG_ANT)
   {
      ueReCfgParam->antInfo.pres = TRUE;
      ueReCfgParam->antInfo.txMode = (CtfUeTxAntMode)uePhyCfg->antCfg.txMode;
      ueReCfgParam->antInfo.ueTxAntSelection.cfgType = uePhyCfg->antCfg.txAntSelectchoice;
      ueReCfgParam->antInfo.ueTxAntSelection.txAntSelect = (CtfUeTxAntSelectType) uePhyCfg->antCfg.txAntSelect;
#ifdef NHU_VER2
      ueReCfgParam->antInfo.codeBookSubsetRes.ctfCdBkRestType = 
	       (CtfCdBkSubsetRestnType)uePhyCfg->antCfg.codeBookSubsetRes.nhuCdBkRestType;
      switch(uePhyCfg->antCfg.codeBookSubsetRes.nhuCdBkRestType)
      {
          case N2TX_ANTENNA_TM3:
              ueReCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n2TxAntTm3 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n2TxAntTm3;
          break;

          case N4TX_ANTENNA_TM3:
              ueReCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n4TxAntTm3 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n4TxAntTm3;
          break;

          case N2TX_ANTENNA_TM4:
              ueReCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n2TxAntTm4 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n2TxAntTm4;
          break;

          case N4TX_ANTENNA_TM4:
              for (idx = 0; idx < 8; idx++)
                  ueReCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                              n4TxAntTm4[idx] = uePhyCfg->antCfg.codeBookSubsetRes.
                          codeBookSubsetRestn.n4TxAntTm4[idx];
          break;

          case N2TX_ANTENNA_TM5:
              ueReCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n2TxAntTm5 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n2TxAntTm5;
          break;

          case N4TX_ANTENNA_TM5:
              ueReCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n4TxAntTm5 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n4TxAntTm5;
          break;

          case N2TX_ANTENNA_TM6:
              ueReCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n2TxAntTm6 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n2TxAntTm6;
          break;

          case N4TX_ANTENNA_TM6:
              ueReCfgParam->antInfo.codeBookSubsetRes.codeBookSubsetRestn.
                      n4TxAntTm6 = uePhyCfg->antCfg.codeBookSubsetRes.
                      codeBookSubsetRestn.n4TxAntTm6;
          break;

          default:
              break;
      }
#endif /* NHU_VER2 */
   }
   RETVALUE(ROK);
}/* End of nhPIMFillPhyReCfgParam */

/**
 *
 *   @brief Function to fill PHY configuration parameter.
 *
 *   @details
 *   This function is used to fill PHY Cell configuration parameter.
 *            
 *
 *   @param[out] ctfCellCfg   PHY cell configuration
 *   @param[in]  cellCfgInfo  Cell configration info
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
/* nh002.201 : filling bandwidth config from the changed structure */
#ifdef ANSI
PRIVATE S16 nhPIMFillPhyCellCfgParam 
(
CtfCellCfgInfo  *ctfCellCfg,
NhuCellCfgInfo  *cellCfgInfo
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPhyCellCfgParam(ctfCellCfg, cellCfgInfo) 
CtfCellCfgInfo  *ctfCellCfg;
NhuCellCfgInfo  *cellCfgInfo;
#endif /* ANSI */
{
   NhuCmnPhyCfg   *phyCfgInfo = &(cellCfgInfo->phyCellCfg);
   
   TRC2(nhPIMFillPhyCellCfgParam)

   ctfCellCfg->cellIdGrpId = phyCfgInfo->pcellCfg.cellIdGrpId;
   ctfCellCfg->physCellId = phyCfgInfo->pcellCfg.physCellId;

   ctfCellCfg->bwCfg.dlBw = NH_GET_SYS_BW(cellCfgInfo->sysBwCfg.dlSysBw);
   ctfCellCfg->bwCfg.ulBw = NH_GET_SYS_BW(cellCfgInfo->sysBwCfg.ulSysBw);
   ctfCellCfg->antennaCfg.antPortsCnt = NH_GET_ANT_PORT_CNT(phyCfgInfo->numOfTxAnt);
   
   ctfCellCfg->bwCfg.eUtraBand = phyCfgInfo->eUtraBand;

/* nh002.301:120713: Added Macros for mapping RRC enum
 *                  to PHY enum.
 */
   ctfCellCfg->txCfg.scSpacing = NH_GET_SC_SPACING (phyCfgInfo->\
         schmCfg.scSpacing);


   if (phyCfgInfo->bitmask & NHU_CMN_PHY_CFG_PRACH)
   {
      ctfCellCfg->prachCfg.pres = TRUE;
      ctfCellCfg->prachCfg.rootSequenceIndex = 
              phyCfgInfo->prachCfg.rootSequenceIndex;
      ctfCellCfg->prachCfg.prachCfgIndex = phyCfgInfo->prachCfg.prachCfgIndex;
      ctfCellCfg->prachCfg.zeroCorrelationZoneCfg = 
                  phyCfgInfo->prachCfg.zeroCorrelationZoneCfg;
      ctfCellCfg->prachCfg.highSpeedFlag = 
                  phyCfgInfo->prachCfg.highSpeedFlag;
      ctfCellCfg->prachCfg.prachFreqOffset = 
                  phyCfgInfo->prachCfg.prachFreqOffset;
   }
   if (phyCfgInfo->bitmask & NHU_CMN_PHY_CFG_PDSCH)
   {
      ctfCellCfg->pdschCfg.pres = TRUE;

      ctfCellCfg->pdschCfg.refSigPwr = phyCfgInfo->cmnPdschCfg.refSigPower;
      ctfCellCfg->pdschCfg.p_b = phyCfgInfo->cmnPdschCfg.pb;
   }

   if (phyCfgInfo->bitmask & NHU_CMN_PHY_CFG_PUSCH)
   {
      ctfCellCfg->puschCfg.pres = TRUE;

      ctfCellCfg->puschCfg.puschBasicCfg.noOfsubBands =
                  phyCfgInfo->cmnPuschCfg.nSB;
      ctfCellCfg->puschCfg.puschBasicCfg.hoppingMode =
                  (CtfPuschHoppingMode) phyCfgInfo->cmnPuschCfg.hoppingMode;
      ctfCellCfg->puschCfg.puschBasicCfg.hoppingOffset =
                  phyCfgInfo->cmnPuschCfg.puschHoppingOffset;
      ctfCellCfg->puschCfg.puschBasicCfg.enable64QAM =
                  phyCfgInfo->cmnPuschCfg.enable64QAM;
      ctfCellCfg->puschCfg.puschUlRS.grpHopEnabled =
                  phyCfgInfo->cmnPuschCfg.refSig.grpHoppngEnb;
      ctfCellCfg->puschCfg.puschUlRS.seqHopEnabled =
                  phyCfgInfo->cmnPuschCfg.refSig.seqHoppingEnb;
      ctfCellCfg->puschCfg.puschUlRS.grpNum =
                  phyCfgInfo->cmnPuschCfg.refSig.grpAssignPUSCH;
      ctfCellCfg->puschCfg.puschUlRS.cycShift =
                  phyCfgInfo->cmnPuschCfg.refSig.cyclicShift;
   }

   if (phyCfgInfo->bitmask & NHU_CMN_PHY_CFG_PHICH)
   {
      ctfCellCfg->phichCfg.pres = TRUE;

      ctfCellCfg->phichCfg.duration = 
              (CtfPhichDuration) phyCfgInfo->cmnPhichCfg.duration;
      ctfCellCfg->phichCfg.resource = 
              (CtfPhichResource) phyCfgInfo->cmnPhichCfg.resrc;
   }

   if (phyCfgInfo->bitmask & NHU_CMN_PHY_CFG_PUCCH)
   {
      ctfCellCfg->pucchCfg.pres = TRUE;

      ctfCellCfg->pucchCfg.nRB = phyCfgInfo->cmnPucchCfg.pucchFixBw;
      ctfCellCfg->pucchCfg.nCS = phyCfgInfo->cmnPucchCfg.cyclShift;
      ctfCellCfg->pucchCfg.n1PUCCH = phyCfgInfo->cmnPucchCfg.n1PucchAn;
      /* nh002.301:118366,120285: Added Macro for mapping RRC enum
       *                  to PHY enum.
       */
      ctfCellCfg->pucchCfg.deltaShift = NH_GET_DELTA_SHIFT
         (phyCfgInfo->cmnPucchCfg.deltaShift);

   }

   if (phyCfgInfo->bitmask & NHU_CMN_PHY_CFG_UL_SRS)
   {
      ctfCellCfg->srsUlCfg.pres = TRUE;
      ctfCellCfg->srsUlCfg.srsCfgType = phyCfgInfo->srsUlCfg.srsCfgType;
      ctfCellCfg->srsUlCfg.srsSetup.srsBw = phyCfgInfo->srsUlCfg.setup.bwCfg;
      ctfCellCfg->srsUlCfg.srsSetup.sfCfg = 
                  phyCfgInfo->srsUlCfg.setup.subfrmCfg;
      ctfCellCfg->srsUlCfg.srsSetup.srsANSimultTx =
                  phyCfgInfo->srsUlCfg.setup.srsANSimultTx;
      ctfCellCfg->srsUlCfg.srsSetup.srsMaxUpPts =
                  phyCfgInfo->srsUlCfg.setup.srsMaxUpPts;
   }

   if (phyCfgInfo->bitmask & NHU_CMN_PHY_CFG_TDD_SF)
   {
      ctfCellCfg->tddSfCfg.pres = TRUE;
      ctfCellCfg->tddSfCfg.sfAssignment = phyCfgInfo->tddSfCfg.sfAssignment;
      ctfCellCfg->tddSfCfg.spclSfPatterns = phyCfgInfo->tddSfCfg.spclSfPatterns;
   }

   RETVALUE(ROK);
}/* End of nhPIMFillPhyCellCfgParam */

/**
 *
 *   @brief Function to fill PHY Cell Specific Reconfiguration parameter.
 *
 *   @details
 *   This function is used to fill PHY Cell Reconfiguration parameter.
 *            
 *
 *   @param[out] ctfCellRecfg   PHY cell configuration
 *   @param[in]  cellReCfgInfo  Cell configration info
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      

#ifdef ANSI
PRIVATE S16 nhPIMFillPhyCellReCfgParam 
(
CtfCellRecfgInfo  *ctfCellRecfg,
NhuCmnPhyCfg      *cellCfgInfo
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPhyCellReCfgParam(ctfCellRecfg, cellCfgInfo) 
CtfCellRecfgInfo  *ctfCellRecfg;
NhuCmnPhyCfg      *cellCfgInfo;
#endif /* ANSI */
{
   
   TRC2(nhPIMFillPhyCellReCfgParam)

   /* ctfCellRecfg->cellId is filled outside this function */

   if (cellCfgInfo->bitmask & NHU_CMN_PHY_CFG_PRACH)
   {
      ctfCellRecfg->prachCfg.pres = TRUE;
      ctfCellRecfg->prachCfg.rootSequenceIndex = 
                  cellCfgInfo->prachCfg.rootSequenceIndex;
      ctfCellRecfg->prachCfg.prachCfgIndex = 
                  cellCfgInfo->prachCfg.prachCfgIndex;
      ctfCellRecfg->prachCfg.zeroCorrelationZoneCfg = 
                  cellCfgInfo->prachCfg.zeroCorrelationZoneCfg;
      ctfCellRecfg->prachCfg.highSpeedFlag = 
                  cellCfgInfo->prachCfg.highSpeedFlag;
      ctfCellRecfg->prachCfg.prachFreqOffset = 
                  cellCfgInfo->prachCfg.prachFreqOffset;
   }
   if (cellCfgInfo->bitmask & NHU_CMN_PHY_CFG_PDSCH)
   {
      ctfCellRecfg->pdschCfg.pres = TRUE;

      ctfCellRecfg->pdschCfg.refSigPwr = cellCfgInfo->cmnPdschCfg.refSigPower;
      ctfCellRecfg->pdschCfg.p_b = cellCfgInfo->cmnPdschCfg.pb;
   }

   if (cellCfgInfo->bitmask & NHU_CMN_PHY_CFG_PUSCH)
   {
      ctfCellRecfg->puschCfg.pres = TRUE;

      ctfCellRecfg->puschCfg.puschBasicCfg.noOfsubBands =
                  cellCfgInfo->cmnPuschCfg.nSB;
      ctfCellRecfg->puschCfg.puschBasicCfg.hoppingMode =
                  (CtfPuschHoppingMode) cellCfgInfo->cmnPuschCfg.hoppingMode;
      ctfCellRecfg->puschCfg.puschBasicCfg.hoppingOffset =
                  cellCfgInfo->cmnPuschCfg.puschHoppingOffset;
      ctfCellRecfg->puschCfg.puschBasicCfg.enable64QAM =
                  cellCfgInfo->cmnPuschCfg.enable64QAM;
      ctfCellRecfg->puschCfg.puschUlRS.grpHopEnabled =
                  cellCfgInfo->cmnPuschCfg.refSig.grpHoppngEnb;
      ctfCellRecfg->puschCfg.puschUlRS.seqHopEnabled =
                  cellCfgInfo->cmnPuschCfg.refSig.seqHoppingEnb;
      ctfCellRecfg->puschCfg.puschUlRS.grpNum =
                  cellCfgInfo->cmnPuschCfg.refSig.grpAssignPUSCH;
      ctfCellRecfg->puschCfg.puschUlRS.cycShift =
                  cellCfgInfo->cmnPuschCfg.refSig.cyclicShift;
   }

   if (cellCfgInfo->bitmask & NHU_CMN_PHY_CFG_PHICH)
   {
      ctfCellRecfg->phichCfg.pres = TRUE;

      ctfCellRecfg->phichCfg.duration = 
                  (CtfPhichDuration) cellCfgInfo->cmnPhichCfg.duration;
      ctfCellRecfg->phichCfg.resource = 
                  (CtfPhichResource) cellCfgInfo->cmnPhichCfg.resrc;
   }

   if (cellCfgInfo->bitmask & NHU_CMN_PHY_CFG_PUCCH)
   {
      ctfCellRecfg->pucchCfg.pres = TRUE;

      ctfCellRecfg->pucchCfg.nRB = cellCfgInfo->cmnPucchCfg.pucchFixBw;
      ctfCellRecfg->pucchCfg.nCS = cellCfgInfo->cmnPucchCfg.cyclShift;
      ctfCellRecfg->pucchCfg.n1PUCCH = cellCfgInfo->cmnPucchCfg.n1PucchAn;
      /* nh002.301:118366,120285: Added Macro for mapping RRC enum
       *                  to PHY enum.
       */
      ctfCellRecfg->pucchCfg.deltaShift = NH_GET_DELTA_SHIFT
         (cellCfgInfo->cmnPucchCfg.deltaShift);
   }

   if (cellCfgInfo->bitmask & NHU_CMN_PHY_CFG_UL_SRS)
   {
      ctfCellRecfg->srsUlCfg.pres = TRUE;
      ctfCellRecfg->srsUlCfg.srsCfgType = cellCfgInfo->srsUlCfg.srsCfgType;
      ctfCellRecfg->srsUlCfg.srsSetup.srsBw = cellCfgInfo->srsUlCfg.setup.bwCfg;
      ctfCellRecfg->srsUlCfg.srsSetup.sfCfg = 
                  cellCfgInfo->srsUlCfg.setup.subfrmCfg;
      ctfCellRecfg->srsUlCfg.srsSetup.srsANSimultTx =
                  cellCfgInfo->srsUlCfg.setup.srsANSimultTx;
      ctfCellRecfg->srsUlCfg.srsSetup.srsMaxUpPts =
                  cellCfgInfo->srsUlCfg.setup.srsMaxUpPts;
   }

   RETVALUE(ROK);
}/* End of nhPIMFillPhyCellReCfgParam */

/**
 *
 *   @brief Filling function to fill ctfInfo for Ue 
 *
 *   @details
 *   This function is used to fill Ue parameter for revert the Modify 
 *   configuration   
 *    
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] transCtrlBlk  Trans Cb
 *  @param[out] ctfCfgParam  config Parameter
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      

#ifdef ANSI
PRIVATE S16 nhPIMFillRvrtPhyModUeCfg
(
CtfCfgReqInfo  *ctfCfgParam,
NhActvUeCb *ueCb,
TransCb *transCtrlBlk
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRvrtPhyModUeCfg(ctfCfgParam, ueCb, transCtrlBlk) 
CtfCfgReqInfo  *ctfCfgParam;
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
#endif /* ANSI */
{

   TRC2(nhPIMFillRvrtPhyModUeCfg)

   RLOG0(L_DEBUG, "nhPIMFillRvrtPhyModUeCfg");

   ctfCfgParam->cfgType = CTF_RECONFIG;
   ctfCfgParam->u.reCfg.cfgElem = CRG_UE_CFG;
   ctfCfgParam->u.reCfg.u.dedRecfg.cellId = ueCb->cellCb->cellId;
   ctfCfgParam->u.reCfg.u.dedRecfg.ueId = ueCb->ueCbKey.crnti;
      
   nhPIMFillPhyReCfgParam(&(ctfCfgParam->u.reCfg.u.dedRecfg), 
                          &(ueCb->crntCfg.phyCfg));


   RETVALUE(ROK);

}/* End of nhPIMFillRvrtPhyModUeCfg */


/**
 *
 *   @brief Handler to Send Rvrt UeId change Request to PHY
 *
 *   @details
 *   This function is used to revert the UeId Change Request.
 *            
 *
 *   @param[in] lSap      Lsap control block
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   trans Cb.
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRvrtPhyUeIdChgReq
(
NhActvUeCb *ueCb,
TransCb    *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhPIMRvrtPhyUeIdChgReq(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;
TransCb    *transCtrlBlk;
#endif /* ANSI */
{
   CtfCfgTransId    ctfCfgTransId;          /* PHY Transaction Id */
   CtfUeInfo        *oldUeInfo = NULLP;
   CtfUeInfo        *newUeInfo = NULLP;
   NhLSapCb         *lSap = NULLP;
   SuId             suId=0;
   Pst              tmpPst;              /* Post structure */ 
   U32              rvrtTransId = NH_RESERVED_TRANS_ID;

   TRC2(nhPIMRvrtPhyUeIdChgReq)

   RLOG0(L_DEBUG, "nhPIMRvrtPhyUeIdChgReq");

   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCTFSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;
   
   if((NH_STATUS_USED == transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].status) &&
     (CTF_CFG_CFM_OK == transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].cfmStatus))
   {
      NH_ALLOC(NH_REG, NH_POOL, &oldUeInfo, sizeof(CtfUeInfo));
      if(NULLP == oldUeInfo)
      {
         RLOG0(L_FATAL, "Could not allocate mem for Old Ue Info");
         RETVALUE(RFAILED);
      }
      NH_ALLOC(NH_REG, NH_POOL, &newUeInfo, sizeof(CtfUeInfo));
      if(NULLP == newUeInfo)
      {
         RLOG0(L_FATAL, "Could not allocate mem for new ue info");
         NH_FREE(NH_REG, NH_POOL, oldUeInfo, sizeof(CtfUeInfo));
         RETVALUE(RFAILED);
      }
      oldUeInfo->ueId = transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;
      oldUeInfo->cellId = ueCb->cellCb->cellId;

      newUeInfo->ueId = ueCb->ueCbKey.crnti;
      newUeInfo->cellId = ueCb->cellCb->cellId;

      NH_SET_ZERO(&ctfCfgTransId, sizeof(CtfCfgTransId));

      NH_GET_PHY_CFG_REQ_TRANSID(ctfCfgTransId.trans, ueCb->cellCb->cellId,
            ueCb->ueCbKey.crnti, rvrtTransId, NH_CTF_UE_CFG);

      /* Fill the reserve Trans Id */
      if (ROK != NhLiCtfUeIdChgReq(&tmpPst, lSap->spId, ctfCfgTransId,
                                             oldUeInfo,  newUeInfo))
      {
          RLOG0(L_ERROR,"Failed to send Rvrt UeId Change Req to PDCP ");
          RETVALUE(RFAILED);
      }
        
   }/*if(NH_STATUS_USED == ...) */

   RETVALUE(ROK);
} /* nhPIMRvrtPhyUeIdChgReq */
#endif /* RRC_PHY_CFG_SUPP */
/**
 *
 *   @brief Handler to send rvrt pdcp release config
 *
 *   @details
 *   This function is used revert the pdcp release configuration.
 *            
 *
 *   @param[in] cpjCfgReq  Pdcp config request
 *   @param[in] ueCb       UE control block
 *   @param[in] transCtrlBlk    trans Cb.
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMRvrtPdcpRlsCfg
(
CpjCfgReqInfo   *cpjCfgReq,
NhActvUeCb *ueCb,
TransCb    *transCtrlBlk
)
#else /* ANSI */
PRIVATE S16 nhPIMRvrtPdcpRlsCfg(cpjCfgReq, ueCb, transCtrlBlk)
CpjCfgReqInfo   *cpjCfgReq; 
NhActvUeCb *ueCb;
TransCb    *transCtrlBlk;
#endif /* ANSI */
{
   U8                i;
   U8                pdcpId = NH_ZERO_VAL;
   U8                rbTyp;
   U8                rvrtAction;
   U32               tmp;
   CmLList           *tmpNode = NULLP;
   Bool              found = FALSE;
   NhuRbCfgInfo      *crntRbCfg = NULLP;
   
   
   TRC2(nhPIMRvrtPdcpRlsCfg)
   RLOG0(L_DEBUG, "nhPIMRvrtPdcpRlsCfg");

   rvrtAction = CPJ_CFG_ADD;
   for (i = 0; i < NH_MAX_CFG_REQ; i++)
   {
      tmp = (transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_RLS] >> i);
      rbTyp = CM_LTE_DRB; 
      if (tmp & 0x01)
      {
         pdcpId = i + 1; 
        
         found = FALSE;
         CM_LLIST_FIRST_NODE(&(ueCb->crntCfg.rbCfgList.rbCfgInfoList), tmpNode);
         while(NULLP != tmpNode)
         {
            crntRbCfg = (NhuRbCfgInfo *)tmpNode->node;
            
            if ((crntRbCfg->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO) && 
                ((pdcpId == crntRbCfg->rbId.rbId) && 
                 (crntRbCfg->rbId.rbTyp == rbTyp)))
            {
               found = TRUE;
               break;
            }
            CM_LLIST_NEXT_NODE(&ueCb->crntCfg.rbCfgList.rbCfgInfoList, tmpNode);
         } /* while(NULLP != tmpNode) */
         if (found)
         {
            nhPIMFillPdcpRvrtCfgParam(ueCb, cpjCfgReq, crntRbCfg, rvrtAction);
         }

      } /*  if (tmp & 0x01) */

   } /*  for (i = 0; i < NH_MAX_CFG_REQ; i++) */

   RETVALUE(ROK);
} /* end of nhPIMRvrtPdcpRlsCfg */
/**
 *
 *   @brief Handler to send rvrt pdcp modify config
 *
 *   @details
 *   This function is used revert the pdcp modify configuration.
 *            
 *
 *   @param[in] cpjCfgReq  Pdcp config request
 *   @param[in] ueCb       UE control block
 *   @param[in] transCtrlBlk    trans Cb.
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMRvrtPdcpModCfg
(
CpjCfgReqInfo   *cpjCfgReq,
NhActvUeCb *ueCb,
TransCb    *transCtrlBlk
)
#else /* ANSI */
PRIVATE S16 nhPIMRvrtPdcpModCfg(cpjCfgReq, ueCb, transCtrlBlk)
CpjCfgReqInfo   *cpjCfgReq; 
NhActvUeCb *ueCb;
TransCb    *transCtrlBlk;
#endif /* ANSI */
{
   U8                i;
   U8                pdcpId = NH_ZERO_VAL;
   U8                rbTyp;
   U8                rvrtAction;
   U32               tmp;
   CmLList           *tmpNode = NULLP;
   NhuRbCfgInfo      *crntRbCfg = NULLP;
   Bool              found = FALSE;
   S16               retVal = ROK;
   
   TRC2(nhPIMRvrtPdcpModCfg)

   rvrtAction = CPJ_CFG_MODIFY;
   for (i = 0; i < NH_MAX_CFG_REQ; i++)
   {
      tmp = (transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_MOD] >> i);
      rbTyp = CM_LTE_SRB;
      if (tmp & 0x01)
      {
         pdcpId = i + 1; 
      
         found = FALSE;
         CM_LLIST_FIRST_NODE(&(ueCb->crntCfg.rbCfgList.rbCfgInfoList), tmpNode);
         while(NULLP != tmpNode)
         {
            crntRbCfg = (NhuRbCfgInfo *)tmpNode->node;
            
            if ((crntRbCfg->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO) &&
                ((pdcpId == crntRbCfg->rbId.rbId) && 
                 (crntRbCfg->rbId.rbTyp == rbTyp)))
            {
               found = TRUE;
               break;
            }
            CM_LLIST_NEXT_NODE(&ueCb->crntCfg.rbCfgList.rbCfgInfoList, tmpNode);
         } /* end of while */
         if (found)
         {
            nhPIMFillPdcpRvrtCfgParam(ueCb, cpjCfgReq, crntRbCfg, rvrtAction);
         }
      } /* if (tmp & 0x01) */
      tmp = (transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_MOD] >> i);
      rbTyp = CM_LTE_DRB;
      if (tmp & 0x01)
      {
         pdcpId = i + 1; 
      
         found = FALSE;
         CM_LLIST_FIRST_NODE(&(ueCb->crntCfg.rbCfgList.rbCfgInfoList), tmpNode);
         while(NULLP != tmpNode)
         {
            crntRbCfg = (NhuRbCfgInfo *)tmpNode->node;
            
            if ((crntRbCfg->rbCfgBitMsk & NHU_RB_CFG_PDCP_INFO) &&
                ((pdcpId == crntRbCfg->rbId.rbId) && 
                 (crntRbCfg->rbId.rbTyp == rbTyp)))
            {
               found = TRUE;
               break;
            }
            CM_LLIST_NEXT_NODE(&ueCb->crntCfg.rbCfgList.rbCfgInfoList, tmpNode);
         } /* end of while */
         if (found)
         {
            nhPIMFillPdcpRvrtCfgParam(ueCb, cpjCfgReq, crntRbCfg, rvrtAction);
         }
      } /* if (tmp & 0x01) */
   }/* for (i = 0; i < NH_MAX_CFG_REQ; i++) */
  
   RETVALUE(retVal);
} /* end of nhPIMRvrtPdcpModCfg */
/**
 *
 *   @brief Handler to send rvrt pdcp add config
 *
 *   @details
 *   This function is used revert the pdcp add configuration.
 *            
 *
 *   @param[in] cpjCfgReq  Pdcp Config Request
 *   @param[in] ueCb       UE control block
 *   @param[in] transCtrlBlk    Trans Cb.
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMRvrtPdcpAddCfg
(
CpjCfgReqInfo  *cpjCfgReq,
NhActvUeCb *ueCb,
TransCb    *transCtrlBlk
)
#else /* ANSI */
PRIVATE S16 nhPIMRvrtPdcpAddCfg(cpjCfgReq, ueCb, transCtrlBlk)
CpjCfgReqInfo  *cpjCfgReq;
NhActvUeCb *ueCb;
TransCb    *transCtrlBlk;
#endif /* ANSI */
{
   U8                i;
   U8                pdcpId = NH_ZERO_VAL;
   U8                rvrtAction;
   U32               tmp;
   U8                rbCount = 0;
   
   
   TRC2(nhPIMRvrtPdcpAddCfg)

   UNUSED(ueCb);

   rvrtAction = CPJ_CFG_DELETE;
   for (i = 0; i < NH_MAX_CFG_REQ; i++)
   {
      tmp = (transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_ADD] >> i);
      
      /* If Cfg Request is sent with the below RB ID, send revert to it. */
      if (tmp & 0x01)
      {
         pdcpId = i + 1; 
         
         rbCount = cpjCfgReq->numEnt;
         cpjCfgReq->cfgEnt[rbCount].cfgType = rvrtAction;
         cpjCfgReq->cfgEnt[rbCount].rbId = pdcpId;
         cpjCfgReq->cfgEnt[rbCount].rbType = CM_LTE_SRB;
         cpjCfgReq->numEnt++;
      }

      tmp = (transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_ADD] >> i);
      
      /* If Cfg Request is sent with the below RB ID, send revert to it. */
      if (tmp & 0x01)
      {
         pdcpId = i + 1; 
         
         rbCount = cpjCfgReq->numEnt;
         cpjCfgReq->cfgEnt[rbCount].cfgType = rvrtAction;
         cpjCfgReq->cfgEnt[rbCount].rbId = pdcpId;
         cpjCfgReq->cfgEnt[rbCount].rbType = CM_LTE_DRB;
         cpjCfgReq->numEnt++;
      }


   } /* end of for loop */
   RETVALUE(ROK);

} /* end of nhPIMRvrtPdcpAddCfg */
/**
 *
 *   @brief Handler to Send Rvrt UeId change Request
 *
 *   @details
 *   This function is used revert the UeId Change Request.
 *            
 *
 *   @param[in] lSap      Lsap control block
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   trans Cb.
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMRvrtPdcpUeIdChgReq
(
NhLSapCb   *lSap,
NhActvUeCb *ueCb,
TransCb    *transCtrlBlk
)
#else /* ANSI */
PRIVATE S16 nhPIMRvrtPdcpUeIdChgReq(lSap, ueCb, transCtrlBlk)
NhLSapCb   *lSap;
NhActvUeCb *ueCb;
TransCb    *transCtrlBlk;
#endif /* ANSI */
{
   CpjUeInfo         *oldUeInfo = NULLP;
   CpjUeInfo         *newUeInfo = NULLP;
   Pst               tmpPst;              /* Post structure */ 

   TRC2(nhPIMRvrtPdcpUeIdChgReq)
   RLOG0(L_DEBUG, "nhPIMRvrtPdcpUeIdChgReq");

   tmpPst = lSap->pst;
   
   if((NH_STATUS_USED == 
                transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PDCP].status) &&
     (CPJ_CFG_CFM_OK == 
                transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PDCP].cfmStatus))
   {
      NH_ALLOC(NH_REG, NH_POOL, &oldUeInfo, sizeof(CpjUeInfo));
      if(NULLP == oldUeInfo)
      {
         RLOG0(L_FATAL, "Could not allocate mem for Old Ue Info");
         RETVALUE(RFAILED);
      }
      NH_ALLOC(NH_REG, NH_POOL, &newUeInfo, sizeof(CpjUeInfo));
      if(NULLP == newUeInfo)
      {
         RLOG0(L_FATAL, "Could not allocate mem for new ue info");
         NH_FREE(NH_REG, NH_POOL, oldUeInfo, sizeof(CpjUeInfo));
         RETVALUE(RFAILED);
      }
      oldUeInfo->ueId = transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;
      oldUeInfo->cellId = ueCb->cellCb->cellId;

      newUeInfo->ueId = ueCb->ueCbKey.crnti;
      newUeInfo->cellId = ueCb->cellCb->cellId;

      if (ROK != NhLiCpjUeIdChgReq(&tmpPst, lSap->spId, NH_RESERVED_TRANS_ID,
                                             oldUeInfo,  newUeInfo))
      {
          RLOG0(L_ERROR,"Failed to send Rvrt UeId Change Req to PDCP ");
          RETVALUE(RFAILED);
      }
        
   }/*if(NH_STATUS_USED == ...) */

   RETVALUE(ROK);
} /* nhPIMRvrtPdcpUeIdChgReq */
/**
 *
 *   @brief Handler to Revert configuration to PDCP.
 *
 *   @details
 *   This function is used revert configuration   
 *   which was sent to PDCP layer.
 *            
 *
 *   @param[in] *ueCb      UE control block
 *   @param[in] *transCtrlBlk   Transaction control block.
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRvrtPdcpCfg
(
NhActvUeCb *ueCb,
TransCb *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhPIMRvrtPdcpCfg(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
#endif /* ANSI */
{
   
   Pst               tmpPst;              /* Post structure */ 
   NhLSapCb          *lSap = NULLP;       /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL; 
   CpjCfgReqInfo     *cpjCfgReq = NULLP;   /* PDCP Config Request Parameter */
   U8                i=0;
  /* unused var deleted*/

   TRC2(nhPIMRvrtPdcpCfg)

   RLOG0(L_DEBUG, "nhPIMRvrtPdcpCfg");
   
   /* get lower Sap */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCPJSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;

   if(ROK != nhPIMRvrtPdcpUeIdChgReq(lSap, ueCb, transCtrlBlk))
   {
      RLOG0(L_ERROR, "nhPIMRvrtPdcpUeIdChgReq Failed ");
      RETVALUE(RFAILED);
   }
   /*No revert behaviour for Re-establishment*/
   for(i = 0; i < NH_MAX_RB_ACTION; i++)
   {
      if((transCtrlBlk->pdcpMapTbl.srbCfmSta[i]) ||
         (transCtrlBlk->pdcpMapTbl.drbCfmSta[i])) 
      {
         /* Allocate memory for cfg Parameter */
         NH_ALLOC(NH_REG, NH_POOL, &cpjCfgReq, sizeof(CpjCfgReqInfo));
         if (NH_CHK_INPUT_PARAM(cpjCfgReq))
         {
            RLOG0(L_FATAL, "GOT NULL cpjCfgReq");
            RETVALUE(RFAILED);
         }
      
         cpjCfgReq->cellId  = ueCb->cellCb->cellId; 
         cpjCfgReq->ueId  = ueCb->ueCbKey.crnti; 
         cpjCfgReq->transId  = NH_RESERVED_TRANS_ID; 
         break;
      }
   }
   if (i == NH_MAX_RB_ACTION)
   {
      RLOG0(L_DEBUG, "No PDCP Id To Rvrt ");
      RETVALUE(ROK);
   }
   if(transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_ADD] ||
     transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_ADD])
   {
      /*R9 upgrade: Kworks Error: ret value check is deleted since this
       * function always returns ROK. in future if somebody changes the
       * ret value then he has to add the check
       */
      (Void)nhPIMRvrtPdcpAddCfg(cpjCfgReq, ueCb, transCtrlBlk);
   }
   
   if(transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_MOD] ||
     transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_MOD])
   {
      /* R9 upgrade: kworks Error: ret val check is deleted since this func
       * always returns ROK. if somebody changes the ret val then he will have
       * add the ret val check and if it is not ROK then return with RFAILED
       * and before returning free the following:-
       * 1) cpjCfgReq->cfgEnt[index].m.modCfg.hoInfo.ulBitMap 
       * 2) cpjCfgReq
       */
      (Void)nhPIMRvrtPdcpModCfg(cpjCfgReq, ueCb, transCtrlBlk);

   } /* modCfmStatus */
   
   if(transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_RLS] ||
     transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_RLS] )
   {
      /*R9 upgrade: Kworks Error: ret value check is deleted since this
       * function always returns ROK. in future if somebody changes the
       * ret value then he has to add the check
       */
      (Void)nhPIMRvrtPdcpRlsCfg(cpjCfgReq, ueCb, transCtrlBlk);

   } /* rlsCfmStatus */

   if((NULLP != cpjCfgReq) && (cpjCfgReq->numEnt > 0))
   {
      /* Send Configuration to PDCP layer */
      if (ROK != NhLiCpjCfgReq(&tmpPst, lSap->spId, cpjCfgReq))
      {
         RLOG0(L_ERROR,"Failed to send data to lower layer");
         RETVALUE(RFAILED);
      }
   }
   else
   {
      RLOG0(L_ERROR, "Config parameter is not avaliable to send lower to layer");
      NH_FREE(NH_REG, NH_POOL, cpjCfgReq, sizeof(CpjCfgReqInfo));
   }
   
   RETVALUE(ROK);
}  /* end of nhPIMRvrtPdcpCfg */

/**
 *
 *   @brief Handler to Revert configuration to RLC.
 *
 *   @details
 *   This function is used revert configuration   
 *   which was sent to RLC layer.
 *            
 *
 *   @param[in]  ueCb     UE control block
 *   @param[in]  transCtrlBlk  Transaction control block
 *   @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRvrtRlcCfg
(
NhActvUeCb *ueCb,
TransCb *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhPIMRvrtRlcCfg(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
#endif /* ANSI */
{
   CkwCfgInfo        *ckwCfgParam = NULLP;   /* RLC Config Request Parameter */
   Pst               tmpPst;           /* Post structure */ 
   NhLSapCb          *lSap = NULLP;    /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL; 
   CkwUeInfo         *oldUeInfo = NULLP;
   CkwUeInfo         *newUeInfo = NULLP;
   U8                 i=0;

   TRC2(nhPIMRvrtRlcCfg)
   
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCKWSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCKWSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
       
   }

   tmpPst = lSap->pst;

   if((NH_STATUS_USED == transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_RLC].status) &&
      (CKW_CFG_CFM_OK == 
                  transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_RLC].cfmStatus))
   {
      NH_ALLOC(NH_REG, NH_POOL, &oldUeInfo, sizeof(CkwUeInfo));
      if(NULLP == oldUeInfo)
      {
         RLOG0(L_FATAL, "Could not allocate mem for UE Info");
         RETVALUE(RFAILED);
      }
      NH_ALLOC(NH_REG, NH_POOL, &newUeInfo, sizeof(CkwUeInfo));
      if(NULLP == newUeInfo)
      {
         RLOG0(L_FATAL, "Could not allocate mem for new UE Info");
         NH_FREE(NH_REG, NH_POOL, oldUeInfo, sizeof(CkwUeInfo));
         RETVALUE(RFAILED);
      }
      oldUeInfo->ueId = transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;
      oldUeInfo->cellId = ueCb->cellCb->cellId;

      newUeInfo->ueId = ueCb->ueCbKey.crnti;
      newUeInfo->cellId = ueCb->cellCb->cellId;

      if (ROK != NhLiCkwUeIdChgReq(&tmpPst, lSap->spId, NH_RESERVED_TRANS_ID,
                                             oldUeInfo,  newUeInfo))
      {
          RLOG0(L_ERROR,"Failed to send Rvrt UeId Change Req to RLC");
          RETVALUE(RFAILED);
      }
        
   }
   for(i = 0; i < NH_MAX_RB_ACTION; i++)
   {
      if((transCtrlBlk->rlcMapTbl.drbCfmSta[i]) ||
         (transCtrlBlk->rlcMapTbl.srbCfmSta[i])) 
      {
         /* allocate memory for cfg Parameter */
         NH_ALLOC(NH_REG, NH_POOL, &ckwCfgParam, sizeof(CkwCfgInfo));
         if (NH_CHK_INPUT_PARAM(ckwCfgParam))
         {
            RLOG0(L_FATAL, "GOT NULL ckwCfgParam");
            RETVALUE(RFAILED);
         }
         ckwCfgParam->transId = NH_RESERVED_TRANS_ID;
         ckwCfgParam->ueId = ueCb->ueCbKey.crnti;
         ckwCfgParam->cellId = ueCb->cellCb->cellId;
         break;
      }
   }
   if (i == NH_MAX_RB_ACTION)
   {
      RLOG0(L_DEBUG, "No Logical Channel Id To Rvrt ");
      RETVALUE(ROK);
   }

   if(transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_ADD] ||
     transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_ADD])
   {
      nhPIMFillRvrtRlcAddParam(ckwCfgParam, transCtrlBlk);
   } /* addCfmStatus */
   
   if(transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_MOD] ||
      transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_MOD])
   {
      nhPIMFillRvrtRlcModParam(ckwCfgParam, ueCb, transCtrlBlk);
      
   } /* modCfmStatus */
   
   if(transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_RLS] ||
     transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_RLS])
   {
      nhPIMFillRvrtRlcRlsParam(ckwCfgParam, ueCb, transCtrlBlk);
    
   } /* rlsCfmStatus */
   
   if((ckwCfgParam->numEnt > 0))
   {
      /* Send Configuration to RLC layer */
      if (ROK != NhLiCkwCfgReq(&tmpPst, lSap->spId, ckwCfgParam))
      {
         RLOG0(L_ERROR,"Failed to send data to lower layer");
         RETVALUE(RFAILED);
      }
   }
   else
   {  /* Free allocated memory */
      RLOG0(L_ERROR,"Config parameter is not avaliable to send lower to layer");
      NH_FREE(NH_REG, NH_POOL, ckwCfgParam, sizeof(CkwCfgInfo));
   
  }
   
   RETVALUE(ROK);
}  /* end of nhPIMRvrtRlcCfg */
/**
 *
 *   @brief Handler to Revert Add configuration to MAC.
 *
 *   @details
 *   This function is used revert the Add configuration   
 *   which was sent to MAC layer.
 *            
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Transaction control block
 *   @param[in] cfgType   Configuration type
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRvrtMacAddCfg
(
NhActvUeCb *ueCb,
TransCb *transCtrlBlk,
U8 cfgType
)
#else /* ANSI */
PUBLIC S16 nhPIMRvrtMacAddCfg(ueCb, transCtrlBlk, cfgType)
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
U8 cfgType;
#endif /* ANSI */
{
   CrgCfgReqInfo     *crgCfgParam = NULLP;   /* MAC Config Request Parameter */
   CrgCfgTransId     crgCfgTransId;    /* MAC Transaction Id */
   Pst               tmpPst;           /* Post structure */ 
   NhLSapCb          *lSap = NULLP;    /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL; 
   U8                lcId = NH_ZERO_VAL;
   U8                rvrtAction;
   U8                i = NH_ZERO_VAL;
   U32                tmpUl = NH_ZERO_VAL;
   U32                tmpDl = NH_ZERO_VAL;
   U32               rvrtTransId = NH_RESERVED_TRANS_ID; 
   U8                lcIdFound = NH_ZERO_VAL;
   U8                dir = NH_ZERO_VAL;
   
   TRC2(nhPIMRvrtMacAddCfg)
   
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCRGSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;

   if (cfgType & NH_CRG_UE_CFG)
   {
      /* Allocate memory for cfg parameter */
      NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
      if (NH_CHK_INPUT_PARAM(crgCfgParam))
      {
         RLOG0(L_FATAL, "GOT NULL crgCfgParam");
         RETVALUE(RFAILED);
      }

      NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));

      crgCfgParam->action = CRG_DELETE;
      crgCfgParam->u.delInfo.delType = CRG_UE_CFG;
      crgCfgParam->u.delInfo.u.ueDel.cellId = ueCb->cellCb->cellId;
      crgCfgParam->u.delInfo.u.ueDel.crnti = ueCb->ueCbKey.crnti;

      NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, ueCb->cellCb->cellId,
         ueCb->ueCbKey.crnti, rvrtTransId, NH_CRG_UE_CFG, 
         NH_CFG_ACTN_RLS, NH_ZERO_VAL, NH_ZERO_VAL);

      /* Send Configuration to MAC layer */
      if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, crgCfgParam))
      {
         RLOG0(L_ERROR,"Failed to send data to lower layer");
         RETVALUE(RFAILED);
      }
      /* This return should not be removed, as UE CFG is deleted, all the
       * logical channels for that UE will be deleted by MAC. */
      RETVALUE(ROK);

   }
   if (cfgType & NH_CRG_LOGCH_MAP)
   {
      if(transCtrlBlk->macLchMapTbl[NH_DL_LC].addCfmStatus ||
            transCtrlBlk->macLchMapTbl[NH_UL_LC].addCfmStatus )
      {
         rvrtAction = CRG_DELETE;
         for (i = 0; i < NH_MAX_CFG_REQ; i++)
         {
            lcIdFound = FALSE;
            dir = NH_ZERO_VAL;
            /* Check if logical ID is configured for Down Link */
            tmpDl = (transCtrlBlk->macLchMapTbl[NH_DL_LC].addCfmStatus >> i);
            
            if (tmpDl & 0x01)
            {
               /* Found logical channel ID for Down Link*/
               lcId = i + 1;
               dir = CRG_DIR_TX;
               lcIdFound = TRUE;

            }
            /* Check if logical ID is configured for Up Link */
            tmpUl = (transCtrlBlk->macLchMapTbl[NH_UL_LC].addCfmStatus >> i);
            if (tmpUl & 0x01)
            {
               /* Found logical channel ID for Up Link*/
               lcId = i + 1;
               dir |= CRG_DIR_RX;
               lcIdFound = TRUE;

            }
            if (lcIdFound)
            {
               /* allocate memory for cfg Parameter */
               NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
               if (NH_CHK_INPUT_PARAM(crgCfgParam))
               {
                  RLOG0(L_FATAL, "GOT NULL crgCfgParam");
                  RETVALUE(RFAILED);
               }
               NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
               
               crgCfgParam->action = CRG_DELETE;
               crgCfgParam->u.delInfo.delType = CRG_LCH_CFG;
               crgCfgParam->u.delInfo.u.lchDel.cellId = ueCb->cellCb->cellId;
               crgCfgParam->u.delInfo.u.lchDel.crnti = ueCb->ueCbKey.crnti;
               crgCfgParam->u.delInfo.u.lchDel.lcId = lcId;
               crgCfgParam->u.delInfo.u.lchDel.dir = dir; 
         
         
               NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, 
                              ueCb->cellCb->cellId,
                  ueCb->ueCbKey.crnti, rvrtTransId, NH_CRG_LOGCH_MAP, 
                  NH_CFG_ACTN_RLS, lcId, dir);

               if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, 
                              crgCfgParam))
               {
                  RLOG0(L_ERROR, "Failed to send data to lower layer");
                  RETVALUE(RFAILED);
               }

            } /* if (lcIdFound) */

         } /* for (i = 0; i < NH_MAX_CFG_REQ; i++) */

      }
   }
   
   RETVALUE(ROK);
}  /* end of nhPIMRvrtMacAddCfg */

/**
 *
 *   @brief Handler to Revert Modify configuration to MAC.
 *
 *   @details
 *   This function is used revert the Modify configuration   
 *    which was sent to MAC layer.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] transCtrlBlk  Transaction Cb
 *  @param[in] cfgType  Configuration type
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRvrtMacModCfg
(
NhActvUeCb *ueCb,
TransCb *transCtrlBlk,
U8 cfgType
)
#else /* ANSI */
PUBLIC S16 nhPIMRvrtMacModCfg(ueCb, transCtrlBlk, cfgType)
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
U8 cfgType;
#endif /* ANSI */
{
   CrgCfgReqInfo       *crgCfgParam = NULLP;  /* MAC Configuration Request parameter */
   CrgCfgTransId       crgCfgTransId;         /* MAC Transaction ID */
   Pst                 tmpPst;                /* Post structure */ 
   NhLSapCb            *lSap = NULLP;         /* Lower SAP Cb */
   SuId                suId = NH_ZERO_VAL; 
   U16                 ueId = NH_ZERO_VAL;
   U32                 rvrtTransId = NH_RESERVED_TRANS_ID;  
   
   TRC2(nhPIMRvrtMacModCfg)
   RLOG0(L_DEBUG, "nhPIMRvrtMacModCfg");
  
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCRGSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;
  
   
   ueId = ueCb->ueCbKey.crnti;
   

   if (cfgType & NH_CRG_UE_CFG)
   {
      /* Re-configuration to MAC */
      NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
      if (NH_CHK_INPUT_PARAM(crgCfgParam))
      {
         RLOG0(L_FATAL, "GOT NULL crgCfgParam");
         RETVALUE(RFAILED);
      }
      NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
      
      nhPIMFillRvrtMacModUeCfg(crgCfgParam, ueCb, transCtrlBlk);
           
      NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, ueCb->cellCb->cellId,
         ueId, rvrtTransId, NH_CRG_UE_CFG, 
         NH_CFG_ACTN_MODIFY, NH_ZERO_VAL, NH_ZERO_VAL);

      /* Send Configuration to MAC layer */
      if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, crgCfgParam))
      {
         RLOG0(L_ERROR,"Failed to send data to lower layer");
         RETVALUE(RFAILED);
      }
   }
   if ((cfgType & NH_CRG_LOGCH_MAP) &&
         (transCtrlBlk->macLchMapTbl[NH_UL_LC].modCfmStatus))
   {

      if (ROK != nhPIMRvrtMacModLchCfg(ueCb, transCtrlBlk, lSap))
      {
         RLOG0(L_ERROR, "Failed to Revert modify logical channel");
         RETVALUE(RFAILED);
      }
     
   }
          
   RETVALUE(ROK);
}  /* end of nhPIMRvrtMacModCfg */

/**
 *
 *   @brief Handler to Revert Modify Logical channel configuration to MAC
 *
 *   @details
 *   This function is used revert the Modify logical channel configuration   
 *    which was sent to MAC layer.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] transCtrlBlk  Trans Cb
 *  @param[in] lSap     sap control block
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMRvrtMacModLchCfg
(
NhActvUeCb *ueCb,
TransCb *transCtrlBlk,
NhLSapCb *lSap
)
#else /* ANSI */
PRIVATE S16 nhPIMRvrtMacModLchCfg(ueCb, transCtrlBlk, lSap)
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
NhLSapCb *lSap;
#endif /* ANSI */
{
   U8                  i =0;
   U8                  lcId = NH_ZERO_VAL;
   U32                 tmpUl = NH_ZERO_VAL;
   U8                  dir = NH_ZERO_VAL;
   
   TRC2(nhPIMRvrtMacModLchCfg)
 
   RLOG0(L_DEBUG, "nhPIMRvrtMacModLchCfg");
   
   for (i = 0; i < NH_MAX_CFG_REQ; i++)
   {
      dir = NH_ZERO_VAL;

         
         /* Uplink logical ID */
      tmpUl = (transCtrlBlk->macLchMapTbl[NH_UL_LC].modCfmStatus >> i);
         
      if (tmpUl & 0x01)
      {
         lcId = i + 1; 
         dir |= NH_LCID_DIR_RX;
      }
        
      if (NH_LCID_DIR_RX == dir )
      {
         if (ROK != nhPIMRvrtMacModRXLch(ueCb, lSap, lcId))
         {
            RLOG0(L_ERROR, "Failed to Revert modify RX logical channel");
            RETVALUE(RFAILED);
         }
      }
   } /* for (i = 0; i < NH_MAX_CFG_REQ; i++) */

   RETVALUE(ROK);
}/* End of nhPIMRvrtMacModLchCfg */

/**
 *
 *   @brief Handler to Revert Rlease Logical channel configuration to MAC
 *
 *   @details
 *   This function is used revert the Rlease logical channel configuration   
 *    which was sent to MAC layer.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] transCtrlBlk  Trans Cb
 *  @param[in] lSap     sap control block
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMRvrtMacRlsLchCfg
(
NhActvUeCb *ueCb,
TransCb *transCtrlBlk,
NhLSapCb *lSap
)
#else /* ANSI */
PRIVATE S16 nhPIMRvrtMacRlsLchCfg(ueCb, transCtrlBlk, lSap)
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
NhLSapCb *lSap;
#endif /* ANSI */
{
   
  
   U8                  i = 0;
   U8                  lcId = NH_ZERO_VAL;
   U32                 tmpUl = NH_ZERO_VAL;
   U32                 tmpDl = NH_ZERO_VAL;
   U8                  dir = NH_ZERO_VAL;
   
   TRC2(nhPIMRvrtMacRlsLchCfg)
 
   RLOG0(L_DEBUG, "nhPIMRvrtMacRlsLchCfg");
   
   for (i = 0; i < NH_MAX_CFG_REQ; i++)
   {
      dir = NH_ZERO_VAL;
      
      /* Down link logical id */
      tmpDl = (transCtrlBlk->macLchMapTbl[NH_DL_LC].rlsCfmStatus >> i);
         
      /* Up link logical id */
      tmpUl = (transCtrlBlk->macLchMapTbl[NH_UL_LC].rlsCfmStatus >> i);
         
         if (tmpDl & 0x01)
         {
            lcId = i + 1;
            dir = NH_LCID_DIR_TX;
         }
         if (tmpUl & 0x01)
         {
            lcId = i + 1; 
            dir |= NH_LCID_DIR_RX;
         }
         
         if (NH_LCID_DIR_TXRX == dir)
         {
            if (ROK != nhPIMRvrtMacRlsTXRXLch(ueCb, lSap, lcId))
            {
               RLOG0(L_ERROR, "Failed to Revert of release RXTX logical channel");
               RETVALUE(RFAILED);    
            }
            
         }/*  if direction id TXRX */
         else
         {
            if (NH_LCID_DIR_RX == dir )
            {
               if (ROK != nhPIMRvrtMacRlsRXLch(ueCb, lSap, lcId))
               {
                  RLOG0(L_ERROR, "Failed to Revert of release RX logical channel");
                  RETVALUE(RFAILED);
               }
                              
            }
            if (NH_LCID_DIR_TX == dir )
            {
               if (ROK != nhPIMRvrtMacRlsTXLch(ueCb, lSap, lcId))
               {
                  RLOG0(L_ERROR, "Failed to Revert of release TX logical channel");
                  RETVALUE(RFAILED);
               }
               
               
            }/* if (NH_LCID_DIR_TX == dir ) */
         
         } /* else of RXTX */

      } /* for (i = 0; i < NH_MAX_CFG_REQ; i++) */

   RETVALUE(ROK);
   
}/* nhPIMRvrtMacRlsLchCfg */

/**
 *
 *   @brief Handler to Revert Release TXRX Logical channel configuration to MAC
 *
 *   @details
 *   This function is used revert the Release lTXRX ogical channel configuration
 *    which was sent to MAC layer.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] lSap     sap control block
 *  @param[in] lcId   Logical Channel ID
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMRvrtMacRlsTXRXLch
(
NhActvUeCb *ueCb,
NhLSapCb *lSap,
U8  lcId
)
#else /* ANSI */
PRIVATE S16 nhPIMRvrtMacRlsTXRXLch(ueCb, lSap, lcId)
NhActvUeCb *ueCb;
NhLSapCb *lSap;
U8  lcId;
#endif /* ANSI */
{
   NhRbToLchMap  *crntLchCfg = NULLP;
   S16   ret = RFAILED;
   
   TRC2(nhPIMRvrtMacRlsTXRXLch)
   
   RLOG0(L_DEBUG, "nhPIMRvrtMacRlsTXRXLch");
   crntLchCfg = nhPIMGetRvrtMacLch(ueCb, NH_LCID_DIR_TXRX, lcId);
   if (crntLchCfg != NULLP)
   {
      ret = nhPIMSendRvrtMacRlsLchCfg(ueCb, crntLchCfg, lSap); 
           
   }
   RETVALUE(ret);

}/* nhPIMRvrtMacRlsTXRXLch */

/**
 *
 *   @brief Handler to Revert release TX Logical channel configuration to MAC
 *
 *   @details
 *   This function is used revert the release TX Logical channel configuration  
 *    which was sent to MAC layer.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] lSap     sap control block
 *  @param[in] lcId   Logical Channel ID
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMRvrtMacRlsTXLch
(
NhActvUeCb *ueCb,
NhLSapCb *lSap,
U8  lcId
)
#else /* ANSI */
PRIVATE S16 nhPIMRvrtMacRlsTXLch(ueCb, lSap, lcId)
NhActvUeCb *ueCb;
NhLSapCb *lSap;
U8 lcId;
#endif /* ANSI */
{
   NhRbToLchMap  *crntLchCfg = NULLP;
   S16   ret = RFAILED;
   
   TRC2(nhPIMRvrtMacRlsTXLch)
   RLOG0(L_DEBUG, "nhPIMRvrtMacRlsTXLch");
   
   crntLchCfg = nhPIMGetRvrtMacLch(ueCb, NH_LCID_DIR_TX, lcId);
   if (crntLchCfg != NULLP)
   {
      ret = nhPIMSendRvrtMacRlsLchCfg(ueCb, crntLchCfg, lSap); 
           
   }
   RETVALUE(ret);

}/* nhPIMRvrtMacRlsTXLch */

/**
 *
 *   @brief Handler to Revert Release RX Logical channel configuration to MAC
 *
 *   @details
 *   This function is used revert the release RX Logical channel configuration  
 *    which was sent to MAC layer.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] lSap     sap control block
 *  @param[in] lcId   Logical Channel ID
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMRvrtMacRlsRXLch
(
NhActvUeCb *ueCb,
NhLSapCb *lSap,
U8 lcId
)
#else /* ANSI */
PRIVATE S16 nhPIMRvrtMacRlsRXLch(ueCb, lSap, lcId)
NhActvUeCb *ueCb;
NhLSapCb *lSap;
U8  lcId;
#endif /* ANSI */
{
   NhRbToLchMap  *crntLchCfg = NULLP;
   S16   ret = RFAILED;

   TRC2(nhPIMRvrtMacRlsRXLch)
   
   RLOG0(L_DEBUG, "nhPIMRvrtMacRlsRXLch");
   crntLchCfg = nhPIMGetRvrtMacLch(ueCb, NH_LCID_DIR_RX, lcId);
   if (crntLchCfg != NULLP)
   {
      ret = nhPIMSendRvrtMacRlsLchCfg(ueCb, crntLchCfg, lSap); 
           
   }
   RETVALUE(ret);

}/* nhPIMRvrtMacRlsRXLch */

/**
 *
 *   @brief Handler to Revert Modify RX Logical channel configuration to MAC
 *
 *   @details
 *   This function is used revert the Modify RX Logical channel configuration   
 *    which was sent to MAC layer.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] lSap     sap control block
 *  @param[in] lcId   Logical Channel ID
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMRvrtMacModRXLch
(
NhActvUeCb *ueCb,
NhLSapCb *lSap,
U8 lcId
)
#else /* ANSI */
PRIVATE S16 nhPIMRvrtMacModRXLch(ueCb, lSap, lcId)
NhActvUeCb *ueCb;
NhLSapCb *lSap;
U8 lcId; 
#endif /* ANSI */
{
   NhRbToLchMap  *crntLchCfg = NULLP;
   S16   ret = RFAILED;

   TRC2(nhPIMRvrtMacModRXLch)
   
   crntLchCfg = nhPIMGetRvrtMacLch(ueCb, NH_LCID_DIR_RX, lcId);
   if (crntLchCfg != NULLP)
   {
      ret = nhPIMSendRvrtMacModLchCfg(ueCb, crntLchCfg, lSap); 
           
   }
   RETVALUE(ret);

}/* End of nhPIMRvrtMacModRXLch */

/**
 *
 *   @brief Handler to Revert release Logical channel configuration to MAC
 *
 *   @details
 *   This function is used revert the released logical channel configuration   
 *    which was sent to MAC layer.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] crntLchCfg  RB to Logical channel mapping information
 *  @param[in] lSap     sap control block
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMSendRvrtMacRlsLchCfg
(
NhActvUeCb *ueCb,
NhRbToLchMap  *crntLchCfg,
NhLSapCb *lSap
)
#else /* ANSI */
PRIVATE S16 nhPIMSendRvrtMacRlsLchCfg(ueCb, crntLchCfg, lSap)
NhActvUeCb *ueCb;
NhRbToLchMap  *crntLchCfg;
NhLSapCb *lSap;
#endif /* ANSI */
{

   CrgCfgReqInfo       *crgCfgParam = NULLP;  /* MAC Config Request Parameter */
   CrgCfgTransId       crgCfgTransId;        /* MAC Transaction Id */
   Pst                 tmpPst;              /* Post structure */ 
   U16                 ueId = NH_ZERO_VAL;
   U32                 rvrtTransId = NH_RESERVED_TRANS_ID; 
   
   TRC2(nhPIMSendRvrtMacRlsLchCfg)
   RLOG0(L_DEBUG, "nhPIMSendRvrtMacRlsLchCfg");

   tmpPst = lSap->pst;

   ueId = ueCb->ueCbKey.crnti;
   /* reconfiguration to MAC */
   NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
   if (NH_CHK_INPUT_PARAM(crgCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL crgCfgParam");
      RETVALUE(RFAILED);
   }
   NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
      
   /* configuration of logical channel */
   
   crgCfgParam->action = CRG_CONFIG;
   crgCfgParam->u.cfgInfo.cfgType = CRG_LCH_CFG;
   crgCfgParam->u.cfgInfo.u.lchCfg.cellId = ueCb->cellCb->cellId;
   crgCfgParam->u.cfgInfo.u.lchCfg.crnti = ueId;


    nhPIMFillMacRvrtLchRlsCfgParam(&crgCfgParam->u.cfgInfo.u.lchCfg,
                     &crntLchCfg->logChCfg);
    NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, ueCb->cellCb->cellId,
                  ueId, rvrtTransId, NH_CRG_LOGCH_MAP, 
               NH_CFG_ACTN_MODIFY, crntLchCfg->lchId.lcId, crntLchCfg->dir);
    /* Send Configuration to MAC layer */
    if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, crgCfgParam))
    {
        RLOG0(L_ERROR,"Failed to send data to lower layer");
        RETVALUE(RFAILED);
    }
    RETVALUE(ROK);
  
}/* nhPIMSendRvrtMacRlsLchCfg */


/**
 *
 *   @brief Handler to Revert Modify Logical channel configuration to MAC
 *
 *   @details
 *   This function is used revert the Modify logical channel configuration   
 *    which was sent to MAC layer.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] crntLchCfg  RB to Logical channel mapping information
 *  @param[in] lSap     sap control block
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMSendRvrtMacModLchCfg
(
NhActvUeCb *ueCb,
NhRbToLchMap  *crntLchCfg,
NhLSapCb *lSap
)
#else /* ANSI */
PRIVATE S16 nhPIMSendRvrtMacModLchCfg(ueCb, crntLchCfg, lSap)
NhActvUeCb *ueCb;
NhRbToLchMap  *crntLchCfg;
NhLSapCb *lSap;
#endif /* ANSI */
{

   CrgCfgReqInfo       *crgCfgParam = NULLP;  /* MAC Config Request Parameter */
   CrgCfgTransId       crgCfgTransId;        /* MAC Transaction Id */
   Pst                 tmpPst;              /* Post structure */ 
   U16                 ueId = NH_ZERO_VAL;
   U32                 rvrtTransId = NH_RESERVED_TRANS_ID; 
   
   TRC2(nhPIMSendRvrtMacModLchCfg)
   RLOG0(L_DEBUG, "nhPIMSendRvrtMacModLchCfg");

   tmpPst = lSap->pst;

   ueId = ueCb->ueCbKey.crnti;
   /* reconfiguration to MAC */
   NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
   if (NH_CHK_INPUT_PARAM(crgCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL crgCfgParam");
      RETVALUE(RFAILED);
   }
   NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
      
   /* reconfiguration of logical channel */
   crgCfgParam->action = CRG_RECONFIG;
   crgCfgParam->u.recfgInfo.recfgType = CRG_LCH_CFG;
   crgCfgParam->u.recfgInfo.u.lchRecfg.cellId = ueCb->cellCb->cellId;
   crgCfgParam->u.recfgInfo.u.lchRecfg.crnti = ueId;
   

   nhPIMFillMacRvrtLchReCfgParam(&crgCfgParam->u.recfgInfo.u.lchRecfg, 
                                 &crntLchCfg->logChCfg);
   NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, ueCb->cellCb->cellId,
         ueId, rvrtTransId, NH_CRG_LOGCH_MAP, 
         NH_CFG_ACTN_MODIFY, crntLchCfg->lchId.lcId, crntLchCfg->dir);
   /* Send Configuration to MAC layer */
   if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, crgCfgParam))
   {
       RLOG0(L_ERROR, "Failed to send data to lower layer");
       RETVALUE(RFAILED);
   }
   RETVALUE(ROK);

}/* End of nhPIMSendRvrtMacModLchCfg */
/**
 *
 *   @brief To get logical channel for MAC
 *
 *   @details
 *   This function is used get the logical channel which needs to be revert 
 *    for MAC layer.
 *            
 *
 *  @param[in] ueCb   UE control block
 *  @param[in] dir    direction of logical channel
 *  @param[in] lcId   Logical Channel ID
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE NhRbToLchMap* nhPIMGetRvrtMacLch
(
NhActvUeCb *ueCb,
U8  dir,
U8  lcId
)
#else /* ANSI */
PRIVATE NhRbToLchMap* nhPIMGetRvrtMacLch(ueCb, dir, lcId)
NhActvUeCb *ueCb;
U8   dir;
U8   lcId;
#endif /* ANSI */
{

   CmLList             *tmpNode = NULLP;
   NhRbToLchMap  *crntLchCfg = NULLP; 
   
   TRC2(nhPIMGetRvrtMacLch)
   
   RLOG0(L_DEBUG, "nhPIMGetRvrtMacLch");
   /* Find logical channel id */
   CM_LLIST_FIRST_NODE(&(ueCb->crntCfg.logChCbLst), tmpNode);
   while(NULLP != tmpNode)
   {
      crntLchCfg = (NhRbToLchMap *)(tmpNode->node);
      if((lcId == crntLchCfg->lchId.lcId) && 
            (dir == crntLchCfg->dir))
      {
         
         break;
      }
      CM_LLIST_NEXT_NODE(&ueCb->crntCfg.logChCbLst, tmpNode);
   }
   
   RETVALUE(crntLchCfg);

}/* nhPIMGetRvrtMacModLch */

/**
 *
 *   @brief Function to get Rb on basic of rbId 
 *
 *   @details
 *   This function is used get Rb on basic of rbId whic needs to 
 *   revert at RLC.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] rbId     RB ID information
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE NhuRbCfgInfo* nhPIMGetRvrtRlcRb
(
NhActvUeCb *ueCb,
NhuRbId  rbId
)
#else /* ANSI */
PRIVATE NhuRbCfgInfo* nhPIMGetRvrtRlcRb(ueCb, rbId)
NhActvUeCb *ueCb;
NhuRbId   rbId;
#endif /* ANSI */
{

   CmLList             *tmpNode = NULLP;
   NhuRbCfgInfo        *crntRbCfg = NULLP; 
   

   TRC2(nhPIMGetRvrtRlcRb)

   RLOG0(L_DEBUG, "nhPIMGetRvrtRlcRb");
   /* traverse the rb list */
   CM_LLIST_FIRST_NODE(&ueCb->crntCfg.rbCfgList.rbCfgInfoList, tmpNode);
   while(NULLP != tmpNode)
   {
     crntRbCfg = (NhuRbCfgInfo *)tmpNode->node;
     /* if rbId matches break and return */
     if(rbId.rbId == crntRbCfg->rbId.rbId && rbId.rbTyp == 
                     crntRbCfg->rbId.rbTyp)
     {
        break;
     }
     CM_LLIST_NEXT_NODE(&ueCb->crntCfg.rbCfgList.rbCfgInfoList, tmpNode);
   }
   RETVALUE(crntRbCfg);

     
}/* nhPIMGetRvrtRlcLch */
/**
 *
 *   @brief Handler to Revert Rls configuration to MAC
 *
 *   @details
 *   This function is used revert the Rls configuration   
 *   which was sent to MAC layer.
 *            
 *
 *   @param[in] ueCb      Ue control block
 *   @param[in] transCtrlBlk   Transacton control block
 *   @param[in] cfgType   confgi type
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRvrtMacRlsCfg
(
NhActvUeCb *ueCb,
TransCb *transCtrlBlk,
U8 cfgType
)
#else /* ANSI */
PUBLIC S16 nhPIMRvrtMacRlsCfg(ueCb, transCtrlBlk, cfgType)
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
U8 cfgType;
#endif /* ANSI */
{
   Pst                   tmpPst;               /* Post structure */ 
   NhLSapCb              *lSap = NULLP;        /* Lower Sap Cb */
   SuId                  suId = 0; 
   NhOrdrdCfgInfo        *ordrdCfg = NULLP;
   
   TRC2(nhPIMRvrtMacRlsCfg)
   RLOG0(L_DEBUG, "nhPIMRvrtMacRlsCfg");

   /* get lower Sap */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCRGSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;
  
   ordrdCfg = &transCtrlBlk->ordrdCfg;
   
   if ((cfgType & NH_CRG_LOGCH_MAP) && 
         (transCtrlBlk->macLchMapTbl[NH_DL_LC].rlsCfmStatus
          ||transCtrlBlk->macLchMapTbl[NH_UL_LC].rlsCfmStatus ))
   {

      if (ROK != nhPIMRvrtMacRlsLchCfg(ueCb, transCtrlBlk, lSap))
      {
         RLOG0(L_ERROR, "Failed to Revert of released logical channel");
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}  /* end of nhPIMRvrtMacRlsCfg */

/**
 *
 *   @brief Filler function for RLC request data.
 *
 *   @details
 *   This function is used to fill RLC data request parameter.
 *            
 *   @param[out] kwDatReqParam   RLC data request
 *   @param[in]  rlcParam        RLC related TM parameters
 *
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMFillRlcDatReqParams
(
KwuDatReqInfo   *kwDatReqParam,
NhTmRlcDatReqParam *rlcParam 
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRlcDatReqParams(kwDatReqParam, rlcParam)
KwuDatReqInfo   *kwDatReqParam;
NhTmRlcDatReqParam  *rlcParam; 
#endif /* ANSI */
{
   TRC2( nhPIMFillRlcDatReqParams)
   RLOG0(L_DEBUG, "nhPIMFillRlcDatReqParams");

#ifdef CCPU_OPT
   kwDatReqParam->lcType = rlcParam->lcType;

   if ((rlcParam->lcType == CM_LTE_LCH_BCCH) ||
       (rlcParam->lcType == CM_LTE_LCH_PCCH))
   {
      NH_COPY_TMG_INFO(kwDatReqParam->tm.tmg, rlcParam->u.timingInfo);
   }
   else /* CCCH */
   {
      kwDatReqParam->tm.rnti = rlcParam->u.ccchInfo.rnti;
   }
#endif /* CCPU_OPT */

   RETVALUE(ROK);

} /* nhPIMFillRlcDatReqParams */

/**
 *
 *   @brief Filling function to fill crgInfo for Ue 
 *
 *   @details
 *   This function is used to fill Ue parameter for revert the Modify configuration   
 *    
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] transCtrlBlk  Trans Cb
 *  @param[out] crgCfgParam  config Parameter
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      

#ifdef ANSI
PRIVATE S16 nhPIMFillRvrtMacModUeCfg
(
CrgCfgReqInfo  *crgCfgParam,
NhActvUeCb *ueCb,
TransCb *transCtrlBlk
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRvrtMacModUeCfg(crgCfgParam, ueCb, transCtrlBlk) 
CrgCfgReqInfo  *crgCfgParam;
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
#endif /* ANSI */
{
   U16          ueId = NH_ZERO_VAL;

   TRC2(nhPIMFillRvrtMacModUeCfg)
   RLOG0(L_DEBUG, "nhPIMFillRvrtMacModUeCfg");

   ueId = ueCb->ueCbKey.crnti;

   crgCfgParam->action = CRG_RECONFIG;
   crgCfgParam->u.recfgInfo.recfgType = CRG_UE_CFG;
   crgCfgParam->u.recfgInfo.u.ueRecfg.cellId = ueCb->cellCb->cellId;
   if(transCtrlBlk->ordrdCfg.ueInfo.crntiCfgType == NH_UE_INFO_MOD_CRNTI)
   {
      crgCfgParam->u.recfgInfo.u.ueRecfg.oldCrnti = 
              transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;
   }   
   else
   {
     crgCfgParam->u.recfgInfo.u.ueRecfg.oldCrnti = ueCb->ueCbKey.crnti;
   }   
   crgCfgParam->u.recfgInfo.u.ueRecfg.newCrnti = ueId;
      
   if (ueCb->crntCfg.bitMskPres & NH_CRNT_CFG_TRCH )
   {
     crgCfgParam->u.recfgInfo.u.ueRecfg.ueUlHqRecfg.maxUlHqTx =
           ueCb->crntCfg.trChCfg.ulHarqCfg.ulMaxHarqTx;
   }

   RETVALUE(ROK);

}/* End of nhPIMFillRvrtMacModUeCfg */

/**
 *
 *   @brief Filling function to fill ckwInfo for Revert Add parameter 
 *
 *   @details
 *   This function is used to fill Revert parameter for add configuration   
 *    
 *            
 *
 *  @param[out] ckwCfgParam  config Parameter
 *  @param[in] transCtrlBlk  Trans Cb
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      

#ifdef ANSI
PRIVATE S16 nhPIMFillRvrtRlcAddParam
(
CkwCfgInfo  *ckwCfgParam,
TransCb *transCtrlBlk
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRvrtRlcAddParam (ckwCfgParam, transCtrlBlk) 
CkwCfgInfo  *ckwCfgParam;
TransCb *transCtrlBlk;
#endif /* ANSI */
{
   U8                i = NH_ZERO_VAL;
   U8                rbId; 
   U8                rvrtAction;
   U8                rbCount = NH_ZERO_VAL;
   U32               tmp = NH_ZERO_VAL;
   
   
   TRC2(nhPIMFillRvrtRlcAddParam)
   RLOG0(L_DEBUG, "nhPIMFillRvrtRlcAddParam");

   rvrtAction = CKW_CFG_DELETE;
   for (i = 0; i < NH_MAX_CFG_REQ; i++)
   {
      tmp = (transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_ADD] >> i);
         
      if (tmp & 0x01)
      {
         rbId = i + 1; 
         rbCount = ckwCfgParam->numEnt;
         ckwCfgParam->entCfg[rbCount].cfgType = rvrtAction;
         ckwCfgParam->entCfg[rbCount].rbId = rbId;
         ckwCfgParam->entCfg[rbCount].rbType = CM_LTE_DRB;
         ckwCfgParam->numEnt++;
      }
      tmp = (transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_ADD] >> i);
         
      if (tmp & 0x01)
      {
         rbId = i + 1; 
         rbCount = ckwCfgParam->numEnt;
         ckwCfgParam->entCfg[rbCount].cfgType = rvrtAction;
         ckwCfgParam->entCfg[rbCount].rbId = rbId;
         ckwCfgParam->entCfg[rbCount].rbType = CM_LTE_SRB;
         ckwCfgParam->numEnt++;
      }
   }
   RETVALUE(ROK);
}/* nhPIMFillRvrtRlcAddParam */


/**
 *
 *   @brief Filling function to fill ckwInfo for Revert Mod parameter 
 *
 *   @details
 *   This function is used to fill Revert parameter for modify configuration   
 *    
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] transCtrlBlk  Trans Cb
 *  @param[out] ckwCfgParam  config Parameter
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      

#ifdef ANSI
PRIVATE S16 nhPIMFillRvrtRlcModParam
(
CkwCfgInfo  *ckwCfgParam,
NhActvUeCb *ueCb,
TransCb *transCtrlBlk
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRvrtRlcModParam (ckwCfgParam, ueCb, transCtrlBlk) 
CkwCfgInfo  *ckwCfgParam;
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
#endif /* ANSI */
{
   U8                i = NH_ZERO_VAL;
   NhuRbId           rbId;
   U8                rvrtAction;
   NhuRbCfgInfo      *crntRbCfg = NULLP;
   U32               tmp = NH_ZERO_VAL;
   
   TRC2(nhPIMFillRvrtRlcModParam)
   RLOG0(L_DEBUG, "nhPIMFillRvrtRlcModParam");

   rvrtAction = CKW_CFG_MODIFY;
   for (i = 0; i < NH_MAX_CFG_REQ; i++)
   {
      tmp = (transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_MOD] >> i);
      /* if CfmStatus is set */       
      if (tmp & 0x01)
     {
        crntRbCfg = NULLP;
        rbId.rbId = i + 1; 
        rbId.rbTyp = CM_LTE_DRB;
       /* Get the Rb Config Info */
       crntRbCfg = nhPIMGetRvrtRlcRb(ueCb, rbId);
       if (NULLP == crntRbCfg)
       {
            RLOG0(L_ERROR, "Failed to find Rb for revert");
            RETVALUE(RFAILED);
       }
       else
       {
         /* Fill Config Info for Revert */  
          nhPIMFillRlcRvrtCfgParam(ckwCfgParam, crntRbCfg, rvrtAction);
       }
   
     }/* if (tmp & 0x01 ) */

     tmp = (transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_MOD] >> i);
      /* if CfmStatus is set */       
      if (tmp & 0x01)
     {
        crntRbCfg = NULLP;
        rbId.rbId = i + 1; 
        rbId.rbTyp = CM_LTE_SRB;
       /* Get the Rb Config Info */
       crntRbCfg = nhPIMGetRvrtRlcRb(ueCb, rbId);
       if (NULLP == crntRbCfg)
       {
            RLOG0(L_ERROR, "Failed to find Rb for revert");
            RETVALUE(RFAILED);
       }
       else
       {
         /* Fill Config Info for Revert */  
          nhPIMFillRlcRvrtCfgParam(ckwCfgParam, crntRbCfg, rvrtAction);
       }
   
     }/* if (tmp & 0x01 ) */

   
   }

   RETVALUE(ROK);
}/* End of nhPIMFillRvrtRlcModParam */

/**
 *
 *   @brief Filling function to fill ckwInfo for Revert Rls parameter 
 *
 *   @details
 *   This function is used to fill Revert parameter for Rls configuration   
 *    
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] transCtrlBlk  Trans Cb
 *  @param[out] ckwCfgParam  config Parameter
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      

#ifdef ANSI
PRIVATE S16 nhPIMFillRvrtRlcRlsParam
(
CkwCfgInfo  *ckwCfgParam,
NhActvUeCb *ueCb,
TransCb *transCtrlBlk
)
#else /* ANSI */
PRIVATE S16 nhPIMFillRvrtRlcRlsParam (ckwCfgParam, ueCb, transCtrlBlk) 
CkwCfgInfo  *ckwCfgParam;
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
#endif /* ANSI */
{
   U8                i = NH_ZERO_VAL;
   NhuRbId           rbId;
   U8                rvrtAction;
   NhuRbCfgInfo      *crntRbCfg = NULLP;
   U32               tmp = NH_ZERO_VAL;
   
   TRC2(nhPIMFillRvrtRlcRlsParam)
   RLOG0(L_DEBUG, "nhPIMFillRvrtRlcRlsParam");


   rvrtAction = CKW_CFG_ADD;
  
   for (i = 0; i < NH_MAX_CFG_REQ; i++)
   {
      tmp = (transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_RLS] >> i);
      /* if CfmStatus is set */   
      if (tmp & 0x01)
      {
         /* RbId */
         rbId.rbId = i + 1;
         rbId.rbTyp = CM_LTE_DRB;
         /* Get the Rb Config Info */
         crntRbCfg = nhPIMGetRvrtRlcRb(ueCb, rbId);
         if (NULLP == crntRbCfg)
         {
             RLOG0(L_ERROR, "Failed to find Rb for revert");
             RETVALUE(RFAILED);
         }
         else
         {
            /* Fill Config Info for Revert */
            nhPIMFillRlcRvrtCfgParam(ckwCfgParam, crntRbCfg, rvrtAction);
         }

      } /* if (tmp & 0x01 ) */
   }

   RETVALUE(ROK);
}/* End of nhPIMFillRvrtRlcRlsParam */

/**
 *
 *   @brief Handler to send Count Reqeust to PDCP.
 *
 *   @details
 *   This function sends Count Request to PDCP to get COUNT values
 *   of all DRBs that are established.
 *            
 *
 *   @param[in] cntHdr    Count Request header 
 *   @param[in] ueCb      UE control block
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSndCountReq
(
NhuHdr    *cntHdr,
NhActvUeCb     *ueCb
)
#else /* ANSI */
PUBLIC S16 nhPIMSndCountReq(cntHdr, ueCb)
NhuHdr    *cntHdr;
NhActvUeCb        *ueCb;
#endif /* ANSI */
{
   U16               ret;
   Pst               tmpPst;
   NhLSapCb          *lSap = NULLP;          /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL;
   CpjCountReqInfo *cpjCountReqInfo = NULLP;

   TRC2(nhPIMSndCountReq)

   UNUSED(ueCb);

   RLOG0(L_DEBUG, "nhPIMSndCountReq");

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);
   if (NULLP == lSap || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer");
      RETVALUE(RFAILED);
   }
   NH_ALLOC(NH_REG, NH_POOL, &cpjCountReqInfo, sizeof(CpjCountReqInfo));

   if (cpjCountReqInfo == NULLP)
   {
      RLOG0(L_FATAL,"Could not alocate mem for cpjCountReqInfo");
      RETVALUE(RFAILED);
   }

   cpjCountReqInfo->ueId = cntHdr->ueId;
   cpjCountReqInfo->cellId = cntHdr->cellId;
   cpjCountReqInfo->transId = cntHdr->transId;

   /* Send request to PDCP */
   tmpPst = lSap->pst;
   ret = NhLiCpjCountReq(&tmpPst, lSap->spId, cpjCountReqInfo);
   if (ret != ROK)
   {
      RLOG0(L_ERROR,"Could not send count req to PDCP");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* nhPIMSndCountReq */


/**
 *
 *   @brief Handler to reset UE specific configuration at MAC.
 *
 *   @details
 *   This function is used to reset configuration specific   
 *   to a UE at MAC.
 *            
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Transaction control block
 *   @param[in] cfgType   Configuration Type
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendMacResetCfg
(
NhActvUeCb *ueCb,       /* UE Cb */
TransCb *transCtrlBlk,       /* Transaction Cb */
U8 cfgType              /* configuration type */
)
#else /* ANSI */
PUBLIC S16 nhPIMSendMacResetCfg(ueCb, transCtrlBlk, cfgType)
NhActvUeCb *ueCb;       /* UE Cb */
TransCb *transCtrlBlk;       /* Transaction Cb */ 
U8 cfgType;             /* configuration type */
#endif /* ANSI */
{
   CrgCfgReqInfo         *crgCfgParam = NULLP;   /* MAC Configuration Request parameter */
   CrgCfgTransId         crgCfgTransId;          /* MAC Transaction ID */
   Pst                   tmpPst;                 /* Post structure */ 
   NhLSapCb              *lSap = NULLP;          /* Lower SAP Cb */
   SuId                  suId = NH_ZERO_VAL;     /* Service user ID */ 

   TRC2(nhPIMSendMacResetCfg)

   RLOG0(L_DEBUG,"Rlsing Cfg at MAC  ");

#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCRGSAP)
#endif /* NHU_MULT_SAP_SUPPRT */

   /* Get lower SAP */
   lSap = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
       
   }

   tmpPst = lSap->pst;
   
   if (cfgType & NH_CRG_UE_MAC_RST)
   {
      /* Allocate memory for cfg Parameter */
      NH_ALLOC(NH_REG, NH_POOL, &crgCfgParam, sizeof(CrgCfgReqInfo));
      if (NH_CHK_INPUT_PARAM(crgCfgParam))
      {
         RLOG0(L_FATAL, "GOT NULL crgCfgParam");
         RETVALUE(RFAILED);
      }
      /* Set zero */
      NH_SET_ZERO(&crgCfgTransId, sizeof(CrgCfgTransId));
      /* Reset-configuration */
      crgCfgParam->action = CRG_RESET;
      crgCfgParam->u.rstInfo.crnti = ueCb->ueCbKey.crnti;
      crgCfgParam->u.rstInfo.cellId = ueCb->cellCb->cellId;
     
      /* Construct transaction ID */
      NH_GET_MAC_CFG_REQ_TRANSID(crgCfgTransId.trans, ueCb->cellCb->cellId,
         ueCb->ueCbKey.crnti, transCtrlBlk->nxTransId, NH_CRG_UE_MAC_RST, 
         NH_CFG_ACTN_MODIFY, NH_ZERO_VAL, NH_ZERO_VAL);

      transCtrlBlk->numOfMacCfms++;

      /* Send Configuration to MAC layer */
      if (ROK != NhLiCrgCfgReq(&tmpPst, lSap->spId, crgCfgTransId, crgCfgParam))
      {
         RLOG0(L_ERROR,"Failed to send data to lower layer");
         transCtrlBlk->numOfMacCfms--;
         RETVALUE(RFAILED);
      }
   }
   
   RETVALUE(ROK);
}  /* end of nhPIMSendMacResetCfg */

/**
 *
 *   @brief Function to fill RLC re-establish Map Table for PDCP SDU status.
 *
 *   @details
 *   This function is used to fill RLC Map Table.
 *            
 *  @param[in] transCtrlBlk       RLC config parameter.
 *  @param[in] rbCfgInfoList RB config info list.
 *
 *  @return  S16
 *   -# ROK 
 **/      
#ifdef ANSI
PRIVATE S16 nhPIMUpdPdcpSduStaRlcMapTbl
(
TransCb        *transCtrlBlk,
CmLListCp      *rbCfgInfoList
)
#else /* ANSI */
PRIVATE S16 nhPIMUpdPdcpSduStaRlcMapTbl(transCtrlBlk, rbCfgInfoList)
TransCb        *transCtrlBlk;
CmLListCp      *rbCfgInfoList;
#endif /* ANSI */
{
   CmLList      *tmpNode =  NULLP;
   Bool         rbType;

   TRC2(nhPIMUpdPdcpSduStaRlcMapTbl)

   CM_LLIST_FIRST_NODE(rbCfgInfoList, tmpNode);
   while(tmpNode)
   { 
      rbType = ((NhuRbCfgInfo *)tmpNode->node)->rbId.rbTyp; 
      if(CM_LTE_DRB == rbType)
      {
         transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_REST] |= 
                     (1 <<(((NhuRbCfgInfo *)tmpNode->node)->rbId.rbId-1));
      }

      CM_LLIST_NEXT_NODE(rbCfgInfoList, tmpNode);
   }
   transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_REST] = 
                            transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_REST];

   RETVALUE(ROK);
}/* End of nhPIMUpdPdcpSduStaRlcMapTbl */

/**
 * @brief This function checks the status and sends the PDCP SDU request
 *  to PDCP.
 *
 * @details
 *
 * This function checks the status. If status is success, PDCP SDU status
 * is sent to PDCP and timer is started. Otherwise transCtrlBlk is deleted and 
 * no timer is started.
 *
 * @param[in] nhIdentity      RRC ID(NhId) containing cell ID and UE ID
 * @param[in] transCtrlBlk   Transaction context
 * @param[in] cellAndUeCb  Cell and UE Control block
 * @param[in] status    Configuration confirm status from MAC and RLC
 *
 * @return S16
 *  -# ROK
*/
#ifdef ANSI
PUBLIC S16 nhPIMSndSduStaReq
(
NhId           nhIdentity,
TransCb        *transCtrlBlk,
NhCellAndUeCb  *cellAndUeCb,
U8             status
)
#else /* ANSI */
PUBLIC S16 nhPIMSndSduStaReq(nhIdentity, transCtrlBlk, cellAndUeCb, status)
NhId           nhIdentity;
TransCb        *transCtrlBlk;
NhCellAndUeCb  *cellAndUeCb;
U8             status;
#endif /* ANSI */
{
   S16   retVal;
   CpjSduStaReqInfo *cpjSduStaReq = NULLP;
   NhLSapCb         *lSap = NULLP;
   SuId              suId = NH_ZERO_VAL;
   Mem               sMem;
   NhuPdcpSduStaCfm  *nhuSduStaCfm = NULLP;

   TRC2(nhPIMSndSduStaReq)

   retVal = ROK;

   RLOG0(L_DEBUG, "nhPIMSndSduStaReq  called  ");

   if(status)
   {
      /* Get lower SAP*/
#ifdef NHU_MULT_SAP_SUPPRT
      NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, cellAndUeCb->cellCb, STNHCPJSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       
      lSap = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);
      if (NULLP == lSap)
      {
         RLOG0(L_ERROR, "Failed to find PDCP lower layer");
         RETVALUE(RFAILED);
      }

      NH_ALLOC(NH_REG, NH_POOL, &cpjSduStaReq, sizeof(CpjSduStaReqInfo));

      /*R9 upgrade: kworks Error: Null check is added */
      if (NULLP == cpjSduStaReq)
      {
         RLOG0(L_FATAL,"cpjSduStaReq memory allocation failed.");
         RETVALUE(RFAILED);
      }
      transCtrlBlk->cfgMask     |= NH_PDCP_CONFIGURED;
      cpjSduStaReq->ueId    = nhIdentity.ueId; 
      cpjSduStaReq->cellId  = nhIdentity.cellId; 
      cpjSduStaReq->transId = transCtrlBlk->nxTransId;

      /* nh002.301:118385: HO issue: NO need to start this timer since it has already been started
       * while snding the MAC cfg*/

      transCtrlBlk->numOfPdcpCfms++;
      retVal = NhLiCpjSduStaReq(&lSap->pst, lSap->spId, cpjSduStaReq); 
      if (ROK != retVal)
      {
         RLOG0(L_ERROR, "NhLiCpjSduStaReq failed");
         transCtrlBlk->numOfPdcpCfms--;
         RETVALUE(RFAILED);
      }
   }
   else
   {
      /* Allocate memory for PDCP Status confirm structure */
      sMem.region = nhCb.init.region; 
      sMem.pool = nhCb.init.pool;

      NH_ALLOCEVNT((Ptr *)&nhuSduStaCfm, sizeof(NhuPdcpSduStaCfm), &sMem);
      if(NULLP == nhuSduStaCfm)
      {
         RLOG0(L_FATAL,"NhuPdcpSduStaCfm memory allocation failed.");
         RETVALUE(RFAILED);
      }

      /* Fill the NHU header information */ 
      nhuSduStaCfm->hdr.transId = transCtrlBlk->nxTransId;
      nhuSduStaCfm->hdr.cellId  = nhIdentity.cellId;
      nhuSduStaCfm->hdr.ueId    = nhIdentity.ueId;

      /* Call function to send PDCP SDU Status confirm to RRC user */
      nhUIMSndPdcpSduStaCfm(nhuSduStaCfm, transCtrlBlk->cfgMask,
                         transCtrlBlk->cfmStatus, cellAndUeCb->ueCb);

      /* If unsuccessful confirm from MAC/RLC, delete transCtrlBlk as no PDCP SDU *
       * status request is sent to PDCP                                      */
      /*R9 upgrade: Kworks Error: ret value check is deleted since this
       * function always returns ROK. in future if somebody changes the
       * ret value then he has to add the check
       */
      (Void)nhDBMDelTransCb(cellAndUeCb->ueCb, &transCtrlBlk);
   }

   RETVALUE(ROK);
} /* end of nhPIMSndSduStaReq */
#ifdef RRC_PHY_CFG_SUPP

/**
 *
 *   @brief Handler to send Cell Configuration to PHY.
 *
 *   @details
 *   This function is used to send cell configuration to PHY layer.
 *            
 *  @param[in] cellCb  Cell control block.
 *
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendPhyCellCfg
(
NhCellCb       *cellCb        /* Cell control block */
)
#else /* ANSI */
PUBLIC S16 nhPIMSendPhyCellCfg(cellCb)
NhCellCb       *cellCb;        /* Cell control block */
#endif /* ANSI */
{
   CtfCfgReqInfo     *ctfCfgParam = NULLP;  /* PHY Configuration Request parameter */
   CtfCfgTransId     ctfCfgTransId;         /* PHY Transaction ID */
   Pst               tmpPst;                /* Post structure */ 
   NhLSapCb          *lSap = NULLP;         /* Lower SAP Cb */
   NhCellOrdrdCfg    *cellCfgInfo = NULLP;  /* RRC cell configuration info */
   CtfCellCfgInfo    *ctfCellCfg = NULLP;   /* PHY cell cfg */ 

   SuId              suId = NH_ZERO_VAL;    /* It is hard coded for only one SAP
                                               configuration */    
   U16               ueId = NH_ZERO_VAL;    /* UE ID */
   
   TRC2(nhPIMSendPhyCellCfg)

   RLOG0(L_DEBUG, "nhPIMSendPhyCellCfg");

   /* Get lower SAP*/
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, cellCb, STNHCTFSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;
   
   /* Initialize transid structure to zero */
   NH_SET_ZERO(&ctfCfgTransId, sizeof(CtfCfgTransId));

   /* Get ordered cell config */
   cellCfgInfo = cellCb->cellTransCb.cellOrdrdCfg;
   
   /* Allocate memory for cfg parameter */
   NH_ALLOC(NH_REG, NH_POOL, &ctfCfgParam, sizeof(CtfCfgReqInfo));
   if (NH_CHK_INPUT_PARAM(ctfCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL ctfCfgParam");
      RETVALUE(RFAILED);
   }
  
   switch(cellCb->cellTransCb.cfgAction)
   {
      case NH_CFG_ACTN_ADD: 
         if (cellCfgInfo->cellCfgInfo.cellCfgBitMsk & 
            (NHU_CMN_PHY_CFG_PRACH | NHU_CMN_PHY_CFG_PHICH 
                  | NHU_CMN_PHY_CFG_PUSCH | NHU_CMN_PHY_CFG_PDSCH 
                  | NHU_CMN_PHY_CFG_UL_SRS | NHU_CMN_PHY_CFG_PUCCH 
                  | NHU_CMN_PHY_CFG_TDD_SF))
         {
            /* Add configuration to PHY */
            ctfCfgParam->cfgType = CTF_CONFIG;

            /* Cell configuration */
            ctfCfgParam->u.cfg.cfgElem = CTF_CELL_CFG;
            ctfCellCfg = &ctfCfgParam->u.cfg.u.cellCfg;

            ctfCellCfg->txCfg.duplexMode = 
                    (CtfDuplexMode) cellCfgInfo->cellCfgInfo.duplexMode; 
            /* Added Cell ID assignments to ctfCellConfig */
            ctfCellCfg->cellId = cellCb->cellId;

            /* nh002.201 - change of input parameters */
            nhPIMFillPhyCellCfgParam(ctfCellCfg, &(cellCfgInfo->cellCfgInfo));
         }

         break;
     
      case NH_CFG_ACTN_MODIFY:

         /* Re-configuration to PHY */
         ctfCfgParam->cfgType = CTF_RECONFIG;

         /* Cell configuration */
         ctfCfgParam->u.reCfg.cfgElem = CTF_CELL_CFG;
         ctfCfgParam->u.reCfg.u.cellRecfg.cellId = cellCb->cellId;
         ctfCfgParam->u.reCfg.u.cellRecfg.cellId = cellCb->cellId;


         ctfCfgParam->u.reCfg.u.cellRecfg.actvnTime.sfn 
                              = cellCfgInfo->reCfgActvnTime.sfn;
         ctfCfgParam->u.reCfg.u.cellRecfg.actvnTime.subframe 
                              = cellCfgInfo->reCfgActvnTime.subframe;

         /* Fill re-configuration parameter */
         nhPIMFillPhyCellReCfgParam(&ctfCfgParam->u.reCfg.u.cellRecfg,
                                     &(cellCfgInfo->cellCfgInfo.phyCellCfg));

         break;

      case NH_CFG_ACTN_RLS:
         /* Delete configuration to PHY */
         ctfCfgParam->cfgType = CTF_DELETE;

         /* Cell delete */
         ctfCfgParam->u.release.cfgElem = CTF_CELL_CFG;
         ctfCfgParam->u.release.u.cellRel.cellId = cellCb->cellId;
         break;
      
      default:
         RLOG0(L_ERROR,"Invalid Cfg Action ");
         RETVALUE(RFAILED);
   }

   /* Construct transaction ID */
   NH_GET_PHY_CFG_REQ_TRANSID(ctfCfgTransId.trans, cellCb->cellId, ueId,
                      cellCb->cellTransCb.nxTransId, NH_CTF_CELL_CFG); 
                      
         
   cellCb->cellTransCb.numOfPhyCfms++;
         
   /* Send Configuration to PHY layer */
   if (ROK != NhLiCtfCfgReq(&tmpPst, lSap->spId, ctfCfgTransId, ctfCfgParam))
   {
      cellCb->cellTransCb.numOfPhyCfms--;

      RLOG0(L_ERROR,"Failed to send data to PHY layer");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* nhPIMSendPhyCellCfg */


/**
 *
 *   @brief Handler to revert successful configuration at PHY.
 *
 *   @details
 *   This function is used to revert successful
 *   cell configuration at PHY layer.
 *            
 *
 *   @param[in] cellCb  Cell control block
 *
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRvrtPhyCellCfg
(
NhCellCb *cellCb                         /* Cell control block */
)
#else /* ANSI */
PUBLIC S16 nhPIMRvrtPhyCellCfg(cellCb)
NhCellCb *cellCb;                        /* Cell control block */
#endif /* ANSI */
{
   CtfCfgReqInfo    *ctfCfgParam = NULLP;   /* PHY Config Request Parameter */
   CtfCfgTransId    ctfCfgTransId;          /* PHY Transaction Id */
   NhCellTransCb    *cellTransCb = NULLP;   /* Cell TransCb */
   NhCellCfgInfo    *cellCfgInfo = NULLP;   /* Cell config Info */ 
   U8               rvrtAction = 0x00;      /* Revert cction */ 
   Pst              tmpPst;                 /* Post structure */ 
   NhLSapCb         *lSap = NULLP;          /* Lower SAP Cb */
   SuId             suId = NH_ZERO_VAL;     /* service user id */
   U16              ueId = NH_ZERO_VAL;     /* UE ID  */
   U32              rvrtTransId = NH_RESERVED_TRANS_ID;  
      
   TRC2(nhPIMRvrtPhyCellCfg)

   RLOG0(L_DEBUG, "nhPIMRvrtPhyCellCfg");

   /* Get lower Sap */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, cellCb, STNHCTFSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;
   
   /* Reterive cell trans Cb */
   cellTransCb = &(cellCb->cellTransCb);

   if (cellTransCb->cfgAction == NH_CFG_ACTN_ADD)
   {
       rvrtAction = NH_CFG_ACTN_RLS;
   }
   else if (cellTransCb->cfgAction == NH_CFG_ACTN_MODIFY)
   {
       rvrtAction = NH_CFG_ACTN_MODIFY;
   }
  
   /* Check for Cell Cfg */

   if ((cellTransCb->phyMapInfo.status ==  NH_STATUS_USED) &&
       (cellTransCb->phyMapInfo.cfmStatus != NH_CFG_FAILED))
   {
      /* Allocate memory for ctf configuration */
      NH_ALLOC(NH_REG, NH_POOL, &ctfCfgParam, sizeof(CtfCfgReqInfo));
      if (NH_CHK_INPUT_PARAM(ctfCfgParam))
      {
         RLOG0(L_FATAL, "GOT NULL ctfCfgParam");
         RETVALUE(RFAILED);
      }
      /* Initialize transaction id */
      NH_SET_ZERO(&ctfCfgTransId, sizeof(CtfCfgTransId));
      
      switch(rvrtAction)
      {
         case NH_CFG_ACTN_RLS:
            /* If revert action is release, then original action was add */
            /* send delete configuration */

            ctfCfgParam->cfgType = CTF_DELETE;
            /* Cell delete */
            ctfCfgParam->u.release.cfgElem = CTF_CELL_CFG;
            ctfCfgParam->u.release.u.cellRel.cellId = 
                               cellCb->cellId;

            NH_GET_PHY_CFG_REQ_TRANSID(ctfCfgTransId.trans,
                     cellCb->cellId, ueId, rvrtTransId, NH_CTF_CELL_CFG)
            
            /* Send Configuration to PHY layer */
            if (ROK != NhLiCtfCfgReq(&tmpPst, lSap->spId, 
                                      ctfCfgTransId, ctfCfgParam))
            {
                RLOG0(L_ERROR, "Failed to configuration to PHY layer");
                RETVALUE(RFAILED);
            }
            RETVALUE(ROK);
         
         case NH_CFG_ACTN_MODIFY:
            {
               /* If revert action is add, then original action was Rls */
               /* Get Cfg info from crntcfg and add it now */
                cellCfgInfo = &(cellCb->crntCellCfgInfo);

                ctfCfgParam->cfgType = CTF_RECONFIG;

                ctfCfgParam->u.reCfg.cfgElem = CTF_CELL_CFG;
                ctfCfgParam->u.reCfg.u.cellRecfg.cellId = cellCb->cellId;

                nhPIMFillPhyCellReCfgParam(&ctfCfgParam->u.reCfg.u.cellRecfg,
                                     &(cellCfgInfo->phyCfg));
                
                NH_GET_PHY_CFG_REQ_TRANSID(ctfCfgTransId.trans,
                      cellCb->cellId, ueId, rvrtTransId, NH_CRG_CELL_CFG)
                
                /* Send Configuration to PHY layer */
                if (ROK != NhLiCtfCfgReq(&tmpPst, lSap->spId, 
                                          ctfCfgTransId, ctfCfgParam))
                {
                   RLOG0(L_ERROR, "Failed to configuration to PHY layer");
                   RETVALUE(RFAILED);
                }

            }
            break;
         default:
            RLOG0(L_ERROR, "Invalid case of revert config for PHY layer");
            NH_FREE(NH_REG, NH_POOL, ctfCfgParam, sizeof(CtfCfgReqInfo));
            RETVALUE(RFAILED);

      }/* Switch case */
   }

                                                  
   RETVALUE(ROK);

} /* nhPIMRvrtPhyCellCfg */

/**
 *
 *   @brief Handler to Revert Add configuration to PHY.
 *
 *   @details
 *   This function is used revert the Add configuration   
 *   which was sent to PHY layer.
 *            
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Transaction control block
 *   @param[in] cfgType   Configuration type
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRvrtPhyAddCfg
(
NhActvUeCb *ueCb,
TransCb *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhPIMRvrtPhyAddCfg(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
#endif /* ANSI */
{
   CtfCfgReqInfo     *ctfCfgParam = NULLP;   /* PHY Config Request Parameter */
   CtfCfgTransId     ctfCfgTransId;    /* PHY Transaction Id */
   Pst               tmpPst;           /* Post structure */ 
   NhLSapCb          *lSap = NULLP;    /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL; 
   U32               rvrtTransId = NH_RESERVED_TRANS_ID; 
   
   TRC2(nhPIMRvrtPhyAddCfg)
   
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCTFSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;

   /* Allocate memory for cfg parameter */
   NH_ALLOC(NH_REG, NH_POOL, &ctfCfgParam, sizeof(CtfCfgReqInfo));
   if (NH_CHK_INPUT_PARAM(ctfCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL ctfCfgParam");
      RETVALUE(RFAILED);
   }

   NH_SET_ZERO(&ctfCfgTransId, sizeof(CtfCfgTransId));

   ctfCfgParam->cfgType = CTF_DELETE;
   ctfCfgParam->u.release.cfgElem = CTF_UE_CFG;
   ctfCfgParam->u.release.u.dedRel.cellId = ueCb->cellCb->cellId;
   ctfCfgParam->u.release.u.dedRel.ueId = ueCb->ueCbKey.crnti;

   NH_GET_PHY_CFG_REQ_TRANSID(ctfCfgTransId.trans, ueCb->cellCb->cellId,
               ueCb->ueCbKey.crnti, rvrtTransId, NH_CTF_UE_CFG);

   /* Send Configuration to PHY layer */
   if (ROK != NhLiCtfCfgReq(&tmpPst, lSap->spId, ctfCfgTransId, ctfCfgParam))
   {
     RLOG0(L_ERROR,"Failed to send data to lower layer");
     RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);
}  /* end of nhPIMRvrtPhyAddCfg */

/**
 *
 *   @brief Handler to Revert Modify configuration to PHY.
 *
 *   @details
 *   This function is used revert the Modify configuration   
 *    which was sent to PHY layer.
 *            
 *
 *  @param[in] ueCb     UE control block
 *  @param[in] transCtrlBlk  Transaction Cb
 *  @param[in] cfgType  Configuration type
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMRvrtPhyModCfg
(
NhActvUeCb *ueCb,
TransCb *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhPIMRvrtPhyModCfg(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;
TransCb *transCtrlBlk;
#endif /* ANSI */
{
   CtfCfgReqInfo       *ctfCfgParam = NULLP;  /* PHY Configuration Request parameter */
   CtfCfgTransId       ctfCfgTransId;         /* PHY Transaction ID */
   Pst                 tmpPst;                /* Post structure */ 
   NhLSapCb            *lSap = NULLP;         /* Lower SAP Cb */
   SuId                suId = NH_ZERO_VAL; 
   U16                 ueId = NH_ZERO_VAL;
   U32                 rvrtTransId = NH_RESERVED_TRANS_ID;  
   
   TRC2(nhPIMRvrtPhyModCfg)
   RLOG0(L_DEBUG, "nhPIMRvrtPhyModCfg");
  
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCTFSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;
   
   ueId = ueCb->ueCbKey.crnti;

   /* Re-configuration to PHY */
   NH_ALLOC(NH_REG, NH_POOL, &ctfCfgParam, sizeof(CtfCfgReqInfo));
   if (NH_CHK_INPUT_PARAM(ctfCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL ctfCfgParam");
      RETVALUE(RFAILED);
   }
   NH_SET_ZERO(&ctfCfgTransId, sizeof(CtfCfgTransId));
      
   nhPIMFillRvrtPhyModUeCfg(ctfCfgParam, ueCb, transCtrlBlk);
   
   NH_GET_PHY_CFG_REQ_TRANSID(ctfCfgTransId.trans, ueCb->cellCb->cellId,
                    ueId, rvrtTransId, NH_CTF_UE_CFG);

   /* Send Configuration to PHY layer */
   if (ROK != NhLiCtfCfgReq(&tmpPst, lSap->spId, ctfCfgTransId, ctfCfgParam))
   {
      RLOG0(L_ERROR,"Failed to send data to lower layer");
      RETVALUE(RFAILED);
   }
          
   RETVALUE(ROK);
}  /* end of nhPIMRvrtPhyModCfg */

/**
 *
 *   @brief Handler to add new configuration to PHY.
 *
 *   @details
 *   This function is used to add new configuration   
 *   to PHY.
 *            
 *  @param[in] ueCb      UE control block 
 *  @param[in] transCtrlBlk   Transaction control block
 *  @return  S16
 *   -# ROK 
 *   -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendPhyAddCfg
(
NhActvUeCb *ueCb,         /* UE control block */
TransCb    *transCtrlBlk      /* Transaction control block */
)
#else /* ANSI */
PUBLIC S16 nhPIMSendPhyAddCfg(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;         /* UE control block */
TransCb    *transCtrlBlk;      /* Transaction control block */
#endif  /* ANSI */
{
   CtfCfgReqInfo     *ctfCfgParam = NULLP;    /* PHY Configuration Request parameter */
   CtfCfgTransId     ctfCfgTransId;           /* PHY Transaction ID */
   Pst               tmpPst;                  /* Post structure */ 
   NhLSapCb          *lSap = NULLP;           /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL;      /* Service user ID */ 
   NhOrdrdCfgInfo    *ordrdCfg = NULLP;       /* Ordered configuration */
   
   TRC2(nhPIMSendPhyAddCfg)
   RLOG0(L_DEBUG, "nhPIMSendPhyAddCfg");
   
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCTFSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;

   ordrdCfg = &transCtrlBlk->ordrdCfg;
   
   /* Allocate memory for cfg Parameter */
   NH_ALLOC(NH_REG, NH_POOL, &ctfCfgParam, sizeof(CtfCfgReqInfo));
   if (NH_CHK_INPUT_PARAM(ctfCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL ctfCfgParam");
      RETVALUE(RFAILED);
   }

   NH_SET_ZERO(&ctfCfgTransId, sizeof(CtfCfgTransId));

   /* configuration to PHY */
   ctfCfgParam->cfgType = CTF_CONFIG;
   ctfCfgParam->u.cfg.cfgElem = CTF_UE_CFG;
   ctfCfgParam->u.cfg.u.dedCfg.cellId = ueCb->cellCb->cellId;
   ctfCfgParam->u.cfg.u.dedCfg.ueId = ueCb->ueCbKey.crnti;
     
   /* call the function to fill the CTF config params */
   nhPIMFillPhyCfgParam(&(ctfCfgParam->u.cfg.u.dedCfg), &(ordrdCfg->phyCfg));

   NH_GET_PHY_CFG_REQ_TRANSID(ctfCfgTransId.trans, ueCb->cellCb->cellId,
                      ueCb->ueCbKey.crnti, transCtrlBlk->nxTransId, 
                      NH_CTF_UE_CFG);

   transCtrlBlk->numOfPhyCfms++;
   /* Send Configuration to PHY layer */
   if (ROK != NhLiCtfCfgReq(&tmpPst, lSap->spId, ctfCfgTransId, ctfCfgParam))
   {
      transCtrlBlk->numOfPhyCfms--;
      RLOG0(L_ERROR,"Failed to send data to PHY layer");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* end of nhPIMSendPhyAddCfg */

/**
 *
 *   @brief Handler to modify configuration at PHY.
 *
 *   @details
 *   This function is used to modify configuration   
 *   at PHY.
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Transaction control block
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendPhyModCfg
(
NhActvUeCb *ueCb,        /* UE control block */
TransCb *transCtrlBlk    /* Transaction control block */                   
)
#else /* ANSI */
PUBLIC S16 nhPIMSendPhyModCfg(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;        /* Ue control block */
TransCb *transCtrlBlk;        /* Trans Cb */
#endif /* ANSI */
{
   CtfCfgReqInfo     *ctfCfgParam = NULLP;   /* PHY Configuration Request parameter */
   CtfCfgTransId     ctfCfgTransId;          /* PHY Transaction ID */
   Pst               tmpPst;                 /* Post structure */ 
   NhLSapCb          *lSap = NULLP;          /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL;     /* suId */
   U16               ueId = NH_ZERO_VAL;     /* UeId */

   TRC2(nhPIMSendPhyModCfg)
   RLOG0(L_DEBUG, "nhPIMSendPhyModCfg");
  
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCTFSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
       
   }

   tmpPst = lSap->pst;

   /* Reterive UE ID */ 
   if(transCtrlBlk->ordrdCfg.ueInfo.crntiCfgType == NH_UE_INFO_MOD_CRNTI)
   {
      ueId = transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;
   }
   else
   {
      ueId = ueCb->ueCbKey.crnti;
   }

   /* Allocate memory for cfg Parameter */
   NH_ALLOC(NH_REG, NH_POOL, &ctfCfgParam, sizeof(CtfCfgReqInfo));
   if (NH_CHK_INPUT_PARAM(ctfCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL ctfCfgParam");
      RETVALUE(RFAILED);
   }
   /* Set zero */
   NH_SET_ZERO(&ctfCfgTransId, sizeof(CtfCfgTransId));
   /* Re-configuration */

   ctfCfgParam->cfgType = CTF_RECONFIG;
   ctfCfgParam->u.reCfg.cfgElem = CTF_UE_CFG;
   ctfCfgParam->u.reCfg.u.dedRecfg.cellId = ueCb->cellCb->cellId;
     
   if ((NH_STATUS_USED == 
                  transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].status) && 
       (NH_PHY_CFG_UEID_CHNG == 
                  transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].action))
   {
      ctfCfgParam->u.reCfg.u.dedRecfg.ueId = ueCb->newUeCbKey.crnti;
   }
   else
      ctfCfgParam->u.reCfg.u.dedRecfg.ueId = ueCb->ueCbKey.crnti;
     
   /* call the function to fill the CTF config params */
   nhPIMFillPhyReCfgParam(&(ctfCfgParam->u.reCfg.u.dedRecfg), 
                          &(transCtrlBlk->ordrdCfg.phyCfg));

   /* Construct transaction ID */
   NH_GET_PHY_CFG_REQ_TRANSID(ctfCfgTransId.trans, ueCb->cellCb->cellId,
         ueCb->ueCbKey.crnti, transCtrlBlk->nxTransId, NH_CTF_UE_CFG);

   transCtrlBlk->numOfPhyCfms++;
   /* Send Configuration to PHY layer */
   if (ROK != NhLiCtfCfgReq(&tmpPst, lSap->spId, ctfCfgTransId, ctfCfgParam))
   {
         transCtrlBlk->numOfPhyCfms--;
         RLOG0(L_ERROR,"Failed to send data to PHY layer");
         RETVALUE(RFAILED);
   }


   RETVALUE(ROK);
}  /* end of nhPIMSendPhyModCfg */

/**
 *
 *   @brief Handler to release specific configuration at PHY.
 *
 *   @details
 *   This function is used to release specific   
 *   configuration at PHY.
 *            
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Transaction control block
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendPhyRlsCfg
(
NhActvUeCb *ueCb       /* UE Cb */
)
#else /* ANSI */
PUBLIC S16 nhPIMSendPhyRlsCfg(ueCb)
NhActvUeCb *ueCb;       /* UE Cb */
#endif /* ANSI */
{
   CtfCfgReqInfo     *ctfCfgParam = NULLP;   /* PHY Configuration Request parameter */
   CtfCfgTransId     ctfCfgTransId;          /* PHY Transaction ID */
   Pst               tmpPst;                 /* Post structure */ 
   NhLSapCb          *lSap = NULLP;          /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL;     /* suId */
   U16               ueId = NH_ZERO_VAL;     /* UeId */
   U32               transId = NH_RESERVED_TRANS_ID;

   TRC2(nhPIMSendPhyRlsCfg)

   UNUSED(ueId);

   RLOG0(L_DEBUG, "nhPIMSendPhyRlsCfg");
  
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCTFSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find PHY layer or SAP not bound");
      RETVALUE(RFAILED);
   }

   tmpPst = lSap->pst;

   /* Allocate memory for cfg Parameter */
   NH_ALLOC(NH_REG, NH_POOL, &ctfCfgParam, sizeof(CtfCfgReqInfo));
   if (NH_CHK_INPUT_PARAM(ctfCfgParam))
   {
      RLOG0(L_FATAL, "GOT NULL ctfCfgParam");
      RETVALUE(RFAILED);
   }
   /* Set zero */
   NH_SET_ZERO(&ctfCfgTransId, sizeof(CtfCfgTransId));
   /* Re-configuration */

   ctfCfgParam->cfgType = CTF_DELETE;
   ctfCfgParam->u.release.cfgElem = CTF_UE_CFG;
   ctfCfgParam->u.release.u.dedRel.cellId = ueCb->cellCb->cellId;
   ctfCfgParam->u.release.u.dedRel.ueId = ueCb->ueCbKey.crnti;
     

   NH_GET_PHY_CFG_REQ_TRANSID(ctfCfgTransId.trans, ueCb->cellCb->cellId,
         ueCb->ueCbKey.crnti, transId, NH_CTF_UE_CFG);

   /* Send Configuration to PHY layer */
   if (ROK != NhLiCtfCfgReq(&tmpPst, lSap->spId, ctfCfgTransId, ctfCfgParam))
   {
      RLOG0(L_ERROR,"Failed to send data to PHY layer");
      RETVALUE(RFAILED);
   }


   RETVALUE(ROK);
}  /* end of nhPIMSendPhyRlsCfg */


/**
 *
 *   @brief Handler to Issue UeId Chage Request to PHY.
 *
 *   @details
 *   This function is used to issue a UeId Change
 *   request to PHY.
 *            
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Transaction control block
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhPIMSendPhyUeIdChgReq
(
NhActvUeCb *ueCb,       /* UE Cb */
TransCb *transCtrlBlk       /* Transaction Cb */
)
#else /* ANSI */
PUBLIC S16 nhPIMSendPhyUeIdChgReq(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;       /* UE Cb */
TransCb *transCtrlBlk;       /* Transaction Cb */ 
#endif /* ANSI */
{
   CtfCfgTransId     ctfCfgTransId;          /* PHY Transaction ID */
   Pst               tmpPst;                 /* Post structure */ 
   NhLSapCb          *lSap = NULLP;          /* Lower SAP Cb */
   SuId              suId = NH_ZERO_VAL;     /* suId */
   CtfUeInfo         *oldUeInfo = NULLP;
   CtfUeInfo         *newUeInfo = NULLP;

   TRC2(nhPIMSendPhyUeIdChgReq)

   RLOG0(L_DEBUG, "nhPIMSendPhyUeIdChgReq");
  
   /* Get lower SAP */
#ifdef NHU_MULT_SAP_SUPPRT
   NH_GET_SUID_FOR_MULT_SAP_SUPPRT(suId, ueCb->cellCb, STNHCTFSAP)
#endif /* NHU_MULT_SAP_SUPPRT */       

   lSap = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP,suId);
   if ((NULLP == lSap) || (lSap->state != LNH_SAP_BOUND))
   {
      RLOG0(L_ERROR, "Failed to find lower layer or SAP not bound");
      RETVALUE(RFAILED);
       
   }

   tmpPst = lSap->pst;

   /* Set zero */
   NH_SET_ZERO(&ctfCfgTransId, sizeof(CtfCfgTransId));

   NH_ALLOC(NH_REG, NH_POOL, &oldUeInfo, sizeof(CtfUeInfo));
   if(NULLP == oldUeInfo)
   {
      RLOG0(L_FATAL, "Could not allocate mem for Ue Info");
      RETVALUE(RFAILED);
   }

   NH_ALLOC(NH_REG, NH_POOL, &newUeInfo, sizeof(CtfUeInfo));
   if(NULLP == newUeInfo)
   {
      RLOG0(L_FATAL, "Could not allocate mem for New Ue Info");
      NH_FREE(NH_REG, NH_POOL, oldUeInfo, sizeof(CtfUeInfo));
      RETVALUE(RFAILED);
   }

   oldUeInfo->ueId = ueCb->ueCbKey.crnti;
   oldUeInfo->cellId = ueCb->cellCb->cellId;

   newUeInfo->ueId = transCtrlBlk->ordrdCfg.ueInfo.u.modCrnti;
   newUeInfo->cellId = ueCb->cellCb->cellId;

   NH_GET_PHY_CFG_REQ_TRANSID(ctfCfgTransId.trans, ueCb->cellCb->cellId,
         ueCb->ueCbKey.crnti, transCtrlBlk->nxTransId, NH_CTF_UE_CFG);

   transCtrlBlk->numOfPhyCfms++;
   if (ROK != NhLiCtfUeIdChgReq(&tmpPst, lSap->spId, ctfCfgTransId,
                                                     oldUeInfo, newUeInfo))
   {
      transCtrlBlk->numOfPhyCfms--;
      RLOG0(L_ERROR,"Failed to send UEID Change Req to PHY");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* end of nhPIMSendPhyUeIdChgReq */

#endif /* RRC_PHY_CFG_SUPP */

/**
 *
 *   @brief Handler to add configuration to PDCP for specified RB Id.
 *
 *   @details
 *   This function is used to add/mod/delete configuration   
 *   to PDCP for specified RB Id.
 *            
 *
 *   @param[in] ueCb      UE control block
 *   @param[in] transCtrlBlk   Trans control block
 *   @param[out] cpjCfgReq  CPJ  Configuration Request information
 *   @return  S16
 *    -# ROK 
 *    -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 nhPIMFillPdcpCfgParam
(
NhActvUeCb    *ueCb,
TransCb       *transCtrlBlk,
CpjCfgReqInfo *cpjCfgReq
)
#else /* ANSI */
PRIVATE S16 nhPIMFillPdcpCfgParam(ueCb, transCtrlBlk, cpjCfgReq)
NhActvUeCb    *ueCb;
TransCb       *transCtrlBlk;
CpjCfgReqInfo *cpjCfgReq;
#endif /* ANSI */
{
   NhuRbCfgList  *rbCfgListInfo;
   U16           idx = NH_ZERO_VAL;
   U8            rbCount = NH_ZERO_VAL;
   Size          ulBitMapSize = NH_ZERO_VAL;

   /* nh004.201 - Correction to TRC functionality. */
   TRC2(nhPIMFillPdcpCfgParam)

   /* action add */
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_ADD)
   {
      rbCfgListInfo = &(transCtrlBlk->ordrdCfg.rbToAddCfgList);
      /* Fill PDCP Configuration parameter to CPJ Cfg structure */
      /*R9 upgrade: Kworks Error: ret value check is deleted since this
       * function always returns ROK. in future if somebody changes the
       * ret value then he has to add the check
       */
     (Void)nhPIMFillAddPdcpCfgParam(ueCb, cpjCfgReq, rbCfgListInfo );

   }
   /* Action modify */
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_MOD)
   {
      rbCfgListInfo = &(transCtrlBlk->ordrdCfg.rbToModCfgList);
      /* Fill PDCP Config parameter to CPJ Cfg structure */
      if (ROK != nhPIMFillModPdcpCfgParam(ueCb, cpjCfgReq, rbCfgListInfo ))
      {
         RLOG0(L_ERROR, "Failed to fill data to lower interface structure");
         /* If anything fails,free the memory of ulBitMap in cpjCfgReq if *
          * allocated */
         for (idx = 0; ((idx < NH_MAX_CFG_REQ) && (idx < CPJ_MAX_CFG)); idx++)
         {
            if(cpjCfgReq->cfgEnt[idx].m.modCfg.hoInfo.isHoPres & CPJ_HO_UL) 
            {
               NH_GET_UL_BITMAP_SIZE(
                            cpjCfgReq->cfgEnt[idx].m.modCfg.hoInfo.numBits,
                            ulBitMapSize);

               NH_FREE(NH_REG, NH_POOL,
                       cpjCfgReq->cfgEnt[idx].m.modCfg.hoInfo.ulBitMap, 
                       ulBitMapSize * sizeof(U8));
            }
         }
         NH_FREE(NH_REG, NH_POOL, cpjCfgReq, sizeof(CpjCfgReqInfo));
         RETVALUE(RFAILED);
      }

   }
   /* Action release */
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS)
   {
      rbCount = cpjCfgReq->numEnt;
      if(ROK != nhPIMFillPdcpRlsCfg(ueCb, transCtrlBlk, cpjCfgReq, &rbCount))
      {
          NH_FREE(NH_REG, NH_POOL, cpjCfgReq, sizeof(CpjCfgReqInfo));
          RETVALUE(RFAILED);
      }
      cpjCfgReq->numEnt = rbCount; 
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS) */

   /* Action re-establish */
   if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RESTB) 
   {
      rbCount = cpjCfgReq->numEnt;
      /*R9 upgrade: Kworks Error: ret value check is deleted since this
       * function always returns ROK. in future if somebody changes the
       * ret value then he has to add the check
       */
      (Void)nhPIMFillPdcpRestbCfg(transCtrlBlk, cpjCfgReq, &rbCount);
      cpjCfgReq->numEnt = rbCount; 
   } /* if (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RLS) */

   RETVALUE(ROK);
}/* End of nhPIMFillPdcpCfgParam */
/** @} */
/**********************************************************************
  
         End of file:     ni_pim.c@@/main/3 - Fri Jul  1 01:13:20 2011
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      dm               1. LTE-RRC Initial release.
/main/2      ---      ch               1. RRC Release 2.1.
             ---      chebli           1. (nh001.201) Filling CQI and Antenna Config 
                                          parameters in nhPIMFillPhyReCfgParam().
         nh002.201    vkulkarni        1. Handling changes due to relocation of
                                          NhuSysBwCfg in NHU.
         nh004.201    vkulkarni        1. Correction to TRC functionality.
                                       2. Correction to pollPdu and pollByte 
                                          as expected at CKW interface.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
         nh001.301    hsingh           1. Corrected filling of UM mode 
                                          Rerodering Timer
        nh002.301     mpatel           1. Added Macro for mapping RRC PUCCH 
                                          delta shift enum to PHY enum.
        nh002.301                      2. Added Macro for mapping RRC Sc Spacing
                                          value to PHY enum.
                                       3.[ccpu00118385]:Removed call of timer start
                                         func from nhPIMSndSduStaReq   
                                       4.[ccpu00121151]:fixed zero memory alloc issue  
/main/4  nh003.301    njha             1.Fillin L2 measurement param.
*********************************************************************91*/
