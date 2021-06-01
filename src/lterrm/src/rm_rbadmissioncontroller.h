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

/********************************************************************20**

     Name:     RRM Application

     Type:     CPP include file

     Desc:     This file contains 
               
     File:     rm_rbadmissioncontroller.h

     Sid:      rm_rbadmissioncontroller.h@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:00 2016

     Prg:      Amit Ghosh

*********************************************************************21*/

#ifndef RM_RBADMISSIONCONTROLLER_H_
#define RM_RBADMISSIONCONTROLLER_H_

#include "rm_cmn.h"
#include "rm_prbbasedadmissioncontroller.h"
#include "rm_qcibasedadmissioncontroller.h"

class CCellManager;

class CRBAdmissionController {
public:
   /**
    * @brief CRBAdmissionController
    *
    * @detail Constructor, initializes PRB based admission controller and QCI
    *         based admission controller objects.
    *
    * @param[in]     stCellCfg   Cell Configuration
    *
    * @return  void
    */
   CRBAdmissionController(LrmCellConfigData &stCellCfg, 
                          CCellManager *pstObjCellMngr);

   /**
    * @brief Funtion to admit UEs when establishing a new ERAB
    *
    * @detail Function to invoke QCI based admission controller and PRB based 
    *         admission controller to determine if the incoming bearers can be
    *         admitted or not. The output is indicated in the form of accepted
    *         and reject bearer lists.
    *
    * @param[in]     pUE             UE control block
    * @param[in]     stERAB          Incoming bearer requests
    * @param[out]    erabRbcRspInfo  ERAB Radio bearer control response
    *
    * @return  void
    */
   void canRBsBeAdmitted(CUEContext *pUE,
                         RmuErabConfigIe & stERAB,
                         RrmRbcRspInfo & erabRbcRspInfo);

   /**
    * @brief Funtion to admit UEs when modifying an existing ERAB
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
    * @param[in/out] erabRbcRspInfo ERAB Radio bearer control response
    *
    * @return  void
    */
   void canModifyRBsBeAdmitted(CUEContext *pUE,
                               RmuErabConfigIe & stERAB, 
                               RrmRbcRspInfo & erabRbcRspInfo);

   /**
    * @brief rbReleaseInd
    *
    * @detail Function to is used to update the release of a bearer of a
    *         particular QCI type.
    *
    * @param[in]     uiQci   QCI Type
    *
    * @return  void
    */
   void rbReleaseInd(U32 qci);

   /**
    * @brief rbAdditionInd
    *
    * @detail Function to is used to update incoming bearer count of a
    *         particular QCI type.
    *
    * @param[in]     uiQci   QCI Type
    *
    * @return  void
    */
    void rbAdditionInd(U32 qci);

   /**
    * @brief triggerPrbReportConfig
    *
    * @detail Function to fetch the trigger PRB report configuration towards
    *         scheduler. This funtion can be used to send both START and 
    *         STOP request.
    *
    * @param[in]     cellId   Cell Identity
    * @param[in]     action   Report configuration type.
    *                         [RGM_PRB_REPORT_START - to start reporting PRB
    *                         usage and RGM_PRB_REPORT_STOP  - to stop 
    *                         reporting PRB usage]
    *
    * @return  S16
    *          Returns ROK in case of success and RFAILED in case of failure.
    */
   S16 triggerPrbReportConfig(U8 cellId, U32 action);
private:
   /*!< Instance of PRB based admission controller class */
   CPRBBasedAdmissionController m_objPrbAdmissionController;
   /*!< Instance of QCI based admission controller class */
   CQCIBasedAdmissionController m_objQciAdmissionController;
   /*!< Reference to cell context */
   CCellManager *m_pstCellMngr;


   /**
    * @brief Funtion to add all accepted ERABs to rejected ERAB list
    *
    * @detail This function moves all the accepted ERABs from accepted list
    *         to rejected ERAB list.
    *
    * @param[in]  stERAB          Incoming bearer requests
    * @param[out] erabRbcRspInfo  ERAB Radio bearer control response
    *
    * @return  void
    */
   void addAllAccptErabsToRejList(
                               RmuErabConfigIe & stERAB, 
                               RrmRbcRspInfo & erabRbcRspInfo);

};

#endif  /* RM_RBADMISSIONCONTROLLER_H_ */

/********************************************************************30**

       End of file:     rm_rbadmissioncontroller.h@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:52:00 2016

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


