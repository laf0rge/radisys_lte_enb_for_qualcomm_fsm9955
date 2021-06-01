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
               

     File:     wr_emm_plmn.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=8;

#include "wr.h"
#include "wr_utils.h"
#include "wr_emm.h"
#include "wr_umm_rrccon.h"
EXTERN Bool wrEmmPlmnPlmnsEqual(WrPlmnId  *plmnId1,WrPlmnId  *plmnId2);
EXTERN S16 wrEmmGetMmeForPlmnEstCause
(
U32                          establishCause,
WrRrcConTransCb              *rrccon
);

EXTERN WrPlmnCb *wrEmmPlmnGetPlmnCb
(
WrPlmnId                     *plmnId
);
/** @brief This function validates the requested PLMNs are equal or not.
 *
 * @details This function validates the requested source and destination PLMNs
 *          are equal or not.
 *
 *     Function: wrEmmPlmnPlmnsEqual
 *
 *         Processing steps:
 *         - If the both requested PLMN ID's number of MNC digitts  not equal
 *           then return both the PLMNs are not equal.
 *         - Otherwise compare both the requested PLMN IDs are having the same
 *           MCC and MNC digit values.If bothe are matches then return TRUE.
 *           otherwise FALSE.
 *
 * @param[in] plmnId1: source PLMN ID
 * @param[in] plmnId2: destination PLMN ID
 * @return Bool
 *        -# Success : TRUE
 *        -# Failure : FALSE
 */
PUBLIC Bool wrEmmPlmnPlmnsEqual
(
WrPlmnId                     *plmnId1,
WrPlmnId                     *plmnId2
)
{
   if ((plmnId1->mcc[0] != plmnId2->mcc[0]) ||
       (plmnId1->mcc[1] != plmnId2->mcc[1]) ||
       (plmnId1->mcc[2] != plmnId2->mcc[2]))
   {
      RETVALUE(FALSE);
   }
   if (plmnId1->numMncDigits != plmnId2->numMncDigits)
   {
      RETVALUE(FALSE);
   }
   if ((plmnId1->mnc[0] != plmnId2->mnc[0]) ||
       (plmnId1->mnc[1] != plmnId2->mnc[1]))
   {
      RETVALUE(FALSE);
   }
   if (plmnId1->numMncDigits == 3)
   {
      if (plmnId1->mnc[2] != plmnId2->mnc[2])
      {
         RETVALUE(FALSE);
      }
   }
   RETVALUE(TRUE);
} /* end of wrEmmPlmnPlmnsEqual */

/** @brief This function adds the new PLMN to list of suppoted PLMNs to the
 *         EnodeB Control block.
 *
 * @details This function adds the new PLMN to list of suppoted PLMNs to the
 *          enodeb control block.
 *
 *     Function: wrEmmPlmnAddPlmn
 *
 *         Processing steps:
 *         - If there is no match of PLM IDs supported with the requested PLMN
 *           ID, then create a new PLMN control block
 *         - Initialise the supported list of MMEs for that PLMN ID is 0.
 *         - Increase the number of PLMNs supported by EnodeB.
 *
 *
 * @param[in] plmnInfo : PLMN information which is going to add if not exist
 *                       already.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPlmnAddPlmn
(
WrPlmnInfo                   *plmnInfo
)
{
   WrPlmnCont                *plmns = &(wrEmmCb.plmns);
   U32                       plmnIdx;

   for (plmnIdx = 0; plmnIdx < plmns->numPlmns; plmnIdx++)
   {
      WrPlmnInfo    *cfgdPlmn = &plmns->plmns[plmnIdx].plmnInfo;
      if (wrUtlPlmnsEqual(&cfgdPlmn->plmnId, &plmnInfo->plmnId) == TRUE)
      {
         RETVALUE(ROK);
      }
   }
   if (plmns->numPlmns >= WR_MAX_PLMN_IDS)
   {
      RETVALUE(RFAILED);
   }
   plmns->plmns[plmns->numPlmns].plmnInfo = *plmnInfo;
   plmns->plmns[plmns->numPlmns].numMmes  = 0;
   plmns->numPlmns++;
   RETVALUE(ROK);
} /* end of wrEmmPlmnAddPlmn */

/** @brief This function find the PLMN Control Block based on the PLMN ID.
 *
 * @details This function find the PLMN Control Block based on the PLMN ID.
 *
 *     Function: wrEmmPlmnGetPlmnCb
 *
 *         Processing steps:
 *         - Traverse the list PLMN Ids supported by EnodeB, If PLMN control
 *           block matches with the same PLMN ID then return with PLMN control
 *           block pointer.
 *
 * @param[in] plmnId  : get PLMNCB for which plmnId is same.
 * @return PTR
 *          -# Success : PLMNCB pointer
 *          -# Failure : NULLP
 */
PUBLIC WrPlmnCb *wrEmmPlmnGetPlmnCb
(
WrPlmnId                     *plmnId
)
{
   U32                       idx;

   for(idx = 0; idx < wrEmmCb.plmns.numPlmns; idx++)
   {
      if (wrEmmPlmnPlmnsEqual(&wrEmmCb.plmns.plmns[idx].plmnInfo.plmnId,
                                                                plmnId) == TRUE)
      {
         RETVALUE(&wrEmmCb.plmns.plmns[idx]);
      }
   }
   RETVALUE(NULLP);
} /* end of wrEmmPlmnGetPlmnCb */

/** @brief This function will add the PLMN supported by a MME.
 *
 * @details This function will add new MME Id to the PLMN suppored MMEs.
 *
 *     Function:wrEmmPlmnAddMme
 *
 *         Processing steps:
 *         - Get the PLMN control block based on PLMN Id.
 *         - For this plmn control block supported MMEs, add the new MME ID also
 *           to supporting MMEs for this PLMN ID.
 *         - If the current PLMN ID is deactivated, then activate the PLMN control
 *           block to start broad casting this new PLMN.
 *
 * @param[in] plmnId  : adding PlmnId for whcih mmeId is same.
 * @param[in] mmeId   : mmeId of the MME.
 * @return  S16
 *          -# Success : ROK
 */
PUBLIC S16 wrEmmPlmnAddMme
(
WrPlmnId                     *plmnId,
WrMmeId                      mmeId
)
{
   WrPlmnCb                  *plmn;

   /* Locate the PLMN related information from the cell control block     */
   plmn = wrEmmPlmnGetPlmnCb(plmnId);
   if (plmn == NULLP)
   {
      RLOG0(L_ERROR, "Unknown PLMN Id");
      RETVALUE(RFAILED);
   }
   if (plmn->numMmes < WR_MAX_MMES)
   {
      plmn->mmes[plmn->numMmes] = mmeId;
      plmn->numMmes++;
      if (plmn->numMmes == 1)
      {
         /* Activate PLMN in the broadcast module for each cell           */
         wrEmmPbmModifyPlmn(&plmn->plmnInfo.plmnId, TRUE);
         wrEmmCb.cellInfo.isCellModified = TRUE;
      }
      RETVALUE(ROK);
   }

   /* This is not one of the broadcast PLMNs. It is fine to ignore        */
   RETVALUE(ROK);
} /* end of wrEmmPlmnAddMme */

/** @brief This function will stops supporting requested PLMN by
 *         the requested MME.
 *
 * @details This function will stops the specific PLMN ID support
 *          for specific MME.If the PLMN is not supporting any MME
 *          then deactivates that particaular PLMN to indicate that
 *          not including in the broadcasting.
 *
 *     Function: wrEmmPlmnDelMme
 *
 *         Processing steps:
 *         - Get the PLMN control block based on PLMN Id.
 *         - Form this plmn control block supported MMEs, if the requested
 *            MME ID matches, then remove the MME ID to support this PLMN.
 *         - If the current PLMN is not supported by any one MME, then
 *           deactivate the PLMN ID.
 *
 * @param[in] plmnId  : deleting PlmnId for whcih mmeId is same.
 * @param[in] mmeId   : mmeId of the MME.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPlmnDelMme
(
WrPlmnId                     *plmnId,
WrMmeId                      mmeId
)
{
   U32                       idx;
   WrPlmnCb                  *plmnCb = NULLP;

   /* Locate the PLMN related information from the cell control block */
   plmnCb = wrEmmPlmnGetPlmnCb(plmnId);
   if ((plmnCb == NULLP) || (plmnCb->numMmes == 0))
   {
      RETVALUE(RFAILED);
   }
   for (idx = 0; idx < plmnCb->numMmes; idx++)
   {
      if (plmnCb->mmes[idx] == mmeId)
      {
         plmnCb->numMmes--;
         break;
      }
   }
   for (;idx < (plmnCb->numMmes); idx++)
   {
      plmnCb->mmes[idx] = plmnCb->mmes[idx + 1];
   }
   if (plmnCb->numMmes == 0)
   {
      /* Deactivate PLMN in the broadcast module                          */
      wrEmmPbmModifyPlmn(&plmnCb->plmnInfo.plmnId, FALSE);
      wrEmmCb.cellInfo.isCellModified = TRUE;
   }
   RETVALUE(ROK);
} /* end of wrEmmPlmnDelMme */

/** @brief This function gets the MME Id for the given PLMN Identity.
 *
 * @details This function gets the MME ID of the MME Control Block for the
 *          given PLMN ID and establishment cause.
 *
 *     Function: wrEmmGetMmeForPlmnEstCause
 *
 *         Processing steps:
 *         - Get the PLMN control block based on PLMN ID.
 *         - select the one of the MME which are serving the same PLMN 
 *           and check for overload status. 
 *
 * @param[in] establishCause : Establishment cause during RRC connection procedure.
 * @param[out] rrccon : the rrccon variable presnt in the UECB.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmGetMmeForPlmnEstCause
(
U32                          establishCause,
WrRrcConTransCb              *rrccon
)
{
   WrPlmnCb                  *plmnCb;
   WrMmeCb                   *mmeCb = NULLP;
   S16                        idx = 0;
   S16                        ret = RFAILED;
   WrMmeId                    mmeIdx ;
#ifdef MME_LDBAL
   U32                        selProb = 0x0;
#else
   U8                         relCapacity = 0;
#endif
   rrccon->mmeId              = 0xff;
   WrPlmnId          *plmnId  = rrccon->plmnId; 

   plmnCb = wrEmmPlmnGetPlmnCb(plmnId);
   if (plmnCb == NULLP)
   {
      RETVALUE(RFAILED);
   }
   
   for (idx = 0; idx < plmnCb->numMmes; idx++)
   {
       /* Get the PLMN Cb depending on the plmnId */
       /*plmnCb = wrEmmPlmnGetPlmnCb(plmnId);*/

       /* Check if we are able to get the PLMN Cb */
       mmeIdx = plmnCb->mmes[idx];
       mmeCb =wrEmmCb.mmeCont.mmes[mmeIdx];
       if (NULLP == mmeCb)
         continue;
       
       /* Incase if we reach the maximum number of mmes, reset the counter
       * else, pick up the next mme from the list */
     
       if ((mmeCb->state == WR_MME_UP) && (mmeCb->relCapacity > 0))
       {
       /*Select the Best MME which is less loaded in the MME group */
          if ((TRUE == mmeCb->mmeOvldFlag) || (mmeCb->numOvrldGummei > 0))
               {
                   if(wrGetMmeRejectStatus(establishCause,  mmeCb->mmeId))
                   {
                       continue;
                         /* RLOG0(L_ERROR,"wrUmmRrcConSelectMme: Selected MME\
                          Overloaded, returning failure. Attach Process is not completed");*/
                         /* RETVALUE(RFAILED);*/
                    }
                }
#ifdef MME_LDBAL
                /*Select the Best MME which is less loaded in the MME group */
                if(selProb < (mmeCb->relCapacity - mmeCb->dynCapacity))
                {
                    selProb = mmeCb->relCapacity - mmeCb->dynCapacity;
                    rrccon->mmeId = mmeCb->mmeId;
                    ret = ROK;
                }
#else
                if(relCapacity < mmeCb->relCapacity)
                {
                    relCapacity =  mmeCb->relCapacity;
                    rrccon->mmeId = mmeCb->mmeId;
                    ret = ROK;
                }

#endif

        }
     }
#ifdef MME_LDBAL
   if (ROK == ret)
   {
       mmeCb = wrEmmCb.mmeCont.mmes[rrccon->mmeId];
       mmeCb->dynCapacity = (mmeCb->dynCapacity+1)%(mmeCb->relCapacity);
   }
#endif
   /*Fix for ccpu00141573*/
   if(mmeCb != NULLP)
   {
      rrccon->gummei.mmec = mmeCb->codes[0];
      rrccon->gummei.mmeGrpId = mmeCb->groupIds[0];
   }
   
   RETVALUE(ret);
} /* end of  wrEmmGetMmeForPlmnEstCause */

/** @brief This function gets the MME Id for the given PLMN Identity,
 *         MME group ID and the MME Code. The parameter mmeId is updated
 *         with the ID of the MME.
 *
 * @details This function is used to get the MME ID of the MME Control block
 *          based on PLMN ID, MME Code and MME Group ID.
 *
 *     Function: wrEmmPlmnGetMme
 *
 *         Processing steps:
 *         - Get the PLMN control Blok based on plmnId
 *         - Get the MME Id of the MME Control Block based on the MME Code and
 *           MME Group ID matches.
 *
 *
 * @param[in] plmnId  : PlmnId for which MME cb needs to be get
 * @param[in] grpId   : Group Id for which MME cb needs to be get
 * @param[in] mmeCode : MME Code for which MME cb needs to be get
 * @param[out] mmeId   : mmeId of the MME CB whcih is having
 *                       same plmnId, grpId and Mme code.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPlmnGetMme
(
WrPlmnId                     *plmnId,
U16                          grpId,
U8                           mmeCode,
WrMmeId                      *mmeId
)
{
   WrPlmnCb                  *plmnCb;
   U16                       idx;
   U16                       nmbMmes;

   /* Get the PLMN Cb depending on the plmnId */
   plmnCb = wrEmmPlmnGetPlmnCb(plmnId);
   if(plmnCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   nmbMmes = plmnCb->numMmes;

   /* Match the group id and MMEcs for the MME */
   for(idx = 0; idx < nmbMmes; idx++)
   {
      if((wrEmmMmeGrpidMmecEqual(plmnCb->mmes[idx], grpId, mmeCode)) == TRUE)
      {
         *mmeId = plmnCb->mmes[idx];
         RETVALUE(ROK);
      }
   }
   RLOG2(L_ERROR,"No MME found for MMEC(%d)\t MMEGrpID(%u)", 
            mmeCode,grpId);

   RETVALUE(RFAILED);
} /* end of wrEmmPlmnGetMme */



/** @brief This function returns list of MME for a given PLMN Id.
 *
 * @details
 *
 *     Function: wrEmmPlmnGetMmeIdList
 *
 *         Processing steps:
 *         - Get the PLMN Cb based upon the PLMN ID passed.
 *         - if Sucessfully reterived, Retunr the list of Mme id configured for
             this PLMN
 *
 * @param[in] wrEmmPlmnId : PLMN id for which Mmelist has to be reterived.
 * @param[out] wrEmmPlmnMmeLst : MMe Id list for a given PLMN
 *
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmPlmnGetMmeIdList(WrPlmnCbMmeList *wrEmmPlmnMmeLst, WrPlmnId wrEmmPlmnId)
{
   WrPlmnCb  *wrPlmnCb = NULLP;

   if(wrEmmPlmnMmeLst == NULLP)
   {
      /*-TODO-*/
      RETVALUE(RFAILED);
   }

   /*-Derive the PLMN CB based upon PLMN Id Passed-*/
   wrPlmnCb = wrEmmPlmnGetPlmnCb(&wrEmmPlmnId);
   if(NULLP == wrPlmnCb)
   {
      /*-TODO-*/
      RETVALUE(RFAILED);
   }

   /*-Return MME Id list for the given PLMN -*/
   wrEmmPlmnMmeLst->crntIdx =(U32*) &wrPlmnCb->crntMmeIdx;
   wrEmmPlmnMmeLst->numOfMme =  wrPlmnCb->numMmes;
   wrEmmPlmnMmeLst->mmeIdList = wrPlmnCb->mmes;
   RETVALUE(ROK);
}

/***************************************************************************/
/* Gets the MME Id for the given PLMN Identity. The selection is made      */
/* round robin among the MMEs which have the same PLMN ID                  */
/*                                                                         */
/* Returns ROK upon success, RFAILED otherwise.                            */
/*                                                                         */
/***************************************************************************/
PUBLIC S16 wrEmmPlmnGetMmeByPlmn
(
WrPlmnId                     *plmnId,
WrMmeId                      *mmeId
)
{
   WrPlmnCb                  *plmnCb;

   /* Get the PLMN Cb depending on the plmnId */
   plmnCb = wrEmmPlmnGetPlmnCb(plmnId);

   /* Check if we are able to get the PLMN Cb */
   if (plmnCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Incase if we reach the maximum number of mmes, reset the counter
    * else, pick up the next mme from the list */
   if (plmnCb->crntMmeIdx == plmnCb->numMmes)
   {
      plmnCb->crntMmeIdx = 0;
      *mmeId = plmnCb->mmes[plmnCb->crntMmeIdx];
   }
   else
   {
      *mmeId = plmnCb->mmes[plmnCb->crntMmeIdx];
   }

   /* Increment the count which to pick next MME */
   plmnCb->crntMmeIdx++;

   RETVALUE(ROK);
} /* wrEmmPlmnGetMmeByPlmn */


PUBLIC S16 wrGetMmeIdfromPlmnId
(
WrGummei                  *gummei,
WrMmeId                   *mmeId,
U32                        establishCause
)
{
   WrPlmnCb                  *plmnCb = NULLP;
   WrMmeCb                    *mmeCb = NULLP;
   U8                         i,idx = 0;
   U8                         idx1 = 0;

   plmnCb = wrEmmPlmnGetPlmnCb(&gummei->plmnId);

   if (plmnCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < plmnCb->numMmes; idx++)
   {
      *mmeId = plmnCb->mmes[idx];
      mmeCb = wrEmmCb.mmeCont.mmes[*mmeId];
      if(mmeCb != NULL)
      {
         for(i = 0; i < mmeCb->numGrpIds; i++)
         {
            if(mmeCb->groupIds[i] == gummei->mmeGrpId)
            {
               /* Same PLMN & mmeGrp
                *                    * Check mmeCode */
               for(idx1 = 0; idx1 < mmeCb->numCodes; idx1++)
               {
                  if(mmeCb->codes[idx1] == gummei->mmec)
                  {
                     /* Same PLMN, mmeGrp & mmeCode */
                     RETVALUE(ROK);
               }
            }
         }
      }
   }
   }
      RETVALUE(RFAILED);
}


/** @brief This function is used to check whether the serving PLMN
 *          received is supported by the cell.
 *
 * @details
 *
 *     Function: wrEmmChkServPlmnHoUe
 *
 *         Processing steps:
 *         - Get the Cell CB from the received Cell Id.
 *         - Compare the received Serving PLMN with all the
 *             PLMNs present in the SIB 1 of the Cell CB.
 *
 * @return  S16
 *          -# Success : TRUE. If the received PLMN is present 
 *                             in SIB 1 of Cell CB
 *          -# Failure : FALSE
 */
PUBLIC Bool wrEmmChkServPlmnHoUe
(
 WrCellId          cellId,
 WrPlmnId          srvngPlmn
 )
{
   WrCellCb                  *cellCb;
   U8                         idx = 0;

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG1(L_ERROR,
            "wrEmmChkServPlmnHoUe: Cell Cb for Cell Id %d Not found", cellId);
      RETVALUE(FALSE);
   }

   for(idx = 0; idx < cellCb->sib1.numPlmnIds; idx++)
   {
      if(wrUtlPlmnsEqual(&srvngPlmn, &cellCb->sib1.plmns[idx].plmnId))
      {
         /* Serving PLMN is supported */
         RETVALUE(TRUE);
      }
   }
   RETVALUE(FALSE);
}

/** @brief This function returns the number of PLMNs served
 *          by the Cell.
 *
 * @details
 *
 *     Function: wrEmmGetNosOfBdcstPlmn
 *
 *         Processing steps:
 *         - Get the Cell CB from the received Cell Id
 *         - Return the number of PLMNs present in the SIB1
 *             of the Cell CB.
 *
 * @return  S16
 *          -# Success : Number of PLMNs served
 *          -# Failure : 0
 */
PUBLIC U8 wrEmmGetNosOfBdcstPlmn
(
 WrCellId          cellId
 )
{
   U8                idx = 0;
   U8                numPlmnsActivated = 0;
   WrCellCb          *cellCb;

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG1(L_ERROR,
            "wrEmmGetNosOfBdcstPlmn: Cell Cb for Cell Id %d Not found", cellId);
      RETVALUE(FALSE);
   }

   for(idx = 0; idx < cellCb->sib1.numPlmnIds; idx++)
   {
      if(cellCb->sib1.plmns[idx].activated == TRUE)
      {
         numPlmnsActivated++;
      }
   }
   RETVALUE(numPlmnsActivated);
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
