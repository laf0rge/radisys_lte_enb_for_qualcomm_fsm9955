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
 
     Name:     System Services -- Timing
 
     Type:     C include file
 
     Desc:     Data structure definitions required for timing.
 
     File:     ss_timer.x
 
     Sid:      ss_timer.x@@/main/2 - Mon Apr  5 18:01:21 2010
 
     Prg:      kp
 
*********************************************************************21*/


#ifndef __SSTIMERX__
#define __SSTIMERX__

#ifdef __cplusplus
extern "C" {
#endif



/* timer ID and type */
typedef SsIdx           SsTmrId;
/* ss015.301 Enclosed all timer activation functions in a union. */
typedef union {
#ifdef SS_MULTIPLE_PROCS
    PAIFTMRS16        tmrActvFn;
#else
    PAIFTMRS16        tmrActvFnMt;
    PFS16             tmrActvFn;
#endif
}pTmrActvFn;


typedef struct ssTmrActvFn
{
    Bool                 mtFlag;                 /* TRUE if tmrActvFnMt to be used */
    pTmrActvFn           actvFnc;
}SsTmrActvFn;



/* individual entry in the timer table */
typedef struct ssTmrEntry
{
   SsdTmrEntry  dep;                    /* implementation specific */


   Bool         used;                   /* entry is used? */
   SsTmrId      tmrId;                  /* timer ID */
/* ss029.103: addition: procId added */ 
#ifdef SS_MULTIPLE_PROCS
   ProcId       ownerProc;              /* owner task processor ID */
#endif /* SS_MULTIPLE_PROCS */
   Ent          ownerEnt;               /* owner task entity ID */
   Inst         ownerInst;              /* owner task instance ID */
   /* ss028.103 - Modification for SRegCfgTmr support */
   U32          interval;               /* timer interval */
   /* ss029.103: modification: timer function type modified */ 
   /* ss015.301 Enclosed all timer activation functions in a union. */
   SsTmrActvFn  ssTmrActvFn;

   Buffer       *mBuf;                  /* timer message buffer */


   SsIdx        nxt;                    /* table implementation */

} SsTmrEntry;



#ifdef __cplusplus
}
#endif

#endif  /*  __SSTIMERX__  */


  
/********************************************************************30**

         End of file:     ss_timer.x@@/main/2 - Mon Apr  5 18:01:21 2010

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
1.2+       ss028.103  bjp 1. Modification for SRegCfgTmr support
1.3+       ss029.103  kkj  1. Multiple proc ids support added
1.3+       ss030.103  apany 1. New timer activation function for
                             multithreading support.
/main/3      ---      rp   1. SSI enhancements for Multi-core architecture
                              support
/main/3    ss015.301  akapoor  1. Enclosed all timer activation functions in
                                  a union.
*********************************************************************91*/
