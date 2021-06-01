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
     Desc:     Consists of REM interface related to functions
     File:     rem_fsm.h
     Sid:      rem_fsm.h@@
     Prg:      Vikram Korde 
**************************************************************************************************/

#ifndef REM_OAMINTERFACE_H_
#define REM_OAMINTERFACE_H_

#include <iostream>
#include <string>
#include <list>

#define REM_MAX_PLMN_IDS	6
#define REM_MAX_PARAM_STR_LEN	512
#define MAX_PLMN_STR_SIZE	6
#define MAX_PLMN_LIST_SIZE	64
#define MAX_INTRA_FREQ_NGH 8
#define MAX_NUM_SIBS_TO_DECODE   12

////////////////////////////////////////////////////////////////////////////////
// Input scan type for multiple scan requests, logical [or/|] operator should
// be applied Ex. remStartScan(NMM_SUPPORTED_RAT_LTE) will start LTE scan
// Ex. remStartSCan(NMM_SUPPORTED_RAT_LTE| NMM_SUPPORTED_RAT_WCDMA ) 
// -> will start LTE as well 3G scan one by one
////////////////////////////////////////////////////////////////////////////////
#define NMM_SUPPORTED_RAT_LTE        0x01
#define NMM_SUPPORTED_RAT_WCDMA      0x02
#define NMM_SUPPORTED_RAT_GERAN      0x04
#define NMM_SUPPORTED_RAT_CDMA       0x08
#define NMM_SUPPORTED_RAT_FREQ_SYNC  0x10

////////////////////////////////////////////////////////////////////////////////
// USER DEFINED DATA TYPES
////////////////////////////////////////////////////////////////////////////////

typedef unsigned char U8;
typedef char S8;
typedef unsigned short U16;
typedef signed short S16;
typedef long unsigned int U32;
typedef long signed int S32;

/* Nfft Changes */

/* Enumerations for DL Bandwidth */
/**
 *  * @enum remBandwidth
 *   * This element enumerates the Bandwidth values for PHY in terms of RBs. */
typedef enum remBandwidth
{
   REM_BW_RB_6,
   REM_BW_RB_15,
   REM_BW_RB_25,
   REM_BW_RB_50,
   REM_BW_RB_75,
   REM_BW_RB_100
} RemBandwidth;


///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemLteCarrBcchPlmnList
////////////////////////////////////////////////////////////////////////////////

typedef struct stLteBcchPlmnInfo
{
	char		szPLMN[MAX_PLMN_STR_SIZE];
	U8 bCellRsrvdForOperUse; // Indicates whether the cell is 
                            // reserved for operator use
} __stLteBcchPlmnInfo;

////////////////////////////////////////////////////////////////////////////////
// @struct __stRemLteCellBcch
// This object contains parameters relating to the PRACH 
// (Physical Random Access Channel) configuration
////////////////////////////////////////////////////////////////////////////////
typedef struct stPrachInfo
{
	U32	uiRootSequenceIndex;
	U32 uiConfigurationIndex;
	U8	bHighSpeedFlag; /* 0 - FALSE, 1 - TRUE */
	U32 uiZeroCorrelationZoneConfig;
	U32 uiFreqOffset;
} __stPrachInfo;


////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemLteCellBcch
////////////////////////////////////////////////////////////////////////////////

typedef struct stLteCellBcchData
{
	unsigned int uiDlBandwidth;  // DL transmission bandwidth
	unsigned int uiUlBandwidth;  // UL transmission bandwidth
	int          iRSTxPower;     // DL reference signal transmit power
	unsigned int uiTac;          // Tracking area code common
	unsigned int uiCellId;       // cell Identity
	U8           bCellBarred;    // Indicates whether cell is barred or not
	U8           bCsgInd;        // Indicates whether CSG is used in this cell or not
	unsigned int uiCsgIdentity;  // Defines CSG ID
	unsigned int uiPlmnListNumOfEntries; // Number of entries in the PLMNList table

	__stLteBcchPlmnInfo	stPLMNList[MAX_PLMN_STR_SIZE];

} __stLteCellBcchData;


////////////////////////////////////////////////////////////////////////////////
//   @struct stLteIntraFreqNghCellInfo
////////////////////////////////////////////////////////////////////////////////

typedef struct stLteIntraFreqNghCellInfo
{
   unsigned int    uiPhyCellId;    /* Physical Cell Id */
   int             iQOffset;       /* q-offset of neighbor cell */
}__stLteIntraFreqNghCellInfo;

typedef struct stLteSibsPeriodicity
{
   U8              numSibsToDecode;
   U32             sibsPeriodicity[MAX_NUM_SIBS_TO_DECODE];
}__stLteSibsPeriodicity;


////////////////////////////////////////////////////////////////////////////////
//   @struct __stLteCellInfo
////////////////////////////////////////////////////////////////////////////////

typedef struct stLteCellInfo
{
	unsigned int uiEARFCN;
	unsigned int uiUlEARFCN;
	unsigned int uiPhyCellID;
	int          iRSRP;
	int   	     iRSRQ;
	int   	     iRSSI;
	U8 	     bSiWindow;
	bool         bSib1Decoded;
	bool         bSib2Decoded;
   bool    bSib4Decoded;
	bool   	     bPLMNNotConfigured;
	int          iSIBRetryCount;
   U8           TxAntenna; // Number of TX antgennas
   U8           ulDlSubframeCfg; /* 0..6 (For TDD mode) */
   U8           splSfCfg; /* 0..8 (For TDD mode) */
   U8           bCpSize; // Normal CP[0] or Extended CP [1]
   U32          freqBandIndicator; /* LTE Band Indicator */
	__stPrachInfo stPrachInfo; /* PRACH (Physical Random Access Channel) Info */
	__stLteCellBcchData  stBcch;
   unsigned int    uiNumIntraFreqNghs;
   __stLteIntraFreqNghCellInfo   stNghInfo[MAX_INTRA_FREQ_NGH]; /* Intra-freq neighbors of this cell */
   __stLteSibsPeriodicity  sibsInfo;
   U16             sibsToDecode;
} __stLteCellInfo;

////////////////////////////////////////////////////////////////////////////////
// @struct __stLteConfigData   
// @brief LTE Configuration Data
////////////////////////////////////////////////////////////////////////////////

typedef struct stLteConfigData
{
	std::string strPlmnList;
	std::string strNBandList;
	std::string strNEarfcnList;
	U16	usScanTimeOut;
	U8	bMaxCellEntries;
}__stLteConfigData;


////////////////////////////////////////////////////////////////////////////////
// Functions that needs to be called by OAM
////////////////////////////////////////////////////////////////////////////////

void remStartApp(U8 bScanType=0);
void remSaveLteConfigData(__stLteConfigData & stLteCfgData);
void remStartScan(U8 bScanType);
void remScanTimerExpired(U8 bTimerType);
void remAbortScan();

////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemWcdmaCellBcchRefPos
//
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
   S32        iLatitude;            // Latitude of device's position in degree
   S32        iLongitude;           // Longitude of device's position in degree
   U32        uiUnSemiMajor;        //  
   U32        uiUnSemiMinor;        //
   U32        uiOrientMajorAxis;    //
   U32        uiConfidence;         // Ellipsoid's points expressed in percent
} __stRemWcdmaCellBcchRefPos;

////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemWcdmaCellBcchCsgSplit
//
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
   U32          uiStartPscRange;    // Start PSC of first PSC range
   U32          uiPscRange2Offset;  // Second PSC range
   char         sNumOfPscs[15];     // Number of PSCs reserved for CSG cells in
                                    // each PSC range
} __stRemWcdmaCellBcchCsgSplit;

////////////////////////////////////////////////////////////////////////////////
//   @struct __stRemWcdmaCellBcch
//
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
   char     sPlmnType[25];             // PLMN Type GSM MAP or ANSI-41
   char     szPLMN[REM_MAX_PLMN_IDS]; // PLMN Id consisting of MCC & MNC
   U32      uiLac;                    // Location Area Code as defined
   U32      uiRac;                    // Routing Area Code as defined in SIB1
   U32      uiCellId;                 // Cell Identity as specified in SIB3
   int      iPcpichTxPwr;             // (CPICH) power level on SIB 5/6
   bool     bCsgInd;                  // Indicates whether CSG is
                                      // used in this cell or not
   U32      uiCsgIdentity;            // Defines CSG ID
   char     sUarfcnDlLst[128];        // Comma seperated list of UTRA ARFCNs.

   __stRemWcdmaCellBcchCsgSplit  stWcdmaCsgPscSplitInfo;
   __stRemWcdmaCellBcchRefPos    stWcdmaBcchRefPosition;
} __stWcdmaCellBcch;

////////////////////////////////////////////////////////////////////////////////
//   @struct __stWcdmaCellInfo
//
////////////////////////////////////////////////////////////////////////////////
typedef struct stWcdmaCellInfo
{
   U16      uiUARFCNDL;
   int      iRSCP;
   int      iEcNo;
   int      iRSSI;
   U16      uiPsc;
   __stWcdmaCellBcch  stBcch;
} __stWcdmaCellInfo;

////////////////////////////////////////////////////////////////////////////////
//                         WCDMA Configuration Data
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	char    szPlmnList[REM_MAX_PARAM_STR_LEN];
	char    szNBandList[REM_MAX_PARAM_STR_LEN];
	char    szNEarfcnList[REM_MAX_PARAM_STR_LEN];
	U16		usScanTimeOut;
	U8		bMaxCellEntries;
}__stWcdmaConfigData;

////////////////////////////////////////////////////////////////////////////////
//   @enum __enRemScanStatus
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    REM_INDETERMINATE = 0,
    REM_IN_PROGRES,
    REM_SUCCESS,
    REM_ERROR,
    REM_ERROR_TIMEOUT
} __enRemScanStatus;

typedef enum
{
   REM_DECODE_SIB3  = 0x01,
   REM_DECODE_SIB4  = 0x02
}__enRemSibsToDecode;

typedef enum
{
   REM_SIB_PERIODICITY_8 = 8,
   REM_SIB_PERIODICITY_16 = 16,
   REM_SIB_PERIODICITY_32 = 32,
   REM_SIB_PERIODICITY_64 = 64,
   REM_SIB_PERIODICITY_128 = 128,
   REM_SIB_PERIODICITY_256 = 256,
   REM_SIB_PERIODICITY_512 = 512
}__enRemSibsPeriodicity;

////////////////////////////////////////////////////////////////////////////////
// Function pointers that should be initialzed given prototype
////////////////////////////////////////////////////////////////////////////////

// hook to start timer, timer value is provide in seconds
extern void (*fpStartTimer)(U8 bTimerType, U16 usScanTimeout);

// state machine may call anytime this function to stop timer
extern void (*fpStopTimer)(U8 bTimerType);

#ifdef USE_C_INTERFACE // use if want C interface
extern "C"
{
// implement the function of this prototype and send the scan status to OAM
void (*fpSendScanStatusC)(U8 bScanType, __enRemScanStatus bScanStatus, const char* strError);

// sniffed data is send to this function
void (*fpSendLteScanResultC)(int iNumOfCellsFound, __stLteCellInfo* pstCellInfo[64]);
}
#else
// implement the function of this prototype and send the scan status to OAM
extern void (*fpSendScanStatus)(U8 bScanType, __enRemScanStatus bScanStatus, std::string & strError);

// sniffed data is send to this function
extern void (*fpSendLteScanResult)(std::list<__stLteCellInfo> & m_tCellList);
#endif
/////////////////////////////////////////////////////////////////////////////////////////
// Logging function, send or write log to appropriate log file
// Enable this function when linking with rem static library
/////////////////////////////////////////////////////////////////////////////////////////
/*
void remLog(U32 level,const char* strLog)
{
}
*/

/////////////////////////////////////////////////////////////////////////////////////////
// 			SAMPLE CODE SNIPPET TO INTEGRATE WITH YOUR OAM
/////////////////////////////////////////////////////////////////////////////////////////

/* 
void oamStartTimer(U8 bTimerType, U16 usScanTimeout) { 
	// start the timer for given timer type
}

void oamStopTimer(U8 bTimerType) { 
	// stop the timer for given timer type
} 
void sendScanStatus(U8 bScanType, __enRemScanStatus bScanStatus, std::string & strError ){
	// send scan status to your OAM
}
void oamSendLteScanResult(std::list<__stLteCellInfo> & m_tCellList) {
	// send scan status to your OAM
}

int main()
{
	fpStartTimer = oamStartTimer;
	fpStopTimer = oamStopTimer;
	fpSendScanStatus = sendScanStatus;
	fpSendLteScanResult = oamSendLteScanResult;

	// Inisitalize REM
	remStartApp();

	// start the scan whenever OAM wants to trigger scan
	remStartScan(NMM_SUPPORTED_RAT_LTE);

	U8 bTimerType; // store this value receieved in oamStartTimer()
	// when timer is expired OAM should call this function
	remScanTimerExpired(bTimerType);

	// abort the scan whenver OAM wants to abort the scan
	remAbortScan();
	
}

*/

/////////////////////////////////////////////////////////////////////////////////////////
// 							END OF INTERFACE FILE
/////////////////////////////////////////////////////////////////////////////////////////
#endif /* REM_OAMINTERFACE_H_ */
