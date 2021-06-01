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

     File:     rm_prbcongestioncontroller.cpp

     Sid:      qf_prbcongestioncontroller.cpp@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/11 - Mon Feb 15 12:52:06 2016

     Prg:     sprasad, anandyal, hkrishna

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=154;

/** @file rm_prbcongestioncontroller.cpp
 * @brief This file contains the implementation of PRB congestion collector 
 *        class functions.
 */
#include "rm_radiobearercontroller.h"
#include "rm_prbcongestioncontroller.h"
#include "rm_outinterface.h"
#include "rm_prbreportcollector.h"

U32 CPRBCongestionController::convertBwToPrb(LrmCellBandwidth cellBw)
{
   U32 prb = 100;

   switch(cellBw)
   {
      case LRM_BANDWIDTH_6:
         prb = 6;
         break;
      case LRM_BANDWIDTH_15:
         prb = 15;
         break;
      case LRM_BANDWIDTH_25:
         prb = 25;
         break;
      case LRM_BANDWIDTH_50:
         prb = 50;
         break;
      case LRM_BANDWIDTH_75:
         prb = 75;
         break;
      case LRM_BANDWIDTH_100:
         prb = 100;
         break;
      default:
         RLOG1(L_ERROR,"Invalid BW config [%d]",cellBw);
         break;
   }
   return prb;
}

CPRBCongestionController::CPRBCongestionController(
                                 CRBearerController *pstRbBrCntrlr, 
                                 U8 bMaxPrbGbrUsage, LrmCellBandwidth cellBw,U32 uiWindowSize)
{
   U32   prbPerBW;
   m_usGBRPRBLimit = bMaxPrbGbrUsage;

   prbPerBW = convertBwToPrb(cellBw);

   /*Maximum threshold for congestion is considered as sum of GBR PRB limit and 
    * 10% of Total PRBs for given bandwidth. This is limited to 100*/
   m_uiMaxPrbThreshold = m_usGBRPRBLimit + 
      ((prbPerBW * RM_CONGES_MAX_PRB_THRESH)/100);

   if(m_uiMaxPrbThreshold > RM_PRB_LIMIT_FULL)
   {
      m_uiMaxPrbThreshold = RM_PRB_LIMIT_FULL;
   }

   /*Minimum threshold for congestion is considered as difference between GBR 
    * PRB limit and 5% of Total PRBs for given bandwidth.*/ 
   m_uiMinPrbThreshold = m_usGBRPRBLimit - 
      (prbPerBW * RM_CONGES_MIN_PRB_THRESH)/100;
   /*If m_usGBRPRBLimit = 0, m_uiMinPrbThreshold will wrap around. Hence the
    * below check.*/
   if(m_uiMinPrbThreshold > RM_PRB_LIMIT_FULL)
   {
      m_uiMinPrbThreshold = 0;
   }

   m_uiTimeSamples = RM_CONGES_PRB_DEFAULT_SAMPLE_TIME;
   m_uiPreemptWaitInterval = uiWindowSize;
   m_uiPrevDlPrb = 0;
   m_uiPrevUlPrb = 0;
   m_objRBearerCtrlr = pstRbBrCntrlr;
   m_bIsCongested = false;
   RLOG_ARG4(L_DEBUG, DBG_CELLID, m_objRBearerCtrlr->getCellId(),
         "PRB Congestion Configuration:: Threshold max/min: [%d, %d] "
         "timeSample During Congestion: %d waitTime: %d",
         m_uiMaxPrbThreshold, m_uiMinPrbThreshold, 
         m_uiTimeSamples, m_uiPreemptWaitInterval);
}


bool CPRBCongestionController::verifyCongestion()
{
   U32  uiCurDlPrbUsage;
   U32  uiCurUlPrbUsage;
   bool bPreemptBearer = false;

   /* get current usage for DL*/
   uiCurDlPrbUsage=CPRBReportCollector::getInstance().getCurrentGbrDlPrbUsage();

   /* get current usage for UL*/
   uiCurUlPrbUsage=CPRBReportCollector::getInstance().getCurrentGbrUlPrbUsage();

   if(uiCurUlPrbUsage >= m_uiMaxPrbThreshold ||
         uiCurDlPrbUsage >= m_uiMaxPrbThreshold)
   {
      m_bIsCongested = true;
      m_uiPrevDlPrb= CPRBReportCollector::getInstance().getCurrentGbrDlPrbUsage(
            m_uiTimeSamples);
      m_uiPrevUlPrb= CPRBReportCollector::getInstance().getCurrentGbrUlPrbUsage(
            m_uiTimeSamples);

      bPreemptBearer = true;

      RLOG_ARG4(L_INFO, DBG_CELLID, m_objRBearerCtrlr->getCellId(),
            "Cell is Congested. Curr DL/UL PRB Usage: [%ld, %ld] [%ld, %ld]",
            uiCurDlPrbUsage, uiCurUlPrbUsage, m_uiPrevDlPrb, m_uiPrevUlPrb);
   }
   return bPreemptBearer;
}

void CPRBCongestionController::verifyUncongestion()
{
   U32  uiCurDlPrbUsage;
   U32  uiCurUlPrbUsage;

   uiCurDlPrbUsage=CPRBReportCollector::getInstance().getCurrentGbrDlPrbUsage();
   uiCurUlPrbUsage=CPRBReportCollector::getInstance().getCurrentGbrUlPrbUsage();

   if(uiCurUlPrbUsage <= m_uiMinPrbThreshold &&
         uiCurDlPrbUsage <= m_uiMinPrbThreshold)
   {
      m_bIsCongested = false;
      m_uiPrevDlPrb = 0;
      m_uiPrevUlPrb = 0;

      RLOG_ARG4(L_INFO, DBG_CELLID, m_objRBearerCtrlr->getCellId(),
            "Cell is Un-Congested. Curr DL/UL PRB Usage: [%ld, %ld] [%ld, %ld]",
            uiCurDlPrbUsage, uiCurUlPrbUsage, m_uiMaxPrbThreshold,
            m_uiMinPrbThreshold);
   }
}

bool CPRBCongestionController::verifyChangeInPrbUsage()
{
   bool bPreemptBearer = false;
   U32  uiCurDlPrbUsage;
   U32  uiCurUlPrbUsage;
   static U16 uiWaitTime = 0;

   /* get current usage for DL*/
   uiCurDlPrbUsage=CPRBReportCollector::getInstance().getCurrentGbrDlPrbUsage(
         m_uiTimeSamples);

   /* get current usage for UL*/
   uiCurUlPrbUsage=CPRBReportCollector::getInstance().getCurrentGbrUlPrbUsage(
         m_uiTimeSamples);

#ifdef DBG_MAC_RRM_PRB_PRINT
   // enable only for debugging
   CPRBReportCollector::getInstance().displayWindow();
#endif

   if(uiCurDlPrbUsage < m_uiPrevDlPrb ||
         uiCurUlPrbUsage < m_uiPrevUlPrb)
   {
      // if the PRB usage is decrementing, do not preempt immediately,
      // wait for further PRB reports before deciding to preempt
      // next ERAB.

      // reset wait time
      uiWaitTime = 0;

      verifyUncongestion();
   }
   else
   {
      if(!(++uiWaitTime % m_uiPreemptWaitInterval))
      {
         // if no change in PRB usage, preempt another bearer
         bPreemptBearer = true;
         // reset preemption wait time
         uiWaitTime = 0;
      }
   }

   if(m_bIsCongested)
   {
      RLOG_ARG4(L_INFO, DBG_CELLID, m_objRBearerCtrlr->getCellId(),
            "Cell is Congested. Curr DL/UL PRB Usage: [%ld, %ld] [%ld, %ld]",
            uiCurDlPrbUsage, uiCurUlPrbUsage, m_uiPrevDlPrb, m_uiPrevUlPrb);
   }

   /* update the change in local variables */
   m_uiPrevDlPrb = uiCurDlPrbUsage;
   m_uiPrevUlPrb = uiCurUlPrbUsage;

   return bPreemptBearer;
}

bool CPRBCongestionController::checkForCongestion()
{
   bool bPreemptBearer;

   if(m_bIsCongested)
   {
      bPreemptBearer = verifyChangeInPrbUsage();
   }
   else
   {
      bPreemptBearer = verifyCongestion();
   }

   if(bPreemptBearer)
   {
      m_objRBearerCtrlr->preemptErabDueToCongestion();
   }

   return m_bIsCongested;
}

/********************************************************************30**

           End of file:     qf_prbcongestioncontroller.cpp@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/11 - Mon Feb 15 12:52:06 2016

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


