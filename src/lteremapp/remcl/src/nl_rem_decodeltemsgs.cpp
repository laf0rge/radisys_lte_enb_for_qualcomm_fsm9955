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

     Desc:     This file consists of LTE related functions for sending messages to PHY

     File:     nl_rem_decodeltemsgs.cpp

     Prg:      Anjali D

**************************************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////
//   Local includes
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "nl_rem_decodeltemsgs.h"

namespace qcomcl
{
   __stNmmRsp g_stQcomRsp;
   __stNmmCnf g_stQcomCnf;
   __stNmmCellSearchIndEvt g_stQcomCellSearchInd;
   __stRssiMeasResult g_stQcomRssiInd;
   __stLteNMM_BCCH_DATA_IND g_stQcomBcchDatInd;
}
using namespace qcomcl;

extern "C" {
#include "nl_rem_interface.h"
   extern g_client g_remclnt;
}


float  g_updatersrq[MAX_CELLS_SEARCH]={0.0};
static int g_numValidCells;
bool g_sibError = 0;
int g_sibResponse=0;
/*============================================================================
FUNCTION:  decodeQcomNlInitInd
============================================================================*/
/*! @brief
  Decode received nl_init indication.

  @details
  This function is called by the rem_ind_cb to forward
  indication to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomNlInitInd(U8 status)
{
   g_stQcomRsp.bErrorCode = status;
   remHandleEvent(REM_NMM_START_RSP_EVT, &g_stQcomRsp);
}

/*============================================================================
FUNCTION:  decodeQcomRssiRsp
============================================================================*/
/*! @brief
  Decode received Freq_scan response.

  @details
  This function is called by the rem_rsp_cb to send
  received status to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomRssiRsp(U8 status)
{
   g_stQcomRsp.bErrorCode = status;
   remHandleEvent(REM_RSSI_MEAS_RSP_EVT, &g_stQcomRsp);
}

/*============================================================================
FUNCTION:  decodeQcomFreqScanInd
============================================================================*/
/*! @brief
  Decode received Freq_scan indication.

  @details
  This function is called by the rem_ind_cb to forward
  indication to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomFreqScanInd(U8 rssi,U16 usArFCN)
{
   g_stQcomRssiInd.bRssi = (S16)rssi;
   g_stQcomRssiInd.usARFCN = usArFCN;
   g_stQcomRssiInd.bErrorCode = NMM_SUCCESS;
   remHandleEvent(REM_RSSI_MEAS_IND_EVT, &g_stQcomRssiInd);
}

/*============================================================================
FUNCTION:  decodeQcomRssiCnf
============================================================================*/
/*! @brief
  Send received Rssi confirmation status.

  @details
  This function is called by the rem_ind_cb to send
  confirmation status to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomRssiCnf(U8 status)
{
   g_stQcomRsp.bErrorCode = status;
   remHandleEvent(REM_RSSI_MEAS_CNF_EVT, &g_stQcomRsp);
}

/*============================================================================
FUNCTION:  decodeQcomCellSearchRsp
============================================================================*/
/*! @brief
  Decode received Cell_search response.

  @details
  This function is called by the rem_rsp_cb to send
  received status to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomCellSearchRsp(U8 status)
{
   g_stQcomRsp.bErrorCode = status;
   remHandleEvent(REM_CELL_SEARCH_RSP_EVT, &g_stQcomRsp);
}

/*============================================================================
FUNCTION:  checkForDuplicateCell
============================================================================*/
/*! @brief
  Check for any duplicate Cells present in cell_search indication.

  @details
  This function is called by decodeQcomCellSearchInd to check and
  avoid any duplicate cells.

  @return
  None.
 */
/*==========================================================================*/
bool checkForDuplicateCell(int instance)
{
   int duplicateCnt;
   bool isDuplicate = false;
   for(duplicateCnt=0;duplicateCnt < g_numValidCells;duplicateCnt++)
   {
      /* If cell already exists in the list */
      if(g_stQcomCellSearchInd.stCellElm[duplicateCnt].usPCI_PSC 
            == (&g_remclnt.ind.cell_search)->cells_lte[instance].pci)
      {
         /*If received RSRP value is greater than available in list,
           replace the existing list with received value*/
         if(((S16)(&g_remclnt.ind.cell_search)->cells_lte[duplicateCnt].rsrp + MIN_RSRP) 
               > (S16)g_stQcomCellSearchInd.stCellElm[instance].bRSRP_RSCP)
         {
            g_stQcomCellSearchInd.stCellElm[duplicateCnt].bRSRP_RSCP 
               = (S16)(&g_remclnt.ind.cell_search)->cells_lte[instance].rsrp + MIN_RSRP;
            g_updatersrq[instance] = 
               (((float)(&g_remclnt.ind.cell_search)->cells_lte[instance].rsrq)/2.0 - 19.5f);
         }
         TRACE_PRINTF("REPLACING CELL %d with PCI %d and RSRP %d\n",
			 duplicateCnt,
			 g_stQcomCellSearchInd.stCellElm[duplicateCnt].usPCI_PSC,
			 g_stQcomCellSearchInd.stCellElm[duplicateCnt].bRSRP_RSCP);
	 isDuplicate = true;
	 break;
      }
   }
   return(isDuplicate);
}

/*============================================================================
FUNCTION:  decodeQcomCellSearchInd
============================================================================*/
/*! @brief
  Decode received Cell_search indication.

  @details
  This function is called by the rem_ind_cb to forward
  indication to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomCellSearchInd()
{
	int i;
	g_numValidCells = 0;
	for(i=0;i<(&g_remclnt.ind.cell_search)->cell_cnt;i++)
	{
      if(checkForDuplicateCell(i))
      {
         TRACE_PRINTF("Found Duplicate Cell %d with PCI = %d\n",i
               , (&g_remclnt.ind.cell_search)->cells_lte[i].pci);
         continue;
      }
      if(((S16)(&g_remclnt.ind.cell_search)->cells_lte[i].rssi + MIN_RSRP) 
            > RSRP_THRESHOLD_TO_IGNORE)
      {
         g_numValidCells++;
         g_stQcomCellSearchInd.stCellElm[i].usPCI_PSC   = 
            (&g_remclnt.ind.cell_search)->cells_lte[i].pci;
         g_stQcomCellSearchInd.stCellElm[i].bRSRP_RSCP  
            = (S16)(&g_remclnt.ind.cell_search)->cells_lte[i].rsrp + MIN_RSRP;
         g_updatersrq[i] = (((float)(&g_remclnt.ind.cell_search)->cells_lte[i].rsrq)/2.0 - 19.5f);
         TRACE_PRINTF("CELL %d PCI %d RSRP %d RSRQ %3.1f\n",g_numValidCells,
               g_stQcomCellSearchInd.stCellElm[i].usPCI_PSC,
               g_stQcomCellSearchInd.stCellElm[i].bRSRP_RSCP,
               g_updatersrq[i]);
      }
      else
      {
         TRACE_PRINTF("CELL %d with PCI %d has RSRP %d \n", i,  
               (&g_remclnt.ind.cell_search)->cells_lte[i].pci,
               (&g_remclnt.ind.cell_search)->cells_lte[i].rsrp);
      }
   }
   g_stQcomCellSearchInd.bnumOfCellsFound = g_numValidCells;
   remHandleEvent(REM_CELL_SEARCH_IND_EVT, &g_stQcomCellSearchInd);
}

/*============================================================================
FUNCTION:  decodeQcomCellSearchCnf
============================================================================*/
/*! @brief
  Send received Cell_search confirmation status.

  @details
  This function is called by the rem_ind_cb to send
  confirmation status to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomCellSearchCnf(U8 status)
{
   g_stQcomRsp.bErrorCode = NMM_SUCCESS;
   remHandleEvent(REM_CELL_SEARCH_CNF_EVT, &g_stQcomRsp);
}

/*============================================================================
FUNCTION:  decodeQcomMibReadInd
============================================================================*/
/*! @brief
  Decode received Mib_read indication.

  @details
  This function is called by the rem_ind_cb to forward
  indication to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomMibReadInd()
{
   int i;
   //MibInfo.numTx,MibInfo.dlSystemBanWidth
   TRACE_PRINTF("Total No of valid Cells = %d\n",
		   g_numValidCells);
   for(i=0;i< g_stQcomCellSearchInd.bnumOfCellsFound;i++)
   {
      if(g_stQcomCellSearchInd.stCellElm[i].usPCI_PSC == 
            (&g_remclnt.ind.mib_read)->mib_lte.pci)
      {
         g_stQcomCellSearchInd.stCellElm[i].TxAntenna   
            =  (U8)(&g_remclnt.ind.mib_read)->mib_lte.mib.tx_ant_config;
         g_stQcomCellSearchInd.stCellElm[i].dlBandwidth 
            =  (U8)(&g_remclnt.ind.mib_read)->mib_lte.mib.dl_sys_bw;
      }
      else
      {
         g_stQcomCellSearchInd.stCellElm[i].TxAntenna   = 0;
         g_stQcomCellSearchInd.stCellElm[i].dlBandwidth = 0;
      }
      TRACE_PRINTF("CELL %d has TxA %d DlBw %d\n",
            g_stQcomCellSearchInd.stCellElm[i].usPCI_PSC,
            g_stQcomCellSearchInd.stCellElm[i].TxAntenna,
            g_stQcomCellSearchInd.stCellElm[i].dlBandwidth);
   }
   remHandleEvent(REM_STOP_PBCH_RSP_EVT, &g_stQcomRsp);
}

/*============================================================================
FUNCTION:  decodeQcomSib1CfgCnf
============================================================================*/
/*! @brief
  Send received Sib1 confirmation status.

  @details
  This function is called by the rem_ind_cb to send
  confirmation status to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomSib1CfgCnf(U8 status)
{
   g_stQcomRsp.bErrorCode = status;
   remHandleEvent(REM_SIB1_CONFIG_CNF_EVT, &g_stQcomRsp);
}

/*============================================================================
FUNCTION:  decodeQcomReadSib1Ind
============================================================================*/
/*! @brief
  Decode received sib1_read indication.

  @details
  This function is called by the rem_ind_cb to forward
  indication to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomReadSib1Ind()
{
   uint16_t i;
   g_stQcomBcchDatInd.bNumBcchDataList = 1;
   g_stQcomBcchDatInd.stBcchDataElmnt[0].usLen 
      = (&g_remclnt.ind.sib_sched)->sib_sched_lte.raw_sib.data_len;
   g_stQcomBcchDatInd.stBcchDataElmnt[0].usSfn 
      = (&g_remclnt.ind.sib_sched)->sib_sched_lte.sfn; 
   for(i=0; i<(&g_remclnt.ind.sib_sched)->sib_sched_lte.raw_sib.data_len;i++)
   {
      g_stQcomBcchDatInd.stBcchDataElmnt[0].bData[i] 
         = (&g_remclnt.ind.sib_sched)->sib_sched_lte.raw_sib.data[i];
   }
   g_stQcomBcchDatInd.bNrVendSpecList = 0;
   remHandleEvent(REM_BCCH_DATA_IND_EVT,&g_stQcomBcchDatInd);
}

/*============================================================================
FUNCTION:  decodeQcomSibReadCfgCnf
============================================================================*/
/*! @brief
  Send received Sib1 confirmation status.

  @details
  This function is called by the rem_ind_cb to send
  confirmation status to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomSibReadCfgCnf(U8 status)
{
   g_stQcomRsp.bErrorCode = status;
   remHandleEvent(REM_BCCH_CONFIG_CNF_EVT, &g_stQcomRsp);
}

/*============================================================================
FUNCTION:  decodeQcomReadSibInd
============================================================================*/
/*! @brief
  Decode received sib_read indication.

  @details
  This function is called by the rem_ind_cb to forward
  indication to rem_fsm.

  @return
  None.
 */
/*==========================================================================*/
void decodeQcomReadSibInd()
{
   uint16_t i;
   g_stQcomBcchDatInd.bNumBcchDataList = 1;
   g_stQcomBcchDatInd.stBcchDataElmnt[0].usLen 
      = (&g_remclnt.ind.sib_read)->sib_data_lte.raw_sib.data_len;
   g_stQcomBcchDatInd.stBcchDataElmnt[0].usSfn 
      = (&g_remclnt.ind.sib_read)->sib_data_lte.sfn;
   g_sibResponse++;
   if(g_stQcomBcchDatInd.stBcchDataElmnt[0].usLen == 0)
   {
      g_sibError++;
   }
   for(i=0; i<(&g_remclnt.ind.sib_read)->sib_data_lte.raw_sib.data_len;i++)
   {
      g_stQcomBcchDatInd.stBcchDataElmnt[0].bData[i] 
         = (&g_remclnt.ind.sib_read)->sib_data_lte.raw_sib.data[i];
   } 
   g_stQcomBcchDatInd.bNrVendSpecList = 0;
   /* Send BCCH_DATA_IND_EVT to REM with filled global structure */
   remHandleEvent(REM_BCCH_DATA_IND_EVT,&g_stQcomBcchDatInd);
}
