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

     Desc:     This file contains class for configuring the CQI report.
               
     File:     rm_rcmpcqi.h

     Sid:      rm_rcmpcqi.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:40 2013

     Prg:     jbm 

*********************************************************************21*/
#ifndef RM_RCMPCQI_H_
#define RM_RCMPCQI_H_


#include "rm_cmn.h"
#include "rm_uecontext.h"

/** @brief Number of Subcarrier per resource block
 */
#define RM_NUM_SC_PER_RB            12

/** @brief Maximum number of values that N_1_CS can take (0 - 7)
 */
#define RM_MAX_NUM_N1CS             8


#define RM_BYTE_MASK_ONE            1

/** @brief  Max number of subbands per bandwith parts
 */
#define RM_MAX_NUM_SUBBANDS         4

#define RM_MSB_OF_BYTE              0x80

/* Macros for Basic Algo */

#ifdef LTE_MULTIUE 
#define RM_DFLT_CQI_RI_IDX_CM2         161//Suggested by anoop for multi UE Testing
#define RM_DFLT_CQI_RI_IDX_CM1         165//Suggested by anoop for multi UE Testing
#define WR_UMM_SR_CQI_PRD_TDD       40

#define WR_NUM_SUBFRMS_IN_FRAME     10

#define WR_NUM_TDD_UL_DL_CFG_TYPES  7

#else

#define RM_DFLT_CQI_RI_IDX          161
#endif//LTE_MULTIUE

#define RM_DFLT_CQI_FACTOR_K 	      1

typedef struct rmCqiResInfo
{
   U16      usCqiResIdx;
   U16      usRiCfgIdx;
   U16      usCqiCfgIdx;
   U16      usRiOffset;
}RmCqiResInfo;

typedef enum rmRcmMri
{
   RM_RCM_MRI_1,
   RM_RCM_MRI_2,
   RM_RCM_MRI_4,
   RM_RCM_MRI_8,
   RM_RCM_MRI_16,
   RM_RCM_MRI_32,
   RM_RCM_MRI_MAX
}RmRcmMri;


class CRmRcmCqiMgr : public CRmRcmResMgr
{
public:
   CRmRcmCqiMgr(LrmCellConfigData &stCellCfg);
   ~CRmRcmCqiMgr();
	S32 rmRcmCfgCqi(CUEContext	*pobjUe);
	S32 rmRcmRecfgCqi(CUEContext  *pobjUe);
	S32 rmRcmRelCqi(CUEContext	*pobjUe);
	S32 rmRcmCfgRi(CUEContext	*pobjUe);
   void rmRcmCqiPopulateCqiResTbl();
private:
	RmuPcqiFormatInd  rmPcqiGetCqiFormatIndicator(CUEContext *pobjUe);
	S32 rmPcqiGetCqiPmiIdxResIdx(CUEContext *pobjUe);
	U8  rmPcqiGetCqiFactorK();
	U16 rmPcqiGetRiIdx(CUEContext *pobjUe);
	S32 rmPcqiRsrvCqiRiRes();
	S32 rmPcqiRelCqiRiRes(CUEContext *pobjUe);
   U16 rmPcqiGetRiOffset(U16 usUeIndex);

private:
	U16 					   	m_usn2Pucch;
	U16							m_isDlfsEnabled;
   U16                     m_transModeUse;
   bool                    m_bSimANCqiEnb;

   U8                      m_bNumCqiPerTti;
   U8                      m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_MAX];
   U16                     m_ausCqiStartOffset[LRM_CQI_PRDCTY_MAX];
   U16                     m_ausRiStartOffset[RM_RCM_MRI_MAX];
   RmCqiResInfo            m_stCqiResAllocInfo[LRM_UL_DL_CONFIG_MAX][RM_RCM_MAX_UES_SUPPORTED];

   LrmCqiPrdcty            m_enCqiPrdcty;
   RmRcmMri                m_enMri;
   U16                     m_enRiPrdcty;
   U8                      m_bMaxActiveUesServed;
};

#endif /* RM_RCMPCQI_H_ */
/********************************************************************30**

           End of file:     rm_rcmpcqi.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:40 2013

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

