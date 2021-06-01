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
 
     Desc:     Data structure definitions required for the memory
               management interface.
 
     File:     ss_mem.x
 
     Sid:      ss_mem.x@@/main/3 - Mon Nov 17 15:54:18 2008
 
     Prg:      kp
 
*********************************************************************21*/


#ifndef __SSMEMX__
#define __SSMEMX__

#ifdef __cplusplus
extern "C" {
#endif


/* region entry structure */
typedef struct ssRegionEntry
{
   SsdRegionEntry dep;                          /* implementation specific */


   Bool         used;                           /* entry used? */

   Data         *start;                         /* start address */
   Size         size;                           /* size */

   Void         *regCb;                         /* control block */

   U32          flags;                          /* flags */

   SsPoolEntry  poolTbl[SS_MAX_POOLS_PER_REG];  /* pool table */
   SsCntr       numPools;                       /* count of pools */

   SsAlloc      alloc;                          /* allocator function */
   SsFree       free;                           /* de-allocator function */
   SsCtl        ctl;                            /* ctl function */
	/* ss006.301 */
	SLockId      mBufRefLock;                    /* per region lock used for 
	                                               data block sharing */
} SsRegionEntry;

/* ss036.103 - addition of data type to return the memory statistics data
* this is used in SGetRegInfo 
*/
typedef struct ssMemBktDbgInfo
{
   Size         size;         /* Size of the block */
   U32          numBlks;      /* Total number of blocks in the bucket */
   U32          numAlloc;     /* Number of blocks allocated */
}SsMemBktDbgInfo;

typedef struct ssMemDbgInfo
{
   Region       region;        /* Region  Id of the memory */
   U16          numBkts;       /* Number of buckets in bktDbgTbl */
   SsMemBktDbgInfo      bktDbgTbl[SS_MAX_BKT_PER_DBGTBL];
   Size         heapSize;      /* Size of the heap pool */
   Size         heapAlloc;          /* Total allocated memory */
   U32          availmem;
#if (ERRCLASS & ERRCLS_DEBUG)
   U16       numFragBlk;       /* Number of fragmented block */
#endif /* ERRCLASS & ERRCLS_DEBUG */
}SsMemDbgInfo;

/* ss036.103 - Addition of  prototypes for memory statistics */
EXTERN S16 SRegInfoShow  ARGS((Region region, U32 *availmem));
EXTERN S16 SGetRegInfo ARGS((Region region, SsMemDbgInfo *dbgInfo));
#ifdef SSI_DEBUG_LEVEL1
EXTERN S16 SPrintRegMemStatusInfo ARGS((Region region, U8 typeFlag));
EXTERN Void SRegMemErrHdlr ARGS((Region region, Data *ptr, S16 errCode));
EXTERN S16 SPrintRegMemProfile ARGS((Region region));
#endif /* SSI_DEBUG_LEVEL1 */

/* ss001.301: additions */
#ifdef SS_HISTOGRAM_SUPPORT 
EXTERN S16 SGetTapaTskEntIds ARGS((Ent *ent));
EXTERN S16 SGetHstGrmInfo ARGS((Ent *entId, Bool *hstReg));
#endif /* SS_HISTOGRAM_SUPPORT */

#ifdef __cplusplus
}
#endif

#endif  /*  __SSMEMX__  */


  
/********************************************************************30**

         End of file:     ss_mem.x@@/main/3 - Mon Nov 17 15:54:18 2008

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
1.2+       ss036.103  vk   1. Addition of data type and prototype for memory statistics.
/main/3      ---      rp   1. SSI enhancements for Multi-core architecture
                              support
/main/3+   ss001.301  schandra1 1. SSI Ph2 value added feature additions
/main/3+   ss006.301  vsrao     1. Added per region lock for dblk sharing
*********************************************************************91*/
