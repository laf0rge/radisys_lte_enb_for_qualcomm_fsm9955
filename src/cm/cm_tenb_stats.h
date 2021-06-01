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
 
    File:  cm_tenb_stats.h
 
    Sid:      cm_tenb_stats.h@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:53:19 2016
 
    Prg:   sm
 
**********************************************************************/
 
/** @file cm_tenb_stats.h
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
#ifndef __TENBSTATSINFH__
#define __TENBSTATSINFH__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TENBSTATSINIT       0x7F
#ifdef BRDCM
#define CL_DSTENT_SELECTOR 0
#define STATS_SRCPROCID 1 
#define STATS_DSTPROCID 1 
#define STATS_DSTENT    ENTPJ
#define STATS_DSTINST   0        /* PDCP UL */
#endif
#ifdef MSPD
#define CL_DSTENT_SELECTOR 0
#ifdef L2_L3_SPLIT
#define STATS_SRCPROCID 1
#define STATS_DSTPROCID 1 
#define STATS_DSTENT    ENTKW
#else
#define STATS_SRCPROCID 0
#define STATS_DSTPROCID 0 
#define STATS_DSTENT    ENTPJ
#endif /* L2_L3_SPLIT */

#define STATS_DSTINST   0        /* PDCP UL */
#endif
#ifdef  LTE_QCOM
#define CL_DSTENT_SELECTOR 0
#define STATS_SRCPROCID 0 
#define STATS_DSTPROCID 0 
#define STATS_DSTENT    ENTPJ
#define STATS_DSTINST   0        /* PDCP UL */
#define EVTTENBL2CELLSTATSIND  0xF3
#define EVTTENBL2UESTATSIND    0xF4

#endif
#define STATS_SRCENT    ENTTF

#define EVTTENBL2CELLSTATS  0xF1
#define EVTTENBL2UESTATS    0xF2

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
#endif /* TENB_STATS */
/**********************************************************************
         End of file:     cm_tenb_stats.h@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/6 - Mon Feb 15 12:53:19 2016
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------

*********************************************************************91*/
