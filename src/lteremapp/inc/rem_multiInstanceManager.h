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

     Name:     REM-APP Multi Instance Manager

     Type:     C++ Header file

     Desc:     Consists of CRemMultiInstanceManager class

     File:     rem_multiIntanceManager.h

     Sid:      rem_multiInstanceManager.h@@/main/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:51:44 2016

     Prg:      Rohit Kumar 

**************************************************************************************************/

#ifdef USE_RADISYS_OAM

#ifndef __rem_multiInstanceManager_h_
#define __rem_multiInstanceManager_h_

///////////////////////////////////////////////////////////////////////////////
//	  System Includes
///////////////////////////////////////////////////////////////////////////////

#include <messaging/transport/ApplicationWithMessaging.h>
#include <messaging/transport/SingleThreadedAppWithAttrCache.h>
#include <messaging/transport/MessagingEntity.h>
#include <messaging/transport/MessageSerialisedData.h>
#include <messaging/transport/MibAccessInterface.h>
#include <mib-common/messages/MibSubscribeAttributesCnf.h>
#include <mib-common/messages/MibSubscribeAttributesInd.h>

using namespace boost;
using namespace threeway;

///////////////////////////////////////////////////////////////////////////////
//	  Local Includes
///////////////////////////////////////////////////////////////////////////////
#include "rem_msgs.h"

///////////////////////////////////////////////////////////////////////////////
//
//   @class CRemMultiInstanceManager
//
///////////////////////////////////////////////////////////////////////////////

class CRemMultiInstanceManager
{
   public:
      static CRemMultiInstanceManager& GetInstance();
      CRemMultiInstanceManager();
      MibDN GetUmtsCellDn(u32 uarfcn, u32 psc);
      MibDN GetLteCellDn(u32 earfcn, u32 pci);
      void sendMessageToOam(CRemScanStatus& objMsg);
      void AddUmtsCell(u32 uarfcn, u32 psc);
      void AddLteCell(u32 earfcn, u32 pci);
      //void AddCarrierMeas(u32 earfcn);
      void EraseRemObjects();
   private:
      map <pair<u32, u32>, u32> umtsCellToInstance;
      map <pair<u32, u32>, u32> lteCellToInstance;
      u32 lastUmtsCellInstance;
      void DeleteUmtsCellInstance(u32 instance);
      void DeleteLteCellInstance(u32 instance);
      static CRemMultiInstanceManager* s_instance;
      u32 m_lastLteCellInstance;
};

#endif
#endif
