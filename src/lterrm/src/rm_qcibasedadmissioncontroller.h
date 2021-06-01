/*******************************************************************16**

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

/** @file rm_qcibasedadmissioncontroller.h 
 * @brief This file contains the class of QCI based radio bearer admission(RBA)
 *        controller.
 */

#ifndef RM_QCIBASEDADMISSIONCONTROLLER_H_
#define RM_QCIBASEDADMISSIONCONTROLLER_H_

#include "rm_cmn.h"
#include "rm_uecontext.h"

#define RM_MAX_NUM_GBR  4             /*!< Maximum number of GBR bearers */
#define RM_MIN(x, y)    ((x) <= (y) ? (x) : (y)) /*!< To find minumum of 2 
                                                      numbers */

/**
 * @brief  Class to check if a RB can be admitted into the system without 
 *         possibly disrupting the timing requirements of the existing GBRs
 */
class CQCIBasedAdmissionController {
public:
   /**
    * @brief Constructor 
    *
    * @details Constructor, to setup the internal variables with values from 
    *          cell configuration
    *
    * @param[in]     stCellCfg   Cell Configuration
    *
    * @return  void
    */
   CQCIBasedAdmissionController(LrmCellConfigData &stCellCfg);

   /**
    * @brief To check if the ERABs can be admitted
    *
    * @detail Function to determine if the incoming list of bearers can be 
    *         allowed based on the maximum number of bearers allowed for the
    *         particular QCI (which is determined based on configured QCI
    *         timings) and the current number of bearers already admitted in 
    *         the system.
    *
    * @param[in]     stERAB         Incoming list of bearers for admission
    * @param[in/out] erabRbcRspInfo  ERAB Radio bearer control response
    *
    * @return  bool
    *          Returns true in case of success; else false
    */
   bool canRBsBeAdmitted(RmuErabConfigIe & stERAB,
                         RrmRbcRspInfo & erabRbcRspInfo);
   /**
    * @brief To check if the ERABs can be modified
    *
    * @detail Function to determine if the incoming list of bearers modification 
    *         can be allowed based on the maximum number of bearers allowed for 
    *         the particular QCI (which is determined based on configured QCI
    *         timings) and the current number of bearers already admitted in 
    *         the system if there is a change the QCI(with-in Bearer's QCI rage).
    *         Reject the modification of bearer from non-GBR to GBR or vice
    *         versa.
    *
    * @param[in]     pUE            UE control block
    * @param[in]     stERAB         Incoming list of bearers for modification
    * @param[in/out] erabRbcRspInfo  ERAB Radio bearer control response
    *
    * @return  bool
    *          Returns true in case of success; else false
    */
   bool canModifyRBsBeAdmitted(CUEContext *pUE,
                               RmuErabConfigIe & stERAB, 
                               RrmRbcRspInfo & erabRbcRspInfo);
   /**
    * @brief Release bearer for a particular QCI 
    *
    * @details Funtion to handle RAB release indication for a particular QCI.
    *          The function updates the internal states and counters regarding 
    *          the radio bearers.
    *
    * @param[in]     uiQci   QCI value
    *
    * @return  void
    */
   void rbReleaseInd(U32 uiQci);

   /**
    * @brief Add  bearer for a particular QCI 
    *
    * @details Funtion to handle RAB addition indication for a particular QCI.
    *          The function updates the internal states and counters regarding 
    *          the radio bearers.
    *
    * @param[in]     uiQci   QCI value
    *
    * @return  void
    */
   void rbAdditionInd(U32 qci);

   /**
    * @brief To fetch maximum number of bearers allowed per QCI 
    *
    * @details Function to fetch the maximum number of bearers 
    *          allowed per QCI.
    *
    * @param[in]     uiQci   QCI value
    *
    * @return  U32
    *          Returns the maximum number of bearers
    */
    U32   getMaxNumOfQCIBearersAllowed(U32 qciVal)
    {
       return(m_MaxNumOfQciBearersAllowed[qciVal-1]);
    };

private:
   /*!< Number of UE for new transmission per TTI in DL */
   U8    m_bNumDlNewUesPerTti;
   /*!< Number of UE for new transmission per TTI in UL */
   U8    m_bNumUlNewUesPerTti;
   /*!< Number of UE for retransmission per TTI in DL */
   U8    m_bNumDlReTxUesPerTti;
   /*!< Number of UE for retransmission per TTI in UL */
   U8    m_bNumUlReTxUesPerTti;
   /*!< Duplex mode - FDD / TDD */
   U8    m_bDuplexMode;
   /*!< Stores the configured QCI timings in milliseconds for QCI 1,2,3 and 4 */
   U8    m_bRbcQciTmngReq[RM_MAX_NUM_GBR];
   /*!< Indicates if timing requirement is to be enabled for QCI 4 */
   bool  m_bRbcQci4TmngReqEnable;
   /*!< Config mode - used when duplex mode is configured as TDD */
   LrmUlDlConfig      m_enConfigMode;
   /*!< Maximum number of QCI (1,2,3,4) bearers that can be allowed based on 
    *   configured QCI timings */
   U16   m_MaxNumOfQciBearersAllowed[RM_MAX_NUM_GBR];
   /*!< Current number of QCI (1,2,3,4) bearers that are admitted in the 
    *   system */
   U16   m_numOfQciBearersAdmitted[RM_MAX_NUM_GBR];
   /**
    * @brief canQCI1RBBeAdmitted
    *
    * @details Funtion to determine of the incoming QCI 1 bearer can be admitted
    *          or not based on the current number of QCI 1 bearers in the 
    *          system and the maximum number of QCI 1 beaers that can be
    *          allowed.
    *
    * @param[in]     stRabQos   Incoming QCI 1 RAB QoS information
    *
    * @return  bool
    *          Returns true in case of success; else false
    */
   bool canQCI1RBBeAdmitted(RmuUErabQosInfoIe &stRabQos);

   /**
    * @brief canQCI2RBBeAdmitted
    *
    * @details Funtion to determine of the incoming QCI 2 bearer can be admitted
    *          or not based on the current number of QCI 2 bearers in the 
    *          system and the maximum number of QCI 2 beaers that can be
    *          allowed.
    *
    * @param[in]     stRabQos   Incoming QCI 2 RAB QoS information
    *
    * @return  bool
    *          Returns true in case of success; else false
    */
   bool canQCI2RBBeAdmitted(RmuUErabQosInfoIe &stRabQos);

   /**
    * @brief canQCI3RBBeAdmitted
    *
    * @details Funtion to determine of the incoming QCI 3 bearer can be admitted
    *          or not based on the current number of QCI 3 bearers in the 
    *          system and the maximum number of QCI 3 beaers that can be
    *          allowed.
    *
    * @param[in]     stRabQos   Incoming QCI 3 RAB QoS information
    *
    * @return  bool
    *          Returns true in case of success; else false
    */
   bool canQCI3RBBeAdmitted(RmuUErabQosInfoIe &stRabQos);

   /**
    * @brief canQCI4RBBeAdmitted
    *
    * @details Funtion to determine of the incoming QCI 4 bearer can be admitted
    *          or not based on the current number of QCI 4 bearers in the 
    *          system and the maximum number of QCI 4 beaers that can be
    *          allowed.
    *
    * @param[in]     stRabQos   Incoming QCI 4 RAB QoS information
    *
    * @return  bool
    *          Returns true in case of success; else false
    */
   bool canQCI4RBBeAdmitted(RmuUErabQosInfoIe &stRabQos);
};

#endif  /* RM_QCIBASEDADMISSIONCONTROLLER_H_ */

/********************************************************************30**

       End of file:     rm_qcibasedadmissioncontroller.h@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:51:59 2016

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


