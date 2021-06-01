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
 
     Name:     MTSS -- implementation specific definitions
 
     Type:     C include file
 
     Desc:     Data structure definitions demanded by systems services.
               The contents of these are for the MTSS implementation.
 
     File:     mt_ss.x
 
     Sid:      mt_ss.x@@/main/2 - Mon Nov 17 15:54:12 2008
 
     Prg:      kp
 
*********************************************************************21*/


#ifndef __MTSSX__
#define __MTSSX__

/* mt001.301: Addition */
#ifdef SS_LOGGER_SUPPORT 
/* #define __USE_POSIX */
#include <limits.h>
#include <netinet/in.h>
#endif /* SS_LOGGER_SUPPORT  */

/* TAPA task entry doesn't need anything extra for MTSS */
typedef S8              SsdTTskEntry;


/* System task entry */
typedef struct ssdSTskEntry
{
   pthread_t            tId;
   Ent                  ent;
   Inst                 inst;
   SLockId              lock;

#ifdef SS_MULTICORE_SUPPORT
   U32                  lwpId;
#endif /* SS_MULTICORE_SUPPORT */
} SsdSTskEntry;


#ifdef SS_DRVR_SUPPORT
/* Driver task entry */
typedef struct ssdDrvrTskEntry
{
   Bool flag;

} SsdDrvrTskEntry;
#endif


/* timer entry--MTSS uses common timers */
typedef struct ssdTmrEntry
{
   CmTimer              timers[TMR_DEF_MAX];

} SsdTmrEntry;


/* dynamic pool doesn't need anything extra for MTSS */
typedef S8              SsdDPoolEntry;


/* static pool -- information for the memory management scheme */
typedef S8              SsdSPoolEntry;


/* generic pool doesn't need anything extra for MTSS */
typedef S8              SsdPoolEntry;


/* region doesn't need anything extra for MTSS */
typedef S8              SsdRegionEntry;


/* system services control point--tick count, timer implementation,
 *   console implementation, IS task implementation
 */
typedef struct ssdOs
{
   unsigned     randSeed;               /* random number generator seed */

   Ticks        sysTicks;               /* elapsed system ticks */

   pthread_t    tmrHdlrTID;             /* timer handler thread */
   CmTqCp       tmrTqCp;                /* common timer control point */
   CmTqType     tmrTq[SS_MAX_TMRS];     /* common timer queue */

   sem_t        ssStarted;              /* posted when SS completes init */
#ifdef CONAVL
   FILE         *conInFp;               /* console input file pointer */
   FILE         *conOutFp;              /* console output file pointer */
   pthread_t    conHdlrTID;             /* console handler thread ID */
#endif

#ifndef NOFILESYS
   FILE         *fileOutFp;             /* output file pointer */
#endif

#ifdef SS_DRVR_SUPPORT
   pthread_t    isTskHdlrTID;           /* IS task handler thread ID */
   int          isFildes[2];            /* pipe for SSetIntPend to isTskHdlr */
#endif
   Bool         sigEvnt;                /*mt010.301 Flag to check interupt signal(SIGINT)*/

} SsdOs;

/* mt018.201 - added for memory configuration */
typedef struct mtBktCfg
{
   Size         blkSize;                /* bucket quantum size */
   U32          numBlks;                /* the total blocks in the bucket */
} MtBktCfg;

typedef struct mtRegCfg
{
   Region       regionId;
   U16          numBkts;
   Size         heapsize;
   MtBktCfg     bkt[SS_MAX_POOLS_PER_REG];
} MtRegCfg;

typedef struct mtMemCfg
{
   U8           numRegions;
   MtRegCfg     region[SS_MAX_REGS];
} MtMemCfg;

/* mt003.301 Readwrite lock additions */
#ifdef SS_LOCK_SUPPORT
typedef struct sLockInfo
{
  union
  {
#ifdef SS_RDWR_LOCK_SUPPORT
   pthread_rwlock_t rdWrLockId;
#endif /* SS_RDWR_LOCK_SUPPORT */
#ifdef SS_REC_LOCK_SUPPORT
	pthread_mutex_t  recurLock;
#endif /* SS_REC_LOCK_SUPPORT */
  }l;
}SLockInfo;
#endif /* SS_LOCK_SUPPORT */
#endif  /*  __MTSSX__  */


  
/********************************************************************30**

         End of file:     mt_ss.x@@/main/2 - Mon Nov 17 15:54:12 2008

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
1.1          ---      kp   1. MTSS-Solaris release 2.1
1.1+       mt018.201 bdu   1. Added some structures for memory configure.
1.1+       mt022.201 bjp   2. Added SRegInfoShow prototype
1.1+       mt033.201 vk    1. Moved SRegInfoShow prototype to ss_mem.x
/main/2      ---        rp   1. SSI enhancements for Multicore support  
/main/2+   mt001.301 schandra1 1. SSI Ph2 value added feature additions
/main/2+   mt003.301 rss       1. Readwrite lock added
/main/2+   mt010.301 ssahniX   1. Fix for core when run with -o option
                                  and when killed with SIGINT
*********************************************************************91*/
 
