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
               

     File:     wr_umm_sch_msg.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=73;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_umm.h"
#include "wr_umm_drx.h"

#include "wr_umm_sch_msg.h"
//#include "rm_wr.h"
#include "wr_ifm_schd.h"
/* SPS changes starts */
#include "wr_umm_trans.h"
#include "wr_ifm_rrm.h"
/* SPS changes ends */

/**
 *  @brief This function fill Ue transmission Mode configuration
 *
 *      Function: wrSchFillRgrUeTxModeCfg
 *
 *          Processing steps:
 *          - Populate RgrUeTxModeCfg Structure
 *
 *  @param [out] ueCfg : UE transmission mode config info
 *  @param [in]  ueCb  : pointer to UE CB
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrSchFillRgrUeTxModeCfg
(
RgrUeTxModeCfg               *ueCfg,
WrUeCb                       *ueCb
)
{
   ueCfg->pres = TRUE;
   /* Need to convert to RGR format from NHU*/
   FILL_ANT_TX_MODE(ueCb->antennaInfo.txmode, ueCfg);

   RETVALUE(ROK);
} /* wrSchFillRgrUeTxModeCfg */


/**
 *  @brief This function fill UE DL CQI configuation info
 *
 *      Function: wrUmmSchFillRgrUeDlCqiCfg
 *
 *          Processing steps:
 *          - populate RgrUeDlCqiCfg Structure
 *
 *  @param [out] ueCfg : pointer to Ue Dl Cqi config to be populated
 *  @param [in]  ueCb  : pointer to UE CB
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmSchFillRgrUeDlCqiCfg
(
RgrUeDlCqiCfg                *ueCfg,
WrUeCb                       *ueCb
)
{
   if(ueCb->bResCfgBitMap & WR_UE_RADIO_RES_CFG_CQI)
   {
      ueCfg->aprdCqiCfg.pres = FALSE;
      /*TODO TFU_UPGRADE  */
      ueCfg->aprdCqiCfg.aprdModeEnum = ueCb->cqiRptCfg.cqiAperiodicMode; 
      /* Fix: tfu upgrade changes */
#ifndef TFU_UPGRADE
      ueCfg->prdCqiCfg.pres = ueCb->cqiRptCfg.periodRptSetupPres;
      ueCfg->prdCqiCfg.prdModeEnum = ueCb->cqiRptCfg.periodicmode;
      /* TFU_UPGRADE  TODO*/
      ueCfg->prdCqiCfg.prdicityEnum = WR_VAL_ZERO /*0*/;
      ueCfg->prdCqiCfg.subframeOffst = WR_VAL_ZERO /*wrDfltSubframeOffst*/;
      /*This param will go away by TFU_UPGRADE TODO */
      ueCfg->prdCqiCfg.cqiOffst = ueCb->cqiOffst;
      /* TODO: Take correct value from UE CB */
      ueCfg->prdCqiCfg.k = ueCb->cqiRptCfg.subbndCqik;
      /* prdCqiCfg.k value should always be greater than 0,issue comes for PAL
       * setup as for PAL setup, this TFU_UPGRADE flag is always disabled */
      ueCfg->prdCqiCfg.k = 1;
      ueCfg->prdCqiCfg.cqiPmiCfgIdx = ueCb->cqiRptCfg.cqipmiCfgIdx;
#else
      if (ueCb->cqiRptCfg.aperCqiEnbld)
      {
         ueCfg->aprdCqiCfg.pres = TRUE;
         ueCfg->aprdCqiCfg.aprdModeEnum = \
            (RgrAprdCqiMode) ueCb->cqiRptCfg.cqiAperiodicMode;
      }
      /* TODO: Need to fill actual values */
      /*ccpu00129641*/
      ueCfg->prdCqiCfg.type = RGR_SCH_PCQI_SETUP; /* Release */


      ueCfg->prdCqiCfg.cqiSetup.cqiPResIdx = ueCb->cqiRptCfg.cqiPucchRsrcIdx; /*!< cqi-PUCCH-ResourceIndex (0.. 1185) */
      ueCfg->prdCqiCfg.cqiSetup.cqiPCfgIdx = ueCb->cqiRptCfg.cqipmiCfgIdx; /*!< cqi-pmi-ConfigIndex (0..1023) */
      /*129311 : CQI format check*/
      if(ueCb->cqiRptCfg.fmtInd ==  WR_UE_CQI_FMT_WB)
      {
         ueCfg->prdCqiCfg.cqiSetup.cqiRepType =  RGR_UE_PCQI_WB_REP;


      }
      else
      {
         ueCfg->prdCqiCfg.cqiSetup.cqiRepType =  RGR_UE_PCQI_SB_REP;

         /* Factor K is Valid only for Subband CQI */
         ueCfg->prdCqiCfg.cqiSetup.k = ueCb->cqiRptCfg.subbndCqik;
      }

      /*!< Rand Indicator is Enabled TRUE(1) FALSE(0) */
      if(ueCb->cqiRptCfg.riPres == TRUE)
      {
         ueCfg->prdCqiCfg.cqiSetup.riEna = TRUE; /*!< Rand Indicator is Enabled TRUE(1) FALSE(0) */
         ueCfg->prdCqiCfg.cqiSetup.riCfgIdx = ueCb->cqiRptCfg.riCfgIdx; /*!< ri-ConfigIndex    (0..1023)  */
      }
      else
      {
         ueCfg->prdCqiCfg.cqiSetup.riEna = FALSE; /*!< Rand Indicator is Enabled TRUE(1) FALSE(0) */
      }
      ueCfg->prdCqiCfg.cqiSetup.sANCQI = ueCb->cqiRptCfg.simulAckNackAndCQI;  /*!< simultaneousAckNackAndCQI TRUE(1) FALSE(0) */
      ueCfg->prdCqiCfg.cqiSetup.prdModeEnum = (RgrPrdCqiMode)ueCb->cqiRptCfg.periodicmode; /*!< Peiodic CQI reporting mode */
      /*ccpu00129641*/
#endif
   }
#ifdef TFU_UPGRADE
   else
   {
      ueCfg->aprdCqiCfg.pres = NOTPRSNT;
      ueCfg->prdCqiCfg.type = RGR_SCH_PCQI_REL; /* Release */
   }
#endif

   RETVALUE(ROK);
} /* wrUmmSchFillRgrUeDlCqiCfg */


/**
 *  @brief This function fill UE UL HARQ configuration Info
 *
 *      Function: wrUmmSchFillRgrUeUlHqCfg
 *
 *          Processing steps:
 *          - populate RgrUeUlHqCfg Structure
 *
 *  @param [out] ueCfg : pointer to UE UL HARQ config info
 *  @param [in]  ueCb  : pointer to UE CB
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
PUBLIC S16 wrUmmSchFillRgrUeUlHqCfg
(
 RgrUeUlHqCfg                 *ueCfg,
 WrUeCb                       *ueCb
)
{
   ueCfg->deltaHqOffst = ueCb->ueUlHqCfg.deltaHqOffst;

   switch(ueCb->ueUlHqCfg.maxUlHqTx)
   {
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn1Enum:
         ueCfg->maxUlHqTx = 1;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn2Enum:
         ueCfg->maxUlHqTx = 2;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn3Enum:
         ueCfg->maxUlHqTx = 3;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn4Enum:
         ueCfg->maxUlHqTx = 4;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn5Enum:
         ueCfg->maxUlHqTx = 5;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn6Enum:
         ueCfg->maxUlHqTx = 6;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn7Enum:
         ueCfg->maxUlHqTx = 7;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn8Enum:
         ueCfg->maxUlHqTx = 8;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn10Enum:
         ueCfg->maxUlHqTx = 10;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn12Enum:
         ueCfg->maxUlHqTx = 12;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn16Enum:
         ueCfg->maxUlHqTx = 16;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn20Enum:
         ueCfg->maxUlHqTx = 20;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn24Enum:
         ueCfg->maxUlHqTx = 24;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigmaxHARQ_Txn28Enum:
         ueCfg->maxUlHqTx = 28;
         break;
      default:
         ueCfg->maxUlHqTx = 1;
         break;
   }
   RETVALUE(ROK);
} /* wrUmmSchFillRgrUeUlHqCfg */

/**
 *  @brief This function fill UE UL power configuration Info
 *
 *      Function: wrUmmSchFillRgrUeUlPwrCfg
 *
 *          Processing steps:
 *          - populate RgrUeUlPwrCfg Structure
 *
 *  @param [out] ueCfg : pointer to UE UL Power config info
 *  @param [in]  ueCb  : pointer to UE CB
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmSchFillRgrUeUlPwrCfg
(
 RgrUeUlPwrCfg                *ueCfg,
 WrUeCb                       *ueCb
 )
{
   ueCfg->uePuschPwr.pres = ueCb->uePuschPwr.pres;
   ueCfg->uePuschPwr.idx = ueCb->uePuschPwr.idx;
   ueCfg->uePuschPwr.tpcRnti = ueCb->uePuschPwr.tpcRnti;

   ueCfg->uePucchPwr.pres = ueCb->uePucchPwr.pres;
   ueCfg->uePucchPwr.idx = ueCb->uePucchPwr.idx;
   ueCfg->uePucchPwr.tpcRnti = ueCb->uePucchPwr.tpcRnti;

   ueCfg->isAccumulated = ueCb->ulPwrDedCtrl.accumlationEnabled;
   ueCfg->isDeltaMCSEnabled = ueCb->ulPwrDedCtrl.deltaMcsEnabled;
   ueCfg->p0UePusch = ueCb->ulPwrDedCtrl.p0UePusch;
   ueCfg->p0UePucch = ueCb->ulPwrDedCtrl.p0UePucch;
   ueCfg->pSRSOffset = ueCb->ulPwrDedCtrl.pSrsOff;
   ueCfg->trgCqi = ueCb->ulPwrDedCtrl.trgCqi;

   RETVALUE(ROK);
} /* wrUmmSchFillRgrUeUlPwrCfg */

/**
 *  @brief This function fill UE Qos Configuration
 *
 *      Function: wrUmmSchFillRgrUeQosCfg
 *
 *          Processing steps:
 *          - populate RgrUeQosCfg Structure
 *
 *  @param [out]  ueCfg : pointer to UE QOS config info to be updated
 *  @param [in]   ueCb  : pointer to UE CB
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmSchFillRgrUeQosCfg
(
 RgrUeQosCfg                  *ueCfg,
 WrUeCb                       *ueCb
 )
{
   ueCfg->ambrPres = ueCb->qosCfg.ambrPres;
   ueCfg->dlAmbr = ueCb->qosCfg.dlAmbr;
   ueCfg->ueBr = ueCb->qosCfg.ueBr;

   RETVALUE(ROK);
} /* wrUmmSchFillRgrUeQosCfg */


/**
 *  @brief This function fill UE TA Timer configuration
 *
 *      Function: wrUmmSchFillRgrUeTaTmrCfg
 *
 *          Processing steps:
 *          - populate RgrUeTaTmrCfg Structure
 *
 *  @param [out]  ueCfg : pointer to UE TA Timer configuration info
 *  @param [in]   ueCb  : pointer to UE CB
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
PUBLIC S16 wrUmmSchFillRgrUeTaTmrCfg
(
 RgrUeTaTmrCfg                 *ueCfg,
 WrUeCb                        *ueCb
 )
{
   ueCfg->pres = ueCb->taTmrCfg.pres;
   ueCfg->taTmr = ueCb->taTmrCfg.taTmr; 

   RETVALUE(ROK);
} /* wrUmmSchFillRgrUeTaTmrCfg */


#ifdef TFU_UPGRADE
/**
 *  @brief This function fill the Scheduling Request Configuration
 *  configuration for a UE
 *
 *      Function: wrUmmSchFillRgrUeSrCfg
 *
 *          Processing steps:
 *          - populate RgrUeSrCfg Structure
 *
 *  @param [out] ueCfg : pointer to UE Srs UL DedCfg
 *  @param [in]  ueCb  : pointer to Ue CB
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
PUBLIC S16 wrUmmSchFillRgrUeSrCfg
(
 RgrUeSrCfg                   *ueCfg,
 WrUeCb                       *ueCb
 )
{
   /* Configure SR information */
   if(ueCb->bResCfgBitMap & WR_UE_RADIO_RES_CFG_SR)
   {
      ueCfg->type = RGR_SCH_SR_SETUP;
      ueCfg->srSetup.srResIdx = ueCb->srCfg.srPUCCHResIdx;
      ueCfg->srSetup.srCfgIdx = ueCb->srCfg.srCfgIdx;
      /* ccpu00130601 - DEL- dSrTransMax param is not used at Scheduler */
      RLOG1(L_DEBUG, "********SRCFGIDX AT SCH CONFIG[%d]",
         ueCfg->srSetup.srCfgIdx);
   }
   else
   {
      ueCfg->type = CTF_IE_CFG_RELEASE;
   }

   RETVALUE(ROK);
} /* wrUmmSchFillRgrUeSrCfg */


/**
 *  @brief This function fill the Sounding reference signal's 
 *  configuration for a UE
 *
 *      Function: wrUmmSchFillRgrUeSrsUlDedCfg
 *
 *          Processing steps:
 *          - populate RgrUeUlSrsCfg Structure
 *
 *  @param [out] ueCfg : pointer to UE Srs UL DedCfg
 *  @param [in]  ueCb  : pointer to Ue CB
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
PUBLIC S16 wrUmmSchFillRgrUeSrsUlDedCfg
(
 RgrUeUlSrsCfg                *ueCfg,
 WrUeCb                       *ueCb
 )
{
   /* Configure SRS information */
   ueCfg->type = ueCb->srsUlDedCfg.pres; /* Release = 0, Setup = 1 */
   ueCfg->srsSetup.srsCfgIdx  = ueCb->srsUlDedCfg.srsCfgIdx;
   ueCfg->srsSetup.srsBw      = ueCb->srsUlDedCfg.srsBw;
   ueCfg->srsSetup.srsHopBw   = ueCb->srsUlDedCfg.srsHoppingBw;
   ueCfg->srsSetup.cycShift   = ueCb->srsUlDedCfg.cyclicshift;
   ueCfg->srsSetup.duration   = ueCb->srsUlDedCfg.duration;
   ueCfg->srsSetup.sANSrs     = 0;
   ueCfg->srsSetup.txComb     = ueCb->srsUlDedCfg.transComb;
   ueCfg->srsSetup.fDomPosi   = ueCb->srsUlDedCfg.freqDomPos;

   RETVALUE(ROK);
} /* wrUmmSchFillRgrUeSrsUlDedCfg */
#endif /* end of TFU_UPGRADE */



/**
 *  @brief This function fill hte UE Ack Nack Repetition Confgiuration
 *
 *      Function: wrUmmSchFillRgrUeAckNackRepCfg
 *
 *          Processing steps:
 *          - populate RgrUeAckNackRepCfg Structure
 *
 *  @param [out] ueCfg : pointer to UE Ack Nack Repitition Configuration
 *  @param [in]  ueCb  : pointer to Ue CB
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
PUBLIC S16 wrUmmSchFillRgrUeAckNackRepCfg
(
 RgrUeAckNackRepCfg           *ueCfg,
 WrUeCb                       *ueCb
 )
{
   ueCfg->isAckNackEnabled = ueCb->ueAckNackRepCfg.isAckNackEnabled;
   ueCfg->pucchAckNackRep = ueCb->ueAckNackRepCfg.pucchAckNackRep;
   ueCfg->ackNackRepFactor = ueCb->ueAckNackRepCfg.ackNackRepFactor;


   RETVALUE(ROK);
} /* wrUmmSchFillRgrUeAckNackRepCfg */


/**
 *  @brief This function fills the UE measurement Gap Configuration
 *
 *      Function: wrUmmSchFillRgrUeMeasGapCfg
 *
 *          Processing steps:
 *          - populate RgrUeMeasGapCfg Structure
 *
 *  @param [out]  ueCfg  : pointer to UE measurement Gap configuration
 *  @param [in]   ueCb   : pointer to UE CB
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
PUBLIC S16 wrUmmSchFillRgrUeMeasGapCfg
(
 RgrUeMeasGapCfg               *ueCfg,
 WrUeCb                        *ueCb
 )
{
   TRC2(wrUmmSchFillRgrUeMeasGapCfg);

   ueCfg->isMesGapEnabled = ueCb->ueMeasGapCfg.isMesGapEnabled;
   ueCfg->gapPrd = ueCb->ueMeasGapCfg.gapPrd;
   ueCfg->gapOffst = ueCb->ueMeasGapCfg.gapOffst;


   RETVALUE(ROK);
} /* wrUmmSchFillRgrUeMeasGapCfg */


/**
 *  @brief This function fill UE capability confgiuraiton info
 *
 *      Function: wrUmmSchFillRgrUeCapCfg
 *
 *          Processing steps:
 *          - populate RgrUeCapCfg Structure
 *
 *  @param [out]  ueCfg : pointer to UE capability config Info
 *  @param [in]   ueCb  : pointer to UE control CB
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
PUBLIC S16 wrUmmSchFillRgrUeCapCfg
(
 RgrUeCapCfg                  *ueCfg,
 WrUeCb                       *ueCb
 )
{
   ueCfg->pwrClass      = ueCb->ueCapCfg.pwrClass;
   ueCfg->intraSfFeqHop = ueCb->ueCapCfg.intraSfFeqHop;
   ueCfg->resAloocType1 = ueCb->ueCapCfg.resAloocType1;
   ueCfg->simCqiAckNack = ueCb->ueCapCfg.simCqiAckNack;

   RETVALUE(ROK);
}


/**
 *  @brief This function fills the Code Book restriction Confgiuration Info 
 *
 *      Function: wrUmmSchFillRgrCodeBookRstCfg
 *
 *          Processing steps:
 *          - populate RgrCodeBookRstCfg Structure
 *
 *
 *  @param [out]  ueCfg : pointer to Code Book restriction configuration
 *  @param [in]   ueCb  : pointer to UE Control Block
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmSchFillRgrCodeBookRstCfg
(
 RgrCodeBookRstCfg            *ueCfg,
 WrUeCb                       *ueCb
 )
{
   ueCfg->pres = ueCb->antennaInfo.codebookpres;
   if(ueCfg->pres)
   {
      /* TENB_BRDCMTDD_TM4 */
      ueCfg->pmiBitMap[0] = wrPmiBitMap0;   
      ueCfg->pmiBitMap[1] = wrPmiBitMap1;
   }   

   RETVALUE(ROK);
}

/**
 *  @brief This function buids the scheduler UE configuraiotn info
 *  
 *      Function: wrUmmFillSchUeCfg
 *
 *          Processing steps:
 *          - Update the UE configuration Info
 *          - send the Ue cfg Info to scheduler
 *
 *  @param [in]  transId : transaction ID
 *  @param [in]  ueCb    : pointer to UE CB
 *  @param [out] cfgInfo : pointer to scheduler UE configuration Info 
 *                         structure to be updated
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmSchFillUeCfg
(
 U32                          transId,
 WrUeCb                       *ueCb,
 RgrCfgReqInfo                *cfgInfo
 )
{
   RgrUeCfg                  *ueCfg = NULLP;

   cfgInfo->action            = RGR_CONFIG;
   cfgInfo->u.cfgInfo.cfgType = RGR_UE_CFG;

   ueCfg                      = &(cfgInfo->u.cfgInfo.u.ueCfg);
   ueCfg->cellId              = ueCb->cellId;
   ueCfg->crnti               = ueCb->crnti;

   /* UE Transmission mode */
   if (wrSchFillRgrUeTxModeCfg(&ueCfg->txMode,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrSchFillRgrUeTxModeCfg failed");
      RETVALUE(RFAILED);
   }

   /* UE DL CQI config */
   if (wrUmmSchFillRgrUeDlCqiCfg(&ueCfg->ueDlCqiCfg,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrUeDlCqiCfg failed");
      RETVALUE(RFAILED);
   }

   /* UE related UL Harq Config */
   if (wrUmmSchFillRgrUeUlHqCfg(&ueCfg->ueUlHqCfg,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrUeUlHqCfg failed");
      RETVALUE(RFAILED);
   }

   /* UE UL power config */
   if(wrUmmSchFillRgrUeUlPwrCfg(&ueCfg->ueUlPwrCfg,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrUeUlPwrCfg failed");
      RETVALUE(RFAILED);
   }

   /* UE related Dl/UL QoS config: AMBR */
   if(wrUmmSchFillRgrUeQosCfg(&ueCfg->ueQosCfg,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrUeQosCfg failed");
      RETVALUE(RFAILED);
   }

   /* UE TA timer config */
   if(wrUmmSchFillRgrUeTaTmrCfg(&ueCfg->ueTaTmrCfg,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrUeTaTmrCfg failed");
      RETVALUE(RFAILED);
   }

   /* UE category */
   wrUmmSchdFillUeCategory(&ueCfg->ueCatEnum, ueCb->ueCat);

   /* Configure periodic bsr timer */
#ifdef RGR_V1
   wrUmmSchdFillPeriodicBsrTmr(&ueCfg->ueBsrTmrCfg, ueCb);
#endif

   /* TtiBundling Enable/Disabled for UE */
   ueCfg->isTtiBundlEnabled = FALSE;

   /* Ack/Nack Configuration for UE */
   if (wrUmmSchFillRgrUeAckNackRepCfg(&ueCfg->ueAckNackCfg,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrUeAckNackRepCfg failed");
      RETVALUE(RFAILED);
   }

   /* measurement gap configuration for UE */
   if(wrUmmSchFillRgrUeMeasGapCfg(&ueCfg->ueMesGapCfg,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrUeMeasGapCfg failed.");
      RETVALUE(RFAILED);
   }

   /* Ue Capabilty configuration */
   if (wrUmmSchFillRgrUeCapCfg(&ueCfg->ueCapCfg,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrUeCapCfg failed");
      RETVALUE(RFAILED);
   }

   /* Number of bits in codebook for Transmission modes */
   if (wrUmmSchFillRgrCodeBookRstCfg(&ueCfg->ueCodeBookRstCfg,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrCodeBookRstCfg failed");
      RETVALUE(RFAILED);
   }

#ifdef TFU_UPGRADE
   /* Sounding reference signal's configuration */
   if (wrUmmSchFillRgrUeSrsUlDedCfg(&ueCfg->srsCfg,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrUeSrsUlDedCfg failed");
      RETVALUE(RFAILED);
   }
   /* Scheduling Request Configuration */
   if (wrUmmSchFillRgrUeSrCfg(&ueCfg->srCfg,ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrUeSrCfg failed");
      RETVALUE(RFAILED);
   }

   /* PDSCH related dedicated configuration */
   ueCfg->uePdschDedCfg.uepACfg.pA = ueCb->pdschDedCfg.p_a;
   ueCfg->uePdschDedCfg.uepACfg.pAPrsnt = 1;

   ueCfg->puschDedCfg.pres = PRSNT_NODEF;
   ueCfg->puschDedCfg.bACKIdx = ueCb->puschDedCfg.ackIdx;
   ueCfg->puschDedCfg.bRIIdx  = ueCb->puschDedCfg.riIdx;
   ueCfg->puschDedCfg.bCQIIdx = ueCb->puschDedCfg.cqiIdx;
#endif

   /* If present then mapping exists at RGR user with crnti */
   ueCfg->dedPreambleId.pres = NOTPRSNT;

   RETVALUE(ROK);
} /* wrUmmFillSchUeCfg */

/**
 *  @brief This function build the UE release request and sends to scheduler
 *
 *      Function: wrUmmSchdUeRel
 *
 *          Processing steps:
 *          - Allocate the memeory to RgrCgReqInfo
 *          - update the UE release request
 *          - sne dthe request Info to UE
 *
 *  @param [in]   transId : transaction Id
 *  @param [in]   crnti   : CRNTI of the UE
 *  @param [in]   cellId; cell Id of the serving Cell
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED 
 */

PUBLIC S16 wrUmmSchdUeRel
(
 U32                          transId,
 U16                          crnti,
 U16                          cellId
 )
{
   RgrCfgReqInfo             *cfgInfo;

   WR_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));
   if(cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   cfgInfo->action = RGR_DELETE;
   cfgInfo->u.delInfo.delType = RGR_UE_CFG;
   cfgInfo->u.delInfo.u.ueDel.cellId = cellId;
   cfgInfo->u.delInfo.u.ueDel.crnti = crnti;

   if (WrIfmSchdCfgUe(cfgInfo, transId) != ROK)
   {
      RETVALUE(ROK);
   }

   RETVALUE(ROK);
}

/**
 *  @brief This function buids the message for UeId change
 *
 *      Function: wrUmmSchdUeIdChg
 *
 *          Processing steps:
 *          - allcate memory for the config request message
 *          - fill the UE Id cgnage request message
 *          - Dispatch the prepared message to scheduler
 *
 *  @param [in] transId : transaction Id
 *  @param [in] cellId  : cell Id of serving cell
 *  @param [in] oldRnti : RNTI to be changed
 *  @param [in] newRnti : New RNTI to be assigned
 *  @param [in] ueCb    : Ue Control Block
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */

PUBLIC S16 wrUmmSchdUeIdChg
(
 U32                          transId,
 U16                          cellId,
 U16                          oldRnti,
 U16                          newRnti,
 WrUeCb                       *ueCb
 )
{
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   cfgReq->action                = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg                       = &cfgReq->u.recfgInfo.u.ueRecfg;
   ueRecfg->cellId               = cellId;
   ueRecfg->oldCrnti             = oldRnti;
   ueRecfg->newCrnti             = newRnti;
   ueRecfg->ueUlHqRecfg.maxUlHqTx = ueCb->ueUlHqCfg.maxUlHqTx;

   /* Dispatch the prepared message to scheduler.                         */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
} /* end of wrUmmSchdUeIdChg */ 


/**
 *  @brief This function fills the Physical C=dedicated configuration info
 *         into the Scheduler UE Recfg info.
 *
 *  @details RRM reConfigures the different phy dedicated configurations.
 *           Based on the phy dedicated configuration reconfigured from RRM
 *           the UE Recfg Info will be updated with the new configurations
 *           which are reconfigured by the RRM
 *
 *      Function: wrUmmSchdFillUeRadioResCfgInfo 
 *
 *          Processing steps:
 *          - if flag phyCfgDedResBitMap for CQI update is set
 *             - update the ueReCfg with CQI info
 *
 *
 *  @param [in] ueRecfg          : pointer to Ue recfg info to be sent to Scheduler
 *  @param [in] ueRecfgRespPtr   : pointer to RRM Ue Recfg Response containing the 
 *                                 reconfigured info
 *  @return Void
 */

PUBLIC Void wrUmmSchdFillUeRadioResCfgInfo
(
 RgrUeRecfg                 *ueRecfg,
 RmuUeRecfgRsp              *ueRecfgRespPtr,
 WrUeCb                    *ueCb,
 Bool                      isTxModeChngTrig
 )
{
#ifdef TFU_UPGRADE
   RmuUeCqiReportCfg       *cqiRepCfg = NULLP;
#endif
   RmuAntInfo              *antInfo=NULLP;

   /*Removed DRX flag */   
   if((ueRecfgRespPtr->stUeRadioResCfg.bRadioResDedBitMap) & (U8)RM_RCM_PHY_DED_RES_CFG)
   {
      if((ueRecfgRespPtr->stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap) & 
            (U8)RM_RCM_PHY_DED_RES_CFG_CQI)
      {
#ifdef TFU_UPGRADE
         cqiRepCfg = &ueRecfgRespPtr->stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg;

         /* ccpu00128304 - CQI reporting related code removed from here */

#ifndef RGR_CQI_REPT
         ueRecfg->ueRecfgTypes |= RGR_UE_PCQI_RECFG;
#else
         ueRecfg->ueRecfgTypes |= RGR_UE_PCQI_RECFG | RGR_UE_CQIREPT_RECFG;
         ueRecfg->ueCqiReptCfg.numColltdCqiRept = 5; 
#endif

         ueRecfg->cqiCfg.type = 1; /* Setup */

         /* ccpu00128304 - CQI reporting related code removed from here */
         ueRecfg->cqiCfg.cqiSetup.cqiPResIdx = 
            cqiRepCfg->stPrdCqiRepCfg.usCqiResourceIndex;
         /*!< cqi-PUCCH-ResourceIndex (0.. 1185) */

         ueRecfg->cqiCfg.cqiSetup.cqiPCfgIdx =  
            cqiRepCfg->stPrdCqiRepCfg.usCqiConfigIndex;
         RLOG1(L_DEBUG, "********CQI IDX AT SCH RECONFIG=%d",
                  cqiRepCfg->stPrdCqiRepCfg.usCqiConfigIndex);
         /*!< cqi-pmi-ConfigIndex (0..1023) */
         if(RMU_PCQI_FMT_WIDEBAND == cqiRepCfg->stPrdCqiRepCfg.enCqiFormatInd)
         {
            ueRecfg->cqiCfg.cqiSetup.cqiRepType = RGR_UE_PCQI_WB_REP;
         }
         else
         {
            ueRecfg->cqiCfg.cqiSetup.cqiRepType = RGR_UE_PCQI_SB_REP;
            /*ccpu00148940 - num of Collated Rept is reduced to 2 in case of 
             *Sub-band periodic CQI reporting to avoid RLF.As only Wide Band Cqi reporting 
             *are indicated in the CQI report from MAC to Application */
            ueRecfg->ueCqiReptCfg.numColltdCqiRept = 2; 
         }
         /*!< Wideband CQI = 1  Subband CQI =2 */

         /* TODO : currently MAC-SCH requires the factor K to be configured
          * irrespective of CQI Report Type, Hence the the factor is set to 1.
          * The below line need to be removed once the dfependency is removed
          */
         ueRecfg->cqiCfg.cqiSetup.k = 1;

         if(ueRecfg->cqiCfg.cqiSetup.cqiRepType == RGR_UE_PCQI_SB_REP)
         {
            /* if Sub Band CQI configured */
            ueRecfg->cqiCfg.cqiSetup.k = 
               cqiRepCfg->stPrdCqiRepCfg.bFactorK;
            /*!< Ref: 36.213 [23, 7.2.2] (1..4). Valid only for Subband CQI */
         }

         if(cqiRepCfg->stPrdCqiRepCfg.riConfigIndex.bPres == TRUE)
         {
            /* SYED : Reconfiguring SCH for RI */
            ueRecfg->cqiCfg.cqiSetup.riEna = TRUE;/*Disbaling RI*/ 
            /*!< Rand Indicator is Enabled TRUE(1) for UE TM3 and TM4*/
            RLOG1(L_DEBUG,"********RI IDX AT SCH RECONFIG=%d",
                     cqiRepCfg->stPrdCqiRepCfg.riConfigIndex.usRiConfigIndex);
            ueRecfg->cqiCfg.cqiSetup.riCfgIdx = cqiRepCfg->stPrdCqiRepCfg.riConfigIndex.usRiConfigIndex; 
         }
         else
         {
            ueRecfg->cqiCfg.cqiSetup.riEna = FALSE; 
            /*!< Rand Indicator is Enabled TRUE(1) for UE TM3 and TM4*/
         }
         RLOG1(L_DEBUG,"********SIMULACKCQI IDX AT SCH RECONFIG=%d",
                  cqiRepCfg->stPrdCqiRepCfg.bSimulAckNackCqi);

         ueRecfg->cqiCfg.cqiSetup.sANCQI = cqiRepCfg->stPrdCqiRepCfg.bSimulAckNackCqi;
         /*!< simultaneousAckNackAndCQI TRUE(1) FALSE(0) */

         ueRecfg->cqiCfg.cqiSetup.prdModeEnum = \
            (RgrPrdCqiMode)cqiRepCfg->stPrdCqiRepCfg.enPucchReportMode;
         /*!< Peiodic CQI reporting mode */

         /* If RRM enabled Aperiodic CQI Reporting,
            reconfigure SCH accordingly */
         if (cqiRepCfg->stAPrdCqiRepCfg.enbld)
         {
            ueRecfg->ueRecfgTypes |= RGR_UE_APRD_DLCQI_RECFG;
            ueRecfg->aprdDlCqiRecfg.pres = TRUE;
            ueRecfg->aprdDlCqiRecfg.aprdModeEnum = 
               (RgrAprdCqiMode)cqiRepCfg->stAPrdCqiRepCfg.enPuschReportMode;
         }

      }

      if((ueRecfgRespPtr->stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap) & 
            (U8)RM_RCM_PHY_DED_RES_CFG_SR)
      {
         /* MS_WORKAROUND : syed Recfg SR to avoid collision with CQI */               
         ueRecfg->srCfg.type = RGR_SCH_SR_SETUP;
         /* syed ResIdx should also come from RRM */
         ueRecfg->srCfg.srSetup.srResIdx = wrSmDfltSrPUCCHResIdx;
         ueRecfg->srCfg.srSetup.srCfgIdx = cqiRepCfg->stPrdCqiRepCfg.srCfgIdx;
         /* ccpu00131601 */
         //ueRecfg->srCfg.srSetup.dTMax = RGR_DSR_TXMAX_4;
         RLOG1(L_DEBUG, "********SRCFGIDX AT SCH RECONFIG=%d",
                  ueRecfg->srCfg.srSetup.srCfgIdx);
#endif
      }
      if((ueRecfgRespPtr->stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap) & 
            (U8)RM_RCM_PHY_DED_RES_CFG_ANT)
      {
         U8 mode=0;
         antInfo = &ueRecfgRespPtr->stUeRadioResCfg.stPhyCfgDed.stAntInfo;
         
         ueRecfg->ueRecfgTypes |= RGR_UE_TXMODE_RECFG;
         WR_GET_RGR_TM(antInfo->eTxnMode,mode);
         ueRecfg->txMode.txModeEnum = mode; 
         /* Setting UeCB tx mode */
         WR_GET_NHU_TM(ueRecfg->txMode.txModeEnum, ueCb->antennaInfo.txmode);
         ueRecfg->txMode.pres = PRSNT_NODEF;

         if(isTxModeChngTrig)
         {   
            ueRecfg->txMode.tmTrnstnState = RGR_TXMODE_RECFG_START;
         }
         else
         {
            ueRecfg->txMode.tmTrnstnState = RGR_TXMODE_RECFG_CMPLT;
         }

         if((RMU_TXN_MODE1 == antInfo->eTxnMode)||(RMU_TXN_MODE2 == antInfo->eTxnMode))
         {
            ueRecfg->ueCodeBookRstRecfg.pres = NOTPRSNT;
         }
         else
         {
            ueRecfg->ueCodeBookRstRecfg.pres = PRSNT_NODEF;
            ueRecfg->ueCodeBookRstRecfg.pmiBitMap[0] = wrPmiBitMap0;
            ueRecfg->ueCodeBookRstRecfg.pmiBitMap[1] = wrPmiBitMap1;
         }
         RLOG1(L_DEBUG, "** Transmission Mode = %d **",ueRecfg->txMode.txModeEnum);
      }
   }
}/* End of wrUmmSchdFillUeRadioResCfgInfo */

/* SPS changes starts */
/**
 *  @brief This function converts UL Implicit Release After from RMU to RGR
 *
 *      Function: wrUmmSpsConvertImplicitRlsRmuToRgr
 *
 *          Processing steps:
 *          - Assign corresponding values
 *
 *
 *  @param [in] rmuSpsIntervalUl    : RMU UL SPS Interval
 *  @return RgrSpsImplRelCnt
 */
PRIVATE RgrSpsImplRelCnt wrUmmSpsConvertImplicitRlsRmuToRgr
(
 RmuSpsImplctRelAftr   eImplctRelAftr
)
{
   switch(eImplctRelAftr)
   {
      case RMU_SPS_E2:
         RETVALUE(RGR_SPS_E2);
      case RMU_SPS_E3:
         RETVALUE(RGR_SPS_E3);
      case RMU_SPS_E4:
         RETVALUE(RGR_SPS_E4);
      case RMU_SPS_E8:
         RETVALUE(RGR_SPS_E8);
      default:
         RETVALUE(RGR_SPS_E2);
   }
}

/**
 *  @brief This function converts DL Interval from RMU to RGR
 *
 *      Function: wrUmmSpsConvertIntervalDlRmuToRgr
 *
 *          Processing steps:
 *          - Assign corresponding values
 *
 *
 *  @param [in] rmuSpsIntervalDl    : RMU DL SPS Interval
 *  @return RgrSpsPrd
 */
PRIVATE RgrSpsPrd wrUmmSpsConvertIntervalDlRmuToRgr
(
 RmuSpsIntervalDl   rmuSpsIntervalDl
)
{
   switch(rmuSpsIntervalDl)
   {
      case RMU_SPS_DL_INTERVAL_E10:
         RETVALUE(RGR_SPS_PRD_10SF);
      case RMU_SPS_DL_INTERVAL_E20:
         RETVALUE(RGR_SPS_PRD_20SF);
      case RMU_SPS_DL_INTERVAL_E32:
         RETVALUE(RGR_SPS_PRD_32SF);
      case RMU_SPS_DL_INTERVAL_E40:
         RETVALUE(RGR_SPS_PRD_40SF);
      case RMU_SPS_DL_INTERVAL_E64:
         RETVALUE(RGR_SPS_PRD_64SF);
      case RMU_SPS_DL_INTERVAL_E80:
         RETVALUE(RGR_SPS_PRD_80SF);
      case RMU_SPS_DL_INTERVAL_E128:
         RETVALUE(RGR_SPS_PRD_128SF);
      case RMU_SPS_DL_INTERVAL_E160:
         RETVALUE(RGR_SPS_PRD_160SF);
      case RMU_SPS_DL_INTERVAL_E320:
         RETVALUE(RGR_SPS_PRD_320SF);
      case RMU_SPS_DL_INTERVAL_E640:
         RETVALUE(RGR_SPS_PRD_640SF);
      default:
         RETVALUE(RGR_SPS_PRD_20SF);
   }
}

/**
 *  @brief This function converts UL Interval from RMU to RGR
 *
 *      Function: wrUmmSpsConvertIntervalUlRmuToRgr
 *
 *          Processing steps:
 *          - Assign corresponding values
 *
 *
 *  @param [in] rmuSpsIntervalUl    : RMU UL SPS Interval
 *  @return RgrSpsPrd
 */
PRIVATE RgrSpsPrd wrUmmSpsConvertIntervalUlRmuToRgr
(
 RmuSpsIntervalUl   rmuSpsIntervalUl
)
{
   switch(rmuSpsIntervalUl)
   {
      case RMU_SPS_UL_INTERVAL_E10:
         RETVALUE(RGR_SPS_PRD_10SF);
      case RMU_SPS_UL_INTERVAL_E20:
         RETVALUE(RGR_SPS_PRD_20SF);
      case RMU_SPS_UL_INTERVAL_E32:
         RETVALUE(RGR_SPS_PRD_32SF);
      case RMU_SPS_UL_INTERVAL_E40:
         RETVALUE(RGR_SPS_PRD_40SF);
      case RMU_SPS_UL_INTERVAL_E64:
         RETVALUE(RGR_SPS_PRD_64SF);
      case RMU_SPS_UL_INTERVAL_E80:
         RETVALUE(RGR_SPS_PRD_80SF);
      case RMU_SPS_UL_INTERVAL_E128:
         RETVALUE(RGR_SPS_PRD_128SF);
      case RMU_SPS_UL_INTERVAL_E160:
         RETVALUE(RGR_SPS_PRD_160SF);
      case RMU_SPS_UL_INTERVAL_E320:
         RETVALUE(RGR_SPS_PRD_320SF);
      case RMU_SPS_UL_INTERVAL_E640:
         RETVALUE(RGR_SPS_PRD_640SF);
      default:
         RETVALUE(RGR_SPS_PRD_20SF);
   }
}

/**
 *  @brief This function fills the Sps configuration info
 *         into the Scheduler UE Cfg/Recfg info.
 *
 *      Function: wrUmmFillSpsSchdReCfgUe
 *
 *          Processing steps:
 *          - In case of UE reconfiguring to Scheduler
 *             - update the spsCfg
 *
 *
 *  @param [in] transType       : transaction type
 *  @param [in] ueCb            : pointer to UE Control Block
 *  @param [in] rrmUeRecfgResp  : pointer to RRM UE Recfg Resp
 *  @param [out] ueRecfg        : ue recfg to be filled for SCHD
 *  @param [in] numErabs        : Number of ERABs received in request
 *  @param [in] pErabList       : ERAB list from the transCb depending on transtype
 *  @return Void
 */

PUBLIC Void wrUmmFillSpsSchdReCfgUe
(
 U8            transType,
 WrUeCb        *ueCb,
 RmuUeRecfgRsp *rrmUeRecfgResp,
 RgrUeRecfg    *ueRecfg,
 U32           numErabs,
 Void          *pErabList,
 WrSpsCfg      *pSpsCfg,
 Bool          *pIsUeDlSpsEnabled,
 Bool          *pIsUeUlSpsEnabled
)
{
   U32         count               = 0;
   S16         ret                 = RFAILED;
   Bool        isSpsNotEnabledErr  = FALSE;
   RmuUeSpsCfg *stRmuSpsCfg        = &rrmUeRecfgResp->stUeRadioResCfg.stSpsCfg;
#ifdef LTE_TDD
   U16         duplexMode          = NHU_MODE_FDD;
#endif
   
   pSpsCfg->spsCfgDl.bIsSpsEnabled = FALSE;
   pSpsCfg->spsCfgUl.bIsSpsEnabled = FALSE;
   do
   {
      if ((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG) && \
            (ueCb->spsCfg.spsCfgUl.bIsSpsEnabled != stRmuSpsCfg->stSpsConfigUl.bIsSpsEnabled))
      {
         if (stRmuSpsCfg->stSpsConfigUl.bIsSpsEnabled == TRUE)
         {
            /* Configure SPS for ERAB */
            switch(transType)
            {
               case WR_UMM_INIT_CTXT_TRANS:
                  wrUmmInitCtxtSchdUlSpsLchCfgReq(rrmUeRecfgResp, numErabs, pErabList, &ueRecfg->ueSpsRecfg);
                  break;
               case WR_UMM_ERAB_SETUP_TRANS:
                  wrUmmRabSetupSchdUlSpsLchCfgReq(rrmUeRecfgResp, numErabs, pErabList, &ueRecfg->ueSpsRecfg);
                  break;
               case WR_UMM_ERAB_MOD_TRANS:
                  wrUmmRabModifySchdUlSpsLchCfgReq(rrmUeRecfgResp, numErabs, pErabList, &ueRecfg->ueSpsRecfg, ueCb);
                  break;
               case WR_UMM_RAB_REL_TRANS:
                  /* The keeping/modifying SPS configuiration is for future enhancement when more than one ERAB is 
                   * configured as SPS and one of then is deleted as a result RRM has modified the SPS configuration.*/
                  /* With our current implementation, we will never encounter this case */
                  wrUmmRabRelSchdUlSpsLchCfgReq(rrmUeRecfgResp, numErabs, pErabList, &ueRecfg->ueSpsRecfg);
                  isSpsNotEnabledErr = TRUE;
                  break;
               default:
                  /* Currently no scenario. */
                  isSpsNotEnabledErr = TRUE;
                  break;
            }
            if (isSpsNotEnabledErr == TRUE)
            {
               break;
            }
            /* if ((ueCb->spsCfg.spsCfgDl.bIsSpsEnabled == FALSE) && (ueCb->spsCfg.spsCfgUl.bIsSpsEnabled == FALSE)) */
            if (ueCb->spsCfg.spsCfgDl.bIsSpsEnabled == FALSE)
            {
               if (ROK != wrUtlGetSpsRnti(&pSpsCfg->spsRnti, ueCb->cellId))
               {
                  isSpsNotEnabledErr = TRUE;
                  break;
               }
            }
            else
            {
               pSpsCfg->spsRnti = ueCb->spsCfg.spsRnti;
            }
            ueRecfg->ueSpsRecfg.ulSpsCfg.isLcSRMaskEnab = FALSE;
            ueRecfg->ueSpsRecfg.spsRnti                   = pSpsCfg->spsRnti;
            ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled   = TRUE;
            ueRecfg->ueSpsRecfg.ulSpsCfg.implicitRelCnt   = wrUmmSpsConvertImplicitRlsRmuToRgr(stRmuSpsCfg->stSpsConfigUl.eImplctRelAftr);
#ifdef LTE_TDD
            if ((duplexMode = wrEmmGetDuplexMode(ueCb->cellId)) == NHU_MODE_TDD)
            {
               ueRecfg->ueSpsRecfg.ulSpsCfg.twoIntervalCfg   = stRmuSpsCfg->stSpsConfigUl.bTwoIntervalCfg;
            }
#endif
            ueRecfg->ueSpsRecfg.ulSpsCfg.pwrCfgPres       = stRmuSpsCfg->stSpsConfigUl.bP0PersistentPres;
            ueRecfg->ueSpsRecfg.ulSpsCfg.pwrCfg.p0NominalPuschVal = stRmuSpsCfg->stSpsConfigUl.stP0Persistent.sP0NominalPUSCHPersistent;
            ueRecfg->ueSpsRecfg.ulSpsCfg.pwrCfg.p0UePuschVal      = stRmuSpsCfg->stSpsConfigUl.stP0Persistent.sP0UEPuschPersistent;
            ueRecfg->ueSpsRecfg.ulSpsCfg.ulSpsPrdctyEnum  = wrUmmSpsConvertIntervalUlRmuToRgr(stRmuSpsCfg->stSpsConfigUl.eSpsIntervalUl);

            pSpsCfg->spsCfgUl = stRmuSpsCfg->stSpsConfigUl;
            *pIsUeUlSpsEnabled = TRUE;
            RLOG1(L_DEBUG, "wrUmmFillSpsSchdReCfgUe: UL SPS is enabled: *pIsUeUlSpsEnabled=[%u]", *pIsUeUlSpsEnabled);
         }
         else
         {
            /* Release SPS */
            ret = wrUtlFreeSpsRnti(ueCb->spsCfg.spsRnti, ueCb->cellId);
            if (ret != ROK)
            {
               RLOG1(L_ERROR, "wrUtlFreeSpsRnti: SPS RNTI [%u] not found" \
                     " in inuse list", ueCb->spsCfg.spsRnti);
            }
            ueRecfg->ueSpsRecfg.ulSpsCfg.isUlSpsEnabled  = FALSE;
            *pIsUeUlSpsEnabled = FALSE;
            RLOG1(L_DEBUG, "wrUmmFillSpsSchdReCfgUe: UL SPS is disabled: *pIsUeUlSpsEnabled=[%u]", *pIsUeUlSpsEnabled);
         }
         ueRecfg->ueRecfgTypes |= RGR_UE_ULSPS_RECFG;
      }
      if ((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG) && \
            (ueCb->spsCfg.spsCfgDl.bIsSpsEnabled != stRmuSpsCfg->stSpsConfigDl.bIsSpsEnabled))
      {
         if (stRmuSpsCfg->stSpsConfigDl.bIsSpsEnabled == TRUE)
         {
            if (ueCb->spsCfg.spsCfgUl.bIsSpsEnabled == TRUE)
            {
               pSpsCfg->spsRnti = ueCb->spsCfg.spsRnti;
            }
            else if (pSpsCfg->spsCfgUl.bIsSpsEnabled == FALSE)
            {
               if (ROK != wrUtlGetSpsRnti(&ueCb->spsCfg.spsRnti, ueCb->cellId))
               {
                  isSpsNotEnabledErr = TRUE;
                  break;
               } 
            }

            /* Configure SPS for ERAB */
            ueRecfg->ueSpsRecfg.spsRnti                  = pSpsCfg->spsRnti;
            ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled  = TRUE;
            ueRecfg->ueSpsRecfg.dlSpsCfg.numSpsHqProc    = stRmuSpsCfg->stSpsConfigDl.usNumOfConfSpsProc;
            ueRecfg->ueSpsRecfg.dlSpsCfg.dlSpsPrdctyEnum = \
                                                           wrUmmSpsConvertIntervalDlRmuToRgr(stRmuSpsCfg->stSpsConfigDl.eSpsIntervalDl);
            ueRecfg->ueSpsRecfg.dlSpsCfg.explicitRelCnt  = stRmuSpsCfg->stSpsConfigDl.usExplctRelAftr;
            ueRecfg->ueSpsRecfg.dlSpsCfg.numPucchVal     = stRmuSpsCfg->stSpsConfigDl.usNumN1Pucch;
            for (count = 0; count < ueRecfg->ueSpsRecfg.dlSpsCfg.numPucchVal; count++)
            {
               ueRecfg->ueSpsRecfg.dlSpsCfg.n1PucchVal[count] = stRmuSpsCfg->stSpsConfigDl.usN1Pucch[count];
            }
            pSpsCfg->spsCfgDl = stRmuSpsCfg->stSpsConfigDl;
            *pIsUeDlSpsEnabled = TRUE;
            RLOG1(L_DEBUG, "wrUmmSchdFillUeRadioResCfgInfo: DL SPS enabled: *pIsUeDlSpsEnabled=[%u]", *pIsUeDlSpsEnabled);
         }
         else
         {
            /* Release SPS */
            if (ret != ROK)
            {
               ret = wrUtlFreeSpsRnti(ueCb->spsCfg.spsRnti, ueCb->cellId);
               if (ret != ROK)
               {
                  RLOG1(L_ERROR, "wrUtlFreeSpsRnti: SPS RNTI [%u] not found" \
                        " in inuse list", ueCb->spsCfg.spsRnti);
               }
            }
            ueRecfg->ueSpsRecfg.dlSpsCfg.isDlSpsEnabled  = FALSE;
            *pIsUeDlSpsEnabled = FALSE;
            RLOG1(L_DEBUG, "wrUmmFillSpsSchdReCfgUe: DL SPS is disabled: *pIsUeUlSpsEnabled=[%u]", *pIsUeDlSpsEnabled);
         }
         ueRecfg->ueRecfgTypes |= RGR_UE_DLSPS_RECFG;
      }
   } while (0); /* Break in case of any error */

   if (isSpsNotEnabledErr == TRUE)
   {
      /* Donot allocate SPS and indicate the same to RRM */
      rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap &= ~RM_RCM_DL_SPS_CFG;
      rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap &= ~RM_RCM_UL_SPS_CFG;

      /* Send RRM an indication stating that the APP is not able to enable SPS */
      wrUmmRrmUeSpsDisInd((U8)ueCb->cellId, ueCb->crnti);
   }
   
   RETVOID;
}

/**
 *  @brief This function fills the Sps configuration info
 *         into the Scheduler UE Cfg/Recfg info.
 *
 *      Function: wrUmmFillSpsSchdCfgUe
 *
 *          Processing steps:
 *          - In case of UE configuring to Scheduler
 *             - update the spsCfg
 *
 *
 *  @param [in] transType       : transaction type
 *  @param [in] ueCb            : pointer to UE Control Block
 *  @param [in] rrmUeRecfgResp  : pointer to RRM UE Recfg Resp
 *  @param [out] ueRecfg        : ue recfg to be filled for SCHD
 *  @param [in] numErabs        : Number of ERABs received in request
 *  @param [in] pErabList       : ERAB list from the transCb depending on transtype
 *  @return Void
 */

PUBLIC Void wrUmmFillSpsSchdCfgUe
(
 U8            transType,
 WrUeCb        *ueCb,
 RmuUeRecfgRsp *rrmUeRecfgResp,
 RgrUeCfg      *ueCfg,
 U32           numErabs,
 Void          *pErabList,
 U32           accessStratumRls,
 WrSpsCfg      *pSpsCfg,
 Bool          *pIsUeDlSpsEnabled,
 Bool          *pIsUeUlSpsEnabled
)
{
   U32         count              = 0;
   S16         ret                = RFAILED;
   Bool        isSpsNotEnabledErr = FALSE;
   RmuUeSpsCfg *stRmuSpsCfg       = &rrmUeRecfgResp->stUeRadioResCfg.stSpsCfg;
#ifdef LTE_TDD
   U16         duplexMode         = NHU_MODE_FDD;
#endif
   pSpsCfg->spsCfgDl.bIsSpsEnabled = FALSE;
   pSpsCfg->spsCfgUl.bIsSpsEnabled = FALSE;
   do
   {
      if (rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG)
      {
         if (stRmuSpsCfg->stSpsConfigUl.bIsSpsEnabled == TRUE)
         {
            /* Configure SPS for ERAB */
            switch(transType)
            {
               case WR_UMM_S1_TGT_HO_TRANS:
                  wrUmmS1TgtHoSchdUlSpsLchCfgReq(rrmUeRecfgResp, numErabs, pErabList, &ueCfg->ueSpsCfg);
                  break;
               case WR_UMM_X2_TGT_HO_TRANS:
                  wrUmmX2TgtHoSchdUlSpsLchCfgReq(rrmUeRecfgResp, numErabs, pErabList, &ueCfg->ueSpsCfg);
                  break;
               default:
                  /* Currently no scenario. */
                  isSpsNotEnabledErr = TRUE;
                  break;
            }
            if (isSpsNotEnabledErr == TRUE)
            {
               break;
            }
            if ((ueCb->spsCfg.spsCfgDl.bIsSpsEnabled == FALSE) && (ueCb->spsCfg.spsCfgUl.bIsSpsEnabled == FALSE))
            {
               if (ROK != wrUtlGetSpsRnti(&pSpsCfg->spsRnti, ueCb->cellId))
               {
                  isSpsNotEnabledErr = TRUE;
                  break;
               }
            }
            else
            {
               pSpsCfg->spsRnti = ueCb->spsCfg.spsRnti;
            }
            ueCfg->ueSpsCfg.ulSpsCfg.isLcSRMaskEnab = FALSE;
            ueCfg->ueSpsCfg.spsRnti                   = pSpsCfg->spsRnti;
            ueCfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled   = TRUE;
            ueCfg->ueSpsCfg.ulSpsCfg.implicitRelCnt   = wrUmmSpsConvertImplicitRlsRmuToRgr(stRmuSpsCfg->stSpsConfigUl.eImplctRelAftr);
#ifdef LTE_TDD
            if ((duplexMode = wrEmmGetDuplexMode(ueCb->cellId)) == NHU_MODE_TDD)
            {
               ueCfg->ueSpsCfg.ulSpsCfg.twoIntervalCfg   = stRmuSpsCfg->stSpsConfigUl.bTwoIntervalCfg;
            }
#endif
            ueCfg->ueSpsCfg.ulSpsCfg.pwrCfgPres       = stRmuSpsCfg->stSpsConfigUl.bP0PersistentPres;
            ueCfg->ueSpsCfg.ulSpsCfg.pwrCfg.p0NominalPuschVal = stRmuSpsCfg->stSpsConfigUl.stP0Persistent.sP0NominalPUSCHPersistent;
            ueCfg->ueSpsCfg.ulSpsCfg.pwrCfg.p0UePuschVal      = stRmuSpsCfg->stSpsConfigUl.stP0Persistent.sP0UEPuschPersistent;
            ueCfg->ueSpsCfg.ulSpsCfg.ulSpsPrdctyEnum  = wrUmmSpsConvertIntervalUlRmuToRgr(stRmuSpsCfg->stSpsConfigUl.eSpsIntervalUl);

            pSpsCfg->spsCfgUl = stRmuSpsCfg->stSpsConfigUl;
            *pIsUeUlSpsEnabled = TRUE;
         }
         else
         {
            /* Release SPS */
            ret = wrUtlFreeSpsRnti(ueCb->spsCfg.spsRnti, ueCb->cellId);
            if (ret != ROK)
            {
               RLOG1(L_ERROR, "wrUtlFreeSpsRnti: SPS RNTI [%u] not found" \
                     " in inuse list", ueCb->spsCfg.spsRnti);
            }
            ueCfg->ueSpsCfg.ulSpsCfg.isUlSpsEnabled  = FALSE;
            *pIsUeUlSpsEnabled = FALSE;
            /* ueCb->spsCfg.spsCfgUl.bIsSpsEnabled = FALSE; */
         }
      }
   if (rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG)
   {
      if (stRmuSpsCfg->stSpsConfigDl.bIsSpsEnabled == TRUE)
   {
         if ((ueCb->spsCfg.spsCfgDl.bIsSpsEnabled == TRUE) || (ueCb->spsCfg.spsCfgUl.bIsSpsEnabled == TRUE))
      {
            pSpsCfg->spsRnti = ueCb->spsCfg.spsRnti;
         }
         else if (pSpsCfg->spsCfgUl.bIsSpsEnabled == FALSE)
         {
               if (ROK != wrUtlGetSpsRnti(&ueCb->spsCfg.spsRnti, ueCb->cellId))
               {
                  isSpsNotEnabledErr = TRUE;
                  break;
               }
         }
         /* Configure SPS for ERAB */
         ueCfg->ueSpsCfg.spsRnti                  = pSpsCfg->spsRnti;
         ueCfg->ueSpsCfg.dlSpsCfg.isDlSpsEnabled  = TRUE;
         ueCfg->ueSpsCfg.dlSpsCfg.numSpsHqProc    = stRmuSpsCfg->stSpsConfigDl.usNumOfConfSpsProc;
         ueCfg->ueSpsCfg.dlSpsCfg.dlSpsPrdctyEnum = wrUmmSpsConvertIntervalDlRmuToRgr(stRmuSpsCfg->stSpsConfigDl.eSpsIntervalDl);
         ueCfg->ueSpsCfg.dlSpsCfg.explicitRelCnt  = stRmuSpsCfg->stSpsConfigDl.usExplctRelAftr;
         ueCfg->ueSpsCfg.dlSpsCfg.numPucchVal     = stRmuSpsCfg->stSpsConfigDl.usNumN1Pucch;
         for (count = 0; count < ueCfg->ueSpsCfg.dlSpsCfg.numPucchVal; count++)
         {
            ueCfg->ueSpsCfg.dlSpsCfg.n1PucchVal[count] = stRmuSpsCfg->stSpsConfigDl.usN1Pucch[count];
         }
         pSpsCfg->spsCfgDl = stRmuSpsCfg->stSpsConfigDl;
         *pIsUeDlSpsEnabled = TRUE;
      }
      else
      {
         /* Release SPS */
         if (ret != ROK)
         {
            ret = wrUtlFreeSpsRnti(ueCb->spsCfg.spsRnti, ueCb->cellId);
            if (ret != ROK)
            {
               RLOG1(L_ERROR, "wrUtlFreeSpsRnti: SPS RNTI [%u] not found" \
                     " in inuse list", ueCb->spsCfg.spsRnti);
            }
         }
         ueCfg->ueSpsCfg.dlSpsCfg.isDlSpsEnabled  = FALSE;
         *pIsUeDlSpsEnabled = FALSE;
         /* ueCb->spsCfg.spsCfgDl.bIsSpsEnabled = FALSE; */
      }
   }
   } while (0);
   if (isSpsNotEnabledErr == TRUE)
   {
      /* Donot allocate SPS and indicate the same to RRM */
      rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap &= ~RM_RCM_DL_SPS_CFG;
      rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap &= ~RM_RCM_UL_SPS_CFG;

      /* Send RRM an indication stating that the APP is not able to enable SPS */
      wrUmmRrmUeSpsDisInd((U8)ueCb->cellId, ueCb->crnti);
   }
}
/* SPS changes ends */
/**
 *  @brief This function fills the Ue Category configuration info
 *         into the Scheduler UE Cfg/Recfg info.
 *
 *      Function: wrUmmSchdFillUeCategory
 *
 *          Processing steps:
 *          - In case of UE configuring/reconfiguring to Scheduler
 *             - update the ueCatEnum value
 *
 *
 *  @param [in] ueCatEnum  : pointer to UeCategory value to be sent to Scheduler
 *  @param [in] ueCat          : ue category value taken from UE Capability message
 *  @return Void
 */

PUBLIC Void wrUmmSchdFillUeCategory
(
   CmLteUeCategory            *ueCatEnum,
   U8                         ueCat
)
{
   switch(ueCat)
   {
      case 1: 
         *ueCatEnum = CM_LTE_UE_CAT_1;
         break;
      case 2: 
         *ueCatEnum = CM_LTE_UE_CAT_2;
         break;
      case 3: 
         *ueCatEnum = CM_LTE_UE_CAT_3;
         break;
      case 4: 
         *ueCatEnum = CM_LTE_UE_CAT_4;
         break;
      case 5: 
         *ueCatEnum = CM_LTE_UE_CAT_5;
         break;
         
      default: 
         *ueCatEnum = CM_LTE_UE_CAT_1;
         break;
   }

   /*ccpu00127802*/
   RLOG1(L_DEBUG, "Filled UE Category value[%d]", *ueCatEnum);
}/* End of wrUmmSchdFillUeCategory */


#ifdef RGR_V1
/**
 *  @brief This function fills the Periodic BSR timer
 *         into the Scheduler UE Cfg info.
 *
 *      Function: wrUmmSchdFillPeriodicBsrTmr
 *
 *          Processing steps:
 *          - In case of UE configuring to Scheduler
 *             - update the BSR timer value
 *
 *
 *  @param [in] ueBsrTmrCfg      : pointer to Bsr timer cfg value to be sent to Scheduler
 *  @param [in] ueCb                  : Ue Control Block
 *  @return Void
 */

PUBLIC Void wrUmmSchdFillPeriodicBsrTmr
(
   RgrUeBsrTmrCfg            *ueBsrTmrCfg,
   WrUeCb                    *ueCb
)
{
   ueBsrTmrCfg->isPrdBsrTmrPres  = TRUE;

   switch(ueCb->uePerBsrTmrCfg.prdBsrTmr)
   {
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf5Enum:
         ueBsrTmrCfg->prdBsrTmr = 5;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf10Enum:
         ueBsrTmrCfg->prdBsrTmr = 10;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf16Enum:
         ueBsrTmrCfg->prdBsrTmr = 16;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf20Enum:
         ueBsrTmrCfg->prdBsrTmr = 20;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf32Enum:
         ueBsrTmrCfg->prdBsrTmr = 32;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf40Enum:
         ueBsrTmrCfg->prdBsrTmr = 40;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf64Enum:
         ueBsrTmrCfg->prdBsrTmr = 64;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf80Enum:
         ueBsrTmrCfg->prdBsrTmr = 80;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf128Enum:
         ueBsrTmrCfg->prdBsrTmr = 128;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf160Enum:
         ueBsrTmrCfg->prdBsrTmr = 160;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf320Enum:
         ueBsrTmrCfg->prdBsrTmr = 320;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf640Enum:
         ueBsrTmrCfg->prdBsrTmr = 640;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf1280Enum:
         ueBsrTmrCfg->prdBsrTmr = 1280;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timersf2560Enum:
         ueBsrTmrCfg->prdBsrTmr = 2560;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_TimerinfinityEnum:
         ueBsrTmrCfg->prdBsrTmr = 0;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigperiodicBSR_Timerspare1Enum:
         ueBsrTmrCfg->prdBsrTmr = 0;
         break;
         
      default:
         ueBsrTmrCfg->prdBsrTmr = 10;
         break;
   }

   switch(ueCb->uePerBsrTmrCfg.retxBsrTmr)
   {
      case NhuMAC_MainConfigul_SCH_ConfigretxBSR_Timersf320Enum:
         ueBsrTmrCfg->retxBsrTmr = 320;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigretxBSR_Timersf640Enum:
         ueBsrTmrCfg->retxBsrTmr = 640;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigretxBSR_Timersf1280Enum:
         ueBsrTmrCfg->retxBsrTmr = 1280;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigretxBSR_Timersf2560Enum:
         ueBsrTmrCfg->retxBsrTmr = 2560;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigretxBSR_Timersf5120Enum:
         ueBsrTmrCfg->retxBsrTmr = 5120;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigretxBSR_Timersf10240Enum:
         ueBsrTmrCfg->retxBsrTmr = 10240;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigretxBSR_Timerspare2Enum:
         ueBsrTmrCfg->retxBsrTmr = 0;
         break;
      case NhuMAC_MainConfigul_SCH_ConfigretxBSR_Timerspare1Enum:
         ueBsrTmrCfg->retxBsrTmr = 0;
         break;
         
      default:
         ueBsrTmrCfg->retxBsrTmr = 0;
         break;
   }
}/* End of wrUmmSchdFillPeriodicBsrTmr */
#endif

/* ccpu00128203 */
/** @brief This function performs logical channel group
 *         configuration towards MAC Scheduler.
 *
 * @details
 *     Function: wrUmmSchdUeLcgCfg
 *
 *         Processing steps:
 *         - Builds the logical channel group configuration based on the inputs
 *         - Delivers the PDU towards MAC Scheduler through IFM module
 *
 * @param WrUeCb        ueCb        UE Control Block
 * @param U32           transId     Transaction ID
 * @param U8            grpId       Logical Channel Group ID
 * @param U32           ulGbr       Total UL GBR.
 * @return ROK/RFAILED
 */
PUBLIC S16 wrUmmSchdUeLcgCfg
(
WrUeCb                       *ueCb,
U32                          transId,
U8                           grpId,
U32                          ulGbr,
U32                          ulMbr
)
{
   RgrCfgReqInfo             *cfgReq;
   RgrLcgCfg                 *lcgCfg;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   cfgReq->action            = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_LCG_CFG;

   lcgCfg         = &cfgReq->u.cfgInfo.u.lcgCfg;
   lcgCfg->cellId = ueCb->cellId;
   lcgCfg->crnti  = ueCb->crnti;
   /* Uplink logical channel configuration information */
   lcgCfg->ulInfo.lcgId = grpId;
   lcgCfg->ulInfo.gbr = ulGbr;
   lcgCfg->ulInfo.mbr = ulMbr;

   /* Dispatch the prepared message to scheduler                         */
   transId |= (WR_UMM_TRANS_ID_LCG0_CFG + grpId);
   WrIfmSchdCfgUe(cfgReq, transId);

   RETVALUE(ROK);
}

/** @brief This function performs logical channel group
 *         reconfiguration towards MAC Scheduler.
 *
 * @details
 *     Function: wrUmmSchdUeLcgReCfg
 *
 *         Processing steps:
 *         - Builds the logical channel group configuration based on the inputs
 *         - Delivers the PDU towards MAC Scheduler through IFM module
 *
 * @param WrUeCb        ueCb        UE Control Block
 * @param U32           transId     Transaction ID
 * @param U8            grpId       Logical Channel Group ID
 * @param U32           ulGbr       Total UL GBR in bites/sec.
 * @return ROK/RFAILED
 */
PUBLIC S16 wrUmmSchdUeLcgReCfg
(
WrUeCb                       *ueCb,
U32                          transId,
U8                           grpId,
U32                          ulGbr,
U32                          ulMbr
)
{
   RgrCfgReqInfo             *cfgReq;
   RgrLcgRecfg               *lcgCfg;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   cfgReq->action                = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_LCG_CFG;

   lcgCfg         = &cfgReq->u.recfgInfo.u.lcgRecfg;
   lcgCfg->cellId = ueCb->cellId;
   lcgCfg->crnti  = ueCb->crnti;
   /* Uplink logical channel configuration information */
   lcgCfg->ulRecfg.lcgId = grpId;
   lcgCfg->ulRecfg.gbr = ulGbr;
   lcgCfg->ulRecfg.mbr = ulMbr;
   /* Dispatch the prepared message to scheduler                         */
   transId |= (WR_UMM_TRANS_ID_LCG0_CFG + grpId);
   WrIfmSchdCfgUe(cfgReq, transId);

   RETVALUE(ROK);
}


/** @brief This function performs logical channel group
 *         delete towards MAC Scheduler.
 *
 * @details
 *     Function: wrUmmSchdUeLcgDel
 *
 *         Processing steps:
 *         - Delete the logical channel group configuration based on the inputs
 *         - Delivers the PDU towards MAC Scheduler through IFM module
 *
 * @param WrUeCb        ueCb        UE Control Block
 * @param U32           transId     Transaction ID
 * @param U8            grpId       Logical Channel Group ID
 * @return ROK/RFAILED
 */
PUBLIC S16 wrUmmSchdUeLcgDel
(
WrUeCb                       *ueCb,
U32                          transId,
U8                           grpId
)
{
   RgrCfgReqInfo             *cfgReq;
   RgrDel                    *lcgDel;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RETVALUE(RFAILED);
   }

   cfgReq->action                = RGR_DELETE;
   cfgReq->u.delInfo.delType     = RGR_LCG_CFG;

   lcgDel         = &cfgReq->u.delInfo;
   lcgDel->u.lcgDel.cellId = ueCb->cellId;
   lcgDel->u.lcgDel.crnti  = ueCb->crnti;
   lcgDel->u.lcgDel.lcgId  = grpId;
   /* Dispatch the prepared message to scheduler                         */
   transId |= (WR_UMM_TRANS_ID_LCG0_CFG + grpId);
   WrIfmSchdCfgUe(cfgReq, transId);

   RETVALUE(ROK);
}

/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
