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
               

     File:     wr_emm_cfg.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=6;

/* header include files (.h) */
#include "wr.h"
#include "wr_cmn.h"
#include "wr_smm_smallcell.h"
#include "wr_emm.h"
#include "wr_lwr.h"
#include "wr_ifm_rrm.h"
#include "wr_ifm_son.h"
/* header include files (.x) */
#include "wr_emm_cfg.h"
#include "wr_emm_neighenb.h"
#include "wr_emm_mme.h"
#include "wr_umm.h"
#include "wr_emm_pbm.h"
#include "wr_utils.h"
#include "wr_emm_pws.h"
#include "wr_emm_rim.h"
#include "wr_emm_rim_app.h"

PUBLIC Void wrDebugShowBaaringPrams(WrSib2AcBarringCfg *sib2DataCall, WrSib2AcBarringCfg *sib2Signalling);
EXTERN S16  wrDiagPrntPlmnId(WrPlmnId *plmnId, U8 diagLvl);
EXTERN S16  wrEmmCellInit(WrCellCb *cellCb);
EXTERN S16 wrUmmInitRabParams(WrCellCb  *cellCb);
EXTERN S16 wrUmmAddKPILaiList(WrCellId celId, WrPlmnId plmnId, U16 lac); 
EXTERN S16 wrEmmS1FlexInitS1FlexVarFrMme(WrMmeCb* mmeCb);
EXTERN U32 numTtiTicks;

PUBLIC Void wrEmmMmeBldEnbConfigTransfer
(
   WrEmmEnodCfgParama         *enbCfgparama,
   U16                        cellId
);
PUBLIC S16 wrEmmSndLwrAddNeighCfm(LwrMngmt *cfg);

PRIVATE S16 wrEmmProcEnbModMmeLdBalPriGrpsLst
(
 LwrENodeBCfg         *cfgMod,
 U16                  *numOfMmePriGrps,
 Bool                 newAddition
);


PRIVATE Void wrEmmPrintCellTddCfg
(
   WrSib1Params              *sib1
);

PRIVATE Void wrEmmPrintNeighFreqCb1
(
     WrEutranFreqCb              *neighFreqCb
);

PRIVATE Void wrEmmPrintNeighFreqCb
(
  WrEutranFreqCb                *neighFreqCb
);

PRIVATE Void wrEmmPrintCellTmrCfg
(
   WrCellCb                    *cellCb
);

PRIVATE Void wrEmmPrintCellAccessInfo
(
   WrSib1Params              *sib1
);

PRIVATE Void wrEmmPrintSib1AccessInfo
(
   WrSib1Params              *sib1
);


PRIVATE Void wrEmmPrintCellSelectInfo
(
   WrSib1Params              *sib1
);

PRIVATE Void wrEmmPrintUlPwrCtrlCfg(WrSib2Params *sib2);

PRIVATE Void wrEmmPrintPrachCfg(WrSib2Params *sib2);

PRIVATE S16 wrEmmCfgValidateNeighFreqAdd
(
 U8                               cellId,
 LwrNeighFreqAddModCfg            *nbrFreqAdd
 );

PRIVATE S16 wrEmmProcessPciUpdate
(
WrCellCb            *cellCb
);

/* For TDDMode UTRA_TDD_128 and given bands,  need to verify the frequency*/
PRIVATE S16 wrEmmMatchFreqAndBandForTDDMode_128
(
 U32        arfcn,
 U8         bandInd
);
/* For TDDMode UTRA_TDD_384 and given bands,  need to verify the frequency*/
PRIVATE S16 wrEmmMatchFreqAndBandForTDDMode_384
(
 U32        arfcn,
 U8         bandInd
);

/* For TDDMode UTRA_TDD_768 and given bands,  need to verify the frequency*/
PRIVATE S16 wrEmmMatchFreqAndBandForTDDMode_768
(
 U32        arfcn,
 U8         bandInd
);

/* For various TDD bands need to verify the following */
PRIVATE U16 UTRATDD384_band_ab[2][4]={
   {9512,9588,10062,10113},
   {9262,9538,9662,9938}
};
PRIVATE U16 UTRATDD384_band_cd[2][2]={
   {9562,9638},
   {12862,13088}
};

PRIVATE U16 UTRATDD128_band_ab[2][4]={
   {9504,9596,10054,10121},
   {9254,9546,9654,9946}
};
PRIVATE U16 UTRATDD128_band_cdef[4][2]={
   {9554,9646},
   {12854,13096},
   {11504,11996},
   {9404,9596}
};

PRIVATE U16 UTRATDD768_band_ab[2][4]={
   {9512,9588,10062,10113},
   {9262,9538,9662,9938}
};
PRIVATE U16 UTRATDD768_band_cd[2][2]={
   {9562,9638},
   {12874,13076}
};


/** @brief This function is responsible for finding out if freq
 *           lies in the assigned band range
 * @details
 *
 *     Function: wrMatchFreqAndBandForTDDMode_128 
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] arfcn : 
 * @param [in] band:
 * @return 
 *       -# TRUE
 *       -# FALSE
 */
PRIVATE S16 wrEmmMatchFreqAndBandForTDDMode_128
(
 U32        arfcn,
 U8         bandInd
)
{
   switch(bandInd)
   {
      case WR_EMM_UTRA_TDD_BAND_A:
      case WR_EMM_UTRA_TDD_BAND_B:
      {
         /*Here. we are subtracting index by 1 to start from 0th index of array.
          * Since Bands start from 1, there array index is 0*/
         if((arfcn >= UTRATDD128_band_ab[bandInd-1][0] &&
                  arfcn <= UTRATDD128_band_ab[bandInd-1][1]) ||
               (arfcn >= UTRATDD128_band_ab[bandInd-1][2] &&
                arfcn <= UTRATDD128_band_ab[bandInd-1][3]))
         {
            RETVALUE(ROK);
         }
         break;
      }

      case WR_EMM_UTRA_TDD_BAND_C:
      case WR_EMM_UTRA_TDD_BAND_D:
      case WR_EMM_UTRA_TDD_BAND_E:
      case WR_EMM_UTRA_TDD_BAND_F:
      {
         /*Here. we are subtracting index by 3 to start from 0th index of array.
          * Since Bands start from 3, there array index is 0*/
         if((arfcn >= UTRATDD128_band_cdef[bandInd-3][0] &&
                  arfcn <= UTRATDD128_band_cdef[bandInd-3][1]))
         {
            RETVALUE(ROK);
         }
         break;
      }
      default:
      {
         RLOG1(L_WARNING, "bandIndicator is configured with incorrect value = %d", bandInd);
         break;
      }
   }   
   RETVALUE(RFAILED);
}

/** @brief This function is responsible for finding out if freq
 *           lies in the assigned band range
 * @details
 *
 *     Function: wrEmmMatchFreqAndBandForTDDMode_384 
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] arfcn : 
 * @param [in] band:
 * @return 
 *       -# TRUE
 *       -# FALSE
 */
PRIVATE S16 wrEmmMatchFreqAndBandForTDDMode_384
(
 U32        arfcn,
 U8         bandInd
)
{
   switch(bandInd)
   {
      case WR_EMM_UTRA_TDD_BAND_A:
      case WR_EMM_UTRA_TDD_BAND_B:
      {
         /*Here. we are subtracting index by 1 to start from 0th index of array.
          * Since Bands start from 1, there array index is 0*/
         if((arfcn >= UTRATDD384_band_ab[bandInd-1][0] &&
                  arfcn <= UTRATDD384_band_ab[bandInd-1][1]) ||
               (arfcn >= UTRATDD384_band_ab[bandInd-1][2] &&
                arfcn <= UTRATDD384_band_ab[bandInd-1][3]))
         {
            RETVALUE(ROK);
         }
         break;
      }   

      case WR_EMM_UTRA_TDD_BAND_C:
      case WR_EMM_UTRA_TDD_BAND_D:
      {
         /*Here. we are subtracting index by 3 to start from 0th index of array.
          * Since Bands start from 3, there array index is 0*/
         if((arfcn >= UTRATDD384_band_cd[bandInd-3][0] &&
                  arfcn <= UTRATDD384_band_cd[bandInd-3][1]))
         {
            RETVALUE(ROK);
         }
         break;
      }
      default:
      {
         RLOG1(L_INFO, "bandIndicator is configured with incorrect value = %d", bandInd);
         break;
      }
   }
   RETVALUE(RFAILED);
}
/** @brief This function is responsible for finding out if freq
 *           lies in the assigned band range
 * @details
 *
 *     Function: wrEmmMatchFreqAndBandForTDDMode_768 
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] arfcn : 
 * @param [in] band:
 * @return 
 *       -# TRUE
 *       -# FALSE
 */
PRIVATE S16 wrEmmMatchFreqAndBandForTDDMode_768
(
 U32        arfcn,
 U8         bandInd
)
{
   switch(bandInd)
   {
      case WR_EMM_UTRA_TDD_BAND_A:
      case WR_EMM_UTRA_TDD_BAND_B:
      {
         /*Here. we are subtracting index by 1 to start from 0th index of array.
          * Since Bands start from 1, there array index is 0*/
         if((arfcn >= UTRATDD768_band_ab[bandInd-1][0] &&
                  arfcn <= UTRATDD768_band_ab[bandInd-1][1]) ||
               (arfcn >= UTRATDD768_band_ab[bandInd-1][2] &&
                arfcn <= UTRATDD768_band_ab[bandInd-1][3]))
         {
            RETVALUE(ROK);
         }
         break;
      }

      case WR_EMM_UTRA_TDD_BAND_C:
      case WR_EMM_UTRA_TDD_BAND_D:
      {
         /*Here. we are subtracting index by 3 to start from 0th index of array.
          * Since Bands start from 3, there array index is 0*/
         if((arfcn >= UTRATDD768_band_cd[bandInd-3][0] &&
                  arfcn <= UTRATDD768_band_cd[bandInd-3][1]))
         {
            RETVALUE(ROK);
         }
         break;
      }
      default:
      {
         RLOG1(L_INFO, "bandIndicator is configured with incorrect value = %d", bandInd);
         break;
      }
   }
   RETVALUE(RFAILED);
}

/** @brief This function adds the SIB group's information to the cellCb
 *
 * @details adds the SIB group's information to the cellCb
 *
 *     Function: wrEmmCfgCellAddSib1
 *
 *         Processing steps:
 *         - copying the sib1 parameter to cell control block
 *
 * @param [out]  cellCb : Cell control Block
 * @param [in]  sib1CfgGrp : SIB1 Configuration parameters
 * @return S16
 *          -# Success : ROK 
 *          -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellAddSib1 
(
 WrCellCb                     *cellCb,
 LwrCellSib1CfgGrp            *sib1CfgGrp
 )
{
   WrSib1Params              *sib1 = &(cellCb->sib1);
   S16                       indx = 0;


#if 1
   sib1->numPlmnIds     = sib1CfgGrp->cellAccessInfo.numOfPlmns;
   for (indx = 0; indx < sib1->numPlmnIds; indx++)
   {
      sib1->plmns[indx].resvForOperUse = 
         sib1CfgGrp->cellAccessInfo.plmnInfo[indx].rsrvdFrOperUse;
      /* Mark the plmn as NOT activated intially, would be marked as up once
       * atleast ONE associated MME comes up.
       */
      sib1->plmns[indx].activated= FALSE;
      sib1->plmns[indx].plmnId.numMncDigits = 
         sib1CfgGrp->cellAccessInfo.plmnInfo[indx].plmnId.numMncDigits;
      /* Copy MCC and MNC */
      cmMemcpy((U8*)sib1->plmns[indx].plmnId.mcc, 
            (U8*)sib1CfgGrp->cellAccessInfo.plmnInfo[indx].plmnId.mcc, 
            (3 * sizeof(U8)));
      cmMemcpy((U8*)sib1->plmns[indx].plmnId.mnc, 
            (U8*)sib1CfgGrp->cellAccessInfo.plmnInfo[indx].plmnId.mnc, 
            (sib1->plmns[indx].plmnId.numMncDigits * sizeof(U8)));
   }
#else
   wrEmmCfgCellAddSib1AddPlmn (cellCb, sib1CfgGrp);
#endif

   /* Fill SIB1 access related parameters */
   sib1->tac                  = sib1CfgGrp->cellAccessInfo.tac;
   sib1->cellIdentity         = sib1CfgGrp->cellAccessInfo.cellId;
   sib1->cellBarred           = sib1CfgGrp->cellAccessInfo.cellBarred;
   sib1->intraFreqReselection = sib1CfgGrp->cellAccessInfo.intraFreqReSelection;
   sib1->csgInd               = sib1CfgGrp->cellAccessInfo.csgIndication;
   sib1->csgId.pres           = sib1CfgGrp->cellAccessInfo.csgIdentity.pres;
   sib1->csgId.val            = sib1CfgGrp->cellAccessInfo.csgIdentity.val;

   /* Fill SIB1 cell selection related parameters */
   sib1->qRxLevMin            = sib1CfgGrp->cellSelectInfo.qRxLevMin;
   sib1->qRxLevMinOffsetPres  = sib1CfgGrp->cellSelectInfo.qRxLevMinOffset.pres;
   sib1->qRxLevMinOffset      = (sib1->qRxLevMinOffsetPres == PRSNT_NODEF) ? 
      sib1CfgGrp->cellSelectInfo.qRxLevMinOffset.val : 0;     

   /* Fill SIB1 Power and frequency related parameters */
   sib1->pMax                    = sib1CfgGrp->pMax;
   sib1->freqBandIndicator       = sib1CfgGrp->freqBandInd;
   /* LWR enum maps to int values */
   sib1->siWindowLen             = sib1CfgGrp->siWindowSize;
   
   /* TDD configuration  */
   sib1->tddCfg.subframeCfg      = sib1CfgGrp->tddCfg.subframeCfg;
   sib1->tddCfg.specialSubframeCfg = sib1CfgGrp->tddCfg.specialSubframeCfg;

   /* initializing the SIB1's info tag to 0 */
   sib1->valueTag                = 0;
   RETVALUE(ROK);
} /* end of wrEmmCfgCellAddSib1 */ 

/** @brief This function adds the SIB group's information to the cellCb
 *
 * @details adds the SIB group's information to the cellCb
 *
 *     Function: wrEmmCfgCellAddSib6
 *
 *         Processing steps:
 *         - copying the sib1 parameter to cell control block
 *
 * @param [out]  cellCb : Cell control Block
 * @param [in]  sib1CfgGrp : SIB1 Configuration parameters
 * @return S16
 *          -# Success : ROK
 *          -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellAddSib6
(
 WrCellCb                     *cellCb,
 LwrCellSib6CfgGrp            *sib6CfgGrp
)
{
   cellCb->sib6.sfUsagePres = sib6CfgGrp->sfUsagePres.pres;
   cellCb->sib6.tReselection = sib6CfgGrp->tReselection;
   if(sib6CfgGrp->sfUsagePres.pres == TRUE)
   {
      cellCb->sib6.tReselectionSfMedium = sib6CfgGrp->tReselectionSfMedium;
      cellCb->sib6.tReselectionSfHigh = sib6CfgGrp->tReselectionSfHigh;
   }
   RETVALUE(ROK);
} /* end of wrEmmCfgCellAddSib6 */


/** @brief This function validates the SIB7 configuration request parameters
 *
 * @details validates SIB7 configuration request parameters
 *
 *     Function: wrEmmCfgCellValidateSib7
 *
 *         Processing steps:
 *         - validate SIB7 configuration parameters
 *
 * @param [in]  sib7CfgGrp : SIB7 Configuration parameters
 * @return S16
 *          -# Success : ROK
 *          -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellValidateSib7
(
 LwrCellSib7CfgGrp            *sib7CfgGrp
)
{
   S16 ret =  ROK;
   U8 tReselectionGERAN = sib7CfgGrp->tReselectionGERAN;

   if(tReselectionGERAN > 7)
   {
      RLOG1(L_ERROR,"Wrong value of tReselectionGERAN [%u] is configured",tReselectionGERAN);
      ret = RFAILED;
   }

   if(sib7CfgGrp->pres_SpeedStateScaleFactors.pres == TRUE)
   {
      if((sib7CfgGrp->tReselectionGERANSFMedium > 31) || 
            (sib7CfgGrp->tReselectionGERANSFHigh > 31))
      {
         RLOG2(L_ERROR,"Wrong value of tReselectionGERANSF parameters is configured."
                      "tReselectionGERANSFMedium = %d,tReselectionGERANSFHigh = %d",
                      sib7CfgGrp->tReselectionGERANSFMedium,sib7CfgGrp->tReselectionGERANSFHigh);
         ret = RFAILED;
      }
   }
   RETVALUE(ret);
}

/** @brief This function adds the SIB group's information to the cellCb
 *
 * @details adds the SIB group's information to the cellCb
 *
 *     Function: wrEmmCfgCellAddSib7
 *
 *         Processing steps:
 *         - copying the sib7 parameter to cell control block
 *
 * @param [out]  cellCb : Cell control Block
 * @param [in]  sib7CfgGrp : SIB7 Configuration parameters
 * @return S16
 *          -# Success : ROK
 *          -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellAddSib7
(
 WrCellCb                     *cellCb,
 LwrCellSib7CfgGrp            *sib7CfgGrp
)
{
   S16   ret = ROK;

   ret = wrEmmCfgCellValidateSib7(sib7CfgGrp);
   if(ret == RFAILED)
   {
      RLOG0(L_ERROR,"Validation of SIB7 configuration paramter(s) failed");
      RETVALUE(ret);
   }
   cellCb->sib7.tReselectionGERAN = sib7CfgGrp->tReselectionGERAN;
   if(sib7CfgGrp->pres_SpeedStateScaleFactors.pres == TRUE)
   {
      cellCb->sib7.tReselSpeedSFPres =
            sib7CfgGrp->pres_SpeedStateScaleFactors.pres;
      cellCb->sib7.tReselGeranSFMedium = sib7CfgGrp->tReselectionGERANSFMedium;
      cellCb->sib7.tReselGeranSFHigh = sib7CfgGrp->tReselectionGERANSFHigh;
   }
   RETVALUE(ROK);
} /* end of wrEmmCfgCellAddSib7 */

/** @brief This function updates the modified MIB group's information
 *  and takes necessary actions.
 *
 * @details adds the Modified MIB group's information to the cellCb
 *
 *     Function: wrEmmCfgCellModMib
 *
 *         Processing steps:
 *         - copying the Modified mib parameter to cell control block
 *
 * @param [out]  cellCb : Cell control Block
 * @param [in]  mibCfgGrp : MIB Configuration parameters
 * @return S16
 *          -# Success : ROK 
 *          -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellModMib
(
WrCellCb                     *cellCb,
LwrCellMibCfgGrp             *mibReCfgGrp
)
{
   U32                       rCfgType = 0;
   U32                       mask = 1;
   Bool                      cellReset = FALSE;

   rCfgType =  mibReCfgGrp->reCfgType;

   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_CELL_MIB_CFG_DLBW:
            {  
               cellCb->mib.dlBw = mibReCfgGrp->dlBw;

                  RLOG1(L_DEBUG, "MIB:DlBw[%d]",cellCb->mib.dlBw);


               /* Performing Cell Delete and Add */ 
               cellReset = TRUE;
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            case LWR_CELL_MIB_CFG_PHICH_CFG:
            {  
              rCfgType =  rCfgType ^ mask; 
              /* Performing Cell Delete and Add */ 
              cellReset = TRUE;

              cellCb->mib.phichDur = (mibReCfgGrp->phichCfg.phichDur == WR_PHICH_DURATION_NORMAL) ?
                                      WR_PHICH_DURATION_NORMAL : WR_PHICH_DURATION_EXTENDED;
              cellCb->mib.phichGrps = mibReCfgGrp->phichCfg.phichRes;

                  RLOG1(L_DEBUG, "MIB:PhichDur",cellCb->mib.phichDur);
                  RLOG1(L_DEBUG, "MIB:PhichGrps",cellCb->mib.phichGrps);

              break;
            }
            default:
            {   
               RLOG1(L_WARNING, "Mib reconfiguration received unknown reCfgType [%ld]",
                  (mibReCfgGrp->reCfgType));
               rCfgType =  rCfgType ^ mask; 
               break;
            }
         } /* end of switch */
      } /* end of if */
      mask = mask << 1;
   } /* end of while */
   RLOG_ARG3(L_DEBUG,DBG_CELLID, cellCb->cellId,"MIB is Modified,"
     "dl-Bandwidth[%d] phich-Duration[%d] phich-Resource[%d]",
     cellCb->mib.dlBw, cellCb->mib.phichDur, cellCb->mib.phichGrps);

   
   if (cellReset)
   {
      RLOG_ARG0(L_INFO, DBG_CELLID,cellCb->cellId,
            "Cell state = RESET, Reason = MIB Re-Configuration");

     if (ROK != wrEmmResetCell(cellCb->cellId))
     {
        RLOG0(L_ERROR, "Cell Reset is failed for Mib reconfiguration");
        RETVALUE(RFAILED);
     }
   }

   RETVALUE(ROK);
} /* end of wrEmmCfgCellModMib */
 

/** @brief This function adds the modified SIB group's information
 *  and takes necessary actions.
 *
 * @details adds the Modified SIB group's information to the cellCb
 *
 *     Function: wrEmmCfgCellModSib1
 *
 *         Processing steps:
 *         - copying the Modified sib1 parameter to cell control block
 *
 * @param [out]  cellCb : Cell control Block
 * @param [in]  sib1CfgGrp : SIB1 Configuration parameters
 * @return S16
 *          -# Success : ROK 
 *          -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellModSib1 
(
WrCellCb                     *cellCb,
LwrCellSib1CfgGrp            *sib1ReCfgGrp
)
{
   WrSib1Params              *sib1 = &(cellCb->sib1);
   S16                       ret = ROK;
   U16                       indx = 0;
   U16                       sib1PlmnIndex = 0;
   U32                       rCfgType = 0;
   U32                       mask = 1;
#ifdef WR_TEST_CODE
   PRIVATE Bool              trgNrUpd = FALSE;
#else
   Bool                      trgNrUpd = FALSE;
#endif
   Bool                      isUpdated = FALSE;
   Bool                      cellReset = FALSE;

   rCfgType =  sib1ReCfgGrp->reCfgType;

   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_CELL_SIB1_CFG_CELL_ACCESS_INFO:
            {  
               rCfgType =  rCfgType ^ mask; 
               if ((sib1->tac != sib1ReCfgGrp->cellAccessInfo.tac) && 
                  (sib1ReCfgGrp->cellAccessInfo.tac != 0) )
               {
                  trgNrUpd = TRUE;
               }
               /* Add the new PLMN to SIB1 PLMNs if the same PLMN is not present
                * otherwise, update the received PLMN information */
               RLOG_ARG0(L_DEBUG,DBG_CELLID,cellCb->cellId,"SIB1 Modified PLMN "
                     "changed, List of PLMN supported:");
               for (indx = 0; indx < sib1ReCfgGrp->cellAccessInfo.numOfPlmns; indx++)
               {
                  isUpdated = FALSE;
                  for (sib1PlmnIndex = 0; sib1PlmnIndex < sib1->numPlmnIds; sib1PlmnIndex++)
                  {
                     if (TRUE == wrEmmPlmnPlmnsEqual(&(sib1->plmns[sib1PlmnIndex].plmnId),
                                    (WrPlmnId*)&(sib1ReCfgGrp->cellAccessInfo.plmnInfo[indx].plmnId)))
                     {
                        sib1->plmns[sib1PlmnIndex].resvForOperUse = 
                           sib1ReCfgGrp->cellAccessInfo.plmnInfo[indx].rsrvdFrOperUse;

                        RLOG2(L_DEBUG, "SIB1:PLMN_IDX[%d] ResvForOperUse[%d]",
                           sib1PlmnIndex,sib1->plmns[sib1PlmnIndex].resvForOperUse);
                        isUpdated = TRUE;
                        break;
                     }
                  }
                  if (isUpdated == FALSE) 
                  {
                     sib1->plmns[sib1->numPlmnIds].resvForOperUse = 
                        sib1ReCfgGrp->cellAccessInfo.plmnInfo[indx].rsrvdFrOperUse;
                     sib1->plmns[sib1->numPlmnIds].activated= FALSE;
                     sib1->plmns[sib1->numPlmnIds].plmnId.numMncDigits = 
                        sib1ReCfgGrp->cellAccessInfo.plmnInfo[indx].plmnId.numMncDigits;
                     /* Copy MCC and MNC */
                     cmMemcpy((U8*)sib1->plmns[sib1->numPlmnIds].plmnId.mcc, 
                           (U8*)sib1ReCfgGrp->cellAccessInfo.plmnInfo[indx].plmnId.mcc, 
                           (3 * sizeof(U8)));
                     cmMemcpy((U8*)sib1->plmns[sib1->numPlmnIds].plmnId.mnc, 
                           (U8*)sib1ReCfgGrp->cellAccessInfo.plmnInfo[indx].plmnId.mnc, 
                           (sib1->plmns[sib1->numPlmnIds].plmnId.numMncDigits * sizeof(U8)));

                    wrEmmPrintCellAccessInfo(sib1); 


                     sib1->numPlmnIds++;
                  }
               }
                 /* Fill SIB1 access related parameters */
               RLOG_ARG2(L_DEBUG,DBG_CELLID,cellCb->cellId,"SIB1 Modified"
                  "CSG-Identity changed,old CSG-Identity[%d] "
                  "new CSG-Identity[%d]",sib1->csgId.val,
                  sib1ReCfgGrp->cellAccessInfo.csgIdentity.val);
               RLOG_ARG2(L_DEBUG,DBG_CELLID,cellCb->cellId,"SIB1 Modified"
                  "TrackingAreaCode Changed,old TrackingAreaCode[%d] "
                  "new TrackingAreaCode[%d]",sib1->tac,
                  sib1ReCfgGrp->cellAccessInfo.tac);
                 sib1->tac                  = sib1ReCfgGrp->cellAccessInfo.tac;
                 sib1->cellIdentity         = sib1ReCfgGrp->cellAccessInfo.cellId;
                 sib1->cellBarred           = sib1ReCfgGrp->cellAccessInfo.cellBarred;
                 sib1->intraFreqReselection = sib1ReCfgGrp->cellAccessInfo.intraFreqReSelection;
                 sib1->csgInd               = sib1ReCfgGrp->cellAccessInfo.csgIndication;
                 sib1->csgId.pres           = sib1ReCfgGrp->cellAccessInfo.csgIdentity.pres;
                 sib1->csgId.val            = sib1ReCfgGrp->cellAccessInfo.csgIdentity.val;


                 wrEmmPrintSib1AccessInfo(sib1);

                 break;
            }
            case LWR_CELL_SIB1_CFG_CELL_SELECT_INFO:
            {  
               /* Fill SIB1 cell selection related parameters */
               rCfgType =  rCfgType ^ mask; 
               sib1->qRxLevMin            = sib1ReCfgGrp->cellSelectInfo.qRxLevMin;
               sib1->qRxLevMinOffsetPres  = sib1ReCfgGrp->cellSelectInfo.qRxLevMinOffset.pres;
               sib1->qRxLevMinOffset      = (sib1->qRxLevMinOffsetPres == PRSNT_NODEF) ? 
                  sib1ReCfgGrp->cellSelectInfo.qRxLevMinOffset.val : 0;   

               wrEmmPrintCellSelectInfo(sib1);

               break;
            }
            case LWR_CELL_SIB1_CFG_PMAX:
            {  
               /* Fill SIB1 Power and frequency related parameters */
               rCfgType =  rCfgType ^ mask;

               RLOG_ARG2(L_DEBUG,DBG_CELLID,cellCb->cellId,
                     "SIB1 Modified p-Max changed,old p-Max[%d],new p-Max[%d]",
                     sib1->pMax, sib1ReCfgGrp->pMax);

               sib1->pMax                    = sib1ReCfgGrp->pMax;

               RLOG1(L_DEBUG,"sib1 pMax = [%d]", sib1->pMax);
               break;
            }
            case LWR_CELL_SIB1_CFG_FREQ_BAND_IND:
            {  
               rCfgType =  rCfgType ^ mask; 
               sib1->freqBandIndicator       = sib1ReCfgGrp->freqBandInd;

               RLOG1(L_DEBUG, "SIB1:FreqBandIndicator [%d]",
                  sib1->freqBandIndicator);

               cellReset = TRUE;
               break;
            }   
            case LWR_CELL_SIB1_CFG_TDD_CFG:
            {
               rCfgType =  rCfgType ^ mask; 
               /* TDD configuration  */
               sib1->tddCfg.subframeCfg      = sib1ReCfgGrp->tddCfg.subframeCfg;
               sib1->tddCfg.specialSubframeCfg = sib1ReCfgGrp->tddCfg.specialSubframeCfg;

               wrEmmPrintCellTddCfg(sib1);  
               cellReset = TRUE;
               break;
            }
            case LWR_CELL_SIB1_CFG_SIWIN_SIZE:
            {
               /* LWR enum maps to int values */
               RLOG0(L_DEBUG, "Modification is not allowed SIB1 Si Window Size");
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            default:
            {   
               RLOG1(L_WARNING, "SIB1 reconfiguration received unknown reCfgType [%ld]",
                  (sib1ReCfgGrp->reCfgType));
               rCfgType =  rCfgType ^ mask; 
               break;
            }
         } /* end of switch */
      } /* end of if */
      mask = mask << 1;
   } /* end of while */

   cellCb->sib1.valueTag++;
   cellCb->sib1.valueTag &= WR_EMM_PBM_SIB1_MAX_VALUE_TAG;
   
   /* action for TAC change in cell access information */
   if (trgNrUpd)
   {
      /* Trigger Cfg Upd to all neighbor eNB's */
      ret = wrEmmNghTrigCfgUpd(cellCb);
      if ( ret != ROK )
      {
         RLOG0(L_ERROR, "Failed to trigger eNB configuration update message to its neighbors");
         RETVALUE(RFAILED);
      }
      /* Trigger Cfg Upd to all MME's */
      /* Fix for the CR ccpu00123949 */
      for ( indx = 0; indx < LWR_SM_MAX_MMES; indx++ )
      {
         if(( wrEmmCb.mmeCont.mmes[indx] != NULLP) &&
           ( wrEmmCb.mmeCont.mmes[indx]->state == WR_MME_UP))
         {
            ret = wrEmmMmeSendCfgUpdate(wrEmmCb.mmeCont.mmes[indx]->mmeId);
            if ( ret != ROK )
            {
               RLOG0(L_ERROR, "Failed to send eNodeB configuration update to MME");
               RETVALUE(RFAILED);
            }
         }
      }
   }
   if (cellReset)
   {
   RLOG_ARG0(L_DEBUG,DBG_CELLID,cellCb->cellId,
            "Cell state = RESET, Reason = SIB1 Re-Configuration");
     if (ROK != wrEmmResetCell(cellCb->cellId))
     {
        RLOG0(L_ERROR,"Cell Reset failed for modifying Sib1 parameters");
        RETVALUE(RFAILED);
     }
   }
   RETVALUE(ROK);
} /* end of wrEmmCfgCellModSib1 */ 


/** @brief This function is a utility to copy RACH configuration from LWR to the
 *    cellCb's structure 
 *
 * @details
 *
 *     Function: wrEmmCfgUtlCpyRachCfg
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[in] srcRachCfg : Received Rach Configuration parameters
 * @param[out] dstRachCfg : Rach Control block
 * @return Void
 */
PRIVATE Void wrEmmCfgUtlCpyRachCfg 
(
 LwrRachCfgCommon            *srcRachCfg,
 WrSib2RachCfg               *dstRachCfg
 )
{
   /* Copy RACH related params from Lwr to WR structures */
   dstRachCfg->noRaPreambles     = srcRachCfg->preambleCfg.numOfRAPreambles;
   dstRachCfg->noGrpAPreambles   = srcRachCfg->preambleCfg.groupASize;
   dstRachCfg->grpAMsgSize       = srcRachCfg->preambleCfg.msgSizeGroupA;
   dstRachCfg->grpBPwrOffset     = srcRachCfg->preambleCfg.pwrOffstGrpB;

   dstRachCfg->pwrRampStep = srcRachCfg->powerRamping.powerRampingstep;
   dstRachCfg->prmbInitRcvdTrgtPwr  = srcRachCfg->powerRamping.initRcvdTrgtPwr;

   dstRachCfg->maxPrmbTrans  = srcRachCfg->raSupervisionCfg.preambleTransMax;
   dstRachCfg->raWindowSize  = srcRachCfg->raSupervisionCfg.rarWindowSize;
   dstRachCfg->contResTimer  = srcRachCfg->raSupervisionCfg.macContResTimer;

   dstRachCfg->maxMsg3HarqTrans     = srcRachCfg->maxHARQMsg3;


   RLOG3(L_DEBUG,"noRaPreambles [%d] noGrpAPreambles [%d] grpAMsgSize [%d]",
      dstRachCfg->noRaPreambles, dstRachCfg->noGrpAPreambles,
      dstRachCfg->grpAMsgSize);
   RLOG3(L_DEBUG,"grpBPwrOffset [%d] pwrRampStep [%d] prmbInitRcvdTrgtPwr [%d]",
      dstRachCfg->grpBPwrOffset, dstRachCfg->pwrRampStep,
      dstRachCfg->prmbInitRcvdTrgtPwr);
   RLOG3(L_DEBUG,"maxPrmbTrans [%d] raWindowSize [%d] contResTimer [%d]",
      dstRachCfg->maxPrmbTrans, dstRachCfg->raWindowSize,
      dstRachCfg->contResTimer);
   RLOG1(L_DEBUG,"maxMsg3HarqTrans [%d]",dstRachCfg->maxMsg3HarqTrans);

   RETVOID;
} /* end of wrEmmCfgUtlCpyRachCfg */ 

/** @brief This function to copy PUSCH configuration from LWR to wr structures
 *
 * @details
 *
 *     Function: wrEmmCfgUtlCpyPuschCfg
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[in] srcPuschCfg : PUSCH Configuration parameters
 * @param[out] dstPuschCfg : PUSCH Configuration parameters
 * @return Void
 */
PRIVATE Void wrEmmCfgUtlCpyPuschCfg
(
 LwrPuschCfgCommon            *srcPuschCfg,
 WrSib2PuschCfg               *dstPuschCfg
 )
{

   dstPuschCfg->nSB           = srcPuschCfg->puschBasicCfg.nSb;
   dstPuschCfg->hoppingMode   = srcPuschCfg->puschBasicCfg.hoppingMode;
   dstPuschCfg->hoppingOffset = srcPuschCfg->puschBasicCfg.hoppingOffset;
   dstPuschCfg->qam64Enabled  = srcPuschCfg->puschBasicCfg.enable64QAM;

   dstPuschCfg->grpHoppingEnabled  = 
      srcPuschCfg->puschRefSigCfg.grpHopEnabled;
   dstPuschCfg->grpAssignPusch  = 
      srcPuschCfg->puschRefSigCfg.grpAsgnPusch;
   dstPuschCfg->seqHoppingEnabled  = 
      srcPuschCfg->puschRefSigCfg.seqHopEnabled;
   dstPuschCfg->cyclicShift  = 
      srcPuschCfg->puschRefSigCfg.cyclicShift;

   
   RLOG3(L_DEBUG,"SIB2:PUSCH: nSB [%d] HoppingMode [%d] Qam64Enabled [%d]",
      dstPuschCfg->nSB, dstPuschCfg->hoppingMode, dstPuschCfg->hoppingOffset);
   RLOG3(L_DEBUG,"SIB2:PUSCH: Qam64Enabled [%d] GrpHoppingEnabled [%d] "
      "GrpAssignPusch[%d]",dstPuschCfg->qam64Enabled,
      dstPuschCfg->grpHoppingEnabled, dstPuschCfg->grpAssignPusch);
   RLOG2(L_DEBUG,"SIB2:PUSCH: SeqHoppingEnabled [%d] CyclicShift [%d]",
      dstPuschCfg->seqHoppingEnabled, dstPuschCfg->cyclicShift);
   

   RETVOID;
} /* end of wrEmmCfgUtlCpyPuschCfg */



/** @brief This function adds the SIB group's information to the cellCb
 *
 * @details
 *
 *     Function: wrEmmCfgCellAddSib2
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[out]  cellCb : Cell control block
 * @param[in] sib2CfgGrp : Sib2 configuration parameters
 * @return S16
 *         -# Success : ROK
 *         -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellAddSib2 
(
 WrCellCb                     *cellCb,
 LwrCellSib2CfgGrp            *sib2CfgGrp
 )
{
   S16                       ret = ROK;
   WrSib2Params              *sib2 = &(cellCb->sib2);

   /* Filling in AC barring information */
   sib2->barringInfo.emergency = 
      sib2CfgGrp->acBarringInfo.acBarringForEmergency;

   sib2->barringInfo.signalling.pres = sib2CfgGrp->acBarringInfo.acBarringForMOSignal.pres.val;
   sib2->barringInfo.dataCall.pres = sib2CfgGrp->acBarringInfo.acBarringForMOData.pres.val;

   /* Access Class Barring Info for MO Signalling */
   sib2->barringInfo.signalling.acBarringFactor = 
      sib2CfgGrp->acBarringInfo.acBarringForMOSignal.acBarringFactor;
   sib2->barringInfo.signalling.acBarringTime = 
      sib2CfgGrp->acBarringInfo.acBarringForMOSignal.acBarringTime;
   sib2->barringInfo.signalling.acBarringForSpecialAC = 
      sib2CfgGrp->acBarringInfo.acBarringForMOSignal.acBarringAC11;

   sib2->barringInfo.signalling.acBarringForSpecialAC =
      (((sib2->barringInfo.signalling.acBarringForSpecialAC) << 1) |
       (sib2CfgGrp->acBarringInfo.acBarringForMOSignal.acBarringAC12));
   sib2->barringInfo.signalling.acBarringForSpecialAC =
      (((sib2->barringInfo.signalling.acBarringForSpecialAC) << 1) |
       (sib2CfgGrp->acBarringInfo.acBarringForMOSignal.acBarringAC13));
   sib2->barringInfo.signalling.acBarringForSpecialAC =
      (((sib2->barringInfo.signalling.acBarringForSpecialAC) << 1) |
       (sib2CfgGrp->acBarringInfo.acBarringForMOSignal.acBarringAC14));
   sib2->barringInfo.signalling.acBarringForSpecialAC =
      (((sib2->barringInfo.signalling.acBarringForSpecialAC) << 1) |
       (sib2CfgGrp->acBarringInfo.acBarringForMOSignal.acBarringAC15));

   sib2->barringInfo.signalling.acBarringForSpecialAC = 
      (sib2->barringInfo.signalling.acBarringForSpecialAC << 3);
   /* Storing the Access Barring Factor and Time  for MO Data in cellCb as 
    * configured by OAM. This configuration shall be retained in cellCb, even
    * after RRM overwrites the SIB 2 information for PBM module.
    */

   sib2->barringInfo.signalling.lwrCfgAcbFactor =
      sib2CfgGrp->acBarringInfo.acBarringForMOSignal.acBarringFactor;
   sib2->barringInfo.signalling.lwrCfgAcbTime =
      sib2CfgGrp->acBarringInfo.acBarringForMOSignal.acBarringTime;
   sib2->barringInfo.signalling.isRrmCfgPres = FALSE;
   
   /* Access Class Barring Info for Data */
   sib2->barringInfo.dataCall.acBarringFactor = 
      sib2CfgGrp->acBarringInfo.acBarringForMOData.acBarringFactor;
   sib2->barringInfo.dataCall.acBarringTime = 
      sib2CfgGrp->acBarringInfo.acBarringForMOData.acBarringTime;
   sib2->barringInfo.dataCall.acBarringForSpecialAC = 
      sib2CfgGrp->acBarringInfo.acBarringForMOData.acBarringAC11;

   sib2->barringInfo.dataCall.acBarringForSpecialAC =
      (((sib2->barringInfo.dataCall.acBarringForSpecialAC) << 1) |
       (sib2CfgGrp->acBarringInfo.acBarringForMOData.acBarringAC12));
   sib2->barringInfo.dataCall.acBarringForSpecialAC =
      (((sib2->barringInfo.dataCall.acBarringForSpecialAC) << 1) |
       (sib2CfgGrp->acBarringInfo.acBarringForMOData.acBarringAC13));
   sib2->barringInfo.dataCall.acBarringForSpecialAC =
      (((sib2->barringInfo.dataCall.acBarringForSpecialAC) << 1) |
       (sib2CfgGrp->acBarringInfo.acBarringForMOData.acBarringAC14));
   sib2->barringInfo.dataCall.acBarringForSpecialAC =
      (((sib2->barringInfo.dataCall.acBarringForSpecialAC) << 1) |
       (sib2CfgGrp->acBarringInfo.acBarringForMOData.acBarringAC15));
   
   sib2->barringInfo.dataCall.acBarringForSpecialAC = 
      (sib2->barringInfo.dataCall.acBarringForSpecialAC << 3);

   /* Storing the Access Barring Factor and Time  for MO Signalling in cellCb  
    * configured by OAM. This configuration shall be retained in cellCb, even
    * after RRM overwrites the SIB 2 information for PBM module.
    */
   sib2->barringInfo.dataCall.lwrCfgAcbFactor =
      sib2CfgGrp->acBarringInfo.acBarringForMOData.acBarringFactor;
   sib2->barringInfo.dataCall.lwrCfgAcbTime =
      sib2CfgGrp->acBarringInfo.acBarringForMOData.acBarringTime;
   sib2->barringInfo.dataCall.isRrmCfgPres = FALSE;
 
   if(sib2->barringInfo.emergency || 
      sib2->barringInfo.signalling.pres ||
      sib2->barringInfo.dataCall.pres)
   {
      RLOG3(L_WARNING, "Emergency:[%d] or Signalling:[%d]"
         "or Data Call:[%d] Is Barred", sib2->barringInfo.emergency,
         sib2->barringInfo.signalling.pres, sib2->barringInfo.dataCall.pres);
      sib2->barringInfoPres = TRUE;
   }
   else
   {
      sib2->barringInfoPres = FALSE;
   }
   
   /* Filling in RACH information  */
   wrEmmCfgUtlCpyRachCfg (&(sib2CfgGrp->radioResourceCommon.rachCfgCommon), 
         &(sib2->rachCfg));

   /* Filling BCCH information */
   sib2->bcchCfg.modifPeriodCoeff = sib2CfgGrp->radioResourceCommon.
      bcchCfgCommon.modPeriodCoeff;

   /* Filling PCCH information */
   sib2->pcchCfg.defaultPagingCycle = sib2CfgGrp->radioResourceCommon.
      pcchCfgCommon.defaultPagingCycle;
   sib2->pdschCfg.refSigPower  = 
      sib2CfgGrp->radioResourceCommon.pdschCfgCommon.referenceSignalPwr;
   sib2->pcchCfg.nB            = sib2CfgGrp->
      radioResourceCommon.pcchCfgCommon.nB;
   
   /* Filling PRACH information */
   sib2->prachCfg.rootSeqIdx     = sib2CfgGrp->radioResourceCommon.
      prachCfgCommon.rootSequenceIndx;
   sib2->prachCfg.prachCfgIdx    = sib2CfgGrp->radioResourceCommon.
      prachCfgCommon.prachCfgInfo.prachCfgIdx;
   sib2->prachCfg.highSpeedFlag  = sib2CfgGrp->radioResourceCommon.
      prachCfgCommon.prachCfgInfo.highSpeedFlag;
   sib2->prachCfg.zeroCorrZone= 
      sib2CfgGrp->radioResourceCommon.prachCfgCommon.prachCfgInfo.
      zeroCorrelationZoneCfg;

   sib2->prachCfg.prachFreqOffset = sib2CfgGrp->radioResourceCommon.
      prachCfgCommon.prachCfgInfo.prachFreqOffset;

   /* Filling PDSCH information */
   sib2->pdschCfg.refSigPower    = sib2CfgGrp->radioResourceCommon.
      pdschCfgCommon.referenceSignalPwr;
   sib2->pdschCfg.pB = sib2CfgGrp->radioResourceCommon.pdschCfgCommon.pB;

   /* Filling PUSCH configuration */
   wrEmmCfgUtlCpyPuschCfg (&(sib2CfgGrp->radioResourceCommon.puschCfgCommon), 
         &(sib2->puschCfg));

   /* Filling PUCCH Configuration */
   sib2->pucchCfg.deltaPucchShift   = 
      sib2CfgGrp->radioResourceCommon.pucchCfgCommon.deltaShift;
   sib2->pucchCfg.nRBCqi   = 
      sib2CfgGrp->radioResourceCommon.pucchCfgCommon.nRBCqi;
   sib2->pucchCfg.nCSAN   = 
      sib2CfgGrp->radioResourceCommon.pucchCfgCommon.nCSAN;
   sib2->pucchCfg.n1PucchAn   = 
      sib2CfgGrp->radioResourceCommon.pucchCfgCommon.n1PucchAn;

   /* Filling SRS configuration */
   if(wrSmEnableSrs == TRUE)
   {
      sib2->srsCfg.release = FALSE; /* Enabled;*/
   }
   else
   {
      sib2->srsCfg.release = TRUE; /* marking SRS as off by default */
   }
   RLOG2(L_INFO, "wrSmEnableSrs is set to [%d]and sib2->srsCfg.release[%d]", wrSmEnableSrs, sib2->srsCfg.release);
   sib2->srsCfg.srsBandwidthCfg     = 
      sib2CfgGrp->radioResourceCommon.srsCfgCommon.cSrs; 
   sib2->srsCfg.srsSubfrmCfg        = 
      sib2CfgGrp->radioResourceCommon.srsCfgCommon.subframeCfg; 
   sib2->srsCfg.ackNackSrsSimulTx   = 
      sib2CfgGrp->radioResourceCommon.srsCfgCommon.simulAckNackSrs; 

   /* Filling up UL power control params */
   sib2->ulPwrCtrlCfg.p0NominalPusch      = 
      sib2CfgGrp->radioResourceCommon.ulPwrCtrlCommon.pNomialPusch;
   sib2->ulPwrCtrlCfg.alpha               =
      sib2CfgGrp->radioResourceCommon.ulPwrCtrlCommon.alpha;
   sib2->ulPwrCtrlCfg.p0NominlPucch       = 
      sib2CfgGrp->radioResourceCommon.ulPwrCtrlCommon.pNomialPucch;
   sib2->ulPwrCtrlCfg.deltaPeambleMsg3 = 
      sib2CfgGrp->radioResourceCommon.ulPwrCtrlCommon.deltaMsg3;

   sib2->ulPwrCtrlCfg.deltaFPucchFormat1  = 
      sib2CfgGrp->radioResourceCommon.ulPwrCtrlCommon.deltaFLst.
      deltaPucchFormat1;
   sib2->ulPwrCtrlCfg.deltaFPucchFormat1b = 
      sib2CfgGrp->radioResourceCommon.ulPwrCtrlCommon.deltaFLst.
      deltaPucchFormat1b;
   sib2->ulPwrCtrlCfg.deltaFPucchFormat2  = 
      sib2CfgGrp->radioResourceCommon.ulPwrCtrlCommon.deltaFLst.
      deltaPucchFormat2;
   sib2->ulPwrCtrlCfg.deltaFPucchFormat2a = 
      sib2CfgGrp->radioResourceCommon.ulPwrCtrlCommon.deltaFLst.
      deltaPucchFormat2a;
   sib2->ulPwrCtrlCfg.deltaFPucchFormat2b = 
      sib2CfgGrp->radioResourceCommon.ulPwrCtrlCommon.
      deltaFLst.deltaPucchFormat2b;
   /* Normal = 1, extended = 2 */
   sib2->ulCyclicPrefix    = sib2CfgGrp->radioResourceCommon.ulCyclicPrefix;   


   /* Timers and constants */
   sib2->t300    = sib2CfgGrp->ueTimerConstants.t300;  
   sib2->t301    = sib2CfgGrp->ueTimerConstants.t301;  
   sib2->t310    = sib2CfgGrp->ueTimerConstants.t310;  
   sib2->n310    = sib2CfgGrp->ueTimerConstants.n310;  
   sib2->t311    = sib2CfgGrp->ueTimerConstants.t311;  
   sib2->n311    = sib2CfgGrp->ueTimerConstants.n311;  
   cellCb->ueTmrConstants.t302 = sib2CfgGrp->ueTimerConstants.t302;
   cellCb->ueTmrConstants.t304 = sib2CfgGrp->ueTimerConstants.t304;
   cellCb->ueTmrConstants.ccoT304 = sib2CfgGrp->ueTimerConstants.ccoT304;
   cellCb->ueTmrConstants.t320 = sib2CfgGrp->ueTimerConstants.t320;

   /* UL Frequency information */
   sib2->ulCarrierFreq        = sib2CfgGrp->ulFreqInfo.eARFCN;
   sib2->ulBw                 = sib2CfgGrp->ulFreqInfo.ulBandwidth;
   sib2->addlSpectrEmission   = sib2CfgGrp->ulFreqInfo.addSpectEmission;
   sib2->timingAlignTimer     = sib2CfgGrp->timeAlignmentCommon;

   RETVALUE(ret);
} /* end of wrEmmCfgCellAddSib2 */

PRIVATE Void wrEmmCfgUtlCpyAcBarringCfg
(
WrSib2AcBarringCfg        *dstAcBarringCfg,
LwrAcBarringCfg           *srcAcBarringCfg
)
{
   if (srcAcBarringCfg->pres.val)
   {
      dstAcBarringCfg->acBarringForSpecialAC = srcAcBarringCfg->acBarringAC11;

      dstAcBarringCfg->acBarringForSpecialAC =
         (((dstAcBarringCfg->acBarringForSpecialAC) << 1) | 
          (srcAcBarringCfg->acBarringAC12));
      dstAcBarringCfg->acBarringForSpecialAC =
         (((dstAcBarringCfg->acBarringForSpecialAC) << 1) | 
          (srcAcBarringCfg->acBarringAC13));
      dstAcBarringCfg->acBarringForSpecialAC =
         (((dstAcBarringCfg->acBarringForSpecialAC) << 1) | 
          (srcAcBarringCfg->acBarringAC14));
      dstAcBarringCfg->acBarringForSpecialAC =
         (((dstAcBarringCfg->acBarringForSpecialAC) << 1) | 
          (srcAcBarringCfg->acBarringAC15));

      dstAcBarringCfg->acBarringForSpecialAC =
         (dstAcBarringCfg->acBarringForSpecialAC << 3);

      if (dstAcBarringCfg->acBarringForSpecialAC)
      {
         dstAcBarringCfg->pres = srcAcBarringCfg->pres.val;
         
         if (dstAcBarringCfg->isRrmCfgPres == FALSE)
         {
            dstAcBarringCfg->acBarringFactor = srcAcBarringCfg->acBarringFactor;
            dstAcBarringCfg->acBarringTime = srcAcBarringCfg->acBarringTime;
         }
      }

      /* Create a copy os OAM configuration in cell Cb for future purpose 
       */
      dstAcBarringCfg->lwrCfgAcbFactor = srcAcBarringCfg->acBarringFactor;
      dstAcBarringCfg->lwrCfgAcbTime = srcAcBarringCfg->acBarringTime;
   }

   
   RLOG1(L_DEBUG,"SIB2:AcBarringCfgPres [%d]",dstAcBarringCfg->pres);
   RLOG3(L_DEBUG,"SIB2:AcBarringFactor [%d] SIB2:AcBarringTime [%d]"
      " SIB2:AcBarringForSpecialAC [%d]",dstAcBarringCfg->acBarringFactor,
      dstAcBarringCfg->acBarringTime,dstAcBarringCfg->acBarringForSpecialAC);
  
} /* end of wrEmmCfgUtlCpyAcBarringCfg */

PRIVATE Void wrEmmCfgUtlCpyBarringCfg
(
WrSib2Params              *sib2,
LwrAcBarringInfo          *acBarringInfo
)
{
   /* Updating in AC barring information */
   if (acBarringInfo->acBarringForEmergency ||
         acBarringInfo->acBarringForMOSignal.pres.val ||
         acBarringInfo->acBarringForMOData.pres.val)
   {
      sib2->barringInfoPres = TRUE;
   } 
   else
   {
      sib2->barringInfoPres = FALSE;
   }
      
   sib2->barringInfo.emergency = acBarringInfo->acBarringForEmergency;

   RLOG2(L_DEBUG,"SIB2:BarringInfoPres[%d] BarringInfo.Emergency[%d]"
      ,sib2->barringInfoPres,sib2->barringInfo.emergency);

   RLOG0(L_DEBUG,"SIB2:BarringInfo:MOSignal");

   wrEmmCfgUtlCpyAcBarringCfg(&sib2->barringInfo.signalling, 
                              &acBarringInfo->acBarringForMOSignal);
   RLOG0(L_DEBUG,"SIB2:BarringInfo:DataCall");
   

   wrEmmCfgUtlCpyAcBarringCfg(&sib2->barringInfo.dataCall, 
                              &acBarringInfo->acBarringForMOData);
} /* end of wrEmmCfgUtlCpyPuschCfg */

PRIVATE Void wrEmmCfgUtlCpyRadioResourceCfg
(
WrCellCb                  *cellCb,
LwrRadioResourceCommon    *radioResourceCommon,
Bool                      *trgCfgUpd,
Bool                      *cellReset
)
{
   U32                       rCfgType = 0;
   U32                       mask = 1;
   RgrCellRecfg              *reCfg = &(cellCb->pendRecfg.schReCfg);
   WrSib2Params              *sib2  = &(cellCb->sib2);

   rCfgType =  radioResourceCommon->reCfgType;

   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_CELL_SIB2_RRSRCMN_RACH_CFG_CMN:
            {
               rCfgType =  rCfgType ^ mask; 
               /* Filling in RACH information  */
               wrEmmCfgUtlCpyRachCfg (&(radioResourceCommon->rachCfgCommon), 
                                      &(sib2->rachCfg));
#ifdef WR_RECFG
               cellCb->pendRecfg.cellReCfgBitMsk |= WR_CELL_SCHED_RECFG_PEND; 
               reCfg->recfgTypes |= RGR_CELL_RACH_RECFG;
               cellCb->pendReCfgPres = TRUE;
               wrEmmCellSchdRachCfg(&reCfg->rachRecfg, cellCb);
#endif
               break;
            }
            case LWR_CELL_SIB2_RRSRCMN_BCCH_CFG_CMN:
            {
               rCfgType =  rCfgType ^ mask; 
               /* Filling BCCH information */
               cellCb->pbmCb.pendModifPeriodCoeff = radioResourceCommon->bcchCfgCommon.modPeriodCoeff;
               RLOG1(L_DEBUG,"SIB2:pendModifPeriodCoeff [%d]",
                           cellCb->pbmCb.pendModifPeriodCoeff);

               cellCb->pbmCb.pendReCalcAllPfsAndPos = TRUE;
               break;
            }
            case LWR_CELL_SIB2_RRSRCMN_PCCH_CFG_CMN:
            {
               rCfgType =  rCfgType ^ mask; 
               /* Filling PCCH information */
               cellCb->pbmCb.pendDfltPagingCycle =   
                         radioResourceCommon->pcchCfgCommon.defaultPagingCycle;
               cellCb->pbmCb.pendnB = radioResourceCommon->pcchCfgCommon.nB;

               RLOG1(L_DEBUG,"SIB2:PendDfltPagingCycle [%d]",
               cellCb->pbmCb.pendDfltPagingCycle);

               RLOG1(L_DEBUG,"SIB2:PendnB [%d]",
                           cellCb->pbmCb.pendnB);

               cellCb->pbmCb.pendReCalcAllPfsAndPos = TRUE;
               *trgCfgUpd = TRUE;
               break;
            }
            case LWR_CELL_SIB2_RRSRCMN_PRACH_CFG_CMN:
            {
               rCfgType =  rCfgType ^ mask; 
               /* Filling PRACH information */
               sib2->prachCfg.rootSeqIdx     = 
                  radioResourceCommon->prachCfgCommon.rootSequenceIndx;
               sib2->prachCfg.prachCfgIdx    = 
                  radioResourceCommon->prachCfgCommon.prachCfgInfo.prachCfgIdx;
               sib2->prachCfg.highSpeedFlag  = 
                  radioResourceCommon->prachCfgCommon.prachCfgInfo.highSpeedFlag;
               sib2->prachCfg.zeroCorrZone= 
                  radioResourceCommon->prachCfgCommon.prachCfgInfo.zeroCorrelationZoneCfg;
               sib2->prachCfg.prachFreqOffset = 
                  radioResourceCommon->prachCfgCommon.prachCfgInfo.prachFreqOffset;

               wrEmmPrintPrachCfg(sib2);

            
               *cellReset = TRUE;
               break;
            }
            case LWR_CELL_SIB2_RRSRCMN_PDSCH_CFG_CMN:
            {
               rCfgType =  rCfgType ^ mask; 
               /* Filling PDSCH information */
               sib2->pdschCfg.refSigPower    = 
                    radioResourceCommon->pdschCfgCommon.referenceSignalPwr;
               sib2->pdschCfg.pB = radioResourceCommon->pdschCfgCommon.pB;

               RLOG2(L_DEBUG,"Sib2:pdschCfg.refSigPower [%d] pdschCfg.pB [%d]"
                  ,sib2->pdschCfg.refSigPower,sib2->pdschCfg.pB);

               break;
            }
            case LWR_CELL_SIB2_RRSRCMN_PUSCH_CFG_CMN:
            {
               rCfgType =  rCfgType ^ mask; 
               /* Filling PUSCH configuration */
                if ((sib2->puschCfg.hoppingOffset != 
                     radioResourceCommon->puschCfgCommon.puschBasicCfg.hoppingOffset) ||
                    (sib2->puschCfg.cyclicShift != 
                     radioResourceCommon->puschCfgCommon.puschRefSigCfg.cyclicShift))
                {
                   *cellReset = TRUE;
                }
                wrEmmCfgUtlCpyPuschCfg (&(radioResourceCommon->puschCfgCommon), 
                                        &(sib2->puschCfg));
               break;
            }
            case LWR_CELL_SIB2_RRSRCMN_PUCCH_CFG_CMN:
            {
               rCfgType =  rCfgType ^ mask; 
               /* Filling PUCCH Configuration */
               sib2->pucchCfg.deltaPucchShift   = 
                   radioResourceCommon->pucchCfgCommon.deltaShift;
               sib2->pucchCfg.nRBCqi   =  radioResourceCommon->pucchCfgCommon.nRBCqi;
               sib2->pucchCfg.nCSAN   = radioResourceCommon->pucchCfgCommon.nCSAN;
               sib2->pucchCfg.n1PucchAn   = radioResourceCommon->pucchCfgCommon.n1PucchAn;
               
               RLOG1(L_DEBUG,"SIB2:PUCCH:DeltaPucchShift [%d]"
                  ,sib2->pucchCfg.deltaPucchShift);
               RLOG3(L_DEBUG,"SIB2:PUCCH:nRBCqi [%d] PUCCH:nCSAN [%d] PUCCH:n1PucchAn[%d]"
                  ,sib2->pucchCfg.nRBCqi,sib2->pucchCfg.nCSAN, sib2->pucchCfg.n1PucchAn);

               *cellReset = TRUE;
               break;
            }
            case LWR_CELL_SIB2_RRSRCMN_SRS_CFG_CMN:
            {
               rCfgType =  rCfgType ^ mask; 
               /* Filling SRS configuration */
               if(wrSmEnableSrs == TRUE)
               {              
                  sib2->srsCfg.release = FALSE; /* Enabled;*/
               }              
               else           
               {
                  sib2->srsCfg.release = TRUE; /* marking SRS as off by default */
               }
               sib2->srsCfg.srsBandwidthCfg     = 
                      radioResourceCommon->srsCfgCommon.cSrs; 
               sib2->srsCfg.srsSubfrmCfg        = 
                     radioResourceCommon->srsCfgCommon.subframeCfg; 
               sib2->srsCfg.ackNackSrsSimulTx   = 
                     radioResourceCommon->srsCfgCommon.simulAckNackSrs;
         
               RLOG3(L_DEBUG,"SIB2:SRS:SrsBandwidthCfg [%d] SrsSubfrmCfg [%d]"
                  " AckNackSrsSimulTx [%d]",sib2->srsCfg.srsBandwidthCfg,
                  sib2->srsCfg.srsSubfrmCfg,sib2->srsCfg.ackNackSrsSimulTx);

               break;
            }
            case LWR_CELL_SIB2_RRSRCMN_UL_PWR_CNRL_CFG_CMN:
            {
               rCfgType =  rCfgType ^ mask; 
               /* Filling up UL power control params */

               RLOG_ARG2(L_DEBUG,DBG_CELLID,cellCb->cellId,
                  "SIB2 Modified p0-NominalPUCCH changed, old p0-NominalPUCCH"
                  "= [%d], new p0-NominalPUCCH = [%d]",
                  sib2->ulPwrCtrlCfg.p0NominalPusch,
                  radioResourceCommon->ulPwrCtrlCommon.pNomialPusch);
               RLOG_ARG2(L_DEBUG,DBG_CELLID,cellCb->cellId,
                  "SIB2 Modified p0-NominalPUSCH changed, old "
                  "p0-NominalPUSCH = [%d], new p0-NominalPUSCH = [%d]",
                  sib2->ulPwrCtrlCfg.p0NominlPucch,
                  radioResourceCommon->ulPwrCtrlCommon.pNomialPucch);
               RLOG_ARG2(L_DEBUG,DBG_CELLID,cellCb->cellId,
                  "SIB2 Modified alpha changed,old alpha=[%d],new alpha=[%d]",
                  sib2->ulPwrCtrlCfg.alpha,radioResourceCommon->ulPwrCtrlCommon.alpha);

               sib2->ulPwrCtrlCfg.p0NominalPusch      = 
                      radioResourceCommon->ulPwrCtrlCommon.pNomialPusch;
               sib2->ulPwrCtrlCfg.alpha = radioResourceCommon->ulPwrCtrlCommon.alpha;
               sib2->ulPwrCtrlCfg.p0NominlPucch       = 
                                   radioResourceCommon->ulPwrCtrlCommon.pNomialPucch;
               sib2->ulPwrCtrlCfg.deltaPeambleMsg3 = 
                                   radioResourceCommon->ulPwrCtrlCommon.deltaMsg3;

               sib2->ulPwrCtrlCfg.deltaFPucchFormat1  = 
                     radioResourceCommon->ulPwrCtrlCommon.deltaFLst.deltaPucchFormat1;
               sib2->ulPwrCtrlCfg.deltaFPucchFormat1b = 
                     radioResourceCommon->ulPwrCtrlCommon.deltaFLst.deltaPucchFormat1b;
               sib2->ulPwrCtrlCfg.deltaFPucchFormat2  = 
                       radioResourceCommon->ulPwrCtrlCommon.deltaFLst.deltaPucchFormat2;
               sib2->ulPwrCtrlCfg.deltaFPucchFormat2a = 
                      radioResourceCommon->ulPwrCtrlCommon.deltaFLst.deltaPucchFormat2a;
               sib2->ulPwrCtrlCfg.deltaFPucchFormat2b = 
                      radioResourceCommon->ulPwrCtrlCommon.deltaFLst.deltaPucchFormat2b;
               wrEmmPrintUlPwrCtrlCfg(sib2);

               break;
            }
            case LWR_CELL_SIB2_RRSRCMN_UL_CYCPREFIX_CFG_CMN:
            {
               rCfgType =  rCfgType ^ mask; 
               /* Normal = 1, extended = 2 */
               sib2->ulCyclicPrefix    = radioResourceCommon->ulCyclicPrefix;

               RLOG1(L_DEBUG,"SIB2:UlCyclicprefix [%d]",sib2->ulCyclicPrefix);

               *cellReset = TRUE;
               break;
            }
            case LWR_CELL_SIB2_RRSRCMN_DL_CYCPREFIX_CFG_CMN:
            {
               rCfgType =  rCfgType ^ mask; 
               /* Normal = 1, extended = 2 */
               cellCb->dlCyclicprefix    = radioResourceCommon->dlCyclicPrefix;
               
               RLOG1(L_DEBUG,"SIB2:DlCyclicprefix [%d]",cellCb->dlCyclicprefix);

               *cellReset = TRUE;
               break;
            }
            default:
            {   
               RLOG1(L_WARNING, "Radio Resource reconfiguration received with unknown"
                     "reCfgType[%ld]",(radioResourceCommon->reCfgType));
               rCfgType =  rCfgType ^ mask; 
               break;
            }
         } /* end of switch */
      }
      mask = mask << 1;
   } /* end of while */
} /* end of wrEmmCfgUtlCpyRadioResourceCfg */


PRIVATE Void wrEmmCfgCellModSib2PrintTimerAndConst
(
 WrCellCb                     *cellCb,
 WrSib2Params              *sib2
)
{
   RLOG3(L_DEBUG,"sib2->t300[%d] sib2->t301[%d] sib2->t310[%d]",
      sib2->t300,sib2->t301,sib2->t310);
   RLOG3(L_DEBUG,"sib2->n310[%d] sib2->n311[%d] sib2->t311[%d]",
      sib2->n310,sib2->n311,sib2->t311);
   RLOG3(L_DEBUG,"cellCb->ueTmrConstants.t302[%d] ueTmrConstants.t304[%d]"
      "cellCb->ueTmrConstants.t320[%d]"
      ,cellCb->ueTmrConstants.t302,cellCb->ueTmrConstants.t304,
      cellCb->ueTmrConstants.t320);
   RLOG1(L_DEBUG,"cellCb->ueTmrConstants.ccoT304[%d]",
      cellCb->ueTmrConstants.ccoT304);
}

/** @brief This function modify the SIB2 group's information to the cellCb
 *
 * @details
 *
 *     Function: wrEmmCfgCellModSib2
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[out]  cellCb : Cell control block
 * @param[in] sib2CfgGrp : Sib2 configuration parameters
 * @return S16
 *         -# Success : ROK
 *         -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellModSib2 
(
WrCellCb                     *cellCb,
LwrCellSib2CfgGrp            *sib2ReCfgGrp
)
{
   WrSib2Params              *sib2            = &(cellCb->sib2);
   S16                       ret              = ROK;
   S16                       indx             = 0;
   U32                       rCfgType         = 0;
   U32                       bkRcfgType       = 0;
   Bool                      cellReset        = FALSE;
   Bool                      trgCfgUpd        = FALSE;
   U32                       mask             = 1;
   Bool                      pbmRecfg         = 0;
   Bool                      isSib2TimMod     = FALSE;
   Bool                      nonSib3TimMod    = FALSE;
#ifndef RM_INTF
   RrmWrCellCfgReqInfo       *rrmWrCellCfgReq = NULLP;
  
   WR_ALLOC (&rrmWrCellCfgReq, sizeof(RrmWrCellCfgReqInfo));
   if (rrmWrCellCfgReq == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed during sib2 modification.");
      RETVALUE(RFAILED);
   }
   
   rrmWrCellCfgReq->reCfgType = 0;
#endif /*RM_INTF*/

   rCfgType =  sib2ReCfgGrp->reCfgType;
   bkRcfgType = rCfgType;

   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_CELL_SIB2_CFG_BARRING_INFO:
               {
                  rCfgType =  rCfgType ^ mask; 

                  wrEmmCfgUtlCpyBarringCfg(sib2, 
                        &sib2ReCfgGrp->acBarringInfo);

                  pbmRecfg = TRUE;
                  break;
               }
            case LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN:
               {
                  rCfgType =  rCfgType ^ mask; 

                  wrEmmCfgUtlCpyRadioResourceCfg(cellCb, 
                        &sib2ReCfgGrp->radioResourceCommon,
                        &trgCfgUpd, &cellReset);

                  break;
               }
            case LWR_CELL_SIB2_CFG_UE_TMR_CONS:
            {
               /* Timers and constants */
               rCfgType =  rCfgType ^ mask; 
               if((sib2->t300 != sib2ReCfgGrp->ueTimerConstants.t300)||
                  (sib2->t301  != sib2ReCfgGrp->ueTimerConstants.t301)||  
                  (sib2->t310  != sib2ReCfgGrp->ueTimerConstants.t310)||  
                  (sib2->n310  != sib2ReCfgGrp->ueTimerConstants.n310)||  
                  (sib2->t311  != sib2ReCfgGrp->ueTimerConstants.t311)||  
                  (sib2->n311  != sib2ReCfgGrp->ueTimerConstants.n311))
               {
                  sib2->t300    = sib2ReCfgGrp->ueTimerConstants.t300;  
                  sib2->t301    = sib2ReCfgGrp->ueTimerConstants.t301;  
                  sib2->t310    = sib2ReCfgGrp->ueTimerConstants.t310;  
                  sib2->n310    = sib2ReCfgGrp->ueTimerConstants.n310;  
                  sib2->t311    = sib2ReCfgGrp->ueTimerConstants.t311;  
                  sib2->n311    = sib2ReCfgGrp->ueTimerConstants.n311;
                  RLOG0(L_DEBUG, "SIB2 Timers Modified");
                  isSib2TimMod = TRUE;
               }  
               if((cellCb->ueTmrConstants.t302 != sib2ReCfgGrp->ueTimerConstants.t302) ||
                  (cellCb->ueTmrConstants.t304 != sib2ReCfgGrp->ueTimerConstants.t304) ||
                  (cellCb->ueTmrConstants.ccoT304 != sib2ReCfgGrp->ueTimerConstants.ccoT304)||
                  (cellCb->ueTmrConstants.t320 != sib2ReCfgGrp->ueTimerConstants.t320))
               {
                  cellCb->ueTmrConstants.t302 = sib2ReCfgGrp->ueTimerConstants.t302;
                  cellCb->ueTmrConstants.t304 = sib2ReCfgGrp->ueTimerConstants.t304;
                  cellCb->ueTmrConstants.ccoT304 = sib2ReCfgGrp->ueTimerConstants.ccoT304;
                  cellCb->ueTmrConstants.t320 = sib2ReCfgGrp->ueTimerConstants.t320;
                  nonSib3TimMod = TRUE;
                  RLOG0(L_DEBUG, "RRC Timers Modified");
               }
               wrEmmCfgCellModSib2PrintTimerAndConst(cellCb, sib2);

               break;
            }
            case  LWR_CELL_SIB2_CFG_UE_FREQ_INFO:
            {
               rCfgType =  rCfgType ^ mask; 
               /* UL Frequency information */
               RLOG_ARG2(L_INFO, DBG_CELLID,cellCb->cellId,"SIB2 Modified" 
                  "EARFCN changed,old EARFCN[%d] new EARFCN[%d]",
                  sib2->ulCarrierFreq, sib2ReCfgGrp->ulFreqInfo.eARFCN);

               sib2->ulCarrierFreq        = sib2ReCfgGrp->ulFreqInfo.eARFCN;
               if (( sib2->ulBw !=  sib2ReCfgGrp->ulFreqInfo.ulBandwidth) || 
              (sib2->addlSpectrEmission != sib2ReCfgGrp->ulFreqInfo.addSpectEmission))
               {
                    sib2->ulBw                 = sib2ReCfgGrp->ulFreqInfo.ulBandwidth;
                    sib2->addlSpectrEmission   = sib2ReCfgGrp->ulFreqInfo.addSpectEmission;
          cellReset = TRUE;
          }

               RLOG1(L_DEBUG, "SIB2:UlCarrierFreq [%d]",sib2->ulCarrierFreq);
               RLOG1(L_DEBUG, "SIB2:UlBw [%d]",sib2->ulBw);
               RLOG1(L_DEBUG, "SIB2:AddlSpectrEmission [%d]",sib2->addlSpectrEmission);

               break;
            }
            case LWR_CELL_SIB2_CFG_TIME_ALIGN:
            {
               rCfgType =  rCfgType ^ mask; 
               sib2->timingAlignTimer     = sib2ReCfgGrp->timeAlignmentCommon;

               RLOG1(L_DEBUG, "SIB2:TimingAlignTimer[%d]",sib2->timingAlignTimer);
            
               break;
            }
            case LWR_CELL_SIB2_CFG_BARRING_VOICE:
            {
               rCfgType =  rCfgType ^ mask;
            
               RLOG0(L_INFO,"SIB2:VoiceBarringInfo");

               wrEmmCfgUtlCpyAcBarringCfg(&sib2->voiceBarringInfo, &sib2ReCfgGrp->acBarringVoice);
               break;
            }
            case LWR_CELL_SIB2_CFG_BARRING_VIDEO:
            {
               rCfgType =  rCfgType ^ mask; 

               RLOG0(L_INFO,"SIB2:VideoBarringInfo");

               wrEmmCfgUtlCpyAcBarringCfg(&sib2->videoBarringInfo, &sib2ReCfgGrp->acBarringVideo);
               break;
            }
            default:
            {   
               RLOG1(L_WARNING, "SIB2 Re-config received unknown reCfgType [%ld]",
                     sib2ReCfgGrp->reCfgType);
               rCfgType =  rCfgType ^ mask; 
               break;
            }
         } /* end of switch */
      }
      mask = mask << 1;
   } /* end of while */

#ifndef RM_INTF
   if (rrmWrCellCfgReq->reCfgType)
   {
      /* If some parameter has been updated, then add the cell id to the
       * configuration and send the update to RRM
       */
      rrmWrCellCfgReq->cellId = cellCb->cellId;

      /* 12792 - Added addModOrDel */
      rrmWrCellCfgReq->addModOrDel = RRM_WR_CFG_MOD;
      
      if (ROK != wrIfmRrmCellCfgReq(rrmWrCellCfgReq))
      {
         RLOG0(L_ERROR,"Failed to send cell configuration to RRM");
         RETVALUE(RFAILED);
      }

      RLOG_ARG1(L_DEBUG, DBG_CELLID,rrmWrCellCfgReq->cellId,
            " Cell reconfiguration succesfully sent to RRM "
            "reCfgType = [%d]",
            rrmWrCellCfgReq->reCfgType);
   }
#endif /*RM_INTF*/

   if ((FALSE == cellReset) && (TRUE == trgCfgUpd))
   {
      /* Trigger Cfg Upd to all MME's */
      for ( indx = 0; indx < LWR_SM_MAX_MMES; indx++ )
      {
         if( wrEmmCb.mmeCont.mmes[indx]->state == WR_MME_UP)
         {
            ret = wrEmmMmeSendCfgUpdate(wrEmmCb.mmeCont.mmes[indx]->mmeId);
            if ( ret != ROK )
            {
               RLOG0(L_ERROR,"Failed to send eNodeB configuration update to MME");
               RETVALUE(RFAILED);
            }
         }
      }
   } 
   if (TRUE == cellReset)
   {
      RLOG_ARG0(L_DEBUG,DBG_CELLID,cellCb->cellId,
            "Cell state = RESET, Reason = SIB2 Re-Configuration");
     if (ROK != wrEmmResetCell(cellCb->cellId))
     {
        RLOG0(L_ERROR, "Cell reset failed for Radio Resource reconfiguration");
        RETVALUE(RFAILED);
     }
   }
   if((isSib2TimMod == FALSE)&&(nonSib3TimMod == TRUE))
   {
      RLOG0(L_INFO,"RRC Timers modified");
      if((bkRcfgType & LWR_CELL_SIB2_CFG_BARRING_VIDEO)||
         (bkRcfgType & LWR_CELL_SIB2_CFG_BARRING_VOICE)||
         (bkRcfgType & LWR_CELL_SIB2_CFG_TIME_ALIGN)   ||
         (bkRcfgType & LWR_CELL_SIB2_CFG_UE_FREQ_INFO) ||
         (bkRcfgType & LWR_CELL_SIB2_CFG_BARRING_INFO) ||
         (bkRcfgType & LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN))
      {
         RLOG0(L_INFO,"Other SIB2 Group parameters are modified");
         RETVALUE(ROK);
      }
      else
      {
         RLOG0(L_WARNING, "Only RRC Timers modified,don't intimate to PBM ");
         RETVALUE(ROKIGNORE);
      }
   }
   RETVALUE(ROK);
} /* end of wrEmmCfgCellModSib2 */

/** @brief This function modify the SIB3 group's information to the cellCb
 *
 * @details
 *
 *     Function: wrEmmCfgCellModSib3
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[out]  cellCb : Cell control block
 * @param[in] sib3CfgGrp : Sib3 configuration parameters
 * @return S16
 *         -# Success : ROK
 *         -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellModSib3
(
WrCellCb                     *cellCb,
LwrCellSib3CfgGrp            *sib3ReCfgGrp
)
{
   U32                       rCfgType = 0;
   U32                       mask = 1;
  
   rCfgType =  sib3ReCfgGrp->reCfgType;
   
   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_CELL_SIB3_CFG_QHYST:
            {
               rCfgType =  rCfgType ^ mask; 
               RLOG2(L_DEBUG,"SIB3:Old qHyst[%d], New qHyst[%d]", cellCb->sib3.qHyst,sib3ReCfgGrp->qHyst);
               cellCb->sib3.qHyst    = sib3ReCfgGrp->qHyst;
               break;
            }
            case LWR_CELL_SIB3_CFG_SPD_ST_RESEL:
            {
               rCfgType =  rCfgType ^ mask; 
               RLOG2(L_DEBUG, "SIB3:Old SpeedStateReselPres [%d], New SpeedStateReselPres[%d]", 
                   cellCb->sib3.speedStateReselPres,sib3ReCfgGrp->speedStateReselPres);
               cellCb->sib3.speedStateReselPres = sib3ReCfgGrp->speedStateReselPres;
               break;
            }
            case LWR_CELL_SIB3_CFG_MOB_ST_PAR:
            {
               rCfgType =  rCfgType ^ mask; 
               RLOG2(L_DEBUG, "Old tEvaluation[%d], New tEvaluation[%d]",
                     cellCb->sib3.mobStateParams.tEvaluation,sib3ReCfgGrp->mobStateParams.tEvalulation);
               RLOG2(L_DEBUG, "Old tHyst[%d], New tHyst[%d]",
                     cellCb->sib3.mobStateParams.tHyst,sib3ReCfgGrp->mobStateParams.tHystNormal);
               RLOG2(L_DEBUG, "Old mobStateParams[%d],New mobStateParams[%d]",
                      cellCb->sib3.mobStateParams.nCellChangeMed,sib3ReCfgGrp->mobStateParams.nCellChangeMedium);
               RLOG2(L_DEBUG, "Old CellChangeHigh[%d],New CellChangeHigh[%d]",
                      cellCb->sib3.mobStateParams.nCellChangeHigh,sib3ReCfgGrp->mobStateParams.nCellChangeHigh);
               cellCb->sib3.mobStateParams.tEvaluation = sib3ReCfgGrp->mobStateParams.tEvalulation;
               cellCb->sib3.mobStateParams.tHyst = sib3ReCfgGrp->mobStateParams.tHystNormal;
               cellCb->sib3.mobStateParams.nCellChangeMed = sib3ReCfgGrp->mobStateParams.nCellChangeMedium;
               cellCb->sib3.mobStateParams.nCellChangeHigh = sib3ReCfgGrp->mobStateParams.nCellChangeHigh;
               break;
            }
            case LWR_CELL_SIB3_CFG_SF_MEDIUM:
            {
               rCfgType =  rCfgType ^ mask; 
               RLOG2(L_DEBUG, "Old qHyst medium[%d],New qHyst medium[%d]", 
                     cellCb->sib3.sfMedium,sib3ReCfgGrp->sfMedium);
               cellCb->sib3.sfMedium = sib3ReCfgGrp->sfMedium;
               break;
            }
            case LWR_CELL_SIB3_CFG_SF_HIG:
            {
               rCfgType =  rCfgType ^ mask; 
               RLOG2(L_DEBUG, "Old qHyst High[%d],New qHyst High[%d]",
                     cellCb->sib3.sfHigh,sib3ReCfgGrp->sfHigh);
               cellCb->sib3.sfHigh   = sib3ReCfgGrp->sfHigh;
               break;
            }
       case LWR_CELL_SIB3_CFG_SERV_FREQ_INFO:
            {
                rCfgType =  rCfgType ^ mask;
                RLOG2(L_DEBUG, "Old sNonIntraSearch[%d],New sNonIntraSearch[%d]",
                      cellCb->sib3.servFreqInfo.sNonIntraSearch,sib3ReCfgGrp->servFreqInfo.sNonIntraSearch);
                RLOG2(L_DEBUG,"Old threshServingLow[%d],New threshServingLow[%d]",
                      cellCb->sib3.servFreqInfo.threshServingLow,sib3ReCfgGrp->servFreqInfo.threshServingLow);
                RLOG2(L_DEBUG,"Old cellReselPrio[%d],New cellReselPrio[%d]",
                      cellCb->sib3.servFreqInfo.cellReselPrio,sib3ReCfgGrp->servFreqInfo.cellReselPrio);

                cellCb->sib3.servFreqInfo.sNonIntraSearch  = sib3ReCfgGrp->servFreqInfo.sNonIntraSearch;
                cellCb->sib3.servFreqInfo.threshServingLow = sib3ReCfgGrp->servFreqInfo.threshServingLow;
                cellCb->sib3.servFreqInfo.cellReselPrio    = sib3ReCfgGrp->servFreqInfo.cellReselPrio;
                break;
            }
            case LWR_CELL_SIB3_CFG_INTRA_RREQ:
            {
                rCfgType =  rCfgType ^ mask;
                RLOG2(L_DEBUG,"Old qRxLevMin[%d],New qRxLevMin[%d]",
                      cellCb->sib3.intraFreqReSel.qRxLevMin,sib3ReCfgGrp->intraFreqReSel.qRxLevMin);
                RLOG2(L_DEBUG,"Old pMax[%d],New pMax[%d]",
                      cellCb->sib3.intraFreqReSel.pMax,sib3ReCfgGrp->intraFreqReSel.pMax);
                RLOG2(L_DEBUG,"Old SIntraSearch[%d],New SIntraSearch[%d]",
                      cellCb->sib3.intraFreqReSel.SIntraSearch,sib3ReCfgGrp->intraFreqReSel.SIntraSearch);
                RLOG2(L_DEBUG,"Old tReselEutra[%d],New tReselEutra[%d]",
                      cellCb->sib3.intraFreqReSel.tReselEutra,sib3ReCfgGrp->intraFreqReSel.tReselEutra);
                RLOG2(L_DEBUG,"Old tResetEutraSFMedium[%d],New tResetEutraSFMedium[%d]",
                      cellCb->sib3.intraFreqReSel.tResetEutraSFMedium,
                      sib3ReCfgGrp->intraFreqReSel.tResetEutraSFMedium);
                RLOG2(L_DEBUG,"Old tResetEutraSFHigh[%d],New tResetEutraSFHigh[%d]",
                      cellCb->sib3.intraFreqReSel.tResetEutraSFHigh,
                      sib3ReCfgGrp->intraFreqReSel.tResetEutraSFHigh);
                cellCb->sib3.intraFreqReSel.qRxLevMin           = sib3ReCfgGrp->intraFreqReSel.qRxLevMin;
                cellCb->sib3.intraFreqReSel.pMax                = sib3ReCfgGrp->intraFreqReSel.pMax;
                cellCb->sib3.intraFreqReSel.SIntraSearch        = sib3ReCfgGrp->intraFreqReSel.SIntraSearch;
                cellCb->sib3.intraFreqReSel.tReselEutra         = sib3ReCfgGrp->intraFreqReSel.tReselEutra;
                cellCb->sib3.intraFreqReSel.tResetEutraSFMedium = sib3ReCfgGrp->intraFreqReSel.tResetEutraSFMedium;
                cellCb->sib3.intraFreqReSel.tResetEutraSFHigh   = sib3ReCfgGrp->intraFreqReSel.tResetEutraSFHigh;

                break;
            }         
            default:
            {   
               rCfgType =  rCfgType ^ mask; 
               RLOG1(L_WARNING, "SIB3 reconfiguration received unknown reCfgType [%ld]",
                           (sib3ReCfgGrp->reCfgType));

               break;
            }
         } /* end of switch */
      }
      mask = mask << 1;
   } /* end of while */
   
   RETVALUE(ROK);
} /* end of wrEmmCfgCellModSib3 */
/** @brief This function adds the SIB group's information to the cellCb
 *
 * @details
 *
 *     Function: wrEmmCfgCellAddSib3
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[out]  cellCb : Cell control block
 * @param[in] sib3CfgGrp : Sib2 configuration parameters
 * @return S16
 *         -# Success : ROK
 */
PRIVATE S16 wrEmmCfgCellAddSib3 
(
WrCellCb                     *cellCb,
LwrCellSib3CfgGrp            *sib3CfgGrp
)
{

   cellCb->sib3.mobStateParams.tEvaluation = sib3CfgGrp->mobStateParams.tEvalulation;
   cellCb->sib3.mobStateParams.tHyst = sib3CfgGrp->mobStateParams.tHystNormal;
   cellCb->sib3.mobStateParams.nCellChangeMed = sib3CfgGrp->mobStateParams.nCellChangeMedium;
   cellCb->sib3.mobStateParams.nCellChangeHigh = sib3CfgGrp->mobStateParams.nCellChangeHigh;
   cellCb->sib3.qHyst    = sib3CfgGrp->qHyst;
   cellCb->sib3.sfHigh   = sib3CfgGrp->sfHigh;
   cellCb->sib3.sfMedium = sib3CfgGrp->sfMedium;
   cellCb->sib3.speedStateReselPres = sib3CfgGrp->speedStateReselPres;

   RETVALUE(ROK);
}

/** @brief This function adds the SIB group's information to the cellCb
 *
 * @details
 *
 *     Function: wrEmmCfgCellAddSib4
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[out]  cellCb : Cell control block
 * @param[in] sib4CfgGrp : Sib4 configuration parameters
 * @return S16
 *         -# Success : ROK
 */
PRIVATE S16 wrEmmCfgCellAddSib4
(
WrCellCb                     *cellCb,
LwrCellSib4CfgGrp            *sib4CfgGrp
)
{
   S16                       ret = ROK;
    
   /* CSG_DEV */
   cellCb->sib4.csgCellInfoPres = FALSE;
   if(sib4CfgGrp->csgInfo.isCsgPciCfgd)
   {
   cellCb->sib4.csgCellInfoPres = TRUE;
   /*cellCb->sib4.csgPhyCellIdRange. = sib4CfgGrp->csgPciType; */
   cellCb->sib4.csgPhyCellIdRange.startPhyCellID = sib4CfgGrp->csgInfo.csgStartPhyCellId;
      cellCb->sib4.csgPhyCellIdRange.phyCellIdRangePres = sib4CfgGrp->csgInfo.isCsgPciCfgd;
   cellCb->sib4.csgPhyCellIdRange.phyCellIdRange = sib4CfgGrp->csgInfo.csgPciRange;
   }

   RETVALUE(ret);
}

/** @brief This function modify the SIB4 group's information to the cellCb
 *
 * @details
 *
 *     Function: wrEmmCfgCellModSib4
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[out]  cellCb : Cell control block
 * @param[in] sib4ReCfgGrp : Sib4 re-configuration parameters
 * @return S16
 *         -# Success : ROK
 *         -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellModSib4
(
WrCellCb                     *cellCb,
LwrCellSib4CfgGrp            *sib4ReCfgGrp
)
{
   U32                       rCfgType = 0;
   U32                       mask = 1;
  
   rCfgType =  sib4ReCfgGrp->reCfgType;
   
   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_CELL_SIB3_CFG_CSG_INFO:
            {
               rCfgType =  rCfgType ^ mask; 
               cellCb->sib4.csgCellInfoPres = TRUE;
               /* Not Useing currently */
               /* cellCb->sib4.csgPhyCellIdRange. = sib4ReCfgGrp->csgInfo.csgPciType; */

               RLOG_ARG2(L_DEBUG,DBG_CELLID,cellCb->cellId,"SIB4 Modified"
                  "PhysCellIdRange changed, old PhysCellId[%d],"
                  "new PhysCellId[%d]",cellCb->sib4.csgPhyCellIdRange.startPhyCellID, 
                   sib4ReCfgGrp->csgInfo.csgStartPhyCellId);

               cellCb->sib4.csgPhyCellIdRange.startPhyCellID = sib4ReCfgGrp->csgInfo.csgStartPhyCellId;
               cellCb->sib4.csgPhyCellIdRange.phyCellIdRangePres = sib4ReCfgGrp->csgInfo.isCsgPciCfgd;
               cellCb->sib4.csgPhyCellIdRange.phyCellIdRange = sib4ReCfgGrp->csgInfo.csgPciRange;

               RLOG3(L_DEBUG, "SIB4:StartPhyCellID [%d] PhyCellIdRangePres [%d] PhyCellIdRange [%d]",
                  cellCb->sib4.csgPhyCellIdRange.startPhyCellID,
                  cellCb->sib4.csgPhyCellIdRange.phyCellIdRangePres,
                  cellCb->sib4.csgPhyCellIdRange.phyCellIdRange);

               break;
            }
            default:
            {   
               rCfgType =  rCfgType ^ mask; 
               RLOG1(L_WARNING,"SIB4 reconfiguration received unknown reCfgType [%ld]",
                  (sib4ReCfgGrp->reCfgType));

               break;
            }
         } /* end of switch */
      }
      mask = mask << 1;
   } /* end of while */
   
   RETVALUE(ROK);
} /* end of wrEmmCfgCellModSib4 */


/** @brief This function modify the SIB6 group's information to the cellCb
 *
 * @details
 *
 *     Function: wrEmmCfgCellModSib6
 *
 *         Processing steps:
 *         -
 *
 *
 * @param[out]  cellCb : Cell control block
 * @param[in]   sib6ReCfgGrp : Sib6 re-configuration parameters
 * @return S16
 *         -# Success : ROK
 *         -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellModSib6
(
WrCellCb                     *cellCb,
LwrCellSib6CfgGrp            *sib6ReCfgGrp
)
{
   U32                       rCfgType = 0;
   U32                       mask = 1;

   rCfgType =  sib6ReCfgGrp->reCfgType;

   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_UTRAN_SIB6_SFUSAGE_PRES:
            {
               rCfgType = rCfgType ^ mask;
               RLOG2(L_DEBUG, "sib6:Old sfUsagePres[%d],New sfUsagePres[%d]",
               cellCb->sib6.sfUsagePres,sib6ReCfgGrp->sfUsagePres.pres);
               cellCb->sib6.sfUsagePres = sib6ReCfgGrp->sfUsagePres.pres;
                break;
            }
            case LWR_UTRAN_SIB6_TRESELECT:
            {
               rCfgType =  rCfgType ^ mask;
               RLOG2(L_DEBUG, "sib6:Old tReselection[%d],New tReselection[%d]",
                     cellCb->sib6.tReselection,sib6ReCfgGrp->tReselection);
               cellCb->sib6.tReselection = sib6ReCfgGrp->tReselection;
               break;
            }
            case LWR_UTRAN_SIB6_TRESELECT_SF_MEDIUM:
            {
               rCfgType =  rCfgType ^ mask;
               RLOG2(L_DEBUG, "sib6:Old tReselectionSfMedium[%d],New tReselectionSfMedium[%d]",
                     cellCb->sib6.tReselectionSfMedium,sib6ReCfgGrp->tReselectionSfMedium);
               cellCb->sib6.tReselectionSfMedium = sib6ReCfgGrp->tReselectionSfMedium;
               break;
            }
           case LWR_UTRAN_SIB6_TRESELECT_SF_HIGH:
           {
               rCfgType =  rCfgType ^ mask;
               RLOG2(L_INFO,"sib6:Old tReselectionSfHigh[%d],New tReselectionSfHigh[%d]",
                     cellCb->sib6.tReselectionSfHigh,sib6ReCfgGrp->tReselectionSfHigh);
               cellCb->sib6.tReselectionSfHigh   = sib6ReCfgGrp->tReselectionSfHigh;
               break;
            }
            default:
            {
               rCfgType =  rCfgType ^ mask;
               RLOG1(L_WARNING,"SIB6 reconfiguration received unknown reCfgType [%ld]",
                     (sib6ReCfgGrp->reCfgType));
               break;
            }
         } /* end of switch */
      }
      mask = mask << 1;
   } /* end of while */

   RETVALUE(ROK);
} /* end of wrEmmCfgCellModSib6 */

/** @brief This function adds the ANR group's information to the cellCb
 *
 * @details
 *
 *     Function: wrEmmCfgCellAnr
 *
 *         Processing steps:
 *
 *         - 
 *
 *
 * @param[in]   cellCb  : Cell control block
 * @param[in] anrCfgGrp : ANR configuration parameters
 * @return S16
 *         -# Success : ROK
 *         -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellAnr 
(
   WrCellCb                     *cellCb,
   LwrCellAnrCfgGrp             *anrCfgGrp
 )
{
   S16                       ret = ROK;

   wrEmmAnrCellCfgReq(cellCb->cellId, anrCfgGrp);

   ret = wrUmmCfgCellMeasTmrs (cellCb->cellId, anrCfgGrp->csfbTransTmrVal);

   RETVALUE(ret); 
} /* end of wrEmmCfgCellAnr */
PRIVATE S16 wrEmmReCfgCellAnr
(
WrCellCb                     *cellCb
)
{
   S16                       ret = ROK;

   ret = wrEmmAnrCellReCfgReq(cellCb->cellId);

   RETVALUE(ret); 
} /* end of wrEmmReCfgCellAnr */

PRIVATE S16 wrEmmCfgCellEaid
(
 WrCellCb                     *cellCb,
 LwrCellEAIDCfgGrp            *eaidGrp
 )
{
   U8                        idx = 0;

   cellCb->numOfCfgActiveEaids = 0;
   for (idx = 0; idx < LWR_SM_MAX_EAIDS; idx++)
   {
      if (eaidGrp->eaid[idx].pres == TRUE)
      {
         cellCb->eaidLst[cellCb->numOfCfgActiveEaids].eaid = eaidGrp->eaid[idx].val;
         cellCb->numOfCfgActiveEaids++;
      } /* end of if for to validate Active EAID */
   } /* end of for to traverse all EAIDs */
   RETVALUE(ROK); 
} /* end of wrEmmCfgCellEaid */

/** @brief This function adds the SIB scheduling group information to cell
 *         control block.  
 *
 * @details
 *
 *     Function: wrEmmCfgCellSibSched
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param [out] cellCb        : cell control block. 
 * @param [in] sibSchedCfgGrp :  SIB Scheduling configuration parameters.
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmCfgCellSibSched
(
 WrCellCb                     *cellCb,
 LwrCellSibSchedCfgGrp        *sibSchedCfgGrp
 )
{
   cellCb->numSibsCfg = 0;

   cellCb->sibsCfg[cellCb->numSibsCfg].period = sibSchedCfgGrp->sib2Period;
   cellCb->sibsCfg[cellCb->numSibsCfg].sibId = WR_SIB_2;
   cellCb->numSibsCfg++;

   cellCb->sibsCfg[cellCb->numSibsCfg].period = sibSchedCfgGrp->sib3Period;
   cellCb->sibsCfg[cellCb->numSibsCfg].sibId = WR_SIB_3;
   cellCb->numSibsCfg++;

   cellCb->sibsCfg[cellCb->numSibsCfg].period = sibSchedCfgGrp->sib4Period;
   cellCb->sibsCfg[cellCb->numSibsCfg].sibId = WR_SIB_4;
   cellCb->numSibsCfg++;
  
   if (sibSchedCfgGrp->sib6PeriodValid == TRUE)
   {
      cellCb->sibsCfg[cellCb->numSibsCfg].period = sibSchedCfgGrp->sib6Period;
      cellCb->sibsCfg[cellCb->numSibsCfg].sibId = WR_SIB_6;
      cellCb->numSibsCfg++;
   }
   if (sibSchedCfgGrp->sib7PeriodValid == TRUE)
   {
      cellCb->sibsCfg[cellCb->numSibsCfg].period = sibSchedCfgGrp->sib7Period;
      cellCb->sibsCfg[cellCb->numSibsCfg].sibId = WR_SIB_7;
      cellCb->numSibsCfg++;
   }
   if (sibSchedCfgGrp->sib8PeriodValid == TRUE)
   {
      cellCb->sibsCfg[cellCb->numSibsCfg].period = sibSchedCfgGrp->sib8Period;
      cellCb->sibsCfg[cellCb->numSibsCfg].sibId = WR_SIB_8;
      cellCb->numSibsCfg++;
   }

   /* CSG_DEV */
   if(strlen((S8 *)cellCb->sib9.henbName) > 0)
   {
   cellCb->sibsCfg[cellCb->numSibsCfg].period = sibSchedCfgGrp->sib9Period;
   cellCb->sibsCfg[cellCb->numSibsCfg].sibId = WR_SIB_9;
   cellCb->numSibsCfg++;
   }

   cellCb->sibsCfg[cellCb->numSibsCfg].period = sibSchedCfgGrp->sib5Period;
   cellCb->sibsCfg[cellCb->numSibsCfg].sibId = WR_SIB_5;
   cellCb->numSibsCfg++;

   cellCb->sib1.siWindowLen = sibSchedCfgGrp->siWindow;
   cellCb->sib10Info.sib10ManPerdUsage = sibSchedCfgGrp->sib10ManualPrdPres;
   cellCb->sib10Info.sib10NumOfBroadcasts = sibSchedCfgGrp->sib10NumBroadCast;
   cellCb->sib10Info.sib10RepetitionPeriod = sibSchedCfgGrp->sib10RepetPeriod;
   cellCb->warMsgSegmentSize = sibSchedCfgGrp->sib11WarnSegSize;

   RETVALUE(ROK);
} /* end of wrEmmCfgCellSibSched */

PRIVATE S16 wrEmmUpdateSibSched
(
WrCellCb                     *cellCb,
U8                           sibNum,
U16                          period
)
{
   U8                        sibIdx = 0;

   for(sibIdx =0; sibIdx < cellCb->numSibsCfg; sibIdx++)
   { 
      if(sibNum == cellCb->sibsCfg[sibIdx].sibId)
      {
         cellCb->sibsCfg[sibIdx].period = period;

         RLOG2(L_DEBUG, "sibNum[%d], period[%d]",
            cellCb->sibsCfg[sibIdx].sibId, cellCb->sibsCfg[sibIdx].period);
         RETVALUE(ROK);
      }
   }
   cellCb->sibsCfg[cellCb->numSibsCfg].period = period;
   cellCb->sibsCfg[cellCb->numSibsCfg].sibId = sibNum;
   cellCb->numSibsCfg++;
   RETVALUE(ROK);
} /* end of wrEmmUpdateSibSched */

PRIVATE S16 wrEmmReCfgCellSibSchedValidate
(
WrCellCb                     *cellCb,
LwrCellSibSchedCfgGrp        *sibSchedReCfgGrp
)
{
      RETVALUE(ROK);
} /* end of wrEmmReCfgCellSibSchedValidate */

PRIVATE S16 wrEmmReCfgCellEaidValidate
(
WrCellCb                     *cellCb,
LwrCellEAIDCfgGrp            *eaidReCfgGrp
)
{
      RETVALUE(ROK);
} /* end of wrEmmReCfgCellEaidValidate */

/** @brief This function updates the EAID information to cell
 *         control block.  
 *
 * @details
 *
 *     Function: wrEmmReCfgCellEaid
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param [out] cellCb        : cell control block. 
 * @param [in] eaidReCfgGrp :  EAID re-configuration.
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmReCfgCellEaid
(
WrCellCb                     *cellCb,
LwrCellEAIDCfgGrp            *eaidReCfgGrp
)
{
   U32                       rCfgType = 0;
   U32                       mask = 1;
   S16                       ret = ROK;
   U8                        idx = 0;

   /* isWarningMsgBroadcastInit is TRUE  then fail the configuration */ 
   ret = wrEmmReCfgCellEaidValidate(cellCb, eaidReCfgGrp);

   if (ROK != ret)
   {
      RLOG1(L_ERROR,"Eaid reconfiguration validation failed[%d]", ret);
      RETVALUE(RFAILED);
   }
   rCfgType =  eaidReCfgGrp->reCfgType;
   
   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_CELL_EAID_CFG_EAID:
            {
                  rCfgType =  rCfgType ^ mask; 
                  cmMemset((U8*)(cellCb->eaidLst), 
                           0, sizeof(WrActCfgEaid));
                  cellCb->numOfCfgActiveEaids = 0;
                  for (idx = 0; idx < LWR_SM_MAX_EAIDS; idx++)
                  {
                     if (eaidReCfgGrp->eaid[idx].pres == TRUE)
                     {
                        cellCb->eaidLst[cellCb->numOfCfgActiveEaids].eaid = eaidReCfgGrp->eaid[idx].val;
                        cellCb->numOfCfgActiveEaids++;
                     } /* end of if for to validate Active EAID */
                  } /* end of for to traverse all EAIDs */
                  break;
            }
            default:
            {   
                rCfgType =  rCfgType ^ mask; 
                RLOG1(L_WARNING, "Eaid reconfiguration received unknown reCfgType [%ld]",
                   (eaidReCfgGrp->reCfgType));
                rCfgType =  rCfgType ^ mask; 
                break;
            }
         } /* end of switch */
      }
      mask = mask << 1;
   } /* end of while */
   RETVALUE(ROK);
} /* end of wrEmmReCfgCellEaid */


/** @brief This function updates the SIB scheduling group information to cell
 *         control block.  
 *
 * @details
 *
 *     Function: wrEmmReCfgCellSibSched
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param [out] cellCb        : cell control block. 
 * @param [in] sibSchedReCfgGrp :  SIB Scheduling configuration parameters.
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmReCfgCellSibSched
(
WrCellCb                     *cellCb,
LwrCellSibSchedCfgGrp        *sibSchedReCfgGrp,
U8                           *numSibs,
U8                           sibs[WR_MAX_SIBS]
)
{
   U32                       rCfgType = 0;
   U32                       mask = 1;
   S16                       ret = ROK;
  
   ret = wrEmmReCfgCellSibSchedValidate(cellCb, sibSchedReCfgGrp);

   if (ROK != ret)
   {
      RLOG1(L_ERROR,"SIB scheduling information reconfiguration validation failed[%d]",
          ret);
      RETVALUE(RFAILED);
   }
   rCfgType =  sibSchedReCfgGrp->reCfgType;
   
   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_CELL_SIBSCH_SIB2_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               wrEmmUpdateSibSched(cellCb, WR_SIB_2, sibSchedReCfgGrp->sib2Period);
               sibs[*numSibs] = WR_SIB_2;
               (*numSibs)++;
               break;
            }
            case LWR_CELL_SIBSCH_SIB3_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               wrEmmUpdateSibSched(cellCb, WR_SIB_3, sibSchedReCfgGrp->sib3Period);
               sibs[*numSibs] = WR_SIB_3;
               (*numSibs)++;
               break;
            }
            case LWR_CELL_SIBSCH_SIB4_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               wrEmmUpdateSibSched(cellCb, WR_SIB_4, sibSchedReCfgGrp->sib4Period);
               sibs[*numSibs] = WR_SIB_4;
               (*numSibs)++;
               break;
            }
            case LWR_CELL_SIBSCH_SIB5_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               wrEmmUpdateSibSched(cellCb, WR_SIB_5, sibSchedReCfgGrp->sib5Period);
               sibs[*numSibs] = WR_SIB_5;
               (*numSibs)++;
               break;
            }
            case LWR_CELL_SIBSCH_SIB6_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               wrEmmUpdateSibSched(cellCb, WR_SIB_6, sibSchedReCfgGrp->sib6Period);
               sibs[*numSibs] = WR_SIB_6;
               (*numSibs)++;
               break;
            }
            case LWR_CELL_SIBSCH_SIB7_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            case LWR_CELL_SIBSCH_SIB8_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            case LWR_CELL_SIBSCH_SIB9_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               wrEmmUpdateSibSched(cellCb, WR_SIB_9, sibSchedReCfgGrp->sib9Period);
               sibs[*numSibs] = WR_SIB_9;
               (*numSibs)++;
               break;
            }
            case LWR_CELL_SIBSCH_SIB10_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            case LWR_CELL_SIBSCH_SIB11_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            case LWR_CELL_SIBSCH_SIB12_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            case LWR_CELL_SIBSCH_SWIN_PERIOD:
            {
               rCfgType =  rCfgType ^ mask; 
               RLOG0(L_INFO,"Modificcation for SI-Window is not allowed");
               break;
            }
            case LWR_CELL_SIBSCH_SIB10_MANUAL_PRD:
            {
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            case LWR_CELL_SIBSCH_SIB10_REPEAT_PRD:
            {
               rCfgType =  rCfgType ^ mask; 
               cellCb->sib10Info.sib10RepetitionPeriod = sibSchedReCfgGrp->sib10RepetPeriod;
               break;
            }
            case LWR_CELL_SIBSCH_SIB10_NUM_BRDCST:
            {
               rCfgType =  rCfgType ^ mask; 
               cellCb->sib10Info.sib10NumOfBroadcasts = sibSchedReCfgGrp->sib10NumBroadCast;
               break;
            }
            case LWR_CELL_SIBSCH_SIB11_WARN_SEG:
            {
               rCfgType =  rCfgType ^ mask; 
               cellCb->warMsgSegmentSize = sibSchedReCfgGrp->sib11WarnSegSize;
               break;
            }
            default:
            {   
               rCfgType =  rCfgType ^ mask; 
               RLOG1(L_WARNING, "SIB scheduling reconfiguration received unknown"
                  "reCfgType [%ld]",(sibSchedReCfgGrp->reCfgType));

               break;
            }
         } /* end of switch */
      }
      mask = mask << 1;
   } /* end of while */
   RETVALUE(ROK);
} /* end of wrEmmReCfgCellSibSched */

/** @brief This function is responsible for addition of a mme at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgMmeAdd
 *
 *         Processing steps:
 *         - This is add a mme to the EnodeB 
 *         - It allocates a new WrMmeCb 
 *         - It copies all the configuration into the allocated mme Control block
 *
 * @param[in] cfgAdd : MME Configuration parameters
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE S16 wrEmmCfgMmeAdd
(
 LwrAddModMmeCfg              *cfgAdd
)
{
   S16                       ret = LCM_REASON_NOT_APPL;
   WrMmeCb                   *mmeCb = NULLP;
   U8                        idx;

   /*Validating the MME Array index*/
   if (cfgAdd->mmeIndex >= WR_MAX_MMES)
   {
      RLOG1(L_WARNING, "MME CB array index is out of bound[%d]",cfgAdd->mmeIndex);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   if (wrEmmCb.mmeCont.mmes[cfgAdd->mmeIndex] != NULLP)
   {
      RLOG1(L_ERROR,"MME index already in use[%d]", cfgAdd->mmeIndex);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   /*Allocating Memory for the MME Control Block*/
   WR_ALLOC(&(wrEmmCb.mmeCont.mmes[cfgAdd->mmeIndex]), sizeof(WrMmeCb));
   mmeCb = wrEmmCb.mmeCont.mmes[cfgAdd->mmeIndex];
   if (mmeCb == NULLP)
   {
      RLOG1(L_FATAL,"Memory allocation failed during MME configuration [%d]", cfgAdd->mmeIndex);
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   /* Boundary check */
   if (cfgAdd->noOfIps >  LWR_MAX_IP_PER_MME)
   {
      cfgAdd->noOfIps = LWR_MAX_IP_PER_MME;
   }
   /*Copying MME IP Address*/
   for(idx = 0; idx < cfgAdd->noOfIps; idx++)
   {
      mmeCb->mmeAddr[idx] = cfgAdd->ipAddr[idx];
   }
   mmeCb->state = WR_MME_INITED;

   mmeCb->peerId = cfgAdd->mmeIndex;
   mmeCb->mmeId = cfgAdd->mmeIndex;
   mmeCb->adminState = WR_MME_UNLOCKED;
   wrEmmCb.mmeCont.numMmes += 1;
   mmeCb->alarmVal.maxErrIndCnt = cfgAdd->alarmVal.maxErrIndCnt;
   mmeCb->alarmVal.maxResetCnt = cfgAdd->alarmVal.maxResetCnt;
   mmeCb->alarmVal.minErrIndTime = cfgAdd->alarmVal.minErrIndTime;
   mmeCb->alarmVal.minResetTime = cfgAdd->alarmVal.minResetTime;
   cmInitTimers(&mmeCb->timer ,1);
   wrEmmS1FlexInitS1FlexVarFrMme(mmeCb);
   RETVALUE(ret);
}

/** @brief This function validates the Reconfiguration of MME related
 * information at EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgMmeModValidate
 *
 *         Processing steps:
 *         - Validates the MME reconfiguration from layer Manger.
 *
 * @param[in] cfgAdd : MME Configuration parameters
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */
PRIVATE S16 wrEmmCfgMmeModValidate(LwrAddModMmeCfg  *cfgMod)
{

   /*Validating the MME Array index*/
   if (cfgMod->mmeIndex >= WR_MAX_MMES)
   {
      RLOG1(L_WARNING, "MME CB array index is out of bound [%d]",cfgMod->mmeIndex);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   
   if (wrEmmCb.mmeCont.mmes[cfgMod->mmeIndex] == NULLP)
   {
      RLOG1(L_ERROR,"MME index is not present[%d]",cfgMod->mmeIndex);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of wrEmmCfgMmeModValidate */

/** @brief This function is responsible for Modifying the mme related
 * information at EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgMmeMod
 *
 *         Processing steps:
 *         - Modifies the mme related parameters at EnodeB 
 *         - It copies all the configuration into the allocated mme Control block
 *
 * @param[in] cfgAdd : MME Configuration parameters
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE S16 wrEmmCfgMmeMod
(
LwrAddModMmeCfg              *cfgMod
)
{
   WrMmeCb                   *mmeCb = NULLP;
   U32                       rCfgType = 0;
   U32                       mask = 1;
   S16                       ret = ROK;

   ret = wrEmmCfgMmeModValidate(cfgMod);
  
   rCfgType =  cfgMod->reCfgType;
   
   if (ROK != ret)
   {
      RLOG1(L_ERROR,"Validation failed for modified MME configuration[%d]",ret);
      RETVALUE(ret);
   }
   
   /* Get the MME Control Block*/
   mmeCb = wrEmmCb.mmeCont.mmes[cfgMod->mmeIndex];

   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_MME_INDEX:
            {
              rCfgType =  rCfgType ^ mask; 
              /*  mmeCb->peerId = cfgMod->mmeIndex; */
              RLOG2(L_DEBUG,"Old MMEID[%d],New MMEID[%d]",mmeCb->mmeId,cfgMod->mmeIndex);
              mmeCb->mmeId = cfgMod->mmeIndex;
              break;
            }
            case LWR_MME_IPADDR:
            {
              rCfgType =  rCfgType ^ mask; 
               /*Copying MME primary IPv4 Address*/
              RLOG0(L_INFO,"Modification is not allowed for MME IP Address");
               break;
            }
            case LWR_MME_SECIP_ADDR:
            {
              rCfgType =  rCfgType ^ mask; 
               /*Copying MME secondary IPv4 Address*/
              RLOG0(L_INFO,"Modification is not allowed for MME Secondary IP"
                 " Address");
               break;
            }
            case LWR_MME_STATE:
            {
              rCfgType =  rCfgType ^ mask; 
               /* TODO-VHK: is which state is suitable during Reconfiguration */ 
              RLOG2(L_DEBUG,"Old MME STATE[%d],New MME STATE[%d]",mmeCb->state,cfgMod->mmeState);
              mmeCb->state = cfgMod->mmeState;
              break;
            }
            case LWR_ADMIN_STATE:
            {
              rCfgType =  rCfgType ^ mask; 
               /* TODO: Functionality need to implement with AdminState */
              RLOG2(L_DEBUG,"Old MME ADMIN STATE[%d],New MME ADMINSTATE[%d]",mmeCb->adminState,cfgMod->adminState);
              mmeCb->adminState = cfgMod->adminState;
              break;
            }
            default:
            {   
              rCfgType =  rCfgType ^ mask; 
              RLOG1(L_WARNING, "MME reconfiguration received unknown reCfgType [%ld]",
                    (cfgMod->reCfgType));

               break;
            }
         } /* end of switch */
      }
      mask = mask << 1;
   } /* end of while */
 
   RETVALUE(ret);
} /* end of wrEmmCfgMmeMod */

/** @brief This function is responsible for Modifying the mme related
 * information at EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgMmeDel
 *
 *         Processing steps:
 *
 * @param[in] cfgDel : MME index which need to delete
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */
PRIVATE S16 wrEmmCfgMmeDel
(
LwrDelMme              *cfgDel
)
{
   WrMmeCb                   *mmeCb = NULLP;
   U8                         idCnt;

   /*Validating the MME Array index*/
   if (cfgDel->mmeIndex >= WR_MAX_MMES)
   {
      RLOG1(L_WARNING, "MME CB Array index is out of bound[%d]", cfgDel->mmeIndex);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   
   if (wrEmmCb.mmeCont.mmes[cfgDel->mmeIndex] == NULLP)
   {
      RLOG1(L_ERROR,"MME index is not present[%d]", cfgDel->mmeIndex);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
  
   /* Get the MME Control Block*/
   mmeCb = wrEmmCb.mmeCont.mmes[cfgDel->mmeIndex];

   /* Release all UEs belongs to this particluar MME */
   RLOG1(L_DEBUG,"Deleting MME Id = %d",mmeCb->mmeId);
   wrUmmMmeFullRst(mmeCb->mmeId);
   /* If MME is UP enodeB Timer will be running else MME setup timer will
    * running*/
   if((mmeCb->state == WR_MME_UP))
   {
       wrStopTmr((PTR)mmeCb->mmeId, WR_TMR_ENB_CFG_RSP);
       wrStopTmr((PTR)mmeCb->mmeId, WR_TMR_RST_RSP);
   }
   else if((mmeCb->state == WR_MME_IN_PROG))
   {
      wrStopTmr((PTR)mmeCb, WR_TMR_MME_SETUP_RSP);
   }
   else
   {
      wrStopTmr((PTR)mmeCb, WR_TMR_MME_SETUP_WAIT);
   }
   for(idCnt = 0; idCnt < mmeCb->numPlmnIds; idCnt++)
   {
      wrEmmPlmnDelMme(&mmeCb->plmnIds[idCnt], mmeCb->mmeId);
   }
   WR_FREE(wrEmmCb.mmeCont.mmes[cfgDel->mmeIndex], sizeof(WrMmeCb));
   RETVALUE(LCM_REASON_NOT_APPL);
} /* end of wrEmmCfgMmeDel */

/** @brief This function is responsible for Modifying the Sib5 related
  parameter
 *
 * @details This function updates sib5 parameter in case of dynamically 
 *          modification request received
 *
 *     Function: wrEmmCfgCellModNghFreq
 *
 *         Processing steps:
 *
 * @param[in] neighFreqCb: 
 * @param[in] eutranFreq: 
 * @return Void
 *     -# Success : ROK
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */
PRIVATE Void wrEmmCfgCellModNghFreq
(
 WrEutranFreqCb                *neighFreqCb, 
 LwrNeighEutraFreq             *eutranFreq
 )
{
   U32                       rCfgType         = 0;
   U32                       mask             = 1;
   rCfgType =  eutranFreq->reCfgType;
   while (rCfgType)
   {
      if(rCfgType & mask)  
      {
         switch(mask)
         {
            case LWR_NBR_EUTRAN_FREQ_QRXLEVMIN:
               {
                  rCfgType =  rCfgType ^ mask;
                  RLOG2(L_DEBUG,"Old qRxLevMin[%d],New qRxLevMin[%d]",neighFreqCb->qRxLevMin,eutranFreq->qRxLevMin);
                  neighFreqCb->qRxLevMin = eutranFreq->qRxLevMin;
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_OFFSETFREQ: 
               {
                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old qOffsetFreq[%d],New qOffsetFreq[%d]",
                        neighFreqCb->qOffsetFreq,eutranFreq->qOffsetFreq);
                  neighFreqCb->qOffsetFreq = eutranFreq->qOffsetFreq;   
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_TRESELECTION:
               {
                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old tReselcxn[%d],New tReselcxn[%d]",
                          neighFreqCb->tReselcxn,eutranFreq->tReselection);
                  neighFreqCb->tReselcxn = eutranFreq->tReselection;
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_PRIORITY:
               {
                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old cellReSelPri[%d],New cellReSelPri[%d]",
                         neighFreqCb->cellReSelPri,eutranFreq->priority);
                  neighFreqCb->cellReSelPri        = eutranFreq->priority;
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_THRESHXHIGH:
               {
                  rCfgType =  rCfgType ^ mask;
                  RLOG2(L_DEBUG,"Old threshXHigh[%d],New threshXHigh [%d]",
                        neighFreqCb->threshXHigh,eutranFreq->threshXHigh);
                  neighFreqCb->threshXHigh = eutranFreq->threshXHigh;
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_THRESHXLOW:
               {
                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old threshXLow[%d],New threshXLow[%d]",
                        neighFreqCb->threshXLow,eutranFreq->threshXLow);
                  neighFreqCb->threshXLow = eutranFreq->threshXLow;
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_PMAX:
               {
                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old pMax[%d],New pMax[%d]",
                        neighFreqCb->pMax,eutranFreq->pMax);
                  neighFreqCb->pMax = eutranFreq->pMax;
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_TRESELECTIONSFMED:
               {

                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old sfMedium[%d],New sfMedium[%d]",
                        neighFreqCb->sfMedium,eutranFreq->tReselectionSFMed);
                  neighFreqCb->sfMedium = eutranFreq->tReselectionSFMed;
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_TRESELECTIONSFHIGH:
               {

                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old sfHigh[%d],New tReselectionSFHigh[%d]",
                        neighFreqCb->sfHigh,eutranFreq->tReselectionSFHigh);
                  neighFreqCb->sfHigh = eutranFreq->tReselectionSFHigh;

               }
               break;
            case LWR_NBR_EUTRAN_FREQ_MEASBANDWIDTH:
               {
                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old allowedMeasBW[%d],New allowedMeasBW[%d]",
                        neighFreqCb->allowedMeasBW.val,eutranFreq->measBandwidth.val);
                  neighFreqCb->allowedMeasBW = eutranFreq->measBandwidth;          
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_QQUALMINREL9:
               {
                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old qQualMinR9[%d],New qQualMinR9[%d]",
                        neighFreqCb->qQualMinR9.val,eutranFreq->qQualMinRel9.val);
                  neighFreqCb->qQualMinR9 =eutranFreq->qQualMinRel9;    
               }  
               break;
            case LWR_NBR_EUTRAN_FREQ_THRESHXQREL9HIGH:
               {
                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old threshXQR9.threshXHighQR9[%d],New threshXQR9.threshXHighQR9[%d]",
                        neighFreqCb->threshXQR9.threshXHighQR9,eutranFreq->threshXQRel9.threshXHighQR9);
                  neighFreqCb->threshXQR9.threshXHighQR9 = eutranFreq->threshXQRel9.threshXHighQR9;
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_THRESHXQREL9LOW:
               {
                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old threshXQR9.threshXLowQR9[%d],New threshXQR9.threshXLowQR9[%d]",
                        neighFreqCb->threshXQR9.threshXLowQR9,eutranFreq->threshXQRel9.threshXLowQR9);
                  neighFreqCb->threshXQR9.threshXLowQR9 = eutranFreq->threshXQRel9.threshXLowQR9;
               }
               break;


            case LWR_NBR_EUTRAN_FREQ_PRESANTENNAPORT1:
               {
                  rCfgType =  rCfgType ^ mask; 
                  RLOG2(L_DEBUG,"Old presenceAntPort1[%d],New presenceAntPort1[%d]",
                        neighFreqCb->presenceAntPort1,eutranFreq->presAntennaPort1);
                  neighFreqCb->presenceAntPort1 = eutranFreq->presAntennaPort1;
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_SERVTHRESHOLDLOW:
               {
                  rCfgType =  rCfgType ^ mask;
                  RLOG2(L_DEBUG,"Old thSrvngLow[%d],New thSrvngLow[%d]",
                        neighFreqCb->thSrvngLow,eutranFreq->servThresholdLow);
                  neighFreqCb->thSrvngLow = eutranFreq->servThresholdLow;
               }
               break;
            case LWR_NBR_EUTRAN_FREQ_SNONINTRASEARCH:
               {
                  rCfgType =  rCfgType ^ mask;
                  RLOG2(L_DEBUG,"Old sNonIntraSrch.val[%d],New sNonIntraSrch.val[%d]",
                        neighFreqCb->sNonIntraSrch.val,eutranFreq->sNonIntraSearch.val);
                  neighFreqCb->sNonIntraSrch.val = eutranFreq->sNonIntraSearch.val;
               }
               break;
            default:
               {   
                  RLOG0(L_WARNING, "SIB5 reconfiguration received unknown reCfgType");
                  rCfgType =  rCfgType ^ mask; 
               }
         }
      } /* end of switch */
      mask = mask << 1;
   }
   RETVOID;
} /* end of wrEmmCfgCellModNghFreq */



PRIVATE Void wrEmmCopyEutranNeighFreq
(
 WrEutranFreqCb                *neighFreqCb, 
 LwrNeighEutraFreq             *eutranFreq
 )
{ 
   /*Filling the received values*/
   /* TODO : eutranFreq->reCfgType, used while rec onfiguration is done */
   neighFreqCb->reCfgType           = eutranFreq->reCfgType;
   neighFreqCb->status              = eutranFreq->status;
   neighFreqCb->dlEarfcn            = eutranFreq->earfcnDl;
   neighFreqCb->ulEarfcn            = eutranFreq->earfcnUl;
   neighFreqCb->cellReSelPri        = eutranFreq->priority;
   neighFreqCb->qRxLevMin           = eutranFreq->qRxLevMin;
   neighFreqCb->pMaxPres            = eutranFreq->pMaxUsage;
   neighFreqCb->pMax                = eutranFreq->pMax;
   neighFreqCb->tReselcxn           = eutranFreq->tReselection;
   neighFreqCb->sfPres              = eutranFreq->sfUsage;
   neighFreqCb->sfMedium            = eutranFreq->tReselectionSFMed;
   neighFreqCb->sfHigh              = eutranFreq->tReselectionSFHigh;
   neighFreqCb->sIntraSrch          = eutranFreq->sIntraSearch;        
   neighFreqCb->sNonIntraSrch       = eutranFreq->sNonIntraSearch;        
   neighFreqCb->thSrvngLow          = eutranFreq->servThresholdLow;
   neighFreqCb->threshXHigh         = eutranFreq->threshXHigh;
   neighFreqCb->threshXLow          = eutranFreq->threshXLow;
   neighFreqCb->allowedMeasBW       = eutranFreq->measBandwidth;          
   neighFreqCb->presenceAntPort1    = eutranFreq->presAntennaPort1;
   neighFreqCb->neighCellConfig     = eutranFreq->neighCellCfg;  /*!< Bit string (2) */
   neighFreqCb->qOffsetFreq         = eutranFreq->qOffsetFreq;   /*!< For SIB 5 */
   neighFreqCb->offsetFreq          = eutranFreq->offsetFreq;    /*!<  FOR MeasEUTRA */
   neighFreqCb->sIntraSrchR9        = *(WrSSearchR9*)&eutranFreq->sIntraSearchRel9;
   neighFreqCb->sNonIntraSrchR9     = *(WrSSearchR9*)&eutranFreq->sInterSearchRel9;
   neighFreqCb->qQualMinR9          = eutranFreq->qQualMinRel9;    /*!< {-34,..., -3} */
   neighFreqCb->thSrvngLowQR9       = eutranFreq->reselecThreshQRel9;
   neighFreqCb->threshXQR9          = *(WrThreshXQR9*)&eutranFreq->threshXQRel9;

   wrEmmPrintNeighFreqCb(neighFreqCb);

   RETVOID;
}

/** @brief This function is responsible for copying the GERAN neighbour 
 *       freqency info to neighbour frequency control block
 *
 * @details
 *
 *     Function: wrEmmCopyGeranNeighFreq
 *
 *         Processing steps:
 *         -
 *
 * @param[out] neighFreqCb  : Geran neighbour frequency Control block
 * @param[in]  geranFreq    : Geran Neighbour Frequency parameters
 * @return void
 */
PRIVATE Void wrEmmCopyGeranNeighFreq
(
 WrGeranFreqCb                *neighFreqCb,
 LwrNeighGERANFreq            *geranFreq
)
{
  neighFreqCb->arfcn             = geranFreq->bcchArfcn;
  neighFreqCb->qRxLevMin         = geranFreq->qRxLevMin;
  neighFreqCb->bandInd           = geranFreq->bandIndicator;
  neighFreqCb->pMaxGeran.pres    = geranFreq->pMaxGeran.pres; 
  neighFreqCb->pMaxGeran.val     = geranFreq->pMaxGeran.val;
  neighFreqCb->cellReselPriority = geranFreq->cellReselePri;
  neighFreqCb->threshXLo         = geranFreq->threshXLow;
  neighFreqCb->threshXHi         = geranFreq->threshXHigh;
  neighFreqCb->nccPermitted      = geranFreq->ncc_Permitted;
 
}

PRIVATE Void wrEmmCopyUtranNeighFreq
(
 WrUtraFreqCb                *neighFreqCb,
 LwrNeighUtraFreq             *utranFreq
)
{
   neighFreqCb->freqIndex = utranFreq->freqIndex;
   if(utranFreq->priority.pres == PRSNT_NODEF)
   {
      neighFreqCb->priority.pres = PRSNT_NODEF;
      neighFreqCb->priority.val  = utranFreq->priority.val;
   }
   else
   {
      neighFreqCb->priority.pres = PRSNT_DEF;
      neighFreqCb->priority.val  = 0;
   }
   neighFreqCb->threshXhigh = utranFreq->threshXhigh;
   neighFreqCb->threshXlow  = utranFreq->threshXlow;
   neighFreqCb->qRxLevMin   = utranFreq->qRxLevMin;
   neighFreqCb->pMaxUtra    = utranFreq->pMaxUtra;
   neighFreqCb->offsetFreq  = utranFreq->offsetFreq;
   neighFreqCb->duplexMode  = utranFreq->duplexMode;
   if(WR_UTRA_MODE_FDD == neighFreqCb->duplexMode)
   {
      neighFreqCb->t.utraFddFreq.thresQR9Pres    = utranFreq->t.utraFdd.thresR9.pres;
      neighFreqCb->t.utraFddFreq.threshXhighQR9  = utranFreq->t.utraFdd.thresR9.thresXHighQR9;
      neighFreqCb->t.utraFddFreq.threshXlowQR9   = utranFreq->t.utraFdd.thresR9.thresXLowQR9;
      neighFreqCb->t.utraFddFreq.arfcnUl         = utranFreq->t.utraFdd.arfcnUl;
      neighFreqCb->t.utraFddFreq.arfcnDl         = utranFreq->t.utraFdd.arfcnDl;
      neighFreqCb->t.utraFddFreq.pQualMin        = utranFreq->t.utraFdd.pQualMin;
   }
   else
   {
      neighFreqCb->t.utraTddFreq.tddMode = utranFreq->t.utraTdd.tddMode;
      neighFreqCb->t.utraTddFreq.arfcn = utranFreq->t.utraTdd.arfcn;
      neighFreqCb->t.utraTddFreq.bandIndicator = utranFreq->t.utraTdd.bandIndicator;
   }
}

PRIVATE S16 wrEmmFreeList
(
 CmLListCp                  *lst, 
 Bool                       freeFlag, 
 U16                        memSize
 )
{
   CmLList                       *lnk;
   PTR                           node;

   TRC2(wrEmmAnrFreeLList);

   lnk = lst->first;
   while(lnk != NULLP)
   {
      cmLListDelFrm(lst, lnk);

      node = lnk->node;
      lnk  = lnk->next;

      if(freeFlag)
      {
         WR_FREE(node, memSize);
      }
   }

   RETVALUE(ROK);
}

/** @brief This function validates the Utran Freq with TddMode and Bandwidth parameters
 *
 * @details validates UTRA TDD Arfcn configuration parameters
 *
 *     Function:wrEmmCfgValidateUtraTddFreq 
 *
 *         Processing steps:
 *         - validate  for valid tddMode and band, is frequency valid. 
 *
 * @param [in]  geranFreq
 * @return S16
 *          -# Success : ROK
 *          -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgValidateUtraTddFreq
(
 LwrNeighUtraFreq *utranFreq
)
{
   S16 status =  ROK;
   
   switch(utranFreq->t.utraTdd.tddMode)
   {
      case LWR_UTRA_TDD128:
         status = wrEmmMatchFreqAndBandForTDDMode_128(utranFreq->t.utraTdd.arfcn, utranFreq->t.utraTdd.bandIndicator);
            break;
      case LWR_UTRA_TDD384:
         status = wrEmmMatchFreqAndBandForTDDMode_384(utranFreq->t.utraTdd.arfcn, utranFreq->t.utraTdd.bandIndicator);
         break;
      case LWR_UTRA_TDD768:
         status = wrEmmMatchFreqAndBandForTDDMode_768(utranFreq->t.utraTdd.arfcn, utranFreq->t.utraTdd.bandIndicator);
         break;
      default:
         break;
   }
   
   RETVALUE(status);
}


/** @brief This function validates the GERAN cell level config parameters
 *
 * @details validates GERAN cell level configuration request parameters
 *
 *     Function: wrEmmCfgCellValidateGeranParam
 *
 *         Processing steps:
 *         - validate GERAN cell level configuration parameters
 *
 * @param [in]  geranFreq : GERAN cell level Configuration parameters
 * @return S16
 *          -# Success : ROK
 *          -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgCellValidateGeranParam
(
 LwrNeighGERANFreq            *geranFreq
)
{
   S16 ret =  ROK;
   U8 qRxLevMin = geranFreq->qRxLevMin;
   U8 cellReselePri = geranFreq->cellReselePri;
   U8 threshXHigh = geranFreq->threshXHigh;
   U8 threshXLow = geranFreq->threshXLow;

   if(qRxLevMin > 45)
   {
      RLOG1(L_DEBUG,"Validation failed for Geran configuration.Value of qRxLevMin=%d",qRxLevMin);
      ret = RFAILED;
   }

   if(cellReselePri > 7)
   {
      RLOG1(L_DEBUG,"Validation failed for Geran cell configuration.Value of cellReselePri=%d",cellReselePri);
      ret = RFAILED;
   }

   if(threshXHigh > 31)
   {
      RLOG1(L_DEBUG,"Validation failed for Geran cell configuration.Value of threshXHigh=%d",threshXHigh);
      ret = RFAILED;
   }

   if(threshXLow > 31)
   {
      RLOG1(L_DEBUG,"Validation failed for Geran cell configuration.Value of threshXLow=%d",threshXLow);
      ret = RFAILED;
   }

   if(geranFreq->pMaxGeran.pres == PRSNT_NODEF)
   {
      if(geranFreq->pMaxGeran.val > 39)
      {
         RLOG2(L_DEBUG,"Validation failed for Geran cell configuration.Value of pMaxGeran.pres=%d,pMaxGeran.val=%d",
               geranFreq->pMaxGeran.pres,geranFreq->pMaxGeran.val);
         ret = RFAILED;
      }
   }
   RETVALUE(ret);
}

/** @brief This function is responsible for addition of a neighbour at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgNeighFreqAdd
 *
 *         Processing steps:
 *         - This is add Eutran frequencies to frequency List in Cell CB 
 *         - It allocates a new WrNeighFreqCb 
 *         - It copies all the configuration into the allocated WrNeighFreqCb
 *
 * @param[in] cellId     : cell for which NR needs to be added
 * @param[in] numFreq    : Number of frequency
 * @param[in] nbrFreqAdd : Neighbour parameters 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */

PRIVATE S16 wrEmmCfgNeighFreqAdd
(
 U8                               cellId,
 U8                               numFreq,
 LwrNeighFreqAddModCfg            *nbrFreqAdd
 )
{
   S16                           ret = LCM_REASON_NOT_APPL;
   WrCellCb                      *cellCb  = NULLP;
   U8                            freqIndex = 0;

   WR_GET_CELLCB(cellCb, cellId)

      if(NULLP == cellCb)
      {
         RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received while adding neighbour freq.");
         RETVALUE(RFAILED);
      }

   while(freqIndex < numFreq)
   {
      if(ROK != wrEmmCfgValidateNeighFreqAdd(cellId,
               &nbrFreqAdd[freqIndex]))
      {
         RLOG0(L_WARNING, "Validation failed while adding neighbour frequency");
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }

      switch(nbrFreqAdd[freqIndex].type)
      {
         case WR_NEIGH_CELL_EUTRA:
            {
               WrEutranFreqCb             *neighFreqCb = NULLP;

               /*Allocating Memory for the neighbour Control Block*/
               WR_ALLOC(&neighFreqCb, sizeof(WrEutranFreqCb));
               if (neighFreqCb == NULLP)
               {
                  RLOG0(L_FATAL,"Memory allocation failed while adding Eutra neighbour frequency.");
                  wrEmmFreeList(&cellCb->eutranNbrFreqLst, TRUE, 
                        sizeof(WrEutranFreqCb));
                  RETVALUE(LCM_REASON_MEM_NOAVAIL);
               }

               /* Copy the Freq Details */
               wrEmmCopyEutranNeighFreq(neighFreqCb, 
                     &nbrFreqAdd[freqIndex].t.eutranFreq);
               neighFreqCb->freqLstEnt.node = (PTR)neighFreqCb;
               /* Add to the Freq List in the Cell CB */
               cmLListAdd2Tail(&cellCb->eutranNbrFreqLst, 
                     &neighFreqCb->freqLstEnt);
               RLOG1(L_ALWAYS,"Frequency(dlEarfcn) added is %d",neighFreqCb->dlEarfcn);
            }
            break;
         case WR_NEIGH_CELL_UTRA_FDD:
            {
               WrUtraFreqCb 	    *neighFreqCb = NULLP;
               WrUtraFreqCb 	    *neighFreqCbTmp = NULLP;
               CmLList            *listNode = NULLP;

               /*Allocating Memory for the neighbour Control Block */
               WR_ALLOC(&neighFreqCb, sizeof(WrUtraFreqCb));
               if(neighFreqCb == NULLP) 		  	   
               {
                  RLOG0(L_FATAL,"Memory allocation failed while adding neighbour Utra Fdd frequency.");
                  wrEmmFreeList(&cellCb->utraFddNbrFreqLst, TRUE,
                        sizeof(WrUtraFreqCb)); 
                  RETVALUE(LCM_REASON_MEM_NOAVAIL);
               }

               /* Copy the Freq Details */
               wrEmmCopyUtranNeighFreq(neighFreqCb,
                     &nbrFreqAdd[freqIndex].t.utranFreq);
               /* Filling the List in the Order of Freq Priority */
               neighFreqCbTmp = (WrUtraFreqCb *)
                  CM_LLIST_FIRST_NODE(&cellCb->utraFddNbrFreqLst, listNode);
               while(neighFreqCbTmp != NULLP)
               {
                  if(neighFreqCbTmp->priority.val <= neighFreqCb->priority.val)
                  {
                     break;
                  }
                  neighFreqCbTmp = (WrUtraFreqCb *)
                     CM_LLIST_NEXT_NODE(&cellCb->utraFddNbrFreqLst,listNode);
               }
               neighFreqCb->freqLstEnt.node = (PTR)neighFreqCb;
               if(cmLListCrnt(&cellCb->utraFddNbrFreqLst) == NULLP)
               {
                  cmLListAdd2Tail(&cellCb->utraFddNbrFreqLst,
                        &neighFreqCb->freqLstEnt);
               }
               else
               {
                  cmLListInsCrnt (&cellCb->utraFddNbrFreqLst,
                        &neighFreqCb->freqLstEnt);
               }
            }	
            break;
         case WR_NEIGH_CELL_UTRA_TDD:
            {
               WrUtraFreqCb 	    *neighFreqCb = NULLP;
               WrUtraFreqCb 	    *neighFreqCbTmp = NULLP;
               CmLList            *listNode = NULLP;

               WR_ALLOC(&neighFreqCb, sizeof(WrUtraFreqCb));
               if(neighFreqCb == NULLP) 		  	   
               {
                  RLOG0(L_FATAL,"Memory allocation failed while adding neighbour Utra Tdd frequency");
                  wrEmmFreeList(&cellCb->utraTddNbrFreqLst, TRUE,
                        sizeof(WrUtraFreqCb)); 
                  RETVALUE(LCM_REASON_MEM_NOAVAIL);
               }
               /* Copy the Freq Details */
               wrEmmCopyUtranNeighFreq(neighFreqCb,
                     &nbrFreqAdd[freqIndex].t.utranFreq);
               CM_LLIST_FIRST_NODE(&cellCb->utraTddNbrFreqLst, listNode);
               while(listNode != NULLP)
               {
                  neighFreqCbTmp = (WrUtraFreqCb *) listNode->node;
                  if(neighFreqCbTmp->priority.val <= neighFreqCb->priority.val)
                  {
                     break;
                  }
                  CM_LLIST_NEXT_NODE(&cellCb->utraTddNbrFreqLst, listNode);
               }
               neighFreqCb->freqLstEnt.node = (PTR)neighFreqCb;
               if(cmLListCrnt(&cellCb->utraTddNbrFreqLst) == NULLP)
               {
                  cmLListAdd2Tail(&cellCb->utraTddNbrFreqLst,
                        &neighFreqCb->freqLstEnt);
               }
               else
               {
                  cmLListInsCrnt (&cellCb->utraTddNbrFreqLst,
                        &neighFreqCb->freqLstEnt);
               }
            }
            break;
         case WR_NEIGH_CELL_CDMA2K_1XRTT:
            {
               ret = wrEmmCfgNeighFreqAddCdma1xFreq(cellCb,
                     &nbrFreqAdd[freqIndex].t.cdma1xFreq);
               if(ret == RFAILED)
               {
                  RLOG0(L_WARNING, "Failed to add  CDMA 1x frequency ");
               }
            }
            break;
         case WR_NEIGH_CELL_GERAN:
            {
               WrGeranFreqCb 	    *neighFreqCb = NULLP;
               WrGeranFreqCb      *neighFreqCbtmp = NULLP;
               CmLList            *listNode = NULLP;

               /*Allocating Memory for the neighbour Control Block */
               WR_ALLOC(&neighFreqCb, sizeof(WrGeranFreqCb));
               if(neighFreqCb == NULLP) 		  	   
               {
                  RLOG0(L_FATAL,"Memory allocation failed while adding geran frequency.");
                  wrEmmFreeList(&cellCb->geranNbrFreqLst, TRUE,
                        sizeof(WrGeranFreqCb)); 
                  RETVALUE(LCM_REASON_MEM_NOAVAIL);
               }

               if(ROK != wrEmmCfgCellValidateGeranParam(&nbrFreqAdd[freqIndex].t.geranFreq))
               {
                  RLOG0(L_ERROR,"GERAN Cell level Configuration \
                        Paramter(s) are wrong \n");
                  RETVALUE(LCM_REASON_INVALID_PAR_VAL);
               }
               /* Copy the Freq Details */
               wrEmmCopyGeranNeighFreq(neighFreqCb,
                     &nbrFreqAdd[freqIndex].t.geranFreq);
               /* Add to the Freq List in the Cell CB */
               /* Neighbor frequency is inserted in descending order
                  from highest value lower as higher the number reselection
                  priority is high for that frequency */
               CM_LLIST_FIRST_NODE(&cellCb->geranNbrFreqLst,listNode);
               while(listNode != NULLP)
               {
                  neighFreqCbtmp = (WrGeranFreqCb *)listNode->node;
                  if(neighFreqCbtmp->cellReselPriority < neighFreqCb->cellReselPriority)
                  {
                     break;
                  }
                  CM_LLIST_NEXT_NODE(&cellCb->geranNbrFreqLst,listNode);
               }
               neighFreqCb->freqLstEnt.node = (PTR)neighFreqCb;
               if(cmLListCrnt(&cellCb->geranNbrFreqLst) == NULLP)
               {
                  cmLListAdd2Tail(&cellCb->geranNbrFreqLst, &neighFreqCb->freqLstEnt);
               }
               else
               {
                  /* add to previous of neighFreqCbtmp */
                  cmLListInsCrnt (&cellCb->geranNbrFreqLst, &neighFreqCb->freqLstEnt);
               }
            }
            break;
         default:
            ret = LCM_REASON_INVALID_PAR_VAL;
      }
      freqIndex++;
   }
   RETVALUE(ret);
}
/** @brief This function is responsible for modification of a neighbour at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmUpdateEutraNbrFreqObj
 *
 *         Processing steps:
 *         - Update the Structure 
 *
 * @param[in] lwrNbeEutra     : pointer to the Lwr Structure
 * @param[in] eutranNbrFreqLst : pointer to the neighbor frequency List 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */
PRIVATE S16 wrEmmUpdateEutraNbrFreqObj
(
 LwrNeighEutraFreq    *lwrNbeEutra, 
 CmLListCp            *eutranNbrFreqLst
 )
{
   WrEutranFreqCb             *neighFreqCb = NULLP;
   CmLList                    *neighFreqLnk = NULLP;

   neighFreqLnk = eutranNbrFreqLst->first;
   while(neighFreqLnk != NULLP)
   {
      neighFreqCb = (WrEutranFreqCb*)neighFreqLnk->node;
      if(neighFreqCb->dlEarfcn == lwrNbeEutra->earfcnDl) 
      {
         /* Modify the Freq Details */
         wrEmmCfgCellModNghFreq(neighFreqCb,lwrNbeEutra);
         break;
      }
      neighFreqLnk = neighFreqLnk->next;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for modification of a neighbour at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmUpdateUtraNbrFreqObj
 *
 *         Processing steps:
 *         - Update the Structure
 *
 * @param[in] lwrUtra     : pointer to the Lwr Structure
 * @param[in] utranNbrFreqLst : pointer to the neighbor frequency List
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */
PRIVATE S16 wrEmmUpdateUtraNbrFreqObj
(
 LwrNeighUtraFreq    *lwrUtra,
 CmLListCp            *utranNbrFreqLst,
 LwrNeighType         type
 )
{
   WrUtraFreqCb             *neighFreqCb = NULLP;
   CmLList                   *neighFreqLnk = NULLP;

   neighFreqLnk = utranNbrFreqLst->first;
   while(neighFreqLnk != NULLP)
   {
      if(WR_NEIGH_CELL_UTRA_FDD == type)
      {
         neighFreqCb = (WrUtraFreqCb*)neighFreqLnk->node;
         if(neighFreqCb->t.utraFddFreq.arfcnDl == lwrUtra->t.utraFdd.arfcnDl)
         {
            /* Copy the Freq Details */
            wrEmmCopyUtranNeighFreq(neighFreqCb, lwrUtra);  
            break;
         }
      }
		else if(WR_NEIGH_CELL_UTRA_TDD == type)
      {
         neighFreqCb = (WrUtraFreqCb*)neighFreqLnk->node;
         if(neighFreqCb->t.utraTddFreq.arfcn== lwrUtra->t.utraTdd.arfcn)
         {
            /* Copy the Freq Details */
            wrEmmCopyUtranNeighFreq(neighFreqCb, lwrUtra);  
            break;
         }
      }
      neighFreqLnk = neighFreqLnk->next;
   }
   RETVALUE(ROK);
}

/** @brief This function is validates the eNodeB re-configuration
 *
 * @details
 *
 *     Function: wrEmmCfgEnbModValidate
 *
 *         Processing steps:
 *         - This is validates the eNodeb Re-configuration 
 *
 * @param [in] cfgMod : EnodeB Configuration parameters
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 */
PRIVATE S16 wrEmmCfgEnbModValidate
(
LwrENodeBCfg                 *cfgMod
)
{
      RETVALUE(LCM_REASON_NOT_APPL);
}
/** @brief This function is responsible for setting the eNodeB Configuration.
 *
 * @details
 *
 *     Function: wrEmmCfgEnbMod
 *
 *         Processing steps:
 *         - This is set the eNodeb Re-configuration 
 *
 * @param [in] cfgMod : EnodeB Configuration parameters
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 */
PRIVATE S16 wrEmmCfgEnbMod
(
LwrENodeBCfg                 *cfgMod
)
{
   S16                       ret = LCM_REASON_NOT_APPL;
   U32                       rCfgType = 0;
   U32                       mask = 1;
   U8                        indx;
   U8                        newpwsEtwsCmasval = 0xff;
   U16                       numOfMmePriGrps = 0;
   Bool                      newAddition = FALSE;
   Bool                      trgS1andX2CfgUpd = FALSE;
   Bool                      peerRestart = FALSE;

   ret = wrEmmCfgEnbModValidate(cfgMod);
   if (LCM_REASON_NOT_APPL != ret)
   {
      RLOG1(L_ERROR,"Failed to validate eNodeB reconfiguration[%d]", ret);
      RETVALUE(ret);
   }
   
   /*Setting the eNodeB Re-configuration values*/
   rCfgType =  cfgMod->reCfgType;
   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_ENB_NUM_OF_UE:
            {
               rCfgType =  rCfgType ^ mask;

               RLOG_ARG2(L_DEBUG,DBG_ENBID, wrEmmCb.enbId, "ENB Modified "
                  "maxNumOfUe Changed, old maxNumOfUe [%d] new maxNumOfUe [%d]",
                  wrEmmCb.maxNumOfUe, cfgMod->maxNumOfUe);

               wrEmmCb.maxNumOfUe =  cfgMod->maxNumOfUe; 
               break;
            }
            case LWR_ENB_PROTOCFG:
            {
               rCfgType =  rCfgType ^ mask; 
               if (wrEmmCb.enbId != cfgMod->protoCfg.enbId)
               {
                  RLOG2(L_DEBUG,"enbId Modified. Old enbId [%d],New enbId[%d]",
                        wrEmmCb.enbId,cfgMod->protoCfg.enbId);
                  wrEmmCb.enbId = cfgMod->protoCfg.enbId;
                  peerRestart = TRUE;
               }
               if(wrEmmCb.enbType != cfgMod->protoCfg.enbType)
               {
                  RLOG2(L_DEBUG,"enbType Modified. Old enbType [%d],New enbType[%d]",
                        wrEmmCb.enbType,cfgMod->protoCfg.enbType);
                  wrEmmCb.enbType = (WrEnbType )cfgMod->protoCfg.enbType;
                  /* Trigger Partial_reset procedure */
                  peerRestart = TRUE;
               }
               if (FALSE == wrEmmPlmnPlmnsEqual(&(wrEmmCb.plmns.plmns[0].plmnInfo.plmnId),
                                               (WrPlmnId *)&(cfgMod->protoCfg.plmns[0])))
               {
                  RLOG3(L_DEBUG,"PLMNID modified.Old PLMNID[(MCC =%d%d%d)]",
                        wrEmmCb.plmns.plmns[0].plmnInfo.plmnId.mcc[0],
                        wrEmmCb.plmns.plmns[0].plmnInfo.plmnId.mcc[1],
                        wrEmmCb.plmns.plmns[0].plmnInfo.plmnId.mcc[2]);

                  RLOG3(L_DEBUG,"MNC[%d%d%d]", wrEmmCb.plmns.plmns[0].plmnInfo.plmnId.mnc[0],
                                               wrEmmCb.plmns.plmns[0].plmnInfo.plmnId.mnc[1],
                                               wrEmmCb.plmns.plmns[0].plmnInfo.plmnId.mnc[2]);
                  RLOG3(L_DEBUG,"New PLMNID[MCC=%d%d%d]",cfgMod->protoCfg.plmns[0].plmnId.mcc[0],
                        cfgMod->protoCfg.plmns[0].plmnId.mcc[1],cfgMod->protoCfg.plmns[0].plmnId.mcc[2]);
                  RLOG3(L_DEBUG,"MNC[%d%d%d]",cfgMod->protoCfg.plmns[0].plmnId.mnc[0],
                                             cfgMod->protoCfg.plmns[0].plmnId.mnc[1],
                                             cfgMod->protoCfg.plmns[0].plmnId.mnc[2]);
                  cmMemcpy((U8 *)&(wrEmmCb.plmns.plmns[0].plmnInfo.plmnId),
                           (U8 *)&(cfgMod->protoCfg.plmns[0]),
                           sizeof(LwrPlmnId));  
                  /* Trigger Partial_reset procedure */
                  peerRestart = TRUE;
               }
               wrEmmCb.bhLinkCapacity = cfgMod->protoCfg.bhLinkCapacity; 
               if (cmMemcmp((U8 *)&wrEmmCb.enbName, 
                             (U8 *)&cfgMod->protoCfg.enbName.val, 
                             cfgMod->protoCfg.enbName.len))
               {
                  cmMemcpy((U8 *)wrEmmCb.enbName, 
                           (U8 *)cfgMod->protoCfg.enbName.val, 
                           cfgMod->protoCfg.enbName.len);
                  wrEmmCb.enbNameLen = cfgMod->protoCfg.enbName.len;
                  /* ccpu00140863 */
                  WR_FREE(cfgMod->protoCfg.enbName.val,
                        cfgMod->protoCfg.enbName.len);
                  /* Trigger SIB9 Update in future */
                  /* Tirgger S1 and X2 Configuration Update Messages */
                  trgS1andX2CfgUpd = TRUE;
               }
               break;
            } 
            case LWR_ENB_ANR_ALGO_INFO:
            {
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            case LWR_ENB_NUM_MME_PRI_GRPS:
            {
               rCfgType =  rCfgType ^ mask; 
               numOfMmePriGrps = cfgMod->numOfMmePriGrps; 
               newAddition = TRUE;
               break;
            }
            case LWR_ENB_MME_LDBAL_PRI_GRPS_LST:
            {
               rCfgType =  rCfgType ^ mask;
               wrEmmProcEnbModMmeLdBalPriGrpsLst(cfgMod, &numOfMmePriGrps,
                                              newAddition);
               break;
            }
            case LWR_ENB_PWS_ENABLE_DISABLE:
            {
               rCfgType = rCfgType ^ mask;
               
               if (wrEmmCb.pwsEtwsCmas != cfgMod->pwsEtwsCmas)
               {             
                  RLOG2(L_DEBUG,"Modified pwsEtwsCmas.Old pwsEtwsCmas[%d],New pwsEtwsCmas[%d]",
                        wrEmmCb.pwsEtwsCmas,cfgMod->pwsEtwsCmas);
                  newpwsEtwsCmasval = cfgMod->pwsEtwsCmas;                   
               }
               else
               {
                    ret = RFAILED;
                    RLOG0(L_WARNING, "Same as old eNodeB configuration recieved \n");
               }
               break;
            }  

            default:
            {
               rCfgType =  rCfgType ^ mask; 
               RLOG1(L_WARNING, "eNodeB reconfiguration received unknown reCfgType[%ld]",
                  (cfgMod->reCfgType));
               break;
            }
         } /* end of switch */
      } /* end of if */
      mask = mask << 1;
   }
   if (trgS1andX2CfgUpd)
   {
      /* Trigger Cfg Upd to all neighbor eNB's */
      ret = wrEmmNghTrigCfgUpd(wrEmmCb.cellCb[0]);
      if ( ret != ROK )
      {
         RLOG0(L_ERROR,"Failed to trigger eNB configuration update message for modifying eNB configuration");
         RETVALUE(RFAILED);
      }
      /* Trigger Cfg Upd to all MME's */
      for ( indx = 0; indx < wrEmmCb.mmeCont.numMmes; indx++ )
      {
         if(( wrEmmCb.mmeCont.mmes[indx] != NULLP) &&
           ( wrEmmCb.mmeCont.mmes[indx]->state == WR_MME_UP))
         {
            ret = wrEmmMmeSendCfgUpdate(wrEmmCb.mmeCont.mmes[indx]->mmeId);
            if ( ret != ROK )
            {
              RLOG0(L_ERROR,"Failed to send eNodeB configuration update to MME");
              RETVALUE(RFAILED);
            }
         }
      }
   }
   if(peerRestart)
   {
      ret = wrEmmMmePeerRestart(wrEmmCb.cellCb[0]->cellId);
      if (ROK != ret)
      {
         RLOG1(L_ERROR,"Failed to send peer restart to MME while modifying eNB configuration [%d]",ret);
      }
   }
   if(newpwsEtwsCmasval != 0xff)
   {
       if (RFAILED == wrEmmupdatePwsConfig(newpwsEtwsCmasval, wrEmmCb.pwsEtwsCmas))
       {
           ret = RFAILED;
           RLOG0(L_ERROR, "PWS New configuration failed to be applied"); 
       }
       wrEmmCb.pwsEtwsCmas = newpwsEtwsCmasval;  
   }
   RETVALUE(ret);
}

/** @brief This function is responsible for modifying MME Load Balancing 
 *          configuration in EMM Cb.
 *
 * @details
 *
 *     Function: wrEmmProcEnbModMmeLdBalPriGrpsLst
 *
 *         Processing steps:
 *         - Update the Structure 
 *
 * @param[in] cfgMod     : Pointer to the configuration received 
 * @param[in] numOfMmePriGrps : Number of MME Primary Groups.
 * @param [in] newAddition: Flag to indicate new addition or modification.
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */
PRIVATE S16 wrEmmProcEnbModMmeLdBalPriGrpsLst
(
 LwrENodeBCfg         *cfgMod,
 U16                  *numOfMmePriGrps,
 Bool                 newAddition
)
{
   U16                        idx;
   U16                        idx1;
   Bool                       matchFound  = FALSE; 

   *numOfMmePriGrps = cfgMod->numOfMmePriGrps; 
   for(idx = 0; idx < *numOfMmePriGrps; idx++)
   {
      for(idx1 = 0; idx1 < wrEmmCb.numOfMmePriGrps; idx1++)
      {
         if(cfgMod->mmeLdBalCfg[idx].mmeGroupId == 
               wrEmmCb.mmeLoadBalCfg[idx1].mmeGroupId)
         {
            RLOG2(L_DEBUG,"Old mmeGrpPrior[%d],New mmeGrpPrior[%d]",
                  wrEmmCb.mmeLoadBalCfg[idx1].mmeGrpPrior,cfgMod->mmeLdBalCfg[idx].mmeGrpPrior);
            wrEmmCb.mmeLoadBalCfg[idx1].mmeGrpPrior = 
               cfgMod->mmeLdBalCfg[idx].mmeGrpPrior;
            matchFound = TRUE;
         }
      }

      /* if not found then only add it in the mmeLoadBalCfg array */
      if((matchFound == FALSE))
      {
         if(wrEmmCb.numOfMmePriGrps < LWR_SM_MAX_MMES)
         {
            wrEmmCb.mmeLoadBalCfg[wrEmmCb.numOfMmePriGrps].mmeGroupId =
               cfgMod->mmeLdBalCfg[idx].mmeGroupId;

            wrEmmCb.mmeLoadBalCfg[wrEmmCb.numOfMmePriGrps].mmeGrpPrior =
               cfgMod->mmeLdBalCfg[idx].mmeGrpPrior;

            wrEmmCb.numOfMmePriGrps++;
         }
      }
      matchFound = FALSE;
   }
   RETVALUE(ROK);
}

/* @brief This function is used to process the Cell Reconfiguration Indication
 * recieved from RRM layer. This may change the information like SIB2 info and
 * need to be processed to reflect the changes at FSM and other layers.
 *
 * @details
 *
 *     Function: wrEmmProcRmCellRecfgInd
 *
 *         Processing steps:
 *
 * @param[in] rrmWrCellRecfgInd: The pointer to Cell reconfiguration structure.
 * 
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmProcRmCellRecfgInd
(
RmuCellRecfgInd         *rrmWrCellRecfgInd
)
{
   U8                        sibs[WR_MAX_SIBS];
   U8                        numSibs            = 0;
   S16                       ret                = RFAILED;
   WrCellCb                  *cellCb            = NULLP;
   WrSib2AcBarringCfg        *sib2DataCall    = NULLP;
   WrSib2AcBarringCfg        *sib2Signalling  = NULLP;

   CmRrmAcb    *rrmMoDtBarring    =
      &(rrmWrCellRecfgInd->stAcbParamInd.stAcbInfo.stAcBarringMoData);

   CmRrmAcb    *rrmMoSigBarring   =
      &(rrmWrCellRecfgInd->stAcbParamInd.stAcbInfo.stAcBarringMoSig);

   TRC3(wrEmmProcRmCellRecfgInd);

   WR_GET_CELLCB(cellCb, rrmWrCellRecfgInd->bCellId);
   if(cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,rrmWrCellRecfgInd->bCellId,"InValid CellId");
      RETVALUE(RFAILED);
   }

   sib2DataCall = &(cellCb->sib2.barringInfo.dataCall);
   sib2Signalling = &(cellCb->sib2.barringInfo.signalling);


   /* Copy access baring information for MO-Data if acbFactor is not 100% and
    * barring info is present.
    */
   if (rrmWrCellRecfgInd->stAcbParamInd.stAcbInfo.bIsacBarringMoData == TRUE)
   {
      if (rrmMoDtBarring->enAcbFactor != RM_ACB_FACTOR_NA)
      {
         sib2DataCall->pres = TRUE;
         sib2DataCall->acBarringFactor = rrmMoDtBarring->enAcbFactor;
         sib2DataCall->acBarringTime = rrmMoDtBarring->enAcbTime;
         sib2DataCall->acBarringForSpecialAC = rrmMoDtBarring->bAcbForSpac;
         sib2DataCall->isRrmCfgPres = TRUE;
      }
      else
      {
         if (sib2DataCall->acBarringForSpecialAC)
         {
            sib2DataCall->acBarringFactor = sib2DataCall->lwrCfgAcbFactor;
            sib2DataCall->acBarringTime = sib2DataCall->lwrCfgAcbTime;
         }
         else
         {
            sib2DataCall->pres = FALSE;
         }
         sib2DataCall->isRrmCfgPres = FALSE;
      }
   }
   else
   {
            sib2DataCall->pres = FALSE;
            sib2DataCall->isRrmCfgPres = FALSE;
   }

   /* Copy access baring information for MO-Signalling if acbFactor is not
    * 100% and barring info is present.
    */
   if (rrmWrCellRecfgInd->stAcbParamInd.stAcbInfo.bIsacBarringMoSig == TRUE)
   {
      if (rrmMoSigBarring->enAcbFactor != RM_ACB_FACTOR_NA)
      {
         sib2Signalling->pres = TRUE;
         sib2Signalling->acBarringFactor = rrmMoSigBarring->enAcbFactor;
         sib2Signalling->acBarringTime   = rrmMoSigBarring->enAcbTime;
         sib2Signalling->acBarringForSpecialAC = rrmMoSigBarring->bAcbForSpac;
         sib2Signalling->isRrmCfgPres = TRUE;
      }
      else
      {
         if (sib2Signalling->acBarringForSpecialAC)
         {
            sib2Signalling->acBarringFactor = sib2Signalling->lwrCfgAcbFactor;
            sib2Signalling->acBarringTime = sib2Signalling->lwrCfgAcbTime;
         }
         else
         {
            sib2Signalling->pres = FALSE;
         }
         sib2Signalling->isRrmCfgPres = FALSE;
      }
   }
   else
   {
            sib2Signalling->pres = FALSE;
            sib2Signalling->isRrmCfgPres = FALSE;
   }
   
   cellCb->sib2.barringInfo.emergency = 
      rrmWrCellRecfgInd->stAcbParamInd.stAcbInfo.bAcBarringEmergency;

   /* Update the PBM to indicate that SIB2 is modified */
   /* Updating in AC barring information */
   if (cellCb->sib2.barringInfo.emergency ||
       sib2Signalling->pres || sib2DataCall->pres)
   {
       cellCb->sib2.barringInfoPres = TRUE;
   }
   else
   {
       cellCb->sib2.barringInfoPres = FALSE;
   }
   if(numSibs < WR_MAX_SIBS) /* KW FIX*/ 
   sibs[numSibs++] = WR_SIB_2;

   if(WR_CELL_STATE_RESET != cellCb->cellState)
   {
      /* Update the sibs which needs to broadcast */
      ret = wrPbmAddOrUpdateSibs(numSibs, sibs, cellCb->cellId);

      if (ret == RFAILED)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,"Failed to Add or Update Sibs.numSibs[%d]"
            ,numSibs);
      }
   }

   RETVALUE(ret);
}/* wrEmmProcRmCellRecfgInd */


/** @brief This function is responsible for modification of a neighbour at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgNeighFreqMod
 *
 *         Processing steps:
 *         - This is modify Eutran frequencies to frequency List in Cell CB 
 *         - It allocates a new WrNeighFreqCb 
 *         - It copies all the configuration into the allocated WrNeighFreqCb
 *
 * @param[in] cellId     : cell for which NR needs to be added
 * @param[in] numFreq    : number of frequency 
 * @param[in] nbrFreqMod : Neighbour parameters 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */

PRIVATE S16 wrEmmCfgNeighFreqMod
(
 U8                               cellId,
 U8                               numFreq,
 LwrNeighFreqAddModCfg            *nbrFreqMod
 )
{
   WrCellCb          *cellCb  = NULLP;
   S16               ret = LCM_REASON_NOT_APPL;
   U8                            freqIndex = 0;
   U8                            pbmMask = 0;
   Bool                          isIntraPres = FALSE;
   Bool                          isInterPres = FALSE;
   Bool                         isUtraPres = FALSE;

   WR_GET_CELLCB(cellCb, cellId)

   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received while modifying neighbour frequency"
            "configurations.");
      RETVALUE(RFAILED);
   }

   while(freqIndex < numFreq)
   {
      switch(nbrFreqMod[freqIndex].type)
      {
         case WR_NEIGH_CELL_EUTRA:
            {
               LwrNeighEutraFreq          *lwrNbeEutra = NULLP;
               U16                        servArfcn;

               servArfcn = wrEmmGetServCarrierFreq(cellId);
               lwrNbeEutra = &nbrFreqMod[freqIndex].t.eutranFreq;
               wrEmmUpdateEutraNbrFreqObj(lwrNbeEutra, &cellCb->eutranNbrFreqLst);
               if(lwrNbeEutra->earfcnDl == servArfcn)
               {
                  isIntraPres = TRUE;
               }
               else
               {
                  isInterPres = TRUE;
               }
            }
            break;
         case WR_NEIGH_CELL_UTRA_FDD:
            {
               LwrNeighUtraFreq          *lwrNbeUtra = NULLP;
               lwrNbeUtra = &nbrFreqMod[freqIndex].t.utranFreq;
               wrEmmUpdateUtraNbrFreqObj(lwrNbeUtra, &cellCb->utraFddNbrFreqLst, WR_NEIGH_CELL_UTRA_FDD);
               isUtraPres = TRUE;
            }
            break;
			case WR_NEIGH_CELL_UTRA_TDD:
            {
               LwrNeighUtraFreq          *lwrNbeUtra = NULLP;
               lwrNbeUtra = &nbrFreqMod[freqIndex].t.utranFreq;
               wrEmmUpdateUtraNbrFreqObj(lwrNbeUtra, &cellCb->utraTddNbrFreqLst, WR_NEIGH_CELL_UTRA_TDD);
               isUtraPres = TRUE;
            }
            break;
         default:
            ret = LCM_REASON_INVALID_PAR_VAL;
      }
      freqIndex++;
   }
   if(isIntraPres == TRUE)
   {
      pbmMask = pbmMask | WR_ANR_INTRA_FREQ_SIB3_BIT;
   }
   if(isInterPres == TRUE)
   {
      pbmMask = pbmMask | WR_ANR_INTER_FREQ_BIT;
   }
   if(isUtraPres == TRUE)
   {
      pbmMask = pbmMask | WR_ANR_UTRA_FREQ_BIT;   
   }
   if(pbmMask != 0)
   {
      wrEmmPbmTrgNrUpdt(cellId, pbmMask);
   }
   RETVALUE(ret);
}

/** @brief This function is responsible for deletion of a neighbour at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmDelEutraNbrFreqObj
 *
 *         Processing steps:
 *         - Update the Structure 
 *
 * @param[in] lwrNbeEutra     : pointer to the Lwr Structure
 * @param[in] eutranNbrFreqLst : pointer to the neighbor frequency List 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */
PRIVATE S16 wrEmmDelEutraNbrFreqObj
(
 LwrNeighEutraDelInfo   *lwrNbeEutra, 
 CmLListCp              *eutranNbrFreqLst
 )
{
   WrEutranFreqCb             *neighFreqCb = NULLP;
   CmLList                    *neighFreqLnk = NULLP;

   neighFreqLnk = eutranNbrFreqLst->first;
   while(neighFreqLnk != NULLP)
   {
      neighFreqCb = (WrEutranFreqCb*)neighFreqLnk->node;
      if(neighFreqCb->dlEarfcn == lwrNbeEutra->earfcn) 
      {
         /* Delete the Freq Details */
         RLOG1(L_INFO,"Deleted Eutra neighbour frequency is [%d]",neighFreqCb->dlEarfcn);
         cmLListDelFrm (eutranNbrFreqLst, neighFreqLnk);
         WR_FREE(neighFreqCb, sizeof(WrEutranFreqCb));
         break;
      }
      neighFreqLnk = neighFreqLnk->next;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for deletion of a neighbour at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmDelUtraNbrFreqObj
 *
 *         Processing steps:
 *         - Update the Structure
 *
 * @param[in] lwrNbeUtra     : pointer to the Lwr Structure
 * @param[in] utranNbrFreqLst : pointer to the neighbor frequency List
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */

PRIVATE S16 wrEmmDelUtraNbrFreqObj
(
 LwrNeighUtraDelInfo*   lwrNbeUtra,
 CmLListCp              *utranNbrFreqLst
 )
{
   WrUtraFreqCb               *neighFreqCb = NULLP;
   CmLList                    *neighFreqLnk = NULLP;

   neighFreqLnk = utranNbrFreqLst->first;
   while(neighFreqLnk != NULLP)
   {
      neighFreqCb = (WrUtraFreqCb*)neighFreqLnk->node;
      if(neighFreqCb->t.utraFddFreq.arfcnDl == lwrNbeUtra->arfcn)
      {
         /* Delete the Freq Details */
         RLOG1(L_INFO,"Deleted Utra Nbr Freq [%d]",neighFreqCb->t.utraFddFreq.arfcnDl);
         cmLListDelFrm (utranNbrFreqLst, neighFreqLnk);
         WR_FREE(neighFreqCb, sizeof(WrUtraFreqCb));
         break;
      }
      neighFreqLnk = neighFreqLnk->next;
   }
   RETVALUE(ROK);
}


/** @brief This function is responsible for deletion of a neighbour at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmDelUtraTddNbrFreqObj
 *
 *         Processing steps:
 *         - Update the Structure
 *
 * @param[in] lwrNbeUtra     : pointer to the Lwr Structure
 * @param[in] utranNbrFreqLst : pointer to the neighbor frequency List
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */

PRIVATE S16 wrEmmDelUtraTddNbrFreqObj
(
 LwrNeighUtraDelInfo*   lwrNbeUtra,
 CmLListCp              *utranNbrFreqLst
 )
{
   WrUtraFreqCb               *neighFreqCb = NULLP;
   CmLList                    *neighFreqLnk = NULLP;

   neighFreqLnk = utranNbrFreqLst->first;
   while(neighFreqLnk != NULLP)
   {
      neighFreqCb = (WrUtraFreqCb*)neighFreqLnk->node;
      if(neighFreqCb->t.utraTddFreq.arfcn == lwrNbeUtra->arfcn)
      {
         /* Delete the Freq Details */
         RLOG1(L_INFO,"Deleted Utra Tdd neighbour frequency [%d]",neighFreqCb->t.utraTddFreq.arfcn);
         cmLListDelFrm (utranNbrFreqLst, neighFreqLnk);
         WR_FREE(neighFreqCb, sizeof(WrUtraFreqCb));
         break;
      }
      neighFreqLnk = neighFreqLnk->next;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for deletion of a neighbour at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgNeighFreqDel
 *
 *         Processing steps:
 *         - This is modify Eutran frequencies to frequency List in Cell CB 
 *         - It allocates a new WrNeighFreqCb 
 *         - It copies all the configuration into the allocated WrNeighFreqCb
 *
 * @param[in] cellId     : cell for which NR needs to be added
 * @param[in] numFreq    : number of frequency
 * @param[in] nbrFreqDel : Neighbour parameters 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */

PRIVATE S16 wrEmmCfgNeighFreqDel
(
 U8                               cellId,
 U8                               numFreq,
 LwrNeighFreqDelCfg               *nbrFreqDel
 )
{
   S16                           ret = LCM_REASON_NOT_APPL;
   WrCellCb                      *cellCb  = NULLP;
   U8                            idCnt = 0;

   WR_GET_CELLCB(cellCb, cellId)

   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received while deleting neighbour frequency.");
      RETVALUE(RFAILED);
   }

   for (; idCnt < numFreq; idCnt++)
   {
      switch(nbrFreqDel[idCnt].type)
      {
         case WR_NEIGH_CELL_EUTRA:
         {
            LwrNeighEutraDelInfo       *lwrNbeEutra = NULLP;
            lwrNbeEutra = &nbrFreqDel[idCnt].t.eutranFreq;
            wrEmmDelEutraNbrFreqObj(lwrNbeEutra, &cellCb->eutranNbrFreqLst);
         }
         break;

         case WR_NEIGH_CELL_UTRA_FDD:
         {
            LwrNeighUtraDelInfo      *lwrNbUtra = NULLP;   
            lwrNbUtra = &nbrFreqDel[idCnt].t.utranFreq;
            wrEmmDelUtraNbrFreqObj(lwrNbUtra, &cellCb->utraFddNbrFreqLst);
         }
         break;
         case WR_NEIGH_CELL_UTRA_TDD:
         {
            LwrNeighUtraDelInfo      *lwrNbUtra = NULLP;   
            lwrNbUtra = &nbrFreqDel[idCnt].t.utranFreq;
            wrEmmDelUtraTddNbrFreqObj(lwrNbUtra, &cellCb->utraTddNbrFreqLst);
         }
         break;
         default:
            ret = LCM_REASON_INVALID_PAR_VAL;
         }
      }

   RETVALUE(ret);
}




/** @brief This function is responsible for addition of a neighbour at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgNeigAdd
 *
 *         Processing steps:
 *         - This is add a neighbour to the EnodeB 
 *         - It allocates a new WrNeighEnbCb 
 *         - It copies all the configuration into the allocated WrNeighEnbCb
 *
 * @param[in] numEnb : Number of EnodeB 
 * @param[in] nehAdd : Neighbour parameters 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */

PRIVATE S16 wrEmmCfgNeigAdd
(
 U8                           numEnb,
 LwrNeighAddModCfg            *nehAdd,
 U16                          cellId
 )
{
   U8                        idCnt = 0;
   WrNeighEnbCb              *neighCb = NULLP;

   for(; idCnt < numEnb; idCnt++)
   {
      /*Allocating Memory for the neighbour Control Block*/
      WR_ALLOC(&neighCb, sizeof(WrNeighEnbCb));
      if (neighCb == NULLP)
      {
         RLOG0(L_FATAL,"Memory allocation failed while adding neighbour.");
         RETVALUE(LCM_REASON_MEM_NOAVAIL);
      }
      /*Filling the received values*/
      neighCb->enbId = nehAdd[idCnt].enbId;
      neighCb->cellId = cellId;
      neighCb->enbType = nehAdd[idCnt].enbType;
      neighCb->x2Connection =  nehAdd[idCnt].state;
      neighCb->neighEnbAddr = nehAdd[idCnt].ipAddr;
      neighCb->secIpAddr = nehAdd[idCnt].secIpAddr;
      neighCb->peerId = nehAdd[idCnt].peerId;
      
      cmLListInit(&(neighCb->srvdCellLst));
      cmInitTimers(&(neighCb->timer), 1);
      cmInitTimers(&(neighCb->enbCfgUpTm), 1);
      cmInitTimers(&(neighCb->enbcfgTransTmr), 1);
      neighCb->rstRtxCnt = 0;
      neighCb->cfgUpdRtxCnt = 0;
      neighCb->x2SetupRtxCnt = 0;
      neighCb->isX2Reqd = nehAdd[idCnt].isX2Reqd; 

/* LTE_ADV_DSFR starts */
      neighCb->dsfrCEFreqRange = nehAdd[idCnt].dsfrCEFreqRange;
/* LTE_ADV_DSFR ends */
      neighCb->cfgUpdRtxCnt = 0;
      neighCb->x2SetupRtxCnt = 0;
      neighCb->alarmVal.maxErrIndCnt = nehAdd[idCnt].alarmVal.maxErrIndCnt;
      neighCb->alarmVal.maxResetCnt = nehAdd[idCnt].alarmVal.maxResetCnt;
      neighCb->alarmVal.minErrIndTime = nehAdd[idCnt].alarmVal.minErrIndTime;
      neighCb->alarmVal.minResetTime = nehAdd[idCnt].alarmVal.minResetTime;
      neighCb->enbCfgTransTriggerd = FALSE;
      /*Adding the  neighbour CB into hash list*/       
      if( cmHashListInsert(&(wrEmmCb.neighEnbLstCp),
               (PTR)neighCb,(U8 *)&neighCb->enbId, sizeof(neighCb->enbId)) != ROK)
      {
         RLOG0(L_ERROR,"cmHashListFind failed");
         WR_FREE(neighCb,sizeof(WrNeighEnbCb));
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for modification of a neighbour at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgNeigMod
 *
 *         Processing steps:
 *         - This is Mod a neighbour to the EnodeB 
 *         - It allocates a new WrNeighEnbCb 
 *         - It copies all the configuration into the allocated WrNeighEnbCb
 *
 * @param[in] numEnb : Number of EnodeB 
 * @param[in] nrEnbMod : Neighbour parameters 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */

PRIVATE S16 wrEmmCfgNeigMod
(
 U8                           numEnb,
 LwrNeighAddModCfg            *nrEnbMod
 )
{
   U8                        idCnt = 0;
   WrNeighEnbCb              *neighCb = NULLP;

   for(; idCnt < numEnb; idCnt++)
   {
      if(ROK != cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&nrEnbMod[idCnt].enbId,
               sizeof(U32), 0,(PTR*)&neighCb))
      {
         RLOG0(L_ERROR,"cmHashListFind failed");
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }

      RLOG2(L_DEBUG,"Old enbId[%d],New enbId[%d]",neighCb->enbId,nrEnbMod[idCnt].enbId);
      RLOG2(L_DEBUG,"Old enbType[%d],New enbType[%d]",neighCb->enbType,nrEnbMod[idCnt].enbType);
      RLOG2(L_DEBUG,"Old x2Connection[%d],New x2Connection[%d]",neighCb->x2Connection,nrEnbMod[idCnt].state);
      if(neighCb->neighEnbAddr.type == CM_NETADDR_IPV4)
      {
         RLOG2(L_DEBUG,"Old neighEnbAddr[%d],New neighEnbAddr[%d]",
               neighCb->neighEnbAddr.u.ipv4TptAddr.address,nrEnbMod[idCnt].ipAddr.u.ipv4TptAddr.address);
      }
      else
      {
         RLOG_STR(L_DEBUG,"Old neighEnbAddr[%s]",(CONSTANT S8*)neighCb->neighEnbAddr.u.ipv6TptAddr.ipv6NetAddr);
         RLOG_STR(L_DEBUG,"New neighEnbAddr[%s]",(CONSTANT S8*)nrEnbMod[idCnt].ipAddr.u.ipv6TptAddr.ipv6NetAddr);
      
      }
      RLOG2(L_DEBUG,"Old isX2Reqd[%d],New isX2Reqd[%d]",neighCb->isX2Reqd,nrEnbMod[idCnt].isX2Reqd);

      /*Filling the received values*/
      neighCb->enbId = nrEnbMod[idCnt].enbId;
      neighCb->enbType = nrEnbMod[idCnt].enbType;
      neighCb->x2Connection =  nrEnbMod[idCnt].state;
      neighCb->neighEnbAddr = nrEnbMod[idCnt].ipAddr;
      neighCb->secIpAddr = nrEnbMod[idCnt].secIpAddr;
      neighCb->isX2Reqd = nrEnbMod[idCnt].isX2Reqd; 
      /* TODO: needs to update X2  */
/* LTE_ADV_DSFR starts */
      neighCb->dsfrCEFreqRange = nrEnbMod[idCnt].dsfrCEFreqRange;
/* LTE_ADV_DSFR ends */
   }
   RETVALUE(ROK);
}



/** @brief This function is responsible for finding the served neighbour cell CB
 *
 * @details
 *
 *     Function: wrEmmDelNbrEnbServedCellList
 *
 *         Processing steps:
 *         - iterate the served cell list and get the served neighbour cell CB
 *         - if pci matches 
 *         - remove node from list
 *         - return 
 *
 * @param [in] neignEnb : neighbour eNodeB Control block
 * @param[out] pci : physical cell id      
 * @return S16
 *      -# Success  : ROK
 *      -# Failure    : RFAILED
 */
PUBLIC  S16 wrEmmDelNbrEnbServedCellList
(
   WrNeighEnbCb                *neignEnb
)
{
   S16                        ret = ROK;
   CmLList                    *nbrLnk =NULLP;
   CmLList                    *nbrNbrLnk =NULLP;
   WrEutranNeighCellCb        *eutranNeighCell = NULLP;
   WrNeighNeighCb             *neighNeighCb = NULLP;
   U8                         cellIdx = 0; /* First cell */

   /* Call Anr to initialize the ANR-MEAS interface data structure 
    * to send the NR update info to Measurement 
    */
   for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
   {
      wrEmmAnrInitMeasNrUpdtInfo(wrEmmCb.cellCb[cellIdx]);
   }

   CM_LLIST_FIRST_NODE(&(neignEnb->srvdCellLst), nbrLnk);

   while(NULLP != nbrLnk)
   {
      eutranNeighCell =(WrEutranNeighCellCb *)nbrLnk->node;

      cmLListDelFrm(&(neignEnb->srvdCellLst),nbrLnk);

      /*Deleting all the neighbours neighbour node and list*/
      CM_LLIST_FIRST_NODE(&eutranNeighCell->neighNeighLstCp, nbrNbrLnk);

      while(NULLP != nbrNbrLnk)
      {
         neighNeighCb =(WrNeighNeighCb *)nbrNbrLnk->node;
         if (cmLListDelFrm(&(eutranNeighCell->neighNeighLstCp),
                  (CmLList *)nbrNbrLnk) != (CmLList  *)neighNeighCb)
         {
            RLOG0(L_ERROR,"neighbours neighbour control block deletion failed");
            RETVALUE(RFAILED);
         }
         WR_FREE(neighNeighCb,sizeof(WrNeighNeighCb));
         /*Moving next node to delete*/

         CM_LLIST_NEXT_NODE(&eutranNeighCell->neighNeighLstCp,nbrNbrLnk);
      }

      for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
      {
         wrEmmAnrDelEutranNr(wrEmmCb.cellCb[cellIdx], eutranNeighCell);
      }
      
      WR_FREE(eutranNeighCell,sizeof(WrEutranNeighCellCb));

      CM_LLIST_NEXT_NODE(&(neignEnb->srvdCellLst), nbrLnk);
   }

   /* Process NR Update Info and send Trigger to Meas and PBM module */
   for ( cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++ )
   {
      ret = wrEmmAnrProcNrUpdtInfo(wrEmmCb.cellCb[cellIdx], FALSE);
   }

   RETVALUE(ret);
}/* End of wrEmmDelNbrEnbServedCellList */



/** @brief This function is responsible for deletion of a neighbour at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgNeigDel
 *
 *         Processing steps:
 *         - This is Del a neighbour to the EnodeB 
 *         - It allocates a new WrNeighEnbCb 
 *         - It copies all the configuration into the allocated WrNeighEnbCb
 *
 * @param[in] cellId : cell Identifier 
 * @param[in] numEnb : Number of EnodeB 
 * @param[in] nrEnbDel : Neighbour parameters 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
 
PRIVATE S16 wrEmmCfgNeigDel
(
U16                          cellId,
U8                           numEnb,
LwrNeighDel                  *nrEnbDel
)
{
   U8                        idCnt = 0;
   WrNeighEnbCb              *neighCb = NULLP;
   WrCellCb                  *cellCb = NULLP;
   
   WR_GET_CELLCB(cellCb, cellId)

   if(NULLP == cellCb)
   {
      RLOG1(L_ERROR,"Invalid cellId received for neighbour cell deletion [%d].",cellId);
      RETVALUE(RFAILED);
   }

      
   for(; idCnt < numEnb; idCnt++)
   {
      if(ROK != cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8*)&nrEnbDel[idCnt].enbId, 
               sizeof(U32), 0,(PTR*) &neighCb))
      {
         RLOG1(L_ERROR,"Failed to find the neighbour cell.EnbId = %d",nrEnbDel[idCnt].enbId);
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
      
      /* List of serving cells to be deleted */
      wrEmmDelNbrEnbServedCellList(neighCb);
      
      cmHashListDelete( &(wrEmmCb.neighEnbLstCp), (PTR)neighCb);
      WR_CLEAR_X2AP_PEER_ID(neighCb->peerId);

      WR_FREE(neighCb, sizeof(WrNeighEnbCb));
   }

   wrEmmNghTrigCfgUpd(cellCb);

   RETVALUE(ROK);
}

/** @brief This function is responsible for setting the eNodeB Configuration.
 *
 * @details
 *
 *     Function: wrEmmCfgEnbAdd
 *
 *         Processing steps:
 *         - This is set the eNodeb Configuration 
 *         - This also setup the enb at RRM
 *
 * @param [in] cfgAdd : EnodeB Configuration parameters
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 */
PRIVATE S16 wrEmmCfgEnbAdd
(
LwrENodeBCfg                 *cfgAdd
)
{
   S16                       ret = LCM_REASON_NOT_APPL;
   U32                       idx = 0;
   U8                        ratIdx = 0;
#ifndef RM_INTF
   RrmWrEnbCfgReqInfo        *rrmWrEnbCfgReq = NULLP;

   /*Validating the MME Load balance cfg array index*/
   if (cfgAdd->numOfMmePriGrps >= LWR_SM_MAX_MMES)
   {
      RLOG1(L_WARNING,"Invalid number of MME primary groups received [%d]",
         cfgAdd->mmeLdBalCfg);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   WR_ALLOC ( &rrmWrEnbCfgReq, sizeof(RrmWrEnbCfgReqInfo));
   if (rrmWrEnbCfgReq == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed for adding eNodeB configuration parameters.");
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }
   
   rrmWrEnbCfgReq->cfgType = RRM_WR_ENB_CFGTYPE_ENBINIT;

   WrRmEnodebCfgReq(rrmWrEnbCfgReq);
#endif /*RM_INTF*/

   /*Setting the eNodeB Configuration values*/
   wrEmmCb.enbId = cfgAdd->protoCfg.enbId;
   cmMemcpy((U8 *)wrEmmCb.enbName, (U8 *)cfgAdd->protoCfg.enbName.val, 
                                               cfgAdd->protoCfg.enbName.len);
   wrEmmCb.enbNameLen = cfgAdd->protoCfg.enbName.len;
   /* Withpld it crashes because cfgAdd->protoCfg is a static variable
    * saving crash at a cost of few bytes */
   /* WR_FREE(cfgAdd->protoCfg.enbName.val, cfgAdd->protoCfg.enbName.len); */
   wrEmmCb.enbType = (WrEnbType )cfgAdd->protoCfg.enbType;
   /* Currently filling the number of TX Antenna from Default should come from
    * LWR */
   wrEmmCb.numTxAntenna = cfgAdd->numTxAntenna; 

   /* configure enodeb address */
   wrCpyCmTptAddr(&wrEmmCb.datAppAddr[WR_IPV4_IDX], 
      &(cfgAdd->protoCfg).enbIpAddr[WR_IPV4_IDX]);
   wrCpyCmTptAddr(&wrEmmCb.datAppAddr[WR_IPV6_IDX], 
      &(cfgAdd->protoCfg).enbIpAddr[WR_IPV6_IDX]);

   if(CM_TPTADDR_IPV4 == wrEmmCb.datAppAddr[WR_IPV4_IDX].type)
   {
      wrEmmCb.datAppAddr[WR_IPV4_IDX].u.ipv4TptAddr.port = WR_DFLT_EGTP_PORT;
   }
   if(CM_TPTADDR_IPV6 == wrEmmCb.datAppAddr[WR_IPV6_IDX].type)
   {
      wrEmmCb.datAppAddr[WR_IPV6_IDX].u.ipv6TptAddr.port = WR_DFLT_EGTP_PORT;
   }

   wrEmmCb.plmns.numPlmns = cfgAdd->protoCfg.numPlmns;

   for ( idx = 0; idx < wrEmmCb.plmns.numPlmns; idx++ )
   {
      cmMemcpy((U8 *)&(wrEmmCb.plmns.plmns[idx].plmnInfo.plmnId),
                   (U8 *)&(cfgAdd->protoCfg.plmns[idx]), sizeof(LwrPlmnId));
		wrEmmCb.plmns.plmns[idx].plmnInfo.plmnRatPriority.numberOfRats = 
      	cfgAdd->protoCfg.plmns[idx].plmnRatPriority.numberOfRats;
      for(ratIdx = 0; ratIdx < (wrEmmCb.plmns.plmns[idx].plmnInfo.plmnRatPriority.numberOfRats); ratIdx++)
      {
         wrEmmCb.plmns.plmns[idx].plmnInfo.plmnRatPriority.rat[ratIdx] = 
            cfgAdd->protoCfg.plmns[idx].plmnRatPriority.rat[ratIdx];
      }
      wrEmmCb.plmns.plmns[idx].plmnInfo.isRedirSupport = cfgAdd->protoCfg.plmns[idx].isRedirSupport;
      RLOG2(L_DEBUG,"plmn[%d] - isRedirectionSupport[%d]",idx,wrEmmCb.plmns.plmns[idx].plmnInfo.isRedirSupport);
   }
#if 0
#else
   wrEmmCb.numGuGrpIds    = 0;
#endif

   wrEmmCb.numOfMmePriGrps =  cfgAdd->numOfMmePriGrps;

   if (cfgAdd->numOfMmePriGrps >= LWR_SM_MAX_MMES)
   {
      RLOG0(L_WARNING, "Invalid number of MME primary groups received.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/

   cmMemcpy((U8 *)&(wrEmmCb.mmeLoadBalCfg[0]), (U8 *)&(cfgAdd->mmeLdBalCfg[0]), 
                                      sizeof(WrMmeLoaddBalCfg)*cfgAdd->numOfMmePriGrps);

   /*Initializing neighbour eNodeB Hash Lits*/
   /* cmHashListInit(&wrEmmCb.neighEnbLstCp,WR_EMM_NEIGHBOUR_ENODEB_NO_BINS,0,
            FALSE,CM_HASH_KEYTYPE_U32MOD, wrEmmCb.mem.region, wrEmmCb.mem.pool);*/
   wrEmmCb.pwsEtwsCmas = cfgAdd->pwsEtwsCmas; /* Enable ETWS or CMAS or disable both*/
   wrEmmCb.avgFreeCpuRqd = cfgAdd->avgFreeCpuRqd; /* free cpu required */
   wrEmmCb.enbCfgTransTmrVal = cfgAdd->enbCfgTransTmrVal;
   wrEmmCb.uePerBsrTmrCfg.prdBsrTmr = cfgAdd->macMainCfg.ulSchCfg.periodicBSR_Timer;
   wrEmmCb.uePerBsrTmrCfg.retxBsrTmr = cfgAdd->macMainCfg.ulSchCfg.retxBSR_Timer;
   wrEmmCb.pdschDedCfg.p_a = cfgAdd->pdschCfgDedicated.pA;
   wrEmmCb.isCnmEnabled       = cfgAdd->isCnmEnabled;
   /* ccpu00142992:TTIs are not coming after s1-close triggered from MME when UE
    * is in attached state.: isAbortCfmRcv flag is added in order to avoid the
    * race condition at application between receiving the ABORT cfm and UE local
    * context release.
    */
   wrEmmCb.peerAbrtCfm.isAbortCfmRcv = FALSE;
   /* Update global X2 Con request flag */
   wrEmmCb.x2ConFlg       = cfgAdd->x2ConFlg;
   wrEmmCb.peerAbrtCfm.mmeId = 0;
   wrUmmCqiRptCfg(&cfgAdd->cqiRptCfg);
   wrUmmPdcpCfg(&cfgAdd->pdcpInfo);
   wrUmmMacMainCfg(&cfgAdd->macMainCfg);
   wrUmmSchReqCfg(&cfgAdd->schReqCfg);
   wrUmmPuschDedCfg(&cfgAdd->puschDedicated);
   wrUmmPucchPwrCfg(&cfgAdd->pucchPwrCfg);
   wrUmmPdschPwrCfg(&cfgAdd->pdschCfgDedicated);
   wrUmmQosCfg(&cfgAdd->qosCfg);
   wrUmmCsfbGeranCfg(&cfgAdd->csfbToGeranCfg); 
   RETVALUE(ret);
}

/** @brief This function adds the MIB group's information to the cellCb
 *
 * @details
 *
 *     Function: wrEmmCfgCellAddMib
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[out] cellCb : Cell control block
 * @param[in] mibCfgGrp : MIB group parameters
 * @return S16
 *     -# Success : ROK
 */
PRIVATE S16 wrEmmCfgCellAddMib 
(
WrCellCb                     *cellCb,
LwrCellMibCfgGrp             *mibCfgGrp
)
{
   
   /* LWR BW corresponds to number of RBs */
   cellCb->mib.dlBw     = mibCfgGrp->dlBw;
   cellCb->mib.phichDur = 
   mibCfgGrp->phichCfg.phichDur 
            == WR_PHICH_DURATION_NORMAL ? WR_PHICH_DURATION_NORMAL : 
                                                    WR_PHICH_DURATION_EXTENDED;
   cellCb->mib.phichGrps = mibCfgGrp->phichCfg.phichRes;

   RETVALUE(ROK);
} /* end of wrEmmCfgCellAddMib */ 

/** @brief This function 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param  
 * @param 
 * @return 
 */
PRIVATE S16 wrEmmCellTmrCfg 
(
 WrCellCb                    *cellCb,
 LwrProcTimerCfgGrp          *tmrCfg
 )
{
   /* copy timers into the cell Cb */
   cellCb->tmrCfg.rrcConnectionSetupTmr         = 
      tmrCfg->rrcConnectionSetupTmr;

   cellCb->tmrCfg.rrcConnectionReconfigTmr      = 
      tmrCfg->rrcConnectionReconfigTmr;

   cellCb->tmrCfg.rrcConnectionReEstablishTmr   = 
      tmrCfg->rrcConnectionReEstablishTmr;

   cellCb->tmrCfg.rrcSecurityModeCommandTmr     = 
      tmrCfg->rrcSecurityModeCommandTmr;

   cellCb->tmrCfg.rrcUeCapabilityEnquiryTmr     = 
      tmrCfg->rrcUeCapabilityEnquiryTmr;

   cellCb->tmrCfg.rrcConnectionReleaseTmr       = 
      tmrCfg->rrcConnectionReleaseTmr;
   
   cellCb->tmrCfg.rrcHandoverPrepTmr     = 
      tmrCfg->rrcHandoverPrepTmr;
   
   cellCb->tmrCfg.rrcUeInformationRequestTmr    = 
      tmrCfg->rrcUeInformationRequestTmr;
   
   cellCb->tmrCfg.s1InitialContextSetupTmr      = 
      tmrCfg->s1InitialContextSetupTmr;
   
   cellCb->tmrCfg.s1ErabReleaseIndicationTmr    = 
      tmrCfg->s1ErabReleaseIndicationTmr;
   
   cellCb->tmrCfg.s1UeContextReleaseTmr         = 
      tmrCfg->s1UeContextReleaseTmr;
   
   cellCb->tmrCfg.s1PathSwitchTmr               = 
      tmrCfg->s1PathSwitchTmr;
   
   cellCb->tmrCfg.s1HandoverPrepTmr      = 
      tmrCfg->s1HandoverPrepTmr;
   
   cellCb->tmrCfg.s1IRatHandoverPrepTmr    = 
        tmrCfg->s1IRatHandoverPrepTmr;

   cellCb->tmrCfg.s1IRatRelocOverallTmr = 
         tmrCfg->s1IRatRelocOverallTmr;
   cellCb->tmrCfg.s1RelocOverallTmr      = 
      tmrCfg->s1RelocOverallTmr;
   
   cellCb->tmrCfg.s1HandoverCancelTmr      = 
      tmrCfg->s1HandoverCancelTmr;
   
   
   cellCb->tmrCfg.x2HandoverPrepTmr      = 
      tmrCfg->x2HandoverPrepTmr;
   
   cellCb->tmrCfg.x2RelocOverallTmr      = 
      tmrCfg->x2RelocOverallTmr;
   
   cellCb->tmrCfg.x2StoreUeContextTmr      = 
      tmrCfg->x2StoreUeContextTmr;
   
   cellCb->tmrCfg.x2SnStatusTransferTmr      = 
      tmrCfg->x2SnStatusTransferTmr;
   
   cellCb->tmrCfg.internalSubCellChangeTmr      = 
      tmrCfg->internalSubCellChangeTmr;
   
   cellCb->tmrCfg.internalSubCellPdcpControlTmr      = 
      tmrCfg->internalSubCellPdcpControlTmr;

   cellCb->tmrCfg.ehrpdOptHoCompleteTmr      = 
      tmrCfg->ehrpdOptHoCompleteTmr;


   cellCb->tmrCfg.intraEnbHoCompleteTmr      = 
      tmrCfg->intraEnbHoCompleteTmr;

   cellCb->tmrCfg.measurementReportToWaitTmr      = 
      tmrCfg->measurementReportToWaitTmr;

   wrEmmPrintCellTmrCfg(cellCb);

   cellCb->tmrCfg.internalResourceSetupTmr      = 
      tmrCfg->internalResourceSetupTmr;


   cellCb->tmrCfg.internalResourceReleaseTmr      = 
      tmrCfg->internalResourceReleaseTmr;


   cellCb->tmrCfg.internalSecurityControlTmr      = 
      tmrCfg->internalSecurityControlTmr;

   RLOG3(L_DEBUG,"internalSecurityControlTmr[%d] internalResourceReleaseTmr[%d]"
      "internalResourceSetupTmr[%d]", cellCb->tmrCfg.internalSecurityControlTmr,
      cellCb->tmrCfg.internalResourceReleaseTmr,
      cellCb->tmrCfg.internalResourceSetupTmr);

   cellCb->tmrCfg.internalRrcDataTransferTmr      = 
      tmrCfg->internalRrcDataTransferTmr;


   cellCb->tmrCfg.internalForwardingPathSetupTmr      = 
      tmrCfg->internalForwardingPathSetupTmr;


   cellCb->tmrCfg.internalReestablishControlTmr      = 
      tmrCfg->internalReestablishControlTmr;

   RLOG3(L_DEBUG,"internalReestablishControlTmr[%d] "
      "internalForwardingPathSetupTmr[%d] internalRrcDataTransferTmr[%d]"
      ,cellCb->tmrCfg.internalReestablishControlTmr,
      cellCb->tmrCfg.internalForwardingPathSetupTmr,
      cellCb->tmrCfg.internalRrcDataTransferTmr);

   cellCb->tmrCfg.internalBufferFlushTmr      = 
      tmrCfg->internalBufferFlushTmr;


   cellCb->tmrCfg.internalDataTransferStartTmr      = 
      tmrCfg->internalDataTransferStartTmr;


   cellCb->tmrCfg.internalDataForwardEndIndTmr      = 
      tmrCfg->internalDataForwardEndIndTmr;

   RLOG3(L_DEBUG,"internalDataForwardEndIndTmr[%d] "
      "internalDataTransferStartTmr[%d] internalBufferFlushTmr[%d]", 
      cellCb->tmrCfg.internalDataForwardEndIndTmr,
      cellCb->tmrCfg.internalDataTransferStartTmr,
      cellCb->tmrCfg.internalBufferFlushTmr);

   cellCb->tmrCfg.internalReestablishTimeToWaitTmr      = 
      tmrCfg->internalReestablishTimeToWaitTmr;


   cellCb->tmrCfg.internalUserInactivityTmr      = 
      tmrCfg->internalUserInactivityTmr;


   cellCb->tmrCfg.internalSignalingInactivityTmr      = 
      tmrCfg->internalSignalingInactivityTmr;

   RLOG3(L_DEBUG,"internalSignalingInactivityTmr[%d] internalUserInactivityTmr"
      "[%d] internalReestablishTimeToWaitTmr[%d]", 
      cellCb->tmrCfg.internalSignalingInactivityTmr, 
      cellCb->tmrCfg.internalUserInactivityTmr,
      cellCb->tmrCfg.internalReestablishTimeToWaitTmr);

   cellCb->tmrCfg.internalUnassignedTmr      = 
      tmrCfg->internalUnassignedTmr;


   cellCb->tmrCfg.internalNoWaitRetryTmr      = 
      tmrCfg->internalNoWaitRetryTmr;


   cellCb->tmrCfg.internalRrcResetTmr      = 
      tmrCfg->internalRrcResetTmr;

   RLOG3(L_DEBUG,"internalRrcResetTmr[%d] internalNoWaitRetryTmr[%d]"
      " internalUnassignedTmr[%d]",cellCb->tmrCfg.internalRrcResetTmr,
      cellCb->tmrCfg.internalNoWaitRetryTmr,
      cellCb->tmrCfg.internalUnassignedTmr);

   cellCb->tmrCfg.internalEnbResetTmr      = 
      tmrCfg->internalEnbResetTmr;


   cellCb->tmrCfg.internalSolicitMeasurementTmr      = 
      tmrCfg->internalSolicitMeasurementTmr;


   cellCb->tmrCfg.internalRtdInfoReqTmr      = 
      tmrCfg->internalRtdInfoReqTmr;

   RLOG3(L_DEBUG,"internalRtdInfoReqTmr[%d] internalSolicitMeasurementTmr"
      "[%d] internalEnbResetTmr[%d]",cellCb->tmrCfg.internalRtdInfoReqTmr
      ,cellCb->tmrCfg.internalSolicitMeasurementTmr
      ,cellCb->tmrCfg.internalEnbResetTmr);

   RETVALUE(ROK);
} /* end of wrEmmCfgTimers */ 

/** @brief This function 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param  
 * @param 
 * @return 
 */
PRIVATE S16 wrEmmCfgTimers 
(
 LwrProcTimerCfgGrp          *tmrCfg
 )
{
   WrRimCfg                  rimCfg;
   
   /* copy timers into the cell Cb */
   wrEmmCb.tmrCfg.s1SetupTmr           = tmrCfg->s1SetupTmr;

   RLOG1(L_DEBUG,"s1SetupTmr[%d]", wrEmmCb.tmrCfg.s1SetupTmr);

   wrEmmCb.tmrCfg.s1UpdateTmr          = tmrCfg->s1UpdateTmr;

   RLOG1(L_DEBUG,"s1UpdateTmr[%d]", wrEmmCb.tmrCfg.s1UpdateTmr);

   if ( tmrCfg->s1RetryCount == ci_CountInfinity )
   {
     /* To identify infinite retransmission */
     wrEmmCb.tmrCfg.s1RetryCount         = 0xFF;

      RLOG0(L_DEBUG,"s1RetryCount = ci_CountInfinity");


   }
   else
   {
     wrEmmCb.tmrCfg.s1RetryCount         = tmrCfg->s1RetryCount;

      RLOG1(L_DEBUG,"s1RetryCount [%d]", wrEmmCb.tmrCfg.s1RetryCount);

   }

   WR_EMM_MME_GET_TIME_TO_WAIT_VAL(tmrCfg->s1TimeToWait, 
             wrEmmCb.tmrCfg.s1TimeToWait);


   RLOG1(L_DEBUG,"s1MmeConfigurationTransferTmr[%d]",
         wrEmmCb.tmrCfg.s1TimeToWait); 


   wrEmmCb.tmrCfg.s1MmeConfigurationTransferTmr         = 
      tmrCfg->s1MmeConfigurationTransferTmr;

   RLOG1(L_DEBUG,"s1MmeConfigurationTransferTmr[%d]",
         wrEmmCb.tmrCfg.s1MmeConfigurationTransferTmr);

   wrEmmCb.tmrCfg.s1ResetTmr           = tmrCfg->s1ResetTmr;

   RLOG1(L_DEBUG,"s1ResetTmr[%d]", wrEmmCb.tmrCfg.s1ResetTmr);

   wrEmmCb.tmrCfg.x2SetupTmr           = tmrCfg->x2SetupTmr;

   RLOG1(L_DEBUG,"x2SetupTmr[%d]", wrEmmCb.tmrCfg.x2SetupTmr);

   wrEmmCb.tmrCfg.x2SetTimeToWait      = tmrCfg->x2SetTimeToWait;

   RLOG1(L_DEBUG,"x2SetTimeToWait[%d]", wrEmmCb.tmrCfg.x2SetTimeToWait);



   wrEmmCb.tmrCfg.x2UpdateTmr          = tmrCfg->x2UpdateTmr;

   RLOG1(L_DEBUG," x2UpdateTmr[%d]", wrEmmCb.tmrCfg.x2UpdateTmr);

   wrEmmCb.tmrCfg.x2UpdTimeToWait      = tmrCfg->x2UpdTimeToWait;

   RLOG1(L_DEBUG," x2UpdTimeToWait[%d]", wrEmmCb.tmrCfg.x2UpdTimeToWait);

   wrEmmCb.tmrCfg.x2ResetTmr           = tmrCfg->x2ResetTmr;

   RLOG1(L_DEBUG," x2ResetTmr[%d]", wrEmmCb.tmrCfg.x2ResetTmr);

   wrEmmCb.tmrCfg.X2ResetRetryCount    = tmrCfg->X2ResetRetryCount;

   RLOG1(L_DEBUG," X2ResetRetryCount[%d]", wrEmmCb.tmrCfg.X2ResetRetryCount);

   wrEmmCb.tmrCfg.X2SetupRetryCount    = tmrCfg->X2SetupRetryCount;

   RLOG1(L_DEBUG," X2SetupRetryCount[%d]", wrEmmCb.tmrCfg.X2SetupRetryCount);

   if ( tmrCfg->x2UpdRetryCount == ci_CountInfinity )
   { 
     /* To identify infinite retransmission */
     wrEmmCb.tmrCfg.x2UpdRetryCount      = 0xFF;

      RLOG0(L_DEBUG," x2UpdRetryCount  = ci_CountInfinity"); 

   }
   else
   {
     wrEmmCb.tmrCfg.x2UpdRetryCount      = tmrCfg->x2UpdRetryCount;

      RLOG1(L_DEBUG,"x2UpdRetryCount  [%d]", wrEmmCb.tmrCfg.x2UpdRetryCount);


   }
   wrEmmCb.tmrCfg.internalNoWaitRetryTmr = tmrCfg->internalNoWaitRetryTmr;

   RLOG1(L_DEBUG," internalNoWaitRetryTmr [%d]", 
      wrEmmCb.tmrCfg.internalNoWaitRetryTmr); 

   wrEmmCb.tmrCfg.rimOvrlTmr      = tmrCfg->rimOvrlTmr;
   rimCfg.tmrRirVal               = tmrCfg->rimRirTmr;
   rimCfg.tmrRiVal                = tmrCfg->rimRiTmr;
   rimCfg.tmrRiaeVal              = tmrCfg->rimRiaeTmr;
   rimCfg.maxRetryCnt             = tmrCfg->rimMaxRetryCnt;

   RLOG4(L_DEBUG," RIM overall timer = [%d] T-RIR = [%d] T-RI ="
         " [%d] T-RIAE = [%d]",
         tmrCfg->rimOvrlTmr,
         tmrCfg->rimRirTmr,
         tmrCfg->rimRiTmr,
         tmrCfg->rimRiaeTmr);
   RLOG1(L_DEBUG,"RIM maximum retry count = [%d]",tmrCfg->rimMaxRetryCnt);  

   /* RIM Initilisation */
   /* Not validating the return value of the function, as this function
    * wil return failure only in case of NULL argument passed to wrRimCfgReq
    * But always the pointer initialized peroperly and hence this function will
    * not fail */ 
   wrEmmRimCfgReq(&rimCfg); 
   
   RETVALUE(ROK);
} /* end of wrEmmCfgTimers */ 


/** @brief This function is responsible for addition of a cell at the eNodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgCellAdd
 *
 *         Processing steps:
 *         - This is add a cell to the EnodeB 
 *         - It allocates a new CellCb and places the same at the index provided
 *             as part of the configuration request
 *         - It copies all the configuration into the allocated cellCb
 *         - Triggers CellCfgReq to all modules
 *            -# RRM
 *            -# UMM
 *            -# etc.
 *
 *
 * @param[in] cfgAdd : Cell configuration parameters 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure   : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE S16 wrEmmCfgCellAdd
(
LwrAddCellCfg                *cfgAdd
)
{
   S16                       ret              = LCM_REASON_NOT_APPL;
   WrCellCb                  *cellCb          = NULLP;
   S16                       grpIndx          = 0;
   U8                        idx              = 0;
   if(wrEmmCb.cellCb[cfgAdd->cellIndex] != NULLP)
   {
       RLOG1(L_ERROR,"Cell Index Already in use[%d]",cfgAdd->cellIndex);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   
   WR_ALLOC(&(wrEmmCb.cellCb[cfgAdd->cellIndex]), sizeof(WrCellCb  ));
   cellCb = wrEmmCb.cellCb[cfgAdd->cellIndex];
   if (cellCb == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed during cell configuration.");
                                                    
      WR_FREE((wrEmmCb.cellCb[cfgAdd->cellIndex]), sizeof(WrCellCb  ));
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   /*DRX QCI*/
   cellCb->isDRXQCIEnabled = cfgAdd->isDRXQCIEnabled;
   cellCb->dlCarrierFreq = cfgAdd->dlCarrierFreq;
   cellCb->duplexMode = cfgAdd->duplexMode;
   cellCb->cellId = cfgAdd->cellId;
   cellCb->cellState  = WR_CELL_STATE_INIT;
   cellCb->physCellId = cfgAdd->physCellId;
   /* sps - HOFIX */
   cellCb->numAntPorts = wrEmmCb.numTxAntenna;
   cellCb->txPrefMode = cfgAdd->txPrefMode;
   cellCb->isAutoCfgModeEnb = cfgAdd->isAutoCfgModeEnb;
   cellCb->cellSizeType = cfgAdd->cellSizeType;/*UE History info*/
   /*intializing the hash list*/
   cmHashListInit(&(cellCb->neighCells.eutran), WR_EMM_EUTRAN_HASH_NO_BINS,0,
           FALSE,CM_HASH_KEYTYPE_U32MOD, wrEmmCb.mem.region, wrEmmCb.mem.pool);
 

#ifdef LTE_TDD
   if (cellCb->duplexMode != CTF_DUPMODE_TDD)
   {
      RLOG1(L_FATAL,"Invalid duplex mode (not TDD)[%d] received while adding cell.",cellCb->duplexMode);
      RETVALUE(RFAILED);
   }
#else
   if (cellCb->duplexMode != CTF_DUPMODE_FDD)
   {
      RLOG1(L_FATAL,"Invalid duplex mode (not FDD)[%d] received while adding cell",cellCb->duplexMode);
      RETVALUE(RFAILED);
   }
#endif // LTE_FDD


   cellCb->shutdwnPwrCfg.lwPwrCfg.lowRefSigPwr = 
      cfgAdd->shutdownPwrCfg.lwPwrCfg.lowRefSigPwr;
   cellCb->shutdwnPwrCfg.lwPwrCfg.lowPilotSigPwr = 
      cfgAdd->shutdownPwrCfg.lwPwrCfg.lowPilotSigPwr;
   cellCb->shutdwnPwrCfg.lwPwrCfg.lowPriSigPwr = 
      cfgAdd->shutdownPwrCfg.lwPwrCfg.lowPriSigPwr;
   cellCb->shutdwnPwrCfg.lwPwrCfg.lowSecSigPwr = 
      cfgAdd->shutdownPwrCfg.lwPwrCfg.lowSecSigPwr;

   cellCb->shutdwnPwrCfg.hiPwrCfg.highRefSigPwr = 
      cfgAdd->shutdownPwrCfg.hiPwrCfg.highRefSigPwr;
   cellCb->shutdwnPwrCfg.hiPwrCfg.highPilotSigPwr = 
      cfgAdd->shutdownPwrCfg.hiPwrCfg.highPilotSigPwr;
   cellCb->shutdwnPwrCfg.hiPwrCfg.highPriSigPwr = 
       cfgAdd->shutdownPwrCfg.hiPwrCfg.highPriSigPwr;
   cellCb->shutdwnPwrCfg.hiPwrCfg.highSecSigPwr = 
      cfgAdd->shutdownPwrCfg.hiPwrCfg.highSecSigPwr;

   wrUmmAddCell(cellCb->cellId, cfgAdd);     
   /* CSG_DEV start */
   /* update CSG info in cellCb */
   wrEmmCsgCopyCsgCfg(cellCb, cfgAdd);
   /* CSG_DEV end */
   wrUmmInitRabParams(cellCb);

   for (grpIndx = 0; grpIndx < cfgAdd->numOfGrps; grpIndx++)
   {
      switch (cfgAdd->cellCfgGrp[grpIndx]->grpType)
      {
         case WR_CELL_CFG_GRP_MIB:
            ret = wrEmmCfgCellAddMib (cellCb, 
                                   &(cfgAdd->cellCfgGrp[grpIndx]->t.mibCfgGrp));
            break;
         case WR_CELL_CFG_GRP_SIB1:
            ret = wrEmmCfgCellAddSib1 (cellCb, 
                                  &(cfgAdd->cellCfgGrp[grpIndx]->t.sib1CfgGrp));
            break;
         case WR_CELL_CFG_GRP_SIB2:
            ret = wrEmmCfgCellAddSib2 (cellCb, 
                                  &(cfgAdd->cellCfgGrp[grpIndx]->t.sib2CfgGrp));
            break;
         case WR_CELL_CFG_GRP_SIB3:
            ret = wrEmmCfgCellAddSib3 (cellCb,
                                 &(cfgAdd->cellCfgGrp[grpIndx]->t.sib3CfgGrp));
            break;
         case WR_CELL_CFG_GRP_SIB4:
            ret = wrEmmCfgCellAddSib4 (cellCb,
                                 &(cfgAdd->cellCfgGrp[grpIndx]->t.sib4CfgGrp));
            break;
         case WR_CELL_CFG_GRP_SIB6:
           ret = wrEmmCfgCellAddSib6 (cellCb,
                                 &(cfgAdd->cellCfgGrp[grpIndx]->t.sib6CfgGrp));
            break;
            /* SIB 7 case and func */
         case WR_CELL_CFG_GRP_SIB7:
	        ret = wrEmmCfgCellAddSib7 (cellCb,
                                 &(cfgAdd->cellCfgGrp[grpIndx]->t.sib7CfgGrp));
            break;
         case WR_CELL_CFG_GRP_SIB8:
           ret = wrEmmCfgCellAddSib8 (cellCb,
                                 &(cfgAdd->cellCfgGrp[grpIndx]->t.sib8CfgGrp));
            break;
         case WR_CELL_CFG_GRP_SIB9:
            cmMemcpy(cellCb->sib9.henbName, cfgAdd->cellCfgGrp[grpIndx]->t.sib9CfgGrp.henbName, 
            sizeof(cfgAdd->cellCfgGrp[grpIndx]->t.sib9CfgGrp.henbName));
            break;
            
         case  WR_CELL_CFG_GRP_ANR:
            ret = wrEmmCfgCellAnr (cellCb, &(cfgAdd->cellCfgGrp[grpIndx]->t.anrCfgGrp));
            break;
         case  WR_CELL_CFG_GRP_MEAS:
            {
               ret = wrUmmCfgCellMeas (cellCb->cellId, 
                        &(cfgAdd->cellCfgGrp[grpIndx]->t.measCfgGrp));
            }
            break;
         case WR_CELL_CFG_GRP_EAID:
            ret = wrEmmCfgCellEaid(cellCb, 
                                  &(cfgAdd->cellCfgGrp[grpIndx]->t.eaidCfgGrp));
            break;
         case WR_CELL_CFG_GRP_SIB_SCHED:
            ret = wrEmmCfgCellSibSched(cellCb,
                                  &(cfgAdd->cellCfgGrp[grpIndx]->t.sibSchedCfgGrp));
            break;
         case WR_CELL_CFG_GRP_RAB_PARAMS:
            ret = wrUmmCfgRabParams(cellCb->cellId,
                                 &(cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp));
            break;
         case WR_CELL_CFG_GRP_TIMER:
            ret = wrEmmCellTmrCfg(cellCb,
                  &(cfgAdd->cellCfgGrp[grpIndx]->t.procTimerCfgGrp)); 
            if (ret != RFAILED)
            {
               ret = wrEmmCfgTimers(&(cfgAdd->cellCfgGrp[grpIndx]->t.procTimerCfgGrp)); 
            }
            break;
         case WR_CELL_CFG_GRP_RRM:
            {
               cellCb->maxUeSupp = cfgAdd->cellCfgGrp[grpIndx]->t.rrmCfgGrp.enbCacInfo.maxEnbCallCount;
               cellCb->wrDfltRrmRntisize = cfgAdd->cellCfgGrp[grpIndx]->t.rrmCfgGrp.enbCacInfo.dfltRrmRntiSize;
               cellCb->wrDfltRrmStartPreamble = cfgAdd->cellCfgGrp[grpIndx]->t.rrmCfgGrp.enbCacInfo.dfltRrmStartPreamble;
               cellCb->wrDfltRrmNumPreamble = cfgAdd->cellCfgGrp[grpIndx]->t.rrmCfgGrp.enbCacInfo.dfltRrmNumPreamble;
            }
            break;
         case WR_CELL_CFG_GRP_ULSCHD:
            /*TODO: Need input from BLR */
            /*Update the default Scheduler Type*/
            cellCb->schCfg.ulSchType = cfgAdd->cellCfgGrp[grpIndx]->t.ulSchdCfgGrp.ulSchedulingAlg;
            cellCb->schCfg.pfsUlTptCoefficient= cfgAdd->cellCfgGrp[grpIndx]->t.ulSchdCfgGrp.pfsUlTptCoefficient;
            cellCb->schCfg.pfsUlFairCoefficient= cfgAdd->cellCfgGrp[grpIndx]->t.ulSchdCfgGrp.pfsUlFairCoefficient;

            for(idx = 0; idx < LWR_MAX_NUM_QCI; idx++)
            {
                cellCb->schCfg.pfsUlQciWgt[idx] =
                cfgAdd->cellCfgGrp[grpIndx]->t.ulSchdCfgGrp.ulQciWgtList[idx];
            }   
            /*127802*/
            RLOG_STR(L_INFO,"Updated the UL Scheduling type to %s",
               ((cellCb->schCfg.ulSchType == 1)? "PFS":"RR"));
            break;
         case WR_CELL_CFG_GRP_DLSCHD:
            /*TODO: Need input from BLR */
            /*Update the default Scheduler Type*/
            cellCb->schCfg.dlSchType = cfgAdd->cellCfgGrp[grpIndx]->t.dlSchdCfgGrp.dlSchedulingAlg;
            cellCb->schCfg.pfsDlTptCoefficient = cfgAdd->cellCfgGrp[grpIndx]->t.dlSchdCfgGrp.pfsDlTptCoefficient;
            cellCb->schCfg.pfsDlFairCoefficient = cfgAdd->cellCfgGrp[grpIndx]->t.dlSchdCfgGrp.pfsDlFairCoefficient;

            for(idx = 0; idx < LWR_MAX_NUM_QCI; idx++)
            {
                cellCb->schCfg.pfsDlQciWgt[idx] =
                cfgAdd->cellCfgGrp[grpIndx]->t.dlSchdCfgGrp.dlQciWgtList[idx];
            }   
            /*127802*/
            RLOG_STR(L_INFO,"Updated the DL Scheduling type to %s",
               ((cellCb->schCfg.dlSchType == 1)? "PFS":"RR"));
            cellCb->schCfg.dlfsSchdType = cfgAdd->cellCfgGrp[grpIndx]->t.dlSchdCfgGrp.dlfsSchdType;
            RLOG_STR(L_INFO,"Updated the DLFS Scheduling type to %s",
               ((cellCb->schCfg.dlSchType == 1)? "DLFS":"NONDLFS"));
            break;
         case WR_CELL_CFG_GRP_RGR:
            
            cellCb->rgrCfg.dfltStartRnti = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltStartRnti;
            cellCb->rgrCfg.dfltRntiSize = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltRntiSize;
            cellCb->rgrCfg.dfltCfiCfg = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltCfiCfg;
            cellCb->rgrCfg.dfltTrgUlCqi = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltTrgUlCqi;
            cellCb->rgrCfg.dfltBcchPchRaCodeRate = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltBcchPchRaCodeRate;
            cellCb->rgrCfg.dfltPdcchCodeRate = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltPdcchCodeRate;
            cellCb->rgrCfg.dfltULCmnCodRateCcchCqi = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltULCmnCodRateCcchCqi;
            cellCb->rgrCfg.dfltDLCmnCodRateCcchCqi = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltDLCmnCodRateCcchCqi;
            cellCb->rgrCfg.dfltIsDlFreqSel = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltIsDlFreqSel;
            cellCb->rgrCfg.dfltThresholdCqi = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltThresholdCqi;
            cellCb->rgrCfg.dfltMaxDlHqTx = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltMaxDlHqTx;
            cellCb->rgrCfg.dfltMaxMsg4HqTx = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltMaxMsg4HqTx;   
            cellCb->rgrCfg.dfltMaxPucchRbs = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltMaxPucchRbs;   
            cellCb->sib2.puschCfg.puschSubBndStrt = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltPuschSubBndStrt;
            cellCb->sib2.puschCfg.puschNoSubbnds = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltPuschNoSubbnds;
            cellCb->sib2.puschCfg.puschSubBandSz = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltPuschSubBandSz;
            cellCb->sib2.puschCfg.puschSubBandDmrs = cfgAdd->cellCfgGrp[grpIndx]->t.rgrCfgGrp.dfltPuschSubBandDmrs;
           break; 
   
         case WR_CELL_CFG_GRP_RLC:

           cellCb->rlcCfg.ulModeInfo.amRlcSrbtStatusProh = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.ulModeInfo.dfltAmRlcSrbtStatusProh; 
           cellCb->rlcCfg.ulModeInfo.srbtReordring = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.ulModeInfo.srbtReordring; 
           cellCb->rlcCfg.ulModeInfo.amRlcDrbtStatusProh = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.ulModeInfo.dfltAmRlcDrbtStatusProh;
           cellCb->rlcCfg.ulModeInfo.drbtReordring = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.ulModeInfo.drbtReordring;
           cellCb->rlcCfg.dlModeInfo.drbtReordring = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.dlModeInfo.drbtReordring;
           cellCb->rlcCfg.ulModeInfo.umRlcsnFieldLen = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.ulModeInfo.dfltUmRlcsnFieldLen;
           cellCb->rlcCfg.dlModeInfo.amRlcSrbmaxRetxThresh = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.dlModeInfo.dfltAmRlcSrbmaxRetxThresh;
           cellCb->rlcCfg.dlModeInfo.srbtReordring = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.dlModeInfo.srbtReordring; 
           cellCb->rlcCfg.dlModeInfo.amRlcSrbmaxpollByte = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.dlModeInfo.dfltAmRlcSrbmaxpollByte;
           cellCb->rlcCfg.dlModeInfo.amRlcSrbpollPdu = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.dlModeInfo.dfltAmRlcSrbpollPdu;
           cellCb->rlcCfg.dlModeInfo.amRlcSrbtPollRetx = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.dlModeInfo.dfltAmRlcSrbtPollRetx;
           cellCb->rlcCfg.dlModeInfo.amRlcDrbmaxRetxThresh = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.dlModeInfo.dfltAmRlcDrbmaxRetxThresh;
           cellCb->rlcCfg.dlModeInfo.amRlcDrbmaxpollByte = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.dlModeInfo.dfltAmRlcDrbmaxpollByte;
           cellCb->rlcCfg.dlModeInfo.amRlcDrbpollPdu = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.dlModeInfo.dfltAmRlcDrbpollPdu;
           cellCb->rlcCfg.dlModeInfo.amRlcDrbtPollRetx = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.dlModeInfo.dfltAmRlcDrbtPollRetx;
           cellCb->rlcCfg.dlModeInfo.umRlcsnFieldLen = cfgAdd->cellCfgGrp[grpIndx]->t.rlcCfgGrp.dlModeInfo.dfltUmRlcsnFieldLen;
           break;

         case WR_CELL_CFG_GRP_RAB_POLICY:
           for( idx = 0; idx < (LWR_MAX_SRBS-1); idx++)
           {
              cellCb->srbPolicy[idx].rlcInfo.tpRetransmit = cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp.srbPolicy[idx].rlcInfo.tpRetransmit; 
              cellCb->srbPolicy[idx].rlcInfo.pollPdu = cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp.srbPolicy[idx].rlcInfo.pollPdu;
              cellCb->srbPolicy[idx].rlcInfo.pollByte = cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp.srbPolicy[idx].rlcInfo.pollByte;
              cellCb->srbPolicy[idx].rlcInfo.maxRetransThresh = cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp.srbPolicy[idx].rlcInfo.maxRetransThresh;
              cellCb->srbPolicy[idx].rlcInfo.timerReorderingDl = cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp.srbPolicy[idx].rlcInfo.timerReorderingDl;
              cellCb->srbPolicy[idx].rlcInfo.timerReorderingUl = cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp.srbPolicy[idx].rlcInfo.timerReorderingUl;
              cellCb->srbPolicy[idx].rlcInfo.timerStatusProhibit = cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp.srbPolicy[idx].rlcInfo.timerStatusProhibit;
              cellCb->srbPolicy[idx].lchInfo.lchGrpId = cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp.srbPolicy[idx].lchInfo.lchGrpId;
              cellCb->srbPolicy[idx].lchInfo.bucketSizeDuration = cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp.srbPolicy[idx].lchInfo.bucketSizeDuration;
              cellCb->srbPolicy[idx].lchInfo.pbr = cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp.srbPolicy[idx].lchInfo.pbr;
              cellCb->srbPolicy[idx].lchInfo.priority = cfgAdd->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp.srbPolicy[idx].lchInfo.priority;
           }

           break;   
         case WR_CELL_CFG_GRP_ULPWRCNTRL:
            
            RLOG3(L_DEBUG, "wrSmDfltP0UEPUSCH =[%d] wrSmDfltDeltaMCSEnabled=[%d]"
               "wrSmDfltAccumulationEnabled=[%d]",wrSmDfltP0UEPUSCH,
               wrSmDfltDeltaMCSEnabled,wrSmDfltAccumulationEnabled);            
            RLOG3(L_DEBUG, "wrSmDfltP0UEPUCCH =[%d] wrSmDfltpSRSOffset=[%d]"
               "wrSmDfltFilterCoef=[%d]",wrSmDfltP0UEPUCCH,wrSmDfltpSRSOffset
               ,wrSmDfltFilterCoef);

            cellCb->ulPwrCtrl.p0UePUSCH = cfgAdd->cellCfgGrp[grpIndx]->t.uLPwrCtrl.p0UePUSCH;
            cellCb->ulPwrCtrl.accumulationEnabled = cfgAdd->cellCfgGrp[grpIndx]->t.uLPwrCtrl.accumulationEnabled;
            cellCb->ulPwrCtrl.deltaMCSenabled = cfgAdd->cellCfgGrp[grpIndx]->t.uLPwrCtrl.deltaMCSenabled;
            cellCb->ulPwrCtrl.p0UePUCCH = cfgAdd->cellCfgGrp[grpIndx]->t.uLPwrCtrl.p0UePUCCH;
            cellCb->ulPwrCtrl.pSRSoffset = cfgAdd->cellCfgGrp[grpIndx]->t.uLPwrCtrl.pSRSoffset;
            cellCb->ulPwrCtrl.filterCoefficient = cfgAdd->cellCfgGrp[grpIndx]->t.uLPwrCtrl.filterCoefficient;
           
            RLOG0(L_INFO, "[OAM: Updated UL Power Control Parameters]");            
            RLOG3(L_DEBUG," cellCb->ulPwrCtrl.p0UePUSCH =[%ld]"
               "cellCb->ulPwrCtrl.accumulationEnabled=[%d]"
               "cellCb->ulPwrCtrl.deltaMCSenabled =[%d]",
               cellCb->ulPwrCtrl.p0UePUSCH,cellCb->ulPwrCtrl.accumulationEnabled
               ,cellCb->ulPwrCtrl.deltaMCSenabled);            
            RLOG3(L_DEBUG, "cellCb->ulPwrCtrl.p0UePUCCH =[%ld]"
               "cellCb->ulPwrCtrl.pSRSoffset=[%ld]"
               "cellCb->ulPwrCtrl.filterCoefficient=[%ld]"
               ,cellCb->ulPwrCtrl.p0UePUCCH,cellCb->ulPwrCtrl.pSRSoffset,
               cellCb->ulPwrCtrl.filterCoefficient);
            break;

         case WR_CELL_CFG_GRP_CTF:
               cellCb->ctfCfg.dfltScSpacing = cfgAdd->cellCfgGrp[grpIndx]->t.ctfCfgGrp.dfltScSpacing; 
               cellCb->ctfCfg.dfltOpMode = cfgAdd->cellCfgGrp[grpIndx]->t.ctfCfgGrp.dfltOpMode;
               cellCb->ctfCfg.dfltTagPeriod = cfgAdd->cellCfgGrp[grpIndx]->t.ctfCfgGrp.dfltTagPeriod;
               break;

         case WR_CELL_CFG_GRP_SCHD:
               cellCb->schdCfg.prdEnum = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.prdEnum; 
               cellCb->schdCfg.retxCnt = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.retxCnt;
               cellCb->schdCfg.modPrd = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.modPrd;
               cellCb->schdCfg.rachCfg.preambleFormat = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.rachCfg.preambleFormat;
               cellCb->schdCfg.rachCfg.raOccSize = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.rachCfg.raOccSize;
               cellCb->schdCfg.rachCfg.raOccSfnEnum = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.rachCfg.raOccSfnEnum;
               cellCb->schdCfg.rachCfg.prachResource = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.rachCfg.prachResource;
               cellCb->schdCfg.pwrCfg.pucchPwrFmt3Rnti = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.pucchPwrFmt3Rnti;
               cellCb->schdCfg.pwrCfg.pucchPwrFmt3Siz = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.pucchPwrFmt3Siz;
               cellCb->schdCfg.pwrCfg.pucchPwrFmt3aRnti = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.pucchPwrFmt3aRnti;
               cellCb->schdCfg.pwrCfg.pucchPwrFmt3aSiz = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.pucchPwrFmt3aSiz;
               cellCb->schdCfg.pwrCfg.puschPwrFmt3Rnti = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.puschPwrFmt3Rnti;
               cellCb->schdCfg.pwrCfg.puschPwrFmt3Siz = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.puschPwrFmt3Siz;
               cellCb->schdCfg.pwrCfg.puschPwrFmt3aRnti = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.puschPwrFmt3aRnti;
               cellCb->schdCfg.pwrCfg.puschPwrFmt3aSiz = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.puschPwrFmt3aSiz;
               cellCb->schdCfg.pwrCfg.pucchPwrFmt3aEnable= cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.pucchPwrFmt3aEnable;
               cellCb->schdCfg.pwrCfg.puschPwrFmt3aEnable= cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.puschPwrFmt3aEnable;
               cellCb->schdCfg.pwrCfg.isUlGrpPwrCntrlPucchEnable = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.isUlGrpPwrCntrlPucchEnable;
               cellCb->schdCfg.pwrCfg.isUlGrpPwrCntrlPuschEnable = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.pwrCfg.isUlGrpPwrCntrlPuschEnable;
               cellCb->schdCfg.preamPres =  cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.preamPres;
               cellCb->schdCfg.preamStart =  cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.preamStart;
               cellCb->schdCfg.preamSize =  cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.preamSize;
               cellCb->schdCfg.genCfg.macInst = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.genCfg.macInst;
               cellCb->schdCfg.genCfg.maxUePerUlSf = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.genCfg.maxUePerUlSf;
               cellCb->schdCfg.genCfg.maxUePerDlSf = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.genCfg.maxUePerDlSf;
               cellCb->schdCfg.genCfg.maxUlBwPerUe = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.genCfg.maxUlBwPerUe;
               cellCb->schdCfg.genCfg.maxDlBwPerUe = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.genCfg.maxDlBwPerUe;
               cellCb->schdCfg.genCfg.maxDlRetxBw = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.genCfg.maxDlRetxBw;
               cellCb->schdCfg.genCfg.maxDlUeNewTxPerTti = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.genCfg.maxDlUeNewTxPerTti;
               cellCb->schdCfg.genCfg.maxUlUeNewTxPerTti = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.genCfg.maxUlUeNewTxPerTti;
               cellCb->schdCfg.genCfg.cellModSchm = cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.genCfg.cellModSchm;
               /* SPS changes starts */
               cellCb->schdCfg.spsCfg.bIsSpsEnabled = cfgAdd->cellCfgGrp[grpIndx]->t.\
                                                                   schdCfgGrp.spsCfg.bIsSpsEnabled;
               if (cellCb->schdCfg.spsCfg.bIsSpsEnabled == TRUE)
               {
                  cellCb->schdCfg.spsCfg.maxDlSpsUePerTti = cfgAdd->cellCfgGrp[grpIndx]->t.\
                                                                         schdCfgGrp.spsCfg.maxDlSpsUePerTti;
                  cellCb->schdCfg.spsCfg.maxUlSpsUePerTti = cfgAdd->cellCfgGrp[grpIndx]->t.\
                                                                         schdCfgGrp.spsCfg.maxUlSpsUePerTti;
                  cellCb->schdCfg.spsCfg.maxDlBwInRbs = cfgAdd->cellCfgGrp[grpIndx]->t.\
                                                        schdCfgGrp.spsCfg.maxDlBwInRbs;
                  if ((cellCb->schdCfg.spsCfg.maxDlBwInRbs == 0) || ((cellCb->schdCfg.spsCfg.maxDlSpsUePerTti == 0) \
                           && (cellCb->schdCfg.spsCfg.maxUlSpsUePerTti == 0)))
                  {
                     cellCb->schdCfg.spsCfg.bIsSpsEnabled = FALSE;
                  }
               }
               ret = wrUmmCreateSpsRntiDb(cellCb->cellId, cfgAdd, grpIndx);
               /* SPS changes ends */
               cellCb->schdCfg.isDynCfiEnb = cfgAdd->cellCfgGrp[grpIndx]->t.\
                                               schdCfgGrp.isDynCfiEnb;
               break;
         case WR_CELL_CFG_GRP_CSFB:
           ret = wrEmmCfgCellAddCsfbCfg (cellCb,
                                 &(cfgAdd->cellCfgGrp[grpIndx]->t.csfbCfgGrp));
            break;
         case WR_CELL_CFG_GRP_LTEA:
            ret = wrEmmHdlLteAdvCfg(cellCb->cellId, &cfgAdd->cellCfgGrp[grpIndx]->t.lteAdvCfgGrp);
            break;
         default:
            RLOG1(L_WARNING, "Incorrect Group type [%d]"
               ,cfgAdd->cellCfgGrp[grpIndx]->grpType);
            break;
      }
      if(ret != ROK)
      {
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
   } /* end of for */
   if( ret == LCM_REASON_NOT_APPL)
   {
      if(ROK != wrEmmPbmCellCfg(cellCb))
      {
        RLOG0(L_ERROR,"Failed to initialise PBM module");
        RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
   }
   RETVALUE(ret);
} /* end of wrEmmCfgCellAdd */

/** @brief This function initiates the PCI modification towards CL once 
 *   PCI modification timer expired
 * @details
 *
 *     Function: wrEmmPciModTmrExp
 *
 *     Processing steps:
 *     - check whether cell state is UP
 *     - If cell state is up then trigger PCI modification to CL
 *     - Else restart the timer
 *
 *
 * @param[in]   cb : cell control block
 * @return S16
 *         SUCCUESS       : ROK
           FAILUERE       : RFAILED
 */
PUBLIC S16 wrEmmPciModTmrExp
(
 PTR   cb
)
{
   wrEmmMmeBrngCellPwrDown();
   RETVALUE(ROK);
} 

/** @brief This function is responsible for trigerring PCI modification procedure
 *
 * @details
 *
 *     Function: wrEmmProcessPciUpdate
 *
 *         Processing steps:
 *         - This function will release all the connected UEs
 *         - Inform RRM about PCI modification process.
 *         - Update cell state to cell reconfiguration
 *         - Triggers power down request to CL
 * @param[in] cellCb : Cell control block
 * @return S16
 *     -# Success   : ROK
 *     -# Failure   : RFAILED
 */

PRIVATE S16 wrEmmProcessPciUpdate
(
WrCellCb            *cellCb
)
{
   U32          count;
   WrMmeCb      *mmeCb = NULLP;
   WrMmeCont    *mmeCont = NULLP;
#ifdef RM_INTF
   RmuFsmStatusInd  *rrmWrStaInd    = NULLP;
#endif
   mmeCont = &(wrEmmCb.mmeCont);
   /*delete all the UE locally*/
   for(count = 0; count < mmeCont->numMmes; count ++)
   {
      mmeCb = mmeCont->mmes[count];
      if(NULLP != mmeCb)
      {
         /* Release all UEs belongs to this particluar MME */
         wrUmmMmeFullRst(mmeCb->mmeId);
      }
   }

#ifdef RM_INTF
   WR_ALLOC (&rrmWrStaInd, sizeof(RmuCommonMsg));
   if(rrmWrStaInd == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed while modifying PCI.");
      RETVALUE(RFAILED);
   } 

   rrmWrStaInd->enStatusType = RMU_FSM_STATUS_ENB;
   rrmWrStaInd->u.stEnbStatusInd.enEnbStatus = RMU_ENB_STATUS_DOWN;
   rrmWrStaInd->u.stEnbStatusInd.bCellId = cellCb->cellId;

   /*Sending ENB status indication*/
   if(ROK != wrIfmRrmStatusInd(rrmWrStaInd))
   {
      RLOG0(L_ERROR,"eNb status indication (PCI Modification)");
      RETVALUE(RFAILED);
   }
#endif

   wrEmmUpdateCellState(cellCb->cellId, WR_CELL_STATE_RECFG);
   wrUmmUpdateCellState(cellCb->cellId, WR_CELL_STATE_RECFG);
   wrEmmMmeBrngCellPwrDown();
   smCb.isPciMod = TRUE;
   RETVALUE(ROK);
}

/** @brief This function store the PA, PRB configuration received 
 *          from SON in cell control block
 *
 * @details
 *
 *         - This function store the cell edge UE, cell centre UE, 
 *           PRB configuration received from SON. This stored SON configuration 
 *           will be sending to scheduler later
 * @param[in] cellCb : Cell control block
 * @param[in] cfgMod : Cell centre, cell edge and PRB configuration from SON
 * @return Void
 */

PRIVATE Void wrEmmStoreSonPaCfg
(
WrCellCb        *cellCb,
LwrAddCellCfg   *cfgMod
)
{
  // U32  count;
   /*store the SON scheduler configuration in cell control block*/
   cmMemcpy((U8 *)&cellCb->sonCfg.schdCfg, (U8 *)&cfgMod->sonCfg.u.schdCfg,
      sizeof(LwrSonSchdCfg));
   RETVOID;
}

PRIVATE S16 wrEmmCfgCellMod
(
LwrAddCellCfg                *cfgMod
)
{
   S16                       ret = LCM_REASON_NOT_APPL;
   U32                       rCfgType = 0;
   U32                       mask = 1;
   WrCellCb                  *cellCb = NULLP;
   Bool                   cellReset = FALSE;
   S16                       grpIndx = 0;
   Bool                      trgPbm = FALSE;
   Bool                      trgMib = FALSE;
   Bool                      trgSib1 = FALSE;
   Bool                      trgSib3 = FALSE;
   U8                        sibs[WR_MAX_SIBS];
   U8                        numSibs = 0;
   /*RE_CHANGES*/
   Bool                      trgSib4 = FALSE;   
                           
   cellCb = wrEmmCb.cellCb[cfgMod->cellIndex];

   if(wrEmmCb.cellCb[cfgMod->cellIndex] == NULLP)
   {
       RLOG1(L_ERROR,"Invalid CellId [%d]",cfgMod->cellIndex);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }
   
   rCfgType =  cfgMod->reCfgType;

   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_CELL_CFG_PHY_PCI_UPDATE :
            {
               if (cellCb->physCellId != cfgMod->physCellId)
               {
                  RLOG2(L_DEBUG, "Old PhysCellId Val:[%d],New PhysCellId Val[%d]",
                        cellCb->physCellId,cfgMod->physCellId);
                  cellCb->physCellId = cfgMod->physCellId;
                  /*As part of PCI modification from SON, instead of cell reset procedure 
                  phy power down procedure initiated*/
                  wrEmmProcessPciUpdate(cellCb);
               }
               rCfgType =  rCfgType ^ mask;
               break;
            }
            case LWR_CELL_SON_UPDATE :
            {
               /*store the SON PA configuration in cell control block*/
               if(LNL_SON_SCHD_CFG == cfgMod->sonCfg.cfgType)
               {
                  wrEmmStoreSonPaCfg(cellCb, cfgMod);
                  wrEmmSonSchdPrbCfg(cellCb);
               }
               rCfgType =  rCfgType ^ mask;
               break;
            }
            case LWR_CELL_CFG_PHY_CELL_ID:
            {  
               if (cellCb->physCellId != cfgMod->physCellId)
               {
                 RLOG2(L_DEBUG,"Old PhysCellId [%d],New PhysCellId[%d]",
                       cellCb->physCellId,cfgMod->physCellId);
                 cellCb->physCellId = cfgMod->physCellId;
                 cellReset = TRUE;
               }
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            case LWR_CELL_CFG_DUP_MODE:
            {  
               if (cellCb->duplexMode != cfgMod->duplexMode)
               {
                  cellReset = TRUE;
                  RLOG2(L_DEBUG,"Old DuplexMode [%d],New DuplexMode[%d]",
                        cellCb->duplexMode,cfgMod->duplexMode);
                  cellCb->duplexMode = cfgMod->duplexMode;
               }
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            case LWR_CELL_CFG_DL_CARR_FREQ:
            {  
               if (cellCb->dlCarrierFreq != cfgMod->dlCarrierFreq)
               {
                  RLOG2(L_DEBUG,"Old DlCarrierFreq [%d],New DlCarrierFreq[%d]"
                        ,cellCb->dlCarrierFreq,cfgMod->dlCarrierFreq);
                  cellCb->dlCarrierFreq = cfgMod->dlCarrierFreq;
                  cellReset = TRUE;
               }
               rCfgType =  rCfgType ^ mask; 
               break;
            }
            case LWR_CELL_CFG_GRP:
            {  
               rCfgType =  rCfgType ^ mask; 
               for (grpIndx = 0; grpIndx < cfgMod->numOfGrps; grpIndx++)
               {
                  switch (cfgMod->cellCfgGrp[grpIndx]->grpType)
                  {
                     case WR_CELL_CFG_GRP_MIB:
                        ret = wrEmmCfgCellModMib(cellCb, 
                              &(cfgMod->cellCfgGrp[grpIndx]->t.mibCfgGrp));
                        trgMib = TRUE;
                        break;
                     case WR_CELL_CFG_GRP_SIB1:
                        ret = wrEmmCfgCellModSib1(cellCb, 
                              &(cfgMod->cellCfgGrp[grpIndx]->t.sib1CfgGrp));
                        trgSib1 = TRUE;
                        break;
                     case WR_CELL_CFG_GRP_SIB2:
                        ret = wrEmmCfgCellModSib2(cellCb,
                              &(cfgMod->cellCfgGrp[grpIndx]->t.sib2CfgGrp));  
                        if((numSibs < WR_MAX_SIBS) && (ret == ROK)) /* KW FIX*/ 
                        {
                           sibs[numSibs++] = WR_SIB_2; 
                           trgPbm = TRUE;
                        }
                        break;
                     case WR_CELL_CFG_GRP_SIB3:
                        ret = wrEmmCfgCellModSib3(cellCb,
                              &(cfgMod->cellCfgGrp[grpIndx]->t.sib3CfgGrp));  
                        trgSib3 = TRUE;
                        break;
                     case WR_CELL_CFG_GRP_SIB4:
                        ret = wrEmmCfgCellModSib4(cellCb,
                              &(cfgMod->cellCfgGrp[grpIndx]->t.sib4CfgGrp));  
                        /*RE_CHANGES*/
                        trgSib4 = TRUE;
                        break;
  	 	    	     case WR_CELL_CFG_GRP_SIB6:
                        ret = wrEmmCfgCellModSib6(cellCb,
                              &(cfgMod->cellCfgGrp[grpIndx]->t.sib6CfgGrp));
                        if(numSibs < WR_MAX_SIBS) /* KW FIX*/ 
                        {
                           sibs[numSibs++] = WR_SIB_6;
                           trgPbm = TRUE;
                        }
                        break;
  /* Add Sib7 for Mod, also check recfg flag in lwr struture. add func */
                 case WR_CELL_CFG_GRP_SIB7:
 #if 0  /* This portion is needed when we support dynamic update in oam */
                     ret = wrEmmCfgCellModSib7(cellCb,
                           &(cfgMod->cellCfgGrp[grpIndx]->t.sib7CfgGrp));
 #endif
                     /* TODO: check for trgSib7 and WR_SIB_7 */
                 break;
  	 	    	     case WR_CELL_CFG_GRP_SIB9:
                     cmMemcpy(cellCb->sib9.henbName, 
                           cfgMod->cellCfgGrp[grpIndx]->t.sib9CfgGrp.henbName, 
                           sizeof(cfgMod->cellCfgGrp[grpIndx]->t.sib9CfgGrp.henbName));
                     if(numSibs < WR_MAX_SIBS) /* KW FIX*/ 
                     {
                        sibs[numSibs++] = WR_SIB_9;
                        trgPbm = TRUE;
                     }
                        break;
                     case  WR_CELL_CFG_GRP_ANR:
                        ret = wrEmmReCfgCellAnr(cellCb);
                        break;
                     case  WR_CELL_CFG_GRP_MEAS:
                        {
                           ret = wrUmmReCfgCellMeas(cellCb->cellId, 
                                 &(cfgMod->cellCfgGrp[grpIndx]->t.measCfgGrp));
                        }
                        break;
                     case WR_CELL_CFG_GRP_EAID:
                        ret = wrEmmReCfgCellEaid(cellCb, 
                              &(cfgMod->cellCfgGrp[grpIndx]->t.eaidCfgGrp));
                        break;
                     case WR_CELL_CFG_GRP_SIB_SCHED:
                        ret = wrEmmReCfgCellSibSched(cellCb,
                              &(cfgMod->cellCfgGrp[grpIndx]->t.sibSchedCfgGrp),
                              &numSibs, sibs);
                        trgPbm = TRUE;
                        break;
                     case WR_CELL_CFG_GRP_RAB_PARAMS:
                        ret = wrUmmReCfgRabParams(cellCb->cellId,
                              &(cfgMod->cellCfgGrp[grpIndx]->t.rabPolicyCfgGrp)); 
                        break;
                     case WR_CELL_CFG_GRP_TIMER:
                        ret = wrEmmCellTmrCfg(cellCb,
                              &(cfgMod->cellCfgGrp[grpIndx]->t.procTimerCfgGrp)); 
                        if (ret != RFAILED)
                        {
                           ret = wrEmmCfgTimers(&(cfgMod->cellCfgGrp[grpIndx]->t.procTimerCfgGrp)); 
                        }
                        break;
                     case WR_CELL_CFG_GRP_RRM:
                        /*          ret = wrEmmReCfgRrmParams(cellCb,
                                    &(cfgMod->cellCfgGrp[grpIndx]->t.rrmCfgGrp)); */
                        break;
/* LTE_ADV starts */
                case WR_CELL_CFG_GRP_LTEA:
                        /*RE_CHANGES*/
                        ret = wrEmmHdlLteAdvReCfg(cellCb->cellId,
                              &cfgMod->cellCfgGrp[grpIndx]->t.lteAdvCfgGrp,
                              &trgSib4);
                        break;
/* LTE_ADV ends */

                     default:
                        RLOG1(L_WARNING, "Incorrect Group type [%d]"
                           ,cfgMod->cellCfgGrp[grpIndx]->grpType);
                        break;
                  }
               } /* end of for */

               break;
            }
            default:
            {   
               RLOG1(L_WARNING, "Cell reconfiguration received unknown reCfgType [%ld]",
                           (cfgMod->reCfgType));
               rCfgType =  rCfgType ^ mask; 
               break;
            }
         } /* end of switch */
      } /* end of if */
      mask = mask << 1;
   } /* end of while */
   
   if (trgSib3)
   {
      if(numSibs < WR_MAX_SIBS) /* KW FIX*/ 
      {
         sibs[numSibs++] = WR_SIB_3; 
         trgPbm = TRUE;
      }
   }
   /*RE_CHANGES*/
   if (trgSib4)
   {
      if(numSibs < WR_MAX_SIBS) /* KW FIX*/ 
      {
         sibs[numSibs++] = WR_SIB_4; 
         trgPbm = TRUE;
      }
   }
   if( ret == LCM_REASON_NOT_APPL && (WR_CELL_STATE_RESET != cellCb->cellState))
   {
      if(trgPbm)
      {
         if(ROK != wrPbmAddOrUpdateSibs(numSibs, sibs, cellCb->cellId))
         {
            RLOG0(L_ERROR,"PBM Updation is Failed");
            RETVALUE(LCM_REASON_INVALID_PAR_VAL);
         }
      }

      if(trgMib)
      {
         if(ROK != wrEmmPbmMibConfig(cellCb->cellId))
         {
            RLOG0(L_ERROR,"PBM MIB Updation is Failed ");
            RETVALUE(LCM_REASON_INVALID_PAR_VAL);
         }
      }

      if(trgSib1)
      {
         /* PH04_CMAS : added one more parameter. */
         if(ROK != wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_TRANS_ID))

         {
            RLOG0(L_ERROR,"PBM SIB1 Updation is Failed");
            RETVALUE(LCM_REASON_INVALID_PAR_VAL);
         }
         /* Whenever contents of SIB1 changes other than addition/deletion of
          * SIB10 /SIB11 /SIB12 we need to do Paging for the UEs in order to
          * read the updated SIB1 */
         if(ROK != wrPbmStartPagingMsg(cellCb,WR_PAGE_SI_TYPE1))
         {
            RLOG0(L_ERROR,"PBM SIB1 Updation is Failed ");
            RETVALUE(RFAILED);
         }
      }
   }
      
  if (cellReset)
  {
     RLOG_ARG0(L_DEBUG,DBG_CELLID, cellCb->cellId,
        "Cell state = RESET, Reason = Physical cellId Change or DL"
        "carrier Frequency Mode");
    if (ROK != wrEmmResetCell(cellCb->cellId))
    {
       RLOG0(L_ERROR,"Cell Reset is failed for Modifcation of"
          "phycelllId or dlcarriearFreq Change");
       RETVALUE(RFAILED);
    }
  }
   RETVALUE(ret);
} /* end of wrEmmCfgCellMod */ 

/** @brief This function is responsible for copying the eutrana neighbour cell 
 *              info to neighbour cell control block
 *
 * @details
 *
 *     Function: wrEmmCopyEutranNeighCell
 *
 *         Processing steps:
 *         - 
 *
 * @param[out] eutranNeighCell  : Eutran neighbour cell Control block
 * @param[in]  neighCell : Eutran Neighbour Cell parameters      
 * @param[in]  maxRank : Maximum Rank      
 * @return S16 
 *       -# Success : ROK
 */

PRIVATE S16 wrEmmCopyEutranNeighCell
(
WrEutranNeighCellCb          *eutranNeighCell,
LwrNeighCellAddCfg           *neighCell,
U8                            maxRank
)
{
   eutranNeighCell->cio = neighCell->t.eutraCell.cio;
   eutranNeighCell->dlBw = neighCell->t.eutraCell.dlBw;
   eutranNeighCell->earfcnDl = neighCell->t.eutraCell.earfcnDl;
   eutranNeighCell->earfcnUl = neighCell->t.eutraCell.earfcnUl;
   wrUtlCopyPlmnId((WrPlmnId*)&neighCell->t.eutraCell.enbPlmn, &eutranNeighCell->enbPlmn);
   wrUtlCopyPlmnId((WrPlmnId*)&neighCell->t.eutraCell.enbPlmn, &eutranNeighCell->eCgi.plmnId);
   eutranNeighCell->enbType = neighCell->t.eutraCell.enbType;
   eutranNeighCell->eCgi.eutranCellId = neighCell->t.eutraCell.enbId;/*28 bits filled in SM*/
   /*eCGI = (enbId(20bits) + cellId(8bits)) for macro and eCGI = enbId(28bits) for home*/
   RLOG1(L_DEBUG,"eutranNeighCell->enbId = %lu\n",eutranNeighCell->enbId);
   if(eutranNeighCell->enbType == WR_ENODEB_TYPE_HOME)
   {
      eutranNeighCell->enbId = neighCell->t.eutraCell.enbId;
   }
   else
   {
      eutranNeighCell->enbId = ((neighCell->t.eutraCell.enbId)>>8);
   }
/*ccpu00138869 - end*/
   eutranNeighCell->dlBw = neighCell->t.eutraCell.dlBw;
   eutranNeighCell->ulBw     = neighCell->t.eutraCell.ulBw;
   eutranNeighCell->tac      = neighCell->t.eutraCell.tac;
   eutranNeighCell->pci      = neighCell->t.eutraCell.pci;
   eutranNeighCell->cio      = neighCell->t.eutraCell.cio;
   eutranNeighCell->qoffset  = neighCell->t.eutraCell.qoffset;
   eutranNeighCell->curRank  = maxRank;
   eutranNeighCell->numPlmn  = neighCell->t.eutraCell.numPlmn;

   if((eutranNeighCell->numPlmn != 0) && 
      (eutranNeighCell->numPlmn < WR_MAX_PLMN_IDS))
   {
      cmMemcpy((U8 *)eutranNeighCell->plmn,(U8 *)&neighCell->t.eutraCell.plmnId, 
                              sizeof(WrPlmnId)*neighCell->t.eutraCell.numPlmn);
   }
   eutranNeighCell->blackListed = neighCell->t.eutraCell.blackListed;
   eutranNeighCell->numAntenna = neighCell->t.eutraCell.numAntenna;
   eutranNeighCell->detectedRsrp = neighCell->t.eutraCell.rsrp;
   eutranNeighCell->isHeMsCell = neighCell->t.eutraCell.isHeMsCell;

   /* CNM_DEV start */
#ifdef LTE_TDD
   eutranNeighCell->ulDlSubframeCfg = neighCell->t.eutraCell.ulDlSubFrameCfg;
   eutranNeighCell->splSfCfg = neighCell->t.eutraCell.splSfCfg;
   eutranNeighCell->cpDl = neighCell->t.eutraCell.cpDl;
   eutranNeighCell->cpUl = neighCell->t.eutraCell.cpUl;
#endif
   /* CNM_DEV end */
   /* CSG_DEV start */
   wrEmmNbrCfgCopyCsgCfg(eutranNeighCell, neighCell);
   /* CSG_DEV end */
   RETVALUE(ROK);
}

/** @brief This function is responsible for copying the UTRAN neighbour cell
 *              info to neighbour cell control block
 *
 * @details
 *
 *     Function: wrEmmCopyUtranNeighCell
 *
 *         Processing steps:
 *         -
 *
 * @param[out] utranNeighCell  : Utran neighbour cell Control block
 * @param[in]  neighCell : Utran Neighbour Cell parameters
 * @return S16
 *       -# Success : ROK
 */

PRIVATE S16 wrEmmCopyUtranNeighCell
(
WrUtraNeighCellCb            *utranNeighCell,
LwrNeighCellAddCfg           *neighCell
)
{
   utranNeighCell->rncId      =  neighCell->t.utranCell.rncId;
   utranNeighCell->cId        =  neighCell->t.utranCell.cId;
   utranNeighCell->lac        =  neighCell->t.utranCell.lac;
   utranNeighCell->rac        =  neighCell->t.utranCell.rac;  
   utranNeighCell->duplexMode = neighCell->t.utranCell.duplexMode;  
   utranNeighCell->isVoipCapable = neighCell->t.utranCell.isVoipCapable; 
   utranNeighCell->isPsHOCapable = neighCell->t.utranCell.isPsHOCapable;  
   RLOG3(L_INFO,"rncId:%d cId:%d lac:%d\n",utranNeighCell->rncId,
         utranNeighCell->cId,utranNeighCell->cId);
   RLOG4(L_INFO,"rac:%d duplexMode:%d isVoipCapable:%d isPsHOCapable %d\n",
         utranNeighCell->rac,utranNeighCell->duplexMode,
         utranNeighCell->isVoipCapable, utranNeighCell->isPsHOCapable);
   if(WR_UTRA_MODE_FDD == utranNeighCell->duplexMode)
   {
      utranNeighCell->t.utraFdd.psc        =  neighCell->t.utranCell.t.utraFdd.psc;
      utranNeighCell->t.utraFdd.arfcnDl    =  neighCell->t.utranCell.t.utraFdd.arfcnDl;
      utranNeighCell->t.utraFdd.arfcnUl    =  neighCell->t.utranCell.t.utraFdd.arfcnUl;
   }
   else
   {
      utranNeighCell->t.utraTdd.psc        =  neighCell->t.utranCell.t.utraTdd.cellParamId;
      utranNeighCell->t.utraTdd.arfcn      =  neighCell->t.utranCell.t.utraTdd.arfcn;
      utranNeighCell->t.utraTdd.pccpch     =  neighCell->t.utranCell.t.utraTdd.pccpch;
   }
   utranNeighCell->numPlmn    =  neighCell->t.utranCell.numPlmn;
   /*Validating the plmnId Array index */
   if((utranNeighCell->numPlmn != 0) && 
         (utranNeighCell->numPlmn < WR_MAX_PLMN_IDS))
   {
      cmMemcpy((U8 *)&utranNeighCell->plmnId,
            (U8 *)&neighCell->t.utranCell.plmnId,sizeof(WrPlmnId)*neighCell->t.utranCell.numPlmn); 
   }
   utranNeighCell->isRimSupported = neighCell->t.utranCell.isRimSupported;  

   RETVALUE(ROK);  
}

/** @brief This function is responsible for copying the GERAN neighbour cell
 *              info to neighbour cell control block
 *
 * @details
 *
 *     Function: wrEmmCopyGeranNeighCell
 *
 *         Processing steps:
 *         -
 *
 * @param[out] geranNeighCell  : Geran neighbour cell Control block
 * @param[in]  neighCell : Geran Neighbour Cell parameters
 * @return S16
 *       -# Success : ROK
 */

PRIVATE S16 wrEmmCopyGeranNeighCell
(
WrGeranCellCb            *geranNeighCell,
LwrNeighCellAddCfg           *neighCell
)
{
  wrUtlCopyPlmnId((WrPlmnId *)&neighCell->t.geranCell.plmnId, &geranNeighCell->plmnId);
  geranNeighCell->lac   = neighCell->t.geranCell.lac;
  geranNeighCell->rac   = neighCell->t.geranCell.rac;
  geranNeighCell->bsic  = neighCell->t.geranCell.bsic;
  geranNeighCell->cId   = neighCell->t.geranCell.ci;
  geranNeighCell->nccPermittedMeas = neighCell->t.geranCell.nccPermittedMeas;
  geranNeighCell->isDtmCapable     = neighCell->t.geranCell.isDTMCapable;  
  geranNeighCell->isRimSupported   = neighCell->t.geranCell.rimSupportForGeranNgh;
  geranNeighCell->offsetFreq   = neighCell->t.geranCell.offsetFreq;
  if(neighCell->t.geranCell.ncoPres == PRSNT_NODEF)
  {
     geranNeighCell->nco.pres = PRSNT_NODEF;
     geranNeighCell->nco.len  = 2;
     WR_ALLOC(&(geranNeighCell->nco.val), (geranNeighCell->nco.len +7)/8);
     if (NULLP == geranNeighCell->nco.val)
     {
        RLOG0(L_FATAL,"Memory allocation failed for copying Geran neighbour cell configurations.");
        RETVALUE(LCM_REASON_MEM_NOAVAIL);
     }
     cmMemcpy((U8*) geranNeighCell->nco.val, (U8*)(&neighCell->t.geranCell.ncoVal),
           (geranNeighCell->nco.len+7)/8);
  }

   RETVALUE(ROK);  
}

/** @brief This function is responsible for finding the served neighbour cell CB
 *
 * @details
 *
 *     Function: wrEmmDelServedNeighCell
 *
 *         Processing steps:
 *         - iterate the served cell list and get the served neighbour cell CB
 *         - if pci matches 
 *         - remove node from list
 *         - return 
 *
 * @param [in] neignEnb : neighbour eNodeB Control block
 * @param[out] pci : physical cell id      
 * @return S16
 *      -# Success  : ROK
 *      -# Failure    : RFAILED
 */
 PRIVATE S16 wrEmmDelServedNeighCell
 (
 WrNeighEnbCb                *neignEnb,
 WrEutranNeighCellCb          *neighCell
 )
 {
    CmLList                  *list =NULLP;
    U8                       count =0;
    WrEutranNeighCellCb      *eutranNeighCell = NULLP;
    
    CM_LLIST_FIRST_NODE(&(neignEnb->srvdCellLst), list);
    for (count = 0; count < neignEnb->srvdCellLst.count; count++)
    {
       eutranNeighCell =(WrEutranNeighCellCb *)list->node;
#if 0 /* PCI Collision not supported currently */
       if ( eutranNeighCell->eCgi.eutranCellId == neighCell->eCgi.eutranCellId)
       {
          if(TRUE == wrUtlPlmnsEqual(&eutranNeighCell->eCgi.plmnId, &neighCell->eCgi.plmnId))
          {
            cmLListDelFrm(&(neignEnb->srvdCellLst),list);
            RETVALUE(ROK); 
          }
       }
#endif
       if ( eutranNeighCell->pci == neighCell->pci)
       {
          cmLListDelFrm(&(neignEnb->srvdCellLst),list);
          RETVALUE(ROK); 
       }
       CM_LLIST_NEXT_NODE(&(neignEnb->srvdCellLst),list);
    }
    RETVALUE(RFAILED);
 }


/** @brief This function is responsible for finding the served neighbour cell CB
 *
 * @details
 *
 *     Function: wrEmmFindServedNeighCell
 *
 *         Processing steps:
 *         - iterate the served cell list and get the served neighbour cell CB
 *         - if pci matches return 
 *
 * @param [in] neignEnb : neighbour eNodeB Control block
 * @param[out] pci : physical cell id      
 * @return S16
 *      -# Success  : ROK
 *      -# Failure    : RFAILED
 */
 PUBLIC  S16 wrEmmFindServedNeighCell
 (
 WrNeighEnbCb                *neignEnb,
 U16                         pci
 )
 {
    CmLList                  *list =NULLP;
    U8                       count =0;
    WrEutranNeighCellCb      *eutranNeighCell = NULLP;
    
    CM_LLIST_FIRST_NODE(&(neignEnb->srvdCellLst), list);
    for (count = 0; count < neignEnb->srvdCellLst.count; count++)
    {
       eutranNeighCell =(WrEutranNeighCellCb *)list->node;
       if ( eutranNeighCell->pci == pci)
       {
          RETVALUE(ROK); 
       }
       CM_LLIST_NEXT_NODE(&(neignEnb->srvdCellLst),list);
    }
    RETVALUE(RFAILED);
 }


/** @brief This function is responsible for finding the served neighbour cell CB
 *
 * @details
 *
 *     Function: wrEmmGetNeighEnbSrvdCell
 *
 *         Processing steps:
 *         - iterate the served cell list and get the served neighbour cell CB
 *         - if pci matches return 
 *
 * @param [in] neignEnb : neighbour eNodeB Control block
 * @param[out] pci : physical cell id      
 * @return S16
 *      -# Success  : ROK
 *      -# Failure    : RFAILED
 */
 PUBLIC WrEutranNeighCellCb* wrEmmGetNeighEnbSrvdCell
 (
 WrNeighEnbCb                *neignEnb,
 U16                         pci
 )
 {
    CmLList                  *list =NULLP;
    U8                       count =0;
    WrEutranNeighCellCb      *eutranNeighCell = NULLP;
    
    CM_LLIST_FIRST_NODE(&(neignEnb->srvdCellLst), list);
    for (count = 0; count < neignEnb->srvdCellLst.count; count++)
    {
       eutranNeighCell =(WrEutranNeighCellCb *)list->node;
       if ( eutranNeighCell->pci == pci)
       {
          RETVALUE(eutranNeighCell); 
       }
       CM_LLIST_NEXT_NODE(&(neignEnb->srvdCellLst),list);
    }
    RETVALUE(NULLP);
 }

/** @brief This function is responsible for fetching the NR cell CB pointer 
 *         from the ANR
 *
 * @details
 *
 *     Function: wrEmmGetUtranNrEntry
 *
 *         Processing steps:
 *         - Fill the Nr IInfo 
 *         - call ANR funciton to get the NR cell CB
 *         - return the NR cell Cb
 *
 * @param[in] cellId          : Cell Id of the seving cell
 * @param[in] nrDelInfo       : nr Info the NR to be deleted
 * @return U16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE WrUtraNeighCellCb* wrEmmGetUtranNrEntry(WrCellCb *cellCb, 
                                      LwrUtraCellDelCfg *utraCellInfo,
                                      LwrNeighType    type)
{
   WrUtraNeighCellCb          *utranCb = NULLP;
   WrUtranNrInfo              nrInfo;

   nrInfo.nrIndex  = utraCellInfo->nrIndex; 
   nrInfo.arfcn    = utraCellInfo->arfcn; 
   nrInfo.psc      = utraCellInfo->psc;
   nrInfo.cid      = utraCellInfo->cid;

   if(type == WR_NEIGH_CELL_UTRA_FDD)
   {
      utranCb = wrEmmAnrGetUtranNr(&cellCb->nrCb->utranCb, &nrInfo);
   }
   else
   {
      utranCb = wrEmmAnrGetUtranNr(&cellCb->nrCb->utranTddCb, &nrInfo);
   }

   RETVALUE(utranCb);

}
/** @brief This function is responsible for deleting the neighbor Cell  
 *         from the serving cell list of the Neighbor eNB.
 *
 * @details
 *
 *     Function: wrEmmDelNrFrmServCellLst
 *
 *         Processing steps:
 *         - Get the neighbor eNB entry
 *         - delete the neighbor cell from teh serving list
 *
 * @param[in] neighCellCb     : pointer to the Neighbor cell Entry
 * 
 * @return U16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */

PUBLIC S16 wrEmmDelNrFrmServCellLst
(
   WrEutranNeighCellCb *neighCellCb
)
{
   WrNeighEnbCb               *neighEnCb= NULLP;
   S16                        ret =ROK;

   /*Finding the neighbour eNodeB control block*/
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp),
         (U8 *)&neighCellCb->enbId,sizeof(neighCellCb->enbId),0,
         (PTR *)&neighEnCb);

   if( ret != ROK)
   {
      RLOG1(L_ERROR,"Failed to find neighbour control block with enbId = %d ",neighCellCb->enbId);
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (wrEmmDelServedNeighCell(neighEnCb, neighCellCb) !=  ROK)
   {
      RLOG0(L_ERROR,"Served Cell Node with pci Not present");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);   
   }

   RETVALUE(ROK);
}
/** @brief This function is responsible for getting peer id based on enodeB ID
 * *  
 *  *
 *  * @details
 *  *
 *  *     Function: wrEmmGetPeerId
 *  *
 *  *         Processing steps:
 *  *         - check the eNodeB is in peer id list, if exist return 
 *   *    
 *    *
 *    * @param[in] enodeB id     : pointer to the Neighbor cell Entry
 *    * @Param[out] peer id       : peer id
 *    * 
 *     * @return U16
 *     *     -# Success : ROK 
 *      *     -# Failure  :  RFAIELD
 *      */
PUBLIC S16 wrEmmGetPeerId
(
 U32 eNodeBId,
 U32  *peerId
 )
{
   U32 i;

   *peerId = 0;
   for(i = 0; i < WR_X2AP_MAX_PEERS; i++)
   {
      if(wrEmmCb.x2ApEnbIdLst[i].enbId == eNodeBId)
      {
         *peerId = wrEmmCb.x2ApEnbIdLst[i].peerId;
         RETVALUE(ROK);
      }
   }
   RLOG1(L_ERROR,"Failed to find peerId for enbId = %d",eNodeBId);
   RETVALUE(RFAILED);
}



/** @brief 
 *
 * @details
 *
 *     Function: wrEmmAddNewNeighEnb 
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param  
 * @param 
 * @return 
 */

PUBLIC WrNeighEnbCb* wrEmmAddNewNeighEnb
(
   WrEutranNeighCellCb        *neighCellCb,
   U16                        cellId   
)
{
   WrNeighEnbCb *neighEnbCb = NULLP;

   WR_ALLOC(&neighEnbCb,sizeof(WrNeighEnbCb));
   if(NULLP == neighEnbCb)
   {
      RLOG0(L_FATAL,"Memory allocation failed while adding new neighbour eNodeB.");
      RETVALUE(NULLP);
   }

   cmLListInit(&(neighEnbCb->srvdCellLst));
   cmInitTimers(&(neighEnbCb->timer), 1);
   cmInitTimers(&(neighEnbCb->enbCfgUpTm), 1);
   cmInitTimers(&(neighEnbCb->enbcfgTransTmr), 1);
   /*  Filling the received values  */
   neighEnbCb->cellId = cellId;
   RLOG2(L_DEBUG, "neighCellCb:enbId = %lu, neighCellCb:enbType = %lu",
                         neighCellCb->enbId,neighCellCb->enbType);

   neighEnbCb->enbId = neighCellCb->enbId;
   neighEnbCb->enbType = neighCellCb->enbType;
   neighEnbCb->x2Connection =  WR_NEIGH_ENB_STATE_DOWN;
   if(wrEmmGetPeerId(neighEnbCb->enbId,&(neighEnbCb->peerId)) != ROK)
   {
      WR_GET_X2AP_PEER_ID(neighEnbCb->peerId, neighEnbCb->enbId);
   }

   neighEnbCb->rstRtxCnt = 0;
   neighEnbCb->cfgUpdRtxCnt = 0;
   neighEnbCb->x2SetupRtxCnt = 0;

   /* Update the isX2Reqd flag based on the 
    * global X2Conflg */
   if ( (neighEnbCb->enbType == WR_ENODEB_TYPE_MACRO) && 
        ((wrEmmCb.x2ConFlg == WR_X2_CON_ENBL_ALL ) ||
         (wrEmmCb.x2ConFlg == WR_X2_CON_ENBL_MACRO )))
   {
   neighEnbCb->isX2Reqd = TRUE; 
   }
   else if ((neighEnbCb->enbType  == WR_ENODEB_TYPE_HOME) &&
            ((wrEmmCb.x2ConFlg == WR_X2_CON_ENBL_ALL) ||
            (wrEmmCb.x2ConFlg == WR_X2_CON_ENBL_FEMTO)))
   {
      neighEnbCb->isX2Reqd = TRUE;
   }
   else
   {
      neighEnbCb->isX2Reqd = FALSE;
   }
   RLOG2(L_DEBUG,"Adding new neighbor eNodeb enbId [%ld] peerId [%ld]",
      neighEnbCb->enbId, neighEnbCb->peerId);
   /*Adding the  neighbour CB into hash list*/
   if( cmHashListInsert(&(wrEmmCb.neighEnbLstCp),
            (PTR)neighEnbCb,(U8 *)&neighEnbCb->enbId, sizeof(neighEnbCb->enbId)) != ROK)
   {
      RLOG1(L_ERROR,"Failed to insert neighEnbCb with enbId = %d",neighEnbCb->enbId);
      WR_FREE(neighEnbCb,sizeof(WrNeighEnbCb));
      RETVALUE(NULLP);
   }   

   RETVALUE(neighEnbCb);
}

/** @brief This function is responsible for deletion of a neighbour cell at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmDeleteX2Con
 *
 *         Processing steps:
 *         - Get the eNodeB CB using eNodeB ID
 *         - Check the served cell list count and X2 connection 
 *           if no of served cell is zero and X2 connection is established
 *            - Send Release request to X2AP with corresponding Peer id
 *            - Delete the eNodeB control CB from neighbour list
 *
 * @param[in] enbId           : eNodeB id
 * @return S16
 *     -# Success : ROK
 *     -# Failure  :RFAILED
 */
PRIVATE S16 wrEmmDeleteX2Con
(
U32                          enbId

)
{
   WrNeighEnbCb              *neighCb = NULLP;
   CztRlsReq                 *x2RelReq = NULLP;

   /*Find the neighbour eNodeB CB with received eNodeB ID*/
   if(ROK != cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8*)&enbId, 
         sizeof(U32), 0,(PTR*) &neighCb))
   {
      RLOG1(L_ERROR," Failed to find the neighbour eNodeB with %d",enbId);
      RETVALUE(RFAILED);
   }
   if(neighCb->srvdCellLst.count == 0)
   {
      if(neighCb->x2Connection == WR_NEIGH_ENB_STATE_UP)
      {
         /*Release the X2 connection*/
         WR_ALLOCEVNT(&x2RelReq, sizeof(CztRlsReq));
         if(x2RelReq == NULLP)
         {
            RLOG0(L_ERROR,"Memory Allocation failed for X2AP release request");
            RETVALUE(RFAILED);
         }
         x2RelReq->peerId = neighCb->peerId;
         x2RelReq->rlsType = CZT_RELTYPE_TERM;
         if ( ROK != WrIfmX2apRelReq (x2RelReq))
         {
            RLOG2(L_ERROR," Failed to send X2AP release request.x2RelReq->peerId = %d,x2RelReq->rlsType = %d",
                  x2RelReq->peerId,x2RelReq->rlsType);
            RETVALUE(RFAILED);
         }
      }
      /*Reset the peer Id*/
      WR_CLEAR_X2AP_PEER_ID(neighCb->peerId);
      /*Delete the neighbour*/
      cmHashListDelete( &(wrEmmCb.neighEnbLstCp), (PTR)neighCb);
      WR_FREE (neighCb, sizeof(WrNeighEnbCb));
      RETVALUE(ROK);
   }
   RETVALUE(ROK);
}



/** @brief This function is responsible for deletion of a neighbour cell at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgNeighCellDelete
 *
 *         Processing steps:
 *         - switch to the NR type
 *         - if EUTRAN, 
 *             - remove the neighbor cell from the serving cell list of
 *               the Neighbor Enb
 *             - call ANR fucntion to remove the Node from the ANR operation
 *               lists
 *             - free the memory
 *         - for other cases
 *             - call ANR fucntion to remove the Node from the ANR operation 
 *               lists
 *
 * @param[in] neighCellDel    : Neighbour cell list, neighbors to be deleted  
 * @param[in] numCells        : number of cells in the list
 * @param[in] cellId          : Cell Id of the seving cell
 * @return U16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE S16 wrEmmCfgNeighCellDelete
(
LwrNeighCellDelCfg           *neighCellDel,
U16                          numCells,
U8                           cellId

)
{
   S16                       ret = LCM_REASON_NOT_APPL;
   U8                        nrIndex = 0;
   WrEutranNeighCellCb       *neighCellCb =NULLP;
   WrUtraNeighCellCb          *utraCellCb = NULLP;   
   WrCellCb                  *cellCb = NULLP;
   WrEutranNbrCellInfo        nrInfo;
   Bool                       eUtranCell = FALSE;

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG1(L_ERROR, "Invalid Cell id received while deleting neighbour cell [%d]",cellId);
      RETVALUE(RFAILED);
   }

   /* Call Anr to initialize the ANR-MEAS interface data structure 
    * to send the NR update info to Measurement 
    */
   wrEmmAnrInitMeasNrUpdtInfo(cellCb);
   
   while(nrIndex < numCells)
   {
      switch( neighCellDel[nrIndex].type )
      {
         case WR_NEIGH_CELL_EUTRA:
            {
               nrInfo.nrIndex   = neighCellDel[nrIndex].t.eutranCell.nrIndex; 
               nrInfo.earfcn    = neighCellDel[nrIndex].t.eutranCell.earfcn; 
               nrInfo.phyCellId = neighCellDel[nrIndex].t.eutranCell.phyCellId;
               nrInfo.cid       = neighCellDel[nrIndex].t.eutranCell.cid;
               wrUtlCopyPlmnId((WrPlmnId *)&neighCellDel[nrIndex].t.eutranCell.plmnId, &nrInfo.plmnId);

               neighCellCb = wrEmmAnrGetEutranNr(&cellCb->nrCb->eutranCb, &nrInfo);
               if(NULLP != neighCellCb)
               {

                  wrEmmAnrDelEutranNr(cellCb, neighCellCb);

                  /* Delete the Node from the Serving Cell list of Enb */
                  wrEmmDelNrFrmServCellLst(neighCellCb);

                  if( wrEmmDeleteX2Con(neighCellCb->enbId) == RFAILED)
                  {
                     RLOG0(L_INFO,"Failed to Delete the neighbour eNodeB");
                     ret = RFAILED;
                  }
                  WR_FREE (neighCellCb, sizeof(WrEutranNeighCellCb));
               }
               else
               {
                  RLOG2(L_WARNING, "EUTRAN Neighbour Entry for FREQ[%d] PCI[%d] is NULL"
                     " in ANR",
                      neighCellDel[nrIndex].t.eutranCell.earfcn, 
                     neighCellDel[nrIndex].t.eutranCell.phyCellId);
                  ret = RFAILED;
               }
               /* Only Neighor should be updated for EUTRAN Cell */
               eUtranCell = TRUE;

            }
            break;
  	      case WR_NEIGH_CELL_UTRA_FDD:
            {
               utraCellCb = wrEmmGetUtranNrEntry(cellCb,&neighCellDel[nrIndex].t.utranCell, WR_NEIGH_CELL_UTRA_FDD);
               if(NULLP != utraCellCb)
               {
                  wrEmmAnrDelUtraNr(cellCb, utraCellCb);
                  WR_FREE (utraCellCb, sizeof(WrUtraNeighCellCb));
               }
               else
               {
                    RLOG2(L_WARNING, "UTRAN Neighbour Entry for FREQ[%d] PCI[%d] is"
                      "NULL in ANR",
                      neighCellDel[nrIndex].t.utranCell.arfcn,
                      neighCellDel[nrIndex].t.utranCell.psc);
                  
                  RETVALUE(RFAILED);
               }
            }
	         break;
  	      case WR_NEIGH_CELL_UTRA_TDD:
            {
               utraCellCb = wrEmmGetUtranNrEntry(cellCb,&neighCellDel[nrIndex].t.utranCell, WR_NEIGH_CELL_UTRA_TDD);
               if(NULLP != utraCellCb)
               {
                  wrEmmAnrDelUtraNr(cellCb, utraCellCb);
                  WR_FREE (utraCellCb, sizeof(WrUtraNeighCellCb));
               }
               else
               {
                    RLOG2(L_WARNING," UTRAN Neighbour Entry for FREQ[%d] PCI[%d] is"
                      "NULL in ANR",
                      neighCellDel[nrIndex].t.utranCell.arfcn,
                      neighCellDel[nrIndex].t.utranCell.psc);
                  
                  RETVALUE(RFAILED);
               }
            }
            break;
        default:
            ret = LCM_REASON_INVALID_PAR_VAL;
      }

      nrIndex++;
   }

   if(ret == ROK) 
   {
      /* Process NR Update Info and send Trigger to Meas and PBM module */
      ret = wrEmmAnrProcNrUpdtInfo(cellCb, FALSE);

      if (ROK == ret)
      {
         ret = LCM_REASON_NOT_APPL;
         if (TRUE == eUtranCell)
         {
            wrEmmNghTrigCfgUpd(cellCb); 
         }
      }
      else
      {
         ret = LCM_REASON_INVALID_PAR_VAL;
      }
   }
   else
   {
      ret = LCM_REASON_INVALID_PAR_VAL;
   }
      
   RETVALUE(ret);
}


/** @brief This function is responsible for deleting a neighbour cell local intiation
 *                after NRT age timer expiry 
 * @details
 *
 *     Function: wrEmmCheckAgeAndDeleteCell
 *
 *         Processing steps:
 *            
 *          Fill the LWR structure
 *          call EMM neighbour cell deletion function to delete a neighbour cell
 *        
 *
 * @param[in] WrCellCb                         : Cell control Block
 * @param[in] WrEutranNeighCellCb        : Neighbour Cell control Block
 * @return void
 *  
 */

PUBLIC Void wrEmmCheckAgeAndDeleteCell
(
WrCellCb                     *cellCb,
WrEutranNeighCellCb          *nbrCell
)
{
   U32                    ageInMSec;
   LwrNeighCellDelCfg     neighDel[1]={{0}}; 

   TRC2(wrEmmCheckAgeAndDeleteCell);

   if (nbrCell->isHeMsCell == FALSE)
   {
      ageInMSec = (numTtiTicks - nbrCell->numTtiTicks) * 10;
              
      if (ageInMSec > (cellCb->nrCb->cellValidAgeTmrVal*60*1000))
      {
         neighDel[0].type = WR_NEIGH_CELL_EUTRA;
         neighDel[0].t.eutranCell.earfcn = nbrCell->earfcnDl;
         neighDel[0].t.eutranCell.phyCellId =nbrCell->pci;
         if((wrEmmCfgNeighCellDelete(neighDel,1,cellCb->cellId)) == LCM_REASON_NOT_APPL)
         {
            RLOG1(L_INFO,"Deleted Cell with PCI [%d] from NRT",nbrCell->pci);
         }
      }
   }
}


/* FIX for ccpu00132596 */

/** @brief This function is responsible for retrieving the Enum
 *         based on the default pciRange and Mapping it to the 
 *         exact PCI Range Value.
 *
 * @details
 *
 *     Function: wrEmmPciRangeMapping
 *
 *         Processing steps:
 *         - Get the default pciRange.
 *         - Based on the Enum Value return the exact PCI Range Value. 
 *
 * @param[in] pciRange     : pointer to the default pciRange
 * 
 * @return U16
 *     -# Exact PCI Range Value
 */
PRIVATE U16 wrEmmPciRangeMapping
(
 WrPhysCellIdRange            *pciRange
)
{
   U16   mappedPci = 0;

   if(pciRange->phyCellIdRangePres == WR_PRSNT)
   {
      switch(pciRange->phyCellIdRange)
      {
         case NhuPhysCellIdRangerangen4Enum : 
            mappedPci = 4;
            break;
         case NhuPhysCellIdRangerangen8Enum :
            mappedPci = 8;
            break;
         case NhuPhysCellIdRangerangen12Enum :
            mappedPci = 12;
            break;
         case NhuPhysCellIdRangerangen16Enum :
            mappedPci = 16;
            break;
         case NhuPhysCellIdRangerangen24Enum :
            mappedPci = 24;
            break;
         case NhuPhysCellIdRangerangen32Enum :    
            mappedPci = 32;
            break;
         case NhuPhysCellIdRangerangen48Enum :
            mappedPci = 48;
            break;
         case NhuPhysCellIdRangerangen64Enum :
            mappedPci = 64;
            break;
         case NhuPhysCellIdRangerangen84Enum :
            mappedPci = 84;
            break;
         case NhuPhysCellIdRangerangen96Enum :
            mappedPci = 96;
            break;
         case NhuPhysCellIdRangerangen128Enum :
            mappedPci = 128;
            break;
         case NhuPhysCellIdRangerangen168Enum :
            mappedPci = 168;
            break;
         case NhuPhysCellIdRangerangen252Enum :
            mappedPci = 252;
            break;
         case NhuPhysCellIdRangerangen504Enum :
            mappedPci = 504;
            break;
         default :
            RLOG1(L_WARNING, "PCI Range value not found for PCI entry [%d]",
               pciRange->phyCellIdRange );
                    break;
      }
   }
   RETVALUE(mappedPci);
}

/* FIX for ccpu00132596 */

/** @brief This function checks whether the serving cell and
 *  neighbour cell to which X2 setup is to be sent are in same access mode
 *
 *  @details
 *     X2 Setup Request is sent to neighbour enodeB only if serving and
 *     neighbour cells are of same access mode
 *
 *     Function: wrEmmIsAccessModeSame
 *
 *     Processing steps:
 * -# SIB4 contains CSG Split Info : Start PCI and PCI range
 * -# WrCellCb contains serving cell's PCI
 * -# Allow X2 interface to be setup only if both serving & neighbor's PCIs
 *    fall within/out of PCI range in CSG Split Info
 *
 *
 * @param[in]  servPci : serving cell's PCI
 * @param[in]  neighPci: neighbour cell's PCI
 * @return Bool
 *        -# TRUE  : Serving and neighbour cells are of same access mode
 *        -# FALSE : Serving and neighbour cells are of different access mode
 */

PRIVATE Bool wrEmmIsAccessModeSame
(
 U16                          servPci,
 U16                          neighPci,
 WrPhysCellIdRange            *pciRange
)
{
   U16                     maxPciVal;
   U8                        accessMode = 0;
   U16                     mappedPciRange = 0;

/* FIX for ccpu00132596 */
   mappedPciRange = wrEmmPciRangeMapping(pciRange);

   if(mappedPciRange != 0)
   {
      maxPciVal = pciRange->startPhyCellID + (mappedPciRange - 1);
   }
   else
   {
      maxPciVal = pciRange->startPhyCellID + mappedPciRange;   
   }

   if(((servPci >= pciRange->startPhyCellID) && (servPci <= maxPciVal)) &&
     ((neighPci >= pciRange->startPhyCellID) && (neighPci <= maxPciVal)))
/* FIX for ccpu00132596 */
   {
      /* if both serving and neighbour PCIs are in PCIRange given
       * in CSG Split Info , then return TRUE */
      accessMode  = WR_ENB_CLOSED_ACCESS;
   }
   else if(((servPci < pciRange->startPhyCellID) || (servPci >= maxPciVal)) &&
        ((neighPci < pciRange->startPhyCellID) || (neighPci >= maxPciVal)))
   {

      /* if both serving and neighbour PCIs are out of PCIRange given
       * in CSG Split Info , then return TRUE */
      accessMode = WR_ENB_OPEN_ACCESS;
   }
   if((accessMode == WR_ENB_CLOSED_ACCESS)|| (accessMode == WR_ENB_OPEN_ACCESS))
   {
       /* if any one is in the range and other is out of the range
        * then return FALSE */
        RETVALUE(TRUE);
   }
   else 
   {
       RETVALUE(FALSE);
   }

}/*end of wrEmmIsAccessModeSame */

PRIVATE S16 wrEmmCfgHandleRetFailure
(
    LwrNeighType neighType,
    S16 ret
)
{
   switch(neighType)
   {
      case WR_NEIGH_CELL_EUTRA:
         {
            if(ret == RFAILED)
            {
               RLOG0(L_WARNING, "Failed to add eUtra neighbour cell in ANR");
            }
         }
         break;
      default:
         {   
            RLOG1(L_WARNING, "unknown Neighbour Type:[%d]",neighType);
            break;
         }   
   }      
   RETVALUE(ret);
}

/*Fix for ccpu00132820 : NeighCell Validation */
/** @brief This function is responsible for validating the addition 
 * of neighbour cells at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgValidateNeighCell
 *
 *         Processing steps:
 *         - This validates neighbor cell 
 *         - Check if frequency node is present or not
 *         - Check if duplicate PCI is present
 *
 * @param[in] cellId     : cell for which NR needs to be added
 * @param[in] numCells   : number of nbr Cells 
 * @param[in] nbrCells    : Neighbour Cells 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */

PRIVATE S16 wrEmmCfgValidateNeighCell
(
 U8                               cellId,
 U16                               numCell,
 LwrNeighCellAddCfg               *nbrCells,
 Bool                             *nbrCellStatus
 )
{
   WrCellCb                      *cellCb    = NULLP;
   U8                            cellIndex  = 0;
   Bool                          freqPresent= FALSE;
   U8                            idx        = 0;

   WR_GET_CELLCB(cellCb, cellId)

      if(NULLP == cellCb)
      {
         RLOG1(L_ERROR, "Invalid Cell Id [%d] received for validating neighbour cell configurations",cellId);
         RETVALUE(RFAILED);
      }

   while(cellIndex < numCell)
   {
      switch(nbrCells[cellIndex].type)
      {
         case WR_NEIGH_CELL_EUTRA:
            {
               LwrEUTRANeighCell          *lwrNbrEutra = NULLP;
               WrEutranFreqCb             *neighFreqCb = NULLP;
               CmLList                    *neighFreqLnk = NULLP;

               lwrNbrEutra  = &nbrCells[cellIndex].t.eutraCell;
               neighFreqLnk = cellCb->eutranNbrFreqLst.first;
               /* Validating the frquency whether it is present in Neighbour
                * Eutra Frequency list or not */
               while(neighFreqLnk != NULLP)
               {
                  neighFreqCb = (WrEutranFreqCb*)neighFreqLnk->node;
                  if(neighFreqCb->dlEarfcn == lwrNbrEutra->earfcnDl)
                  {
                     freqPresent = TRUE;
                     break;
                  }
                  neighFreqLnk = neighFreqLnk->next;
               }
               if(FALSE == freqPresent)
               {
                  /* Frequency is not present in Ngh Eutra Frequency list, 
                   * so mark neighbour cell as Invalid neighbour */
                  RLOG2(L_WARNING, " Validation fails for Eutra Neighbour PCI %d\
                        because frequency %d is not present ",lwrNbrEutra->pci, lwrNbrEutra->earfcnDl);
                  nbrCellStatus[cellIndex] = FALSE;
               }
               else
               {
                  /* Freq is valid so now validate for PCI confusion. First of all 
                   * assume it as Valid PCI,so set the status as TRUE */
                  nbrCellStatus[cellIndex] = TRUE;
                  /* Check if duplicate PCI is present in the nbr cell configiuration list */
                  for(idx = 0; idx < numCell; idx++)
                  {
                     /* Checking the PCI with same type of cell.*/
                     if((idx != cellIndex) &&
                           (nbrCells[idx].type == WR_NEIGH_CELL_EUTRA))
                     {
                        /*If CGI(Plmn, here enbId is CellIdentity) and PCI are same 
                         * then Ignore both configuraton and mark them as invalid*/
                        if((TRUE == wrUtlPlmnsEqual((WrPlmnId *)&nbrCells[idx].t.eutraCell.enbPlmn,
                                    (WrPlmnId *)&nbrCells[cellIndex].t.eutraCell.enbPlmn)) &&
                              (nbrCells[idx].t.eutraCell.pci == lwrNbrEutra->pci) &&
                              (nbrCells[idx].t.eutraCell.enbId == lwrNbrEutra->enbId))
                        {
                           /* CGI and PCI are duplicate in the nbr cell configiuration list,
                            * so mark both as Invalid neighbor cell configuration*/
                           RLOG1(L_WARNING, " Validation fails for PCI %d because PCI is duplicate ",
                                 nbrCells[idx].t.eutraCell.pci);
                           nbrCellStatus[cellIndex] = FALSE;
                           nbrCellStatus[idx] = FALSE;
                        }
                     }
                  }
               }
            }
            break;
         case WR_NEIGH_CELL_UTRA_FDD:
            {
               LwrUtraNeighCell         *lwrNbrUtra = NULLP;
               WrUtraFreqCb             *neighFreqCb = NULLP;
               CmLList         *neighFreqLnk = NULLP;

               lwrNbrUtra  = &nbrCells[cellIndex].t.utranCell;
               neighFreqLnk = cellCb->utraFddNbrFreqLst.first;

               /* Validating the frquency whether it is present in Neighbour 
                * Utra FDD Frequency list or not */
               while(neighFreqLnk != NULLP )
               {
                  neighFreqCb = (WrUtraFreqCb*)neighFreqLnk->node;
                  if(neighFreqCb->t.utraFddFreq.arfcnDl == lwrNbrUtra->t.utraFdd.arfcnDl)
                  {
                     freqPresent = TRUE;
                     break;
                  }
                  neighFreqLnk = neighFreqLnk->next;
               }
               if(FALSE == freqPresent)
               {
                  /* Frequency is not present in Ngh Utra Frequency list,
                   * so mark it as Invalid UTRA neighbour */
                  RLOG2(L_WARNING, " Validation fails for UTRA Neighbour PSC %d\
                        because frequency %d is not present ",lwrNbrUtra->t.utraFdd.psc,lwrNbrUtra->t.utraFdd.arfcnDl);
                  nbrCellStatus[cellIndex] = FALSE;
               }
               else
               {
                  /* Freq is valid so now validate for duplicate PSC. First of all
                   * assume it as Valid PSC,so set the status as TRUE */
                  nbrCellStatus[cellIndex] = TRUE;
                  /* Check if duplicate PSC is present in the nbr cell configiuration list */
                  for(idx = 0; idx < numCell; idx++)
                  {
                     /* Checking the PSC with same type of cell.*/
                     if((idx != cellIndex) &&
                           (nbrCells[idx].type == WR_NEIGH_CELL_UTRA_FDD))
                     {
                        /*If CGI(Plmn, CellIdentity) and PSC are same then Ignore  
                         * both configuraton and mark them as invalid*/
                        if(TRUE == wrUtlPlmnsEqual((WrPlmnId *)&nbrCells[idx].t.utranCell.plmnId,
                                 (WrPlmnId *)&nbrCells[cellIndex].t.utranCell.plmnId)&&
                              (nbrCells[idx].t.utranCell.t.utraFdd.psc == lwrNbrUtra->t.utraFdd.psc) &&
                              (nbrCells[idx].t.utranCell.cId == lwrNbrUtra->cId))
                        {
                           /* CGI and PSC are duplicate in the nbr cell configiuration list,
                            * so mark both as Invalid neighbour cell configuration*/
                           RLOG1(L_WARNING, " Validation fails for PSC %d because PSC is duplicate ",
                                 nbrCells[idx].t.utranCell.t.utraFdd.psc);
                           nbrCellStatus[cellIndex] = FALSE;
                           nbrCellStatus[idx] = FALSE;
                        }
                     }
                  }
               }
            }
            break;
         case WR_NEIGH_CELL_UTRA_TDD:
            {
               LwrUtraNeighCell         *lwrNbrUtra = NULLP;
               WrUtraFreqCb             *neighFreqCb = NULLP;
               CmLList			*neighFreqLnk = NULLP;

               lwrNbrUtra  = &nbrCells[cellIndex].t.utranCell;
               neighFreqLnk = cellCb->utraTddNbrFreqLst.first;

               /* Validating the frquency whether it is present in Neighbour 
                * Utra FDD Frequency list or not */
               while(neighFreqLnk != NULLP )
               {
                  neighFreqCb = (WrUtraFreqCb*)neighFreqLnk->node;
                  if(neighFreqCb->t.utraTddFreq.arfcn == lwrNbrUtra->t.utraTdd.arfcn)
                  {
                     freqPresent = TRUE;
                     break;
                  }
                  neighFreqLnk = neighFreqLnk->next;
               }
               if(FALSE == freqPresent)
               {
                  /* Frequency is not present in Ngh Utra Frequency list,
                   * so mark it as Invalid UTRA neighbour */
                  RLOG2(L_WARNING, " Validation fails for UTRA Neighbour PSC %d\
                        because frequency %d is not present ",lwrNbrUtra->t.utraTdd.cellParamId,lwrNbrUtra->t.utraTdd.arfcn);
                  nbrCellStatus[cellIndex] = FALSE;
               }
               else
               {
                  /* Freq is valid so now validate for duplicate PSC. First of all
                   * assume it as Valid PSC,so set the status as TRUE */
                  nbrCellStatus[cellIndex] = TRUE;
                  /* Check if duplicate PSC is present in the nbr cell configiuration list */
                  for(idx = 0; idx < numCell; idx++)
                  {
                     /* Checking the PSC with same type of cell.*/
                     if((idx != cellIndex) &&
                           (nbrCells[idx].type == WR_NEIGH_CELL_UTRA_TDD))
                     {
                        /*If CGI(Plmn, CellIdentity) and PSC are same then Ignore  
                         * both configuraton and mark them as invalid*/
                        if(TRUE == wrUtlPlmnsEqual((WrPlmnId *)&nbrCells[idx].t.utranCell.plmnId,
                                 (WrPlmnId *)&nbrCells[cellIndex].t.utranCell.plmnId)&&
                              (nbrCells[idx].t.utranCell.t.utraTdd.cellParamId == lwrNbrUtra->t.utraTdd.cellParamId) &&
                              (nbrCells[idx].t.utranCell.cId == lwrNbrUtra->cId))
                        {
                           /* CGI and PSC are duplicate in the nbr cell configiuration list,
                            * so mark both as Invalid neighbour cell configuration*/
                           RLOG1(L_WARNING, " Validation fails for PSC %d because PSC is duplicate ",
                                 nbrCells[idx].t.utranCell.t.utraTdd.cellParamId);
                           nbrCellStatus[cellIndex] = FALSE;
                           nbrCellStatus[idx] = FALSE;
                        }
                     }
                  }
               }
            }
            break;
            /* cell config validate */
         case WR_NEIGH_CELL_GERAN:
            {
               LwrGERANNeighCell   *lwrNbrGeran = NULLP;
               WrGeranFreqCb       *neighFreqCb = NULLP;
               CmLList			     *neighFreqLnk = NULLP;

               lwrNbrGeran  = &nbrCells[cellIndex].t.geranCell;
               neighFreqLnk = cellCb->geranNbrFreqLst.first;

               /* Validating the frquency whether it is present in Neighbour 
                * geran Frequency list or not */
               while(neighFreqLnk != NULLP )
               {
                  neighFreqCb = (WrGeranFreqCb*)neighFreqLnk->node;
                  if(neighFreqCb->arfcn == lwrNbrGeran->bcchArfcn)
                  {
                     freqPresent = TRUE;
                     break;
                  }
                  neighFreqLnk = neighFreqLnk->next;
               }
               if(FALSE == freqPresent)
               {
                  /* Frequency is not present in Ngh Geran Frequency list,
                   * so mark it as Invalid GERAN neighbour */
                  RLOG1(L_WARNING, " Validation fails for GERAN Neighbour \
                        because frequency %d is not present ",lwrNbrGeran->bcchArfcn);
                  nbrCellStatus[cellIndex] = FALSE;
                  RETVALUE(RFAILED); 
               }
               else /* freqPresent */
               {
                  /* Freq is valid so now validate for duplicate arfcn & PLMN
                     combination */
                  nbrCellStatus[cellIndex] = TRUE;
                  for(idx = 0; idx < numCell; idx++)
                  {
                     if((idx != cellIndex) &&
                           (nbrCells[idx].type == WR_NEIGH_CELL_GERAN))
                     {
                        if((TRUE == wrUtlPlmnsEqual((WrPlmnId *)&nbrCells[idx].t.geranCell.plmnId, 
                                    (WrPlmnId *)&nbrCells[cellIndex].t.geranCell.plmnId)))
                        {
                           /* If PLMNs are same then Frequency should not be same */
                           if((nbrCells[idx].t.geranCell.bcchArfcn) == 
                                 (nbrCells[cellIndex].t.geranCell.bcchArfcn))
                           {
                              RLOG1(L_ERROR, " Validation fails because arfcn %d is duplicate with PLMN combination",
                                    nbrCells[idx].t.geranCell.bcchArfcn);
                              nbrCellStatus[cellIndex] = FALSE;
                              nbrCellStatus[idx] = FALSE;
                              RETVALUE(RFAILED); 
                           }
                           /* if PLMNs are same then nccPermitted per Cell for measurement configuration
                            * should not be different */
                           if((nbrCells[idx].t.geranCell.nccPermittedMeas) ^ 
                                 (nbrCells[cellIndex].t.geranCell.nccPermittedMeas))
                           {
                              RLOG0(L_ERROR, " Validation fails because nccPermittedMeas is different with same the PLMN");
                              nbrCellStatus[cellIndex] = FALSE;
                              nbrCellStatus[idx] = FALSE;
                              RETVALUE(RFAILED); 
                           }
                        }
                     }
                  }
               }/* freqPresent */
            }
            break;
            /* cell config validate */
         case WR_NEIGH_CELL_CDMA2K_1XRTT:
            /* No validation done for CDMA 1xRTT cell */
            nbrCellStatus[cellIndex] = TRUE;
            break;
         default:
            RETVALUE(ROK);
      }
      freqPresent = FALSE;
      cellIndex++;
   }
   RETVALUE(ROK);
}

/** @brief This function validates the SIB7 configuration request parameters
 *
 * @details validates GERAN Neighbor cell configuration request parameters
 *
 *     Function: wrEmmCfgGeranNeighCellValidate
 *
 *         Processing steps:
 *         - validate GERAN Neighbor cell configuration parameters
 *
 * @param [in]  sib7CfgGrp : SIB7 Configuration parameters
 * @return S16
 *          -# Success : ROK
 *          -# Failure   : RFAILED
 */
PRIVATE S16 wrEmmCfgGeranNeighCellValidate
(
 WrCellCb                 *cellCb,
 LwrGERANNeighCell        *geranCellCb
)
{
   WrGeranCellCb        *neighCellCbtmp = NULLP;
   CmLList              *listNode = NULLP;

   TRC2(wrEmmCfgGeranNeighCellValidate);

      neighCellCbtmp = (WrGeranCellCb *)CM_LLIST_FIRST_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,listNode);
      while(neighCellCbtmp != NULLP)
      {
         if((TRUE == wrUtlPlmnsEqual((WrPlmnId *)&geranCellCb->plmnId, 
                     (WrPlmnId *)&neighCellCbtmp->plmnId)))
         {
            /* If PLMNs are same then Frequency should not be same */
            if((geranCellCb->bcchArfcn) == 
                  (neighCellCbtmp->freqCb->arfcn))
            {
               RLOG1(L_ERROR, " Validation fails because arfcn %d is duplicate with PLMN combination",
                     geranCellCb->bcchArfcn);
               RETVALUE(RFAILED); 
            }
            /* if PLMNs are same then nccPermitted per Cell for measurement configuration
             * should not be different */
            if((geranCellCb->nccPermittedMeas) ^ 
                  (neighCellCbtmp->nccPermittedMeas))
            {
               RLOG1(L_ERROR, " Validation fails because nccPermittedMeas[%d]"
                     "is different with same the PLMN",
                     geranCellCb->nccPermittedMeas);
               RLOG3(L_DEBUG,"PLMNID[MCC = %d%d%d]",neighCellCbtmp->plmnId.mcc[0],
                     neighCellCbtmp->plmnId.mcc[1],neighCellCbtmp->plmnId.mcc[2]);
               RLOG3(L_DEBUG,"PLMNID[MNC = %d%d%d]",neighCellCbtmp->plmnId.mnc[0],
                     neighCellCbtmp->plmnId.mnc[1],neighCellCbtmp->plmnId.mnc[2]);
               RETVALUE(RFAILED); 
            }
         }
         neighCellCbtmp = (WrGeranCellCb *)CM_LLIST_NEXT_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,listNode);
      }

   RETVALUE(ROK);
}

/** @brief This function is responsible for addition of a neighbour cell at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgNeighCellAdd
 *
 *         Processing steps:
 *         - This is add a neighbour cell to the EnodeB 
 *
 * @param[in] neighCellAdd   : Neighbour cell list
 * @param[in] numCells       : Num of cells to be modified
 * @param[in] cellId         : serving Cell Id
 * @return U16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE S16 wrEmmCfgNeighCellAdd
(
LwrNeighCellAddCfg           *neighCellAdd,
U16                          numCells,
U8                           cellId
)
{
   S16                       ret = LCM_REASON_NOT_APPL;
   U8                        nrIndex = 0;
   WrEutranNeighCellCb       *neighCellCb =NULLP;
   WrNeighEnbCb              *neighEnCb= NULLP;
   WrUtraNeighCellCb          *utraCellCb = NULLP;   
   WrGeranCellCb             *geranCellCb = NULLP;

   WrCellCb                  *cellCb = NULLP;
   WrEmmEnodCfgParama        eNodCfgParams; 
   WrPhysCellIdRange        *pciRange=NULLP;
   Bool                      eUtranCell = FALSE; 
   /*Fix for ccpu00132820 : NeighCell Validation */
   Bool                      nbrCellStatus[MAX_NUM_NEIGH_CELLS] = {FALSE};
   Bool                      geranCellPres = FALSE; 
   U8                         geranMeasCnt =0;
   U32                        idx =0;
   WrUmmMeasGeranMeasInfo    geranMeasInfo[WR_UMM_MEAS_MAX_GERAN_ARFCN];
   WR_GET_CELLCB(cellCb, cellId);

   if(NULLP == cellCb)
   {
      RLOG1(L_ERROR,"Addition of Neighbour Cell failed.Invalid CellId [%d].",cellId);
      RETVALUE(RFAILED);
   }

   /* Call Anr to initialize the ANR-MEAS interface data structure 
    * to send the NR update info to Measurement 
    */
   wrEmmAnrInitMeasNrUpdtInfo(cellCb);
   
   pciRange=&(cellCb->sib4.csgPhyCellIdRange);
   eNodCfgParams.numEnb  = 0;
   /* Calling the validate function to check all incoming nbr cell configration
    * set the value of nbrCellStatus[idx] as FALSE if validation fails else set
    * as TRUE
    */
   /*ccpu00132820 Fix : NeighCell Validation */

   if(numCells <= MAX_NUM_NEIGH_CELLS) /* KW Fix */  
   {
      ret = wrEmmCfgValidateNeighCell(cellId, numCells, neighCellAdd, nbrCellStatus);
      /* by returing LCM_REASON_INVALID_PAR_VAL rejecting the neighbor cell configuration
       * just by setting neighCellStatus to FALSE and continuing for neighbor cell
       * add will not reject the entire configuration but it ignores the wrongly configured neighbor cell
       * and proceeds further */
      if(RFAILED == ret)
      {
         RLOG0(L_ERROR, "Neighbor cell Validation failed and rejecting the configuration");
         ret = LCM_REASON_INVALID_PAR_VAL;
         RETVALUE(ret);
      }
      while(nrIndex < numCells)
      {
         /*ccpu00132820 Fix : NeighCell Validation */
         if(FALSE != nbrCellStatus[nrIndex])
         {
            switch( neighCellAdd[nrIndex].type )
            {
               case WR_NEIGH_CELL_EUTRA:
                  {
                     /*Allocating Memory for the neighbour Control Block*/
                     WR_ALLOC(&neighCellCb, sizeof(WrEutranNeighCellCb));
                     if (neighCellCb == NULLP)
                     {
                        RLOG0(L_FATAL,"Memory allocation failed for neighbr cell while adding neighCell.");
                        RETVALUE(LCM_REASON_MEM_NOAVAIL);
                     }
                     cmLListInit(&(neighCellCb->neighNeighLstCp));
                     /*Copying  the values*/
                     wrEmmCopyEutranNeighCell(neighCellCb, &neighCellAdd[nrIndex],
                           cellCb->nrCb->maxRnkWght);

                     RLOG1(L_DEBUG,"Adding new Neighbor cell enbId [%lu]",
                           neighCellCb->enbId);
                     RLOG1(L_DEBUG,"Adding new Neighbor cell enbId [%lu]",
                           neighCellCb->enbId);

                     /*Finding the neighbour eNodeB control block*/
                     ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp),
                           (U8 *)&neighCellCb->enbId,sizeof(neighCellCb->enbId),
                           0,(PTR *)&neighEnCb);
                     if( ret != ROK)
                     {
                        neighEnCb = wrEmmAddNewNeighEnb(neighCellCb,cellId);
                        if(neighEnCb == NULLP)
                        {
                           RLOG1(L_ERROR,"Failed to add new Neighbour Enb.Cell id is %d",cellId);
                           RETVALUE(LCM_REASON_INVALID_PAR_VAL); 
                        }
                        if ( TRUE == neighEnCb->isX2Reqd )
                        {
                        neighEnCb->isX2Reqd = wrEmmIsAccessModeSame(cellCb->physCellId,
                              neighCellCb->pci,pciRange);                
                        }

                        if(neighEnCb->isX2Reqd == TRUE)
                        {
                           if(eNodCfgParams.numEnb <= MAX_NUM_NEIGH_CELLS)/* KW fix */
                           {
                              eNodCfgParams.neighEnbCb[eNodCfgParams.numEnb] = neighEnCb;
                              eNodCfgParams.numEnb++;
                           }
                           ret = ROK;
                        }
                     }
                     if (wrEmmFindServedNeighCell(neighEnCb, neighCellCb->pci ) ==  ROK)
                     {
                        RLOG1(L_ERROR,"Served Cell Node with" 
                              "pci[%d] already present",neighCellCb->pci);
                        WR_FREE(neighCellCb,sizeof(WrEutranNeighCellCb));
                        RETVALUE(LCM_REASON_INVALID_PAR_VAL);   
                     }
                     neighCellCb->srvdCellLstEnt.node = (PTR)neighCellCb;

                     /*Adding the neighbour Control block to neighbour eNodeB served cell list*/
                     cmLListAdd2Tail(&(neighEnCb->srvdCellLst), &neighCellCb->srvdCellLstEnt);

                     ret = wrEmmAnrAddEutranNr(cellCb, neighCellCb);

                     ret = wrEmmCfgHandleRetFailure( WR_NEIGH_CELL_EUTRA, ret);
                     /*inform the neighbor cell information to SON server as well*/
                     /*Neighbor cell information as part of initial configuration
                     is not required to inform to SON.SM will do the initial neighbor
                     configuration as well as dynamic neighbor configuration to SON.
                     This neighbor information mainly detected through ANR */
                     if((ROK == ret ) && (WR_NEIGH_CELL_CFG_ANR == neighCellAdd->cfgType))
                     {
                        wrIfmSonAddAnrNeighborReq((U32)cellId, neighCellCb);
                     } 
                  }
                  /* Only Neighor should be updated for EUTRAN Cell */
                  eUtranCell = TRUE; 

                  break;
               case WR_NEIGH_CELL_UTRA_FDD:
               case WR_NEIGH_CELL_UTRA_TDD:
                  {
                     WR_ALLOC(&utraCellCb, sizeof(WrUtraNeighCellCb));
                     if (utraCellCb == NULLP)
                     {
                        RLOG0(L_FATAL,"Failed to allocate memory for Utra cell.");
                        RETVALUE(LCM_REASON_MEM_NOAVAIL);
                     }

                     /*Copying the values from LWR to EMMCB - for UTRA FDD */
                     wrEmmCopyUtranNeighCell(utraCellCb, &neighCellAdd[nrIndex]);

                     ret = wrEmmAnrAddUtraNr(cellCb, utraCellCb);
                     if(ret == RFAILED)
                     {
                        RLOG0(L_ERROR,"Failed to add Utra neighbour cell in ANR");
                     }
                     for (idx=0; idx < utraCellCb->numPlmn; idx++)
                     {
                        wrUmmAddKPILaiList(cellCb->cellId, utraCellCb->plmnId[idx], utraCellCb->lac);
                     }
                  }
                  break;
               case WR_NEIGH_CELL_GERAN:
                  {
                     LwrGERANNeighCell    *geranCell;
                     ret = wrEmmCfgGeranNeighCellValidate(cellCb, &(neighCellAdd[nrIndex].t.geranCell));
                     if(ret == RFAILED)
                     {
                        RLOG0(L_ERROR,"Geran Neighbor cell Configuration Paramter(s) are wrong \n");
                        RETVALUE(ret);
                     }
                     WR_ALLOC(&geranCellCb, sizeof(WrGeranCellCb));
                     if (geranCellCb == NULLP)
                     {
                        RLOG0(L_FATAL,"Memory allocation failed while adding Geran neighbour Cell.");
                        RETVALUE(LCM_REASON_MEM_NOAVAIL);
                     }
                     ret = wrEmmCopyGeranNeighCell(geranCellCb, &neighCellAdd[nrIndex]);
                     if (ROK != ret)
                     {
                        RLOG0(L_ERROR,"Failed to copy Geran neighbour cell configurations");
                        RETVALUE(LCM_REASON_MEM_NOAVAIL);
                     }
                     /* Add WrGeranCellCb(geranCellCb) into wrcellCb->nrCb.geranCb*/
                     ret = wrEmmAnrAddGeranNr(cellCb, geranCellCb, neighCellAdd[nrIndex].t.geranCell.bcchArfcn);
                     wrUmmAddKPILaiList(cellCb->cellId, geranCellCb->plmnId, geranCellCb->lac);
                     geranCell = &(neighCellAdd[nrIndex].t.geranCell);
                     if(geranMeasCnt < WR_UMM_MEAS_MAX_GERAN_ARFCN)
                     {
                        geranMeasInfo[geranMeasCnt].arfcn = geranCell->bcchArfcn;
                        geranMeasInfo[geranMeasCnt].bsic = geranCell->bsic;
                        geranMeasInfo[geranMeasCnt].nccPermitted = geranCell->nccPermittedMeas;
                        geranMeasInfo[geranMeasCnt].bandIndicator = geranCell->bandIndicator;
                        geranMeasInfo[geranMeasCnt].qOffset = geranCell->offsetFreq;
                        geranCellPres = TRUE;
                        geranMeasCnt++;
                     }
                     else
                     {
                        RLOG2(L_FATAL,"geranMeasCnt[%d] exceeded maximum Geran arfcn [%d] supoorted",
                              geranMeasCnt,WR_UMM_MEAS_MAX_GERAN_ARFCN);
                     }
                  }
                  break;
               case WR_NEIGH_CELL_CDMA2K_1XRTT:
                  {
                     ret = wrEmmCfgNeighCellAddCdma1xRtt(&neighCellAdd[nrIndex],
                           cellCb);
                     if (ret == RFAILED)
                     {
                        RLOG0(L_WARNING, "Addition of CDMA 1x cell failed ");
                     }
                  }
                  break;
               default:
                  ret = LCM_REASON_INVALID_PAR_VAL;
            }
         }
         else
         {
            RLOG1(L_ERROR, " validation fails for nrIndex [%d] ",nrIndex);
         }
         nrIndex++;
      }
   }
   else
   {
      RLOG1(L_ERROR, " validation fails for nrIndex [%d] ",nrIndex);
      ret = RFAILED;
   }

   if(ret == ROK)
   {
      if(geranCellPres == TRUE)
      {
         wrUmmAddNrGeranObj(cellId, geranMeasCnt, geranMeasInfo);
      }
      /* Process NR Update Info and send Trigger to Meas and PBM module */
      ret = wrEmmAnrProcNrUpdtInfo(cellCb, FALSE);
      if(ROK == ret) 
      {
         ret = LCM_REASON_NOT_APPL;
         if(0 != eNodCfgParams.numEnb)
         {            
            wrEmmMmeBldEnbConfigTransfer(&eNodCfgParams,cellId);
         }
         if (TRUE == eUtranCell)
         {
            wrEmmNghTrigCfgUpd(cellCb); 
         }
      }
      else
      {
         ret = LCM_REASON_INVALID_PAR_VAL;
      }
   }
   else
   {
      ret = LCM_REASON_INVALID_PAR_VAL;
   }

/* LTE_ADV starts */
   if(WR_LTE_ADV_PICO_ENB == cellCb->lteAdvCb.lteAdvCfg.enbType)
   {
      if(RGR_ENABLE == cellCb->lteAdvCb.lteAdvCfg.reCfg.status)
      {
         wrEmmLteAdvUpdCellSpfcOffset(cellCb->cellId, RGR_RE, RGR_ENABLE);
      }
   }
/* LTE_ADV ends */

   RETVALUE(ret);
}

/** @brief This function is responsible for Modification of a neighbour
 *   cell at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgNeighCellMod
 *
 *         Processing steps:
 *         - Update the Neighbor cell params and extract the Algo info and send
 *           to ANR 
 *
 * @param[in] numCells       : number of cells
 * @param[in] neighCellMod   : Neighbour cell parameters 
 * @param[in] cellId         : Cell Id
 * @return U16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE S16 wrEmmCfgNeighCellMod
(
LwrNeighCellAddCfg           *neighCellMod,
U16                          numCells,
U8                           cellId
)
{
   S16                       ret = LCM_REASON_NOT_APPL;
   U8                        nrIndex = 0;
   WrUtraNeighCellCb           *utraCellCb = NULLP;
   WrEutranNeighCellCb        *eutranCellCb = NULLP;
   WrCellCb                  *cellCb = NULLP;
   WrEutranNbrCellInfo       nrInfo;
   U32                       reCfgType;
   U32                       mask = 1;
   WrEutraNghCellModInfo     modInfo = {0};
   Bool                      eutraCell = FALSE;
   

   WR_GET_CELLCB(cellCb, cellId);

   if(NULLP == cellCb)
   {
      RLOG1(L_ERROR,"Invalid CellId [%d] while modifying neighbour cell configurations.",cellId);
      RETVALUE(RFAILED);
   }

   RLOG1(L_INFO,"Modification of neighbor, Type is %d\n",
         neighCellMod[nrIndex].type);

   /* Call Anr to initialize the ANR-MEAS interface data structure 
    * to send the NR update info to Measurement 
    */
   wrEmmAnrInitMeasNrUpdtInfo(cellCb);
   
   while(nrIndex < numCells)
   {
      switch( neighCellMod[nrIndex].type )
      {
         case WR_NEIGH_CELL_EUTRA:
           {
               eutraCell = TRUE;
               reCfgType=neighCellMod[nrIndex].t.eutraCell.reCfgType;
               modInfo.mask = reCfgType;
               
               nrInfo.nrIndex   = neighCellMod[nrIndex].t.eutraCell.nrIndex;
               nrInfo.earfcn    = neighCellMod[nrIndex].t.eutraCell.earfcnDl; 
               nrInfo.phyCellId = neighCellMod[nrIndex].t.eutraCell.pci;
               nrInfo.cid       = neighCellMod[nrIndex].t.eutraCell.enbId;
               wrUtlCopyPlmnId((WrPlmnId *)&neighCellMod[nrIndex].t.eutraCell.enbPlmn, &nrInfo.plmnId);
               
               if(reCfgType & LWR_EUTRAN_NGH_CELL_PCI)
               {
                  RLOG0(L_INFO,"Neighbour Cell PCI is modified");
                  nrInfo.phyCellId = neighCellMod[nrIndex].t.eutraCell.oldPci;
                  modInfo.pci   = neighCellMod[nrIndex].t.eutraCell.pci;
                  reCfgType     =  reCfgType^LWR_EUTRAN_NGH_CELL_PCI;
               }
               if(reCfgType & LWR_EUTRAN_NGH_CELL_DL_EARFCN)
               {
                  RLOG0(L_INFO,"Neighbour Cell earfcn is  modified");
                  nrInfo.earfcn      = neighCellMod[nrIndex].t.eutraCell.oldEarfcnDl;
                  modInfo.earfcnDl   = neighCellMod[nrIndex].t.eutraCell.earfcnDl;
                  reCfgType          =  reCfgType^LWR_EUTRAN_NGH_CELL_DL_EARFCN;
               }
               
               eutranCellCb = wrEmmAnrGetEutranNr(&cellCb->nrCb->eutranCb, &nrInfo);

               if(NULLP == eutranCellCb)
               {
                  RLOG0(L_ERROR,"eutranCellCb is NULL.");
                  RETVALUE(RFAILED);
               }
               while(reCfgType)
               {
                  switch(reCfgType & mask)
                  {
                     case LWR_EUTRAN_NGH_CELL_PCI:
                     {
                        RLOG2(L_DEBUG,"Old PCI :[%d] new PCI :[%d]",
                                             eutranCellCb->pci,
                                        neighCellMod[nrIndex].t.eutraCell.pci);
                        eutranCellCb->pci =
                                        neighCellMod[nrIndex].t.eutraCell.pci;
                        reCfgType =  reCfgType^mask;
                        break;
                     }
                     case LWR_EUTRAN_NGH_CELL_TAC:
                     {
                        RLOG2(L_DEBUG,"Old TAC :[%d] new TAC :[%d]",
                                             eutranCellCb->tac,
                                        neighCellMod[nrIndex].t.eutraCell.tac);
                        eutranCellCb->tac = 
                                        neighCellMod[nrIndex].t.eutraCell.tac;
                        reCfgType =  reCfgType^mask;
                        break;
                     }
                     case LWR_EUTRAN_NGH_CELL_QOFFSET:
                     {
                        RLOG2(L_DEBUG,"Old Qoffset :[%d] new Qoffset :[%d]",
                                             eutranCellCb->qoffset,
                                    neighCellMod[nrIndex].t.eutraCell.qoffset);
                        eutranCellCb->qoffset= 
                                      neighCellMod[nrIndex].t.eutraCell.qoffset;
                        reCfgType =  reCfgType^mask;
                        break;
                     }
                     case LWR_EUTRAN_NGH_CELL_CSG_ID:
                     {
                        RLOG2(L_DEBUG,"Old CSG ID :[%ld] new CSG ID :[%ld]",
                                             eutranCellCb->csgId,
                                      neighCellMod[nrIndex].t.eutraCell.csgId);
                        eutranCellCb->csgId = 
                                      neighCellMod[nrIndex].t.eutraCell.csgId;
                        reCfgType =  reCfgType^mask;
                        break;
                     }
                     case LWR_EUTRAN_NGH_CELL_CSG_ACC_MODE:
                     {
                        RLOG2(L_DEBUG,"Old CSG Mode:[%d] new CSG mode:[%d]",
                                             eutranCellCb->csgAccessMode,
                              neighCellMod[nrIndex].t.eutraCell.csgAccessMode);
                        eutranCellCb->csgAccessMode= 
                              neighCellMod[nrIndex].t.eutraCell.csgAccessMode;
                        reCfgType =  reCfgType^mask;
                        break;
                     }
                     case LWR_EUTRAN_NGH_CELL_BLACK_LIST:
                     {
                        RLOG2(L_DEBUG,"Old blacklisted:[%d] new blacklisted:[%d]",
                                             eutranCellCb->blackListed,
                              neighCellMod[nrIndex].t.eutraCell.blackListed);
                        modInfo.blackListed = 
                             neighCellMod[nrIndex].t.eutraCell.blackListed;
                        reCfgType =  reCfgType^mask;
                        break;
                     }
                     default:
                     {
                        RLOG1(L_ERROR,"Neighbour cell reconfiguration received with "
                              "unknown reCfgType [%ld]",reCfgType);
                        if(reCfgType & mask)
                        {
                           reCfgType =  reCfgType^mask;
                        }
                     }
                  }
                  mask = mask<<1;
               }
               wrEmmAnrUpdtEutranKeyModInfo(cellCb,&modInfo,eutranCellCb);              
           }
           break;
         case WR_NEIGH_CELL_UTRA_FDD:
           {
              utraCellCb = wrEmmGetUtranNrEntry(cellCb, (LwrUtraCellDelCfg *)&neighCellMod[nrIndex].t.utranCell, WR_NEIGH_CELL_UTRA_FDD);
              wrEmmAnrUpdtUtranModInfo(cellCb, utraCellCb);
           }
            break;
         default:
            ret = LCM_REASON_INVALID_PAR_VAL;
      }

      nrIndex++;
   }

   if(ret == ROK) 
   {
      if(eutraCell ==  TRUE)
      {
         if((modInfo.mask & LWR_EUTRAN_NGH_CELL_BLACK_LIST) ||
            (modInfo.mask & LWR_EUTRAN_NGH_CELL_PCI) ||
            (modInfo.mask & LWR_EUTRAN_NGH_CELL_DL_EARFCN))
         {
            cellCb->nrCb->nrUpdtInfo->skipMeasUpt = FALSE;
         }
         else
         {
            cellCb->nrCb->nrUpdtInfo->skipMeasUpt = TRUE;
         }
      
         if((modInfo.mask & LWR_EUTRAN_NGH_CELL_PCI) ||
            (modInfo.mask & LWR_EUTRAN_NGH_CELL_QOFFSET)||
            (modInfo.mask & LWR_EUTRAN_NGH_CELL_BLACK_LIST)||
            (modInfo.mask & LWR_EUTRAN_NGH_CELL_DL_EARFCN))
         {
            cellCb->nrCb->nrUpdtInfo->skipSibsUpd = FALSE;
         }
         else
         {
            cellCb->nrCb->nrUpdtInfo->skipSibsUpd = TRUE;
         }
         
         /*Send eNodeB Configuration Update to other neighbour cell*/
         if((modInfo.mask & LWR_EUTRAN_NGH_CELL_PCI) ||
            (modInfo.mask & LWR_EUTRAN_NGH_CELL_DL_EARFCN))
         {
            if(wrEmmNghTrigCfgUpdNghCellMod(cellCb,eutranCellCb->enbId) != ROK)
            {
               RLOG0(L_INFO,"eNodeB Configuration Update Failed");
               ret = LCM_REASON_INVALID_PAR_VAL;
               RETVALUE(ret);
            }
         }

         if((modInfo.mask & LWR_EUTRAN_NGH_CELL_TAC) && 
               (0 == cellCb->nrCb->nrUpdtInfo->eutran.numFreq))
         {
            /* If only TAC is modified for neighbor cell, call this API to update
             * all the HRL UEs by RRC Reconfiguration msg, whereas in other case
             * wrEmmAnrProcNrUpdtInfo API will update the HRL UEs based on modified
             * TAC along with PCI/EARFCN/BLACK_LIST parameter updation */
            wrUpdtHrlUeCb(cellCb->cellId);
         }
      }
      
      /* Process NR Update Info and send Trigger to Meas and PBM module */
      ret = wrEmmAnrProcNrUpdtInfo(cellCb, FALSE);

      if(ret == ROK)
      {
         ret = LCM_REASON_NOT_APPL;
      }
      else
      {
         ret = LCM_REASON_INVALID_PAR_VAL;
      }
   }

   RETVALUE(ret);
}
/** @brief This function implements cell configuratio/reconfiguration/deletion.
 * 
 *
 * @details
 *
 *     Function: wrEmmCfgCellCfg
 *
 *         Processing steps:
 *         - if Addition of cell
 *          - Call add cell functionality
 *         - if Modication of cell
 *          - Call Mod cell functionality
 *         - if Deletion of cell
 *          - Call Del cell functionality
 *
 * @param[in] cellCfg : Cell configuration parameters 
 * @return U16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE U16 wrEmmCfgCellCfg 
(
LwrCellCfg                   *cellCfg
)
{
   U16                       reason = LCM_REASON_INVALID_PAR_VAL;

   switch (cellCfg->action)
   {
      case WR_ACT_ADD:
         reason = wrEmmCfgCellAdd (&(cellCfg->t.addCell));
         break;
      case WR_ACT_MOD:
         reason = wrEmmCfgCellMod (&(cellCfg->t.modCell));
         break;
      case WR_ACT_DEL:
         /*reason = wrEmmCfgCellDel (&(cellCfg->t.delCell));*/
         break;
      default:
         reason = LCM_REASON_INVALID_PAR_VAL;
   }

   RETVALUE(reason);

} /* end of wrEmmCfgCellCfg */ 

/** @brief This function handles MME configuration from the SM
 *
 * @details
 *
 *     Function: wrEmmCfgMmeCfg
 *
 *         Processing steps:
 *          - if Addition of mme
 *          - Call add mme functionality
 *         - if Modication of mme
 *          - Call Mod mee functionality
 *         - if Deletion of mme
 *          - Call Del mme functionality
 *
 *
 * @param[in] mmeCfg  : MME Configuration
 * @return U16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE U16 wrEmmCfgMmeCfg 
(
LwrMmeCfg                    *mmeCfg
)
{
   U16                       reason;

   switch (mmeCfg->action)
   {
      case WR_ACT_ADD:
         reason = wrEmmCfgMmeAdd (&(mmeCfg->t.addMme));
         break;
      case WR_ACT_MOD:
         reason = wrEmmCfgMmeMod (&(mmeCfg->t.modMme));
         break;
      case WR_ACT_DEL:
         reason = wrEmmCfgMmeDel (&(mmeCfg->t.delMme));
         break;
      default:
         reason = LCM_REASON_INVALID_PAR_VAL;
   }
   RETVALUE(reason);
} /* end of wrEmmCfgMmeCfg */ 


/** @brief This function handles neighbour cell configuration from the SM
 *
 * @details
 *
 *     Function: wrEmmCfgNeighCell
 *
 *         Processing steps:
 *          - if Addition of neighbour cell
 *          - Call add neighbour functionality
 *         - if Modication of neighbour cell
 *          - Call Mod neighbour cell functionality
 *         - if Deletion of neighbour cell
 *          - Call Del neighbour cell functionality
 *
 *
 * @param[in] neighCellCfg : neighbour cell configuration parameters
 * @return U16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE U16 wrEmmCfgNeighCell
(
LwrNeighCellCfg              *neighCellCfg
)
{
  U16                        reason;

   /* kw fixes */
   if(neighCellCfg->numCells <= MAX_NUM_NEIGH_CELLS)
   {
      switch (neighCellCfg->action)
      {
         case WR_ACT_ADD:
            reason = wrEmmCfgNeighCellAdd (neighCellCfg->t.neighAddCfg,
                  neighCellCfg->numCells, neighCellCfg->cellId);
            break;
         case WR_ACT_MOD:
            reason = wrEmmCfgNeighCellMod(neighCellCfg->t.neighModCfg,
                  neighCellCfg->numCells,  neighCellCfg->cellId);
            break;
         case WR_ACT_DEL:
            reason = wrEmmCfgNeighCellDelete (neighCellCfg->t.neighDelCfg,
                  neighCellCfg->numCells, neighCellCfg->cellId);

            break;
         default:
            reason = LCM_REASON_INVALID_PAR_VAL;
      }
   }
   else
   {
      RLOG0(L_WARNING, "neighCellCfg->numCells is greter than MAX_NUM_NEIGH_CELLS");
      reason = LCM_REASON_INVALID_PAR_VAL;
   }
   RETVALUE(reason);
} /* end of wrEmmCfgMmeCfg */ 



/** @brief This function handles eNodeB configuration from the SM
 *
 * @details
 *
 *     Function: wrEmmCfgEnbCfg
 *
 *         Processing steps:
 *          - if Addition of eNode Config
 *          - Call add eNode Config functionality
 *         - if Modication of eNode Config
 *          - Call Mod eNode Config functionality
 *         - if Deletion of eNode Config
 *          - Call Del eNode Config functionality
 *
 *
 * @param[in] enbCfg : eNodeB Configuration parameters 
 * @return U16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure   : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE U16 wrEmmCfgEnbCfg 
(
LwrENbCfg                    *enbCfg
)
{
   U16                       reason = LCM_REASON_INVALID_PAR_VAL;

   switch (enbCfg->action)
   {
      case WR_ACT_ADD:
         reason = wrEmmCfgEnbAdd (&(enbCfg->t.eNodeBCfg));
         break;
      case WR_ACT_MOD:
         reason = wrEmmCfgEnbMod (&(enbCfg->t.eNodeBReCfg));
         break;
      case WR_ACT_DEL:
         /*reason = wrEmmCfgEnbDel (&(mmeCfg->t.delMme));*/
         break;
      default:
         reason = LCM_REASON_INVALID_PAR_VAL;
   }
   RETVALUE(reason);
} /* end of wrEmmCfgMmeCfg */ 


/** @brief This function handles Neighbour configuration from the SM
 *
 * @details
 *
 *     Function: wrEmmCfgNeighCfg
 *
 *         Processing steps:
 *          - if Addition of neighbour
 *          - Call add neighbour functionality
 *         - if Modication of neighbour
 *          - Call Mod neighbour functionality
 *         - if Deletion of neighbour
 *          - Call Del neighbour functionality
 *
 * @param[in] neighCfg : neighbour cell configuration
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE U16 wrEmmCfgNeighCfg 
(
LwrNeighCfg                  *neighCfg
)
{
   U16                       reason;

   /* kw fixes */
   if(neighCfg->numEnb < LWR_MAX_NBR_ENB)
   {
      switch (neighCfg->action)
      {
         case WR_ACT_ADD:
            reason = wrEmmCfgNeigAdd (neighCfg->numEnb, neighCfg->t.neighAdd,
                  neighCfg->cellId);
            break;
         case WR_ACT_MOD:
            reason = wrEmmCfgNeigMod (neighCfg->numEnb, neighCfg->t.neighMod);
            break;
         case WR_ACT_DEL:
            reason = wrEmmCfgNeigDel (neighCfg->cellId, neighCfg->numEnb, neighCfg->t.neighDel);
            break;
         default:
            reason = LCM_REASON_INVALID_PAR_VAL;
      }
   }
   else
   {
      RLOG2(L_WARNING, "neighCfg->numEnb[%d] is greter than LWR_MAX_NBR_ENB[%d]",neighCfg->numEnb,LWR_MAX_NBR_ENB);
      reason = LCM_REASON_INVALID_PAR_VAL;
   }
   RETVALUE(reason);
} /* end of wrEmmCfgNeighCfg */ 

/** @brief This function is responsible for validating addition of neighbour 
 *   frequency at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgValidateNeighFreqAdd
 *
 *         Processing steps:
 *         - This is validate neighbor  frequencies to frequency List in Cell CB 
 *
 * @param[in] cellId     : cell for which NR needs to be added
 * @param[in] nbrFreqAdd : Neighbour parameters 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */

PRIVATE S16 wrEmmCfgValidateNeighFreqAdd
(
 U8                               cellId,
 LwrNeighFreqAddModCfg            *nbrFreqAdd
 )
{
   WrCellCb                      *cellCb  = NULLP;

   WR_GET_CELLCB(cellCb, cellId)

      if(NULLP == cellCb)
      {
         RLOG1(L_ERROR,"Invalid CellId [%d] received while validating neighbour frequency",cellId);
         RETVALUE(RFAILED);
      }

   switch(nbrFreqAdd->type)
   {
      case WR_NEIGH_CELL_EUTRA:
         {
            LwrNeighEutraFreq          *lwrNbeEutra = NULLP;
            WrEutranFreqCb             *neighFreqCb = NULLP;
            CmLList                    *neighFreqLnk = NULLP;

            lwrNbeEutra = &nbrFreqAdd->t.eutranFreq;
            neighFreqLnk = cellCb->eutranNbrFreqLst.first;
            while(neighFreqLnk != NULLP)
            {
               neighFreqCb = (WrEutranFreqCb*)neighFreqLnk->node;
               if(neighFreqCb->dlEarfcn == lwrNbeEutra->earfcnDl)
               {
                  RLOG1(L_DEBUG,"Eutra Cell Frequency Already Exists [%d]",neighFreqCb->dlEarfcn);
                  RETVALUE(RFAILED);
               }
               neighFreqLnk = neighFreqLnk->next;
            }
         }
         break;
      case WR_NEIGH_CELL_UTRA_FDD:
         {
            LwrNeighUtraFreq	 	*lwrUtra = NULLP;  
            WrUtraFreqCb              *neighFreqCb = NULLP;
            CmLList			*neighFreqLnk = NULLP;

            lwrUtra = &nbrFreqAdd->t.utranFreq;
            neighFreqLnk = cellCb->utraFddNbrFreqLst.first;

            while(neighFreqLnk != NULLP )
            {
               neighFreqCb = (WrUtraFreqCb*)neighFreqLnk->node;
               if(neighFreqCb->t.utraFddFreq.arfcnDl == lwrUtra->t.utraFdd.arfcnDl) 
               {
                  RLOG1(L_DEBUG,"Utra Fdd cell frequency already exists [%d]",neighFreqCb->t.utraFddFreq.arfcnDl);
                  RETVALUE(RFAILED);
               }  
               neighFreqLnk = neighFreqLnk->next;
            }
         }
         break;
      case WR_NEIGH_CELL_UTRA_TDD:
         {
            LwrNeighUtraFreq	 	*lwrUtra = NULLP;  
            WrUtraFreqCb              *neighFreqCb = NULLP;
            CmLList			*neighFreqLnk = NULLP;

            lwrUtra = &nbrFreqAdd->t.utranFreq;
            neighFreqLnk = cellCb->utraTddNbrFreqLst.first;

            while(neighFreqLnk != NULLP)
            {
               neighFreqCb = (WrUtraFreqCb *) neighFreqLnk->node;
               if(neighFreqCb->t.utraTddFreq.arfcn == lwrUtra->t.utraTdd.arfcn)
               {
                  RLOG1(L_DEBUG,"Utra Tdd cell frequency already exists [%d]",neighFreqCb->t.utraTddFreq.arfcn);
                  RETVALUE(RFAILED);
               }
               neighFreqLnk = neighFreqLnk->next;
            }

            if(ROK != wrEmmCfgValidateUtraTddFreq(lwrUtra))
            {
               RLOG1(L_ERROR,"Validation of Utra Tdd frequency failed:Configured arfcn value is invalid  =%d\n", lwrUtra->t.utraTdd.arfcn);
               RETVALUE(RFAILED);
            }
         }
         break;
      case WR_NEIGH_CELL_CDMA2K_1XRTT:
         {
            if(wrEmmCfgValidateNeighFreqAddCdma1xRtt(cellCb,
                     nbrFreqAdd) == RFAILED)
            {
               RLOG0(L_WARNING, " CDMA 1x frequency validation failed");
            }
         }
         break;
         /* Geran Case here */ 
      case WR_NEIGH_CELL_GERAN:
         {
            LwrNeighGERANFreq   *rcvGeranFreq = NULLP;  
            WrGeranFreqCb       *neighFreqCb = NULLP;
            CmLList			      *neighFreqLnk = NULLP;

            rcvGeranFreq = &nbrFreqAdd->t.geranFreq;
            neighFreqLnk = cellCb->geranNbrFreqLst.first;
            if(rcvGeranFreq->bcchArfcn >= 1024)
            {
               RLOG1(L_ERROR,"Invalid frequency received[%d] for adding neighbour frequency.",rcvGeranFreq->bcchArfcn);
               RETVALUE(RFAILED);
            }
            /* ------- 1. Band validation ------------*/
            if((rcvGeranFreq->bandIndicator > WR_GERAN_PCS1900) || 
                  (rcvGeranFreq->bandIndicator < WR_GERAN_GSM850))
            {
               /* received GERAN freq Bandindicator is invalid */
               RLOG1(L_ERROR," Invalid BandIndicator received[%d] for adding neighbour frequency "
                     ,rcvGeranFreq->bandIndicator);
               RETVALUE(RFAILED);
            }
            /* ------- 2. Band Range validation ------------*/
            switch(rcvGeranFreq->bandIndicator)
            {
               RLOG1(L_INFO,"BandIndicator Received is %d",rcvGeranFreq->bandIndicator);

               case WR_GERAN_GSM850:
                  /* GSM 850 Range: ARFCN 128 to 251 */
                  {
                     if((rcvGeranFreq->bcchArfcn < 128) || 
                           (rcvGeranFreq->bcchArfcn > 251))
                     {
                        /* received GERAN freq is out of GERAN band range */
                        RLOG1(L_ERROR,"Invalid Arfcn received[%d] for adding neighbour frequency"
                              ,rcvGeranFreq->bcchArfcn);
                        RETVALUE(RFAILED);
                     }
                     break;
                  }
               case WR_GERAN_GSM900:
                  /* E-GSM 900 Range: ARFCN 0 - 124 and 975 to 1023 */
                  {
                     if((rcvGeranFreq->bcchArfcn > 124) && 
                           (rcvGeranFreq->bcchArfcn < 975))
                     {
                        /* received GERAN freq is out of GERAN band range */
                        RLOG1(L_ERROR,"Invalid Arfcn received[%d] for adding neighbour frequency."
                              ,rcvGeranFreq->bcchArfcn);
                        RETVALUE(RFAILED);
                     }
                     break;
                  }
               case WR_GERAN_DCS1800:
                  /* DCS 1800 Range: ARFCN 512 - 885 */
                  {
                     if((rcvGeranFreq->bcchArfcn < 512) || 
                           (rcvGeranFreq->bcchArfcn > 885))
                     {
                        /* received GERAN freq is out of GERAN band range */
                        RLOG1(L_ERROR,"Invalid Arfcn received[%d] for adding neighbour frequency.",
                              rcvGeranFreq->bcchArfcn);
                        RETVALUE(RFAILED);
                     }
                     break;
                  }
               case WR_GERAN_PCS1900:
                  /* PCS 1900 Range: ARFCN 512 - 810 */
                  {
                     if((rcvGeranFreq->bcchArfcn < 512) ||
                           (rcvGeranFreq->bcchArfcn > 810))
                     {
                        /* received GERAN freq is out of GERAN band range */
                        RLOG1(L_ERROR,"Invalid Arfcn received[%d] for adding neighbour frequency.",
                              rcvGeranFreq->bcchArfcn);
                        RETVALUE(RFAILED);
                     }
                     break; 
                  }
               default:
                  /* This case is being added to avoid compiler warning. Control
                   * will not reach here.*/
                  RLOG0(L_ERROR,"Invalid Bandindicator received for adding neighbour frequency.");
                  break;
            }
            while(neighFreqLnk != NULLP )
            {
               neighFreqCb = (WrGeranFreqCb*)neighFreqLnk->node;
               /* ------- 3. validate for duplicate arfcn ------------*/
               if((neighFreqCb->arfcn == rcvGeranFreq->bcchArfcn) && 
                     (neighFreqCb->bandInd == rcvGeranFreq->bandIndicator))
               {
                  /*  received Duplicate GERAN freq configuration */
                  RLOG1(L_ERROR,"Duplicate Arfcn received for adding neighbour frequency.[%d] ",neighFreqCb->arfcn);
                  RETVALUE(RFAILED);
               }  
               /* ------- 4. validate for exclusive OR of DCS1800 & PSC1900 frequencies ------------*/
               /* if an in-use frequency and received frequency are either DCS1800 or PCS1900
                * then if the received frequency bandId should be equal to the
                * in-use frequency bandId only. checking with exclusive OR operation
                * if Exclusive-OR is true that means received and in-use are different 
                * */
                if((rcvGeranFreq->bandIndicator >= WR_GERAN_DCS1800) &&
                (neighFreqCb->bandInd >= WR_GERAN_DCS1800) &&
                (neighFreqCb->bandInd ^ rcvGeranFreq->bandIndicator))
                {
                  RLOG0(L_ERROR,"Frequencies of both DCS1800 and PCS1900 received for adding neighbour frequency. ");
                  RETVALUE(RFAILED);
               }
               neighFreqLnk = neighFreqLnk->next;
            }
         }
         break;
      default:
         RETVALUE(ROK);
   }

   RETVALUE(ROK);
}

/** @brief This function handles neighbor Frequency configuration from the SM
 *
 * @details
 *
 *     Function: wrEmmCfgNeighFreqCfg
 *
 *         Processing steps:
 *          - if Addition of Neighbor Frequency Config
 *          - Call add eNode Config functionality
 *         - if Modication of Neighbor Frequency Config
 *          - Call Mod eNode Config functionality
 *         - if Deletion of Neighbor Frequency Config
 *          - Call Del eNode Config functionality
 *
 * @param[in] neighFreqCfg : Neighbor Frequency Configuration parameters 
 * @return U16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure   : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE U16 wrEmmCfgNeighFreqCfg 
(
LwrNeighFreqCfg               *neighFreqCfg
)
{
   U16                        reason = LCM_REASON_INVALID_PAR_VAL;
   U16                        servArfcn = 0;

   /* kw fixes */
   if(neighFreqCfg->numFreq <= MAX_NUM_FREQ_CFG)
   {
      switch (neighFreqCfg->action)
      {
         case WR_ACT_ADD:
            reason = wrEmmCfgNeighFreqAdd (neighFreqCfg->cellId, 
                  neighFreqCfg->numFreq, neighFreqCfg->t.addNeighFreq);
            if(reason == LCM_REASON_NOT_APPL)
            {
               if(ROK == wrUmmAddNrFreq(neighFreqCfg->cellId,
                        neighFreqCfg->numFreq, neighFreqCfg->t.addNeighFreq))
               {
                  if (wrEmmAnrAddNrFreq(neighFreqCfg->cellId,
                           neighFreqCfg->numFreq, neighFreqCfg->t.addNeighFreq)!=  ROK)
                  {
                     RLOG0(L_ERROR,"Addition of neighbour frequency failed");
                     RETVALUE(LCM_REASON_INVALID_PAR_VAL);
                  }

               }
            }

            break;
         case WR_ACT_MOD:
            reason = wrEmmCfgNeighFreqMod (neighFreqCfg->cellId,
                  neighFreqCfg->numFreq, neighFreqCfg->t.modNeighFreq);
            servArfcn = wrEmmGetServCarrierFreq(neighFreqCfg->cellId);
                        
            if((reason == LCM_REASON_NOT_APPL) && 
               (servArfcn != neighFreqCfg->t.modNeighFreq[0].t.eutranFreq.earfcnDl))
            {
               wrUmmModNrFreq(neighFreqCfg->cellId,
                             neighFreqCfg->numFreq, neighFreqCfg->t.modNeighFreq);
            }
            break;
         case WR_ACT_DEL:
            reason = wrEmmCfgNeighFreqDel (neighFreqCfg->cellId,
                  neighFreqCfg->numFreq, neighFreqCfg->t.delNeighFreq);
            if(reason == LCM_REASON_NOT_APPL)
            {
               if(ROK == wrUmmDelNrFreq(neighFreqCfg->cellId,
                        neighFreqCfg->numFreq, neighFreqCfg->t.delNeighFreq))
               {
                  wrEmmAnrDelNrFreq(neighFreqCfg->cellId,
                        neighFreqCfg->numFreq, neighFreqCfg->t.delNeighFreq);
               }
            }

            break;
         default:
            reason = LCM_REASON_INVALID_PAR_VAL;
      }
   }
   else
   {
      RLOG2(L_WARNING, "neighFreqCfg->numFreq[%d] is greter than MAX_NUM_FREQ_CFG[%d]",neighFreqCfg->numFreq,MAX_NUM_FREQ_CFG);
      reason = LCM_REASON_INVALID_PAR_VAL;
   }

   RETVALUE(reason);
} /* end of wrEmmCfgNeighFreqCfg */ 



/** @brief This function is for cell configuration/reconfiguration.
 *
 * @details
 *
 *     Function: wrEmmCfgProtoCfg
 *
 *         Processing steps:
 *         - check the type of protocol configuration and call the corresponding 
 *            function to handle the  handle the configuration
 *
 * @param[in] cfg : Configuration parameters 
 * @return U16
 *  -# LCM_REASON_NOT_APPL
 *  -# LCM_REASON_INVALID_ELMNT
 *  -# LCM_REASON_INVALID_PAR_VAL
 */
PRIVATE U16 wrEmmCfgProtoCfg
(
LwrMngmt                     *cfg
)
{
   U16                       reason = LCM_REASON_NOT_APPL;
   U8                        type = cfg->t.protoCfg.type;

   switch (type)
   {
      case WR_CFG_TYPE_CELL:
         reason = wrEmmCfgCellCfg (&(cfg->t.protoCfg.t.cellCfg));
         break;
      case WR_CFG_TYPE_MME:
         reason = wrEmmCfgMmeCfg (&(cfg->t.protoCfg.t.mmeCfg));
         break;
      case WR_CFG_TYPE_NEIGH:
         reason = wrEmmCfgNeighCfg (&(cfg->t.protoCfg.t.neighCfg));
         break;
      case WR_CFG_TYPE_NEIGH_CELL:
         reason = wrEmmCfgNeighCell (&(cfg->t.protoCfg.t.neighCellCfg));
         break;
      case WR_CFG_TYPE_ENB:
         reason = wrEmmCfgEnbCfg (&(cfg->t.protoCfg.t.eNbCfg));
         break;
      case WR_CFG_TYPE_NEIGH_FREQ:
         reason = wrEmmCfgNeighFreqCfg (&(cfg->t.protoCfg.t.neighFreqCfg));
         break;
      case WR_CFG_TYPE_BAND_CLASS:
         reason = 
            wrEmmCfgNeighBandClassCfg(&(cfg->t.protoCfg.t.bandClassCfg));
         break;
      default:
         break;
   } /* end of switch */

   RETVALUE(reason);
} /* end of wrEmmCfgProtoCfg */ 



/** @brief This function handles the configuration request from the layer
 * manager towards the EnoodeB Application.
 *
 * @details
 * Configuration from the Stack manager can be of the following types 
 *
 * -# General configuration that corresponds to the Application
 * -# SAP configuration 
 * -# Cell configuration 
 * -# MME configuration 
 * -# Neighbour configuration 
 *
 *     Function: WrMiLwrCfgReq
 *
 *         Processing steps:
 *         - Based on the type of configuration branch out to GenCfg or CellCfg
 *         etc. 
 * @param[in]  cfg : Configuration parameters
 * @param[out] status : configuration status
 * @return S16
 *         -# Success : ROK
 */
PUBLIC S16 WrEmmCfgReq 
(
LwrMngmt                     *cfg,
CmStatus                     *status
)
{
   U16                       reason = LCM_REASON_NOT_APPL;
 
   switch (cfg->hdr.elmId.elmnt)
   {
      case STWRPROTOCFG:
         reason = wrEmmCfgProtoCfg (cfg);
         break;
      default:
         RLOG0(L_ERROR,"General configuraiton not done");
         reason = LCM_REASON_INVALID_ELMNT;
         break;
   } /* end of switch */
   
   if( reason == LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_OK;
      status->reason = reason;
   }
   else
   {
       status->status = LCM_PRIM_NOK;
       status->reason = reason;
   }
   
   RETVALUE(ROK);

} /* end of WrMiLwrCfgReq */ 

 /** @brief This function utility to get all neighbor 
 *   x2 ip from mme
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param  
 * @param 
 * @return 
 */


PUBLIC Void wrEmmMmeBldEnbConfigTranForAllNegh
(
   WrCellCb   *cellCb
)
{
   WrNeighEnbCb               *neighEnbCb = NULLP;
   WrEmmMmeEnbCfgTrnsfr       enbCfgTrnsfr;
   WrEutranNeighCellCb        *neighCellCb = NULLP;
   PTR                        prevEntry = NULLP;
   S16                        retVal = ROK;

   while ((retVal = cmHashListGetNext(&(wrEmmCb.neighEnbLstCp), prevEntry, (PTR *) &neighEnbCb)) == ROK)
   {
    /*once cell is up trigger enb config tansfer to get x2 peer ip address 
      SM can enable or disable x2 per neighbor enb implementation is custemer specific with radisy oam
   if x2 is enbale it will be enabled for all neighbor or it will be disabled for all neighbor*/
      if((neighEnbCb->isX2Reqd == TRUE) && (neighEnbCb->srvdCellLst.count != 0))
      {
        if(NULLP != neighEnbCb->srvdCellLst.first)
        {
           enbCfgTrnsfr.targetInfo.enbType = neighEnbCb->enbType;
           enbCfgTrnsfr.targetInfo.cellId  = neighEnbCb->enbId;

           neighCellCb = (WrEutranNeighCellCb*)neighEnbCb->srvdCellLst.first->node;
           enbCfgTrnsfr.targetInfo.tac     = neighCellCb->tac;
           enbCfgTrnsfr.targetInfo.plmnId  = neighCellCb->enbPlmn;
           
           enbCfgTrnsfr.sourceInfo.enbType = wrEmmCb.enbType;
           if(wrEmmCb.enbType == WR_ENODEB_TYPE_MACRO)
           {
              enbCfgTrnsfr.sourceInfo.cellId = (cellCb->sib1.cellIdentity)>>8;
           }
           else
           {
              enbCfgTrnsfr.sourceInfo.cellId = cellCb->sib1.cellIdentity;
           }
           RLOG1(L_INFO,"enbCfgTrnsfr.sourceInfo.cellId %lu ", enbCfgTrnsfr.sourceInfo.cellId);           
           enbCfgTrnsfr.sourceInfo.tac     = cellCb->sib1.tac;
           enbCfgTrnsfr.sourceInfo.plmnId  = cellCb->sib1.plmns[0].plmnId;
           enbCfgTrnsfr.isReply            = FALSE;
           neighEnbCb->enbCfgTransTriggerd = TRUE;
           if(ROK != wrEmmMmeEnbConfigTransfer(&enbCfgTrnsfr,wrEmmCb.enbCfgTransTmrVal,neighEnbCb))
           {
              RLOG0(L_ERROR,"Failed to send eNB configuration transfer to MME");
           }
        }
        else
        {
           RLOG0(L_WARNING, "neighEnbCb->srvdCellLst.first is NULL");
        }
      }
      prevEntry = (PTR )neighEnbCb;
   }
} /* end of wrEmmMmeBldEnbConfigTranForAllNegh*/  


 /** @brief This function utility to copy Barring info
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param  
 * @param 
 * @return 
 */
PUBLIC Void wrEmmMmeBldEnbConfigTransfer
(
   WrEmmEnodCfgParama         *enbCfgparama,
   U16                        cellId
)
{
   U8                         idxEnbNu  = 0;
   WrCellCb                   *cellCb;
   WrNeighEnbCb               *neighEnbCb = NULLP;
   WrEmmMmeEnbCfgTrnsfr       enbCfgTrnsfr;
   WrEutranNeighCellCb        *neighCellCb = NULLP;
      
   WR_GET_CELLCB(cellCb, cellId);
  
   if(NULLP == cellCb)
   {
      RLOG1(L_ERROR,"Invalid CellId received while sending eNB configurations to MME"
            "[%d].",cellId);
      RETVOID;
   }
 
   for(idxEnbNu = 0; idxEnbNu < enbCfgparama->numEnb; idxEnbNu++)
   {
      neighEnbCb = enbCfgparama->neighEnbCb[idxEnbNu];
      if((NULLP != neighEnbCb) && (neighEnbCb->srvdCellLst.count != 0))
      {
         neighCellCb = (WrEutranNeighCellCb*)neighEnbCb->srvdCellLst.first->node;

         enbCfgTrnsfr.targetInfo.enbType = neighEnbCb->enbType;
         enbCfgTrnsfr.targetInfo.cellId =  neighEnbCb->enbId;
         enbCfgTrnsfr.targetInfo.tac = neighCellCb->tac;
         wrUtlCopyPlmnId(&neighCellCb->enbPlmn, (WrPlmnId*)&enbCfgTrnsfr.targetInfo.plmnId);

         enbCfgTrnsfr.sourceInfo.enbType = wrEmmCb.enbType;
         if(wrEmmCb.enbType == WR_ENODEB_TYPE_MACRO)
         {
            enbCfgTrnsfr.sourceInfo.cellId = (cellCb->sib1.cellIdentity)>>8;
         }
         else
         {
            enbCfgTrnsfr.sourceInfo.cellId = cellCb->sib1.cellIdentity;
         }
         RLOG1(L_INFO,"enbCfgTrnsfr.sourceInfo.cellId %lu ", enbCfgTrnsfr.sourceInfo.cellId);
         enbCfgTrnsfr.sourceInfo.tac = cellCb->sib1.tac;
         wrUtlCopyPlmnId(&cellCb->sib1.plmns[0].plmnId, (WrPlmnId*)&enbCfgTrnsfr.sourceInfo.plmnId);

         enbCfgTrnsfr.isReply = FALSE;

         wrEmmMmeEnbConfigTransfer(&enbCfgTrnsfr,wrEmmCb.enbCfgTransTmrVal,neighEnbCb);
      }
      else
      {
         RLOG1(L_WARNING,"No eNodeB configurations found to send to MME [%d]",idxEnbNu);
         continue;
      }
   }
} /* end of wrEmmMmeBldEnbConfigTransfer*/  


 /** @brief 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param  
 * @param 
 * @return 
 */
PUBLIC S16 wrEmmSndLwrAddNeighCfm
(
LwrMngmt                     *cfg
)
{
   S16                 ret             = LCM_REASON_NOT_APPL;
   S16                 retVal          = ROK;
   WrNeighEnbCb        *neighEnCb      = NULLP;
   

   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp),
         (U8 *)&cfg->t.nbrAddCfm.enbId, sizeof(cfg->t.nbrAddCfm.enbId),0,(PTR *)&neighEnCb);
   if( ret != ROK)
   {
      RLOG1(L_ERROR,"Failed to find eNB control block for id [%lu]"
            ,cfg->t.nbrAddCfm.enbId);
      RETVALUE(RFAILED);
   } 
   /* update ip address */
   wrCpyCmTptAddr(&neighEnCb->neighEnbAddr, &cfg->t.nbrAddCfm.ipAddr);
   /* Before setting X2 interface check whether both serving and
   * neighbour cells are of same access mode or not 
   */
   if(neighEnCb->isX2Reqd == TRUE )
   {
      retVal = wrEmmNghEnbSndX2SetupReq(neighEnCb);
      if (retVal == RFAILED)
      {
         RLOG0(L_ERROR,"Failure occured while sending X2 setup request");
         return retVal;
      } 
   }
   else
   {
      RLOG0(L_WARNING, "neighEnCb->isX2Reqd is FALSE");
      RETVALUE(RFAILED);    
   }
   RETVALUE(ROK);
}
/** @brief This function copies the Nhu structure Plmn to Lwr structure Plmn
 *
 *  @details
 *
 *  Function: wrEmmCpyNhuPlmnToLwr
 *
 *  Processing steps:
 *     - Copy MCC and MNC to Lwr structure
 *
 *
 *  @param[in] enbPlmn : pointer to LwrPlmnId structure
 *  @param[in] plmnId  : pointer to NhuPLMN_Identity structure
 *
 *  @return U16
 *     -# ROK       : Success
 *     -# RFAILED   : Failure
 */
PRIVATE Void wrEmmCpyNhuPlmnToLwr
(
  LwrPlmnId                  *enbPlmn, 
  NhuPLMN_Identity           *plmnId
)
{
   U8                         idCnt;
   TRC2(wrEmmCpyNhuPlmnToLwr)

   for(idCnt = 0;idCnt < plmnId->mcc.noComp.val; idCnt++)
   {
     enbPlmn->mcc[idCnt] = plmnId->mcc.member[idCnt].val;
   }
   enbPlmn->numMncDigits = plmnId->mnc.noComp.val;
   for(idCnt = 0;idCnt < plmnId->mnc.noComp.val; idCnt++)
   {
     enbPlmn->mnc[idCnt] = plmnId->mnc.member[idCnt].val;
   }
}

/** @brief This function copies the neighbour cell information received 
 *  in Measurement report to WrEutranNeighCellCb
 *
 *  @details
 *
 *  Function:  wrEmmGetNbrEutraUlEarfcn
 *
 *  Processing steps:
 *     - Copy neighbour frequenncy to get UlArfcn
 *
 *
 *  @param[in] *cellCb  : pointer to CellCb
 *              earfcnDl: Neighbor Cell Dl Earfcn
 *        [out] earfcnUl: DL Earfcn for neighbor cell
 *
 *  @return Void
 */
PRIVATE Void wrEmmGetNbrEutraUlEarfcn
(
  WrCellCb   *cellCb,
  U16        earfcnDl, 
  U16        *earfcnUl
)
{
   WrEutranFreqCb             *neighFreqCb = NULLP;
   CmLList                    *neighFreqLnk = NULLP;

   neighFreqLnk = cellCb->eutranNbrFreqLst.first;
   while(neighFreqLnk != NULLP)
   {
      neighFreqCb = (WrEutranFreqCb*)neighFreqLnk->node;
      if(neighFreqCb->dlEarfcn == earfcnDl) 
      {
         /* Delete the Freq Details */
         *earfcnUl = neighFreqCb->ulEarfcn;
         break;
      }
      neighFreqLnk = neighFreqLnk->next;
   }
   RETVOID;
}/*End of wrEmmGetNbrEutraUlEarfcn */

/** @brief This function copies the neighbour cell information received 
 *  in Measurement report to WrEutranNeighCellCb
 *
 *  @details
 *
 *  Function:  wrEmmCopyEutranNeighCellFromMeasRpt
 *
 *  Processing steps:
 *     - Copy neighbour cell information
 *
 *
 *  @param[in] *eutranNeighCell : pointer to WrEutranNeighCellCb
 *              measRprt : Measurement Report received from UE 
 *              carrFreq : Carrier Frequency
 *
 *  @return U16
 *     -# ROK       : Success
 *     -# RFAILED   : Failure
 */
PRIVATE S16 wrEmmCopyEutranNeighCellFromMeasRpt
(
   U16                        cellId,
   NhuMeasResults             *measRprt,
   U16                        carrFreq,
LwrNeighCellAddCfg           *neighCell,
U8                           ueAccessStratRel
)
{
   U8                         idCnt;
   U32                         cellIdentity = 0;
   U16                         femtoPci; 
   U16                         maxPciVal = 0;
   WrPhysCellIdRange           *pciRange=NULLP;
   WrCellCb                   *cellCb;
   NhuMeasResultEUTRA         *measResultEUTRA;
   NhuMeasResultEUTRAcgi_Info *cgiInfo;
   U16                     mappedPciRange = 0;
   /* CSG_DEV */
   U32                        csgIdentity;
   NhuAdditionalSI_Info_r9    *additnlSiInfo_r9;
   U16                        smallcellStartPci;
   U16                        maxSmallcellPci;
   Bool                       isCsgPciCfgd = FALSE;

   WR_GET_CELLCB(cellCb, cellId);
   if( cellCb == NULLP )
   {
      RLOG1(L_ERROR,"Invalid CellId[%d] received while copying EUTRA neighbour cell"
            "configurations",cellId);
      RETVALUE(RFAILED);
   }

   pciRange=&(cellCb->sib4.csgPhyCellIdRange);

/* FIX for ccpu00132596 */
   if(TRUE == cellCb->sib4.csgCellInfoPres)
   {
      isCsgPciCfgd  = TRUE;
      mappedPciRange = wrEmmPciRangeMapping(pciRange);
      maxPciVal  = pciRange->startPhyCellID + (mappedPciRange - 1);
   }
   /* CSG_DEV */
   smallcellStartPci = cellCb->smallCellCsgPciRange.startPci;
   maxSmallcellPci   = cellCb->smallCellCsgPciRange.startPci + 
      cellCb->smallCellCsgPciRange.pciRange;
/* FIX for ccpu00132596 */

   neighCell->t.eutraCell.cellNum = cellId;
   neighCell->t.eutraCell.earfcnDl = carrFreq;
   wrEmmGetNbrEutraUlEarfcn(cellCb, carrFreq, 
         &neighCell->t.eutraCell.earfcnUl);
   neighCell->t.eutraCell.cio = WR_EMM_DEFAULT_OFFSET;
   neighCell->t.eutraCell.nrIndex  = 0;
   neighCell->t.eutraCell.dlBw = 0;
   neighCell->t.eutraCell.ulBw = 0;
   neighCell->t.eutraCell.qoffset = WR_EMM_DEFAULT_OFFSET;
   measResultEUTRA = &measRprt->measResultNeighCells.val.
                              measResultLstEUTRA.member[0];
   if(measResultEUTRA->pres.pres == PRSNT_NODEF)
   {
      cgiInfo = &measResultEUTRA->cgi_Info;
      neighCell->t.eutraCell.pci = measResultEUTRA->physCellId.val;
      femtoPci                   = measResultEUTRA->physCellId.val;
      if(cgiInfo->pres.pres == PRSNT_NODEF)
      {
         if(cgiInfo->cellGlobalId.pres.pres == PRSNT_NODEF)
         {
            if(cgiInfo->cellGlobalId.cellIdentity.pres ==
                  PRSNT_NODEF)
            {
               wrUtlGetU32FrmBSXL(&cellIdentity, &cgiInfo->cellGlobalId.cellIdentity);
               neighCell->t.eutraCell.enbId = cellIdentity;
               /* CSG_DEV */
               if(((isCsgPciCfgd == TRUE) &&
                        (femtoPci >= pciRange->startPhyCellID) && 
                        (femtoPci <= maxPciVal)) ||
                     ((femtoPci >= smallcellStartPci) &&
                      (femtoPci <= maxSmallcellPci)))
               {
                  neighCell->t.eutraCell.enbType = WR_ENODEB_TYPE_HOME;
                  neighCell->t.eutraCell.enbId  = (neighCell->t.eutraCell.enbId)
                     & 0x0fffffff;
               }
               else
               {
                  neighCell->t.eutraCell.enbType = WR_ENODEB_TYPE_MACRO;
               }
               wrEmmCpyNhuPlmnToLwr(&neighCell->t.eutraCell.enbPlmn, 
                        &cgiInfo->cellGlobalId.plmn_Identity);
            }

            if(cgiInfo->trackingAreaCode.pres == PRSNT_NODEF)
            {
               wrUtlGetU16FrmBSXL(&neighCell->t.eutraCell.tac,&cgiInfo->trackingAreaCode);
            }

            if(cgiInfo->plmn_IdentityLst.noComp.val != 0)
            {
               neighCell->t.eutraCell.numPlmn = cgiInfo->plmn_IdentityLst.noComp.val;
               for(idCnt = 0; idCnt < neighCell->t.eutraCell.numPlmn; idCnt++)
               {
                  wrEmmCpyNhuPlmnToLwr(&neighCell->t.eutraCell.plmnId[idCnt],
                        &cgiInfo->plmn_IdentityLst.member[idCnt]);
               }
            }
            /* CSG_DEV start */
            /*Rel 9 UE reporting CSG ID*/
            if(measResultEUTRA->measResult.extaddgrp_1.pres.pres == PRSNT_NODEF)
            {
               additnlSiInfo_r9 = 
                  &measResultEUTRA->measResult.extaddgrp_1.additionalSI_Info_r9;
               if(additnlSiInfo_r9->csg_Identity_r9.pres)
               {
                  wrUtlGetU32FrmBSXL(&csgIdentity,
                        &additnlSiInfo_r9->csg_Identity_r9);
                  neighCell->t.eutraCell.csgId = csgIdentity;
                  neighCell->t.eutraCell.isCsgIdResolved =
                     WR_EMM_NBR_CSG_ID_DISCOVERED;
                  /* determine accessmode */
                  neighCell->t.eutraCell.isCsgAccessModeResolved =
                     WR_EMM_CELL_ACCESS_MODE_DETERMINED;
                  if(carrFreq == cellCb->dlCarrierFreq)
                  {
                     wrEmmCsgSetNbrAccessMode(cellId,
                           measResultEUTRA->physCellId.val, TRUE,
                           &neighCell->t.eutraCell.csgAccessMode);
                  }
               }
            }
            /*Rel 9 UE not reporting CSG ID*/
            else if(ueAccessStratRel > NhuAccessStratumRlsrel8Enum)
            {
               neighCell->t.eutraCell.isCsgIdResolved =
                  WR_EMM_NBR_CSG_ID_DISCOVERED;
               neighCell->t.eutraCell.isCsgAccessModeResolved =
                  WR_EMM_CELL_ACCESS_MODE_DETERMINED;
               neighCell->t.eutraCell.csgAccessMode = WR_ENB_OPEN_ACCESS;
            }
            /*Rel 8 UE */
            else
            {
               neighCell->t.eutraCell.isCsgIdResolved =
                  WR_EMM_NBR_CSG_ID_NOT_DISCOVERED;
               neighCell->t.eutraCell.isCsgAccessModeResolved =
                  WR_EMM_CELL_ACCESS_MODE_NOT_DETERMINED;
               neighCell->t.eutraCell.csgAccessMode = WR_ENB_OPEN_ACCESS;
            }            
            /* CSG_DEV end */
         }
      }
   }
  
   RETVALUE(ROK);
}
/** @brief This function copies the neighbour cell information received 
 *  in Measurement report to WrEutranNeighCellCb
 *
 *  @details
 *
 *  Function:  wrEmmCopyUtranNeighCellFromMeasRpt
 *
 *  Processing steps:
 *     - Copy neighbour cell information
 *
 *
 *  @param[in] *eutranNeighCell : pointer to WrEutranNeighCellCb
 *              measRprt : Measurement Report received from UE 
 *              carrFreq : Carrier Frequency
 *
 *  @return U16
 *     -# ROK       : Success
 *     -# RFAILED   : Failure
 */
PRIVATE S16 wrEmmCopyUtranNeighCellFromMeasRpt
(
  U16                           cellId,
  NhuMeasResults                  *measRprt,
  U16                           carrFreq,
  LwrNeighCellAddCfg              *neighCell
)
{
   U8                         idCnt = 0;
   U32                        cellIdentity = 0;
   WrCellCb                   *cellCb;
   NhuMeasResultUTRA          *measResultUTRA;
   NhuMeasResultUTRAcgi_Info  *cgiInfo;

   WR_GET_CELLCB(cellCb, cellId);
   if( cellCb == NULLP )
   {
      RLOG1(L_ERROR,"Invalid CellId[%d] while copying Utran neighbour cell configurations"
            "from measurement report",cellId);
      RETVALUE(RFAILED);
   }

   neighCell->t.utranCell.cellNum = cellId;
   neighCell->t.utranCell.status = 0;
   neighCell->t.utranCell.nrIndex  = 0;
   neighCell->t.utranCell.curRank  = 10;
   neighCell->t.utranCell.prevRank  = 0;
   neighCell->t.utranCell.isVoipCapable  = FALSE;
   neighCell->t.utranCell.isPsHOCapable  = FALSE;
   measResultUTRA = &measRprt->measResultNeighCells.val.
                              measResultLstUTRA.member[0];
   if(measResultUTRA->pres.pres == PRSNT_NODEF)
   {
      cgiInfo = &measResultUTRA->cgi_Info;
      if(PHYSCELLID_FDD == measResultUTRA->physCellId.choice.val)
      {
         neighCell->t.utranCell.duplexMode = 0;
         neighCell->t.utranCell.t.utraFdd.psc = measResultUTRA->physCellId.val.fdd.val;
         neighCell->t.utranCell.t.utraFdd.arfcnDl = carrFreq;
         neighCell->t.utranCell.t.utraFdd.arfcnUl = 0;
      }
      else
      {
         neighCell->t.utranCell.duplexMode = 1;
         neighCell->t.utranCell.t.utraTdd.cellParamId = measResultUTRA->physCellId.val.tdd.val;
         neighCell->t.utranCell.t.utraTdd.arfcn = carrFreq;
      }
      if(cgiInfo->pres.pres == PRSNT_NODEF)
      {
         if(cgiInfo->cellGlobalId.pres.pres == PRSNT_NODEF)
         {
            if(cgiInfo->cellGlobalId.cellIdentity.pres == PRSNT_NODEF)
            {
               /*This logic shall taken from KT Requirement, RNC-ID(12-bit 
                * of MSB from cell Identity), CID(16-bit of LSB frmo Cell identity) */
               WR_GET_U32_FRM_OSXL(cellIdentity, cgiInfo->cellGlobalId.cellIdentity);
               neighCell->t.utranCell.rncId = cellIdentity >> 16; 
               neighCell->t.utranCell.cId = cellIdentity & 0x000ffff; 
            }
         }

         if(cgiInfo->locationAreaCode.pres == PRSNT_NODEF)
         {
            WR_GET_U32_FRM_OSXL(neighCell->t.utranCell.lac, cgiInfo->locationAreaCode);
         }
         if(cgiInfo->routingAreaCode.pres == PRSNT_NODEF)
         {
            WR_GET_U32_FRM_OSXL(neighCell->t.utranCell.rac, cgiInfo->routingAreaCode);
         }

         if(cgiInfo->plmn_IdentityLst.noComp.val != 0)
         {
            neighCell->t.utranCell.numPlmn = cgiInfo->plmn_IdentityLst.noComp.val;
            for(idCnt = 0; idCnt < neighCell->t.utranCell.numPlmn; idCnt++)
            {
               wrEmmCpyNhuPlmnToLwr(&neighCell->t.utranCell.plmnId[idCnt],
                     &cgiInfo->plmn_IdentityLst.member[idCnt]);
            }
         }
         if(cgiInfo->cellGlobalId.pres.pres == PRSNT_NODEF)
         {
            wrEmmCpyNhuPlmnToLwr(&neighCell->t.utranCell.plmnId[idCnt], 
                     &cgiInfo->cellGlobalId.plmn_Identity);
         }
      }
   }
   RETVALUE(ROK);
}

#ifndef WR_RSYS_OAM
/** @brief This function used to update New found cell to OAM.
 *  
 *  @details
 *
 *  Function:  wrEmmUpdateMIB
 *
 *  Processing steps:
 *     - Handle the Api
 *
 *  @param[in] numCells : Number of cells
 *             neighCellAdd : pointer to LwrNeighCellAddCfg object
 *
 *  @return U16
 *     -# ROK       : Success
 *     -# RFAILED   : Failure
 */
PRIVATE S16 wrEmmUpdateMIB
(
 U8                          numCells,
 LwrNeighCellAddCfg          *neighCellAdd
)
{
   /*This function used to update New found cell to OAM*/
   RETVALUE(ROK);
}
#endif

/** @brief This function is adds the neighbour cell Information
 *  received through measurement report 
 *  
 *  @details
 *
 *  Function:  wrEmmMeasReportCgiHdlr
 *
 *  Processing steps:
 *     - Add a neighbour cell to EnodeB
 *    
 *
 *  @param[in] cellId : Cell ID
 *             nrType : NR Type (Inter/Intra/UTRA)
 *             crnti  : CRNTI
 *             carrFreq : Frequency
 *             measRprt : Measurement Report rxd from UE
 *
 *  @return U16
 *     -# ROK       : Success
 *     -# RFAILED   : Failure
 */

PUBLIC S16 wrEmmMeasReportCgiHdlr
(
   U16                              cellId,
   U8                              nrType,
   U16                              carrFreq,
NhuMeasResults               *measRprt,
U8                           ueAccessStratRel
)
{
#ifndef WR_RSYS_OAM
   LwrNeighCellAddCfg        neighCellAdd = {0};
#else
   LwrMngmt                  *lwrMng = NULLP;
#endif
   /* CSG_DEV */
   WrCellCb                  *cellCb;
   WrEutranNeighCellCb       *eutranNrCellCb = NULLP;
   NhuMeasResultEUTRA        *measResultEUTRA = NULLP;
   WrEutranNbrCellInfo       eutraNrInfo;

   TRC2(wrEmmMeasReportCgiHdlr);

   /* CSG_DEV */
   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG1(L_ERROR,"Invalid Cell Id [%d] received in measurement report CGI handler",cellId);
      RETVALUE(RFAILED);
   }
 
#ifdef WR_RSYS_OAM
   WR_ALLOC(&lwrMng, sizeof(LwrMngmt));
   if(lwrMng == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed for LwrMngnt.");
      RETVALUE(RFAILED);

   }
   if((nrType == WR_UMM_MEAS_INTRA_FREQ) ||
     (nrType == WR_UMM_MEAS_INTER_FREQ))
   {
      /* CSG_DEV */
      measResultEUTRA = &measRprt->measResultNeighCells.val.
                            measResultLstEUTRA.member[0];
      eutraNrInfo.earfcn = carrFreq;
      eutraNrInfo.phyCellId = measResultEUTRA->physCellId.val;

      eutranNrCellCb = wrEmmAnrGetEutranNr(&cellCb->nrCb->eutranCb,
               &eutraNrInfo);
      if(NULLP == eutranNrCellCb)
      {
         /*Copying  the values*/
         wrEmmCopyEutranNeighCellFromMeasRpt(cellId, measRprt,carrFreq,
               &lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0],
               ueAccessStratRel); 
         lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0].type =
               WR_NEIGH_CELL_EUTRA;
      }
      else
      {
         RETVALUE(ROK); 
      }
   }/*manjappa*/
   else if(nrType == WR_UMM_MEAS_UTRA_FDD_FREQ) 
   {
      wrEmmCopyUtranNeighCellFromMeasRpt(cellId, measRprt,carrFreq,
                                &lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0]); 
      lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0].type =
            WR_NEIGH_CELL_UTRA_FDD;
   }
   lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0].cfgType = WR_NEIGH_CELL_CFG_ANR;
   wrEmmCfgNeighCellAdd(&lwrMng->t.protoCfg.t.neighCellCfg.t.neighAddCfg[0], 1, cellId);
   wrSendNghCellInfo(lwrMng);
   RETVALUE(ROK); 
#else
   if((nrType == WR_UMM_MEAS_INTRA_FREQ) ||
     (nrType == WR_UMM_MEAS_INTER_FREQ))
   {
      /* CSG_DEV */
      measResultEUTRA = &measRprt->measResultNeighCells.val.
                            measResultLstEUTRA.member[0];
      eutraNrInfo.earfcn = carrFreq;
      eutraNrInfo.phyCellId = measResultEUTRA->physCellId.val;

      eutranNrCellCb = wrEmmAnrGetEutranNr(&cellCb->nrCb->eutranCb,
               &eutraNrInfo);
      if(NULLP == eutranNrCellCb)
      {
         /*Copying  the values*/
         wrEmmCopyEutranNeighCellFromMeasRpt(cellId, measRprt,carrFreq,
               &neighCellAdd, ueAccessStratRel); 
      neighCellAdd.type = WR_NEIGH_CELL_EUTRA;  
      }
      else
      {
         RETVALUE(ROK); 
      }
   }
   else if(nrType == WR_UMM_MEAS_UTRA_FDD_FREQ) 
   {
      wrEmmCopyUtranNeighCellFromMeasRpt(cellId, measRprt,carrFreq,
            &neighCellAdd);
      neighCellAdd.type = WR_NEIGH_CELL_UTRA_FDD;  
   }
   neighCellAdd.cfgType = WR_NEIGH_CELL_CFG_ANR;  
   wrEmmCfgNeighCellAdd(&neighCellAdd, 1, cellId);
      wrEmmUpdateMIB(1, &neighCellAdd);
   RETVALUE(ROK); 
#endif
}

/** @brief This function handles the configuration request from the layer
 * manager towards the EnoodeB Application.
 *
 * @details
 * Configuration from the Stack manager can be of the following types 
 *
 * -# General configuration that corresponds to the Application
 * -# SAP configuration 
 * -# Cell configuration 
 * -# MME configuration 
 * -# Neighbour configuration 
 *
 *     Function: WrMiLwrCfgReq
 *
 *         Processing steps:
 *         - Based on the type of configuration branch out to GenCfg or CellCfg
 *         etc. 
 * @param[in]  cfg : Configuration parameters
 * @param[out] status : configuration status
 * @return S16
 *         -# Success : ROK
 */
PUBLIC S16 wrEmmDynCfgReq 
(
 LwrDynMngmt *cfg
)
{
   U16                       reason = LCM_REASON_NOT_APPL;
   WrCellCb                  *cellCb = NULLP;
   LwrNeighCellCfg           *neighCellCfg = NULLP;
   U8                        sibs[WR_MAX_SIBS];
   U8                        numSibs = 0;
   Bool                      trgPbm = FALSE;
   Bool                      cellReset = FALSE;
   S16                       ret = RFAILED;

   cellCb = wrEmmCb.cellCb[WR_DFLT_CELL_IDX];
  
   RLOG1(L_DEBUG, "Dynamic configuration of neighbor of Type %d", cfg->cfgType);
   switch (cfg->cfgType)
   {
      case LWR_DYN_EVNT_NEIGH_UTRA_FREQ_CFG:
      case LWR_DYN_EVNT_NEIGH_EUTRA_FREQ_CFG:
      {
        reason = wrEmmCfgNeighFreqCfg((LwrNeighFreqCfg*)cfg->dynCfg);
        WR_FREE(cfg->dynCfg, sizeof(LwrNeighFreqCfg));
      } 
      break;
      case LWR_DYN_EVNT_NEIGH_UTRA_CELL_CFG:
      {
        reason = wrEmmCfgNeighCell((LwrNeighCellCfg*)cfg->dynCfg);
        WR_FREE(cfg->dynCfg, sizeof(LwrNeighCellCfg));
      }
      break;
      case LWR_DYN_EVNT_NEIGH_EUTRA_CELL_CFG:
      {
         reason = wrEmmCfgNeighCell((LwrNeighCellCfg*)cfg->dynCfg);
         neighCellCfg = (LwrNeighCellCfg*)cfg->dynCfg;
         WR_FREE(cfg->dynCfg, sizeof(LwrNeighCellCfg));
       } 
      break;
      case LWR_DYN_EVNT_NEIGH_ENB_CFG:
        {
          reason = wrEmmCfgNeighCfg((LwrNeighCfg*)cfg->dynCfg);
          WR_FREE(cfg->dynCfg, sizeof(LwrNeighCfg));
        } 
      break;
      case LWR_DYN_EVNT_MME_IP_PRAMS:
         {
            reason = wrEmmCfgMmeCfg((LwrMmeCfg *)cfg->dynCfg);
            /* freeing the cfg params */
            WR_FREE(cfg->dynCfg, sizeof(LwrMmeCfg));
         }
         break;
      case LWR_DYN_EVNT_CELL_SIB1_CONFIG_GROUP_PRAMS:
         {

            if(ROK != wrEmmCfgCellModSib1(cellCb, (LwrCellSib1CfgGrp *) cfg->dynCfg))
            {
               RLOG0(L_ERROR,"SIB1 reconfiguration updation is Failed");
               reason = LCM_REASON_INVALID_PAR_VAL;
               break;
            }
            /* PH04_CMAS : added one more parameter. */
            if(ROK != wrEmmPbmSib1Config(cellCb->cellId, WR_SIB1_TRANS_ID))
            {
               RLOG0(L_ERROR,"PBM SIB1 updation is failed");
               reason = LCM_REASON_INVALID_PAR_VAL;
               break;
            }
            WR_FREE(cfg->dynCfg, sizeof(LwrCellSib1CfgGrp));

            /* Whenever contents of SIB1 changes other than addition/deletion of
             * SIB10 /SIB11 /SIB12 we need to do Paging for the UEs in order to
             * read the updated SIB1 */
            if(ROK != wrPbmStartPagingMsg(cellCb,WR_PAGE_SI_TYPE1))
            {
               RETVALUE(RFAILED);
            }
         }
         break;
      case LWR_DYN_EVNT_CELL_SIB2_CONFIG_GROUP_PRAMS:
         {
            ret = wrEmmCfgCellModSib2(cellCb,(LwrCellSib2CfgGrp *) cfg->dynCfg);
            if(ret == RFAILED)
            {
               RLOG0(L_ERROR,"SIB2 reconfiguration updation is Failed");
               reason = LCM_REASON_INVALID_PAR_VAL;
            }
            else
            {
               if((numSibs < WR_MAX_SIBS) && (ret != ROKIGNORE))/*kw Fix*/ 
               {
                  sibs[numSibs++] = WR_SIB_2;
                  trgPbm = TRUE;
               }
            }

            WR_FREE(cfg->dynCfg, sizeof(LwrCellSib2CfgGrp));
         }
         break;
      case LWR_DYN_EVNT_CELL_SIB3_CONFIG_GROUP_PRAMS:
         {
            if(ROK != wrEmmCfgCellModSib3(cellCb, (LwrCellSib3CfgGrp *) cfg->dynCfg))
            {
               RLOG0(L_ERROR,"SIB3 reconfiguration updation is failed");
               reason = LCM_REASON_INVALID_PAR_VAL;
            }
            else
            {
               if(numSibs < WR_MAX_SIBS)/*kw Fix*/ 
               {
                  sibs[numSibs++] = WR_SIB_3;
                  trgPbm = TRUE;
               }
            }
            WR_FREE(cfg->dynCfg, sizeof(LwrCellSib3CfgGrp));
         }
         break;
      case LWR_DYN_EVNT_CELL_SIB6_CONFIG_GROUP_PRAMS:
         {
            if(ROK != wrEmmCfgCellModSib6(cellCb, (LwrCellSib6CfgGrp *) cfg->dynCfg))
            {
               RLOG0(L_ERROR,"SIB6 reconfiguration updation is failed");
               reason = LCM_REASON_INVALID_PAR_VAL;
            }
            else
            {
               if(numSibs < WR_MAX_SIBS)/*kw Fix*/ 
               {
                  sibs[numSibs++] = WR_SIB_6;
                  trgPbm = TRUE;
               }
            }
            WR_FREE(cfg->dynCfg, sizeof(LwrCellSib6CfgGrp));
         }
         break;
      case LWR_DYN_EVNT_CELL_RABPOLICY_CONFIG_GROUP_PRAMS:
         {
            if (ROK != wrUmmReCfgRabParams (cellCb->cellId, (LwrRabPolicyCfgGrp *) cfg->dynCfg))
            {
               RLOG0(L_ERROR,"RAB POLICY reconfiguration updation is failed");
               reason = LCM_REASON_INVALID_PAR_VAL;
            }
            WR_FREE(cfg->dynCfg, sizeof(LwrRabPolicyCfgGrp));
         }
         break;
      case LWR_DYN_EVNT_ENB_PROTO_CONFIG_PRAMS:
         {
            if (ROK != wrEmmCfgEnbMod((LwrENodeBCfg *) cfg->dynCfg))
            {
               RLOG0(L_ERROR,"eNodeB Proto reconfiguration updation is failed");
               reason = LCM_REASON_INVALID_PAR_VAL;
            }
            WR_FREE(cfg->dynCfg, sizeof(LwrENodeBCfg));
         }
         break;
      case LWR_DYN_EVNT_CELL_DYN_CFI_CONFIG_PRAMS:
         {
            cellCb->schdCfg.isDynCfiEnb = 
               ((LwrCellSchdCfg *) cfg->dynCfg)->isDynCfiEnb;

            if(wrEmmCellSchdReCfgDynCfi(cellCb) != ROK)
            {   
               RLOG0(L_ERROR,"DYN CFI reconfiguration is failed");
               reason = LCM_REASON_INVALID_PAR_VAL;
            }   
            WR_FREE(cfg->dynCfg, sizeof(LwrCellSchdCfg));
         }
         break;
       case LWR_DYN_EVNT_CELL_TM_CONFIG_PRAMS:
         {
            cellCb->txPrefMode = ((LwrTxnModeInfo *) cfg->dynCfg)->txPrefMode;
            cellCb->isAutoCfgModeEnb = ((LwrTxnModeInfo *) cfg->dynCfg)->isAutoCfgModeEnb;
            if(wrEmmCellSchdReCfgTmMode(cellCb) != ROK)
            {
               RLOG0(L_ERROR,"Dynamic configuration of TM Mode failed");
               reason = LCM_REASON_INVALID_PAR_VAL;
            }
            WR_FREE(cfg->dynCfg, sizeof(LwrTxnModeInfo));
         }
         break;
      case LWR_DYN_EVNT_ADD_CELL_CONFIG_PRAMS:
         {
            LwrHCsgParam *hcsgParam = 
               ((LwrHCsgParam *)cfg->dynCfg);
            cellCb->minDlResNonCsg = hcsgParam->minDlResNonCsg;
            cellCb->minUlResNonCsg = hcsgParam->minUlResNonCsg;
            wrEmmSndSchdReCfgReq(cellCb->cellId,cellCb->minDlResNonCsg,
                  cellCb->minUlResNonCsg);
            WR_FREE(cfg->dynCfg, sizeof(LwrHCsgParam));
         }
         break;
      case LWR_DYN_EVNT_FAP_ACCS_PRAMS:
         {
            LwrHCsgParam *hcsgParam = 
               ((LwrHCsgParam *)cfg->dynCfg);
            cellCb->maxCsgUeSup    = hcsgParam->maxCsgUeSup;
            cellCb->maxNonCsgUeSup = hcsgParam->maxNonCsgUeSup;
            WR_FREE(cfg->dynCfg, sizeof(LwrHCsgParam));
         }
         break;
      default:
         {  
            RLOG1(L_ERROR,"Recived Dynamic configuration has No handling."
               "DynCfg type = [%u]", cfg->cfgType);
            cfg->cfm.status = LCM_PRIM_NOK;
            cfg->cfm.reason = reason;
            RETVALUE(ROK);
         }
         break;
   }

   if( reason == LCM_REASON_NOT_APPL && (WR_CELL_STATE_RESET != cellCb->cellState))
   {
      if(trgPbm)
      {
         if(ROK != wrPbmAddOrUpdateSibs(numSibs, sibs, cellCb->cellId))
         {
            RLOG0(L_ERROR,"PBM Updation is Failed");
            reason = LCM_REASON_INVALID_PAR_VAL;
         }
      }
   }

   if (cellReset)
   {
      if (ROK != wrEmmResetCell(cellCb->cellId))
      {
         RLOG0(L_ERROR,"Cell Reset is failed for Modifcation of phycelllId or"
            "dlcarriearFreq Change");
         reason = LCM_REASON_INVALID_PAR_VAL;
      }
   }


   if( reason == LCM_REASON_NOT_APPL)
   {
      cfg->cfm.status = LCM_PRIM_OK;
      cfg->cfm.reason = reason;
   }
   else
   {
      cfg->cfm.status = LCM_PRIM_NOK;
      cfg->cfm.reason = reason;
   }
   RETVALUE(ROK);
}

/** @brief This Function prints Neighbour Cell Frequency Control Block
 *
 * @details
 *
 *  Function:  wrEmmPrintNeighFreqCb1
 *
 *         Processing steps:
 *     - prints the Debug Information
 *
 *  @param[in] neighFreqCb : neighbour cell frequency control block
 *
 *  @return VOID
 */
PRIVATE Void wrEmmPrintNeighFreqCb1
(
    WrEutranFreqCb              *neighFreqCb
)
{
    if(!neighFreqCb)
        RETVOID;


    RLOG3(L_DEBUG,"Printing Eutran Neigh Freq Info."
         " sIntraSrchR9.pres [%d] sIntraSrchR9.sSearchPRel9 [%d]"
          " sIntraSrchR9.sSearchQRel9 [%d]"
       ,neighFreqCb->sIntraSrchR9.pres,neighFreqCb->sIntraSrchR9.sSearchPRel9
       ,neighFreqCb->sIntraSrchR9.sSearchQRel9);
    RLOG3(L_DEBUG,"sNonIntraSrchR9.pres [%d] sNonIntraSrchR9.sSearchPRel9 [%d]"
       " sNonIntraSrchR9.sSearchQRel9 [%d]",neighFreqCb->sNonIntraSrchR9.pres,
       neighFreqCb->sNonIntraSrchR9.sSearchPRel9,
       neighFreqCb->sNonIntraSrchR9.sSearchQRel9);
    RLOG3(L_DEBUG,"qQualMinR9 [%d] thSrvngLowQR9 [%d] threshXQR9.threshXHighQR9"
       " [%d]",neighFreqCb->qQualMinR9.val,neighFreqCb->thSrvngLowQR9.val
       ,neighFreqCb->threshXQR9.threshXHighQR9);
    RLOG1(L_DEBUG,"threshXQR9.threshXLowQR9 [%d]"
       ,neighFreqCb->threshXQR9.threshXLowQR9);

}

/** @brief This Function prints Neighbour Cell Frequency Control Block
 *  
 *  @details
 *
 *  Function:  wrEmmPrintNeighFreqCb
 *
 *  Processing steps:
 *     - prints the Debug Information
 *
 *  @param[in] neighFreqCb : neighbour cell frequency control block
 *
 *  @return VOID
 */
PRIVATE Void wrEmmPrintNeighFreqCb
(
 WrEutranFreqCb                *neighFreqCb
)
{
    if(!neighFreqCb)
        RETVOID;

    RLOG3(L_DEBUG,"status [%d] dlEarfcn [%d] ulEarfcn [%d]",neighFreqCb->status
       ,neighFreqCb->dlEarfcn,neighFreqCb->ulEarfcn);
    RLOG3(L_DEBUG,"cellReSelPri [%d] qRxLevMin [%d] pMaxPres [%d]"
       ,neighFreqCb->cellReSelPri,neighFreqCb->qRxLevMin,neighFreqCb->pMaxPres);
    RLOG3(L_DEBUG,"pMax [%d] tReselcxn [%d] sfPres [%d]",neighFreqCb->pMax,
       neighFreqCb->tReselcxn,neighFreqCb->sfPres);
    RLOG3(L_DEBUG,"sfMedium [%d] sfHigh [%d] sIntraSrch [%d]",
       neighFreqCb->sfMedium,neighFreqCb->sfHigh,neighFreqCb->sIntraSrch.val);
    RLOG3(L_DEBUG,"sNonIntraSrch [%d] thSrvngLow [%d] threshXHigh [%d]",
       neighFreqCb->sNonIntraSrch.val,neighFreqCb->thSrvngLow,
       neighFreqCb->threshXHigh);
    RLOG3(L_DEBUG,"threshXLow [%d] allowedMeasBW [%d] presenceAntPort1 [%d]"
       ,neighFreqCb->threshXLow,neighFreqCb->allowedMeasBW.val,
       neighFreqCb->presenceAntPort1);
    RLOG3(L_DEBUG,"neighCellConfig [%d] qOffsetFreq [%d] offsetFreq [%d]"
       ,neighFreqCb->neighCellConfig,neighFreqCb->qOffsetFreq,
       neighFreqCb->offsetFreq);

    wrEmmPrintNeighFreqCb1(neighFreqCb);
}

/** @brief This Function prints cell timer configuration
 *  
 *  @details
 *
 *  Function:  wrEmmPrintCellTmrCfg
 *
 *  Processing steps:
 *     - prints the Debug Information
 *
 *  @param[in] cellCb : Cell's control block
 *
 *  @return VOID
 */
PRIVATE Void wrEmmPrintCellTmrCfg
(
  WrCellCb                    *cellCb
)
{
   RLOG3(L_DEBUG,"RRC Connection Setup Timer = [%ld] RRC ReconfigTmr = [%ld] RRC rrcConnectionReEstablishTmr =[%ld]",
      cellCb->tmrCfg.rrcConnectionSetupTmr,cellCb->tmrCfg.rrcConnectionReconfigTmr,cellCb->tmrCfg.rrcConnectionReEstablishTmr);
   RLOG3(L_DEBUG,"RRC SecurityModeCommandTmr = [%ld] RRC UeCapabilityEnquiryTmr"
      " = [%ld] RRC ConnectionReleaseTmr= [%ld]",
      cellCb->tmrCfg.rrcSecurityModeCommandTmr,
      cellCb->tmrCfg.rrcUeCapabilityEnquiryTmr,
      cellCb->tmrCfg.rrcConnectionReleaseTmr);
   RLOG3(L_DEBUG,"RRC HandoverPreperationTmr = [%ld] RRC UeInformationRequestTmr"
      "= [%ld] S1InitialContextSetupTmr= [%d]",
      cellCb->tmrCfg.rrcHandoverPrepTmr,
      cellCb->tmrCfg.rrcUeInformationRequestTmr,
      cellCb->tmrCfg.s1InitialContextSetupTmr);
   RLOG3(L_DEBUG,"s1ErabReleaseIndicationTmr = [%d] s1UeContextReleaseTmr= [%d]"
      " s1PathSwitchTmr = [%d]",
       cellCb->tmrCfg.s1ErabReleaseIndicationTmr,
       cellCb->tmrCfg.s1UeContextReleaseTmr,cellCb->tmrCfg.s1PathSwitchTmr);
   RLOG3(L_DEBUG,"s1HandoverPreperationTmr [%d] s1RelocOveallTmr [%d]"
      " s1HandoverCancelTmr[%d]",
      cellCb->tmrCfg.s1HandoverPrepTmr,
      cellCb->tmrCfg.s1RelocOverallTmr,cellCb->tmrCfg.s1HandoverCancelTmr);
   RLOG3(L_DEBUG,"s1HandoverPreperationTmr [%d] x2HandoverPreperationTmr = [%d]"
      "  x2RelocOverallTmr = [%d]",cellCb->tmrCfg.s1HandoverPrepTmr,
      cellCb->tmrCfg.x2HandoverPrepTmr,cellCb->tmrCfg.x2RelocOverallTmr);
   RLOG3(L_DEBUG,"x2StoreUeContextTmr = [%d] x2SnStatusTransferTmr = [%d] "
      "internalSubCellChangeTmr = [%d]",cellCb->tmrCfg.x2StoreUeContextTmr,
      cellCb->tmrCfg.x2SnStatusTransferTmr,
      cellCb->tmrCfg.internalSubCellChangeTmr);
   RLOG3(L_DEBUG,"internalSubCellPdcpControlTmr = [%d] ehrpdOptHoCompleteTmr="
      " [%d] intraEnbHoCompleteTmr= [%d]",
      cellCb->tmrCfg.internalSubCellPdcpControlTmr,
      cellCb->tmrCfg.ehrpdOptHoCompleteTmr,
      cellCb->tmrCfg.intraEnbHoCompleteTmr);
   RLOG1(L_DEBUG,"measurementReportToWaitTmr = [%d]",
      cellCb->tmrCfg.measurementReportToWaitTmr);
    
}

/** @brief This Function prints cell Access Info
 *  
 *  @details
 *
 *  Function:  wrEmmPrintCellAccessInfo
 *
 *  Processing steps:
 *     - prints the Debug Information
 *
 *  @param[in] sib1 : sib's parameter
 *
 *  @return VOID
 */
PRIVATE Void wrEmmPrintCellAccessInfo
(
  WrSib1Params              *sib1
)
{

   RLOG2(L_DEBUG,"Printing SIB1 Cell Access Info:PLMN_IDX=[%d]"
         "ResvForOperUse [%d]", sib1->numPlmnIds,
         sib1->plmns[sib1->numPlmnIds].resvForOperUse);

   RLOG2(L_DEBUG,"SIB1:PLMN_IDX=[%d] Activated [%d]",
                sib1->numPlmnIds,
         sib1->plmns[sib1->numPlmnIds].activated);

   RLOG2(L_DEBUG,"SIB1:PLMN_IDX=[%d] NumMNC Digits [%d]",
                sib1->numPlmnIds,
         sib1->plmns[sib1->numPlmnIds].plmnId.numMncDigits);

   RLOG1(L_DEBUG,"SIB1:PLMN_IDX=[%d]",sib1->numPlmnIds);
   RLOG3(L_DEBUG," MCC[%d][%d][%d]",
                sib1->plmns[sib1->numPlmnIds].plmnId.mcc[0],
                sib1->plmns[sib1->numPlmnIds].plmnId.mcc[1],
         sib1->plmns[sib1->numPlmnIds].plmnId.mcc[2]);

   RLOG1(L_DEBUG,"SIB1:PLMN_IDX=[%d]",sib1->numPlmnIds);
   RLOG3(L_DEBUG," MNC[%d][%d][%d]",
                sib1->plmns[sib1->numPlmnIds].plmnId.mnc[0],
                sib1->plmns[sib1->numPlmnIds].plmnId.mnc[1],
         sib1->plmns[sib1->numPlmnIds].plmnId.mnc[2]);

}

/** @brief This Function prints sib's Access Info
 *  
 *  @details
 *
 *  Function:  wrEmmPrintSib1AccessInfo
 *
 *  Processing steps:
 *     - prints the Debug Information
 *
 *  @param[in] sib1 : sib's parameter
 *
 *  @return VOID
 */
PRIVATE Void wrEmmPrintSib1AccessInfo
(
  WrSib1Params              *sib1
)
{
   RLOG3(L_DEBUG,"Printing SIB1 Access Info:Tac [%d] CellIdentity [%d]" 
         "CellBarred [%d]",sib1->tac,sib1->cellIdentity,sib1->cellBarred);

   RLOG3(L_DEBUG,"SIB1:IntraFreqReselection [%d] CsgInd [%d] CSGID:Pres [%d]"
      ,sib1->intraFreqReselection,sib1->csgInd,sib1->csgId.pres);

   RLOG1(L_DEBUG,"SIB1:CSGID:Val [%d]",sib1->csgId.val);

}

/** @brief This Function prints cell selection info
 *  
 *  @details
 *
 *  Function:  wrEmmPrintCellSelectInfo
 *
 *  Processing steps:
 *     - prints the Debug Information
 *
 *  @param[in] sib1 : sib's parameter
 *
 *  @return VOID
 */
PRIVATE Void wrEmmPrintCellSelectInfo
(
  WrSib1Params              *sib1
)
{

   RLOG3(L_DEBUG,"Printing Cell Select Info.Sib1 qRxLevMin = [%d]"
         "qRxLevMinOffsetPres = [%d]  qRxLevMinOffset = [%d]",
      sib1->qRxLevMin,sib1->qRxLevMinOffsetPres,sib1->qRxLevMinOffset);
}

/** @brief This Function prints cell TDD cfg info
 *  
 *  @details
 *
 *  Function:  wrEmmPrintCellTddCfg
 *
 *  Processing steps:
 *     - prints the Debug Information
 *
 *  @param[in] sib1 : sib's parameter
 *
 *  @return VOID
 */
PRIVATE Void wrEmmPrintCellTddCfg
(
  WrSib1Params              *sib1
   )
{
   RLOG2(L_DEBUG,"Printing TDD Cell Configuration."
         "SIB1:SubframeCfg [%d] SpecialSubframeCfg [%d]"
      ,sib1->tddCfg.subframeCfg,sib1->tddCfg.specialSubframeCfg);

}

/** @brief This Function prints cell UL power control configuration Info
 *  
 *  @details
 *
 *  Function:  wrEmmPrintUlPwrCtrlCfg
 *
 *  Processing steps:
 *     - prints the Debug Information
 *
 *  @param[in] sib2 : sib's parameter
 *
 *  @return VOID
 */
PRIVATE Void wrEmmPrintUlPwrCtrlCfg(WrSib2Params *sib2)
{

   RLOG3(L_DEBUG,"Printing Ul Pwr Control Configuration."
         "SIB2:p0NominalPusch [%d] alpha [%d] p0NominlPucch [%d]"
         ,sib2->ulPwrCtrlCfg.p0NominalPusch,sib2->ulPwrCtrlCfg.alpha,
          sib2->ulPwrCtrlCfg.p0NominlPucch);
   RLOG3(L_DEBUG,"SIB2:deltaPeambleMsg3 [%d] deltaFPucchFormat1 [%d] "
      "deltaFPucchFormat1b [%d]"
      ,sib2->ulPwrCtrlCfg.deltaPeambleMsg3,sib2->ulPwrCtrlCfg.deltaFPucchFormat1
      ,sib2->ulPwrCtrlCfg.deltaFPucchFormat1b);
   RLOG3(L_DEBUG,"SIB2:deltaFPucchFormat2 [%d] deltaFPucchFormat2a [%d]"
      " deltaFPucchFormat2b [%d]",sib2->ulPwrCtrlCfg.deltaFPucchFormat2,
      sib2->ulPwrCtrlCfg.deltaFPucchFormat2a,
      sib2->ulPwrCtrlCfg.deltaFPucchFormat2b);
   RLOG1(L_DEBUG,"SIB2:p0NominalPusch [%d]",sib2->ulPwrCtrlCfg.p0NominalPusch);
}

/** @brief This Function prints cell UL Prach Configuartion
 *  
 *  @details
 *
 *  Function:  wrEmmPrintPrachCfg
 *
 *  Processing steps:
 *     - prints the Debug Information
 *
 *  @param[in] sib2 : sib's parameter
 *
 *  @return VOID
 */
PRIVATE Void wrEmmPrintPrachCfg(WrSib2Params *sib2)
{
   RLOG3(L_DEBUG,"Printing Prach Configuration for SIB2:RootSeqIdx [%d] PrachCfgIdx [%d]"
         "HighSpeedFlag [%d]",sib2->prachCfg.rootSeqIdx,
         sib2->prachCfg.prachCfgIdx, sib2->prachCfg.highSpeedFlag);
   RLOG2(L_DEBUG,"SIB2:ZeroCorrZone [%d] PrachFreqOffset [%d]",
      sib2->prachCfg.zeroCorrZone,sib2->prachCfg.prachFreqOffset);
}
 
/** @brief This function is responsible for copying the geran neighbour cell 
 *              info to neighbour cell control block
 *
 * @details
 *
 *     Function: wrEmmCopyGeranNghOamInd
 *
 *         Processing steps:
 *         - 
 *
 * @param[out] GeranNeighCell  : Geran neighbour cell Control block
 * @param[in]  neighCell : Geran Neighbour Cell parameters           
 * @return void 
 *   
 */

PRIVATE Void wrEmmCopyGeranNghOamInd
(
LwrNghCellInd    *geranNeighInd,
WrGeranCellCb    *nrtNgCell
)
{
   geranNeighInd->t.neighAddCfg.type = WR_NEIGH_CELL_GERAN;
   geranNeighInd->t.neighAddCfg.t.geranCell.lac = nrtNgCell->lac;
   geranNeighInd->t.neighAddCfg.t.geranCell.rac = nrtNgCell->rac;
   geranNeighInd->t.neighAddCfg.t.geranCell.bsic = nrtNgCell->bsic;
   geranNeighInd->t.neighAddCfg.t.geranCell.ci = nrtNgCell->cId;
   geranNeighInd->t.neighAddCfg.t.geranCell.nccPermittedMeas = nrtNgCell->nccPermittedMeas;
   geranNeighInd->t.neighAddCfg.t.geranCell.isDTMCapable = nrtNgCell->isDtmCapable;
   geranNeighInd->t.neighAddCfg.t.geranCell.rimSupportForGeranNgh = nrtNgCell->isRimSupported;
   geranNeighInd->t.neighAddCfg.t.geranCell.offsetFreq = nrtNgCell->offsetFreq;
   geranNeighInd->t.neighAddCfg.t.geranCell.ncoPres = nrtNgCell->nco.pres;
   geranNeighInd->t.neighAddCfg.t.geranCell.bcchArfcn = nrtNgCell->freqCb->arfcn;
   geranNeighInd->t.neighAddCfg.t.geranCell.bandIndicator = nrtNgCell->freqCb->bandInd;

   wrUtlCopyPlmnId(&nrtNgCell->plmnId, (WrPlmnId*)&geranNeighInd->t.neighAddCfg.t.geranCell.plmnId);
}

/** @brief This function is responsible for copying the eutrana neighbour cell 
 *              info to neighbour cell control block
 *
 * @details
 *
 *     Function: wrEmmCopyEutraNghOamInd
 *
 *         Processing steps:
 *         - 
 *
 * @param[out] eutranNeighCell  : Eutran neighbour cell Control block
 * @param[in]  neighCell : Eutran Neighbour Cell parameters           
 * @return void 
 *   
 */

PRIVATE Void wrEmmCopyEutraNghOamInd
(
LwrNghCellInd          *eutranNeighInd,
WrEutranNeighCellCb    *nrtNgCell
)
{
   if(eutranNeighInd->action == WR_ACT_DEL)
   {
      eutranNeighInd->t.neighDelCfg.t.eutranCell.earfcn = nrtNgCell->earfcnDl;
      eutranNeighInd->t.neighDelCfg.t.eutranCell.phyCellId = nrtNgCell->pci;
      eutranNeighInd->t.neighDelCfg.t.eutranCell.cid = 
                                     nrtNgCell->eCgi.eutranCellId;
      wrUtlCopyPlmnId(&nrtNgCell->enbPlmn, (WrPlmnId*)&eutranNeighInd->t.neighDelCfg.t.eutranCell.plmnId);
      eutranNeighInd->t.neighDelCfg.type = WR_NEIGH_CELL_EUTRA;
   }
   else
   {
      eutranNeighInd->t.neighAddCfg.type = WR_NEIGH_CELL_EUTRA;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.cio = nrtNgCell->cio;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.dlBw = nrtNgCell->dlBw;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.earfcnDl = nrtNgCell->earfcnDl;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.earfcnUl = nrtNgCell->earfcnUl;

      wrUtlCopyPlmnId(&nrtNgCell->enbPlmn, (WrPlmnId*)&eutranNeighInd->t.neighAddCfg.t.eutraCell.enbPlmn);
      eutranNeighInd->t.neighAddCfg.t.eutraCell.enbType = nrtNgCell->enbType;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.enbId = nrtNgCell->eCgi.eutranCellId;

      eutranNeighInd->t.neighAddCfg.t.eutraCell.ulBw = nrtNgCell->ulBw;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.tac = nrtNgCell->tac;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.pci = nrtNgCell->pci;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.qoffset = nrtNgCell->qoffset;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.qoffset = nrtNgCell->qoffset; 

      eutranNeighInd->t.neighAddCfg.t.eutraCell.numPlmn = nrtNgCell->numPlmn;
      
      if((nrtNgCell->numPlmn != 0) && 
         (nrtNgCell->numPlmn < WR_MAX_PLMN_IDS))
      {
         cmMemcpy((U8 *)eutranNeighInd->t.neighAddCfg.t.eutraCell.plmnId,(U8 *)nrtNgCell->plmn, 
                                 (sizeof(WrPlmnId)*nrtNgCell->numPlmn));
      }
      
      eutranNeighInd->t.neighAddCfg.t.eutraCell.blackListed = nrtNgCell->blackListed;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.refSigPower= nrtNgCell->detectedRsrp;
      
#ifdef LTE_TDD
      eutranNeighInd->t.neighAddCfg.t.eutraCell.ulDlSubFrameCfg = nrtNgCell->ulDlSubframeCfg;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.splSfCfg = nrtNgCell->splSfCfg;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.cpDl = nrtNgCell->cpDl;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.cpUl = nrtNgCell->cpUl;
#endif
      
      eutranNeighInd->t.neighAddCfg.t.eutraCell.csgId= nrtNgCell->csgId;
      eutranNeighInd->t.neighAddCfg.t.eutraCell.csgAccessMode= nrtNgCell->csgAccessMode;
      
   }
  
}

/** @brief This function is responsible for copying the UTRAN neighbour cell
 *              info to neighbour cell control block
 *
 * @details
 *
 *     Function: wrEmmCopyUtranNeighCell
 *
 *         Processing steps:
 *         -
 *
 * @param[out] utranNeighCell  : Utran neighbour cell Control block
 * @param[in]  neighCell : Utran Neighbour Cell parameters
 * @return S16
 *       -# Success : ROK
 */
 
/** @brief This Function send GERAN neighbour cell details to OAM
 *  
 *  @details
 *
 *  Function:  wrEmmSendAddGeranNghInd
 *
 *  Processing steps:
 *     - copy the GERAN neighbour cell details in lwr structure and send to OAM
 *
 *  @param[in] geranCell :  GERAN cell CB
 *  @param[in] actType   : action type
 *
 *  @return : ROK or RFAILED;
 */

PUBLIC S16 wrEmmSendAddGeranNghInd(WrGeranCellCb *geranNgh,LwrActType actType)
{
   LwrNghCellInd *geranCell;

   WR_ALLOC(&geranCell,sizeof(LwrNghCellInd));
   if(geranCell == NULLP)
   {
      RLOG0(L_ERROR," Memory allocation failed while sending  Geran neighbour addition indication to OAM ");
      RETVALUE(RFAILED);
   }
   geranCell->action = actType;
   
   wrEmmCopyGeranNghOamInd(geranCell, geranNgh);

   RETVALUE(smBldAndSendNghCellInd(geranCell));
}

/** @brief This Function send EUTRA neighbour cell details to OAM
 *  
 *  @details
 *
 *  Function:  wrEmmSendAddEutraNghInd
 *
 *  Processing steps:
 *     - copy the EUTRA neighbour cell details in lwr structure and send to OAM
 *
 *  @param[in] eutraCell :  EUTRA cell CB
 *  @param[in] actType   : action type
 *
 *  @return : ROK or RFAILED;
 */

PUBLIC S16 wrEmmSendAddEutraNghInd(WrEutranNeighCellCb *eutraNgh,LwrActType actType)
{
   LwrNghCellInd *eutraCell;

   WR_ALLOC(&eutraCell,sizeof(LwrNghCellInd));
   if(eutraCell == NULLP)
   {
      RLOG0(L_ERROR," Memory allocation failed while sending Eutra neighbour addition indication to OAM");
      RETVALUE(RFAILED);
   }
   eutraCell->action = actType;
   
   wrEmmCopyEutraNghOamInd(eutraCell,eutraNgh);

   RETVALUE(smBldAndSendNghCellInd(eutraCell));
}

/** @brief This function is responsible for copying the utra neighbour cell 
 *              info to LWR interface structure
 *
 * @details
 *
 *     Function: wrEmmCopyUtraNghOamInd
 *
 *         Processing steps:
 *         - 
 *
 * @param[out] utranNeighInd  : LWR NGH cell  
 * @param[in]  neighCell : Eutran Neighbour Cell parameters           
 * @return void 
 *   
 */

PRIVATE Void wrEmmCopyUtraNghOamInd
(
LwrNghCellInd          *utranNeighInd,
WrUtraNeighCellCb       *nrtNgCell
)
{
   if(utranNeighInd->action == WR_ACT_ADD)
   {
      utranNeighInd->t.neighAddCfg.t.utranCell.rncId = nrtNgCell->rncId;
      utranNeighInd->t.neighAddCfg.t.utranCell.cId   = nrtNgCell->cId;
      utranNeighInd->t.neighAddCfg.t.utranCell.lac   = nrtNgCell->lac;
      utranNeighInd->t.neighAddCfg.t.utranCell.rac   = nrtNgCell->rac;
      utranNeighInd->t.neighAddCfg.t.utranCell.duplexMode = 
                                                nrtNgCell->duplexMode;
    
      utranNeighInd->t.neighAddCfg.t.utranCell.isPsHOCapable=nrtNgCell->isPsHOCapable;
      if(WR_UTRA_MODE_FDD == nrtNgCell->duplexMode)
      {
         utranNeighInd->t.neighAddCfg.type = WR_NEIGH_CELL_UTRA_FDD;
         utranNeighInd->t.neighAddCfg.t.utranCell.t.utraFdd.psc = 
                                                 nrtNgCell->t.utraFdd.psc;
         utranNeighInd->t.neighAddCfg.t.utranCell.t.utraFdd.arfcnDl = 
                                             nrtNgCell->t.utraFdd.arfcnDl;
         utranNeighInd->t.neighAddCfg.t.utranCell.t.utraFdd.arfcnUl = 
                                             nrtNgCell->t.utraFdd.arfcnUl;
      }
      else
      {
         utranNeighInd->t.neighAddCfg.type = WR_NEIGH_CELL_UTRA_TDD;
         utranNeighInd->t.neighAddCfg.t.utranCell.t.utraTdd.cellParamId = 
                                          nrtNgCell->t.utraTdd.psc;
         utranNeighInd->t.neighAddCfg.t.utranCell.t.utraTdd.arfcn = 
                                                nrtNgCell->t.utraTdd.arfcn;
         utranNeighInd->t.neighAddCfg.t.utranCell.t.utraTdd.pccpch = 
                                               nrtNgCell->t.utraTdd.pccpch;
      }
      utranNeighInd->t.neighAddCfg.t.utranCell.numPlmn = nrtNgCell->numPlmn;
     
      if((nrtNgCell->numPlmn != 0) && 
         (nrtNgCell->numPlmn < WR_MAX_PLMN_IDS))
      {
         cmMemcpy((U8 *)&utranNeighInd->t.neighAddCfg.t.utranCell.plmnId,
            (U8 *)&nrtNgCell->plmnId,sizeof(WrPlmnId)*nrtNgCell->numPlmn); 
      }
      utranNeighInd->t.neighAddCfg.t.utranCell.isRimSupported = 
                                                nrtNgCell->isRimSupported;
   }
   else if (utranNeighInd->action == WR_ACT_DEL)
   {
      utranNeighInd->t.neighDelCfg.t.utranCell.cid= nrtNgCell->cId;
      if(WR_UTRA_MODE_FDD == nrtNgCell->duplexMode)
      {
         utranNeighInd->t.neighDelCfg.type = WR_NEIGH_CELL_UTRA_FDD;
      }
      else
      {
         utranNeighInd->t.neighDelCfg.type = WR_NEIGH_CELL_UTRA_TDD;
      }
   }
}


/** @brief This Function send UTRA neighbour cell details to OAM
 *  
 *  @details
 *
 *  Function:  wrEmmSendUtraNghInd
 *
 *  Processing steps:
 *     - copy the UTRA neighbour cell details in lwr structure and send to OAM
 *
 *  @param[in] utraNgh :  UTRA cell CB
 *  @param[in] actType   : action type
 *
 *  @return : ROK or RFAILED;
 */

PUBLIC S16 wrEmmSendUtraNghInd(WrUtraNeighCellCb *utraNgh,LwrActType actType)
{
   LwrNghCellInd *utraCell;

   WR_ALLOC(&utraCell,sizeof(LwrNghCellInd));
   if(utraCell == NULLP)
   {
      RLOG0(L_ERROR," Memory allocation failed while sending Utra neighbour addition Indication to OAM");
      RETVALUE(RFAILED);
   }
   utraCell->action = actType;
   
   wrEmmCopyUtraNghOamInd(utraCell,utraNgh);

   RETVALUE(smBldAndSendNghCellInd(utraCell));
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
