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

     Name:     RRM Application

     Type:     CPP source file

     Desc:     This file contains 
               

     File:     rm_resourceconfigmanager.cpp

     Sid:      qf_resourceconfigmanager.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 16:45:38 2013

     Prg:     jbm 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=147;
/* header include files */
#include "rm_cmn.h"
#include "rmu.x"
#include "rm_resourceconfigmanager.h"



CRmRsrcCfgMgr::CRmRsrcCfgMgr(LrmCellConfigData &stCellCfg):m_objRcmCqiMgr(stCellCfg), m_objRcmDrxMgr(stCellCfg),
m_objSRMgr(stCellCfg), m_objRcmSpsMgr(stCellCfg)
{
   m_bN2Rb = stCellCfg.bNRbCqi;
   m_bN1Cs = stCellCfg.bNCsAn;
   m_enDlBw = stCellCfg.enCellDlBandWidth;
   m_bRcmAlgoType = stCellCfg.bRcmAlgoType;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRmRsrcCfgMgr::rmRcmPopulateRadioRes()
{
   U16  usSrN1PucchResStart;
   /* Populate the Resource Index Table for CQI resources */
   m_objRcmCqiMgr.rmRcmCqiPopulateCqiResTbl();

   /* Populate the Resource Index Table for SPS N1-PUCCH resources */
   usSrN1PucchResStart = m_objRcmSpsMgr.rmRcmSpsPopulateN1PucchResTbl();

   /* Populate the Resource Index Table for SR resources */
   m_objSRMgr.rmRcmSrPopulateSrResTbl(usSrN1PucchResStart);

   /* Populate the Onduration for DRX configuraiton */
   m_objRcmDrxMgr.rmRcmDrxPopulateOnduration();
}



//////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
S32 CRmRsrcCfgMgr::rmRcmCfgUeRes(CUEContext* pUE, RmuUeRadioResCfg   &stUeRadioResCfg)
{
   U32 bResCfgMask = RM_RCM_RES_CFG_SR | RM_RCM_RES_CFG_CQI | RM_RCM_RES_CFG_TX_MODE;
   
	stUeRadioResCfg.bRadioResDedBitMap = 0;
	stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap = 0;
	stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap = 0;

	/* allocate SR for UE */
   configureSR(bResCfgMask, stUeRadioResCfg, pUE);

   /* Confiugre TxnMode */ 
	configureTxnMode(bResCfgMask, stUeRadioResCfg, pUE);

	/* Conifugre PCQI */ 
	configurePCQI(bResCfgMask, stUeRadioResCfg, pUE);

   RETVALUE(ROK);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
S32 CRmRsrcCfgMgr::configureUeResources(CUEContext* pUE,  RmuUeRadioResCfg & stUeRadioResCfg)
{
	U32 bResCfgMask = m_objRcmAlgoInfo.rmRcmGetResToCfg
                     ( 	RMU_RECFG_REQ_ERAB_CONFIG, 
							 	pUE ,m_objRcmDrxMgr.isDrxEnabled(),
                        m_objRcmSpsMgr.isSpsEnabled(),
                        m_objRcmSpsMgr.isSpsUeRem()
                     );

	stUeRadioResCfg.bRadioResDedBitMap = 0;
	stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap = 0;
	stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap = 0;

	/* allocate SR for UE */
   configureSR(bResCfgMask, stUeRadioResCfg, pUE);

	/* Conifugre PCQI */ 
	configurePCQI(bResCfgMask, stUeRadioResCfg, pUE);

	/* Confiugre TxnMode */ 
	configureTxnMode(bResCfgMask, stUeRadioResCfg, pUE);

	/* Conifugre DRX */ 
	if( m_objRcmDrxMgr.isDrxEnabled() )
   {
		configureDRX(bResCfgMask, stUeRadioResCfg, pUE);
   }

	/* Check and reconfigure SRS */
	if(bResCfgMask & RM_RCM_UE_RADIO_RES_SRS_BITMAP_VAL)
	{
		/* This block need to be updated in when SRS Configuration is
		 * supported in RRM
		 */
	}

   if( m_objRcmSpsMgr.isSpsEnabled() )
   {
      configureSPS(bResCfgMask, stUeRadioResCfg, pUE); 
   }   

	RETVALUE(ROK);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
S32 CRmRsrcCfgMgr::rmRcmRecfgUeRes(CUEContext* pUE, RmuUeHoReq & stUeHoReq, RmuUeHoRsp *pstUeHoRsp)
{
	configureUeResources(pUE, pstUeHoRsp->stUeRadioResCfg);

   if((pstUeHoRsp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG) || 
         (pstUeHoRsp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG))
   {
      U8 bIdx;

      for(bIdx = 0; bIdx < pstUeHoRsp->bNoErabsAdmitted; bIdx++)
      {
         if(pstUeHoRsp->stErabAdmitLst[bIdx].bErabId == pUE->getSpsErabId())
         {
            RmuUeSpsCfg  stUeSpsCfg = pUE->getSpsCfg();

            pstUeHoRsp->stErabAdmitLst[bIdx].bIsDlSpsEnabled = 
               stUeSpsCfg.stSpsConfigDl.bIsSpsEnabled;
            pstUeHoRsp->stErabAdmitLst[bIdx].bIsUlSpsEnabled = 
               stUeSpsCfg.stSpsConfigUl.bIsSpsEnabled;
         }  
      }   
   }

   RETVALUE(ROK);
} /* end of rmRcmProcUeRecfg */

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
S32 CRmRsrcCfgMgr::rmRcmProcTxnModeInd(CUEContext* pUE, RmuUeRadioResCfg &stUeRadioResCfg)
{
   U32 bResCfgMask = RM_RCM_RES_CFG_CQI | RM_RCM_RES_CFG_TX_MODE;

	stUeRadioResCfg.bRadioResDedBitMap = 0;
	stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap = 0;
	stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap = 0;

	/* Conifugre PCQI */ 
	configurePCQI(bResCfgMask, stUeRadioResCfg, pUE);
   /* Confiugre TxnMode */ 
	configureTxnMode(bResCfgMask, stUeRadioResCfg, pUE);

   RETVALUE(ROK);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRmRsrcCfgMgr::configurePCQI
	(
		U8	bResCfgMask,
		RmuUeRadioResCfg & stUeRadioResCfg,
		CUEContext* pUE
	)
{
   S32 retVal=RFAILED;
   /* Check and reconfigure PCQI */
   if(bResCfgMask & RM_RCM_RES_CFG_CQI)
   {
      U8 bResAllocated = pUE->getUeRadioDedResAllocBitMap();

      if(bResAllocated & RM_RCM_UE_RADIO_RES_CQI_BITMAP_VAL)
      {
         retVal = m_objRcmCqiMgr.rmRcmRecfgCqi(pUE);
      }
      else
      {
         retVal = m_objRcmCqiMgr.rmRcmCfgCqi(pUE);
      }
   }
   else if(bResCfgMask & RM_RCM_RES_CFG_CQI_REL)
   {
      retVal = m_objRcmCqiMgr.rmRcmRelCqi(pUE);
   }

   if(retVal == ROK)
   {
      stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap |= RM_RCM_PHY_DED_RES_CFG_CQI;
      stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_PHY_DED_RES_CFG;
      stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg = pUE->getCqiReport();
   }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRmRsrcCfgMgr::configureTxnMode
	(
		U32	bResCfgMask,
		RmuUeRadioResCfg & stUeRadioResCfg,
		CUEContext* pUE
	)
{
   if(bResCfgMask & RM_RCM_RES_CFG_TX_MODE)
   {
      U32 ueTxnMode;
      ueTxnMode = (U32)pUE->getUeTxnMode();

      stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap |= RM_RCM_PHY_DED_RES_CFG_ANT;
      stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_PHY_DED_RES_CFG;
      stUeRadioResCfg.stPhyCfgDed.stAntInfo.eTxnMode = (RmuTxnMode)pUE->getUeTxnMode(); 
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRmRsrcCfgMgr::configureSR
	(
		U8	bResCfgMask,
		RmuUeRadioResCfg & stUeRadioResCfg,
		CUEContext* pUE
	)
{
	/* Check and reconfigure SR */
	if(bResCfgMask & RM_RCM_RES_CFG_SR)
	{
		m_objSRMgr.configureSR(pUE);
		stUeRadioResCfg.stPhyCfgDed.stSrCfg = pUE->getSrCfg();
		stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap |= RM_RCM_PHY_DED_RES_CFG_SR;
	   stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_PHY_DED_RES_CFG;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRmRsrcCfgMgr::configureDRX
	(
		U8	bResCfgMask,
		RmuUeRadioResCfg & stUeRadioResCfg,
		CUEContext* pUE
	)
{
	/* Check and reconfigure DRX */
	if(bResCfgMask & RM_RCM_RES_CFG_DRX)
	{
		S32 retVal = m_objRcmDrxMgr.rmRcmCfgDrx(pUE, bResCfgMask);

		if(retVal == RM_RCM_DRX_ROK)
		{      
			stUeRadioResCfg.stMacMainCfg.stRmDrxCfg = pUE->getDrxCfg();
         stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_MAC_MAIN_CFG;
         stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap |= RM_RCM_MAC_MAIN_CFG_DRX;
		}
		else if(retVal == RM_RCM_DRX_RFAILED)
		{
         RLOG_ARG0(L_INFO, DBG_CRNTI, (pUE->getCRNTI()),
		 	                 "DRX Configuration Could not be done");
		}   
		else if(retVal == RM_RCM_DRX_PRES)
		{
         RLOG_ARG0(L_INFO, DBG_CRNTI, (pUE->getCRNTI()),
			              "DRX configuration is already done");
		}   
	} 
	else if (bResCfgMask & RM_RCM_RES_CFG_DRX_REL)
	{
		pUE->relDrxCfg();
		stUeRadioResCfg.stMacMainCfg.stRmDrxCfg = pUE->getDrxCfg();
		stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_MAC_MAIN_CFG;
      stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap |= RM_RCM_MAC_MAIN_CFG_DRX;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
S32 CRmRsrcCfgMgr::rmRcmRecfgUeRes(CUEContext* pUE, RmuUeRecfgReq & stUeReCfgReq,RmuUeRecfgRsp *pstUeRecfgRsp)
{
   S32 retVal;

	pstUeRecfgRsp->stUeRadioResCfg.bRadioResDedBitMap = 0;
   
   retVal = configureUeResources(pUE, pstUeRecfgRsp->stUeRadioResCfg);

	/* if any of the resource is reconfigured then set recfg type for radio res cfg */
	if(pstUeRecfgRsp->stUeRadioResCfg.bRadioResDedBitMap)
	{
      if((pstUeRecfgRsp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG) || 
         (pstUeRecfgRsp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG))
      {
         U8 bIdx;

         for(bIdx = 0; bIdx < pstUeRecfgRsp->bNoErabsAccepted; bIdx++)
         {
            if(pstUeRecfgRsp->stErabAcceptLst[bIdx].bErabId == pUE->getSpsErabId())
            {
               RmuUeSpsCfg  stUeSpsCfg = pUE->getSpsCfg();

               pstUeRecfgRsp->stErabAcceptLst[bIdx].bIsDlSpsEnabled = 
                                     stUeSpsCfg.stSpsConfigDl.bIsSpsEnabled;
               pstUeRecfgRsp->stErabAcceptLst[bIdx].bIsUlSpsEnabled = 
                                    stUeSpsCfg.stSpsConfigUl.bIsSpsEnabled;
            }  
         }   
      } 

		pstUeRecfgRsp->bRecfgBitMap |= RMU_RECFG_RSP_UE_RSRC_CONFIG;
	}

	RETVALUE(retVal);
} /* end of rmRcmProcUeRecfg */

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
S32 CRmRsrcCfgMgr::rmRcmRelUeRes(CUEContext *pUE)
{
	S16   retVal = ROK;
	U8    bResAllocated = pUE->getUeRadioDedResAllocBitMap();

	/* Release the allocated CQI resources */
   if(bResAllocated & RM_RCM_UE_RADIO_RES_CQI_BITMAP_VAL)
	{
		retVal = m_objRcmCqiMgr.rmRcmRelCqi(pUE);
	}

   /* Release the allocated DRX resources */
   if(bResAllocated & RM_RCM_UE_RADIO_RES_DRX_BITMAP_VAL)
   {
      retVal = m_objRcmDrxMgr.rmRcmRelDrx(pUE);
   }

   /* Release the allocated SPS resources */
   if(bResAllocated & RM_RCM_UE_RADIO_RES_SPS_BITMAP_VAL)
   {
      retVal = m_objRcmSpsMgr.rmRcmRelSps(pUE);
   }

	RETVALUE(retVal);
}
#ifdef RRM_UNUSED_FUNC
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
S32  CRmRsrcCfgMgr::rmRcmModCellRes()
{
	return ROK;
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U32 CRmRcmAlgoBasic::rmRcmGetResToCfg
(
		U8 	            bRecfgType,
		CUEContext 			*pUE,
      Bool              bIsDrxEnabled,
      Bool              bIsSpsEnabled,
      Bool              bIsSpsUeRem
)
{
	U32                        uiReCfgMask = 0;
	U8  					         bUeResAllocated;

	bUeResAllocated = pUE->getUeRadioDedResAllocBitMap();

   if(bIsDrxEnabled == TRUE)
   {
      U8 isUeEnbDrx = pUE->isUeSupportsDRX();

      if(!(bUeResAllocated & RM_RCM_UE_RADIO_RES_DRX_BITMAP_VAL))
      {  
         if(isUeEnbDrx == RM_RCM_DRX_LONG_SHORT_CFG)
         {   
            uiReCfgMask |= RM_RCM_RES_CFG_DRX;
            uiReCfgMask |= RM_RCM_RES_CFG_DRX_WITH_SHORT;
         }
         else if(isUeEnbDrx == RM_RCM_DRX_LONG_CFG)
         {   
            uiReCfgMask |= RM_RCM_RES_CFG_DRX;
         }            
      }   
      else if(bUeResAllocated & RM_RCM_UE_RADIO_RES_DRX_BITMAP_VAL) 
      {
         if(isUeEnbDrx == RM_RCM_DRX_NOT_CFG)
         {   
            /* Releasing the DRX profile of this UE because QCI 3 bearer
             * has been established */
            uiReCfgMask |= RM_RCM_RES_CFG_DRX_REL;
         }
         else
         {
            /* In case of bearer reconfiguration, need to perform DRX 
             * profile reconfiguration if long cycle value changes based 
             * on the new maximum highest QCI priority */ 
            if(isUeEnbDrx == RM_RCM_DRX_LONG_SHORT_CFG)
            {   
               uiReCfgMask |= RM_RCM_RES_CFG_DRX;
               uiReCfgMask |= RM_RCM_RES_CFG_DRX_WITH_SHORT;
            }
            else if(isUeEnbDrx == RM_RCM_DRX_LONG_CFG)
            {   
               uiReCfgMask |= RM_RCM_RES_CFG_DRX;
            }
         }
      }   
   }

   if(!(bUeResAllocated & RM_RCM_UE_RADIO_RES_CQI_BITMAP_VAL) || !(pUE->isRiCfgDone()))
	{
		uiReCfgMask |= RM_RCM_RES_CFG_CQI;
	}

   if(!(bUeResAllocated & RM_RCM_UE_RADIO_RES_SR_BITMAP_VAL))
	{
		uiReCfgMask |= RM_RCM_RES_CFG_SR;
	}

   /* Checking if SPS is enabled and no. of SPS UE is not zero */
   if(bIsSpsEnabled == TRUE)
   {
      U8 isUeSpsEnb = pUE->isUeSupportsSPS();
         
      /* If SPS is not configured already and UE supports SPS 
       * then set the bit for SPS configuration */
      if(!(bUeResAllocated & RM_RCM_UE_RADIO_RES_SPS_BITMAP_VAL))
      {
         if((isUeSpsEnb == TRUE) && (bIsSpsUeRem == TRUE))
         {
            uiReCfgMask |= RM_RCM_RES_CFG_SPS; 
         }   
         else
         {
            /* If UE does not support SPS then reset the SPS parameter 
             * (flag for SPS enabling + erab ID) to default value */
            pUE->relSpsParam();
         }    
      }   
      else
      {
         /* If SPS is already enabled and bearer for SPS is released or 
          * modified (QCI modified from 1 to some other QCI) then 
          * release the SPS */
         if(!isUeSpsEnb)
         {
           uiReCfgMask |= RM_RCM_RES_REL_SPS;
         }    
      }   
   }   

   if(pUE->isTxnModeChng())
   {
      uiReCfgMask |= RM_RCM_RES_CFG_TX_MODE;
      pUE->setTxnModeChng(FALSE);
   }    
	return uiReCfgMask;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U16 CRmRcmResMgr::getAvailUlSubframeLst(U16 usPrdcty, U16 *usAvalUlSfs)
{
#ifdef LTE_TDD
   U16                  usSfnIdx;
   U16                  usSfnSf;
   U16                  bUlSfIdx;
   U16                  usNumSfn = usPrdcty / RM_NUM_SF_PER_SFN;
   U16                  usNumUlSfs;
#else
   U16                  bTti;
#endif

#ifdef LTE_TDD
   /* if SR periodicity is 5ms then initialize number of SFN as 1 
    */
   if (RM_RCM_SR_PERIODICTY_5MS != usPrdcty)
   {
      usNumUlSfs = m_astRmTddUlSfInfo[m_enUlDlCfg].bNumUlSfs;
   }
   else
   {
      /* For %ms periodicity usNumSfn will be Zero, Hence setting it to 1
       * to run the loop one time
       */
      usNumSfn = 1;
      /* For 5ms SR periodicity the number of UL subframe should be half of the 
       * number of UL subframes available per SFN
       */
      usNumUlSfs = m_astRmTddUlSfInfo[m_enUlDlCfg].bNumUlSfs / 2;
   }
      

   for (usSfnIdx = 0,usSfnSf = 0; usSfnIdx < usNumSfn; usSfnIdx++)
   {
      for (bUlSfIdx = 0; bUlSfIdx < usNumUlSfs;bUlSfIdx++)
      {
         usAvalUlSfs[usSfnSf] = m_astRmTddUlSfInfo[m_enUlDlCfg].bUlSfNum[bUlSfIdx] + (usSfnIdx * RM_NUM_SF_PER_SFN);
         usSfnSf++;
      }
   }
   return usSfnSf;
#else
   for (bTti = 0; (bTti < usPrdcty) && (bTti < RM_RCM_MAX_OFFSET_ARRAY_SIZE); bTti++)
   {
      usAvalUlSfs[bTti] = bTti;
   }

   return bTti;
#endif
}


void CRmRsrcCfgMgr::configureSPS
	(
		U32	bResCfgMask,
		RmuUeRadioResCfg & stUeRadioResCfg,
		CUEContext* pUE
	)
{
	/* If SPS config is set then set the SPS configuration */
	if(bResCfgMask & RM_RCM_RES_CFG_SPS)
	{
      m_objRcmSpsMgr.rmRcmCfgSps(pUE); 
      stUeRadioResCfg.stSpsCfg = pUE->getSpsCfg();
      stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_DL_SPS_CFG;
      stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_UL_SPS_CFG;
   }
   else if(bResCfgMask & RM_RCM_RES_REL_SPS)
   {
      m_objRcmSpsMgr.rmRcmRelSps(pUE);
      stUeRadioResCfg.stSpsCfg = pUE->getSpsCfg();
      stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_DL_SPS_CFG;
      stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_UL_SPS_CFG;
   }   
}   

void CRmRsrcCfgMgr::rmRcmSpsDisInd(CUEContext* pUE)
{
   /* When SPS disable Indication is received from APP
    * release the SPS configuration */
   m_objRcmSpsMgr.rmRcmRelSps(pUE);
}   
/********************************************************************30**

           End of file:     qf_resourceconfigmanager.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 16:45:38 2013

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
