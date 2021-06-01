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
               
     File:     rm_rcmdrx.h

     Sid:      rm_rcmdrx.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 16:45:37 2013

     Prg:     jbm 

*********************************************************************21*/

#ifndef RM_RCMDRX_H_
#define RM_RCMDRX_H_


#include "rm_cmn.h"
#include "rm_uecontext.h"

#define RM_DRX_LONG_CYCLE_MIN            10
#define RM_DRX_LONG_CYCLE_MAX            2560

/*Below Macro defines the On Duration percentage in long cycle value */
#define RM_DRX_ON_DUR_PERCENT     10

/** @brief The below Macros are used to return configuration status */
#define RM_RCM_DRX_PRES           1
#define RM_RCM_DRX_ROK            2
#define RM_RCM_DRX_RFAILED        3

class CRmRcmDrxMgr : public CRmRcmResMgr
{
public:
   CRmRcmDrxMgr(LrmCellConfigData &stCellCfg);
   ~CRmRcmDrxMgr();
	S32 rmRcmCfgDrx(CUEContext	*pobjUe, U32  bResCfgMask);
	S32 rmRcmRelDrx(CUEContext	*pobjUe);
   U16 rmRcmGetDrxOffset(U16 usDrxLongCycle, U16 usCqiPmiCfgIdx, U8 bResAllocated, U16 usOnDurationTmr);
   bool isDrxEnabled() {return m_stRmCellDrxCfg.bIsDrxEnabled;};
   U16 rmRcmDrxGetOnDuration(U16 usDrxLongCycle, CmRrmDrxCfg& m_stRmDrxCfg, CUEContext *pobjUe, U16& usOnDurationTmr);
   S16 rmRcmDrxGetShortCycle(U16 usDrxLongCycle, U16 usOnDurationTmr, CmRrmDrxCfg& m_stRmDrxCfg);
   U8 rmRcmGetDrxLongCycleIdx(U16 usDrxLongCycle);
   void rmRcmDrxPopulateOnduration();
private:
   CmRrmCellDrxCfg                m_stRmCellDrxCfg;
   U16                            rmDrxONDurationTmr[16];
   U16                            rmDrxLongCycle[16];
   U16                            rmDrxShortCycle[16];
   U16                            m_usTddOnDuration;
   LrmUlDlConfig                  m_enUlDlConfig;
   LrmCqiPrdcty                   m_enCqiPrdcty;
};

#endif /* RM_RCMDRX_H_ */
/********************************************************************30**

           End of file:     rm_rcmdrx.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 16:45:37 2013

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
