/*********************************************************************16**

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
               

     File:     rm_uecontext.cpp

     Sid:      qf_uecontext.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 16:45:39 2013

     Prg:     hkrishna 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=150;
/* header include files */
#include "rm_uecontext.h"
#include "rm_cellmanager.h"
#include "rm_utils.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CERABInfo::updateERABInfo(RmERabInfo & stERAB, U16  usCRNTI)
{
   RLOG_ARG1(L_DEBUG,DBG_CRNTI,usCRNTI,"Updating ERAB [%d] info",stERAB.bErabId);
   m_stERAB = stERAB;
   m_usCRNTI = usCRNTI;
   m_bIsPreEmpted = FALSE;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CERABInfo::modifyERAB(RmERabInfo & stERAB)
{
   m_stERAB = stERAB;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CUEContext::CUEContext()
{
   // release and initialisation is same.
   releaseUE();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::releaseUE()
{
   m_tlERABList.clear(); // empty
   memset(&m_stUeRadioResCfg, 0 , sizeof(RmuUeRadioResCfg));
   memset(&m_stUeStats, 0 , sizeof(RmRcmUeStats));
   memset(&m_stUECapability, 0 , sizeof(RmuUeCapInfoIe));
   resetUeInactivityTimerExpiryCount();

   memset(&m_stRmDrxCfg, 0 , sizeof(CmRrmDrxCfg));
   m_bIsUeMemberShipIdentified = false;
   m_usRiOffset = 0;
   m_bIsMember = false;
   m_usCRNTI = 0;
   m_numQciThreeRab = 0;
   m_uiMAXGBR = 0;
   m_bTxnMode = 0;
   m_radioResAllocBitMap =0;
   m_bHaveNonPremptableERAB = 0;
   m_uiUlUeAmbr = 0;
   m_uiDlUeAmbr = 0;
   m_bMaxARP = 0xff;
   m_bMaxArpQciPriority = 0xff;
   m_bHighestQciPriority = 0;
   m_bEstablishmentCause = 0;
   m_bIsActiveUE = false;
   m_bDrxDisBrrCnt = 0;
   m_bUeIdx       = 0xff;  
   m_bQciOneBrrAdmit = false;
   m_bQciOneErabId  = 0;
   m_bIsTxnModeChng = false;
   m_bIsEmrgncyBrPrsnt = false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::setUeCapInfo(RmuUeCapInfoIe &stUECapabilityInfo)
{
   m_stUECapability = stUECapabilityInfo;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::updateHighPriorityQosParams()
{
   std::list<CERABInfo*>::iterator   it;
   U8                                bERAB_ARP;
   U8                                bQci;
   U8                                bERABQciPriority;

   setNonPreEmptableRabforUe(FALSE);

   // traverse the list Rab list in Ue context  and update the Emergency algoritham required
   // params in UE context

   m_bDrxDisBrrCnt = 0; /* Number of bearers with QCI-3 */

   for(it = m_tlERABList.begin(); it != m_tlERABList.end() ; ++it)
   {
      CERABInfo  *ptrObjErab = (*it);
      bERAB_ARP = ptrObjErab->getArp();
      bQci = ptrObjErab->getQci();
      bERABQciPriority = getQciPriority(bQci);

      // set m_bHaveNonPremptableERAB in UE context if UE have a single non premptable E-Rab
      if (ptrObjErab->isNonPreEmptableRab())
      {
         setNonPreEmptableRabforUe(TRUE);
      }

      /* count the number of bearers, requires DRX to to be disabled */
      if((CCellManager::getGbrDrxDisFlag()) && (bQci <= RM_QCI_FOUR))
      {
         m_bDrxDisBrrCnt++;
      }

      if( it == m_tlERABList.begin() ) 
      {
         setMaxArpOfUe(bERAB_ARP);
         m_bMaxArpQciPriority = bERABQciPriority;
         m_bHighestQciPriority = bERABQciPriority;
         continue;
      }

      /* If ARPs are equal then consider Qci Priority */
      if (bERAB_ARP == m_bMaxARP && bERABQciPriority < m_bMaxArpQciPriority) 
         m_bMaxArpQciPriority = bERABQciPriority;

      /* lower value of ARP having Highest priority */
      if (bERAB_ARP < m_bMaxARP)
      {
         setMaxArpOfUe(bERAB_ARP);
         m_bMaxArpQciPriority = bERABQciPriority;
      }

      if(m_bHighestQciPriority > bERABQciPriority)
      {
         setHighestQciPriorityOfUe(bERABQciPriority);
      }

   } /* end of for to traverse the Rab list for the UE */
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 CUEContext::getMaxArpQciPriorityOfUe() const
{
   return m_bMaxArpQciPriority;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 CUEContext::getHighestQciPriorityOfUe() const
{
   return m_bHighestQciPriority;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::setHighestQciPriorityOfUe(U8 bQciPriority)
{
   m_bHighestQciPriority = bQciPriority;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 CUEContext::getMaxArpOfUe() const
{
   return m_bMaxARP;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::setMaxArpOfUe(U8 bMaxArp)
{
   m_bMaxARP = bMaxArp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::setActiveUEFlag(bool bIsActiveUe)
{
   m_bIsActiveUE = bIsActiveUe;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::setNonPreEmptableRabforUe(bool bIsNonPreEmptable)
{
   m_bHaveNonPremptableERAB = bIsNonPreEmptable;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CUEContext::isNonPreemptableUE() const
{
   return(m_bHaveNonPremptableERAB);
}

void CUEContext::addERAB(CERABInfo *ptrERab)
{
   m_tlERABList.push_back(ptrERab);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::setRabsCRNTI(U16 usCRNTI)
{
   std::list<CERABInfo*>::iterator it;
   for( it = m_tlERABList.begin(); it != m_tlERABList.end(); ++it)
   {
      (*it)->setRabCrnti(usCRNTI);
   }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CERABInfo* CUEContext::findERAB(U8 bERABId)
{
   std::list<CERABInfo*>::iterator it;

   for( it = m_tlERABList.begin(); it != m_tlERABList.end(); ++it)
   {
      if( (*it)->getERABId() == bERABId )
      {
         return (*it);
      }
   }
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CERABInfo*  CUEContext::removeFirstERabFrmUeRabList()
{
   CERABInfo *ptrERabObj = NULL;

   if(!m_tlERABList.empty())
   {
      ptrERabObj = m_tlERABList.front();
      m_tlERABList.remove(ptrERabObj);
   }
   return ptrERabObj;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::releaseERAB(CERABInfo *ptrObjERab)
{
   m_tlERABList.remove(ptrObjERab);
}

void CUEContext::updateEmrgncyBrPrsnt(CUEContext *pUE, U32 uEmrgncyArp)
{
   pUE->setIsEmrgBrPrst(false);

   std::list<CERABInfo*>::iterator it;

   for(it = m_tlERABList.begin(); it != m_tlERABList.end(); ++it)
   {
      if((*it)->getArp() == uEmrgncyArp)
      {
         pUE->setIsEmrgBrPrst(true);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::setUeCqiRprtInfo()
{
   //m_stPCQIReport = stCqIReport;
   m_stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap |= RM_RCM_PHY_DED_RES_CFG_CQI;

   m_stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_PHY_DED_RES_CFG;

   m_radioResAllocBitMap |= RM_RCM_UE_RADIO_RES_CQI_BITMAP_VAL;

   m_stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.type = TRUE;
}

void CUEContext::setUePeriodicCqi(RmuPucchReportMode mode)
{
   m_stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.enPucchReportMode = mode;
}

void CUEContext::setUeAperiodicCqi(RmuPuschReportMode mode)
{
   m_stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stAPrdCqiRepCfg.enbld = TRUE;

   m_stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stAPrdCqiRepCfg.enPuschReportMode = mode;
}

void CUEContext::setUeCqiRiIndex(U32 uiRiCfgIdx, Bool pres)
{
   m_stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.riConfigIndex.bPres = pres;
   m_stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.riConfigIndex.usRiConfigIndex = uiRiCfgIdx; 
}   
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::setUePCqiFormat(RmuPcqiFormatInd enCqiFormatInd)
{
   m_stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.enCqiFormatInd = enCqiFormatInd;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::setUeCqiPmiIdxResIdx(U16 usCqiPmiIdx, U16 usCqiResIdx)
{
   m_stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.usCqiResourceIndex = usCqiResIdx;
   m_stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.usCqiConfigIndex = usCqiPmiIdx;
}


#define RM_UE_FGI_PCQI_SUBBAND_SUPP_CHK_BIT  0x40000000

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CUEContext::getUePcqiSubbandSupp()
{
   if(m_stUECapability.stEutraCapInfo.uiFeatureGrpIndicator & RM_UE_FGI_PCQI_SUBBAND_SUPP_CHK_BIT)
   {
      return TRUE;
   }

   return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// @brief Configure Scheduling Request Parameter
///////////////////////////////////////////////////////////////////////////////////////////////////
void CUEContext::setSrCfg(U16 usSrIdx, U16 usSrResIdx, CmRrmDsrTransMax enDsrTransMax)
{
   m_stUeRadioResCfg.stPhyCfgDed.stSrCfg.usSrIdx = usSrIdx;
   m_stUeRadioResCfg.stPhyCfgDed.stSrCfg.usSrResIdx = usSrResIdx;
   m_stUeRadioResCfg.stPhyCfgDed.stSrCfg.enDsrTransMax = enDsrTransMax;

   m_stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap |= RM_RCM_PHY_DED_RES_CFG_SR;
   m_stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_PHY_DED_RES_CFG;
   m_radioResAllocBitMap |= RM_RCM_UE_RADIO_RES_SR_BITMAP_VAL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
Bool CUEContext::isUeSupportsDRX()
{
   /* if QCI 3 bearer is not established and UE is DRX supported,
    * DRX configuration is done for that UE. In case if QCI 3 bearer 
    * is added, either DRX is releases if setup earlier or else not
    * configured */  
   if(!m_bDrxDisBrrCnt)
   {
      if(((m_stUECapability.stEutraCapInfo.uiFeatureGrpIndicator & RM_RCM_LONG_DRX_BIT_SET) && 
               (m_stUECapability.stEutraCapInfo.uiFeatureGrpIndicator & RM_RCM_SHORT_DRX_BIT_SET)) ||   
            (m_stUECapability.stEutraCapInfo.uiFeatureGrpIndicator == 0))
      {
         return RM_RCM_DRX_LONG_SHORT_CFG;
      }
      else if(m_stUECapability.stEutraCapInfo.uiFeatureGrpIndicator & RM_RCM_LONG_DRX_BIT_SET)
      {
         return RM_RCM_DRX_LONG_CFG;
      }   
   }

   return RM_RCM_DRX_NOT_CFG;
}

void CUEContext::setDrxCfg(CmRrmDrxCfg &stRmDrxCfg)
{
   m_stRmDrxCfg = stRmDrxCfg;
   m_radioResAllocBitMap |= RM_RCM_UE_RADIO_RES_DRX_BITMAP_VAL;
   m_stUeRadioResCfg.bRadioResDedBitMap |= RM_RCM_MAC_MAIN_CFG;
   m_stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap |= RM_RCM_MAC_MAIN_CFG_DRX;
}


void CUEContext::relDrxCfg()
{
   m_stRmDrxCfg.bIsDrxEnabled = FALSE;
   m_radioResAllocBitMap ^= RM_RCM_UE_RADIO_RES_DRX_BITMAP_VAL;
   m_stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap ^= RM_RCM_MAC_MAIN_CFG_DRX;
   if(!(m_stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap))
   {
      m_stUeRadioResCfg.bRadioResDedBitMap ^= RM_RCM_MAC_MAIN_CFG;
   }
}   

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

Bool CUEContext::isUeSupportsSPS()
{
   /* If QCI 1 bearer is admitted for this UE then check if UE supports SPS
    * feature and UM mode. 
    * FGI bit for SPS is different for Rel-8 and Rel-9 UEs*/
   if(m_bQciOneBrrAdmit)
   {   
      if(((((m_stUECapability.stEutraCapInfo.uiUeAccessStratumRls == RMU_ACCESS_STRATUM_REL8) &&
                     (m_stUECapability.stEutraCapInfo.uiFeatureGrpIndicator & RM_RCM_SPS_REL8_BIT_SET)) || 
                  (((m_stUECapability.stEutraCapInfo.uiUeAccessStratumRls == RMU_ACCESS_STRATUM_REL9) ||
                    (m_stUECapability.stEutraCapInfo.uiUeAccessStratumRls == RMU_ACCESS_STRATUM_REL9HIGHER)) &&
                   (m_stUECapability.stEutraCapInfo.uiFeatureGrpIndicator & RM_RCM_SPS_REL9_BIT_SET))) && 
               (m_stUECapability.stEutraCapInfo.uiFeatureGrpIndicator & RM_RCM_UM_MODE_BIT_SET))  ||
            (m_stUECapability.stEutraCapInfo.uiFeatureGrpIndicator == 0)) 
      {
         RETVALUE(TRUE);
      }
   }

   RETVALUE(FALSE);
}   

void CUEContext::setSpsCfg(RmuUeSpsCfg &stRmSpsCfg)
{
   m_stRmSpsCfg = stRmSpsCfg;
   m_radioResAllocBitMap |= RM_RCM_UE_RADIO_RES_SPS_BITMAP_VAL;
}   

S32 CUEContext::relSpsCfg()
{
   m_stRmSpsCfg.stSpsConfigDl.bIsSpsEnabled = FALSE;
   m_stRmSpsCfg.stSpsConfigUl.bIsSpsEnabled = FALSE;

   m_radioResAllocBitMap ^= RM_RCM_UE_RADIO_RES_SPS_BITMAP_VAL;

   relSpsParam();

   RETVALUE(ROK);
}   

void CUEContext::setSpsParam(U8 bErabId)
{
   if(!m_bQciOneBrrAdmit)
   {
      m_bQciOneBrrAdmit = TRUE;
      m_bQciOneErabId  = bErabId;
   }   
}   

void CUEContext::relSpsParam()
{
   if(m_bQciOneBrrAdmit)
   {
      m_bQciOneBrrAdmit = FALSE;
      m_bQciOneErabId  = 0;
   }  
}   

void CUEContext::setUeTxnMode(U8 bTxnMode)
{ 
   m_bTxnMode = bTxnMode;
   m_bIsTxnModeChng = TRUE;
}


/********************************************************************30**

           End of file:     qf_uecontext.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 16:45:39 2013

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
