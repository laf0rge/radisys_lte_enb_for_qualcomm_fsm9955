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

     Desc:     This file consists of LTE related functions for sending messages to QCOM

     File:     nmmcl_lte.cpp

     Prg:      Anjali D

**************************************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////////////////
//   Local includes
///////////////////////////////////////////////////////////////////////////////////////////////////
#include "nmmcl_interface.h"
#include "rem_datatype.h"
#include "rem_common.h"
#include "nmmcl_lte.h"
#include "rem_interface.h"
#include "LtePhyApi.h"
#ifdef __cplusplus
extern "C" {
#include "nl_rem_interface.h"
#include "rf/rfBandwidthMhz.h"
#include "rf/rfTechnology.h"
#include "rf/rfPathDirection.h"
#include "qctTypes.h"
#include "oamClientProtoWrapper.hpp"
   g_client g_remclnt;
bool g_firstQcomRssiRsp = FALSE;
bool g_qcomCellSearchRsp = FALSE;
bool g_qcomAllSibReadRsp = FALSE;
bool g_qcomSib1ReadRsp = FALSE;
}
#endif

#define MEAS_PERIOD 1
#define MEAS_BANDWIDTH 6 
#define NUM_SI_PERIOD_LIST 3
#define RETRY_COUNT 20

U8 getBandIdx(U8 bandClass);
U8 g_bCmplBandScan;
U8 g_usBandClass;
U16 g_usArfcnListStart;
U16 g_usArfcnListEnd;
U16 g_usArfcn;
U16 g_arfcnCount;
U16 g_usArfcnCounter;
U16 g_arfcnList[MAX_EARFCN_SCAN_LIST_SIZE];
U8 g_firstRssiRsp;
U8 g_bStartFlag;
U16 g_usPCI;
U16 g_usARFCN;
int g_sibRequest = 0;

typedef struct {
   U8             band;
   U16            usNDL_Low;
   U16            usNDL_Max;
} __stLteBandinfo;

__stLteBandinfo g_stLteBandLst[] =
{
   // BAND , NDL_Low, NDL_Max
   { 1, 0, 599 },
   { 2, 600, 1199 },
   { 3, 1200,1949 },
   { 4, 1950,2399 },
   { 5, 2400,2649 },
   { 6, 2650,2749 },
   { 7, 2750,3449 },
   { 8, 3450,3799 },
   { 9, 3800,4149 },
   { 10,4150,4749 },
   { 11,4750,4949 },
   { 12,5010,5179 },
   { 13,5180,5279 },
   { 14,5280,5379 },
   { 17,5730,5849 },
   { 18,5850,5999 },
   { 19,6000,6149 },
   { 20,6150,6449 },
   { 21,6450,6599 },
   { 22,6600,7399 },
   { 23,7500,7699 },
   { 24,7700,8039 },
   { 25,8040,8689 },
   { 26,8690,9039 },
   { 27,9040,9209 },
   { 28,9210,9659 },
   { 33,36000,36199},
   { 34,36200,36349},
   { 35,36350,36949},
   { 36,36950,37549},
   { 37,37550,37749},
   { 38,37750,38249},
   { 39,38250,38649},
   { 40,38650,39649},
   { 41,39650,41589},
   { 42,41590,43589},
   { 43,43590,45589},
   { 44,45590,46589},
   { 0, 0, 0 }
};

U8 getBandIdx(U8 bandClass)
{
   U8 cnt;
   for(cnt = 0;g_stLteBandLst[cnt].band !=0;cnt++)
   {
      if (g_stLteBandLst[cnt].band == bandClass)
      {
         return (cnt);
      }
   }

   TRACE_PRINTF("Could not find Band Index for given bandclass\n");
   return (cnt);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Class: CNmmLtePhyInterface
// Desc : Program entry point, storage for system wide data, etc.
///////////////////////////////////////////////////////////////////////////////////////////////////
CNmmLtePhyInterface::CNmmLtePhyInterface(bool bExpressScan)
{
   m_bExpressScan = bExpressScan;
   m_bMeasurementPeriod = MEAS_PERIOD;
   m_bMeasBandwidth = MEAS_BANDWIDTH;

   m_siWindow = SI_WINDOW;
   m_bRetryCount = RETRY_COUNT;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : initProvXmlFile
//	  @param[in]: void
//    This funtions is used to create records in provisioning.xml file
///////////////////////////////////////////////////////////////////////////////////////////////////

void CNmmLtePhyInterface :: initProvXmlFile(U8 dlBw, U16 bandSupported, U8 supportedTech)
{
   printf( "Creating provisioning.xml file \n" );
   U8 result = 1;   /* MGMT_RESULT_FAIL = 1 */
   RfConfig rfConfig;

   /* Set AdminState to false while creating record for the first time
    * as rfTxCarrierProv and rfRxCarrierProv requires adminState to be false */
   rfConfig.rfCarrierCommonAdminState = (QctBoolean_t)FALSE;
   rfConfig.rfCarrierCommonBandWidth = (RfBandwidthMhz_t)(dlBw + 1); 

   /* Below parameters are ReadOnly, dynamic configuration not supported  */
   /* Currently only one band is supported */
   rfConfig.BandList_0 = bandSupported;
   rfConfig.BandList_1 = 0;
   rfConfig.BandList_2 = 0;
   rfConfig.BandList_3 = 0;
   rfConfig.tech = (RfTechnology_t)supportedTech;
   rfConfig.rfCarrierCommonCarrierId = 1;
   rfConfig.rfCarrierCommonStandalone = (QctBoolean_t)TRUE;
   rfConfig.rfCarrierCommonRxPathIdLst_0 = 3;
   rfConfig.rfCarrierCommonRxPathIdLst_1 = 4;
   rfConfig.rfCarrierCommonRxPathIdLst_2 = 0;
   rfConfig.rfCarrierCommonRxPathIdLst_3 = 0;
   rfConfig.rfCarrierCommonTxPathIdLst_0 = 3;
   rfConfig.rfCarrierCommonTxPathIdLst_1 = 4;
   rfConfig.rfCarrierCommonTxPathIdLst_2 = 0;
   rfConfig.rfCarrierCommonTxPathIdLst_3 = 0;
   rfConfig.rfPathProvPathId1 = 3;
   rfConfig.rfPathProvPath1TxDirection = RF_PATH_DIRECTION_TX;
   rfConfig.rfPathProvPath1RxDirection = RF_PATH_DIRECTION_RX;
   rfConfig.rfPathProvPathId2 = 4;
   rfConfig.rfPathProvPath2TxDirection = RF_PATH_DIRECTION_TX;
   rfConfig.rfPathProvPath2RxDirection = RF_PATH_DIRECTION_RX;
   rfConfig.rfTxCarrierProvCarrierId = 1;
   rfConfig.rfRxCarrierProvCarrierId = 1;

   result = oamClientCreateRecords(rfConfig, (QctBoolean_t)TRUE);

   if ( !result ) 
   {
      printf(" Record creation failure in provisioning.xml, exiting REM binary  !!!! ");
      exit(0);
   }

}


///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendNmmNmmStart
//	  @param[in]: void
//    This funtions is used to send the NMM start request.           
///////////////////////////////////////////////////////////////////////////////////////////////////

void CNmmLtePhyInterface :: sendNmmStart()
{
   printf( "NL_REM::Initializing NL...\n" );
   nl_init_req_msg_v01        nl_init_req_msg;
   memset(&nl_init_req_msg,0,sizeof(nl_init_req_msg));
   nl_init_req_msg.config_valid = FALSE;
   nl_init_req_msg.config = 0;
   /* Send an NL_INIT_REQ. The client must send this initialization request
      prior to issuing any other messages. */
   rem_nl_qmi_clnt_send_req( g_remclnt.clnt_handle,
         NL_INIT_REQ_V01,
         "nl_init_req",
         &nl_init_req_msg,
         sizeof( nl_init_req_msg ),
         &g_remclnt.rsp,
         sizeof( g_remclnt.rsp ),
         rem_rsp_cb,
         NULL,
         &g_remclnt.txn_handle );
   printf("REM::sending NmmStart request");
}

nl_freq_scan_req_msg_v01 nl_freq_scan_lte_req_msg;

///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendNmmRssiMeasReq
//	  @param[in]: bandClass - uint8_t
//                    arfcnCount - uint8_t
//                    uiARFCNList - pointer to uiARFCNList
//    This funtions is used to send the RSSI measurement request.           
///////////////////////////////////////////////////////////////////////////////////////////////////
void CNmmLtePhyInterface::sendNmmRssiMeasReq(U8 bBandClass, U8 bArfcnCount, U16* usARFCNList)
{
   printf("REM::RSSI meas request sent\n");
   U8 bandIdx;
   U8 freqCount;

   TRACE_PRINTF("SENDING RSSI_MEAS_REQ for Band %d , ARFCNCOUNT %d\n",
         bBandClass,bArfcnCount);

   /* Reset all the global parameters as it is first Rssi Request */
   g_usArfcnCounter = 0;
   g_firstRssiRsp = true;
   g_firstQcomRssiRsp = TRUE;
   /*NL changes*/
   /* Frequency Scan request */
   /*! NL_FREQ_SCAN_REQ_V01 message (LTE) fill band information received from OAM */

   memset(&nl_freq_scan_lte_req_msg, 0, sizeof(nl_freq_scan_req_msg_v01));

   nl_freq_scan_lte_req_msg.rat            =   NL_RAT_LTE_V01;
   nl_freq_scan_lte_req_msg.meas_period    =   MEAS_PERIOD;
   nl_freq_scan_lte_req_msg.bw             =   NL_BW_1P4_MHZ_V01;
   nl_freq_scan_lte_req_msg.priority_valid =   TRUE;
   nl_freq_scan_lte_req_msg.priority       =   NL_PRIO_NORMAL_V01;
   nl_freq_scan_lte_req_msg.band           =   bBandClass;

   /* copy all the arfcn present in the list in a global structure */
   if(bArfcnCount == 0)
   {
      g_bCmplBandScan    = true;
      g_usBandClass      = bBandClass;
      bandIdx = getBandIdx(g_usBandClass);
      g_usArfcnListStart = g_stLteBandLst[bandIdx].usNDL_Low;
      g_usArfcnListEnd   = g_stLteBandLst[bandIdx].usNDL_Max;
      g_arfcnCount       = g_usArfcnListEnd - g_usArfcnListStart +1;
      nl_freq_scan_lte_req_msg.chan_cnt = 0;
      nl_freq_scan_lte_req_msg.chans_len = 0;

      for(freqCount = 0; freqCount < MAX_SNIFFER_RTWP_LIST_LEN 
            && freqCount < g_arfcnCount ; freqCount++)
      {
         g_usArfcnListStart++;
      }
      g_usArfcnCounter = g_usArfcnCounter + freqCount;
   }
   else
   {
      g_arfcnCount = bArfcnCount;
      nl_freq_scan_lte_req_msg.chan_cnt = g_arfcnCount;
      nl_freq_scan_lte_req_msg.chans_len = g_arfcnCount;
      g_bCmplBandScan = false;

      /*store the EARFCN in global list, maximum 200 EARFCN we can store now*/
      if(g_arfcnCount > MAX_EARFCN_SCAN_LIST_SIZE)
      {
         g_arfcnCount = MAX_EARFCN_SCAN_LIST_SIZE;
      }

      for(U32 i = 0; i< g_arfcnCount ; i ++)
      {
         g_arfcnList[i] = usARFCNList[i];
         nl_freq_scan_lte_req_msg.chans[i] = usARFCNList[i];
         TRACE_PRINTF("EARFCN in the list is %d\n", g_arfcnList[i]);
      }

      g_usArfcnCounter = g_usArfcnCounter + freqCount;
   }
   rem_nl_qmi_clnt_send_req( g_remclnt.clnt_handle,
         NL_FREQ_SCAN_REQ_V01,
         "nl_freq_scan_req",
         &nl_freq_scan_lte_req_msg,
         sizeof( nl_freq_scan_lte_req_msg ),
         &g_remclnt.rsp,
         sizeof( g_remclnt.rsp ),
         rem_rsp_cb,
         NULL,
         &g_remclnt.txn_handle );
   /* end of NL changes */ 
}

nl_cell_search_req_msg_v01 nl_cell_search_lte_req_msg;

void CNmmLtePhyInterface::createAndSendCellSearchReq(U16 usEARFCN, 
      U8 bPCICount, 
      U16 *usPCIList, 
      U8 searchType)
{

   TRACE_PRINTF("SENDING EARFCN %d in CELL_SEARCH_REQ\n", usEARFCN);
   memset(&nl_cell_search_lte_req_msg, 0, sizeof(nl_cell_search_req_msg_v01));
   nl_cell_search_lte_req_msg.carrier.rat              =   NL_RAT_LTE_V01;
   nl_cell_search_lte_req_msg.carrier.band             =   nl_freq_scan_lte_req_msg.band;
   nl_cell_search_lte_req_msg.carrier.chan             =   usEARFCN;
   nl_cell_search_lte_req_msg.pwr_params.bw            =   NL_BW_1P4_MHZ_V01;
   nl_cell_search_lte_req_msg.pwr_params.meas_period   =   MEAS_PERIOD; 
   nl_cell_search_lte_req_msg.priority_valid           =   TRUE;
   nl_cell_search_lte_req_msg.priority                 =   NL_PRIO_NORMAL_V01; 

   g_qcomCellSearchRsp = TRUE;
   /* Cell Search request */
   rem_nl_qmi_clnt_send_req( g_remclnt.clnt_handle,
         NL_CELL_SEARCH_REQ_V01,
         "nl_cell_search_req",
         &nl_cell_search_lte_req_msg,
         sizeof( nl_cell_search_lte_req_msg ),
         &g_remclnt.rsp,
         sizeof( g_remclnt.rsp ),
         rem_rsp_cb,
         NULL,
         &g_remclnt.txn_handle );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendNmmCellSearchReq
//	  @param[in]: uiEARFCN - uint16_t
//                    numPci - uint8_t
//                    pciListList - pointer to uint16_t
//                    band - uint16_t
//    This funtions is used to send cell search request.
///////////////////////////////////////////////////////////////////////////////////////////////////
void CNmmLtePhyInterface::sendNmmCellSearchReq(U16 usEARFCN, 
      U8 bPCICount, 
      U16 *usPCIList)
{
   return createAndSendCellSearchReq(usEARFCN, 
         bPCICount, 
         usPCIList, 
         SNIFFER_SEARCH_ALL_CELLS);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendCellPbchReq
//	  @param[in]: enRATType - __enSupportedRAT
//                    uiPCIorPSC - uint16_t
//                    uiARFCN - uint16_t
//    This funtions is used to send PBCH request.
///////////////////////////////////////////////////////////////////////////////////////////////////
nl_mib_read_req_msg_v01 nl_mib_read_lte_req_msg;
void CNmmLtePhyInterface :: sendNmmPbchCfgReq(U16 usPCIorPSC, U16 usARFCN)
{
   /* store PCI,Arfcn information in global data structure 
      to use when sending SIB1 Req */
   g_usPCI    = usPCIorPSC;
   g_usARFCN  = usARFCN;
   memset(&nl_mib_read_lte_req_msg, 0, sizeof(nl_mib_read_req_msg_v01));
   nl_mib_read_lte_req_msg.carrier.rat    =   NL_RAT_LTE_V01;
   nl_mib_read_lte_req_msg.carrier.band   =   nl_freq_scan_lte_req_msg.band;
   nl_mib_read_lte_req_msg.carrier.chan   =   usARFCN;
   nl_mib_read_lte_req_msg.cell_id        =   usPCIorPSC;
   nl_mib_read_lte_req_msg.priority_valid =   TRUE;
   nl_mib_read_lte_req_msg.priority       =   NL_PRIO_NORMAL_V01;
   TRACE_PRINTF("Sending MIB read request \n");
   rem_nl_qmi_clnt_send_req( g_remclnt.clnt_handle,
         NL_MIB_READ_REQ_V01,
         "nl_mib_read_req",
         &nl_mib_read_lte_req_msg,
         sizeof( nl_mib_read_lte_req_msg ),
         &g_remclnt.rsp,
         sizeof( g_remclnt.rsp ),
         rem_rsp_cb,
         NULL,
         &g_remclnt.txn_handle );
   TRACE_PRINTF("Sending PbchCfgRsp \n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendNmmSib1CfgReq
//	  @param[in]: retCnt- uint8_t
//    This funtions is used to send SIB1 request.
///////////////////////////////////////////////////////////////////////////////////////////////////
nl_sib_sched_req_msg_v01 nl_sib_sched_lte_req_msg;
void CNmmLtePhyInterface :: sendNmmSib1CfgReq(U16 usPCIorPSC, U16 usARFCN)
{
   memset(&nl_sib_sched_lte_req_msg, 0, sizeof(nl_sib_sched_lte_req_msg));
   nl_sib_sched_lte_req_msg.carrier.rat    =   NL_RAT_LTE_V01;
   nl_sib_sched_lte_req_msg.carrier.band   =   nl_freq_scan_lte_req_msg.band;
   nl_sib_sched_lte_req_msg.carrier.chan   =   usARFCN;
   nl_sib_sched_lte_req_msg.cell_id        =   usPCIorPSC;
   nl_sib_sched_lte_req_msg.retry_cnt      =   4;
   nl_sib_sched_lte_req_msg.priority_valid =   TRUE;
   nl_sib_sched_lte_req_msg.priority       =   NL_PRIO_NORMAL_V01;
   g_qcomSib1ReadRsp                       = TRUE;

   TRACE_PRINTF("Sending SIB1 request \n");
   rem_nl_qmi_clnt_send_req( g_remclnt.clnt_handle,
         NL_SIB_SCHED_REQ_V01,
         "nl_sib_sched_req",
         &nl_sib_sched_lte_req_msg,
         sizeof(nl_sib_sched_lte_req_msg),
         &g_remclnt.rsp,
         sizeof( g_remclnt.rsp ),
         rem_rsp_cb,
         NULL,
         &g_remclnt.txn_handle );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendNmmSib1CfgReq
//	  @param[in]: retCnt- uint8_t
//    This funtions is used to send SIB1 request.
///////////////////////////////////////////////////////////////////////////////////////////////////
void CNmmLtePhyInterface :: sendNmmSib1CfgReq()
{
   memset(&nl_sib_sched_lte_req_msg, 0, sizeof(nl_sib_sched_lte_req_msg));
   nl_sib_sched_lte_req_msg.carrier.rat    =   NL_RAT_LTE_V01;
   nl_sib_sched_lte_req_msg.carrier.band   =   nl_freq_scan_lte_req_msg.band;
   nl_sib_sched_lte_req_msg.carrier.chan   =   g_usARFCN;
   nl_sib_sched_lte_req_msg.cell_id        =   g_usPCI;
   nl_sib_sched_lte_req_msg.retry_cnt      =   4;
   nl_sib_sched_lte_req_msg.priority_valid =   TRUE;
   nl_sib_sched_lte_req_msg.priority       =   NL_PRIO_NORMAL_V01;
   g_qcomSib1ReadRsp                       = TRUE;
   TRACE_PRINTF("Sending SIB1 request taking values from global variables\n");
   rem_nl_qmi_clnt_send_req( g_remclnt.clnt_handle,
         NL_SIB_SCHED_REQ_V01,
         "nl_sib_sched_req",
         &nl_sib_sched_lte_req_msg,
         sizeof(nl_sib_sched_lte_req_msg),
         &g_remclnt.rsp,
         sizeof( g_remclnt.rsp ),
         rem_rsp_cb,
         NULL,
         &g_remclnt.txn_handle );

   TRACE_PRINTF("Sending SIB1_CONFIG_REQ [PCI=%d EARFCN=%d]\n", g_usPCI, g_usARFCN);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendNmmBcchCfgReq
//	  @param[in]: numSiLst - uint8_t
//                    siLst - pointer to uint16_t
//                    uiARFCN - uint16_t
//    This funtions is used to send PBCH request.
///////////////////////////////////////////////////////////////////////////////////////////////////
nl_sib_read_req_msg_v01 nl_sib_read_lte_req_msg;
void CNmmLtePhyInterface :: sendNmmBcchCfgReq(U8 bSiWindow, __stLteSibsPeriodicity *sibsInfo)
{

   g_qcomAllSibReadRsp = TRUE;
   memset(&nl_sib_read_lte_req_msg, 0,sizeof(nl_sib_read_lte_req_msg));
   nl_sib_read_lte_req_msg.carrier.rat         =   NL_RAT_LTE_V01;
   nl_sib_read_lte_req_msg.carrier.band        =   nl_freq_scan_lte_req_msg.band;
   nl_sib_read_lte_req_msg.carrier.chan        =   g_usARFCN;
   nl_sib_read_lte_req_msg.cell_id             =   g_usPCI;
   nl_sib_read_lte_req_msg.sib_attr_lte_valid  =   TRUE;
   nl_sib_read_lte_req_msg.priority_valid      =   TRUE;
   nl_sib_read_lte_req_msg.priority            =   NL_PRIO_NORMAL_V01;
   U32 count=0;
   TRACE_PRINTF("Sib_periodicity received from rem is");
   g_sibRequest = sibsInfo->numSibsToDecode;
   for(count = 1; count <= sibsInfo->numSibsToDecode; count ++)
   {
      TRACE_PRINTF("%d\t",sibsInfo->sibsPeriodicity[count-1]);
   }
   TRACE_PRINTF("\n");
   for(count = 1; count <= sibsInfo->numSibsToDecode; count ++)
   {
      nl_sib_read_lte_req_msg.sib_attr_lte.si_id = count;
      nl_sib_read_lte_req_msg.sib_attr_lte.si_periodicity 
         = sibsInfo->sibsPeriodicity[count-1]*10;
      nl_sib_read_lte_req_msg.sib_attr_lte.si_window 
         = bSiWindow ? bSiWindow : m_siWindow;
      nl_sib_read_lte_req_msg.sib_attr_lte.retry_cnt =   4;
      TRACE_PRINTF("Sending SIB_ID[%lu] request with si_periodicty is %d\n",count
                   ,nl_sib_read_lte_req_msg.sib_attr_lte.si_periodicity);
      TRACE_PRINTF("SiWindow value is %d\n",nl_sib_read_lte_req_msg.sib_attr_lte.si_window);
      rem_nl_qmi_clnt_send_req( g_remclnt.clnt_handle,
            NL_SIB_READ_REQ_V01,
            "nl_sib_read_req",
            &nl_sib_read_lte_req_msg,
            sizeof(nl_sib_read_lte_req_msg),
            &g_remclnt.rsp,
            sizeof( g_remclnt.rsp ),
            rem_rsp_cb,
            NULL,
            &g_remclnt.txn_handle );
   }
}

void CNmmLtePhyInterface::sendNmmFreqSyncLock(U16 usEARFCN, U16 usPCI)
{
   return createAndSendCellSearchReq(usEARFCN,
         1, /* No of PCI */
         &usPCI,
         SNIFFER_SEARCH_SPECIFIC_CELL_AFC_LOCK);
}


/*Need to write support for below messages*/
///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : clNmmStopRssiMeasReq
//   @param[in]: void
//     This funtions is used to send stop rssi NMM message.
///////////////////////////////////////////////////////////////////////////////////////////////////
void CNmmLtePhyInterface::sendNmmStopRssiMeasReq()
{
   //U8 status;
   TRACE_PRINTF("StopRssi Req Recieved\n");
   //status = NMM_SUCCESS;
   //Sending Stop Rsp to REM
   //decodeNmmStopRssiRsp(status);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendNmmStopCellSearchReq
//   @param[in]: void
//     This funtions is used to send cell search NMM message.
///////////////////////////////////////////////////////////////////////////////////////////////////
void CNmmLtePhyInterface::sendNmmStopCellSearchReq()
{
   //U8 status;
   TRACE_PRINTF("StopCellSearch Req Recieved\n");
   //status = NMM_SUCCESS;
   //decodeNmmStopCellSearchRsp(status);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendNmmStopBcchSib1Req
//   @param[in]: void
//     This funtion is used to send stop SIB READ message to rem_fsm
//      and forward received data to OAM.
///////////////////////////////////////////////////////////////////////////////////////////////////
void CNmmLtePhyInterface::sendNmmStopBcchReq()
{
   TRACE_PRINTF("REM_NL::StopBcch Req Received\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//    @Function : sendNmmStopPbchReq
//   @param[in]: void
//     This funtions is used to send PBCH rssi NMM message.
///////////////////////////////////////////////////////////////////////////////////////////////////
void CNmmLtePhyInterface::sendNmmStopPbchReq()
{
   TRACE_PRINTF("StopPbchReq Received\n");
}
