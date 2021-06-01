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

*************************************************************************************************17*/


/**************************************************************************************************

     Name:     REM-APP

     Type:     C++ Header file

     Desc:     Consists of data-structures related to fsm functions

     File:     rem_fsm.h

     Sid:      rem_fsm.h@@/main/Tenb_T3k_Perf/1 - Wed Jan  9 20:53:11 2013

     Prg:      Vikram K

**************************************************************************************************/

#ifndef REM_FSM_H_
#define REM_FSM_H_

#include "rem_events.h"
#include "rem_eventhandler.h"
#include "rem_lte_event_handler.h"
#include <sstream>

#define REM_READ_SIB_TIMEOUT 60
namespace remfsm {
	extern pthread_mutex_t g_fsmMutex;
#ifdef ENABLE_CNM
	extern std::list <CRemEvtHandler*> gRATList;
	extern CRemEvtHandler *g_pRemEvtHdlr;
	extern int g_iCurrState;
   extern 	__enRemScanStatus g_bScanStatus; // Last Scan Status
	extern std::string g_strRemError; // Last Scan Error
	extern std::map<U8 , std::string> g_tErrValue;
	extern int g_bScanHalted;
   extern 	std::vector<int> g_tvEvtStatistics;
	extern bool g_bSendNmmBcchCfgReq;
   extern U16 g_cnmSlctdEarfcn;
   extern U16 g_cnmSlctdPci;
#endif
}

inline void getFsmLock() {
	pthread_mutex_lock(&remfsm::g_fsmMutex);
}
inline void releaseFsmLock() {
	pthread_mutex_unlock(&remfsm::g_fsmMutex);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @enum __enREMAppStates
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	REM_STATE_INIT = 0,
	REM_STATE_MEAS_CONFIG,
	REM_STATE_CELL_CONFIG,
	REM_STATE_PBCH_CONFIG,
	REM_STATE_SIB1_CONFIG,
	REM_STATE_BCCH_CONFIG,
#ifdef ENABLE_CNM
   REM_STATE_CNM,
#endif
        REM_STATE_WAIT_NMM_STOP, // due to timer expiry
        REM_STATE_WAIT_NMM_ABORT, // due to OAM abort scan
	MAX_REM_APP_STATES

} __enREMAppStates;

void remHandleEvent(__enREMAppEvent enEvent, void* pBuff );
void generateErrorString(int iLineNo,U8 bErrorCode);
inline void getFsmLock();
inline void releaseFsmLock();
int haltNmmScan(int iLineNo,U8 bErrorCode, bool bTimerExpired);
int fsmStopScanSuccessfuly(const char* strErrDetails= "No ERROR!!");

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemFSM
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__enREMAppEvent enEvent;		// Event
		int (*fpEvtHdlr) (void *);	// Function pointer to EVENT handler method
} __stRemFSM;

// COMMON
int handleNmmStopScan(void*);
int handleNmmTimerExpEvt(void*);
int handleNmmAbortScan(void*);
int handleNmmStopRspEvt(void*);

// REM_STATE_INIT
int handleNmmStartScan(void*);
int handleNmmStartRsp(void*);

// REM_STATE_MEAS_CONFIG
int handleRssiMeasRspEvt(void*);
int handleRssiMeasIndEvt(void*);
int handleRssiMeasCnfEvt(void*);
int handleStopRssiRspEvt(void*);
int handleRssiTimeoutEvt(void*);

// REM_STATE_CELL_CONFIG
int handleCellSearchRspEvt(void*);
int handleCellSearchIndEvt(void*);
int handleCellSearchCnfEvt(void*);
int handleStopCellSearchRspEvt(void*);

// REM_STATE_PBCH_CONFIG
int handlePbchConfigCnfEvt(void*);
int handlePbchDataIndEvt(void*);

// REM_STATE_SIB1_CONFIG
int handleSIB1ConfigCnfEvt(void*);

// REM_STATE_BCCH_CONFIG
int handleBCCHConfigCnfEvt(void*);
int handleBCCHDataIndEvt(void*);
int handleStopPBCHRspEvt(void*);
int handleStopBCCHRspEvt(void*);
int handleSibReadTimerExpEvt(void*);
int handleNmmFreqSyncRspEvt(void *);
int handleNmmFreqSyncIndEvt(void *);
int handleNmmFreqSyncCnfEvt(void *);
unsigned int handleSib1CfgCnf(void *);

#ifdef ENABLE_CNM
//CNM Related API's
int trigCnmStart();
int handleCnmStopEvt(void *);
int handleCnmAbortScan(void *);
int handleCnmCellSearchRspEvt(void*);
int handleCnmCellSearchIndEvt(void*);
int handleCnmCellSearchCnfEvt(void*);
int handleCnmPbchConfigCnfEvt(void*);
int handleCnmPbchDataIndEvt(void*);
int handleCnmStopPBCHRspEvt(void*);
#endif

void initFsm();
void fsmSendScanResultToOAM();
int startCellSearch();
#endif /* REM_FSM_H_ */
