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

#ifdef ENABLE_CNM
#ifdef USE_RADISYS_OAM
#include <system/Trace.h>
#endif
#include <map>
#include <string>

#include "rem_datatype.h"
#include "rem_fsm.h"
#include "rem_eventhandler.h"
#include "nmmcl_interface.h"
#include "rem_interface.h"
#include "rem_lte_event_handler.h"

using namespace remfsm;

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
// @ param[out] int : Returns the new state of State machine
//
// This function takes care of triggering  NMM Stop procedure
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleCnmStopEvt(void *pBuff)
{
   printf("\n Stopping CNM and NMM....,\n");
	(*fpStopTimer)(REM_SCAN_TIMEOUT_EVT);
   g_pRemEvtHdlr->m_pCl->sendNmmStop(); 

	delete g_pRemEvtHdlr;
	g_pRemEvtHdlr = NULL;

   // Scan was aborted, hence delete remaining list
   if( g_bScanStatus == REM_INDETERMINATE )
   {
	   while( !gRATList.empty() )
      {
		   delete gRATList.front();
	  	   gRATList.pop_front();
      }
	}

   return REM_STATE_INIT;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
// @ param[out] int : Returns the new state of State machine
//
// This function takes care of starting the CNM procedure
// Select a strongest cell
// Trigger cell search req for the selected cell
// Stop NMM if no cells are present
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int trigCnmStart()
{
   printf("\n @@@@@ Starting CNM@@@@@\n");
   if(g_pRemEvtHdlr->cnmSelectCellToSync(&g_cnmSlctdEarfcn,&g_cnmSlctdPci))
   {
      /* Select the cell */
      g_pRemEvtHdlr->m_pCl->sendNmmCellSearchReq(g_cnmSlctdEarfcn,1,&g_cnmSlctdPci);

  	   printf ("Sending NMM_CELL_SEARCH_REQ in CNM state with ARFCN %d PCI %d:\n",g_cnmSlctdEarfcn,g_cnmSlctdPci);
      /* Start cell search for the selected */
#ifdef DEBUG_PRINT
  	   printf ("Sending NMM_CELL_SEARCH_REQ in CNM state with ARFCN %d PCI %d:\n",g_cnmSlctdEarfcn,g_cnmSlctdPci);
#endif
   }
   else
   {
      /* No cell to search ::Stop CNM operations */
      /* Trig NMM Stop */
      printf("\n NO Cells available for CNM to sync\n");
      return handleCnmStopEvt(NULL);

   }
   return REM_STATE_CNM;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
// @ param[out] int : Returns the new state of State machine
//
// This function handles teh Cell Search Int event in CNM State
// Stop NMM if cell search status is failure
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleCnmCellSearchIndEvt(void* pBuff)
{
    printf("\n Recieved CellSearch Ind in CNM State");
#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"NMM_CELL_SEARCH_REQ_RESP Received");
#endif

	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
   {
      /* Try for next cell if available */
      return trigCnmStart();
   }
   
   return REM_STATE_CNM;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
// @ param[out] int : Returns the new state of State machine
//
// This function handles teh Cell Search CNF event in CNM State
// Initiate PBCH req in case of success
// Stop NMM if cell search status is failure
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleCnmCellSearchCnfEvt(void* pBuff)
{
   printf("\n Recieved CellSearch Cfm in CNM State");
#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"NMM_CELL_SEARCH_REQ_RESP Received");
#endif

	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
   {
      /* Try for next cell if available */
      return trigCnmStart();
   }

   /* Start the PBCH req for the selected cell for CNM */
#ifdef	ENABLE_CNM
   g_pRemEvtHdlr->m_pCl->sendNmmPbchCfgReq(g_cnmSlctdPci,g_cnmSlctdEarfcn,true);
#else
   g_pRemEvtHdlr->m_pCl->sendNmmPbchCfgReq(g_cnmSlctdPci,g_cnmSlctdEarfcn);
#endif
   return REM_STATE_CNM;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
// @ param[out] int : Returns the new state of State machine
//
// This function handles teh Cell Search rsp event in CNM State
// Stop NMM if cell search status is failure
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleCnmCellSearchRspEvt(void* pBuff)
{
   printf("\n Cell Search Rsp rcvd\n");
#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"NMM_CELL_SEARCH_REQ_RESP Received");
#endif

	if( ((__stNmmRsp*)pBuff)->bErrorCode != NMM_SUCCESS )
   {
      /* Try for next cell if available */
      return trigCnmStart();
   }

   return REM_STATE_CNM;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void*
// @ param[out] int : Returns the new state of State machine
// 
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleCnmPbchConfigCnfEvt(void* pBuff)
{
   printf("\n PBCH CNFrcvd,\n");
#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO,"NMM_CELL_SEARCH_REQ_RESP Received");
#endif

   return REM_STATE_CNM;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void
// @ param[out] int : Returns the new state of State machine
//  This function is considered as the end of the initial sync operation
//  Stop NMM 
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleCnmPbchDataIndEvt(void* pBuff)
{
	int iErr = g_pRemEvtHdlr->handlePbchDataInd(pBuff);

   printf("\n [CNM]:: Received PBCH_DAT_IND in CNM state:: Stopping CNM..\n");
	if( iErr == MIB_FAILED ) // MIB DECODING FAILED
	{
#ifdef DEBUG_PRINT
		printf("MIB/PBCH Decode failed for PCI %d\n", g_pRemEvtHdlr->getPciOrPsc());
#endif		
      /* Try for next cell if available */
      return trigCnmStart();
	}
   return handleCnmStopEvt(NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void*
// @ param[out] int : Returns the new state of State machine
// This funnction handles the PBCH RSP event in CNM state 
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleCnmStopPBCHRspEvt(void* pBuff)
{
#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO," PBCH_STOP_RSP in CNM state Received");
#endif

   return REM_STATE_CNM;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] void*
// @ param[out] int : Returns the new state of State machine
// This funnction handles the Scan abort event in CNM state
//
///////////////////////////////////////////////////////////////////////////////////////////////////
int handleCnmAbortScan(void* pBuff)
{
#ifdef DEBUG_PRINT
	remLog(REM_TRACE_INFO," PBCH_STOP_RSP in CNM state Received");
#endif

   return REM_STATE_CNM;
}
#endif
