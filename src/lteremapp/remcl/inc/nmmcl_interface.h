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
 * remcl_interface.h
 *
 *  Created on: 06-Aug-2012
 *      Author: vikram korde
 */

#ifndef REM_INTERFACE_H_
#define REM_INTERFACE_H_

#include "rem_events.h"
#include "nmm_msgs.h"
#include "LtePhyApi.h"

#include <string.h>

#define RSRP_THRESHOLD_TO_IGNORE	-105
#define RSCP_THRESHOLD_TO_IGNORE	-105
#define ECIO_THRESHOLD_TO_IGNORE	-20

#define MIN_RSCP    -140
#define MIN_RSRP    -144

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   @class CNmmPhyInf
//
///////////////////////////////////////////////////////////////////////////////////////////////////
class CNmmPhyInterface
{
public:
	// derived calls shall overwrite below funtions
     virtual void sendNmmStart() =0;
     virtual void sendNmmRssiMeasReq(U8 bBandClass, U8 bArfcnCount, U16* usARFCNList)=0;
     virtual void sendNmmCellSearchReq(U16 usEARFCN, U8 bPCICount, U16 *usPCIList) = 0;
     virtual void sendNmmPbchCfgReq(U16 usPCIorPSC, U16 usARFCN) = 0;
     virtual void sendNmmSib1CfgReq() = 0;
     virtual void sendNmmSib1CfgReq(U16 usPCIorPSC, U16 usARFCN) = 0;
     virtual void initProvXmlFile( U8 dlBw, U16 bandSupported, U8 supportedTech ) = 0;
#ifndef LTE_TDD
     virtual void sendNmmBcchCfgReq(U8 bSiWindow, __stLteSibsPeriodicity *sibsInfo) = 0;
#else
     virtual void  sendNmmBcchCfgReq(U8 bSiWindow,U8 subframeAssignment,
                       U8 specialSubframePatterns, __stLteSibsPeriodicity *sibsInfo)=0;
#endif

     virtual void sendNmmStopRssiMeasReq()=0;
     virtual void sendNmmStopCellSearchReq()=0;
     virtual void sendNmmStopBcchReq()=0;
     virtual void sendNmmStopPbchReq()=0;
	  virtual void sendNmmFreqSyncLock(U16 usEARFCN, U16 usPCI){}

     // Below funtions shall be common for LTE or WCMDA as structures for these does not exist
     void sendNmmAbort();
     void sendNmmStop();
     bool skipPbchMsg();
     void sendMsgToPhy(U16 usMsgType, U32 uiMsgLen, int iDest=0);

public:
	 CNmmPhyInterface();
	 virtual ~CNmmPhyInterface() { };

public:
     //MSGHEADER *m_nmmTxdata; /*NMM message transmit structure*/

     U8 *m_pNmmMsg, *m_pNmmStartMsg;
     U8	m_bMeasurementPeriod; // to be initialized in LTE or WCDMA constructor
     bool m_bExpressScan;
     //bool m_bSkipPbch;

     unsigned char m_txBuff[16384];
     //__stNMM_START_REQ* m_pstNmmStart;
     CPHY_MSG_HEADER* 	m_pstNmmHdr;
     //__stPHY_INIT*		m_pstPhyInit;
};



#endif /* REM_INTERFACE_H_ */
