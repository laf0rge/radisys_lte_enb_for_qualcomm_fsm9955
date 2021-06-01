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
  
     Desc:     Defines required by LTE RRC.

     File:     nh.h

     Sid:      nh.h@@/main/3 - Fri Jul  1 01:13:11 2011

     Prg:      vk

**********************************************************************/

/*
*     The defines in this file correspond to structures and variables 
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1096369        LTE RRC
*
*/

#ifndef __NHH__
#define __NHH__

/* header include files (.h) */

#ifdef RLOG_REDIRECT
#define NH_REDIRECT_RLOGS 1
#endif

#include "rl_interface.h"    /*Binary logging interface*/
/** 
 * Macro for LTE RRC layer name
 */
#define NHLAYERNAME  "LTE-RRC"

/** 
 * RRC State Machine Events for Lower Interface 
 */
#define NH_EVNT_LI_INVALID                       0  /*!< RRC Invalid Event */
#define NH_EVNT_LI_RRC_CON_REQ                   1  /*!< RRC Connection Request */
#define NH_EVNT_LI_RRC_REESTAB_REQ               2  /*!< RRC Re-Establishment Request */
#define NH_EVNT_LI_UL_DT                         3  /*!< UL  Data Transfer */
#define NH_EVNT_LI_RRC_CON_SETUP_CMP             4  /*!< RRC Connection Setup Complete */
#define NH_EVNT_LI_RRC_CON_RECFG_CMP             5  /*!< RRC Connection Re-Configuration Complete */
#define NH_EVNT_LI_SEC_MOD_CMP                   6  /*!< RRC Security Mode Complete */
#define NH_EVNT_LI_SEC_MOD_FAIL                  7  /*!< RRC Security Mode Fail */
#define NH_EVNT_LI_RRC_CON_RLS_CMP               8  /*!< RRC Connection Release Complete */
#define NH_EVNT_LI_UL_HO_PREP_TFR                9  /*!< RRC UL Handover Prepare Transfer */  
#define NH_EVNT_LI_CSFB_PARAM_REQ                10 /*!< RRC CSFB Param Request */
#define NH_EVNT_LI_UE_CAP_INFO                   11 /*!< UE Capability Info*/
#define NH_EVNT_LI_COUNT_CHK_RSP                 12 /*!< Counter Check Response*/
#define NH_EVNT_LI_RRC_MES_RPT                   13 /*!< Measurement Report */
#define NH_EVNT_LI_RRC_CON_RESTB_CMP             14 /*!< Connection Re-Establishment complete */
#ifdef LTERRC_REL9
#define NH_EVNT_LI_UE_INFO_RSP                   15 /*!< UE Information response*/
#define NH_EVNT_LI_PRXMTY_IND                    16 /*!< Proximity Indication */
#endif /* LTERRC_REL9 */

/** 
 * This is the last in LI events, add new events above this
 * and change NH_EVNT_LI_MAX_EVNTS accordingly  
 */
#ifdef LTERRC_REL9
#define NH_EVNT_LI_MAX_EVNTS                     17 /* RRC Lower Interface Max Events */
#else /*LTERRC_REL9*/
#define NH_EVNT_LI_MAX_EVNTS                     15 /* RRC Lower Interface Max Events */
#endif /*LTERRC_REL9*/




/** 
 * RRC State Machine Events for Upper Interface 
 */
#define NH_EVNT_UI_INVALID                      (NH_EVNT_LI_MAX_EVNTS + 0)  /* Invalid Event */
#define NH_EVNT_UI_DL_DT                        (NH_EVNT_LI_MAX_EVNTS + 1)  /* DL Data Transfer */
#define NH_EVNT_UI_SEC_MOD_CMD                  (NH_EVNT_LI_MAX_EVNTS + 2)  /* Security Mode Command */
#define NH_EVNT_UI_RRC_CON_RECFG                (NH_EVNT_LI_MAX_EVNTS + 3)  /* RRC Connection Reconfig */
#define NH_EVNT_UI_RRC_CON_RLS                  (NH_EVNT_LI_MAX_EVNTS + 4)  /* RRC Connection Release */
#define NH_EVNT_UI_SYS_INFO_TX                  (NH_EVNT_LI_MAX_EVNTS + 5)  /* System Info. Transmission */
#define NH_EVNT_UI_PAGING_MSG                   (NH_EVNT_LI_MAX_EVNTS + 6)  /* Paging Message */
#define NH_EVNT_UI_CONN_REESTB                  (NH_EVNT_LI_MAX_EVNTS + 7)  /* RRC Connection Re-Establishment */
#define NH_EVNT_UI_CONN_REESTB_REJ              (NH_EVNT_LI_MAX_EVNTS + 8)  /* RRC Connection Re-Establishment Reject */
#define NH_EVNT_UI_CONN_SETUP                   (NH_EVNT_LI_MAX_EVNTS + 9)  /* RRC Conncection Setup */
#define NH_EVNT_UI_CONN_REJ                     (NH_EVNT_LI_MAX_EVNTS + 10) /* RRC Connection Reject */
#define NH_EVNT_UI_MOB_FRM_EUTRA_CMD            (NH_EVNT_LI_MAX_EVNTS + 11) /* Mobility from EUTRA Command */
#define NH_EVNT_UI_HO_FRM_EUTRA_PRP_TFR         (NH_EVNT_LI_MAX_EVNTS + 12) /* HO from EUTRA Prepare Transfer */
#define NH_EVNT_UI_CSFB_PARAM_RSP               (NH_EVNT_LI_MAX_EVNTS + 13) /* CSFB Params Response CDMA2000 */
#define NH_EVNT_UI_UE_CAP_ENQ                   (NH_EVNT_LI_MAX_EVNTS + 14) /* UE Capability Enquiry */
#define NH_EVNT_UI_COUNT_CHK                    (NH_EVNT_LI_MAX_EVNTS + 15) /* Counter Check */
#ifdef LTERRC_REL9 
#define NH_EVNT_UI_UE_INFO_REQ                  (NH_EVNT_LI_MAX_EVNTS + 16) /*UE information request Event*/
#endif /* LTERRC_REL9 */

#define NH_INVALID_RRC_TRANS_ID              0xff  /* RRC Invalid Transaction ID */

/**  
 * This is the last in UI events, add new events above this
 * and change NH_EVNT_UI_MAX_EVNTS accordingly  
 */
#ifdef LTERRC_REL9 
#define NH_EVNT_UI_MAX_EVNTS                     17 /* RRC Upper Interface Max Events */
#else /* LTERRC_REL9 */
#define NH_EVNT_UI_MAX_EVNTS                     16 /* RRC Upper Interface Max Events */
#endif /* LTERRC_REL9*/


/**  
 * This section defines the macros used in nh.x  
 */
/* Common RBs: BCCH-BCH, BCCH-DLSCH1, BCCH-DLSCH2, 
 *             PCCH-PCH, CCCH-DLSCH, CCCH-ULSCH 
 */
#define NH_MAX_RB_PER_CELL_CFG                     8 /* Max Radio Bearer Per Cell Configuration */
#define NH_MAX_CFGD_RB_CELL                        6 /* Max Radio Bearer Configured per Cell */
#define NH_MAX_MAC_CELL_CFG_ELMNT                  2 /* Max MAC Cell configuration elements */
#define NH_MAX_MAC_UE_CFG_ELMNT                    2 /* Max MAC UE Configuration elements */

/**  
 * This section defines the macros used in nh.x  
 */
#define NH_LCID_DIR_TXRX           (NH_LCID_DIR_TX | NH_LCID_DIR_RX) /* LCID Direction - Tx/Rx */
#define NH_LCID_DIR_TX                          0x01 /* LCID Direction Tx */
#define NH_LCID_DIR_RX                          0x02 /* LCID Direction Rx */

/** 
  * This section defines the maximum count variables
  */
#define NH_MAX_LC_PER_RB                          2 /* Max. Logical Channel Per Radio Bearer */
#define NH_DL_LC                                  0 /* RRC DL Logical Channels */
#define NH_UL_LC                                  1 /* RRC UL Logical Channels */

#define NH_MAX_SEC_CFG                            2 /* RRC Max. Security Configuration */
#define NH_INTG_CFG                               0 /* RRC Integrity Configuration */
#define NH_CIPH_CFG                               1 /* RRC Ciphering Configuration */

#define NH_STATUS_UNUSED                          0 /* RRC Unused Status */ 
#define NH_STATUS_USED                            1 /* RRC Used Status */ 

/** 
 * This section defines the cell configuration actions used in the
 * transaction control block.
 */
#define NH_CFG_ACTN_ADD      0x01 /* Configuration Action Add */
#define NH_CFG_ACTN_MODIFY   0x02 /* Configuration Action Modify */
#define NH_CFG_ACTN_RLS      0x04 /* Configuration Action Release */

/**  
 * This section defines the hashlist parameters.
 */
#define NH_NEW_UECB_BINS   32 /* New UE Control Block Bins */

#define NH_UECB_BINS       256 /* UE Control Block Bins */
#define NH_UECB_OFFSET     0   /* UE Control Block Offset */

#define NH_CELLCB_BINS       256 /* Cell Control Block Bins */
#define NH_CELLCB_OFFSET     0   /* Cell Control Block Offset */

/**  
 * This section defines the hashlist parameters.
 */
#define NH_TRANSCB_BINS       256 /* Transaction Control Block bins */
#define NH_TRANSCB_OFFSET     0  /* Transaction Control Block Offset */


/**  
 * This section defines the reserved transaction ID.
 */
#define NH_RESERVED_TRANS_ID       0xffffffff /* RRC Reserved Transaction ID */



/** 
 *  These macros define the primitive types.
 */
#define NH_DAT_REQ_SDU   0x00 /* RRC Data Request SDU */
#define NH_DAT_RSP_SDU   0x01 /* RRC Data Response SDU */


/** 
 *  These macros define the different waitFlag states.
 */
#define NH_AWAIT_NO_RSP          0x00 /* No Response Wait Flag */
#define NH_AWAIT_UE_RSP          0x01 /* UE Response Wait flag */
#define NH_AWAIT_DAT_CFM         0x02 /* Data Confirm Wait Flag */


/** 
 *  These macros define the SRB1 configuration status
 *  and security procedure is initiated or completed.
 */
#define NH_SRB1_CFGD          0x01 /* SRB1 Configured */
#define NH_SEC_INIT           0x02 /* Security CMD initiated */
#define NH_SEC_CFG_DONE       0x04 /* Security Config Confirm  (Success) received */
#define NH_SEC_CMPLTD         0x08 /* Security CMD Complete Received */

/**
 *  This macro define UE control block state.
 */
#define NH_CON_RESTB_CMP   0x01 /* Connection Restablish complete */


/* 
 * Radio Bearer Ids
 */
#define NH_RB_ID_SRB0         0 /* SRB0 Radio Bearer ID */
#define NH_RB_ID_SRB1         1 /* SRB1 Radio Bearer ID */
#define NH_RB_ID_SRB2         2 /* SRB2 Radio Bearer ID */

/** 
 *  These macros define the RRC Message Types.
 */
#define   NH_IGNORE                      0  /* Ignore Message */
#define   NH_MSGTYPE_SIB1                1  /* SIB1 Message */
#define   NH_MSGTYPE_SI                  2  /* SI Message */
#define   NH_MSGTYPE_PAG                 3  /* Paging */
#define   NH_MSGTYPE_CONNREQ             4  /* RRC Connection Request */
#define   NH_MSGTYPE_CONNSETUP           5  /* RRC Connection Setup */
#define   NH_MSGTYPE_CONNSETUPCOMP       6  /* RRC Connection Setup Complete */
#define   NH_MSGTYPE_CONNREJ             7  /* RRC Connection Reject */
#define   NH_MSGTYPE_CONNRECFG           8  /* RRC Connection Reconfiguration */
#define   NH_MSGTYPE_CONNRECFGCOMP       9  /* RRC Connection Reconfiguration Complete */
#define   NH_MSGTYPE_CONNRLS            10  /* RRC Connection Release */
#define   NH_MSGTYPE_DLTFR              11  /* Downlink Information Transfer */
#define   NH_MSGTYPE_ULTFR              12  /* Uplink Information Transfer */
#define   NH_MSGTYPE_CONNREESTABREQ     13  /* RRC Connection Restablishment Request */
#define   NH_MSGTYPE_CONNREESTAB        14  /* RRC Connection Restablishment */
#define   NH_MSGTYPE_CONNREESTABCOMP    15  /* RRC Connection Restablishment Complete */
#define   NH_MSGTYPE_CONNREESTABREJ     16  /* RRC Connection Restablishment Reject */
#define   NH_MSGTYPE_SECMODCMD          17  /* Security Mode Command */
#define   NH_MSGTYPE_SECMODCOMP         18  /* Security Mode Complete */
#define   NH_MSGTYPE_SECMODFAIL         19  /* Security Mode Failure */
#define   NH_MSGTYPE_UECAPENQ           20  /* UE Capability Enquiry*/
#define   NH_MSGTYPE_UECAPINFO          21  /* UE Capability Info*/
#define   NH_MSGTYPE_COUNTCHK           22  /* Counter Check*/
#define   NH_MSGTYPE_COUNTCHKRSP        23  /* Counter Check Response*/
#define   NH_MSGTYPE_MEASUREMENTRPT     24  /* Measurement report */
#define   NH_MSGTYPE_MOBFRMEUTRAN       25  /* Mobility From E-UTRA Cmd */
#define   NH_MSGTYPE_HOFRMEUTRAPREPREQ  26  /* HO from E-UTRA Preparation Request - CDMA2000 */
#define   NH_MSGTYPE_ULHOPREPTFR        27  /* UL HO preparation transfer - CDMA2000 */
#define   NH_MSGTYPE_CSFBPARAMREQCDMA   28  /* CSFB Parameters Request CDMA2000 */
#define   NH_MSGTYPE_CSFBPARAMRSPCDMA   29  /* CSFB Parameters Response CDMA2000 */
#define   NH_MSGTYPE_CONNRECFGCOMP_HO   30  /* RRC Connection Reconfiguration Complete 
                                               during HO */
#define   NH_MSGTYPE_HOPREPINFO         31  /* HO Preparation Infomation - inter-node msg */
#ifdef LTERRC_REL9 
#define   NH_MSGTYPE_UEINFOREQ          32  /* UE information request */
#define   NH_MSGTYPE_UEINFORSP          33  /* UE information response */
#define   NH_MSGTYPE_PRXMTYIND          34  /* Proximity Indication */
#endif /*LTERRC_REL9*/

/** Defines for General Statistics Cause
  */
#define NH_STS_CAUSE_CON_SUCC              1    /* RRC Connection Successful */
#define NH_STS_CAUSE_CON_FAIL              2    /* RRC Connection Failure */
#define NH_STS_CAUSE_RECFG_SUCC            3    /* RRC Reconfiguration
                                                   Successful */
#define NH_STS_CAUSE_RECFG_FAIL            4    /* RRC Reconfiguration Failure */
#define NH_STS_CAUSE_LOWER_LYR_FAIL        5    /* Lower Layer Configuration
                                                   Failure */
#define NH_STS_CAUSE_DEC_ERR               6    /* Message Decode Failure */
#define NH_STS_CAUSE_ENC_ERR               7    /* Message Encode Failure */
#define NH_STS_CAUSE_PDCP_ERR              8    /* PDCP related errors:
                                                   Integrity Verification,
                                                   Deciphering Failure and SDU
                                                   Discard Timer Expired */
#define NH_STS_CAUSE_RLC_ERR               9    /* RLC related errors:
                                                   Maximum Transmissions Reached */

#define NH_STS_CAUSE_CON_RESTB_SUCC        10   /*RRC Conn Re-establishemnt  Successful*/
#define NH_STS_CAUSE_CON_RESTB_FAIL        11   /*RRC Conn Re-establishemnt  Successful*/

/* This is the value for transCb->cause while creating it through NhUiNhuCfgReq
  * It must be more than the NH_MSGTYPE_NOTIMPLEMENTED, since for other cases
  * NH_MSGTYPE_ as the cause for transCb creation is used. 
  */
  
#define   NH_TRANSCB_CFG_REQ            0xEE  /* Transaction Control Block Config Request */
#define   NH_TRANSCB_SDU_STA_RESTAB     0xEF  /* Transaction Control Block SDU Status Re-establish */

#define NH_TRANSCB_DATA_FWD_HO          0xF0 /* Transaction Control Block Data Forwarding Request */
#define NH_TRANSCB_DATA_RESUME          0xF1 /* Transaction Control Block Data Forwarding Request */
/** 
 *  defines for Statistics Cause
 */
#define NH_STS_CAUSE_SUCCESS          0 /* Message successfully received */
#define NH_STS_CAUSE_MOB_FRM_EUTRA_FAIL    1   /* Mobility from E-UTRA failure at source */
#define NH_STS_CAUSE_HO_TO_EUTRA_SUCC      2   /* Handover to E-UTRA success at target */

/** 
 * This section describes the macros for RRC Encode/Decode module.
 */
#define NH_PASN_MAX_BLOCK_SIZE          4096 /* Max ASN Block Size */
#define NH_PASN_DBG_FLAG                TRUE /* ASN Debug Flag */

/** 
 * Defines for bind confirm. 
*/
#define NH_TMR_WAIT_CPJ_BNDCFM  1 /* CPJ Interface Bind confirm */
#define NH_TMR_WAIT_CKW_BNDCFM  2 /* CKW Interface Bind confirm */
#define NH_TMR_WAIT_CRG_BNDCFM  3 /* CRG Interface Bind confirm */
#define NH_TMR_WAIT_PJU_BNDCFM  4 /* PJU Interface Bind confirm */
#define NH_TMR_WAIT_KWU_BNDCFM  5 /* KWU Interface Bind confirm */
#define NH_TMR_CELL_CFGCFM      6 /* Cell Confiuration confirm */
#define NH_TMR_UE_LLYR_CFGCFM   7 /* Lower Layer Config confirm */
#ifdef RRC_PHY_CFG_SUPP
#define NH_TMR_WAIT_CTF_BNDCFM  8 /* CTF Interface Bind confirm */
#endif /* RRC_PHY_CFG_SUPP */

/** 
 * Defines for timer.
*/
#define NH_TMR_STOP   0 /* RRC Timer Stop */
#define NH_TMR_START  1 /* RRC Timer Start */
 
/** 
 * Other timer defines.
TBD verify NH_MAX_CFG_TMRS
*/
#define NH_MAX_TMRS             255 /* Max Number of Timers */
#define NH_MAX_CFG_TMRS         64  /* Max Confgiure Timers */
#define NH_MAX_LSAP_TMRS        8   /* Lower SAP Timers */
#define NH_BND_INTRETRY         3   /* Bind Interrupt Retry */
#define NH_GEN_TMR_TIMEOUT      8000 /* General Timer Timeout */
#define NH_TQNUMENT             64   /* General Timer Queue */

/* Macro for Ordered Configuration Action Mask */
#define NH_ORD_CFG_RB_ADD           0x0001 /* Ordered Configure Radio Bearer Add */
#define NH_ORD_CFG_RB_MOD           0x0002 /* Ordered Configure Radio Bearer Modify */
#define NH_ORD_CFG_RB_RLS           0x0004 /* Ordered Configure Radio Bearer Release */
#define NH_ORD_CFG_TRCH             0x0008 /* Ordered Configure Transport Channel*/
#define NH_ORD_CFG_SCHED            0x0010 /* Ordered Configure Schedule */
#define NH_ORD_CFG_SEC              0x0020 /* Ordered Configure Security */
#define NH_ORD_CFG_UEID             0x0040 /* Ordered Configure UEID */
#define NH_ORD_CFG_RB_RESTB         0x0080 /* Ordered Configure RB Re-establishment */
#ifdef RRC_PHY_CFG_SUPP
#define NH_ORD_CFG_PHYCH            0x0100 /* Ordered Configure PHY Channel */
#endif /* RRC_PHY_CFG_SUPP */
#define NH_ORD_CFG_RESTB_SRB1       0x0200 /* Ordered Configure Re-establish SRB1 */
#define NH_ORD_CFG_RESET_MAC        0x0400 /* Ordered Configure Reset MAC */

/* Macro for Current Configuration Bitmask */
#define NH_CRNT_CFG_RB              0x0001 /* Current Configured Radio Bearer */
#define NH_CRNT_CFG_LOGCHCB         0x0002 /* Current Configured Logical Channel Control Block */
#define NH_CRNT_CFG_TRCH            0x0004 /* Current Configured Transport Channel */
#define NH_CRNT_CFG_SEC             0x0008 /* Current Configured Security */
#define NH_CRNT_CFG_SCHED           0x0010 /* Current Configured Schedule */
#ifdef RRC_PHY_CFG_SUPP
#define NH_CRNT_CFG_PHYCH           0x0020 /* Current Configured PHY Channel */
#endif /* RRC_PHY_CFG_SUPP */
#ifdef LTERRC_REL9
/* Added for SRB2 and DRBs Establishment status */ 
#define NH_CRNT_CFG_SRB2            0x0040 /* SRB2 Established */
#define NH_CRNT_CFG_DRBS            0x0080 /* Atleast one DRB Established */
#endif /* LTERRC_REL9*/

/* Macro for layers to be configured */
#define NH_LAYER_NONE            0x00   /* No Layer Configuration */
#define NH_RLC_CONFIGURED        0x01   /* RLC Configured */
#define NH_MAC_CONFIGURED        0x02   /* MAC Configured */
#define NH_PDCP_CONFIGURED       0x04   /* PDCP Configured */

#ifdef RRC_PHY_CFG_SUPP
#define NH_PHY_CONFIGURED        0x08   /* PHY Configured */
#endif /* RRC_PHY_CFG_SUPP */

/* Macro for MAC Cfg Actions */
#define NH_MAC_CFG_ADD            0x01  /* MAC Configuration Add */
#define NH_MAC_CFG_MODIFY         0x02  /* MAC Configuration Modify */
#define NH_MAC_CFG_RLS            0x04  /* MAC Configuration Release */
#define NH_MAC_CFG_RESET          0x08  /* MAC Configuration Reset */


/* Macro for MAC UE specific CFG Elements */
#define NH_CRG_UE_CFG               0x01 /* MAC UE Configuration */
#define NH_CRG_LOGCH_MAP            0x02 /* MAC Logical Channel Mapping */
#define NH_CRG_UE_MAC_RST           0x03 /* MAC UE Re-establishment */

/* Macro for MAC cell CFG Elements */
#define NH_CRG_CELL_LOGCH_MAP       0x01 /* MAC Cell Logical Channel Mapping */
#define NH_CRG_CELL_CFG             0x02 /* MAC Cell Configuration */

/* Macro for RLC Cfg Actions */
#define NH_RLC_CFG_ESTABLISH    0x01 /* RLC Configuration Establishment */
#define NH_RLC_CFG_RLS          0x02 /* RLC Configuration Release */
#define NH_RLC_CFG_RECONFIGURE  0x03 /* RLC Configuration Reconfigure */
#define NH_RLC_CFG_STOP         0x04 /* RLC Configuration Stop */
#define NH_RLC_CFG_CONTINUE     0x05 /* RLC Configuration Continue */
#define NH_RLC_CFG_REESTABLISH  0x06 /* RLC Configuration Restablishment */
#define NH_RLC_CFG_UEID_CHNG    0x07 /* RLC Configuration UEID Change */


/* Macro for PDCP actions */
#define NH_PDCP_CFG_ESTABLISH   0x01 /* PDCP Configuration Establish */
#define NH_PDCP_CFG_RLS         0x02 /* PDCP Configuration Release */
#define NH_PDCP_CFG_RECONFIGURE 0x03 /* PDCP Configuration Reconfigure */
#define NH_PDCP_CFG_UEID_CHNG   0x04 /* PDCP Configuration UEID Change */
#define NH_PDCP_CFG_RESTB_SRB1 0x05  /* PDCP Configuration Restablish SRB1 */
/* Macro for PDCP Status in Dat Cfm */
#define NH_PDCP_DAT_CFM_SUCCESS    0x01 /* PDCP Data Confirm Success */
#define NH_PDCP_DAT_CFM_DISCARD    0x00 /* PDCP Data Confirm Discard */

#ifdef RRC_PHY_CFG_SUPP
/* Macro for PHY UE specific CFG Elements */
#define NH_CTF_UE_CFG               0x01 /* PHY UE Configuration */
#define NH_CTF_CELL_CFG             0x02 /* PHY Cell Configuration */

/* Macro for PHY Cfg Actions */
#define NH_PHY_CFG_ADD            0x01 /* RRC PHY Configuration Add */
#define NH_PHY_CFG_MODIFY         0x02 /* RRC PHY Configuration Modify */
#define NH_PHY_CFG_RLS            0x04 /* RRC PHY Configuration Release */
#define NH_PHY_CFG_UEID_CHNG      0x08 /* RRC PHY Configuration UEID Change */
#endif /* RRC_PHY_CFG_SUPP */

/* Macro for Cfg Cfm result */
#define NH_CFG_RES_UNKNOWN  0x00 /* Result unknown */
#define NH_CFG_SUCCESS      0x01 /* Cfg success */
#define NH_CFG_FAILED       0x02 /* Cfg failed */

/* Macro for used/not used */
#define NH_NOT_USED     0x00 /* Value for unused */
#define NH_USED         0x01 /* Value for used */

/* NhuUeInfo Type */
#define NH_UE_INFO_NEW_CRNTI  0x01 /* UE Info for new CRNTI */
#define NH_UE_INFO_MOD_CRNTI  0x02 /* UE Info for modified CRNTI */
#define NH_UE_INFO_RLS_CRNTI  0x03 /* UE Info for Released CRNTI */



#define NH_ZERO_VAL             0 /* Zero Value */
#define NH_OCTET_VAL            8 /* Octet Value */

/* Hashlist size */
#define NH_UE_HASH_LIST_SIZE    67 /* UE Hash List Size */
#define NH_CELL_HASH_LIST_SIZE  67 /* Cell Hash List Size */

#define NH_MAX_CFG_REQ          32 /* RRC Max Configuration Request */
#define NH_MAC_MAP_RB_MAP_IDX   1  /* MAC Radio Bearer Mapping Index */
#define NH_CRG_RB_MAP           0x02 /* MAC Radio Bearer Map */
#define NH_UEID                 0x01 /* RRC UEID */
#define NH_CELLID               0x02 /* RRC Cell ID */
#define NH_MAC_MAP_CELL_MAP_IDX 1  /* MAC Cell Mapping Index */

#define NH_UE_MAPTBL_IDX_MAC            0 /* UE Mapping Table Index for MAC */
#define NH_UE_MAPTBL_IDX_RLC            1 /* UE Mapping Table Index for RLC */
#define NH_UE_MAPTBL_IDX_PDCP           2 /* UE Mapping Table Index for PDCP */
#define NH_RESTB_MAPTBL_IDX_PDCP_SRB1   0 /* Restablish Mapping Table Index for PDCP SRB1 */
#define NH_RESET_MAPTBL_IDX_MAC         0 /* Restablish Mapping Table Index for MAC */
#ifndef RRC_PHY_CFG_SUPP
#define NH_UE_MAPTBL_MAX_IDX    3 /* UE Mapping Table Index Max */
#else  /* RRC_PHY_CFG_SUPP */
#define NH_UE_MAPTBL_IDX_PHY    3 /* UE Mapping Table Index for PHY */
#define NH_UE_MAPTBL_MAX_IDX    4 /* UE Mapping Table Index Max */
#endif /* RRC_PHY_CFG_SUPP */
#define NH_RESTB_SRB1_TBL_MAX_IDX 1 /* Restablish Mapping Table Index for MAC */
#define NH_MAC_RST_MAX_IDX        1 /* Restablish Mapping Table Index for MAC */

/* Bitmask for NhLogChCfg */
#define NH_LOGCH_CFG_LCH         0x01 /* Logical Channel Configure */
#define NH_LOGCH_CFG_UL_TRCHTYP  0x02 /* Logical Channel configure UL */
#define NH_LOGCH_CFG_DL_TRCHTYP  0x04 /* Logical Channel configure DL */

/** Flags for Enabling/Disabling Selective Security Mode Configuration 
  */
#define NH_SEL_SEC_MOD_CFG_DISABLE   0x00 /* Security Mode Config Disable */
#define NH_SEL_SEC_MOD_CFG_ENABLE   0x01  /* Security Mode config Enable */

#define NH_RB_TO_ADD         0 /* Radio Bearer to Add */
#define NH_RB_TO_MOD         1 /* Radio Bearer to Modify */
#define NH_RB_TO_RLS         2 /* Radio Bearer to Release */
#define NH_RB_TO_REST        3 /* Radio Bearer to Re-Establish */
#define NH_MAX_RB_ACTION     4 /* Radio Bearer Max Actions */

/* Size of TB size table of BCCH and PCCH */
#define NH_BCCH_PCCH_TBSZTBL_SIZE   44

/**  
 * @details Macro for setting memory to zero
 * using cmMemset.
 */
#define NH_SET_ZERO(_memPtr, _size)                   \
do                                                    \
{                                                     \
   /*20568: removed NULL check to remove warnings*/                                                  \
      (Void)cmMemset((U8*)(_memPtr), 0, (Size)(_size)); \
}                                                     \
while(0)
      

/**  
 * This section defines the macros used for
 * Memory allocation in RRC.  
 */

/**  
 * @details Macro for SGetSBuf.
 */
#define NH_ALLOC(_region, _pool, _buf, _size)         \
do                                                    \
{                                                     \
   if (SGetSBuf((_region), (_pool), (Data **)(_buf),  \
                (_size)) == ROK)                      \
   {                                                  \
      NH_SET_ZERO((*(_buf)), (_size));                \
   }                                                  \
   else                                               \
   {                                                  \
      (*(_buf)) = NULLP;                              \
   }                                                  \
}                                                     \
while(0)

#ifdef SS_LOCKLESS_MEMORY
#define NH_ALLOC_SHARABLE_BUF(_region, _pool, _buf, _size)    \
{  \
 if (SGetStaticBuffer(_region, _pool, (Data **)_buf,      \
                (Size) _size, 0) == ROK)                                \
   {                                                                 \
      cmMemset((U8 *)(*_buf), 0, _size);                              \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (*_buf) = NULLP;                                                \
   }                                                                 \
}
#else
#define NH_ALLOC_SHARABLE_BUF(_region, _pool, _buf, _size)    \
do                                                    \
{                                                     \
   if (SGetSBuf((_region), (_pool), (Data **)(_buf),  \
                (_size)) == ROK)                      \
   {                                                  \
      NH_SET_ZERO((*(_buf)), (_size));                \
   }                                                  \
   else                                               \
   {                                                  \
      (*(_buf)) = NULLP;                              \
   }                                                  \
}                                                     \
while(0)
#endif

/**  
 * @details Macro for SPutSBuf.
 */
#define NH_FREE(_region, _pool, _buf, _size)          \
{                                                     \
   if (NH_CHK_INPUT_PARAM(_buf))                    \
   {                                                  \
       NHLOGERROR(ERRCLS_INT_PAR, 0,(ErrVal) 0, \
       "Can't free the memory: GOT NULL PTR\n");  \
   }                                                  \
   else                                               \
   {                                                  \
      (Void) SPutSBuf((_region), (_pool), (Data *)(_buf),\
      (Size)(_size));                                     \
      (_buf) = NULLP;                                    \
   }                                                   \
}

/**  
 * @details Macro for SGetMsg.
 */
#define NH_ALLOCMBUF(_region, _pool, _buf)            \
do                                                    \
{                                                     \
   if (SGetMsg((_region), (_pool),                    \
                        (Buffer **)(_buf)) != ROK)    \
   {                                                  \
      (*(_buf)) = NULLP;                              \
   }                                                  \
}                                                     \
while(0)

/**  
 * @details Macro for SPutMsg.
 */
#define NH_FREEMBUF(_mBuf)                               \
do{                                                      \
   if (NH_CHK_INPUT_PARAM(_mBuf))                        \
   {                                                     \
       NHLOGERROR(ERRCLS_INT_PAR, 0,(ErrVal) 0,          \
       "Can't free the mBuf: GOT NULL PTR\n");           \
   }                                                     \
   else                                                  \
   {                                                     \
      (Void)SPutMsg((_mBuf));                            \
      (_mBuf) = NULLP;                                   \
   }                                                  \
}while(0)

/**  
 * @details Macro for allocating an Event structure
 * using cmAllocEvnt.
 */
#define NH_ALLOCEVNT(_evntPtr, _size, _mem)           \
do                                                    \
{                                                     \
   (*(_evntPtr)) = NULLP;                             \
   (Void)cmAllocEvnt((Size)(_size), NH_MEM_PDU_SIZE,  \
                     (_mem), (Ptr *)(_evntPtr));      \
}                                                     \
while(0)

/**  
 * @details Macro for allocating in an event structure
 * using cmGetMem.
 */
#define NH_GET_MEM(_memPtr, _size, _allocPtr)               \
do                                                          \
{                                                           \
   (*(_allocPtr)) = NULLP;                                  \
   (Void)cmGetMem((_memPtr), (_size), (Ptr *)(_allocPtr));  \
}                                                           \
while(0)

/**  
 * @details Macro for freeing an event structure
 * using cmFreeMem.
 */
#define NH_FREEEVNT(_evnt)                            \
do                                                    \
{                                                     \
   cmFreeMem((_evnt));                                \
   (_evnt) = NULLP;                                   \
}                                                     \
while(0)

/**  
 * @details Macro for copying memory from source to target
 * using cmMemcpy.
 */
#define NH_MEM_COPY(_tgtPtr,_srcPtr, _size)                              \
do                                                                       \
{                                                                        \
   (Void)cmMemcpy((U8*)(_tgtPtr),(CONSTANT U8*)(_srcPtr), (Size)(_size));\
}                                                                        \
while(0)

/**  
 * @details Macro for allocating Transaction ID.
 */
#define NH_ALLOC_TRANS_ID(_transCb, _pTransId) \
{ \
   if((_transCb)->transId >= nhCb.genCfg.minTransLstLen) \
   { \
      SGetSysTime((_pTransId)); \
   } \
}


/**  
 * @details Macros in LTE RRC for debug logging.
 */
#define NH_DBG_INFO(_args)
#define NH_DBG_DEBUG(_args)
#define NH_DBG_PARAM(_args)
#define NH_DBG_ERROR(_args)
#define NH_DBG_FATAL(_args)


/**  
 * @details NULL CHECK in LTE-RRC for debug
 * NULL CHECKs are available by enabling ERRCLASS
 * & ERRCLS_DEBUG and DEBUGP (for log).
 */
#ifdef ERRCLASS
#define NHLOGERROR(errCls, errCode, errVal, errDesc) \
            SLogError(nhCb.init.ent, nhCb.init.inst, nhCb.init.procId, \
            __FILE__, __LINE__, errCls, errCode, errVal, errDesc)
#else  /* ERRCLASS */
#define NHLOGERROR(errCls, errCode, errVal, errDesc)
#endif /* ERRCLASS */

/* Checks for null pointer input parameter for a function */
#define NH_CHK_INPUT_PARAM(_ptr) (_ptr) == NULLP


/** 
 Macro to reset the Statistics counter. 
*/
#define NH_VALIDATE_CNTR(_cntr)  \
{                                \
   if (*(_cntr) == 0xFFFFFFFF)   \
      *(_cntr) = 0;              \
}

/** @def NH_FORM_NHU_HDR
 *  This macro forms the NHU header.
 */
#define NH_FORM_NHU_HDR(_hdr, _nhId) \
{ \
   (_hdr).cellId = (_nhId).cellId; \
   (_hdr).ueId = (_nhId).ueId; \
}


/** @def NH_FILL_MSG_PDU_AND_TYPE
 *  This macro fills NhMsgPduAndType.
 */
#define NH_FILL_MSG_PDU_AND_TYPE(_msgPduType, _pdu, _type) \
{ \
   (_msgPduType).pdu = (_pdu);  \
   (_msgPduType).type = (_type);  \
}

 
/** @def NH_FILL_NHU_ERROR
 *  This macro fills the NhuError.
 */
#define NH_FILL_NHU_ERROR(_err, _errType, _errCause) \
{ \
   (_err).errPres = TRUE;  \
   (_err).errType = (_errType);  \
   (_err).errCause = (_errCause);  \
}

/** @def NH_FORM_NH_ID
 *  This macro fills the NhId from data response.
 */
#define NH_FORM_NH_ID(_nhId, _datRspSdu, _crnti) \
{ \
   (_nhId).cellId = (_datRspSdu)->hdr.cellId; \
   (_nhId).ueId = (_crnti); \
   if((_datRspSdu)->sdu.msgCategory == NHU_MSG_DLDCCH) \
   { \
      (_nhId).isPdcpId = TRUE; \
      (_nhId).rbId = (_datRspSdu)->sdu.m.dlDcchMsg.pdcpId; \
   } \
   else \
   { \
      (_nhId).isPdcpId = FALSE; \
      (_nhId).rbId = (_datRspSdu)->sdu.m.dlCcchMsg.rbId; \
   } \
}

/** @def NH_FORM_NH_ID
 *  This macro fills the NhId from data response.
 */
#define NH_FORM_NH_ID_FRM_DATRSP(_nhId, _datRspSdu) \
{ \
   (_nhId).cellId = (_datRspSdu)->hdr.cellId; \
   (_nhId).ueId = 0; \
   (_nhId).isPdcpId = FALSE; \
   (_nhId).rbId = (_datRspSdu)->sdu.m.dlCcchMsg.rbId; \
}

/** @def NH_FORM_NH_ID_FRM_DCCH_DATRSP
 *  This macro fills the NhId from data response.
 */
#define NH_FORM_NH_ID_FRM_DCCH_DATRSP(_nhId, _datRspSdu) \
{ \
   (_nhId).cellId = (_datRspSdu)->hdr.cellId; \
   (_nhId).ueId = (_datRspSdu)->hdr.ueId; \
   (_nhId).isPdcpId = TRUE; \
   (_nhId).rbId = (_datRspSdu)->sdu.m.dlDcchMsg.pdcpId;  \
}

/** @def NH_FORM_NHID_FRM_DATREQ
 *  This macro fills the NhId from data request.
 */
#define NH_FORM_NHID_FRM_DATREQ(_nhId, _datReqSdu) \
{ \
   (_nhId).cellId = (_datReqSdu)->hdr.cellId; \
   (_nhId).ueId = (_datReqSdu)->hdr.ueId; \
   if ((_datReqSdu)->sdu.ueMsg.isMsgDed) \
   { \
      (_nhId).isPdcpId = TRUE; \
      (_nhId).rbId = (_datReqSdu)->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId; \
   } \
   else \
   { \
      (_nhId).isPdcpId = FALSE; \
      (_nhId).rbId = (_datReqSdu)->sdu.ueMsg.msg.cmnMsg.cmnMsgInfo.rbId; \
   } \
}


/** @def NH_GET_TRANSCB_KEY
 *  This macro forms the transaction control block key from
 *  message type and transaction ID.
 */
#define NH_GET_TRANSCB_KEY(_key, _msgType, _transId) \
                                (_key) = (((_msgType) << 8) | (_transId));

/** @def NH_ALLOC_MUI_ID
 *  It allocates the MUI ID based on the system time.
 */
#define NH_ALLOC_MUI_ID(_muiCb, _pMui) \
{ \
   if ((_muiCb)->mui >= nhCb.genCfg.minMuiLstLen) \
   { \
      SGetSysTime((_pMui)); \
   } \
}

/** @def NH_SEND_DAT_CFM
 *  It forms and sends data confirm to RRC user
 */
#define NH_SEND_DAT_CFM(_nhId, _nxTransId, _pdcpCause) \
{\
   NhuDatCfmSdus *(_datCfmSdus)=NULLP; \
   NH_ALLOC(NH_REG, NH_POOL, &(_datCfmSdus), sizeof(NhuDatCfmSdus)); \
   if(NULLP == (_datCfmSdus)) \
   { \
      NH_DBG_FATAL((NH_PRNT_BUF,"NhuDatCfmSdus memory allocation failed.\n")); \
   } \
   else \
   { \
      (_datCfmSdus)->hdr.ueId = (_nhId).ueId; \
      (_datCfmSdus)->hdr.cellId = (_nhId).cellId; \
      (_datCfmSdus)->hdr.transId = (_nxTransId); \
      (_datCfmSdus)->pdcpId = (_nhId).rbId; \
      (_datCfmSdus)->datCfmStatus = (_pdcpCause); \
      nhUIMSndDatCfm((_datCfmSdus)); \
   } \
}

/** @def NH_SEND_STATUS_IND
 *  It forms and sends status indication to RRC user
 */
#define NH_SEND_STATUS_IND(_nhId, _cause) \
{\
   NhuStaIndSdus *_staIndSdus=NULLP; \
   NH_ALLOC(NH_REG, NH_POOL, &_staIndSdus, sizeof(NhuStaIndSdus)); \
   if(NULLP == _staIndSdus) \
   { \
      NH_DBG_FATAL((NH_PRNT_BUF, \
         "NhuStaIndSdus memory allocation failed.\n")); \
   } \
   else \
   { \
      _staIndSdus->hdr.ueId = (_nhId).ueId; \
      _staIndSdus->hdr.cellId = (_nhId).cellId; \
      _staIndSdus->hdr.transId = NH_ZERO_VAL; \
      _staIndSdus->pdcpId = (_nhId).rbId; \
      _staIndSdus->cause = (_cause); \
      nhUIMSndStaInd(_staIndSdus); \
   } \
}

/** @def NH_MAP_PDCP_DAT_STA_IND_CAUSE
 *  It maps PDCP cause to NHU cause
 */
#define NH_MAP_PDCP_DAT_STA_IND_CAUSE(_pdcpCause, _nhuCause) \
{ \
   switch((_pdcpCause)) \
   { \
      case PJU_INTVER_FAILED: \
         (_nhuCause) = NHU_ERRCAUSE_INTEGRITY_FAILED; \
      break; \
      case PJU_DCIP_FAILED: \
         (_nhuCause) = NHU_ERRCAUSE_DECIPHERING_FAILED; \
      break; \
      case PJU_DCOMP_FAILED: \
         (_nhuCause) = NHU_ERRCAUSE_DECOMPRESSION_FAILED; \
      break; \
      case PJU_COMP_FAILED: \
         (_nhuCause) = NHU_ERRCAUSE_COMPRESSION_FAILED; \
      break; \
      case PJU_INTPROT_FAILED: \
         (_nhuCause) = NHU_ERRCAUSE_INTEG_PROT_FAILED; \
      break; \
      case PJU_CIPHER_FAILED: \
         (_nhuCause) = NHU_ERRCAUSE_CIPHERING_FAILED; \
      break; \
      default: \
         (_nhuCause) = NHU_ERRCAUSE_NONE; \
      break; \
   } \
}

/** @def NH_MAP_PDCP_DAT_CFM_CAUSE
 *  It maps PDCP cause to NHU cause
 */
#define NH_MAP_PDCP_DAT_CFM_CAUSE(_pdcpCause, _nhuCause) \
{ \
   if(PJU_RLC_TRANS_FAIL == (_pdcpCause)) \
      (_nhuCause) = NHU_ERRCAUSE_MAX_RLC_TRANS_REACH; \
   else if(PJU_DISCARD_TMR_EXP == (_pdcpCause)) \
      (_nhuCause) = NHU_ERRCAUSE_PDCP_SDU_DIS_TMR_EXP; \
   else if(PJU_DAT_CFM_SUCCESS == (_pdcpCause)) \
      (_nhuCause) = NHU_ERRCAUSE_NONE; \
}

/** @def NH_SEND_ERROR_IND
 *  It forms and sends the error indication to the RRC user.
 */
#define NH_SEND_ERROR_IND(_errSdu, _hdr) \
{ \
   NhuErrIndSdus *_errIndSdus=NULLP; \
   if ((_errSdu).sdu.errPres) \
   { \
      NH_ALLOC(NH_REG, NH_POOL, &_errIndSdus, sizeof(NhuErrIndSdus)); \
      if(NULLP == _errIndSdus) \
      { \
         NH_DBG_FATAL((NH_PRNT_BUF, \
                     "NhuErrIndSdus memory allocation failed.\n")); \
      } \
      else \
      { \
         NH_MEM_COPY(&(_errIndSdus->hdr), &(_hdr), sizeof(NhuHdr)); \
         NH_MEM_COPY(&(_errIndSdus->sdu), &((_errSdu).sdu), \
                  sizeof(NhuErrorInfo)); \
         nhUIMSndErrInd(_errIndSdus); \
      } \
   } \
}

/* Macro to check if General configuration and Protocol configuration is done */
#define NH_CHK_LYR_CFG(_fn, _ret) \
   {\
      (_ret) = ROK;\
      if (nhCb.init.cfgDone != TRUE) \
      { \
         NH_DBG_ERROR((nhCb.init.prntBuf,  \
                "_fn: General Configuration not done")); \
         (_ret) = LCM_REASON_GENCFG_NOT_DONE;\
      }\
      if(((_ret) == ROK) && (nhCb.eNodeBCb.cfgDone != TRUE))\
      {\
         NH_DBG_ERROR((nhCb.init.prntBuf, \
                "_fn: Protocol Configuration not done")); \
         (_ret) = LNH_REASON_PROTCFG_NOT_DONE;\
      }\
   }

/** @def NH_GET_MAC_CFG_REQ_TRANSID
 * This macro prepares Configuration Transaction ID for MAC Configuration Request.
 */
#define NH_GET_MAC_CFG_REQ_TRANSID(_cfgTransId, _cellId, _crnti, _transId,\
                                   _cfgElem, _action, _lcId, _dir) \
{ \
   U8 *_tmp = (_cfgTransId); \
   cmMemcpy((U8*)_tmp, (CONSTANT U8*)&(_cellId), sizeof(U16)); \
   _tmp = _tmp + sizeof(U16); \
   cmMemcpy((U8*)_tmp, (CONSTANT U8*)&(_crnti), sizeof(U16)); \
   _tmp = _tmp + sizeof(U16); \
   cmMemcpy((U8*)_tmp, (CONSTANT U8*)&(_transId), sizeof(U32)); \
   _tmp = _tmp + sizeof(U32); \
   *_tmp = (_cfgElem); \
   _tmp++; \
   *_tmp = (_action); \
   _tmp++; \
   *_tmp = (_lcId); \
   _tmp++; \
   *_tmp = (_dir); \
}

#ifdef RRC_PHY_CFG_SUPP
/* Check if this macro is correct */
/** @def NH_GET_PHY_CFG_REQ_TRANSID
 * This macro prepares Configuration Transaction ID for PHY Configuration Request.
 */
#define NH_GET_PHY_CFG_REQ_TRANSID(_cfgTransId, _cellId, _crnti, \
                                   _transId, _cfgElem) \
{ \
   U8 *_tmp = (_cfgTransId); \
   cmMemcpy((U8*)_tmp, (CONSTANT U8*)&(_cellId), sizeof(U16)); \
   _tmp = _tmp + sizeof(U16); \
   cmMemcpy((U8*)_tmp, (CONSTANT U8*)&(_crnti), sizeof(U16)); \
   _tmp = _tmp + sizeof(U16); \
   cmMemcpy((U8*)_tmp, (CONSTANT U8*)&(_transId), sizeof(U32)); \
   _tmp = _tmp + sizeof(U32); \
   *_tmp = (_cfgElem); \
}
#endif /* RRC_PHY_CFG_SUPP */

/** @def NH_GET_PJU_SAP
 * This macro reterives the PJU SAP. 
 */
#define NH_GET_PJU_SAP(_lSapCb,_spId) \
{ \
   (_lSapCb) = *(nhCb.pjuSapCbLst + (_spId)); \
  \
}

/** @def NH_GET_KWU_SAP
 * This macro reterives the KWU SAP. 
 */
#define NH_GET_KWU_SAP(_lSapCb,_spId) \
{ \
   (_lSapCb) = nhCb.kwuSapCbLst[(_spId)]; \
  \
}
/** @def NH_GET_CPJ_SAP
 * This macro reterives the CPJ SAP. 
 */
#define NH_GET_CPJ_SAP(_lSapCb,_spId) \
{ \
   (_lSapCb) = nhCb.cpjSapCbLst[(_spId)]; \
  \
}
/** @def NH_GET_CKW_SAP
 * This macro reterives the CKW SAP. 
 */
#define NH_GET_CKW_SAP(_lSapCb,_spId) \
{ \
   (_lSapCb) = nhCb.ckwSapCbLst[(_spId)]; \
  \
}
/** @def NH_GET_CRG_SAP
 * This macro reterives the CRG SAP. 
 */
#define NH_GET_CRG_SAP(_lSapCb,_spId) \
{ \
   (_lSapCb) = nhCb.crgSapCbLst[(_spId)]; \
  \
}

#ifdef RRC_PHY_CFG_SUPP
/* Macro for updating the cfgCfm structure */
#define NH_FORM_CFG_CFM_RESULT(_cfgMsk, _cfgCfm, _cfmStatus, _errCause) \
{ \
   if ((_cfgMsk) & NH_PHY_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_PHY_CONFIGURED )) \
      { \
         (_cfgCfm).sdu.cfgError.errPres = TRUE; \
         (_cfgCfm).sdu.cfgError.errType |= NHU_CFGERRTYPE_PHY; \
      } \
   } \
   if ((_cfgMsk) & NH_RLC_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_RLC_CONFIGURED )) \
      { \
         (_cfgCfm).sdu.cfgError.errPres = TRUE; \
         (_cfgCfm).sdu.cfgError.errType |= NHU_CFGERRTYPE_RLC; \
      } \
   } \
   if ((_cfgMsk) & NH_MAC_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_MAC_CONFIGURED )) \
      { \
         (_cfgCfm).sdu.cfgError.errPres = TRUE; \
         (_cfgCfm).sdu.cfgError.errType |= NHU_CFGERRTYPE_MAC; \
      } \
   } \
   if ((_cfgMsk) & NH_PDCP_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_PDCP_CONFIGURED )) \
      { \
         (_cfgCfm).sdu.cfgError.errPres = TRUE; \
         (_cfgCfm).sdu.cfgError.errType |= NHU_CFGERRTYPE_PDCP; \
      } \
   } \
   if ((_cfgCfm).sdu.cfgError.errPres) \
   { \
      (_cfgCfm).sdu.isCfgSuccess = FALSE; \
      (_cfgCfm).sdu.cfgError.errCause = _errCause; \
   } \
   else \
   { \
      (_cfgCfm).sdu.isCfgSuccess = TRUE; \
   } \
}
#else /* RRC_PHY_CFG_SUPP */
/* Macro for updating the cfgCfm structure */
#define NH_FORM_CFG_CFM_RESULT(_cfgMsk, _cfgCfm, _cfmStatus, _errCause) \
{ \
   if ((_cfgMsk) & NH_RLC_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_RLC_CONFIGURED )) \
      { \
         (_cfgCfm).sdu.cfgError.errPres = TRUE; \
         (_cfgCfm).sdu.cfgError.errType |= NHU_CFGERRTYPE_RLC; \
      } \
   } \
   if ((_cfgMsk) & NH_MAC_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_MAC_CONFIGURED )) \
      { \
         (_cfgCfm).sdu.cfgError.errPres = TRUE; \
         (_cfgCfm).sdu.cfgError.errType |= NHU_CFGERRTYPE_MAC; \
      } \
   } \
   if ((_cfgMsk) & NH_PDCP_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_PDCP_CONFIGURED )) \
      { \
         (_cfgCfm).sdu.cfgError.errPres = TRUE; \
         (_cfgCfm).sdu.cfgError.errType |= NHU_CFGERRTYPE_PDCP; \
      } \
   } \
   if ((_cfgCfm).sdu.cfgError.errPres) \
   { \
      (_cfgCfm).sdu.isCfgSuccess = FALSE; \
      (_cfgCfm).sdu.cfgError.errCause = _errCause; \
   } \
   else \
   { \
      (_cfgCfm).sdu.isCfgSuccess = TRUE; \
   } \
}
#endif /* RRC_PHY_CFG_SUPP */

#ifdef RRC_PHY_CFG_SUPP
/* Macro for updating the cfgCfm structure */
#define NH_FORM_PDCP_STA_CFM_RESULT(_cfgMsk, _status, _cfmStatus) \
{ \
   if ((_cfgMsk) & NH_PHY_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_PHY_CONFIGURED )) \
      { \
         _status |= NHU_CFGERRTYPE_PHY; \
      } \
   } \
   if ((_cfgMsk) & NH_RLC_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_RLC_CONFIGURED )) \
      { \
         _status |= NHU_CFGERRTYPE_RLC; \
      } \
   } \
   if ((_cfgMsk) & NH_MAC_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_MAC_CONFIGURED )) \
      { \
         _status |= NHU_CFGERRTYPE_MAC; \
      } \
   } \
   if ((_cfgMsk) & NH_PDCP_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_PDCP_CONFIGURED )) \
      { \
         _status |= NHU_CFGERRTYPE_PDCP; \
      } \
   } \
}
#else  /* RRC_PHY_CFG_SUPP */
/* Macro for updating the cfgCfm structure */
#define NH_FORM_PDCP_STA_CFM_RESULT(_cfgMsk, _status, _cfmStatus) \
{ \
   if ((_cfgMsk) & NH_RLC_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_RLC_CONFIGURED )) \
      { \
         _status |= NHU_CFGERRTYPE_RLC; \
      } \
   } \
   if ((_cfgMsk) & NH_MAC_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_MAC_CONFIGURED )) \
      { \
         _status |= NHU_CFGERRTYPE_MAC; \
      } \
   } \
   if ((_cfgMsk) & NH_PDCP_CONFIGURED) \
   { \
      if (!((_cfmStatus) & NH_PDCP_CONFIGURED )) \
      { \
         _status |= NHU_CFGERRTYPE_PDCP; \
      } \
   } \
}
#endif /* RRC_PHY_CFG_SUPP */

/* Macros to the init structure */
#define NH_PRNT_BUF   nhCb.init.prntBuf

#define NH_REG       nhCb.init.region
#define NH_POOL      nhCb.init.pool

/* This parameter must be decided by the RRC User */
/* The maximum size of the RRC PDU */
#define NH_MEM_PDU_SIZE   3584 

/* This macro resets bit of layer from which cfm is received */
#define NH_RESET_TRANS_CFM_MASK(_cfmMask, _layerId) \
{ \
   (_cfmMask) &= ~(_layerId); \
} 

/* This macro sets/resets bit for the layer depending on cfm results */
#define NH_SET_CFG_CFM_STATUS(_cfmStatus, _status, _layerId) \
{ \
  if ((_status) == NH_CFG_SUCCESS) /* cfg cfm is successful */ \
  { \
    (_cfmStatus) |= (_layerId); \
  } \
  else if ((_status) == NH_CFG_FAILED)/* Cfg Cfm is failed */ \
  { \
     (_cfmStatus) &= ~(_layerId); \
  } \
}

/* This macro sets the cfg type for MAC */
#define NH_SET_MAC_CFG_TYPE(_elmId, _cfgType) \
{ \
   (_cfgType) |= (_elmId); \
}

#define NH_IS_LOWER_LYR_CFG_SUCCESS(_transCb, _status) \
{ \
      if ((_transCb)->cfmStatus == (_transCb)->cfgMask) \
      { \
         (_status) = 1; \
      } \
      else \
      { \
         (_status) = 0; \
      } \
} 

/* Macro to generate the NhId based on RlcId */
#define NH_MAP_RLCID_TO_NHID(_nhId, _rlcId) \
{ \
   (_nhId).cellId = (_rlcId).cellId; \
   (_nhId).ueId = (_rlcId).ueId; \
   (_nhId).isPdcpId = FALSE; \
   (_nhId).rbId = (_rlcId).rbId; \
}

/* Macro to generate the NhId based on PdcpId */
#define NH_MAP_PDCPID_TO_NHID(_nhId, _pdcpId) \
{ \
   (_nhId).cellId = (_pdcpId)->cellId; \
   (_nhId).ueId = (_pdcpId)->ueId; \
   (_nhId).isPdcpId = TRUE; \
   (_nhId).rbId = (_pdcpId)->rbId; \
}
/* Fix for ccpu00124582 */
#define NH_VALIDATE_DRBID_TO_LCID_MAP(_rbCfg, _drbMsk, _dlcMsk, _ddirMsk, _ret) \
{                                    \
   U32  _tmpDrb = 0, _tmpDlc = 0;       \
   U8   _drbId, _dlcId;                  \
   _drbId = _rbCfg->rbId.rbId;              \
   _tmpDrb = (1 << (_drbId - 1));       \
   _dlcId = _rbCfg->lcId.lcId;         \
   _tmpDlc = (1 << (_dlcId - 1));       \
   if((_drbMsk) & _tmpDrb)                \
      (_ret) = RFAILED;                 \
   else                               \
      (_drbMsk) |= _tmpDrb;               \
   if((_dlcMsk) & _tmpDlc)                \
   {                                  \
      if(((_rbCfg)->rbCfgBitMsk & NHU_RB_CFG_DL) && (_ddirMsk[0] & _tmpDlc)) \
         (_ret) = RFAILED;              \
      else                            \
         _ddirMsk[0] |= _tmpDlc;        \
      if(((_rbCfg)->rbCfgBitMsk & NHU_RB_CFG_UL) && (_ddirMsk[1] & _tmpDlc)) \
         (_ret) = RFAILED;              \
      else                            \
         _ddirMsk[1] |= _tmpDlc;        \
   }                                  \
   else                               \
   {                                  \
      (_dlcMsk) |= _tmpDlc;               \
      if((_rbCfg)->rbCfgBitMsk & NHU_RB_CFG_DL)  \
          _ddirMsk[0] |= _tmpDlc;       \
      if((_rbCfg)->rbCfgBitMsk & NHU_RB_CFG_UL)  \
          _ddirMsk[1] |= _tmpDlc;                \
   }                                           \
}
/* Fix for ccpu00124582 */
#define NH_VALIDATE_SRBID_TO_LCID_MAP(_rbCfg, _srbMsk, _slcMsk, _sdirMsk, _ret) \
{                                    \
   U32  _tmpSrb = 0, _tmpSlc = 0;       \
   U8   _srbId, _slcId;                  \
   _srbId = _rbCfg->rbId.rbId;              \
   _tmpSrb = (1 << (_srbId - 1));       \
   _slcId = _rbCfg->lcId.lcId;         \
   _tmpSlc = (1 << (_slcId - 1));       \
   if((_srbMsk) & _tmpSrb)                \
      (_ret) = RFAILED;                 \
   else                               \
      (_srbMsk) |= _tmpSrb;               \
   if((_slcMsk) & _tmpSlc)                \
   {                                  \
      if(((_rbCfg)->rbCfgBitMsk & NHU_RB_CFG_DL) && (_sdirMsk[0] & _tmpSlc)) \
         (_ret) = RFAILED;              \
      else                            \
         _sdirMsk[0] |= _tmpSlc;        \
      if(((_rbCfg)->rbCfgBitMsk & NHU_RB_CFG_UL) && (_sdirMsk[1] & _tmpSlc)) \
         (_ret) = RFAILED;              \
      else                            \
         _sdirMsk[1] |= _tmpSlc;        \
   }                                  \
   else                               \
   {                                  \
      (_slcMsk) |= _tmpSlc;               \
      if((_rbCfg)->rbCfgBitMsk & NHU_RB_CFG_DL)  \
          _sdirMsk[0] |= _tmpSlc;       \
      if((_rbCfg)->rbCfgBitMsk & NHU_RB_CFG_UL)  \
          _sdirMsk[1] |= _tmpSlc;                \
   }                                           \
}

#define NH_VALIDATE_RB_ID(_rbId, _rbType, _ret) \
{ \
   if (_rbType == CM_LTE_SRB) \
   { \
      if ((_rbId == NH_RB_ID_SRB1) || (_rbId == NH_RB_ID_SRB2)) \
         _ret = ROK; \
      else \
         _ret = RFAILED; \
   } \
}

#define NH_CHECK_NULLP_CKWCFG(_ckwCfgParam) \
{                                           \
   if ((CkwCfgInfo *)NULLP == (_ckwCfgParam)) \
   {                                        \
       NH_DBG_FATAL((NH_PRNT_BUF,            \
                      "Could not allocate mem for ckwCfgParam")); \
       RETVALUE(RFAILED);                   \
   }                                        \
}

#define NH_CHECK_NULLP_CRGCFG(_crgCfgParam) \
{                                    \
   if ((CrgCfgReqInfo *)NULLP == (_crgCfgParam)) \
   {                                        \
       NH_DBG_FATAL((NH_PRNT_BUF,            \
                      "Could not allocate mem for crgCfgParam")); \
       RETVALUE(RFAILED);                   \
   }                                        \
}

#ifdef RRC_PHY_CFG_SUPP
#define NH_CHECK_NULLP_CTFCFG(_ctfCfgParam) \
{                                    \
   if ((CtfCfgReqInfo *)NULLP == (_ctfCfgParam)) \
   {                                        \
       NH_DBG_FATAL((NH_PRNT_BUF,            \
                      "Could not allocate mem for ctfCfgParam")); \
       RETVALUE(RFAILED);                   \
   }                                        \
}
#endif /* RRC_PHY_CFG_SUPP */

#define NH_CHECK_NULLP_CPJCFG(_cpjCfgReq) \
{                                         \
   if (NULLP == (_cpjCfgReq))             \
   {                                      \
       NH_DBG_FATAL((NH_PRNT_BUF,          \
                      "Could not allocate mem for cpjCfgReq")); \
       RETVALUE(RFAILED);                  \
   }                                       \
}

#define NH_CHECK_NULLP_CPJSECCFG(_cpjSecCfgReq) \
{                                    \
   if ((CpjSecCfgReqInfo *)NULLP == (_cpjSecCfgReq)) \
   {                                        \
       NH_DBG_FATAL((NH_PRNT_BUF,            \
                      "Could not allocate mem for CpjSecCfgReqInfo")); \
       RETVALUE(RFAILED);                   \
   }                                        \
}
/* trace macro */
#define NH_GEN_TRC(_suId, _sapType, _trcEvnt, _mBuf) \
{\
   switch(_sapType)\
   {\
      case STNHKWUSAP:\
        if(nhCb.kwuSapCbLst[(_suId)]->trc != FALSE)\
           nhUTLGenTrc(_suId, _sapType, _trcEvnt, _mBuf);\
        break;\
      case STNHPJUSAP:\
        if(nhCb.pjuSapCbLst[(_suId)]->trc != FALSE)\
           nhUTLGenTrc(_suId, _sapType, _trcEvnt, _mBuf);\
        break;\
      default :\
         NH_DBG_ERROR((NH_PRNT_BUF, "Invalid SAP Type received"));\
        break;\
   }\
}

#ifdef NHU_MULT_SAP_SUPPRT
#ifdef RRC_PHY_CFG_SUPP
#define NH_GET_SUID_FOR_MULT_SAP_SUPPRT(_suId, _cellCb, _sapType)\
{\
   switch(_sapType)\
   {\
      case STNHKWUSAP:\
         _suId = (_cellCb)->llyrSapIdInfo.kwuSapId;\
         break;\
      case STNHCKWSAP:\
         _suId = (_cellCb)->llyrSapIdInfo.ckwSapId;\
         break;\
      case STNHCRGSAP:\
         _suId = (_cellCb)->llyrSapIdInfo.crgSapId;\
         break;\
      case STNHCPJSAP:\
         _suId = (_cellCb)->llyrSapIdInfo.cpjSapId;\
         break;\
      case STNHPJUSAP:\
         _suId = (_cellCb)->llyrSapIdInfo.pjuSapId;\
         break;\
      case STNHCTFSAP:\
         _suId = (_cellCb)->llyrSapIdInfo.ctfSapId;\
         break;\
      default :\
         NH_DBG_ERROR((NH_PRNT_BUF, "Invalid SAP Type received"));\
        break;\
   }\
}
#else /* RRC_PHY_CFG_SUPP */
#define NH_GET_SUID_FOR_MULT_SAP_SUPPRT(_suId, _cellCb, _sapType)\
{\
   switch(_sapType)\
   {\
      case STNHKWUSAP:\
         _suId = (_cellCb)->llyrSapIdInfo.kwuSapId;\
         break;\
      case STNHCKWSAP:\
         _suId = (_cellCb)->llyrSapIdInfo.ckwSapId;\
         break;\
      case STNHCRGSAP:\
         _suId = (_cellCb)->llyrSapIdInfo.crgSapId;\
         break;\
      case STNHCPJSAP:\
         _suId = (_cellCb)->llyrSapIdInfo.cpjSapId;\
         break;\
      case STNHPJUSAP:\
         _suId = (_cellCb)->llyrSapIdInfo.pjuSapId;\
         break;\
      default :\
         NH_DBG_ERROR((NH_PRNT_BUF, "Invalid SAP Type received"));\
        break;\
   }\
}
#endif /* RRC_PHY_CFG_SUPP */
#endif /* NHU_MULT_SAP_SUPPRT */

/** @def Conidtional IE Validation */

#define NH_IS_TKN_PRSNT(_elem)      NH_IS_PRSNT((_elem).pres)
#define NH_IS_TKN_NOTPRSNT(_elem)   NH_IS_NOTPRSNT((_elem).pres)

#define NH_IS_IE_PRSNT(_elem)       NH_IS_PRSNT((_elem).pres.val)
#define NH_IS_IE_NOTPRSNT(_elem)    NH_IS_NOTPRSNT((_elem).pres.val)

#define NH_IS_PRSNT(_elem) ((_elem) != NOTPRSNT)
#define NH_IS_NOTPRSNT(_elem) ((_elem) == NOTPRSNT)

/** @def NH_CONDIES_GET_MSG_TYPE */
#define NH_CONDIES_GET_SDU_TYPE(_msgSdu, _evtType, _reCfgIEs) \
{\
   switch((_evtType))\
   {\
      default:\
           (_reCfgIEs) = &(((NhuDatReqSdus *)(_msgSdu))->sdu.ueMsg.msg.dedMsg.\
                           dlDcchMsg.m.dcchMsg.message.val.c1.val.rrcConRecfgn.\
                           criticalExtns.val.c1.val.rrcConRecfgn_r8);\
            break;\
      case EVTNHUENCODEREQ:\
           (_reCfgIEs) = &(((NhuEncReqSdus *)(_msgSdu))->sdu.u.dcchMsg.message.\
                         val.c1.val.rrcConRecfgn.criticalExtns.val.c1.val.\
                         rrcConRecfgn_r8);\
            break;\
   }\
}


/** @def NH_GET_UL_BITMAP_SIZE
 *  This macro calculates the number of octets required for ulBitMap using numBits.
 */
#define NH_GET_UL_BITMAP_SIZE(_numBits, _size) \
{ \
   _size = _numBits / NH_OCTET_VAL; \
   if(_numBits % NH_OCTET_VAL) \
   { \
      _size++; \
   } \
}

/** @def NH_CHK_RB_ACTN_CFG_CFM
   *  This macro checks whether Cfg Cfm was expected fro specific RB
   *  and layer.
   */
#define NH_CHK_RB_ACTN_CFG_CFM(_tbl, _rbBitMsk, _found, _idx) \
   {\
      for (_idx = 0; _idx < NH_MAX_RB_ACTION; _idx++)\
      {\
         if(_tbl[_idx] & _rbBitMsk)\
         {\
            _found = TRUE;\
            break;\
         }\
      }\
   }

/** @def NH_SET_RCVD_CFG_CFM_MSK
  * This macro sets received RB Cfg Cfm
  * Note: Same bit will be set even if it is duplicate - no harm
  * in doing that but it will reduce "else" statement /
*/

#define NH_SET_RCVD_CFG_CFM_MSK(_rcvdRbIds, _rbBitMsk) \
   {\
      if(*_rcvdRbIds & _rbBitMsk)\
      {\
         NH_DBG_ERROR((NH_PRNT_BUF, "Duplicate Cfg Cfm RCVD\n"));\
      }\
      else\
      {\
         *_rcvdRbIds |= _rbBitMsk;\
      }\
   }
/** @def NH_SET_RCVD_STA_CFM_MSK
  * This macro is used to set Cfm Status of each RBs 
  */
#define NH_SET_RCVD_STA_CFM_MSK(_tbl, _status, _okVal, _rbBitMsk, _idx)\
   {\
     if ((_status == _okVal) && (_idx < NH_MAX_RB_ACTION))\
         _tbl[_idx] &= ~_rbBitMsk;\
   }
/** @def NH_COPY_TMG_INFO
 *  This macro assigns timing info from source to destination.
 */
#define NH_COPY_TMG_INFO(_dstTmgInfo, _srcTmgInfo) \
{ \
   _dstTmgInfo.sfn = _srcTmgInfo.sfn; \
   _dstTmgInfo.subframe = _srcTmgInfo.subframe; \
}

/** @def NH_SET_CELLCFG_RBMAP_ELMNT
 *  This macro sets the array entry for each common RB in cell configuration.
 */
#define NH_SET_CELLCFG_RBMAP_ELMNT(_chkForRb, _idx, _ret) \
{ \
   if (TRUE == _chkForRb[(_idx)]) \
   { \
      _ret = RFAILED; \
   } \
   else \
   { \
      _chkForRb[(_idx)] = TRUE; \
      _ret = ROK; \
   } \
}

/** @def NH_SET_CELLCFG_RBMAP_ELMNT
 *  This macro sets the array entry for BCCH-DL_SCH common RB during
 *  cell configuration. This macro shall be used with _idx=4 only.
 */
#define NH_SET_CELLCFG_BCCH_DLSCH_RBMAP(_chkForRb, _idx, _bcchLcId, \
                                        _lcId, _ret) \
{ \
   if (TRUE == _chkForRb[(_idx)]) \
   { \
      if ((TRUE == _chkForRb[(_idx)+1]) || \
          ((0xFF != _bcchLcId) && (_lcId == _bcchLcId))) \
      { \
         _ret = RFAILED; \
      } \
      else \
      { \
         _chkForRb[(_idx)+1] = TRUE; \
         _ret = ROK; \
      } \
   } \
   else \
   { \
      _chkForRb[(_idx)] = TRUE; \
      _bcchLcId = _lcId; \
      _ret = ROK; \
   } \
}

/** @def NH_GET_SYS_BW
 *  This macro maps the System bandwidth enum value to CTF bandwidth
 *   for cell configuration.
 */
#define NH_GET_SYS_BW(_cfg)         \
        ((_cfg == NHU_SYS_BW_N6) ?  CTF_BW_RB_6 :    \
          (_cfg == NHU_SYS_BW_N15) ? CTF_BW_RB_15 :   \
          (_cfg == NHU_SYS_BW_N25) ? CTF_BW_RB_25 :   \
          (_cfg == NHU_SYS_BW_N50) ? CTF_BW_RB_50 :   \
          (_cfg == NHU_SYS_BW_N75) ? CTF_BW_RB_75 : CTF_BW_RB_100 )


/* nh002.301:118366,120285: Added Macros for mapping RRC enum
 *                  to PHY enum.
 */
/** @def NH_GET_DELTA_SHIFT
 *  This macro maps the RRC Delta Shift enum value to CTF Delta
 *  Shift for cell configuration.
 */
#define NH_GET_DELTA_SHIFT(_cfg)         \
	((_cfg == NHU_PUCCH_DS1) ? CTF_PUCCH_DELTASHIFT1 :    \
	  (_cfg == NHU_PUCCH_DS2) ? CTF_PUCCH_DELTASHIFT2 :   \
                                    CTF_PUCCH_DELTASHIFT3)


/** @def NH_GET_ANT_PORT_CNT
 *  This macro maps the System bandwidth enum value to CTF bandwidth
 *   for cell configuration.
 */
#define NH_GET_ANT_PORT_CNT(_cfg)         \
        ((_cfg == 1) ?  CTF_AP_CNT_1 :    \
          (_cfg == 2) ? CTF_AP_CNT_2 : CTF_AP_CNT_4 )


/** @def NH_GET_BCCH_PCCH_TBSZ
 *  This macro finds the appropriate TB size for PCCH and BCCH messages.
 *  It applies binary seach on sorted TB size list to find the TB size.
 */
#define NH_GET_BCCH_PCCH_TBSZ(_msgOctets, _tbSz) \
do \
{ \
   S32 _minIdx, _midIdx, _maxIdx; \
   for (_minIdx = 0, _maxIdx = NH_BCCH_PCCH_TBSZTBL_SIZE - 1; _minIdx <= _maxIdx;) \
   { \
      _midIdx = ((_minIdx + _maxIdx) / 2); \
      if (NhBcchPcchTbSzTbl[_midIdx] < _msgOctets) \
      { \
         _minIdx = _midIdx + 1; \
      } \
      else if (NhBcchPcchTbSzTbl[_midIdx] > _msgOctets) \
      { \
         _maxIdx = _midIdx - 1; \
      } \
      else \
      { \
         _tbSz = NhBcchPcchTbSzTbl[_midIdx]; \
         break; \
      } \
   } \
   if (_minIdx > _maxIdx) \
      _tbSz = NhBcchPcchTbSzTbl[_minIdx]; \
} \
while(0)
/* Note: Any changes to these enums should reflect to */
/** @details Enums for special argument
 *
*/
typedef enum
{
   NH_DIAG_SPID,
   NH_DIAG_SUID,
   NH_DIAG_UEID,
   NH_DIAG_CRNTI,
   NH_DIAG_NA
} NhDiagSplArg;

/* nh002.301:120713: Added Macros for mapping RRC enum
 *                  to PHY enum.
 */
/** @def NH_GET_SC_SPACING
 *  This macro maps the RRC Sc Spacing value to CTF Sc Spacing
 *  for cell configuration.
 */
#define NH_GET_SC_SPACING(_cfg)         \
   ((_cfg == NHU_TX_SCHM_SC_SPACING_15KHZ) ? CTF_SC_SPACING_15KHZ :\
    CTF_SC_SPACING_7DOT5KHZ)


#define NH_FREE_SHRABL_BUF(_cb,_buf, _size)                          \
{                                                         \
   if (_buf != NULLP)                                     \
   {                                                      \
      (Void) SPutStaticBuffer(_cb->init.region, _cb->init.pool,   \
            (Data *) _buf, (Size) _size, 0);                 \
      _buf = NULLP;                                       \
   }                                                      \
}

#define NH_ALLOC_SHRABL_BUF(_cb,_buf, _size)                                    \
{                                                                    \
 if (SGetStaticBuffer(_cb->init.region, _cb->init.pool, (Data **)&_buf,      \
                (Size) _size, 0) == ROK)                                \
   {                                                                 \
      cmMemset((U8 *)(_buf), 0, _size);                              \
   }                                                                 \
   else                                                              \
   {                                                                 \
      (_buf) = NULLP;                                                \
   }                                                                 \
}

#define NH_ALLOC_SHRABL_BUF_WM(_cb,_buf, _size)                                    \
{                                                                    \
 SGetStaticBuffer(_cb->init.region, _cb->init.pool, (Data **)&_buf,      \
                (Size) _size, 0);                                \
}

#ifdef SS_LOCKLESS_MEMORY
#define NH_FREE_SHRABL_BUF_PST(_region, _pool, _buf, _size)                          \
{                                                         \
   if (_buf != NULLP)                                     \
   {                                                      \
      (Void) SPutStaticBuffer(_region, _pool,   \
            (Data *) _buf, (Size) _size, 0);                 \
      _buf = NULLP;                                       \
   }                                                      \
}
#else
#define NH_FREE_SHRABL_BUF_PST(_region, _pool, _buf, _size)                          \
{                                                     \
   if (NH_CHK_INPUT_PARAM(_buf))                    \
   {                                                  \
       NHLOGERROR(ERRCLS_INT_PAR, 0,(ErrVal) 0, \
       "Can't free the memory: GOT NULL PTR\n");  \
   }                                                  \
   else                                               \
   {                                                  \
      (Void) SPutSBuf((_region), (_pool), (Data *)(_buf),\
      (Size)(_size));                                     \
      (_buf) = NULLP;                                    \
   }                                                   \
}
#endif

#define NH_ALLOC_SHRABL_BUF_PST(_region, _pool, _buf, _size)                                    \
{                                                                    \
 SGetStaticBuffer(_region, _pool, (Data **)&_buf,      \
                (Size) _size, 0);                                \
}

#endif /* __NHH__ */


/**********************************************************************
  
         End of file:     nh.h@@/main/3 - Fri Jul  1 01:13:11 2011
   
**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      rer              1. LTE-RRC Initial Release.
/main/2      ---      chebli           1. RRC Release 2.1.
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
            nh002.301 mpatel           1. Macro NH_GET_DELTA_SHIFT is added
                                       2. Macro NH_GET_SC_SPACING is added
*********************************************************************91*/
