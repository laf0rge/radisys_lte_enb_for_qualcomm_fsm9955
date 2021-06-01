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

/********************************************************************20**

     Name:     EnodeB Application

     Type:     C include file

     Desc:     This file contains 
               

     File:     wr_umm_meas.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_UMM_MEAS_H__
#define __WR_UMM_MEAS_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

#include "wr_emm_anr.h"

#define WR_UMM_MAX_MEAS_ID    32
#define WR_UMM_MAX_MEASOBJ_ID 32
#define WR_UMM_MAX_RPTCFG_ID  32 

#define WR_UMM_MEAS_MAX_CELL_IDX  32 /*!< Measurement Maximum number of cells Index*/

#define WR_UMM_MEAS_MAX_NUM_IDS  32 /*!< Measurement Maximum Number of Id's*/
/* ccpu00131966 : Remove unused Macro */

#define WR_UMM_MEAS_GAP_PERD_NONE  0 /*!< Measurement Gap Period Not Configured*/
#define WR_UMM_MEAS_GAP_PERD_40MS  1 /*!< Measurement Gap Period For 40Ms*/ 
#define WR_UMM_MEAS_GAP_PERD_80MS  2 /*!< Measurement Gap Period For 80Ms*/
#define WR_UMM_MEAS_CSFB_CNT  

/* BitMask to indicate measurement gap configuration per module.
   measurement module will set measUeCB->measGapModuleBitMask 
   resepctively for each module when requested */

#define WR_UMM_MEAS_GAP_INTER_FREQ_HO   0x01
#define WR_UMM_MEAS_GAP_INTER_FREQ_ANR  0x02
#define WR_UMM_MEAS_GAP_INTER_RAT_HO    0x04           
#define WR_UMM_MEAS_GAP_CSFB            0x08           
#define WR_UMM_MEAS_GAP_REEST           0x10          

#define WR_UMM_MEAS_ID_REMOVE_MAX   32
#define WR_UMM_MEAS_OBJ_REMOVE_MAX  32
#define WR_UMM_MEAS_RPT_REMOVE_MAX  32

#define WR_UMM_MEAS_MAX_NEW_PCIS    32
/* LTE_ADV_ABS starts */
#define WR_UMM_MEAS_ID_ABS_REPORTNGH 20
/* LTE_ADV_ABS ends */


#define WR_UMM_MEAS_GAP_PERD_40MS_VAL    40   
#define WR_UMM_MEAS_GAP_PERD_80MS_VAL    80   
/* ccpu00131966 */
#define WR_UMM_MEAS_GAP_OFF_FRM_SR_INDX   2  

/*DRX QCI*/
#define WR_UMM_ANR_MEAS_COUNT            2

/*rsrq_ho*/
#define WR_UMM_RSRQ_RANGE_MIN        0 
#define WR_UMM_RSRQ_RANGE_MAX        34
#define WR_UMM_RSRQ_DFLT_VAL         10

/* In case of DRX to make measurement gap ofset 
    * fall before 6 subframe of DRX start offset */    
#define WR_UMM_MEAS_DRX_OFFSET_DELTA(drxLongCycle)  \
   (wrDrxLongCycle[drxLongCycle] - 6)   

#define WR_UMM_GET_MEAS_GAP_OFFSET(drxOffset, drxLongCycle, gapPeriod)\
   (drxOffset + WR_UMM_MEAS_DRX_OFFSET_DELTA(drxLongCycle))%gapPeriod                             

   
/*#define WR_UMM_MEAS_ENABLE_HRPD_FLAG   1*/


#define WR_SET_MEAS_GAP_BIT_MASK(_measUeCb, _reqGAPpat, _module) \
{                                                                \
   if (WR_UMM_MEAS_GAP_PERD_80MS == _reqGAPpat)                  \
   {                                                             \
      _measUeCb->gapMeasInfo.measGap80msBitMask |= _module;      \
   }                                                             \
   _measUeCb->gapMeasInfo.measGapModuleBitMask |= _module;       \
}


#define WR_RESET_MEAS_GAP_BIT_MASK(_measUeCb, _gapPattern, _module)\
{                                                           \
   LwrMeasGapConfig          *measGapCfg;                   \
    measGapCfg = &(cellCb->measCellCfg->measGap);           \
    _gapPattern = WR_UMM_MEAS_GAP_PERD_NONE;                \
   switch(_module)                                          \
   {                                                        \
      case WR_UMM_MEAS_GAP_INTER_FREQ_HO:                   \
         _gapPattern = measGapCfg->gapPatternForInterFA;    \
         break;                                             \
      case WR_UMM_MEAS_GAP_INTER_FREQ_ANR:                  \
         _gapPattern = measGapCfg->gapPatternForANR;        \
         break;                                             \
      case WR_UMM_MEAS_GAP_INTER_RAT_HO:                    \
         _gapPattern = measGapCfg->gapPatternForInterRAT;   \
         break;                                             \
      case WR_UMM_MEAS_GAP_CSFB:                            \
         _gapPattern = measGapCfg->gapPatternForInterRAT;   \
         break;                                             \
   }                                                        \
   if (WR_UMM_MEAS_GAP_PERD_80MS == _gapPattern)            \
   {                                                        \
      _measUeCb->gapMeasInfo.measGap80msBitMask &= (~_module); \
   }                                                        \
   _measUeCb->gapMeasInfo.measGapModuleBitMask &= (~_module);  \
}

#define WR_FILL_MEASGAP_FOR_CONFIG(_measUeCb, _gapPattern, _transId, _ueCb, _measGapStatus)  \
{                                                                            \
   _measUeCb->gapMeasInfo.gapPattern = _gapPattern;                          \
   _measUeCb->gapMeasInfo.latestTxId  =  _transId;                           \
   _measGapStatus = WR_UMM_MEAS_GAP_INIT_SCH_CONFIG;   \
   /* Measurement gap offset calculation:                                    \
    * To avoid measurement gap offset collision with RI/CQI/SR index OR      \
    * with DRX on period configure measurement gap offset 8 subframe         \
    * before the CQI index, in our present implementation for an UE          \
    * the order of these index would be DRX,RI,CQI and SR. Hence             \
    * placing measurement gap offset index 8 subframe before the CQI         \
    * index. As per the 36.133  specification, UE doesn't transmit           \
    * data in the next subframe after completing the measurement gap.*/      \
   _measUeCb->gapMeasInfo.measGapOffset = ((_ueCb->cqiOffst - 8              \
            + (_gapPattern * WR_UMM_MEAS_GAP_PERD_40MS_VAL))                 \
         % (_gapPattern * WR_UMM_MEAS_GAP_PERD_40MS_VAL));                   \
}





/*Internal Measurement Config structure:*/

typedef struct wrUmmMeasRmvInfo
{
    U8                       measIdRmvLstCnt;
    U8                       measIdRmvLst[WR_UMM_MEAS_ID_REMOVE_MAX];
    U8                       measObjIdRmvLstCnt;
    U8                       measObjIdRmvLst[WR_UMM_MEAS_OBJ_REMOVE_MAX];
    U8                       rptCfgRmvLstCnt;
    U8                       rptCfgRmvLst[WR_UMM_MEAS_RPT_REMOVE_MAX];
} WrUmmMeasRmvInfo;

typedef struct wrUmmMeasAddInfo
{
    U8                       measIdCnt;
    U8                       measObjIdCnt;
    U8                       rptCfgIdCnt;
    U8                       measIdLst[WR_UMM_MAX_MEAS_ID];
    U8                       measObjLst[WR_UMM_MAX_MEASOBJ_ID];
    U8                       rptCfgLst[WR_UMM_MAX_RPTCFG_ID];
    Bool                     isCellsToBeSkip;
    Bool                     isEutraRptCgiFlg;
    Bool                     isInterRatRptCgiFlg;
    Bool                     isUtraRptCgiFlg;
} WrUmmMeasAddInfo;

typedef enum wrUmmMeasGapStatus
{
  WR_UMM_MEAS_GAP_NONE,
  WR_UMM_MEAS_GAP_INIT_SCH_CONFIG,
  WR_UMM_MEAS_GAP_INIT_UE_CONFIG,
  WR_UMM_MEAS_GAP_CONFIGURED,
  WR_UMM_MEAS_GAP_REL
}WrUmmMeasGapStatus;

typedef struct wrUmmMeasGapInfo
{
    U32                     measGapModuleBitMask; /*!< Features requested for MeasGAP of different patterns.*/
    U32                     measGap80msBitMask;   /*!< Features requested for MeasGAP of GP1 pattern only.  */
    U32                     latestTxId;           /*!< Latest Transaction accessed measGAP module.          */
    WrUmmMeasGapStatus       isMeasGapCfg;
    Bool                     isRecfgRqd;
    U8                       gapPattern;          /*!< 1 = 40ms , 2 = 80ms GAP pattern  */ 
    U8                       measGapOffset;       /*!< MeasGap start offset             */
} WrUmmMeasGapInfo;


/** @brief This structure used to store measurement Transaction configuration
 * in Transaction control block.

 -  U8          transId                 Transaction identifier
 -  U8          measIdRmvLstCnt         Meas Id Remove List count
 -  U8          *measIdRmvLst           Meas Id Remove List
 -  U8          measObjIdRmvLstCnt      Meas Object Id Remove List count 
 -  U8          *measObjIdRmvLst        Meas Object Id Remove List
 -  U8          rptCfgRmvLstCnt         Report Config Id Remove List count
 -  U8          *rptCfgRmvLst           Report Config Id Remove List
 -  U8          measIdCnt               Meas Id List count
 -  U8          *measIdLst              Meas Id List per Transaction.
 -  U8          measObjIdCnt            Meas Object List count 
 -  U8          *measObjLst             Meas Object List per Transaction.
 -  U8          rptCfgIdCnt             Report Config List count
 -  U8          *rptCfgLst              Report Config List per Transaction
 -  Bool        isMasGapSetUp           meas Gap Config Flag
 -  Bool        isMeasGapRel            meas Gap Config Release
*/ 
typedef struct wrUmmMeasTranInfo
{
    U8                       transId;
    U8                       transState;
    WrUmmMeasGapStatus       measGapStatus;        /*!< Current MeasGap Status                               */
    PTR                      rrcReCfgMsgPtr;
    WrUmmMeasRmvInfo         rmvMeasInfo;
    WrUmmMeasAddInfo         addMeasInfo;
} WrUmmMeasTransInfo;

/** @brief This Enum used for measurement report config Type.
 */
typedef enum wrUmmMeasIntRptCfgTyp
{
  WR_UMM_MEAS_RPTCFG_NONE,           /*!< None                    */
  WR_UMM_MEAS_RPTCFG_INTRA,          /*!< Intra                   */
  WR_UMM_MEAS_RPTCFG_INTRA_ANR,      /*!< Intra for ANR           */
  WR_UMM_MEAS_RPTCFG_INTER_RSRP,     /*!< RSRP Inter Frequency HO */
  WR_UMM_MEAS_RPTCFG_INTER_RSRQ,     /*!< RSRQ Inter Frequency HO */
  WR_UMM_MEAS_RPTCFG_INTER_ANR,      /*!< ANR Inter Frequency     */  /* Inter-Freq ANR */
  WR_UMM_MEAS_RPTCFG_IRATB2_REDIR,   /*!< IRAT B2 Redirection     */  
  WR_UMM_MEAS_RPTCFG_IRATB2_HO       /*!< IRAT HO    */  
}WrUmmMeasIntRptCfgTyp;

/** @brief This Enum used for measurement Neighbour Frequency Updation
 *  Type.
 */
typedef enum wrUmmMeasNrFreqTyp
{
  WR_UMM_MEAS_NR_NEW_FREQ,        /*!< Neighbour New Frequency */
  WR_UMM_MEAS_NR_EXIST_FREQ       /*!< Neighbour Exist Frequency */
}WrUmmMeasNrFreqTyp;

/** @brief This Enum used for store Cdma carrier frequency information
 -  U8            bandClass    Band Class
 -  U32           arfcn        Arfcn
 */
typedef struct wrMeasCdmaCarrFreq
{
  U8                         bandClass;
  U32                        arfcn;
}WrMeasCdmaCarrFreq;

/** @brief This structure used to maintain carrier freq for RAT Type.
 - U8                     choice            Eutra or CDMA2000
 - U8                     numFreq           Number of frequency
 - union {
 -   U32                  carrierFreq[MAX_NUM_FREQ_CFG]   Eutra Carrier
                                                              frequency
 -   WrMeasCdmaCarrFreq   measObjCDMA2000[MAX_NUM_FREQ_CFG]   CDMA2000
                                                       Carrier Frequency
 - }t;
*/ 
typedef struct wrMeasObjFreq
{
   U8                     choice;
   union {
       U32                     carrierFreq;
       WrMeasCdmaCarrFreq      measObjCDMA2000;
   }t;
} WrMeasObjFreq;

typedef enum wrUtraRatType
{
   WR_UMM_MEAS_DUPLEX_MODE_FDD,
   WR_UMM_MEAS_DUPLEX_MODE_TDD
}WrUtraRatType;


/** @brief This structure used to maintain Cell reconfig information.

 - LwrActType                action         ADD/MOD/DEL 
 - U8                        numMeasObjId   number of measurement object id's
   union
 - {
 -    WrMeasObjFreq          addNeighFreq     addition of neighbours
 -    WrMeasObjFreq          modNeighFreq     modification of neighbour
 -    U8                     measObjId Deletion of neighbour
 - }t;
*/ 
typedef struct wrMeasCellReCfgInfo
{
   LwrActType                action;   
   U8                        numFreq;
   union
   {
      WrMeasObjFreq          addNeighFreq[MAX_NUM_FREQ_CFG];
      WrMeasObjFreq          modNeighFreq[MAX_NUM_FREQ_CFG];
      U8                     measObjIds[MAX_NUM_FREQ_CFG];
   }t;
}WrMeasCellReCfgInfo;

typedef enum wrUmmMeasTyp
{
   WR_UMM_MEAS_TYPE_NONE,
   WR_UMM_MEAS_TYPE_PERD_ANR,
   WR_UMM_MEAS_TYPE_EVENT_ANR,
   WR_UMM_MEAS_TYPE_HO,
   WR_UMM_MEAS_TYPE_CSFB,
   WR_UMM_MEAS_TYPE_CSFB_CCO,
   WR_UMM_MEAS_TYPE_ECSFB,
   WR_UMM_MEAS_TYPE_ABS,
   WR_UMM_MEAS_TYPE_REDIR
   ,
   WR_UMM_MEAS_TYPE_PERD_STRONG_CELL
}WrUmmMeasTyp;
/** @brief This structure used to maintain Meas Id Node.
 - CmLList         lnk           Linked List Node
 - WrUmmMeasTyp    measIdTyp     measurment Id Type
 - U8              measID        Measurement Identifier
 - U8              measObjID     Mesaurement Object identifier
 - U8              rptCfgID      Report config Identifier
 */
typedef struct wrMeasIdNode
{
   CmLList                   lnk; 
   WrUmmMeasTyp              measIdTyp;
   U8                        measID;
   U8                        measObjID; 
   U8                        rptCfgID;
}WrMeasIdNode;

/** @brief This structure used to maintain Free List Node.
 - CmLList         lnk         Linked List Node
 - U8              val         value
 */
typedef struct wrMeasFreeNode
{
   CmLList                   lnk; 
   U8                        val; 
}WrMeasFreeNode;

/** @brief This structure used to maintain Eutra Meas Object Linked list node in
 * cell Cb.

 - CmLList            lnk               Linked list node
 - U32                carrFreq          Carrier frequency
 - U8                 allowedMeasBW     allowed Measurement Bandwidth
 - U8                 presAntPort1      Presence Antenna Port1
 - U8                 neighCellCfg      Neighbour cell configaration 
 - U8                 qOffsetFreq       cell or frequency specific offset
 - Bool               wlCellIdxInUse[WR_UMM_MEAS_MAX_CELL_IDX]  White List 
                                       Cell Id index
 - CmLListCp          wlCellLst         White List cell Information
 - Bool               blCellIdxInUse[WR_UMM_MEAS_MAX_CELL_IDX]  Black List 
                                       Cell Id index
 - CmLListCp          blCellLst         Black List cell Information
*/ 
typedef struct wrUmmMeasEutraMeasObjNode
{
   CmLList                   lnk;
   U32                       carrFreq;
   U8                        allowedMeasBW;
   U8                        presAntPort1;
   U8                        neighCellCfg;
   U8                        qOffsetFreq;
   Bool                      wlCellIdxInUse[WR_UMM_MEAS_MAX_CELL_IDX];
   CmLListCp                 wlCellLst;
   Bool                      blCellIdxInUse[WR_UMM_MEAS_MAX_CELL_IDX];
   CmLListCp                 blCellLst;
}WrUmmMeasEutraMeasObjNode;

/** @brief This structure used to maintain Utra Meas Object Linked list node in
 * cell Cb.

 - CmLList            lnk               Linked list node
 - U16                arfcn             Carrier frequency
 - U8                 qOffsetFreq       cell or frequency specific offset
 - Bool               wlCellIdxInUse[WR_UMM_MEAS_MAX_CELL_IDX]  White List
                                       Cell Id index
 - CmLListCp          wlCellLst         White List cell Information
*/
typedef struct wrUmmMeasUtraMeasObjNode
{
   CmLList                   lnk;
   U16                       arfcn;
   S8                        qOffsetFreq;
   Bool                      cellIdxInUse[WR_UMM_MEAS_MAX_CELL_IDX];
   CmLListCp                 cellLst;
   U8                        duplexMode;
   U8                        tddBandIndicator; /*This is filled only for tddFrequencies. It is configured by OAM. If at some time later, we want bandIndicator fro FDD, we can change the name and use it. Currently, we dont introduce any bugs in FDD, hence named specifically as tddBandIndicator*/
   LwrUtraTddMode            tddMode;
}WrUmmMeasUtraMeasObjNode;
/* @brief This structure used to maintain GERAN meas obj
 * */
typedef struct wrUmmMeasGeranMeasObjNode
{
   CmLList lnk; /* Linked list node*/
   U32     arfcnCnt; /*number of arfcns stored in arfcn[]*/
   U16     arfcn[WR_UMM_MEAS_MAX_GERAN_ARFCN];/*BCCH ARFCN list*/
   U8      bsic[WR_UMM_MEAS_MAX_GERAN_ARFCN];/*BSIC*/
   U8      nccPermitted[WR_UMM_MEAS_MAX_GERAN_ARFCN]; /* nccPermitted*/
   U8      nccPermittedMeas; /* nccPermitted*/
   U8      bandIndicator[WR_UMM_MEAS_MAX_GERAN_ARFCN]; /*BCCH ARFCN bandindicator*/
   S8      qOffset;       /*Q-offset GERAN*/
   Bool    isBandInd1900Pres;/*bandIndicator  1900 Present*/
}WrUmmMeasGeranMeasObjNode;

/** @brief This structure used to maintain Cdma meas Object linked list node in
 * cellCb.

 - CmLList           lnk               Linked list node
 - U8                cdmaType          CdmaCell Type 
 - U8                bandClass         BandClass
 - U32               arfcn             Arfcn 
 - U8                qOffsetRangeInterRAT   frequency specific offset for CDMA2000
 - U8                searchWindowSize   search Window Size for CDMA2000
 - CmLListCp         cdmaCellInfo       Cell List Information
 - Bool              cdmaCellIdxInUse[WR_UMM_MEAS_MAX_CELL_IDX]  Cdma Cell Id index List
*/ 
typedef struct wrUmmMeasCdmaMeasObjNode
{
   CmLList                   lnk;
   U8                        cdmaType;
   U8                        bandClass;
   U32                       arfcn;
   U8                        qOffsetRangeInterRAT;
   U32                       searchWindowSize;
   CmLListCp                 cdmaCellInfo; /*List of WrMeasNrCdmaCellNode structure */ 
   Bool                      cdmaCellIdxInUse[WR_UMM_MEAS_MAX_CELL_IDX];
}WrUmmMeasCdmaMeasObjNode;

/** @brief This structure used to maintain Id's for Updated  Eutra Nr List.

 - U32                         arfcn            Arfcn
 - WrUmmMeasEutraMeasObjNode   *cellCbMeasObj   Pointer to CellCb Measurement Object
 - U8                          addWCellCnt      Addition White Cell Count
 - U8                          addWCellLst[WR_UMM_MEAS_MAX_CELL_IDX]   Addition of white CellList
 - U8                          delWCellCnt       Deletion of White cell Count
 - U8                          delWCellLst [WR_UMM_MEAS_MAX_CELL_IDX]   Deletion of white cellList
 - U8                          addBCellCnt       Addition of Black cell Count
 - U8                          addBCellLst[WR_UMM_MEAS_MAX_CELL_IDX]   Adition of Black cellList
 - U8                          delBCellCnt       deletion of Black cell Count
 - U8                          delBCellLst[WR_UMM_MEAS_MAX_CELL_IDX]   Deletion of black cellList
*/ 
typedef struct wrMeasEutraNrUpdtIdNode
{
   U32                         arfcn;
   WrUmmMeasEutraMeasObjNode   *cellCbMeasObj;
   U8                          addWCellCnt;
   U8                          addWCellLst[WR_UMM_MEAS_MAX_CELL_IDX];
   U8                          delWCellCnt;
   U8                          delWCellLst [WR_UMM_MEAS_MAX_CELL_IDX];
   U8                          addBCellCnt;
   U8                          addBCellLst[WR_UMM_MEAS_MAX_CELL_IDX];
   U8                          delBCellCnt;
   U8                          delBCellLst[WR_UMM_MEAS_MAX_CELL_IDX];
} WrMeasEutraNrUpdtIdNode;

/** @brief This structure used to maintain Id's for Updated  Utra Nr List.

 - U32                         arfcn            Arfcn
 - WrUmmMeasUtraMeasObjNode    *cellCbMeasObj    Pointer to CellCb Measurement Object
 - U8                          addCellCnt       Addition of Utra cell count
 - U8                          addCellLst[WR_UMM_MEAS_MAX_CELL_IDX]   Addition of Utra cell List
 - U8                          delCellCnt       Deletion of Utra Cell Count
 - U8                          delCellLst[WR_UMM_MEAS_MAX_CELL_IDX]   Deletion of Utra cell List
*/
typedef struct wrMeasUtraNrUpdtIdNode
{
   U32                         arfcn;
   WrUmmMeasUtraMeasObjNode    *cellCbMeasObj;
   U8                            addCellCnt;
   U8                            addCellLst[WR_UMM_MEAS_MAX_CELL_IDX];
   U8                             delCellCnt;
   U8                            delCellLst[WR_UMM_MEAS_MAX_CELL_IDX];
} WrMeasUtraNrUpdtIdNode;

/** @brief This structure used to maintain Id's for Update Cdma Nr List.

 - U8                        bandClass        BandClass 
 - U16                       arfcn            Arfcn
 - WrUmmMeasCdmaMeasObjNode  *cellCbMeasObj   Pointer to CellCb Measurement Object
 - U8                        addCellCnt       Addition of Cdma cell count
 - U8                        addCellLst[WR_UMM_MEAS_MAX_CELL_IDX]   Addition of Cdma cell List
 - U8                        delCellCnt       Deletion of Cdma Cell Count
 - U8                        delCellLst[WR_UMM_MEAS_MAX_CELL_IDX]   Deletion of Cdma cell List
*/ 
typedef struct wrMeasCdmaNrUpdtIdNode
{
   U8                        bandClass;
   U16                       arfcn;
   WrUmmMeasCdmaMeasObjNode  *cellCbMeasObj;
   U8                        addCellCnt;
   U8                        addCellLst[WR_UMM_MEAS_MAX_CELL_IDX];
   U8                        delCellCnt;
   U8                        delCellLst[WR_UMM_MEAS_MAX_CELL_IDX];
} WrMeasCdmaNrUpdtIdNode;

/** @brief This structure used to maintain Id's to delete Cdma Nr measObj.
 *
 - U8                        bandClass        BandClass 
 - U16                       arfcn            Arfcn
 */
typedef struct wrMeasCdmaNrDelIdNode
{
   U8                        bandClass;
   U16                       arfcn;
}WrMeasCdmaNrDelIdNode;

/** @brief This structure used to maintain Id's for Updated  Eutra Nr List.

 -  U8                         numEutra   
 -  U8                         num1xCdma
 -  U8                         numCdmaHrpd
 -  U8                         numUtraDel       Nos. of UTRA nodes to be deleted
 -  U8                         numCdmaDel       Nos. of CDMA nodes to be deleted
 -  WrMeasEutraNrUpdtIdNode    eutraNode        eutra Node
 -  WrMeasCdmaNrUpdtIdNode     cdma1xNode       1xcdma Node
 -  WrMeasCdmaNrUpdtIdNode     cdmaHrpdNode     cdma Hrpd Node
 -  U32                        utraNodeDel      UTRA Freq to be deleted
 -  WrMeasCdmaNrDelIdNode     cdma1xDelNode     CDMA Freq to be deleted
*/ 
typedef struct wrUmmIntMeasTrans
{
   CmMemListCp                memCp;         /*!< Added for chained allocation */
   U8                         numEutra;
   U8                         num1xCdma;
   U8                         numCdmaHrpd;
   U8                           numUtra;
   U8                         numUtraDel;
   U8                         numCdmaDel;
   U32                        *utraNodeDel;
   WrMeasEutraNrUpdtIdNode    *eutraNode;
   WrMeasCdmaNrUpdtIdNode     *cdma1xNode;
   WrMeasCdmaNrUpdtIdNode     *cdmaHrpdNode;
   WrMeasUtraNrUpdtIdNode     *utraNode;
   WrMeasCdmaNrDelIdNode      *cdma1xDelNode;
}WrUmmIntMeasTrans;

#if 0
/** @brief This structure used to maintain neighbour frequency information to
 * create NHU measurement Object.

 - WrUmmMeasCellTyp             nrType           Neighbour Type
 - U8                           nrUpdtIdCnt      Neighbour Update Id Count
 - WrMeasEutraNrUpdtIdNode      *nrEutraUpdtId   Neighbour Eutra Update Id
 - WrMeasCdmaNrUpdtIdNode       *nrCdmaUpdtId    Neighbour Cdma Update Id
*/ 
typedef struct wrUmmIntMeasTrans
{
   WrUmmMeasCellTyp             nrType;
   U8                           nrUpdtIdCnt;
   WrMeasEutraNrUpdtIdNode      *nrEutraUpdtId;
   WrMeasCdmaNrUpdtIdNode       *nrCdmaUpdtId;
} WrUmmIntMeasTrans;
#endif
/** @brief This structure used to maintain Report config Linked List Node.

 - CmLList                    lnk          Linked list Node
 - U8                         rptCfgType   Report config Type
 - U8                         rptCfgId     Report config Id
   union {
 -    NhuReportConfigEUTRA    eventA124    Report config for Event A1, or A2 or A4
 -    NhuReportConfigEUTRA    eventA3      Report config for Event A3
 -    NhuReportConfigEUTRA    eventA5      Report Config for Event A5
 -    NhuReportConfigInterRAT eventB1      Report config for Event B1
 -    NhuReportConfigInterRAT eventB2      Report config for Event B2
 -    NhuReportConfigEUTRA    strgCell      Report config for Eutra Periodic Storgest cell
 -    NhuReportConfigInterRAT interRatStrgCell   Report config for Cdma Periodic Strongest cell
 -    NhuReportConfigInterRAT strgCellAnr      Report config for Cdma strongest cell for ANR
 -    NhuReportConfigEUTRA    eutraRptCgi      Report config for Eutra report CGI
 -    NhuReportConfigInterRAT interRatRptCgi   Report config for Inter RAT Report CGI
   } val;
*/ 
typedef struct wrMeasRptCfgNode
{
   CmLList                    lnk;
   U8                         rptCfgType;
   U8                         rptCfgId;
   WrUmmMeasIntRptCfgTyp      intRptCfgTyp;
   union {
      NhuReportConfigEUTRA    eventA124;
      NhuReportConfigEUTRA    eventA3;
      NhuReportConfigEUTRA    eventA5;
      NhuReportConfigInterRAT eventB1;
      NhuReportConfigInterRAT eventB2;
      NhuReportConfigEUTRA    strgCell;
      NhuReportConfigInterRAT interRatStrgCell;
      NhuReportConfigInterRAT strgCellAnr;
      NhuReportConfigInterRAT utraStrgCellAnr;
      NhuReportConfigEUTRA    eutraRptCgi;
      NhuReportConfigInterRAT interRatRptCgi;
      NhuReportConfigInterRAT utraRptCgi;
      NhuReportConfigInterRAT strgCellCsfb;
      NhuReportConfigInterRAT b2Utra;
      NhuReportConfigInterRAT b2Geran;
   } val;
}WrMeasRptCfgNode;

/** @brief This structure used to maintain Eutra Meas Object Linked list node in UeCb.

 - CmLList                     lnk             Linked List node
 - U8                          measObjId       Meas Object Id
 - U16                         newPci          NewPci 
 - WrUmmMeasEutraMeasObjNode  *eutraMeasObj    Eutran CellCb Meas Object pointer
*/ 
typedef struct wrMeasUeCbEutraMeasObjNode
{
   CmLList                     lnk;
   U8                          measObjId;
   U16                         newPci;
   WrUmmMeasEutraMeasObjNode  *eutraMeasObj;
}WrMeasUeCbEutraMeasObjNode;

/** @brief This structure used to maintain Utra Meas Object Linked list node in UeCb.

 - CmLList                     lnk             Linked List node
 - U8                          measObjId       Meas Object Id
 - U16                         newPsc          NewPsc 
 - WrUmmMeasUtraMeasObjNode    *utraMeasObj    Utra Meas Object pointer
*/ 
typedef struct wrMeasUeCbUtraMeasObjNode
{
   CmLList                     lnk;
   U8                          measObjId;
   U16                         newPsc;
   WrUmmMeasUtraMeasObjNode    *utraMeasObj;
}WrMeasUeCbUtraMeasObjNode;
/* @brief This structure used to maintain GERAN meas object linked list node in
 * ueCb.
 * CmLList                     lnk          - Linked list node
 * U8                          measObjId    - Meas obj Id
 * WrUmmMeasGeranMeasObjNode   geranMeasObj - GERAN Meas Obj 
 * */
typedef struct wrMeasUeCbGeranMeasObjNode
{
   CmLList                     lnk;
   U8                          measObjId;
   U16                         newPsc;
   WrUmmMeasGeranMeasObjNode   *geranMeasObj;
}WrMeasUeCbGeranMeasObjNode;

/** @brief This structure used to maintain Cdma Meas Object Linked List nodein
 * UeCb.

 - CmLList                    lnk            Linked List node
 - U8                         measObjId      Meas Object Id
 - U16                        newPci         NewPci 
 - WrUmmMeasCdmaMeasObjNode   *cdmaMeasObj   Cdma CellCb Meas Object pointer
*/ 
typedef struct wrMeasUeCbCdmaMeasObjNode
{
   CmLList                    lnk;
   U8                         measObjId;
   U16                        newPci;
   WrUmmMeasCdmaMeasObjNode   *cdmaMeasObj;
}WrMeasUeCbCdmaMeasObjNode;

/** @brief This structure used to maintain Cdma Cell Information Linked List node.

 - CmLList           lnk         Linked List node
 - U8                cellIndex   Cell Index
 - U16               pci         Physical cell Identifier
*/ 
typedef struct wrMeasNrCdmaCellNode
{
   CmLList                   lnk;
   U8                        cellIndex;
   U16                       pci;
}WrMeasNrCdmaCellNode;

/** @brief This structure used to maintain Eutra black cell info Linked list node.

 - CmLList            lnk              Linked list node
 - U8                 cellIndex        Cell index
 - U16                startPhyCellID   Start Physical cell identifier
 - U8                 phyCellIdRange   Physical Cell Id Range
*/ 
typedef struct wrMeasNrBCellNode
{
   CmLList                   lnk;
   U8                        cellIndex;
   U16                       startPhyCellID;
   U8                        phyCellIdRange;
}WrMeasNrBCellNode;

/** @brief This structure used to maintain Utra cell info Linked list
 * node.

 - CmLList           lnk         Linked list node
 - U8                cellIndex   cell index
 - U16               pci         Pci 
*/ 
typedef struct wrMeasNrUtraCellNode
{
   CmLList                   lnk;
   U8                        cellIndex;
   U16                       psc;
}WrMeasNrUtraCellNode;

/** @brief This structure used to maintain Eutra white cell info Linked list
 * node.

 - CmLList           lnk         Linked list node
 - U8                cellIndex   cell index
 - U16               pci         Pci 
 - U32               cio         Cell individual Offset 
*/ 
typedef struct wrMeasNrWCellNode
{
   CmLList                   lnk;
   U8                        cellIndex;
   U16                       pci;
   U32                       cio;
}WrMeasNrWCellNode;

/** @brief This structure used to maintain utra white cell info Linked list
 * node.

 - CmLList           lnk         Linked list node
 - U8                cellIndex   cell index
 - U16               psc         Psc
*/
typedef struct wrMeasNrUtraWCellNode
{
   CmLList           lnk;
   U8                 cellIndex;
   U16               psc;
}WrMeasNrUtraWCellNode;

/** @brief This Enum used for Threshold Type.
 */
typedef enum wrThrshldTyp
{
  WR_UMM_MEAS_THRES_TYPE_NONE,    /*!< NONE */
  WR_UMM_MEAS_THRESHOLD_RSRP,     /*!< Threshold RSRP */
  WR_UMM_MEAS_THRESHOLD_RSRQ,      /*!< Threshold RSRQ */
  WR_UMM_MEAS_THRESHOLD_RSCP,     /*!< Threshold RSCP */
  WR_UMM_MEAS_THRESHOLD_RSSI     /*!< Threshold RSSI */
}WrThrshldTyp;

/** @brief This Enum used for Cdma Cell Type.
 */
typedef enum wrCdma2kCellTyp
{
  WR_UMM_MEAS_CELL_TYPE_NONE,     /*!< NONE */
  WR_UMM_MEAS_CDMA2K_1XRTT,       /*!< 1xRTT Cdma2000*/
  WR_UMM_MEAS_CDMA2K_HRPD         /*!< Cdma HRPD */
}WrCdma2kCellTyp;

/** @brief This Enum used for Report config Type.
 */
typedef enum wrMeasRptCfgType
{
 WR_UMM_MEAS_EVENT_A1,                 /*!< Event A1 */
 WR_UMM_MEAS_EVENT_A2,                 /*!< Event A2 */
 WR_UMM_MEAS_EVENT_A3,                 /*!< Event A3 */
 WR_UMM_MEAS_EVENT_A4,                 /*!< Event A4 */
 WR_UMM_MEAS_EVENT_A5,                 /*!< Event A5 */
 WR_UMM_MEAS_EVENT_B1,                 /*!< Event B1 */
 WR_UMM_MEAS_EVENT_B2,                 /*!< Event B2 */
 WR_UMM_MEAS_STRONG_CELL,              /*!<Strongest Cell */
 WR_UMM_MEAS_CDMA_STRONG_CELL,    /*!<Strongest Cell for Inter RAT*/
 WR_UMM_MEAS_STRONG_CELL_ANR,          /*!<Strongest Cell for ANR*/
 WR_UMM_MEAS_UTRA_STRONG_CELL_ANR,     /*!<Utra Strongest Cell for ANR*/
 WR_UMM_MEAS_REPORT_CGI,               /*!<Report CGI */
 WR_UMM_MEAS_UTRA_REPORT_CGI,          /*!<Report CGI for Utra*/
 WR_UMM_MEAS_CDMA_REPORT_CGI,     /*!<Report CGI for Inter RAT*/
 WR_UMM_MEAS_INITIAL_CNTXT_REQ,         /*!< Initial Context Procedure */
 WR_UMM_MEAS_UTRA_STRONG_CELL_CSFB,
/* LTE_ADV_ABS starts */
 WR_UMM_MEAS_EVENT_A3_ABS,                 /* To get neighbor cell for ABS purpose */
/* LTE_ADV_ABS ends */
 WR_UMM_MEAS_UTRA_CSFB,
 WR_UMM_MEAS_EVENT_B2_UTRA,             
 WR_UMM_MEAS_EVENT_B2_GERAN,             
 WR_UMM_MEAS_EVENT_A3_FOR_ANR,
 WR_UMM_MEAS_GERAN_STRONG_CELL_CSFB   
} WrMeasRptCfgType;

/** @brief This structure used to maintain New Pci Information.
 - CmLList          lnk         Linked List for New PCI's
 - U8               measObjId   Meas Object ID
 - U16              newPci      New PCI
 - Bool             isConfigred Whether report CGI s configured for this PCI
*/ 
typedef struct wrUmmMeasNewPciInfo
{
   CmLList                   lnk;
   U16                       crnti;
   U16                       newPci;
   Bool                      isConfigred;
} WrUmmMeasNewPciInfo;

/** @brief This structure used to maintain New Pci Information in UeCb.

 - CmLList          lnk         Linked List for New PCI's
 - U8               measObjId   Meas Object ID
 - U16              newPci      New PCI
 - Bool             isConfigred Whether report CGI s configured for this PCI
*/ 
typedef struct wrMeasNewPciNode
{
   CmLList                   lnk;
   U32                       arfcn;
   WrUmmMeasCellTyp          cellTyp;
   CmLListCp                 newPciLst;
} WrMeasNewPciNode;

/** @brief This structure used to maintain New Pci Information in UeCb.

 - WrUmmMeasCellTyp             nrType
 - WrMeasUeCbEutraMeasObjNode   *eutraObj
 - WrMeasUeCbCdmaMeasObjNode    *cdmaObj
 - WrMeasUeCbUtraMeasObjNode    *utraObj
 - WrMeasUeCbGeranMeasObjNode   *geranObj
*/ 
typedef struct wrUmmMeasObjInfo
{
   WrUmmMeasCellTyp             nrType;
   WrMeasUeCbEutraMeasObjNode   *eutraObj;
   WrMeasUeCbCdmaMeasObjNode    *cdmaObj;
   WrMeasUeCbUtraMeasObjNode    *utraFddObj;
   WrMeasUeCbUtraMeasObjNode    *utraTddObj;
   WrMeasUeCbGeranMeasObjNode   *geranObj;
} WrUmmMeasObjInfo;

/** @brief This structure used to maintain report config Information.

 - WrMeasRptCfgType          rptCfgType
 - WrThrshldTyp              thrshldType
 - WrCdma2kCellTyp           cdma2kCellType
 - WrUmmMeasIntRptCfgTyp     intRptCfgType
*/ 
typedef struct wrUmmMeasRptCfgInfo
{
   WrMeasRptCfgType          rptCfgTyp;
   WrThrshldTyp              thrshldTyp;
   WrCdma2kCellTyp           cdma2kCellType;
   WrUmmMeasIntRptCfgTyp     intRptCfgType;
} WrUmmMeasRptCfgInfo;

/** @brief This structure used to store measurement Objects in Cell
 * Control block.

 - U8                          measGapCellCnt      Meas Gap Cell Count 
 - WrUmmMeasEutraMeasObjNode   intraMeasObj        List of Intra Meas Object List per UE.
 - CmLListCp                   interMeasObjLst     List of Inter Meas Object List per UE.
 - CmLListCp                   cdma1xMeasObjLst    List of 1xCDMA Meas Object List per UE.
 - CmLListCp                   cdmaHrpdMeasObjLst  List of CDMA HRPD Meas Object List per UE.
 - CmLListCp                    utraMeasObjLst      List of UTRA Meas Object List per UE.
*/ 
typedef struct wrUmmMeasCellCb
{
   U8                          measGapCellCnt;
   U32                         csfbTmrVal;
   WrUmmMeasEutraMeasObjNode   intraMeasObj;
   CmLListCp                   interMeasObjLst;
   CmLListCp                   cdma1xMeasObjLst; /* Lis of wrUmmMeasCdmaMeasObjNode structure */
   CmLListCp                   cdmaHrpdMeasObjLst;
   CmLListCp                   utraFddMeasObjLst;
   CmLListCp                   utraTddMeasObjLst;
   CmLListCp                   geranMeasObjLst;
   WrUmmAnrMeasInfo            *anrMeasInfo;
   U8                          anrRptCfg;
   U8                          hoRptCfg;
   Bool                        isAnrEnabled;
} WrUmmMeasCellCb;

/*
** @brief This structure used to store measurement configaration in Ue
*  Control block.

 - WrMeasUeCbEutraMeasObjNode   intraMeasObj         Intra Freq Measurement object
 - CmLListCp                    interMeasObjLst      Inter frequency MEas Object List
 - CmLListCp                    interRatMeasObjLst   Inter Rat Meas Object List
 - CmLListCp                    measIdLst            List of Meas Id List per UE.
 - CmLListCp                    rptCfgLst            Report Config List per UE
 - WrMeasRptCfgNode             *eutraRptCgiCfg      Eutra Report CGI Report Config per UE
 - WrMeasRptCfgNode             *utraRptCgiCfg       Utra Report CGI Report Config per UE
 - CmLListCp                    newPciLst            List of New PCI's
 - CmLListCp                    measIDFreeLLst       Free List For Meas Id's
 - CmLListCp                    measIDUsedLLst       Used List For Meas Id's
 - CmLListCp                    measObjIDFreeLLst    Free List For Meas Obj Id's
 - CmLListCp                    measObjIDUsedLLst    Used List For Meas Obj Id's
 - CmLListCp                    measRptCfgIDFreeLLst   Free List For Report Config Id's
 - CmLListCp                    measRptCfgIDUsedLLst   Used List For Report Config Id's
 - U8                           measGap                Meas Gap configuration
 - U8                           preRegHrpdInfo         Pre Registration Info HRPD
 - U8                           measGapPerd            0: None, 1: 40ms: 2: 80ms
 - U8                           measGapOffset          Meas Gap Offset Value
 - Bool                         isRptCgiCfg            To know Report CGI id Configured
                                                       or not. TRUE: Configured,
                                                       FALSE: Not Configured
 - Bool                         isQuantityCfged        TRUE: Configured S-Measure, Quantity
                                            and Mobility Params, FALSE: Not configured
*/
typedef struct wrMeasUeCb
{
   WrMeasUeCbEutraMeasObjNode   intraMeasObj;
   CmLListCp                    interMeasObjLst;
   CmLListCp                    interRatMeasObjLst;
   CmLListCp                    utraFddMeasObjLst;
   CmLListCp                    utraTddMeasObjLst;
   CmLListCp                    geranMeasObjLst;
   CmLListCp                    measIdLst;
   CmLListCp                    rptCfgLst;
   WrMeasRptCfgNode             *eutraRptCgiCfg;
   WrMeasRptCfgNode             *utraRptCgiCfg;
   CmLListCp                    measIDFreeLLst;
   CmLListCp                    measIDUsedLLst;
   CmLListCp                    measObjIDFreeLLst;
   CmLListCp                    measObjIDUsedLLst;
   CmLListCp                    measRptCfgIDFreeLLst;
   CmLListCp                    measRptCfgIDUsedLLst;
   WrUmmAnrMeasInfo             ueAnrMeasInfo;
   WrMeasNewPciNode             intraNewPci;
   CmLListCp                    interNewPciLst;
   CmLListCp                    utranNewPscLst;
   WrUmmMeasGapInfo             gapMeasInfo;
   U8                           rmvAnrMeasId;
   Bool                         isRptCgiCfg;
   Bool                         isAnrMeasCfged;
   Bool                         isQuantityCfged; /* is quantity configuration done */
/* LTE_ADV starts */
   U16                          crnti;
   U16                          cellId;
/* LTE_ADV ends */
   U32                          measIdForR1; /*MeasId configured when report amount is r1 */
} WrMeasUeCb;

EXTERN S16 wrUmmMeasUpdtUtraMeasObjAndIdFromArfcn
(
U32                          arfcn,
WrMeasUeCb                   *measUeCb,
U8                           *measObjId,
WrUmmMeasUtraMeasObjNode     **cellCbMeasObj
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_UMM_MEAS_H__ */

/********************************************************************30**

           End of file:    $SID$

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
------------ -------- ---- ----------------------------------------------
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
