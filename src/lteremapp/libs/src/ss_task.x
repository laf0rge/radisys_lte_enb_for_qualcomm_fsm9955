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
 
     Desc:     Data structure definitions required for the task mgmt.
 
     File:     ss_task.x
 
     Sid:      ss_task.x@@/main/3 - Mon Nov 17 15:54:24 2008
 
     Prg:      kp
 
*********************************************************************21*/


#ifndef __SSTASKX__
#define __SSTASKX__

#ifdef __cplusplus
extern "C" {
#endif


/* miscellaneous types */
/* ss029.103: modification: 
   data type changed to allow more number of TAPA tasks */ 
#ifndef SS_MULTIPLE_PROCS
typedef U8                      SsCntr;
typedef U8                      SsIdx;
#else /* SS_MULTIPLE_PROCS */
typedef U16                      SsCntr;
typedef U16                      SsIdx;
#endif /* SS_MULTIPLE_PROCS */


/* forward declaration */
typedef struct ssSTskEntry      SsSTskEntry;


/* individual entry in the table of TAPA tasks */
typedef struct ssTTskEntry
{
   SsdTTskEntry dep;                    /* implementation specific */


   Bool         used;                   /* entry is used? */
/* ss029.103: addition: procId added */ 
#ifdef SS_MULTIPLE_PROCS
   ProcId       proc;                   /* task processor ID */
#endif /* SS_MULTIPLE_PROCS */
   Ent          ent;                    /* task entity ID */
   Inst         inst;                   /* task instance ID */
   Ttype        tskType;                /* normal/permanent/driver */
   Prior        tskPrior;               /* priority of task */
   PAIFS16      initTsk;                /* initialization function */
   ActvTsk      actvTsk;                /* activation function */

   SsSTskEntry  *sTsk;                  /* system task */


   SsIdx        nxt;                    /* table implementation */
/* ss029.103: addition: TAPA task control block (user level) added */ 
#ifdef SS_MULTIPLE_PROCS
   Void         *xxCb;                  /* protocol control block */
#endif /* SS_MULTIPLE_PROCS */

/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT
   Bool         hstReg;
#endif /* SS_HISTOGRAM_SUPPORT */

/* ss001.301: additions */
/* ss002.301: Modifications */
#ifdef SS_THREAD_PROFILE
   Bool        updated;
   Event       curEvent;
   U32         curEvtTime;
   U64         totTime;
#endif /* SS_THREAD_PROFILE */
	/* ss02.301 */
	ActvTsk     cbTsk; /* call back function pointer */

} SsTTskEntry;



/* individual entry in the table of system tasks */
struct ssSTskEntry
{
   SsdSTskEntry dep;                    /* implementation specific */


   Bool         used;                   /* entry is used or not */
   Bool         termPend;               /* termination pending */
   SSTskId      tskId;                  /* system task ID */
   SSTskPrior   tskPrior;               /* system task priority */
   SsDmndQ      dQ;                     /* demand queue */


   /* TAPA task information: the TAPA tasks being handled by
    *  this system task.
    */
   SsIdx        tTsks[SS_MAX_TTSKS];    /* indices into TAPA task table */
   SsCntr       numTTsks;               /* count of TAPA tasks */
   SLockId      lock;                   /* lock for table access */


   SsIdx        nxt;                    /* table implementation */
#ifdef SS_MULTICORE_SUPPORT
   Region       region;
#endif

};


#ifdef __cplusplus
}
#endif

#endif  /*  __SSTASKX__  */


  
/********************************************************************30**

         End of file:     ss_task.x@@/main/3 - Mon Nov 17 15:54:24 2008

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
1.3+      ss029.103   kkj  1. Multiple proc ids support added
/main/3      ---      rp   1. SSI enhancements for Multi-core architecture
                              support
/main/3+  ss001.301   schandra1 1. SSI Ph2 value added feature additions
/main/3+  ss002.301   snatarajan 1. Added call back function pointer
                      pmacharla  1. Removed BIT_64 for thread profile
*********************************************************************91*/
