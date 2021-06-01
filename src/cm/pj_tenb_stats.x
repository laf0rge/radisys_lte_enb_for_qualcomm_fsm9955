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
 
    File:  pj_tenb_stats.x 
 
    Sid:      pj_tenb_stats.x@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:53:15 2016
 
    Prg:   sm
 
**********************************************************************/
 
/** @file pj_tenb_stats.x
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
#ifndef __TENBSTATSL2X__
#define __TENBSTATSL2X__

#include "cm_tenb_stats.x"
#include "pj_tenb_stats.h"
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef TSInfL2CellStats TSL2CellStatsCb;

typedef struct tSL2UeStatsCb
{
   CmLList         lnk;
   U32             inUse;

   TSInfL2UeStats  stats;
}TSL2UeStatsCb;

EXTERN Void TSL2AllocStatsMem ARGS((
 Region region,
 Pool   pool
));
EXTERN TSL2UeStatsCb* TSL2AllocUeStatsBlk ARGS((
 U16   rnti
));
EXTERN TSL2UeStatsCb* TSL2MapUeStatsBlk ARGS((
 U16   rnti
));
EXTERN Void TSL2DeallocUeStatsBlk ARGS((
 U16         rnti,
 TSL2UeStatsCb   *statsCb
));
EXTERN TSL2CellStatsCb* TSL2AllocCellStatsBlk ARGS((
 U32 cellId
));
EXTERN Void TSL2DeallocCellStatsBlk ARGS((
 U32 cellId
));
EXTERN Void TSL2SendStatsToApp ARGS((
 Pst    *pst,
 SuId   suId
));
EXTERN TSL2UeStatsCb* TSL2GetUeStatsBlk ARGS((
 U16   rnti
));
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __TENBSTATSL2X__ */
#endif /* TENB_STATS */

/**********************************************************************
         End of file:     pj_tenb_stats.x@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:53:15 2016
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------

*********************************************************************91*/