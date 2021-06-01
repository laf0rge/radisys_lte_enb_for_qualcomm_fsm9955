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



/**********************************************************************
 
    Name:  
 
    Type:   C include file
 
    Desc:   
 
    File:  cm_tenb_stats.c
 
    Sid:      cm_tenb_stats.c@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:53:18 2016
 
    Prg:   sm
 
**********************************************************************/
 
/** @file cm_tenb_stats.c
@brief This file contains 
*/

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*
*/
/* header include files (.h) */
  
#include "envopt.h"             /* Environment options */  
#include "envdep.h"             /* Environment dependent */
#include "envind.h"             /* Environment independent */
#include "gen.h"                /* General */
#include "ssi.h"                /* System services interface */
#include "cm_lte.h"             /* Common LTE header file */
#include "cm_llist.h"           /* Linked list header file */
#include "cm_tkns.h"            /* Common tokens header file */


/* header/extern include files (.x) */

#include "gen.x"                /* General */
#include "ssi.x"                /* System services interface */
#include "cm_lib.x"             /* Common lib header file */
#include "cm_llist.x"           /* Linked list header file */
#include "cm_tkns.x"            /* Common tokens header file */
#include "cm_lte.x"             /* Common LTE header file */

#include "ss_queue.h"
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"
#include "cm_tenb_stats.x"    /* Total EnodeB Stats declarations */

#ifdef TENB_STATS
PRIVATE Void TSInfUtlPackUeInfo ARGS((
 Buffer            *mBuf,
 TSInfL2UeStats     *stats 
));
PRIVATE Void TSInfUtlPackCellInfo ARGS((
 Buffer              *mBuf,
 TSInfL2CellStats     *stats 
));
PRIVATE Void TSInfUtlUnpkUeInfo ARGS((
 Buffer            *mBuf,
 TSInfL2UeStats     *stats 
));
PRIVATE Void TSInfUtlUnpkCellInfo ARGS((
 Buffer              *mBuf,
 TSInfL2CellStats     *stats
));
PRIVATE Buffer* TSInfUtlAllocMsg ARGS((
 Pst   *pst
));
 
/*
*
*       Fun:   TSInfTrigL2Stats 
*
*       Desc:  Indicate PDCP UL to initiate Stats procedure 
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PRIVATE Void TSInfTrigL2Stats
(
 Region    region,
 Pool      pool
)
#else
PRIVATE Void TSInfTrigL2Stats(region, pool)
 Region    region;
 Pool      pool;
#endif
{
   Buffer* pBuf;
   Pst pst = {0};

   TRC2(TSInfTrigL2Stats)

   SGetMsg(region, pool, &pBuf);
//#if defined(SCH_STATS) || defined(TENB_STATS)
   pst.dstEnt = STATS_DSTENT;
   pst.dstInst = STATS_DSTINST;
   pst.selector = CL_DSTENT_SELECTOR;

   pst.srcProcId = STATS_SRCPROCID;
   pst.dstProcId = STATS_DSTPROCID;
   pst.srcEnt = STATS_SRCENT;
   pst.event = TENBSTATSINIT; 
   SPstTsk(&pst, pBuf);

   RETVOID;
}

/*
*
*       Fun:   TSInfTrigStats
*
*       Desc:  Trigger Sending L2 Stats
*
*       Ret:   
*
*       Notes: None
*
*
*/
#ifdef ANSI
PUBLIC Void TSInfTrigStats
(
 Region    region,
 Pool      pool
)
#else
PUBLIC Void TSInfTrigStats(region, pool)
 Region    region;
 Pool      pool;
#endif
{
   TRC2(TSInfTrigStats)
//TODO
   TSInfTrigL2Stats(region, pool);

   RETVOID;
}

         
#ifdef ANSI
PRIVATE Buffer* TSInfUtlAllocMsg
(
 Pst   *pst
)
#else
PRIVATE Buffer* TSInfUtlAllocMsg(pst)
 Pst   *pst;
#endif
{
   Buffer *mBuf; 

   TRC2(TSInfUtlAllocMsg)

   if (SGetMsg(pst->region, pst->pool, &mBuf) != ROK) {
      printf("\n MBuf Allocation failed\n");
   }
   RETVALUE(mBuf);
}

#ifdef ANSI
PRIVATE Void TSInfUtlPackUeInfo
(
 Buffer            *mBuf,
 TSInfL2UeStats     *stats
)
#else
PRIVATE Void TSInfUtlPackUeInfo(mBuf, stats)
 Buffer            *mBuf;
 TSInfL2UeStats     *stats; 
#endif
{
   S32 i;

   TRC2(TSInfUtlPackUeInfo)

   CMCHKPK(SPkU32, stats->rlc.dlMaxWindowSz, mBuf);
   CMCHKPK(SPkU32, stats->rlc.dlMaxPktsInSduQ, mBuf);
   CMCHKPK(SPkU32, stats->rlc.ulReOdrTmrExpCnt, mBuf);

   CMCHKPK(SPkU32, stats->pdcp.dlPdcpAckWaitDropCnt, mBuf);
   CMCHKPK(SPkU32, stats->pdcp.dlPdcpDropCnt, mBuf);

   CMCHKPK(SPkU32, stats->sch.ulTpt, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulNumiTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulSumiTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulNumCqi, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulSumCqi, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulDtxCnt, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulNackCnt, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulAckNackCnt, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulPrbUsg, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulRetxOccns, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulTxOccns, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlTpt, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlBo, mBuf);
   for (i=0; i<4; i++)
   {
      CMCHKPK(SPkU32, stats->sch.riCnt[i], mBuf);
   }
   for (i=0; i<5; i++)
   {
      CMCHKPK(SPkU32, stats->sch.dlPdbLvl[i], mBuf);
   }
   CMCHKPK(SPkU32, stats->sch.cqiDropCnt, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlNumCw1iTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlSumCw1iTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlNumCw0iTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlSumCw0iTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlNumCw1Cqi, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlSumCw1Cqi, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlNumCw0Cqi, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlSumCw0Cqi, mBuf);
   for (i=0; i<2; i++)
   {
      CMCHKPK(SPkU32, stats->sch.dlDtxCnt[i], mBuf);
   }
   for (i=0; i<2; i++)
   {
      CMCHKPK(SPkU32, stats->sch.dlAckNackCnt[i], mBuf);
   }
   for (i=0; i<2; i++)
   {
      CMCHKPK(SPkU32, stats->sch.dlNackCnt[i], mBuf);
   }
   CMCHKPK(SPkU32, stats->sch.dlPrbUsg, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlRetxOccns, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlTxOccns, mBuf);

   CMCHKPK(SPkU32, stats->rnti, mBuf);
   RETVOID;
}

#ifdef ANSI
PRIVATE Void TSInfUtlPackCellInfo
(
 Buffer              *mBuf,
 TSInfL2CellStats     *stats  
)
#else
PRIVATE Void TSInfUtlPackCellInfo(mBuf, stats)
 Buffer              *mBuf;
 TSInfL2CellStats     *stats; 
#endif
{
   S32 i,j;

   TRC2(TSInfUtlPackCellInfo)

   CMCHKPK(SPkU32, stats->rlc.reOdrTmrExp, mBuf);
   CMCHKPK(SPkU32, stats->rlc.maxRlcDrbRetxFail, mBuf);
   CMCHKPK(SPkU32, stats->rlc.maxRlcSrbRetxFail, mBuf);

   CMCHKPK(SPkU32, stats->sch.ulTtlTpt, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlTtlTpt, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulNumiTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulSumiTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulNumCqi, mBuf);
   CMCHKPK(SPkU32, stats->sch.ulSumCqi, mBuf);
   CMCHKPK(SPkU32, stats->sch.cntTtiProcTime500, mBuf);
   CMCHKPK(SPkU32, stats->sch.cntTtiProcTime600, mBuf);
   CMCHKPK(SPkU32, stats->sch.cntTtiProcTime700, mBuf);
   CMCHKPK(SPkU32, stats->sch.cntTtiProcTime800, mBuf);
   CMCHKPK(SPkU32, stats->sch.cntTtiProcTime900, mBuf);
   CMCHKPK(SPkU32, stats->sch.cntTtiProcTime1000, mBuf);
   CMCHKPK(SPkU32, stats->sch.avgTtiProcTime, mBuf);
   for (i=9; i>=0; i--)
   {
      CMCHKPK(SPkU32, stats->sch.dlPdbRatio[i], mBuf);
   }
   for (i=9; i>=0; i--)
   {
      CMCHKPK(SPkU32, stats->sch.ulPrbUsage[i], mBuf);
   }
   for (i=9; i>=0; i--)
   {
      CMCHKPK(SPkU32, stats->sch.dlPrbUsage[i], mBuf);
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(SPkU32, stats->sch.ulDtx[i], mBuf);
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(SPkU32, stats->sch.ulAckNack[i], mBuf);
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(SPkU32, stats->sch.ulNack[i], mBuf);
   }
   for (i=1; i>=0; i--)
   {
      for (j=3; j>=0; j--)
      {
         CMCHKPK(SPkU32, stats->sch.dlDtx[i][j], mBuf);
      }
   }
   for (i=1; i>=0; i--)
   {
      for (j=3; j>=0; j--)
      {
         CMCHKPK(SPkU32, stats->sch.dlAckNack[i][j], mBuf);
      }
   }
   for (i=1; i>=0; i--)
   {
      for (j=3; j>=0; j--)
      {
         CMCHKPK(SPkU32, stats->sch.dlNack[i][j], mBuf);
      }
   }
   for (i=3; i>=0; i--)
   {
      CMCHKPK(SPkU32, stats->sch.riCnt[i], mBuf);
   }
   CMCHKPK(SPkU32, stats->sch.dlNumCw1iTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlSumCw1iTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlNumCw0iTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlSumCw0iTbs, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlNumCw1Cqi, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlSumCw1Cqi, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlNumCw0Cqi, mBuf);
   CMCHKPK(SPkU32, stats->sch.dlSumCw0Cqi, mBuf);
   CMCHKPK(SPkU32, stats->sch.msg3Fail, mBuf);
   CMCHKPK(SPkU32, stats->sch.msg4Fail, mBuf);

   RETVOID;
}

#ifdef ANSI
PRIVATE Void TSInfUtlUnpkUeInfo
(
 Buffer            *mBuf,
 TSInfL2UeStats     *stats 
)
#else
PRIVATE Void TSInfUtlUnpkUeInfo(mBuf, stats)
 Buffer            *mBuf;
 TSInfL2UeStats     *stats; 
#endif
{
   S32 i;

   TRC2(TSInfUtlUnpkUeInfo)

   CMCHKUNPK(SUnpkU32, &stats->rnti, mBuf);

   CMCHKUNPK(SUnpkU32, &stats->sch.dlTxOccns, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlRetxOccns, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlPrbUsg, mBuf);
   for (i=1; i>=0; i--)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.dlNackCnt[i], mBuf);
   }
   for (i=1; i>=0; i--)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.dlAckNackCnt[i], mBuf);
   }
   for (i=1; i>=0; i--)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.dlDtxCnt[i], mBuf);
   }
   CMCHKUNPK(SUnpkU32, &stats->sch.dlSumCw0Cqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlNumCw0Cqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlSumCw1Cqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlNumCw1Cqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlSumCw0iTbs, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlNumCw0iTbs, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlSumCw1iTbs, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlNumCw1iTbs, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.cqiDropCnt, mBuf);
   for (i=4; i>=0; i--)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.dlPdbLvl[i], mBuf);
   }
   for (i=3; i>=0; i--)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.riCnt[i], mBuf);
   }
   CMCHKUNPK(SUnpkU32, &stats->sch.dlBo, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlTpt, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulTxOccns, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulRetxOccns, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulPrbUsg, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulAckNackCnt, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulNackCnt, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulDtxCnt, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulSumCqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulNumCqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulSumiTbs, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulNumiTbs, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulTpt, mBuf);

   CMCHKUNPK(SUnpkU32, &stats->pdcp.dlPdcpDropCnt, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->pdcp.dlPdcpAckWaitDropCnt, mBuf);

   CMCHKUNPK(SUnpkU32, &stats->rlc.ulReOdrTmrExpCnt, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->rlc.dlMaxPktsInSduQ, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->rlc.dlMaxWindowSz, mBuf);

   RETVOID;
}

#ifdef ANSI
PRIVATE Void TSInfUtlUnpkCellInfo
(
 Buffer              *mBuf,
 TSInfL2CellStats     *stats 
)
#else
PRIVATE Void TSInfUtlUnpkCellInfo(mBuf, stats)
 Buffer              *mBuf;
 TSInfL2CellStats     *stats; 
#endif
{
   S32 i,j;

   TRC2(TSInfUtlUnpkCellInfo)

   CMCHKUNPK(SUnpkU32, &stats->sch.msg4Fail, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.msg3Fail, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlSumCw0Cqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlNumCw0Cqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlSumCw1Cqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlNumCw1Cqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlSumCw0iTbs, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlNumCw0iTbs, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlSumCw1iTbs, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlNumCw1iTbs, mBuf);
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.riCnt[i], mBuf);
   }
   for (i=0; i<2; i++)
   {
      for (j=0; j<4; j++)
      {
         CMCHKUNPK(SUnpkU32, &stats->sch.dlNack[i][j], mBuf);
      }
   }
   for (i=0; i<2; i++)
   {
      for (j=0; j<4; j++)
      {
         CMCHKUNPK(SUnpkU32, &stats->sch.dlAckNack[i][j], mBuf);
      }
   }
   for (i=0; i<2; i++)
   {
      for (j=0; j<4; j++)
      {
         CMCHKUNPK(SUnpkU32, &stats->sch.dlDtx[i][j], mBuf);
      }
   }
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.ulNack[i], mBuf);
   }
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.ulAckNack[i], mBuf);
   }
   for (i=0; i<4; i++)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.ulDtx[i], mBuf);
   }
   for (i=0; i<10; i++)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.dlPrbUsage[i], mBuf);
   }
   for (i=0; i<10; i++)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.ulPrbUsage[i], mBuf);
   }
   for (i=0; i<10; i++)
   {
      CMCHKUNPK(SUnpkU32, &stats->sch.dlPdbRatio[i], mBuf);
   }
   CMCHKUNPK(SUnpkU32, &stats->sch.avgTtiProcTime, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.cntTtiProcTime1000, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.cntTtiProcTime900, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.cntTtiProcTime800, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.cntTtiProcTime700, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.cntTtiProcTime600, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.cntTtiProcTime500, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulSumCqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulNumCqi, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulSumiTbs, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulNumiTbs, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.dlTtlTpt, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->sch.ulTtlTpt, mBuf);

   CMCHKUNPK(SUnpkU32, &stats->rlc.maxRlcSrbRetxFail, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->rlc.maxRlcDrbRetxFail, mBuf);
   CMCHKUNPK(SUnpkU32, &stats->rlc.reOdrTmrExp, mBuf);

   RETVOID;
}

#ifdef ANSI
PUBLIC Void TSInfPkSndL2UeStats
(
 Pst               *pst,
 SuId              suId,
 TSInfL2UeStats     *stats  
)
#else
PUBLIC Void TSInfPkSndL2UeStats(pst, suId, stats)
 Pst               *pst;
 SuId              suId;
 TSInfL2UeStats     *stats;
#endif
{
   Buffer *mBuf;

   TRC2(TSInfPkSndL2UeStats)

   mBuf = TSInfUtlAllocMsg(pst);
   TSInfUtlPackUeInfo(mBuf, stats);
   SPkS16(suId, mBuf);
   pst->event = (Event) EVTTENBL2UESTATS;
   SPstTsk(pst, mBuf);
   RETVOID;
}

#ifdef ANSI
PUBLIC Void TSInfUnpkL2UeStats
(
 TSInfL2UeStatsInd   func,
 Pst                *pst,
 Buffer             *mBuf
)
#else
PUBLIC Void TSInfUnpkL2UeStats(func, pst, mBuf)
 TSInfL2UeStatsInd   func;
 Pst                *pst;
 Buffer             *mBuf;
#endif
{
   SuId              suId;
   TSInfL2UeStats     stats; 

   TRC2(TSInfUnpkL2UeStats)

   SUnpkS16(&suId, mBuf);
   TSInfUtlUnpkUeInfo(mBuf, &stats);
   SPutMsg(mBuf);
   (*func)(pst, suId, &stats);
   RETVOID;
}

#ifdef ANSI
PUBLIC Void TSInfPkSndL2CellStats
(
 Pst                 *pst,
 SuId                suId,
 TSInfL2CellStats   *stats 
)
#else
PUBLIC Void TSInfPkSndL2CellStats(pst, suId, stats)
 Pst                 *pst;
 SuId                suId;
 TSInfL2Cellstats   *stats;
#endif
{
   Buffer *mBuf;

   TRC2(TSInfPkSndL2CellStats)

   mBuf = TSInfUtlAllocMsg(pst);
   TSInfUtlPackCellInfo(mBuf, stats);
   SPkS16(suId, mBuf);
   pst->event = (Event) EVTTENBL2CELLSTATS;
   SPstTsk(pst, mBuf);
   RETVOID;
}

#ifdef ANSI
PUBLIC Void TSInfUnpkL2CellStats
(
 TSInfL2CellStatsInd   func,
 Pst                *pst,
 Buffer             *mBuf
)
#else
PUBLIC Void TSInfUnpkL2CellStats(func, pst, mBuf)
 TSInfL2CellStatsInd   func;
 Pst                *pst;
 Buffer             *mBuf;
#endif
{
   SuId                suId;
   TSInfL2CellStats    stats; 

   TRC2(TSInfUnpkL2CellStats)

   SUnpkS16(&suId, mBuf);
   TSInfUtlUnpkCellInfo(mBuf, &stats);
   SPutMsg(mBuf);
   (*func)(pst, suId, &stats);
   RETVOID;
}
#endif /* TENB_STATS */
/**********************************************************************
         End of file:     cm_tenb_stats.c@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:53:18 2016
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------

*********************************************************************91*/
