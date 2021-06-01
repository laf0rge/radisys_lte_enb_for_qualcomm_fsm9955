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
/*
 * nmmcl_ltemsgs.h
 *
 *  Created on: Aug 29, 2012
 *      Author: ashetty
 */

#ifndef NMMCL_LTEMSGS_H_
#define NMMCL_LTEMSGS_H_

#include "LtePhyApi.h"
#include "rem_events.h"
#include "nmm_msgs.h"

#define MAX_CELL_FOUND_LIST 16
#define MAX_BCCH_DATA_LIST  32
#define MAX_BCCH_SIZE		2048

typedef __stNMM_RSP_HDR __stLteNMM_START_RSP;
typedef __stNMM_RSP_HDR __stLteNMM_STOP_RSP;
typedef __stNMM_RSP_HDR __stLteNMM_RSSI_MEAS_REQ_RESP;
typedef __stNMM_RSP_HDR __stLteNMM_STOP_RSSI_MEAS_REQ_RESP;
typedef __stNMM_RSP_HDR __stLteNMM_STOP_CELL_SEARCH_REQ_RESP;
typedef __stNMM_RSP_HDR __stLteNMM_STOP_PBCH_REQ_RESP;
typedef __stNMM_RSP_HDR __stLteNMM_STOP_BCCH_REQ_RESP;
typedef __stNMM_RSP_HDR __stLteNMM_CELL_SEARCH_REQ_RESP;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stLteNMM_RSSI_MEAS_CNF
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__stNMM_RSP_HDR stNmmRspHdr;		// Common Header to all the responses from PHY
	U8	bNrVendSpecList;	// Number of elements in the next array
	U16	usVendSpecList[MAX_VENDOR_LIST]; // Vendor specific information
} __attribute__ ((__packed__)) __stLteNMM_RSSI_MEAS_CNF;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stNMM_RSSI_MEAS_IND_EVT
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
        __stNMM_RSP_HDR         stNmmRspHdr;
        U8                      bRssi;
        U16                     usEARFCN;
        U8                      bNrVendSpecList;     // Number of elements in the next array
        U16                     usVendSpecList[MAX_VENDOR_LIST];        // Vendor specific information

} __attribute__ ((__packed__)) __stNMM_RSSI_MEAS_IND_EVT;


///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stLteCellFoundElmnt
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	U8		bRsrp; // RSRP in dBFS
	U16		usPci; // Physical cell Identity of the cell which the NMM is synchronized to
}  __attribute__ ((__packed__))  __stLteCellFoundElmnt;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stLteNMM_CELL_SEARCH_IND
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__stNMM_RSP_HDR stNmmRspHdr;
	U8 bnrCellFoundList; // Number of cells indicated in this message
	__stLteCellFoundElmnt  stCellFoundElmnt[MAX_CELL_FOUND_LIST];
	U8 bnrVendSpecList;	// Number of elements in the next array
	U16	usVendSpecList[MAX_VENDOR_LIST]; // Vendor specific information
} __attribute__ ((__packed__)) __stLteNMM_CELL_SEARCH_IND;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stLteNMM_CELL_SEARCH_CNF
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__stNMM_RSP_HDR stNmmRspHdr;
	U8		bNrVendSpecList;	// Number of elements in the next
								// array
	U16		usVendSpecList[MAX_VENDOR_LIST];
								// Vendor specific information
} __attribute__ ((__packed__)) __stLteNMM_CELL_SEARCH_CNF;


///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stLteNMM_PBCH_CONFIG_CNF
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__stNMM_RSP_HDR stNmmRspHdr;
	U8		bNrVendSpecList;	// Number of elements in the next
								// array
	U16		usVendSpecList[0];
} __attribute__ ((__packed__)) __stLteNMM_PBCH_CONFIG_CNF;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stLteNMM_PBCH_DATA_IND
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__stNMM_RSP_HDR stNmmRspHdr;
	U16		usPci;				// Physical cell Identity of the
								// cell which the NMM is synchronized to
	U8		bCause;				// Indicates if MIB reading was successful
								// or not . Incase reading failed,the next
								// parameters are invalid
	U8		bNumTx;				// Number of transmit antennas of the cell
	U16		usSfn;				// The system frame number of the cell
	U8		bPhichDuratn;		// The PHICH duration of the cell
	U8		bPhichRsrc;			// The PHICH resource size of the cell
	U8		bDlSysBw;			// DL transmission bandwidth in RBs
	U8		bNrVendSpecList;	// Number of elements in the next array
	U16		usVendSpecList[MAX_VENDOR_LIST];
								// Vendor specific information

} __attribute__ ((__packed__)) __stLteNMM_PBCH_DATA_IND;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stLteNMM_SIB1_CONFIG_CNF
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__stNMM_RSP_HDR stNmmRspHdr;
	U8		bNrVendSpecList;	// Number of elements in the next array
	U16		usVendSpecList[0];	// Vendor specific information
} __attribute__ ((__packed__)) __stLteNMM_SIB1_CONFIG_CNF;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stbcchDataElmnt
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	U16		usLen;				// Length of the BCCH payload in byte
	U16		usSfn;				// The radio frame the data was
								// received on
	U8		bSf;				// The subframe the data was received on
	U8		bData[MAX_BCCH_SIZE];// The BCCH payload
}  __attribute__ ((__packed__)) __stBcchDataElmnt;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stLteNMM_BCCH_CONFIG_CNF
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__stNMM_RSP_HDR stNmmRspHdr;
	U8		bNrVendSpecList;	// Number of elements in the next array
	U16		usVendSpecList[0];	// Vendor specific information
} __attribute__ ((__packed__)) __stLteNMM_BCCH_CONFIG_CNF;

///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stLteNMM_BCCH_DATA_IND
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	__stNMM_RSP_HDR 	stNmmRspHdr;
	U8					bNumBcchDataList;	// Number of BCCHs in this message
	// List of decode BCCH information, the number is indicated by bnrbcchDataList
	__stBcchDataElmnt 	stBcchDataElmnt[MAX_BCCH_DATA_LIST];
	U8					bNrVendSpecList;	// Number of elements in the next array
	U16					usVendSpecList[0];	// Vendor specific information
} __attribute__ ((__packed__)) __stLteNMM_BCCH_DATA_IND;

#endif /* NMMCL_LTEMSGS_H_ */
