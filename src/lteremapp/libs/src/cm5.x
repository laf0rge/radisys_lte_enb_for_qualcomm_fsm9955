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
  
     Name:     common functions - 5
  
     Type:     C include file
  
     Desc:     Structures, variables and typedefs required for
               common timer routines
  
     File:     cm5.x
  
     Sid:      cm5.x@@/main/12 - Mon Nov 17 15:55:43 2008
  
     Prg:      na
  
*********************************************************************21*/

#ifndef __CM5X__
#define __CM5X__

#ifdef __cplusplus
extern "C" {
#endif


/* typedefs */

typedef struct cmTimer         /* timer structure */
{
   S16 tmrEvnt;                /* Timer Event */
   U32 tqExpire;               /* Timing Queue Expiry */
   PTR cb;                     /* Pointer to start of control block list */
   struct cmTimer *next;       /* Next item in the timing queue */
   struct cmTimer *prev;       /* Previous item in the timing queue */
   U8     ent2bUpd;            /* TRUE, if the timer block is not in the
                                  proper bin */
   U16   entIdx;                /* Current entry index */
} CmTimer;

typedef struct cmTqCp          /*  timing queue control point */
{
   U32 nxtEnt;                 /* next Entry */
   U32 tmrLen;                 /* length of timer (MODULO is -1) */
   CmTimer *tmp;               /* Temporary  variable used in cmPrcTmr */
} CmTqCp;

typedef struct cmTqType
{
   CmTimer *first;             /* First Entry */
   CmTimer *tail;              /* Last Entry */
} CmTqType;

typedef struct cmTmrArg
{
   CmTqCp     *tqCp;           /* timing queue control point */
   CmTqType   *tq;             /* timing queue point */
   CmTimer    *timers;         /* timers array */
   PTR        cb;              /* control block pointer */
   S16        evnt;            /* event */
   U32        wait;            /* wait */
   U8         tNum;            /* timer number */
   U8         max;             /* max timers */
} CmTmrArg;


/* trillium supplied functions */

typedef Void (*PFV) ARGS((PTR, S16));

extern Void cmInitTimers ARGS((CmTimer* timers, U8 max));
extern Void cmPlcCbTq ARGS((CmTmrArg* arg));
extern Void cmRmvCbTq ARGS((CmTmrArg* arg));
extern Void cmPrcTmr ARGS((CmTqCp* tqCp, CmTqType* tq, PFV func));
extern Void cmRstCbTq ARGS((CmTmrArg* arg));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __CM3X__ */


/********************************************************************30**
  
         End of file:     cm5.x@@/main/12 - Mon Nov 17 15:55:43 2008
  
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
1.1          ---  fmg   1. initial release.

1.2          ---  mc    1. replace U8 with S16 in PFV function typedef.

1.3          ---  mc    1. changed tmrEvnt and event type from U8 to S16 
                           in cmTimer and cmTmrArg.

*********************************************************************81*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.4          ---      bbk  1. Changed copyright header date.

1.5          ---      ns   1. Added extern "C" for making this file C++
                              compatible

1.6          ---      tej  1. Changed copyright header date.
1.7          ---      rm   1. Added new fields in CmTqCp and CmTqType for
                              the implementation of double link list timer list.
             ---      rm   2. Add a new parameter entIdx to CmTimer.
             ---      rm   3. Add prototype for cmRstCbTq
             ---      rm   4. Added a new parameter in CmTqCp for
                              timer list traversal in CmPrcTmr.
/main/9      ---      vt   1. Changes for 1 ms timer
/main/10     ---      st   1. Update for MAP Release 2.3
/main/11     ---      ad   1. Updated for Sip Release 2.2 Beta.
/main/12     ---      ch   1. Updated copyright SSI Multicore Support release.
*********************************************************************91*/
