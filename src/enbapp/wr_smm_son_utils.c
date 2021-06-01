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

     Name:     Eson Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_smm_son_utils.c

     Sid:      fc_smm_son_utils.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/9 - Tue Jan 13 12:01:15 2015

     Prg:      subhamay 

*********************************************************************21*/


/* header include files */ 
#include "wr.h"
#include "wr_cmn.h"
#include "wr_smm_init.h"
#include "wr_smm_smallcell.h"
#include "wr_utils.h"
#include "wr_lwr.h"
#include "lnl.h"
#include "nlu.h"
#ifdef WR_RSYS_OAM
#include "wr_msm_common.h"
#endif
#include "lnl.x"
#include "nlu.x"
#include "lys.x"
#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */

static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_FILE_ID=312;
static int RLOG_MODULE_ID=2;

#ifdef WR_RSYS_OAM
EXTERN MsmLteeNodeBparams lteeNodeBparams;
EXTERN Void smUpdateSONParams(U32 msgType, void* params);
#endif /* WR_RSYS_OAM*/

PRIVATE  S16 smBuildNlCellPucchCfg ARGS((
LnlPucchCfg   *pucchCfg
));

PRIVATE S16 smBuildNlCellPrachCfg   ARGS((
LnlPrachCfg   *prachCfg
));

PRIVATE S16 smBuildNlCellPciCfg   ARGS((
LnlCellCfg   *cellCfg
));

PRIVATE S16 smBuildNlCellEarfcnCfg  ARGS((
LnlCellCfg   *cellCfg
));

PRIVATE S16 smBuildNlCellCsgCfg  ARGS((
LnlCellCfg   *cellCfg
));

#ifdef WR_RSYS_OAM

/*
 *
 *       Fun:    smBuildNlCellPucchCfg
 *
 *       Desc:   Build Cell PUCCH Config params of a cell
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PRIVATE S16 smBuildNlCellPucchCfg
(
LnlPucchCfg   *pucchCfg
)
#else
PRIVATE S16 smBuildNlCellPucchCfg(pucchCfg)
LnlPucchCfg *pucchCfg;
#endif /* ANSI */
{
   TRC3(smBuildNlCellPucchCfg);

   pucchCfg->pucchCqiResIdx = lteeNodeBparams.ltePucchCfgCommon.nRBCqi;
   pucchCfg->n1PucchAn = lteeNodeBparams.ltePucchCfgCommon.n1PucchAn;
   pucchCfg->deltaShift = lteeNodeBparams.ltePucchCfgCommon.deltaShift + 1;
   pucchCfg->cyclicShift = lteeNodeBparams.ltePucchCfgCommon.nCSAN;

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smBuildNlCellPrachCfg
 *
 *       Desc:   Build Cell Prach Config params of a cell
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PRIVATE S16 smBuildNlCellPrachCfg
(
LnlPrachCfg   *prachCfg
)
#else
PRIVATE S16 smBuildNlCellPrachCfg(prachCfg)
LnlPrachCfg   *prachCfg;
#endif /* ANSI */
{
   TRC3(smBuildNlCellPrachCfg);

   prachCfg->servRootSeqIdx =
      lteeNodeBparams.ltePrachCfgCommon.rootSequenceIndx;
   prachCfg->servPrachCfgIdx =
      lteeNodeBparams.ltePrachCfgCommon.prachCfgInfo.prachCfgIdx;
   prachCfg->servZeroCorrelationZoneCfg =
      lteeNodeBparams.ltePrachCfgCommon.prachCfgInfo.zeroCorrelationZoneCfg;
   prachCfg->servPrachFreqOffset =
      lteeNodeBparams.ltePrachCfgCommon.prachCfgInfo.prachFreqOffset;
   prachCfg->highSpeedFlag = lteeNodeBparams.ltePrachCfgCommon.prachCfgInfo.highSpeedFlag;

   prachCfg->numRootSeqIndex =
      lteeNodeBparams.ltePrachCfgCommon.prachParamLst.numRootSeqIndex;
   if(prachCfg->numRootSeqIndex)
   {
      U8 rootSeqIdxCnt;
      for(rootSeqIdxCnt = 0; 
          rootSeqIdxCnt < prachCfg->numRootSeqIndex;
          rootSeqIdxCnt++)
      {
         prachCfg->rootSeqIdx[rootSeqIdxCnt] =
            lteeNodeBparams.ltePrachCfgCommon.prachParamLst.rootSequenceIndxLst[rootSeqIdxCnt];
      }
   }

   prachCfg->numPrachCfgIdx =
      lteeNodeBparams.ltePrachCfgCommon.prachParamLst.numPrachCfgIdx;
   if(prachCfg->numPrachCfgIdx)
   {
      U8 prachCfgIdxCnt;
      for(prachCfgIdxCnt = 0;
          prachCfgIdxCnt < prachCfg->numPrachCfgIdx;
          prachCfgIdxCnt ++)
      {
         prachCfg->prachConfigIdx[prachCfgIdxCnt] =
            lteeNodeBparams.ltePrachCfgCommon.prachParamLst.prachCfgIdxLst[prachCfgIdxCnt];
      }
   }

   prachCfg->numZeroCorrCfgZone =
      lteeNodeBparams.ltePrachCfgCommon.prachParamLst.numZeroCorrZoneCfg;
   if(prachCfg->numZeroCorrCfgZone)
   {
      U8 zeroCorrCfgZoneCnt;
      for(zeroCorrCfgZoneCnt = 0;
          zeroCorrCfgZoneCnt < prachCfg->numZeroCorrCfgZone;
          zeroCorrCfgZoneCnt ++)
      {
         prachCfg->zeroCorrelationZoneCfg[zeroCorrCfgZoneCnt] =
            lteeNodeBparams.ltePrachCfgCommon.prachParamLst.zeroCorrelationZoneCfgLst[zeroCorrCfgZoneCnt];
      }
   }

   prachCfg->numFreqOffset =
      lteeNodeBparams.ltePrachCfgCommon.prachParamLst.numPrachFreqOffset;
   if(prachCfg->numFreqOffset)
   {
      U8 freqOffsetCnt;
      for(freqOffsetCnt = 0;
          freqOffsetCnt < prachCfg->numFreqOffset;
          freqOffsetCnt ++)
      {
         prachCfg->freqOffset[freqOffsetCnt] =
            lteeNodeBparams.ltePrachCfgCommon.prachParamLst.prachFreqOffsetLst[freqOffsetCnt];
      }
   }

   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smBuildNlCellReCfg
 *
 *       Desc:   fill all the cell reconfiguration parameter received from OAM
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildNlCellReCfg
(
LnlCellReCfg   *cellReCfg
)
#else
PUBLIC S16 smBuildNlCellReCfg(cellReCfg)
LnlCellReCfg   *cellReCfg;
#endif /* ANSI */
{
   U32                   idx;
   MsmLteCellSib1CfgGrp *sib1cfg = NULLP;
   MsmLtePlmnId         *priPlmn = NULLP;
   TRC3(smBuildNlCellReCfg);
   cellReCfg->cellId = 1;
   sib1cfg  = &(lteeNodeBparams.lteCellSib1CfgGrp);
   cellReCfg->ecgi.cellId = sib1cfg->cellAccessInfo.cellId;

   for(idx = 0; idx < sib1cfg->cellAccessInfo.numOfPlmns; idx++)
   {
      if(lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.plmnInfo[idx].isPrimary == TRUE)
      {
         priPlmn = &sib1cfg->cellAccessInfo.plmnInfo[idx].plmnId;
         cellReCfg->ecgi.plmn.numMncDigits  = priPlmn->numMncDigits;
         cellReCfg->ecgi.plmn.mcc[0] = priPlmn->mcc[0];
         cellReCfg->ecgi.plmn.mcc[1] = priPlmn->mcc[1];
         cellReCfg->ecgi.plmn.mcc[2] = priPlmn->mcc[2];

         cellReCfg->ecgi.plmn.mnc[0]= priPlmn->mnc[0];
         cellReCfg->ecgi.plmn.mnc[1]= priPlmn->mnc[1];
         cellReCfg->ecgi.plmn.mnc[2]= priPlmn->mnc[2];
         break;
      }
   }
   cellReCfg->numPci = lteeNodeBparams.lteAddCellCfg.pciListCfg.numPci;
   for(idx = 0; idx < cellReCfg->numPci; idx++)
   {
      cellReCfg->pciList[idx] = 
         lteeNodeBparams.lteAddCellCfg.pciListCfg.pciLst[idx]; 
   }
   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smBuildNlCellRsysTpmCfg
 *
 *       Desc:   Fill the Rsys TPM configuration from OAM
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PRIVATE S16 smBuildNlCellRsysTpmCfg
(
LnlCellCfg   *cellCfg
)
#else
PRIVATE S16 smBuildNlCellRsysTpmCfg(cellCfg)
LnlCellCfg   *cellCfg;
#endif /* ANSI */
{
   TRC3(smBuildNlCellRsysTpmCfg);

   cellCfg->tpmCfg.tgtRsrp = lteeNodeBparams.tpmCfg.tgtRsrp;
   cellCfg->tpmCfg.attnConstIdx = lteeNodeBparams.tpmCfg.attnConstIdx;
   cellCfg->tpmCfg.minRsrp = lteeNodeBparams.tpmCfg.minRsrp;
   cellCfg->tpmCfg.maxRsrp = lteeNodeBparams.tpmCfg.maxRsrp;
   
   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smBuildNlCellTxPwrCfg
 *
 *       Desc:   Fill the Tx Power configuration parameter received from OAM
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PRIVATE S16 smBuildNlCellTxPwrCfg
(
LnlCellCfg   *cellCfg
)
#else
PRIVATE S16 smBuildNlCellTxPwrCfg(cellCfg)
LnlCellCfg   *cellCfg;
#endif /* ANSI */
{
   U32 refSigIdx, pbIdx, paIdx;
   U32 pschIdx, sschIdx, pbchIdx;

   TRC3(smBuildNlCellTxPwrCfg);

   cellCfg->txPwrCfg.refSignalPwrCnt =
      lteeNodeBparams.ltePdschCfgCommon.refSignalPwrCnt;
   for(refSigIdx=0; refSigIdx< cellCfg->txPwrCfg.refSignalPwrCnt; refSigIdx++)
   {
      cellCfg->txPwrCfg.referenceSignalPwr[refSigIdx] =
         lteeNodeBparams.ltePdschCfgCommon.referenceSignalPwr[refSigIdx];
   }

   cellCfg->txPwrCfg.pBCnt = lteeNodeBparams.ltePdschCfgCommon.pBCnt;
   for(pbIdx=0; pbIdx< cellCfg->txPwrCfg.pBCnt; pbIdx++)
   {
      cellCfg->txPwrCfg.pB[pbIdx] =
         lteeNodeBparams.ltePdschCfgCommon.pB[pbIdx];
   }

   cellCfg->txPwrCfg.pACnt = lteeNodeBparams.ltePdschCfgDedicated.pACnt;
   for(paIdx=0; paIdx< cellCfg->txPwrCfg.pACnt; paIdx++)
   {
      cellCfg->txPwrCfg.pA[paIdx] = lteeNodeBparams.ltePdschCfgDedicated.pA[paIdx];
   }

   cellCfg->txPwrCfg.pschPowerOffCnt = lteeNodeBparams.ltePowerOffsetParams.pschPowerOffCnt;
   for(pschIdx=0;pschIdx<cellCfg->txPwrCfg.pschPowerOffCnt;pschIdx++)
   {
      cellCfg->txPwrCfg.pschPowerOffset[pschIdx] = 
         lteeNodeBparams.ltePowerOffsetParams.pschPowerOffset[pschIdx];
   }

   cellCfg->txPwrCfg.sschPowerOffCnt = lteeNodeBparams.ltePowerOffsetParams.sschPowerOffCnt;
   for(sschIdx=0;sschIdx<cellCfg->txPwrCfg.sschPowerOffCnt;sschIdx++)
   {
      cellCfg->txPwrCfg.sschPowerOffset[sschIdx] = 
         lteeNodeBparams.ltePowerOffsetParams.sschPowerOffset[sschIdx];
   }

   cellCfg->txPwrCfg.pbchPowerOffCnt = lteeNodeBparams.ltePowerOffsetParams.pbchPowerOffCnt;
   for(pbchIdx=0;pbchIdx<cellCfg->txPwrCfg.pbchPowerOffCnt;pbchIdx++)
   {
      cellCfg->txPwrCfg.pbchPowerOffset[pbchIdx] = 
         lteeNodeBparams.ltePowerOffsetParams.pbchPowerOffset[pbchIdx];
   }

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smBuildNlCellCsgCfg
 *
 *       Desc:   fill all the cell Csg configuration parameter received from OAM
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PRIVATE S16 smBuildNlCellCsgCfg
(
LnlCellCfg   *cellCfg
)
#else
PRIVATE S16 smBuildNlCellCsgCfg(cellCfg)
LnlCellCfg   *cellCfg;
#endif /* ANSI */
{
   U16        csgPciRange = 0;
   TRC3(smBuildNlCellCsgCfg);
   cellCfg->csgPciCfg.isCsgPciCfg = lteeNodeBparams.lteSib4CsgInfo.isCsgPciCfgd;
   cellCfg->csgPciCfg.csgStartPci = lteeNodeBparams.lteSib4CsgInfo.csgStartPhyCellId;
   WR_EMM_GET_PCI_RANGE(lteeNodeBparams.lteSib4CsgInfo.csgPciRange,
            csgPciRange);
   cellCfg->csgPciCfg.csgPciRange = csgPciRange;

   cellCfg->smallCellPciCfg.isSmallCellPciCfg = 
          lteeNodeBparams.lteAddCellCfg.isSmallCellPciCfgd;
   cellCfg->smallCellPciCfg.smallCellStartPci = lteeNodeBparams.lteAddCellCfg.startPci;
   if(TRUE == cellCfg->smallCellPciCfg.isSmallCellPciCfg)
   {
      cellCfg->smallCellPciCfg.maxSmallCellPci = 
       cellCfg->smallCellPciCfg.smallCellStartPci + lteeNodeBparams.lteAddCellCfg.pciRange;   
   }
   switch(lteeNodeBparams.lteFapAccessParams.accessMode)
   {
      case WR_ENB_OPEN_ACCESS :
      {
         cellCfg->csgPciCfg.accessMode = LNL_ENB_OPEN_ACCESS;
      }
      break;
      case WR_ENB_CLOSED_ACCESS :
      {
         cellCfg->csgPciCfg.accessMode = LNL_ENB_CLOSED_ACCESS;
      }
      break;
      case WR_ENB_HYBRID_ACCESS :
      {
         cellCfg->csgPciCfg.accessMode = LNL_ENB_HYBRID_ACCESS;
      }
      default :
      {
         break;
      }
   }         
   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smBuildNlCellEarfcnCfg
 *
 *       Desc:   fill all the cell configuration parameter received from OAM
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PRIVATE S16 smBuildNlCellEarfcnCfg
(
LnlCellCfg   *cellCfg
)
#else
PRIVATE S16 smBuildNlCellEarfcnCfg(cellCfg)
LnlCellCfg   *cellCfg;
#endif /* ANSI */
{
   U32                  earfcnCount;

   TRC3(smBuildNlCellEarfcnCfg);

   cellCfg->numDlEarfcn = lteeNodeBparams.lteAddCellCfg.earfcnListCfg.numEarfcnDl;
   cellCfg->numUlEarfcn = lteeNodeBparams.lteAddCellCfg.earfcnListCfg.numEarfcnUl;
   for(earfcnCount = 0; earfcnCount < cellCfg->numDlEarfcn; earfcnCount ++)
   {
      cellCfg->dlEarfcnList[earfcnCount] =
          lteeNodeBparams.lteAddCellCfg.earfcnListCfg.earfcnDlLst[earfcnCount];
   }
   for(earfcnCount = 0; earfcnCount < cellCfg->numUlEarfcn; earfcnCount ++)
   {
      cellCfg->ulEarfcnList[earfcnCount] =
          lteeNodeBparams.lteAddCellCfg.earfcnListCfg.earfcnUlLst[earfcnCount];
   }
   cellCfg->numRemScanEarfcn = lteeNodeBparams.lteEarfcnRssiCfg.numEarfcn;
   for(earfcnCount = 0; earfcnCount < cellCfg->numRemScanEarfcn; earfcnCount ++)
   {
      cellCfg->remScanEarfcn[earfcnCount].dlEarfcn =
      lteeNodeBparams.lteEarfcnRssiCfg.remScanEarfcn[earfcnCount].earfcnDl;
      cellCfg->remScanEarfcn[earfcnCount].rssi =
      lteeNodeBparams.lteEarfcnRssiCfg.remScanEarfcn[earfcnCount].rssi;
   }
   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smBuildNlCellPciCfg
 *
 *       Desc:   fill all the cell configuration parameter received from OAM
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PRIVATE S16 smBuildNlCellPciCfg
(
LnlCellCfg   *cellCfg
)
#else
PRIVATE S16 smBuildNlCellPciCfg(cellCfg)
LnlCellCfg   *cellCfg;
#endif /* ANSI */
{
   U32            pciCount;

   TRC3(smBuildNlCellPciCfg);

   cellCfg->pci = lteeNodeBparams.lteAddCellCfg.physCellId;
   /*FILL the PCI LIST*/
   cellCfg->numPci = lteeNodeBparams.lteAddCellCfg.pciListCfg.numPci;
   for(pciCount = 0; pciCount < cellCfg->numPci; pciCount ++)
   {
      cellCfg->pciList[pciCount] =
       lteeNodeBparams.lteAddCellCfg.pciListCfg.pciLst[pciCount];
   }

   RETVALUE(ROK);
}
/*
 *
 *       Fun:    smBuildNlCellCfg
 *
 *       Desc:   fill all the cell configuration parameter received from OAM
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildNlCellCfg
(
LnlCellCfg   *cellCfg
)
#else
PUBLIC S16 smBuildNlCellCfg(cellCfg)
LnlCellCfg   *cellCfg;
#endif /* ANSI */
{
   
   U32                  idx;
   MsmLteCellSib1CfgGrp *sib1cfg = NULLP;
   MsmLtePlmnId         *priPlmn = NULLP;
   TRC3(smBuildNlCellCfg);
   cellCfg->cellId = 1;
   sib1cfg  =&(lteeNodeBparams.lteCellSib1CfgGrp);
   cellCfg->ecgi.cellId = sib1cfg->cellAccessInfo.cellId;
   for(idx =0; idx < sib1cfg->cellAccessInfo.numOfPlmns; idx++)
   {
      if(lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.plmnInfo[idx].isPrimary == TRUE)
      {
         priPlmn = &sib1cfg->cellAccessInfo.plmnInfo[idx].plmnId;
         cellCfg->ecgi.plmn.numMncDigits  = priPlmn->numMncDigits;
         cellCfg->ecgi.plmn.mcc[0] = priPlmn->mcc[0];
         cellCfg->ecgi.plmn.mcc[1] = priPlmn->mcc[1];
         cellCfg->ecgi.plmn.mcc[2] = priPlmn->mcc[2];

         cellCfg->ecgi.plmn.mnc[0]= priPlmn->mnc[0];
         cellCfg->ecgi.plmn.mnc[1]= priPlmn->mnc[1];
         cellCfg->ecgi.plmn.mnc[2]= priPlmn->mnc[2];
         break;
      }
   }
  
   cellCfg->bandWidth = lteeNodeBparams.lteULFrequencyInfo.ulBandwidth;
   /*lteeNodeBparams.lteCellMibCfgGrp.dlBw*/
   cellCfg->duplexMode = lteeNodeBparams.lteAddCellCfg.duplexMode;
   
   smBuildNlCellCsgCfg(cellCfg);
   smBuildNlCellEarfcnCfg(cellCfg);
   smBuildNlCellPciCfg(cellCfg);

#ifdef LTE_TDD
   cellCfg->subFrameCfg = lteeNodeBparams.lteTddParam.subframeAssignment;
#endif

   smBuildNlCellPrachCfg(&(cellCfg->prachCfgInfo));
   smBuildNlCellPucchCfg(&(cellCfg->pucchResCfg));
   /* PHICH Configuration */
   cellCfg->phichCfg.ngEnum = wrSmDfltPhichRes;
   cellCfg->phichCfg.isDurExtend = wrSmDfltPhichDur;

   smBuildNlCellTxPwrCfg(cellCfg);
   smBuildNlCellRsysTpmCfg(cellCfg);

   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smBuildNlGenCfg
 *
 *       Desc:   fill all the general configuration parameter received from OAM
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildNlGenCfg
(
NlGenCfg   *genCfg
)
#else
PUBLIC S16 smBuildNlGenCfg(genCfg)
NlGenCfg   *genCfg;
#endif /* ANSI */
{
   TRC3(smBuildNlGenCfg);
   genCfg->tmrRes = 10;
   genCfg->pciSelType = lteeNodeBparams.lteSonGenericParams.isPciSelEnabled;
   genCfg->prachSelType = lteeNodeBparams.lteSonGenericParams.isPrachSelEnabled;
   genCfg->earfcnSelType = lteeNodeBparams.lteSonGenericParams.isEarfcnSelEnabled;
   genCfg->txPowerSelType = lteeNodeBparams.lteSonGenericParams.isTxPowerSelEnabled;
   genCfg->maxUeSupp = lteeNodeBparams.lteSmCellCfg.maxUeSupp;
   genCfg->maxUePerTti = lteeNodeBparams.lteCellDlSchdCfgGrp.maxUePerDlSf;
   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smBuildNlNghCellCfg
 *
 *       Desc:   fill all the Neighbor cell configuration parameter received from OAM
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smBuildNlNghCellCfg
(
LnlNghCellCfg   *nghCellCfg
)
#else
PUBLIC S16 smBuildNlNghCellCfg(nghCellCfg)
LnlNghCellCfg   *nghCellCfg;
#endif /* ANSI */
{
   LwrEUTRANeighCell   *nghCell      = NULLP;
   LwrEUTRANeighCell   *prevNghCell  = NULLP;
   SmNeigEutraCellList *nghCellList  = NULLP;
   SmNeigEutraCellList *prevCellList  = NULLP;
   LnlNghCellAdd       *lnlNghCell   = NULLP;
   PTR                 prevCellEntry = NULLP;
   U32                 numEnbs       = 0;
   U32                 numCells      = 0;
   U32                 count;
   TRC3(smBuildNlNghCellCfg);
   nghCellCfg->servCellId = 1; 
/*     lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.cellId;*/


      while(ROK == cmHashListGetNext(&(smCfgCb.neighEutraCellLstCp), 
                  prevCellEntry, (PTR *) &nghCellList)) 
      {
         nghCell  = nghCellList->sonNghCellCfg.nghCellCfg;
         if(NULLP != nghCell)
         {
           if(prevCellEntry)
           {
              prevCellList = (SmNeigEutraCellList *)prevCellEntry;
              prevNghCell = prevCellList->sonNghCellCfg.nghCellCfg;
              if (prevNghCell != NULLP)
              {
                 if(prevNghCell->enbId != nghCell->enbId)  /* ccpu00146861 */
                 {
                    numEnbs++;
                 }
              }
           }
           else
           {
              RLOG0(L_WARNING, "prevCellEntry is NULL");
           }

           numCells = 0;
           nghCellCfg->nghEnbCellCfg[numEnbs].nghEnbId = nghCell->enbId; 
           nghCellCfg->nghEnbCellCfg[numEnbs].nghCellParam.cfgType = LNL_NGH_ADD;
           RLOG3(L_INFO,"SM->SON: Add cell Config -- PCI %d, DL EARFCN %d, CellId %d\n", 
           nghCell->pci, nghCell->earfcnDl,
           nghCell->enbId);
           RLOG3(L_INFO,"SM->SON: Add cell Config -- MCC[0] %d, MCC[1] %d, MCC[2] %d\n", 
                  nghCell->enbPlmn.mcc[0], nghCell->enbPlmn.mcc[1],
                  nghCell->enbPlmn.mcc[2]);
           RLOG2(L_INFO,"SM->SON: Add cell Config -- MNC[0] %d, MNC[1] %d\n", 
                  nghCell->enbPlmn.mnc[0], nghCell->enbPlmn.mnc[1]);
           lnlNghCell = &(nghCellCfg->nghEnbCellCfg[numEnbs].
                          nghCellParam.u.nghCellAdd);
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.pci = 
                          nghCell->pci;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.dlEarfcn = 
                         nghCell->earfcnDl;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.ulEarfcn = 
                         nghCell->earfcnUl;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.ecgi.cellId =
                         nghCell->enbId;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.ecgi.
                 plmn.numMncDigits = 2 /*nghCell->enbPlmn.numMncDigits*/; 
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.ecgi.
                 plmn.mcc[0] = nghCell->enbPlmn.mcc[0]; 
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.ecgi.
                 plmn.mcc[1] = nghCell->enbPlmn.mcc[1]; 
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.ecgi.
                 plmn.mcc[2] = nghCell->enbPlmn.mcc[2]; 
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.ecgi.
                 plmn.mnc[0] = nghCell->enbPlmn.mnc[0]; 
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.ecgi.
                 plmn.mnc[1] = nghCell->enbPlmn.mnc[1]; 
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.ecgi.
                 plmn.mnc[2] = /*nghCell->enbPlmn.mnc[2]*/0;

            /* Tx Power Configuration */
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.lteBand =
               nghCellList->sonNghCellCfg.lteBand;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.tac =
               nghCell->tac;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.ulBw =
               nghCell->ulBw;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.dlBw =
               nghCell->dlBw;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.RSTxPower =
               nghCell->refSigPower;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.csgAccessMode =
               nghCell->csgAccessMode;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.csgId =
               nghCell->csgId;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.rsrp =
               nghCell->rsrp;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.rssi =
               nghCell->rssi;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.isCellBarred =
               nghCellList->sonNghCellCfg.isCellBarred;

            /*PRACH CONFIG */
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.isNghPrachInfo =
               nghCellList->sonNghCellCfg.isPrachConfig;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.prachCfgInfo.nghRootSeqIdx
               = nghCellList->sonNghCellCfg.rootSeqIndex;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.prachCfgInfo.nghPrachCfgIdx
               = nghCellList->sonNghCellCfg.prachCfgIdx;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.prachCfgInfo.nghZeroCorrelationZoneCfg
               = nghCellList->sonNghCellCfg.zeroCorrelationZoneCfg;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.prachCfgInfo.nghPrachFreqOffset
               = nghCellList->sonNghCellCfg.prachFreqOffset;
           lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.prachCfgInfo.highSpeedFlag
               = nghCellList->sonNghCellCfg.highSpeedFlag;


           lnlNghCell->nghCellAddInfo[numCells].numNghNgh = 
                 nghCellList->sonNghCellCfg.numNghNgh;
           for(count = 0; count < nghCellList->sonNghCellCfg.numNghNgh; count ++)
           {
               lnlNghCell->nghCellAddInfo[numCells].nghNghCellInfo[count].pci = 
                  nghCellList->sonNghCellCfg.nghNghcellPci[count];
               /*fill the DL and UL earfcn of neighbor's neighbor cell same as neighbor cell*/
               lnlNghCell->nghCellAddInfo[numCells].nghNghCellInfo[count].
                      dlEarfcn = nghCell->earfcnDl;
               lnlNghCell->nghCellAddInfo[numCells].nghNghCellInfo[count].
                      ulEarfcn = nghCell->earfcnUl;
           }

           RLOG3(L_INFO,"SM->SON Neighbor Config [Numcell %u]: enbid = %lu, pci = %u\n",
	               numCells + 1,
                  lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.ecgi.cellId,
                  lnlNghCell->nghCellAddInfo[numCells].nghCellInfo.pci);

           numCells++;
           lnlNghCell->numNgh = numCells; 
         }
         else
         {
            RLOG0(L_INFO,"No nghCell entry\n");
         }
         prevCellEntry = (PTR)nghCellList;
      }


      /* Removed the "+ 1" increament, as the numEnbs will be initialized to One
       * when the prevNghCellEntry is NULL, i.e for the first neighbor Cell */
      if(numCells)
      {
   nghCellCfg->numNghEnb = numEnbs + 1;
      }
      else
      {
         nghCellCfg->numNghEnb = 0;
      }
   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smUpdateNlPciVal
 *
 *       Desc:   update the PCI value modified by SON
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smUpdateNlPciVal
(
U16   pci
)
#else
PUBLIC S16 smUpdateNlPciVal(pci)
U16   pci;
#endif /* ANSI */
{
   TRC3(smUpdateNlPciVal);

   lteeNodeBparams.lteAddCellCfg.physCellId = pci;

   smUpdateSONParams(MSM_LTE_UPDATE_SON_SELECTED_PCI, 
                  (void *)&(lteeNodeBparams.lteAddCellCfg.physCellId));
   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smUpdateNlEarfcnVal
 *
 *       Desc:   update the EARFCN val selected by SON 
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smUpdateNlEarfcnVal
(
NlUpdateEarfcnCfgInd   *earfcnCfgInd
)
#else
PUBLIC S16 smUpdateNlEarfcnVal(earfcnCfgInd)
NlUpdateEarfcnCfgInd   *earfcnCfgInd;
#endif /* ANSI */
{
   TRC3(smUpdateNlEarfcnVal);

   U16   servCellDlEarfcn;
   SmNeigEutraFreqList *nghEutraFreqNewNode = NULLP;
   SmNeigEutraFreqList *nghEutraFreqNode = NULLP;
   SmNeigEutraFreqList *nghEutraFreqOldNode = NULLP;
   lteeNodeBparams.lteAddCellCfg.earfdnDl = earfcnCfgInd->dlEarfcn;
   lteeNodeBparams.lteAddCellCfg.earfdnUl = earfcnCfgInd->ulEarfcn;
   lteeNodeBparams.lteULFrequencyInfo.eARFCN = earfcnCfgInd->ulEarfcn;

   /*we should always make neighbor frequency hash table serving frequency with selected EARFCN*/
   servCellDlEarfcn = lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[0].eutranFreq.earfcnDl;
   /*check whether EARFCN got modified by SON*/
   if(servCellDlEarfcn != earfcnCfgInd->dlEarfcn)
   {
      /*Find the Hash Entry with existing DlEarfcn*/
      if(ROK != cmHashListFind(&(smCfgCb.neighEutraFreqLstCp),
               (U8 *)&servCellDlEarfcn, sizeof(servCellDlEarfcn), 0, (PTR *)&nghEutraFreqOldNode))
      {
         RLOG1(L_ERROR, "Failed To find Neighbor Earfcn Node with Earfcn %d \n", servCellDlEarfcn);
         RETVALUE(RFAILED);
      }
      /*check whether any neighbor already added with SON selected EARFCN*/
      if(ROK != cmHashListFind(&(smCfgCb.neighEutraFreqLstCp),
               (U8 *)&earfcnCfgInd->dlEarfcn, sizeof(earfcnCfgInd->dlEarfcn), 0, (PTR *)&nghEutraFreqNode))
      {
         WR_ALLOC(&nghEutraFreqNewNode,sizeof(SmNeigEutraFreqList));
         if(NULLP == nghEutraFreqNewNode)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         cmMemcpy((U8 *)nghEutraFreqNewNode, (U8 *) nghEutraFreqOldNode, sizeof(SmNeigEutraFreqList));
         /*Update the DL and UL Earfcn selected by SON*/
         nghEutraFreqNewNode->nghEutraFreqCfg.earfcnDl = earfcnCfgInd->dlEarfcn;
         nghEutraFreqNewNode->nghEutraFreqCfg.earfcnUl = earfcnCfgInd->ulEarfcn;
         /* update msm */
         lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[0].eutranFreq.earfcnDl = earfcnCfgInd->dlEarfcn;
         lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[0].eutranFreq.earfcnUl = earfcnCfgInd->ulEarfcn;

         if(cmHashListInsert(&(smCfgCb.neighEutraFreqLstCp),
               (PTR)nghEutraFreqNewNode, (U8 *)&nghEutraFreqNewNode->nghEutraFreqCfg.earfcnDl,
                   sizeof(nghEutraFreqNewNode->nghEutraFreqCfg.earfcnDl)) != ROK)
         {
            RLOG1(L_ERROR, "Earfcn Updation Failed in Neighbor Hash table with Earfcn = %d",
               nghEutraFreqNewNode->nghEutraFreqCfg.earfcnDl);
            WR_FREE(nghEutraFreqNewNode, sizeof(SmNeigEutraFreqList));
            RETVALUE(RFAILED);
         }
      }
      cmHashListDelete(&(smCfgCb.neighEutraFreqLstCp), (PTR)nghEutraFreqOldNode);
   }

   smUpdateSONParams(MSM_LTE_UPDATE_SON_SELECTED_EARFCNUL, 
         (void *)&(lteeNodeBparams.lteAddCellCfg.earfdnUl));

   smUpdateSONParams(MSM_LTE_UPDATE_SON_SELECTED_EARFCNDL, 
         (void *)&(lteeNodeBparams.lteAddCellCfg.earfdnDl));

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smUpdateNlPrachVal
 *
 *       Desc:   update the PRACH val selected by SON 
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smUpdateNlPrachVal
(
NlUpdatePrachCfgInd   *prachCfgInd
)
#else
PUBLIC S16 smUpdateNlPrachVal(prachCfgInd)
NlUpdatePrachCfgInd   *prachCfgInd;
#endif /* ANSI */
{
   //S32   count;
   //S32   idx;
   TRC3(smUpdateNlPrachVal);

   lteeNodeBparams.ltePrachCfgCommon.rootSequenceIndx =
      prachCfgInd->rootSequenceIndx;
   lteeNodeBparams.ltePrachCfgCommon.prachCfgInfo.prachCfgIdx =
      prachCfgInd->prachCfgIdx;
   lteeNodeBparams.ltePrachCfgCommon.prachCfgInfo.highSpeedFlag =
      prachCfgInd->highSpeedFlag;
   lteeNodeBparams.ltePrachCfgCommon.prachCfgInfo.zeroCorrelationZoneCfg =
      prachCfgInd->zeroCorrelationZoneCfg;
   lteeNodeBparams.ltePrachCfgCommon.prachCfgInfo.prachFreqOffset =
      prachCfgInd->prachFreqOffset;


   /* Inform OAM */
   smUpdateSONParams(MSM_LTE_UPDATE_PRACH_CONFIG, 
                    (void *)&(lteeNodeBparams.ltePrachCfgCommon));

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smUpdateNlPowerOffsetParams
 *
 *       Desc:   update the Power Offset parameter values modified by SON for
 *       TxPower.
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PRIVATE S16 smUpdateNlPowerOffsetParams
(
NlUpdateTxPwrCfgInd  *txPwrCfgInd
)
#else
PRIVATE S16 smUpdateNlPowerOffsetParams(txPwrCfgInd)
NlUpdateTxPwrCfgInd  *txPwrCfgInd;
#endif /* ANSI */
{
   U32    idx;
   U32    pssCnt;
   U32    sssCnt;
   U32    pbchCnt;

   TRC3(smUpdateNlPowerOffsetParams);

   for(pssCnt = lteeNodeBparams.ltePowerOffsetParams.pschPowerOffCnt -1; 
       pssCnt > 0; 
       pssCnt --)
   {
      if(lteeNodeBparams.ltePowerOffsetParams.pschPowerOffset[pssCnt] ==
            txPwrCfgInd->pssPowerOffset)
      {
         for(idx = pssCnt; idx > 0; idx --)
         {
            lteeNodeBparams.ltePowerOffsetParams.pschPowerOffset[idx] = 
               lteeNodeBparams.ltePowerOffsetParams.pschPowerOffset[idx -1]; 
         }
         break;
      }
   }
   lteeNodeBparams.ltePowerOffsetParams.pschPowerOffset[0] =
      txPwrCfgInd->pssPowerOffset;
   for(sssCnt = lteeNodeBparams.ltePowerOffsetParams.sschPowerOffCnt -1; 
       sssCnt > 0; 
       sssCnt --)
   {
      if(lteeNodeBparams.ltePowerOffsetParams.sschPowerOffset[sssCnt] ==
            txPwrCfgInd->sssPowerOffset)
      {
         for(idx = sssCnt; idx > 0; idx --)
         {
            lteeNodeBparams.ltePowerOffsetParams.sschPowerOffset[idx] = 
               lteeNodeBparams.ltePowerOffsetParams.sschPowerOffset[idx -1]; 
         }
         break;
      }
   }
   lteeNodeBparams.ltePowerOffsetParams.sschPowerOffset[0] =
      txPwrCfgInd->sssPowerOffset;

   for(pbchCnt = lteeNodeBparams.ltePowerOffsetParams.pbchPowerOffCnt -1; 
       pbchCnt > 0; 
       pbchCnt --)
   {
      if(lteeNodeBparams.ltePowerOffsetParams.pbchPowerOffset[pbchCnt] ==
            txPwrCfgInd->pbchPowerOffset)
      {
         for(idx = pbchCnt; idx > 0; idx --)
         {
            lteeNodeBparams.ltePowerOffsetParams.pbchPowerOffset[idx] = 
               lteeNodeBparams.ltePowerOffsetParams.pbchPowerOffset[idx -1]; 
         }
         break;
      }
   }
   lteeNodeBparams.ltePowerOffsetParams.pbchPowerOffset[0] =
      txPwrCfgInd->pbchPowerOffset;

   smUpdateSONParams(MSM_LTE_POWER_OFFSET_PARAMETERS, 
                  (void *)&(lteeNodeBparams.ltePowerOffsetParams));

   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smUpdateNlPdschCfgCommonParams
 *
 *       Desc:   update the PdschConfigCommon value modified by SON for
 *       TxPower.
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PRIVATE S16 smUpdateNlPdschCfgCommonParams
(
NlUpdateTxPwrCfgInd  *txPwrCfgInd
)
#else
PRIVATE S16 smUpdateNlPdschCfgCommonParams(txPwrCfgInd)
NlUpdateTxPwrCfgInd  *txPwrCfgInd;
#endif /* ANSI */
{
   U32    idx;
   U32    refSigTxPwrCount;

   TRC3(smUpdateNlPdschCfgCommonParams);

   for(refSigTxPwrCount = lteeNodeBparams.ltePdschCfgCommon.refSignalPwrCnt -1; 
       refSigTxPwrCount > 0; 
       refSigTxPwrCount --)
   {
      if(lteeNodeBparams.ltePdschCfgCommon.referenceSignalPwr[refSigTxPwrCount] ==
            txPwrCfgInd->refSignalTxPwr)
      {
         for(idx = refSigTxPwrCount; idx > 0; idx --)
         {
            lteeNodeBparams.ltePdschCfgCommon.referenceSignalPwr[idx] = 
               lteeNodeBparams.ltePdschCfgCommon.referenceSignalPwr[idx -1]; 
         }
         break;
      }
   }
   lteeNodeBparams.ltePdschCfgCommon.referenceSignalPwr[0] = txPwrCfgInd->refSignalTxPwr;

   for(refSigTxPwrCount = lteeNodeBparams.ltePdschCfgCommon.pBCnt -1; 
       refSigTxPwrCount > 0; 
       refSigTxPwrCount --)
   {
      if(lteeNodeBparams.ltePdschCfgCommon.pB[refSigTxPwrCount] ==
            txPwrCfgInd->refSignalTxPwr)
      {
         for(idx = refSigTxPwrCount; idx > 0; idx --)
         {
            lteeNodeBparams.ltePdschCfgCommon.pB[idx] = 
               lteeNodeBparams.ltePdschCfgCommon.pB[idx -1]; 
         }
         break;
      }
   }
   lteeNodeBparams.ltePdschCfgCommon.pB[0] = txPwrCfgInd->pB;
 
   smUpdateSONParams(MSM_LTE_PDSCH_CONFIG_COMMON_PARAMETERS, 
                  (void *)&(lteeNodeBparams.ltePdschCfgCommon));
   RETVALUE(ROK);
}

#if 1
/*
 *
 *       Fun:    smUpdateSib2Params
 *
 *       Desc:   update the PCI value modified by SON
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PRIVATE S16 smUpdateSib2Params
(
NlUpdateTxPwrCfgInd  *txPwrCfgInd
)
#else
PRIVATE S16 smUpdateSib2Params(txPwrCfgInd)
NlUpdateTxPwrCfgInd  *txPwrCfgInd;
#endif /* ANSI */
{
   TRC3(smUpdateSib2Params);
   MsmLtePdschCfgCommon modPdschCfg;

   /* Trigger SIB2 change to UE for PDSCH Params */
   if(lteeNodeBparams.ltePdschCfgCommon.referenceSignalPwr[0] !=
         txPwrCfgInd->refSignalTxPwr)
   {
      modPdschCfg.actType = MSM_ACT_MOD;
      modPdschCfg.refSignalPwrCnt = 1;
      modPdschCfg.referenceSignalPwr[0] = txPwrCfgInd->refSignalTxPwr;
      modPdschCfg.pBCnt = 1;
      modPdschCfg.pB[0] = txPwrCfgInd->pB;

      smDynCfgReq((Void *)&modPdschCfg,MSM_LTE_PDSCH_CONFIG_COMMON_PARAMETERS,0);

      /* Note: This has been added till OAM supports dynamic configuration of
       * MSM_LTE_PDSCH_CONFIG_COMMON_PARAMETERS once SON updates the same to
       * OAM. As of now, we see that once SON updates, dynamic configuration
       * is not being triigered from OAM. This will be removed once OAM
       * provides the required functionality */
      smCfgCb.sonTriggeredDynCfg = TRUE;
   }

   RETVALUE(ROK);
}
#endif


/*
 *
 *       Fun:    smUpdateNlTxPwrVal
 *
 *       Desc:   update the PCI value modified by SON
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smUpdateNlTxPwrVal
(
NlUpdateTxPwrCfgInd  *txPwrCfgInd
)
#else
PUBLIC S16 smUpdateNlTxPwrVal(txPwrCfgInd)
NlUpdateTxPwrCfgInd  *txPwrCfgInd;
#endif /* ANSI */
{
   TRC3(smUpdateNlTxPwrVal);

   /* Only if Cell is up (PHY is up), send Tx Power change Req to CL */
   if(smCb.smState == WR_SM_STATE_CELL_UP 
#ifdef WR_RSYS_OAM
     || smCb.smState ==  WR_SM_STATE_OAM_DYN_START
#endif
     )
   {
      S16 refSigPwr = (-(txPwrCfgInd->refSignalTxPwr) * 4);
      SmLysTPMTxPwrChangeReq(refSigPwr);         
#if 1
      smUpdateSib2Params(txPwrCfgInd); 
#endif
   }

   smUpdateNlPdschCfgCommonParams(txPwrCfgInd);
   smUpdateNlPowerOffsetParams(txPwrCfgInd);


   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smUpdateNlMeasRptInterval
 *
 *       Desc:   update the measurement interval value configured by BRDCM-SON
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smUpdateNlMeasRptInterval
(
U32   measInterval
)
#else
PUBLIC S16 smUpdateNlMeasRptInterval(measInterval)
U32   measInterval;
#endif /* ANSI */
{
   TRC3(smUpdateNlMeasRptInterval);
   lteeNodeBparams.lteCellMeasCfgGrp.
     measEutraPeridRptCfg[MSM_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS].sonCfgRptInterval = 
      measInterval;
   lteeNodeBparams.lteCellMeasCfgGrp.
     measEutraPeridRptCfg[MSM_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS].
     isPerdStrongCellCfgRptEnable = TRUE; 
   RETVALUE(ROK);
}

#else

/*
 *
 *       Fun:    smUpdateNlTxPwrVal
 *
 *       Desc:   update the PCI value modified by SON
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smUpdateNlTxPwrVal
(
NlUpdateTxPwrCfgInd  *txPwrCfgInd
)
#else
PUBLIC S16 smUpdateNlTxPwrVal(txPwrCfgInd)
NlUpdateTxPwrCfgInd  *txPwrCfgInd;
#endif /* ANSI */
{
   TRC3(smUpdateNlTxPwrVal);

   RETVALUE(ROK);
}
/*
 *
 *       Fun:    smBuildNlGenCfg
 *
 *       Desc:   Build SON general configuration
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PUBLIC S16 smBuildNlGenCfg
(
NlGenCfg   *genCfg
)
#else
PUBLIC S16 smBuildNlGenCfg(genCfg)
NlGenCfg   *genCfg;
#endif /* ANSI */
{
   TRC3(smBuildNlGenCfg);

   genCfg->tmrRes = 10;
   /*update PCI selection type*/
   switch(wrSmPciSelType)
   {
      case 0 : 
      {
         genCfg->pciSelType = NL_PCI_SEL_DISABLE;
      }
      break;
      case 1 :
      {
         genCfg->pciSelType = NL_PCI_SEL_DSON;
      }
      break;
      case 2 :
      {
         genCfg->pciSelType = NL_PCI_SEL_CSON;
      }
      break;
      default :
      break;
   }
   /* update PRACH selection type*/
   switch(wrSmPrachSelType)
   {
      case 0 :
      {
         genCfg->prachSelType = NL_PRACH_SEL_DISABLE;
      }
      break;
      case 1 :
      {
         genCfg->prachSelType = NL_PRACH_SEL_DSON;
      }
      break;
      case 2 :
      {
         genCfg->prachSelType = NL_PRACH_SEL_CSON;
      }
      break;
      default :
      break;
   }
   /*update EARFCN selection type*/
   switch(wrSmEarfcnSelType)
   {
      case 0 :
      {
         genCfg->earfcnSelType = NL_EARFCN_SEL_DISABLE;
      }
      break;
      case 1 :
      {
         genCfg->earfcnSelType = NL_EARFCN_SEL_DSON;
      }
      break;
      case 2 :
      {
         genCfg->earfcnSelType = NL_EARFCN_SEL_CSON;
      }
      break;
      default :
      break;
   }
   genCfg->maxUeSupp = smCfgCb.maxUeSupp;
   genCfg->maxUePerTti = wrSmDfltMaxUePerDlSf;
   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smBuildNlNghCellCfg
 *
 *       Desc:   Build neighbor cell configuration
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PUBLIC S16 smBuildNlNghCellCfg
(
LnlNghCellCfg   *nghCellCfg
)
#else
PUBLIC S16 smBuildNlNghCellCfg(nghCellCfg)
LnlNghCellCfg   *nghCellCfg;
#endif /* ANSI */
{
   U32                 enbId;
   U32                 dupCount;
   U32                 nghCellCount; 
   LnlNghCellAdd       *nghAdd = NULLP;
   LnlNghCellCfgParam  *nghCellParam = NULLP;
   LnlNghCellInfo      *nghCellInfo = NULLP;
   TRC3(smBuildNlNghCellCfg);
   nghCellCfg->servCellId = /*wrSmDfltCellId*/ 1;
   nghCellCfg->numNghEnb = smCfgCb.noOfNghCfg;
   for(nghCellCount = 0; nghCellCount < nghCellCfg->numNghEnb; nghCellCount ++)
   {
      enbId = smCfgCb.wrNghCellCfg[nghCellCount]->enbId;
      for(dupCount = 0; dupCount < nghCellCount; dupCount++)
      {
         /* checking if the eNB is already add to configuration */
         /* if yes, ignoring the dulicate configuration and proceeding */
         /* this may occur when multiple cells or same eNB is configured */
         if(enbId == smCfgCb.wrNghCellCfg[dupCount]->enbId)
         {
            continue;
         }
      }
      if(smCfgCb.wrNghCellCfg[nghCellCount]->enbType == 0)
      {
         nghCellCfg->nghEnbCellCfg[nghCellCount].nghEnbId = (enbId >> 8); 
      }
      else
      {
         nghCellCfg->nghEnbCellCfg[nghCellCount].nghEnbId = enbId;
      }
      nghCellParam = &(nghCellCfg->nghEnbCellCfg[nghCellCount].nghCellParam);
      nghCellParam->cfgType = LNL_NGH_ADD;
      nghAdd = &(nghCellParam->u.nghCellAdd);
      nghAdd->numNgh = 1;
      nghAdd->nghCellAddInfo[0].numNghNgh = 0;
      nghCellInfo = &(nghAdd->nghCellAddInfo[0].nghCellInfo);
      nghCellInfo->ecgi.cellId = nghCellCfg->nghEnbCellCfg[nghCellCount].nghEnbId;
      /*FILL PLMN ID*/
      nghCellInfo->ecgi.plmn.numMncDigits = 
           smCfgCb.wrNghCellCfg[nghCellCount]->noMnc;
      nghCellInfo->ecgi.plmn.mcc[0] = 
           smCfgCb.wrNghCellCfg[nghCellCount]->nghPlmn[0];
      nghCellInfo->ecgi.plmn.mcc[1] = 
           smCfgCb.wrNghCellCfg[nghCellCount]->nghPlmn[1];
      nghCellInfo->ecgi.plmn.mcc[2] = 
           smCfgCb.wrNghCellCfg[nghCellCount]->nghPlmn[2];
      nghCellInfo->ecgi.plmn.mnc[0] = 
           smCfgCb.wrNghCellCfg[nghCellCount]->nghPlmn[3];
      nghCellInfo->ecgi.plmn.mnc[1] = 
           smCfgCb.wrNghCellCfg[nghCellCount]->nghPlmn[4];
      nghCellInfo->ecgi.plmn.mnc[2] = 
           smCfgCb.wrNghCellCfg[nghCellCount]->nghPlmn[5];
      nghCellInfo->pci = 
           smCfgCb.wrNghCellCfg[nghCellCount]->physCellId;
      nghCellInfo->dlEarfcn = 
           smCfgCb.wrNghCellCfg[nghCellCount]->dlEarfcn;
      nghCellInfo->ulEarfcn = 
           smCfgCb.wrNghCellCfg[nghCellCount]->ulEarfcn;

      RLOG3(L_INFO,"SM->SON Neighbor Config [Numcell %u]: enbid = %lu, pci = %u\n",
            nghCellCount + 1,
            nghCellCfg->nghEnbCellCfg[nghCellCount].nghEnbId,
            nghCellInfo->pci);

      /* This is from wr_cfg.txt. Hence no neighbor cell has prach information
       * populated */
      nghCellInfo->isNghPrachInfo = FALSE;
   }

   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smBuildNlCellReCfg
 *
 *       Desc:   Build Cell Re-configuration of a cell
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PUBLIC S16 smBuildNlCellReCfg
(
LnlCellReCfg   *cellReCfg
)
#else
PUBLIC S16 smBuildNlCellReCfg(cellReCfg)
LnlCellReCfg   *cellReCfg;
#endif /* ANSI */
{
   TRC3(smBuildNlCellReCfg);
   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smBuildNlCellPucchCfg
 *
 *       Desc:   Build Cell PUCCH Config params of a cell
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PRIVATE S16 smBuildNlCellPucchCfg
(
LnlPucchCfg   *pucchCfg
)
#else
PRIVATE S16 smBuildNlCellPucchCfg(pucchCfg)
LnlPucchCfg *pucchCfg;
#endif /* ANSI */
{
   TRC3(smBuildNlCellPucchCfg);

   pucchCfg->pucchCqiResIdx = wrSmDfltPUCCHnRBCQI;
   pucchCfg->n1PucchAn = wrSmDfltNlPUCCHAn;
   pucchCfg->deltaShift = wrSmDfltPUCCHDeltaShift + 1;
   pucchCfg->cyclicShift = wrSmDfltPUCCHnCSAN;

   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smBuildNlCellPrachCfg
 *
 *       Desc:   Build Cell Prach Config params of a cell
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PRIVATE S16 smBuildNlCellPrachCfg
(
LnlPrachCfg   *prachCfg
)
#else
PRIVATE S16 smBuildNlCellPrachCfg(prachCfg)
LnlPrachCfg   *prachCfg;
#endif /* ANSI */
{
   TRC3(smBuildNlCellPrachCfg);

   prachCfg->servRootSeqIdx = smCfgCb.sonPrachCfg.servRootSeqIdx;
   prachCfg->servPrachCfgIdx = smCfgCb.sonPrachCfg.servPrachCfgIdx;
   prachCfg->servZeroCorrelationZoneCfg =
      smCfgCb.sonPrachCfg.servZeroCorrelationZoneCfg;
   prachCfg->servPrachFreqOffset = smCfgCb.sonPrachCfg.servPrachFreqOffset;
   prachCfg->highSpeedFlag = smCfgCb.sonPrachCfg.highSpeedFlag;

   prachCfg->numRootSeqIndex = smCfgCb.sonPrachCfg.numRootSeqIndex;
   if(smCfgCb.sonPrachCfg.numRootSeqIndex)
   {
      U8 rootSeqIdxCnt;
      for(rootSeqIdxCnt = 0; 
          rootSeqIdxCnt < prachCfg->numRootSeqIndex;
          rootSeqIdxCnt++)
      {
         prachCfg->rootSeqIdx[rootSeqIdxCnt] =
            smCfgCb.sonPrachCfg.rootSeqIdxLst[rootSeqIdxCnt];
      }
   }

   prachCfg->numPrachCfgIdx = smCfgCb.sonPrachCfg.numPrachCfgIdx;
   if(smCfgCb.sonPrachCfg.numPrachCfgIdx)
   {
      U8 prachCfgIdxCnt;
      for(prachCfgIdxCnt = 0;
          prachCfgIdxCnt < prachCfg->numPrachCfgIdx;
          prachCfgIdxCnt ++)
      {
         prachCfg->prachConfigIdx[prachCfgIdxCnt] =
            smCfgCb.sonPrachCfg.prachCfgIdxLst[prachCfgIdxCnt];
      }
   }

   prachCfg->numZeroCorrCfgZone = smCfgCb.sonPrachCfg.numZeroCorrCfgZone;
   if(smCfgCb.sonPrachCfg.numZeroCorrCfgZone)
   {
      U8 zeroCorrCfgZoneCnt;
      for(zeroCorrCfgZoneCnt = 0;
          zeroCorrCfgZoneCnt < prachCfg->numZeroCorrCfgZone;
          zeroCorrCfgZoneCnt ++)
      {
         prachCfg->zeroCorrelationZoneCfg[zeroCorrCfgZoneCnt] =
            smCfgCb.sonPrachCfg.zeroCorrZoneCfgLst[zeroCorrCfgZoneCnt];
      }
   }

   prachCfg->numFreqOffset = smCfgCb.sonPrachCfg.numPrachFreqOffset;
   if(smCfgCb.sonPrachCfg.numRootSeqIndex)
   {
      U8 freqOffsetCnt;
      for(freqOffsetCnt = 0;
          freqOffsetCnt < prachCfg->numFreqOffset;
          freqOffsetCnt ++)
      {
         prachCfg->freqOffset[freqOffsetCnt] =
            smCfgCb.sonPrachCfg.freqOffsetLst[freqOffsetCnt];
      }
   }

   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smBuildNlCellCsgCfg
 *
 *       Desc:   Build Cell Csg configuration of a cell
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PRIVATE S16 smBuildNlCellCsgCfg
(
LnlCellCfg   *cellCfg
)
#else
PRIVATE S16 smBuildNlCellCsgCfg(cellCfg)
LnlCellCfg   *cellCfg;
#endif /* ANSI */
{
   TRC3(smBuildNlCellCsgCfg);
   U16        csgPciRange = 0;
   TRC3(smBuildNlCellCsgCfg);
   cellCfg->csgPciCfg.isCsgPciCfg = wrSmDfltIsCsgPciCfgd;
   cellCfg->csgPciCfg.csgStartPci = wrSmDfltCSGStartPci;
   WR_EMM_GET_PCI_RANGE(wrSmDfltCSGPciRange, csgPciRange);
   cellCfg->csgPciCfg.csgPciRange = csgPciRange;

   cellCfg->smallCellPciCfg.isSmallCellPciCfg = wrSmDfltIsSmlCellPciCfgd;
   if(TRUE == cellCfg->smallCellPciCfg.isSmallCellPciCfg)
   {
      cellCfg->smallCellPciCfg.smallCellStartPci = wrSmDfltSmallCellStartPci;
      cellCfg->smallCellPciCfg.maxSmallCellPci = wrSmDfltSmallCellPciRange;
   }
   switch(wrSmDfltAccessMode)
   {
      case WR_ENB_OPEN_ACCESS :
      {
         cellCfg->csgPciCfg.accessMode = LNL_ENB_OPEN_ACCESS;
      }
      break;
      case WR_ENB_CLOSED_ACCESS :
      {
         cellCfg->csgPciCfg.accessMode = LNL_ENB_CLOSED_ACCESS;
      }
      break;
      case WR_ENB_HYBRID_ACCESS :
      {
         cellCfg->csgPciCfg.accessMode = LNL_ENB_HYBRID_ACCESS;
      }
      default :
      {
         break;
      }
   }

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smBuildNlCellEarfcnCfg
 *
 *       Desc:   Build Cell configuration of a cell
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PUBLIC S16 smBuildNlCellEarfcnCfg
(
LnlCellCfg   *cellCfg
)
#else
PUBLIC S16 smBuildNlCellEarfcnCfg(cellCfg)
LnlCellCfg   *cellCfg;
#endif /* ANSI */
{

   U32    earfcnCount;

   TRC3(smBuildNlCellEarfcnCfg);

   cellCfg->numDlEarfcn = smCfgCb.numDlEarfcn;
   cellCfg->numUlEarfcn = smCfgCb.numUlEarfcn;
   for(earfcnCount = 0; earfcnCount < cellCfg->numDlEarfcn; earfcnCount ++)
   {
      cellCfg->dlEarfcnList[earfcnCount] = smCfgCb.dlEarfcnList[earfcnCount];
   }
   for(earfcnCount = 0; earfcnCount < cellCfg->numUlEarfcn; earfcnCount ++)
   {
      cellCfg->ulEarfcnList[earfcnCount] = smCfgCb.ulEarfcnList[earfcnCount];
   }
   cellCfg->numRemScanEarfcn = 0;
   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smBuildNlCellPciCfg
 *
 *       Desc:   Build Cell configuration of a cell
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PUBLIC S16 smBuildNlCellPciCfg
(
LnlCellCfg   *cellCfg
)
#else
PUBLIC S16 smBuildNlCellPciCfg(cellCfg)
LnlCellCfg   *cellCfg;
#endif /* ANSI */
{
   U32   pciCount;

   TRC3(smBuildNlCellPciCfg);
   cellCfg->pci = wrSmDfltPhysCellId;
   cellCfg->numPci = smCfgCb.numPci;
   for(pciCount = 0; pciCount < cellCfg->numPci; pciCount ++)
   {
      cellCfg->pciList[pciCount] = smCfgCb.pciList[pciCount];
   }
   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smBuildNlCellCfg
 *
 *       Desc:   Build Cell configuration of a cell 
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PUBLIC S16 smBuildNlCellCfg
(
LnlCellCfg   *cellCfg
)
#else
PUBLIC S16 smBuildNlCellCfg(cellCfg)
LnlCellCfg   *cellCfg;
#endif /* ANSI */
{
   TRC3(smBuildNlCellCfg);
   cellCfg->cellId = 1;
   cellCfg->ecgi.cellId = wrSmDfltCellId;
   cellCfg->ecgi.plmn.numMncDigits = wrSmDfltNumMncDigits;
   cellCfg->ecgi.plmn.mcc[0] = smCfgCb.mcc[0].val;
   cellCfg->ecgi.plmn.mcc[1] = smCfgCb.mcc[1].val;
   cellCfg->ecgi.plmn.mcc[2] = smCfgCb.mcc[2].val;
   cellCfg->ecgi.plmn.mnc[0] = smCfgCb.mnc[0].val;
   cellCfg->ecgi.plmn.mnc[1] = smCfgCb.mnc[1].val;
   cellCfg->ecgi.plmn.mnc[2] = smCfgCb.mnc[2].val;
   smBuildNlCellCsgCfg(cellCfg);
   smBuildNlCellEarfcnCfg(cellCfg);
   smBuildNlCellPciCfg(cellCfg);

   cellCfg->bandWidth = wrSmDfltCellDlBW;
   if(NHU_MODE_TDD == wrSmDfltDuplexMode)
   {
      cellCfg->duplexMode = 1;
   }
   else
   {
      cellCfg->duplexMode = 0;
   }

#ifdef LTE_TDD
   cellCfg->subFrameCfg = wrSmTddSubframeConfig;
#endif

   smBuildNlCellPrachCfg(&(cellCfg->prachCfgInfo));
   smBuildNlCellPucchCfg(&(cellCfg->pucchResCfg));
   /* PHICH Configuration */
   cellCfg->phichCfg.ngEnum = wrSmDfltPhichRes;
   cellCfg->phichCfg.isDurExtend = wrSmDfltPhichDur;

   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smUpdateNlPciVal
 *
 *       Desc:   update the PCI val selected by SON 
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */

#ifdef ANSI
PUBLIC S16 smUpdateNlPciVal
(
U16   pci
)
#else
PUBLIC S16 smUpdateNlPciVal(pci)
U16   pci;
#endif /* ANSI */
{
   TRC3(smUpdateNlPciVal);
   wrSmDfltPhysCellId = pci;
   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smUpdateNlEarfcnVal
 *
 *       Desc:   update the EARFCN val selected by SON 
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smUpdateNlEarfcnVal
(
NlUpdateEarfcnCfgInd   *earfcnCfgInd
)
#else
PUBLIC S16 smUpdateNlPciVal(earfcnCfgInd)
NlUpdateEarfcnCfgInd   *earfcnCfgInd;
#endif /* ANSI */
{
   U8   count;
   TRC3(smUpdateNlEarfcnVal);
   /*update the DL/UL EARFCN value configured through wr_sfg.txt*/
   for(count = 0; count < smCfgCb.noOfEutraFreq; count ++)
   {
      if((smCfgCb.nghEutraFreq[count]->arfcnDl == wrSmDfltServingCellDLCarrierFreq) &&
        (smCfgCb.nghEutraFreq[count]->arfcnUl == wrSmDfltUlEarFcn))
      {
         smCfgCb.nghEutraFreq[count]->arfcnDl = earfcnCfgInd->dlEarfcn;
         smCfgCb.nghEutraFreq[count]->arfcnUl = earfcnCfgInd->ulEarfcn;
         break;
      }
   }
   wrSmDfltServingCellDLCarrierFreq = earfcnCfgInd->dlEarfcn;
   wrSmDfltEartcnNrDLEarfcn[0] = wrSmDfltServingCellDLCarrierFreq;
   wrSmDfltUlEarFcn = earfcnCfgInd->ulEarfcn; 
   RETVALUE(ROK);
}


/*
 *
 *       Fun:    smUpdateNlPrachVal
 *
 *       Desc:   update the PRACH val selected by SON 
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smUpdateNlPrachVal
(
NlUpdatePrachCfgInd   *prachCfgInd
)
#else
PUBLIC S16 smUpdateNlPrachVal(prachCfgInd)
NlUpdatePrachCfgInd   *prachCfgInd;
#endif /* ANSI */
{
   TRC3(smUpdateNlPrachVal);

   wrSmDfltRootSequenceIdx = prachCfgInd->rootSequenceIndx;;
   wrSmDfltHighSpeedFlag = prachCfgInd->highSpeedFlag;
   wrSmDfltPrachCfgIdx = prachCfgInd->prachCfgIdx;
   wrSmDfltPrachFreqOffset = prachCfgInd->prachFreqOffset;
   wrSmDfltZeroCorrelZoneCfg = prachCfgInd->zeroCorrelationZoneCfg;

   RETVALUE(ROK);
}

/*
 *
 *       Fun:    smUpdateNlMeasRptInterval
 *
 *       Desc:   update the measurement interval value configured by BRDCM-SON
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_smm_son_utils.c
 *
 */
#ifdef ANSI
PUBLIC S16 smUpdateNlMeasRptInterval
(
U32   measInterval
)
#else
PUBLIC S16 smUpdateNlMeasRptInterval(measInterval)
U32   measInterval;
#endif /* ANSI */
{
   TRC3(smUpdateNlMeasRptInterval);
//      wrSmDfltRptInterval = measInterval;
   RETVALUE(ROK);
}



#endif /* WR_RSYS_OAM */
/********************************************************************30**

           End of file:     fc_smm_son_utils.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/9 - Tue Jan 13 12:01:15 2015

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
SID$        ---      
*********************************************************************91*/

