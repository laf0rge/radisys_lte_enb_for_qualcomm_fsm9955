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

Name:     REM-MultiInstance Manager

Type:     C++ Source file

Desc:     This file consists of Radisys OAM-REM muliInstance related functions

File:     rem_multiInstanceManager.cpp

Sid:      rem_multiInstanceManager.cpp@@/main/tenb_5.0_RIB_GA/8 - Mon Feb 15 12:52:05 2016

Prg:      Rohit Kumar 

 **************************************************************************************************/

#ifdef USE_RADISYS_OAM

//////////////////////////////////////////////////////////////////////
//   Local includes
//////////////////////////////////////////////////////////////////////

#include "MibCreateObjectReq.h"
#include "MibDeleteObjectReq.h"
#include "MessagingEntity.h"
#include "rem_multiInstanceManager.h"
#include "rem_application.h"

using namespace threeway;
using namespace boost;

CRemMultiInstanceManager* CRemMultiInstanceManager::s_instance = 0;

///////////////////////////////////////////////////////////////////////
// @ class : CRemMultiInstanceManager
//
// Class CMultiInstanceManager implements the access to 
// MIB cache at REM for the 3G REM object
///////////////////////////////////////////////////////////////////////

CRemMultiInstanceManager::CRemMultiInstanceManager():
   m_lastLteCellInstance(0)
{
}

CRemMultiInstanceManager & CRemMultiInstanceManager::GetInstance()
{
   if (!s_instance)
   {
      s_instance = new CRemMultiInstanceManager();
   }
   return *s_instance;
}

void CRemMultiInstanceManager::AddUmtsCell(u32 uarfcn, u32 psc)
{
   umtsCellToInstance[pair<u32, u32>(uarfcn, psc)] = lastUmtsCellInstance;

   MibDN parentDn = MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP);
   MibRDN childRdn(MIB_OBJECT_CLASS_REM_SCANNED_WCDMA_CELL, lastUmtsCellInstance);

   MibAttributeValues attr;
   MibCreateObjectReq mibCreateObjectReq(parentDn, childRdn, attr);
   CRemApplication::GetInstance().SendMessage(mibCreateObjectReq, ENTITY_MIB, ENTITY_REM);
   lastUmtsCellInstance++;
}

void CRemMultiInstanceManager::AddLteCell(u32 earfcn, u32 pci)
{
   std::pair<u32, u32> earfcn_pci_pair(earfcn, pci);
   lteCellToInstance[pair<u32,u32>(earfcn, pci)] = m_lastLteCellInstance;

   MibDN dnobj = MibDN::GenerateMibDN(MIB_OBJECT_CLASS_LTE_REM_SCANNED_CELL,m_lastLteCellInstance);

      MibDN parentDn = MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP);
      MibRDN childRdn(MIB_OBJECT_CLASS_LTE_REM_SCANNED_CELL, m_lastLteCellInstance);
      MibAttributeValues attr;
      MibCreateObjectReq mibCreateObjectReq(parentDn, childRdn, attr);
      CRemApplication::GetInstance().SendMessage(mibCreateObjectReq, ENTITY_MIB, ENTITY_REM);
      m_lastLteCellInstance ++;

      MibAttributeValues attrNumCells;
      attrNumCells.AddAttribute(PARAM_ID_LTE_NUM_REM_ENTRIES,m_lastLteCellInstance);
      CRemApplication::GetInstance().GetMibCache().SetMibAttributesByDn
         (MibDN::GenerateMibDN(MIB_OBJECT_CLASS_FAP_LTE),attrNumCells, ENTITY_OAM_REM);

}

MibDN CRemMultiInstanceManager::GetUmtsCellDn(u32 uarfcn, u32 psc)
{
   std::pair<u32, u32> uarfcn_psc_pair(uarfcn, psc);
   u32 instance = umtsCellToInstance[uarfcn_psc_pair];
   MibDN cellDN = MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_SCANNED_WCDMA_CELL, instance);

   return cellDN;
}

MibDN CRemMultiInstanceManager::GetLteCellDn(u32 earfcn, u32 pci)
{
   std::pair<u32, u32> earfcn_pci_pair(earfcn, pci);
   u32 instance = lteCellToInstance[earfcn_pci_pair];
   MibDN cellDN = MibDN::GenerateMibDN(MIB_OBJECT_CLASS_LTE_REM_SCANNED_CELL, instance);
   return cellDN;
}

void CRemMultiInstanceManager::DeleteUmtsCellInstance(u32 instance)
{
   MibDN cellDn = MibDN::GenerateMibDN(MIB_OBJECT_CLASS_REM_SCANNED_WCDMA_CELL, instance);
   MibDeleteObjectReq mibDeleteObjectReq(cellDn);
   CRemApplication::GetInstance().SendMessage(mibDeleteObjectReq, ENTITY_MIB, ENTITY_REM);
}

void CRemMultiInstanceManager::DeleteLteCellInstance(u32 instance)
{
   MibDN cellDn = MibDN::GenerateMibDN(MIB_OBJECT_CLASS_LTE_REM_SCANNED_CELL, instance);
   MibDeleteObjectReq mibDeleteObjectReq(cellDn);
   CRemApplication::GetInstance().SendMessage(mibDeleteObjectReq, ENTITY_MIB, ENTITY_REM);
}

void CRemMultiInstanceManager::EraseRemObjects()
{
   // This block is for releasing UMTS cell objects created
   {
      map<pair<u32, u32>, u32>::iterator i = umtsCellToInstance.begin();
      while (i != umtsCellToInstance.end())
      {
         u32 instance = i->second;
         DeleteUmtsCellInstance(instance);
         i++;
      }
      /* Re-initialise the instance number to zero */
      lastUmtsCellInstance = 0;
   }

   // This block is for releasing LTE cell objects created
   {
      map<pair<u32, u32>, u32>::iterator i = lteCellToInstance.begin();
      while (i != lteCellToInstance.end())
      {
         u32 instance = i->second;
         DeleteLteCellInstance(instance);
         i++;
      }
      /* Re-initialise the instance number to zero */
      m_lastLteCellInstance = 0;
   }
}

#endif
