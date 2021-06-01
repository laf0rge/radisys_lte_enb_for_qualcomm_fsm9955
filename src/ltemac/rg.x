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
 
    Name:   LTE MAC layer
 
    Type:   C include file
 
    Desc:   Defines required by LTE MAC
 
    File:  rg.x 
 
    Sid:      rg.x@@/main/3 - Sat Jul 30 02:22:13 2011
 
    Prg:   sd
 
**********************************************************************/
 
/** @file rg.x
@brief This file contains basic data structures for the Mac.
*/

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000372       LTE MAC
*
*/
 
#ifndef __RGX__
#define __RGX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @brief Logical Channel Id */
typedef U8                 LteLcgId;        
typedef RguDDatReqInfo     RgRguDedDatReq;
typedef RguCDatReqInfo     RgRguCmnDatReq;
typedef RguDStaRspInfo     RgRguDedStaRsp;
typedef RguCStaRspInfo     RgRguCmnStaRsp;
typedef RguDStaIndInfo     RgRguDedStaInd;
typedef RguCStaIndInfo     RgRguCmnStaInd;
typedef RguDDatIndInfo     RgRguDedDatInd;
typedef RguCDatIndInfo     RgRguCmnDatInd;
typedef RguLchStaInd       RgRguLcStaInd;
typedef TfuPdschDciInfo    RgTfuPdschDciInfo;
typedef TfuDatReqInfo      RgTfuDatReqInfo;
typedef TfuDatReqPduInfo   RgTfuDatReqPduInfo;
typedef RguDDatReqPerUe    RgRguDDatReqPerUe;
#ifdef L2_OPTMZ
typedef TfuDatReqTbInfo    RgTfuDatReqTbInfo;
#endif

#ifdef LTE_L2_MEAS
typedef struct rgL2MeasCb     RgL2MeasCb;
typedef struct rgQciCb        RgQciCb;

typedef RguL2MUlThrpMeasReqInfo RgRguL2MUlThrpMeasReq;

#endif /* LTE_L2_MEAS */
/* Forward declarations for some structures */
typedef struct rgUeCb         RgUeCb;
typedef struct rgErrInfo      RgErrInfo;
typedef struct rgCellCb       RgCellCb;
typedef struct rgUlHqProcCb   RgUlHqProcCb;
typedef struct rgDlHqProcCb   RgDlHqProcCb;
typedef struct rgLcgCb        RgLcgCb;
typedef struct rgDlHqEnt      RgDlHqEnt;
typedef struct _rgCb           RgCb;
typedef struct rgUlLcCb       RgUlLcCb;
typedef struct rgDlLcCb       RgDlLcCb;

EXTERN U8 rgRguDlSap;
EXTERN U8 rgRguUlSap;
/* Declaration for RgCb */
EXTERN RgCb rgCb;

#ifdef LTE_L2_MEAS
/* @brief UE Specific Uplink allocation info needed for measurements */
typedef struct rgUeUlAlloc
{
   CmLteRnti        rnti;      /*!< Ue Id */ 
   U8               numPrb;    /*!< Number of total PRB's allocated for this UE */
} RgUeUlAlloc;
/**
 * @brief Structure hold uplink allocation information for sub frames.
 * */
typedef struct rgUlSf
{
   CmLteTimingInfo schdTime;       /*!< scheduled frame and subframe number */
   U8              numUe;          /*!< Number of UE's allocated in this uplink
                                      subframe */
   RgUeUlAlloc     *ueUlAllocInfo; /*!< Uplink alloc infor for this subframe */
} RgUlSf;

/** @brief
 *  L2 measurement control block.
 *  */
struct rgL2MeasCb
{
   CmLList            measLnk;        /*!< MeasCb node */
   RgInfL2MeasReq     measReq;        /*!< Request received for measurement */
   CmLteTimingInfo    startTime;      /*!< Time at which measurement started */
   U32                sfnCycle;       /*!< Count of Num of SFN wraps */ 
};
/** @brief
 *  Control block to store Logical Channels with Qci.
 *  */
struct rgQciCb
{
   U8               qci;      /*< QCI for measurement */
   U32              prbCount; /*!< Cummulative PRB count */
   U8               mask;     /*!< Measurement Active or Not*/
   CmLteTimingInfo  startTime;/*!< Time at which measurement started */
};
#endif /* LTE_L2_MEAS */
/**
  * @brief
  * Information about one PDCCH.
  */
typedef struct rgPdcch {
   CmLteRnti         rnti;    /*!< RNTI to who the PDCCH is allocated */
   RgTfuPdschDciInfo dci;     /*!< PDCCH format */
} RgPdcch;

typedef struct rgBcchTb
{
   RgPdcch        pdcch;
   Buffer         *tb;
   U16            txPwrOffset; /*!< PDSCH Tx Pwr offset */
} RgBcchTb;

typedef struct rgPcchTb
{
   RgPdcch        pdcch;
   Buffer         *tb;
   U16            txPwrOffset; /*!< PDSCH Tx Pwr offset */
} RgPcchTb;

typedef struct rgRaRspAlloc
{
   RgPdcch        pdcch;     /*!< NULLP if no Rsp allocation done for raRnti*/
   Buffer         *rar;
   U16            txPwrOffset; /*!< PDSCH Tx Pwr offset */
}RgRaRspAlloc;

typedef struct rgBchTb
{
   Buffer         *tb;    /*!< BCH data for this frame */
}RgBchTb;

typedef struct rgDlSf
{
   CmLteTimingInfo schdTime;  /*!< scheduled frame and subframe number */
   Bool           txDone;     /*!< Flag to indicate transmission done */
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ  
   U8            remDatReqCnt; /*!< Counter to maintain count of Ded data received.*/  
   /* Fix [ccpu00126310]: Tracks Data Requests from RLC for both loosely and tight coupled 
      RLC-MAC */
   Bool           statIndDone; /*!< Marks sending of all status indication done */
#endif
   RgBchTb        bch;        /*!< BCH data for this frame */
   RgBcchTb       bcch;       /*!< BCCH allocation for this frame */
   RgPcchTb       pcch;       /*!< PCCH allocation for this frame */
   CmLListCp      tbs;        /*!< TBs or DL HARQ processes */
   U8             numRaRsp;   /*!< No.of RaRsp */
   RgRaRspAlloc   raRsp[RG_MAX_RA_RNTI]; /* Array of list of Ra Rsp 
                                       Allocations for each rarnti */
} RgDlSf;

/** 
  * @brief
  * Logical channel control block for BCCH and PCCH channels. 
  */
typedef struct rgBcchDlschLcCb
{
   CmLteLcId       lcId;                     /*!< Logical channel ID */
   Buffer          *tb;
} RgBcchDlschLcCb;

typedef struct rgBcchBchLcCb
{
   CmLteLcId       lcId;                     /*!< Logical channel ID */
} RgBcchBchLcCb;

typedef struct rgPcchLcCb
{
   CmLteLcId       lcId;                     /*!< Logical channel ID */
} RgPcchLcCb;

/** 
  * @brief
  * Random access information per cell. 
  */
typedef struct rgRaInfoCb
{
   CmLListCp       ueRachLst;                   /*!< List of RaCbs */
} RgRaInfoCb;

typedef struct rgSchInstMap
{
   S16          cellSapId; /*!< SAP ID of the cell in scheduler instance */
   Inst          schInst;  /*!< Scheduler instance that is serving this cell*/
} RgSchInstMap;

/** 
  * @brief
  * Cell Control block per cell 
  */
struct rgCellCb
{
   CmHashListEnt  cellLstEnt;       /*!< Hash list Entity for cell list */
   RgSchInstMap   schInstMap;      /*!< Sch Inst Info */
   CmLteCellId    cellId;           /*!< Cell ID */
   CmLteTimingInfo crntTime;
   U8             maxDlHqProcPerUe;  /*!< Store based on FDD/TDD(UL/DL Index) */
   CrgBwCfg        bwCfg;            /*!< Bandwidth Configuration */
   CrgRachCfg      rachCfg;          /*!< RACH Configuration */
  
   U8              cellActvState;    /*!< Bitmask indicating if cell is active:
                                       Cell is active only after BCCH, PCCH, DL
                                       CCCH, UL CCCH and scheduler config */
   CmLteLcId       ulCcchId;         /*!< LcId for uplink CCCH */
   CmLteLcId       dlCcchId;         /*!< LcId for downlink CCCH */ 
   RgBcchBchLcCb  bcchBchInfo;      /*!< BCCH mapped on BCH 
                                       logical channel control block */
   RgPcchLcCb      pcchInfo;        /*!< PCCH logical channel control block */
   U8              numBcchDlschInfo;
   RgBcchDlschLcCb bcchDlschInfo[RG_MAX_BCCH_DLSCH]; /*!< BCCH mapped on DLSCH 
                                             logical channel control block */
   CmHashListCp    ueLst;            /*!< Hash list of UE control 
                                                   blocks: RgUeCb */
   /*Added support for SPS*/
#ifdef LTEMAC_SPS
   CmHashListCp    spsUeLst;         /*!< Hash list of UECbs with SPS-Rnti
                                                  : Element is RgUeCb */
#endif
   RgRaInfoCb      raInfo;           /*!< Random access related information for
                                          cell */
   RgDlSf          subFrms[RG_NUM_SUB_FRAMES];
#ifdef LTE_L2_MEAS
   CmLListCp         l2mList;                     /*!< List of all L2M requests
                                                     rgSchL2MeasCb */
   RgQciCb           qciArray[LRG_MAX_QCI];       /*!< List of all qci's configured
                                                      rgQciCb */
   RgUlSf            ulSf[RG_NUM_UL_SUB_FRAMES];  /*!< UL Sub frame */
   Bool              sndL2Meas;                   /*!< Send L2 Meas when No L2 Measuremnt timer is running*/

   U32               ttiCycle;             /*!< Count of Number of 10240 Cycles
                                             completed */

#endif /* LTE_L2_MEAS */
   RguFlowCntrlInd  *flowCntrlInd;        /*!<Pre Alloced buffer for flow control */
   /* RRM_SP1_START */
   U32             qcisUlPrbCnt[RG_MAX_QCI_REPORTS];         /*!< GBR Lc PRB count */
   /* RRM_SP1_END */
};

/**
  * @brief
  * Uplink HARQ entity information per UE.
  */
typedef struct rgUlHqEnt
{
   U8           maxHqRetx;        /*!< Maximum number of harq
                                   * re-transmissions */ 
} RgUlHqEnt;

#ifdef LTE_L2_MEAS

typedef struct rgL2MeasUlThrP
{ 
   S32 outStndngBs;
   Bool firstDatSegRcvd;
   CmLteTimingInfo frstSegTimInfo;
   CmLteTimingInfo nextSegTimInfo;
   RguL2MUlThrpSegInd segInd;
   U32 fSegTtiCycNum;
   U32 nSegTtiCycNum;
} RgL2MeasUlThrP;
#endif

/** 
  * @brief
  * Logical channel group
  */
struct rgLcgCb
{
   /* Right now not keeping associated logical channels, searching for
    * associated channels needed only during config */
   LteLcgId    lcgId;  /*!< Group ID */
   U8          lcCount;
#ifdef LTE_L2_MEAS

   RgL2MeasUlThrP lcgBsInfo;

#endif
   Bool        isGbr;  /*!< Indicate if LCG is GBR LCG */
};

/** 
  * @brief
  * Uplink Dedicated Logical channel control block.
  */
struct rgUlLcCb
{
   CmLteLcId     lcId;    /*!< Logical channel ID */
   LteLcgId      lcgId;    /*!< Logical channel group */
#ifdef LTE_L2_MEAS
   U8              qci;     /*!< QCI for this logical channel control block */

   Bool          measOn;   /*!< TRUE if Timing Info needs to be fetched for
                             Scheduled UL IP throughput else FALSE */

#endif
};
/**
  * @brief
  * Uplink control block for UE specific information
  */
typedef struct rgUeUlCb
{
   RgUlHqEnt  hqEnt;      /*!< Uplink HARQ information for the UE */
   RgUlLcCb   lcCb[RG_MAX_LC_PER_UE]; /*!< Dedicated Uplink logical channel 
                                           information */
   RgLcgCb    lcgArr[RG_MAX_LCG_PER_UE]; /*!< Logical channel groups */
   /* Added support for SPS*/
#ifdef LTEMAC_SPS
   Bool            spsLcId[RG_MAX_LC_PER_UE];  /*!< Set to TRUE for SPS LCs */ 
   U8              implRelCntr;    /*!< Number of empty sps transmissions for
                                    implicit(?) release */
   U8              implRelCnt; /*!< Configured value of implicitRelAfter +
                                    RG_ADDL_SPS_OCC_FOR_EXP_REL */
   U8              explRelCntr;/*!< Counter to track explicit release */
   U8              explRelCnt; /*!< Count to trigger explicit release */
/*Added for explicit release - start*/
   U8              spsDatRcvd; /*!< Count to track SPS data for exlicit 
                     release*/
   U32             totSpsDatRcvd; /*!< total number of SPS data recieved
                     from activation*/
#endif
} RgUeUlCb;

/** 
  * @brief
  * Logical channel data as being scheduled by the scheduler.
  */
typedef struct rgLchSchdData
{
   CmLteLcId       lcId;       /*!< Logical channel ID */
   U32             schdSz;   /*!< Scheduled logical channel data */
} RgLchSchdData;

/** 
  * @brief
  * Contention Resolution ID. 
  */
typedef struct RgContResId
{
   U8        resId[RG_CRES_LEN];
}RgContResId;
/** 
  * @brief
  * Transport Block Info scheduled by the scheduler.
  */
typedef struct rgTbInfo{
   U8              numSchLch;
   RgLchSchdData   schdData[RG_MAX_NUM_DED_LC];
   TknU8           schdTa;  /*!< TA scheduled to be sent */
   /* Changed as a result of CR timer implementation*/
   U8              contResCe;   /*!< Contention Resolution CE scheduled 
                                   to be sent */
   RgContResId     *contResId;  /*!< Contention Resolution Identity */
#ifndef L2_OPTMZ
   Buffer          *tb;    /*!< Multiplexed Pdu */
#else
   /* L2 Optimization for mUe/Tti:Pre-allocated MAC Hdr, MAC CEs and padding
    * will be used to store and send to CL. These pointer will be allocated
    * during Harq Proc creation. CL shall not free these pointers.
    * These pointers will be freed duing harq proc deletion
    */
    RgTfuDatReqTbInfo     tb;   /* TB to be sent to CL/PHY*/
#endif
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
   U32             tbSz;   /*!< TB size scheduled for the hqP */    
   RgPdcch         pdcch;  /*!< PDCCH allocated for this HQ Proc */
   CmLList         sfLnk;  /*!< Used to queue it up on a subfrm */
   RgDlSf          *sf;
   CmLteTimingInfo timingInfo; /*!<This field holds the scheduled time */
   TknU32           doa;
   TfuTxMode        txMode;
   Bool              puschRptUsd; /*!< True if Precoding Info in PDCCH has to be
                                   in-accordance with the latest PUSCH report */
   TfuDlCqiPuschInfo puschPmiInfo;/*!< PUSCH report details for explicit PMI
                                       * information to PHY during a PDSCH */
   /* LTE_ADV_FLAG_REMOVED_START */
#ifdef TFU_UPGRADE
   U8            pa;                 /* DL Power control paramter P_A
                                     configured by higher layers
                                     Ref: RRC 36.331, 6.3.2,
                                     PDSCH-Config*/
#endif
   U8 isEnbSFR;
   /* LTE_ADV_FLAG_REMOVED_END */
}RgTbInfo;

/** 
  * @brief
  * Downlink HARQ process information per UE.
  */
struct rgDlHqProcCb
{
   U8              procId;    /*!< HARQ process ID */
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
   U8              numOfTBs;
   RgTbInfo        tbInfo[RG_MAX_TB_PER_UE];
#ifdef LTE_L2_MEAS

   U32           tbId[RGU_MAX_TB];          /*!< RLC's internal tbId */                                    
   U16           status[RGU_MAX_TB];        /*!< Status back to RLC ACK/NACK*/
#endif /* LTE_L2_MEAS */
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
};

/** 
  * @brief
  * Downlink HARQ entity information per UE.
  */
struct rgDlHqEnt
{
   U8           numHqProcs;
   RgDlHqProcCb *procs[RG_MAX_DL_HARQ_NUM]; /*!< Downlink harq processes */
};

/** 
  * @brief
  * Downlink Dedicated Logical channel control block.
  */
struct rgDlLcCb
{
   CmLteLcId            lcId;    /*!< Logical channel ID */
};

/**
  * @brief
  * Downlink control block for UE specific information.
  */
typedef struct rgUeDlCb
{
   RgDlHqEnt  hqEnt;        /*!< Downlink HARQ information for the UE */          
   RgDlLcCb   lcCb[RG_MAX_LC_PER_UE];/*!< Dedicated Downlink logical channels in
UE */ 
} RgUeDlCb;                   


/** 
  * @brief
  * UE control block for UE specific information
  */
struct rgUeCb
{
   CmHashListEnt     ueLstEnt;   /*!< Hash List entity for UE List */
   /* Added suport for SPS*/
#ifdef LTEMAC_SPS
   CmHashListEnt   spsUeLstEnt;/*!< Hash List entity for SPS Rnti */
#endif
   CmLList           rachLstEnt; /*!< Link List entity for RACH UE List*/ 
   CmLteRnti         ueId;       /*!< UE identifier */
   /* Added suport for SPS*/
#ifdef LTEMAC_SPS
   CmLteRnti       spsRnti;    /*!< SPS-Rnti for the UE */
#endif
   RgContResId       contResId;  /*!< Contention Resolution Identity */
   RgUeUlCb          ul;        /*!< UE Specific Uplink information */
   RgUeDlCb          dl;        /*!< UE Specific Downlink information */
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
   CrgTxMode       txMode;      /*!< UE Transmission mode Cfg */
};


/** 
  * @brief
  * Configuration Information for Upper SAPs at RGU,CRG and RGR interfaces.
  */
typedef struct rgUpSapCfgInfo
{
   Pst  sapPst;              /*!< Post information associated with SAP */
   SpId spId;                /*!< SpId associated with SAP */
   SuId suId;                /*!< SuId associated with SAP */
}RgUpSapCfgInfo;

/** 
  * @brief
  * Configuration Information for Lower SAP at TFU interface
  */
typedef struct rgLowSapCfgInfo
{
   Pst    sapPst;            /*!< Post information associated with SAP */
   SpId   spId;              /*!< SpId associated with SAP */
   SuId   suId;              /*!< SuId associated with SAP */
   TmrCfg bndTmr;            /*!< Bind Timer Value */
}RgLowSapCfgInfo;

/** 
  * @brief
  * Control Block structure for Upper SAPs at RGU,CRG and RGR interfaces.
  */
typedef struct rgUpSapCb
{
   RgUpSapCfgInfo sapCfg;    /*!< Configuration information */
   RgSapSts     sapSts;      /*!< Statistics related to SAP */
   RgSapSta     sapSta;      /*!< SAP Status */
}RgUpSapCb;

/** 
  * @brief
  * Control Block structure for Lower SAP at TFU interface.
  */
typedef struct rgLowSapCb
{
   RgLowSapCfgInfo sapCfg;          /*!< SAP configuration information */
   RgSapSts        sapSts;          /*!< Statistics related to SAP */
   RgSapSta        sapSta;          /*!< SAP Status */
   U8              numBndRetries;   /*!< Number of Bind Retries */
}RgLowSapCb;

/** 
  * @brief
  * structure holding Layer Manager Response information cached from control
  * request.
  */
typedef struct rgLmResponse
{
   TranId transId;           /*!< Transaction ID */
   Resp   response;          /*!< Response */
}RgLmResponse;

/** 
  * @brief
  * Structure holding LTE MAC's General Configuration Information.
  */
typedef struct rgGenCb
{
   U8           tmrRes;           /*!< Timer resolution */
   RgLmResponse trcResp;          /*!< Response value for Trace Indication */
   RgLmResponse ustaResp;         /*!< Respones value for Alarms */
   RgLmResponse bndCfmResp;       /*!< Respones value for Bind Confirm */
   Status       memUpperThr;      /*!< upper threshold of the memory pool */
   Status       memLowerThr;      /*!< lower threshold of the memory pool */
}RgGenCb;



/** 
  * @brief
  * Global Control block for LTE-MAC.
  */
struct _rgCb
{
   TskInit      rgInit;                    /*!< Task Init information */
   S16          trcLen;                    /*!< trace length */
   RgGenCb      genCfg;                    /*!< General Config information */
   RgUpSapCb    rguSap[LRG_MAX_RGU_SAPS];  /*!< RGU SAP Control Block */
   RgUpSapCb    crgSap;                    /*!< CRG SAP Control Block */
   RgLowSapCb   tfuSap;                    /*!< TFU SAP Control Block */
   RgGenSts     genSts;                    /*!< General Statistics */
   CmTqCp       tmrTqCp;                   /*!< Timer Task Queue Cntrl Point */
   CmTqType     tmrTq[RG_TQ_SIZE];         /*!< Timer Task Queue */
   CmTimer      tmrBlk[RG_MAX_TIMER];      /*!< Timer Block */
   CmHashListCp cellLst;                   /*!< List of actvie cells: RgCellCb */
   CmHashListCp inactvCellLst;             /*!< List of inactive cells: RgCellCb */
   RgCellCb *prevCell;                     /*!< Reference of last accessed cellCb*/
   U8 bufCnt;                              /*!< Count of buffers in buffersToFree[] */
   Buffer *bufToFree[RG_MAX_DFRD_FREE_BUFS];/*!< List of Shared Buffers to be freed */
};

/** 
  * @brief
  * Subheader of MAC CE.
  */
typedef struct rgMacCeSubHdr
{
   CmLList  subHdrLstEnt;   /*!< List entry for MAC CE subheader */    
   CmLteLcId       lcId;           /*!< Logical channel ID */
} RgMacCeSubHdr;

/** 
  * @brief
  * Subheader of MAC SDU.
  */
typedef struct rgMacSdu
{
   CmLList  sduLstEnt;  /*!< List entry for MAC SDU subheader */
   CmLteLcId       lcId;       /*!< Logical channel ID */
   U16      len;        /*!< Length of the SDU */
   Buffer   *mBuf;      /*!< SDU Buffer */
} RgMacSdu;

/** 
  * @brief
  * MAC PDU Contents.
  */
typedef struct rgMacPdu
{
   CmMemListCp memCp;     /*!< Memory control point */
   CmLListCp   ceLst;     /*!< List of CEs */
   CmLListCp   sduLst;    /*!< List of SDUs */
} RgMacPdu;

/** 
  * @brief
  * Carries the Error Information.
  */
struct rgErrInfo
{
   U8    errType;    /*!< Error Type */
   U16   errCause;   /*!< Cause of Error */
};

/** 
  * @brief
  * Carries the Information to build PDU.
  */
typedef struct rgBldPduInfo
{
   Void           *datReq;    /*!< Common/Dedicated Dat Request */
   U8             reqType;    /*!< EVTRGUCDATREQ/EVTRGUDDATREQ */
   MsgLen         schdTbSz;   /*!< Schedule TB size */
   U8             tbIndex;    /*!< Transport Block Index */
   TknU8          ta;         /*!< Timing Advance */
   RgContResId    *contResId; /*!< Contention Resolution ID */
} RgBldPduInfo;



/* APIs exposed by LIM */

EXTERN S16 rgLIMTfuBndReq ARGS((
         SuId                 suId,
         SpId                 spId));

EXTERN S16 rgLIMTfuUbndReq ARGS((
         SuId                 suId,
         Reason               reason));

EXTERN S16 rgLIMTfuDatReq ARGS((
         TfuDatReqInfo *datReq));

/* APIs exposed by TOM */

EXTERN S16 rgTOMDatInd ARGS((
         TfuDatIndInfo *datInd));

EXTERN S16 rgTOMTtiInd ARGS((
         TfuTtiIndInfo *ttiInd));
EXTERN Void rgTOMRlsSf ARGS((RgDlSf *dlSf));

EXTERN S16 rgSchMacSfAllocReq ARGS((Pst *pst, RgInfSfAlloc *sfInfo));

/*
 * APIs exposed by LMM
 */
EXTERN S16 rgLMMStartTmr ARGS((S16 tmrEvnt, U32 tmrVal, PTR cb));
EXTERN S16 rgLMMStopTmr ARGS((S16 tmrEvnt, PTR cb));
EXTERN S16 rgLMMTmrExpiry ARGS((PTR cb, S16 tmrEvnt));
/* To send a Unsolicited Status Indication to Layer Manager */
EXTERN S16 rgLMMStaInd ARGS((U16 category, U16 event, U16 cause, RgUstaDgn *dgn));
/* To send a Trace Indication to Layer Manager.
 * srcMbuf is not freed. caller has to take care. */
EXTERN Void rgLMMTrcInd ARGS((Buffer *srcMbuf, U8 event));
/* LIM uses this to forward the BndCfm to LMM. This function invokes a
 * Control Confirmation to the LM. */
EXTERN S16 rgLMMBndCfm ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 rgActvTmr ARGS((Ent ent, Inst inst ));
EXTERN S16 rgActvInit ARGS((Ent entity, Inst inst, Region 
       region, Reason reason));
EXTERN S16 rgActvTsk ARGS((Pst *pst, Buffer *mBuf));

/* 
 * APIs exposed by CFG module
 */
EXTERN S16 rgCFGVldtCrgCellCfg ARGS((CrgCellCfg *cellCfg, RgErrInfo *errInfo));
EXTERN S16 rgCFGVldtCrgUeCfg ARGS((CrgUeCfg  *ueCfg, RgCellCb  **cell, RgErrInfo
      *errInfo));
EXTERN S16 rgCFGVldtCrgLcCfg ARGS((CrgLchCfg *lcCfg, RgCellCb **cell, RgUeCb
      **ue, RgErrInfo  *errInfo));
EXTERN S16 rgCFGVldtCrgCellRecfg ARGS((CrgCellRecfg *cellRecfg, RgCellCb **cell,
      RgErrInfo *errInfo));
EXTERN S16 rgCFGVldtCrgUeRecfg ARGS((CrgUeRecfg *ueRecfg, RgCellCb **cell,
      RgUeCb **ue, RgErrInfo *errInfo));
EXTERN S16 rgCFGVldtCrgLcRecfg ARGS((CrgLchRecfg *lcRecfg, RgCellCb **cell,
      RgUeCb **ue, RgUlLcCb **ulLc, RgErrInfo *errInfo));
EXTERN S16 rgCFGVldtCrgUeReset ARGS ((CrgRst *reset,RgCellCb **cell,RgUeCb **ue,
         RgErrInfo   *errInfo));

EXTERN S16 rgCFGCrgCellCfg ARGS((CrgCellCfg *cellCfg, RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgUeCfg ARGS((RgCellCb  *cell, CrgUeCfg  *ueCfg, RgErrInfo
      *errInfo));
EXTERN S16 rgCFGCrgLcCfg ARGS((RgCellCb *cell, RgUeCb *ue, CrgLchCfg
      *lcCfg, RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgCellRecfg ARGS((RgCellCb *cell, CrgCellRecfg *cellRecfg,
      RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgUeRecfg ARGS((RgCellCb *cell, RgUeCb *ue, CrgUeRecfg
      *ueRecfg, RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgLcRecfg ARGS((RgCellCb *cell, RgUeCb *ue, RgUlLcCb
      *ulLc, CrgLchRecfg *lcRecfg, RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgCellDel ARGS((CrgDel *cellDelInfo, RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgUeDel ARGS((CrgDel *ueDelInfo, RgErrInfo *errInfo));
EXTERN S16 rgCFGCrgLcDel ARGS((CrgDel *lcDelInfo, RgErrInfo *errInfo));
EXTERN Void rgCFGFreeCellCb ARGS((RgCellCb *cell));
EXTERN Void rgCFGFreeInactvCellCb ARGS((RgCellCb *cell));
EXTERN S16 rgSchMacCellRegReq ARGS((Pst* pst,RgInfCellReg* regReq));
EXTERN S16 rgCFGCrgUeReset ARGS((RgCellCb *cell,RgUeCb *ue,CrgRst *reset,
         RgErrInfo *errInfo));

/* 
 * APIs exposed by COM module
 */
EXTERN S16 rgCOMCfgReq ARGS((CrgCfgTransId transId, CrgCfgReqInfo
         *cfgReqInfo));

/* APIs Exposed by UIM */
EXTERN S16 rgUIMRguBndCfm ARGS ((SuId suId, U8 status));
EXTERN S16 rgUIMSndDedStaInd ARGS ((RgRguDedStaInd  *staInd));
EXTERN S16 rgUIMSndCmnStaInd ARGS ((RgRguCmnStaInd  *staInd));
EXTERN S16 rgUIMSndDedDatInd ARGS ((RgRguDedDatInd  *datInd));
#ifdef LTE_L2_MEAS
EXTERN S16 rgL2mCreateMeasCb ARGS((
         RgCellCb       *cell,
         RgInfL2MeasReq *measInfo, 
         U8              measType,
         RgErrInfo      *err
));
EXTERN  S16 rgL2mMeasReq ARGS((
         RgCellCb       *cell,
         RgInfL2MeasReq *measInfo,
         RgErrInfo      *err
));
EXTERN S16 rgL2Meas ARGS ((
         RgCellCb  *cell
));
#endif /* LTE_L2_MEAS */
EXTERN S16 rgUIMSndCmnDatInd ARGS ((RgRguCmnDatInd  *datInd));
EXTERN S16 rgUIMCrgBndCfm ARGS ((SuId suId,U8 status));
EXTERN S16 rgUIMCrgCfgCfm ARGS ((CrgCfgTransId transId,U8 status));


/* 
 * APIs exposed by ROM module
 */
EXTERN S16 rgROMDedDatReq ARGS((RgRguDedDatReq *datReq));
EXTERN S16 rgROMCmnDatReq ARGS((RgRguCmnDatReq *datReq));
EXTERN S16 rgROMDedStaRsp ARGS((RgRguDedStaRsp *staRsp));
EXTERN S16 rgROMCmnStaRsp ARGS((RgRguCmnStaRsp *staRsp));
#ifdef LTE_L2_MEAS

EXTERN S16 rgROML2MUlThrpMeasReq ARGS((RgRguL2MUlThrpMeasReq *measReq));

#endif

/* 
 * APIs exposed by RAM module
 */
EXTERN RgUeCb* rgRAMCreateUeCb ARGS((RgCellCb *cell,
                     CmLteRnti tmpCrnti,Bool insert, RgErrInfo *err));
EXTERN S16 rgRAMFreeCell ARGS((RgCellCb    *cell));
EXTERN Void rgRAMFreeUeCb ARGS((RgUeCb *ueCb));
EXTERN S16 rgHndlRaResp ARGS((RgCellCb *cell,CmLteTimingInfo timingInfo,
            RgInfRarInfo  *rarInfo,RgErrInfo *err));
EXTERN S16 rgSchMacRlsRntiReq ARGS((Pst *pst,RgInfRlsRnti* rlsRnti));
/* 
 * APIs exposed by UHM module
 */
EXTERN Void rgUHMCrgUeCfg ARGS(( RgCellCb *cellCb, RgUeCb *ueCb, 
                                 CrgUeCfg *ueCfg));
EXTERN Void rgUHMCrgUeRecfg ARGS(( RgCellCb *cellCb, RgUeCb *ueCb, 
                              CrgUeRecfg *ueRecfg));
/* 
 * APIs exposed by DUX module
 */
EXTERN S16 rgDUXDemuxData ARGS((RgMacPdu *pdu, RgInfCeInfo   *ceInfo,
                                Buffer **mBuf, RgErrInfo *err)); 

/* 
 * APIs exposed by DBM module
 */

EXTERN S16 rgDBMInitInactvCellLst ARGS((U16 numBins));
EXTERN S16 rgDBMDeInitInactvCellLst ARGS((Void));
EXTERN S16 rgDBMInsInactvCellCb ARGS(( RgCellCb *cellCb));
EXTERN RgCellCb* rgDBMGetInactvCellCb ARGS(( CmLteCellId cellId));
EXTERN S16 rgDBMDelInactvCellCb ARGS(( RgCellCb *cellCb));
EXTERN RgCellCb* rgDBMGetNextInactvCellCb ARGS((RgCellCb *cellCb));

EXTERN S16 rgDBMInitCellLst ARGS(( U16 numBins));
EXTERN S16 rgDBMDeInitCellLst ARGS((Void));
EXTERN S16 rgDBMInsCellCb ARGS(( RgCellCb *cellCb));
EXTERN RgCellCb* rgDBMGetCellCb ARGS(( CmLteCellId cellId));
EXTERN S16 rgDBMDelCellCb ARGS(( RgCellCb *cellCb));
EXTERN RgCellCb* rgDBMGetNextCellCb ARGS((RgCellCb *cellCb));
EXTERN S16 rgDBMInitCell ARGS((RgCellCb *cellCb));

EXTERN S16 rgDBMDeInitUeCbLst ARGS(( RgCellCb *cellCb));
EXTERN S16 rgDBMInsUeCb ARGS(( RgCellCb *cellCb, RgUeCb *ueCb));
EXTERN RgUeCb* rgDBMGetUeCb ARGS(( RgCellCb *cellCb, CmLteRnti ueId));
EXTERN RgUeCb* rgDBMGetNextUeCb ( RgCellCb *cellCb, RgUeCb *ueCb);
EXTERN S16 rgDBMDelUeCb ARGS(( RgCellCb *cellCb, RgUeCb *ueCb));
EXTERN Void rgDBMInitUe ARGS((RgUeCb *ueCb));

#ifdef LTE_L2_MEAS
EXTERN Void rgDBMInsUlDedLcCb ARGS((RgUeCb *ueCb, CmLteLcId idx, LteLcgId  gId,
                        U8  qci));
#else
EXTERN Void rgDBMInsUlDedLcCb ARGS((RgUeCb *ueCb, 
                  CmLteLcId idx, LteLcgId gId));
#endif
EXTERN Void rgDBMUpdUlDedLcCb ARGS((RgUeCb *ueCb, 
               RgUlLcCb *ulLcCb, LteLcgId gId));

EXTERN Void rgDBMInsDlDedLcCb ARGS((RgUeCb *ueCb, CmLteLcId idx));
EXTERN Void rgDBMDelUlDedLcCb ARGS((RgUeCb *ueCb, RgUlLcCb *ulLcCb));
EXTERN Void rgDBMDelDlDedLcCb ARGS((RgUeCb *ueCb, RgDlLcCb *dlLcCb));
EXTERN RgDlLcCb* rgDBMGetDlDedLcCb ARGS(( RgUeCb *ueCb, CmLteLcId idx));
EXTERN RgUlLcCb* rgDBMGetUlDedLcCb ARGS(( RgUeCb *ueCb, CmLteLcId idx));
EXTERN Void rgDBMFreeCmnLcLst ARGS((RgCellCb *cellCb));
EXTERN S16 rgDBMChkCmnLcCb ARGS(( RgCellCb *cellCb, CmLteLcId lcId));
EXTERN RgBcchBchLcCb* rgDBMGetBcchOnBch ARGS(( RgCellCb *cellCb ));
EXTERN RgBcchDlschLcCb* rgDBMGetBcchOnDlsch ARGS(( RgCellCb *cellCb, 
                                                   CmLteLcId lcId));
EXTERN RgPcchLcCb* rgDBMGetPcch ARGS(( RgCellCb *cellCb));
EXTERN Void rgDBMInsBcchOnBch ARGS(( RgCellCb *cellCb, CmLteLcId idx));
EXTERN Void rgDBMInsBcchOnDlsch ARGS(( RgCellCb *cellCb, CmLteLcId idx));
EXTERN Void rgDBMInsPcch ARGS(( RgCellCb *cellCb, CmLteLcId idx));

EXTERN Void rgDBMInsUeCbInRachLst ARGS((RgCellCb *cellCb,RgUeCb *ueCb));
EXTERN RgUeCb* rgDBMGetUeCbFromRachLst ARGS((RgCellCb *cellCb,CmLteRnti key));
EXTERN RgUeCb* rgDBMGetNextUeCbFromRachLst ARGS((RgCellCb *cellCb,
                                                      RgUeCb *ueCb));
EXTERN Void rgDBMDelUeCbFromRachLst ARGS((RgCellCb *cellCb,RgUeCb *ueCb));
/* Added support for SPS*/
#ifdef LTEMAC_SPS
EXTERN S16 rgDBMDeInitSpsUeCbLst ARGS((RgCellCb *cellCb));
EXTERN S16 rgDBMInsSpsUeCb ARGS((RgCellCb *cellCb, RgUeCb *ueCb));
EXTERN RgUeCb* rgDBMGetSpsUeCb ARGS((RgCellCb *cellCb, CmLteRnti ueId));
EXTERN RgUeCb* rgDBMGetNextSpsUeCb ARGS((RgCellCb *cellCb, RgUeCb *ueCb));
EXTERN S16 rgDBMDelSpsUeCb ARGS((RgCellCb *cellCb,RgUeCb *ueCb));
#endif /* LTEMAC_SPS */
                        
/* 
 * APIs exposed by DHM module
 */
EXTERN Void rgCFGFreeUeCb ARGS((RgCellCb *cell, RgUeCb *ue));
EXTERN S16 rgDHMHqEntInit ARGS((RgDlHqEnt *hqE, U8 maxHqProcs));
EXTERN Void rgDHMRlsHqProc ARGS((RgDlHqProcCb *hqP));
EXTERN Void rgDHMUeReset ARGS ((RgDlHqEnt *hqE));
EXTERN Void rgDHMHdlBufFree ARGS((Buffer **mBuf));
EXTERN Void rgDHMFreeTbBufs ARGS((Void));
EXTERN S16 rgDHMRlsHqProcTB ARGS((RgDlHqProcCb *hqP,U8 tbIndex));
EXTERN S16 rgDHMGetHqProcFrmId ARGS((RgUeCb *ue, U8 idx, RgDlHqProcCb **hqP));
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
EXTERN S16 rgDHMSndDatReq ARGS((RgCellCb *cellCb, RgDlSf *dlSf, RgTfuDatReqInfo *datInfo, 
                                RgDlHqProcCb *hqProc, RgErrInfo *err));
EXTERN S16 rgDHMHndlDedDatReq ARGS((RgDlHqProcCb *hqProc, 
                              RgRguDDatReqPerUe *datReq, RgErrInfo *err));
/* Changed for contention resolution timer implementation*/
EXTERN S16 rgDHMHndlCmnDatReq ARGS((RgDlHqProcCb *hqProc, 
                RgRguCmnDatReq *datReq, RgErrInfo *err));
/* Changes for MIMO feature addition */
/* Removed dependency on MIMO compile-time flag */
EXTERN S16 rgDHMSndConsolidatedStaInd ARGS ((RgCellCb *cell,RgInfUeInfo *ueInfo,
      CmLteTimingInfo timingInfo, RgErrInfo *err));

EXTERN Void rgDHMFreeUe ARGS((RgDlHqEnt *hqE));
EXTERN S16 rgSchMacRlsHqReq ARGS((Pst *pst, RgInfRlsHqInfo *rlshqUeInfo));

/* 
 * APIs exposed by MUX module
 */
#ifndef L2_OPTMZ
EXTERN S16 rgMUXBldPdu ARGS((RgBldPduInfo *bldPdu, Buffer **txPdu, 
         RgErrInfo *err));
#else
EXTERN S16 rgMUXBldPdu ARGS((RgBldPduInfo *bldPdu, RgTfuDatReqTbInfo *tb, 
         RgErrInfo *err));
EXTERN Bool RgUtlIsTbMuxed ARGS((TfuDatReqTbInfo *tb));
#endif
EXTERN S16 rgMUXBldRarPdu ARGS((RgCellCb *cell, RgInfRaRntiInfo *alloc,
         Buffer **txPdu, RgErrInfo *err));

/*
 * Utility APIs
 */
EXTERN S16  rgAllocSBuf     ARGS((Data **pData, Size size));
EXTERN S16  rgGetMsg        ARGS((Buffer **mBuf));
/*ccpu00117052 - MOD  Passing double pointer for proper NULLP
                      assignment */
EXTERN Void rgFreeSBuf      ARGS((Data **data, Size size));
EXTERN Void rgFillDgnParams ARGS((RgUstaDgn *dgn,U8 dgnType));
EXTERN Void rgUpdtRguDedSts ARGS((U8 stsType, RgRguDedDatReq *datReq));
EXTERN Void rgUpdtRguCmnSts ARGS((U8 stsType));
EXTERN Void rgUpdtCellCnt   ARGS((U8 updtType));
EXTERN Void rgUpdtUeCnt     ARGS((U8 updtType));
EXTERN Void rgGetPstToInst ARGS((Pst *pst, Inst schInst));
EXTERN S16 rgAllocEventMem ARGS((Ptr *memPtr,Size memSize));
EXTERN S16 rgGetEventMem ARGS((Ptr *ptr,Size len,Ptr memCp));
EXTERN S16 rgAllocShrablSBuf ARGS((Data **pData, Size size));
EXTERN Void rgPrintfSubFrameInfo ARGS((RgDlSf *dlSf));
EXTERN Void printMacCellInfo ARGS((Void));
EXTERN Void rgFreeSharableSBuf ARGS((Data **data, Size size)); 


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RGX__ */


/**********************************************************************
         End of file:     rg.x@@/main/3 - Sat Jul 30 02:22:13 2011
**********************************************************************/
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       sd   1. Initial Release.
/main/2      ---       sd   1. LTE MAC 2.1 release
            rg007.201 ap  1. Added support for MIMO
            rg008.201 sd  1. Removed dependency on MIMO compile-time flag
              rsharon 2. Added support for SPS.
            rg009.201 rsharon 1.[ccpu00112372]Changes for CR timer 
                           implementation.
/main/3      ---     gj                1. LTE MAC 3.1 Release.                           
$SID$      ---       rt   1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
