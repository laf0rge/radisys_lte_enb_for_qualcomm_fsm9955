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

     Desc:     This file contains APIs for Qualcomm OAM integration
               
     File:     wr_smm_enbapp_qcom_oam_int.c

     Sid:      wr_smm_qcom_oam_init.c

     Prg:      mmegha

**********************************************************************/

#include "wr.h"
#include "wr_cmn.h"
#include "wr_smm_smallcell.h"
#include "wr_smm_init.h"
#include "oamClientProtoWrapper.hpp"
#ifdef WR_RSYS_OAM
#include "wr_msm_common.h"
extern MsmLteeNodeBparams lteeNodeBparams;
#endif

/** @brief This function is responsible for returning carrier bandwidth 
 *  based on number of resource blocks 
 *         
 *
 * @details
 *
 *     Function: 
 *             wrGetBandWidthFromRBs
 *         Processing steps:
 *         - Convert number of resource blocks to carrier bandwidth
 *
 * @param[in] numOfRbs
 * 
 * @return carrierBw
 */
PRIVATE U16 wrGetBandWidthFromRBs( U16 numOfRbs )
{
   switch( numOfRbs)
#ifdef WR_RSYS_OAM
   {
      case MSM_LWR_BANDWIDTH_6: 
         {
            return RF_BANDWIDTH_MHZ_1_4;
         }
      case MSM_LWR_BANDWIDTH_15:
         {
            return RF_BANDWIDTH_MHZ_3;
         }
      case MSM_LWR_BANDWIDTH_25:
         {
            return RF_BANDWIDTH_MHZ_5;
         }
      case MSM_LWR_BANDWIDTH_50:
         {
            return RF_BANDWIDTH_MHZ_10;
         }
      case MSM_LWR_BANDWIDTH_75:
         {
            return RF_BANDWIDTH_MHZ_15;
         }
      case MSM_LWR_BANDWIDTH_100:
         {
            return RF_BANDWIDTH_MHZ_20;
         }
      default :
         {
            return RF_BANDWIDTH_MHZ_0; 
         }
   }
#else
   {
       case LWR_BANDWIDTH_6: 
         {
            return RF_BANDWIDTH_MHZ_1_4;
         }
      case LWR_BANDWIDTH_15:
         {
            return RF_BANDWIDTH_MHZ_3;
         }
      case LWR_BANDWIDTH_25:
         {
            return RF_BANDWIDTH_MHZ_5;
         }
      case LWR_BANDWIDTH_50:
         {
            return RF_BANDWIDTH_MHZ_10;
         }
      case LWR_BANDWIDTH_75:
         {
            return RF_BANDWIDTH_MHZ_15;
         }
      case LWR_BANDWIDTH_100:
         {
            return RF_BANDWIDTH_MHZ_20;
         }
      default :
         {
            return RF_BANDWIDTH_MHZ_0; 
         }
  }
#endif
}

/** @brief This function is responsible for handling dynamic 
 *  configuration request for RfCarrierCommon record        
 *
 * @details
 *
 *     Function: 
 *           wrQcomRfCarrierCommonDynamicCfgReq 
 *         Processing steps:
 *         - Initiate Set request with operation  type set to 
 *         update to update modified parameters
 * 
 * @return void
 */
PUBLIC S16 wrQcomRfCarrierCommonDynamicCfgReq(Bool adminState)
{

   S16 ret = RFAILED;
   RfConfig recordRfCarrierCommon;

   recordRfCarrierCommon.rfCarrierCommonAdminState = adminState;
#ifdef WR_RSYS_OAM
   recordRfCarrierCommon.tech = lteeNodeBparams.lteRsysQcomCfgParam.supportedSysList.supportedSystemList[0];
   recordRfCarrierCommon.rfCarrierCommonBandWidth = wrGetBandWidthFromRBs(lteeNodeBparams.lteCellMibCfgGrp.dlBw[0]); 
#else
   recordRfCarrierCommon.tech = RF_TECHNOLOGY_LTE;
   recordRfCarrierCommon.rfCarrierCommonBandWidth = wrGetBandWidthFromRBs(wrSmDfltCellDlBW);
#endif
   recordRfCarrierCommon.rfCarrierCommonCarrierId = rfCarrierCommonCarrierId;
   recordRfCarrierCommon.rfCarrierCommonStandalone = rfCarrierCommonStandalone;
   recordRfCarrierCommon.rfCarrierCommonRxPathIdLst_0 = rfCarrierCommonRxPathIdLst_0;
   recordRfCarrierCommon.rfCarrierCommonRxPathIdLst_1 = rfCarrierCommonRxPathIdLst_1;
   recordRfCarrierCommon.rfCarrierCommonRxPathIdLst_2 = rfCarrierCommonRxPathIdLst_2;
   recordRfCarrierCommon.rfCarrierCommonRxPathIdLst_3 = rfCarrierCommonRxPathIdLst_3;
   recordRfCarrierCommon.rfCarrierCommonTxPathIdLst_0 = rfCarrierCommonTxPathIdLst_0;
   recordRfCarrierCommon.rfCarrierCommonTxPathIdLst_1 = rfCarrierCommonTxPathIdLst_1;
   recordRfCarrierCommon.rfCarrierCommonTxPathIdLst_2 = rfCarrierCommonTxPathIdLst_2;
   recordRfCarrierCommon.rfCarrierCommonTxPathIdLst_3 = rfCarrierCommonTxPathIdLst_3;
   /* Fill all the parameters required by RecordRfCarrierCommon */
   ret = (S16)oamClientSetRecord(recordRfCarrierCommon);

   return ret;
}



/** @brief This function is responsible initializing oam client
 *  filling configuration parameters required to bring up 
 *  Qualcomm L1 layer 
 *         
 *
 * @details
 *
 *     Function: 
 *            wrRegisterAndCreateRecords 
 *         Processing steps:
 *         - Create OAM Client and fill Qualcomm RF configuration 
 *           parameters
 * 
 * @return void
 */
PUBLIC S16 wrRegisterAndCreateRecords()
{  
   S16 ret = RFAILED;
   RfConfig rfConfig;

#ifdef WR_RSYS_OAM
   /* Set AdminState to false while creating record for the first time 
    * as rfTxCarrierProv and rfRxCarrierProv requires adminState to be false */
   rfConfig.rfCarrierCommonAdminState = FALSE;
   rfConfig.rfCarrierCommonBandWidth = wrGetBandWidthFromRBs(lteeNodeBparams.lteCellMibCfgGrp.dlBw[0]); 
   rfConfig.txARFCN = lteeNodeBparams.lteAddCellCfg.earfdnDl;
   rfConfig.rfRxARFCN = lteeNodeBparams.lteAddCellCfg.earfdnUl;
   rfConfig.txPowerDbm = (double)(lteeNodeBparams.ltePdschCfgCommon.referenceSignalPwr[0] + 27);

   /* Below check is to ensure hte txPowerDbm is in the valid range of Qcom */
   if (rfConfig.txPowerDbm > 24)
   {
      rfConfig.txPowerDbm = 24;
   }
   else if (rfConfig.txPowerDbm < -26)
   {
      rfConfig.txPowerDbm = -26;
   }

    /* Below parameters are ReadOnly, dynamic configuration not supported  */
   /* Currently only one band is supported */
   rfConfig.BandList_0 = lteeNodeBparams.lteRsysQcomCfgParam.bandSupportedList.bandLst[0];
   rfConfig.BandList_1 = 0;
   rfConfig.BandList_2 = 0;
   rfConfig.BandList_3 = 0;
   rfConfig.tech = lteeNodeBparams.lteRsysQcomCfgParam.supportedSysList.supportedSystemList[0];
#else
   /* Initialize without Radisys OAM */
   rfConfig.rfCarrierCommonAdminState = FALSE;
   rfConfig.tech = RF_TECHNOLOGY_LTE; 
   rfConfig.rfCarrierCommonBandWidth = wrGetBandWidthFromRBs(wrSmDfltCellDlBW);
   rfConfig.txARFCN = wrSmDfltServingCellDLCarrierFreq; 
   rfConfig.txPowerDbm = 20;
   rfConfig.rfRxARFCN = wrSmDfltUlEarFcn;
   rfConfig.BandList_0 = wrSmDfltFreqBandInd;
   rfConfig.BandList_1 = 0;
   rfConfig.BandList_2 = 0;
   rfConfig.BandList_3 = 0;
#endif
   /* Static initialization of  rf parameters */
   rfConfig.rfCarrierCommonCarrierId = rfCarrierCommonCarrierId;
   rfConfig.rfCarrierCommonStandalone = rfCarrierCommonStandalone;
   rfConfig.rfCarrierCommonRxPathIdLst_0 = rfCarrierCommonRxPathIdLst_0;
   rfConfig.rfCarrierCommonRxPathIdLst_1 = rfCarrierCommonRxPathIdLst_1;
   rfConfig.rfCarrierCommonRxPathIdLst_2 = rfCarrierCommonRxPathIdLst_2;
   rfConfig.rfCarrierCommonRxPathIdLst_3 = rfCarrierCommonRxPathIdLst_3;
   rfConfig.rfCarrierCommonTxPathIdLst_0 = rfCarrierCommonTxPathIdLst_0;
   rfConfig.rfCarrierCommonTxPathIdLst_1 = rfCarrierCommonTxPathIdLst_1;
   rfConfig.rfCarrierCommonTxPathIdLst_2 = rfCarrierCommonTxPathIdLst_2;
   rfConfig.rfCarrierCommonTxPathIdLst_3 = rfCarrierCommonTxPathIdLst_3;
   rfConfig.rfPathProvPathId1 = rfPathProvPathId1;
   rfConfig.rfPathProvPath1TxDirection = rfPathProvPath1TxDirection;
   rfConfig.rfPathProvPath1RxDirection = rfPathProvPath1RxDirection;
   rfConfig.rfPathProvPathId2 = rfPathProvPathId2;
   rfConfig.rfPathProvPath2TxDirection = rfPathProvPath2TxDirection;
   rfConfig.rfPathProvPath2RxDirection = rfPathProvPath2RxDirection;
   rfConfig.rfTxCarrierProvCarrierId = rfTxCarrierProvCarrierId;
   rfConfig.rfRxCarrierProvCarrierId = rfRxCarrierProvCarrierId;
   rfConfig.rfManagerConfigLogLevel = rfManagerConfigLogLevel;
   rfConfig.rfManagerConfigLogArea = rfManagerConfigLogArea;
   rfConfig.rfManagerConfigRequireCal = rfManagerConfigRequireCal;
   
   /* Initialise oamProxy client and create RF records with static configuration */
   ret = (S16)oamClientCreateRecords(rfConfig, FALSE);

   if( ret != RFAILED )
   {
      /* Update AdminState to true */
      ret = wrQcomRfCarrierCommonDynamicCfgReq(TRUE);

      if(RFAILED == ret)
      {
         printf(" Rf Carrier Cmn Cfg Failed :%d\n",ret);
      }
   }
   else
   {
      printf(" Failed record create : mgmtResult:%d\n",ret);
   }

   return ROK; //ret;
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
$SID$        ---      Sriky         1. Radisys OAM integration with 
                                       Qualcomm
*********************************************************************91*/

