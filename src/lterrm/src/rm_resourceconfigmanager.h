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

     Sid:      rm_resourceconfigmanager.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 16:45:37 2013

     Prg:     jbm 

*********************************************************************21*/

#ifndef RM_RSRCCFGMGR_H_
#define RM_RSRCCFGMGR_H_

#include "rm_cmn.h"
#include "rm_rcmpcqi.h"
#include "rm_rcmdrx.h"
#include "rm_schedulingrequestmgr.h"
#include "rm_rcmsps.h"


/** @brief The below macros are the mask for the RCM resource config type
 */
#define RM_RCM_RES_CFG_SR           0x01
#define RM_RCM_RES_CFG_CQI          0x02
#define RM_RCM_RES_CFG_DRX          0x04
#define RM_RCM_RES_CFG_SRS          0x08
#define RM_RCM_RES_CFG_RI           0x10
#define RM_RCM_RES_CFG_DRX_REL      0x20
#define RM_RCM_RES_CFG_SR_REL       0x40
#define RM_RCM_RES_CFG_CQI_REL      0x80
#define RM_RCM_RES_CFG_SRS_REL      0x100
#define RM_RCM_RES_CFG_SPS          0x200
#define RM_RCM_RES_REL_SPS          0x400
#define RM_RCM_RES_CFG_TX_MODE      0x800

/** @brief This represents the number of resource configuration type supported
 * in RRM
 */
#define RM_RCM_MAX_RES_CFG_TYPE     4


/* macros for the Ranges of balues for RCM Weightage and Level Mappign tables */


#if 0
class CRmRcmAlgo
{
public:
	virtual U32 rmRcmGetResToCfg(U8 bRecfgType,CUEContext *pUE, Bool bIsDrxEnabled) = 0;
   virtual ~CRmRcmAlgo() = 0;
};

class CRmRcmAlgoBasic : public CRmRcmAlgo

#endif

class CRmRcmAlgoBasic 
{
public:
	U32 rmRcmGetResToCfg (U8 bRecfgType, CUEContext	*pUE , Bool bIsDrxEnabled, Bool bIsSpsEnabled, Bool usNumSpsUe);
   virtual ~CRmRcmAlgoBasic() {};
};


class CRmRsrcCfgMgr
{
public:
   CRmRsrcCfgMgr(LrmCellConfigData &stCellCfg);

   S32 rmRcmCfgUeRes(CUEContext* pUE, RmuUeRadioResCfg	&stUeRadioResCfg);
   S32 rmRcmRecfgUeRes(CUEContext* pUE, RmuUeRecfgReq & stUeReCfgReq,RmuUeRecfgRsp *pstUeRecfgRsp);
   S32 rmRcmRecfgUeRes(CUEContext* pUE, RmuUeHoReq & stUeHoReq, RmuUeHoRsp *pstUeHoRsp);
   S32 rmRcmProcTxnModeInd(CUEContext* pUE,RmuUeRadioResCfg  &stUeRadioResCfg );

   S32 rmRcmRelUeRes(CUEContext	*pUE);
#ifdef RRM_UNUSED_FUNC
   S32 rmRcmModCellRes();
#endif
   void rmRcmPopulateRadioRes();
   void rmRcmSpsDisInd(CUEContext* pUE);
private:
	S32 configureUeResources(CUEContext* pUE,  RmuUeRadioResCfg & stUeRadioResCfg);
	void configurePCQI(U8 bResCfgMask, RmuUeRadioResCfg & stUeRadioResCfg, CUEContext* pUE);
	void configureTxnMode(U32 bResCfgMask, RmuUeRadioResCfg & stUeRadioResCfg, CUEContext* pUE);
	void configureDRX(U8 bResCfgMask, RmuUeRadioResCfg & stUeRadioResCfg, CUEContext* pUE);
	void configureSR(U8 bResCfgMask, RmuUeRadioResCfg & stUeRadioResCfg, CUEContext* pUE);
	void configureSPS(U32 bResCfgMask, RmuUeRadioResCfg & stUeRadioResCfg, CUEContext* pUE);

private:
	CRmRcmAlgoBasic		m_objRcmAlgoInfo;
	CRmRcmCqiMgr 			m_objRcmCqiMgr;
   CRmRcmDrxMgr         m_objRcmDrxMgr;
	CRmSchedulingReqMgr	m_objSRMgr;
   CRmRcmSpsMgr         m_objRcmSpsMgr; 
	LrmCellBandwidth		m_enDlBw;
	U8						   m_bN2Rb;
	U8						   m_bN1Cs;
   U8                   m_bRcmAlgoType;

};

#endif /* RM_RSRCCFGMGR_H_ */
/********************************************************************30**

           End of file:     rm_resourceconfigmanager.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 16:45:37 2013

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
