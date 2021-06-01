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
  
     File:     wr_alarm.h 
  
     Sid:      wr_alarm.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/1 - Tue Jul 23 17:01:13 2013
  
     Prg:      pagarwal 

*********************************************************************21*/

#ifndef __WR_ALARM_H__
#define __WR_ALARM_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

/** @brief Enum defines alarm id **/
typedef enum
{
   WR_ALARM_ID_FIRST = 11100,                       /*!< first alarm id */
   WR_ALARM_ID_CFG_PLAN_ERROR = WR_ALARM_ID_FIRST,  /*!< Critical Configuration Plan Error */
   WR_ALARM_ID_CFG_PLAN_INCONSISTENCY,              /*!< Configuration Plan Inconsistency */
   WR_ALARM_ID_FREQ_SYNC_FAIL,                      /*!< Frequency Synchronization Failure */
   WR_ALARM_ID_NTP_SERVER_NOT_RESPONDING,           /*!< NTP Server Not Responding */
   WR_ALARM_ID_OSCILLATOR_STABILITY_PROBLEM,        /*!< Oscillator Stability Problem */
   WR_ALARM_ID_FAP_TEMP_OVER_THRESH,                /*!< FAP Temperature Above Threshold */
   WR_ALARM_ID_PCI_CONFLICT,                        /*!< PCI conflict */
   WR_ALARM_ID_LTE_LOW_QUALITY_CHANNEL,             /*!< Low Quality LTE channel */
   WR_ALARM_ID_NETWORK_JITTER,                      /*!< Network Jitter */
   WR_ALARM_ID_S1_SETUP_FAIL,                       /*!< S1 Setup failure */
   WR_ALARM_ID_X2_SETUP_FAIL,                       /*!< X2 Setup failure */
   WR_ALARM_ID_MEM_ALLOC_FAIL,                      /*!< Memory allocation failure */
   WR_ALARM_ID_SCTP_LNK_FAIL,                       /*!< SCTP link failure */
   WR_ALARM_ID_TNL_SETUP_FAIL,                      /*!< User plane TNL setup failure */
   WR_ALARM_ID_S1_ERROR_IND,                        /*!< Frequent S1 Error Indications from MME */
   WR_ALARM_ID_S1_RESET,                            /*!< Frequent S1 Resets from MME */
   WR_ALARM_ID_X2_ERROR_IND,                        /*!< Frequent X2 Error Indications from neighbour */
   WR_ALARM_ID_X2_RESET                            /*!< Frequent X2 Resets from neighbour */
}WrAlarmId;

/** @brief Enum defines alarm severity **/
typedef enum
{
   WR_ALARM_SEVERITY_CLEARED,
   WR_ALARM_SEVERITY_WARNING,
   WR_ALARM_SEVERITY_MINOR,
   WR_ALARM_SEVERITY_MAJOR,
   WR_ALARM_SEVERITY_CRITICAL
} WrAlarmSeverity;

#define S1_CAUSE_RADIO_NW 0
#define S1_CAUSE_TRANSPORT 1
#define S1_CAUSE_NAS 2
#define S1_CAUSE_PROTOCOL 3
#define S1_CAUSE_MISC 4

#define X2_CAUSE_RADIO_NW 0
#define X2_CAUSE_TRANSPORT 1
#define X2_CAUSE_PROTOCOL 2
#define X2_CAUSE_MISC 3

/*This macro calcualtes arrayIdx */
#define WR_CAL_ARR_IDX(_BASE, _OFFSET, _IDX)\
{\
  _IDX = _OFFSET - _BASE; \
}

EXTERN S16 wrAlarmEvt(WrAlarmId alarmId, WrAlarmSeverity severity,
                            U8 causeType, U8 causeVal);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif/* __WR_ALARM_H__*/

/********************************************************************30**

         End of file:     wr_alarm.h@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/1 - Tue Jul 23 17:01:13 2013

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
