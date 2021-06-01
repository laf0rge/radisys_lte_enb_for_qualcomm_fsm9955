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
  
     Name:     common functions - linked list management
  
     Type:     C source file
  
     Desc:     common functions for linked lists
  
     File:     rg_sch_utl_clist.c
  
     Sid:      gk_sch_utl_clist.c@@/main/2 - Sat Jul 30 02:21:53 2011
 
     Prg:      rr
  
*********************************************************************21*/
  
/* header include files (.h) */

#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */

/* header/extern include files (.x) */
  
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "rg_sch_clist.x"      /* common functions */


/* Linked List functions */


/*
*
*       Fun:   rgSCHRrCListInit
*
*       Desc:  initializes a linked list control pointer.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  rr_clist.c
*
*/
#ifdef ANSI
PUBLIC Void rgSCHRrCListInit
(
RgSchRrCListCp *lCp                /* list control point */
)
#else 
PUBLIC Void rgSCHRrCListInit(lCp)
RgSchRrCListCp *lCp;               /* list control point */
#endif
{
   TRC2(rgSCHRrCListInit);
   
   lCp->first = (RgSchRrCList *)NULLP;
   lCp->crnt  = (RgSchRrCList *)NULLP;
   lCp->count = 0;

   RETVOID;
} /* end of rgSCHRrCListInit */

/* LTE_ADV_FLAG_REMOVED_START */
/*
 *        Fun:   rgSCHRrCListAdd2Crnt
 * 
 *        Desc:  adds node to linked list behind crnt.
 * 
 *        Ret:   ROK   - ok
 * 
 *        Notes: None
 * 
 *        File:  rr_clist.c
 */
#ifdef ANSI
PUBLIC Void rgSCHRrCListAdd2Crnt
(
 RgSchRrCListCp *lCp,               /* list control point */
 RgSchRrCList   *node               /* node to be added */
 )
#else
PUBLIC Void rgSCHRrCListAdd2Crnt(lCp, node)
   RgSchRrCListCp *lCp;               /* list control point */
   RgSchRrCList   *node;              /* node to be added */
#endif
{
   TRC2(rgSCHRrCListAdd2Crnt);
#ifdef ERRCHK
   if (lCp == (RgSchRrCListCp *)NULLP)
      RETVOID;
#endif

   lCp->count++;

   if(!lCp->first)
   {
      node->prev = node;
      node->next = node;
      lCp->first = node;

      lCp->crnt = lCp->first;

      RETVOID;
   }

   node->next = lCp->crnt;
   node->prev = lCp->crnt->prev;
   lCp->crnt->prev->next = node;
   lCp->crnt->prev = node;

   RETVOID;
}
/* LTE_ADV_FLAG_REMOVED_END */

/*
*
*       Fun:   rgSCHRrCListAdd2Tail
*
*       Desc:  adds node to linked list after last.
*
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  rr_clist.c
*
*/
#ifdef ANSI
PUBLIC Void rgSCHRrCListAdd2Tail
(
RgSchRrCListCp *lCp,               /* list control point */
RgSchRrCList   *node               /* node to be added */
)
#else 
PUBLIC Void rgSCHRrCListAdd2Tail(lCp, node)
RgSchRrCListCp *lCp;               /* list control point */
RgSchRrCList   *node;              /* node to be added */
#endif
{
   TRC2(rgSCHRrCListAdd2Tail);

#ifdef ERRCHK
   if (lCp == (RgSchRrCListCp *)NULLP)
      RETVOID;
#endif
 
   lCp->count++;

   if(!lCp->first)
   {
      node->prev = node;
      node->next = node;
      lCp->first = node;

      lCp->crnt = lCp->first;

      RETVOID;
   }

   node->next = lCp->first;
   node->prev = lCp->first->prev;
   lCp->first->prev->next = node;
   lCp->first->prev = node;

   RETVOID;
} /* end of rgSCHRrCListAdd2Tail */

/*
*
*       Fun:   rgSCHRrCListDelFrm
*
*       Desc:  remove node pointed to by nodePtr from list and return node.
*              nodePtr could be anywhere in the list.
*              - resets crnt to NULLP.
*
*       Ret:   pointer
*
*       Notes: None
*
*       File:  rr_clist.c
*
*/
#ifdef ANSI
PUBLIC RgSchRrCList *rgSCHRrCListDelFrm
(
RgSchRrCListCp *lCp,                /* list control pointer */
RgSchRrCList *node                  /* node to be removed */
)
#else 
PUBLIC RgSchRrCList *rgSCHRrCListDelFrm(lCp, node)
RgSchRrCListCp *lCp;               /* list control pointer */
RgSchRrCList *node;                /* node to be removed */
#endif
{
   TRC2(rgSCHRrCListDelFrm);
  
#ifdef ERRCHK
   if (lCp == (RgSchRrCListCp *)NULLP)
      RETVALUE(NULLP);
#endif

   if(lCp->count == 0)
   {
      RETVALUE(NULLP);
   }
   if (lCp->count == 1)
   {
      if(lCp->first == node)
      {
         lCp->first = lCp->crnt = (RgSchRrCList *)NULLP;
         lCp->count = 0;
         node->next = node->prev = (RgSchRrCList *)NULLP;
         RETVALUE(node);
      }
      RETVALUE(NULLP);
   }
   
   if (lCp->first == node)
   {
      lCp->first->prev->next = node->next;
      node->next->prev = lCp->first->prev;
      lCp->first = node->next;
      if(lCp->crnt == node)
      {
         lCp->crnt = node->next;
      }
      node->next = node->prev = (RgSchRrCList *)NULLP;
       /* Adding this check and guarding the decrement of counter when
       node is deleted with reshuffling */
      lCp->count--;
      RETVALUE(node);
   }

   if(node->prev && node->next)
   {
      node->prev->next = node->next;
      node->next->prev = node->prev;
      lCp->count--;
   }
   if(lCp->crnt == node)
   {
      lCp->crnt = node->next;
   }
   node->next = node->prev = (RgSchRrCList *)NULLP;
   RETVALUE(node);
} /* end of rgSCHRrCListDelFrm */

/*
*
*       Fun:   rgSCHRrCListInsrtAtCrnt
*
*       Desc:  Inserting the given node at CuRRENT and Moving present CURRENT 
*              node to next.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  rr_clist.c
*
*/
#ifdef ANSI
PUBLIC Void rgSCHRrCListInsrtAtCrnt
(
RgSchRrCListCp *lCp,                /* list control pointer */
RgSchRrCList *node                  /* node to be removed */
)
#else 
PUBLIC Void rgSCHRrCListInsrtAtCrnt(lCp, node)
RgSchRrCListCp *lCp;               /* list control pointer */
RgSchRrCList *node;                /* node to be inserted */
#endif
{
   RgSchRrCList *crnt;
   TRC2(rgSCHRrCListInsrtAtCrnt);

#ifdef ERRCHK
   if (lCp == (RgSchRrCListCp *)NULLP)
      RETVOID;
#endif

   crnt = lCp->crnt;
   lCp->crnt = node;

   node->prev = crnt->prev;
   crnt->prev->next = node;
   node->next = crnt;
   crnt->prev = node;

   lCp->count++;

   RETVOID;
}

/********************************************************************30**
  
         End of file:     gk_sch_utl_clist.c@@/main/2 - Sat Jul 30 02:21:53 2011
   
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

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------
/main/1      ---  rr    1. initial release
        rg003.201 ns    1. tarce added to functions.
        rg004.201 ms    1. Functions and Datatypes starting 
                           with rr are renamed as rgSchRr
        rg005.201 sm    1. Fix for CR ccpu00110744
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---  rt    1. LTE MAC 4.1 release
*********************************************************************91*/
