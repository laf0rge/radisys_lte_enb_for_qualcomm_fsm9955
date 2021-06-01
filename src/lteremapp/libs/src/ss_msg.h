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
 
     Name:     System Services -- Message manipulation functions
 
     Type:     C source file
 
     Desc:     Macro definitions for message related functions.
 
     File:     ss_msg.h
 
     Sid:      ss_msg.h@@/main/3 - Mon Nov 17 15:54:20 2008
 
     Prg:      sn
 
*********************************************************************21*/

#ifndef __SSMSGH__
#define __SSMSGH__

#ifdef __cplusplus
extern "C" {
#endif


/* default SS region ID */
#define SS_DFLT_REGION                  0
#define SS_DFLT_POOL                    0

#ifdef SS_MULTICORE_SUPPORT
#define SS_REGION_0                     0
#define SS_REGION_1                     1
#define SS_REGION_2                     2
#define SS_REGION_3                     3
#define SS_REGION_4                     4
#define SS_REGION_5                     5
#endif /* SS_MULTICORE_SUPPORT */

/* pool types */
#define SS_POOL_UND                     0
#define SS_POOL_DYNAMIC                 1
#define SS_POOL_STATIC                  2


/* utility macros */
#define FIND_OFFSET(current, idx)   \
   { \
      MsgLen bufSiz; \
      while (idx) \
      { \
         bufSiz = (current->b_wptr - current->b_rptr); \
         if (bufSiz > idx) \
            break; \
         idx -= bufSiz; \
         current = current->b_cont; \
      } \
   }
 
#define FIND_OFFSET_AND_PREV(previous, current, idx)   \
   { \
      MsgLen bufSiz; \
      while (idx) \
      { \
         bufSiz = (current->b_wptr - current->b_rptr); \
         if (bufSiz > idx) \
            break; \
         idx -= bufSiz; \
         previous = current; \
         current = current->b_cont; \
      } \
   }


#ifdef __cplusplus
}
#endif

#endif /* __SSMSGH__ */

 

/********************************************************************30**
 
         End of file:     ss_msg.h@@/main/3 - Mon Nov 17 15:54:20 2008
 
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
1.1          ---      sn   1. initial release

1.2          ---      kp   1. C++ compilable, cosmetic changes
/main/3      ---      rp   1. SSI enhancements for Multi-core architecture
                              support
ss038.103   			 ve   1. Changed to MsgLen from S16.
/main/1+    ss002.301 prasanna  1. SDeRegTTsk
*********************************************************************91*/
