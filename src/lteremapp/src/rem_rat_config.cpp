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


/************************************************************************

     Name:     REM-APP

     Type:     C++ Source file

     Desc:     This file consists of the fsm functions for REM-APP

     File:     rem_rat_config.cpp

     Sid:      rem_rat_config.cpp@@/main/tenb_main_ccb/1 - Thu Jan 21 17:51:27 2016

     Prg:      


************************************************************************/

#include "rem_datatype.h"
#include "rem_rat_config.h"
#include "rem_common.h"
#include <stdlib.h>
#ifdef USE_RADISYS_OAM
#include "Trace.h"
#endif


CRemRatConfigData::CRemRatConfigData()
{
	m_usScanTimeout = 300;
	m_usRssiTimeout = 0;
	m_bMaxCellEntry = 64;
}


////////////////////////////////////////////////////////////////////////////////
// @ param[in] plmnList - Comma separated PLMNId list
//
// This function extracts the PLMN Ids from a comma separated list and then finds respective 
// band to add it in local cache.
////////////////////////////////////////////////////////////////////////////////
void CRemRatConfigData::savePlmnList(std::string strPlmnList)
{
	size_t	commaPos;  //coma position
	size_t  currPos = 0;
	std::string strPlmn;

   if( strPlmnList.empty() )
      return;

   TRACE_PRINTF("PLMN LIST: %s\n", strPlmn.c_str());

	while((commaPos = strPlmnList.find(',',currPos))!= std::string::npos)
	{
		strPlmn = strPlmnList.substr(currPos, commaPos-currPos);
		currPos = commaPos+1;
		m_tlPLMNList.push_back(strPlmn);
	}

   strPlmn = strPlmnList.substr(currPos, commaPos);
   m_tlPLMNList.push_back(strPlmn);
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] earfcnList - Comma separated EARFCN list
//
// This function extracts the EARFCNs from a comma separated list and then finds respective 
// band to add it in local cache.
////////////////////////////////////////////////////////////////////////////////
void CRemRatConfigData::saveEARFCNList(std::string strEARFCNList)
{
	size_t commaPos; // coma position
	size_t currPos = 0;
	std::string strEARFCN;

	if( strEARFCNList.empty() )
		return;
#ifdef ENABLE_REM_LOGS
   TRACE_PRINTF("EARFCN LIST: %s\n", strEARFCNList.c_str());
#endif
	while( (commaPos = strEARFCNList.find(',', currPos)) != std::string::npos )
	{
		strEARFCN = strEARFCNList.substr(currPos, commaPos-currPos);
		currPos = commaPos+1;
		addEARFCN((U16) atoi(strEARFCN.c_str()));
	}

	strEARFCN = strEARFCNList.substr(currPos, commaPos);
	addEARFCN((U16) atoi(strEARFCN.c_str()));

	BANDLIST::iterator it;
	for(m_usRssiTimeout = 0, it=m_tmBandList.begin(); it != m_tmBandList.end(); it++ )
		m_usRssiTimeout += (it->second).size() * RSSI_TIMEOUT_PER_EARFCN;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] usEARFCN - EUTRA ARFCN
//
// This function adds band that corresponds to EARFCN read from MIB to the bandlist
////////////////////////////////////////////////////////////////////////////////
void CRemRatConfigData::addEARFCN(U16 usEARFCN)
{
	U8   band = getBand(usEARFCN);
	if( band )
		m_tmBandList[band].push_back(usEARFCN);
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] iTimeout
//
// This function populates the timeout configuration parameter to member variable
////////////////////////////////////////////////////////////////////////////////
void CRemRatConfigData::saveScanTimeout(U16 usTimeout)
{
	m_usScanTimeout = usTimeout;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] cellEntry
//
// This function populates the cellEntry configuration parameter to member variable
////////////////////////////////////////////////////////////////////////////////
void CRemRatConfigData::saveMaxCellEntry(U8 cellEntry)
{
	m_bMaxCellEntry = cellEntry;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//        [out] BANDLIST - bandlist
// This function returns the bandlist
////////////////////////////////////////////////////////////////////////////////
BANDLIST & CRemRatConfigData::getBANDList()
{
	if( m_tmBandList.empty() )
   {
		configureAllBands();
   }
	return m_tmBandList;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//        [out] PLMNLIST - plmn list
// This function returns the plmn list
////////////////////////////////////////////////////////////////////////////////
const PLMNLIST & CRemRatConfigData::getPLMNList()
{
	return m_tlPLMNList;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//        U16 - scanTimeOut
// This function returns the scan time out
////////////////////////////////////////////////////////////////////////////////
U16 CRemRatConfigData::getScanTimeout()
{
	return m_usScanTimeout;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//      Void getRssiTimeout
// This function get RSSI Time out Info
////////////////////////////////////////////////////////////////////////////////

U16 CRemRatConfigData::getRssiTimeout()
{
	return m_usRssiTimeout;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//      Void saveScanOnBootInfo
// This function saves scan on boot Info
////////////////////////////////////////////////////////////////////////////////

void CRemRatConfigData::saveScanOnBootInfo(bool ratInfo)
{
   m_bratPres = ratInfo;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
//      Void getScanOnBootInfo
// This function get scan on boot Info from savelist
////////////////////////////////////////////////////////////////////////////////

bool CRemRatConfigData::getScanOnBootInfo(void)
{
   return(m_bratPres);
}

