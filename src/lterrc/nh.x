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

     Name:     LTE-RRC Layer 
  
     Type:     C include file
  
     Desc:     Structures, variables, and typedefs required 
               by the LTE RRC layer.

     File:     nh.x

     Sid:      nh.x@@/main/3 - Fri Jul  1 01:13:12 2011

     Prg:      rer 

**********************************************************************/


/*
 *     The structures and variables declared in this file
 *     correspond to structures and variables used by
 *     the following TRILLIUM software:
 *
 *     part no.             description
 *     --------     ----------------------------------------------
 *     1000369      LTE-RRC
 *
*/

#ifndef __NHX__
#define __NHX__

/* Include files */


#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

/* Defines for Statistics */
typedef U8     MsgType;
typedef U8     Direction;
typedef U8     CntrType;

/** @def NhTransId
   Transaction Identifier 
 */
typedef U32 NhTransId;

/** @enum nhSduType
 * This enum defines logical channel types with respect to transport channels.
 */
typedef enum nhSduType
{
   NH_SDU_DL_BCCHBCH,
   NH_SDU_DL_BCCHDLSCH,
   NH_SDU_DL_PCCH,
   NH_SDU_DL_CCCH,
   NH_SDU_UL_CCCH,
   NH_SDU_DL_DCCH,
   NH_SDU_UL_DCCH,
   NH_SDU_MAX
}NhSduType;

/**
 * @struct nhId
   This structure contains radio bearer identity for the UE or Cell.
*/
typedef struct nhId
{
   U16 cellId;      /*!< Cell identity */
   U16 ueId;        /*!< UE identity */
   Bool isPdcpId;   /*!< TRUE indicates PDCP ID and FALSE indicates RLC ID */
   U8  rbId;        /*!< Radio Bearer identity */
} NhId;

/**
 * @struct muiInfo
   This structure stores Message Unit Identifier (MUI) information. 
*/
typedef struct muiInfo
{
   Bool     status;     /*!< Available status - USED/UNUSED */
   U32      nxTransId;  /*!< rrc User TransId */
   U32      mui;        /*!< Message Unit Identifier */
} MuiInfo;

/**
 * @struct nhUeCbKey
   This structure Stores UeCb Key, which contains crnti and cellId.
*/
typedef struct nhUeCbKey
{
    U16      crnti;      /*!< C-RNTI which identifies a UE ID */
} NhUeCbKey;

/**
 * @struct nhCellCbList
   This structure contains current RB configuration for a particular UE.   
*/
typedef struct nhCellCbList     
{
   U8              numOfCellCb;  /*!< Number of Cell Cb present */  
   CmLListCp       cellCbList;   /*!< Linked list of NhCellCb   */    
} NhCellCbList;

/**
 * @struct nhRbCfgList
   This structure contains current RB configuration for a particular UE.   
*/
typedef struct nhRbCfgList     
{
   U8              numOfRbCfgInfo;  /*!< Number of RB cfg present */  
   CmLListCp       rbCfgInfoList;  /*!< Linked list of NhuRbCfgInfo */    
} NhRbCfgList;

/**
 * @struct nhUeInfo
   This is the UE ID information that RRC user gives to RRC,
   whenever it allocates a new crnti, so that RRC can reconfigure the 
   lower layers with the new crnti information.
*/ 
typedef struct nhUeInfo
{
   U8          crntiCfgType;
   union
   {
      NhuCrnti    newCrnti;  /*!< Crnti with which new UE context needs to be 
                                                     created */
      NhuCrnti    modCrnti;  /*!< Crnti to be re-configured */
   } u;
} NhUeInfo;

/**
* @struct nhLogChCfg
* This structure contains logical channel configuration. This is updated 
* during commit after all successful configuration.
*/
typedef struct nhLogChCfg
{
   U8             lChCfgBitMsk;
   CmLteTrchType  dlTrchType;
   CmLteTrchType  ulTrchType;
   NhuLogChCfg    logChCfg;
}NhLogChCfg;

/**
* @struct nhRbToLchMap
* This structure contains RB ID to Logical channel mapping along with 
* the configuration. This is updated during commit after all the successful
* configuration.
*/
typedef struct nhRbToLchMap
{
   NhuRbId    rbId;
   NhuLogChId lchId;
   U8         dir;
   NhLogChCfg logChCfg;
   Bool       isPdcpConfigured; /* indicates if this RB has a PDCP entity */
} NhRbToLchMap;
/**
 * @struct nhCrntCfgInfo
   This structure contains current configuration for a particular UE.   
*/
typedef struct nhCrntCfgInfo     
{
   U8                bitMskPres;     /*!< Selects which config 
                                                is present */
   NhRbCfgList       rbCfgList;      /*!< RB configuration info */
   CmLListCp         logChCbLst;     /*!< List of NhRbToLchMap */
   NhuTrChCfg        trChCfg;        /*!< Transport Channel related 
                                          configuration - derived from NHU */
   NhuSecurityCfg    secCfg;         /*!< Security configuration */

#ifdef RRC_PHY_CFG_SUPP
   NhuPhyCfg         phyCfg;        /*!< Physical layer specific configuration.*/
#endif /* RRC_PHY_CFG_SUPP */
} NhCrntCfgInfo;

#ifdef RRC_PHY_CFG_SUPP
/**
 * @struct nhPhyCellCfgCfmMap
   This structure stores cell cfg confirm status received from PHY.  
*/
typedef struct nhPhyCellCfgCfmMap
{
   U8    status;      /*!< Status - USED/NOT-USED */
   U8    cfmStatus;   /*!< Confirm status - ROK/RFAILED */
   U8    rvrtAction;  /*!< Revert Action -  Add/Modify/Release */
} NhPhyCellCfgCfmMap;
#endif /* RRC_PHY_CFG_SUPP */

/**
 * @struct nhRlcCellCfgCfmMap
   This structure stores cfg confirms received from RLC for a cell.  
*/
typedef struct nhRlcCellCfgCfmMap
{
   U8    status;      /*!< Status - USED/NOT-USED */
   U8    cfmStatus;   /*!< Confirm status - ROK/RFAILED */
   U8    rvrtAction;  /*!< Revert Action -  Add/Modify/Release */
   U8    rbId;        /*!< Radio Bearer ID */
} NhRlcCellCfgCfmMap;

/**
 * @struct nhMacCellCfgCfmMap
   This structure stores cfg confirms received from MAC for a cell.    
*/
typedef struct nhMacCellCfgCfmMap
{
   U8    status;      /*!< Status - USED/NOT-USED */
   U8    cfmStatus;   /*!< Confirm status - ROK/RFAILED */
   U8    rvrtAction;  /*!< Revert Action - Add/Modify/Release */
   U8    logChId;     /*!< Radio Bearer ID */
   U8    dir;
} NhMacCellCfgCfmMap;

/**
 * @struct nhCellOrdrdCfg
   This structure stores ordered configuration for cell in config/reconfig.    
*/
typedef struct nhCellOrdrdCfg
{
   CmLteTimingInfo   reCfgActvnTime; /*!< Activation time, only SFN is valid,
                                          Used for Reconfig case only */ 
   NhuCellCfgInfo    cellCfgInfo;    /*!< Cell configuration info */
} NhCellOrdrdCfg;

/**
 * @struct nhCellTransCb
   This structure stores Cell Transaction info.    
*/
typedef struct nhCellTransCb
{
   U8                  status;        /*!< Available status - USED/UNUSED */
   U32                 nxTransId;     /*!< RRC User - Transaction ID */
   NhCellOrdrdCfg      *cellOrdrdCfg; /*!< Ordered configuration */
   U8                  cfmMask;       /*!< Current Cfg bitmask for all layers*/
   U8                  cfgMask;       /*!< Bitmask for layers to which cfg 
                                           is sent */
   U8                  cfmStatus;     /*!< Bitmask for layers cfm results */
   U8                  cfgAction;     /*!< Add/Modify/release */
   S16                 numOfRlcCfms;  /*!< Expected number of RLC cfms */
   S16                 numOfMacCfms;  /*!< Expected number of MAC cfms */
   NhRlcCellCfgCfmMap  rlcMapTbl[NH_MAX_RB_PER_CELL_CFG];  /*!< RLC cfm map 
                                                                table */
   NhMacCellCfgCfmMap  macMapTbl[NH_MAX_RB_PER_CELL_CFG];  /*!< MAC cfm map 
                                                                table */
#ifdef RRC_PHY_CFG_SUPP
   S16                 numOfPhyCfms;  /*!< Expected number of PHY cfms */
   NhPhyCellCfgCfmMap  phyMapInfo;    /*!< PHY cfm map info */
#endif /* RRC_PHY_CFG_SUPP */
} NhCellTransCb;

/**
 * @struct nhCmnPhyCfg
   This structure holds the physical channel configuration.   
*/


/**
 * @struct nhCellCfgInfo
   This structure is the Cell Configuration info.   
*/
typedef struct nhCellCfgInfo
{
   NhuDuplexMode    duplexMode;   /*!< Mode: FDD/TDD */
   U8               cfgBitMsk;    /*!< Bitmask indicates the presence 
                                       of Cell related cfg elements */
   NhRbCfgList      rbCfgList;    /*!< RB configuration */                       
   NhuCmnMacCfg     macCfg;       /*!< Common config for MAC layer */
#ifdef RRC_PHY_CFG_SUPP
   NhuCmnPhyCfg     phyCfg;       /*!< PHY configuration for cell */
#endif /* RRC_PHY_CFG_SUPP */
    Bool             isCSGCell;   /*!< Identifies whether it is CSG
                                       Cell or non-CSG cell */
} NhCellCfgInfo;

/**
 * @struct nhCellCb
   This structure is the Cell control block.
*/
typedef struct nhCellCb
{
    U16              cellId;              /*!< Cell ID */
#ifdef NHU_MULT_SAP_SUPPRT
    NhuLLyrSapIdInfo llyrSapIdInfo;       /*!< Lower SAP ID info */   
#endif /* NHU_MULT_SAP_SUPPRT */
    NhCellCfgInfo    crntCellCfgInfo;     /*!< Stores current cell 
                                               configuration */ 
#ifndef NH_NO_STS     
    NhCellSts        cellSts;             /*!< Cell specific statistics */
#endif /* NH_NO_STS */
    NhCellTransCb    cellTransCb;         /*!< Stores Cell Transaction Info */
    CmHashListCp     actvUeHlCp;          /*!< Active UE hash lst control ptr*/
    CmHashListCp     newUeHlCp;           /*!< Hash lst control ptr for New UE
                                               for which UEID chgReq has just 
                                               sent and cfms are still 
                                               awaiting */
    CmTimer          cfgTimers[NH_MAX_CFG_TMRS]; /*!< UE lower layer config 
                                                   timers */
} NhCellCb;

/**
 * @struct nhActvUeCb
   This structure is the UE control block.
*/
typedef  struct nhActvUeCb
{
   CmHashListEnt     actUeEnt;            /*!< Hashlist manipulator */
   CmHashListEnt     newUeEnt;            /*!< Hashlist manipulator for New
                                            UE for which ueIdChgReq has 
                                            been sent */
   NhUeCbKey         ueCbKey;             /*!< Active UE control block key, 
                                             same as CRNTI */
   NhUeCbKey         newUeCbKey;          /*!< Active UE control block key, 
                                             same as CRNTI */
   CmLListCp         muiInfoLst;          /*!< MUI Info List */
#ifndef NH_NO_STS   
   NhUeSts           ueSts;               /*!< UE specific statistics */
#endif /* NH_NO_STS */
   NhCrntCfgInfo     crntCfg;             /*!< Current configuration */
   CmLListCp         transCbLst;          /*!< Transaction information */
   CmHashListCp      msgTransHlCp;        /*!< Transactions Hashlist */
   Ticks             tmStmp;              /*!< Time of creation */
   U8                rbSecCfgBitMask;     /*!< SRBs configuration and 
                                             security configuration status */   
   NhCellCb          *cellCb;              /*!< Cell CB to which UE belongs */
   U8                restbState;          /*!< UE re-establishment proc state*/
   U8                hoToEutraOrMobFrmEutra;  /*!< This is set to  
                                               1. NH_MSGTYPE_CONNRECFGCOMP_HO when ueCb 
                                               is created during handover to EUTRA. 
                                               This is used to increment ho success 
                                               counter when HO Recfg complete is 
                                               received and this is reset. 
                                               2. NH_MSGTYPE_MOBFRMEUTRAN when Mobility
                                               from E-UTRA message is sent by RRC.
                                               This is used in case of mobility failure.
                                               On failure, UE triggers re-establishment,
                                               and if this flag is set to mobility
                                               message type, on receiving Re-establishment
                                               request from UE, RRC increments mobility
                                               failure counter. */ 
} NhActvUeCb;

/**
 * @struct nhENodeBCb
   This structure is the Node database.
*/
typedef struct nhENodeBCb
{
   NhProtCfg         protCfg;           /*!< Protocol configuration */
   Bool              cfgDone;           /*!< TRUE or FALSE */
   U32               trcMask;           /*!< Trace Mask */
   S32               trcLen;            /*!< Trace Length */
   U32               numActUEs;         /*!< Number of Active UEs */
   U32               numActCells;       /*!< Number of Active Cells */
   NhCellCbList      nhCellCbList;      /*!< cell Cb list */
} NhENodeBCb;

/**
 * @struct nhUSapCb
   This structure is the Upper SAP control block.
*/
typedef struct nhUSapCb
{
   SuId          suId;           /*!< Service user SAP ID */
   SpId          spId;           /*!< SAP ID */
   Pst           pst;            /*!< Post structure */
   State         state;          /*!< State of this SAP */
} NhUSapCb;

/**
 * @struct nhLSapCb
   This structure is Lower SAP Control Block.  
*/
typedef struct nhLSapCb
{
   U16           sapElmId;                 /*!< SAP Entity */
   SuId          suId;                     /*!< Service user SAP ID */
   SpId          spId;                     /*!< Service provider SAP ID */
   State         state;                    /*!< State of this SAP */
   Pst           pst;                      /*!< Post structure */
   U8            bndRetryCnt;              /*!< Bind retry count */
   TmrCfg        bndTmr;                   /*!< Bind timer */
   CmTimer       timers[NH_MAX_LSAP_TMRS]; /*!< Maximum simultaneous timers */
   Bool          trc;                      /*!< trace flag */
} NhLSapCb;

/**
 * @struct nhCb
   This structure contains informaton about the configuratons for RRC.
*/
typedef struct nhCb
{
   TskInit       init;                          /*!< Task initialization structure */
   NhGenCfg      genCfg;                        /*!< General configuration */
   NhUSapCb      *uSapCb;                       /*!< NHU upper SAP control block*/
   NhLSapCb      **kwuSapCbLst;                 /*!< RLC data SAP control block*/
   NhLSapCb      **ckwSapCbLst;                 /*!< RLC Control SAP control block */
   NhLSapCb      **crgSapCbLst;                 /*!< MAC Control SAP control block */
   NhLSapCb      **cpjSapCbLst;                 /*!< PDCP Control SAP control block */
   NhLSapCb      **pjuSapCbLst;                 /*!< PDCP Data SAP control block */
#ifdef RRC_PHY_CFG_SUPP
   NhLSapCb      **ctfSapCbLst;                 /*!< PHY Control SAP control block */
#endif /* RRC_PHY_CFG_SUPP */
   NhGenSts       genSts;                       /*!< General statistics */
   NhGenSta       genSta;                       /*!< General status */
   NhENodeBCb     eNodeBCb;                     /*!< Node control block */
   CmTqCp         nhGenTqCp;                    /*!< MSec timing queue */
   CmTqType       nhGenTq[NH_TQNUMENT];         /*!< General timing queue */ 
   CmTimer        timers[NH_MAX_TMRS];          /*!< Maximum simultaneous timers */
   CmTqCp         nhLLyrCfgTqCp;                /*!< Lower layer cfg timing queue */
   CmTqType       nhLLyrCfgTq[NH_TQNUMENT];     /*!< Lower layer cfg queue */ 
   Bool           outOfRsrc;                    /*!< Out of Resources */
   NhCellCb       **cellCbLst;                  /*!< list of all cells
                                                     configured.
                                                */ 
#ifdef NH_LOG_TO_FILE
   FILE*         dbgFp;                         /*!< Debug file pointer */
   Bool          fileLogEnb;                    /*!< To Enable/Disable file logging */
   Txt           filePath[LNH_MAX_FILE_PATH];   /*!< Path to store log files */
   U32           nmbDbgLines;                   /*!< Number of lines per debug file */
   U32           cfgDbgLines;                   /*!< Cfgd number of lines/Dbg file */
#endif /* end of NH_LOG_TO_FILE */
#ifdef LTE_RRC_DISSECTOR
   S32           udpSock;                       /*!< UDP socket for rrc dissector*/
   struct sockaddr_in server;                      /* Server Details */ 
#endif

} NhCb;

/**
  @struct nhRlcPdcpCfgCfmMapInfo  
  This structure contains configuration confirm status, configuration type used and 
  configuration type to be applied for reverting the configuration at RLC/PDCP.
*/
typedef struct nhRlcPdcpCfgCfmMapInfo
{
   U32    drbToCfg[NH_MAX_RB_ACTION]; /* Index 0-Add, 1-Mod, 2-Rls, 3-Restb */
   U8     srbToCfg[NH_MAX_RB_ACTION]; /* Index 0-Add, 1-Mod, 2-Rls, 3-Restb */
   U32    drbCfmSta[NH_MAX_RB_ACTION]; /* Index 0-Add, 1-Mod, 2-Rls, 3-Restb */
   U8     srbCfmSta[NH_MAX_RB_ACTION]; /* Index 0-Add, 1-Mod, 2-Rls, 3-Restb */
} NhRlcPdcpCfgCfmMapInfo;

/**
  @struct nhUeCfgCfmMapInfo  
  This structure contains configuration confirm status, configuration type used and 
  configuration type to be applied for reverting the configuration at MAC.
*/
typedef struct nhUeCfgCfmMapInfo
{
   U8     status;          /*!< Radio Bearer ID */
   U8     action;
   U8     cfmStatus;       /*!< Configuration sent for security */
} NhUeCfgCfmMapInfo;

/**
  @struct nhSrb1RestbInfo  
  This structure contains configuration confirm status, configuration type used and 
  configuration type to be applied for reverting the configuration at MAC.
*/
typedef struct nhSrb1RestbInfo 
{
   U8     status;          /*!< Radio Bearer ID */
   U8     action;
   U8     cfmStatus;       /*!< Configuration sent for security */
} NhSrb1RestbInfo;

/**
  @struct nhMacRstInfo  
  This structure contains configuration confirm status, configuration type used and 
  configuration type to be applied for reverting the configuration at MAC.
*/
typedef struct nhMacRstInfo 
{
   U8     status;          /*!< Radio Bearer ID */
   U8     action;
   U8     cfmStatus;       /*!< Configuration sent for security */
} NhMacRstInfo;


/**
  @struct nhMacLchCfgCfmMapInfo  
  This structure contains configuration confirm status, configuration type used and 
  configuration type to be applied for reverting the configuration at MAC.
*/
typedef struct nhMacLchCfgCfmMapInfo
{
   U32    logchsToAdd;     /*!< Logical channel id to add */
   U32    logchsToMod;     /*!< Logical channel id to Modify */
   U32    logchsToRls;     /*!< Logical channel id to rls */
   U32    addCfmStatus;    /*!< Confirm status for rcvd logchs */
   U32    modCfmStatus;    /*!< Confirm status for rcvd logchs */
   U32    rlsCfmStatus;    /*!< Confirm status for rcvd logchs */
   U32    logchsRcvd;      /*!< Logchs from which Confirm rcvd  */
} NhMacLchCfgCfmMapInfo;

/**
  @struct nhOrdrdCfgInfo 
  This structure is the UE ordered configuration per transaction. Ordered configuration is
  maintained at UE context till it is committed in UE context.
*/
typedef  struct nhOrdrdCfgInfo
{
   U16                  actionMask;       /*!< Bitmask RLC/PDCP/MAC/Security 
                                               configuration */
   NhuRbCfgList         rbToAddCfgList;   /*!< RB configuration for Add */
   NhuRbCfgList         rbToModCfgList;   /*!< RB configuration for Modify */
   NhuRbRlsCfgList      rbToRlsCfgList;   /*!< RB configuration for Release */
   NhRbCfgList          rbToRestbCfgList;  /*!< RB config for Re-establish */
   NhuTrChCfg           trChCfg;          /*!< Transport Channel configuration */
   NhuSecurityCfg       secCfg;           /*!< Security configuration at PDCP*/ 
   NhUeInfo             ueInfo;           /*!< CRNTI to be used for 
                                               re-establishment */
#ifdef RRC_PHY_CFG_SUPP
   NhuPhyCfg            phyCfg;           /*!< Physical channel configuration */
#endif /* RRC_PHY_CFG_SUPP */
} NhOrdrdCfgInfo;

#ifdef RRC_PHY_CFG_SUPP
/**
  @struct nhPhyChCfgMapInfo  
  This structure contains the confirm status for physical channel configuration
  at PHY.
*/
typedef struct nhPhyChCfgMapInfo
{
   Bool    status;      /*!< Available Status - USED/NOT-USED */
   U8      cfmStatus;   /*!< ROK/RFAILED */
   U8      action;      /*!< Configuration Type - Add/Modify/Release */
} NhPhyChCfgMapInfo;
#endif /* RRC_PHY_CFG_SUPP */

/**
  @struct nhSecCfgMapInfo  
  This structure contains the Security configuraton confirm status.
*/
typedef struct nhSecCfgMapInfo
{
   Bool    status;      /*!< Available Status - USED/NOT-USED */
   U8      cfmStatus;   /*!< ROK/RFAILED */
   U8      action;      /*!< Configuration Type - Add/Release */
} NhSecCfgMapInfo;

/**
  @struct nhPdcpDatReqParam
  This structure contains the PDCP parameters to send messages to PDCP entities.
*/
typedef struct nhPdcpDatReqParam
{
   U32 mui; 
} NhPdcpDatReqParam;

/**
   @struct nhTmCcchInfo
   It contains the RLC-TM CCCH specific parameters required for sending
   the UE message to RLC for transmission
*/
typedef struct nhTmCcchInfo
{
   CmLteRnti   rnti;      /*! Temporary C-RNTI*/
} NhTmCcchInfo;

/**
  @struct nhTmRlcDatReqParam
  This structure contains the RLC parameters to send messages to RLC TM entities.
*/
typedef struct nhTmRlcDatReqParam
{
   CmLteLcType    lcType;             /* BCCH/PCCH/CCCH */
   union
   {
      CmLteTimingInfo   timingInfo;    /*!< Timing information(BCCH/PCCH) */
      NhTmCcchInfo      ccchInfo;      /*!< CCCH Information */
   } u;
} NhTmRlcDatReqParam;

/**
  @struct nhDatReqParam
  This structure contains the RLC or PDCP parameters to send messages to RLC or 
  PDCP entities.
*/

typedef struct nhDatReqParam
{
   NhId nhId;  /*!< Identifies the UE or cell specific lower layer entity */      
   Bool isPdcpDatReq; /*!< TRUE indicates PDCP and FALSE indicates RLC */
   Bool isMsgEncoded; /*!< TRUE indicates indicates msg is already encoded */
   union
   {
      NhTmRlcDatReqParam rlcParam;    /*!< RLC parameters */
      NhPdcpDatReqParam pdcpParam;    /*!< PDCP parameters */       
   } u; 
} NhDatReqParam;

/**
  @struct NhPdcpSduStaInfo
  This structure contains the PDCP SDU status information provided by PDCP.
*/

typedef struct nhPdcpSduStaInfo
{
   Reason            reason;          /*!< Reason for failure in case of failure,
                                          otherwise 0 */
   U16               numRbId;         /*!< Number of Radio bearers */
   NhuPdcpSduStaInfo *pdcpSduStaInfo; /*!< SDU status info provided by PDCP */
} NhPdcpSduStaInfo;

/**
  @struct transCb
  This structure UE Transaction control block contains the non-committed 
  information for each configured lower layers per transaction.
*/
typedef struct transCb
{
   CmHashListEnt        msgTransEnt;   /*!< Hashlist manipulator */
   U32                  msgTransKey;   /*!< Message Type and Message 
                                            Transaction ID */
   Bool                 status;        /*!< Available status - USED/UNUSED */
   NhTransId            nxTransId;     /*!< RRC User - Transaction ID */
   NhTransId            transId;       /*!< Configuration - Transaction ID */
   U8                   cause;         /*!< Origin message causing configuration */
   NhOrdrdCfgInfo       ordrdCfg;      /*!< Ordered configuration */
   U8                   waitFlag;      /*!< Waiting for UE response or not */
   U8                   cfmMask;       /*!< Current Configuration 
                                            bitmask: RLC/MAC/PDCP */
   U8                   cfgMask;       /*!< Bitmask for layers to which 
                                            configuration has been sent */
   U8                   cfmStatus;     /*!< Bitmask for layers from which 
                                            confirm has been received */
   S16                  numOfMacCfms;  /*!< Expected no. of MAC confirms */
   S16                  numOfRlcCfms;  /*!< Expected no. of RLC confirms */
   S16                  numOfPdcpCfms; /*!< Expected no. of PDCP confirms */
   NhUeCfgCfmMapInfo    ueMapTbl[NH_UE_MAPTBL_MAX_IDX];
   NhSrb1RestbInfo      restbTbl[NH_RESTB_SRB1_TBL_MAX_IDX];
   NhMacRstInfo         rstTbl[NH_MAC_RST_MAX_IDX];
   NhMacLchCfgCfmMapInfo macLchMapTbl[NH_MAX_LC_PER_RB]; /*!< 0 for DL, 1 for UL */
   NhRlcPdcpCfgCfmMapInfo  rlcMapTbl;     /*!< RLC Radio Bearer Confirm Information */
   NhRlcPdcpCfgCfmMapInfo  pdcpMapTbl;    /*!< RLC Radio Bearer Confirm Information */
   NhSecCfgMapInfo      secCfgMapTbl[NH_MAX_SEC_CFG];/*!< Stores Security 
                                                       config  cfm Info*/
#ifdef RRC_PHY_CFG_SUPP
   NhPhyChCfgMapInfo    phyMapInfo;    /*!< PHY confirm information */
   S16                  numOfPhyCfms;  /*!< Expected no. of PHY confirms */
#endif /* RRC_PHY_CFG_SUPP */
   U8                   macCfgMask;    /*!< Bitmask Add/Modify/Release*/
   Bool                 rvrtReqd;      /*!< Lower Layer configuration need to be 
                                            reverted or not */
   Buffer               *storedMsg;    /*!< Encoded message is stored for 
                                            transmission after configuration 
                                            is successful */
   NhDatReqParam       *lowerLyrParam; /*!< Lower layer parameters required by 
                                             PIM to send the stored message */ 
   U8              enblSelSecModeCfg;      /*!< Control to enable or disable Selective */
   NhuPdcpSduStaCfm  *nhuSduStaCfm;    /*!< nh002.301:118385:HO issue: pdcp sdu cfm required
                                            to send it after receiving cfm from all the lower layers*/
} TransCb;

/**
   @struct nhMsgPduAndType
   This structure contains Message PDU and the corresponding message type.
*/
typedef struct nhMsgPduAndType
{
   Void *pdu; /*!< Message PDU */
   Void *encBuf; /*!< Encoded message buffer */
   U8   type; /*!< Message type */
} NhMsgPduAndType;

/**
  @struct nhErrInfo
  This structure contains the error information for use while sending Error Indication to the RRC user. The values of the subfields in this structure are derived from NHU.
*/ 
typedef struct nhErrInfo 
{ 
   U8    errType;    /* Type of the error */
   U8    errCause;   /* Cause of the error */
} NhErrInfo;

/** @struct nhCellAndUeCb
 * This structure is a placeholder of ueCb and cellCb, which are used wherever
 * either/both of these are expected.
*/
typedef struct nhCellAndUeCb
{
   NhCellCb    *cellCb;
   NhActvUeCb  *ueCb;
} NhCellAndUeCb;

/** @struct nhDatReqRspSdu 
  * Union of NhuDatReqSdus and NhuDatRspSdus. This is filled while sending
  * message to the lower layers.
 */
typedef struct nhDatReqRspSdu
{
   Bool  isDatReqSdu;      /* TRUE: DatReq, FALSE: DatRsp */
   union 
   {
      NhuDatReqSdus  *datReqSdu;
      NhuDatRspSdus  *datRspSdu;
   } sdu;
} NhDatReqRspSdu;

/** @struct nhLLyrCfgTmrCb
  * This structure is a placeholder for UE-CB and NX transaction ID
 */
typedef struct nhLLyrCfgTmrCb
{
   NhActvUeCb  *ueCb;
   U32         transId;
} NhLLyrCfgTmrCb;

/** @struct nhRlcPdcpCfmEnt
  * This structure is a placeholder for RB information of RLC and PDCP Cfg Cfms
 */
typedef struct nhRlcPdcpCfmEnt
{
   U8    rbId;
   U8    rbType;
   U16   status;
   U16   reason;
}NhRlcPdcpCfmEnt;

/* LTE-RRC Control Block */
EXTERN NhCb nhCb;

typedef S16  (*PCDFS16) ARGS ((NhDatReqRspSdu    *datReqRspSdu,
                               NhCellAndUeCb    *cellAndUeCb,
                               U8                msgTransId,
                               NhuErrorInfo      *errInfo));
#ifdef NH_TRACE_ENABLED
EXTERN Void nhUTLGenTrc ARGS((SuId suId,   
                              U8 sapType, 
                              U16 evnt,   
                              Buffer *mBuf 
                              ));
#endif /* NH_TRACE_ENABLED */
EXTERN S16 nhGetSId ARGS((SystemId *s));
EXTERN S16 nhActvTsk ARGS((Pst *pst, Buffer *mBuf));

EXTERN S16 nhBRMSysInfoTx ARGS((NhDatReqRspSdu *datReqRspSdu,
                                NhCellAndUeCb *cellAndUeCb,
                                U8 msgTransId,
                                NhuErrorInfo *errInfo));

EXTERN S16 nhCCMUlCmnHndlr ARGS((NhuDatIndSdus *nhuDatIndSdu,
                                 NhCellAndUeCb   *cellAndUeCb,
                                 U8 msgTransId,
                                 U8 msgType));

EXTERN S16 nhCCMConSetupCmpHndlr ARGS((NhuDatIndSdus *nhuDatIndSdu,
                                       NhCellAndUeCb   *cellAndUeCb,
                                       U8 msgTransId,
                                       U8 msgType));

EXTERN S16 nhCCMConRlsCmpHndlr ARGS((NhuDatIndSdus *nhuDatIndSdu,
                                       NhCellAndUeCb   *cellAndUeCb,
                                       U8 msgTransId,
                                       U8 msgType));

EXTERN S16 nhCCMReCfgCmpHndlr ARGS((NhuDatIndSdus *nhuDatIndSdu,
                                    NhCellAndUeCb   *cellAndUeCb,
                                    U8 msgTransId,
                                    U8 msgType));

EXTERN S16 nhCCMSecModCmpHndlr ARGS((NhuDatIndSdus *nhuDatIndSdu,
                                       NhCellAndUeCb   *cellAndUeCb,
                                       U8 msgTransId,
                                       U8 msgType));

EXTERN S16 nhCCMSecModFailHndlr ARGS((NhuDatIndSdus *nhuDatIndSdu,
                                       NhCellAndUeCb   *cellAndUeCb,
                                       U8 msgTransId,
                                       U8 msgType));

EXTERN S16 nhCCMConSetupHdlr ARGS((NhDatReqRspSdu *datReqRspSdu,
                                   NhCellAndUeCb     *cellAndUeCb,
                                   U8 msgTransId,
                                   NhuErrorInfo *errInfo));

EXTERN S16 nhCCMConRstbRejHdlr ARGS((NhDatReqRspSdu *datReqRspSdu,
                                     NhCellAndUeCb   *cellAndUeCb,
                                     U8 msgTransId,
                                     NhuErrorInfo *errInfo));

EXTERN S16 nhCCMConRejHdlr ARGS((NhDatReqRspSdu *datReqRspSdu,
                                  NhCellAndUeCb   *cellAndUeCb,
                                  U8 msgTransId,
                                  NhuErrorInfo *errInfo));

EXTERN S16 nhCCMConRlsHndlr ARGS((NhDatReqRspSdu *datReqRspSdu,
                                  NhCellAndUeCb   *cellAndUeCb,
                                  U8 msgTransId,
                                  NhuErrorInfo *errInfo));

EXTERN S16 nhCCMReCfgReqHndlr ARGS((NhDatReqRspSdu *datReqRspSdu,
                                  NhCellAndUeCb   *cellAndUeCb,
                                  U8 msgTransId,
                                  NhuErrorInfo *errInfo));

EXTERN S16 nhCCMSecModCmdHndlr ARGS((NhDatReqRspSdu *datReqRspSdu,
                                  NhCellAndUeCb   *cellAndUeCb,
                                  U8 msgTransId,
                                  NhuErrorInfo *errInfo));

EXTERN S16 nhCGMSecCfgReq ARGS((NhCellAndUeCb   *cellAndUeCb,
                                TransCb       *transCb));

EXTERN S16 nhCGMCfgReq ARGS((NhCellAndUeCb   *cellAndUeCb,
                             TransCb       *transCb,
                             NhuUeCfgInfo  *cfgInfo));

EXTERN S16 nhCGMCnclUe ARGS((NhCellCb    *cellCb, NhActvUeCb  *ueCb));

EXTERN S16 nhCGMLowerLyrCfgReq ARGS((NhCellAndUeCb   *cellAndUeCb,
                                     NhuUeCfgInfo  *cfgInfo,
                                     TransCb      *transCb,
                                     Bool           cfgReqd));

EXTERN S16 nhCGMLowerLyrCellCfgReq ARGS((NhuCellCfgReqSdus *cellCfgSdu,
                                         NhCellCb    *cellCb));

EXTERN S16 nhCGMUpdTransCbCfgMask ARGS((TransCb  *transCb,
                                        NhActvUeCb  *ueCb));


EXTERN S16 nhCGMUpdCellTransCbCfgMask ARGS((NhuCellCfgReqSdus *cellCfgSdu,
                                        NhCellCb *cellCb));

EXTERN S16 nhCGMDeleteTrans ARGS ((NhActvUeCb    *ueCb,
                                   TransCb       *transCb));

EXTERN S16 nhCGMRbRestbCfgReq ARGS(( NhCellAndUeCb  *cellAndUeCb,
                                     TransCb        *transCb));

EXTERN S16 nhDBMAddTransCb ARGS((NhActvUeCb *ueCb,
                                      TransCb  **transCb,
                                      U32    msgTransKey));

EXTERN S16 nhDBMDelTransCb ARGS((NhActvUeCb *ueCb,
                                      TransCb  **transCb));

/* ccpu00142868 : eNB does not send RRC_CONN_RLS in case of RRC_REEST with cause other */
EXTERN S16 nhDBMDelTransCbs ARGS((NhActvUeCb *ueCb));

EXTERN S16 nhDBMAddMuiCb ARGS((NhActvUeCb *ueCb,
                                      MuiInfo    **muiCb,
                                      U32     nxTransId));

EXTERN S16 nhDBMDelMuiCb ARGS((NhActvUeCb *ueCb,
                                      MuiInfo    **muiCb));

EXTERN MuiInfo* nhDBMGetMuiCb ARGS((NhActvUeCb *ueCb,
                                    U32        muiId));

EXTERN S16 nhDBMCreateCellCb ARGS((U16      cellId,
                                   NhCellCb   **cellCb));

EXTERN S16 nhDBMDeleteCellCb ARGS((NhCellCb   **cellCb));

EXTERN NhCellCb* nhDBMGetCellCb ARGS((U16 cellId));

EXTERN S16 nhDBMCommitCellOrdrdCfg ARGS((NhCellCb   *cellCb));

EXTERN S16 nhDBMCommitOrdrdCfg ARGS((NhActvUeCb *ueCb,
                                     TransCb  *transCb));
EXTERN S16 nhDBMUpdModifiedRb ARGS((NhuRbCfgInfo *dstRbCfgInfo,
                                    NhuRbCfgInfo *srcRbCfgInfo));

EXTERN S16 nhDBMUpdModifiedRbCfgUl ARGS((NhuRbCfgInfo *dstRbCfgInfo,
                                    NhuRbCfgInfo *srcRbCfgInfo));

EXTERN S16 nhDBMUpdModifiedRbCfgDl ARGS((NhuRbCfgInfo *dstRbCfgInfo,
                                    NhuRbCfgInfo *srcRbCfgInfo));

EXTERN S16 nhDBMUpdModifiedRbPdcpInfo ARGS((NhuRbCfgInfo *dstRbCfgInfo,
                                    NhuRbCfgInfo *srcRbCfgInfo));

EXTERN NhActvUeCb* nhDBMGetUeCb ARGS((NhCellCb  *cellCb, U16 ueId));

EXTERN NhActvUeCb* nhDBMGetNewUeCb ARGS((NhCellCb  *cellCb, U16 ueId));

EXTERN S16 nhDBMCreateUeCb ARGS((NhCellCb *cellCb, 
                                 U16         crnti,
                                 NhActvUeCb **ueCb));

EXTERN S16 nhDBMDeleteUeCb ARGS((NhCellCb    *cellCb,
                                 NhActvUeCb  **ueCb));

EXTERN S16 nhEDMDecReq ARGS((U8 msgCategory,
                             Buffer  *mBuf,
                             Void    *memCp,
                             Void    *pdu));

EXTERN S16 nhEDMEncReq ARGS((Void     *pdu,
                            U8       msgCategory,
                            Buffer   **mBuf,
                            U32      *length));

EXTERN S16 nhMapRlcIdtoNhId ARGS(( NhId     *nhId,
                                   CmLteRlcId        *rlcId));

EXTERN S16 nhMapPdcpIdtoNhId ARGS((NhId     *nhId,
                               CmLtePdcpId        *pdcpId));

EXTERN S16 nhLIMHndlLSapBndCfm ARGS(( U8 sapId,
                                      NhLSapCb *lSapCb,
                                      U8 bndCfmStatus));
EXTERN S16 nhLIMHndlRvrtCfg ARGS((NhId        *nhId,     
                                  NhActvUeCb  *ueCb,     
                                  TransCb     *transCb));

EXTERN S16 nhLIMHndlDatInd ARGS((NhId    *nhId,      
                                 Buffer  *mBuf,
                                 U16     tCrnti));

EXTERN S16 nhLIMHndlDatCfm ARGS((NhId   nhId,   
                                 U8     status,   
                                 U32    mui));


EXTERN S16 nhLIMRptCfgCfm ARGS((NhId       *nhId,
                                NhCellAndUeCb *cellAndUeCb,     
                                TransCb    *transCb,     
                                U8         cfmStatus,           
                                U8         layerId));

EXTERN S16 nhLIMRptCellCfgCfm ARGS((NhCellCb *cellCb,     
                                    U8       cfmStatus,           
                                    U8       layerId));

EXTERN S16 nhLIMHndlRvrtCellCfg ARGS((NhCellCb *cellCb));

EXTERN S16 nhLIMSendCellCfg ARGS((NhCellCb *cellCb));

EXTERN S16 nhLIMHndlRvrtPdcpCfg ARGS((NhActvUeCb *ueCb,   
                                      TransCb    *transCb));

EXTERN S16 nhLIMHndlRvrtMacCfg ARGS((NhId    *nhId,
                                     NhActvUeCb *ueCb,    
                                     TransCb    *transCb));

#ifdef RRC_PHY_CFG_SUPP
EXTERN S16 nhLIMHndlRvrtPhyCfg ARGS((NhId    *nhId,
                                     NhActvUeCb *ueCb,    
                                     TransCb    *transCb));
#endif /* RRC_PHY_CFG_SUPP */

EXTERN S16 nhLIMSendCfgMsg ARGS((NhCellAndUeCb *cellAndUeCb,
                                 TransCb    *transCb));

EXTERN S16 nhLIMSendPdcpCfg ARGS((NhId       *nhId,
                                  NhActvUeCb *ueCb,
                                  TransCb    *transCb));

EXTERN S16 nhLIMSendRlcCfg ARGS((NhId       *nhId,
                                 NhActvUeCb *ueCb,
                                 TransCb    *transCb));

EXTERN S16 nhLIMSendMacCfg ARGS((NhActvUeCb *ueCb,
                                 TransCb    *transCb));

EXTERN S16 nhCGMPdcpDiscReqdCfgReq ARGS((TransCb       *transCb,
                                             NhCellAndUeCb *cellAndUeCb));


#ifdef RRC_PHY_CFG_SUPP
EXTERN S16 nhLIMSendPhyCfg ARGS((NhActvUeCb *ueCb,
                                 TransCb    *transCb));
#endif /* RRC_PHY_CFG_SUPP */

EXTERN S16 nhLIMRlsLowerLyrCfg ARGS((NhActvUeCb *ueCb));

EXTERN S16 nhLIMSendUbndReq ARGS((U8 sapElmId,
                                  NhLSapCb *lSapCb));

EXTERN S16 nhLIMSendBndReq ARGS((U16  sapElmId,
                                  NhLSapCb *lSapCb));

EXTERN S16 nhLIMHndlRvrtRlcCfg ARGS((NhId        *nhId,
                                     NhActvUeCb  *ueCb,
                                     TransCb     *transCb));

EXTERN S16 nhLIMCountReqHndlr ARGS((NhuHdr    *cntHdr,
                                    NhActvUeCb  *ueCb));

EXTERN S16 nhLMMLnhCfgReq ARGS((Pst     *pst,                
                                NhMngmt *cfg));

EXTERN S16 nhGenCfg ARGS((NhGenCfg  *gen,      
                          CmStatus  *status));

EXTERN S16 nhProtCfg ARGS((NhProtCfg  *prot,   
                           CmStatus   *status));

EXTERN S16 nhUSapCfg ARGS((NhUSapCfg *uSap,     
                           CmStatus  *status));

EXTERN S16 nhLSapCfg ARGS((NhLSapCfg *lSap,     
                           U16       elmntId,   
                           CmStatus  *status));

EXTERN S16 nhGenCntrl ARGS((NhCntrl  *cntrl,        
                            CmStatus *status));
EXTERN S16 nhLSapCntrlActHndl ARGS((S16       action,
                                    S16       sAction,
                                    NhLSapCb *lSapCb,
                                    U8       evntType,
                                    U8       lSapType));

EXTERN TransCb *nhGetTransCbFrmNxTransId ARGS((NhActvUeCb   *ueCb,
                                  NhTransId    transId));

EXTERN TransCb *nhGetTransCbFrmMsgTransId ARGS((NhActvUeCb   *ueCb,
                                 U32         msgTransKey));

EXTERN S16 nhUSapCntrl ARGS((NhCntrl  *cntrl,         
                             CmStatus *status));

EXTERN S16 nhKwuSapCntrl ARGS((NhCntrl  *cntrl,         
                             CmStatus *status));

EXTERN S16 nhCkwSapCntrl ARGS((NhCntrl  *cntrl,         
                             CmStatus *status));

EXTERN S16 nhCrgSapCntrl ARGS((NhCntrl  *cntrl,         
                             CmStatus *status));

EXTERN S16 nhCpjSapCntrl ARGS((NhCntrl  *cntrl,         
                             CmStatus *status));

EXTERN S16 nhPjuSapCntrl ARGS((NhCntrl  *cntrl,         
                             CmStatus *status));

#ifdef RRC_PHY_CFG_SUPP
EXTERN S16 nhCtfSapCntrl ARGS((NhCntrl  *cntrl,
                             CmStatus *status));
#endif /* RRC_PHY_CFG_SUPP */

EXTERN S16 nhGetGenSts ARGS((NhGenSts *sts,         
                             CmStatus *status,
                             Action action));

EXTERN S16 nhGetUeSts ARGS((NhUeSts *sts,         
                             CmStatus *status,
                             Action action,
                             NhActvUeCb *ueCb));

EXTERN S16 nhGetCellSts ARGS((NhCellSts *sts,         
                             CmStatus *status,
                             Action action,
                             NhCellCb *cellCb));

EXTERN S16 nhGetLSapSta ARGS((NhSapSta *sta,         
                             U16  elmntId,
                             CmStatus *status));

EXTERN S16 nhGetUSapSta ARGS((NhSapSta *sta,         
                             CmStatus *status));

EXTERN S16 nhLMMSndLmCfm ARGS((Pst     *pst,
                               Header  *hdr,
                               U8      type,
                               NhMngmt *cfm));

EXTERN Void nhSendLmAlarm ARGS((U16       category,           
                                U16       event,             
                                U16       cause,            
                                NhUstaDgn *dgn));

EXTERN S16 nhShutdown ARGS((Void));

EXTERN Void nhPrcCmplShutdown ARGS((Void));

EXTERN S16 nhPIMRvrtPdcpCfg ARGS((NhActvUeCb *ueCb,
                                 TransCb      *transCb));
EXTERN S16 nhPIMRvrtRlcCfg ARGS((NhActvUeCb *ueCb,
                                 TransCb      *transCb));


EXTERN S16 nhPIMSendUeMsg ARGS((NhCellAndUeCb  *cellAndUeCb,
                                NhDatReqParam  *nhDatReqParam, 
                                Buffer         *mBuf));

EXTERN S16 nhPIMSendRlcCellCfg ARGS((NhCellCb       *cellCb));
EXTERN S16 nhPIMSendRlcCfg ARGS((NhActvUeCb     *ueCb,
                                 TransCb      *transCb));
EXTERN S16 nhPIMSendPdcpCfg ARGS((NhActvUeCb     *ueCb,
                                 TransCb      *transCb));

EXTERN S16 nhPIMRvrtRlcCellCfg ARGS((NhCellCb  *cellCb));
EXTERN S16 nhPIMRvrtMacAddCfg ARGS((NhActvUeCb *ueCb,
                                    TransCb      *transCb,
                                    U8 addCfgType));


EXTERN S16 nhPIMSendMacRlsCompCfg ARGS((NhId          *nhId,
                                        NhCrntCfgInfo *crntCfg));

EXTERN S16 nhPIMRlcPdcpMacRlsCfg ARGS((NhActvUeCb *ueCb));

EXTERN S16 nhPIMRlsMacOrdCfg ARGS((NhId       *nhId,
                                   NhActvUeCb   *ueCb,
                                   TransCb      *transCb,
                                   NhuRbRlsCfgList *rbCfgList,   
                                   U8           cfgType));

EXTERN S16 nhPIMSendMacCellCfg ARGS((NhCellCb *cellCb));

EXTERN S16 nhPIMRvrtMacCellCfg ARGS((NhCellCb *cellCb));

EXTERN S16 nhPIMRvrtMacModCfg ARGS(( NhActvUeCb *ueCb,
                                    TransCb *transCb,
                                    U8 cfgType));

EXTERN S16 nhPIMRvrtMacRlsCfg ARGS((NhActvUeCb *ueCb,
                                    TransCb *transCb,
                                    U8 cfgType));

EXTERN S16 nhPIMSendMacAddCfg ARGS((NhActvUeCb *ueCb,
                                    TransCb    *transCb,
                                    U8         cfgType));

EXTERN S16 nhPIMSendMacModCfg ARGS((NhActvUeCb *ueCb,
                                    TransCb    *transCb,
                                    U8         cfgType));

EXTERN S16 nhPIMSendMacRlsCfg ARGS((NhActvUeCb *ueCb,
                                    TransCb    *transCb,
                                    U8         cfgType));

EXTERN S16 nhPIMSendMacResetCfg ARGS((NhActvUeCb *ueCb,
                                    TransCb    *transCb,
                                    U8         cfgType));

EXTERN S16 nhPIMSendPdcpAddCfg ARGS((NhId         *nhId,
                                     TransCb      *transCb,
                                     NhuRbCfgList *rbCfgInfo,
                                     NhTransId    rvrtTransId));

EXTERN S16 nhPIMSendPdcpModCfg ARGS((NhId         *nhId,
                                     TransCb      *transCb,
                                     NhuRbCfgList *rbCfgInfo,
                                     NhTransId    rvrtTransId));

EXTERN S16 nhPIMSendPdcpRlsCfg ARGS((NhId    *nhId,
                                     U8      pdcpInstId,
                                     TransCb *transCb));

EXTERN S16 nhPIMSendRlcPdcpRlsCompCfg ARGS((NhId       *nhId,
                                            NhActvUeCb *ueCb,
                                            TransCb *transCb));

EXTERN S16 nhPIMSendPdcpRlsCompCfg ARGS((NhId       *nhId,
                                            NhActvUeCb *ueCb,
                                            TransCb *transCb));

EXTERN S16 nhPIMRlsRlcPdcpOrdCfg  ARGS((NhId         *nhId,          
                                        NhuRbCfgList *rbCfgList));

EXTERN S16 nhPIMSendPdcpSecCfg ARGS((NhId      *nhId,
                                     TransCb   *transCb,
                                     NhuSecurityCfg *secCfg,
                                     NhTransId rvrtTransId));

EXTERN S16 nhPIMSndCountReq ARGS((NhuHdr    *cntHdr,
                                     NhActvUeCb     *ueCb));

#ifdef RRC_PHY_CFG_SUPP
EXTERN S16 nhPIMRvrtPhyUeIdChgReq ARGS((NhActvUeCb *ueCb,
                                        TransCb    *transCb));
#endif /* RRC_PHY_CFG_SUPP */


EXTERN S16 nhHashListInit ARGS((CmHashListCp *hashListCp,  
                                U16          nmbBins,      
                                U16          offset,       
                                Bool         dupFlg,      
                                U16          keyType,  
                                Region       region,   
                                Pool         pool));

EXTERN S16 nhHashListInsert ARGS((CmHashListCp *hashListCp,  
                                  PTR          hashListEnt, 
                                  U8           *key,        
                                  U16          keyLen));

EXTERN S16 nhHashListDelete ARGS((CmHashListCp *hashListCp, 
                                  PTR          hashListEnt));

EXTERN S16 nhHashListFind ARGS((CmHashListCp *hashListCp, 
                                U8*          key,         
                                U16          keyLen,      
                                PTR          *data));

EXTERN S16 nhHashListGetNext ARGS((CmHashListCp *hashListCp, 
                                   PTR          prevEnt,     
                                   PTR          *entry));

EXTERN Void nhHashListDeInit ARGS((CmHashListCp *hashListCp));

EXTERN S16 nhActvTmr ARGS((Void));

EXTERN S16 nhSchedGenTmr ARGS((Ptr     cb,       
                               S16     tmrEvnt,  
                               Action  action));

EXTERN S16 nhStartLLyrCfgTmr ARGS((NhCellAndUeCb *cellAndUeCb,
                                   U32            nxTransId));

EXTERN S16 nhStopLLyrCfgTmr ARGS((NhCellAndUeCb *cellAndUeCb,
                                  U32            nxTransId));

EXTERN Void nhProcGenTmrExpiry ARGS((Ptr   cb,       
                                     S16   tmrEvnt));

EXTERN S16 nhProcLLyrCfgTmrExpiry ARGS((Ptr   cb));

EXTERN S16 nhUIMSndCountCfm ARGS((NhuDRBCountInfoLst   *nhuDRBCountInfoLst));
EXTERN S16 nhUIMDatReqHndlr ARGS((NhuDatReqSdus *nhuDatReqSdu, 
                                  NhActvUeCb   *ueCb));

EXTERN S16 nhUIMCellCfgHndlr ARGS((NhuCellCfgReqSdus *nhuCellCfgSdu, 
                                   NhCellCb          *cellCb, 
                                   NhuErrorInfo      *errInfo));

EXTERN S16 nhUIMSndCellCfgCfm ARGS((U8       cfgCfmStatus, 
                                    NhCellCb *cellCb,
                                    U8        errCause));

EXTERN S16 nhUIMDatInd ARGS((NhuDatIndSdus *nhuDatIndSdu));

EXTERN S16 nhUIMSndErrInd ARGS((NhuErrIndSdus *nhuErrInd));

EXTERN S16 nhUIMSndStaInd ARGS((NhuStaIndSdus *nhuStaInd));

EXTERN S16 nhUIMSndDatCfm ARGS((NhuDatCfmSdus *nhuDatCfm));

EXTERN S16 nhUIMSndCfgCfm ARGS((NhId        nhId,
                                NhTransId   nxTransId,
                                U8          cfgMsk,
                                U8          cfmStatus,
                                U8          errCause));

EXTERN S16 nhValidateSRBforMsgRx ARGS ((NhActvUeCb *ueCb, 
                                        U8 pdcpId,
                                        U8 msgType));

EXTERN S16 nhUTLEncAndSnd ARGS((NhCellAndUeCb  *cellAndUeCb,
                                U32                 nxTransId,
                                NhMsgPduAndType     *msgPduAndType,
                                NhDatReqParam       *datReqParam,
                                NhuErrorInfo        *errInfo));

EXTERN S16 nhUTLEncAndStoreMsg ARGS((NhMsgPduAndType     *msgPduAndType,
                                     NhDatReqParam       *datReqParam,
                                     TransCb             *transCb,
                                     NhuErrorInfo        *errInfo));

EXTERN S16 nhUTLSendStoredMsg ARGS((NhCellAndUeCb *cellAndUeCb,
                                    TransCb      *transCb));

EXTERN TransCb *nhUTLGetTransCbFrmNxTransId ARGS((NhActvUeCb  *ueCb,
                                               NhTransId   transId));

EXTERN TransCb *nhUTLGetTransCbFrmMsgTransId ARGS((NhActvUeCb *ueCb,
                                                U32        msgTransKey));

EXTERN S16 nhUTLGetRrcTransId ARGS((Void       *pdu,
                                 U8         msgChoice,
                                 NhSduType  sduType,
                                 U8         *rrcTransId));

EXTERN S16 nhUTLGetMsgTypeAndEvntFrmSdu ARGS((U8          msgChoice,
                                           NhSduType   sduType,
                                           U8          *nhMsgType,
                                           U8          *event));

#ifndef NH_NO_STS
EXTERN S16 nhUTLUpdGenSts ARGS((CntrType cntrType));

EXTERN S16 nhUTLUpdPduSts ARGS((NhActvUeCb  *ueCb,
                             MsgType     msgType,
                             Cause       cause));
EXTERN S16 nhUTLUpdCellPduSts ARGS((NhCellCb   *cellCb,
                                 MsgType    msgType,
                                 Cause      cause));
#endif /* NH_NO_STS */

EXTERN S16 nhUTLValidateDatRsp ARGS((NhuDatRspSdus  *datRspSdus,
                                         NhCellAndUeCb  *cellAndUeCb,
                                         U8             *msgTransId,
                                         U8             *event,
                                         NhuErrorInfo   *errInfo));

EXTERN S16 nhUTLValidateCfgReq ARGS((NhuCfgReqSdus *cfgReqSdus,
                                  NhActvUeCb    **ueCb,
                                  NhCellCb      **cellCb,
                                  NhuErrorInfo  *errInfo));

EXTERN S16 nhUTLValidateCountReq ARGS((NhuHdr *countHdr,
                                       NhuErrorInfo   *errInfo,
                                       U8             *event));

EXTERN S16 nhUTLValidateDatReq ARGS((NhuDatReqSdus  *datReqSdus,
                                  NhCellAndUeCb  *cellAndUeCb,
                                  U8             *msgTransId,
                                  NhuErrorInfo   *errInfo,
                                  U8             *event));

EXTERN S16 nhUTLValidateUeCfg ARGS((NhuUeCfgInfo   *ueCfg,
                                    NhActvUeCb     *ueCb,
                                    NhuErrorInfo   *errInfo));

EXTERN S16 nhUTLValidateCellCfgReq ARGS((NhuCellCfgReqSdus *cellCfgReqSdus,
                                      NhCellCb          *cellCb,
                                      NhuErrorInfo   *errInfo));

EXTERN NhLSapCb* nhUTLFindLlyrSapFrmSuId ARGS((U8      sapType,
                                            SuId    lSapId));

EXTERN S16 nhUTLValidateSRBforMsgRx ARGS((NhActvUeCb *ueCb, 
                                          U8 rbId,
                                          U8 msgType));

EXTERN S16 nhUTLFillDfltConfig ARGS ((NhCellAndUeCb    *cellAndUeCb,
                                      NhuUeCfgInfo  *ueCfg));

EXTERN S16 nhCPMHndlDlDirTx ARGS((NhDatReqRspSdu   *datReqRspSdu,
                                  NhCellAndUeCb    *cellAndUeCb,
                                  U8               msgTransId,
                                  NhuErrorInfo     *errInfo));

EXTERN S16 nhCPMHndlPagingMsg ARGS((NhDatReqRspSdu   *datReqRspSdu,
                                    NhCellAndUeCb    *cellAndUeCb,
                                    U8               msgTransId,
                                    NhuErrorInfo     *errInfo));

#ifdef LTERRC_REL9 
EXTERN S16 nhCPMUEInfoReqHndlr ARGS((NhDatReqRspSdu   *datReqRspSdu,
                                  NhCellAndUeCb    *cellAndUeCb,
                                  U8               msgTransId,
                                  NhuErrorInfo     *errInfo));
#endif /*LTERRC_REL9*/

EXTERN Void nhUTLCreateDbgFile ARGS((Void));

EXTERN S16 nhCPMUeCapEnqHndlr ARGS((NhDatReqRspSdu *datReqRspSdu, 
                                  NhCellAndUeCb    *cellAndUeCb,
                                  U8 msgTransId,  NhuErrorInfo *errInfo));

EXTERN S16 nhCPMUeCapInfoHndlr ARGS((NhuDatIndSdus *nhuDatIndSdu, 
                                   NhCellAndUeCb    *cellAndUeCb,
                                   U8 msgTransId));

EXTERN S16 nhCGMCountReqHndlr ARGS((NhDatReqRspSdu *datReqRspSdu, 
                                  NhCellAndUeCb    *cellAndUeCb,
                                  U8 msgTransId));

EXTERN S16 nhCGMCountCfmHndlr ARGS((NhuDatIndSdus *nhuDatIndSdu, 
                                  NhCellAndUeCb    *cellAndUeCb,
                                  U8 msgTransId));

EXTERN S16 nhCCMCountChkHndlr  ARGS((NhDatReqRspSdu *datReqRspSdu, 
                                   NhCellAndUeCb    *cellAndUeCb,
                                   U8 msgTransId,  NhuErrorInfo *errInfo));

EXTERN S16 nhCCMUlDedMsgHndlr ARGS((NhuDatIndSdus *nhuDatIndSdu,
                                    NhCellAndUeCb *cellAndUeCb,
                                    U8 msgTransId,
                                    U8 msgType));


/* Handover related functions */
EXTERN S16 nhCPMMobFrmEutraCmd ARGS((NhDatReqRspSdu   *datReqRspSdu,
                                     NhCellAndUeCb    *cellAndUeCb,
                                     U8 msgTransId,
                                     NhuErrorInfo     *errInfo));
EXTERN S16 nhCPMHoFrmEutraPrepTfr ARGS((NhDatReqRspSdu   *datReqRspSdu,
                                        NhCellAndUeCb    *cellAndUeCb,
                                        U8 msgTransId,
                                        NhuErrorInfo     *errInfo));
EXTERN S16 nhCPMCDMACsfbParamRsp ARGS((NhDatReqRspSdu   *datReqRspSdu,
                                       NhCellAndUeCb    *cellAndUeCb,
                                       U8 msgTransId,
                                       NhuErrorInfo     *errInfo));

typedef S16 (*PCDFLIS16) ARGS ((NhuDatIndSdus *nhDatSdus, 
                              NhCellAndUeCb   *cellAndUeCb,
                              U8 msgTransId, U8 msgType));

/*Connection re-establishment funtions*/
EXTERN S16 nhCCMConRestbHdlr ARGS (( NhDatReqRspSdu    *datReqRspSdu,
                                     NhCellAndUeCb   *cellAndUeCb,
                                     U8                msgTransId,
                                     NhuErrorInfo      *errInfo));
EXTERN S16 nhCCMConRestbCmpHndlr ARGS((NhuDatIndSdus *nhuDatIndSdu,
                                       NhActvUeCb *ueCb,
                                       U8 msgTransId));
EXTERN S16 nhUIMSndPdcpSduStaCfm ARGS ((NhuPdcpSduStaCfm *pdcpCfgCfm,
                                        U8            cfgMsk,
                                        U8            cfmStatus,
                                        NhActvUeCb    *ueCb));
EXTERN S16 nhUTLGetRbToRestb ARGS(( TransCb         *transCb,
                                 NhRbCfgList     crntRbCfgList));

EXTERN S16 nhUTLFreeUlBitMapFrmCrntRbCfg ARGS((NhCrntCfgInfo *crntCfgInfo));
EXTERN S16 nhUTLFreeUlBitMapFrmOrdCfg ARGS((TransCb *transCb, U8 numRbs));
EXTERN S16 nhPIMSndSduStaReq ARGS((NhId        nhId,
                                   TransCb     *transCb,
                                   NhCellAndUeCb  *cellAndUeCb,
                                   U8          status));
EXTERN S16 nhPIMSendPdcpDatResumeReq ARGS ((NhActvUeCb *ueCb,
                                            NhTransId transId));

EXTERN S16 nhPIMSendStartPdcpDataFwdReq ARGS ((NhActvUeCb *ueCb,
                                               Bool isUlDatFwdReq));
 
EXTERN S16 nhPIMRlcSndRestbCfg ARGS ((TransCb *transCtrlBlk, NhActvUeCb *ueCb));

EXTERN S16 nhUIMSndPdcpDatResumeCfm
ARGS ((
NhuPdcpDatResumeCfm *pdcpDatResumeCfm,
NhActvUeCb       *ueCb
));

#ifdef NH_ASN_COND_VALIDATE
EXTERN S16 nhUTLValidateAsnCondIes ARGS((Void         *msgSdu,
                                         U8           msgType,
                                         NhuErrorInfo *errInfo,
                                         U8           evtType));

EXTERN S16 nhUTLValidateCondIesReCfgHo ARGS((NhuRRCConRecfgn_r8_IEs   *reCfgIE,
                                             NhCellAndUeCb   *cellandUeCb));

EXTERN S16 nhUTLValidateCondIesRadResDed ARGS((
                                 NhuRadioResourceConfigDedicated *radioResDed,
                                 NhCellAndUeCb                *cellandUeCb,
                                 Bool                         isHo));

EXTERN S16 nhUTLValidateCondIesRadResPhyCfgDed ARGS((
                                 NhuRadioResourceConfigDedicated *radioResDed,
                                 NhCellAndUeCb                  *cellandUeCb));

EXTERN S16 nhUTLValidateCondIesMeasCfg ARGS((NhuMeasConfig         measCfg));

#ifdef LTERRC_REL9
EXTERN S16 nhUTLValidateCondIesReportCfg ARGS((
         NhuReportConfigToAddModreportConfig  reportCfg));
#endif /* LTERRC_REL9*/

EXTERN S16 nhUTLValidateCondIesRRCConnSetup ARGS ((
                                 NhuDatRspSdus *nhuDatRspSdus,
                                 NhCellAndUeCb *cellandUeCb));

EXTERN Bool nhUTLIsRbAvailInLst ARGS((
                                 CmLListCp   *rbCfgInfoList,
                                 U32         rbId,
                                 Bool       isDRB,
                                 NhActvUeCb *ueCb));


EXTERN S16 nhUTLValidateCondIesRRCConnReCfg ARGS((
                                 NhuRRCConRecfgn_r8_IEs *ies,
                                 NhCellAndUeCb          *cellandUeCb));

EXTERN S16 nhUTLValidateCondIesSibs ARGS((
                                 NhuDatReqSdus *nhuDatReqSdus,
                                 NhCellAndUeCb *cellandUeCb));

EXTERN S16 nhUTLValidateCondIesMobFrmEutra ARGS((
                                 NhuDatReqSdus *nhuDatReqSdus,
                                 NhActvUeCb               *ueCb));

EXTERN S16 nhUTLValidateCondIesHoFrmEutraPrepTfr ARGS((
                                 NhuDatReqSdus *nhuDatReqSdus,
                                 NhActvUeCb               *ueCb));

EXTERN S16 nhUTLValidateCondIesHoPrepInfo ARGS((
                                 NhuEncReqSdus   *nhuEncReqSdus,
                                 NhCellAndUeCb   *cellandUeCb));

EXTERN Void nhUTLValidateCondIesRRCSysInfo ARGS((
                                 NhDatReqRspSdu *nhDatReqRspSdu,
                                 U8             *errorCause));

EXTERN S16 nhUTLValidateCondIesRbs ARGS((
                                 NhuRadioResourceConfigDedicated *radioResDed,
                                 NhActvUeCb                      *ueCb));

EXTERN S16 nhUTLValidateCondIesRb_DRB_RLCCfg ARGS((
                                 NhuRadioResourceConfigDedicated *radioResDed,
                                 NhActvUeCb                      *ueCb));

EXTERN S16 nhUTLValidateCondIesRb_DRB_LgclCfg ARGS((
                                 NhuRadioResourceConfigDedicated *radioResDed,
                                 NhActvUeCb                      *ueCb));

EXTERN S16 nhUTLValidateCondIesRb_DRB_PdcpCfg ARGS((
                                 NhuRadioResourceConfigDedicated *radioResDed,
                                 NhActvUeCb                      *ueCb));

EXTERN S16 nhUTLValidateCondIesRb_DRB_Ul_SpParam ARGS((
                                 NhuRadioResourceConfigDedicated *radioResDed,
                                 NhActvUeCb                      *ueCb));

EXTERN S16 nhUTLValidateCondIesRb_DRB_Othr ARGS((
                                 NhuRadioResourceConfigDedicated *radioResDed,
                                 NhActvUeCb                      *ueCb));
#endif /* NH_ASN_COND_VALIDATE */

EXTERN S16 nhUTLValidateLSapSuId ARGS((U16       elmntId,
                                       SuId      suId));

EXTERN S16 nhUTLValidateDatRspDlCChMsg ARGS((
                                 NhuDatRspSdus  *datRspSdus,
                                 NhCellAndUeCb  *cellAndUeCb,
                                 NhuErrorInfo   *errInfo,
                                 U16            *crnti,
                                 U8             *msgTransId,
                                 U8             *event,
                                 MsgType        *msgType));

EXTERN S16 nhUTLValidateUeCfgRbAdd ARGS((
                                 NhuUeCfgInfo   *ueCfg,
                                 NhActvUeCb     *ueCb,
                                 NhuErrorInfo   *errInfo));

#ifdef RRC_PHY_CFG_SUPP
/* PHY configuration related RRC functions. */
EXTERN S16 nhPIMSendPhyCellCfg ARGS((NhCellCb *cellCb));

EXTERN S16 nhPIMSendPhyCfg ARGS((NhActvUeCb *ueCb,
                                 TransCb    *transCb));

EXTERN S16 nhPIMSendPhyUeIdChgReq ARGS((NhActvUeCb *ueCb,
                                  TransCb    *transCb));

EXTERN S16 nhPIMSendPhyAddCfg ARGS((NhActvUeCb *ueCb,
                                 TransCb    *transCb));

EXTERN S16 nhPIMSendPhyModCfg ARGS((NhActvUeCb *ueCb,
                                 TransCb    *transCb));

EXTERN S16 nhPIMSendPhyRlsCfg ARGS((NhActvUeCb *ueCb));

EXTERN S16 nhPIMRvrtPhyCellCfg ARGS((NhCellCb *cellCb));

EXTERN S16 nhPIMRvrtPhyCfg ARGS((NhActvUeCb *ueCb,
                                 TransCb    *transCb));
EXTERN S16 nhPIMRvrtPhyAddCfg ARGS((NhActvUeCb *ueCb,
                                    TransCb    *transCb));
EXTERN S16 nhPIMRvrtPhyModCfg ARGS((NhActvUeCb *ueCb,
                                    TransCb    *transCb));
#endif /* RRC_PHY_CFG_SUPP */

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* __NHX__ */

/**********************************************************************
         End of file:     nh.x@@/main/3 - Fri Jul  1 01:13:12 2011
**********************************************************************/

/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      chebli           1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
           nh002.301  mpatel           1.[ccpu00118385]:nhuSduStaCfm is added in TransCb
*********************************************************************91*/
