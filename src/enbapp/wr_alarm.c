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
  
     Name:     Fault Management Module
  
     Type:    
  
     Desc:     
  
     File:     wr_alarm.c
  
     Sid:      fc_alarm.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/1 - Tue Jul 23 17:00:52 2013
  
     Prg:      pagarwal

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="ENBAPP";
static int RLOG_MODULE_ID=64;
static int RLOG_FILE_ID=103;

#include "wr.h"
#include "wr_smm_init.h"
#include "wr_emm.h"
#include "wr_alarm.h"
PUBLIC S16 wrX2AlarmHandle (WrNeighEnbCb *nbrEnbCb, U16 alarmId);
PUBLIC S16 wrS1AlarmHandle (WrMmeCb *mmeCb, U16 alarmId);

#define WR_MAX_CNT_ONE 1


S1Alarm s1Alarm[16] =
{  {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO,"S1_ERROR_IND"},
   {L_INFO,"S1_RESET"},
   {L_INFO, "\0"}
};


X2Alarm x2Alarm[18] =
{
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO, "\0"},
   {L_INFO,"X2_ERROR_IND"},
   {L_INFO,"X2_RESET"},
   {L_INFO, "\0"}
};

/** @brief This function will be called whenever any alarm is raised or any
 *         existing alarm has been cleared.
 *          
 * @details This function will fill the information in the WrAlarmInfo structure
 *          and will pass it to packing function.
 *          
 *          
 *     Function: wrAlarmEvt
 *          
 * @param[in]    alarmId 
 * @param[in]    severity
 * @param[in]    cause
 * @param[in]    causeVal  
 *
 * @return  S16
 *        -# Success : ROK
 */
PUBLIC S16 wrAlarmEvt
(
  WrAlarmId       alarmId,
  WrAlarmSeverity severity,
  U8              causeType,
  U8              causeVal
 )
{
   LwrMngmt *usta = NULLP;

   TRC2(wrAlarmSend);

   WR_ALLOC(&usta, sizeof(LwrMngmt));
   if(usta == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Fill Header */
   usta->hdr.msgType             = TUSTA;
   usta->hdr.entId.ent           = ENTWR;
   usta->hdr.entId.inst          = SM_INST_ZERO;
   usta->hdr.elmId.elmnt         = STWRGEN;

   usta->t.usta.alarm.category = LCM_CATEGORY_PROTOCOL;
   usta->t.usta.alarm.event    = LWR_EVENT_SEND_ALARM;
   usta->t.usta.alarm.cause    = LCM_CAUSE_UNKNOWN;
   
   usta->t.usta.dgn.type = LWR_USTA_DGNVAL_ALARM;

   usta->t.usta.dgn.u.alarm.alarmId = alarmId;
   usta->t.usta.dgn.u.alarm.severity = severity;
   usta->t.usta.dgn.u.alarm.causeType = causeType;
   usta->t.usta.dgn.u.alarm.causeVal = causeVal;

   /* Send an unsolicited status indication */
   WrMiLwrStaInd(&wrCb.init.lmPst, usta);
 
   RETVALUE(ROK);
} /* end of wrAlarmEvt */

/** 
 *     Function: wrS1AlarmHandle 
 *
 *       @brief: This function is used to check whether the alarm raised is
 *               frequent or not.
 *          
 *     @details: In this function whenever first reset/error indication will be
 *                 received for any MME, it will store its time, and any time
 *                 reset/error indication will come for same MME, count will be
 *                 incremented. When count for any MME will reach the MAX value
 *                 it will check the time difference between current time and
 *                 the first reset/error indication time. If condition for
 *                 alarm is fulfilled, it will raise alarm and reset the time
 *                 and count for that MME.
 *          
 *   @param[in]: mmeCb
 *   @param[in]: alarmId
 *
 *      @return: S16
 *  -# Success : ROK
 */

PUBLIC S16 wrS1AlarmHandle
(
 WrMmeCb    *mmeCb,
 U16        alarmId
 )
{
   Bool    raiseAlarm = FALSE;
   Ticks   currTime = NULLP;

   TRC2(wrS1AlarmHandle);

   SGetSysTime(&(currTime));

   switch(alarmId)
   {
      case WR_ALARM_ID_S1_ERROR_IND:
         /* if max count is 1, no need to check time, directly will raise alarm */
         if(WR_MAX_CNT_ONE == mmeCb->alarmVal.maxErrIndCnt)
         {
            raiseAlarm = TRUE;
         }
         else
         {
            /* check for alarm count reached max */
            if(mmeCb->alarmVal.alarmCnt >= mmeCb->alarmVal.maxErrIndCnt - 1)
            {
               /* check for alarm time still less that minimum alarm time */
               if((currTime - mmeCb->alarmVal.alarmTime) <= mmeCb->alarmVal.minErrIndTime)
               {
                  raiseAlarm = TRUE;
               }
               mmeCb->alarmVal.alarmCnt = NULLP;
               mmeCb->alarmVal.alarmTime = NULLP;
            }
            else /* alarm count not reached to max */
            {
               /* if alarm time is NULL, this alarm came first time, so update time and count */
               if(mmeCb->alarmVal.alarmTime == NULLP)
               {
                  mmeCb->alarmVal.alarmTime = currTime;
                  mmeCb->alarmVal.alarmCnt++;
               }
               /* if alarm time already crossed min time limit, reset count and time */
               else if((currTime - mmeCb->alarmVal.alarmTime) > mmeCb->alarmVal.minErrIndTime)
               {
                  mmeCb->alarmVal.alarmCnt = 1;
                  mmeCb->alarmVal.alarmTime = currTime;
               }
               else
               {
                  mmeCb->alarmVal.alarmCnt++;
               }
            }
         }
         break;
      case WR_ALARM_ID_S1_RESET:
         /* if max count is 1, no need to check time, directly will raise alarm */
         if(WR_MAX_CNT_ONE == mmeCb->alarmVal.maxResetCnt)
         {
            raiseAlarm = TRUE;
         }
         else
         {
            /* check for alarm count reached max */
            if(mmeCb->alarmVal.alarmCnt >= mmeCb->alarmVal.maxResetCnt - 1)
            {
               /* check for alarm time still less that minimum alarm time */
               if((currTime - mmeCb->alarmVal.alarmTime) <= mmeCb->alarmVal.minResetTime)
               {
                  raiseAlarm = TRUE;
               }
               mmeCb->alarmVal.alarmCnt = NULLP;
               mmeCb->alarmVal.alarmTime = NULLP;
            }
            else /* alarm count not reached to max */
            {
               /* if alarm time is NULL, this alarm came first time, so update time and count */
               if(mmeCb->alarmVal.alarmTime == NULLP)
               {
                  mmeCb->alarmVal.alarmTime = currTime;
                  mmeCb->alarmVal.alarmCnt++;
               }
               /* if alarm time already crossed min time limit, reset count and time */
               else if((currTime - mmeCb->alarmVal.alarmTime) > mmeCb->alarmVal.minResetTime)
               {
                  mmeCb->alarmVal.alarmCnt = 1;
                  mmeCb->alarmVal.alarmTime = currTime;
               }
               else
               {
                  mmeCb->alarmVal.alarmCnt++;
               }
            }
         }
         break;
      default:
         ALARM("wrS1AlarmHandle: Invalid Cause\n");
         break;
   }
   U8 idx = 0 ;
   WR_CAL_ARR_IDX(11100 , alarmId, idx);
   RLOG_STR(s1Alarm[idx-1].logLevel,"[ALARM] Sending %s to OAM",(PSTR)s1Alarm[idx-1].s1AlarmType);
   RETVALUE(raiseAlarm);
}/* end of wrS1AlarmHandle */

/** 
 *     Function: wrX2AlarmHandle 
 *
 *       @brief: This function is used to check whether the alarm raised is
 *               frequent or not.
 *          
 *     @details: In this function whenever first reset/error indication will be
 *               received for any Neigh Enb, it will store its time, and any
 *               time reset/error indication will come for same Neigh Enb,
 *               count will be incremented. When count for any Neigh Enb will
 *               reach the MAX value it will check the time difference between
 *               current time and the first reset/error indication time. If
 *               condition for alarm is fulfilled, it will raise alarm and reset
 *               the time and count for that Neigh Enb.
 *          
 *   @param[in]: nbrEnbCb 
 *   @param[in]: alarmId
 *
 *      @return: S16
 *  -# Success : ROK
 */

PUBLIC S16 wrX2AlarmHandle
(
 WrNeighEnbCb *nbrEnbCb,
 U16          alarmId
 )
{
   Bool    raiseAlarm = FALSE;
   Ticks   currTime = NULLP;

   TRC2(wrX2AlarmHandle);

   SGetSysTime(&(currTime));

   switch(alarmId)
   {
      case WR_ALARM_ID_X2_ERROR_IND:
         /* if max count is 1, no need to check time, directly will raise alarm */
         if(WR_MAX_CNT_ONE == nbrEnbCb->alarmVal.maxErrIndCnt)
         {
            raiseAlarm = TRUE;
         }
         else
         {
            /* check for alarm count reached max */
            if(nbrEnbCb->alarmVal.alarmCnt >= nbrEnbCb->alarmVal.maxErrIndCnt - 1)
            {
               /* check for alarm time still less that minimum alarm time */
               if((currTime - nbrEnbCb->alarmVal.alarmTime) <= nbrEnbCb->alarmVal.minErrIndTime)
               {
                  raiseAlarm = TRUE;
               }
               nbrEnbCb->alarmVal.alarmCnt = NULLP;
               nbrEnbCb->alarmVal.alarmTime = NULLP;
            }
            else /* alarm count not reached to max */
            {
               /* if alarm time is NULL, this alarm came first time, so update time and count */
               if(nbrEnbCb->alarmVal.alarmTime == NULLP)
               {
                  nbrEnbCb->alarmVal.alarmTime = currTime;
                  nbrEnbCb->alarmVal.alarmCnt++;
               }
               /* if alarm time already crossed min time limit, reset count and time */
               else if((currTime - nbrEnbCb->alarmVal.alarmTime) > nbrEnbCb->alarmVal.minErrIndTime)
               {
                  nbrEnbCb->alarmVal.alarmCnt = 1;
                  nbrEnbCb->alarmVal.alarmTime = currTime;
               }
               else
               {
                  nbrEnbCb->alarmVal.alarmCnt++;
               }
            }
         }
         break;
      case WR_ALARM_ID_X2_RESET:
         /* if max count is 1, no need to check time, directly will raise alarm */
         if(WR_MAX_CNT_ONE == nbrEnbCb->alarmVal.maxResetCnt)
         {
            raiseAlarm = TRUE;
         }
         else
         {
            /* check for alarm count reached max */
            if(nbrEnbCb->alarmVal.alarmCnt >= nbrEnbCb->alarmVal.maxResetCnt - 1)
            {
               /* check for alarm time still less that minimum alarm time */
               if((currTime - nbrEnbCb->alarmVal.alarmTime) <= nbrEnbCb->alarmVal.minResetTime)
               {
                  raiseAlarm = TRUE;
               }
               nbrEnbCb->alarmVal.alarmCnt = NULLP;
               nbrEnbCb->alarmVal.alarmTime = NULLP;
            }
            else /* alarm count not reached to max */
            {
               /* if alarm time is NULL, this alarm came first time, so update time and count */
               if(nbrEnbCb->alarmVal.alarmTime == NULLP)
               {
                  nbrEnbCb->alarmVal.alarmTime = currTime;
                  nbrEnbCb->alarmVal.alarmCnt++;
               }
               /* if alarm time already crossed min time limit, reset count and time */
               else if((currTime - nbrEnbCb->alarmVal.alarmTime) > nbrEnbCb->alarmVal.minResetTime)
               {
                  nbrEnbCb->alarmVal.alarmCnt = 1;
                  nbrEnbCb->alarmVal.alarmTime = currTime;
               }
               else
               {
                  nbrEnbCb->alarmVal.alarmCnt++;
               }
            }
         }
         break;
      default:
         ALARM("wrX2AlarmHandle: Invalid Cause\n");
         break;
   }
   U8 idx = 0;
   WR_CAL_ARR_IDX(11100 , alarmId, idx);
   RLOG_STR(x2Alarm[idx-1].logLevel,"[ALARM] Sending %s to OAM",(PSTR)x2Alarm[idx-1].x2AlarmType);
   RETVALUE(raiseAlarm);
}/* end of wrX2AlarmHandle */



/********************************************************************30**

         End of file:     fc_alarm.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/1 - Tue Jul 23 17:00:52 2013

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
     -   ---     
*********************************************************************91*/
