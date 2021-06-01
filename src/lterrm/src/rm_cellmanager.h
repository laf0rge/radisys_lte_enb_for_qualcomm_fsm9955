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
               

     File:     rm_cellmanager.h

     Sid:      rm_cellmanager.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:38 2013

     Prg:      hkrishna, vkorade 

*********************************************************************21*/


#ifndef RM_CELLCONTEXT_H_
#define RM_CELLCONTEXT_H_

#include "rm_cmn.h"
/*#include "cm_lte.x"
#include "cm_lte_rrm.x"
#include "lrm.x"
#include "rmu.x"*/
#include "rgm.x"
#include "rm_admissioncontroller.h"
#include "rm_resourceconfigmanager.h"
#include "rm_rcmpcqi.h"
#include "rm_radiobearercontroller.h"
#include "rm_uemanager.h"
#include "rm_acb.h"
#include "rm_prbcongestioncontroller.h"

typedef enum 
{
   RM_ENB_STATE_UP = 0,
   RM_ENB_STATE_DOWN
}RmEnbState;

class CCellManager;
class CAccessClassBarring;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
class CCellManager
{
public:
   CCellManager(LrmCellConfigData *pstCellCfg);
   void reConfigureCell(LrmCellConfiguration &stCellCfg);
   void triggerUeRelInd(U16 usCrnti, RmuRrcCause enRelCause);
   void procUeAdmitReq(RmuUeAdmitReq & stUeAdmitReq);
   void procUeRecfgReq(RmuUeRecfgReq & stUeRecfgReq);
   void procUeRelReq(RmuUeRelReq *pstUeRelReq);
   void procUeHoReq(RmuUeHoReq & stUeHoReq);
   void procUeSpsDisInd(RmuUeSpsDisInd *pstUeSpsDisInd);
   /*RRM_SP3*/
   /**
    * @brief Funtion to handle UE inactivity indication
    *
    * @detail Function to update the UE control block with UE inactivity 
    *         information based on incoming inactivity event.
    *
    * @param[in]   stUeInactInd    UE inactivity report
    *
    * @return  void
    */
   void procUeInactInd(RmuUeInactInd & stUeInactInd);
   void deleteUe(CUEContext *pUE);
   void procChangeTransMode(RgmTransModeInd *stChangeTransMode);
   S16 triggerStopPrbUsageReport();
   S16 triggerStartPrbUsageReport();

   void handlePrbRprt(RgmPrbRprtInd *prbRprt);
#if 0
   bool isUeCanBeMadeActive(CUEContext * pUE, RmuStatusInfo& stStatusInfo);
#endif

   inline U16 getbroadBandUlBandwidth();
   inline U16 getbroadBandDlBandwidth();
   inline U16 getMaxUesServed();
   inline U8 getCellID() { return m_bCellId; } ;
   inline U8 getNumTxAnt()
   {
      return m_stCellCfgInfo.bNumTxAnt;
   }

   inline void  rmInitRadioRes();

public:
   CAccessClassBarring m_objAcb;
   static Bool m_bIsGbrDrxDis;
  
   static Bool  getGbrDrxDisFlag(){return CCellManager::m_bIsGbrDrxDis;};
   static void  setGbrDrxDisFlag(Bool bIsGbrDrxDis){CCellManager::m_bIsGbrDrxDis = bIsGbrDrxDis;};
   void setEnbState(U8 enEnbState);

   /**
    * @brief Function to get cell congestion state.
    * 
    * @details This function returns the congestion state of the system.
    * 
    * @return bool
    *         Returns m_bIsCellCongested;
    */
   bool getIsCellCongested(){return m_bIsCellCongested;};

private:
   /*!< Object of UE manager class */
   CUEManager m_objUEMgr;
   /*!< Object of Admission Controller class */
   CAdmissionController m_oAdmCtrlr;
   /*!< Object of Resource configuration manager class */
   CRmRsrcCfgMgr m_objResCfgMgr;
   /*!< Object of Radio bearer controller class */
   CRBearerController m_objRBearerCtrlr;
   /*!< Object of PRB congestion class */
   CPRBCongestionController m_objPrbCongCntrl;
   /*!< Cell Identity */
   U8 m_bCellId;
   /*!< Current Cell Configuration */
   LrmCellConfigData m_stCellCfgInfo;
   /*!< Indicates if PRB reporting is configured to Scheduler */
   bool m_bIsPrbReportConfigured;
   /*!< Operating state of system */
   RmEnbState m_enEnbState;
   U8 m_dftTxMode;

   /*!< Inidicates if cell is congested */
   bool m_bIsCellCongested;

private:
   U8 getTxnModeForUe(RmuEutraCapInfo &stEutraCapInfo);
   void updateCRNTI(RmuUeRecfgReq & stUeRecfgReq, RmuUeRecfgRsp *pstUeRecfgRsp);
   void modCsgMembrShp(CUEContext * pUE, RmuUeRecfgReq & stUeRecfgReq, RmuUeRecfgRsp *pstUeRecfgRsp);
   void handleERABReq(CUEContext * pUE, RmuUeRecfgReq & stUeRecfgReq, RmuUeRecfgRsp *pstUeRecfgRsp);
   void handleUEReconfigReq(CUEContext * pUE, RmuUeRecfgReq & stUeRecfgReq, RmuUeRecfgRsp *pstUeRecfgRsp);
   void handleUECapInfo(CUEContext * pUE, RmuUeRecfgReq & stUeRecfgReq);
   RmEnbState getEnbState();
};

inline U16 CCellManager::getbroadBandUlBandwidth()
{
   return (m_stCellCfgInfo.usMaxUlBroadbandBw);
}
inline U16 CCellManager::getbroadBandDlBandwidth()
{
   return (m_stCellCfgInfo.usMaxDlBroadbandBw);
}
inline U16 CCellManager::getMaxUesServed()
{
   return (m_stCellCfgInfo.bMaxActiveUesServed);
}
inline void CCellManager::rmInitRadioRes()
{
   m_objResCfgMgr.rmRcmPopulateRadioRes();
}


#endif /* RM_CELLCONTEXT_H_ */
/********************************************************************30**

           End of file:     rm_cellmanager.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:38 2013

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
