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
               

     File:     rm_rcmpcqi.cpp

     Sid:      qf_rcmpcqi.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:35 2013

     Prg:     jbm 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=146;
/* header include files */
#ifndef RM_RCMPCQI_CPP_
#define RM_RCMPCQI_CPP_

#include "rm_cmn.h"
#include "rm_rcmpcqi.h"

#define RM_RCM_CQI_PMI_IDX_FACTOR 2

CRmRcmCqiMgr::CRmRcmCqiMgr(LrmCellConfigData &stCellCfg)
{
#ifdef LTE_TDD
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_1] = 1;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_5] = 5;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_10] = 10;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_20] = 20;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_40] = 40;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_80] = 80;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_160] = 160;
   
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_1] = 0;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_5] = 1;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_10] = 6;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_20] = 16;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_40] = 36;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_80] = 76;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_160] = 156;
#else
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_2] = 2;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_5] = 5;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_10] = 10;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_20] = 20;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_40] = 40;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_80] = 80;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_160] = 160;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_32] = 32;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_64] = 64;
   m_abCqiPrdctySfNum[LRM_CQI_PRDCTY_128] = 128;
   
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_2] = 0;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_5] = 2;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_10] = 7;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_20] = 17;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_40] = 37;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_80] = 77;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_160] = 157;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_32] = 318;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_64] = 350;
   m_ausCqiStartOffset[LRM_CQI_PRDCTY_128] = 414;
#endif
   m_ausRiStartOffset[RM_RCM_MRI_1] = 0;
   m_ausRiStartOffset[RM_RCM_MRI_2] = 161;
   m_ausRiStartOffset[RM_RCM_MRI_4] = 322;
   m_ausRiStartOffset[RM_RCM_MRI_8] = 483;
   m_ausRiStartOffset[RM_RCM_MRI_16] = 644;
   m_ausRiStartOffset[RM_RCM_MRI_32] = 805;

   m_enUlDlCfg = stCellCfg.enUlDlConfig;
   m_enCqiPrdcty = stCellCfg.enCqiPrdcty;
   m_enMri = RM_RCM_MRI_2;
   m_bNumCqiPerTti = stCellCfg.usNumCqiResPerTti;
   m_bMaxActiveUesServed = 
      (stCellCfg.bMaxActiveUesServed + stCellCfg.usMaxExtraUes);

   m_usn2Pucch = stCellCfg.bNRbCqi * RM_NUM_SC_PER_RB + 
                  ((RM_CEIL((stCellCfg.bNCsAn), (RM_MAX_NUM_N1CS))) *
                   (RM_NUM_SC_PER_RB - stCellCfg.bNCsAn - 2));

   m_isDlfsEnabled = stCellCfg.flagDlfsScheduling;
   m_transModeUse = stCellCfg.flagTransMode;

   m_bSimANCqiEnb = stCellCfg.bSimulCqiAckNack;

   m_enRiPrdcty = RM_RCM_MRI_2;
   cmMemset((U8 *)m_stCqiResAllocInfo,0,sizeof(m_stCqiResAllocInfo));
}

void CRmRcmCqiMgr::rmRcmCqiPopulateCqiResTbl()
{
   const U8             bCqiPrdcty = m_abCqiPrdctySfNum[m_enCqiPrdcty];
   U16                  usNumUlSf;
   U16                  usSfnSf;
   U16                  usUeIndex; 
   U16                  usCqiResIdx; 
   U16                  usCqiOffset[RM_RCM_MAX_OFFSET_ARRAY_SIZE];
   U16                  ausRiOffset[RM_RCM_MAX_OFFSET_ARRAY_SIZE];
   U16                  usAvalUlSfs[RM_RCM_MAX_OFFSET_ARRAY_SIZE];
   U16                  usCqiOffsetIdx;
   U16                  usRiOffsetIdx;
   U16                  usRiOffset;

   usNumUlSf = getAvailUlSubframeLst(bCqiPrdcty, usAvalUlSfs);

   if(usNumUlSf > RM_RCM_MAX_OFFSET_ARRAY_SIZE)
   {
      RLOG3(L_WARNING,"Number of UL subframes [%d] for CQI resource assignment"
             "for periodicity [%d] is more than Max supported [%d]." 
             "Hence reassignig it to Max Value",usNumUlSf,bCqiPrdcty,
             RM_RCM_MAX_OFFSET_ARRAY_SIZE);

      usNumUlSf = RM_RCM_MAX_OFFSET_ARRAY_SIZE;
   }

   /* Sequence the SR offsets to avoid SR-CQI collisions */
   for (usSfnSf = 0; usSfnSf < usNumUlSf; usSfnSf++)
   {
      /* UE Index is multiplied by two, to get the CQIPMI index difference of 
       * Two between two UE's. The RI will scheduled a subframe ahead of CQI.
       * This is to accomodate the RI for the same UE in 
       * subsequent subframes and avoid the CQI-RI collision
       * Hence the RI offset is CQI - 1
       */
      usCqiOffsetIdx = (usSfnSf * RM_RCM_CQI_PMI_IDX_FACTOR + RM_RCM_CQI_PMI_IDX_FACTOR) % usNumUlSf;
      usRiOffsetIdx = (usCqiOffsetIdx - 1 + usNumUlSf) % usNumUlSf;

      usCqiOffset[usSfnSf] = usAvalUlSfs[usCqiOffsetIdx];
      ausRiOffset[usSfnSf]  = usAvalUlSfs[usRiOffsetIdx];
   }


   /* Populate the CQI Index table using the SR Offset Table */
   for (usUeIndex = 0, usCqiResIdx = 0; 
         (usUeIndex < m_bMaxActiveUesServed) && 
         (usCqiResIdx < m_bNumCqiPerTti); usCqiResIdx++)
   {
      /* For each UE two resources (one CQI and One RI, in different subfames)
       * will be allocated hence in the below check the usNumUlSf is 
       * divided by two.
       */
      for (usSfnSf = 0; (usSfnSf < usNumUlSf/2) && 
               (usUeIndex < m_bMaxActiveUesServed); usSfnSf++)
      {
         m_stCqiResAllocInfo[m_enCqiPrdcty][usUeIndex].usCqiCfgIdx = 
            usCqiOffset[usSfnSf] + m_ausCqiStartOffset[m_enCqiPrdcty];

         /* Since RI will be havng the least offset we are xsaving to 
          * derive the DRX offset */
         m_stCqiResAllocInfo[m_enCqiPrdcty][usUeIndex].usRiOffset = 
            ausRiOffset[usSfnSf];

         usRiOffset  = 
            (-1 * (((S16)(ausRiOffset[usSfnSf] - usCqiOffset[usSfnSf] 
                          - bCqiPrdcty)) % bCqiPrdcty));

         m_stCqiResAllocInfo[m_enCqiPrdcty][usUeIndex].usRiCfgIdx = 
            usRiOffset + m_ausRiStartOffset[m_enRiPrdcty];

         m_stCqiResAllocInfo[m_enCqiPrdcty][usUeIndex].usCqiResIdx = usCqiResIdx;
         usUeIndex++;
      }
   }

}

CRmRcmCqiMgr::~CRmRcmCqiMgr()
{
}

S32 CRmRcmCqiMgr::rmRcmCfgCqi(CUEContext *pobjUe)
{
	RmuPcqiFormatInd  enUeCqiFormat;
	U32               uiFactorK;
	U32               uiTxnMode;
	U32               uiRiIndex;
	/* configure the CQI Format Indicator */
	enUeCqiFormat = rmPcqiGetCqiFormatIndicator(pobjUe);

	pobjUe->setUePCqiFormat(enUeCqiFormat);

	/* Configure the CQI PMI Index and Resourfce Index */
	rmPcqiGetCqiPmiIdxResIdx(pobjUe);
   
   /* Configure the RI Index */
   uiTxnMode = (U32)pobjUe->getUeTxnMode();
	/* Configure the Factor K */
	if(enUeCqiFormat != RMU_PCQI_FMT_WIDEBAND)
	{
      if(uiTxnMode == RM_UE_TXN_MODE4)
      {
         pobjUe->setUePeriodicCqi(RMU_CQI_MODE21);
#ifndef BRDCM         
         /* In case of TM4 and DLFSS Enabled, Aperiodic CQI Reporting NOT
          * Supported to due to PHY Limitation on BRDCM SoC*/
         pobjUe->setUeAperiodicCqi(RMU_APRD_CQI_MODE31);
#endif      
      }
      else
      {
      pobjUe->setUePeriodicCqi(RMU_CQI_MODE20);
      /* if UE is SB CQI reporting capable and cell DLFSS
       * enabled then Aperiodic SB CQI reporting is configured. */
      /* Note: Only Mode 3-0 supported */
		pobjUe->setUeAperiodicCqi(RMU_APRD_CQI_MODE30);
      }
		uiFactorK = rmPcqiGetCqiFactorK();

		pobjUe->setUeCqiFactorK(uiFactorK);

	}
   else
   {
      if(uiTxnMode == RM_UE_TXN_MODE4)
      {
         pobjUe->setUePeriodicCqi(RMU_CQI_MODE11);
      }
      else
      {
         pobjUe->setUePeriodicCqi(RMU_CQI_MODE10);
      }
   }


	if((uiTxnMode == RM_UE_TXN_MODE3) ||
			(uiTxnMode == RM_UE_TXN_MODE4))
	{
		uiRiIndex = rmPcqiGetRiIdx(pobjUe);

		pobjUe->setUeCqiRiIndex(uiRiIndex, TRUE);
	}
	else
	{
		pobjUe->setUeCqiRiIndex(0, FALSE);
	}   

	rmPcqiRsrvCqiRiRes();

	pobjUe->setSimCqiAckSupp(m_bSimANCqiEnb);

   pobjUe->setUeCqiRprtInfo();

   pobjUe->setUeRiOffset(rmPcqiGetRiOffset(pobjUe->getUeIdx()));

	return ROK;
}


U16 CRmRcmCqiMgr::rmPcqiGetRiOffset(U16 usUeIndex)
{
   return m_stCqiResAllocInfo[m_enCqiPrdcty][usUeIndex].usRiOffset;
}

S32 CRmRcmCqiMgr::rmRcmRecfgCqi(CUEContext *pobjUe)
{
	rmRcmRelCqi(pobjUe);

	rmRcmCfgCqi(pobjUe);

	return ROK;
}

S32 CRmRcmCqiMgr::rmRcmRelCqi(CUEContext *pobjUe)
{
	rmPcqiRelCqiRiRes(pobjUe);

	return ROK;
}

S32 CRmRcmCqiMgr::rmPcqiGetCqiPmiIdxResIdx
(
   CUEContext  *pobjUe
)
{
	U16    	   usUeIndex;
	U16 			usCqiPmiIdx;
	U16  			usCqiResIdx;

   usUeIndex = pobjUe->getUeIdx();
   usCqiPmiIdx	= m_stCqiResAllocInfo[m_enCqiPrdcty][usUeIndex].usCqiCfgIdx;

	usCqiResIdx = m_stCqiResAllocInfo[m_enCqiPrdcty][usUeIndex].usCqiResIdx;

	pobjUe->setUeCqiPmiIdxResIdx(usCqiPmiIdx, usCqiResIdx);

	return ROK;
}

U8 CRmRcmCqiMgr::rmPcqiGetCqiFactorK()
{
	return RM_DFLT_CQI_FACTOR_K;
}

U16 CRmRcmCqiMgr::rmPcqiGetRiIdx(CUEContext	*pobjUe)
{
	return m_stCqiResAllocInfo[m_enCqiPrdcty][pobjUe->getUeIdx()].usRiCfgIdx;
}

S32 CRmRcmCqiMgr::rmPcqiRsrvCqiRiRes()
{
	return ROK;
}
S32 CRmRcmCqiMgr::rmPcqiRelCqiRiRes(CUEContext	*pobjUe)
{
	pobjUe->resetUePCqiRprtInfo();
	return ROK;
}

RmuPcqiFormatInd CRmRcmCqiMgr::rmPcqiGetCqiFormatIndicator(CUEContext *pobjUe)
{
   Bool                       isUeSuppSbCqi;

	isUeSuppSbCqi = pobjUe->getUePcqiSubbandSupp();

	if(!(isUeSuppSbCqi & m_isDlfsEnabled))
	{
		return RMU_PCQI_FMT_WIDEBAND;
	}
	else
	{
		return RMU_PCQI_FMT_SUBBAND;
	}
}

#endif /* RM_RCMPCQI_CPP_ */
/********************************************************************30**

           End of file:     qf_rcmpcqi.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:35 2013

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
