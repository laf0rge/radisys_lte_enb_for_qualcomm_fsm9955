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
 * remcl_lte.h
 *
 *  Created on: 20-Aug-2012
 *      Author: vkorde
 */

#ifndef REMCL_LTE_H_
#define REMCL_LTE_H_
/* defining global variable */
extern U8 g_bCmplBandScan;
extern U8 g_usBandClass;
extern U16 g_usArfcnListStart;
extern U16 g_usArfcnListEnd;
extern U16 g_usArfcn;
extern U16 g_arfcnCount;
extern U16 g_usArfcnCounter;
extern U16 g_arfcnList[MAX_EARFCN_SCAN_LIST_SIZE];
extern U8 g_firstRssiRsp;
extern U8 g_bStartFlag;
extern U16 g_usPCI;
extern U16 g_usARFCN;


class CNmmLtePhyInterface : public CNmmPhyInterface
{

   public:
      CNmmLtePhyInterface(bool bExpressScan=false);
      void sendNmmStart();
      void sendNmmRssiMeasReq(U8 bBandClass, U8 bArfcnCount, U16* usARFCNList);
      void sendNmmCellSearchReq(U16 usEARFCN, U8 bPciCount, U16 *usPCIList);
      void sendNmmPbchCfgReq(U16 usPCIorPSC, U16 usARFCN);
      void sendNmmSib1CfgReq();
      void sendNmmSib1CfgReq(U16 usPCIorPSC, U16 usARFCN);
      void initProvXmlFile(U8 dlBw, U16 bandSupported, U8 supportedTech);
#ifndef LTE_TDD
      void sendNmmBcchCfgReq(U8 bSiWindow, __stLteSibsPeriodicity *sibsInfo);
#else
      void  sendNmmBcchCfgReq(U8 bSiWindow,U8 subframeAssignment,
              U8 specialSubframePatterns, __stLteSibsPeriodicity *sibsInfo);
#endif
      void sendNmmStopBcchReq();
      void sendNmmStopRssiMeasReq();
      void sendNmmStopCellSearchReq();
      void sendNmmStopPbchReq();
	   void sendNmmFreqSyncLock(U16 usEARFCN, U16 usPCI);

   private:
      void createAndSendCellSearchReq(U16 usEARFCN, 
	  	                              U8 bPCICount, 
	  	                              U16 *usPCIList, 
	  	                              U8 searchType);
      U8 m_bMeasBandwidth;
      U8 m_siWindow;
      U8 m_bRetryCount;

};



#endif /* REMCL_LTE_H_ */
