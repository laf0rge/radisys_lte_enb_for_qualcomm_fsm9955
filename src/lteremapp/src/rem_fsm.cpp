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

     Desc:     This file consists of the fsm functions for REM-APP

     File:     rem_fsm.cpp

     Sid:      rem_fsm.cpp@@/main/Tenb_Intg_Branch_CC/TeNB_Dev_REM_Branch/2 - Fri Jan 11 16:43:39 2013

     Prg:      Vikram K
      	  	   Ambika S


**************************************************************************************************/
#ifdef USE_RADISYS_OAM
#include <system/Trace.h>
#endif
#include <map>
#include <string>

#include "rem_datatype.h"
#include "rem_fsm.h"
#include "rem_eventhandler.h"
#include "nmmcl_interface.h"
#include "rem_interface.h"
#include "rem_lte_event_handler.h"

#ifdef LTE_QCOM
#include "nl_rem_decodeltemsgs.h"
extern "C"
{
#include "nl_rem_interface.h"
extern int g_sibRequest;
}
U8 g_qcomNmmStatus      = 0;
U8* g_QCOMStartFreqScan = &g_qcomNmmStatus;
extern bool g_sibError;
extern int g_sibResponse;
#endif

extern const char *g_strState[MAX_REM_APP_STATES];
extern const char *g_strEvent[MAX_REM_APP_EVENTS];
///////////////////////////////////////////////////////////////////////////////////////////////////
// Defining namespace "remfsm"
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace remfsm
{
	std::list <CRemEvtHandler*> gRATList;
	CRemEvtHandler *g_pRemEvtHdlr;
	int g_iCurrState = REM_STATE_INIT;
	__enRemScanStatus g_bScanStatus = REM_INDETERMINATE; // Last Scan Status
	std::string g_strRemError; // Last Scan Error
	std::map<U8 , std::string> g_tErrValue;
	int g_bScanHalted = false;
	std::vector<int> g_tvEvtStatistics;
	pthread_mutex_t g_fsmMutex = PTHREAD_MUTEX_INITIALIZER;
    int g_sib1ReqCount = 0;
    bool g_bSendNmmBcchCfgReq = false;
#ifdef ENABLE_CNM
   U16 g_cnmSlctdEarfcn;
   U16 g_cnmSlctdPci;
#endif
}

using namespace remfsm;

/* flag added to guard the on-going request at phy*/
bool g_reqSent = false;
bool g_ignorePhyMsg = false;
bool checkRemStateAndSndScan(void);

void remSendScanStatus(U8 bScanType, __enRemScanStatus bScanStatus, std::string & strError);
///////////////////////////////////////////////////////////////////////////////////////////////////
// Array of structure containing events and corresponding eventhandlers
///////////////////////////////////////////////////////////////////////////////////////////////////
__stRemFSM g_stRemFsm[MAX_REM_APP_STATES][MAX_REM_APP_EVENTS] =
{
    {
    		{ REM_START_SCAN_EVT,         handleNmmStartScan  		},	// REM_STATE_INIT
    		{ REM_NMM_START_RSP_EVT,    	handleNmmStartRsp   		},	// REM_STATE_INIT
    		{ REM_NMMSTOP_RSP_EVT,			handleNmmStopRspEvt			},	// REM_STATE_INIT
    		{ REM_SCAN_TIMEOUT_EVT,  	 	handleNmmTimerExpEvt		},	// REM_STATE_MEAS_CONFIG
    		{ REM_APP_INVALID_EVT,      	NULL		      			}	// REM_STATE_INIT
    },

    {
    		{ REM_RSSI_MEAS_RSP_EVT, 		handleRssiMeasRspEvt		},	// REM_STATE_MEAS_CONFIG
    		{ REM_RSSI_MEAS_IND_EVT, 		handleRssiMeasIndEvt 		},	// REM_STATE_MEAS_CONFIG
    		{ REM_RSSI_MEAS_CNF_EVT, 		handleRssiMeasCnfEvt 		},	// REM_STATE_MEAS_CONFIG
    		{ REM_STOP_RSSI_RSP_EVT, 		handleStopRssiRspEvt 		},	// REM_STATE_MEAS_CONFIG
    		{ REM_ABORT_SCAN_EVT,  	 		handleNmmAbortScan			},	// REM_STATE_MEAS_CONFIG
    		{ REM_SCAN_TIMEOUT_EVT,  	 	handleNmmTimerExpEvt		},	// REM_STATE_MEAS_CONFIG
    		{ REM_RSSI_TIMEOUT_EVT,			handleRssiTimeoutEvt		},	// REM_STATE_MEAS_CONFIG
    		{ REM_APP_INVALID_EVT,   		NULL						}	// REM_STATE_MEAS_CONFIG
    },

    {
    		{ REM_CELL_SEARCH_RSP_EVT, 		handleCellSearchRspEvt		},	// REM_STATE_CELL_CONFIG
    		{ REM_CELL_SEARCH_IND_EVT, 		handleCellSearchIndEvt 		},	// REM_STATE_CELL_CONFIG
    		{ REM_CELL_SEARCH_CNF_EVT, 		handleCellSearchCnfEvt 		},	// REM_STATE_CELL_CONFIG
    		{ REM_STOP_CELL_SEARCH_RSP_EVT, handleStopCellSearchRspEvt	},	// REM_STATE_CELL_CONFIG
    		{ REM_ABORT_SCAN_EVT,  	 		handleNmmAbortScan			},	// REM_STATE_CELL_CONFIG
    		{ REM_SCAN_TIMEOUT_EVT,  	 	handleNmmTimerExpEvt		},	// REM_STATE_CELL_CONFIG
    		{ REM_APP_INVALID_EVT,  		NULL						}	// REM_STATE_CELL_CONFIG
    },

    {
    		{ REM_PBCH_CONFIG_CNF_EVT, 		handlePbchConfigCnfEvt		},	// REM_STATE_PBCH_CONFIG
    		{ REM_PBCH_DATA_IND_EVT, 		handlePbchDataIndEvt 		},	// REM_STATE_PBCH_CONFIG
    		{ REM_STOP_PBCH_RSP_EVT, 		handleStopPBCHRspEvt 		},	// REM_STATE_PBCH_CONFIG
    		{ REM_ABORT_SCAN_EVT,  	 		handleNmmAbortScan			},	// REM_STATE_PBCH_CONFIG
    		{ REM_SCAN_TIMEOUT_EVT,  	 	handleNmmTimerExpEvt		},	// REM_STATE_PBCH_CONFIG
    		{ REM_APP_INVALID_EVT,  		NULL						}	// REM_STATE_PBCH_CONFIG
    },

    {
    		{ REM_SIB1_CONFIG_CNF_EVT, 		handleSIB1ConfigCnfEvt		},	// REM_STATE_SIB1_CONFIG
    		{ REM_ABORT_SCAN_EVT,  	 		handleNmmAbortScan			},	// REM_STATE_SIB1_CONFIG
    		{ REM_SCAN_TIMEOUT_EVT,  	 	handleNmmTimerExpEvt		},	// REM_STATE_SIB1_CONFIG
    		{ REM_APP_INVALID_EVT,  		NULL						}	// REM_STATE_SIB1_CONFIG
    },

    {
    		{ REM_BCCH_CONFIG_CNF_EVT, 	handleBCCHConfigCnfEvt     },	// REM_STATE_BCCH_CONFIG
    		{ REM_BCCH_DATA_IND_EVT, 		handleBCCHDataIndEvt       },	// REM_STATE_BCCH_CONFIG
    		{ REM_STOP_BCCH_RSP_EVT, 		handleStopBCCHRspEvt       },	// REM_STATE_BCCH_CONFIG
    		{ REM_ABORT_SCAN_EVT,  	 		handleNmmAbortScan         },	// REM_STATE_BCCH_CONFIG
    		{ REM_SCAN_TIMEOUT_EVT,  	 	handleNmmTimerExpEvt       },	// REM_STATE_BCCH_CONFIG
    		{ REM_READ_SIB_TIMEOUT_EVT,  	handleSibReadTimerExpEvt   },	// REM_STATE_BCCH_CONFIG
    		{ REM_CELL_SEARCH_RSP_EVT, 	handleNmmFreqSyncRspEvt    },	// REM_STATE_BCCH_CONFIG
    		{ REM_CELL_SEARCH_IND_EVT, 	handleNmmFreqSyncIndEvt    },	// REM_STATE_BCCH_CONFIG
    		{ REM_CELL_SEARCH_CNF_EVT, 	handleNmmFreqSyncCnfEvt    }, // REM_STATE_BCCH_CONFIG
    		{ REM_APP_INVALID_EVT,  		NULL						}	// REM_STATE_BCCH_CONFIG
    },/* CNM State handlers*/
#ifdef ENABLE_CNM
    {
    		{ REM_CELL_SEARCH_RSP_EVT, 		handleCnmCellSearchRspEvt		},	// REM_STATE_CELL_CONFIG
    		{ REM_CELL_SEARCH_IND_EVT, 		handleCnmCellSearchIndEvt 		},	// REM_STATE_CELL_CONFIG
    		{ REM_CELL_SEARCH_CNF_EVT, 		handleCnmCellSearchCnfEvt 		},	// REM_STATE_CELL_CONFIG
    		{ REM_PBCH_CONFIG_CNF_EVT, 		handleCnmPbchConfigCnfEvt		},	// REM_STATE_PBCH_CONFIG
    		{ REM_PBCH_DATA_IND_EVT, 		handleCnmPbchDataIndEvt 		},	// REM_STATE_PBCH_CONFIG
    		{ REM_STOP_PBCH_RSP_EVT, 		handleCnmStopPBCHRspEvt 		},	// REM_STATE_PBCH_CONFIG
    		{ REM_ABORT_SCAN_EVT,  	 		handleCnmAbortScan			},	// REM_STATE_CELL_CONFIG
    		{ REM_SCAN_TIMEOUT_EVT,  	 	handleCnmStopEvt		},	// REM_STATE_CELL_CONFIG
    		{ REM_NMMSTOP_RSP_EVT,			handleNmmStopRspEvt			},	// REM_STATE_INIT
    		{ REM_APP_INVALID_EVT,  		NULL						}	// REM_STATE_CELL_CONFIG
    },
#endif
    {
    		{ REM_NMM_START_RSP_EVT, 		handleNmmTimerExpEvt		},	// REM_STATE_WAIT_NMM_STOP 
    		{ REM_RSSI_MEAS_RSP_EVT, 		handleNmmTimerExpEvt    },	// REM_STATE_WAIT_NMM_STOP
    		{ REM_STOP_BCCH_RSP_EVT, 		handleNmmTimerExpEvt		},	// REM_STATE_WAIT_NMM_STOP
         { REM_RSSI_MEAS_IND_EVT, 		handleNmmTimerExpEvt	    },	// REM_STATE_WAIT_NMM_STOP
    		{ REM_SCAN_TIMEOUT_EVT,  	 	handleNmmTimerExpEvt		},	// REM_STATE_WAIT_NMM_STOP
    		{ REM_SIB1_CONFIG_CNF_EVT,  	handleNmmTimerExpEvt		},	// REM_STATE_WAIT_NMM_STOP
         { REM_STOP_PBCH_RSP_EVT,      handleNmmTimerExpEvt    },  // REM_STATE_WAIT_NMM_STOP
    	   { REM_CELL_SEARCH_RSP_EVT,    handleNmmTimerExpEvt    },
    	   { REM_CELL_SEARCH_IND_EVT,    handleNmmTimerExpEvt    },
    	   { REM_CELL_SEARCH_CNF_EVT,    handleNmmTimerExpEvt    },
    	   { REM_PBCH_CONFIG_CNF_EVT,    handleNmmTimerExpEvt    },
    	   { REM_PBCH_DATA_IND_EVT,      handleNmmTimerExpEvt    },
    	   { REM_BCCH_DATA_IND_EVT,      handleNmmTimerExpEvt    },
    		{ REM_SIB1_CONFIG_CNF_EVT,    handleNmmTimerExpEvt    },
    		{ REM_APP_INVALID_EVT,  		NULL				    }	// REM_STATE_WAIT_NMM_STOP
    },

    {
    		{ REM_NMM_START_RSP_EVT, 		handleNmmAbortScan		},	// REM_STATE_WAIT_NMM_ABORT 
    		{ REM_RSSI_MEAS_RSP_EVT, 		handleNmmAbortScan 		},	// REM_STATE_WAIT_NMM_ABORT
    		{ REM_STOP_BCCH_RSP_EVT, 		handleNmmAbortScan		},	// REM_STATE_WAIT_NMM_ABORT
            { REM_RSSI_MEAS_IND_EVT, 		handleNmmAbortScan	    },	// REM_STATE_WAIT_NMM_ABORT
    		{ REM_SCAN_TIMEOUT_EVT,  	 	handleNmmAbortScan		},	// REM_STATE_WAIT_NMM_ABORT
    		{ REM_SIB1_CONFIG_CNF_EVT,  	handleNmmAbortScan		},	// REM_STATE_WAIT_NMM_ABORT
            { REM_STOP_PBCH_RSP_EVT,        handleNmmAbortScan      },  // REM_STATE_WAIT_NMM_ABORT
    		{ REM_APP_INVALID_EVT,  		NULL				    }	// REM_STATE_WAIT_NMM_ABORT
    },
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : iLineNo - Line at which error occurs
//				 bErrorCode - Error code
// This function generated the error string which will be later sent to OAM
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void generateErrorString(int iLineNo,U8 bErrorCode)
{
	g_strRemError.clear();

	if (g_tErrValue.find(bErrorCode) == g_tErrValue.end())
	{
		g_strRemError = "Unknown Error String. ErrorCode=";
		char errCode[24]; sprintf(errCode, "%d", bErrorCode);
		g_strRemError += errCode;
		//TRACE_PRINTF_LEV(TRACE_INFO," Error in line : %d",iLineNo);
		return;
	}

	g_strRemError = g_tErrValue[bErrorCode];
	switch(bErrorCode)
	{
	 	 case NMM_INVALID_FREQ_BAND:
	 	 case NMM_FREQ_BAND_NOT_SUPPORTED_BY_HW:
            {
                std::ostringstream convertBand;
                convertBand << g_pRemEvtHdlr->getBand();
                g_strRemError += convertBand.str();
            }
 			break;
		 case NMM_INVALID_EARFCN:
		 {
			 std::ostringstream convertEarfcn;
			 if( g_iCurrState == REM_STATE_PBCH_CONFIG )
				 convertEarfcn << g_pRemEvtHdlr->getARFCN2();
			 else
				 convertEarfcn << g_pRemEvtHdlr->getARFCN();
			 g_strRemError += convertEarfcn.str();
		 }
		 break;
     }

	 //TRACE_PRINTF_LEV(TRACE_INFO," Error in line : %d",iLineNo);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : iLineNo - Line at which error occurs
//				 bErrorCode - Error code
//               bTimerExpired - boolean variable to indicate Timer Expiry
// This function halts the Scan procedure by sending NMM_STOP message
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int haltNmmScan(int iLineNo,U8 bErrorCode, bool bTimerExpired=false)
{
#ifdef ENABLE_CNM
   CRemLteConfigData & oLteCfg = CRemLteConfigData::getInstance();
#endif
   bool wait = false;
   /* We need to see the state and do stop scan reuqest if required */
   wait = checkRemStateAndSndScan();
   if(wait == true)
   {
      return REM_STATE_WAIT_NMM_STOP;
   }

#ifdef DEBUG_PRINT
   remLog(REM_TRACE_INFO,"Halting Scan");
#endif

   g_bScanHalted = true; //Scan Halted due to some reason
   if( bTimerExpired ) 
   {
      g_bScanStatus = REM_ERROR_TIMEOUT;
      g_strRemError = "Scan Timer Expired !!";

   } 
   else
   {
      generateErrorString(iLineNo, bErrorCode);
      g_bScanStatus = REM_ERROR;
      (*fpStopTimer)(REM_SCAN_TIMEOUT_EVT);
   }

   g_pRemEvtHdlr->m_pCl->sendNmmStop(); 

   fsmSendScanResultToOAM();

   return REM_STATE_INIT;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_ABORT_SCAN_EVT event.It sends NMM_STOP and assigns appropriate
// error string that can be sent to OAM.State is changes to REM_STATE_INIT
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleNmmAbortScan(void* )
{
#ifndef LTE_QCOM
   bool wait = false;
   /* We need to see the state and do stop scan reuqest if required */
   wait = checkRemStateAndSndScan();
   if(wait == true)
   {
      return REM_STATE_WAIT_NMM_ABORT;
   }
#endif
	// Abort scan is called, but scan is already finished
#ifdef ENABLE_CNM
   CRemLteConfigData & oLteCfg = CRemLteConfigData::getInstance();
#endif
	if( g_pRemEvtHdlr == NULL )
		return REM_STATE_INIT;

	// convergence layer API
	#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"Scan Aborted by Admin");
	#endif

	g_strRemError = "Scan Aborted by Admin!!";
	g_bScanStatus = REM_INDETERMINATE;

#ifdef ENABLE_CNM
   if(oLteCfg.getCnmEnableFlag())
   {
	   fsmSendScanResultToOAM();
  	   return REM_STATE_CNM;
   }
   else
#endif
   {
		(*fpStopTimer)(REM_SCAN_TIMEOUT_EVT);
	   g_pRemEvtHdlr->m_pCl->sendNmmStop(); 
	   fsmSendScanResultToOAM();
   }

	return REM_STATE_INIT;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles Timer Expiry
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleNmmTimerExpEvt(void*)
{
	// When timer got expired, scan was completed at same time, can happen in rarest of scenario
   // Need to save cell decoded info and don't move untill nmmstoprsp not come
	if( g_pRemEvtHdlr == NULL )
		return REM_STATE_INIT;

	return haltNmmScan(__LINE__, 0, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles Timer Expiry
///////////////////////////////////////////////////////////////////////////////////////////////////
int  handleSibReadTimerExpEvt(void*)
{
              
      TRACE_PRINTF("SIB READ TIMER EXPIRED ... SENDING STOP_BCCH_REQ MSG\n");
#ifdef LTE_QCOM
      return handleStopBCCHRspEvt(g_QCOMStartFreqScan);
#else
      g_pRemEvtHdlr->m_pCl->sendNmmStopBcchReq();
	   return REM_STATE_BCCH_CONFIG;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_NMMSTOP_RSP_EVT event.
//				- It calls the function to send Scan Result to OAM
// 				- If the list of RATs for which scanning is required is not empty,then
//				  it starts scanning for other RATs in the list and changes the state to
//				  REM_STATE_INIT
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleNmmStopRspEvt(void* pBuff)
{
	#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"NMM_STOP_RESP Received");
	#endif
	if( ((__stNMM_RSP_HDR*)pBuff)->bErrorCode != NMM_SUCCESS )
		generateErrorString(__LINE__,((__stNMM_RSP_HDR*)pBuff)->bErrorCode);

   //if scan list is not empty, start scanning for other RAT's

   if( !gRATList.empty() )
   {
      TRACE_PRINTF("More RATs to be scanned. Initializing next scan...\n");
	   releaseFsmLock();
		remHandleEvent(REM_START_SCAN_EVT,  (void*)NULL);
   }

	return REM_STATE_INIT;
}

void fsmSendScanResultToOAM()
{
#ifdef ENABLE_CNM
   CRemLteConfigData & oLteCfg = CRemLteConfigData::getInstance();
#endif

   // send scan status to OAM
   std::string strErr = g_pRemEvtHdlr->sendScanResultToOAM();
   if (g_pRemEvtHdlr->cellsAvailableAfterFilter()) //check whether there is any cell available in the list
   {
      if (g_bScanStatus == REM_ERROR_TIMEOUT)
      {
         g_strRemError = "Err: Scan Timer Expired !!";
      }
      else 
      {
         g_bScanStatus = REM_SUCCESS;
         g_strRemError = ""; /* Rem Error details should be NULL when status is success */
      }
   }
   else
   {
      g_bScanStatus = REM_ERROR; /* Rem Scan Status should always be ERROR when there is no cell to update */

      if(g_bScanHalted)
      {
         g_strRemError = "Err: No Cells found, Scan Halted in Middle";
      }
      else
      {
         g_strRemError = strErr; /* This will have string with information related to filter */
      }
   }

	remSendScanStatus(g_pRemEvtHdlr->getRATType(), g_bScanStatus, g_strRemError);

#ifdef ENABLE_CNM
   if(oLteCfg.getCnmEnableFlag())
   {
      trigCnmStart();
   }
   else
#endif
   {
      // We are done with scanning and sending result
      delete g_pRemEvtHdlr;
      g_pRemEvtHdlr = NULL;

      // Scan was aborted, hence delete remaining list
      if( g_bScanStatus == REM_INDETERMINATE )
      {
         while( !gRATList.empty() )
         {
            delete gRATList.front();
	    gRATList.pop_front();
         }
      }
   }

#if 0
   //if scan list is not empty, start scanning for other RAT's

   if( !gRATList.empty() )
   {
      TRACE_PRINTF("RAT SCAN LIST is not empty! Initializing next scan\n");
      remHandleEvent(REM_START_SCAN_EVT,  (void*)NULL);
   }
#endif

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_START_SCAN_EVT event.
//				- It sends NMM_START message and changes the state to  REM_STATE_INIT
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleNmmStartScan(void* )
{
    /* Nfft changes for DL BW */
    CRemLteConfigData & oLteCfg = CRemLteConfigData::getInstance();

    TRACE_PRINTF("NMM_START Received\n");

    if( gRATList.empty() ) {
        // log error
        return REM_STATE_INIT;
    }

    g_pRemEvtHdlr = gRATList.front();
    gRATList.pop_front();
#ifdef LTE_QCOM
    /* Initialize provisioning.xml file basic configuration for REM to work */
    g_pRemEvtHdlr->m_pCl->initProvXmlFile( (U8)oLteCfg.getDlBw(), oLteCfg.getBandSupported(), oLteCfg.getSupportedTech() );
    g_pRemEvtHdlr->m_pCl->sendNmmStart();
#else
        g_pRemEvtHdlr->m_pCl->sendNmmStart(oLteCfg.getDlBw());
#endif
        (*fpStartTimer)(REM_SCAN_TIMEOUT_EVT, g_pRemEvtHdlr->getScanTimeout());

        g_ignorePhyMsg = false;
    return REM_STATE_INIT;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_NMM_START_RSP_EVT event.
//				- It checks the type of scan by checking Express Scan Flags
//				- If SON Carrier Express Flag is enabled it sends RSSI Meas Req and changes
//					 the state to REM_STATE_MEAS_CONFIG
//				- If SON PCI Express Flag is enabled it sends Cell Search Req and changes
//					 the state to REM_STATE_CELL_CONFIG
//				- If both SON flags are set to False, then it sends RSSI Meas Req and changes
//					 the state to REM_STATE_MEAS_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleNmmStartRsp(void* pBuff)
{
#ifdef DEBUG_PRINT
    remLog(REM_TRACE_INFO,"NMM_START_RESP Received");
#endif
        if( ((__stNMM_RSP_HDR*)pBuff)->bErrorCode != NMM_SUCCESS )
            return haltNmmScan(__LINE__, ((__stNMM_RSP_HDR*)pBuff)->bErrorCode);
    
    //Added check for Qcom NL responding after scan timeout
    if( g_pRemEvtHdlr == NULL )
    {
       TRACE_PRINTF("REM Scan Timed-out\n");
       return REM_STATE_INIT;
    }

    if( g_pRemEvtHdlr->isCarrierExprScanEnabled() )
    {
        g_pRemEvtHdlr->m_pCl->sendNmmRssiMeasReq(g_pRemEvtHdlr->getBand(),
                g_pRemEvtHdlr->getARFCNCount(), g_pRemEvtHdlr->getARFCNList());
        return REM_STATE_MEAS_CONFIG;
    }

    if(g_pRemEvtHdlr->isPciExpScanEnabled() == true)
    {
        CLteEvtHandler* pObjLteEvtHdlr = dynamic_cast<CLteEvtHandler*>(g_pRemEvtHdlr);
        g_pRemEvtHdlr->m_pCl->sendNmmCellSearchReq(0, pObjLteEvtHdlr->m_bPCICount, pObjLteEvtHdlr->m_usPCIList);
        return REM_STATE_CELL_CONFIG;
    }

#if 0
    // This condition will not hit, as we configure all bands, if OAM doesnt configure any
    if( g_pRemEvtHdlr->getBand() == 0 ) {
        // Error, bands are not configured by OAM
#ifdef DEBUG_PRINT
        remLog(REM_TRACE_CRITICAL,"Band Not configured by OAM");
#endif
        return haltNmmScan(__LINE__, NMM_INVALID_FREQ_BAND);
    }
#endif

    g_pRemEvtHdlr->m_pCl->sendNmmRssiMeasReq(g_pRemEvtHdlr->getBand(),
            g_pRemEvtHdlr->getARFCNCount(), g_pRemEvtHdlr->getARFCNList());

    return REM_STATE_MEAS_CONFIG;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_RSSI_MEAS_RSP_EVT event.
//				- State is changed to REM_STATE_MEAS_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleRssiMeasRspEvt(void* pBuff)
{
	#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"NMM_RSSI_MEAS_REQ_RESP Received");
	#endif

	if( ((__stNMM_RSP_HDR*)pBuff)->bErrorCode == NMM_SUCCESS ) {
		//(*fpStartTimer)(REM_RSSI_TIMEOUT_EVT, g_pRemEvtHdlr->getRssiTimeout());
                TRACE_PRINTF("NMM_RSSI_MEAS_REQ_RESP Success\n");
		return REM_STATE_MEAS_CONFIG;
	}

	if( ((__stNMM_RSP_HDR*)pBuff)->bErrorCode == NMM_FREQ_BAND_NOT_SUPPORTED_BY_HW ||  
		 ((__stNMM_RSP_HDR*)pBuff)->bErrorCode == NMM_INVALID_FREQ_BAND  ) 
	{
		if( g_pRemEvtHdlr->nextBandAvailable() ) 
		{
         TRACE_PRINTF("Is that nextBandAvailable\n");
			g_pRemEvtHdlr->m_pCl->sendNmmRssiMeasReq(g_pRemEvtHdlr->getBand(),
				g_pRemEvtHdlr->getARFCNCount(), g_pRemEvtHdlr->getARFCNList());
			return REM_STATE_MEAS_CONFIG;
		}
		
		// if carriers are detected for differnt band or EARFCN, proceed to cell search
		if ( g_pRemEvtHdlr->sortARFCNWithStrongestRSSI() == true ) 
			return startCellSearch();
	}

	return haltNmmScan(__LINE__, ((__stNMM_RSP_HDR*)pBuff)->bErrorCode);
}

int handleRssiTimeoutEvt(void*)
{
	 g_pRemEvtHdlr->m_pCl->sendNmmStopRssiMeasReq();

	 return REM_STATE_MEAS_CONFIG;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_RSSI_MEAS_IND_EVT event.
//				- State is changed to REM_STATE_MEAS_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleRssiMeasIndEvt(void* pBuff)
{
	if( ((__stRssiMeasResult*)pBuff)->bErrorCode != NMM_SUCCESS )
		return haltNmmScan(__LINE__, ((__stNMM_RSSI_MEAS_IND_EVT*)pBuff)->stNmmRspHdr.bErrorCode);

#ifndef BRDCM 
	int iRSSI = (int)(((__stRssiMeasResult*)pBuff)->bRssi) + MIN_RSSI;
#else
   int iRSSI = (int) ((__stRssiMeasResult*)pBuff)->bRssi;
#endif

#ifdef LTE_QCOM
   (((__stRssiMeasResult*)pBuff)->bRssi) = -(((__stRssiMeasResult*)pBuff)->bRssi);
#endif

	TRACE_PRINTF("RSSI= %d ABS_RSSI %d ARFCN: %d\n", ((__stRssiMeasResult*)pBuff)->bRssi, iRSSI,
			((__stRssiMeasResult*)pBuff)->usARFCN);

	if( iRSSI > RSSI_THRESHOLD_TO_IGNORE )
		g_pRemEvtHdlr->handleRssiInd(static_cast<__stRssiMeasResult*>(pBuff));

	return REM_STATE_MEAS_CONFIG;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_RSSI_MEAS_CNF_EVT event.
//				- Sends NMM_STOP_RSSI_MEAS_REQ message
//				- State is changed to REM_STATE_MEAS_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleRssiMeasCnfEvt(void* pBuff)
{
	if( ((__stNMM_RSP_HDR*)pBuff)->bErrorCode != NMM_SUCCESS )
		return haltNmmScan(__LINE__, ((__stNMM_RSP_HDR*)pBuff)->bErrorCode);

	if( g_pRemEvtHdlr->nextBandAvailable() ) {
		g_pRemEvtHdlr->m_pCl->sendNmmRssiMeasReq(g_pRemEvtHdlr->getBand(),
				g_pRemEvtHdlr->getARFCNCount(), g_pRemEvtHdlr->getARFCNList());
		return REM_STATE_MEAS_CONFIG;
	}

	if ( g_pRemEvtHdlr->sortARFCNWithStrongestRSSI() == false ) 
		return fsmStopScanSuccessfuly("RSSI Scan Completed. No Carriers Found");

	return startCellSearch();
}

int startCellSearch()
{

	TRACE_PRINTF("Sending NMM_CELL_SEARCH_REQ with ARFCN %d:\n", g_pRemEvtHdlr->getARFCN());

  	//(*fpStopTimer)(REM_RSSI_TIMEOUT_EVT);
   g_pRemEvtHdlr->m_pCl->sendNmmCellSearchReq(g_pRemEvtHdlr->getARFCN(), 0, NULL);

	return REM_STATE_CELL_CONFIG;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_STOP_RSSI_RSP_EVT event.
//				- If the Scan type is Express scan for Carrier,then this is the end of scanning
//				- Else , for normal scan if there is next band available in the list,
//					RSSI Meas Req is sent for that band and state is changed
//					to REM_STATE_MEAS_CONFIG
//				- If there is no more bands available in the list,then Cell Search Req is sent
//					 and state is changed to REM_STATE_CELL_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleStopRssiRspEvt(void* pBuff)
{
	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
		return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);

	if( g_pRemEvtHdlr->isCarrierExprScanEnabled() )
	{
		if ( g_pRemEvtHdlr->sortARFCNWithStrongestRSSI() == false )
			return fsmStopScanSuccessfuly("RSSI Scan Completed. No Carriers Found");

		return fsmStopScanSuccessfuly();
	}

	if( g_pRemEvtHdlr->nextBandAvailable() ) {
		g_pRemEvtHdlr->m_pCl->sendNmmRssiMeasReq(g_pRemEvtHdlr->getBand(),
				g_pRemEvtHdlr->getARFCNCount(), g_pRemEvtHdlr->getARFCNList());
		return REM_STATE_MEAS_CONFIG;
	}

	// No neighbor cells available
	if ( g_pRemEvtHdlr->sortARFCNWithStrongestRSSI() == false ) {
		return fsmStopScanSuccessfuly("RSSI Scan Completed. No Carriers Found");
	}

	TRACE_PRINTF("Sending NMM_CELL_SEARCH_REQ with ARFCN %d:\n", g_pRemEvtHdlr->getARFCN());
	// send NMM_CELL_SEARCH_REQ
    return startCellSearch();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_CELL_SEARCH_RSP_EVT event.
//			- State is changed to REM_STATE_CELL_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleCellSearchRspEvt(void* pBuff)
{
	#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"NMM_CELL_SEARCH_REQ_RESP Received");
	#endif

	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
		return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);

	return REM_STATE_CELL_CONFIG;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_CELL_SEARCH_IND_EVT event.
//			- State is changed to REM_STATE_CELL_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleCellSearchIndEvt(void* pBuff)
{
	#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"NMM_CELL_SEARCH_IND Received");
	#endif

	if( ((__stNmmCellSearchIndEvt*)pBuff)->bErrorCode != NMM_SUCCESS )
			return haltNmmScan(__LINE__, ((__stNmmCellSearchIndEvt*)pBuff)->bErrorCode);

   if(((__stNmmCellSearchIndEvt*)pBuff)->bnumOfCellsFound > 0)
   {
		g_pRemEvtHdlr->handleCellSearchInd(static_cast<__stNmmCellSearchIndEvt *>(pBuff));
   }
   else
   {
	   TRACE_PRINTF("NMM_CELL_SEARCH_IND: No cells found \n");
   }
	
   return REM_STATE_CELL_CONFIG;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_CELL_SEARCH_CNF_EVT event.
//			- Send NMM_STOP_CELL_SEARCH_REQ
//			- Chnage the state to REM_STATE_CELL_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleCellSearchCnfEvt(void* pBuff)
{
	#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"NMM_CELL_SEARCH_CNF Received");
	#endif

	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
		return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);
   
   if( g_pRemEvtHdlr->isPciExpScanEnabled() ) {
      g_pRemEvtHdlr->m_pCl->sendNmmStop();
      g_bScanStatus = REM_SUCCESS;
      return REM_STATE_INIT;
   }

   if( g_pRemEvtHdlr->nextARFCNAvailable() ) {
      // send NMM_CELL_SEARCH_REQ
      return startCellSearch();
   }

   if( g_pRemEvtHdlr->cellsAvailableForScanning() ) 
   {
      (*fpStartTimer)(REM_READ_SIB_TIMEOUT_EVT, REM_READ_SIB_TIMEOUT);

      if( g_pRemEvtHdlr->m_pCl->skipPbchMsg()) 
      {
         TRACE_PRINTF("\nSending Sib1CfgReq\n");
         g_pRemEvtHdlr->m_pCl->sendNmmSib1CfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
         return REM_STATE_SIB1_CONFIG;
      }
      else
      {
         g_pRemEvtHdlr->m_pCl->sendNmmPbchCfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
         return REM_STATE_PBCH_CONFIG;
      }
   }

   return fsmStopScanSuccessfuly("Cell Search Completed. No Neighbour Cells Found");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_STOP_CELL_SEARCH_RSP_EVT event.
//			- For PCI Express Scan this is the end of scanning procedure.
//				So,send NMM STOP and change the state to REM_STATE_INIT
//			- For normal scan,If more bands are available in the list then send
//				Cell Search Req and change the state to REM_STATE_CELL_CONFIG
//			- If more cells are available for scanning send PBCH Config Req
//				and change the state to REM_STATE_PBCH_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleStopCellSearchRspEvt(void* pBuff)
{
	#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"NMM_CELL_SEARCH_CNF Received");
	#endif

	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
		return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);

	if( g_pRemEvtHdlr->isPciExpScanEnabled() ) {
		g_pRemEvtHdlr->m_pCl->sendNmmStop();
		g_bScanStatus = REM_SUCCESS;
		return REM_STATE_INIT;
	}

	if( g_pRemEvtHdlr->nextARFCNAvailable() )	{
		// send NMM_CELL_SEARCH_REQ
      return startCellSearch();
	}

   if( g_pRemEvtHdlr->cellsAvailableForScanning() ) 
   {
      (*fpStartTimer)(REM_READ_SIB_TIMEOUT_EVT, REM_READ_SIB_TIMEOUT);

      if( g_pRemEvtHdlr->m_pCl->skipPbchMsg()) 
      {
         TRACE_PRINTF("\nSending Sib1CfgReq\n");
         g_pRemEvtHdlr->m_pCl->sendNmmSib1CfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
         return REM_STATE_SIB1_CONFIG;
      }
      else
      {
         g_pRemEvtHdlr->m_pCl->sendNmmPbchCfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
         return REM_STATE_PBCH_CONFIG;
      }
   }

	return fsmStopScanSuccessfuly("Cell Search Completed. No Neighbour Cells Found");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_PBCH_CONFIG_CNF_EVT event.
//			- State is changed to REM_STATE_PBCH_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handlePbchConfigCnfEvt(void* pBuff)
{
	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
		return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);

	return REM_STATE_PBCH_CONFIG;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_PBCH_DATA_IND_EVT event.
//			- Send NMM_SIB1_CONFIG_REQ
//			- State is changed to REM_STATE_SIB1_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handlePbchDataIndEvt(void* pBuff)
{
	int iErr = g_pRemEvtHdlr->handlePbchDataInd(pBuff);

	if( iErr == MIB_FAILED ) // MIB DECODING FAILED
	{
		TRACE_PRINTF("MIB/PBCH Decode failed for PCI %d\n", g_pRemEvtHdlr->getPciOrPsc());
		if( g_pRemEvtHdlr->nextCellAvailable() ) {
			g_pRemEvtHdlr->m_pCl->sendNmmPbchCfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
			return REM_STATE_PBCH_CONFIG;
		}

		return haltNmmScan(__LINE__, NMM_ERR_MIB_FAILED);
	}

	g_pRemEvtHdlr->m_pCl->sendNmmStopPbchReq();

	return REM_STATE_PBCH_CONFIG;
		TRACE_PRINTF("MIB/PBCH Decode failed for PCI %d\n", g_pRemEvtHdlr->getPciOrPsc());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_STOP_PBCH_RSP_EVT event.
//			- Send NMM_STOP_PBCH_REQ message
//			- Change the state to REM_STATE_BCCH_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleStopPBCHRspEvt(void* pBuff)
{
	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
		return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);

    (*fpStartTimer)(REM_READ_SIB_TIMEOUT_EVT, REM_READ_SIB_TIMEOUT);
    g_pRemEvtHdlr->m_pCl->sendNmmSib1CfgReq(); 

	return REM_STATE_SIB1_CONFIG;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_SIB1_CONFIG_CNF_EVT event.
//			- State is changed to REM_STATE_SIB1_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleSIB1ConfigCnfEvt(void* pBuff)
{
   if( ((__stNmmCnf*)pBuff)->bErrorCode != NMM_SUCCESS && ((__stNmmCnf*)pBuff)->bErrorCode != NMM_DROPPING )
   {
	   unsigned int iErr = g_pRemEvtHdlr->handleSib1CfgCnf(pBuff);
      if(iErr == NMM_ERR_BCH_FAILED)
      {
         (*fpStopTimer)(REM_READ_SIB_TIMEOUT_EVT);
		   if( g_pRemEvtHdlr->nextCellAvailable() ) 
         {
            (*fpStartTimer)(REM_READ_SIB_TIMEOUT_EVT, REM_READ_SIB_TIMEOUT);

            if( g_pRemEvtHdlr->m_pCl->skipPbchMsg()) 
            {
               TRACE_PRINTF("\nSending Sib1CfgReq\n");
               g_pRemEvtHdlr->m_pCl->sendNmmSib1CfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
               return REM_STATE_SIB1_CONFIG;
            }
            else
            {
               g_pRemEvtHdlr->m_pCl->sendNmmPbchCfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
               return REM_STATE_PBCH_CONFIG;
            }
         }
         else
         {
            g_bSendNmmBcchCfgReq = false; 
            return fsmStopScanSuccessfuly("Cell Search Completed");
         }
      }

      return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);
   }

   if(((__stNmmCnf*)pBuff)->bErrorCode == NMM_DROPPING )
   {
      /* Stopping Previous READ_SIB_TIMER */
      (*fpStopTimer)(REM_READ_SIB_TIMEOUT_EVT);
      g_bSendNmmBcchCfgReq = false;
      if(g_sib1ReqCount < 2)
      {
         (*fpStartTimer)(REM_READ_SIB_TIMEOUT_EVT, REM_READ_SIB_TIMEOUT);
         g_sib1ReqCount++;
         TRACE_PRINTF("\nSending Sib1CfgReq as SIB-1 was not decoded Properly");
         g_pRemEvtHdlr->m_pCl->sendNmmSib1CfgReq(g_pRemEvtHdlr->getPciOrPsc(),
               g_pRemEvtHdlr->getARFCN2());
         return REM_STATE_SIB1_CONFIG;
      }
      else
      {
         g_sib1ReqCount = 0;
         if( g_pRemEvtHdlr->nextCellAvailable())
         {
            (*fpStartTimer)(REM_READ_SIB_TIMEOUT_EVT, REM_READ_SIB_TIMEOUT);
            g_pRemEvtHdlr->m_pCl->sendNmmSib1CfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
            return REM_STATE_SIB1_CONFIG;
         }
         else
         {
            return fsmStopScanSuccessfuly("Cell Search Completed");  
         }
      }
   }
   return REM_STATE_BCCH_CONFIG;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_BCCH_CONFIG_CNF_EVT event.
//			- Send NMM_BCCH_CONFIG_REQ
//			- Change the state to REM_STATE_BCCH_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleBCCHConfigCnfEvt(void* pBuff)
{
   if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
      return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);

   return REM_STATE_BCCH_CONFIG;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_BCCH_DATA_IND_EVT event.
//			- Send NMM_BCCH_CONFIG_REQ
//			- Change the state to REM_STATE_BCCH_CONFIG
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleBCCHDataIndEvt(void* pBuff)
{
   __stLteSibsPeriodicity  sibsInfo;
#ifdef LTE_QCOM
   bool stopBcch = false;
#endif
#ifdef LTE_TDD
 CRemLteConfigData & oLteCfg = CRemLteConfigData::getInstance();
#endif
	// PHY is unable to decode SIB, send STOP_BCCH_REQ
	if( ((__stNmmRsp*)pBuff)->bErrorCode == NMM_SIB_INCOMPLETE ) {
		g_pRemEvtHdlr->m_pCl->sendNmmStopBcchReq();
		return REM_STATE_BCCH_CONFIG;
	}

	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
   {
		TRACE_PRINTF("handleBCCHDataIndEvt: ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS,\
                    Value is %d\n", ((__stNmmRsp*)pBuff)->bErrorCode);
   #if 0
		return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);
   #endif
   }

	// BCCH decoding completed, advance to next PCI/RSSI
   if( g_pRemEvtHdlr->handleBcchDataInd(pBuff) == true ) {
      TRACE_PRINTF("SIB1 SIB2 Decoding Successfull, sending NMM_STOP_BCCH_REQ\n");
#ifdef LTE_QCOM
      stopBcch = true;
#else
      (*fpStopTimer)(REM_READ_SIB_TIMEOUT_EVT);
      g_pRemEvtHdlr->m_pCl->sendNmmStopBcchReq();
#endif
	}

	if( g_bSendNmmBcchCfgReq == false  && g_pRemEvtHdlr->isSib1Decoded() == true) {
      g_sib1ReqCount = 0;
        g_pRemEvtHdlr->prepareNmmSibsPeriodicity(&sibsInfo);
#ifdef LTE_TDD
  		g_pRemEvtHdlr->m_pCl->sendNmmBcchCfgReq(g_pRemEvtHdlr->getSiWindow(),
                               oLteCfg.getsubframeAssignment(),
                               oLteCfg.getspecialSubframePatterns(),
                               &sibsInfo);   
#else
		g_pRemEvtHdlr->m_pCl->sendNmmBcchCfgReq(g_pRemEvtHdlr->getSiWindow(),
                                                        &sibsInfo);   
#endif
		g_bSendNmmBcchCfgReq = true ;
	}
   
   /* if Sib1 is not decoded send again sib1 req with same earfcn and if maximum number of 
       sib request is sent then go for other freq sib request. */
   if( g_pRemEvtHdlr->isSib1Decoded() == false )
   {
      if(g_sib1ReqCount < 2)
      {
         (*fpStartTimer)(REM_READ_SIB_TIMEOUT_EVT, REM_READ_SIB_TIMEOUT);
         g_sib1ReqCount++;
         printf("\nSending Sib1CfgReq as SIB-1 was not decoded Properly \n");
         g_pRemEvtHdlr->m_pCl->sendNmmSib1CfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
         return REM_STATE_SIB1_CONFIG;
      }
      else
      {
         g_sib1ReqCount = 0;
         if( g_pRemEvtHdlr->nextCellAvailable() )
         {
            (*fpStartTimer)(REM_READ_SIB_TIMEOUT_EVT, REM_READ_SIB_TIMEOUT);
            if( g_pRemEvtHdlr->m_pCl->skipPbchMsg())
            {
               printf("\nSENDING SIB1-CFGREQ\n");
               g_bSendNmmBcchCfgReq = false;
               g_pRemEvtHdlr->m_pCl->sendNmmSib1CfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
               return REM_STATE_SIB1_CONFIG;
            }
            else
            {
               g_pRemEvtHdlr->m_pCl->sendNmmPbchCfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
               g_bSendNmmBcchCfgReq = false;
               return REM_STATE_PBCH_CONFIG;
            }
         }
         /* set for second time scan */
         g_bSendNmmBcchCfgReq = false;

         return fsmStopScanSuccessfuly();

      }

   }
#ifdef LTE_QCOM
   else
   {
      /*If either sib2 or sib4 not decoded properly and sib1 decoding 
        is successful send sib_read request*/
      if(g_bSendNmmBcchCfgReq == true  && g_pRemEvtHdlr->isSib1Decoded() == true && (g_sibRequest == g_sibResponse) && (g_sib1ReqCount < 2))
      {
         if(g_sibError == 1)
         {
            g_sibError = 0;
            g_sib1ReqCount++;
         g_pRemEvtHdlr->prepareNmmSibsPeriodicity(&sibsInfo);
         g_pRemEvtHdlr->m_pCl->sendNmmBcchCfgReq(g_pRemEvtHdlr->getSiWindow(),
               &sibsInfo);
         }
      }
   }
#endif

#ifdef LTE_QCOM
   if(stopBcch == true)
   {
      TRACE_PRINTF("Moving to Stop Bcch Response event state\n");
      (*fpStopTimer)(REM_READ_SIB_TIMEOUT_EVT);
      return handleStopBCCHRspEvt(g_QCOMStartFreqScan);
   }
#endif

	return REM_STATE_BCCH_CONFIG;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
// This function handles REM_STOP_BCCH_RSP_EVT event.
//			- If more cells are available , Send NMM_PBCH_CONFIG_REQ message
//				Change the state to REM_STATE_PBCH_CONFIG
//			- If No more cells are available then this is the end of scan
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleStopBCCHRspEvt(void* pBuff)
{
	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
		return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);

   // check if we can do frequency sync with this cell
   if(g_pRemEvtHdlr->startFreqSync())
   {
      return REM_STATE_BCCH_CONFIG;
   }
   
   if( g_pRemEvtHdlr->nextCellAvailable() ) 
   {
      (*fpStartTimer)(REM_READ_SIB_TIMEOUT_EVT, REM_READ_SIB_TIMEOUT);

      if( g_pRemEvtHdlr->m_pCl->skipPbchMsg()) 
      {
         TRACE_PRINTF("SENDING SIB1-CFGREQ\n");
         g_bSendNmmBcchCfgReq = false;
         g_pRemEvtHdlr->m_pCl->sendNmmSib1CfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
         return REM_STATE_SIB1_CONFIG;
      }
      else
      {
         g_pRemEvtHdlr->m_pCl->sendNmmPbchCfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
         g_bSendNmmBcchCfgReq = false;
         return REM_STATE_PBCH_CONFIG;
      }
   }
   /* set for second time scan */
   g_bSendNmmBcchCfgReq = false;

	return fsmStopScanSuccessfuly();
}

int getPbchORSib1OfNextCell()
{
   // if not successful then we should try the next cell
   if( g_pRemEvtHdlr->nextCellAvailable() ) 
   {
      (*fpStartTimer)(REM_READ_SIB_TIMEOUT_EVT, REM_READ_SIB_TIMEOUT);

      if( g_pRemEvtHdlr->m_pCl->skipPbchMsg()) 
      {
         TRACE_PRINTF("SENDING SIB1-CFGREQ\n");
         g_bSendNmmBcchCfgReq = false;
         g_pRemEvtHdlr->m_pCl->sendNmmSib1CfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
         return REM_STATE_SIB1_CONFIG;
      }
      else
      {
         g_pRemEvtHdlr->m_pCl->sendNmmPbchCfgReq(g_pRemEvtHdlr->getPciOrPsc(), g_pRemEvtHdlr->getARFCN2());
         g_bSendNmmBcchCfgReq = false;
         return REM_STATE_PBCH_CONFIG;
      }
   }

   return MAX_REM_APP_STATES;
}

int handleNmmFreqSyncRspEvt(void* pBuff)
{
	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
		return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);
   
   return REM_STATE_BCCH_CONFIG;
}

int handleNmmFreqSyncIndEvt(void* pBuff)
{
	if(((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS)
   {  
		return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);
   }

   // if there is an error in the message or no cells were found, go to the next cell
   if(g_pRemEvtHdlr->handleFreqSyncInd(pBuff) == false)
   {
      int state = getPbchORSib1OfNextCell();

      if(state != MAX_REM_APP_STATES)
      {
         return state;
      }
   }
   
	return REM_STATE_BCCH_CONFIG;
}

int handleNmmFreqSyncCnfEvt(void *pBuff)
{
	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
   {  
		return haltNmmScan(__LINE__, ((__stNmmRsp*)pBuff)->bErrorCode);
   }
   
	g_bSendNmmBcchCfgReq = false;
   return fsmStopScanSuccessfuly("LTE Freq Sync Successful");
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : strErrDetails - string to be displayed
// This function is called at the end of scanning.
//			- Send NMM STOP message
//			- Go to initial state REM_STATE_INIT
///////////////////////////////////////////////////////////////////////////////////////////////////
int fsmStopScanSuccessfuly(const char* strErrDetails)
{
   TRACE_PRINTF("Enabling CNM and sending data to OAM\n");
#ifdef ENABLE_CNM
   CRemLteConfigData & oLteCfg = CRemLteConfigData::getInstance();
#endif

  	g_bScanStatus = REM_SUCCESS;
 	g_strRemError = strErrDetails;

#ifdef ENABLE_CNM
   if(oLteCfg.getCnmEnableFlag())
   {
     fsmSendScanResultToOAM();
 	  return REM_STATE_CNM;
   }
   else
#endif
   {
  	   (*fpStopTimer)(REM_SCAN_TIMEOUT_EVT);
      g_pRemEvtHdlr->m_pCl->sendNmmStop(); 
      fsmSendScanResultToOAM();

    	return REM_STATE_INIT;
   }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : enEvent - Type of Event
//				 pBuff   - void pointer
// This is the eventhandler function
///////////////////////////////////////////////////////////////////////////////////////////////////
void remHandleEvent(__enREMAppEvent enEvent, void* pBuff )
{
	getFsmLock();

	g_tvEvtStatistics[enEvent]++;
	int iNewState = g_iCurrState;

	TRACE_PRINTF("Received Event:[%s] State:[%s]\n",
         g_strEvent[enEvent],
         g_strState[g_iCurrState]);

	for(int i=0; ; i++)
	{
		   if( g_stRemFsm[g_iCurrState][i].enEvent == enEvent )
		   {
		      iNewState = (*g_stRemFsm[g_iCurrState][i].fpEvtHdlr)(pBuff);
		      break;
		   }

		   if( g_stRemFsm[g_iCurrState][i].enEvent != REM_APP_INVALID_EVT )
			   continue;
#ifdef DEBUG_PRINT
      TRACE_PRINTF("ERROR: Event:[%s] State:[%s]\n",
                    g_strEvent[enEvent],
                    g_strState[g_iCurrState]);
		   remLog(REM_TRACE_CRITICAL, "Invalid Event");
#endif
		   break;
	}

	g_iCurrState = iNewState;
   TRACE_PRINTF("Current State:[%s]\n",
         g_strState[g_iCurrState]);

	releaseFsmLock();
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//
// This function takes care of all intialization functionality for REM-FSM
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void initFsm()
{
	pthread_mutex_init(&g_fsmMutex, NULL);

	g_tvEvtStatistics.reserve(MAX_REM_APP_EVENTS);
	std::fill(g_tvEvtStatistics.begin(),g_tvEvtStatistics.end(),0);

	g_tErrValue[RC_PHY_NOT_PARAM] = "NO required parameter";
	g_tErrValue[RC_PHY_CTX_ALLOC_ERROR] = "PHY Instance context allocation error";
	g_tErrValue[RC_PHY_PART_ALLOC_ERROR] = "Allocation of Intel message partition error";
	g_tErrValue[RC_PHY_UNKNOWN_API] = "Unknown Message ID";
	g_tErrValue[RC_PHY_UNKNOWN_TYPE] = "Unknown Type ID";
	g_tErrValue[RC_PHY_UNKNOWN_PARAM]="Unknown Parameter ID";
	g_tErrValue[RC_PHY_CONTEX_ALLOC_ERROR]="PHY TX/RX context allocation error";
	g_tErrValue[RC_PHY_INCORRECT_PARAM]="Incorrect Parameter";
	g_tErrValue[RC_PHY_NO_VALID_DATA_ERROR]="Parameter value is incorrect";
	g_tErrValue[RC_PHY_HEAP_ALLOC_ERROR]="General Heap allocation error";
	g_tErrValue[NMM_LTE_TDD_NOT_SUPPORTED]="LTE TDD not supported";
	g_tErrValue[NMM_NOT_ENABLED]="NMM command sent without NMM init";
	g_tErrValue[NMM_UNEXPECTED_CMD]="Invalid API command";
	g_tErrValue[NMM_INVALID_FREQ_BAND]="Invalid Frequency Band";
	g_tErrValue[NMM_INVALID_EARFCN]="Invalid LTE carrier";
	g_tErrValue[NMM_FREQ_BAND_NOT_SUPPORTED_BY_HW]="Unsupported frequency band requested";
	g_tErrValue[NMM_INVALID_NR_CARRIER_LIST]="Invalid number of carrier requested for RSSI measurement";
	g_tErrValue[NMM_INVALID_CARRIER_LIST]="Invalid carrier list requested for RSSI measurement";
	g_tErrValue[NMM_LTE_IN_USE]="NMM already in use.Successive NMM_START without NMM_STOP";
	g_tErrValue[NMM_LTE_OUT_OF_MEMORY]="Heap allocation error during NMM";
	g_tErrValue[NMM_LTE_DI_ERROR]="DI Error";
	g_tErrValue[NMM_ERR_MIB_FAILED] = "PBCH / MIB Decode Failed";
}

bool checkRemStateAndSndScan(void)
{
   bool wait = false;
   switch(g_iCurrState)
   {
      case REM_STATE_BCCH_CONFIG:
      case REM_STATE_SIB1_CONFIG:
      {
         g_pRemEvtHdlr->m_pCl->sendNmmStopBcchReq();
         wait = true;
         break;
      }
      case REM_STATE_PBCH_CONFIG:
      {
         g_pRemEvtHdlr->m_pCl->sendNmmStopPbchReq();
         wait = true;
         break;
      }
      default:
      {
         /* some request is still pending at PHY so don't send this request */
         if(g_reqSent == TRUE)
         {
            TRACE_PRINTF("\nGot Abort/Halt scan during ongoing request...\n"); 
#ifdef DEBUG_PRINT
            remLog(REM_TRACE_INFO,"some request is still pending in phy so don't do Halting Scan");
#endif
            g_ignorePhyMsg = true;
            wait = true;
         }
      }
   }
   return wait;
}



