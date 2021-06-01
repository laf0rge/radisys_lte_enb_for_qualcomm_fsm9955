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

     Name:    EGTP - 

     Type:    C source file

     Desc:    

     File:    eg_tunn.h

     Sid:      eg_tunn.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:18:51 2015

     Prg:    mm  

*********************************************************************21*/

#ifndef __EG_TUNN_H__
#define __EG_TUNN_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************
 *                             MACROS                                         *
 ******************************************************************************/

#define ZERO 0

/******************************************************************************
* Macro to send an alarm in case of hash failure *
 ******************************************************************************/
#define SEND_HASH_FAIL_ALARM()\
   {\
      EgUsta     sta; \
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));     \
      sta.dgn.dgnVal[0].type    = LEG_PAR_GEN; \
      egSendLmAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_LYR_SPECIFIC,\
            LCM_CAUSE_HASH_FAIL, &sta.dgn);\
   }

/******************************************************************************
* macro to get the pointer to the tunnel Control block given the event *
 ******************************************************************************/

#define GET_TUNN_FRM_EVENT(event, teCCb)\
   {\
      if((event->lclTeid.pres != PRSNT_NODEF) || (event->lclTeid.val == 0))\
      {\
         *teCCb = NULLP;\
         EG_DBG_ERR(EG_DBG_TUNN, 0, (egp, \
                  "Message received on a non-existent tunnel.")); \
         event->resultType = EGT_ERR_UNKNOWN_TEID_MSG; \
         egSendErrInd(event, usap); \
         EG_RETVALUE( RFAILED); \
      }\
      else\
      {\
         EgTECCb **teCCbIdx; \
         teCCbIdx = NULLP; \
         teCCbIdx = egGetTunnelCb(event->lclTeid.val);\
         /* eg012.201:klockwork fix */\
         if ((teCCbIdx != NULLP) && (*teCCbIdx != NULLP))\
         {\
            *teCCb = *teCCbIdx;\
      }\
         else\
         {\
            EG_RETVALUE(RFAILED);\
         }\
      }\
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EG_TUNN_H__ */


  
/********************************************************************30**

         End of file:     eg_tunn.h@@/main/TeNB_Main_BR/tenb_main_ccb/1 - Wed Jul 22 18:18:51 2015

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**

    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---      mm                1. Created for Initial Release 1.1
/main/2      ---      rsharon           1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3   eg012.201   shpandey         1. Added klockwork fix
*********************************************************************91*/
