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
               

     File:     rm_acb.cpp

     Sid:      qf_acb.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:32 2013

     Prg:      hkrishna, vkorde

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=151;


/* header include files */

#include "rm_outinterface.h"
#include "rm_application.h"
#include "rm_acb.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
RmPrbAcbTbl g_stAcbPRBTbl[RRM_RAC_ACB_MAX_ENTRIES]  =
{
   {0,    20, RM_ACB_FACTOR_P95,  RM_ACB_TIME_S4  },
   {21,   25, RM_ACB_FACTOR_P90,  RM_ACB_TIME_S8  },
   {26,   30, RM_ACB_FACTOR_P85,  RM_ACB_TIME_S8  },
   {31,   35, RM_ACB_FACTOR_P80,  RM_ACB_TIME_S16 },
   {36,   40, RM_ACB_FACTOR_P75,  RM_ACB_TIME_S16 },
   {41,   45, RM_ACB_FACTOR_P70,  RM_ACB_TIME_S32 },
   {46,   50, RM_ACB_FACTOR_P60,  RM_ACB_TIME_S32 },
   {51,   55, RM_ACB_FACTOR_P50,  RM_ACB_TIME_S64 },
   {56,   60, RM_ACB_FACTOR_P40,  RM_ACB_TIME_S64 },
   {61,   65, RM_ACB_FACTOR_P30,  RM_ACB_TIME_S128 },
   {66,   70, RM_ACB_FACTOR_P25,  RM_ACB_TIME_S128 },
   {71,   75, RM_ACB_FACTOR_P20,  RM_ACB_TIME_S256 },
   {76,   80, RM_ACB_FACTOR_P15,  RM_ACB_TIME_S256 },
   {81,   85, RM_ACB_FACTOR_P10,  RM_ACB_TIME_S512 },
   {86,   90, RM_ACB_FACTOR_P05,  RM_ACB_TIME_S512 },
   {91,   100,RM_ACB_FACTOR_P00,  RM_ACB_TIME_S512 }
};
#define RM_ACB_BAR_ALL_SPECIAL_ACC_CLASS   (0x1f << 3)
#define RM_ACB_NO_BAR_ALL_SPECIAL_ACC_CLASS   0

void CAccessClassBarring::printAcbParams(CmRrmAcb & stAcbParams)
{
   RLOG3(L_INFO, "ACB configuration: Special"
         "Access Class=%d Factor=%d Time=%d", stAcbParams.bAcbForSpac, 
         stAcbParams.enAcbFactor, stAcbParams.enAcbTime);
}

CAccessClassBarring::CAccessClassBarring(LrmCellConfigData  &stCellCfg)
{
   m_stCpuACBData.bAcbForSpac = stCellCfg.stAcbInfo.stAcBarringMoData.bAcbForSpac;
   m_stCpuACBData.enAcbFactor = stCellCfg.stAcbInfo.stAcBarringMoData.enAcbFactor; 
   m_stCpuACBData.enAcbTime = stCellCfg.stAcbInfo.stAcBarringMoData.enAcbTime;

   m_stMmeMOSigAcbParams.bAcbForSpac = stCellCfg.stAcbInfo.stAcBarringMoSig.bAcbForSpac;
   m_stMmeMOSigAcbParams.enAcbFactor = stCellCfg.stAcbInfo.stAcBarringMoSig.enAcbFactor;
   m_stMmeMOSigAcbParams.enAcbTime = stCellCfg.stAcbInfo.stAcBarringMoSig.enAcbTime;

   m_stMmeMODataAcbParams.bAcbForSpac = stCellCfg.stAcbInfo.stAcBarringMoData.bAcbForSpac;
   m_stMmeMODataAcbParams.enAcbFactor = stCellCfg.stAcbInfo.stAcBarringMoData.enAcbFactor;
   m_stMmeMODataAcbParams.enAcbTime = stCellCfg.stAcbInfo.stAcBarringMoData.enAcbTime;


   bIsAcbMoDataEnabledByOam = stCellCfg.stAcbInfo.bIsacBarringMoData;
   bIsAcbMoSigEnabledByOam = stCellCfg.stAcbInfo.bIsacBarringMoSig;
   bIsAcbMoSigEnabledByMme = false;
   bIsAcbMoDataEnabledByMme = false; 

   m_stMmeHighPriMOTermAcbMoDataParams.bAcbForSpac = 
      RM_ACB_NO_BAR_ALL_SPECIAL_ACC_CLASS;
   m_stMmeHighPriMOTermAcbMoDataParams.enAcbFactor = 
      stCellCfg.stAcbInfo.stAcBarringMoData.enAcbFactor;
   m_stMmeHighPriMOTermAcbMoDataParams.enAcbTime = 
      stCellCfg.stAcbInfo.stAcBarringMoData.enAcbTime;

   m_stMmeHighPriMOTermAcbMoSigParams.bAcbForSpac = 
      RM_ACB_NO_BAR_ALL_SPECIAL_ACC_CLASS;
   m_stMmeHighPriMOTermAcbMoSigParams.enAcbFactor = 
      stCellCfg.stAcbInfo.stAcBarringMoSig.enAcbFactor;
   m_stMmeHighPriMOTermAcbMoSigParams.enAcbTime = 
      stCellCfg.stAcbInfo.stAcBarringMoSig.enAcbTime;

   m_bAcBarringForEmergency = stCellCfg.stAcbInfo.bAcBarringEmergency;

   /* SP4 :The below logic is taken care in SM  */
   m_stPRBAcbParams.bAcbForSpac = stCellCfg.stAcbInfo.stAcBarringMoSig.bAcbForSpac;
   /* RRC-REL10-Upgrade */
   if(RM_ACB_NO_BAR_ALL_SPECIAL_ACC_CLASS != stCellCfg.stAcbInfo.stAcBarringMoSig.bAcbForSpac)
   {
      m_stPRBAcbParams.enAcbFactor = RM_ACB_FACTOR_P00;
   }
   else
   {
      m_stPRBAcbParams.enAcbFactor = RM_ACB_FACTOR_P95;
   }
   m_stPRBAcbParams.enAcbTime = RM_ACB_TIME_S4;

   m_bCellId = stCellCfg.bCellId;
   m_bCpuLoad = 0;
   m_enMMEOverldStatus = RMU_MME_OVLD_STATUS_NORMAL;
   m_bPrevPrbUsageIndex = 0;

   RLOG1(L_INFO, "Acb params for CellId %d", m_bCellId);
   RLOG3(L_INFO, "BarringForEmergency %d MO Data"
         "Enabled %d MO Signal Enabled %d", m_bAcBarringForEmergency,
         bIsAcbMoDataEnabledByOam, bIsAcbMoSigEnabledByOam);
   printAcbParams(m_stMmeMOSigAcbParams);
   printAcbParams(m_stMmeMODataAcbParams);

   memset(&m_stAcbCfgInfo, 0, sizeof(RmuAcbParamInd));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAccessClassBarring::handleCellReConfiguration(LrmCellConfiguration & stCellCfgInfo)
{
   LrmCellConfigData &stCellCfgData = stCellCfgInfo.stCellCfgInfo;
   bool bTriggerRecfgIndForAcb = false;

   if (stCellCfgInfo.usReCfgType & LRM_CELL_RECFGTYPE_ACB_MODATA)
   {
      CmRrmAcb    &stMoDataAcb = stCellCfgData.stAcbInfo.stAcBarringMoData;

      /* OAM is enabling the barring for MO-data; if already Mo-data barring is
       * on-going (triggered by either by OAM or MME), no need to trigger again 
       */
      if((false == bIsAcbMoDataEnabledByOam) &&  (false == bIsAcbMoDataEnabledByMme) &&
         (true == stCellCfgData.stAcbInfo.bIsacBarringMoData))
      {
         RLOG3(L_INFO, "CELL RECFG MO-DATA BAR ENABLE: Previously is enabled by OAM:%d"
               "is enabled by MME:%d, recvd reconfig from OAM:%d",
               bIsAcbMoDataEnabledByOam, bIsAcbMoDataEnabledByMme, 
               stCellCfgData.stAcbInfo.bIsacBarringMoData);
         bTriggerRecfgIndForAcb = true;
      }

      /* OAM is disabling the barring for MO-data either which is enabled by OAM or MME */
      if(((true == bIsAcbMoDataEnabledByOam) || (true == bIsAcbMoDataEnabledByMme)) &&
         ( false == stCellCfgData.stAcbInfo.bIsacBarringMoData))
      {
         RLOG3(L_INFO, "CELL RECFG MO-DATA BAR DISABLE : Previously is enabled by OAM:%d"
               "is enabled by MME:%d, recvd reconfig from OAM:%d",
               bIsAcbMoDataEnabledByOam, bIsAcbMoDataEnabledByMme, 
               stCellCfgData.stAcbInfo.bIsacBarringMoData);
         bTriggerRecfgIndForAcb = true;
      }


      if( (true == stCellCfgData.stAcbInfo.bIsacBarringMoData) || 
          (true == bIsAcbMoDataEnabledByOam) || (true == bIsAcbMoDataEnabledByMme))
      {
         if((m_stMmeMODataAcbParams.enAcbFactor != stMoDataAcb.enAcbFactor) ||
               (m_stMmeMODataAcbParams.enAcbTime != stMoDataAcb.enAcbTime)||
               (m_stMmeMODataAcbParams.bAcbForSpac != stMoDataAcb.bAcbForSpac))
         {
            RLOG3(L_INFO, "CELL RECFG MO-DATA BAR SP : Previously is enabled by OAM:%d"
                  "is enabled by MME:%d, recvd reconfig from OAM:%d",
                  bIsAcbMoDataEnabledByOam, bIsAcbMoDataEnabledByMme, 
                  stCellCfgData.stAcbInfo.bIsacBarringMoData);
            bTriggerRecfgIndForAcb = true;
         }
      }

      m_stMmeMODataAcbParams.enAcbFactor = stMoDataAcb.enAcbFactor;
      m_stMmeMODataAcbParams.enAcbTime = stMoDataAcb.enAcbTime;
      m_stMmeMODataAcbParams.bAcbForSpac = stMoDataAcb.bAcbForSpac;
      bIsAcbMoDataEnabledByOam = stCellCfgData.stAcbInfo.bIsacBarringMoData;

      m_stCpuACBData.bAcbForSpac = stMoDataAcb.enAcbFactor;
      m_stCpuACBData.enAcbFactor = stMoDataAcb.enAcbFactor; 
      m_stCpuACBData.enAcbTime = stMoDataAcb.enAcbTime;

      m_stMmeHighPriMOTermAcbMoDataParams.enAcbFactor = 
         stMoDataAcb.enAcbFactor;
      m_stMmeHighPriMOTermAcbMoDataParams.enAcbTime = 
         stMoDataAcb.enAcbTime;


      /* If already overload is going on for ACB triggered by MME, 
       * if OAM says to stop on going ACB, we will stop the 
       * ongoing ACB triggered by RRM based on the MME over load status.
       */
      if((true == bIsAcbMoDataEnabledByMme) && 
            (false == bIsAcbMoDataEnabledByOam))
      {
         bIsAcbMoDataEnabledByMme = false;
      }

      if(bTriggerRecfgIndForAcb)
      {
         printAcbParams(m_stMmeMODataAcbParams);
      }
   }

   if (stCellCfgInfo.usReCfgType & LRM_CELL_RECFGTYPE_ACB_MOSIG)
   {
      CmRrmAcb    &stAcBarringMoSig = stCellCfgData.stAcbInfo.stAcBarringMoSig;

      /* OAM is enabling the barring for MO-Sig; if already Mo-sig barring is
       * on-going (triggered by either by OAM or MME), no need to trigger again 
       */
      if((false == bIsAcbMoSigEnabledByOam) &&  (false == bIsAcbMoSigEnabledByMme) &&
            (true == stCellCfgData.stAcbInfo.bIsacBarringMoSig))
      {
         RLOG3(L_INFO, "CELL RECFG MO-SIG BAR ENABLE: Previously is enabled by OAM:%d"
               "is enabled by MME:%d, recvd reconfig from OAM:%d",
               bIsAcbMoSigEnabledByOam, bIsAcbMoSigEnabledByMme, 
               stCellCfgData.stAcbInfo.bIsacBarringMoSig);
         bTriggerRecfgIndForAcb = true;
      }

      /* OAM is disabling the barring for MO-sig either which is enabled by OAM or MME */
      if(((true == bIsAcbMoSigEnabledByOam) || (true == bIsAcbMoSigEnabledByMme)) &&
            ( false == stCellCfgData.stAcbInfo.bIsacBarringMoSig))
      {
         RLOG3(L_INFO, "CELL RECFG MO-SIG BAR DISABLE: Previously is enabled by OAM:%d"
               "is enabled by MME:%d, recvd reconfig from OAM:%d",
               bIsAcbMoSigEnabledByOam, bIsAcbMoSigEnabledByMme, 
               stCellCfgData.stAcbInfo.bIsacBarringMoSig);
         bTriggerRecfgIndForAcb = true;
      }

      if( (true == bIsAcbMoSigEnabledByOam) || (true == bIsAcbMoSigEnabledByMme) ||
          ( true == stCellCfgData.stAcbInfo.bIsacBarringMoSig))
      {
         if ((m_stMmeMOSigAcbParams.enAcbFactor != stAcBarringMoSig.enAcbFactor)||
               (m_stMmeMOSigAcbParams.enAcbTime != stAcBarringMoSig.enAcbTime) ||
               (m_stMmeMOSigAcbParams.bAcbForSpac != stAcBarringMoSig.bAcbForSpac))
         {
            RLOG3(L_INFO, "CELL RECFG MO-SIG BAR SP: Previously is enabled by OAM:%d"
                  "is enabled by MME:%d, recvd reconfig from OAM:%d",
                  bIsAcbMoSigEnabledByOam, bIsAcbMoSigEnabledByMme, 
                  stCellCfgData.stAcbInfo.bIsacBarringMoSig);
            bTriggerRecfgIndForAcb = true;
         }
      }

      m_stMmeMOSigAcbParams.enAcbFactor = stAcBarringMoSig.enAcbFactor;
      m_stMmeMOSigAcbParams.enAcbTime = stAcBarringMoSig.enAcbTime;
      m_stMmeMOSigAcbParams.bAcbForSpac = stAcBarringMoSig.bAcbForSpac;
      bIsAcbMoSigEnabledByOam = stCellCfgData.stAcbInfo.bIsacBarringMoSig;

      m_stMmeHighPriMOTermAcbMoSigParams.enAcbFactor = 
         stAcBarringMoSig.enAcbFactor;
      m_stMmeHighPriMOTermAcbMoSigParams.enAcbTime = 
         stAcBarringMoSig.enAcbTime;

      /* If already overload is going on for ACB triggered by MME, 
       * if OAM says to stop on going ACB, we will stop the 
       * ongoing ACB triggered by RRM based on the MME over load status.
       */
      if((true == bIsAcbMoSigEnabledByMme) && 
            (false == bIsAcbMoSigEnabledByOam))
      {
         bIsAcbMoSigEnabledByMme = false;
      }

      if(bTriggerRecfgIndForAcb)
      {
         printAcbParams(m_stMmeMOSigAcbParams);
      }
   }

   if (stCellCfgInfo.usReCfgType & LRM_CELL_RECFGTYPE_ACB_EMERGENCY)
   {
      if(m_bAcBarringForEmergency != stCellCfgData.stAcbInfo.bAcBarringEmergency)
      {
         bTriggerRecfgIndForAcb = true;
      }
      m_bAcBarringForEmergency = stCellCfgData.stAcbInfo.bAcBarringEmergency;
   }

   if(bTriggerRecfgIndForAcb)
   {
      applyACBParams(m_stMmeMODataAcbParams, m_stMmeMOSigAcbParams);
   }
}

#ifdef RRM_UNUSED_FUNC
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAccessClassBarring::handlePrbUsageInd(U8 bUlPrbUsage, U8  bDlPrbUsage)
{
   U8   bCellPrbUsage = (bUlPrbUsage > bDlPrbUsage) ? bUlPrbUsage : bDlPrbUsage ;
   U8   bPrbUsageIndex = getPrbUsageIndex(bCellPrbUsage);

   if( bPrbUsageIndex == m_bPrevPrbUsageIndex)
      return;

   m_bPrevPrbUsageIndex = bPrbUsageIndex;
   m_stPRBAcbParams.enAcbFactor = g_stAcbPRBTbl[bPrbUsageIndex].enAcbFactor;
   m_stPRBAcbParams.enAcbTime = g_stAcbPRBTbl[bPrbUsageIndex].enAcbTime;

   // if CPU load is already >90, just update the PRB usage
   if( m_bCpuLoad > RM_RAC_ACB_CPULD_CRITICAL_MIN || m_enMMEOverldStatus != RMU_MME_OVLD_STATUS_NORMAL )
      return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 CAccessClassBarring::getPrbUsageIndex(U8 bCellPrbUsage)
{
   U8    bAcbTblIndx=0;

   for(bAcbTblIndx=0; bAcbTblIndx < RRM_RAC_ACB_MAX_ENTRIES; bAcbTblIndx++)
   {
     if( (bCellPrbUsage >= g_stAcbPRBTbl[bAcbTblIndx].bPrbStartThres) &&
         (bCellPrbUsage <= g_stAcbPRBTbl[bAcbTblIndx].bPrbEnbThres))
     {
        return bAcbTblIndx;
     }
   }
   /* If PRB usage is not matches any one of the defined Ragne then
      returning last entry in the Table index:currently 15 */
   return (bAcbTblIndx-1);
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAccessClassBarring::applyACBParams(CmRrmAcb  &stAcBarringMoData, 
                                         CmRrmAcb  &stAcBarringMoSig)
{
   if((true == bIsAcbMoDataEnabledByOam) ||
         (true == bIsAcbMoDataEnabledByMme))
   {
      m_stAcbCfgInfo.stAcbInfo.bIsacBarringMoData = true;
   }
   else
   {
      m_stAcbCfgInfo.stAcbInfo.bIsacBarringMoData = false;
   }

   if((true == bIsAcbMoSigEnabledByOam) ||
         (true == bIsAcbMoSigEnabledByMme))
   {
      m_stAcbCfgInfo.stAcbInfo.bIsacBarringMoSig = true;
   }
   else
   {
      m_stAcbCfgInfo.stAcbInfo.bIsacBarringMoSig = false;
   }

   m_stAcbCfgInfo.stAcbInfo.stAcBarringMoData = stAcBarringMoData;

   m_stAcbCfgInfo.stAcbInfo.stAcBarringMoSig = stAcBarringMoSig;

   m_stAcbCfgInfo.stAcbInfo.bAcBarringEmergency = m_bAcBarringForEmergency;

   rrmSendAcbParamsToFsm(m_stAcbCfgInfo, m_bCellId);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAccessClassBarring::handleCpuLoadInd(U8 bCurrCpuLoad)
{
   U8 bPrevCpuLoad = m_bCpuLoad;

   m_bCpuLoad = bCurrCpuLoad;

   // current CPU load is above 90
   if( bCurrCpuLoad >= RM_RAC_ACB_CPULD_CRITICAL_MIN)
   { 
      // but previous cpu load is also above 90, no need to apply ACB settings
      if( bPrevCpuLoad >= RM_RAC_ACB_CPULD_CRITICAL_MIN)
         return;

      // apply ACB for Mo-signalling and Mo-data
      applyACBParams(m_stCpuACBData, m_stCpuACBData);
      return;
   }

   // previous cpu load and current cpu load is below 90, ignore this condition
   if( bPrevCpuLoad < RM_RAC_ACB_CPULD_CRITICAL_MIN) 
      return;

   // previous CPU load was above 90, and current load is CPU below 90, then trigger the
   // ACB based on prev PRB usage if MME status is normal
   if( m_enMMEOverldStatus != RMU_MME_OVLD_STATUS_NORMAL )
   {
      applyMmeOverloadForACB();
      return;
   }

   // apply ACB settings according to PRB Usage
   applyACBParams(m_stPRBAcbParams, m_stPRBAcbParams);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAccessClassBarring::applyMmeOverloadForACB()
{
   switch (m_enMMEOverldStatus)
   {
      case RMU_MME_REJECT_MODATA:
         {
            if (bIsAcbMoDataEnabledByOam == false)
            {
               /* Enable the barring for MO-data and 
                * disable the barring for Mo-signalling 
                */
               bIsAcbMoDataEnabledByMme = true;
               bIsAcbMoSigEnabledByMme = false;
               applyACBParams(m_stMmeMODataAcbParams, m_stMmeMOSigAcbParams);
            }
            break;
         }

      case RMU_MME_REJECT_MOSIG_MODATA:
      case RMU_MME_PERMITONLY_EMG_MTACCESS:
         {
            if ((bIsAcbMoDataEnabledByOam == false) || 
                  (bIsAcbMoSigEnabledByOam == false))
            {
               /* Enable the barring for MO-data and Mo-signalling */
               bIsAcbMoDataEnabledByMme = true;
               bIsAcbMoSigEnabledByMme = true;
               applyACBParams(m_stMmeMODataAcbParams, m_stMmeMOSigAcbParams);
            }
            break;
         }

      case RMU_MME_PERMITONLY_HIGHPRI_MTACCESS:
         {
            if ((bIsAcbMoDataEnabledByOam == false) || 
                (bIsAcbMoSigEnabledByOam == false))
            {
               /* Enable the barring for MO-data and Mo-signalling */
               bIsAcbMoDataEnabledByMme = true;
               bIsAcbMoSigEnabledByMme = true;
               applyACBParams(m_stMmeHighPriMOTermAcbMoDataParams, 
                     m_stMmeHighPriMOTermAcbMoSigParams);
            }
            break;
         }

      case RMU_MME_REJECT_DELAY_TOLERNT_ACCESS:
         {
            /* ACB is not applied here for this overload status and RRM will 
             * take action only during admit request from an UE
             */
            break;
         }

      case RMU_MME_OVLD_STATUS_NORMAL:
         {
            if(bIsAcbMoSigEnabledByMme || bIsAcbMoDataEnabledByMme)
            {   
               RLOG2(L_INFO, "is Mo-Sig Barring enabled By MME:%d,"
                     "is Mo-Data Barring enabled by MME:%d",
                     bIsAcbMoSigEnabledByMme, bIsAcbMoDataEnabledByMme);
               /* Disable the MME overload for MO-signalling and Mo-data */
               bIsAcbMoDataEnabledByMme = false;
               bIsAcbMoSigEnabledByMme = false;

               /* Apply ACB settings according to OAM configured values But not
                * enabled these optional parameters based MME overload status. 
                */
               applyACBParams(m_stMmeMODataAcbParams, m_stMmeMOSigAcbParams);
            }
            break;
         }
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CAccessClassBarring::updateLeastMmeOverloadStatus(RmuMmeOvldStatus enOvldStatus)
{
   // if previous overload status is same return
   if (m_enMMEOverldStatus == enOvldStatus)
      return;

   m_enMMEOverldStatus = enOvldStatus;

   // If CPU load indication is >= 90 , already both Mo-sig and Mo-data is barred.
   // no need to trigger ACB based on Mme overload status
   if (m_bCpuLoad >= RM_RAC_ACB_CPULD_CRITICAL_MIN)
      return;

   // ACB info based on MME overlaod status
   applyMmeOverloadForACB();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CAccessClassBarring::admitUE(U8 bEstablishmentCause)
{
   if ( m_bAcBarringForEmergency && 
         (bEstablishmentCause == RMU_EST_CAUSE_EMERGENCY))
      return false;

   if( m_enMMEOverldStatus == RMU_MME_OVLD_STATUS_NORMAL )
      return true;

   if( m_enMMEOverldStatus == RMU_MME_REJECT_MODATA )
   {
      if( bEstablishmentCause == RMU_EST_CAUSE_MO_DATA )
         return false;

      if( bEstablishmentCause == RMU_EST_CAUSE_DELAY_TOLERANT )
         return false;

      return true;
   }

   if( m_enMMEOverldStatus == RMU_MME_REJECT_MOSIG_MODATA )
   {
      if( bEstablishmentCause == RMU_EST_CAUSE_MO_DATA )
         return false;

      if( bEstablishmentCause == RMU_EST_CAUSE_MO_SIGNALLING )
         return false;

      if( bEstablishmentCause == RMU_EST_CAUSE_DELAY_TOLERANT )
         return false;

      return true;
   }

   if( m_enMMEOverldStatus == RMU_MME_PERMITONLY_EMG_MTACCESS )
   {
      if( bEstablishmentCause == RMU_EST_CAUSE_EMERGENCY )
         return true;

      if( bEstablishmentCause == RMU_EST_CAUSE_MT_ACCESS )
         return true;

      return false;
   }

   if( m_enMMEOverldStatus == RMU_MME_PERMITONLY_HIGHPRI_MTACCESS )
   {
      if( bEstablishmentCause == RMU_EST_CAUSE_MT_ACCESS )
         return true;

      if( bEstablishmentCause == RMU_EST_CAUSE_HIGH_PRIORTY_ACCESS )
         return true;

      return false;
   }

   if( m_enMMEOverldStatus == RMU_MME_REJECT_DELAY_TOLERNT_ACCESS )
   {
      if( bEstablishmentCause == RMU_EST_CAUSE_DELAY_TOLERANT )
         return false;

      return true;
   }

   return true;
}
/********************************************************************30**

           End of file:     qf_acb.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:32 2013

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
