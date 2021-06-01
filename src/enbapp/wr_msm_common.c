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

     Type:     C include file

     Desc:     This file contains 
               

     File:     wr_msm_interface.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#include "wr.h"
#include "wr_smm_init.h"
#include "wr_msm_common.h"
PUBLIC MsmLteeNodeBparams lteeNodeBparams;
PUBLIC MsmWcdmaNodeBparams wcdmaNodeBparams;
PUBLIC MsmFaultMgmtParams faultMgmtParams;

/** @brief This function is used for setting prach cfg  parameters
*
* @details
*
*     Function: msmSetPrachConfigCommonParameters
*
*         Processing steps:
*
* @param[in] ltePrachCfgCommon
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetPrachConfigCommonParameters(const MsmLtePrachCfgCommon *ltePrachCfgCommon)
{
   if(ltePrachCfgCommon == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.ltePrachCfgCommon = *ltePrachCfgCommon;
   RETVALUE(ROK);
}

PUBLIC S16 msmSetFreqSyncParameters(const MsmLteFreqSyncParams *lteFreqSyncParams)
{
   if(lteFreqSyncParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteFreqSyncParams = *lteFreqSyncParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting RIM parameters
/** @brief This function is used for setting RIM parameters
 * *
 * * @details
 * *
 * *     Function: msmSetRimParameters
 * *
 * *         Processing steps:
 * *
 * * @param[in] lteRimParams
 * * @return S16
 * *        -# Success : ROK
 * *        -# Failure : RFAILED
 * */
PUBLIC S16 msmSetRimParameters(const MsmLteRimParams* lteRimParams)
{
   if(lteRimParams == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteRimParams = *lteRimParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting eNodeb IP  parameters
*
* @details
*
*     Function:msmSetEnodebIpParameters 
*
*         Processing steps:
*
* @param[in] lteeNodeBIPparams 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetEnodebIpParameters(const MsmLteeNodeBIPparams *lteeNodeBIPparams)
{
   if(lteeNodeBIPparams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteeNodeBIPparams = *lteeNodeBIPparams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting RACH cfg parameters
*
* @details
*
*     Function: msmSetRachConfigCommonParameters 
*
*         Processing steps:
*
* @param[in] lteRachCfgCommonParams 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetRachConfigCommonParameters(const MsmLteRachCfgCommonParams *lteRachCfgCommonParams)
{
   if(lteRachCfgCommonParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteRachCfgCommonParams = *lteRachCfgCommonParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting PdschCfg parameters
*
* @details
*
*     Function:msmSetPdschConfigCommonParameters 
*
*         Processing steps:
*
* @param[in] ltePdschCfgCommon 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetPdschConfigCommonParameters(const MsmLtePdschCfgCommon *ltePdschCfgCommon)
{
   if(ltePdschCfgCommon == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.ltePdschCfgCommon = *ltePdschCfgCommon;
   RETVALUE(ROK);
}

/** @brief This function is used for setting ULPwrCtrl parameters
*
* @details
*
*     Function: msmSetUlPowerControlCommonParameters
*
*         Processing steps:
*
* @param[in] lteULPwrCtrlCommon
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetUlPowerControlCommonParameters(const MsmLteULPwrCtrlCommon *lteULPwrCtrlCommon)
{ 
   if(lteULPwrCtrlCommon == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.lteULPwrCtrlCommon = *lteULPwrCtrlCommon;
   RETVALUE(ROK);
}

/** @brief This function is used for setting ULFrequencyInfo parameters
*
* @details
*
*     Function:msmSetUlFrequencyInfoParameters 
*
*         Processing steps:
*
* @param[in] lteULFrequencyInfo
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetUlFrequencyInfoParameters(const MsmLteULFrequencyInfo *lteULFrequencyInfo)
{
   if(lteULFrequencyInfo == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteULFrequencyInfo = *lteULFrequencyInfo;
   RETVALUE(ROK);
}

/** @brief This function is used for setting lteCellSib3CfgGrp parameters
*
* @details
*
*     Function:msmSetCell_Sib3ConfigGroupParameters 
*
*         Processing steps:
*
* @param[in] lteCellSib3CfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCell_Sib3ConfigGroupParameters(const MsmLteCellSib3CfgGrp *lteCellSib3CfgGrp)
{
   if( lteCellSib3CfgGrp== NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteCellSib3CfgGrp = *lteCellSib3CfgGrp;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Cell Sib1 Cfg parameters
*
* @details
*
*     Function:msmSetCellSib1ConfigGroupParameters 
*
*         Processing steps:
*
* @param[in] lteCellSib1CfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCellSib1ConfigGroupParameters(const MsmLteCellSib1CfgGrp *lteCellSib1CfgGrp, U32 isPlmnCfgd)
{
   if(lteCellSib1CfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   if(FALSE == isPlmnCfgd)
   {
      lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.tac = lteCellSib1CfgGrp->cellAccessInfo.tac;
      lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.cellId = lteCellSib1CfgGrp->cellAccessInfo.cellId;
      lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.cellBarred = lteCellSib1CfgGrp->cellAccessInfo.cellBarred;
      lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.csgIdentity = lteCellSib1CfgGrp->cellAccessInfo.csgIdentity;
      lteeNodeBparams.lteCellSib1CfgGrp.cellSelectInfo = lteCellSib1CfgGrp->cellSelectInfo;
      lteeNodeBparams.lteCellSib1CfgGrp.pMax = lteCellSib1CfgGrp->pMax;
      lteeNodeBparams.lteCellSib1CfgGrp.freqBandInd = lteCellSib1CfgGrp->freqBandInd;
   }
   else
   {
      lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.numOfPlmns = lteCellSib1CfgGrp->cellAccessInfo.numOfPlmns;
      lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.maxNumOfPlmns = lteCellSib1CfgGrp->cellAccessInfo.maxNumOfPlmns;
      for(U32 idx = 0; idx < lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.numOfPlmns; idx++)
      {
         lteeNodeBparams.lteCellSib1CfgGrp.cellAccessInfo.plmnInfo[idx] = lteCellSib1CfgGrp->cellAccessInfo.plmnInfo[idx];
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is used for setting Drx Cfg Params parameters
*
* @details
*
*     Function:msmSetDrxConfigParameters 
*
*         Processing steps:
*
* @param[in] lteDrxCfgParams 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetDrxConfigParameters(const MsmLteDrxCfgParams *lteDrxCfgParams)
{
   if(lteDrxCfgParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteDrxCfgParams = *lteDrxCfgParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Cell Meas Cfg parameters
*
* @details
*
*     Function: msmSetCellMeasConfigParameters
*
*         Processing steps:
*
* @param[in] lteCellMeasCfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCellMeasConfigParameters(const MsmLteCellMeasCfgGrp *lteCellMeasCfgGrp)
{
   if(lteCellMeasCfgGrp == NULLP)

   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteCellMeasCfgGrp = *lteCellMeasCfgGrp;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Add Cell Cfg parameters
*
* @details
*
*     Function: msmSetAddCellConfigParameters 
*
*         Processing steps:
*
* @param[in] lteAddCellCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetAddCellConfigParameters(const MsmLteAddCellCfg *lteAddCellCfg)
{
   if(lteAddCellCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteAddCellCfg = *lteAddCellCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Sm Cell Cfg parameters
*
* @details
*
*     Function:msmSetSmCellConfigParameters 
*
*         Processing steps:
*
* @param[in] lteSmCellCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetSmCellConfigParameters(const MsmLteSmCellCfg *lteSmCellCfg)
{
   if(lteSmCellCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteSmCellCfg = *lteSmCellCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Cell Mib Cfg parameters
*
* @details
*
*     Function:msmSetCellMibConfigParameters 
*
*         Processing steps:
*
* @param[in] lteCellMibCfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCellMibConfigParameters(const MsmLteCellMibCfgGrp *lteCellMibCfgGrp)
{
   if(lteCellMibCfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteCellMibCfgGrp = *lteCellMibCfgGrp;
   RETVALUE(ROK);
}

/** @brief This function is used for setting MAC_SCH_Conf parameters
*
* @details
*
*     Function:msmSetMacSchConfigParameters 
*
*         Processing steps:
*
* @param[in] lteMAC_SCH_Config
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetMacSchConfigParameters(const MsmLteMAC_SCH_Config *lteMAC_SCH_Config)
{
   if(lteMAC_SCH_Config == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteMAC_SCH_Config = *lteMAC_SCH_Config;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Antenna Common Config parameters
*
* @details
*
*     Function:msmSetAntennaCommonConfigParameters 
*
*         Processing steps:
*
* @param[in] lteAntenna_Common_Config 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetAntennaCommonConfigParameters(const MsmLteAntenna_Common_Config *lteAntenna_Common_Config)
{
   if(lteAntenna_Common_Config == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteAntenna_Common_Config = *lteAntenna_Common_Config;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Pdsch Cfg Dedicated parameters
*
* @details
*
*     Function:msmSetPdschConfigDedicatedParameters 
*
*         Processing steps:
*
* @param[in] ltePdschCfgDedicated
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetPdschConfigDedicatedParameters(const MsmLtePdschCfgDedicated *ltePdschCfgDedicated)
{
   if(ltePdschCfgDedicated == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.ltePdschCfgDedicated = *ltePdschCfgDedicated;

   RETVALUE(ROK);
}

/** @brief This function is used for setting Pusch Basic Cfg Common parameters
*
* @details
*
*     Function:msmSetPuschBasicCfgCommonParameters 
*
*         Processing steps:
*
* @param[in] ltePuschBasicCfgCommon
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetPuschBasicCfgCommonParameters(const MsmLtePuschBasicCfgCommon *ltePuschBasicCfgCommon)
{ 
   if(ltePuschBasicCfgCommon== NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.ltePuschBasicCfgCommon = *ltePuschBasicCfgCommon;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Pusch Ref Signa lCfg  parameters
*
* @details
*
*     Function:msmSetPuschRefSignalCfgParameters 
*
*         Processing steps:
*
* @param[in] ltePuschRefSignalCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetPuschRefSignalCfgParameters(const MsmLtePuschRefSignalCfg *ltePuschRefSignalCfg)
{ 
   if(ltePuschRefSignalCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.ltePuschRefSignalCfg = *ltePuschRefSignalCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Pucch Cfg Common parameters
*
* @details
*
*     Function:msmSetPucchCfgCommonParameters 
*
*         Processing steps:
*
* @param[in] ltePucchCfgCommon
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetPucchCfgCommonParameters(const MsmLtePucchCfgCommon *ltePucchCfgCommon)
{
   if(ltePucchCfgCommon == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.ltePucchCfgCommon = *ltePucchCfgCommon;
   RETVALUE(ROK);
}
/** @brief This function is used for setting SRS Cfg Common parameters
*
* @details
*
*     Function:msmSetSrsCfgCommonParameters 
*
*         Processing steps:
*
* @param[in]lteSRSCfgCommon 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetSrsCfgCommonParameters(const MsmLteSRSCfgCommon *lteSRSCfgCommon)
{
   if(lteSRSCfgCommon == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.lteSRSCfgCommon = *lteSRSCfgCommon;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Rab Policy Cfg parameters
*
* @details
*
*     Function:msmSetRabPolicyCfgGrpParameters 
*
*         Processing steps:
*
* @param[in] lteRabPolicyCfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetRabPolicyCfgGrpSrb1Parameters(const MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp)
{ 
   if(lteRabPolicyCfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.lteRabPolicyCfgGrp.actType = lteRabPolicyCfgGrp->actType;
   lteeNodeBparams.lteRabPolicyCfgGrp.srbPolicy[MSM_SRB1] = lteRabPolicyCfgGrp->srbPolicy[MSM_SRB1];
   RETVALUE(ROK);
}

/** @brief This function is used for setting Rab Policy Cfg parameters
*
* @details
*
*     Function:msmSetRabPolicyCfgGrpParameters 
*
*         Processing steps:
*
* @param[in] lteRabPolicyCfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetRabPolicyCfgGrpSrb2Parameters(const MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp)
{ 
   if(lteRabPolicyCfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.lteRabPolicyCfgGrp.actType = lteRabPolicyCfgGrp->actType;
   lteeNodeBparams.lteRabPolicyCfgGrp.srbPolicy[MSM_SRB2] = lteRabPolicyCfgGrp->srbPolicy[MSM_SRB2];
   RETVALUE(ROK);
}

/** @brief This function is used for setting Rab Policy Cfg parameters
*
* @details
*
*     Function:msmSetRabPolicyCfgGrpParameters 
*
*         Processing steps:
*
* @param[in] lteRabPolicyCfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetRabPolicyCfgGrpDrbParameters(const MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp)
{
   U32 idx;
   if(lteRabPolicyCfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteRabPolicyCfgGrp.isDscpEnable = lteRabPolicyCfgGrp->isDscpEnable;
   for(idx = 0; idx < MSM_MAX_QCI; idx++)
   {   
      lteeNodeBparams.lteRabPolicyCfgGrp.drbPolicy[idx] = lteRabPolicyCfgGrp->drbPolicy[idx];
   } 
   RETVALUE(ROK);
}

/** @brief This function is used for setting Ue Timer Constants parameters
*
* @details
*
*     Function:msmSetUeTimerConstantsParameters 
*
*         Processing steps:
*
* @param[in] lteUeTimerConstants) 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetUeTimerConstantsParameters(const MsmLteUeTimerConstants *lteUeTimerConstants)
{
   if(lteUeTimerConstants == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteUeTimerConstants = *lteUeTimerConstants;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Cell EAID Cfg parameters
*
* @details
*
*     Function:msmSetCellEaidCfgGrpParameters 
*
*         Processing steps:
*
* @param[in] lteCellEAIDCfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCellEaidCfgGrpParameters(const MsmLteCellEAIDCfgGrp *lteCellEAIDCfgGrp)
{
   if(lteCellEAIDCfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteCellEAIDCfgGrp = *lteCellEAIDCfgGrp;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Cell Sib9 Cfg parameters
*
* @details
*
*     Function:msmSetCellSib9CfgGrpParameters 
*
*         Processing steps:
*
* @param[in] lteCellSib9CfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCellSib9CfgGrpParameters(const MsmLteCellSib9CfgGrp *lteCellSib9CfgGrp)
{
   if(lteCellSib9CfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteCellSib9CfgGrp = *lteCellSib9CfgGrp;
   RETVALUE(ROK);
}

/** @brief This function is used for setting CQIPeriodicReportCfg parameters
*
* @details
*
*     Function:msmSetCqiPeriodicReportCfgParameters 
*
*         Processing steps:
*
* @param[in] lteCQIPeriodicReportCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetCqiPeriodicReportCfgParameters(const MsmLteCQIPeriodicReportCfg *lteCQIPeriodicReportCfg)
{
   if(lteCQIPeriodicReportCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteCQIPeriodicReportCfg = *lteCQIPeriodicReportCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting lteNeighFreqCfg parameters
*
* @details
*
*     Function:msmSetNeighFreqCfgParameters 
*
*         Processing steps:
*
* @param[in]lteNeighFreqCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetNeighFreqEutraCfgParameters(const MsmLteNeighFreqCfg *lteNeighFreqCfg)
{
   U16 idx;
   if(lteNeighFreqCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.lteNeighFreqCfg.actType = lteNeighFreqCfg->actType;
   lteeNodeBparams.lteNeighFreqCfg.numEutraFreq = lteNeighFreqCfg->numEutraFreq;
   for(idx=0;idx< (lteNeighFreqCfg->numEutraFreq);idx++)

   {
      lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq = lteNeighFreqCfg->t.addNeighFreq[idx].eutranFreq;
   }
   RETVALUE(ROK);
}
 
/** @brief This function is used for setting lteNeighFreqCfg parameters
*
* @details
*
*     Function:msmSetTdsCdmaUtranTddFreqParameters
*
*         Processing steps:
*
* @param[in]lteNeighFreqCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetTdsCdmaUtranTddFreqParameters(const MsmLteNeighFreqCfg *lteNeighFreqCfg)
{
   U16 idx;
   if(lteNeighFreqCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteNeighFreqCfg.actType = lteNeighFreqCfg->actType;
   lteeNodeBparams.lteNeighFreqCfg.numUtraTddFreq = lteNeighFreqCfg->numUtraTddFreq;
   lteeNodeBparams.lteNeighFreqCfg.maxUtraTddFreqEntries = lteNeighFreqCfg->maxUtraTddFreqEntries;
   for(idx=0; idx< (lteNeighFreqCfg->numUtraTddFreq);idx++)
   {
      lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].utranTddFreq = lteNeighFreqCfg->t.addNeighFreq[idx].utranTddFreq;
   }
   RETVALUE(ROK);

}

/** @brief This function is used for setting lteNeighFreqCfg parameters
*
* @details
*
*     Function:msmSetNeighFreqCfgParameters 
*
*         Processing steps:
*
* @param[in]lteNeighFreqCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetNeighFreqUtraCfgParameters(const MsmLteNeighFreqCfg *lteNeighFreqCfg)
{
   U16 idx;
   if(lteNeighFreqCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteNeighFreqCfg.actType = lteNeighFreqCfg->actType;
   lteeNodeBparams.lteNeighFreqCfg.numUtraFreq = lteNeighFreqCfg->numUtraFreq;
   lteeNodeBparams.lteNeighFreqCfg.maxUtraFreqEntries = lteNeighFreqCfg->maxUtraFreqEntries;
   for(idx=0; idx< (lteNeighFreqCfg->numUtraFreq);idx++)
   {
      lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].utranFreq = lteNeighFreqCfg->t.addNeighFreq[idx].utranFreq;
   }
   RETVALUE(ROK);

}

/** @brief This function is used for setting Eutra Neighbor Cell Configuration parameters
*
* @details
*
*     Function:msmSetNeighborListInUseParameters 
*
*         Processing steps:
*
* @param[in]lteNeighCellCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetNeighborListInUseEutraParameters(const MsmNeighCellCfg *lteNeighCellCfg)
{
   U16 idx;
   if((lteNeighCellCfg == NULLP)|| (lteeNodeBparams.lteNeighCellCfg.numEutraCells > MSM_MAX_NUM_NEIGH_CELLS))
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteNeighCellCfg.isX2Enabled = lteNeighCellCfg->isX2Enabled;
   lteeNodeBparams.lteNeighCellCfg.actType = lteNeighCellCfg->actType;
   lteeNodeBparams.lteNeighCellCfg.maxCellEntries = lteNeighCellCfg->maxCellEntries;
   lteeNodeBparams.lteNeighCellCfg.numEutraCells = lteNeighCellCfg->numEutraCells;
   for(idx=0;idx< lteNeighCellCfg->numEutraCells;idx++)
   {  
      lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell = lteNeighCellCfg->t.neighAddCfg[idx].eutraCell;
   }  
   RETVALUE(ROK);
}
 
/** @brief This function is used for setting UTRA lteNeighCellCfg parameters
*
* @details
*
*     Function:msmSetTdsCdmaUtranTddCellParameters
*
*         Processing steps:
*
* @param[in]lteNeighCellCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetTdsCdmaUtranTddCellParameters(const MsmNeighCellCfg *lteNeighCellCfg)
{
   U16 idx;
   if((lteNeighCellCfg == NULLP)|| (lteeNodeBparams.lteNeighCellCfg.numUtraTddCells > MSM_MAX_NUM_NEIGH_CELLS))
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteNeighCellCfg.actType = lteNeighCellCfg->actType;
   lteeNodeBparams.lteNeighCellCfg.maxTddCellEntries = lteNeighCellCfg->maxTddCellEntries;
   lteeNodeBparams.lteNeighCellCfg.numUtraTddCells = lteNeighCellCfg->numUtraTddCells;
   for(idx=0;idx< lteNeighCellCfg->numUtraTddCells ;idx++)
   {  
      lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].utranTddCell = lteNeighCellCfg->t.neighAddCfg[idx].utranTddCell;
   }  

   RETVALUE(ROK);
}

/** @brief This function is used for setting UTRA lteNeighCellCfg parameters
*
* @details
*
*     Function:msmSetNeighborListInUseParameters 
*
*         Processing steps:
*
* @param[in]lteNeighCellCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetNeighborListInUseUtraParameters(const MsmNeighCellCfg *lteNeighCellCfg)
{
   U16 idx;
   if((lteNeighCellCfg == NULLP)|| (lteeNodeBparams.lteNeighCellCfg.numUtraCells > MSM_MAX_NUM_NEIGH_CELLS))
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteNeighCellCfg.actType = lteNeighCellCfg->actType;
   lteeNodeBparams.lteNeighCellCfg.maxCellEntries = lteNeighCellCfg->maxCellEntries;
   lteeNodeBparams.lteNeighCellCfg.numUtraCells = lteNeighCellCfg->numUtraCells;
   for(idx=0;idx< lteNeighCellCfg->numUtraCells ;idx++)
   {  
      lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].utranCell = lteNeighCellCfg->t.neighAddCfg[idx].utranCell;
   }  

   RETVALUE(ROK);
}

/** @brief This function is used for setting Sib5 Inter Freq Carrier parameters
*
* @details
*
*     Function: msmSetSib5InterFreqCarrierInfoParameters
*
*         Processing steps:
*
* @param[in]lteSib5InterFreqCarrierInfo 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetSib5InterFreqCarrierInfoParameters(const MsmLteSib5InterFreqCarrierInfo *lteSib5InterFreqCarrierInfo)
{
   if(lteSib5InterFreqCarrierInfo == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteSib5InterFreqCarrierInfo = *lteSib5InterFreqCarrierInfo;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Cell Sib6 Cfg Grp parameters
*
* @details
*
*     Function:msmSetCellSib6ConfigGroupParameters 
*
*         Processing steps:
*
* @param[in]lteCellSib6CfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCellSib6ConfigGroupParameters(const MsmLteCellSib6CfgGrp *lteCellSib6CfgGrp)
{
   if(lteCellSib6CfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteCellSib6CfgGrp = *lteCellSib6CfgGrp;
   RETVALUE(ROK);
}
/** @brief This function is used for setting Enb Proto Cfg parameters
*
* @details
*
*     Function:msmSetEnbProtoCfgParameters 
*
*         Processing steps:
*
* @param[in] lteEnbProtoCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetEnbProtoCfgParameters(const MsmLteEnbProtoCfg *lteEnbProtoCfg)
{
   if(lteEnbProtoCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteEnbProtoCfg = *lteEnbProtoCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Ac Barring Info parameters
*
* @details
*
*     Function:msmSetAcBarringInfoParameters 
*
*         Processing steps:
*
* @param[in]lteAcBarringInfo 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetAcBarringInfoParameters(const MsmLteAcBarringInfo *lteAcBarringInfo)
{
   if(lteAcBarringInfo == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteAcBarringInfo = *lteAcBarringInfo;
   RETVALUE(ROK);
}

/** @brief This function is used for setting enodeb IP parameters
*
* @details
*
*     Function: msmSetEnodebIpAddr
*
*         Processing steps:
*
* @param[in]lteEnodebIpAddr
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetEnodebIpAddr(const MsmEnodebIpAddr *lteEnodebIpAddr)
{
   if(lteEnodebIpAddr == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteEnodebIpAddr = *lteEnodebIpAddr;
   RETVALUE(ROK);
}


/** @brief This function is used for setting EARFCNDL and RSSI scanned by REM
*
* @details
*
*     Function:msmSetEarfcnRssiParameters
*
*         Processing steps:
*
* @param[in]lteEarfcnRssiCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetEarfcnRssiParameters(const MsmLteEarfcnRssiCfg *lteEarfcnRssiCfg)
{
   if(lteEarfcnRssiCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteEarfcnRssiCfg = *lteEarfcnRssiCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Self Config Params parameters
*
* @details
*
*     Function:msmSetSelfConfigParameters
*
*         Processing steps:
*
* @param[in]lteSelfConfigParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetSelfConfigParameters(const MsmLteSonGenericParams *lteSelfConfigParams)
{
   if(lteSelfConfigParams == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteSonGenericParams = *lteSelfConfigParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Power Off set parameters
*
* @details
*
*     Function:msmSetDynCfiParameters 
*
*         Processing steps:
*
* @param[in] lteDynCfiParams 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetDynCfiParameters(const MsmLteDynCfiGrp *lteDynCfiParams)
{
   if(lteDynCfiParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteDynCfiParams = *lteDynCfiParams;
   RETVALUE(ROK);
}


/** @brief This function is used for setting Power Off set parameters
*
* @details
*
*     Function:msmSetPowerOffsetParameters 
*
*         Processing steps:
*
* @param[in] ltePowerOffsetParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetPowerOffsetParameters(const MsmLtePowerOffsetParams *ltePowerOffsetParams)
{
   if(ltePowerOffsetParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.ltePowerOffsetParams = *ltePowerOffsetParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Fap Control parameters
*
* @details
*
*     Function:msmSetFapControlParameters 
*
*         Processing steps:
*
* @param[in]lteFapControlParams 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetFapControlParameters(const MsmLteFapControlParams *lteFapControlParams)
{
   if(lteFapControlParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteFapControlParams = *lteFapControlParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting PRB allocation parameters
*
* @details
*
*     Function:msmSetPrbAllocationParameters 
*
*         Processing steps:
*
* @param[in] ltePRBallocation
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetPrbAllocationParameters(const MsmLtePRBallocation *ltePRBallocation)
{
   if(ltePRBallocation == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.ltePRBallocation = *ltePRBallocation;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Sib4 Csg Info parameters
*
* @details
*
*     Function:msmSetSib4CsgInfoParameters 
*
*         Processing steps:
*
* @param[in] lteSib4CsgInfo
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetSib4CsgInfoParameters(const MsmLteSib4CsgInfo *lteSib4CsgInfo)
{
   if(lteSib4CsgInfo == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteSib4CsgInfo = *lteSib4CsgInfo;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Rrm Tdd Param parameters
*
* @details
*
*     Function:msmSetRrmTddParameters 
*
*         Processing steps:
*
* @param[in] lteRrmTddParam
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetRrmTddParameters(const MsmLteRrmTddParam *lteRrmTddParam)
{
   if(lteRrmTddParam== NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteRrmTddParam = *lteRrmTddParam;
   RETVALUE(ROK);
}

/** @brief This function is used for setting X2 Config Params parameters
*
* @details
*
*     Function:msmSetX2ConfigParameters 
*
*         Processing steps:
*
* @param[in] lteX2ConfigParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetX2ConfigParameters(const MsmLteX2ConfigParams *lteX2ConfigParams)
{
   if(lteX2ConfigParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteX2ConfigParams = *lteX2ConfigParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Sctp Cfg parameters
*
* @details
*
*     Function: msmSetSctpConfigParameters
*
*         Processing steps:
*
* @param[in] lteSctpCfgParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetSctpConfigParameters(const MsmLteSctpCfgParams *lteSctpCfgParams)
{
   if(lteSctpCfgParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteSctpCfgParams = *lteSctpCfgParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting PCCH Cfg parameters
*
* @details
*
*     Function:msmSetPcchConfigCommonParameters 
*
*         Processing steps:
*
* @param[in]ltePCCHCfgCommon 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetPcchConfigCommonParameters(const MsmLtePCCHCfgCommon *ltePCCHCfgCommon)
{
   if(ltePCCHCfgCommon == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.ltePCCHCfgCommon = *ltePCCHCfgCommon;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Cell Ul Schd Cfg parameters
*
* @details
*
*     Function:msmSetCellUlSchdConfigParameters 
*
*         Processing steps:
*
* @param[in]lteCellUlSchdCfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCellUlSchdConfigParameters(const MsmLteCellUlSchdCfgGrp *lteCellUlSchdCfgGrp)
{
   if(lteCellUlSchdCfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteCellUlSchdCfgGrp = *lteCellUlSchdCfgGrp;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Cell Dl Schd Cfg parameters
*
* @details
*
*     Function:msmSetCellDlSchdConfigParameters 
*
*         Processing steps:
*
* @param[in]lteCellDlSchdCfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCellDlSchdConfigParameters(const MsmLteCellDlSchdCfgGrp *lteCellDlSchdCfgGrp)
{
   if(lteCellDlSchdCfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteCellDlSchdCfgGrp = *lteCellDlSchdCfgGrp;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Ms Cell Cfg parameters
*
* @details
*
*     Function:msmSetMsCellCfgReqParameters 
*
*         Processing steps:
*
* @param[in] lteMsCellCfgReq
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetMsCellCfgReqParameters(const MsmLteMsCellCfgReq *lteMsCellCfgReq)
{
   if(lteMsCellCfgReq == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteMsCellCfgReq = *lteMsCellCfgReq;
   RETVALUE(ROK);
}

/** @brief This function is used for setting DbgParams parameters
*
* @details
*
*     Function:msmSetDbgParameters 
*
*         Processing steps:
*
* @param[in] lteDbgParams 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetDbgParameters(const MsmLteDbgParams *lteDbgParams)
{
   if(lteDbgParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteDbgParams = *lteDbgParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Ul Alloc Info parameters
*
* @details
*
*     Function:msmSetUlAllocInfoCbParameters 
*
*         Processing steps:
*
* @param[in]lteUlAllocInfoCbParams 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetUlAllocInfoCbParameters(const MsmLteUlAllocInfoCbParams *lteUlAllocInfoCbParams)
{
   if(lteUlAllocInfoCbParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteUlAllocInfoCbParams = *lteUlAllocInfoCbParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Proc Timer Cfg parameters
*
* @details
*
*     Function:msmSetProcTimerCfgParameters 
*
*         Processing steps:
*
* @param[in] lteProcTimerCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetProcTimerCfgParameters(const MsmLteProcTimerCfg *lteProcTimerCfg)
{
   if(lteProcTimerCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteProcTimerCfg = *lteProcTimerCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Rem Control parameters
*
* @details
*
*     Function:msmSetFactorySmCellParameters 
*
*         Processing steps:
*
* @param[in] lteSmCellCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetFactorySmCellParameters(const MsmLteSmCellCfg *lteSmCellCfg)
{
   if(lteSmCellCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteSmCellCfg.maxUeSupp = lteSmCellCfg->maxUeSupp;
   RETVALUE(ROK);
}



PUBLIC S16 msmSetFapAccessParameters(const MsmLteFapAccessParams *lteFapAccessParams)
        {
           if(lteFapAccessParams == NULLP)
           {
              RETVALUE(RFAILED);
              
           }
      lteeNodeBparams.lteFapAccessParams=*lteFapAccessParams;  //    *lteFapAccessParams = lteeNodeBparams.lteFapAccessParams;
           RETVALUE(ROK);
        }



PUBLIC S16 msmGetFapAccessParameters( MsmLteFapAccessParams *lteFapAccessParams)
	{
	   if(lteFapAccessParams == NULLP)
	   {
	      RETVALUE(RFAILED);
	   }
	   *lteFapAccessParams = lteeNodeBparams.lteFapAccessParams;
	   RETVALUE(ROK);
	}

/** @brief This function is used for setting Rem Control parameters
*
* @details
*
*     Function: msmSetFactoryFapAceessParameters
*
*         Processing steps:
*
* @param[in] lteFapAccessParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
           
PUBLIC S16 msmSetFapAcceessParameters(const MsmLteFapAccessParams *lteFapAccessParams)
{
   if(lteFapAccessParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteFapAccessParams = *lteFapAccessParams;
   RETVALUE(ROK);
}

#ifdef LTE_QCOM
/** @brief This function is used for setting Qualcomm
*   RF configuration parameters
*
* @details
*
*     Function:msmSetRsysQcomCfgParam 
*
*         Processing steps:
*
* @param[in] lteRsysQcomCfgParam
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
S16 msmSetRsysQcomCfgParam(const MsmRsysQcomCfgParam  *lteRsysQcomCfgParam)
{
   if(lteRsysQcomCfgParam == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteRsysQcomCfgParam= *lteRsysQcomCfgParam;
   RETVALUE(ROK);

}
#endif

/** @brief This function is used for setting perf Mgmt parameters
*
* @details
*
*     Function:msmSetPmParameters 
*
*         Processing steps:
*
* @param[in] perfMgmtParameters
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*rakesh*/

PUBLIC S16 msmSetPmParameters(const MsmPerfMgmtParameters *perfMgmtParameters)
{
   if(perfMgmtParameters == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   wcdmaNodeBparams.perfMgmtParameters = *perfMgmtParameters;
   RETVALUE(ROK);
}

/** @brief This function is used for setting frequent Fault Mgmt parameters
*
* @details
*
*     Function:msmSetFrequentFaultParameters 
*
*         Processing steps:
*
* @param[in]frequentFaultMgmtParameters 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*rakesh*/

PUBLIC S16 msmSetFrequentFaultParameters(const MsmFrequentFaultMgmtParameters *frequentFaultMgmtParameters)
{
   if(frequentFaultMgmtParameters == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   faultMgmtParams.frequentFaultMgmtParameters = *frequentFaultMgmtParameters;
   RETVALUE(ROK);
}

/** @brief This function is used for setting dma Band Class parameters
*
* @details
*
*     Function:msmSetCdmaBandClassParameters 
*
*         Processing steps:
*
* @param[in] cdmaBandClass
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
**/

PUBLIC S16 msmSetCdmaBandClassParameters(const MsmCdmaBandClass *cdmaBandClassParams)
{
   if(cdmaBandClassParams == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.cdmaBandClassParams = *cdmaBandClassParams;
   RETVALUE(ROK);
}

/** @brief This function is used for setting generic parameters
*
* @details
*
*     Function: msmSetCdmaMobilityParameters
*
*         Processing steps:
*
* @param[in] cdmaMobilityParam
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCdmaMobilityParameters(const MsmCdmaMobilityParam *cdmaMobilityParam)
{
   if(cdmaMobilityParam == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.cdmaMobilityParam = *cdmaMobilityParam;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Config 1x RTTC parameters
*
* @details
*
*     Function:msmSetBarringCfg1xRtt 
*
*         Processing steps:
*
* @param[in] barringConfig1xRTTCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetBarringCfg1xRtt(const MsmBarringConfig1xRTTCfg *barringConfig1xRTTCfg)
{
   if(barringConfig1xRTTCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.barringConfig1xRTTCfg = *barringConfig1xRTTCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Neigh Cell parameters
*
* @details
*
*     Function:msmSetCdma1XRttNghCell 
*
*         Processing steps:
*
* @param[in] lteNeighCellCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCdma1XRttNghCell(const MsmNeighCellCfg *lteNeighCellCfg)
{
   U32 idx = 0;
   if(lteNeighCellCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteNeighCellCfg.numCdma1xCells = lteNeighCellCfg->numCdma1xCells;
   for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numCdma1xCells; idx++)
   {
      lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].cdma1xCell = lteNeighCellCfg->t.neighAddCfg[idx].cdma1xCell;
   }
   RETVALUE(ROK);
}

/** @brief This function is used for setting generic parameters
*
* @details
*
*     Function:msmSetSib8Parameters 
*
*         Processing steps:
*
* @param[in] cellSib8CfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetSib8Parameters(const MsmCellSib8CfgGrp *cellSib8CfgGrp)
{
   if(cellSib8CfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.cellSib8CfgGrp = *cellSib8CfgGrp;
   RETVALUE(ROK);
}

/** @brief This function is used for setting csfb Cfg parameters
*
* @details
*
*     Function:msmSetCsfbCfgParameters 
*
*         Processing steps:
*
* @param[in] csfbCfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetCsfbCfgParameters(const MsmCsfbCfgGrp *csfbCfgGrp)
{
   if(csfbCfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.csfbCfgGrp = *csfbCfgGrp;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Sps Cell Cfg parameters
*
* @details
*
*     Function:msmSetSpsParameters 
*
*         Processing steps:
*
* @param[in] stSpsCellCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetSpsParameters(const MsmSpsCellConfigData *stSpsCellCfg)
{
   if(stSpsCellCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.stSpsCellCfg = *stSpsCellCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting eNB log level parameter
*
* @details
*
*     Function:msmSetLogLvlParameter 
*
*         Processing steps:
*
* @param[in] lteLogLvlCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetLogLvlParameter(const MsmEnbLogLvlData *lteLogLvlCfg)
{
   if(lteLogLvlCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteLogLvlCfg = *lteLogLvlCfg;
   RETVALUE(ROK);
}

/*TM4 changes */
/** @brief This function is used for setting Transmission Mode Cfg 
*
* @details
*
*     Function:msmSetTMParameters 
*
*         Processing steps:
*
* @param[in]lteTransModeCellCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmSetTMParameters (const MsmTMCellConfig *lteTransModeCellCfg)
{
   if(lteTransModeCellCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.lteTransModeCellCfg = *lteTransModeCellCfg;
   RETVALUE(ROK);
}
/** @brief This function is used for setting CNM Param 
*
* @details
*
*     Function:msmSetCellCnmCfgGrpParameters
*
*         Processing steps:
*
* @param[in]lteTddParam 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
   
PUBLIC S16 msmSetCellCnmCfgGrpParameters(MsmCnmCfg *lteCnmCfg)
{ 
   if(lteCnmCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.lteCnmCfg = *lteCnmCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Total eNodeB stats configuration 
*
* @details
*
*     Function:msmSetTenbStatsCfgGrpParameters
*
*         Processing steps:
*
* @param[in] : 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
   
PUBLIC S16 msmSetTenbStatsCfgGrpParameters(MsmTenbStatsCfg *tenbStatsCfg)
{ 
   if(tenbStatsCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.tenbStatsCfg= *tenbStatsCfg;
   RETVALUE(ROK);
}


/** @brief This function is used for setting Tdd Param 
*
* @details
*
*     Function:msmSetTddParameters
*
*         Processing steps:
*
* @param[in]lteTddParam 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
   
PUBLIC S16 msmSetTddParameters(MsmLteTddParam *lteTddParam)
{ 
   if(lteTddParam == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.lteTddParam =*lteTddParam;
   RETVALUE(ROK);
}


/** @brief This function is used for setting Sib7Cfg Param 
 *
 * @details
 *
 *     Function:msmSetsib7CfgGrpParameters
 *
 *         Processing steps:
 *
 * @param[in]sib7CfgGrp
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */

PUBLIC S16 msmSetsib7CfgGrpParameters(MsmLteCellSib7CfgGrp *sib7CfgGrp)
{ 
   if(sib7CfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.sib7CfgGrp = *sib7CfgGrp;
   RETVALUE(ROK);
}


/** @brief This function is used for setting csfbGranCfg Param 
 *
 * @details
 *
 *     Function:msmSetcsfbGeranCfgParameters
 *
 *         Processing steps:
 *
 * @param[in]csfbToGeranCfg
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */

PUBLIC S16 msmSetcsfbGeranCfgParameters(MsmLteCsfbGeranCfg *csfbToGeranCfg)
{ 
   if(csfbToGeranCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.csfbToGeranCfg = *csfbToGeranCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting NeighFreqCfgGeran Param 
 *
 * @details
 *
 *     Function:msmSetNeighFreqCfgGeranParameters
 *
 *         Processing steps:
 *
 * @param[in]lteNeighFreqCfg
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */

PUBLIC S16 msmSetNeighFreqCfgGeranParameters(const MsmLteNeighFreqCfg *lteNeighFreqCfg)
{ 
   U16 idx = 0;

   if(lteNeighFreqCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.lteNeighFreqCfg.actType = lteNeighFreqCfg->actType;
   lteeNodeBparams.lteNeighFreqCfg.numGeranFreq = lteNeighFreqCfg->numGeranFreq;
   for(idx=0; idx< (lteNeighFreqCfg->numGeranFreq); idx++)
   {
      lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].geranFreq = lteNeighFreqCfg->t.addNeighFreq[idx].geranFreq;
   }

   RETVALUE(ROK);
}

/** @brief This function is used for setting Neigh Cell Geran parameters
 *
 * @details
 *
 *     Function:msmSetNeighCellCfgGeranParameters
 *
 *         Processing steps:
 *
 * @param[in] lteNeighCellCfg
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */

PUBLIC S16 msmSetNeighCellCfgGeranParameters(const MsmNeighCellCfg *lteNeighCellCfg)
{
   U32 idx = 0;

   if(lteNeighCellCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.lteNeighCellCfg.numGeranCells = lteNeighCellCfg->numGeranCells;

   for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numGeranCells; idx++)
   {
      lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].geranCell = lteNeighCellCfg->t.neighAddCfg[idx].geranCell;
   }

   RETVALUE(ROK);
}
/** @brief This function is used for setting Sib2 Param 
*
* @details
*
*     Function:msmSetSib2Cfg
*
*         Processing steps:
*
* @param[in]
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetSib2Cfg(MsmSib2Cfg *sib2Cfg)
{ 
   if(sib2Cfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.sib2Cfg = *sib2Cfg;
   RETVALUE(ROK);
}
 
/** @brief This function is used for setting Mac Main Param 
*
* @details
*
*     Function:msmSetMacMainConfig
*
*         Processing steps:
*
* @param[in]
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetMacMainConfig(MsmMacMainConifg *macMainConfig)
{ 
   if(macMainConfig == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.macMainConfig = *macMainConfig;
   RETVALUE(ROK);
}
 
/** @brief This function is used for setting ltecellSchdPwrCfg Param 
*
* @details
*
*     Function:msmSetSchdPwrCfgParameters
*
*         Processing steps:
*
* @param[in]
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetSchdPwrCfgParameters(MsmCellSchdPwrCfg *ltecellSchdPwrCfg)
{ 
   if(ltecellSchdPwrCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.ltecellSchdPwrCfg = *ltecellSchdPwrCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for setting Rsys TPM Param 
*
* @details
*
*     Function:msmSetRsysTpmCfgParameters
*
*         Processing steps:
*
* @param[in]   MsmRsysTpmCfg tpmCfg
* @return      S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmSetRsysTpmCfgParameters(MsmRsysTpmCfg *tpmCfg)
{ 
   if(tpmCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteeNodeBparams.tpmCfg = *tpmCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for getting generic parameters
*
* @details
*
*     Function: MsmLtePrachCfgCommon 
*
*         Processing steps:
*
* @param[in] ltePrachCfgCommon 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetPrachConfigCommonParameters(MsmLtePrachCfgCommon *ltePrachCfgCommon)
{
   if(ltePrachCfgCommon == NULLP)
   {
      RETVALUE(RFAILED);
   }
   *ltePrachCfgCommon = lteeNodeBparams.ltePrachCfgCommon;
   RETVALUE(ROK);
}

/** @brief This function is used for getting RIM parameters
 * *
 * * @details
 * *
 * *     Function: msmGetRimParameters
 * *
 * *         Processing steps:
 * *
 * * @param[in] lteRimParams
 * * @return S16
 * *        -# Success : ROK
 * *        -# Failure : RFAILED
 * */
PUBLIC S16 msmGetRimParameters(MsmLteRimParams* lteRimParams)
{
   if(lteRimParams == NULLP)
   {
      RETVALUE(RFAILED);
   }
   *lteRimParams = lteeNodeBparams.lteRimParams;
   RETVALUE(ROK);
}
/** @brief This function is used for setting generic parameters
*
* @details
*
*     Function: msmGetEnodebIpParameters
*
*         Processing steps:
*
* @param[in]lteeNodeBIPparams 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetEnodebIpParameters(MsmLteeNodeBIPparams *lteeNodeBIPparams)
{
   if(lteeNodeBIPparams == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteeNodeBIPparams = lteeNodeBparams.lteeNodeBIPparams;
   RETVALUE(ROK);
}
/** @brief This function is used for getting generic parameters
*
* @details
*
*     Function: msmGetRachConfigCommonParameters
*
*         Processing steps:
*
* @param[in] lteRachCfgCommonParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetRachConfigCommonParameters(MsmLteRachCfgCommonParams *lteRachCfgCommonParams)
{
   if(lteRachCfgCommonParams == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteRachCfgCommonParams = lteeNodeBparams.lteRachCfgCommonParams;
   RETVALUE(ROK);
}
/** @brief This function is used for getting generic parameters
*
* @details
*
*     Function:msmGetPdschConfigCommonParameters 
*
*         Processing steps:
*
* @param[in]ltePdschCfgCommon
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetPdschConfigCommonParameters(MsmLtePdschCfgCommon *ltePdschCfgCommon)
{
   if(ltePdschCfgCommon == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *ltePdschCfgCommon = lteeNodeBparams.ltePdschCfgCommon;
   RETVALUE(ROK);
}
/** @brief This function is used for getting generic parameters
*
* @details
*
*     Function:msmGetUlPowerControlCommonParameters
*
*         Processing steps:
*
* @param[in]lteULPwrCtrlCommon
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetUlPowerControlCommonParameters(MsmLteULPwrCtrlCommon *lteULPwrCtrlCommon)
{
   if(lteULPwrCtrlCommon == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteULPwrCtrlCommon = lteeNodeBparams.lteULPwrCtrlCommon;
   RETVALUE(ROK);
}
/** @brief This function is used for etting generic parameters
*
* @details
*
*     Function:msmGetUlFrequencyInfoParameters
*
*         Processing steps:
*
* @param[in]lteULFrequencyInfo
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetUlFrequencyInfoParameters(MsmLteULFrequencyInfo *lteULFrequencyInfo)
{
   if(lteULFrequencyInfo == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteULFrequencyInfo = lteeNodeBparams.lteULFrequencyInfo;
   RETVALUE(ROK);
}
/** @brief This function is used for etting generic parameters
*
* @details
*
*     Function:msmGetCell_Sib3ConfigGroupParameters
*
*         Processing steps:
*
* @param[in]lteCellSib3CfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetCell_Sib3ConfigGroupParameters(MsmLteCellSib3CfgGrp *lteCellSib3CfgGrp)
{
   if(lteCellSib3CfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteCellSib3CfgGrp = lteeNodeBparams.lteCellSib3CfgGrp;
   RETVALUE(ROK);
}
/** @brief This function is used for getting lteCellSib1CfgGrp
*
* @details
*
*     Function:msmGetCellSib1ConfigGroupParameters:
*
*         Processing steps:
*
* @param[in]lteCellSib1CfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetCellSib1ConfigGroupParameters(MsmLteCellSib1CfgGrp *lteCellSib1CfgGrp)
{
   if(lteCellSib1CfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteCellSib1CfgGrp = lteeNodeBparams.lteCellSib1CfgGrp;
   RETVALUE(ROK);
}
/** @brief This function is used for getting lteDrxCfgParams 
*
* @details
*
*     Function:msmGetDrxConfigParameters
*
*         Processing steps:
*
* @param[in] lteDrxCfgParam
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetDrxConfigParameters(MsmLteDrxCfgParams *lteDrxCfgParams)
{
   if(lteDrxCfgParams == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteDrxCfgParams = lteeNodeBparams.lteDrxCfgParams;
   RETVALUE(ROK);
}
/** @brief This function is used for getting lteCellMeasCfgGrp  
*
* @details
*
*     Function: msmGetCellMeasConfigParameters 
*
*         Processing steps:
*
* @param[in]lteCellMeasCfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetCellMeasConfigParameters(MsmLteCellMeasCfgGrp *lteCellMeasCfgGrp)
{
   if(lteCellMeasCfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteCellMeasCfgGrp = lteeNodeBparams.lteCellMeasCfgGrp;
   RETVALUE(ROK);
}
/** @brief This function is used for getting lteAddCellCfg 
*
* @details
*
*     Function:msmGetAddCellConfigParameters
*
*         Processing steps:
*
* @param[in]lteAddCellCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetAddCellConfigParameters(MsmLteAddCellCfg *lteAddCellCfg)
{
   if(lteAddCellCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteAddCellCfg = lteeNodeBparams.lteAddCellCfg;
   RETVALUE(ROK);
}
/** @brief This function is used for getting generic parameters
*
* @details
*
*     Function:msmGetSmCellConfigParameters 
*
*         Processing steps:
*
* @param[in] lteSmCellCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetSmCellConfigParameters(MsmLteSmCellCfg *lteSmCellCfg)
{
   if(lteSmCellCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteSmCellCfg = lteeNodeBparams.lteSmCellCfg;
   RETVALUE(ROK);
}
/** @brief This function is used for getting lteCellMibCfgGrp 
*
* @details
*
*     Function:msmGetCellMibConfigParameters 
*
*         Processing steps:
*
* @param[in]lteCellMibCfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetCellMibConfigParameters(MsmLteCellMibCfgGrp *lteCellMibCfgGrp)
{
   if(lteCellMibCfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteCellMibCfgGrp = lteeNodeBparams.lteCellMibCfgGrp;
   RETVALUE(ROK);
}
/** @brief This function is used for getting lteCellMibCfgGrp 
*
* @details
*
*     Function:msmGetMacSchConfigParameters 
*
*         Processing steps:
*
* @param[in]lteMAC_SCH_Config 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetMacSchConfigParameters(MsmLteMAC_SCH_Config *lteMAC_SCH_Config)
{
   if(lteMAC_SCH_Config == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteMAC_SCH_Config = lteeNodeBparams.lteMAC_SCH_Config;
   RETVALUE(ROK);
}
/** @brief This function is used for getting lteCellMibCfgGrp 
*
* @details
*
*     Function:msmGetAntennaCommonConfigParameters 
*
*         Processing steps:
*
* @param[in]lteAntenna_Common_Config 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetAntennaCommonConfigParameters(MsmLteAntenna_Common_Config *lteAntenna_Common_Config)
{
   if(lteAntenna_Common_Config == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteAntenna_Common_Config = lteeNodeBparams.lteAntenna_Common_Config;
   RETVALUE(ROK);
}
/** @brief This function is used for getting  lteGenericParaCfgDedicateds
*
* @details
*
*     Function:msmGetPdschConfigDedicatedParameters 
*
*         Processing steps:
*
* @param[in]lteGenericParaCfgDedicateds 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetPdschConfigDedicatedParameters(MsmLtePdschCfgDedicated *ltePdschCfgDedicated)
{
   if(ltePdschCfgDedicated == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *ltePdschCfgDedicated = lteeNodeBparams.ltePdschCfgDedicated;
   RETVALUE(ROK);
}
/** @brief This function is used for getting ltePuschBasicCfgCommon 
*
* @details
*
*     Function:msmGetPuschBasicCfgCommonParameters 
*
*         Processing steps:
*
* @param[in] ltePuschBasicCfgCommon
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetPuschBasicCfgCommonParameters(MsmLtePuschBasicCfgCommon *ltePuschBasicCfgCommon)
{
   if(ltePuschBasicCfgCommon == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *ltePuschBasicCfgCommon = lteeNodeBparams.ltePuschBasicCfgCommon;
   RETVALUE(ROK);
}
/** @brief This function is used for getting ltePuschRefSignalCfg 
*
* @details
*
*     Function:msmGetPuschRefSignalCfgParameters 
*
*         Processing steps:
*
* @param[in] ltePuschRefSignalCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetPuschRefSignalCfgParameters(MsmLtePuschRefSignalCfg *ltePuschRefSignalCfg)
{
   if(ltePuschRefSignalCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *ltePuschRefSignalCfg = lteeNodeBparams.ltePuschRefSignalCfg;
   RETVALUE(ROK);
}
/** @brief This function is used for getting ltePucchCfgCommon 
*
* @details
*
*     Function:msmGetPucchCfgCommonParameters 
*
*         Processing steps:
*
* @param[in]ltePucchCfgCommon 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetPucchCfgCommonParameters(MsmLtePucchCfgCommon *ltePucchCfgCommon)
{
	if(ltePucchCfgCommon == NULLP)
   {
      RETVALUE(RFAILED);
   }

*ltePucchCfgCommon = lteeNodeBparams.ltePucchCfgCommon;
	RETVALUE(ROK);
}
/** @brief This function is used for getting lteSRSCfgCommon 
*
* @details
*
*     Function:msmGetSrsCfgCommonParameters 
*
*         Processing steps:
*
* @param[in]lteSRSCfgCommon 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetSrsCfgCommonParameters(MsmLteSRSCfgCommon *lteSRSCfgCommon)
{
	if(lteSRSCfgCommon == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteSRSCfgCommon = lteeNodeBparams.lteSRSCfgCommon;
	RETVALUE(ROK);
}
/** @brief This function is used for getting lteRabPolicyCfgGrp 
*
* @details
*
*     Function:msmGetRabPolicyCfgGrpParameters 
*
*         Processing steps:
*
* @param[in] lteRabPolicyCfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetRabPolicyCfgGrpSrb1Parameters(MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp)
{  
   if(lteRabPolicyCfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

lteRabPolicyCfgGrp->actType = lteeNodeBparams.lteRabPolicyCfgGrp.actType;
   lteRabPolicyCfgGrp->srbPolicy[MSM_SRB1] = lteeNodeBparams.lteRabPolicyCfgGrp.srbPolicy[MSM_SRB1]; 
	RETVALUE(ROK);
}
/** @brief This function is used for getting lteRabPolicyCfgGrp 
*
* @details
*
*     Function:msmGetRabPolicyCfgGrpParameters 
*
*         Processing steps:
*
* @param[in] lteRabPolicyCfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetRabPolicyCfgGrpSrb2Parameters(MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp)
{
   if(lteRabPolicyCfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

lteRabPolicyCfgGrp->actType = lteeNodeBparams.lteRabPolicyCfgGrp.actType;
   lteRabPolicyCfgGrp->srbPolicy[MSM_SRB2] = lteeNodeBparams.lteRabPolicyCfgGrp.srbPolicy[MSM_SRB2]; 
	RETVALUE(ROK);
}
/** @brief This function is used for getting lteRabPolicyCfgGrp 
*
* @details
*
*     Function:msmGetRabPolicyCfgGrpParameters 
*
*         Processing steps:
*
* @param[in] lteRabPolicyCfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetRabPolicyCfgGrpDrbParameters(MsmLteRabPolicyCfgGrp *lteRabPolicyCfgGrp)
{
   if(lteRabPolicyCfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

U32 idx;
   lteRabPolicyCfgGrp->isDscpEnable = lteeNodeBparams.lteRabPolicyCfgGrp.isDscpEnable;
   for(idx = 0; idx < MSM_MAX_QCI; idx++)
   {   
      lteRabPolicyCfgGrp->drbPolicy[idx] = lteeNodeBparams.lteRabPolicyCfgGrp.drbPolicy[idx];
   } 
   RETVALUE(ROK);
}

/** @brief This function is used for getting Ue Timer Constants
*
* @details
*
*     Function:msmGetUeTimerConstantsParameters 
*
*         Processing steps:
*
* @param[in]lteUeTimerConstants 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetUeTimerConstantsParameters(MsmLteUeTimerConstants *lteUeTimerConstants)
{
	if(lteUeTimerConstants == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteUeTimerConstants = lteeNodeBparams.lteUeTimerConstants;
	RETVALUE(ROK);
}
/** @brief This function is used for getting Cell EAID Cfg
*
* @details
*
*     Function:msmGetCellEaidCfgGrpParameters 
*
*         Processing steps:
*
* @param[in] lteCellEAIDCfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetCellEaidCfgGrpParameters(MsmLteCellEAIDCfgGrp *lteCellEAIDCfgGrp)
{
		if(lteCellEAIDCfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }
*lteCellEAIDCfgGrp = lteeNodeBparams.lteCellEAIDCfgGrp;
	RETVALUE(ROK);
}
/** @brief This function is used for getting Cell Sib9 Cfg
*
* @details
*
*     Function:msmGetCellSib9CfgGrpParameters 
*
*         Processing steps:
*
* @param[in] lteCellSib9CfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetCellSib9CfgGrpParameters(MsmLteCellSib9CfgGrp *lteCellSib9CfgGrp)
{
	if(lteCellSib9CfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteCellSib9CfgGrp = lteeNodeBparams.lteCellSib9CfgGrp;
	RETVALUE(ROK);
}
/** @brief This function is used for getting CQI Periodic Report 
*
* @details
*
*     Function:msmGetCqiPeriodicReportCfgParameters 
*
*         Processing steps:
*
* @param[in]lteCQIPeriodicReportCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetCqiPeriodicReportCfgParameters(MsmLteCQIPeriodicReportCfg *lteCQIPeriodicReportCfg)
{
	if(lteCQIPeriodicReportCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteCQIPeriodicReportCfg = lteeNodeBparams.lteCQIPeriodicReportCfg;
	RETVALUE(ROK);
}
/** @brief This function is used for getting Neigh Freq Cfg 
*
* @details
*
*     Function:msmGetNeighFreqCfgParameters 
*
*         Processing steps:
*
* @param[in] lteNeighFreqCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetNeighFreqEutraCfgParameters(MsmLteNeighFreqCfg *lteNeighFreqCfg)
{
   U16 idx;
   if(lteNeighFreqCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   lteNeighFreqCfg->actType = lteeNodeBparams.lteNeighFreqCfg.actType; 
   lteNeighFreqCfg->numEutraFreq = lteeNodeBparams.lteNeighFreqCfg.numEutraFreq; 
   for(idx=0;idx< lteNeighFreqCfg->numEutraFreq;idx++)
   {
      lteNeighFreqCfg->t.addNeighFreq[idx].eutranFreq=lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].eutranFreq;
   }

   RETVALUE(ROK);
}
 
/** @brief This function is used for getting Neigh Freq Cfg 
*
* @details
*
*     Function:msmGetTdsCdmaUtranTddFreqParameters
*
*         Processing steps:
*
* @param[in] lteNeighFreqCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetTdsCdmaUtranTddFreqParameters(MsmLteNeighFreqCfg *lteNeighFreqCfg)
{  
   U16 idx;
   if(lteNeighFreqCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   lteNeighFreqCfg->actType = lteeNodeBparams.lteNeighFreqCfg.actType; 
   lteNeighFreqCfg->numUtraTddFreq = lteeNodeBparams.lteNeighFreqCfg.numUtraTddFreq; 
   lteNeighFreqCfg->maxUtraTddFreqEntries = lteeNodeBparams.lteNeighFreqCfg.maxUtraTddFreqEntries; 
   for(idx=0;idx < lteNeighFreqCfg->numUtraTddFreq;idx++)
   {
      lteNeighFreqCfg->t.addNeighFreq[idx].utranTddFreq =lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].utranTddFreq;
   }

   RETVALUE(ROK);
}

/** @brief This function is used for getting Neigh Freq Cfg 
*
* @details
*
*     Function:msmGetNeighFreqCfgParameters 
*
*         Processing steps:
*
* @param[in] lteNeighFreqCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetNeighFreqUtraCfgParameters(MsmLteNeighFreqCfg *lteNeighFreqCfg)
{  
   U16 idx;
   if(lteNeighFreqCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   lteNeighFreqCfg->actType = lteeNodeBparams.lteNeighFreqCfg.actType; 
   lteNeighFreqCfg->numUtraFreq = lteeNodeBparams.lteNeighFreqCfg.numUtraFreq; 
   lteNeighFreqCfg->maxUtraFreqEntries = lteeNodeBparams.lteNeighFreqCfg.maxUtraFreqEntries; 
   for(idx=0;idx < lteNeighFreqCfg->numUtraFreq;idx++)
   {
      lteNeighFreqCfg->t.addNeighFreq[idx].utranFreq =lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].utranFreq; 
   }


   RETVALUE(ROK);
}
 
/** @brief This function is used for getting Neigh Cell 
*
* @details
*
*     Function: msmGetTdsCdmaUtranTddCellParameters
*
*         Processing steps:
*
* @param[in] lteNeighCellCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetTdsCdmaUtranTddCellParameters(MsmNeighCellCfg *lteNeighCellCfg)
{
   U16 idx;
   if(lteNeighCellCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   lteNeighCellCfg->actType =lteeNodeBparams.lteNeighCellCfg.actType; 
   lteNeighCellCfg->maxTddCellEntries = lteeNodeBparams.lteNeighCellCfg.maxTddCellEntries; 
   lteNeighCellCfg->numUtraTddCells = lteeNodeBparams.lteNeighCellCfg.numUtraTddCells; 
   for(idx=0;idx< lteNeighCellCfg->numUtraTddCells && lteeNodeBparams.lteNeighCellCfg.numUtraTddCells <=MSM_MAX_NUM_NEIGH_CELLS;idx++)
   {  
      lteNeighCellCfg->t.neighAddCfg[idx].utranTddCell = lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].utranTddCell; 
   } 
	RETVALUE(ROK);
}

/** @brief This function is used for getting Neigh Cell 
*
* @details
*
*     Function: msmGetNeighborListInUseParameters
*
*         Processing steps:
*
* @param[in] lteNeighCellCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetNeighborListInUseUtraParameters(MsmNeighCellCfg *lteNeighCellCfg)
{
   U16 idx;
   if(lteNeighCellCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   lteNeighCellCfg->actType =lteeNodeBparams.lteNeighCellCfg.actType; 
   lteNeighCellCfg->maxCellEntries = lteeNodeBparams.lteNeighCellCfg.maxCellEntries; 
   lteNeighCellCfg->numUtraCells = lteeNodeBparams.lteNeighCellCfg.numUtraCells; 
   for(idx=0;idx< lteNeighCellCfg->numUtraCells && lteeNodeBparams.lteNeighCellCfg.numUtraCells <=MSM_MAX_NUM_NEIGH_CELLS;idx++)
   {  
      lteNeighCellCfg->t.neighAddCfg[idx].utranCell = lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].utranCell; 
   } 
	RETVALUE(ROK);
}

/** @brief This function is used for getting Neigh Cell 
*
* @details
*
*     Function: msmGetNeighborListInUseEutraParameters
*
*         Processing steps:
*
* @param[in] lteNeighCellCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetNeighborListInUseEutraParameters(MsmNeighCellCfg *lteNeighCellCfg)
{
   U16 idx;
   if(lteNeighCellCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   lteNeighCellCfg->actType =lteeNodeBparams.lteNeighCellCfg.actType; 
   lteNeighCellCfg->maxCellEntries = lteeNodeBparams.lteNeighCellCfg.maxCellEntries; 
   lteNeighCellCfg->numEutraCells = lteeNodeBparams.lteNeighCellCfg.numEutraCells; 
   for(idx=0;idx< lteNeighCellCfg->numEutraCells && lteeNodeBparams.lteNeighCellCfg.numEutraCells <=MSM_MAX_NUM_NEIGH_CELLS;idx++)
   {  
      lteNeighCellCfg->t.neighAddCfg[idx].eutraCell= lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].eutraCell; 
   }  
   RETVALUE(ROK);

}

/** @brief This function is used for getting Sib5 InterFreq CarrierInfo 
*
* @details
*
*     Function: msmGetSib5InterFreqCarrierInfoParameters
*
*         Processing steps:
*
* @param[in] lteSib5InterFreqCarrierInfo
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetSib5InterFreqCarrierInfoParameters(MsmLteSib5InterFreqCarrierInfo *lteSib5InterFreqCarrierInfo)
{
	if(lteSib5InterFreqCarrierInfo == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteSib5InterFreqCarrierInfo = lteeNodeBparams.lteSib5InterFreqCarrierInfo;
	RETVALUE(ROK);
}

/** @brief This function is used for getting Cell Sib6 Cfg 
*
* @details
*
*     Function: msmGetCellSib6ConfigGroupParameters
*
*         Processing steps:
*
* @param[in] lteCellSib6CfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetCellSib6ConfigGroupParameters(MsmLteCellSib6CfgGrp *lteCellSib6CfgGrp)
{
	if(lteCellSib6CfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteCellSib6CfgGrp = lteeNodeBparams.lteCellSib6CfgGrp;
	RETVALUE(ROK);
}
/** @brief This function is used for getting Enb Proto Cfg 
*
* @details
*
*     Function: msmGetEnbProtoCfgParameters
*
*         Processing steps:
*
* @param[in] lteEnbProtoCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetEnbProtoCfgParameters(MsmLteEnbProtoCfg *lteEnbProtoCfg)
{
	if(lteEnbProtoCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteEnbProtoCfg = lteeNodeBparams.lteEnbProtoCfg;
	RETVALUE(ROK);
}

/** @brief This function is used for getting Ac BarringInfo 
*
* @details
*
*     Function: msmGetAcBarringInfoParameters
*
*         Processing steps:
*
* @param[in] lteAcBarringInfo
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetAcBarringInfoParameters(MsmLteAcBarringInfo *lteAcBarringInfo)
{
	if(lteAcBarringInfo == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteAcBarringInfo = lteeNodeBparams.lteAcBarringInfo;
	RETVALUE(ROK);
}

/** @brief This function is used for getting EnodebIpAddr 
*
* @details
*
*     Function: msmGetEnodebIpAddr 
*
*         Processing steps:
*
* @param[in]lteEnodebIpAddr
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetEnodebIpAddr(MsmEnodebIpAddr *lteEnodebIpAddr)
{
	if(lteEnodebIpAddr == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteEnodebIpAddr = lteeNodeBparams.lteEnodebIpAddr; 
	RETVALUE(ROK);
}


/** @brief This function is used for getting EARFCNDL and RSSI scanned by REM
*
* @details
*
*     Function:msmGetEarfcnRssiParameters
*
*         Processing steps:
*
* @param[in]lteEarfcnRssiCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetEarfcnRssiParameters(MsmLteEarfcnRssiCfg *lteEarfcnRssiCfg)
{
   if(lteEarfcnRssiCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteEarfcnRssiCfg = lteeNodeBparams.lteEarfcnRssiCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for getting Self Config Params
*
* @details
*
*     Function:msmGetSelfConfigParameters
*
*         Processing steps:
*
* @param[in] lteSelfConfigParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetSelfConfigParameters(MsmLteSonGenericParams *lteSelfConfigParams)
{
        if(lteSelfConfigParams == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteSelfConfigParams = lteeNodeBparams.lteSonGenericParams;
        RETVALUE(ROK);
}


/** @brief This function is used for getting Power Off set
*
* @details
*
*     Function: msmGetPowerOffsetParameters
*
*         Processing steps:
*
* @param[in] ltePowerOffsetParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetPowerOffsetParameters(MsmLtePowerOffsetParams *ltePowerOffsetParams)
{
	if(ltePowerOffsetParams == NULLP)
   {
      RETVALUE(RFAILED);
   }

*ltePowerOffsetParams = lteeNodeBparams.ltePowerOffsetParams;
	RETVALUE(ROK);
}

/** @brief This function is used for getting lteFapControlParams 
*
* @details
*
*     Function: msmGetFapControlParameters 
*
*         Processing steps:
*
* @param[in] lteFapControlParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetFapControlParameters(MsmLteFapControlParams *lteFapControlParams)
{
	if(lteFapControlParams == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteFapControlParams = lteeNodeBparams.lteFapControlParams;
	RETVALUE(ROK);
}

/** @brief This function is used for getting PRB allocation 
*
* @details
*
*     Function:msmGetPrbAllocationParameters 
*
*         Processing steps:
*
* @param[in] ltePRBallocation
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetPrbAllocationParameters(MsmLtePRBallocation *ltePRBallocation)
{
	if(ltePRBallocation == NULLP)
   {
      RETVALUE(RFAILED);
   }

*ltePRBallocation = lteeNodeBparams.ltePRBallocation;
	RETVALUE(ROK);
}

/** @brief This function is used for getting Sib4 CsgI 
*
* @details
*
*     Function: msmGetSib4CsgInfoParameters
*
*         Processing steps:
*
* @param[in] lteSib4CsgInfo
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetSib4CsgInfoParameters(MsmLteSib4CsgInfo *lteSib4CsgInfo)
{
	if(lteSib4CsgInfo == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteSib4CsgInfo = lteeNodeBparams.lteSib4CsgInfo;
	RETVALUE(ROK);
}

/** @brief This function is used for getting Rrm Tdd Param
*
* @details
*
*     Function:msmGetRrmTddParameters 
*
*         Processing steps:
*
* @param[in] lteGenericParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetRrmTddParameters(MsmLteRrmTddParam *lteRrmTddParam)
{
	if(lteRrmTddParam == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteRrmTddParam = lteeNodeBparams.lteRrmTddParam;
	RETVALUE(ROK);
}

/** @brief This function is used for getting X2 Config Params
*
* @details
*
*     Function: msmGetX2ConfigParameters
*
*         Processing steps:
*
* @param[in] lteGenericParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetX2ConfigParameters(MsmLteX2ConfigParams *lteX2ConfigParams)
{
	if(lteX2ConfigParams == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteX2ConfigParams = lteeNodeBparams.lteX2ConfigParams;
	RETVALUE(ROK);
}

/** @brief This function is used for Sctp Cfg Params
*
* @details
*
*     Function: msmGetSctpConfigParameters 
*
*         Processing steps:
*
* @param[in] lteSctpCfgParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetSctpConfigParameters(MsmLteSctpCfgParams *lteSctpCfgParams)
{
	if(lteSctpCfgParams == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteSctpCfgParams = lteeNodeBparams.lteSctpCfgParams;
	RETVALUE(ROK);
}

/** @brief This function is used for getting PCCH Cfg 
*
* @details
*
*     Function:msmGetPcchConfigCommonParameters 
*
*         Processing steps:
*
* @param[in] ltePCCHCfgCommon
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetPcchConfigCommonParameters(MsmLtePCCHCfgCommon *ltePCCHCfgCommon)
{
	if(ltePCCHCfgCommon == NULLP)
   {
      RETVALUE(RFAILED);
   }

*ltePCCHCfgCommon = lteeNodeBparams.ltePCCHCfgCommon;
	RETVALUE(ROK);
}

/** @brief This function is used for getting Cell Ul Schd Cfg 
*
* @details
*
*     Function:lteCellUlSchdCfgGrp 
*
*         Processing steps:
*
* @param[in] lteGenericParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetCellUlSchdConfigParameters(MsmLteCellUlSchdCfgGrp *lteCellUlSchdCfgGrp)
{
	if(lteCellUlSchdCfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteCellUlSchdCfgGrp = lteeNodeBparams.lteCellUlSchdCfgGrp;
	RETVALUE(ROK);
}

/** @brief This function is used for getting Cell DlSchd Cfg 
*
* @details
*
*     Function:msmGetCellDlSchdConfigParameters 
*
*         Processing steps:
*
* @param[in] lteCellDlSchdCfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetCellDlSchdConfigParameters(MsmLteCellDlSchdCfgGrp *lteCellDlSchdCfgGrp)
{
	if(lteCellDlSchdCfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteCellDlSchdCfgGrp = lteeNodeBparams.lteCellDlSchdCfgGrp;
	RETVALUE(ROK);
}

/** @brief This function is used for getting Ms Cell Cfg
*
* @details
*
*     Function:msmGetMsCellCfgReqParameters 
*
*         Processing steps:
*
* @param[in] MsCellCfgReq 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetMsCellCfgReqParameters(MsmLteMsCellCfgReq *lteMsCellCfgReq)
{
	if(lteMsCellCfgReq == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteMsCellCfgReq = lteeNodeBparams.lteMsCellCfgReq;
	RETVALUE(ROK);
}

/** @brief This function is used for getting lteDbgParams 
*
* @details
*
*     Function: msmGetDbgParameters 
*
*         Processing steps:
*
* @param[in] lteDbgParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetDbgParameters(MsmLteDbgParams *lteDbgParams)
{
	if(lteDbgParams == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteDbgParams = lteeNodeBparams.lteDbgParams;
	RETVALUE(ROK);
}

/** @brief This function is used for getting lteUlAllocInfoCbParams 
*
* @details
*
*     Function: msmGetUlAllocInfoCbParameters
*
*         Processing steps:
*
* @param[in] lteGenericParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetUlAllocInfoCbParameters(MsmLteUlAllocInfoCbParams *lteUlAllocInfoCbParams)
{
	if(lteUlAllocInfoCbParams == NULLP)
   {
      RETVALUE(RFAILED);
   }

*lteUlAllocInfoCbParams = lteeNodeBparams.lteUlAllocInfoCbParams;
	RETVALUE(ROK);
}

/** @brief This function is used for getting Proc Timer Cfg 
*
* @details
*
*     Function: msmGetProcTimerCfgParameters 
*
*         Processing steps:
*
* @param[in] lteGenericParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetProcTimerCfgParameters(MsmLteProcTimerCfg *lteProcTimerCfg)
{
	if(lteProcTimerCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteProcTimerCfg = lteeNodeBparams.lteProcTimerCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for getting Proc Timer Cfg
*
* @details
*
*     Function: msmGetFactoryFapAccessParameters
*
*         Processing steps:
*
* @param[in] lteFapAccessParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetFactoryFapAccessParameters(MsmLteFapAccessParams *lteFapAccessParams)
{
   if(lteFapAccessParams == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteFapAccessParams = lteeNodeBparams.lteFapAccessParams;
   RETVALUE(ROK);
}

/** @brief This function is used for getting Proc Timer Cfg
*
* @details
*
*     Function: msmGetFactorySmCellParameters
*
*         Processing steps:
*
* @param[in] lteSmCellCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetFactorySmCellParameters(MsmLteSmCellCfg *lteSmCellCfg)
{
   if(lteSmCellCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteSmCellCfg = lteeNodeBparams.lteSmCellCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for getting perf Mgmt Parameters parameters
*
* @details
*
*     Function: msmGetPmParameters
*
*         Processing steps:
*
* @param[in] lteGenericParams
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*rakesh*/

PUBLIC S16 msmGetPmParameters(MsmPerfMgmtParameters *perfMgmtParameters)
{
   if(perfMgmtParameters == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *perfMgmtParameters = wcdmaNodeBparams.perfMgmtParameters;
   RETVALUE(ROK);
}

/** @brief This function is used for getting frequent Fault Mgmt Parameters parameters
*
* @details
*
*     Function:msmGetFrequentFaultParameters 
*
*         Processing steps:
*
* @param[in] frequentFaultMgmtParameters
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*rakesh*/

PUBLIC S16 msmGetFrequentFaultParameters(MsmFrequentFaultMgmtParameters *frequentFaultMgmtParameters)
{
   if(frequentFaultMgmtParameters == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *frequentFaultMgmtParameters = faultMgmtParams.frequentFaultMgmtParameters;
   RETVALUE(ROK);
}

/** @brief This function is used for getting cdma Band Class 
*
* @details
*
*     Function:msmGetCdmaBandClassParameters 
*
*         Processing steps:
*
* @param[in] cdmaBandClass
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
**/

PUBLIC S16 msmGetCdmaBandClassParameters(MsmCdmaBandClass *cdmaBandClassParams)
{
   if(cdmaBandClassParams == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *cdmaBandClassParams = lteeNodeBparams.cdmaBandClassParams;
   RETVALUE(ROK);
}

/** @brief This function is used for getting cdma Mobility 
*
* @details
*
*     Function:msmGetCdmaMobilityParameters 
*
*         Processing steps:
*
* @param[in] cdmaMobilityParam
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetCdmaMobilityParameters(MsmCdmaMobilityParam *cdmaMobilityParam)
{
   if(cdmaMobilityParam == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *cdmaMobilityParam = lteeNodeBparams.cdmaMobilityParam;
   RETVALUE(ROK);
}

/** @brief This function is used for 
*
* @details
*
*     Function:msmGetBarringCfg1xRtt 
*
*         Processing steps:
*
* @param[in] barringConfig1xRTTCfg
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetBarringCfg1xRtt(MsmBarringConfig1xRTTCfg *barringConfig1xRTTCfg)
{
   if(barringConfig1xRTTCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *barringConfig1xRTTCfg = lteeNodeBparams.barringConfig1xRTTCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for getting neigh cell cfg 
*
* @details
*
*     Function: msmGetCdma1XRttNghCell
*
*         Processing steps:
*
* @param[in]lteNeighCellCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetCdma1XRttNghCell(MsmNeighCellCfg *lteNeighCellCfg)
{
   U32 idx = 0;
   if(lteNeighCellCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lteNeighCellCfg->numCdma1xCells = lteeNodeBparams.lteNeighCellCfg.numCdma1xCells;
   for(idx = 0; idx < lteNeighCellCfg->numCdma1xCells; idx++)
   {
      lteNeighCellCfg->t.neighAddCfg[idx].cdma1xCell = lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].cdma1xCell;
   }

   RETVALUE(ROK);
}

/** @brief This function is used for getting cell Sib8 Cfg
*
* @details
*
*     Function:msmGetSib8Parameters 
*
*         Processing steps:
*
* @param[in] cellSib8CfgGrp
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetSib8Parameters(MsmCellSib8CfgGrp *cellSib8CfgGrp)
{
   if(cellSib8CfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *cellSib8CfgGrp = lteeNodeBparams.cellSib8CfgGrp;
   RETVALUE(ROK);
}

/** @brief This function is used for getting csfb Cfg Grp
*
* @details
*
*     Function:msmGetCsfbCfgParameters 
*
*         Processing steps:
*
* @param[in]csfbCfgGrp 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetCsfbCfgParameters(MsmCsfbCfgGrp *csfbCfgGrp)
{
   if(csfbCfgGrp == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *csfbCfgGrp = lteeNodeBparams.csfbCfgGrp;
   RETVALUE(ROK);
}

/** @brief This function is used for getting Sps Cell Cfg 
*
* @details
*
*     Function:msmGetSpsParameters 
*
*         Processing steps:
*
* @param[in]stSpsCellCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetSpsParameters(MsmSpsCellConfigData *stSpsCellCfg)
{
   if(stSpsCellCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *stSpsCellCfg = lteeNodeBparams.stSpsCellCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for getting eNB Log Level Param 
*
* @details
*
*     Function:msmGetLogLvlParameter 
*
*         Processing steps:
*
* @param[in]lteLogLvlCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetLogLvlParameter(MsmEnbLogLvlData *lteLogLvlCfg)
{
   if(lteLogLvlCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteLogLvlCfg = lteeNodeBparams.lteLogLvlCfg;
   RETVALUE(ROK);
}

/* TM4 changes*/
/** @brief This function is used for getting Transmission Mode Cfg 
*
* @details
*
*     Function:msmGetTMParameters 
*
*         Processing steps:
*
* @param[in]lteTransModeCellCfg 
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/

PUBLIC S16 msmGetTMParameters (MsmTMCellConfig *lteTransModeCellCfg)
{
   if(lteTransModeCellCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteTransModeCellCfg = lteeNodeBparams.lteTransModeCellCfg;
   RETVALUE(ROK);
}
/** @brief This function is used for getting Tdd parameters 
*
* @details
*
*     Function: msmGetTddParameters 
*
*         Processing steps:
*
* @param[in]lteTddParam
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/ 

PUBLIC S16 msmGetTddParameters(MsmLteTddParam *lteTddParam)
{
   if(lteTddParam== NULLP)
   {
      RETVALUE(RFAILED);
   }

   *lteTddParam = lteeNodeBparams.lteTddParam;
   RETVALUE(ROK);
}

/** @brief This function is used for getting ltecellSchdPwrCfg Param 
*
* @details
*
*     Function:msmGetSchdPwrCfgParameters
*
*         Processing steps:
*
* @param[in]
* @return S16
*        -# Success : ROK
*        -# Failure : RFAILED
*/
PUBLIC S16 msmGetSchdPwrCfgParameters(MsmCellSchdPwrCfg *ltecellSchdPwrCfg)
{ 
   if(ltecellSchdPwrCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   *ltecellSchdPwrCfg = lteeNodeBparams.ltecellSchdPwrCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for getting Sib7Cfg Param 
 *
 * @details
 *
 *     Function:msmGetsib7CfgGrpParameters
 *
 *         Processing steps:
 *
 * @param[in]sib7CfgGrp
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */

PUBLIC S16 msmGetsib7CfgGrpParameters(MsmLteCellSib7CfgGrp *sib7CfgGrp)
{ 
   if(sib7CfgGrp == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   *sib7CfgGrp = lteeNodeBparams.sib7CfgGrp;
   RETVALUE(ROK);
}


/** @brief This function is used for getting csfbGranCfg Param 
 *
 * @details
 *
 *     Function:msmGetcsfbGeranCfgParameters
 *
 *         Processing steps:
 *
 * @param[in]csfbToGeranCfg
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */

PUBLIC S16 msmGetcsfbGeranCfgParameters(MsmLteCsfbGeranCfg *csfbToGeranCfg)
{ 
   if(csfbToGeranCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   *csfbToGeranCfg = lteeNodeBparams.csfbToGeranCfg;
   RETVALUE(ROK);
}

/** @brief This function is used for getting NeighFreqCfgGeran Param 
 *
 * @details
 *
 *     Function:msmGetNeighFreqCfgGeranParameters
 *
 *         Processing steps:
 *
 * @param[in]lteNeighFreqCfg
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */

PUBLIC S16 msmGetNeighFreqCfgGeranParameters(MsmLteNeighFreqCfg *lteNeighFreqCfg)
{ 
   U16 idx = 0;

   if(lteNeighFreqCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteNeighFreqCfg->actType = lteeNodeBparams.lteNeighFreqCfg.actType;
   lteNeighFreqCfg->numGeranFreq = lteeNodeBparams.lteNeighFreqCfg.numGeranFreq;
   for(idx=0; idx< (lteNeighFreqCfg->numGeranFreq); idx++)
   {
      lteNeighFreqCfg->t.addNeighFreq[idx].geranFreq = lteeNodeBparams.lteNeighFreqCfg.t.addNeighFreq[idx].geranFreq;
   }

   RETVALUE(ROK);
}

/** @brief This function is used for getting Neigh Cell Geran parameters
 *
 * @details
 *
 *     Function:msmGetNeighCellCfgGeranParameters
 *
 *         Processing steps:
 *
 * @param[in] lteNeighCellCfg
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */

PUBLIC S16 msmGetNeighCellCfgGeranParameters(MsmNeighCellCfg *lteNeighCellCfg)
{
   U32 idx = 0;

   if(lteNeighCellCfg == NULLP)
   {   
      RETVALUE(RFAILED);
   }

   lteNeighCellCfg->numGeranCells = lteeNodeBparams.lteNeighCellCfg.numGeranCells;

   for(idx = 0; idx < lteeNodeBparams.lteNeighCellCfg.numGeranCells; idx++)
   {
      lteNeighCellCfg->t.neighAddCfg[idx].geranCell = lteeNodeBparams.lteNeighCellCfg.t.neighAddCfg[idx].geranCell;
   }

   RETVALUE(ROK);
}
PUBLIC S16 msmSetGostLog(MsmGostLog *lteGostLog)
{
   if(lteGostLog == NULLP)
   {
      RETVALUE(RFAILED);
   }
   lteeNodeBparams.lteGostLog=*lteGostLog;
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


