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
               

     File:     wr_emm_ecsfb_anr.c

     Sid:      fc_emm_ecsfb_anr.c@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:54:21 2014

     Prg:      BSL

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=13;

/* header include files (.h) */
#include "wr.h"            /* Common headers and Data Structures */
#include "wr_emm.h"        /* Environment options */
#include "wr_emm_pbm.h"
#include "cm_os.h"       /* common hashing */
#include "cm_os.x"
#include "wr_emm_neighenb.h"
#ifdef WR_RSYS_OAM
#include "wr_utils.h"
#endif

PUBLIC S16 wrEmmAnrAddCdma1xRttNeighFreqLst
(
   U8                            cellId,
   LwrNeighCdma1xFreq            *cdma1xRttFreqLst
);
PUBLIC S16 wrEmmAnrAddNrBand
(
U8                           cellId,
U8                           numBand,
LwrNeighBandClassCfg        *nbrBandAdd
);
PRIVATE S16 wrEmmAnrAddCdma1xRttNeighCell
(
   WrNrCdmaFreqCb                *freqNode,
   WrCdma2kNeighCellCb           *newCell,
   WrCellCb                      *cellCb
);
PUBLIC S16 wrEmmAnrAddCdma1xRttNr
(
   WrCellCb                      *cellCb,
   WrCdma2kNeighCellCb           *newCell
);
PUBLIC S16 wrEmmAnrFormCdma1xRttFreqLst
(
   WrNrWlBlInfo                  *nrWlBlInfo,
   WrCellCb                      *cellCb,
   WrBrdCstInfo                  brInfo
);
EXTERN S16 wrEmmAnrGetCdma1xRttCell
(
   U8                     cellId,
   WrCdmaRttPilotLst      *cdmaRttPilotLst
);
PUBLIC S16 wrEmmAnrGetCdma1xRttCellByPnOffset
(
   U8                     cellId,
   WrCdma1xRttPnOffset    *cdmaRttPnOffset,
   WrCdma1xRttPilotInfo   *cdmaPilotInfo
);
/**
 *  @brief This function add the CDMA1xRTT neighbor frequency into the 
 *         neighbor Frequency list
 *
 *      Function: wrEmmAnrAddCdma1xRttNeighFreq
 *
 *          Processing steps:
 *             - create new frequency node 
 *             - Add into the freq List
 *
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAnrAddCdma1xRttNeighFreq
(
   U16                           arfcn,
   CmLListCp                     *cdma1xRttFreqLst
)
{
   WrNrCdmaFreqCb              *newFreqNode = NULLP;

   TRC2(wrEmmAnrAddCdma1xRttNeighFreq);

   /* allocate new frequency Node */
   WR_ALLOC(&newFreqNode, sizeof(WrNrCdmaFreqCb));
   if(newFreqNode != NULLP)
   {
      newFreqNode->arfcn = arfcn;

      /* Initialize the PBM NrCb. For this frequency initialize 
       * nrWlLst, nrBlLst. Same will be sent to Measurment module 
       * to populate its CB i.e wrUmmMeasCdmaMeasObjNode */
      cmLListInit(&newFreqNode->nrWlLst);
      cmLListInit(&newFreqNode->nrBlLst);

      newFreqNode->freqLstEnt.node = (PTR)newFreqNode;

      cmLListAdd2Tail(cdma1xRttFreqLst, &newFreqNode->freqLstEnt);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmAnrAddCdma1xRttNeighFreq */

/**
 *  @brief This function add the CDMA1xRTT band node into the 
 *         corresponding band list
 *
 *      Function: wrEmmAnrAddCdma1xRttBandNodeIdx
 *
 *          Processing steps:
 *             - create new band node 
 *             - Add into the band List
 *
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAnrAddCdma1xRttBandNodeIdx
(
   U16                           bandClass,
   WrCDMA2KNeighInfo            *nrCdma1xRttBandInfo
)
{
   U8                            bandNodeIndex = 0;
   U8                            maxBandElem;

   TRC2(wrEmmAnrAddCdma1xRttBandNodeIdx);

   maxBandElem = nrCdma1xRttBandInfo->numBandClass;

   while(bandNodeIndex < maxBandElem)
   {
      if(nrCdma1xRttBandInfo->neighBandLst[bandNodeIndex].bandClass
           == bandClass)
      {
         break;
      }
      bandNodeIndex++;
   }
   if(bandNodeIndex < WR_MAX_CDMA_BAND_CLASS)
   {
      if(bandNodeIndex == maxBandElem)
      {
         nrCdma1xRttBandInfo->neighBandLst[bandNodeIndex].bandClass = bandClass;
         nrCdma1xRttBandInfo->numBandClass++;
         RETVALUE (ROK);
      }
   }

   RETVALUE(RFAILED);
} /* end of wrEmmAnrAddCdma1xRttBandNodeIdx */

/**
 *  @brief This function gets the CDMA1xRTT band node from the 
 *         corresponding band list
 *
 *      Function: wrEmmAnrGetCdma1xRttBandNodeIdx
 *
 *          Processing steps:
 *             - Get the band node based on the band class
 *
 */
PRIVATE U8 wrEmmAnrGetCdma1xRttBandNodeIdx
(
   WrCDMA2KNeighInfo            *nrCdma1xRttBandInfo,
   U16                          bandClass
)
{
   U8                            bandNodeIndex = 0;

   TRC2(wrEmmAnrGetCdma1xRttBandNodeIdx);

   while(bandNodeIndex < nrCdma1xRttBandInfo->numBandClass)
   {
      if(nrCdma1xRttBandInfo->neighBandLst[bandNodeIndex].bandClass
           == bandClass)
      {
         break;
      }
      bandNodeIndex++;
   }
   if(bandNodeIndex < WR_MAX_CDMA_BAND_CLASS)
   {
      if(bandNodeIndex == nrCdma1xRttBandInfo->numBandClass)
      {
         nrCdma1xRttBandInfo->neighBandLst[bandNodeIndex].bandClass = bandClass;
         nrCdma1xRttBandInfo->neighBandLst[bandNodeIndex].numFreq = 0;
         nrCdma1xRttBandInfo->numBandClass++;
      }
   }

   RETVALUE(bandNodeIndex);
} /* end of wrEmmAnrGetCdma1xRttBandNodeIdx */

/**
 *  @brief This function gets the CDMA1xRTT freq node from the 
 *         corresponding freq list
 *
 *      Function: wrEmmAnrGetCdma1xRttFreqNodeIdx
 *
 *          Processing steps:
 *             - Get the freq node based on the arfcn
 *
 */
PRIVATE U8 wrEmmAnrGetCdma1xRttFreqNodeIdx
(
   WrCDMA2KBandInfo             *bandNode,
   U16                          arfcn
)
{
   U8                            freqNodeIndex = 0;

   TRC2(wrEmmAnrGetCdma1xRttFreqNodeIdx);

   while(freqNodeIndex < bandNode->numFreq)
   {
      if(bandNode->neighFreqInfo[freqNodeIndex].arfcn == arfcn)
      {
         break;
      }
      freqNodeIndex++;
   }
   if(freqNodeIndex < WR_MAX_CDMA_FREQ_PER_BAND)
   {
      if(freqNodeIndex == bandNode->numFreq)
      {
         bandNode->neighFreqInfo[freqNodeIndex].arfcn = arfcn;
         bandNode->neighFreqInfo[freqNodeIndex].wlData.numWlCells = 0;
         bandNode->numFreq++;
      }
   }

   RETVALUE(freqNodeIndex);
} /* end of wrEmmAnrGetCdma1xRttFreqNodeIdx */

/**
 *  @brief This function add the CDMA1xRTT freq node into the 
 *         corresponding freq list
 *
 *      Function: wrEmmAnrAddCdma1xRttFreqNodeIdx
 *
 *          Processing steps:
 *             - create new freq node 
 *             - Add into the freq List
 *
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16  wrEmmAnrAddCdma1xRttFreqNodeIndx
(
   WrCDMA2KBandInfo            *nrCdma1xRttBandInfo,
   U16                           arfcn
)
{
   U8                            freqNodeIndex = 0;
   U8                            maxFreqElem;

   TRC2(wrEmmAnrAddCdma1xRttFreqNodeIndx);

   maxFreqElem = nrCdma1xRttBandInfo->numFreq;

   while( freqNodeIndex < maxFreqElem)
   {
      if(nrCdma1xRttBandInfo->neighFreqInfo[freqNodeIndex].arfcn == arfcn)
      {
         break;
      }
      freqNodeIndex++;
   }
   if(freqNodeIndex < WR_MAX_CDMA_FREQ_PER_BAND)
   {
      if(freqNodeIndex == maxFreqElem)
      {
         nrCdma1xRttBandInfo->neighFreqInfo[freqNodeIndex].arfcn = arfcn;
         nrCdma1xRttBandInfo->numFreq++;
         RETVALUE (ROK);
      }
   }

   RETVALUE(RFAILED);
} /* end of wrEmmAnrAddCdma1xRttFreqNodeIndx */

/**
 *  @brief This function gets the CDMA1xRTT band node from the 
 *         corresponding band list
 *
 *      Function: wrEmmAnrGetCdma1xRttBandNode
 *
 *          Processing steps:
 *             - Get the band node based on the band class
 *
 */
PRIVATE WrNrCdmaBandCb*  wrEmmAnrGetCdma1xRttBandNode
(
   CmLListCp                     *cdmaBandClassLst,
   U8                            bandClass
)
{
   CmLList                *lnk=NULLP;
   WrNrCdmaBandCb         *bandNode = NULLP;

   TRC2(wrEmmAnrGetCdma1xRttBandNode);
   
   /*get the first Node from ANR CB i.e
    * cellCb->nrCb->cdma1xRttCb.cdmaBandList by SM configured 
    * bandClass */
   CM_LLIST_FIRST_NODE(cdmaBandClassLst, lnk);

   while (lnk != NULLP)
   {
      bandNode = (WrNrCdmaBandCb*)lnk->node;
      if (bandNode->bandClass == bandClass)
      {
         RETVALUE(bandNode);
      }
      CM_LLIST_NEXT_NODE(cdmaBandClassLst, lnk);
   }
   RETVALUE (NULLP);
} /* end of wrEmmAnrGetCdma1xRttBandNode */ 

/**
 *  @brief This function gets the CDMA1xRTT frequency node from the 
 *         corresponding band list
 *
 *      Function: wrEmmAnrGetCdma1xRttFreqNode
 *
 *          Processing steps:
 *             - Get the band node based on the band class
 *
 */
PRIVATE WrNrCdmaFreqCb*  wrEmmAnrGetCdma1xRttFreqNode
(
   CmLListCp                     *cdmaFreqLst,
   U16                            arfcn
)
{
   CmLList                *lnk=NULLP;
   WrNrCdmaFreqCb         *freqNode = NULLP;

   TRC2(wrEmmAnrGetCdma1xRttFreqNode);
   
   CM_LLIST_FIRST_NODE(cdmaFreqLst, lnk);

   while (lnk != NULLP)
   {
      freqNode = (WrNrCdmaFreqCb*)lnk->node;
      if (freqNode->arfcn == arfcn)
      {
         RETVALUE(freqNode);
      }
      CM_LLIST_NEXT_NODE(cdmaFreqLst, lnk);
   }
   RETVALUE (NULLP);
} /* wrEmmAnrGetCdma1xRttFreqNode */
/**
 *  @brief This function add the CDMA1xRTT neighbor frequency into the 
 *         neighbor Frequency list
 *
 *      Function: wrEmmAnrAddCdma1xRttNeighFreqLst
 *
 *          Processing steps:
 *             - create new frequency node 
 *             - Add into the freq List
 *
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrAddCdma1xRttNeighFreqLst
(
   U8                            cellId,
   LwrNeighCdma1xFreq            *cdma1xRttFreqLst
)
{
   S16                           ret = ROK;
   WrCellCb                      *cellCb  = NULLP;
   WrNrCdmaBandCb                *bandNode = NULLP;
   WrCDMA2KBandInfo              *bandInfo = NULLP;
   U8                            bandIndx = 0;

   TRC2(wrEmmAnrAddCdma1xRttNeighFreqLst);

   WR_GET_CELLCB(cellCb, cellId);

   if (NULLP == cellCb)
   {   
       RLOG0(L_ERROR, " Failed to get cell CB ");
       RETVALUE(RFAILED);
   }   

   /* Get the band class node from ANR */
   bandNode = wrEmmAnrGetCdma1xRttBandNode(&cellCb->nrCb->cdma1xRttCb.cdmaBandList,cdma1xRttFreqLst->bandClass); 
   if (bandNode == NULLP)
   {
       RLOG0(L_WARNING, " Failed to get band node ");
       RETVALUE(RFAILED);
   }

   /* Get Frequency Node from ANR by using the recieved bandClass frequency Node */
   ret = wrEmmAnrAddCdma1xRttNeighFreq(cdma1xRttFreqLst->arfcn,
            &bandNode->cdmaFreqList);
   if(ret == RFAILED)
   {
      RLOG0(L_WARNING, " Failed to get freq ");
      RETVALUE(RFAILED);
   }

   /* Get the band class in WL/BL table */
   bandIndx = 
   wrEmmAnrGetCdma1xRttBandNodeIdx(&cellCb->nrCb->nrUpdtInfo->cdma1xRtt,
      cdma1xRttFreqLst->bandClass);

   /* Call the Function to Add Frequency Node */
   /* Klocwork fix for ECSFB */
   if (bandIndx >= WR_MAX_CDMA_BAND_CLASS)
   {
      RLOG0(L_WARNING, " Invalid band index ");
      RETVALUE(RFAILED);
   }
   bandInfo = &cellCb->nrCb->nrUpdtInfo->cdma1xRtt.neighBandLst[bandIndx];
   wrEmmAnrAddCdma1xRttFreqNodeIndx(bandInfo,cdma1xRttFreqLst->arfcn);
   if(ret == RFAILED)
   {
      RLOG0(L_ERROR, " Failed to add freq in nrupdtinfo ");
      RETVALUE(RFAILED);
   }

   cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_CDMA_1XRTT_FREQ_BIT;

   RETVALUE(ROK);
}/* end of wrEmmAnrAddCdma1xRttNeighFreqLst */


/**
 *  @brief This function add the CDMA1xRTT band class node into the 
 *         band list
 *
 *      Function: wrEmmAnrAddCdma1xRttBandClassNode
 *
 *          Processing steps:
 *             - create new band node 
 *             - Add into the band List
 *
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAnrAddCdma1xRttBandClassNode
(
   U8                            bandClass,
   CmLListCp                     *cdmaBandClassLst
)
{
   WrNrCdmaBandCb                *newBandNode = NULLP;

   TRC2(wrEmmAnrAddCdma1xRttBandClassNode);

   /* create new Band Class Node */
   WR_ALLOC(&newBandNode, sizeof(WrNrCdmaBandCb));
   if(newBandNode != NULLP)
   {
      newBandNode->bandClass = bandClass;

      cmLListInit(&newBandNode->cdmaFreqList);

      newBandNode->bandLstEnt.node = (PTR)newBandNode;

      cmLListAdd2Tail(cdmaBandClassLst, &newBandNode->bandLstEnt);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmAnrAddCdma1xRttBandClassNode */

/** @brief This function adds the Neighbor cdma1x band List
 *
 * @details This function is called to add neighbor band list by EMM.
 *          The function receives the neighbor cdma1x 
 *          band List as input arguements. Returns the status
 *          success/failure.
 *
 *     Function: wrEmmAnrAddCdma1xRttBandClassLst()
 *
 *          Processing steps:
 *          - traverse through the cdma 1x band list
 *          - call fucntion to add each neighbor band
 *
 *  @param [in] cellId        : cellId of the serving Cell
 *  @param [in] cdmaBandClass : pointer to neighbor  containing
 *                                the cfg info 
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAnrAddCdma1xRttBandClassLst
(
   U8                            cellId,
   LwrNeighCdmaBandClassCfg      *cdmaBandClass
)
{
   S16                           ret = ROK;
   WrCellCb                      *cellCb  = NULLP;

   TRC2(wrEmmAnrAddCdma1xRttBandClassLst);
  
   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {   
       RLOG0(L_ERROR, " Failed to get cell CB ");
       RETVALUE(RFAILED);
   }    

   ret = wrEmmAnrAddCdma1xRttBandClassNode(cdmaBandClass->bandClass, 
                              &cellCb->nrCb->cdma1xRttCb.cdmaBandList);

   if (ret == RFAILED)
   {
      RLOG0(L_ERROR, " Failed to add Bandclass in cdmaBandList ");
      RETVALUE(RFAILED);
   }

   ret = wrEmmAnrAddCdma1xRttBandNodeIdx(cdmaBandClass->bandClass,
      &cellCb->nrCb->nrUpdtInfo->cdma1xRtt);
   if (ret == RFAILED)
   {
      RLOG0(L_ERROR, " Failed to add Bandclass in NrUpdtInfo ");
      RETVALUE(RFAILED);
   }

   cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_CDMA_1XRTT_BAND_BIT;
   
   RETVALUE(ret);
}/* end of wrEmmAnrAddCdma1xRttBandClassLst */

/** @brief This function adds the Neighbors bands into NR CB 
 *
 * @details This function is called to add a neighbor by EMM, upon the reception
 *          of new NR trigger from OAM/X2I/MCH. The receives the cell Id
 *          and the neighbor band class details as input arguements.The funciton
 *          calls function to Add bands of different NR types. And returns
 *          the status success/failure.
 *
 *     Function: wrEmmAnrAddNrBand() 
 *
 *         Processing steps:
 *          - switch to corresponding NR type
 *          - call fucntion to add Band Info to band list
 *          
 * @param [in] cellId     : cell for which NR needs to be added
 * @param [in] numBand    : Number of bands to be added
 * @param [in] nbrBandAdd : NR details containing the cfg info and pointer to 
 *                          cellNode
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrAddNrBand
(
U8                           cellId,
U8                           numBand,
LwrNeighBandClassCfg        *nbrBandAdd
)
{
   S16                           ret = ROK;
   U8                            bandIndex = 0;
   WrCellCb                      *cellCb  = NULLP;

   TRC2(wrEmmAnrAddNrBand);
  
   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {   
      RLOG0(L_ERROR, " Failed to get cell CB ");
      RETVALUE(RFAILED);
   }    

   if((nbrBandAdd == NULLP))
   {
      RETVALUE(RFAILED); 
   }
   while(bandIndex < numBand)
   {
      switch(nbrBandAdd->bandType)
      {
         case LWR_NEIGH_BAND_TYPE_CDMA_1XRTT:
            ret = wrEmmAnrAddCdma1xRttBandClassLst(cellId, 
                           &nbrBandAdd[bandIndex].t.bandClass1xRtt);
            break;
         default:
            RLOG0(L_WARNING, " Invalid NrType ");
            ret = RFAILED;    
      }  
      bandIndex++;
   }

   if(ret == ROK)
   {
      /* send the list update to EMM module */
      ret = wrEmmAnrProcNrUpdtInfo(cellCb, TRUE);
   }
   else
   {
      RLOG1(L_WARNING, " Failed adding %d band ", nbrBandAdd->bandType);
      ret = RFAILED;    
   }

   RETVALUE(ret); 
} /* end of wrEmmAnrAddNrBand */


/** @brief This function adds the Neighbors CDMA cells into white list
 *         based on the band class and arfcn
 *
 *     Function: wrEmmAnrUpdateCdma1xWlInfo() 
 *
 *         Processing steps:
 *          - Get the band node idx and freq node idx as per the new cell's
 *            band and arfcn
 *          - Add the new cell into the respective white list
 *          
 */
PRIVATE S16 wrEmmAnrUpdateCdma1xWlInfo
(
   WrCdma2kNeighCellCb     *newCell,
   WrCDMA2KNeighInfo     *nrCdma1xRttWlBlInfo,
   WrAnrNrUpdtType         nrUpdtType
)
{
   U8                     bandIndx=0;
   U8                     freqIndx=0;
   U8                     cellIndx=0;
   WrNrCDMA2KPerFreqInfo  *wlData;

   TRC2(wrEmmAnrUpdateCdma1xWlInfo);
   bandIndx = 
   wrEmmAnrGetCdma1xRttBandNodeIdx(nrCdma1xRttWlBlInfo, 
      newCell->bandClass);
 
   /* Klocwork fix for ECSFB */
   if (bandIndx >= WR_MAX_CDMA_BAND_CLASS)
   {
      RLOG0(L_WARNING, " Invalid band index ");
      RETVALUE(RFAILED);
   }
   
   freqIndx = wrEmmAnrGetCdma1xRttFreqNodeIdx(&nrCdma1xRttWlBlInfo->neighBandLst[bandIndx], newCell->arfcn);

   if (freqIndx < WR_MAX_CDMA_FREQ_PER_BAND)
   {
      wlData = 
      &nrCdma1xRttWlBlInfo->neighBandLst[bandIndx].neighFreqInfo[freqIndx].wlData;
      cellIndx = wlData->numWlCells;

      if (cellIndx < WR_MAX_CDMA_1xRTT_CELLS_PER_FREQ)
      {
         wlData->pciInfoLst[cellIndx].pnOffset = newCell->pnOffset;
         wlData->pciInfoLst[cellIndx].nrUpdtType = nrUpdtType;
         wlData->numWlCells++; 
      }
      else
      {
         RLOG0(L_WARNING, " Max cells per freq exceeded ");
         RETVALUE(RFAILED);
      }
   }
   else
   {
      RLOG0(L_WARNING, " Max number of freq supported is exceeded ");
      RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);
}

/** @brief This function adds the Neighbors CDMA cells into white list
 *         based on the band class and arfcn
 *
 *     Function: wrEmmAnrInsertCdma1xNodeIntoWlList() 
 *
 *         Processing steps:
 *          - Get the band node idx and freq node idx as per the new cell's
 *            band and arfcn
 *          - Add the new cell into the respective white list
 *          
 */
PRIVATE Void wrEmmAnrInsertCdma1xNodeIntoWlList
(
   WrCellCb                      *cellCb,
   WrCdma2kNeighCellCb           *newCell
)
{
   WrCDMA2KNeighInfo     *nrCdma1xRttWlBlInfo = NULLP;
   WrNrCdmaFreqCb          *cdmaFreqNode = NULLP;
   CmLList                 *nrLnk = NULLP;
   CmLListCp               *nrLst = NULLP;

   TRC2 (wrEmmAnrInsertCdma1xNodeIntoWlList)

   nrCdma1xRttWlBlInfo = &cellCb->nrCb->nrUpdtInfo->cdma1xRtt;
   cdmaFreqNode = newCell->freqNode;
   nrLnk = &newCell->cdmaNrLLstEnt;

   if (newCell->blackListed == FALSE)
   {
      nrLst = &cdmaFreqNode->nrWlLst;
      cmLListAdd2Tail(nrLst, nrLnk);

      wrEmmAnrUpdateCdma1xWlInfo(newCell,
         nrCdma1xRttWlBlInfo,WR_EMM_ANR_ADD_NR);
   }

   RETVOID;
}

/** @brief This function adds the Neighbors CDMA cells into NRT
 *         based on the band class and arfcn
 *
 *     Function: wrEmmAnrAddCdma1xRttNeighCell() 
 *
 *          
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrAddCdma1xRttNeighCell
(
   WrNrCdmaFreqCb                *freqNode,
   WrCdma2kNeighCellCb           *newCell,
   WrCellCb                      *cellCb
)
{
   U8           maxWlCount;
   U8           maxBlCount;

   TRC2(wrEmmAnrAddCdma1xRttNeighCell);

   maxWlCount = WR_MAX_WCELL_PER_INTERRAT;
   maxBlCount = WR_MAX_BCELL_PER_INTERRAT;

   if (((newCell->blackListed == FALSE) && 
        (freqNode->nrWlLst.count < maxWlCount)) || 
       ((newCell->blackListed == TRUE) && 
        (freqNode->nrBlLst.count < maxBlCount)))
   {
     newCell->cdmaNrLLstEnt.node = (PTR)newCell;
     newCell->freqNode = freqNode;
     /* Add the list in NrUpdtInfo also */
     wrEmmAnrInsertCdma1xNodeIntoWlList(cellCb, newCell);
   }
   else
   {
      RLOG0(L_WARNING, " NRT Count reached MAX. ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* end of wrEmmAnrAddCdma1xRttNeighCell */

/** @brief This function adds the Neighbors CDMA cells into NRT
 *         based on the band class and arfcn
 *
 *     Function: wrEmmAnrAddCdma1xRttNr() 
 *
 *          
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrAddCdma1xRttNr
(
   WrCellCb                      *cellCb,
   WrCdma2kNeighCellCb           *newCell
)
{
   S16                           ret = ROK;
   WrNrCdmaBandCb                *bandNode = NULLP;
   WrNrCdmaFreqCb                *freqNode = NULLP;

   TRC2(wrEmmAnrAddCdma1xRttNr);

   /* Get the band node in cdma1xRttCb */
   bandNode = wrEmmAnrGetCdma1xRttBandNode(&cellCb->nrCb->cdma1xRttCb.cdmaBandList,newCell->bandClass);
   if (bandNode == NULLP)
   {
       RLOG0(L_WARNING, "Failed to get band node ");
       RETVALUE(RFAILED);
   }

   /* Get the freq node in cdmaBandList */
   freqNode = wrEmmAnrGetCdma1xRttFreqNode(&bandNode->cdmaFreqList,newCell->arfcn);
   if (freqNode == NULLP)
   {
       RLOG0(L_WARNING, " Failed to get freq node ");
       RETVALUE(RFAILED);
   }

   /* Add the cell in cdmaFreqList */
   wrEmmAnrAddCdma1xRttNeighCell(freqNode, newCell, cellCb);

   cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=
            WR_ANR_CDMA_1XRTT_FREQ_BIT;

   RETVALUE(ret);
}/* end of wrEmmAnrAddCdma1xRttNr */

/** @brief This function forms the list of neighbour CDMA cells in the 
 *         white list to be used by the PBM to construct SIB8
 *
 *     Function: wrEmmAnrFormCdma1xRttFreqLst()
 *
 *          
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrFormCdma1xRttFreqLst
(
   WrNrWlBlInfo                  *nrWlBlInfo,
   WrCellCb                      *cellCb,
   WrBrdCstInfo                  brInfo
)
{
   WrCDMA2KNeighInfo             *nrCdma1xRttWlBlInfo = NULLP;
                                 
   CmLList                       *freqNodeLnk = NULLP;
   CmLList                       *bandNodeLnk = NULLP;
   WrNrCdmaBandCb                *bandNode = NULLP;
   WrNrCdmaFreqCb                *freqNode = NULLP;
   WrCdma2kNeighCellCb           *nbrCellNode = NULLP;
   CmLList                       *nbrCellLnk = NULLP;
   U8                            bandIndx=0, freqIndx=0;

   TRC2(wrEmmAnrFormCdma1xRttFreqLst)
   
   nrCdma1xRttWlBlInfo = &nrWlBlInfo->cdma1xRtt;
   nrCdma1xRttWlBlInfo->numBandClass = 0;

   bandNodeLnk = cellCb->nrCb->cdma1xRttCb.cdmaBandList.first;
   
   if(NULLP == bandNodeLnk)
   {   
      RLOG0(L_ERROR, " No Bands are present ");
      RETVALUE(RFAILED);
   }    
   
   while(bandNodeLnk != NULLP)
   {
      bandNode = (WrNrCdmaBandCb *)bandNodeLnk->node;
      freqNodeLnk = bandNode->cdmaFreqList.first;
      if (freqNodeLnk == NULLP)
      {
         bandNodeLnk = bandNodeLnk->next;
         continue;
      }
      freqIndx = 0;

      while (freqNodeLnk != NULLP)
      {
         freqNode = (WrNrCdmaFreqCb *)freqNodeLnk->node;
         /* Update the White List */
         nbrCellLnk = freqNode->nrWlLst.first;

         while(nbrCellLnk != NULLP)
         {
            nbrCellNode = (WrCdma2kNeighCellCb *)nbrCellLnk->node;
            if(nbrCellNode != NULLP)
            {
               wrEmmAnrUpdateCdma1xWlInfo(nbrCellNode, 
                  nrCdma1xRttWlBlInfo, WR_EMM_ANR_ADD_NR);
            }
            nbrCellLnk = nbrCellLnk->next;
         }

         /* Update the Black List */
         nbrCellLnk = freqNode->nrBlLst.first;

         while(nbrCellLnk != NULLP)
         {
            nbrCellNode = (WrCdma2kNeighCellCb *)nbrCellLnk->node;
            if(nbrCellNode != NULLP)
            {
               wrEmmAnrUpdateCdma1xWlInfo(nbrCellNode,
                  nrCdma1xRttWlBlInfo, WR_EMM_ANR_ADD_NR);
            }
            nbrCellLnk = nbrCellLnk->next;
         }
         freqIndx++;
         freqNodeLnk = freqNodeLnk->next;
      }
      bandIndx++;
      bandNodeLnk = bandNodeLnk->next;
   }

   RETVALUE(ROK);
} /* end of wrEmmAnrFormCdma1xRttFreqLst */ 

/** @brief This function returns the CDMA 1xRTT cell corresponding to 
 * provided band and frequency
 *
 *     Function: wrEmmAnrGetCdma1xRttCell()
 *
 *          
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrGetCdma1xRttCell
(
   U8                     cellId,
   WrCdmaRttPilotLst      *cdmaRttPilotLst
)
{

   WrCellCb                      *cellCb = NULLP;
   CmLList                       *freqNodeLnk = NULLP;
   CmLList                       *bandNodeLnk = NULLP;
   WrNrCdmaBandCb                *bandNode = NULLP;
   WrNrCdmaFreqCb                *freqNode = NULLP;
   WrCdma2kNeighCellCb           *nbrCellNode = NULLP;
   CmLList                       *nbrCellLnk = NULLP;

   TRC2(wrEmmAnrGetCdma1xRttCell)

   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {   
      RLOG0(L_ERROR, "wrEmmAnrGetCdma1xRttCell Failed to get cellCb");
      RETVALUE(RFAILED);
   }    
   bandNodeLnk = cellCb->nrCb->cdma1xRttCb.cdmaBandList.first;

   if(NULLP == bandNodeLnk)
   {   
      RLOG0(L_WARNING, "No Bands are present");
      RETVALUE(RFAILED);
   }    

   while(bandNodeLnk != NULLP)
   {
      bandNode = (WrNrCdmaBandCb *)bandNodeLnk->node;
      if(cdmaRttPilotLst->bandClass != bandNode->bandClass)
      {
         bandNodeLnk = bandNodeLnk->next;    
         continue;
      }
      freqNodeLnk = bandNode->cdmaFreqList.first;

      /* If bandClass doesn't have any frequency then return Failure 
       * As no CDMA 1xRTT cell can be found */
      if (freqNodeLnk == NULLP)
      {
         RLOG0(L_WARNING, "wrSmDfltCdmaRptAmount Failed to get freqNode\n");
         RETVALUE(RFAILED);
      }

      while (freqNodeLnk != NULLP)
      {
         freqNode = (WrNrCdmaFreqCb *)freqNodeLnk->node;
         if(cdmaRttPilotLst->arfcn != freqNode->arfcn)
         {
            /* Frequency is not matching with the provided frequency so go for
             * next frequency in the band */
            freqNodeLnk = freqNodeLnk->next;
            continue;
         }
         /* Update the White List */
         nbrCellLnk = freqNode->nrWlLst.first;

         while(nbrCellLnk != NULLP)
         {
            nbrCellNode = (WrCdma2kNeighCellCb *)nbrCellLnk->node;
            if(nbrCellNode != NULLP)
            {
               /* Copy the First cell in the Frequency list to the output 
                * structure cdmaRttPilotLst */ 
               cdmaRttPilotLst->pnOffset      = nbrCellNode->pnOffset;
               cdmaRttPilotLst->pilotPnPhase  = nbrCellNode->pilotPnPhase;
               cdmaRttPilotLst->pilotStrength = nbrCellNode->pilotStrength;
               RETVALUE(ROK);
            }
            nbrCellLnk = nbrCellLnk->next;
         }
         /* Control will reach here when band and frequency is matched 
          * But no cell found in that band,frequency List, retrun FAILURE */
         RETVALUE(RFAILED);
      }
      /* Control will reach here when band is matched 
       * But no frequency Node found in that band List, retrun FAILURE */
      RETVALUE(RFAILED);
   }
   /* No band matced with the provided band, retrun FAILURE */
   RETVALUE(RFAILED);
} /* end of wrEmmAnrGetCdma1xRttCell */ 


/** @brief This function returns the CDMA 1xRTT cell corresponding to 
 * provided band and frequency
 *
 *     Function: wrEmmAnrGetCdma1xRttCellByPnOffset()
 *
 *          
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrGetCdma1xRttCellByPnOffset
(
   U8                     cellId,
   WrCdma1xRttPnOffset    *cdmaRttPnOffset,
   WrCdma1xRttPilotInfo   *cdmaPilotInfo
)
{

   WrCellCb                      *cellCb = NULLP;
   CmLList                       *freqNodeLnk = NULLP;
   CmLList                       *bandNodeLnk = NULLP;
   WrNrCdmaBandCb                *bandNode = NULLP;
   WrNrCdmaFreqCb                *freqNode = NULLP;
   WrCdma2kNeighCellCb           *nbrCellNode = NULLP;
   CmLList                       *nbrCellLnk = NULLP;

   TRC2(wrEmmAnrGetCdma1xRttCellByPnOffset)

   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {   
      RLOG0(L_ERROR, "wrEmmAnrGetCdma1xRttCellByPnOffset"
                                    "Failed to get cellCb\n");
      RETVALUE(RFAILED);
   }    
   bandNodeLnk = cellCb->nrCb->cdma1xRttCb.cdmaBandList.first;

   if(NULLP == bandNodeLnk)
   {   
      RLOG0(L_ERROR, "wrEmmAnrGetCdma1xRttCellByPnOffset"
                                    "No Bands are present\n");
      RETVALUE(RFAILED);
   }    

   while(bandNodeLnk != NULLP)
   {
      bandNode = (WrNrCdmaBandCb *)bandNodeLnk->node;
      if(cdmaRttPnOffset->bandClass != bandNode->bandClass)
      {
         bandNodeLnk = bandNodeLnk->next;    
         continue;
      }
      freqNodeLnk = bandNode->cdmaFreqList.first;

      /* If bandClass doesn't have any frequency then return Failure 
       * As no CDMA 1xRTT cell can be found */
      if (freqNodeLnk == NULLP)
      {
         RLOG0(L_WARNING, "freqNodeLnk is NULL");
         RETVALUE(RFAILED);
      }

      while (freqNodeLnk != NULLP)
      {
         freqNode = (WrNrCdmaFreqCb *)freqNodeLnk->node;
         if(cdmaRttPnOffset->arfcn != freqNode->arfcn)
         {
            /* Frequency is not matching with the provided frequency so go for
             * next frequency in the band */
            freqNodeLnk = freqNodeLnk->next;
            continue;
         }
         /* Update the White List */
         nbrCellLnk = freqNode->nrWlLst.first;

         while(nbrCellLnk != NULLP)
         {
            nbrCellNode = (WrCdma2kNeighCellCb *)nbrCellLnk->node;
            /* if pnoffset matches then copy sectorId else process the next */
            if((nbrCellNode != NULLP) & (cdmaRttPnOffset->pnOffset == nbrCellNode->pnOffset))
            {
               cdmaPilotInfo->sectorId    = &nbrCellNode->sectorId;
               cdmaPilotInfo->systemType  = nbrCellNode->systemType;
               cdmaPilotInfo->channelNum  = nbrCellNode->channelNum;
               cdmaPilotInfo->pilotOneWay = nbrCellNode->pilotOneWay;
               cdmaPilotInfo->bandClass   = nbrCellNode->bandClass;
               RETVALUE(ROK);
            }
            nbrCellLnk = nbrCellLnk->next;
         }
         /* Control will reach here when band and frequency is matched 
          * But no cell found in that band,frequency List, retrun FAILURE */
         RLOG0(L_WARNING, "Band and Frequency are matched but NO Cell Found in that Band, Frequency List");
         RETVALUE(RFAILED);
      }
      /* Control will reach here when band is matched 
       * But no frequency Node found in that band List, retrun FAILURE */
      RLOG0(L_WARNING, "Band is matched but no frequency Node found in that band List");
      RETVALUE(RFAILED);
   }
   /* No band matced with the provided band, retrun FAILURE */
   RLOG0(L_WARNING, "No band matched with the provided band");
   RETVALUE(RFAILED);
} /* end of wrEmmAnrGetCdma1xRttCell */ 


/********************************************************************30**

           End of file:     fc_emm_ecsfb_anr.c@@/main/TeNB_Main_BR/tenb_brcm_fdd_3.0.1_to_tdd_GA_4.0_merge/1 - Fri Sep 12 10:54:21 2014

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

