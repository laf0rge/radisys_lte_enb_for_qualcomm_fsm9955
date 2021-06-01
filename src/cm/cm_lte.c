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


/************************************************************************
 
     Name:     Common LTE
  
     Type:     C source file
  
     Desc:     C code for packing/unpacking of cm_lte data structures. 
  
     File:     cm_lte.c 
  
     Sid:      cm_lte.c@@/main/2 - Fri Nov 13 14:09:17 2009
  
     Prg:      sm
  
**********************************************************************/

/* header include files (.h) */

#include "envopt.h"             /* environment options */  
#include "envdep.h"             /* environment dependent */
#include "envind.h"             /* environment independent */

/* header/extern include files (.x) */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_lte.h"             /* common lte header file */
#include "cm5.h"           /* common timers */
#include "ss_queue.h"      /* queues */
#include "ss_task.h"       /* tasking */
#include "ss_msg.h"        /* messaging */
#include "ss_mem.h"        /* memory management interface */
#include "ss_gen.h"        /* general */
/* mt003.301 Additions - Task deregistration */
#include "ss_err.h"        /* error */


/* header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_lte.x"             /* common lte header file */
#include "cm5.x"           /* common timers */
#include "ss_queue.x"      /* queues */
#include "ss_task.x"       /* tasking */
#include "ss_timer.x"      /* timers */
#include "ss_strm.h"
#include "ss_strm.x"       /* STREAMS */
#include "ss_msg.x"        /* messaging */
#include "ss_mem.x"        /* memory management interface */
#include "ss_gen.x"        /* general */


/* public variable declarations */
PUBLIC U32 cmLteTime;


/***********************************************************
*
*     Func : cmPkLteRlcId
*
*
*     Desc : RLC Identifier
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkLteRlcId
(
CmLteRlcId *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkLteRlcId(param, mBuf)
CmLteRlcId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkLteRlcId);

   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(SPkU8, param->rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->rbId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkLteRlcId
*
*
*     Desc : RLC Identifier
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkLteRlcId
(
CmLteRlcId *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLteRlcId(param, mBuf)
CmLteRlcId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkLteRlcId);

   CMCHKUNPK(cmUnpkLteRbId, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->rbType, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkLteTimingInfo
*
*
*     Desc : LTE Timing Info
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkLteTimingInfo
(
CmLteTimingInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkLteTimingInfo(param, mBuf)
CmLteTimingInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkLteTimingInfo);

   CMCHKPK(SPkU8, param->subframe, mBuf);
   CMCHKPK(SPkU16, param->sfn, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkLteTimingInfo
*
*
*     Desc : LTE Timing Info
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkLteTimingInfo
(
CmLteTimingInfo *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLteTimingInfo(param, mBuf)
CmLteTimingInfo *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkLteTimingInfo);

   CMCHKUNPK(SUnpkU16, &param->sfn, mBuf);
   CMCHKUNPK(SUnpkU8, &param->subframe, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmPkLtePdcpId
*
*
*     Desc : PDCP Id
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmPkLtePdcpId
(
CmLtePdcpId *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmPkLtePdcpId(param, mBuf)
CmLtePdcpId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmPkLtePdcpId);

   CMCHKPK(SPkU8, param->rbType, mBuf);
   CMCHKPK(cmPkLteRbId, param->rbId, mBuf);
   CMCHKPK(cmPkLteRnti, param->ueId, mBuf);
   CMCHKPK(cmPkLteCellId, param->cellId, mBuf);
   RETVALUE(ROK);
}



/***********************************************************
*
*     Func : cmUnpkLtePdcpId
*
*
*     Desc : PDCP Id
*
*
*     Ret  : S16
*
*     Notes:
*
*     File  : 
*
**********************************************************/
#ifdef ANSI
PUBLIC S16 cmUnpkLtePdcpId
(
CmLtePdcpId *param,
Buffer *mBuf
)
#else
PUBLIC S16 cmUnpkLtePdcpId(param, mBuf)
CmLtePdcpId *param;
Buffer *mBuf;
#endif
{

   TRC3(cmUnpkLtePdcpId);

   CMCHKUNPK(cmUnpkLteCellId, &param->cellId, mBuf);
   CMCHKUNPK(cmUnpkLteRnti, &param->ueId, mBuf);
   CMCHKUNPK(cmUnpkLteRbId, &param->rbId, mBuf);
   CMCHKUNPK(SUnpkU8, &param->rbType, mBuf);
   RETVALUE(ROK);
}
#ifdef LTE_L2_MEAS 
#ifdef ANSI
PUBLIC S16 cmUpdateSsiMemInfo
(
CmLteMemInfo *mInfo
)
#else
PUBLIC S16 cmUpdateSsiMemInfo(mInfo)
CmLteMemInfo *mInfo;
#endif
{
   U8   numReg = 0;
   U8   numPool = 0;
   U8   idxReg;
   U8   idxPool;
   SsMemDbgInfo dbgInfo;
   SGetRegPoolInfo(&numReg,&numPool); 
   mInfo->numRegions = numReg;

   for(idxReg=0; idxReg< numReg; idxReg++)
   {
      SGetRegInfo(idxReg, &dbgInfo);

      if(mInfo->regInfo[idxReg].isGenMemInfoUpdated == TRUE)
      {
         for(idxPool=0; idxPool<numPool; idxPool++)
         {
            mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed = dbgInfo.bktDbgTbl[idxPool].numAlloc;
            if(mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed > mInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed)
               mInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed = mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed;
         }
      }
      else
      {
      mInfo->regInfo[idxReg].numPools = numPool;
      mInfo->regInfo[idxReg].regionId = idxReg;
      mInfo->regInfo[idxReg].regionType = 0;

      for(idxPool=0; idxPool<numPool; idxPool++)
      {
         mInfo->regInfo[idxReg].poolInfo[idxPool].totAvailable = dbgInfo.bktDbgTbl[idxPool].numBlks;
         mInfo->regInfo[idxReg].poolInfo[idxPool].poolSize = dbgInfo.bktDbgTbl[idxPool].size;
         mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed = dbgInfo.bktDbgTbl[idxPool].numAlloc;
         if(mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed > mInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed)
            mInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed = mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed;
      }
         mInfo->regInfo[idxReg].isGenMemInfoUpdated = TRUE; 
      }
   }
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmFillMemUtilizationMeas
(
 CmLteMemInfo *memoryInfo,
CmLteMemInfo *memInfo
)
#else
PUBLIC S16 cmFillMemUtilizationMeas(memoryInfo,memInfo)
CmLteMemInfo *memoryInfo;
CmLteMemInfo *memInfo;
#endif
{
   U8 idxReg = 0;
   U8 idxPool = 0;
   U8 numPool = 0;

   memoryInfo->numRegions = memInfo->numRegions;
   for(idxReg=0; idxReg < memInfo->numRegions; idxReg++)
   {
      memoryInfo->regInfo[idxReg].regionId = memInfo->regInfo[idxReg].regionId;
      memoryInfo->regInfo[idxReg].numPools = memInfo->regInfo[idxReg].numPools;
      memoryInfo->regInfo[idxReg].regionType = memInfo->regInfo[idxReg].regionType;
      numPool = memoryInfo->regInfo[idxReg].numPools;

      for(idxPool=0; idxPool < numPool; idxPool++)
      {
         memoryInfo->regInfo[idxReg].poolInfo[idxPool].totAvailable = memInfo->regInfo[idxReg].poolInfo[idxPool].totAvailable;
         memoryInfo->regInfo[idxReg].poolInfo[idxPool].poolSize = memInfo->regInfo[idxReg].poolInfo[idxPool].poolSize;
         memoryInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed = memInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed;
         memoryInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed  = memInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed;
      }
   }
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmClearMemUtilizationCounter
(
CmLteMemInfo *memInfo
)
#else
PUBLIC S16 cmClearMemUtilizationCounter(memInfo)
CmLteMemInfo *memInfo;
#endif
{
   U8 idxReg = 0;
   U8 idxPool = 0;
   U8 numPool = 0;
   for(idxReg=0; idxReg < memInfo->numRegions; idxReg++)
   {
      memInfo->regInfo[idxReg].regionId = 0;
      memInfo->regInfo[idxReg].numPools = 0;
      memInfo->regInfo[idxReg].regionType = 0;
      memInfo->regInfo[idxReg].isGenMemInfoUpdated = FALSE;
      numPool = memInfo->regInfo[idxReg].numPools;

      for(idxPool=0; idxPool<numPool; idxPool++)
      {
         memInfo->regInfo[idxReg].poolInfo[idxPool].totAvailable = 0;
         memInfo->regInfo[idxReg].poolInfo[idxPool].poolSize = 0;
         memInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed = 0;
         memInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed  = 0;
      }
   }
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmClearCpuUtilizationCounter
(
CmCpuStatsInfo *cpuInfo
)
#else
PUBLIC S16 cmClearCpuUtilizationCounter(cpuInfo)
CmCpuStatsInfo *cpuInfo
#endif
{
   U8 idx= 0;
   for(idx=0; idx < cpuInfo->numCores; idx++)
   {
      cpuInfo->cpuUtil[idx].maxCpuUtil = 0;
      cpuInfo->cpuUtil[idx].totCpuUtil = 0;
      cpuInfo->cpuUtil[idx].numSamples = 0;
   }
   cpuInfo->numCores = 0;
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 cmFillCpuUtilizationMeas
(
CmLteCpuInfo *cpuMeasInfo,
CmCpuStatsInfo *cpuInfo
)
#else
PUBLIC S16 cmFillCpuUtilizationMeas(cpuMeasInfo,cpuInfo)
CmLteCpuInfo *cpuMeasInfo;
CmCpuStatsInfo *cpuInfo;
#endif
{
   U8 idx= 0;
   cpuMeasInfo->numCores = cpuInfo->numCores;
   for(idx=0; idx < cpuInfo->numCores; idx++)
   {
      cpuMeasInfo->cpuUtil[idx].avgCpuUtil = cpuInfo->cpuUtil[idx].totCpuUtil/cpuInfo->cpuUtil[idx].numSamples;
      cpuMeasInfo->cpuUtil[idx].maxCpuUtil = cpuInfo->cpuUtil[idx].maxCpuUtil;
   }
   RETVALUE(ROK);

}
#endif

#ifdef TENB_TTI_PERF
CmTtiProc ttiProc;
#define TTI_1MS 950

PUBLIC Void cmUpdateTtiCounters(U32 ttiProcessingTime)
{
   ttiProc.numOfTti++;
   ttiProc.totTtiProcessingTime += ttiProcessingTime;
   if(ttiProcessingTime > ttiProc.maxTtiProcessingTime)  
   {
      ttiProc.maxTtiProcessingTime = ttiProcessingTime;
   }
   if(ttiProcessingTime > TTI_THRESHOLD_VALUE)
   {
      ttiProc.ttiThresholdExceedCount++;
   }
   if(ttiProcessingTime > TTI_1MS)
   {
      ttiProc.ttiStretchCount++;
   }
   RETVOID;    
}

PUBLIC Void cmResetTtiCounters(Void)
{
	ttiProc.totTtiProcessingTime = 0;
	ttiProc.numOfTti = 0;
	ttiProc.maxTtiProcessingTime = 0;
	ttiProc.ttiStretchCount = 0;
	ttiProc.ttiThresholdExceedCount = 0;
	ttiProc.phyReptTtiStretchCount = 0;
	RETVOID;
}
#endif

/**********************************************************************
 
         End of file:     cm_lte.c@@/main/2 - Fri Nov 13 14:09:17 2009
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     sm                1. Initial Release.
*********************************************************************91*/
