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

     Desc:     Consists of data-structures related to different events

     File:     rem_events.h

     Sid:      rem_events.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/2 - Thu May 23 19:18:24 2013

     Prg:      Ambika S

**************************************************************************************************/

#ifndef REM_EVENTS_H_
#define REM_EVENTS_H_

#include "rem_common.h"

#define MAX_BCCH_DATA_LIST  32
#define MAX_BCCH_SIZE		2048

// Remove this Enum later once it is available in LtePhyL2Api.h
///////////////////////////////////////////////////////////////////////////////////////////////////
//   @enum __enPHYErrorCode
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	SUCESS = 0,						// OK
	RC_PHY_NOT_PARAM,			// NO required parameter
	RC_PHY_CTX_ALLOC_ERROR, 	// PHY Instance context allocation error
	RC_PHY_PART_ALLOC_ERROR,	// Allocation of Intel Message Partition error
	RC_PHY_UNKNOWN_API,			// Unknown Msg Id
	RC_PHY_UNKNOWN_TYPE,		// Unknown Type Id
	RC_PHY_UNKNOWN_PARAM,		// Unknown Parameter Id
	RC_PHY_CONTEX_ALLOC_ERROR,  // PHY TX/RX context allocation error
	RC_PHY_INCORRECT_PARAM, 	// Incorrect parameter
	RC_PHY_NO_VALID_DATA_ERROR, // Parameter value is incorret
	RC_PHY_HEAP_ALLOC_ERROR		// General Heap Allocation Error
}__enPHYErrorCode;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @enum __enREMAppEvent
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{

	REM_APP_INVALID_EVT = 0,

	REM_SCAN_TIMEOUT_EVT,	// COMMON
	REM_ABORT_SCAN_EVT,		// COMMON
	REM_RSSI_TIMEOUT_EVT,	// COMMON

	REM_START_SCAN_EVT,             // REM_STATE_INIT
	REM_START_CARRIER_EXPSCAN_EVT,  // REM_STATE_INIT
	REM_START_PCI_EXPSCAN_EVT,      // REM_STATE_INIT
	REM_NMM_START_RSP_EVT,          // REM_STATE_INIT
	REM_NMMSTOP_RSP_EVT,			// REM_STATE_INIT

	REM_RSSI_MEAS_RSP_EVT,  // REM_STATE_MEAS_CONFIG
	REM_RSSI_MEAS_IND_EVT,  // REM_STATE_MEAS_CONFIG
	REM_RSSI_MEAS_CNF_EVT,  // REM_STATE_MEAS_CONFIG
	REM_STOP_RSSI_RSP_EVT,  // REM_STATE_MEAS_CONFIG

	REM_CELL_SEARCH_RSP_EVT, 		// REM_STATE_CELL_CONFIG
	REM_CELL_SEARCH_IND_EVT, 		// REM_STATE_CELL_CONFIG
	REM_CELL_SEARCH_CNF_EVT, 		// REM_STATE_CELL_CONFIG
	REM_STOP_CELL_SEARCH_RSP_EVT, 	// REM_STATE_CELL_CONFIG

	REM_PBCH_CONFIG_CNF_EVT,	// REM_STATE_PBCH_CONFIG
	REM_PBCH_DATA_IND_EVT,		// REM_STATE_PBCH_CONFIG

	REM_SIB1_CONFIG_CNF_EVT,	// REM_STATE_SIB1_CONFIG

	REM_BCCH_CONFIG_CNF_EVT,	// REM_STATE_BCCH_CONFIG
	REM_BCCH_DATA_IND_EVT,		// REM_STATE_BCCH_CONFIG
	REM_STOP_PBCH_RSP_EVT,	// REM_STATE_BCCH_CONFIG
	REM_STOP_BCCH_RSP_EVT,	// REM_STATE_BCCH_CONFIG
	REM_READ_SIB_TIMEOUT_EVT,	// REM_STATE_BCCH_CONFIG

#ifdef ENABLE_CNM
   REM_CNM_CELL_SEARCH_RSP_EVT,//Rem start event for cell search
   REM_CNM_CELL_SEARCH_IND_EVT,
   REM_CNM_CELL_SEARCH_CNF_EVT,
   REM_CNM_PBCH_CONFIG_CNF_EVT,
   REM_CNM_PBCH_DATA_IND_EVT,
   REM_CNM_STOP_PBCH_RSP_EVT,
#endif

	MAX_REM_APP_EVENTS

} __enREMAppEvent;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stNmmRsp
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	U8		bSubFrameNumber;  	/* sub frame number */
	U8		bErrorCode;		  	/* Error code value  */
} __stNmmRsp;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stNmmCnf
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	U8		bSubFrameNumber;  	/* sub frame number */
	U8		bErrorCode;		  	/* Error code value  */
} __stNmmCnf;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stCellElement
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
   S16	  bRSRP_RSCP;
   U16	  usPCI_PSC;
   S16    ecio;
   U8     TxAntenna; // Number of TX antgennas
   U8     ulDlSubframeCfg; /* 0..6 (For TDD mode) */
   U8     splSfCfg; /* 0..8 (For TDD mode) */
//#ifdef BRDCM
   U8     dlBandwidth;
//#endif
} __stCellElement;


///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stCellAdtnlElemt
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
        U8              bDuplex; // FDD[0]/TDD[1] 
        U8              bCpSize; // Normal CP[0] or Extended CP [1]
        S16             sFreqOffset; //Frequency Offset in Hz 
        S16             bRssi; // EUTRA Carrier RSSI
} __stCellAdtnlElemt;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stNmmCellSearchIndEvt
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	U8		 bnumOfCellsFound;
	U8       	 bErrorCode;
   U16    bearfcn;
	__stCellElement stCellElm[REM_MAX_CELL_FOUND_LIST];
	U8		 bnumOfAdditionalElmnt;
        __stCellAdtnlElemt  stCellAddElmnt[REM_MAX_VENDOR_LIST];
} __stNmmCellSearchIndEvt;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRssiMeasResult
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	U16	usARFCN;
	S16 	bRssi;
	U8		bErrorCode;
	U8		bnumOfCellsFound;
   U8    bCpSize[REM_MAX_CELL_FOUND_LIST]; // Normal CP[0] or Extended CP [1]
	__stCellElement stCellElm[REM_MAX_CELL_FOUND_LIST];

} __stRssiMeasResult;

#endif /* REM_EVENTS_H_ */
