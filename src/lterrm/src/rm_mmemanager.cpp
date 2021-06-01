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

/********************************************************************20**

     Name:     RRM Application

     Type:     CPP source file

     Desc:     This file contains 
               

     File:     rm_mmemanager.cpp

     Sid:      qf_mmemanager.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:33 2013

     Prg:     vkorde, hkrishna 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=161;
/* header include files */
#include "rm_mmemanager.h"


CMMEManger CMMEManger::m_objMME;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CMMEManger::CMMEManger()
{
   m_enMMEOverloadStatus = RMU_MME_OVLD_STATUS_NORMAL;
   m_enHighPriOvldStatus = RMU_MME_OVLD_STATUS_NORMAL;
   m_bActiveMmesCnt = 0;
   m_bOverloadedMmesCnt = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CMMEManger & CMMEManger::getInstance()
{
   return m_objMME;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// @param[in] pstMmeCfg - MME Information
//
///////////////////////////////////////////////////////////////////////////////////////////////////
LrmCause CMMEManger::addMME(LrmMmeCfg & stMmeCfg)
{
   if (m_tmMme.find(stMmeCfg.usMmeId) != m_tmMme.end())
   {
      RLOG1(L_WARNING,"MME [%d] : Already exist in RRM",stMmeCfg.usMmeId); 
      return LRM_CAUSE_MME_ALREADY_EXISTS;
   }

   RmMmeInfo stMmeCb;
   stMmeCb.usMmeId = stMmeCfg.usMmeId;
   stMmeCb.enMmeState = RM_MME_STATE_DOWN;
   stMmeCb.enMmeOvldStatus = RMU_MME_OVLD_STATUS_NORMAL;

   RLOG1 (L_ALWAYS, "New MME added: MME ID: %d", stMmeCb.usMmeId);
   /* insert the MME into the MME Map */
   m_tmMme[stMmeCfg.usMmeId] = stMmeCb;

   return LRM_CAUSE_NOT_APP;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
RmuMmeOvldStatus CMMEManger::upMME(U16 usMmeId)
{
   if (m_tmMme.find(usMmeId) == m_tmMme.end())
   {
      return m_enMMEOverloadStatus;
   }

   RmMmeInfo & stMmeCb = m_tmMme[usMmeId];

   if(RM_MME_STATE_UP == stMmeCb.enMmeState)
   {
      RLOG3 (L_ALWAYS, "MME ID: %d is already UP status:%ld MME count=%d",
            stMmeCb.usMmeId, stMmeCb.enMmeOvldStatus, m_bActiveMmesCnt);
      return m_enMMEOverloadStatus;
   }

   stMmeCb.enMmeState = RM_MME_STATE_UP;
   stMmeCb.enMmeOvldStatus = RMU_MME_OVLD_STATUS_NORMAL;

   m_bActiveMmesCnt++;

   RLOG2(L_ALWAYS, "MME UP: MME ID: %d Active MME count: %d",
         stMmeCb.usMmeId, m_bActiveMmesCnt);
   return updateMMEOverloadStatus();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
RmuMmeOvldStatus CMMEManger::downMME(U16 usMmeId)
{
   if (m_tmMme.find(usMmeId) == m_tmMme.end())
   {
      return m_enMMEOverloadStatus;
   }

   RmMmeInfo & stMmeCb = m_tmMme[usMmeId];

   if(RM_MME_STATE_DOWN == stMmeCb.enMmeState)
   {
      RLOG3 (L_ALWAYS, "MME ID: %d is already down status:%ld MME count=%d",
            stMmeCb.usMmeId, stMmeCb.enMmeOvldStatus, m_bActiveMmesCnt);
      return m_enMMEOverloadStatus;
   }

   if (stMmeCb.enMmeOvldStatus != RMU_MME_OVLD_STATUS_NORMAL)
   {
      m_bOverloadedMmesCnt--;
   }

   m_bActiveMmesCnt--;
   stMmeCb.enMmeState = RM_MME_STATE_DOWN;
   stMmeCb.enMmeOvldStatus = RMU_MME_OVLD_STATUS_NORMAL;


   RLOG2(L_ALWAYS, "MME DOWN: MME ID: %d Active MME count=%d",
         stMmeCb.usMmeId, m_bActiveMmesCnt);
   return updateMMEOverloadStatus();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
LrmCause CMMEManger::deleteMME(LrmMmeCfg & stMmeCfg)
{
   if (m_tmMme.find(stMmeCfg.usMmeId) == m_tmMme.end())
      return LRM_CAUSE_MMEID_UNKNOWN;

   RmMmeInfo & stMmeCb = m_tmMme[stMmeCfg.usMmeId];

   if (stMmeCb.enMmeOvldStatus != RMU_MME_OVLD_STATUS_NORMAL)
      m_bOverloadedMmesCnt--;

   if (stMmeCb.enMmeState == RM_MME_STATE_UP)
      m_bActiveMmesCnt--;

   m_tmMme.erase(stMmeCfg.usMmeId);

   RLOG2 (L_ALWAYS, "MME deleted: MME ID: %d Active MME count: %d",
         stMmeCfg.usMmeId, m_bActiveMmesCnt);
   return LRM_CAUSE_NOT_APP;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
RmuMmeOvldStatus CMMEManger::handleMMEOvldStart(RmuMmeOvldStartInd & stMmeOvldStrtInd)
{
   if (m_tmMme.find(stMmeOvldStrtInd.usMmeId) == m_tmMme.end())
      return m_enMMEOverloadStatus;

   RmMmeInfo & stMmeCb = m_tmMme[stMmeOvldStrtInd.usMmeId];

   // if received overload status has invalid value, return current status
   if ( stMmeOvldStrtInd.enMmeOvldStatus >=  RMU_MME_OVLD_STATUS_NORMAL)
      return m_enMMEOverloadStatus;

   // if previous and old status is same, return current status
   if (stMmeCb.enMmeOvldStatus == stMmeOvldStrtInd.enMmeOvldStatus )
      return m_enMMEOverloadStatus;

   // if previous MME OVERLOAD STATUS was NORMAL, then only increment count
   if( stMmeCb.enMmeOvldStatus == RMU_MME_OVLD_STATUS_NORMAL)
      m_bOverloadedMmesCnt++;

   stMmeCb.enMmeOvldStatus = stMmeOvldStrtInd.enMmeOvldStatus;

   RLOG3 (L_INFO, "Overload Start: MME ID: %d OverloadAction: %d "
         "Overload MME count: %d",
         stMmeCb.usMmeId, stMmeCb.enMmeOvldStatus, m_bOverloadedMmesCnt);

   return updateMMEOverloadStatus();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
RmuMmeOvldStatus CMMEManger::handleMmeOverloadStop(RmuMmeOvldStopInd & stMmeOvldStopInd)
{
   if (m_tmMme.find(stMmeOvldStopInd.usMmeId) == m_tmMme.end())
      return m_enMMEOverloadStatus;

   RmMmeInfo & stMmeCb = m_tmMme[stMmeOvldStopInd.usMmeId];

   // if previous status is normal, no need to worry
   if (stMmeCb.enMmeOvldStatus == RMU_MME_OVLD_STATUS_NORMAL)
      return m_enMMEOverloadStatus;

   m_bOverloadedMmesCnt--;

   RLOG2 (L_INFO, "Overload Stop: MME ID: %d Overload MME count: %d",
         stMmeCb.usMmeId, m_bOverloadedMmesCnt);
   // move MME to NORMAL status
   stMmeCb.enMmeOvldStatus = RMU_MME_OVLD_STATUS_NORMAL;

   return updateMMEOverloadStatus();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
RmuMmeOvldStatus CMMEManger::updateMMEOverloadStatus()
{
   m_enMMEOverloadStatus = RMU_MME_OVLD_STATUS_NORMAL;

   if ( m_bActiveMmesCnt && m_bOverloadedMmesCnt == m_bActiveMmesCnt)
   {
      std::map<MMEID, RmMmeInfo>::iterator it = m_tmMme.begin();
      m_enMMEOverloadStatus  = (it->second).enMmeOvldStatus;
      for (; it != m_tmMme.end(); it++)
      {
         /*if the MME overload status of all the MME's is not same then return 
           MME overload status as NORMAL */
         if ((it->second).enMmeOvldStatus != m_enMMEOverloadStatus)
         {   
            m_enMMEOverloadStatus = RMU_MME_OVLD_STATUS_NORMAL;
            break;
         }
      }
   }

   return m_enMMEOverloadStatus;
}
#ifdef RRM_UNUSED_FUNC
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CMMEManger::printMmes()
{
   std::map<MMEID, RmMmeInfo>::iterator it;
   for(it = m_tmMme.begin(); it != m_tmMme.end(); it++)
   {
      RLOG3 (L_INFO, "MME ID: %d, MME State: %d, MME staus: %d",
            it->second.usMmeId, it->second.enMmeState, 
            it->second.enMmeOvldStatus);
   }
}
#endif

/********************************************************************30**

           End of file:     qf_mmemanager.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:33 2013

*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
*********************************************************************91*/
