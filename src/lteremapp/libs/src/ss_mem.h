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
 
     Name:     System Services -- Memory management interface
 
     Type:     C include file
 
     Desc:     Various macro definitions required for the Memory
               management interface.
 
     File:     ss_mem.h
 
     Sid:      ss_mem.h@@/main/3 - Mon Nov 17 15:54:17 2008
 
     Prg:      kp
 
*********************************************************************21*/


#ifndef __SSMEMH__
#define __SSMEMH__

#ifdef __cplusplus
extern "C" {
#endif



/* region flags */
#define SS_OUTBOARD_FLAG        16

/* ss036.103 - addition of a macro. This implies that we
* can hold the memory statistics of upto SS_MAX_BKT_PER_DBGTBL buckets
*/
#define SS_MAX_BKT_PER_DBGTBL   16

/* ss036.103 - addition of macros for memory statistics related information */
#ifdef SSI_DEBUG_LEVEL1
#define SS_DYNAMIC_MEM_FLAG     0x04 /* same as CMM_DYNAMIC_MEM_FLAG */
#define SS_STATIC_MEM_FLAG      0x08 /* same as CMM_STATIC_MEM_FLAG */
#define SS_MEM_BLK_SIZE_PROFILE    0  /* to print size vs. numRequests */
#define SS_MEM_BKT_ALLOC_PROFILE   1  /* to print the static/dynamic mem used */
#endif /* SSI_DEBUG_LEVEL1 */

/* ss001.301: FAP related changes in SSI */
#ifdef SS_FAP
#define SAlloc(_region, _size,_flags, _ptr) \
           ((osCp.regionTbl[(_region)].alloc)((osCp.regionTbl[(_region)].regCb), (_size), (_flags),(_ptr)))
#define SFree(_region, _ptr, _size) \
           (osCp.regionTbl[(_region)].free)(\
           (osCp.regionTbl[(_region)].regCb), ( _ptr), (_size))

#endif

#ifdef __cplusplus
}
#endif

#endif  /*  __SSMEMH__  */


  
/********************************************************************30**

         End of file:     ss_mem.h@@/main/3 - Mon Nov 17 15:54:17 2008

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
1.2+        ss036.103 vk   1. Changes for SSI enhancements
/main/3      ---      rp   1. SSI enhancements for Multi-core architecture
                              support
/main/3     ss003.301 mg   1. FAP related changes for SSI
*********************************************************************91*/
