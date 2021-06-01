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
 
    Name:   LTE MAC DLFS scheduler
 
    Type:   C include file
 
    Desc:   Defines required by DLFS scheduler
 
    File:   rg_sch_dlfs.x
 
    Sid:      rg_sch_dlfs.x@@/main/2 - Sat Jul 30 02:22:03 2011
 
    Prg:    sd
 
**********************************************************************/
/** @file rg_sch_dlfs.x
@brief This file contains data structures for downlink frequency selective
scheduler */
 

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE-MAC
*
*/
#ifdef TFU_UPGRADE
#ifndef __RGDLFSX__
#define __RGDLFSX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct rgSchDlfsSbCqi RgSchDlfsSbCqi;
typedef struct rgSchDlfsAlloc RgSchDlfsAlloc;
typedef struct rgSchDlfsSb RgSchDlfsHole;


/** 
  @brief Downlink subband information. */
typedef struct rgSchDlfsSb
{
   U8      start;      /*!< Subband start RB index */
   U8      sbSize;     /*!< Subband size */
   U8      sbIdx;      /*!< Subband index */
   U8      numOvrlpPbchRb; /*!< Number of PBCH RBs in this SB */
   Bool    isAvail;    /*!< Bool to indicate if the subband is free or not */
} RgSchDlfsSb;

/** 
  @brief Downlink subband control block. */
typedef struct rgSchDlfsSf
{
   RgSchDlfsSb *dlSb;      /*!< Array of dlSb for cell */
   U8          numAvailSb; /*!< Total number of downlink subbands */
   U8          allocDlBw;  /*!< Allocated DL BW */
   U8          availDlBw;  /*!< Available DL BW */
   U16         numPbchRePerRb; /*!< Number of PBCH REs per RB for this SF */
} RgSchDlfsSf;

/**
  @brief Downlink subband CQI information per UE. */
struct rgSchDlfsSbCqi
{
   RgSchDlfsSbCqi *next;       /*!< Pointer to the next subband CQI */
   RgSchDlfsSbCqi *prev;       /*!< Pointer to the next subband CQI */
   U8             sbIdx;       /*!< Subband index */
   U8             cqi;         /*!< Subband CQI value */
};

/**
  @brief Bandwidth part control block for Mode 2-0 periodic CQI reporting. */
typedef struct rgSchDlfsUeBpCb
{
   U8     sbStartIdx;  /*!< Subband index of the bandwidth part start */
   U8     sbEndIdx;    /*!< Subband index of the bandwidth part end */
} RgSchDlfsUeBpCb;

/**
  @brief UE control block for DLFS scheduling. */
typedef struct rgSchDlfsUeCb
{
   U32             avgSbCqiVal;    /*!< Average CQI computed over the 
                                        subband CQIs, scaled to 1024 multiple */
   U32             avgCqiDeviation;/*!< Average Subband CQI deviation from 
                                        average CQI, scaled to 1024 multiple */
   RgSchDlfsSbCqi  *sbCqiLst;      /*!< Array of Subband CQIs */
   RgSchDlfsSbCqi  *sortSbCqiLst;  /*!< Subband CQIs list arranged in 
                                        descending order of CQI */
   RgSchDlfsUeBpCb *bpCbs;         /*!< Array of bandwidth part control blocks */
   U8              wbCqi;          /*!< Wideband CQI */
   U8              prdSbSize;      /*!< Subband size for periodic reporting */
   U8              aprdSbSize;     /*!< Subband size for aperiodic reporting */
   /* Following values are valid only for periodic reporting mode 2-0*/
   U8              periodicity;    /*!< Periodicity value */
   U8              cqiRptOffst;    /*!< CQI Reporting offset value */
   U8              k;              /*!< k value for periodic reporting */
} RgSchDlfsUeCb;

/**
  @brief Cell control block for DLFS scheduling. */
typedef struct rgSchDlfsCellCb
{
   U8     defaultCqi;     /*!< Cell's default CQI */
   U8     totDlBw;        /*!< Total DL bandwidth */
   U8     numDlSb;        /*!< Total number of DL subbands for configured
                            rbgSize */
   U8     rbgSize;        /*!< RBG size */
   U8     numBp;          /*!< Total number of bandwidth parts */
} RgSchDlfsCellCb;


/* APIs exposed by DLFS */

EXTERN Void rgSCHDlfsInit ARGS((
RgDlfsSchdApis *rgDlfsApis
));

EXTERN S16 rgSCHDlfsCellCfg ARGS((
RgSchCellCb  *cell,
RgrCellCfg   *cellCfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHDlfsCellRecfg ARGS((
RgSchCellCb  *cell,
RgrCellRecfg *cellRecfg,
RgSchErrInfo *err
));
EXTERN Void rgSCHDlfsCellDel ARGS((
RgSchCellCb  *cell
));
EXTERN S16 rgSCHDlfsUeCfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeCfg     *ueCfg,
RgSchErrInfo *err
));
EXTERN S16 rgSCHDlfsUeRecfg ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue,
RgrUeRecfg   *ueRecfg,
RgSchErrInfo *err
));
EXTERN Void rgSCHDlfsUeDel ARGS((
RgSchCellCb  *cell,
RgSchUeCb    *ue
));
EXTERN Void rgSCHDlfsDlCqiIndHndlr ARGS((
RgSchCellCb     *cell,
RgSchUeCb       *ue,
TfuDlCqiRpt     *dlCqiIndInfo,
CmLteTimingInfo timingInfo
));
EXTERN Void rgSCHDlfsReinitSf ARGS((
RgSchCellCb  *cell,
RgSchDlSf    *dlSf
));

/* Added for BCCH/PCCH handling */
EXTERN Void rgSCHDlfsRbAlloc ARGS((
RgSchCellCb  *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));

EXTERN Void rgSCHDlfsBcchPcchRbAlloc ARGS((
RgSchCellCb           *cell,
RgSchCmnDlRbAllocInfo *allocInfo
));
EXTERN Void rgSCHDlfsOdrUeForRbAssgnmt ARGS((
CmLListCp          *lCp, 
RgSchDlHqProcCb    *hqP
));

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGDLFSX__ */
#endif


/**********************************************************************
         End of file:     rg_sch_dlfs.x@@/main/2 - Sat Jul 30 02:22:03 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     sd   1. LTE MAC 2.1 release
             rg004.201  rnt   1. Modified for SI Enhancement
/main/2      ---         gvj 1. Updated for LTE MAC Rel 3.1
$SID$        ---     rt  1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
