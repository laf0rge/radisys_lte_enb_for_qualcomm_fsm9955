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

     Desc:     All datastructures are added in this header file

     File:     rem_common.h

     Sid:      rem_common.h@@

     Prg:      Ambika S

**************************************************************************************************/

#ifndef REM_COMMON_H_
#define REM_COMMON_H_

#ifdef USE_RADISYS_OAM
#include "Trace.h"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//	  System Includes
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <stdio.h>
#include <time.h>

#include "rem_interface.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//	  Message Constants
///////////////////////////////////////////////////////////////////////////////////////////////////
#define REM_MAX_PLMN_LIST       64
#define REM_MAX_PCI_LIST        32
#define REM_MAX_CARRIER_LIST	64
#define REM_MAX_BAND_LIST		64
#define REM_MAX_RSSI_LIST		16
#define REM_MAX_RSRP_LIST		16
#define REM_MAX_CELL_LIST		64
#define REM_MAX_ERRORS			256
#define REM_MAX_VENDOR_LIST 	6
#define REM_MAX_CELL_FOUND_LIST	16
#define TEST 00

#define RSSI_THRESHOLD_TO_IGNORE	-100
#define MIN_RSSI                        -128

#define SI_WINDOW  40

#define MIB_FAILED 	1
#define MIB_OK		0

#define RSSI_TIMEOUT_PER_EARFCN	15

#define NMM_SUCCESS			0
#define NMM_ERR_MIB_FAILED	200
#define NMM_DROPPING 3
#define NMM_ERR_BCH_FAILED	255

// FREQ SYNC START
#define MAX_EARFCN_SCAN_LIST_SIZE 256
#define MAX_SNIFFER_RTWP_LIST_LEN 50
// FREQ SYNC END

///////////////////////////////////////////////////////////////////////////////
// Debugging Flags
///////////////////////////////////////////////////////////////////////////////

#define AIRV_ASSERT assert
#ifdef TRACE_PRINTF
#undef TRACE_PRINTF
#endif
#define TRACE_PRINTF printf

///////////////////////////////////////////////////////////////////////////////////////////////////
// Trace Constants & Trace levels bit masks.
///////////////////////////////////////////////////////////////////////////////////////////////////

static const U32 REM_TRACE_VERBOSE          = 0x00000001;
static const U32 REM_TRACE_INFO             = 0x00000002;
static const U32 REM_TRACE_WARNING          = 0x00000004;
static const U32 REM_TRACE_CRITICAL         = 0x00000008;
static const U32 REM_TRACE_FATAL            = 0x00000010;
static const U32 REM_TRACE_CALL_STACK       = 0x00000020;
static const U32 REM_TRACE_EXCEPTIONS       = 0x00000040;
static const U32 REM_TRACE_CAT_NONE=0; // Null category used when no category is required/relevant

// MAP < band, list(ARFCN) >
typedef std::map<U8, std::list<U16> > BANDLIST;
typedef std::list<U16> ARFCNLIST;
typedef std::list<std::string> PLMNLIST;
typedef std::list<std::string> UARFCNLIST;

#ifdef USE_RADISYS_OAM
inline void remLog(U32 level,const char* strLog)
{
    TRACE_MSG_LEV(level,strLog);
}
#else
void remLog(U32 level,const char* strLog);
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @enum __enSupportedRAT
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	RAT_LTE = 0,
	RAT_WCDMA,
	RAT_GERAN,
	RAT_CDMA,
	RAT_LTE_FREQ_SYNC,
	RAT_WCDMA_FREQ_SYNC,
	MAX_SUPPORTED_RATS
} __enSupportedRAT;

#if 0
///////////////////////////////////////////////////////////////////////////////////////////////////
//   @enum __enRemScanStatus
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	REM_INDETERMINATE = 0,
	REM_IN_PROGRES,
	REM_SUCCESS,
	REM_ERROR,
	REM_ERROR_TIMEOUT
} __enRemScanStatus;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemFromOamStartScan
//   Message from OAM to RemApp
///////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	__enSupportedRAT   enScanType;	// Scantype : LTE/WCDMA/GERAN/CDMA
} __stRemFromOamStartScan;

#if 0
///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemMsgScanStatus
//   Message from RemApp to SON/OAM
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__enSupportedRAT		enScanType;        // Scantype
	__enRemScanStatus  		enRemScanStatus;   // Scan status
	std::string				strErrorDetails; // Reason when scan status
											 // is error or timeout
} __stRemMsgScanStatus;
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
//   @enum __enRemCsgSpiltNumOfPsc
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	REM_NUM_OF_PSC_5 = 5,
	REM_NUM_OF_PSC_10 = 10,
	REM_NUM_OF_PSC_15 = 15,
	REM_NUM_OF_PSC_20 = 20,
	REM_NUM_OF_PSC_30 = 30,
	REM_NUM_OF_PSC_40 = 40,
	REM_NUM_OF_PSC_50 = 50,
	REM_NUM_OF_PSC_64 = 64,
	REM_NUM_OF_PSC_80 = 80,
	REM_NUM_OF_PSC_120 = 120,
	REM_NUM_OF_PSC_160 = 160,
	REM_NUM_OF_PSC_256 = 256
} __enRemCsgSpiltNumOfPsc;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemMsgSonStartCarrierExpScan
//   Message from SON to RemApp
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	U8			    		bScanTimeOut; 	// Maximum duration of time during
										 	// which Express scan shall be active
	U8						bNumOfEarfcn; 	// Number of carriers which should be
										 	// Measured
	U16						usnEarfcnList[REM_MAX_CARRIER_LIST];
										 	// The list of carriers to be measured
} __stRemMsgSonStartCarrierExpScan;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemMsgSonStartPciExpScan
//   Message from SON to RemApp
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	U8						bScanTimeOut; // Maximum duration of time during
								   		  // which Express scan shall be active
	U8						bNumOfPcis;   // Number of PCIs in the array
	U16						usPci[REM_MAX_PCI_LIST];
									      // Physical Cell Id of the detected
										  // EUTRA cell
} __stRemMsgSonStartPciExpScan;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemMsgSonCarrierScanRes
//   Message from RemApp to SON
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__enRemScanStatus  		enScanStatus;  	// Scan status
	std::string			   	strErrorDeatails;
											// Reason when scan status is error
										 	// or timeout
	U8				   		bNumRssi;	 	// Number of RSSI's in array
	S16				   		sNRssi[REM_MAX_RSSI_LIST];
											//Received Signal Strength Indicator
} __stRemMsgSonCarrierScanRes;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemMsgSonPciScanRes
//   Message from RemApp to SON
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__enRemScanStatus  		enScanStatus;// Scan status
	std::string			   	strErrorDeatils;
							       		 // Reason when scan status
										 // is error or timeout
	U8				   		bNumRsrp;	 // Numer of RSRP's in the array
	S16				   		sNRsrp[REM_MAX_RSSI_LIST];
										 // Received RSRP level of detected
										 // EUTRA cell, specified in dBm.
} __stRemMsgSonPciScanRes;



#if 0
///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemWcdmaCellBcchCsgSplit
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	U8					bStartPscRangeCoeff; // Value of this IE multiplied
										     // by 8 specifies the start PSC
											 // value
	__enRemCsgSpiltNumOfPsc	enNumOfPscs;	 // Number of PSCs reserved for
											 // CSG cells in each PSC range
	U8					bPscRange2Offset;	 // If present, UE shall calculate
											 // the second PSC range
} __stRemWcdmaCellBcchCsgSplit;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemWcdmaCellBcchRefPos
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	S8					bLatitude;	       // Latitude of device's position
										   // in degrees,multpilied by 1
										   // million
	U16					usLongitude;	   // Longitude of device's position
										   // in degrees,multiplied by 1
										   // million
	U8					bUncertSemiMajor;  // Uncertainty r is derived from
										   // this parameter 'k' by
										   // r = 10 * (1.1 <power> (k-1))
	U8					bUncertSemiMinor;  // Uncertainty r is derived from
	   	   	   	   	   	   	   	   	   	   // this parameter 'k' by
	   	   	   	   	   	   	   	   	   	   // r = 10 * (1.1 <power> (k-1))
	U8					bOrientMajorAxis;  // This is(N) derived by the
										   // formula: 2N <= a < 2(N+1)
	U8					bConfidence;	   	   // The degree of confidence in the
										   // ellipsoid's points expressed in
										   // percent
} __stRemWcdmaCellBcchRefPos;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemWcdmaCellBcch
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	std::string			strPlmnType;	   // Type of PLMN ,Enumeration of
										   // GSM-MAP & ANSI-41
	std::string	  		strPlmnId[REM_MAX_PLMN_IDS];
										   //PLMN Id consisting of MCC & MNC
	U16					usLac;			   // Location Area Code as defined
	U8					bRac;			   // Routing Area Code as defined
										   // in SIB1
	U32					uiCellId;		   // Cell Identity as specified in
										   // SIB3
	S16					sPcpichTxPwr;	   // Primary Common Pilot Channel
										   // (CPICH) power level on SIB 5/6
	U8					bCsgInd;		  	   // Indicates whether CSG is
	  	  	  	  	  	  	  	  	  	   // used in this cell or not
	U32					uiCsgIdentity;	   // Defines CSG ID
	std::string			strUarfcnDlList;   // Comma seperated list of UTRA
										   // ARFCNs.This list is broadcast
										   // in SIB-11 bits
	__stRemWcdmaCellBcchCsgSplit 	stWcdmaCsgPscSplitInfo;
	__stRemWcdmaCellBcchRefPos		stWcdmaBcchRefPosition;
} __stRemWcdmaCellBcch;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemWcdmaCellRf
//
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	U16					usNuarfcndl;	// UTRA ARFCN in the DL direction
										// of an FDD mode cell
	S32					icPichRscp; 	// Received signal level in dBm
										// of the CPICH channel
	S32		 			icPichEcNo;		// Measured EcNo in dB received
										// in the downlink pilot channel
	S32					iRssi;			// This RSSI measurement is for
										// Inter-frequency handover evalu
										// -ation
	U16					usPsc;			// Primary Scrambling Code
	 __stRemWcdmaCellBcch	stRemWcdmaCellBcch[REM_MAX_CELL_LIST];
} __stWcdmaRemResult;
#endif

extern bool g_reqSent;
extern bool g_ignorePhyMsg;
#endif /* REM_COMMON_H_ */
