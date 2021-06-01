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
  
     Name:     common - linked list functions
  
     Type:     C include file
  
     Desc:     macros for linked list functions
 
     File:     cm_llist.h
  
     Sid:      cm_llist.h@@/main/9 - Thu Feb 21 14:52:11 2008
    
     Prg:      ak
 
*********************************************************************21*/
  
#ifndef __CMLLISTH__
#define __CMLLISTH__


/* cm_llist_h_001.main_8 */
#define cmLListFirst(l)  ((l)->crnt = (l)->first)
#define cmLListLast(l)   ((l)->crnt = (l)->last)
#define cmLListCrnt(l)   ((l)->crnt)
#define cmLListNext(l)   ((l)->crnt = ((l)->crnt ? (l)->crnt->next : \
                                       (l)->first))
#define cmLListPrev(l)   ((l)->crnt = ((l)->crnt ? (l)->crnt->prev : \
                                       (l)->first))
#define cmLListLen(l)    ((l)->count)
#define cmLListNode(n)   ((n)->node)

/* cm_llist_h_001.main_9 : added explicit check for warning fix */
#ifndef __cplusplus
#define CM_LLIST_FIRST_NODE(l, n)  ((((n) = cmLListFirst(l)) != NULLP) ? (n)->node : NULLP)
#define CM_LLIST_NEXT_NODE(l, n)   ((((n) = cmLListNext(l))!= NULLP) ? (n)->node : NULLP)
#define CM_LLIST_PREV_NODE(l, n)   ((((n) = cmLListPrev(l)) != NULLP) ? (n)->node : NULLP)
#else
#define CM_LLIST_FIRST_NODE(l, n)  ((n) = cmLListFirst(l))
#define CM_LLIST_NEXT_NODE(l, n)   ((n) = cmLListNext(l))
#define CM_LLIST_PREV_NODE(l, n)   ((n) = cmLListPrev(l))
#endif

#endif /* __CMLLISTH__ */

 
/********************************************************************30**
  
         End of file:     cm_llist.h@@/main/9 - Thu Feb 21 14:52:11 2008
  
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

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------
1.1          ---  ak    1. initial release

1.2          ---  bw    1. add 2 more macros for faster access
             ---  rrb   1. changed the macros CM_LLIST_FIRST_NODE and 
                           CM_LLIST_NEXT_NODE.
*********************************************************************81*/

/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.3          ---      rrb  1. changed the macros CM_LLIST_FIRST_NODE and
                              CM_LLIST_NEXT_NODE.
1.4          ---      bbk  1. Changed copyright header date.
1.5          ---      ag   1. Added CM_LLIST_PREV_NODE 
             ---      ag   2. Changed copyright header
/main/7      ---     rbabu 1. Updated for NBAP software release 1.2
cm_llist_h_001.main_7 --- rbhat 1. Modified Linked List Macros
/main/8      ---      
cm_llist_h_001.main_8 --- rbhat 1.Modified Linked list Macros for lvalue 
/main/9      ---      
$SID$     cm_llist_h_001.main_9   akaranth 1. Modified conditional check for warning fix
 *********************************************************************91*/
