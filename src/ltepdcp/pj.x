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
  
        Name:    LTE RLC file 
    
        Type:    C include file
  
        Desc:    This file contains all the data structures and 
                 prototypes for LTE RLC.
 
        File:    pj.x

        Sid:      pj.x@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:43:08 2014
        Sid:      pj.x@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:43:08 2014
  
        Prg:     N/A 
  
*********************************************************************21*/
/** @file pj.x
@brief RLC Product Structures, prototypes
*/

#ifndef __PJX__
#define __PJX__


/*
 *     The structures and variables declared in this file
 *     correspond to structures and variables used by
 *     the following TRILLIUM software:
 *
 *     part no.             description
 *     --------     ----------------------------------------------
 *     1000371      LTE-RLC
 *
*/
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef U16   PjSn;                        /*!< Sequence Number length */

/* pj005.201 added support for L2 Measurement */
/** @defgroup l2meas L2Meas Module Info 
*/
/** @addtogroup l2meas */
/*@{*/
#ifdef LTE_L2_MEAS

/** @struct PjL2Cntr
 * PDCP L2 Counter  */
typedef struct pjL2Cntr
{
   struct 
   {
       U32      numPkts; /*!< Number of packet monitired in dlDelay */        
       EpcTime  val;     /*!< L2 Measurement value in dlDelay */
   }dlDelay;
   struct
   {
       U32      numPkts; /*!< Number of packet monitired in ulLoss */        
       U32      val;     /*!< L2 Measurement value in ulLoss */
   }ulLoss;
   struct
   {
       U32      numPkts;  /*!< Number of packet monitired in ulLoss */        
       U32      val;     /*!< L2 Measurement value in dlDisc */
   }dlDisc;
   U32    totDrbsPerQci;     /*!< Total Count of DRB's for this QCI */
}PjL2Cntr;

/** @struct PjL2MeasCb
 * PDCP L2 Measurement CB */
typedef struct pjL2MeasCb
{
   U16           measType;               /*!< Bit-wise set measurement types */ 
#if 0 /*PRERIT - Not Required */
   CmLListCp     rbList;                 /*!< RB List maintained during measurements */       
#endif
   U16           numQci;                 /*!< number of valid qcI */            
   U8            qci[LPJ_MAX_QCI];       /*!< list of valid qcI */     
   PjL2Cntr      measData[LPJ_MAX_QCI];  /*!< Measurement CB */             
   CmLteMemInfo       memInfo;
   CmCpuStatsInfo  cpuInfo;
}PjL2MeasCb;

/** @struct PjL2MeasEvtCb
 * PDCP L2 Measurement Evt CB */
typedef struct pjL2MeasEvtCb
{
   U32           transId;  /*!< TransId of Measurement Req */
   U32           cbIdx;    /*!< TransId of Measurement Req */
   CmTimer       l2Tmr;    /*!< L2 Timer per request */
   TmrCfg        l2TmrCfg; /*!< Time period of measurement */
   PjL2MeasCb    measCb;   /*!< Measurement CB */ 
}PjL2MeasEvtCb;

/** @struct PjL2Cb
 * PDCP L2  CB */
typedef struct pjL2Cb
{
   U16           pjNumMeas;                       /*!< Number of measurements going on */
   PjL2MeasEvtCb pjL2EvtCb[PJ_MAX_L2MEAS_EVT]; /*!< Pointers to Measurement Cb */
   U8           measOn[LPJ_MAX_QCI];              /*!< Measurement on */
   U16           memUtilizationMask;             /*!< Memory mask */
   U16           cpuUtilizationMask;            /*!< CPU Mask */
   CmTimer      measTmr; /* !< measurement timer */ 
   TmrCfg       measTmrCfg; /*!< Time period of measurement */
}PjL2Cb;

/** @struct PjL2MeasRbCb
 * PDCP L2 Measurement Rb CB */
typedef struct pjL2MeasRbCb
{
#if 0
   U32           startCount;                /*!< start count for dl Delay */       
   CmLList       listNode[PJ_MAX_L2MEAS_INRB + 1];  /*!< list Entry for measurements */             
#endif
   PjL2Cntr      *l2Sts[PJ_MAX_L2MEAS_INRB + 1];    /*!< Dl Delay counter */     
}PjL2MeasRbCb;
#endif /* LTE_L2_MEAS */
/*@}*/

typedef struct pjCfmEnt
{
   U8             rbId;                  /*!< RB ID */
   U8             rbType;                /*!< RB Type */
   U8             cfgType;               /*!< CfgType for the Cfm */
   U8             res1;
   Status         status;                /*!< ROK or RFAILED */
   Reason         reason;                /*!< Reason for failure */
}PjCfmEnt;

/**
 *  @brief
 *  When the libraries are offboarded, the confirms are sent
 *  asynchronously. This is to store them.
 */
typedef struct pjAsyncCfm
{
   U8             cfmType;                  /*!< PJ_SEC_ASYNC_CFM, PJ_CFG_ASYNC_CFM, 
                                                 PJ_REEST_ASYNC_CFM, PJ_CFG_REEST_ASYNC_CFM
                                                 PJ_CFG_UEDEL_ASYNC_CFM */
   U8             startTmr;                 /*!< Whether to start the timer or not */
   U8             entity;                   /*!< Entity */
   U8             numEnt;                   /*!< Number of entities */
   U32            libInitBitMask;           /*!< Bitmask to maintain the cfm for Init req */
   U32            cmpInitBitMask;           /*!< Bitmask to track cfm for compression */
   CmLteRnti      ueId;                     /*!< UE ID */
   CmLteCellId    cellId;                   /*!< Cell ID */
   U32            transId;                  /*!< Transcation ID */
   PjCfmEnt       cfmEnt[CPJ_MAX_CFM];      /*!< Confirm parameter for
                                                 PDCP entity. */
}PjAsyncCfm;
/**
 *  @brief
 *  Hook information maintainned during processing
 */
typedef struct pjLibInfo
{
   U8         state;                        /*!< State of the UE- Normal or ReEst. */
   U8         res[1];                    
   U16        numReEstCmp;                  /*!< Number of RBs reestablishment received */
   U16        numReEstDrb;                /*!< Number of RBs under reestablishment*/
   CmTimer    obdTmr;                       /*!< Timer to wait for InitCfm and UE Delete */
   U32        tLibInitBitMask;              /*!< Bitmask to maintain the cfm for Init req */
   U32        tCmpInitBitMask;              /*!< Bitmask to track cfm for compression */
   PjAsyncCfm *asyncCfm[PJ_MAX_ASYNC_CFM];  /*!< Confirm list to handle multiple simultaneous
                                                 config requests when cfm is sent asynchronously */
   U16        nxtAvlbTxId;                  /*!< Next available transaction Id for entry 
                                                 in the async Cfm list */
   U16        crntTmrTxId;                  /*!< Transaction id for which the timer is started */
   Bool       onlySrb1;
   Bool       waitForReEstTrig;
#ifdef PJ_SEC_ASYNC
   Bool       reEstCfmSent;
#endif
}PjLibInfo;

/** @struct PjSecInp
Input parameters for security algorithm */
typedef struct pjSecInp
{
   U32         count;                       /*!< COUNT associated with SDU/PDU */ 
   CmLteRbId   rbId;                        /*!< RB Identifier */
   U8          dir;                         /*!< Direction UL/DL/biderectional */
   U8          res[2];                      /*!< reserved */
}PjSecInp;

/** @struct PjCmpCxt
Context for ROHC */
typedef struct pjCmpCxt
{
   CmLteCellId cellId;                      /*!< Cell Identifier */
   CmLteRnti   ueId;                        /*!< UE Identifier */
   CmLteRbId   rbId;                        /*!< RB Identifier */
   U8          rbType;                      /*!< RB Type */
   U16         cfgTxId;                     /*!< Tx Idx of config confirm */
}PjCmpCxt;

/** @struct PjSecCxt
Context for Security Algorithm */
typedef struct pjSecCxt
{
   CmLteCellId cellId;                      /*!< Cell Identifier */
   CmLteRnti   ueId;                        /*!< UE Identifier */
   U16         cfgTxId;                     /*!< Tx Idx of config confirm */
   U16         res;                         /*!< reserved */
   U8          dir;                         /*!< direction DL/UL */
}PjSecCxt;

/** @struct PjLibTrans
Transaction Id for Security Algorithm */
typedef struct pjLibTrans
{
   /* New parameters added from PjMsSecInfo to avoid multiple copy of parameters */
   CmLteCellId cellId;                      /*!< cell ID */
   CmLteRnti   ueId;                        /*!< UE ID */
   U8          pdcpInstance;                /*!< PDCP instance UL/DL */
   U8          hash_key[PJ_MS_KENB_LEN];    /*!< Current eNodeB key at App */
   U8          strS[PJ_MS_STR_S_LEN];
   U8          strSlen;
   U8          intAlgoType;                 /*!< Integ algo type  */
   U8          fresh[PJ_NUM_BYTES_FRESH];
   U8          ciphAlgoType;                /*!< Ciph algo type */
   U8          *key;                        /*!< Integrity key */
   U32         count;                       /*!< COUNT associated with SDU/PDU */
   CmLteRbId   rbId;                        /*!< PDCP RbCb */
   U8          rbType;                      /*!< RB Type : 0 - PJ_SRB, 1 - PJ_DRB */
   U16         res;                         /*!< Reserved */
   PTR         ueCb;                        /*!< UE control block */
   U8          dir;                          /*!< Direction */
   U8          snLen;                       /*!< SN Length */
   U32         macI;			            /*!< MACI for integrity verification */
   PTR         rxEnt;                       /*!< PDCP PDU receive entity */
   PTR         txEnt;                       /*!< PDCP PDU transmit entity */
}PjLibTrans;

/** @struct PjIntInfo
PDCP Integrity Algo Information */
typedef struct pjIntInfo
{
   U32          algoType;                    /*!< Type of the algorithm */
   U8          intKey[PJ_INTG_KEY_LEN];      /*!< Integrity key */
}PjIntInfo;

/** @struct PjCipherInfo
PDCP Ciphering Algo Information */
typedef struct pjCipherInfo
{
   U32          algoType;                    /*!< Type of the algorithm */
   U8          cpKey[PJ_CIPHER_KEY_LEN];    /*!< Control plane ciphering key */
   U8          upKey[PJ_CIPHER_KEY_LEN];    /*!< User plane ciphering key */
}PjCipherInfo;

/** @struct PjSec
PDCP Security Info */
typedef struct pjSec
{
   Bool          secAct;                    /*!< Flag to indicate if security is 
                                                 activated for this UE */ 
   Bool          selSecAct;                 /*!< Flag to indicate if selective 
                                                 security activation is set */ 
   Bool          firstMsg;                   /*!< Flag to indicate the first DL
                                                 SRB msg received */
   PjIntInfo     intInfo;                   /*!< Integrity Algo Information */
   PjCipherInfo  cipherInfo;                /*!< Ciphering Algo Information */
   PjSecCxt      secCxt;                    /*!< Context for security algorithm */

   Void          *intCxtId;                  /*!< Context Id for Integrity
                                                 protection / verification */
   Void          *cpCxtId;                   /*!< Context Id for cntrl plane 
                                                 ciphering / deciphering */
   Void          *upCxtId;                   /*!< Context Id for user plane
                                                 ciphering / deciphering */
}PjSec;

/** @struct PjBuf
PDCP Tx/Rx Buffer */
typedef struct pjBuf
{
   U8          numBins;                     /*!< Number of lists for datQ */   
   U8          res1[3];                     /*!< Reserved */
   U32         numEntries;                  /*!< Total number of entries in the datQ */   
   CmLListCp   *datQ;                       /*!< Array of lists for Tx SDUs/Rx PDUs */
   CmLListCp   datCfmQ;
   CmLListCp   sduSubmitQ;
   CmLListCp   reEstPktQ;
   CmLListCp   datPktQ;
#ifdef L2_L3_SPLIT
   U32         pduState[8];                 /*!< Added for debugging purpose */
#endif
   CmLList     *lastSntPktdatCfmQ;       /*!< ptr which stores the addres of the
                                         nxt node frm which abve "datCfmQ"
                                              list should be read */       
   CmLList     *lastSntPktsduSubmitQ;   /*!< ptr which stores the addres of the
                                         nxt node frm which abve "sduSubmitQ"
                                              list should be read */
   Bool        datCfmQRead;             /*!<indicates whether "datCfmQ" has 
                                         been read in pjDlmStartDataFrwdPerRb()*/
   Bool        sduSubmitQRead;        /*!<indicates whether "sduSubmitQ" has
                                         been read in pjDlmStartDataFrwdPerRb()*/
   U8          sndCnt;
   U16         pkTxCnt;
}PjBuf;

typedef struct pjUeKey
{
   CmLteRnti     ueId;                      /*!< UE Id */
   CmLteCellId   cellId;                    /*!< Cell Id */
}PjUeKey;

/** @struct PjCpjSapCb
 * CPJ Sap Control Block */
typedef struct pjCpjSapCb
{
   Pst           pst;                       /*!< Service user post structure */
   SpId          spId;                      /*!< Service provider Id */
   SuId          suId;                      /*!< Service user Id */
   U8            state;                     /*!< Sap Status */
   U8            res[3];                    /*!< reserved  */
}PjCpjSapCb;

/** @struct PjPjuSapCb
 * PJU Sap Control Block */
typedef struct pjPjuSapCb
{
   SpId          spId;                      /*!< Service provider Id */
   SuId          suId;                      /*!< Service user Id */
   U8            state;                     /*!< Sap Status */
   U8            res[3];                    /*!< Reserved */
   PjPjuSts      sts;                       /*!< Statistics */
   Pst           pst;                       /*!< Service user post structure */
}PjPjuSapCb;

typedef struct pjUdxDlSapCb
{
   SpId          spId;                      /*!< Service provider Id */
   SuId          suId;                      /*!< Service user Id */
   U8            state;                     /*!< Sap Status */
   U8            res[3];                    /*!< Reserved */
   PjPjuSts      sts;                       /*!< Statistics */
   Pst           pst;                       /*!< Service user post structure */
}PjUdxDlSapCb;


typedef struct pjUdxUlSapCb
{
   Pst           pst;                       /*!< Service user post structure */
   SpId          spId;                      /*!< Service provider Id */
   SuId          suId;                      /*!< Service user Id */
   U8            state;                     /*!< Sap Status */
   U8            retryCnt;                  /*!< Bind Retry Count */
   U16           bndTmrInt;                 /*!< Timer Interval */
   CmTimer       bndTmr;                    /*!< Bind Timer */
}PjUdxUlSapCb;


typedef struct pjKwuSapCb
{
   Pst           pst;                       /*!< Service user post structure */
   SpId          spId;                      /*!< Service provider Id */
   SuId          suId;                      /*!< Service user Id */
   U8            state;                     /*!< Sap Status */
   U8            retryCnt;                  /*!< Bind Retry Count */
   U16           bndTmrInt;                 /*!< Timer Interval */
   CmTimer       bndTmr;                    /*!< Bind Timer */
}PjKwuSapCb;

/** @struct PjHdrCmp
  Robust header compression Information */
typedef struct pjRohc
{
        Bool        hdrCmpUsed;                  /*!< Header Compression Used/Not */
        U8          res[3];                      /*!< reserved */
        U16         maxCid;                      /*!< Max CID */
        U16         profId;                      /*!< ROHC Profile Id */
}PjRohc; 
typedef struct pjCfgInfo
{
   CmHashListEnt     transHlEnt;
   U32               transId;
   U32               uprLyrTransId;
   U8                cfgType;
   CpjCfgReqInfo     *cfgInfo;
   CpjUeInfo         *ueInfo;
   CpjUeInfo         *newUeInfo;
   CpjSecCfgReqInfo  *secCfgInfo;
   CpjReEstReqInfo   *reEstReq;
   CpjCountReqInfo   *cntReq;
   CpjSduStaReqInfo  *staReq;
   CpjDatResumeReqInfo *datResReq;
} PjCfgInfo;

/** @struct PjPerfStats
PDCP performance stats */
typedef struct _pjPerfSts
{
   U32           pjSduRcvCnt;               /*!< Number of SDUs received at PDCP */
}PjPerfSts;


/** @struct PjCb
RLC global control block */
typedef struct _pjUlCb
{
   U32           transId;                   /*!< next available TransId */
   PjCpjSapCb    cpjSap;                    /*!< CPJ Sap Conrol Block */ 
   PjUdxUlSapCb  udxUlSap[PJ_MAX_UDXSAPS];  /*!< UDX Sap Control Block */
   PjPjuSapCb    pjuSap[PJ_MAX_PJUSAPS];    /*!< PJU Sap Conrol Block */ 
   PjKwuSapCb    kwuSap[PJ_MAX_KWUSAPS];    /*!< kwu Sap Control Block */
   CmHashListCp  transIdLstCp;              /*!< Hashlist of cfg trans */
   CmHashListCp  ueLstCp;                   /*!< Hashlist of UeCb */
#ifdef LTE_L2_MEAS
   PjL2Cb        pjL2Cb;             /*!< Control block of L2 Measurements in PDCP */
#endif /* LTE_L2_MEAS */
}PjUlgCb; 


/** @struct PjCb
RLC global control block */
typedef struct _pjDlCb
{
   PjUdxDlSapCb  udxDlSap[PJ_MAX_UDXSAPS];  /*!< Udx Sap Control Block */
   PjPjuSapCb    pjuSap[PJ_MAX_PJUSAPS];    /*!< PJU Sap Conrol Block */ 
   PjKwuSapCb    kwuSap[PJ_MAX_KWUSAPS];    /*!< kwu Sap Control Block */
   PjGenSts      pjGenSts;                  /*!< PDCP General Statistics */
   CmHashListCp  ueLstCp;                   /*!< Hashlist of UeCb */
#ifdef LTE_L2_MEAS
   PjL2Cb        pjL2Cb;             /*!< Control block of L2 Measurements in PDCP */
#endif /* LTE_L2_MEAS */
}PjDlgCb; 


/** @struct PjCb
RLC global control block */
typedef struct _pjCb
{
   CmTqType      pjTq[PJ_TMR_LEN];  /*!< Timer queue */
   CmTqCp        pjTqCp;            /*!< Timer queue control point */
   S16           trcLen;            /*!< Trace Length */
   U8            trcMask;           /*!< Trace Mask */
   TskInit       init;              /*!< Task Initialization Info */
   PjGenCfg      pjGenCfg;          /*!< PDCP General Configuration Structure */
   PjGenSts      pjGenSts;          /*!< PDCP General Statistics */
   PjPerfSts     pjPerfSts;         /*!< PDCP Performance Statistics */
   union
   {
      PjUlgCb      *ulCb;            /*!< Ul Control Block */ 
      PjDlgCb      *dlCb;            /*!< Dl Control Block */ 
   } u; 
}PjCb;

typedef struct _pdcpStats
{
    U32  numPdcpSdusDiscarded;
    U32  numPdcpSdusDiscMem;
}PDCPStats;

EXTERN PDCPStats gPdcpStats;

/* Extern variable declaration */
EXTERN PjCb *pjCb[PJ_MAX_PDCP_INSTANCES];  /*!< PDCP global control block */ 

/* TMR Module */
EXTERN S16 pjStartTmr  ARGS ((
PjCb     *gCb,
PTR       cb,           
S16       tmrEvnt   
));

EXTERN S16 pjStopTmr ARGS ((
PjCb  *gCb,
PTR    cb,              
U8     tmrType         
));

EXTERN Bool pjChkTmr ARGS ((
PjCb     *gCb,
PTR       cb,         
S16       tmrEvnt       
));

EXTERN S16 pjTmrExpiry ARGS ((
PTR cb,
S16 tmrEvnt
)); 
EXTERN S16 pjDbmBufInit ARGS ((
PjCb     *gCb,
PjBuf       *buf,          
U8          numBins          
));

EXTERN S16 pjLmmSendTrc ARGS ((PjCb *gCb, Event event, Buffer *mBuf));
#ifdef LTE_L2_MEAS
EXTERN Void pjLmmSendAlarm ARGS ((PjCb *gCb, U16 category, U16 event, 
U16 cause, SuId suId, U32 ueId, U8 qci));
#else /* LTE_L2_MEAS */
EXTERN Void pjLmmSendAlarm ARGS ((PjCb *gCb, U16 category, 
                                 U16 event, U16 cause, 
                                   SuId suId, U32 ueId));
#endif /* LTE_L2_MEAS */
EXTERN S16 pjUtlCmpClose ARGS((
PjCb    *gCb,
PTR      cmpCxtId                 /* ROHC Context to be closed */
));

#if 0
EXTERN S16 pjUtlIntClose ARGS((
PjCb    *gCb,
PTR      intCxtId                 /* Integration CxtId to be closed */
));
EXTERN S16 pjUtlCipherClose ARGS((
PjCb    *gCb,
PTR      cpCxtId                  /* Context Id for Ciphering to be closed */
));
#endif


/* Activation functions */
EXTERN S16 pjDlActvInit ARGS ((
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
));

EXTERN S16 pjDlActvTsk ARGS ((
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
));

/* Activation functions */
EXTERN S16 pjUlActvInit ARGS ((
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
));

EXTERN S16 pjUlActvTsk ARGS ((
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
));

EXTERN U32 pjMsSpaccDeInitDeCipherQ ARGS ((Void));
EXTERN U32 pjMsSpaccDeInitCiphQ ARGS ((Void));
EXTERN U32 pjMsCheckSpaccQueue ARGS ((Bool isUl));
EXTERN U32  pjQcomGetCryptoCoreId ARGS ((PjLibTrans *libTrans));

EXTERN Void pjProcessSpaccRdWr ARGS ((
U32 ttitick
));

#ifdef LTE_L2_MEAS
EXTERN Void UpdateMemInfo ARGS ((
PjL2Cb *tPjL2Cb /* l2Cb structure */
));
EXTERN S16 UpdateCpuInfo ARGS ((
PjL2Cb *tPjL2Cb /* l2Cb structure */
));

EXTERN S16 pjUtlMemCpuHdlTmrExp ARGS((
PjCb          *tPjCb,/*pjCb structure */
PjL2Cb *tPjL2Cb /*l2Cb structure */
));
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PJX__ */

  
/********************************************************************30**
  
         End of file:     pj.x@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:43:08 2014
  
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
/main/1      ---       nm  1. Initial release.
*********************************************************************91*/
