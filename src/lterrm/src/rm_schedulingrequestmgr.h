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
               
     File:     rm_resourceconfigmanager.h

     Sid:      rm_schedulingrequestmgr.h@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:51:21 2016

     Prg:     jbm 

*********************************************************************21*/

#ifndef __RM_SCHDUELING_REQUEST_MGR_H__
#define __RM_SCHDUELING_REQUEST_MGR_H__

#include "rm_cmn.h"
#include "rm_uecontext.h"


/** @brief 
 */
#define  RM_RCM_SR_PRDICITY_40MS          40
#define  RM_RCM_SR_PRDICITY_20MS          20
#define  RM_RCM_SR_START_IDX_40MS         35
#define  RM_RCM_SR_START_IDX_20MS         15
#define  RM_RCM_SR_FACTOR                 2
#define  MAX_NUM_UE_20             			20


typedef struct rmSrResAllocInfo
{
   U16      usSrResIdx;
   U16      usSrCfgIdx;
}RmSrResAllocInfo;

class CRmSchedulingReqMgr : public CRmRcmResMgr
{
public:
	CRmSchedulingReqMgr(LrmCellConfigData &stCellCfg);
	S16 configureSR(CUEContext* pUE);
   void rmRcmSrPopulateSrResTbl(U16 usSpsN1PucchResEnd);
#if 0
	S16 reConfigureSR(CUEContext* pUE);
	S16 releaseSR(CUEContext* pUE);
	S16 reserveSRRes(CUEContext* pUE);
#endif

private:
	U16 getSRResIdx(U16 bUEIdx);
	U16 getSRIdx(U16 bUEIdx);
   CmRrmDsrTransMax getDsrTransMax();

private:
	U16                  m_usN1Pucch;
	U16                  m_usSRPeriodicity;
	U16                  m_usSRStartIdx;
   U16                  m_bNumSrPerTti;
   U8                   m_abSrPrdctySfNum[LRM_SR_PRDCTY_MAX];
   U16                  m_usSrStartOffset[LRM_SR_PRDCTY_MAX];
   LrmSrPrdcty          m_enSrPrdcty;
   CmRrmDsrTransMax     m_enDsrTransMax;
   U16                  m_bMaxActiveUesServed;
   
   RmSrResAllocInfo     m_stSrResAllocInfo[LRM_UL_DL_CONFIG_MAX][RM_RCM_MAX_UES_SUPPORTED];
   

};

#endif /* __RM_SCHDUELING_REQUEST_MGR_H__ */
/********************************************************************30**

           End of file:     rm_schedulingrequestmgr.h@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:51:21 2016

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
