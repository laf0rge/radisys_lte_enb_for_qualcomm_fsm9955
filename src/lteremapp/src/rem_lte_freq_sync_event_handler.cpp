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


/******************************************************************************

     Name:     REM-APP

     Type:     C++ Source file

     Desc:     

     File:     rem_lte_freq_sync_event_handler.cpp

     Sid:      rem_lte_freq_sync_event_handler.cpp@@/main/tenb_main_ccb/1 - Thu Jan 21 17:51:27 2016

     Prg:      


******************************************************************************/
#include <string.h>
//#include "rem_datatype.h"
#include "rem_lte_freq_sync_event_handler.h"
//#include "rem_lteconfig.h"
//#include <cstring>
#include "rem_application.h"
#define REM_ENABLE_DEBUG

#define MAX_SIB_RETRYS_ALLOWED 8

void decodeLteBCCHMessage(__stLteNMM_BCCH_DATA_IND* pstBcchData, 
                                  __stLteCellInfo & stCellInfo);


void remSendLteLastScanTime();
void remSendLteScanResult(std::list<__stLteCellInfo> & tCellList);
///////////////////////////////////////////////////////////////////////////////
// @ param[in] : exScanTimeout - Scan TimeOut
//
// Constructor to CLteEvtHandler class
//
///////////////////////////////////////////////////////////////////////////////
CLteFreqSyncEvtHandler::CLteFreqSyncEvtHandler(U16 exScanTimeout):
                                                CLteEvtHandler(exScanTimeout)
{
}
///////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
//        [out]: bool - by default returns false. Returns true if bSib1Decoded or bSib2Decoded
//						are set
// This function copies Data received in BCCH DATA IND to member variables
//
///////////////////////////////////////////////////////////////////////////////
bool CLteFreqSyncEvtHandler::handleBcchDataInd(void* pBuff)
{
	__stLteNMM_BCCH_DATA_IND* pstBcchData = (__stLteNMM_BCCH_DATA_IND*)pBuff;

	__stLteCellInfo & stCell = *m_tCellIt;

   decodeLteBCCHMessage(pstBcchData, stCell);

   ++stCell.iSIBRetryCount;

	TRACE_PRINTF("LTE FREQ SYNC - BCCH DATA DECODED [SIBRetryCount:%d SIB1:%s SIB2:%s]\n",
                 stCell.iSIBRetryCount,
			        stCell.bSib1Decoded ? "DECODED" : "FAILED",
                 stCell.bSib2Decoded ? "DECODED" : "FAILED");  

   if(stCell.bSib1Decoded &&  stCell.bSib2Decoded)
   {
	   return true;
   }

   if( stCell.iSIBRetryCount == MAX_SIB_RETRYS_ALLOWED )
	   return true;

   return false;
}

bool CLteFreqSyncEvtHandler::isMacroCell(__stLteCellInfo & stCell)
{
   printf("stCell.stBcch.iRSTxPower = %d MACRO_RS_TX_POWER = %d \n", stCell.stBcch.iRSTxPower, MACRO_RS_TX_POWER);
   return (stCell.stBcch.iRSTxPower > MACRO_RS_TX_POWER);
}

bool CLteFreqSyncEvtHandler::startFreqSync()
{
   __stLteCellInfo & stCell = *m_tCellIt;
   // if the decoding of SIBs was not successful then we can't do sync with this
   if(!stCell.bSib1Decoded ||  !stCell.bSib2Decoded)
   {
      return false;
   }
   
   if(isMacroCell(stCell))
   {
      printf("It IS MACRO CELL SO DO FREQ SYNC= EARFCN %d PCI %d===\n", stCell.uiEARFCN, stCell.uiPhyCellID);
      // send AFC lock on this EARFCN and PCI
      m_pCl->sendNmmFreqSyncLock(stCell.uiEARFCN, stCell.uiPhyCellID);  
      /*Update OAM about this FREQ and PCI*/
      CRemApplication::GetInstance().setLteFreqSyncInfo(stCell.uiPhyCellID, stCell.uiEARFCN); 
      return true;
   }
   return false;
}

bool CLteFreqSyncEvtHandler::handleFreqSyncInd(void* pBuff)
{
   __stNmmCellSearchIndEvt* pCellSearchInd = (__stNmmCellSearchIndEvt*)pBuff;

   if(pCellSearchInd->bnumOfCellsFound)
   {
      m_synchedPCI = pCellSearchInd->stCellElm[0].usPCI_PSC;
      m_synchedEARFCN = pCellSearchInd->bearfcn;

      return true;
   }

   return false;
}
#ifdef REM_ENABLE_DEBUG
static std::string g_SibDecodeError;
#endif
///////////////////////////////////////////////////////////////////////////////
// @ param[in] : void
//        [out]: std::string
//	This function is used to send the scanned reult to OAM.Only scan status and error string is
//  sent to OAM remaining results are updated to MIB
//
///////////////////////////////////////////////////////////////////////////////
std::string CLteFreqSyncEvtHandler::sendScanResultToOAM()
{

	g_SibDecodeError = "INFO:";
#if 0
	remSendLteLastScanTime();

	if(m_tCellList.empty())
	{
	    g_SibDecodeError += "Neighbour cell list is empty";
	    return g_SibDecodeError;
	}

	m_tCellList.remove_if(decoding_unsuccessful);

	if(m_tCellList.empty())
	{
	    g_SibDecodeError += "Neighbour cell list is empty";
	    return g_SibDecodeError;
	}

	if(!m_tlPLMNList.empty() )
	{
		for( m_tCellIt = m_tCellList.begin(); m_tCellIt != m_tCellList.end(); m_tCellIt++ )
			applyPLMNFilter((*m_tCellIt));
		m_tCellList.remove_if(plmn_notconfigured);
	}

	TRACE_PRINTF("%s\n", g_SibDecodeError.c_str());

	if(m_tCellList.empty())
	{
	    g_SibDecodeError += "CELLs are filtered based on input PLMN list. No CELL's match";
        return g_SibDecodeError;
	}

	remSendLteScanResult(m_tCellList);
#endif
	return g_SibDecodeError;
}
#if 0
unsigned int CLteEvtHandler::handleSib1CfgCnf(void *pBuff)
{
   return NMM_SUCCESS;   
}
#endif
