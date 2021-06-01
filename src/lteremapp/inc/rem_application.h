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
**************************************************************************************************17*/


/**************************************************************************************************

     Name:     REM-APP

     Type:     C++ Header file

     Desc:     Consists of CRemApplication class and related member functions and variables

     File:     rem_application.h

     Sid:      rem_application.h@@/main/Tenb_T3k_Perf/1 - Wed Jan  9 20:53:09 2013

     Prg:      Ambika S

**************************************************************************************************/

#ifdef USE_RADISYS_OAM

#ifndef __rem_application_h_
#define __rem_application_h_

///////////////////////////////////////////////////////////////////////////////////////////////////
//	  System Includes
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <boost/shared_ptr.hpp>
#include <system/TimerInterface.h>
#include <system/SerialisedData.h>
#include <system/TimerExpiry.h>
#include <system/Queueable.h>
#include <system/Exception.h>
#include <messaging/transport/ApplicationWithMessaging.h>
#include <messaging/transport/SingleThreadedAppWithAttrCache.h>
#include <messaging/transport/MessagingEntity.h>
#include <messaging/transport/MessageSerialisedData.h>
#include <messaging/transport/MibAccessInterface.h>
#include <mib-common/messages/MibSubscribeAttributesCnf.h>
#include <mib-common/messages/MibSubscribeAttributesInd.h>


#include <messaging/messages/rem/rem_msgs.h>
#include <rem_events.h>

using namespace boost;
using namespace threeway;

///////////////////////////////////////////////////////////////////////////////////////////////////
//	  Local Includes
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "rem_multiInstanceManager.h"
#include "rem_lteconfig.h"
#include "rem_wcdmaconfig.h"
#include "rem_interface.h"

#define NUM_NEIGHBOR_LIST_INFO 10
#define NUM_UMTS_SCANNED_CELL 16
#define MAX_NEIGHBOR_CELLS 16

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   @class CRemApplication
//
///////////////////////////////////////////////////////////////////////////////////////////////////
class CRemApplication : public threeway::SingleThreadedAppWithAttrCache, public CRemMultiInstanceManager
{
public:
    static CRemApplication& GetInstance();
    void stopScanTimer(U8 bTimerType);
    void sendMessageToOam(CRemScanStatus& objMsg);
    CRemApplication(std::string errString,U8 bScanType,U8 bScanStatus);
    void startScanTimer(U8 bTimerType, U16 scanTimeout);
    void setLteRemDataInMIB(unsigned int numOfCells);
    void addLteCellInMIB(int bCellIndex,__stLteCellInfo& stLteResult);
    void setLteNeighBourList(int bCellIndex,__stLteCellInfo& stLteResult,unsigned int numOfCells);
    void setLastScanTimeInMIB();
    void setScanOnStart(bool);
    void setLteFreqSyncInfo(unsigned int pci, unsigned int earfcn);
    void setWcdmaFreqSyncInfo(unsigned int pci, unsigned int earfcn);
    void HandleLogUpload();
    void StartLogTimer(u32 timer_value);
#if 0
      void setUtraNeighBourList(int bCellIndex,__stWcdmaCellInfo & stUtraResult, U16 numOfCells);
#endif
      void setWcdmaResultInMIB(int bCellIndex,__stWcdmaCellInfo & stWcdmaResult);

protected:
	void InitialiseApp();

private:

    CRemApplication();

    // Singleton instance.
    bool MessageHandler (shared_ptr<threeway::MessageSerialisedData> & message);
    void handleOamStartScanCmd(shared_ptr <CRemOamStartScanCmd> oamStartScanCmd);
    void handleOamAbortScanCmd(shared_ptr <CRemOamAbortScanCmd> oamAbortScanCmd);
    void handleSonStartCarrExpScan(shared_ptr <CRemSonStartCarrExpScan> remSonStartCarrExpScanCmd);
    void handleSonStartPciExpScan(shared_ptr <CRemSonStartPciExpScan> remSonStartPciExpScanCmd);
    void HandleTimerExpiry(shared_ptr<TimerExpiry> timer);
    std::vector<u32> saveEutraScanList(std::string EarfcnRssiScan);
    std::vector<s32> saveEutraRssiScanList(std::string EarfcnRssiScan);
//    bool GetWcdmaConfigParams();
//    bool GetLteConfigParams();
    void initErrorCodes();
//Nfft changes
    RemBandwidth getDLBandWidth(string s_dlBandwidth);
private:

    void GetSubscriptions(std::vector<threeway::SubscriptionDescriptor>& subscriptions);
    std::vector<MessagingEntity> GetSupportedEntities();
    threeway::TimerHandle  m_tRemScanHandle;
    threeway::TimerHandle  m_tRemRssiHandle;
    threeway::TimerHandle  m_tRemSibReadHandle;

    bool m_bScanOnStart;
    bool m_bOamUpdate;
    static CRemApplication  m_rObjRemAppInstance;

    // Required for Timer, Vikram, 01/11/2012
    Mutex           m_mTimerMutex;
    Condition       m_cTimerCondition;

    std::vector<s32> uiRemScanRssi;
    std::vector<u32> uiRemScanEarfcnDl;
    std::vector<u32> uiIndirectNeighbors; 

    threeway::TimerHandle m_logTimerHandle;
   map <pair<u32, u32>, u32> lteCellToInstance ;
   u32 lastLteCellInstance;

	typedef enum
	{
   	   REM_SUBSCRIPTION_LTE_PLMN,
	   REM_SUBSCRIPTION_LTE_EARFCN,
	   REM_SUBSCRIPTION_LTE_BAND,
	   REM_SUBSCRIPTION_LTE_SCANTIMEOUT,
	   REM_SUBSCRIPTION_LTE_MAXCELL,
         REM_SUBSCRIPTION_LTE_REM_SCANONBOOT,
#ifdef LTE_TDD
      REM_SUBSCRIPTION_LTE_SUBFRAME_ASSGN,
      REM_SUBSCRIPTION_LTE_SPECIAL_SUBFRAME_PATTERN,
#endif
/* Nfft changes*/
      REM_SUBSCRIPTION_LTE_DL_BANDWIDTH,
      REM_SUBSCRIPTION_LTE_CNM,
      REM_SUBSCRIPTION_LTE_PARAMS,
      REM_SUBSCRIPTION_EARFCNS_RSSI_SCAN,
      REM_SUBSCRIPTION_LTE_MIO=100,
      REM_SUBSCRIPTION_LTE_MIO_END = 116,
         // WCDMA REM: WCDMA MIB Subscription IDs
         REM_SUBSCRIPTION_INSERV_HANDL,
         REM_SUBSCRIPTION_WCDMA_REM_SCANONBOOT,
         REM_SUBSCRIPTION_PERIODIC_SCANMODE,
         REM_SUBSCRIPTION_PERIODIC_INTERVAL,
         REM_SUBSCRIPTION_PERIODIC_TIME,
         REM_SUBSCRIPTION_WCDMA_SCAN_PLMNLIST,
         REM_SUBSCRIPTION_UMTS_SCAN_BANDLIST,
         REM_SUBSCRIPTION_UMTS_SCAN_UARFCNLIST,
         REM_SUBSCRIPTION_WCDMA_SCANTIMEOUT,
         REM_SUBSCRIPTION_WCDMA_SCANSTATUS,
         REM_SUBSCRIPTION_ERROR_DETAILS,
         REM_SUBSCRIPTION_WCDMA_LAST_SCANTIME,
         REM_SUBSCRIPTION_WCDMA_MAXCELL,
         REM_SUBSCRIPTION_WCDMA_NUMOFCELL,
#ifdef LTE_QCOM
         REM_SUBSCRIPTION_RF_CONFIG_PARAM,
#endif
         REM_SUBSCRIPTION_LOG_UPLOAD
	} RemSubscriptionIds;


};

#endif
#endif

