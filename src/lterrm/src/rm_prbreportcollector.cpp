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

     File:     rm_prbreportcollector.cpp

     Sid:      qf_prbreportcollector.cpp@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:02 2016

     Prg:     sprasad, anandyal, hkrishna

*********************************************************************21*/
/** @file rm_prbreportcollector.cpp
 * @brief This file is the PRB reprot collector class holds the SCH reproted PRB
 *         usasge for all GBR beares. which is useful during PRB based 
 *         RB admission.
 */
static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=155;

#include <math.h>
#include "rm_prbreportcollector.h"

void CPRBReportCollector::configurePRBReportCollector(
                                 U32 uiSchReportPeriodicity,
                                 U32 uiWindowSize)
{
   m_schReportingInterval   = uiSchReportPeriodicity;
   m_windowSize             = uiWindowSize;
   m_curntSlotIndx          = 0;
   m_areAllSlotsFilled      = false;

#ifdef DBG_MAC_RRM_PRB_PRINT
   if(uiSchReportPeriodicity)
   {
      m_DemoPrintCntFor20Sec   = 20000/uiSchReportPeriodicity;
   }
   else
   {
      m_DemoPrintCntFor20Sec   = 20000;
   }
   RLOG1 (L_INFO, "DemoPrintCount=%d", m_DemoPrintCntFor20Sec);
#endif
   m_currentGbrDlPrbUsage   = 0;
   m_currentGbrUlPrbUsage   = 0;

   m_tvPrbUsge.resize(uiWindowSize);
   RLOG2 (L_INFO, "Reporting Interval=%d WindowSize=%d",
         m_schReportingInterval, m_windowSize);
   return;
}

void CPRBReportCollector::clearPrbUsageData()
{
   m_curntSlotIndx        = 0;
   m_areAllSlotsFilled    = false;
   m_currentGbrDlPrbUsage = 0;
   m_currentGbrUlPrbUsage = 0;
   m_windowSize           = 0;
   m_tvPrbUsge.resize(0);
}

CPRBUsageInfo* CPRBReportCollector::getCurrentSlot()
{
   CPRBUsageInfo*  usageInfo;

   usageInfo = &m_tvPrbUsge[m_curntSlotIndx++];
   if(!(m_curntSlotIndx % m_windowSize))
   {
      m_areAllSlotsFilled = true;
      m_curntSlotIndx = 0;
   }

   return usageInfo;
}

U32 CPRBReportCollector::getPrbForLastSlots(U32 uiNumSlots,
                                            rmBearerDirection dir) const
{
   U32 uiPrb = 0;
   U16 uiCurSlotNum = m_curntSlotIndx;

   if(uiNumSlots == 0)
   {
      // numSlots calculated can be 0 only if the requested timePeriod is less
      // than the report interval. in that case, returning the current slot.
      uiNumSlots = 1;
   }

   const CPRBUsageInfo* usageInfo;
   for(U32 index = 0; index < uiNumSlots; index++)
   {
      usageInfo = &m_tvPrbUsge[uiCurSlotNum];
      if(dir == RM_DIR_DL)
      {
         uiPrb += usageInfo->m_gbrDlPrbRptsSum;
      }
      else
      {
         uiPrb += usageInfo->m_gbrUlPrbRptsSum;
      }
      if(uiCurSlotNum == 0)
      {
         uiCurSlotNum = (m_windowSize-1);
      }
      else
      {
         uiCurSlotNum--;
      }
   }
   return (uiPrb/uiNumSlots);
}

U32 CPRBReportCollector::getCurrentGbrUlPrbUsage(U32 uiTimePeriod) const
{
   if(uiTimePeriod > 0 && uiTimePeriod < 100)
   {
      U32 uiTotWindTime = (m_windowSize * m_schReportingInterval);
      U32 uiTimeReqd    = (uiTimePeriod * uiTotWindTime)/100;
      U32 uiNumSlots = (uiTimeReqd/m_schReportingInterval) + 1;
      return getPrbForLastSlots(uiNumSlots, RM_DIR_DL);
   }
   if(m_areAllSlotsFilled)
   {
      return((U32) RM_ROUND(m_currentGbrUlPrbUsage, m_windowSize));
   }
   else if(m_curntSlotIndx)
   {
      return((U32) RM_ROUND(m_currentGbrUlPrbUsage, m_curntSlotIndx));
   }
   return 0;
}

U32 CPRBReportCollector::getCurrentGbrDlPrbUsage(U32 uiTimePeriod) const
{
   if(uiTimePeriod > 0 && uiTimePeriod < 100)
   {
      U32 uiTotWindTime = (m_windowSize * m_schReportingInterval);
      U32 uiTimeReqd    = (uiTimePeriod * uiTotWindTime)/100;
      U32 uiNumSlots = (uiTimeReqd/m_schReportingInterval) + 1;
      return getPrbForLastSlots(uiNumSlots, RM_DIR_DL);
   }
   if(m_areAllSlotsFilled)
   {
      return((U32) RM_ROUND(m_currentGbrDlPrbUsage, m_windowSize));
   }
   else if(m_curntSlotIndx)
   {
      return((U32) RM_ROUND(m_currentGbrDlPrbUsage, m_curntSlotIndx));
   }
   return 0;
}

#ifdef DBG_MAC_RRM_PRB_PRINT
void CPRBReportCollector::displayWindow()
{
   char buffer[100];
   RLOG0 (L_INFO, "\n==========================================================="
         "==========================");
   RLOG0 (L_INFO, "\n Slot   | QCI-1[DL:UL]  | QCI-2[DL:UL]  | QCI-3[DL:UL]  |"
         "QCI-4 [DL:UL]  | SUM:[DL:UL]\n");
   RLOG0 (L_INFO, "=============================================================="
         "========================\n");
   for(U32 indx = 0; indx < m_windowSize; indx++)
   {
     sprintf(buffer, "%ld\t| [%ld: %ld]\t| [%ld: %ld]\t| [%ld: %ld]\t| [%ld: "
            "%ld]\t| [%ld:%ld]\t\n",indx+1,
            m_tvPrbUsge[indx].m_dlPrbReport[0],
            m_tvPrbUsge[indx].m_ulPrbReport[0],
            m_tvPrbUsge[indx].m_dlPrbReport[1],
            m_tvPrbUsge[indx].m_ulPrbReport[1],
            m_tvPrbUsge[indx].m_dlPrbReport[2],
            m_tvPrbUsge[indx].m_ulPrbReport[2],
            m_tvPrbUsge[indx].m_dlPrbReport[3],
            m_tvPrbUsge[indx].m_ulPrbReport[3],
            m_tvPrbUsge[indx].m_gbrDlPrbRptsSum,
            m_tvPrbUsge[indx].m_gbrUlPrbRptsSum);

      RLOG_STR (L_INFO, "%s", buffer);
   }
   RLOG2 (L_INFO, "\n Avg Current GBR PRB Usage [DL:UL]-[%ld:%ld]\n",
         getCurrentGbrDlPrbUsage(), getCurrentGbrUlPrbUsage());
}
#endif

void CPRBReportCollector::updateQCIsPRBReport(const RgmPrbRprtInd *pstPrbReport)
{
   CPRBUsageInfo*   usageInfo;
   U32              totDlPrbUsage = 0;
   U32              totUlPrbUsage = 0;

   // ignore the incoming PRB report
   if(!m_windowSize)
      return;

   usageInfo = getCurrentSlot();
   for(U32 index = 0; index < RRM_MAX_GBR_QCIS; index++)
   {
      usageInfo->m_dlPrbReport[index] = pstPrbReport->stQciPrbRpts[index].bAvgPrbDlUsage;
      usageInfo->m_ulPrbReport[index] = pstPrbReport->stQciPrbRpts[index].bAvgPrbUlUsage;
      
      totDlPrbUsage += (usageInfo->m_dlPrbReport[index]);
      totUlPrbUsage += (usageInfo->m_ulPrbReport[index]); 
   }
    
   /* update the all slots PRB usage sum */
   if(m_areAllSlotsFilled)
   {
      U32   prevStoredDlPrbReprtSum = usageInfo->m_gbrDlPrbRptsSum;
      U32   prevStoredUlPrbReprtSum = usageInfo->m_gbrUlPrbRptsSum;

      m_currentGbrDlPrbUsage += (totDlPrbUsage - prevStoredDlPrbReprtSum);
      m_currentGbrUlPrbUsage += (totUlPrbUsage - prevStoredUlPrbReprtSum);
   }
   else
   {
      m_currentGbrDlPrbUsage += totDlPrbUsage;
      m_currentGbrUlPrbUsage += totUlPrbUsage;
   }
   
   usageInfo->m_gbrDlPrbRptsSum = totDlPrbUsage;
   usageInfo->m_gbrUlPrbRptsSum = totUlPrbUsage;

#ifdef DBG_MAC_RRM_PRB_PRINT
   /* print the PRB usage report for every 20sec 
    * for Demo/debug purpose */
   static U32 count = 0 ; 
   count++;
   if((m_areAllSlotsFilled) && (count % m_DemoPrintCntFor20Sec == 0))
   {
      displayWindow();
   }
#endif
}

/********************************************************************30**

           End of file:     qf_prbreportcollector.cpp@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:02 2016

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


