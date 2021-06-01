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

     Name:     LTE SON Application

     Type:     C include file

     Desc:     This file contains 
               

     File:     nl.x

     Sid:      nl.x@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:53:08 2014

     Prg:      subhamay 

*********************************************************************21*/

#ifndef NL_X_
#define NL_X_

/** 
 * @structure nlLmResponse 
 * structure holding Layer Manager Response information */
typedef struct nlLmResponse
{
   TranId   transId;           /*!< Transaction ID */
   Resp     response;          /*!< Response */
}NlLmResponse;

/**
 * @struct nlGenCb
   This structure contains informaton about the general configuratons of SON.*/
typedef struct nlGenCb
{
   NlLmResponse       CfmResp;          /*!< Respones value of Confirm */
   Status             memUpperThr;      /*!< upper threshold of the memory pool */
   Status             memLowerThr;      /*!< lower threshold of the memory pool */
   NlPciSelType       pciSelType;       /*!< whether PCI conflict/confusion detection enabled or not*/
   NlPrachSelType     prachSelType;     /*!< whether PRACH config enabled or not*/
   NlEarfcnSelType    earfcnSelType;    /*!< whether EARFCN config enabled or not*/
   NlTxPowerSelType   txPowerSelType;   /*!< whether Tx-Power config enabled or not*/
   U32                maxUeSupp;        /*!< maxmimum number of UE supported in eNB */
   U32                maxUePerTti;      /*!< maximum number of UE can be scheduled per TTI*/

}NlGenCb;

/**
 * @struct nlUiSapCb
   This structure contains informaton about NLU SAP configuratons of SON.*/
typedef struct nlUiSapCb
{
   Pst          pst;              /*!< post structure */
   SuId         suId;             /*!< service user Id */
   SpId         spId;             /*!< service provider Id */
   State        sapState;         /*!< SAP state (bound/unbound) */
   Mem          mem;              /*!< memory region, pool information */
   CmTimer      timer;            /*!< timer for bind */
   U8           bndRetryCnt;      /*!< number of bind retries */
   U8           maxBndRetry;      /*!< maximum number of bind retries */
   TmrCfg       bndTmr;           /*!< bind timer configurations */
} NlUiSapCb;

typedef enum nlAlarmId
{
   Nl_ALARM_CLEAR,
   NL_ALARM_PCI_CONFLICT,
   NL_ALARM_PCI_CONFUSION
}NlAlarmId;

typedef enum nlAlarmPeerType
{
   NL_ALARM_PEER_NONE,
   NL_ALARM_PEER_SERV_CELL,
   NL_ALARM_PEER_NGH_SERV_CELL,
   NL_ALARM_PEER_NGH_NGH_CELL
}NlAlarmPeerType;

typedef struct nlAlarmPeerInfo
{
   Bool              isActive;
   NlAlarmPeerType   peerType;
   NlAlarmId         alarmId;
   Void              *peerAddr;
}NlAlarmPeerInfo;

typedef struct nlAlarmInfo
{
   Bool              isAlarmRaised;
   U8                numPeerAlarm;
   NlAlarmPeerInfo   peerInfo[NL_MAX_NGH_ALARM];
}NlAlarmInfo;


/**
 * @struct nlPrachCfg
   This structure contains PRACH configuration informaton about cell control in SON module .*/
typedef struct nlPrachCfg
{
   U16       servRootSeqIdx;
   U8        servPrachCfgIdx;
   U8        servZeroCorrelationZoneCfg;
   U8        servPrachFreqOffset;
   Bool      highSpeedFlag;

   U8        numRootSeqIndex;
   U16       rootSeqIdx[LNL_MAX_ROOTSEQ_INDX];
   U8        numPrachCfgIdx;
   U8        prachConfigIdx[LNL_MAX_PRACH_CFG_IDX];
   U8        numZeroCorrCfgZone;
   U8        zeroCorrelationZoneCfg[LNL_MAX_ZERO_CORR_CFG_ZONE];
   U8        numFreqOffset;
   U8        freqOffset[LNL_MAX_NUM_FREQ_OFFSET];
}NlPrachCfg;

typedef enum nlBandwidth
{
   NL_BANDWIDTH_6,
   NL_BANDWIDTH_15,
   NL_BANDWIDTH_25,
   NL_BANDWIDTH_50,
   NL_BANDWIDTH_75,
   NL_BANDWIDTH_100
} NlBandwidth;


/**
 * @struct nlNhbrCellInfo
 * This structure contains all the neighbor cell informations in SON module */
typedef struct nlCellInfo
{
   NlAlarmInfo      alarmInfo;
   U16              pci;                /*!< physical cell Id */
   LnlEcgi          ecgi;               /*!< ECGI */
   U16              dlEarfcn;           /*!< downlink carrier frequency*/
   U16              ulEarfcn;           /*!< uplink carrier frequency*/
   Bool             isPrachCfg;         /*!< Indicates if PRACH Configuration is
                                       present or not */
   NlPrachCfg       prachCfg;           /*!< PRACH Configuration */
   Bool          isCsgCell;          /*!< whether this cell is CSG or not */
   U8            csgAccessMode;  /*!< Indicates the type of access mode the FAP operates in */
   U32           csgId;          /*!< Identity of the Closed Subscriber Group the cell belongs to. */
   U16           lteBand;        /*!< Band of the LTE cell */
   U16           tac;            /*!< Tracking Area Code */
   NlBandwidth   ulBw;           /*!< UL Bandwidth of the neighbor 0=6,1=15,2=25,3=50,4=75,5=100 */
   NlBandwidth   dlBw;           /*!< DL Bandwidth of the neighbor 0=6,1=15,2=25,3=50,4=75,5=100 */
   S8            RSTxPower;      /*!< The downlink reference-signal transmit power*/
   S16           rsrp;           /*!< RSRP in the range of -128 to 0 */
   S16           rssi;           /*!< RSSI in the range of 0 to 256 */
   Bool          isCellBarred;   /*!< Indicates if cell is barred or not */
   Bool             isCsgPciCfg;
   LnlCsgAccessMode accessMode;  
   U16              csgStartPci;
   U16              csgPciRange;
   Bool             isSmallCellPciCfg;
   U16              smallCellStartPci;
   U16              maxSmallCellPci;
}NlCellInfo;

/**
 * @struct nlNghNghCellCb
 * This structure contains all the neighbor's neighbor cell informations in SON module */
typedef struct nlNghNghCellCb
{
   CmLList          lnk;                /*!< list linker */
   NlCellInfo       nghNghCellInfo;
}NlNghNghCellCb;

typedef struct nlNghServCellCb
{
   CmLList          lnk;
   NlCellInfo       servCellInfo;
   CmLListCp        nghCellLst;
}NlNghServCellCb;

typedef struct nlNeighCellCb
{
   CmHashListEnt            neighEnbHashEnt;
   CmLListCp                servCellLst;
   U32                      enbId;  
} NlNeighCellCb;

typedef struct nlNeighEnbCb 
{
   CmHashListCp              nghEnbListCp;
}NlNeighEnbCb;

typedef enum nlCellEvent
{
   NL_CELL_EVENT_NGH_CFG = 0,    /* Neighbor configured through SM by OAM*/
   NL_CELL_EVENT_RECFG,      /* PCI reconfiguration as part of collision 
                             resolve/ dynamic PCI modification by OAM*/
   NL_CELL_UE_ADD,
   NL_CELL_UE_DELETE,
   NL_CELL_UE_MEAS_RPT,
   NL_CELL_UE_CQI_RPT,
   NL_CELL_UE_MODIFY,
   NL_CELL_EVENT_MAX
}NlCellEvent;

typedef enum nlCellState
{
   NL_CELL_STATE_INIT,
   NL_CELL_STATE_PCI_CONFLICT,
   NL_CELL_STATE_CONFIGURED,
   NL_CELL_STATE_PCI_MODIFY,
/* NL_CELL_STATE_WAIT_TXPOWER_CFG,*/
   NL_CELL_STATE_MAX
}NlCellState;

typedef enum nlVendorDSonEvent
{
   NL_VENDOR_DSON_EVENT_CELL_CFG,
   NL_VENDOR_DSON_EVENT_TX_POWER_CFG,
   NL_VENDOR_DSON_EVENT_REPORT_CFG,
   NL_VENDOR_DSON_EVENT_IM_CFG,
   NL_VENDOR_DSON_EVENT_SON_STOP_CFM,
   NL_VENDOR_DSON_EVENT_MAX

}NlVendorDSonEvent;

typedef enum nlVendorDSonState
{
   NL_VENDOR_DSON_STATE_INIT = 0,
   NL_VENDOR_DSON_STATE_WAIT_CELL_CFG_CFM,
   NL_VENDOR_DSON_STATE_WAIT_TX_POWER_CFG,
   NL_VENDOR_DSON_STATE_CONFIGURED,
   NL_VENDOR_DSON_STATE_WAIT_REPORT_CFG_REQ,
   NL_VENDOR_DSON_STATE_MAX
}NlVendorDSonState;

/** 
   * Ng values for PHICH For more details refer to 36.211 Sec 6.9*/
typedef enum nlPhichNg
{
   NL_NG_ONESIXTH, /**< PHICH Ng Values 1/6 */
   NL_NG_HALF,     /**< PHICH Ng Values 1/2 */
   NL_NG_ONE,      /**< PHICH Ng Values 1 */
   NL_NG_TWO       /**< PHICH Ng Values 2 */
}NlPhichNg;


/**
 * @struct zeroCorrerelationConfigZoneType
   This structure contains Mapping information of Ncs as per 36.211, Table 5.7.2-2 
   along with calculation of cyclic shift (36.211, section 5.7.2) */
typedef struct zeroCorrerelationConfigZoneType
{
   U16 prachNcsVal;
   U16 prachNumCyclicShifts;
   U16 numRootSeqIndex; 
} ZeroCorrerelationConfigZoneType;


/** 
 * @brief
   This structure holds PUCCH configuration for Serving Cell. Its used for
   Prach Freq-Offset calculation. */
typedef struct nlPucchCfg
{
   U8          pucchCqiResIdx;             /*!< N2-PUCCH */
   U16         n1PucchAn;                  /*!< N1-PUCCH */
   U8          deltaShift;                 /*!< Delta Shift for PUCCH: a value
                                             in set {1,2,3} */ 
   U8          cyclicShift;                /*!< Cyclic Shift for PUCCH (
                                             N^(1)_CS): range [0-7] */

   /* This will be done during initialization */
   U8          mappingNpwithP[NL_TDD_MAX_P_PLUS_ONE_VAL];   /*!<Mapping Np with P */
}NlPucchCfg;


/** 
 * @brief
   This structure holds PHICH configuration for Serving Cell. Its used for
   Prach Freq-Offset calculation. */
typedef struct nlPhichCfg
{
   NlPhichNg    ngEnum;        /*!< Ng value for PHICH */
   Bool         isDurExtend;   /*!< PHICH Duration:
                                  TRUE-extended/FALSE-normal */
}NlPhichCfg;

/** 
 * @brief
   This structure PCI update infor as part of cell config and cell reconfig.*/
typedef struct nlPciModAlarmInfo
{
   U16            oldPci;       /*!< Store the previous PCI value before modification*/
   Bool           isAlarmSend;  /*!< Indicate whether alarm raised for this PCI modification*/
   LnlDgnType     alarmType;    /*!< alarm type if raised*/
   LnlDgnAlarm    alarmId;      /*!< alarm Id if raised*/
}NlPciModAlarmInfo;

typedef struct nlTmpNghCellModInfo
{
   LnlEcgi          oldEcgi;
   NlCellInfo       nghCellInfo;
   U8               numNghNgh;
   NlCellInfo       nghNghCellInfo[LNL_MAX_NGH_NGH_CELL_INFO];
}NlTmpNghCellModInfo;

typedef struct nlTmpNghCellMod
{
   U8                    numNgh;
   NlTmpNghCellModInfo   nghCellModInfo[LNL_MAX_NGH_CELL_INFO];

}NlTmpNghCellMod;

typedef struct nlTmpNghCellDel
{
   U8                numNgh;
   LnlEcgi           ecgi[LNL_MAX_NGH_CELL_INFO];
}NlTmpNghCellDel;


typedef struct NlTmpNghCellAddInfo
{
   NlCellInfo      nghCellInfo;
   U8              numNghNgh;
   NlCellInfo      nghNghCellInfo[LNL_MAX_NGH_NGH_CELL_INFO];
}NlTmpNghCellAddInfo;

typedef struct nlTmpNghCellAdd
{
   U8                    numNgh;
   NlTmpNghCellAddInfo   nghCellAddInfo[LNL_MAX_NGH_CELL_INFO];
}NlTmpNghCellAdd;

typedef enum nlTmpNghCfgType
{
   NL_NGH_ADD,
   NL_NGH_DEL,
   NL_NGH_MOD
}NlTmpNghCfgType;

typedef struct nlTmpNghCellCfgParam
{
   NlTmpNghCfgType      cfgType;
   union
   {
      NlTmpNghCellAdd   nghCellAdd;
      NlTmpNghCellDel   nghCellDel;
      NlTmpNghCellMod   nghCellMod;
   }u;
}NlTmpNghCellCfgParam;

typedef struct nlTmpNghEnbCellCfg
{
   U32                  nghEnbId;
   NlTmpNghCellCfgParam nghCellParam;
}NlTmpNghEnbCellCfg;

typedef struct nlTmpNghCellCfg
{
   U32                  cellId;
   U8                   numNghEnb;
   NlTmpNghEnbCellCfg   nghEnbCellCfg[LNL_MAX_NGH_ENB_CFG];
}NlTmpNghCellCfg;

typedef struct nlNghCfgPendMsg
{
   CmLList           link;
   NlTmpNghCellCfg   nghCellCfg; 
}NlNghCfgPendMsg;

typedef struct nlCellReCfgPendMsg
{
   CmLList          link;
   LnlCellReCfg     cellReCfg;
}NlCellReCfgPendMsg;

typedef struct nlPciModPendMsg
{
   CmLListCp        nghCfgMsgList;   /*!< list of neighbor config message*/
   CmLListCp        cellReCfgMsgList;/*!< list of cell reconfig message*/
}NlPciModPendMsg;

typedef enum nlCellPciType
{
   NL_SELECTED_PCI_MOD3  = 3,
   NL_SELECTED_PCI_MOD6  = 6,
   NL_SELECTED_PCI_MOD30 = 30,
   NL_SELECTED_DEFAULT_PCI = 255
}NlCellPciType;

typedef struct nlTxPowerCfg
{
   U8       refSignalPwrCnt;                           /*!< Number Parameters to be configured for Reference signal power */
   S8       referenceSignalPwr[LNL_MAX_RS_TXPWR_CNT];  /*!< Array of Reference signal power {-60..50} */
   U8       pBCnt;                                     /*!< Num Parameters to be configured for pB */
   U8       pB[LNL_MAX_PB_CNT];                        /*!< Array of p-b values */
   U8       pACnt;                                     /*!< Num Parameters to be configured for pA */
   U8       pA[LNL_MAX_PA_CNT];                        /*!< Array of p-a values */
   U8       pschPowerOffCnt;                           /*!< Number of PschPowerOffset count */
   U8       sschPowerOffCnt;                           /*!< Number of sschPowerOffset count */
   U8       pbchPowerOffCnt;                           /*!< Number of PbchPowerOffset count */
   S8       pschPowerOffset[LNL_MAX_POWER_OFFSET_CNT]; /*!< power offset of PSCH w.r.t ReferenceSignalPower */
   S8       sschPowerOffset[LNL_MAX_POWER_OFFSET_CNT]; /*!< power offset of SSCH w.r.t ReferenceSignalPower */
   S8       pbchPowerOffset[LNL_MAX_POWER_OFFSET_CNT]; /*!< power offset of PBCH w.r.t ReferenceSignalPower */
}NlTxPowerCfg;

typedef struct nlTxPowerSel
{
   S8    refSignalTxPwr;        /*!< Selected Reference signal Tx Power */
   S16   pssPowerOffset;        /*!< PSS Power offset value */
   S16   sssPowerOffset;        /*!< PSS Power offset value */
   S16   pbchPowerOffset;       /*!< PBCH Power offset value */
   U8    pB;                    /*!< p-b values */
   S8    pout;                  /*!< pout. Max Tx Power */
}NlTxPowerSel;

/**
 * @struct nlcellCb
   This structure contains informaton about cell control block in SON module .*/
typedef struct nlCellCb
{
   CmHashListEnt     cellCbHashEnt;
   U32               cellId;
   U8                duplexMode;        /*!< TDD or FDD */
   NlCellState       state;             /*!< cell state*/
   NlCellInfo        cellInfo;          /*!< cell information*/
   U16               numNgh;            /*!< number of neighbor connection with this cell and same operator*/
   U32               numPci;            /*!< number of PCI in PCI List*/
   U16               pciList[LNL_MAX_PCI_CFG];  /*!< List of alternative PCI*/
   U32               selPciIdx;         /*!  selected PCI index value of PCI List*/
   U8                bandWidth;         /*!< Bandwidth */
   U32               bandWidthEnum;     /*!< Bandwidth in eNum Value*/
   U8                numTxAntPorts;     /*!< Num Antenna Prots */
   U8                subFrameCfg;       /*!< Sub-frame config */
   NlNeighEnbCb      neighEnbCb;        /*!< Neighbor eNodeB information*/
   NlPrachCfg        prachCfg;          /*!< PRACH Configuration */
   NlPucchCfg        pucchResCfg;       /*!< Pucch Resource Configuration
                                          for serving cell */
   Bool              isCpUlExtend;      /*!< Cyclic prefix :
                                          TRUE-extended/FALSE-normal */
   NlPhichCfg        phichCfg;          /*!< PHICH Configuration */
   NlPciModAlarmInfo pciModAlarmInfo;  /*!< Indicated whether PCI modified during cell Cfg*/
   NlPciModPendMsg   pciModPendMsg;    /*!< List to store message received from SM during 
                                            ongoing PCI modification process*/
   Bool              isSmSonCfgDone;    /*!< This flag indicates whether SON configuration completed in SM, 
                                            this flag used to manupulate alarm for PCI conflict state*/
   NlTxPowerCfg      txPowerCfg;        /*!< Tx power configuration received from SM*/
   NlTxPowerSel      txPowerSel;        /*!< tx power configuration selected by BRDCM DSON*/
   NlVendorDSonState dSonState;         /*!< vendor specific DSON states*/ 
   S8                tgtRsrp;           /*!< Cell Edge target RSRP. Used by TPM module */
   U8                attnConstIdx;      /*!< Attenuation factor gamma
                                             Refer nl_rsystpm.h */
   S8                rsysTpmRsrpMin;    /*!< Min RSRP value that is used by the 
                                             RSYS TPM module */ 
   S8                rsysTpmRsrpMax;    /*!< Max RSRP value that is used by the 
                                             RSYS TPM module */ 
}NlCellCb;

/**
 * @struct nlCb
   This structure contains informaton about the configuratons of SON.*/
typedef struct nlCb
{
   TskInit           init;              /*!< Task initialization structure */
   NlGenCb           genCfg;            /*!< SON general configuration */
   Header            lmHdr;             /*!< layer manager header information */
   NlUiSapCb         nluSap;            /*!< SON upper SAP towards eNB App information */
   CmHashListCp      cellCbList;        /*!< Cell control block*/
   CmTqCp            tqCp;              /*!< Timing Queue Control point */
   CmTqType          tq[NL_NUM_TQENTRY];   /*!< Timing Queue for a resln. */
   Ticks             timeRes;           /*!< Time resolution */
} NlCb;

typedef struct nlRemScanEarfcnSel
{
   S32  rssi;
   U8   dlEarfcnIdx;
}NlRemScanEarfcnSel;

typedef struct nlAlarmRaised
{
   NlAlarmPeerType   peerType;
   Void              *peerAddr;
   LnlUstaDgn        dgn;
}NlAlarmRaised;

typedef struct nlAlarm
{
   /*we can move this falg as bitmask as well TODO*/
   Bool              resolveDeferred;
   Bool              isAlarmRaised;
   Bool              isAlarmSend;
   Bool              isPciUpdated;       /*!< whether PCI is updated or not*/
   Bool              isAlarmCleared;
   U8                numAlarmCleared;
   U8                numAlarmRaised;
   NlAlarmRaised     alarmRaised[LNL_MAX_NGH_CELL_INFO];
   LnlUstaDgn        alarmCleared[LNL_MAX_NGH_CELL_INFO];/*!< diagonistics if alarm cleared*/
}NlAlarm;

typedef enum nlEventMsgType
{
   SON_CELL_RECFG,
   SON_NGH_CFG,
   SON_X2SETUP_REQ_NGH_CFG,
   SON_X2SETUP_RSP_NGH_CFG,
   SON_ENB_CFG_UPD_NGH_CFG,
   SON_ANR_NGH_CFG,
   SON_UE_ADD,
   SON_UE_DELETE,
   SON_UE_MODIFY,
   SON_UE_MEAS_RPT,
   SON_UE_CQI_RPT
}NlEventMsgType;



typedef struct nlEventMsg
{
  NlEventMsgType       msgType;
  Void                 *msg;
}NlEventMsg;

typedef struct nlVendorDSonEventMsg
{
   Void                *msg;
   U32                  msgType;
}NlVendorDSonEventMsg;

typedef S16 (*nlGlbStateMachine)(NlEventMsg * , NlCellCb *);
typedef S16 (*nlVendorDSonStateMachine)(NlVendorDSonEventMsg *, NlCellCb *);

typedef S16 (*NlVendorDSonDatIndFp)   ARGS((
        Pst                    *pst,           /* Post Structure */
        NlVendorDSonEventMsg   *msgInd         /* message structure */
     ));

/* LTE-SON Control Block */
EXTERN NlCb nlCb;


EXTERN nlGlbStateMachine nlGlbCellFSM[NL_CELL_STATE_MAX][NL_CELL_EVENT_MAX];
EXTERN nlVendorDSonStateMachine nlGlbVendorDSonFSM[NL_VENDOR_DSON_STATE_MAX][NL_VENDOR_DSON_EVENT_MAX];

EXTERN S16 nlActvTsk ARGS((
   Pst *pst, 
   Buffer *mBuf
));

EXTERN S16 nlSendPciUpdateInd ARGS ((
   NlCellCb                   *cellCb
));

EXTERN S16 nlSendEarfcnUpdateInd ARGS ((
   NlCellCb                   *cellCb
));

EXTERN S16 nlSendTxPwrUpdateInd  ARGS((
NlCellCb                 *cellCb  /* cell control block */
));

EXTERN Bool nlNghPciCollisionDetect ARGS((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg,  /* neighbor cell configuration*/
U32                msgTye,      /* type of message*/
NlAlarm           *alarm       /* alarm structure*/
));

EXTERN Bool nlNghPciConfusionDetect ARGS((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from eNB-APP/SM*/
NlAlarm           *alarm       /* alarm structure*/
));

EXTERN Bool nlNghAddPciConfusionDetect ARGS((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellAdd   *nghCellAdd, /* neighbor cell configuration structure*/
NlAlarm           *alarm       /* alarm structure*/
));

EXTERN Bool nlNghModPciConfusionDetect ARGS((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellMod   *nghCellMod, /* neighbor cell configuration structure*/
NlAlarm           *alarm       /* alarm structure*/
));

EXTERN S16 nlValidatePciEcgiWithNghCellInfo ARGS((
NlCellCb     *cellCb,  /* cell control block*/
U16          pci,      /* physical cell Id*/
LnlEcgi      *ecgi,    /* cell id and plmn id*/
U16          dlEarfcn, /* downlink carrier frequency*/
NlAlarm      *alarm    /* alarm information*/
));

EXTERN S16 nlNghPciInfoAdd ARGS((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from eNB-APP*/
NlAlarm           *alarm       /* alarm structure*/
));

EXTERN S16 nlNghPciInfoDelete ARGS((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB*/
NlAlarm           *alarm       /* alarm structure*/
));

EXTERN S16 nlNghPciInfoModify ARGS((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm       /* alarm structure*/
));


EXTERN S16 nlSendLnlAlarm  ARGS((
LnlUstaDgn                 *alarm  /* alarm structure */
));

EXTERN S16 nlInitNghCellCfgEvtHndlr ARGS((
NlEventMsg       *msg,         /* neighbor cell config structure  */
NlCellCb         *cellCb       /* cell control block*/
));

EXTERN S16 nlNghPciInfoUpdateInNghTbl ARGS ((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm       /* alarm structure*/
));

EXTERN S16 nlNghPciCollisionResolve   ARGS((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm       /* alarm structure*/
));

EXTERN S16 nlValidatePciWithNghCellInfo ARGS((
NlCellCb        *cellCb,  /* cell control block*/
U16             pci,      /* physical cell Id*/
U16             dlEarfcn, /* downlink carrier frequency*/
NlCellPciType   modType   /* PCI selection MOD type*/
));

EXTERN S16 nlStoreCellReCfgPciInfo   ARGS((
NlCellCb       *cellCb,
LnlCellReCfg   *cellReCfg,
U32            selPciIdx
));

EXTERN S16 nlConflictNghCellCfgEvtHndlr ARGS((
NlEventMsg       *msg,         /* neighbor cell config structure  */
NlCellCb         *cellCb       /* cell control block*/
));

EXTERN S16 nlConfigNghCellCfgEvtHndlr ARGS((
NlEventMsg       *msg,         /* neighbor cell config structure  */
NlCellCb         *cellCb       /* cell control block*/
));

EXTERN S16 nlConfigCellReCfgEvtHndlr ARGS((
NlEventMsg             *msg,         /* cell reconfig structure  */
NlCellCb               *cellCb       /* cell control block*/
));

EXTERN S16 nlConflictCellReCfgEvtHndlr ARGS((
NlEventMsg       *msg,         /* cell reconfig structure  */
NlCellCb         *cellCb       /* cell control block*/
));

EXTERN S16 nlProcessInvalidMsg ARGS((
NlEventMsg      *msg,    /* message structure  */
NlCellCb        *cellCb  /* cell control block*/
));

EXTERN S16 nlUpdatePciListWithSelPci   ARGS((
NlCellCb       *cellCb
));

EXTERN S16 nlStoreNghPeerAlarmInfo ARGS((
NlAlarmInfo     *alarmInfo,
LnlDgnAlarm     alarmId,
NlAlarmPeerType peerType,
Void            *peerAddr
));

EXTERN S16 nlNghPeerAlarmClear ARGS((
NlNghServCellCb *nghServCell
));

EXTERN S16 nlNghNghPeerAlarmClear ARGS((
NlNghNghCellCb *nghNghCell
));

EXTERN S16 nlServCellPeerAlarmClear ARGS((
NlCellCb *servCell
));

EXTERN  S16 nlCellSelectPrach ARGS ((
NlCellCb        *cellCb,       /* cell control block*/
NlEventMsg      *evtMsg,       /* neighbor cell config structure */
NlAlarm         *alarm         /* alarm structure*/
));

EXTERN S16 nlSelectServCellPrachConfig ARGS((
NlEventMsg      *evtMsg,       /* neighbor cell config structure */
NlCellCb        *cellCb,       /* cell control block*/
NlAlarm         *alarm        /* alarm structure*/
));

EXTERN S16 nlSendPrachParamUpdateInd ARGS((
NlCellCb                 *cellCb  /* cell control block */
));

EXTERN S16 nlValidateCellPrachCfg ARGS((
LnlPrachCfg   *prachCfg   /* cell prach configuration */
));

EXTERN S16 nlValidateCellEarfcnCfg   ARGS((
LnlCellCfg    *cellCfg  /* cell config structure  */
));

EXTERN S16 nlValidateCellPciCfg   ARGS((
LnlCellCfg    *cellCfg
));

EXTERN S16 nlCellSelectEarfcn ARGS((
NlCellCb      *cellCb,   /* cell control block */
LnlCellCfg    *cellCfg  /* cell config structure  */
));

EXTERN S16 nlCellStorePciCfg   ARGS((
NlCellCb     *cellCb,
LnlCellCfg   *cellCfg
));
 
EXTERN S16 nlCellStorePrachCfg   ARGS((
NlCellCb     *cellCb,       /* cell control block*/
LnlCellCfg   *cellCfg       /* cell configuration */
));

EXTERN S16 nlCellSelectPci   ARGS((
NlCellCb          *cellCb,
NlTmpNghCellCfg   *nghCellCfg,
U32                msgType,
NlAlarm           *alarm
));

EXTERN S16 nlCellSelectTxPower  ARGS((
NlCellCb      *cellCb   /* cell control block */
)); 

EXTERN S16 nlHandleCellSelectedTxPower  ARGS((
NlCellCb         *cellCb       /* cell control block*/
));  

EXTERN S16 nlPciModifyCellReCfgEvtHndlr ARGS((
NlEventMsg   *evtMsg,   /* msg structure  */
NlCellCb     *cellCb    /* cell control block*/
));

EXTERN S16 nlPciModifyNghCellCfgEvtHndlr ARGS((
NlEventMsg   *evtMsg,   /* msg structure  */
NlCellCb     *cellCb    /* cell control block*/
));

EXTERN S16 nlUpdateNghNghCellAlarmInfo   ARGS((
NlNghNghCellCb *nghNghCellCb,
NlAlarm         *alarm
));

EXTERN S16 nlUpdateNghCellAlarmInfo   ARGS((
NlNghServCellCb *nghCellCb,
NlAlarm         *alarm
));

EXTERN S16 nlNghPciCollisionResolveWithNghDelOrMod ARGS((
NlCellCb          *cellCb,     /* cell control block*/
NlTmpNghCellCfg   *nghCellCfg, /* neighbor cell configuration from SM/eNB-APP*/
NlAlarm           *alarm       /* alarm structure*/
));

EXTERN S16 nlCellSelectTxPowerByDSon   ARGS((
NlCellCb          *cellCb   /* cell control block */
)); 

EXTERN S16 nlCellStoreTxPowerCfg   ARGS((
NlCellCb     *cellCb,
LnlCellCfg   *cellCfg
));

EXTERN Void SendToRRM   ARGS((
Void     *msg    /* message received from DSON */
));

EXTERN Void FreeByRRM   ARGS((
Void     *msg
));

EXTERN S16 nlUnpkVendorDSonDatInd   ARGS((
NlVendorDSonDatIndFp func,    /* primitive to call after unpacking */
Pst                  *pst,    /* post structure */
Buffer               *mBuf    /* message buffer which will be unpacked */
));

EXTERN S16 nlVendorDSonConnEstReq   ARGS((
Void
));

EXTERN S16 NlVendorDSonDatInd   ARGS((
Pst                   *pst,      /* post structure  */
NlVendorDSonEventMsg  *evtMsg    /* message structure from DSON */
));

EXTERN S16 nlVendorDSonCellCfgReq  ARGS((
Void                  *cellCfgReq
));

EXTERN S16 nlVendorDSonCellTxPwrCfgCfm  ARGS((
Void                  *txPwrCfm
));

EXTERN S16 nlVendorDSonRemResultInd   ARGS((
Void    *remInd
));

EXTERN S16 nlBuildRemResultInd    ARGS((
NlCellCb               *cellCb
));

EXTERN S16 nlRegInitTmr   ARGS((
Void
));

EXTERN S16 nlActvTmr   ARGS((
Ent    ent,
Inst   inst
));

EXTERN Void nlStopTmr   ARGS((
PTR   cb,
S16   event
));
/* Periodic REM fro TPM */
EXTERN S16 nlHdlPeriodicRemCellSrchRsp  ARGS((
NluPeriodicRemCellSearchRsp  *cellSrchRsp   /* cell Info */
));

EXTERN S16 nlBuildPeriodicRemResultInd  ARGS((
NluPeriodicRemCellSearchRsp  *cellSrchRsp
));

/* Periodic REM fro TPM End */

EXTERN Void nlStartTmr   ARGS((
PTR   cb,
S16   tmrEvnt,
U32   delay
));

EXTERN S16 nlSendSonREMScanReq   ARGS((
U16        remScanInterval, /*!< REM Scan interval*/
U16        remScanCount,    /*!< REM Scan count*/
NlCellCb   *cellCb          /*!< cell control block*/
));

EXTERN Void nlUtlFindStrongestNghCell ARGS((
NlCellCb          *cellCb,    /* cell control block */
NlNghServCellCb   **strngstNghCell /* Strongest Neighbour cell */
));

EXTERN S16 nlShutDownVendorDSon ARGS((
Void
));

EXTERN Void nlStopTimers   ARGS((
Void
));

EXTERN S16 nlFillBwConfig   ARGS((
U8 dlbW
));

EXTERN Void nlInitSonDB ARGS((
Void
));

EXTERN S16 nlReportNumNghToSon ARGS((
U16   numNgh
));

EXTERN S16 nlConfigUeAddEvtHndlr   ARGS((
NlEventMsg       *evtMsg,      /*!< UE added message structure  */
NlCellCb         *cellCb       /*!< cell control block*/
));

EXTERN S16 nlConfigUeModEvtHndlr   ARGS((
NlEventMsg       *evtMsg,      /*!< UE modify message structure  */
NlCellCb         *cellCb       /*!< cell control block*/
));

EXTERN S16 nlConfigUeDeleteEvtHndlr   ARGS((
NlEventMsg       *evtMsg,      /*!< UE deleted message structure  */
NlCellCb         *cellCb       /*!< cell control block*/
));

EXTERN S16 nlConfigUeMeasRptEvtHndlr   ARGS((
NlEventMsg       *evtMsg,      /*!< UE measurement report message structure */
NlCellCb         *cellCb       /*!< cell control block*/
));

EXTERN S16 nlConfigUeCqiRptEvtHndlr   ARGS((
NlEventMsg       *evtMsg,      /*!< UE CQI report message structure */
NlCellCb         *cellCb       /*!< cell control block*/
));

EXTERN Void nlProcTmr   ARGS((
Ptr    cb,
S16    event
));

EXTERN S16 nlSendSonSchdCfgInd   ARGS((
LnlSonSchdCfg    *schdCfg,
NlCellCb         *cellCb
));

EXTERN S16 nlSendSonMeasRptIntervalInd   ARGS((
U32        ueMeasInterval,  /*!< UE measurement reporting interval*/
NlCellCb   *cellCb          /*!< cell control block*/
));


#endif /* NL_X_ */
/********************************************************************30**

           End of file:     nl.x@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:53:08 2014

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
*********************************************************************91*/

