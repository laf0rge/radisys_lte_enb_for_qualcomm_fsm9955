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
 * nmm_msgs.h
 *
 *  Created on: 27-Aug-2012
 *      Author: vkorde
 */

#ifndef NMM_MSGS_H_
#define NMM_MSGS_H_

#include "LtePhyApi.h"
#include "rem_datatype.h"

void initClNmmRecvTask();


///////////////////////////////////////////////////////////////////////////////////////////////////
//   @struct __stLteNMM_START_RSP
///////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	U8		bSubFrameNumber;  	// sub frame number
	U8		bErrorCode;		  	// Error code value
	U8		bMsgType;
	U8  	bPhyEntityId;		// PHY Identifictaion in multi-PHY configuration
	U16 	usMsgLength;
	U16		usFrameNo;			// System Frame Number
} __attribute__ ((__packed__)) __stNMM_RSP_HDR;


typedef struct
{
	U16 msgLength;
	U8 	msgType;
	U8 	PhyEntityId;
} __attribute__ ((__packed__)) __stNMM_HEADER;


typedef struct
{
	U16 usSupportedRAT;
    U16 usDuplex;
    U16 reverse1;
    U16 mode;
    U8  reverse2;
    U8  reverse3;
    U32 count;
    U32 period;
} __attribute__ ((__packed__)) __stNMM_START_REQ;

typedef struct
{
	U8 channelBandwidth;
	U8 frtypeDuplexmode;
	U8 radioAccessMode;
	U8 physicalResourceBandwidth;
	U8 numberResourceBlocksperSlot;
	U8 phyLayerCellIdGroup;
	U8 phyLayerId;
	U8 txAntennaPortCount;
	U16 fastfwdorPreclockingNumber;
	U16 fftSize;
	U16 numberUsedSubcarriers;
	U8 nMaxDlRb;
	U8 nMaxUlRb;
	S16 referenceSignalPower;
	S16 primarySyncSignalPower;
	S16 secondarySyncSignalPower;
	U8 numDataRePerPRB;
	U8 cyclicPrefixType;

#if 0
	BIT_FIELD rxAntennaPortCount :8;
	BIT_FIELD phyCfg :4;
	BIT_FIELD reserved :11;
	BIT_FIELD rxSduLineMonitorInfoenable :1;
	BIT_FIELD txStartConfDisable :1;
	BIT_FIELD rxStartConfDisable :1;
	BIT_FIELD pb :2;
	BIT_FIELD customExtensionReportEn :1;
	BIT_FIELD rachReportMode :1;
	BIT_FIELD txSduConfEnable :1;
	BIT_FIELD sduConfig :1;
#endif

	U8 rxAntennaPortCount;
	U8 dummy[3];

	U16 radioFrameNumber;
	U8 subframeNumber;
	U8 slotNumber;
	S16 srsBandwidthConfig;
	S16 srsSubframeConfig;
	S16 srsSimultaneousANandSRS;
	S16 prachConfigurationIndex;
	S16 prachFrequencyOffset;
	S16 prachHighSpeedFlag;
	S16 prachCyclicShiftConfig;
	S16 prachRootSequenceIndex;
	S16 phichResource; 
	S16 phichDuration; 
}__attribute__ ((__packed__)) __stPHY_INIT;


#if 0
typedef struct
{
    U8 bFreqBandIndicator; /*The E-UTRA band for which the carrierList applies. */
    U8 bMeasurementPeriod ; /*The length of time the NMM measure RSSI over. In 10ms radio frames. */
    U8 bEARFCNList; /*The number of carriers which should be measured. In case no carrier (value 0) is specified all carriers in the band shall be measured. */
    U16 usEARFCNList[NMM_LTE_MAX_CARRIER_LIST];/*The list of carriers to be measured.  */
    U8  bVendorSpecificList;/*The number of elements in the next array. */
    U16 usVendorSpecificList[MAX_VENDOR_LIST];
} __stNMM_RSSI_MEAS_REQ;

typedef struct {
    U16 earfcn;
    U8  exhaustive_search;
    U8  nr_pci_list;
    U16 pci_list[MAX_PCI_LIST];
    U8  meas_bandwidth;
    U8  measurement_period;
    U8  nr_vendor_specific_list;
    U16 vendor_specific_list[MAX_VENDOR_LIST];
} __stNMM_CELL_SEARCH_REQ;

typedef struct {
    U16 pci;
    U16 earfcn;
    U8  nr_vendor_specific_list;
    U16 vendor_specific_list[MAX_VENDOR_LIST];
} __stNMM_PBCH_CFG_REQ;

typedef struct {
    U8 retry_count;
    U8  nr_vendor_specific_list;
    U16 vendor_specific_list[MAX_VENDOR_LIST];
} __stNMM_SIB_CFG_REQ;

typedef struct {

    U8  nr_siPeriodicityList;
    U16 siPeriodicityList[MAX_SI_LIST];
    U8  siWindow;
    U8  retry_count;
    U8  nr_vendor_specific_list;
    U16 vendor_specific_list[MAX_VENDOR_LIST];
} __stNMM_BCCH_CFG_REQ;
#endif
#endif /* NMM_MSGS_H_ */
