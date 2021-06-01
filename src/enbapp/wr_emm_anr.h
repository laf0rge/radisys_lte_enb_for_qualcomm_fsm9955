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
               

     File:     wr_emm_anr.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

/* File Header */

#ifndef __WR_EMM_ANR_H__
#define __WR_EMM_ANR_H__

#include "wr_lwr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Hash Defines */

#define ZERO 0

#define WR_ANR_EUTRAN_HASH_KEY_LEN  2

#define WR_ANR_MOD_REM_ALLOWED 0x00000001
#define WR_ANR_MOD_HO_ALLOWED  0x00000002
#define WR_ANR_MOD_RANK        0x00000004

#define WR_ANR_MULTIPLY_FACTOR 1000

#define MCC_SIZE  3
#define MNC_SIZE  3

#define WR_MAX_PLMN_ID       5
#define MAX_NR_INFO          64
#define MAX_CDMA2K_NR_INFO   32
#define MAX_BL_CELL_INFO     32
#define MAX_BL_NR_INFO       32
#define MAX_BL_NR_INFO       32
#define MAC_NBR_ENB_LIST     32

#define MAX_NR_CONFIGURED    32

   /* ANR MEASUREMENT*/
#define WR_MAX_WCELL_PER_INTRA      32
#define WR_MAX_WCELL_PER_INTER      32
#define WR_MAX_WCELL_PER_INTERRAT   40

#define WR_MAX_BCELL_PER_INTRA      32
#define WR_MAX_BCELL_PER_INTER      32
#define WR_MAX_BCELL_PER_INTERRAT   32

#define WR_ANR_DEFAULT_MAX_RANK_WEIGHT   10
#define WR_ANR_DEFAULT_MIN_RANK_WEIGHT    8
   /* ANR MEASUREMENT*/


#define WR_MAX_CELL_PER_FREQ   64
#define WR_MAX_INTRA_LTE_FREQ  32
#define WR_MAX_CDMA_1xRTT_CELLS_PER_FREQ  40 /*Total Number of Cells per Freq*/
#define WR_MAX_CDMA_FREQ_PER_BAND   16      /*NeighCellsPerBandclassCDMA2000*/
#define WR_MAX_CDMA_BAND_CLASS      32      /*RRC SPec-maxCDMABandClass 32 but
                                              Supported BND class are only 18
                                              bc 0 to bc 17 - 36.331 V950*/
#define WR_MAX_NUM_CDMA_NEIGH_CELLS 16       /*NeighCellCDMA2000*/
#define WR_MAX_CDMA_NGH_CELL_PER_BAND_CLS 16
#define WR_MAX_CDMA_1xRTT_CELLS           40 /*Total number of Cells per Band*/
#define WR_MAX_CDMA_1xRTT_CELLS_CDMA2K    16 /*PhysCellIdListCDMA2000*/
#define WR_MAX_CDMA_1xRTT_CELLS_CDMA2K_V920  24 /*PhysCellIdListCDMA2000-v920*/

#define WR_MAX_UTRA_FREQ            32
#define WR_MAX_UTRA_CELLS_PER_FREQ  16

#define WR_EMM_MAX_PCI_PER_REPORT   8

#define WR_ANR_DEFAULT_QOFFSET     0
#define WR_ANR_DEFAULT_CIO         0

#define WR_DEFAULT_NCL_SIZE         32
#define WR_DEFAULT_NRT_SIZE         64
#define WR_DEFAULT_1XRTT_NCL_SIZE   16
#define WR_DEFAULT_1XRTT_NRT_SIZE   32
#define WR_DEFAULT_HRPD_NCL_SIZE    16
#define WR_DEFAULT_HRPD_NRT_SIZE    32


#define WR_ANR_DEFAULT_CELL_FLAG 2 /* cell present in both SIB8 and Meas */
#define WR_ANR_DEFAULT_LOAD_BALANCE_RATE  2 /* cell present in both SIB8 and Meas */
   
#define WR_ANR_INTRA_FREQ_BIT        1 /* Bit 0  set*/
#define WR_ANR_INTER_FREQ_BIT        2 /* Bit 1 set */
#define WR_ANR_CDMA_1XRTT_FREQ_BIT   4 /* Bit 2 set */
#define WR_ANR_CDMA_HRPD_FREQ_BIT    8 /* Bit 3 set */
#define WR_ANR_UTRA_FREQ_BIT         16 /* Bit 4 set */
#define WR_ANR_CDMA_1XRTT_BAND_BIT   32 /* Bit 5 set */
#define WR_ANR_GERAN_FREQ_BIT        64 /* Bit 6 set */ 

#define WR_ANR_INTRA_FREQ_SIB3_BIT   0x80


/* Macros for the CDMA 2K bandclass bands */
#define WR_ANR_BAND_CLASS_6 6
#define WR_ANR_BAND_CLASS_0 0

/* Fix for ccpu00132334 - Added to validate the ANR DRX related parameters*/
#define WR_ANR_DEFAULT_DRX_ON_DURATION_TMR_VAL  7
#define WR_ANR_DEFAULT_LONG_DRX_TMR_VAL         11
#define WR_ANR_DEFAULT_DRX_INACTIVITY_TMR_VAL   8
#define WR_ANR_MIN_DRX_ON_DURATION_TMR_VAL      0
#define WR_ANR_MAX_DRX_ON_DURATION_TMR_VAL      15
#define WR_ANR_MIN_LONG_DRX_TMR_VAL             0
#define WR_ANR_MAX_LONG_DRX_TMR_VAL             15
#define WR_ANR_MIN_DRX_INACTIVITY_TMR_VAL       0
#define WR_ANR_MAX_DRX_INACTIVITY_TMR_VAL       21

typedef Bool WrBrdCstInfo;

/* Enumerations */
/** @brief This enumeration contains the configuration status whther the entry
 *         is valid or not
 */
typedef enum {
    WR_N_EQUIP = 0,
    WR_EQUIP
} WrConfStatus;

/** @brief This is the enumerations for different NR types, used to fetch the
 *         White List and Black List
 *  */
typedef enum wrLstNrType
{
    WR_ANR_LST_INTRA_FREQ = 0,
    WR_ANR_LST_INTER_FREQ,
    WR_ANR_LST_CDMA_2K_1xRTT,
    WR_ANR_LST_CDMA_2K_HRPD
}WrLstNrType;


/**   @brief These are the enumerations for the HO counters
 */
typedef enum wrAnrCntrType{
    WR_ANR_HO_FAILURE = 0,
    WR_ANR_HO_ATTEMPT,
    WR_ANR_HO_SUCCESS,
    WR_ANR_HO_WRONG_CELL,
    WR_ANR_MMM_MR_STRG_CELL,
    WR_ANR_MMM_MR_HO,
    WR_ANR_MMM_MR_STRG_CELL_SON,
    WR_ANR_MMM_MR_CS_FB,
    WR_ANR_MMM_MR_PS_HO,
    WR_ANR_MMM_MR_RPRT_CGI_LTE,
    WR_ANR_MMM_MR_RPRT_CGI_HRPD
}WrAnrCntrType;


/** @brief This is the enumerations for different NR types, used for configuring
 *         and maintaing the NR details
 *  */
typedef enum wrEmmNrType
{
    WR_EMM_ANR_EUTRAN,
    WR_EMM_ANR_CDMA_2K_1xRTT,
    WR_EMM_ANR_CDMA_2K_HRPD,
    WR_EMM_ANR_UTRA_FDD,
    WR_EMM_ANR_UTRA_TDD,
    WR_EMM_ANR_GERAN
}WrEmmNrType;

/** @brief This enumeration defines values for the change of NR from White List
 * to Black List and ViceVersa
 *  */

typedef enum wrNrChgType
{
   WR_ANR_WL_TO_BL = 1,
   WR_ANR_BL_TO_WL
}WrNrChgType;


/** @brief This enumeration defines the types of update done on an neighbor
 *         cell. i.e NR is added or deleted from the white list/Black list of
 *         ANR
 *  */

typedef enum wrAnrNrUpdtType
{
   WR_EMM_ANR_ADD_NR,
   WR_EMM_ANR_DEL_NR
}WrAnrNrUpdtType;

/** @brief This structure 
 *  */
typedef enum wrSpdStSclFctr
{
   WR_EMM_ANR_O_DOT_25,
   WR_EMM_ANR_O_DOT_5,
   WR_EMM_ANR_O_DOT_75,
   WR_EMM_ANR_1_DOT_0
} WrSpdStSclFctr;

 /* Interface PBM & MCH structure */

/** @brief This structure contains PCI ID Range info of a 
 *         black listed cells
 *  */
typedef struct wrPhysCellIdRange
{
    U16              startPhyCellID; /**< first PCI of Pci-Range */
    Bool             phyCellIdRangePres; /**< Indicates whether PCI range 
                                          *   is present or not */
    U8               phyCellIdRange; /**< number of PCI in th range */
    WrAnrNrUpdtType  nrUpdtType; /**< Enum to indicate,whether the pci-range is
                                  *   to be added or deleted from the WL/BL */
} WrPhysCellIdRange;

/** @brief This structure contains PCI and offset info of white list cell
 *  */
typedef struct wrNeighCellInfo
{
    U16              phyCellId; /**< PCI of the Neighbor Cell */
    U32              qoffset; /**< qOffset of the Neighbor Cell */
    U32              cio; /**< Cell Individual Offset fo the neighbor cell */
    WrAnrNrUpdtType  nrUpdtType; /**< Enum to indicate,whether the cell is
                                  *   to be added or deleted from the WL/BL */

} WrNeighCellInfo;



/** @brief This structure contains white list cells of a frequency
 *  */
typedef struct wrNrPerFreqWlInfo
{
    U8                  numWlCells; /**< Number of cells(PCI) present in WL */
    WrNeighCellInfo     cellInfo[WR_MAX_CELL_PER_FREQ];
                                   /**< Cell Info of White listed neighbors */
}WrNrPerFreqWlInfo;


/** @brief This structure contains black list cells of a frequency
 *  */
typedef struct wrNrPerFreqBlInfo
{
    U8                  numBlInfos; /**< Number of cells(PCI) present in BL */
    WrPhysCellIdRange   blInfo[WR_MAX_CELL_PER_FREQ];
                                 /**< Cell Info of Black listed neighbors */
} WrNrPerFreqBlInfo;



/* Intra-frequency neighbor related Information: */
/** @brief This structure contains the Intra freq white list and black list 
 *  */
typedef struct wrNrIntraFrqInfo
{
    WrNrPerFreqWlInfo    wlData; /**< WL Info of the intra freq */
    WrNrPerFreqBlInfo    blData; /**< BL Info of the intra freq */
}WrNrIntraFreqInfo;

/* Inter-frequency neighbor related information: */
/** @brief This structure contains the white list and black list for a frequency
 *  */
typedef struct wrNrPerFreqInfo
{
   U16                  dlCarrierFreq; /**< EARFCN of a inter Frequency */
   WrNrPerFreqWlInfo    wlData;  /**< WL Info of the freq */
   WrNrPerFreqBlInfo    blData;  /**< BL Info of the freq */
}WrNrPerFreqInfo;


/** @brief This structure contains the white list and black list for inter
 * frequency neighbors
 */
typedef struct wrNrInterFreqInfo
{
    U8              numFreq;  /**< Number of Inter frequency configured */
    WrNrPerFreqInfo freqList[WR_MAX_INTRA_LTE_FREQ];
                              /**< WL & BL info of the neighbor inter
                               *   frequencies */
} WrNrInterFreqInfo;

/* CDMA2K neighbor related information:*/


/** @brief This structure contains phyCellId of a cell and nr Update Type
 *         
 *  */
typedef struct wrCDMA2KPciInfo
{
    U16              pnOffset;  /**< CDMA pnOffset to be updated */
    WrAnrNrUpdtType  nrUpdtType; /**< Indicates whether to Add/Del the 
                                  *   PCI from list */
}WrCDMA2KPciInfo;

typedef struct wrNrCDMA2KPerFreqInfo
{
   U8               numWlCells;
   WrCDMA2KPciInfo  pciInfoLst[WR_MAX_CDMA_1xRTT_CELLS_PER_FREQ];
}WrNrCDMA2KPerFreqInfo;
/** @brief This structure contains phyCellId info per frquency info. 
 *  */
typedef struct wrCDMA2KFreqInfo
{
    U16              arfcn;   /**< CDMA neighbor ARFCN  */
    WrNrCDMA2KPerFreqInfo  wlData;
} WrCDMA2KFreqInfo;

/** @brief This structure contains per band info of the CDMA 2K neighbors
 *  */
typedef struct wrCDMA2KBandInfo
{
    U8               bandClass; /**< CDMA neighbor bandClass */
    U8               numFreq; /**< number of CDMA frequencies belongs 
                               *   to the bandClass */
    WrCDMA2KFreqInfo neighFreqInfo[WR_MAX_CDMA_FREQ_PER_BAND];
                              /**< FreqInfo list of frequecies in the band */
} WrCDMA2KBandInfo;

/** @brief This structure contains Neighbor cell info of the CDMA 2K neighbors
 *  */
typedef struct wrCDMA2KNeighInfo
{
    U8   numBandClass; /**< number of bandClass present in CDMA 1xRTT/HRPD 
                        *   neighbors list */
    WrCDMA2KBandInfo  neighBandLst[WR_MAX_CDMA_BAND_CLASS];
                       /**< Inforamtion of each bandClass */
} WrCDMA2KNeighInfo;

/** @brief This structure contains psc and offset info of white list cell
 *  */
typedef struct wrUtraNeighCellInfo
{
    U16              psc; /**< PSC of the Neighbor Cell */
    U32              qoffset; /**< qOffset of the Neighbor Cell */
    WrAnrNrUpdtType  nrUpdtType; /**< Enum to indicate,whether the cell is
                                  *   to be added or deleted from the WL/BL */

} WrUtraNeighCellInfo;


/** @brief This structure contains white list cells of a frequency
 *  */
typedef struct wrNrUtraPerFreqBlInfo
{
    U8                  numWlCells; /**< Number of cells(PSC) present in WL */
    WrUtraNeighCellInfo     cellInfo[WR_MAX_CELL_PER_FREQ];
                                   /**< Cell Info of White listed neighbors */
}WrNrUtraPerFreqBlInfo;


/** @brief This structure contains white list cells of a frequency
 *  */
typedef struct wrNrUtraPerFreqWlInfo
{
    U8                  numWlCells; /**< Number of cells(PSC) present in WL */
    WrUtraNeighCellInfo     cellInfo[WR_MAX_CELL_PER_FREQ];
                                   /**< Cell Info of White listed neighbors */
}WrNrUtraPerFreqWlInfo;


typedef struct wrNrUtraPerFreqInfo
{
   U16                  dlCarrierFreq; /**< EARFCN of a inter Frequency */
   WrNrUtraPerFreqWlInfo    wlData;  /**< WL Info of the freq */
}WrNrUtraPerFreqInfo;

typedef struct wrNrUtraFreqInfo
{
   U8                  numFreq;  /**< Number of utra frequency configured */
   WrNrUtraPerFreqInfo freqList[WR_MAX_INTRA_LTE_FREQ];
                              /**< WL info of the neighbor utra frequencies */    
}WrNrUtraFreqInfo;


/** @brief This is union of structures containing White List and Black List for
 *         different NrTypes
 *  */
typedef union wrNrWlBlInfo
{
    WrNrIntraFreqInfo  intra; /**< Holds Intra Freq neighbors WL & BL */
    WrNrInterFreqInfo  inter; /**< Holds Inter Frequency neighbors details */
    WrCDMA2KNeighInfo  cdma1xRtt;   /**< Holds CDMA neighbors details */
} WrNrWlBlInfo;

/** @brief This is union of structures containing White List and Black List for
 *         different NrTypes
 *  */
typedef struct wrNrUpdtInfo
{
    U8                        freqTypeBitMap; /**< Indicates the serving Freq is present
                                            *   in Frequency List 
                                            */
    WrNrInterFreqInfo         eutran; /**< Holds Inter Frequency neighbors details */
    WrCDMA2KNeighInfo         cdma1xRtt;   /**< Holds CDMA 1xRTT neighbors details */
    WrCDMA2KNeighInfo         cdmaHrpd;   /**< Holds CDMA HRPD neighbors details */
    WrNrUtraFreqInfo          utran;   /**< Holds UTRA FDD neighbors details */
    WrNrUtraFreqInfo          utranTdd;   /**< Holds UTRA TDD neighbors details */
    Bool                      skipSibsUpd;/*inidicates to skip  SIBS update*/
    Bool                      skipMeasUpt;/*inidcate to skip measurment update*/
} WrNrUpdtInfo;


typedef struct wrNrEutranFreqCb
{
   U16                        earfcn; /**< EARFCN of the frequency node */
                              
   CmLListCp                  nrWlLst; /**< list conyttaining the white listed cells */
   CmLListCp                  nrBlLst; /**< llist containing the black listed cells */
   CmLList                     freqLstEnt; /**< link node for frequency list */
}WrNrEutranFreqCb;

typedef struct wrNrUtranFreqCb
{
   U16                        arfcn;       /**< ARFCN of the frequency node */
   CmLListCp                  nrWlLst;     /**< list containing the white listed cells */
   CmLListCp                  nrBlLst;     /**< list containing the Black listed cells */
   CmLList                    freqLstEnt;     /**< link node for frequency list */ 
}WrNrUtranFreqCb;

typedef struct wrNrEutranCb
{
   WrNrEutranFreqCb           intraFreqCb;
   CmLListCp                  interFreqList; /* list of EUTRAN frequencies WrEutraFreqCb */
}WrNrEutranCb;

typedef struct wrNrUtranCb
{
   CmLListCp                  utranFreqList;  /* list of UTRAN frequencies WrUtranCb */
}WrNrUtranCb;
typedef struct wrNrCdmaFreqCb
{
   CmLListCp    nrWlLst; /**< list conyttaining the white listed cells */
   CmLListCp    nrBlLst; /**< llist containing the black listed cells */
   U16          arfcn; /**< ARFCN of the frequency node */
   CmLList       freqLstEnt; /**< link node for frequency list */
}WrNrCdmaFreqCb;

typedef struct wrNrCdmaBandCb
{
   U8        bandClass; /* bandClass of the node */
   CmLListCp cdmaFreqList; /* list of WrNrCdmaFreqCb */
   CmLList     bandLstEnt; /**< link node for BandClass list */
}WrNrCdmaBandCb;


typedef struct wrNrCdmaCb
{
   CmLListCp cdmaBandList; /* list of WrNrCdmaBandCb */

}WrNrCdmaCb;
/* This structure holds the list of In-use GERAN neighbours 
 * i.e. it rpresents 
 * FAPService.{i}.CellConfig.LTE.RAN.NeighborListInUse.InterRATCell.GSM */
typedef struct wrNrGeranCb
{
   CmLListCp   geranNrCellLst; /* List of WrGeranCellCb*/
}WrNrGeranCb;
/** @brief This structure contains the all info specific to a CELL
 *  */

typedef struct wrCellNrCb
{
   U8                         cellId;/*!< cell ID to the nrCb belongs */

   WrNrEutranCb               eutranCb; /*!< Eutran Neighbors Control Block */
   WrNrCdmaCb                 cdma1xRttCb; /*!< Cdma 1xRTT Neighbors Control Block */
   WrNrCdmaCb                 cdmaHrpdCb; /*!< Cdma HRPD Neighbors Control Block */
   WrNrUtranCb                utranCb;    /*!< Utran FDD Neighbors control block */
   WrNrUtranCb                utranTddCb;    /*!< Utran TDD Neighbors control block */
   WrNrGeranCb                geranCb;   /*!<GERAN neighbour control blocks */ 
   WrNrWlBlInfo               *nrWlBlInfo;   /**< Pointer to structure containg the WL 
                                              *   and BL info to PBM and MCH */
   WrNrUpdtInfo               *nrUpdtInfo;   /**< Pointer to structure containg the WL 
                                              *   and BL info to PBM and MCH */
   U8                         maxRnkWght;
   U8                         minRnkWght;
   U16                        epochCnt;
   U64                        epochTmrVal;
   U32                        tricePeriodIntvCnt; /*TRICE timer intervel count */
   CmTimer                    epochTmr;
   CmTimer                    triceTmr;
   U32                        cellValidAgeTmrVal;
   CmTimer                    cellValidAgeTmr;
} WrCellNrCb;

/** @brief This structure used to get Pci Information from ANR.
 *  */
typedef struct wrNewPci
{
   Bool   pciExist; /*! TRUE: OLD PCI, FALSE:NEW PCI */
   U16    pci;  /*!< Physical cell Identifier*/
} WrNewPci;

typedef struct wrCdmaFreqInfo
{
   U8                         bandClass;
   U16                        arfcn;
}WrCdmaFreqInfo;


/** @brief This union contains the frequency info of PCI to be validated
 *  */
typedef  union wrNrInfo
{
   U16                  earfcn;
   WrCdmaFreqInfo       cdmaNrInfo;
}WrNrInfo;


/** @brief This structure used to get List of PCI Information from ANR.
 *  */
typedef struct wrNewPciLst
{
   U8        numComp; /*! < number of Components */
   U8        newPciCnt; /*<!Number of New PCI's found in the List.
                          This info filled by ANR Module */
   WrEmmNrType nrType;

   WrNrInfo  nrInfo; /* cell freq info of PCI list */

   WrNewPci  newPci[WR_EMM_MAX_PCI_PER_REPORT];  /*!< Physical cell Identifier List*/
} WrNewPciLst;

/* PUBLIC Function Prototypes */

/* API for teh MCH, X2I and PBM Module */
EXTERN WrNrWlBlInfo* wrEmmAnrGetWhiteAndBlackList 
(
    U8                           cellId,
    WrLstNrType                  nrType, 
    WrBrdCstInfo                 brInfo
);

EXTERN Bool wrEmmAnrValidatePci 
(
   U8                            cellId, 
   U16                           pci, 
   WrEmmNrType                   nrType,
   WrNrInfo                      *nrInfo,
   U8                            *reportType,
   U8                            isEvntAnr,
   U8                            ueIdx
);

EXTERN S16 wrEmmAnrValidatePciList
(
   U8                            cellId,
   WrEmmNrType                   nrType,
   WrNewPciLst                   *pciList,
   U8                            isEvntAnr,
   U8                            ueIdx
);

/* API for EMM */
EXTERN S16 wrEmmAnrDeInit
(
   U8                            cellId
);

EXTERN S16 wrEmmAnrCellCfgReq
(
   U8                            cellId,
   LwrCellAnrCfgGrp           *anrCfgGrp
);

EXTERN S16 wrEmmAnrCellReCfgReq
(
   U8                            cellId
);


/* API for HO and MCH */

/* ANR Periodic NR Rank OPtimisation */
EXTERN Void wrEmmAnrAlgoPrcPrdEutranNrOpt
(
   U8                            cellId
);

EXTERN Void wrEmmAnrAlgoPrcPrdInterRatNrOpt
(
   U8                            cellId
);

EXTERN S16 wrEmmAnrAddNrFreq 
(
    U8                           cellId,
    U8                           numFreq,
    LwrNeighFreqAddModCfg        *nbrFreqAdd
);

EXTERN S16 wrEmmAnrDelNrFreq 
(
    U8                           cellId,
    U8                           numFreq,
    LwrNeighFreqDelCfg           *nbrFreqDel
);


EXTERN PTR wrEmmAnrCheckEnbBL
(
   U8                            cellId, 
   U32                           enbid
);

EXTERN S16 wrEmmAnrProcessIntraFreqChg
(
   U8                            cellId
);

EXTERN S16 wrEmmAnrUpdtNrCellRank ARGS(
(
      U16 cellId,
      U16 hoPci,
      U32 earfcnDl,
      Bool status
      ));
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_EMM_ANR_H__ */


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
