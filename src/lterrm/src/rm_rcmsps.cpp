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
               

     File:     rm_rcmsps.cpp

     Sid:      qf_rcmsps.cpp@@/main/TeNB_Main_BR/1 - Tue Aug 12 15:42:56 2014

     Prg:      sagarwal 

*********************************************************************21*/

/* header include files */
#ifndef RM_RCMSPS_CPP_
#define RM_RCMSPS_CPP_

#include "rm_cmn.h"
#include "rm_rcmsps.h"

CRmRcmSpsMgr::CRmRcmSpsMgr(LrmCellConfigData &stCellCfg)
{
   m_stRmCellSpsCfg        = stCellCfg.stSpsCellCfg;
   m_eSpsIntervalDl        = RMU_SPS_DL_INTERVAL_E20;
   m_usExplctRelAftrDl     = RM_RCM_EXPLCT_REL_AFTR_DL; 
   m_usNumOfConfSpsProcDl  = RM_RCM_NUM_SPS_PROC_DL; 
   m_eSpsIntervalUl        = RMU_SPS_UL_INTERVAL_E20;
   m_eImplctRelAftrUl      = RMU_SPS_E2;
#ifdef LTE_TDD    
   m_bTwoIntervalCfg       = TRUE;
#endif   
   m_bMaxActiveUesServed   = stCellCfg.bMaxActiveUesServed;

   /* Calculating the no. of SPS UE and DL SPS UE per TTI 
    * MIN(usMaxDlSpsUePerTti, usMaxDlSpsUePerTti) is set and
    * usMaxSpsUePerTti and based on this value no. of SPS UEs
    * are calculated */
   m_enUlDlCfg = stCellCfg.enUlDlConfig;
   calcMaxNumSpsUe();
   cmMemset((U8 *)m_stN1PucchRes, 0, sizeof(m_stN1PucchRes));
}

CRmRcmSpsMgr::~CRmRcmSpsMgr()
{
   //Do Nothing 
   return;
}

void CRmRcmSpsMgr::rmRcmCfgSps(CUEContext  *pobjUe)
{
   RmuUeSpsCfg       stRmSpsCfg;
   RmuSpsConfigDl    *stRmSpsDlCfg = &stRmSpsCfg.stSpsConfigDl;
   RmuSpsConfigUl    *stRmSpsUlCfg = &stRmSpsCfg.stSpsConfigUl;

   /* Fill parameter for DL SPS */
   stRmSpsDlCfg->bIsSpsEnabled = TRUE;
   stRmSpsDlCfg->eSpsIntervalDl = m_eSpsIntervalDl;
   stRmSpsDlCfg->usExplctRelAftr = m_usExplctRelAftrDl;
   stRmSpsDlCfg->usNumOfConfSpsProc = m_usNumOfConfSpsProcDl;

   /* calculate the N1 PUCCH resources for this UE */
   getUeN1PucchResForDl(stRmSpsDlCfg, pobjUe);

   /* Fill parameter for UL SPS */
   stRmSpsUlCfg->bIsSpsEnabled = TRUE;
   stRmSpsUlCfg->eSpsIntervalUl = m_eSpsIntervalUl;
   stRmSpsUlCfg->eImplctRelAftr = m_eImplctRelAftrUl;
#ifdef LTE_TDD    
   stRmSpsUlCfg->bTwoIntervalCfg = m_bTwoIntervalCfg;
#endif   
   stRmSpsUlCfg->bP0PersistentPres = FALSE;

   /*Decrease the count of number of SPS UE */
   m_usMaxNumSpsUe--;

   pobjUe->setSpsCfg(stRmSpsCfg);
}

S32 CRmRcmSpsMgr::rmRcmRelSps(CUEContext  *pobjUe)
{
   /* Calling function to release SPS configuration for this UE */
   pobjUe->relSpsCfg(); 

   /* Increasing the count of number of SPS UE */
   m_usMaxNumSpsUe++;

   RETVALUE(ROK);
}   

bool CRmRcmSpsMgr::isSpsUeRem()
{
   if(m_usMaxNumSpsUe > 0)
   {
      RETVALUE(TRUE);
   }   
   else 
   {
      RETVALUE(FALSE);
   }   
}   

void CRmRcmSpsMgr::calcMaxNumSpsUe()
{
#ifdef LTE_TDD
    U8           bMinDlUlSf = 0;
#endif    
   /* usMaxSpsUePerTti is set as minimum of usMaxUlSpsUePerTti and 
    * usMaxDlSpsUePerTti. */
   if(m_stRmCellSpsCfg.usMaxDlSpsUePerTti < m_stRmCellSpsCfg.usMaxUlSpsUePerTti)
   {   
      m_usMaxSpsUePerTti = m_stRmCellSpsCfg.usMaxDlSpsUePerTti;
   } 
   else
   {   
      m_usMaxSpsUePerTti = m_stRmCellSpsCfg.usMaxUlSpsUePerTti;
   }

#ifdef LTE_TDD
   /* Max no. of SPS UEs are calculated based on
    * MIN(NUM_DL_SF_PER_RF, NUM_UL_SF_PER_RF) and SPS periodicity and 
    * numSpsUePerTti */
   switch(m_enUlDlCfg)
   {
      case LRM_UL_DL_CONFIG_0:
         bMinDlUlSf = RM_TDD_CONFIG0_MIN_DLUL_SF;
         break;
      case LRM_UL_DL_CONFIG_1:
         bMinDlUlSf = RM_TDD_CONFIG1_MIN_DLUL_SF;
         break;
      case LRM_UL_DL_CONFIG_2:
         bMinDlUlSf = RM_TDD_CONFIG2_MIN_DLUL_SF;
         break;
      case LRM_UL_DL_CONFIG_3:
         bMinDlUlSf = RM_TDD_CONFIG3_MIN_DLUL_SF;
         break;
      case LRM_UL_DL_CONFIG_4:
         bMinDlUlSf = RM_TDD_CONFIG4_MIN_DLUL_SF;
         break;
      case LRM_UL_DL_CONFIG_5:
         bMinDlUlSf = RM_TDD_CONFIG5_MIN_DLUL_SF;
         break;
      case LRM_UL_DL_CONFIG_6:
         bMinDlUlSf = RM_TDD_CONFIG6_MIN_DLUL_SF;
         break;
      case LRM_UL_DL_CONFIG_MAX:
         break;
   }
   m_usMaxNumSpsUe = (U16) (bMinDlUlSf * m_usMaxSpsUePerTti * (RM_RCM_SPS_PRDCT/10));
#else   
   /* usMaxNumSpsUe is calculated based on usMaxSpsUePerTti and SPS 
    * periodicity (currently set as 20 ms) */ 
   m_usMaxNumSpsUe = (U16) (m_usMaxSpsUePerTti * RM_RCM_SPS_PRDCT);
#endif 

   /* In case if calculated usMaxNumSpsUe is greater than MAX UE supported
    * then set usMaxNumSpsUe as RM_RCM_MAX_UES_SUPPORTED */
   if(m_usMaxNumSpsUe > m_bMaxActiveUesServed)
   {   
      m_usMaxNumSpsUe = m_bMaxActiveUesServed;
   }   
}                     

U16 CRmRcmSpsMgr::rmRcmSpsPopulateN1PucchResTbl()
{
   U16 usMaxSpsUeSchd = 0;          /* Used to specify max no. of SPS UEs which 
                                       will be scheduled to use N1-PUCCH resources 
                                       in a particular TTI */
   U8  usMaxN1PucchResAllocPerUe;   /* Used tp specify the MAX no. of PUCCH resources 
                                       allocated for a particular UE */
   U32 cnt = 0;

   /* By default assigning the value of usMaxN1PucchResAllocPerUe as 
    * RM_RCM_SPS_MAX_N1_PUCCH (4)  */
   usMaxN1PucchResAllocPerUe = RM_RCM_SPS_MAX_N1_PUCCH;

   /* Calulating the values for parameter usMaxSpsUeSchd and 
    * usMaxN1PucchResAllocPerUe. usMaxN1PucchResAlloc can have max value as 4 
    * because atmost 4 N1-PUCCH resources can be used in a particular TTI 
    * for SPS. This value may change depending on usMaxSpsUeSchd.
    * In case if usMaxSpsUeSchd is less than 4 then usMaxN1PucchResAllocPerUe
    * will be set as the value of usMaxSpsUeSchd else it will be set as 4 */
    
   if(m_stRmCellSpsCfg.bIsSpsEnabled)
   {   
#ifdef LTE_TDD
      if(m_enUlDlCfg == LRM_UL_DL_CONFIG_1)
      {             
         /* In case of TDD Config-1, 2 DL subframe UEs are scheduled in one 
          * UL subframe to send UL data. So usMaxSpsUeSchd is calculated 
          * as given below */
         usMaxSpsUeSchd = m_usMaxSpsUePerTti * 2;  

         if(m_usMaxSpsUePerTti == 1)
         {
            /* In case if usMaxSpsUePerTti is 1 then the value of 
             * usMaxN1PucchResAllocPerUe is changed to 2*/
            usMaxN1PucchResAllocPerUe = 2;
         }
      }   
      else if(m_enUlDlCfg == LRM_UL_DL_CONFIG_2)
      {
         /* In case of TDD Config-2, 4 DL subframe UEs are scheduled in one 
          * UL subframe to send UL data. So usMaxSpsUeSchd is calculated 
          * as given below */
         usMaxSpsUeSchd = m_usMaxSpsUePerTti * 4;  
      }  
#else
      /* In case of FDD, the value of usMaxN1PucchResAllocPerUe will be set based 
       * on usMaxSpsUePerTti and the value of usMaxSpsUeSchd will 
       * always be equal to usMaxSpsUePerTti */
      if(m_usMaxSpsUePerTti < RM_RCM_SPS_MAX_N1_PUCCH)
      {
         usMaxN1PucchResAllocPerUe = (U8) m_usMaxSpsUePerTti;
      }
      usMaxSpsUeSchd = m_usMaxSpsUePerTti;
#endif   

      /* **************ALGO LOGIC DESCRIPTION**************/

      /* Case 1: If usMaxSpsUeSchd <= 4 then the value of usMaxN1PucchResAllocPerUe 
       * will be same as the value of usMaxSpsUeSchd 
       * For ex: if both the parameter are set as 4 then N1-PUCCH pattern will be:
       * UE1: {0,1,2,3}, UE2: {0,1,2,3}...... Same paatern will repeat for max no.
       * of UEs. 
       * */

      /* case 2: If usMaxSpsUeSchd > 4 then usMaxN1PucchResAllocPerUe will always 
       * be 4
       * For ex: If usMaxSpsUeSchd is 5 then N1-PUCCH pattern will be:
       * UE1: {0,1,2,3}, UE2: {1,2,3,4}, UE3: {2,3,4,0}, UE4: {3,4,0,1}.......
       * in the same way pattens are generated for max no. of UEs. 
       * */
      
      for(U16 bUeIdx = 0; bUeIdx < m_bMaxActiveUesServed; bUeIdx++)                                  
      { 
         for(U8 bIdx = 0; bIdx < usMaxN1PucchResAllocPerUe; bIdx++)
         {
            m_stN1PucchRes[bUeIdx].usN1Pucch[bIdx] = (U16) (cnt % usMaxSpsUeSchd); 
            cnt++;
         }   
         m_stN1PucchRes[bUeIdx].bN1PucchCnt = usMaxN1PucchResAllocPerUe;

         if(usMaxSpsUeSchd > RM_RCM_SPS_MAX_N1_PUCCH)
         {
            /* In case if usMaxSpsUeSchd is more than 4 then case 2 is 
             * applicable. In this case the 2nd entry of last pattern becomes 
             * the first entfy of latest pattern generating. So assigning the 
             * 2nd value to cnt paramter before generating the next pattern */
            cnt = m_stN1PucchRes[bUeIdx].usN1Pucch[1];
         }   
      }
   }

   RETVALUE(usMaxSpsUeSchd);
}

void CRmRcmSpsMgr::getUeN1PucchResForDl(RmuSpsConfigDl *stRmSpsDlCfg, CUEContext *pobjUe)
{
   U16 usUeIdx = pobjUe->getUeIdx();

   cmMemset((U8 *)stRmSpsDlCfg->usN1Pucch, 0, sizeof(stRmSpsDlCfg->usN1Pucch));

   /* Fetch the N1-PUCCH resourcesi for this UE based on its ueIdx */
   stRmSpsDlCfg->usNumN1Pucch = m_stN1PucchRes[usUeIdx].bN1PucchCnt;

   for(U8 bIdx = 0; bIdx < stRmSpsDlCfg->usNumN1Pucch; bIdx++)
   {
      stRmSpsDlCfg->usN1Pucch[bIdx] = m_stN1PucchRes[usUeIdx].usN1Pucch[bIdx];
   }
}   
   
#endif /* RM_RCMSPS_CPP_ */
/********************************************************************30**

           End of file:     qf_rcmsps.cpp@@/main/TeNB_Main_BR/1 - Tue Aug 12 15:42:56 2014

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
