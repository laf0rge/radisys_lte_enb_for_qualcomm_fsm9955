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

     Desc:     This file consists of the Interface(OAM) related functions for REM-APP

     File:     rem_interface.cpp

     Sid:      rem_interface.cpp@@

     Prg:      Vikram K
      	  	   Ambika S
      	  	   Sushant K


**************************************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "rem_datatype.h"
#include "rem_interface.h"
#include "rem_fsm.h"
#include "nmm_msgs.h"
#ifndef LTE_QCOM
#include "nmmcl_phyinterface.h"
#else
#ifdef __cplusplus
extern "C" {
#include "nl_rem_interface.h"
}
#endif
#endif
#include "rem_wcdma_event_handler.h"
#include "rem_eventhandler.h"
#include "rem_lte_freq_sync_event_handler.h"
#ifdef USE_RADISYS_OAM
#include "rem_application.h"
#include "Trace.h"
#endif

bool g_debugModeFlag = false;

void remStartTimer(U8 bTimerType, U16 scanTimeout);
void remStopTimer(U8 bTimerType);
void remStartCarrierExprScan(U8 bEARFCNCount, U16* usEARFCNList, U16 scanTimeout);
void remStartPCIExpressScan(U8 bPCICount, U16* usPCIList, U16 scanTimeout);
void remSendScanStatus(U8 bScanType, __enRemScanStatus bScanStatus, std::string & strError);
void remSendLteScanResult(std::list<__stLteCellInfo> & tCellList);
#ifndef LTE_QCOM
void remSendWcdmaScanResult(std::list<__stWcdmaCellInfo> & tCellList);
#endif
U8 remCmdLineParsing(int argc,char **argv);
void printHelpUsage();

extern "C" {
S16 SInit();
S16 tst();
S16 rdConQ(U8 data);
/*we are reading memory configuration from file for rem binary,
  hence we are assigning the msArgc and msArgv value accordingly*/
S16 msArgc;
Txt **msArgv;
}

S16 tst()
{
   return 0;
}

S16 rdConQ(U8 data)
{
   return 0;
}

void (*fpStartTimer)(U8 bTimerType, U16 usScanTimeout);
void (*fpStopTimer)(U8 bTimerType);
//void (*fpSendScanStatus)(U8 bScanType, __enRemScanStatus bScanStatus, std::string & strError);
//void (*fpSendLteScanResult)(std::list<__stLteCellInfo> & m_tCellList);

#ifdef DEBUG_PRINT
void remTestSendScanStatus();
void remTestSendLteResult();
#endif
 
///////////////////////////////////////////////////////////////////////////////
// Extern declaration to namespace "remfsm"
///////////////////////////////////////////////////////////////////////////////
namespace remfsm
{
    extern std::list <CRemEvtHandler*> gRATList;
    extern std::vector<int> g_tvEvtStatistics;
}

static struct option long_options[] =
{
   /* These options don't set a flag.
   We distinguish them by their indices. */
   {"arfcn-list", required_argument,   0, 'a'},
   {"band-list",  required_argument,   0, 'b'},
   {"plmn-list",  required_argument,   0, 'p'},
   {"start-scan", no_argument,         0, 's'},
   {"time-out",   required_argument,   0, 't'},
   {"debug-mode", no_argument,         0, 'd'},
   {"lte-mode",   no_argument,         0, 'l'},
   {"wcdma-mode", no_argument,         0, 'w'},
   {"help",       no_argument,         0 ,'h'},
   {0, 0, 0, 0}
};


///////////////////////////////////////////////////////////////////////////////
// array of strings that will be displayed against the event statistic(count)
///////////////////////////////////////////////////////////////////////////////
const char *g_strEvent[MAX_REM_APP_EVENTS] = {
		"REM_APP_INVALID_EVT",

		"REM_SCAN_TIMEOUT_EVT",     // COMMON
		"REM_ABORT_SCAN_EVT",		// COMMON
		"REM_RSSI_TIMEOUT_EVT",		// COMMON

		"REM_START_SCAN_EVT",             // REM_STATE_INIT
		"REM_START_CARRIER_EXPSCAN_EVT",  // REM_STATE_INIT
		"REM_START_PCI_EXPSCAN_EVT",      // REM_STATE_INIT
		"REM_NMM_START_RSP_EVT",          // REM_STATE_INIT
		"REM_NMMSTOP_RSP_EVT",			// REM_STATE_INIT

		"REM_RSSI_MEAS_RSP_EVT",  // REM_STATE_MEAS_CONFIG
		"REM_RSSI_MEAS_IND_EVT",  // REM_STATE_MEAS_CONFIG
		"REM_RSSI_MEAS_CNF_EVT",  // REM_STATE_MEAS_CONFIG
		"REM_STOP_RSSI_RSP_EVT",  // REM_STATE_MEAS_CONFIG

		"REM_CELL_SEARCH_RSP_EVT", 		// REM_STATE_CELL_CONFIG
		"REM_CELL_SEARCH_IND_EVT", 		// REM_STATE_CELL_CONFIG
		"REM_CELL_SEARCH_CNF_EVT", 		// REM_STATE_CELL_CONFIG
		"REM_STOP_CELL_SEARCH_RSP_EVT", 	// REM_STATE_CELL_CONFIG

		"REM_PBCH_CONFIG_CNF_EVT",	// REM_STATE_PBCH_CONFIG
		"REM_PBCH_DATA_IND_EVT",		// REM_STATE_PBCH_CONFIG

		"REM_SIB1_CONFIG_CNF_EVT",	// REM_STATE_SIB1_CONFIG

		"REM_BCCH_CONFIG_CNF_EVT",	// REM_STATE_BCCH_CONFIG
		"REM_BCCH_DATA_IND_EVT",		// REM_STATE_BCCH_CONFIG
		"REM_STOP_PBCH_RSP_EVT",	// REM_STATE_BCCH_CONFIG
		"REM_STOP_BCCH_RSP_EVT",	// REM_STATE_BCCH_CONFIG
      "REM_READ_SIB_TIMEOUT_EVT", // REM_STATE_BCCH_CONFIG
};

const char *g_strState[MAX_REM_APP_STATES] = {
		"REM_STATE_INIT",
		"REM_STATE_MEAS_CONFIG",
		"REM_STATE_CELL_CONFIG",
		"REM_STATE_PBCH_CONFIG",
		"REM_STATE_SIB1_CONFIG",
		"REM_STATE_BCCH_CONFIG",
                 "REM_STATE_WAIT_NMM_STOP",
                 "REM_STATE_WAIT_NMM_ABORT"
#ifdef ENABLE_CNM
       ,
      "REM_STATE_CNM"
#endif
};

U8 remCmdLineParsing(int argc,char **argv)
{
	/* getopt_long stores the option index here */
	int c, option_index = 0;
	bool bStartScan = false;
	U8 bScanType = 0;
	__stLteConfigData stLteCfgData;
#ifndef LTE_QCOM
	__stWcdmaConfigData stWcdmaCfgData;
	stWcdmaCfgData.usScanTimeOut = 600;
#endif

	// Thse are default arguments, vkorde, 14/11/2012
	stLteCfgData.usScanTimeOut = 600;

	while(  (c = getopt_long(argc, argv, "hsa:b:p:t:d:l:w:", long_options,&option_index)) != -1 )
	{
      switch(c)
       {
           case 's':
                bStartScan = 1;
                break;
           case 'a':
                std::cout<<"option -a with value %s"<< optarg << std::endl;
                stLteCfgData.strNEarfcnList = optarg;
                break;
           case 'b':
                std::cout<<"option -b with value %s"<< optarg << std::endl;
                stLteCfgData.strNBandList = optarg;
                break;
           case 'p':
                std::cout<<"option -p with value %s"<< optarg << std::endl;
                stLteCfgData.strPlmnList = optarg;
                break;
           case 't':
                stLteCfgData.usScanTimeOut = atoi(optarg);
                break;
           case 'h':
                printHelpUsage();
                break;
           case 'd':
                std::cout<<"Starting rem module in Debugging mode"<< std::endl;
                g_debugModeFlag = true;
                break;
           case 'l':
                std::cout<<"Starting rem module in LTE mode"<< std::endl;
                bScanType = NMM_SUPPORTED_RAT_LTE;
                break;
           case 'w':
                std::cout<<"Starting rem module in WCDMA mode"<< std::endl;
                bScanType = NMM_SUPPORTED_RAT_WCDMA;
                break;
           default:
                printHelpUsage();
                break;  
       }
    }

   if(g_debugModeFlag)
   {
      
   }

   if (bStartScan)
   {
      TRACE_PRINTF("--------------------------------------------------------\n");
      TRACE_PRINTF("-------REM SCAN INITIATED WITH CMD LINE ----------------\n");

      if( !stLteCfgData.strNBandList.empty() )
         TRACE_PRINTF("BAND LIST: %s\n", stLteCfgData.strNBandList.c_str());

      if( !stLteCfgData.strNEarfcnList.empty() )
         TRACE_PRINTF("EARFCN LIST: %s\n", stLteCfgData.strNEarfcnList.c_str());

      if( !stLteCfgData.strPlmnList.empty())
         TRACE_PRINTF("PLMN LIST: %s\n", stLteCfgData.strPlmnList.c_str());

      TRACE_PRINTF("Scan Timeout: %u\n", stLteCfgData.usScanTimeOut);

      TRACE_PRINTF("--------------------------------------------------------\n");

#ifdef USE_RADISYS_OAM
      CRemApplication::GetInstance().setScanOnStart(bStartScan);
#endif
      remSaveLteConfigData(stLteCfgData);

   }

   if (optind < argc)
   {
        std::cout << "non-option ARGV-elements: ";
        while (optind < argc)
           std::cout<<"%s "<<argv[optind++];
        putchar ('\n');
    }
	return bScanType;
	
}

void printHelpUsage()
{
   std::cout<<"rem [--band-list or -b <comma separated list without spaces> ]";
   std::cout<<"\trem [--arfcn-list or -a <comma separated list without spaces> ]";
   std::cout<<"\trem [--plmn-list or -p <comma separated list without spaces>]";
   std::cout<<"\trem [--start-scan or -s ]";
   std::cout<<"\trem [--time-out or -t<timeout value in seconds>]";
   std::cout<<"\trem [-d < developer option to run rem in debugging mode >]";
   std::cout<<"\trem [-l < run rem in LTE mode >]";
   std::cout<<"\trem [-w < run rem in WCDMA mode >]";
   std::cout<< std::endl;
}


#ifdef USE_RADISYS_OAM
///////////////////////////////////////////////////////////////////////////////
// @ param[in] : argc , argv[]
//
// This is the main / Application Entry Function
//
///////////////////////////////////////////////////////////////////////////////
int main (int argc, char **argv)
{

#ifdef LTE_QCOM
   /* NL client creation and registration to the NLservice */
   nl_ret_type                ret;

   memset( &g_remclnt, 0, sizeof( g_remclnt ) );
   /* Initialize the request semaphore */
   if ( sem_init( &g_remclnt.req_sem, 0, 0 ) != 0 )
   {
      printf( "NL_REM::ERROR: Failed to initialize req semaphore!\n" );
   }
#ifdef ENABLE_REM_LOGS
   printf( "NL_REM::Connecting to the NL QMI Service...\n" );
#endif

   /* Connect to the NL QMI Service. The following call waits for the NL QMI
      Service to come up and connects to the service once it is available. */
   ret = rem_nl_qmi_clnt_create( nl_get_service_object_v01(),
                                 &g_remclnt.clnt_handle,
                                 &g_remclnt.os_params,
                                 rem_ind_cb,
                                 rem_err_cb );
   if ( ret != NL_SUCCESS )
   {
      printf( "NL_REM::ERROR: Failed to create NL client!\n" );
      return -1;
   }

#ifdef ENABLE_REM_LOGS
   printf( "NL_REM::Initializing NL...\n" );
#endif

   /* End of NL code changes*/ 
#endif
   /*check whether rem memory configuration file present or not*/
   FILE *file;
   char *argvForSSI[50];
   if((file = fopen("../config/ssi_rem", "r")))
   {
      fclose(file);
      argvForSSI[0]= argv[0];
      argvForSSI[1] = (char *)"-f";
      argvForSSI[2] = (char *)"../config/ssi_rem";
      msArgc = 3;
      msArgv = argvForSSI;
   }
   remStartApp(remCmdLineParsing(argc,argv)); 

#ifdef USE_RADISYS_OAM
   return(CRemApplication::GetInstance().Run(argc, argv));
#endif
}

#endif

///////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function calls the Initialization functions
//
///////////////////////////////////////////////////////////////////////////////
void remStartApp(U8 bScanType)
{
#ifdef ENABLE_REM_LOGS
   printf("REM::Entered in remStartApp\n");
#endif
	SInit();
   	initFsm();
#ifndef LTE_QCOM
	initNmmLayer();
    printf("REM::Initialized Nmm layer\n");
#endif
#ifdef USE_RADISYS_OAM
#ifdef ENABLE_REM_LOGS
      printf("REM::sending msg to OAM\n");
#endif
	fpStartTimer = remStartTimer;
	fpStopTimer = remStopTimer;
   //	fpSendScanStatus = remSendScanStatus;
   //	fpSendLteScanResult = remSendLteScanResult;

#endif
	if( bScanType )
   {
      printf("REM::started REM scan bscantype is %d\n",bScanType);
		remStartScan(bScanType);
    }
}

bool remIsRatEventHandlerAlreadyPresent(__enSupportedRAT rat)
{
   std::list <CRemEvtHandler*>::iterator it;
   for(it = remfsm::gRATList.begin(); it!= remfsm::gRATList.end(); it++)
   {
      if((*it)->getRATType() == rat)
      {
         return true;
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////
// @ param[in] : bScanType - Indicates the type of RAT 
//                           for which scanning is to be done
// This function calls the Initialization functions
//
///////////////////////////////////////////////////////////////////////////////
void remStartScan(U8 bScanType)
{
	std::string strError;
	bool bStartScan=false;

	if( bScanType == 0 )
	{
		strError = "ERROR: Invalid Scan Type Received !!";
		remSendScanStatus(RAT_LTE_FREQ_SYNC, REM_ERROR, strError);
		return;
	}

	getFsmLock();

   if(bScanType & NMM_SUPPORTED_RAT_FREQ_SYNC)
   {
      TRACE_PRINTF("Configuring LTE Freq Synch Sniffing .....\n");

      if(remIsRatEventHandlerAlreadyPresent(RAT_LTE_FREQ_SYNC))
      {
			strError = "ERROR: LTE Freq Synch Scan is already in progress !!";
			remSendScanStatus(RAT_LTE_FREQ_SYNC, REM_IN_PROGRES, strError);
			releaseFsmLock();
			return;
		}
            
      remfsm::gRATList.push_back (new CLteFreqSyncEvtHandler()); 
   }
   
	if( bScanType & NMM_SUPPORTED_RAT_LTE )
	{
      TRACE_PRINTF("Configuring LTE Neighbor Sniffing .....\n");

      if(remIsRatEventHandlerAlreadyPresent(RAT_LTE))
      {
			strError = "ERROR: LTE Scan is already in progress !!";
			remSendScanStatus(RAT_LTE, REM_IN_PROGRES, strError);
			releaseFsmLock();
			return;
		}

		bStartScan = true;
		remfsm::gRATList.push_back (new CLteEvtHandler());

      /* Updating Scan Status to OAM */
      strError = "INFO : Scan is in progress !!";
      remSendScanStatus(RAT_LTE, REM_IN_PROGRES, strError);      
#ifndef LTE_QCOM
	   CRemWcdmaConfigData & oWcdmaCfg = CRemWcdmaConfigData::getInstance();
      if(oWcdmaCfg.getScanOnBootInfo())
      {
         bScanType = NMM_SUPPORTED_RAT_WCDMA;
      }
#endif
	}

   if( bScanType & NMM_SUPPORTED_RAT_WCDMA)
   {
      TRACE_PRINTF("Configuring UMTS Neighbor Sniffing .....\n");

      if(remIsRatEventHandlerAlreadyPresent(RAT_WCDMA))
      {
         strError = "3G Sniffing is allready in progress !!";
		remSendScanStatus(RAT_WCDMA, REM_ERROR, strError);

         if(bStartScan == false) // means that we are not doing any other scan
         {
			releaseFsmLock();
			return;
		}
      }

      bStartScan = true;
#ifndef LTE_QCOM
      remfsm::gRATList.push_back (new CWcdmaEvtHandler());
      /* Updating Scan Status to OAM */
      strError = "INFO : Scan is in progress !!";
      remSendScanStatus(RAT_WCDMA, REM_IN_PROGRES, strError);       
#endif
	}

	if( bScanType & NMM_SUPPORTED_RAT_GERAN) {
		strError = "ERROR: GERAN Sniffing not supported !!";
		remSendScanStatus(RAT_GERAN, REM_ERROR, strError);
		if ( bStartScan == false ) {
			releaseFsmLock();
			return;
		}
	}

	if( bScanType & NMM_SUPPORTED_RAT_CDMA) {
		strError = "ERROR: CDMA Sniffing not supported !!";
		remSendScanStatus(RAT_CDMA, REM_ERROR, strError);
		if ( bStartScan == false ) {
			releaseFsmLock();
			return;
		}
	}

	releaseFsmLock();

	remHandleEvent(REM_START_SCAN_EVT, (void*)NULL);
}

///////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function handles the timer expiry event
//
///////////////////////////////////////////////////////////////////////////////
void remScanTimerExpired(U8 bTimerType)
{
	time_t currTime;

	time(&currTime);
   TRACE_PRINTF("Timer [%s] Expired @ %s",g_strEvent[bTimerType], ctime(&currTime));

	remHandleEvent((__enREMAppEvent)bTimerType, (void*)NULL);
}

///////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function handles Abort Scan Command from OAM
//
///////////////////////////////////////////////////////////////////////////////
void remAbortScan()
{
	#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO, "Aborting Scan");
	#endif
	remHandleEvent(REM_ABORT_SCAN_EVT, (void*)NULL);
}

///////////////////////////////////////////////////////////////////////////////
// @ param[in] :bEARFCNCount - EARFCN count
//				usEARFCNList - List of Earfcns
//				scanTimeout  - Scan Time Out
//
// This function handles Incoming SON_START_CARRIER_EXP_SCAN command
//
///////////////////////////////////////////////////////////////////////////////
void remStartCarrierExprScan
(
 U8 bEARFCNCount,
 U16* usEARFCNList,
 U16 scanTimeout
 )
{
	CLteEvtHandler* pObjLteEvtHdlr = new CLteEvtHandler(scanTimeout);

	pObjLteEvtHdlr->saveCarrierList(bEARFCNCount, usEARFCNList);

	getFsmLock();
	remfsm::gRATList.push_back(pObjLteEvtHdlr);
	releaseFsmLock();
#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO, "Start Carrier Express Scan");
	#endif
	remHandleEvent(REM_START_SCAN_EVT,  (void*)NULL);
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] :bPCICount - PCI count
//				usPCIList - List of PCIs
//				scanTimeout  - Scan Time Out
//
// This function handles Incoming SON_START_PCI_EXP_SCAN command
//
////////////////////////////////////////////////////////////////////////////////
void remStartPCIExpressScan(U8 bPCICount, U16* usPCIList, U16 scanTimeout)
{
	CLteEvtHandler* pObjLteEvtHdlr = new CLteEvtHandler(scanTimeout);

	pObjLteEvtHdlr->savePciList(bPCICount, usPCIList);

	getFsmLock();
	remfsm::gRATList.push_back(pObjLteEvtHdlr);
	releaseFsmLock();

	#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO, "Start PCI Express Scan");
	#endif
	remHandleEvent(REM_START_SCAN_EVT,  (void*)NULL);
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : scanTimeout  - Scan Time Out
//
// This function is responsible for starting the Scan Timer
//
////////////////////////////////////////////////////////////////////////////////
void remStartTimer(U8 bTimerType, U16 usScanTimeout)
{
	time_t currTime;

	time(&currTime);
   TRACE_PRINTF("Starting Timer [%s] Timeout:[%d] @ %s",
         g_strEvent[bTimerType],
         usScanTimeout,
         ctime(&currTime));

#ifdef USE_RADISYS_OAM
	CRemApplication::GetInstance().startScanTimer(bTimerType, usScanTimeout);
#endif
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function is responsible for stopping the Scan Timer
//
////////////////////////////////////////////////////////////////////////////////
void remStopTimer(U8 bTimerType)
{
#ifdef USE_RADISYS_OAM
	CRemApplication::GetInstance().stopScanTimer(bTimerType);
#endif

	time_t currTime;

	time(&currTime);
	TRACE_PRINTF("Stopped [%s] Timer @ %s",
                 g_strEvent[bTimerType],
                 ctime(&currTime));
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : bScanType - type of RAT for which scanning is done
//				 bScanStatus - Indicates the status - 0 :
//				 INDETERMINATE/PROGRESS/SUCCESS/ERROR/TIMEOUT
//				 strError - details reason when scan status is ERROR or TIMEOUT
// This function is responsible for sending the scan status message to OAM
//
////////////////////////////////////////////////////////////////////////////////
void remSendScanStatus
(
 U8 bScanType,
 __enRemScanStatus bScanStatus,
 std::string & strError
 )
{
   TRACE_PRINTF("remSendScanStatus: SCAN_TYPE: %d, SCAN_STATUS: %d, MSG: %s\n",
         bScanType,
         bScanStatus,
         strError.c_str());

#ifdef USE_RADISYS_OAM
	CRemScanStatus oRemScanStatus(strError,bScanType,bScanStatus);
	CRemApplication::GetInstance().sendMessageToOam(oRemScanStatus);
#else
#ifdef USE_C_INTERFACE
	(*fpSendScanStatusC)(bScanType, bScanStatus, strError.c_str());
#else
	(*fpSendScanStatus)(bScanType, bScanStatus, strError);
#endif
#endif
}


////////////////////////////////////////////////////////////////////////////////
// @ param[in] : 
//
// This function is responsible for setting LTE Last Scan Time in MIB
//
////////////////////////////////////////////////////////////////////////////////
void remSendLteLastScanTime()
{
#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO, "Setting Last Scan Time");
#endif

#ifdef USE_RADISYS_OAM
    CRemApplication::GetInstance().setLastScanTimeInMIB();
#endif
}
////////////////////////////////////////////////////////////////////////////////
// @ param[in] : tCellList - list of __stLteCellInfo
//
// This function is responsible for sending the LTE Scan result to MIB
//
////////////////////////////////////////////////////////////////////////////////
void remSendLteScanResult(std::list<__stLteCellInfo> & tCellList)
{
	std::list<__stLteCellInfo>::iterator it;

	unsigned int numOfCells = tCellList.size();
	TRACE_PRINTF("Number of LTE cells = %d\n", numOfCells);

#ifdef USE_RADISYS_OAM
   int iIndx;
   for(iIndx=0,it=tCellList.begin();it!=tCellList.end();it++,iIndx++)
   {
      CRemApplication::GetInstance().addLteCellInMIB(iIndx,*it);
   }

   CRemApplication::GetInstance().setLteRemDataInMIB(numOfCells);
#else /* FOR THIRD PARTY OAM */

#ifdef USE_C_INTERFACE
	__stLteCellInfo* pstCellInfo[64]; int iNum;
	for(iNum=0,it=tCellList.begin();it!=tCellList.end() && iNum<64;it++,iNum++)
		pstCellInfo[iNum] = &(*it)
	(*fpSendLteScanResultC)(iNum, pstCellInfo);
#else
	(*fpSendLteScanResult)(tCellList);
#endif

#endif
}

#if 1
////////////////////////////////////////////////////////////////////////////////
// @ param[in] : tCellList - list of __stWcdmaCellInfo
//
// This function is responsible for sending the WCDMA Scan result to MIB
//
////////////////////////////////////////////////////////////////////////////////
void remSendWcdmaScanResult(std::list<__stWcdmaCellInfo> & tCellList)
{
    std::list<__stWcdmaCellInfo>::iterator it;
	unsigned int numOfCells = tCellList.size();

	TRACE_PRINTF("Number of UMTS cells=%d\n", numOfCells);

#ifdef USE_RADISYS_OAM
	CRemApplication::GetInstance().setLastScanTimeInMIB();

    int iIndx;
    for(iIndx=0,it=tCellList.begin();it!=tCellList.end();it++,iIndx++)
    {
       CRemApplication::GetInstance().setWcdmaResultInMIB(iIndx,*it);
       //CRemApplication::GetInstance().setUtraNeighBourList(iIndx,*it,numOfCells);
    }

#endif
}
#endif


////////////////////////////////////////////////////////////////////////////////
// @ param[in] : stLteCfgData - instance of struct __stLteConfigData
//
// This function saves the LTE configuration data read from MIB
//
////////////////////////////////////////////////////////////////////////////////
void remSaveLteConfigData(__stLteConfigData & stLteCfgData)
{
	 CRemLteConfigData & oLteCfg = CRemLteConfigData::getInstance();

	 oLteCfg.savePlmnList(stLteCfgData.strPlmnList);
	 oLteCfg.saveBandList(stLteCfgData.strNBandList);
	 oLteCfg.saveEARFCNList(stLteCfgData.strNEarfcnList);

	 oLteCfg.saveScanTimeout(stLteCfgData.usScanTimeOut);
	 oLteCfg.saveMaxCellEntry(stLteCfgData.bMaxCellEntries);
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : stWcdmaCfgData - instance of struct __stWcdmaConfigData
//
// This function saves the WCDMA configuration data read from MIB
//
////////////////////////////////////////////////////////////////////////////////
void remSaveWcdmaConfigData(__stWcdmaConfigData & stWcdmaCfgData)
{
	CRemWcdmaConfigData & oWcdmaCfg = CRemWcdmaConfigData::getInstance();

	oWcdmaCfg.savePlmnList(stWcdmaCfgData.szPlmnList);
	oWcdmaCfg.saveBandList(stWcdmaCfgData.szNBandList);
	oWcdmaCfg.saveEARFCNList(stWcdmaCfgData.szNEarfcnList);

	oWcdmaCfg.saveScanTimeout(stWcdmaCfgData.usScanTimeOut);
	oWcdmaCfg.saveMaxCellEntry(stWcdmaCfgData.bMaxCellEntries);
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function is used for printing the number of times each event has occured
//
////////////////////////////////////////////////////////////////////////////////
void remPrintEvtStatistics()
{
	std::fill(remfsm::g_tvEvtStatistics.begin(),remfsm::g_tvEvtStatistics.end(),0);
	for(int i=0;i<MAX_REM_APP_EVENTS;i++)
	{
		if(remfsm::g_tvEvtStatistics[i] !=0 )
		{
			TRACE_PRINTF("Event      : %s\n", g_strEvent[i]);
			TRACE_PRINTF("Event count: %d\n", remfsm::g_tvEvtStatistics[i]);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function is used for sending Scan status to OAM for PAL Testing
//
////////////////////////////////////////////////////////////////////////////////
void remTestSendScanStatus()
{
	#ifdef USE_RADISYS_OAM
	std::string strError="PAL Testing : Sending Scan Status Message";
	U8 bScanType = 0;   //RAT_LTE
	U8 bScanStatus = 1; //REM_IN_PROGRESS
	CRemScanStatus oRemScanStatus(strError,bScanType,bScanStatus);
	CRemApplication::GetInstance().sendMessageToOam(oRemScanStatus);
	#endif
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function is used for sending LTE Scan result to OAM for PAL Testing
//
////////////////////////////////////////////////////////////////////////////////
void remTestSendLteResult()
{
 	std::list<__stLteCellInfo> tLteCellInfo;
	__stLteCellInfo stCellInfo;

	for(int i=0;i<2;i++)
	{
	stCellInfo.uiEARFCN = 2110;
	stCellInfo.uiPhyCellID = 2;
	stCellInfo.iRSRP = -93;
	stCellInfo.iRSRQ = 0;
	stCellInfo.iRSSI = -79;	 
	stCellInfo.bSib1Decoded =0;
	stCellInfo.bSib2Decoded =0;
	stCellInfo.bPLMNNotConfigured =0;
	stCellInfo.iSIBRetryCount= 3;
//	stCellInfo.stBcch.usDlBandwidth=50;
//	stCellInfo.stBcch.usUlBandwidth=3;
	stCellInfo.stBcch.iRSTxPower=1;
	stCellInfo.stBcch.uiTac=0;
	stCellInfo.stBcch.uiCellId=1;
	stCellInfo.stBcch.bCellBarred=1;
	stCellInfo.stBcch.bCsgInd=1;
	stCellInfo.stBcch.uiCsgIdentity=0;
	stCellInfo.stBcch.uiPlmnListNumOfEntries=1;
#ifdef USE_RADISYS_OAM
	CRemApplication::GetInstance().addLteCellInMIB(i,stCellInfo);
#endif
	}
}

