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

     Type:     C++ Header file

     Desc:     Consists of WCDMA event handler class and member functions

     File:     rem_wcdma_event_handler.h

     Sid:      rem_wcdma_event_handler.h@@/main/Tenb_T3k_Perf/1 - Wed Jan  9 20:53:13 2013

     Prg:      Ambika S


**************************************************************************************************/

#ifndef REM_WCDMA_EVENT_HANDLER_H_
#define REM_WCDMA_EVENT_HANDLER_H_

#include "rem_eventhandler.h"
#include "rem_wcdmaconfig.h"
#include "rem_interface.h"
#ifndef LTE_QCOM
#include "nmmcl_wcdma.h"
#endif
#include "Pasn_BchDecodeParams.h"

#include <string.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
//
//   @class CWcdmaEvtHandler
//
///////////////////////////////////////////////////////////////////////////////
class CWcdmaEvtHandler :public CRemEvtHandler
{

public:
  CWcdmaEvtHandler(U16 scanTimeout=0);
  __enSupportedRAT  getRATType() { return RAT_WCDMA; }

  unsigned int   handleSib1CfgCnf(void *pBuff);
  int   handlePbchDataInd(void* pBuff);
  bool  handleBcchDataInd(void* pBuff);
  void  saveARFCNList(U16 usARFCN);
  bool  cellsAvailableForScanning();
  bool  nextCellAvailable();
  bool  cellsAvailableAfterFilter();
  void  saveWcdmaCellInfo();
  char* getBcchPlmnType();
  char* wcdmaBcchCsgSplitNumOfPscs();
  S32   calBcchRefPosLatitude();
  S32   calBcchRefPosLongitude();
  std::string sendScanResultToOAM();

  bool isSib1Decoded() {return true;}
  void prepareNmmSibsPeriodicity(__stLteSibsPeriodicity  *sibsInfo) {};
  U16  getScanTimeout();
  U16  getRssiTimeout();
  void generateRFList();
  U16  getPciOrPsc();
  U16  getARFCN2();
  U8  getSiWindow();
#ifdef ENABLE_CNM
  bool cnmSelectCellToSync(U16 *earfcn,U16 *pci);
#endif
#ifdef LTE_TDD
    U8 getsubframeAssignment();
    U8 getspecialSubframePatterns();
#endif

private:

  std::list<__stWcdmaCellInfo> m_tRFList;
  std::list<__stWcdmaCellInfo> :: iterator m_tRFIt;
  CRemWcdmaConfigData & m_objWcdmaConfigData;
#ifndef LTE_QCOM
  CNmmWcdmaPhyInterface m_objWcdmaPhyInterface;
#endif
  threeway::Pasn_BchDecodeParams m_bch;

};

#endif /* REM_WCDMA_EVENT_HANDLER_H_ */
