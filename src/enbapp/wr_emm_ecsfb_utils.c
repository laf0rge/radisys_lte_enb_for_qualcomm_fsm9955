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
               

     File:     wr_emm_ecsfb_utils.c

     Sid:      fc_emm_ecsfb_utils.c@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:54:21 2014

     Prg:      BSL

*********************************************************************21*/


/* header include files (.h) */
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=38;

#include "wr.h"            /* Common headers and Data Structures */
#include "wr_emm.h"        /* Environment options */
#include "nhu.x" 
#include "nhu_asn.x" 
#include "wr_emm_pbm.h"
#include "wr_emm_pws.h"
#include "wr_utils.h"
#include "wr_ifm_schd.h"
#include "wr_emm_ecsfb_utils.h"
#include "wr_umm.h"
#define WR_EMM_RRC_CELL_ID_LEN         28
#define WR_EMM_RRC_MIB_SPARE_LEN       10
#define WR_EMM_RRC_SIB1_SUBFRM_NO       5 
#define WR_MAX_NUM_SIB1_PLMNS           6

PUBLIC Bool wrUmmChk1xRttCdmaFreqCfgForBand
(
U32        bandClass,
WrCellId   cellId
);

PUBLIC S16 wrEmmChkCsfbToUtraMeasSup
(
WrCellId                    cellId, 
U8                          *csfbUtraMeasSup
);

PUBLIC S16 wrEmmGetCdmaSysTime
(
U8                      offset,
U64                     *sysTym,
U64                     *longcodeState,
Bool                    cdmaEutraTimeSync,
U16                     cdmaTimeOffset
);
PUBLIC S16 wrEmmGetLongCodeState
(
U64                     *longcodeState
);
PUBLIC S16 wrSibBldSIB8IEs
(
 WrCellCb      *cellCb,
 NhuDatReqSdus *sib8Sdu,
 NhuSysInfoBlockTyp8  *Sib8Ie
);
PUBLIC S16 wrEmmPrcSib8SchdEvnt(WrCellCb *cb);
PUBLIC S16 wrEmmGetCdma1xRttTgtCell
(
 TknStrOSXL    **sectorIdPtr,
 WrCellId      cellId
);
PUBLIC S16 wrEmmGetHighestPrioFreq
(
  U32        bandClass,
  U32        *arfcn,
  WrCellId   cellId
);

PUBLIC S16 wrEmmGetHighestPrioBand
(
  WrUeCb                    *ueCb,
  U32                       *band
);

/**
 *  @brief This function fills the structure TknStr4U64
 *
 *      Function: wrFillTknStr64
 *
 *          Processing steps:
 *          - update token present field as TRUE
 *          - fill Bit string of specified length
 *
 *
 *  @param[out] ptr  : TknBStr32 stucture pointer to be updated
 *  @param[in]  len  : length of message to be updated
 *  @param[in]  val  : value getting updated
 *  @param[in]  mem  : pointer to memCp
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 *          RFAILED
 */
PUBLIC S16 wrFillTknBStr64
(
TknStrBSXL                   *ie,
U16                          len,
U64                          val,
CmMemListCp                  *mem
)
{
   U8                        idx;
   U8                        octets[8] = {0};
   U32                       noOcts = (len + 7)/8;

   /* make the element as present */
   if (len > 64)
   {
      RETVALUE(RFAILED);
   }

   WR_GET_MEM(mem, noOcts, &ie->val);
   if (ie->val == NULLP)
   {
      RLOG0(L_ERROR, " wrFillTknBStr64 Failed ");
      RETVALUE(RFAILED);
   }

   octets[0] = ((val << (64 - len)) & 0xff00000000000000) >> 56;
   octets[1] = ((val << (64 - len)) & 0x00ff000000000000) >> 48;
   octets[2] = ((val << (64 - len)) & 0x0000ff0000000000) >> 40;
   octets[3] = ((val << (64 - len)) & 0x000000ff00000000) >> 32;
   octets[4] = ((val << (64 - len)) & 0x00000000ff000000) >> 24;
   octets[5] = ((val << (64 - len)) & 0x0000000000ff0000) >> 16;
   octets[6] = ((val << (64 - len)) & 0x000000000000ff00) >> 8;
   octets[7] = ((val << (64 - len)) & 0x00000000000000ff);

   ie->len  = len;
   ie->pres = PRSNT_NODEF;

   for(idx = 0; idx < noOcts; idx++)
   {
      ie->val[idx] = octets[idx];
   }

   RETVALUE(ROK);
}

/**
 *  @brief This function calculates the CDMA time by using the current system
 *  time either by GPS or any other function
 *
 *      Function: wrEmmGetCdmaSysTime
 *
 *          Processing steps:
 *          - Gets the current time from either GPS or system time function
 *          - Conver the time in CDMA time format(in terms of 20ms frame)
 *
 *
 *  @param[in]  offset  :  offset to be used while calculating the CDMA time
 *  @param[out] sysTym  :  Calculated CDMA time 
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 *          RFAILED
 */
PUBLIC S16 wrEmmGetCdmaSysTime
(
U8                      offset,
U64                     *sysTym,
U64                     *longCodeState,
Bool                    cdmaEutraTimeSync,
U16                     cdmaTimeOffset  
)
{
   EpcTime sysTymVal = NULLP;

   /* Call SGetEpcTime function which returns the time.
    * which represents the number of milliseconds 
    * elapsed since 00:00 hours, Jan 1, 1970 UTC. */

   SGetEpcTime(&(sysTymVal));

   /* The start of CDMA system time is Jan 6, 1980 00:00:00:00 UTC, 
    * which coincides with the start of GPS time.
    * SGetEpcTime() function returns the time from 00:00 hours, Jan 1, 1970 UTC.
    * Get the time difference from 00:00 hours, Jan 1, 1970 UTC to 
    * Jan 6, 1980 00:00:00:00 UTC in milliseconds*/

    /* From Jan 1, 1970 UTC to Jan 6, 1980 UTC, 
     * Leap years = 2 and non-leap year = 8 and 5 days
     * So calculate the days as - 
     * days = (2*366) + (8*365) and convert into seconds.
     * now decrement the difference from Jan 1 to Jan6 .
     * milliseconds = (days * 24 * 60 * 60 *1000) 
     * and WR_MSEC_DIFF = 315532800000 + 432000000 = 315964800000 */

   sysTymVal = (sysTymVal - WR_MSEC_DIFF);
   sysTymVal = sysTymVal + cdmaTimeOffset;

   *longCodeState = sysTymVal;

   if(cdmaEutraTimeSync) /*TRUE - SYNC */
   {
      *sysTym = sysTymVal / 10;
   }
   else
   {
      /* Calculate 49 bit value by -
       * 49 bit converted value =  current_time_in_ms *   M_8_chip_unit 
       * where 1ms = (1.288 * 10 ^ 3)/8   in 8-chip units
       * offset is the advance time given by the caller of this function. We consider
       * this offset and calculate time considering this offset also in advance */

      *sysTym = (((sysTymVal/1000) * 1228800)/8) + 0;
   }

      RETVALUE(ROK);
}

/**
 *  @brief This function calculates LongCodeState by using the CDMA time in 1ms
 *  unit. This time can be calculated on the basis of system time or GPS
 *
 *      Function: wrEmmGetLongCodeState
 *
 *          Processing steps:
 *          - Use the current time in 1ms unit and use the same to get the value
 *          of LongCodeState using following function of time
 *          LCS = ceiling(t/10)* 10 + 320ms 
 *
 *  @param[out] longCodeState  :  Calculated LongCodeState value 
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 *          RFAILED
 */
PUBLIC S16 wrEmmGetLongCodeState
(
U64                     *numOfCdmaMs
)
{

   F64               ceilValue     = (F64)*numOfCdmaMs/10;
   U64               longCodeState = 0;

   
   longCodeState =  WR_EMM_GET_CEILING_VALUE(ceilValue);
   /* The state of long code generation registers in CDMA2000 1XRTT system 
    *  [t/10]*10 +320ms where t equals to the cdma-SystemTime */
   longCodeState = longCodeState * 10 + 320;
   *numOfCdmaMs = longCodeState;
      RETVALUE(ROK);
}


/** @brief This function fills neighbour cell list of CDMA reselec params.
 *
 * @details
 *
 *     Function: wrSibBldSIB8NeighCellsList
 *
 *         Processing steps:
 *         - Get the white cell lsit from ANNR module and
 *           fill in SIB8
 *
 */
PRIVATE S16 wrSibBldSIB8NeighCellsList
(
   NhuDatReqSdus             *sib8Sdu,
   NhuNeighCellLstCDMA2000   *neighCellList,
   NhuNeighCellLstCDMA2000_v920 *neighCellListv920,
   WrNrWlBlInfo              *nrAllWlBlInfo
 )
{
   WrCDMA2KNeighInfo   *cdma1xWlInfo = NULLP;
   U8                  numBands=0, numFreqs=0, numCells=0;
   U8                  bandIdx=0, freqIdx=0, cellIdx=0;
   NhuNeighCellsPerBandclassLstCDMA2000  *freqList = NULLP;
   NhuNeighCellsPerBandclassLstCDMA2000_v920 *freqListv920 =NULLP;
   NhuPhysCellIdLstCDMA2000              *cellList=NULLP;
   NhuPhysCellIdLstCDMA2000_v920         *cellListv920 =NULLP;

   TRC2(wrSibBldSIB8NeighCellsList);

   cdma1xWlInfo = &nrAllWlBlInfo->cdma1xRtt;
   numBands = cdma1xWlInfo->numBandClass;

   wrFillTknU16(&(neighCellList->noComp),numBands);
   wrFillTknU16(&(neighCellListv920->noComp),numBands);
   /* Allocate memory for Neighbour cell list */
   WR_GET_MEM(sib8Sdu, numBands * sizeof(NhuNeighCellCDMA2000),
      &(neighCellList->member));

   /* Allocate memory for Neighbour cell list v920 */
   WR_GET_MEM(sib8Sdu, numBands * sizeof(NhuNeighCellCDMA2000_v920),
      &(neighCellListv920->member));
   for (bandIdx=0; bandIdx < numBands; bandIdx++ /*numBands++*/)
   {
      wrFillTknU8(&neighCellList->member[bandIdx].pres, PRSNT_NODEF);
      wrFillTknU8(&neighCellListv920->member[bandIdx].pres, PRSNT_NODEF);
      wrFillTknU32(&neighCellList->member[bandIdx].bandClass,
        cdma1xWlInfo->neighBandLst[bandIdx].bandClass);
      
      freqList = &neighCellList->member[bandIdx].neighCellsPerFreqLst;
      numFreqs = cdma1xWlInfo->neighBandLst[bandIdx].numFreq;
      freqListv920 = &neighCellListv920->member[bandIdx].neighCellsPerFreqLst_v920;

      wrFillTknU16(&(freqList->noComp),numFreqs);
      wrFillTknU16(&(freqListv920->noComp),numFreqs);
      WR_GET_MEM(sib8Sdu, numFreqs * sizeof(NhuNeighCellsPerBandclassCDMA2000),
         &(freqList->member));

      WR_GET_MEM(sib8Sdu, numFreqs * sizeof(NhuNeighCellsPerBandclassCDMA2000_v920),
         &(freqListv920->member));
      for (freqIdx=0; freqIdx < numFreqs; freqIdx++)
      {
         wrFillTknU8(&freqList->member[freqIdx].pres, PRSNT_NODEF);
         wrFillTknU8(&freqListv920->member[freqIdx].pres, PRSNT_NODEF);
         wrFillTknU32(&freqList->member[freqIdx].arfcn, 
            cdma1xWlInfo->neighBandLst[bandIdx].neighFreqInfo[freqIdx].arfcn);

         cellList = &freqList->member[freqIdx].physCellIdLst;
         cellListv920 = &freqListv920->member[freqIdx].physCellIdLst_v920;
         numCells = cdma1xWlInfo->neighBandLst[bandIdx].
            neighFreqInfo[freqIdx].wlData.numWlCells;

         /* numCells are more than 16 then fill first 16 PCIs here and
          * remaining 24 fill in the extended list i.. v920 extension */
         if (numCells <= WR_MAX_CDMA_FREQ_PER_BAND)
         {
            wrFillTknU16(&(cellList->noComp),numCells);
         }
         else
         {
            wrFillTknU16(&(cellList->noComp),WR_MAX_CDMA_FREQ_PER_BAND);
         }
         WR_GET_MEM(sib8Sdu,numCells * sizeof(NhuPhysCellIdCDMA2000),
            &(cellList->member));

         for (cellIdx=0; cellIdx < numCells && 
               cellIdx < WR_MAX_CDMA_FREQ_PER_BAND; cellIdx++)
         {
            wrFillTknU32(&cellList->member[cellIdx],
            cdma1xWlInfo->neighBandLst[bandIdx].neighFreqInfo[freqIdx].
            wlData.pciInfoLst[cellIdx].pnOffset);
         }
         /* numCells is 17 cellIdx runs from 0 to 16
          * cellIdx will be 16, 16 < 17 is true
          * if numCells 16, 16 < 16 is false */
         if(cellIdx < numCells) // if true there are PCIs more than 16 
         {
            /* if numCells can not be more than 40 i.e. LWR itself restricting to be maximum 40
             * then there is no need of check for maximum of 24 number of components
             * else here there is a need for check */
            if( (numCells - cellIdx) > WR_MAX_CDMA_1xRTT_CELLS_CDMA2K_V920)
            {
               wrFillTknU16(&(cellListv920->noComp), 
                                    WR_MAX_CDMA_1xRTT_CELLS_CDMA2K_V920);
            }
            else
            {
               wrFillTknU16(&(cellListv920->noComp),numCells - cellIdx);
            }
            WR_GET_MEM(sib8Sdu,
                ((numCells - cellIdx) * sizeof(NhuPhysCellIdLstCDMA2000_v920)),
                  &(cellListv920->member));
            for(;cellIdx<numCells && cellIdx < WR_MAX_CDMA_1xRTT_CELLS;
                  cellIdx++) // allow maximum of additional 24
            {
               wrFillTknU32(&cellListv920->member[cellIdx - WR_MAX_CDMA_1xRTT_CELLS_CDMA2K],
                     cdma1xWlInfo->neighBandLst[bandIdx].neighFreqInfo[freqIdx].
                     wlData.pciInfoLst[cellIdx].pnOffset);
            }
         }
         else
         {
            wrFillTknU16(&(cellListv920->noComp),0);
         }
      }
   }

   RETVALUE(ROK);
}

/** @brief This function fills band class list of CDMA reselec params.
 *
 * @details
 *
 *     Function: wrSibBldSIB8BandClsList
 *
 *         Processing steps:
 *         - Get the Band class list from cellCb and
 *           fill in SIB8
 *
 */
PRIVATE S16 wrSibBldSIB8BandClsList
(
   WrCellCb                          *cellCb,
   NhuDatReqSdus                     *sib8Sdu,
   NhuBandClassLstCDMA2000           *bandClsList
 )
{
   CmLListCp              *bandLst = NULLP;
   U8                     numOfBands=0;
   WrCdmaBandClassCb      *bandNode = NULLP;
   U8                     bandIdx=0;
   CmLList                *bandLnk;

   TRC2(wrSibBldSIB8BandClsList);

   bandLst = &cellCb->cdmaBandLst;
   numOfBands = cellCb->cdmaBandLst.count;

   wrFillTknU16(&(bandClsList->noComp),numOfBands);

   /* Allocate memory for Band class list */
   WR_GET_MEM(sib8Sdu, numOfBands * sizeof(NhuBandClassInfoCDMA2000),
      &(bandClsList->member));

   CM_LLIST_FIRST_NODE(bandLst, bandLnk);

   while(bandLnk != NULLP)
   {
      bandNode = (WrCdmaBandClassCb*)(bandLnk->node);

      wrFillTknU8(&bandClsList->member[bandIdx].pres, PRSNT_NODEF);
      wrFillTknU32(&bandClsList->member[bandIdx].bandClass,bandNode->bandClass);
      wrFillTknU32(&bandClsList->member[bandIdx].cellReselectionPriority, 
         bandNode->cellReselPri);
      wrFillTknU32(&bandClsList->member[bandIdx].threshX_High,
         bandNode->threshxHigh);
      wrFillTknU32(&bandClsList->member[bandIdx].threshX_Low,
         bandNode->threshxLow);
      
      CM_LLIST_NEXT_NODE(bandLst, bandLnk);
      bandIdx++;
   }

   RETVALUE(ROK);
}/* wrSibBldSIB8BandClsList */


/** @brief This function fills CDMA reselec params.
 *
 * @details
 *
 *     Function: wrSibBldSIB81xRttCellReselParams
 *
 */
PRIVATE S16 wrSibBldSIB81xRttCellReselParams
(
 WrCellCb             *cellCb,
 NhuDatReqSdus        *sib8Sdu,
 NhuSysInfoBlockTyp8  *Sib8Ie,
 WrSib8Params         *sib8
 )
{
   WrNrWlBlInfo                      *nrAllWlBlInfo = NULLP;
   NhuCellReselectionParamsCDMA2000  *cellReselParams = NULLP;
   NhuCellReselectionParamsCDMA2000_v920 *cellReselParamsv920 = NULLP;

   TRC2(wrSibBldSIB81xRttCellReselParams);

   cellReselParams = &(Sib8Ie->parameters1XRTT.cellReselectionParams1XRTT);
   cellReselParamsv920 = &(Sib8Ie->extaddgrp_1.cellReselectionParams1XRTT_v920);
   

   /* Get neighbor list info from ANR Module */
   nrAllWlBlInfo = wrEmmAnrGetWhiteAndBlackList(cellCb->cellId, 
       WR_ANR_LST_CDMA_2K_1xRTT, TRUE);
   /* Klockwork fix for ECSFB */
   if (nrAllWlBlInfo == NULLP)
   {
      wrFillTknU8(&(cellReselParams->pres),NOTPRSNT);
   }
   else
   {
      wrFillTknU8(&(cellReselParams->pres),PRSNT_NODEF);
      wrFillTknU8(&(cellReselParamsv920->pres),PRSNT_NODEF);
   }

   if (cellReselParams->pres.val == TRUE)
   {
     /* Band class list */
     wrSibBldSIB8BandClsList(cellCb,sib8Sdu,&cellReselParams->bandClassLst); 
     /* Neighbour cell list */
     wrSibBldSIB8NeighCellsList(sib8Sdu, &cellReselParams->neighCellLst, &cellReselParamsv920->neighCellLst_v920,
        nrAllWlBlInfo); 
     /* T-Reselection */
     wrFillTknU32(&cellReselParams->t_ReselectionCDMA2000,sib8->tReselection);
     /* Speed state scale factors */
     if (sib8->speedStatePres)
     {
        wrFillTknU8(&(cellReselParams->t_ReselectionCDMA2000_SF.pres),
          PRSNT_NODEF);
        wrFillTknU32(&(cellReselParams->t_ReselectionCDMA2000_SF.sf_Medium),
          sib8->sfMedium);
        wrFillTknU32(&(cellReselParams->t_ReselectionCDMA2000_SF.sf_High),
          sib8->sfHigh);
     }
   }
   RETVALUE(ROK);
}

/** @brief This function fills 1xRTT access class barring parameters
 *
 * @details
 *
 *     Function: wrSibBldSIB81xRttAcBarringParams
 *
 *         Processing steps:
 *         - Checks if AC barring parameters are configured and fill in SIB8
 *
 */
PRIVATE S16 wrSibBldSIB81xRttAcBarringParams
(
 WrCellCb             *cellCb,
 NhuDatReqSdus        *sib8Sdu,
 NhuSysInfoBlockTyp8  *Sib8Ie,
 WrSib8Params         *sib8
 )
{
   TRC2(wrSibBldSIB81xRttAcBarringParams);
     if (sib8->XRTTparam.acBarringPres == TRUE)
     {
        wrFillTknU8(&(Sib8Ie->extaddgrp_1.ac_BarringConfig1XRTT_r9.pres),
           PRSNT_NODEF);
        wrFillTknU32(
        &(Sib8Ie->extaddgrp_1.ac_BarringConfig1XRTT_r9.ac_Barring0to9_r9),
        sib8->XRTTparam.acBarring.acBarring0to9_r9);

        wrFillTknU32(
        &(Sib8Ie->extaddgrp_1.ac_BarringConfig1XRTT_r9.ac_Barring10_r9),
        sib8->XRTTparam.acBarring.acBarring10_r9);

        wrFillTknU32(
        &(Sib8Ie->extaddgrp_1.ac_BarringConfig1XRTT_r9.ac_Barring11_r9),
        sib8->XRTTparam.acBarring.acBarring11_r9);

        wrFillTknU32(
        &(Sib8Ie->extaddgrp_1.ac_BarringConfig1XRTT_r9.ac_Barring12_r9),
        sib8->XRTTparam.acBarring.acBarring12_r9);

        wrFillTknU32(
        &(Sib8Ie->extaddgrp_1.ac_BarringConfig1XRTT_r9.ac_Barring13_r9),
        sib8->XRTTparam.acBarring.acBarring13_r9);

        wrFillTknU32(
        &(Sib8Ie->extaddgrp_1.ac_BarringConfig1XRTT_r9.ac_Barring14_r9),
        sib8->XRTTparam.acBarring.acBarring14_r9);

        wrFillTknU32(
        &(Sib8Ie->extaddgrp_1.ac_BarringConfig1XRTT_r9.ac_Barring15_r9),
        sib8->XRTTparam.acBarring.acBarring15_r9);

        wrFillTknU32(
        &(Sib8Ie->extaddgrp_1.ac_BarringConfig1XRTT_r9.ac_BarringMsg_r9),
        sib8->XRTTparam.acBarring.acBarringMsg_r9);

        wrFillTknU32(
        &(Sib8Ie->extaddgrp_1.ac_BarringConfig1XRTT_r9.ac_BarringReg_r9),
        sib8->XRTTparam.acBarring.acBarringReg_r9);

        wrFillTknU32(
        &(Sib8Ie->extaddgrp_1.ac_BarringConfig1XRTT_r9.ac_BarringEmg_r9),
        sib8->XRTTparam.acBarring.acBarringEmg_r9);
     }
   RETVALUE (ROK);
}

/** @brief This function fills 1xRTT CSFB Registration parameters
 *
 * @details
 *
 *     Function: wrSibBldSIB81xRttCsfbRegParams
 *
 *         Processing steps:
 *         - Checks if 1xRtt Reg parameters are configured and fill in SIB8
 *
 */
PRIVATE S16 wrSibBldSIB81xRttCsfbRegParams
(
 WrCellCb             *cellCb,
 NhuDatReqSdus        *sib8Sdu,
 NhuSysInfoBlockTyp8  *Sib8Ie,
 WrSib8Params         *sib8
 )
{
     NhuCSFB_RegistrationParam1XRTT  *csfb1xrttRegParamsIE;
     Wr1xRttCsfbRegInfo              *storedCsfbRegInfo = NULLP;

   TRC2(wrSibBldSIB81xRttCsfbRegParams);

        if(TRUE == sib8->XRTTparam.csfbRegParamPres)
        {
           storedCsfbRegInfo = &sib8->XRTTparam.csfbRegParam;
           csfb1xrttRegParamsIE = 
              &Sib8Ie->parameters1XRTT.csfb_RegistrationParam1XRTT;

           WR_FILL_TKN_UINT(csfb1xrttRegParamsIE->pres, PRSNT_NODEF);

           /* SID */
           WR_FILL_BIT_STR_VAR_LEN(csfb1xrttRegParamsIE->sid,15,
                    storedCsfbRegInfo->sid,sib8Sdu);

           /* NID */
           WR_FILL_BIT_STR_VAL(csfb1xrttRegParamsIE->nid,16,
                    storedCsfbRegInfo->nid,sib8Sdu);
         
           /* Multiple SID */
           wrFillTknU8(&csfb1xrttRegParamsIE->multipleSID,
              storedCsfbRegInfo->mulSID);

           /* Multiple NID */
           wrFillTknU8(&csfb1xrttRegParamsIE->multipleNID,
              storedCsfbRegInfo->mulNID);

           /* HomeReg */
           wrFillTknU8(&csfb1xrttRegParamsIE->homeReg,
              storedCsfbRegInfo->homeReg);

           /* Foreign SID Reg */
           wrFillTknU8(&csfb1xrttRegParamsIE->foreignSIDReg,
              storedCsfbRegInfo->foreignSIDReg);

           /* Foreign NID Reg */
           wrFillTknU8(&csfb1xrttRegParamsIE->foreignNIDReg,
              storedCsfbRegInfo->foreignNIDReg);
  
           /* Parameter Reg */
           wrFillTknU8(&csfb1xrttRegParamsIE->parameterReg,
              storedCsfbRegInfo->paramReg);
 
           /* Power up Registration */
           wrFillTknU8(&csfb1xrttRegParamsIE->powerUpReg,
              storedCsfbRegInfo->powerUpReg);

           /* Registration period */
           WR_FILL_BIT_STR_VAR_LEN(csfb1xrttRegParamsIE->registrationPeriod,7,
                    storedCsfbRegInfo->regPeriod,sib8Sdu);

           /* Registration zone */
           WR_FILL_BIT_STR_VAR_LEN(csfb1xrttRegParamsIE->registrationZone,12,
                    storedCsfbRegInfo->regZone,sib8Sdu);
 
           /* Total zone */
           WR_FILL_BIT_STR_VAR_LEN(csfb1xrttRegParamsIE->totalZone,3,
                    storedCsfbRegInfo->totalZone,sib8Sdu);

           /* Zone timer */
           WR_FILL_BIT_STR_VAR_LEN(csfb1xrttRegParamsIE->zoneTimer,3,
                    storedCsfbRegInfo->zoneTimer,sib8Sdu);
           }
   RETVALUE(ROK);
}

/** @brief This function fills 1xRTT parameters of SIB8
 *
 * @details
 *
 *     Function: wrSibBldSIB81xRttParams
 *
 *         Processing steps:
 *         - Checks if 1xRtt parameters are configured and fill in SIB8
 *
 */
PRIVATE S16 wrSibBldSIB81xRttParams
(
 WrCellCb             *cellCb,
 NhuDatReqSdus        *sib8Sdu,
 NhuSysInfoBlockTyp8  *Sib8Ie,
 WrSib8Params         *sib8
 )
{
    

   TRC2 (wrSibBldSIB81xRttParams);
     /*1xRTT Params*/
     if(sib8->XRTTParamPres)
     {
        WR_FILL_TKN_UINT(Sib8Ie->parameters1XRTT.pres, PRSNT_NODEF);

        /* Fill CSFB Registration parameters */
        wrSibBldSIB81xRttCsfbRegParams(cellCb,sib8Sdu,Sib8Ie,sib8);
        /* Long code state */
        wrEmmGetLongCodeState(&sib8->XRTTparam.longCodeState);
        wrFillTknBStr64(&(Sib8Ie->parameters1XRTT.longCodeState1XRTT),
              42,sib8->XRTTparam.longCodeState,
              &(sib8Sdu->memCp));
        wrSibBldSIB81xRttCellReselParams(cellCb, sib8Sdu, Sib8Ie, sib8);
     }

   RETVALUE (ROK);
}
  
/** @brief This function fills the CDMA system time in SIB8
 *
 * @details
 *
 *     Function: wrSibBldSIB8SysInfoTime
 *
 *         Processing steps:
 *         - Checks if CDMA system time has to be filled and fill in SIB8
 *
 */
PRIVATE S16 wrSibBldSIB8SysInfoTime
(
 NhuDatReqSdus        *sib8Sdu,
 NhuSysInfoBlockTyp8  *Sib8Ie,
 WrSib8Params         *sib8,
 U16                  cdmaTimeOffset
 )
{
   U64           cdmaSysTime = 0;

   TRC2 (wrSibBldSIB8SysInfoTime);

   wrEmmGetCdmaSysTime(10, &cdmaSysTime, &sib8->XRTTparam.longCodeState,
                           sib8->sysTime.cdmaEutraTimeSync, cdmaTimeOffset);
     
   /* Fill the system time info*/
   WR_FILL_TKN_UINT(Sib8Ie->systemTimeInfo.pres, PRSNT_NODEF);
   /* CDMA E-UTRA synchronisation */
   wrFillTknU8(&Sib8Ie->systemTimeInfo.cdma_EUTRA_Synchronisation,
         (U8)sib8->sysTime.cdmaEutraTimeSync);

   /* CDMA system time */
   wrFillTknU8(&Sib8Ie->systemTimeInfo.cdma_SysTime.choice,
         sib8->sysTime.syncChoice);

   if (sib8->sysTime.syncChoice == WR_CDMA_ASYNC_TIME)
   {
      wrFillTknBStr64(
            &(Sib8Ie->systemTimeInfo.cdma_SysTime.val.asynchronousSysTime),
            49, cdmaSysTime, &(sib8Sdu->memCp));
   }
   else
   {
      wrFillTknBStr64(
            &(Sib8Ie->systemTimeInfo.cdma_SysTime.val.synchronousSysTime),
            39, cdmaSysTime, &(sib8Sdu->memCp));
   }

   RETVALUE (ROK);
}


/** @brief This function fills SIB8 IEs for a member in system information.
 *
 * @details
 *
 *     Function: wrSibBldSIB8IEs
 *
 *         Processing steps:
 *         - Build the SIB8 IEs based on OAM configured information and ANR
 *           given inforamtion.
 *         - Get the CDMA Neighebour information from ANR module by
 *           using wrAnrGetWhiteAndBlackList API. 
 *
 * @param [in]       cellCb : Cell control block
 * @param [in, out] sib8Sdu : RRC Data Request for SIB8 SDU.
 * @param [out]      sib8Ie : SIB8 IE.
 * @return  S16
 *      -# Success : ROK
 */
PUBLIC S16 wrSibBldSIB8IEs
(
 WrCellCb      *cellCb,
 NhuDatReqSdus *sib8Sdu,
 NhuSysInfoBlockTyp8  *Sib8Ie
 )
{
     WrSib8Params            *sib8     = &(cellCb->sib8);
     U8                      siIdx     = sib8->siIndex;
   U8                      siWinSz;
   U16                     numOfSfnOffset;
   U16                     cdmaSubfrmOffset;

     TRC2(wrSibBldSIB8IEs);

   /* Caluculate the SFN boundary at or after the ending boundary of
    * the SI-window in which SystemInformationBlockType8 is transmitted.
      */
   wrEmCellSchdSiWinSzMap(cellCb->sib1.siWindowLen, &siWinSz);

   numOfSfnOffset = (((siIdx + 1)* siWinSz) + 9) / 10;
  
     /* Since L3 is updating the time 40ms(i.e WR_SFN_INCREMENT) before 
      * the SIB8 periodicity so add the same offset(in ms)to current GPS time
      */
   cdmaSubfrmOffset = ( WR_SFN_INCREMENT * 10 + numOfSfnOffset * 10);
     /* System Information time */ 
     WR_FILL_TKN_UINT(Sib8Ie->pres, PRSNT_NODEF);

     /* System Information time */ 
   wrSibBldSIB8SysInfoTime(sib8Sdu, Sib8Ie, sib8, cdmaSubfrmOffset);

     /* Search window size*/
     if(sib8->searchWindSize.pres)
     {
        wrFillTknU32(&(Sib8Ie->searchWindowSize),sib8->searchWindSize.val);
     }

     /* 1xRTT parameters */
     wrSibBldSIB81xRttParams(cellCb, sib8Sdu, Sib8Ie, sib8);

     wrFillTknU8(&(Sib8Ie->extaddgrp_1.pres),PRSNT_NODEF);
     wrFillTknU8(&(Sib8Ie->extaddgrp_1.csfb_SupportForDualRxUEs_r9),
        cellCb->csfbCfgGrp.dualRxSup);

     /* Extended CSFB 1xrtt registration parameters */
     if (sib8->XRTTparam.csfbRegParam.powerDownRegPres == TRUE)
     {
        wrFillTknU8(&(Sib8Ie->extaddgrp_1.csfb_RegistrationParam1XRTT_v920.pres),PRSNT_NODEF);
        wrFillTknU32(&(Sib8Ie->extaddgrp_1.csfb_RegistrationParam1XRTT_v920.powerDownReg_r9),
           NhuCSFB_RegistrationParam1XRTT_v920powerDownReg_r9trueEnum);
     }
     
     /* AC barring parameters */
     wrSibBldSIB81xRttAcBarringParams(cellCb, sib8Sdu, Sib8Ie, sib8);
/*RRC Rel10 Upgrade - start*/
	 if((TRUE == wrEmmGetEcsfbAndDualRxTxSuppCfgs(cellCb->cellId))&&
			 (TRUE == sib8->XRTTparam.csfbRegParamPres))
	 {
		 RLOG0(L_DEBUG, "wrSibBldSIB8IEs:Filling csfb_DualRxTxSupport_r10\n");
		 wrFillTknU8(&(Sib8Ie->extaddgrp_2.pres),PRSNT_NODEF);
		 wrFillTknU32(&(Sib8Ie->extaddgrp_2.csfb_DualRxTxSupport_r10),
				 NhuSysInfoBlockTyp8extaddgrp_2csfb_DualRxTxSupport_r10trueEnum);
	 }
	 else
	 {
		 Sib8Ie->extaddgrp_2.pres.pres = NOTPRSNT;
	 }
/*RRC Rel10 Upgrade - end*/
   RETVALUE(ROK);
}

/** @brief This function handles the expiry of SIB8 timer
 *
 * @details
 *
 *     Function: wrEmmPrcSib8SchdEvnt
 *
 *         Processing steps:
 *         - On expiry of the timer SIB8 PDU shall be constructed with 
 *           the CDMA system time that is calculated based on the offset to 
 *           which the PDU has to be given to the scheduler.
 *         - The constructed PDU shall be given to the scheduler indicating 
 *           that it should be transmitted immediately without waiting for the
 *           best modification period.
 *
 */
PUBLIC S16 wrEmmPrcSib8SchdEvnt
(
   WrCellCb *cb
)
{
   RgrSiCfgReqInfo    *siCfgReq         = NULLP;
   NhuDatReqSdus      *nhuDatReqEvntSdu = NULLP;
   WrCellCb           *cellCb           = NULLP;
   NhuBCCH_DL_SCH_Msg        *bcchSchMsg = NULLP;
   NhuSysInfocriticalExtns   *critExt    = NULLP;
   NhuSysInfo_r8_IEssib_TypAndInfoMember *member = NULLP;

   TRC2(wrEmmPrcSib8SchdEvnt);

   cellCb = cb;

   WR_ALLOCEVNT(&nhuDatReqEvntSdu, sizeof(NhuDatReqSdus));
   if(NULLP == nhuDatReqEvntSdu)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   if(wrSibBldSIHeader(cellCb, nhuDatReqEvntSdu, 0, 0, 1) != ROK)
   {
      RLOG0(L_ERROR, " Build SIB SI header failed. ");
      RETVALUE(RFAILED);
   }

   bcchSchMsg = &(nhuDatReqEvntSdu->sdu.ueMsg.msg.cmnMsg.cmn.bcchDlSchMsg);
   critExt = &(bcchSchMsg->message.val.c1.val.systemInfo.criticalExtns);
   
   member = critExt->val.systemInfo_r8.sib_TypAndInfo.member;
   WR_FILL_TKN_UINT(member->choice, MEMBER_SIB8);

   WR_ALLOC(&siCfgReq, sizeof(RgrSiCfgReqInfo));
   if(NULLP == siCfgReq)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(nhuDatReqEvntSdu);
      RETVALUE(RFAILED);
   }

   siCfgReq->cellId = cellCb->cellId;
   siCfgReq->cfgType = RGR_SI_CFG_TYPE_SIB8_CDMA;
   siCfgReq->siId = cellCb->sib8.siIndex + 1;


   /* Build SIB8 PDU */   
   if (wrSibBldSIB8IEs(cellCb, nhuDatReqEvntSdu, &(member->val.sib8)) != ROK)
   {
      RLOG0(L_ERROR, "wrSibBldSIB8IEs failed ");
      WR_FREE_EVNT(nhuDatReqEvntSdu);
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RETVALUE(RFAILED);
   }

   /*Encoding of DLSCH Message*/
   if (wrBldDlBcchDlSchPdu(nhuDatReqEvntSdu, &siCfgReq->pdu) != ROK)
   {
      RLOG0(L_ERROR, " wrBldDlBcchDlSchPdu failed ");
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RETVALUE(RFAILED);
   }
 
   if (WrIfmSchdSiCfgReq(cellCb->cellId << 16 | WR_SIB8_TRANS_ID, siCfgReq) != ROK)
   {
      RLOG0(L_ERROR, " WrIfmSchdSiCfgReq primitive for SIB1 failed. ");
      WR_FREE(siCfgReq, sizeof(RgrSiCfgReqInfo));
      RETVALUE(RFAILED);
   }
   RETVALUE (ROK);
}

/* 
 *       Fun:     wrEmmGetRatPriority
 *
 *       Desc:    This function will return the CSFB RAT priority
 *       Processing steps:
 *         - Get the CSFB RAT Priority from plmnCb. 
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 wrEmmGetRatPriority
(
WrPlmnId                     *plmn,
WrRatPriorityList            *ratPriority,
WrCellId                     cellId
)
{
   WrPlmnId                  plmnId ={0};
   WrCellCb                  *cellCb;
   U8                        numOfPlmn =0;
   U8                        idx =0;
   S16                       ret = RFAILED;

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
		RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL");
      RETVALUE(RFAILED);
   }

   /* Get the num Of Plmn from Sib1 */
   numOfPlmn = wrEmmCb.plmns.numPlmns;

   for(idx = 0; idx < numOfPlmn ; idx ++)
   {
      plmnId = wrEmmCb.plmns.plmns[idx].plmnInfo.plmnId;
      if (wrEmmPlmnPlmnsEqual(&plmnId, plmn) == TRUE)
      {
         /* get csfb prior RatTyp for selected plmn */
         *ratPriority = wrEmmCb.plmns.plmns[idx].plmnInfo.plmnRatPriority;
         ret = ROK;
         break;
      }
   }
   RETVALUE(ret);
}

/** @brief This function is responsible for finding out 
 *  if ENB supports CSFB-Dual Rx.
 *
 * @details
 *
 *     Function: wrEmmDualRxInd
 *
 * @param [in]       cellId : Cell control block Id
 * @return BOOL 
 *       -# TRUE
 *       -# FALSE
 */
PUBLIC Bool wrEmmIsDualRxSupported
(
WrCellId                     cellId
)
{
   WrCellCb                  *cellCb = NULLP;

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
		RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL");
      RETVALUE(FALSE);
   }

   RETVALUE(cellCb->csfbCfgGrp.dualRxSup);
}


/* @brief This function is responsible to find out 
 * whether ENB supports CSFB to UTRA with or without meas
 *
 * @details
 *
 *     Function: wrEmmChkCsfbToUtraMeasSup
 * @param [in]       cellId : Cell control block Id
 * @return BOOL 
 *       -# TRUE  -> If eCSFB .
 *       -# FALSE -> if CSFB  .
 *       
 *       */
PUBLIC S16 wrEmmChkCsfbToUtraMeasSup
(
WrCellId                    cellId, 
U8                          *csfbUtraMeasSup
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
		RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL");
      RETVALUE(RFAILED);
   }

   *csfbUtraMeasSup =  cellCb->csfbCfgGrp.csfbToUtraMeasSup;
   RETVALUE(ROK);
}
/* @brief This function is responsible to find out 
 * whether ENB supports eCSFB or not from EmmCellCb
 *
 * @details
 *
 *     Function: wrEmmIsCsfbOrEcsfbMeasSup
 * @param [in]       cellId : Cell control block Id
 * @return BOOL 
 *       -# TRUE  -> If eCSFB .
 *       -# FALSE -> if CSFB  .
 *       
 *       */
PUBLIC S16 wrEmmIsCsfbOrEcsfbMeasSup
(
WrCellId                    cellId, 
Bool                        csfbOrEcsfbSup,
U8                          *csfbOreCsfbSup
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
		RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL");
      RETVALUE(RFAILED);
   }
   /* TRUE then eCSFB, FALSE CSFB */
   if (TRUE == csfbOrEcsfbSup)
   {
      *csfbOreCsfbSup =  cellCb->csfbCfgGrp.eCsfbMeasSup;
   }
   else
   {
      *csfbOreCsfbSup =  cellCb->csfbCfgGrp.csfbToCdmaMeasSup;
   }
   RETVALUE(ROK);
}

/*  @brief This function gets the target cell which will be used to build 
 *  mobilityParams. It will fill following target cell parametrs and return
 *  1. marketId
 *  2. switchNum
 *  3. cellId
 *  4. sectorNum
 *  5. mobParams
 *
 *
 *      Function: wrEmmGetCdma1xRttTgtCell 
 *
 *
 *  @param [in]  tgtCell   : pointer to WrCdmaCell 
 *  @param [in]  cellId    : Cell control block Id
 *  @return S16
    -# Success: void
 */
PUBLIC S16 wrEmmGetCdma1xRttTgtCell
(
 TknStrOSXL     **sectorIdPtr,
 WrCellId      cellId
)
{
   WrCellCb                      *cellCb;

   WR_GET_CELLCB(cellCb, cellId);

   if (cellCb == NULLP)
   {
		RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL");
      RETVALUE(RFAILED);
   }
      *sectorIdPtr = &cellCb->csfbCfgGrp.cdma1xrttCsfbCfg.sectorIdPtr;

   RETVALUE(ROK);
} /* wrEmmGetCdma1xRttTgtCell */

/**
 *  @brief This function finds the Configured Frequency for the Band
 *
 *      Function: wrUmmChk1xRttCdmaFreqCfgForBand 
 *
 *
 *  @param [in]  bandClass : bandclass value
 *  @param [in]  cellId    : Cell control block Id
 *  @return U32
    -# Success: TRUE
    -# Failure: FALSE
 */
PUBLIC Bool wrUmmChk1xRttCdmaFreqCfgForBand
(
  U32        bandClass,
  WrCellId   cellId
)
{
   CmLList              *neighFreqLnk = NULLP;
   WrCdma1xFreqCb       *neighFreqCb = NULLP;
   WrCellCb             *cellCb;

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
		RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL");
      RETVALUE(RFAILED);
   }
   /* Go through the frequency list and return the highest priority 
    * frequency from the cellCb->cdma1xFreqLst */
   neighFreqLnk = cellCb->cdma1xFreqLst.first;
   if(NULLP == neighFreqLnk)
   {
      RLOG0(L_WARNING, "neighFreqLnk is NULLP ");
      RETVALUE(RFAILED);
   }

   while(neighFreqLnk != NULLP)
   {
      neighFreqCb = (WrCdma1xFreqCb*)neighFreqLnk->node;
      if(neighFreqCb->bandClass == bandClass)
      {
         RETVALUE(TRUE);
      }
      neighFreqLnk = neighFreqLnk->next;
   }
   RLOG0(L_WARNING, "CDMA Neighbour is NOT Configured for Supported BandClass");
   RETVALUE(FALSE);
}/*wrUmmChk1xRttCdmaFreqCfgForBand*/
/**
 *  @brief This function finds the highest priority frequnency from
 *         cellCb->cdmaBandLst list 
 *
 *      Function: wrEmmGetHighestPrioFreq 
 *
 *
 *  @param [in]  bandClass : bandclass value
 *  @param [out] arfcn     : arfcn of highest priority
 *  @param [in]  cellId    : Cell control block Id
 *  @return U32
    -# Success: ROK
    -# Failure: RFAILED
 */
PUBLIC S16 wrEmmGetHighestPrioFreq
(
  U32        bandClass,
  U32        *arfcn,
  WrCellId   cellId
)
{
   CmLList              *neighFreqLnk = NULLP;
   WrCdma1xFreqCb       *neighFreqCb = NULLP;
   U8                   priority = 0;
   WrCellCb             *cellCb;

   WR_GET_CELLCB(cellCb, cellId);

   if (cellCb == NULLP)
   {
		RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL");
      RETVALUE(RFAILED);
   }
   /* Go through the frequency list and return the highest priority 
    * frequency from the cellCb->cdma1xFreqLst */
   neighFreqLnk = cellCb->cdma1xFreqLst.first;
   if(NULLP == neighFreqLnk)
   {
      RLOG0(L_WARNING, "neighFreqLnk is NULLP ");
      RETVALUE(RFAILED);
   }

   while(neighFreqLnk != NULLP)
   {
      neighFreqCb = (WrCdma1xFreqCb*)neighFreqLnk->node;
      if(neighFreqCb->bandClass == bandClass)
      {
         if(priority < neighFreqCb->freqPriority)
         {
            priority = neighFreqCb->freqPriority;
            *arfcn = neighFreqCb->arfcn;
         }
      }
      neighFreqLnk = neighFreqLnk->next;
   }/* end of frequency list */
 
   RETVALUE(ROK);
} /* wrEmmGetHighestPrioFreq */


/**
 *  @brief This function finds the highest priority band from
 *         cellCb->cdmaBandLst list 
 *
 *      Function: wrEmmGetHighestPrioBand 
 *
 *
 *  @param [in]  ueCb      : ueCb
 *  @param [out] band      : bandclass value
 *
 * @return U32
    -# Success: ROK
    -# Failure: RFAILED
 */
PUBLIC S16 wrEmmGetHighestPrioBand
(
  WrUeCb                    *ueCb,
  U32                       *band
)
{
   Bool                isUeSupported = FALSE;
   Bool                isFreqCfgForBand = FALSE;
   U8                  priority = 0;
   WrCellCb            *cellCb;
   CmLList             *neighBandLnk = NULLP;
   WrCdmaBandClassCb   *neighBandCb = NULLP;
   U8                  cnt = 0;

   WR_GET_CELLCB(cellCb, ueCb->cellId);
   if (cellCb == NULLP)
   {
		RLOG_ARG0(L_ERROR, DBG_CELLID, ueCb->cellId, "Cell Cb is NULL");
      RETVALUE(RFAILED);
   }
   /* Go through the band list and return the highest priority 
    * band from the cellCb->cdma1xBandLst */
   neighBandLnk = cellCb->cdmaBandLst.first;
   if(NULLP == neighBandLnk)
   {
      RLOG0(L_WARNING, "neighBandLnk is NULLP ");
      RETVALUE(RFAILED);
   }

   while(neighBandLnk != NULLP)
   {
      neighBandCb = (WrCdmaBandClassCb*)neighBandLnk->node;

      if(priority <= neighBandCb->cellReselPri)
      {
         isUeSupported = wrUmmChk1xRttCdmaBandSupported(ueCb,
               neighBandCb->bandClass);
         /*Check if Supported band has any Freq Configured */
         isFreqCfgForBand = 
               wrUmmChk1xRttCdmaFreqCfgForBand(neighBandCb->bandClass, ueCb->cellId);
         /* *band contains the UESupported band and now if prior 
          * band is also supportd by UE then copy this bandClass
          * into *band
          */
         if((isUeSupported == TRUE) && (isFreqCfgForBand == TRUE))
         {
            priority = neighBandCb->cellReselPri;
            *band = neighBandCb->bandClass;
            cnt++;
         }
      }
      neighBandLnk = neighBandLnk->next;
   }/* end of band list */
   if(cnt > 0)
   {
      RETVALUE(ROK);
   }
   RLOG0(L_WARNING, "UE does Not Supported Configured CDMA BAND");
   RETVALUE(RFAILED);

} /* wrEmmGetHighestPrioBand */

/*RRC Rel10 Upgrade - start*/
/** @brief This function is responsible for finding out 
 *  if ENB supports e_CSFB_dual_1XRTT_r10.
 *
 * @details
 *
 *     Function: wrEmmIsDualRxTxSupported
 *
 * @param [in]       cellId : Cell control block Id
 * @return BOOL 
 *       -# TRUE
 *       -# FALSE
 */
PUBLIC Bool wrEmmIsDualRxTxSupported
(
WrCellId                     cellId
)
{
   WrCellCb                  *cellCb = NULLP;

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
		RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL");
      RETVALUE(FALSE);
   }

   RETVALUE(cellCb->csfbCfgGrp.dualRxTxSup);
}

/** @brief This function is responsible for finding out 
 *  if ENB supports 1xrtt registration and ecsfb and dualRxTx.
 *
 * @details
 *
 *     Function:wrEmmGetEcsfbAndDualRxTxSuppCfgs
 *
 * @param [in]       cellId : Cell control block Id
 * @return BOOL 
 *       -# TRUE
 *       -# FALSE
 */
PUBLIC Bool wrEmmGetEcsfbAndDualRxTxSuppCfgs
(
 WrCellId                     cellId
)
{
	WrCellCb                  *cellCb;
	WR_GET_CELLCB(cellCb, cellId);

	if (cellCb == NULLP)
	{
		RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL");
		RETVALUE(FALSE);
	}

	if ((TRUE == wrEmmIsDualRxTxSupported(cellId)) &&
			(LWR_ECSFB_NOT_SUPPORTED != cellCb->csfbCfgGrp.eCsfbMeasSup))
	{
		/*Need not check for dualRx only as the behaviour is same
          for with or without dualRx if the ue supports dualRxTx*/
		RETVALUE(TRUE);
	}
	RETVALUE(FALSE);
}

/********************************************************************30**

           End of file:     fc_emm_ecsfb_utils.c@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:54:21 2014

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
