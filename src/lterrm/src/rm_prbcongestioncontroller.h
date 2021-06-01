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

     Type:     CPP include file

     Desc:     This file contains 
               
     File:     rm_prbcongestioncontroller.h

     Sid:      rm_prbcongestioncontroller.h@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/11 - Mon Feb 15 12:52:07 2016

     Prg:      RRM Team

*********************************************************************21*/

/** @file rm_prbcongestioncontroller.h
 * @brief This file contains the PRB congestion collector class which 
 *        is used to decide if the system is congested or not based on 
 *        the current GBR PRB usage reported by scheduler.
 */

#ifndef RM_PRBCONGESTIONCONTROLLER_H_
#define RM_PRBCONGESTIONCONTROLLER_H_

#include "rm_cmn.h"

/*!< Percentage of PRB usage to be used to calculate the lower threshold for 
 * congestion */
#define RM_CONGES_MIN_PRB_THRESH    5

/*!< Percentage of PRB usage to be used to calculate the upper threshold for 
 * congestion */
#define RM_CONGES_MAX_PRB_THRESH    10

/*!< Percentage of time samples to be considered while checking for 
 *   un-congestion */
#define RM_CONGES_PRB_DEFAULT_SAMPLE_TIME  99

/*!< Maximum available PRBs a the system */
#define RM_PRB_LIMIT_FULL 100

/*!< PRB limit when RM_PRB_LIMIT_FULL is configured */
#define RM_PRB_LIMIT_FOR_FULL_CFG 98

class CRBearerController;

/**
 * @brief Class to determine if system is congested.
 */
class CPRBCongestionController
{
public:
   /**
    * @brief Constructor
    * 
    * @details This function updates the min and max PRB threshold values from
    *          file. If RRM configuration file is not present, the default 
    *          values are used.
    * 
    */
   CPRBCongestionController(CRBearerController *pstRbBrCntrlr, U8 bMaxPrbGbrUsage, 
                            LrmCellBandwidth cellBw, U32 uiWindowSize);

   /**
    * @brief Function to decide if system is congested
    * 
    * @details This function checks for congestion and preempts a ERAB,
    *          if indicated, to recover from congestion.
    *          If the system is already congested, this function invokes
    *          verifies the change in PRB usage to determine if 
    *          the system has recovered from congestion.
    * 
    * @return bool
    *          Returns true if congestion, else false.
    */
   bool checkForCongestion();

private:
   /*!< Indicates if system is congested or not */
   bool  m_bIsCongested;

   /*!< Max GBR PRB limit to decide that system is congested (GBR PRB Limit +
    * 10% of GBR PRB Limit) */
   U32   m_uiMaxPrbThreshold;

   /*!< Min GBR PRB limit to decide that system is uncongested (GBR PRB Limit -
    * 5% of GBR PRB Limit) */
   U32   m_uiMinPrbThreshold;

   /*!< Maximum limit of GBR PRB usage in percentage */
   U8    m_usGBRPRBLimit;

   /*!< Reference of radio bearer controller object in cellManager */
   CRBearerController *m_objRBearerCtrlr;

   /*!< DL PRB usage when congestion occured */
   U32 m_uiPrevDlPrb;

   /*!< UL PRB usage when congestion occured */
   U32 m_uiPrevUlPrb;

   /*!< Samples for last 'x' millisecons to be used to detect un-congestion */
   U16 m_uiTimeSamples;

   /*!< Number of reports to consider before preempting another bearer 
    * during congestion */
   U16 m_uiPreemptWaitInterval;

   /**
    * @brief Function to decide congestion
    * 
    * @details This function verifies the current GBR PRB usage reported by
    *          Scheduler with max PRB thresholds to decide if
    *          the system is congested.
    * 
    * @return bool
    *          Returns true if congestion, else false.
    */
   bool verifyCongestion();

   /**
    * @brief Function to decide un-congestion
    * 
    * @details This function verifies the change in GBR PRB usage reported by
    *          Scheduler with the min PRB thresholds to decide if
    *          the system has recovered from congestion.
    * 
    * @return void
    */
   void verifyUncongestion();

   /**
    * @brief Function to detect change in PRB usage after preempting a ERAB
    *        to recover from congestion.
    * 
    * @details This function verifies the current GBR PRB usage reported by
    *          Scheduler in the last n number of samples, to decide if the
    *          earlier preemption of an ERAB has caused reduction in PRB
    *          usage or not.
    *          If there is no change in the GBR PRB usage or if the change in
    *          PRB usage is not enough to recover from congestion, another
    *          ERAB is preempted.
    * 
    * @return bool
    *          Returns true if congestion, else false.
    */
   bool verifyChangeInPrbUsage();

   /**
    * @brief Function to read from internal RRM configuration
    * 
    * @details This function reads from the internal RRM configuration file and 
    *          updates the internal variables. If the file is not present,
    *          the default values are used.
    * 
    * @return void
    */
   void readAndUpdateFromRrmCfgFile();

   /**
    * @brief Function to return the number of PRBs for given bandwidth
    * 
    * @details This function takes bandwidth enum as input and returns the value
    *          of total number of PRBs for particular bandwidth.
    * 
    * @param[in] cellBw   Bandwidth enum
    *
    * @return U32 
    *            Return the total number of PRBs
    */
    U32 convertBwToPrb(LrmCellBandwidth cellBw);

};

#endif  /* RM_PRBCONGESTIONCONTROLLER_H_ */

/********************************************************************30**

       End of file:     rm_prbcongestioncontroller.h@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/11 - Mon Feb 15 12:52:07 2016

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


