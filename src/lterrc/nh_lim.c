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
  
     Type:     C souce file
  
     Desc:     RRC Lower Interface Module Primitives. 

     File:     nh_lim.c

     Sid:      ni_lim.c@@/main/3 - Fri Jul  1 01:13:19 2011

     Prg:      mc

**********************************************************************/

/** @defgroup lim_group Lower Interface Module
 *  @{
 */
/*
 *     The structures and variables declared in this file
 *     correspond to structures and variables used by
 *     the following TRILLIUM software:
 *
 *     part no.             description
 *     --------     ----------------------------------------------
 *     1000369      LTE-RRC
 *
*/

static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=89;

#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
  
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list defines  */
#include "cm_hash.h"       /* Common hashlist defines */
#include "cm_lte.h"        /* Common LTEE defines */
#include "cm_pasn.h"       /* Pasn defines */

#include "lnh.h"           /* LNH Interface defines */
#include "kwu.h"           /* KWU Interface defines */
#include "ckw.h"           /* CKW Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "cpj.h"           /* CPJ Interface defines */
#include "pju.h"           /* PJU Interface defines */
#ifdef RRC_PHY_CFG_SUPP
#include "ctf.h"           /* PHY Interface defines */
#endif /* RRC_PHY_CFG_SUPP */

#include "nhu.h"           /* RRC includes */
#include "nh.h"            /* RRC defines */

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"        /* Common LTE includes */
#include "cm_pasn.x"       /* Common Pasn includes */
#include "cm_lib.x"        /* Common memory allocation library */

#include "kwu.x"           /* KWU Interface includes */
#include "ckw.x"           /* CKW Interface includes */
#include "lnh.x"           /* LNH Interface includes */
#include "crg.x"           /* CRG Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "pju.x"           /* PJU Interface includes */
#ifdef RRC_PHY_CFG_SUPP
#include "ctf.x"           /* PHY interface includes */
#endif /* RRC_PHY_CFG_SUPP */

#include "nhu.x"           /* RRC includes */
#include "nh.x"            /* RRC includes */



PRIVATE S16 nhLIMHndlStatusInd ARGS((NhId   *nhIdentity,   
                                  PjuStaIndInfo *staInd,
                                  Buffer *buf));
PRIVATE S16 nhLIMMacDlMapTblUpd ARGS((TransCb       *transCtrlBlk,
                                    U8               lclId,
                                    U8               status));
PRIVATE S16 nhLIMMacUlMapTblUpd ARGS((TransCb       *transCtrlBlk,
                                    U8               lclId,
                                    U8               status));
PRIVATE S16 nhLIMHndlRlcCfgCfm ARGS((CkwCfgCfmInfo *cfmInfo));
PRIVATE S16 nhLIMHndlRlcUECfgCfm ARGS((CkwCfgCfmInfo *cfmInfo,
                                       NhCellCb *cellCb));
PRIVATE S16 nhLIMHndlRlcCellCfgCfm ARGS((CkwCfgCfmInfo *cfmInfo,
                                       NhCellCb *cellCb));

PRIVATE S16 nhLIMHndlPdcpCfgCfm ARGS((CpjCfgCfmInfo  *pdcpCfgCfm));

PRIVATE S16 nhLIMHndlMacCfgCfm ARGS((CrgCfgTransId transId,
                                    U8 status));
PRIVATE S16 nhLIMHndlMacCellCfgCfm ARGS((NhId *nhIdentity, NhCellCb *cellCb,
                                         U8 cfgElms, U8 status,
                                         U8 lclId));
PRIVATE S16 nhLIMHndlMacUECfgCfm ARGS((NhId *nhIdentity, NhCellCb *cellCb,
                                       NhTransId nxTransId, U8 lclId, 
                                       U8 cfgElms, U8 status, U8 dir));
#ifdef RRC_PHY_CFG_SUPP
PRIVATE S16 nhLIMHndlPhyUECfgCfm ARGS((NhId *nhIdentity, NhCellCb *cellCb,
                                       NhTransId nxTransId, U8 cfgElms, 
                                       U8 status));
#endif /* RRC_PHY_CFG_SUPP */
PRIVATE S16 nhLIMHndlUeIdChgCfm ARGS(( NhId    *nhIdentity ,
                                       U32      transId,
                                       CmStatus status,
                                       U8       lyrCfgd));

PRIVATE S16 nhLIMHndlCountCfm ARGS((CpjCountCfmInfo *countCfmInfo));
PRIVATE S16 nhLIMHndlPdcpSecCfgCfm ARGS((CpjSecCfgCfmInfo *secCfgCfm));
PRIVATE S16 nhLIMHndlRestbCfgCfm ARGS((NhId    *nhIdentity,
                                       U32     transId,
                                       Status  statusCfm));
PRIVATE S16 nhLIMFreeCpjSduStaCfm ARGS((Pst *pst, 
                                        CpjSduStaCfmInfo  *cpjSduStaCfm));
PRIVATE S16 nhLIMFillPdcpSduStaInfo ARGS((CpjSduStaCfmInfo *cpjSduStaCfm,
                                          NhuPdcpSduStaCfm *nhuSduStaCfm));

PRIVATE S16 nhLIMVldtAndRptCfgCfm ARGS((  Void *vCfmInfo,
                                          TransCb  *transCtrlBlk,
                                          NhId *nhIdentity,
                                          NhCellAndUeCb *cellAndUeCb,
                                          U8   layerId));

PRIVATE S16 nhLIMSrchAndSetSRBBitMsk ARGS(( U8        *rcvdRbIds,
                                       U8         *rbCfgTbl,
                                       U8         *rbStaTbl,
                                       U8         rbId,
                                       U16        status,
                                       U8         okValue
                                       ));

PRIVATE S16 nhLIMSrchAndSetDRBBitMsk ARGS(( U32   *rcvdRbIds,
                                       U32        *rbCfgTbl,
                                       U32        *rbStaTbl,
                                       U8         rbId,
                                       U16        status,
                                       U8         okValue
                                       ));

PRIVATE S16 nhLIMHndlUeIdChgCfmRlcCfg ARGS((TransCb    *transCtrlBlk,
                                       NhCellCb        *cellCb,
                                       NhActvUeCb      *ueCb,
                                       U16             cfmStatus,
                                       NhId    *nhIdentity
                                       ));


PRIVATE S16 nhLIMHndlUeIdChgCfmPdcpCfg ARGS((TransCb         *transCtrlBlk,
                                       NhCellCb        *cellCb,
                                       NhActvUeCb      *ueCb,
                                       U16             cfmStatus,
                                       NhId    *nhIdentity
                                       ));

#ifdef RRC_PHY_CFG_SUPP
PRIVATE S16 nhLIMHndlPhyCfgCfm ARGS((CtfCfgTransId transId, U8 status));
PRIVATE S16 nhLIMHndlPhyCellCfgCfm ARGS(( NhId            *nhIdentity,
                                       NhCellCb        *cellCb,
                                       U8              cfgElms,
                                       U8              status
                                       ));

PRIVATE S16 nhLIMHndlUeIdChgCfmPhyCfg ARGS((TransCb         *transCtrlBlk,
                                       NhCellCb        *cellCb,
                                       NhActvUeCb      *ueCb,
                                       U16             cfmStatus,
                                       NhId    *nhIdentity
                                       ));


#endif /* RRC_PHY_CFG_SUPP */


/* Local defines */

/* KWU Interface flags */


/* CKW Interface flags */

/* CRG Interface flags */


/* CPJ Interface flags */

/* PJU Interface flags */

/* Lower Interface State Matrix */

/** @var nhLISMTbl
 *  Lower Layer Interface State Matrix.
*/

PUBLIC PCDFLIS16 nhLISMTbl[NH_EVNT_LI_MAX_EVNTS] =
{
   NULLP,                     /*!< Unimplemented Message Handler */
   nhCCMUlCmnHndlr,           /*!< RRC Connection Request Handler */
   nhCCMUlCmnHndlr,           /*!< RRC Reestablishment Request Handler */
   nhCCMUlDedMsgHndlr,        /*!< UL Informaton Transfer Handler */
   nhCCMConSetupCmpHndlr,     /*!< RRC Connection Setup Complete Handler */
   nhCCMReCfgCmpHndlr,        /*!< RRC Connection Reconfiguration Complete Handler */
   nhCCMSecModCmpHndlr,       /*!< Security Mode Complete Handler */
   nhCCMSecModFailHndlr,      /*!< Security Mode Failure Handler */
   nhCCMConRlsCmpHndlr,       /*!< RRC Connection Release Complete Handler */
   nhCCMUlDedMsgHndlr,        /*!< UL HO preparation transfer - CDMA2000 */
   nhCCMUlDedMsgHndlr,        /*!< CSFB Parameters Request CDMA2000 */ 
   nhCCMUlDedMsgHndlr,        /*!< UE Capability Info*/
   nhCCMUlDedMsgHndlr,        /*!< RRC Counter Check Response handler */
   nhCCMUlDedMsgHndlr,        /*!< Measurement report handler */
   nhCCMUlDedMsgHndlr         /*!< RRC Reestablishment Complete Handler*/
#ifdef LTERRC_REL9
   , nhCCMUlDedMsgHndlr,      /*!< UE Information Response handler */
   nhCCMUlDedMsgHndlr         /*!< Proximity Indication Handler*/
#endif /* LTERRC_REL9 */
};



/**
 *     @brief This function is used to process the Bind cfm
 *            received from the lower SAP.
 *            
 *     @param[in] sapId        Lower SAP ID
 *     @param[in] lSapCb       Lower SAP control block
 *     @param[in] bndCfmStatus Bind confirm status
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 nhLIMHndlLSapBndCfm
(
U8 sapId,
NhLSapCb *lSapCb,
U8 bndCfmStatus
)
#else /* ANSI */
PUBLIC S16 nhLIMHndlLSapBndCfm(sapId, lSapCb, bndCfmStatus)
U8  sapId;
NhLSapCb  *lSapCb;
U8  bndCfmStatus;
#endif /* ANSI */
{
   S16  ret = RFAILED;
   S16     event;
   NhUstaDgn dgn;       /* Alarm Diagnostics */

   TRC2(nhLIMHndlLSapBndCfm)
   RLOG0(L_DEBUG, "nhLIMHndlLSapBndCfm ");

   switch(sapId)
   {
      case STNHCKWSAP:
         event = NH_TMR_WAIT_CKW_BNDCFM;
         break;
      case STNHKWUSAP:
         event = NH_TMR_WAIT_KWU_BNDCFM;
         break;
      case STNHCRGSAP:
         event = NH_TMR_WAIT_CRG_BNDCFM;
         break;
      case STNHCPJSAP:
         event = NH_TMR_WAIT_CPJ_BNDCFM;
         break;
      case STNHPJUSAP:
         event = NH_TMR_WAIT_PJU_BNDCFM;
         break;
#ifdef RRC_PHY_CFG_SUPP
      case STNHCTFSAP:
         event = NH_TMR_WAIT_CTF_BNDCFM;
         break;
#endif /* RRC_PHY_CFG_SUPP */

      default:
         RLOG1(L_ERROR, "Invalid sapId suId [%d]", sapId);
         RETVALUE(RFAILED);
   }  /* end of switch */

   if( ROK != (ret = nhSchedGenTmr(lSapCb, event, NH_TMR_STOP)))
   {
      RLOG1(L_ERROR, "stop timer failed for event [%d]", event);
      RETVALUE(RFAILED);
   }
   
   lSapCb->bndRetryCnt = 0;

   if (CM_BND_NOK == bndCfmStatus)
   {
      lSapCb->bndRetryCnt = 0;
      lSapCb->state = LNH_SAP_CFG;
      NH_SET_ZERO (&dgn, sizeof(dgn));
      dgn.type = LNH_USTA_DGNVAL_SAP;
      dgn.u.sapInfo.sapId = lSapCb->spId;
      dgn.u.sapInfo.sapElmId = sapId;
      nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                                           LCM_CAUSE_UNKNOWN, &dgn);
      RETVALUE(ROK);
   }
   
   if (CM_BND_OK == bndCfmStatus)
   {
      lSapCb->state = LNH_SAP_BOUND; 
      NH_SET_ZERO (&dgn, sizeof(dgn));
      dgn.type = LNH_USTA_DGNVAL_SAP;
      dgn.u.sapInfo.sapId = lSapCb->spId;
      dgn.u.sapInfo.sapElmId = sapId;
      nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK,
                                           LCM_CAUSE_UNKNOWN, &dgn);
   }

   RETVALUE(ROK);
} /* end of nhPIMHndlLSapBndCfm */

/**
 *
 *   @brief Handles the bind cfm from RLC at the control plane.
 *
 *   @details
 *        This function handles the bind cfm from RLC at the control 
 *             plane.
 *
 *     @param[in] pst    Post structure 
 *     @param[in] suId   Service user SAP ID
 *     @param[in] status Status of bind request
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiCkwBndCfm
(
Pst       *pst,         
SuId      suId,        
U8        status      
)
#else /* ANSI */
PUBLIC S16 NhLiCkwBndCfm(pst, suId, status)
Pst       *pst;      
SuId      suId;     
U8        status;  
#endif /* ANSI */
{
   S16  ret = RFAILED;
   NhLSapCb *ckwSapCb = NULLP;
   NhUstaDgn dgn;       /* Alarm Diagnostics */

   TRC2(NhLiCkwBndCfm);

   UNUSED(pst);

   RLOG2(L_DEBUG, "suId[%d] status[%d]", suId, status);

   ckwSapCb = nhUTLFindLlyrSapFrmSuId(STNHCKWSAP, suId);

   if (ckwSapCb == NULLP)
   {
      RLOG0(L_ERROR, "Invalid suId from lower layer ");
      RETVALUE(RFAILED);
   }

   /* nh003.301: added NH_DIAG */
   RLOG3(L_DEBUG, "Received BndCfm from RLC for control SAP spId[%d] suId[%d]"
     "sap state[%d]",ckwSapCb->spId, suId, ckwSapCb->state);

   if (LNH_SAP_BINDING != ckwSapCb->state)
   {
      RLOG1(L_ERROR, "Invalid Sap state [%d]",ckwSapCb->state);

      NH_SET_ZERO (&dgn, sizeof(dgn));
      dgn.type = LNH_USTA_DGNVAL_SAP;
      dgn.u.sapInfo.sapId = ckwSapCb->spId;
      dgn.u.sapInfo.sapElmId = STNHCKWSAP;
      nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                    LCM_CAUSE_INV_SPID, &dgn);
      RETVALUE(RFAILED);
   }

   ret = nhLIMHndlLSapBndCfm(STNHCKWSAP, ckwSapCb, status);

   RETVALUE(ret);
} /* end of NhLiCkwBndCfm */


/**
 * @brief Handles the bind cfm from RLC at the data plane.
 *
 *      @details  This function handles the bind cfm from RLC at the data 
 *             plane.
 *
 *     @param[in] pst     Post structure 
 *     @param[in] suId    Service user SAP ID
 *     @param[in] status  Status of bind request
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiKwuBndCfm
(
Pst       *pst,          
SuId      suId,         
U8        status       
)
#else /* ANSI */
PUBLIC S16 NhLiKwuBndCfm(pst, suId, status)
Pst       *pst;       
SuId      suId;      
U8        status;   
#endif /* ANSI */
{
   S16  ret = RFAILED;
   NhLSapCb  *kwuSapCb = NULLP;
   NhUstaDgn dgn;       /* Alarm Diagnostics */
   
   TRC2(NhLiKwuBndCfm);

   UNUSED(pst);

   RLOG2(L_DEBUG, "suId[%d] status[%d]", suId, status);

   kwuSapCb = nhUTLFindLlyrSapFrmSuId(STNHKWUSAP,suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (kwuSapCb == NULLP)
   {
      RLOG0(L_ERROR, "Invalid suId from lower layer ");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (LNH_SAP_BINDING != kwuSapCb->state)
   {
      RLOG0(L_ERROR, "Invalid Sap state ");
      NH_SET_ZERO (&dgn, sizeof(dgn));
      dgn.type = LNH_USTA_DGNVAL_SAP;
      dgn.u.sapInfo.sapId = kwuSapCb->spId;
      dgn.u.sapInfo.sapElmId = STNHKWUSAP;
      nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                    LCM_CAUSE_INV_SPID, &dgn);
      RETVALUE(RFAILED);
   }

  /* nh003.301: added NH_DIAG */
   RLOG3(L_DEBUG, "Received BndCfm from RLC for data SAP spId[%d] suId[%d] "
      "sap status[%d]",kwuSapCb->spId, suId, kwuSapCb->state);
   
   ret = nhLIMHndlLSapBndCfm(STNHKWUSAP, kwuSapCb, status);

   RETVALUE(ret);
} /* end of NhLiKwuBndCfm */


/**
 * @brief  Handles the bind cfm from MAC at the control plane.
 *
 *      @details This function handles the bind cfm from MAC at the control 
 *             plane.
 *
 *     @param[in] pst    Post structure
 *     @param[in] suId   Service user SAP ID
 *     @param[in] status Status of bind request
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiCrgBndCfm
(
Pst       *pst,         
SuId      suId,        
U8        status      
)
#else /* ANSI */
PUBLIC S16 NhLiCrgBndCfm(pst, suId, status)
Pst       *pst;      
SuId      suId;     
U8        status;  
#endif /* ANSI */
{
   S16  ret = RFAILED;
   NhLSapCb *crgSapCb = NULLP;
   NhUstaDgn dgn;       /* Alarm Diagnostics */

   TRC2(NhLiCrgBndCfm);

   UNUSED(pst);

   RLOG2(L_DEBUG, "suId[%d] status[%d]", suId, status);

   crgSapCb = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP,suId);

  /* nh003.301: added NH_DIAG */
   RLOG2(L_DEBUG, "Received BndCfm from MAC suId[%d] status[%d]", suId, status);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (crgSapCb == NULLP)
   {
      RLOG0(L_ERROR,"Invalid suId from lower layer ");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (LNH_SAP_BINDING != crgSapCb->state)
   {
      RLOG0(L_ERROR, "Invalid Sap state ");
      NH_SET_ZERO (&dgn, sizeof(dgn));
      dgn.type = LNH_USTA_DGNVAL_SAP;
      dgn.u.sapInfo.sapId = crgSapCb->spId;
      dgn.u.sapInfo.sapElmId = STNHCRGSAP;
      nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                    LCM_CAUSE_INV_SPID, &dgn);
      RETVALUE(RFAILED);
   }

   ret = nhLIMHndlLSapBndCfm(STNHCRGSAP, crgSapCb, status);

   RETVALUE(ret);
} /* end of NhLiCrgBndCfm */


/**
 * @brief  Handles the bind cfm from PDCP at the control plane.
 *
 *      @details  This function handles the bind cfm from PDCP at the control 
 *             plane.
 *
 *     @param[in] pst    Post structure
 *     @param[in] suId   Service user SAP ID
 *     @param[in] status Status of bind request
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiCpjBndCfm
(
Pst       *pst,         
SuId      suId,        
U8        status      
)
#else /* ANSI */
PUBLIC S16 NhLiCpjBndCfm(pst, suId, status)
Pst       *pst;      
SuId      suId;     
U8        status;  
#endif /* ANSI */
{
   NhLSapCb *cpjSapCb = NULLP;
   NhUstaDgn dgn;       /* Alarm Diagnostics */
   S16      ret = RFAILED;
   
   TRC2(NhLiCpjBndCfm);

   UNUSED(pst);

   RLOG2(L_DEBUG, "suId[%d] status[%d]", suId, status);

   cpjSapCb = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (cpjSapCb == NULLP)
   {
      RLOG0(L_ERROR, "Invalid suId from lower layer ");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (LNH_SAP_BINDING != cpjSapCb->state)
   {
      RLOG0(L_ERROR, "Invalid Sap state ");
      NH_SET_ZERO (&dgn, sizeof(dgn));
      dgn.type = LNH_USTA_DGNVAL_SAP;
      dgn.u.sapInfo.sapId = cpjSapCb->spId;
      dgn.u.sapInfo.sapElmId = STNHCPJSAP;
      nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                    LCM_CAUSE_INV_SPID, &dgn);
      RETVALUE(RFAILED);
   }

  /* nh003.301: added NH_DIAG */
   RLOG2(L_DEBUG, "RRC Recived Sap Bind Cfm from PDCP suId[%d] status[%d]",
      suId, status);

   ret = nhLIMHndlLSapBndCfm(STNHCPJSAP, cpjSapCb, status);

   RETVALUE(ret);
} /* end of NhLiCpjBndCfm */


/**
 * @brief  Handles the bind cfm from PDCP at the data plane.
 *
 *      @details  This function handles the bind cfm from PDCP at the data 
 *             plane.
 *
 *     @param[in] pst    Post structure
 *     @param[in] suId   Service user SAP ID
 *     @param[in] status Status of bind request
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiPjuBndCfm
(
Pst       *pst,         
SuId      suId,        
U8        status      
)
#else /* ANSI */
PUBLIC S16 NhLiPjuBndCfm(pst, suId, status)
Pst       *pst;      
SuId      suId;     
U8        status;  
#endif /* ANSI */
{
   NhLSapCb *pjuSapCb = NULLP;
   NhUstaDgn dgn;       /* Alarm Diagnostics */
   S16      ret = RFAILED;
   
   TRC2(NhLiPjuBndCfm);

   UNUSED(pst);

   RLOG2(L_DEBUG, "Received BndCfm from PDCP suId[%d] status[%d]",suId, status);
   pjuSapCb = nhUTLFindLlyrSapFrmSuId(STNHPJUSAP,suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pjuSapCb == NULLP)
   {
      RLOG0(L_ERROR,"Invalid suId from lower layer ");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (LNH_SAP_BINDING != pjuSapCb->state)
   {
      RLOG0(L_ERROR, "Invalid Sap state ");
      NH_SET_ZERO (&dgn, sizeof(dgn));
      dgn.type = LNH_USTA_DGNVAL_SAP;
      dgn.u.sapInfo.sapId = pjuSapCb->spId;
      dgn.u.sapInfo.sapElmId = STNHPJUSAP;
      nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                    LCM_CAUSE_INV_SPID, &dgn);
      RETVALUE(RFAILED);
   }

   ret = nhLIMHndlLSapBndCfm(STNHPJUSAP, pjuSapCb, status);

   RETVALUE(ret);
} /* end of NhLiPjuBndCfm */

#ifdef RRC_PHY_CFG_SUPP
/**
 * @brief  Handles the bind cfm from PHY at the data plane.
 *
 *      @details  This function handles the bind cfm from PHY at the control 
 *             plane.
 *
 *     @param[in] pst    Post structure
 *     @param[in] suId   Service user SAP ID
 *     @param[in] status Status of bind request
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiCtfBndCfm
(
Pst       *pst,         
SuId      suId,        
U8        status      
)
#else /* ANSI */
PUBLIC S16 NhLiCtfBndCfm(pst, suId, status)
Pst       *pst;      
SuId      suId;     
U8        status;  
#endif /* ANSI */
{
   NhLSapCb *ctfSapCb = NULLP;
   NhUstaDgn dgn;       /* Alarm Diagnostics */
   S16      ret = RFAILED;
   
   TRC2(NhLiCtfBndCfm);

   UNUSED(pst);

   RLOG2(L_DEBUG, "Received BndCfm from PHY suId[%d] status[%d]", suId, status);

   ctfSapCb = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP,suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (ctfSapCb == NULLP)
   {
      RLOG0(L_ERROR,"Invalid suId from lower layer ");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (LNH_SAP_BINDING != ctfSapCb->state)
   {
      RLOG0(L_ERROR, "Invalid Sap state ");
      NH_SET_ZERO (&dgn, sizeof(dgn));
      dgn.type = LNH_USTA_DGNVAL_SAP;
      dgn.u.sapInfo.sapId = ctfSapCb->spId;
      dgn.u.sapInfo.sapElmId = STNHCTFSAP;
      nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                    LCM_CAUSE_INV_SPID, &dgn);
      RETVALUE(RFAILED);
   }

   ret = nhLIMHndlLSapBndCfm(STNHCTFSAP, ctfSapCb, status);

   RETVALUE(ret);
} /* end of NhLiCtfBndCfm */

#endif /* RRC_PHY_CFG_SUPP */

/**
 * @brief To receive message from RLC layer at the KWU interface.
 *
 *     @details This function is used to receive message from
 *            RLC layer at the KWU interface.
 *            
 *     @param[in] pst         Post structure 
 *     @param[in] suId        Lower SAP ID 
 *     @param[in] diParam     RLC data indication structure
 *     @param[in] mBuf        RLC message buffer
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiKwuDatInd
(
Pst *pst,                      
SuId suId,                    
KwuDatIndInfo *diParam,      
Buffer        *mBuf         
)
#else /* ANSI */
PUBLIC S16 NhLiKwuDatInd(pst, suId, diParam, mBuf)
Pst *pst;                  
SuId suId;                
KwuDatIndInfo *diParam;  
Buffer        *mBuf;    
#endif /* ANSI */
{
   NhId  nhIdentity;                  /* RRC ID */
   NhLSapCb *kwuSapCb = NULLP;  /* KWU SAP control Cb */
   CmLteRnti      tCrnti = 0;   /* Temporary C-RNTI */


   TRC3(NhLiKwuDatInd)

   UNUSED(pst);

   RLOG1(L_DEBUG, "suId[%d]", suId);

   /* Error Check - check for null pointer passed by RLC */
   if (NH_CHK_INPUT_PARAM(mBuf))
   {
      RLOG0(L_FATAL, "GOT NULL mBuf");
      if (!(NH_CHK_INPUT_PARAM(diParam)))
      {
         NH_FREE_SHRABL_BUF_PST(pst->region, pst->pool, diParam, sizeof(KwuDatIndInfo));
      }
      RETVALUE(RFAILED);
   }
   if (NH_CHK_INPUT_PARAM(diParam))
   {
      RLOG0(L_FATAL, "GOT NULL diParam");
      NH_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Get the sapCb based on suId */
   kwuSapCb = nhUTLFindLlyrSapFrmSuId(STNHKWUSAP, suId);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (kwuSapCb == NULLP)
   {
      RLOG0(L_ERROR, "SAP CHECK Failed in NhLiKwuDatInd");
      /* no sapCb, so free memory */
      NH_FREEMBUF(mBuf);
      NH_FREE_SHRABL_BUF_PST(pst->region, pst->pool, diParam, sizeof(KwuDatIndInfo));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   /* generate message trace */
#ifdef NH_TRACE_ENABLED
   NH_GEN_TRC(suId, STNHKWUSAP, LNH_MSG_RECVD, mBuf);
#endif/* NH_TRACE_ENABLED */

   /* Map RlcId parameters to NhId */
   NH_MAP_RLCID_TO_NHID(nhIdentity, (diParam->rlcId));

#ifdef CCPU_OPT
   tCrnti = diParam->tCrnti;
#endif /* CCPU_OPT */

   RLOG2(L_DEBUG, "Received DatInd from RLC suId[%d] rbId[%d]", suId, nhIdentity.rbId);
   
   /* Call common datInd handler. Same handler is called for PJU datInd */
   if (ROK != nhLIMHndlDatInd(&nhIdentity, mBuf, tCrnti))
   {
      RLOG0(L_ERROR, "nhLIMHndlDatInd Failed ");
      NH_FREEMBUF(mBuf);
      NH_FREE_SHRABL_BUF_PST(pst->region, pst->pool, diParam, sizeof(KwuDatIndInfo));
      RETVALUE(RFAILED);
   }

   /* Free buffer , it is not freed up in nhLIMHndlDatInd */ 
   NH_FREEMBUF(mBuf);
   NH_FREE_SHRABL_BUF_PST(pst->region, pst->pool, diParam, sizeof(KwuDatIndInfo));

   RETVALUE(ROK);
}  /* end of NhLiKwuDatInd */


/**
 * @brief To receive message from PDCP layer at PJU interface.
 *
 *     @details This function is used to receive message from
 *            RLC layer at the PJU interface.
 *            
 *     @param[in] pst              Post structure
 *     @param[in] suId             Lower SAP ID
 *     @param[in] pdcpId           PdcpId structure
 *     @param[in] mBuf             PDCP message buffer
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 NhLiPjuDatInd
(
Pst *pst,
SuId suId,
CmLtePdcpId *pdcpId,
Buffer *mBuf
)
#else /* ANSI */
PUBLIC S16 NhLiPjuDatInd(pst, suId, pdcpId, mBuf)
Pst *pst;
SuId suId;
CmLtePdcpId *pdcpId;
Buffer *mBuf;
#endif /* ANSI */
{
   NhId  nhIdentity;
   NhLSapCb *pjuSapCb = NULLP;
   CmLteRnti      tCrnti = 0;   /* Temporary C-RNTI */

   TRC2(NhLiPjuDatInd)

   UNUSED(pst);


   /* Error Check - check for null pointer passed by PDCP */
   if (NH_CHK_INPUT_PARAM(mBuf))
   {
      RLOG0(L_FATAL, "GOT NULL mBuf");
      RETVALUE(RFAILED);
   }
   if (NH_CHK_INPUT_PARAM(pdcpId))
   {
      RLOG0(L_FATAL, "GOT NULL pdcpId");
      NH_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
   }
   RLOG2(L_DEBUG, "suId[%d] pdcpId[%d]", suId, pdcpId->rbId);

   pjuSapCb = nhUTLFindLlyrSapFrmSuId(STNHPJUSAP, suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pjuSapCb == NULLP)
   {
      RLOG0(L_ERROR, "SAP CHECK Failed in NhLiPjuDatInd");
      NH_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   /* generate message trace */
#ifdef NH_TRACE_ENABLED
   NH_GEN_TRC(suId, STNHPJUSAP, LNH_MSG_RECVD, mBuf);
#endif /* NH_TRACE_ENABLED */

   /* Map PdcpId parameters to NhId */
   NH_MAP_PDCPID_TO_NHID(nhIdentity, pdcpId);

   RLOG2(L_DEBUG, "Received DatInd from PDCP suId[%d] pdcp UeId[%d]",
         suId, pdcpId->ueId);

   if (ROK != nhLIMHndlDatInd(&nhIdentity, mBuf, tCrnti))
   {
      RLOG0(L_ERROR, "nhLIMHndlDatInd Failed ");
      NH_FREEMBUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Free buffer , it is not freed up in nhLIMHndlDatInd */ 
   NH_FREEMBUF(mBuf);
   RETVALUE(ROK);
}/* End of NhLiPjuDatInd */

/**
 *
 * @brief Handler to receive the confirm from PDCP layer for data request.
 *
 *  @details This function is used to receive the confirm 
 *            from PDCP layer for data request.
 *            
 *     @param[in] pst     Post structure
 *     @param[in] suId    Lower SAP ID
 *     @param[in] pdcpId  PDCP ID 
 *     @param[in] datCfm  PDCP Data confirm 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiPjuDatCfm
(
Pst *pst,          
SuId suId,         
CmLtePdcpId *pdcpId,
PjuDatCfmInfo *datCfm
)
#else /* ANSI */
PUBLIC S16 NhLiPjuDatCfm(pst, suId, pdcpId, datCfm)
Pst *pst;         
SuId suId;       
CmLtePdcpId *pdcpId;
PjuDatCfmInfo *datCfm;
#endif /* ANSI */
{
   NhActvUeCb      *ueCb = NULLP;  /* UE control block */
   MuiInfo         *muiInfo;       /* MUI information */
   U32             nxTransId;
   U8              msgTransId;
   NhId            nhIdentity;
   NhCellAndUeCb   cellandUeCb;
   NhCellCb        *cellCb = NULLP;
   TransCb         *transCtrlBlk = NULLP;
   U8              nhuCause = NHU_ERRCAUSE_NONE;
   U32             idx; 
   U32             sduNum; 

   TRC2(NhLiPjuDatCfm)

   UNUSED(pst);



   if (NH_CHK_INPUT_PARAM(datCfm))
   {
      RLOG0(L_FATAL, "GOT NULL datCfm");
      RETVALUE(RFAILED);
   }
   if (NH_CHK_INPUT_PARAM(pdcpId))
   {
      RLOG0(L_FATAL, "GOT NULL pdcpId");
      RETVALUE(RFAILED);
   }

   /* validate that atleast one element is present in datCfm */
   if (datCfm->numSdus == 0)
   {
      RLOG0(L_ERROR, "Invalid interface param");
      RETVALUE(RFAILED); 
   }

   nhIdentity.cellId = pdcpId->cellId;
   nhIdentity.ueId = pdcpId->ueId;
   nhIdentity.isPdcpId = TRUE;
   nhIdentity.rbId = pdcpId->rbId;

   if (NULLP == (cellCb = nhDBMGetCellCb(nhIdentity.cellId)))
   {
      RLOG0(L_ERROR, "NULL CELL CB ");
      RETVALUE(RFAILED); 
   }

   /* Get UE control block based on UE ID */
   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, nhIdentity.ueId)))
   {
      RLOG0(L_ERROR, "NULL UE CB ");
      RETVALUE(RFAILED); 
   }

   sduNum = datCfm->numSdus;
   for(idx = 0; idx < sduNum; idx++)
   {
      /* Find MUI node in list with specified MUI value */
      if ((MuiInfo*)NULLP == 
            (muiInfo = nhDBMGetMuiCb(ueCb, datCfm->cfmSta[idx].sduId)))
      {
         RLOG0(L_ERROR, "mui doesnt exist ");
         RETVALUE(RFAILED);
      }

      nxTransId = muiInfo->nxTransId;
      nhDBMDelMuiCb(ueCb, &muiInfo);

      RLOG_ARG2(L_DEBUG, DBG_CRNTI, ueCb->ueCbKey.crnti, "Received DatCfm from "
            "PDCP suId[%d] status[%d]", suId, datCfm->cfmSta[idx].status);
      /*
       * Not checking the ret of the above function since it always 
       * returns ROK. If in future any changes are done in this function 
       * then return value validation might be required.
       */
      if (PJU_DAT_CFM_SUCCESS == datCfm->cfmSta[idx].status)
      {
         /* Data is transmitted by PDCP successfully, remove MUI 
          * entry from muiList in ueCb.
          */
         RLOG0(L_DEBUG,"status is SUCCESS, deleting mui");

         /* Send Data Confirm through UIM */
         NH_SEND_DAT_CFM(nhIdentity, nxTransId, NHU_ERRCAUSE_NONE);

         transCtrlBlk = nhUTLGetTransCbFrmNxTransId(ueCb, nxTransId);
         if (transCtrlBlk != NULLP) 
         {
            if (transCtrlBlk->cause == NH_MSGTYPE_CONNRLS)
            {
               /* Message TransId is required here, but it can be fetched only from
                * message transaction key, as it is Data Confirm case. TransId is
                * only from 0 to 3. It is present in the last byte of message
                * transaction key. */
               msgTransId = transCtrlBlk->msgTransKey & 0x0F;
               cellandUeCb.cellCb = cellCb;
               cellandUeCb.ueCb = ueCb;
               /* msgType is not required in this case. So set to NH_ZERO_VAL */
               if (ROK != nhLISMTbl[NH_EVNT_LI_RRC_CON_RLS_CMP](NULLP, 
                        &cellandUeCb, msgTransId, NH_ZERO_VAL))
               {
                  RLOG0(L_ERROR, "Lower state Machine Function Failed ");
                  RETVALUE(RFAILED);
               }
            }
         }
      }
      /* For discard and maximum RLC retransmissions, send error 
       * indication to user */   
      else
      {

         transCtrlBlk = nhUTLGetTransCbFrmNxTransId(ueCb, nxTransId);
         if (transCtrlBlk != NULLP) 
         {
            if (transCtrlBlk->cause == NH_MSGTYPE_CONNRLS)
            {
               (Void)nhDBMDelTransCb(ueCb, &transCtrlBlk);
               /* 
                * Not checking the ret of the above function since it always 
                * returns ROK. If in future any changes are done in this function 
                * then add the ret val check and if it is not ROK
                * Free pdcpId, datCfm and set the flag released as TRUE.
                */
            }
         }

         /* map PDCP cause to NHU cause */
         NH_MAP_PDCP_DAT_CFM_CAUSE(datCfm->cfmSta[idx].status, nhuCause);

         /* Update General Statistics */
         if(NHU_ERRCAUSE_MAX_RLC_TRANS_REACH == nhuCause)
         {
            nhUTLUpdGenSts(NH_STS_CAUSE_RLC_ERR);
         }
         else
         {
            nhUTLUpdGenSts(NH_STS_CAUSE_PDCP_ERR);
         }

         /* Send Data Confirm through UIM */
         NH_SEND_DAT_CFM(nhIdentity, nxTransId, nhuCause);
      }
   }
   RETVALUE(ROK);
} /* end of NhLiPjuDatCfm */



/**
 * @brief Handler is used to receive the confirm 
 *            from PDCP layer for Configuration Request.
 *
 * @details This function is used to receive the confirm 
 *            from PDCP layer for Configuration Request.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower SAP ID
 *     @param[in] pdcpCfgCfm PDCP Data confirm 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 NhLiCpjCfgCfm
(
Pst                 *pst,           
SuId                 suId,         
CpjCfgCfmInfo            *pdcpCfgCfm   
)
#else /* ANSI */
PUBLIC S16 NhLiCpjCfgCfm(pst, suId, pdcpCfgCfm)
Pst                  *pst;       
SuId                  suId;     
CpjCfgCfmInfo             *pdcpCfgCfm; 
#endif /* ANSI */
{
   NhLSapCb          *cpjSapCb = NULLP;   /* CPJ SAP CB */

   TRC3(NhLiCpjCfgCfm)

   UNUSED(pst);

   RLOG1(L_DEBUG, "suId[%d]", suId);

   if (NH_CHK_INPUT_PARAM(pdcpCfgCfm))
   {
      RLOG0(L_FATAL, "GOT NULL pdcpCfgCfm");
      RETVALUE(RFAILED);
   }

   cpjSapCb = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (cpjSapCb == NULLP)
   {
      RLOG0(L_ERROR, " INVALID SAP ID in NhLiCpjCfgCfm");
      NH_FREE(pst->region, pst->pool, pdcpCfgCfm, sizeof(CpjCfgCfmInfo));
      RETVALUE(RFAILED);
   }
#endif/* (ERRCLASS & ERRCLS_INT_PAR) */

   if (ROK != nhLIMHndlPdcpCfgCfm(pdcpCfgCfm))
   {
      RLOG0(L_ERROR, "nhLIMHndlPdcpCfgCfm failed ");
      NH_FREE(pst->region, pst->pool, pdcpCfgCfm, sizeof(CpjCfgCfmInfo));
      RETVALUE(RFAILED);
   }

   NH_FREE(pst->region, pst->pool, pdcpCfgCfm, sizeof(CpjCfgCfmInfo)); 
   
   RETVALUE(ROK);
}   /* end of NhLiCpjCfgCfm */

/**
 *
 *     @brief This function is used to receive the UeId Change confirm 
 *            from PDCP layer for UeId change Request.
 * 
 *     @details This function is used to receive the UeId change confirm 
 *            from PDCP layer for UeId change Request.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower sap Id
 *     @param[in] transId    Transaction Id
 *     @param[in] ueInfo     Old UE Info
 *     @param[in] status     Status success/failure with reason 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiCpjUeIdChgCfm
(
Pst       *pst,            
SuId       suId,
U32        transId,
CpjUeInfo *ueInfo, 
CmStatus   status
)  
#else /* ANSI */
PUBLIC S16 NhLiCpjUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst        *pst;             
SuId        suId;
U32         transId;
CpjUeInfo  *ueInfo;
CmStatus    status;
#endif /* ANSI */
{
   NhLSapCb *cpjSapCb = NULLP;
   NhUstaDgn dgn;
   NhId      nhIdentity;

   TRC3(NhLiCpjUeIdChgCfm)
  
   UNUSED(pst);

   if (NH_CHK_INPUT_PARAM(ueInfo))
   {
      RLOG0(L_FATAL, "GOT NULL ueInfo");
      RETVALUE(RFAILED);
   }

   RLOG1(L_DEBUG, "status [%d]", status.status);

   cpjSapCb = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP, suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (cpjSapCb == NULLP)
   {
      RLOG0(L_ERROR, "SAP CHECK Failed in NhLiCpjUeIdChgCfm ");
      NH_FREE(pst->region, pst->pool, ueInfo,sizeof(CpjUeInfo));
      RETVALUE(RFAILED);
   }
#endif/* (ERRCLASS & ERRCLS_INT_PAR) */

   if (transId == NH_RESERVED_TRANS_ID)
   {
      /* Raise a critical alarm to layer manager */
      if (status.status == CPJ_CFG_CFM_NOK)
      {
         NH_SET_ZERO (&dgn, sizeof(dgn));
         dgn.type = LNH_USTA_DGNVAL_CELLUEID;
         dgn.u.cellUeId.ueId = ueInfo->ueId;
         dgn.u.cellUeId.cellId = ueInfo->cellId;
         nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LNH_EVENT_PDCP_RVRT_FAIL,
                      LCM_CAUSE_UNKNOWN, &dgn);
      }
      NH_FREE(pst->region, pst->pool, ueInfo,sizeof(CpjUeInfo));
      RETVALUE(ROK);
   }
   nhIdentity.cellId = ueInfo->cellId;
   nhIdentity.ueId   = ueInfo->ueId;

   RLOG2(L_DEBUG,"Received UeIdChgCfm from PDCP suId[%d] status[%d]",
         suId, status.status);

   /* Call LIM Module function */
   if (ROK != nhLIMHndlUeIdChgCfm(&nhIdentity, transId, status, 
                                  NH_PDCP_CONFIGURED))
   {
      RLOG0(L_ERROR, "nhLIMHndlUeIdChgCfgCfm failed");
      NH_FREE(pst->region, pst->pool, ueInfo,sizeof(CpjUeInfo));
      RETVALUE(RFAILED);
   }

   NH_FREE(pst->region, pst->pool, ueInfo,sizeof(CpjUeInfo));

   RETVALUE(ROK);
}  /* end of NhLiCpjUeIdChgCfm */

#ifdef RRC_PHY_CFG_SUPP
/**********************************************************
 *
 *     @brief This function is used to receive the confirm 
 *            from PHY layer for Configuration Request.
 *
 *     @details This function is used to receive the confirm 
 *            from PHY layer for Configuration Request.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower SAP ID
 *     @param[in] transId    PHY transaction ID
 *     @param[in] status     Status of configuration request
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiCtfCfgCfm
(
Pst *pst,             
SuId suId,           
CtfCfgTransId transId,
U8 status            
)
#else /* ANSI */
PUBLIC S16 NhLiCtfCfgCfm(pst, suId, transId, status)
Pst *pst;           
SuId suId;         
CtfCfgTransId transId; 
U8 status;            
#endif /* ANSI */
{
   NhLSapCb   *ctfSapCb = NULLP;

   TRC3(NhLiCtfCfgCfm)

   UNUSED(pst);

   RLOG2(L_DEBUG, "Received CfgCfm from PHY suId[%d] status[%d]", suId, status);

   ctfSapCb = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP, suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (ctfSapCb == NULLP)
   {
      RLOG0(L_ERROR, "SAP CHECK Failed in NjLiCtfCfgCfm");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   /* Call LIM Module function */
   if (ROK != nhLIMHndlPhyCfgCfm(transId, status))
   {
      RLOG0(L_ERROR, "nhLIMHndlPhyCfgCfm failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}   /* end of NhLiCtfCfgCfm */

/**
 *
 *     @brief This function is used to receive the UeId Change confirm 
 *            from PHY layer for UeId change Request.
 * 
 *     @details This function is used to receive the UeId change confirm 
 *            from PHY layer for UeId change Request.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower sap Id
 *     @param[in] transId    Transaction Id
 *     @param[in] ueInfo     Old UE Info
 *     @param[in] status     Status success/failure with reason 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiCtfUeIdChgCfm
(
Pst       *pst,            
SuId       suId,
CtfCfgTransId transId,
CtfUeInfo *ueInfo, 
CmStatus status
)  
#else /* ANSI */
PUBLIC S16 NhLiCtfUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst        *pst;             
SuId        suId;
CtfCfgTransId         transId;
CtfUeInfo  *ueInfo;
CmStatus status;
#endif /* ANSI */
{
   NhLSapCb    *ctfSapCb = NULLP;
   NhUstaDgn   dgn;
   NhId        nhIdentity;
   U8          cfgElms = 0x00;
   NhTransId   nxTransId;
   U8          phyActionBitMask = 0x00;
   U8          *transIdPtr = NULLP;

   TRC3(NhLiCtfUeIdChgCfm)
  
   UNUSED(pst);

   transIdPtr = transId.trans;
   
   /* First byte is NHU_CELL_ID/NHU_UE_ID */
   NH_MEM_COPY((U8*)&nhIdentity.cellId, (CONSTANT U8*)transIdPtr, sizeof(U16));
   transIdPtr += sizeof(U16);
   NH_MEM_COPY((U8*)&nhIdentity.ueId, (CONSTANT U8*)transIdPtr, sizeof(U16));
   transIdPtr += sizeof(U16);

   NH_MEM_COPY((U8*)&nxTransId, (CONSTANT U8*)transIdPtr, sizeof(NhTransId));
   transIdPtr += sizeof(NhTransId);
   NH_MEM_COPY((U8*)&cfgElms, (CONSTANT U8*)transIdPtr, sizeof(U8));
   transIdPtr += sizeof(U8);
   NH_MEM_COPY((U8*)&phyActionBitMask, (CONSTANT U8*)transIdPtr, sizeof(U8));

   RLOG_ARG2(L_DEBUG, DBG_TRNSID,nxTransId,"status[%d] bitmask[%d]",
      status.status, phyActionBitMask);

   if (NH_CHK_INPUT_PARAM(ueInfo))
   {
      RLOG0(L_FATAL, "GOT NULL ueInfo");
      RETVALUE(RFAILED);
   }

   RLOG3(L_DEBUG,"Received UeIdChgCfm from PHY suId [%d]status [%d] transId [%d]",
         suId, status.status, nxTransId);

   ctfSapCb = nhUTLFindLlyrSapFrmSuId(STNHCTFSAP, suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (ctfSapCb == NULLP)
   {
      RLOG0(L_ERROR, "SAP CHECK Failed in NhLiCtfUeIdChgCfm ");
      NH_FREE(NH_REG, NH_POOL, ueInfo,sizeof(CtfUeInfo));
      RETVALUE(RFAILED);
   }
#endif/* (ERRCLASS & ERRCLS_INT_PAR) */

   if (nxTransId == NH_RESERVED_TRANS_ID)
   {
      /* Raise a critical alarm to layer manager */
      if (status.status == CTF_CFG_CFM_NOK)
      {
         NH_SET_ZERO (&dgn, sizeof(dgn));
         dgn.type = LNH_USTA_DGNVAL_CELLUEID;
         dgn.u.cellUeId.ueId = ueInfo->ueId;
         dgn.u.cellUeId.cellId = ueInfo->cellId;
         nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LNH_EVENT_PDCP_RVRT_FAIL,
                      LCM_CAUSE_UNKNOWN, &dgn);
      }
      NH_FREE(NH_REG, NH_POOL, ueInfo,sizeof(CtfUeInfo));
      RETVALUE(ROK);
   }
   nhIdentity.cellId = ueInfo->cellId;
   nhIdentity.ueId   = ueInfo->ueId;

   /* Call LIM Module function */
   if (ROK != nhLIMHndlUeIdChgCfm(&nhIdentity, nxTransId, status, 
                                  NH_PHY_CONFIGURED))
   {
      RLOG0(L_ERROR, "nhLIMHndlUeIdChgCfgCfm failed");
      NH_FREE(NH_REG, NH_POOL, ueInfo,sizeof(CtfUeInfo));
      RETVALUE(RFAILED);
   }

   NH_FREE(pst->region, pst->pool, ueInfo,sizeof(CtfUeInfo));

   RETVALUE(ROK);
}  /* end of NhLiCtfUeIdChgCfm */

#endif /* RRC_PHY_CFG_SUPP */

/**
 *
 *     @brief This function is used to receive the re-establishment confirm 
 *            from PDCP layer.
 * 
 *     @details This function is used to receive the re-establish confirm 
 *            from PDCP layer for re-establish Request only for SRB1.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower sap Id
 *     @param[in] restbCfm   Confirm info. 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiCpjReEstCfm
(
Pst         *pst,            
SuId         suId,
CpjReEstCfmInfo *restbCfm
)  
#else /* ANSI */
PUBLIC S16 NhLiCpjReEstCfm(pst, suId, restbCfm)
Pst          *pst;             
SuId          suId;
CpjReEstCfmInfo  *restbCfm;
#endif /* ANSI */
{
   NhLSapCb *cpjSapCb = NULLP;
   NhId      nhIdentity;

   TRC3(NhLiCpjReEstCfm)
  
   UNUSED(pst);

   if (NH_CHK_INPUT_PARAM(restbCfm))
   {
      RLOG0(L_FATAL, "GOT NULL restbCfm");
      RETVALUE(RFAILED);
   }

   RLOG1(L_DEBUG,"status [%d]", restbCfm->status);

   cpjSapCb = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP, suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (cpjSapCb == NULLP)
   {
      RLOG0(L_ERROR, "SAP CHECK Failed in NhLiCpjReEstCfm ");
      NH_FREE(pst->region, pst->pool, restbCfm,sizeof(CpjReEstCfmInfo));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   nhIdentity.cellId = restbCfm->cellId;
   nhIdentity.ueId   = restbCfm->ueId;

   RLOG2(L_DEBUG, "Received ReEstCfm from PDCP, re-establish Request only for SRB1 suId[%d] status[%d]",
         suId, restbCfm->status);
   /* Call LIM Module function */
   if (ROK != nhLIMHndlRestbCfgCfm(&nhIdentity, restbCfm->transId, 
                                   restbCfm->status ))
   {
      RLOG0(L_ERROR, "nhLIMHndlRestbCfgCfm failed");
      NH_FREE(pst->region, pst->pool, restbCfm,sizeof(CpjReEstCfmInfo));
      RETVALUE(RFAILED);
   }

   NH_FREE(pst->region, pst->pool, restbCfm,sizeof(CpjReEstCfmInfo));
   
   RETVALUE(ROK);
}  /* end of NhLiCpjReEstCfm */

/**
 *
 *     @brief  This function is used to receive PDCP security configuration  
 *              confirmation from PDCP layer.
 * 
 *     @details  This function is used to receive PDCP security configuration  
 *              confirmation from PDCP layer.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower sap Id
 *     @param[in] secCfgCfm  PDCP Security configuration confirmation structure 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 NhLiCpjSecCfgCfm
(
Pst       *pst,
SuId       suId,
CpjSecCfgCfmInfo *secCfgCfm
)
#else /* ANSI */
PUBLIC S16 NhLiCpjSecCfgCfm(pst, suId, secCfgCfm)
Pst        *pst;
SuId        suId;
CpjSecCfgCfmInfo *secCfgCfm;
#endif /* ANSI */
{

   NhLSapCb          *cpjSapCb = NULLP;   /* CPJ SAP CB */

   TRC3(NhLiCpjSecCfgCfm)

   UNUSED(pst);

   RLOG1(L_DEBUG, "suId[%d]", suId);

   if (NH_CHK_INPUT_PARAM(secCfgCfm))
   {
      RLOG0(L_FATAL, "GOT NULL secCfgCfm");
      RETVALUE(RFAILED);
   }

   cpjSapCb = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (cpjSapCb == NULLP)
   {
      RLOG0(L_ERROR, " INVALID SAP ID in NhLiCpjCfgCfm");
      NH_FREE(pst->region, pst->pool, secCfgCfm, sizeof(CpjSecCfgCfmInfo));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (ROK != nhLIMHndlPdcpSecCfgCfm(secCfgCfm))
   {
      RLOG0(L_ERROR, "nhLIMHndlPdcpSecCfgCfm failed ");
      NH_FREE(pst->region, pst->pool, secCfgCfm, sizeof(CpjSecCfgCfmInfo));
      RETVALUE(RFAILED);
   }

   RLOG2(L_DEBUG," Received SecCfgCfm from PDCP suId [%d]status [%d]", suId, 0);

   NH_FREE(pst->region, pst->pool, secCfgCfm, sizeof(CpjSecCfgCfmInfo)); 
   
   RETVALUE(ROK);

}  /* end of NhLiCpjSecCfgCfm */

/**
 *
 *     @brief  This function is used to receive COUNT values of all 
 *             established DRB from PDCP layer.
 * 
 *     @details  This function is used to receive COUNT values of 
 *               all established DRB from PDCP layer.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower sap Id
 *     @param[in] countCfmInfo   PDCP Count confirmation structure containing 
 *                               COUNT values of DRBs
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 NhLiCpjCountCfm
(
Pst       *pst,
SuId       suId,
CpjCountCfmInfo *countCfmInfo
)
#else /* ANSI */
PUBLIC S16 NhLiCpjCountCfm(pst, suId, countCfmInfo)
Pst        *pst;
SuId        suId;
CpjCountCfmInfo *countCfmInfo;
#endif /* ANSI */
{

   NhLSapCb *cpjSapCb = NULLP;
   NhCellCb    *cellCb = NULLP;
   NhActvUeCb    *ueCb = NULLP;

   TRC3(NhLiCpjCountCfm)

   UNUSED(pst);

   if (NH_CHK_INPUT_PARAM(countCfmInfo))
   {
      RLOG0(L_FATAL, "GOT NULL countCfmInfo");
      RETVALUE(RFAILED);
   }

   RLOG2(L_INFO, "NhLiCpjCountCfm: \
                for UeId (%d) and status %d \n", countCfmInfo->ueId, 
                countCfmInfo->status);

   cpjSapCb = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP, suId);

   RLOG2(L_DEBUG, "Received CountCfm, COUNT values of all established DRB from PDCP layer suId[%d] status[%d]",
         suId, countCfmInfo->status);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (cpjSapCb == NULLP)
   {
      RLOG0(L_ERROR, "SAP CHECK Failed in NhLiCpjUeIdChgCfm ");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (NULLP == (cellCb = nhDBMGetCellCb(countCfmInfo->cellId)))
   {
      RLOG0(L_ERROR, "NULL CELL CB ");
      NH_FREE(pst->region, pst->pool, countCfmInfo,sizeof(CpjCountCfmInfo));
      RETVALUE(RFAILED);
   }

   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, countCfmInfo->ueId)))
   {
      RLOG0(L_ERROR, "NULL UE CB ");
      NH_FREE(pst->region, pst->pool, countCfmInfo,sizeof(CpjCountCfmInfo));
      RETVALUE(RFAILED);
   }

   /* Call LIM Module function */
   if (ROK != nhLIMHndlCountCfm(countCfmInfo))
   {
      RLOG0(L_ERROR, "NhLiCpjCountCfm failed");
      NH_FREE(pst->region, pst->pool, countCfmInfo,sizeof(CpjCountCfmInfo));
      RETVALUE(RFAILED);
   }

   NH_FREE(pst->region, pst->pool, countCfmInfo,sizeof(CpjCountCfmInfo));

   RETVALUE(ROK);

}  /* end of NhLiCpjCountCfm */

/**
 *
 *     @brief  This function is used to receive COUNT values of all 
 *             established DRB from PDCP layer.
 * 
 *     @details  This function is used to receive COUNT values of all 
 *               established DRB from PDCP layer.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower sap Id
 *     @param[in] cpjSduStaCfm   PDCP Count confirmation structure 
 *                               containing COUNT values of DRBs
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 NhLiCpjSduStaCfm
(
Pst          *pst,
SuId         suId,
CpjSduStaCfmInfo *cpjSduStaCfm
)
#else /* ANSI */
PUBLIC S16 NhLiCpjSduStaCfm(pst, suId, cpjSduStaCfm)
Pst          *pst;
SuId         suId;
CpjSduStaCfmInfo *cpjSduStaCfm;
#endif /* ANSI */
{
   NhLSapCb          *cpjSapCb = NULLP;   /* CPJ SAP CB */
   NhCellCb          *cellCb;
   NhActvUeCb        *ueCb; 
   TransCb           *transCtrlBlk; 
   U8                cfmStatus = NH_CFG_SUCCESS;
   NhuPdcpSduStaCfm  *nhuSduStaCfm;
   S16               retVal;
   NhCellAndUeCb     cellAndUeCb;
   Mem               sMem;
   NhId              nhIdentity; /*nh002.301:118385:HO issue: nh identity*/

   TRC3(NhLiCpjSduStaCfm)

   UNUSED(pst);

   RLOG1(L_DEBUG, "suId[%d]", suId);

   if (NH_CHK_INPUT_PARAM(cpjSduStaCfm))
   {
      RLOG0(L_FATAL, "GOT NULL cpjSduStaCfm");
      RETVALUE(RFAILED);
   }

   retVal = ROK;

   cpjSapCb = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (cpjSapCb == NULLP)
   {
      RLOG0(L_ERROR, " INVALID SAP ID in NhLiCpjSduStaCfm");
      nhLIMFreeCpjSduStaCfm(pst, cpjSduStaCfm);
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (NULLP == (cellCb = nhDBMGetCellCb(cpjSduStaCfm->cellId)))
   {
      RLOG0(L_ERROR, "NULL CELL CB ");
      nhLIMFreeCpjSduStaCfm(pst,cpjSduStaCfm);
      RETVALUE(RFAILED); 
   }

   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, cpjSduStaCfm->ueId)))
   {
      RLOG0(L_ERROR, "NULL UE CB ");
      nhLIMFreeCpjSduStaCfm(pst,cpjSduStaCfm);
      RETVALUE(RFAILED); 
   }

   transCtrlBlk = nhUTLGetTransCbFrmNxTransId(ueCb, cpjSduStaCfm->transId);

   /* Find trans node in list with specified transId value */
   if ( ((TransCb*)NULLP == transCtrlBlk) || 
        (transCtrlBlk->cause != NH_TRANSCB_SDU_STA_RESTAB) )
   {
      RLOG0(L_ERROR, "transId doesnt exist ");
      nhLIMFreeCpjSduStaCfm(pst,cpjSduStaCfm);
      RETVALUE(RFAILED);
   }

   /* Drop Undesired/Duplicate Cfg Cfm */
   if (transCtrlBlk->numOfPdcpCfms <= 0)
   {
      RLOG0(L_DEBUG, " Duplicate Pdcp Cfg Cfm Rcvd ");
      RETVALUE(ROK);
   }
   transCtrlBlk->numOfPdcpCfms--;

   cellAndUeCb.ueCb = ueCb;
   cellAndUeCb.cellCb = cellCb;

   /*nh002.301:118385:HO issue: Removing this timer stop since it has 
    *not been started when the pdcp sdu sta req has been sent by rrc */

   /* Allocate memory for PDCP Status confirm structure */
   sMem.region = nhCb.init.region; 
   sMem.pool = nhCb.init.pool;

   NH_ALLOCEVNT((Ptr *)&nhuSduStaCfm, sizeof(NhuPdcpSduStaCfm), &sMem);
   if(NULLP == nhuSduStaCfm)
   {
      RLOG0(L_FATAL,"NhuPdcpSduStaCfm memory allocation failed.");
      nhLIMFreeCpjSduStaCfm(pst,cpjSduStaCfm);
      RETVALUE(RFAILED);
   }

   /* Fill the pdcpSduStaInfo in nhuSduStaCfm only if status is success, *
    * else fill the status as failed                                     */
   if(ROK == cpjSduStaCfm->status)
   {
      cfmStatus = NH_CFG_SUCCESS;

      /* Call the function to fill pdcpSduStaInfo structure in nhuSduStaCfm */
      retVal = nhLIMFillPdcpSduStaInfo(cpjSduStaCfm, nhuSduStaCfm);

      /* If the fillPdcpSduStaInfo returns fails,free the allocated memory of *
       * - pdcpSduStaInfo in nhuSduStaCfm.                                    *
       * - ulBitMap in pdcpSduStaInfo.                                        */
      if(ROK != retVal)
      {
         NH_FREEEVNT(nhuSduStaCfm);
         nhLIMFreeCpjSduStaCfm(pst,cpjSduStaCfm);
         RETVALUE(RFAILED);
      }
   }
   else if(RFAILED == cpjSduStaCfm->status)
   {
      cfmStatus = NH_CFG_FAILED;
   }
 
   nhuSduStaCfm->numRbId = cpjSduStaCfm->numRb;

   /* Reason will be set to reason given by the PDCP layer *
    * - zero in case of success from PDCP                  *
    * - non-zero in case of failure                        */
   nhuSduStaCfm->reason  = cpjSduStaCfm->reason;
   nhuSduStaCfm->status  = cpjSduStaCfm->status;

   /* Fill the NHU header information */ 
   nhuSduStaCfm->hdr.transId = cpjSduStaCfm->transId;
   nhuSduStaCfm->hdr.cellId  = cpjSduStaCfm->cellId;
   nhuSduStaCfm->hdr.ueId    = cpjSduStaCfm->ueId;

   /*nh002.301:118385:HO issue: Store nhuSduStaCfm to transaction control block*/
   transCtrlBlk->nhuSduStaCfm = nhuSduStaCfm;
   nhIdentity.cellId = cpjSduStaCfm->cellId;
   nhIdentity.ueId = cpjSduStaCfm->ueId;
   /* Call function to report Cfm to CGM */
   if (ROK != nhLIMRptCfgCfm(&nhIdentity, &cellAndUeCb, transCtrlBlk, 
            cfmStatus, NH_PDCP_CONFIGURED))
   {
      RLOG0(L_ERROR, "nhLIMRptCfgCfm failed ");
      RETVALUE(RFAILED);
   }
   /* Set Status of Confirm of PDCP */
   /* Call function to send PDCP SDU Status confirm to RRC user */

   /* Delete the transaction control block. No return value check as *
    * this function always returns ROK                               */

   /* Delete the ulBitMap for all DRBs in CpjSduStaCfmInfo structure *
    * and CpjSduStaCfmInfo structure                                 */
   nhLIMFreeCpjSduStaCfm(pst,cpjSduStaCfm);

   RETVALUE(ROK);
}  /* end of NhLiCpjSduStaCfm */


/**
 *
 *     @brief  This function is used to receiving Data Resume Confirm from
 *      PDCP layer
 *
 *     @details  This function is used to receive Data Resume  
 *               from PDCP layer.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower sap Id
 *     @param[in] cpjDatResumeCfm   
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 NhLiCpjDatResumeCfm
(
Pst          *pst,
SuId         suId,
CpjDatResumeCfmInfo *cpjDatResumeCfm
)
#else /* ANSI */
PUBLIC S16 NhLiCpjDatResumeCfm(pst, suId, cpjDatResumeCfm)
Pst          *pst;
SuId         suId;
CpjDatResumeCfmInfo *cpjDatResumeCfm;
#endif /* ANSI */
{
   NhLSapCb          *cpjSapCb = NULLP;   /* CPJ SAP CB */
   NhCellCb          *cellCb;
   NhActvUeCb        *ueCb; 
  // U8                cfmStatus = NH_CFG_SUCCESS;
   NhuPdcpDatResumeCfm  *nhuDatResumeCfm;
   S16               retVal;
   NhCellAndUeCb     cellAndUeCb;
   Mem               sMem;

   TRC3(NhLiCpjDatResumeCfm)

   UNUSED(pst);

   RLOG1(L_INFO, "NhLiCpjDatResumeCfm: suId(%d),", suId);

   if (NH_CHK_INPUT_PARAM(cpjDatResumeCfm))
   {
      RLOG0(L_FATAL, "GOT NULL cpjDatResumeCfm");
      RETVALUE(RFAILED);
   }

   retVal = ROK;

   cpjSapCb = nhUTLFindLlyrSapFrmSuId(STNHCPJSAP,suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (cpjSapCb == NULLP)
   {
      RLOG0(L_ERROR, " INVALID SAP ID in NhLiCpjDatResumeCfm");
      NH_FREE(pst->region, pst->pool, cpjDatResumeCfm, sizeof(CpjDatResumeCfmInfo));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (NULLP == (cellCb = nhDBMGetCellCb(cpjDatResumeCfm->cellId)))
   {
      RLOG0(L_ERROR, "NULL CELL CB ");
      NH_FREE(pst->region, pst->pool, cpjDatResumeCfm, sizeof(CpjDatResumeCfmInfo));
      RETVALUE(RFAILED); 
   }

   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, cpjDatResumeCfm->ueId)))
   {
      RLOG0(L_ERROR, "NULL UE CB ");
      NH_FREE(pst->region, pst->pool, cpjDatResumeCfm, sizeof(CpjDatResumeCfmInfo));
      RETVALUE(RFAILED); 
   }
   cellAndUeCb.ueCb = ueCb;
   cellAndUeCb.cellCb = cellCb;

   /* Allocate memory for PDCP Resume confirm structure */
   sMem.region = nhCb.init.region; 
   sMem.pool = nhCb.init.pool;

   NH_ALLOCEVNT((Ptr *)&nhuDatResumeCfm, sizeof(NhuPdcpDatResumeCfm), &sMem);
   if(NULLP == nhuDatResumeCfm)
   {
      RLOG0(L_FATAL,"NhuPdcpDatResumeCfm memory allocation failed.");
      NH_FREE(pst->region, pst->pool, cpjDatResumeCfm, sizeof(CpjDatResumeCfmInfo));
      RETVALUE(RFAILED);
   }
   /* Reason will be set to reason given by the PDCP layer *
    * - zero in case of success from PDCP                  *
    * - non-zero in case of failure                        */
   nhuDatResumeCfm->reason  = cpjDatResumeCfm->reason;
   nhuDatResumeCfm->status  = cpjDatResumeCfm->status;

   /* Fill the NHU header information */ 
   nhuDatResumeCfm->hdr.transId = cpjDatResumeCfm->transId;
   nhuDatResumeCfm->hdr.cellId  = cpjDatResumeCfm->cellId;
   nhuDatResumeCfm->hdr.ueId    = cpjDatResumeCfm->ueId;

   /* Set Status of Confirm of PDCP */
   /* Call function to send PDCP SDU Status confirm to RRC user */
   if (ROK != nhUIMSndPdcpDatResumeCfm(nhuDatResumeCfm, cellAndUeCb.ueCb))
   {
      RLOG0(L_ERROR, "nhUIMSndPdcpDatResumeCfm failed");
      RETVALUE(RFAILED);
   }
   /* Free CpjDatResumeCfmInfo structure */
   NH_FREE(pst->region, pst->pool, cpjDatResumeCfm, sizeof(CpjDatResumeCfmInfo));

   RETVALUE(ROK);
}  /* end of NhLiCpjSduStaCfm */

/**
 *
 *     @brief This function is used to receive the confirm 
 *            from RLC layer for Configuration Request.
 * 
 *     @details This function is used to receive the confirm 
 *            from RLC layer for Configuration Request.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower SAP ID
 *     @param[in] cfmInfo    Configuration confirm structure
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiCkwCfgCfm
(
Pst *pst,            
SuId suId,
CkwCfgCfmInfo *cfmInfo 
)  
#else /* ANSI */
PUBLIC S16 NhLiCkwCfgCfm(pst, suId, cfmInfo)
Pst *pst;             
SuId suId;
CkwCfgCfmInfo *cfmInfo; 
#endif /* ANSI */
{
   NhLSapCb *ckwSapCb = NULLP;

   TRC3(NhLiCkwCfgCfm)
  
   UNUSED(pst);

   RLOG1(L_DEBUG, "Received CfgCfm from RLC suId[%d]",suId);
   

   if (NH_CHK_INPUT_PARAM(cfmInfo))
   {
      RLOG0(L_FATAL, "GOT NULL cfmInfo");
      RETVALUE(RFAILED);
   }

   ckwSapCb = nhUTLFindLlyrSapFrmSuId(STNHCKWSAP, suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (ckwSapCb == NULLP)
   {
      RLOG0(L_ERROR, "SAP CHECK Failed in NhLiCkwCfgCfm");
      NH_FREE(pst->region, pst->pool, cfmInfo, sizeof(CkwCfgCfmInfo));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   /* Call LIM Module function */
   if (ROK != nhLIMHndlRlcCfgCfm(cfmInfo))
   {
      RLOG0(L_ERROR, "nhLIMHndlRlcCfgCfm failed");
      NH_FREE(pst->region, pst->pool, cfmInfo, sizeof(CkwCfgCfmInfo));
      RETVALUE(RFAILED);
   }

   NH_FREE(pst->region, pst->pool, cfmInfo, sizeof(CkwCfgCfmInfo));

   RETVALUE(ROK);
}  /* end of NhLiCkwCfgCfm */

/**
 *
 *     @brief This function is used to receive the UeId Change confirm 
 *            from RLC layer for UeId change Request.
 * 
 *     @details This function is used to receive the UeId change confirm 
 *            from RLC layer for UeId change Request.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower sap Id
 *     @param[in] transId    Transaction Id
 *     @param[in] ueInfo     Old UE Info
 *     @param[in] status     Status success/failure with reason 
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiCkwUeIdChgCfm
(
Pst       *pst,            
SuId       suId,
U32        transId,
CkwUeInfo *ueInfo, 
CmStatus   status
)  
#else /* ANSI */
PUBLIC S16 NhLiCkwUeIdChgCfm(pst, suId, transId, ueInfo, status)
Pst        *pst;             
SuId        suId;
U32         transId;
CkwUeInfo  *ueInfo;
CmStatus    status;
#endif /* ANSI */
{
   NhLSapCb *ckwSapCb = NULLP;
   NhUstaDgn dgn;
   NhId      nhIdentity;

   TRC3(NhLiCkwUeIdChgCfm)
  
   UNUSED(pst);

   if (NH_CHK_INPUT_PARAM(ueInfo))
   {
      RLOG0(L_FATAL, "GOT NULL ueInfo");
      RETVALUE(RFAILED);
   }

   RLOG2(L_INFO, "NhLiCkwUeIdChgCfm: \
                for UeId (%d) and status %d", ueInfo->ueId, status.status);

   ckwSapCb = nhUTLFindLlyrSapFrmSuId(STNHCKWSAP, suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (ckwSapCb == NULLP)
   {
      RLOG0(L_ERROR, "SAP CHECK Failed in NhLiCkwUeIdChgCfm ");
      NH_FREE(pst->region, pst->pool, ueInfo, sizeof(CkwUeInfo));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (transId == NH_RESERVED_TRANS_ID)
   {
      /* Raise a critical alarm to layer manager */
      if (status.status == CKW_CFG_CFM_NOK)
      {
         NH_SET_ZERO (&dgn, sizeof(dgn));
         dgn.type = LNH_USTA_DGNVAL_CELLUEID;
         dgn.u.cellUeId.ueId = ueInfo->ueId;
         dgn.u.cellUeId.cellId = ueInfo->cellId;
         nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LNH_EVENT_RLC_RVRT_FAIL,
                      LCM_CAUSE_UNKNOWN, &dgn);
      }
      NH_FREE(pst->region, pst->pool, ueInfo,sizeof(CkwUeInfo));
      RETVALUE(ROK);
   }

   nhIdentity.ueId  = ueInfo->ueId;
   nhIdentity.cellId = ueInfo->cellId;
   RLOG1(L_DEBUG, "Received UeIdChgCfm from RLC suId [%d]", suId);

   /* Call LIM Module function */
   if (ROK != nhLIMHndlUeIdChgCfm(&nhIdentity, transId, status, 
                                  NH_RLC_CONFIGURED))
   {
      RLOG0(L_ERROR, "nhLIMHndlUeIdChgCfm failed");
      NH_FREE(pst->region, pst->pool, ueInfo,sizeof(CkwUeInfo));
      RETVALUE(RFAILED);
   }
   NH_FREE(pst->region, pst->pool, ueInfo,sizeof(CkwUeInfo));

   RETVALUE(ROK);
}  /* end of NhLiCkwUeIdChgCfm */



/**
 *
 *     @brief This function is used to receive the confirm 
 *            from MAC layer for Configuration Request.
 *
 *     @details This function is used to receive the confirm 
 *            from MAC layer for Configuration Request.
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower SAP ID
 *     @param[in] transId    MAC transaction ID
 *     @param[in] status     Status of configuration request
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiCrgCfgCfm
(
Pst *pst,             
SuId suId,           
CrgCfgTransId transId,
U8 status            
)
#else /* ANSI */
PUBLIC S16 NhLiCrgCfgCfm(pst, suId, transId, status)
Pst *pst;           
SuId suId;         
CrgCfgTransId transId; 
U8 status;            
#endif /* ANSI */
{
   NhLSapCb             *crgSapCb = NULLP;

   TRC3(NhLiCrgCfgCfm)

   UNUSED(pst);

   RLOG2(L_DEBUG, "Received CfgCfm from MAC suId[%d] status[%d]", suId, status);

   crgSapCb = nhUTLFindLlyrSapFrmSuId(STNHCRGSAP, suId);

   RLOG1(L_DEBUG, " Received CfgCfm from MAC status [%d]", status);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (crgSapCb == NULLP)
   {
      RLOG0(L_ERROR, "SAP CHECK Failed in RrLiCmkCfgCfm");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   /* Call LIM Module function */
   if (ROK != nhLIMHndlMacCfgCfm(transId, status))
   {
      RLOG0(L_ERROR, "nhLIMHndlMacCfgCfm failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}   /* end of NhLiCrgCfgCfm */


/**
 *
 * @brief Handler to revert configuration of successful layers, if any 
 *        layer failed to configure.
 *
 * @details
 *
 *       This function is used to revert configuration 
 *            of successful layers, in case any layer failed  
 *            to configure.
 *            
 *     @param[in] nhIdentityentity     RRC ID (NhId)
 *     @param[in] ueCb           UE context
 *     @param[in] transCtrlBlk   Transaction context
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMHndlRvrtCfg
(
NhId *nhIdentity,   
NhActvUeCb *ueCb,
TransCb *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhLIMHndlRvrtCfg(nhIdentity, ueCb, transCtrlBlk)
NhId *nhIdentity;    
NhActvUeCb *ueCb;
TransCb *transCtrlBlk; 
#endif /* ANSI */
{
   NhUstaDgn   dgn;
   S16         macRet = 0;
   S16         rlcRet = 0;
   S16         pdcpRet = 0;
#ifdef RRC_PHY_CFG_SUPP
   S16         phyRet = 0;
#endif /* RRC_PHY_CFG_SUPP */

   TRC2(nhLIMHndlRvrtCfg)

   RLOG0(L_DEBUG, "nhLIMHndlRvrtCfg ");

#ifdef RRC_PHY_CFG_SUPP
   if (transCtrlBlk->cfgMask & NH_PHY_CONFIGURED)
   {
      phyRet = nhLIMHndlRvrtPhyCfg(nhIdentity, ueCb, transCtrlBlk);
   }
#endif /* RRC_PHY_CFG_SUPP */

   if (transCtrlBlk->cfgMask & NH_MAC_CONFIGURED)
   {
      macRet = nhLIMHndlRvrtMacCfg(nhIdentity, ueCb, transCtrlBlk);
   }

   if (transCtrlBlk->cfgMask & NH_RLC_CONFIGURED)
   {
      rlcRet = nhPIMRvrtRlcCfg(ueCb, transCtrlBlk);
   }

   if (transCtrlBlk->cfgMask & NH_PDCP_CONFIGURED)
   {
      pdcpRet = nhPIMRvrtPdcpCfg(ueCb, transCtrlBlk);
   }

#ifdef RRC_PHY_CFG_SUPP
   if((ROK != macRet) || (ROK != rlcRet) ||  (ROK != pdcpRet) 
      || (ROK != phyRet))
#else /* RRC_PHY_CFG_SUPP */
   if((ROK != macRet) || (ROK != rlcRet) ||  (ROK != pdcpRet))
#endif /* RRC_PHY_CFG_SUPP */
   {
      /* Raise a critical alarm to layer manager */
      NH_SET_ZERO (&dgn, sizeof(dgn));
      dgn.type = LNH_USTA_DGNVAL_MEM;
      dgn.u.mem.region = NH_REG;
      dgn.u.mem.pool = NH_POOL;
      nhSendLmAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_EVT,
                    LCM_CAUSE_UNKNOWN, &dgn);
   }

   RETVALUE(ROK);
} /* end of nhLIMHndlRvrtCfg */

/**
 * @brief Handler to raise an alarm at LM and notify RRC user of decode error.
 * @details
 *
 *
 *      This function is used to handle status indication
 *             from RLC layer.
 *            
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMDecFailNotify
(
NhuDatIndSdus   *datIndSdu,
NhId            *nhIdentity
)
#else /* ANSI */
PRIVATE S16 nhLIMDecFailNotify(datIndSdu, nhIdentity)
NhuDatIndSdus   *datIndSdu;
NhId            *nhIdentity;
#endif /* ANSI */
{

   NhUstaDgn       dgn;                    /* Diagnostics variable */
   NhuErrIndSdus   errIndSdus;
   NhuHdr          sduHdr;

   /* nh004.201 - Correction to TRC functionality. */
   TRC2(nhLIMDecFailNotify)

   RLOG0(L_ERROR, "Decoding failed.");

   /*  Generate Alarm to layer manager.  */
   NH_SET_ZERO (&dgn, sizeof(dgn));
   dgn.type = LNH_USTA_DGNVAL_DATA;
   dgn.u.mem.region = nhCb.init.region;
   dgn.u.mem.pool = nhCb.init.pool;

   nhSendLmAlarm(LCM_CATEGORY_PROTOCOL, LCM_EVENT_UPDMSG_ERR,
         LCM_CAUSE_DECODE_ERR, &dgn);
   /* Generate error indication so that the user also knows about
      this event */
   NH_SET_ZERO((U8*)&sduHdr, sizeof(NhuHdr));
   NH_FORM_NHU_HDR(sduHdr, (*nhIdentity));
   NH_FILL_NHU_ERROR(errIndSdus.sdu, NHU_ERRTYPE_DEC_FAILED,
         NHU_ERRCAUSE_NONE);
   NH_SEND_ERROR_IND(errIndSdus, sduHdr);

   NH_FREEEVNT(datIndSdu);
   RETVALUE(RFAILED);

}/* End of nhLIMDecFailNotify */

/**
 * @brief Handler to handle status indication from RLC layer.
 * @details
 *
 *
 *      This function is used to handle status indication
 *             from RLC layer.
 *            
 *     @param[in] nhIdentity    RRC ID (NhId)
 *     @param[in] staInd        Status indication
 *     @param[in] buf           Buffer containing the message
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlStatusInd
(
NhId *nhIdentity,
PjuStaIndInfo *staInd,
Buffer *buf
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlStatusInd(nhIdentity, staInd, buf)
NhId *nhIdentity;
PjuStaIndInfo *staInd; 
Buffer *buf;
#endif /* ANSI */
{
   NhActvUeCb     *ueCb;
   NhCellCb       *cellCb = NULLP;
   U8             nhuCause = NHU_ERRCAUSE_NONE;
   NhuDatIndSdus   *datIndSdu = NULLP;     /* RRU Data Indication structure -
                                           used only incase of Security Mode 
                                           Failure message*/
   Void            *pdu       = NULLP;
   U8              nhMsgType = NH_ZERO_VAL; /* Message Type */
   U8              nhEvnt = NH_ZERO_VAL;   /* Event */
   NhuUL_DCCH_Msg  *ulDcchPdu = NULLP;     /* UL DCCH structure */
   U8              rrcTransId = NH_ZERO_VAL; /* RRC Transaction ID */
   Mem             sMem;
   NhCellAndUeCb   cellandUeCb;


   TRC2(nhLIMHndlStatusInd)
   RLOG0(L_DEBUG, "nhLIMHndlStatusInd ");

   sMem.region = nhCb.init.region;
   sMem.pool = nhCb.init.pool;

   cellCb = nhDBMGetCellCb(nhIdentity->cellId);
   if (NULLP == cellCb)
   {
      RLOG0(L_ERROR, " NULL cellCb ");
      RETVALUE(RFAILED);
   }

   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, nhIdentity->ueId)))
   {
      RLOG0(L_ERROR, "NULL UE CB ");
      RETVALUE(RFAILED);
   }

   /* Map PDCP cause to NHU Cause */
   NH_MAP_PDCP_DAT_STA_IND_CAUSE(staInd->cause, nhuCause);

   /* Update General Statistics */
   nhUTLUpdGenSts(NH_STS_CAUSE_PDCP_ERR);

   /* If it is Inegrity Failure and Status Indication for Security Mode
      Fauilure Message, then call CCM SecModeFailure handler to send
      DatInd to upper layer
   */
   if ((staInd->cause == PJU_INTVER_FAILED) && (buf != NULLP))
   {
      NH_ALLOCEVNT((Ptr *)&datIndSdu, sizeof(NhuDatIndSdus), &sMem);
      if (NULLP == datIndSdu)
      {
         RLOG0(L_FATAL,"cmAllocEvnt failed ");
         RETVALUE(RFAILED);
      }

      datIndSdu->hdr.transId = NH_ZERO_VAL;
      datIndSdu->err.errPres = FALSE;

      datIndSdu->hdr.ueId = nhIdentity->ueId;
      datIndSdu->hdr.cellId = nhIdentity->cellId;
      datIndSdu->sdu.msgCategory = NHU_MSG_ULDCCH;
      pdu = (Void *)(&(datIndSdu->sdu.m.ulDcchMsg.dcchMsg));

      /* Call EDM module function to decode the message */
      if (ROK != nhEDMDecReq(datIndSdu->sdu.msgCategory, buf, (Void *)datIndSdu,
                     pdu))
      {
         RETVALUE(nhLIMDecFailNotify(datIndSdu, nhIdentity));
      } /* end of decode */

      ulDcchPdu = (NhuUL_DCCH_Msg*)pdu;
      nhUTLGetMsgTypeAndEvntFrmSdu (ulDcchPdu->message.val.c1.choice.val, 
                    NH_SDU_UL_DCCH, &nhMsgType, &nhEvnt);
      RLOG1(L_DEBUG,"UE MSG RCVD: event is [%d]", nhEvnt);
      if(nhMsgType == NH_MSGTYPE_SECMODFAIL)
      {
         nhUTLGetRrcTransId((Void*)ulDcchPdu, 
                           ulDcchPdu->message.val.c1.choice.val, 
                           NH_SDU_UL_DCCH, &rrcTransId);
         if (ROK != nhUTLValidateSRBforMsgRx(ueCb, nhIdentity->rbId,
                     ulDcchPdu->message.val.c1.choice.val))
         {
            datIndSdu->err.errPres = TRUE;
            datIndSdu->err.errType = NHU_ERRTYPE_PDCP_DAT_IND;
            datIndSdu->err.errCause = NHU_ERRCAUSE_WRONG_RB_ID;
            /* no need to go to state handler, Pass it to UIM */
            if (ROK != nhUIMDatInd(datIndSdu))
            {
               RLOG0(L_ERROR, "Send to UIM Failed ");
               NH_FREEEVNT(datIndSdu);
               RETVALUE(RFAILED);
            }

            RETVALUE(RFAILED);
         }

         cellandUeCb.cellCb = cellCb;
         cellandUeCb.ueCb = ueCb;

         /* Process the message in lower state machine */
         if (ROK != nhLISMTbl[nhEvnt](datIndSdu, &cellandUeCb, 
                                      rrcTransId, nhMsgType))
         {
            RLOG0(L_ERROR,"nhCCMSecModFailHndlr failed ");
            /* Free the memmory allocated by nhAllocEvnt above */
            NH_FREEEVNT(datIndSdu);
            RETVALUE(RFAILED);
         }
         RETVALUE(ROK);
      }

      NH_FREEEVNT(datIndSdu);
   }

   /* Send Status Indication to UIM */
   NH_SEND_STATUS_IND(*nhIdentity, nhuCause);

   RETVALUE(ROK);
}  /* end of nhLIMHndlStatusInd */

/**
 *
 * @brief Handler to process message from RLC layer at the RLU interface.

 * @details
 *
 *       This function is used to process message from
 *            RLC layer at the RLU interface.
 *            
 *     @param[in] nhIdentity     RRC ID (NhId)
 *     @param[in] mBuf           Message buffer
 *     @param[in] tCrnti         Temporary C-RNTI
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMHndlDatInd
(
NhId   *nhIdentity,    
Buffer *mBuf,   
CmLteRnti tCrnti
)
#else /* ANSI */
PUBLIC S16 nhLIMHndlDatInd(nhIdentity, mBuf, tCrnti)
NhId   *nhIdentity;   
Buffer *mBuf;  
CmLteRnti tCrnti; 
#endif /* ANSI */
{
   Void            *pdu       = NULLP;
   NhuDatIndSdus   *datIndSdu = NULLP;  /* RRU Data Indication structure */
   NhuUL_CCCH_Msg  *ulCcchPdu = NULLP;  /* UL CCCH structure */
   NhuUL_DCCH_Msg  *ulDcchPdu = NULLP;  /* UL DCCH structure */
   NhActvUeCb      *ueCb      = NULLP;  /* UE control block */
   U8              nhEvnt = NH_ZERO_VAL;              /* Event */
   /* RRC transaction identifier within UE msg */
   U8              rrcTransId = NH_ZERO_VAL;
   NhCellCb        *cellCb = NULLP;
   Mem             sMem;
   U8              nhMsgType = NH_ZERO_VAL;
   NhCellAndUeCb   cellandUeCb;

   TRC2(nhLIMHndlDatInd)

   RLOG0(L_DEBUG, "nhLIMHndlDatInd ");

   sMem.region = nhCb.init.region; 
   sMem.pool = nhCb.init.pool;

   NH_ALLOCEVNT((Ptr *)&datIndSdu, sizeof(NhuDatIndSdus), &sMem);
   if (NULLP == datIndSdu)
   {
      RLOG0(L_FATAL, "cmAllocEvnt failed ");
      RETVALUE(RFAILED);
   }

   datIndSdu->hdr.transId = NH_ZERO_VAL; 
   datIndSdu->err.errPres = FALSE;

   cellCb = nhDBMGetCellCb(nhIdentity->cellId);
   if (NULLP == cellCb)
   {
      RLOG0(L_ERROR, "nhLIMHndlDatInd: NULL cellCb ");
      NH_FREEEVNT(datIndSdu);
      RETVALUE(RFAILED);
   }

   RLOG_ARG1(L_DEBUG, DBG_CELLID, nhIdentity->cellId,"ueId[%d]",nhIdentity->ueId);

   if (TRUE == nhIdentity->isPdcpId)
   {
      /* Find out the right UE control block */
      ueCb = nhDBMGetUeCb(cellCb, nhIdentity->ueId);
      if (NULLP == ueCb)
      {
         RLOG0(L_ERROR, "NULL ueCb ");
         NH_FREEEVNT(datIndSdu);
         RETVALUE(RFAILED);
      }
      datIndSdu->hdr.ueId = nhIdentity->ueId;
      datIndSdu->hdr.cellId = nhIdentity->cellId;
      datIndSdu->sdu.msgCategory = NHU_MSG_ULDCCH;
      datIndSdu->sdu.m.ulDcchMsg.pdcpId = nhIdentity->rbId;

      pdu = (Void *)(&(datIndSdu->sdu.m.ulDcchMsg.dcchMsg));
   }
   else
   {
      datIndSdu->hdr.cellId = nhIdentity->cellId;
      datIndSdu->sdu.msgCategory = NHU_MSG_ULCCCH;
      datIndSdu->sdu.m.ulCcchMsg.tmpCrnti = tCrnti;
      datIndSdu->sdu.m.ulCcchMsg.rbId = nhIdentity->rbId;

      pdu = (Void *)(&(datIndSdu->sdu.m.ulCcchMsg.ccchMsg));
   }

   /* Call EDM module function to decode the message */
   if (ROK != nhEDMDecReq(datIndSdu->sdu.msgCategory, 
                          mBuf, (Void *)datIndSdu, pdu))
   {
      RETVALUE(nhLIMDecFailNotify(datIndSdu, nhIdentity));
   } /* end of decode */

   /* Validate SRB ID for DCCH messages */
   if (datIndSdu->sdu.msgCategory == NHU_MSG_ULDCCH)
   {
      ulDcchPdu = (NhuUL_DCCH_Msg*)pdu;
      if (ROK != nhUTLValidateSRBforMsgRx(ueCb, nhIdentity->rbId, 
                                  ulDcchPdu->message.val.c1.choice.val))
      {
         datIndSdu->err.errPres = TRUE;
         datIndSdu->err.errType = NHU_ERRTYPE_PDCP_DAT_IND;
         datIndSdu->err.errCause = NHU_ERRCAUSE_WRONG_RB_ID;
         /* no need to go to state handler, Pass it to UIM */
         if (ROK != nhUIMDatInd(datIndSdu))
         {
            RLOG0(L_ERROR,"Send to UIM Failed ");
            NH_FREEEVNT(datIndSdu);
            RETVALUE(RFAILED);
         }
         RETVALUE(RFAILED); 
      }
      nhUTLGetMsgTypeAndEvntFrmSdu (ulDcchPdu->message.val.c1.choice.val, 
                        NH_SDU_UL_DCCH, &nhMsgType, &nhEvnt);
      RLOG1(L_DEBUG, "UE MSG RCVD: event is [%d] ", nhEvnt);
      nhUTLGetRrcTransId((Void*)ulDcchPdu,
                        ulDcchPdu->message.val.c1.choice.val, 
                        NH_SDU_UL_DCCH, &rrcTransId);
   } /* if (datIndSdu->sdu.msgCategory == NHU_MSG_ULDCCH) */

   /* Message is received on CCCH */
   if (NHU_MSG_ULCCCH == datIndSdu->sdu.msgCategory)
   {
      ulCcchPdu = (NhuUL_CCCH_Msg*)pdu;
     
      /* For CCCH message from UE does no contain transId */
      rrcTransId = 0;
      nhUTLGetMsgTypeAndEvntFrmSdu (ulCcchPdu->message.val.c1.choice.val, 
                                    NH_SDU_UL_CCCH, &nhMsgType, &nhEvnt);
   }  /* end of if (NHU_MSG_ULCCCH == datIndSdu->sdu.msgCategory) */

   cellandUeCb.cellCb = cellCb;
   cellandUeCb.ueCb = ueCb;

   /* Process the message in lower state machine */
   if (ROK != nhLISMTbl[nhEvnt](datIndSdu, &cellandUeCb, rrcTransId, nhMsgType))
   {
      RLOG0(L_ERROR, "Lower state Machine Function Failed ");

      /* Free the memmory allocated by nhAllocEvnt above */
      NH_FREEEVNT(datIndSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}   /* nhLIMHndlDatInd */

/**
 *
 * @brief Handler to handle cell configuration confirm from the RLC layer.
 *
 * @details
 *
 *      This function is used to handle cell configuration confirm
 *            from the RLC layer.
 *            
 *     @param[in] cfmInfo   RLC configuration confirm info 
 *     @param[in] cellCb    Cell context
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlRlcCellCfgCfm
(
CkwCfgCfmInfo *cfmInfo,  
NhCellCb    *cellCb
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlRlcCellCfgCfm(cfmInfo, cellCb)
CkwCfgCfmInfo *cfmInfo; 
NhCellCb    *cellCb;
#endif /* ANSI */
{
   NhCellTransCb   *cellTransCb = NULLP;
   U8              count;
   U8              cgmCfmStatus = NH_CFG_SUCCESS;
   U8              entityIndex;
   U8              rbId;
   U16              status;

   TRC2(nhLIMHndlRlcCellCfgCfm)

   RLOG0(L_DEBUG, "nhLIMHndlRlcCellCfgCfm ");

   cellTransCb = &(cellCb->cellTransCb);

   if(cellTransCb->cfgAction != NH_CFG_ACTN_RLS)
   {
      for (entityIndex = 0; entityIndex < cfmInfo->numEnt; entityIndex++)
      {
         rbId = cfmInfo->entCfgCfm[entityIndex].rbId;
         status = cfmInfo->entCfgCfm[entityIndex].status.status;

         for (count = 0; count < NH_MAX_RB_PER_CELL_CFG; count++)
         {
            if (NH_STATUS_USED == (cellTransCb->rlcMapTbl[count].status)  && 
                                  (cellTransCb->rlcMapTbl[count].rbId == rbId)) 
            {
               cellTransCb->numOfRlcCfms--;
               if (CKW_CFG_CFM_OK == status)
                  cellTransCb->rlcMapTbl[count].cfmStatus = NH_CFG_SUCCESS;
               else
               {
                  cellTransCb->rlcMapTbl[count].cfmStatus = NH_CFG_FAILED;
                  RLOG1(L_DEBUG, "Received CellCfgCfm from RLC Status = Failed,"
                     " RAB-ID[%d]", rbId);
               }
               break;
            }
         } /* end for (count = 0; count < MAX_CELL_RB_CFG; count++) */
      }
   }
   else
   {
      status = cfmInfo->entCfgCfm[0].status.status;
      RLOG2(L_DEBUG, "status[%d] numOfRlcCfms[%d]",
         status, cellTransCb->numOfRlcCfms);
      if (NH_STATUS_USED == (cellTransCb->rlcMapTbl[0].status)  && 
         (NH_ZERO_VAL == cellTransCb->rlcMapTbl[0].rbId)) 
      {
         cellTransCb->numOfRlcCfms--;
         if (CKW_CFG_CFM_OK == status)
            cellTransCb->rlcMapTbl[0].cfmStatus = NH_CFG_SUCCESS;
         else
         {
            RLOG1(L_DEBUG, "Receivedd CellCfgCfm from RLC, Releasing of RAB "
               "failed  RAB-ID[%d]", cellTransCb->rlcMapTbl[0].rbId);
            cellTransCb->rlcMapTbl[0].cfmStatus = NH_CFG_FAILED;
      }
      }
      
   }
   /* If all cfms rcvd then set cfmStatus for layer */
   if (cellTransCb->numOfRlcCfms == 0)
   {
      for (count = 0; count < NH_MAX_RB_PER_CELL_CFG; count++)
   {
         if ((NH_STATUS_USED == cellTransCb->rlcMapTbl[count].status) &&
                  (cellTransCb->rlcMapTbl[count].cfmStatus == NH_CFG_FAILED))
      {
            cgmCfmStatus = NH_CFG_FAILED;
            break;
   }
      } /* end for (count = 0; count < MAX_CELL_RB_CFG; count++) */

      /* Now call function to send Cfm Report to CGM */
      if (ROK != nhLIMRptCellCfgCfm(cellCb, cgmCfmStatus, NH_RLC_CONFIGURED))
      {
         RLOG0(L_ERROR, "nhLIMRptCellCfgCfm failed");
         RETVALUE(RFAILED);
      }
   }  /* if (!transCtrlBlk->numOfRlcCfms) */
   RETVALUE(ROK);
}/* End of nhLIMHndlRlcCellCfgCfm */

/**
 *
 * @brief Handler to handle UE configuration confirm from RLC layer.
 *
 * @details
 *
 *       This function is used to handle config confirm
 *            from RLC layer.
 *
 *     @param[in] cfmInfo   Configuration confirm info 
 *     @param[in] cellCb    Cell context
 *            
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlRlcUECfgCfm
(
CkwCfgCfmInfo *cfmInfo,  
NhCellCb    *cellCb
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlRlcUECfgCfm(cfmInfo, cellCb)
CkwCfgCfmInfo *cfmInfo; 
NhCellCb    *cellCb;
#endif /* ANSI */
{
   NhActvUeCb      *ueCb = NULLP;
   TransCb         *transCtrlBlk = NULLP;
   NhId            nhIdentity;
   NhCellAndUeCb   cellAndUeCb;

   TRC2(nhLIMHndlRlcUECfgCfm)

   RLOG0(L_DEBUG, "nhLIMHndlRlcUECfgCfm ");

   nhIdentity.ueId = cfmInfo->ueId;
   nhIdentity.cellId = cfmInfo->cellId;

   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, cfmInfo->ueId)))
   {
      /* If Ue Cb is NULLP then try to get the uecb from cellCb->newUeHlCp 
       * since we might have sent UeIdChgReq to RLC & PDCP */
      if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetNewUeCb(cellCb, cfmInfo->ueId)))
      {
         RLOG0(L_ERROR, "NULL UE CB ");
         RETVALUE(RFAILED); 
      }
   }
    /* Find trans node in list with specified transId value */
   if ((TransCb*)NULLP == (transCtrlBlk = nhUTLGetTransCbFrmNxTransId (ueCb, 
                                     cfmInfo->transId)))
   {
      RLOG0(L_ERROR, "transId doesnt exist ");
      RETVALUE(RFAILED);
   }

   /* Drop Undesired/Duplicate Cfg Cfm */
   if (transCtrlBlk->numOfRlcCfms <= 0)
   {
      RLOG0(L_ERROR, "Invalid Rlc Cfg Cfm Rcvd ");
      RETVALUE(ROK);
   }

   cellAndUeCb.ueCb = ueCb;
   cellAndUeCb.cellCb = cellCb;

   if (ROK != nhLIMVldtAndRptCfgCfm ((Void *)cfmInfo, transCtrlBlk,
                        &nhIdentity, &cellAndUeCb, NH_RLC_CONFIGURED))
   {
      RLOG0(L_ERROR, "nhLIMVldtAndRptCfgCfm failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* End of nhLIMHndlRlcUECfgCfm */

/**
 *
 * @brief Handler to handle Re-establish confirm of SRB1 from PDCP layer.
 *
 * @details
 *
 *       This function is used to handle Re-establish confirm
 *            from PDCP layer.
 *            
 *     @param[in] nhIdentity  RRC ID(NhId) containing cell ID and UE ID
 *     @param[in] transId     Transaction context
 *     @param[in] statusCfm   Configuration status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlRestbCfgCfm
(
NhId    *nhIdentity,
U32     transId,
Status  statusCfm
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlRestbCfgCfm(nhIdentity, transId, statusCfm)
NhId    *nhIdentity;
U32     transId;
Status  statusCfm;
#endif /* ANSI */
{
   NhCellCb        *cellCb    = NULLP;
   NhActvUeCb      *ueCb      = NULLP;
   TransCb         *transCtrlBlk   = NULLP;
   U16              cfmStatus = ROK;
   U8               cgmCfmStatus = NH_CFG_SUCCESS;
   NhSrb1RestbInfo *restTbl = NULLP;
   NhCellAndUeCb    cellAndUeCb;

   TRC2(nhLIMHndlRestbCfgCfm)

   RLOG_ARG2(L_DEBUG, DBG_CELLID, nhIdentity->cellId, "transId[%d] ueId[%d]",
      transId, nhIdentity->ueId);

   if (NULLP == (cellCb = nhDBMGetCellCb(nhIdentity->cellId)))
   {
      RLOG0(L_ERROR, "nhPIMRlcPdcpMacRlsCfg failed.");
      RETVALUE(RFAILED);
   }

   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, nhIdentity->ueId)))
   {
      if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetNewUeCb(cellCb, 
                                        nhIdentity->ueId)))
      {
        RLOG0(L_ERROR, "nhLIMSendPhyRlsCfg failed.");
        RETVALUE(RFAILED);
      }  
   }


    /* Find trans node in list with specified transId value */
   if ((TransCb*)NULLP == 
         (transCtrlBlk = nhUTLGetTransCbFrmNxTransId (ueCb, transId)))
   {
      RLOG0(L_ERROR, "nhLIMHndlRestbCfgCfm:transId doesnt exist");
      RETVALUE(RFAILED);
   }
   cfmStatus = statusCfm;
   
   restTbl  = &(transCtrlBlk->restbTbl[NH_RESTB_MAPTBL_IDX_PDCP_SRB1]);      

   /* Drop Undesired/Duplicate Cfg Cfm */
   if (transCtrlBlk->numOfPdcpCfms <= 0)
   {
      RLOG0(L_INFO, "Duplicate Pdcp Cfg Cfm Rcvd ");
      RETVALUE(ROK);
   }

   if (restTbl->status == NH_STATUS_USED)
   {
      if (cfmStatus != CPJ_CFG_CFM_OK) 
      {
         restTbl->cfmStatus =  CPJ_CFG_CFM_NOK;
         /* Update General Statistics for lower layer failure */
         nhUTLUpdGenSts(NH_STS_CAUSE_LOWER_LYR_FAIL);
      }
      else
      {
         restTbl->cfmStatus = CPJ_CFG_CFM_OK;
      }
   }
   transCtrlBlk->numOfPdcpCfms--;

   /* If all cfms rcvd then set cfmStatus for layer */
   if (transCtrlBlk->numOfPdcpCfms <= 0)
   {
     if(((restTbl->status == NH_STATUS_USED) &&
       (restTbl->cfmStatus == CPJ_CFG_CFM_NOK))
      ||
       ((transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PDCP].status == 
                                NH_STATUS_USED) &&
        (transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PDCP].cfmStatus != 
                                CPJ_CFG_CFM_OK)))
      {
         cgmCfmStatus = NH_CFG_FAILED;
      }
      cellAndUeCb.ueCb = ueCb;
      cellAndUeCb.cellCb = cellCb;

       /* Call function to report Cfm to CGM */
      if (ROK != nhLIMRptCfgCfm(nhIdentity, &cellAndUeCb, transCtrlBlk, 
                        cgmCfmStatus, NH_PDCP_CONFIGURED))
      {
         RLOG0(L_ERROR, "nhLIMRptCfgCfm failed ");
         RETVALUE(RFAILED);
      }
   }  /* if (!transCtrlBlk->numOfPdcpCfms) */
   
   RETVALUE(ROK);
} /* end of nhLIMHndlRestbCfgCfm */

/**
 *
 * @brief Handler to handle UeId config confirm from RLC,PDCP and PHY layer.
 *
 * @details
 *
 *       This function is used to handle Ue config confirm
 *            from RLC layer when RLC Layer is Configured.
 *            
 *     @param[in] transCtrlBlk     Transaction Control Block
 *     @param[in] cellCb      Cell Control Block
 *     @param[in] ueCb        UE Control Block
 *     @param[in] cfmStatus   Confirmation Status 
 *     @param[in] nhIdentity  RRC ID(NhId) containing cell ID and UE ID
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlUeIdChgCfmRlcCfg
(
TransCb         *transCtrlBlk,
NhCellCb        *cellCb,
NhActvUeCb      *ueCb,
U16             cfmStatus,
NhId    *nhIdentity
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlUeIdChgCfmRlcCfg(transCtrlBlk,cellCb,ueCb,
                                     cfmStatus,nhIdentity)
TransCb         *transCtrlBlk;
NhCellCb        *cellCb;
NhActvUeCb      *ueCb;
U16             cfmStatus;
NhId            *nhIdentity;
#endif /* ANSI */
{
   U8                 cgmCfmStatus = NH_CFG_SUCCESS;
   NhUeCfgCfmMapInfo  *ueMapTbl = NULLP;
   NhCellAndUeCb      cellAndUeCb;


   ueMapTbl = &(transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_RLC]);

   /* Drop Undesired/Duplicate Cfg Cfm */
   if (transCtrlBlk->numOfRlcCfms <= 0)
   {
      RLOG0(L_ERROR, "Invalid Rlc Cfg Cfm Rcvd ");
      RETVALUE(ROK);
   }
   if (ueMapTbl->status == NH_STATUS_USED) 
   {
      if (cfmStatus == CKW_CFG_CFM_NOK) 
      {
         ueMapTbl->cfmStatus = CKW_CFG_CFM_NOK;
         /* Update General Statistics for lower layer failure */
         nhUTLUpdGenSts(NH_STS_CAUSE_LOWER_LYR_FAIL);
      }
      else
      {
         ueMapTbl->cfmStatus = CKW_CFG_CFM_OK;
      }
   }
   transCtrlBlk->numOfRlcCfms--;
   if(transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RESTB_SRB1)
   {
     /* As initiated the configuration request 
     no need to send the confirmation to the CGM*/
     nhPIMRlcSndRestbCfg(transCtrlBlk, ueCb);
     RETVALUE(ROK);
   }

   /* If all cfms rcvd then set cfmStatus for layer */
   if (transCtrlBlk->numOfRlcCfms <= 0)
   {
       if (((transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_MOD] != 
            transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_MOD]) ||
       (transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_RLS] != 
            transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_RLS]) ||
       (transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_ADD] != 
            transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_ADD]) ||
       (transCtrlBlk->rlcMapTbl.srbCfmSta[NH_RB_TO_REST] != 
            transCtrlBlk->rlcMapTbl.srbToCfg[NH_RB_TO_REST]) ||
       /* Check for Signalling Radio bearer failures */
       (transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_MOD] != 
            transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_MOD]) ||
       (transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_RLS] != 
            transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_RLS]) ||
       (transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_ADD] != 
            transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_ADD]) ||
       (transCtrlBlk->rlcMapTbl.drbCfmSta[NH_RB_TO_REST] != 
            transCtrlBlk->rlcMapTbl.drbToCfg[NH_RB_TO_REST]))
      ||
      ((ueMapTbl->status    == NH_STATUS_USED) &&
          (ueMapTbl->cfmStatus == CKW_CFG_CFM_NOK)))
      {
         cgmCfmStatus = NH_CFG_FAILED;
      }

      cellAndUeCb.ueCb = ueCb;
      cellAndUeCb.cellCb = cellCb;

       /* Call function to report Cfm to CGM */
      if (ROK != 
         nhLIMRptCfgCfm(nhIdentity , &cellAndUeCb, transCtrlBlk, 
                        cgmCfmStatus, NH_RLC_CONFIGURED))
      {
         RLOG0(L_ERROR, "nhLIMRptCfgCfm failed ");
         RETVALUE(RFAILED);
      }
   }  /* if (!transCtrlBlk->numOfRlcCfms) */
   RETVALUE(ROK);

}/* End of nhLIMHndlUeIdChgCfmRlcCfg */

/**
 *
 * @brief Handler to handle UeId config confirm from RLC,PDCP and PHY layer.
 *
 * @details
 *
 *       This function is used to handle Ue config confirm
 *       when PDCP Layer is Configured.
 *            
 *     @param[in] transCtrlBlk     Transaction Control Block
 *     @param[in] cellCb      Cell Control Block
 *     @param[in] ueCb        UE Control Block
 *     @param[in] cfmStatus   Confirmation Status 
 *     @param[in] nhIdentity  RRC ID(NhId) containing cell ID and UE ID
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlUeIdChgCfmPdcpCfg
(
TransCb         *transCtrlBlk,
NhCellCb        *cellCb,
NhActvUeCb      *ueCb,
U16             cfmStatus,
NhId    *nhIdentity
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlUeIdChgCfmPdcpCfg(transCtrlBlk,cellCb,ueCb,
                                       cfmStatus,nhIdentity)
TransCb         *transCtrlBlk;
NhCellCb        *cellCb;
NhActvUeCb      *ueCb;
U16             cfmStatus;
NhId            *nhIdentity;
#endif /* ANSI */
{
   U8                 cgmCfmStatus = NH_CFG_SUCCESS;
   NhUeCfgCfmMapInfo  *ueMapTbl = NULLP;
   NhCellAndUeCb      cellAndUeCb;

   ueMapTbl = &(transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PDCP]);

   /* Drop Undesired/Duplicate Cfg Cfm */
   if (transCtrlBlk->numOfPdcpCfms <= 0)
   {
      RLOG0(L_ERROR, "Duplicate Pdcp Cfg Cfm Rcvd ");
      RETVALUE(ROK);
   }
   if (ueMapTbl->status == NH_STATUS_USED) 
   {
      if (cfmStatus != CPJ_CFG_CFM_OK) 
      {
         ueMapTbl->cfmStatus = CPJ_CFG_CFM_NOK;
         /* Update General Statistics for lower layer failure */
         nhUTLUpdGenSts(NH_STS_CAUSE_LOWER_LYR_FAIL);
      }
      else
      {
         ueMapTbl->cfmStatus = CPJ_CFG_CFM_OK;
      }
   }
   transCtrlBlk->numOfPdcpCfms--;

   /* If all cfms rcvd then set cfmStatus for layer */
   if (transCtrlBlk->numOfPdcpCfms <= 0)
   {
      if (((ueMapTbl->status    == NH_STATUS_USED) &&
          (ueMapTbl->cfmStatus == CPJ_CFG_CFM_NOK))
      /*Check for RB Configuration failure*/ 
      || ((transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_MOD] !=
            transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_MOD]) ||
       (transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_RLS] !=
            transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_RLS]) ||
       (transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_ADD] !=
            transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_ADD]) ||
       (transCtrlBlk->pdcpMapTbl.srbCfmSta[NH_RB_TO_REST] !=
            transCtrlBlk->pdcpMapTbl.srbToCfg[NH_RB_TO_REST]) ||
       /* Check for data Radio bearer failures */
       (transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_MOD] !=
            transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_MOD]) ||
       (transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_RLS] !=
            transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_RLS]) ||
       (transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_ADD] !=
            transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_ADD]) ||
       (transCtrlBlk->pdcpMapTbl.drbCfmSta[NH_RB_TO_REST] !=
            transCtrlBlk->pdcpMapTbl.drbToCfg[NH_RB_TO_REST]))
      /*Connection re-establishment primitive failure*/
      ||
      ((transCtrlBlk->restbTbl[NH_RESTB_MAPTBL_IDX_PDCP_SRB1].status == 
                                                NH_STATUS_USED) &&
      transCtrlBlk->restbTbl[NH_RESTB_MAPTBL_IDX_PDCP_SRB1].cfmStatus == 
                                                CPJ_CFG_CFM_NOK))
      {
          cgmCfmStatus = NH_CFG_FAILED; 
      }

      cellAndUeCb.ueCb = ueCb;
      cellAndUeCb.cellCb = cellCb;

       /* Call function to report Cfm to CGM */
      if (ROK != nhLIMRptCfgCfm(nhIdentity , &cellAndUeCb, transCtrlBlk, 
                        cgmCfmStatus, NH_PDCP_CONFIGURED))
      {
         RLOG0(L_ERROR, "nhLIMRptCfgCfm failed");
         RETVALUE(RFAILED);
      }
   }  /* if (!transCtrlBlk->numOfPdcpCfms) */
   RETVALUE(ROK);
   
}/* End of nhLIMHndlUeIdChgCfmPdcpCfg */

#ifdef RRC_PHY_CFG_SUPP
/**
 *
 * @brief Handler to handle UeId config confirm from RLC,PDCP and PHY layer.
 *
 * @details
 *
 *       This function is used to handle Ue config confirm
 *       when PHY Layer is Configured.
 *            
 *     @param[in] transCtrlBlk     Transaction Control Block
 *     @param[in] cellCb      Cell Control Block
 *     @param[in] ueCb        UE Control Block
 *     @param[in] cfmStatus   Confirmation Status 
 *     @param[in] nhIdentity  RRC ID(NhId) containing cell ID and UE ID
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlUeIdChgCfmPhyCfg
(
TransCb         *transCtrlBlk,
NhCellCb        *cellCb,
NhActvUeCb      *ueCb,
U16             cfmStatus,
NhId    *nhIdentity
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlUeIdChgCfmPhyCfg(transCtrlBlk,cellCb,ueCb,
                                      cfmStatus,nhIdentity)
TransCb         *transCtrlBlk;
NhCellCb        *cellCb;
NhActvUeCb      *ueCb;
U16             cfmStatus;
NhId            *nhIdentity;
#endif /* ANSI */
{
   U8                 cgmCfmStatus = NH_CFG_SUCCESS;
   NhUeCfgCfmMapInfo  *ueMapTbl = NULLP;
   NhCellAndUeCb      cellAndUeCb;

   ueMapTbl = &(transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY]);

   /* Drop Undesired/Duplicate Cfg Cfm */
   if (transCtrlBlk->numOfPhyCfms <= 0)
   {
      RLOG0(L_DEBUG, "Duplicate Phy Cfg Cfm Rcvd ");
      RETVALUE(ROK);
   }
   if (ueMapTbl->status == NH_STATUS_USED) 
   {
      if (cfmStatus != CTF_CFG_CFM_OK) 
      {
         ueMapTbl->cfmStatus = CTF_CFG_CFM_NOK;
         /* Update General Statistics for lower layer failure */
         nhUTLUpdGenSts(NH_STS_CAUSE_LOWER_LYR_FAIL);
      }
      else
      {
         ueMapTbl->cfmStatus = CTF_CFG_CFM_OK;
      }
   }
   transCtrlBlk->numOfPhyCfms--;

   /* If all cfms rcvd then set cfmStatus for layer */
   if (transCtrlBlk->numOfPhyCfms <= 0)
   {
      if (((ueMapTbl->status == NH_STATUS_USED) && 
           (ueMapTbl->cfmStatus != CTF_CFG_CFM_OK)) 
           || ((transCtrlBlk->phyMapInfo.status == NH_STATUS_USED)
             && (transCtrlBlk->phyMapInfo.cfmStatus != CTF_CFG_CFM_OK)))
      {
         cgmCfmStatus = NH_CFG_FAILED;
      }

      cellAndUeCb.ueCb = ueCb;
      cellAndUeCb.cellCb = cellCb;

       /* Call function to report Cfm to CGM */
      if (ROK != nhLIMRptCfgCfm(nhIdentity , &cellAndUeCb, transCtrlBlk, 
                        cgmCfmStatus, NH_PHY_CONFIGURED))
      {
         RLOG0(L_ERROR, "nhLIMRptCfgCfm failed");
         RETVALUE(RFAILED);
      }
   }  /* if (!transCtrlBlk->numOfPhyCfms) */
   /* Fix for Solaris gcc compile time warning */  
   RETVALUE(ROK);
}/* End of nhLIMHndlUeIdChgCfmPhyCfg */
#endif /* RRC_PHY_CFG_SUPP */
   
/**
 *
 * @brief Handler to handle UeId config confirm from RLC,PDCP and PHY layer.
 *
 * @details
 *
 *       This function is used to handle Ue config confirm
 *            from RLC layer.
 *            
 *     @param[in] nhIdentity  RRC ID(NhId) containing cell ID and UE ID
 *     @param[in] transId     Transaction context
 *     @param[in] status      Configuration status
 *     @param[in] lyrCfgd     Indication to know which layer got configured
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlUeIdChgCfm
(
NhId    *nhIdentity ,
U32      transId,
CmStatus status,
U8       lyrCfgd
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlUeIdChgCfm(nhIdentity , transId, status, lyrCfgd)
NhId    *nhIdentity;
U32      transId;
CmStatus status;
U8       lyrCfgd;
#endif /* ANSI */
{
   NhCellCb        *cellCb    = NULLP;
   NhActvUeCb      *ueCb      = NULLP;
   TransCb         *transCtrlBlk   = NULLP;
   U16             cfmStatus = ROK;
   S16             ret;

   TRC2(nhLIMHndlUeIdChgCfm)

   RLOG_ARG2(L_DEBUG, DBG_TRNSID,transId,"ueId[%d] cellId[%d]",
      nhIdentity->ueId, nhIdentity->cellId);

   if (NULLP == (cellCb = nhDBMGetCellCb(nhIdentity ->cellId)))
   {
      RLOG0(L_ERROR, "NULLP Cell Cb");
      RETVALUE(RFAILED);
   }

   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, nhIdentity ->ueId)))
   {
      RLOG0(L_ERROR, "NULL UE CB ");
      RETVALUE(RFAILED); 
   }
    /* Find trans node in list with specified transId value */
   if ((TransCb*)NULLP == 
         (transCtrlBlk = nhUTLGetTransCbFrmNxTransId (ueCb, transId)))
   {
      RLOG0(L_ERROR, 
                  "transId doesnt exist ");
      RETVALUE(RFAILED);
   }
   cfmStatus = status.status;
   
   if (lyrCfgd == NH_RLC_CONFIGURED)
   {
      ret = nhLIMHndlUeIdChgCfmRlcCfg(transCtrlBlk,cellCb,ueCb,
                                     cfmStatus,nhIdentity);
      if(ROK != ret)
      {
          RETVALUE(RFAILED);
      }
   }   
   else if (lyrCfgd == NH_PDCP_CONFIGURED)
   {
      ret = nhLIMHndlUeIdChgCfmPdcpCfg(transCtrlBlk,cellCb,ueCb,
                                       cfmStatus,nhIdentity);
      if(ROK != ret)
      {
          RETVALUE(RFAILED);
      }
   }   
#ifdef RRC_PHY_CFG_SUPP
   else if (lyrCfgd == NH_PHY_CONFIGURED)
   {
      ret = nhLIMHndlUeIdChgCfmPhyCfg(transCtrlBlk,cellCb,ueCb,
                                      cfmStatus,nhIdentity);
      if(ROK != ret)
      {
          RETVALUE(RFAILED);
      }
   }   
#endif /* RRC_PHY_CFG_SUPP */
   
   RETVALUE(ROK);
} /* end of nhLIMHndlUeIdChgCfm */
/**
 *
 * @brief Handler to handle configuration confirm from RLC layer.
 *
 * @details
 *
 *       This function is used to handle configuration confirm
 *            from RLC layer.
 *            
 *     @param[in] cfmInfo   RLC configuration confirm info
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlRlcCfgCfm
(
CkwCfgCfmInfo *cfmInfo  
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlRlcCfgCfm(cfmInfo)
CkwCfgCfmInfo *cfmInfo;
#endif /* ANSI */
{
   NhCellCb        *cellCb = NULLP;
   NhUstaDgn       dgn;
   U8 entIndex;

   TRC2(nhLIMHndlRlcCfgCfm)

   RLOG_ARG2(L_DEBUG, DBG_TRNSID,cfmInfo->transId,"ueId[%d] cellId[%d]",
      cfmInfo->ueId, cfmInfo->cellId);

   /* If transId is special reserved Id then transCtrlBlk must have got  
    * deleted, so handle confirm gracefully. 
    */
   if (cfmInfo->transId == NH_RESERVED_TRANS_ID)
   {
      /* Raise a critical alarm to layer manager */
      for (entIndex = 0; entIndex < cfmInfo->numEnt; entIndex++)
      {
         if (cfmInfo->entCfgCfm[entIndex].status.status == CKW_CFG_CFM_NOK)
         {
            NH_SET_ZERO (&dgn, sizeof(dgn));
            dgn.type = LNH_USTA_DGNVAL_CELLUEID;
            dgn.u.cellUeId.ueId = cfmInfo->ueId;
            dgn.u.cellUeId.cellId = cfmInfo->cellId;
            nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LNH_EVENT_RLC_RVRT_FAIL,
                       LCM_CAUSE_UNKNOWN, &dgn);
         }
      }
      RETVALUE(ROK);
   }

   if (NULLP == (cellCb = nhDBMGetCellCb(cfmInfo->cellId)))
   {
      RLOG0(L_ERROR, "NULLP Cell Cb");
      RETVALUE(RFAILED);
   }


   if (cfmInfo->ueId > 0)
   {
      if (ROK != (nhLIMHndlRlcUECfgCfm(cfmInfo, cellCb)))
      {
         RLOG0(L_ERROR, "failed ");
         RETVALUE(RFAILED);
      }

   }
   else
   {
      if (ROK != (nhLIMHndlRlcCellCfgCfm(cfmInfo, cellCb)))
      {
         RLOG0(L_ERROR, "failed ");
         RETVALUE(RFAILED);
      }
    
   }
   RETVALUE(ROK);
}  /* end of nhLIMHndlRlcCfgCfm */


/**
 *
 * @brief Handler to handle configuration confirm from PDCP layer.
 * @details
 *
 *       This function is used to handle configuration confirm
 *            from PDCP layer.
 *            
 *     @param[in] pdcpCfgCfm   PDCP configuration confirm info
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlPdcpCfgCfm
(
CpjCfgCfmInfo             *pdcpCfgCfm   
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlPdcpCfgCfm(pdcpCfgCfm)
CpjCfgCfmInfo             *pdcpCfgCfm; 
#endif /* ANSI */
{
   U8                entIndex;
   NhId              nhIdentity;
   NhActvUeCb        *ueCb         = NULLP;
   TransCb           *transCtrlBlk      = NULLP;
   NhUstaDgn          dgn;
   NhCellCb          *cellCb       = NULLP;
   NhCellAndUeCb      cellAndUeCb;

   TRC2(nhLIMHndlPdcpCfgCfm)

   RLOG0(L_DEBUG, "nhLIMHndlPdcpCfgCfm ");

   /* Reserved Trans ID is used in complete release of UE Cb or configuration
    * during revert. If transId is special reserved ID, then transCtrlBlk 
    * must have got deleted, so handle confirm gracefully.  
    */
   if (pdcpCfgCfm->transId == NH_RESERVED_TRANS_ID)
   {
      /* Raise a critical alarm to layer manager */
      for (entIndex = 0; entIndex < pdcpCfgCfm->numEnt && 
                      (entIndex < CPJ_MAX_CFM ); entIndex++)
      {
         if (pdcpCfgCfm->cfmEnt[entIndex].status == CPJ_CFG_CFM_NOK)
         {
            NH_SET_ZERO (&dgn, sizeof(dgn));
            dgn.type = LNH_USTA_DGNVAL_CELLUEID;
            dgn.u.cellUeId.ueId = pdcpCfgCfm->ueId;
            dgn.u.cellUeId.cellId = pdcpCfgCfm->cellId;
            nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LNH_EVENT_PDCP_RVRT_FAIL,
                          LCM_CAUSE_UNKNOWN, &dgn);
         }
      }
      RETVALUE(ROK);
   }

   nhIdentity.cellId = pdcpCfgCfm->cellId;
   nhIdentity.ueId = pdcpCfgCfm->ueId;

   if (NULLP == (cellCb = nhDBMGetCellCb(pdcpCfgCfm->cellId)))
   {
      RLOG0(L_ERROR, "NULL CELL CB ");
      RETVALUE(RFAILED); 
   }

   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, pdcpCfgCfm->ueId)))
   {
      if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetNewUeCb(cellCb, 
                                        pdcpCfgCfm->ueId)))
      {
         RLOG0(L_ERROR, "NULL UE CB ");
         RETVALUE(RFAILED); 
      }
   }

   /* Find trans node in list with specified transId value */
   if ((TransCb*)NULLP == (transCtrlBlk = nhUTLGetTransCbFrmNxTransId(ueCb,
                                          pdcpCfgCfm->transId)))
   {
      RLOG0(L_ERROR, "transId doesnt exist ");
      RETVALUE(RFAILED);
   }

   /* Drop Undesired/Duplicate Cfg Cfm */
   if (transCtrlBlk->numOfPdcpCfms <= 0)
   {
      RLOG0(L_DEBUG, "Invalid Pdcp Cfg Cfm Rcvd ");
      RETVALUE(ROK);
   }

   /* If confirm required for Discard Required modification, return */
   if( (transCtrlBlk->cause == NH_MSGTYPE_SECMODCMD) &&
       (transCtrlBlk->ordrdCfg.actionMask == NH_ORD_CFG_RB_MOD) )
   {
      RLOG0(L_DEBUG, "Confirm received for Discard Required modification");
      transCtrlBlk->numOfPdcpCfms--;

      cellAndUeCb.ueCb = ueCb;
      cellAndUeCb.cellCb = cellCb;
      /* Stop lower Layer Cfg Timer */
      nhStopLLyrCfgTmr(&cellAndUeCb, transCtrlBlk->nxTransId);

      (Void)nhDBMDelTransCb(ueCb, &transCtrlBlk);
      /* 
       * Not checking the ret of the above function since it always 
       * returns ROK. If in future any changes are done in this function 
       * ROK then return with RFAILED.
       */

      RETVALUE(ROK);
   }

   cellAndUeCb.ueCb = ueCb;
   cellAndUeCb.cellCb = cellCb;

   if (ROK !=  nhLIMVldtAndRptCfgCfm ((Void *)pdcpCfgCfm, transCtrlBlk,
                        &nhIdentity, &cellAndUeCb, NH_PDCP_CONFIGURED))
   {
      RLOG0(L_ERROR, "nhLIMVldtAndRptCfgCfm failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* end of nhLIMHndlPdcpCfgCfm */


/**
 *
 * @brief Handler to report configuration confirm to CGM module and take 
 *        revert action, if required.
 *
 * @details
 *
 *       This function is used to report configuration confirm
 *            to CGM module and take revert action if required.
 *            
 *     @param[in] nhIdentity    RRC ID(NhId) containing cell ID and UE ID
 *     @param[in] cellAndUeCb   Cell and UE context
 *     @param[in] transCtrlBlk  Transaction context
 *     @param[in] cfmStatus     Configuration confirm status
 *     @param[in] layerId       Layer ID, that is, RLC/PDCP/MAC
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMRptCfgCfm
(
NhId       *nhIdentity ,
NhCellAndUeCb  *cellAndUeCb,
TransCb *transCtrlBlk,
U8 cfmStatus,   
U8 layerId     
)
#else /* ANSI */
PUBLIC S16 nhLIMRptCfgCfm(nhIdentity , cellAndUeCb, transCtrlBlk, 
                          cfmStatus, layerId)
NhId       *nhIdentity;
NhCellAndUeCb  *cellAndUeCb;
TransCb *transCtrlBlk;
U8 cfmStatus;   
U8 layerId;    
#endif /* ANSI */
{
   S16         ret = RFAILED;
   U8          tmpCfgMask; 
   U8          tmpCfmStatus; 
   U32         nxTransId;
   U8          status;

   TRC2(nhLIMRptCfgCfm)

   RLOG2(L_DEBUG, "cfmStatus[%d] layerId[%d]", cfmStatus, layerId);

   if(layerId != NH_LAYER_NONE)
   {
      /* Reset Cfm Mask as Cfm Rcvd from layerId */
      NH_RESET_TRANS_CFM_MASK(transCtrlBlk->cfmMask, layerId);
      /* Set Status of Confirm to send to CGM module */
      NH_SET_CFG_CFM_STATUS(transCtrlBlk->cfmStatus, cfmStatus, layerId);
   }
   
   /* check whether this is last confirm from lower layers */
   if (0x00 == transCtrlBlk->cfmMask) 
   {
      /* Stop lower Layer Cfg Timer */
      nhStopLLyrCfgTmr(cellAndUeCb, transCtrlBlk->nxTransId);

      tmpCfgMask = transCtrlBlk->cfgMask; 
      tmpCfmStatus = transCtrlBlk->cfmStatus;

      /* Check if Revert action needs to be taken */

      /* transCtrlBlk->cfgMask contains all bits set for layers to which cfg
       * has been sent. transCtrlBlk->cfmStatus contains all bit set for layers
       * successfully configured.
       */
      NH_IS_LOWER_LYR_CFG_SUCCESS(transCtrlBlk, status);

      /* After receiving confirms from MAC and RLC for re-establishment in *
       * case of PDCP SDU Status (HO), nhPIMSndSduStaReq is called to      *
       *   -- send PDCP SDU status request to PDCP if successful confirms  *
       *      are received from MAC and RLC                                *
       *   -- If unsuccessful confirms, PDCP SDU request is not sent and   *
       *      transCtrlBlk is deleted                                           
       */
     
     /*nh002.301:118385: HO issue: send sdu sta cfm to rrc user since cfm
      *from all the layer has been received 
      */

      /* Call function to send PDCP SDU Status confirm to RRC user */
      if (transCtrlBlk->cause == NH_TRANSCB_SDU_STA_RESTAB)
      {
         if(transCtrlBlk->nhuSduStaCfm != NULLP)
         {
            nhUIMSndPdcpSduStaCfm(transCtrlBlk->nhuSduStaCfm, transCtrlBlk->cfgMask,
                  transCtrlBlk->cfmStatus, cellAndUeCb->ueCb);
         }
         RETVALUE(ROK);
      }

      if (status)
      {
         if (ROK != nhDBMCommitOrdrdCfg(cellAndUeCb->ueCb, transCtrlBlk))
         {
            RLOG0(L_ERROR, 
                          "nhDBMCommitOrdrdCfg failed");
            RETVALUE(RFAILED);
         }
         /* Fix for the CR ccpu00125207 */
         cellAndUeCb->ueCb->rbSecCfgBitMask |= NH_SRB1_CFGD;

         /* In case cause is connection setup or security command and storeMsg
          * is available, then call utl function to transmit.
          */ 
         /* nh003.201 - Order of sending rrc conn reconfig 
                        message is corrected */
         if ((transCtrlBlk->cause == NH_MSGTYPE_CONNSETUP) || 
             (transCtrlBlk->cause == NH_MSGTYPE_CONNREESTAB) ||
             (transCtrlBlk->cause == NH_MSGTYPE_SECMODCMD) ||
             (transCtrlBlk->cause == NH_MSGTYPE_CONNRECFG))
         {
            if (transCtrlBlk->storedMsg)
            {
               if (ROK != nhUTLSendStoredMsg(cellAndUeCb, transCtrlBlk))
               {
                  RLOG0(L_ERROR, "nhUTLSendStoredMsg failed");
                  RETVALUE(RFAILED);
               }

            }
         }
      } 
      else
      {
         /* Revert is required, Delete the new entry for UE if ueidChangereq
          * has been sent to lower layers */
         if(transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_UEID &&
            transCtrlBlk->ordrdCfg.ueInfo.crntiCfgType == NH_UE_INFO_MOD_CRNTI)
         {
            ret = nhHashListDelete(&(cellAndUeCb->cellCb->newUeHlCp),
                                   (PTR)(cellAndUeCb->ueCb));
            if (ROK != ret)
            {
                RLOG0(L_INFO, "Could not delete from  list.");
            }
         }
         (Void)nhLIMHndlRvrtCfg(nhIdentity, cellAndUeCb->ueCb, 
                                transCtrlBlk);
         /* 
          * Not checking the ret of the above function since it always 
          * returns ROK. If in future any changes are done in this function 
          * ROK then assign RFAILED to ret.
          */
      }

      nxTransId = transCtrlBlk->nxTransId;

      /* Confirmation is being sent to the RRC User at last so that
       * it does not initiate any other transaction during this ongoing
       * procedure.This is true for both postive and negative config.
       */
      /* Send Cfg Cfm Report to CGM module */
      if (ROK != nhUIMSndCfgCfm(*nhIdentity , nxTransId, tmpCfgMask, 
                                tmpCfmStatus, NHU_CFGERRCAUSE_UNKNOWN))
      { 
         RLOG0(L_ERROR, "nhUIMSndCfgCfm failed");
         RETVALUE(ret);
      }

      /* If Security Mode Command is set in transCtrlBlk, there is a possibility
       * that cipgering configuration is present in it. So transCtrlBlk should 
       * not be deleted here. But in case of configuration failure, 
       * transCtrlBlk can be deleted */
      if ((!status) || (transCtrlBlk->cause != NH_MSGTYPE_SECMODCMD))
      {
         (Void)nhDBMDelTransCb(cellAndUeCb->ueCb, &transCtrlBlk);
         /* 
          * Not checking the ret of the above function since it always 
          * returns ROK. If in future any changes are done in this function 
          * ROK then return with RFAILED.
          */
      }
   }  /* end if (0x00 == transCtrlBlk->cfmMask) */ 

   RETVALUE(ROK);
} /* end of nhLIMRptCfgCfm */


/**
 *
 * @brief Handler to report cell configuration confirm to CGM module and take 
 *        revert action, if required.

 * @details
 *       This function is used to report cell configuration confirm
 *            to CGM module and take revert action, if required.
 *            
 *     @param[in] cellCb     Cell context
 *     @param[in] cfmStatus  Configuration confirm status
 *     @param[in] layerId    Layer ID, that is, RLC/PDCP/MAC
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMRptCellCfgCfm
(
NhCellCb *cellCb,
U8 cfmStatus,   
U8 layerId     
)
#else /* ANSI */
PUBLIC S16 nhLIMRptCellCfgCfm(cellCb, cfmStatus, layerId)
NhCellCb *cellCb; 
U8 cfmStatus;    
U8 layerId;     
#endif /* ANSI */
{
   NhCellTransCb *cellTransCb = NULLP;
   S16            ret = ROK;
   U8             status = NH_CFG_SUCCESS;

   TRC2(nhLIMRptCellCfgCfm)

   RLOG2(L_DEBUG, "nhLIMRptCellCfgCfm: cfmStatus(%d) layerId(%d)", 
                   cfmStatus, layerId);

   cellTransCb = &(cellCb->cellTransCb); 

   /* Reset Cfm Mask as Cfm Rcvd from layerId */
   NH_RESET_TRANS_CFM_MASK(cellTransCb->cfmMask, layerId);
   /* Set Status of Confirm to send to CGM module */
   NH_SET_CFG_CFM_STATUS(cellTransCb->cfmStatus, cfmStatus, layerId);

   /* check whether this is the last confirm from lower layers */
   if (0x00 == cellTransCb->cfmMask)
   {
      /* Stop Cell Cfg Timer */
      if( ROK != (ret = nhSchedGenTmr(cellCb, NH_TMR_CELL_CFGCFM, NH_TMR_STOP)))
      {
         RLOG0(L_ERROR, "stop timer failed ");
         RETVALUE(RFAILED);
      }

      /* Check if Revert action needs to be taken */

      /* cellTransCb->cfgMask contains all bits set for layers to which cfg
       * is sent. cellTransCb->cfmStatus contains all bits set for layers
       * successfully configured.
       */
      if (cellTransCb->cfmStatus == cellTransCb->cfgMask)
      {
         if (ROK != nhDBMCommitCellOrdrdCfg(cellCb))
         {
            RLOG0(L_ERROR,"nhUDMCommitCellOrdrdCfg failed");
            RETVALUE(RFAILED);
         }
      } 
      else
      {
         status = NH_CFG_FAILED;

         if(NH_CFG_ACTN_RLS != cellCb->cellTransCb.cfgAction)
         {
            (Void)nhLIMHndlRvrtCellCfg(cellCb);
            /* 
             * Not checking the ret of the above function since it always 
             * returns ROK. If in future any changes are done in this function 
             * ROK then return with RFAILED.
             */
         }
      }  /* end else */
      
      /* Send Cfg Cfm report to CGM module */
      if (ROK != nhUIMSndCellCfgCfm(cellTransCb->cfmStatus, cellCb,
                                                NHU_CFGERRCAUSE_UNKNOWN))
      { 
         RLOG0(L_ERROR, "nhUIMSndCellCfgCfm failed");
         ret = RFAILED;
      }

      /* If the event is cell release or cell setup, 
       * and configuration fails, delete the cellcb here after sending 
       * the rvrt config request */
      if((NH_CFG_ACTN_RLS == cellCb->cellTransCb.cfgAction) || 
            ((NH_CFG_ACTN_ADD == cellCb->cellTransCb.cfgAction) && 
            ((NH_CFG_FAILED == status) || (RFAILED == ret))))
      {
         if(ROK != nhDBMDeleteCellCb(&cellCb))
         {
            RLOG0(L_ERROR, "nhDBMDeleteCellCb failed ");
         }
         RETVALUE(ret);
      }
      else
      {
         cellCb->cellTransCb.status = NH_NOT_USED;
      }
   }  /* end if (0x00 == cellTransCb->cfmMask) */ 

   RETVALUE(ROK);
} /* end of nhLIMRptCellCfgCfm */


/**
 *
 * @brief Handler to revert configuration of successful layers, if any 
 *        layer failed to configure.
 *
 * @details
 *
 *       This function is used to revert configuration 
 *            of successful layers, if any layer failed  
 *            to configure.
 *            
 *     @param[in] cellCb    Cell context
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMHndlRvrtCellCfg
(
NhCellCb *cellCb 
)
#else /* ANSI */
PUBLIC S16 nhLIMHndlRvrtCellCfg(cellCb)
NhCellCb *cellCb;
#endif /* ANSI */
{
   NhUstaDgn   dgn;
   NhCellTransCb *cellTransCb = NULLP;
#ifdef RRC_PHY_CFG_SUPP
   S16         phyRet = 0;
#endif /* RRC_PHY_CFG_SUPP */
   S16         macRet = 0;
   S16         rlcRet = 0;

   TRC2(nhLIMHndlRvrtCellCfg)
 
   RLOG0(L_DEBUG, "nhLIMHndlRvrtCellCfg ");

   cellTransCb = &(cellCb->cellTransCb);
   /* Set transId in transCtrlBlk to reserved trans ID, 
    * do not care for cfg confirms for these requests.
    */
   cellTransCb->nxTransId = NH_RESERVED_TRANS_ID;
   
#ifdef RRC_PHY_CFG_SUPP
   if (cellTransCb->cfgMask & NH_PHY_CONFIGURED)
   {
      phyRet = nhPIMRvrtPhyCellCfg(cellCb);
   }
#endif /* RRC_PHY_CFG_SUPP */

 
   if (cellTransCb->cfgMask & NH_MAC_CONFIGURED)
   {
      macRet = nhPIMRvrtMacCellCfg(cellCb);
   }

   if (cellTransCb->cfgMask & NH_RLC_CONFIGURED)
   {
      rlcRet = nhPIMRvrtRlcCellCfg(cellCb);
   }

#ifdef RRC_PHY_CFG_SUPP
      if ((ROK != macRet) || (ROK != rlcRet) || (ROK != phyRet))
#else /* RRC_PHY_CFG_SUPP */
      if((ROK != macRet) || (ROK != rlcRet))
#endif /* RRC_PHY_CFG_SUPP */
   {
      /* Raise a critical alarm to layer manager */
      NH_SET_ZERO (&dgn, sizeof(dgn));
      dgn.type = LNH_USTA_DGNVAL_CELLUEID;
      dgn.u.mem.region = nhCb.init.region;
      dgn.u.mem.pool = nhCb.init.pool;
      nhSendLmAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_INV_EVT,
                       LCM_CAUSE_UNKNOWN, &dgn);
   }
   /* Do not start Cfg timer here */
   /* All Revert Cfg Req is sent with special reserved trans ID, and confirmations
    * corresponding to these requests are handled gracefully.
    */
   cellTransCb->status = NH_STATUS_UNUSED;
   cellTransCb->numOfRlcCfms = 0;
   cellTransCb->numOfMacCfms = 0;
   NH_SET_ZERO(cellTransCb->rlcMapTbl, 
         NH_MAX_RB_PER_CELL_CFG*sizeof(NhRlcCellCfgCfmMap));
   NH_SET_ZERO(cellTransCb->macMapTbl, 
         NH_MAX_RB_PER_CELL_CFG*sizeof(NhMacCellCfgCfmMap));

   RETVALUE(ROK);
} /* end of nhLIMHndlRvrtCellCfg */


/**
 *
 * @brief Handler to send cell configuration to the lower layers.
 * @details
 *
 *       This function is used to send cell configuration   
 *            to the lower layers.
 *            
 *     @param[in] cellCb   Cell context
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMSendCellCfg
(
NhCellCb *cellCb        
)
#else /* ANSI */
PUBLIC S16 nhLIMSendCellCfg(cellCb)
NhCellCb *cellCb;      
#endif /* ANSI */
{
   NhCellTransCb       *cellTransCb = NULLP;
   S16  ret = RFAILED;

   TRC2(nhLIMSendCellCfg)

   RLOG0(L_DEBUG, "sending cell Cfg ... ");

   cellTransCb = &(cellCb->cellTransCb);

   if (NH_STATUS_USED != cellTransCb->status)
   {
      RLOG0(L_ERROR,"Invalid Cell to Configure");
      RETVALUE(RFAILED);
   }

   /* Start Cell Cfg Timer */
   if( ROK != (ret = nhSchedGenTmr(cellCb, NH_TMR_CELL_CFGCFM, NH_TMR_START)))
   {
      RLOG0(L_ERROR, "start timer failed  ");
      RETVALUE(RFAILED);
   }

   if (cellTransCb->cfgMask & NH_MAC_CONFIGURED)
   {
      if (ROK != nhPIMSendMacCellCfg(cellCb))
      {
         /* Stop Cell Cfg Timer */
         if( ROK != (ret = nhSchedGenTmr(cellCb, NH_TMR_CELL_CFGCFM, 
                                         NH_TMR_STOP)))
         {
            RLOG0(L_ERROR, "stop timer failed ");
            RETVALUE(RFAILED);
         }

         RLOG0(L_ERROR, "nhPIMSendMacCellCfg failed ");
         RETVALUE(RFAILED);
      }
   }

   if (cellTransCb->cfgMask & NH_RLC_CONFIGURED)
   {
      if (ROK != nhPIMSendRlcCellCfg(cellCb))
      {
         /* Stop Cell Cfg Timer */
         if( ROK != (ret = nhSchedGenTmr(cellCb, NH_TMR_CELL_CFGCFM, 
                                         NH_TMR_STOP)))
         {
            RLOG0(L_ERROR, "stop timer failed ");
            RETVALUE(RFAILED);
         }

         RLOG0(L_ERROR, "nhPIMSendRlcCellCfg failed ");
         RETVALUE(RFAILED);
      }
   }

#ifdef RRC_PHY_CFG_SUPP
   if (cellTransCb->cfgMask & NH_PHY_CONFIGURED)
   {
      if (ROK != nhPIMSendPhyCellCfg(cellCb))
      {
         /* Stop Cell Cfg Timer */
         if( ROK != (ret = nhSchedGenTmr(cellCb, NH_TMR_CELL_CFGCFM, 
                                         NH_TMR_STOP)))
         {
            RLOG0(L_ERROR, "stop timer failed ");
            RETVALUE(RFAILED);
         }

         RLOG0(L_ERROR, "nhPIMSendPhyCellCfg failed ");
         RETVALUE(RFAILED);
      }
   }
#endif /* RRC_PHY_CFG_SUPP */
   RETVALUE(ROK);
}  /* end of nhLIMSendCellCfg */


/**
 *
 * @brief Handler to revert configuration of MAC layer, if any layer failed 
 *        to configure.
 *
 * @details
 *
 *       This function is used to revert configuration 
 *            of MAC layer, if any layer failed to configure.
 *            
 *     @param[in] nhIdentity     RRC ID (NhId)
 *     @param[in] ueCb           UE context
 *     @param[in] transCtrlBlk   Transaction context
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMHndlRvrtMacCfg
(
NhId *nhIdentity,
NhActvUeCb *ueCb,  
TransCb *transCtrlBlk  
)
#else /* ANSI */
PUBLIC S16 nhLIMHndlRvrtMacCfg(nhIdentity, ueCb, transCtrlBlk)
NhId *nhIdentity;
NhActvUeCb *ueCb;
TransCb *transCtrlBlk; 
#endif /* ANSI */
{
   U8      rvrtCfgType = 0;
   S16      ret;

   TRC2(nhLIMHndlRvrtMacCfg)

   UNUSED(nhIdentity);

   RLOG0(L_DEBUG, "In nhLIMHndlRvrtMacCfg ");

   /* Set transId in transCtrlBlk to reserved trans ID, 
   * do not care for cfg confirms for these requests.
    */
   /* If Cfg was added in MAC */
   if ((transCtrlBlk->macLchMapTbl[NH_DL_LC].addCfmStatus > 0)
         || (transCtrlBlk->macLchMapTbl[NH_UL_LC].addCfmStatus > 0))
   {
      rvrtCfgType |= NH_CRG_LOGCH_MAP;
   }

   if ((NH_STATUS_USED == 
            transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].status) && 
       (NH_MAC_CFG_ADD == 
            transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].action) &&
       (CRG_CFG_CFM_OK == 
            transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].cfmStatus))
   {
      rvrtCfgType |= NH_CRG_UE_CFG;
   }

   if(rvrtCfgType > 0)
   {
      ret = nhPIMRvrtMacAddCfg(ueCb, transCtrlBlk, rvrtCfgType);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, " nhPIMRvrtMacAddCfg failed.");
         RETVALUE(RFAILED);
      }
   }
   rvrtCfgType = 0;

   if (transCtrlBlk->macLchMapTbl[NH_UL_LC].modCfmStatus > 0)
   {
      rvrtCfgType |= NH_CRG_LOGCH_MAP;
   }

   if ((NH_STATUS_USED == 
           transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].status) &&
       (NH_MAC_CFG_MODIFY == 
           transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].action) &&
       (CRG_CFG_CFM_OK == 
           transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].cfmStatus))
   {
      rvrtCfgType |= NH_CRG_UE_CFG;
   }

   if(rvrtCfgType > 0)
   {
      ret = nhPIMRvrtMacModCfg(ueCb, transCtrlBlk, rvrtCfgType);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, "nhPIMRvrtMacModCfg failed.");
         RETVALUE(RFAILED);
      }
   }

   rvrtCfgType = 0;

   if ((transCtrlBlk->macLchMapTbl[NH_DL_LC].rlsCfmStatus > 0)
         || (transCtrlBlk->macLchMapTbl[NH_UL_LC].rlsCfmStatus > 0))
   {
      rvrtCfgType |= NH_CRG_LOGCH_MAP;
   }

   if(rvrtCfgType > 0)
   {
      ret = nhPIMRvrtMacRlsCfg(ueCb, transCtrlBlk, rvrtCfgType);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, "nhPIMRvrtMacRlsCfg failed.");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
} /* end of nhLIMHndlRvrtMacCfg */ 


/**
 *
 * @brief Handler to handle cell configuration confirm from MAC layer.
 * @details
 *
 *       This function is used to handle configuration confirm
 *            from MAC layer.
 *            
 *     @param[in] nhIdentity         RRC ID (NhId)
 *     @param[in] cellCb       Cell context
 *     @param[in] cfgElms      Configuration element (Cell or Logical Channel)
 *     @param[in] status       Configuration status
 *     @param[in] lclId        Logical channel ID
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlMacCellCfgCfm
(
NhId            *nhIdentity,
NhCellCb        *cellCb,
U8              cfgElms,
U8              status,
U8              lclId
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlMacCellCfgCfm(nhIdentity, cellCb, cfgElms, status, lclId)
NhId            *nhIdentity;
NhCellCb        *cellCb;
U8              cfgElms;
U8              status;
U8              lclId;
#endif /* ANSI */
{
   U8            rbIndex = 0;
   NhCellTransCb   *cellTransCb = NULLP;
   U8              cgmCfmStatus = NH_CFG_SUCCESS;

   TRC2(nhLIMHndlMacCellCfgCfm)

   RLOG0(L_DEBUG, "nhLIMHndlMacCellCfgCfm");

   UNUSED(nhIdentity);

   cellTransCb = &(cellCb->cellTransCb);

   if (cellTransCb->numOfMacCfms <= 0)
   {
      RLOG0(L_DEBUG, "duplicate Mac Cfg Cfm RCVD ");
      RETVALUE(ROK);
   }
   /* Index 0 is for cell config */
   if (cfgElms & NH_CRG_CELL_CFG)
   {
      if (cellTransCb->macMapTbl[0].status == NH_STATUS_USED)
      {
         cellTransCb->numOfMacCfms--;
         if (status != CRG_CFG_CFM_OK)
         {
            cellTransCb->macMapTbl[0].cfmStatus = NH_CFG_FAILED;
         }
         else
         {
            cellTransCb->macMapTbl[0].cfmStatus = NH_CFG_SUCCESS;
         }
      }
   }
   else if (cfgElms & NH_CRG_CELL_LOGCH_MAP)
   {
      cellTransCb->numOfMacCfms--;
      /* Starting from 1, since index 0 is reserved for
       * cell config.
       */ 
      for (rbIndex = 1; rbIndex < NH_MAX_RB_PER_CELL_CFG; rbIndex++)
      {
         if ((cellTransCb->macMapTbl[rbIndex].status == NH_STATUS_USED) && 
                         (lclId == cellTransCb->macMapTbl[rbIndex].logChId))
         {
            if (status != CRG_CFG_CFM_OK)
            {
               cellTransCb->macMapTbl[rbIndex].cfmStatus = NH_CFG_FAILED;
            }
            else
            {
               cellTransCb->macMapTbl[rbIndex].cfmStatus = NH_CFG_SUCCESS;
            }
            break;
         }
      }
   }

   /* If all cfms are received, then set cfmStatus for layer */
   if (cellTransCb->numOfMacCfms <= 0)
   {
      for (rbIndex = 0; rbIndex < NH_MAX_RB_PER_CELL_CFG; rbIndex++)
      {
         if ((cellTransCb->macMapTbl[rbIndex].status == NH_STATUS_USED) && 
             (NH_CFG_FAILED == cellTransCb->macMapTbl[rbIndex].cfmStatus))
         {
               cgmCfmStatus = NH_CFG_FAILED;
         }
      }
      /* Call function to report Cfm to CGM */
      if (ROK != nhLIMRptCellCfgCfm(cellCb, cgmCfmStatus, NH_MAC_CONFIGURED))
      {
         RLOG0(L_ERROR, "nhLIMRptCellCfgCfm failed ");
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}/* End of nhLIMHndlMacCellCfgCfm */

/**
 *
 * @brief Handler to handle updation of MAC Map table. 
 *
 * @details
 *
 *       This function is used to update MAC map table in transCtrlBlk
 *            from MAC layer.
 *            
 *     @param[in] transCtrlBlk    Transaction context
 *     @param[in] lclId           Logical channel ID
 *     @param[in] status          Config confirm status
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMMacUlMapTblUpd
(
TransCb       *transCtrlBlk,
U8             lclId,
U8             status
)
#else /* ANSI */
PRIVATE S16 nhLIMMacUlMapTblUpd(transCtrlBlk, lclId, status)
TransCb       *transCtrlBlk;
U8             lclId;
U8             status;
#endif /* ANSI */
{
   U32 tmp = 0x1;
   Bool found = FALSE;
   U32 *tmpCfmStatus = NULLP;

   TRC2(nhLIMMacUlMapTblUpd)
   RLOG0(L_DEBUG, "nhLIMMacUlMapTblUpd");

   found = FALSE;
   /* substract lclId with 1 */
   tmp = (1 << (lclId - 1));

   if(transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsRcvd & tmp)
   {
      RLOG1(L_INFO, "Duplicate MAC Cfg Cfm RCVD for logical channel ID[%d]",
         lclId);
      RETVALUE(ROK);
   }
   else
   {
      transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsRcvd |= tmp;
   }
      
   if (transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToAdd & tmp)
   {
      found = TRUE;
      tmpCfmStatus = &(transCtrlBlk->macLchMapTbl[NH_UL_LC].addCfmStatus);
   }
   else if (transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToMod & tmp)
   {
      found = TRUE;
      tmpCfmStatus = &(transCtrlBlk->macLchMapTbl[NH_UL_LC].modCfmStatus);
   }
   else if (transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToRls & tmp)
   {
      found = TRUE;
      tmpCfmStatus = &(transCtrlBlk->macLchMapTbl[NH_UL_LC].rlsCfmStatus);
   }

   if (FALSE == found)
   {
      RLOG0(L_ERROR, "Invalid MAC Cfg Cfm RCVD ");
      RETVALUE(RFAILED);
   }

   if (status == CRG_CFG_CFM_NOK) 
      *tmpCfmStatus &= ~tmp;

   RETVALUE(ROK);
}/* End of nhLIMMacUlMapTblUpd */

/**
 *
 * @brief Handler to handle updation of MAC DL Map table 
 *
 * @details
 *
 *       This function is used to update MAC DL map table in transCtrlBlk
 *            from MAC layer.
 *            
 *     @param[in] transCtrlBlk    transaction context
 *     @param[in] lclId           logical channel Id
 *     @param[in] status          config confirm status
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMMacDlMapTblUpd
(
TransCb       *transCtrlBlk,
U8             lclId,
U8             status
)
#else /* ANSI */
PRIVATE S16 nhLIMMacDlMapTblUpd(transCtrlBlk, lclId, status)
TransCb       *transCtrlBlk;
U8             lclId;
U8             status;
#endif /* ANSI */
{
   U32 tmp = 0x1;
   Bool found = FALSE;
   U32 *tmpCfmStatus = NULLP;

   TRC2(nhLIMMacDlMapTblUpd)
   RLOG0(L_DEBUG, "nhLIMMacDlMapTblUpd");

   found = FALSE;
   /* substract lclId with 1 */
   tmp = (1 << (lclId - 1));
   if(transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsRcvd & tmp)
   {
      RLOG1(L_INFO, "Duplicate MAC Cfg Cfm RCVD for logical channel ID[%d]",
         lclId);
      RETVALUE(ROK);
   }
   else
   {
      transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsRcvd |= tmp;
   }

   if (transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToAdd & tmp)
   {
      found = TRUE;
      tmpCfmStatus = &(transCtrlBlk->macLchMapTbl[NH_DL_LC].addCfmStatus);
   }
   else if (transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToRls & tmp)
   {
      found = TRUE;
      tmpCfmStatus = &(transCtrlBlk->macLchMapTbl[NH_DL_LC].rlsCfmStatus);
   }

   if (FALSE == found)
   {
      RLOG0(L_ERROR, "Invalid MAC Cfg Cfm RCVD ");
      RETVALUE(RFAILED);
   }

   if (status == CRG_CFG_CFM_NOK) 
      *tmpCfmStatus &= ~tmp;

   RETVALUE(ROK);
}/* End of nhLIMMacDlMapTblUpd */
/**
 *
 * @brief Handler to handle UE configuration confirm from MAC layer.
 * @details
 *
 *       This function is used to handle UE config confirm
 *            from MAC layer.
 *            
 *     @param[in] nhIdentity  RRC ID(NhId)
 *     @param[in] cellCb      Cell context
 *     @param[in] nxTransId   RRC user transaction ID
 *     @param[in] lclId       Logical channel ID 
 *     @param[in] cfgElms     Configuration element (UE or Logical channel)
 *     @param[in] status      Configuration confirm status
 *     @param[in] dir         Direction (UL/DL)
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlMacUECfgCfm
(
NhId            *nhIdentity,
NhCellCb        *cellCb,
NhTransId        nxTransId,
U8               lclId,
U8               cfgElms,
U8               status,
U8               dir
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlMacUECfgCfm(nhIdentity, cellCb, nxTransId, lclId, cfgElms, 
                                 status, dir)
NhId            *nhIdentity;
NhCellCb        *cellCb;
NhTransId        nxTransId;
U8               lclId;
U8               cfgElms;
U8               status;
U8               dir;
#endif /* ANSI */
{
   TransCb            *transCtrlBlk = NULLP;
   NhUeCfgCfmMapInfo  *macUeMapTblPtr = NULLP;
   U8                 cgmCfmStatus = NH_CFG_SUCCESS;
   NhActvUeCb         *ueCb = NULLP;
   NhCellAndUeCb      cellAndUeCb;
   NhMacRstInfo       *rstTbl=NULLP;


   TRC2(nhLIMHndlMacUECfgCfm)

   RLOG0(L_DEBUG, "nhLIMHndlMacUECfgCfm");

   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, nhIdentity->ueId)))
   {
      RLOG0(L_ERROR, "NULL UE CB");
      RETVALUE(RFAILED); 
   }
   /* Find trans node in the list with specified transId value */
   if (NULLP == (transCtrlBlk = nhUTLGetTransCbFrmNxTransId(ueCb, nxTransId)))
   {
      RLOG0(L_ERROR, "transId doesnt exist");
      RETVALUE(RFAILED);
   }

   /* Drop Undesired/Duplicate Cfg Cfm, treat it as successful case */
   if (transCtrlBlk->numOfMacCfms <= 0)
   {
      RLOG0(L_DEBUG, "duplicate Mac Cfg Cfm RCVD ");
      RETVALUE(ROK);
   }

   if (cfgElms == NH_CRG_UE_CFG)
   {
      macUeMapTblPtr = &(transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC]);

      if (macUeMapTblPtr->status == NH_STATUS_USED) 
      {
         if (status != CRG_CFG_CFM_OK) 
         {
            macUeMapTblPtr->cfmStatus = CRG_CFG_CFM_NOK;
            /* Update General Statistics for lower layer failure */
            nhUTLUpdGenSts(NH_STS_CAUSE_LOWER_LYR_FAIL);
         }
         else
         {
            macUeMapTblPtr->cfmStatus = CRG_CFG_CFM_OK;
         }
      }
      transCtrlBlk->numOfMacCfms--;
   }   
   else if (cfgElms == NH_CRG_UE_MAC_RST)
   {
      rstTbl = &(transCtrlBlk->rstTbl[NH_RESET_MAPTBL_IDX_MAC]);

      if (rstTbl->status == NH_STATUS_USED)
      {
         if (status != CRG_CFG_CFM_OK)
         {
            rstTbl->cfmStatus = CRG_CFG_CFM_NOK;
            /* Update General Statistics for lower layer failure */
            nhUTLUpdGenSts(NH_STS_CAUSE_LOWER_LYR_FAIL);
         }
         else
         {
            rstTbl->cfmStatus = CRG_CFG_CFM_OK;
         }
      }
      transCtrlBlk->numOfMacCfms--;
   }
   else if (cfgElms == NH_CRG_LOGCH_MAP)
   {
      /* Now search each Add/Mod/Rls bitmask and set the
       * corresponding cfm bit mask.
       * For duplicate case, return ROK.
       */ 
      transCtrlBlk->numOfMacCfms--;
      if (dir & NH_LCID_DIR_TX)
      {
         if (ROK != (nhLIMMacDlMapTblUpd(transCtrlBlk, lclId, status)))
         {
            RLOG0(L_DEBUG, " nhLIMMacDlMapTblUpd failed ");
            RETVALUE(ROK);
         }
      }
      if (dir & NH_LCID_DIR_RX)
      {
         if (ROK != (nhLIMMacUlMapTblUpd(transCtrlBlk, lclId, status)))
         {
            RLOG0(L_DEBUG, " nhLIMMacUlMapTblUpd failed ");
            RETVALUE(ROK);
         }
      }
      if(status == CRG_CFG_CFM_NOK)
      {
         /* Update General Statistics for lower layer failure */
         nhUTLUpdGenSts(NH_STS_CAUSE_LOWER_LYR_FAIL);
      }
   } /* else if (cfgElms == NH_CRG_LOGCH_MAP) */

   /* If all cfms are received, then set cfmStatus for layer */
   if (transCtrlBlk->numOfMacCfms <= 0)
   {
      macUeMapTblPtr = &(transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC]);
      /* Looping twice, once for UE and once for logical 
       * channel configuration confirm handling.
       */
      if ((macUeMapTblPtr->status == NH_STATUS_USED)
           && (macUeMapTblPtr->cfmStatus == CRG_CFG_CFM_NOK))
      {
         cgmCfmStatus = NH_CFG_FAILED;
      }
   
      rstTbl = &(transCtrlBlk->rstTbl[NH_RESET_MAPTBL_IDX_MAC]);
      if ((rstTbl->status == NH_STATUS_USED)
           && (rstTbl->cfmStatus == CRG_CFG_CFM_NOK))
      {
         cgmCfmStatus = NH_CFG_FAILED;
      }

      if ((transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToAdd != 
                  transCtrlBlk->macLchMapTbl[NH_DL_LC].addCfmStatus) ||
          (transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToRls != 
                  transCtrlBlk->macLchMapTbl[NH_DL_LC].rlsCfmStatus) || 
          (transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToAdd != 
                  transCtrlBlk->macLchMapTbl[NH_UL_LC].addCfmStatus) || 
          (transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToMod != 
                  transCtrlBlk->macLchMapTbl[NH_UL_LC].modCfmStatus) || 
          (transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToRls != 
                  transCtrlBlk->macLchMapTbl[NH_UL_LC].rlsCfmStatus))
      {
         cgmCfmStatus = NH_CFG_FAILED;
      }
    
      cellAndUeCb.ueCb = ueCb;
      cellAndUeCb.cellCb = cellCb;

       /* Call function to report Cfm to CGM */
      if (ROK != nhLIMRptCfgCfm(nhIdentity, &cellAndUeCb, transCtrlBlk, 
                                cgmCfmStatus, NH_MAC_CONFIGURED))
      {
         RLOG0(L_ERROR, "nhLIMRptCfgCfm failed ");
         RETVALUE(RFAILED);
      }
   }  /* if (!transCtrlBlk->numOfMacCfms) */
   RETVALUE(ROK);
}/* End of nhLIMHndlMacUECfgCfm */
#ifdef RRC_PHY_CFG_SUPP
/**
 *
 * @brief Handler to handle cell configuration confirm from PHY layer.
 * @details
 *
 *       This function is used to handle configuration confirm
 *            from PHY layer.
 *            
 *     @param[in] nhIdentity   RRC ID (NhId)
 *     @param[in] cellCb       Cell context
 *     @param[in] cfgElms      Configuration element (Cell or Logical Channel)
 *     @param[in] status       Configuration status
 *     @param[in] lclId        Logical channel ID
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlPhyCellCfgCfm
(
NhId            *nhIdentity,
NhCellCb        *cellCb,
U8              cfgElms,
U8              status
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlPhyCellCfgCfm(nhIdentity, cellCb, cfgElms, status)
NhId            *nhIdentity;
NhCellCb        *cellCb;
U8              cfgElms;
U8              status;
#endif /* ANSI */
{
   NhCellTransCb   *cellTransCb = NULLP;
   U8              cgmCfmStatus = NH_CFG_SUCCESS;

   TRC2(nhLIMHndlPhyCellCfgCfm)

   RLOG0(L_DEBUG, "nhLIMHndlPhyCellCfgCfm");

   UNUSED(nhIdentity);

   cellTransCb = &(cellCb->cellTransCb);

   if (cellTransCb->numOfPhyCfms <= 0)
   {
      RLOG0(L_DEBUG, "duplicate Phy Cfg Cfm RCVD ");
      RETVALUE(ROK);
   }
   if (cfgElms & NH_CTF_CELL_CFG)
   {
      if (cellTransCb->phyMapInfo.status == NH_STATUS_USED)
      {
         cellTransCb->numOfPhyCfms--;
         if (status != CTF_CFG_CFM_OK)
         {
            cellTransCb->phyMapInfo.cfmStatus = NH_CFG_FAILED;
            cgmCfmStatus = NH_CFG_FAILED;
         }
         else
         {
            cellTransCb->phyMapInfo.cfmStatus = NH_CFG_SUCCESS;
         }
      }
   }
   /* If all cfms are received, then set cfmStatus for layer */
   if (cellTransCb->numOfPhyCfms <= 0)
   {
      /* Call function to report Cfm to CGM */
      if (ROK != nhLIMRptCellCfgCfm(cellCb, cgmCfmStatus, NH_PHY_CONFIGURED))
      {
         RLOG0(L_ERROR, "nhLIMRptCellCfgCfm failed ");
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}/* End of nhLIMHndlPhyCellCfgCfm */

/**
 *
 * @brief Handler to handle UE configuration confirm from PHY layer.
 * @details
 *
 *       This function is used to handle UE config confirm
 *            from PHY layer.
 *            
 *     @param[in] nhIdentity  RRC ID(NhId)
 *     @param[in] cellCb      Cell context
 *     @param[in] nxTransId   RRC user transaction ID
 *     @param[in] cfgElms     Configuration element (UE or Logical channel)
 *     @param[in] status      Configuration confirm status
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlPhyUECfgCfm
(
NhId            *nhIdentity,
NhCellCb        *cellCb,
NhTransId        nxTransId,
U8               cfgElms,
U8               status
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlPhyUECfgCfm(nhIdentity, cellCb, nxTransId, cfgElms, status)
NhId            *nhIdentity;
NhCellCb        *cellCb;
NhTransId        nxTransId;
U8               cfgElms;
U8               status;
#endif /* ANSI */
{
   TransCb            *transCtrlBlk = NULLP;
   U8                 cgmCfmStatus = NH_CFG_SUCCESS;
   NhActvUeCb         *ueCb = NULLP;
   NhCellAndUeCb      cellAndUeCb;

   TRC2(nhLIMHndlPhyUECfgCfm)
   /* Review the code - it needs to be corrected YOG */

   RLOG0(L_DEBUG, "nhLIMHndlPhyUECfgCfm");

   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, nhIdentity->ueId)))
   {
      RLOG0(L_ERROR, "NULL UE CB");
      RETVALUE(RFAILED); 
   }
   /* Find trans node in the list with specified transId value */
   if (NULLP == (transCtrlBlk = nhUTLGetTransCbFrmNxTransId(ueCb, nxTransId)))
   {
      RLOG0(L_ERROR, "transId doesnt exist");
      RETVALUE(RFAILED);
   }

   /* Drop Undesired/Duplicate Cfg Cfm, treat it as successful case */
   if (transCtrlBlk->numOfPhyCfms <= 0)
   {
      RLOG0(L_DEBUG, "duplicate Phy Cfg Cfm RCVD ");
      RETVALUE(ROK);
   }

   if (cfgElms == NH_CTF_UE_CFG)
   {
      if (transCtrlBlk->phyMapInfo.status == NH_STATUS_USED) 
      {
         if (status != CTF_CFG_CFM_OK) 
         {
            transCtrlBlk->phyMapInfo.cfmStatus = CTF_CFG_CFM_NOK;
            /* Update General Statistics for lower layer failure */
            nhUTLUpdGenSts(NH_STS_CAUSE_LOWER_LYR_FAIL);
         }
         else
         {
            transCtrlBlk->phyMapInfo.cfmStatus = CTF_CFG_CFM_OK;
         }
      }
      transCtrlBlk->numOfPhyCfms--;
   }   

   /* If all cfms are received, then set cfmStatus for layer */
   if (transCtrlBlk->numOfPhyCfms <= 0)
   {
      /* Looping twice, once for UE and once for logical 
       * channel configuration confirm handling.
       */
      if (((transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].status == 
                                   NH_STATUS_USED) &&
         (transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].cfmStatus != 
                                   CTF_CFG_CFM_OK)) 
         || ((transCtrlBlk->phyMapInfo.status == NH_STATUS_USED)
           && (transCtrlBlk->phyMapInfo.cfmStatus != CTF_CFG_CFM_OK)))
      {
         cgmCfmStatus = NH_CFG_FAILED;
      }

      cellAndUeCb.ueCb = ueCb;
      cellAndUeCb.cellCb = cellCb;

       /* Call function to report Cfm to CGM */
      if (ROK != nhLIMRptCfgCfm(nhIdentity, &cellAndUeCb, transCtrlBlk, 
                                cgmCfmStatus, NH_PHY_CONFIGURED))
      {
         RLOG0(L_ERROR, "nhLIMRptCfgCfm failed ");
         RETVALUE(RFAILED);
      }
   }  /* if (!transCtrlBlk->numOfPhyCfms) */

   RETVALUE(ROK);
} /* nhLIMHndlPhyUECfgCfm */

/**
 *
 * @brief Handler to handle configuration confirm from PHY layer.
 * @details
 *
 *       This function is used to handle configuration confirm
 *            from PHY layer.
 *            
 *     @param[in] transId   Configuration transaction ID
 *     @param[in] status    Configuration status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlPhyCfgCfm
(
CtfCfgTransId transId, 
U8 status             
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlPhyCfgCfm(transId, status)
CtfCfgTransId transId; 
U8 status;            
#endif /* ANSI */
{
   NhCellCb             *cellCb = NULLP;
   NhUstaDgn            dgn;       /* Alarm Diagnostics */
   NhId                 nhIdentity;
   NhTransId            nxTransId;
   U8                   phyActionBitMask = 0x00;
   U8                   cfgElms = 0x00;
   U8                   *transIdPtr = NULLP;

   TRC2(nhLIMHndlPhyCfgCfm);

   RLOG0(L_DEBUG, "nhLIMHndlPhyCfgCfm");

   transIdPtr = transId.trans;
   
   NH_SET_ZERO (&nhIdentity, sizeof(NhId));

   /* First byte is NHU_CELL_ID/NHU_UE_ID */
   NH_MEM_COPY((U8*)&nhIdentity.cellId, (CONSTANT U8*)transIdPtr, sizeof(U16));
   transIdPtr += sizeof(U16);
   NH_MEM_COPY((U8*)&nhIdentity.ueId, (CONSTANT U8*)transIdPtr, sizeof(U16));
   transIdPtr += sizeof(U16);

   NH_MEM_COPY((U8*)&nxTransId, (CONSTANT U8*)transIdPtr, sizeof(NhTransId));
   transIdPtr += sizeof(NhTransId);
   NH_MEM_COPY((U8*)&cfgElms, (CONSTANT U8*)transIdPtr, sizeof(U8));
   transIdPtr += sizeof(U8);
   NH_MEM_COPY((U8*)&phyActionBitMask, (CONSTANT U8*)transIdPtr, sizeof(U8));

   RLOG_ARG2(L_DEBUG, DBG_TRNSID,nxTransId,"status[%d] bitmask[%d]",
      status, phyActionBitMask);

   /* If transId is special reserved ID, then transCtrlBlk must have got  
    * deleted, so handle confirm gracefully.
    */
   if (nxTransId == NH_RESERVED_TRANS_ID)
   {
      /* If any of the elements had failed at PHY, then 
       * status must not be equal to 0. 
       */
      if (status != CTF_CFG_CFM_OK)
      {
         /* Raise a critical alarm to the layer manager */
         NH_SET_ZERO (&dgn, sizeof(dgn));
         dgn.type = LNH_USTA_DGNVAL_CELLUEID;
         dgn.u.cellUeId.ueId = nhIdentity.ueId;
         dgn.u.cellUeId.cellId = nhIdentity.cellId;
         nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LNH_EVENT_PHY_RVRT_FAIL,
                       LCM_CAUSE_UNKNOWN, &dgn);
      }
      /* Return from here */
      RETVALUE(ROK);
   }
   if (NULLP == (cellCb = nhDBMGetCellCb(nhIdentity.cellId)))
   {
      RLOG0(L_ERROR, "NULLP Cell Cb");
      RETVALUE(RFAILED);
   }

   if (nhIdentity.ueId > 0)
   {
      if (ROK != (nhLIMHndlPhyUECfgCfm(&nhIdentity, cellCb, nxTransId, cfgElms,
                                       status)))
      {
         RLOG0(L_ERROR, "nhLIMHndlPhyUECfgCfm failed ");
         RETVALUE(RFAILED); 
      }
   }
   else
   {
      if (ROK != (nhLIMHndlPhyCellCfgCfm(&nhIdentity, cellCb, cfgElms, 
                                         status)))
      {
         RLOG0(L_ERROR, "nhLIMHndlPhyCellCfgCfm failed ");
         RETVALUE(RFAILED); 
      }
   }
   RETVALUE(ROK);
}  /* end of nhLIMHndlPhyCfgCfm */

#endif /* RRC_PHY_CFG_SUPP */
/**
 *
 * @brief Handler to handle configuration confirm from MAC layer.
 * @details
 *
 *       This function is used to handle configuration confirm
 *            from MAC layer.
 *            
 *     @param[in] transId   Configuration transaction ID
 *     @param[in] status    Configuration status
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlMacCfgCfm
(
CrgCfgTransId transId, 
U8 status             
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlMacCfgCfm(transId, status)
CrgCfgTransId transId; 
U8 status;            
#endif /* ANSI */
{
   NhCellCb             *cellCb = NULLP;
   NhUstaDgn            dgn;       /* Alarm Diagnostics */
   NhId                 nhIdentity;
   NhTransId            nxTransId;
   U8                   macActionBitMask = 0x00;
   U8                   cfgElms = 0x00;
   U8                   *transIdPtr = NULLP;
   U8                   lclId;
   U8                   dir;

   TRC2(nhLIMHndlMacCfgCfm);

   RLOG0(L_DEBUG, "nhLIMHndlMacCfgCfm");

   transIdPtr = transId.trans;
   
   /* First byte is NHU_CELL_ID/NHU_UE_ID */
   NH_MEM_COPY((U8*)&nhIdentity.cellId, (CONSTANT U8*)transIdPtr, sizeof(U16));
   transIdPtr += sizeof(U16);
   NH_MEM_COPY((U8*)&nhIdentity.ueId, (CONSTANT U8*)transIdPtr, sizeof(U16));
   transIdPtr += sizeof(U16);

   NH_MEM_COPY((U8*)&nxTransId, (CONSTANT U8*)transIdPtr, sizeof(NhTransId));
   transIdPtr += sizeof(NhTransId);
   NH_MEM_COPY((U8*)&cfgElms, (CONSTANT U8*)transIdPtr, sizeof(U8));
   transIdPtr += sizeof(U8);
   NH_MEM_COPY((U8*)&macActionBitMask, (CONSTANT U8*)transIdPtr, sizeof(U8));
   transIdPtr += sizeof(U8);
   NH_MEM_COPY((U8*)&lclId, (CONSTANT U8*)transIdPtr, sizeof(U8));
   transIdPtr += sizeof(U8);
   NH_MEM_COPY((U8*)&dir, (CONSTANT U8*)transIdPtr, sizeof(U8));

   RLOG_ARG2(L_DEBUG, DBG_TRNSID,nxTransId,"status[%d] bitmask[%d]",
      status, macActionBitMask);

   /* If transId is special reserved ID, then transCtrlBlk must have got  
    * deleted, so handle confirm gracefully.
    */
   if (nxTransId == NH_RESERVED_TRANS_ID)
   {
      /* If any of the elements had failed at MAC, then 
       * status must not be equal to 0. 
       */
      if (status != CRG_CFG_CFM_OK)
      {
         /* Raise a critical alarm to the layer manager */
         NH_SET_ZERO (&dgn, sizeof(dgn));
         dgn.type = LNH_USTA_DGNVAL_CELLUEID;
         dgn.u.cellUeId.ueId = nhIdentity.ueId;
         dgn.u.cellUeId.cellId = nhIdentity.cellId;
         nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LNH_EVENT_MAC_RVRT_FAIL,
                       LCM_CAUSE_UNKNOWN, &dgn);
      }
      /* Return from here */
      RETVALUE(ROK);
   }
   if (NULLP == (cellCb = nhDBMGetCellCb(nhIdentity.cellId)))
   {
      RLOG0(L_ERROR, "NULLP Cell Cb");
      RETVALUE(RFAILED);
   }

   if (nhIdentity.ueId > 0)
   {
      if (ROK != (nhLIMHndlMacUECfgCfm(&nhIdentity, cellCb, nxTransId, lclId, 
                                       cfgElms, status, dir)))
      {
         RLOG0(L_ERROR, "nhLIMHndlMacUECfgCfm failed ");
         RETVALUE(RFAILED); 
      }
   }
   else
   {
      if (ROK != (nhLIMHndlMacCellCfgCfm(&nhIdentity, cellCb, cfgElms, 
                                         status, lclId)))
      {
         RLOG0(L_ERROR, "nhLIMHndlMacCellCfgCfm failed ");
         RETVALUE(RFAILED); 
      }
   }
   RETVALUE(ROK);
}  /* end of nhLIMHndlMacCellCfgCfm */


/**
 *
 * @brief Handler to send configuration to lower layers, that is, RLC, 
 *        MAC, and PDCP.
 * @details
 *
 *       This function is used to send configuration   
 *            to lower layers, that is, RLC, MAC, and PDCP.
 *            
 *     @param[in] cellAndUeCb    Cell and UE context
 *     @param[in] transCtrlBlk   Transaction context
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMSendCfgMsg
(
NhCellAndUeCb *cellAndUeCb,
TransCb   *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhLIMSendCfgMsg(cellAndUeCb, transCtrlBlk)
NhCellAndUeCb *cellAndUeCb;
TransCb    *transCtrlBlk;
#endif /* ANSI */
{
   S16         ret = RFAILED; /*nh002.301:118385: HO issue*/
   NhId  nhIdentity;

   TRC2(nhLIMSendCfgMsg)

   RLOG0(L_DEBUG, "sending Cfg msg ");

   /* Start UE Lower Layer Cfg timer */
   nhStartLLyrCfgTmr(cellAndUeCb, transCtrlBlk->nxTransId);

#ifdef RRC_PHY_CFG_SUPP
   /* Check if PHY needs to be configured */
   if (transCtrlBlk->cfgMask & NH_PHY_CONFIGURED)
   {
      if (ROK != nhLIMSendPhyCfg(cellAndUeCb->ueCb, transCtrlBlk))
      {
         /* Stop lower Layer Cfg Timer */
         nhStopLLyrCfgTmr(cellAndUeCb, transCtrlBlk->nxTransId);

         RLOG0(L_ERROR, "nhLIMSendPhyCfg failed ");
         RETVALUE(RFAILED);
      }
   }
#endif /* RRC_PHY_CFG_SUPP */

   /* Check if MAC needs to be configured */
   if (transCtrlBlk->cfgMask & NH_MAC_CONFIGURED)
   {
      if (ROK != nhLIMSendMacCfg(cellAndUeCb->ueCb, transCtrlBlk))
      {
         /* Stop lower Layer Cfg Timer */
         nhStopLLyrCfgTmr(cellAndUeCb, transCtrlBlk->nxTransId);

         RLOG0(L_ERROR, "nhLIMSendMacCfg failed ");
         RETVALUE(RFAILED);
      }
   }
   if((transCtrlBlk->cfgMask & NH_PDCP_CONFIGURED) &&
		   (transCtrlBlk->cause == NH_TRANSCB_DATA_FWD_HO))
   {
      RLOG0(L_DEBUG, "Data Forward");
	   /* Send Data Forwarding Request to PDCP */
	   if(ROK != nhPIMSendStartPdcpDataFwdReq((cellAndUeCb->ueCb),0))
	   {
		   /* Stop lower Layer Cfg Timer */
		   nhStopLLyrCfgTmr(cellAndUeCb, transCtrlBlk->nxTransId);

		   RLOG0(L_ERROR, "nhLIMSendPdcpDataFwdReq failed");
		   RETVALUE(RFAILED);
	   } 
	   RETVALUE(ROK);
   }
   /* Handlign Data Resume request */
   if((transCtrlBlk->cfgMask & NH_PDCP_CONFIGURED) &&
		   (transCtrlBlk->cause == NH_TRANSCB_DATA_RESUME))
   {
	   /* Send Data Resume Request to PDCP */
	   if(ROK != nhPIMSendPdcpDatResumeReq(cellAndUeCb->ueCb, transCtrlBlk->transId))
	   {
		   /* Stop lower Layer Cfg Timer */
		   nhStopLLyrCfgTmr(cellAndUeCb, transCtrlBlk->nxTransId);

		   RLOG0(L_ERROR, "nhLIMSendPdcpDatResumeReq failed ");
		   RETVALUE(RFAILED);
	   } 
	   RETVALUE(ROK);
   }
   /* ReEstablishement fix ccpu00125241 */
   if (!(transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RESTB || 
         transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RESTB_SRB1))
   {
       /*nh002.301:118385: HO issue: Filling nhIdentity*/
       nhIdentity.cellId = cellAndUeCb->cellCb->cellId;
       nhIdentity.ueId = cellAndUeCb->ueCb->ueCbKey.crnti;
       if (transCtrlBlk->cause == NH_TRANSCB_SDU_STA_RESTAB) 
       {
          ret = nhPIMSndSduStaReq(nhIdentity , transCtrlBlk, cellAndUeCb, NH_CFG_SUCCESS);
          if (ret != ROK)
            RETVALUE(ret);
       } /* (transCtrlBlk->cause == NH_TRANSCB_SDU_STA_RESTAB) */
       /* Check if RLC needs to be configured */
       /* Check if RLC needs to be configured */
       if (transCtrlBlk->cfgMask & NH_RLC_CONFIGURED)
       {
           /* directly call PIM */
           if (ROK != nhPIMSendRlcCfg(cellAndUeCb->ueCb, transCtrlBlk))
           {
              /* Stop lower Layer Cfg Timer */
              nhStopLLyrCfgTmr(cellAndUeCb, transCtrlBlk->nxTransId);
              RLOG0(L_ERROR, "nhLIMSendRlcCfg failed ");
              RETVALUE(RFAILED);
           }
       }
   }

   /*nh002.301:118385:HO issue: dont send pdcp cfg in case of SDU Status Re-establish*/
   /* Check if PDCP needs to be configured */
   if ((transCtrlBlk->cfgMask & NH_PDCP_CONFIGURED) &&
       (transCtrlBlk->cause != NH_TRANSCB_SDU_STA_RESTAB))
   {
      /* Directly call PIM */
      if (ROK != nhPIMSendPdcpCfg(cellAndUeCb->ueCb, transCtrlBlk))
      {
         /* Stop lower Layer Cfg Timer */
         nhStopLLyrCfgTmr(cellAndUeCb, transCtrlBlk->nxTransId);

         RLOG0(L_ERROR, "nhLIMSendPdcpCfg failed ");
         RETVALUE(RFAILED);
      }
   }

         /* nh003.301:  Fix for ReEstablishment the 125241 */
   if ( (transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RB_RESTB || 
            transCtrlBlk->ordrdCfg.actionMask & NH_ORD_CFG_RESTB_SRB1))
   {
      /* MS_WORKAROUND for HO ccpu00118385 */
      nhIdentity.cellId = cellAndUeCb->cellCb->cellId;
      nhIdentity.ueId = cellAndUeCb->ueCb->ueCbKey.crnti;
      if (transCtrlBlk->cause == NH_TRANSCB_SDU_STA_RESTAB) 
      {
         ret = nhPIMSndSduStaReq(nhIdentity , transCtrlBlk, cellAndUeCb, NH_CFG_SUCCESS);
         if (ret != ROK)
            RETVALUE(ret);
      } /* (transCtrlBlk->cause == NH_TRANSCB_SDU_STA_RESTAB) */
      /* Check if RLC needs to be configured */
      if (transCtrlBlk->cfgMask & NH_RLC_CONFIGURED)
      {
         /* directly call PIM */
         if (ROK != nhPIMSendRlcCfg(cellAndUeCb->ueCb, transCtrlBlk))
         {
            /* Stop lower Layer Cfg Timer */
            nhStopLLyrCfgTmr(cellAndUeCb, transCtrlBlk->nxTransId);

            RLOG0(L_ERROR, "nhLIMSendRlcCfg failed ");
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ROK);
}  /* end of nhLIMSendCfgMsg */

/**
 *
 * @brief Handler to send configuration to MAC.
 * @details
 *
 *      This function is used to send configuration   
 *            to MAC.
 *            
 *     @param[in] ueCb           UE context
 *     @param[in] transCtrlBlk   Transaction context
 *     @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMSendMacCfg
(
NhActvUeCb *ueCb,
TransCb    *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhLIMSendMacCfg(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;
TransCb    *transCtrlBlk;
#endif /* ANSI */
{
   S16 ret = RFAILED;
   U8  addCfgType = 0x00;  /* bit mask for mac cfgtype */
   U8  modCfgType = 0x00;  /* bit mask for mac cfgtype */
   U8  rlsCfgType = 0x00;  /* bit mask for mac cfgtype */
   U8  rstCfgType = 0x00;  /* bit mask for mac reset cfgtype */

   TRC2(nhLIMSendMacCfg)

   RLOG0(L_DEBUG, "sending Cfg to MAC ");

   /* Reset request is issued during RRC Connection re-establishment scenario
    * The old crnti is used to issue this request*/
   if ((NH_STATUS_USED == transCtrlBlk->rstTbl[NH_RESET_MAPTBL_IDX_MAC].status)
   &&(NH_MAC_CFG_RESET == transCtrlBlk->rstTbl[NH_RESET_MAPTBL_IDX_MAC].action))
   {
      rstCfgType |= NH_CRG_UE_MAC_RST;
   }
   if(rstCfgType > 0)
   {
      ret = nhPIMSendMacResetCfg(ueCb, transCtrlBlk, rstCfgType);
      if (ROK != ret)
      {
         RLOG0(L_ERROR,"nhPIMSendMacResetCfg failed.");
         RETVALUE(RFAILED);
      }
   }
   /* If Cfg to be added in MAC */
   if ((transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToAdd > 0) ||
         (transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToAdd > 0))
   {
      addCfgType |= NH_CRG_LOGCH_MAP;
   }

   if ((NH_STATUS_USED == 
           transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].status) && 
       (NH_MAC_CFG_ADD == transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].action))
   {
      addCfgType |= NH_CRG_UE_CFG;
   }

   if(addCfgType > 0)
   {
      ret = nhPIMSendMacAddCfg(ueCb, transCtrlBlk, addCfgType);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, "nhPIMSendMacAddCfg failed.");
         RETVALUE(RFAILED);
      }
   }

   if ((transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToMod > 0) ||
         (transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToMod > 0))
   {
      modCfgType |= NH_CRG_LOGCH_MAP;
   }

   if ((NH_STATUS_USED == 
           transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].status) &&
       (NH_MAC_CFG_MODIFY  == 
           transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_MAC].action))
   {
      modCfgType |= NH_CRG_UE_CFG;
   }

   if(modCfgType > 0)
   {
      ret = nhPIMSendMacModCfg(ueCb, transCtrlBlk, modCfgType);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, "nhLIMSendMacModCfg failed.");
         RETVALUE(RFAILED);
      }
   }

   if ((transCtrlBlk->macLchMapTbl[NH_DL_LC].logchsToRls > 0) ||
         (transCtrlBlk->macLchMapTbl[NH_UL_LC].logchsToRls > 0))
   {
      rlsCfgType |= NH_CRG_LOGCH_MAP;
   }

   if(rlsCfgType > 0)
   {
      ret = nhPIMSendMacRlsCfg(ueCb, transCtrlBlk, rlsCfgType);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, "nhLIMSendMacModCfg failed.");
         RETVALUE(RFAILED);
      }  
   }

   RETVALUE(ROK);
}  /* end of nhLIMSendMacCfg */

#ifdef RRC_PHY_CFG_SUPP
/**
 *
 * @brief Handler to send configuration to PHY.
 * @details
 *
 *      This function is used to send configuration   
 *            to PHY.
 *            
 *     @param[in] ueCb           UE context
 *     @param[in] transCtrlBlk   Transaction context
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMSendPhyCfg
(
NhActvUeCb *ueCb,
TransCb    *transCtrlBlk
)
#else /* ANSI */
PUBLIC S16 nhLIMSendPhyCfg(ueCb, transCtrlBlk)
NhActvUeCb *ueCb;
TransCb    *transCtrlBlk;
#endif /* ANSI */
{
   S16 ret = RFAILED;

   TRC2(nhLIMSendPhyCfg)

   RLOG0(L_DEBUG, "sending Cfg to PHY ");

   if ((NH_STATUS_USED == 
           transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].status) && 
       (NH_PHY_CFG_UEID_CHNG == 
           transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].action))
   {
      ret = nhPIMSendPhyUeIdChgReq(ueCb, transCtrlBlk);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, "nhPIMSendPhyUeIdChgReq failed.");
         RETVALUE(RFAILED);
      }
   }

   if ((NH_STATUS_USED == transCtrlBlk->phyMapInfo.status) && 
         (NH_PHY_CFG_ADD == transCtrlBlk->phyMapInfo.action))
   {
      ret = nhPIMSendPhyAddCfg(ueCb, transCtrlBlk);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, "nhPIMSendPhyAddCfg failed.");
         RETVALUE(RFAILED);
      }
   }

   if ((NH_STATUS_USED == transCtrlBlk->phyMapInfo.status) &&
         (NH_PHY_CFG_MODIFY  == transCtrlBlk->phyMapInfo.action))
   {
      ret = nhPIMSendPhyModCfg(ueCb, transCtrlBlk);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, "nhLIMSendPhyModCfg failed.");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}  /* end of nhLIMSendPhyCfg */


/**
 *
 * @brief Handler to revert configuration of PHY layer, if any layer 
 *        failed to configure.
 *
 * @details
 *
 *       This function is used to revert configuration 
 *            of PHY layer, if any layer failed to configure.
 *            
 *     @param[in] nhIdentity     RRC ID (NhId)
 *     @param[in] ueCb           UE context
 *     @param[in] transCtrlBlk   Transaction context
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMHndlRvrtPhyCfg
(
NhId *nhIdentity,
NhActvUeCb *ueCb,  
TransCb *transCtrlBlk  
)
#else /* ANSI */
PUBLIC S16 nhLIMHndlRvrtPhyCfg(nhIdentity, ueCb, transCtrlBlk)
NhId *nhIdentity;
NhActvUeCb *ueCb;
TransCb *transCtrlBlk; 
#endif /* ANSI */
{
   S16      ret;

   TRC2(nhLIMHndlRvrtPhyCfg)

   UNUSED(nhIdentity);

   RLOG0(L_DEBUG, "In nhLIMHndlRvrtPhyCfg ");

   /* Set transId in transCtrlBlk to reserved trans ID, 
   * do not care for cfg confirms for these requests.
    */
   if ((NH_STATUS_USED == 
          transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].status) && 
       (NH_PHY_CFG_UEID_CHNG == 
          transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].action) &&
       (CTF_CFG_CFM_OK == 
          transCtrlBlk->ueMapTbl[NH_UE_MAPTBL_IDX_PHY].cfmStatus))
   {
      ret = nhPIMRvrtPhyUeIdChgReq(ueCb, transCtrlBlk);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, " nhPIMRvrtPhyUeIdChgReq failed.");
         RETVALUE(RFAILED);
      }
   }


   /* If Cfg was added in PHY */

   if ((NH_STATUS_USED == transCtrlBlk->phyMapInfo.status) && 
         (NH_PHY_CFG_ADD == transCtrlBlk->phyMapInfo.action) &&
         (CTF_CFG_CFM_OK == transCtrlBlk->phyMapInfo.cfmStatus))
   {
      ret = nhPIMRvrtPhyAddCfg(ueCb, transCtrlBlk);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, " nhPIMRvrtPhyAddCfg failed.");
         RETVALUE(RFAILED);
      }
   }

   if ((NH_STATUS_USED == transCtrlBlk->phyMapInfo.status) &&
         (NH_PHY_CFG_MODIFY == transCtrlBlk->phyMapInfo.action) &&
         (CTF_CFG_CFM_OK == transCtrlBlk->phyMapInfo.cfmStatus))
   {
      ret = nhPIMRvrtPhyModCfg(ueCb, transCtrlBlk );
      if (ROK != ret)
      {
         RLOG0(L_ERROR, "nhPIMRvrtPhyModCfg failed.");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
} /* end of nhLIMHndlRvrtPhyCfg */ 

#endif /* RRC_PHY_CFG_SUPP */


/**
 *
 * @brief Handler to release complete configuration of lower layers.
 * @details
 *
 *      This function is used to release complete  
 *             configuration of lower layers.
 *            
 *     @param[in] *ueCb   UE context
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMRlsLowerLyrCfg
(
NhActvUeCb *ueCb   
)
#else /* ANSI */
PUBLIC S16 nhLIMRlsLowerLyrCfg(ueCb)
NhActvUeCb *ueCb; 
#endif /* ANSI */
{
   S16      ret = RFAILED;

   TRC2(nhLIMRlsLowerLyrCfg)

   RLOG0(L_DEBUG, "complete release of lower lyr cfg ");

   ret = nhPIMRlcPdcpMacRlsCfg(ueCb);
   if (ROK != ret)
   {
      RLOG0(L_ERROR, "nhPIMRlcPdcpMacRlsCfg failed.");
      RETVALUE(RFAILED);
   }

#ifdef RRC_PHY_CFG_SUPP
   ret = RFAILED;
   /* Release PHY if it was configured */
   if (ueCb->crntCfg.bitMskPres & NH_CRNT_CFG_PHYCH)
   {
      ret = nhPIMSendPhyRlsCfg(ueCb);
      if (ROK != ret)
      {
        RLOG0(L_ERROR, "nhLIMSendPhyRlsCfg failed.");
        RETVALUE(RFAILED);
      }  
   }
#endif /* RRC_PHY_CFG_SUPP */

   RETVALUE(ROK);
} /* end of nhLIMRlsLowerLyrCfg */


/**
 *
 * @brief Handler to send UNBIND request to various layers.
 * @details
 *
 *       This function is used to send UNBIND request 
 *            to various layers.
 *            
 *     @param[in] sapElmId   Type of lower SAP (KWU, CKW, CPJ, PJU, and CRG)
 *     @param[in] lSapCb     Lower SAP context
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMSendUbndReq
(
U8       sapElmId,
NhLSapCb *lSapCb
)
#else /* ANSI */
PUBLIC S16 nhLIMSendUbndReq(sapElmId, lSapCb)
U8       sapElmId;
NhLSapCb *lSapCb;
#endif /* ANSI */
{
   S16      ret = RFAILED;

   TRC2(nhLIMSendUbndReq)

   RLOG0(L_DEBUG, "nhLIMSendUbndReq");

   switch(sapElmId)
   {
      case STNHKWUSAP:       /* RRC's RLC user plane SAPs */ 
         ret = NhLiKwuUbndReq(&lSapCb->pst,lSapCb->spId, 0);
         break;

      case STNHCKWSAP:       /* RRC's RLC control plane SAPs */
         ret = NhLiCkwUbndReq(&lSapCb->pst,lSapCb->spId, 0);
         break;

      case STNHCRGSAP:        /* RRC's MAC control plane SAPs */
         ret = NhLiCrgUbndReq(&lSapCb->pst,lSapCb->spId, 0);
         break;

      case STNHCPJSAP:       /* RRC's PDCP control plane SAPs */
         ret = NhLiCpjUbndReq(&lSapCb->pst,lSapCb->spId, 0);
         break;

      case STNHPJUSAP:       /* RRC's PDCP user plane SAPs */
         ret = NhLiPjuUbndReq(&lSapCb->pst,lSapCb->spId, 0);
         break;

#ifdef RRC_PHY_CFG_SUPP 
      case STNHCTFSAP:       /* RRC's PHY user plane SAPs */
         ret = NhLiCtfUbndReq(&lSapCb->pst,lSapCb->spId, 0);
         break;
#endif /* RRC_PHY_CFG_SUPP */

      default:
         RLOG0(L_ERROR, " Invalid sapElmId");
         RETVALUE(RFAILED);
   }
   
   RETVALUE(ret); 
}  /* end of nhLIMSendUbndReq */


/**
 *
 * @brief Handler to send BIND request to various layers.
 * 
 * @details
 *       This function is used to send BIND request 
 *            to various layers.
 *            
 *     @param[in] sapElmId   Type of lower SAP (KWU, CKW, CPJ, PJU, and CRG)
 *     @param[in] lSapCb     Lower SAP context
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMSendBndReq
(
U16       sapElmId,
NhLSapCb *lSapCb
)
#else /* ANSI */
PUBLIC S16 nhLIMSendBndReq(sapElmId, lSapCb)
U16       sapElmId; 
NhLSapCb *lSapCb;
#endif /* ANSI */
{
   S16      ret = RFAILED;

   TRC2(nhLIMSendBndReq)

   RLOG1(L_DEBUG, " nhLIMSendBndReq ElmId(%d)",sapElmId);

   switch(sapElmId)
   {
      case STNHKWUSAP:       /* RRC's RLC user plane SAP */ 
         ret = NhLiKwuBndReq(&lSapCb->pst, lSapCb->suId, lSapCb->spId);
         break;

      case STNHCKWSAP:       /* RRC's RLC control plane SAPs */ 
         ret = NhLiCkwBndReq(&lSapCb->pst, lSapCb->suId, lSapCb->spId);
         break;

      case STNHCRGSAP:        /* RRC's MAC control place SAPs */
         ret = NhLiCrgBndReq(&lSapCb->pst, lSapCb->suId, lSapCb->spId);
         break;

      case STNHCPJSAP:       /* RRC's PDCP control plane SAPs */
         ret = NhLiCpjBndReq(&lSapCb->pst, lSapCb->suId, lSapCb->spId);
         break;

      case STNHPJUSAP:       /* RRC's PDCP user plane SAPs */
         ret = NhLiPjuBndReq(&lSapCb->pst, lSapCb->suId, lSapCb->spId);
         break;

#ifdef RRC_PHY_CFG_SUPP
      case STNHCTFSAP:       /* RRC's PHY user plane SAPs */
         ret = NhLiCtfBndReq(&lSapCb->pst, lSapCb->suId, lSapCb->spId);
         break;
#endif /* RRC_PHY_CFG_SUPP */

      default:
         RLOG0(L_ERROR, " Invalid sapElmId");
         RETVALUE(RFAILED);
   }
   
   RETVALUE(ret); 
}  /* end of nhLIMSendBndReq */

/**
 *
 *     @brief This function is used to receive Status indication  
 *             from PDCP layer for protocol errors.
 *            
 * @details This function receives the Status indication from PDCP layer 
 *  for invalid PDUs(integrity protection or ciphering failed).
 *
 *            
 *     @param[in] pst        Post structure
 *     @param[in] suId       Lower SAP ID
 *     @param[in] pdcpId     PDCP ID containing UE ID and PDCP instance ID   
 *     @param[in] staInd     Cause of alarm from PDCP
 *     @param[in] buf        Buffer containing the message
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 NhLiPjuStaInd
(
Pst *pst,           
SuId suId,         
CmLtePdcpId *pdcpId, 
PjuStaIndInfo *staInd,
Buffer   *buf 
)
#else /* ANSI */
PUBLIC S16 NhLiPjuStaInd(pst, suId, pdcpId, staInd, buf)
Pst *pst;             
SuId suId;           
CmLtePdcpId *pdcpId;
PjuStaIndInfo *staInd; 
Buffer *buf; 
#endif /* ANSI */
{
   S16   ret = RFAILED;
   NhId  nhIdentity;
   NhLSapCb *pjuSapCb = NULLP;

   TRC2(NhLiPjuStaInd)

   UNUSED(pst);

   if (NH_CHK_INPUT_PARAM(pdcpId))
   {
      RLOG0(L_FATAL, "GOT NULL pdcpId");
      if (!(NH_CHK_INPUT_PARAM(staInd)))
      {
         NH_FREE(NH_REG, NH_POOL, staInd, sizeof(PjuStaIndInfo));
      }
      if (!(NH_CHK_INPUT_PARAM(buf)))
      {
         NH_FREEMBUF(buf);
      }
      RETVALUE(RFAILED);
   }
   if (NH_CHK_INPUT_PARAM(staInd))
   {
      RLOG0(L_FATAL, "GOT NULL staInd");
      if (!(NH_CHK_INPUT_PARAM(buf)))
      {
         NH_FREEMBUF(buf);
      }
      RETVALUE(RFAILED);
   }

   RLOG2(L_DEBUG, "suId[%d status[%d]", suId, staInd->cause);

   pjuSapCb = nhUTLFindLlyrSapFrmSuId(STNHPJUSAP, suId);

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (pjuSapCb == NULLP)
   {
      RLOG0(L_ERROR, "INVALID SAP ID in NhLiPjuStaInd");
      NH_FREE(NH_REG, NH_POOL, staInd, sizeof(PjuStaIndInfo));
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   NH_MAP_PDCPID_TO_NHID(nhIdentity, pdcpId);

   RLOG2(L_DEBUG, "Received StaInd from PDCP suId[%d] rbId[%d]",
         suId, nhIdentity.rbId); 
   
   if (nhIdentity.isPdcpId == TRUE)
   {
      /* Call LIM module function to handle status indication  */
      ret = nhLIMHndlStatusInd(&nhIdentity, staInd, buf);
      if (ROK != ret)
      {
         RLOG0(L_ERROR, "nhLIMHndlStatusInd failed.");
      }
   }
   else
   {
      RLOG0(L_DEBUG, "not a pdcp ID");
   }

   if (buf != NULLP)
   {
      NH_FREEMBUF(buf);
   }

   RETVALUE(ret);
}  /* end of NhLiPjuStaInd */

/**
 *
 * @brief Handler to handle Count Confirm from PDCP layer.
 * @details
 *
 *       This function is used to handle Count confirm
 *            from PDCP layer for the count request.
 *            
 *     @param[in] countCfmInfo COUNT information of DRBs sent by CPJ
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 *
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlCountCfm
(
CpjCountCfmInfo *countCfmInfo
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlCountCfm(countCfmInfo)
CpjCountCfmInfo *countCfmInfo;
#endif /* ANSI */
{
   NhuDRBCountInfoLst   *nhuDRBCountInfoLst = NULLP;
   NhuDrbCountInfo      *nhuDrbCountInfo = NULLP;
   NhuErrIndSdus        errIndSdus;
   Mem                  sMem;
   U8                   rbIndex;

   TRC2(nhLIMHndlCountCfm)

   RLOG_ARG2(L_DEBUG, DBG_TRNSID,countCfmInfo->transId, "ueId[%d] cellId[%d]",
      countCfmInfo->ueId, countCfmInfo->cellId);

   /* Alocate memory for nhuDRBCountInfoLst */
         /* Allocate memory for PDCP Status confirm structure */
  sMem.region = nhCb.init.region;
  sMem.pool = nhCb.init.pool;

   NH_ALLOCEVNT((Ptr *)&nhuDRBCountInfoLst, sizeof(NhuDRBCountInfoLst), &sMem);
   if (nhuDRBCountInfoLst == NULLP)
   {
      RLOG0(L_FATAL, "Memory Allocation failed for storing DRB Count info List.");
      RETVALUE(RFAILED);
   }

   /* fill NhuHdr structure */
   nhuDRBCountInfoLst->countHdr.ueId = countCfmInfo->ueId;
   nhuDRBCountInfoLst->countHdr.cellId = countCfmInfo->cellId;
   nhuDRBCountInfoLst->countHdr.transId = countCfmInfo->transId;
   nhuDRBCountInfoLst->numDrbs = countCfmInfo->numRb;

   /* If status is not OK then update only the status same in
      nhuDRBCountInfoLst. If status is OK, then update all inforamtion*/
   if (countCfmInfo->status != ROK)
   {
      RLOG0(L_DEBUG, "Count Confirm Failed ");
      /* flagIsStsSuccess = FALSE; */
      nhuDRBCountInfoLst->status = FALSE;
   }
   else
   {
      nhuDRBCountInfoLst->status = TRUE;
      if (countCfmInfo->numRb >= NHU_MAX_DRB)
      {
         RLOG0(L_ERROR,"Number of DRBs out of range");
         NH_FILL_NHU_ERROR(errIndSdus.sdu, NHU_ERRTYPE_INV_SDU_PARAM,
                                          NHU_ERRCAUSE_MAX_DRB);
         NH_FREEEVNT(nhuDRBCountInfoLst);
         RETVALUE(RFAILED);
      }

      NH_GET_MEM(nhuDRBCountInfoLst,
                  countCfmInfo->numRb * sizeof(NhuDrbCountInfo),
                  &nhuDrbCountInfo);

      if (nhuDrbCountInfo == NULLP)
      {
         RLOG0(L_FATAL, "Memory Allocation failed for storing DRB Count info.");
         NH_FREEEVNT(nhuDRBCountInfoLst);
         RETVALUE(RFAILED);
      }

      /* Update Count info of each DRB from countCfmInfo */

      for (rbIndex = 0; rbIndex < countCfmInfo->numRb; rbIndex++)
      {
         nhuDrbCountInfo[rbIndex].drbId =
                        countCfmInfo->countInfo[rbIndex].rbId;
         nhuDrbCountInfo[rbIndex].drbDirection =
                        countCfmInfo->countInfo[rbIndex].dir;
         nhuDrbCountInfo[rbIndex].ulCount =
                        countCfmInfo->countInfo[rbIndex].ulCount;
         nhuDrbCountInfo[rbIndex].dlCount =
                        countCfmInfo->countInfo[rbIndex].dlCount;
      }
      nhuDRBCountInfoLst->member = nhuDrbCountInfo;

   }

   /* Call function to send Cfm to NHU user */
   if (ROK != nhUIMSndCountCfm(nhuDRBCountInfoLst))
   {
      RLOG0(L_ERROR,"nhUIMSndCountCfm failed ");
      NH_FREEEVNT(nhuDRBCountInfoLst);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* nhLIMHndlCountCfm */

/**
 *
 * @brief Handler to handle sec configuration confirm from PDCP layer.
 * @details
 *
 *       This function is used to handle sec configuration confirm
 *            from PDCP layer.
 *            
 *     @param[in] secCfgCfm   PDCP configuration confirm info
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PRIVATE S16 nhLIMHndlPdcpSecCfgCfm
(
CpjSecCfgCfmInfo *secCfgCfm
)
#else /* ANSI */
PRIVATE S16 nhLIMHndlPdcpSecCfgCfm(secCfgCfm)
CpjSecCfgCfmInfo *secCfgCfm;
#endif /* ANSI */
{
   NhId              nhIdentity;
   U8                cgmCfmStatus = NH_CFG_SUCCESS;
   NhActvUeCb        *ueCb         = NULLP;
   TransCb           *transCtrlBlk      = NULLP;
   NhCellCb          *cellCb       = NULLP;
   NhCellAndUeCb     cellAndUeCb;

   TRC2(nhLIMHndlPdcpSecCfgCfm)

   RLOG_ARG2(L_DEBUG, DBG_TRNSID, secCfgCfm->transId, "ueId[%d] cellId[%d]",
      secCfgCfm->ueId, secCfgCfm->cellId);

   nhIdentity.cellId = secCfgCfm->cellId;
   nhIdentity.ueId = secCfgCfm->ueId;

   if (NULLP == (cellCb = nhDBMGetCellCb(nhIdentity.cellId)))
   {
      RLOG0(L_ERROR, "NULL CELL CB ");
      RETVALUE(RFAILED); 
   }

   if ((NhActvUeCb*)NULLP == (ueCb = nhDBMGetUeCb(cellCb, secCfgCfm->ueId)))
   {
      RLOG0(L_ERROR, "NULL UE CB ");
      RETVALUE(RFAILED); 
   }
   cellAndUeCb.ueCb = ueCb;
   cellAndUeCb.cellCb = cellCb;


   /* Find trans node in list with specified transId value */
   if ((TransCb*)NULLP == (transCtrlBlk = nhUTLGetTransCbFrmNxTransId(ueCb,
                                          secCfgCfm->transId)))
   {
      RLOG0(L_ERROR, "transId doesnt exist ");
      RETVALUE(RFAILED);
   }

    /* Drop Undesired/Duplicate Cfg Cfm */
    if (transCtrlBlk->numOfPdcpCfms <= 0)
    {
       RLOG0(L_DEBUG, "Invalid Pdcp Cfg Cfm Rcvd ");
       RETVALUE(ROK);
    }
    transCtrlBlk->numOfPdcpCfms--;

   /* If security configuration is sent, no bitmask is updated.
    * But the security configuration table is updated in transCtrlBlk.
    * This entry is used to validate whether the PDCP Cfg Cfm is
    * expected or not.*/
   if ((NH_STATUS_USED == transCtrlBlk->secCfgMapTbl[NH_INTG_CFG].status) ||
       (NH_STATUS_USED == transCtrlBlk->secCfgMapTbl[NH_CIPH_CFG].status))
   {
         if (secCfgCfm->status == CPJ_CFG_CFM_NOK)
         {
            cgmCfmStatus = NH_CFG_FAILED;
         }
   }


   /* If the cfg and cfm bitmask matches then all the successful
    * cfms are received
    */
   if(transCtrlBlk->numOfPdcpCfms <= 0)
   {
      if (cgmCfmStatus == NH_CFG_SUCCESS)
      {
         ueCb->rbSecCfgBitMask |= NH_SEC_CFG_DONE;
      }
         /* Now call function to send Cfm Report to CGM */
      if (ROK != nhLIMRptCfgCfm(&nhIdentity, &cellAndUeCb, transCtrlBlk,
                                 cgmCfmStatus, NH_PDCP_CONFIGURED))
      {
         RLOG0(L_ERROR, "nhLIMRptCfgCfm failed");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}  /* end of nhLIMHndlPdcpSecCfgCfm */

/**
 *
 * @brief Handler to to send Count Request to PDCP.
 * @details
 *
 *       This function is used to handle sec configuration confirm
 *            from PDCP layer.
 *            
 *     @param[in] cntHdr   Count Request Header 
 *     @param[in] ueCb     UE Control block
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/      
#ifdef ANSI
PUBLIC S16 nhLIMCountReqHndlr
(
NhuHdr         *cntHdr,
NhActvUeCb     *ueCb
)
#else /* ANSI */
PUBLIC S16 nhLIMCountReqHndlr(cntHdr, ueCb)
NhuHdr            *cntHdr;
NhActvUeCb     *ueCb;
#endif /* ANSI */
{

   TRC2(nhLIMCountReqHndlr)
   UNUSED(ueCb);
   if (ROK != nhPIMSndCountReq(cntHdr, ueCb))
   {
      RLOG0(L_ERROR,"Cound not send Count request");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* End of nhLIMCountReqHndlr */

/**
 * @brief This function fills PDCP SDU status information in NHU SDU status
 *        confirm.
 *
 * @details
 *
 * This function  allocates memory for pdcpSduStaInfo in NhuPdcpSduStaCfm
 * and ulBitMap structure in pdcpSduStaInfo for each RB. If allocation 
 * is successful, the function fills the PDCP SDU status information
 * from cpjSduStaCfm structure.
 *
 * @param[in] cpjSduStaCfm   CPJ SDU status confirm
 * @param[in] nhuSduStaCfm   NHU PDCP SDU status confirm
 *
 * @return S16
 *  -# ROK
 *  -# RFAILED
*/
#ifdef ANSI
PUBLIC S16 nhLIMFillPdcpSduStaInfo
(
CpjSduStaCfmInfo *cpjSduStaCfm,
NhuPdcpSduStaCfm *nhuSduStaCfm
)
#else /* ANSI */
PUBLIC S16 nhLIMFillPdcpSduStaInfo(cpjSduStaCfm, nhuSduStaCfm)
CpjSduStaCfmInfo *cpjSduStaCfm;
NhuPdcpSduStaCfm *nhuSduStaCfm;
#endif /* ANSI */
{
   NhuPdcpSduStaInfo *pdcpSduStaInfo = NULLP;
   U8                numRbId;
   U8                rbCnt;

   TRC2(nhLIMFillPdcpSduStaInfo)

   RLOG0(L_DEBUG," nhLIMFillPdcpSduStaInfo called.");

   numRbId = cpjSduStaCfm->numRb;

   /* Allocate memory for PDCP Status Info structure  for number of RBs */
   if (numRbId == 0)
   {
      RLOG0(L_ERROR,"NhuPdcpSduStaInfo Number of RbId is Zero failed.");
      RETVALUE(RFAILED);
   }

   NH_GET_MEM(nhuSduStaCfm, (sizeof(NhuPdcpSduStaInfo) * numRbId),
                  &(nhuSduStaCfm->pdcpSduStaInfo));
   if(NULLP == nhuSduStaCfm->pdcpSduStaInfo)
   {
      RLOG0(L_FATAL,"NhuPdcpSduStaInfo memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* For each RB, allocate memory for ulBitMap */
   /* If allocation successful, fill the PDCP Status info */
   for(rbCnt = 0; rbCnt < numRbId; rbCnt++)
   {
      pdcpSduStaInfo = &nhuSduStaCfm->pdcpSduStaInfo[rbCnt];

      pdcpSduStaInfo->drbId      = cpjSduStaCfm->sduStaInfo[rbCnt].rbId;
      pdcpSduStaInfo->dir        = cpjSduStaCfm->sduStaInfo[rbCnt].dir;
      /* Filling SN length */
      pdcpSduStaInfo->snLen      = cpjSduStaCfm->sduStaInfo[rbCnt].snLen;
      pdcpSduStaInfo->dlCountVal = 
         cpjSduStaCfm->sduStaInfo[rbCnt].dlSduStaInfo.count;
      pdcpSduStaInfo->ulCountVal = 
         cpjSduStaCfm->sduStaInfo[rbCnt].ulSduStaInfo.count;
      pdcpSduStaInfo->rcvStaOfUlPdcpSdus.numBits =
         cpjSduStaCfm->sduStaInfo[rbCnt].ulSduStaInfo.numBits;

      /* Allocate memory for ulBitMap */
      NH_GET_MEM(nhuSduStaCfm, 
            (pdcpSduStaInfo->rcvStaOfUlPdcpSdus.numBits * sizeof(U8)),
            &pdcpSduStaInfo->rcvStaOfUlPdcpSdus.ulBitMap);
      if(NULLP == pdcpSduStaInfo->rcvStaOfUlPdcpSdus.ulBitMap)
      {
         NH_DBG_FATAL((NH_PRNT_BUF,
                  "Memory Allocation failed for storing ulBitMap.\n"));
         RETVALUE(RFAILED);
      }
      else
      {
         /* copy ulBitMap information */
         NH_MEM_COPY((U8*)pdcpSduStaInfo->rcvStaOfUlPdcpSdus.ulBitMap,
               (CONSTANT U8*)cpjSduStaCfm->sduStaInfo[rbCnt].ulSduStaInfo.ulBitMap,
               pdcpSduStaInfo->rcvStaOfUlPdcpSdus.numBits);
      }
   }

   RETVALUE(ROK);
} /* end of nhLIMFillPdcpSduStaInfo */

/**
 * @brief Function for freeing CpjSduStaCfmInfo structure.
 *
 * @details
 *
 *
 * This function frees ulBitMap received from PDCP and CpjSduStaCfmInfo.

 * @param[in] pst Pst event structure.
 * @param[in] cpjSduStaCfm PDCP SDU status confirm.
 *
 * @return S16
 *  -# ROK
*/
#ifdef ANSI
PUBLIC S16 nhLIMFreeCpjSduStaCfm
(
Pst                  *pst,
CpjSduStaCfmInfo     *cpjSduStaCfm
)
#else /* ANSI */
PUBLIC S16 nhLIMFreeCpjSduStaCfm(pst,cpjSduStaCfm)
Pst                  *pst;
CpjSduStaCfmInfo     *cpjSduStaCfm;
#endif /* ANSI */
{
   U8                 rbCnt;
   Size               ulBitMapSize;

   TRC2(nhLIMFreeCpjSduStaCfm)

   RLOG0(L_DEBUG," nhLIMFreeCpjSduStaCfm called.");

   /* Delete ulBitMap for each RB from cpjSduStaCfm */
   for(rbCnt = 0; rbCnt < cpjSduStaCfm->numRb; rbCnt++)
   {
      if(NH_ZERO_VAL != cpjSduStaCfm->sduStaInfo[rbCnt].ulSduStaInfo.numBits)
      {
         NH_GET_UL_BITMAP_SIZE(
            cpjSduStaCfm->sduStaInfo[rbCnt].ulSduStaInfo.numBits,
                          ulBitMapSize);

         /* Allocate memory for ulBitMap */
         NH_FREE(pst->region, pst->pool, 
                 cpjSduStaCfm->sduStaInfo[rbCnt].ulSduStaInfo.ulBitMap,
                 sizeof(U8) * ulBitMapSize);
      }
   }
   NH_FREE(pst->region, pst->pool, cpjSduStaCfm, sizeof(CpjSduStaCfmInfo));
   RETVALUE(ROK);
} /* end of nhLIMFreeCpjSduStaCfm */

/**
 * @brief Function for Validating RBs Cfg Confirm and Reporting to CGM.
 *
 * @details
 *
 *
 * Function for Validating RBs Cfg Confirm and Reporting to CGM.

 * @param[in] vCfmInfo       Config Confirm Structure
 * @param[in] transCtrlBlk   Transaction context
 * @param[in] nhIdentity     Nh ID
 * @param[in] cellAndUeCb    Cell and UE Context
 * @param[in] layerId Layer  ID
 *
 * @return S16
 *  -# ROK
*/

#ifdef ANSI
PRIVATE S16 nhLIMVldtAndRptCfgCfm
(
Void        *vCfmInfo,
TransCb     *transCtrlBlk,
NhId        *nhIdentity,
NhCellAndUeCb   *cellAndUeCb,
U8          layerId
)
#else /* ANSI */
PRIVATE S16 nhLIMVldtAndRptCfgCfm(vCfmInfo, transCtrlBlk, nhIdentity, 
                                  cellAndUeCb, layerId)
Void        *vCfmInfo;
TransCb     *transCtrlBlk;
NhId        *nhIdentity;
NhCellAndUeCb   *cellAndUeCb;
U8          layerId;
#endif /* ANSI */
{
   NhRlcPdcpCfgCfmMapInfo  *mapTbl;
   NhuHdr                  hdr;
   NhRlcPdcpCfmEnt         cfmEnt[CKW_MAX_ENT_CFG];
   U8                      cgmCfmStatus = NH_CFG_SUCCESS;
   U8                      numEnt;
   U8                      rbId;
   Bool                    rbType;
   Bool                    found = FALSE;
   U8                      rcvdSrbIds = NH_ZERO_VAL;
   U32                     rcvdDrbIds = NH_ZERO_VAL;
   U8                      entIndex;
   U8                      maxTblIdx;
   U8                      okValue;
   U16                     numOfCfms;
   NhUstaDgn               dgn;                  /* diagnostics variable */

   TRC2(nhLIMVldtAndRptCfgCfm)

   RLOG0(L_DEBUG, "nhLIMVldtAndRptCfgCfm");

   if (layerId == NH_PDCP_CONFIGURED)
   {
      /* For PDCP */
      transCtrlBlk->numOfPdcpCfms--;
      numEnt = ((CpjCfgCfmInfo *)vCfmInfo)->numEnt;
      for (entIndex = 0; ((entIndex < numEnt) && 
                         (entIndex < CPJ_MAX_CFM)); entIndex++)
      {
         cfmEnt[entIndex].rbId    = 
                 ((CpjCfgCfmInfo *)vCfmInfo)->cfmEnt[entIndex].rbId;
         cfmEnt[entIndex].rbType  = 
                 ((CpjCfgCfmInfo *)vCfmInfo)->cfmEnt[entIndex].rbType;
         cfmEnt[entIndex].status  = 
                 (U16)((CpjCfgCfmInfo *)vCfmInfo)->cfmEnt[entIndex].status;
         cfmEnt[entIndex].reason  = 
                 (U16)((CpjCfgCfmInfo *)vCfmInfo)->cfmEnt[entIndex].reason;
      }
      mapTbl      = &(transCtrlBlk->pdcpMapTbl);
      hdr.cellId  = ((CpjCfgCfmInfo *)vCfmInfo)->cellId;
      hdr.ueId    = ((CpjCfgCfmInfo *)vCfmInfo)->ueId;
      hdr.transId = ((CpjCfgCfmInfo *)vCfmInfo)->transId;
      maxTblIdx   = NH_UE_MAPTBL_IDX_PDCP;
      okValue     = CPJ_CFG_CFM_NOK;
   RLOG0(L_DEBUG, "nhLIMVldtAndRptCfgCfm:1");
   }
   else /* otherwise It must be RLC */
   {
      /* For RLC */
      transCtrlBlk->numOfRlcCfms--;
      numEnt = ((CkwCfgCfmInfo *)vCfmInfo)->numEnt;
      for (entIndex = 0; ((entIndex < numEnt) && 
                         (entIndex < CKW_MAX_ENT_CFG)); entIndex++)
      {
         cfmEnt[entIndex].rbId    = 
                 ((CkwCfgCfmInfo *)vCfmInfo)->entCfgCfm[entIndex].rbId;
         cfmEnt[entIndex].rbType  = 
                 ((CkwCfgCfmInfo *)vCfmInfo)->entCfgCfm[entIndex].rbType;
         cfmEnt[entIndex].status  = (U16) 
                 ((CkwCfgCfmInfo *)vCfmInfo)->entCfgCfm[entIndex].status.status;
         cfmEnt[entIndex].reason  = (U16) 
                 ((CkwCfgCfmInfo *)vCfmInfo)->entCfgCfm[entIndex].status.reason;
      }
      mapTbl      = &(transCtrlBlk->rlcMapTbl);
      hdr.cellId  = ((CkwCfgCfmInfo *)vCfmInfo)->cellId;
      hdr.ueId    = ((CkwCfgCfmInfo *)vCfmInfo)->ueId;
      hdr.transId = ((CkwCfgCfmInfo *)vCfmInfo)->transId;
      maxTblIdx   = NH_UE_MAPTBL_IDX_RLC;
      okValue     = CKW_CFG_CFM_NOK;
   RLOG0(L_DEBUG, "nhLIMVldtAndRptCfgCfm:2");
   }


   for (entIndex = 0; entIndex < numEnt; entIndex++)
   {
      found = FALSE;
      rbId = cfmEnt[entIndex].rbId;
      rbType = cfmEnt[entIndex].rbType;

   RLOG0(L_DEBUG, "nhLIMVldtAndRptCfgCfm:3");
      if(rbType == CM_LTE_SRB)
      {
   RLOG0(L_DEBUG, "nhLIMVldtAndRptCfgCfm:4");
         if ((ROK != nhLIMSrchAndSetSRBBitMsk(&rcvdSrbIds,
                  mapTbl->srbToCfg, mapTbl->srbCfmSta, rbId, 
                  cfmEnt[entIndex].status, okValue)))
         {
   RLOG0(L_DEBUG, "nhLIMVldtAndRptCfgCfm:5");
            RETVALUE(RFAILED);
         }
      }
      else
      {
   RLOG0(L_DEBUG, "nhLIMVldtAndRptCfgCfm:6");
         if (ROK != nhLIMSrchAndSetDRBBitMsk(&rcvdDrbIds, mapTbl->drbToCfg,
                  mapTbl->drbCfmSta, rbId, cfmEnt[entIndex].status, okValue))
         {
   RLOG0(L_DEBUG, "nhLIMVldtAndRptCfgCfm:7");
            RETVALUE(RFAILED);
         }
      }
   }
   if (
           (rcvdSrbIds != ((mapTbl->srbToCfg[NH_RB_TO_ADD]) |
                          (mapTbl->srbToCfg[NH_RB_TO_MOD]) |
                          (mapTbl->srbToCfg[NH_RB_TO_RLS]) |
                          (mapTbl->srbToCfg[NH_RB_TO_REST])))
           ||
           (rcvdDrbIds != ((mapTbl->drbToCfg[NH_RB_TO_ADD]) |
                          (mapTbl->drbToCfg[NH_RB_TO_MOD]) |
                          (mapTbl->drbToCfg[NH_RB_TO_RLS]) |
                          (mapTbl->drbToCfg[NH_RB_TO_REST])))
      )
   {
      /* generate alarm */
      NH_SET_ZERO (&dgn, sizeof(dgn));
      dgn.type = LNH_USTA_DGNVAL_CELLUEID;
      dgn.u.cellUeId.ueId = hdr.ueId;
      dgn.u.cellUeId.cellId = hdr.cellId;
      nhSendLmAlarm(LCM_CATEGORY_INTERFACE, LNH_EVENT_OPERATION_FAIL,
                 LCM_CAUSE_UNKNOWN, &dgn);
      RLOG0(L_ERROR, "Invalid number of RLC Cfg Cfm RCVD ");
      RLOG0(L_DEBUG, "nhLIMVldtAndRptCfgCfm:8");
      RETVALUE(RFAILED);
   }
      
   /* If the cfg and cfm bitmask matches, then all the successful
    * cfm is received
    */
   if (layerId == NH_PDCP_CONFIGURED)
   {
      numOfCfms = transCtrlBlk->numOfPdcpCfms;
   }
   else
   {
      numOfCfms = transCtrlBlk->numOfRlcCfms;
   }

   if(numOfCfms <= 0)
   {
      if( (mapTbl->srbCfmSta[NH_RB_TO_MOD] != 
               mapTbl->srbToCfg[NH_RB_TO_MOD]) ||
          (mapTbl->srbCfmSta[NH_RB_TO_RLS] != 
               mapTbl->srbToCfg[NH_RB_TO_RLS]) ||
          (mapTbl->srbCfmSta[NH_RB_TO_ADD] != 
               mapTbl->srbToCfg[NH_RB_TO_ADD]) ||
          (mapTbl->srbCfmSta[NH_RB_TO_REST] != 
               mapTbl->srbToCfg[NH_RB_TO_REST]) ||
          /* Check for Signalling Radio bearer failures */
          (mapTbl->drbCfmSta[NH_RB_TO_MOD] != 
               mapTbl->drbToCfg[NH_RB_TO_MOD]) ||
          (mapTbl->drbCfmSta[NH_RB_TO_RLS] != 
               mapTbl->drbToCfg[NH_RB_TO_RLS]) ||
          (mapTbl->drbCfmSta[NH_RB_TO_ADD] != 
               mapTbl->drbToCfg[NH_RB_TO_ADD]) ||
          (mapTbl->drbCfmSta[NH_RB_TO_REST] != 
               mapTbl->drbToCfg[NH_RB_TO_REST]) ||
          ((transCtrlBlk->ueMapTbl[maxTblIdx].status == NH_STATUS_USED) && 
           (transCtrlBlk->ueMapTbl[maxTblIdx].cfmStatus != 
               CKW_CFG_CFM_OK)))
      {
         nhUTLUpdGenSts(NH_STS_CAUSE_LOWER_LYR_FAIL);
         cgmCfmStatus = NH_CFG_FAILED;
      }

      RLOG0(L_DEBUG, "nhLIMVldtAndRptCfgCfm:9");
      /* Now call function to send Cfm Report to CGM */
      if (ROK != nhLIMRptCfgCfm(nhIdentity, cellAndUeCb, transCtrlBlk, 
                                cgmCfmStatus, layerId))
      {
         RLOG0(L_DEBUG, "nhLIMVldtAndRptCfgCfm:10");
         RLOG0(L_ERROR, "nhLIMRptCfgCfm failed");
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
} /* end of nhLIMVldtAndRptCfgCfm */

/**
 * @brief Function for Validating RBs Cfg Confirm and Reporting to CGM.
 *
 * @details
 *
 *
 * Function for Validating RBs Cfg Confirm and Reporting to CGM.

 * @param[in] rcvdRbIds placeholder for total received cfms.
 * @param[in] rbCfgTbl Maptable for expected cfg cfms.
 * @param[in] rbStaTbl Maptable for status of cfg cfms.
 * @param[in] rbId     RB Id.
 * @param[in] status   Status received.
 * @param[in] okValue  Value ok Proper status when cfm is successful.
 *
 * @return S16
 *  -# ROK
*/
#ifdef ANSI
PRIVATE S16 nhLIMSrchAndSetSRBBitMsk
(
U8         *rcvdRbIds,
U8         *rbCfgTbl,
U8         *rbStaTbl,
U8         rbId,
U16        status,
U8         okValue
)
#else /* ANSI */
PRIVATE S16 nhLIMSrchAndSetSRBBitMsk(rcvdRbIds, rbCfgTbl, rbStaTbl, rbId, 
                                     status, okValue)
U8         *rcvdRbIds;
U8         *rbCfgTbl;
U8         *rbStaTbl;
U8         rbId;
U16        status;
U8         okValue;
#endif /* ANSI */
{
   U16      j = 0;
   U8       tmp =  0x01;
   Bool     found = FALSE;

   /* nh004.201 - Correction to TRC functionality. */
   TRC2(nhLIMSrchAndSetSRBBitMsk)

   /* Now search each Add/Mod/Rls bit mask and set the
    * corresponding cfm bit mask. 
    * For duplicate case, return ROK.
    */

   tmp = (1 << (rbId-1));

   /* Check for duplicate */

   NH_SET_RCVD_CFG_CFM_MSK(rcvdRbIds, tmp);

   NH_CHK_RB_ACTN_CFG_CFM(rbCfgTbl, tmp, found, j);

   if (FALSE == found)
   {
      RLOG0(L_ERROR, "Invalid Cfg Cfm RCVD ");
      RETVALUE(RFAILED);
   }

   NH_SET_RCVD_STA_CFM_MSK(rbStaTbl, status, okValue, tmp, j);

   RETVALUE(ROK);
}/* End of nhLIMSrchAndSetSRBBitMsk */

/**
 * @brief Function for Validating RBs Cfg Confirm and Reporting to CGM.
 *
 * @details
 *
 *
 * Function for Validating RBs Cfg Confirm and Reporting to CGM.

 * @param[in] rcvdRbIds placeholder for total received cfms.
 * @param[in] rbCfgTbl Maptable for expected cfg cfms.
 * @param[in] rbStaTbl Maptable for status of cfg cfms.
 * @param[in] rbId     RB Id.
 * @param[in] status   Status received.
 * @param[in] okValue  Value ok Proper status when cfm is successful.
 *
 * @return S16
 *  -# ROK
*/
#ifdef ANSI
PRIVATE S16 nhLIMSrchAndSetDRBBitMsk
(
U32        *rcvdRbIds,
U32         *rbCfgTbl,
U32         *rbStaTbl,
U8         rbId,
U16        status,
U8         okValue
)
#else /* ANSI */
PRIVATE S16 nhLIMSrchAndSetDRBBitMsk(rcvdRbIds, rbCfgTbl, rbStaTbl, rbId, 
                                     status, okValue)
U32        *rcvdRbIds;
U32         *rbCfgTbl;
U32         *rbStaTbl;
U8         rbId;
U16        status;
U8         okValue;
#endif /* ANSI */
{
   U16      j = 0;
   U32       tmp =  0x01;
   Bool     found = FALSE;

   /* nh004.201 - Correction to TRC functionality. */
   TRC2(nhLIMSrchAndSetDRBBitMsk)

   /* Now search each Add/Mod/Rls bit mask and set the
    * corresponding cfm bit mask. 
    * For duplicate case, return ROK.
    */

   tmp = (1 << (rbId-1));
   /* Check for duplicate */
   NH_SET_RCVD_CFG_CFM_MSK(rcvdRbIds, tmp);

   NH_CHK_RB_ACTN_CFG_CFM(rbCfgTbl, tmp, found, j);

   if (FALSE == found)
   {
      RLOG0(L_ERROR, "Invalid Cfg Cfm RCVD ");
      RETVALUE(RFAILED);
   }

   NH_SET_RCVD_STA_CFM_MSK(rbStaTbl, status, okValue, tmp, j);

   RETVALUE(ROK);
}/* End of nhLIMSrchAndSetDRBBitMsk */

/** @} */
/**********************************************************************
  
         End of file:     ni_lim.c@@/main/3 - Fri Jul  1 01:13:19 2011
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      mc               1. Initial LTE-RRC release.
/main/2      ---      ch               1. RRC Release 2.1.
          nh003.201   vkulkarni        1. Order of sending rrc conn reconfig 
                                          message is corrected 
          nh004.201 vkulkarni  1. Correction to TRC functionality.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
         nh002.301    mpatel           1. [ccpu00118385]: Sending Pdcp Sdu Status
                                          req before RLC reset in case of HO
/main/4  nh003.301    njha             1. Re-Establishment fix and NH_DIAG_LVL0 added.
*********************************************************************91*/
