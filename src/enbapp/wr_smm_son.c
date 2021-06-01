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

     Name:     Eson Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_smm_son.c

     Sid:      fc_smm_son.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/3 - Tue Jan  6 18:36:16 2015

     Prg:      subhamay 

*********************************************************************21*/


/* header include files */ 
#include "wr.h"
#include "wr_cmn.h"
#include "wr_smm_init.h"
#include "wr_smm_smallcell.h"
#include "wr_utils.h"
#include "wr_lwr.h"
#include "lnl.h"
#include "nlu.h"
#ifdef WR_RSYS_OAM
#include "wr_msm_common.h"
#endif
#include "lnl.x"
#include "nlu.x"
#include "stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */

static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_FILE_ID=300;
static int RLOG_MODULE_ID=2;

#ifdef WR_RSYS_OAM
EXTERN Void FapAlarmInd(U32 alarmId, U8 severity, U8 causeType, U8 causeVal);
EXTERN Void FapAlarmIndWithAddInfo(MsmUpdateAlarms *smUpdateAlarms);
#endif

PRIVATE S16 smSendNlGenCfg      ARGS((
Void
));

PRIVATE  S16 smSendNlNluUSapCfg ARGS((
SpId spId
));

PRIVATE S16 smSendNlCellCfg     ARGS((
Void
));

PRIVATE S16 smSendNlNghCellCfg     ARGS((
Void
));

PRIVATE U8  wrNlCfg = 0;
/*
*       Desc:  Invoked to create ESON TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_eson.c
*/
#ifdef ANSI
PUBLIC S16 smNlInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smNlInit(sysTskId)
SSTskId    sysTskId;
#endif
{
   TRC2(smNlInit);

   /* Register SON TAPA Task */
   if (SRegTTsk((Ent)ENTNL, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR2,
            nlActvInit, (ActvTsk)nlActvTsk) != ROK) 
   {
      RLOG0(L_ERROR, "SON tapa task registration failed");
      RETVALUE(RFAILED);
   } /* end of if statement */
   /* Attach SON TAPA Task */
   if (SAttachTTsk((Ent)ENTNL, (Inst)0, sysTskId)!= ROK) 
   {
      RLOG0(L_ERROR, "SON tapa task registratioin with system task failed");
      RETVALUE(RFAILED);
   } /* end of if statement */

   RETVALUE(ROK);
} /* end of smNlInit */

/*
*
*       Fun:   Activate Task - initialize
*
*       Desc:  Invoked by system services to initialize a task.
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/

#ifdef ANSI
PUBLIC S16 smNlActvInit
(
Ent ent,                      /* entity */
Inst inst,                    /* instance */
Region region,                /* region */
Reason reason                 /* reason */
)
#else
PUBLIC S16 smNlActvInit(ent, inst, region, reason)
Ent ent;                      /* entity */
Inst inst;                    /* instance */
Region region;                /* region */
Reason reason;                /* reason */
#endif
{
   TRC3(smNlActvInit)

   UNUSED(ent);
   UNUSED(inst);
   UNUSED(region);
   UNUSED(reason);
   RETVALUE(ROK);
}

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from NL
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   wr_smm_son.c
*
*/

#ifdef ANSI
PUBLIC S16 smNlActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 smNlActvTsk(pst, mBuf)
Pst *pst;                   /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   S16 ret;

   TRC2(smNlActvTsk)

   ret = ROK;

   switch(pst->event)
   {
      case EVTLNLCFGCFM: 
         ret = cmUnpkLnlCfgCfm(SmMiLnlCfgCfm, pst, mBuf);
         break;      
      case EVTLNLCNTRLCFM:
         ret = cmUnpkLnlCntrlCfm(SmMiLnlCntrlCfm, pst, mBuf);
         break;
      case EVTLNLCFGUPDATEIND :
         ret = cmUnpkLnlCfgUpdateInd (SmMiLnlCfgUpdateInd, pst, mBuf);  
         break;
      case EVTLNLSTAIND :
         ret = cmUnpkLnlStatInd(SmMiLnlStatInd, pst, mBuf);
         break;
      case EVTLNLSONCFGIND :
         ret = cmUnpkLnlSonCfgInd(SmMiLnlSonCfgInd, pst, mBuf);
         break;
      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         RLOG1(L_ERROR, "Invalid event [%d] received", pst->event);
         break;
   }
   RETVALUE(ret);
} /* end of smNlActvTsk */

/*
*
*       Fun:   update BRDCM SON Configuration
*
*       Desc:  This function is used to handle BRDCM-SON configuration
*              received from SON module
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnlSonCfgInd
(
Pst         *pst,
LnlSonCfg   *sonCfg
)
#else
PUBLIC S16 SmMiLnlSonCfgInd(pst, sonCfg)
Pst          *pst;
LnlSonCfg    *sonCfg;
#endif
{
   if(LNL_SON_PERD_UE_MEAS_INTERVAL_CFG == sonCfg->cfgType) 
   {
      /*update periodic measurement time to trigger value*/
      smUpdateNlMeasRptInterval(sonCfg->u.measCfg.periodicUeMeasInterval);
   }
   else if(LNL_SON_SCHD_CFG == sonCfg->cfgType)
   {
      /*trigger scheduler configuration to eNB-APP*/
      smSndWrSchdPrbPaUpdateReq(sonCfg);
   }
   else if(LNL_SON_PERD_REM_SCAN_CFG == sonCfg->cfgType)
   {
      /* Update into global structure here */
      /* Once Cell comes up then send REM Scan Request to BRDCM-CL */
      smCb.cfgCb.remScanInterval = sonCfg->u.remScanCfg.perdREMScanInterval;
      smCb.cfgCb.remScanCount = sonCfg->u.remScanCfg.perdREMScanCount;

      RLOG2(L_INFO,"SmMiLnlSonCfgInd:: remScanInt[%d] remScanCount[%d]",
             smCb.cfgCb.remScanInterval,smCb.cfgCb.remScanCount);
   }

   SPutSBuf(pst->region, pst->pool, (Data*)sonCfg, sizeof(LnlSonCfg));
   RETVALUE(ROK);
}

/*
*
*       Fun:   update EARFCN/PCI/PRACH Configuration
*
*       Desc:  This function is used to handle EARFCN/PCI/PRACH update
*              configuration indication
*              received from SON module
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnlCfgUpdateInd
(
Pst                     *pst,
NlCfgUpdateInd          *cfgUpdateInd
)
#else
PUBLIC S16 SmMiLnlCfgUpdateInd(pst, cfgUpdateInd)
Pst                     *pst;
NlCfgUpdateInd          *cfgUpdateInd;
#endif
{
   Pst      nlPst;
   TRC3(SmMiLnlCfgUpdateInd);
   if(LNL_CFG_UPDATE_PCI == cfgUpdateInd->cfgType)
   {
      SmMiLnlUpdatePciCfgInd(&cfgUpdateInd->u.pciCfgInd);
   }
   else if(LNL_CFG_UPDATE_EARFCN == cfgUpdateInd->cfgType)
   {
      SmMiLnlUpdateEarfcnCfgInd(&cfgUpdateInd->u.earfcnCfgInd);
   }
   else if(LNL_CFG_UPDATE_PRACH == cfgUpdateInd->cfgType)
   {
      SmMiLnlUpdatePrachCfgInd(&cfgUpdateInd->u.prachCfgInd);
   }
   else if(LNL_CFG_UPDATE_TXPWR == cfgUpdateInd->cfgType)
   {
      SmMiLnlUpdateTxPwrCfgInd(&cfgUpdateInd->u.txPwrCfgInd);
   }

   /* Uday: Ensure Self-Post of TUCL configuration is done only if
    * smCb.smState is WR_SM_STATE_INIT and wrNlCfg is set to SM_NL_CONFIGURED */
   if((smCb.smState == WR_SM_STATE_INIT) && (wrNlCfg == SM_NL_CONFIGURED))
   {
      smCb.smState = WR_SM_STATE_SON_CFG_DONE;
      RLOG0(L_DEBUG, "SM: Doing a self post of EVTWRTUCLCFG");
      WR_FILL_PST(nlPst, SM_NL_PROC, ENTSM, ENTWR, EVTWRTUCLCFG, WR_POOL, SM_SELECTOR_LC);
      if(wrSmSelfPstMsg(&nlPst) != ROK)
      {
         RLOG0(L_ERROR,"wrSmSelfPstMsg:SON CONFIGURATION SUCCESSFULLY"
         "PERFORMED. Failure at Self Pst");
      }
      else
      {
         RLOG0(L_DEBUG,"SON CONFIGURATION SUCCESSFULLY PERFORMED.");
      }
   }

   SPutSBuf(pst->region, pst->pool, (Data*)cfgUpdateInd, sizeof(NlCfgUpdateInd));
   RETVALUE(ROK);
}


/*
*
*       Fun:   update PRACH Configuration
*
*       Desc:  This function is used to handle prach configuration  update
*              indication received from SON module
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnlUpdatePrachCfgInd
(
NlUpdatePrachCfgInd     *prachCfgInd
)
#else
PUBLIC S16 SmMiLnlUpdatePrachCfgInd(prachCfgInd)
NlUpdatePrachCfgInd     *prachCfgInd;
#endif
{
   TRC3(SmMiLnlUpdatePrachCfgInd);
   smUpdateNlPrachVal(prachCfgInd);
   /*Inform to HeMs TODO*/
   if(WR_SM_STATE_INIT == smCb.smState)
   {
      wrNlCfg |= SM_NL_PRACH_CFG_IND;
   }
   RETVALUE(ROK);
}

/*
*
*       Fun:   update TxPower Configuration
*
*       Desc:  This function is used to handle tx power configuration update
*              indication received from SON module
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnlUpdateTxPwrCfgInd
(
NlUpdateTxPwrCfgInd     *txPwrCfgInd
)
#else
PUBLIC S16 SmMiLnlUpdateTxPwrCfgInd(txPwrCfgInd)
NlUpdateTxPwrCfgInd     *txPwrCfgInd;
#endif
{
   TRC3(SmMiLnlUpdateTxPwrCfgInd);
   RLOG0(L_DEBUG, "At SM: Received Tx Pwr Cfg Indication");
   smUpdateNlTxPwrVal(txPwrCfgInd);
   if(WR_SM_STATE_INIT == smCb.smState)
   {
      wrNlCfg |= SM_NL_TXPWR_CFG_IND;
   }
   RLOG0(L_DEBUG, "Done with function SmMiLnlUpdateTxPwrCfgInd");
   RETVALUE(ROK);
}


/*
*
*       Fun:   update EARFCN Configuration
*
*       Desc:  This function is used to handle EARFCN update
*              configuration indication
*              received from SON module
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnlUpdateEarfcnCfgInd
(
NlUpdateEarfcnCfgInd    *earfcnCfgInd
)
#else
PUBLIC S16 SmMiLnlUpdateEarfcnCfgInd(earfcnCfgInd)
NlUpdateEarfcnCfgInd    *earfcnCfgInd;
#endif
{
   TRC3(SmMiLnlUpdateEarfcnCfgInd);
   smUpdateNlEarfcnVal(earfcnCfgInd);
   /*update the PCI and EARFCN information to HeMS TODO*/
   if(WR_SM_STATE_INIT == smCb.smState)
   {
      wrNlCfg |= SM_NL_EARFCN_CFG_IND;
   }
   RETVALUE(ROK);
}

/*
*
*       Fun:   update pci Configuration
*
*       Desc:  This function is used to handle pci update
*              configuration indication
*              received from SON module
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnlUpdatePciCfgInd
(
NlUpdatePciCfgInd       *pciCfgInd
)
#else
PUBLIC S16 SmMiLnlUpdatePciCfgInd(pciCfgInd)
NlUpdatePciCfgInd       *pciCfgInd;
#endif
{
   TRC3(SmMiLnlUpdatePciCfgInd);
   smUpdateNlPciVal(pciCfgInd->pci);

   if(ROK != smWrValidateConfigParams())
   {
      RLOG0(L_ERROR, "smWrValidateConfigParams Failed");
      RETVALUE(RFAILED);
   }

   /*Inform about PCI update to HeMS TODO*/
   switch(smCb.smState)
   {
      case WR_SM_STATE_INIT :
      {
         wrNlCfg |= SM_NL_PCI_CFG_IND;
      }
      break;
      case WR_SM_STATE_CELL_UP :
#ifdef WR_RSYS_OAM
      case WR_SM_STATE_OAM_DYN_START :
#endif
      {
         smSndWrPciUpdateReq(pciCfgInd); 
      }
      default :
      {
         break;
      }
   }
   RETVALUE(ROK);
}

#ifdef WR_RSYS_OAM

#ifdef ANSI
PRIVATE S16 wrSmMapSONalarmIndtoOamAlarms
(
MsmUpdateAlarms *alarm,
LnlUstaDgn *dgn
)
#else
PRIVATE S16 wrSmMapSONalarmIndtoOamAlarms(alarm, dgn)
(
MsmUpdateAlarms *alarm;
LnlUstaDgn *dgn;
)
#endif
{
   if(dgn->alarmId == LNL_ALARM_PCI_CONFUSION)
   {
      RLOG3(L_INFO,"PCI Confusion alarm received at SM, Severity is %d, CellID %d, Action %d",
            dgn->severity, dgn->u.addInfo.cellId, dgn->u.addInfo.action);
      alarm->alarmId = PCI_CONFUSION;
   }
   else if(dgn->alarmId == LNL_ALARM_PCI_CONFLICT)
   {
      RLOG3(L_INFO,"PCI Collision alarm received at SM, Severity is %d, CellID %d, Action %d",
            dgn->severity, dgn->u.addInfo.cellId, dgn->u.addInfo.action);
      alarm->alarmId = PCI_COLLISION;
   }

   alarm->severity = dgn->severity;
   alarm->causeValue = dgn->causeVal;
   alarm->causeType = dgn->causeType;

   /* Add Additional Info */
   if((dgn->alarmId == LNL_ALARM_PCI_CONFUSION) || 
      (dgn->alarmId == LNL_ALARM_PCI_CONFLICT))
   {
      alarm->isAdditionalInfo = TRUE;
      alarm->t.info.pci = dgn->u.addInfo.pci;
      alarm->t.info.actionType =  dgn->u.addInfo.action;
      alarm->t.info.cellId =  dgn->u.addInfo.cellId;
   }
   else
   {
      alarm->isAdditionalInfo = TRUE;
   }
   RETVALUE(ROK);
}
#endif

#ifdef ANSI
PUBLIC S16 SmMiLnlStatInd
(
Pst                     *pst,
NlMngmt                 *mngmt
)
#else
PUBLIC S16 SmMiLnlStatInd(pst, mngmt)
Pst                     *pst;
NlMngmt                 *mngmt;
#endif
{
   LnlUstaDgn   *dgn = NULLP;
#ifdef WR_RSYS_OAM
   MsmUpdateAlarms oamAlarm;
#endif
   TRC3(SmMiLnlStatInd);
   if(LNL_STAT_ALARM == mngmt->u.usta.staType)
   {
      dgn = &(mngmt->u.usta.u.stAlarm.dgn);
      /*send the alarm info to OAM*/
#ifdef WR_RSYS_OAM
         wrSmMapSONalarmIndtoOamAlarms(&oamAlarm, dgn);
         FapAlarmIndWithAddInfo(&oamAlarm);
#endif
   }
   SPutSBuf(pst->region, pst->pool, (Data*)mngmt, sizeof(NlMngmt));
   RETVALUE(ROK);
}

/*
*
*       Fun:   Control Confirm
*
*       Desc:  This function is used by to present control confirm
*              information to Layer Managemer.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLnlCntrlCfm
(
Pst            *pst,
NlMngmt        *cfm
)
#else
PUBLIC S16 SmMiLnlCntrlCfm(pst, cfm)
Pst            *pst;
NlMngmt        *cfm;
#endif
{
   Pst      nlPst;
   TRC2(SmMiLnlCntrlCfm)

   SM_SET_ZERO(&nlPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      if (smCb.smState == WR_SM_STATE_APP_SHUTDWN_DONE)
      {
          smCb.smState = WR_SM_STATE_SON_SHUTDWN_DONE;
          WR_FILL_PST(nlPst, SM_NL_PROC, ENTSM, ENTWR, EVTWRRRCSHUT, nlMemMngr.nlPool, SM_SELECTOR_LC);

          if(wrSmSelfPstMsg(&nlPst) != ROK)
          {
               RLOG0(L_WARNING, "SON SHUTDOWN SUCCESSFULLY PERFORMED. "
                   "Failure at Self Pst.");
          }
          else
          {
               RLOG0(L_INFO, "SON SHUTDOWN SUCCESSFULLY PERFORMED. ");
          }
      }
   } 
   SPutSBuf(pst->region, pst->pool, (Data*)cfm, sizeof(NlMngmt)); 
   RETVALUE(ROK);
}

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Managemer.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLnlCfgCfm
(
Pst            *pst,
NlMngmt        *cfm
)
#else
PUBLIC S16 SmMiLnlCfgCfm(pst, cfm)
Pst            *pst;
NlMngmt        *cfm;
#endif
{
   Pst      nlPst;

   TRC2(SmMiLnlCfgCfm);
   SM_SET_ZERO(&nlPst, sizeof(Pst));
   switch(smCb.smState)
   {
      case WR_SM_STATE_INIT :
      {
         if(LCM_PRIM_OK == cfm->cfm.status)
         {
            switch(cfm->hdr.transId)
            {
               case LNL_TRANSID_GEN:
               {
                  wrNlCfg |= SM_NL_GEN_CFG;
                  smSendNlNluUSapCfg(WR_NL_SPID);
               }
               break;
               case LNL_TRANSID_NLU:
               {
                  wrNlCfg |= SM_NL_NLU_SAP_CFG;
                  smSendNlCellCfg();
               }
               break;
               case LNL_TRANSID_CELL:
               {
                  wrNlCfg |= SM_NL_CELL_CFG;
                  smSendNlNghCellCfg();
               }
               break;
               case LNL_TRANSID_NGH_CELL :
               {
                  wrNlCfg |= SM_NL_NGH_CELL_CFG;
               }
               break;
               case LNL_TRANSID_CELL_PCI:
               {
               }
               break;
               default:
               {
                  RLOG1(L_ERROR,"SON Cfg Cfm received with invalid element:[%d]"
                  ,cfm->hdr.elmId.elmnt);
               }
            }
         }
         else
         {
            RLOG2(L_ERROR,"Configuration Cfm with NOK Received reason:[%d]"
             "for element:[%d]", cfm->cfm.reason, cfm->hdr.elmId.elmnt);
         }
         if (wrNlCfg == SM_NL_CONFIGURED)
         {  
            RLOG0(L_ALWAYS, "SON configuration done");
            smCb.smState = WR_SM_STATE_SON_CFG_DONE;
            WR_FILL_PST(nlPst, SM_NL_PROC, ENTSM, ENTWR, EVTWRTUCLCFG, WR_POOL, SM_SELECTOR_LC);
            if(wrSmSelfPstMsg(&nlPst) != ROK)
            {
               RLOG0(L_ERROR,"wrSmSelfPstMsg:SON CONFIGURATION SUCCESSFULLY"
               "PERFORMED. Failure at Self Pst");
            }
            else
            {
               RLOG0(L_DEBUG,"SON CONFIGURATION SUCCESSFULLY PERFORMED.");
            }
         }
      }
      break;
      case WR_SM_STATE_CELL_UP :
#ifdef WR_RSYS_OAM
      case WR_SM_STATE_OAM_DYN_START :
#endif
      {
         if(LCM_PRIM_OK != cfm->cfm.status)
         {
            RLOG2(L_ERROR,"Configuration Cfm with NOK Received in Cell Up state reason:[%d]"
             "for element:[%d]", cfm->cfm.reason, cfm->hdr.elmId.elmnt);
         }
      }
      break;
      default :
      {
         RLOG0(L_ERROR,"Configuration Cfm in Invalis SM state");
      }
   }
   /*free the Interface message structure*/
   SPutSBuf(pst->region, pst->pool, (Data*)cfm, sizeof(NlMngmt));
   RETVALUE(ROK);
} /* end of SmMiLnlCfgCfm */

/*
 *
 *       Fun:    smSendCellUpIndToSon - cell Up indication NL
 *
 *       Desc:   This function is used to indicate SON module when cell is UP.
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_son.c
 *
 */

#ifdef ANSI
PUBLIC Void smSendCellUpIndToSon
(
Void
)
#else
PUBLIC Void smSendCellUpIndToSon(Void)
#endif /* ANSI */
{
   NlMngmt       *nlMngmt = NULLP;
   NlUsta        *usta = NULLP;
   Pst           pst;

   TRC2(smSendCellUpIndToSon);

   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, smCb.init.pool, (Data**)&nlMngmt, sizeof(NlMngmt));
   if(NULLP == nlMngmt)
   {
     RLOG0(L_FATAL, "Memory allocation failed.");
     RETVOID;
   }

   SM_SET_ZERO(nlMngmt, sizeof(NlMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   usta = &(nlMngmt->u.usta);
   usta->staType = LNL_STAT_ENB;
   usta->u.stEnbStaInfo.enStaType = LNL_ENBSTATYPE_CELLUP;
   usta->u.stEnbStaInfo.u.cellId = wrSmDfltCellId;

   /* Fill Header */
   nlMngmt->hdr.msgType             = TCFG;
   nlMngmt->hdr.msgLen              = 0;
   nlMngmt->hdr.entId.ent           = ENTNL;
   nlMngmt->hdr.entId.inst          = SM_INST_ZERO;
   nlMngmt->hdr.elmId.elmnt         = STNLGEN;
   nlMngmt->hdr.seqNmb              = 0;
   nlMngmt->hdr.version             = 0;
   nlMngmt->hdr.transId             = LNL_TRANSID_GEN;

   nlMngmt->hdr.response.prior      = PRIOR0;
   nlMngmt->hdr.response.route      = RTESPEC;
   nlMngmt->hdr.response.mem.region = nlMemMngr.nlRegion;
   nlMngmt->hdr.response.mem.pool   = nlMemMngr.nlPool;
   nlMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.event     = EVTLNLSTAIND;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNL;
   pst.dstProcId = SM_NL_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = smCb.init.pool;
 
#ifdef DEBUGP
   WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, "NL Cell Up Indication sent\n"));
#endif
   /* Send the request to NL */
   SmMiLnlEnbStatInd(&pst, nlMngmt);

   RETVOID;
}

/*
 *
 *       Fun:    wrSmNlCfg - configure NL 
 *
 *       Desc:   This function is used to configure NL interface ie. SM
 *               and SON.This is used to configure general configuration as 
 *               well as NLU SAP configuration
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:    wr_smm_son.c
 *
 */

#ifdef ANSI
PUBLIC Void wrSmNlCfg
(
Void
)
#else
PUBLIC Void wrSmNlCfg(Void)
#endif /* ANSI */
{
   smSendNlGenCfg();
   RETVOID;
} /* end of wrSmNlCfg */

/*
*
*       Fun:   smSendNlGenCfg
*
*       Desc:  Invoked to build NL GEN CFG.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_eson.c
*
*/
#ifdef ANSI
PRIVATE S16 smSendNlGenCfg
(
Void
)
#else
PRIVATE S16 smSendNlGenCfg(Void)
#endif
{
   NlMngmt       *nlMngmt = NULLP;
   NlGenCfg      *genCfg = NULLP;
   Pst           pst;

   TRC2(smSendNlGenCfg);

   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, smCb.init.pool, (Data**)&nlMngmt, sizeof(NlMngmt));
   if(NULLP == nlMngmt)
   {
      RLOG0(L_FATAL, "NL Gen Cfg allocation failure");
      RETVALUE(RFAILED);
   }

   SM_SET_ZERO(nlMngmt, sizeof(NlMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   genCfg   = &(nlMngmt->u.cfg.u.genCfg);
   genCfg->maxNluSaps = 1;
   smBuildNlGenCfg(genCfg);
   genCfg->lmPst.dstProcId = SM_SM_PROC;
   genCfg->lmPst.srcProcId = SM_NL_PROC;
   genCfg->lmPst.dstEnt    = ENTSM;
   genCfg->lmPst.dstInst   = 0;
   genCfg->lmPst.srcEnt    = ENTNL;
   genCfg->lmPst.srcInst   = 0;
   genCfg->lmPst.prior     = PRIOR0;
   genCfg->lmPst.route     = RTESPEC;
   genCfg->lmPst.region    = nlMemMngr.nlRegion;
   genCfg->lmPst.pool      = nlMemMngr.nlPool;
   genCfg->lmPst.selector  = SM_SELECTOR_LWLC;

   genCfg->mem.region      = nlMemMngr.nlRegion;
   genCfg->mem.pool        = nlMemMngr.nlPool;
   genCfg->tmrRes          = 10; /*Timer resolution*/ 


   /* Fill Header */
   nlMngmt->hdr.msgType             = TCFG;
   nlMngmt->hdr.msgLen              = 0;
   nlMngmt->hdr.entId.ent           = ENTNL;
   nlMngmt->hdr.entId.inst          = SM_INST_ZERO;
   nlMngmt->hdr.elmId.elmnt         = STNLGEN;
   nlMngmt->hdr.seqNmb              = 0;
   nlMngmt->hdr.version             = 0;
   nlMngmt->hdr.transId             = LNL_TRANSID_GEN;

   nlMngmt->hdr.response.prior      = PRIOR0;
   nlMngmt->hdr.response.route      = RTESPEC;
   nlMngmt->hdr.response.mem.region = nlMemMngr.nlRegion;
   nlMngmt->hdr.response.mem.pool   = nlMemMngr.nlPool;
   nlMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.event     = EVTLNLCFGREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNL;
   pst.dstProcId = SM_NL_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = smCb.init.pool;

   RLOG0(L_DEBUG, "NL Gen Cfg Req sent\n");
   /* Send the request to NL */
   SmMiLnlCfgReq(&pst, nlMngmt);

   RETVALUE(ROK);
}

/*
*
*       Fun:   smSendNlNluUSapCfg
*
*       Desc:  Invoked to send NL NLU SAP Config (NLU).
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_eson.c
*
*/
#ifdef ANSI
PRIVATE  S16 smSendNlNluUSapCfg
(
SpId spId
)
#else
PRIVATE  S16 smSendNlNluUSapCfg (spId)
SpId spId;
#endif
{
   
   NlMngmt       *nlMngmt = NULLP;
   NlUSapCfg     *usap=NULLP;
   Pst           pst;

   TRC2(smSendNlNluUSapCfg)

   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, smCb.init.pool, (Data**)&nlMngmt, sizeof(NlMngmt));
   if(NULLP == nlMngmt)
   {
     RLOG0(L_FATAL, "Send NLU SAP Cfg Memory allocation failure");
     RETVALUE(RFAILED);
   }
   SM_SET_ZERO(nlMngmt, sizeof(NlMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   usap   = &(nlMngmt->u.cfg.u.nluSap);

   usap->spId        = spId;
   usap->procId      = SM_NL_PROC;
   usap->prior       = PRIOR0;
   usap->route       = RTESPEC;
   usap->selector    = NLU_SEL_LWLC;
   usap->ent         = ENTWR;
   usap->inst        = SM_INST_ZERO;
   usap->mem.region  = nlMemMngr.nlRegion;
   usap->mem.pool    = nlMemMngr.nlPool;

   /* Fill Header */
   nlMngmt->hdr.msgType             = TCFG;
   nlMngmt->hdr.msgLen              = 0;
   nlMngmt->hdr.entId.ent           = ENTNL;
   nlMngmt->hdr.entId.inst          = SM_INST_ZERO;
   nlMngmt->hdr.elmId.elmnt         = STNLUSAP;
   nlMngmt->hdr.seqNmb              = 0;
   nlMngmt->hdr.version             = 0;
   nlMngmt->hdr.transId             = LNL_TRANSID_NLU;
  
   nlMngmt->hdr.response.prior      = PRIOR0;
   nlMngmt->hdr.response.route      = RTESPEC;
   nlMngmt->hdr.response.mem.region = nlMemMngr.nlRegion;
   nlMngmt->hdr.response.mem.pool   = nlMemMngr.nlPool;
   nlMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;
   
   /* Fill Pst */
   pst.event     = EVTLNLCFGREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNL;
   pst.dstProcId = SM_NL_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = smCb.init.pool;

#ifdef DEBUGP
  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, "ESON nlu Sap Cfg Req sent\n"));
#endif
   /* Send the request to the ESON */
   SmMiLnlCfgReq(&pst, nlMngmt);

   RETVALUE(ROK);
} /* end of smSendNlNluSapCfg */

/*
*
*
*       Fun:   smSendNlCellReCfg
*
*       Desc:  Invoked to send NL Cell Re configurations.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/
#ifdef ANSI
PUBLIC  S16 smSendNlCellReCfg
(
Void
)
#else
PUBLIC  S16 smSendNlCellReCfg (Void)
#endif
{

   NlMngmt             *nlMngmt = NULLP;
   LnlCellReCfg        *cellReCfg = NULLP;
   Pst                 pst;

   TRC2(smSendNlCellReCfg)
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, smCb.init.pool, (Data**)&nlMngmt, sizeof(NlMngmt));
   if(NULLP == nlMngmt)
   {
     RLOG0(L_FATAL, "Send Cell Re-Cfg Memory allocation failure");
     RETVALUE(RFAILED);
   }
   SM_SET_ZERO(nlMngmt, sizeof(NlMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   nlMngmt->u.protoCfg.bCfgType = LNL_CELL_RECFG;
   cellReCfg = &(nlMngmt->u.protoCfg.u.cellReCfg);

   if(ROK != smBuildNlCellReCfg(cellReCfg))
   {
      RLOG0(L_FATAL," Building cell configuration failed \n");
      SPutSBuf(smCb.init.region, smCb.init.pool,
                                  (Data*)nlMngmt, sizeof(NlMngmt));
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   nlMngmt->hdr.msgType             = TCFG;
   nlMngmt->hdr.msgLen              = 0;
   nlMngmt->hdr.entId.ent           = ENTNL;
   nlMngmt->hdr.entId.inst          = SM_INST_ZERO;
   nlMngmt->hdr.elmId.elmnt         = STNLPROTOCFG;
   nlMngmt->hdr.seqNmb              = 0;
   nlMngmt->hdr.version             = 0;
   nlMngmt->hdr.transId             = LNL_TRANSID_CELL_PCI;

   nlMngmt->hdr.response.prior      = PRIOR0;
   nlMngmt->hdr.response.route      = RTESPEC;
   nlMngmt->hdr.response.mem.region = nlMemMngr.nlRegion;
   nlMngmt->hdr.response.mem.pool   = nlMemMngr.nlPool;
   nlMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.event     = EVTLNLCFGREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNL;
   pst.dstProcId = SM_NL_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = smCb.init.pool;
   
#ifdef DEBUGP
  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, "SON cell ReCfg Req sent\n"));
#endif
   /* Send the request to the SON */
   SmMiLnlCfgReq(&pst, nlMngmt);
   RETVALUE(ROK);
} /* end of smSendNlCellReCfg */

/*
*
*
*       Fun:   smSendNlCellCfg
*
*       Desc:  Invoked to send NL Cell configurations.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/
#ifdef ANSI
PRIVATE  S16 smSendNlCellCfg
(
Void
)
#else
PRIVATE  S16 smSendNlCellCfg (Void)
#endif
{

   NlMngmt             *nlMngmt = NULLP;
   LnlCellCfg          *cellCfg = NULLP;
   Pst                 pst;

   TRC2(smSendNlCellCfg)
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, smCb.init.pool, (Data**)&nlMngmt, sizeof(NlMngmt));
   if(NULLP == nlMngmt)
   {
     RLOG0(L_FATAL, "Send SON server initialisation allocation failure");
     RETVALUE(RFAILED);
   }
   SM_SET_ZERO(nlMngmt, sizeof(NlMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   nlMngmt->u.protoCfg.bCfgType = LNL_CELL_CFG;
   cellCfg = &(nlMngmt->u.protoCfg.u.cellCfg);
   if(ROK != smBuildNlCellCfg(cellCfg))
   {
      RLOG0(L_FATAL," Building cell configuration failed \n");
      SPutSBuf(smCb.init.region, smCb.init.pool,
                                  (Data*)nlMngmt, sizeof(NlMngmt));
      RETVALUE(RFAILED);
   }
   /* Fill Header */
   nlMngmt->hdr.msgType             = TCFG;
   nlMngmt->hdr.msgLen              = 0;
   nlMngmt->hdr.entId.ent           = ENTNL;
   nlMngmt->hdr.entId.inst          = SM_INST_ZERO;
   nlMngmt->hdr.elmId.elmnt         = STNLPROTOCFG;
   nlMngmt->hdr.seqNmb              = 0;
   nlMngmt->hdr.version             = 0;
   nlMngmt->hdr.transId             = LNL_TRANSID_CELL;

   nlMngmt->hdr.response.prior      = PRIOR0;
   nlMngmt->hdr.response.route      = RTESPEC;
   nlMngmt->hdr.response.mem.region = nlMemMngr.nlRegion;
   nlMngmt->hdr.response.mem.pool   = nlMemMngr.nlPool;
   nlMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.event     = EVTLNLCFGREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNL;
   pst.dstProcId = SM_NL_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = smCb.init.pool;
   
#ifdef DEBUGP
  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, "SON cell Cfg Req sent\n"));
#endif
   /* Send the request to the SON */
   SmMiLnlCfgReq(&pst, nlMngmt);
   RETVALUE(ROK);
} /* end of smSendNlCellCfg */

/*
*
*
*       Fun:   smSendNlNghCellCfg
*
*       Desc:  Invoked to build NL Neighbor Cell configurations.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/
#ifdef ANSI
PRIVATE  S16 smSendNlNghCellCfg
(
Void
)
#else
PRIVATE  S16 smSendNlNghCellCfg (Void)
#endif
{

   NlMngmt             *nlMngmt = NULLP;
   LnlNghCellCfg       *nghCellCfg = NULLP;
   Pst                 pst;

   TRC2(smSendNlNghCellCfg)
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, smCb.init.pool, (Data**)&nlMngmt, sizeof(NlMngmt));
   if(NULLP == nlMngmt)
   {
     RLOG0(L_FATAL, "Send SON server initialisation allocation failure");
     RETVALUE(RFAILED);
   }
   SM_SET_ZERO(nlMngmt, sizeof(NlMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));
   nlMngmt->u.protoCfg.bCfgType = LNL_NGH_CELL_CFG;
   nghCellCfg = &(nlMngmt->u.protoCfg.u.nghCellCfg);
   smBuildNlNghCellCfg(nghCellCfg);

   /* Fill Header */
   nlMngmt->hdr.msgType             = TCFG;
   nlMngmt->hdr.msgLen              = 0;
   nlMngmt->hdr.entId.ent           = ENTNL;
   nlMngmt->hdr.entId.inst          = SM_INST_ZERO;
   nlMngmt->hdr.elmId.elmnt         = STNLPROTOCFG;
   nlMngmt->hdr.seqNmb              = 0;
   nlMngmt->hdr.version             = 0;
   nlMngmt->hdr.transId             = LNL_TRANSID_NGH_CELL;

   nlMngmt->hdr.response.prior      = PRIOR0;
   nlMngmt->hdr.response.route      = RTESPEC;
   nlMngmt->hdr.response.mem.region = nlMemMngr.nlRegion;
   nlMngmt->hdr.response.mem.pool   = nlMemMngr.nlPool;
   nlMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.event     = EVTLNLCFGREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNL;
   pst.dstProcId = SM_NL_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = smCb.init.pool;
   
#ifdef DEBUGP
  WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, "SON cell Cfg Req sent\n"));
#endif
   /* Send the request to the SON */
   SmMiLnlCfgReq(&pst, nlMngmt);
   RETVALUE(ROK);
} /* end of smSendNlNghCellCfg */


/*
*
*       Fun:   smBuildNlShutDownCntrl
*
*       Desc:  Invoked shutdown to SON module
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  wr_smm_son.c
*
*/
#ifdef ANSI
PUBLIC S16 smBuildNlShutDownCntrl
(
Void
)
#else
PUBLIC S16 smBuildNlShutDownCntrl(Void)
#endif
{
   NlCntrl  *cntrl = NULLP;
   NlMngmt  *nlMngmt = NULLP; 
   Pst      pst;
  
   TRC2(smBuildNlShutDownCntrl)
 
   /*Allocate memory for the message. This is done since we are using LWLC*/
   SGetSBuf(smCb.init.region, smCb.init.pool, (Data**)&nlMngmt, sizeof(NlMngmt));
   if(NULLP == nlMngmt)
   {
     RLOG0(L_FATAL, "SON shutdown memory allocation failure");
     RETVALUE(RFAILED);
   }
   SM_SET_ZERO(nlMngmt, sizeof(NlMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

   cntrl = &(nlMngmt->u.cntrl);

   cntrl->action        =  ASHUTDOWN;
   wrNlCfg = 0;
/* Fill Header */
   nlMngmt->hdr.msgType             = TCNTRL;
   nlMngmt->hdr.msgLen              = 0;
   nlMngmt->hdr.entId.ent           = ENTNL;
   nlMngmt->hdr.entId.inst          = SM_INST_ZERO;
   nlMngmt->hdr.elmId.elmnt         = STNLGEN;
   nlMngmt->hdr.seqNmb              = 0;
   nlMngmt->hdr.version             = 0;
   nlMngmt->hdr.transId             = 0;

   nlMngmt->hdr.response.prior      = PRIOR0;
   nlMngmt->hdr.response.route      = RTESPEC;
   nlMngmt->hdr.response.mem.region = nlMemMngr.nlRegion;
   nlMngmt->hdr.response.mem.pool   = nlMemMngr.nlPool;
   nlMngmt->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.event     = EVTLNLCNTRLREQ;
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTNL;
   pst.dstProcId = SM_NL_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.dstInst   = SM_INST_ZERO;
   pst.region    = smCb.init.region;
   pst.pool      = smCb.init.pool;

   RLOG0(L_DEBUG,"SON shut down Cntrl Req sent");

   /* Send the request to the SON */
   SmMiLnlCntrlReq(&pst, nlMngmt);
   RETVALUE(ROK);
} 

PRIVATE CONSTANT LnlCfgReq SmMiLnlCfgReqMt[MAXNLMI] =
{
#ifdef LCSMSONILNL
   cmPkLnlCfgReq,
#else
   PtMiLnlCfgReq,
#endif
#ifdef TCSMSONILNL
   NlMiLnlCfgReq,
#else
   PtMiLnlCfgReq,
#endif
#ifdef LWLCSMSONILNL
   cmPkLnlCfgReq,
#else
   PtMiLnlCfgReq,
#endif
};

PRIVATE CONSTANT LnlCntrlReq SmMiLnlCntrlReqMt[MAXNLMI] =
{
#ifdef LCSMSONILNL
   cmPkLnlCntrlReq,
#else
   PtMiLnlCntrlReq,
#endif
#ifdef TCSMSONILNL
   NlMiLnlCntrlReq,
#else
   PtMiLnlCntrlReq,
#endif
#ifdef LWLCSMSONILNL
   cmPkLnlCntrlReq,
#else
   PtMiLnlCntrlReq,
#endif
};

PRIVATE CONSTANT LnlStatInd SmMiLnlEnbStatIndMt[MAXNLMI] =
{
#ifdef LCSMSONILNL
   cmPkLnlStatInd,
#else
   PtMiLnlStatInd,
#endif
#ifdef TCSMSONILNL
   NlMiLnlStatInd,
#else
   PtMiLnlStatInd,
#endif
#ifdef LWLCSMSONILNL
   cmPkLnlStatInd,
#else
   PtMiLnlStatInd,
#endif
};

/*
 *
 *      Fun:   Control request
 *
 *      Desc:  This function is used to call appropriate NL interface
 *             API based on selector for sending all the possible 
 *             control messages from SM to SON
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_smm_son.c
 *
 */
#ifdef ANSI
PUBLIC S16 SmMiLnlCntrlReq
(
Pst       *pst,
NlMngmt   *cntrl
)
#else
PUBLIC S16 SmMiLnlCntrlReq(pst, cntrl)
Pst       *pst;
NlMngmt   *cntrl;
#endif
{
   S16 ret;

   TRC3(SmMiLnlCntrlReq);

   ret = (*SmMiLnlCntrlReqMt[pst->selector])(pst, cntrl);
   RETVALUE(ret);
}

/*
 *
 *      Fun:   Configuration request
 *
 *      Desc:  This function is used to call appropriate NL interface
 *             API based on selector for sending all the possible 
 *             configuration messages from SM to SON
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_smm_son.c
 *
 */
#ifdef ANSI
PUBLIC S16 SmMiLnlCfgReq
(
Pst * pst,
NlMngmt * cfg
)
#else
PUBLIC S16 SmMiLnlCfgReq(pst, cfg)
Pst * pst;
NlMngmt * cfg;
#endif
{
   S16 ret;

   TRC3(SmMiLnlCfgReq);

   ret = (*SmMiLnlCfgReqMt[pst->selector])(pst, cfg);
   RETVALUE(ret);
}

/*
 *
 *      Fun:   EnodeB status indication for sendign cell up indication to SON   
 *
 *      Desc:  This function is used to call appropriate NL interface
 *             API based on selector for sending eNodeB cell up 
 *             indication messages from SM to SON
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  wr_smm_son.c
 *
 */
#ifdef ANSI
PUBLIC S16 SmMiLnlEnbStatInd
(
Pst        *pst,
NlMngmt    *cellUpInd
)
#else
PUBLIC S16 SmMiLnlEnbStatInd(pst, cellUpInd)
Pst        *pst;
NlMngmt    *cellUpInd;
#endif
{
   S16 ret;

   TRC3(SmMiLnlEnbStatInd);
   ret = (*SmMiLnlEnbStatIndMt[pst->selector])(pst, cellUpInd);
   RETVALUE(ret);
}

/********************************************************************30**

           End of file:     fc_smm_son.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/3 - Tue Jan  6 18:36:16 2015

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
SID$        ---      
*********************************************************************91*/

