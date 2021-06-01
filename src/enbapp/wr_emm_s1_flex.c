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
               

     File:    wr_emm_s1_flex.c 

     Sid:      fc_emm_s1_flex.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:51:46 2014

     Prg:     Aravind 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=27;

/* header include files (.h) */
#include <stdlib.h>
#include "wr.h"
#include "wr_utils.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"
#include "wr_emm_mme.h"
#include "wr_emm_cfg.h"
#include "wr_emm_pbm.h"
#include "wr_ifm_s1ap.h"
#include "wr_ifm_rrm.h"
#include "wr_emm_s1_flex.h"
#include "wr_umm_ho.h"

#ifdef __cplusplus
EXTERN "C" {
#endif

PUBLIC S16 wrEmmGetLdBalMme(WrRrcConTransCb           *rrccon);
PRIVATE S16 wrEmmS1FlexGetMmeCbNotDown(CmLListCp *cmListmmeIdList, 
                                       WrMmeId *mme, WrGummei *gummei,
                                       U32 establishCause);
PRIVATE S16 wrEmmS1FlexgetMmeCbList(CmLListCp **cmListmmeIdList, WrGummei *gummei);
PRIVATE S16 wrEmmS1FlexgetFreeGummeiCbLstnUpd(WrGummei *gummei, WrMmeCb* mmeCb);

PUBLIC S16 wrEmmS1FlexHdlOverloadResponse(WrMmeCb* mmeCb, U32 overloadAction);
PUBLIC S16 wrEmmS1FlexUpdateGummeiInMmeCb(WrMmeCb* mmeCb, SztSrvdGUMMEIs* gummeis);
PUBLIC S16 wrEmmS1FlexInsertGummeiInMmeCb(WrMmeCb* mmeCb, SztSrvdGUMMEIs* gummeis);
PUBLIC S16 wrEmmSelectMmeOnGummei(WrMmeId *mmeId, WrGummei *gummei,
                                        U32 establishmentCause);
PUBLIC S16 wrEmmS1FlexInitS1FlexVarFrMme(WrMmeCb* mmeCb);
PUBLIC S16 wrEmmS1FlexInitS1FlexVarForEmmCb(Void);
PUBLIC S16 wrEmmS1FlexdelMmeCbEntryFrmGummeiCbLst(WrMmeCb* mmeCb);
PRIVATE S16 wrEmmS1FlexGetFreeGuGrupCbLstnUpd(WrGuGrpIdLst *guGrupId, WrMmeCb* mmeCb);
EXTERN WrPlmnCb *wrEmmPlmnGetPlmnCb(WrPlmnId *plmnId);

PRIVATE S16 wrEmmS1FlexGetMmeCbListFrmGuGrup
   (CmLListCp **cmListmmeIdList, WrGuGrpIdLst *guGrupId);

PUBLIC S16 wrEmmS1FlexAddGuGrupIds
(
U8 count, 
WrGuGrpIdLst *guGrpIdLstToAdd,
WrMmeCb * mmeCb
);

PUBLIC S16 wrEmmS1FlexDelMmeCbEntryFrmGuGrupCbLst(WrMmeCb* mmeCb);

PUBLIC S16 wrEmmBuildHoRestrnLst
(
WrUeCb                *ueCb,
CztEvnt               *hoReq,
CztHovrRestrnLst      *hoRestrnLst   
);

PUBLIC S16 wrEmmValidateNghAvailability
(
WrUeCb                *ueCb,
WrHoNghInfo           *hoNghInfo,
U8                    *hoStatus,
U8                    *hoType
);
EXTERN S16 wrEmmCztFillPLMNId
(
CztEvnt                      *pdu,
WrPlmnId                     *plmnId,
CztPLMN_Identity             *plmnIe
); 
EXTERN S16 wrEmmGetLdBalMme( WrRrcConTransCb           *rrccon);

EXTERN S16 wrEmmGetMmeForPlmnEstCause
(
U32                          establishCause,
WrRrcConTransCb              *rrccon
);

EXTERN  WrEmmCb   wrEmmCb;

/** @brief This function is called to check if the GUMMEI is not
 * overloaded
 *
 * @details
 *     Function: wrEmmGetGummeiOvldStatus   
 *
 *         Processing steps:
 *         - Check if the GUMME IE is overloaded
 *         - If yes, return TRUE else 
 *         - return FLASE
 *
 * @param WrRrcConTransCb  *transCb
 */

PUBLIC U32 wrEmmGetGummeiOvldStatus
(
WrGummei                    *gummei,
WrMmeId                      mmeId
)
{
   U32           gummeiOvrld = FALSE;
   U32           ovrldCnt;
   U32           isPlmnEqual;
   WrMmeCb      *mmeCb = NULLP; 

   /* Get the MME Cb for a given mmeId */
   mmeCb = wrEmmCb.mmeCont.mmes[mmeId];
   if(mmeCb == NULL)
   {
	   RETVALUE(TRUE);
   }	   

   /* Loop through all the GUMMEI list which are present in overloaded list
    * and compare them aginst the given GUMMEI. If the match is found,
    * the given GUMMEI is overloaded and return TRUE
    */
   for(ovrldCnt = 0; ovrldCnt < mmeCb->numOvrldGummei; ovrldCnt++)
   {
      isPlmnEqual = FALSE;

      isPlmnEqual = wrUtlPlmnsEqual(&(gummei->plmnId), 
            &(mmeCb->ovrldGummeiLst[ovrldCnt].plmnId));

      if((TRUE == isPlmnEqual) &&
            (mmeCb->ovrldGummeiLst[ovrldCnt].mmeGroupId == 
             gummei->mmeGrpId) &&
            (mmeCb->ovrldGummeiLst[ovrldCnt].mmeCode == 
             gummei->mmec))
      {
         gummeiOvrld = TRUE;
         break;
      }
   }

   RETVALUE(gummeiOvrld);
}

/** @brief This function is called to select MME either based on
 *         GUMMEI Info received from UE or based on primary PLMN.
 *
 * @details
 *     Function: wrUmmRrcConSelectMme   
 *
 *         Processing steps:
 *         - Selection of MME based on GUMMEI info received from
 *           UE.
 *         - Selection of MME based on primary PLMN Index and primary
 *           PLMN.
 *
 *
 * @param WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */
PUBLIC S16 wrEmmSelectMmeOnGummei
(
WrMmeId                      *mmeId,
WrGummei                     *gummei,
U32                           establishmentCause
)
{
  S16                       ret = RFAILED;
  CmLListCp                 *cmListmmeIdList = NULLP;

  ret =  wrEmmS1FlexgetMmeCbList(&cmListmmeIdList, gummei); 

  if(ROK == ret)
  {
     /* Get the MME CB which is not down */
     ret = wrEmmS1FlexGetMmeCbNotDown(cmListmmeIdList, mmeId, gummei, 
                                      establishmentCause);
  }
  
  RETVALUE(ret);
}

/** @brief This function is called to select MME based on
 *         GUMMEI Info received from UE. If that MME is alive we select it or else we select some other MME.
 *
 * @details
 *     Function:wrEmmS1FlexGetMmeCbNotDown 
 *
 *         Processing steps:
 *         - Selection of MME based on GUMMEI info received from
 *           UE.
 *        
 *
 * @param  CmLListCp *cmListmmeIdList,  U8 *mme 
 * @return ROK/RFAILED
 */


PRIVATE S16 wrEmmS1FlexGetMmeCbNotDown
(
CmLListCp                    *cmListmmeIdList,
WrMmeId                      *mme,
WrGummei                     *gummei,
U32                           establishCause
)
{
  S16                       ret  = RFAILED;
  U8                        count =0;
  CmLList                   *list = NULLP;
  WrMmeCb                   *ptrMmeCb = NULLP;
  WrGummeiMmeInfo           *ptrMmeInfo = NULLP;
  U32                        gummeiOvrld;

  CM_LLIST_FIRST_NODE(cmListmmeIdList, list);
  for (count = 0; count < cmListmmeIdList->count; count++)
  {
     gummeiOvrld = FALSE;
     ptrMmeInfo =(WrGummeiMmeInfo*)list->node;
     ptrMmeCb = ptrMmeInfo->mmePtr;
     if ((WR_MME_UP == ptrMmeCb->state )) 
     {
        /* Check if the GUMMEI selected by MME is overloaded and skip the same
         * if it is overloaded
         */
        gummeiOvrld = wrEmmGetGummeiOvldStatus(gummei, ptrMmeCb->mmeId);

        if ((TRUE == ptrMmeCb->mmeOvldFlag) || (TRUE == gummeiOvrld))
        {
           if(wrGetMmeRejectStatus(establishCause,  ptrMmeCb->mmeId))
           {
              continue;
           }
        }
        *mme = ptrMmeCb->mmeId;
        ret = ROK;
        break;
     }
     CM_LLIST_NEXT_NODE(cmListmmeIdList,list);
  }
  RETVALUE(ret);
}

 /** @brief This function is called to select MMECB list based on the GUMMEI  
 *        if not found sends back the RFAILED 
 *
 * @details
 *     Function:  wrEmmS1FlexGetMmeCbListFrmGuGrup 
 *
 *         Processing steps:
 *         - Selection of MMECB list  based on GU Group ID info received from
 *           UE.
 *        
 *
 * @param  CmLListCp **cmListmmeIdList, WrGuGrpIdLst *Gummei 
 * @return ROK/RFAILED
 */

PRIVATE S16 wrEmmS1FlexGetMmeCbListFrmGuGrup(CmLListCp **cmListmmeIdList, WrGuGrpIdLst *guGrupId)
{
  
  S16 ret = RFAILED;
  U8 guGrupIdArr[8] = {0};
  WrGuGrpIdLst tempGuGrupId = *guGrupId;

  WrGuGrupCbList *ptrGuGrupCbLst = NULLP; 
  WR_EMM_GET_GUGRUP_ARR(tempGuGrupId, guGrupIdArr);
  ret = cmHashListFind(&(wrEmmCb.guGrupIdLst),
         (U8 *)guGrupIdArr,GU_GRUP_KEY_LEN,0,(PTR *)&ptrGuGrupCbLst);
  if(ROK == ret)
  {
    *cmListmmeIdList = &ptrGuGrupCbLst->mmeList;
  }
  RETVALUE(ret);
}

/** @brief This function is called to select MMECB list based on the GUMMEI  
 *        if not found sends back the RFAILED 
 *
 * @details
 *     Function: wrEmmS1FlexgetMmeCbList  
 *
 *         Processing steps:
 *         - Selection of MMECB list  based on GUMMEI info received from
 *           UE.
 *        
 *
 * @param  CmLListCp **cmListmmeIdList, WrGummei *Gummei 
 * @return ROK/RFAILED
 */

PRIVATE S16 wrEmmS1FlexgetMmeCbList(CmLListCp **cmListmmeIdList, WrGummei *gummei)
{
  
   S16 ret = RFAILED;
   U8 gummeiId[CB_KEY_LEN];
   gummeiId[0] = gummei->plmnId.mcc[0]; 
   gummeiId[1] = gummei->plmnId.mcc[1]; 
   gummeiId[2] = gummei->plmnId.mcc[2];
   gummeiId[3] = gummei->plmnId.mnc[0]; 
   gummeiId[4] = gummei->plmnId.mnc[1];
   gummeiId[5] = 0;
   if(gummei->plmnId.numMncDigits == 3)
   { 
     gummeiId[5] = gummei->plmnId.mnc[2];
   }
   gummeiId[6] = gummei->mmeGrpId >> 8; 
   gummeiId[7] = gummei->mmeGrpId &0xff;
   gummeiId[8] = gummei->mmec; 


  U32 size = CB_KEY_LEN;
  WrGummeicbList *ptrGummeiCbList = NULLP; 
  ret = cmHashListFind(&(wrEmmCb.GummeiMmeCbs),
         (U8 *)gummeiId,size,0,(PTR *)&ptrGummeiCbList);
  if(ROK == ret)
  {
    *cmListmmeIdList = &ptrGummeiCbList->mmeIdList;
  }
  RETVALUE(ret);
}

/** @brief This function is called to initialise the mmeCb S1AP parameter for MMECB
 *        
 *
 * @details
 *     Function: wrEmmS1FlexInitS1FlexVarFrMme 
 *
 *         Processing steps:
 *         - 
 *        
 *
 * @param WrMmeCb* mmeCb 
 * @return ROK/RFAILED
 */

PUBLIC S16 wrEmmS1FlexInitS1FlexVarFrMme(WrMmeCb* mmeCb)
{
   S16 ret = ROK;
   mmeCb->numPlmnIds = 0; 
   mmeCb->numGrpIds = 0; 
   mmeCb->numCodes = 0; 
   mmeCb->mmeOvldFlag = 0;
   mmeCb->relCapacity = 0;
   mmeCb->overloadAction = 0;
   RETVALUE(ret);
}

PUBLIC S16  wrEmmS1FlexInitS1FlexVarForEmmCb(Void)
{
   S16 ret = ROK;

   ret = cmHashListInit(&wrEmmCb.GummeiMmeCbs, WR_UMM_MAX_MME_CB_LIST/*nmbBins*/,
                           0 /*offset*/, FALSE, 
                           CM_HASH_KEYTYPE_ANY,
                           wrEmmCb.init.region, wrEmmCb.init.pool);
    ret = cmHashListInit(&wrEmmCb.guGrupIdLst, WR_MAX_GU_GRP_IDS/*nmbBins*/,
                           0 /*offset*/, FALSE, 
                           CM_HASH_KEYTYPE_ANY,
                           wrEmmCb.init.region, wrEmmCb.init.pool); 
   RETVALUE(ret);
}

 /** @brief This function is called to prepare GU Grup ID lst
 *        
 *
 * @details
 *     Function: wrEmmS1FlexAddGuGrupIds
 *
 *         Processing steps:
 *         Takes Gu Grup Id list to be added
 *         compares this list with existing list
 *         if there is no match fund add the new entry
 *        
 *
 * @param[in]   count  :number of GU grup ID to add
 * @param[in]   guGrpIdLstToAdd  :GU grup ID list
 * @return ROK/RFAILED
 */
 
PUBLIC S16  wrEmmS1FlexAddGuGrupIds
(
U8 count, 
WrGuGrpIdLst *guGrpIdLstToAdd,
WrMmeCb * mmeCb
)
{
   U8 addIdx = 0;
   S16 ret = RFAILED;
   U8 idx = count;
   U8 index1;
   U8 index3;
   U8 numGuGrpIds;
   Bool findFlag = FALSE;
   CmLListCp    *mmeIdList = NULLP;
   WrGuGrupMmeInfo* guGrupInfo;
   /*Taking first cell*/
   WrEmmEnbCfgUpd*             cellInfo = &wrEmmCb.cellInfo;
   cellInfo->numGrpIdAdded = 0;

   for(addIdx = 0; addIdx<idx; addIdx++)
   {
      /* Gummei is prepared. Put it into the GUMMEI CB LIST */
      ret =  wrEmmS1FlexGetMmeCbListFrmGuGrup(&mmeIdList, &guGrpIdLstToAdd[addIdx]);

      if(RFAILED == ret)
      {
         ret = wrEmmS1FlexGetFreeGuGrupCbLstnUpd(&guGrpIdLstToAdd[addIdx], mmeCb);
         if(RFAILED == ret)
         {
            RLOG0(L_ERROR, " wrEmmS1FlexGetFreeGuGrupCbLstnUpd failed to add GUGrups ");
            RETVALUE(RFAILED);
         }
         /*GU Grup ID is newly added need to send eNb config Update*/
         if( 0 != cellInfo->numGrpIdDeleted)
         {
            numGuGrpIds = cellInfo->numGrpIdDeleted;
            for (index1 =0; index1 < numGuGrpIds; index1++)
            {
               /*Comparing the GU Group ID to check if the Delete List
                * has the sam entry as that of hte new GU grup IDs which
                * we are adding Hence this check is done to 
                * remove duplicate entries*/
               if (cmMemcmp((U8 *)&cellInfo->guIdDelLst[index1],
                        (U8 *)&guGrpIdLstToAdd[addIdx],
                        sizeof(WrGuGrpIdLst)) == FALSE)
               {
                  findFlag = TRUE;
                  break;
               }
            }
            if (findFlag == TRUE)
            { 
               findFlag = FALSE;
               for (index3 =index1; index3<(numGuGrpIds-1); index3++)
               {
                 cellInfo->guIdDelLst[index3]=  cellInfo->guIdDelLst[index3+1];
               }
               /*Decrementing the GU Group ID to be deleted count*/
               cellInfo->numGrpIdDeleted--;   
               /*go to next Gu grup ID*/ 
               continue; 
            }
            
         }
         cmMemcpy((U8*)&cellInfo->guIdAddLst[cellInfo->numGrpIdAdded++],
               (U8*)&guGrpIdLstToAdd[addIdx], sizeof(WrGuGrpIdLst));

      }
      else
      {
         /* if found insert into the linked list */
         WR_ALLOC(&guGrupInfo, sizeof(WrGuGrupMmeInfo));
         if(NULLP == guGrupInfo)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         guGrupInfo->mmePtr = mmeCb;
         guGrupInfo->lnk.node = (PTR)guGrupInfo;
         cmLListAdd2Tail(mmeIdList, &guGrupInfo->lnk); 
      }
   }

   RETVALUE(ROK);
}
/** @brief This function is called to insert the MMECB in GUMMEI list. based on the GUMMEI generated
 *        
 *
 * @details
 *     Function: wrEmmS1FlexInsertGummeiInMmeCb
 *
 *         Processing steps:
 *         - Creates the GUMMEI, checks if gummei CB list is already present if already present, add the MMECB to tail 
*            other wise allocates the index and inserts the MMECB to the tail of that index list 
 *        
 *
 * @param WrMmeCb* mmeCb, SztSrvdGUMMEIs* gummeis
 * @return ROK/RFAILED
 */

PUBLIC S16 wrEmmS1FlexInsertGummeiInMmeCb(WrMmeCb* mmeCb, SztSrvdGUMMEIs* gummeis)
{
   S16 ret = RFAILED;
   SztSrvdGUMMEIsItem          *gummei  = gummeis->member;
   SztSrvdPLMNs                *plmns   = &gummei->servedPLMNs;
   U32                         noPlmns = plmns->noComp.val;
   SztSrvdGroupIDs             *grpIds = &gummei->servedGroupIDs;
   SztSrvdMMECs                *mmeCs = &gummei->servedMMECs;
   U32                         noGrpIds;
   U32                         noMmeCs;
   CmLListCp                   *mmeIdList = NULLP;
   WrGummeiMmeInfo             *gummeInfoPtr = NULLP;
   WrPlmnCb                    *plmn = NULLP;
   WrGuGrpIdLst              guGrpIdLstToAdd[WR_MAX_GU_GRP_IDS];
   U8                        idx = 0;

   noGrpIds = grpIds->noComp.val;
   noMmeCs = mmeCs->noComp.val;
   U32 plmnIdx, grpIdx, mmeCodeIdx; 
   for (plmnIdx  = 0; plmnIdx < noPlmns; plmnIdx++)
   {
      WrGummei localGummei;
      localGummei.pres = TRUE;
      localGummei.plmn_identity_pres = TRUE;
      plmn = NULLP;
      wrEmmMmeParsePlmnIe(plmns->member + plmnIdx, &localGummei.plmnId);

      /* Locate the PLMN related information from the cell control block     */
      plmn = wrEmmPlmnGetPlmnCb(&localGummei.plmnId);
      if (plmn == NULLP)
      {
         RLOG0(L_ERROR, " Unknown PLMN Id ");
         continue;
      }
      for(grpIdx = 0; grpIdx < noGrpIds; grpIdx++)
      {
        localGummei.mmeGrpId = (((grpIds->member + grpIdx)->val[1]) | 
                              ((grpIds->member + grpIdx)->val[0] << 8));
        if( WR_MAX_GU_GRP_IDS > idx )
        {
           /*Selecting first 16 GU groups only*/
           guGrpIdLstToAdd[idx].mmeGrpId = localGummei.mmeGrpId;
           guGrpIdLstToAdd[idx].plmnId = localGummei.plmnId;
           idx++;
        }
        for(mmeCodeIdx = 0; mmeCodeIdx < noMmeCs; mmeCodeIdx++)
        {
           localGummei.mmec = (mmeCs->member + mmeCodeIdx)->val[0];

           /* Gummei is prepared. Put it into the GUMMEI CB LIST */
           ret = wrEmmS1FlexgetMmeCbList(&mmeIdList, &localGummei);

           if(RFAILED == ret)
           {
              ret =  wrEmmS1FlexgetFreeGummeiCbLstnUpd(&localGummei, mmeCb);
           }
           else
           {
              /* if found insert into the linked list */
              WR_ALLOC(&gummeInfoPtr, sizeof(WrGummeiMmeInfo));
              if(NULLP == gummeInfoPtr)
              {
                 RLOG0(L_FATAL, "Memory allocation failed.");
                 RETVALUE(RFAILED);
              }
              gummeInfoPtr->mmePtr = (WrMmeCb *)mmeCb;
              gummeInfoPtr->isLoaded = FALSE;
              gummeInfoPtr->lnk.node = (PTR)gummeInfoPtr;
              cmLListAdd2Tail(mmeIdList, &gummeInfoPtr->lnk); 
           }
        }
      }
   }
   wrEmmS1FlexAddGuGrupIds(idx, guGrpIdLstToAdd, mmeCb);
   
   RETVALUE(ret);
}

 /** @brief This function is called to delete the MMECB in all the GU Grup ID list. 
  * called during the MME configuration update messag, S1 AP message 
 *        
 *
 * @details
 *     Function:  wrEmmS1FlexDelMmeCbEntryFrmGuGrupCbLst
 *
 *         Processing steps:
 *         - goes through whole list of GU Grup CB and 
 *         if it is valid and MMECB matches the one passed delete the entry.
 *        
 *
 * @param WrMmeCb* mmeCb
 * @return ROK/RFAILED
 */

PUBLIC S16  wrEmmS1FlexDelMmeCbEntryFrmGuGrupCbLst(WrMmeCb* mmeCb)
{

   S16                       ret = ROK;
   Bool                      isFreeMmeInfo = FALSE;    
   CmLListCp                 *mmeIdList = NULLP;
   CmLList*                   link = NULLP;
   WrGuGrupMmeInfo           *mmeInfo = NULLP;              
   WrGuGrupCbList            *guGrupCbList = NULLP;
   PTR                       prevEnt = NULLP;
   WrEmmEnbCfgUpd*             cellInfo = &wrEmmCb.cellInfo;
   cellInfo->numGrpIdDeleted = 0;
   while((ret = cmHashListGetNext(&(wrEmmCb.guGrupIdLst), prevEnt, (PTR *) &guGrupCbList)) == ROK)
   {
         /* Check in the list, If not present, Then add it */
         mmeIdList = &guGrupCbList->mmeList;
         CM_LLIST_FIRST_NODE(mmeIdList, link);
         while (NULLP != link)
         {
            mmeInfo = (WrGuGrupMmeInfo*)link->node;
            if(NULLP ==  mmeInfo->mmePtr)
            {
              RLOG0(L_ERROR, " mmeInfo->mmePtr is NULL ");
              continue;
            }
            if((WrMmeCb *) mmeInfo->mmePtr == (WrMmeCb *)mmeCb)
            {
               cmLListDelFrm(mmeIdList, link);
               WR_FREE(mmeInfo, sizeof(WrGuGrupMmeInfo));
               if(0 == mmeIdList->count)
               {
                  isFreeMmeInfo = TRUE;
                  cmHashListDelete(&wrEmmCb.guGrupIdLst, (PTR )guGrupCbList);
        /*GU Grup ID is deleted need to send eNb config Update*/
                  cmMemcpy((U8*)&cellInfo->guIdDelLst[cellInfo->numGrpIdDeleted++],
                     (U8*)&guGrupCbList->grupId, \
                     sizeof(WrGuGrpIdLst));

                  WR_FREE(guGrupCbList, sizeof(WrGuGrupCbList));
                  wrEmmCb.numGuGrpIds--; 
               }
               break;
            }
            CM_LLIST_NEXT_NODE(mmeIdList,link);
         }
         if(TRUE == isFreeMmeInfo)
         {
           prevEnt = (PTR) NULLP;
           isFreeMmeInfo = FALSE;
         }
         else
         {
           prevEnt = (PTR) guGrupCbList;
         }

   }
   RETVALUE(ret);
}

/** @brief This function is called to delete the MMECB in all the GUMMEI list. called during the MME configuration 
 *     update messag, S1 AP message 
 *        
 *
 * @details
 *     Function:  wrEmmS1FlexdelMmeCbEntryFrmGummeiCbLst
 *
 *         Processing steps:
 *         - goes through whole list of GUMMEI CB and if it is valid and MMECB matches the one passed delete the entry.
 *        
 *
 * @param WrMmeCb* mmeCb
 * @return ROK/RFAILED
 */

PUBLIC S16 wrEmmS1FlexdelMmeCbEntryFrmGummeiCbLst(WrMmeCb* mmeCb)
{

   S16                                 ret = ROK;
   Bool                        isFreeCbInfo = FALSE;    
   CmLListCp                    *mmeIdList = NULLP;
   CmLList                          *link  = NULLP;
   WrGummeiMmeInfo                *mmeInfo = NULLP;              
   WrGummeicbList                *gummeiCbList = NULLP;
   PTR                       prevEntry = NULLP;
   TRC2(wrEmmS1FlexdelMmeCbEntryFrmGummeiCbLst);
   while((ret = cmHashListGetNext(&(wrEmmCb.GummeiMmeCbs), prevEntry, (PTR *) &gummeiCbList)) == ROK)
   {
         /* Check in the list, If not present, Then add it */
         mmeIdList = &gummeiCbList->mmeIdList;
         CM_LLIST_FIRST_NODE(mmeIdList, link);
         while (NULLP != link)
         {
            mmeInfo = (WrGummeiMmeInfo*)link->node;
            if(NULLP ==  mmeInfo->mmePtr)
            {
              RLOG0(L_ERROR, "  mmeInfo->mmePtr is NULL ");
              continue;
            }
            if((WrMmeCb *) mmeInfo->mmePtr == (WrMmeCb *)mmeCb)
            {
               cmLListDelFrm(mmeIdList, link);
               WR_FREE(mmeInfo , sizeof(WrGummeiMmeInfo));
               if(0 == mmeIdList->count)
               {
                  isFreeCbInfo = TRUE;
                  cmHashListDelete(&wrEmmCb.GummeiMmeCbs, (PTR )gummeiCbList);
                  WR_FREE(gummeiCbList, sizeof(WrGummeicbList));
               }
               break;
            }
            CM_LLIST_NEXT_NODE(mmeIdList,link);
         }
         if(TRUE == isFreeCbInfo)
         {
           prevEntry = (PTR) NULLP;
           isFreeCbInfo = FALSE;
         }
         else
         {
           prevEntry = (PTR) gummeiCbList;
         }
   }
   /*Update the GU Grup ID List*/
   wrEmmS1FlexDelMmeCbEntryFrmGuGrupCbLst(mmeCb);
   RETVALUE(ret);
}

/** @brief This function is called to update the MMECB in the GUMMEI list. called during the MME configuration 
 *     update messag, S1 AP message 
 *        
 *
 * @details
 *     Function: wrEmmS1FlexUpdateGummeiInMmeCb 
 *
 *         Processing steps:
 *         -  First delete the MMECB in whole list. Then insert the MMECBs in the GUMMEI list
 *        
 *
 * @param WrMmeCb* mmeCb, SztSrvdGUMMEIs* gummeis 
 * @return ROK/RFAILED
 */

PUBLIC S16 wrEmmS1FlexUpdateGummeiInMmeCb(WrMmeCb* mmeCb, SztSrvdGUMMEIs* gummeis)
{
   S16 ret = RFAILED;
   TRC2(wrEmmS1FlexUpdateGummeiInMmeCb);

   ret =  wrEmmS1FlexdelMmeCbEntryFrmGummeiCbLst(mmeCb);
   ret = wrEmmS1FlexInsertGummeiInMmeCb(mmeCb, gummeis);
   

   RETVALUE(ret);
}
 
/** @brief This function is used to build 
 *  handover restriction list IE by using 
 *  the information store in UE control Block
 *
 * @details
 *
 *     Function: wrEmmBuildHoRestrnLst
 *
 *         Processing steps:
 *
 * @param[in]    ueCb   : Ue control Block to copy Restriction info
 * @param[in]    hoReq   : Handover Req event
 * @param[in]    hoRestrnLst : HO Restriction list received from Peer
 * @return S16 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */ 
PUBLIC S16 wrEmmBuildHoRestrnLst
(
WrUeCb                *ueCb,
CztEvnt               *hoReq,
CztHovrRestrnLst      *hoRestrnLst   
)
{
   U8 i = 0;
   U8 j = 0;
   WrHoRstrctInfo *hoRstrctInfo;
   
   hoRstrctInfo = ueCb->hoRstrctInfo;
   TRC2(wrEmmBuildHoRestrnLst);
   wrFillTknU8(&hoRestrnLst->pres, PRSNT_NODEF);
   
   /*Filling Served PLMN*/
   wrEmmCztFillPLMNId(hoReq, &hoRstrctInfo->srvngPlmn, \
         &hoRestrnLst->servingPLMN);

   if(0 < hoRstrctInfo->noOfEplmn)
   {
      wrFillTknU16(&hoRestrnLst->equivalentPLMNs.noComp, hoRstrctInfo->noOfEplmn);
   }
   /*Filling Equivalent PLMN List*/
   WR_GET_MEM(&hoReq->memCp,
         (sizeof(CztPLMN_Identity)*hoRstrctInfo->noOfEplmn),
         (Ptr*)&hoRestrnLst->equivalentPLMNs.member);

   for(i = 0;i < hoRstrctInfo->noOfEplmn;i++)
   {
      wrEmmCztFillPLMNId(hoReq,&hoRstrctInfo->ePlmn[i],
            &hoRestrnLst->equivalentPLMNs.member[i]);

   }
   /*Filling Forbidden TAs*/
   if(0 < hoRstrctInfo->noOfTAs)
   {
      wrFillTknU16(&hoRestrnLst->forbiddenTAs.noComp, hoRstrctInfo->noOfTAs);
      WR_GET_MEM(&hoReq->memCp,
         (sizeof(CztForbiddenTAs_Item)*hoRstrctInfo->noOfTAs),
         (Ptr*)&hoRestrnLst->forbiddenTAs.member);
  
      for(i = 0;i < hoRstrctInfo->noOfTAs;i++)
      {
        CztForbiddenTAs_Item* forbiddenTA =  
          &hoRestrnLst->forbiddenTAs.member[i];
        WrForbiddenTAs * wrForbiddenTa = &hoRstrctInfo->frbdnTAs[i];

        wrFillTknU8(&forbiddenTA->pres, PRSNT_NODEF);
        wrEmmCztFillPLMNId(hoReq, &wrForbiddenTa->plmnId,
            &forbiddenTA->pLMN_Identity);
        /*Filling Forbidden TACs*/
        wrFillTknU16(&forbiddenTA->forbiddenTACs.noComp,  wrForbiddenTa->noOfTac);
        WR_GET_MEM(&hoReq->memCp,
          (sizeof(CztTAC)*wrForbiddenTa->noOfTac),
          (Ptr*)&forbiddenTA->forbiddenTACs.member);

        for(j = 0; j < wrForbiddenTa->noOfTac; j++)
        {
          SztTAC *szTac = &forbiddenTA->forbiddenTACs.member[j];
          wrFillTknStr4(szTac,2, wrForbiddenTa->tac[j]);
        }
      }
   }
   /*Filling Forbidden LAs*/
   if(0 < hoRstrctInfo->noOfLAs)
   {
      wrFillTknU16(&hoRestrnLst->forbiddenLAs.noComp, hoRstrctInfo->noOfLAs);
      WR_GET_MEM(&hoReq->memCp,
         (sizeof(CztForbiddenLAs_Item)*hoRstrctInfo->noOfLAs),
         (Ptr*)&hoRestrnLst->forbiddenLAs.member);
  
      for(i = 0;i < hoRstrctInfo->noOfLAs;i++)
      {
         CztForbiddenLAs_Item* forbiddenLA =  
           &hoRestrnLst->forbiddenLAs.member[i];
         WrForbiddenLAs * wrForbiddenLa = &hoRstrctInfo->frbdnLAs[i];

         wrFillTknU8(&forbiddenLA->pres, PRSNT_NODEF);
         wrEmmCztFillPLMNId(hoReq, &wrForbiddenLa->plmnId,
             &forbiddenLA->pLMN_Identity);
         /*Filling Forbidden LACs*/

         wrFillTknU16(&forbiddenLA->forbiddenLACs.noComp,  wrForbiddenLa->noOfLac);
         WR_GET_MEM(&hoReq->memCp,
           (sizeof(CztLAC)*wrForbiddenLa->noOfLac),
           (Ptr*)&forbiddenLA->forbiddenLACs.member);

         for(j = 0; j < wrForbiddenLa->noOfLac; j++)
         {
            CztLAC *szLac = &forbiddenLA->forbiddenLACs.member[j];
            wrFillTknStr4(szLac,2,wrForbiddenLa->lac[j]);
         }
      }
   }
   if(PRSNT_NODEF == hoRstrctInfo->frbdnInterRATs.pres)
   { 
     wrFillTknU32(&hoRestrnLst->forbiddenInterRATs,hoRstrctInfo->frbdnInterRATs.val);
   }
   RETVALUE(ROK);  
}

  
 /** @brief This function is called to allocate the free index in GUMMEI CB list if found insert to the tail. 
 *     
 *        
 *
 * @details
 *     Function: wrEmmS1FlexGetFreeGuGrupCbLstnUpd 
 *
 *         Processing steps: allocate the free index in GUMMEI CB list if found insert to the tail, else return RFAILED
 *         -  
 *        
 *
 * @param WrGummei *gummei, WrMmeCb* mmeCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrEmmS1FlexGetFreeGuGrupCbLstnUpd(WrGuGrpIdLst *guGrupId, WrMmeCb* mmeCb)
{
   S16 ret = RFAILED;
   WrGuGrupCbList *guGrupCbPtr = NULLP;
   WrGuGrupMmeInfo *guGrupInfoPtr = NULLP;
   TRC2(wrEmmS1FlexGetFreeGuGrupCbLstnUpd);

   /*If the num Grups in eNodeB exceeds 16, Cant add more Grups to this list*/
   if(WR_MAX_GU_GRP_IDS <= wrEmmCb.numGuGrpIds)
   { 
      RLOG0(L_ERROR, "  reached MAX grup IDs ");
      RETVALUE(ret);
   }
   WR_ALLOC(&guGrupCbPtr, sizeof(WrGuGrupCbList));
   if(NULLP == guGrupCbPtr)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ret);
   }
   cmMemcpy((U8*)&guGrupCbPtr->grupId, 
                (U8*)guGrupId, sizeof(WrGuGrpIdLst));
   WR_EMM_GET_GUGRUP_ARR(guGrupCbPtr->grupId, guGrupCbPtr->guGrupIdArr);
   ret = cmHashListInsert(&(wrEmmCb.guGrupIdLst), (PTR)guGrupCbPtr,
                                 (U8 *)guGrupCbPtr->guGrupIdArr,
                                 GU_GRUP_KEY_LEN);
   cmLListInit(&guGrupCbPtr->mmeList);
   /* if found insert into the linked list */
   WR_ALLOC(&guGrupInfoPtr, sizeof(WrGuGrupMmeInfo));
   if(NULLP == guGrupInfoPtr)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE(guGrupCbPtr, sizeof(WrGuGrupCbList));
      RETVALUE(RFAILED);
   }
   guGrupInfoPtr->mmePtr = (WrMmeCb*) mmeCb;
   guGrupInfoPtr->lnk.node = (PTR)guGrupInfoPtr;
   cmLListAdd2Tail(&guGrupCbPtr->mmeList, &guGrupInfoPtr->lnk);
   wrEmmCb.numGuGrpIds++; 
   RETVALUE(ret);
}
 

/** @brief This function is called to allocate the free index in GUMMEI CB list if found insert to the tail. 
 *     
 *        
 *
 * @details
 *     Function:wrEmmS1FlexgetFreeGummeiCbLstnUpd 
 *
 *         Processing steps: allocate the free index in GUMMEI CB list if found insert to the tail, else return RFAILED
 *         -  
 *        
 *
 * @param WrGummei *gummei, WrMmeCb* mmeCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrEmmS1FlexgetFreeGummeiCbLstnUpd(WrGummei *gummei, WrMmeCb* mmeCb)
{
   S16 ret = RFAILED;
   WrGummeicbList  *gummeiCbPtr = NULLP;
   WrGummeiMmeInfo *gummeInfoPtr = NULLP;
   TRC2(wrEmmS1FlexgetFreeGummeiCbLstnUpd);
   WR_ALLOC(&gummeiCbPtr, sizeof(WrGummeicbList));
   if(NULLP == gummeiCbPtr)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   gummeiCbPtr->gummeiId[0] = gummei->plmnId.mcc[0]; 
   gummeiCbPtr->gummeiId[1] = gummei->plmnId.mcc[1]; 
   gummeiCbPtr->gummeiId[2] = gummei->plmnId.mcc[2];
   gummeiCbPtr->gummeiId[3] = gummei->plmnId.mnc[0]; 
   gummeiCbPtr->gummeiId[4] = gummei->plmnId.mnc[1]; 
   gummeiCbPtr->gummeiId[5] = 0;
   if ( gummei->plmnId.numMncDigits == 3)
   {
      gummeiCbPtr->gummeiId[5] = gummei->plmnId.mnc[2];
   }
   gummeiCbPtr->gummeiId[6] = gummei->mmeGrpId >> 8; 
   gummeiCbPtr->gummeiId[7] = gummei->mmeGrpId &0xff;
   gummeiCbPtr->gummeiId[8] = gummei->mmec; 

   ret = cmHashListInsert(&(wrEmmCb.GummeiMmeCbs), (PTR)gummeiCbPtr,
                                 (U8 *)gummeiCbPtr->gummeiId,
                                 CB_KEY_LEN);
   cmLListInit(&gummeiCbPtr->mmeIdList);
   /* if found insert into the linked list */
   WR_ALLOC(&gummeInfoPtr, sizeof(WrGummeiMmeInfo));
   if(NULLP == gummeInfoPtr)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE(gummeiCbPtr, sizeof(WrGummeicbList));
      RETVALUE(RFAILED);
   }
   gummeInfoPtr->mmePtr = (WrMmeCb *) mmeCb;
   gummeInfoPtr->isLoaded = FALSE;
   gummeInfoPtr->lnk.node = (PTR)gummeInfoPtr;
   cmLListAdd2Tail(&gummeiCbPtr->mmeIdList, &gummeInfoPtr->lnk);
    
   RETVALUE(ret);
}

/** @brief This function is called to update the oveload action for the mmeCb and send an indiacation to RRM..
 *     update messag, S1 AP message 
 *        
 *
 * @details
 *     Function:wrEmmS1FlexHdlOverloadResponse 
 *
 *         Processing steps:
 *         -  store the overload response message and send indication to RRM.
 *        
 *
 * @param WrMmeCb* mmeCb, U32 overloadAction
 * @return ROK/RFAILED
 */

PUBLIC S16  wrEmmS1FlexHdlOverloadResponse(WrMmeCb* mmeCb, U32 overloadAction)
{
   S16 ret = ROK;
   RmuMmeOvldStartInd *rrmWrEnbStaInd = NULLP;
   RmuCommonMsg       *rmuCommonMsg = NULLP;
   TRC2(wrEmmS1FlexHdlOverloadResponse);
   
   mmeCb->mmeOvldFlag = TRUE;
   /* Here MME overload starts, thus we need to indicate the mme overload
   * action to RRM for access class baring using Enb status indication 
   */
   WR_ALLOC ( &rmuCommonMsg, sizeof(RmuCommonMsg));
   if (rmuCommonMsg== NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   rrmWrEnbStaInd = (RmuMmeOvldStartInd *)rmuCommonMsg; 
   rrmWrEnbStaInd->usMmeId = mmeCb->mmeId;

   /* MME overload action comes from S1AP interface (i.e. SZT interface),
    * thus we need to keep FSM-RRM Interface enum (RrmWrMmeOvldStatus) in
    * sync with SZT Interface
   */
   rrmWrEnbStaInd->enMmeOvldStatus = 
     (RmuMmeOvldStatus)(mmeCb->overloadAction);

   if (ROK != wrIfmRrmMmeOvldStartInd(rrmWrEnbStaInd))
   {
      RLOG2(L_ERROR, " ENB status ind Failed. mmeId = [%d] mmeOvldStatus = [%d] ",
            rrmWrEnbStaInd->usMmeId, rrmWrEnbStaInd->enMmeOvldStatus);
      WR_FREE(rmuCommonMsg,  sizeof(RmuCommonMsg));
      ret = RFAILED;
   }
   else
   {
      RLOG2(L_DEBUG, " ENB status indication Success. mmeId = [%d],  mmeOvldStatus = [%d] ",
            rrmWrEnbStaInd->usMmeId, rrmWrEnbStaInd->enMmeOvldStatus);
   }
   RETVALUE(ret);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
/********************************************************************30**

  End of file:     fc_emm_s1_flex.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:51:46 2014

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
/main/1        ---      Aravind               1. initial release TotaleNodeB 1.1
*********************************************************************91*/

