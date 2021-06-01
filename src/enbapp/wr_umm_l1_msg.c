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
               

     File:     wr_umm_l1_msg.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=59;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_umm_l1_msg.h"
#include "wr_ifm_l1.h"


/**
 *  @brief This function build Ctf Ue config
 *
 *      Function: wrUmmFillPhyUeCfg
 *
 *          Processing steps:
 *          - update the structure from the UE CB
 *          - call the interfce function to send the message
 *
 *
 *  @param [in]  transId : transid of the UE config message sent to Ctf
 *  @param [in]  ueCb    : pointer to UeCb of the UE for which ueCfg 
 *                         message is sent
 *  @param [out] cfgInfo : pointer to be update with UE config Info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmPhyFillUeCfg
(
U32                          transId,
WrUeCb                       *ueCb,
CtfCfgReqInfo                *cfgInfo
)
{
   CtfDedCfgInfo             *ueCfgParam = NULLP;

   cfgInfo->cfgType = CTF_CONFIG;
   cfgInfo->u.cfg.cfgElem = CTF_UE_CFG;
   ueCfgParam = &cfgInfo->u.cfg.u.dedCfg;
   if(ueCb == NULLP)
   {
      RLOG0(L_ERROR, "Invalid UeCb");
      RETVALUE(RFAILED);
   }
   else
   {
      ueCfgParam->cellId = ueCb->cellId;
      ueCfgParam->ueId = ueCb->crnti;
   }

   /* Ue category */
   ueCfgParam->ueCatCfg.pres = TRUE;
   ueCfgParam->ueCatCfg.ueCategory = ueCb->ueCat;

   /* PDSCH Cfg */
   ueCfgParam->pdschCfg.pres = TRUE;
   ueCfgParam->pdschCfg.pA = ueCb->pdschDedCfg.p_a;

   /* PUCCH Cfg */
   ueCfgParam->pucchCfg.pres = ueCb->pucchDedCfg.pres;
   ueCfgParam->pucchCfg.dedPucchCfgType = CTF_IE_CFG_SETUP;
   ueCfgParam->pucchCfg.pucchSetup.repFact = ueCb->pucchDedCfg.repeatFactor;
   ueCfgParam->pucchCfg.pucchSetup.n1PUCCHRep = ueCb->pucchDedCfg.n1pucchAn;

   /* PUSCH Cfg */
   ueCfgParam->puschCfg.pres = TRUE;
   ueCfgParam->puschCfg.betaOffsetAckIdx = ueCb->puschDedCfg.ackIdx;
   ueCfgParam->puschCfg.betaOffsetRiIdx = ueCb->puschDedCfg.riIdx;
   ueCfgParam->puschCfg.betaOffsetCqiIdx = ueCb->puschDedCfg.cqiIdx;

/* Fix: tfu upgrade changes */
#ifndef TFU_UPGRADE
   if(ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_CQI)
   {
      /* CQI Reporting */

      /*Fix for ccpu00126943- removal of platform name macro*/

      ueCfgParam->cqiRptCfg.pres = TRUE;

      ueCfgParam->cqiRptCfg.reportingMode = CTF_CQI_RPTMODE_PRDIOC;

      /* APERIODIC is not supported for now */
      ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiPeriodicCfgType 
         = CTF_IE_CFG_SETUP;
      ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPUCCHRsrcIndx 
         = ueCb->cqiRptCfg.cqiPucchRsrcIdx;
      ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.cqiPmiCfgIndx 
         = ueCb->cqiRptCfg.cqipmiCfgIdx;
      ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.formatIndicator 
         = ueCb->cqiRptCfg.fmtInd; 
      ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.subbandCqi.k 
         = ueCb->cqiRptCfg.subbndCqik;
      ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.riCfgPres
         = FALSE;
      ueCfgParam->cqiRptCfg.reportMode.periodicRpt.cqiSetup.simultaneousAckNack
         = ueCb->cqiRptCfg.simulAckNackAndCQI;
   }
   else
   {
      ueCfgParam->cqiRptCfg.pres = FALSE;
   }
   /* UL SRS Cfg */
   ueCfgParam->srsUlCfg.pres = ueCb->srsUlDedCfg.pres;
   if(ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_SR)
   {
      if (ueCb->srsUlDedCfg.pres)
      {
         ueCfgParam->srsUlCfg.dedSrsUlCfgType            = CTF_IE_CFG_SETUP;
         ueCfgParam->srsUlCfg.dedSrsSetup.srsBw          = ueCb->srsUlDedCfg.srsBw;
         ueCfgParam->srsUlCfg.dedSrsSetup.srsHopngBw     = ueCb->srsUlDedCfg.srsHoppingBw;
         ueCfgParam->srsUlCfg.dedSrsSetup.freqDmnPos     = ueCb->srsUlDedCfg.freqDomPos;
         ueCfgParam->srsUlCfg.dedSrsSetup.duration       = ueCb->srsUlDedCfg.duration;
         ueCfgParam->srsUlCfg.dedSrsSetup.srsCfgIdx      = ueCb->srsUlDedCfg.srsCfgIdx;
         ueCfgParam->srsUlCfg.dedSrsSetup.txComb         = ueCb->srsUlDedCfg.transComb;
         ueCfgParam->srsUlCfg.dedSrsSetup.cyclicShift    = ueCb->srsUlDedCfg.cyclicshift;
      }
      /* Dedicated SR Cfg */
      ueCfgParam->dedSRCfg.pres = TRUE;
      ueCfgParam->dedSRCfg.dedSRCfgType = CTF_IE_CFG_SETUP;
      ueCfgParam->dedSRCfg.dedSrSetup.srPUCCHRi = ueCb->srCfg.srPUCCHResIdx;

      ueCfgParam->dedSRCfg.dedSrSetup.srCfgIdx = ueCb->srCfg.srCfgIdx;
   }
   else
   {
      ueCfgParam->dedSRCfg.pres = FALSE;
   }

#endif /* TFU_UPGRADE */

   /* Antenna Cfg */
   ueCfgParam->antInfo.pres = TRUE;
   ueCfgParam->antInfo.txMode = ueCb->antennaInfo.txmode;
   ueCfgParam->antInfo.ueTxAntSelection.cfgType = CTF_IE_CFG_SETUP; 
   ueCfgParam->antInfo.ueTxAntSelection.txAntSelect = 
              ueCb->antennaInfo.ueTxAntSel;
   RETVALUE(ROK);
} /* wrUmmPhyFillUeCfg */

/**
 *  @brief This function build and send the UE release message
 *
 *      Function: wrUmmL1SndUeRel
 *
 *          Processing steps:
 *          - Allocate memory for ue release message 
 *          - update the structure
 *          - call the interface function to send the message
 *
 *
 *  @param [in]  transId : transid of the UE config message sent to Ctf
 *  @param [in]  crnti   : crnti of the UE to be released
 *  @param [in]  cellId  : cell ID of cell from which UE is released 
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmL1SndUeRel
(
U32                          transId,
U16                          crnti,
U16                          cellId
)
{
   CtfCfgReqInfo             *cfgInfo;

   WR_ALLOC(&cfgInfo, sizeof(CtfCfgReqInfo));
   if(cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   cfgInfo->cfgType = CTF_DELETE;
   cfgInfo->u.release.cfgElem = CTF_UE_CFG;
   cfgInfo->u.release.u.dedRel.cellId = cellId;
   cfgInfo->u.release.u.dedRel.ueId = crnti;

   if(ROK != wrIfmPhyUeCfg(cfgInfo, transId))
   {
      RLOG0(L_ERROR, "wrIfmPhyUeCfg failed.");
      RETVALUE(RFAILED);
   }
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
