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


/**************************************************************************************************

Name:     REM-APP

Type:     C++ Source file

Desc:     This file consists of most of the entry level function for REM-APP like receiving
messages from other entities(OAM/MIB/SON) and updating the results to MIB/OAM

File:     rem_application.cpp

Sid:      rem_application.cpp@@/main/Tenb_T3k_Perf/1 - Wed Jan  9 20:53:21 2013

Prg:      Vikram K
Ambika S

 **************************************************************************************************/

#ifdef USE_RADISYS_OAM
#include <system/SerialisedData.h>
#include <boost/shared_ptr.hpp>
#include <string.h>
#include "Queueable.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//   Local includes
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "rem_datatype.h"
#include "rem_interface.h"
#include "rem_application.h"

/// This header files for MIO creation once implement rem_multiinstanceobject.cpp then can remove this
#include <messaging/transport/ApplicationWithMessaging.h>
#include <messaging/transport/SingleThreadedAppWithAttrCache.h>
#include <messaging/transport/MessagingEntity.h>
#include <messaging/transport/MessageSerialisedData.h>
#include <messaging/transport/MibAccessInterface.h>
#include <mib-common/messages/MibSubscribeAttributesCnf.h>
#include <mib-common/messages/MibSubscribeAttributesInd.h>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <mib-common/messages/MibCreateObjectReq.h>
#include <mib-common/messages/MibDeleteObjectReq.h>
#include <messaging/transport/AppMibAttributeCache.h>
#include <messaging/messages/common/UploadLogReq.h>
#include "MessagingEntity.h"
///////////////////////////////////////////////////////////
using namespace threeway;
using namespace boost;

#define INVALID_CELL_INSTANCE 0

void remScanTimerExpired(U8 bTimerType);
void remStartCarrierExprScan(U8 bEARFCNCount, U16* usEARFCNList, U16 scanTimeout);
void remStartPCIExpressScan(U8 bPCICount, U16* usPCIList, U16 scanTimeout);
extern void remPrintLteResult(__stLteCellInfo stCellInfo);

CRemApplication CRemApplication::m_rObjRemAppInstance;

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ class : CRemApplication
//
// Class CRemApplication is derived from the class SingleThreadedAppWithAttrCache
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CRemApplication::CRemApplication():SingleThreadedAppWithAttrCache("oam-rem")
{
	ENTER();
	m_bScanOnStart = false;
	Thread::InitialiseMutex(m_mTimerMutex);
	Thread::ConditionInit(m_cTimerCondition);
	TimerEngine::GetInstance(m_mTimerMutex, m_cTimerCondition);
	m_tRemScanHandle = 0;
	m_tRemRssiHandle = 0;
   m_tRemSibReadHandle =0;
	EXIT();
}

void CRemApplication::setScanOnStart(bool bFlag)
{
	m_bScanOnStart = bFlag;
}

void CRemApplication::InitialiseApp()
{
	SingleThreadedApplicationWithMessaging::RegisterMessageHandler(this, "oam-rem");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//
// This function gets the single instnace of class CRemApplication
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CRemApplication & CRemApplication::GetInstance()
{
	return m_rObjRemAppInstance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//
// This function gets entity for REM Application defined in class MessagingEntity
//
///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<MessagingEntity> CRemApplication::GetSupportedEntities()
{
	vector<MessagingEntity> entities;
	entities.push_back(ENTITY_OAM_REM);
	return entities;
}
/* Nfft Chanegs */
RemBandwidth CRemApplication::getDLBandWidth(string s_dlBandwidth)
{
   const char* arrdlBandwidthInfo[] = {"n6","n15","n25","n50","n75","n100"};
   U8 bDlBwIdx = 0;
   U8 bMaxNumBwSupp = 6;

   while(bDlBwIdx < bMaxNumBwSupp)
   {
      if(0 == strcmp(s_dlBandwidth.c_str(),arrdlBandwidthInfo[bDlBwIdx]))
      {
#ifdef ENABLE_REM_LOGS
         printf("\n%s == %s --> Index %d\n",s_dlBandwidth.c_str(),arrdlBandwidthInfo[bDlBwIdx],bDlBwIdx);
#endif
         return (RemBandwidth)bDlBwIdx;
      }
      bDlBwIdx++;
   };
   return (RemBandwidth)(bDlBwIdx-1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//
// This function is to use save EARFCN values
//
///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<u32> CRemApplication::saveEutraScanList(std::string EarfcnRssiScan)
{
	std::string strBand;
	size_t	commaPos;  //coma position
	size_t  currPos = 0;
   std::vector<u32> globalList;

   if( EarfcnRssiScan.empty() )
      return globalList;

	while((commaPos = EarfcnRssiScan.find(',',currPos))!= std::string::npos)
	{
		strBand = EarfcnRssiScan.substr(currPos, commaPos-currPos);
		currPos = commaPos+1;
		globalList.push_back(atoi(strBand.c_str()));
	}

	strBand = EarfcnRssiScan.substr(currPos, commaPos);
	globalList.push_back(atoi(strBand.c_str()));
   return globalList;
}   




///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//
// This function is to use save RSSI pair values
//
///////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<s32> CRemApplication::saveEutraRssiScanList(std::string EarfcnRssiScan)
{
	std::string strBand;
	size_t	commaPos;  //coma position
	size_t  currPos = 0;
   std::vector<s32> globalList;

   if( EarfcnRssiScan.empty() )
      return globalList;

	while((commaPos = EarfcnRssiScan.find(',',currPos))!= std::string::npos)
	{
		strBand = EarfcnRssiScan.substr(currPos, commaPos-currPos);
		currPos = commaPos+1;
		globalList.push_back(atoi(strBand.c_str()));
	}

	strBand = EarfcnRssiScan.substr(currPos, commaPos);
	globalList.push_back(atoi(strBand.c_str()));
   return globalList;
}   

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] message : incoming message to Rem Application
//
// This is the message handler function that handles the incoming messages from OAM and SON
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRemApplication::MessageHandler(shared_ptr<threeway::MessageSerialisedData> & message)
{

	bool bHandled = SingleThreadedAppWithAttrCache::MessageHandler(message);
	if(bHandled)
	{
		return true;
	}

	CRemLteConfigData & oLteCfg = CRemLteConfigData::getInstance();
        CRemWcdmaConfigData & oWcdmaCfg = CRemWcdmaConfigData::getInstance();
	try
	{
		switch (message->GetSerialisationId())
		{
			case SERIALISATION_ID_REQUEST_START_RSP:
				//Nothing to do
				break;
			case SERIALISATION_ID_REM_OAM_ABORT_SCAN:
				handleOamAbortScanCmd(Serialisable::DeSerialise<CRemOamAbortScanCmd>(message));
				break;

			case SERIALISATION_ID_REM_OAM_START_SCAN:
				handleOamStartScanCmd(Serialisable::DeSerialise<CRemOamStartScanCmd>(message));
				break;

			case SERIALISATION_ID_REM_SON_START_CARRIER_EXPRESS_SCAN:
				handleSonStartCarrExpScan(Serialisable::DeSerialise<CRemSonStartCarrExpScan>(message));
				break;

			case SERIALISATION_ID_REM_SON_START_PCI_EXPRESS_SCAN:
				handleSonStartPciExpScan(Serialisable::DeSerialise<CRemSonStartPciExpScan>(message));
				break;
			case SERIALISATION_ID_MIB_SUBSCRIBE_ATTRIBUTES_IND:
				{
					shared_ptr<MibSubscribeAttributesInd> subscribeAttributesInd = Serialisable::DeSerialise<MibSubscribeAttributesInd>(message);
					RSYS_ASSERT(subscribeAttributesInd != NULL);
					switch(subscribeAttributesInd->GetSubscriptionId())
					{
						case REM_SUBSCRIPTION_LTE_PLMN:
							{

								std::string strPlmn;
								GetMibCache().GetMibAttributeAsString(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE), (MibAttributeId)(PARAM_ID_LTE_REM_SCAN_PLMN_LIST), strPlmn);
								strPlmn.erase(remove( strPlmn.begin(), strPlmn.end(), '\"' ),strPlmn.end());
								if(!m_bScanOnStart)
									oLteCfg.savePlmnList(strPlmn);
								break;

							}

						case REM_SUBSCRIPTION_LTE_EARFCN:
							{
								std::string strEarfcn;
								GetMibCache().GetMibAttributeAsString(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),(MibAttributeId)(PARAM_ID_LTE_REM_SCAN_ARFCN_LIST),strEarfcn);
								strEarfcn.erase(remove( strEarfcn.begin(), strEarfcn.end(), '\"' ),strEarfcn.end());
								if(!m_bScanOnStart)
									oLteCfg.saveEARFCNList(strEarfcn);
								break;
							}
						case REM_SUBSCRIPTION_LTE_BAND:
							{
								std::string strBand;
								GetMibCache().GetMibAttributeAsString(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),(MibAttributeId)(PARAM_ID_LTE_REM_SCAN_BAND_LIST),strBand);
								strBand.erase(remove( strBand.begin(), strBand.end(), '\"' ),strBand.end());
								if(!m_bScanOnStart)
									oLteCfg.saveBandList(strBand);
								break;

							}
						case REM_SUBSCRIPTION_LTE_SCANTIMEOUT:
							{
								U16  scanTimeOut;
								GetMibCache().GetMibAttribute(PARAM_ID_LTE_SCAN_TIMEOUT, scanTimeOut);
								if(!m_bScanOnStart)
									oLteCfg.saveScanTimeout(scanTimeOut);
								break;
							}

						case REM_SUBSCRIPTION_LTE_MAXCELL:
							{
								U8  maxCellEntry;
								GetMibCache().GetMibAttribute(PARAM_ID_LTE_MAX_REM_ENTRIES, maxCellEntry);
								oLteCfg.saveMaxCellEntry(maxCellEntry);
								break;

							}
                     
#ifdef LTE_TDD
                     case REM_SUBSCRIPTION_LTE_SUBFRAME_ASSGN:
                     {
                        U8  subframeAssignment;
                        GetMibCache().GetMibAttribute(PARAM_ID_LTE_TDD_SUBFRAME_ASSIGNMENT, subframeAssignment);
                        oLteCfg.savesubframeAssignment(subframeAssignment);
                        break;
                      }
                     case REM_SUBSCRIPTION_LTE_SPECIAL_SUBFRAME_PATTERN:
                     {
                        U8  specialSubframePatterns;
                        GetMibCache().GetMibAttribute(PARAM_ID_LTE_TDD_SPECIAL_SUB_FRAME_PATTERNS, specialSubframePatterns);
                        oLteCfg.savespecialSubframePatterns(specialSubframePatterns);
                        break;
                     }
#endif           
                  /* Nfft Changes */ 
                  case REM_SUBSCRIPTION_LTE_DL_BANDWIDTH:
                     {
                        RemBandwidth enDlBw;
                        std::vector<u32> dlBandwidth;

                        GetMibCache().GetMibAttribute(PARAM_ID_LTE_DL_BANDWIDTH, dlBandwidth);
                        if (dlBandwidth.size()>0)
                        {
                           stringstream s_dlBandwidth;
                           s_dlBandwidth<<"n"<<dlBandwidth.at(0);
                           enDlBw = getDLBandWidth(s_dlBandwidth.str());
                           oLteCfg.saveDlBw(enDlBw);
                        }
                        break;
                     }

                  case REM_SUBSCRIPTION_LTE_CNM:
                     {
                        U8 cnmEnable;
                        GetMibCache().GetMibAttribute(PARAM_ID_LTE_CNM_ENABLE, cnmEnable);
#if 0/* Naw:: Need to enable this code
        once CNM is moved to new design
        .One approach is force disabling of
        the below flag so that it will 
        not enter into CNM state machine.
        Another approach is to remove
        all the code related to CNM */
                        cnmEnable = 0;
#endif
                        oLteCfg.saveCnmEnableFlag(cnmEnable);
                        printf("\n CNM Enable :: %d\n",cnmEnable);
                     }
                     break;
                  /* RSSI Changes */
						case REM_SUBSCRIPTION_EARFCNS_RSSI_SCAN:
							{
                        std::string remScanRssi;
                        std::string remScanEarfcn;
                        u32 numEarfcnRssiScan = 0;
                        GetMibCache().GetMibAttribute(PARAM_ID_LTE_REM_NUM_EARFCN, numEarfcnRssiScan);
                        GetMibCache().GetMibAttributeAsString(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),(MibAttributeId)(PARAM_ID_LTE_REM_RSSI),remScanRssi);
                        remScanRssi.erase(remove(remScanRssi.begin(), remScanRssi.end(), '\"' ), remScanRssi.end());
                        GetMibCache().GetMibAttributeAsString(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),(MibAttributeId)(PARAM_ID_LTE_REM_EARFCNDL),remScanEarfcn);
                        remScanEarfcn.erase(remove(remScanEarfcn.begin(), remScanEarfcn.end(), '\"' ), remScanEarfcn.end());

#if 1
                        if(numEarfcnRssiScan != 0)
                        {
                           uiRemScanRssi = saveEutraRssiScanList(remScanRssi);
                           uiRemScanEarfcnDl = saveEutraScanList(remScanEarfcn);
                        }
#endif

                        break;
                     }
                  /* End of RSSI changes */
                  case REM_SUBSCRIPTION_LTE_REM_SCANONBOOT:
                     {
                        bool lteScanOnBoot;
                        GetMibCache().GetMibAttribute(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE), (MibAttributeId)(PARAM_ID_LTE_REM_SCAN_ON_BOOT), lteScanOnBoot);
                        if(!m_bScanOnStart)
                           oLteCfg.saveScanOnBootInfo(lteScanOnBoot);
                        break;
                     }
                  case REM_SUBSCRIPTION_WCDMA_REM_SCANONBOOT:
                     {
                        bool wcdmaScanOnBoot;
                        GetMibCache().GetMibAttribute(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_UMTS_WCDMA), (MibAttributeId)(PARAM_ID_WCDMA_REM_SCAN_ON_BOOT), wcdmaScanOnBoot);
                        if(!m_bScanOnStart)
                           oWcdmaCfg.saveScanOnBootInfo(wcdmaScanOnBoot);
                        break;
                     }
                  case REM_SUBSCRIPTION_WCDMA_SCAN_PLMNLIST:
                     {
                        std::string strPlmn;
                        GetMibCache().GetMibAttributeAsString(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_UMTS_WCDMA), (MibAttributeId)(PARAM_ID_WCDMA_REM_SCAN_PLMN_LIST), strPlmn);
                        strPlmn.erase(remove( strPlmn.begin(), strPlmn.end(), '\"' ),strPlmn.end());
                        if(!m_bScanOnStart)
                           oWcdmaCfg.savePlmnList(strPlmn);
                        break;
                     }
                  case REM_SUBSCRIPTION_UMTS_SCAN_UARFCNLIST:
                     {
                        std::string strUarfcn;
                        GetMibCache().GetMibAttributeAsString(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_UMTS_WCDMA),(MibAttributeId)(PARAM_ID_REM_SCAN_UARFCN_LIST),strUarfcn);
                        strUarfcn.erase(remove( strUarfcn.begin(), strUarfcn.end(), '\"' ),strUarfcn.end());
                        if(!m_bScanOnStart)
                           oWcdmaCfg.saveEARFCNList(strUarfcn);
                        break;
                     }

                  case REM_SUBSCRIPTION_UMTS_SCAN_BANDLIST:
                     {
                        std::string strBand;
                        GetMibCache().GetMibAttributeAsString(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_UMTS_WCDMA),(MibAttributeId)(PARAM_ID_UMTS_REM_SCAN_BAND_LIST),strBand);
                        strBand.erase(remove( strBand.begin(), strBand.end(), '\"' ),strBand.end());
                        if(!m_bScanOnStart)
                           oWcdmaCfg.saveBandList(strBand);
                        break;
                     }

                  case REM_SUBSCRIPTION_WCDMA_SCANTIMEOUT:
                     {
                        U16  scanTimeOut;
                        GetMibCache().GetMibAttribute(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_UMTS_WCDMA), (MibAttributeId)PARAM_ID_WCDMA_SCAN_TIMEOUT, scanTimeOut);
                        if(!m_bScanOnStart)
                           oWcdmaCfg.saveScanTimeout(scanTimeOut);
                        break;
                     }

                  case REM_SUBSCRIPTION_WCDMA_MAXCELL:
                     {
                        U8  maxCellEntry;
                        GetMibCache().GetMibAttribute(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_UMTS_WCDMA), (MibAttributeId)PARAM_ID_MAX_CELL_ENTRIES, maxCellEntry);
                        oWcdmaCfg.saveMaxCellEntry(maxCellEntry);
                        break;
                     }
#ifdef LTE_QCOM
                  case REM_SUBSCRIPTION_RF_CONFIG_PARAM:
                     {
                        std::vector<u32> bandsupportedlist;
                        std::vector<std::string> supportedSystemList;
                        U16 bandSupported;
                        U8 supportedSystem;

                        GetMibCache().GetMibAttribute(PARAM_ID_BANDS_SUPPORTED, bandsupportedlist);
                        U32 bandSupportedListSize = bandsupportedlist.size();

                        for( size_t idx = 0; idx < bandSupportedListSize; idx++ )
                        {
                           bandSupported = (u16)(bandsupportedlist[idx]);
                           oLteCfg.saveBandSupported(bandSupported);
                        }

                        /* Read Supported System */
                        GetMibCache().GetMibAttribute(PARAM_ID_SUPPORTED_SYSTEMS, supportedSystemList);
                        U32 SupportedSystemListSize = supportedSystemList.size();
                        for( size_t idx = 0; idx < SupportedSystemListSize; idx++ )
                        {
                           /* Assigning 1,2,3 as per Qualcomm enum values
                            * RF_TECHNOLOGY_UMTS = 1 / RF_TECHNOLOGY_LTE = 2/ RF_TECHNOLOGY_LAST = 3 */
                           if ( ! strcmp( supportedSystemList[idx].c_str(), "UMTS"))
                           {
                              supportedSystem = 1;
                           }
                           else if( ! strcmp( supportedSystemList[idx].c_str(), "LTE"))
                           {
                              supportedSystem = 2;
                           }
                           else
                           {
                              supportedSystem = 3;
                           }
                           oLteCfg.saveSupportedTech(supportedSystem);
                        }
                         break;
                     }
#endif
                  case REM_SUBSCRIPTION_LOG_UPLOAD:
                     {
                        HandleLogUpload();
                        break;
                     }
						default :
						TRACE_MSG("Invalid Subscription Id");
						break;
					}//end of switch
				break;//break of case
				}
			default:
			break;
		}
	}

catch (threeway::Exception& e)
{
	TRACE_EXCEPTION(e);
}
return bHandled;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] scanTimeout  :  scanTimeOut value
//
// This function starts scantimer
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::startScanTimer(U8 bTimerType, U16 scanTimeout)
{
   stopScanTimer(bTimerType);

	threeway::TimerHandle timerHandle = CreateTimer("REMScanTimeOut",scanTimeout*1000,
			TimerEngineInterface::TIMER_START_NOW,
			TimerEngineInterface::TIMER_SINGLE_SHOT);

	switch (bTimerType )
	{
		case REM_SCAN_TIMEOUT_EVT:
			m_tRemScanHandle = timerHandle;
			break;

		case REM_RSSI_TIMEOUT_EVT:
			m_tRemRssiHandle = timerHandle;
			break;

		case REM_READ_SIB_TIMEOUT_EVT:
			m_tRemSibReadHandle = timerHandle;
			break;
	}

	if( !IsTimerRunning(timerHandle) )
		StartTimer(timerHandle);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//
// This function stops scantimer
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::stopScanTimer(U8 bTimerType)
{
	threeway::TimerHandle timerHandle = threeway::NULL_TIMER;

	switch (bTimerType)
	{
		case REM_SCAN_TIMEOUT_EVT:
			timerHandle = m_tRemScanHandle;
			m_tRemScanHandle = threeway::NULL_TIMER;
			break;

		case REM_RSSI_TIMEOUT_EVT:
			timerHandle = m_tRemRssiHandle;
			m_tRemRssiHandle = threeway::NULL_TIMER;
			break;

		case REM_READ_SIB_TIMEOUT_EVT:
			timerHandle = m_tRemSibReadHandle;
			m_tRemSibReadHandle = threeway::NULL_TIMER;
			break;
	}

	if (timerHandle != threeway::NULL_TIMER)
	{
		StopTimer (timerHandle);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] expiredTimer : shared pointer to the class TimerExpiry
//
// This is the handler function for timer expiry
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::HandleTimerExpiry(shared_ptr<TimerExpiry> timer)
{
	SingleThreadedAppWithAttrCache::HandleTimerExpiry(timer);

	if( timer->GetTimerHandle() == m_tRemScanHandle) {
		m_tRemScanHandle = threeway::NULL_TIMER;
		remScanTimerExpired(REM_SCAN_TIMEOUT_EVT);
	}

	if( timer->GetTimerHandle() == m_tRemRssiHandle) {
		m_tRemRssiHandle = threeway::NULL_TIMER;
		remScanTimerExpired(REM_RSSI_TIMEOUT_EVT);
	}

	if( timer->GetTimerHandle() == m_tRemSibReadHandle) {
		m_tRemSibReadHandle = threeway::NULL_TIMER;
		remScanTimerExpired(REM_READ_SIB_TIMEOUT_EVT);
	}
   else if(timer->GetTimerHandle() == m_logTimerHandle)
   {
      string logFileName[3];
      int numClosedFiles = Trace::GetInstance().GetLogFilesToUpload(logFileName);
      for(int n_uploadfile = 0;n_uploadfile < numClosedFiles;n_uploadfile++)
      {
         UploadLogReq upLogReq(logFileName[n_uploadfile]);
         SendMessage(upLogReq,ENTITY_FTP,ENTITY_OAM_REM);
      }
      HandleLogUpload();
   }
   }

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] oamStartScanCmd : shared pointer to the class CRemOamStartScanCmd
//
// This function handles the Incoming start scan command from OAM
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::handleOamStartScanCmd(shared_ptr <CRemOamStartScanCmd> oamStartScanCmd)
{
	if( oamStartScanCmd == NULL)
	{
		threeway::Exception oNullPtrExc(__FILE__, __LINE__, "NULL PTR of CRemOamStartScanCmd");
		throw oNullPtrExc;
	}

	if( m_bScanOnStart == false )
	{
           MibAttributeValues numOfLteCellsAttrs, numOfWcdmaCellsAttrs;
           
            EraseRemObjects();
            u32 numCellEntries = 0;
            numOfWcdmaCellsAttrs.AddAttribute(PARAM_ID_NUM_CELL_ENTRIES, numCellEntries);
            GetMibCache().SetMibAttributesByDn(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_UMTS_WCDMA),
               numOfWcdmaCellsAttrs, ENTITY_OAM_REM);

            numOfLteCellsAttrs.AddAttribute(PARAM_ID_LTE_NUM_REM_ENTRIES, numCellEntries);
            GetMibCache().SetMibAttributesByDn(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),
               numOfLteCellsAttrs, ENTITY_OAM_REM);

            remLog(REM_TRACE_INFO,"Scan trigger is from OAM \n");
	    remStartScan(oamStartScanCmd->getScanType());
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] oamAbortScanCmd : shared pointer to the class CRemOamAbortScanCmd
//
// This function handles the Incoming Abort Scan Command from OAM
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::handleOamAbortScanCmd(shared_ptr <CRemOamAbortScanCmd> oamAbortScanCmd)
{
	if( oamAbortScanCmd == NULL)
	{
		threeway::Exception oNullPtrExc(__FILE__, __LINE__, "NULL PTR of CRemOamAbortScanCmd");
		throw oNullPtrExc;
	}

	remAbortScan();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] remSonStartCarrExpScanCmd : shared pointer to the class CRemSonStartCarrExpScan
//
// This function handles START CARRIER EXPRESS SCAN command that comes from SON
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::handleSonStartCarrExpScan(shared_ptr <CRemSonStartCarrExpScan> sonCmd)
{
	if(sonCmd == NULL)
	{
		threeway::Exception oNullPtrExc(__FILE__, __LINE__, "NULL PTR of CRemSonStartCarrExpScan");
		throw oNullPtrExc;
	}

	remStartCarrierExprScan(sonCmd->getEARFCNCount(), sonCmd->getEARFCNList(),sonCmd->m_usscanTimeOut);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] remSonStartPciExpScanCmd : shared pointer to the class CRemSonStartPciExpScan
//
// This function handles START PCI EXPRESS SCAN command that comes from SON
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::handleSonStartPciExpScan(shared_ptr <CRemSonStartPciExpScan> sonCmd)
{
	if(sonCmd == NULL)
	{
		threeway::Exception oNullPtrExc(__FILE__, __LINE__, "NULL PTR of CRemSonStartPciExpScan");
		throw oNullPtrExc;
	}

	remStartPCIExpressScan(sonCmd->m_bNumOfPci, sonCmd->m_usPCIList,sonCmd->m_bscanTimeOut);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] objMsg : object of class Serialisable
//
// This function sends OAM Scan status message to OAM
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::sendMessageToOam(CRemScanStatus& objMsg)
{
#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"Sending Message to OAM");
#endif
	SendMessage(objMsg, ENTITY_OAM, ENTITY_OAM_REM);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] - bCellIndex  : Cell Index
//				 stLteResult : instance of sturcture __stLteCellInfo
//
// This function LTE Result Parameters to MIB Cache
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::addLteCellInMIB(int bCellIndex,__stLteCellInfo & stLteResult)
{
   u32 numLteRemCells = 0, maxLteRemCells;
   MibAttributeValues numOfCellsAttrs;

   GetMibCache().GetMibAttribute(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),
         PARAM_ID_LTE_MAX_REM_ENTRIES, maxLteRemCells);
   GetMibCache().GetMibAttribute(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),
         PARAM_ID_LTE_NUM_REM_ENTRIES, numLteRemCells);
   if (bCellIndex < (int)maxLteRemCells)
   {
#ifdef DEBUG_PRINT
      remLog(REM_TRACE_INFO,"CRemApplication::setLteResultInMIB");
#endif
      AddLteCell(stLteResult.uiEARFCN, stLteResult.uiPhyCellID);
      MibDN cellDn = GetLteCellDn(stLteResult.uiEARFCN, stLteResult.uiPhyCellID);

      MibAttributeValues attrs;
      /* RF Parameters */
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_EARFCN, stLteResult.uiEARFCN);
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_PCI, stLteResult.uiPhyCellID);
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_RSRP, stLteResult.iRSRP);
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_RSRQ, stLteResult.iRSRQ);
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_RSSI, stLteResult.iRSSI);

      /* BCCH Parameters */
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_DL_BANDWIDTH,stLteResult.stBcch.uiDlBandwidth);
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_UL_BANDWIDTH,stLteResult.stBcch.uiUlBandwidth);
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_RS_TX_POWER,stLteResult.stBcch.iRSTxPower);
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_TAC,stLteResult.stBcch.uiTac);
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_CELL_IDENTITY,stLteResult.stBcch.uiCellId);
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_CELL_BARRED,!(stLteResult.stBcch.bCellBarred));
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_CSG_INDICATOR,stLteResult.stBcch.bCsgInd);
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_CSG_ID,stLteResult.stBcch.uiCsgIdentity);
      attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_NUM_OF_PLMN_ENTRIES, 
            stLteResult.stBcch.uiPlmnListNumOfEntries);

      for( unsigned int idx=0 ; idx< stLteResult.stBcch.uiPlmnListNumOfEntries; idx++ )
      {
         attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_PLMNID,stLteResult.stBcch.stPLMNList[idx].szPLMN);
         attrs.AddAttribute(PARAM_ID_LTE_REM_SCANNED_CELL_RESERVE_FOR_OPERATOR,
               stLteResult.stBcch.stPLMNList[idx].bCellRsrvdForOperUse);
      }

      /* Radisys propreitary parameters */
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_ULDL_SUB_FRAME_CFG,
            stLteResult.ulDlSubframeCfg);
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_SPL_SF_CFG,
            stLteResult.splSfCfg);
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_CP_DL,
            stLteResult.bCpSize);
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_CP_UL,
            stLteResult.bCpSize);

      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_IS_PRACH_CFG_PRESENT,
            TRUE);

      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_UL_EARFCN,
            stLteResult.uiUlEARFCN);
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_NUM_ANTENNA,
            stLteResult.TxAntenna);

      /* Parameters not scanned by REM, filling with default values */
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_QOFFSET,-10);
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_CIO,0);
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_BLACKLISTED,0);

      uiIndirectNeighbors.push_back(stLteResult.uiNumIntraFreqNghs);
      for(u32 numNgh=1; numNgh<= stLteResult.uiNumIntraFreqNghs; numNgh++)
      {
         uiIndirectNeighbors.push_back(stLteResult.stNghInfo[numNgh].uiPhyCellId);
      }
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_INTRA_FREQ_NEIGH_PCI, uiIndirectNeighbors);
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_PRACH_ROOT_SEQ_INDEX, (u32)stLteResult.stPrachInfo.uiRootSequenceIndex);
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_PRACH_ZERO_CORRELATIONZONE_CONFIG, (u32)stLteResult.stPrachInfo.uiZeroCorrelationZoneConfig);
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_PRACH_CONFIG_INDEX, (u32)stLteResult.stPrachInfo.uiConfigurationIndex);
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_PRACH_HIGH_SPEED_FLAG, (u32)stLteResult.stPrachInfo.bHighSpeedFlag);
      attrs.AddAttribute(PARAM_ID_LTE_X_RADISYS_REM_SCANNED_PRACH_FREQ_OFFSET, (u32)stLteResult.stPrachInfo.uiFreqOffset);

      GetMibCache().SetMibAttributes(cellDn,attrs,ENTITY_OAM_REM);

      numLteRemCells++;
      numOfCellsAttrs.AddAttribute(PARAM_ID_LTE_NUM_REM_ENTRIES, numLteRemCells);
      GetMibCache().SetMibAttributes(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),
            numOfCellsAttrs, ENTITY_OAM_REM);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] -bCellIndex      : Cell Index
//				stWcdmaCellInfo : instance of structure __stWcdmaCellInfo
//
// This function WCDMA Result Parameters to MIB Cache
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::setWcdmaResultInMIB(int bCellIndex,__stWcdmaCellInfo & stWcdmaCellInfo)
{
   u32 numWcdmaRemCells = 0;
   MibAttributeValues numOfCellsAttrs;

    GetMibCache().GetMibAttribute(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_UMTS_WCDMA),
          PARAM_ID_NUM_CELL_ENTRIES, numWcdmaRemCells);

   if(bCellIndex < NUM_UMTS_SCANNED_CELL)
   {
#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"CRemApplication::setWcdmaResultInMIB");
#endif

   AddUmtsCell(stWcdmaCellInfo.uiUARFCNDL, stWcdmaCellInfo.uiPsc);
   MibDN cellDn = GetUmtsCellDn(stWcdmaCellInfo.uiUARFCNDL, stWcdmaCellInfo.uiPsc);

	MibAttributeValues attrs;
   attrs.AddAttribute(PARAM_ID_UARFCN_DL, stWcdmaCellInfo.uiUARFCNDL);
   attrs.AddAttribute(PARAM_ID_CPICH_RSCP, stWcdmaCellInfo.iRSCP);
   attrs.AddAttribute(PARAM_ID_CPICH_ECNO, stWcdmaCellInfo.iEcNo);
   attrs.AddAttribute(PARAM_ID_RSSI, stWcdmaCellInfo.iRSSI);
   attrs.AddAttribute(PARAM_ID_PRIM_SC, stWcdmaCellInfo.uiPsc);
   attrs.AddAttribute(PARAM_ID_PLMN_TYPE, stWcdmaCellInfo.stBcch.sPlmnType);
   attrs.AddAttribute(PARAM_ID_PLMN_ID, stWcdmaCellInfo.stBcch.szPLMN);
   attrs.AddAttribute(PARAM_ID_LAC, static_cast<u32> (stWcdmaCellInfo.stBcch.uiLac));
   attrs.AddAttribute(PARAM_ID_RAC, static_cast<u32> (stWcdmaCellInfo.stBcch.uiRac));
   attrs.AddAttribute(PARAM_ID_CELL_IDENTITY, static_cast<u32> (stWcdmaCellInfo.stBcch.uiCellId));
      attrs.AddAttribute(PARAM_ID_PCPICH_TXPOWER, stWcdmaCellInfo.stBcch.iPcpichTxPwr);
   attrs.AddAttribute(PARAM_ID_CSG_INDICATOR, stWcdmaCellInfo.stBcch.bCsgInd);
   attrs.AddAttribute(PARAM_ID_CSG_ID, static_cast<u32> (stWcdmaCellInfo.stBcch.uiCsgIdentity));
	GetMibCache().SetMibAttributes(cellDn, attrs, ENTITY_OAM_REM);

    numWcdmaRemCells++;
    numOfCellsAttrs.AddAttribute(PARAM_ID_NUM_CELL_ENTRIES, numWcdmaRemCells);
    GetMibCache().SetMibAttributes(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_UMTS_WCDMA),
          numOfCellsAttrs, ENTITY_OAM_REM);
   }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//
// This function Updates the curent time to MIB , which indicates the last scan time
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::setLastScanTimeInMIB()
{
#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"CRemApplication::setLastScanTimeInMIB");
#endif
	MibAttributeValues attrs;

//	TimeWrap now;
//	attrs.AddAttribute(PARAM_ID_LTE_LAST_REM_SCAN_TIME, now.GetUnixTime() );	
//	GetMibCache().SetMibAttributesByDn(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),attrs, ENTITY_OAM_REM);

}

void CRemApplication::setLteRemDataInMIB(unsigned int numOfCells)
{
#ifdef DEBUG_PRINT
   remLog(REM_TRACE_INFO,"CRemApplication::setLteRemDataInMIB");
#endif
   TimeWrap now;
   MibAttributeValues attrs;
   attrs.AddAttribute(PARAM_ID_LTE_NUM_REM_ENTRIES, numOfCells);
   //attrs.AddAttribute(PARAM_ID_REM_LTE_LAST_SCAN_TIME, now.GetUnixTime());

   GetMibCache().SetMibAttributesByDn(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),attrs,ENTITY_OAM_REM);
}

/* FREQ SYNC STATR*/

void CRemApplication::setLteFreqSyncInfo(unsigned int pci, unsigned int earfcn)
{
   MibAttributeValues attrs;
   TRACE_PRINTF("*********** LTE FREQ SYNC VALUES TO BE UPDATED IN OAM ************* \n");
   attrs.AddAttribute(PARAM_ID_LTE_FREQ_SYNC_PCI, pci); 
   attrs.AddAttribute(PARAM_ID_LTE_FREQ_SYNC_EARFCN, earfcn); 
   GetMibCache().SetMibAttributesByDn(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),attrs, ENTITY_OAM_REM);
}
void CRemApplication::setWcdmaFreqSyncInfo(unsigned int pci, unsigned int earfcn)
{
   MibAttributeValues attrs;
   TRACE_PRINTF("*********** WCDMA FREQ SYNC VALUES TO BE UPDATED IN OAM ************* \n");
   //printAllVectorsVal();
   attrs.AddAttribute(PARAM_ID_WCDMA_FREQ_SYNC_PSC, pci); 
   attrs.AddAttribute(PARAM_ID_WCDMA_FREQ_SYNC_UARFCN, earfcn); 
   GetMibCache().SetMibAttributesByDn(MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),attrs, ENTITY_OAM_REM);
}
/* FREQ SYNC END*/


///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in]
//
// This function gets the configuration parameters from MIB
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemApplication::GetSubscriptions(std::vector<SubscriptionDescriptor>& subscriptions)
{
#ifdef DEBUG_PRINT
   remLog(REM_TRACE_INFO,"CRemApplication::GetSubscriptions");
#endif

   MibDN fapDn = MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP, 0);
   MibDN fapLteDn = MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE);

   MibAttributeIds mibLteScanTimeOut;
   mibLteScanTimeOut.insert(PARAM_ID_LTE_SCAN_TIMEOUT);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_LTE_SCANTIMEOUT, ENTITY_OAM_REM, fapLteDn, mibLteScanTimeOut);

   MibAttributeIds mibLteScanOnBoot;
   mibLteScanOnBoot.insert(PARAM_ID_LTE_REM_SCAN_ON_BOOT);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_LTE_REM_SCANONBOOT, ENTITY_OAM_REM, fapLteDn, mibLteScanOnBoot);

   MibAttributeIds mibLteMaxCellEntries;
   mibLteMaxCellEntries.insert(PARAM_ID_LTE_MAX_REM_ENTRIES);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_LTE_MAXCELL, ENTITY_OAM_REM, fapLteDn, mibLteMaxCellEntries);

   MibAttributeIds mibLtePlmnParams;
   mibLtePlmnParams.insert(PARAM_ID_LTE_REM_SCAN_PLMN_LIST);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_LTE_PLMN, ENTITY_OAM_REM, fapLteDn, mibLtePlmnParams);

   MibAttributeIds mibLteBandParams;
   mibLteBandParams.insert(PARAM_ID_LTE_REM_SCAN_BAND_LIST);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_LTE_BAND, ENTITY_OAM_REM, fapLteDn, mibLteBandParams);

   MibAttributeIds mibLogUploadParams;
   mibLogUploadParams.insert(PARAM_ID_LOG_UPLOAD_INTERVAL);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_LOG_UPLOAD, ENTITY_OAM_REM,fapDn, mibLogUploadParams);
#ifdef LTE_TDD
   MibAttributeIds mibLteSubframeParams;
   mibLteSubframeParams.insert(PARAM_ID_LTE_TDD_SUBFRAME_ASSIGNMENT);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_LTE_SUBFRAME_ASSGN, ENTITY_OAM_REM, fapLteDn, mibLteSubframeParams);

   MibAttributeIds mibLteSpecialSubframeParams;
   mibLteSpecialSubframeParams.insert(PARAM_ID_LTE_TDD_SPECIAL_SUB_FRAME_PATTERNS);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_LTE_SPECIAL_SUBFRAME_PATTERN, ENTITY_OAM_REM, fapLteDn, mibLteSpecialSubframeParams);

#endif
   u32 remMioSubs = REM_SUBSCRIPTION_LTE_MIO;
   for(u32 rem_lte=0;rem_lte < MAX_NEIGHBOR_CELLS ; rem_lte++)
    {

      MibAttributeIds mibRemMioParams;
      mibRemMioParams.insert(PARAM_ID_LTE_REM_SCANNED_DL_BANDWIDTH);

      MibDN remLteMO = MibDN::GenerateMibDN(MIB_OBJECT_CLASS_LTE_REM_SCANNED_CELL,rem_lte);
      AddSubscription(subscriptions, remMioSubs, ENTITY_OAM_REM,remLteMO, mibRemMioParams);
      remMioSubs++;
    }
   
    /* Nfft Changes */
   MibAttributeIds mibLteDlBwParams;
   mibLteDlBwParams.insert(PARAM_ID_LTE_DL_BANDWIDTH);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_LTE_DL_BANDWIDTH, ENTITY_OAM_REM, fapLteDn, mibLteDlBwParams);
   
   MibAttributeIds mibLteCnmParams;
   mibLteCnmParams.insert(PARAM_ID_LTE_CNM_ENABLE);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_LTE_CNM, ENTITY_OAM_REM, fapLteDn, mibLteCnmParams);

   
   MibAttributeIds mibLteEarfcnParams;
   mibLteEarfcnParams.insert(PARAM_ID_LTE_REM_SCAN_ARFCN_LIST);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_LTE_EARFCN, ENTITY_OAM_REM, fapLteDn, mibLteEarfcnParams);

   MibAttributeIds mibLteParams;
   mibLteParams.insert(PARAM_ID_LTE_LAST_REM_SCAN_TIME);
   mibLteParams.insert(PARAM_ID_LTE_IN_USE_MAX_LTE_CELL); 
   mibLteParams.insert(PARAM_ID_LTE_NUM_EUTRA_NEIGH_CELLS_IN_USE_ENTRIES);

   mibLteParams.insert(PARAM_ID_LTE_OAM_CELL_RESERVE_FOR_OPERATOR);
   /* CSG_DEV start */
   mibLteParams.insert(PARAM_ID_LTE_NEIGH_IN_USE_CSG_INDICATOR);
   mibLteParams.insert(PARAM_ID_LTE_NEIGH_IN_USE_CSG_ID);
   /* CSG_DEV end */
   
   /* RSSI Changes */
   mibLteParams.insert(PARAM_ID_LTE_REM_NUM_EARFCN);
   mibLteParams.insert(PARAM_ID_LTE_REM_RSSI);
   mibLteParams.insert(PARAM_ID_LTE_REM_EARFCNDL);
   /* RSSI Changes */

   AddSubscription(subscriptions, REM_SUBSCRIPTION_LTE_PARAMS, ENTITY_OAM_REM, fapLteDn, mibLteParams);

   //WCDMA REM: Adding subscription of WCDMA REM attributes
   MibDN fapRemUmtsDn = MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_UMTS_WCDMA);

   MibAttributeIds mibInServHandl;
   mibInServHandl.insert(PARAM_ID_IN_SERVICE_HANDLING);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_INSERV_HANDL, ENTITY_OAM_REM, fapRemUmtsDn, mibInServHandl);

   MibAttributeIds mibRemScanOnBoot;
   mibRemScanOnBoot.insert(PARAM_ID_WCDMA_REM_SCAN_ON_BOOT);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_WCDMA_REM_SCANONBOOT, ENTITY_OAM_REM, fapRemUmtsDn, mibRemScanOnBoot);

   MibAttributeIds mibPeriodicScanMode;
   mibPeriodicScanMode.insert(PARAM_ID_PERIODIC_SCAN_MODE);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_PERIODIC_SCANMODE, ENTITY_OAM_REM, fapRemUmtsDn, mibPeriodicScanMode);

   MibAttributeIds mibPeriodicInterval;
   mibPeriodicInterval.insert(PARAM_ID_PERIODIC_INTERVAL);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_PERIODIC_INTERVAL, ENTITY_OAM_REM, fapRemUmtsDn, mibPeriodicInterval);

   MibAttributeIds mibPeriodicTime;
   mibPeriodicTime.insert(PARAM_ID_PERIODIC_TIME);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_PERIODIC_TIME, ENTITY_OAM_REM, fapRemUmtsDn, mibPeriodicTime);

   MibAttributeIds mibWcdmaPlmnLst;
   mibWcdmaPlmnLst.insert(PARAM_ID_WCDMA_REM_SCAN_PLMN_LIST);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_WCDMA_SCAN_PLMNLIST, ENTITY_OAM_REM, fapRemUmtsDn, mibWcdmaPlmnLst);

   MibAttributeIds mibUmtsBandLst;
   mibUmtsBandLst.insert(PARAM_ID_UMTS_REM_SCAN_BAND_LIST);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_UMTS_SCAN_BANDLIST, ENTITY_OAM_REM, fapRemUmtsDn, mibUmtsBandLst);

   MibAttributeIds mibUmtsUarfcnLst;
   mibUmtsUarfcnLst.insert(PARAM_ID_REM_SCAN_UARFCN_LIST);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_UMTS_SCAN_UARFCNLIST, ENTITY_OAM_REM, fapRemUmtsDn, mibUmtsUarfcnLst);

   MibAttributeIds mibWcdmaScanTimeOut;
   mibWcdmaScanTimeOut.insert(PARAM_ID_WCDMA_SCAN_TIMEOUT);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_WCDMA_SCANTIMEOUT, ENTITY_OAM_REM, fapRemUmtsDn, mibWcdmaScanTimeOut);

   MibAttributeIds mibWcdmaScanStat;
   mibWcdmaScanStat.insert(PARAM_ID_WCDMA_REM_SCAN_STATUS);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_WCDMA_SCANSTATUS, ENTITY_OAM_REM, fapRemUmtsDn, mibWcdmaScanStat);

   MibAttributeIds mibErrorDetails;
   mibErrorDetails.insert(PARAM_ID_ERROR_DETAILS);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_ERROR_DETAILS, ENTITY_OAM_REM, fapRemUmtsDn, mibErrorDetails);

   MibAttributeIds mibLastScanTime;
   mibLastScanTime.insert(PARAM_ID_WCDMA_LAST_REM_SCAN_TIME);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_WCDMA_LAST_SCANTIME, ENTITY_OAM_REM, fapRemUmtsDn, mibLastScanTime);

   MibAttributeIds mibMaxCellEntries;
   mibMaxCellEntries.insert(PARAM_ID_MAX_CELL_ENTRIES);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_WCDMA_MAXCELL, ENTITY_OAM_REM, fapRemUmtsDn, mibMaxCellEntries);

   MibAttributeIds mibNumOfCellEntry;
   mibNumOfCellEntry.insert(PARAM_ID_NUM_CELL_ENTRIES);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_WCDMA_NUMOFCELL, ENTITY_OAM_REM, fapRemUmtsDn, mibNumOfCellEntry);

#ifdef LTE_QCOM
   MibAttributeIds mibRfCfgParam;
   mibRfCfgParam.insert(PARAM_ID_BANDS_SUPPORTED);
   mibRfCfgParam.insert(PARAM_ID_SUPPORTED_SYSTEMS);
   AddSubscription(subscriptions, REM_SUBSCRIPTION_RF_CONFIG_PARAM, ENTITY_OAM_REM, fapDn, mibRfCfgParam);
#endif
}
void CRemApplication::HandleLogUpload()
{
   TRACE_PRINTF("%s\n",__FUNCTION__);
   u32 timer_value=0;
   GetMibCache().GetMibAttribute((MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP)), PARAM_ID_LOG_UPLOAD_INTERVAL,timer_value);

   if(IsTimerRunning(m_logTimerHandle))
   {
      DeleteTimer(m_logTimerHandle);
   }
   if(timer_value > 0)
   {
      CRemApplication::GetInstance().StartLogTimer(timer_value * 60000);
   }
   else
   {
      TRACE_PRINTF("\nLOG UPLOAD FUNCTIONALITY IS DISABLED");
   }
}

void CRemApplication::StartLogTimer(u32 timer_value)
{
   m_logTimerHandle = CreateTimer((const std::string&)"LOG expiration timeout", timer_value ,TimerEngineInterface::TIMER_START_NOW,
         TimerEngineInterface::TIMER_SINGLE_SHOT);
}
#endif



