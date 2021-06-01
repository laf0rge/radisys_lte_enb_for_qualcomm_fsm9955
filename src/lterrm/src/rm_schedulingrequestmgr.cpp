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
               

     File:     rm_schedulingrequestmgr.cpp

     Sid:      qf_schedulingrequestmgr.cpp@@/main/TeNB_Main_BR/2 - Mon Aug 11 16:56:51 2014

     Prg:     jbm , vikram korde

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=148;
/* header include files */
#include "rm_cmn.h"
#include "rmu.x"
#include "rm_schedulingrequestmgr.h"


CRmSchedulingReqMgr::CRmSchedulingReqMgr(LrmCellConfigData &stCellCfg)
{
   m_abSrPrdctySfNum[LRM_SR_PRDCTY_5] = 5;
   m_abSrPrdctySfNum[LRM_SR_PRDCTY_10] = 10;
   m_abSrPrdctySfNum[LRM_SR_PRDCTY_20] = 20;
   m_abSrPrdctySfNum[LRM_SR_PRDCTY_40] = 40;
   m_abSrPrdctySfNum[LRM_SR_PRDCTY_80] = 80;
   m_abSrPrdctySfNum[LRM_SR_PRDCTY_2] =  2;
   m_abSrPrdctySfNum[LRM_SR_PRDCTY_1] =  1;

   m_usSrStartOffset[LRM_SR_PRDCTY_5] = 0;
   m_usSrStartOffset[LRM_SR_PRDCTY_10] = 5;
   m_usSrStartOffset[LRM_SR_PRDCTY_20] = 15;
   m_usSrStartOffset[LRM_SR_PRDCTY_40] = 35;
   m_usSrStartOffset[LRM_SR_PRDCTY_80] = 75;
   m_usSrStartOffset[LRM_SR_PRDCTY_2] =  155;
   m_usSrStartOffset[LRM_SR_PRDCTY_1] =  157;

   m_usN1Pucch = 0;
   m_usSRPeriodicity = 0;
   m_usSRStartIdx = 0;
   m_enSrPrdcty = stCellCfg.enSrPrdcty;
   m_bNumSrPerTti = stCellCfg.usNumSrResPerTti;
   m_enDsrTransMax = stCellCfg.enDsrTransMax;
   m_enUlDlCfg = stCellCfg.enUlDlConfig;
   m_bMaxActiveUesServed = 
      (stCellCfg.bMaxActiveUesServed + stCellCfg.usMaxExtraUes);
   cmMemset((U8 *)m_stSrResAllocInfo, 0, sizeof(m_stSrResAllocInfo)); 
}


void CRmSchedulingReqMgr::rmRcmSrPopulateSrResTbl(U16  usSrResIdxStart)
{
   U8                   bSrPrdcty = m_abSrPrdctySfNum[m_enSrPrdcty];
   U16                  usNumUlSf;
   U16                  usOffsetIdx;
   U16                  usSfnSf;
   U16                  usUeIndex; 
   U16                  usSrResIdx; 
   U16                  usSrOffset[RM_RCM_MAX_OFFSET_ARRAY_SIZE];
   U16                  usAvalUlSfs[RM_RCM_MAX_OFFSET_ARRAY_SIZE];

   usNumUlSf = getAvailUlSubframeLst(bSrPrdcty, usAvalUlSfs);

   if(usNumUlSf > RM_RCM_MAX_OFFSET_ARRAY_SIZE)
   {
      RLOG3(L_WARNING,"Number of UL subframes [%d] for SR resource assignment"
            "for periodicity [%d] is more than Max supported [%d]."
            "Hence reassignig it to Max Value\n",usNumUlSf,bSrPrdcty,
            RM_RCM_MAX_OFFSET_ARRAY_SIZE);

      usNumUlSf = RM_RCM_MAX_OFFSET_ARRAY_SIZE;
   }
   
   /* Sequence the SR offsets to avoid SR-CQI collisions */
   for (usSfnSf = 0; usSfnSf < usNumUlSf; usSfnSf++)
   {

      /* UE Index is multiplied by two, to get the CQIPMI index difference of 
       * Two between two UE's. This is to accomodate the RI for the same UE in 
       * subsequent subframes and avoid the CQI-RI collision
       */
      if (usSfnSf < (RM_RCM_GET_HALF(usNumUlSf)))
      {
         usOffsetIdx = (usSfnSf * RM_RCM_SR_FACTOR) % usNumUlSf;
      }
      else
      {
         /* Offset = ( - ((UeId - (SrPrd/2)) * 2 + 1 )) % srPrd */
         usOffsetIdx = (usNumUlSf - 
                ((usSfnSf - (usNumUlSf/RM_RCM_SR_FACTOR)) * RM_RCM_SR_FACTOR
                      + RM_RCM_GET_HALF(RM_RCM_SR_FACTOR))) % usNumUlSf;
      }
      
      usSrOffset[usSfnSf] = usAvalUlSfs[usOffsetIdx];
   }

   RLOG0(L_DEBUG,"SR Resources Resever Per UE Index is as Below");
   /* Populate the SR Index table using the SR Offset Table */
   for (usUeIndex = 0, usSrResIdx = usSrResIdxStart; 
         (usUeIndex < m_bMaxActiveUesServed) && 
           (usSrResIdx < (m_bNumSrPerTti + usSrResIdxStart)); usSrResIdx++)
   {
      for (usSfnSf = 0; (usSfnSf < usNumUlSf) && 
            (usUeIndex < m_bMaxActiveUesServed); usSfnSf++)
      {
         m_stSrResAllocInfo[m_enSrPrdcty][usUeIndex].usSrResIdx = usSrResIdx;
         m_stSrResAllocInfo[m_enSrPrdcty][usUeIndex].usSrCfgIdx = 
                    usSrOffset[usSfnSf] + m_usSrStartOffset[m_enSrPrdcty];
         RLOG3(L_DEBUG,"UeIdx =%lu SR Res Idx =%lu SR Cfg Idx =%lu\n", usUeIndex,
                    m_stSrResAllocInfo[m_enSrPrdcty][usUeIndex].usSrResIdx,
                    m_stSrResAllocInfo[m_enSrPrdcty][usUeIndex].usSrCfgIdx);
         usUeIndex++;
      }
   }
   RLOG0(L_DEBUG,"End of SR resource Distribution");
}

S16 CRmSchedulingReqMgr::configureSR(CUEContext* pUE)
{
   U16 usUeIndex = pUE->getUeIdx();
	/* Configure the SR Index */
	U16 usSrIdx = getSRIdx(usUeIndex);

	/* Configure the SR Resource Index */
	U16 usSrResIdx = getSRResIdx(usUeIndex);

   CmRrmDsrTransMax enDsrTransMax = getDsrTransMax();
   
	/* Configure SR Index & SR Resource Index */
	pUE->setSrCfg(usSrIdx, usSrResIdx, enDsrTransMax);

	return ROK;
}

U16 CRmSchedulingReqMgr::getSRResIdx(U16 usUeIndex)
{
	return m_stSrResAllocInfo[m_enSrPrdcty][usUeIndex].usSrResIdx;
}

U16 CRmSchedulingReqMgr::getSRIdx(U16 usUeIndex)
{

	return m_stSrResAllocInfo[m_enSrPrdcty][usUeIndex].usSrCfgIdx;
}
 
CmRrmDsrTransMax CRmSchedulingReqMgr::getDsrTransMax()
{
	return m_enDsrTransMax;
}

/********************************************************************30**

End of file:     qf_schedulingrequestmgr.cpp@@/main/TeNB_Main_BR/2 - Mon Aug 11 16:56:51 2014

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
