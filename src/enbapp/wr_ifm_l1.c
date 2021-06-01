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
               

     File:     wr_ifm_l1.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/


static const char* RLOG_MODULE_NAME="IFM";

static int RLOG_FILE_ID=22;

static int RLOG_MODULE_ID=4;

#include "wr.h" 
#include "wr_emm.h" 
#include "wr_umm.h" 
#include "wr_ifm_l1.h"
#include "wr_lwr.h"
#include "wr_smm_init.h"
#include "ctf.x"
#include "wr_emm_cnm.h"
#include "wr_ifm_son.h"
#include "wr_emm_neighenb.h"
#include "wr_ifm_rrm.h"

#ifdef TENB_AS_SECURITY
EXTERN S16 YsUiCtfKdfReq ARGS((
         Pst *pst,
         SpId spId,
         CtfCfgTransId transId,
         CtfKdfReqInfo* cfgReqInfo
         ));

#endif /* TENB_AS_SECURITY */
#ifdef ENABLE_CNM
EXTERN PUBLIC S16 wrCnmTrigIctaStartReq(Void);
#endif
/* Request from RRC to PHY to bind the CTF interface sap. */
PRIVATE CONSTANT CtfBndReq WrIfmL1BndReqMt [] =
{
   cmPkCtfBndReq,          /* 0 - loosely coupled */
#ifdef TF 
   TfUiCtfBndReq,          /* 1 - tightly coupled, CTF */
#else
   NULLP,                  /* 1 - tightly coupled,  INVALID */
#endif /* TF */
#ifdef DM 
   DmUiCtfBndReq           /* 2 - tightly coupled, CTF */
#else
   NULLP                   /* 2 - tightly coupled, INVALID */
#endif /* DM */
};

/* Request from RRC to PHY to Unbind the CTF interface sap. */
PRIVATE CONSTANT CtfUbndReq WrIfmL1UbndReqMt [] =
{
   cmPkCtfUbndReq,          /* 0 - loosely coupled */
#ifdef TF 
   TfUiCtfUbndReq,          /* 1 - tightly coupled, CTF */
#else
   NULLP,                   /* 1 - tightly coupled, INVLAID */
#endif /* TF */
#ifdef DM 
   DmUiCtfUbndReq           /* 2 - tightly coupled, CTF */
#else
   NULLP                    /* 2 - tightly coupled, INVALID */
#endif /* DM */
};

/* Configuration Request from RRC to PHY for 
 * cell or dedicated configuration. 
*/
PRIVATE CONSTANT CtfCfgReq WrIfmL1CfgReqMt[] =
{
   cmPkCtfCfgReq,          /* 0 - loosely coupled */
#ifdef TF
   TfUiCtfCfgReq,          /* 1 - tightly coupled, CTF */
#else
   NULLP,                  /* 1 - tightly coupled, INVALID */
#endif
#ifdef DM 
   DmUiCtfCfgReq           /* 2 - tightly coupled, CTF */
#else
   NULLP                   /* 2 - tightly coupled, INVALID */
#endif
};

/** UE ID change Request from RRC to PHY. */

#ifdef TENB_AS_SECURITY
/** UE ID change Request from RRC to PHY. */
PRIVATE CONSTANT CtfKdfReq WrIfmL1KdfReqMt[] =
{
   cmPkCtfKdfReq,      /* 0 - loosely coupled */
#ifdef TF
   YsUiCtfKdfReq,   /* 1 - Tightly coupled */
#endif   
#ifdef DM
   DmUiCtfKdfReq       /* 2 - tightly coupled, CTF */
#else
   NULLP                   /* 2 - tightly coupled, INVALID */
#endif
};
#endif /* TENB_AS_SECURITY */

#define WR_CTF_UE_CFG      0
#define WR_CTF_CELL_CFG    1


/* CNM_DEV start */
PRIVATE CONSTANT CtfCnmInitSyncReqMsg WrIfmL1InitialSyncReqMt[] =
{
   cmPkCtfCnmInitSyncReq,  /* 0 - loosely coupled */
#ifdef TF
   TfUiCtfCfgReq,          /* 1 - tightly coupled, CTF */
#else
   NULLP,                  /* 1 - tightly coupled, INVALID */
#endif
#ifdef DM 
   NULLP                   /* 2 - tightly coupled, CTF */
#else
   NULLP                   /* 2 - tightly coupled, INVALID */
#endif
};

PRIVATE CONSTANT CtfCnmCellSyncReqMsg WrIfmL1CellSyncReqMt[] =
{
   cmPkCtfCnmCellSyncReq,  /* 0 - loosely coupled */
#ifdef TF
   TfUiCtfCfgReq,          /* 1 - tightly coupled, CTF */
#else
   NULLP,                  /* 1 - tightly coupled, INVALID */
#endif
#ifdef DM 
   NULLP                   /* 2 - tightly coupled, CTF */
#else
   NULLP                   /* 2 - tightly coupled, INVALID */
#endif
};
/* CNM_DEV end */

#define WR_CTF_UE_CFG         0
#define WR_CTF_CELL_CFG       1
/* CNM_DEV */
#define WR_CTF_INITIAL_SYNC   2

/** @brief This function is responsible for Updating the transaction id.
 *
 * @details
 *
 * Function:wrIfmPhySetTrans
 *
 *   Processing steps:
 *    - Update trans id.
 *
 * @param[in]  transID: transaction Identifier
 * @param[in]  transId: transaction Identifier
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */

PRIVATE Void  wrIfmPhySetTrans
(
CtfCfgTransId                *transID,
U32                          transId
)
{
   transID->trans[7] = transId & 0x000000ff; transId >>= 8;
   transID->trans[6] = transId & 0x000000ff; transId >>= 8;
   transID->trans[5] = transId & 0x000000ff; transId >>= 8;
   transID->trans[4] = transId & 0x000000ff; transId >>= 8;
}

/** @brief This function is responsible for Sending the bind request.
 *
 * @details
 *
 * Function:WrIfmL1BndReq
 *
 *   Processing steps:
 *    - based on selector present in Post structure return appropriate function
 *       pointer.
 *
 *  @param[in]  pst: Pointer to the Post Structure
 *  @param[in]  suId: Service user Identifier
 *  @param[in]  spid: Service provider Identifier
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 WrIfmL1BndReq
(
Pst*                         pst,
SuId                         suId,
SpId                         spId
)
{
   TRC3(WrIfmL1BndReq);
   RETVALUE((*WrIfmL1BndReqMt[pst->selector])(pst, suId, spId));
} /* end of function WrIfmL1BndReq */

/** @brief This function is responsible for Sending the Unbind request.
 *
 * @details
 *
 * Function:WrIfmL1UbndReq
 *
 *   Processing steps:
 *    - based on selector present in Post structure return appropriate function
 *       pointer.
 *
 *  @param[in]  pst: Pointer to the Post Structure
 *  @param[in]  spId: Service provider Identifier
 *  @param[in]  reason: reason
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 WrIfmL1UbndReq
(
Pst                          *pst,
SpId                         spId,
Reason                       reason
)
{
   TRC3(WrIfmL1UbndReq);

   RETVALUE((*WrIfmL1UbndReqMt[pst->selector])(pst, spId, reason));
} /* end of function WrIfmL1UbndReq */

/** @brief This function is responsible for Sending the L1 config request.
 *
 * @details
 *
 * Function:WrIfmL1CfgReq
 *
 *   Processing steps:
 *    - based on selector present in Post structure return appropriate function
 *       pointer.
 *
 *  @param[in]  pst: Pointer to the Post Structure
 *  @param[in]  spId: Service provider Identifier
 *  @param[in]  transId: transaction Id
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 WrIfmL1CfgReq
(
Pst                          *pst,
SpId                         spId,
CtfCfgTransId                transId,
CtfCfgReqInfo                *cfgReqInfo
)
{
   TRC3(WrIfmL1CfgReq);

   RETVALUE((*WrIfmL1CfgReqMt[pst->selector])(pst, spId, transId, cfgReqInfo));
} /* end of function WrIfmL1CfgReq */

/** @brief This function is responsible for Sending the L1 ue Id change request.
 *
 * @details
 *
 * Function:WrIfmL1UeIdChgReq
 *
 *   Processing steps:
 *    - based on selector present in Post structure return appropriate function
 *       pointer.
 *
 *  @param[in]  pst: Pointer to the Post Structure
 *  @param[in]  spId: Service provider Identifier
 *  @param[in]  transId: transaction Id
 *  @param[in]  ueInfo: pointer to the Ue information
 *  @param[in]  newUeInfo: Pointer to New Ue info
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/


/**************************************************************
       Handler functions for message from CTF
***************************************************************/
/** @brief This function is responsible for Sending Bind confirm
 * message
 * 
 * @details
 *
 * Function:WrIfmL1BndCfm
 *
 *   Processing steps:
 *    - Check the status.
 *    - Send the alarm depends on status.
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suId: service user Id
 *  @param[in]  status: Status
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 WrIfmL1BndCfm
(
Pst                          *pst,         
SuId                         suId,
U8                           status
)
{
   TRC2(WrIfmL1BndCfm);

   RLOG0(L_DEBUG, "WrIfmL1BndCfm: CTF Bind Cfm");

   /* Send alarm */
   if (CM_BND_OK == status)
   {
      RLOG0(L_INFO, "[ALARM] Sending CTF_SAP_BND_OK Alarm to OAM ");
      wrStopTmr((PTR)wrCb.ctfSap[0],WR_TMR_CTF_SAP_BND);
      wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, 
                                                LWR_CAUSE_CTF_SAP_BOUND);
   }
   else
   {
      RLOG0(L_FATAL, "[ALARM] Sending CTF_SAP_BND_FAIL Alarm to OAM ");
      wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                                                        LCM_CAUSE_UNKNOWN);
   }

   RETVALUE(ROK);
} /* end of WrIfmL1BndCfm */

/** @brief This function is responsible for Sending COnfig confirm
 * message
 * 
 * @details
 *
 * Function:WrIfmL1CfgCfm
 *
 *   Processing steps:
 *    - Check the Transaction message type.
 *    - process phy config confirm
 *    - Send the alarm.
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suId: service user Id
 *  @param[in]  transId: transaction identifier
 *  @param[in]  status: Status
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 WrIfmL1CfgCfm
(
Pst                          *pst,             
SuId                         suId,           
CtfCfgTransId                transId,
U8                           status            
)
{
   U32                       lTransId;
   static U32                count;
   TRC3(WrIfmL1CfgCfm);

   extern Bool g_usettitmr;
   lTransId = transId.trans[4];
   lTransId = (lTransId << 8) | transId.trans[5];
   lTransId = (lTransId << 8) | transId.trans[6];
   lTransId = (lTransId << 8) | transId.trans[7];

   RLOG1(L_DEBUG, "WrIfmL1CfgCfm: CTF Cfg Cfm : Trans Id %ld",lTransId);

   if(status != CTF_CFG_CFM_OK)
   {
#ifdef ALIGN_64BIT
      RLOG1(L_ERROR, "WrIfmL1CfgCfm Status failed: %d", wrEmmCb.cellCb[0]->sib1.cellIdentity);
#else
      RLOG1(L_ERROR, "WrIfmL1CfgCfm Status failed: %ld", wrEmmCb.cellCb[0]->sib1.cellIdentity);
#endif
      /* 130095 - Removed RFAILED to handle NOK case below */
   }
   if(transId.trans[0] == WR_CTF_UE_CFG)
   {
      if(status != CTF_CFG_CFM_OK)
      {
         wrUmmPrcPhyCfgCfm(lTransId, WR_CFG_NOK);
      }
      else
      {
         wrUmmPrcPhyCfgCfm(lTransId, WR_CFG_OK);
      }
   }
   else
   { 
     if(CTF_DELETE == transId.trans[1])
     {
        if(ROK!= wrEmmCellDelSchdCfg(lTransId))
        {
           RLOG1(L_ERROR, "wrEmmCellDelSchdCfg returned failure for Cell Id: %ld", lTransId);
        }
     }
     else if(CTF_CONFIG == transId.trans[1])
     {
        if(status != CTF_CFG_CFM_OK)
        {
           wrEmmPrcPhyCfgCfm(lTransId, WR_CFG_NOK);
        }
        else
        {
#ifdef ENABLE_CNM
/*#if 0*/
           /* TODO:: Check whether SM variables can be accessed here */
           if(wrEmmCb.isCnmEnabled)
           {
              printf("\n Received CTF CellCfg Cfm.. Starting CNM for cellId %d\n",wrEmmCb.cellCb[0]->cellId);
              wrCnmTrigIctaStartReq();
           }
#endif
/*#endif*/
           wrEmmPrcPhyCfgCfm(lTransId, WR_CFG_OK);
              /* Send alarm */
              RLOG0(L_INFO, "[ALARM] Sending CELL_CFG_SUCCESS to OAM ");
              wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_CFG_SUCCESS, 
                                                       LCM_CAUSE_UNKNOWN);
            /* Send alarm */
            RLOG0(L_INFO, "[ALARM] Sending CELL_CFG_AVAILABLE to OAM ");
            wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_AVAILABLE, 
                           LCM_CAUSE_UNKNOWN);

        }
     }
     else if ((CTF_RECONFIG == transId.trans[1]) && 
           (transId.trans[8] == WR_MS_PHY_PWR_UP_CFM))
     {
        RLOG0(L_DEBUG, "PHY BRING UP DONE SENDING ALARM TO SM");
        g_usettitmr = FALSE;
        RLOG0(L_INFO, "[ALARM] Sending CELL_CFG_AVAILABLE after PHY_PWR_UP to OAM ");
        wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_AVAILABLE,
              LCM_CAUSE_UNKNOWN);
     }
     else if ((CTF_RECONFIG == transId.trans[1]) && 
           (transId.trans[8] == WR_MS_PHY_PWR_DOWN_CFM))
     {
        RLOG0(L_DEBUG, "PHY SHUTDOWN DONE SENDING ALARM TO SM");
        g_usettitmr = FALSE;
		  if (smCb.reCfg == TRUE)
		  { 
			  wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_PHY_STOP_SUCCESS,
					  LCM_CAUSE_UNKNOWN);
		  }
		  else
		  {
			  wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_AVAILABLE,
					  LCM_CAUSE_UNKNOWN);
		  }
     }
     else if ((CTF_RECONFIG == transId.trans[1]) && 
           (transId.trans[8] == WR_MS_CELL_STOP_CFM))
     {
        RLOG0(L_EVENT, "CELL STOP CFM Received from CL");
		  if (smCb.reCfg == TRUE)
		  { 
			  wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_PHY_STOP_SUCCESS,
					  LCM_CAUSE_UNKNOWN);
        }
     }
     else if ((CTF_RECONFIG == transId.trans[1]) && 
           (transId.trans[8] == WR_MS_PHY_SHUTDOWN_CFM))
     {
        RLOG0(L_DEBUG, "PHY SHUTDOWN DONE SENDING ALARM TO SM");
        g_usettitmr = FALSE;
        if (smCb.isPciMod == TRUE)
           {  
              RLOG0(L_INFO, "[ALARM] Sending CELL_AVAILABLE to OAM ");
              wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_AVAILABLE,
                 LCM_CAUSE_UNKNOWN);
              if(!IS_DUMMY_SHUTDOWN(count))
                 smCb.isPciMod = FALSE;
              count++;
           }
        else
           {  
               RLOG0(L_INFO, "[ALARM] Sending PHY_STOP_SUCCESS to OAM ");
               wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_PHY_STOP_SUCCESS,
               LCM_CAUSE_UNKNOWN);
           }
     }
     else if ((CTF_RECONFIG == transId.trans[1]) && 
           (transId.trans[8] == WR_MS_TX_PWR_DOWN_CFM) )
     {
        /* IPSEC chnages*/
        RLOG0(L_DEBUG, "TX POWER DOWN");
        RLOG0(L_INFO, "[ALARM] Sending CELL_RECFG_SUCCESS after TX_PWR_DOWN to OAM ");
        wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_PHY_TX_DOWN_SUCCESS,
              LCM_CAUSE_UNKNOWN);
        WrIfmL1EnbStopInd(pst,suId,transId);

     }
     else if ((CTF_RECONFIG == transId.trans[1]) && 
           (transId.trans[8] == WR_MS_TX_PWR_UP_CFM) )
     {
        /* IPSEC chnages*/
        RLOG0(L_DEBUG, "TX POWER UP");
        RLOG0(L_INFO, "[ALARM] Sending CELL_RECFG_SUCCESS after TX_PWR_UP to OAM ");
        wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_PHY_TX_UP_SUCCESS,
              LCM_CAUSE_UNKNOWN);

      }
      else
      {
         RLOG0(L_INFO, "SH:Received the Cell Re-Cfg Cfm");
         /* Handler for Reconfiguration Confirm */
         WrCellCb   *cellCb = NULLP;
         WR_GET_CELLCB(cellCb, lTransId);
         if(cellCb == NULLP)
         {
            RLOG_ARG0(L_ERROR, DBG_CELLID, lTransId,
                   "Failure in getting the cell CB for RECFG CFM");
            RETVALUE(RFAILED);
         }
         if(WR_CELL_STATE_RECFG == cellCb->cellState)
         {
            if(status != CTF_CFG_CFM_OK)
            {
                cmInitTimers(&(cellCb->pciModTmr), 1);
                wrStartTmr((PTR)(cellCb), WR_TMR_PCI_MOD_TMR, 6000);
                RETVALUE(ROK);
            }
            wrEmmUpdateCellState(cellCb->cellId, WR_CELL_STATE_UP);
#if 0
            RLOG0(L_INFO, "[ALARM] Sending CELL_AVAILABLE after CELL_STATE_UP to OAM ");
            wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_AVAILABLE, LCM_CAUSE_UNKNOWN);
#endif
            /*send PCI modification Indication confirm to SON*/
            wrIfmSonPciModIndCfm(cellCb);
            /*Initiate eNB config update message to all connected neighbor eNB*/
            wrEmmNghTrigCfgUpd(cellCb);
            RLOG0(L_DEBUG, "SH: Sending Alarm");  
            wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_RECFG_SUCCESS,
                                                       LCM_CAUSE_UNKNOWN);
         }
         else
         {
            if( status == WR_CFG_OK)
            {
               wrEmmPrcPhyCfgCfm(lTransId, WR_CFG_OK);
               /* Send alarm */
               RLOG0(L_INFO, "[ALARM] Sending CELL_AVAILABLE to OAM ");
               wrSendLmAlarm(LCM_CATEGORY_PROTOCOL, LWR_EVENT_CELL_AVAILABLE, 
                     LCM_CAUSE_UNKNOWN);
            }
            else
            {
               wrEmmPrcPhyCfgCfm(lTransId, WR_CFG_NOK);
               /* Send alarm */
            }
         }
      }
   }
   RETVALUE(ROK);
}  /* end of WrIfmL1CfgCfm */

/** @brief This function is responsible for handling ENB-STOP Indication from PHY
 *
 * @details
 *
 * Function:WrIfmL1EnbStopInd
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suId: service user Id
 *  @param[in]  transId: transaction identifier
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 WrIfmL1EnbStopInd
(
Pst                          *pst,
SuId                         suId,
CtfCfgTransId                transId
)
{
   U32                       lTransId;
   WrCellCb                  *cellCb = NULLP;

   TRC3(WrIfmL1EnbStopInd);

   lTransId = transId.trans[4];
   lTransId = (lTransId << 8) | transId.trans[5];
   lTransId = (lTransId << 8) | transId.trans[6];
   lTransId = (lTransId << 8) | transId.trans[7];
   
   WR_GET_CELLCB(cellCb, lTransId);
   if(cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, lTransId,"Failure in getting the cell CB for eNB STOP IND");
      RETVALUE(RFAILED);
   }
   /*Send cell reconfiguration to PHY IF PCI modification is triggered by SON*/
   
   if(WR_CELL_STATE_RECFG == cellCb->cellState)
   {
      wrFillCtfCellReCfgReq(lTransId);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for Sending Ue Id change confirm
 * message
 * 
 * @details
 *
 * Function:WrIfmL1UeIdChgCfm
 *
 *   Processing steps:
 *    - Process ue id change message.
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suId: service user Id
 *  @param[in]  transId: transaction identifier
 *  @param[in]  ueInfo: Pointer to Ue information
 *  @return S16
 *       SUCCESS : ROK
*/
PRIVATE S16 WrIfmL1UeIdChgCfm
(
Pst                          *pst,            
SuId                         suId,
CtfCfgTransId                transId,
CtfUeInfo                    *ueInfo, 
U8 status
)  
{
   TRC3(WrIfmL1UeIdChgCfm)

   RLOG0(L_DEBUG, "WrIfmL1UeIdChgCfm: CTF UeId Change Cfm");

   RETVALUE(ROK);
}  /* end of WrIfmL1UeIdChgCfm */

#ifdef TENB_AS_SECURITY
/** @brief This function is responsible for Sending Ue Id change confirm
 * message
 * 
 * @details
 *
 * Function:WrIfmL1UeIdChgCfm
 *
 *   Processing steps:
 *    - Process ue id change message.
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suId: service user Id
 *  @param[in]  transId: transaction identifier
 *  @param[in]  ueInfo: Pointer to Ue information
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 WrIfmL1KdfCfm
(
Pst                          *pst,            
SuId                         suId,
CtfCfgTransId                transId,
CtfKdfCfmInfo                *kdfCfmInf, 
U8 status
)
{
   U32                       lTransId;

   TRC3(WrIfmL1KdfCfm);
   
   lTransId = transId.trans[4];
   lTransId = (lTransId << 8) | transId.trans[5];
   lTransId = (lTransId << 8) | transId.trans[6];
   lTransId = (lTransId << 8) | transId.trans[7];

   RLOG1(L_DEBUG, "WrIfmL1CfgCfm: CTF Cfg Cfm : Trans Id %ld",lTransId);

   if(status != CTF_CFG_CFM_OK)
   {
#ifdef ALIGN_64BIT
      RLOG1(L_ERROR, "WrIfmL1CfgCfm Status failed: %d", wrEmmCb.cellCb[0]->sib1.cellIdentity);
#else
      RLOG1(L_ERROR, "WrIfmL1CfgCfm Status failed: %ld", wrEmmCb.cellCb[0]->sib1.cellIdentity);
#endif
      RETVALUE(RFAILED);
   }
   if(status != CTF_CFG_CFM_OK)
   {
      wrUmmPrcKdfCfm(lTransId, WR_CFG_NOK, kdfCfmInf);
   }
   else
   {
      wrUmmPrcKdfCfm(lTransId, WR_CFG_OK, kdfCfmInf);
   }

   RETVALUE(ROK);
}  /* end of WrIfmL1UeIdChgCfm */
#endif

/* CNM_DEV start */
/** @brief This function is responsible for processing CNM Initial Sync response
 * 
 * @details
 *
 * Function: WrIfmL1CnmInitialSyncRsp
 *
 *   Processing steps:
 *    - Forward the CNM Initial Sync Response to EMM
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suId: service user Id
 *  @param[in]  transId: transaction identifier
 *  @param[in]  cnmInitialSyncRsp: CNM Initial Sync Response Information
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 WrIfmL1CnmInitialSyncRsp
(
Pst                          *pst,            
SuId                         suId,
CtfCfgTransId                *transId,
CtfCnmInitSyncRsp            *cnmInitSyncRsp
)
{
   U32                       lTransId;

   TRC3(WrIfmL1CnmInitialSyncRsp);

   lTransId = transId->trans[4];
   lTransId = (lTransId << 8) | transId->trans[5];
   lTransId = (lTransId << 8) | transId->trans[6];
   lTransId = (lTransId << 8) | transId->trans[7];

   RLOG1(L_DEBUG, "CNM Initial Sync Response : Trans Id %ld", lTransId);

   wrEmmCnmPrcInitSyncRsp((U16)lTransId, cnmInitSyncRsp);

   WR_FREE(cnmInitSyncRsp, sizeof(CtfCnmInitSyncRsp));

   RETVALUE(ROK);
}  /* end of WrIfmL1CnmInitialSyncRsp */

/** @brief This function is responsible for processing CNM Sync Indication
 * 
 * @details
 *
 * Function: WrIfmL1CnmSyncInd
 *
 *   Processing steps:
 *    - Forward the CNM Sync Indication to EMM
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suId: service user Id
 *  @param[in]  transId: transaction identifier
 *  @param[in]  cnmSyncInd: CNM Initial Sync Indication Information
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 WrIfmL1CnmSyncInd
(
Pst                          *pst,            
SuId                         suId,
CtfCfgTransId                *transId,
CtfCnmCellSyncInd            *cnmSyncInd
)
{
   U32                       lTransId;

   TRC3(WrIfmL1CnmSyncInd);
   
   lTransId = transId->trans[4];
   lTransId = (lTransId << 8) | transId->trans[5];
   lTransId = (lTransId << 8) | transId->trans[6];
   lTransId = (lTransId << 8) | transId->trans[7];

   RLOG1(L_DEBUG, "CNM Sync Status Indication: Trans Id %ld", lTransId);

   wrEmmCnmPrcSyncInd((U16)lTransId, cnmSyncInd);

   WR_FREE(cnmSyncInd, sizeof(CtfCnmCellSyncInd));

   RETVALUE(ROK);
}  /* end of WrIfmL1CnmSyncInd */

/** @brief This function is responsible for processing CNM Sync Indication
 * 
 * @details
 *
 * Function: WrIfmL1CnmSyncRsp
 *
 *   Processing steps:
 *    - Forward the CNM Sync Indication to EMM
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suId: service user Id
 *  @param[in]  transId: transaction identifier
 *  @param[in]  cnmSyncInd: CNM Initial Sync Indication Information
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 WrIfmL1CnmSyncRsp
(
Pst                          *pst,            
SuId                         suId,
CtfCfgTransId                *transId,
CtfCnmCellSyncRsp            *cnmSyncRsp
)
{
   U32                       lTransId;

   TRC3(WrIfmL1CnmSyncRsp);
   
   lTransId = transId->trans[4];
   lTransId = (lTransId << 8) | transId->trans[5];
   lTransId = (lTransId << 8) | transId->trans[6];
   lTransId = (lTransId << 8) | transId->trans[7];

   RLOG1(L_DEBUG, "CNM Initial Sync Response : Trans Id %ld", lTransId);

   if(CTF_CFG_CFM_OK != cnmSyncRsp->status)
   {
#ifdef ALIGN_64BIT
      RLOG1(L_ERROR, "Status failed: %d", cnmSyncRsp->status);
#else
      RLOG1(L_ERROR, "Status failed: %ld", cnmSyncRsp->status);
#endif
      RETVALUE(RFAILED);
   }
   wrEmmCnmPrcSyncRsp((U16)lTransId, cnmSyncRsp);

   WR_FREE(cnmSyncRsp, sizeof(CtfCnmCellSyncRsp));

   RETVALUE(ROK);
}  /* end of WrIfmL1CnmSyncRsp */

/* CNM_Dev end */
/**************************************************************
 CTF Interface wrapper functions
***************************************************************/
/** @brief This function is responsible for Sending CTF Bind confirm
 * message
 * 
 * @details
 *
 * Function:TfLiCtfBndCfm
 *
 *   Processing steps:
 *    - Process Li bind confirm message.
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suId: service user Id
 *  @param[in]  status: status
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 TfLiCtfBndCfm
(
Pst                          *pst,         
SuId                         suId,
U8                           status
)
{
   TRC2(TfLiCtfBndCfm);
   RETVALUE(WrIfmL1BndCfm(pst, suId, status));
} /* end of TfLiCtfBndCfm */

/** @brief This function is responsible for Sending CTF config confirm
 * message
 * 
 * @details
 *
 * Function:TfLiCtfCfgCfm
 *
 *   Processing steps:
 *    - Process L1 CTF config confirm message.
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suId: service user Id
 *  @param[in]  transId: transaction Id
 *  @param[in]  status: status
 *  @return S16
 *       SUCCESS : ROK
*/
PUBLIC S16 TfLiCtfCfgCfm
(
Pst                          *pst,
SuId                         suId,
CtfCfgTransId                transId,
U8                           status
)
{
   TRC3(TfLiCtfCfgCfm);
   RETVALUE(WrIfmL1CfgCfm(pst, suId, transId, status));
}   /* end of TfLiCtfCfgCfm */

/** @brief This function is responsible for Sending CTF UE id change confirm
 * message
 * 
 * @details
 *
 * Function:TfLiCtfUeIdChgCfm
 *
 *   Processing steps:
 *    - Process UeId change confirm message.
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suid: service user Id
 *  @param[in]  transId: transaction Id
 *  @param[in]  ueInfo: ue information
 *  @param[in]  status: Status
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 TfLiCtfUeIdChgCfm
(
Pst                          *pst,
SuId                         suId,
CtfCfgTransId                transId,
CtfUeInfo                    *ueInfo,
U8                           status
)  
{
   TRC3(TfLiCtfUeIdChgCfm);
   RETVALUE(WrIfmL1UeIdChgCfm(pst, suId, transId, ueInfo, status));
}  /* end of TfLiCtfUeIdChgCfm */

/* CNM_DEV start */
/** @brief This function is responsible for forwarding CNM Sync Response
 * 
 * @details
 *
 * Function: TfLiCtfCnmInitialSyncRsp
 *
 *   Processing steps:
 *    - Forward CNM Initial Sync Response to IFM
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suid: service user Id
 *  @param[in]  transId: transaction Id
 *  @param[in]  cnmInitialSyncRsp: CNM Initial Sync Response Information
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 TfLiCtfCnmInitialSyncRsp
(
Pst                          *pst,
SuId                         suId,
CtfCfgTransId                *transId,
CtfCnmInitSyncRsp            *cnmInitSyncRsp
)  
{
   TRC3(TfLiCtfCnmInitialSyncRsp);
   RETVALUE(WrIfmL1CnmInitialSyncRsp(pst, suId, transId, cnmInitSyncRsp));
}  /* end of TfLiCtfCnmInitialSyncRsp */

/** @brief This function is responsible for forwarding CNM Sync Indication
 * 
 * @details
 *
 * Function: TfLiCtfCnmSyncInd
 *
 *   Processing steps:
 *    - Forward CNM Sync Indication to IFM
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suid: service user Id
 *  @param[in]  transId: transaction Id
 *  @param[in]  cnmSyncInd: CNM Sync Indication Information
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 TfLiCtfCnmSyncInd
(
Pst                          *pst,
SuId                         suId,
CtfCfgTransId                *transId,
CtfCnmCellSyncInd            *cnmSyncInd
)  
{
   TRC3(TfLiCtfCnmSyncInd);
   RETVALUE(WrIfmL1CnmSyncInd(pst, suId, transId, cnmSyncInd));
}  /* end of TfLiCtfCnmSyncInd */

/** @brief This function is responsible for forwarding CNM Sync Response
 * 
 * @details
 *
 * Function: TfLiCtfCnmSyncRsp
 *
 *   Processing steps:
 *    - Forward CNM Sync Response to IFM
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suid: service user Id
 *  @param[in]  transId: transaction Id
 *  @param[in]  cnmSyncRsp: CNM Sync Response Information
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 TfLiCtfCnmSyncRsp
(
Pst                          *pst,
SuId                         suId,
CtfCfgTransId                *transId,
CtfCnmCellSyncRsp            *cnmSyncRsp
)  
{
   TRC3(TfLiCtfCnmSyncRsp);
   RETVALUE(WrIfmL1CnmSyncRsp(pst, suId, transId, cnmSyncRsp));
}  /* end of TfLiCtfCnmSyncRsp */

/* CNM_DEV end */

/** @brief This function is responsible for Sending phy cell configuration.
 *
 * @details
 *
 * Function:wrIfmPhyCellCfg
 *
 *   Processing steps:
 *    - Process L1 cell config request.
 *
 *  @param[in]  cellCfgReq: pointer to the cell Config request
 *  @param[in]  transId: transaction Id
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrIfmPhyCellCfg
(
CtfCfgReqInfo                *cellCfgReq,
U32                          transId
)
{
   Pst                       *pst = NULLP;
   CtfCfgTransId             transID;

   TRC2(wrIfmPhyCellCfg);

   RLOG0(L_DEBUG, "wrIfmPhyCellCfg: Triggering Cell Cfg Req over the CTF Interface");

   /* Copying the pst structure */
   cmMemset((U8 *)&transID, '\0', sizeof(CtfCfgTransId));
   transID.trans[0] = WR_CTF_CELL_CFG;
   transID.trans[1] = cellCfgReq->cfgType;
   pst              = &wrCb.ctfSap[0]->pst;
   pst->event       = EVTCTFCFGREQ;
   wrIfmPhySetTrans(&transID, transId);
 
   if(WrIfmL1CfgReq(pst, wrCb.ctfSap[0]->spId, transID, cellCfgReq) != ROK)
   {
      RLOG0(L_ERROR, "WrIfmL1CfgReq failed.");      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/*wrIfmPhyCellCfg*/

/** @brief This function is responsible for Sending of CTF UE Config request.
 *
 * @details
 *
 * Function:wrIfmPhyUeCfg
 *
 *   Processing steps:
 *    - Process L1 config request.
 *
 *  @param[in]  uecfg: pointer to the Ue Config
 *  @param[in]  transId: transaction Id
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrIfmPhyUeCfg
(
CtfCfgReqInfo                *ueCfg, 
U32                          transId
)
{
   Pst                       *pst = NULLP;
   CtfCfgTransId             transID;

   TRC2(wrIfmPhyUeCfg);

   RLOG1(L_DEBUG, "Triggering UE Cfg Req over the CTF Interface TransId %ld", transId);

   /* Copying the pst structure */
   cmMemset((U8 *)&transID, '\0', sizeof(CtfCfgTransId));
   transID.trans[0] = WR_CTF_UE_CFG;
   pst              = &wrCb.ctfSap[0]->pst;
   pst->event       = EVTCTFCFGREQ;
   wrIfmPhySetTrans(&transID, transId);

   if(WrIfmL1CfgReq(pst, wrCb.ctfSap[0]->spId, transID, ueCfg) != ROK)
   {
      RLOG0(L_ERROR, "WrIfmL1CfgReq failed.");      
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/*wrIfmPhyUeCfg*/

#ifdef TENB_AS_SECURITY
/** @brief This function is responsible for Sending of CTF UE Config request.
 *
 * @details
 *
 * Function:wrIfmPhyUeCfg
 *
 *   Processing steps:
 *    - Process L1 config request.
 *
 *  @param[in]  uecfg: pointer to the Ue Config
 *  @param[in]  transId: transaction Id
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrIfmPhyKdfReq
(
CtfKdfReqInfo                *kdfReqInf, 
U32                          transId
)
{
   Pst                       *pst = NULLP;
   CtfCfgTransId             transID;

   TRC2(wrIfmPhyKdfReq);

   RLOG0(L_DEBUG, "wrIfmPhyUeCfg: Triggering UE Cfg Req over the CTF Interface");

   /* Copying the pst structure */
   cmMemset((U8 *)&transID, '\0', sizeof(CtfCfgTransId));
   transID.trans[0] = WR_CTF_UE_CFG;
   pst              = &wrCb.ctfSap[0]->pst;
   pst->event       = EVTCTFKDFREQ;
   wrIfmPhySetTrans(&transID, transId);

   RETVALUE((*WrIfmL1KdfReqMt[pst->selector])(pst, wrCb.ctfSap[0]->spId, transID, kdfReqInf)); 

}/*wrIfmPhyUeCfg*/
#endif

/* CNM_DEV start */
/** @brief This function is responsible for sending CNM Initial Sync Request.
 *
 * @details
 *
 * Function: wrIfmPhyCnmInitialSyncReq
 *
 *   Processing steps:
 *    - Process L1 CNM Initial Sync Request
 *
 *  @param[in]  uecfg: pointer to the Ue Config
 *  @param[in]  transId: transaction Id
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrIfmPhyCnmInitialSyncReq
(
CtfCnmInitSyncReq            *cnmInitSyncReq,
U32                          transId
)
{
   Pst                       *pst = NULLP;
   CtfCfgTransId             transID;

   TRC2(wrIfmPhyInitialSyncReq);

   RLOG0(L_DEBUG, "Triggering Initial Sync Request over the CTF Interface");

   /* Copying the pst structure */
   cmMemset((U8 *)&transID, '\0', sizeof(CtfCfgTransId));
   transID.trans[0] = WR_CTF_INITIAL_SYNC;
   pst              = &wrCb.ctfSap[0]->pst;
   pst->event       = EVTCTFCNMINITSYNCREQ;
   wrIfmPhySetTrans(&transID, transId);

   RETVALUE((*WrIfmL1InitialSyncReqMt[pst->selector])(pst, wrCb.ctfSap[0]->spId,
            &transID, cnmInitSyncReq));

} /* end of wrIfmPhyInitialSyncReq */

/** @brief This function is responsible for sending CNM Initial Sync Request.
 *
 * @details
 *
 * Function: wrIfmPhyCnmCellSyncReq
 *
 *   Processing steps:
 *    - Process L1 CNM Initial Sync Request
 *
 *  @param[in]  uecfg: pointer to the Ue Config
 *  @param[in]  transId: transaction Id
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrIfmPhyCnmCellSyncReq
(
CtfCnmCellSyncReq            *cnmCellSyncReq,
U32                          transId
)
{
   Pst                       *pst = NULLP;
   CtfCfgTransId             transID;

   TRC2(wrIfmPhyInitialSyncReq);

   RLOG0(L_DEBUG, "Triggering Initial Sync Request over the CTF Interface");

   /* Copying the pst structure */
   cmMemset((U8 *)&transID, '\0', sizeof(CtfCfgTransId));
   transID.trans[0] = WR_CTF_INITIAL_SYNC;
   pst              = &wrCb.ctfSap[0]->pst;
   pst->event       = EVTCTFCNMSYNCREQ;
   wrIfmPhySetTrans(&transID, transId);

   RETVALUE((*WrIfmL1CellSyncReqMt[pst->selector])(pst, wrCb.ctfSap[0]->spId,
            &transID, cnmCellSyncReq));

} /* end of wrIfmPhySyncReq */
/* CNM_DEV end */

/* Periodic REM Scan for TPM */
/** @brief This function is responsible for forwarding Peridoc Cell Srach
 * Response to SON.
 * 
 * @details
 *
 * Function: TfLiCtfPerCellSrchRsp
 *
 *   Processing steps:
 *    - Forward Periodic Cell search Response to SON
 *
 *  @param[in]  pst: pointer to the Post structure
 *  @param[in]  suid: service user Id
 *  @param[in]  transId: transaction Id
 *  @param[in]  CtfPeriodicRemCellSearchRsp: Cell Search Response
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 TfLiCtfPerCellSrchRsp
(
Pst                          *pst,
SuId                         suId,
CtfCfgTransId                *transId,
CtfPeriodicRemCellSearchRsp  *perCellSrchRsp
)  
{
   TRC3(TfLiCtfPerCellSrchRsp);
   if(smCb.cellState == TRUE)  /* ccpu00146885 */
   {
      RETVALUE(WrIfmSonPerCellSrchRsp(perCellSrchRsp));
   }
   else
   {
      RLOG0(L_INFO,"Cell State is DOWN!!! Drop Periodic Cell Search Reports \n");
   }

   RETVALUE(ROK);
}  /* end of TfLiCtfPerCellSrchRsp */

/* Periodic REM Scan for TPM End */


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
