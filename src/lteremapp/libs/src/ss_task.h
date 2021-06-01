/********************************************************************16**

                         (c) COPYRIGHT 2006 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

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
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9380, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/


/********************************************************************20**
 
     Name:     System Services -- Task Management
 
     Type:     C include file
 
     Desc:     Various macro definitions required for the task mgmt.
 
     File:     ss_task.h
 
     Sid:      ss_task.h@@/main/3 - Mon Nov 17 15:54:24 2008
 
     Prg:      kp
  
*********************************************************************21*/


#ifndef __SSTASKH__
#define __SSTASKH__

#ifdef __cplusplus
extern "C" {
#endif


/* task types */
#define SS_TSK_UND              0               /* undefined */
#define SS_TSK_NORMAL           1               /* normal task */
#define SS_TSK_PERMANENT        2               /* permanent task */

/* ss001.301: additions */
#ifdef SS_LOGGER_SUPPORT 
#define SS_LOG_TO_FILE          0               /* write log to file */
#define SS_LOG_TO_SOCKET        1               /* write log to socket */
#define SS_LOG_ALL              2               /* write log to file and socket*/
#endif /* SS_LOGGER_SUPPORT */

#ifdef SS_WATCHDOG
#define ENTDW                           0xcd
#define ENTHB                           0xce
#define INST0                           0x00
#define SS_TMR_HRTBT                    0x00
#define EVTSSHRTBTREQ                   0x00
#define EVTSSHRTBTRSP                   0x01
#define SS_LOOSE_COUPLING               0x00
#endif /* SS_WATCHDOG */

/* task priorities (these are system task priorities--0 to 31) */
#define SS_NORM_TSK_PRI         13              /* for normal tasks */
#define SS_PERM_TSK_PRI         21              /* for permanent tasks */


#define SS_LOWEST_STSK_PRIOR    31              /* lowest sys task priority */


#ifdef __cplusplus
}
#endif

#endif /* __SSTASKH__ */


/********************************************************************30**
  
         End of file:     ss_task.h@@/main/3 - Mon Nov 17 15:54:24 2008
  
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
1.1          ---      kp   1. initial release

1.2          ---      kp   1. C++ compilable, cosmetic changes
1.2+         ss001.301 schandra1 1. SSI Ph2 value added feature additions

*********************************************************************91*/
