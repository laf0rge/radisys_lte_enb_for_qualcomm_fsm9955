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

     Sid:      rm_rcmsps.h@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:54:57 2014

     Prg:     jbm 

*********************************************************************21*/

#ifndef RM_RCMSPS_H_
#define RM_RCMSPS_H_

#include "rm_cmn.h"
#include "rm_uecontext.h"

#define RM_RCM_EXPLCT_REL_AFTR_DL         4
#define RM_RCM_NUM_SPS_PROC_DL            2
#define RM_RCM_SPS_PRDCT                  20

typedef struct rmRcmN1PucchRes
{
   U8             bN1PucchCnt;
   U16            usN1Pucch[RM_RCM_SPS_MAX_N1_PUCCH];
}RmRcmN1PucchRes;   

#ifdef LTE_TDD
typedef enum 
{
  RM_TDD_CONFIG0_MIN_DLUL_SF = 4,
  RM_TDD_CONFIG1_MIN_DLUL_SF = 4,
  RM_TDD_CONFIG2_MIN_DLUL_SF = 2,
  RM_TDD_CONFIG3_MIN_DLUL_SF = 3,
  RM_TDD_CONFIG4_MIN_DLUL_SF = 2,
  RM_TDD_CONFIG5_MIN_DLUL_SF = 1,
  RM_TDD_CONFIG6_MIN_DLUL_SF = 5
}RmMinDlUlSfTdd;
#endif

class CRmRcmSpsMgr : public CRmRcmResMgr
{
public:
   CRmRcmSpsMgr(LrmCellConfigData &stCellCfg);
   ~CRmRcmSpsMgr();
	void rmRcmCfgSps(CUEContext	*pobjUe);
	S32 rmRcmRelSps(CUEContext	*pobjUe);
   bool isSpsEnabled() {return m_stRmCellSpsCfg.bIsSpsEnabled;};
   bool isSpsUeRem();
   void calcMaxNumSpsUe();
   U16 rmRcmSpsPopulateN1PucchResTbl();
   void getUeN1PucchResForDl(RmuSpsConfigDl *stRmSpsDlCfg, CUEContext  *pobjUe);
private:
   LrmSpsCellConfigData        m_stRmCellSpsCfg;
   RmuSpsIntervalDl            m_eSpsIntervalDl;
   U16                         m_usExplctRelAftrDl;
   U8                          m_usNumOfConfSpsProcDl;
   RmuSpsIntervalUl            m_eSpsIntervalUl;
   RmuSpsImplctRelAftr         m_eImplctRelAftrUl;
#ifdef LTE_TDD   
   Bool                        m_bTwoIntervalCfg;
#endif   
   U16                         m_usMaxSpsUePerTti;     
   U16                         m_usMaxNumSpsUe;
   RmRcmN1PucchRes             m_stN1PucchRes[RM_RCM_MAX_UES_SUPPORTED];
   U8                          m_bMaxActiveUesServed;
};
#endif /* RM_RCMDRX_H_ */
