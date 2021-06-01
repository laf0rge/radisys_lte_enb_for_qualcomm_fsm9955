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

     File:     rem_wcdma_event_handler.cpp

     Sid:      rem_wcdma_event_handler.cpp@@/main/Tenb_T3k_Perf/1 - Wed Jan  9 20:53:25 2013

     Prg:      Ambika S
     	 	   Sushant K


**************************************************************************************************/
#include "rem_datatype.h"
#include "rem_eventhandler.h"
#include "rem_wcdma_event_handler.h"
#include "rem_interface.h"
#include "Pasn_BchDecodeParams.h"
#ifndef LTE_QCOM
#include "nmmcl_wcdmamsgs.h"
#endif

void decode3gBCCHMessage(void* pBuff);
void remSendWcdmaScanResult(std::list<__stWcdmaCellInfo> & tCellList);

// Mapping for find Num of PSC in CSGSplitInfo

const char *findNumPsc[16] = { "5", "10", "15", "20", "30",
                               "40", "50", "64","80", "120",
                               "160", "256", "Alltherest",
                               "Spare3", "Spare2", "Spare1"
                             };

#define TWO_POWER_23 8388608.0
#define TWO_POWER_24 16777216

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//	Constructor function to the class
//
////////////////////////////////////////////////////////////////////////////////
#ifndef LTE_QCOM
CWcdmaEvtHandler::CWcdmaEvtHandler(U16 exScanTimeout): 
                        m_objWcdmaConfigData(CRemWcdmaConfigData::getInstance())
{
	m_pCl = &m_objWcdmaPhyInterface;
	m_tBandList = m_objWcdmaConfigData.getBANDList();
	m_tBandIt = m_tBandList.begin();
   m_bch.Reset();
}
#endif
////////////////////////////////////////////////////////////////////////////////
// @ param[in] : rfInfo1 - instance of __stWcdmaCellInfo
//				 rfInfo2 - instance of __stWcdmaCellInfo
//        [out]: bool
// This function returns true if RSCP1 is greater than RSCP2, else returns false
//
////////////////////////////////////////////////////////////////////////////////
bool rscp_sort_func(__stWcdmaCellInfo rfInfo1, __stWcdmaCellInfo rfInfo2)
{
    return ( rfInfo1.iRSCP >= rfInfo2.iRSCP ) ? true : false;
}


U16 CWcdmaEvtHandler::getRssiTimeout()
{
      return m_objWcdmaConfigData.getRssiTimeout();
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: bool
// This function Copies the scanned RSSI meas result to __stWcdmaCellInfo .
// Returns a bool bariable that indicates whether list is empty or not
//
////////////////////////////////////////////////////////////////////////////////
bool CWcdmaEvtHandler::cellsAvailableForScanning()
{
	 for(m_tRssiIt = m_tRssiMeasList.begin(); m_tRssiIt != m_tRssiMeasList.end(); m_tRssiIt++ )
	 {
		 for(int i=0; i < (*m_tRssiIt).bnumOfCellsFound; i++)
	     {
			 __stWcdmaCellInfo stCellInfo={0};

          /* For BRDCM MIN_RSCP/MIN_RSSI is 0 */
			 stCellInfo.uiUARFCNDL = (*m_tRssiIt).usARFCN;
			 stCellInfo.iEcNo   = (*m_tRssiIt).stCellElm[i].ecio;
			 stCellInfo.iRSSI   = (*m_tRssiIt).bRssi;
			 stCellInfo.uiPsc   = (*m_tRssiIt).stCellElm[i].usPCI_PSC;
                         if ((*m_tRssiIt).stCellElm[i].bRSRP_RSCP <= MIN_RSCP)
                         {
                            stCellInfo.iRSCP = stCellInfo.iRSSI + stCellInfo.iEcNo;
                         }
                         else
                         {
                            stCellInfo.iRSCP = (*m_tRssiIt).stCellElm[i].bRSRP_RSCP;
                         }
			 m_tRFList.push_back(stCellInfo);
	     }
	 }
	 m_tRFList.sort(rscp_sort_func);
	 m_tRFIt = m_tRFList.begin();

	 return !m_tRFList.empty();
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: U16
// This function returns the PSC
//
////////////////////////////////////////////////////////////////////////////////
U16 CWcdmaEvtHandler::getPciOrPsc()
{
    return m_tRFIt == m_tRFList.end() ? 0 : (*m_tRFIt).uiPsc;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: U16
// This function returns the ARFCN
//
////////////////////////////////////////////////////////////////////////////////
U16 CWcdmaEvtHandler::getARFCN2()
{
    return m_tRFIt == m_tRFList.end() ? 0 : (*m_tRFIt).uiUARFCNDL;
}


U8 CWcdmaEvtHandler::getSiWindow()
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: bool
// This function checks whether any more cells are available 
// for which scanning is required
////////////////////////////////////////////////////////////////////////////////
bool CWcdmaEvtHandler::nextCellAvailable()
{
   m_bch.Reset();
   return ( ++m_tRFIt != m_tRFList.end() );
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: bool
// This function checks whether Cells are available After applying all filters
// [plmn,decoding failed] Returns True if Cells are available and false 
// if No Cells are available
////////////////////////////////////////////////////////////////////////////////
bool CWcdmaEvtHandler::cellsAvailableAfterFilter()
{
   return ( !m_tRFList.empty());
}
 
////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: char *
// This function checks for PLMN Type 
// Returns char * of PLMN Type
////////////////////////////////////////////////////////////////////////////////
char* CWcdmaEvtHandler::getBcchPlmnType(void)
{
   switch(m_bch.m_plmnType)
   {
      case PLMN_TYP_GSM_MAP :
         return ((char *) "GSM-MAP");
      case PLMN_TYP_ANSI_41 :
         return ((char *) "ANSI-41");
      case PLMN_TYP_GSM_MAP_AND_ANSI_41:
         return ((char *) "GSM-MAP and ANSI-41");
      case PLMN_TYP_SPAR:
         return ((char *) "Spare");
      default:
         return((char *) "Invalid");
   }
}
 
////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: char *
// This function checks for Number of PSCs configured in wcdam neighbor
// Returns char * of num of PSCs.
////////////////////////////////////////////////////////////////////////////////

char* CWcdmaEvtHandler::wcdmaBcchCsgSplitNumOfPscs(void)
{
   if(m_bch.m_numOfPsc < 16)
   return ((char *) findNumPsc[m_bch.m_numOfPsc]);
   else
      return((char *) "Invalid");
}
 
////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: S32
// This function calculates BCCH Reference position latitude device position
// (see tr-196 spec). Returns calculated latitude.
////////////////////////////////////////////////////////////////////////////////
S32 CWcdmaEvtHandler::calBcchRefPosLatitude(void)
{
   S32 dirMult = -1; /* NORTH needs to be reported as +ve SOUTH as -ve */
   if(m_bch.m_latiSign == 0) /* latitude is an enum of NORTH, SOUTH */
   {
      dirMult = 1; 
   }
   /* N=2^23 X /90 < N+1 ; X being the latitude and 
    *                      N is value reported in RRC Msg */

   float latitudeWithMins = (m_bch.m_latitude * 90)/TWO_POWER_23;
   float latitude = trunc(latitudeWithMins);
   float minutes = (latitudeWithMins - latitude) * 100; // * 100 as per TR-196
   TRACE_PRINTF("ellip.lati.val = %d lat = %f min = %f \n",
                 (int)m_bch.m_latitude,
                 latitude,
                 minutes);

   TRACE_PRINTF("Again lat = %f min = %f\n", latitude, minutes);

   return (S32((latitude * 1000000) + (minutes * 1000000)/60) * dirMult);
}
 
////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: S32
// This function calculates BCCH Reference position longitude device position
// (see tr-196 spec). Returns calculated longitude.
////////////////////////////////////////////////////////////////////////////////
S32 CWcdmaEvtHandler::calBcchRefPosLongitude(void)
{
   float longitudeWithMins = (m_bch.m_longitude * 360)/TWO_POWER_24;
   float longitude = trunc(longitudeWithMins);
   float minutes = (longitudeWithMins - longitude)*100; // *100 as per TR-196
   TRACE_PRINTF("ellip.lng.val = %d lng = %f min = %f \n",
                 (int) m_bch.m_longitude,
                 longitude,
                 minutes);

   minutes *= 100;
   TRACE_PRINTF("Again lng = %f min = %f\n", longitude, minutes);
                        
   return (S32((longitude * 1000000) + (minutes * 1000000)/60));
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: std::string
//	This function is used to send the scanned reult to OAM.Only scan status and 
//	error string is sent to OAM remaining results are updated to MIB
//
////////////////////////////////////////////////////////////////////////////////
std::string CWcdmaEvtHandler::sendScanResultToOAM()
{
	if( m_tRFList.empty() )
		return "Empty Cell List";
	
#if 1
	remSendWcdmaScanResult(m_tRFList);
#endif

	return "Wcdma Info";
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
//        [out]:
//	This is the handler function for BCCH DATA IND message
//
////////////////////////////////////////////////////////////////////////////////
bool CWcdmaEvtHandler::handleBcchDataInd(void* pBuff)
{

   // Check if all the required data has been received
   if(m_bch.HaveWeGotEverythingFromBch())
   {
      TRACE_PRINTF("We have got every thing!!!\n");
      saveWcdmaCellInfo();
      return true;
   }

#ifndef LTE_QCOM
   __stWcdmaNMM_3G_BCH_DATA_IND* bcchDataInd = 
      static_cast<__stWcdmaNMM_3G_BCH_DATA_IND*>(pBuff);
   m_bch.m_bcchBchCount += 1;
   m_bch.ProcessSibData(bcchDataInd->bData);
#endif

   return false;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
//        [out]:
//	This is the handler function for PBCH DATA IND message
//
////////////////////////////////////////////////////////////////////////////////
void CWcdmaEvtHandler::saveWcdmaCellInfo()
{
   // Filling Bcch Params in wcdmaCellInfo List
   m_tRFIt->stBcch.uiLac         = m_bch.m_lac;
   m_tRFIt->stBcch.uiCellId      = m_bch.m_cellId;
   m_tRFIt->stBcch.uiRac         = m_bch.m_rac;
   m_tRFIt->stBcch.iPcpichTxPwr  = m_bch.m_pcpichTxPwr;
   m_tRFIt->stBcch.bCsgInd       = m_bch.m_csgInd;
   m_tRFIt->stBcch.uiCsgIdentity = m_bch.m_csgId;
   strcpy(m_tRFIt->stBcch.sPlmnType, getBcchPlmnType());
   sprintf(m_tRFIt->stBcch.szPLMN,"%d%d",m_bch.m_mcc,m_bch.m_mnc);

   // Filling Bcch CsgSplitInfo in wcdmaCellInfo List
   m_tRFIt->stBcch.stWcdmaCsgPscSplitInfo.uiStartPscRange   = m_bch.m_startPsc;
   m_tRFIt->stBcch.stWcdmaCsgPscSplitInfo.uiPscRange2Offset = m_bch.m_pscRange;

   //function call to fill num of PSc
   strcpy(m_tRFIt->stBcch.stWcdmaCsgPscSplitInfo.sNumOfPscs,
         wcdmaBcchCsgSplitNumOfPscs());

   // Filling Bcch Referance Position in wcdmaCellInfo List

   //calculation for latitude
   m_tRFIt->stBcch.stWcdmaBcchRefPosition.iLatitude = calBcchRefPosLatitude();
   //calculation for latitude
   m_tRFIt->stBcch.stWcdmaBcchRefPosition.iLongitude
      = calBcchRefPosLongitude();
   m_tRFIt->stBcch.stWcdmaBcchRefPosition.uiUnSemiMajor = m_bch.m_unSemiMajor;
   m_tRFIt->stBcch.stWcdmaBcchRefPosition.uiUnSemiMinor = m_bch.m_unSemiMinor;
   m_tRFIt->stBcch.stWcdmaBcchRefPosition.uiOrientMajorAxis
      = m_bch.m_orieMajorAxis;
   m_tRFIt->stBcch.stWcdmaBcchRefPosition.uiConfidence = m_bch.m_confid;
}
////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
//        [out]:
//	This is the handler function for PBCH DATA IND message
//
////////////////////////////////////////////////////////////////////////////////
int CWcdmaEvtHandler::handlePbchDataInd(void* pBuff)
{
	return MIB_OK;
}

////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function gets the ScanTimeOut from WCDMA configuration read from MIB
//
////////////////////////////////////////////////////////////////////////////////
U16 CWcdmaEvtHandler::getScanTimeout()
{
   return (m_objWcdmaConfigData.getScanTimeout());
}
#ifdef ENABLE_CNM
bool CWcdmaEvtHandler::cnmSelectCellToSync(U16 *earfcn,U16 *pci)
{
   return true;
}
#endif

unsigned int CWcdmaEvtHandler::handleSib1CfgCnf(void *pBuff)
{
	return ( ((__stNmmCnf*)pBuff)->bErrorCode);
}
#ifdef LTE_TDD
U8 CWcdmaEvtHandler::getsubframeAssignment()
{
      return m_subframeAssignment;
}


U8 CWcdmaEvtHandler::getspecialSubframePatterns()
{
      return m_specialSubframePatterns;
}
#endif
