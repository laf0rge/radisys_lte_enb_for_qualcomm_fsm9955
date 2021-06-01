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

     File:     rem_lte_event_handler.cpp

     Sid:      rem_lte_event_handler.cpp@@/main/Tenb_Intg_Branch_CC/TeNB_Dev_REM_Branch/2 - Fri Jan 11 16:47:22 2013

     Prg:      Vikram K
      	  	   Ambika S


**************************************************************************************************/
#include <string.h>
#include <math.h>
#include "rem_datatype.h"
#include "rem_lte_event_handler.h"
#include "rem_lteconfig.h"
#include <cstring>

#define REM_ENABLE_DEBUG
#define MAX_SIB_RETRYS_ALLOWED 8

void decodeLteBCCHMessage(__stLteNMM_BCCH_DATA_IND* pstBcchData, __stLteCellInfo & stCellInfo);

void remSendLteLastScanTime();
void remSendLteScanResult(std::list<__stLteCellInfo> & tCellList);
///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : exScanTimeout - Scan TimeOut
//
// Constructor to CLteEvtHandler class
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CLteEvtHandler::CLteEvtHandler(U16 exScanTimeout):m_objLteConfigData(CRemLteConfigData::getInstance())
{
	m_pCl = &m_objLtePhyInterface;
	m_tBandList = m_objLteConfigData.getBANDList();
	m_tlPLMNList = m_objLteConfigData.getPLMNList();
	m_tBandIt = m_tBandList.begin();
	m_bIsPciExpScanEnabled = false;
	m_bIsCarrierExprScanEnabled = false;
	m_bPCICount = 0;
	m_usExpScanTimeout = exScanTimeout; // Express Scan Timeout
	m_bEARFCNCount = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//
// This function gets the ScanTimeOut from LTE configuration read from MIB
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U16 CLteEvtHandler::getScanTimeout()
{
	return m_usExpScanTimeout ? m_usExpScanTimeout : m_objLteConfigData.getScanTimeout();
}


U16 CLteEvtHandler::getRssiTimeout()
{
	return m_objLteConfigData.getRssiTimeout();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : bEARFCNCount - EARFCN Count
//				 usEARFCNList - EARFCN List
// This function copies EARFCN Count and list to the member variables
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CLteEvtHandler::saveCarrierList(U8 bEARFCNCount, U16* usEARFCNList)
{
	m_bEARFCNCount = bEARFCNCount;
	std::copy(usEARFCNList,usEARFCNList+ bEARFCNCount ,m_usEARFCNList);
	m_bIsCarrierExprScanEnabled = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : bPCICount - PCI Count
//				 usPCIList - PCI List
// This function copies PCI Count and list to the member variables
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CLteEvtHandler::savePciList(U8 bPCICount, U16* usPCIList)
{
	m_bPCICount = bPCICount;
	std::copy( usPCIList, usPCIList + bPCICount, m_usPCIList);
	m_bIsPciExpScanEnabled = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: bool
// This function returns isSib1Decoded
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CLteEvtHandler::isSib1Decoded()
{
	return((*m_tCellIt).bSib1Decoded);
}

void CLteEvtHandler::prepareNmmSibsPeriodicity(__stLteSibsPeriodicity  *sibsInfo)
{

   int count;
   for(count = 0; count < (*m_tCellIt).sibsInfo.numSibsToDecode; count ++)
   {
      sibsInfo->sibsPeriodicity[count] = 
        (*m_tCellIt).sibsInfo.sibsPeriodicity[count]; 
   }  
   sibsInfo->numSibsToDecode = (*m_tCellIt).sibsInfo.numSibsToDecode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: bool
// This function returns the CarrierExpScanFlag
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CLteEvtHandler::isCarrierExprScanEnabled()
{
	return m_bIsCarrierExprScanEnabled;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: bool
// This function returns the PciExpScanFlag
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CLteEvtHandler::isPciExpScanEnabled()
{
	return m_bIsPciExpScanEnabled;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: __enSupportedRAT
// This function returns the RAT Type (LTE/WCDMA/GERAN/CDMA)
//
///////////////////////////////////////////////////////////////////////////////////////////////////
inline __enSupportedRAT CLteEvtHandler::getRATType()
{
    return RAT_LTE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : rfInfo1 - instance of __stLteCellInfo
//				 rfInfo2 - instance of __stLteCellInfo
//        [out]: bool
// This function returns true if RSRP1 is greater than RSRP2, else returns false
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool rsrp_sort_func(__stLteCellInfo rfInfo1, __stLteCellInfo rfInfo2)
{
    return ( rfInfo1.iRSRP >= rfInfo2.iRSRP ) ? true : false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: bool
// This function Copies the scanned RSSI meas result to __stLteCellInfo . Returns a bool bariable
// that indicates whether list is empty or not
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CLteEvtHandler::cellsAvailableForScanning()
{
	for(m_tRssiIt = m_tRssiMeasList.begin(); m_tRssiIt != m_tRssiMeasList.end(); m_tRssiIt++ )
	{
		for(int i=0; i < (*m_tRssiIt).bnumOfCellsFound; i++)
		{
			__stLteCellInfo stCellInfo={0};
         double rsrq = 0; 
         /* Calculating RSRQ with taking MeasBandwidth of RSSI to be 50 */
         //if((*m_tRssiIt).stCellElm[i].bRSRP_RSCP != 0)
         //rsrq = ((double)(*m_tRssiIt).stCellElm[i].bRSRP_RSCP / (*m_tRssiIt).bRssi ) * (*m_tRssiIt).stCellElm[i].dlBandwidth; 
         /* for BRDCM MIN_RSSI/MIN_RSRP is 0*/         
			stCellInfo.uiEARFCN = (*m_tRssiIt).usARFCN;
			stCellInfo.iRSSI = (*m_tRssiIt).bRssi;
			stCellInfo.uiPhyCellID = (*m_tRssiIt).stCellElm[i].usPCI_PSC;
         stCellInfo.iRSRP = (*m_tRssiIt).stCellElm[i].bRSRP_RSCP;
#ifdef LTE_QCOM
         rsrq = g_updatersrq[i];
#elif BRDCM
         rsrq = 10*log((*m_tRssiIt).stCellElm[i].dlBandwidth) + stCellInfo.iRSRP - stCellInfo.iRSSI;
#else
         rsrq = ((double)(*m_tRssiIt).stCellElm[i].bRSRP_RSCP / (*m_tRssiIt).bRssi ) * 100; 
#endif
         stCellInfo.iRSRQ = (int)rsrq; 
			stCellInfo.bSib1Decoded = false;
			stCellInfo.bSib2Decoded = false;
			stCellInfo.bSib4Decoded = false;
			stCellInfo.bPLMNNotConfigured = false;
			stCellInfo.iSIBRetryCount = 0;
			stCellInfo.bSiWindow = SI_WINDOW;
			stCellInfo.TxAntenna = (*m_tRssiIt).stCellElm[i].TxAntenna;
			stCellInfo.ulDlSubframeCfg = 0;
			stCellInfo.splSfCfg = 0;
			stCellInfo.bCpSize = (*m_tRssiIt).bCpSize[i];
#ifdef BRDCM
			stCellInfo.stBcch.uiDlBandwidth = (*m_tRssiIt).stCellElm[i].dlBandwidth;
#else
			stCellInfo.stBcch.uiDlBandwidth = 0;
#endif
			stCellInfo.stBcch.uiDlBandwidth = (*m_tRssiIt).stCellElm[i].dlBandwidth;
			stCellInfo.stBcch.uiUlBandwidth = 0;
			stCellInfo.stBcch.iRSTxPower = 0;
			stCellInfo.stBcch.uiTac = 0;
			stCellInfo.stBcch.uiCellId = 0;
			stCellInfo.stBcch.bCellBarred = 0;
			stCellInfo.stBcch.bCsgInd = 0;
			stCellInfo.stBcch.uiPlmnListNumOfEntries = 0;
			

			m_tCellList.push_back(stCellInfo);
		}
	}

	m_tCellList.sort(rsrp_sort_func);
	m_tCellIt = m_tCellList.begin();

	return !m_tCellList.empty();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: U16
// This function returns the PCI
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U16 CLteEvtHandler::getPciOrPsc()
{
    return m_tCellIt == m_tCellList.end() ? 0 : (*m_tCellIt).uiPhyCellID;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: U16
// This function returns the ARFCN
//
///////////////////////////////////////////////////////////////////////////////////////////////////
U16 CLteEvtHandler::getARFCN2()
{
    return m_tCellIt == m_tCellList.end() ? 0 : (*m_tCellIt).uiEARFCN ;
}

U8 CLteEvtHandler::getSiWindow()
{
    return m_tCellIt == m_tCellList.end() ? SI_WINDOW : (*m_tCellIt).bSiWindow ;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: bool
// This function checks whether any more cells are available for which scanning is required
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CLteEvtHandler::nextCellAvailable()
{
	return ( ++m_tCellIt != m_tCellList.end() );
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: bool
// This function checks whether Cells are available After applying all filters [plmn,decoding failed]
// Returns True if Cells are available and false if No Cells are available
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CLteEvtHandler::cellsAvailableAfterFilter()
{
	return ( !m_tCellList.empty());
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
//        [out]: void
// This function copies Data received in PBCH DATA IND to member variables
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int CLteEvtHandler::handlePbchDataInd(void* pBuff)
{
#ifndef BRDCM
	TRACE_PRINTF("PBCH DATA IND [Cause: %d PCI %d DL BW %d]\n",
                 ((__stLteNMM_PBCH_DATA_IND*)pBuff)->bCause,
 	              ((__stLteNMM_PBCH_DATA_IND*)pBuff)->usPci,
                 ((__stLteNMM_PBCH_DATA_IND*)pBuff)->bDlSysBw);

	if( ((__stLteNMM_PBCH_DATA_IND*)pBuff)->bCause == MIB_FAILED )
		return MIB_FAILED;

	(*m_tCellIt).stBcch.uiDlBandwidth  = ((__stLteNMM_PBCH_DATA_IND*)pBuff)->bDlSysBw;
#endif

	return MIB_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
//        [out]: bool - by default returns false. Returns true if bSib1Decoded or bSib2Decoded
//						are set
// This function copies Data received in BCCH DATA IND to member variables
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CLteEvtHandler::handleBcchDataInd(void* pBuff)
{
	__stLteNMM_BCCH_DATA_IND* pstBcchData = (__stLteNMM_BCCH_DATA_IND*)pBuff;

	__stLteCellInfo & stCell = *m_tCellIt;

   decodeLteBCCHMessage(pstBcchData, stCell);

   ++stCell.iSIBRetryCount;

	TRACE_PRINTF("BCCH DATA DECODED [SIBRetryCount:%d SIB1:%s SIB2:%s SIB4:%s]\n",
                 stCell.iSIBRetryCount,
			        stCell.bSib1Decoded ? "DECODED" : "FAILED",
                 stCell.bSib2Decoded ? "DECODED" : "FAILED",
                 stCell.bSib4Decoded ? "DECODED" : "FAILED");
   if((stCell.sibsToDecode & REM_DECODE_SIB3) && 
               (stCell.sibsToDecode & REM_DECODE_SIB4))
   {
      if( stCell.bSib1Decoded &&  stCell.bSib2Decoded  && stCell.bSib4Decoded)
      {
         return true;
      }
   }
   else
   {
      if( stCell.bSib1Decoded &&  stCell.bSib2Decoded)
      {
         return true;
      }
   }

   if( stCell.iSIBRetryCount == MAX_SIB_RETRYS_ALLOWED )
	   return true;

   return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : & __stLteCellInfo
//        [out]: void
//	This function checks whether the PLMN belongs to the one configured
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CLteEvtHandler::applyPLMNFilter(__stLteCellInfo & cellInfo)
{
   U8 numOfPlmnEntry = cellInfo.stBcch.uiPlmnListNumOfEntries;
   U8 i;

	for(i=0; i<numOfPlmnEntry;i++)
	{
      for(PLMNLIST::iterator plmn_list_it = m_tlPLMNList.begin(); plmn_list_it != m_tlPLMNList.end(); plmn_list_it++)
      {
         //if((*it).strPLMN == *plmn_list_it) //strings are same
        if(!std::strcmp((const char *) cellInfo.stBcch.stPLMNList[i].szPLMN ,(*plmn_list_it).c_str())) 
			   return;
      }
	}

   TRACE_PRINTF("CONFIGURED PLMN LIST\n");
   for (PLMNLIST::iterator it_plmn = m_tlPLMNList.begin(); it_plmn != m_tlPLMNList.end(); it_plmn++)
      TRACE_PRINTF("PLMN : %s\n", (*it_plmn).c_str());


   TRACE_PRINTF("PLMN LIST RECIEVED\n");
	for(i=0; i<numOfPlmnEntry;i++)
      TRACE_PRINTF("PLMN : %s\n", cellInfo.stBcch.stPLMNList[i].szPLMN);

	cellInfo.bPLMNNotConfigured = true;
}

#ifdef REM_ENABLE_DEBUG
std::string g_SibDecodeError;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : & __stLteCellInfo
//        [out]: bool
//	This function returns a bool variable that indicates whether SIB1 / SIB2 decoding is successful
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool decoding_unsuccessful(__stLteCellInfo  & cellInfo)
{
#ifdef DEBUG_PRINT
	char tmp[124];
	sprintf(tmp, "RSSI %d EARFCN %d PCI %d: SIB1->", cellInfo.iRSSI, cellInfo.uiEARFCN, cellInfo.uiPhyCellID);
	g_SibDecodeError += tmp;

	g_SibDecodeError+= 	cellInfo.bSib1Decoded ? "SUCCESS, SIB2->" : "FAILED, SIB2->";
	g_SibDecodeError+= 	cellInfo.bSib2Decoded ? "SUCCESS, SIB4->" : "FAILED, SIB4->";
	g_SibDecodeError+= 	cellInfo.bSib4Decoded ? "SUCCESS\n" : "FAILED\n";
#endif

	return !(cellInfo.bSib1Decoded && cellInfo.bSib2Decoded);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : & __stLteCellInfo
//        [out]: bool
//	This function returns a bool variable that indicates whether PLMN is configured or not
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool plmn_notconfigured(__stLteCellInfo  & cellInfo)
{
#ifdef DEBUG_PRINT
	char tmp[124];
	sprintf(tmp, "RSSI %d EARFCN %d PCI %d: PLMN Not Configured", cellInfo.iRSSI, cellInfo.uiEARFCN, cellInfo.uiPhyCellID);
#endif
	return cellInfo.bPLMNNotConfigured;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: std::string
//	This function is used to send the scanned reult to OAM.Only scan status and error string is
//  sent to OAM remaining results are updated to MIB
//
///////////////////////////////////////////////////////////////////////////////////////////////////
std::string CLteEvtHandler::sendScanResultToOAM()
{
   g_SibDecodeError = "";
	remSendLteLastScanTime();

	if(m_tCellList.empty())
	{
	    g_SibDecodeError += "No Neighbour Cell found in Scan";
       TRACE_PRINTF("SCAN Error Details : %s\n", g_SibDecodeError.c_str());
	    return g_SibDecodeError;
	}

	m_tCellList.remove_if(decoding_unsuccessful);

	if(m_tCellList.empty())
	{
	    g_SibDecodeError += "Neighbour cell list is empty as neighbor sibs not decoded properly";
       TRACE_PRINTF("SCAN Error Details : %s\n", g_SibDecodeError.c_str());
	    return g_SibDecodeError;
	}

	if(!m_tlPLMNList.empty() )
	{
		for( m_tCellIt = m_tCellList.begin(); m_tCellIt != m_tCellList.end(); m_tCellIt++ )
			applyPLMNFilter((*m_tCellIt));
		m_tCellList.remove_if(plmn_notconfigured);
	}


	if(m_tCellList.empty())
	{
	    g_SibDecodeError += "Neighbour cell list empty after aplying PLMN Filter";
       TRACE_PRINTF("SCAN Error Details : %s\n", g_SibDecodeError.c_str());
       return g_SibDecodeError;
	}

	remSendLteScanResult(m_tCellList);

	return g_SibDecodeError;
}
#ifdef LTE_TDD
U8 CLteEvtHandler::getsubframeAssignment()
{
   return m_subframeAssignment;
}


U8 CLteEvtHandler::getspecialSubframePatterns()
{
   return m_specialSubframePatterns;
}
#endif
//Nfft changes
RemBandwidth CLteEvtHandler::getDlBw()
{
   return m_enDlBw;
}

#ifdef ENABLE_CNM
bool cnmIsCsgCell(U16 neighPci)
{
   CRemLteConfigData & oLteCfg = CRemLteConfigData::getInstance();
   U16                     maxPciVal;
   U16                     csgRange = oLteCfg.getCnmCsgPhyIdRange();
   U16                     physCellIdStart = oLteCfg.getCnmCsgStartPhyId();
   U16                     servPci = oLteCfg.getCnmServCellPci();
   bool                    retVal = false;
      
   if(csgRange)
   {
      maxPciVal = physCellIdStart + (csgRange - 1);
   }
   else
   {
      maxPciVal = physCellIdStart + csgRange ;
   }

   if(((servPci >= physCellIdStart) && (servPci <= maxPciVal)) &&
      ((neighPci >= physCellIdStart) && (neighPci <= maxPciVal)))
   {/* CLOSED_ACCESS_MODE */
      retVal = true;
   }
   else if(((servPci < physCellIdStart) || (servPci >= maxPciVal)) &&
           ((neighPci < physCellIdStart) || (neighPci >= maxPciVal)))
   {/* OPEN_ACCESS_MODE */
      retVal = true;
   }

   return retVal;
}
bool CLteEvtHandler::cnmSelectCellToSync(U16 *earfcn,U16 *pci)
{
	std::list<__stLteCellInfo>::iterator cellListIt;
   bool nonCsgCellFound = false;

   /* Scan thru the list
    * Select the strongest among the non csg cell
    * Else select the strongest among the femto cells
    * The list is sorted based on RSSI
    * So select the first non csg cell if present
    * Else select the first cell from the list
    */
	if(m_tCellList.empty())
	{
#ifdef DEBUG_PRINT
      printf("\n No Cells present to sync...\n");
#endif
	   return false;
	}

	for(cellListIt = m_tCellList.begin(); cellListIt != m_tCellList.end(); cellListIt++ )
   {
      /* Check the presence of non csg cell*/
      if(!cnmIsCsgCell((*cellListIt).uiPhyCellID))
      {
         nonCsgCellFound = true;
         break;
      }
   }

   if(nonCsgCellFound)
   {  /* Iterator points to the first NonCSG cell */
      *earfcn    = (*cellListIt).uiEARFCN;
      *pci       = (*cellListIt).uiPhyCellID;
      /* Remove the cell from the list */
      m_tCellList.erase(cellListIt);
      return true;
   }  
   else
   {
      /* As no nonCsg cells are present, take the first cell in the list
       * as the list is sorted based on RSSI */
      cellListIt = m_tCellList.begin();
      *earfcn    = (*cellListIt).uiEARFCN;
      *pci       = (*cellListIt).uiPhyCellID;
      /* Remove the cell from the list */
      m_tCellList.erase(cellListIt);
      return true;
   } 

   return false;
}
#endif
unsigned int CLteEvtHandler::handleSib1CfgCnf(void *pBuff)
{
   return NMM_SUCCESS;   
}
