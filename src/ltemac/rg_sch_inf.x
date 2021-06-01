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

  Name:     LTE-MAC layer 
  
  Type:     C Include File 
  
  Desc:     Structures, variables, and typedefs required by the interface between
            MAC and Scheduler.

  File:     rg_sch_inf.x 

  Sid:      rg_sch_inf.x@@/main/2 - Sat Jul 30 02:22:07 2011

  Prg:      

**********************************************************************/
/** 
  @file rg_sch_inf.x 
  @brief Structure declarations and definitions for MAC-SCH interface.
  */

#ifndef __GKSCH_X__ 
#define __GKSCH_X__

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief This structure contains common channel buffer occupancy report information.
 */
typedef struct rgInfCmnBoRpt
{
   S16          cellSapId;
   CmLteCellId  cellId;         /*!< Identifies the cell. CellId value must be within the set of configured cell IDs. */
   CmLteLcId    lcId;           /*!< Identifies the logical channel. lcId value range is defined in
                                     Section 6.2.1 in 36.321 specification. */
   CmLteLcType  lcType;         /*!< Identifies the Logical channel type.lcType can take the following values:
                                      CM_LTE_LCH_BCCH
                                      CM_LTE_LCH_PCCH
                                      CM_LTE_LCH_CCCH
                                      CM_LTE_LCH_DCCH
                                      CM_LTE_LCH_DTCH */
   S32 bo;                      /*!< Buffer occupancy reported by RLC in bytes. */
   union                        /*!< lcType in the primitive is the union selector. */
   {
      CmLteTimingInfo timeToTx; /*!< Timing info for the BO, applicable for BCCH and PCCH. */
      CmLteRnti       rnti;     /*!< Temporary C-RNTI, only for CCCH. RNTI range is specified 
                                     in Section 7.1 in 36.321 specification. */
   } u;
} RgInfCmnBoRpt;
/**
 * @brief This structure contains dedicated channel buffer occupancy report 
 *  information.
 */
typedef struct rgInfDedBoRpt
{
   S16          cellSapId;  /*!< Identifies the cell SAP. cellSapId value must be within the set of configured cell SAP IDs. */
   CmLteCellId  cellId;     /*!< Identifies the cell. CellId value must be within the set of configured cellIds. */
   CmLteRnti    rnti;       /*!< Identifies the UE. RNTI value range is specified in Section 7.1 in 25.321 specification. */
   CmLteLcId    lcId;       /*!< Identifies the logical channel. lcId value range is defined in Section 6.2.1 in 36.321 specification. */
   S32          bo;         /*!< Number of bytes reported as Buffer occupancy by RLC. */
   U32          oldestSduArrTime; /*!< Oldest SDU Arrival Time from Upper Layer */
   Bool         setMaxUlPrio; /*!< set when Pollbit is set from RLC in PDU */
   Bool         setMaxDlPrio; /*!< Set when there is a status PDU in the DL*/
} RgInfDedBoRpt;

/*Fix: start: Indicate UE deletion from MAC to Scheduler*/
/**
 * @brief This structure contains the RNTI which is deleted at MAC 
 *  information.
 */
typedef struct rgInfUeDelInd
{
   S16          cellSapId;  /*!< Identifies the cell SAP. cellSapId value must be within the set of configured cell SAP IDs. */
   CmLteCellId  cellId;     /*!< Identifies the cell. CellId value must be within the set of configured cellIds. */
   CmLteRnti    rnti;       /*!< Identifies the UE. RNTI value range is specified in Section 7.1 in 25.321 specification. */
} RgInfUeDelInd;

/*Fix: end: Indicate UE deletion from MAC to Scheduler*/



/** 
 * @brief This structure contains the uplink grant information that is sent in
          response to the random access request from the UE.
 */
typedef struct rgInfRarUlGrnt
{
   U8 hop;        /*!< Indicates the hopping flag. */     
   U8 rbStart;    /*!< Start Resource block of allocation. */
   U8 numRb;      /*!< Number of resource blocks allocated. */
   U8 tpc;        /*!< TPC command for the uplink grant. */
   U8 iMcsCrnt;   /*!< Current MCS index of the grant. */
   U8 delayBit;   /*!< Indicates the Delay bit. */
   U8 cqiBit;     /*!< Indicates the CQI is enabled or not. */
} RgInfRarUlGrnt;

/** 
 * @brief This structure carries timing adjustment, uplink grant information for the specific temporary C-RNTI.
 */
typedef struct rgInfCrntiInfo
{
    CmLteRnti       tmpCrnti;    /*!< Temporary C-RNTI. RNTI range is specified in Section 7.1 in 36.321 specification. */     
    U8              rapId;       /*!< rapId identifies the index of the Random Access Preamble. rapId ranges from 0 to 63.*/ 
    TknU16          ta;          /*!< Timing Adjustment. Timing Adjustment Value range is defined in Section 6.1.3.5 in 36.321 specification. */ 
    RgInfRarUlGrnt  grnt;        /*!< Uplink Grant to go in RAR. */ 
    Bool            isContFree; /*!< Indicates whether the procedure is contention-free or not. */
}RgInfCrntiInfo;

/** 
 * @brief This structure carries information about downlink control format, scheduled TB size,
          backoff indicator value, and the set of Random Access Responses within this RA-RNTI. 
*/
typedef struct rgInfRaRntiInfo
{
   U16             raRnti;     /*!< RA-RNTI. RNTI range is specified in Section 7.1 in 36.321 specification. */
   TfuPdschDciInfo dciInfo;    /*!< PDCCH allocated for RA-RNTI. For more information
                                    refer to <i>TFU Interface Service Definition</i> (p/n 1100091). */
   U32             schdTbSz;   /*!< Scheduled TB size. schdTbSz value range is defined in Section 7.1.7.2.1 in 36.213
                                    specification. */
   TknU8           backOffInd; /*!< Indicates the Backoff Indicator value. backOffInd value range
                                    is defined in Section 7.2 in 36.321 specification. */
   U8              numCrnti;   /*!< Number of valid RARs in the array. */
   RgInfCrntiInfo  *crntiInfo; /*!< RAR information. */
}RgInfRaRntiInfo;
  
/**
 * @brief This structure contains information about the RA-RNTIs for which
 *  random access responses are generated. 
 */
typedef struct rgInfRarInfo
{
 U8              numRaRntis;    /*!< Indicates the number of valid RA-RNTIs present. */
 RgInfRaRntiInfo *raRntiInfo;   /*!< Contains allocation information per RA-RNTI.  */
 U16             txPwrOffset;   /*!< PDSCH tx power offset for RAR transmission */ 
}RgInfRarInfo;

/**
 *  @brief Logical channel allocation information. 
 */
typedef struct rgInfLcDatInfo
{
   CmLList  lchLstEnt;   /*!< Is not used when part of resource allocation. */
   U8       lcId;        /*!< Identifies the logical channel. lcId value range is defined
                              in Section 6.2.1 in 36.321 specification. */
   U32      numBytes;    /*!< Number of bytes allocated/received to logical channel. */
} RgInfLcDatInfo; 

/**
 * @brief This structure contains control element information received from a 
 *  particular UE.
 */
typedef struct rgInfCeInfo
{
   U16         bitMask;   /*!< Bitmask for the MAC Control elements present. */
   struct                                                  
   {                                                       
      U16   cRnti;        /*!< C-RNTI value. RNTI range is specified in Section 7.1 in 36.321 specification. */
      U8    phr;          /*!< PHR value. PHR value is defined in Section 6.1.3.6 in 36.321 specification. */
      union                                                
      {                                                    
         U8    truncBsr;  /*!< Truncated BSR value. BSR value is defined in 
                               Section 6.1.3.1 in 36.321 specification. */
         U8    shortBsr;  /*!< Short BSR  value.BSR value is defined in 
                               Section 6.1.3.1 in 36.321 specification. */
         struct                                            
         {                                                 
            U8    bs1;   /*!< Buffer size 1 of long BSR. */ 
            U8    bs2;   /*!< Buffer size 2 of long BSR. */ 
            U8    bs3;   /*!< Buffer size 3 of long BSR. */ 
            U8    bs4;   /*!< Buffer size 4 of long BSR. */ 
         }longBsr;       /*!< BSR value is defined in Section 6.1.3.1 in 36.321 specification. */ 
      }bsr;   
   } ces;
      /* L2_COUNTERS */
#ifdef LTE_L2_MEAS
   U8 ulActLCs[RGINF_MAX_NUM_DED_LC];
                        /*!< List of LCs for which Data is received in UL */
#endif


#ifdef LTEMAC_SPS
   U16 spsSduSize;
#endif

} RgInfCeInfo;

typedef struct rgInfLcgInfo
{
   U8    lcgId;        /*!< LCGID. GBR DRB LCG */
   U32   bytesRcvd;    /*!< Contains bytes received for LCG */
}RgInfLcgInfo;

/** 
 * @brief This structure contains control element information received from a 
 *  particular UE identified by the RNTI.
 */
typedef struct rgInfUeDatInd
{
   CmLteRnti   rnti;      /*!< RNTI. RNTI range is specified in Section 7.1 in 36.321 specification. */
   CmLList     ueLstEnt;  /*!< UE linked list entry. */
   RgInfCeInfo ceInfo;    /*!< Contains the control elements received from the UE. */
   RgInfLcgInfo lcgInfo[RGINF_MAX_LCG_PER_UE - 1];  /*!< Contains the bytes received per DRB LCG. */
}RgInfUeDatInd;

/** 
 * @brief This structure carries the control element information received for a
          set of UEs along with timing information. 
 */
typedef struct rgInfSfDatInd
{
   CmMemListCp       memCp;      /*!< Memory control point. */
   CmLListCp         ueLst;      /*!< Pointer to the UE linked list. */
   /* RRM_SP1_START */
   U32               qcisUlPrbCnt[RGINF_MAX_GBR_QCI_REPORTS];/*!< UL gbr LC's PRB count*/
   /* RRM_SP1_END */
   S16               cellSapId;  /*!< Cell SAP Identifier. CellSAP ID value must be within the set of configured cell SAP IDs. */
   CmLteCellId       cellId;     /*!< Identifies the cell. CellId must be within the set of configured cell IDs. */
   CmLteTimingInfo   timingInfo; /*!< Contains information about SFN and subframe. SFN ranges from 0 to 1023 whereas subframe ranges from 0 to 9. */
}RgInfSfDatInd;

/** 
 * @brief This structure carries the information about scheduled logical 
 *  channels within this transport block along with timing adjustment 
 *  information. 
 */
typedef struct rgInfUeTbInfo
{
   Bool                 disTb;       /*!< Currently, not used, but is 
                                          applicable in MIMO case. */
   Bool                 isReTx;      /*!< Indicates the TB transmission type. */
   TknU8                ta;          /*!< Timing Adjustment. */ 
   /* Changed as a result of CR timer implementation*/
   U8 contResCe; /*!< Indicating presence of Contention Resolution CE across MAC-SCH
                   interface to
                  * identify CCCH SDU transmissions which need to
                  * be done without the
                  * contention resolution CE.*/

   U8                   numSchLch;   /*!< Indicates the number of logical 
                                          channels scheduled. */    
   U32                  schdTbSz; 
   RgInfLcDatInfo       schdDat[RGINF_MAX_NUM_DED_LC]; /*!< Contains 
                                          information about scheduled logical
                                          channel. */
} RgInfUeTbInfo;

/** 
 *  @brief This structure carries the UE-specific Resource allocation 
 *   information like RNTI, downlink control format, HARQ process Identifier, 
 *   scheduled TB size, and Transport Block information. 
 */
typedef struct rgInfUeAlloc
{
   CmLteRnti        rnti;      /*!< RNTI. RNTI range is specified in Section 7.1 in 36.321 specification. */ 

/* Added support for SPS*/
#ifdef LTEMAC_SPS
   CmLteRnti            pdcchRnti;   /*!< RNTI used for PDCCH scrambling */
#endif



   TfuPdschDciInfo  dciInfo;   /*!< Contains Downlink Control Information. For more information, 
                                    refer to <i>TFU Interface Service Definition</i> (p/n 1100091). */
   U8               hqProcId;  /*!< HARQ process Identifier. hqProcId ranges between 1 and 8 for FDD mode and 1 and 15 for TDD mode */
   S8                  tbStrtIdx;
   TknU32              doa;
   TfuTxMode           txMode;
   Bool                puschRptUsd;/*!< True, if Precoding Information in PDCCH has to be
                                   in-accordance with the latest PUSCH report */
   TfuDlCqiPuschInfo   puschPmiInfo;/*!< PUSCH report details for explicit PMI
                                       * information to PHY during a PDSCH */


   U8               nmbOfTBs;  /*!< Indicates the number of TBs. Currently this is set to 1. */
   RgInfUeTbInfo    tbInfo[RGINF_MAX_TB_PER_UE]; /*!< Contains TB identifier. */
   /* LTE_ADV_FLAG_REMOVED_START */
#ifdef TFU_UPGRADE
   U8                  pa;          /*!<  DL Power control paramter P_A
                                        configured by higher layers
                                        Ref: RRC 36.331, 6.3.2, PDSCH-Config */
#endif
   U8   isEnbSFR;     /*To check if SFR is enabled*/
   /* LTE_ADV_FLAG_REMOVED_END */
}RgInfUeAlloc;

/** 
 * @brief This structure carries the Resource allocation information for the 
 *  set of scheduled UEs. 
 */
typedef struct rgInfUeInfo
{
   U8               numUes;    /*!< Number of UEs allocated. */
   RgInfUeAlloc     *allocInfo; /*!< Allocation information per UE. */
}RgInfUeInfo;

/** 
 * @brief This structure contains the RNTI and downlink control format 
 *  information for the scheduled BCCH logical channel.  
 */
typedef struct rgInfBcchInfo
{
   CmLteRnti            rnti;       /*!< RNTI range is specified in Section 7.1 in 36.321 specification. */ 
   TfuPdschDciInfo      dciInfo;    /*!< Downlink Control Information. For more information,
                                         refer to <i>TFU Interface Service Definition</i> (p/n 1100091).*/
/* Modified for SI Enhancement*/
#ifndef RGR_SI_SCH
   CmLteLcId            lcId;       /*!< Logical Channel Identifier.lcId value range is defined in 
                                         Section 6.2.1 in 36.321 specification. */
   Bool                 sndStatInd; /*!< Currently this is set to 1 for fresh transmission of BCCH data. */
#else
   Buffer               *pdu;       /*!< PDU being specified for BCCH. */
#endif
   U16                  txPwrOffset;   /*!< PDSCH tx power offset for BCCH 
                                            transmission */ 
}RgInfBcchInfo;

/** 
 * @brief This structure contains the RNTI and the downlink control information for the scheduled PCCH logical channel. 
 */
typedef struct rgInfPcchInfo
{
   CmLteRnti            rnti;      /*!< RNTI range is specified in Section 7.1 in 36.321 specification. */ 
   TfuPdschDciInfo      dciInfo;   /*!< Downlink Control Information.
                                        For more information, refer to <i>TFU Interface Service Definition</i> (p/n 1100091). */
   CmLteLcId            lcId;      /*!< Logical Channel Identifier. lcId value range is defined in
                                        Section 6.2.1 in 36.321 specification. */
                                        
   U16                  txPwrOffset;   /*!< PDSCH tx power offset for PCCH 
                                            transmission */ 
}RgInfPcchInfo;

/** 
 * @brief This structure contains the scheduled logical channel information 
 *  mapped onto the BCH transport channel.
 */
typedef struct rgInfBchInfo
{
/* Modified for SI Enhancement*/
#ifndef RGR_SI_SCH
   CmLteLcId       lcId;    /*!< Logical Channel Identifier. lcId value range is defined in Section 6.2.1 in 36.321 specification. */
#else
   Buffer          *pdu;    /*!< PDU being specified for BCH */
#endif
}RgInfBchInfo;

/** 
 * @brief This structure contains the scheduling information of BCH, PCCH, and 
 *  BCCH channel information.
 */
typedef struct rgInfCmnLcInfo
{
   U16               bitMask;   /*!< Bitmask representing all the common channels present. */
   RgInfBchInfo      bchInfo;   /*!< BCH channel Information. */
   RgInfPcchInfo     pcchInfo;  /*!< Paging Logical Channel Information. */
   RgInfBcchInfo     bcchInfo;  /*!< Broadcast Logical Channel Information. */
}RgInfCmnLcInfo;

#ifdef LTE_L2_MEAS
/**
 * @brief UE-specific allocation information needed for measurements. 
 * */
typedef struct rgInfUeUlAlloc
{
   CmLteRnti            rnti;        /*!< UE ID */ 
   U8                   numPrb;      /*!< Number of total PRB's allocated for this UE */
} RgInfUeUlAlloc;

/**
 * @brief Allocation information of all UEs in this subframe. 
 * */
typedef struct rgInfUlUeInfo
{
   U8                   numUes;      /*!< Number of UE's*/
   CmLteTimingInfo      timingInfo;  /*!< Uplink timing information */
   RgInfUeUlAlloc       *ulAllocInfo;/*!< Uplink Allocations information */
}RgInfUlUeInfo;
#endif /*LTE_L2_MEAS */

/**
*@brief this structure contains the lcId on which flow control need to be performed and the number of packets allowed for admission */
typedef struct rgInfLcInfo
{
   CmLteLcId   lcId;        /*!< lcId for flow control*/
   U32         pktAdmitCnt; /*!<Pkts allowed for the LC */
   U32         maxBo4FlowCtrl; /*!< MAX BO of Queued SDUs at RLC beyond which Flow is Controlled */
}RgInfLcFlowCntrlInfo;


/** 
 * @brief This structure contains the flow control information per UE */
typedef struct rgInfUeFlowCntrlInfo 
{
   CmLteRnti            ueId;  /*!<ueId for flow control*/
   U32                  numLcs;/*!<numLcs in the UE*/
   RgInfLcFlowCntrlInfo lcInfo[RGINF_MAX_NUM_DED_LC]; /*!<LC list*/
}RgInfUeFlowCntrlInfo;  
/**
* @brief This structure contains the flow control information to be sent to MAC */
typedef struct rgInfFlowCntrlInfo
{
   U32                   numUes;/*!<Num UEs for flow control */
   RgInfUeFlowCntrlInfo  ueFlowCntrlInfo[RGINF_MAX_NUM_UE_PER_TTI]; /*!<Ue Info for flow control */
}RgInfFlowCntrlInfo;
/** 
 * @brief This structure contains the scheduling information of common channels, random access responses and UE specific allocations along with timing information.
 */
typedef struct rgInfSfAlloc
{
   CmLteCellId       cellId;        /*!< Identifies the cell. CellId value should be within the set of configured
                                         cell IDs. */
   CmLteTimingInfo   timingInfo;    /*!< Contains Information about SFN and subframe. SFN ranges from 0 to 1023 where as subframe is from 0 to 9. */
   RgInfCmnLcInfo    cmnLcInfo;     /*!< Contains Information about commmon channels. */
   RgInfRarInfo      rarInfo;       /*!< Contains Information about Random Access Responses. */
   RgInfUeInfo       ueInfo;        /*!< Contains UE specific allocation information. */
#ifdef LTE_L2_MEAS
   RgInfUlUeInfo        ulUeInfo;    /*!< Uplink allocation information needed at MAC */
#endif /* LTE_L2_MEAS */
   RgInfFlowCntrlInfo    flowCntrlInfo; /*!<Flow Control information */
}RgInfSfAlloc;

/** 
 * @brief This structure contains the UE specific HARQ and Transport Block 
 *  information to be released.  
 */
typedef struct rgInfUeHqInfo
{
   CmLteRnti   rnti;                        /*!< RNTI which uniquely identifies the UE. RNTI value range is 
                                                 specified in Section 7.1 in 25.321 */
   U8          hqProcId;                    /*!< HARQ Process Identifier.
                                                 hqProcId ranges between 1 and 8 for FDD mode and 1 and 15 for TDD mode. */
   U8          numOfTBs;                    /*!< Identifies the number of TBs. Currently this is set to 1. */
   U8          tbId[RGINF_MAX_TB_PER_UE];   /*!< Indicates the TB Identifier. */
   /* MS_WORKAROUND: to increase Harq Fail Counter .
       The status field is required for tracking the number of harq faliures at MAC.
       As this is already present under L2_MEAS flag, we have replaced it with platform flag */
   U8                   status[RGINF_MAX_TB_PER_UE];      /*!< Indicates HARQ ACK or NACK */ 
} RgInfUeHqInfo;

/** 
 * @brief This structure contains HARQ process information and buffer 
 *  information for a set of UEs to be released in a cell identified by the 
 *  cell ID. 
 */
typedef struct rgInfRlsHqInfo
{
   CmLteCellId   cellId;           /*!< Identifies Cell. CellId should be within
                                        the set of configured cell IDs. */
   U8            numUes;           /*!< Indicates number of UEs present. */
   RgInfUeHqInfo *ueHqInfo;        /*!< Contains UE specific HARQ information. */
}RgInfRlsHqInfo;

/** 
 * @brief This structure contains the released RNTI in a particular cell 
 *  identified by a cellId.
 */
typedef struct rgInfRlsRnti
{
   CmLteCellId   cellId;           /*!< cellId uniquely identifies the
                                        cell. cellId value should be within the set of configured cell IDs. */
   CmLteRnti     rnti;             /*!< RNTI which uniquely identifies the UE. 
                                        RNTI range is specified in Section 7.1 in 25.321 specification. */
   /* Fix : syed ueId change as part of reestablishment.
    * Now SCH to trigger this. CRG ueRecfg for ueId change 
    * is dummy */      
   Bool          ueIdChng;         /*!< Indicates if there is a ueId change as part
                                        of reestablishment */
   CmLteRnti     newRnti;          /*!< new RNTI changed as part of reestablishment */
}RgInfRlsRnti;

/** 
 * @brief This structure contains the cell information along with the 
 *  maximum number of downlink HARQ processes configured per UE in the cell.
 */
typedef struct rgInfCellReg
{
   CmLteCellId cellId;            /*!< Identifies the cell. cellId value should be within the set of configured cell IDs. */
   S16         cellSapId;         /*!< Identifies the cell SAP. cellSapId value should be within the set of configured cell SAP IDs. */
   U8          maxDlHqProcPerUe;  /*!< Indicates Maximum number of downlink HARQ processes per UE. */
}RgInfCellReg;

#ifdef LTE_L2_MEAS
/**
 * @brief struture to hold measurement request for Uplink Average PRB usage per
 * QCI
 * */
typedef struct rgInfPrbReq
{
   U8                   numQci;      /*!< Number fo QCI's in Request */
   U8                   qci[LRG_MAX_QCI_PER_REQ];  /*!< QCI for which PRB has
                                          to be measured */
} RgInfPrbReq;


typedef struct prbUsage
{
   U8                   qciValue;   /*!< Qci Value */
   U32                  prbUsage;   /*!<Prb Usage for this Qci */
}PrbUsage;
/**
 * @brief struture to hold measurement confirm for Uplink Average PRB usage per
 * QCI
 * */
typedef struct rgInfPrbCfm
{
   U8                   numQci;      /*!< Number fo QCI's in Request */
   PrbUsage             prbUsage[LRG_MAX_QCI_PER_REQ];  /*!< Average PRB usage
                                          per QCI */
} RgInfPrbCfm;

/**
 * @brief Structure used by Scheduler to send the measurement request to MAC 
 * */
typedef struct RgInfL2MeasReq
{
   U32                  transId;     /*!< TransId to uniquely identify request */
   U16                  measType;    /*!< For action type Avegare PRB usage 
                                          in Uplink */
   U16                  timePrd;     /*!< Time Period for which measurement
                                          is done */
   CmLteCellId          cellId;      /*!< CellId for which measurement done*/
   union {
      RgInfPrbReq       prbReq;      /*!< Avgerage PRB usage per QCI*/
   } t;
} RgInfL2MeasReq;

/**
 * @brief Structure used by Scheduler to send the measurement stop request to MAC 
 * */
typedef struct RgInfL2MeasStopReq
{
   U32                  transId;     /*!< TransId to uniquely identify request */
   U8                   measType;    /*!< For action type Avegare PRB usage 
                                          in Uplink */
   CmLteCellId          cellId;      /*!< CellId for which measurement done*/
} RgInfL2MeasStopReq;


/**
 * @brief Structure used by Scheduler to send the measurement send request to MAC 
 * */
typedef struct RgInfL2MeasSndReq
{
   U32                  transId;     /*!< TransId to uniquely identify request */
   U8                   measType;    /*!< For action type Avegare PRB usage 
                                          in Uplink */
   U16                  timePrd;     /*!< Time Period for which measurement
                                          is done */
   CmLteCellId          cellId;      /*!< CellId for which measurement done*/
} RgInfL2MeasSndReq;

/**
 * @brief Structure used MAC to send the confirm back to scheduler 
 * */
typedef struct rgInfL2MeasCfm
{
   U16                  transId;     /*!< transId to map to request */
   U8                   measType;    /*!< Action for which measurement done */
   CmLteCellId          cellId;      /*!< CellId for which measurement done*/
   CmStatus             cfm;        /*!< Confirmation possible Values when measType
                                  is invalid status -> NOK and reason -> INVALID */
   union {
      RgInfPrbCfm       prbCfm;      /*!< Avgerage PRB usage per QCI*/
   } u;
} RgInfL2MeasCfm;

/**
 * @brief This API is invoked from l2 Measurements module at scheduler.
 * When Scheduler receives a measurement request from stack manager for Average 
 * PRB usage Per QCI in Uplink, Scheduler invokes this API towards LTE MAC 
 * for the calculations. LTE MAC utilizes the uplink allocations information provided 
 * by scheduler for every subframe and data indications received for this calculation.
 * This API carries a transId to uniquely identify the confirm received for 
 * this request from LTEMAC.
 * */
EXTERN S16 RgSchMacL2MeasReq ARGS((
   Pst*                 pst,
   RgInfL2MeasReq*      l2MeasReq
));
/**
 * @brief This API is invoked from l2 Measurements module at scheduler.
 * When Scheduler receives a measurement send request from stack manager, 
 * it sends L2 measurement to layer manager.
 * */

EXTERN S16 RgSchMacL2MeasSendReq ARGS((
   Pst*                 pst,
   RgInfL2MeasSndReq*      l2MeasReq
));
/**
 * @brief This API is invoked from l2 Measurements module at scheduler.
 * When Scheduler receives a measurement stop request from stack manager,
 * it stops L2 Measurement 
 */
EXTERN S16 RgSchMacL2MeasStopReq ARGS((
   Pst*                 pst,
   RgInfL2MeasStopReq*      l2MeasReq
));

/**
 * @brief This API is invoked from L2 Measurement module at LTE MAC. When LTE MAC
 * completes calculation of Avergae PRB usage per QCI in Uplink for a given time 
 * period, It invokes this API to indicate result back to scheduler. LTE  
 * MAC sends the same transId received in measurement request from Scheduler.
 * */
EXTERN S16 RgMacSchL2MeasCfm ARGS((
   Pst*                 pst,
   RgInfL2MeasCfm*      l2MeasCfm
));
/**
 * @brief This API is invoked from L2 Measurement module at LTE MAC. When LTE MAC
 * stops L2 measurement, it sends L2 measurement cfm.
 * */

EXTERN S16 RgMacSchL2MeasStopCfm ARGS((
   Pst*                 pst,
   RgInfL2MeasCfm*      l2MeasCfm
));

EXTERN S16 RgMacSchL2MeasStop ARGS
((
Pst*                pst,   
RgInfL2MeasCfm      *measInfo
));
#endif /* LTE_L2_MEAS */

typedef struct rgInfLcgRegReq
{
   CmLteCellId          cellId;      /*!< Cell Identifier */
   CmLteRnti            crnti;       /*!< RNTI which uniquely identifies the UE
                                          RNTI range is specified in Section 
                                          7.1 in 25.321 Specification. */
   U8                   lcgId;
   Bool                 isGbr;       /* Indicate if the LCG is Gbr */
} RgInfLcgRegReq;


/* Added support for SPS*/
#ifdef LTEMAC_SPS
/** 
 * @brief This structure contains the logical channel information related to
 *  uplink SPS for a particular UE identified by the CRNTI.
 */
typedef struct rgInfSpsLcInfo
{
   CmLteCellId          cellId;      /*!< Cell Identifier */
   CmLteRnti            crnti;       /*!< RNTI which uniquely identifies the UE
                                          RNTI range is specified in Section 
                                          7.1 in 25.321 Specification. */
   CmLteRnti            spsRnti;     /*!< SPS RNTI. RNTI range is specified in
                                          Section 7.1 in 25.321 Specification. */
   U8                   spsLcCnt;    /*!< identifies the number of SPS
                                          configured logical channels */ 
   U8                   spsLcId[RGINF_MAX_NUM_DED_LC]; /*!< Logical Channel
                                          Identifier. lcId value range is 
                                          defined in Section 6.2.1
                                          in 36.321 Specification. */
   U8                   implRelCnt;  /*!< "implicitRelAfter" vallue */  
   U16                  spsPrd;      /*!< SPS periodicity of the UE */
} RgInfSpsLcInfo;

/** 
 * @brief This structure contains UL SPS param Reset related to
 *  uplink SPS for a particular UE identified by the CRNTI.
 */
typedef struct rgInfUlSpsReset
{
   CmLteCellId          cellId;      /*!< Cell Identifier */
   CmLteRnti            crnti;       /*!< RNTI which uniquely identifies the UE
                                          RNTI range is specified in Section 
                                          7.1 in 25.321 Specification. */
} RgInfUlSpsReset;


/** 
 * @brief This structure contains the information to release UL SPS for a UE */
typedef struct rgInfSpsRelInfo
{
   S16                  cellSapId;   /*!< identifies the cell SAP. Value range
                                          should be within the set of 
                                          configured cell SAP(s).*/
   CmLteRnti            cRnti;       /*!< RNTI of the UE */
   Bool                 isExplRel;    /*!< TRUE if explicit release needs to be
                                          sent to UE */
} RgInfSpsRelInfo;
#endif /* LTEMAC_SPS */

/* 
 * Function Prototypes 
 */
/** 
 * @brief Request from Scheduler to MAC to register a cell. 
 * @details This primitive is used for light-weight loose coupling.
 */
EXTERN S16 cmPkSchMacCellRegReq ARGS((
   Pst*                 pst,    
   RgInfCellReg*        regReq  
));

typedef S16 (*CellRegReq) ARGS((
   Pst*                 pst,
   RgInfCellReg*        regReq
));

/** 
 * @brief Request from Scheduler to MAC to register a cell. 
 * @details The scheduler invokes this primitive after the scheduler cell
 *  configuration is completed. Before calling this primitive, the scheduler
 *  creates a mapping of the cell which is uniquely idetified by cell ID and 
 *  scheduler instance that is serving the cell.
 */
EXTERN S16 RgSchMacCellRegReq ARGS((
   Pst*                 pst,
   RgInfCellReg*        regReq
));
/** 
 * @brief Request from Scheduler to MAC to register a cell. 
 * @details This primitive is used for light-weight loose coupling.
 */
EXTERN S16 cmUnpkSchMacCellRegReq ARGS((
   CellRegReq           func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** 
 * @brief Request from MAC to scheduler to update dedicated BO.
 * @details This primitive is used for light-weight loose coupling.
 */
EXTERN S16 cmPkMacSchDedBoUpdtReq ARGS((
   Pst*                 pst,    
   RgInfDedBoRpt*       boRpt  
));

typedef S16 (*DedBoUpdtReq) ARGS((
   Pst*                 pst,
   RgInfDedBoRpt*       boRpt
));

/** 
 * @brief Request from MAC to scheduler to update dedicated BO.
 * @details MAC layer invokes this primitive towards scheduler when it 
 *  receives status response from the RLC for dedicated channels. Scheduler
 *  takes the buffer occupancy information into consideration while taking
 *  scheduling decisons. 
 */
EXTERN S16 RgMacSchDedBoUpdtReq ARGS((
   Pst*                 pst,
   RgInfDedBoRpt*       boRpt
));
/** 
 * @brief Request from MAC to scheduler to update dedicated BO.
 * @details This primitive is used for light-weight loose coupling.
 */
EXTERN S16 cmUnpkMacSchDedBoUpdtReq ARGS((
   DedBoUpdtReq         func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** 
 * @brief Request from MAC to scheduler to update common channel BO.
 * @details This primitive is used for light-weight loose coupling. 
 */
EXTERN S16 cmPkMacSchCmnBoUpdtReq ARGS((
   Pst*                 pst,    
   RgInfCmnBoRpt*       boRpt  
));

typedef S16 (*CmnBoUpdtReq) ARGS((
   Pst*                 pst,
   RgInfCmnBoRpt*       boRpt
));

/** 
 * @brief Request from MAC to scheduler to update common channel BO.
 * @details MAC layer invokes this primitive towards scheduler when it 
 *  receives status response from the RLC for common channels. Scheduler
 *  takes the buffer occupancy information into consideration while taking
 *  scheduling decisons. 
 */
EXTERN S16 RgMacSchCmnBoUpdtReq ARGS((
   Pst*                 pst,
   RgInfCmnBoRpt*       boRpt
));
/** 
 * @brief Request from MAC to scheduler to update common channel BO.
 * @details This primitive is used for light-weight loose coupling. 
 */
EXTERN S16 cmUnpkMacSchCmnBoUpdtReq ARGS((
   CmnBoUpdtReq         func,
   Pst*                 pst,
   Buffer               *mBuf
));

/*Fix: start:Indicate UE deletion at MAC to scheduler*/
/** 
 * @brief UE delete indication from MAC to scheduler.
 * @details This primitive is used for light-weight loose coupling. 
 */
EXTERN S16 cmPkMacSchUeDelInd ARGS((
   Pst*                 pst,    
   RgInfUeDelInd*       ueDelInd
));

typedef S16 (*UeDelInd) ARGS((
   Pst*                 pst,
   RgInfUeDelInd*       ueDelInd
));

/** 
 * @brief UE deletion indication from MAC to scheduler.
 * @details MAC layer invokes this primitive towards scheduler when it 
 *  receives UE delete Request. As the UE is now deleted at MAC, it should 
 *  not be scheduled.
 */
EXTERN S16 RgMacSchUeDelInd ARGS((
   Pst*                 pst,
   RgInfUeDelInd*       ueDelInd
));
/** 
 * @brief UE delete Indication Request from MAC to scheduler.
 * @details This primitive is used for light-weight loose coupling. 
 */
EXTERN S16 cmUnpkMacSchUeDelInd ARGS((
   UeDelInd            func,
   Pst*                 pst,
   Buffer               *mBuf
));
/*Fix: end:Indicate UE deletion at MAC to scheduler*/
/** 
 * @brief Data Indication Request from MAC to scheduler.
 * @details This primitive is used for light-weight loose coupling. 
 */
EXTERN S16 cmPkMacSchSfRecpInd ARGS((
   Pst*                 pst,    
   RgInfSfDatInd*       datInd
));

typedef S16 (*SfRecpInd) ARGS((
   Pst*                 pst,
   RgInfSfDatInd*       datInd
));

/** 
 * @brief Data Indication Request from MAC to scheduler.
 * @details MAC layer invokes this primitive towards scheduler when it 
 *  receives MAC Control Elements from the data received from the UE. Scheduler
 *  takes these control elements into consideration while taking scheduling 
 *  decisons for the uplink.
 */
EXTERN S16 RgMacSchSfRecpInd ARGS((
   Pst*                 pst,
   RgInfSfDatInd*       datInd
));
/** 
 * @brief Data Indication Request from MAC to scheduler.
 * @details This primitive is used for light-weight loose coupling. 
 */
EXTERN S16 cmUnpkMacSchSfRecpInd ARGS((
   SfRecpInd            func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** 
 * @brief Resource Allocation Request from Scheduler to MAC.
 * @details This primitive is used for light-weight loose coupling.
 */ 
EXTERN S16 cmPkSchMacSfAllocReq ARGS((
   Pst*                 pst,    
   RgInfSfAlloc*        resAllocReq
));

typedef S16 (*SfAllocReq) ARGS((
   Pst*                 pst,
   RgInfSfAlloc*        resAllocReq
));

/** 
 * @brief Resource Allocation Request from Scheduler to MAC.
 * @details Scheduler invokes this primitive for every TTI towards MAC to  
 *  inform the scheduling decisions taken for uplink grants, common channels
 *  and list of UEs to be scheduling during this TTI.
 */
EXTERN S16 RgSchMacSfAllocReq ARGS((
   Pst*                 pst,
   RgInfSfAlloc*        resAllocReq
));
/** 
 * @brief Resource Allocation Request from Scheduler to MAC.
 * @details This primitive is used for light-weight loose coupling.
 */ 
EXTERN S16 cmUnpkSchMacSfAllocReq ARGS((
   SfAllocReq           func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** 
 * @brief Request from Scheduler to release HARQ processes at MAC.
 * @details This primitive is used for light-weight loose coupling.
 */
EXTERN S16 cmPkSchMacRlsHqReq ARGS((
   Pst*                 pst,    
   RgInfRlsHqInfo*      sfHqInfo
));

typedef S16 (*RlsHqReq) ARGS((
   Pst*                 pst,
   RgInfRlsHqInfo*      sfHqInfo
));

/** 
 * @brief Request from Scheduler to release HARQ processes at MAC.
 * @details  Scheduler calls this primitive to send the list of UEs for  
 *  which the HARQ buffers are released to MAC. The Scheduler invokes this
 *  primitive when a positive acknowledgement is received for the TB transmitted
 *  or a TB is retransmitted for the allowed maximum number of retransmissions. 
 */
EXTERN S16 RgSchMacRlsHqReq ARGS((
   Pst*                 pst,
   RgInfRlsHqInfo*      sfHqInfo
));
/** 
 * @brief Request from Scheduler to release HARQ processes at MAC.
 * @details This primitive is used for light-weight loose coupling.
 */

EXTERN S16 cmUnpkSchMacRlsHqReq ARGS((
   RlsHqReq             func,
   Pst*                 pst,
   Buffer               *mBuf
));
/** 
 * @brief Request from Scheduler to release RNTI at MAC.
 * @details  This primitive is used for light-weight loose coupling.
 */
EXTERN S16 cmPkSchMacRlsRntiReq ARGS((
   Pst*                 pst,    
   RgInfRlsRnti*        rlsRnti
));

typedef S16 (*RlsRntiReq) ARGS((
   Pst*                 pst,
   RgInfRlsRnti*        rlsRnti
));

/** 
 * @brief Request from Scheduler to release RNTI at MAC.
 * @details The Scheduler calls this primitive to send the list of RNTIs for 
 *  which the RRC Connection is being rejected.
 */
EXTERN S16 RgSchMacRlsRntiReq ARGS((
   Pst*                 pst,
   RgInfRlsRnti*        rlsRnt
));
/** 
 * @brief Request from Scheduler to release RNTI at MAC.
 * @details  This primitive is used for light-weight loose coupling.
 */
EXTERN S16 cmUnpkSchMacRlsRntiReq ARGS((
   RlsRntiReq           func,
   Pst*                 pst,
   Buffer               *mBuf
));
/* Added support for SPS*/

typedef S16 (*LcgReg) ARGS((
   Pst*                 pst,    
   RgInfLcgRegReq       *lcgRegReq    
));

EXTERN S16 cmPkSchMacLcgRegReq ARGS((
   Pst*                 pst,
   RgInfLcgRegReq       *lcgRegReq  
));

EXTERN S16  RgSchMacLcgRegReq ARGS((Pst *pst, RgInfLcgRegReq *lcgRegReq));

EXTERN S16 cmUnpkSchMacLcgRegReq ARGS((
   LcgReg          func,
   Pst             *pst,
   Buffer          *mBuf
));

EXTERN S16  RgSchMacLcgReg ARGS((Pst* pst, RgInfLcgRegReq *lcgRegReq));

#ifdef LTEMAC_SPS
/** 
 * @brief Primitive from Scheduler to MAC to register the logical channels of
 * a SPS UE
 * @details  This primitive is used for light-weight loose coupling.
 */
EXTERN S16 cmPkSchMacSpsLcRegReq ARGS((
   Pst*                 pst,
   RgInfSpsLcInfo       *lcInfo    
));

typedef S16 (*SpsLcReg) ARGS((
   Pst*                 pst,    
   RgInfSpsLcInfo       *lcInfo    
));

/** 
 * @brief Request from Scheduler to register the SPS related logical channels.
 * @details  Scheduler calls this primitive to send the list of logical channels
 *  that belong to the SPS logical channel group.   
 */
EXTERN S16  RgSchMacSpsLcRegReq ARGS((Pst *pst, RgInfSpsLcInfo *lcInfo));

EXTERN S16 cmUnpkSchMacSpsLcRegReq ARGS((
   SpsLcReg        func,
   Pst             *pst,
   Buffer          *mBuf
));


/** 
 * @brief Primitive from Scheduler to MAC to Reset UL SPS related Params
 * @details  This primitive is used for light-weight loose coupling.
 */
EXTERN S16 cmPkSchMacUlSpsResetReq ARGS((
   Pst*                 pst,
   RgInfUlSpsReset       *ulSpsResetInfo    
));

typedef S16 (*UlSpsReset) ARGS((
   Pst*                 pst,    
   RgInfUlSpsReset       *ulSpsResetInfo    
));

/** 
 * @brief Request from Scheduler to reset UL SPS Params
 * @details  Scheduler calls this primitive to reset implicit and explicit
 * release counters for the UE
 */
EXTERN S16  RgSchMacUlSpsResetReq ARGS((Pst *pst, RgInfUlSpsReset *ulSpsResetInfo));

EXTERN S16 cmUnpkSchMacUlSpsResetReq ARGS((
   UlSpsReset        func,
   Pst             *pst,
   Buffer          *mBuf
));



/** 
 * @brief Primitive from Scheduler to MAC to deregister the logical channels of
 * a SPS UE
 * @details  This primitive is used for light-weight loose coupling.
 */
EXTERN S16 cmPkSchMacSpsLcDeregReq ARGS((
   Pst*                 pst,    
   CmLteCellId          cellId,
   CmLteRnti            crnti
));

typedef S16 (*SpsLcDereg) ARGS((
   Pst*                 pst,    
   CmLteCellId          cellId,
   CmLteRnti            crnti
));

/** 
 * @brief Request from Scheduler to deregister the SPS related logical channels.
 * @details  Scheduler calls this primitive to send the deregistration request
 * for a UE once SPS is released for it
 */
EXTERN S16  RgSchMacSpsLcDeregReq ARGS((Pst *pst, CmLteCellId cellId, CmLteRnti
    crnti));

EXTERN S16 cmUnpkSchMacSpsLcDeregReq ARGS((
   SpsLcDereg           func,
   Pst*                 pst,
   Buffer               *mBuf
));

/** 
 * @brief Primitive from MAC to Scheduler to indicate release of UL SPS for a UE
 * @details  This primitive is used for light-weight loose coupling.
 */
EXTERN S16 cmPkMacSchSpsRelInd ARGS((
   Pst*                 pst,    
   RgInfSpsRelInfo*     relInfo
));

typedef S16 (*SpsRelInd) ARGS((
   Pst*                 pst,    
   RgInfSpsRelInfo*     relInfo
));

/** 
 * @brief Indication from  MAC to Scheduler to release UL SPS for a UE 
 * @details  MAC calls this primitive to inform the scheduler that UL SPS needs
 * to be released for a UE
 */
EXTERN S16  RgMacSchSpsRelInd ARGS((Pst *pst, RgInfSpsRelInfo *relInfo));

EXTERN S16 cmUnpkMacSchSpsRelInd ARGS((
   SpsRelInd            func,
   Pst*                 pst,
   Buffer               *mBuf
));

#endif
#ifdef LTE_L2_MEAS
EXTERN S16 cmPkMacSchL2MeasCfm ARGS((
Pst*                 pst,
RgInfL2MeasCfm       *measCfm
));

EXTERN S16 cmPkMacSchL2MeasStopCfm ARGS((
Pst*                 pst,
RgInfL2MeasCfm       *measCfm
));


EXTERN S16 cmPkSchMacL2MeasReq ARGS((
   Pst*                 pst,
   RgInfL2MeasReq      *measInfo
));

EXTERN S16 cmPkSchMacL2MeasStopReq ARGS((
   Pst*                 pst,
   RgInfL2MeasStopReq *measInfo
));

EXTERN S16 cmPkSchMacL2MeasSendReq ARGS((
   Pst*                 pst,
   RgInfL2MeasSndReq *measInfo
));
typedef S16 (*L2MeasReq) ARGS((
   Pst*                 pst,    
   RgInfL2MeasReq       *measInfo
));

typedef S16 (*L2MeasStopReq) ARGS((
   Pst*                 pst,
   RgInfL2MeasStopReq *measInfo
));

typedef S16 (*L2MeasSendReq) ARGS((
   Pst*                 pst,
   RgInfL2MeasSndReq *measInfo
));
typedef S16 (*L2MeasStopCfm) ARGS((
   Pst                *pst,
   RgInfL2MeasCfm     *measCfm
));

typedef S16 (*L2MeasCfm) ARGS((
   Pst                *pst, 
   RgInfL2MeasCfm     *measCfm
));
EXTERN S16 cmUnpkMacSchL2MeasCfm ARGS
((
L2MeasCfm      func,
Pst            *pst,
Buffer         *mBuf
));
EXTERN S16 cmUnpkSchMacL2MeasReq ARGS((
   L2MeasReq      func,
   Pst            *pst,
   Buffer         *mBuf
));
EXTERN S16 cmUnpkSchMacL2MeasSendReq ARGS((
   L2MeasSendReq      func,
   Pst            *pst,
   Buffer         *mBuf
));
EXTERN S16 cmUnpkSchMacL2MeasStopReq ARGS((
   L2MeasStopReq      func,
   Pst            *pst,
   Buffer         *mBuf
));

EXTERN S16 cmUnpkMacSchL2MeasStopCfm ARGS((
   L2MeasCfm      func,
   Pst            *pst,
   Buffer         *mBuf
));
#endif
EXTERN S16  RgSchMacRlsRnti ARGS((Pst* pst, RgInfRlsRnti* rlsRnti));
EXTERN S16  RgSchMacRlsHq ARGS((Pst* pst, RgInfRlsHqInfo* sfHqInfo));
EXTERN S16  RgSchMacSfAlloc ARGS((Pst* pst, RgInfSfAlloc* resAllocReq));
EXTERN S16  RgMacSchSfRecp ARGS((Pst* pst, RgInfSfDatInd* datInd));
EXTERN S16  RgMacSchCmnBoUpdt ARGS(( Pst* pst, RgInfCmnBoRpt* boRpt));
EXTERN S16 RgMacSchDedBoUpdt ARGS(( Pst* pst, RgInfDedBoRpt* boRpt));
EXTERN S16  RgSchMacCellReg ARGS((Pst* pst,RgInfCellReg* regReq));
#ifdef LTE_L2_MEAS
EXTERN S16 RgSchMacL2Meas ARGS((Pst* pst, RgInfL2MeasReq* l2MeasReq));
EXTERN S16 RgMacSchL2Meas ARGS((Pst* pst, RgInfL2MeasCfm* l2MeasCfm));
EXTERN S16 RgSchMacL2MeasStop ARGS((Pst* pst, RgInfL2MeasStopReq *measInfo));
EXTERN S16 RgSchMacL2MeasSend ARGS((Pst* pst, RgInfL2MeasSndReq *measInfo));
#endif  /* LTE_L2_MEAS */
/* Added support for SPS*/
#ifdef LTEMAC_SPS
EXTERN S16 RgSchMacSpsLcReg ARGS((Pst *pst, RgInfSpsLcInfo *lcInfo));
EXTERN S16 RgSchMacUlSpsReset ARGS((Pst *pst, RgInfUlSpsReset *lcInfo));
EXTERN S16 RgSchMacSpsLcDereg ARGS((Pst *pst, CmLteCellId cellId, CmLteRnti
    crnti));
EXTERN S16 RgMacSchSpsRel ARGS((Pst *pst, RgInfSpsRelInfo* relInfo));
#endif
EXTERN S16 RgMacSchUeDel ARGS((Pst* pst, RgInfUeDelInd*  ueDelInd));

#ifdef __cplusplus
}
#endif
#endif /* __GKSCH_X__*/

/**********************************************************************
  
         End of file:     rg_sch_inf.x@@/main/2 - Sat Jul 30 02:22:07 2011
  
**********************************************************************/

/**********************************************************************
  
        Notes:
  
**********************************************************************/

/**********************************************************************
Put notes here
**********************************************************************/


/**********************************************************************
  
        Revision history:
  
**********************************************************************/
  
/********************************************************************90**
 
     ver       pat       init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       rr         1. LTE MAC 2.1 release
            rg003.201  pk         1. Added Doxygen Comments 
            rg004.201  rnt        1. Modified for SI Enhancement
            rg007.201  ap  1. Added support for MIMO
            rg008.201 sd   1. Removed dependency on MIMO compile-time flag
               rsharon 2. Added support for SPS.
            rg009.201 rsharon   1.[ccpu00112372]: Changes for CR timer.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1
$SID$        ---       rt    1. LTE MAC 4.1 Release.                           
*********************************************************************91*/
