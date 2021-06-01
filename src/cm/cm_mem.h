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
 
     Name:     Common Memory Manager 
 
     Type:     C include file
 
     Desc:     Defines that are required by the Common Memory Manager.
 
     File:     cm_mem.h
 
     Sid:      cm_mem.h@@/main/11 - Fri Feb 26 13:13:53 2010
 
     Prg:      rm
 
*********************************************************************21*/

#ifndef __CMMEMH_
#define __CMMEMH_

/* cm_mem_h_001.103 - Addition */
#ifdef __cplusplus
extern "C" {
#endif

#define CMM_MINBUFSIZE   (PTRALIGN(sizeof(CmHEntry)))
#define CMM_DATALIGN(s, msz)  (((Size)(s) % msz) ? ((Size)(s) + ((msz - (Size)(s) % msz))): (Size)(s)) 

#define CMM_BUFSIZE(s, msz)   ((Size)(s) < (Size) msz ? \
                                      (Size) msz : \
                                      (Size) CMM_DATALIGN(s, msz))

/* defines */
/* cm_mem_h_001.103 - Addition for MEMCAL */
#ifndef CMM_MAX_BKT_ENT
#define  CMM_MAX_BKT_ENT    30  
#endif
#ifndef CMM_MAX_MAP_ENT
/* cm_mem_h_001.main_10: Modifications*/
#define  CMM_MAX_MAP_ENT    512
#endif

/* cm_mem_h_001.main_6 - addition for SSI enhancements */
#if (defined(SSI_DEBUG_LEVEL1) || defined(BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1) || defined (SS_LIGHT_MEM_LEAK_STS) || defined(SS_DEBG_MEM_CORRUP_LEVEL1))
/* 
* Memory block header signature length
* Customer might choose to modify this to set his desired trampling signature length
* Customer should choose a value between 1 byte to 4 bytes 
*/
#define CMM_TRAMPLING_SIGNATURE_LEN   4

/* 
* Maximum number of trampling allowed
* Customer might choose to modify this, however too big a value is not suggestible
*/
#define CMM_TRAMPLING_THRESHOLD   3

/* 
* Hash list size to mantain the statistics of size requested & the number of attempts for allocation
* Customer might choose to modify this to accomodate statistics for more sizes
* preferably choose a prime number for better performance
*/
#define CMM_STAT_HASH_TBL_LEN   211 /* prime number */

/* values for memFlags in CmMmBlkHdr */
#define CMM_FREE_FLAG         0x01
#define CMM_DYNAMIC_MEM_FLAG  0x04
#define CMM_STATIC_MEM_FLAG   0x08

/* 
* MACROs for setting/resetting above flags
* FREE state related flags 
*/
#define CMM_SET_FREE_FLAG(x) ((x) |= CMM_FREE_FLAG)
#define CMM_RESET_FREE_FLAG(x) ((x) &= (~(CMM_FREE_FLAG)))
#define CMM_IS_FREE(x) ((x) & CMM_FREE_FLAG)

/* DYNAMIC state related flags */
#define CMM_SET_DYNAMIC_FLAG(x) ((x) |= CMM_DYNAMIC_MEM_FLAG)
#define CMM_RESET_DYNAMIC_FLAG(x) ((x) &= (~(CMM_DYNAMIC_MEM_FLAG)))
#define CMM_IS_DYNAMIC(x) ((x) & CMM_DYNAMIC_MEM_FLAG)

/* STATIC state related flags */
#define CMM_SET_STATIC_FLAG(x) ((x) |= CMM_STATIC_MEM_FLAG)
#define CMM_RESET_STATIC_FLAG(x) ((x) &= (~(CMM_STATIC_MEM_FLAG)))
#define CMM_IS_STATIC(x) ((x) & CMM_STATIC_MEM_FLAG)
#endif /* SSI_DEBUG_LEVEL1 */

/* Valid Physical Bit */
#define  CMM_REG_PHY_VALID  0x01 
#define  CMM_REG_OUTBOARD   0x02 
/* cm_mem_h_001.main_7 : Additions */ 
#if (defined(SS_MEM_LEAK_STS) || defined(BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL1))
#ifndef CM_MEM_USR_MDL
#define CM_MEM_USR_MDL 10
#ifndef CM_MAX_STACK_TRACE
#define CM_MAX_STACK_TRACE 20
#endif /* if CM_MAX_STACK_TRACE is not defined in Makefile */
#endif
#define MEMRAW2STR(x,y)   {#x, #y}
#define CM_MAX_HASH_PER_TSK 4
#ifdef BRDCM_SSI_MEM_LEAK_DEBUG_LEVEL2
#define BRDCM_MEM_LEAK_BTRACE  6 /* Back trace */
#endif
#endif /* SS_MEM_LEAK_STS */ 

#ifdef SS_HISTOGRAM_SUPPORT
#define CMM_HIST_MAX_FILENAME 100
#define CMM_HIST_MAX_MEM_ENTRY_PER_BIN 25
#define CMM_HIST_MAX_MEM_BIN 25
#endif /* SS_HISTOGRAM_SUPPORT */

#ifdef SS_LOCKLESS_MEMORY
#define CMM_MAX_NUMBER_BKT_NODE   2
#endif

#ifdef SS_LIGHT_MEM_LEAK_STS
#define CM_MAX_ALLOC_ENTERIES 100000
#endif
/* cm_mem_h_001.103 */
#ifdef __cplusplus
}
#endif

#endif /* __CMMEMH_ */


/********************************************************************30**
 
         End of file:     cm_mem.h@@/main/11 - Fri Feb 26 13:13:53 2010
 
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
------------ -------- ---- -----------------------------------------------
1.1          ---      rm   1. initial release

1.2          ---      kp   1. C++ compilable

1.3          ---      dvs  1. rev sync
/main/5      ---      rbabu 1. update copy right information

/main/6      ---      bjp  1. Modifications for MEMCAL

/main/6  cm_mem_h_001.main_6      vk   1. Changes for SSI enhancements
/main/8      ---      ch   1. Updated copyright SSI Multicore Support release.
/main/9  cm_mem_h_001.main_7 schandra1 1. ssi-ph 2 value added feature additions
/main/10 cm_mem_h_001.main_9 pmacharla 1.CMM_MAX_MAP_ENT increased to 1024
/main/11     ---      cm_mem_h_001.main_10 pmacharla 1.CMM_MAX_MAP_ENT changing back to 128 as
                                          it may affect FAP environments
*********************************************************************91*/

 
