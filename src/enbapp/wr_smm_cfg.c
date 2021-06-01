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

     Name:     EnodeB Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_smm_cfg.c

     Sid:      fc_smm_cfg.c@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:52:01 2014

     Prg:      Sriky 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_MODULE_ID=2;

static int RLOG_FILE_ID=41;
/* header includes */
#include "wr.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_lwr.h"
#include "wr_smm_init.h"
#include "wr_smm_smallcell.h"
#include "cm_os.h"       /* common hashing */
#include "cm_os.x"
#include "ss_diag.h"      /* Common logging header */
#include <stdlib.h>
#ifdef LTE_L2_MEAS
#include "lpj.h"
#include "lkw.h"
#include "lrg.h"
#endif/*LTE_L2_MEAS*/
#include "wr_kpi.h"
#ifdef WR_RSYS_OAM
#include "wr_msm_common.h"
extern MsmLteeNodeBparams lteeNodeBparams;
#endif
#ifdef WR_WATCHDOG
EXTERN U16 softTimeout;
EXTERN U16 hardTimeout;
#endif
#define MAX_LOG_FILE_NAME_LENGTH  32
U8 logFileName[WR_MAX_LOG_PATH_VALUE]; 
U8 logPath[MAX_LOG_FILE_NAME_LENGTH]; 

#define WR_SMM_VALIDATE_CFG_PARAM(_param, _min, _max)\
{\
   if(_min > _param || _param > _max)\
   {\
      fprintf(stderr, "Invalid parameter value %s=%d, it should be min=%d, max=%d\n", \
      #_param, _param, _min, _max);\
      RETVALUE(RFAILED);\
   }\
}

PUBLIC Void wrSmmSetBandwidth(U32    freqBandwidth);
/* SPS changes starts */
PUBLIC U32 wrSmDlCellBwInRbs(CtfBandwidth dlCellBw);
/* SPS changes ends */

 PUBLIC S16 smWrReadConfigParams (Void);
 PUBLIC Void getData(FILE *fptr,char *fieldName ,char *FieldValue);

/*ccpu00126694*/
/*
*
*       Fun:   wrGetNeghPlmn
*
*       Desc:  This function return neighbours plmn
*
*       Ret:   
*
*       Notes: None
*
*       File:  wr_smm_init_brcm.c
*
*/
#ifdef ANSI
PRIVATE S16 wrGetNeghPlmn
(
U32 plmn,
U8 *nghPlmn,
U8 *noMnc
)
#else
PRIVATE S16 wrGetNeghPlmn(plmn, nghPlmn, noMnc)
U32 plmn;
U8 *nghPlmn;
U8 *noMnc;
#endif
{
   S16 count=0;
   U32 temp;
   temp = plmn;
   while( temp>0)
   {
      temp =temp/10;
      count ++;
   }
   if( count == 6)
   {
      *noMnc = 3;
   }
   else if(count == 5)
   {
      *noMnc =2;
      nghPlmn[count] = -1;
   }
   else
   {
      RETVALUE(RFAILED);
   }
   count --;
   while( count >=0 )
   {
     nghPlmn[count] = plmn%10;
     plmn = plmn/10;
     count --;
    }
    RETVALUE(ROK);
}

/* SPS changes starts */
/*
*
*       Fun:  wrSmDlCellBwInRbs 
*
*       Desc:  Function Converts enum to exact value
*
*       Ret:   U8
*
*       Notes: None
*
*       File:  wr_smm_init.c
*
*/
PUBLIC U32 wrSmDlCellBwInRbs
(
 CtfBandwidth    dlCellBw
)
{
   U32 dlBwInRbs = 0;
   switch (dlCellBw)
   {
      case CTF_BW_RB_6:
         dlBwInRbs = 6;
         break;
      case CTF_BW_RB_15:
         dlBwInRbs = 15;
         break;
      case CTF_BW_RB_25:
         dlBwInRbs = 25;
         break;
      case CTF_BW_RB_50:
         dlBwInRbs = 50;
         break;
      case CTF_BW_RB_75:
         dlBwInRbs = 75;
         break;
      case CTF_BW_RB_100:
         dlBwInRbs = 100;
         break;
      default:
         break;
   }
   RETVALUE(dlBwInRbs);
}
/* SPS changes ends */

/*ccpu00126694*/
/*
*
*       Fun:   smWrReadConfigParams
*
*       Desc:  Reads the values of the configurable parameters from 
*              the file wr_cfg.txt and stores them
*              in the smCfgCb
*
*       Ret:   ROK, RFAILED
*
*       File:  wr_smm_cfg.c
*
*/

/*
   U8    parm[30][40];
   S32   val[50];
   U8 tag[50];
   U8 tagVal[50];
   U8  paramLst[50][50];
   S8    numstr[30];*/
#ifdef ANSI
PUBLIC S16 smWrReadConfigParams
(
)
#else
PUBLIC S16 smWrReadConfigParams (Void)
#endif
{  
   FILE  *fp;
   U16 tagNum = 0;
   Bool status = TRUE;
   U16 noOfParams = 0;
   U8 loopCnt = 0;
   U16 paramCnt = 0;
   U8 noOfElem = 0;
   LwrNghCellCfg *nghCellCfg = NULLP;
   U8 tag[100];
   U8 tagVal[512];
   U8  paramLst[50][50];
   LwrNghUtraFddCfg   *nghUtraFddInfo = NULLP;                  /* Utra FDD neighbor info */
   LwrNghUtraTddCfg   *nghUtraTddInfo = NULLP;                  /* Utra TDD neighbor info */
   LwrNghFreqCfg   *nghFreqCfg = NULLP;
   LwrNeighUtraFreq  *utraFreqCfg= NULLP;
   LwrNeighGERANFreq   *geranFreqCfg = NULLP; 
   LwrGERANNeighCell   *geranCellCfg = NULLP; 
   LwrCdma1xrttBndClsCfg *cdma1xBndClsCfg = NULLP;
   LwrCdma1xrttNeighFreqCfg *cdma1xNeighFreqCfg = NULLP;
   LwrCdma1xrttNeighCellCfg *cdma1xNeighCellCfg = NULLP;

   U32 plmnVal;
   U8  plmnLen;
   WrPlmnId plmnId;
   SztPLMNidentity pLMNidentity;
   U8 dscp = 0;
   U32 qciWgtVal = 0;
#ifndef WR_RSYS_OAM
   U32 enbId = 0;
   LwrSmMmeCfg *mmeCfg = NULLP;
   U8 ipIdx;
#endif
   U32 peerId = 0;
   /*ccpu00126694*/
   U8 nghPlmn[6],noMnc;
   /* CSG_DEV */
   U8   tagLen = 0;
   U8   pIdx = 1;

   TRC2(smWrReadConfigParams);

   if((fp = fopen("wr_cfg.txt","r")) == NULL)
   {
      /* configuration file checking */
      WR_LOGERROR(ERRCLS_DEBUG,0,ERRZERO,
            "FAILED to open the file wr_cfg.txt\n");
      RETVALUE (RFAILED);
   } /* end of if statement */

   /* Read the file line by line */
   for(;;)
   {
      cmMemset(tagVal, '\0', sizeof(tagVal));
      if (fscanf(fp,"%s %*s %s",tag,tagVal) != 2)
      {
         fprintf(stderr, " Error in while reading configuration file \n");
         RETVALUE(RFAILED);
      }
      if ((0 == strcmp("INVLD",(S8 *)tagVal)))
         continue;

      if(NULLP == strcmp("CFGEND", (S8 *)tag))
         break;

      smWrCfgGetTagNum(tag,&tagNum);
      switch(tagNum)
      {
         case WR_TAG_CELL_ID:
            {
               wrSmDfltCellId = atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_MOD_TYPE:
            {
               smCfgCb.modType = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_DUP_MODE:
            {
               smCfgCb.duplexMode = (U16)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_MAX_UE_SUPPORT:
            {
               smCfgCb.maxUeSupp = (U8)atoi((S8 *)tagVal);
               wrSmDfltMaxNoUE = smCfgCb.maxUeSupp;
            }
            break;   
         case WR_TAG_MCC_0:
            {
               smCfgCb.mcc[0].pres = TRUE;
               smCfgCb.mcc[0].val = (U8)atoi((S8 *)tagVal);
               wrSmDfltPlmnMcc[0][0] = smCfgCb.mcc[0].val;
            }
            break;   
         case WR_TAG_MCC_1:
            {
               smCfgCb.mcc[1].pres = TRUE;
               smCfgCb.mcc[1].val = (U8)atoi((S8 *)tagVal);
               wrSmDfltPlmnMcc[0][1] = smCfgCb.mcc[1].val;
            }
            break;   
         case WR_TAG_MCC_2:
            {
               smCfgCb.mcc[2].pres = TRUE;
               smCfgCb.mcc[2].val = (U8)atoi((S8 *)tagVal);
               wrSmDfltPlmnMcc[0][2] = smCfgCb.mcc[2].val;
            }
            break;   
         case WR_TAG_MNC_0:
            {
               smCfgCb.mnc[0].pres = TRUE;
               smCfgCb.mnc[0].val = (U8)atoi((S8 *)tagVal);
               wrSmDfltPlmnMnc[0][0] = smCfgCb.mnc[0].val;
            }
            break;   
         case WR_TAG_MNC_1:
            {
               smCfgCb.mnc[1].pres = TRUE;
               smCfgCb.mnc[1].val = (U8)atoi((S8 *)tagVal);
               wrSmDfltPlmnMnc[0][1] = smCfgCb.mnc[1].val;
            }
            break;   
         case WR_TAG_MNC_2:
            {
               if (-1 == atoi((S8 *)tagVal))
               {
                  smCfgCb.mnc[2].pres= FALSE;
                  smCfgCb.mnc[2].val = (U8)atoi((S8 *)tagVal);
                  wrSmDfltNumOfMncDigits[0] = 2;
               }
               else
               {
                  smCfgCb.mnc[2].pres= TRUE;
                  smCfgCb.mnc[2].val = (U8)atoi((S8 *)tagVal);
                  wrSmDfltNumOfMncDigits[0] = 3;
                  wrSmDfltPlmnMnc[0][2] = smCfgCb.mnc[2].val;
               }
            }
            break;   
         case WR_TAG_TA_CODE:
            {
               smCfgCb.trackAreaCode = (U16)atoi((S8 *)tagVal);
               wrSmDfltTac = smCfgCb.trackAreaCode;
            }
            break;   
         case WR_TAG_FREQ_BAND_IND:
            {
               wrSmDfltFreqBandInd = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_ENB_IP_ADDR:
            {
               wrUpdateIpAddr((U8 *)tagVal,&smCfgCb.enbIpAddr[WR_IPV4_IDX]);
               wrCpyCmTptAddr(&(wrSmDfltEnbIpAddr[WR_IPV4_IDX]), &smCfgCb.enbIpAddr[WR_IPV4_IDX]);
               wrCpyCmTptAddr(&smCfgCb.sctpIpAddr[WR_IPV4_IDX], &smCfgCb.enbIpAddr[WR_IPV4_IDX]);
            }
            break;  
         case WR_TAG_ENB_IP_ADDR_IPV6:
            {
               wrUpdateIpAddr((U8 *)tagVal,&smCfgCb.enbIpAddr[WR_IPV6_IDX]);
               wrCpyCmTptAddr(&wrSmDfltEnbIpAddr[WR_IPV6_IDX], &smCfgCb.enbIpAddr[WR_IPV6_IDX]);
               wrCpyCmTptAddr(&smCfgCb.sctpIpAddr[WR_IPV6_IDX], &smCfgCb.enbIpAddr[WR_IPV6_IDX]);
            }
            break;
         case WR_TAG_PCI_SELECT_TYPE :
            {
               wrSmPciSelType = (Bool) atoi((S8 *)tagVal);
               if(wrSmPciSelType > 2)
               {
                  fprintf(stderr, " Invalid PCI selection type configured \n");
                  RETVALUE(RFAILED);
               }
            }
            break;
            
         case WR_TAG_MAX_AVG_GBRPRB_USAGE:
            {
               smCfgCb.maxAvgGbrPrbUsage = (U8)atoi((S8 *)tagVal);  
            }
            break;
         case WR_TAG_PRACH_SELECT_TYPE :
            {
               wrSmPrachSelType = (Bool) atoi((S8 *)tagVal);  
               if(wrSmPrachSelType > 2)
               {
                  fprintf(stderr, " Invalid PRACH selection type configured \n");
                  RETVALUE(RFAILED);
               }
            }
            break;
         case WR_TAG_EARFCN_SELECT_TYPE :
            {
               wrSmEarfcnSelType = (Bool) atoi((S8 *)tagVal);
               if(wrSmEarfcnSelType > 2)
               {
                  fprintf(stderr, " Invalid EARFCN selection type configured \n");
                  RETVALUE(RFAILED);
               }
            }
            break;
         case WR_TAG_PCI_LIST :
            {
               status     = TRUE;
               noOfParams = 0;
               noOfElem   = 0;
               
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfParams > WR_SM_MAX_PCI)
                  {
                     noOfParams = WR_SM_MAX_PCI;
                  }
                  smCfgCb.numPci = noOfParams;
                  for (loopCnt=0; loopCnt<noOfParams; loopCnt++)
                  {
                     smCfgCb.pciList[loopCnt] = atoi((S8*)paramLst[loopCnt]);
                  }
               }
            }
            break;
         case WR_TAG_DL_EARFCN_LIST :
            {
               status     = TRUE;
               noOfParams = 0;
               noOfElem   = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfParams > WR_SM_MAX_EARFCN)
                  {
                     noOfParams = WR_SM_MAX_EARFCN;
                  }
                  smCfgCb.numDlEarfcn = noOfParams;
                  for (loopCnt=0; loopCnt < noOfParams; loopCnt++)
                  {
                     smCfgCb.dlEarfcnList[loopCnt] = atoi((S8*)paramLst[loopCnt]);
                  }
               }
            }
            break;
         case WR_TAG_UL_EARFCN_LIST :
            {
               status     = TRUE;
               noOfParams = 0;
               noOfElem   = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfParams > WR_SM_MAX_EARFCN)
                  {
                     noOfParams = WR_SM_MAX_EARFCN;
                  }
                  smCfgCb.numUlEarfcn = noOfParams;
                  for (loopCnt=0; loopCnt < noOfParams; loopCnt++)
                  {
                     smCfgCb.ulEarfcnList[loopCnt] = atoi((S8*)paramLst[loopCnt]);
                  }
               }
            }
            break;
         case WR_TAG_ROOTSEQ_IDX_LIST:
            {
               status     = TRUE;
               noOfParams = 0;
               noOfElem   = 0;
               U16  rootSeqIdx = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfParams > WR_SM_MAX_ROOTSEQ_IDX)
                  {
                     noOfParams = WR_SM_MAX_ROOTSEQ_IDX;
                  }
                  smCfgCb.sonPrachCfg.numRootSeqIndex = noOfParams;
                  for (loopCnt=0; loopCnt < noOfParams; loopCnt++)
                  {
                     rootSeqIdx = atoi((S8*)paramLst[loopCnt]);
                     smCfgCb.sonPrachCfg.rootSeqIdxLst[loopCnt]
                                   = rootSeqIdx;
                  }
               }
            }
            break;
         case WR_TAG_PRACHCFG_IDX_LIST:
            {
               status     = TRUE;
               noOfParams = 0;
               noOfElem   = 0;
               U16 prachCfgIdx = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfParams > WR_SM_MAX_PRACHCFG_IDX)
                  {
                     noOfParams = WR_SM_MAX_PRACHCFG_IDX;
                  }
                  smCfgCb.sonPrachCfg.numPrachCfgIdx = noOfParams;

                  for (loopCnt=0; loopCnt < noOfParams; loopCnt++)
                  {
                     prachCfgIdx = atoi((S8*)paramLst[loopCnt]);
                     smCfgCb.sonPrachCfg.prachCfgIdxLst[loopCnt] =
                        prachCfgIdx;
                  }
               }
            }
            break;
         case WR_TAG_ZERO_CORR_ZONE_CFG_LIST:
            {
               status     = TRUE;
               noOfParams = 0;
               noOfElem   = 0;
               U16 zeroCorrZoneCfg = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfParams > WR_SM_MAX_ZEROCORRZONECFG_IDX)
                  {
                     noOfParams = WR_SM_MAX_ZEROCORRZONECFG_IDX;
                  }
                  smCfgCb.sonPrachCfg.numZeroCorrCfgZone =
                                 noOfParams;

                  for (loopCnt=0; loopCnt < noOfParams; loopCnt++)
                  {
                     zeroCorrZoneCfg = atoi((S8*)paramLst[loopCnt]);
                     smCfgCb.sonPrachCfg.zeroCorrZoneCfgLst[loopCnt]
                                   = zeroCorrZoneCfg;
                  }
               }
            }
            break;
         case WR_TAG_PRACH_FREQ_OFFSET_LIST:
            {
               status     = TRUE;
               noOfParams = 0;
               noOfElem   = 0;
               U16 prachFreqOffset = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfParams > WR_SM_MAX_PRACHFREQOFFSET_IDX)
                  {
                     noOfParams = WR_SM_MAX_PRACHFREQOFFSET_IDX;
                  }
                  smCfgCb.sonPrachCfg.numPrachFreqOffset = noOfParams;

                  for (loopCnt=0; loopCnt < noOfParams; loopCnt++)
                  {
                     prachFreqOffset = atoi((S8*)paramLst[loopCnt]);
                     smCfgCb.sonPrachCfg.freqOffsetLst[loopCnt] = prachFreqOffset;
                  }
               }
            }
            break;
         case WR_TAG_ROOTSEQ_IDX:
            {
               smCfgCb.sonPrachCfg.servRootSeqIdx = (U16)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_PRACHCFG_IDX:
            {
               smCfgCb.sonPrachCfg.servPrachCfgIdx = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_ZERO_CORR_ZONE_CFG:
            {
               smCfgCb.sonPrachCfg.servZeroCorrelationZoneCfg = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_PRACH_FREQ_OFFSET:
            {
               smCfgCb.sonPrachCfg.servPrachFreqOffset = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_NO_OF_MME_INFO:
         case WR_TAG_NO_OF_MME_INFO_IPV6:
            {
               smCfgCb.noOfCfg = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_MME_INFO:
         case WR_TAG_MME_INFO_IPV6:
            {
#ifndef WR_RSYS_OAM
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfElem != smCfgCb.noOfCfg)
                  {
                     fprintf(stderr, " Number of parameters in WR_TAG_MME_INFO mismatch \n");
                     RETVALUE(RFAILED);
                  }
                  paramCnt = 0;
                  for(loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     WR_ALLOC(&mmeCfg, sizeof(LwrMmeCfg));
                     mmeCfg->mmeId = (U16)atoi((S8 *)paramLst[paramCnt++]);
                     mmeCfg->noOfIps = (U8)atoi((S8 *)paramLst[paramCnt++]);
                     if (mmeCfg->noOfIps > LWR_MAX_IP_PER_MME)
                     {
                        mmeCfg->noOfIps = LWR_MAX_IP_PER_MME;
                     }
                     for(ipIdx = 0; ipIdx < mmeCfg->noOfIps; ipIdx++)  
                     {
                        wrUpdateIpAddr((U8 *)paramLst[paramCnt++], 
                                   &(mmeCfg->mmeAddr[ipIdx]));
                        /* Update the mme ip address type */
                        /*wrSmDfltMmeIPType = mmeCfg->mmeAddr[ipIdx].type;*/
                     }
                     smCfgCb.mmeCfg[loopCnt] = mmeCfg;
                  }
               }
               else
               {
                  fprintf(stderr, " Error in WR_TAG_MME_INFO tag format ");
                  RETVALUE(RFAILED);
               }
#else
#endif
            }
            break;   
         case WR_TAG_HI_DBG:
            {
               smCfgCb.hiDbg = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_SB_DBG:
            {
               smCfgCb.sbDbg = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_SZ_DBG:
            {
               smCfgCb.szDbg = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_EG_DBG:
            {
               smCfgCb.egDbg = (U8)atoi((S8 *)tagVal);               
            }
            break;   
         case WR_TAG_WR_DBG:
            {
               smCfgCb.wrDbg  = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_NH_DBG:
            {
               smCfgCb.nhDbg = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_KW_DBG:
            {
               smCfgCb.kwDbg = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_RG_DBG:
            {
               smCfgCb.rgDbg = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_YS_DBG:
            {
               smCfgCb.ysDbg = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_SM_DBG:
            {
               smCfgCb.smDbg = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_INACTIVITY_TIMER_VAL:
            {
               smCfgCb.inactvTmrVal = atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_END_MARKER_TIMER_VAL:
            {
               smCfgCb.endMrkTmrVal = atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_MAX_EXPIRY:
            {
               smCfgCb.maxExpires = atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_CZ_DBG:
            {
               smCfgCb.czDbg = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_X2_PREP_TMR:
            {
               smCfgCb.x2PrepTimerVal = atoi((S8 *)tagVal); 
            }
            break;
         case WR_TAG_X2_OVRALL_TMR:
            {
               smCfgCb.x2OvrAllTimerVal = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_UE_HO_ATTACH_TMR:
            {
               smCfgCb.ueHoAttachTimerVal = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_X2_TIME_TO_WAIT:
            {
               smCfgCb.x2TimeToWaitTimerVal = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_ENB_NAME:
            {
               strcpy((S8 *)smCfgCb.enbName, (S8 *)tagVal); 
            }
            break;
         case WR_TAG_NO_OF_BRDCST_PLMN:
            {
               smCfgCb.noOfBrdCstPlmn = (U8)atoi((S8 *)tagVal);
               wrSmDfltNumPlmn = smCfgCb.noOfBrdCstPlmn;
            }
            break;
         case WR_TAG_PLMN_ID_LST:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfParams; loopCnt++)
                  {
                     strcpy((S8 *)smCfgCb.plmnId[loopCnt], (S8 *)paramLst[loopCnt]);

                     plmnVal = atoi((S8*)&paramLst[loopCnt][0]);
                     plmnLen = strlen((S8 *)paramLst[loopCnt]);

                     if(plmnLen == 5)
                     {
                        smCfgCb.plmnId[loopCnt][0] = (((plmnVal/1000)%10)<<4)|((plmnVal/10000)%10);
                        smCfgCb.plmnId[loopCnt][1] = 0xF0|((plmnVal/100)%10);
                        smCfgCb.plmnId[loopCnt][2] = ((plmnVal%10)<<4)|((plmnVal/10)%10);
                     }
                     else if(plmnLen == 6)
                     {
                        smCfgCb.plmnId[loopCnt][0] = (((plmnVal/10000)%10)<<4)|((plmnVal/100000)%10);
                        smCfgCb.plmnId[loopCnt][1] = (((plmnVal/100)%10)<<4)|((plmnVal/1000)%10);
                        smCfgCb.plmnId[loopCnt][2] = ((plmnVal%10)<<4)|((plmnVal/10)%10);
                     }

                     pLMNidentity.val = &smCfgCb.plmnId[loopCnt][0];
                     wrUtlGetPlmnId(&plmnId, &pLMNidentity);

                     wrSmDfltNumOfMncDigits[pIdx] = plmnId.numMncDigits;
                     wrSmDfltPlmnMcc[pIdx][0] = plmnId.mcc[0];
                     wrSmDfltPlmnMcc[pIdx][1] = plmnId.mcc[1];
                     wrSmDfltPlmnMcc[pIdx][2] = plmnId.mcc[2];
                     wrSmDfltPlmnMnc[pIdx][0] = plmnId.mnc[0];
                     wrSmDfltPlmnMnc[pIdx][1] = plmnId.mnc[1];
                     wrSmDfltPlmnMnc[pIdx][2] = plmnId.mnc[2];
                     pIdx++;

                  }
               }
               else
               {
                  fprintf(stderr, " Error in WR_TAG_PLMN_ID_LST tag format \n");
               }
            }
            break;
         case WR_TAG_NO_OF_UTRA_FDD_NGH_CFG:
            {
               wrSmDfltNumofUtraFddNrCell = (U8)atoi((S8 *)tagVal);
               if( 0 == smCfgCb.noOfUtraFddFreq)
               {
                  wrSmDfltNumofUtraFddNrCell = 0;
               }
               if(wrSmDfltNumofUtraFddNrCell > MAX_NBR_UTRA_CELL)
               {
                  fprintf(stderr, " Number of UTRA FDD cell exceeds MAX_NBR_UTRA_CELL \n");
               }
            }
            break;           
         case WR_TAG_NGH_UTRA_FDD_CELL_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               if(0 == smCfgCb.noOfUtraFddFreq)
               {
                  fprintf(stderr, "AS the number of UTRA FDD frequency is zero \
                        no need to read the list");
               }
               else
               {
                  wrCfgValdtInput(tagVal, &status, &noOfElem);
                  if (TRUE == status)
                  {
                     wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                     if(noOfElem != wrSmDfltNumofUtraFddNrCell)
                     {
                        fprintf(stderr, "Number of parameters in WR_TAG_NO_OF_UTRA_FDD_NGH_CFG mismatch \n");
                        RETVALUE(RFAILED);
                     }
                     paramCnt = 0;
                     for (loopCnt = 0; loopCnt < wrSmDfltNumofUtraFddNrCell; loopCnt++)
                     {
                        WR_ALLOC(&nghUtraFddInfo, sizeof(LwrNghUtraFddCfg));
                        nghUtraFddInfo->rncId = (U16)atoi((S8 *)paramLst[paramCnt++]);
                        nghUtraFddInfo->cId = (U16)atoi((S8 *)paramLst[paramCnt++]);   
                        /* If extended RNC Id is used 
                         * then CID should be 12 bits */ 
                        if( nghUtraFddInfo->rncId >= WR_SM_START_EXT_RNC)
                        {
                           if( nghUtraFddInfo->cId >= WR_SM_START_EXT_RNC)
                           {
                              WR_LOGERROR(ERRCLS_DEBUG,0,ERRZERO, 
                                    "Invalid CID configured.RNC Id is 16 bits, \
                                    so CID should be 12 bits \n");
                              RETVALUE(RFAILED);
                           }
                        }

                        nghUtraFddInfo->arfcnDl = (U16)atoi((S8 *)paramLst[paramCnt++]);
                        nghUtraFddInfo->arfcnUl = (U16)atoi((S8 *)paramLst[paramCnt++]);
                        nghUtraFddInfo->psc =  atoi((S8*)paramLst[paramCnt++]);
                        /* Removed one of the comparision to fix warning */
                        /*if(( nghUtraFddInfo->psc < WR_SM_MIN_PSC_VAL) || 
                          ( nghUtraFddInfo->psc > WR_SM_MAX_PSC_VAL))*/
                        if(nghUtraFddInfo->psc > WR_SM_MAX_PSC_VAL)
                        {
                           WR_LOGERROR(ERRCLS_DEBUG,0,ERRZERO, "Invalid PSC configured \n");
                           RETVALUE(RFAILED);
                        }
                        wrSmDfltExtUtraCell[loopCnt].numPlmns = atoi((S8*)paramLst[paramCnt++]);
                        plmnVal = atoi((S8*)paramLst[paramCnt++]);
                        if(wrGetNeghPlmn(plmnVal, nghPlmn, &noMnc) == RFAILED)
                        {
                           fprintf(stderr, "Plmn format is wrong");
                           RETVALUE(RFAILED);
                        }
                        wrSmDfltExtUtraCell[loopCnt].plmnIds[0].numMncDigits = noMnc;
                        wrSmDfltExtUtraCell[loopCnt].plmnIds[0].mcc[0] = nghPlmn[0];
                        wrSmDfltExtUtraCell[loopCnt].plmnIds[0].mcc[1] = nghPlmn[1];
                        wrSmDfltExtUtraCell[loopCnt].plmnIds[0].mcc[2] = nghPlmn[2];
                        wrSmDfltExtUtraCell[loopCnt].plmnIds[0].mnc[0] = nghPlmn[3];
                        wrSmDfltExtUtraCell[loopCnt].plmnIds[0].mnc[1] = nghPlmn[4];
                        wrSmDfltExtUtraCell[loopCnt].plmnIds[0].mnc[2] = nghPlmn[5];
                        wrSmDfltExtUtraCell[loopCnt].duplexType = WR_UTRA_DUPLEX_FDD; 
                        nghUtraFddInfo->lac =  atoi((S8*)paramLst[paramCnt++]);
                        nghUtraFddInfo->rac =  atoi((S8*)paramLst[paramCnt++]); 
                        nghUtraFddInfo->isRimSupported =  
                           atoi((S8*)paramLst[paramCnt++]); 
                        nghUtraFddInfo->voipCapable = 
                           atoi((S8*)paramLst[paramCnt++]); 
                        nghUtraFddInfo->psHOCapable = 
                           atoi((S8*)paramLst[paramCnt++]); 
                        smCfgCb.nghUtraFddInfo[loopCnt] = nghUtraFddInfo;

                     }
                  }
                  else
                  {
                     fprintf(stderr, "Error in WR_TAG_NGH_UTRA_FDD_CELL_CFG tag format \n");
                  }
               }
            }
            break;
         case WR_TAG_NO_OF_UTRA_TDD_NGH_CFG:
            {
               wrSmDfltNumofUtraTddNrCell = (U8)atoi((S8 *)tagVal);
               if( 0 == smCfgCb.noOfUtraTddFreq)
               {
                  wrSmDfltNumofUtraTddNrCell = 0;
               }
               if(wrSmDfltNumofUtraTddNrCell > MAX_NBR_UTRA_CELL)
               {
                  fprintf(stderr, " Number of UTRA TDD cell exceeds MAX_NBR_UTRA_CELL \n");
               }
            }
            break;
         case WR_TAG_NGH_UTRA_TDD_CELL_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               if(0 == smCfgCb.noOfUtraTddFreq)
               {
                  fprintf(stderr, "AS the number of UTRA frequency is zero \
                        no need to read the list");
               }
               else
               {
                  wrCfgValdtInput(tagVal, &status, &noOfElem);
                  if (TRUE == status)
                  {
                     wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                     if(noOfElem != wrSmDfltNumofUtraTddNrCell)
                     {
                        fprintf(stderr, "Number of parameters in WR_TAG_NGH_UTRA_TDD_CELL_CFG mismatch \n");
                        RETVALUE(RFAILED);
                     }
                     paramCnt = 0;
                     for (loopCnt = 0; loopCnt < wrSmDfltNumofUtraTddNrCell; loopCnt++)
                     {
                        WR_ALLOC(&nghUtraTddInfo, sizeof(LwrNghUtraTddCfg));
                        nghUtraTddInfo->rncId = (U16)atoi((S8 *)paramLst[paramCnt++]);
                        nghUtraTddInfo->cId = (U16)atoi((S8 *)paramLst[paramCnt++]);   
                        /* If extended RNC Id is used 
                         * then CID should be 12 bits */ 
                        if( nghUtraTddInfo->rncId >= WR_SM_START_EXT_RNC)
                        {
                           if( nghUtraTddInfo->cId >= WR_SM_START_EXT_RNC)
                           {
                              WR_LOGERROR(ERRCLS_DEBUG,0,ERRZERO, 
                                    "Invalid CID configured.RNC Id is 16 bits, \
                                    so CID should be 12 bits \n");
                              RETVALUE(RFAILED);
                           }
                        }

                        nghUtraTddInfo->arfcn = (U16)atoi((S8 *)paramLst[paramCnt++]);
                        nghUtraTddInfo->lac =  atoi((S8*)paramLst[paramCnt++]);
                        nghUtraTddInfo->ccpchTPwr =  atoi((S8*)paramLst[paramCnt++]);
                        nghUtraTddInfo->cellParamId =  atoi((S8*)paramLst[paramCnt++]);
                        if(nghUtraTddInfo->cellParamId > WR_SM_MAX_CELL_PARAM_VAL)
                        {
                           WR_LOGERROR(ERRCLS_DEBUG,0,ERRZERO, "Invalid Cell Param Id configured \n");
                           RETVALUE(RFAILED);
                        }
                        wrSmDfltExtUtraTddCell[loopCnt].numPlmns = atoi((S8*)paramLst[paramCnt++]);
                        plmnVal = atoi((S8*)paramLst[paramCnt++]);
                        if(wrGetNeghPlmn(plmnVal, nghPlmn, &noMnc) == RFAILED)
                        {
                           fprintf(stderr, "Plmn format is wrong");
                           RETVALUE(RFAILED);
                        }
                        wrSmDfltExtUtraTddCell[loopCnt].plmnIds[0].numMncDigits = noMnc;
                        wrSmDfltExtUtraTddCell[loopCnt].plmnIds[0].mcc[0] = nghPlmn[0];
                        wrSmDfltExtUtraTddCell[loopCnt].plmnIds[0].mcc[1] = nghPlmn[1];
                        wrSmDfltExtUtraTddCell[loopCnt].plmnIds[0].mcc[2] = nghPlmn[2];
                        wrSmDfltExtUtraTddCell[loopCnt].plmnIds[0].mnc[0] = nghPlmn[3];
                        wrSmDfltExtUtraTddCell[loopCnt].plmnIds[0].mnc[1] = nghPlmn[4];
                        wrSmDfltExtUtraTddCell[loopCnt].plmnIds[0].mnc[2] = nghPlmn[5];
                        wrSmDfltExtUtraTddCell[loopCnt].duplexType = WR_UTRA_DUPLEX_TDD;
                        nghUtraTddInfo->rac =  atoi((S8*)paramLst[paramCnt++]); 
                        nghUtraTddInfo->isRimSupported =  
                           atoi((S8*)paramLst[paramCnt++]); 
                        smCfgCb.nghUtraTddInfo[loopCnt] = nghUtraTddInfo;
                     }
                  }
                  else
                  {
                     fprintf(stderr, "Error in WR_TAG_NGH_UTRA_CELL_CFG tag format \n");
                  }
               }
            }
            break;
         case WR_TAG_NUM_EUTRA_FREQ:
            {
               smCfgCb.noOfEutraFreq = (U16)atoi((S8 *)tagVal);
               /* Fix for the CR ccpu00127482  */
               wrSmDfltNumNrFreq =  smCfgCb.noOfEutraFreq; 
            }
            break;
         case WR_TAG_EUTRA_FREQ_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfElem != smCfgCb.noOfEutraFreq)
                  {
                     fprintf(stderr, "Number of parameters in WR_TAG_EUTRA_FREQ_CFG mismatch \n");
                     RETVALUE(RFAILED);
                  }
                  paramCnt = 0;
                  for (loopCnt = 0; loopCnt < smCfgCb.noOfEutraFreq; loopCnt++)
                  {
                     WR_ALLOC(&nghFreqCfg, sizeof(LwrNghFreqCfg));
                     nghFreqCfg->arfcnDl = (U16)atoi((S8 *)paramLst[paramCnt++]);
                     nghFreqCfg->arfcnUl = (U16)atoi((S8 *)paramLst[paramCnt++]);
                     smCfgCb.nghEutraFreq[loopCnt] = nghFreqCfg;

                  }
               }
               else
               {
                  fprintf(stderr, "Error in WR_TAG_EUTRA_FREQ_CFG tag format \n");
               }
            }
            break;
         case WR_TAG_NUM_UTRA_FDD_FREQ:
            {
               smCfgCb.noOfUtraFddFreq = (U16)atoi((S8 *)tagVal);
               if (smCfgCb.noOfUtraFddFreq > MAX_NBR_UTRA_FREQ)
               {
                  fprintf(stderr, " smCfgCb.noOfUtraFddFreq(%d) greater than MAX_NBR_UTRA_FREQ(%d)\n", 
                        smCfgCb.noOfUtraFddFreq, MAX_NBR_UTRA_FREQ);
                  RETVALUE(RFAILED);
               }
            }
            break;
         case WR_TAG_UTRA_FDD_FREQ_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfElem != smCfgCb.noOfUtraFddFreq)
                  {
                     fprintf(stderr, " Number of parameters in WR_TAG_UTRA_FDD_FREQ_CFG mismatch \n");
                     RETVALUE(RFAILED);
                  }
                  paramCnt = 0;
                  for (loopCnt = 0; loopCnt < smCfgCb.noOfUtraFddFreq; loopCnt++)
                  {
                     WR_ALLOC(&utraFreqCfg, sizeof(LwrNeighUtraFreq));
                     utraFreqCfg->duplexMode = WR_LWR_UTRA_FDD_MODE;
                     utraFreqCfg->t.utraFdd.arfcnDl = (U16)atoi((S8 *)paramLst[paramCnt++]);
                     utraFreqCfg->t.utraFdd.arfcnUl = (U16)atoi((S8 *)paramLst[paramCnt++]);
                     smCfgCb.nghUtraFddFreq[loopCnt] = utraFreqCfg;
                  }
               }
               else
               {
                  fprintf(stderr, " Error in WR_TAG_UTRA_FDD_FREQ_CFG tag format \n");
               }
            }
            break;
         case WR_TAG_NUM_UTRA_TDD_FREQ:
            {
               smCfgCb.noOfUtraTddFreq = (U16)atoi((S8 *)tagVal);
               if (smCfgCb.noOfUtraTddFreq > MAX_NBR_UTRA_FREQ)
               {
                  fprintf(stderr, " smCfgCb.noOfUtraTddFreq(%d) greater than MAX_NBR_UTRA_FREQ(%d)\n", 
                        smCfgCb.noOfUtraTddFreq, MAX_NBR_UTRA_FREQ);
                  RETVALUE(RFAILED);
               }
            }
            break;
         case WR_TAG_UTRA_TDD_FREQ_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfElem != smCfgCb.noOfUtraTddFreq)
                  {
                     fprintf(stderr, " Number of parameters in WR_TAG_UTRA_TDD_FREQ_CFG mismatch \n");
                     RETVALUE(RFAILED);
                  }
                  paramCnt = 0;
                  for (loopCnt = 0; loopCnt < smCfgCb.noOfUtraTddFreq; loopCnt++)
                  {
                     WR_ALLOC(&utraFreqCfg, sizeof(LwrNeighUtraFreq));
                     utraFreqCfg->duplexMode = WR_LWR_UTRA_TDD_MODE;
                     utraFreqCfg->t.utraTdd.arfcn = (U16)atoi((S8 *)paramLst[paramCnt++]);
                     utraFreqCfg->t.utraTdd.tddMode = (U16)atoi((S8 *)paramLst[paramCnt++]);
                     utraFreqCfg->t.utraTdd.bandIndicator = (U8)atoi((S8 *)paramLst[paramCnt++]);
                     smCfgCb.nghUtraTddFreq[loopCnt] = utraFreqCfg;
                  }
               }
               else
               {
                  fprintf(stderr, " Error in WR_TAG_UTRA_TDD_FREQ_CFG tag format \n");
               }
            }
            break;
         case WR_TAG_NUM_GERAN_NEIGH_FREQ:
            {
               smCfgCb.noOfGeranFreq = (U16)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_GERAN_NEIGH_FREQ:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if((noOfElem != smCfgCb.noOfGeranFreq) ||
                     (LWR_MAX_NUM_NEIGH_GERAN_FREQS < noOfElem))
                  {
                     fprintf(stderr, " Number of parameters in WR_TAG_GERAN_NEIGH_FREQ mismatch \n");
                     RETVALUE(RFAILED);
                  }
                  paramCnt = 0;
                  for (loopCnt = 0; loopCnt < smCfgCb.noOfGeranFreq; loopCnt++)
                  {
                     WR_ALLOC(&geranFreqCfg, sizeof(LwrNeighGERANFreq));
                     geranFreqCfg->bandIndicator = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                     geranFreqCfg->bcchArfcn = (U16)atoi((S8 *)paramLst[paramCnt++]);
                     geranFreqCfg->cellReselePri = (U8)atoi((S8 *)paramLst[paramCnt++]);
                     geranFreqCfg->ncc_Permitted = (U8)atoi((S8 *)paramLst[paramCnt++]);
                     smCfgCb.geranFreq[loopCnt] = geranFreqCfg;

                  }
               }
               else
               {
                  fprintf(stderr, " Error in WR_TAG_GERAN_NEIGH_FREQ tag format \n");
               }
            }
         break;
         case WR_TAG_GERAN_NEIGH_CELL:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfElem > LWR_MAX_NUM_NEIGH_GERAN_CELLS)
                  {
                     fprintf(stderr, " Number of nighbors in WR_TAG_GERAN_NEIGH_CELL are exceeding 16 \n");
                     RETVALUE(RFAILED);
                  }
                  paramCnt = 0;
                  smCfgCb.noOfGeranCell = noOfElem;
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     WR_ALLOC(&geranCellCfg, sizeof(LwrGERANNeighCell));
                     plmnVal = (U32)atoi((S8 *)paramLst[paramCnt++]);
                     if(wrGetNeghPlmn(plmnVal, nghPlmn, &noMnc) == RFAILED)
                     {
                        fprintf(stderr, " Plmn format is wrong \n");
                        RETVALUE(RFAILED);
                     }
                     geranCellCfg->plmnId.numMncDigits = noMnc;
                     geranCellCfg->plmnId.mcc[0] = nghPlmn[0];
                     geranCellCfg->plmnId.mcc[1] = nghPlmn[1];
                     geranCellCfg->plmnId.mcc[2] = nghPlmn[2];
                     geranCellCfg->plmnId.mnc[0] = nghPlmn[3];
                     geranCellCfg->plmnId.mnc[1] = nghPlmn[4];
                     if(noMnc == 3)
                     {
                        geranCellCfg->plmnId.mnc[2] = nghPlmn[5];
                     }

                     geranCellCfg->lac = (U16)atoi((S8 *)paramLst[paramCnt++]);
                     geranCellCfg->rac = (U8)atoi((S8 *)paramLst[paramCnt++]);
                     geranCellCfg->bsic = (U8)atoi((S8 *)paramLst[paramCnt++]);
                     geranCellCfg->ci = (U16)atoi((S8 *)paramLst[paramCnt++]);
                     geranCellCfg->bandIndicator = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                     geranCellCfg->bcchArfcn = (U16)atoi((S8 *)paramLst[paramCnt++]);
                     geranCellCfg->nccPermittedMeas = (U8)atoi((S8 *)paramLst[paramCnt++]);
                     geranCellCfg->isDTMCapable = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                     geranCellCfg->rimSupportForGeranNgh = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                     /* not reading nco from wr_cfg.txt but by default setting the value to 0
                      * and presence set to true*/
                     geranCellCfg->ncoPres = PRSNT_NODEF; 
                     geranCellCfg->ncoVal  = 0;
                     smCfgCb.geranCell[loopCnt] = geranCellCfg;

                  }
               }
               else
               {
                  fprintf(stderr, " Error in WR_TAG_GERAN_NEIGH_CELL tag format \n");
               }
            }
         break;
         case WR_TAG_GERAN_MEAS_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  paramCnt = 0;
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  wrSmDfltGeranRptInterval = atoi((S8 *)paramLst[paramCnt++]);
                  wrSmDfltisGERANMeasAllowedForCCO = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                  wrSmDfltGeranisCCOAllowed = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                  wrSmDfltGERANMeasWaitTmr = (U32)atoi((S8 *)paramLst[paramCnt++]);
                  wrSmDfltGeranT304expWaitTmrVal = (U32)atoi((S8 *)paramLst[paramCnt++]);
                  wrSmDfltGeranrrcDatCfmWaitTmr = (U32)atoi((S8 *)paramLst[paramCnt++]);
               }
               else
               {
                  ALARM("Wrong WR_TAG_MEAS_GERAN tag format\n");
               }
            }
         break;
         case WR_TAG_NUM_CDMA_1XRTT_BAND_CLS:
            {
               smCfgCb.noOfCdma1xBndCls = atoi((S8*)tagVal);
            }
            break;

         case WR_TAG_CDMA_1XRTT_BAND_CLS:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  if(noOfElem != smCfgCb.noOfCdma1xBndCls)
                  {
                     fprintf(stderr, "Number of parameters in WR_TAG_CDMA_1XRTT_BAND_CLS mismatch");
                     RETVALUE(RFAILED);
                  }
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  paramCnt = 0;
                  for (loopCnt = 0; loopCnt < smCfgCb.noOfCdma1xBndCls; loopCnt++)
                  {
                     WR_ALLOC(&cdma1xBndClsCfg,sizeof(LwrCdma1xrttBndClsCfg));
                     if (cdma1xBndClsCfg == NULLP)
                     {
                        fprintf(stderr, "Memory not allocated for CDMA band class");
                        RETVALUE(RFAILED);
                     }
                     cdma1xBndClsCfg->bandClass = 
                        atoi((S8*)paramLst[paramCnt++]);
                     cdma1xBndClsCfg->cellReselectPrior = 
                        atoi((S8*)paramLst[paramCnt++]);
                     cdma1xBndClsCfg->threshXHigh = 
                        atoi((S8*)paramLst[paramCnt++]);
                     cdma1xBndClsCfg->threshXLow = 
                        atoi((S8*)paramLst[paramCnt++]);
                     smCfgCb.cdma1xBndClsCfg[loopCnt] = cdma1xBndClsCfg;
                  }
               }
               else
               {
                  fprintf(stderr, "Error in WR_TAG_CDMA_1XRTT_BAND_CLS tag format");
               }
            }
            break;
         case WR_TAG_NUM_CDMA_1XRTT_NEIGH_FREQ:
            {
               smCfgCb.noOfNeighCdma1xFreq = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_CDMA_1XRTT_NEIGH_FREQ:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  if(noOfElem != smCfgCb.noOfNeighCdma1xFreq)
                  {
                     fprintf(stderr, "Number of parameters in WR_TAG_CDMA_1XRTT_NEIGH_FREQ mismatch");
                     RETVALUE(RFAILED);
                  }
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  paramCnt = 0;
                  for (loopCnt = 0; loopCnt < smCfgCb.noOfNeighCdma1xFreq; loopCnt++)
                  {
                     WR_ALLOC(&cdma1xNeighFreqCfg, sizeof(LwrCdma1xrttNeighFreqCfg));
                     if (cdma1xNeighFreqCfg == NULLP)
                     {
                        fprintf(stderr, "Memory not allocated for CDMA freq");
                        RETVALUE(RFAILED);
                     }
                     cdma1xNeighFreqCfg->bandClass = 
                        atoi((S8*)paramLst[paramCnt++]);
                     cdma1xNeighFreqCfg->arfcn = 
                        atoi((S8*)paramLst[paramCnt++]);
                     cdma1xNeighFreqCfg->freqPriority = 
                        atoi((S8*)paramLst[paramCnt++]);
                     smCfgCb.neighCdma1xFreq[loopCnt] = cdma1xNeighFreqCfg;
                  }
               }
               else
               {
                  fprintf(stderr, "Error in WR_TAG_CDMA_1XRTT_BAND_CLS tag format");
               }
            }
            break;
         case WR_TAG_NUM_CDMA_1XRTT_NEIGH_CELL:
            {
               smCfgCb.noOfNeighCdma1xCells = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_CDMA_1XRTT_NEIGH_CELL:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  if(noOfElem != smCfgCb.noOfNeighCdma1xCells)
                  {
                     fprintf(stderr, "Number of parameters in WR_TAG_CDMA_1XRTT_NEIGH_CELL mismatch");
                     RETVALUE(RFAILED);
                  }
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  paramCnt = 0;
                  for (loopCnt = 0; loopCnt < smCfgCb.noOfNeighCdma1xCells; loopCnt++)
                  {
                     WR_ALLOC(&cdma1xNeighCellCfg, sizeof(LwrCdma1xrttNeighCellCfg));
                     if (cdma1xNeighCellCfg == NULLP)
                     {
                        fprintf(stderr, "Memory not allocated for CDMA cell");
                        RETVALUE(RFAILED);
                     }
                     cdma1xNeighCellCfg->bandClass = 
                        atoi((S8*)paramLst[paramCnt++]);
                     cdma1xNeighCellCfg->arfcn = 
                        atoi((S8*)paramLst[paramCnt++]);
                     cdma1xNeighCellCfg->pnOffset = 
                        atoi((S8*)paramLst[paramCnt++]);
                     cdma1xNeighCellCfg->curRank = 
                        atoi((S8*)paramLst[paramCnt++]);
                     cdma1xNeighCellCfg->neighCellFlag = 
                        atoi((S8*)paramLst[paramCnt++]);
                     smCfgCb.neighCdma1xCell[loopCnt] = cdma1xNeighCellCfg;
                  }
               }
               else
               {
                  fprintf(stderr, "Error in WR_TAG_CDMA_1XRTT_BAND_CLS tag format");
               }
            }
            break;
         case WR_TAG_CSFB_UTRA_CFG_VAL:
            {
               /*0 withoutMeas, 1 withMeas */
               wrSmDfltCsfbToUtraMeasSup = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_CSFB_CDMA_CFG_VAL:
            {
               /*0 withoutMeas, 1 withMeas */
               wrSmDfltCsfbToCdmaMeasSup = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_ECSFB_CFG_VAL:
            {
               /* 0 notSuporrted, 1 withoutMeas, 2 withMeas */
               wrSmDfltEcsfbMeasSup = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_DUAL_RX_SUPPORTED:
            {
               wrSmDfltDualRxSup = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_NO_OF_NGH_INFO:
            {
               smCfgCb.noOfNghInfo = (U16)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_NGH_INFO_CFG:
            {
            }
            break;
         case WR_TAG_NO_OF_GU_GRP:
            {
               smCfgCb.noOfguGrp = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_GU_GRP_ID_CFG:
            {
            }
            break;
         case WR_TAG_S1_PREP_TMR:
            {
               smCfgCb.s1PrepTimerVal = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_S1_OVRALL_TMR:
            {
               smCfgCb.s1OvrAllTimerVal = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_SRC_HO_CANCEL_TMR:
            {
               smCfgCb.s1CancelTimerVal = atoi((S8*)tagVal);
               if((smCfgCb.s1CancelTimerVal < 400) || (smCfgCb.s1CancelTimerVal > 1000))
               {
                  smCfgCb.s1CancelTimerVal = wrSmDflts1HandoverCancelTmr;
               } 
            }
            break;
         case WR_TAG_NO_OF_NGH_CFG:
         case WR_TAG_NO_OF_NGH_CFG_IPV6:
            {
               smCfgCb.noOfNghCfg = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_NGH_CELL_CFG:
         case WR_TAG_NGH_CELL_CFG_IPV6:
            {
               status = TRUE;
               U8    tmpPlmnId[LWR_SM_MAX_PLMN_IDS][3];
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);

                  for (loopCnt = 0; loopCnt < WR_SMM_MAX_NBR_ENB; loopCnt++)
                  {
                     smCfgCb.wrNghCellCfg[loopCnt] = NULLP;
                  }

                  paramCnt = 0;
                  loopCnt = 0;
                  for (loopCnt = 0; loopCnt < smCfgCb.noOfNghCfg; loopCnt++)
                  {
                     peerId = 0;
                     WR_ALLOC(&nghCellCfg, sizeof(LwrNghCellCfg));
                     if (NULLP != nghCellCfg)
                     {
                        wrUpdateIpAddr((U8 *)paramLst[paramCnt++],&(nghCellCfg->enbAddr));
                        /* Update neighboring enodeb ip address type */
                        /*wrSmDfltNeEnbIPType = nghCellCfg->enbAddr.type;*/
                        nghCellCfg->physCellId = (U16)atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->tac = (U16)atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->enbId   = atoi((S8 *)paramLst[paramCnt++]);
                        plmnLen = strlen((S8 *)paramLst[paramCnt]);
                        nghCellCfg->plmnId = atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->enbType = smGetEnbType(nghCellCfg->physCellId);
                        printf("CNM:nghCellCfg->plmnId:%lu\n",nghCellCfg->plmnId);
                        #if 0
                        /*ccpu00126694*/
                        if(wrGetNeghPlmn(nghCellCfg->plmnId, nghPlmn, &noMnc) == RFAILED)
                        {
                           fprintf(stderr, " Plmn format is wrong \n");
                           RETVALUE(RFAILED);
                        }
                        #endif
                        if(plmnLen == 5)
                        {
                           tmpPlmnId[loopCnt][0] = (((nghCellCfg->plmnId/1000)%10)<<4)|((nghCellCfg->plmnId/10000)%10);
                           tmpPlmnId[loopCnt][1] = 0xF0|((nghCellCfg->plmnId/100)%10);
                           tmpPlmnId[loopCnt][2] = ((nghCellCfg->plmnId%10)<<4)|((nghCellCfg->plmnId/10)%10);

                        }
                        else if(plmnLen == 6)
                        {
                           tmpPlmnId[loopCnt][0] = (((nghCellCfg->plmnId/10000)%10)<<4)|((nghCellCfg->plmnId/100000)%10);
                           tmpPlmnId[loopCnt][1] = (((nghCellCfg->plmnId/100)%10)<<4)|((nghCellCfg->plmnId/1000)%10);
                           tmpPlmnId[loopCnt][2] = ((nghCellCfg->plmnId%10)<<4)|((nghCellCfg->plmnId/10)%10);

                        }
                        else
                        {
                           fprintf(stderr, " Plmn format is wrong \n");
                           RETVALUE(RFAILED);
                        }
                        pLMNidentity.val = &tmpPlmnId[loopCnt][0];
                        wrUtlGetPlmnId(&plmnId, &pLMNidentity);
                        wrSmDfltNeEnbNumMncDigits = nghCellCfg->noMnc = plmnId.numMncDigits;
                        wrSmDfltNeEnbPlmn1Mcc0[0] = nghCellCfg->nghPlmn[0] = plmnId.mcc[0];
                        wrSmDfltNeEnbPlmn1Mcc1[0] = nghCellCfg->nghPlmn[1] = plmnId.mcc[1];
                        wrSmDfltNeEnbPlmn1Mcc2[0] = nghCellCfg->nghPlmn[2] = plmnId.mcc[2];
                        wrSmDfltNeEnbPlmn1Mnc0[0] = nghCellCfg->nghPlmn[3] = plmnId.mnc[0];
                        wrSmDfltNeEnbPlmn1Mnc1[0] = nghCellCfg->nghPlmn[4] = plmnId.mnc[1];
                        wrSmDfltNeEnbPlmn1Mnc2[0] = nghCellCfg->nghPlmn[5] = plmnId.mnc[2];

#ifndef WR_RSYS_OAM
                        /* for same ENB ID dont allocate new peerId */
                        if(nghCellCfg->enbType == WR_ENODEB_TYPE_MACRO)
                           enbId = nghCellCfg->enbId >> 8;
                        else
                           enbId = nghCellCfg->enbId;

                        WR_GET_PEERID_FROM_ENBID(enbId, peerId);

                        if(peerId == 0)
                        {
                           WR_GET_X2AP_PEER_ID(peerId, enbId);
                        }
                        nghCellCfg->peerId = peerId;
#endif
                        nghCellCfg->dlEarfcn = (U16)atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->ulEarfcn = (U16)atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->trgrX2 = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                        /* CSG_DEV start */
                        nghCellCfg->csgAccessMode = (U8)atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->csgId         = (U32)atoi((S8 *)paramLst[paramCnt++]);
                        /* CSG_DEV end */
                        nghCellCfg->numAntenna = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->ulDlSubFrameCfg = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->splSfCfg = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->cpDl = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->cpUl = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->rsrp = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                        nghCellCfg->rssi = (Bool)atoi((S8 *)paramLst[paramCnt++]);
                        smCfgCb.wrNghCellCfg[loopCnt] = nghCellCfg;
                     }
                  }
               }
               else
               {
                  RLOG0(L_ERROR, "Error in WR_TAG_NGH_CELL_CFG tag format ");
               }
            }
            break;
         case WR_TAG_MEAS_CFG_ENB:
            {
               smCfgCb.suppMsrmntCfg = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_RRM_RNTI_STRT:
            {
               smCfgCb.rrmRntiStart = (U16)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_MAX_RRM_RNTIS:
            {
               smCfgCb.maxRrmRntis = (U16)atoi((S8 *)tagVal);
               wrSmDfltRrmRntisize = smCfgCb.maxRrmRntis;
            }
            break;
         case WR_TAG_MAC_RNTI_STRT:
            {
               smCfgCb.macRntiStart = (U16)atoi((S8 *)tagVal);
               wrSmDfltStartRnti = smCfgCb.macRntiStart;
            }
            break;
         case WR_TAG_MAX_MAC_RNTIS:
            {
               smCfgCb.maxMacRntis = (U16)atoi((S8 *)tagVal);
               wrSmDfltMacRntisize = smCfgCb.maxMacRntis;
            }
            break;
         case WR_TAG_RRM_NO_OF_DED_PREMBL:
            {
               smCfgCb.rrmNumDedPrm = (U8)atoi((S8 *)tagVal);
               wrSmDfltRrmNumPreambles = smCfgCb.rrmNumDedPrm;
            }
            break;
         case WR_TAG_RRM_DED_PREMBL_STRT:
            {
               smCfgCb.rrmDedPrmStart = (U8)atoi((S8 *)tagVal);
               wrSmDfltRrmStartPreamble = smCfgCb.rrmDedPrmStart;
            }
            break;
         case WR_TAG_MAC_NO_OF_PREMBL:
            {
               smCfgCb.numOfMacPrm = (U8)atoi((S8 *)tagVal);
            }
            break;
            /* ccpu00124317 */
            /* start */
         case WR_TAG_A1_RSRP_THRSHLD_VAL:
            {
               wrSmDfltA1ThresRSRP = atoi((S8 *)tagVal);
            }
            break;
            /*rsrq_ho*/
         case WR_TAG_A2_RSRP_THRSHLD_VAL:
            {
               wrSmDfltA2ThresRSRP = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_INTRA_A5_RSRP_THRSD1_VAL:
            {
               wrSmDfltIntraA5ThresRSRP1 = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_INTRA_A5_RSRP_THRSD2_VAL:
            {
               wrSmDfltIntraA5ThresRSRP2 = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_INTER_A5_RSRP_THRSD1_VAL:
            {
               wrSmDfltInterA5ThresRSRP1 = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_UTRA_B2_RSRP_THRSD1_VAL:
            {
               wrSmDfltUtraB2ThresRSRP = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_GERAN_B2_RED_RSRP_THRSD1_VAL:
            {
               wrSmDfltGeranB2REDThres1RSRP = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_GERAN_B2_HO_RSRP_THRSD1_VAL:
            {
               wrSmDfltGeranB2HOThres1RSRP = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_GERAN_B2_RED_THRSD2_VAL:
            {
               wrSmDfltGeranB2REDThres2 = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_GERAN_B2_HO_THRSD2_VAL:
            {
               wrSmDfltGeranB2HOThres2 = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_UTRA_FDD_B2_RSCP_THRSD2_VAL:
            {
               wrSmDfltUtraB2ThresRSCP = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_UTRA_TDD_B2_RSCP_THRSD2_VAL:
            {
               wrSmDfltUtraTddB2ThresRSCP = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_INTER_A5_RSRP_THRSD2_VAL:
            {
               wrSmDfltInterA5ThresRSRP2 = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_INTRA_HO_A3_OFFSET:
            {
               wrSmDfltIntraA3HoOffset = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_INTRA_ANR_A3_OFFSET:
            {
               wrSmDfltA3AnrOffset = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_ANR_EPOC_TMR_VAL_IN_SECS:
            {
               wrSmDfltEpocTmrVal = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_ANR_TRICE_INTV_COUNT:
            {
               wrSmDfltTricePrdIntv = atoi((S8*)tagVal);
            }
            break;

         case  WR_TAG_ANR_REPORT_CFG_VAL:
            {
               wrSmDfltAnrRptCfgVal = atoi((S8*)tagVal);
            }
            break;
         case  WR_TAG_ANR_CELL_VALID_AGE_VAL:
            {
               wrSmDfltAnrCelValidAge = atoi((S8*)tagVal);
            }
            break;
         
         case  WR_TAG_HO_REPORT_CFG_VAL:
            {
               wrSmDfltHoRptCfgVal = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_RSRQ_THRSHLD_VAL:
            {
               wrSmDfltThresRSRQ = atoi((S8 *)tagVal);
            }
            break;
            /*ccpu00130010*/
         case WR_TAG_S_MEASURE_VAL:
            {
               wrSmDfltSMeasure = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_INTRA_TTT_VAL:
            {
               wrSmDfltTimeToTrig = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_INTRA_HYTERISIS:
            {
               wrSmDfltHysteresis = atoi((S8*)tagVal);
            }
            /*ccpu00130010*/
            break;
         case WR_TAG_NO_OF_NGH_ENB_CFG:
            {
               smCfgCb.noOfnghENBs = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_NGH_ENB_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
            }
            break;
         case WR_TAG_PCI_VAL:
            {
               wrSmDfltPhysCellId = atoi((S8 *)tagVal);
               /*Fix for ccpu00144101 */
               smCfgCb.pci = wrSmDfltPhysCellId;
               /*SON will do PCI validation and select a valid PCI, henece no need to validate PCI here*/
            }
            break;
         case WR_TAG_DL_NUM_UE_PER_TTI:
            {
               wrSmDfltMaxUePerDlSf = (U8)atoi((S8 *)tagVal); 
               WR_SMM_VALIDATE_CFG_PARAM(wrSmDfltMaxUePerDlSf, 1, 4);
            }
            break;
         case WR_TAG_UL_NUM_UE_PER_TTI:
            {
               wrSmDfltMaxUePerUlSf = (U8)atoi((S8 *)tagVal); 
               WR_SMM_VALIDATE_CFG_PARAM(wrSmDfltMaxUePerUlSf, 1, 4);
            }
            break;
         case WR_TAG_MAX_DL_UE_PER_TTI:
            {
               wrSmDfltMaxDlUeNwTxPerTti  = (U8)atoi((S8 *)tagVal); 
               WR_SMM_VALIDATE_CFG_PARAM(wrSmDfltMaxDlUeNwTxPerTti, 1, 4);
            }
            break;
         case WR_TAG_MAX_UL_UE_PER_TTI:
            {
               wrSmDfltMaxUlUeNwTxPerTti   = (U8)atoi((S8 *)tagVal); 
               WR_SMM_VALIDATE_CFG_PARAM(wrSmDfltMaxUlUeNwTxPerTti, 1, 4);
            }
            break;
         case WR_TAG_DL_SCHD_TYPE:
            {
               wrSmDfltDlSchdType = (U8)atoi((S8 *)tagVal); 
               WR_SMM_VALIDATE_CFG_PARAM(wrSmDfltDlSchdType, 1, 2);
            }
            break;
         case WR_TAG_UL_SCHD_TYPE:
            {
               wrSmDfltUlSchdType = (U8)atoi((S8 *)tagVal); 
               WR_SMM_VALIDATE_CFG_PARAM(wrSmDfltUlSchdType, 1, 2);
            }
            break;
         case WR_TAG_DLFS_SCHD_TYPE:
            {
               wrSmDfltDLFSSchdTyp = (U8)atoi((S8 *)tagVal); 
               if(wrSmDfltDLFSSchdTyp > 1)
               {
                  fprintf(stderr, "Invalid parameter value wrSmDfltDLFSSchdTyp = %d, it should not be more than 1 \n", \
                     wrSmDfltDLFSSchdTyp);\
                  RETVALUE(RFAILED);\
               }   
            }
            break;
         case WR_TAG_PFS_DL_TPT_COEFFICIENT:
            {
               if ((atol((S8*)tagVal)) > 10)
               {
                  fprintf(stderr, "Invalid parameter, auto config value should not be more than 1 \n");
                  RETVALUE(RFAILED);
               }
               wrSmDfltPfsDlTptCoefficient = (U8)atoi((S8 *)tagVal); 
            }
            break;
         case WR_TAG_PFS_DL_FAIRNESS_COEFFICIENT:
            {
               wrSmDfltPfsDlFairCoefficient = (U8)atoi((S8 *)tagVal); 
               if ((atol((S8*)tagVal)) > 10)
               {
                  fprintf(stderr, "Invalid parameter, auto config value should not be more than 1 \n");
                  RETVALUE(RFAILED);
               }
            }
            break;
         case WR_TAG_PFS_UL_TPT_COEFFICIENT:
            {
               wrSmDfltPfsUlTptCoefficient = (U8)atoi((S8 *)tagVal); 
               if ((atol((S8*)tagVal)) > 10)
               {
                  fprintf(stderr, "Invalid parameter, auto config value should not be more than 1 \n");
                  RETVALUE(RFAILED);
               }
            }
            break;
         case WR_TAG_PFS_UL_FAIRNESS_COEFFICIENT:
            {
               wrSmDfltPfsUlFairCoefficient = (U8)atoi((S8 *)tagVal); 
               if ((atol((S8*)tagVal)) > 10)
               {
                  fprintf(stderr, "Invalid parameter, auto config value should not be more than 1 \n");
                  RETVALUE(RFAILED);
               }
            }
            break;
         case WR_TAG_TM_AUTO_CONFIG:
            {
               if ((atol((S8*)tagVal)) > 1)
               {
                  fprintf(stderr, "Invalid parameter, auto config value should not be more than 1 \n");\
                     RETVALUE(RFAILED);\
               }
               wrSmDfltTmAutoConfig = ((atol((S8*)tagVal)) == 1) ? TRUE : FALSE;
            }
            break;
         case WR_TAG_PREFERRED_TM:
            {
               wrSmDfltPreferredTm = (U8)atoi((S8 *)tagVal);
               if(wrSmDfltPreferredTm < 1 || wrSmDfltPreferredTm > 4)
               {
                  fprintf(stderr, "Invalid parameter value wrSmDfltPreferredTm = %d, it should be among 2,3,4 \n", \
                        wrSmDfltPreferredTm);\
                     RETVALUE(RFAILED);\
               }
            }
            break;
         case WR_TAG_MAX_X2_PEERS:
            {
               smCfgCb.maxX2Peers = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_SCTP_SRVC_TYPE:
            {
               smCfgCb.sctpUdpServiceType = (U8)atoi((S8 *)tagVal);
            }
            break;   
         case WR_TAG_OP_MODE:
            {
               wrSmDfltOpMode = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_PERIOD:
            {
               wrSmDfltTagPeriod = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_DL_EARFCN:
            {
               wrSmDfltServingCellDLCarrierFreq  = (U16)atoi((S8 *)tagVal);
               wrSmDfltEartcnNrDLEarfcn[0] = wrSmDfltServingCellDLCarrierFreq;
               break;
            }
         case WR_TAG_UL_EARFCN:
            {
               wrSmDfltUlEarFcn = (U16)atoi((S8 *)tagVal);
               break;
            }
#ifdef OAM_PH0_SUPPORT
         case WR_TAG_S1_RESET_TMR:
            {
               smCfgCb.s1ResetTmr = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_X2_RESET_TMR:
            {
               smCfgCb.x2ResetTmr = atoi((S8 *)tagVal);
            }
            break;
#endif
#ifdef TENB_AS_SECURITY
         case WR_TAG_AS_PRI_INTG_ALGO_LST:
            {
               /*reset default values when an entry is present in config file*/
               for(loopCnt = 0; loopCnt < WR_MAX_AS_SEC_ALGOS; loopCnt++)
               {
                  wreNodBPriIntgAlgo[loopCnt].pres = FALSE;
               }
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfElem == 0 || noOfElem > WR_MAX_AS_SEC_ALGOS)
                  {
                     fprintf(stderr, "Number of parameters in WR_TAG_AS_PRI_INTG_ALGO_LST invalid");
                     RETVALUE(RFAILED);
                  }
                  paramCnt = 0;
                  for(loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     wreNodBPriIntgAlgo[loopCnt].val = atoi((S8*)paramLst[paramCnt++]);
                     if(wreNodBPriIntgAlgo[loopCnt].val > (WR_MAX_AS_SEC_ALGOS - 1) || wreNodBPriIntgAlgo[loopCnt].val < 0)
                     {
                        fprintf(stderr, "Invalid values in WR_TAG_AS_PRI_INTG_ALGO_LST");
                        RETVALUE(RFAILED);
                     }
                     wreNodBPriIntgAlgo[loopCnt].pres = TRUE;
                  }
               }
               else
               {
                  fprintf(stderr, "Error in WR_TAG_AS_PRI_INTG_ALGO_LST tag format");
                  RETVALUE(RFAILED);
               }
            }
            break;
         case WR_TAG_AS_PRI_CIPH_ALGO_LST:
            {
               /*reset default values when an entry is present in config file*/
               for(loopCnt = 0; loopCnt < WR_MAX_AS_SEC_ALGOS; loopCnt++)
               {
                  wreNodBPriCiphAlgo[loopCnt].pres = FALSE;
               }
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  if(noOfElem == 0 || noOfElem > WR_MAX_AS_SEC_ALGOS)
                  {
                     fprintf(stderr, "Number of parameters in WR_TAG_AS_PRI_CIPH_ALGO_LST: invalid");
                     RETVALUE(RFAILED);
                  }
                  paramCnt = 0;
                  for(loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     wreNodBPriCiphAlgo[loopCnt].val = atoi((S8*)paramLst[paramCnt++]);
                     if(wreNodBPriCiphAlgo[loopCnt].val > (WR_MAX_AS_SEC_ALGOS - 1) || wreNodBPriCiphAlgo[loopCnt].val < 0)
                     {
                        fprintf(stderr, "Invalid values in WR_TAG_AS_PRI_CIPH_ALGO_LST");
                        RETVALUE(RFAILED);
                     }

                     wreNodBPriCiphAlgo[loopCnt].pres = TRUE;
                  }
               }
               else
               {
                  //RLOG0(L_ERROR, "Error in WR_TAG_AS_PRI_CIPH_ALGO_LST: tag format");
                  RETVALUE(RFAILED);
               }
            }
            break;
#endif
         case WR_TAG_DIAG_TUCL:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     smCfgCb.diagInfo.tucl.diagState = (Bool)atoi((S8 *)paramLst[loopCnt++]);
                     smCfgCb.diagInfo.tucl.diagLvl = atoi((S8 *)paramLst[loopCnt++]);
                  }
               }
               else
               {
                  ALARM("Wrong WR_TAG_DIAG_TUCL tag format\n");
               }
            }
            break;
         case WR_TAG_DIAG_S1AP:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     smCfgCb.diagInfo.s1ap.diagState = (Bool)atoi((S8 *)paramLst[loopCnt++]);
                     smCfgCb.diagInfo.s1ap.diagLvl = atoi((S8 *)paramLst[loopCnt++]);
                  }
               }
               else
               {
                  ALARM("Wrong WR_TAG_DIAG_S1AP tag format\n");
               }
            }           
            break;   
         case WR_TAG_DIAG_EGTP:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     smCfgCb.diagInfo.egtp.diagState = (Bool)atoi((S8 *)paramLst[loopCnt++]);
                     smCfgCb.diagInfo.egtp.diagLvl = atoi((S8 *)paramLst[loopCnt++]);
                  }
               }
               else
               {
                  ALARM("Wrong WR_TAG_DIAG_EGTP tag format\n");
               }
            }
            break;   
         case WR_TAG_DIAG_APP:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     smCfgCb.diagInfo.app.diagState = (Bool)atoi((S8 *)paramLst[loopCnt++]);
                     smCfgCb.diagInfo.app.diagLvl = atoi((S8 *)paramLst[loopCnt++]);
                  }
               }
               else
               {
                  ALARM("Wrong WR_TAG_DIAG_APP tag format\n");
               }
            }
            break;   
         case WR_TAG_DIAG_LTERRC:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     smCfgCb.diagInfo.lteRrc.diagState = (Bool)atoi((S8 *)paramLst[loopCnt++]);
                     smCfgCb.diagInfo.lteRrc.diagLvl = atoi((S8 *)paramLst[loopCnt++]);
                  }
               }
               else
               {
                  ALARM("Wrong WR_TAG_DIAG_LTERRC tag format\n");
               }
            }
            break;   
         case WR_TAG_DIAG_LTERLC:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     smCfgCb.diagInfo.lteRlc.diagState = (Bool)atoi((S8 *)paramLst[loopCnt++]);
                     smCfgCb.diagInfo.lteRlc.diagLvl = atoi((S8 *)paramLst[loopCnt++]);
                  }
               }
               else
               {
                  ALARM("Wrong WR_TAG_DIAG_LTERLC tag format\n");
               }
            }
            break;   
         case WR_TAG_DIAG_LTEPDCP:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     smCfgCb.diagInfo.ltePdcp.diagState = (Bool)atoi((S8 *)paramLst[loopCnt++]);
                     smCfgCb.diagInfo.ltePdcp.diagLvl = atoi((S8 *)paramLst[loopCnt++]);
                  }
               }
               else
               {
                  ALARM("Wrong WR_TAG_DIAG_LTEPDCP tag format\n");
               }
            }
            break;   
         case WR_TAG_DIAG_LTECL:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     smCfgCb.diagInfo.lteCl.diagState = (Bool)atoi((S8 *)paramLst[loopCnt++]);
                     smCfgCb.diagInfo.lteCl.diagLvl = atoi((S8 *)paramLst[loopCnt++]);
                  }
               }
               else
               {
                  ALARM("Wrong WR_TAG_DIAG_LTECL tag format\n");
               }
            }
            break;   
         case WR_TAG_DIAG_LTEMAC:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     smCfgCb.diagInfo.lteMac.diagState = (Bool)atoi((S8 *)paramLst[loopCnt++]);
                     smCfgCb.diagInfo.lteMac.diagLvl = atoi((S8 *)paramLst[loopCnt++]);
                  }
               }
               else
               {
                  ALARM("Wrong WR_TAG_DIAG_LTEMAC tag format\n");
               }
            }
            break;   
         case WR_TAG_DIAG_X2AP:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     smCfgCb.diagInfo.x2ap.diagState = (Bool)atoi((S8 *)paramLst[loopCnt++]);
                     smCfgCb.diagInfo.x2ap.diagLvl = atoi((S8 *)paramLst[loopCnt++]);
                  }
               }
               else
               {
                  ALARM("Wrong WR_TAG_DIAG_LTEX2AP tag format\n");
               }
            }
            break;   
         case WR_TAG_DIAG_SCTP:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     smCfgCb.diagInfo.sctp.diagState = (Bool)atoi((S8 *)paramLst[loopCnt++]);
                     smCfgCb.diagInfo.sctp.diagLvl = atoi((S8 *)paramLst[loopCnt++]);
                  }
               }
               else
               {
                  ALARM("Wrong WR_TAG_DIAG_LTESCTP tag format\n");
               }
            }
            break;   
#ifdef PHY_ERROR_LOGING
         case  WR_TAG_UL_MCS_VAL:
            {
               smUlAllocInfoCb.mcs = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_UL_RB_START_VAL:
            {
               smUlAllocInfoCb.rbStart = (U16)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_UL_NUM_RB_VAL:
            {
               smUlAllocInfoCb.numOfRb = (U16)atoi((S8 *)tagVal);
            }
            break;
         case  WR_TAG_UL_ENABLE_TIME:
            {
               smUlAllocInfoCb.logTime = (U16)atoi((S8 *)tagVal);
            }
            break;
#endif /* PHY_ERROR_LOGING */
            /* ccpu00126774 : Added the WR_TAG_DEFAULT_PAGING_CYCLE 
             * and WR_TAG_DEFAULT_PCCH_CFG_NB to read from config file */
         case WR_TAG_DEFAULT_PAGING_CYCLE: 
            {
               wrSmDfltDefaultPagCycle = atoi((S8 *)tagVal);
               if( wrSmDfltDefaultPagCycle > WR_PCCHCONFLOGIC_DEFAULTPAGINGCYCLE_MAX )
               {
                  wrSmDfltDefaultPagCycle = WR_PCCHCONFLOGIC_DEFAULTPAGINGCYCLE_MIN;
               }
               break;
            }
         case WR_TAG_DEFAULT_PCCH_CFG_NB: 
            {
               wrSmDfltPCCHCfgNB = atoi((S8 *)tagVal);
               if( wrSmDfltPCCHCfgNB > WR_PCCHCONFLOGIC_NB_MAX )
               {
                  wrSmDfltPCCHCfgNB = WR_PCCHCONFLOGIC_NB_MAX;
               } 
               break;
            }
         case  WR_TAG_SCTP_RTO_MIN:
            {
               smCfgCb.lwrSctpParamCfg.rtoMin = (atoi((S8 *)(tagVal))/100);

            }
            break;
         case  WR_TAG_SCTP_RTO_MAX:
            {
               smCfgCb.lwrSctpParamCfg.rtoMax= (atoi((S8 *)(tagVal))/100);
            }
            break;
         case  WR_TAG_SCTP_RTO_INITIAL:
            {
               smCfgCb.lwrSctpParamCfg.rtoInitial = (atoi((S8 *)(tagVal))/100);
            }
            break;
         case  WR_TAG_SCTP_HRTBEAT_INTERVAL:
            {
               smCfgCb.lwrSctpParamCfg.heartBeatInterval = (atoi((S8 *)(tagVal))/100);
            }
            break;

            /*DRX QCI*/
         case  WR_TAG_DRX_QCI_SUPPORT_ENABLED:
            {
               wrSmDfltisDrxQCIEnabled = atoi((S8*)tagVal);
               if( wrSmDfltisDrxQCIEnabled == TRUE)
               {
                  if(wrSmDfltAnrRptCfgVal == FALSE && wrSmDfltIsDrxEnabled ==FALSE)
                  {
                     wrSmDfltisDrxQCIEnabled = FALSE;	
                  }
               }
            }
            break;
            /*removed DRX flag*/ 
         case  WR_TAG_DRX_ENABLED:
            {
               wrSmDfltIsDrxEnabled = atoi((S8 *)tagVal);
            }   
            break;
         case  WR_TAG_DRX_INACTIVITY_TMR:
            {
               wrSmDfltDrxInactvTmr = atoi((S8 *)tagVal);
            }   
            break;
         case  WR_TAG_DRX_RETX_TMR:
            {
               wrSmDfltDrxReTxTmr = atoi((S8 *)tagVal);
            }   
            break;
         case WR_TAG_BANDWIDTH:
            {
               U32 freqBandwidth;
               freqBandwidth = atoi((S8 *)tagVal);
               wrSmmSetBandwidth(freqBandwidth);
            }
            break;
#ifdef WR_WATCHDOG
         case  WR_TAG_WATCHDOG_SOFT_LIMIT:
            {
               softTimeout = atoi((S8 *)tagVal);
            }   
            break;  
         case  WR_TAG_WATCHDOG_HARD_LIMIT:
            {
               hardTimeout = atoi((S8 *)tagVal);
            }   
            break;
#endif
         case  WR_TAG_DRX_LONG_CYCLE_GBR:
            {
               wrSmDfltDrxLongCycleGbr = atoi((S8 *)tagVal);
            }   
            break;
         case  WR_TAG_DRX_LONG_CYCLE_NON_GBR:
            {
               wrSmDfltDrxLongCycleNonGbr = atoi((S8 *)tagVal);
            }   
            break;
            /* ccpu00128279 */
         case WR_TAG_PRIMETWS_NO_BROADCAST:
            {
               wrSmDfltPrimEtwsBrdcst = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_PRIMETWS_REPETITION_PERIOD:
            {
               wrSmDfltPrimEtwsRepetition = atoi((S8*)tagVal);
            }
            break;
#ifdef LTE_TDD
         case WR_TAG_TDD_ULDL_CFG_MODE:
            {
               wrSmTddSubframeConfig = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_TDD_SPCL_SF_CONFIG:
            {
               wrSmTddSpecialSubframeConfig = atoi((S8*)tagVal);
            }
            break;
#endif
         case WR_TAG_LOG_PATH:
            {
#ifndef WR_RSYS_OAM
                  if(strlen((S8*)tagVal) < WR_MAX_LOG_PATH_VALUE)
                  {
                     strcpy((S8*)logPath,(S8 *)tagVal);
                  }
                  else
                  {
                     ALARM("WR_TAG_LOG_PATH tag value is greater than \
                           WR_MAX_LOG_PATH_VALUE ,taking default log path:/var/log/\n");
                     strcpy((S8*)logPath,"/var/log/");
                  }

               rlSetLogPath((const char*)logPath);
#endif
            }
            break;   
         case WR_TAG_LOG_MASK:
            {
               char* token = strtok((S8*)tagVal, ",");
               while( token) {
                  rlSetModuleMask((U32)atoi((S8*)token));
                  token = strtok(NULL, ",");
               }
            }
            break;

         case WR_TAG_LOG_FILE:
#ifndef WR_RSYS_OAM
            rlSetLogFile((S8 *)tagVal);
#endif

            break;        

         case WR_TAG_LOG_MAX_FILES:
            rlSetNumOfLogFiles(atoi((S8*)tagVal));
            break;        

         case WR_TAG_LOG_FILESIZE_LIMIT: 
            rlSetLogFileSizeLimit(atol((S8*)tagVal));
            break;        

         case WR_TAG_LOG_LEVEL:
            wrSmDfltRlLogLvl = atoi((S8*)tagVal);
            break;        

         case WR_TAG_LOG_PORT: 
            rlSetLogPort(atoi((S8*)tagVal));
            break;        

         case WR_TAG_LOG_REMOTE_LOGGING:
            rlSetRemoteLoggingFlag(atoi((S8*)tagVal));
            break;

         case WR_TAG_LOG_CIRBUF_SIZE:
            rlSetCircularBufferSize(atol((S8*)tagVal));
            break;

         case WR_TAG_LOG_COREDUMP_FLAG:
            rlEnableDisableCore(atoi((S8*)tagVal));
            break;
         case WR_TAG_RRM_SR_PRDCTY:
            {
               wrSmDfltSrPrdcty = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_RRM_CQI_PRDCTY:
            {
               wrSmDfltCqiPrdcty = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_RRM_NUM_SR_PER_TTI:
            {
               wrSmDfltNumSrResPerTti = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_RRM_NUM_CQI_PER_TTI:
            {
               wrSmDfltNumCqiResPerTti = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_RRM_N1_PUCCH:
            {
               wrSmDfltNlPUCCHAn = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_PWS_ETWS_CMAS_CNTRL:
            {
               wrSmPwsEtwsCmas = atoi((S8*)tagVal);
               if(wrSmPwsEtwsCmas > 2)
               {
                  RLOG0(L_ERROR, "Invalid value entered to enable ETWS or CMAS");
                  RLOG0(L_ERROR, "Disabling PWS in the System");
               }

            }
            break;
            /* SPS changes starts */
         case WR_TAG_SPS_ENABLE_FLAG:
            if ((atol((S8*)tagVal)) == 1)
            {
               wrSmDfltIsSpsEnabled = TRUE;
            }
            break;

         case WR_TAG_DL_NUM_SPS_UE_PER_TTI:
            if (((atol((S8*)tagVal)) > 0) && ((atol((S8*)tagVal)) <= wrSmDfltMaxUePerDlSf))
            {
               /* Incase invalid value is passed the default value is taken as 1 */
               wrSmDfltMaxDlSpsUePerTti = (U16)atol((S8*)tagVal);
            }
            break;

         case WR_TAG_UL_NUM_SPS_UE_PER_TTI:
            if (((atol((S8*)tagVal)) > 0) && ((atol((S8*)tagVal)) <= wrSmDfltMaxUePerUlSf))
            {
               /* Incase invalid value is passed the default value is taken as 1 */
               wrSmDfltMaxUlSpsUePerTti = (U16)atol((S8*)tagVal);
            }
            break;
         case WR_TAG_MAX_SPS_RB:
            {
               U32  dlCellBw = 0;
               dlCellBw = wrSmDlCellBwInRbs(wrSmDfltCellDlBW);
               wrSmDfltSpsDlBandwidth = (U32)atol((S8*)tagVal);
               if ((wrSmDfltSpsDlBandwidth <= 0) || (wrSmDfltSpsDlBandwidth > dlCellBw))
               {
                  /* default value is taken as 50% of the cell bandwidth */
                  wrSmDfltSpsDlBandwidth = (dlCellBw * 50) / 100; /* By default 50% */
               }
            }
            break;
            /* SPS changes ends */
         case  WR_TAG_DSCP_ENABLE:

            smCfgCb.isDscpEnable=atoi((S8*)tagVal);

            break;
         case  WR_TAG_QCI_DSCP_MAP:
            {
               status = TRUE;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status && smCfgCb.isDscpEnable == TRUE)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt=0;loopCnt<noOfParams;loopCnt++)
                  {
                     dscp = atoi((S8*)paramLst[loopCnt]);
                     if(!(dscp & DSCP_ECN_BITS))
                     {
                        smCfgCb.qciDscpMap[loopCnt]=dscp;
                     }
                     else
                     {
                        fprintf(stderr, "Invalid value for DSCP");
                        RETVALUE(RFAILED);
                     }   
                  }
               }
               else
               {
                  fprintf(stderr, "DSCP is disabled");
               }
            }
            break;
         case  WR_TAG_DL_QCI_SCHD_WGT:
            {
               status = TRUE;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt=0;loopCnt<noOfParams;loopCnt++)
                  {
                     qciWgtVal = atoi((S8*)paramLst[loopCnt]);
                     if(qciWgtVal)
                     {
                        wrSmDfltDlQciWgt[loopCnt]= qciWgtVal;
                     }
                     else
                     {
                        fprintf(stderr, "Invalid value ");
                        RETVALUE(RFAILED);
                     }   
                  }
               }
               else
               {
                  fprintf(stderr, "Dl Qci Wgt is disabled");
               }
            }
            break;
         case  WR_TAG_UL_QCI_SCHD_WGT:
            {
               status = TRUE;
               noOfParams = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt=0;loopCnt<noOfParams;loopCnt++)
                  {
                     qciWgtVal = atoi((S8*)paramLst[loopCnt]);
                     if(qciWgtVal)
                     {
                        wrSmDfltUlQciWgt[loopCnt]= qciWgtVal;
                     }
                     else
                     {
                        fprintf(stderr, "Invalid value ");
                        RETVALUE(RFAILED);
                     }   
                  }
               }
               else
               {
                  fprintf(stderr, "Ul Qci Wgt is disabled");
               }
            }
            break;

            /* CSG_DEV start CSG configuration parameters */
         case WR_TAG_CSG_CSG_ID:
            {
               wrSmDfltCsgIdVal = (U32)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_CSG_ACCESS_MODE:
            {
               wrSmDfltAccessMode = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_CSG_CSG_PCI_START:
            {
               wrSmDfltIsCsgPciCfgd = TRUE;
               wrSmDfltCSGStartPci = (U16)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_CSG_CSG_PCI_RANGE:
            {
               wrSmDfltCSGPciRange = (U16)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_CSG_SMCELL_PCI_START:
            {
               wrSmDfltIsSmlCellPciCfgd  = TRUE;
               wrSmDfltSmallCellStartPci = (U16)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_CSG_SMCELL_PCI_RANGE:
            {
               wrSmDfltSmallCellPciRange = (U16)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_CSG_HENB_NAME:
            {
               tagLen = strlen((S8 *)tagVal);
               if(48 < tagLen)
               {
                  tagLen = 48;
                  fprintf(stderr, "Shrinking HeNB name to 48 charectors");
               }
               cmMemcpy((U8*)&wrSmDfltHenbName,(U8*)tagVal, tagLen);
            }
            break;
         case WR_TAG_MAX_CSG_UES:
            {
               wrSmDfltMaxCsgUEs = (U16)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_MAX_NON_CSG_UES:
            {
               wrSmDfltMaxNonCsgUEs = (U16)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_MIN_DL_RSRC_FOR_NON_CSG:
            {
               wrSmDfltMinDlResNonCsg = (U8)atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_MIN_UL_RSRC_FOR_NON_CSG:
            {
               wrSmDfltMinUlResNonCsg = (U8)atoi((S8 *)tagVal);
            }
            break;
            /* CSG_DEV end CSG configuration parameters */
         /*UE History info - start*/
         case WR_TAG_CELL_SIZE_TYPE:
            {
               wrSmDfltCellSizeType = (U8)atoi((S8*)tagVal);
               if((wrSmDfltCellSizeType < WR_ENB_CELL_SIZE_TYPE_VERY_SMALL) ||
                   (wrSmDfltCellSizeType > WR_ENB_CELL_SIZE_TYPE_LARGE))
               {
                  wrSmDfltCellSizeType = WR_ENB_CELL_SIZE_TYPE_VERY_SMALL;
               }
           }
            break;
         /*UE History info - end*/

            /* LTE_ADV starts */
            /* LTE_ADV_ABS starts */
         case WR_TAG_ARP_EMER_SERV:
            {
               wrSmDfltArpForEmerServ = (U8)atoi((S8*)tagVal);
               if(wrSmDfltArpForEmerServ > 15)
               {
                  wrSmDfltArpForEmerServ = 1;
               }
            }
            break;
         case WR_TAG_ABS_A3_OFFSET:
            {
               smCfgCb.lteAdvancedConfig.a3Offset = (S8)atoi((S8*)tagVal); 
            }
            break;
         case WR_TAG_PICO_OFFSET:
            {
               smCfgCb.lteAdvancedConfig.reCfg.neighCIO= (U8)atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_ABS_PATTERN_TYPE:
            {
               smCfgCb.lteAdvancedConfig.absCfg.absPatternType   = (U32)atoi((S8*)tagVal); 
               if(RGR_ABS_TRANSMIT == smCfgCb.lteAdvancedConfig.absCfg.absPatternType)
               {
                  smCfgCb.lteAdvancedConfig.enbType = WR_LTE_ADV_PICO_ENB;
               }
               else
               {
                  smCfgCb.lteAdvancedConfig.enbType = WR_LTE_ADV_MACRO_ENB;
               }
            }
            break;
        case WR_TAG_CELL_PMAX:
            {
               wrSmDfltPMax = (S8)atoi((S8*)tagVal);
            }
            break; 
         case  WR_TAG_CNM_ENABLED:
            {
               wrSmDfltIsCnmEnabled = atoi((S8 *)tagVal);
               printf("\n CNM Status ::%d\n",wrSmDfltIsCnmEnabled);
            }   
            break;
         case  WR_TAG_CNM_BOARD_TYPE:
            {
               wrSmDfltTddBoardType = atoi((S8 *)tagVal);
               printf("\n CNM board type is  ::%d\n",wrSmDfltTddBoardType);
            }   
            break;
            
         case WR_TAG_ABS_PATTERN:
            {
               U8   temp[RGR_ABS_PATTERN_LEN]; 
               Bool invalidValue =FALSE;
               U32 idx;
               const U32 baseAscii2Num = 48;
               cmMemcpy((U8 *) temp,  (U8 *) &tagVal, RGR_ABS_PATTERN_LEN);
               for (idx = 0; idx < RGR_ABS_PATTERN_LEN; idx++)
               {
                  if (temp[idx] != '0' && temp[idx] != '1')
                  {
                     fprintf(stderr, "Please check the Value.The pattern should have only 0 and 1\n");
                     invalidValue = TRUE;
                     break;
                  }
                  smCfgCb.lteAdvancedConfig.absCfg.absPattern[idx] = temp[idx] - baseAscii2Num;
               }
            }
            break;
         case WR_TAG_ABS_LOAD_PERIODICITY:
            {
               smCfgCb.lteAdvancedConfig.absCfg.absLoadPeriodicity = (U32)atoi((S8*)tagVal);
            }
            break;
            /* LTE_ADV_ABS ends */
         case WR_TAG_SFR_START_RB:
            {
               smCfgCb.lteAdvancedConfig.sfrCfg.cellEdgeRbRange.startRb = (U8)atoi((S8*)tagVal); 
            }
            break;

         case WR_TAG_SFR_END_RB:
            {
               smCfgCb.lteAdvancedConfig.sfrCfg.cellEdgeRbRange.endRb  = (U8)atoi((S8*)tagVal); 
            }
            break;

#ifdef TFU_UPGRADE            
         case WR_TAG_SFR_POWER_LOW:
            {
               smCfgCb.lteAdvancedConfig.sfrCfg.pwrThreshold.pLow = (U32)atoi((S8*)tagVal);
            }
            break;

         case WR_TAG_SFR_POWER_HIGH:
            {
               smCfgCb.lteAdvancedConfig.sfrCfg.pwrThreshold.pHigh = (U32)atoi((S8*)tagVal);
            }
            break;
#endif            
            /* LTE_ADV ends */
         case WR_TAG_RIM_CFG:
            {   
               status     = TRUE;
               noOfElem   = 0;
               noOfParams = 0;
               loopCnt    = 0;
               wrCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  wrCfgGetParamVal(tagVal, paramLst, &noOfParams);

                  smCfgCb.rimOvrlTmr   = 
                     (U32)atoi((S8*)paramLst[loopCnt++]);
                  if(( smCfgCb.rimOvrlTmr < WR_RIM_OVRL_TMR_MIN_VAL ) || 
                     ( smCfgCb.rimOvrlTmr > WR_RIM_OVRL_TMR_MAX_VAL ))
                  {
                     smCfgCb.rimOvrlTmr = WR_RIM_OVRL_TMR_DFLT_VAL;
                     RLOG0(L_ERROR, "rimOvrlTmr out of range.Assigned rimOvrlTmr with default value ");
                  }

                  smCfgCb.rimRirTmr       = 
                     (U32)atoi((S8*)paramLst[loopCnt++]);
                  if(( smCfgCb.rimRirTmr < WR_RIM_TMR_MIN_VAL ) || 
                     ( smCfgCb.rimRirTmr > WR_RIM_TMR_MAX_VAL ))
                  {
                     smCfgCb.rimRirTmr = WR_RIM_TMR_DFLT_VAL;
                     RLOG0(L_ERROR, "rimRirTmr out of range.Assigned rimRirTmr with default value ");
                  }

                  smCfgCb.rimRiTmr        = 
                     (U32)atoi((S8*)paramLst[loopCnt++]);
                  if(( smCfgCb.rimRiTmr < WR_RIM_TMR_MIN_VAL ) || 
                     ( smCfgCb.rimRiTmr > WR_RIM_TMR_MAX_VAL ))
                  {
                     smCfgCb.rimRiTmr = WR_RIM_TMR_DFLT_VAL;
                     RLOG0(L_ERROR, "rimRiTmr out of range.Assigned rimRiTmr with default value ");
                  }

                  smCfgCb.rimRiaeTmr      = 
                     (U32)atoi((S8*)paramLst[loopCnt++]);
                  if(( smCfgCb.rimRiaeTmr < WR_RIM_TMR_MIN_VAL ) || 
                     ( smCfgCb.rimRiaeTmr > WR_RIM_TMR_MAX_VAL ))
                  {
                     smCfgCb.rimRiaeTmr = WR_RIM_TMR_DFLT_VAL;
                     RLOG0(L_ERROR, "rimRiaeTmr out of range.Assigned rimRiaeTmr with default value ");
                  }

                  smCfgCb.rimMaxRetryCnt  = 
                     (U32)atoi((S8*)paramLst[loopCnt]);
                  if(( smCfgCb.rimMaxRetryCnt < WR_RIM_RETRY_CNT_MIN_VAL ) || 
                     ( smCfgCb.rimMaxRetryCnt > WR_RIM_RETRY_CNT_MAX_VAL ))
                  {
                     smCfgCb.rimMaxRetryCnt = WR_RIM_RETRY_CNT_DFLT_VAL;
                     RLOG0(L_ERROR, "rimMaxRetryCnt out of range.Assigned rimMaxRetryCnt with default value ");
                  }
               }
               else
               {
                  fprintf(stderr, "Failed reading the RIM Parameters");
                  RETVALUE(RFAILED);
               }
            }
            break;

         case WR_TAG_DCFI_ENABLE:
            {
               wrSmDfltIsDcfiEnabled = (Bool)atoi((S8*)tagVal);  
            }
            break;

         case WR_TAG_CFI:
            {
               wrSmDfltCfiCfg = (U32)atoi((S8*)tagVal);
            }
            break; 
            /* Start ccpu00138740 */
         case  WR_TAG_PRACH_CONFIG_IDX:
            {
               wrSmDfltPrachCfgIdx = atoi((S8 *)tagVal);
            }
            break;
            /* End ccpu00138740 */
         case WR_TAG_GRP_PUCCH_PWR_CTRL_ENABLE:
            {
               wrSmDfltisUlGrpPwrCntrlPucchEnable = (U8)atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_GRP_PUSCH_PWR_CTRL_ENABLE:
            {
               wrSmDfltisUlGrpPwrCntrlPuschEnable = (U8)atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_LTE_PUCCH_PWR_FMT3A_ENABLE:
            {
               wrSmDfltPucchPwrFmt3AEnable = (U8)atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_LTE_PUSCH_PWR_FMT3A_ENABLE:
            {
               wrSmDfltPuschPwrFmt3AEnable = (U8)atoi((S8*)tagVal);
            }
         case  WR_TAG_TA_TMR_CMN:
            {
               wrSmDfltTaTmrCmn = atoi((S8 *)tagVal);
            }   
            break;
	     case  WR_TAG_TA_TMR_DED:
            {
               wrSmDfltTaTmrDed = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_DUAL_RX_TX_SUPPORTED:
            {
               wrSmDfltDualRxTxSup = atoi((S8*)tagVal);
            }
            break;
            /*RBC_SP1_X*/
         case WR_TAG_NUM_PRB_RPTS:
            {
               wrSmDfltNoOfPrbRpts = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_PRB_RPT_INTERVAL:
            {
               wrSmDfltPrbRptInterval = atoi((S8*)tagVal);
            }
            break;
            /*RBC_SP1_Y*/
#ifdef RSYS_WIRESHARK
         case WR_TAG_WIRESHARK_PORT:
            {
               wrSmDfltWiresharkPort = atoi((S8*)tagVal);
            }
            break;
         case WR_TAG_WIRESHARK_DST_IP_ADDR:
            {
               cmInetAddr((S8 *)tagVal,&(wrSmDfltWiresharkDstIpAddr));
               wrSmDfltWiresharkDstIpAddr = CM_INET_HTON_U32(wrSmDfltWiresharkDstIpAddr);
            }
            break;
#endif/*RSYS_WIRESHARK*/

         case WR_TAG_SRS_ENABLE:
            {
               wrSmEnableSrs = atoi((S8*)tagVal);
               if (wrSmEnableSrs > 1)
               {
#ifdef DEBUGP
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Please enter value which is in valid range 0 or 1 for enabling or disabling SRS \n"));
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Current disabling by default"));
#endif
                  wrSmEnableSrs = FALSE;
                  RETVALUE(RFAILED);
               }
               printf("[SRS] SRS status [%d] 0- Disabled, 1 - enabled", wrSmEnableSrs);
            }
            break;
         case WR_TAG_SRS_PERIODICITY:
            {
               wrSmUlSrsPeriod = atoi((S8*)tagVal);
               if ((wrSmUlSrsPeriod < LWR_SM_MIN_ULSRS_PERIOD) && (wrSmUlSrsPeriod > LWR_SM_MAX_ULSRS_PERIOD))
               {
#ifdef DEBUGP
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Please enter value which is in valid range Min [%d] ms and Max [%d] ms for UL SRS Periodicity\n", LWR_SM_MIN_ULSRS_PERIOD, LWR_SM_MAX_ULSRS_PERIOD));
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Current support only for 20 millisecond only"));
#endif
                  wrSmUlSrsPeriod = 20;
                  RETVALUE(RFAILED);
               }
               if(wrSmUlSrsPeriod == SRS_PERIODICITY_20MS)
               {
                  /* SUBBU Need to check */
                  wrSmDfltSrsSubfrmCfg = 0;
                  wrSmDfltSrsCfgPrdEnum = 5;
               }
               else
               {
                  //printf("[SRS] configured BW is not 20ms, it is [%d], need to visit for srsCfgPrdenum[%d]",(U32)wrSmUlSrsPeriod, (U32 )wrSmDfltSrsCfgPrdEnum);
               }
            }
            break;

         case WR_TAG_SRS_CELL_BANDWIDTH:
            {
               wrSmDfltSrsBandwidthCfg = atoi((S8*)tagVal);

               if(wrSmDfltSrsBandwidthCfg > 7)
               {
#ifdef DEBUGP
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Please enter value which is in valid range Min [0] ms and Max [7] ms for UL CELL SRS Bandwidth\n"));
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Current support only for bandwidth 2"));
#endif
                  wrSmDfltSrsBandwidthCfg = 2;
                  RETVALUE(RFAILED);
               }

            }
            break;

         case WR_TAG_SRS_CELL_SUBFRAME_CFG:
            {
               wrSmDfltSrsSubfrmCfg = atoi((S8*)tagVal);
               if(wrSmDfltSrsSubfrmCfg > 15)
               {
#ifdef DEBUGP
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Please enter value which is in valid range Min [0] ms and Max [15] ms for UL CELL SRS Subframe Config\n"));
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Current support only for bandwidth 0"));
#endif
                  wrSmDfltSrsSubfrmCfg = 0;
                  RETVALUE(RFAILED);
               }
            }
            break;

         case WR_TAG_SRS_CELL_ACKNACK_SIMTX:
            {
               if ((atol((S8*)tagVal)) == 1)
               {
                  wrSmDfltAckNackSrsSimTrns = TRUE;
               }
               else
               {
                  wrSmDfltAckNackSrsSimTrns = FALSE;
#ifdef DEBUGP     
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "AckNack SRS simultaneous is set to FALSE"));
#endif

               }
            }
            break;

         case WR_TAG_SRS_CELL_MAX_UPPTS:
            {
               wrSmDfltSrsMaxUpPts = atoi((S8*)tagVal);
               if(wrSmDfltSrsMaxUpPts > 1)
               {
#ifdef DEBUGP
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Please enter value which is in valid value either 0 or 1 for UL CELL SRS Max UpPTS\n"));
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Current support only for 0 (disalbing)"));
#endif
                  wrSmDfltSrsMaxUpPts = 0;
                  RETVALUE(RFAILED);
               }

            }
            break;

         case WR_TAG_SRS_UE_BANDWIDTH:
            {
               wrSmDfltSrsUeBandwidthCfg = atoi((S8*)tagVal);
               if(wrSmDfltSrsUeBandwidthCfg > 3)
               {
#ifdef DEBUGP
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Please enter value which is in valid range Min [0] ms and Max [3] ms for UL UE SRS Bandwidth\n"));
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Current support only for bandwidth 0"));
#endif
                  wrSmDfltSrsUeBandwidthCfg = 0;
                  RETVALUE(RFAILED);
               }
            }

            break;

         case WR_TAG_SRS_UE_HOPPING_BW:
            {
               wrSmDfltSrsUEHoppingBandwidth = atoi((S8*)tagVal);
               if(wrSmDfltSrsUEHoppingBandwidth > 3)
               {
#ifdef DEBUGP
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Please enter value which is in valid range Min [0] ms and Max [3] ms for UL UE SRS Bandwidth\n"));
                  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, \
                           "Current support only for hopping bandwidth 0"));
#endif
                  wrSmDfltSrsUEHoppingBandwidth = 0;
                  RETVALUE(RFAILED);
               }
            }
            break;
            
         case WR_TAG_RSYS_TPM_TGT_RSRP:
            {
               smCfgCb.tpmCfg.tgtRsrp = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_RSYS_TPM_ATTN_CONST_IDX:
            {
               smCfgCb.tpmCfg.attnConstIdx = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_RSYS_TPM_MIN_RSRP:
            {
               smCfgCb.tpmCfg.minRsrp = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_RSYS_TPM_MAX_RSRP:
            {
               smCfgCb.tpmCfg.maxRsrp = atoi((S8 *)tagVal);
            }
            break;
         case WR_TAG_NUM_TX_ANTENNA:         
            {
               wrSmDfltNumOfTxAnt = atoi((S8 *)tagVal);
            }   
            break;

         default :
            {
            }
            break;   
      }/* End of switch() */
      tagNum = MAX_CFG_PARAM;
   }/* End of for() */

   fprintf(stdout, "Read the wr_cfg.txt file successfully ");
   fclose(fp);
#ifdef WR_RSYS_OAM
   char config_log_path[WR_MAX_LOG_PATH_VALUE*2];
   char config_log_file[MAX_LOG_FILE_NAME_LENGTH*2];
   FILE *fptr;
   if ((fptr=fopen(SGetConfigPath(), "r")))
   {
      getData(fptr,"OAM_LOGGING_DIR",config_log_path);
      if(strlen(config_log_path) >= 100)//WR_MAX_LOG_PATH_VALUE)
      {
         ALARM("WR_TAG_LOG_PATH tag value is greater than \
               WR_MAX_LOG_PATH_VALUE ,taking default log path:/var/log/\n");
         strcpy((S8*)logPath,"/var/log");
      }
      else
      {
         strcpy((S8*)logPath,config_log_path);
      }
      getData(fptr,"OAM_TAG_LOG_FILE_NAME",config_log_file);
      if(strlen(config_log_file) >= MAX_LOG_FILE_NAME_LENGTH)
      {
         ALARM("OAM_TAG_LOG_FILE_NAME length is greater than  MAX_LOG_FILE_NAME_LENGTH");
         RETVALUE (RFAILED);
      }
      else
      {
         strcpy((S8*)logFileName, config_log_file);
      }
      rlSetLogFile((S8 *)logFileName);
      rlSetLogPath((const char*)logPath);
   }

   fclose(fptr);
   fprintf(stdout, "Read the configFile file successfully ");
#endif


   RETVALUE (ROK);
} /* end of smWrReadConfigParams */

PUBLIC Void getData
 (
  FILE *fptr,
  char *fieldName ,
  char *FieldValue
  )
{
   *FieldValue='\0';
   char buff[75];
   U8 ret;
   while(fscanf(fptr,"%s",buff)!=EOF)
   {
      if(!strcmp(buff,fieldName))
      {
         ret = fscanf(fptr,"%s",buff); // read value
         strcpy(FieldValue,buff);
         break;
      }
   }
}
