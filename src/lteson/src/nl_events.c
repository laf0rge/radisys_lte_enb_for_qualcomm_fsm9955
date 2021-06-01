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
               

     File:     nl_eventc.c

     Sid:      bk_events.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/2 - Fri Jan  2 16:28:39 2015

     Prg:      subhamay 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="SON";
static int RLOG_FILE_ID=247;
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

/* local typedefs */
 
/* local externs */
EXTERN NlCellPciType   pciModType[NL_MAX_PCI_MOD_VAL];
/* forward references */
PRIVATE S16 nlBuildCellReCfgPciList ARGS((
NlCellCb        *cellCb,
LnlCellReCfg    *tmpReCfg,
LnlCellReCfg    *cellReCfg
));

/*SON state machine*/
nlGlbStateMachine nlGlbCellFSM[NL_CELL_STATE_MAX][NL_CELL_EVENT_MAX] =
{
   {                              /*NL_CELL_STATE_INIT*/
      nlInitNghCellCfgEvtHndlr,   /*NL_CELL_EVENT_NGH_CFG*/
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
   },
   {                                /*NL_CELL_STATE_PCI_CONFLICT*/
      nlConflictNghCellCfgEvtHndlr, /*NL_CELL_EVENT_NGH_CFG*/       
      nlConflictCellReCfgEvtHndlr,  /*NL_CELL_EVENT_RECFG*/
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
   },
   {                               /*NL_CELL_STATE_CONFIGURED*/
      nlConfigNghCellCfgEvtHndlr,  /*NL_CELL_EVENT_NGH_CFG*/
      nlConfigCellReCfgEvtHndlr,   /*NL_CELL_EVENT_RECFG*/
      nlConfigUeAddEvtHndlr,       /*NL_CELL_UE_ADD*/
      nlConfigUeDeleteEvtHndlr,    /*NL_CELL_UE_DELETE*/
      nlConfigUeMeasRptEvtHndlr,   /*NL_CELL_UE_MEAS_RPT*/
      nlConfigUeCqiRptEvtHndlr,    /*NL_CELL_UE_CQI_RPT*/
      nlConfigUeModEvtHndlr,       /*NL_CELL_UE_MODIFY*/
   },
   {                                  /*NL_CELL_STATE_PCI_MODIFY*/
      nlPciModifyNghCellCfgEvtHndlr,  /*NL_CELL_EVENT_NGH_CFG*/
      nlPciModifyCellReCfgEvtHndlr,   /*NL_CELL_EVENT_RECFG*/
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
      nlProcessInvalidMsg,
   }
};

#ifdef ANSI
PUBLIC S16 nlProcessInvalidMsg
(
NlEventMsg   *evtMsg,   /* msg structure  */
NlCellCb     *cellCb    /* cell control block*/
)
#else
PUBLIC S16 nlProcessInvalidMsg(evtMsg, cellCb)
NlEventMsg    *evtMsg;   /* msg structure */
NlCellCb      *cellCb;   /* cell control block*/
#endif
{
   RETVALUE(RFAILED);
}

/**
 * @brief handle the neighbor cell configuration when PCI 
 *  modification in progress
 *
 * @details
 *
 *     Function : nlPciModifyNghCellCfgEvtHndlr
 *
 *     This function handles the cell reconfiguration information when
 *     PCI modification is in progress in SON. SON should store the cell
 *     reconfiguration information and process it after PCI modification
 *     process successfully complete.

 *  @param[in]  evtMsg  : neighbor cell information parameter structure
 *  @param[in]  cellCb  : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PUBLIC S16 nlPciModifyNghCellCfgEvtHndlr
(
NlEventMsg   *evtMsg,   /* msg structure  */
NlCellCb     *cellCb    /* cell control block*/
)
#else
PUBLIC S16 nlPciModifyNghCellCfgEvtHndlr(evtMsg, cellCb)
NlEventMsg    *evtMsg;   /* msg structure */
NlCellCb      *cellCb;   /* cell control block*/
#endif
{
   NlTmpNghCellCfg   *nghCellCfg = NULLP;   
   NlNghCfgPendMsg   *pendNghCfg = NULLP;
   TRC3(nlPciModifyNghCellCfgEvtHndlr);

   nghCellCfg     = (NlTmpNghCellCfg *)evtMsg->msg;

   /*store the neighbor information in pending message Q*/
   NL_ALLOC(NL_REGION, NL_POOL, &pendNghCfg, sizeof(NlNghCfgPendMsg));
   if(NULLP == pendNghCfg)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for neighbor cfg during PCI Mod");
      RETVALUE(RFAILED);
   }
   /*store the neighbor information*/
   cmMemcpy((U8 *)&pendNghCfg->nghCellCfg, (U8 *)nghCellCfg,
                sizeof(NlTmpNghCellCfg));
   pendNghCfg->link.node = (PTR)pendNghCfg;
   cmLListAdd2Tail(&cellCb->pciModPendMsg.nghCfgMsgList, &pendNghCfg->link); 
   RETVALUE(ROK);   
}

/**
 * @brief handle the cell reconfiguration when PCI modification in progress
 *
 * @details
 *
 *     Function : nlPciModifyCellReCfgEvtHndlr 
 *
 *     This function handles the cell reconfiguration information when 
 *     PCI modification is in progress in SON. SON should store the cell
 *     reconfiguration information and process it after PCI modification
 *     process successfully complete.
 *
 *  @param[in]  evtMsg  : neighbor cell information parameter structure
 *  @param[in]  cellCb  : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PUBLIC S16 nlPciModifyCellReCfgEvtHndlr
(
NlEventMsg   *evtMsg,   /* msg structure  */
NlCellCb     *cellCb    /* cell control block*/
)
#else
PUBLIC S16 nlPciModifyCellReCfgEvtHndlr(evtMsg, cellCb)
NlEventMsg    *evtMsg;   /* msg structure */
NlCellCb      *cellCb;   /* cell control block*/
#endif
{
   LnlCellReCfg       *cellReCfg    = NULLP;
   CmLList            *link         = NULLP;
   NlCellReCfgPendMsg *reCfgPendMsg = NULLP;
   TRC3(nlPciModifyCellReCfgEvtHndlr);

   cellReCfg = (LnlCellReCfg *)evtMsg->msg;
   
   /*store the cell reconfig information in pending message Q*/
   /* we always store the latest cell reconfig message*/
   CM_LLIST_FIRST_NODE(&cellCb->pciModPendMsg.cellReCfgMsgList, link);
   if(NULLP == link)
   { 
      NL_ALLOC(NL_REGION, NL_POOL, &reCfgPendMsg, sizeof(NlCellReCfgPendMsg));
      if(NULLP == reCfgPendMsg)
      {
         RLOG0(L_ERROR, "Failed to allocate memory for Cell re cfg during PCI Mod");
         RETVALUE(RFAILED);
      }
      cmMemcpy((U8 *)&reCfgPendMsg->cellReCfg, (U8 *)cellReCfg, sizeof(LnlCellReCfg));
      reCfgPendMsg->link.node = (PTR)reCfgPendMsg;
      cmLListAdd2Tail(&cellCb->pciModPendMsg.cellReCfgMsgList, &reCfgPendMsg->link);
   }
   else
   {
      /*update the cell reconfig message with latest PCI list*/
      reCfgPendMsg = (NlCellReCfgPendMsg *)link->node;
      cmMemcpy((U8 *)&reCfgPendMsg->cellReCfg, (U8 *)cellReCfg, sizeof(LnlCellReCfg));
   }
   RETVALUE(ROK);
}

/**
 * @brief handle the neighbor cell configuration when cell state is Init
 *
 * @details
 *
 *     Function : nlInitNghCellCfgEvtHndlr
 *
 *     This function handles the neighbor cell information when cell state is
 *     init. Cell state will be updated to configured if neighbor cell
 *     information handled successfully.
 *
 *  @param[in]  evtMsg  : neighbor cell information parameter structure
 *  @param[in]  cellCb  : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlInitNghCellCfgEvtHndlr
(
NlEventMsg       *evtMsg,      /* neighbor cell config structure  */
NlCellCb         *cellCb       /* cell control block*/
)
#else
PUBLIC S16 nlInitNghCellCfgEvtHndlr(evtMsg, cellCb)
(
NlEventMsg      *evtMsg;       /* neighbor cell config structure */
NlCellCb        *cellCb;       /* cell control block*/
)
#endif
{
   NlAlarm           alarm;
   NlTmpNghCellCfg   *nghCellCfg = NULLP;   /* neighbor cell config structure */
   U8                alarmCount;
   TRC3(nlInitNghCellCfgEvtHndlr);

   cmMemset((U8 *)&alarm, 0, sizeof(NlAlarm));

   nghCellCfg     = (NlTmpNghCellCfg *)evtMsg->msg;
   if((nghCellCfg->numNghEnb > 0) && 
      (NL_NGH_ADD != nghCellCfg->nghEnbCellCfg[0].nghCellParam.cfgType))
   {
      RLOG1(L_ERROR, "Invalid ngh cell config type %d received",
             nghCellCfg->nghEnbCellCfg[0].nghCellParam.cfgType);
      RETVALUE(RFAILED);
   }
   if(ROK != nlCellSelectPrach(cellCb, evtMsg, &alarm))
   {
      RLOG0(L_ERROR,
           "Failed to select prach configuration for serving cell");
      RETVALUE(RFAILED);
   }
   nlCellSelectPci(cellCb, nghCellCfg, evtMsg->msgType, &alarm);
   /* check whether any alarm raised*/
   if(TRUE == alarm.isAlarmSend)
   {
      for(alarmCount = 0; alarmCount < alarm.numAlarmRaised; alarmCount ++)
      {
         nlSendLnlAlarm(&alarm.alarmRaised[alarmCount].dgn);
      }
   }
   if(TRUE == cellCb->isSmSonCfgDone) 
   {
      nlCellSelectTxPower(cellCb);
   }
   RETVALUE(ROK);
}

/**
 * @brief handle the neighbor cell configuration when cell state is conflict due 
 *   to PCI collison or confusion
 *
 * @details
 *
 *     Function : nlConflictNghCellCfgEvtHndlr
 *
 *     This function handles the neighbor cell information when cell state is
 *     conflict due to PCI collision or confusion. Cell state will be updated to 
 *     configured if neighbor cell information handled successfully.
 *
 *  @param[in]  evtMsg  : neighbor cell information parameter structure
 *  @param[in]  cellCb  : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlConflictNghCellCfgEvtHndlr
(
NlEventMsg       *evtMsg,      /* neighbor cell config structure  */
NlCellCb         *cellCb      /* cell control block*/
)
#else
PUBLIC S16 nlConflictNghCellCfgEvtHndlr(evtMsg, cellCb)
(
NlEventMsg      *evtMsg;       /* neighbor cell config structure */
NlCellCb        *cellCb;       /* cell control block*/
)
#endif
{
   NlAlarm           alarm;
   NlTmpNghCellCfg   *nghCellCfg = NULLP;   /* neighbor cell config structure */
   U32               alarmCount;

   TRC3(nlConflictNghCellCfgEvtHndlr);
   
   cmMemset((U8 *)&alarm, 0, sizeof(NlAlarm));
   nghCellCfg     = (NlTmpNghCellCfg *)evtMsg->msg;

   /*Perform PCI collision detection and resolution activity*/
   nlNghPciCollisionDetect(cellCb, nghCellCfg, evtMsg->msgType, &alarm);
   /*Perform PCI confusion detection activity*/
   nlNghPciConfusionDetect(cellCb, nghCellCfg, &alarm);
   /*do neighbor info add/mod/del in neighbor table*/
   nlNghPciInfoUpdateInNghTbl(cellCb, nghCellCfg, &alarm);
   
   /*check whether any alarm cleared or not*/
   if(TRUE == alarm.isAlarmCleared)
   {
      for(alarmCount = 0; alarmCount < alarm.numAlarmCleared; alarmCount ++)
      {
         nlSendLnlAlarm(&alarm.alarmCleared[alarmCount]);
      }
   }
   /* check whether any alarm raised*/
   if(TRUE == alarm.isAlarmSend)
   {
      for(alarmCount = 0; alarmCount < alarm.numAlarmRaised; alarmCount ++)
      {
         nlSendLnlAlarm(&alarm.alarmRaised[alarmCount].dgn);
      }
   }
   /*send PCI updation to SM only when SON moves from Conflict state 
     to config state only after cell id UP*/
   if(NL_CELL_STATE_CONFIGURED == cellCb->state)
   {
      /*This is when SON goes to conflict state because of PCI conflict after cell was UP*/
      if(TRUE == alarm.isPciUpdated)
      {
         NL_UPDATE_CELL_STATE(cellCb, NL_CELL_STATE_PCI_MODIFY);
         cellCb->pciModAlarmInfo.isAlarmSend = TRUE;
         nlSendPciUpdateInd(cellCb);
      }
      else if(FALSE == cellCb->isSmSonCfgDone)
      {
         cellCb->isSmSonCfgDone = TRUE;
         RLOG1(L_INFO, "SON selected Serv cell PCI : %d",
                    cellCb->cellInfo.pci);
         /*send PCI update to OAM still now cell is not UP*/
         nlSendPciUpdateInd(cellCb);
         nlCellSelectTxPower(cellCb);
      }
   }
   else
   {
      /*check whether As part of PCI modification / delete can resolve the PCI conflict problem*/
      nlNghPciCollisionResolveWithNghDelOrMod(cellCb, nghCellCfg, &alarm);
   }
   /*SON should send REMIND to BRDCM-SON irrespective of conflict/configured state*/
   /*SON should not send REM INDICATION If It already initiated TxpowerCfg request to BRDCM SON*/
   if(NL_VENDOR_DSON_STATE_CONFIGURED == cellCb->dSonState)
   {
      nlBuildRemResultInd(cellCb);
   }
   RETVALUE(ROK);
}
/**
 * @brief handle the neighbor cell configuration when cell state is configured
 *
 * @details
 *
 *     Function : nlConfigNghCellCfgEvtHndlr
 *
 *     This function handles the neighbor cell information when cell state is
 *     configured. This function will update neighbor cell information received 
 *     from OAM and raised alarm accordingly for PCI collision or confusion.
 *
 *  @param[in]  evtMsg  : neighbor cell information parameter structure
 *  @param[in]  cellCb  : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *
 **/

#ifdef ANSI
PUBLIC S16 nlConfigNghCellCfgEvtHndlr
(
NlEventMsg       *evtMsg,      /* neighbor cell config structure  */
NlCellCb         *cellCb       /* cell control block*/
)
#else
PUBLIC S16 nlConfigNghCellCfgEvtHndlr(evtMsg, cellCb)
(
NlEventMsg      *evtMsg;       /* neighbor cell config structure */
NlCellCb        *cellCb;       /* cell control block*/
)
#endif
{
   NlTmpNghCellCfg   *nghCellCfg = NULLP;   /* neighbor cell config structure */
   U32               alarmCount;
   NlAlarm           alarm;
   TRC3(nlConfigNghCellCfgEvtHndlr);
   cmMemset((U8 *)&alarm, 0, sizeof(NlAlarm));
   nghCellCfg     = (NlTmpNghCellCfg *)evtMsg->msg;

   /*process only if DSON PCI selection is enable*/
   if(NL_PCI_SEL_DSON != nlCb.genCfg.pciSelType)
   {
      RLOG0(L_INFO, "Distributed SON PCI selection is not enable");
      RETVALUE(ROK);
   }
   /*Perform PCI collision detection and resolution activity*/
   nlNghPciCollisionDetect(cellCb, nghCellCfg, evtMsg->msgType, &alarm);
   /*Perform PCI confusion detection activity*/
   nlNghPciConfusionDetect(cellCb, nghCellCfg, &alarm);
   /*do neighbor info add/mod/del in neighbor table*/
   nlNghPciInfoUpdateInNghTbl(cellCb, nghCellCfg, &alarm);
   
   /*check whether any alarm cleared or not*/
   if(TRUE == alarm.isAlarmCleared)
   {
      for(alarmCount = 0; alarmCount < alarm.numAlarmCleared; alarmCount ++)
      {
         nlSendLnlAlarm(&alarm.alarmCleared[alarmCount]);
      }
   }
   /* check whether any alarm raised*/
   if(TRUE == alarm.isAlarmSend)
   {
      for(alarmCount = 0; alarmCount < alarm.numAlarmRaised; alarmCount ++)
      {
         nlSendLnlAlarm(&alarm.alarmRaised[alarmCount].dgn);
      }
   }
   if(TRUE == alarm.isPciUpdated)
   {
      /*whenever PCI update will trigger cell state must be in PCI MODIFY
        state and consecutive PCI modification trigger from OAM will be stoped.
        Cell state again will be in configured state only after getting PCI
        modification success from eNB-APP*/
      NL_UPDATE_CELL_STATE(cellCb, NL_CELL_STATE_PCI_MODIFY);
      cellCb->pciModAlarmInfo.isAlarmSend = TRUE;
      nlSendPciUpdateInd(cellCb);
   }
   else if (NL_CELL_STATE_CONFIGURED != cellCb->state)
   {
      if((NL_NGH_MOD == nghCellCfg->nghEnbCellCfg[0].nghCellParam.cfgType) &&
         (TRUE == alarm.resolveDeferred))
      {
         if(ROK == nlNghPciCollisionResolve(cellCb, nghCellCfg, &alarm))
         {
            if(TRUE == alarm.isPciUpdated)
            {
               NL_UPDATE_CELL_STATE(cellCb, NL_CELL_STATE_PCI_MODIFY);
               cellCb->pciModAlarmInfo.isAlarmSend = TRUE;
            }
            nlSendPciUpdateInd(cellCb);
         }
      }
   }
   /*Inform REM Indication to BRDCM - SON */
   /* Uday-Ensure REM result indication is not sent to BRCM-SON Once TX
    * Power configuration is done */
   if(NL_VENDOR_DSON_STATE_CONFIGURED != cellCb->dSonState)
   {
      RLOG0(L_INFO,"Sending REM result indication");	   
      nlBuildRemResultInd(cellCb);
   }

   RETVALUE(ROK);
}

/**
 * @brief Cell Reconfig List preparation considering CSG and small cell
 *
 * @details
 *
 *     Function : nlBuildCellReCfgPciList
 *
 *     This function builds the cell reconfiguration list based on CSG and
 *     small cell PCI ranges
 *
 *  @param[in]  cellCb    : cell control block
 *  @param[in]  tmpReCfg  : cell reconfig list which will be rebuild
 *  @param[in]  tmpReCfg  : cell reconfig list received from SM
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PRIVATE S16 nlBuildCellReCfgPciList
(
NlCellCb        *cellCb,
LnlCellReCfg    *tmpReCfg,
LnlCellReCfg    *cellReCfg
)
#else
PRIVATE S16 nlBuildCellReCfgPciList(cellCb, tmpReCfg, cellReCfg)
(
NlCellCb        *cellCb;
LnlCellReCfg    *tmpReCfg;
LnlCellReCfg    *cellReCfg;
)
#endif
{
   U32             count;
   U32             numPci = 0;
   U16             maxCsgPci;
   TRC3(nlBuildCellReCfgPciList);
   maxCsgPci = cellCb->cellInfo.csgStartPci + cellCb->cellInfo.csgPciRange;
   if((TRUE == cellCb->cellInfo.isCsgPciCfg) && 
      (LNL_ENB_CLOSED_ACCESS == cellCb->cellInfo.accessMode))
   {
      for(count = 0; count < cellReCfg->numPci; count ++)
      {
         if((cellReCfg->pciList[count] >= cellCb->cellInfo.csgStartPci) && 
             (( cellReCfg->pciList[count] < maxCsgPci)))
         {
            tmpReCfg->pciList[numPci] = cellReCfg->pciList[count];
            numPci ++;
         }
      }
      if(numPci < 1)
      {
         RLOG0(L_ERROR, "No Valid PCI found for Cell ReCfg CSG PCI Range config");
         RETVALUE(RFAILED);
      }
      tmpReCfg->numPci = numPci;
   }
   else if(TRUE == cellCb->cellInfo.isSmallCellPciCfg)
   {
      for(count = 0; count < cellReCfg->numPci; count ++)
      {
         if((cellReCfg->pciList[count] >= cellCb->cellInfo.smallCellStartPci) && 
             (cellReCfg->pciList[count] <= cellCb->cellInfo.maxSmallCellPci))
         {
            if(TRUE == cellCb->cellInfo.isCsgPciCfg)
            {
               if((cellReCfg->pciList[count] < cellCb->cellInfo.csgStartPci) ||
                  (cellReCfg->pciList[count] >= maxCsgPci))
               {
                  tmpReCfg->pciList[numPci] = cellReCfg->pciList[count];
                  numPci ++;
               }
            }
            else
            {
               tmpReCfg->pciList[numPci] = cellReCfg->pciList[count];
               numPci ++;
            }
         }
      }
      if(numPci < 1)
      {
         RLOG0(L_ERROR, "No Valid PCI found for Cell ReCfg  Small Cell PCI Range config");
         RETVALUE(RFAILED);
      }
      tmpReCfg->numPci = numPci;
   }
   else
   {
      if(TRUE == cellCb->cellInfo.isCsgPciCfg)
      {
         for(count = 0; count < cellReCfg->numPci; count ++)
         {
            if((cellReCfg->pciList[count] < cellCb->cellInfo.csgStartPci) ||
               (cellReCfg->pciList[count] >= maxCsgPci))
            {
               tmpReCfg->pciList[numPci] = cellReCfg->pciList[count];
               numPci ++;
            }
         }
         tmpReCfg->numPci = numPci;
      }
      else
      {
         tmpReCfg->numPci = cellReCfg->numPci;
         cmMemcpy((U8 *)&tmpReCfg->pciList[0], (U8 *)&cellReCfg->pciList[0],
            sizeof(cellReCfg->pciList[0]) * cellReCfg->numPci);
      }
      if(tmpReCfg->numPci < 1)
      {
         RLOG0(L_ERROR, "No Valid PCI found for Cell ReCfg  Cell PCI Range config");
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief Layer Manager cell ReConfiguration request handler
 *
 * @details
 *
 *     Function : nlConflictCellReCfgEvtHndlr
 *
 *     This function handles the SON cell Reconfiguration
 *     request received from the Layer Manager before SON is configured.
 *     This scenario mainly arise if PCI collison or confusion occured due to
 *     neighbor cell information configuration time.
 *
 *  @param[in]  msg     : cell reconfiguration parameter structure
 *  @param[in]  cellCb  : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlConflictCellReCfgEvtHndlr
(
NlEventMsg       *evtMsg,      /* cell reconfig structure  */
NlCellCb         *cellCb      /* cell control block*/
)
#else
PUBLIC S16 nlConflictCellReCfgEvtHndlr(evtMsg, cellCb)
(
NlEventMsg      *evtMsg;       /* cell reconfig structure */
NlCellCb        *cellCb;       /* cell control block*/
)
#endif
{
   LnlCellReCfg   *cellReCfg = NULLP;
   LnlCellReCfg    tmpReCfg;
   U32            pciCount;
   Bool           pciCollRes = TRUE;
   U32            count;
   TRC3(nlConflictCellReCfgEvtHndlr);
   cellReCfg = (LnlCellReCfg *)evtMsg->msg;
   cmMemset((U8 *)&tmpReCfg, 0, sizeof(LnlCellReCfg));
   tmpReCfg.cellId = cellReCfg->cellId;
   cmMemcpy((U8 *)&tmpReCfg.ecgi, (U8 *)&cellReCfg->ecgi, sizeof(LnlEcgi));
   if(cellReCfg->numPci > 0)
   {
      /*Regenarate the PCI list based on CSG as well as small cell PCI configuration*/
      if(ROK != nlBuildCellReCfgPciList(cellCb, &tmpReCfg, cellReCfg))
      {
         RLOG0(L_ERROR, 
          "Failed to select PCI as per CSG and small cell range from PCI list");
         RETVALUE(RFAILED);
      }
      /*select a PCI from pci list and check whether the corresponding PCI creates PCI
        collision with existing neighbor configuration of this cell*/
      for(count = 0; count < NL_MAX_PCI_MOD_VAL; count ++)
      {
         NlCellPciType   modType;
         modType = pciModType[count];
         for(pciCount = 0; pciCount < tmpReCfg.numPci; pciCount ++)
         {
            if((tmpReCfg.pciList[pciCount] <= NL_MAX_PCI_VAL)&&
               (tmpReCfg.pciList[pciCount] != cellCb->cellInfo.pci))
            {
               if(RFAILED != nlValidatePciWithNghCellInfo(cellCb,
                       tmpReCfg.pciList[pciCount], 
                       cellCb->cellInfo.dlEarfcn, modType))
               {
                  pciCollRes = FALSE;
                  break;
               }
            }
         }
         if(FALSE == pciCollRes)
         {
            break;
         }
      }
      if(FALSE != pciCollRes)
      {
         RLOG0(L_ERROR, "Failed to Perform PCI mod for cell ReCfg in conflict state");
         RETVALUE(RFAILED);
      }
      /*update the new PCI and pci list in cell control block*/
      nlStoreCellReCfgPciInfo(cellCb, &tmpReCfg, pciCount);
      nlSendPciUpdateInd(cellCb);
      if(FALSE == cellCb->isSmSonCfgDone)
      {
         cellCb->isSmSonCfgDone = TRUE;
         nlCellSelectTxPower(cellCb);
      }
      RETVALUE(ROK);
   }
   else
   {
      RLOG0(L_ERROR, "Empty PCI List received in cell reconfiguration");
   }
   RETVALUE(RFAILED);
}
/**
 * @brief PCI list modification initiated by OAM
 *
 * @details
 *
 *     Function : nlCellReCfgPciListHndlr
 *
 *     This function handles the PCI list modification initiated by OAM.
       PCI modification procedure will trigger for successful PCI list updation
 *
 *  @param[in]  cellReCfg  : cell reconfiguration parameter structure
 *  @param[in]  cellCb     : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/

#ifdef ANSI
PRIVATE S16 nlCellReCfgPciListHndlr
(
LnlCellReCfg     *cellReCfg,   /* cell reconfig structure  */
NlCellCb         *cellCb       /* cell control block*/
)
#else
PRIVATE S16 nlCellReCfgPciListHndlr(cellReCfg, cellCb)
(
LnlCellReCfg   *cellReCfg;     /* cell reconfig structure */
NlCellCb        *cellCb,       /* cell control block*/
)
#endif
{
   U32             pciCount;
   Bool            pciCollRes = TRUE;
   U32             count;
   TRC3(nlCellReCfgPciListHndlr);
   /*kloc fix*/
   if(cellReCfg->numPci > LNL_MAX_PCI_CFG)
   {
      RLOG0(L_ERROR, "Invalid number of PCI received in Cell ReCfg");
      RETVALUE(RFAILED);
   }
   /*check whether serving pci matching with newly received pci list*/
   for(pciCount = 0; pciCount < cellReCfg->numPci; pciCount ++)
   {
      if((cellReCfg->pciList[pciCount] <= NL_MAX_PCI_VAL) &&
          (cellReCfg->pciList[pciCount] == cellCb->cellInfo.pci))
      {
         cmMemcpy((U8 *)&cellCb->pciList[0], (U8 *)&cellReCfg->pciList[0],
            sizeof(cellReCfg->pciList[0]) * cellReCfg->numPci);
         cellCb->numPci = cellReCfg->numPci;
         cellCb->selPciIdx = pciCount;
         nlUpdatePciListWithSelPci(cellCb);
         /* ccpu00143256 */
         nlSendPciUpdateInd(cellCb);
         RETVALUE(ROK);
      }
   }
   /*select a PCI from pci list and check whether the corresponding PCI creates PCI 
     collision with existing neighbor configuration of this cell*/
   for(count = 0; count < NL_MAX_PCI_MOD_VAL; count ++)
   {
      NlCellPciType   modType;
      modType = pciModType[count];
      for(pciCount = 0; pciCount < cellReCfg->numPci; pciCount ++)
      {
         if((cellReCfg->pciList[pciCount] <= NL_MAX_PCI_VAL)&&
            (cellReCfg->pciList[pciCount] != cellCb->cellInfo.pci))
         {
            if(RFAILED != nlValidatePciWithNghCellInfo(cellCb,
                       cellReCfg->pciList[pciCount],
                       cellCb->cellInfo.dlEarfcn, modType))
            {
               pciCollRes = FALSE;
               break;
            }
         }
      }
      if(FALSE == pciCollRes)
      {
         break;
      }
   }

   if(FALSE != pciCollRes)
   {
      RLOG0(L_ERROR, "Failed to Perform PCI modification");
      RETVALUE(RFAILED);
   }
   /*update the new PCI and pci list in cell control block*/
   nlStoreCellReCfgPciInfo(cellCb, cellReCfg, pciCount);
   /*whenever PCI update will trigger cell state must be in conflict
     state and consecutive PCI modification trigger from OAM will be stoped.
     Cell state again will be in configured state only after getting PCI
     modification success from eNB-APP*/
   NL_UPDATE_CELL_STATE(cellCb, NL_CELL_STATE_PCI_MODIFY);
   cellCb->pciModAlarmInfo.isAlarmSend = TRUE;
   nlSendPciUpdateInd(cellCb);
   RETVALUE(ROK);
}
/**
 * @brief Layer Manager cell ReConfiguration request handler.
 *
 * @details
 *
 *     Function : nlConfigCellReCfgEvtHndlr
 *
 *     This function handles the SON cell Reconfiguration
 *     request received from the Layer Manager.
 *
 *  @param[in]  evtMsg  : cell reconfiguration parameter structure
 *  @param[in]  cellCb  : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlConfigCellReCfgEvtHndlr
(
NlEventMsg       *evtMsg,      /* cell reconfig structure  */
NlCellCb         *cellCb       /* cell control block*/
)
#else
PUBLIC S16 nlConfigCellReCfgEvtHndlr(evtMsg, cellCb)
(
NlEventMsg      *evtMsg;       /* cell reconfig structure */
NlCellCb        *cellCb;       /* cell control block*/
)
#endif
{
   LnlCellReCfg   *cellReCfg = NULLP;
   LnlCellReCfg    tmpReCfg;
   NlAlarm         alarm = {0};
   U32             alarmCount;
   TRC3(nlConfigCellReCfgEvtHndlr);

   /*process only if DSON PCI selection is enable*/
   if(NL_PCI_SEL_DSON != nlCb.genCfg.pciSelType)
   {
      RLOG0(L_INFO, "Distributed SON PCI selection is not enable");
      RETVALUE(ROK);
   }
   cellReCfg = (LnlCellReCfg *)evtMsg->msg;

   cmMemset((U8 *)&tmpReCfg, 0, sizeof(LnlCellReCfg));
   tmpReCfg.cellId = cellReCfg->cellId;
   cmMemcpy((U8 *)&tmpReCfg.ecgi, (U8 *)&cellReCfg->ecgi, sizeof(LnlEcgi));

   if(cellReCfg->numPci > 0)
   {
      /*Regenarate the PCI list based on CSG as well as small cell PCI configuration*/
      if(ROK != nlBuildCellReCfgPciList(cellCb, &tmpReCfg, cellReCfg))
      {
         RLOG0(L_ERROR,
          "Failed to select PCI as per CSG and small cell range from PCI list");
         RETVALUE(RFAILED);
      }
      if(ROK != nlCellReCfgPciListHndlr(&tmpReCfg, cellCb))
      {
         RLOG0(L_ERROR, "Failed to Perform PCI modification");
         /*update PCI value for raising alarm*/
         NL_FILL_PCI_COLLISON_ALARM(cellCb->cellId, cellCb, (&alarm));
         for(alarmCount = 0; alarmCount < alarm.numAlarmRaised; alarmCount ++)
         {
            nlSendLnlAlarm(&alarm.alarmRaised[alarmCount].dgn);
         }
         RETVALUE(RFAILED);
      }
   }
   else
   {
      RLOG0(L_ERROR, "Invalid PCI Recfg msg received");
      RETVALUE(RFAILED);
   } 
   RETVALUE(ROK);
}

/**
 * @brief Tx power selected by DSON handler.
 *
 * @details
 *
 *     Function : nlHandleCellSelectedTxPower
 *
 *     This function handles the DSON selected Tx Power
 *     request received from the vendor specific DSON.
 *
 *  @param[in]  cellCb  : cell control block
 *  @return  S16
 *     -# Success       : ROK
 *     -# Failure       : RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 nlHandleCellSelectedTxPower
(
NlCellCb         *cellCb       /* cell control block*/
)
#else
PUBLIC S16 nlHandleCellSelectedTxPower(cellCb)
(
NlCellCb        *cellCb;       /* cell control block*/
)
#endif
{
   TRC3(nlHandleCellSelectedTxPower);

   NL_UPDATE_CELL_STATE(cellCb, NL_CELL_STATE_CONFIGURED);
   nlSendTxPwrUpdateInd(cellCb);
   RETVALUE(ROK);
}

/********************************************************************30**

           End of file:     bk_events.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/2 - Fri Jan  2 16:28:39 2015

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

