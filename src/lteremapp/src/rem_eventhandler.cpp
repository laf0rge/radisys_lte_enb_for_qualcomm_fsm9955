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

     Type:     C++ Source file

     Desc:     This file consists of the event handler functions that are common to LTE & WCDMA

     File:     rem_eventhandler.cpp

     Sid:      rem_eventhandler.cpp@@/main/Tenb_Intg_Branch_CC/TeNB_Dev_REM_Branch/1 - Wed Jan  9 12:01:26 2013

     Prg:      Vikram K
      	  	   Ambika S
      	  	   Sushant K

**************************************************************************************************/

#include "rem_datatype.h"
#include "rem_eventhandler.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This is the default destructor
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CRemEvtHandler::~CRemEvtHandler()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function checks if there is band available in the bandlist
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRemEvtHandler::nextBandAvailable()
{
    return ( ++m_tBandIt != m_tBandList.end() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function gets the first band in the Bandlist
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 CRemEvtHandler::getBand()
{
	if( m_tBandIt == m_tBandList.end() )
		return 0;

	return (*m_tBandIt).first;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function gets the number of(count) bands in the list
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 CRemEvtHandler::getARFCNCount()
{
	if( m_tBandIt == m_tBandList.end() )
		return 0;

	return ((*m_tBandIt).second).size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function copies the ARFCN List to a local array and returns the same
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U16* CRemEvtHandler::getARFCNList()
{
    static U16 uiEARFCN[REM_MAX_CARRIER_LIST];

 	if( m_tBandIt == m_tBandList.end() )
 		return NULL;

 	ARFCNLIST & arfcnList  = (*m_tBandIt).second;

 	if( arfcnList.size() == 0)
   {
 	    return NULL;
   }

 	std::copy( arfcnList.begin(), arfcnList.end(), uiEARFCN);

 	return uiEARFCN;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : rssiMeas1,rssiMeas2- RSSI measurement results to be sorted
//
// This function returns the RSSI with higher value
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool rssi_sort_func(__stRssiMeasResult rssiMeas1, __stRssiMeasResult rssiMeas2)
{
    return ( rssiMeas1.bRssi >= rssiMeas2.bRssi ) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function sorts the RSSI list based on its strength.List is maintained in descending order
// of RSSI
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRemEvtHandler::sortARFCNWithStrongestRSSI()
{
	// If neighor cells are not available
	if( m_tRssiMeasList.empty() )
		return false;

	m_tRssiMeasList.sort(rssi_sort_func);
	m_tRssiIt = m_tRssiMeasList.begin();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function checks if there is arfcn available in the list
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRemEvtHandler::nextARFCNAvailable()
{
    return ( ++m_tRssiIt != m_tRssiMeasList.end() );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function gets the ARFCN from the list
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U16 CRemEvtHandler::getARFCN()
{
    if( m_tRssiIt == m_tRssiMeasList.end() )
		return 0;

	return (*m_tRssiIt).usARFCN;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function returns false because Carrier Express Scan Flag can be enabled only by LTE Event
// handler.SON Express scan is applicable only for LTE.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRemEvtHandler::isCarrierExprScanEnabled()
{
	return false; // Because only LTE event handler should return true
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function returns false because PCI Express Scan Flag can be enabled only by LTE Event
// handler.SON Express scan is applicable only for LTE.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRemEvtHandler::isPciExpScanEnabled()
{
	return false; // Because only LTE event handler should return true
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : pRssiInd : pointer to RSSI Indication Structure
//
// This function copies the ARFCN and RSSI returned by REM-CL to its local structure and prepares
// a list of received RSSI Meas Result .This function is common for both LTE and WCDMA
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemEvtHandler::handleRssiInd(__stRssiMeasResult* pRssiInd)
{
	m_tRssiMeasList.push_back(*pRssiInd);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : pRssiInd : pointer to Cell Search Indication Structure
//
// This function copies the ARFCN and Cell Search Indication returned by REM-CL to its
// local structure and prepares a list of received Cell search Indication .
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRemEvtHandler::handleCellSearchInd(__stNmmCellSearchIndEvt* pCellSearchInd)
{
	U8 bIdx=(*m_tRssiIt).bnumOfCellsFound;
   (*m_tRssiIt).bnumOfCellsFound += pCellSearchInd->bnumOfCellsFound;

   for(int iCellIdx=0;  iCellIdx <  pCellSearchInd->bnumOfCellsFound; bIdx++, iCellIdx++ )
   {
      (*m_tRssiIt).stCellElm[bIdx] =  pCellSearchInd->stCellElm[iCellIdx];
      (*m_tRssiIt).bCpSize[bIdx] =  pCellSearchInd->stCellAddElmnt[iCellIdx].bCpSize;
   }
}

