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
               

     File:     wr_emm_ecsfb_cfg.c

     Sid:      fc_emm_ecsfb_cfg.c@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:54:21 2014

     Prg:      BSL

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=31;

#include "wr.h"
#include "wr_cmn.h"
#include "wr_smm_smallcell.h"
#include "wr_emm.h"
#include "wr_lwr.h"
#include "wr_ifm_rrm.h"

/* header include files (.x) */
#include "wr_emm_cfg.h"
#include "wr_emm_neighenb.h"
#include "wr_emm_mme.h"
#include "wr_umm.h"
#include "wr_emm_pbm.h"
#include "wr_utils.h"

EXTERN S16 wrFillSectorId
(
TknStrOSXL                  *ptr,
U8                           len,
Data                         *val
);
EXTERN S16 wrEmmAnrAddNrBand(U8 cellId, U8 numBand,
LwrNeighBandClassCfg  *nbrBandAdd);
EXTERN S16 wrEmmAnrAddCdma1xRttNr
(
   WrCellCb                      *cellCb,
   WrCdma2kNeighCellCb           *newCell
);
PUBLIC S16 wrEmmCfgCellAddSib8
(
 WrCellCb                     *cellCb,
 LwrCellSib8CfgGrp            *sib8CfgGrp
);
PUBLIC S16 wrEmmCfgCellAddCsfbCfg
(
 WrCellCb                 *cellCb,
 LwrCsfbCfgGrp            *csfbCfgGrp
);
PUBLIC S16 wrEmmCfgValidateNeighFreqAddCdma1xRtt
(
 WrCellCb                         *cellCb,
 LwrNeighFreqAddModCfg            *nbrFreqAdd
 );
PUBLIC U16 wrEmmCfgNeighBandClassCfg 
(
LwrNeighBandClass               *neighBandClsCfg
);

/** @brief This function adds the SIB group's information to the cellCb
 *
 * @details adds the SIB group's information to the cellCb
 *
 *     Function: wrEmmCfgCellAddSib8
 *
 *         Processing steps:
 *         - copying the sib8 parameter to cell control block
 *
 * @param [out]  cellCb : Cell control Block
 * @param [in]  sib8CfgGrp : SIB8 Configuration parameters
 * @return S16
 *          -# Success : ROK
 *          -# Failure   : RFAILED
 */
PUBLIC S16 wrEmmCfgCellAddSib8
(
 WrCellCb                     *cellCb,
 LwrCellSib8CfgGrp            *sib8CfgGrp
)
{
   Wr1xRTTParams     *sib81xRttParams;
   LwrCdma1xRttParams *lwr1xRttParams;

   cmMemset((U8*)&cellCb->sib8,0,sizeof(WrSib8Params));

   /* Fill system information time */
   cellCb->sib8.sysTime.cdmaEutraTimeSync = sib8CfgGrp->cdmaEutraTimeSync;

   /* Supporting only asynchronous time, hence hard coding */
   if(sib8CfgGrp->cdmaEutraTimeSync)
   {
      cellCb->sib8.sysTime.syncChoice = WR_CDMA_SYNC_TIME;
   }
   else
   {
      cellCb->sib8.sysTime.syncChoice = WR_CDMA_ASYNC_TIME;
   }


 
   /* Fill 1xRTT Registration parameters */
   sib81xRttParams = &cellCb->sib8.XRTTparam;
   lwr1xRttParams = &sib8CfgGrp->cdma1xrttParams;

   if (lwr1xRttParams->cdma1xrttRegParams.pres == TRUE)
   {
      cellCb->sib8.XRTTParamPres = TRUE;
      sib81xRttParams->csfbRegParamPres = TRUE;

      sib81xRttParams->csfbRegParam.sid = 
         lwr1xRttParams->cdma1xrttRegParams.sid; 

      sib81xRttParams->csfbRegParam.nid = 
         lwr1xRttParams->cdma1xrttRegParams.nid; 

      sib81xRttParams->csfbRegParam.mulSID = 
         lwr1xRttParams->cdma1xrttRegParams.mulSID; 

      sib81xRttParams->csfbRegParam.mulNID = 
         lwr1xRttParams->cdma1xrttRegParams.mulNID; 

      sib81xRttParams->csfbRegParam.homeReg = 
         lwr1xRttParams->cdma1xrttRegParams.homeReg; 

      sib81xRttParams->csfbRegParam.foreignSIDReg = 
         lwr1xRttParams->cdma1xrttRegParams.foreignSIDReg; 

      sib81xRttParams->csfbRegParam.foreignNIDReg = 
         lwr1xRttParams->cdma1xrttRegParams.foreignNIDReg; 

      sib81xRttParams->csfbRegParam.paramReg = 
         lwr1xRttParams->cdma1xrttRegParams.paramReg; 

      sib81xRttParams->csfbRegParam.powerUpReg = 
         lwr1xRttParams->cdma1xrttRegParams.powerUpReg; 

      sib81xRttParams->csfbRegParam.regPeriod = 
         lwr1xRttParams->cdma1xrttRegParams.regPeriod; 

      sib81xRttParams->csfbRegParam.regZone = 
         lwr1xRttParams->cdma1xrttRegParams.regZone; 

      sib81xRttParams->csfbRegParam.totalZone = 
         lwr1xRttParams->cdma1xrttRegParams.totalZone; 

      sib81xRttParams->csfbRegParam.zoneTimer = 
         lwr1xRttParams->cdma1xrttRegParams.zoneTimer; 

      sib81xRttParams->csfbRegParam.powerDownRegPres = 
         lwr1xRttParams->cdma1xrttRegParams.powerDownRegPres; 
   }

   cellCb->sib8.tReselection = sib8CfgGrp->tReselection;
   /* Speed state scale factors */
   if (sib8CfgGrp->speedStatePres == TRUE)
   {
      cellCb->sib8.speedStatePres = TRUE;
      cellCb->sib8.sfMedium = sib8CfgGrp->sfMedium;
      cellCb->sib8.sfHigh = sib8CfgGrp->sfHigh;
   }
   
   if (lwr1xRttParams->cdma1xrttACBarringCfg.pres == TRUE)
   {
      sib81xRttParams->acBarringPres = TRUE;
      sib81xRttParams->acBarring.acBarring0to9_r9 = 
         lwr1xRttParams->cdma1xrttACBarringCfg.acBarring0to9_r9;
      sib81xRttParams->acBarring.acBarring10_r9 = 
         lwr1xRttParams->cdma1xrttACBarringCfg.acBarring10_r9;
      sib81xRttParams->acBarring.acBarring11_r9 = 
         lwr1xRttParams->cdma1xrttACBarringCfg.acBarring11_r9;
      sib81xRttParams->acBarring.acBarring12_r9 = 
         lwr1xRttParams->cdma1xrttACBarringCfg.acBarring12_r9;
      sib81xRttParams->acBarring.acBarring13_r9 = 
         lwr1xRttParams->cdma1xrttACBarringCfg.acBarring13_r9;
      sib81xRttParams->acBarring.acBarring14_r9 = 
         lwr1xRttParams->cdma1xrttACBarringCfg.acBarring14_r9;
      sib81xRttParams->acBarring.acBarring15_r9 = 
         lwr1xRttParams->cdma1xrttACBarringCfg.acBarring15_r9;
      sib81xRttParams->acBarring.acBarringMsg_r9 = 
         lwr1xRttParams->cdma1xrttACBarringCfg.acBarringMsg_r9;
      sib81xRttParams->acBarring.acBarringReg_r9 = 
         lwr1xRttParams->cdma1xrttACBarringCfg.acBarringReg_r9;
      sib81xRttParams->acBarring.acBarringEmg_r9 = 
         lwr1xRttParams->cdma1xrttACBarringCfg.acBarringEmg_r9;
   }
  
   RETVALUE(ROK);
} /* end of wrEmmCfgCellAddSib8 */

/** @brief This function adds the CSFB group's information to the cellCb
 *
 * @details adds the CSFB group's information to the cellCb
 *
 *     Function: wrEmmCfgCellAddCsfbCfg
 *
 *         Processing steps:
 *         - copying the csfb parameter to cell control block
 *
 * @param [out]  cellCb : Cell control Block
 * @param [in]  csfbCfgGrp : CSFB Configuration parameters
 * @return S16
 *          -# Success : ROK
 *          -# Failure   : RFAILED
 */
PUBLIC S16 wrEmmCfgCellAddCsfbCfg
(
 WrCellCb                 *cellCb,
 LwrCsfbCfgGrp            *csfbCfgGrp
)
{
   LwrCdmaTgtCell    *csfbSecId;
   cellCb->csfbCfgGrp.csfbToUtraMeasSup = csfbCfgGrp->csfbToUtraMeasSup;
   cellCb->csfbCfgGrp.csfbToCdmaMeasSup = csfbCfgGrp->csfbToCdmaMeasSup;
   cellCb->csfbCfgGrp.eCsfbMeasSup = csfbCfgGrp->eCsfbMeasSup;
   cellCb->csfbCfgGrp.dualRxSup = csfbCfgGrp->dualRxSup;
/*RRC Rel10 Upgrade - start*/
   if (TRUE == csfbCfgGrp->dualRxTxSup)
   {
   		if(LWR_ECSFB_NOT_SUPPORTED == csfbCfgGrp->eCsfbMeasSup) 
   		{
            RLOG2(L_ERROR, "Invalid Cfg for paramter DUAL_RX_TX_SUPPORTED:\
            eCSFB should be supported(currently=%d) inorder to support \
			DualRxTx Cfg(%d)",csfbCfgGrp->eCsfbMeasSup,csfbCfgGrp->dualRxTxSup);
             RETVALUE(LCM_REASON_INVALID_PAR_VAL);
		}
   }
   cellCb->csfbCfgGrp.dualRxTxSup = csfbCfgGrp->dualRxTxSup;
   cellCb->csfbCfgGrp.cdma1xRttSup= csfbCfgGrp->cdma1xRttSup;
/*RRC Rel10 Upgrade - end*/

   if(FALSE == csfbCfgGrp->cdma1xrttCsfbCfg.secId.isOctStrPres)
   {
      csfbSecId = &csfbCfgGrp->cdma1xrttCsfbCfg.secId.s.cdmaSectorId;

      wrUmmEncCsfbSectorId(csfbSecId->marketId, csfbSecId->switchNum, 
            csfbSecId->cellId, csfbSecId->sectorNum, &cellCb->csfbCfgGrp.cdma1xrttCsfbCfg.sectorIdPtr, 5);
   }
   else
   {
      wrFillSectorId(&cellCb->csfbCfgGrp.cdma1xrttCsfbCfg.sectorIdPtr, 5,
            (Data *)csfbCfgGrp->cdma1xrttCsfbCfg.secId.s.sectorIdPtr.val);
      TknStrOSXL *ptr = &(csfbCfgGrp->cdma1xrttCsfbCfg.secId.s.sectorIdPtr);
      /* to free the smm allocated memory for sector Id */
      if((TRUE == ptr->pres) && (NULLP != ptr->val))
      {
         U8 len;
         len = ptr->len;
         WR_FREE(ptr->val, len * sizeof(U8));
      }
   }
   cmMemcpy((U8*)&cellCb->csfbCfgGrp.cdma1xrttCsfbCfg.mobParams,
         (U8*)&csfbCfgGrp->cdma1xrttCsfbCfg.mobParams,
         sizeof(WrCdma2k1xMobParams));

   if(ROK != wrUmmCsfbCdma2k1xMobParams(cellCb->cellId,
            &csfbCfgGrp->cdma1xrttCsfbCfg.mobParams))
   {
      RLOG0(L_ERROR, " wrUmmCsfbCdma2k1xMobParams Failed ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmCfgCellAddCsfbCfg */

/** @brief This function is responsible for copying the CDMA 1x
 *         neighbour freq information into cellCb neighbour freq
 *
 * @details
 *
 *     Function: wrEmmCopyCdma1xNeighFreq
 * @param [out]  neighFreqCb : Newly added Freq
 * @param [in]   cdma1xFreq  : Configuration received from SM
 *
 *
 *  @return VOID
 */
PRIVATE Void wrEmmCopyCdma1xNeighFreq
(
 WrCdma1xFreqCb                *neighFreqCb, 
 LwrNeighCdma1xFreq            *cdma1xFreq
 )
{
   neighFreqCb->reCfgType            = cdma1xFreq->reCfgType; 
   neighFreqCb->freqIndex            = cdma1xFreq->freqIndex; 
   neighFreqCb->status               = cdma1xFreq->status;    
   neighFreqCb->bandClass            = cdma1xFreq->bandClass; 
   neighFreqCb->arfcn                = cdma1xFreq->arfcn;     
   neighFreqCb->searchRate           = cdma1xFreq->searchRate;
   neighFreqCb->offsetFreq           = cdma1xFreq->offsetFreq;
   neighFreqCb->freqPriority         = cdma1xFreq->freqPriority;
}

/** @brief This function is responsible for adding the CDMA 1x
 *         neighbour freq information into cellCb neighbour freq list
 *
 * @details
 *
 *     Function: wrEmmCfgNeighFreqAddCdma1xFreq()
 *
 * @param [in] cdma1xFreq : Configuration received from SM
 * @param [in]   cellCb  : cellCb into which information have to be added
 *
 *     -# Success : ROK
 *     -# Failure  : LCM_REASON_MEM_NOAVAIL
 */
PUBLIC S16 wrEmmCfgNeighFreqAddCdma1xFreq
(
 WrCellCb                         *cellCb,
 LwrNeighCdma1xFreq               *cdma1xFreq
 )                                                           
{  
   WrCdma1xFreqCb        *neighCdma1xFreqCb = NULLP;

   /*Allocating Memory for the neighbour Control Block*/
   WR_ALLOC(&neighCdma1xFreqCb, sizeof(WrCdma1xFreqCb));
   if (neighCdma1xFreqCb == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   /* Copy the Freq Details */
   wrEmmCopyCdma1xNeighFreq(neighCdma1xFreqCb, 
         cdma1xFreq);
   cellCb->sib8.searchWindSize.pres = TRUE;
   cellCb->sib8.searchWindSize.val = 
      cdma1xFreq->searchRate;
   neighCdma1xFreqCb->freqLstEnt.node = (PTR)neighCdma1xFreqCb;

   /* Add to the Freq List in the Cell CB */
   cmLListAdd2Tail(&cellCb->cdma1xFreqLst, 
         &neighCdma1xFreqCb->freqLstEnt);

   RETVALUE(ROK);
}


/** @brief This function is responsible for addition of a neighbour band at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgNeighBandAdd
 *
 *         Processing steps:
 *         - This is add band List in Cell CB 
 *         - It allocates a new WrCdmaBandClassCb 
 *         - It copies all the config into the allocated WrCdmaBandClassCb
 *
 * @param[in] cellId     : cell for which NR needs to be added
 * @param[in] numBandCls : Number of band classes
 * @param[in] nbrBandAdd : Neighbour parameters 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PRIVATE S16 wrEmmCfgNeighBandAdd
(
 U8                               cellId,
 U8                               numBandCls,
 LwrNeighBandClassCfg            *nbrBandAdd
 )
{
   S16                           ret = LCM_REASON_NOT_APPL;
   WrCellCb                      *cellCb  = NULLP;
   U8                            bandIndex = 0;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "cellCb is NULL.");
      RETVALUE(RFAILED);
   }

   while(bandIndex < numBandCls)
   {
      switch(nbrBandAdd[bandIndex].bandType)
      {
         case LWR_NEIGH_BAND_TYPE_CDMA_1XRTT:
         {
            WrCdmaBandClassCb          *neighBandCb = NULLP;

            if(nbrBandAdd[bandIndex].t.bandClass1xRtt.bandClass > 
                          WR_CDMA_2K_BAND_CLASS_21)
            {
               RLOG0(L_WARNING, "Invalid CDMA Band Class ");
               RETVALUE(LCM_REASON_INVALID_PAR_VAL);
            }
            /*Allocating Memory for the neighbour Control Block*/
            WR_ALLOC(&neighBandCb, sizeof(WrCdmaBandClassCb));
            if (neighBandCb == NULLP)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(LCM_REASON_MEM_NOAVAIL);
            }

            /* Copy the Band Details */
            neighBandCb->reCfgType = 
               nbrBandAdd[bandIndex].t.bandClass1xRtt.reCfgType;
            neighBandCb->bandIndex = 
               nbrBandAdd[bandIndex].t.bandClass1xRtt.bandClassIndex;
            neighBandCb->status = 
               nbrBandAdd[bandIndex].t.bandClass1xRtt.status;
            neighBandCb->bandClass = 
               nbrBandAdd[bandIndex].t.bandClass1xRtt.bandClass;
            neighBandCb->cellReselPri = 
               nbrBandAdd[bandIndex].t.bandClass1xRtt.cellReselectPrior;
            neighBandCb->threshxHigh = 
               nbrBandAdd[bandIndex].t.bandClass1xRtt.threshXHigh;
            neighBandCb->threshxLow = 
               nbrBandAdd[bandIndex].t.bandClass1xRtt.threshXLow;

            neighBandCb->bandLstEnt.node = (PTR)neighBandCb;
            /* Add to the Band List in the Cell CB */
            cmLListAdd2Tail(&cellCb->cdmaBandLst, &neighBandCb->bandLstEnt);
         }
         break;
         default:
            ret = LCM_REASON_INVALID_PAR_VAL;
      }
      bandIndex++;
   }
   RETVALUE(ret);
}


/** @brief This function is responsible for copying the CDMA 1x
 *         neighbour cell information into cellCb neighbour cell
 *
 * @details
 *
 *     Function: wrEmmCopyCdma1xRttNeighCell
 *
 *
 *       -# Success : ROK
 */
PRIVATE S16 wrEmmCopyCdma1xRttNeighCell
(
WrCdma2kNeighCellCb          *cdmaNeighCell,
LwrNeighCellAddCfg           *neighCell
)
{
   cdmaNeighCell->nrIndex = neighCell->t.cdma1xCell.nrIndex;
   cdmaNeighCell->bandClass = neighCell->t.cdma1xCell.bandClass;
   cdmaNeighCell->arfcn = neighCell->t.cdma1xCell.arfcn;
   cdmaNeighCell->pnOffset = neighCell->t.cdma1xCell.pnOffset;
   cdmaNeighCell->cdmaType = LWR_1XRTT;
   cdmaNeighCell->blackListed = neighCell->t.cdma1xCell.blackListed;
   cdmaNeighCell->cid = neighCell->t.cdma1xCell.cellId;
   
   if(TRUE == neighCell->t.cdma1xCell.secId.isOctStrPres)
   {
      //copy the string here
      wrFillSectorId(&cdmaNeighCell->sectorId, 5,
            (Data *)neighCell->t.cdma1xCell.secId.s.sectorIdPtr.val);
      /*   wrFillTknStrOSXL(&(cdmaNeighCell->sectorId), 
           neighCell->t.cdma1xCell.secId.s.sectorIdPtr->len, 
           neighCell->t.cdma1xCell.secId.s.sectorIdPtr->val,
           &(neighCell->t.cdma1xCell.secId.s.sectorIdPtr));
           */
      /* to free the smm allocated memory for sector Id */
      TknStrOSXL *ptr = &(neighCell->t.cdma1xCell.secId.s.sectorIdPtr);
      if((TRUE == ptr->pres) && (NULLP != ptr->val))
      {
         U8 len;
         len = ptr->len;
         WR_FREE(ptr->val, len * sizeof(U8));
      }
   }
   else
   {
      LwrCdmaTgtCell    *cdmaSectorId = &(neighCell->t.cdma1xCell.secId.s.cdmaSectorId);

      wrUmmEncCsfbSectorId( cdmaSectorId->marketId, cdmaSectorId->switchNum, 
            cdmaSectorId->cellId, cdmaSectorId->sectorNum, &cdmaNeighCell->sectorId, 5);
   }
   
   cdmaNeighCell->systemType = neighCell->t.cdma1xCell.systemType;
   cdmaNeighCell->channelNum = neighCell->t.cdma1xCell.channelNum;
   cdmaNeighCell->pilotOneWay = neighCell->t.cdma1xCell.pilotOneWay;
   RETVALUE(ROK);
}

/** @brief This function is responsible for adding the CDMA 1x
 *         neighbour cell information into cellCb
 *
 * @details
 *
 *     Function: wrEmmCfgNeighCellAddCdma1xRtt
 *
 *
 *       -# Success : ROK
 */
PUBLIC S16 wrEmmCfgNeighCellAddCdma1xRtt
(
LwrNeighCellAddCfg           *neighCellAdd,
WrCellCb                     *cellCb
)
{
   WrCdma2kNeighCellCb    *cdma1xRttCellCb = NULLP;
   S16                    ret = LCM_REASON_NOT_APPL;

   WR_ALLOC(&cdma1xRttCellCb, sizeof(WrCdma2kNeighCellCb));
   if (cdma1xRttCellCb == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(LCM_REASON_MEM_NOAVAIL);
   }

   /*Copying the values from LWR to EMMCB - for CDMA 1xRTT */
   wrEmmCopyCdma1xRttNeighCell(cdma1xRttCellCb,neighCellAdd);

   ret = wrEmmAnrAddCdma1xRttNr(cellCb,cdma1xRttCellCb);

   RETVALUE(ret);
}


/** @brief This function validates if the newly received CDMA1xRTT
 *         frequency is already configured
 *
 * @details
 *
 *     Function: wrEmmCfgValidateNeighFreqAddCdma1xRtt
 *
 *
 *       -# Success : ROK
 */
PUBLIC S16 wrEmmCfgValidateNeighFreqAddCdma1xRtt
(
 WrCellCb                         *cellCb,
 LwrNeighFreqAddModCfg            *nbrFreqAdd
 )
{
   LwrNeighCdma1xFreq         *lwr1xCdmaFreq = NULLP;
   WrCdma1xFreqCb             *neighFreqCb = NULLP;
   CmLList                    *neighFreqLnk = NULLP;

   lwr1xCdmaFreq = &nbrFreqAdd->t.cdma1xFreq;
   neighFreqLnk = cellCb->cdma1xFreqLst.first;
   while(neighFreqLnk != NULLP)
   {
      neighFreqCb = (WrCdma1xFreqCb*)neighFreqLnk->node;
      if((neighFreqCb->arfcn == lwr1xCdmaFreq->arfcn) &&
       (neighFreqCb->bandClass == lwr1xCdmaFreq->bandClass))
      {
    RETVALUE(RFAILED);
      }
      neighFreqLnk = neighFreqLnk->next;
   }

   RETVALUE(ROK);
}


/** @brief This function is responsible for validating addition of neighbour 
 *   band class at the EnodeB.
 *
 * @details
 *
 *     Function: wrEmmCfgValidateNeighBandAdd
 *
 *         Processing steps:
 *         - This is validate neighbor bandclass to band List in Cell CB 
 *
 * @param[in] cellId     : cell for which NR needs to be added
 * @param[in] numBandCls    : number of Bandclasses
 * @param[in] nbrBandAdd : Neighbour parameters 
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */

PRIVATE S16 wrEmmCfgValidateNeighBandAdd
(
 U8                               cellId,
 U8                               numBandCls,
LwrNeighBandClassCfg              *nbrBandAdd
 )
{
   WrCellCb                      *cellCb  = NULLP;
   U8                            bandIndex = 0;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "cellCb is NULL.");
      RETVALUE(RFAILED);
   }

   while(bandIndex < numBandCls)
   {
      switch(nbrBandAdd[bandIndex].bandType)
      {
         case LWR_NEIGH_BAND_TYPE_CDMA_1XRTT:
            {
               LwrNeighCdmaBandClassCfg   *lwrNbCdmaBnd = NULLP;
               WrCdmaBandClassCb          *neighBandCb = NULLP;
               CmLList                    *neighBandLnk = NULLP;

               lwrNbCdmaBnd = &nbrBandAdd[bandIndex].t.bandClass1xRtt;
               neighBandLnk = cellCb->cdmaBandLst.first;
               while(neighBandLnk != NULLP)
               {
                  neighBandCb = (WrCdmaBandClassCb*)neighBandLnk->node;
                  if(neighBandCb->bandClass == lwrNbCdmaBnd->bandClass)
                  {
                     RETVALUE(RFAILED);
                  }
                  neighBandLnk = neighBandLnk->next;
               }
            }
            break;
         default:
            RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
      bandIndex++;
   }
   RETVALUE(ROK);
}


/** @brief This function handles neighbor band class configuration from the SM
 *
 * @details
 *
 *     Function: wrEmmCfgNeighBandClassCfg
 *
 *         Processing steps:
 *          - if Addition of Neighbor Band class Config
 *          - Call add neigh band Config functionality
 *
 * @param[in] neighBandClsCfg : Band class Configuration parameters 
 * @return U16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure   : LCM_REASON_INVALID_PAR_VAL,LCM_REASON_MEM_NOAVAIL
 */
PUBLIC U16 wrEmmCfgNeighBandClassCfg 
(
LwrNeighBandClass               *neighBandClsCfg
)
{
   U16    reason = LCM_REASON_INVALID_PAR_VAL;

   if(neighBandClsCfg->numBandClass <= MAX_NUM_BAND_CFG) /* KW Fix */
   {
   switch (neighBandClsCfg->action)
   {
      case WR_ACT_ADD:
      {
         if(ROK == wrEmmCfgValidateNeighBandAdd(neighBandClsCfg->cellId,
               neighBandClsCfg->numBandClass, neighBandClsCfg->t.bcAddCfg))
         {
            reason = wrEmmCfgNeighBandAdd (neighBandClsCfg->cellId, 
                  neighBandClsCfg->numBandClass, neighBandClsCfg->t.bcAddCfg);
            if(reason == LCM_REASON_NOT_APPL)
            {               
               if(wrEmmAnrAddNrBand(neighBandClsCfg->cellId,
              neighBandClsCfg->numBandClass,neighBandClsCfg->t.bcAddCfg)!=  ROK)
               {
                  RLOG0(L_ERROR, " Adding neighbour band to ANR failed ");
                  RETVALUE(LCM_REASON_INVALID_PAR_VAL);
               }
            }
         }
      }
      break;
      default:
         reason = LCM_REASON_INVALID_PAR_VAL;
   }
   }
   else
   {
      RLOG0(L_WARNING, "neighBandClsCfg->numBandClass is greter than MAX_NUM_BAND_CFG");
      reason = LCM_REASON_INVALID_PAR_VAL;
   }

   RETVALUE(reason);
} /* end of wrEmmCfgNeighBandClassCfg */ 

/********************************************************************30**

           End of file:     fc_emm_ecsfb_cfg.c@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:54:21 2014

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
*********************************************************************91*/
