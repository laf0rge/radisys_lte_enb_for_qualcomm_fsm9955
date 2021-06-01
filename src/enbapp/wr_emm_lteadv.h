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

     Name:     EnodeB LTE-Advanced Application

     Type:     C include file

     Desc:     This file contains 
               
     File:     wr_emm_lteadv.h

     Sid:      wr_emm_lteadv.h@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB_GA/9 - Mon Feb 15 12:50:58 2016

     Prg:      skulal

*********************************************************************21*/


#ifndef __WR_EMM_LTEADV_H__
#define __WR_EMM_LTEADV_H__ 1

/* LTE_ADV starts */
/* LTE_ADV_ABS starts */

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                        EMM ABS MODULE - START                    %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*********************************************************************/
/*                      ABS MODULE MACRO                             */
/*********************************************************************/
#define WR_EMM_ABS_LOAD_PERIODICITY          1000
#define WR_EMM_ABS_PATTERN_SIZE              5
#define WR_EMM_CELL_MAX_ABS_PATTERN          25
#define WR_EMM_ABS_RPT_CHARACTERISTICS       0x08000000
#define WR_EMM_ABS_LOAD_INFO_RETRY_TIMER_VAL 5000 


#define LTE_ADV_MAX_NUM_NIL_RSRC_UPD 10

#define LTE_ADV_ABS_OFFSET           0

/*ABS_TESTING*/
#define WR_EMM_ABS_PERCENTAGE_0               0
#define WR_EMM_ABS_PERCENTAGE_25             25
#define WR_EMM_ABS_PERCENTAGE_50             50
#define WR_EMM_ABS_PERCENTAGE_75             75
/*ABS_TESTING*/

#define WR_EMM_ABS_FILL_STRBSXL(_pdu, _dstStrBsXL, _src, _len){\
    cmGetMem(_pdu, _len, (Ptr*)&_dstStrBsXL.val);\
    cmMemcpy(_dstStrBsXL.val, _src, _len);\
    _dstStrBsXL.len = _len*8;\
    _dstStrBsXL.pres = PRSNT_NODEF;\
}\

#define WR_EMM_ABS_GET_CELLID(_absCellId, _cellId){\
   wrEmmAbsGetCellId(&_absCellId->cId, &_cellId->eUTRANcellIdentifier);\
   wrEmmLteAdvNghEnbParsePlmnIe(&_cellId->pLMN_Identity, &_absCellId->plmnId);\
}\

#define WR_GET_ABS_CELLCB(_cellCb){\
   U32 _idx;\
   _cellCb = NULLP;\
   for(_idx = 0; _idx < WR_EMM_MAX_CELLS; _idx++) {\
      if (wrEmmCb.cellCb[_idx] != NULLP) {\
            _cellCb = wrEmmCb.cellCb[_idx];\
      }\
   }\
}
/* KW fix for LTE_ADV */
#define WR_UMM_GET_ABS_CELLCB(_cellCb){\
   WrCellCb *_emmCellCb = NULLP ;\
   WR_GET_ABS_CELLCB(_emmCellCb)\
    if(_emmCellCb != NULLP) {\
        WR_UMM_GET_CELLCB(_cellCb, _emmCellCb->cellId)\
    }\
}

#define WR_EMM_ABS_CONVERT_NEGSEC_ENUM(_sec, _enum)\
{\
   if(_sec <= 1)\
   {\
      _enum = CztRprtngPeriodicityone_thousand_msEnum;\
   }\
   else if(_sec <= 2)\
   {\
      _enum = CztRprtngPeriodicitytwo_thousand_msEnum;\
   }\
   else if(_sec <= 5)\
   {\
      _enum = CztRprtngPeriodicityfive_thousand_msEnum;\
   }\
   else\
   {\
      _enum = CztRprtngPeriodicityten_thousand_msEnum;\
   }\
}

#define WR_EMM_ABS_CONVERT_ENUM_NEGSEC(_enum, _sec)\
{\
   switch(_enum)\
   {\
      case CztRprtngPeriodicityone_thousand_msEnum:\
      {\
         _sec = 1;\
         break;\
      }\
      case CztRprtngPeriodicitytwo_thousand_msEnum:\
      {\
         _sec = 2;\
         break;\
      }\
      case CztRprtngPeriodicityfive_thousand_msEnum:\
      {\
         _sec = 5;\
         break;\
      }\
      case CztRprtngPeriodicityten_thousand_msEnum:\
      {\
         _sec = 10;\
         break;\
      }\
      default:\
      {\
         _sec = 10;\
         break;\
      }\
   }\
}
/* KW fix for LTE_ADV */
#define WR_EMM_ABS_CONVERT_NEGSEC_COUNT(_sec, _count)\
{\
   _count = (U8)((_sec*1000)/WR_EMM_ABS_LOAD_PERIODICITY);\
   if(0 == _count)\
   {\
      _count = 1;\
   }\
}
/* KW fix for LTE_ADV */
#define WR_EMM_ABS_GET_UECB(_ueId, _ueCb)\
{\
    U16                    _ueCnt = 0;\
    WrUmmCellCb            *_ummCellCb = NULLP ;\
    WR_UMM_GET_ABS_CELLCB(_ummCellCb);\
    if(_ummCellCb != NULLP)\
    {\
      while(_ueCnt < _ummCellCb->maxUesPerCell)\
      {\
         _ueCb = _ummCellCb->ueCbs[_ueCnt++];\
         if(_ueCb == NULLP)\
         {\
            continue;\
         }\
         if(_ueCb->crnti == _ueId)\
         {\
            break;\
         }\
         else\
         {\
            _ueCb = NULLP;\
         }\
        break;\
     }\
  }\
}

/*********************************************************************/
/*  ABS MODULE EVENTS & STATES                                       */
/*********************************************************************/
/**
  @brief Events of ABS Module */
typedef enum _wrEmmAbsEventEnum
{
   WR_EMM_ABS_CELL_INIT_EVENT,         /* 0-Cell Initialization */
   WR_EMM_ABS_ABS_UE_EVENT,            /* 1-ABS UE decision */
   WR_EMM_ABS_NONABS_UE_EVENT,         /* 2-Non ABS UE decision */
   WR_EMM_ABS_LOAD_INFO_RCVD_EVENT,    /* 3-Reception of Load Indication */
   WR_EMM_ABS_RSRC_STS_REQ_RCVD_EVENT, /* 4-Reception of Resource Status Request */
   WR_EMM_ABS_RSRC_STS_RSP_RCVD_EVENT, /* 5-Reception of Resource Status Response */
   WR_EMM_ABS_RSRC_STS_FLR_RCVD_EVENT, /* 6-Reception of Resource Status Failure */
   WR_EMM_ABS_RSRC_STS_UPD_RCVD_EVENT, /* 7-Reception of Resource Status Update */
   WR_EMM_ABS_X2AP_UP_IND_EVENT,       /* 8-Peer Up Indication X2AP */
   WR_EMM_ABS_X2AP_DOWN_IND_EVENT,     /* 9-Peer Down Indication X2AP */
   WR_EMM_ABS_CELL_UP_IND_EVENT,       /* 10-Cell Down Indication */ 
   WR_EMM_ABS_CELL_DOWN_IND_EVENT,     /* 11-Cell Up Indication */
   WR_EMM_ABS_SCH_LOAD_IND_EVENT,      /* 12-ABS Load Information from MAC Scheduler */
   WR_EMM_ABS_ENABLED_EVENT,           /* 13-Dynamic ABS feature enabling */
   WR_EMM_ABS_DISABLED_EVENT,          /* 14-Dynamic ABS feature disabling */
   WR_EMM_ABS_UE_REL_IND_EVENT,        /* 15-UE Release Indication */
   WR_EMM_ABS_TIMER_EVENT,             /* 16-Timer Expiry */
   WR_EMM_ABS_ERROR_IND_EVENT,         /* 17-Error Indication */
  
   WR_EMM_ABS_MAX_EVENT                 
} WrEmmAbsEventEnum;

/**
  @brief States of ABS Module */
typedef enum _wrEmmAbsState
{
   WR_EMM_ABS_DISABLED_STATE,          /* ABS is disabled */
   WR_EMM_ABS_ENABLED_STATE,           /* ABS is enabled */

   WR_EMM_ABS_MAX_STATE                
} WrEmmAbsState;

/**
  @brief Type of ABS Negotiation */
typedef enum _wrEmmAbsNegType
{
   WR_EMM_ABS_STATIC_NEG_TYPE,          /* ABS static negotiation */
   WR_EMM_ABS_DYNAMIC_NEG_TYPE,         /* ABS dynamic negotiation */

   WR_EMM_ABS_MAX_NEG_TYPE
} WrEmmAbsNegType;


/*********************************************************************/
/*  ABS MODULE COMMON DATA STRUCTURE                                 */
/*********************************************************************/
typedef enum _wrEnumAbsPatternSelectFlrCause
{
   WR_EMM_ABS_DUP_PATTERN, /* Same as currently used pattern is selected */
   WR_EMM_ABS_NO_PATTERN,  /* No pattern present */
   WR_EMM_ABS_NULL_PATTERN /* Pattern with zero percentage pattern present */
} WrEnumAbsPatternSelectFlrCause;

typedef struct _wrEmmAbsEnbId
{ 
   U32                          val;
} WrEmmAbsEnbId;

typedef struct _wrEmmAbsCellId
{ 
   U32                          cId;
   WrPlmnId                     plmnId;
} WrEmmAbsCellId;

typedef struct _wrEmmAbsCellInfo
{
   WrEmmAbsCellId               cellId;
   WrEmmAbsEnbId                enbId;
} WrEmmAbsCellInfo;

typedef struct _wrEmmAbsCellInfoLstItem
{
   CmLList                      lnk;
   WrEmmAbsCellInfo             cellInfo;
} WrEmmAbsCellInfoLstItem;

/*********************************************************************/
/*               ABS MODULE INTERFACE DATA STRUCTURE                 */
/*********************************************************************/
/**
  @brief ABS module interface structure to receive cell related event */
typedef struct _wrEmmAbsCellEvent
{
   U32                          cellId;
} WrEmmAbsCellEvent;

/**
  @brief ABS module interface structure to receive X2AP event */
typedef struct _wrEmmAbsX2apEvent
{
   U32                          peerId;
   CztEvnt                      *pdu;
} WrEmmAbsX2apEvent;

/**
  @brief ABS module interface structure to receive scheduler event */
typedef struct _wrEmmAbsSchEvent
{
   RgrLoadInfIndInfo            *schLoadInfo;
} WrEmmAbsSchEvent;

/**
  @brief ABS module interface structure to receive UE info*/
typedef struct _wrEmmAbsUeInfo
{
   U32                          ueId;
   U32                          cellId;
   U8                           numOfCell;
   WrEmmAbsCellInfo             cellInfo[8];
} WrEmmAbsUeInfo;

/**
  @brief ABS module interface structure to receive UE release indication */
typedef struct _wrEmmAbsUeRelInfo
{
   U32                          ueId;
} WrEmmAbsUeRelInfo;

/**
  @brief ABS module interface structure to receive ABS configuration from SM */
typedef struct _wrEmmAbsCfgEvent
{
   CmLteCellId                  cellId;
   LwrLteAdvancedConfigTrans    *lteAdvTrans;                          
} WrEmmAbsCfgEvent;      /*KW fix for LTE_ADV */

/**
  @brief ABS module interface structure to receive error indication event */
typedef struct _wrEmmAbsErrorIndEvent
{
   U32                          event; 
   PTR                          cb;
} WrEmmAbsErrorIndEvent;

/**
  @brief ABS module interface structure to receive timer expiry event */
typedef struct _wrEmmAbsTimerExpiryEvent
{
   U32                          event;
   PTR                          cb;
} WrEmmAbsTimerExpiryEvent;


/**
  @brief ABS module interface structure to receive all events */
typedef struct _wrEmmAbsEvent
{
   WrEmmAbsEventEnum            eventId; 
   union
   {
      WrEmmAbsCellEvent         cellEvent;
      WrEmmAbsX2apEvent         x2apEvent;
      WrEmmAbsSchEvent          schLoadInfoEvent;
      WrEmmAbsUeInfo            absUeEvent;
      WrEmmAbsUeRelInfo         ueRelEvent; 
      WrEmmAbsCfgEvent          absCfgEvent;
      WrEmmAbsErrorIndEvent     errIndEvent;
      WrEmmAbsTimerExpiryEvent  timerEvent;
   } u;   
} WrEmmAbsEvent;

/*********************************************************************/
/*            DATA STRUCTURES OF ABS MODULE(MACRO)                   */
/*********************************************************************/

typedef struct _wrEmmLteAdvAbsAggrPeerEnbInfo
{
   CmLListCp                    absPeerEnbLstCp;
} WrEmmLteAdvAbsAggrPeerEnbInfo;

typedef struct _wrEmmAbsAggrPeerEnbCb
{             
   CmLList                      lnk;
   WrEmmAbsEnbId                enbId;              
   U32                          enb1MeasId;    /* KW fix for LTE_ADV */
   U32                          enb2MeasId;
   CmLListCp                    absPeerCellLstCp;
} WrEmmAbsAggrPeerEnbCb;

typedef struct _wrEmmAbsAggrPeerCellCb
{
   CmLList                      lnk;
   WrEmmAbsCellId               cellId;       
   U8                           usablePatternId;
   U8                           absPercentageRsrcUsed;
} WrEmmAbsAggrPeerCellCb;

typedef struct _wrEmmAbsAggrPatternCfg
{
    U8                          absPattern[WR_EMM_ABS_PATTERN_SIZE];
    U8                          absPercentage;
    U32                         absWeightage;
} WrEmmAbsAggrPatternCfg;

typedef struct _wrEmmAbsAggrPatternCfgTable
{  
    U8                          absPatternCount;
    WrEmmAbsAggrPatternCfg      absPatternCfg[WR_EMM_CELL_MAX_ABS_PATTERN];
} WrEmmAbsAggrPatternCfgTable;

typedef struct _wrEmmLteAdvAbsAggrCellInfo
{
   WrEmmAbsAggrPatternCfgTable  absPatternCfgTable;
   U8                           absMinRsrc;   
   U8                           absMaxRsrc;
   U8                           absPatternId;
   U8                           absTotalRsrcAvail;
   U8                           absRprtPeriodCount;
} WrEmmLteAdvAbsAggrCellInfo;

/*********************************************************************/
/*            DATA STRUCTURES OF ABS MODULE(PICO)                    */
/*********************************************************************/

typedef struct _wrEmmLteAdvAbsVctmPeerEnbInfo
{
   CmLListCp                    absPeerEnbLstCp;
   CmLListCp                    absPatternLstCp;
} WrEmmLteAdvAbsVctmPeerEnbInfo;
                                  
/* KW fix for LTE_ADV */
typedef struct _wrEmmAbsVctmPeerEnbCb
{             
   CmLList                      lnk;
   WrEmmAbsEnbId                enbId;              
   U32                          enb1MeasId;
   U32                          enb2MeasId;
   U16                          rprtPeriodicity;
   U8                           absRprtStatus;
   U16                          absRprtPeriodCount;
   CmLListCp                    absPeerCellLstCp;
   CmTimer                      tmr;
} WrEmmAbsVctmPeerEnbCb;

typedef struct _wrEmmAbsVctmPatternCb
{             
   CmLList                      lnk;
   U8                           absPattern[WR_EMM_ABS_PATTERN_SIZE];
   U8                           absPercentage;
   U32                          absWeightage;  /* KW fix for LTE_ADV */
   CmLListCp                    absCellLstCp;
} WrEmmAbsVctmPatternCb;

typedef struct _wrEmmAbsVctmPeerCellCb
{
   CmLList                      enbLnk;
   CmLList                      absPatternLnk;
   WrEmmAbsCellId               cellId;       
   U8                           noCSAntPorts;
   U8                           measSubset[WR_EMM_ABS_PATTERN_SIZE];
   U32                          cellWeightage;
   WrEmmAbsVctmPatternCb        *absPatternCb;
} WrEmmAbsVctmPeerCellCb;

typedef struct _wrEmmLteAdvAbsVctmCellInfo
{
   U8                           absPercentage;
   U8                           absPercentageRsrcUsed;
} WrEmmLteAdvAbsVctmCellInfo;

/**********************************************************************/
/*          DATA STRUCTURES OF ABS MODULE(MACRO+PICO)                 */
/**********************************************************************/

typedef struct _wrEmmLteAdvAbsPeerEnbInfo
{
   union
   {
      WrEmmLteAdvAbsAggrPeerEnbInfo    aggrInfo;
      WrEmmLteAdvAbsVctmPeerEnbInfo    vctmInfo;
   } u;
} WrEmmLteAdvAbsPeerEnbInfo;

typedef struct _wrEmmLteAdvAbsCellInfo
{
   U8                           absPattern[WR_EMM_ABS_PATTERN_SIZE];
   U8                           isAbsEnabled;
   union
   {
      WrEmmLteAdvAbsAggrCellInfo    aggrInfo;
      WrEmmLteAdvAbsVctmCellInfo    vctmInfo;
   } u;
} WrEmmLteAdvAbsCellInfo;


typedef struct _wrEmmAbsInfo
{
   WrEmmAbsState                state;
   WrEmmAbsNegType              negType;
   WrEmmLteAdvAbsPeerEnbInfo    peerInfo;
} WrEmmAbsInfo;


/**********************************************************************/
/*              INTERFACE FUNCTIONS OF ABS MODULE                     */
/**********************************************************************/
EXTERN S16 wrEmmAbsProcessEvent (WrEmmAbsEvent *evnt);

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                        EMM ABS MODULE - END                      %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/* LTE_ADV_ABS ends */

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                        EMM DSFR MODULE - START                   %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/* LTE_ADV_DSFR starts */
#define WR_EMM_CHECK_RNTP_BIT(_var, _pos) ((_var) & (1<<(_pos)))
/* LTE_ADV_DSFR ends */

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                        EMM DSFR MODULE - START                   %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                EMM LTE-ADVANCED MODULE - START                   %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
typedef struct _wrEmmLteAdvCellCb
{
   LwrLteAdvancedConfig         lteAdvCfg;
/* LTE_ADV_ABS starts */
   WrEmmLteAdvAbsCellInfo       absInfo;
/* LTE_ADV_ABS ends */
} WrEmmLteAdvCellCb;

typedef struct _wrEmmLteAdvCb
{
   U32                          enbType; 
/* LTE_ADV_ABS starts */
   WrEmmAbsInfo                 absInfo;
/* LTE_ADV_ABS ends */
} WrEmmLteAdvCb;

/**********************************************************************/
/*          INTERFACE FUNCTIONS OF LTE-Adavanced MODULE               */
/**********************************************************************/
EXTERN S16 wrEmmHdlLteAdvReCfg (CmLteCellId                       cellId, 
                                LwrLteAdvancedConfigTrans *lteAdvancedTrans,
                                Bool *trgSib4);  /*KW fix for LTE_ADV */
EXTERN S16 wrEmmHdlLteAdvCfg (U32                         cellId, 
                              LwrLteAdvancedConfigTrans   *lteAdvancedTrans);
EXTERN S16 wrEmmLteAdvUpdCellSpfcOffset (U32              cellId,
                                         RgrFeature       feature,
                                         RgrFeatureStatus status);
EXTERN S16 wrEmmSchdLoadInfInd (RgrLoadInfIndInfo *loadInfInd);
EXTERN S16 wrEmmLteAdvPrcLoadInfo (U32 peerId, CztEvnt *pdu);
EXTERN S16 wrEmmLteAdvPrcTmrExpiry(PTR cb, U32 event);

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                        EMM DSFR MODULE - START                   %*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/* LTE_ADV ends */
#endif /* #define __WR_EMM_LTEADV_H__ 1 */ 
