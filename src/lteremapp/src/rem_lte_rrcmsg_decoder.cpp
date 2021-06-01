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


/**************************************************************************************************

     Name:     REM-APP

     Type:     C++ Source file

     Desc:     This file consists of the fsm functions for REM-APP

     File:     rem_lte_rrcmsg_decoder.cpp

     Sid:      rem_lte_rrcmsg_decoder.cpp@@/main/Tenb_Intg_Branch_CC/TeNB_Dev_REM_Branch/1 - Fri Jan 11 16:46:06 2013

     Prg:      Chetan H


**************************************************************************************************/

#define CMFILE_REORG_2
#define SS_SINGLE_THREADED
#define _REENTRANT
#define DEBUGNOEXIT
/*DEBUGP
CM_INET2 -DCCPU_OPT*/

/* Header include files */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm_tkns.h"       /* Common Token defines */
#include "cm_mblk.h"       /* Common Mem Allocation defines */
#include "cm_llist.h"      /* Common Link list defines */
#include "cm_hash.h"       /* Common Hashlist defines */
//#include "cm_lte.h"        /* Common Umts defines */
#include "cm_pasn.h"       /* Common Pasn defines */
#include "cm_lte.h"       /* Common LTE defines */
#include "nhu_asn.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"           /* Timer services */
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_mblk.x"       /* Common Memory Allocation */
#include "cm_llist.x"      /* Common Linklist definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_pasn.x"       /* Common Pasn definitions */
//#include "cm_lte.x"        /* Common LTE definitions */
#include "cm_lib.x"        /* Common Library */
#include "nhu_asn.x"

#include "nmmcl_ltemsgs.h"
#include "rem_interface.h"
#include <sstream>
#include <math.h>

EXTERN PUBLIC CmPAsnElmDef *nhBCCH_DL_SCH_Pdu[];
EXTERN PUBLIC CmPAsnElmDef *nhBCCH_BCH_Pdu[];

typedef struct lteDecodePdu
{
   CmMemListCp    memCp;
   union
   {
      NhuBCCH_BCH_Msg    bcchBchMsg;
      NhuBCCH_DL_SCH_Msg bcchDlSchMsg;
   }u;
} LteDecodePdu;


void remExtractPLMNInfo(NhuPLMN_IdentityInfo & stPLMNInfo,  __stLteBcchPlmnInfo & stPlmnInfo);
void remLteDecodeSIB1(NhuSysInfoBlockTyp1cellAccessRelatedInfo *cellAcs,  __stLteCellInfo & stCellInfo);
void remLteStoreSibsSchedulingInfo(__stLteCellInfo & stCellInfo, NhuSchedulingInfoLst *schdList);
void remPrintLteResult(__stLteCellInfo stCellInfo); 
void remLteStrBuffToDec(U8 *val,U8 len,U32 *decVal);
// void remDecodePrach(NhuPRACH_ConfigSIB & prach_Config, __stPrachInfo & stPrachInfo );
void remDecodePrach(NhuPRACH_ConfigSIB & prach_Config, __stLteCellInfo & stCellInfo);
int getbSiWindow(U8 val);
bool checkSib4Transmitted(NhuSchedulingInfoLst schInfoLst);

///////////////////////////////////////////////////////////////////////////////////////////////////
// This function is used to decode Si Window
///////////////////////////////////////////////////////////////////////////////////////////////////
int getbSiWindow(U8 val)
{
U8 bSiWindow = 2;
switch(val)
{
   case 0:
      bSiWindow = 1;
      break;
   case 1:
      bSiWindow = 2;
      break;
   case 2:
      bSiWindow = 5;
      break;
   case 3:
      bSiWindow = 10;
      break;
   case 4:
      bSiWindow = 15;
      break;
   case 5:
      bSiWindow = 20;
      break;
   case 6:
      bSiWindow = 40;
      break;
   }

   return bSiWindow;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : pstBcchData  - pointer to __stLteNMM_BCCH_DATA_IND
//				 stCellInfo   - __stLteCellInfo
//        [out]: void
//	This function is used to decode BCCH Message
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void decodeLteBCCHMessage(__stLteNMM_BCCH_DATA_IND* pstBcchData, __stLteCellInfo & stCellInfo)
{ 
	Buffer             *mBuf = NULL;
	U32        numDecOct = 0;
	CmPAsnErr      err;
	S16 ret;

   TRACE_PRINTF("Num BCCH DATA %d\n", pstBcchData->bNumBcchDataList);

	for(int i=0;i <pstBcchData->bNumBcchDataList;i++)
	{
		LteDecodePdu decodePdu;

      TRACE_PRINTF("BCCH DATA PACKET %d LEN %d\n", i+1, pstBcchData->stBcchDataElmnt[i].usLen);
      TRACE_PRINTF("BCCH Buffer\n");

	   for(int j =0; j <pstBcchData->stBcchDataElmnt[i].usLen; j++)
         TRACE_PRINTF("%x ", pstBcchData->stBcchDataElmnt[i].bData[j]);

      TRACE_PRINTF("\n");	

      if (pstBcchData->stBcchDataElmnt[i].usLen != 0)
      { 
		   memset(&decodePdu, 0, sizeof(decodePdu));

		   ret = SGetMsg(DFLT_REGION, DFLT_POOL, &mBuf);
		   if(ret == RFAILED)
			   return;

		   SAddPstMsgMult((Data *)pstBcchData->stBcchDataElmnt[i].bData, 
						(MsgLen)pstBcchData->stBcchDataElmnt[i].usLen, mBuf);

		   /* Calling PASN library decode function */
		   ret = cmPAsnDecMsgUalign ((Void*)&(decodePdu.u.bcchDlSchMsg), CM_PASN_USE_MBUF, (Void **)&mBuf,
              &numDecOct, 0, nhBCCH_DL_SCH_Pdu, CM_PASN_GEN_ERR, &err,  pstBcchData->stBcchDataElmnt[i].usLen, (CmMemListCp *)&decodePdu, 0);

		   SPutMsg(mBuf);
		   if(ret == RFAILED) {
			   return;
		   }
      }
      else
      {
        continue;
      }
		/*Check for SIB1 and other SIB presence.*/
      if(decodePdu.u.bcchDlSchMsg.message.val.c1.choice.val == C1_SYSTEMINFOBLOCKTYP1 )
      {
         NhuSysInfoBlockTyp1cellAccessRelatedInfo *cellAcs = NULL;
         U32 freqBandIndicator;
         NhuSchedulingInfoLst  *schdLst = NULLP;
         /* Tx Power Changes */
         freqBandIndicator =
            decodePdu.u.bcchDlSchMsg.message.val.c1.val.systemInfoBlockTyp1.freqBandIndicator.val;
         /* End of Tx Power Changes */

         cellAcs = &(decodePdu.u.bcchDlSchMsg.message.val.c1.val.systemInfoBlockTyp1.cellAccessRelatedInfo);
         remLteDecodeSIB1(cellAcs, stCellInfo);
         /*update the scheduling info list*/
         schdLst = &(decodePdu.u.bcchDlSchMsg.message.val.c1.val.
                            systemInfoBlockTyp1.schedulingInfoLst);
         remLteStoreSibsSchedulingInfo(stCellInfo, schdLst);

#ifdef LTE_TDD
         NhuTDD_Config *tddCfg = NULL;
         tddCfg = &(decodePdu.u.bcchDlSchMsg.message.val.c1.val.systemInfoBlockTyp1.tdd_Config);
         if (tddCfg->pres.pres != NOTPRSNT )
         {
            stCellInfo.ulDlSubframeCfg = (U8)tddCfg->subframeAssignment.val;
            stCellInfo.splSfCfg = (U8)tddCfg->specialSubfrmPatterns.val;
         }

#endif
         stCellInfo.bSiWindow=
            getbSiWindow((U8)decodePdu.u.bcchDlSchMsg.message.val.c1.val.systemInfoBlockTyp1.si_WindowLength.val);
         stCellInfo.bSib1Decoded = true;
         //stCellInfo.bSib4Decoded = !(checkSib4Transmitted(decodePdu.u.bcchDlSchMsg.message.val.c1.val.systemInfoBlockTyp1.schedulingInfoLst));
         remPrintLteResult(stCellInfo);
         return;
      }

      /* else Other SI's*/
      if(decodePdu.u.bcchDlSchMsg.message.val.c1.val.systemInfo.criticalExtns.val.systemInfo_r8.pres.pres == PRSNT_NODEF)
      {
         TRACE_PRINTF("Number of SIB's received is %d\n",
               decodePdu.u.bcchDlSchMsg.message.val.c1.val.systemInfo.criticalExtns.val.systemInfo_r8.sib_TypAndInfo.noComp.val);

         for(int j=0;j<(decodePdu.u.bcchDlSchMsg.message.val.c1.val.systemInfo.criticalExtns.val.systemInfo_r8.sib_TypAndInfo.noComp.val);j++)
         {
            NhuSysInfo_r8_IEssib_TypAndInfoMember *member=NULL;
            member = &(decodePdu.u.bcchDlSchMsg.message.val.c1.val.systemInfo.criticalExtns.val.systemInfo_r8.sib_TypAndInfo.member[j]);
            if( member->choice.val ==  MEMBER_SIB2 )
            {
               TRACE_PRINTF("%%%% SIB2 DECODED %%%%%% \n");
               stCellInfo.bSib2Decoded = true;

               //Update remaining

               if(member->val.sib2.freqInfo.pres.pres != NOTPRSNT )
               {
                  stCellInfo.stBcch.uiUlBandwidth = (U16)member->val.sib2.freqInfo.ul_Bandwidth.val;
                  stCellInfo.uiUlEARFCN = (U16)member->val.sib2.freqInfo.ul_CarrierFreq.val;
               }
               else
               {
                  stCellInfo.uiUlEARFCN = stCellInfo.uiEARFCN + 18000;
               }
               stCellInfo.stBcch.iRSTxPower = (S16)member->val.sib2.radioResourceConfigCmn.pdsch_ConfigCmn.referenceSignalPower.val;

					if( member->val.sib2.radioResourceConfigCmn.prach_Config.pres.pres != NOTPRSNT )
                        remDecodePrach(member->val.sib2.radioResourceConfigCmn.prach_Config, stCellInfo);

				   TRACE_PRINTF("BW %d RX POWER %d UL Freq is %d\n",
                     stCellInfo.stBcch.uiUlBandwidth,
                     stCellInfo.stBcch.iRSTxPower, stCellInfo.uiUlEARFCN);
				}


				if( member->choice.val ==  MEMBER_SIB3 )
				{
               TRACE_PRINTF("%%%% SIB3 DECODED %%%%%% \n");
            }

            if( member->choice.val ==  MEMBER_SIB4 )
				{
               U8 numNghCell;

               TRACE_PRINTF("%%%% SIB4 DECODED %%%%%% \n");
					stCellInfo.bSib4Decoded = true;

				   TRACE_PRINTF("Num Intra-Freq Neighbors = %d\n",
                             member->val.sib4.intraFreqNeighCellLst.noComp.val);
               stCellInfo.uiNumIntraFreqNghs =
                  member->val.sib4.intraFreqNeighCellLst.noComp.val;
               for(numNghCell = 0; numNghCell <
                   member->val.sib4.intraFreqNeighCellLst.noComp.val;
                   numNghCell++)
               {
                  NhuIntraFreqNeighCellInfo intraFreqNghCell;
                  intraFreqNghCell =
                     member->val.sib4.intraFreqNeighCellLst.member[numNghCell];

                  stCellInfo.stNghInfo[numNghCell + 1].uiPhyCellId =
                     intraFreqNghCell.physCellId.val;
                  stCellInfo.stNghInfo[numNghCell + 1].iQOffset =
                     intraFreqNghCell.q_OffsetCell.val;
                  TRACE_PRINTF("Neighbor %d, PCI %d, q-Offset %d\n", numNghCell,
                        stCellInfo.stNghInfo[numNghCell + 1].uiPhyCellId,
                        stCellInfo.stNghInfo[numNghCell + 1].iQOffset);
               }
				} 
			}
		}
	}
}

/* check and return true if SIB4 is not transmitted */

bool checkSib4Transmitted(NhuSchedulingInfoLst schInfoLst)
{
   for(int i=0;i < schInfoLst.noComp.val;i++)
   {
      for(int j=0;j < schInfoLst.member[i].sib_MappingInfo.noComp.val;j++)

      {
         if(schInfoLst.member[i].sib_MappingInfo.member[j].val == NhuSIB_TypsibTyp4Enum)
            return true;
      }
   }
   return false;
}

void remDecodePrach(NhuPRACH_ConfigSIB & prach_Config, __stLteCellInfo & stCellInfo )
{
   stCellInfo.stPrachInfo.uiRootSequenceIndex  = prach_Config.rootSequenceIdx.val;

    TRACE_PRINTF("DECODING PRACH prarmaters \n");
    if( prach_Config.prach_ConfigInfo.pres.pres != NOTPRSNT )
    {
		stCellInfo.stPrachInfo.uiConfigurationIndex= prach_Config.prach_ConfigInfo.prach_ConfigIdx.val;
		stCellInfo.stPrachInfo.bHighSpeedFlag= prach_Config.prach_ConfigInfo.highSpeedFlag.val;
		stCellInfo.stPrachInfo.uiZeroCorrelationZoneConfig= prach_Config.prach_ConfigInfo.zeroCorrelationZoneConfig.val;
		stCellInfo.stPrachInfo.uiFreqOffset= prach_Config.prach_ConfigInfo.prach_FreqOffset.val;

      TRACE_PRINTF("REM: prachCfgIdx %lu, HighSpeedFlag %d, RootSeqIdx %lu, FreqOffset %lu, ZeroCorrZoneCfg %lu\n", stCellInfo.stPrachInfo.uiConfigurationIndex,
            stCellInfo.stPrachInfo.bHighSpeedFlag, stCellInfo.stPrachInfo.uiRootSequenceIndex,
            stCellInfo.stPrachInfo.uiFreqOffset,
            stCellInfo.stPrachInfo.uiZeroCorrelationZoneConfig);
    }
}

void remLteStoreSibsSchedulingInfo(__stLteCellInfo & stCellInfo, NhuSchedulingInfoLst *schdList )
{
   U8                count;
   U8                sibsCount;
   NhuSchedulingInfo *schdInfoLst = NULLP;
   NhuSIB_Typ        *sibType = NULLP;

   stCellInfo.sibsToDecode = 0;
   /*As SIB2 will be broadcasted in every 16 milisecond 
     so hard coding the SIB2 periodicity*/
   stCellInfo.sibsInfo.numSibsToDecode = 0;
   
   stCellInfo.sibsInfo.sibsPeriodicity[stCellInfo.sibsInfo.numSibsToDecode] = 
                  REM_SIB_PERIODICITY_16;
   stCellInfo.sibsInfo.numSibsToDecode++;
   /*check for SIB3 and SIB4 decoding periodicity*/
   for(count = 0; count < schdList->noComp.val; count ++)
   {
      schdInfoLst = &(schdList->member[count]);
      switch(schdInfoLst->si_Periodicity.val)
      {
         case NhuSchedulingInfosi_Periodicityrf8Enum :
         {
            stCellInfo.sibsInfo.sibsPeriodicity[stCellInfo.sibsInfo.numSibsToDecode] = 
             REM_SIB_PERIODICITY_8;
         }
         break;
         case NhuSchedulingInfosi_Periodicityrf16Enum :
         {
            stCellInfo.sibsInfo.sibsPeriodicity[stCellInfo.sibsInfo.numSibsToDecode] = 
             REM_SIB_PERIODICITY_16;
         }
         break;
         case NhuSchedulingInfosi_Periodicityrf32Enum :
         {
            stCellInfo.sibsInfo.sibsPeriodicity[stCellInfo.sibsInfo.numSibsToDecode] = 
             REM_SIB_PERIODICITY_32;
         }
         break;
         case NhuSchedulingInfosi_Periodicityrf64Enum :
         {
            stCellInfo.sibsInfo.sibsPeriodicity[stCellInfo.sibsInfo.numSibsToDecode] = 
             REM_SIB_PERIODICITY_64;
         }
         break;
         case NhuSchedulingInfosi_Periodicityrf128Enum :
         {
            stCellInfo.sibsInfo.sibsPeriodicity[stCellInfo.sibsInfo.numSibsToDecode] = 
             REM_SIB_PERIODICITY_128;
         }
         break;
         case NhuSchedulingInfosi_Periodicityrf256Enum :
         {
            stCellInfo.sibsInfo.sibsPeriodicity[stCellInfo.sibsInfo.numSibsToDecode] = 
             REM_SIB_PERIODICITY_256;
         }
         break;
         default :
         break;
      }
      //stCellInfo.sibsInfo.numSibsToDecode++;
      printf("***************number of SIBS to decode %d \n", stCellInfo.sibsInfo.numSibsToDecode);
      for(sibsCount = 0; sibsCount < schdInfoLst->sib_MappingInfo.noComp.val; sibsCount ++)
      {
         /*check whether this periodicity for SIB3 or SIB4 */
         sibType = &(schdInfoLst->sib_MappingInfo.member[sibsCount]);
         if(sibType->val == NhuSIB_TypsibTyp3Enum)
         {
            stCellInfo.sibsInfo.numSibsToDecode++;
   //printf("====SIB3 has to be decode====\n");
            stCellInfo.sibsToDecode |= REM_DECODE_SIB3; 
         }
         if(sibType->val == NhuSIB_TypsibTyp4Enum)
         {
            stCellInfo.sibsInfo.numSibsToDecode++;
  // printf("====SIB4 has to be decode====\n");
            stCellInfo.sibsToDecode |= REM_DECODE_SIB4; 
         }
      }
   }
}

void remLteDecodeSIB1(NhuSysInfoBlockTyp1cellAccessRelatedInfo *cellAcs,  __stLteCellInfo & stCellInfo )
{
   int iNum = 0;
   U32 uiCellId = 0;
   U32 uiCsgIdentity = 0;
   U32 uiTac = 0;


	if (cellAcs->cellIdentity.val != NULL)
   remLteStrBuffToDec(cellAcs->cellIdentity.val,cellAcs->cellIdentity.len,&uiCellId);
   stCellInfo.stBcch.uiCellId = (U32)uiCellId;
    stCellInfo.stBcch.bCsgInd = cellAcs->csg_Ind.val;
   if (cellAcs->csg_Identity.val != NULL)
   remLteStrBuffToDec(cellAcs->csg_Identity.val,cellAcs->csg_Identity.len,&uiCsgIdentity);
   stCellInfo.stBcch.uiCsgIdentity = (U32) uiCsgIdentity;
    stCellInfo.stBcch.bCellBarred= cellAcs->cellBarred.val;
   if (cellAcs->trackingAreaCode.val != NULL)
   remLteStrBuffToDec(cellAcs->trackingAreaCode.val,cellAcs->trackingAreaCode.len,&uiTac);
    stCellInfo.stBcch.uiTac = (U32) uiTac;
    stCellInfo.stBcch.uiPlmnListNumOfEntries = cellAcs->plmn_IdentityLst.noComp.val;

   TRACE_PRINTF("**********After DECODING *************\n");
   TRACE_PRINTF("\tCELL IDENTITY = %d\n ", stCellInfo.stBcch.uiCellId);
   TRACE_PRINTF("\tCSG IDENTITY = %d\n ", stCellInfo.stBcch.uiCsgIdentity);
   TRACE_PRINTF("\tTRACKING AREA CODE =%d\n ",stCellInfo.stBcch.uiTac);
   TRACE_PRINTF("************************* *************\n");

	// Updated by Vikram K, on 16/01/2013
	// PLMN List extraction

	stCellInfo.stBcch.uiPlmnListNumOfEntries = 0; 
	for(int j=0;j<(cellAcs->plmn_IdentityLst.noComp.val);j++) 
	{
		if( cellAcs->plmn_IdentityLst.member[j].pres.pres == NOTPRSNT )
			continue;
		remExtractPLMNInfo(cellAcs->plmn_IdentityLst.member[j],  stCellInfo.stBcch.stPLMNList[iNum++]);
	}

	stCellInfo.stBcch.uiPlmnListNumOfEntries = iNum;
}

void remExtractPLMNInfo(NhuPLMN_IdentityInfo & stPLMNInfo,  __stLteBcchPlmnInfo & stPlmnInfo)
{
	U16 i,j;

	stPlmnInfo.bCellRsrvdForOperUse = (U8)stPLMNInfo.cellReservedForOperatorUse.val;
	
	for(i=0; i<stPLMNInfo.plmn_Identity.mcc.noComp.val; i++)
      stPlmnInfo.szPLMN[i] = stPLMNInfo.plmn_Identity.mcc.member[i].val + '0';

	for(j=0; j<stPLMNInfo.plmn_Identity.mnc.noComp.val; j++)
      stPlmnInfo.szPLMN[i+j] = stPLMNInfo.plmn_Identity.mnc.member[j].val + '0';
            
	TRACE_PRINTF("\tPLMN= %s\n", stPlmnInfo.szPLMN);
}

void remPrintLteResult(__stLteCellInfo stCellInfo)
{
#ifdef DEBUG_PRINT	
   TRACE_PRINTF("***************remPrintLteResult start****************\n");
   TRACE_PRINTF("stCellInfo.uiEARFCN = %d \n",stCellInfo.uiEARFCN);
   TRACE_PRINTF("stCellInfo.uiPhyCellID  = %d \n",stCellInfo.uiPhyCellID);
   TRACE_PRINTF("stCellInfo.iRSRP = %d \n",stCellInfo.iRSRP);
   TRACE_PRINTF("stCellInfo.iRSRQ = %d \n",stCellInfo.iRSRQ);
   TRACE_PRINTF("stCellInfo.iRSSI = %d \n",stCellInfo.iRSSI);
   TRACE_PRINTF("stCellInfo.bSib1Decoded = %d \n",stCellInfo.bSib1Decoded);
   TRACE_PRINTF("stCellInfo.bSib2Decoded = %d \n",stCellInfo.bSib2Decoded);
   TRACE_PRINTF("stCellInfo.bSib4Decoded = %d \n",stCellInfo.bSib4Decoded);
   TRACE_PRINTF("stCellInfo.bPLMNNotConfigured = %d \n",stCellInfo.bPLMNNotConfigured);
   TRACE_PRINTF("stCellInfo.iSIBRetryCount = %d \n",stCellInfo.iSIBRetryCount);
   TRACE_PRINTF("stCellInfo.stBcch.iRSTxPower = %d \n",stCellInfo.stBcch.iRSTxPower);
   TRACE_PRINTF("stCellInfo.stBcch.uiTac = %d \n",stCellInfo.stBcch.uiTac);
   TRACE_PRINTF("stCellInfo.stBcch.uiCellId = %d \n",stCellInfo.stBcch.uiCellId);
   TRACE_PRINTF("stCellInfo.stBcch.bCellBarred = %d \n",stCellInfo.stBcch.bCellBarred);
   TRACE_PRINTF("stCellInfo.stBcch.bCsgInd = %d \n",stCellInfo.stBcch.bCsgInd);
   TRACE_PRINTF("stCellInfo.stBcch.uiCsgIdentity = %d \n",stCellInfo.stBcch.uiCsgIdentity);
   TRACE_PRINTF("stCellInfo.stBcch.uiPlmnListNumOfEntries = %d \n",stCellInfo.stBcch.uiPlmnListNumOfEntries);
   TRACE_PRINTF("***************remPrintLteResult end****************\n");
#endif
}

void remLteStrBuffToDec (U8 *val,U8 len,U32 *decVal)
{
   U8 length = len;
   U32 tmpVal = 0;
   U8 x = 0;
   U8 maskedVal = 0;
   for(;length>8;)
   {
      length = length - 8;
      tmpVal |=val[x++];
      if (length > 8)
         tmpVal = tmpVal << 8;
      else
      {
         maskedVal = val[x];
         maskedVal = maskedVal >> (8 - length);
         tmpVal = tmpVal << length;
         tmpVal |= maskedVal;
      }
   }
   *decVal = tmpVal;
}

#if 0
///////////////////////////////////////////////////////////////////////////////////////////////////
// @ param[in] : void*
//        [out]: void
//	This function is used to decode PBCH Message
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void decodeLtePBCHMessage(void *pBuff)
{ 
   LteDecodePdu      *decodePdu = NULL;
   Buffer             *mBuf = NULL;
   U32                numDecOct;
   CmPAsnErr      err;
   MsgLen len=0;
   S16 ret;

   //Allocate memory to nhuDecodeSduCfm using cmAllocEvent

   //Allocate memory for mBuf
   SAddPstMsgMult((Data *)pBuff, len, mBuf);

   /* Calling PASN library decode function */
   ret = cmPAsnDecMsgUalign ((Void*)&(decodePdu.u.bcchBchMsg), CM_PASN_USE_MBUF, (Void **)&mBuf,
                            &numDecOct, 0, nhBCCH_BCH_Pdu, CM_PASN_GEN_ERR, &err,
                            4096, (CmMemListCp *)decodePdu,
                            0);
}
#endif
