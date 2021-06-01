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
 
    Type:  
 
    Desc:  
 
    File:  cm_tenb_stats.x 
 
    Sid:      cm_tenb_stats.x@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:53:20 2016
 
    Prg:   sm
 
**********************************************************************/
 
/** @file cm_tenb_stats.x
@brief This file contains .
*/

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*
*/
#ifdef TENB_STATS
 
#include "cm_tenb_stats.h"

#ifndef __TENBSTATSINFX__
#define __TENBSTATSINFX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct tsInfL2CellStats
{
   struct schCellStats{ 
      U32   msg4Fail;
      U32   msg3Fail;
      U32   dlSumCw0Cqi;
      U32   dlNumCw0Cqi;
      U32   dlSumCw1Cqi;
      U32   dlNumCw1Cqi;
      U32   dlSumCw0iTbs;
      U32   dlNumCw0iTbs;
      U32   dlSumCw1iTbs;
      U32   dlNumCw1iTbs;
      U32   riCnt[4]; /* Rank = 1..4 */
      U32   dlNack[2][4];
      U32   dlAckNack[2][4]; /* [CW_CNT][RV_CNT] */
      U32   dlDtx[2][4];
      U32   ulNack[4];
      U32   ulAckNack[4];    /* [RV_CNT] */
      U32   ulDtx[4];
      U32   dlPrbUsage[10]; /* dlPrbUsage[0] to have overall PRB usage */
      U32   ulPrbUsage[10]; /* dlPrbUsage[0] to have overall PRB usage */
      U32   dlPdbRatio[10];
      U32   avgTtiProcTime; /* In us */
      U32   cntTtiProcTime1000; /* Count of instances where TTI proc time was
      greater than 1000us */
      U32   cntTtiProcTime900; /* Count of instances where TTI proc time was
      greater than 900us and less than 1000us */
      U32   cntTtiProcTime800;
      U32   cntTtiProcTime700;
      U32   cntTtiProcTime600;
      U32   cntTtiProcTime500;
      U32   ulSumCqi;
      U32   ulNumCqi;
      U32   ulSumiTbs;
      U32   ulNumiTbs;
      U32   dlTtlTpt;
      U32   ulTtlTpt;
   }sch;
   struct rlcCellStats{ 
      U32   maxRlcSrbRetxFail;
      U32   maxRlcDrbRetxFail;
      U32   reOdrTmrExp;
   }rlc;
}TSInfL2CellStats;

typedef struct tsInfL2UeStats
{
   U32 rnti;
   struct schUeStats{
      U32 dlTxOccns;
      U32 dlRetxOccns;
      U32 dlPrbUsg;
      U32 dlNackCnt[2]; /* [CW_CNT] */
      U32 dlAckNackCnt[2]; /* [CW_CNT] */
      U32 dlDtxCnt[2]; /* [CW_CNT] */
      U32 dlSumCw0Cqi;
      U32 dlNumCw0Cqi;
      U32 dlSumCw1Cqi;
      U32 dlNumCw1Cqi;
      U32 dlSumCw0iTbs;
      U32 dlNumCw0iTbs;
      U32 dlSumCw1iTbs;
      U32 dlNumCw1iTbs;
      U32 cqiDropCnt;
      U32 dlPdbLvl[5]; /* PDB Levels */ 
      U32 riCnt[4]; /* Rank = 1..4 */
      U32 dlBo;
      U32 dlTpt;
      U32 ulTxOccns;
      U32 ulRetxOccns;
      U32 ulPrbUsg;
      U32 ulAckNackCnt;
      U32 ulNackCnt;
      U32 ulDtxCnt;
      U32 ulSumCqi;
      U32 ulNumCqi;
      U32 ulSumiTbs;
      U32 ulNumiTbs;
      U32 ulTpt;
   }sch;
   struct pdcpUeStats{
      U32 dlPdcpDropCnt;
      U32 dlPdcpAckWaitDropCnt;
   }pdcp;
   struct rlcUeStats{
      U32 ulReOdrTmrExpCnt;
      U32 dlMaxPktsInSduQ;
      U32 dlMaxWindowSz;
   }rlc;
}TSInfL2UeStats;

typedef struct tsInfStatsCb
{
   Bool initDone;
   Bool sockSend;
   Bool fileSend;
   Bool consPrint;
   U32  statsPer;
   U32  sockPort;
   Bool startSockSend;
   U8   cmdForFileStats;
   char fileName[100];
   FILE *fptr;
   U32  enbIp;
}TSInfStatsCb;

typedef Void (*TSInfL2UeStatsInd) ARGS((
 Pst *, 
 SuId , 
 TSInfL2UeStats *
));
EXTERN Void TSInfHdlL2UeStatsInd ARGS((
 Pst               *pst,
 SuId              suId,
 TSInfL2UeStats     *stats 
));
EXTERN Void TSInfUnpkL2UeStats ARGS((
 TSInfL2UeStatsInd   func,
 Pst                *pst,
 Buffer             *mBuf
));
typedef Void (*TSInfL2CellStatsInd) ARGS((
 Pst *, 
 SuId , 
 TSInfL2CellStats *
));
EXTERN Void TSInfHdlL2CellStatsInd ARGS((
 Pst                 *pst,
 SuId                suId,
 TSInfL2CellStats    *stats 
));
EXTERN Void TSInfUnpkL2CellStats ARGS((
 TSInfL2CellStatsInd   func,
 Pst                  *pst,
 Buffer               *mBuf
));
EXTERN Void TSInfPkSndL2UeStats ARGS((
 Pst               *pst,
 SuId              suId,
 TSInfL2UeStats     *stats 
));
EXTERN Void TSInfPkSndL2CellStats ARGS((
 Pst                 *pst,
 SuId                suId,
 TSInfL2CellStats     *stats 
));
EXTERN Void TSInfTrigStats ARGS((
 Region    region,
 Pool      pool
));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __TENBSTATSINFX__ */
#endif /* TENB_STATS */

/**********************************************************************
         End of file:     cm_tenb_stats.x@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:53:20 2016
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------

*********************************************************************91*/
