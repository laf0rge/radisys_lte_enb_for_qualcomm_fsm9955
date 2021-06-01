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
               

     File:     wr_tmr.c

     Sid:      fc_smm_tmr.c@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:52:34 2014

     Prg:      Sriky 

*********************************************************************21*/
/* Header include files (.h) */
#include "ssi.h"
#include "wr_cmn.h"
#include "wr_smm_init.h"


#if defined (WR_RSYS_OAM) || (WR_DBG_CIRLOG) 
 
static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_FILE_ID=17;
static int RLOG_MODULE_ID=2;

EXTERN Void FapAlarmInd(U32 alarmId, U8 severity, U8 causeType, U8 causeVal);

PUBLIC Void smStaticCfgFailAlarm(Void);
PUBLIC Void smDynCfgFailAlarm(U32 event);
PRIVATE Void smProcTmr (Ptr cb, S16 tmrEvnt);
PUBLIC S16 smRegInitTmr ARGS((Void));
PUBLIC S16 smStartTmr ARGS((PTR cb, S16 tmrEvnt,U32 delay));
PUBLIC Void smStopTmr ARGS((PTR cb,S16 event));
PUBLIC Bool smIsTmrRunning ARGS((CmTimer *tmr,S16 event));

EXTERN U32 smDynTmrEvent;


StaticAlarms StaticCfgFailAlarms[20] =
{
   {L_FATAL, "SOCKET_CFG_FAIL"},
   {L_FATAL, "SCTP_CFG_FAIL"},
   {L_FATAL, "S1AP_CFG_FAIL,"},
   {L_FATAL, "EGTP_CFG_FAIL"},
   {L_FATAL, "RRM_CFG_FAIL"},
   {L_FATAL, "X2AP_CFG_FAIL"},
   {L_FATAL, "RRC_CFG_FAIL"},
   {L_FATAL, "PDCPUL_CFG_FAIL"},
   {L_FATAL,  "\0"      },
   {L_FATAL,  "\0"       },
   {L_FATAL, "PDCPDL_CFG_FAIL"},
   {L_FATAL, "RLCUL_CFG_FAIL"},
   {L_FATAL, "RLCDL_CFG_FAIL"},
   {L_FATAL, "MAC_CFG_FAIL"},
   {L_FATAL, "CL_CFG_FAIL"},
   {L_FATAL, "CELLUP_CFG_FAIL"},
   {L_FATAL, "APP_CFG_FAIL"}
};


DynamicAlarms DynamicCfgFailAlarms[50] =
{
   {L_FATAL, "ADMIN_STATE_CHANGE_FAIL"},
   {L_ERROR, "GENERIC_PRAMS_CFG_FAIL"},
   {L_ERROR, "MME_IP_PRAMS_CFG_FAIL"},
   {L_ERROR, "CELL_CONFIG_PRAMS_FAIL"},
   {L_ERROR, "ENBIP_CFG_FAIL"},
   {L_ERROR, "MIB_CFG_FAIL"},
   {L_ERROR, "PRACH_CFG_FAIL"},
   {L_ERROR, "RACH_CFG_FAIL"},
   {L_ERROR, "PDSCH_CFG_FAIL"},
   {L_ERROR, "SRS_CFG_FAIL"},
   {L_ERROR, "PUSCH_CFG_FAIL"},
   {L_ERROR, "ULPOWER_CFG_FAIL"},
   {L_ERROR, "ULFREQ_CFG_FAIL"},
   {L_ERROR, "PUCCH_CFG_FAIL"},
   {L_ERROR, "UETMRCONS_CFG_FAIL"},
   {L_ERROR, "SIB1_CFG_FAIL"},
   {L_ERROR, "SIB2_CFG_FAIL"},
   {L_ERROR, "SIB3_CFG_FAIL"},
   {L_ERROR, "SIB4_CFG_FAIL"},
   {L_ERROR, "SIB6_CFG_FAIL"},
   {L_ERROR, "SIB9_CFG_FAIL"},
   {L_ERROR, "ANR_CFG_FAIL"},
   {L_ERROR, "MEAS_CFG_FAIL"},
   {L_ERROR, "EAID_CFG_FAIL"},
   {L_ERROR, "SIBSCHED_CFG_FAIL"},
   {L_ERROR, "RABPOLICY_CFG_FAIL"},
   {L_ERROR, "TIMER_CFG_FAIL"},
   {L_ERROR, "RRM_CELL_CFG_FAIL"},
   {L_ERROR, "ULSCHD_CFG_FAIL"},
   {L_ERROR, "DLSCHD_CFG_FAIL"},
   {L_ERROR, "NEIGH_CFG_FAIL"},
   {L_ERROR, "NEIGH_CELL_CFG_FAIL"},
   {L_ERROR, "NEIGH_FREQ_CFG_FAIL"},
   {L_ERROR, "NEIGH_BAND_CLASS_CFG_FAIL"},
   {L_ERROR, "ENB_PROTO_CFG_FAIL"},
   {L_ERROR, "UTRANEIFREQ_CFG_FAIL"},
   {L_ERROR, "EUTRANEIFREQ_CFG_FAIL"},
   {L_ERROR, "UTRANEICELL_CFG_FAIL"},
   {L_ERROR, "EUTRANEICELL_CFG_FAIL"},
   {L_ERROR, "UTRANEILIST_CFG_FAIL"},
   {L_ERROR, "EUTRANEILIST_CFG_FAIL"},
   {L_ERROR, "RABSRB1_CFG_FAIL"},
   {L_ERROR, "RABSRB2_CFG_FAIL"},
   {L_ERROR, "EVNT_NEIGH_ENB_CFG_FAIL"},
   {L_ERROR, "SMCELL_CFG_FAIL"},
   {L_ERROR, "PROCTMR_CFG_FAIL"},
   {L_ERROR, "CFG_FAIL_FROM_SON"}
};


/** @brief Invoked by system services to activate a task with
 *              a timer tick.
 *
 * @details
 *
 *     Function: smActvTmr
 *
 *         Processing steps:
 *
 * @param[in] Void 
 * @return S16
 *        -# Success : ROK
 */

PUBLIC S16 smActvTmr
(
 Ent ent,
 Inst  inst
)
{
   cmPrcTmr(&smCb.tqCp, smCb.tq, (PFV) smProcTmr);
   RETVALUE(ROK);
} /* end of smActvTmr */

/** @brief This function registers and initializes the timer
 *         queue structures in LTE eNodeB during general config.
 *
 * @details
 *
 *     Function: smRegInitTmr
 *
 *         Processing steps:
 *
 * @param[in] Void 
 * @return S16
 *        -# Success : ROK
 */
PUBLIC S16 smRegInitTmr
(
Void
)
{
   U16                       idx;
   S16                       ret;

   ret = SRegTmrMt((Ent)ENTSM, (Inst)0, SM_TMR_RES, smActvTmr);
   if (ret != ROK)
   {
      RLOG0(L_FATAL,"smRegInitTmr: SRegTmr Failed ");
      RETVALUE(RFAILED);
   }

   /* initialize timing queues */
   smCb.tqCp.tmrLen      = SM_NUM_TQENTRY;
   for (idx = 0; idx < SM_NUM_TQENTRY; idx++)
   {
      smCb.tq[idx].first    = NULLP;
      smCb.tq[idx].tail     = NULLP;
   }

   RETVALUE(ROK);

} /* smRegInitTmr */


/** @brief This function Raise an alarm on static configuration failure to OAM
 *
 * @details
 *
 *     Function: smStaticCfgFailAlarm 
 *
 *         Processing steps:
 *               Check the which configuration is failing.
 *               Raise a Alarm on OAM with Reason, event type and  
 *
 * @param[in] Void 
 * @return Void
 */
Void smStaticCfgFailAlarm
(
 Void
 )
{
   LwrUstaDgn *dgn;
   WR_ALLOC(&dgn, sizeof(LwrUstaDgn));

   switch(smCb.smState)
   {
      case WR_SM_STATE_INIT:
         {
            dgn->u.alarm.alarmId   =  SOCKET_CFG_FAIL;
         }
         break;
#ifdef LTE_QCOM
    case WR_SM_STATE_SON_CFG_DONE:
         {
            /* Reusing alarm ID defined for WR_SM_STATE_CL_CFG_DONE to avoid 
             * changes in OAM-SM and Tr069 */
            dgn->u.alarm.alarmId   =  CELLUP_CFG_FAIL;
         }
         break;
#endif
      case WR_SM_STATE_TUCL_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  SCTP_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_SCTP_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  S1AP_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_S1AP_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  EGTP_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_EGTP_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  X2AP_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_X2AP_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  RRM_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_RRM_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  APP_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_APP_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  RRC_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_RRC_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  PDCPUL_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_PDCP_UL_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  PDCPDL_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_PDCP_DL_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  RLCUL_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_RLC_UL_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  RLCDL_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_RLC_DL_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  MAC_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_MAC_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  CL_CFG_FAIL;
         }
         break;
      case WR_SM_STATE_CL_CFG_DONE:
         {
            dgn->u.alarm.alarmId   =  CELLUP_CFG_FAIL;
         }
         break;
      default:
         {
            RLOG0(L_ERROR, "Other Confiuration Failure");
         }
   }

   dgn->u.alarm.severity  = smCb.cfgCb.alarm.severity;
   dgn->u.alarm.causeType = smCb.cfgCb.alarm.causeType;
   dgn->u.alarm.causeVal  = smCb.cfgCb.alarm.causeVal;

   RLOG_STR(StaticCfgFailAlarms[smCb.smState].logLevel,"[ALARM] Sending %s to OAM",(PSTR)StaticCfgFailAlarms[smCb.smState].alarmType);
   FapAlarmInd(dgn->u.alarm.alarmId, dgn->u.alarm.severity, dgn->u.alarm.causeType, dgn->u.alarm.causeVal);

   WR_FREE(dgn, sizeof(LwrUstaDgn));
}
/* end of smStaticCfgFailAlarm */

/** @brief This function Raise an alarm on dynamic configuration failure to OAM
 *
 * @details
 *
 *     Function: smDynCfgFailAlarm 
 *
 *         Processing steps:
 *               Check the which configuration is failing.
 *               Raise a Alarm on OAM with Reason, event type and  
 *
 * @param[in] Void 
 * @return Void
 */
Void smDynCfgFailAlarm
(
 U32 event
 )
{
   LwrUstaDgn *dgn;
   WR_ALLOC(&dgn, sizeof(LwrUstaDgn));

   switch(event)
   {
      case LWR_DYN_EVNT_ADMIN_STATE_CHANGE:
         {
            dgn->u.alarm.alarmId   =  ADMIN_STATE_CHANGE_FAIL;
         }
         break;
      case LWR_DYN_EVNT_GENERIC_PRAMS:
         {
            dgn->u.alarm.alarmId   =  GENERIC_PRAMS_CFG_FAIL;
         }
         break;
      case LWR_DYN_EVNT_MME_IP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  MME_IP_PRAMS_CFG_FAIL;
         }
         break;
      case LWR_DYN_EVNT_CELL_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  CELL_CONFIG_PRAMS_FAIL;
         }
         break;
      case LWR_DYN_EVNT_ENBIP_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  ENBIP_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_MIB_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  MIB_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_PRACH_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  PRACH_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_RACH_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  RACH_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_PDSCH_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  PDSCH_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_SRS_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  SRS_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_PUSCH_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  PUSCH_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_ULPOWER_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  ULPOWER_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_ULFREQ_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  ULFREQ_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_PUCCH_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  PUCCH_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_UETMRCONS_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  UETMRCONS_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_SIB1_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  SIB1_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_SIB2_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  SIB2_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_SIB3_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  SIB3_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_SIB4_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  SIB4_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_SIB6_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  SIB6_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_SIB9_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  SIB9_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_ANR_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  ANR_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_MEAS_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  MEAS_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_EAID_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  EAID_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_SIBSCHED_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  SIBSCHED_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_RABPOLICY_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  RABPOLICY_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_TIMER_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  TIMER_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_RRM_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  RRM_CELL_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_ULSCHD_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  ULSCHD_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_CELL_DLSCHD_CONFIG_GROUP_PRAMS:
         {
            dgn->u.alarm.alarmId   =  DLSCHD_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_NEIGH_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  NEIGH_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_NEIGH_CELL_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  NEIGH_CELL_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_NEIGH_FREQ_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  NEIGH_FREQ_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_NEIGH_BAND_CLASS_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  NEIGH_BAND_CLASS_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_ENB_PROTO_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  ENB_PROTO_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_NEIGH_UTRA_FREQ_CFG:
         {
            dgn->u.alarm.alarmId   =  UTRANEIFREQ_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_NEIGH_EUTRA_FREQ_CFG:
         {
            dgn->u.alarm.alarmId   =  EUTRANEIFREQ_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_NEIGH_UTRA_CELL_CFG:
         {
            dgn->u.alarm.alarmId   =  UTRANEICELL_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_NEIGH_EUTRA_CELL_CFG:
         {
            dgn->u.alarm.alarmId   =  EUTRANEICELL_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_UTRANEILIST_CONFIG:
         {
            dgn->u.alarm.alarmId   =  UTRANEILIST_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_EUTRANEILIST_CONFIG:
         {
            dgn->u.alarm.alarmId   =  EUTRANEILIST_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_RABSRB1_CONFIG_PARAMS:
         {
            dgn->u.alarm.alarmId   =  RABSRB1_CFG_FAIL;
         }
         break;
   case LWR_DYN_EVNT_RABSRB2_CONFIG_PARAMS:
         {
            dgn->u.alarm.alarmId   =  RABSRB2_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_NEIGH_ENB_CFG:
         {
            dgn->u.alarm.alarmId   =  EVNT_NEIGH_ENB_CFG_FAIL;
         }
         break;
      case  LWR_DYN_EVNT_SM_CELL_CONFIG_PRAMS:
         {
            dgn->u.alarm.alarmId   =  SMCELL_CFG_FAIL;
         }
         break;
      default:
         {
            RLOG0(L_ERROR, "Invalid Event Type");
         }

         dgn->u.alarm.severity  = smCb.cfgCb.alarm.severity;
         dgn->u.alarm.causeType = smCb.cfgCb.alarm.causeType;
         dgn->u.alarm.causeVal  = smCb.cfgCb.alarm.causeVal;
         RLOG_STR(DynamicCfgFailAlarms[smCb.smState].logLevel,"[ALARM] Sending %s to OAM",(PSTR)DynamicCfgFailAlarms[smCb.smState].alarmType);
         FapAlarmInd(dgn->u.alarm.alarmId, dgn->u.alarm.severity, dgn->u.alarm.causeType, dgn->u.alarm.causeVal);

         WR_FREE(dgn, sizeof(LwrUstaDgn));

   }
}/* end of smDynCfgFailAlarm */

/** @brief This function is used to start a given timer. 
 *
 * @details
 *
 *     Function: smStartTmr
 *
 *         Processing steps:
 *               This function based upon the timer event, derives the TimerCB 
 *               and adds it to the timer queue.
 *
 * @param[in] Cb : This holds approriate Control block for the timer event passed. 
 * @param[in] tmrEvent : One of the many possible timer types. 
 * @param[in] delay : Duration of the timer.
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 smStartTmr
(
 PTR                          cb,
 S16                          tmrEvnt,
 U32                          delay
 )
{
   U32                       wait;
   CmTmrArg                  arg;
   CmTimer                   *tmr;
   U8                        maxTmrs;
   U16                       idx;
   wait = 0;

   wait = SM_CALC_WAIT_TIME(delay);

   switch (tmrEvnt)
   { 
#ifdef WR_DBG_CIRLOG
      case SM_TMR_LOG_TMR:
         {
            tmr = &smCb.cfgCb.logTimer;
            maxTmrs = 1;   
         }
         break;
#endif
      case SM_TMR_STATIC_CFG_TMR:
      case SM_TMR_DYN_CFG_TMR:
         {
            tmr = &smCb.cfgCb.guardCfgTmr;
            maxTmrs = 1;   
         }
         break;
      default:
      {
         RETVALUE(RFAILED);
      }       
   }
   for(idx = 0; idx < maxTmrs; idx++)
   {
      if (tmr[idx].tmrEvnt == TMR_NONE)
      {
         break;
      }
   }
   if (idx >= maxTmrs)
   {
      RETVALUE(RFAILED);
   } 
   /* initialize argument for common timer function */
   if(wait != 0)
   {
      arg.tqCp   = &(smCb.tqCp);
      arg.tq     = smCb.tq;
      arg.cb     = cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;
      cmPlcCbTq(&arg);
   }

   RETVALUE(ROK); 
}
/** @brief This function is used to stop a previously running timer. 
 *
 * @details
 *
 *     Function: wrStopTmr
 *
 *         Processing steps:
 *               This function based upon the timer event reterives relevant timerCb and 
 *               tries to determine whether timer is running or not.In case timer 
 *               was running, it is stopped .
 *
 * @param[in] Cb : This holds approriate Control block for the timer event passed. 
 * @param[in] tmrEvent : One of the many possible timer types. 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC Void smStopTmr
(
 PTR                          cb,
 S16                          event
 )
{
   CmTmrArg                  arg;
   U8                        idx;
   Bool                      tmrRunning;
   CmTimer                   *timers = NULLP;
   U8                        max = 0;  
   idx = 0;

   tmrRunning = FALSE;
   switch(event)
   {
#ifdef WR_DBG_CIRLOG
      case SM_TMR_LOG_TMR:
         {
            timers = &smCb.cfgCb.logTimer;
            max    = 1;
            if (timers->tmrEvnt == event)
            {  
              tmrRunning = TRUE;
            } 
         }
         break;       
#endif
      case SM_TMR_STATIC_CFG_TMR:
      case SM_TMR_DYN_CFG_TMR:
         {
            timers = &smCb.cfgCb.guardCfgTmr;
            max    = 1;
            if (timers->tmrEvnt == event)
            {  
              tmrRunning = TRUE;
            } 
         }
         break;       
      default:
         break;
   }
   if(tmrRunning == FALSE)
   {
      RETVOID;
   } 
  /* initialize argument for common timer function */
   arg.tqCp    = &smCb.tqCp;
   arg.tq      = smCb.tq; 
   arg.timers  = timers;
   arg.cb      = cb;
   arg.evnt    = event;
   arg.wait    = 0;
   arg.max     = max;
   arg.tNum    = idx;
   cmRmvCbTq(&arg);
   RETVOID; 
}
/** @brief This function handles timer expiry. 
 *
 * @details
 *
 *     Function: smProcTmr
 *
 *         Processing steps:
 *               This function based upon the timer event invokes specific 
 *               routine to handle timer expiry. 
 *
 * @param[in] Cb : This holds approriate Control block for the timer event passed. 
 * @param[in] tmrEvent : One of the many possible timer types. 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE Void smProcTmr
(
Ptr                          cb,
S16                          event
)
{ 
   switch(event)
   {
#ifdef WR_DBG_CIRLOG
      case SM_TMR_LOG_TMR:
         {
            /* Dump the logs to the file */
            wrDbgWriteCircBufToFile();
            smStartTmr((PTR)&(smCb),SM_TMR_LOG_TMR, SM_TMR_LOG_TMR_VAL);
         }
         break;       
#endif
      case SM_TMR_STATIC_CFG_TMR:
         {
            smCb.cfgCb.alarm.severity  = SM_ALARM_SEVERITY_CRITICAL;
            smCb.cfgCb.alarm.causeType = SM_ALARM_CAUSE_TYPE_TMR_EXP;
            smCb.cfgCb.alarm.causeVal  = SM_ALARM_CAUSE_VAL_ABNORMAL;
            /* Raise an Alarm to OAM-SM */
            smStaticCfgFailAlarm();
         }
         break;       
      case SM_TMR_DYN_CFG_TMR:
         {
            smCb.cfgCb.alarm.severity  = SM_ALARM_SEVERITY_CRITICAL;
            smCb.cfgCb.alarm.causeType = SM_ALARM_CAUSE_TYPE_TMR_EXP;
            smCb.cfgCb.alarm.causeVal  = SM_ALARM_CAUSE_VAL_ABNORMAL;
            /* Raise an Alarm to OAM-SM */
            smDynCfgFailAlarm(smDynTmrEvent);
         }
         break;       
      default:
         break;
   } 
}
/** @brief This function Finds whether Timer is running or not
 *
 * @details
 *
 *     Function: smIsTmrRunning
 *
 *         Processing steps:
 *               Check Whetether Timer is running or not based on Event type
 *
 * @param[in] tmr : pointre to the Timer. 
 * @param[in] event : One of the many possible timer types. 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC Bool smIsTmrRunning
(
 CmTimer                      *tmr,
 S16                          event
 )
{
   if(tmr != NULLP)
   {   
      if(tmr->tmrEvnt == event)
      {
         RETVALUE(TRUE);
      }
      else if (tmr->tmrEvnt != TMR_NONE)
      {
         RLOG1(L_WARNING,"wrIsTmrRunning: Invalid timer is RUNNING[%d]", event);
      }
   }
   RETVALUE(FALSE);
} 

#endif /*WR_DBG_CIRLOG*/
/********************************************************************30**
           End of file:     fc_smm_tmr.c@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:52:34 2014

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
        ---         sasahni         1. initial release logging phase2
*********************************************************************91*/
