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
  
     Name:     common memory allocation library 
  
     Type:     C include file
  
     Desc:     Structures, variables and typedefs required by the
               memory library routines 
 
     File:     cm_mblk.x
  
     Sid:      cm_mblk.x@@/main/4 - Thu Oct 27 00:36:14 2005
    
     Prg:      mp
 
*********************************************************************21*/
  
#ifndef __CMMBLKX__
#define __CMMBLKX__

#ifdef __cplusplus
EXTERN "C" {
#endif

/************************************************
 Memory Management Control Block 
************************************************/

typedef struct cmMemCb
{
   Size      maxSize;      /* Size of memory chunk */ 
   Mem       sMem;         /* Static memory region and pool */ 
   U32       memAllocated; /* amount of memory already allocated */
   PTR       initPtr;      /* Initial pointer */
   PTR       runPtr;       /* Start of avaliable memory chunk */ 
} CmMemCb;

/************************************************
 Memory Link List Node Structure
************************************************/
typedef struct  cmMemList CmMemList;

struct cmMemList
{
   CmMemList *next;        /* next */
   CmMemList *prev;        /* previous */
   /* cm_mblk_x_001.101: added new element "size" */
   Size       size;        /* block size */
};

/************************************************
 Memory Link List Control Point 
************************************************/

typedef struct cmMemListCp
{
   CmMemList *first;       /* first entry in list */
   CmMemList *last;        /* last entry in list */
   U32       count;        /* number of entries */
   CmMemCb   memCb;        /* Memory Control Block */
}CmMemListCp;

/***********************************************
 Memory Status structure 
***********************************************/
typedef struct cmMemStatus
{
   Mem       sMem;          /* Static Memory region,pool */
   U32       memBlkCnt;     /* Memory Blocks Count */ 
   Size      maxBlkSize;    /* Size of memory Block */
   Size      memAllocated;  /* Memory allocated off chunk */
} CmMemStatus;


/***********************************************
 Extern Declarations
***********************************************/

#ifdef TFU_ALLOC_EVENT_NO_INIT
EXTERN  S16 cmAllocEvntNoInit ARGS((
                                   Size evntSize,
                                   Size maxBlkSize,
                                   Mem  *sMem, 
                                   Ptr  *ptr));
#endif
EXTERN  S16 cmAllocEvnt ARGS((
                                   Size evntSize,
                                   Size maxBlkSize,
                                   Mem  *sMem, 
                                   Ptr  *ptr));

EXTERN  Void cmInitMemCp ARGS((
                                   CmMemListCp *memCp,
                                   Size  maxBlkSize,
                                   Mem   *sMem )); 

#ifdef TFU_ALLOC_EVENT_NO_INIT
EXTERN  S16 cmGetMemNoInit ARGS((
                               Ptr  memPtr,
                               Size size, 
                               Ptr  *allocPtr));
#endif
EXTERN  S16 cmGetMem ARGS((
                               Ptr  memPtr,
                               Size size, 
                               Ptr  *allocPtr));

EXTERN  Void cmFreeMem ARGS((
                                Ptr memPtr));

EXTERN  Void cmGetMemStatus ARGS((
                                  Ptr         memPtr,
                                  CmMemStatus *status));

#ifdef __cplusplus
}
#endif

#endif /* __CMMBLKX__ */

 
/********************************************************************30**
  
         End of file:     cm_mblk.x@@/main/4 - Thu Oct 27 00:36:14 2005
  
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
1.1          ---      mp   1. initial release
/main/3      ---      wh   1. added new element"size" in CmMemList structure.
/main/4      ---      rbabu 1. Updated copy right information
*********************************************************************91*/
