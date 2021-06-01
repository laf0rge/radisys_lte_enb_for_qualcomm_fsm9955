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
 
    Name:   LTE MAC Round Robin scheduler
 
    Type:   C include file
 
    Desc:   Defines required by Round Robin scheduler
 
    File:   rg_sch_rr.x
 
    Sid:      rg_sch_rr.x@@/main/2 - Sat Jul 30 02:22:09 2011
 
    Prg:    rr
 
**********************************************************************/
/** @file rg_sch_rr.x
@brief This file contains data structures for the round robin scheduler.
*/
 

/*
*     The defines in this file correspond to structures and variables
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE-MAC
*
*/

#ifndef __RGRRX__
#define __RGRRX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct rgSchRrDlProc
{
   RgSchRrCList           retxLnk; /*!< To link retransmitting HARQ processes in cell
*/
} RgSchRrDlHqProc;

/**
  @brief Downlink Logical channel control block for Round Robin scheduling. */
typedef struct rgSchRrDlLc

{
   RgSchRrCList      lnk;     /*!< To link lc into UE's reported bo list */
   RgSchRrCList      schdLnk; /*!< To link lc into UE's schd list in harq */
   U32               schdBo;  /*!< Scheduled Buffer occupancy */
   U16          rlcHdrEstmt; /*!< RLC Hdr est computed during SVC allocn */
} RgSchRrDlLc;

/**
  @brief UE DL control block for Round Robin scheduling. */
typedef struct rgSchRrDlUe
{
   RgSchRrCList           ueTxLnk;   /*!< To link UE in queue for new tx */
   RgSchRrCList           toBeSchdLnk; /*!<To link to the selected UE list.*/
   RgSchRrCListCp         sigLst;    /*!< List of DCCH services */
   RgSchRrCListCp         dataLst;   /*!< List of DTCH services */
   RgSchRrCListCp         schedLcs;  /*!< LCs scheduled by RR in the TTI*/
   Bool                   schdTa;    /*!< Set if UE's TA is considered in this
                                          scheduling occassion. */
} RgSchRrDlUe;

/**
  @brief UE UL control block for Round Robin scheduling. */
typedef struct rgSchRrUlUe
{
   RgSchRrCList      ueTxLnk;     /*!< To link UE in queue for new tx */
   RgSchRrCList      contResLnk;  /*!< To link UE in contRes queue for tx */
   Bool              srRcvd;
} RgSchRrUlUe;

/**
  @brief Cell DL control block for Round Robin scheduling. */
typedef struct rgSchRrDlCell
{
   RgSchRrCListCp         prioUeTxLst;     /*!< Queue for priority UEs corresponding to new tx*/
   RgSchRrCListCp         normUeTxLst;  /*!< Queue for non priority UEs corresponding to new tx*/
   RgSchRrCListCp         ueRetxLst;  /*!<Queue for UEs corresponding to re-tx*/
   RgSchRrCListCp         toBeSchedLst;/*!<List for storing the selected ues for scheduling */
} RgSchRrDlCell;

/**
  @brief Cell UL control block for Round Robin scheduling. */
typedef struct rgSchRrUlCell
{
   RgSchRrCListCp         contResLst;     /*!< UEs with pending UL alloc for msg4 */
   RgSchRrCListCp         prioUeTxLst;    /*!< Queue for Priority UEs 
                                               corresponding to new tx*/
   RgSchRrCListCp         normUeTxLst; /*!< Queue for non priority 
                                               UEs corresponding to new tx*/
} RgSchRrUlCell;

EXTERN Void rgSCHRrDlCqiInd
ARGS((
RgSchCellCb        *cell,
RgSchUeCb          *ue,
TfuDlCqiRpt        *dlCqiRpt
));

EXTERN Void rgSCHRrDlActvtUe
ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue
));

EXTERN Void rgSCHRrDlUeRefresh
ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));

EXTERN S16 rgSCHRrDlCellCfg
ARGS((
RgSchCellCb          *cell,
RgrCellCfg           *cfg,
RgSchErrInfo     *err
));

EXTERN S16 rgSCHRrDlCellRecfg
ARGS((
RgSchCellCb     *cell,
RgrCellRecfg    *recfg,
RgSchErrInfo     *err
));

EXTERN Void rgSCHRrDlCellDel
ARGS((
RgSchCellCb       *cell
));

EXTERN S16 rgSCHRrDlUeCfg
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeCfg         *cfg,
RgSchErrInfo     *err
));

EXTERN S16 rgSCHRrDlUeRecfg
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeRecfg       *recfg,
RgSchErrInfo     *err
));

EXTERN Void rgSCHRrDlProcAddToRetx
ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));

EXTERN Void rgSCHRrDlUeDel
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));

EXTERN S16 rgSCHRrDlLcCfg
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc,
RgrLchCfg        *cfg,
RgSchErrInfo     *err
));

EXTERN S16 rgSCHRrDlLcRecfg
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc,
RgrLchRecfg      *recfg,
RgSchErrInfo     *err
));

EXTERN Void rgSCHRrDlLcDel
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchDlLcCb      *dlLc
));

EXTERN Void rgSCHRrDlDedBoUpd
ARGS((
RgSchCellCb            *cell,
RgSchUeCb              *ue,
RgSchDlLcCb            *dlLc
));

EXTERN Void rgSCHRrDlUeReset
ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ueCb
));

EXTERN Void rgSCHRrDlSchd
ARGS((
RgSchCellCb             *cell,
RgSchCmnDlRbAllocInfo   *allocInfo
));

EXTERN Void rgSCHRrDlAllocFnlz
ARGS((
RgSchCellCb         *cell,
RgSchCmnDlRbAllocInfo  *allocInfo
));

EXTERN Void rgSCHRrDlInactvtUes
ARGS((
RgSchCellCb        *cell,
CmLListCp          *lst
));

EXTERN Void rgSCHRrDlInit
ARGS((
RgDlSchdApis *rgSchDlApis
));

EXTERN Void rgSCHRrUlCqiInd
ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue,
TfuUlCqiRpt          *ulCqiInfo
));

EXTERN Void rgSCHRrUlActvtUe
ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ue
));

EXTERN Void rgSCHRrUlUeRefresh
ARGS((
RgSchCellCb    *cell,
RgSchUeCb      *ue
));

EXTERN S16 rgSCHRrUlCellCfg
ARGS((
RgSchCellCb          *cell,
RgrCellCfg           *cfg,
RgSchErrInfo         *err
));

EXTERN S16 rgSCHRrUlCellRecfg
ARGS((
RgSchCellCb     *cell,
RgrCellRecfg    *recfg,
RgSchErrInfo    *err
));

EXTERN Void rgSCHRrUlCellDel
ARGS((
RgSchCellCb       *cell
));

EXTERN S16 rgSCHRrUlUeCfg
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeCfg         *cfg,
RgSchErrInfo     *err
));

EXTERN S16 rgSCHRrUlUeRecfg
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrUeRecfg       *recfg,
RgSchErrInfo     *err
));

EXTERN Void rgSCHRrUlUeDel
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));

EXTERN S16 rgSCHRrUlLcgCfg
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *ulLcg,
RgrLcgCfg        *cfg,
RgSchErrInfo     *err
));

EXTERN S16 rgSCHRrUlLcgRecfg
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *ulLcg,
RgrLcgRecfg      *recfg,
RgSchErrInfo     *err
));

EXTERN Void rgSCHRrUlLcgDel
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgSchLcgCb       *ulLcg
));

EXTERN Void rgSCHRrUpdShortBsr
ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchLcgCb          *ulLcg,
U8                  bs
));

EXTERN Void rgSCHRrUpdTruncBsr
ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
RgSchLcgCb          *ulLcg,
U8                  bs
));

EXTERN Void rgSCHRrUpdLongBsr
ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue,
U8                  bsArr[]
));

EXTERN Void rgSCHRrUlUeReset
ARGS((
RgSchCellCb          *cell,
RgSchUeCb            *ueCb
));

EXTERN Void rgSCHRrSrInd
ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));

EXTERN Void rgSCHRrContResUlGrant
ARGS((
RgSchCellCb         *cell,
RgSchUeCb           *ue
));

EXTERN Void rgSCHRrUlSchd
ARGS((
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
));

EXTERN Void rgSCHRrUlAllocFnlz
ARGS((
RgSchCellCb             *cell,
RgSchCmnUlRbAllocInfo   *allocInfo
));

EXTERN Void rgSCHRrUlInactvtUes
ARGS((
RgSchCellCb        *cell,
CmLListCp          *lst
));

EXTERN Void rgSCHRrUlInit
ARGS((
RgUlSchdApis *rgSchUlApis
));

EXTERN S16 rgSCHRrDlUeHqEntInit
ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue
));

PUBLIC S16 rgSCHRrDlUeHqEntDeInit 
ARGS((
RgSchCellCb      *cell,
RgSchDlHqEnt     *hqE
));
/* MS_WORKAROUND for ccpu00122892  : 
 * Temp fix for erroeneous RETX Harq release by rgSCHCmnDlAllocRetxRb */
EXTERN Void rgSCHRrDlRmvProcFrmRetxLst 
ARGS((
RgSchCellCb     *cell, 
RgSchUeCb       *ue,
RgSchDlHqProcCb *hqP));

EXTERN Void rgSCHRrDlProcRmvFrmRetx
ARGS((
RgSchCellCb                *cell,
RgSchDlHqProcCb            *hqP
));

EXTERN void rgSCHRrDlUpdLcPdbMetric
ARGS((
RgSchCellCb *cell
));

EXTERN S16 rgSCHRrUlLchCfg ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLchCfg        *cfg,
RgSchErrInfo     *err
));

EXTERN S16 rgSCHRrUlLchRecfg ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
RgrLchRecfg      *recfg,
RgSchErrInfo     *err
));

EXTERN S16 rgSCHRrUlLchDel ARGS((
RgSchCellCb      *cell,
RgSchUeCb        *ue,
CmLteLcId        lcId,
U8               lcgId
));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGRRX__ */


/**********************************************************************
         End of file:     rg_sch_rr.x@@/main/2 - Sat Jul 30 02:22:09 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     rr   1. LTE MAC 2.1 release
         rg004.201   ms   1. ccpu00110457: DL DTX Handling
         rg004.201   ms   1. Functions and Datatypes starting 
                             with rr are renamed as rgSchRr
           rg007.201   ap  1. Added support for MIMO
           rg008.201 sd    1.Removed dependency on MIMO compile-time flag
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---       rt    1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
