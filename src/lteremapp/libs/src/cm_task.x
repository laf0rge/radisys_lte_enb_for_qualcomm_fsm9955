/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
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
 
     Name:     Common Task Manager 
 
     Type:     C include file
 
     Desc:     Data structure definitions that are used by 
               the Common Task Manager.
 
     File:     cm_task.x
 
     Sid:      cm_task.x@@/main/tenb_main_ccb/1 - Wed Jul 22 19:00:23 2015
 
     Prg:      rp
 
*********************************************************************21*/

#ifndef __CMTSKX_
#define __CMTSKX_

#ifdef __cplusplus
extern "C" {
#endif



/* typedefs */

#ifdef SS_MULTIPLE_PROCS
typedef S16 (*FNACTVINIT)(ProcId proc, Ent ent, Inst inst, Region reg, Reason res);
#else
typedef S16 (*FNACTVINIT)(Ent ent, Inst inst, Region reg, Reason res);
#endif /* SS_MULTIPLE_PROCS */
typedef S16 (*FNACTVTSK)(Pst *pst, Buffer *mBuf);
 
typedef struct cmTskTh2C
{
   U32      coreId;
   U32      numTh;
   U32      thId[CM_TSK_MAX_TH_INFO];
}CmTskTh2C;

typedef struct cmTskInfo
{
   Ent       entId;
   Inst      inst;
   Prior     prio;
#ifdef SS_MULTIPLE_PROCS
	ProcId    proc;
#endif /* SS_MULTIPLE_PROCS */
   FNACTVINIT fnActvInit;
   FNACTVTSK fnActvTsk;
}CmTskInfo;

typedef struct cmTskTsk2Th
{
   U32         thId;
   U32          numTsk;
   CmTskInfo   tskInfo[CM_TSK_MAX_TSK_INFO];
}CmTskTsk2Th;

typedef struct cmTskRd
{
   U8        state;
   U32       numTh;
   U32        numCore;
   CmTskTh2C    th2C[CM_TSK_MAX_CORE_INFO];
   CmTskTsk2Th  tsk2Th[CM_TSK_MAX_TH_INFO];
}CmTskRd;


typedef struct cmTskStrEntry
{
   void    *str;
   Ent   entId;
   FNACTVINIT   fnActvInit;
   FNACTVTSK   fnActvTsk;
}CmTskStrEntry;

/* forward definitions */


/* functions prototypes */
/* cm_task_x_001.main_1 Compilation fixes */ 
/* cm_task_x_001.main_2 : Fixed Compilation warnings  32/64 bit*/ 
#ifdef SS_FBSED_TSK_REG
PUBLIC S16 cmCfgrTskReg   ARGS((U8 *filePath));
#endif /*SS_FBSED_TSK_REG*/


#ifdef __cplusplus
}
#endif

#endif

/********************************************************************30**
 
         End of file:     cm_task.x@@/main/tenb_main_ccb/1 - Wed Jul 22 19:00:23 2015
 
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
1.1          ---      rp   1. initial release
/main/2      --- cm_task_x_001.main_1  pmacharla 1. Fixed compilation warnings
/main/3      --- cm_task_x_001.main_2  ssahniX   1. Fixed Compilation warnings 
                                                    32/64 bit
*********************************************************************91*/
