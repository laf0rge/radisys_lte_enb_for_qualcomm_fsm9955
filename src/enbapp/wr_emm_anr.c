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
               

     File:     wr_emm_anr.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="EMM";

static int RLOG_MODULE_ID=1;

static int RLOG_FILE_ID=5;

#include "wr.h"            /* Common headers and Data Structures */
#include "wr_emm.h"        /* Environment options */
#include "wr_emm_pbm.h"
#include "cm_os.h"       /* common hashing */
#include "cm_os.x"
#include "wr_emm_neighenb.h"
#include "wr_utils.h"
#include "wr_emm_cfg.h"
/* Default Timer value for EPOCH Timer */
#define WR_EMM_DEFAULT_EPOCH_TIMER_VAL_INSECS    1000
#define WR_EMM_SECOND_VAL_IN_MSECS               1000
#define WR_EMM_OAM_EPOCH_VAL_MIN_INSEC           60
#define WR_EMM_OAM_EPOCH_VAL_MAX_INSEC           7200
#define WR_EMM_OAM_TRICE_VAL_MIN_INSEC           5
#define WR_EMM_OAM_TRICE_VAL_MAX_INSEC           10

EXTERN U32 numTtiTicks;

EXTERN   S16 wrUmmMeasProcNrUpdt
(
    U16                      cellId,
    Bool                     isNewFreq,
    WrNrUpdtInfo             *nrUpdtInfo
);
EXTERN S16 wrEmmAnrAddCdma1xRttNeighFreqLst
(
   U8                            cellId,
   LwrNeighCdma1xFreq            *cdma1xRttFreqLst
);
EXTERN S16 wrEmmAnrFormCdma1xRttFreqLst
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

PRIVATE S16 wrEmmRmvNghCellFrmFreqNode
(
 WrCellCb                      *cellCb,
 WrEutranNeighCellCb           *modNr
);

/**
 *  @brief This function initializes the the ANR module
 *
 *      Function: wrEmmAnrInit
 *
 *          Processing steps:
 *          - Allocate memory for the cell neighbor CB 
 *          - Allocate the memory for the API message pointer used to provide
 *            the Black List and White List.
 *          - Create the serving frequecy node in the EUTRAN CB
 *          - Initialize the Neighbour CB lists and Timers
 *
 *  @param [in] cellCb : Pointer to the cell for which ANR is initialized
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAnrInit
(
   WrCellCb                      *cellCb
)
{
   TRC2(wrEmmAnrInit);

   /* Allocate memory for the Nr CB pointer */
   WR_ALLOC(&cellCb->nrCb, sizeof(WrCellNrCb));
   if(cellCb->nrCb == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Allocate memory for the PBM & MCH interface api pointer */
   WR_ALLOC(&cellCb->nrCb->nrWlBlInfo,sizeof(WrNrWlBlInfo));
   if(cellCb->nrCb->nrWlBlInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE(cellCb->nrCb, sizeof(WrCellNrCb));
      RETVALUE(RFAILED);
   }
   
   /* Allocate memory for the PBM & MCH interface api pointer */
   WR_ALLOC(&cellCb->nrCb->nrUpdtInfo,sizeof(WrNrUpdtInfo));
   if(cellCb->nrCb->nrUpdtInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE(cellCb->nrCb, sizeof(WrCellNrCb));
      RETVALUE(RFAILED);
   }

   /* Initialize Intra frequency node pointer and other neighbor list CP */
   cellCb->nrCb->eutranCb.intraFreqCb.earfcn = cellCb->dlCarrierFreq;

   cmLListInit(&cellCb->nrCb->eutranCb.intraFreqCb.nrWlLst);
   cmLListInit(&cellCb->nrCb->eutranCb.intraFreqCb.nrBlLst);

   /* Initialize the Inter frequency list */
   cmLListInit(&cellCb->nrCb->eutranCb.interFreqList);

   /* Initialize the Utran FDD frequency list */
   cmLListInit(&cellCb->nrCb->utranCb.utranFreqList);
	
   /* Initialize the Utran TDD frequency list */
   cmLListInit(&cellCb->nrCb->utranTddCb.utranFreqList);

   cmLListInit(&cellCb->nrCb->geranCb.geranNrCellLst);
   /* Initialize the epoch and Trice Timers */
   cmInitTimers(&(cellCb->nrCb->epochTmr), 1);
   cmInitTimers(&(cellCb->nrCb->triceTmr), 1);
   cmInitTimers(&(cellCb->nrCb->cellValidAgeTmr), 1);
   RETVALUE(ROK);
} /* end of wrEmmAnrInit */ 

/**
 *  @brief This function removes the nodes from the Linked list and check the
 *         expiry of default period timers, if not expired stop the timers.
 *
 *      Function: wrEmmAnrUtranFreeLList
 *
 *          Processing steps:
 *          - traverse the neighbor cell list
 *          - for each list node
 *             - remove the node from list
 *             - free the memory
 *
 *  @param [in] lst : Pointer to UTRAN NRT linked list to be freed
 *
 *  @return None
 */
PRIVATE Void wrEmmAnrUtranFreeLList
(
   CmLListCp                     *lst
)
{
   WrUtraNeighCellCb           *utranNr = NULLP;
   CmLList                      *lnk = NULLP;

   TRC2(wrEmmAnrUtranFreeLList);

   /* get the first entry of the hash list */
   lnk = lst->first;

   while(lnk != NULLP)
   {
      utranNr = (WrUtraNeighCellCb *)lnk->node;

      if(utranNr->utraSI.val != NULLP)
      {
         WR_FREE(utranNr->utraSI.val,utranNr->utraSI.len);
      }
      /* delete the node from List */
      cmLListDelFrm(lst, lnk);

      WR_FREE(utranNr, sizeof(WrUtraNeighCellCb));

      lnk = lst->first;
   }

   RETVOID;
}/* end of wrEmmAnrUtranFreeLList */

/**
 *  @brief This function delete the UTRAN Frequency Nodes
 *
 *      Function: wrEmmAnrDelUtranNeighFreq
 *
 *          Processing steps:
 *          - free WL and BL
 *          - free the freq Node memory
 *
 *
 *  @param [in] utranFreqNode : The Frequency Node to be deleted
 *  @return None
 */

PRIVATE Void  wrEmmAnrDelUtranNeighFreq
(
   WrNrUtranFreqCb            *utranFreqNode,
   CmLListCp                  *freqList
)
{

   /* Free The node from NRT and  Free teh NR Entry */
   wrEmmAnrUtranFreeLList(&utranFreqNode->nrWlLst);

   cmLListDelFrm(freqList, &utranFreqNode->freqLstEnt);

   WR_FREE(utranFreqNode, sizeof(WrNrUtranFreqCb));

   RETVOID;
} /* end of wrEmmAnrDelUtranNeighFreq */


/**
 *  @brief This function frees the EUTRAN frequency nodes and the list of 
 *         cells within the freq.
 *
 *      Function: wrAnrFreeUtranNeighbors
 *
 *          Processing steps:
 *          - traverse through the frequency node list
 *          - for each node
 *             - call Function to delete the Node
 *
 *  @param [in] eutranCb : pointer to Eutran Neighbor Cb
 *
 *  @return None
 */
PRIVATE Void wrAnrFreeUtranNeighbors
(
   WrNrUtranCb                  *utranCb
)
{
   CmLListCp                  *lst = NULLP;
   CmLList                    *lnk = NULLP;
   WrNrUtranFreqCb            *utranFreqNode = NULLP;

   TRC2(wrAnrFreeUtranNeighbors);
   
   lst = &utranCb->utranFreqList;

   lnk = lst->first;

   while(lnk != NULLP)
   {
      utranFreqNode = (WrNrUtranFreqCb *)lnk->node;
      
      wrEmmAnrDelUtranNeighFreq(utranFreqNode, lst);

      lnk = lst->first;
   }

   RETVOID;
   
} /* end of wrAnrFreeUtranNeighbors */ 

/**
 *  @brief This function frees the GERAN list of 
 *         cells within the list ofnieghbor control block.
 *
 *      Function: wrAnrFreeGeranNeighbors
 *
 *          Processing steps:
 *          - traverse through the cells node list
 *          - for each node
 *             - call Function to delete the Node
 *
 *  @param [in] eutranCb : pointer to Geran Neighbor Cb
 *
 *  @return None
 */
PRIVATE Void wrAnrFreeGeranNeighbors
(
   WrNrGeranCb                  *geranCb
)
{
   CmLListCp                  *lst = NULLP;
   CmLList                    *lnk = NULLP;
   WrGeranCellCb             *geranCellCb = NULLP;

   TRC2(wrAnrFreeGeranNeighbors);
   
   lst = &geranCb->geranNrCellLst;

   lnk = lst->last;
   /* Traversing geran Cell list and free each node */
   while(lnk != NULLP)
   {
      /* Delete the Geran SI if it present */
      geranCellCb = (WrGeranCellCb *)lnk->node;
      if(geranCellCb->noOfsiPsi)
      {
         wrEmmDelGeranSi(geranCellCb);
      }
      cmLListDelFrm(lst, lnk);
      if(geranCellCb->nco.pres == PRSNT_NODEF)
      {
         WR_FREE(geranCellCb->nco.val, (geranCellCb->nco.len +7)/8);
      }
      WR_FREE(lnk->node, sizeof(WrGeranCellCb));
      lnk = lst->last;
   }

   RETVOID;
   
} /* end of wrAnrFreeGeranNeighbors */ 

/**
 *  @brief This function removes the nodes from the Linked list and check the
 *         expiry of default period timers, if not expired stop the timers.
 *
 *      Function: wrEmmAnrEutranFreeLList
 *
 *          Processing steps:
 *          - traverse the neighbor cell list
 *          - for each list node
 *             - remove the node from list
 *             - free the memory
 *
 *  @param [in] lst : Pointer to EUTRAN NRT linked list to be freed
 *  
 *  @return None
 */
PRIVATE Void wrEmmAnrEutranFreeLList
(
   CmLListCp                     *lst
)
{
   WrEutranNeighCellCb           *eutranNr = NULLP;
   CmLList                       *lnk = NULLP;
   
   TRC2(wrEmmAnrEutranFreeLList);

   /* get the first entry of the hash list */
   lnk = lst->first;
   
   while(lnk != NULLP)
   {
      eutranNr = (WrEutranNeighCellCb *)lnk->node;

      /* Delete the Node from the Serving Cell list of Enb */
      if(ROK == wrEmmDelNrFrmServCellLst(eutranNr))
      {
         /* delete the node from List */
         cmLListDelFrm(lst, lnk);
         WR_FREE(eutranNr, sizeof(WrEutranNeighCellCb));
      }
      lnk = lst->first;
   }      

   RETVOID;
}/* end of wrEmmAnrEutranFreeLList */



/**
 *  @brief This function delete the EUTRAN Frequency Node
 *
 *      Function: wrEmmAnrDelEutranNeighFreq
 *
 *          Processing steps:
 *          - free Whiete List and Black List
 *          - free the freq Node memory
 *
 *
 *  @param [in] eutranFreqNode : The Frequency Node to be deleted 
 *  @return None
 */

PRIVATE Void  wrEmmAnrDelEutranNeighFreq
(
   WrNrEutranFreqCb           *eutranFreqNode,
   CmLListCp                  *freqList
)
{
   /* Free The node from WL/BL */
   wrEmmAnrEutranFreeLList(&eutranFreqNode->nrWlLst);
   wrEmmAnrEutranFreeLList(&eutranFreqNode->nrBlLst);
   
   cmLListDelFrm(freqList, &eutranFreqNode->freqLstEnt);

   WR_FREE(eutranFreqNode, sizeof(WrNrEutranFreqCb));

   RETVOID;
} /* end of wrEmmAnrDelEutranNeighFreq */ 



/**
 *  @brief This function frees the EUTRAN frequency nodes and the list of 
 *         cells within the freq.
 *
 *      Function: wrAnrFreeEutranNeighbors
 *
 *          Processing steps:
 *          - traverse through the frequency node list
 *          - for each node
 *             - call Function to delete the Node
 *
 *  @param [in] eutranCb : pointer to Eutran Neighbor Cb
 *
 *  @return None
 */
PRIVATE Void wrAnrFreeEutranNeighbors
(
   WrNrEutranCb                  *eutranCb
)
{
   CmLListCp                  *lst = NULLP;
   CmLList                    *lnk = NULLP;
   WrNrEutranFreqCb           *eutranFreqNode = NULLP;

   TRC2(wrAnrFreeEutranNeighbors);
   
   wrEmmAnrEutranFreeLList(&eutranCb->intraFreqCb.nrWlLst);
   wrEmmAnrEutranFreeLList(&eutranCb->intraFreqCb.nrBlLst);

   lst = &eutranCb->interFreqList;

   lnk = lst->first;

   while(lnk != NULLP)
   {
      eutranFreqNode = (WrNrEutranFreqCb *)lnk->node;
      
      wrEmmAnrDelEutranNeighFreq(eutranFreqNode, lst);

      lnk = lst->first;
   }

   RETVOID;
   
} /* end of wrAnrFreeEutranNeighbors */ 

/*
   * This function frees the CDMA bands, frequencies nodes and the list of 
    * cells within the freq.
 *
 *      Function: wrAnrFreeCdmaNeighbors
 *
 *          Processing steps:
 *          - traverse through the bandList node list
 *          - for each node
 *             - call Function to delete the frequency
 *                - In called function go through the neighbor list
 *                  and delete each neighbor from the list and free the memory
 *
 *  @param [in] cdmaCb : pointer to cdma Neighbor Cb
 *
 *  @return None
 */
PRIVATE Void wrAnrFreeCdmaNeighbors
(
   WrCellCb          *cellCb
)
{
   CmLListCp                  *bandLstCp    = NULLP;
   CmLListCp                  *freqLstCp    = NULLP;
   CmLListCp                  *nbrCellLstCp = NULLP;
   CmLList                    *node         = NULLP;
   CmLList                    *node1        = NULLP;
   CmLList                    *node2        = NULLP;
   /* To deallocate the sector Id of CSFB configuration */
   TknStrOSXL                  *ptr = NULLP;

   TRC2(wrAnrFreeCdmaNeighbors);

   /* Deleting CDMA neighbour in NRT */

   bandLstCp = &cellCb->nrCb->cdma1xRttCb.cdmaBandList;
   node = bandLstCp->last;
   while (node != NULLP)
   {
      freqLstCp = &((WrNrCdmaBandCb*)node->node)->cdmaFreqList;
      node1 = freqLstCp->last;
      while (node1 != NULLP)
      {
         nbrCellLstCp = &((WrNrCdmaFreqCb*)node1->node)->nrWlLst;
         node2 = nbrCellLstCp->last;
         while (node2 != NULLP)
         {
            WrCdma2kNeighCellCb          *cdmaNeighCell = NULLP;
            cdmaNeighCell= (WrCdma2kNeighCellCb *)node2->node;

            ptr = &(cdmaNeighCell->sectorId);
            if((TRUE == ptr->pres) && (NULLP != ptr->val))
            {
               U8 len;
               len = ptr->len;
               WR_FREE(ptr->val, len * sizeof(U8 ));
            }
            cmLListDelFrm(nbrCellLstCp, node2);
            WR_FREE(node2->node, sizeof(WrCdma2kNeighCellCb));

            node2 = nbrCellLstCp->last;
         }

         /* All white list cell are deleted for a particular frequency
          * in a band, now deleting the black list cdma cells */
         nbrCellLstCp = NULLP;
         nbrCellLstCp = &((WrNrCdmaFreqCb*)node1->node)->nrBlLst;
         node2 = nbrCellLstCp->last;
         while (node2 != NULLP)
         {
            cmLListDelFrm(nbrCellLstCp, node2);
            WR_FREE(node2->node, sizeof(WrCdma2kNeighCellCb));
            node2 = nbrCellLstCp->last;
         }
         cmLListDelFrm(freqLstCp, node1);
         WR_FREE(node1->node, sizeof(WrNrCdmaFreqCb));
         node1 = freqLstCp->last;
      } /* Ends for frequency list */
      cmLListDelFrm(bandLstCp, node);
      WR_FREE(node->node, sizeof(WrNrCdmaBandCb));
      node = bandLstCp->last;
   } /* Ends for band list */
   RETVOID;
} /* End of wrAnrFreeCdmaNeighbors */

/**
 *  @brief This function initializes the the ANR module
 *
 *      Function: wrEmmAnrDeInit
 *
 *          Processing steps:
 *          - Free the memory for the API message pointer used to provide
 *            the Black List abd White List.
 *          - Free the lists
 *          - Free the memory of the cell neighbor CB pointer
 *
 *  @param [in] cellId : Id of the cell for which ANR is initialized
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrDeInit
(
   U8                            cellId
)
{
   S16                        ret     = ROK;
   WrCellCb                   *cellCb  = NULLP;

   TRC2(wrEmmAnrDeInit);
  
   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb || NULLP == cellCb->nrCb)
   {   
       RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrDeInit: Failed to get cell CB");
       RETVALUE(RFAILED);
   }    
   WR_FREE(cellCb->nrCb->nrWlBlInfo,sizeof(WrNrWlBlInfo));
   WR_FREE(cellCb->nrCb->nrUpdtInfo,sizeof(WrNrUpdtInfo));

   /* This function frees the EUTRAN frequency nodes and the list of 
    * cells within the freq. */
   wrAnrFreeEutranNeighbors(&cellCb->nrCb->eutranCb);
   
   /* This function frees the UTRAN FDD frequency nodes and the list of 
    * cells within the freq. */
   wrAnrFreeUtranNeighbors(&cellCb->nrCb->utranCb);

	   /* This function frees the UTRAN TDD frequency nodes and the list of 
    * cells within the freq. */
   wrAnrFreeUtranNeighbors(&cellCb->nrCb->utranTddCb);

   /* This function frees the list of GERAN cells from nrCb */
   wrAnrFreeGeranNeighbors(&cellCb->nrCb->geranCb);

   /* This function frees the CDMA bands, frequencies nodes and the list of 
    * cells within the freq. */
   wrAnrFreeCdmaNeighbors(cellCb);
   /* Free the black lists in NR CB */

   /* Free memory for the Nr CB pointer */
   WR_FREE(cellCb->nrCb, sizeof(WrCellNrCb));

   RETVALUE(ret);
} /* end of wrEmmAnrDeInit */ 

/**
 *  @brief This function initmates the NR Update to PBM and PBM
 *
 *      Function: wrEmmAnrProcessNRUpdt
 *
 *          Processing steps:
 *          - call PBM api to initimate the NR Update
 *          - call MCH api with NR update details
 *
 *
 *  @param [in] cellId     : cell Id of the serving cell
 *  @param [in] nrType     : NR type for the update has happened
 *  @param [in] isNewFreq  : NR Updatet is due to New Freq Addition
 *  @param [in] nrWlBlInfo : pointer to Nr Update info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */


PRIVATE S16 wrEmmAnrProcessNRUpdt
(
   U8                            cellId, 
   Bool                          isNewFreq,
   WrNrUpdtInfo                  *nrUpdtInfo
)
{
   S16                           ret = ROK;

   TRC2(wrEmmAnrProcessNRUpdt);

   if(nrUpdtInfo->skipSibsUpd == FALSE)
   {
      /* Trigger PBM about Nr Update, with corresponding frequency Type set */
      ret = wrEmmPbmTrgNrUpdt(cellId, nrUpdtInfo->freqTypeBitMap);
   }
   if(nrUpdtInfo->skipMeasUpt == FALSE)
   {
      /* Provide the measurement module ith the updated NR details */
      ret = wrUmmMeasProcNrUpdt(cellId, isNewFreq, nrUpdtInfo);
   }
  
   cmMemset((U8 *)nrUpdtInfo,0,sizeof(WrNrUpdtInfo));
 
   RETVALUE(ret);
} /* end of wrEmmAnrProcessNRUpdt */ 

/**
 *  @brief This function sends the nrWlBlInfo in the cellCb for a
 *         particular NR type to EMM
 *
 *      Function: wrEmmAnrProcNrUpdtInfo
 *
 *          Processing steps:
 *          - switch to the particular NR Type
 *             - map the Cfg Nr type to List NrType
 *          -call EMM to update the NR changes
 *
 *  @param  [in] cellCb     : pointer to cell control Block
 *  @param  [in] isNewFreq  : NR Updatet is due to New Freq Addition
 *  @param  [in] nrType     : Nr Type for which the update is done
 *  @return None 
 */
PUBLIC S16 wrEmmAnrProcNrUpdtInfo
(
   WrCellCb                   *cellCb, 
   Bool                       isNewFreq
)
{
   S16                        retVal = ROK;
   
   TRC2(wrEmmAnrProcNrUpdtInfo);

   retVal = wrEmmAnrProcessNRUpdt(cellCb->cellId,  isNewFreq, cellCb->nrCb->nrUpdtInfo);

   RETVALUE(retVal);
}/* end of wrEmmAnrProcNrUpdtInfo */

/**
 *  @brief This function fetches the index of the particular frequency in the
 *         Inter-freq list, that is given to the PBM and MCH module 
 *
 *  @details This function returns index of the existing frequency node in the
 *           WrNrInterFreqInfo structure. If the frequency is not present, then
 *           initializes the next index for that frequency and returns that
 *           index.
 *
 *      Function: wrEmmAnrGetEutranFreqNodeIndx
 *
 *          Processing steps:
 *          - Traverse through the frequency node array
 *             - if node EARFCN matches with required earfcn
 *                - break loop
 *             - else
 *                - increment node index and continue
 *          - if the node doesn't exist, 
 *             - make new node 
 *             - initialize the number of white list and black list entries to
 *               zero
 *             - increment the number of freq elements
 *          - return the freq node Index
 *
 *  @param [in] nrInterWlBlInfo : Inter frequency List Info
 *  @param [in] earfcnDl        : EARFCN for which node index to be fetched
 *  @return freqNodeIndex
 */
PRIVATE U8  wrEmmAnrGetEutranFreqNodeIndx
(
   WrNrInterFreqInfo             *nrInterWlBlInfo, 
   U16                           earfcnDl
)
{
   U8                            freqNodeIndex = 0;
   U8                            maxFreqElem;

   TRC2(wrEmmAnrGetEutranFreqNodeIndx)

   maxFreqElem = nrInterWlBlInfo->numFreq;

   while( freqNodeIndex < maxFreqElem)
   {
      if(nrInterWlBlInfo->freqList[freqNodeIndex].dlCarrierFreq == earfcnDl)
      {
         break;
      }
      freqNodeIndex++;
   }
   if(freqNodeIndex < WR_MAX_INTRA_LTE_FREQ)
   {
      if(freqNodeIndex == maxFreqElem) 
      {
         nrInterWlBlInfo->freqList[freqNodeIndex].dlCarrierFreq = earfcnDl;
         nrInterWlBlInfo->freqList[freqNodeIndex].wlData.numWlCells = 0;
         nrInterWlBlInfo->freqList[freqNodeIndex].blData.numBlInfos = 0;

         nrInterWlBlInfo->numFreq++;
      }
   }
   RETVALUE(freqNodeIndex);
} /* end of wrEmmAnrGetEutranFreqNodeIndx */ 

/**
 *  @brief This function encodes the Inter Freq cell info as white list cell
 *         into the cell Info list, which will be sent to MCH/PBM
 *
 *      Function: wrEmmAnrUpdateInterFreqWlInfo
 *
 *          Processing steps:
 *             - check for the frequency in the current frequency list
 *             - get the frequency node index  
 *             - if freq Index < max num of freq taht can be encoded
 *                - get the cell node index with in the White list
 *                - if cell Index < max num of cells taht can be encoded
 *                   - add the physCellId into the list of that frequency
 *                   - update the nrUpdtType
 *                   - increment number of PCI in the list
 *
 *  @param [in] interNr         : Inter Frequency node to be encoded
 *  @param [in] nrInterWlBlInfo : pointer that points to API list, that is updated
 *                                with the white and Black lsited Inter Freq cells
 *  @param [in] nrUpdtType : used by MCH module only, during NR Update message.
 *                           Flag Indicates whether the cell need to be added or
 *                           deleted from the list
 *  @return None
 */
PRIVATE Void wrEmmAnrUpdateInterFreqWlInfo
(
   WrEutranNeighCellCb           *interNr,
   WrNrInterFreqInfo             *nrInterWlBlInfo,
   WrAnrNrUpdtType               nrUpdtType
)
{
   U8                            freqIndex = 0;
   U8                            cellInfoIndex = 0;
   WrNrPerFreqWlInfo             *wlData = NULLP;

   TRC2(wrEmmAnrUpdateInterFreqWlInfo);

   freqIndex =  wrEmmAnrGetEutranFreqNodeIndx(nrInterWlBlInfo, 
         interNr->earfcnDl);

   if(freqIndex < WR_MAX_INTRA_LTE_FREQ)
   {
      /* Get the cell Details */
      wlData = &nrInterWlBlInfo->freqList[freqIndex].wlData;
      cellInfoIndex = wlData->numWlCells;

      if(cellInfoIndex < WR_MAX_CELL_PER_FREQ)
      {
         wlData->cellInfo[cellInfoIndex].phyCellId = interNr->pci;
         wlData->cellInfo[cellInfoIndex].qoffset = interNr->qoffset;
         wlData->cellInfo[cellInfoIndex].cio = interNr->cio;
         wlData->cellInfo[cellInfoIndex].nrUpdtType = nrUpdtType;

         /* Increment the number of PCI in List */
         wlData->numWlCells++;
      }
      else
      {
         RLOG0(L_WARNING, "wrEmmAnrUpdateInterFreqWlInfo \
                  Max cells per freq exceeded ");
      }
   }
   else
   {
      RLOG0(L_WARNING, "wrEmmAnrUpdateInterFreqWlInfo \
               Max number of freq supported is exceeded ");
   }
   RETVOID;
} /* end of wrEmmAnrUpdateInterFreqWlInfo */ 

/**
 *  @brief This function encodes the Inter Freq cell info as Black list cell
 *         into the cell Info list, which will be sent to MCH/PBM
 *
 *      Function: wrEmmAnrUpdateInterFreqBlInfo
 *
 *          Processing steps:
 *             - check for the frequency in the current frequency list
 *             - get the frequency node index  
 *             - if freq Index < max num of freq taht can be encoded
 *                - get the cell node index the in the Black List
 *                - if cell Index < max num of cells taht can be encoded
 *                   - add the physCellId into the list of that frequency
 *                   - update the nrUpdtType
 *                   - increment number of PCI in the list
 *
 *  @param [in] interNr         : Inter Frequency node to be encoded
 *  @param [in] nrInterWlBlInfo : pointer that points to API list, that is 
 *                                updated with the white and Black lsited 
 *                                Inter Freq cells
 *  @param [in] nrUpdtType      : used by MCH module only, during NR Update 
 *                                message. Flag Indicates whether the cell 
 *                                need to be added or deleted from the list
 *  @return None
 */
PRIVATE Void wrEmmAnrUpdateInterFreqBlInfo
(
   WrEutranNeighCellCb           *interNr,
   WrNrInterFreqInfo             *nrInterWlBlInfo,
   WrAnrNrUpdtType               nrUpdtType
)
{
   U8                            freqIndex = 0;
   U8                            cellInfoIndex = 0;
   WrNrPerFreqBlInfo             *blData = NULLP;

   TRC2(wrEmmAnrUpdateInterFreqBlInfo);
   
   /*
   if(interNr->earfcnDl != cellCb->dlCarrierFreq)
   */
   {
      freqIndex =  wrEmmAnrGetEutranFreqNodeIndx(nrInterWlBlInfo, 
                        interNr->earfcnDl);

      if(freqIndex < WR_MAX_INTRA_LTE_FREQ)
      {
         /* Get the cell Details */
         blData = &nrInterWlBlInfo->freqList[freqIndex].blData;
         cellInfoIndex = blData->numBlInfos;

         if(cellInfoIndex < WR_MAX_CELL_PER_FREQ)
         {
            blData->blInfo[cellInfoIndex].startPhyCellID = interNr->pci;
            blData->blInfo[cellInfoIndex].phyCellIdRangePres = FALSE;
            blData->blInfo[cellInfoIndex].phyCellIdRange = 0;
            blData->blInfo[cellInfoIndex].nrUpdtType = nrUpdtType;

            /* Increment the nu8mber of PCI in List */
            blData->numBlInfos++;
         }
         else
         {
            RLOG0(L_WARNING, "wrEmmAnrUpdateInterFreqBlInfo \
                     Max cells per freq exceeded ");
         }
      }
      else
      {
         RLOG0(L_WARNING,":wrEmmAnrUpdateInterFreqBlInfo \
                  Max number of freq supported is exceeded ");
      }
   }
   /*
   else
   {
         RLOG0(L_ERROR,":wrEmmAnrUpdateInterFreqBlInfo \
                  Serving freq Node received ");
   }*/
   RETVOID;
} /* end of wrEmmAnrUpdateInterFreqBlInfo */ 

/**
 *  @brief This function add the EUTRAN neighbor frequency into the 
 *         neighbor Frequency list
 *
 *      Function: wrEmmAnrAddEutranNeighFreq
 *
 *          Processing steps:
 *             - create new frequency node 
 *             - Add into the freq List
 *
 *  @param  [in] cellCb   : pointer to cell control block
 *  @param  [in] eutranNr : pointer to neighbor algo info configured
 *  @param  [in] newNr    : pointer to neighbor cell info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAnrAddEutranNeighFreq
(
   U16                           earfcnDl,
   CmLListCp                     *eutranFreqLst
)
{
   WrNrEutranFreqCb              *newFreqNode = NULLP;

   TRC2(wrEmmAnrAddEutranNeighFreq);

   /* allocate new frequency Node */
   WR_ALLOC(&newFreqNode, sizeof(WrNrEutranFreqCb));

   if(newFreqNode != NULLP)
   {
      newFreqNode->earfcn = earfcnDl;

      cmLListInit(&newFreqNode->nrWlLst);
      cmLListInit(&newFreqNode->nrBlLst);

      newFreqNode->freqLstEnt.node = (PTR)newFreqNode;

      cmLListAdd2Tail(eutranFreqLst, &newFreqNode->freqLstEnt);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmAnrAddEutranNeighFreq */ 

/** @brief This function adds the Neighbor Frequency List 
 *
 * @details This function is called to add neighbor Frequency list by EMM.
 *          The function receives the number of neighbors and neighbor 
 *          frequency List as input arguements. Returns the status 
 *          success/failure.
 *
 *     Function: wrEmmAnrAddEutranNeighFreqLst()
 *     
 *          Processing steps:
 *          - traverse through the Frequency list
 *          - call fucntion to add each neighbor frequency
 *
 *  @param [in] cellId        : cellId of the serving Cell
 *  @param [in] eutranFreqLst : pointer to neighbor  containing 
 *                                the cfg info and neighbor cell CB  pointer.

 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAnrAddEutranNeighFreqLst
(
   U8                            cellId,
   LwrNeighEutraFreq             *eutranFreqLst
)
{
   S16                           ret = ROK;
   U8                            freqIndex = 0;
   WrCellCb                      *cellCb  = NULLP;
   WrNrInterFreqInfo             *nrInterWlBlInfo = NULLP;

   TRC2(wrEmmAnrAddEutranNeighFreqLst);

   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {   
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrAddEutranNeighFreqLst: Failed to get cell CB");
      RETVALUE(RFAILED);
   }    
   nrInterWlBlInfo = &cellCb->nrCb->nrUpdtInfo->eutran;

   if(eutranFreqLst->earfcnDl != cellCb->dlCarrierFreq)
   {
      ret = wrEmmAnrAddEutranNeighFreq(eutranFreqLst->earfcnDl, 
            &cellCb->nrCb->eutranCb.interFreqList);
   }

   if(ret == RFAILED)
   {
      RLOG0(L_ERROR, "wrEmmAnrAddEutranNeighFreq failed");
      RETVALUE(ret);
   }

   /* Call the Function to Add Frequency Node */
   wrEmmAnrGetEutranFreqNodeIndx(nrInterWlBlInfo, 
         eutranFreqLst->earfcnDl);

   if(eutranFreqLst->earfcnDl == cellCb->dlCarrierFreq)
   {
      cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=  
         WR_ANR_INTRA_FREQ_BIT;
   }
   else
   {
      cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=
         WR_ANR_INTER_FREQ_BIT;
   }

   freqIndex++;

   RETVALUE(ROK);
}/* end of wrEmmAnrAddEutranNeighFreqLst */

/**
 *  @brief This function returns the frequency node of the EUTRAN neighbor list
 *
 *      Function: wrEmmAnrGetEutranFreqNode
 *
 *          Processing steps:
 *          - traverse through the Eutran Freq list
 *          - for each freq node
 *             - if received EARFCN matches with node EARFCN
 *                - return the freqNode
 *          -if no freq node found 
 *             - return NULL
 *
 *  @param [in] earfcnDl : EARFCN for which freq node need to be fetched
 *  @param [in] eutranCb : EUTRAN Control block containing the frequency node list
 *  @return WrNrEutranFreqCb*
 */
PUBLIC WrNrEutranFreqCb* wrEmmAnrGetEutranFreqNode
(
   U16                           earfcnDl, 
   WrNrEutranCb                  *eutranCb
)
{
   CmLList                       *lnk = NULLP;
   WrNrEutranFreqCb              *lnkFreqNode = NULLP;

   TRC2(wrEmmAnrGetEutranFreqNode);

   if(eutranCb->intraFreqCb.earfcn == earfcnDl)
   {
       RETVALUE(&eutranCb->intraFreqCb);
   }
   else
   {
      lnk = eutranCb->interFreqList.first;

      while(lnk != NULLP)
      {
         lnkFreqNode = (WrNrEutranFreqCb*)lnk->node;

         if(lnkFreqNode->earfcn == earfcnDl)
         {
            RETVALUE(lnkFreqNode);
         }
         lnk = lnk->next;
      }
   }
   RETVALUE(NULLP);
} /* end of wrEmmAnrGetEutranFreqNode */ 

/**
 *  @brief This function returns the frequency node of the UTRAN neighbor list
 *
 *      Function: wrEmmAnrGetUtranFreqNode
 *
 *          Processing steps:
 *          - traverse through the Utran Freq list
 *          - for each freq node
 *             - if received ARFCN matches with node ARFCN
 *                - return the freqNode
 *          -if no freq node found
 *             - return NULL
 *
 *  @param [in] arfcnDl : ARFCN for which freq node need to be fetched
 *  @param [in] utranCb : UTRAN Control block containing the frequency node list
 *  @return WrNrUtranFreqCb*
 */
PRIVATE WrNrUtranFreqCb* wrEmmAnrGetUtranFreqNode
(
   U16                           arfcnDl,
   WrNrUtranCb                   *utranCb
)
{
   CmLListCp                     *lst = NULLP;
   CmLList                       *lnk = NULLP;
   WrNrUtranFreqCb               *lnkFreqNode = NULLP;

   TRC2(wrEmmAnrGetUtranFreqNode);

   lst = &utranCb->utranFreqList;
   lnk = lst->first;

   while(lnk != NULLP)
   {
      lnkFreqNode = (WrNrUtranFreqCb*)lnk->node;

      if(lnkFreqNode->arfcn == arfcnDl)
      {
         RETVALUE(lnkFreqNode);
      }
      lnk = lnk->next;
   }

   RETVALUE(NULLP);
} /* end of wrEmmAnrGetUtranFreqNode */
/**
 *  @brief This function returns the frequency node from GERAN neighbor list
 *
 *      Function: wrEmmAnrGetGeranNbr
 *
 *          Processing steps:
 *          - traverse through the Geran cell list
 *                - return the cell CB if arfcn, badInd, ncc and bcc matches.
 *          -if no cell  found
 *             - return NULL
 *
 *  @param [in] cell Id : Cell Control block index
 *  @param [in] arfcn   : ARFCN for which freq node need to be fetched
 *  @param [in] bandInd : band Indicator 
 *  @param [in] ncc     : Network Color Code
 *  @param [in] bcc     : baseStatuion color code
 *  @return WrGeranCellCb*
 */
PUBLIC Void* wrEmmAnrGetGeranNbr 
(
  U16                cellId,
  U16                arfcn,
  U8                 bandInd,
  U8                 ncc,
  U8                 bcc
)
{
   CmLList                  *listNode = NULLP;
   WrCellCb                 *cellCb = NULLP;
   WrGeranCellCb            *neighCellCbtmp = NULLP;
   U8                        bsic = 0x00;

   TRC2(wrEmmAnrGetGeranNbr);

   WR_GET_CELLCB(cellCb, cellId);

   if(cellCb == NULLP)
   {
      RLOG1(L_ERROR, "cellCb is NULLP, cellId = %d", cellId);
      RETVALUE(NULLP);
   }
   neighCellCbtmp = (WrGeranCellCb *)CM_LLIST_FIRST_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,listNode);
   while(neighCellCbtmp != NULLP)
   {
      bsic = neighCellCbtmp->bsic;

      if(((neighCellCbtmp->freqCb != NULLP ) && 
               (arfcn == neighCellCbtmp->freqCb->arfcn) && \
               (WR_GET_NCC_FRM_BSIC(bsic) == ncc) && \
               (WR_GET_BCC_FRM_BSIC(bsic) == bcc)) && \
            ((TRUE == wrUmmChkGeranBandindEql(neighCellCbtmp->freqCb->bandInd,\
                                                                 bandInd)))) 
      {
         break;
      }
      neighCellCbtmp = (WrGeranCellCb *)CM_LLIST_NEXT_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,listNode);
   }
   RETVALUE(neighCellCbtmp);
}

/**
 *  @brief This function returns the frequency node from GERAN neighbor list
 *
 *      Function: wrEmmAnrGetGeranFreqNode
 *
 *          Processing steps:
 *          - traverse through the Geran Freq list
 *          - for each freq node
 *             - if received ARFCN matches with node ARFCN
 *                - return the freqNode
 *          -if no freq node found
 *             - return NULL
 *
 *  @param [in] arfcn : ARFCN for which freq node need to be fetched
 *  @param [in] cellCb : Cell Control block 
 *  @return WrGeranFreqCb*
 */
PRIVATE WrGeranFreqCb* wrEmmAnrGetGeranFreqNode
(
   U16              arfcn,
   WrCellCb         *cellCb
)
{
   CmLList        *neighFreqLnk = NULLP;
   WrGeranFreqCb  *freqNode = NULLP;

   TRC2(wrEmmAnrGetGeranFreqNode);

   neighFreqLnk = cellCb->geranNbrFreqLst.first;
   while(neighFreqLnk != NULLP)
   {
      freqNode = (WrGeranFreqCb*)neighFreqLnk->node;

      if(freqNode->arfcn == arfcn)
      {
         RETVALUE(freqNode);
      }
      neighFreqLnk = neighFreqLnk->next;
   }

   RETVALUE(NULLP);
} /* end of wrEmmAnrGetGeranFreqNode */


/**
 *  @brief This function add the UTRAN neighbor frequency into the
 *         neighbor Frequency list
 *
 *      Function: wrEmmAnrAddUtraNeighFreq
 *
 *          Processing steps:
 *             - create new frequency node
 *             - Add into the freq List
 *
 *  @param  [in] cellCb   : pointer to cell control block
 *  @param  [in] utranNr  : pointer to neighbor algo info configured
 *  @param  [in] newNr    : pointer to neighbor cell info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAnrAddUtraNeighFreq
(
   U16                           arfcnDl,
   CmLListCp                     *utranFreqLst
)
{
   WrNrUtranFreqCb              *newFreqNode = NULLP;

   TRC2(wrEmmAnrAddUtraNeighFreq);

   /* allocate new frequency Node */
   WR_ALLOC(&newFreqNode, sizeof(WrNrUtranFreqCb));

   if(newFreqNode != NULLP)
   {
      newFreqNode->arfcn = arfcnDl;

      cmLListInit(&newFreqNode->nrWlLst);

      newFreqNode->freqLstEnt.node = (PTR)newFreqNode;

      cmLListAdd2Tail(utranFreqLst, &newFreqNode->freqLstEnt);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmAnrAddUtraNeighFreq */


/**
 *  @brief This function fetches the index of the particular frequency in the
 *         Utra-freq list, that is given to the PBM module
 *
 *  @details This function returns index of the existing frequency node in the
 *           WrNrUtraFreqInfo structure. If the frequency is not present, then
 *           initializes the next index for that frequency and returns that
 *           index.
 *
 *      Function: wrEmmAnrGetUtranFreqNodeIndx
 *
 *          Processing steps:
 *          - Traverse through the frequency node array
 *             - if node ARFCN matches with required arfcn
 *                - break loop
 *             - else
 *                - increment node index and continue
 *          - if the node doesn't exist,
 *             - make new node
 *             - initialize the number of white list and black list entries to
 *               zero
 *             - increment the number of freq elements
 *          - return the freq node Index
 *
 *
 *  @param [in] nrUtraWlInfo    : Utra frequency List Info
 *  @param [in] arfcnDl        : ARFCN for which node index to be fetched
 *  @return freqNodeIndex
 */

PRIVATE U8  wrEmmAnrGetUtranFreqNodeIndx
(
   WrNrUtraFreqInfo              *nrUtraWlInfo,
   U16                           arfcnDl
)
{
   U8                            freqNodeIndex = 0;
   U8                            maxFreqElem;

   TRC2(wrEmmAnrGetUtranFreqNodeIndx);

   maxFreqElem = nrUtraWlInfo->numFreq;

   while( freqNodeIndex < maxFreqElem)
   {
      if(nrUtraWlInfo->freqList[freqNodeIndex].dlCarrierFreq == arfcnDl)
      {
         break;
      }
      freqNodeIndex++;
   }
   if(freqNodeIndex < WR_MAX_UTRA_FREQ)
   {
      if(freqNodeIndex == maxFreqElem)
      {
         nrUtraWlInfo->freqList[freqNodeIndex].dlCarrierFreq = arfcnDl;
         nrUtraWlInfo->numFreq++;

      }
   }

   RETVALUE(freqNodeIndex);
} /* end of wrEmmAnrGetUtranFreqNodeIndx */


/** @brief This function adds the Neighbor Frequency List
 *
 * @details This function is called to add neighbor Frequency list by EMM.
 *          The function receives the number of neighbors and neighbor
 *          frequency List as input arguements. Returns the status
 *          success/failure.
 *
 *     Function: wrEmmAnrAddUtranNeighFreqLst()
 *
 *          Processing steps:
 *          - traverse through the Frequency list
 *          - call fucntion to add each neighbor frequency
 *
 *  @param [in] cellId        : cellId of the serving Cell
 *  @param [in] utranFreqLst  : pointer to neighbor  containing
 *                                the cfg info and neighbor cell CB  pointer.

 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PRIVATE S16 wrEmmAnrAddUtranNeighFreqLst
(
   U8                            cellId,
   LwrNeighUtraFreq              *utranFreqLst
)
{
   S16                           ret = ROK;
   U8                            freqIndex = 0;
   WrCellCb                      *cellCb  = NULLP;
   WrNrUtraFreqInfo                *nrUtraFreqInfo = NULLP;

   TRC2(wrEmmAnrAddUtranNeighFreqLst);

   WR_GET_CELLCB(cellCb, cellId);
   if (NULLP == cellCb)
   {   
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrAddUtranNeighFreqLst: Failed to get cell CB ");
      RETVALUE(RFAILED);
   }    

   if(utranFreqLst->duplexMode == WR_UTRA_MODE_FDD)
   {
      nrUtraFreqInfo  = &cellCb->nrCb->nrUpdtInfo->utran;

      ret = wrEmmAnrAddUtraNeighFreq(utranFreqLst->t.utraFdd.arfcnDl,
            &cellCb->nrCb->utranCb.utranFreqList);
      if(ret == RFAILED)
      {
         RETVALUE(ret);
      }

      /* Call the Function to Add Frequency Node */
      wrEmmAnrGetUtranFreqNodeIndx(nrUtraFreqInfo,
            utranFreqLst->t.utraFdd.arfcnDl);
   }
   else
   {
      nrUtraFreqInfo = &cellCb->nrCb->nrUpdtInfo->utranTdd;

      ret = wrEmmAnrAddUtraNeighFreq(utranFreqLst->t.utraTdd.arfcn,
            &cellCb->nrCb->utranTddCb.utranFreqList);
      if(ret == RFAILED)
      {
         RETVALUE(ret);
      }

      /* Call the Function to Add Frequency Node */
      wrEmmAnrGetUtranFreqNodeIndx(nrUtraFreqInfo,
            utranFreqLst->t.utraTdd.arfcn);
   }
   cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=
      WR_ANR_UTRA_FREQ_BIT;

   freqIndex++;

   RETVALUE(ROK);
}/* end of wrEmmAnrAddUtranNeighFreqLst */

/** @brief This function adds the Neighbors Frequencies into NR CB 
 *
 * @details This function is called to add a neighbor by EMM, upon the reception
 *          of new NR trigger from OAM/X2I/MCH. The receives the cell Id
 *          and the neighbor Frequency details as input arguements. The funciton
 *          calls function to Add frequencies of different NR types. And returns
 *          the status success/failure.
 *
 *     Function: wrEmmAnrAddNrFreq() 
 *
 *         Processing steps:
 *          - switch to corresponding NR type
 *          - call fucntion to add Freq Info to Frequency list
 *          
 * @param [in] cellId     : cell for which NR needs to be added
 * @param [in] nbrFreqAdd : NR details containing the cfg info and pointer to 
 *                          cellNode
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrAddNrFreq 
(
    U8                           cellId,
    U8                           numFreq,
    LwrNeighFreqAddModCfg        *nbrFreqAdd
)
{
   S16                           ret = ROK;
   U16                           freqIndex = 0;
   WrCellCb                      *cellCb = NULLP;
   WrNrInterFreqInfo             *nrWlBlInfo = NULLP;
   WrNrUtraFreqInfo               *utraCellInfo = NULLP;

   TRC2(wrEmmAnrAddNrFreq);

   if((nbrFreqAdd == NULLP))
   {
      RLOG0(L_ERROR, "wrEmmAnrAddNrFreq: nbrFreqAdd is NULLP");
      RETVALUE(RFAILED);
   }
   
   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {   
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrAddNrFreq: Failed to get cell CB");
       RETVALUE(RFAILED);
   }    
   nrWlBlInfo = &cellCb->nrCb->nrUpdtInfo->eutran;
   nrWlBlInfo->numFreq = 0;
   
   utraCellInfo = &cellCb->nrCb->nrUpdtInfo->utran;
   utraCellInfo->numFreq = 0;   

   utraCellInfo = &cellCb->nrCb->nrUpdtInfo->utranTdd;
   utraCellInfo->numFreq = 0;   

   while((freqIndex < numFreq)&& (ret == ROK))
   {
      switch(nbrFreqAdd[freqIndex].type)
      {
         case WR_EMM_ANR_EUTRAN :
            ret = wrEmmAnrAddEutranNeighFreqLst(cellId, 
                           &nbrFreqAdd[freqIndex].t.eutranFreq);
            break;
    	 case WR_EMM_ANR_UTRA_FDD:
       case WR_EMM_ANR_UTRA_TDD:
 	         ret = wrEmmAnrAddUtranNeighFreqLst(cellId,
			   &nbrFreqAdd[freqIndex].t.utranFreq);
  	    break;
    	 case WR_EMM_ANR_CDMA_2K_1xRTT:
 	         ret = wrEmmAnrAddCdma1xRttNeighFreqLst(cellId,
			   &nbrFreqAdd[freqIndex].t.cdma1xFreq);
  	    break;
       /* in case of GERAN RAT */
       case WR_EMM_ANR_GERAN:
       {
          /* Note:
           * here only updating bit mask for GERAN neighbor dynamic update but not supported yet 
           * with the bitMask setting PBM update will go i.e. if dynamic update happened then SIB7
           * will be updated and broadcasted with the latest GERAN neighbors info
           * but for connected mode UEs any measurement configuration updates have to be handled */
          cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_GERAN_FREQ_BIT;
       }
       break;
         default:
            RLOG0(L_ERROR,"Received Invalid NrType");
            ret = RFAILED;    
      }   
      freqIndex++;
   }

   if(ret == ROK)
   {
      /* send the list update to EMM module */
      ret = wrEmmAnrProcNrUpdtInfo(cellCb, TRUE);
   }
   else
   {
      RLOG1(L_ERROR,"wrEmmAnrAddNrFreq failed adding %d Freq",
                                    nbrFreqAdd[freqIndex].type);
      ret = RFAILED;    
   }
   
   RETVALUE(ret); 
} /* end of wrEmmAnrAddNrFreq*/

/** @brief This function deletes the Neighbor Frequency List 
 *
 * @details This function is called to add neighbor Frequency list by EMM.
 *          The function receives the number of neighbors and neighbor 
 *          frequency List as input arguements. Returns the status 
 *          success/failure.
 *
 *     Function: wrEmmAnrDelEutranNeighFreqLst()
 *     
 *          Processing steps:
 *          - traverse through the Frequency list
 *          - call fucntion to delete each neighbor frequency
 *
 *  @param [in] cellId        : cellId of the serving Cell
 *  @param [in] eutranFreqLst : pointer to neighbor  containing 
 *                                the cfg info and neighbor cell CB  pointer.

 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAnrDelEutranNeighFreqLst
(
   U8                            cellId,
   LwrNeighEutraDelInfo          *eutranFreqLst
)
{
   WrNrEutranFreqCb              *freqNode = NULLP;
   WrCellCb                      *cellCb  = NULLP;

   TRC2(wrEmmAnrDelEutranNeighFreqLst);

   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {   
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrDelEutranNeighFreqLst: Failed to get cell CB ");
       RETVALUE(RFAILED);
   }    
   /* Get Freq Node */
   freqNode = wrEmmAnrGetEutranFreqNode(eutranFreqLst->earfcn,
            &cellCb->nrCb->eutranCb);

   if(freqNode != NULLP)
   {
      /* Delete The Frequency Node */
      wrEmmAnrDelEutranNeighFreq(freqNode, 
            &cellCb->nrCb->eutranCb.interFreqList);
   }
   else
   {
      RLOG0(L_ERROR, "wrEmmAnrDelEutranNeighFreqLst: wrEmmAnrDelEutranNeighFreq failed.");
      RETVALUE(RFAILED);
   }

   if(eutranFreqLst->earfcn == cellCb->dlCarrierFreq)
   {
      cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=  
         WR_ANR_INTRA_FREQ_BIT;
   }
   else
   {
      cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=
         WR_ANR_INTER_FREQ_BIT;
   }

   RETVALUE(ROK);
}/* end of wrEmmAnrDelEutranNeighFreqLst */

/** @brief This function deletes the Neighbor Frequency List
 *
 * @details This function is called to add neighbor Frequency list by EMM.
 *          The function receives the number of neighbors and neighbor
 *          frequency List as input arguements. Returns the status
 *          success/failure.
 *
 *     Function: wrEmmAnrDelUtraFreqLst()
 *
 *          Processing steps:
 *          - traverse through the Frequency list
 *          - call fucntion to delete each neighbor frequency
 *
 *  @param [in] cellId        : cellId of the serving Cell
 *  @param [in] utranFreqLst : pointer to neighbor  containing
 *                                the cfg info and neighbor cell CB  pointer.

 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAnrDelUtraFreqLst
(
   U8                            cellId,
   LwrNeighUtraDelInfo           *utranFreqLst,
   WrEmmNrType                   nrTyp
)
{
   WrNrUtranFreqCb               *freqNode = NULLP;
   WrCellCb                      *cellCb  = NULLP;

   TRC2(wrEmmAnrDelUtraFreqLst);

   WR_GET_CELLCB(cellCb, cellId);
   if (NULLP == cellCb)
   {   
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrDelUtraFreqLst: Failed to get cell CB");
      RETVALUE(RFAILED);
   }    
   /* Get Freq Node */
   if(WR_EMM_ANR_UTRA_FDD == nrTyp)
   {
      freqNode = wrEmmAnrGetUtranFreqNode(utranFreqLst->arfcn,         
            &cellCb->nrCb->utranCb);
      if(freqNode != NULLP)
      {
         /* Delete The Frequency Node */
         wrEmmAnrDelUtranNeighFreq(freqNode,
               &cellCb->nrCb->utranCb.utranFreqList);
      }
      else
      {
         RLOG0(L_ERROR, "wrEmmAnrDelEutranNeighFreqLst: wrEmmAnrDelEutranNeighFreq failed.");
         RETVALUE(RFAILED);
      }

   }
   else
   {
      freqNode = wrEmmAnrGetUtranFreqNode(utranFreqLst->arfcn,         
            &cellCb->nrCb->utranTddCb);
      if(freqNode != NULLP)
      {
         /* Delete The Frequency Node */
         wrEmmAnrDelUtranNeighFreq(freqNode,
               &cellCb->nrCb->utranTddCb.utranFreqList);
      }
      else
      {
         RLOG0(L_ERROR, "wrEmmAnrDelEutranNeighFreqLst: wrEmmAnrDelUtranNeighFreq failed.");
         RETVALUE(RFAILED);
      }

   }


   if(utranFreqLst->arfcn == cellCb->dlCarrierFreq)
   {
      cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=
         WR_ANR_UTRA_FREQ_BIT;
   }
   else
   {
      cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=
         WR_ANR_UTRA_FREQ_BIT;
   }

   RETVALUE(ROK);
}/* end of wrEmmAnrDelUtranNeighFreqLst */


/** @brief This function deletes the Neighbors Frequencies into NR CB 
 *
 * @details This function is called to add a neighbor by EMM, upon the reception
 *          of new NR trigger from OAM/X2I/MCH. The receives the cell Id
 *          and the neighbor Frequency details as input arguements. The funciton
 *          calls function to Add frequencies of different NR types. And returns
 *          the status success/failure.
 *
 *     Function: wrEmmAnrDelNrFreq() 
 *
 *         Processing steps:
 *          - switch to corresponding NR type
 *          - call fucntion to delete NR to list
 *          
 * @param [in] cellId     : cell for which NR needs to be added
 * @param [in] nbrFreqDel : NR index list to be deleted  
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrDelNrFreq 
(
    U8                           cellId,
    U8                           numFreq,
    LwrNeighFreqDelCfg           *nbrFreqDel
)
{
   S16                           ret = ROK;
   U16                           freqIndex = 0;
   WrCellCb                      *cellCb = NULLP;
   WrNrInterFreqInfo             *nrWlBlInfo = NULLP;
   WrNrUtraFreqInfo               *utraWlInfo = NULLP;

   TRC2(wrEmmAnrDelNrFreq);

   WR_GET_CELLCB(cellCb, cellId)
   if ((NULLP == cellCb) || (NULLP == cellCb->nrCb))
   {   
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrDelNrFreq: Failed to get cell CB and nrCb");
       RETVALUE(RFAILED);
   }    
   nrWlBlInfo = &cellCb->nrCb->nrUpdtInfo->eutran;
   nrWlBlInfo->numFreq = 0;
   
   utraWlInfo = &cellCb->nrCb->nrUpdtInfo->utran;
   utraWlInfo->numFreq = 0;
   
   utraWlInfo = &cellCb->nrCb->nrUpdtInfo->utranTdd;
   utraWlInfo->numFreq = 0;

   if((nbrFreqDel == NULLP))
   {
      RLOG0(L_WARNING, "wrEmmAnrDelNrFreq: nbrFreqDel is NULL");
      RETVALUE(RFAILED);
   }
   
   while((freqIndex < numFreq)&& (ret == ROK))
   {
      switch(nbrFreqDel[freqIndex].type)
      {
         case WR_EMM_ANR_EUTRAN :
            ret = wrEmmAnrDelEutranNeighFreqLst(cellId, 
                        &nbrFreqDel[freqIndex].t.eutranFreq);
            break;
         case WR_EMM_ANR_UTRA_FDD:
            ret = wrEmmAnrDelUtraFreqLst(cellId,
                        &nbrFreqDel[freqIndex].t.utranFreq, WR_EMM_ANR_UTRA_FDD);
            break;
         case WR_EMM_ANR_UTRA_TDD:
            ret = wrEmmAnrDelUtraFreqLst(cellId,
                        &nbrFreqDel[freqIndex].t.utranFreq, WR_EMM_ANR_UTRA_TDD);
            break;
         default:
            RLOG0(L_ERROR,":wrEmmAnrDelNrFreq Invalid NrType");
            ret = RFAILED;    
      }   
      freqIndex++;
   }
   if(ret == ROK)
   {
      if(cellCb->nrCb->nrUpdtInfo!= NULLP)
      {
         cellCb->nrCb->nrUpdtInfo->skipMeasUpt = TRUE;
         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_INTER_FREQ_BIT;
         /* send the list update to EMM module */
         wrEmmAnrProcNrUpdtInfo(cellCb, FALSE);
      }
   }

   RETVALUE(ret); 
} /* end of wrEmmAnrDelNrFreq*/

/**
 *  @brief This function compares the rank of the EUTRAN node and return the
 *         comparision status
 *
 *      Function: wrEmmAnrCmpEutranRank
 *
 *          Processing steps:
 *          - compare the two ranks
 *          - return success if entry rank is less than lnkNode else return
 *            failure
 *
 *  @param [in] lnkNode : link Node  th which comparision to be done
 *  @param [in] entry   : entry node to be compared
 *  @return Bool
 -# Success : TRUE
 -# Failure : FALSE
 */
PRIVATE Bool wrEmmAnrCmpEutranRank
(
   WrEutranNeighCellCb           *lnkNode, 
   WrEutranNeighCellCb           *entry
)
{
   TRC2(wrEmmAnrCmpEutranRank);

   if(entry->curRank > lnkNode->curRank)
   {
      RETVALUE(FALSE);
   }
   else
   {
      RETVALUE(TRUE);
   }
} /* end of wrEmmAnrCmpEutranRank*/ 

/**
 *  @brief This function inserts the EUTRAN node in the list according to its rank
 *
 *  @details Traverse the list, compare the rank and find the position to insert
 *           the node. Insert the node
 *
 *      Function: wrEmmAnrCmpAndInsertListEutranNode 
 *
 *          Processing steps:
 *          - Travese the list provided
 *          - if entry rank not less the tail rank 
 *            - for each node
 *             - compare the rank
 *             - if compare successful (node rank less than entry rank)
 *                - insert the enrty before the node
 *          - else add entry into tail
 *
 *
 *  @param [in] entryLnk : List Entry of the node to be Inseted
 *  @param [in] lst      : List in which the node to be inserted
 *  @return None
 */

PRIVATE Void wrEmmAnrCmpAndInsertListEutranNode
(
   CmLList                       *entryLnk,
   CmLListCp                     *lst
)
{
   CmLList                       *lnk = NULLP;
   Bool                          ret = FALSE;
   
   TRC2(wrEmmAnrCmpAndInsertListEutranNode);

   CM_LLIST_FIRST_NODE(lst,lnk);

   if(lst->last != NULLP)
   {
      /* check if rank of node to be inserted is greater than tail rank */
      ret = wrEmmAnrCmpEutranRank( (WrEutranNeighCellCb*)lst->last->node, 
                                 (WrEutranNeighCellCb*)entryLnk->node);
   }
   if(ret == TRUE)
   {
      /* traverse from start of list */
      while(lnk != NULLP)
      {
         /* check if the entry node rank is greater than lnk node rank */
         ret = wrEmmAnrCmpEutranRank((WrEutranNeighCellCb*)lnk->node, 
                                       (WrEutranNeighCellCb*)entryLnk->node);
         if(ret == TRUE)
         {
            cmLListInsCrnt(lst, entryLnk);
            break;
         }

         CM_LLIST_NEXT_NODE(lst,lnk);
      }
   } 
   else
   {
      cmLListAdd2Tail(lst, entryLnk);
   }

   RETVOID;
} /* end of wrEmmAnrCmpAndInsertListEutranNode */ 

/**
 *  @brief This function removes the node from the NR list
 *
 *  @details The funciton removes the node from the list specified for the 
 *           given nrType. and decrements the corresponding counters.
 *
 *      Function: wrEmmAnrRemEutranNodeFromNrList
 *
 *          Processing steps:
 *          - get the list in which the node is present
 *          - remove the node from the list
 *
 *  @param  [in] cellCb        : pointer to cell control Block
 *  @param  [in] neighCellInfo : list entry to be deleted
 *  @return CmLList*
 */

PRIVATE CmLList* wrEmmAnrRemEutranNodeFromNrList
(
   WrCellCb                      *cellCb,
   WrEutranNeighCellCb           *neighCellInfo
)
{
   WrNrEutranFreqCb              *freqNode = NULLP;
   CmLListCp                     *nrLst = NULLP;
   CmLList                       *entryLnk = NULLP;
   WrNrInterFreqInfo             *nrInterWlBlInfo = NULLP;

   TRC2(wrEmmAnrRemEutranNodeFromNrList);

   nrInterWlBlInfo = &cellCb->nrCb->nrUpdtInfo->eutran;
         
   freqNode = neighCellInfo->freqNode;
   /* Verify whether the frequency node exists in the neighbour */
   if (freqNode == NULLP)
   {
       RLOG0(L_ERROR,"wrEmmAnrRemEutranNodeFromNrList: Failed to get FREQ NODE ");
       RETVALUE(NULLP);
   }
   
   if(neighCellInfo->blackListed == FALSE)
   {
      nrLst = &freqNode->nrWlLst;
      wrEmmAnrUpdateInterFreqWlInfo(neighCellInfo, nrInterWlBlInfo, WR_EMM_ANR_DEL_NR);
   }
   else
   {
      nrLst = &freqNode->nrBlLst;
      wrEmmAnrUpdateInterFreqBlInfo(neighCellInfo, nrInterWlBlInfo, WR_EMM_ANR_DEL_NR);
   }
   
   entryLnk = &neighCellInfo->nrWlBlLnk;
   if (nrLst->count != 0)
   {
      entryLnk = cmLListDelFrm(nrLst, entryLnk);
   }

   RETVALUE(entryLnk);
} /* end of wrEmmAnrRemEutranNodeFromNrList */ 


/**
 *  @brief This function moves the EUTRAN tail node of the NCL to HO Black list of
 *         corresponding NR type
 *
 *      Function: wrEmmAnrMoveEutranNclTailToHOBList
 *
 *          Processing steps:
 *          - delete the node from the NCL of the corresponding NR type
 *          - get the compare fucntion pointer for the nr Type
 *          - insert to the HO Black List of the corresponding NR type
 *
 *  @param [in] cellCb    : pointer to cell CB
 *  @param [in] nrWlLst  : pointer to EUTRAN white list
 *  @param [in] nrBlLst : pointer to EUTRAN Black list
 *  @return None
 */


/**
 *  @brief This function inserts the EUTRAN NR node into the Nr list based on its rank
 *         and configuration.
 *
 *  @details This function gets the current rank. Checks for the HO flag
 *           whether HO is supported or not. Based on the flag and the number of
 *           entries int the NRT and NCl lists, find the nr list in which the
 *           node need to be added. And inserts the node in its position
 *           according to the rank.
 *
 *      Function: wrEmmAnrInsertEutranNodeIntoList
 *
 *          Processing steps:
 *          - get the frequency node
 *          - get the tail rank and node's rank
 *          - check whethr node to be added to White list
 *          - if node to be added in white list and number of NR in NCL 
 *            is equal to max value
 *             - move the tail node to black list
 *          - insert the node at it's position in the list
 *
 *  @param [in] cellCb : pointer to cell control Block
 *  @param [in] nrNode : Node to be inserted
 *  @return None 
 */
PRIVATE Void wrEmmAnrInsertEutranNodeIntoList 
(
   WrCellCb                      *cellCb,
   WrEutranNeighCellCb           *nrNode
)
{

   WrNrEutranFreqCb              *eutranFreqNode = NULLP;
   WrNrInterFreqInfo             *nrInterWlBlInfo = NULLP;
   CmLListCp                     *nrLst = NULLP;
   CmLList                       *nrLnk = NULLP;
   Bool                          isWhiteList = TRUE;

   TRC2(wrEmmAnrInsertEutranNodeIntoList);

   nrInterWlBlInfo = &cellCb->nrCb->nrUpdtInfo->eutran;

   /* Get the frequency node in the current node is residing */
   eutranFreqNode = nrNode->freqNode;
   nrLnk = &nrNode->nrWlBlLnk;
#if 0
#else
   if(nrNode->blackListed == TRUE)
   {
      isWhiteList = FALSE;
   }
#endif

   if(isWhiteList == TRUE)
   {
      nrLst = &eutranFreqNode->nrWlLst;
      wrEmmAnrUpdateInterFreqWlInfo(nrNode, nrInterWlBlInfo, WR_EMM_ANR_ADD_NR);
      /* insert the node into the list */
      wrEmmAnrCmpAndInsertListEutranNode(nrLnk, nrLst);
   }
   else
   {
      nrLst = &eutranFreqNode->nrBlLst; 
      wrEmmAnrUpdateInterFreqBlInfo(nrNode, nrInterWlBlInfo, WR_EMM_ANR_ADD_NR);
      /*update time stamp*/
      osGetTimeOfDay(&nrNode->timeStamp.timeSeconds,&nrNode->timeStamp.timeMs);
      cmLListAdd2Tail(nrLst,nrLnk);
   }


   RETVOID;
} /* end of wrEmmAnrInsertEutranNodeIntoList */ 

/**
 *  @brief This function updates and add the EUTRAN cell into the 
 *         neighbor list, white/black list of the frequency node, of
 *         its freq
 *
 *      Function: wrEmmAnrAddEutranNr
 *
 *          Processing steps:
 *          - get the frequency node, of the cell frequency 
 *          - check the number of entry in the list with max number of entries
 *             supported
 *          - Update the Algo params provided
 *          - add cell into the NRT list
 *          - insert the cell into White/Black list
 *          - if number of entries reaches max, notify OAM/EMS
 *
 *  @param  [in] cellCb   : pointer to cell control block
 *  @param  [in] newNr    : pointer to neighbor cell info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrAddEutranNr
(
   WrCellCb                      *cellCb, 
   WrEutranNeighCellCb           *newNr
)
{
   S16                           ret = ROK;
   U32                           maxNrCount;
   U32                           freqType;
   WrNrEutranFreqCb              *freqNode = NULLP;

   TRC2(wrEmmAnrAddEutranNr);

   if(newNr->earfcnDl == cellCb->dlCarrierFreq)
   {
      freqType = WR_ANR_INTRA_FREQ_BIT;
      maxNrCount = WR_MAX_WCELL_PER_INTRA;

      freqNode =  &cellCb->nrCb->eutranCb.intraFreqCb;
   }
   else
   {
      freqType =  WR_ANR_INTER_FREQ_BIT;
      maxNrCount = WR_MAX_WCELL_PER_INTER;

      freqNode = wrEmmAnrGetEutranFreqNode(newNr->earfcnDl, 
            &cellCb->nrCb->eutranCb);
   }

   if(freqNode != NULLP)
   {
/*      if(freqNode->nrWlLst.count < maxNrCount)*/
      if(((newNr->blackListed == FALSE) &&(freqNode->nrWlLst.count < maxNrCount)) ||
          ((newNr->blackListed == TRUE) && (freqNode->nrBlLst.count < maxNrCount)))
      {
         newNr->curRank      = cellCb->nrCb->maxRnkWght;
         newNr->nrWlBlLnk.node = (PTR)newNr;
         /* back reference to frequency Node */
         newNr->freqNode = freqNode;

         /* Update the timestamp at which the node is created */
         newNr->numTtiTicks = numTtiTicks;

         /* Insert hte node in the NCL according to the rank */
         wrEmmAnrInsertEutranNodeIntoList(cellCb, newNr);

         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= freqType;
#ifdef WR_RSYS_OAM
         if( wrEmmSendAddEutraNghInd(newNr,WR_ACT_ADD) != ROK)
         {
            RLOG0(L_ERROR,"Failed to send an indication to OAM");
            ret = RFAILED;
         }
#endif

      }
      else
      {
         RLOG0(L_WARNING,"wrEmmAnrAddEutranNr NRT Count \
                  reached MAX.");
         ret = RFAILED;
      }
   }/* if(freqNode != NULLP) */
   else
   {
      RLOG1(L_WARNING,"wrEmmAnrAddEutranNr Failed to get \
               %d freq Node.", newNr->earfcnDl);
      ret = RFAILED;
   } 

   RETVALUE(ret);
}/* end of wrEmmAnrAddEutranNr */

/**
 *  @brief This function encodes the Utra Freq cell info as white list cell
 *         into the cell Info list, which will be sent to MCH/PBM
 *
 *      Function: wrEmmAnrUpdateUtraFreqWlInfo
 *
 *          Processing steps:
 *             - check for the frequency in the current frequency list
 *             - get the frequency node index
 *             - if freq Index < max num of freq taht can be encoded
 *                - get the cell node index with in the White list
 *                - if cell Index < max num of cells taht can be encoded
 *                   - add the psc into the list of that frequency
 *                   - update the nrUpdtType
 *                   - increment number of Psc in the list
 *
 *  @param [in] interNr         : Utra Frequency node to be encoded
 *  @param [in] nrInterWlInfo   : pointer that points to API list, that is updated
 *                                with the white lsited Utra Freq cells
 *  @param [in] nrUpdtType : used by MCH module only, during NR Update message.
 *                           Flag Indicates whether the cell need to be added or
 *                           deleted from the list
 *  @return None
 */
PRIVATE Void wrEmmAnrUpdateUtraFreqWlInfo
(
   WrUtraNeighCellCb             *utraNr,
   WrNrUtraFreqInfo              *nrUtraWlInfo,
   WrAnrNrUpdtType               nrUpdtType
)
{
   U8                            freqIndex = 0;
   U8                            cellInfoIndex = 0;
   U16                           psc = 0;
   WrNrUtraPerFreqWlInfo         *wlData = NULLP;

   TRC2(wrEmmAnrUpdateUtraFreqWlInfo);

   if(WR_UTRA_MODE_FDD == utraNr->duplexMode)
   {
      freqIndex =  wrEmmAnrGetUtranFreqNodeIndx(nrUtraWlInfo,
            utraNr->t.utraFdd.arfcnDl);
      psc = utraNr->t.utraFdd.psc;
   }
   else
   {
      freqIndex =  wrEmmAnrGetUtranFreqNodeIndx(nrUtraWlInfo,
            utraNr->t.utraTdd.arfcn);
      psc = utraNr->t.utraTdd.psc;
   }

   if(freqIndex < WR_MAX_UTRA_FREQ)
   {
      /* Get the cell Details */
      wlData = &nrUtraWlInfo->freqList[freqIndex].wlData;
      cellInfoIndex = wlData->numWlCells;

      if(cellInfoIndex < WR_MAX_CELL_PER_FREQ)
      {
         wlData->cellInfo[cellInfoIndex].psc = psc;
         wlData->cellInfo[cellInfoIndex].qoffset = utraNr->qoffset;
         wlData->cellInfo[cellInfoIndex].nrUpdtType = nrUpdtType;
         /* Increment the number of PSC in List */
         wlData->numWlCells++;
      }
      else
      {
         RLOG0(L_WARNING, "wrEmmAnrUpdateUtraFreqWlInfo \
               Max cells per freq exceeded ");
      }
   }
   else
   {
      RLOG0(L_WARNING, "wrEmmAnrUpdateUtraFreqWlInfo \
            Max number of freq supported is exceeded ");
   }
   RETVOID;
} /* end of wrEmmAnrUpdateUtraFreqWlInfo */



/**
 *  @brief This function compares the rank of the EUTRAN node and return the
 *         comparision status
 *
 *      Function: wrEmmAnrCmpUtranRank
 *
 *          Processing steps:
 *          - compare the two ranks
 *          - return success if entry rank is less than lnkNode else return
 *            failure
 *
 *  @param [in] lnkNode : link Node  th which comparision to be done
 *  @param [in] entry   : entry node to be compared
 *  @return Bool
    -# Success : TRUE
    -# Failure : FALSE
 */
PRIVATE Bool wrEmmAnrCmpUtranRank
(
   WrUtraNeighCellCb         *lnkNode, 
   WrUtraNeighCellCb         *entry
)
{
   TRC2(wrEmmAnrCmpUtranRank);

   if(entry->curRank > lnkNode->curRank)
   {
      RETVALUE(FALSE);
   }
   else
   {
      RETVALUE(TRUE);
   }
} /* end of wrEmmAnrCmpUtranRank*/ 

/**
 *  @brief This function inserts the EUTRAN node in the list according to its rank
 *
 *  @details Traverse the list, compare the rank and find the position to insert
 *           the node. Insert the node
 *
 *      Function: wrEmmAnrCmpAndInsertListEutranNode
 *
 *          Processing steps:
 *          - Travese the list provided
 *          - if entry rank not less the tail rank
 *            - for each node
 *             - compare the rank
 *             - if compare successful (node rank less than entry rank)
 *                - insert the enrty before the node
 *          - else add entry into tail
 *
 *
 *  @param [in] entryLnk : List Entry of the node to be Inseted
 *  @param [in] lst      : List in which the node to be inserted
 *  @return None
 */

PRIVATE Void wrEmmAnrCmpAndInsertListUtraNode
(
   CmLList                       *entryLnk,
   CmLListCp                     *lst
)
{
   CmLList                       *lnk = NULLP;
   Bool                           isEntryNodeGreater = FALSE;

   TRC2(wrEmmAnrCmpAndInsertListUtraNode);

   lnk = lst->first;

   if(lst->last != NULLP)
   {
   /* check if rank of node to be inserted is greater than tail rank */
   isEntryNodeGreater  = wrEmmAnrCmpUtranRank( (WrUtraNeighCellCb*)lst->last->node,
                                 (WrUtraNeighCellCb*)entryLnk->node);
   }
   if(isEntryNodeGreater == TRUE)
   {
      /* traverse from start of list */
      while(lnk != NULLP)
      {
         /* check if the entry node rank is greater than lnk node rank */
         isEntryNodeGreater = wrEmmAnrCmpUtranRank((WrUtraNeighCellCb*)lnk->node,
                                       (WrUtraNeighCellCb*)entryLnk->node);
         if(isEntryNodeGreater == TRUE)
         {
            cmLListInsCrnt(lst, entryLnk);
            break;
         }
         CM_LLIST_NEXT_NODE(lst,lnk);
      }
   }
   else
   {
      cmLListAdd2Tail(lst, entryLnk);
   }

   RETVOID;
} /* end of wrEmmAnrCmpAndInsertListUtraNode */

/**
 *  @brief This function inserts the UTRAN NR node into the Nr list based on its rank
 *         and configuration.
 *
 *  @details This function gets the current rank. Checks for the HO flag
 *           whether HO is supported or not. Based on the flag and the number of
 *           entries int the NRT , find the nr list in which the
 *           node need to be added. And inserts the node in its position
 *           according to the rank.
 *
 *      Function: wrEmmAnrInsertUtranNodeIntoWlList
 *
 *          Processing steps:
 *          - get the frequency node
 *          - get the tail rank and node's rank
 *          - check whethr node to be added to White list
 *            is equal to max value
 *          - insert the node at it's position in the list
 *
 *  @param [in] cellCb : pointer to cell control Block
 *  @param [in] nrNode : Node to be inserted
 *  @return None
 */

PRIVATE Void wrEmmAnrInsertUtranNodeIntoWlList
(
   WrCellCb                   *cellCb,
   WrUtraNeighCellCb          *nrNode
)
{
   WrNrUtranFreqCb             *utranFreqNode = NULLP;
   WrNrUtraFreqInfo            *nrUtraWlInfo = NULLP;

   CmLListCp                  *nrLst = NULLP;
   CmLList                     *nrLnk = NULLP;

   TRC2(wrEmmAnrInsertUtranNodeIntoWlList);

   if(WR_UTRA_MODE_FDD == nrNode->duplexMode)
   {
      nrUtraWlInfo = &cellCb->nrCb->nrUpdtInfo->utran;
   }
   else
   {
      nrUtraWlInfo = &cellCb->nrCb->nrUpdtInfo->utranTdd;
   }

   /*get the current frequency node */
   utranFreqNode = nrNode->freqNode;
   nrLnk = &nrNode->nrWlBlLnk;

   nrLst = &utranFreqNode->nrWlLst;
   wrEmmAnrUpdateUtraFreqWlInfo(nrNode, nrUtraWlInfo, WR_EMM_ANR_ADD_NR);

   /* insert the node into the list */
   wrEmmAnrCmpAndInsertListUtraNode(nrLnk, nrLst);

   RETVOID;
}

/**
 *  @brief This function updates and adds the UTRAN cell into the
 *         neighbor list
 *
 *      Function: wrEmmAnrAddUtraNr
 *
 *          Processing steps:
 *          - get the frequency node, of the cell frequency
 *          - check the number of entry in the list with max number of entries
 *             supported
 *          - Update the Algo params provided
 *          - add cell into the NRT list
 *          - if number of entries reaches max, notify OAM/EMS
 *
 *  @param  [in] cellCb   : pointer to cell control block
 *  @param  [in] utranNr  : pointer to neighbor algo info configured
 *  @param  [in] newNr    : pointer to neighbor cell info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrAddUtraNr
(
   WrCellCb                      *cellCb,
   WrUtraNeighCellCb             *newNr
)
{
   S16                           ret = ROK;
   WrNrUtranFreqCb              *freqNode = NULLP;

   TRC2(wrEmmAnrAddUtraNr);

   if(WR_UTRA_MODE_FDD == newNr->duplexMode)
   {
      freqNode = wrEmmAnrGetUtranFreqNode(newNr->t.utraFdd.arfcnDl,
            &cellCb->nrCb->utranCb);
      if(freqNode == NULLP)
      {
         /* Fix For ccpu00123729 */
         RLOG1(L_ERROR,":wrEmmAnrAddUtraNr Failed to get \
               %d freq Node.", newNr->t.utraFdd.arfcnDl);
         ret = RFAILED;
      }
   }
   else
   {
      freqNode = wrEmmAnrGetUtranFreqNode(newNr->t.utraTdd.arfcn,
            &cellCb->nrCb->utranTddCb);
      if(freqNode == NULLP)
      {
         /* Fix For ccpu00123729 */
         RLOG1(L_ERROR,":wrEmmAnrAddUtraNr Failed to get \
               %d freq Node.", newNr->t.utraTdd.arfcn);
         ret = RFAILED;
      }
   }

   if(freqNode != NULLP)
   {
      if(freqNode->nrWlLst.count < WR_MAX_CELL_PER_FREQ)
      {
         newNr->curRank    = cellCb->nrCb->maxRnkWght;
         newNr->nrWlBlLnk.node = (PTR)newNr;
         newNr->blackListed = FALSE;

         /* back reference to frequency Node */
         newNr->freqNode = freqNode;

         /* Insert the node in the NCL according to the rank */
         wrEmmAnrInsertUtranNodeIntoWlList(cellCb, newNr);

         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=
            WR_ANR_UTRA_FREQ_BIT;
#ifdef WR_RSYS_OAM
        if((wrEmmSendUtraNghInd(newNr,WR_ACT_ADD)) != ROK)
        {
           RLOG0(L_ERROR,"Failed to send UTRA neighbour cell to OAM");
        }
#endif
      }
      else
      {
         RLOG0(L_WARNING, "wrEmmAnrAddUtraNr NRT Count \
               reached MAX.");
         ret = RFAILED;
      }
   }/* if(freqNode != NULLP) */

   RETVALUE(ret);
}/* end of wrEmmAnrAddUtraNr */


/**
 *  @brief This function updates and adds the GERAN cell into the
 *         neighbor list
 *
 *      Function: wrEmmAnrAddGeranNr
 *
 *          Processing steps:
 *          - get the frequency node
 *          - add cell into the NRT list
 *          - if number of entries reaches max, notify OAM/EMS
 *
 *  @param  [in] cellCb   : pointer to cell control block
 *  @param  [in] geranNr  : pointer to neighbor algo info configured
 *  @param  [in] newNr    : pointer to neighbor cell info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrEmmAnrAddGeranNr
(
   WrCellCb          *cellCb,
   WrGeranCellCb     *newNr,
   U16               bcchArfcn
)
{
   S16                  ret = ROK;
   WrGeranFreqCb        *freqNode = NULLP;
   WrGeranCellCb        *neighCellCbtmp = NULLP;
   CmLList              *listNode = NULLP;

   TRC2(wrEmmAnrAddGeranNr);

   freqNode = wrEmmAnrGetGeranFreqNode(bcchArfcn, cellCb);
   if(freqNode != NULLP)
   {
      /* back reference to frequency Node */
      newNr->freqCb = freqNode;

      /* insertion sort */
      neighCellCbtmp = (WrGeranCellCb *)CM_LLIST_FIRST_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,listNode);
      while(neighCellCbtmp != NULLP)
      {
         if(neighCellCbtmp->freqCb->cellReselPriority < freqNode->cellReselPriority)
         {
            break;
         }
         neighCellCbtmp = (WrGeranCellCb *)CM_LLIST_NEXT_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,listNode);
      }
      /* add to previous of neighCellCbtmp */
      newNr->cellLstEnt.node = (PTR)newNr;
      if(cmLListCrnt(&cellCb->nrCb->geranCb.geranNrCellLst) == NULLP)
      {
         cmLListAdd2Tail(&cellCb->nrCb->geranCb.geranNrCellLst, (CmLList *)newNr);
      }
      else
      {
         cmLListInsCrnt (&cellCb->nrCb->geranCb.geranNrCellLst, &newNr->cellLstEnt);
      }
      /*cmLListAdd2Tail(&cellCb->nrCb->geranCb.geranNrCellLst, (CmLList *)newNr);*/
#ifdef WR_RSYS_OAM
      if( wrEmmSendAddGeranNghInd(newNr, WR_ACT_ADD) != ROK)
      {
         RLOG0(L_ERROR,"Failed to send an indication to OAM");
         ret = RFAILED;
      }
#endif
   }/* if(freqNode != NULLP) */
   else
   {
      RLOG1(L_ERROR,":wrEmmAnrAddGeranNr Failed to get \
            %d freq Node.", bcchArfcn);
      ret = RFAILED;
   }

   RETVALUE(ret);
}/* end of wrEmmAnrAddGeranNr */


/** @brief This function Initializes the interface datastrutures updated for
 *         measrement module
 *
 *     Function: wrEmmAnrInitMeasNrUpdtInfo
 *
 *         Processing steps:
 *          - switch to corresponding NR type
 *          - call fucntion to update ANR algo params and add NR to list
 *          
 * @param [in] cellId    : cell for which NR needs to be added
 * @param [in] nrAddInfo : NR details containing the cfg info and pointer to 
 *                         cellNode
 * @return Void
 */
PUBLIC Void wrEmmAnrInitMeasNrUpdtInfo
(
   WrCellCb                      *cellCb
)
{

   TRC2(wrEmmAnrInitMeasNrUpdtInfo);

   cellCb->nrCb->nrUpdtInfo->eutran.numFreq = 0;
   
   cellCb->nrCb->nrUpdtInfo->utran.numFreq = 0;

   cellCb->nrCb->nrUpdtInfo->utranTdd.numFreq = 0;
   
} /* end of wrEmmAnrInitMeasNrUpdtInfo */

/**
 *  @brief This function fetches the NR Entry from the eutranCb for the given
 *         frequency and PCI
 *
 *     Function: wrEmmAnrGetEutranNr
 *
 *         Processing steps:
 *         - get the frequency Node for the given frequency 
 *         - search for the NR in the NRT present in the frequency Node
 *         - return the pointer to NR Entry
 *         
 *  @param [in] eutranCb : pointer to Eutran Cb
 *  @param [in] nrInfo   : pointer containing the eutran NR info
 *
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC  WrEutranNeighCellCb* wrEmmAnrGetEutranNr
(
   WrNrEutranCb               *eutranCb, 
   WrEutranNbrCellInfo        *nrInfo
)
{
   CmLList                    *nrLnk = NULLP;
   WrNrEutranFreqCb           *freqNode = NULLP;
   WrEutranNeighCellCb        *eutranNr = NULLP;
   
   /* Get Freq Node */
   freqNode = wrEmmAnrGetEutranFreqNode(
         nrInfo->earfcn, eutranCb);

   if(freqNode != NULLP)
   {
      nrLnk = freqNode->nrWlLst.first;
      while(nrLnk != NULLP)
      {
         eutranNr = (WrEutranNeighCellCb *)nrLnk->node;
         RLOG2(L_INFO,"WR:Stored CellID is %d, Received Cell ID is %d\n",
               eutranNr->enbId,  nrInfo->phyCellId);
#if 0 /* PCI Collision not supported currently */
         if(eutranNr->enbId == nrInfo->cid)
         {
           if(TRUE == wrUtlPlmnsEqual(&eutranNr->enbPlmn, &nrInfo->plmnId))
            {
              RETVALUE(eutranNr);
            }
         }
#endif

         if(eutranNr->pci == nrInfo->phyCellId)
         {
            RETVALUE(eutranNr);
         }
         nrLnk = nrLnk->next;        
      }

      nrLnk = freqNode->nrBlLst.first;
      while(nrLnk != NULLP)
      {
         eutranNr = (WrEutranNeighCellCb *)nrLnk->node;
         RLOG2(L_INFO,"WR:Stored CellID is %d, Received Cell ID is %d\n",
               eutranNr->enbId,  nrInfo->phyCellId);
#if 0 /* PCI Collision not supported currently */
         if(eutranNr->enbId == nrInfo->cid)
         {
           if(TRUE == wrUtlPlmnsEqual(&eutranNr->enbPlmn, &nrInfo->plmnId))
            {
              RETVALUE(eutranNr);
            }
         }
#endif
         if(eutranNr->pci == nrInfo->phyCellId)
         {
            RETVALUE(eutranNr);
         }
         nrLnk = nrLnk->next;        
      }
   }
   RETVALUE(NULLP);
}/* end of wrEmmAnrGetEutranNr */

/**
 *  @brief This function deletes the Eutran NR from the NRT list and 
 *         NCL/HO Black List
 *
 *      Function: wrEmmAnrDelEutranNr
 *
 *          Processing steps:
 *          - initializes the structure for sending the nr update info to
 *            measurement module.
 *          - remove the node from WL and BL
 *
 *  @param [in] cellCb   : pointer to cell Cb
 *  @param [in] nrNode   : Pointer to the cell Cb's to be
 *                         deleted
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 *  
 */
PUBLIC Void wrEmmAnrDelEutranNr
(
   WrCellCb                      *cellCb, 
   WrEutranNeighCellCb           *nrNode
)
{
   TRC2(wrEmmAnrDelEutranNr);

   /* Delete the Node from NCL/HOBL list */
   if(NULLP != wrEmmAnrRemEutranNodeFromNrList( cellCb, nrNode))
   {

      if(nrNode->earfcnDl == cellCb->dlCarrierFreq)
      {
         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=  
            WR_ANR_INTRA_FREQ_BIT;
      }
      else
      {
         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=
            WR_ANR_INTER_FREQ_BIT;
      }
#ifdef WR_RSYS_OAM 
   if(wrEmmSendAddEutraNghInd(nrNode,WR_ACT_DEL) != ROK) 
   { 
      RLOG0(L_ERROR,"Failed to send notification to OAM"); 
   } 
#endif 

   }
   else
   {
      cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=
         WR_ANR_INTER_FREQ_BIT;
   }
} /* end of wrEmmAnrDelEutranNr */

/**
 *  @brief This function removes the node from the NR list
 *
 *  @details The funciton removes the node from the list specified for the
 *           given nrType. and decrements the corresponding counters.
 *
 *      Function: wrEmmAnrRemUtranNodeFromNrList
 *
 *          Processing steps:
 *          - get the list in which the node is present
 *          - remove the node from the list
 *
 *  @param  [in] cellCb        : pointer to cell control Block
 *  @param  [in] neighCellInfo : list entry to be deleted
 *  @return CmLList*
 */

PRIVATE CmLList* wrEmmAnrRemUtranNodeFromNrList
(
   WrCellCb                      *cellCb,
   WrUtraNeighCellCb             *neighCellInfo
)
{
   WrNrUtranFreqCb               *freqNode = NULLP;
   CmLListCp                     *nrLst = NULLP;
   CmLList                       *entryLnk = NULLP;
   WrNrUtraFreqInfo              *nrUtraInfo = NULLP;

   TRC2(wrEmmAnrRemUtranNodeFromNrList);

   if(WR_UTRA_MODE_FDD == neighCellInfo->duplexMode)
   {
      nrUtraInfo  = &cellCb->nrCb->nrUpdtInfo->utran;
   }
   else
   {
      nrUtraInfo  = &cellCb->nrCb->nrUpdtInfo->utranTdd;
   }

   freqNode = neighCellInfo->freqNode;

   if(neighCellInfo->blackListed != TRUE)
   {
      nrLst = &freqNode->nrWlLst;
      wrEmmAnrUpdateUtraFreqWlInfo(neighCellInfo, nrUtraInfo, WR_EMM_ANR_DEL_NR);
   }
   else
   {
      nrLst = &freqNode->nrBlLst;
   }

   entryLnk = &neighCellInfo->nrWlBlLnk;

   entryLnk = cmLListDelFrm(nrLst, entryLnk);

   RETVALUE(entryLnk);
} /* end of wrEmmAnrRemUtranNodeFromNrList */

/**
 *  @brief This function fetches the NR Entry from the utranCb for the given
 *         frequency and PSC
 *
 *     Function: wrEmmAnrGetUtranNr
 *
 *         Processing steps:
 *         - get the frequency Node for the given frequency
 *         - search for the NR in the NRT present in the frequency Node
 *         - return the pointer to NR Entry
 *
 *  @param [in] utranCb  : pointer to utran Cb
 *  @param [in] nrInfo   : pointer containing the utran NR info
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC WrUtraNeighCellCb* wrEmmAnrGetUtranNr
(
   WrNrUtranCb                *utranCb,
   WrUtranNrInfo              *nrInfo
)
{
   CmLList                    *nrLnk;
   WrNrUtranFreqCb            *freqNode;
   WrUtraNeighCellCb          *utranNr = NULLP;

   /* Get Freq Node */
   freqNode = wrEmmAnrGetUtranFreqNode(
         nrInfo->arfcn, utranCb);

   if(freqNode != NULLP)
   {
      nrLnk = freqNode->nrWlLst.first;
      while(nrLnk != NULLP)
      {
         utranNr = (WrUtraNeighCellCb *)nrLnk->node;
#if 0 /* PCI Collision not supported currently */
        if(utranNr->cId == nrInfo->cid)
         {
           if(TRUE == wrUtlPlmnsEqual(&utranNr->plmnId[0], &nrInfo->plmnId))
           {
             RETVALUE(utranNr);
           }
         }
#endif
         if(utranNr->duplexMode == WR_UTRA_MODE_FDD)
         {
            if(utranNr->t.utraFdd.psc == nrInfo->psc)
            {
               RETVALUE(utranNr);
            }
         }
         else
         {
            if(utranNr->t.utraTdd.psc == nrInfo->psc)
            {
               RETVALUE(utranNr);
            }
         }
         nrLnk = nrLnk->next;
      }
   }
   RETVALUE(NULLP);
}/* end of wrEmmAnrGetUtranNr */

/**
 *  @brief This function deletes the Utran NR's from the NRT list 
 *
 *      Function: wrEmmAnrDelUtraNr
 *
 *          Processing steps:
 *          - initializes the structure for sending the nr update info to
 *            measurement module.
 *          - remove the node from NRT and 
 *          - if default timer is not expired
 *             - stop the timer
 *
 *  @param [in] cellCb   : pointer to cell Cb
 *  @param [in] nrNode   : Pointer to WrUtraNeighCellCb to be deleted
 *  @return Void
 *
 */
PUBLIC Void wrEmmAnrDelUtraNr
(
   WrCellCb                      *cellCb,
   WrUtraNeighCellCb             *nrNode
)
{
   TRC2(wrEmmAnrDelUtraNr);

   /* Delete the Node from NCL list */
   wrEmmAnrRemUtranNodeFromNrList( cellCb, nrNode);

   cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=
      WR_ANR_UTRA_FREQ_BIT;
   
} /* end of wrEmmAnrDelUtraNr */

/**
 *  @brief This function update the Modified Info of UTRAN algo parameters from
 *         the config structure and performs corresponding changes
 *
 *      Function: wrEmmAnrUpdtUtranModInfo
 *
 *          Processing steps:
 *
 *  @param [in] cellCb   : pointer to cell control block
 *  @param [in] nrModInfo: pointer to Modified Info of the cell
 *  @param [in] modNr    : pointer to UTRAN Neighbor cell control block
 *  
 *  @return None
 */

PUBLIC Void wrEmmAnrUpdtUtranModInfo
(
   WrCellCb                      *cellCb,
   WrUtraNeighCellCb             *modNr
)
{
   TRC2(wrEmmAnrUpdtUtranModInfo);

   RETVOID;
} /* end of wrEmmAnrUpdtUtranModInfo */ 


/**
 *  @brief This function remove the neighbour cell from frequency list
 *
 *   Function: wrEmmRmvNghCellFrmFreqNode
 *
 *   Processing steps:
 *
 *  @param [in] cellCb    : pointer to cell control block
 *  @param [in] modNr     : pointer to EUTRAN Neighbor cell control block
 *  
 *  @return ROK
 */
PRIVATE S16 wrEmmRmvNghCellFrmFreqNode
(
 WrCellCb                      *cellCb,
 WrEutranNeighCellCb           *modNr
)
{
   TRC2(wrEmmRmvNghCellFrmFreqNode);
   CmLListCp                     *lst;
   
   /* get the lst in which cell is present WL/BL */
   if(modNr->blackListed != TRUE)
   {
      wrEmmAnrUpdateInterFreqWlInfo(modNr, 
                 &(cellCb->nrCb->nrUpdtInfo->eutran),WR_EMM_ANR_DEL_NR);
      lst = &modNr->freqNode->nrWlLst;
   }
   else
   {
      wrEmmAnrUpdateInterFreqBlInfo(modNr,
                 &(cellCb->nrCb->nrUpdtInfo->eutran),WR_EMM_ANR_DEL_NR);
      lst = &modNr->freqNode->nrBlLst;
   }
   
   /* delete the node from the current Frequency Node List */
   cmLListDelFrm(lst, &modNr->nrWlBlLnk);
#ifdef WR_RSYS_OAM 
   if((wrEmmSendAddEutraNghInd(modNr,WR_ACT_DEL))!= ROK) 
   { 
      RLOG0(L_ERROR,"Failed to send notification to OAM"); 
      RETVALUE(RFAILED); 
   } 
#endif 

   RETVALUE(ROK);
}


/**
 *  @brief This function update the modified neigbour cell key parameters
 *
 *   Function: wrEmmAnrUpdtEutranKeyModInfo
 *
 *   Processing steps:
 *
 *  @param [in] cellCb     : pointer to cell control block
 *  @param [in] modCellInfo: pointer to Modified Info of the cell
 *  @param [in] modNr      : pointer to EUTRAN Neighbor cell control block
 *  
 *  @return ROK
 */
PUBLIC S16 wrEmmAnrUpdtEutranKeyModInfo
(
 WrCellCb                      *cellCb,
 WrEutraNghCellModInfo         *modCellInfo,
 WrEutranNeighCellCb           *modNr
)
{
   
   TRC2(wrEmmAnrUpdtEutranKeyModInfo);


   if (modNr->freqNode == NULLP)
	{
       RLOG0(L_ERROR,"wrEmmAnrUpdtEutranKeyModInfo: Failed to get FREQ NODE ");
       RETVALUE(RFAILED);
	}
   
   if((modCellInfo->mask & LWR_EUTRAN_NGH_CELL_PCI)&&
      (modCellInfo->mask & LWR_EUTRAN_NGH_CELL_DL_EARFCN)&&
      (modCellInfo->mask & LWR_EUTRAN_NGH_CELL_BLACK_LIST))
   {
   
      wrEmmRmvNghCellFrmFreqNode(cellCb,modNr);
      
      /*Updating Modified Values*/
      modNr->earfcnDl = modCellInfo->earfcnDl;
      modNr->pci = modCellInfo->pci;
      modNr->blackListed = modCellInfo->blackListed;

      /*Setting mask for updating SIB4 or SIB5. we need to update SIB4 and SIB5
        If neighbour cell earfcn is changed (intra to inter or inter to intra). 
        The modified frequency neighbour mask(SIB4 or SIB5) will be set in wrEmmAnrAddEutranNr function.
        So here we are updating mask for removing the entris from SIB4 or SIB5 based old earfcn values
      */
      if(modNr->earfcnDl == cellCb->dlCarrierFreq)
      {
            cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_INTER_FREQ_BIT;
      }
      else
      {
         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_INTRA_FREQ_BIT;
      }   


      /* Insert  the modified neighbour cell */
      wrEmmAnrAddEutranNr(cellCb, modNr);
   }
   else if((modCellInfo->mask & LWR_EUTRAN_NGH_CELL_BLACK_LIST) &&
      ((modCellInfo->mask & LWR_EUTRAN_NGH_CELL_PCI)))
   {
   
      wrEmmRmvNghCellFrmFreqNode(cellCb,modNr);
      
      /*Updating Modified Values*/
      modNr->pci =modCellInfo->pci;
      modNr->blackListed = modCellInfo->blackListed;

      /* Insert the modified neighbour cell */
      wrEmmAnrAddEutranNr(cellCb, modNr); 
   }
   else if((modCellInfo->mask & LWR_EUTRAN_NGH_CELL_BLACK_LIST)&&
      ((modCellInfo->mask & LWR_EUTRAN_NGH_CELL_DL_EARFCN)))
   {
   
      wrEmmRmvNghCellFrmFreqNode(cellCb,modNr);
      
      /*Updating Modified Values*/
      modNr->earfcnDl=modCellInfo->earfcnDl;
      modNr->blackListed = modCellInfo->blackListed;
      /* Setting mask for updating SIB4 or SIB5. we need to update SIB4 and SIB5
        If neighbour cell earfcn is changed (intra to inter or inter to intra). 
        The modified frequency neighbour mask(SIB4 or SIB5) will be set in wrEmmAnrAddEutranNr function.
        So here we are updating mask for removing the entris from SIB4 or SIB5 based old earfcn values
      */
      
      if(modNr->earfcnDl == cellCb->dlCarrierFreq)
      {
            cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_INTER_FREQ_BIT;
      }
      else
      {
         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_INTRA_FREQ_BIT;
      }   

      /* Insert  the modified neighbour cell */
      wrEmmAnrAddEutranNr(cellCb, modNr); 
   }
   else if((modCellInfo->mask & LWR_EUTRAN_NGH_CELL_PCI)&&
      ((modCellInfo->mask & LWR_EUTRAN_NGH_CELL_DL_EARFCN)))
   {

      wrEmmRmvNghCellFrmFreqNode(cellCb,modNr);
      
      /*Updating Modified Values*/
      modNr->earfcnDl=modCellInfo->earfcnDl;
      modNr->pci = modCellInfo->pci;
      /*Setting mask for updating SIB4 or SIB5. we need to update SIB4 and SIB5
        If neighbour cell earfcn is changed (intra to inter or inter to intra). 
        The modified frequency neighbour mask(SIB4 or SIB5) will be set in wrEmmAnrAddEutranNr function.
        So here we are updating mask for removing the entris from SIB4 or SIB5 based old earfcn values
      */
      if(modNr->earfcnDl == cellCb->dlCarrierFreq)
      {
            cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_INTER_FREQ_BIT;
      }
      else
      {
         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_INTRA_FREQ_BIT;
      }

      /* Insert the modified neighbour cell */
      wrEmmAnrAddEutranNr(cellCb, modNr); 
   }
   else if((modCellInfo->mask & LWR_EUTRAN_NGH_CELL_QOFFSET))
   {
      /*Updating Frequeny bit mask*/
      if(modNr->earfcnDl == 
               cellCb->dlCarrierFreq)
      {
            cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_INTRA_FREQ_BIT;
      }
      else
      {
         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=  WR_ANR_INTER_FREQ_BIT;
      }   
   }
   else if ((modCellInfo->mask & LWR_EUTRAN_NGH_CELL_PCI))
   {
      if(modNr->blackListed != TRUE)
      {
         wrEmmAnrUpdateInterFreqWlInfo(modNr, 
                    &(cellCb->nrCb->nrUpdtInfo->eutran),WR_EMM_ANR_DEL_NR);
      }
      else
      {
         wrEmmAnrUpdateInterFreqBlInfo(modNr,
                    &(cellCb->nrCb->nrUpdtInfo->eutran),WR_EMM_ANR_DEL_NR);
      }
      /*Updating PCI*/
      modNr->pci = modCellInfo->pci;
      
      if(modNr->blackListed != TRUE)
      {
         wrEmmAnrUpdateInterFreqWlInfo(modNr, 
                    &(cellCb->nrCb->nrUpdtInfo->eutran),WR_EMM_ANR_ADD_NR);
      }
      else
      {
         wrEmmAnrUpdateInterFreqBlInfo(modNr,
                    &(cellCb->nrCb->nrUpdtInfo->eutran),WR_EMM_ANR_ADD_NR);
      }
      /*Updating Frequeny bit mask*/
      if(modNr->earfcnDl == 
               cellCb->dlCarrierFreq)
      {
            cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_INTRA_FREQ_BIT;
      }
      else
      {
         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=  WR_ANR_INTER_FREQ_BIT;
      }
   }
   else if((modCellInfo->mask & LWR_EUTRAN_NGH_CELL_DL_EARFCN))
   {
   
      wrEmmRmvNghCellFrmFreqNode(cellCb,modNr);
      
      /*Updating Modified Values*/
      modNr->earfcnDl = modCellInfo->earfcnDl;
       /* Setting mask for updating SIB4 or SIB5. we need to update SIB4 and SIB5
        If neighbour cell earfcn is changed (intra to inter or inter to intra). 
        The modified frequency neighbour mask(SIB4 or SIB5) will be set in wrEmmAnrAddEutranNr function.
        So here we are updating mask for removing the entris from SIB4 or SIB5 based old earfcn values
      */
      if(modNr->earfcnDl == cellCb->dlCarrierFreq)
      {
            cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_INTER_FREQ_BIT;
      }
      else
      {
         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |= WR_ANR_INTRA_FREQ_BIT;
      }   

      /* Insert the modified neighbour cell */
      wrEmmAnrAddEutranNr(cellCb, modNr);   
   }
   else if((modCellInfo->mask & LWR_EUTRAN_NGH_CELL_BLACK_LIST))
   {
   
      wrEmmRmvNghCellFrmFreqNode(cellCb,modNr);
      
      /*Updating Modified Values*/
      modNr->blackListed= modCellInfo->blackListed;

      /* Insert the modified neighbour cell */
      wrEmmAnrAddEutranNr(cellCb, modNr); 
   }
   
   RETVALUE(ROK);
}


/**
 *  @brief This function update the Modified Info of EUTRAN algo parameters from
 *         the config structure and performs corresponding changes
 *
 *      Function: wrEmmAnrUpdtEutranModInfo
 *
 *          Processing steps:
 *
 *  @param [in] cellCb   : pointer to cell control block
 *  @param [in] nrModInfo: pointer to Modified Info of the cell
 *  @param [in] modNr    : pointer to EUTRAN Neighbor cell control block
 *  
 *  @return None
 */

PUBLIC Void wrEmmAnrUpdtEutranModInfo
(
   WrCellCb                      *cellCb,
   WrEutranNeighCellCb           *modNr
)
{
   CmLListCp                     *lst;
   TRC2(wrEmmAnrUpdtEutranModInfo);

   /* Verify whether the frequency node exist in the neighbour */
   if (modNr->freqNode == NULLP)
   {
       RLOG0(L_ERROR,"wrEmmAnrUpdtEutranModInfo: Failed to get FREQ NODE ");
       RETVOID;
   }
   
   /* Validate the Modified Frequescy against the Frequency of the 
    * Frequency Node */
   if(modNr->earfcnDl != modNr->freqNode->earfcn)
   {
      /* If Frequency Modified */

      /* get the lst in whichcell is present WL/BL */
      if(modNr->blackListed != TRUE)
      {
         lst = &modNr->freqNode->nrWlLst;
      }
      else
      {
         lst = &modNr->freqNode->nrBlLst;
      }
      
      /* delete the node from the current Frequency Node List */
      cmLListDelFrm(lst, &modNr->nrWlBlLnk);
      
      /* Insert in teh new frequency Node */
      wrEmmAnrAddEutranNr(cellCb, modNr);
   }

   RETVOID;
} /* end of wrEmmAnrUpdtEutranModInfo */ 

/**
 *  @brief This function Updates the EUTRAN Frequency list first Node with the 
 *         serving Frequecy, if the swerving frequency is changed
 *
 *      Function: wrEmmAnrProcessIntraFreqChg
 *
 *          Processing steps:
 *          - if the serving freq is not matching with frequency of the first 
 *            node in the freq list
 *             - get the frequency node matching with the serving frequency
 *             - delete it from list
 *             - add it as the firsty node of the cell
 *          - else do nothing
 *          - return
 *          
 *  @param [in] cellId          : cell ID of the cell getting configured                     
 *  @return S16
 *   -# Success : ROK
 */
PUBLIC S16 wrEmmAnrProcessIntraFreqChg
(
   U8                            cellId
)
{
   S16                           ret = ROK;
   WrCellCb                      *cellCb  = NULLP;
   WrNrEutranFreqCb              tempFreqCb;
   WrNrEutranFreqCb              *freqCb = NULLP;
   CmLList                       *freqLnk = NULLP;
   CmLListCp                     *freqList = NULLP;

   TRC2(wrEmmAnrProcessIntraFreqChg);

   WR_GET_CELLCB(cellCb, cellId);
   if (NULLP == cellCb)
   {   
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrProcessIntraFreqChg: Failed to get cell CB");
       RETVALUE(RFAILED);
   }    
   tempFreqCb = cellCb->nrCb->eutranCb.intraFreqCb;
   freqList = &cellCb->nrCb->eutranCb.interFreqList;

   CM_LLIST_FIRST_NODE(freqList, freqLnk);

   freqCb = (WrNrEutranFreqCb *)freqLnk->node;

   while(freqLnk != NULLP)
   {
      freqCb = (WrNrEutranFreqCb *)freqLnk->node;

      if(cellCb->dlCarrierFreq == freqCb->earfcn)
      {
         cmLListDelFrm(freqList, freqLnk);

         cellCb->nrCb->eutranCb.intraFreqCb = *freqCb;

         *freqCb = tempFreqCb;

         cmLListInsCrnt(freqList, &freqCb->freqLstEnt);

         break;
      }

      CM_LLIST_NEXT_NODE(freqList, freqLnk);
   }
   RETVALUE(ret);
} /* end of wrEmmAnrCellReCfgReq */ 


/**
 *  @brief This function Re-Configures the ANR algorithm and BL NR details 
 *
 *      Function: wrEmmAnrCellReCfgReq
 *
 *          Processing steps:
 *          - reconfigure the global ANR config structure
 *          - Process the modified BL NR info received
 *
 *  @param [in] cellId          : cell ID of the cell getting configured                     
 *  @param [in] anrReConfigInfo : This contains the ANR Re-configuration info.
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrEmmAnrCellReCfgReq
(
   U8                            cellId
)
{
   S16                           ret = ROK;
/*   WrCellCb *cellCb  = NULLP;*/

   TRC2(wrEmmAnrCellReCfgReq);

   RETVALUE(ret);
} /* end of wrEmmAnrCellReCfgReq */ 


/**
 *  @brief This function process the NR details received as part of NR
 *         configuration
 *
 *  @details The function is called when the Initial NR configuration is done.
 *           This fucntion creates the HASH tables for all the NR types and
 *           updates them with the NR info received. Rank the NR's and makes the
 *           NCL (white list) and Black list.
 *
 *      Function: wrEmmAnrProcessCfgReq 
 *
 *          Processing steps:
 *          - for each Black List type 
 *             - call funciton to update the black list info
 *          - start the ANR timer for Intra LTE and inter RAT Nr's
 *
 *
 *  @param [in] cellCb         : pointer to cell control block
 *  @param [in] anrCfgGrp  : Holds the ANR configuraiton details 
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrEmmAnrProcessCfgReq 
(
   WrCellCb                      *cellCb,
   LwrCellAnrCfgGrp              *anrCfgGrp
)
{
   S16                           ret = ROK;

   TRC2(wrEmmAnrProcessCfgReq);

   cellCb->nrCb->epochCnt = 1;
   if(anrCfgGrp->maxRankWght.pres == TRUE)
   {
      cellCb->nrCb->maxRnkWght = anrCfgGrp->maxRankWght.val;
   }
   else
   {
      cellCb->nrCb->maxRnkWght = WR_ANR_DEFAULT_MAX_RANK_WEIGHT;
   }
   if(anrCfgGrp->minRankWght.pres == TRUE)
   {
      cellCb->nrCb->minRnkWght = anrCfgGrp->minRankWght.val;
   }
   else
   {
      cellCb->nrCb->minRnkWght = WR_ANR_DEFAULT_MIN_RANK_WEIGHT;
   }
   if(anrCfgGrp->epocTimerVal.pres == TRUE)
   {
      if((anrCfgGrp->epocTimerVal.val >= WR_EMM_OAM_EPOCH_VAL_MIN_INSEC) && (anrCfgGrp->epocTimerVal.val <= WR_EMM_OAM_EPOCH_VAL_MAX_INSEC))
      {
         cellCb->nrCb->epochTmrVal = WR_EMM_SECOND_VAL_IN_MSECS * anrCfgGrp->epocTimerVal.val;
      }
      else
      {
         cellCb->nrCb->epochTmrVal = WR_EMM_SECOND_VAL_IN_MSECS * WR_EMM_DEFAULT_EPOCH_TIMER_VAL_INSECS;
      }
   }
   else
   {
      cellCb->nrCb->epochTmrVal = WR_EMM_SECOND_VAL_IN_MSECS * WR_EMM_DEFAULT_EPOCH_TIMER_VAL_INSECS;
   }
   if((anrCfgGrp->tricePeriodIntv >= WR_EMM_OAM_TRICE_VAL_MIN_INSEC) && (anrCfgGrp->tricePeriodIntv <=WR_EMM_OAM_TRICE_VAL_MAX_INSEC))
   {
      cellCb->nrCb->tricePeriodIntvCnt = anrCfgGrp->tricePeriodIntv;
   }
   else
   {
      /* if the trice value configures as '0' taking defalut values as '10'*/
      cellCb->nrCb->tricePeriodIntvCnt = WR_EMM_OAM_TRICE_VAL_MAX_INSEC;
   }

   /* Valid received from SM is in Minutes. So converting to milli seconds */
   cellCb->nrCb->cellValidAgeTmrVal = anrCfgGrp->anrCellValidAge;
   /*Removed DRX feature Flag*/
   /* Fix for ccpu00132334 - Validate the ANR DRX specific parameters*/
   if(anrCfgGrp->ueAnrDrxCfg.drxOnDurTmr > WR_ANR_MAX_DRX_ON_DURATION_TMR_VAL)
   {
      /* Fix for ccpu00132334 - assign default ANR on duraton timer value*/
      cellCb->ueAnrDrxCfg.drxOnDurTmr  = WR_ANR_DEFAULT_DRX_ON_DURATION_TMR_VAL;
   }
   else
   {
      cellCb->ueAnrDrxCfg.drxOnDurTmr  = anrCfgGrp->ueAnrDrxCfg.drxOnDurTmr;
   }
   if (anrCfgGrp->ueAnrDrxCfg.longDrxCycle > WR_ANR_MAX_LONG_DRX_TMR_VAL)
   {
      /* Fix for ccpu00132334 - assign default ANR long DRX timer value*/
      cellCb->ueAnrDrxCfg.longDrxCycle = WR_ANR_DEFAULT_LONG_DRX_TMR_VAL; 
   }
   else
   {
      cellCb->ueAnrDrxCfg.longDrxCycle = anrCfgGrp->ueAnrDrxCfg.longDrxCycle;
   }
   if(
         (anrCfgGrp->ueAnrDrxCfg.inActivTmr > WR_ANR_MAX_DRX_INACTIVITY_TMR_VAL))
   {
      /* Fix for ccpu00132334 - assign default ANR DRX inactivity timer value*/
      cellCb->ueAnrDrxCfg.inActivTmr =WR_ANR_DEFAULT_DRX_INACTIVITY_TMR_VAL;
   }
   else
   {
      /* Fix for ccpu00132334 - To read DRX Inactivity timer for ANR*/
      cellCb->ueAnrDrxCfg.inActivTmr = anrCfgGrp->ueAnrDrxCfg.inActivTmr; 
   }
   RETVALUE(ret);
} /* end of wrEmmAnrProcessCfgReq */ 

/**
 *  @brief This function Initializes the ANR module and configures the ANR 
 *         algorithm and Initial NR details 
 *
 *  @details This function is called by EMM, when it receives the configuration
 *           during cell configuration. EMM configures the ANR as per OAM. 
 *           This function first initializes the ANR module and then configures 
 *           ANR. The configuration information contains ANR algorithm 
 *           specific config parameters, ANR control Info that enables or
 *           disable the ANR operation for different NR types and the neighbor
 *           details of different NR types. The differnt NR types for which ANR
 *           operation is supported will be enabled. NR's configured will be 
 *           added into the corresponding NRT list and to NCL(whitel list)/
 *           HO Black list as per the algorithm definition. 
 *
 *      Function: wrEmmAnrCellCfgReq
 *
 *          Processing steps:
 *          - initialize the ANR module
 *          - update the global ANR config structure
 *          - Process the NR info received as per the control Info
 *
 *  @param [in] cellId        : cell ID of the cell getting configured                     
 *  @param [in] anrCfgGrp : This contains the ANR configuration info.
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrEmmAnrCellCfgReq
(
   U8                         cellId,
   LwrCellAnrCfgGrp           *anrCfgGrp
)
{
   S16                           ret = ROK;
   WrCellCb                      *cellCb  = NULLP;

   TRC2(wrEmmAnrCellCfgReq);
  
   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {   
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrCellCfgReq: Failed to get cell CB");
       RETVALUE(RFAILED);
   }    
   ret = wrEmmAnrInit(cellCb);

   if(ret == ROK)
   {
      ret = wrEmmAnrProcessCfgReq(cellCb, anrCfgGrp);
   }
   else
   {
      RLOG0(L_ERROR, "wrEmmAnrInit Failed");
   }

   RETVALUE(ret);
} /* end of wrEmmAnrCellCfgReq */ 


/* Internal Functions */


/* API for teh MCH, X2I and PBM Module */


PUBLIC WrEutranNeighCellCb* wrEmmFindEutranNeighCell
(
   U8                         cellId, 
   U16                        earfcn, 
   U16                        pci
)
{
   WrCellCb                     *cellCb = NULLP;
   WrEutranNeighCellCb          *eutranNr = NULLP;
   WrEutranNbrCellInfo        nrInfo;

   WR_GET_CELLCB(cellCb, cellId);
   if (NULLP == cellCb)
   {   
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmFindEutranNeighCell: Failed to get cell CB ");
       RETVALUE(NULLP);
   }    
   nrInfo.phyCellId = pci;
   nrInfo.earfcn    = earfcn;
   eutranNr = wrEmmAnrGetEutranNr(&cellCb->nrCb->eutranCb, &nrInfo);

   RETVALUE(eutranNr);
}

/**
 *  @brief This function validates the presence of an PCI in NRT
 *
 *  @details The PCI for which validation need to be done is searched in hte
 *           both Intra and Inter NRT maintained in ANR
 *
 *      Function: wrEmmAnrValidatePci
 *
 *          Processing steps:
 *          - switch to the Nr Type 
 *          - check for PCI in the configured neighbor list
 *          - if not found
 *             - check in teh black list maintained in ANR
 *          - if PCI found 
 *             - return ROK
 *          - else 
 *             - continue
 *          
 *  @param  [in] cellId   : cellID of the cell, in which PCI validation 
 *                         need to be done        
 *  @param  [in] pci    : PCI to be searched for existance
 *  @param  [in] nrType : Type of List in which the PCI need to be searched
 *  @param  [in] nrInfo : Neighbor Info corresponding to the reported PCI
 *  @param  [out] reportType : Indicate the reason for reportCGI (CSG or CGI)
 *  @param  [in] isEvntAnr : Indicates the PCI validation is for Periodic or
 *                          Event based ANR
 *  @param  [in] ueIdx : UE Index in Cell CB UE list
 *  @return Bool
    -# Success : TRUE
    -# Failure : FALSE
 */
PUBLIC Bool wrEmmAnrValidatePci 
(
U8                           cellId, 
U16                          pci, 
WrEmmNrType                  nrType,
WrNrInfo                     *nrInfo,
U8                           *reportType,
U8                           isEvntAnr,
U8                           ueIdx
)
{
   /* CSG_DEV */
   Bool                      retVal = TRUE;
   WrCellCb                  *cellCb  = NULLP;
   WrEutranNbrCellInfo       eutraNrInfo;
   WrUtranNrInfo             utraNrInfo;
   WrEutranNeighCellCb       *eutNbrCellCb;

   TRC2(wrEmmAnrValidatePci)
  
   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrValidatePci: Failed to get cell CB");
      RETVALUE(NULLP);
   }

   switch(nrType)
   {
      case WR_EMM_ANR_EUTRAN:
      {
         eutraNrInfo.phyCellId = pci;
         eutraNrInfo.earfcn    = nrInfo->earfcn;
         /* CSG_DEV */
         eutNbrCellCb = wrEmmAnrGetEutranNr(&cellCb->nrCb->eutranCb,
                  &eutraNrInfo);
         if(NULLP != eutNbrCellCb)
         {
            retVal = wrEmmCsgIsReportCgiNeeded(ueIdx, isEvntAnr,
                  cellCb, eutNbrCellCb);
            (*reportType) = WR_EMM_ANR_REPORT_CSG;
         }
      }
      break;
      case WR_EMM_ANR_UTRA_FDD:
      {
         utraNrInfo.psc = pci;
          utraNrInfo.arfcn = nrInfo->earfcn;
         /* CSG_DEV */
         if(NULLP != (PTR)wrEmmAnrGetUtranNr(&cellCb->nrCb->utranCb,
                  &utraNrInfo))
         {
            retVal = FALSE;
         }
      }
      break;
		case WR_EMM_ANR_UTRA_TDD:
      {
	      utraNrInfo.psc = pci;
       	utraNrInfo.arfcn = nrInfo->earfcn;
         /* CSG_DEV */
         if(NULLP != (PTR)wrEmmAnrGetUtranNr(&cellCb->nrCb->utranTddCb,
                  &utraNrInfo))
         {
            retVal = FALSE;
         }
      }
      break;
      default:
      break;
   }
   
   RETVALUE(retVal);
} /* end of wrEmmAnrValidatePci */ 

 /**
  *  @brief This function validates the PCI list and updates teh existance state
  *
  *      Function: wrEmmAnrValidatePciList
  *
  *          Processing steps:
  *          - traverse the PCI list
  *          - for each PCI
  *            - validate the PCI
  *            - update the the boolean
  *            - if found 
  *               - update the counter
  *          - return the number of successful PCI
  *
  *  @param [in] cellId : cellID of the cell in PCI list to be validated
  *  @param [in] nrType : Nr Type for which PCI need to be validated
  *  @param [in] pciList: pointer where the validation result is updated
  *  @param [in] isEvntAnr : Indicates the PCI validation is for Periodic or
  *                          Event based ANR
  *  @param [in] ueIdx : UE Index in Cell CB UE list
  *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
  */

PUBLIC S16 wrEmmAnrValidatePciList
(
U8                           cellId, 
WrEmmNrType                  nrType, 
WrNewPciLst                  *pciList,
U8                           isEvntAnr,
U8                           ueIdx
)
{
   S16                       ret = ROK;
   U8                        newPciCnt = 0;
   U8                        pciNodeIndex = 0;
   WrNewPci                  *newPci = NULLP;
   /* CSG_DEV start */
   WrNewPci                  reportCgiLst[WR_EMM_MAX_PCI_PER_REPORT];
   WrNewPci                  reportCsgLst[WR_EMM_MAX_PCI_PER_REPORT];
   U8                        cgiPciIndx = 0;
   U8                        csgPciIndx = 0;
   U8                        reportType;
   /* CSG_DEV end */

   TRC2(wrEmmAnrValidatePciList);

   newPci = pciList->newPci;

   while(pciNodeIndex < pciList->numComp)
   {
      reportType = WR_EMM_ANR_REPORT_CGI;
      newPci->pciExist = wrEmmAnrValidatePci(cellId, newPci->pci, nrType,
            &pciList->nrInfo, &reportType, isEvntAnr, ueIdx);
      if(newPci->pciExist)
      {
         newPciCnt++;
      }
      /* CSG_DEV start */
      if(WR_EMM_ANR_REPORT_CSG == reportType)
      {
         reportCsgLst[csgPciIndx].pciExist = newPci->pciExist;
         reportCsgLst[csgPciIndx].pci      = newPci->pci;
         csgPciIndx++;
      }
      else
      {
         reportCgiLst[cgiPciIndx].pciExist = newPci->pciExist;
         reportCgiLst[cgiPciIndx].pci      = newPci->pci;
         cgiPciIndx++;
      }
      /* CSG_DEV end */
      pciNodeIndex++;
      newPci++;
   }

   /* CSG_DEV start */
   /* reinitializing newPci - since newPci is moved in the above for loop */
   newPci = pciList->newPci;

   /* copy PCI's for which CGI needs to be discovered */
   if(cgiPciIndx)
   {
      cmMemcpy((U8*)newPci, (U8*)reportCgiLst, (cgiPciIndx * sizeof(WrNewPci)));
   }
   /* copy PCI's for which CSG ID needs to be discovered */
   if(csgPciIndx)
   {
      /* increment indexes */
      newPci = newPci + cgiPciIndx;
      cmMemcpy((U8*)newPci, (U8*)reportCsgLst, (csgPciIndx * sizeof(WrNewPci)));
   }
   /* CSG_DEV end */

   pciList->newPciCnt = newPciCnt;
  
   RETVALUE(ret);
} /* end of wrEmmAnrValidatePciList*/

/**
 *  @brief This function forms the white list and Black list of Inter-Freq 
 *         Neibhor cells for PBM and MCH
 *
 *  @details This Function forms the list of neighbor cell PCI's for whom HO
 *           need to be done(white List) and not to be done (Black List). The
 *           White List and Black List are maintained per frquency.
 *
 *      Function: wrEmmAnrFormInterFreqLst
 *
 *          Processing steps:
 *          - Traverse through the freq node list
 *          - for each node
 *             - traverse through the white list
 *             - for each node
 *                - encode the cell into white list
 *             - traverse through the black list
 *             - for each node
 *                - encode the cell into black list
 *          - traverse Black listed PCI list
 *                - for each cell present in the above lists
 *                - add the cell into the PCI Black list of that frequency
 *                - encode the cell into black list
 *
 *  @param [in] nrWlBlInfo : pointer in which inter freq WL and BL is formed
 *  @param [in] cellCb     : pointer to cell CB
 *  @param [in] brInfo     : enum indicating whether the info is for Broadcast
 *                           module or not
 *  @return None
 */

PRIVATE S16 wrEmmAnrFormInterFreqLst
(
   WrNrWlBlInfo                  *nrWlBlInfo,
   WrCellCb                      *cellCb,
   WrBrdCstInfo                  brInfo
)
{
   WrNrInterFreqInfo             *nrInterWlBlInfo = NULLP;
                                 
   CmLList                       *freqNodeLnk = NULLP;
   CmLListCp                     *freqLst = NULLP;
   WrNrEutranFreqCb              *freqNode = NULLP;
   WrEutranNeighCellCb           *nbrCellNode = NULLP;
                                 
   CmLListCp                     *nbrCellLst = NULLP;
   CmLList                       *nbrCellLnk = NULLP;

   TRC2(wrEmmAnrFormInterFreqLst)
   
   nrInterWlBlInfo = &nrWlBlInfo->inter;
   nrInterWlBlInfo->numFreq = 0;
   
   freqLst    = &cellCb->nrCb->eutranCb.interFreqList;

   /* The first node is for the serving frequency. 
    * Hence strat from the second node for the inter frequency list */
   /*127181: adding NULLP check to avoid crash*/
   if(NULLP == freqLst->first)
   {   
      RLOG0(L_ERROR,"Serving frequency not present");
      RETVALUE(RFAILED);
   }    
   freqNodeLnk = freqLst->first;
   
   while(freqNodeLnk != NULLP)
   {
      freqNode = (WrNrEutranFreqCb *)freqNodeLnk->node;

      /* Update the White List */
      nbrCellLst = &freqNode->nrWlLst;
      nbrCellLnk = nbrCellLst->first;

      while(nbrCellLnk != NULLP)
      {
         nbrCellNode = (WrEutranNeighCellCb *)nbrCellLnk->node;
         if(nbrCellNode != NULLP)
         {
            wrEmmAnrUpdateInterFreqWlInfo(nbrCellNode, nrInterWlBlInfo, 
                  WR_EMM_ANR_ADD_NR);
         }
         nbrCellLnk = nbrCellLnk->next;
      }

      /* Update the Black List */
      nbrCellLst = &freqNode->nrBlLst;
      nbrCellLnk = nbrCellLst->first;

      while(nbrCellLnk != NULLP)
      {
         nbrCellNode = (WrEutranNeighCellCb *)nbrCellLnk->node;
         if(nbrCellNode != NULLP)
         {
            wrEmmAnrUpdateInterFreqBlInfo(nbrCellNode, nrInterWlBlInfo, 
                  WR_EMM_ANR_ADD_NR);
         }
         nbrCellLnk = nbrCellLnk->next;
      }

      freqNodeLnk = freqNodeLnk->next;
   }

   RETVALUE(ROK);
} /* end of wrEmmAnrFormInterFreqLst */ 

/**
 *  @brief This function encodes the Intra Freq cell info as white list cell
 *         into the cell Info list, which will be sent to MCH/PBM
 *
 *      Function: wrEmmAnrUpdateIntraFreqWlInfo
 *
 *          Processing steps:
 *          - check for the frequency in the current frequency list
 *             - get the cell node index with in the White list
 *             - if cell Index < max num of cells taht can be encoded
 *                - add the physCellId into the list of that frequency
 *                - update the nrUpdtType
 *                - increment number of PCI in the list
 *
 *  @param [in] intraNr         : Intra Frequency cell node to be encoded
 *  @param [in] nrIntraWlBlInfo : pointer that points to API list, that is 
 *                                updated with the white and Black listed 
 *                                intra freq cells
 *  @param [in] nrUpdtType      : used by MCH module only, during NR Update 
 *                                message. Flag Indicates whether the cell 
 *                                need to be added or deleted from the list
 *  @return None
 */
PRIVATE Void wrEmmAnrUpdateIntraFreqWlInfo
(
   WrEutranNeighCellCb           *intraNr,
   WrNrIntraFreqInfo             *nrIntraWlBlInfo,
   WrAnrNrUpdtType               nrUpdtType
)
{
   U8                            cellIndex = 0;
   WrNeighCellInfo               *nrCellInfo = NULLP; 
   
   TRC2(wrEmmAnrUpdateIntraFreqWlInfo);

   cellIndex = nrIntraWlBlInfo->wlData.numWlCells;
  
   if(cellIndex < WR_MAX_CELL_PER_FREQ)
   {
      nrCellInfo = nrIntraWlBlInfo->wlData.cellInfo; 

      nrCellInfo[cellIndex].phyCellId = intraNr->pci;
      nrCellInfo[cellIndex].qoffset = intraNr->qoffset;
      nrCellInfo[cellIndex].cio = intraNr->cio;

      nrCellInfo[cellIndex].nrUpdtType = nrUpdtType;

      nrIntraWlBlInfo->wlData.numWlCells++;
   }
   else
   {
      RLOG0(L_WARNING,":wrEmmAnrUpdateIntraFreqWlInfo \
                                     Max cells per freq exceeded ");
   }

   RLOG1(L_INFO, "Number of cells in NR = %d", nrIntraWlBlInfo->wlData.numWlCells);

   RETVOID;
} /* end of wrEmmAnrUpdateIntraFreqWlInfo */ 


/**
 *  @brief This function encodes the Intra Freq cell info as black list cell
 *         into the cell Info list, which will be sent to MCH/PBM
 *
 *      Function: wrEmmAnrUpdateIntraFreqBlInfo
 *
 *          Processing steps:
 *          - check for the frequency in the current frequency list
 *             - get the cell node index with in the Black list
 *             - if cell Index < max num of cells taht can be encoded
 *                - add the physCellId into the list of that frequency
 *                - update the nrUpdtType
 *                - increment number of PCI in the list
 *
 *  @param [in] intraNr         : Intra Frequency cell node to be encoded
 *  @param [in] nrIntraWlBlInfo : pointer that points to API list, that is 
 *                                updated with the white and Black listed 
 *                                intra freq cells
 *  @param [in] nrUpdtType      : used by MCH module only, during NR Update 
 *                                message. Flag Indicates whether the cell 
 *                                need to be added or deleted from the list
 *  @return None
 */

PRIVATE Void wrEmmAnrUpdateIntraFreqBlInfo
(
   WrEutranNeighCellCb           *intraNr,
   WrNrIntraFreqInfo             *nrIntraWlBlInfo,
   WrAnrNrUpdtType               nrUpdtType
)
{
   U8                            pciIndex = 0;
   WrPhysCellIdRange             *blInfo = NULLP; 

   TRC2(wrAnrEmmUpdateIntraFreqInfo);

   pciIndex = nrIntraWlBlInfo->blData.numBlInfos;

   if(intraNr != NULLP)
   {
      if(pciIndex < WR_MAX_CELL_PER_FREQ)
      {
         blInfo =  nrIntraWlBlInfo->blData.blInfo;
            
         blInfo[pciIndex].startPhyCellID = intraNr->pci;
         blInfo[pciIndex].phyCellIdRangePres = FALSE;
         blInfo[pciIndex].nrUpdtType = nrUpdtType;

         nrIntraWlBlInfo->blData.numBlInfos++;
      }
      else
      {
         RLOG0(L_WARNING,"wrEmmAnrUpdateIntraFreqBlInfo \
                  Max cells per freq exceeded ");
      }
   }
   else
   {
      RLOG0(L_WARNING, "wrEmmAnrUpdateIntraFreqBlInfo \
               Max cells per freq exceeded ");
   }

   RLOG1(L_INFO, "Number of cells in ANR = %d", 
      nrIntraWlBlInfo->blData.numBlInfos);

   RETVOID;
} /* end of wrEmmAnrUpdateIntraFreqBlInfo */ 


/**
 *  @brief This function forms the white list and Black list of Intra-Freq 
 *         Neibhor cells for PBM and MCH
 *
 *  @details This Function forms the list of neighbor cell PCI's for whom HO
 *           need to be done(white List) and not to be done (Black List).
 *
 *      Function: wrEmmAnrFormIntraFreqLst
 *
 *          Processing steps:
 *          - get the white list freq Node conatining Wl and BL cells
 *          - traverse the white list 
 *             - for each node update the cell Info
 *          - traverse the Black list 
 *             - for each node update the cell Info
 *          - get the BL cell Info   
 *          - traverse the BL cell Info list 
 *             - for each node update the cell Info
 *          - get the BL Eutran Nr List
 *          - traverse the BL Eutran NR list 
 *             - for each node update the cell Info
 *
 *  @param [in] nrWlBlInfo : pointer in which intra freq WL and BL is formed
 *  @param [in] cellCb     : pointer to cell CB
 *  @param [in] brInfo     : enum indicating whether the info is for Broadcast
 *                           module or not
 *  @return None
 */

PRIVATE Void wrEmmAnrFormIntraFreqLst
(
   WrNrWlBlInfo                  *nrWlBlInfo,
   WrCellCb                      *cellCb,
   WrBrdCstInfo                  brInfo
)
{
   WrNrIntraFreqInfo             *nrIntraWlBlInfo = NULLP;

   CmLListCp                     *nbrCellLst = NULLP;
   CmLList                       *nbrCellLnk = NULLP;
   WrNrEutranFreqCb               *freqNode = NULLP;
   WrEutranNeighCellCb           *nbrCellNode = NULLP;

   TRC2(wrEmmAnrFormIntraFreqLst)

   nrIntraWlBlInfo = &nrWlBlInfo->intra;
   nrIntraWlBlInfo->wlData.numWlCells = 0;
   nrIntraWlBlInfo->blData.numBlInfos = 0;

   /* Update the Frequency List */
   freqNode = &cellCb->nrCb->eutranCb.intraFreqCb;

   nbrCellLst = &freqNode->nrWlLst;
   nbrCellLnk = nbrCellLst->first;

   while(nbrCellLnk != NULLP)
   {
      nbrCellNode = (WrEutranNeighCellCb *)nbrCellLnk->node;
      if(nbrCellNode != NULLP)
      {
         wrEmmAnrUpdateIntraFreqWlInfo(nbrCellNode, nrIntraWlBlInfo, WR_EMM_ANR_ADD_NR);
      }
      nbrCellLnk = nbrCellLnk->next;
   }

   /* Update the Black List from HO Black List */
   nbrCellLst = &freqNode->nrBlLst;
   nbrCellLnk = nbrCellLst->first;

   while(nbrCellLnk != NULLP)
   {
      nbrCellNode = (WrEutranNeighCellCb *)nbrCellLnk->node;
      if(nbrCellNode != NULLP)
      {
         wrEmmAnrUpdateIntraFreqBlInfo(nbrCellNode, nrIntraWlBlInfo, WR_EMM_ANR_ADD_NR);
      }
      nbrCellLnk = nbrCellLnk->next;
   }

   RETVOID;   
} /* end of wrEmmAnrFormIntraFreqLst */ 

/**
 *  @brief This function forms the white list and black list for all
 *         frequiencies in the neighbor list and send to PBM
 *
 *      Function: wrEmmAnrGetWhiteAndBlackList
 *
 *          Processing steps:
 *          - Switch to the given NR type
 *          - call the Handler to find and form the NR list
 *          - return the pointer 
 *          
 *  @param [in] cellId : cellID of the cell from which white and black list 
 *                        need to be generated
 *  @param [in] nrType : Type of the NR for which NR white list need to be
 *                       fetched
 *  @param [in] brInfo : Indicates whether the info is for broadcast module
 *  @return WrNrWlBlInfo*
 */
PUBLIC WrNrWlBlInfo* wrEmmAnrGetWhiteAndBlackList 
(
    U8                           cellId,
    WrLstNrType                  nrType, 
    WrBrdCstInfo                 brInfo
)
{
   WrCellCb                      *cellCb  = NULLP;

   TRC2(wrEmmAnrGetWhiteAndBlackList)
  
   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {   
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrGetWhiteAndBlackList: Failed to get cell CB");
       RETVALUE(NULLP);
   }    
   switch(nrType)
   {
      case     WR_ANR_LST_INTRA_FREQ:
         wrEmmAnrFormIntraFreqLst(cellCb->nrCb->nrWlBlInfo, 
                     cellCb, brInfo);
         break;
      case     WR_ANR_LST_INTER_FREQ:
         wrEmmAnrFormInterFreqLst(cellCb->nrCb->nrWlBlInfo,
                     cellCb, brInfo);
         break;
      case     WR_ANR_LST_CDMA_2K_1xRTT:
         wrEmmAnrFormCdma1xRttFreqLst(cellCb->nrCb->nrWlBlInfo,
                     cellCb, brInfo);
         break;
      default:
         RLOG0(L_WARNING, "Nr Type is not supported");
         break;
   };

   RETVALUE(cellCb->nrCb->nrWlBlInfo);
} /* end of wrEmmAnrGetWhiteAndBlackList */ 

/**
 *  @brief This function used to configure List of frequencies to UMM ANR 
 *       module for the ANR measurement algorithm
 *
 *      Function: wrEmmCfgAnrMeasFreqInfo
 *
 *          Processing steps:
 *          - Update list of frequency to UMM Anr module.
 *          
 *  @param [in] cellCb : pointer to cell Cb
 *  @return S16 
 */
PUBLIC S16 wrEmmCfgAnrMeasFreqInfo
(
 WrCellCb                     *cellCb
)
{
   WrUmmAnrMeasInfo            *anrMeasInfo = NULLP;
   WrNrEutranCb                *eutranCb;
   CmLList                     *lnk;
   WrNrEutranFreqCb            *eutranFreqCb;
   WrNrUtranFreqCb             *utranFreqCb;

   TRC2(wrEmmCfgAnrMeasFreqInfo)
   WR_ALLOC(&anrMeasInfo, sizeof(WrUmmAnrMeasInfo));
   if(anrMeasInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   eutranCb = &cellCb->nrCb->eutranCb;

   /*Intra frequency Arfcn used for ANR Algorithm*/
   anrMeasInfo->intraFreq.pres = TRUE;
   anrMeasInfo->intraFreq.val = eutranCb->intraFreqCb.earfcn;
   /*List of Inter frequency Arfcn's used for ANR Algorithm*/
   lnk = eutranCb->interFreqList.first;
   while(lnk != NULLP)
   {
      eutranFreqCb = (WrNrEutranFreqCb*)lnk->node;
      if(eutranFreqCb != NULLP)
      {
         anrMeasInfo->interArfcn[anrMeasInfo->interArfcnCnt++] = eutranFreqCb->earfcn;
      }
      lnk = lnk->next;
   }
   lnk = NULLP;
   /* adding Utra frequency list*/
   lnk = cellCb->nrCb->utranCb.utranFreqList.first;
   while(lnk != NULLP)
   {
      utranFreqCb = (WrNrUtranFreqCb*)lnk->node;
      if(utranFreqCb != NULLP)
      {
         anrMeasInfo->utranArfcn[anrMeasInfo->utranArfcnCnt++] = utranFreqCb->arfcn;
      }
      lnk = lnk->next;
   }
   wrUmmAnrMeasInitCfg(cellCb->cellId, anrMeasInfo);
   RETVALUE(ROK);
}
PRIVATE Void wrEmmAnrPrintInterFreqNode
(
WrNrEutranCb                 *eutranCb 
)
{
   CmLList                       *nbrCellLnk = NULLP;
   WrNrEutranFreqCb              *freqNode = NULLP;
   WrEutranNeighCellCb           *nbrCellNode = NULLP;
   CmLListCp                     *nbrCellLst = NULLP;


   TRC2(wrEmmAnrPrintInterFreqNode)

   freqNode = &eutranCb->intraFreqCb;

   nbrCellLst = &freqNode->nrWlLst;
   nbrCellLnk = nbrCellLst->first;

   ALARM(" \n\n**************************************************************** \n");
   ALARM(" Printing Intra-Frequency Node Information\n");
   ALARM(" **************************************************************** \n\n");


   while(nbrCellLnk != NULLP)
   {
      nbrCellNode = (WrEutranNeighCellCb *)nbrCellLnk->node;
      if(nbrCellNode != NULLP)
      {
          ALARM("wrEmmAnrPrintCellInfo:PCI of whitelisted cell = %d\n",nbrCellNode->pci);
          ALARM("wrEmmAnrPrintCellInfo:Rank of whitelisted cell = %d\n",nbrCellNode->curRank);

      }
      nbrCellLnk = nbrCellLnk->next;
   }

   nbrCellLst = &freqNode->nrBlLst;
   nbrCellLnk = nbrCellLst->first;

   while(nbrCellLnk != NULLP)
   {
      nbrCellNode = (WrEutranNeighCellCb *)nbrCellLnk->node;
      if(nbrCellNode != NULLP)
      {
          ALARM("wrEmmAnrPrintCellInfo:PCI of blacklisted cell  = %d\n",nbrCellNode->pci);
          ALARM("wrEmmAnrPrintCellInfo:Rank of blacklisted cell = %d\n",nbrCellNode->curRank);

      }
      nbrCellLnk = nbrCellLnk->next;
   }

   RETVOID;

}

/**
 *  @brief This function used to DUMP the NRT information for UTRA TDD
 *
 *      Function: wrEmmAnrPrintUtraTddCellInfo
 *
 *          Processing steps:
 *          - Access the FreqList from there get Nr table
 *            for each Freq get Cell Information 
 *          
 *  @return VOID 
 */
PUBLIC  void wrEmmAnrPrintUtraTddCellInfo
(
U8                            cellId
)
{
   WrCellCb                  *cellCb  = NULLP;
   CmLListCp						*utraTddFreqLst = NULLP;
   CmLList							*lnk = NULLP;
   WrNrUtranFreqCb			  *freqNode  = NULLP;
   WrUtraNeighCellCb 		  *nbrUtraCellNode = NULLP;
   CmLListCp					  *utraCellLst = NULLP;
   CmLList						  *nbrUtraCelllnk = NULLP;
   U16                       index1 = 0;

   TRC2(wrEmmAnrPrintUtraTddCellInfo)
   ALARM(" \n\n**************************************************************** \n");
   ALARM(" Printing UTRA TDD NRT Information\n");
   RLOG0(L_INFO, "Printing UTRA TDD NRT Information");
   ALARM(" **************************************************************** \n\n");

      WR_GET_CELLCB(cellCb, cellId);
      if(NULLP == cellCb)
      {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrPrintUtraTddCellInfo: Failed to get cell CB");
         RETVOID;
      }

      utraTddFreqLst = &cellCb->nrCb->utranTddCb.utranFreqList; // wrEmmGetUtraTddFreqLst(1);

      lnk = utraTddFreqLst->first;

   while(lnk != NULLP)
   {		
      freqNode = (WrNrUtranFreqCb*) lnk->node;

      if (NULLP == freqNode)
      {
         RLOG0(L_WARNING, "freqNode is NULLP");
         RETVOID;
      }
      /*Get the UTRA WL cell count */
      utraCellLst = &freqNode->nrWlLst;
      nbrUtraCelllnk = utraCellLst->first;
      while (nbrUtraCelllnk != NULLP)
      { 
         nbrUtraCellNode = (WrUtraNeighCellCb*) nbrUtraCelllnk->node;
         if (nbrUtraCellNode != NULLP)
         {
            ALARM("\nUTRA TDD ARFCN = %d\n", nbrUtraCellNode->t.utraTdd.arfcn);
            ALARM("UTRA TDD PSC = %d\n", nbrUtraCellNode->t.utraTdd.psc);
            RLOG2(L_INFO, "UTRA TDD ARFCN: %d and PSC: %d", nbrUtraCellNode->t.utraTdd.arfcn, nbrUtraCellNode->t.utraTdd.psc);
            if(nbrUtraCellNode->utraSI.pres == PRSNT_NODEF)
            {
               ALARM("UTRA [%d] PSC has SI\n", nbrUtraCellNode->t.utraTdd.psc);
               RLOG1(L_INFO, "UTRA [%d] PSC has SI", nbrUtraCellNode->t.utraTdd.psc);
            }
            else
            {
               ALARM("UTRA [%d] PSC Does NOT has SI\n", nbrUtraCellNode->t.utraTdd.psc);
               RLOG1(L_INFO, "UTRA [%d] PSC Does NOT has SI", nbrUtraCellNode->t.utraTdd.psc);
            }
            index1++;
         }
         nbrUtraCelllnk = nbrUtraCelllnk->next;
      }
      if (nbrUtraCellNode != NULLP)
      {
         ALARM("Total UTRA TDD NGH ARFCN [%d] has [%d] Cells \n\n",
               nbrUtraCellNode->t.utraTdd.arfcn, index1);
         RLOG2(L_INFO, "Total UTRA TDD NGH ARFCN [%d] has [%d] Cells",
               nbrUtraCellNode->t.utraTdd.arfcn, index1);
      }
      index1 = 0;
      lnk = lnk->next;
   }

   RETVOID;
}

/**
 *  @brief This function used to DUMP the NRT information for UTRA FDD
 *
 *      Function: wrEmmAnrPrintUtraFddCellInfo
 *
 *          Processing steps:
 *          - Access the FreqList from there get Nr table
 *            for each Freq get Cell Information 
 *          
 *  @return VOID 
 */
PUBLIC  void wrEmmAnrPrintUtraFddCellInfo
(
 U8                       cellId
 )
{
   WrCellCb                  *cellCb  = NULLP;
   CmLListCp						*utraFddFreqLst = NULLP;
   CmLList							*lnk = NULLP;
   WrNrUtranFreqCb			  *freqNode  = NULLP;
   WrUtraNeighCellCb 		  *nbrUtraCellNode = NULLP;
   CmLListCp					  *utraCellLst = NULLP;
   CmLList						  *nbrUtraCelllnk = NULLP;
   U16                       index1 = 0;

   TRC2(wrEmmAnrPrintUtraTddCellInfo)
   ALARM(" \n\n**************************************************************** \n");
   ALARM(" Printing UTRA FDD NRT Information\n");
   ALARM(" **************************************************************** \n\n");

      WR_GET_CELLCB(cellCb, cellId);

      if(NULLP == cellCb)
      {
         RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "wrEmmAnrPrintUtraFddCellInfo: Failed to get cell CB");
         RETVOID;
      }
   utraFddFreqLst = &cellCb->nrCb->utranCb.utranFreqList; // wrEmmGetUtraTddFreqLst(1);

  lnk = utraFddFreqLst->first;

   while(lnk != NULLP)
   {		
      freqNode = (WrNrUtranFreqCb*) lnk->node;

      if (NULLP == freqNode)
      {
         RLOG0(L_WARNING, "freqNode is NULLP");
         RETVOID;
      }
      /*Get the UTRA WL cell count */
      utraCellLst = &freqNode->nrWlLst;
      nbrUtraCelllnk = utraCellLst->first;
      while (nbrUtraCelllnk != NULLP)
      { 
         nbrUtraCellNode = (WrUtraNeighCellCb*) nbrUtraCelllnk->node;
         if (nbrUtraCellNode != NULLP)
         {
            ALARM("\nUTRA FDD ARFCN = %d\n", nbrUtraCellNode->t.utraFdd.arfcnDl);
            ALARM("UTRA FDD PSC = %d\n", nbrUtraCellNode->t.utraFdd.psc);
            if(nbrUtraCellNode->utraSI.pres == PRSNT_NODEF)
            {
               ALARM("UTRA [%d] PSC has SI\n", nbrUtraCellNode->t.utraFdd.psc);
            }else
            {
               ALARM("UTRA [%d] PSC Does NOT has SI\n", nbrUtraCellNode->t.utraFdd.psc);
            }
            index1++;
         }
         nbrUtraCelllnk = nbrUtraCelllnk->next;
      }
      if (nbrUtraCellNode != NULLP)
      {
         ALARM("Total UTRA FDD NGH ARFCN [%d] has [%d] Cells \n\n",
               nbrUtraCellNode->t.utraFdd.arfcnDl, index1);
      }
      index1 = 0;
      lnk = lnk->next;
   }

   RETVOID;
}

/**
 *  @brief This function used to DUMP the NRT information for EUTRAN
 *
 *      Function: wrEmmAnrPrintCellInfoo
 *
 *          Processing steps:
 *          - Access the FreqList from there get Nr table
 *            for each Freq get Cell Information 
 *          
 *  @return VOID 
 */
PUBLIC Void wrEmmAnrPrintCellInfo
(
 void
)
{
   WrCellId                  cellId = 0x01;   /* cellId should be as 
                                                 the one configured in
                                                 config .txt file */
   WrCellCb                  *cellCb  = NULLP;
   WrNrEutranCb              *eutranCb =NULLP;
   WrNrEutranFreqCb          *freqNode = NULLP;
   WrNrEutranFreqCb          *eutranFreqNode = NULLP;
   WrEutranNeighCellCb       *eutranNr = NULLP;
   CmLList                   *interFreqlink;
   CmLListCp                 *lst = NULLP;
   CmLListCp                 *nbrWCellLst =NULLP;
   CmLList                   *nbrCbWlLnk;
   CmLListCp                 *nbrBCellLst =NULLP;
   CmLList                   *nbrCbBlLnk;
   U8              nodeCount;


   TRC2(wrEmmAnrPrintCellInfo)

   WR_GET_CELLCB(cellCb, cellId)
   
   if (cellCb == NULLP)
   {
      RLOG1(L_ERROR,
               "wrEmmAnrPrintCellInfo:Cell Cb for Cell Id %d Not found\n", cellId);
      RETVOID;
   }
   eutranCb = &cellCb->nrCb->eutranCb;
   freqNode = &cellCb->nrCb->eutranCb.intraFreqCb;
   lst      = &eutranCb->interFreqList; 

   ALARM(" \n\n**************************************************************** \n");
   ALARM(" Printing Inter-Frequency Node Information\n");
   ALARM(" **************************************************************** \n\n");

   /* Printing list of Inter frequency node Information */
   interFreqlink = lst->first;
   nodeCount = 1;
   while(interFreqlink != NULLP)
   {
      ALARM("Frequency Node %d\n ",nodeCount);
      eutranFreqNode = (WrNrEutranFreqCb *)interFreqlink->node;
      ALARM("wrEmmAnrPrintCellInfo: Frequency node with arfcn :%d\n",eutranFreqNode->earfcn);

      nbrWCellLst = &freqNode->nrWlLst;
      nbrCbWlLnk = nbrWCellLst->first;

      while(nbrCbWlLnk != NULLP)
      {
         eutranNr = (WrEutranNeighCellCb *)nbrCbWlLnk->node;

         ALARM("wrEmmAnrPrintCellInfo:PCI of whitelisted cell  = %d\n",eutranNr->pci);
         ALARM("wrEmmAnrPrintCellInfo:Rank of whitelisted cell = %d\n",eutranNr->curRank);
         RLOG2(L_DEBUG, "PCI of whitelisted cell = %d, Rank of whitelisted cell = %d", eutranNr->pci, eutranNr->curRank);
         nbrCbWlLnk = nbrCbWlLnk->next;
      }

      nbrBCellLst = &freqNode->nrBlLst;
      nbrCbBlLnk =  nbrBCellLst->first;
      while(nbrCbBlLnk != NULLP)
      {
         eutranNr = (WrEutranNeighCellCb *)nbrCbBlLnk->node;

         ALARM("wrEmmAnrPrintCellInfo:PCI of blacklisted cell = %d\n",eutranNr->pci);
         ALARM("wrEmmAnrPrintCellInfo:Rank of blacklisted cell = %d\n",eutranNr->curRank);
         RLOG2(L_DEBUG, "PCI of blacklisted cell = %d, Rank of blacklisted cell = %d", eutranNr->pci, eutranNr->curRank);
         nbrCbBlLnk = nbrCbBlLnk->next;
      }
     
      interFreqlink = interFreqlink->next;
      nodeCount++; 
   }

   wrEmmAnrPrintInterFreqNode(eutranCb);
   RETVOID;
 }

/**
 *  @brief This function returns neighbour cell node from   
 *       whitelist for a given PCI
 *
 *      Function: wrEmmAnrGetNbrEutranCellNode
 *
 *          Processing steps:
 *          
 *  @param [in] cellId : serving cell ID 
 *  @param [in] earfcnDl : downlink arfcn 
 *  @param [in] pci : physical cell id  
 *  @return WrEutranNeighCellCb*
 */

PRIVATE WrEutranNeighCellCb* wrEmmAnrGetNbrEutranCellNode
                                    (CmLListCp * lCp, U32 pci)
{

   WrEutranNeighCellCb           *nbrCellNode = NULLP;
   CmLList                       *nbrCellLnk = NULLP;
   
   TRC2(wrEmmAnrGetNbrEutranCellNode)
   
   /*get the cell node from white list*/
   nbrCellLnk = lCp->first;
   while(nbrCellLnk != NULLP)
   {
      nbrCellNode = (WrEutranNeighCellCb *)nbrCellLnk->node;

      if(nbrCellNode->pci == pci )
      {
         break;
      }
      nbrCellLnk = nbrCellLnk->next;
   }
   RETVALUE(nbrCellNode);
}


/**
 *  @brief     function for handling scenarios when s1 handover fails
 *
 *      Function: wrEmmAnrHdlS1HoFailScenario
 *
 *          Processing steps:
 *             - decrease the cell Rank by 1
 *             - check if cell Rank becomes less than threshold
 *                - if yes
 *                   - delete the cell from the list and insert it into black list
 *                   - if all the cells for this frequency become white,take the 
 *                     oldest black cell(assumption: first node of black list)
 *                   - update the changes in black list to PBM and measurment module
 *                - if no
 *                   - do nothing
 *          
 *  @param [in] cellCb :  
 *  @param [in] earfcnDl : downlink arfcn of target cell
 *  @param [in] freqCb : frequency node 
 *  @param [in] nbrCellNode :cellNode
 */

PRIVATE S16 wrEmmAnrHdlS1HoFailScenario(WrCellCb *cellCb ,U32 earfcnDl,
               WrNrEutranFreqCb *freqNode,WrEutranNeighCellCb * nbrCellNode)
{

   CmLListCp                     *nbrCellLst = NULLP;
   CmLList                       *cellNode = NULLP;  
   S16                            ret      = ROK;

   TRC2(wrEmmAnrHdlS1HoFailScenario)

   nbrCellLst = &freqNode->nrWlLst;
   nbrCellNode->curRank -= 1;
   if(nbrCellNode->curRank < WR_ANR_DEFAULT_MIN_RANK_WEIGHT )
   {
      cmLListDelFrm(nbrCellLst,&nbrCellNode->nrWlBlLnk);
      nbrCellNode->blackListed = TRUE;
      wrEmmAnrInsertEutranNodeIntoList(cellCb, nbrCellNode);
      if(earfcnDl == cellCb->dlCarrierFreq)
      {
         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=  
            WR_ANR_INTRA_FREQ_BIT;
      }
      else
      {
         cellCb->nrCb->nrUpdtInfo->freqTypeBitMap |=
            WR_ANR_INTER_FREQ_BIT;
      }
      if(freqNode->nrWlLst.count == 0)
      {
         cellNode = freqNode->nrBlLst.first;
         /*move the first black list cell to white list */
         cmLListDelFrm(&freqNode->nrBlLst,cellNode);
         /*add this node to white list*/
         cmLListAdd2Tail(&freqNode->nrWlLst,cellNode);
      } 
      /* send the list update to EMM module */
      ret = wrEmmAnrProcNrUpdtInfo(cellCb, FALSE);
   }
   RETVALUE(ret);
}/*end of wrEmmAnrHdlS1HoFailScenario*/

/**
 *  @brief     function for updating ranking of cells,should be called after
 *            s1 handover success /failure
 *
 *      Function: wrEmmAnrUpdtNrCellRank
 *
 *          Processing steps:
 *          
 *  @param [in] cellId : serving cell ID 
 *  @param [in] earfcnDl : downlink arfcn of target cell
 *  @param [in] pci : physical cell id of target cell 
 *  @param [in] hoFailed :handover status success/failure
 *  @return ROK
 */
PUBLIC S16 wrEmmAnrUpdtNrCellRank(U16 cellId,U16 hoPci,U32 earfcnDl, Bool hoFailed)
{

   WrCellCb                      *cellCb  = NULLP;
   WrNrEutranFreqCb              *freqNode = NULLP; 
   WrEutranNeighCellCb           *nbrCellNode = NULLP;
   CmLListCp                     *nbrCellLst = NULLP;
   S16                            ret        = ROK;
   TRC2(wrEmmAnrUpdtNrCellRank)
   
   WR_GET_CELLCB(cellCb, cellId)
   if(cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "cellCb is NULL");
      RETVALUE(RFAILED);
   }
   /*get the frequency node*/
   freqNode = wrEmmAnrGetEutranFreqNode(earfcnDl, 
                                       &cellCb->nrCb->eutranCb);
   if(freqNode == NULLP)
   {
      RLOG0(L_ERROR, "freqNode is NULL");
      RETVALUE(RFAILED);
   }
   nbrCellLst = &freqNode->nrWlLst;
   nbrCellNode = wrEmmAnrGetNbrEutranCellNode(nbrCellLst,hoPci);
   if(nbrCellNode != NULLP)
   {
      if(hoFailed == FALSE) 
      {
         if(nbrCellNode->curRank < WR_ANR_DEFAULT_MAX_RANK_WEIGHT)
         {
            nbrCellNode->curRank += 1;
         }
         RETVALUE(ROK);
      }
      ret = wrEmmAnrHdlS1HoFailScenario(cellCb,earfcnDl,freqNode,nbrCellNode);
   }/*end of if(nbrCellNode!=NULLP) */
   RETVALUE(ret);
}

/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
