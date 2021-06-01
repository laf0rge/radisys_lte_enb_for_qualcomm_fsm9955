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
 
     Name:     System Services -- Queueing
 
     Type:     C Include file
 
     Desc:     System Services queuing functions.
 
     File:     ss_queue.h
 
     Sid:      ss_queue.h@@/main/4 - Mon Nov 17 15:54:21 2008
 
     Prg:      bsr
  
*********************************************************************21*/


#ifndef __SSQUEUEH__
#define __SSQUEUEH__

#ifdef __cplusplus
extern "C" {
#endif


#define SS_MAX_TASK_PRI         4               /* maximum task priorities */
#define SS_MAX_MSG_PRI          4               /* maximum msg priorities */


#define SS_DQ_FIRST             0               /* queue at beginning */
#define SS_DQ_LAST              1               /* queue at end */


#define SS_MAX_NUM_DQ           (SS_MAX_TASK_PRI * SS_MAX_MSG_PRI)
                                                /* number of queues */
#define SS_DQ_BIT_MASK_LEN      ((SS_MAX_NUM_DQ - 1) /8 + 1)
                                                /* length of bitmask */

#define SS_MAX_DQ_PRIOR         (SS_MAX_TASK_PRI * SS_MAX_MSG_PRI) 


/* macros for first and last calls */
#define ssDmndQPutFirst(dQueue, mBuf, priority) \
        ssDmndQPut(dQueue, mBuf, (Prior)priority, SS_DQ_FIRST)
 
#define ssDmndQPutLast(dQueue, mBuf, priority) \
        ssDmndQPut(dQueue, mBuf, (Prior)priority, SS_DQ_LAST)
 
#define ssDmndQGetFirst(dQueue, mBuf) \
        ssDmndQGet(dQueue, mBuf, SS_DQ_FIRST)

#define ssDmndQGetLast(dQueue, mBuf) \
         ssDmndQGet(dQueue, mBuf, SS_DQ_LAST)
 

#ifdef __cplusplus
}
#endif

#endif /* __SSQUEUEH__ */



/********************************************************************30**
  
         End of file:     ss_queue.h@@/main/4 - Mon Nov 17 15:54:21 2008
  
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
1.1          ---      bsr  1. initial release.

1.2          ---      kp   1. C++ compilable, cosmetic changes

1.3          ---      kp   1. Corrected a typo

/main/4      ---      rp   1. SSI enhancements for Multi-core architecture
                              support
*********************************************************************91*/

