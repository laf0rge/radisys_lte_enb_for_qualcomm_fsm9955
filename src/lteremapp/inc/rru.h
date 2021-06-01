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

    Name:   RRC common hash defines

    Type:   C include file

    Desc:   Defines required by RRC user

    File:   rru.h

    Sid:      rru.h@@/main/13 - Wed May 22 14:43:20 2013

    Prg:    vk

**********************************************************************/


/*
*     The defines in this file correspond to structures and variables
*     used by the following TRILLIUM software:
*
*     part no.             description
*     --------     ----------------------------------------------
*     1000206       RRC
*
*/


#ifndef __RRU_H__
#define __RRU_H__

/* include files */
#include "rru_asn.h"         /* ASN.1 definitions */

#define CMCHKUNPKRRPTR(func, val, ptr, mBuf) \
{ \
   S16 ret; \
   if ((ret = func(val, mBuf, ptr)) != ROK) \
   RETVALUE(ret); \
}

/* Event Types for RRU interface */
#define EVTRRUBNDREQ         0x01 /* Bind request */
#define EVTRRUBNDCFM         0x02 /* Bind Confirm */
#define EVTRRUUBNDREQ        0x03 /* Unbind request */
#define EVTRRUDATREQ         0x04 /* Date Request */
#define EVTRRUDATIND         0x05 /* Data Indication */
#define EVTRRUDATRSP         0x06 /* Data Response */
#define EVTRRUENCDATREQ      0x07 /* Encode data request */
#define EVTRRUENCDATCFM      0x08 /* Encode data confirm */
#define EVTRRUDECDATREQ      0x09 /* Decode data request */
#define EVTRRUDECDATCFM      0x0a /* Decode data confirm */
#define EVTRRUERRIND         0x0b /* Error Indication */
#define EVTRRUCFGREQ         0x0c /* lower layer config request */
#define EVTRRUCFGCFM         0x0d /* lower layer config confirm */
#define EVTRRUMESCFGREQ      0x0e /* Measurement request */
#define EVTRRUMESRPTIND      0x0f /* MeasurementReport Indication */
#define EVTRRUSTATUSIND      0x10 /* Status indication */
#define EVTRRUCNCLUEREQ      0x11 /* Cancel UE request */
#define EVTRRUPREDEFCFGREQ   0x12 /* Pre-defined config request */
#define EVTRRUCELLCFGREQ     0x13 /* Cell config request */
#define EVTRRUCELLCFGCFM     0x14 /* Cell  config confirm */
#define EVTRRUDELETETRANSREQ 0x15 /* Delete transaction request */
#define EVTRRUCOUNTCREQ      0x16 /* Measurement request */
#define EVTRRUCOUNTCIND      0x17 /* Measurement request */
#define EVTRRUMBMSDATREQ     0x18 /* MBMS Data Request */
#define EVTRRUMBMSTXREQ      0x19 /* MBMS Transmit Request */
#define EVTRRURELOCPREPINFOREQ   0x1A /* SRNS Relocation PrepareInfo Request */
#define EVTRRURELOCPREPINFOCFM   0x1B /* SRNS Relocation PrepareInfo Confirm */
#define EVTRRURELOCCMTINFOREQ    0x1C /* SRNS Relocation CommitInfo Request */
#define EVTRRURELOCCMTINFOCFM    0x1D /* SRNS Relocation CommitInfo Confirm */

/* SysInfoType */
#define RRU_SIB_1      0x01 /* Sys Info type 1 */
#define RRU_SIB_2      0x02 /* Sys Info type 2 */
#define RRU_SIB_3      0x03 /* Sys Info type 3 */
#define RRU_SIB_4      0x04 /* Sys Info type 4 */
#define RRU_SIB_5      0x05 /* Sys Info type 5 */
#define RRU_SIB_5BIS   0x06 /* Sys Info type 5bis */
#define RRU_SIB_6      0x07 /* Sys Info type 6 */
#define RRU_SIB_7      0x08 /* Sys Info type 7 */
#define RRU_SIB_8      0x09 /* Sys Info type 8 */
#define RRU_SIB_9      0x0a /* Sys Info type 9 */
#define RRU_SIB_10     0x0b /* Sys Info type 10 */
#define RRU_SIB_11     0x0c /* Sys Info type 11 */
#define RRU_SIB_11BIS  0x0d /* Sys Info type 11bis */
#define RRU_SIB_12     0x0e /* Sys Info type 12 */
#define RRU_SIB_13     0x0f /* Sys Info type 13 */
#define RRU_SIB_13_1   0x10 /* Sys Info type 13_1 */
#define RRU_SIB_13_2   0x11 /* Sys Info type 13_2 */
#define RRU_SIB_13_3   0x12 /* Sys Info type 13_3 */
#define RRU_SIB_13_4   0x13 /* Sys Info type 13_4 */
#define RRU_SIB_15     0x14 /* Sys Info type 15 */
#define RRU_SIB_15_1   0x15 /* Sys Info type 15_1 */
#define RRU_SIB_15_2   0x16 /* Sys Info type 15_2 */
#define RRU_SIB_15_3   0x17 /* Sys Info type 15_3 */
#define RRU_SIB_15_4   0x18 /* Sys Info type 15_4 */
#define RRU_SIB_15_5   0x19 /* Sys Info type 15_5 */
#define RRU_SIB_16     0x1a /* Sys Info type 16 */
#define RRU_SIB_18     0x1b /* Sys Info type 18 */
#define RRU_SIB_SB1    0x1c /* Scheduling Block 1 */
#define RRU_SIB_SB2    0x1d /* Scheduling Block 2 */
#define RRU_SIB_MIB    0x1e /* Master Information Block */
#define RRU_SIB_20     0x1f /* rru_h_001.main_8 Sys Info type 20 */



/* msgCategory */
#define RRU_MSG_DLDCCH     0x01 /* DL-DCCH message */
#define RRU_MSG_ULDCCH     0x02 /* UL-DCCH message */
#define RRU_MSG_DLCCCH     0x03 /* DL-CCCH message */
#define RRU_MSG_ULCCCH     0x04 /* UL-CCCH message */
#define RRU_MSG_DLPCCH     0x05 /* DL-PCCH message */
#define RRU_MSG_DLBCCHFACH 0x06 /* DL-BCCH on FACH message */
#define RRU_MSG_DLBCCHBCH  0x07 /* DL-BCCH on BCH message */
#define RRU_MSG_MCCH       0x08 /* MCCH message */
#define RRU_MSG_MSCH       0x09 /* MSCH message */
#define RRU_MSG_SIB        0x0a /* System information block */
#define RRU_MSG_SRNCTOTRNC_CONTA  0x0b /* Source RNC to Target RNC conatiner */
#define RRU_MSG_TRNCTOSRNC_CONTA  0x0c /* Target RNC to Source RNC container */
#define RRU_MSG_HOTOUTRAN_CMD     0x0d /* HO To UTRAN CMD */
#define RRU_MSG_INTERRATHO_INFO   0x0e /* Inter RAT HO Info */
#define RRU_MSG_HOTOUTRAN_CMD_R7  0x0f /* HO To UTRAN CMD R7 IEs */
/* rru_h_001.main_6 Adding message category for UE Capability conatiner */
#define RRU_MSG_UE_CAPAB_CONTA    0x10 /* UE CAPABILITY CONTAINER IEs */

/* mcchInfoBitMsk */
#define RRU_MCCH_MSG_MBMS_ACCS    0x01
#define RRU_MCCH_MSG_MBMS_MOD     0x02
#define RRU_MCCH_MSG_MBMS_UNMOD   0x04

/* msg bit mask for MBMS MCCH messages */
#define RRU_MBMS_MOD_SRV_INFO    0x01
#define RRU_MBMS_UNMOD_SRV_INFO  0x02
#define RRU_MBMS_GEN_INFO        0x04
#define RRU_MBMS_COMMON_INFO     0x08
#define RRU_MBMS_CUR_CELL_INFO   0x10
#define RRU_MBMS_NEIGH_CELL_INFO 0x40

/* errType */
#define RR_ERRTYPE_NONE                     0x00 /* reserved, not to be used */
#define RR_ERRTYPE_RLC_DAT_IND              0x01
#define RR_ERRTYPE_RLC_DAT_CFM              0x02
#define RRU_ERRTYPE_INV_DEL_TRANS_REQ       0x03
#define RRU_ERRTYPE_INV_CNCL_UE_REQ         0x04
#define RRU_ERRTYPE_INV_DEFAULT_CFG         0x05
#define RRU_ERRTYPE_INV_SDU_PARAM           0x06
#define RRU_ERRTYPE_ENC_FAILED              0x07
#define RRU_ERRTYPE_DEC_FAILED              0x08
#define RRU_ERRTYPE_INV_RRU_HDR             0x09
#define RRU_ERRTYPE_INV_UE_MSG_PARAM        0x0a
#define RRU_ERRTYPE_INV_CFG                 0x0b
#define RRU_ERRTYPE_TX_FAILED               0x0c
#define RRU_ERRTYPE_INV_RLS_REQ             0x0d
#define RRU_ERRTYPE_MSG_SEND_FAILED         0x0e
#define RRU_ERRTYPE_RELOCINFO_NOT_RCVD      0x0f
#define RRU_ERRTYPE_COUNTCCFM_NOT_RCVD      0x10
/* rru_h_001.main_7 Validation of the primitives */
#define RRU_ERRTYPE_RELOC_PREP_INFO_REQ     0x11
#define RRU_ERRTYPE_RELOC_COMMIT_INFO_REQ   0x12

/* errCause */
#define RRU_ERRCAUSE_NONE                       0x00
#define RRU_ERRCAUSE_WRONG_RB_ID                0x01
#define RRU_ERRCAUSE_CELLID_NOT_CONFIGURED      0x02
#define RRU_ERRCAUSE_UEID_NOT_CONFIGURED        0x03
#define RRU_ERRCAUSE_RB_NOT_CONFIGURED          0x04
#define RRU_ERRCAUSE_DAT_CFM_DISCARD            0x05
#define RRU_ERRCAUSE_CFG_IN_PROGRESS            0x06
#define RRU_ERRCAUSE_DEF_CFG_NOT_PRSNT          0x07
#define RRU_ERRCAUSE_INV_PREDEF_CFG             0x08
#define RRU_ERRCAUSE_INV_CN_DOMAIN              0x09
#define RRU_ERRCAUSE_INV_HDR_IDCHOICE           0x0a
#define RRU_ERRCAUSE_INV_HDR_PRES               0x0b
#define RRU_ERRCAUSE_INV_RX_TRANSID             0x0c
#define RRU_ERRCAUSE_INV_RRC_TRANSID            0x0d
#define RRU_ERRCAUSE_TRANSCB_UPD_FAIL           0x0e
#define RRU_ERRCAUSE_LOW_LYR_CFG_FAIL           0x0f
#define RRU_ERRCAUSE_TRANSCB_NOT_FOUND          0x10
#define RRU_ERRCAUSE_INVALID_CFG_INFO           0x11
#define RRU_ERRCAUSE_INV_BITMASK                0x12
#define RRU_ERRCAUSE_INV_SDU_CHOICE_VAL         0x13
#define RRU_ERRCAUSE_INV_MBMS_TX_PARAM          0x14
#define RRU_ERRCAUSE_INV_MSG_CATEGORY           0x15
#define RRU_ERRCAUSE_INV_TRCH                   0x16
#define RRU_ERRCAUSE_INV_CFG_TYPE               0x17
#define RRU_ERRCAUSE_RLS_IN_PROGRESS            0x18
#define RRU_ERRCAUSE_FAILED_SENDTORLC           0x19
#define RRU_ERRCAUSE_INV_NMB_OF_NEIGH_CELLS     0x20
#define RRU_ERRCAUSE_SCCPCH_NOT_CONFIGURED      0x21
#define RRU_ERRCAUSE_INV_RB_CFG_INFO            0x22
#define RRU_ERRCAUSE_INTEGRITY_CFG_MISMATCH     0x23
#define RRU_ERRCAUSE_RB_SUSPENDED               0x24
#define RRU_ERRCAUSE_CFG_PARAM_OUT_OF_BOUNDS    0x25
#define RRU_ERRCAUSE_ALGORITHM_MISMATCH         0x26
#define RRU_ERRCAUSE_MAX_INTG_CFG_PRESENT       0x27
#define RRU_ERRCAUSE_TIMEOUT                    0x28
#define RRU_ERRCAUSE_INV_DRNC_CFG_TYPE          0x29


/* bitmask for RruExpCfg */
#define RRU_EXP_CFG_RB_INFO  0x01 /* RB info */
#define RRU_EXP_CFG_CCTRCH_INFO   0x02 /* Cctrch info */
#define RRU_EXP_CFG_UE_INFO      0x04
#define RRU_EXP_CFG_SEC_INFO   0x08 /* Security info */
#define RRU_EXP_CFG_SRNS_RELOC_INFO 0x10 /* SRNS relocation info */
#define RRU_EXP_CFG_DRIFTUE_SRNC_INFO 0x20 /* SRNS relocation info */
#define RRU_EXP_CFG_DRIFTUE_DRNC_INFO 0x40 /* SRNS relocation info */

/* MACROS to know which config type will be applied during a drifted UE case */
#define RRU_CFG_TYPE_FACH        0x01  /* Configuration Type FACH */
#define RRU_CFG_TYPE_RACH        0x02  /* Configuration Type RACH */
#define RRU_CFG_TYPE_HSDSCH      0x04  /* Configuration Type HSDSCH */

#define RRU_CFG_SRC_SRNC         0x01
#define RRU_CFG_SRC_DRNC         0x02

/* actionmask for Fach Config */
#define RRU_CFG_FACH_ADD        0x01
#define RRU_CFG_FACH_MODIFY     0x02
#define RRU_CFG_FACH_RLS        0x04

/* actionmask for Rach Config */
/* Only Add and Release are supported */
#define RRU_CFG_RACH_ADD        0x01
#define RRU_CFG_RACH_RLS        0x02

/* actionmask for HSDSCH Config */
/* Only Add and Release are supported */
#define RRU_CFG_HSDSCH_ADD        0x01
#define RRU_CFG_HSDSCH_RLS        0x02



/* bit mask for RruRbCfgInfo */
#define RRU_RB_CFG_UL            0x01
#define RRU_RB_CFG_DL            0x02
#define RRU_RB_CFG_PDCP_INFO     0x04

/* bit mask for RruUlRbCfg */
#define RRU_RB_CFG_LOGCH_MAP_UL  0x01
#define RRU_RB_CFG_RLC_MODE_UL   0x02
#define RRU_RB_CFG_CIPH_INFO_UL  0x04

/* bit mask for RruDlRbCfg */
#define RRU_RB_CFG_LOGCH_MAP_DL  0x01
#define RRU_RB_CFG_RLC_MODE_DL   0x02
#define RRU_RB_CFG_CIPH_INFO_DL  0x04

/* infoChoice for RLC Mode */
#define RRU_RB_RLC_MODE_AM   0x01
#define RRU_RB_RLC_MODE_UM   0x02
#define RRU_RB_RLC_MODE_TM   0x03

/* bit mask for RruUlTmRlcModeInfo */
#define RRU_RB_RLC_MODE_UL_TM_TX_DISCARD   0x01
#define RRU_RB_RLC_MODE_UL_TM_SEG_IND      0x02

/* choice for RruTxRlcDiscard */
#define RRU_RB_RLC_TX_DISCARD_TMR_BASED_EXP      0x01 /* for AM */
#define RRU_RB_RLC_TX_DISCARD_TMR_BASED_NO_EXP   0x02 /* for UM,TM */
#define RRU_RB_RLC_TX_DISCARD_MAX_DAT_RETRANS    0x03 /* for AM */
#define RRU_RB_RLC_TX_DISCARD_NO_DISCARD         0x04 /* for AM */
/* rru_h_001.main_10 - adding macro to indicate that there is no 
                       discard related configuration to be made at RLC. */
#define RRU_RB_RLC_TX_DISCARD_NOT_CFG            0x05 /* for AM,UM,TM */

/* bit mask for RruDlUmRlcModeInfo */
#define RRU_RB_RLC_MODE_DL_UM_RX_WINDOW   0x01
#define RRU_RB_RLC_MODE_DL_UM_RL_PDU_SIZE 0x02
#define RRU_RB_RLC_MODE_DL_UM_TX_DISCARD  0x04
#define RRU_RB_RLC_MODE_DL_UM_RLC_LI      0x08

/* bit mask for RruRbPdcpCfgInfo */
#define RRU_RB_PDCP_CFG_SRNSRELOC_SUP   0x01
#define RRU_RB_PDCP_CFG_MAX_SN_WINDOW   0x02
#define RRU_RB_PDCP_CFG_PDU_HDR         0x04
#define RRU_RB_PDCP_CFG_HDR_COMPR_INFO  0x08
#define RRU_RB_PDCP_CFG_DL_SEND_SN      0x10
#define RRU_RB_PDCP_CFG_CTXT_INFO       0x20
#define RRU_RB_PDCP_CFG_RELOC_INFO      0x40



/* infoChoice for RruPdcpHdrComprInfo */
#define RRU_RB_PDCP_HDR_COMPR_RFC2507   0x01
#define RRU_RB_PDCP_HDR_COMPR_RFC3095   0x02

/* bit mask for RruRfc2507Info */
#define RRU_RB_RFC2507_F_MAX_PERIOD   0x01
#define RRU_RB_RFC2507_F_MAX_TIME     0x02
#define RRU_RB_RFC2507_MAX_HDR        0x04
#define RRU_RB_RFC2507_TCP_SPACE      0x08
#define RRU_RB_RFC2507_EXPCT_REORDER  0x10

/* RLC size list type */
#define RRU_RB_RLC_SIZE_LIST_ALL         0x01
#define RRU_RB_RLC_SIZE_LIST_CONFIGURED  0x02
#define RRU_RB_RLC_SIZE_LIST_EXPLICIT    0x03

/* cfgSrc in RruCfgSdus */
#define RRU_CFGSRC_EXPLICIT     0x01 /* Explicit Configuration */
#define RRU_CFGSRC_DEFAULT      0x02 /* Default configuration */
#define RRU_CFGSRC_PREDEFINED   0x04 /* Pre-defined Configuration */

/* Configuration Type */
#define RRU_CFGTYPE_SETUP      0x01 /* Setup */
#define RRU_CFGTYPE_MODIFY     0x02 /* Modify */
#define RRU_CFGTYPE_RELEASE    0x04 /* Release */
#define RRU_CFGTYPE_STOP       0x08 /* RLC RB Stop */
#define RRU_CFGTYPE_CONTINUE   0x10 /* RLC RB Continue */

/* RruCfgErrType */
#define RRU_CFGERRTYPE_RLC   0x01 /* RLC Configuration error */
#define RRU_CFGERRTYPE_MAC   0x02 /* MAC Configuration error */
#define RRU_CFGERRTYPE_PDCP  0x04 /* PDCP Configuration error */

/* RruCfgErrCause */
#define RRU_CFGERRCAUSE_INV_PARAM     0x01 /* Invalid parameters */
#define RRU_CFGERRCAUSE_INV_STATE     0x02 /* invalid state */
#define RRU_CFGERRCAUSE_INCOMPATIBLE  0x03 /* Incompatible config */
#define RRU_CFGERRCAUSE_UNKNOWN       0x04 /* Unknown error ar layer */

/* srnsRelocSupChoice */
#define RRU_SRNS_RELOC_SUPPORTED       0x01
#define RRU_SRNS_RELOC_NOT_SUPPORTED   0x02

/* Identifier present */
#define RRU_CELL_ID   0x01
#define RRU_UE_ID     0x02
#define RRU_CELL_GRP_ID  0x04

/* bitmask for data request */
#define RRU_DAT_UE_MSG   0x01
#define RRU_DAT_UE_CFG   0x02

/* bitmask for CCTRCH config list */
#define RRU_CFG_CCTRCH_UL   0x01
#define RRU_CFG_CCTRCH_DL   0x02

/* bitmask for CcTrch Config */
#define RRU_CFG_CCTRCH_TFS_ADD        0x01
#define RRU_CFG_CCTRCH_TFS_MODIFY     0x02
#define RRU_CFG_CCTRCH_TFS_RLS        0x04

/* bitmask for RB Config list */
#define RRU_CFG_RB_ADD       0x01
#define RRU_CFG_RB_MODIFY    0x02
#define RRU_CFG_RB_RLS       0x04
/* rru_h_001.main_11 : Following flag is added to fix ciphering config error while
   transfering CELL_DCH to CELL_FACH is reported in CCPU00121443*/
/* changing the RRU_CFG_RB_MOD_CIPH from 0x10 as it is overlapping with another MASK */
#define RRU_CFG_RB_MOD_CIPH  0x00800000

/* bit mask for RruCellCfg */
#define RRU_CELL_CFG_RB         0x01
#define RRU_CELL_CFG_TFCS       0x02
#define RRU_CELL_CFG_TFS        0x04

/* bit mask for RruDlStatusInfo */
#define RRU_RLC_AM_STS_PERIODIC             0x01
#define RRU_RLC_AM_STS_MISSING_PDU_IND      0x02
#define RRU_RLC_AM_STS_TMR_EPC              0x04
#define RRU_RLC_AM_STS_TMR_STS_PROH         0x08

/* bit mask for RruCellCfgList */
#define RRU_CELL_CFG_PRACH   0x01
#define RRU_CELL_CFG_SCCPCH  0x02

/* bit mask for polling Info */
#define RRU_POLL_TMR_POLL_PROH        0x01
#define RRU_POLL_TMR_POLL             0x02
#define RRU_POLL_TMR_POLL_PDU         0x04
#define RRU_POLL_TMR_POLL_SDU         0x08
#define RRU_POLL_LAST_TX_PDU_POLL     0x10
#define RRU_POLL_LAST_RETX_PDU_POLL   0x20
#define RRU_POLL_WINDOW               0x40
#define RRU_POLL_TMR_POLL_PERIODIC    0x80

/* types of RB in RbCfgInfo */
#define RRU_RB_TYPE_SRB               0x01
#define RRU_RB_TYPE_DATA_RB           0x02
/*default configuration defines*/
#define RB_TO_RLS 0x00
#define RB_TO_RECFG 0x01


/* Coupling flags */
#define RRU_SEL_LC   0
#define RRU_SEL_TC   1
#define RRU_SEL_LWLC 2

/* macros for Urnti/Crnti in RruUmtsUeId */
#define RRU_UE_ID_CRNTI   0x01
#define RRU_UE_ID_URNTI   0x02

/* Maximum length of Paging Indication Bitmap */
#define RRU_MAX_PI_BITMAP_LEN   18

/* macros for presence/absence of activation time */
#define RRU_ACTVN_TIME_ABSENT    0
#define RRU_ACTVN_TIME_PRESENT   1

/* Maximum integrity key len */
#define RRU_INTG_KEY_LEN       16
#define RRU_CIPH_KEY_LEN       16

/* RB Type - CS/PS/Common */
/* Rb type needs to be common in case of SRBs */
#define RRU_RB_TYPE_CMN       0
#define RRU_RB_TYPE_CS        1
#define RRU_RB_TYPE_PS        2

/* Security - Integrity Protection SRBs */
#define RRU_MAX_INTG_SRBS     5      /* SRB 0,1,2,3,4 */
#define RRU_INTG_OPT_START    0
#define RRU_INTG_OPT_MODIFY   1
/*rru_h_001.main_9 Update the integrity info at TRNC after SRNC relocation*/
#define RRU_INTG_OPT_RELOC    2

/* Security - Defines for type of ciphering applicable to AM-UM or TM RBs */
#define RRU_AM_UM_CIPH        0
#define RRU_TM_CIPH           1  

/* Max Queue Id for MAC-HS priority queue */
#define RRU_MAX_HS_DSCH_QUEUE_ID            8
/* Number of  RLC PDU for EDCH */
#define RRU_MAX_NMB_RLC_PDU_PER_LOG_CHAN   32
/* number of MAC-D PDU sizes */
#define RRU_NMB_MACD_PDU_SIZES              8
/* number of nodeBs in E-DCH active set */
#define RRU_NMB_NODEB_PER_EDCH_ACTV_SET     4

/* MAC EHS Flow Types */
#define RRU_EHS_FLOWTYPE_UE       1
#define RRU_EHS_FLOWTYPE_COMMON   2
#define RRU_EHS_FLOWTYPE_PAGING   3

/*  Established signalling connection */
#define RRU_MAX_CN_DOMAINS    2   /* CS + PS */

#define RRU_CS_DOMAIN          1
#define RRU_PS_DOMAIN          2

/* RRU Status Indication events */
#define RRU_STATUS_NONE             0x00
#define RRU_STATUS_RLC_RESET        0x01
#define RRU_STATUS_RLC_MRW_FAILED   0x02

/* RRU SRNS Relocation Indication Events */
/* rru_h_001.main_7 Count-C and Count-I support */
#define RRU_SRNS_RELOC_COUNT_VAL 0x01
#define RRU_SRNS_LOSSLESS_RELOC_PDCP_INFO 0x02
#define RRU_SRNS_SEAMLESS_RELOC_PDCP_INFO 0x04
#define RRU_ROHC_PROF_LIST      4

/* rru_h_001.main_7 Count-C support */
/* Direction */
#define RRU_DOWNLINK 1
#define RRU_UPLINK   2

/* rru_h_001.main_5 : multiple lower sap support and drift RNC feature. */
/* bitmask for lower SAP Id */ 
#define RRU_RLU_SAP_ID_INFO      0x01
#define RRU_CRL_SAP_ID_INFO      0x02
#define RRU_CMK_SAP_ID_INFO      0x04
#define RRU_CTC_SAP_ID_INFO      0x08

/*rru_h_001.main_12 - ccpu00129401 */
/* Macros for RRU data structures */
/* rr011.202 KlockWorks Fix, changed 
   RRU_MAX_RB_PER_CELL_CFG 4 to 8 */
#define RRU_MAX_RB_PER_CELL_CFG    8
#define RRU_MAX_RB_PER_UE_CFG      8
#define RRU_MAX_RB_PER_RRC_CONN   32
#define RRU_MAX_RRC_TRCHS         32
#define RRU_MAX_TFS_PER_CFG       10
#define RRU_MAX_RLC_SIZE_LST_LEN  12
#define RRU_MAX_ROHC_PROF_LST_LEN 4
#define RRU_MAX_HDR_COMPR_INFO_LIST_LEN   2
#define RRU_MAX_TFCS_CFG          64
#define RRU_MAX_TF_PER_TRCH       16
#define RRU_MAX_TTI_PER_TRCH      4
#define RRU_MAX_MES_PER_CFG       32
#define RRU_MAX_LOGCH_PER_RLC     2


/* Error Codes */
#define   ERRUXXX      0 /* reserved */
#define   ERRRRU       0 /* reserved */


#define   ERRU001      (ERRRRU +    1)    /*        rru.c: 195 */
#define   ERRU002      (ERRRRU +    2)    /*        rru.c: 196 */
#define   ERRU003      (ERRRRU +    3)    /*        rru.c: 241 */
#define   ERRU004      (ERRRRU +    4)    /*        rru.c: 242 */
#define   ERRU005      (ERRRRU +    5)    /*        rru.c: 287 */
#define   ERRU006      (ERRRRU +    6)    /*        rru.c: 288 */
#define   ERRU007      (ERRRRU +    7)    /*        rru.c:3028 */
#define   ERRU008      (ERRRRU +    8)    /*        rru.c:3045 */
#define   ERRU009      (ERRRRU +    9)    /*        rru.c:3054 */
#define   ERRU010      (ERRRRU +   10)    /*        rru.c:3059 */
#define   ERRU011      (ERRRRU +   11)    /*        rru.c:3357 */
#define   ERRU012      (ERRRRU +   12)    /*        rru.c:3374 */
#define   ERRU013      (ERRRRU +   13)    /*        rru.c:3383 */
#define   ERRU014      (ERRRRU +   14)    /*        rru.c:3388 */
#define   ERRU015      (ERRRRU +   15)    /*        rru.c:3431 */
#define   ERRU016      (ERRRRU +   16)    /*        rru.c:3532 */
#define   ERRU017      (ERRRRU +   17)    /*        rru.c:3537 */
#define   ERRU018      (ERRRRU +   18)    /*        rru.c:3538 */
#define   ERRU019      (ERRRRU +   19)    /*        rru.c:3540 */
#define   ERRU020      (ERRRRU +   20)    /*        rru.c:3582 */
#define   ERRU021      (ERRRRU +   21)    /*        rru.c:3587 */
#define   ERRU022      (ERRRRU +   22)    /*        rru.c:3588 */
#define   ERRU023      (ERRRRU +   23)    /*        rru.c:3590 */
#define   ERRU024      (ERRRRU +   24)    /*        rru.c:3922 */
#define   ERRU025      (ERRRRU +   25)    /*        rru.c:3945 */
#define   ERRU026      (ERRRRU +   26)    /*        rru.c:3950 */
#define   ERRU027      (ERRRRU +   27)    /*        rru.c:3998 */
#define   ERRU028      (ERRRRU +   28)    /*        rru.c:4021 */
#define   ERRU029      (ERRRRU +   29)    /*        rru.c:4026 */
#define   ERRU030      (ERRRRU +   30)    /*        rru.c:4070 */
#define   ERRU031      (ERRRRU +   31)    /*        rru.c:4075 */
#define   ERRU032      (ERRRRU +   32)    /*        rru.c:4076 */
#define   ERRU033      (ERRRRU +   33)    /*        rru.c:4125 */
#define   ERRU034      (ERRRRU +   34)    /*        rru.c:4151 */
#define   ERRU035      (ERRRRU +   35)    /*        rru.c:4156 */
#define   ERRU036      (ERRRRU +   36)    /*        rru.c:4205 */
#define   ERRU037      (ERRRRU +   37)    /*        rru.c:4223 */
#define   ERRU038      (ERRRRU +   38)    /*        rru.c:4232 */
#define   ERRU039      (ERRRRU +   39)    /*        rru.c:4237 */
#define   ERRU040      (ERRRRU +   40)    /*        rru.c:4331 */
#define   ERRU041      (ERRRRU +   41)    /*        rru.c:4348 */
#define   ERRU042      (ERRRRU +   42)    /*        rru.c:4357 */
#define   ERRU043      (ERRRRU +   43)    /*        rru.c:4362 */
#define   ERRU044      (ERRRRU +   44)    /*        rru.c:4488 */
#define   ERRU045      (ERRRRU +   45)    /*        rru.c:4505 */
#define   ERRU046      (ERRRRU +   46)    /*        rru.c:4514 */
#define   ERRU047      (ERRRRU +   47)    /*        rru.c:4519 */
#define   ERRU048      (ERRRRU +   48)    /*        rru.c:4610 */
#define   ERRU049      (ERRRRU +   49)    /*        rru.c:4618 */
#define   ERRU050      (ERRRRU +   50)    /*        rru.c:4701 */
#define   ERRU051      (ERRRRU +   51)    /*        rru.c:4718 */
#define   ERRU052      (ERRRRU +   52)    /*        rru.c:4727 */
#define   ERRU053      (ERRRRU +   53)    /*        rru.c:4732 */
#define   ERRU054      (ERRRRU +   54)    /*        rru.c:4863 */
#define   ERRU055      (ERRRRU +   55)    /*        rru.c:4881 */
#define   ERRU056      (ERRRRU +   56)    /*        rru.c:4890 */
#define   ERRU057      (ERRRRU +   57)    /*        rru.c:4895 */
#define   ERRU058      (ERRRRU +   58)    /*        rru.c:5015 */
#define   ERRU059      (ERRRRU +   59)    /*        rru.c:5033 */
#define   ERRU060      (ERRRRU +   60)    /*        rru.c:5041 */
#define   ERRU061      (ERRRRU +   61)    /*        rru.c:5124 */
#define   ERRU062      (ERRRRU +   62)    /*        rru.c:5141 */
#define   ERRU063      (ERRRRU +   63)    /*        rru.c:5150 */
#define   ERRU064      (ERRRRU +   64)    /*        rru.c:5155 */
#define   ERRU065      (ERRRRU +   65)    /*        rru.c:5274 */
#define   ERRU066      (ERRRRU +   66)    /*        rru.c:5291 */
#define   ERRU067      (ERRRRU +   67)    /*        rru.c:5299 */
#define   ERRU068      (ERRRRU +   68)    /*        rru.c:5432 */
#define   ERRU069      (ERRRRU +   69)    /*        rru.c:5449 */
#define   ERRU070      (ERRRRU +   70)    /*        rru.c:5458 */
#define   ERRU071      (ERRRRU +   71)    /*        rru.c:5463 */
#define   ERRU072      (ERRRRU +   72)    /*        rru.c:5595 */
#define   ERRU073      (ERRRRU +   73)    /*        rru.c:5612 */
#define   ERRU074      (ERRRRU +   74)    /*        rru.c:5621 */
#define   ERRU075      (ERRRRU +   75)    /*        rru.c:5626 */
#define   ERRU076      (ERRRRU +   76)    /*        rru.c:5708 */
#define   ERRU077      (ERRRRU +   77)    /*        rru.c:5724 */
#define   ERRU078      (ERRRRU +   78)    /*        rru.c:5732 */
#define   ERRU079      (ERRRRU +   79)    /*        rru.c:5737 */
#define   ERRU080      (ERRRRU +   80)    /*        rru.c:5782 */
#define   ERRU081      (ERRRRU +   81)    /*        rru.c:5787 */
#define   ERRU082      (ERRRRU +   82)    /*        rru.c:5788 */
#define   ERRU083      (ERRRRU +   83)    /*        rru.c:5922 */
#define   ERRU084      (ERRRRU +   84)    /*        rru.c:5939 */
#define   ERRU085      (ERRRRU +   85)    /*        rru.c:5948 */
#define   ERRU086      (ERRRRU +   86)    /*        rru.c:5953 */
#define   ERRU087      (ERRRRU +   87)    /*        rru.c:6136 */
#define   ERRU088      (ERRRRU +   88)    /*        rru.c:6153 */
#define   ERRU089      (ERRRRU +   89)    /*        rru.c:6162 */
#define   ERRU090      (ERRRRU +   90)    /*        rru.c:6167 */
#define   ERRU091      (ERRRRU +   91)    /*        rru.c:6287 */
#define   ERRU092      (ERRRRU +   92)    /*        rru.c:6304 */
#define   ERRU093      (ERRRRU +   93)    /*        rru.c:6312 */
#define   ERRU094      (ERRRRU +   94)    /*        rru.c:6360 */
#define   ERRU095      (ERRRRU +   95)    /*        rru.c:6365 */
#define   ERRU096      (ERRRRU +   96)    /*        rru.c:6366 */
#define   ERRU097      (ERRRRU +   97)    /*        rru.c:6367 */
#define   ERRU098      (ERRRRU +   98)    /*        rru.c:6409 */
#define   ERRU099      (ERRRRU +   99)    /*        rru.c:6410 */
#define   ERRU100      (ERRRRU +  100)    /*        rru.c:6450 */
#define   ERRU101      (ERRRRU +  101)    /*        rru.c:6451 */
#define   ERRU102      (ERRRRU +  102)    /*        rru.c:6491 */
#define   ERRU103      (ERRRRU +  103)    /*        rru.c:6492 */
#define   ERRU104      (ERRRRU +  104)    /*        rru.c:9383 */
#define   ERRU105      (ERRRRU +  105)    /*        rru.c:9402 */
#define   ERRU106      (ERRRRU +  106)    /*        rru.c:9410 */
#define   ERRU107      (ERRRRU +  107)    /*        rru.c:9751 */
#define   ERRU108      (ERRRRU +  108)    /*        rru.c:9759 */
#define   ERRU109      (ERRRRU +  109)    /*        rru.c:10096 */
#define   ERRU110      (ERRRRU +  110)    /*        rru.c:10368 */
#define   ERRU111      (ERRRRU +  111)    /*        rru.c:10377 */
#define   ERRU112      (ERRRRU +  112)    /*        rru.c:10519 */
#define   ERRU113      (ERRRRU +  113)    /*        rru.c:10529 */
#define   ERRU114      (ERRRRU +  114)    /*        rru.c:10578 */
#define   ERRU115      (ERRRRU +  115)    /*        rru.c:10579 */
#define   ERRU116      (ERRRRU +  116)    /*        rru.c:10627 */
#define   ERRU117      (ERRRRU +  117)    /*        rru.c:10662 */
#define   ERRU118      (ERRRRU +  118)    /*        rru.c:10710 */
#define   ERRU119      (ERRRRU +  119)    /*        rru.c:10729 */
#define   ERRU120      (ERRRRU +  120)    /*        rru.c:10737 */
#define   ERRU121      (ERRRRU +  121)    /*        rru.c:10841 */
#define   ERRU122      (ERRRRU +  122)    /*        rru.c:10860 */
#define   ERRU123      (ERRRRU +  123)    /*        rru.c:10868 */
#define   ERRU124      (ERRRRU +  124)    /*        rru.c:11006 */
#define   ERRU125      (ERRRRU +  125)    /*        rru.c:11025 */
#define   ERRU126      (ERRRRU +  126)    /*        rru.c:11033 */
#define   ERRU127      (ERRRRU +  127)    /*        rru.c:11127 */
#define   ERRU128      (ERRRRU +  128)    /*        rru.c:11141 */
#define   ERRU129      (ERRRRU +  129)    /*        rru.c:11236 */
#define   ERRU130      (ERRRRU +  130)    /*        rru.c:11255 */
#define   ERRU131      (ERRRRU +  131)    /*        rru.c:11263 */
#define   ERRU132      (ERRRRU +  132)    /*        rru.c:11402 */
#define   ERRU133      (ERRRRU +  133)    /*        rru.c:11421 */
#define   ERRU134      (ERRRRU +  134)    /*        rru.c:11429 */
#define   ERRU135      (ERRRRU +  135)    /*        rru.c:11558 */
#define   ERRU136      (ERRRRU +  136)    /*        rru.c:11572 */
#define   ERRU137      (ERRRRU +  137)    /*        rru.c:11668 */
#define   ERRU138      (ERRRRU +  138)    /*        rru.c:11687 */
#define   ERRU139      (ERRRRU +  139)    /*        rru.c:11695 */
#define   ERRU140      (ERRRRU +  140)    /*        rru.c:11829 */
#define   ERRU141      (ERRRRU +  141)    /*        rru.c:11843 */
#define   ERRU142      (ERRRRU +  142)    /*        rru.c:11991 */
#define   ERRU143      (ERRRRU +  143)    /*        rru.c:12010 */
#define   ERRU144      (ERRRRU +  144)    /*        rru.c:12018 */
#define   ERRU145      (ERRRRU +  145)    /*        rru.c:12162 */
#define   ERRU146      (ERRRRU +  146)    /*        rru.c:12181 */
#define   ERRU147      (ERRRRU +  147)    /*        rru.c:12189 */
#define   ERRU148      (ERRRRU +  148)    /*        rru.c:12284 */
#define   ERRU149      (ERRRRU +  149)    /*        rru.c:12297 */
#define   ERRU150      (ERRRRU +  150)    /*        rru.c:12306 */
#define   ERRU151      (ERRRRU +  151)    /*        rru.c:12349 */
#define   ERRU152      (ERRRRU +  152)    /*        rru.c:12350 */
#define   ERRU153      (ERRRRU +  153)    /*        rru.c:12494 */
#define   ERRU154      (ERRRRU +  154)    /*        rru.c:12513 */
#define   ERRU155      (ERRRRU +  155)    /*        rru.c:12521 */
#define   ERRU156      (ERRRRU +  156)    /*        rru.c:12719 */
#define   ERRU157      (ERRRRU +  157)    /*        rru.c:12738 */
#define   ERRU158      (ERRRRU +  158)    /*        rru.c:12746 */
#define   ERRU159      (ERRRRU +  159)    /*        rru.c:12882 */
#define   ERRU160      (ERRRRU +  160)    /*        rru.c:12896 */
#define   ERRU161      (ERRRRU +  161)    /*        rru.c:12944 */
#define   ERRU162      (ERRRRU +  162)    /*        rru.c:12945 */
#define   ERRU163      (ERRRRU +  163)    /*        rru.c:12946 */


#endif /* __RRU_H__ */

/**********************************************************************
         End of file:     rru.h@@/main/13 - Wed May 22 14:43:20 2013
**********************************************************************/
/**********************************************************************

        Revision history:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/2      ---     vk        1. RRC Initial Release.
/main/3      ---     vk        1. RRC Release 2.1
/main/5      ---     ds        1. RRC Release 2.2
/main/6      ---     dm        1. rru_h_001.main_5. multiple lower sap 
                                  support and drift RNC feature.
/main/7      ---     ds        1. rru_h_001.main_6 Adding message category 
                                  for UE Capability conatiner 
/main/8      ---     gm        1. rru_h_001.main_7 Added Count-I and MAC Count-C support.
/main/9      ---      rru_h_001.main_8     vg        1. added support for SIB20.
/main/10     ---      rru_h_001.main_9  pkn  1. Update the integrity info at TRNC after SRNC relocation
/main/11     ---      rru_h_001.main_10 vkulkarni 1. Adding #define for no RLC discard 
                                       mode configuration.
/main/12     ---      rru_h_001.main_11 sdey    1.  Added one macro to fix ciphering config error
                                  while transfering CELL_DCH to CELL_FACH is
                                  reported in CCPU00121443.
/main/13     ---      rru_h_001.main_12 rpagoti 1.Changes to make provision for new user Qy as
                                      a part of ccpu00129401- Interface related naming
                                      convention changes for new FAP &Cl
*********************************************************************91*/
