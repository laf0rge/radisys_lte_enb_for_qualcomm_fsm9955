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

     Name:     SON Application

     Type:     

     Desc:     This file contains 
               

     File:     nl_lmm.c

     Sid:      bk_prach_sel.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/3 - Fri Jan  2 16:28:40 2015

     Prg:      uday 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="SON";
static int RLOG_FILE_ID=251;
static int RLOG_MODULE_ID=256;

/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"

#include "cm_tkns.h"       /* Common Token defines */
#include "cm_mblk.h"       /* Common Mem Allocation defines */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_pasn.h"       /* Common Pasn defines */
#include "cm_lte.h"        /* Common Pasn defines */
#include "rl_common.h"     /* RLOG defines */   
#include "rl_interface.h"   
#include "lnl.h"           /* NL Interface defines */
#include "nlu.h"
#include "nl.h"            /* SON defines */
/* Header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_mblk.x"       /* Common Memory Allocation */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_lib.x"        /* Common Library definitions */
#include "cm_pasn.x"       /* Common Hashlist definitions */
#include "cm_lte.x"       /* Common Pasn defines */

#include "lnl.x"           /* NL Interface includes */
#include "nlu.x"
#include "nl.x"            /* SON includes */

/* local defines */
static ZeroCorrerelationConfigZoneType zero_correlation_config_table[16] =
{
   {0,   0,   0},
   {13,  64,  1},
   {15,  55,  2},
   {18,  46,  2},
   {22,  38,  2},
   {26,  32,  2},
   {32,  26,  3},
   {38,  22,  3},
   {46,  18,  4},
   {59,  14,  5},
   {76,  11,  6},
   {93,  9,   8},
   {119, 7,  10},
   {167, 5,  13},
   {279, 3,  22},
   {419, 2,  32},
};

/* local typedefs */
 
/* local externs */
/* forward references */
PRIVATE S16 nlCheckIfRootSeqIndexIsUnique ARGS ((
NlTmpNghCellCfg   *nghCellCfg,       /* neighbor cell config structure */
U32               rootSeqIdx         /* Root Sequence Index */
));

PRIVATE U16 nlCalcTotalResElemGrps ARGS ((
U8    bw,
U8    cfi,
U8    numAntna,
Bool  isEcp
));

PRIVATE U16 nlCalcPhichResElemGrps ARGS ((
U8 bw,
U8 ng6
));

PRIVATE S16 nlPrachSelPrachFreqOffsetByDSon ARGS ((
NlCellCb        *cellCb        /* cell control block*/
));

PRIVATE U8 nlCalcNumCce ARGS ((
U8            bw,
NlPhichNg     ng,
U8            cfi,
U8            mi,
U8            numAntna,
Bool          isEcp
));

PRIVATE S16 nlPrachSelPrachCfgIndxByDSon ARGS ((
NlEventMsg      *evtMsg,       /* neighbor cell config structure */
NlCellCb        *cellCb        /* cell control block*/
));

PRIVATE S16 nlPrachSelZeroCorrZoneCfgByDSon ARGS ((
NlEventMsg      *evtMsg,       /* neighbor cell config structure */
NlCellCb        *cellCb        /* cell control block*/
));

PRIVATE S16 nlSelPrachInfoByDSon   ARGS((
NlCellCb        *cellCb,       /* cell control block*/
NlEventMsg      *evtMsg,       /* neighbor cell config structure */
NlAlarm         *alarm         /* alarm structure*/
));


PRIVATE S16 nlPrachSelRootSeqIdxByDSon ARGS((
NlEventMsg      *evtMsg,       /* neighbor cell config structure */
NlCellCb        *cellCb,       /* cell control block*/
NlAlarm         *alarm         /* alarm structure*/
));

PRIVATE S16 nlBldNghRootSeqIndexs ARGS((
NlCellCb         *cellCb,       /* cell control block*/
NlTmpNghCellAdd  *nghCellAdd,   /* neighbor cell configuration structure*/
NlAlarm          *alarm         /* alarm structure*/
));


/**
 * @brief Validation of PRACH Configuration.
 *
 * @details
 *
 *     Function : nlValidateCellPrachCfg
 *
 *     This function validates the SON PRACH Configuration to see if the lists
 *     are empty or not.
 *
 *  @param[in]  prachCfg  : PRACH Configuration from SM
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlValidateCellPrachCfg
(
LnlPrachCfg   *prachCfg   /* cell prach configuration */
)
#else
PUBLIC S16 nlValidateCellPrachCfg(prachCfg)
(
LnlPrachCfg   *prachCfg;   /* cell prach configuration */
)
#endif
{
   S16   ret = ROK;
   TRC3(nlValidateCellPrachCfg);
   do
   {
      if(0 == prachCfg->numRootSeqIndex)
      {
         RLOG0(L_ERROR, "Empty PRACH root sequence List received");
         ret = RFAILED;
         break;
      }
      if(0 == prachCfg->numPrachCfgIdx)
      {
         RLOG0(L_ERROR, "Empty PRACH config index List received");
         ret = RFAILED;
         break;
      }
      if(0 == prachCfg->numZeroCorrCfgZone)
      {
         RLOG0(L_ERROR, "Empty PRACH Zero correlation config zone List received");
         ret = RFAILED;
         break;
      }
      if(0 == prachCfg->numFreqOffset)
      {
         RLOG0(L_ERROR, "Empty PRACH Frequency Offset List received");
         ret = RFAILED;
         break;
      }
   }while(0);

   RETVALUE(ret);
}

/**
 * @brief convert B/W configuration from LNL to PRB's
 *
 * @details
 *
 *     Function : nlFillBwConfig
 *
 *     This function store PRACH Configuration in SON
 *
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlFillBwConfig
(
U8 dlbW
)
#else
PUBLIC S16 nlFillBwConfig(dlBw)
(
U8 dlbW;
)
#endif
{
   U8 bwOut;
   switch(dlbW)
   {
      case 0:
         bwOut = 6;
         break;
      case 1:
         bwOut = 15;
         break;
      case 2:
         bwOut = 25;
      break;
      case 3:
         bwOut = 50;
         break;
      case 4:
         bwOut = 75;
         break;
      case 5:
         bwOut = 100;
         break;
      default:
         RLOG1(L_ERROR,"Unknown bwIn [%d]", dlbW);
         bwOut = 6;
         break;
   }   
   RETVALUE(bwOut);
}


/**
 * @brief Store Np value from P
 *
 * @details
 *
 *     Function : nlDlNpValueInit
 *
 *  Prepare Np value from P
 *
 *  @param[in]  cellCb   : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlDlNpValueInit
(
NlCellCb        *cellCb       /* cell control block*/
)
#else
PRIVATE S16 nlDlNpValueInit(cellCb)
(
NlCellCb        *cellCb;       /* cell control block*/
)
#endif
{
   U8 idx;
   U16 npVal;

   /* Always Np is 0 for p=0 */
   cellCb->pucchResCfg.mappingNpwithP[0] = 0;

   for(idx=1; idx < NL_TDD_MAX_P_PLUS_ONE_VAL; idx++)
   {
      npVal = cellCb->bandWidth * (idx * NL_NUM_SUBCAR - 4);
      cellCb->pucchResCfg.mappingNpwithP[idx] = (U8) (npVal/36);
   }

   RETVALUE(ROK);
}



/**
 * @brief storing of PRACH Configuration in SON cell control block
 *
 * @details
 *
 *     Function : nlCellStorePrachCfg
 *
 *     This function store PRACH Configuration in SON
 *
 *  @param[in]  cellCb    : cell control block
 *  @param[in]  cellCfg   : cell cfg received from SM
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlCellStorePrachCfg
(
NlCellCb     *cellCb,       /* cell control block*/
LnlCellCfg   *cellCfg       /* cell configuration */
)
#else
PUBLIC S16 nlCellStorePrachCfg(cellCb, cellCfg)
(
NlCellCb     *cellCb;      /* cell control block*/
LnlCellCfg   *cellCfg;     /* cell configuration */
)
#endif
{
   TRC3(nlCellStorePrachCfg);

   cellCb->isCpUlExtend = cellCfg->isCpUlExtend;
   cellCb->numTxAntPorts = cellCfg->numTxAntPorts;
   cellCb->bandWidthEnum = cellCfg->bandWidth; 
   cellCb->bandWidth = nlFillBwConfig(cellCfg->bandWidth);
   /*cellCb->bandWidth = cellCfg->bandWidth[0];*/
   cellCb->subFrameCfg = cellCfg->subFrameCfg;

   cmMemcpy((U8 *)(&cellCb->pucchResCfg),
            (U8 *)(&cellCfg->pucchResCfg), sizeof(NlPucchCfg));

   cmMemcpy((U8 *)(&cellCb->phichCfg),
            (U8 *)(&cellCfg->phichCfg), sizeof(NlPhichCfg));
   cmMemcpy((U8 *)(&cellCb->prachCfg), 
           (U8 *)(&cellCfg->prachCfgInfo), sizeof(NlPrachCfg));

   nlDlNpValueInit(cellCb);
   RETVALUE(ROK);
}

/**
 * @brief Selects the Prach Configuration to be applied for the serving cell
 *
 * @details
 *
 *     Function : nlCellSelectPrach
 *
 *     This function compute all the root sequence index's used by neighbor
 *     cells and pick a root sequence index from the serving cell prach
 *     rootSequenceLst.
 *
 *  @param[in]  evtMsg   : neighbor cell config structure
 *  @param[in]  cellCb   : cell control block
 *  @param[in]  alarm    : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PUBLIC S16 nlCellSelectPrach
(
NlCellCb        *cellCb,       /* cell control block*/
NlEventMsg      *evtMsg,       /* neighbor cell config structure */
NlAlarm         *alarm         /* alarm structure*/
)
#else
PUBLIC S16 nlCellSelectPrach(evtMsg, cellCb, alarm)
(
NlCellCb        *cellCb;       /* cell control block*/
NlEventMsg      *evtMsg;       /* neighbor cell config structure */
NlAlarm         *alarm;        /* alarm structure*/
)
#endif
{
   TRC3(nlCellSelectPrach);
   switch(nlCb.genCfg.prachSelType)
   {
      case NL_PRACH_SEL_DISABLE :
      {
         cellCb->prachCfg.servRootSeqIdx = cellCb->prachCfg.rootSeqIdx[0];
         cellCb->prachCfg.servPrachCfgIdx = cellCb->prachCfg.prachConfigIdx[0];
         cellCb->prachCfg.servZeroCorrelationZoneCfg =
                 cellCb->prachCfg.zeroCorrelationZoneCfg[0];
         cellCb->prachCfg.servPrachFreqOffset = cellCb->prachCfg.freqOffset[0];
         cellCb->prachCfg.highSpeedFlag = cellCb->prachCfg.highSpeedFlag;
#ifndef L2_L3_SPLIT
         RLOG4(L_INFO, "SON selected PRACH Cfg [RootSeqIdx: %d] [PrachCfgIdx: %d] "
                       "[ZeroZoneCfg: %d] [FreqOffset: %d]",
               cellCb->prachCfg.servRootSeqIdx, cellCb->prachCfg.servPrachCfgIdx, 
               cellCb->prachCfg.servZeroCorrelationZoneCfg, cellCb->prachCfg.servPrachFreqOffset);
#endif
         RLOG1(L_INFO, "SON Selected PRACH Cfg [HighSpeedFlag: %d]",cellCb->prachCfg.highSpeedFlag);
         nlSendPrachParamUpdateInd(cellCb);
         RETVALUE(ROK);
      }
      break;
      case NL_PRACH_SEL_DSON :
      {
         if(ROK == nlSelPrachInfoByDSon(cellCb, evtMsg, alarm))
         {
#ifndef L2_L3_SPLIT
            RLOG4(L_INFO, "SON selected PRACH Cfg [RootSeqIdx: %d] [PrachCfgIdx: %d] "
                       "[ZeroZoneCfg: %d] [FreqOffset: %d]",
               cellCb->prachCfg.servRootSeqIdx, cellCb->prachCfg.servPrachCfgIdx,
               cellCb->prachCfg.servZeroCorrelationZoneCfg, cellCb->prachCfg.servPrachFreqOffset);
#endif
            RLOG1(L_INFO, "SON Selected PRACH Cfg [HighSpeedFlag: %d]",cellCb->prachCfg.highSpeedFlag);
            nlSendPrachParamUpdateInd(cellCb);
            RETVALUE(ROK);
         }
      }
      break;
      case NL_PRACH_SEL_CSON :
      {


      }
      default :
      break;
   }
   RETVALUE(RFAILED);
}
/*
 *
 *     @brief Calculate total REGs, given a bandwidth, CFI and Antenna.
 *
 *     @details
 *
 *     Function : nlCalcTotalResElemGrps
 *
 *     Desc : Calculate total REGs, given a bandwidth, CFI
 *            and number of antennas.
 *
 *     Ret  : Total REGs (U16)
 *
 **/
#ifdef ANSI
PRIVATE U16 nlCalcTotalResElemGrps
(
U8    bw,
U8    cfi,
U8    numAntna,
Bool  isEcp
)
#else
PRIVATE U16 nlCalcTotalResElemGrps(bw, cfi, numAntna, isEcp)
U8    bw;
U8    cfi;
U8    numAntna;
Bool  isEcp;
#endif
{
   U16 regs = 0;
   TRC2(nlCalcTotalResElemGrps);


   if (bw <= 10)
      ++cfi;

   switch (cfi)
   {
      /* Refer 36.211 section 6.10.1.2
       * For symbols 2 and 4, the REGs per RB will be based on cyclic prefix
       *  and number of antenna ports.
       * For symbol 1, there are 2 REGs per RB always. Similarly symbol 3
       * will have 3 REGS.
       */
      case 4:
         if(isEcp)
         {
            regs = bw * NL_NUM_REGS_4TH_SYM_EXT_CP;
         }
         else
         {
            regs = bw * NL_NUM_REGS_4TH_SYM_NOR_CP;
         }

      case 3:
         regs += bw * NL_NUM_REGS_3RD_SYM;

      case 2:
         regs += (numAntna == NL_NUM_ANT_PORT_FOUR) ? \
            (bw * NL_NUM_REGS_2ND_SYM_FOUR_ANT_PORT) : \
            (bw * NL_NUM_REGS_2ND_SYM_1OR2_ANT_PORT);

      default: /* case 1 */
         regs += bw * NL_NUM_REGS_1ST_SYM;
   }

   RETVALUE(regs);
}


/*
 *
 *     @Brief: Calculates number of PHICH REGs
 *
 *     @detais: Calculates number of PHICH REGs
 *
 *     @param[in]  U8      bw
 *     @param[in]  U8      ng6
 *
 *     Ret  : Number of PHICH REGs (U8)
 ***/
#ifdef ANSI
PRIVATE U16 nlCalcPhichResElemGrps
(
U8 bw,
U8 ng6
)
#else
PRIVATE U16 nlCalcPhichResElemGrps(bw, ng6)
U8 bw;
U8 ng6;
#endif
{
   TRC2(nlCalcPhichResElemGrps);
   RETVALUE(NL_CEIL((bw * ng6) ,(8 * 6)) * NL_NUM_REG_PER_PHICH_GRP);
}


/**
 * @brief Calculates total CCEs (N_cce)
 *
 * @details
 *
 *     Function: nlCalcNumCce
 *     Purpose:  This function calculates and returns total CCEs for a
 *               cell, given the following: bandwidth, Ng configuration
 *               (multiplied by six), cfi (actual number of control
 *               symbols) and number of antennas.
 *
 *  @param[in]  U8      bw
 *  @param[in]  U8      ng6
 *  @param[in]  U8      cfi
 *  @param[in]  U8      mPhich
 *  @param[in]  U8      numAntna
 *  @param[in]  Bool    isEcp
 *  @return     N_cce (U8)
 *
 **/
#ifdef ANSI
PRIVATE U8 nlCalcNumCce
(
U8            bw,
NlPhichNg     ng,
U8            cfi,
U8            mPhich,
U8            numAntna,
Bool          isEcp
)
#else
PRIVATE U8 nlCalcNumCce(bw, ng, cfi, mPhich, numAntna, isEcp)
U8             bw;
NlPhichNg      ng;
U8             cfi;
U8             mPhich;
U8             numAntna;
Bool           isEcp;
#endif
{
   U16         totalRegs;
   U16         phichRegs;
   U16         cceRegs;
   U8          ng6;
   TRC2(nlCalcNumCce);

   switch (ng)
   {
      case NL_NG_ONESIXTH:
         ng6 = 1;
         break;
      case NL_NG_HALF:
         ng6 = 3;
         break;
      case NL_NG_ONE:
         ng6 = 6;
         break;
      case NL_NG_TWO:
      default:
         ng6 = 12;
         break;
   }

   totalRegs = nlCalcTotalResElemGrps(bw, cfi, numAntna, isEcp);
   phichRegs = nlCalcPhichResElemGrps(bw, ng6);

   RLOG2(L_INFO,"TotalRegs is %d and total PhichRegs is %d", totalRegs,
         phichRegs);

   if(mPhich != 0xff) /* Mode TDD */
   {
      cceRegs   = totalRegs - mPhich*phichRegs - NL_NUM_PCFICH_REG;
   }
   else
   {
      cceRegs   = totalRegs - phichRegs - NL_NUM_PCFICH_REG;
   }

   RETVALUE((U8)(cceRegs/NL_NUM_REG_PER_CCE));
}



/**
 * @brief Sort the Array of Zero correlation Zone Cfg values 
 *
 * @details
 *
 *     Function : nlPrachSortZeroCorrZoneCfg
 *
 *  Sort the Array of Zero correlation Zone Cfg values in Ascending Order
 *
 *  @param[in]  cellCb   : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlPrachSortZeroCorrZoneCfg
(
U8          *zeroCorrZoneCfgArr,       /* Array of zeroCorrZoneCfgArr */
U8          numZeroCorrZoneCfg         /* Number of element in the array */
)
#else
PRIVATE S16 nlPrachSortZeroCorrZoneCfg(evtMsg, cellCb)
(
U8          *zeroCorrZoneCfgArr;       /* Array of zeroCorrZoneCfgArr */
U8          numZeroCorrZoneCfg;        /* Number of element in the array */
)
#endif
{
   U8 tmpZeroCorrZoneCfgVal;
   U8 tmp1, tmp2;

   for(tmp1= 0; tmp1 < numZeroCorrZoneCfg; tmp1++)
   {
      tmpZeroCorrZoneCfgVal = zeroCorrZoneCfgArr[tmp1];
      for(tmp2=tmp1; 
          tmp2>0 && zeroCorrZoneCfgArr[tmp2-1] > tmpZeroCorrZoneCfgVal;
          tmp2--)
      {
         zeroCorrZoneCfgArr[tmp2] = zeroCorrZoneCfgArr[tmp2-1];
      }

      zeroCorrZoneCfgArr[tmp2] = tmpZeroCorrZoneCfgVal;
   }
   RETVALUE(ROK);
}


/**
 * @brief Sort the Array of PRACH Freq Offset Cfg values 
 *
 * @details
 *
 *     Function : nlPrachSortFreqOffsetCfgVal
 *
 *  Sort the Array of Prach Freq Offset Cfg values in Ascending Order
 *
 *  @param[in]  cellCb   : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlPrachSortFreqOffsetCfgVal
(
U8          *prachFreqCfgArr,       /* Array of prachFreqOffset */
U8          numPrachFreqOffset      /* Number of element in the array */
)
#else
PRIVATE S16 nlPrachSortFreqOffsetCfgVal(evtMsg, cellCb)
(
U8          *prachFreqCfgArr;       /* Array of prachFreqOffset */
U8          numPrachFreqOffset;     /* Number of element in the array */
)
#endif
{
   U8 tmpPrachFreqOffsetCfgVal;
   U8 tmp1, tmp2;

   for(tmp1= 0; tmp1 < numPrachFreqOffset; tmp1++)
   {
      tmpPrachFreqOffsetCfgVal = prachFreqCfgArr[tmp1];
      for(tmp2=tmp1; 
          tmp2>0 && prachFreqCfgArr[tmp2-1] > tmpPrachFreqOffsetCfgVal;
          tmp2--)
      {
         prachFreqCfgArr[tmp2] = prachFreqCfgArr[tmp2-1];
      }

      prachFreqCfgArr[tmp2] = tmpPrachFreqOffsetCfgVal;
   }
   RETVALUE(ROK);
}


/**
 * @brief To get 'p' value from nCCE.
 *
 * @details
 *
 *     Function: nlGetPValFrmCCE
 *     Purpose:  Gets 'p' value for HARQ ACK/NACK reception from CCE.
 *
 *  @param[in]  NlCellCb     *cell
 *  @param[in]  U8            cce
 *  @return U8
 *
 **/
#ifdef ANSI
PRIVATE U8  nlGetPValFrmCCE
(
NlCellCb *cell,
U8        cce
)
#else
PRIVATE U8  nlGetPValFrmCCE(cell, cce)
NlCellCb *cell;
U8        cce;
#endif
{
   U8 i;
   TRC2(nlGetPValFrmCCE);

   for(i=1; i < NL_TDD_MAX_P_PLUS_ONE_VAL; i++)
   {
      if(cce < cell->pucchResCfg.mappingNpwithP[i])
      {
         RETVALUE(i-1);
      }
   }

   RETVALUE(0);
}


/**
 * @brief Selects the Prach Frequency Offset for the serving cell.
 *
 * @details
 *
 *     Function : nlPrachSelPrachFreqOffsetByDSon
 *
 *  Selects the Prach Freq Offset for the serving cell based on CFI, N1Pucch
 *  an N2Pucch resources configured for the serving cell
 *
 *  @param[in]  cellCb   : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlPrachSelPrachFreqOffsetByDSon
(
NlCellCb        *cellCb       /* cell control block*/
)
#else
PRIVATE S16 nlPrachSelPrachFreqOffsetByDSon(evtMsg, cellCb)
(
NlCellCb        *cellCb;       /* cell control block*/
)
#endif
{
   U8  c        = 3;
   U8  cfi      = 3;
   U8  n2Rb     = cellCb->pucchResCfg.pucchCqiResIdx;
   U8  pucchDeltaShft = cellCb->pucchResCfg.deltaShift;
   U16 n1Pucch  = cellCb->pucchResCfg.n1PucchAn;
   U8  n1Cs     = cellCb->pucchResCfg.cyclicShift;

   U8  n1PerRb;
   U8  totalCce;
   /*kwork fix*/
   U16 n1Max = 100;
   U8  n1Rb;
   U8  mixedRb;
   U8  exclRb; /* RBs to exclude */
   U8  n1RbPart;
   U8  prachRbStart;
   U8  cntFreqOff;
   U8 preFreqOffset = 0;
   U8  cntGr8rThnBw = 0;

   U8 numFreqOffset = cellCb->prachCfg.numFreqOffset;
   U8 P = 0xff;
   U8 n1PlusOne = 0xff;
   U8 mi = 0xff;
   /* Maximum value of M as per Table 10.1-1 */
   U8  M[7] = {1, 2, 4, 3, 4, 9, 1};
 
   TRC3(nlPrachSelPrachFreqOffsetByDSon);

   if (cellCb->isCpUlExtend)
   {
      c = 2;
   }

   n1PerRb  = c * 12 / pucchDeltaShft; /* 12/18/36 */


   if(cellCb->duplexMode == NL_MODE_TDD)
   {
      /* Considering the max no. of CCEs for PUSCH BW calculation based on min
       * mi value */
      if(cellCb->subFrameCfg == 0 || cellCb->subFrameCfg == 6)
      {
         mi = 1;
      }
      else
      {
         mi = 0;
      }
   }

   /* Calculate total num of CCE's for CFI = 3 */
   if(cellCb->duplexMode == NL_MODE_TDD)
   {
      totalCce = nlCalcNumCce(cellCb->bandWidth, 
                              cellCb->phichCfg.ngEnum, 
                              cfi,
                              mi,
                              cellCb->numTxAntPorts,
                              cellCb->isCpUlExtend);

      P = nlGetPValFrmCCE(cellCb, totalCce -1);
      n1PlusOne = cellCb->pucchResCfg.mappingNpwithP[P + 1];
      n1Max    = (M[cellCb->subFrameCfg] -1)*n1PlusOne + n1Pucch + totalCce-1;
   }
   else
   {
      totalCce = nlCalcNumCce(cellCb->bandWidth, 
                              cellCb->phichCfg.ngEnum, 
                              cfi,
                              mi,
                              cellCb->numTxAntPorts,
                              cellCb->isCpUlExtend);

      n1Max    = n1Pucch + totalCce-1;
   }
   

   /* ccpu00129978- MOD- excluding RBs based on formula in section 5.4.3 in 
    * TS 36.211  */
   n1RbPart = (c*n1Cs)/pucchDeltaShft;
   n1Rb = (U8)((n1Max - n1RbPart) / n1PerRb);
   mixedRb = NL_CEIL(n1Cs, 8); /* same as 'mixedRb = n1Cs ? 1 : 0' */
   RLOG3(L_INFO,"n1RbPart is %d, n1Rb %d, mixedRb %d", n1RbPart, n1Rb,
         mixedRb);

   /* get the total Number of RB's to be excluded for PUSCH */
   /* ccpu00137339 */
   if(n1Pucch < n1RbPart)
   {
      exclRb = n2Rb;
   }
   else
   {
      exclRb = n2Rb + mixedRb + n1Rb; /* RBs to exclude */
   }

   RLOG1(L_INFO,"exclRb is %d", exclRb);
   prachRbStart = exclRb/2 + 1;
   RLOG1(L_INFO,"prachRbStart is %d\n",prachRbStart);

   nlPrachSortFreqOffsetCfgVal(&(cellCb->prachCfg.freqOffset[0]),
                               numFreqOffset);

   for(cntFreqOff=0; cntFreqOff<numFreqOffset; cntFreqOff++)
   {
      if((cellCb->prachCfg.freqOffset[cntFreqOff] >= prachRbStart) &&
         (cellCb->prachCfg.freqOffset[cntFreqOff] < cellCb->bandWidth))
      {
         cellCb->prachCfg.servPrachFreqOffset =
            cellCb->prachCfg.freqOffset[cntFreqOff];
         RLOG1(L_INFO,"Selected PRACH Freq Offset value is %d",
               cellCb->prachCfg.servPrachFreqOffset);
         RETVALUE(ROK);
      }
      else
      {
         cntGr8rThnBw++;
      }
   }

   if(cntGr8rThnBw == numFreqOffset)
   {
      U8 tmpPrachFreqOffset = 0;
      RLOG0(L_INFO,"All PRACH Freq Offsets have startRB > Total DL BW");
      for(cntFreqOff=0; cntFreqOff<numFreqOffset; cntFreqOff++)
      {
         if(cellCb->prachCfg.freqOffset[cntFreqOff] < prachRbStart)
         {
            if(cellCb->prachCfg.freqOffset[cntFreqOff] > tmpPrachFreqOffset)
            {
               tmpPrachFreqOffset = cellCb->prachCfg.freqOffset[cntFreqOff];
            }
         }
      }
      preFreqOffset = tmpPrachFreqOffset;
      cellCb->prachCfg.servPrachFreqOffset = preFreqOffset;
      RLOG1(L_INFO,"Selected PRACH Freq Offset value is %d",
            cellCb->prachCfg.servPrachFreqOffset);
      RETVALUE(ROK);
   }

   for(cntFreqOff=0; cntFreqOff<numFreqOffset; cntFreqOff++)
   {
      if(cellCb->prachCfg.freqOffset[cntFreqOff] >= prachRbStart)
      {
         break;
      }
      preFreqOffset = cellCb->prachCfg.freqOffset[cntFreqOff];
   }
      
   cellCb->prachCfg.servPrachFreqOffset = preFreqOffset;

   RLOG1(L_INFO,"Selected PRACH Freq Offset value is %d",
         cellCb->prachCfg.servPrachFreqOffset);

   RETVALUE(ROK);
}


/**
 * @brief Exclude few Prach Configuration Index for the serving cell.
 *
 * @details
 *
 *     Function : nlPrachExcludePrachCfgIndxByDSon
 *
 *  Exclude PRACH configuration indexes as per 5.1.7-2 and 5.1.7-3 from 36.211
 *
 *  @param[in]  evtMsg   : neighbor cell config structure
 *  @param[in]  cellCb   : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlPrachExcludePrachCfgIndxByDSon
(
NlCellCb        *cellCb,       /* cell control block*/
U8              prachCfgIdx    /* PRACH configuration Index */

)
#else
PRIVATE S16 nlPrachExcludePrachCfgIndxByDSon(evtMsg, cellCb)
(
NlCellCb        *cellCb;       /* cell control block*/
U8              prachCfgIdx;   /* PRACH configuration Index */
)
#endif
{
   if(cellCb->duplexMode == NL_MODE_TDD)
   {
      if(prachCfgIdx >= 58)
      {
         RETVALUE(RFAILED);
      }
   }
   else
   {
      if(prachCfgIdx == 30 || prachCfgIdx == 46 || prachCfgIdx == 60 ||
         prachCfgIdx == 61 || prachCfgIdx == 62)
      {
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}


/**
 * @brief Selects the Prach Configuration Index for the serving cell.
 *
 * @details
 *
 *     Function : nlPrachSelPrachCfgIndxByDSon
 *
 *  Selects the Prach Configuration Index for the serving cell. Updates the
 *  PrachCfgIdxList in such a way that the first index of this list will have
 *  the selected value.
 *
 *  @param[in]  evtMsg   : neighbor cell config structure
 *  @param[in]  cellCb   : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlPrachSelPrachCfgIndxByDSon
(
NlEventMsg      *evtMsg,       /* neighbor cell config structure */
NlCellCb        *cellCb        /* cell control block*/
)
#else
PRIVATE S16 nlPrachSelPrachCfgIndxByDSon(evtMsg, cellCb)
(
NlEventMsg      *evtMsg;       /* neighbor cell config structure */
NlCellCb        *cellCb;       /* cell control block*/
)
#endif
{
   U8 tmpIdxPrachCfgIdxLst;
   U8 *prachCfgIdx = &(cellCb->prachCfg.prachConfigIdx[0]); 
   U8 minPrachCfgIdx = cellCb->prachCfg.prachConfigIdx[0];
   Bool prachCfgIndexSelectable = FALSE;

   TRC3(nlPrachSelPrachCfgIndxByDSon);

   for(tmpIdxPrachCfgIdxLst = 1; 
       tmpIdxPrachCfgIdxLst < cellCb->prachCfg.numPrachCfgIdx;
       tmpIdxPrachCfgIdxLst ++)
   {
      if(prachCfgIdx[tmpIdxPrachCfgIdxLst] <= minPrachCfgIdx)
      {
         if(nlPrachExcludePrachCfgIndxByDSon(cellCb, prachCfgIdx[tmpIdxPrachCfgIdxLst]) == ROK)
         {
            prachCfgIndexSelectable = TRUE;
            minPrachCfgIdx = prachCfgIdx[tmpIdxPrachCfgIdxLst];
         }
      }
   }

   /* Thsi is for the case where the first index of PrachCfgIdx is the minimum */
   if(nlPrachExcludePrachCfgIndxByDSon(cellCb, minPrachCfgIdx) == ROK)
   {
      prachCfgIndexSelectable = TRUE;
   }


   if(prachCfgIndexSelectable)
   {
      cellCb->prachCfg.servPrachCfgIdx = minPrachCfgIdx;
      RLOG1(L_INFO,"Selected PRACH Config Index is %d\n", cellCb->prachCfg.servPrachCfgIdx);
   }
   else
   {
      RLOG0(L_INFO,"PRACH Cfg Index NOT Selectable\n");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/**
 * @brief Returns the Preamble format.
 *
 * @details
 *
 *     Function : nlFillPreambleFormat
 *
 *  Returns the Preamble format for the selected Prach Cfg Index
 *
 *  @param[in]  cellCb   : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE U8 nlFillPreambleFormat
(
NlCellCb        *cellCb        /* cell control block*/
)
#else
PRIVATE U8 nlFillPreambleFormat(cellCb)
(
NlCellCb        *cellCb;       /* cell control block*/
)
#endif
{
   U8 preambleFormat = 0;
   if(cellCb->duplexMode == NL_MODE_TDD)
   {
      if(cellCb->prachCfg.servPrachCfgIdx >= 48)
      {
         preambleFormat = 4;
      }
      else if(cellCb->prachCfg.servPrachCfgIdx >= 40)
      {
         preambleFormat = 3;
      }
      else if(cellCb->prachCfg.servPrachCfgIdx >= 30)
      {
         preambleFormat = 2;
      }
      else if(cellCb->prachCfg.servPrachCfgIdx >= 20)
      {
         preambleFormat = 1;
      }
      else
      {
         preambleFormat = 0;
      }
   }
   else
   {
      if(cellCb->prachCfg.servPrachCfgIdx >= 48)
      {
         preambleFormat = 3;
      }
      else if(cellCb->prachCfg.servPrachCfgIdx >= 32)
      {
         preambleFormat = 2;
      }
      else if(cellCb->prachCfg.servPrachCfgIdx >= 16)
      {
         preambleFormat = 1;
      }
      else
      {
         preambleFormat = 0;
      }
   }

   RLOG2(L_INFO,"Preamble format chosen is %d for PrachCfgIdx %d\n", preambleFormat, cellCb->prachCfg.servPrachCfgIdx);

   RETVALUE(preambleFormat);
}

/**
 * @brief Selects the Prach Configuration Index for the serving cell.
 *
 * @details
 *
 *     Function : nlPrachSelZeroCorrZoneCfgByDSon
 *
 *  Selects the Zero correlation zone config for the serving cell. Selects a
 *  Zero correlation zone config value which is equal to 10 or nearest to 10.
 *
 *  @param[in]  evtMsg   : neighbor cell config structure
 *  @param[in]  cellCb   : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlPrachSelZeroCorrZoneCfgByDSon
(
NlEventMsg      *evtMsg,       /* neighbor cell config structure */
NlCellCb        *cellCb        /* cell control block*/
)
#else
PRIVATE S16 nlPrachSelZeroCorrZoneCfgByDSon(evtMsg, cellCb)
(
NlEventMsg      *evtMsg;       /* neighbor cell config structure */
NlCellCb        *cellCb;       /* cell control block*/
)
#endif
{
   U8 numZeroCorrZoneCfg, cntZeroCorrZoneCfg = 0;
   U8 valLessrThan10nClosestTo10 = 0;
   U8 preambleFormat = 0;
   U8 valLessrThanOrEqTo5nClosestTo5 = 0xFF;

   TRC3(nlPrachSelZeroCorrZoneCfgByDSon);
   numZeroCorrZoneCfg = cellCb->prachCfg.numZeroCorrCfgZone;
   /*kwork fixes*/
   if(LNL_MAX_ZERO_CORR_CFG_ZONE < numZeroCorrZoneCfg)
   {
      RLOG0(L_ERROR, "Invalid number of ZeroCorelationZone has congiured");
      RETVALUE(RFAILED);
   }   
   /* 1. Sort zero correlation zone cfg list received as part of serving cell
    * configuration in ascending order */
   nlPrachSortZeroCorrZoneCfg(&(cellCb->prachCfg.zeroCorrelationZoneCfg[0]),
                              numZeroCorrZoneCfg);

   preambleFormat = nlFillPreambleFormat(cellCb);

   for(cntZeroCorrZoneCfg = 0; 
       cntZeroCorrZoneCfg < numZeroCorrZoneCfg;
       cntZeroCorrZoneCfg++)
   {
      if(preambleFormat != 4)
      {
         if(10 == cellCb->prachCfg.zeroCorrelationZoneCfg[cntZeroCorrZoneCfg])
         {
            cellCb->prachCfg.servZeroCorrelationZoneCfg = 10;
            RETVALUE(ROK);
         }
         if(cellCb->prachCfg.zeroCorrelationZoneCfg[cntZeroCorrZoneCfg] > 10)
         {
            cellCb->prachCfg.servZeroCorrelationZoneCfg =
               cellCb->prachCfg.zeroCorrelationZoneCfg[cntZeroCorrZoneCfg];
            RETVALUE(ROK);
         }
         else
         {
            if(valLessrThan10nClosestTo10 <=
                  cellCb->prachCfg.zeroCorrelationZoneCfg[cntZeroCorrZoneCfg])
            {
               valLessrThan10nClosestTo10 =
               cellCb->prachCfg.zeroCorrelationZoneCfg[cntZeroCorrZoneCfg];
            }
         }
      }
      else
      {
         /* Select a value closest to <= 5 */
         if(cellCb->prachCfg.zeroCorrelationZoneCfg[cntZeroCorrZoneCfg] <= 5)
         {
            valLessrThanOrEqTo5nClosestTo5 = 
             cellCb->prachCfg.zeroCorrelationZoneCfg[cntZeroCorrZoneCfg];
         }
      }
   }

   if(preambleFormat != 4)
   {
      cellCb->prachCfg.servZeroCorrelationZoneCfg = valLessrThan10nClosestTo10;
   }
   else
   {
      cellCb->prachCfg.servZeroCorrelationZoneCfg = valLessrThanOrEqTo5nClosestTo5;
   }

   if(cellCb->prachCfg.servZeroCorrelationZoneCfg == 0xFF)
   {
      RLOG0(L_INFO,"All configured values for ZeroCorrelationZoneCfg are not selectable");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/**
 * @brief Checks if the given root sequence Index is unique among neighbor
 * root sequence Index's
 *
 * @details
 *
 *     Function : nlCheckIfRootSeqIndexIsUnique
 *
 *  Checks if the given root sequence Index is unique among neighbor
 *  root sequence Index's
 *
 *  @param[in]  rootSeqIdx  : root sequence Index
 *  @param[in]  nghCellCfg  : neighbor cell config structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlCheckIfRootSeqIndexIsUnique
(
NlTmpNghCellCfg   *nghCellCfg,       /* neighbor cell config structure */
U32               rootSeqIdx         /* Root Sequence Index */
)
#else
PRIVATE S16 nlCheckIfRootSeqIndexIsUnique(rootSeqIdx, nghCellCfg)
(
NlTmpNghCellCfg   *nghCellCfg;       /* neighbor cell config structure */
U32               rootSeqIdx;        /* Root Sequence Index */
)
#endif
{
   U32 numNghEnb, numCells;
   U32 selectedRootSeqIndex = rootSeqIdx;

   TRC3(nlCheckIfRootSeqIndexIsUnique);
 
   for(numNghEnb = 0; numNghEnb < nghCellCfg->numNghEnb; numNghEnb++)
   {
      NlTmpNghCellAdd *nghCellAdd;
      nghCellAdd =
         &(nghCellCfg->nghEnbCellCfg[numNghEnb].nghCellParam.u.nghCellAdd);

      for(numCells = 0; numCells < nghCellAdd->numNgh; numCells++)
      {
         NlCellInfo *nghCellInfo = NULLP;
         nghCellInfo = &(nghCellAdd->nghCellAddInfo[numCells].nghCellInfo);
         if(nghCellInfo->isPrachCfg)
         {
            U8  numRootSeqIndex;
            U32 tmpCounter;
            numRootSeqIndex = nghCellInfo->prachCfg.numRootSeqIndex;

            for(tmpCounter = 0; 
                tmpCounter < numRootSeqIndex;
                tmpCounter ++)
            {
               if(selectedRootSeqIndex ==
                     nghCellInfo->prachCfg.rootSeqIdx[tmpCounter])
               {
                  RLOG2(L_ERROR,
                  "Selected RootSeqIdx %d is not unique. Conflict with"
                  "Neighbor with PCI %d", selectedRootSeqIndex,nghCellInfo->pci);

                  RETVALUE(FALSE);
               }
            }
         }
      }
   }

   RETVALUE(TRUE);
}

/**
 * @brief Builds the List of root sequence indexes used for Neighbor cell
 *
 * @details
 *
 *     Function : nlBldNghRootSeqIndexs
 *
 *     This function compute all the root sequence index's used by neighbor
 *     cell.
 *
 *  @param[in]  cellCb      : cell control block
 *  @param[in]  nghCellAdd  : neighbor cell config structure
 *  @param[in]  alarm       : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlBldNghRootSeqIndexs
(
NlCellCb         *cellCb,       /* cell control block*/
NlTmpNghCellAdd  *nghCellAdd,   /* neighbor cell configuration structure*/
NlAlarm          *alarm         /* alarm structure*/
)
#else
PRIVATE S16 nlBldNghRootSeqIndexs(evtMsg, cellCb, alarm)
(
NlCellCb         *cellCb;       /* cell control block*/
NlTmpNghCellAdd  *nghCellAdd;   /* neighbor cell configuration structure*/
NlAlarm          *alarm;        /* alarm structure*/
)
#endif
{
   U32 numCells;
   NlCellInfo *cellInfo;

   TRC3(nlBldNghRootSeqIndexs);

   for(numCells = 0; numCells < nghCellAdd->numNgh; numCells++)
   {
      U32 zeroCorrelationCfgZone;
      U8  highSpeedFlag;
      U32 startRootSequenceIndex;
      cellInfo = &(nghCellAdd->nghCellAddInfo[numCells].nghCellInfo);

      zeroCorrelationCfgZone =
         cellInfo->prachCfg.servZeroCorrelationZoneCfg;
      highSpeedFlag = cellInfo->prachCfg.highSpeedFlag;
      startRootSequenceIndex = cellInfo->prachCfg.servRootSeqIdx;

      if(highSpeedFlag == FALSE)
      {
         U32 tmpCntRootSeq;
         U16 numRootSeqIndexPerCv =
         zero_correlation_config_table[zeroCorrelationCfgZone].numRootSeqIndex;
      
         /* The first root sequence index starts with the one configured
          * for that cell */
         cellInfo->prachCfg.rootSeqIdx[0] = startRootSequenceIndex;

         /* Populate rest of the rootSequenceIndex in the array list */
         for(tmpCntRootSeq = 1; 
             tmpCntRootSeq < numRootSeqIndexPerCv; 
             tmpCntRootSeq ++)
         {
            cellInfo->prachCfg.rootSeqIdx[tmpCntRootSeq] =
               startRootSequenceIndex + tmpCntRootSeq;

            /* For the case of wrap-around, 0 is a continuation of 837.
             * Refer 36.211 section 5.7.2. */
            if(cellInfo->prachCfg.rootSeqIdx[tmpCntRootSeq] > 837)
            {
               cellInfo->prachCfg.rootSeqIdx[tmpCntRootSeq] =
                  cellInfo->prachCfg.rootSeqIdx[tmpCntRootSeq] % 838;
            }
         }

         cellInfo->prachCfg.numRootSeqIndex = numRootSeqIndexPerCv;
      }
      else
      {
         /* TBD: Yet to fill rootSeqIdx for restricted set */
      }
   }

   RETVALUE(TRUE);
}


/**
 * @brief Selects the root Sequence Index for the serving cell
 *
 * @details
 *
 *     Function : nlPrachSelRootSeqIdxByDSon
 *
 *     This function Selects the root Sequence Index for the serving cell. It
 *     loops through all the neighbor root Sequence Index and checks if any from
 *     the list of rootSequenceIndex available for Serving cell can be
 *     applied. 
 *
 *  @param[in]  evtMsg   : neighbor cell config structure
 *  @param[in]  cellCb   : cell control block
 *  @param[in]  alarm    : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlPrachSelRootSeqIdxByDSon
(
NlEventMsg      *evtMsg,       /* neighbor cell config structure */
NlCellCb        *cellCb,       /* cell control block*/
NlAlarm         *alarm         /* alarm structure*/
)
#else
PRIVATE S16 nlPrachSelRootSeqIdxByDSon(evtMsg, cellCb, alarm)
(
NlEventMsg      *evtMsg;       /* neighbor cell config structure */
NlCellCb        *cellCb;       /* cell control block*/
NlAlarm         *alarm;        /* alarm structure*/
)
#endif
{

   NlTmpNghCellCfg   *nghCellCfg = NULLP;   /* neighbor cell config structure */
   U32 selectedRootSeqIndex = 0xffffffff;
   U32 rootSeqIdxCounter, numNghEnb;
   Bool rootSeqIdxSelectable = FALSE;
   Bool rootSeqIndxNotInRange = FALSE;

   TRC3(nlPrachSelRootSeqIdxByDSon);
 
   nghCellCfg     = (NlTmpNghCellCfg *)evtMsg->msg;

   /* 1. Build Neighbor rootSequenceIndex's that will be used by the neighbor
    * cell */
   for(numNghEnb = 0; numNghEnb < nghCellCfg->numNghEnb; numNghEnb++)
   {
      switch(nghCellCfg->nghEnbCellCfg[numNghEnb].nghCellParam.cfgType)
      {
         case NL_NGH_ADD :
         {
            nlBldNghRootSeqIndexs(cellCb, 
                          &(nghCellCfg->nghEnbCellCfg[numNghEnb].
                          nghCellParam.u.nghCellAdd), alarm);
            break; 
         }

         default :
         {
            RLOG0(L_ERROR, "Inavlid neighbor cfg type received");
            RETVALUE(TRUE);
         }
      }
   }

   /* 2. Loop through all the rootSequenceIndex List for serving cell */
   for(rootSeqIdxCounter = 0; 
       rootSeqIdxCounter < cellCb->prachCfg.numRootSeqIndex; 
       rootSeqIdxCounter ++)
   {
      U8 preambleFormat = 0;
      preambleFormat = nlFillPreambleFormat(cellCb);

      /* 3. Select a particular rootSequenceIndex */
      selectedRootSeqIndex = cellCb->prachCfg.rootSeqIdx[rootSeqIdxCounter];

      if(preambleFormat == 4 && selectedRootSeqIndex > 137)
      {
         RLOG1(L_INFO,"Not considering Root sequence Index %d\n", selectedRootSeqIndex);
         rootSeqIndxNotInRange = TRUE;
         continue;
      }

      rootSeqIndxNotInRange = FALSE;
      /* 4. Check if the selected root sequence Index is unique among all the
       * neighbors */
      if(TRUE == nlCheckIfRootSeqIndexIsUnique(nghCellCfg,
                                               selectedRootSeqIndex))
      {
         rootSeqIdxSelectable = TRUE;
         break;
      }
   }

   if(TRUE == rootSeqIdxSelectable)
   {
      /* A Unique root sequence index is found for serving cell. Set that as
       * the root sequnece Index to be used. */
      RLOG2(L_DEBUG, "RootSeqIdx (%d) present in the List location (%d) is"
            "selected", selectedRootSeqIndex, (rootSeqIdxCounter -1));
      cellCb->prachCfg.servRootSeqIdx = selectedRootSeqIndex; 
   }
   else if(rootSeqIndxNotInRange != TRUE)
   {
      /* If no rootSeqIndex is present in the list which is unique among the
       * neighbors, select the rootSeqIndex in the first position from the
       * list */
      cellCb->prachCfg.servRootSeqIdx = cellCb->prachCfg.rootSeqIdx[0]; 
   }
   else
   {
      RLOG0(L_INFO,"Root-Seq Index NOT selectable !!!!!\n");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}



/**
 * @brief This function chooses serving cell prach configuration parameters
 *
 * @details
 *
 *     Function : nlSelPrachInfoByDSon
 *
 *     This function chooses serving cell prach configuration for every
 *     parameter.
 *
 *  @param[in]  evtMsg   : neighbor cell config structure
 *  @param[in]  cellCb   : cell control block
 *  @param[in]  alarm    : alarm structure
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlSelPrachInfoByDSon
(
NlCellCb        *cellCb,       /* cell control block*/
NlEventMsg      *evtMsg,       /* neighbor cell config structure */
NlAlarm         *alarm         /* alarm structure*/
)
#else
PUBLIC S16 nlSelPrachInfoByDSon(evtMsg, cellCb, alarm)
(
NlCellCb        *cellCb;       /* cell control block*/
NlEventMsg      *evtMsg;       /* neighbor cell config structure */
NlAlarm         *alarm;        /* alarm structure*/
)
#endif
{

   TRC3(nlSelPrachInfoByDSon);

   /* Select Prach Configuration Index */
   if(nlPrachSelPrachCfgIndxByDSon(evtMsg, cellCb) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Select Zero Correlation config Zone */
   if(nlPrachSelZeroCorrZoneCfgByDSon(evtMsg, cellCb) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Select Prach Frequency Offset */
   if(nlPrachSelPrachFreqOffsetByDSon(cellCb) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Set HighSpeedFlag = FALSE */
   cellCb->prachCfg.highSpeedFlag = FALSE;

   /* Select a RootSequenceIndex for the serving cell */
   if(nlPrachSelRootSeqIdxByDSon(evtMsg, cellCb, alarm) != ROK)
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/********************************************************************30**

           End of file:     bk_prach_sel.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/3 - Fri Jan  2 16:28:40 2015

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


