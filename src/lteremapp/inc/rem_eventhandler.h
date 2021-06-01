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

     Type:     C++ Header file

     Desc:     Consists of Eventhandler class and related member functions and variables
	  				The Eventhandler class consists of functions that are common to both LTE 
					and WCDMA like Hanlding PBCH DATA INDICATION message,BCCH DATA INDICATION 
					and related functions.

     File:     rem_eventhandler.h

     Sid:      rem_eventhandler.h@@/main/Tenb_T3k_Perf/1 - Wed Jan  9 20:53:10 2013

     Prg:      Vikram K

**************************************************************************************************/

#ifndef REM_EVENTHANDLER_H_
#define REM_EVENTHANDLER_H_

#include "rem_events.h"
#include "nmmcl_interface.h"
#include "nmmcl_ltemsgs.h"

#include <iostream>
#include <algorithm>
#include <map>
#include <string>


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   @class CRemEvtHandler
//   This class consists of Event handler functionalities that are common to both
//   LTE and WCDMA
//
///////////////////////////////////////////////////////////////////////////////////////////////////
class CRemEvtHandler
{
public: /* Public Interface Functions */
    virtual ~CRemEvtHandler();

    // Vikram, 14/09/12
    // Pure virtual functions, derived class must implement these functions
    virtual __enSupportedRAT getRATType() = 0;
    virtual bool cellsAvailableForScanning() = 0;
    virtual U16 getPciOrPsc() = 0;
    virtual U16 getARFCN2() = 0;
    virtual int handlePbchDataInd(void* pBuff) = 0;
    virtual bool handleBcchDataInd(void* pBuff) = 0;
	 virtual bool handleFreqSyncInd(void *pBuff){return false;}
    virtual bool nextCellAvailable() = 0;
    virtual bool cellsAvailableAfterFilter() = 0;
    virtual U16 getScanTimeout() = 0;
    virtual U16 getRssiTimeout() = 0;
    virtual std::string sendScanResultToOAM()=0;
	 virtual U8 getSiWindow() = 0;
	 virtual bool isSib1Decoded() = 0;
	 virtual bool startFreqSync(){return false;}
    virtual void prepareNmmSibsPeriodicity(__stLteSibsPeriodicity  *sibsInfo)=0;
    virtual unsigned int handleSib1CfgCnf(void *pBuff) = 0;
#ifdef ENABLE_CNM
    virtual bool cnmSelectCellToSync(U16 *earfcn,U16 *pci)=0;
#endif
#ifdef LTE_TDD
    virtual U8 getsubframeAssignment()=0;
    virtual U8 getspecialSubframePatterns()=0;
#endif
    /*Nfft DL BW Chenges */
    void saveDlBw(RemBandwidth enDlBw);
    RemBandwidth getDlBw();
    // Vikram, 14/09/12
    // Virtual functions, there is no compulsion to implement in derived class,specially in WCDMA
    virtual bool isCarrierExprScanEnabled();
    virtual bool isPciExpScanEnabled();

    // Vikram, 14/09/12
    // Non-Virtual functions, they are common to LTE/WCDAM
    void handleCellSearchInd(__stNmmCellSearchIndEvt* pCellSearchInd);
    void handleRssiInd(__stRssiMeasResult * pRssiInd);
    bool nextARFCNAvailable();
    bool nextBandAvailable();
    U8   getBand();
    U16  getARFCN();
    U8   getARFCNCount();
    U16* getARFCNList();
    bool sortARFCNWithStrongestRSSI();

    CNmmPhyInterface* m_pCl;

private: /* Private Data Members, meant for only this class and not for derived classes */

protected: /* Protected Data Members */
    /* Vikram, 14/09/12 */
    /* Only this or derived classes should use these variables */
    std::list<__stRssiMeasResult> m_tRssiMeasList;
    std::list<__stRssiMeasResult>::iterator m_tRssiIt;
    BANDLIST m_tBandList;
    BANDLIST::iterator m_tBandIt;
    PLMNLIST	m_tlPLMNList;
#ifdef LTE_TDD
    U8 m_subframeAssignment;
    U8 m_specialSubframePatterns;
#endif
    RemBandwidth m_enDlBw;
};

#endif /* REM_EVENTHANDLER_H_ */
