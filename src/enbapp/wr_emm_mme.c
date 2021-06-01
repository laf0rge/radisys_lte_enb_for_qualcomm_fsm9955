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
               

     File:     wr_emm_mme.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=9;

#include <stdlib.h>
#include "wr.h"
#include "wr_umm.h"
#include "wr_utils.h"
#include "wr_emm.h"
#include "wr_emm_mme.h"
#include "wr_emm_cfg.h"
#include "wr_emm_pbm.h"
#include "wr_ifm_s1ap.h"
#include "wr_ifm_rrm.h"
#include "wr_kpi.h"
#include "wr_umm_rrccon.h"
#include "wr_emm_cnm.h"
#include "wr_emm_rim_app.h"

#ifdef WR_TEST_CODE
EXTERN U8 wrTgrMmeCfgUpdFail;
EXTERN U8 wrTgrEnbErrInd;
EXTERN U8 wrDntRspMmeCfgUpd;
EXTERN U8 wrTgrS1SetupRtx;
EXTERN U8 wrTgrEnbCfgUpdRtx;
#endif

EXTERN S16 wrS1AlarmHandle (WrMmeCb *mmeCb, U16 alarmId);

#define WR_MME_CON_WAIT_TIMER 200

/* Default Timer value for S1-Setup which is set to highest value */
#define WR_EMM_S1_SETUP_DFLT_TMR_VAL        6000
#define WR_EMM_S1_SETUP_FAIL_RET_TMR_VAL        6000
#define WR_EMM_ENB_CFGUPD_RSP_DFLT_TMR_VAL  6000
#define WR_EMM_ENB_CFGUPD_FAIL_RET_TMR_VAL  6000
#define WR_EMM_ENB_CFGUPD_RTX_CNT    0xFF
#define WR_EMM_ENB_CFG_TRANSFER_RTX_CNT 0xFF
#define WR_EMM_RST_RSP_DFLT_TMR_VAL  6000
#define WR_EMM_RST_RTX_CNT    0xFF

#ifdef MME_LDBAL
PRIVATE S16 wrEmmGetNextMmeCbPrb(WrPlmnCbMmeList *mmeLst, WrRrcConTransCb  *rrccon);
#ifdef DG_SG
PRIVATE S16 wrEmmGrpIdGetMmeIdList(WrPlmnCbMmeList *wrEmmPlmnMmeLst, WrRrcConTransCb *rrccon);
#endif
#endif
PUBLIC S16 wrEmmS1FlexInsertGummeiInMmeCb(WrMmeCb* mmeCb, SztSrvdGUMMEIs* gummeis);
PUBLIC S16 wrEmmS1FlexUpdateGummeiInMmeCb(WrMmeCb* mmeCb, SztSrvdGUMMEIs* gummeis);
PUBLIC S16 wrEmmS1FlexdelMmeCbEntryFrmGummeiCbLst(WrMmeCb* mmeCb);
PRIVATE S16 wrEmmMmePrcOvldStart(U32 peerId, S1apPdu *pdu);
PRIVATE S16 wrEmmMmePrcOvldStop(U32  peerId, S1apPdu  *pdu);
PRIVATE S16 wrEmmMmePrcMmeCfgUpd( U32  peerId, S1apPdu  *pdu);
PUBLIC S16 wrEmmMmePrcRcvdPdu(U32 peerId, S1apPdu *pdu);
PUBLIC S16 wrEmmMmeEnbConfigTransfer(WrEmmMmeEnbCfgTrnsfr *trnsfrInfo,
                         U32 enbCfgTransTmrVal,WrNeighEnbCb *neighEnbCb);
PRIVATE S16 wrEmmMmePrcMMEConfigTfr( U32  peerId, S1apPdu  *pdu);
PUBLIC S16 wrGetMmeRejectStatus(U32 establishmentCause,U32 mmeId);
PUBLIC S16 wrEmmS1FlexHdlOverloadResponse(WrMmeCb* mmeCb, U32 overloadAction);

/** @brief This function is invoked to get the MME control block based on
 *         message received from MME over S1 interface.
 *
 * @details Get the MME control block based on peer Id.
 *
 *     Function: wrEmmMmeGetMmeByPeerId
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[in] peerId: MME Peer Id.
 * @return PTR 
 *        -# Success : MME control block pointer.
 *        -# Failure : NULLP 
 */
PUBLIC WrMmeCb *wrEmmMmeGetMmeByPeerId
(
U32                          peerId
)
{
   U8                        mmeCount = 0;

   if (peerId < WR_MAX_MMES)
   {
      for (mmeCount = 0; mmeCount < WR_MAX_MMES; mmeCount++)
      {
         if( (wrEmmCb.mmeCont.mmes[mmeCount] != NULLP) &&  
               (wrEmmCb.mmeCont.mmes[mmeCount]->peerId == peerId))
         {
            RETVALUE(wrEmmCb.mmeCont.mmes[mmeCount]);
         }
      }
   }
   RETVALUE(NULLP);
} /* end of wrEmmMmeGetMmeByPeerId */

/** @brief This function gets the MME Id for the given MME code
 *
 * @details
 *
 *     Function: wrEmmMmeGetMmeByMmec
 *
 *         Processing steps:
 *         - Traverse the list of suppported MMEs by EnodeB, If MME code is equl
 *         to the MME control block MME code. 
 *
 *
 * @param[in] mmeCode : MME Code for which MME ID needs to be get
 * @param[out] mmeId  : mmeId of the MMECB which have the equals mmeCode. 
 * @return  S16
 *        -# Success : ROK and MME Id on success 
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeGetMmeByMmec
(
U8                           mmeCode,
WrMmeId                      *mmeId
)
{
   WrMmeCb                   *mmeCb;
   U16                       idx;
   U16                       idx1;

   /*Fix for ccpu00125227 */ 
   /* Go through all the MMEs which are configured and compare the 
    * MME code with the one received */
   for(idx = 0; idx < WR_MAX_MMES; idx++)
   {
	   mmeCb = wrEmmCb.mmeCont.mmes[idx];
	   if (mmeCb == NULLP)
	   {
		   continue;
	   }
	   for(idx1 = 0; idx1 < mmeCb->numCodes; idx1++)
	   {
		   if(mmeCb->codes[idx1] == mmeCode)
		   {
			   /*Check the MME status, Relative Capacity and overload status of
			    * particluar MME */
			   if((mmeCb->state == WR_MME_UP) && 
                               (mmeCb->relCapacity > 0))
			   {
			       *mmeId = mmeCb->mmeId;
                                RETVALUE(ROK);
			   }
		   }
	   }
   }
    RLOG1(L_ERROR,"No MME found for MME CODE ID[%d]", mmeCode);

RETVALUE(RFAILED);
}

/** @brief This function gets the MME Id for the given MME code
 *
 * @details
 *
 *     Function: wrEmmMmeGetMmeByMmeGrpId
 *
 *         Processing steps:
 *         - Traverse the list of suppported MMEs by EnodeB, Find the less
 *         overlaoded MME in the given MME group ID
 *
 *
 * @param[in] mmeGrpId : MME group Id for which MME ID needs to be get
 * @param[out] mmeId  : mmeId of the MMECB which have the equals mmeCode. 
 * @return  S16
 *        -# Success : ROK and MME Id on success 
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeGetMmeByMmeGrpId
(
U32                          establishCause,
U16                          mmeGrpId,
WrMmeId                      *mmeId
)
{
   WrMmeCb                   *mmeCb;
   U16                       idx;
   U16                       idx1;
   U8                        relCap = 0;
   U8                        found = FALSE;
   U32                       ovrldCnt;
   U32                       isGrpIdOvrld;

   /* Find list of all MMEs for groupId and
    * select one MME from this list now */
   RLOG1(L_INFO,"MME selection will be done for MME Group Id[%d]",mmeGrpId);

   for(idx = 0; idx < WR_MAX_MMES; idx++)
   {
      mmeCb = wrEmmCb.mmeCont.mmes[idx];
      if (mmeCb == NULLP)
      {
         continue;
      }
      for(idx1 = 0; idx1 < mmeCb->numGrpIds; idx1++)
      {
         if(mmeCb->groupIds[idx1] == mmeGrpId)
         {
            /*Check teh MME status, Relative Capacity and overload status of
             * particluar MME */
            if ((mmeCb->state == WR_MME_UP) && (mmeCb->relCapacity > 0))
            {
               isGrpIdOvrld = FALSE;
               /* Check if the given mmeGroup ID is overloaded and if so, will
                * not be selecting the given MME
                */
               for(ovrldCnt = 0; ovrldCnt < mmeCb->numOvrldGummei; ovrldCnt++)
               {
                  if(mmeGrpId == mmeCb->ovrldGummeiLst[ovrldCnt].mmeGroupId)
                  {
                     isGrpIdOvrld = TRUE;
                     break;
                  }
               }
               if ((TRUE == mmeCb->mmeOvldFlag) || (TRUE == isGrpIdOvrld))
               {
                  if(wrGetMmeRejectStatus(establishCause,  mmeCb->mmeId))
                  {
                     break;
                  }
               }

               /*Select the Best MME which is less loaded in the MME group */
               if(relCap < mmeCb->relCapacity)
               {
                  relCap = mmeCb->relCapacity;
                  *mmeId = mmeCb->mmeId;
                  found = TRUE;
               }
            }
         }
      }
   }
   if(TRUE == found)
   {
      RETVALUE(ROK);
   }
   else
   {
      RLOG1(L_ERROR,"No MME found for MME group ID[%d]", mmeGrpId);
      RETVALUE(RFAILED);
   }
} /* end of wrEmmMmeGetMmeByMmeGrpId */

/** @brief This function gets the MME Id for the given MME code
 *
 * @details
 *
 *     Function: wrEmmMmeGetMmeByPlmn
 *
 *         Processing steps:
 *         - Traverse the list of connected MMEs by EnodeB,
 *         - if PLMN is equal to the plmn in MME control block, then
 *           - return the MME CB
 *         - else
 *           - return NULLP
 *
 *
 * @param[in] plmn : PLMN for which MME CB needs to be fetched
 * @return  S16
 *        -# mmeCb : MME Control Block
 *        -# NULLP : Failure
 */
PUBLIC WrMmeCb *wrEmmMmeGetMmeByPlmn
(
WrPlmnId                     *plmnId
)
{
   WrMmeCb                   *mmeCb;
   U16                       idx;
   U16                       plmnIdx;

   /* Go through all the MMEs which are configured and compare the 
    * MME code with the one received */
   for(idx = 0; idx < WR_MAX_MMES; idx++)
   {
      mmeCb = wrEmmCb.mmeCont.mmes[idx];
      if (mmeCb == NULLP)
      {
         continue;
      }
      for(plmnIdx = 0; plmnIdx < mmeCb->numPlmnIds; plmnIdx++)
      {
         if(wrUtlPlmnsEqual(plmnId, &mmeCb->plmnIds[plmnIdx]) == TRUE)
         {
            RETVALUE(mmeCb);
         }  
      }
   }
   RLOG0(L_ERROR,"Invalid PLMN Id");
   RLOG3(L_ERROR,"MCC [%d] [%d] [%d]",plmnId->mcc[0],plmnId->mcc[1],
         plmnId->mcc[2]);
   RLOG3(L_ERROR,"MNC [%d] [%d] [%d]",plmnId->mnc[0],plmnId->mnc[1],
         plmnId->mnc[2]);
   RETVALUE(NULLP);
} /* end of wrEmmMmeGetMmeByPlmn */

/** @brief This function validates the requested PLMNs are equal or not.
 *
 * @details This function validates the requested source and destination PLMNs
 *          are equal or not.
 *
 *     Function: wrEmmcheckActiveMmeForPlmn
 *
 *         Processing steps:
 *         - If the both requested PLMN ID's number of MNC digitts  not equal
 *           then return both the PLMNs are not equal.
 *         - Otherwise compare both the requested PLMN IDs are having the same
 *           MCC and MNC digit values.If bothe are matches then return TRUE.
 *           otherwise FALSE.
 *
 * @param[in] plmnId1: Primary PLMN ID
 * @return Bool
 *        -# Success : TRUE
 *        -# Failure : FALSE
 */

PRIVATE S16 wrEmmcheckActiveMmeForPlmn
(
WrPlmnId                     *plmnId
)
{
   WrPlmnCbMmeList           mmeList;
   U32                       mmeIdx;
   WrMmeCb                   *mmeCb;
   
   wrEmmPlmnGetMmeIdList(&mmeList, *plmnId);

   for(mmeIdx = 0; mmeIdx < mmeList.numOfMme; mmeIdx++)
   {
      /* Get the MME control block */
      mmeCb = WR_EMM_MME_GET_MME(mmeList.mmeIdList[mmeIdx]);
      if ((mmeCb->state == WR_MME_UP))
      {
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
}/* End of wrEmmcheckActiveMmeForPlmn */ 

PUBLIC S16 wrEmmcheckActiveMmme
(
)
{
   WrMmeCb                   *mmeCb = NULL;
   U16                       idx = 0;

   for(idx = 0; idx < WR_MAX_MMES; idx++)
   {
      mmeCb = wrEmmCb.mmeCont.mmes[idx];
      if (mmeCb == NULLP)
      {
         continue;
      }
      if ((mmeCb->state == WR_MME_UP) && (mmeCb->relCapacity > 0))
      {
         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
} 

#ifndef MME_LDBAL
/** @brief This function 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param  
 * @param 
 * @return 
 */
PRIVATE Void wrEmmChkAndDelGrpId 
(
 U16         grpId
)
{
   U8                        idx;

   for (idx = 0; idx < wrEmmCb.numGuGrpIds; idx++)
   {
      if (grpId == wrEmmCb.guGrpIdLst[idx].mmeGrpId)
      {
         /* Decrement the ref count */
         wrEmmCb.guGrpIdLst[idx].refCnt--;
         if (0 == wrEmmCb.guGrpIdLst[idx].refCnt)
         {
            if (wrEmmCb.numGuGrpIds)
            {
               wrEmmCb.numGuGrpIds--;
            }
            wrEmmCb.guGrpIdLst[idx].isValid = FALSE;
         }
      }
   }
} /* end of wrEmmChkAndDelGrpId */ 
#endif

/** @brief This function deletes the PLMN Ids supported by EnodeB for the 
 *         downed MME.
 *
 * @details This function deletes the PLMN Ids supported by EnodeB for the
 *          downed MME.
 *
 *     Function: wrEmmMmeDown 
 *
 *         Processing steps:
 *         - Delete all PLMN IDs suppporting by EnodeB for this MME.
 *         - Move the MME control block state to WR_MME_INITED.
 *         - Decrement the refCnt of 
 *
 *
 * @param[in] mmeCb: MME control block.
 * @return Nothing 
 */
PUBLIC Void wrEmmMmeDown
(
WrMmeCb                      *mmeCb
)
{
   U32                       idx             = 0;
#ifdef RM_INTF
   RmuFsmStatusInd           *rrmWrStaInd    = NULLP;
#else
   RrmWrEnbCfgReqInfo        *rrmWrEnbCfgReq = NULLP;
#endif /*RM_INTF*/
   WrEmmEnbCfgUpd*            cellInfo = &wrEmmCb.cellInfo;
   WrCellCb                   *cellCb = NULLP;
   PTR                        prevEntry = NULLP;
   WrNeighEnbCb               *nghEnbCb;
   U8                         cellIdx;
   S16                        retVal = RFAILED;
   cmMemset((U8 *) cellInfo, (U8) 0, (S16) sizeof(WrEmmEnbCfgUpd)); 

   mmeCb->state = WR_MME_INITED;
   
   for (idx = 0; idx < mmeCb->numPlmnIds; idx++)
   {    

      /*Need to check under Overall RIM timer condition  */
      if(TRUE == wrIsTmrRunning(&(wrEmmCb.rimOvrlTmr),WR_TMR_RIM_OVERALL))
      {
         if( wrEmmPlmnPlmnsEqual(&(mmeCb->plmnIds[idx]),
                     &(wrEmmCb.cellCb[0]->sib1.plmns[0].plmnId)))
         {
            retVal = wrEmmcheckActiveMmeForPlmn(
                  &(wrEmmCb.cellCb[0]->sib1.plmns[0].plmnId));
            if( RFAILED == retVal)
            {
               /* Stop the overall RIM timer */
               wrStopTmr((PTR)wrEmmCb.cellCb[0], WR_TMR_RIM_OVERALL);
               /* Set the RIM state to  WR_RIM_PROC_NOT_INITIATED*/
               wrEmmRimNghStateChange(wrEmmCb.cellCb[0]);
               /* As all the MMEs with Primary PLMN went down
                * delete all the RIM Associations */
               wrEmmRimMmeDownHdlr();
            }
         }
      }
      wrEmmPlmnDelMme(&mmeCb->plmnIds[idx], mmeCb->mmeId);
   }

   mmeCb->numPlmnIds = 0;
#ifndef MME_LDBAL
   U8                        oldCnt          = 0;
   oldCnt = wrEmmCb.numGuGrpIds;
   for (idx = 0; idx < mmeCb->numGrpIds; idx++)
   {
      wrEmmChkAndDelGrpId (mmeCb->groupIds[idx]);
   }
#endif

    wrEmmS1FlexdelMmeCbEntryFrmGummeiCbLst(mmeCb);
   /* Trigger Cfg Upd to all neihgbor enodeb's */
   /*MME_LDBAL: if the isCellModified is set to true prepare srvdCellMod list for eNB configuration update message*/      
   if((TRUE == cellInfo->isCellModified)  
         || (0 != cellInfo->numGrpIdDeleted))
   {
      for(cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
      {
         cellCb = wrEmmCb.cellCb[cellIdx];
         /* 
            check if this flag is set in wrEmmPbmModPlmnInSib1. if yes update the served cell
            and set the flag to false
         */
         if(TRUE == cellCb->sib1Modified) 
         {
            cellInfo->srvdCellMod[cellInfo->numSrvdCellModified].oldCellId 
               = cellCb->cellId;
            cellInfo->srvdCellMod[cellInfo->numSrvdCellModified].newCellId 
               = cellCb->cellId; 
            cellInfo->numSrvdCellModified++;
            cellCb->sib1Modified = FALSE;
         }

      }
      while ((retVal = cmHashListGetNext(&(wrEmmCb.neighEnbLstCp), prevEntry, (PTR *) &nghEnbCb)) == ROK)
      {
         if ( nghEnbCb->x2Connection == WR_NEIGH_ENB_STATE_UP )
         {
            retVal = wrEmmSndEnCfgUpReq(nghEnbCb, cellInfo);
            if ( retVal != ROK )
            {
               RLOG0(L_ERROR," wrEmmSndEnCfgUpReq failed");
               RETVOID;
            }
         }
         prevEntry = (PTR )nghEnbCb;
      }
   }
   /*MME_LDBAL*/
   /* Filling MME configuration in the structure. The MME addition information
    * shall be indicated to RRM using ENB reconfiguration request to RRM
    */
#ifdef RM_INTF
   WR_ALLOC (&rrmWrStaInd, sizeof(RmuCommonMsg));
   if(rrmWrStaInd == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVOID;
   } 

   rrmWrStaInd->enStatusType = RMU_FSM_STATUS_MME;
   rrmWrStaInd->u.stMmeStatusInd.enMmeStatus = RMU_MME_STATUS_DOWN;
   rrmWrStaInd->u.stMmeStatusInd.usMmeId = mmeCb->mmeId;


   /*Sending MME status indication*/
   if(ROK != wrIfmRrmStatusInd(rrmWrStaInd))
   {
      RLOG_ARG0(L_ERROR,DBG_MMEID,mmeCb->mmeId,"MMe status indication"
            "(MME DOWN)");
   }
#else /*RM_INTF*/
   WR_ALLOC (&rrmWrEnbCfgReq, sizeof(RrmWrEnbCfgReqInfo));

   if (rrmWrEnbCfgReq == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      
   }
   rrmWrEnbCfgReq->cfgType = RRM_WR_ENB_CFGTYPE_MMECFG;
   rrmWrEnbCfgReq->mmeCfg.addModOrDel = RRM_WR_CFG_DEL;
   rrmWrEnbCfgReq->mmeCfg.mmeId = mmeCb->mmeId;

   /* Sending the ENB reconfiguration request to RRM */
   if (ROK != wrIfmRrmEnodebCfgReq (rrmWrEnbCfgReq))
   {
      RLOG_ARG0(L_ERROR,DBG_MMEID,mmeCb->mmeId,"eNodeB recfg(MME DEL) failed");
   }
#endif /*RM_INTF*/
   RLOG_ARG0(L_INFO,DBG_MMEID,mmeCb->mmeId,"eNodeB recfg(MME DEL)");

   /* Check if there any MME is avilable, if not, move the cell state to 
    * power down. We are using index 0 as there is only one cell supported.
    * The cell power will be brought down, once the all the UEs attached to
    * this MME are released */
   if( wrEmmcheckActiveMmme() == RFAILED)
   {
      /* CNM_DEV Start */
      /* Inform CNM about the Cell-Down */
      wrEmmCnmPrcS1LnkDown ();
      /* CNM_DEV Stop */

      wrEmmStopNRTAgeTimer();
      for(idx = 0; idx < WR_EMM_MAX_CELLS; idx++) 
      {
         if ((wrEmmCb.cellCb[idx] != NULLP) &&  
               (WR_CELL_STATE_UP == wrEmmCb.cellCb[idx]->cellState))
         {
            if(ROK != wrEmmUpdateCellState(wrEmmCb.cellCb[idx]->cellId,
                     WR_CELL_STATE_PWR_DOWN))
            {
               RLOG0(L_ERROR,"wrEmmUpdateCellState is not"
                  "updated with WR_CELL_STATE_PWR_DOWN");
            }
            wrUmmUpdateCellState(wrEmmCb.cellCb[idx]->cellId, WR_CELL_STATE_PWR_DOWN);
            RLOG0(L_ERROR,"[ALARM] Sending CELL_UNAVAILABLE to OAM" );
            wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_UNAVAILABLE,
                  LCM_CAUSE_UNKNOWN);
            /* The cell power will be brought down here as there are no UEs 
             * which needs to be released. The same is done once all the UEs
             * are released if there are any */
            if(NULLP == (wrUmmGetUeCntxToRel(wrEmmCb.cellCb[idx]->cellId)))
            {
               wrEmmMmeBrngCellPwrDown();
            }
         }
         else if((wrEmmCb.cellCb[idx] != NULLP) &&  
               (WR_CELL_STATE_INIT == wrEmmCb.cellCb[idx]->cellState))
         {
	    wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_PHY_STOP_SUCCESS,
	       LCM_CAUSE_UNKNOWN);
         }
         else
         {
            /* When S1AP Association is already down cell state is 
             * WR_CELL_STATE_PWR_DOWN  for handling the IP change duirng 
             * this stage*/
            if(WR_CELL_STATE_PWR_DOWN == wrEmmCb.cellCb[idx]->cellState)
            {
               RLOG0(L_INFO,"[ALARM] Sending CELL_RECFG_SUCCESS to OAM" );
               wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_RECFG_SUCCESS,
                     LCM_CAUSE_UNKNOWN); 
            }
         } 
      }
   }
} /* end of wrEmmMmeDown */

/**
 * @brief This function brings down the power of the cell if there are no mme
 * in active state
 *
 * @details This function brings down the cell power if no mme is up
 *
 *     Function: wrEmmMmeBrngCellPwrDown
 *
 *         Processing steps:
 *            - Find if there are any MME active state
 *            - If there are no MME, then bring down the cell power
 *
 * @param[in] void
 * @return  S16
 *        -# Success : TRUE 
 *        -# Failure : FALSE 
 */
PUBLIC S16 wrEmmMmeBrngCellPwrDown
(
Void
)
{
   U16      idx;

   /* Starts - Fix for CR ccpu00123185 */
   /* Check if any MME is present or not if number of 
    * MME UP is zero then set Tx power low else 
    * continue with current Tx power
   */
   /* All MMEs are down, no S1 Connection available with eNodeb */
   for(idx = 0; idx < WR_EMM_MAX_CELLS; idx++) 
   {
      if ((wrEmmCb.cellCb[idx] != NULLP) &&
            ((WR_CELL_STATE_PWR_DOWN == wrEmmCb.cellCb[idx]->cellState) || 
             (WR_CELL_STATE_RECFG == wrEmmCb.cellCb[idx]->cellState)))
      {
        /*127802*/
         RLOG0(L_DEBUG,"******wrEmmMmeBrngCellPwrDown Power going DOWN****");
         if(wrEmmCellTxPwrReCfg(WR_TX_LOW, wrEmmCb.cellCb[idx]->cellId,wrEmmCb.cellCb[idx]->cellId) !=ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,wrEmmCb.cellCb[idx]->cellId,
               "wrEmmCellTxPwrReCfg: returned failure while setting power Low");
            RETVALUE(RFAILED);
         }
      }
   } 
   /* Ends - Fix for CR ccpu00123185 */

   RLOG0(L_ERROR,"[ALARM] Sending CELL_UNAVAILABLE to OAM" );
   wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_UNAVAILABLE,
                             LCM_CAUSE_UNKNOWN); 

   RETVALUE(ROK);
} /* wrEmmMmeBrngCellPwrDown */

/* Fix for ccpu00142992*/
/**
 * @brief This function checks the whether we have received abort confirm for
 * the peer MME or not before intiating the S1 setup.
 *
 * @details This function calls the s1 setup with peer MME. 
 *
 *     Function: wrEmmCallMmeSetup 
 *
 *         Processing steps:
 *            - Find if there are any MME active state
 *            - If there are no MME, then bring down the cell power
 *
 * @param[in] void
 * @return  S16
 *        -# Success : TRUE 
 *        -# Failure : FALSE 
 */
PUBLIC S16 wrEmmCallMmeSetup 
(
Void
)
{
   /* ccpu00142992:[BCM][GA4.0][TDD][S1AP]TTIs are not coming after
    * s1-close triggered from MME when UE is in attached state
    */

   if (wrEmmCb.peerAbrtCfm.isAbortCfmRcv == TRUE)
   {
      RLOG0(L_DEBUG,"wrEmmCallMmeSetup MME Connection will be done");

      wrEmmMmeSetup(wrEmmCb.peerAbrtCfm.mmeId);
      wrEmmCb.peerAbrtCfm.isAbortCfmRcv = FALSE;
      wrEmmCb.peerAbrtCfm.mmeId = 0;
   }
   else
   { 
       RLOG0(L_DEBUG," Abort Cfm has not come but context rel done ");
   }

   RETVALUE(ROK);
} /*wrEmmCallMmeSetup*/

/* CNM_DEV start */
/**
 * @brief This function brings up the power of the cell
 *
 * @details This function brings up the cell power
 *
 *     Function: wrEmmMmeBrngCellPwrUp
 *
 *         Processing steps:
 *            - For each configured cell, 
 *              - bring up the cell power
 *
 * @param[in] void
 * @return  S16
 *        -# Success : TRUE 
 *        -# Failure : FALSE 
 */
PUBLIC S16 wrEmmMmeBrngCellPwrUp
(
Void
)
{
   U16      idx;

   for(idx = 0; idx < WR_EMM_MAX_CELLS; idx++) 
   {
      if ((wrEmmCb.cellCb[idx] != NULLP) &&  
            (WR_CELL_STATE_UP == wrEmmCb.cellCb[idx]->cellState))
      {
         RLOG0(L_DEBUG,"******wrEmmMmeBrngCellPwrUp Power going UP****");
         if(ROK != wrEmmCellTxPwrReCfg(WR_TX_NORMAL,
                  wrEmmCb.cellCb[idx]->cellId, wrEmmCb.cellCb[idx]->cellId))
         {
            RLOG_ARG0(L_ERROR, DBG_CELLID, wrEmmCb.cellCb[idx]->cellId,
                  "returned failure while setting power to normal");
            RETVALUE(RFAILED);
         }
      }
   } 

   RETVALUE(ROK);
} /* wrEmmMmeBrngCellPwrUp */
/* CNM_DEV end */

/**
 * @brief This function compare the group id and MMEcs with the give MME id.
 *
 * @details This function compare the group id and MMEcs with the give MME id.
 *
 *     Function: wrEmmMmeGrpidMmecEqual
 *
 *         Processing steps:
 *         - find the MME control block based on mmeId.
 *         - find the MME GroupId in MME Control Blcok based on grpId
 *         - if mme Code is present in the MME Group MME code is present MMECb.
 *
 * @param[in] mmeId   : mmeId to compare the grpId and Mmecs
 * @param[in] grpId   : Group Id for which MME cb needs to be get
 * @param[in] mmeCode : MME Code for which MME cb needs to be get
 * @return  S16
 *        -# Success : TRUE 
 *        -# Failure : FALSE 
 */
PUBLIC S16 wrEmmMmeGrpidMmecEqual
(
U8                           mmeId,
U16                          grpId,
U8                           mmeCode
)
{
   WrMmeCb                   *mmeCb;
   U16                       idx;

   mmeCb = WR_EMM_MME_GET_MME(mmeId);
   if(mmeCb == NULLP)
   {
      RLOG1(L_ERROR,"Invalid MME ID [%d]",mmeId);
      RETVALUE(FALSE);
   }
   if(mmeCb->state != WR_MME_UP)
   {
     RLOG1(L_INFO," S1 Connection not up for MME [%d]",mmeId);
     RETVALUE(FALSE); 
   }

   /* Validate the groupId and mme code for the given mme Id 
    * In case, if we are not able to find match them, return failure */
   for(idx = 0; idx < mmeCb->numGrpIds; idx++)
   {
      if(mmeCb->groupIds[idx] == grpId)
      {
         break;
      }
   }
   if(idx == mmeCb->numGrpIds)
   {
      RETVALUE(FALSE);
   }

   for(idx = 0; idx < mmeCb->numCodes; idx++)
   {
      if(mmeCb->codes[idx] == mmeCode)
      {
         break;
        }
   }
   if(idx == mmeCb->numCodes)
   {
      RETVALUE(FALSE);
   }

   RETVALUE(TRUE);
} /* end of wrEmmMmeGrpidMmecEqual */

/** @brief This function is used to build and send the S1 Setup Request to the
 *         MME. 
 *
 * @details This function is used to build and send the S1 Setup Request to the
 *          MME.
 *
 *     Function: wrEmmMmeSetup 
 *
 *         Processing steps:
 *         - Build and send S1 Setup Request to the MME.
 *         - Move the MME control block state to WR_MME_IN_PROG.
 *         - Start the Setup response timer to retransmit the setup request
 *           incase of reponse is not received with in the time.
 *         - This function is invoked EMM module when EMM Module ges trigger
 *           from Management intrerface ie., during cell config/setup.
 *
 * @param[in] mmeId: MME Id.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeSetup
(
WrMmeId                      mmeId
)
{
   SztUDatEvnt               uDatEvnt;
   WrMmeCb                   *mmeCb;
   
   RLOG0(L_DEBUG, "Building S1 SETUP");
   mmeCb = WR_EMM_MME_GET_MME(mmeId);
   if(mmeCb == NULLP)
   {
      RLOG1(L_ERROR, "Invalid MME ID [%d]", mmeId);
      RETVALUE(RFAILED);
   }

   mmeCb->adminState = WR_MME_UNLOCKED;
   if (wrEmmMmeBldSetupReq(&(uDatEvnt.pdu)) != ROK)
   {
      RLOG0(L_ERROR, "Building S1 SETUP Failed");
      RETVALUE(RFAILED);
   }

   uDatEvnt.transId.pres = PRSNT_NODEF;
   uDatEvnt.transId.val = 1;
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val = mmeCb->peerId;
   RLOG1(L_INFO, "Sending S1 SETUP For Peer [%d]", mmeCb->peerId);
   /* Send the pdu to the MME */
   if((WrIfmS1apSndMgmtMsg(&uDatEvnt)) != ROK)
   {
      RLOG0(L_ERROR, "Sending S1 SETUP Failed");
      RETVALUE(RFAILED);
   }

   /* update state of the s1 connection */
   mmeCb->state = WR_MME_IN_PROG;

#if 0
#else
   wrStartTmr((PTR)mmeCb, WR_TMR_MME_SETUP_RSP, wrEmmCb.tmrCfg.s1SetupTmr);
#endif

   RETVALUE(ROK);
}

#ifdef SZTV2
/** @brief This function is used send S1 abort request message to MME
 *         MME. 
 *
 * @details This function is used send S1 Abort request message to MME
 *
 *     Function: wrEmmMmeAbortMsg 
 *
 *         Processing steps:
 *         Fill the peerId in AbortS1Req structure and call WrIfmS1apSndAbortMsg
 *
 * @param[in] WrMmeCb  mmeCb
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PRIVATE S16 wrEmmMmeAbortMsg
(
 WrMmeCb *mmeCb
)
{
   SztAbortS1   abortS1;
   abortS1.peerId.pres = PRSNT_NODEF;
   abortS1.peerId.val   = mmeCb->peerId;
   /* sndAbortFlag set to TRUE means this is SCTP ungraceful
    * close (i.e., abort) SCTP association */
   abortS1.sndAbortFlag = TRUE;
   RLOG0(L_DEBUG, "Sending S1 ABORT Msg");
   /* Send the pdu to the MME */
   if((WrIfmS1apSndAbortMsg(&abortS1)) != ROK)
   {
      RLOG0(L_ERROR, "Sending S1 Abort Msg Failed");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
#endif
/** @brief This function 
 *
 * @details
 *
 *     Function: wrEmmMmeSndMmeCfgUpdAck
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[in] mmeCb: MME Control Block.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PRIVATE S16 wrEmmMmeSndMmeCfgUpdAck
(
WrMmeCb                      *mmeCb
)
{
   SztUDatEvnt               uDatEvnt;

   if (wrEmmMmeBldMmeCfgUpdAck(&(uDatEvnt.pdu)) != ROK)
   {
      RLOG0(L_ERROR, "Building MME CONFIGURAION UPDATE ACKNOWLEDGE Failed");
      RETVALUE(RFAILED);
   }

   uDatEvnt.transId.pres = PRSNT_NODEF;
   uDatEvnt.transId.val  = 1;
   uDatEvnt.peerId.pres  = PRSNT_NODEF;
   uDatEvnt.peerId.val   = mmeCb->peerId;

   /* Send the pdu to the MME */
   if((WrIfmS1apSndMgmtMsg(&uDatEvnt)) != ROK)
   {
      RLOG0(L_ERROR, "Sending MME CONFIGURAION UPDATE ACKNOWLEDGE Failed");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG_ARG0(L_EVENT, DBG_MMEID, mmeCb->mmeId,"[S1AP]:Sending MME CONFIGURATION UPDATE ACKNOWLEDGE");
   }

   RETVALUE(ROK);
} /* end of wrEmmMmeSndMmeCfgUpdAck */

/** @brief This function is to send the EnodeB Configuration Update message to
 *         the MME. 
 *
 *  @details This function is used to build and send the EnodeB Configuration
 *          Update message to the MME.
 *
 *     Function: wrEmmMmeSendCfgUpdate
 *
 *         Processing steps:
 *         -  Set a flag that Cfg Update is sent
 *         -  Build config update PDU and dispatch the message to S1AP
 *         -  Start a timer for config udate response                     
 *
 * @param[in] mmeId: MME ID.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeSendCfgUpdate
(
WrMmeId                      mmeId
)
{
   SztUDatEvnt               uDatEvnt;
   WrMmeCb                   *mmeCb;
   RLOG0(L_DEBUG,"Building ENB CONFIGURATION UPDATE");
   mmeCb = WR_EMM_MME_GET_MME(mmeId);
   if (mmeCb == NULLP)
   {
      RLOG1(L_ERROR, "Invalid MME ID [%d]", mmeId);

      RETVALUE(RFAILED);
   }
   if (wrEmmMmeBldEnbCfgUpd(&(uDatEvnt.pdu)) != ROK)
   {
      RLOG0(L_ERROR, "Building ENB CONFIGURATION UPDATE Failed.");
      RETVALUE(RFAILED);
   }
   uDatEvnt.transId.pres = PRSNT_NODEF;
   uDatEvnt.transId.val = 1;
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val = mmeCb->peerId;

   /* Send the pdu to the MME */
   if((WrIfmS1apSndMgmtMsg(&uDatEvnt)) != ROK)
   {
      RLOG0(L_ERROR, "Sending ENB CONFIGURATION UPDATE Failed.");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG_ARG0(L_EVENT, DBG_MMEID, mmeCb->mmeId,"[S1AP]:Sending ENB CONFIGURATION UPDATE");
   }

   /* Start a timer here for tracking the response                        */
   wrStartTmr((PTR)mmeId, WR_TMR_ENB_CFG_RSP,
                             wrEmmCb.tmrCfg.s1UpdateTmr);
   RETVALUE(ROK);
} /* end of wrEmmMmeSendCfgUpdate */

/** @brief This function is to send the Reset Req message to
 *         the MME. 
 *
 * @details This function is used to build and send the RESET REQ 
 *          message to the MME.
 *
 *     Function: wrEmmMmeSendResetReq
 *
 *         Processing steps:
 *         -  Build RESET REQ PDU and dispatch the message to S1AP
 *         -  Start a timer for RESET  response                     
 *
 * @param[in] mmeId: mme Id.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeSendResetReq
(
U32          mmeId,
U8           causeType,
U32          cause,
U16          numCons,
U8           rstType,
WrEmmS1ConId    *conIds
)
{
   SztUDatEvnt               uDatEvnt;
   WrMmeCb                   *mmeCb;
   U32                        idx;
   
   mmeCb = WR_EMM_MME_GET_MME(mmeId);
   if (mmeCb == NULLP)
   {
      RLOG1(L_ERROR,"Invalid MME ID [%d]",mmeId);
      RETVALUE(RFAILED);
   }

   mmeCb->resetRtxInputs.causeType = causeType;
   mmeCb->resetRtxInputs.causeVal = cause;
   mmeCb->resetRtxInputs.nmbUes = numCons;
   mmeCb->resetRtxInputs.rstType = rstType;

   for ( idx = 0; idx < numCons; idx++)
   { 
      mmeCb->resetRtxInputs.conIds[idx].conId = conIds[idx].conId;
      mmeCb->resetRtxInputs.conIds[idx].type  = conIds[idx].type;
   }

   if (wrEmmMmeBldResetReq(&(uDatEvnt.pdu), rstType, numCons, causeType, cause, conIds) != ROK)
   {
      RLOG0(L_ERROR, "Building S1 RESET Failed");
      RETVALUE(RFAILED);
   }

   uDatEvnt.transId.pres = PRSNT_NODEF;
   uDatEvnt.transId.val = 1;
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val = mmeCb->peerId;

   if(rstType == RESETTYP_S1_INTF)
   {
      wrUmmMmeFullRst(mmeId);
   }
   else
   {
      wrUmmMmePartRst(mmeId, conIds, numCons);
   }

   /* Send the pdu to the MME */
   if((WrIfmS1apSndMgmtMsg(&uDatEvnt)) != ROK)
   {
      RLOG0(L_ERROR, "Sending S1 RESET REQUEST Failed");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG_ARG1(L_EVENT, DBG_MMEID, mmeCb->mmeId,
            "[S1AP]:Sending S1 RESET [Reset type:%d]",rstType);
      wrPrintS1apCauseInfo(causeType,cause);
   }
   /* Start a timer here for tracking the response                        */
#if 0
#else
   wrStartTmr((PTR)mmeId, WR_TMR_RST_RSP,
                             wrEmmCb.tmrCfg.s1ResetTmr);
#endif
   RETVALUE(ROK);
} /* end of wrEmmMmeSendCfgUpdate */

/* RIM */
/**
 *  @brief This function is used to Process MME CONFIGURATION TRANSFER
 *
 *  @details
 *      Function  : wrEmmMmePrcMmeDirectInfoTransfer
 *          Processing Steps:
 *          - Get MME Cb based upon PeerId
 *          - Update MME CONFIGURATION TRANSFER parameters as received.
 *
 *  @param[in] peerId : MME Id as received in S1AP message 
 *  @param[in] pdu : S1AP payload 
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
#ifdef ANSI
PRIVATE S16 wrEmmMmePrcMmeDirectInfoTransfer
(
 S1apPdu                     *pdu
)
#else
PRIVATE S16 wrEmmMmePrcMmeDirectInfoTransfer(S1apPdu  *pdu)
S1apPdu                      *pdu;
#endif
{
   U32                                    idx = 0;
   SztMMEDirectInformTfr                 *infoTfr = &(pdu->pdu.val.
                                                      initiatingMsg.value.u.
                                                      sztMMEDirectInformTfr);
   SztInter_SystemInformTfrTyp           *tfrType = NULLP;
   SztProtIE_Field_MMEDirectInformTfrIEs *ie = NULLP;

   TRC2(wrEmmMmePrcMMEDirectInformTfr);

   if((infoTfr->pres.pres == FALSE) || (infoTfr->protocolIEs.noComp.val == 0) ||
      (infoTfr->protocolIEs.member[0].pres.pres == FALSE))
   {
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < infoTfr->protocolIEs.noComp.val; idx++)
   {
      ie = infoTfr->protocolIEs.member + idx;
      switch(ie->id.val)
      {
         case Sztid_Inter_SystemInformTfrTypMDT:
            {
               tfrType = &infoTfr->protocolIEs.member[idx].value.u.sztInter_SystemInformTfrTyp;

               if(TRUE != tfrType->choice.pres)
               {
                  break;
               }

               switch(tfrType->choice.val)
               {
                  case INTER_SYSTEMINFORMTFRTYP_RIMTFR:
                     {
                        /* call the RIM API for pasing the received RIM PDU */
                        wrEmmRimPrcRimPdu(&(tfrType->val.rIMTfr.rIMInform));
                        if(tfrType->val.rIMTfr.rIMRoutingAddr.choice.pres)
                        {
                           if( RIMROUTINGADDR_GERAN_CELL_ID == 
                                 tfrType->val.rIMTfr.rIMRoutingAddr.choice.val )
                           {
                              RLOG0(L_INFO,"Geran Cell ID present in the"
                                    "RIM routing Info of received RIM PDU");
                           }
                           else if ( RIMROUTINGADDR_TARGETRNC_ID == 
                                        tfrType->val.rIMTfr.rIMRoutingAddr.choice.val )
                           {
                              RLOG0(L_INFO,"RNC ID is present in the"
                                    "RIM routing Info of received RIM PDU");
                           }
                        }
                     }
                     break;
                  default:
            RLOG1(L_ERROR,"Invalid case [%d]",tfrType->choice.val);
                    break;

               } /* end of switch */
            }
            break;
         default :
            RLOG1(L_ERROR,"Invalid case [%d]",ie->id.val);
            break;

      } /*end of the switch */
   } /* end of for loop */  

   RETVALUE(ROK); 		

} /* wrEmmMmePrcMmeDirectInfoTransfer */

/* RIM */  

/** @brief This function is to send the EnodeB Configuration Update Fail message to
 *         the MME. 
 *
 * @details This function is used to build and send the EnodeB Configuration
 *          Update Fail message to the MME.
 *
 *     Function: wrEmmMmeSendCfgUpdateFail
 *
 *         Processing steps:
 *         -  Set a flag that Cfg Update is sent
 *         -  Build config update Fail PDU and dispatch the message to S1AP
 *
 * @param[in] mmeId: MME ID.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeSendCfgUpdateFail
(
WrMmeId                    mmeId,
WrUmmMsgCause              *cause, 
U8                         timeToWaitSecs
)
{
   SztUDatEvnt               uDatEvnt;
   WrMmeCb                   *mmeCb;

   mmeCb = WR_EMM_MME_GET_MME(mmeId);
   if (mmeCb == NULLP)
   {
      RLOG1(L_ERROR,"Invalid MME ID [%d]",mmeId);
      RETVALUE(RFAILED);
   }
   
  RLOG_ARG2(L_DEBUG,DBG_MMEID, mmeCb->mmeId,
     "Sending S1 MME CONFIGURATION UPDATE FAILURE, Cause type = [%d],"
     "Cause value = [%d]",cause->causeTyp, cause->causeVal);
   if ( wrEmmMmeBldEnbCfgUpdFail(&(uDatEvnt.pdu), cause, timeToWaitSecs) != ROK)
   {
      RETVALUE(RFAILED);
   }
   uDatEvnt.transId.pres = PRSNT_NODEF;
   uDatEvnt.transId.val = 1;
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val = mmeCb->peerId;


   /* Send the pdu to the MME */
   if((WrIfmS1apSndMgmtMsg(&uDatEvnt)) != ROK)
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmMmeSendCfgUpdateFail */

/** @brief This function is to send the EnodeB Err Indication to MME 
 *
 * @details This function is used to build and send the EnodeB Err Indication 
 *   
 *
 *     Function: wrEmmMmeSendErrInd
 *
 *         Processing steps:
 *         -  Build Error Indication PDU and dispatch the message to S1AP
 *
 * @param[in] mmeId  : MME ID.
 * @param[in] cause  : causeType and causeVal 
 * @param[in] eNbUeId: enodeb UE Id
 * @param[in] mmeUeId: MME  UE Id.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmeSendEnbErrInd
(
WrMmeId                      mmeId,
WrUmmMsgCause               *cause, 
U32                         *eNbUeId,
U32                         *mmeUeId 
)
{
   SztUDatEvnt               uDatEvnt;
   WrMmeCb                   *mmeCb;

   mmeCb = WR_EMM_MME_GET_MME(mmeId);
   if (mmeCb == NULLP)
   {
      RLOG1(L_ERROR,"Invalid MME ID [%d]",mmeId);
      RETVALUE(RFAILED);
   }
   if ( wrEmmMmeBldEnbErrInd(&(uDatEvnt.pdu), cause, eNbUeId, mmeUeId) != ROK)
   {
      RETVALUE(RFAILED);
   }
   uDatEvnt.transId.pres = PRSNT_NODEF;
   uDatEvnt.transId.val = 1;
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val = mmeCb->peerId;


   RLOG_ARG2(L_DEBUG,DBG_MMEID, mmeId,
      "Sending S1 ERROR-INDICATION to MME, Cause Type = [%d], Cause = [%d]",
      cause->causeTyp, cause->causeVal);
   /* Send the pdu to the MME */
   if((WrIfmS1apSndMgmtMsg(&uDatEvnt)) != ROK)
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrEmmMmeSendEnbErrInd */

/** @brief This function adds a PLMN ID to MME. 
 *
 * @details It is invoked as part of SETUP response or configuration update
 *         received from MME
 *
 *     Function: wrEmmMmeAddPlmnId 
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[in, out] mmeCb: MME control block 
 * @param[in]     plmnId: adding PLMN ID. 
 * @return  S16
 *     -# Success :Always ROK as checks are performed prior to calling this. 
 */
PRIVATE S16 wrEmmMmeAddPlmnId
(
WrMmeCb                      *mmeCb,
WrPlmnId                     *plmnId
)
{
   wrEmmPlmnAddMme(plmnId, mmeCb->mmeId);
   mmeCb->plmnIds[mmeCb->numPlmnIds] = *plmnId;
   mmeCb->numPlmnIds++;
   RETVALUE(ROK);
} /* end of  wrEmmMmeAddPlmnId */

/** @brief This function deletes a PLMN ID from MME. It is invoked when 
 *         config update is received from MME.
 *
 * @details This function deletes a PLMN ID from the MME supported PLMN IDs.
 *           when MME config update received.
 *
 *     Function: wrEmmMmeDelPlmnId 
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[in, out] mmeCb: MME control block 
 * @param[in]     plmnId: deletes PLMN ID. 
 * @return  S16
 *      -# Success : Always ROK as checks are performed prior to calling this.
 */
PRIVATE S16 wrEmmMmeDelPlmnId
(
WrMmeCb                      *mmeCb,
WrPlmnId                     *plmnId
)
{
   S32                       idx = 0;
   S32                       loopCnt = 0;
   for (idx = 0; idx < mmeCb->numPlmnIds; idx++)
   {
      if (wrUtlPlmnsEqual(plmnId, &mmeCb->plmnIds[idx]) == TRUE)
      {
         wrEmmPlmnDelMme(plmnId, mmeCb->mmeId);
         mmeCb->numPlmnIds--;
         break;
      }
   }
   loopCnt =  mmeCb->numPlmnIds;
   for (;idx < loopCnt;idx++)
   {
      mmeCb->plmnIds[idx] = mmeCb->plmnIds[idx + 1];
   }
   RETVALUE(ROK);
} /* end of wrEmmMmeDelPlmnId */

/** @brief This function constructs the PLMN ID from the received S1AP PLMN IE.
 *
 * @details This function constructs the PLMN ID from the received S1AP PLMN IE.
 *
 *     Function: wrEmmMmeParsePlmnIe
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[in]   plmnIe: S1-AP PLMN IE  
 * @param[out]  plmnId: PLMN ID
 * @return  S16
 *          -# Success : ROK
 */
PUBLIC S16 wrEmmMmeParsePlmnIe
(
SztPLMNidentity              *plmnIe,
WrPlmnId                     *plmnId
)
{
   plmnId->mcc[0]          = plmnIe->val[0] &0x0f;
   plmnId->mcc[1]          = (plmnIe->val[0] & 0xf0) >> 4;
   plmnId->mcc[2]          = plmnIe->val[1] &0x0f;
   if ((plmnIe->val[1] & 0xf0) == 0xf0)
   {
      plmnId->numMncDigits = 2;
      plmnId->mnc[1]       = (plmnIe->val[2] & 0xf0) >> 4;
      plmnId->mnc[0]       = plmnIe->val[2] & 0x0f;
   }
   else
   {
      plmnId->numMncDigits = 3;
      plmnId->mnc[0]       = (plmnIe->val[1] & 0xf0) >> 4;
      plmnId->mnc[1]       = plmnIe->val[2] & 0x0f;
      plmnId->mnc[2]       = (plmnIe->val[2] & 0xf0) >> 4;
   }
   RETVALUE(ROK);
} /* end of wrEmmMmeParsePlmnIe */


#ifndef   MME_LDBAL
/** @brief This function adds the reported Group IDs supported by the MME to the
 * list of Group IDs serviced by the ENodeB. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - 
            Loop through the new group IDs looking for a group Id that is not present
 *          in the list of group IDs present in the emmCb
 *          - Alternative approach is to keep the groupIds sorted in the emmCb
 *          then this comparison would be less expensive
 *          - If same Group ID is present then increment the refCnt
 *          - Group ID is deleted only if refCnt is 0
 *
 *
 * @param  
 * @param 
 * @return 
 */
PRIVATE Void wrEmmMmePrcGrpIds
(
 WrMmeCb    *mmeCb
)
{
   U8    mmeGrpIdx;
   U16    eNbGrpIdx;
   U16   mmeGrpId;
   U16   enbGrpId;
   U16    numGrpToAdd;
   U16   grpIdToAdd[WR_MAX_GRPS_PER_MME];
   Bool  isOld;
   WrPlmnId *plmnId;

   numGrpToAdd = 0;

   for (mmeGrpIdx = 0; mmeGrpIdx < mmeCb->numGrpIds; mmeGrpIdx++)
   {
      isOld = FALSE;
      mmeGrpId = mmeCb->groupIds[mmeGrpIdx];
      for (eNbGrpIdx = 0; eNbGrpIdx < wrEmmCb.numGuGrpIds; eNbGrpIdx++)
      {
         if (wrEmmCb.guGrpIdLst[eNbGrpIdx].isValid == FALSE)
            continue;
         enbGrpId = wrEmmCb.guGrpIdLst[eNbGrpIdx].mmeGrpId;
         if (mmeGrpId == enbGrpId)
         {
            isOld = TRUE;
            break;
         }
      }
      if ((isOld == FALSE) && (numGrpToAdd < WR_MAX_GRPS_PER_MME))
      {
         grpIdToAdd[numGrpToAdd] = mmeGrpId;
         numGrpToAdd++;
      }
   } /* end of mmeGrpIdx */

   for (eNbGrpIdx = 0; ((eNbGrpIdx < numGrpToAdd) && (eNbGrpIdx < WR_MAX_GRPS_PER_MME)
            && ((eNbGrpIdx + wrEmmCb.numGuGrpIds) < WR_MAX_GU_GRP_IDS)); eNbGrpIdx++)
   {
      wrEmmCb.guGrpIdLst[(wrEmmCb.numGuGrpIds + eNbGrpIdx)].mmeGrpId = 
         grpIdToAdd[eNbGrpIdx];
      /* sri - HOFIX */
      /* using the first plmn of this mme */
      plmnId = &(wrEmmCb.guGrpIdLst[(wrEmmCb.numGuGrpIds + eNbGrpIdx)].plmnId);
      plmnId->mcc[0] = mmeCb->plmnIds[0].mcc[0];
      plmnId->mcc[1] = mmeCb->plmnIds[0].mcc[1];
      plmnId->mcc[2] = mmeCb->plmnIds[0].mcc[2];
      plmnId->mnc[0] = mmeCb->plmnIds[0].mnc[0];
      plmnId->mnc[1] = mmeCb->plmnIds[0].mnc[1];
      if(mmeCb->plmnIds[0].numMncDigits > 2)
      {
         plmnId->mnc[0] = mmeCb->plmnIds[0].mnc[2];
      }
      plmnId->numMncDigits = mmeCb->plmnIds[0].numMncDigits;
      RLOG1(L_INFO,"MME Group ID : [%d]",
            wrEmmCb.guGrpIdLst[(wrEmmCb.numGuGrpIds + eNbGrpIdx)].mmeGrpId);
      
   }
   wrEmmCb.numGuGrpIds += numGrpToAdd;

   RETVOID;
} /* end of wrEmmMmePrcGrpIds */ 
#endif

/** @brief This function updates the MME control Block with 
 *         GUMMEIs received in S1 Setup response message.
 *
 * @details This function updates the MME control Block with
 *          GUMMEIs received in S1 Setup response message.
 *
 *     Function: wrEmmMmePrcGummeis
 *
 *         Processing steps:
 *         - 
 *
 *         - Notes: We support only LTE PLMN IDs and hence we process 
 *            only one IE in this function.
 *
 * @param[in, out] mmeCb: MME control block. 
 * @param[in]         ie: served GUMME IDs IE receved in
 *                        S1-Setup Response msg.
 * @return  S16
 *          -# Success : ROK
 */
PRIVATE S16 wrEmmMmePrcGummeis
(
WrMmeCb                        *mmeCb,
SztProtIE_Field_S1SetupRespIEs *ie
)
{
   U32                         idx;
   SztSrvdGUMMEIs              *gummeis = &ie->value.u.sztSrvdGUMMEIs;
   SztSrvdGUMMEIsItem          *gummei  = gummeis->member;
   SztSrvdPLMNs                *plmns   = &gummei->servedPLMNs;
   WrPlmnId                    plmnId;
   U32                         noPlmns = plmns->noComp.val;
   SztSrvdGroupIDs             *grpIds = &gummei->servedGroupIDs;
   SztSrvdMMECs                *mmeCs = &gummei->servedMMECs;
   U32                         noGrpIds;
   U32                         noMmeCs;
   PTR                        prevEntry = NULLP;
   WrNeighEnbCb              *nghEnbCb;
   WrEmmEnbCfgUpd*             cellInfo = &wrEmmCb.cellInfo;
   WrCellCb                   *cellCb = NULLP;
   U8                         cellIdx;
   S16                        retVal = RFAILED;
   
   cmMemset((U8 *) cellInfo, (U8) 0, (S16) sizeof(WrEmmEnbCfgUpd));
  
   wrEmmS1FlexInsertGummeiInMmeCb(mmeCb, gummeis);
   

   if (noPlmns > WR_MAX_PLMNS_PER_MME)
   {
      noPlmns = WR_MAX_PLMNS_PER_MME;
   }

   for (idx = 0; idx < noPlmns; idx++)
   {
      wrEmmMmeParsePlmnIe(plmns->member + idx, &plmnId);
      wrEmmMmeAddPlmnId(mmeCb, &plmnId);
   }

   noGrpIds = grpIds->noComp.val;
   if(noGrpIds > WR_MAX_GRPS_PER_MME)
   {
      noGrpIds = WR_MAX_GRPS_PER_MME;
   }

   for(idx = 0; idx < noGrpIds; idx++)
   {
      mmeCb->groupIds[idx] = (((grpIds->member + idx)->val[1]) | 
                              ((grpIds->member + idx)->val[0] << 8));
   }

   mmeCb->numGrpIds = noGrpIds;
   /* The reported Group IDs are the ones that have to be added the the eNodeB's
    * Group IDs
    */
#ifndef MME_LDBAL
   wrEmmMmePrcGrpIds (mmeCb);
#endif

   noMmeCs = mmeCs->noComp.val;
   if(noMmeCs > WR_MAX_CODES_PER_MME)
   {
      noMmeCs = WR_MAX_CODES_PER_MME;
   }

   for(idx = 0; idx < noMmeCs; idx++)
   {
      mmeCb->codes[idx] = (mmeCs->member + idx)->val[0];
   }

   mmeCb->numCodes = noMmeCs;

/* CNM_DEV start */
/* CNM_DEV end */

    /* Trigger Cfg Upd to all neihgbor enodeb's */
   if((TRUE == cellInfo->isCellModified) || (0 != cellInfo->numGrpIdAdded))
   {
      for(cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
      {
         cellCb = wrEmmCb.cellCb[cellIdx];
         if(TRUE == cellCb->sib1Modified) 
         {
            cellInfo->srvdCellMod[cellInfo->numSrvdCellModified].oldCellId 
               = cellCb->cellId;
            cellInfo->srvdCellMod[cellInfo->numSrvdCellModified].newCellId 
               = cellCb->cellId; 
            cellInfo->numSrvdCellModified++;
            cellCb->sib1Modified = FALSE;
         }

      }

      while ((retVal = cmHashListGetNext(&(wrEmmCb.neighEnbLstCp), prevEntry, (PTR *) &nghEnbCb)) == ROK)
      {
         if ( nghEnbCb->x2Connection == WR_NEIGH_ENB_STATE_UP )
         {
            retVal = wrEmmSndEnCfgUpReq(nghEnbCb, cellInfo);
            if ( retVal != ROK )
            {
               RLOG0(L_ERROR, " wrEmmSndEnCfgUpReq failed ");
               RETVALUE(RFAILED);
            }
         }
         prevEntry = (PTR )nghEnbCb;
      }
   } 
   RETVALUE(ROK);
} /* end of wrEmmMmePrcGummeis */

#define WR_PLMNID_OLD 1
#define WR_PLMNID_NEW 2
/** @brief This function updates MME with the new set of GUMMEIs 
 *         received in MME configuration update.
 *
 * @details This function updates MME with the new set of GUMMEIs
 *          received in MME configuration update.
 *
 *     Function: wrEmmMmeUpdGummeis
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[in, out] mmeCb: MME control block. 
 * @param[in]         ie: served GUMME IDs IE receved in
 *                        MME configuration update msg. 
 * @return  S16
 *          -# Success : ROK
 */
PRIVATE S16 wrEmmMmeUpdGummeis
(
WrMmeCb                        *mmeCb,
SztProtIE_Field_S1SetupRespIEs *ie
)
{
   U32                         idx;
   U32                         idx1;
   SztSrvdGUMMEIs              *gummeis   = &ie->value.u.sztSrvdGUMMEIs;
   SztSrvdGUMMEIsItem          *gummei    = gummeis->member;
   SztSrvdPLMNs                *plmns     = &gummei->servedPLMNs;
   SztSrvdGroupIDs             *grpIds = &gummei->servedGroupIDs;
   SztSrvdMMECs                *mmeCs = &gummei->servedMMECs;
   U32                         totalPlmns = 0;
   U32                         rcvdPlmns = plmns->noComp.val;
   WrPlmnId                    plmnIds[2 * WR_MAX_PLMNS_PER_MME];
   WrPlmnId                    plmnId;
   U32                         oldNewFlag[2 * WR_MAX_PLMNS_PER_MME];
   U32                         nmbGrpIds = grpIds->noComp.val;
   U32                         nmbMmecs = mmeCs->noComp.val;
   WrEmmEnbCfgUpd*             cellInfo = &wrEmmCb.cellInfo;
   U8                         cellIdx;
   WrCellCb                   *cellCb = NULLP;
   PTR                        prevEntry = NULLP;
   WrNeighEnbCb              *nghEnbCb;
   S16                        retVal = RFAILED;
   
   cmMemset((U8 *) cellInfo, (U8) 0, (S16) sizeof(WrEmmEnbCfgUpd));
   
   wrEmmS1FlexUpdateGummeiInMmeCb(mmeCb,gummeis);
   if (rcvdPlmns > WR_MAX_PLMNS_PER_MME)
   {
      rcvdPlmns = WR_MAX_PLMNS_PER_MME;
   }
   for (idx = 0; idx < mmeCb->numPlmnIds; idx++)
   {
      plmnIds[totalPlmns]    = mmeCb->plmnIds[idx];
      oldNewFlag[totalPlmns] = WR_PLMNID_OLD;
      totalPlmns++;
   }
   for (idx = 0; idx < rcvdPlmns; idx++)
   {
      wrEmmMmeParsePlmnIe(plmns->member + idx, &plmnId);
      for (idx1 = 0; idx1 < mmeCb->numPlmnIds; idx1++)
      {
         if (wrUtlPlmnsEqual(plmnIds + idx1, &plmnId) == TRUE)
         {
            oldNewFlag[idx1] |= WR_PLMNID_NEW;
            break;
         }
      }
      if (idx1 == mmeCb->numPlmnIds)
      {
         plmnIds[totalPlmns]    = plmnId;
         oldNewFlag[totalPlmns] = WR_PLMNID_NEW;
         totalPlmns++;
      }
   }
   for (idx = 0; idx < totalPlmns; idx++)
   {
      if (oldNewFlag[idx] == WR_PLMNID_NEW)
      {
         wrEmmMmeAddPlmnId(mmeCb, &plmnIds[idx]);
      }
      else if (oldNewFlag[idx] == WR_PLMNID_OLD)
      {
         wrEmmMmeDelPlmnId(mmeCb, &plmnIds[idx]);
      }
   }

   if(nmbGrpIds > WR_MAX_GRPS_PER_MME)
   {
      nmbGrpIds = WR_MAX_GRPS_PER_MME;
   }

   for(idx = 0; idx < nmbGrpIds; idx++)
   {
      mmeCb->groupIds[idx] = (((grpIds->member + idx)->val[0]) |
            ((grpIds->member + idx)->val[1] << 8));
   }

   mmeCb->numGrpIds = nmbGrpIds;

   nmbMmecs = mmeCs->noComp.val;
   if(nmbMmecs > WR_MAX_CODES_PER_MME)
   {
      nmbMmecs = WR_MAX_CODES_PER_MME;
   }

   for(idx = 0; idx < nmbMmecs; idx++)
   {
      mmeCb->codes[idx] = (mmeCs->member + idx)->val[0];
   }

   mmeCb->numCodes = nmbMmecs;
   /*Send eNB config Update if there is any modification in served cell*/
   if((TRUE == cellInfo->isCellModified) || (0 != cellInfo->numGrpIdAdded) 
         || (0 != cellInfo->numGrpIdDeleted))
   {
      for(cellIdx = 0; cellIdx < wrEmmCb.numCells; cellIdx++)
      {
         cellCb = wrEmmCb.cellCb[cellIdx];
         if(TRUE == cellCb->sib1Modified) 
         {
            cellInfo->srvdCellMod[cellInfo->numSrvdCellModified].oldCellId 
               = cellCb->cellId;
            cellInfo->srvdCellMod[cellInfo->numSrvdCellModified].newCellId 
               = cellCb->cellId; 
            cellInfo->numSrvdCellModified++;
            cellCb->sib1Modified = FALSE;
         }

      }

      while ((retVal = cmHashListGetNext(&(wrEmmCb.neighEnbLstCp), prevEntry, (PTR *) &nghEnbCb)) == ROK)
      {
         if ( nghEnbCb->x2Connection == WR_NEIGH_ENB_STATE_UP )
         {
            retVal = wrEmmSndEnCfgUpReq(nghEnbCb, cellInfo);
            if ( retVal != ROK )
            {
               RLOG0(L_ERROR, " wrEmmSndEnCfgUpReq failed ");
               RETVALUE(RFAILED);
            }
         }
         prevEntry = (PTR )nghEnbCb;
      }
   } 
   RETVALUE(ROK);
} /* end of wrEmmMmeUpdGummeis */

/** @brief This function updates the Relative Capcity IE values in MME
 *         control block.
 *
 * @details
 *
 *     Function: wrEmmMmeUpdRelCap 
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param[in, out] mmeCb: MME control block. 
 * @param[in]         ie: S1-AP:Relative capacity IE. 
 * @return  S16
 *          -# Success : ROK
 */
PRIVATE S16 wrEmmMmeUpdRelCap
(
WrMmeCb                      *mmeCb,
SztRelativeMMECapacity       *ie
)
{
   mmeCb->relCapacity = ie->val;
   RLOG1(L_INFO,"Received MME Relative Capacity = [%d]", mmeCb->relCapacity);
   /* Recompute any parameters regarding MME selection                    */
   RETVALUE(ROK);
} /* end of wrEmmMmeUpdRelCap */

/** @brief This function process the S1AP:S1 Setup Response mesage.
 *
 * @details This function process the S1AP:S1 Setup Response mesage.
 *
 *     Function: wrEmmMmePrcSetupRsp 
 *
 *         Processing steps:
 *         - Stop the S1 Setup Response timer.
 *         - parse and Update the MME control block with the received
 *           information in S1 Setup Response.
 *         - Finally move the MME control block state is WR_MME_UP.
 *         - Send MME add request to RRM
 *         - Send an Alram to SM to indicate the S1 Setup procedure is completed
 *           successfully.
 *
 *@param[in]     peerId: Peer Id from which S1 setup resp recvd
 *@param[in]        pdu: S1-AP:setup response PDU 
 *@return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PRIVATE S16 wrEmmMmePrcSetupRsp
(
U32                          peerId,
S1apPdu                      *pdu
)
{
   U32                            idx;
   WrMmeCb                        *mmeCb;
   SztSuccessfulOutcome           *succMsg  = &pdu->pdu.val.successfulOutcome;
   SztS1SetupResp                 *setupRsp = &succMsg->value.u.sztS1SetupResp;
   SztProtIE_Field_S1SetupRespIEs *IEs = setupRsp->protocolIEs.member;
#ifdef RM_INTF
   RmuFsmStatusInd                *rrmWrStaInd = NULLP;
#else
   RrmWrEnbCfgReqInfo             *rrmWrEnbCfgReq = NULLP;
#endif /*RM_INTF*/
   S16                        retVal = RFAILED;

   /* Get MME from the peerId                                             */
   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);
   if (mmeCb == NULLP)
   {
      RLOG1(L_ERROR,"Processing S1 Setup Response,"
      "Received Invalid Peer Id [%d]",peerId);
      RETVALUE(RFAILED);
   }
#ifdef WR_TEST_CODE
   if ( wrTgrS1SetupRtx == TRUE )
   {
      /* Do nothing and return */  
      RLOG0(L_INFO,"Dropping S1-Setup Rsp, Verify the Rtx");
      RETVALUE(ROK);
   }
#endif

   /* Stop the S1-Setup Timer started for this peer */
   wrStopTmr((PTR)mmeCb, WR_TMR_MME_SETUP_RSP);
   RLOG_ARG0(L_EVENT, DBG_MMEID, mmeCb->mmeId, "[S1AP]:Received S1 SETUP RESPONSE");

   for(idx = 0; idx < setupRsp->protocolIEs.noComp.val; idx++)
   {
      SztProtIE_Field_S1SetupRespIEs* ie = IEs + idx;

      switch(ie->id.val)
      {
         case Sztid_RelativeMMECapacity:
            wrEmmMmeUpdRelCap(mmeCb, &ie->value.u.sztRelativeMMECapacity);
            break;
         case Sztid_MMEname:
            /* Currently being ignored                                    */
            break;
         case Sztid_SrvdGUMMEIs:
            if (wrEmmMmePrcGummeis(mmeCb, ie) != ROK)
            {
               RLOG0(L_ERROR, "wrEmmMmePrcGummeis failed.");
               RETVALUE(RFAILED);
            }
            break;
      }
   }
  
#ifdef RM_INTF
   WR_ALLOC (&rrmWrStaInd, sizeof(RmuCommonMsg));
   if(rrmWrStaInd == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   rrmWrStaInd->enStatusType = RMU_FSM_STATUS_MME;
   rrmWrStaInd->u.stMmeStatusInd.enMmeStatus = RMU_MME_STATUS_UP;
   rrmWrStaInd->u.stMmeStatusInd.usMmeId = mmeCb->mmeId;

   /*Sending MME status indication*/
   if(ROK != wrIfmRrmStatusInd(rrmWrStaInd))
   {
      RLOG_ARG0(L_ERROR,DBG_MMEID,mmeCb->mmeId,"MMe status indication(MME UP)");
   }
#else 
   /* Filling MME configuration in the structure. The MME addition information
    * shall be indicated to RRM using ENB reconfiguration request to RRM
    */
   WR_ALLOC (&rrmWrEnbCfgReq, sizeof(RrmWrEnbCfgReqInfo));
   if (rrmWrEnbCfgReq == NULLP)
   {
       RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   rrmWrEnbCfgReq->cfgType = RRM_WR_ENB_CFGTYPE_MMECFG;
   rrmWrEnbCfgReq->mmeCfg.addModOrDel = RRM_WR_CFG_ADD;
   rrmWrEnbCfgReq->mmeCfg.mmeId = mmeCb->mmeId;

   /* Sending the ENB reconfiguration request to RRM */
   if (wrIfmRrmEnodebCfgReq (rrmWrEnbCfgReq) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_MMEID,mmeCb->mmeId,
         "eNodeB recfg(MME ADD) failed ");
      RETVALUE(RFAILED);
   }
#endif /*RM_INTF*/

   RLOG_ARG0(L_INFO,DBG_MMEID,mmeCb->mmeId,"eNodeB recfg(MME ADD)");

   mmeCb->state   = WR_MME_UP;
   /* CNM_DEV start */
   wrEmmCnmPrcS1LnkUp();
  // wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_S1CON_SET_SUCCESS,
  //                                      LCM_CAUSE_UNKNOWN);
   /* CNM_DEV end */
 
   wrCb.parResetCmfState |= WR_S1_SETUP_DONE_PAR_RESET; 


   for(idx = 0; idx < WR_EMM_MAX_CELLS; idx++) 
   {
       if ((wrEmmCb.cellCb[idx] != NULLP) &&  
           (((WR_CELL_STATE_RESET == wrEmmCb.cellCb[idx]->cellState) &&
            (WR_PAR_RESET_DONE == wrCb.parResetCmfState)) ||
           (WR_CELL_STATE_PWR_DOWN == wrEmmCb.cellCb[idx]->cellState)))
       {
         if(wrEmmCellTxPwrReCfg(WR_TX_NORMAL, wrEmmCb.cellCb[idx]->cellId,wrEmmCb.cellCb[idx]->cellId) !=ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,wrEmmCb.cellCb[idx]->cellId,
               "wrEmmCellTxPwrReCfg: returned failure while setting power UP");
            RETVALUE(RFAILED);
         }
          wrCb.parResetCmfState = WR_INIT_STATE_NONE;
          /* This is done as the CELL RESET State is Common 
           * for Partial Reset and Full Reset Case 
           */
          wrCb.resetCmfState = WR_INIT_STATE_NONE;
          if(ROK != wrEmmUpdateCellState(wrEmmCb.cellCb[idx]->cellId,
                                         WR_CELL_STATE_UP))
            {
               RLOG0(L_ERROR,"wrEmmUpdateCellState is not"
                  "updated with WR_CELL_STATE_UP");
            }
          RLOG_ARG0(L_DEBUG,DBG_CELLID, wrEmmCb.cellCb[idx]->cellId,
             "Cell State = UP");

          /* Update the UMM Module Cell State as WR_CELL_STATE_UP */
          if(ROK != wrUmmUpdateCellState(wrEmmCb.cellCb[idx]->cellId,WR_CELL_STATE_UP))
          {
             RLOG_ARG0(L_ERROR,DBG_CELLID,wrEmmCb.cellCb[idx]->cellId,
                      "wrEmmMmePeerRestart: Cell Cb Not found");
             RETVALUE(RFAILED);
          }
       }
   }

   if(FALSE == wrIsTmrRunning(&(wrEmmCb.rimOvrlTmr),WR_TMR_RIM_OVERALL))
   {
      if( WR_CELL_STATE_UP      == wrEmmCb.cellCb[0]->cellState)
      {
         for( idx = 0; idx < mmeCb->numPlmnIds;idx++)
         {
            if ( wrEmmPlmnPlmnsEqual(&(mmeCb->plmnIds[idx]),
                     &(wrEmmCb.cellCb[0]->sib1.plmns[0].plmnId))) 
            {
               retVal = wrEmmcheckActiveMmeForPlmn(
                     &(wrEmmCb.cellCb[0]->sib1.plmns[0].plmnId));
               if( ROK == retVal)
               {
                  /* Sending the RIR with Multiple request to all the 
                   * UTRA & GERAN Cells*/
                  wrEmmTrigRirProc(wrEmmCb.cellCb[0],WR_RIM_RIR_MULTI);
               }
            }
         }
      }
   }
   RETVALUE(ROK);
} /* end of wrEmmMmePrcSetupRsp */

/** @brief This function process the S1AP: MME Configuration Update Message.
 *
 * @details This function is process the S1AP: MME Configuration Update Message.
 *
 *     Function: wrEmmMmePrcMmeCfgUpd 
 *
 *         Processing steps:
 *         - Parse and update the already existed MME control block with the
 *           received information in MME Configuration Update message.
 *         - Finally build and send a MME Configuration Update Acknoledgement
 *           message to the MME.
 *
 *@param[in] peerId    : Peer Id from which MME cfg update recvd
 *@param[in] pdu       : S1-AP MME cfg update PDU 
 *@return  S16
 *          -# Success : ROK
 */
PRIVATE S16 wrEmmMmePrcMmeCfgUpd
(
U32                          peerId,
S1apPdu                      *pdu
)
{
   S16                       ret = ROK;
   U32                       idx;
   WrMmeCb                   *mmeCb;
   SztInitiatingMsg          *initMsg = &pdu->pdu.val.initiatingMsg;
   SztMMEConfigUpd           *cfgUpd  = &initMsg->value.u.sztMMEConfigUpd;
   SztProtIE_Field_MMEConfigUpdIEs *ie;
#ifdef WR_TEST_CODE
   WrUmmMsgCause  cause;
#endif

   /* Get MME from the peerId                                             */
   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);

   if (mmeCb == NULLP) 
   {
      RLOG1(L_ERROR,"Processing MME Configuartaion Update,"
      "Received Invalid Peer Id [%d]",peerId);
      RETVALUE(RFAILED);
   }
   if (mmeCb->state != WR_MME_UP)
   {
      /* Dont respond with any here as the state is invalid to transmit   */
      /* any response at all. May be an error indication may be sent      */
      RLOG1(L_INFO," S1 Connection not up for MME [%d]",mmeCb->mmeId);
      RETVALUE(RFAILED);
   }
#ifdef WR_TEST_CODE
   if ( wrDntRspMmeCfgUpd == TRUE )
   {
     RLOG0(L_INFO,"Dropping the MME Cfg Upd");
     RETVALUE(ROK);
   }
#endif
   for(idx = 0; idx < cfgUpd->protocolIEs.noComp.val; idx++)
   {
      ie = cfgUpd->protocolIEs.member + idx;
      switch(ie->id.val)
      {
         case Sztid_MMEname:
            /* Currently being ignored                                    */
            break;
         case Sztid_SrvdGUMMEIs:
            ret = wrEmmMmeUpdGummeis(mmeCb, (SztProtIE_Field_S1SetupRespIEs *)ie);
            break;
         case Sztid_RelativeMMECapacity:
            wrEmmMmeUpdRelCap(mmeCb, &ie->value.u.sztRelativeMMECapacity);
            RLOG_ARG0(L_DEBUG,DBG_MMEID,mmeCb->mmeId,
               "Received MME configuration update message, relative"
               "capacities of MME changed");
      }
      if (ret != ROK)
      {
         /* Send a failure response to the peer and go away from here     */
         RETVALUE(ROK);
      }
   }
#ifdef WR_TEST_CODE
   if ( wrTgrMmeCfgUpdFail == TRUE )
   {
      cause.causeTyp = 0;
      cause.causeVal = 0;
      wrEmmMmeSendCfgUpdateFail(mmeCb->mmeId, &cause, 
                                   wrEmmCb.tmrCfg.s1TimeToWait);
      RETVALUE(ROK);
   }
   if ( wrTgrEnbErrInd == TRUE )
   {
      cause.causeTyp = 0;
      cause.causeVal = 0;
      wrEmmMmeSendEnbErrInd(mmeCb->mmeId, &cause, NULLP, NULLP);
      RETVALUE(ROK);
   }
#endif
   for(idx = 0; idx <= mmeCb->numGrpIds; idx++)
   {
      RLOG_ARG1(L_EVENT, DBG_MMEID, mmeCb->mmeId, "[S1AP]:Received MME CONFIGURATION"
         "UPDATE [Relative Capacity:%d]", mmeCb->relCapacity);
   }
   /* Send a success response to the peer as the processing is complete   */
   wrEmmMmeSndMmeCfgUpdAck(mmeCb);
   RETVALUE(ROK);
} /* end of wrEmmMmePrcMmeCfgUpd */

/** @brief This function is used to process the Reset request recieved
 *         from peer node MME.
 *
 * @details This function process the RESET Request message received from MME.
 *
 *     Function: wrEmmMmePrcReset 
 *
 *         Processing steps:
 *         - If the RESET Type IE is not present return failure.
 *         - If the RESET Type is S1 Interface received in the RESET message,
 *           then send a trigger to UMM Module to release all the UE contexts
 *           locally by using the function wrUmmMmeFullRst.
 *         - Otherwise, Trigger the UMM Module to release Partial UE contexts 
 *           locally by using the function wrUmmMmePartRst.
 *         - Finally build and send S1AP: RESET Acknowledgement message to the
 *           peer node(MME) from which EnodeB received RESET Request.
 *
 *@param[in] peerId    : Peer Id from which reset recvd
 *@param[in] pdu       : Reset PDU received
 *@return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PRIVATE S16 wrEmmMmePrcReset
(
U32                          peerId,
S1apPdu                      *pdu
)
{
   TknU8                     *ie = NULLP;
   SztCause                  *cause = NULLP;
   TknU8                     *rstTypeIE = NULLP;
   SztUDatEvnt               uDatEvnt;
   WrMmeCb                   *mmeCb;
   U8                        causeType = 0;
   U8                        causeVal = 0;

   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);
   if (mmeCb == NULLP)
   {
      RLOG1(L_ERROR,"Could not process S1 reset"
      "message due to Invalid Peer Id [%d]",peerId);
      RETVALUE(ROK);
   }

   /* Get the Reset Type IE from the PDU received */
   wrSzGetIE(&(pdu->pdu), Sztid_ResetTyp, &rstTypeIE);
   if(rstTypeIE == NULLP)
   {
      RLOG0(L_ERROR, "rstTypeIE is NULL");
      RETVALUE(RFAILED);
   }

   wrSzGetIE(&(pdu->pdu), Sztid_Cause, &ie);
   if(ie == NULLP)
   { 
      RLOG0(L_ERROR,"Failed to retrive cause");
      RETVALUE(RFAILED);
   }

   cause = (SztCause *)ie;
   wrEmmUpdateKpiCounter(cause);
   
   RLOG_ARG1(L_EVENT, DBG_MMEID, mmeCb->mmeId,
         "[S1AP]:Received S1 RESET [Reset type:%d]",
         ((SztResetTyp *)rstTypeIE)->choice.val);

   if(TRUE == wrS1AlarmHandle(mmeCb, WR_ALARM_ID_S1_RESET))
   {
      causeType = cause->choice.val;
      causeVal = wrPrintS1apCause(cause);
      wrAlarmEvt(WR_ALARM_ID_S1_RESET, WR_ALARM_SEVERITY_MINOR, causeType, causeVal);
   }


   /* Handle the reset depending on the reset type */
   if(((SztResetTyp *)rstTypeIE)->choice.val == RESETTYP_S1_INTF)
   {
      wrUmmMmeFullRst(mmeCb->mmeId);
   }
   else
   {
      /*ccpu00128714: Corrected memory corruption */
      wrUmmMmePartInRst(mmeCb->mmeId,(SztResetTyp *)rstTypeIE);
   }

   /* Build the Reset ACK PDU to send it to peer */
   if ((wrEmmMmeBldResetAck(pdu, &(uDatEvnt.pdu))) != ROK)
   {
      RLOG0(L_ERROR, "Building S1 RESET ACKNOWLEDGE Failed");
      RETVALUE(RFAILED);
   }

   /* Fill the Peer Id value in data event struct */
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val = peerId;

   /* Send the reset ack message to MME */
   if((WrIfmS1apSndMgmtMsg(&uDatEvnt)) != ROK)
   {
      RLOG0(L_ERROR, "Sending S1 RESET ACKNOWLEDGE Failed");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG_ARG0(L_EVENT, DBG_MMEID, mmeCb->mmeId, "[S1AP] Sending S1 RESET ACKNOWLEDGE");
   }

   RETVALUE(ROK);

} /* end of wrEmmMmePrcReset */
/** @brief This function is process the EnodeB Configuration update failure 
 *         messaage which is received from MME.
 *
 * @details This function is is process the EnodeB Configuration update failure
 *         messaage which is received from MME.
 *
 *     Function: wrEmmMmePrcEnbCfgFail
 *
 *         Processing steps:
 *         - Stops the enodeb confguration update response timer.
 *         - If time to wait IE is received in failure reponse message,
 *           ENB starts the time to wait timer to send againg the EnodeB
 *           Configuration Update message.
 *         - Otherwise EnodeB sends an alaram to indicate that insynch of
 *           configuration between MME and ENB.
 *
 * @param[in] peerId: Peer ID  
 * @param[in]    pdu: S1-AP PDU 
 * @return  S16
 *       -# Success : ROK
 */
PRIVATE S16 wrEmmMmePrcEnbCfgFail
(
U32                          peerId,
S1apPdu                      *pdu
)
{
   TknU8       *timeToWait = NULLP;
   U32          wait = 0;
   WrMmeCb      *mmeCb;
   SztCause     *s1cause = NULLP;      
   TknU8        *ie = NULLP;          

   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);
   if(NULLP == mmeCb)
   {
      RLOG1(L_ERROR,"Processing ENB CONFIG FAIL, "
      "Received Invalid PeerId [%d]",peerId);
      RETVALUE(RFAILED);
   }

   /* Stop the response timer */
   wrStopTmr((PTR)(mmeCb->mmeId), WR_TMR_ENB_CFG_RSP);


   /* Decode the Cause value */
   wrSzGetIE(&(pdu->pdu), Sztid_Cause, &ie);
   s1cause = (SztCause *)ie;
   if(NULLP == s1cause)
   {
      RLOG_ARG0(L_ERROR, DBG_MMEID, mmeCb->mmeId,
                "[S1AP]:Received ENB CONFIGURATION UPDATE FAILURE without cause");
      RETVALUE(RFAILED);
   }
   wrEmmUpdateKpiCounter(s1cause);

   /* Check if the MME has included the time to wait IE in the failure 
    * message. If IE is present, start a timer */
   wrSzGetIE(&(pdu->pdu), Sztid_TimeToWait, &timeToWait);
   if(timeToWait != NULLP)
   {
      WR_EMM_MME_GET_TIME_TO_WAIT_VAL(timeToWait->val, wait);
      wrStartTmr((PTR)(mmeCb->mmeId), WR_TMR_ENB_CFG_WAIT, wait);
      RLOG_ARG1(L_EVENT, DBG_MMEID, mmeCb->mmeId,"[S1AP]:Received ENB CONFIGURATION UPDATE FAILURE"
                "[timeToWait:%d]", wait);
   }
   else
   {
      RLOG_ARG0(L_EVENT, DBG_MMEID, mmeCb->mmeId,"[S1AP]:Received ENB CONFIGURATION UPDATE FAILURE"
                " without timeToWait");
      wrStartTmr((PTR)(mmeCb->mmeId), WR_TMR_ENB_CFG_WAIT, 
                       wrEmmCb.tmrCfg.internalNoWaitRetryTmr);
      /* Send Alarm to SM about the config update failure  */
      RLOG0(L_ERROR,"[ALARM] Sending ENB_CFGUPD_FAIL to OAM" );
      wrSendLmAlarm(LCM_CATEGORY_LYR_SPECIFIC, LWR_EVENT_ENB_CFGUPD_FAIL,
                                               LWR_CAUSE_MME_SYNC_LOST);
   }
   wrPrintS1apCause(s1cause);
   RETVALUE(ROK);
} /* end of wrEmmMmePrcEnbCfgFail */

/** @brief This function process the S1AP: EnodeB Configuration Update
 *         Acknowledgemnt Message.
 *
 * @details This function process the S1AP: EnodeB Configuration Update
 *          Acknowledgement Message which is received from MME.
 *
 *     Function: wrEmmMmePrcEnbCfgUpd 
 *
 *         Processing steps:
 *         - Stop EnodeB Configuration Response timer. 
 *         - Send an Alarm to SM to convey the EnodeB Configuration Update
 *           Success.
 *
 * @param[in] peerId: Peer ID.
 * @param[in]    pdu: S1-AP PDU.
 * @return  S16
 *       -# Success : ROK
 */
PRIVATE S16 wrEmmMmePrcEnbCfgUpd
(
U32                          peerId,
S1apPdu                      *pdu
)
{
   WrMmeCb     *mmeCb;
   /* Drop the ACK PDU                                                    */
   UNUSED(pdu);
#ifdef WR_TEST_CODE
  if ( wrTgrEnbCfgUpdRtx == TRUE )
  {
    /* Do Nothing and return */
    /* Verify the CfgUpd Retransmission */
    RLOG0(L_INFO,"Dropping ENB CfgUpd ACK");
    RETVALUE(ROK);
  }
#endif
  

   /* Stop the response timer                                             */
   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);
   if(NULLP == mmeCb)
   {
      RLOG1(L_ERROR,"Processing ENB CONFIG UPDATE,"
      "Received Invalid Peer Id [%d]",peerId);
      RETVALUE(RFAILED);
   }

   mmeCb->cfgUpdRtxCnt = 0;
   wrStopTmr((PTR)mmeCb->mmeId, WR_TMR_ENB_CFG_RSP);
   /* Send Alarm to SM about the config update procedure completion       */
   RLOG_ARG1(L_EVENT, DBG_MMEID, mmeCb->mmeId,
      "[S1AP]:Received ENB CONFIGURATION UPDATE ACKNOWLEDGE [Relative Capacity:%d]",
       mmeCb->relCapacity);

   RLOG0(L_INFO,"[ALARM] Sending ENB_CFGUPD_SUCCESS to OAM" );
   wrSendLmAlarm(LCM_CATEGORY_LYR_SPECIFIC, LWR_EVENT_ENB_CFGUPD_SUCCESS,
                                            LCM_CAUSE_UNKNOWN);
   RETVALUE(ROK);
} /* end of wrEmmMmePrcEnbCfgUpd */

/** @brief This function process the S1AP: S1 Setup Failure message.
 *
 * @details This function will process the S1-AP: S1 Setup Failure message.
 *
 *     Function: wrEmmMmePrcSetupFail
 *
 *         Processing steps:
 *         - Move the MME(mmeCb) state to WR_MME_INITED.
 *         - If the failure message contains Time to Wait IE, the start timer
 *           to send again S1 Setup Request message to that MME after "time to
 *           wait" value time.
 *
 *
 * @param[in] peerId: Peer ID.
 * @param[in]    pdu: S1-AP PDU 
 * @return  S16
 *       -# Success : ROK
 */
PRIVATE S16 wrEmmMmePrcSetupFail
(
U32                          peerId,
S1apPdu                      *pdu
)
{
   WrMmeCb      *mmeCb;
   TknU32       *s1TimeToWait = NULLP;  /* Fix for ccpu00128730 */
   TknU8        *ie = NULLP;            /* Fix for ccpu00128730 */
   U32          wait = 0;
   S16          ret = ROK;
   SztCause     *cause = NULLP;
   U8           causeType = 0;
   U8           causeVal = 0;

   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);

   if(NULLP == mmeCb)
   {
      RLOG1(L_ERROR,"Processing S1 Setup Fail,"
      "Received Invalid Peer Id [%d]",peerId);
      RETVALUE(RFAILED);
   }

   /* Need to raise an alarm for SETUP FAILURE                            */
   mmeCb->state = WR_MME_INITED;

   wrStopTmr((PTR)mmeCb, WR_TMR_MME_SETUP_RSP);

   wrSzGetIE(&(pdu->pdu), Sztid_Cause, &ie);

   if(ie == NULLP)
   { 
      RLOG0(L_ERROR,"Failed to retrive cause");
      RETVALUE(RFAILED);
   }

   RLOG_ARG0(L_EVENT, DBG_MMEID, mmeCb->mmeId,"[S1AP]:Received S1 SETUP FAILURE");

   cause = (SztCause *)ie;
   causeType = cause->choice.val;
   wrEmmUpdateKpiCounter(cause);/*128869*/
   wrPrintS1apCause(cause);
   if(CAUSE_MISC == cause->choice.val)
   {
        causeVal = cause->val.misc.val;
        /*ccpu00128869*/
        switch(causeVal)
        {
            case SztCauseMiscunknown_PLMNEnum:

               /*ccpu00128869 :S1 setup request shouldn't be sent again.
                *Hence the state is locked  */ 
               mmeCb->adminState = WR_MME_LOCKED;

               RLOG0(L_FATAL,"[ALARM] Sending S1_SETUP_FAIL to OAM ");
               /*Alarm is raised towards OAM*/
               wrAlarmEvt(WR_ALARM_ID_S1_SETUP_FAIL, WR_ALARM_SEVERITY_MAJOR, 
                          causeType, causeVal);
               wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_S1CON_SET_FAIL,
                  LCM_CAUSE_UNKNOWN);

               /*Aborting SCTP connection*/
#ifdef SZTV2
               if((wrEmmMmeAbortMsg(mmeCb)) != ROK)
               {
                  RLOG0(L_ERROR, "wrEmmMmePrcSetupFail: Aborting SCTP connection failed");
                  RETVALUE(RFAILED);
               }
#endif
                break;           
         default:
         RLOG0(L_ERROR,"Invalid case");
         break;
        }
   }
   RLOG0(L_FATAL,"[ALARM] Sending S1_SETUP_FAIL to OAM ");
   wrAlarmEvt(WR_ALARM_ID_S1_SETUP_FAIL, WR_ALARM_SEVERITY_MAJOR, causeType, causeVal);

   /* Check if the MME has included the time to wait IE in the failure 
    * message. If IE is present, start a timer */
   /* Fix for ccpu00128730 - addressing time to wait IE*/
   wrSzGetIE(&(pdu->pdu), Sztid_TimeToWait, &ie);
   s1TimeToWait = (TknU32 *)ie;
   if(s1TimeToWait != NULLP)
   {
      WR_EMM_MME_GET_TIME_TO_WAIT_VAL(s1TimeToWait->val, wait)
         ret = wrStartTmr((PTR)mmeCb, WR_TMR_MME_SETUP_WAIT, wait);
      if ( ret != ROK )
      {
         RLOG0(L_ERROR,"Unable to start the timer (WR_TMR_MME_SETUP_WAIT)");
      }
   }
   else
   {
      RLOG0(L_DEBUG,"Received S1-Setup  Failure without timeToWait");
      /* Retry After some time */
      ret = wrStartTmr((PTR)mmeCb, WR_TMR_MME_SETUP_WAIT,
            wrEmmCb.tmrCfg.internalNoWaitRetryTmr );
      if ( ret != ROK )
      {
         RLOG0(L_ERROR,"Unable to start the timer (WR_TMR_MME_SETUP_WAIT)");
      }
   }
   RETVALUE(ROK);
} /* end of wrEmmMmePrcSetupFail */

/** @brief This function process the S1AP Unsuccessful Outcome messages.
 *
 * @details This function process the S1AP Unsuccessful Outcome messages like 
 *          S1-Setup Response Fail, EnodeB Configuration Update failure meesages.
 *
 *     Function: wrEmmMmePrcUnsuccPdu
 *
 *         Processing steps:
 *         - Based on the Unsuccessful S1AP PDU's procedure code, this funcation
 *           calls the respective functions to process the failure messages.
 *
 * @param[in] peerId: Peer ID.
 * @param[in]    pdu: S1-AP PDU 
 * @return    -# Success: ROK
 *            -# Failure: RFAILED
 */
PRIVATE S16 wrEmmMmePrcUnsuccPdu
(
U32                          peerId,
S1apPdu                      *pdu
)
{
   SztUnsuccessfulOutcome    *unsuccMsg = &pdu->pdu.val.unsuccessfulOutcome;
   S16                       ret        = ROK;

   switch(unsuccMsg->procedureCode.val)
   {
      case Sztid_S1Setup:
         ret = wrEmmMmePrcSetupFail(peerId, pdu);
         break;
      case Sztid_ENBConfigUpd:
         ret = wrEmmMmePrcEnbCfgFail(peerId, pdu);
         break;
   }
   RETVALUE(ret);
} /* end of wrEmmMmePrcUnsuccPdu */

/** @brief This function process the Reset Acknowledge message.
 *
 * @details  This function process the Reset Acknowledge message.
 *
 *     Function: wrEmmMmePrcResetRsp
 *
 *         Processing steps:
 *         - 
 *
 * @param[in] peerId: Peer ID
 * @param[in]   pdu: S1-AP PDU 
 * @return  S16
 *       -# Success : ROK
 */
PRIVATE S16 wrEmmMmePrcResetRsp
(
U32                          peerId,
S1apPdu                      *pdu
)
{

   WrMmeCb     *mmeCb;
   U32         idx;
   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);
  
   if(NULLP == mmeCb)
   {
      RLOG1(L_ERROR,"Processing S1 RESET Response,"
      "Received Invalid Peer Id [%d]",peerId);
      RETVALUE(RFAILED);
   }

   mmeCb->resetRtxCnt = 0;
   
   wrStopTmr((PTR)(mmeCb->mmeId), WR_TMR_RST_RSP);
   /* for all the cells send delete if the state is Cell Reset*/
   RLOG_ARG0(L_EVENT, DBG_MMEID, mmeCb->mmeId, 
      "[S1AP]:Received S1 RESET ACKNOWLEDGE");

   for(idx = 0; idx < WR_EMM_MAX_CELLS; idx++) 
   {
       if ((wrEmmCb.cellCb[idx] != NULLP) &&  
           (WR_CELL_STATE_RESET == wrEmmCb.cellCb[idx]->cellState) && (--(wrEmmCb.cellCb[idx]->noMmeToReset) == 0))
       {
          wrCb.resetCmfState |= WR_S1_RESET_DONE; 
       }
       if(WR_ALL_RESET_DONE == wrCb.resetCmfState)
       {
          wrCb.resetCmfState = WR_INIT_STATE_NONE;
          /* This is done as the CELL RESET State is Common 
           * for Partial Reset and Full Reset Case 
           */
          wrCb.parResetCmfState = WR_INIT_STATE_NONE;
          if(ROK != wrFillCtfCellDelCfgReq(wrEmmCb.cellCb[idx]->cellId))
          {
             RLOG_ARG0(L_ERROR,DBG_CELLID,wrEmmCb.cellCb[idx]->cellId,
                "wrFillCtfCellDelCfgReq failed.");     
             RETVALUE(RFAILED); 
          }
       } 
   }
   RETVALUE(ROK);
} /* end of wrEmmMmePrcResetRsp */

/** @brief This function process the Successful Outcome S1AP PDUs.
 *
 * @details This function process the Successful Outcome S1AP PDUs like
 *          S1 Setup Response, EnodeB Configration Update ack or Reset
 *          Acknowledge messages.
 *
 *     Function: wrEmmMmePrcSuccPdu
 *
 *         Processing steps:
 *         - Based on the Succesful Outcome S1AP PDU's procedure code call the
 *           corresponding response processing function.
 *
 * @param[in] peerId: Peer ID.
 * @param[in]    pdu: S1-AP PDU
 * @return  S16
 *       -# Success : ROK
 *       -# Failure : RFAILED
 */
PRIVATE S16 wrEmmMmePrcSuccPdu
(
U32                          peerId,
S1apPdu                      *pdu
)
{
   SztSuccessfulOutcome      *succMsg = &pdu->pdu.val.successfulOutcome;
   S16                       ret = ROK;

   switch(succMsg->procedureCode.val)
   {
      case Sztid_S1Setup:
         ret = wrEmmMmePrcSetupRsp(peerId, pdu);
         break;
      case Sztid_Reset:
         ret = wrEmmMmePrcResetRsp(peerId, pdu);
         break;
      case Sztid_ENBConfigUpd:
         ret = wrEmmMmePrcEnbCfgUpd(peerId, pdu);
         break;
      default:
         RLOG0(L_ERROR," Invalid IE.");
         ret = RFAILED;
         break;
   }

   RETVALUE(ret);
} /* end of wrEmmMmePrcSuccPdu */

/** @brief This function will process the S1-AP Status Indication.
 *
 * @details his function will process the S1-AP Status Indication.
 *
 *     Function: wrEmmMmePrcStaInd
 *
 *         Processing steps:
 *         - Start timer to esatblish S1 Setup with MME when the timer expires.
 *         - If MME state is already UP, then only rlease all local contexts 
 *           with that MME.Then finally move the MME state to INIT.
 *
 * @param[in] sztSta: S1-AP Status Indication. 
 * @return  S16
 *       -# Success : ROK
 */
PUBLIC S16 wrEmmMmePrcStaInd
(
SztStaInd                    *sztSta
)
{
   WrMmeCb                   *mmeCb;
   U32                       size = 0;

   /* Verify if Assoc is down; in that case RRC Connection Release
    * is sent to all UEs and hence leading to release of resources 
    * */
   if(sztSta->type == SZT_STA_TYPE_ASSOC)
   {
      if(sztSta->status == SZT_STATUS_ASSOC_DOWN)
      {
         U32                 peerId;

         RLOG1(L_INFO,"Assoc. Down Reason (SZT) [%x]", 
            sztSta->reason);
         SRegInfoShow(0, &size);
        /*127802*/
         RLOG0(L_INFO,"###################Memory Status###################");
         peerId = sztSta->peerId.val;
         mmeCb = wrEmmMmeGetMmeByPeerId(peerId);
         if (mmeCb == NULLP)
         {
            RLOG1(L_ERROR,"Processing S1AP Status Ind,"
            "Received Invalid Peer Id [%d]",peerId);
            RETVALUE(ROK);
         }

         wrAlarmEvt(WR_ALARM_ID_SCTP_LNK_FAIL, WR_ALARM_SEVERITY_CRITICAL, NULLP, NULLP);


         if (mmeCb->state == WR_MME_UP)
         {
            /* Fix for ccpu00131680*/
            mmeCb->mmeOvldFlag = FALSE;
            /* Start a timer for attempting to connect again */
            if (wrStartTmr((PTR)mmeCb, WR_TMR_MME_SETUP_WAIT,
                     WR_MME_CON_WAIT_TIMER) != ROK)
            {
               RLOG0(L_ERROR,"Unable to start timer");
            }
            RLOG0(L_FATAL,"[ALARM] Sending S1CON_SET_FAIL to OAM" );

            wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_S1CON_SET_FAIL,
                  LCM_CAUSE_UNKNOWN);
            wrUmmMmeFullRst(mmeCb->mmeId);
            wrEmmMmeDown(mmeCb);
         }
      }
   }

   RETVALUE(ROK);
} /* end of wrEmmMmePrcStaInd */

/* TODO - Need to add appropriate declaration for this.                   */
/** @brief This function handler is to handle the time to wait timer expiration
 *         to send the S1 Setup request message to MME.
 *
 * @details This function is used to build and send again the S1AP: S1 Setup
 *          Request message to MME in case of eNodeB receives S1 Setup Failure 
 *          message with time to wait IE.
 *
 *     Function: wrEmmMmeSetupWaitTmrExpiry
 *
 *         Processing steps:
 *         - Invokes the wrEmmMmeSetup to build and send the 
 *           S1AP: S1 Setup Request message to MME.
 *         - This function is invoked by the following module
 *           # TIMER
 *
 * @param[in] mmeCb: MME Control Block
 * @return  S16
 *       -# Success : ROK
 */
PUBLIC S16 wrEmmMmeSetupWaitTmrExpiry
(
PTR                         mmeCb
)
{
   /* Abort the association using AbortS1Req */
#ifdef SZTV2
   if((wrEmmMmeAbortMsg((WrMmeCb *)mmeCb)) != ROK)
   {
      RLOG2(L_ERROR, "Aborting SCTP connection For MME [%d],"
      "and Peer[%d]",((WrMmeCb*)mmeCb)->mmeId,((WrMmeCb*)mmeCb)->peerId);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
#endif
} /* end of wrEmmMmeSetupWaitTmrExpiry */

/** @brief This function handler is to handle the time to wait timer expiration
 *         to send the EnodeB Configuration Update message to MME.
 *
 * @details This function is used to build and send again the S1AP:eNodeB
 *          Configuration Update message to MME in case of eNodeB 
 *          receives EnodeB Confiuration Update failure message with time to
 *          wait IE.
 *
 *     Function: wrEmmMmeEnbCfgUpdWaitTmrExp 
 *
 *         Processing steps:
 *         - Invokes the wrEmmMmeSendCfgUpdate to build and send the 
 *           S1AP:EnodeB Configuration Update message to MME.
 *         - This function is invoked by the following module
 *           # TIMER
 *
 * @param[in] peerId: Peer ID.
 * @return  S16
 *       -# Success : ROK
 */
PUBLIC S16 wrEmmMmeEnbCfgUpdWaitTmrExp
(
U32                          mmeId
)
{
   wrEmmMmeSendCfgUpdate(mmeId);
   RETVALUE(ROK);
} /* end of wrEmmMmeEnbCfgUpdWaitTmrExp */

/** @brief This function handler is to handle the enodeb configuration update
 *         response timer expires.
 *
 * @details This function is used to build and send again the S1AP:eNodeB
 *          Configuration Update message to MME in case of eNodeB 
 *          doesn't receive EnodeB Confiuration Update Ack/failure 
 *          response with in time.
 *
 *     Function: wrEmmMmeEnbCfgRspTmrExp
 *
 *         Processing steps:
 *         - Invokes the wrEmmMmeSendCfgUpdate to build and send the 
 *           S1AP:EnodeB Configuration Update message to MME.
 *         - This function is invoked by the following module
 *           # TIMER
 *
 * @param[in] peerId: Peer ID
 * @return  S16
 *       -# Success : ROK
 */
PUBLIC S16 wrEmmMmeEnbCfgRspTmrExp
(
U32                          mmeId
)
{
   WrMmeCb                   *mmeCb;

   mmeCb = WR_EMM_MME_GET_MME(mmeId);
   if (mmeCb == NULLP)
   {
      RLOG1(L_ERROR,"Invalid MME ID [%d]",mmeId);
      RETVALUE(RFAILED);
   }
   if ((wrEmmCb.tmrCfg.s1RetryCount == 0xFF) 
       || (mmeCb->cfgUpdRtxCnt < wrEmmCb.tmrCfg.s1RetryCount))
   {
      /* Resend the update request                                           */
      wrEmmMmeSendCfgUpdate(mmeId);
      mmeCb->cfgUpdRtxCnt++;
   }
   else
   {
     mmeCb->cfgUpdRtxCnt = 0;
   }
   /* Resend the update request                                           */
   RETVALUE(ROK);
} /* end of wrEmmMmeEnbCfgRspTmrExp */

/** @brief This function handler is to handle the Reset
 *         response timer expires.
 *
 * @details This function is used to Resend the  Reset
 *          message to MME in case of eNodeB 
 *          doesn't receive reset response with in time.
 *
 *     Function: wrEmmMmeResetRspTmrExp
 *
 *         Processing steps:
 *         - Invokes the wrEmmMmeSendCfgUpdate to build and send the 
 *           S1AP:EnodeB Configuration Update message to MME.
 *         - This function is invoked by the following module
 *           # TIMER
 *
 * @param[in] peerId: Peer ID
 * @return  S16
 *       -# Success : ROK
 */
PUBLIC S16 wrEmmMmeResetRspTmrExp
(
U32                          mmeId
)
{
   SztUDatEvnt               uDatEvnt;
   WrMmeCb                   *mmeCb;
   WrEmmS1Reset              *rst;

   mmeCb = WR_EMM_MME_GET_MME(mmeId);
   if (mmeCb == NULLP)
   {
      RLOG1(L_ERROR,"Invalid MME ID [%d]",mmeId);
      RETVALUE(RFAILED);
   }

   if ((WR_EMM_RST_RTX_CNT == 0xFF) 
       || (mmeCb->resetRtxCnt < WR_EMM_RST_RTX_CNT))
   {
      rst = &(mmeCb->resetRtxInputs);
      RLOG_ARG2(L_DEBUG,DBG_MMEID, mmeCb->mmeId,
         "Re-Sending S1 MME RESET REQUEST, Reason = Timer expiry,"
         "Cause = [%d], Reset type = [%d]", 
         rst->causeVal ,rst->rstType);

      if(rst->rstType  == RESETTYP_S1_INTF)
      {
         if (wrEmmMmeBldResetReq(&(uDatEvnt.pdu), rst->rstType, 0, rst->causeType,
             rst->causeVal, NULLP) != ROK)
        {
           RLOG0(L_ERROR, "Failed To Build S1 Full Reset"); 
           RETVALUE(RFAILED);
        }
      }
      else
      {
         if (wrEmmMmeBldResetReq(&(uDatEvnt.pdu), rst->rstType, rst->nmbUes, 
              rst->causeType, rst->causeVal,(WrEmmS1ConId *) &(rst->conIds)) != ROK)
         {
           RLOG0(L_ERROR, "Failed To Build S1 Partial Reset");
           RETVALUE(RFAILED);
         }
      }

      uDatEvnt.transId.pres = PRSNT_NODEF;
      uDatEvnt.transId.val = 1;
      uDatEvnt.peerId.pres = PRSNT_NODEF;
      uDatEvnt.peerId.val = mmeCb->peerId;
      /* Start the timer */
      wrStartTmr((PTR)mmeId, WR_TMR_RST_RSP,
                               wrEmmCb.tmrCfg.s1ResetTmr);
      /* Send the pdu to the MME */
      if((WrIfmS1apSndMgmtMsg(&uDatEvnt)) != ROK)
      {
         RETVALUE(RFAILED);
      }
      mmeCb->resetRtxCnt++;
   }
   else
   {
     mmeCb->resetRtxCnt = 0;
   }
   /* Resend the update request                                           */
   RETVALUE(ROK);
} /* end of wrEmmMmeEnbCfgRspTmrExp */


/** @brief This function returns MME Overload Staus .
 *
 * @details This function returns MME Overload Staus.If MME's
 *          Overload Staus is SET to TRUE, function
 *          Returns "TRUE" else it returns "FALSE"
 *
 *     Function: wrEmmGetOvldStatus
 *
 * @param[in] mmeId: U32
 * @return  S16
 *       -# Success : TRUE
 *       -# Failure : FALSE
 */

PUBLIC S16 wrEmmGetOvldStatus(U32 mmeId)
{
   WrMmeCb   *wrMmeCb = NULLP; 

   /*-- Get MME CB using MME ID --*/  
   wrMmeCb = wrEmmCb.mmeCont.mmes[mmeId];
   if( wrMmeCb == NULL)
   {
      RLOG1(L_ERROR,"Invalid MME ID [%d] While Getting Overload Status",mmeId); 
      RETVALUE(FALSE);
   }      
   RETVALUE(wrMmeCb->mmeOvldFlag);
}






/** @brief This function returns MME Operational Staus.
 *
 * @details This function returns MME Operational Staus.If MME is
 *          UP, function Returns "TRUE" else it returns "FALSE"
 *
 *     Function: wrEmmGetMmeStatus
 *
 * @param[in] mmeId: U32
 * @return  S16
 *       -# Success : TRUE
 *       -# Failure : FALSE
 */

PUBLIC Bool wrEmmGetMmeStatus(U32 mmeId)
{
   WrMmeCb   *wrMmeCb = NULLP;

   /*-- Get MME CB using MME ID --*/
   wrMmeCb = wrEmmCb.mmeCont.mmes[mmeId];
   if( wrMmeCb == NULL)
   {
      RETVALUE(FALSE);
   }

   if(WR_MME_UP == wrMmeCb->state)
      RETVALUE(TRUE);
   else
      RETVALUE(FALSE);
}


/**
 *  @brief This function is used to derive MME using loadsharing algorithms.
 *
 *  @details
 *      Function wrEmmGetLdBalMme
 *          Processing Steps:
 *              This function tries to derive MME Id based upon PLMN .
 *              When PLMN is used to derive  MMEId , we use two different
 *              load sharing algorithms to derive MME Id.
 *
 *  @param[in] plmnId : Selected PLMN Identity
 *  @param[out] mme : Selected MME Id
 *  @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
**/
PUBLIC S16 wrEmmGetLdBalMme
(
   WrRrcConTransCb           *rrccon
)
{
   WrPlmnCbMmeList   mmeList;
   S32               ret = RFAILED;
   WrPlmnId          *plmnId = rrccon->plmnId;
#ifndef MME_LDBAL
   U8               mmeIdx = 0;
   U8               relCap = 0;
   WrMmeId          *mme    = &rrccon->mmeId;
   WrMmeCb          *mmeCb    = NULLP;
   U32              ovrldCnt;
   U32              selPlmnOvrld;
#endif

   /*-Now we need to make selection based upon Selected PLMN value-*/
   ret = wrEmmPlmnGetMmeIdList(&mmeList, *plmnId);
   if(ROK != ret)
   {
      /*-TODO-*/     
      RETVALUE(RFAILED);
   }

#ifdef MME_LDBAL
#ifdef DG_SG
      ret = wrEmmGrpIdGetMmeIdList(&mmeList,rrccon );
#else
      ret = wrEmmGetNextMmeCbPrb(&mmeList, rrccon );
#endif
#else

    {
      ret = RFAILED;
      for(mmeIdx = 0; mmeIdx < mmeList.numOfMme; mmeIdx++)
      {
         selPlmnOvrld = FALSE;
         /* Get the MME control block */
         mmeCb = WR_EMM_MME_GET_MME(mmeList.mmeIdList[mmeIdx]);
         /* Check for MME eligibility for LD algorithm */
         if ((mmeCb->state == WR_MME_UP) && (mmeCb->relCapacity > 0) && (!(mmeCb->mmeOvldFlag)))
         {
            /* Check is the selected PLMN is overloaded and if so, will not 
             * be selecting the MME
             */
            for(ovrldCnt = 0; ovrldCnt < mmeCb->numOvrldGummei; ovrldCnt++)
            {
               if(TRUE == (wrUtlPlmnsEqual(plmnId, &(mmeCb->ovrldGummeiLst[ovrldCnt].plmnId))))
               {
                  selPlmnOvrld = TRUE;
                  break;
               }
            }
            if ((relCap < mmeCb->relCapacity) && (FALSE == selPlmnOvrld))
            {
               /* Choose the MME with Highest relative Capacity in current priority group */
               *mme = mmeCb->mmeId;
               relCap = mmeCb->relCapacity;
               ret = ROK;
            }
         }
      }
   }
#endif
   RETVALUE(ret);
}
/*
 *   @brief This function is used to select MME based on s-tmsi sent by UE
 *          Assumption: It is the operator's responsibility to configure
 *          orthogonal MMECs for MMEs across pools.
 *   
 *   @details
 *       Function: wrEmmGetMmeFrmStmsi
 *       Processing steps:
 *            This function derives the MMEs list serving PLMN as provided  
 *            in the selected PLMN Index by the UE. Then match the MMECs of 
 *            every MME with the MMEC provided in s-Tmsi.
 *
 *   @param[in]  mmeCode: MMEC from s-Tmsi
 *               *causeTauRequiredSet: TAU required flag
 *               establishCause: RRC Establishment cause
 *               *rrccon: RRC Transaction CB   
 *   @param[out] rrccon->mmeId: MME ID of the selected MME
 *   @return S16
 *           -# Success : ROK
 *           -# Failure : RFAILED
 **/
PUBLIC S16 wrEmmGetMmeFrmStmsi
(
 U8 mmeCode,
 U8 *causeTauRequiredSet,
 U32 establishCause,
 WrRrcConTransCb           *rrccon
)
{

   WrPlmnCbMmeList   mmeList;
   U32               mmeIdx;
   WrMmeCb           *mmeCb;
   S16               ret;
   U8                idx;
   WrPlmnId          *plmnId=rrccon->plmnId;
   U32               ovrldCnt;
   U32               mmeCodeOvrld;

   /*Retrieve MME ID list based on selected PLMN Index*/
   ret = wrEmmPlmnGetMmeIdList(&mmeList,*plmnId);
   if(ROK != ret)
   {
      RLOG0(L_ERROR,"Failed to retreive MME List for");  
      RLOG3(L_ERROR,"MCC [%d] [%d] [%d]",plmnId->mcc[0],plmnId->mcc[1],
            plmnId->mcc[2]);  
      RLOG3(L_ERROR,"MNC [%d] [%d] [%d]",plmnId->mnc[0],plmnId->mnc[1],
            plmnId->mnc[2]);  
      *causeTauRequiredSet = TRUE;
      RETVALUE(RFAILED);
   }
   for(mmeIdx = 0; mmeIdx < mmeList.numOfMme; mmeIdx++)
   {
      mmeCodeOvrld = FALSE;
      /* Get the MME control block */
      mmeCb = WR_EMM_MME_GET_MME(mmeList.mmeIdList[mmeIdx]);
      if(NULLP == mmeCb)
      {
         RLOG0(L_ERROR,"mmeCb is NULL");
         continue;
      }
      /* Check for MME's status */
      if(WR_MME_UP == mmeCb->state)
      { 
         for(idx = 0;idx < mmeCb->numCodes;idx++)
         {
            if(mmeCode == mmeCb->codes[idx])
            {
               /* Check if the slected MME Code is overloaded, if yes,
                * reject the connection request
                */
               for(ovrldCnt = 0; ovrldCnt < mmeCb->numOvrldGummei; ovrldCnt++)
               {
                  if(mmeCode == mmeCb->ovrldGummeiLst[ovrldCnt].mmeCode)
                  {
                     mmeCodeOvrld = TRUE;
                     break;
                  }
               }
               if((TRUE == mmeCb->mmeOvldFlag) || (TRUE == mmeCodeOvrld))
               {
                  if(TRUE == wrGetMmeRejectStatus(establishCause,mmeCb->mmeId))
                  {
                     RLOG0(L_ERROR,"Selected MME is Overloaded. Releasing the UE"
                           "with loadBalancingTAU");
                     *causeTauRequiredSet = TRUE;
                     RETVALUE(RFAILED);
                  }
               }
               rrccon->mmeId = mmeCb->mmeId; 
               rrccon->gummei.mmec = mmeCode;
               rrccon->gummei.mmeGrpId = mmeCb->groupIds[idx];
               RLOG_ARG0(L_DEBUG, DBG_MMEID, rrccon->mmeId,
                     "Selected MME using S-TMSI");
               RETVALUE(ROK);
            }
         } 

      }
   }
   RLOG0(L_ERROR,"MME selection using S-TMSI failed!");
   *causeTauRequiredSet = TRUE;
   RETVALUE(RFAILED);
}
/*
 *
 *      Fun:   wrGetMmeRejectStatus
 *
 *      Desc:  Function compares RRC Connection Establishment cause with MME Overload A
 ction
 *
 *      Ret:   Returns MME Accept Status
 *
 *      Notes: None
 *
 *      File:
 *
 */


PUBLIC S16 wrGetMmeRejectStatus
(
 U32       establishmentCause,
 U32       mmeId
)
{

   /*-TODO: Use Correct Function to get MME CB-*/
   WrMmeCb   *mmeCb = NULLP;
   Bool mmeRejectStatus = FALSE;

   mmeCb =  wrEmmCb.mmeCont.mmes[mmeId];

   if(!mmeCb)
      RETVALUE(ROK);

   /* As per the spec 36.413 v9.7.0 sec 8.7.6.2 */ 
   switch(mmeCb->overloadAction)
   {
      case SztOverloadActionreject_non_emergency_mo_dtEnum:
         {
            if ((establishmentCause == NhuEstablishmentCausemo_DataEnum) ||
                (establishmentCause == NhuEstablishmentCausedelayTolerantAccess_v1020Enum))
            {
               mmeRejectStatus = TRUE;
            }
         }
         break;
      case SztOverloadActionreject_rrc_cr_signallingEnum:
         {
            if ((establishmentCause == NhuEstablishmentCausemo_DataEnum) ||
                (establishmentCause == NhuEstablishmentCausemo_SignallingEnum)
                || (establishmentCause == 
                          NhuEstablishmentCausedelayTolerantAccess_v1020Enum))
            {
               mmeRejectStatus = TRUE;
            }
         }
         break;
      case SztOverloadActionpermit_emergency_sessions_and_mobile_terminated_services_onlyEnum:
         {
            if (!((establishmentCause == NhuEstablishmentCauseemergencyEnum) ||
                (establishmentCause == NhuEstablishmentCausemt_AccessEnum)))
            {
               mmeRejectStatus = TRUE;  
            }
         }
         break;
         /* Handling of new overload causes which will be received from MME */
      case SztOverloadActionpermit_high_priority_sessions_and_mobile_terminated_services_onlyEnum:
         {
            if(!((establishmentCause == 
                  NhuEstablishmentCausehighPriorityAccessEnum) ||
                (establishmentCause == NhuEstablishmentCausemt_AccessEnum)))
            {
               mmeRejectStatus = TRUE;
            }
         }
         break;
      case SztOverloadActionreject_delay_tolerant_accessEnum:
         {
            if(establishmentCause ==
                   NhuEstablishmentCausedelayTolerantAccess_v1020Enum)
            {
               mmeRejectStatus = TRUE;
            }
         }
         break;
   }
   RETVALUE(mmeRejectStatus);
}
/**
 *  @brief This function is used to process the Err Indication received from MME 
 *
 *  @details
 *      Function wrEmmMmePrcErrInd 
 *          Processing Steps:
 *
 * @param[in]    peerId: Peer ID  
 * @param[in]    pdu: S1-AP PDU 
 * @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
**/

PUBLIC S16 wrEmmMmePrcErrInd
(
U32       peerId,
S1apPdu   *pdu
)
{
   TknU8           *ie = NULLP;
   SztCause        *cause = NULLP;
   WrEmmS1ConId     conId[1];
   TknU8           *mmeIdTkn = NULLP;
   TknU8           *enbIdTkn = NULLP;
   WrMmeCb         *mmeCb;
   U8              causeType = 0;
   U8              causeVal = 0;

   TRC2(wrEmmMmePrcErrInd);

   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);
   if (mmeCb == NULLP)
   {
      RLOG1(L_ERROR,"Proessing S1 Error Ind, Received Invalid Peer Id [%d]",peerId); 
      RETVALUE(RFAILED);
   }

   wrSzGetIE(&(pdu->pdu), Sztid_Cause, &ie);

   if(ie == NULLP)
   { 
      RLOG0(L_ERROR,"Failed to retrive cause");
      RETVALUE(RFAILED);
   }
   cause = (SztCause *)ie;
   /*ccpu00131043*/
   wrEmmUpdateKpiCounter(cause);
   RLOG0(L_ERROR,"[ALARM] Sending ERR_IND_RCVD to OAM" );
   wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_ERR_IND_RCVD,
         cause->val.radioNw.val);
   RLOG_ARG0(L_EVENT, DBG_MMEID, mmeCb->mmeId, "[S1AP] Received S1 ERROR INDICATION");
   causeVal = wrPrintS1apCause(cause);

   if(TRUE == wrS1AlarmHandle(mmeCb, WR_ALARM_ID_S1_ERROR_IND))
   {
      cause = (SztCause *)ie;
      causeType = cause->choice.val;
      wrAlarmEvt(WR_ALARM_ID_S1_ERROR_IND, WR_ALARM_SEVERITY_WARNING, causeType, causeVal);
   }


   /* If the error indication message has come for the specific UE, then
    * release that UE context by sending the RESET message */
   wrSzGetIE(&(pdu->pdu), Sztid_MME_UE_S1AP_ID, &mmeIdTkn);
   wrSzGetIE(&(pdu->pdu), Sztid_eNB_UE_S1AP_ID, &enbIdTkn);
   if ( enbIdTkn != NULLP )
   { 
      conId[0].conId = enbIdTkn->val;
      conId[0].type = WR_S1AP_ENB_ID;
      wrUmmMmePartRst(mmeCb->mmeId, conId, 1);
   }
   else if ( mmeIdTkn != NULLP )
   {
      conId[0].conId = mmeIdTkn->val;
      conId[0].type = WR_S1AP_MME_ID;
      wrUmmMmePartRst(mmeCb->mmeId, conId, 1);
   } 
   RETVALUE(ROK);
}
#ifdef MME_LDBAL
/**
 *  @brief This function is used to derive MME based upon load sharing Algorithm 2 
 *  @details
 *      Function: wrEmmGetNextMmeCbPrb 
 *          Processing Steps: This function tries to select best possible MME based upon 
 *          algorithm. Here it tries to find a MME with best selection  
 *          probability.
 *
 *  @param[in] wrPlmnCb  : PLMN Cb for which MME Id has to be derived.
 *  @return  WrMmeCb*
 *        -# Success : WrMmeCb*
 *        -# Failure : NULLP
**/
PRIVATE S16  wrEmmGetNextMmeCbPrb(WrPlmnCbMmeList *mmeLst, WrRrcConTransCb *rrccon)
{
   S16        ret = RFAILED;
   WrMmeCb* mmeCb = NULLP;
   U16      mmeGrpList[WR_MAX_MMES];
   U32      idx, idx1 = 0;
   U32      mmeIdx = 0, mmeIdx1 = 0;
   U32      mmeGrp;
   U32      selProb = 0x0;
   U8       preGrpId = 0x00;
   U8       pri = 0;
   U8       selPlmnOvrld;
   U16      ovrldCnt;
   
   rrccon->mmeId = 0xFF;
   while(pri < 5)
   {
      for (idx1 = 0; ((idx1 < wrEmmCb.numOfMmePriGrps) && (mmeIdx1 < WR_MAX_MMES)); idx1++)
      {
         if (wrEmmCb.mmeLoadBalCfg[idx1].mmeGrpPrior == pri)
         {
            mmeGrpList[mmeIdx1++] = wrEmmCb.mmeLoadBalCfg[idx1].mmeGroupId;
         } 
      }  
      pri++;
   }
   for (idx = 0; idx < mmeIdx1; idx++)
   {
      /* Loop through the list of MME groups in their order of priority */
      preGrpId = mmeGrpList[idx];
      for(mmeIdx = 0; mmeIdx < mmeLst->numOfMme; mmeIdx++)
      {
         selPlmnOvrld = FALSE;
         mmeCb = WR_EMM_MME_GET_MME(mmeLst->mmeIdList[mmeIdx]);
         if((WR_MME_UP == mmeCb->state) 
               && (selProb < (mmeCb->relCapacity - mmeCb->dynCapacity)) 
               && (!(mmeCb->mmeOvldFlag)))   
         {
            /* Check is the selected PLMN is overloaded and if so, will not 
             * be selecting the MME
             */
            for(ovrldCnt = 0; ovrldCnt < mmeCb->numOvrldGummei; ovrldCnt++)
            {
               if(TRUE == (wrUtlPlmnsEqual(rrccon->plmnId, 
                            &(mmeCb->ovrldGummeiLst[ovrldCnt].plmnId))))
               {
                  selPlmnOvrld = TRUE;
                  break;
               }
            }

            if(TRUE == selPlmnOvrld)
            {
               continue;
            }

           for(mmeGrp = 0; mmeGrp < WR_MAX_GRPS_PER_MME; mmeGrp++)
           {
             if(mmeCb->groupIds[mmeGrp] == preGrpId)
              {
                selProb = mmeCb->relCapacity - mmeCb->dynCapacity;
                rrccon->mmeId = mmeCb->mmeId;
                rrccon->gummei.mmeGrpId = preGrpId;
                break;
              }
            }
          }
       }
    }

   if(0xFF != rrccon->mmeId )
   {
      mmeCb = WR_EMM_MME_GET_MME(rrccon->mmeId);
      mmeCb->dynCapacity = (mmeCb->dynCapacity+1)%(mmeCb->relCapacity);
      ret = ROK;
   }
   else
   {
      for(mmeIdx = 0; mmeIdx < mmeLst->numOfMme; mmeIdx++)
      {
         selPlmnOvrld = FALSE;

         mmeCb = WR_EMM_MME_GET_MME(mmeLst->mmeIdList[mmeIdx]);
         if((mmeCb->state == WR_MME_UP)
                  && (selProb < (mmeCb->relCapacity - mmeCb->dynCapacity)) &&
                  (!(mmeCb->mmeOvldFlag)))
         {
            /* Check is the selected PLMN is overloaded and if so, will not 
             * be selecting the MME
             */
            for(ovrldCnt = 0; ovrldCnt < mmeCb->numOvrldGummei; ovrldCnt++)
            {
               if(TRUE == (wrUtlPlmnsEqual(rrccon->plmnId,
                           &(mmeCb->ovrldGummeiLst[ovrldCnt].plmnId))))
               {
                  selPlmnOvrld = TRUE;
                  break;
               }
            }

            if(TRUE == selPlmnOvrld)
            {
               continue;
            }

            selProb = mmeCb->relCapacity - mmeCb->dynCapacity;
            rrccon->mmeId = mmeCb->mmeId;
         }
      }
      if(0xFF != rrccon->mmeId )
      {
        mmeCb = WR_EMM_MME_GET_MME(rrccon->mmeId);
        mmeCb->dynCapacity = (mmeCb->dynCapacity+1)%(mmeCb->relCapacity);
        ret = ROK;
      }
   }
   RETVALUE(ret);
}

#ifdef DG_SG
/**
 *  @brief This function is used 
 *
 *  @details
 *      Function  :wrEmmGrpIdGetMmeIdList
 *          Processing Steps:
 *          - 
 *
 *  @param[in] mme : MME Id  
 *  @param[in] mmeList : MME ID list 
 *   @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/  
PRIVATE S16 wrEmmGrpIdGetMmeIdList
(
WrPlmnCbMmeList         *wrEmmPlmnMmeLst,
WrRrcConTransCb           *rrccon
)
{
   S16      ret = RFAILED;
   U8       pri = 0;
   U16      mmeGrpList[WR_MAX_MMES];
   U8       idx1;
   U32      preGrpId = 0; 
   U8       mmeIdx = 0;
   U8       mmeIdx1 = 0;
   WrMmeCb* mmeCb = NULLP;
   U32      mmeGrp;
   Bool     isMmeIdSelected = FALSE;
   U16      mmeIdSelected = 0;
   U8       relCap = 0;
   
   while(pri < 5)
   {
      for (idx1 = 0; ((idx1 < wrEmmCb.numOfMmePriGrps) && (mmeIdx1 < WR_MAX_MMES)); idx1++)
      {
         if (wrEmmCb.mmeLoadBalCfg[idx1].mmeGrpPrior == pri)
         {
            mmeGrpList[mmeIdx1++] = wrEmmCb.mmeLoadBalCfg[idx1].mmeGroupId;
         } 
      }  
      pri++;
   }                 

   for (idx1 = 0; idx1 < mmeIdx1; idx1++)
   {
      /* Loop through the list of MME groups in their order of priority */
      preGrpId = mmeGrpList[idx1];
      for(mmeIdx = 0; mmeIdx < wrEmmPlmnMmeLst->numOfMme; mmeIdx++)
      {
         /* Get the MME control block */
         mmeCb = WR_EMM_MME_GET_MME(wrEmmPlmnMmeLst->mmeIdList[mmeIdx]);
         /* Check for MME eligibility for LD algorithm */
         if ((mmeCb->state == WR_MME_UP) && (mmeCb->relCapacity > 0) && (!(mmeCb->mmeOvldFlag)))
         {
            for(mmeGrp = 0; mmeGrp < WR_MAX_GRPS_PER_MME; mmeGrp++)
            {
               if(mmeCb->groupIds[mmeGrp] == preGrpId)
               {
                  if (relCap < mmeCb->relCapacity)
                  {
                     /* Choose the MME with Highest relative Capacity in current priority group */
                     relCap = mmeCb->relCapacity;
                     isMmeIdSelected = TRUE;
                     mmeIdSelected = mmeIdx;
                     rrccon->gummei.mmeGrpId = preGrpId;
                  }
               }
            }
         }
      }
   }
   if (TRUE == isMmeIdSelected )
   {
      mmeCb = WR_EMM_MME_GET_MME(wrEmmPlmnMmeLst->mmeIdList[mmeIdSelected]);
      rrccon->mmeId = mmeCb->mmeId;
      ret = ROK;
   }
   else
   {
     for(mmeIdx = 0; mmeIdx < wrEmmPlmnMmeLst->numOfMme; mmeIdx++)
     {
        /* Get the MME control block */
        mmeCb = WR_EMM_MME_GET_MME(wrEmmPlmnMmeLst->mmeIdList[mmeIdx]);
        if ((WR_MME_UP == mmeCb->state ) && (mmeCb->relCapacity > 0) && (!(mmeCb->mmeOvldFlag)))
        {
           if (relCap < mmeCb->relCapacity)
           {
                  /* Choose the MME with Highest relative Capacity in current priority group */
                  relCap = mmeCb->relCapacity;
                  isMmeIdSelected = TRUE;
                  mmeIdSelected = mmeIdx;
           }
        }
     }
     if (TRUE == isMmeIdSelected )
     {
        mmeCb = WR_EMM_MME_GET_MME(wrEmmPlmnMmeLst->mmeIdList[mmeIdSelected]);
        rrccon->mmeId = mmeCb->mmeId;
        ret = ROK;
     }
   }
   RETVALUE(ret);
}/* end of wrEmmGrpIdGetMmeIdList */
#endif
#endif

/**
 *  @brief This function is used to Process GUMME IE list received
 *  in the Overload Stop Message 
 *
 *  @details
 *      Function  :wrEmmMmeRemoveGummeIEList
 *          Processing Steps:
 *          - Remove the GUMME IEs which are received in the overload stop
 *
 *  @param[in] mmeCb  : mmeCb
 *  @param[in] Gummei : GUMMEI list received from MME
 *   @return VOID 
**/
PRIVATE Void wrEmmMmeRemoveGummeIEList
(
WrMmeCb       *mmeCb,
SztGUMMEILst  *gummeiLst
)
{
   U32                 cntIndx;
   U32                 mmeGroupId;
   U32                 mmeCode;
   U32                 ovrldCnt;
   U32                 isPlmnEqual;
   SztGUMMEI          *gummeIE;
   WrPlmnId            plmnId;

   for(cntIndx = 0; cntIndx < gummeiLst->noComp.val; cntIndx++)
   {
      /* Extract GUMMEI list received from MME */
      gummeIE = &(gummeiLst->member[cntIndx]);
      wrEmmMmeParsePlmnIe(&(gummeIE->pLMN_Identity), &(plmnId));

      mmeGroupId = ((gummeIE->mME_Group_ID.val[1]) | 
                 (gummeIE->mME_Group_ID.val[0] << 8));
      mmeCode = gummeIE->mME_Code.val[0];

      /* Loop through the list and check is the GUMMEI is present in the 
       * overloaded GUMMEI list. If so, copy the last - 1 GUMMEI to this
       * given index and decrement the totoal number of overloaded
       * GUMMEI
       */
      for(ovrldCnt = 0; ovrldCnt < mmeCb->numOvrldGummei; ovrldCnt++)
      {
         isPlmnEqual = wrUtlPlmnsEqual(&(plmnId), 
                                    &(mmeCb->ovrldGummeiLst[ovrldCnt].plmnId));

         if((TRUE == isPlmnEqual) && 
             (mmeGroupId == mmeCb->ovrldGummeiLst[ovrldCnt].mmeGroupId) &&
             (mmeCode == mmeCb->ovrldGummeiLst[ovrldCnt].mmeCode))
         {
            /* In case of gummei is the last which needs to be removed, then
             * no need to copy the GUMMEI and just decrement the count
             */
            if(ovrldCnt != mmeCb->numOvrldGummei - 1)
            {
               cmMemcpy((U8*) &(mmeCb->ovrldGummeiLst[ovrldCnt]), 
                     (U8*) &(mmeCb->ovrldGummeiLst[mmeCb->numOvrldGummei - 1]),
                      sizeof(WrOvrldGummeiLst));

            }
            mmeCb->numOvrldGummei--;
         }
      }
   }

   RETVOID;
}



/**
 *  @brief This function is used to Process GUMME IE list received
 *  in the Overload Start Message 
 *
 *  @details
 *      Function  :wrEmmMmeUpdateGummeIEList
 *          Processing Steps:
 *          - Update GUMMEI list values in mmeCb
 *
 *  @param[in] mmeCb  : mmeCb
 *  @param[in] Gummei : GUMMEI list received from MME
 *   @return VOID 
**/
PRIVATE Void wrEmmMmeUpdateGummeIEList
(
WrMmeCb       *mmeCb,
SztGUMMEILst  *gummeiLst
)
{
   U32                 cntIndx;
   SztGUMMEI          *gummeIE;

   /* Extract the overload GUMMEI list from the overload start message
    * received from MME. Will be overwriting the GUMMEI list which
    * are already present. This is done as per S1AP spec 36.413, overload
    * information which is received recently needs to be applied at eNB
    */
   for(cntIndx = 0; cntIndx < gummeiLst->noComp.val; cntIndx++)
   {
      gummeIE = &(gummeiLst->member[cntIndx]);
      wrEmmMmeParsePlmnIe(&(gummeIE->pLMN_Identity), 
                          &(mmeCb->ovrldGummeiLst[cntIndx].plmnId));

      mmeCb->ovrldGummeiLst[cntIndx].mmeGroupId = 
                                     ((gummeIE->mME_Group_ID.val[1]) | 
                                      (gummeIE->mME_Group_ID.val[0] << 8));
      mmeCb->ovrldGummeiLst[cntIndx].mmeCode = gummeIE->mME_Code.val[0];

   }

   mmeCb->numOvrldGummei = cntIndx;

   RETVOID;
}

/**
 *  @brief This function is used to Clear ACB applied by RRM
 *
 *  @details
 *      Function  : wrEmmMmeClearOvrLoadAcb
 *          Processing Steps:
 *          - Removes the ACB which is applied due to overload
 *          start message received without GUMMEI list IE
 *
 *  @param[in] mmeCb  : mmeCb
 *   @return VOID 
**/
PRIVATE Void wrEmmMmeClearOvrLoadAcb
(
WrMmeCb       *mmeCb
)
{
   RmuMmeOvldStopInd    *rrmWrEnbStaInd   = NULLP;

   TRC2(wrEmmMmeClearOvrLoadAcb);

   /* Here MME overload stops, thus we need to indicate the mme overload
    * action to RRM for access class baring using Enb status indication 
    */
   WR_ALLOC (&rrmWrEnbStaInd, sizeof(RmuCommonMsg));

   if (rrmWrEnbStaInd == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");

      RETVOID;
   }

   rrmWrEnbStaInd->usMmeId = mmeCb->mmeId;

   /* Send the overload stop message to RMM to remove the ACB which is 
    * applied
    */
   if (ROK != wrIfmRrmMmeOvldStopInd(rrmWrEnbStaInd))
   {
      RLOG_ARG0(L_ERROR,DBG_MMEID,rrmWrEnbStaInd->usMmeId,
            "ENB status ind Failed.");

      WR_FREE(rrmWrEnbStaInd, sizeof(RmuCommonMsg));
      RETVOID;
   }
   else
   {
      RLOG_ARG0(L_INFO,DBG_MMEID,rrmWrEnbStaInd->usMmeId,
            "ENB status indication Success.");
   }

   RETVOID;
}

/**
 *  @brief This function is used to Process Overload start message 
 *
 *  @details
 *      Function  :wrEmmMmePrcOvldStart
 *          Processing Steps:
 *          - Get MME Cb based upon PeerId
 *          - Update Overload Start parameters as received.
 *
 *  @param[in] peerId : MME Id as received in S1AP message 
 *  @param[in] pdu : S1AP payload 
 *   @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/
PRIVATE S16 wrEmmMmePrcOvldStart
(
 U32  peerId,
 S1apPdu  *pdu
)
{
   SztProtIE_Field_OverloadStartIEs *ie               = NULLP;
   WrMmeCb                          *mmeCb            = NULLP;
   U32                              idx;
   S16                              ret               = RFAILED;
   U32                              isGummeieListRcvd = FALSE;


   TRC2(wrEmmMmePrcOvldStart);
   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);

   if(NULLP == mmeCb)
   {
      RLOG0(L_ERROR,"mmecb is NULL.");
      RETVALUE(RFAILED);
   }

   RLOG_ARG0(L_EVENT, DBG_MMEID, mmeCb->mmeId,"[S1AP]:Received OVERLOAD START");

   for(idx = 0; idx < pdu->pdu.val.initiatingMsg.value.u.sztOverloadStart.protocolIEs.noComp.val; idx++)
   {
       ie = &pdu->pdu.val.initiatingMsg.value.u.sztOverloadStart.protocolIEs.member[idx];
       switch(ie->id.val)
       {
          case Sztid_OverloadResp:
             {
                mmeCb->overloadAction = 
                   ie->value.u.sztOverloadResp.val.overloadAction.val;
             }
             break;

          case Sztid_GUMMEILst:
             {
                wrEmmMmeUpdateGummeIEList(mmeCb, &(ie->value.u.sztGUMMEILst));
                isGummeieListRcvd = TRUE;
             }
             break;

          case Sztid_TrafficLoadReductionInd:
             /* This IE is not handled presently */
             break;

          default: 
             RLOG0(L_ERROR, " Not supported IE present ");
             ret = RFAILED;
             break;
       }
   }

   /* Check if we have received the GUMMEI list in the Overload start message
    * if so, the overload status is not transfered to RRM and no ACB will be
    * applied. The overload status is only sent to RRM in case if we
    * have not received any GUEMMI IE list in overload start message 
    */
   if(FALSE == isGummeieListRcvd)
   {
      ret = wrEmmS1FlexHdlOverloadResponse(mmeCb, mmeCb->overloadAction);
      if(RFAILED == ret)
      {
         RLOG0(L_ERROR, "unable to process overload response IE");
      }
   }
   else
   {
      /* If we have received GUMMEI, and ACB is applied due to earlier 
       * overload start message, clear the same. GUMME IE list is updated
       * before while processing the IE
       */
      if(TRUE == mmeCb->mmeOvldFlag)
      {
         wrEmmMmeClearOvrLoadAcb(mmeCb);
         mmeCb->mmeOvldFlag = FALSE;
      }
   }
   RETVALUE(ret);
} /* wrEmmMmePrcOvldStart */

/**
 *  @brief This function is used to Process Overload Stop message 
 *
 *  @details
 *      Function  : wrEmmMmePrcOvldStop
 *          Processing Steps:
 *          - Get MME Cb based upon PeerId
 *          - Update Overload Stop parameters as received.
 *
 *  @param[in] peerId : MME Id as received in S1AP message 
 *  @param[in] pdu : S1AP payload 
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/
PRIVATE S16 wrEmmMmePrcOvldStop
(
 U32  peerId,
 S1apPdu  *pdu
)
{
   WrMmeCb              *mmeCb            = NULLP;
   RmuMmeOvldStopInd    *rrmWrEnbStaInd   = NULLP;
   SztProtIE_Field_OverloadStopIEs     *ovrldStopIE;
   SztProtIE_Cont_OverloadStopIEs      *ovrldStop;
   U32                                 idx;
   U32                                 isGummeiPres = FALSE;

   TRC2(wrEmmMmePrcOvldStop);

   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);

   if(NULLP == mmeCb)
   {
      RLOG0(L_ERROR,"mmecb is NULL.");
      RETVALUE(RFAILED);
   }
   
   RLOG_ARG0(L_EVENT, DBG_MMEID, mmeCb->mmeId,"[S1AP]:Received OVERLOAD STOP");

   if((mmeCb) && (TRUE == mmeCb->mmeOvldFlag))
    {
      mmeCb->mmeOvldFlag = FALSE;

      /* Here MME overload stops, thus we need to indicate the mme overload
       * action to RRM for access class baring using Enb status indication 
       */
      WR_ALLOC (&rrmWrEnbStaInd, sizeof(RmuCommonMsg));
         
      if (rrmWrEnbStaInd == NULLP)
      {
         RLOG0(L_FATAL,"Memory allocation failed.");
         
         RETVALUE(RFAILED);
      }
      
      rrmWrEnbStaInd->usMmeId = mmeCb->mmeId;

      if (ROK != wrIfmRrmMmeOvldStopInd(rrmWrEnbStaInd))
      {
         RLOG_ARG0(L_ERROR,DBG_MMEID,rrmWrEnbStaInd->usMmeId,
            "ENB status ind Failed.");

         WR_FREE(rrmWrEnbStaInd, sizeof(RmuCommonMsg));
         RETVALUE(RFAILED);
      }
      else
      {
         RLOG_ARG0(L_INFO,DBG_MMEID,rrmWrEnbStaInd->usMmeId,
            "ENB status indication Success.");
      }

      /* In this case, eNodeB received the Overload Start message without
       * GUMMEI list. When stop is received, in the current implementation
       * will clearing the Overload for all the GUMMEIs which are supported
       * by MME even if the GUMMEI list is present in the overload stop
       * message. It is assumed that MME should send the Overload Start 
       * message with GUMMEI list to make selected GUMMEI overloaded
       * instead of sending overload stop message with non-overloaded
       * GUMMEI */
      mmeCb->numOvrldGummei = 0;
   }

   /* Update the GUMMEI list if it is present in the overload stop message 
    * received 
    */
   ovrldStop = 
           &(pdu->pdu.val.initiatingMsg.value.u.sztOverloadStop.protocolIEs);

   for(idx = 0; idx < ovrldStop->noComp.val; idx++)
   {
      ovrldStopIE = &ovrldStop->member[idx];
      switch(ovrldStopIE->id.val)
      {
          case Sztid_GUMMEILst:
             {
                wrEmmMmeRemoveGummeIEList(mmeCb, 
                                         &(ovrldStopIE->value.u.sztGUMMEILst));
                isGummeiPres = TRUE;
             }
             break;
         default:
            WR_DBGP(WR_ERROR,(WR_PRNT_BUF,
                     "wrEmmMmePrcOvldStart: Not supported IE present  \n"));
            break;
      }

   }

   /* Clear the GUMMEI overload for all the GUMMEI if no GUMMEI
    * is present in the overload stop message
    */
   if(FALSE == isGummeiPres)
   {
      mmeCb->numOvrldGummei = 0;
   }

   RETVALUE(ROK);
} /* wrEmmMmePrcOvldStop */


/** @brief This function process the MME initiated messages which are belongs to
 *         EMM specific module. 
 *
 * @details This function process the MME initiated messags like S1AP: MME
 *          Configuration Update Messages, MME initiated RESET message,
 *          PAging message, Overload Start message, Write Replacement warning
 *          message etc., 
 *
 *     Function: wrEmmMmePrcInitPdu
 *
 *         Processing steps:
 *         - Based on the S1AP Initing PDU's procedure code,
 *           call the respective function to process the received PDU.
 *
 * @param[in] peerId: Peer ID from which S1AP PDU received.
 * @param[in]    pdu: S1-AP MME initiated PDU. 
 * @return  S16
 *       -# Success : ROK
 */
PRIVATE S16 wrEmmMmePrcInitPdu
(
U32                          peerId,
S1apPdu                      *pdu
)
{
   S16                       ret = ROK;
   SztInitiatingMsg          *initMsg = &pdu->pdu.val.initiatingMsg;

   switch(initMsg->procedureCode.val)
   {
      case Sztid_MMEConfigUpd:
         wrEmmMmePrcMmeCfgUpd(peerId, pdu);
         break;
      case Sztid_Reset:
         wrEmmMmePrcReset(peerId, pdu);
         break;
      case Sztid_Pag:
         wrEmmPbmProcPagingMsg(pdu);
         break;
      case Sztid_OverloadStart:
/* MME_LDBAL*/
         wrEmmMmePrcOvldStart(peerId, pdu);
         break;
      case Sztid_OverloadStop:
/* MME_LDBAL*/
         wrEmmMmePrcOvldStop(peerId, pdu);
         break;
      case Sztid_WriteReplaceWarning:
         /* PH4_CMAS */
         ret = wrEmmPwsProcWarningReq(peerId, pdu);
         if(ret == RFAILED)
         {
            RLOG0(L_ERROR,"Error in handling warning message."); 
         }
         break;
      /* PH4_CMAS */
      case Sztid_Kill:
         ret = wrEmmPwsProcKillReq(peerId, pdu);
         if(ret == RFAILED)
         {
            RLOG0(L_ERROR,"Error in handling kill message."); 
         }
         break;
      case Sztid_MMEConfigTfr:
         wrEmmMmePrcMMEConfigTfr(peerId, pdu);
         break;
      case Sztid_ErrInd:
         wrEmmMmePrcErrInd(peerId, pdu);
         break;
      case Sztid_MMEDirectInformTfr:
         wrEmmMmePrcMmeDirectInfoTransfer(pdu);
         break;
      default:
         RLOG1(L_ERROR,"Err: Msg not handled:[%ld]",
                       initMsg->procedureCode.val);
         RETVALUE(ROK);
   }

   RETVALUE(ret);
} /* end of wrEmmMmePrcInitPdu */


/** @brief This function process incoming the recieved S1AP PDU.
 *
 * @details
 *
 *     Function: wrEmmMmePrcRcvdPdu
 *
 *         Processing steps:
 *         - Based on the choice of the message PDU, It will procss the Initiing
 *           PDU, Successful outcome and Unsuccessful outcome messages.
 *
 *
 * @param[in] peerId: Peer ID. 
 * @param[in]    pdu: S1AP PDU 
 * @return  S16
 *       -# Success : ROK
 *       -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmePrcRcvdPdu
(
U32                          peerId,
S1apPdu                      *pdu
)
{
   S16                       ret = RFAILED;

   switch(pdu->pdu.choice.val)
   {
      case S1AP_PDU_INITIATINGMSG:
         ret = wrEmmMmePrcInitPdu(peerId, pdu);
         break;
      case S1AP_PDU_SUCCESSFULOUTCOME:
         ret = wrEmmMmePrcSuccPdu(peerId, pdu);
         break;
      case S1AP_PDU_UNSUCCESSFULOUTCOME:
         ret = wrEmmMmePrcUnsuccPdu(peerId, pdu);
         break;
   }

   RETVALUE(ret);
} /* end of wrEmmMmePrcRcvdPdu */

/** @brief This function process incoming Abort S1 Confirm
 *
 * @details
 *
 *     Function: wrEmmMmePrcS1Abort
 *
 *         Processing steps:
 *         Based on the Abort S1 confirmation status, this function will act.
 *         If status is OK, then send s1 Setup request message to MME. If not,
 *         return RFAILED.
 *
 *
 * @param[in] peerId: Peer ID. 
 * @param[in]    pdu: S1AP PDU 
 * @return  S16
 *       -# Success : ROK
 *       -# Failure : RFAILED
 */
PUBLIC S16 wrEmmMmePrcS1Abort
(
SztAbortS1   *abortS1
)
{
   WrMmeCb     *mmeCb;
   U32         idx = 0;

   /*RLOG1(L_DEBUG,"received status[%d]",abortS1->abortS1Cfm.status);*/
   RLOG0(L_FATAL,"MME is down. Reconnecting .....");
   if(abortS1->peerId.pres == PRSNT_NODEF)
   {
      mmeCb = wrEmmMmeGetMmeByPeerId(abortS1->peerId.val);
      if(mmeCb == NULLP)
      {
         /* discard the Abort Confirm */
         RLOG1(L_ERROR,
                 "S1Abort  from Invalid PeerId [%ld]", abortS1->peerId.val);
         RETVALUE(RFAILED);
      }
      /* Fix for ccpu00142992:TTIs are not coming after s1-close triggered from
       * MME when UE is in attached state:
       * To avoid the race condition at application between receiving the ABORT cfm and
       * UE local context release.
       */
      for(idx = 0; idx < WR_EMM_MAX_CELLS; idx++)
      {
         /* Fix for CR ccpu00143884 */
         if ( WR_CELL_STATE_PWR_DOWN == wrEmmCb.cellCb[idx]->cellState )
         {
            if(NULLP == (wrUmmGetUeCntxToRel(wrEmmCb.cellCb[idx]->cellId)))
            {
               if (mmeCb->adminState != WR_MME_LOCKED)
               {
                  /* 
                   * Here extract the list and check against the mmeId.
                   * for matching MMEID set the AbortCfm falg to FALSE again.
                   */
                  wrEmmCb.peerAbrtCfm.isAbortCfmRcv = FALSE;
                  wrEmmMmeSetup(mmeCb->mmeId);
               }
            }
            else
            {
               /* Here extract the list check against the mmeID. Against the 
                * matching mmeID set the Abort Cfm flag to true. This will be
                * checked again in UMM module after the Local context release
                * for all the UE's.
                */
               wrEmmCb.peerAbrtCfm.mmeId = mmeCb->mmeId;
               wrEmmCb.peerAbrtCfm.isAbortCfmRcv = TRUE;
            }
         }
         else
         {
            if (mmeCb->adminState != WR_MME_LOCKED)
            {
                wrEmmMmeSetup(mmeCb->mmeId);
            }
         }

      }
   }/* end of peerId.pres */
   RETVALUE(ROK);
}/* wrEmmMmePrcS1Abort */
/*
 *
 *      Fun:   wrEmmMMeEnbRst
 *
 *      Desc:  Sends out the eNB reset to all the MME
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *
 */

PUBLIC S16 wrEmmMmeEnbRst
(
   WrCellCb                  *cellCb
)
{
   WrMmeCb         *mmeCb;
   U32              mmeId;
   U8               noMmeToReset = 0;
   PTR                  prevEntry = NULLP;
   WrNeighEnbCb         *nghEnbCb;
   S16                  retVal;
   WrUmmMsgCause        cause;
   U16                  noNghEnbToReset = 0;
  /*ccpu00131048*/ 
   for(mmeId = 0; mmeId < WR_MAX_MMES; mmeId++)
   {
      mmeCb = WR_EMM_MME_GET_MME(mmeId);

      if((mmeCb != NULLP) && (mmeCb->state == WR_MME_UP))
      {
         noMmeToReset ++;
         wrEmmMmeSendResetReq(mmeCb->mmeId,
               CAUSE_RADIONW,SztCauseRadioNwunspecifiedEnum, 0,
            RESETTYP_S1_INTF, NULLP);
      }
   }

   /* Update the number of MMEs to be 
    * reset in the WrCellCb so that 
    * after getting all the S1 Reset Ack 
    * Cell Reset can be triggered 
    */

   if(cellCb->cellState == WR_CELL_STATE_RESET)
   {
      if(noMmeToReset > 0)
      {
         cellCb->noMmeToReset = noMmeToReset;  
      } 
      else /* Set the state to WR_S1_RESET_DONE so the Cell Reset can be triggered */ 
      {
         wrCb.resetCmfState |= WR_S1_RESET_DONE;
      }
   }

   /* Resetting neighbour cells */
   cause.causeTyp = 1;
   cause.causeVal = 0;

   while ((retVal = cmHashListGetNext(&(wrEmmCb.neighEnbLstCp), prevEntry, (PTR *) &nghEnbCb)) == ROK)
   {
      if ( nghEnbCb->x2Connection == WR_NEIGH_ENB_STATE_UP )
      {
         noNghEnbToReset ++;
         retVal = wrEmmNghEnbSndResetReq(nghEnbCb->peerId, &cause);
         if ( retVal != ROK )
         {
            RLOG0(L_ERROR,"wrEmmNghEnbSndResetReq failed");
            RETVALUE(RFAILED);
         }
      }
      prevEntry = (PTR)nghEnbCb;
     }
   /* Update the number of neighbour eNodebs to be 
    * reset in the WrCellCb so that 
    * after getting all the X2 Reset Ack 
    * Cell Reset can be triggered 
    */

   if(cellCb->cellState == WR_CELL_STATE_RESET)
   {
      if(noNghEnbToReset > 0)
      {
         cellCb->noNghEnbToReset = noNghEnbToReset;  
      } 
      else /* Set the state to WR_X2_RESET_DONE so the Cell Reset can be triggered */ 
      {
         wrCb.resetCmfState |= WR_X2_RESET_DONE;
      }
   }

   RETVALUE(ROK);
}

PUBLIC S16 wrEmmMmePeerRestart
(
   WrCellId                     cellId
)
{
   WrCellCb                  *cellCb = NULLP;
   WrMmeCb                   *mmeCb  = NULLP;
   U32                       mmeId = 0;

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Cell Cb Not found");
      RETVALUE(RFAILED);
   }

   RLOG_ARG0(L_INFO,DBG_CELLID, cellCb->cellId,
         "Cell state changed to RESET, state = WR_CELL_STATE_RESET");

   cellCb->cellState = WR_CELL_STATE_RESET;
   wrCb.parResetCmfState = WR_INIT_STATE_NONE;
  /* This is done as the CELL RESET State is Common 
   * for Partial Reset and Full Reset Case 
   */
   wrCb.resetCmfState = WR_INIT_STATE_NONE;

   /* Update the WrummCellCb state here */
   if(ROK != wrUmmUpdateCellState(cellId,WR_CELL_STATE_RESET))
   {
     RLOG_ARG0(L_ERROR,DBG_CELLID,cellId,"Cell Cb Not found");
      RETVALUE(RFAILED);
   }

   /* Release all UEs  per MME and Send S1 Setup Request */
   for(mmeId = 0; mmeId < wrEmmCb.mmeCont.numMmes; mmeId++)
   {
      mmeCb = WR_EMM_MME_GET_MME(mmeId);

      if((mmeCb != NULLP) && (mmeCb->state == WR_MME_UP))
      {
         /* Release all UEs belongs to this particluar MME */
         if(ROK != wrUmmMmeFullRst(mmeCb->mmeId))
         {
            RLOG_ARG0(L_ERROR,DBG_MMEID,mmeCb->mmeId,"wrUmmMmeFullRst Failed");
            RETVALUE(RFAILED);
         }
    wrEmmMmeDown(mmeCb);
         /* Send S1 Setup Request to MME */
         if (ROK != wrEmmMmeSetup(mmeCb->mmeId))
         { 
            RLOG_ARG0(L_ERROR,DBG_MMEID,mmeCb->mmeId,"wrEmmMmeSetup Failed");
            RETVALUE(RFAILED);
         }
      }
   }
    /*This is done here as func:wrEmmMmeDown looks at the cell state. If the cell state 
    is not WR_CELL_STATE_UP, POwerDown Command is not sent. So we need to set the state 
    after this procedure.*/
    
   /* cellCb->cellState = WR_CELL_STATE_RESET; */
   /* wrEmmUpdateCellState(cellId,WR_CELL_STATE_RESET); */
   /*Cell State is updated in the func:wrEmmMmeDown to WR_CELL_STATE_PWR_DOWN*
   So we are checking if the cell state is not WR_CELL_STATE_PWR_DOWN, then we 
   set the state as WR_CELL_STATE_RESET else we keep the same state. */

   if(cellCb->cellState != WR_CELL_STATE_PWR_DOWN)
   {
     if(ROK != wrEmmUpdateCellState(cellId,WR_CELL_STATE_RESET))
     {
        RLOG0(L_ERROR,"wrEmmUpdateCellState is not updated with"
           "WR_CELL_STATE_RESET");
        RETVALUE(RFAILED);
     }
   }

   /* TODO: Trigger X2 setup Request for all Neighbour enbs and also change
    * WR_PAR_RESET_DONE macro value to 26 */
   RETVALUE(ROK);
} /* end of wrEmmMmePeerRestart */

#ifdef WR_TEST_CODE
/*
 *
 *      Fun:   wrEmmMMeTstEnbRst
 *
 *      Desc:  Sends out the eNB reset for testing
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  ve_lmm.c
 *
 */
#ifdef ANSI
PUBLIC S16 wrEmmMmeTstEnbRst
(
 U8         rstType
)
#else
PUBLIC S16 wrEmmMmeTstEnbRst (mmeCb, rstType)
U8          rstType;
#endif
{
   /* Logic is to test all type of resets. So, using counter, each time,
    * one one type of reset is called */
   WrEmmS1ConId tmpConId[10];
   WrMmeCb      *mmeCb;
   U32           mmeId;
   U32           ueEnbS1apId = 0;
   U32           ueMmeS1apId = 0;
   S16           ret = 0;

   for(mmeId = 0; mmeId < WR_MAX_MMES; mmeId++)
   {
      mmeCb = WR_EMM_MME_GET_MME(mmeId);

      if((mmeCb != NULLP) && (mmeCb->state == WR_MME_UP))
      {
         break;
      }
   }

   if (mmeCb == NULLP)
   {
      RLOG1(L_ERROR,"Invalid MME ID [%d]",mmeId); 
      RETVALUE(RFAILED);
   }
   if ( mmeCb->state != WR_MME_UP )
   {
      /*127802*/
      RLOG1(L_INFO," S1 Connection not up for MME [%d]",mmeId);
      RETVALUE(RFAILED);
   }

  if(rstType == RESETTYP_PARTOFS1_INTF)
  {
     ret = wrUmmMmeGetFirstUeS1apId(mmeCb->mmeId, &ueEnbS1apId, &ueMmeS1apId);
     if ( ret != ROK )
     {
       /* No UE is present, still sending Reset with some 
           id */
       tmpConId[0].conId = 1;
     }
     else
     {
       tmpConId[0].conId = ueEnbS1apId;
     }
     tmpConId[0].type = WR_S1AP_ENB_ID;

     wrEmmMmeSendResetReq(mmeCb->mmeId, CAUSE_RADIONW, SztCauseRadioNwunspecifiedEnum, 1,
             RESETTYP_PARTOFS1_INTF, tmpConId);
  }
  else
  {
     wrEmmMmeSendResetReq(mmeCb->mmeId, CAUSE_RADIONW,SztCauseRadioNwunspecifiedEnum, 0,
            RESETTYP_S1_INTF, NULLP);
  }
   RETVALUE(ROK);
}
#endif /* WR_TEST_CODE */

/** @brief This function handler is to handle the
 *          eNB CONFIGURATION TRANSFER RESPONSE timer expiry
 *
 * @details This function is used to Resend the eNB
 *          CONFIGURATION TRANSFER message to MME in case of
 *          eNodeB not receiving MME CONFIGURATION TRANSFER
 *          message within time. 
 *
 *     Function: wrEmmMmeEnbCfgTransferRspTmrExpiry
 *     
 *         Processing steps:
 *         - Invokes the wrEmmMmeEnbConfigTransfer to build and
 *           send S1AP: eNB CONFIGURATION TRANSFER message to MME.
 *         - This function is invoked by the following module
 *           # TIMER
 *
 * @param[in] nghEnbCb : pointer to WrNeighCb
 * @return  S16
 *       -# Success : ROK
 */
PUBLIC S16 wrEmmMmeEnbCfgTransferRspTmrExpiry
(
   WrNeighEnbCb              *nghEnbCb
)
{
   WrEmmEnodCfgParama         enbCfgparama;

   TRC2(wrEmmMmeEnbCfgTransferRspTmrExpiry)

   if (nghEnbCb->enbcfgTransRtxCnt < WR_EMM_ENB_CFG_TRANSFER_RTX_CNT)
   {
      enbCfgparama.numEnb = 1;
      enbCfgparama.neighEnbCb[0] = nghEnbCb;
      wrEmmMmeBldEnbConfigTransfer(&enbCfgparama, nghEnbCb->cellId);
      nghEnbCb->enbcfgTransRtxCnt++;
   }
   RETVALUE(ROK);
}/*end of wrEmmMmeEnbCfgTransferRspTmrExpiry */

 /** @brief This function is used to build and send the eNB CONFIGURATION TRANSFER to the
 *         MME.
 *
 * @details This function is used to build and send the eNB CONFIGURATION TRANSFER to the
 *          MME.
 *
 *     Function: wrEmmMmeEnbConfigTransfer
 *
 *         Processing steps:
 *         - Build and send eNB CONFIGURATION TRANSFER to the MME.
 *         - Move the MME control block state to WR_MME_IN_PROG.
 *         - Start the Setup response timer to retransmit the setup request
 *           incase of reponse is not received with in the time.
 *         - This function is invoked EMM module when EMM Module ges trigger
 *           from Management intrerface ie., during cell config/setup.
 *
 * @param[in] mmeId: MME Id.
 * @return  S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */

#ifdef ANSI
PUBLIC S16 wrEmmMmeEnbConfigTransfer
(
 WrEmmMmeEnbCfgTrnsfr        *enbCfgTrnsfr,
U32                          enbCfgTransTmrVal,
 WrNeighEnbCb                *nghEnbcb
)
#else
PUBLIC S16 wrEmmMmeEnbConfigTransfer(WrEmmMmeEnbCfgTrnsfr *enbCfgTrnsfr,
               U32 enbCfgTransTmrVal, WrNeighEnbCb *nghEnbcb)
 WrEmmMmeEnbCfgTrnsfr        *enbCfgTrnsfr,
 U32                          enbCfgTransTmrVal, 
 WrNeighEnbCb                *nghEnbcb
#endif
{
   SztUDatEvnt               uDatEvnt;
   WrMmeCb                   *mmeCb;

   if(wrEmmMmeBldEnbCfgTrnsfr(&(uDatEvnt.pdu),enbCfgTrnsfr) != ROK)
   {
      RETVALUE(RFAILED);
   }
   
   uDatEvnt.transId.pres = PRSNT_NODEF;
   uDatEvnt.transId.val = 1;
   uDatEvnt.peerId.pres = PRSNT_NODEF;

   /* Fetch MME CB */
   mmeCb = wrEmmMmeGetMmeByPlmn(&enbCfgTrnsfr->targetInfo.plmnId);

   if(mmeCb != NULLP)
   {
      uDatEvnt.peerId.val = mmeCb->peerId;
      if((WrIfmS1apSndMgmtMsg (&uDatEvnt)) == ROK)
      {
         /* Start the timer */
         if(nghEnbcb != NULLP)
         {
            wrStartTmr((PTR)nghEnbcb, WR_TMR_ENB_CFG_TRANSFER_WAIT, 
               enbCfgTransTmrVal);
         }
         RETVALUE(ROK);
      }   
   }   

   RETVALUE(RFAILED);
}

/**
 *  @brief This function is used to Process ENB CONFIGURATION TRANSFER request
 *
 *  @details
 *      Function  : wrEmmSndEnbCfgTransferReq
 *          Processing Steps:
 *          - Process Enb config transfer message
 *
 *  @param[in] srcEnbId :Source cell Enodeb identifier 
 *  @param[in] tgtEnbId :target cell Enodeb identifier 
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
PRIVATE S16 wrEmmSndEnbCfgTransferReq
(
  SztSrceNB_ID         *srcEnbId,
  SztTgeteNB_ID        *tgtEnbId
)
{
   WrEmmMmeEnbCfgTrnsfr      enbCfgTrnsfr = { 0 }; 
   TRC2(wrEmmSndEnbCfgTransferReq)

   enbCfgTrnsfr.targetInfo.enbType = srcEnbId->global_ENB_ID.eNB_ID.choice.val;
   switch(enbCfgTrnsfr.targetInfo.enbType)
   {
      case ENB_ID_MACROENB_ID:
         wrUtlGetU32FrmBStr32(&enbCfgTrnsfr.targetInfo.cellId,
               &srcEnbId->global_ENB_ID.eNB_ID.val.macroENB_ID);   
         break;
      case ENB_ID_HOMEENB_ID:
         wrUtlGetU32FrmBStr32(&enbCfgTrnsfr.targetInfo.cellId,
               &srcEnbId->global_ENB_ID.eNB_ID.val.homeENB_ID);
         break;
   }
   wrGetU32FrmTknStr4(&enbCfgTrnsfr.targetInfo.tac, &srcEnbId->selected_TAI.tAC);
   wrUtlGetPlmnId(&enbCfgTrnsfr.targetInfo.plmnId,
              &srcEnbId->selected_TAI.pLMNidentity);

   enbCfgTrnsfr.sourceInfo.enbType = tgtEnbId->global_ENB_ID.eNB_ID.choice.val;
   switch(enbCfgTrnsfr.sourceInfo.enbType)
   {
      case ENB_ID_MACROENB_ID:
         wrUtlGetU32FrmBStr32(&enbCfgTrnsfr.sourceInfo.cellId,
               &tgtEnbId->global_ENB_ID.eNB_ID.val.macroENB_ID);   
         break;
      case ENB_ID_HOMEENB_ID:
         wrUtlGetU32FrmBStr32(&enbCfgTrnsfr.sourceInfo.cellId,
               &tgtEnbId->global_ENB_ID.eNB_ID.val.homeENB_ID);
         break;
   }
   wrGetU32FrmTknStr4(&enbCfgTrnsfr.sourceInfo.tac, &tgtEnbId->selected_TAI.tAC);
   wrUtlGetPlmnId(&enbCfgTrnsfr.sourceInfo.plmnId,
              &tgtEnbId->selected_TAI.pLMNidentity);
   enbCfgTrnsfr.isReply = TRUE;
   wrEmmMmeEnbConfigTransfer(&enbCfgTrnsfr,wrEmmCb.enbCfgTransTmrVal,NULLP);
   RETVALUE(ROK);
}

 /**
 *  @brief This function is used to Update TLA's in MME CONFIGURATION TRANSFER
 *
 *  @details
 *      Function  :wrEmmUpdateTLA 
 *          Processing Steps:
 *
 *  @param[in] TLAs : TLA
 *  @param[in] enbIp : enBIp address
 *  @param[in] lwrMngmt : layer managment
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
PRIVATE S16 wrEmmUpdateTLA
(
SztENBX2TLAs *TLAs, 
CmTptAddr *enbIp, 
LwrMngmt *lwrMngmt
)
{
   S8                         str[MAX_IPADDR_LEN];
  
   CmInetIpAddr nbrIpv4Addr = 
      ntohl(lwrMngmt->t.nbrAddReq.nbIpAddress.u.ipv4TptAddr.address);

   {
      /*Fix for ccpu00123536 */
      wrUtlGetTportAddr(&TLAs->member[0], 
             &lwrMngmt->t.nbrAddReq.nbIpAddress);
      if(lwrMngmt->t.nbrAddReq.nbIpAddress.type == 
          CM_TPTADDR_IPV4)
      {
         cmInetNtop(lwrMngmt->t.nbrAddReq.nbIpAddress.
         type,&nbrIpv4Addr,str,MAX_IPADDR_LEN);
      }
      else if(lwrMngmt->t.nbrAddReq.nbIpAddress.type == 
               CM_TPTADDR_IPV6)
      {
         cmInetNtop(lwrMngmt->t.nbrAddReq.nbIpAddress.
         type,&lwrMngmt->t.nbrAddReq.nbIpAddress.u.
         ipv6TptAddr.ipv6NetAddr,str,MAX_IPADDR_LEN);
      }
      RLOG_STR(L_INFO," Neighbor ip address :: [%s] ",str);
      /* CR ccpu00140501: eNB sends x2 setup request to
       * self. As in this issue the MME was sending the
       * Source ENB IP address incorrectly as target
       * in order to avoid this included the check
       * here. The issue has come due to the faulty MME.
       */
      WR_GET_ENBIP_FROM_CELLID(*enbIp, 
        lwrMngmt->t.nbrAddReq.nbIpAddress.type);

      if(CM_TPTADDR_IPV4 == enbIp->type)
      {
         if (lwrMngmt->t.nbrAddReq.nbIpAddress.u.ipv4TptAddr.address 
             == enbIp->u.ipv4TptAddr.address)
         {
            RLOG0(L_ERROR,"Trying to add self as neighbour.returnign from here:IPv4");
            RETVALUE(RFAILED); 
         }
      }
      else if(CM_TPTADDR_IPV6 == enbIp->type)
      {
         if(ROK == cmMemcmp((U8 *)&lwrMngmt->t.nbrAddReq.nbIpAddress.u.ipv6TptAddr.ipv6NetAddr,
            (U8 *)&enbIp->u.ipv6TptAddr.ipv6NetAddr, sizeof(CmInetIpAddr6)))
         {
            RLOG0(L_ERROR,"Trying to add self as neighbour.returnign from here:IPv6");
            RETVALUE(RFAILED); 
         }
      }
      else if(WR_IP_INVALID == enbIp->type)
      {
         RLOG0(L_ERROR,"eNB and Neigbour IP address mismatch");
         RETVALUE(RFAILED); 
      }
   }
   RETVALUE(ROK);
}

 /**
 *  @brief This function is used to Process MME CONFIGURATION TRANSFER
 *
 *  @details
 *      Function  : wrEmmMmePrcMMEConfigTfr
 *          Processing Steps:
 *          - Get MME Cb based upon PeerId
 *          - Update MME CONFIGURATION TRANSFER parameters as received.
 *
 *  @param[in] peerId : MME Id as received in S1AP message 
 *  @param[in] pdu : S1AP payload 
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
#ifdef ANSI
PRIVATE S16 wrEmmMmePrcMMEConfigTfr
(
 U32                         peerId,
 S1apPdu                     *pdu
)
#else
PRIVATE S16 wrEmmMmePrcMMEConfigTfr(U32  peerId,S1apPdu  *pdu)
U32                          peerId;
S1apPdu                      *pdu;
#endif
{
   U32                        idx             = 0;
   WrMmeCb                    *mmeCb          = NULLP;
   SztInitiatingMsg           *initMsg        = &pdu->pdu.val.initiatingMsg;
   SztMMEConfigTfr            *cfgTfr         = &initMsg->value.u.sztMMEConfigTfr;
   SztSONConfigTfr            *sonCfgTfr      = NULLP;
   SztENBX2TLAs               *TLAs           = NULLP;
   LwrMngmt                   *lwrMngmt = NULLP;
   SztSrceNB_ID               *srcEnbId = NULLP;
   SztTgeteNB_ID              *tgetEnbId= NULLP;
   SztProtIE_Field_MMEConfigTfrIEs *ie = NULLP;
   WrNeighEnbCb               *neighEnbCb = NULLP;
   //U8                         indx = 0;
   /*CR ccpu00140501 */
   CmTptAddr                  enbIp={0};

   TRC2(wrEmmMmePrcMMEConfigTfr);

   mmeCb = wrEmmMmeGetMmeByPeerId(peerId);
   if (mmeCb ==  NULLP)
   {
      RLOG1(L_ERROR,"Processing MME Config Transfer,"
      "Received Invalid Peer Id [%d]",peerId);
      RETVALUE(RFAILED);
   }
   if (mmeCb->state != WR_MME_UP)
   {
      /* Dont respond with any here as the state is invalid to transmit   */
      /* any response at all. May be an error indication may be sent      */
      RLOG0(L_INFO," Could not find any MMEs in active state");
      RETVALUE(RFAILED);
   }
   if((cfgTfr->pres.pres == FALSE) || (cfgTfr->protocolIEs.noComp.val == 0) ||
      (cfgTfr->protocolIEs.member[0].pres.pres == FALSE))
   {
      RETVALUE(RFAILED);
   }
   sonCfgTfr = &cfgTfr->protocolIEs.member[0].value.u.sztSONConfigTfr;
   if(sonCfgTfr->pres.pres == FALSE)
   {
      RETVALUE(RFAILED);
   }
   srcEnbId = &sonCfgTfr->sourceeNB_ID; 
   tgetEnbId = &sonCfgTfr->targeteNB_ID;

   RLOG_ARG1(L_EVENT, DBG_MMEID, mmeCb->mmeId,"[S1AP]:Received MME CONFIGURATION TRANSFER"
             "[SRC-ENB-TYPE:%d]",srcEnbId->global_ENB_ID.eNB_ID.choice.val);

   for(idx = 0; idx < cfgTfr->protocolIEs.noComp.val; idx++)
   {
      ie = cfgTfr->protocolIEs.member + idx;
      switch(ie->id.val)
      {
         case Sztid_SONConfigTfrMCT:
         {  
            U32                   sonInfoType;

            if(TRUE != sonCfgTfr->sONInform.choice.pres)
            {
               break;
            }
            sonInfoType = sonCfgTfr->sONInform.choice.val;
            switch(sonInfoType)
            {
               case SONINFORM_SONINFORMRQST:
               {
                  RLOG0(L_DEBUG,"Send eNodeB config transfer with reply");
                  wrEmmSndEnbCfgTransferReq(srcEnbId, tgetEnbId);
                  break;
               }
               case SONINFORM_SONINFORMREPLY:
               {
                  SztSONInformReply     *reply = NULLP; 
                  U32                   enbType;
                  WR_ALLOC(&lwrMngmt, sizeof(LwrMngmt));
                  if(lwrMngmt == NULLP)
                  {
                     RLOG0(L_FATAL,"Memory allocation failed.");
                     RETVALUE(RFAILED);       
                  }
                  enbType   = srcEnbId->global_ENB_ID.eNB_ID.choice.val;
                  switch(enbType)
                  {
                     case ENB_ID_MACROENB_ID:
                        wrUtlGetU32FrmBStr32(&lwrMngmt->t.nbrAddReq.enbId,
                              &srcEnbId->global_ENB_ID.eNB_ID.val.macroENB_ID);   
                        break;
                     case ENB_ID_HOMEENB_ID:
                        wrUtlGetU32FrmBStr32(&lwrMngmt->t.nbrAddReq.enbId,
                              &srcEnbId->global_ENB_ID.eNB_ID.val.homeENB_ID);
                        break;
                  }

                  cmHashListFind(&(wrEmmCb.neighEnbLstCp),(U8 *)&lwrMngmt->t.nbrAddReq.enbId,
                        sizeof(lwrMngmt->t.nbrAddReq.enbId),0,(PTR *)&neighEnbCb);
                  if(neighEnbCb != NULLP)
                  {
                     wrStopTmr((PTR)neighEnbCb, WR_TMR_ENB_CFG_TRANSFER_WAIT);    
                  }
                  else
                  {
                     RLOG0(L_ERROR,"Neigh enodeb Cb is NULLP");
                  }
                  reply = &sonCfgTfr->sONInform.val.sONInformReply;

                  if(TRUE == reply->pres.pres)
                  {
                     TLAs = &reply->x2TNLConfigInfo.eNBX2TportLyrAddres;
                     if((TLAs->noComp.pres) && (TLAs->noComp.val))
                     {
                        /* sri - for now using only the first IP address */
                        //while (indx < TLAs->noComp.val)
                        if(ROK != wrEmmUpdateTLA(TLAs, &enbIp, lwrMngmt))
                        {
                            RLOG0(L_ERROR,"wrEmmUpdateTLA failed\n");
                        }
                     }
                  }
                  wrGenerateAddNeighCfgReq(lwrMngmt);  
                  break;
               }
            }
         }
         break;
      }   
   }
   RETVALUE(ROK);       

} /* wrEmmMmePrcMMEConfigTfr */

#ifdef SZTV2

/**
 *  @brief This function is used to bring down All connected MMEs.
 *
 *  @details
 *      Function  : wrEmmMmeBrngAllS1Down
 *          Processing Steps:
 *          - Get list of active MMES
 *          - Make MME admin state as locked for each MME.
 *          - Abort the SCTP connection of each MME.
 *
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
#ifdef ANSI
PUBLIC S16 wrEmmMmeBrngAllS1Down
(
)
#else
PUBLIC S16 wrEmmMmeBrngAllS1Down()
#endif
{

   SztAbortS1   abortS1;
   WrMmeCb      *mmeCb = NULLP;
   U8           idx;

   TRC2(wrEmmMmeBrngAllS1Down);

   for(idx = 0; idx < WR_MAX_MMES; idx++)
   {
      mmeCb = wrEmmCb.mmeCont.mmes[idx];
      if ((mmeCb == NULLP) || (mmeCb->state == WR_MME_INITED))
      {
         continue;
      }
      {  /* 
          * TODO  Redirect all UEs before bring down the MMEs using primitive  
          * wrUmmFillRedirectInfo.  
          * Below implementation is only required till wrUmmFillRedirectInfo
          * available  
          */
         wrUmmMmeFullRst(mmeCb->mmeId);

         mmeCb->adminState = WR_MME_LOCKED;

         abortS1.peerId.pres = PRSNT_NODEF;
         abortS1.peerId.val   = mmeCb->peerId;
         /* sndAbortFlag set to FALSE means this is SCTP graceful
          * close (i.e., Shutdown) SCTP association  */
         abortS1.sndAbortFlag = FALSE;

         /* Send the pdu to the MME */
         if((WrIfmS1apSndAbortMsg(&abortS1)) != ROK)
         {
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
} /* end of wrEmmMmeBrngAllS1Down */
#endif
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
