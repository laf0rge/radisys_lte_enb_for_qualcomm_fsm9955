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
               

     File:     wr_smm_init.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_SM_INIT_H__
#define __WR_SM_INIT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "wr_lwr.h"
#include "wr_cmn.h"
#include "lrm.h"
#include "lrm.x"
#include "lnl.h"
#include "lnl.x"
#ifdef WR_RSYS_OAM
#include "wr_msm_common.h"
#endif
#include "lys.h"
#include "lys.x"

/* X2AP Default configuration Macros */
#define DFLT_CZ_RES_THR_UP 2
#define DFLT_CZ_RES_THR_LOW 1
#define UNKNOWN_VALUE 1
#define MAX_ENB_NAME_LEN 20
#define WR_X2AP_MAX_PEERS 32
#define WR_MAX_QCI 9
/*Last two bits of DSCP field in ip header are ECN bits*/
#define DSCP_ECN_BITS 0x03
/* Port number to receive CLI command from CLI client */
#ifdef WR_CLI_TARGET_BOARD
#define WR_CLI_READ_PORT_NUM 11000
#endif
#define WR_SM_MAX_PCI  64
#define WR_SM_MAX_EARFCN   128
#define WR_SM_MAX_ROOTSEQ_IDX           512
#define WR_SM_MAX_PRACHCFG_IDX          256
#define WR_SM_MAX_ZEROCORRZONECFG_IDX   64
#define WR_SM_MAX_PRACHFREQOFFSET_IDX   256
/* define the procs */
/*#ifdef SS_4GMX_UCORE*/
#if 1
#define SM_SM_PROC  1
#define SM_NH_PROC  1
#define SM_NX_PROC  1
#define SM_RX_PROC  1
#define SM_SZ_PROC  1
#define SM_CZ_PROC  1
#define SM_SB_PROC  1
#define SM_HI_PROC  1
#define SM_EG_PROC  1
#define SM_WR_PROC  1
#ifdef RM_INTF
#define SM_RM_PROC  1
#endif /*RM_INTF*/
#define SM_NL_PROC  1
#define SM_PJ_PROC  0
#define SM_TF_PROC  0
#define SM_KW_PROC  0
#define SM_RG_PROC  0
#define SM_YS_PROC  0
#else
#define SM_SM_PROC  100
#define SM_NH_PROC  100
#define SM_NX_PROC  100
#define SM_RX_PROC  100
#define SM_SZ_PROC  100
#define SM_CZ_PROC  100

#define SM_SB_PROC  100
#define SM_HI_PROC  100
#define SM_EG_PROC  100
#define SM_WR_PROC  100
#ifdef RM_INTF
#define SM_RM_PROC  100
#endif /*RM_INTF*/
#define SM_NL_PROC  100 //BC_CPU_H_ID
#define SM_PJ_PROC  100
#define SM_TF_PROC  100
#define SM_KW_PROC  100
#define SM_RG_PROC  100
#define SM_YS_PROC  100
#endif

/* wr001.102: Assigned Pool values to 0 */
/* wr004.102: Platform specific changes */
#define RG_POOL     0
#define HI_POOL     0
#define SB_POOL     0
#define SZ_POOL     0
#define CZ_POOL     0
#define NH_POOL     0
#define WR_POOL     0
#define EG_POOL     0
#define YS_POOL     0
#define PJ_POOL     0
#define KW_POOL     0
#ifdef RM_INTF
#define RM_POOL     0 
#endif /*RM_INTF*/

#define SCH_INST_ID   1
#ifdef RM_INTF
#define RRM_INST_ID   1
#endif /*RM_INTF*/

#define WR_YS_SUID                      0
#define WR_YS_SPID                      0
#define WR_NH_SUID                      1
#define WR_NH_SPID                      0
#define WR_PJ_SUID                      0
#define WR_PJ_SPID                      1 /* Vamsee */
#define WR_RG_SUID                      0
#define WR_RG_SPID                      0
#define WR_SZ_SPID                      0
#define WR_EG_SUID                      5
#define WR_EG_SPID                      0
/*RRM->FSM */
#ifdef RM_INTF
#define WR_RM_SUID                      0
#define WR_RM_SPID                      0
#define RM_RG_SUID                      0
#define RM_RG_SPID                      0
#endif /*RM_INTF*/

#define WR_NL_SUID                      0
#define WR_NL_SPID                      0

#define NH_PJ_SPID                      0 /* Vamsee */
#define NH_PJ_SUID                      0

#define RG_YS_SUID                      0
#define RG_YS_SPID                      0
#define RG_SCH_YS_SPID                  1

#define SB_HI_SUID                      0
#define SB_HI_SPID                      0

#define EG_HI_SUID                      0
#define EG_HI_SPID                      1

#define SZ_SB_SUID                      0
#define SZ_SB_SPID                      0
#define CZ_SB_SUID                      0
#define CZ_SB_SPID                      1

#define NH_KW_KWU_SUID                  0
#define NH_KW_CKW_SUID                  1
#define NH_RG_CRG_SUID                  2
#define NH_KW_CPJ_SUID                  3
#define NH_KW_PJU_SUID                  4



#define WR_RG_GEN_CFG                   1
#define WR_RG_RGU_SAP_CFG               2
#define WR_RG_CRG_SAP_CFG               4
#define WR_RG_SCH_INS_CFG               8
#define WR_RG_TFU_SAP_CFG              16

#define WR_KW_GEN_CFG                  1
#define WR_KW_RGU_SAP_CFG              2
#define WR_KW_CKW_SAP_CFG              4
#define WR_KW_KWU_SAP_CFG              8
#define WR_KW_UDX_SAP_CFG              0x10

#define WR_NH_GEN_CFG            1
#define WR_NH_PROT_CFG           2
#define WR_NH_USAP_CFG           4
#define WR_NH_CKW_SAP_CFG        8
#define WR_NH_KWU_SAP_CFG       16 
#define WR_NH_CRG_SAP_CFG       32
#define WR_NH_CPJ_SAP_CFG       64
#define WR_NH_PJU_SAP_CFG      128
#define WR_NH_CTF_SAP_CFG      256

#define WR_SZ_GEN_CFG                  1 
#define WR_SZ_PROT_CFG                 2
#define WR_SZ_SZT_SAP_CFG              4
#define WR_SZ_SCT_SAP_CFG              8
#define WR_SZ_PEER_CFG                16

#define WR_CZ_GEN_CFG                  1 

#define WR_CZ_PROT_CFG                 2
#define WR_CZ_CZT_SAP_CFG              4
#define WR_CZ_SCT_SAP_CFG              8
#define WR_CZ_PEER_CFG                16

#define WR_SB_GEN_CFG                  1
#define WR_SB_TSAP_CFG                 2
#define WR_SB_SCT_SAP_CFG              4 

#define WR_EG_GEN_CFG                  1
#define WR_EG_EGT_SAP_CFG              2
#define WR_EG_TSAP_CFG                 4
#define WR_EG_TSRV_CFG                 8
#define WR_EG_IPADDR_CFG              16


#define WR_WR_GEN_CFG                  1 
#define WR_WR_CTF_CFG                  2 
#define WR_WR_NHU_CFG                  4
#define WR_WR_PJU_CFG                  8 
#define WR_WR_EGT_CFG                  16 
#define WR_WR_SZT_CFG                  32 
#define WR_WR_CZT_CFG                  64 
#define WR_WR_RGR_CFG                  128
#define WR_WR_ENBCFG_CFG               256
#define WR_WR_CELLCFG_CFG              512
#define WR_WR_MME_CFG                  1024
#define WR_WR_NHCFG_CFG                2048 
#define WR_WR_NREUTRAN_CFG             4096
#define WR_WR_NRUTRAN_CFG              8192
#define WR_WR_NHCELL_CFG               16384
#define WR_WR_NHUTRAN_CELL_CFG         32768
#define WR_WR_INTER_NHCELL_CFG         65536
#define WR_WR_CDMA1XRTT_BAND_CFG       131072  
#define WR_WR_CDMA1XRTT_FREQ_CFG       262144
#define WR_WR_CDMA1XRTT_CELL_CFG       524288
#define WR_WR_CDMAHRPD_FREQ_CFG        1048576
#define WR_WR_GERAN_FREQ_CFG           2097152 
#define WR_WR_GERAN_CELL_CFG           4194304 
#define WR_WR_DG_OAM_CFG               8388608
#ifdef RM_INTF
#define WR_WR_RMU_SAP_CFG              (1<<23)
#define WR_WR_RGM_SAP_CFG              (1<<24)
#else
#define WR_WR_RMU_SAP_CFG              0 
#define WR_WR_RGM_SAP_CFG              0 
#endif /*RM_INTF*/
#define WR_WR_NLU_SAP_CFG              (1<<25)

#ifdef RM_INTF
#define SM_RM_GEN_CFG                  1
#define SM_RM_RMU_SAP_CFG              1<<1 
#define SM_RM_RMU_PROT_CFG             1<<2 
#define SM_RM_RGM_SAP_CFG              1<<3
#define SM_RM_CONFIGURED  (SM_RM_GEN_CFG | SM_RM_RMU_SAP_CFG | SM_RM_RMU_PROT_CFG | SM_RM_RGM_SAP_CFG)
#endif /*RM_INTF*/

#define SM_NL_GEN_CFG                  1
#define SM_NL_NLU_SAP_CFG              1<<1
#define SM_NL_CELL_CFG                 1<<2
#define SM_NL_NGH_CELL_CFG             1<<3
#define SM_NL_PCI_CFG_IND              1<<4
#define SM_NL_EARFCN_CFG_IND           1<<5
#define SM_NL_PRACH_CFG_IND            1<<6
#define SM_NL_TXPWR_CFG_IND            1<<7
#define SM_NL_CONFIGURED               (SM_NL_GEN_CFG | SM_NL_NLU_SAP_CFG | SM_NL_CELL_CFG | SM_NL_NGH_CELL_CFG | \
                                         SM_NL_PCI_CFG_IND | SM_NL_EARFCN_CFG_IND | SM_NL_PRACH_CFG_IND | SM_NL_TXPWR_CFG_IND)


#define WR_SM_MAX_PSC_VAL 511
#define WR_SM_MAX_CELL_PARAM_VAL 127
#define WR_SM_MIN_PSC_VAL 0
#define WR_SM_START_EXT_RNC 4096
#define WR_SM_MAX_B2_THRESH1_RSCP 91
#define WR_SM_MIN_B2_THRESH1_RSCP -5
#define WR_SM_MAX_B2_THRESH1_RSRP 97
#define WR_SM_MAX_B2_THRESH1_RSRQ 34
#define WR_SM_MAX_B2_THRESH1_RSRQ 34
#define WR_SM_MAX_B2_THRESH1_ECNO 49



#define WR_YS_GEN_CFG                  1
#define WR_YS_CTF_SAP_CFG              2
#define WR_YS_TFU_SAP_CFG              4
#ifdef E_TM
#define WR_YS_CELL_CFG                 8 
#define WR_YS_ETM_CFG                  9
#endif /* E_TM */
#define WR_PJ_GEN_CFG                  1
#define WR_PJ_CPJ_SAP_CFG              2
#define WR_PJ_PJU_SAP_CFG              4
#define WR_PJ_KWU_SAP_CFG              8
#define WR_PJ_UDX_SAP_CFG              16
/* wr002.102: Added maximum file path */
#if (WR_SM_LOG_TO_FILE && DEBUGP)
#define WR_SM_MAX_FILE_PATH            256
#endif

#define   WR_SMM_MAX_NBR_ENB          16
#define   WR_SMM_INVALID_PEERID       0

#define WR_SM_PJ_UL_CONFIGURED  (WR_PJ_GEN_CFG | WR_PJ_CPJ_SAP_CFG | \
                                 WR_PJ_PJU_SAP_CFG | WR_PJ_KWU_SAP_CFG |\
                                 WR_PJ_UDX_SAP_CFG)
#define WR_SM_PJ_DL_CONFIGURED  (WR_PJ_GEN_CFG | WR_PJ_PJU_SAP_CFG | \
                                 WR_PJ_KWU_SAP_CFG | WR_PJ_UDX_SAP_CFG)
#ifdef EU_DAT_APP
#define WR_SM_WR_CONFIGURED  (WR_WR_GEN_CFG | WR_WR_CTF_CFG | WR_WR_NHU_CFG | \
               WR_WR_PJU_CFG | WR_WR_EGT_CFG | WR_WR_SZT_CFG| \
                              WR_WR_CZT_CFG | WR_WR_RGR_CFG | WR_WR_ENBCFG_CFG | \
                              WR_WR_CELLCFG_CFG | WR_WR_MME_CFG | WR_WR_NHCFG_CFG | \
                              WR_WR_NREUTRAN_CFG | WR_WR_NRUTRAN_CFG | WR_WR_NHCELL_CFG | \
                              WR_WR_NHUTRAN_CELL_CFG | WR_WR_INTER_NHCELL_CFG | WR_WR_CDMA1XRTT_BAND_CFG | \
                              WR_WR_CDMA1XRTT_FREQ_CFG | WR_WR_CDMA1XRTT_CELL_CFG | \
                              WR_WR_RMU_SAP_CFG | WR_WR_GERAN_FREQ_CFG |WR_WR_GERAN_CELL_CFG | WR_WR_NLU_SAP_CFG | \
                              WR_WR_CDMAHRPD_FREQ_CFG)
                              
#else
#define WR_SM_WR_CONFIGURED  (WR_WR_GEN_CFG | WR_WR_CTF_SAP_CFG | \
                              WR_WR_NHU_SAP_CFG | \
                              WR_WR_RGR_SAP_CFG | \
                              WR_WR_RMU_SAP_CFG | \
                              WR_WR_NLU_SAP_CFG | \
                              WR_WR_SZT_SAP_CFG)
#endif /* end of EU_DAT_APP */

#define WR_HI_GEN_CFG        1
#define WR_HI_TSAP_CFG       2

#define WR_SM_HI_CONFIGURED  (WR_HI_GEN_CFG | WR_HI_TSAP_CFG)
#define WR_SM_SB_CONFIGURED  (WR_SB_GEN_CFG | WR_SB_TSAP_CFG | WR_SB_SCT_SAP_CFG)
#define WR_SM_SZ_CONFIGURED  (WR_SZ_GEN_CFG | WR_SZ_PROT_CFG | WR_SZ_SZT_SAP_CFG | \
                              WR_SZ_SCT_SAP_CFG | WR_SZ_PEER_CFG)
#ifdef WR_RSYS_OAM
#define WR_SM_CZ_CONFIGURED      (WR_CZ_GEN_CFG | WR_CZ_PROT_CFG | WR_CZ_CZT_SAP_CFG | \
                                  WR_CZ_SCT_SAP_CFG )
#else
#define WR_SM_CZ_CONFIGURED      (WR_CZ_GEN_CFG | WR_CZ_PROT_CFG | WR_CZ_CZT_SAP_CFG | \
                                  WR_CZ_SCT_SAP_CFG | WR_CZ_PEER_CFG)
#endif
#define WR_SM_CZ_CONFGRD_WO_PEER (WR_CZ_GEN_CFG | WR_CZ_PROT_CFG | WR_CZ_CZT_SAP_CFG | \
                                  WR_CZ_SCT_SAP_CFG)

#define WR_SM_EG_CONFIGURED  (WR_EG_GEN_CFG | WR_EG_EGT_SAP_CFG | \
                              WR_EG_TSAP_CFG | WR_EG_TSRV_CFG | WR_EG_IPADDR_CFG)


#define WR_SM_YS_CONFIGURED   (WR_YS_GEN_CFG | WR_YS_CTF_SAP_CFG | WR_YS_TFU_SAP_CFG)

#define WR_SM_RG_CONFIGURED  (WR_RG_GEN_CFG |  WR_RG_RGU_SAP_CFG |\
                              WR_RG_CRG_SAP_CFG | WR_RG_SCH_INS_CFG |\
                              WR_RG_TFU_SAP_CFG)
#define WR_SM_KW_UL_CONFIGURED  (WR_KW_GEN_CFG |  WR_KW_RGU_SAP_CFG |\
                                WR_KW_CKW_SAP_CFG | WR_KW_KWU_SAP_CFG | \
                                WR_KW_UDX_SAP_CFG)
#define WR_SM_KW_DL_CONFIGURED  (WR_KW_GEN_CFG |  WR_KW_RGU_SAP_CFG |\
                               WR_KW_KWU_SAP_CFG | WR_KW_UDX_SAP_CFG)
#define WR_SM_NH_CONFIGURED  (WR_NH_GEN_CFG | WR_NH_PROT_CFG | WR_NH_USAP_CFG | \
                              WR_NH_CKW_SAP_CFG | WR_NH_KWU_SAP_CFG | \
                              WR_NH_CRG_SAP_CFG | WR_NH_CPJ_SAP_CFG | \
                              WR_NH_PJU_SAP_CFG )

/* EKPAL */
#define WR_SM_KWUL_INST               0
#define WR_SM_PJUL_INST               0
#define WR_SM_PJDL_INST               1

#define WR_SM_KWDL_INST               1

#define WR_KWU_UL_SUID                0
#define WR_KWU_DL_SUID                1
#define WR_SM_KW_INST                 0
#define WR_SM_PJ_INST                 0

#define WRSM_LYS_MAX_TFU_SAPS         2
#define WRSM_LYS_NMB_UES              2

/* These shall be used to determine the choice of parameter for ENB
 * reconfiguration 
 */
#define WRSM_ENB_CPU_LOAD     1

#ifdef ENB_OVRLOAD
#define WR_CRIT_THRLD_VAL         99;
#define WR_MAX_THRLD_VAL          95; 
#define WR_MIN_THRLD_VAL          85;
#define WR_CRIT_RCVRY_VAL         96;
#define WR_MAX_RCVRY_VAL          86;
#define WR_MIN_RCVRY_VAL          50;
#define WR_ENB_MEM_CRIT_THRES_VAL 6;
#define WR_ENB_MEM_MAX_THRES_VAL  7;
#define WR_ENB_MEM_MIN_THRES_VAL  8;
#define WR_ENB_OVLD_MON_TMR_VAL   1000;
#define WR_ENB_OVLD_MON_FREQ      5;
#define WR_MAX_CNT_VAL            5;
#define WR_ENB_OVLD_FLAG          0;
#endif
/* RIM Macros */
#define WR_RIM_OVRL_TMR_MIN_VAL        3600000
#define WR_RIM_OVRL_TMR_DFLT_VAL       86400000
#define WR_RIM_OVRL_TMR_MAX_VAL        172800000

#define WR_RIM_TMR_MIN_VAL             1000
#define WR_RIM_TMR_DFLT_VAL            5000
#define WR_RIM_TMR_MAX_VAL             50000

#define WR_RIM_RETRY_CNT_MIN_VAL       0
#define WR_RIM_RETRY_CNT_DFLT_VAL      2
#define WR_RIM_RETRY_CNT_MAX_VAL       20
/* RIM Macros */

#define WR_FILL_PST(_pst,_srcProc,_dstEnt,_srcEnt,_event,_pool,_selector) \
{                                                                \
   cmMemset((U8 *)&(_pst), 0, sizeof(Pst));                       \
   _pst.dstProcId = SM_SM_PROC;                                    \
   _pst.srcProcId = _srcProc;                                    \
   _pst.dstEnt = _dstEnt;                                        \
   _pst.srcEnt = _srcEnt;                                        \
   _pst.dstInst = 0;                                             \
   _pst.srcInst = 0;                                             \
   _pst.event  = _event;                                         \
   _pst.pool   = _pool;                                         \
   _pst.selector= _selector;                                     \
}

#define SM_SET_ZERO(_buf, _size)   \
    cmMemset((U8 *)(_buf), 0, _size);

#define SM_FREE(_region, _pool, _buf, _size)          \
{                                                     \
   if (_buf != NULLP)                                 \
   {                                                  \
   (Void) SPutSBuf(_region, _pool, (Data *) _buf,     \
   (Size) _size);                                     \
   (_buf) = NULLP;                                    \
   }                                                  \
}

/* Fix for CR ccpu00140501 */

#define WR_GET_ENBIP_FROM_CELLID(enbIp, type) do{\
   if(type == CM_TPTADDR_IPV4)                   \
   {                                             \
      enbIp = smCfgCb.enbIpAddr[WR_IPV4_IDX];    \
   }                                             \
   else if(type == CM_TPTADDR_IPV6)              \
   {                                             \
      enbIp = smCfgCb.enbIpAddr[WR_IPV6_IDX];    \
   }                                             \
}while(0)

#define WRSM_ALLOC(_buf, _size)         \
{                                                     \
   if (SGetSBuf(smCb.init.region, DFLT_POOL, (Data **)(_buf),      \
                _size) == ROK)                        \
   {                                                  \
      SM_SET_ZERO((*_buf), _size);                    \
   }                                                  \
   else                                               \
   {                                                  \
      (*_buf) = NULLP;                                \
   }                                                  \
}


#define SM_DFLT_MEM_UPPER_THR_CFG   9
#define SM_DFLT_MEM_LOWER_THR_CFG   2
#define SM_DFLT_MSEC_TMR_CFG        10

#define SM_INST_ZERO   0
#define SM_ZERO_VAL    0

#define KWU_SAP_ID 0x00
#define CKW_SAP_ID 0x01
#define CRG_SAP_ID 0x02
#define CPJ_SAP_ID 0x03
#define PJU_SAP_ID 0x04
#define RGR_SAP_ID 0x05
#define RGU_SAP_ID 0x05
#define NHU_SAP_ID 0x06
#ifdef RM_INTF
#define RMU_SAP_ID 0x07
#endif /*RM_INTF*/


#define SM_SELECTOR_LC      0
#define SM_SELECTOR_TC      1
#define SM_SELECTOR_LWLC    2


#define WR_SZ_SPID                      0
#define WR_SZ_SUID                      0

#define WR_CZ_SPID                      0
#define WR_CZ_SUID                      0

#define WRSM_LSZ_MAX_SZTSAP   10
#define WRSM_LSZ_MAX_SCTSAP   10

#define WRSM_LCZ_MAX_CZTSAP   3
#define WRSM_LCZ_MAX_SCTSAP   3

#define WRSM_LSZ_TIMERES      10

#define WRSM_LCZ_TIMERES      10

#define WRSM_MSGPRIOR        PRIOR0    


/* CZ ---->SM posts */
#define WRSM_CZSMSEL           0
/* SM ---->CZ posts */
#define WRSM_SMCZSEL           0

/* SZ ---->SM posts */
#define WRSM_SZSMSEL           0
/* SM ---->SZ posts */
#define WRSM_SMSZSEL           0
 
/* YS ---->SM posts */
#define WRSM_YSSMSEL           0
/* SM ---->YS posts */
#define WRSM_SMYSSEL           0
 
/* SB ----> SM selector */
#define WRSM_SBSMSEL           0
/* SM ----> SB selector */
#define WRSM_SMSBSEL           0

#ifdef CL_MAC_LWLC
/* RG ----> YS selector */
#define WRSM_RGYSSEL           2
/* YS ----> RG selector */
#define WRSM_YSRGSEL           2
#else
/* RG ----> YS selector */
#define WRSM_RGYSSEL           1
/* YS ----> RG selector */
#define WRSM_YSRGSEL           1
#endif

/* KW ----> PJ Selector */
#define WRSM_KWPJSEL           2  /* Tightly coupled */

/* HI ----> SM selector */
#define WRSM_HISMSEL           0
/* SM ----> HI selector */
#define WRSM_SMHISEL           0


/* WR ----> SZ selector */
#define WRSM_WRSZSEL           2
/* SZ ----> WR selector */
#define WRSM_SZWRSEL           2


/* WR ----> CZ selector */
#define WRSM_WRCZSEL           2
/* CZ ----> WR selector */
#define WRSM_CZWRSEL           2

/* SZ ----> SB selector */
#define WRSM_SZSBSEL           0
/* SB ----> SZ selector */
#define WRSM_SBSZSEL           0

/* CZ ----> SB selector */
#define WRSM_CZSBSEL           0
/* SB ----> CZ selector */
#define WRSM_SBCZSEL           0

/* SB ----> HI selector */
#define WRSM_SBHISEL           0
/* HI ----> SB selector */
#define WRSM_HISBSEL           0

/* EG ----> HI selector */
#define WRSM_EGHISEL           1
/* HI ----> EG selector */
#define WRSM_HIEGSEL           0

/* WR ----> SM selector */
#define WRSM_WRSMSEL           1
/* SM ----> WR selector */
#define WRSM_SMWRSEL           1

/* WR ----> YS selector */
#define WRSM_WRYSSEL           0
/* YS ----> WR selector */
#define WRSM_YSWRSEL           0

/* WR ----> NH selector */
#define WRSM_WRNHSEL           0
/* YS ----> WR selector */
#define WRSM_NHWRSEL           0

/* WR ----> PJ selector */
#define WRSM_WRPJSEL           0
/* PJ ----> WR selector */
#define WRSM_PJWRSEL           0

/* WR ----> RG selector */
#define WRSM_WRRGSEL           0
/* RG ----> WR selector */
#define WRSM_RGWRSEL           0

/* WR ----> EG selector */
#define WRSM_WREGSEL           2
/* EG ----> WR selector */
#define WRSM_EGWRSEL           2

#define WRSM_LSZ_VAL_3        3
#define WRSM_LSZ_VAL_5        5
#define WRSM_LSZ_VAL_10       10 
#define WRSM_LSZ_VAL_50       50
#define WRSM_LCZ_VAL_3        3
#define WRSM_LCZ_VAL_10       10 
#define WRSM_LCZ_VAL_50       50

#ifndef CNE_UE_SIM_TEST
#define WRSM_ENB_IPADDR       0x0A010101
#define WRSM_MME_IPADDR       0x0A010102
#define WRSM_SCTP_IP_ADDR     0x0A010102
#else
#define WRSM_ENB_IPADDR       0xAC19005E
#define WRSM_MME_IPADDR       0xAC1A0006
#define WRSM_SCTP_IP_ADDR     0xAC19005E
#endif

#define WRSM_LSZ_ENB_PRC0     0

#define WRSM_LCZ_ENB_SCTPORT  36422
#define WRSM_LCZ_ENB_PRC0     0
#define WRSM_X2HO_PREP_MAX_TIME          3
#define WRSM_X2HO_OVRALL_MAX_TIME        20
#define WRSM_X2HO_TIME_TO_WAIT           20

#define WRSM_LSZ_MME_PRC1     1

#define WRSM_MAX_PROFILE      50
#define WRSM_LSZ_INV_NODE     10
#define WRSM_LSZ_RESETRETRY   10

/* Peer Cfg */
#define WRSM_LSZ_NUM_PEERS       1
#define WRSM_LSZ_OUT_STRMS       3


#define WRSM_LCZ_OUT_STRMS       3

/* Peer cntrl */
#define WRSM_LSZ_PEERID       0

#define WRSM_LSZ_MAX_PEER     WR_MAX_MMES


#define WRSM_LCZ_MAX_PEER     20

#define WRSM_HI_NUMBSAPS           10
#define WRSM_HI_NMBCONS            10

#ifdef HI_MULTI_THREADED
#define WRSM_HI_FDS                1024
#else
#define WRSM_HI_FDS                64
#endif /* HI_MULTI_THREADED */
#define WRSM_HI_FDBINS             4

#define WRSM_HI_SELTIMEOUT         2
#define WRSM_HI_UDP_MSGS_TOREAD    20
#define WRSM_HI_NUMCL_TOACCEPT     5

#define WRSM_HI_PERM_TSK_FLAG           FALSE
#define WRSM_HI_SCHDTMR_VAL             1
/* timer resolution unused */
#define WRSM_HI_TIME_RES                0
#define WRSM_HI_STRT_THRESH             1
#define WRSM_HI_DRP_THRESH              1
#define WRSM_HI_STP_THRESH              1

#define WRSM_HI_CONG_STRT                15000
#define WRSM_HI_CONG_DRP                 20000
#define WRSM_HI_CONG_STP                 10000
#define WRSM_HI_NMB_HLBINS               16


#define WRSM_LWR_VAL_10                 100 
#define WRSM_LWR_MAX_BND_RETRY           3

#define WRSM_CELLID                      1

#if defined (WR_RSYS_OAM) || (WR_DBG_CIRLOG) 
/* SM Timer */
/* Number of timer queue entries */
#define SM_NUM_TQENTRY                  5
/* Timer resolution */
#define SM_TMR_RES                      1
#define SM_CALC_WAIT_TIME(_delay) (_delay/((1000/SS_TICKS_SEC) * SM_TMR_RES))

#define SM_TMR_LOG_TMR                 1
#define SM_TMR_LOG_TMR_VAL             60000

EXTERN S16 smActvTmr(Ent ent, Inst inst);

#endif 
/* wr002.102: Added sm layer name and print buf */
#if (WR_SM_LOG_TO_FILE && DEBUGP)
#define SMLAYERNAME                      "EnodeB Sm"
#define SM_PRNT_BUF                      smCb.init.prntBuf
#else
#ifndef WR_DBG_CIRLOG
#define SM_PRNT_BUF                      WR_PRNT_BUF/*smCb.init.prntBuf*/
#else
/* Currently passing index as zero, when multiple system tasks are used then 
 * retrieve the corresponding circular buffer for that system task.
 */
#define SM_PRNT_BUF                      wrCb.init.prntCirBuf, MAX_LOG_BUF_SIZE 
#endif /* WR_DBG_CIRLOG */
#endif
  
#define SM_GETSBUF(_size, _datPtr, _ret)               \
{                                                      \
   _ret = SGetSBuf(smCb.init.region,           \
                   DFLT_POOL,             \
                   (Data**)&_datPtr, _size);           \
}

/* _logLevel applicable only when enabling the log otherwise even level 0 will
 * be disabled
 * _logLevel 0 will be represented by 1st bit in the _logMask and so on
 */
#define SM_SND_DIAG_CNTRL_REQ(_logLevel, _logState, _logMask, _lyr, _maskIdx)\
{\
   _logMask = 0x01;\
   if(_logState == TRUE)\
   {\
      for(_maskIdx = 0; _maskIdx <= _logLevel; _maskIdx++)\
      {\
         _logMask = (_logMask | (0x01 << _maskIdx));\
      }\
      smSendDiagCntrlReq(WR_ON, _lyr, logMask);\
   }\
   else\
   {\
      _logMask = 0x0;\
      smSendDiagCntrlReq(WR_OFF, _lyr, logMask);\
   }\
}

#define     EVTWRTUCLCFG       10
#define     EVTWRSCTPCFG       11
#define     EVTWRS1APCFG       12
#define     EVTWREGTPCFG       13
#define     EVTWRAPPCFG        14
#define     EVTWRRRCCFG        15
#define     EVTWRPDCPCFG       16
#define     EVTWRRLCCFG        17
#define     EVTWRMACCFG        18
#define     EVTWRCLCFG         19
#define     EVTWRX2APCFG       20
#ifdef RM_INTF
#define     EVTWRRRMCFG        21
#endif /*RM_INTF*/
#define     EVTWRSONCFG        22

#define     EVTWRS1APSTKBND    25
#define     EVTWREGTPSTKBND    26
#define     EVTWRX2APSTKBND    27
#define     EVTWRRRCSTKBND     28
#define     EVTWRENBDEBUGS     29
#define     EVTWRENBALRMS      30
#define     EVTWRINITS1SETUP   31
#define     EVTWRCELLCFG       32
#define     EVTWRINITX2SETUP   33
#define     EVTWRENBLOG        34
#define     EVTWRCLIRECV       36
#define     EVTWRSTARTDYNUPDATE  38
#ifdef RM_INTF
#define     EVTWRRRMSTKBND     39
#endif /*RM_INTF*/
#define     EVTOAMCMDRECV 35

#define WR_SMM_EUTRAN_FREQ_HASH_NO_BINS  32
#define WR_SMM_UTRAN_FREQ_HASH_NO_BINS  32
#define WR_SMM_UTRAN_TDD_FREQ_HASH_NO_BINS 16
#define WR_SMM_CDMA1X_CELL_HASH_NO_BINS  16
#define WR_SMM_NGH_ENB_HASH_NO_BINS      32
#define WR_SMM_EUTRAN_CELL_HASH_NO_BINS  32
#define WR_SMM_UTRAN_CELL_HASH_NO_BINS   32
#define WR_SMM_UTRAN_TDD_CELL_HASH_NO_BINS 32
#define WR_SMM_GERAN_FREQ_HASH_NO_BINS   16
#define WR_SMM_GERAN_CELL_HASH_NO_BINS   16

#define WR_SMM_MAX_PEERID_SUPPORTED 100

/** 
 * @brief enum to indicate new list contail old list also or not
 */
typedef enum{
WR_CFG_FOUND,
WR_CFG_DONE,
WR_CFG_ADD,
WR_CFG_MOD,
WR_CFG_DEL,
WR_CFG_INV
}SmDynCfgInd;

/** 
 * @brief Structure to maintain free peerId
 */

typedef struct _smFreeEnbPeerId
{
  U32   freePeerIdIdx;/*index of freeId list*/
  U32   peerId[WR_SMM_MAX_PEERID_SUPPORTED];
}SmFreeEnbPeerId;

/** 
 * @brief Structure to store PlmnId and CellId to get Hash Key
 */

typedef struct _smNghCellHasKey
{
 LwrPlmnId plmnId;
 U16       cellId; 
}smNghCellHasKey;

/** 
 * @brief Structure to store Neighbor enb and number of cells within the enb
 */

typedef struct _smNeigEnbCfg
{
  CmHashListEnt     nghEnbListEnt;
  LwrNeighAddModCfg nghEnbCfg;
  U32               numCell;/*number of cell within the enb*/
  SmDynCfgInd       dynCfgInd;
}SmNeigEnbList;

/** 
 * @brief Structure to store Neighbor Eutra freq List 
 */

typedef struct _smNeigEutraFreqCfg
{
  CmHashListEnt     nghEutraFreqListEnt;
  LwrNeighEutraFreq nghEutraFreqCfg;
  SmDynCfgInd       dynCfgInd;
}SmNeigEutraFreqList;

/** 
 * @brief Structure to store Neighbor Utra freq List 
 */

typedef struct _smNeigUtraTddFreqCfg
{
  CmHashListEnt     nghUtraTddFreqListEnt;
  LwrNeighUtraFreq  nghUtraTddFreqCfg;
  SmDynCfgInd       dynCfgInd;
}SmNeigUtraTddFreqList; 

/** 
 * @brief Structure to store Neighbor Utra freq List 
 */

typedef struct _smNeigUtraFreqCfg
{
  CmHashListEnt     nghUtraFreqListEnt;
  LwrNeighUtraFreq  nghUtraFreqCfg;
  SmDynCfgInd       dynCfgInd;
}SmNeigUtraFreqList;

/** 
 * @brief Structure to store Neighbor CDMA 1XRTT Freq List 
 */

typedef struct _smNeigCdma1xFreqCfg
{
  CmHashListEnt      nghCdma1xFreqListEnt;
  LwrNeighCdma1xFreq nghCdma1xFreqCfg;
  SmDynCfgInd        dynCfgInd;
}SmNeigCdma1xFreqList;  
 
/** 
 * @brief Structure to store Neighbor Geran freq List 
 */

typedef struct _smNeigGeranFreqCfg
{
  CmHashListEnt     nghGeranFreqListEnt;
  LwrNeighGERANFreq nghGeranFreqCfg;
}SmNeigGeranFreqList;
 
/** 
 * @brief Structure to store Neighbor CDMA 1XRTT band List 
 */
typedef struct _smNeigCdma1xBandCfg
{
  CmHashListEnt            nghCdma1xBandListEnt;
  LwrNeighCdmaBandClassCfg nghCdma1xBandCfg;
  SmDynCfgInd              dynCfgInd;
}SmNeigCdma1xBandList;  
/** 
 * @brief Structure to store Neighbor CDMA 1XRTT cell List 
 */
typedef struct _smNeigCdma1xCellCfg
{
  CmHashListEnt      nghCdma1xCellListEnt;
  LwrCDMA1xNeighCell nghCdma1xCellCfg;
  SmDynCfgInd        dynCfgInd;
}SmNeigCdma1xCellList; 
 
typedef struct _smSonNghCellCfg
{
   LwrEUTRANeighCell   *nghCellCfg;
   Bool                isPrachConfig;
   U16                 rootSeqIndex;
   U8                  zeroCorrelationZoneCfg;
   U8                  highSpeedFlag;
   U8                  prachCfgIdx;
   U8                  prachFreqOffset;
   U8                  numNghNgh;
   U16                 nghNghcellPci[LNL_MAX_NGH_NGH_CELL_INFO];
   U16                 lteBand;
   Bool                isCellBarred; 
}SmSonNghCellCfg;

/** 
 * @brief Structure to store Neighbor Eutra Cell List 
 */
typedef struct _smNeigEutraCellCfg
{
  CmHashListEnt     nghEutraCellListEnt;
  LwrEUTRANeighCell nghEutraCellCfg;
  smNghCellHasKey   hashKey;
  SmDynCfgInd       dynCfgInd;
  SmSonNghCellCfg   sonNghCellCfg;
}SmNeigEutraCellList;

/** 
 * @brief Structure to store Neighbor Utra Tdd Cell List 
 */

typedef struct _smNeigUtraTddCellCfg
{
  CmHashListEnt     nghUtraTddFreqListEnt;
  LwrUtraNeighCell  nghUtraTddCellCfg;
  smNghCellHasKey   hashKey;
  SmDynCfgInd       dynCfgInd;
}SmNeigUtraTddCellList;
 
/** 
 * @brief Structure to store Neighbor Utra Cell List 
 */

typedef struct _smNeigUtraCellCfg
{
  CmHashListEnt     nghEutraFreqListEnt;
  LwrUtraNeighCell  nghUtraCellCfg;
  smNghCellHasKey   hashKey;
  SmDynCfgInd       dynCfgInd;
}SmNeigUtraCellList;
 
/** 
 * @brief Structure to store Neighbor Geran Cell List 
 */
typedef struct _smNeigGeranCellCfg
{
  CmHashListEnt     nghGeranCellListEnt;
  LwrGERANNeighCell nghGeranCellCfg;
  smNghCellHasKey   hashKey;
}SmNeigGeranCellList; 

/*IPSec */
#define     EVTWRRRMSHUT        40
#define     EVTWRTUCLSHUT       41
#define     EVTWRSCTPSHUT       42
#define     EVTWRS1APSHUT       43
#define     EVTWREGTPSHUT       44
#define     EVTWRAPPSHUT        45
#define     EVTWRRRCSHUT        46
#define     EVTWRPDCPSHUT       47
#define     EVTWRRLCSHUT        48
#define     EVTWRMACSHUT        49
#define     EVTWRCLSHUT         50
#define     EVTWRX2APSHUT       51
#define     EVTWRMACSCHSHUT     52
#define     EVTWRSONSHUT        53
#define     EVTWRRRMSTKUNBND    54
#define     EVTWRRRCSTKUNBND    55
#define     EVTWRX2APSTKUNBND   56
#define     EVTWREGTPSTKUNBND   57
#define     EVTWRS1APSTKUNBND   58

/*IPSec */
typedef enum  {
    WR_SM_STATE_INIT = 0,
    WR_SM_STATE_TUCL_CFG_DONE,
    WR_SM_STATE_SCTP_CFG_DONE,
    WR_SM_STATE_S1AP_CFG_DONE,
    WR_SM_STATE_X2AP_CFG_DONE,
    WR_SM_STATE_EGTP_CFG_DONE,
    WR_SM_STATE_APP_CFG_DONE,
    WR_SM_STATE_RRC_CFG_DONE,
    WR_SM_STATE_PDCP_CFG_DONE,
    WR_SM_STATE_RLC_CFG_DONE,
    WR_SM_STATE_PDCP_UL_CFG_DONE,
    WR_SM_STATE_PDCP_DL_CFG_DONE,
    WR_SM_STATE_RLC_UL_CFG_DONE,
    WR_SM_STATE_RLC_DL_CFG_DONE,
    WR_SM_STATE_MAC_CFG_DONE,
    WR_SM_STATE_CL_CFG_DONE,
#ifdef RM_INTF
    WR_SM_STATE_RRM_CFG_DONE,
#endif /*RM_INTF*/
    WR_SM_STATE_SON_CFG_DONE,
    WR_SM_STATE_S1AP_STK_BND_DONE,
    WR_SM_STATE_EGTP_STK_BND_DONE,
    WR_SM_STATE_X2AP_STK_BND_DONE,
    WR_SM_STATE_RRC_STK_BND_DONE,
#ifdef RM_INTF
    WR_SM_STATE_RRM_STK_BND_DONE,
#endif
    WR_SM_STATE_ENB_DEBUGS_DONE,
    WR_SM_STATE_ENB_ALRMS_DONE,
    WR_SM_STATE_AWAIT_S1_CON,
    WR_SM_STATE_AWAIT_CELL_UP,
    WR_SM_STATE_DYN_X2AP_CFG_DONE,
    /* CNM_DEV */
    WR_SM_STATE_AWAIT_SYNC,
    WR_SM_STATE_CELL_UP,
    WR_SM_STATE_CELL_DOWN,
#ifdef WR_RSYS_OAM 
    WR_SM_STATE_OAM_DYN_START,
#endif
#ifdef E_TM
    WR_SM_STATE_CL_INIT,
#endif      
    WR_SM_STATE_ENB_LOG_DONE,
/* IPSec Support */
    WR_SM_STATE_REBOOT_REQUIRED,
    WR_SM_STATE_DINIT,
    WR_SM_STATE_TUCL_SHUTDWN_DONE,
    WR_SM_STATE_SCTP_SHUTDWN_DONE,
    WR_SM_STATE_S1AP_SHUTDWN_DONE,
    WR_SM_STATE_X2AP_SHUTDWN_DONE,
    WR_SM_STATE_EGTP_SHUTDWN_DONE,
    WR_SM_STATE_APP_SHUTDWN_DONE,
    WR_SM_STATE_RRM_SHUTDWN_DONE,
    WR_SM_STATE_RRC_SHUTDWN_DONE,
    WR_SM_STATE_SON_SHUTDWN_DONE,
    WR_SM_STATE_PDCP_DL_SHUTDWN_DONE,
    WR_SM_STATE_PDCP_UL_SHUTDWN_DONE,
    WR_SM_STATE_RLC_DL_SHUTDWN_DONE,
    WR_SM_STATE_RLC_UL_SHUTDWN_DONE,
    WR_SM_STATE_PDCP_SHUTDWN_DONE,
    WR_SM_STATE_RLC_SHUTDWN_DONE,
    WR_SM_STATE_MAC_SCH_SHUTDWN_DONE,
    WR_SM_STATE_MAC_SHUTDWN_DONE,
    WR_SM_STATE_CL_SHUTDWN_DONE,
    WR_SM_STATE_RRM_STK_UNBND_DONE,
    WR_SM_STATE_RRC_STK_UNBND_DONE,
    WR_SM_STATE_X2AP_STK_UNBND_DONE,
    WR_SM_STATE_EGTP_STK_UNBND_DONE,
    WR_SM_STATE_S1AP_STK_UNBND_DONE
}WrSmState;

typedef enum 
{
   WR_ON,
   WR_OFF
}WrCliSecArg;  

typedef enum 
{
   APP = 1,
   S1AP,
   SCTP,
   TUCL,
   X2AP,
   EGTP,
   RRC,
   RLC,
   PDCP,
   MAC,
   CL,
   CPLANE,
   ALL
}WrLyrDbg;     

#define WR_SM_HI_DBG_MASK 0x01
#define WR_SM_SB_DBG_MASK 0x02
#define WR_SM_SZ_DBG_MASK 0x04
#define WR_SM_EG_DBG_MASK 0x08
#define WR_SM_WR_DBG_MASK 0x10
#define WR_SM_NH_DBG_MASK 0x20
#define WR_SM_KW_DBG_MASK 0x40
#define WR_SM_RG_DBG_MASK 0x80
#define WR_SM_YS_DBG_MASK 0x100
#define WR_SM_CZ_DBG_MASK 0x200

#define MAXARGS        20
#define MAXCMDLEN      50
/* ccpu00126774  */
#define WR_PCCHCONFLOGIC_NB_MAX 7
#define WR_PCCHCONFLOGIC_NB_MIN 0
#define WR_PCCHCONFLOGIC_DEFAULTPAGINGCYCLE_MAX 3 
#define WR_PCCHCONFLOGIC_DEFAULTPAGINGCYCLE_MIN 0

#ifdef E_TM
#define ETM_CELL_STATE_IDLE  0
#define ETM_CELL_STATE_CFG   1
#define ETM_CELL_STATE_UP    2
#endif /* E_TM */
#define WR_MAX_LOG_PATH_VALUE  21
/* ccpu00131966 */
#define WR_MAX_MEAS_GAP_VALUE  2

EXTERN  S16 smSndWrLteAdvancedCellReCfg( WrEmmlteAdvFeatureActDeact *configInfo);
EXTERN S16 smBuildLteAdvConfigTrans(LwrLteAdvancedConfigTrans *lteAdvGroup, WrEmmlteAdvFeatureActDeact *configInfo);

   
/* Defines the memory regions for individual tasks. This needs to be changed
 * if the task creation part is changed. This is used while sending any
 * confirmation from layer to SM. This is filled in the request primitive
 * response structure */

#ifdef SS_LOCKLESS_MEMORY
#ifdef LTE_PAL_ENB
#define EG_MEM_REGION        1
#define HI_MEM_REGION        1
#define KW_UL_MEM_REGION     1
#define PJ_DL_MEM_REGION     1
#define PJ_UL_MEM_REGION     1
#define WR_DATAPP_MEM_REGION 1
#define SM_MEM_REGION        1
#define SZ_MEM_REGION        1
#define SB_MEM_REGION        1
#define CZ_MEM_REGION        1
#define NH_MEM_REGION        1
#define WR_MEM_REGION        1
#define RG_MEM_REGION        4
#define YS_MEM_REGION        4
#define KW_DL_MEM_REGION     4
#define HI_USAP_MEM_REGION   5

#else
#define SZ_MEM_REGION        0
#define CZ_MEM_REGION        0
#define SB_MEM_REGION        0
#define NH_MEM_REGION        0
#define SM_MEM_REGION        0
#define WR_MEM_REGION        0

#ifdef TUCL_TTI_RCV
#define HI_MEM_REGION        0

#define EG_MEM_REGION        1
#define PJ_DL_MEM_REGION     1
#define PJ_UL_MEM_REGION     1
#define KW_UL_MEM_REGION     1
#define WR_DATAPP_MEM_REGION 1
#define RM_REGION            1 

#define RG_MEM_REGION        2
#define YS_MEM_REGION        2
#define KW_DL_MEM_REGION     2

#define HI_USAP_MEM_REGION   3
#else
#define HI_MEM_REGION        1

#define EG_MEM_REGION        2
#define PJ_DL_MEM_REGION     2
#define PJ_UL_MEM_REGION     2
#define KW_UL_MEM_REGION     2
#define WR_DATAPP_MEM_REGION 2

#define RG_MEM_REGION        3
#define YS_MEM_REGION        3
#define KW_DL_MEM_REGION     3

#define HI_USAP_MEM_REGION   4
#endif

#endif

#else
#define SZ_MEM_REGION        0
#define CZ_MEM_REGION        0
#define SB_MEM_REGION        0
#define NH_MEM_REGION        0
#define SM_MEM_REGION        0
#define WR_MEM_REGION        0

#define HI_MEM_REGION        0

#define EG_MEM_REGION        0
#define PJ_DL_MEM_REGION     0
#define PJ_UL_MEM_REGION     0
#define KW_UL_MEM_REGION     0
#define WR_DATAPP_MEM_REGION 0

#define RG_MEM_REGION        0
#define YS_MEM_REGION        0
#define KW_DL_MEM_REGION     0

#define HI_USAP_MEM_REGION   0
#define RM_REGION            0 
#endif


/* Bandwidth values */
#define WR_FREQ_BANDWIDTH_5MHZ       5      /* 5MHZ */
#define WR_FREQ_BANDWIDTH_10MHZ      10     /* 10MHZ */
#define WR_FREQ_BANDWIDTH_15MHZ      15     /* 15MHZ */
#define WR_FREQ_BANDWIDTH_20MHZ      20     /* 20MHZ */
   
   
 /** 
 * @brief Structure to store log level and log state (enable/disable)
 */

typedef struct _smDiagInfo
{
   U32  diagLvl;   /*!< log level */
   Bool diagState; /*!< log state (enable/disable) */
}SmDiagInfo;

/**
 * @brief Structure to store log level and log state of all the layer
 */

typedef struct _smLyrDiagInfo
{
  SmDiagInfo tucl;
  SmDiagInfo sctp;
  SmDiagInfo s1ap;
  SmDiagInfo x2ap;
  SmDiagInfo egtp;
  SmDiagInfo app;
  SmDiagInfo lteRrc;
  SmDiagInfo lteRlc;
  SmDiagInfo ltePdcp;
  SmDiagInfo lteMac;
  SmDiagInfo lteCl;
}SmLyrDiagInfo;


typedef struct _smSonPrachCfg
{
   U16       servRootSeqIdx;
   U8        servPrachCfgIdx;
   U8        servZeroCorrelationZoneCfg;
   U8        servPrachFreqOffset;

   U8        numRootSeqIndex;
   U16       rootSeqIdxLst[LNL_MAX_ROOTSEQ_INDX];
   U8        numPrachCfgIdx;
   U8        prachCfgIdxLst[LNL_MAX_PRACH_CFG_IDX];
   U8        highSpeedFlag;
   U8        numZeroCorrCfgZone;
   U8        zeroCorrZoneCfgLst[LNL_MAX_ZERO_CORR_CFG_ZONE];
   U8        numPrachFreqOffset;
   U8        freqOffsetLst[LNL_MAX_NUM_FREQ_OFFSET];
}SmSonPrachCfg;

typedef struct _smCfgCb
{
   U16          cellId;           /* cell Id */
   U8           modType;          /* modulation type */
   U16          duplexMode;       /* Duplex Mode */
   U8           maxUeSupp;        /* max UE supported */
   U8           maxAvgGbrPrbUsage; /* max Average GBR PRB Usage in % */ /* RRM_SP1_START */
   TknU8        mcc[3];           /* mcc   */
   TknU8        mnc[3];           /* mnc  */
   U16          trackAreaCode;    /* Track area code */
   U8           freqBandInd;      /* Frequency Band Indicator */
   CmTptAddr    enbIpAddr[WR_MAX_IP_TYPES];      /* EnodeB IP address */
   CmTptAddr    sctpIpAddr[WR_MAX_IP_TYPES];     /* SCTP IP Address    */
   U8           numPci;                /* number of PCI present in PCI list*/
   U16          pciList[WR_SM_MAX_PCI]; /*PCI value in PCI list*/
   U8           numDlEarfcn;           /* number of DL Earfcn*/
   U16          dlEarfcnList[WR_SM_MAX_EARFCN]; /*DL Earfcn values*/
   U8           numUlEarfcn;           /* number of UL Earfcn*/
   U16          ulEarfcnList[WR_SM_MAX_EARFCN];/*UL Earfcn values*/
   SmSonPrachCfg sonPrachCfg; 
   U8           noOfCfg;         /* num MME configurations */
   LwrSmMmeCfg  *mmeCfg[LWR_SM_MAX_MMES];
   U8           hiDbg;            /* TUCL Debug  */
   U8           sbDbg;            /* SCTP Debug  */
   U8           szDbg;            /* S1AP Debug  */
   U8           egDbg;            /* EGTP Debug  */
   U8           wrDbg;            /* RRC APP Debug  */
   U8           nhDbg;            /* RRC Debug  */
   U8           kwDbg;            /* RLC/PDCP Debug  */
   U8           rgDbg;            /* MAC Debug  */
   U8           ysDbg;            /* CL Debug  */
   U8           smDbg;            /* SM Debug  */

   U32          smDbgMsk;

   U32          inactvTmrVal;
   U32          endMrkTmrVal;
   /* Modified max expires value from U8 to U32 */
   U32          maxExpires;
   U8           sctpUdpServiceType;
#ifdef LTE_HO_SUPPORT
   U8              czDbg;            /* X2AP Debug  */
   U32             x2PrepTimerVal;
   U32             x2OvrAllTimerVal;
   U32             x2TimeToWaitTimerVal;
   U16             pci;                        /* Physical Cell ID */
   U8              enbName[MAX_ENB_NAME_LEN];  /* eNB Name */
   U8              noOfBrdCstPlmn;             /* numbre of broadcasr plmns */
   U8              plmnId[LWR_SM_MAX_PLMN_IDS][3];
   U16             noOfNghInfo;                /* number of neighbor info */
   LwrNghInfoCfg   **nghInfo;                  /* neighbor info */
   LwrNghUtraFddCfg   *nghUtraFddInfo[MAX_NBR_UTRA_CELL];/* UTRA FDD neighbor cell info */
   LwrNghUtraTddCfg   *nghUtraTddInfo[MAX_NBR_UTRA_CELL];/* UTRA TDD neighbor cell info */
   U16             noOfEutraFreq;                /* number of neighbor info */
   LwrNghFreqCfg   *nghEutraFreq[MAX_NBR_EUTRA_FREQ];
   U16             noOfUtraFddFreq;                   /* number of UTRA FDD neighbor info */
   LwrNeighUtraFreq   *nghUtraFddFreq[MAX_NBR_UTRA_FREQ];/* UTRA FDD neighbor freq info */
   U16             noOfUtraTddFreq;                   /* number of UTRA TDD neighbor info */
   LwrNeighUtraFreq  *nghUtraTddFreq[MAX_NBR_UTRA_FREQ];/* UTRA TDD neighbor freq info */
   U16                   noOfGeranFreq; /* number of Geran Freqs */
   U16                   noOfGeranCell; /* number of Geran Cells */
   LwrGERANNeighCell     *geranCell[LWR_MAX_NUM_NEIGH_GERAN_CELLS];
   LwrNeighGERANFreq     *geranFreq[LWR_MAX_NUM_NEIGH_GERAN_FREQS];
   U8              noOfguGrp;                  /* number of pools configured */
   LwrGuGrpIdCfg   **guGrpId;                  /* Pool ID list */
   U32             s1PrepTimerVal;             /* Preparation Timer Value */
   U32             s1OvrAllTimerVal;           /* Overall Timer Value */
   U32             s1CancelTimerVal;           /* HO Cancel Timer Value*/ 
   U32             ueHoAttachTimerVal;         /* Time within which UE \
                                                  must detach from source\
                                                  cell and attach to \
                                                  target cell during \
                                                  Handover. */
   U32             maxX2Peers;                 /* MAX number of X2 Peers */
   U8              noOfNghCfg;                 /* number of neighbor cells configured */
   LwrNghCellCfg   *wrNghCellCfg[WR_SMM_MAX_NBR_ENB];             /* neighbor cell config info */
   /*Chinna:X2ap */
   TknU16          wrX2apPeerIdLst[WR_SMM_MAX_NBR_ENB];

   U8              suppMsrmntCfg;              /* enable meas config */
   U16             rrmRntiStart;               /* Start RNTI for RRM */
   U16             maxRrmRntis;                /* Max RNTIs managed by RRM */
   U8              rrmDedPrmStart;             /* initial rapId for RRM */
   U8              rrmNumDedPrm;               /* dedicated prmbls for RRM */
   U16             macRntiStart;               /* Start RNTI for MAC */
   U16             maxMacRntis;                /* Max RNTIs managed by MAC */
   U8              numOfMacPrm;               /* dedicated prmbls for MAC */
   U32             rsrpTrshldRange;           /* Serving Cell threshold 
                                                   RSRP value */
   U8              noOfnghENBs;                /* Number of neighbor eNBs */
#if 0
   WrNghENBCfg     **nghENBCfg;                /* List of neighbor eNB Configurations */ 
#endif
   LwrEnbType       eNBType;                    /* Type of eNB HOME/MACRO */

#endif 
  U8                       noOfCdma1xBndCls;
  LwrCdma1xrttBndClsCfg    *cdma1xBndClsCfg[WR_SMM_MAX_CDMA_1X_BAND_CLASS];
  U8                       noOfNeighCdma1xFreq;
  LwrCdma1xrttNeighFreqCfg *neighCdma1xFreq[WR_SMM_MAX_CDMA_1X_NEIGH_FREQ];
  U8                       noOfNeighCdma1xCells;
  LwrCdma1xrttNeighCellCfg *neighCdma1xCell[WR_SMM_MAX_CDMA_1X_NEIGH_CELL];

  LwrSctpParamCfg lwrSctpParamCfg;
  SmLyrDiagInfo diagInfo;     /*!< information required to enable/disable log */
#ifdef WR_RSYS_OAM
  CmHashListCp    neighEutraFreqLstCp;/*Hash List of neighbours Eutra frequency */
  CmHashListCp    neighUtraFreqLstCp; /*Hash List of neighbours Utra frequency */
  CmHashListCp    neighUtraTddFreqLstCp; /*Hash List of neighbours Utra frequency */
  CmHashListCp    neighGeranFreqLstCp;/*Hash List of neighbours Geran frequency */
  CmHashListCp    neighCdma1xBandLstCp; /*Hash List of neighbours CDMA 1XRTT Band */
  CmHashListCp    neighCdma1xCellLstCp; /*Hash List of neighbours CDMA 1XRTT cell */
  CmHashListCp    neighCdma1xFreqLstCp; /*Hash List of neighbours CDMA 1XRTT frequency */
  CmHashListCp    neighEnbLstCp;      /*Hash List of neighbours Enb */
  CmHashListCp    neighEutraCellLstCp;/*Hash List of neighbours Eutra Cell */
  CmHashListCp    neighUtraCellLstCp; /*Hash List of neighbours Utra Cell */
  CmHashListCp    neighUtraTddCellLstCp; /*Hash List of neighbours Utra Cell */
  CmHashListCp    neighGeranCellLstCp;/*Hash List of neighbours Geran Cell */
  SmFreeEnbPeerId freePeeIdList;/*free peerId list*/ 
  U16             freeMmeIdx;          /*number free mme Ids*/ 
  U16             freeMmeIdList[LWR_SM_MAX_MMES];/*free mme Ids*/
  U16             numMmeAdded;         /*number of mme added*/
  U16             addMmeList[LWR_SM_MAX_MMES];/*mme list*/
  U16             numMmeDel;   /*number of mme deleted*/
  U16             noMmeAddCfm;
  U16             noMmeDelCfm; 
#endif    
/* LTE_ADV starts */
  LwrLteAdvancedConfig        lteAdvancedConfig;
/* LTE_ADV ends */
  Bool          isDscpEnable;           /*Parameter to enable or disable DSCP*/
  U8            qciDscpMap[WR_MAX_QCI]; /*Map of QCI values to the corresponding DSCP values*/
#ifdef WR_DBG_CIRLOG
  Txt           logFilePath[512];    /* Log file directory path */
  U8            numLogFiles;         /* Number of log files */
  U32           maxSpaceforLogFiles; /*maximum space in system for log files*/
  U32           logFileSize;         /* Log file size in MBs(Mega Bytes) */
  CmTimer       logTimer;            /* Log timer */
#endif /* WR_DBG_CIRLOG */
#ifdef WR_RSYS_OAM 
  CmTimer       guardCfgTmr;         /* Guard (Static/Dynamic Cfg) Timer */
  struct{
  U32           alarmId;          /* alarm Id          */
  U8            severity;         /* alarm severity    */
  U8            causeType;        /* alarm Cause Type  */
  U8            causeVal;         /* alarm Cause Value */
  }alarm;
#endif /* WR_RSYS_OAM */
  U32           rimOvrlTmr;
  U32           rimRirTmr;
  U32           rimRiTmr;
  U32           rimRiaeTmr;
  U32           rimMaxRetryCnt;
  U16           remScanInterval;
  U16           remScanCount;
  Bool          sonTriggeredDynCfg; /* This needs to be removed once OAM
                                       changes come in */
  LnlTpmCfg	    tpmCfg;	    	      /*!< Rsys TPM Configurartion */
                                       
}SmCfgCb;

EXTERN SmCfgCb   smCfgCb;

typedef struct _smCb
{
   TskInit         init;       /* Task Init info */
#if defined (WR_RSYS_OAM) || (WR_DBG_CIRLOG) 
      CmTqCp          tqCp;       /* Timing Queue Control point    */
      CmTqType        tq[SM_NUM_TQENTRY]; /* Timing Queue for a resln. */
#endif 
   WrSmState       smState;    /* state for state machine */
   SmCfgCb         cfgCb;      /* Configurations read from the file */
   Bool            cellState;  /* Cell State*/
   Bool            reCfg;  /* Configuration already done or not */
   Bool            prem;       /* for periodic REM*/
#if (WR_SM_LOG_TO_FILE && DEBUGP)
   FILE*         dbgFp;                         /*!< Debug file pointer */
   Txt           filePath[WR_SM_MAX_FILE_PATH]; /*!< Path to store log files */
   U32           nmbDbgLines;                   /*!< Number of lines per debug file */
   U32           cfgDbgLines;                   /*!< Cfgd number of lines/Dbg file */
#endif /* (WR_SM_LOG_TO_FILE && DEBUGP) */
   U16           transIdMdfy;                    /*!< Lwr interfce Transaction Modifier */
   Bool          isPciMod;
   /* CNM_DEV */
   Bool            syncState;  /* CNM Sync State*/
   Bool            isPhyCfgd;
   U32 portNumber;
   U8 ipAddress[16];
   U8 rrcLogEnable;
   WrMemCpuMeasCb memCpuMeasCb;
}SmCb;

typedef struct _tstTkn
{
   TknStr    tok;
   U16       nmbArgs;
   TknStr    args[MAXARGS];
} TstTkn;

#ifdef PHY_ERROR_LOGING
/*typedef struct smPhyCmd
  {
  U32  phyCmd;
  }SmPhyCmd;
  */
typedef enum 
{
   PHY_CMD_START_TEST,
   PHY_CMD_STOP_TEST,
   PHY_CMD_ENABLE_LOG,
   PHY_CMD_DISABLE_LOG
}SmPhyCmd;
/* typedef struct _smUlAllocInfoCb SmUlAllocInfoCb; */
typedef struct _smUlAllocInfoCb
{
   U8  mcs;
   U16 numOfRb;
   U16 rbStart;
   Bool testStart;
   Bool enaLog;
   U16  logTime;
   U32  crcOk;
   U32  crcErr;
   U32  numUlPackets;
   U32  numPrach;
   U32  taZero;
}SmUlAllocInfoCb;

EXTERN  SmUlAllocInfoCb smUlAllocInfoCb;

EXTERN Void wrSmSendUlAllocCfgReq (SmPhyCmd phyCmd);
EXTERN Void wrSmRgSchUlAllocCfg(SmPhyCmd phyCmd);
#endif /* PHY_ERROR_LOGING */

EXTERN SmCb   smCb;

#if (WR_SM_LOG_TO_FILE && DEBUGP)
EXTERN Void wrSmCreateDbgFile(Void);
EXTERN S16 wrGetSId (SystemId *s);
#endif

EXTERN S16 SRegInfoShow  (Region region, U32 *availmem);
EXTERN S16 smWrReadConfigParams (Void);
/* CSG_DEV */
EXTERN S16 smWrValidateConfigParams (Void);
EXTERN Void smWrCfgGetTagNum (U8* tag, U16* tagNum);
EXTERN Void wrCfgValdtInput (U8* input, Bool* status, U8 *noOfElem);
EXTERN Void wrCfgGetParamVal (U8* tagVal, U8 paramLst[][50], U16* noOfParams);
EXTERN S32 wrUpdateIpAddr(U8 *strIpAddr, CmTptAddr *binIpAddr);

/* Prototypes of Init functions */
EXTERN S16 smWrInit (SSTskId sysTskId);
EXTERN S16 smSzInit (SSTskId sysTskId);
EXTERN S16 smRmInit (SSTskId sysTskId);

#ifdef LTE_HO_SUPPORT
EXTERN S16 smCzInit (SSTskId sysTskId);
#endif
EXTERN S16 smEgInit (SSTskId sysTskId);
EXTERN S16 smEuInit (SSTskId sysTskId);
EXTERN S16 smSbInit (SSTskId sysTskId);
EXTERN S16 smHiInit (SSTskId sysTskId);
EXTERN S16 smNhInit (SSTskId sysTskId);
EXTERN S16 smPjInit (SSTskId sysTskId);
EXTERN S16 smRgInit (SSTskId sysTskId);
EXTERN S16 smKwInit (SSTskId sysTskId);
EXTERN S16 smYsInit (SSTskId sysTskId);

EXTERN S16 smKwUlInit (SSTskId sysTskId);
EXTERN S16 smKwDlInit (SSTskId sysTskId);
EXTERN S16 smPjDlInit (SSTskId sysTskId);
EXTERN S16 smPjUlInit (SSTskId sysTskId);
EXTERN S16 smNlInit (SSTskId sysTskId);
EXTERN S16 smNlActvTsk (Pst *pst, Buffer *mBuf);
EXTERN S16 smNlActvInit(Ent entity, Inst inst, Region region, Reason reason);

/* Prototypes of state machine functions */
/*IPSec*/
EXTERN Void smWrShutdownSm     (U8 event);
EXTERN Void smWrProcSm          (U8 event);
#ifdef E_TM
PUBLIC S16 wrSmYsEtmCfg         (Void); 
EXTERN Void smWrEtmProcSm       (U8 event);
EXTERN S16 smBuildYsEtmCntrl    (Action action); 
#endif
EXTERN S16 smBldYsSndLogStrmInfoReq (U8 logStrmType);
EXTERN S16  wrSmSelfPstMsg      (Pst *pst);
EXTERN Void wrSmHdlTuclCfgEvent (U8 event);
EXTERN Void wrSmHdlSctpCfgEvent (U8 event);
EXTERN Void wrSmHdlS1apCfgEvent (U8 event);
EXTERN Void wrSmHdlEgtpCfgEvent (U8 event);
EXTERN Void wrSmHdlAppCfgEvent  (U8 event);
EXTERN Void wrSmHdlRrcCfgEvent  (U8 event);
EXTERN Void wrSmHdlPdcpUlCfgEvent (U8 event);
EXTERN Void wrSmHdlPdcpDlCfgEvent (U8 event);
EXTERN Void wrSmHdlRlcUlCfgEvent  (U8 event);
EXTERN Void wrSmHdlRlcDlCfgEvent  (U8 event);
EXTERN Void wrSmHdlMacCfgEvent  (U8 event);
EXTERN Void wrSmHdlCLCfgEvent   (U8 event);
EXTERN Void wrSmHdlBndS1apStack (U8 event);
EXTERN Void wrSmHdlBndEgtpStack (U8 event);
EXTERN Void wrSmHdlBndRrcStack  (U8 event);
#ifdef RM_INTF
EXTERN Void wrSmHdlBndRrmStack  (U8 event);
EXTERN Void wrSmHdlRrmCfgEvent  (U8 event);
EXTERN Void wrSmSndRrmCellUpInd (U8 event);
#endif

EXTERN Void wrSmHdlSonCfgEvent  (U8 event);
EXTERN Void wrSmSndSonCellUpInd (U8 event);
EXTERN Void wrSmNlCfg (Void);
EXTERN Void smSendCellUpIndToSon(Void);
EXTERN S16 smSndWrPciUpdateReq(NlUpdatePciCfgInd   *pciUpdate);
EXTERN S16 smSndWrSchdPrbPaUpdateReq(LnlSonCfg   *sonCfg);

EXTERN Void wrSmHdlEnbAlarms    (U8 event);
EXTERN Void wrSmHdlEnbDebugs    (U8 event);
EXTERN Void wrSmHdlInitS1Setup  (U8 event);
EXTERN Void wrSmHdlInitCellCfg  (U8 event);
EXTERN Void wrSmHdlEnbLog       (U8 event);
EXTERN Void wrSmConfigComplete  (Void);
EXTERN Void smConfigFailure   (Void);

#ifdef LTE_HO_SUPPORT
EXTERN Void wrSmHdlX2apCfgEvent (U8 event);
EXTERN Void wrSmHdlBndX2apStack (U8 event);
EXTERN Void wrSmHdlInitX2Setup  (U8 event);
#endif

/* Actv Task  and Actv Init*/
EXTERN S16 nhActvTsk (Pst *, Buffer *);
EXTERN S16 nhActvInit (Ent, Inst, Region, Reason);

EXTERN S16 nlActvTsk (Pst *, Buffer *);
EXTERN S16 nlActvInit (Ent, Inst, Region, Reason);

EXTERN S16 kwUlActvTsk (Pst *, Buffer *);
EXTERN S16 kwUlActvInit (Ent, Inst, Region, Reason);
EXTERN S16 kwDlActvTsk (Pst *, Buffer *);
EXTERN S16 kwDlActvInit (Ent, Inst, Region, Reason);
EXTERN S16 rgActvTsk (Pst *, Buffer *);
EXTERN S16 rgActvInit (Ent, Inst, Region, Reason);
EXTERN S16 schActvTsk (Pst *, Buffer *);
EXTERN S16 schActvInit (Ent, Inst, Region, Reason);
EXTERN S16 egActvTsk (Pst *, Buffer *);
EXTERN S16 egActvInit (Ent, Inst, Region, Reason);
EXTERN S16 pjUlActvTsk (Pst *, Buffer *);
EXTERN S16 pjUlActvInit (Ent, Inst, Region, Reason);
EXTERN S16 pjDlActvTsk (Pst *, Buffer *);
EXTERN S16 pjDlActvInit (Ent, Inst, Region, Reason);

EXTERN S16 ysActvTsk (Pst *, Buffer *);
EXTERN S16 ysActvInit (Ent, Inst, Region, Reason);
EXTERN S16 rmActvTsk (Pst *, Buffer *);
EXTERN S16 rmActvInit (Ent, Inst, Region, Reason);
#ifdef LTE_ENB_PAL
EXTERN S16 tfActvTsk (Pst *, Buffer *);
EXTERN S16 tfActvInit (Ent, Inst, Region, Reason);
#else
#endif /* LTE_ENB_PAL */

EXTERN S16 smEgActvTsk (Pst *, Buffer *);
EXTERN S16 smRgActvTsk (Pst *, Buffer *);
EXTERN S16 smNhActvTsk (Pst *, Buffer *);
EXTERN S16 smKwActvTsk (Pst *, Buffer *);
EXTERN S16 smPjActvTsk (Pst *, Buffer *);
EXTERN S16 smHiActvTsk (Pst *, Buffer *);
EXTERN S16 smSbActvTsk (Pst *, Buffer *);
EXTERN S16 smSzActvTsk (Pst *, Buffer *);

#ifdef LTE_HO_SUPPORT
EXTERN S16 smSzActvTsk (Pst *, Buffer *);
#endif

/* Config functions */
EXTERN Void wrSmHiCfg (Void);
EXTERN Void wrSmSbCfg (Void);
EXTERN Void wrSmSzCfg (Void);
EXTERN Void wrSmEgCfg (Void);
EXTERN Void wrSmWrCfg (Void);
#ifdef RM_INTF
EXTERN Void wrSmRmCfg (Void);
#endif /*RM_INTF*/
EXTERN Void wrSmNhCfg (Void);
EXTERN Void wrSmPjUlCfg (Void);
EXTERN Void wrSmPjDlCfg (Void);
EXTERN Void wrSmKwUlCfg (Void);
EXTERN Void wrSmKwDlCfg (Void);
EXTERN Void wrSmRgCfg (Void);
EXTERN Void wrSmYsCfg (Void);
#ifdef E_TM
EXTERN Void wrSmYsCellCfg (Void);
#endif

#ifdef LTE_HO_SUPPORT
EXTERN Void wrSmCzCfg (Void);
#endif


/* HI(TUCL) Build Functions */
EXTERN Void smBuildHiGenCfg     (Void);
EXTERN Void smBuildHiTSapCfg    (SpId spId);
EXTERN Void smBuildHiDbgCntrl   (Void);
EXTERN Void smBuildHiLogCntrl   (Bool actType, U32 mask);

/* SB(SCTP) Build Functions */
EXTERN Void smBuildSbGenCfg     (Void);
EXTERN Void smBuildSbTSapCfg    (SpId spId, SuId suId);
EXTERN Void smBuildSbSctSapCfg  (SpId spId);
EXTERN Void smBindUnBindSbToLSap(SpId spId,U8 action);
EXTERN Void smBuildSbDbgCntrl   (Void);
EXTERN Void smBuildSbLogCntrl   (Bool actType, U32 mask);

/* SZ(S1AP) Build Functions */
EXTERN S16 smBuildSzGenCfg     (Void);
EXTERN S16 smBuildSzLSapCfg    (SpId spId, SuId suId);
EXTERN S16 smBuildSzUSapCfg    (SpId spId);
EXTERN S16 smBuildSzProtCfg    (Void);
EXTERN S16 smBuildSzPeerCfg    (Void);
EXTERN S16 smBuildDynSzPeerCfg (U8 mmeIndex, U8 dbIndex);
#ifdef WR_RSYS_OAM
EXTERN S16 smBuildDynCzPeerCfg (LwrNeighAddModCfg *nghEnbCfg);
#else
EXTERN S16 smBuildDynCzPeerCfg (U32 nbrIndex,U8 dbIndex);
#endif
EXTERN S16 smBldSzPeerDelCntrlReq(U8 peerId, U8 dbIndex);
EXTERN S16 smBldCzPeerDelCntrlReq(U8 peerId, U8 dbIndex);
EXTERN S16 smBindUnBindSzToLSap  (U8 action);
EXTERN S16 smBuildSzGenCntrl   (Void);
EXTERN S16 smBuildSzDbgCntrl   (Void);
EXTERN S16 smBuildSzLogCntrl   (Bool actType, U32 mask);
EXTERN S16 SmMiLwrUpDateNhgCellCfgReq(Pst *pst,LwrMngmt *nghCellCfgReq);


#ifdef LTE_HO_SUPPORT
/* CZ(X2AP) Build Functions */
EXTERN S16 smBuildCzGenCfg     (Void);
EXTERN S16 smBuildCzGenCntrl   (Void);
EXTERN S16 smBuildCzProtCfg    (Void);
EXTERN S16 smBuildCzLSapCfg    (SuId suId, SpId spId);
EXTERN S16 smBuildCzUSapCfg    (SpId spId);
EXTERN S16 smBuildCzPeerCfg    (Void);
EXTERN S16 smBuildCzUstaCntrl    (Void);
EXTERN S16 smBuildCzDbgCntrl    (Void);
EXTERN S16 smBuildCzLogCntrl    (Bool actType, U32 mask);
EXTERN S16 smBuildCzShutDownCntrl    (Void);
EXTERN S16 smBuildCzLSapTrcCntrl    (U16 action);
EXTERN S16 smBuildCzPeerTrcCntrl  (Void);
EXTERN S16 smBuildCzEndPtCntrl    (Void);
EXTERN S16 smBuildCzAssocCntrl    (Void);
EXTERN S16 smBuildCzGenStsReq    (Void);
EXTERN S16 smBuildCzPeerStsReq    (Void);
EXTERN S16 smBuildCzGenStaReq    (Void);
EXTERN S16 smBuildCzSidStaReq    (Void);
EXTERN S16 smBuildCzUSapStaReq    (Void);
EXTERN S16 smBuildCzLSapStaReq    (Void);
EXTERN S16 smBuildCzPeerStaReq    (Void);
EXTERN S16 smBindUnBindCzToLSap    (U8 action);
EXTERN S16 smUbindCzFrmUSap    (Void);
EXTERN S16 smDelCzUSap    (Void);
EXTERN S16 smUbindCzFrmLSap    (Void);
EXTERN S16 smDelCzLSap    (Void);
EXTERN S16 smDelCzPeer    (Void);
EXTERN S16 smBldrCzPeerDelCntrlReq (U32 peerId, U32 dbIndex);
EXTERN S16 smBuildAddNeighCfg (LwrMngmt *lwrMngmt);
#endif

/* EG(EGTP) Build Functions */
EXTERN S16  smBuildEgGenCfg     (Void);
EXTERN S16  smBuildEgGenCntrl    (Void);
EXTERN S16  smBuildEgUSapCfg    (SpId spId);
EXTERN S16  smBuildEgLSapCfg    (SpId spId, SuId suId);
/* Shwetha - adding IP Address configuration for EGTP-U */
EXTERN S16  smBuildEgIpAddrCfg  (SpId  spId);
EXTERN Void smBindUnBindEgToHitSap  (SpId spId,U8 action);
EXTERN Void  smBuildEgDbgCntrl   (Void);
EXTERN Void  smBuildEgLogCntrl   (Bool actType, U32 mask);

/* YS(CL) Build Functions */
EXTERN S16  smBuildYsGenCfg     (Void);
EXTERN S16  smBuildYsTfuSapCfg  (SpId spId, SuId suId);
EXTERN S16  smBuildYsSchTfuSapCfg  (SpId spId, SuId suId);
EXTERN S16  smBuildYsCtfSapCfg  (SpId spId, SuId suId);
EXTERN S16  smBuildYsGenCntrl   (Void);
EXTERN S16  smBuildYsDbgCntrl   (Void);
EXTERN S16  smBuildYsLogCntrl   (Bool actType, U32 mask);
EXTERN S16  smYsSendRlogInd     (U8 rlLogLvl);
#ifdef E_TM
EXTERN  S16 smBuildYsCellCfg    (Void);
#endif


/* WR(APP) Build Functions */
EXTERN S16 smBuildWrGenCfg     (Void);
EXTERN S16 smBuildWrCtfLSapCfg (SpId spId, SuId suId);
EXTERN S16 smBuildWrNhuLSapCfg (SpId spId, SuId suId);
EXTERN S16 smBuildWrRgrLSapCfg (SpId spId, SuId suId);
EXTERN S16 smBuildWrSztLSapCfg (SpId spId, SuId suId);

EXTERN S16 smBuildWrCztLSapCfg (SpId spId, SuId suId);
EXTERN S16 smBindUnBindWrToCztSap (U8 action);
EXTERN S16 smBuildCdmaHrpdaBandClass (LwrMngmt *lwrMng);
EXTERN S16 smBuildCdma1xRTTBandClass (LwrMngmt *lwrMng);
EXTERN S16 smBuildWrNehUtraFddCellCfg (LwrMngmt *lwrMng);
EXTERN S16 smBuildWrNehUtraTddCellCfg (LwrMngmt *lwrMng);
EXTERN S16 smBuildWrNrCdmaHrpdFreqCfg(LwrMngmt *lwrMng);


EXTERN S16 smBuildWrNehCellCfg (LwrMngmt *lwrMng);
EXTERN S16 smBuildWrInterFreqNehCellCfg (LwrMngmt *lwrMng);
EXTERN S16 smBuildWrNghCfg (LwrMngmt *lwrMng);
EXTERN S16 smBuildCellCfg (LwrMngmt *lwrMng);
EXTERN S16 smBuildModCellCfg (LwrMngmt *lwrMng);
EXTERN S16 smBuildWrMmeCfg (LwrMngmt *lwrMng, LwrSmMmeCfg *mmeCfg);
EXTERN Void smGetS1apStats(Void);
EXTERN S16 smSndRmuProtDynMmeCfg(U16 mmeId, U32 action);
EXTERN S16 smSndRmuProtModCellCfg(U8 cellId, LrmCellConfiguration modCellCfg);
EXTERN S16 wrSendDynMmeDelcfg( U32 mmeIdx ); 
EXTERN S16 smBuildWrNrUtraFddFreqCfg(LwrMngmt   *lwrMng);
EXTERN S16 smBuildWrNrUtraTddFreqCfg(LwrMngmt   *lwrMng);
EXTERN S16 smBuildWrNrGeranFreqCfg(LwrMngmt *lwrMng);
EXTERN S16 smBuildWrNrGeranCellCfg(LwrMngmt *lwrMng);
EXTERN S16 smBuildWrNrEutraFreqCfg(LwrMngmt *lwrMng);
EXTERN S16 smBuildWrEnbCfg (LwrMngmt *lwrMng);
EXTERN Void smBldCellAnrCfg (LwrMngmt   *lwrMng, U8 cfgGrpIndex);
EXTERN Void smBldCellSibSchCfg (LwrMngmt   *lwrMng, U8         cfgGrpIndex);
EXTERN Void smBldCellEaidCfg(LwrMngmt   *lwrMng, U8         cfgGrpIndex);
EXTERN S16 smFillX2InitCntrl(LwrMngmt *cntrl);

EXTERN S16 smBuildDynMmeAddCfg(U32 mmeIndex);
EXTERN S16 smBuildDynMmeDelCfg(U32 mmeIndex);
EXTERN S16 MsmEnodeBinitialCfgComplete(Void);
EXTERN S16 smStartStopStack(U8 isPremReq,U8 isStkSt,U8 isImd);
EXTERN S16 smSndWrModCellCfg(Void);
EXTERN S16 smBuildWrErabRelIndCntrl(Void); 
EXTERN S16 wrSmDynInitS1Setup(Void);
EXTERN S16 smBuildWrX2InitCntrl(Void);
EXTERN S16  wrSmDynInitX2Setup(Void);
EXTERN S16 smBuildWrPartRstCntrl(Void);
EXTERN S16 smBuildWrFullRstCntrl(Void);
EXTERN S16 smBuildNghRstCntrl(Void);
EXTERN S16 smBuildAnrPrintCntrl(Void);
EXTERN S16 smSndWrCellReCfgSib2(U8 ovldFlag);
EXTERN S16 smSndLoadInd(U16 choice,U16 value);
EXTERN S16 smSndDynWrNhCellCfg(Void);
#if 0
EXTERN S16 smBuildWrNghCfg     (Void);
EXTERN S16 smBuildWrMmeCfg     (Void);
#endif

#ifdef RM_INTF
/*RRM->FSM */
EXTERN S16 smBuildWrRmuLSapCfg (SpId spId, SuId suId);
EXTERN S16 smBindUnBindWrToRmuSap(U8 action);
EXTERN S16 smBuildRmRgmLsapCfg (Void);
EXTERN S16 smBindUnBindRmToRgmSap    (U8 action);
EXTERN S16 smSendCellUpIndToRrm (Void);
EXTERN S16 smSendCpuLoadToRrm(U8 cpuload);
#endif /*RM_INTF*/

#ifdef EU_DAT_APP
EXTERN S16 smBuildWrEgtLSapCfg (SpId spId, SuId suId);
EXTERN S16 smBuildWrPjuLSapCfg (SpId spId, SuId suId);
EXTERN S16 smBindUnBindWrToEgtSap(U8 action);
EXTERN S16 smBindUnBindWrToPjuSap    (Inst inst,U8 action);
#endif
EXTERN S16 smBuildPjLogCntrl   (Bool actType, U32 mask);

EXTERN S16 smBindUnBindWrToSztSap    (U8 action);
EXTERN S16 smBindUnBindWrToNhuSap    (U8 action);
EXTERN S16 smBindUnBindWrToCtfSap    (U8 action);
EXTERN S16 smBindUnBindWrToRgrSap    (U8 action);
EXTERN S16 smBuildWrGenCntrl   (Void);
EXTERN S16 smBuildWrS1InitCntrl   (Void);
EXTERN S16 smBuildWrCellCntrl   (Void);
/* CNM_DEV */
EXTERN S16 smBuildWrSyncCntrl   (Void);
EXTERN S16 smBuildWrDbgCntrl   (Void);
EXTERN S16 smBuildWrLogCntrl   (Bool actType, U32 mask);
EXTERN S16 smBuildRmuProtCellDel(LrmCellConfiguration *cellCfg);

/*IPSec support */

EXTERN S16 smBuildRgSCHShutDownCtrl (Void);
EXTERN S16 smBuildRmShutDownCntrl   (Void);
EXTERN S16 smBuildNlShutDownCntrl   (Void);
EXTERN S16 smBuildWrshutDownCntrl   (Void);
EXTERN S16 smBuildNhShutDownCntrl   (Void);
EXTERN S16 smBuildEgShutDownCntrl   (Void);
EXTERN S16 smBuildPjShutDownCntrl   (U8 inst);
EXTERN S16 smBuildKwShutDownCntrl   (Inst inst);
EXTERN S16 smBuildRgShutDownCntrl   (Void);
EXTERN S16 smBuildCzShutDownCntrl   (Void);
EXTERN S16 smBuildSzShutDownCntrl   (Void);
EXTERN S16 smBuildYsShutDownCntrl   (Void);
EXTERN S16 smBuildSbShutDownCntrl   (Void);
EXTERN S16 smBuildWrShutDownCntrl   (Void);
EXTERN Void smBuildHiShutDownCntrl  (Void);
/* NH(LTE RRC) Build Functions */
EXTERN S16 smBuildNhGenCfg     (Void);
EXTERN S16 smBuildNhGenCntrl   (Void);
EXTERN S16 smBuildNhProtCfg    (Void);
EXTERN S16 smBuildNhUsapCfg    (Void);
EXTERN S16 smBuildNhKwuLsapCfg (Inst inst);
EXTERN S16 smBuildNhCkwLsapCfg (Void);
EXTERN S16 smBuildNhCrgLsapCfg (Void);
EXTERN S16 smBuildNhCpjLsapCfg (Void);
EXTERN S16 smBuildNhPjuLsapCfg (Inst inst);
EXTERN S16 smBuildNhLsapCfg    (U8 elemId,Inst inst);
EXTERN S16 smBindUnBindNhToKwuSap    (Inst inst,U8 action);
EXTERN S16 smBindUnBindNhToCkwSap    (U8 action);
EXTERN S16 smBindUnBindNhToCrgSap    (U8 action);
EXTERN S16 smBindUnBindNhToCpjSap    (U8 action);
EXTERN S16 smBindUnBindNhToPjuSap    (Inst inst,U8 action);
EXTERN S16 smBindUnBindNhToLsap      (U8 elemId,U8 action);
EXTERN S16 smBuildNhGenCntrl   (Void);
EXTERN S16 smBuildNhDbgCntrl   (Void);
EXTERN S16 smBuildNhLogCntrl   (Bool actType, U32 mask);

/* PJ(LTE PDCP) Build Functions */
EXTERN S16 smBuildPjGenCfg     (Inst  inst);
EXTERN S16 smBuildPjCpjSapCfg  (Inst inst);
EXTERN S16 smBuildPjPjuSapCfg  (SpId spId, Inst inst);

/* KW(LTE RLC) Build Functions */
EXTERN S16 smBuildKwGenCfg     (Inst inst);
EXTERN S16 smBuildKwUsapCfg    (U8 elemId, U8 ent,Inst inst);
EXTERN S16 smBuildCkwUsapCfg   (Inst  inst);
EXTERN S16 smBuildKwuUsapCfg   (U8 ent,Inst inst);
EXTERN S16 smBuildKwRguLsapCfg (Void);
EXTERN S16 smBuildKwLsapCfg    (U8 ent,Inst inst);
EXTERN S16 smBindUnBindKwToRguSap    (Inst,U8 action);
EXTERN S16 smBuildKwDbgCntrl   (Void);
EXTERN S16 smBuildKwLogCntrl   (Bool actType, U32 mask);

EXTERN S16 smBuildUdxUsapCfg  (Inst  inst);
EXTERN S16 smBindUnBindKwUdxSap  (Inst  inst,U8 action);
EXTERN S16 smBindUnBindPJToKwuSap  (Inst  inst,U8 action);
EXTERN S16 smBindUnBindPjUdxSap  (Inst  inst,U8 action);
EXTERN S16 smBindUnBindPjKwuSap  (Inst  inst,U8 action);
EXTERN S16 smBuildPjKwuSapCfg  (Inst  inst);
EXTERN S16 smBuildPjUdxSapCfg  (Inst  inst);

/* RG(LTE MAC) Build Functions */
EXTERN S16 smBuildRgGenCfg     (Void);
EXTERN S16 smBuildRgUsapCfg    (U8 elemId,SpId sapId);
EXTERN S16 smBuildCrgUsapCfg   (Void);
EXTERN S16 smBuildRguUsapCfg   (SpId sapId);
EXTERN S16 smBuildRgSchInsCfg  (Void);
EXTERN S16 smBuildRgStsReq     (Void);
EXTERN S16 smBuildRgTfuLsapCfg (Void);
EXTERN S16 smBindUnBindRgToTfuSap    (U8 action);
EXTERN S16 smBindUnBindRgSchToTfuSap (U8 action);
EXTERN S16 smBuildRgGenCntrl   (Void);
EXTERN S16 smBuildRgDbgCntrl   (Void);
EXTERN S16 smBuildSchdDbgCntrl   (Void);
EXTERN S16 smBuildRgLogCntrl   (Bool actType, U32 mask);
EXTERN S16 smCreateLogThr      (Void);

EXTERN S16 smSndWrCellModSib2       (Void);
EXTERN S16 smSndWrCellModSib3       (Void);
EXTERN S16 smSndWrCellModSib4       (Void);
EXTERN S16 smSndWrCellModSib5       (Void);
EXTERN S16 smSndWrCellModSib6       (Void);
EXTERN S16 smSndWrCellModSib9       (Void);
EXTERN S16 smSndWrModCellCfg        (Void);
EXTERN S16 smDynWrNrEutraFreqCfg    (U8 type);
EXTERN S16 smDynWrNrUtraFreqCfg     (U8 type);
EXTERN S16 smModBuildWrNehCellCfg   (LwrMngmt *lwrMng);
EXTERN S16 smBuildDynWrNehCellDel   (LwrMngmt *lwrMng);
EXTERN S16 smBuildDynWrNehCellCfg   (LwrMngmt *lwrMng);
EXTERN S16 smModWrNhCellCfg         (Void);
EXTERN S16 smSndDynWrNhCellDel      (Void);
EXTERN S16 smSndDynWrNhCellCfg      (Void);
EXTERN S16 wrSmDynInitX2Setup       (Void);
EXTERN S16 wrSmDynInitS1Setup       (Void);
EXTERN S16 smBuildAnrPrintCntrl     (Void);
EXTERN S16 smBuildWrFullRstCntrl    (Void);
EXTERN S16 smBuildWrPartRstCntrl    (Void);
EXTERN S16 smBuildNghRstCntrl       (Void);
EXTERN S16 smBuildWrX2InitCntrl     (Void);
EXTERN S16 smBuildWrErabRelIndCntrl (Void);
EXTERN S16 smSndLoadInd             (U16 choice, U16 value);
EXTERN S16 smSndWrCellReCfgSib2     (U8 ovldFlag);
EXTERN S16 wrSmmSendDynNbrEnbCfg    (U32 dbIndex);
EXTERN S16 smBuildDynWrNbrEnbCfg    (U32 dbIndex, LwrMngmt *lwrMng);
EXTERN U8 smGetEnbType(U16 phyCellId);

#ifdef WR_RSYS_OAM
EXTERN S16 wrSmDynAddMme(Void);
EXTERN Void smInitFreePeerIdList(Void);
EXTERN U8 smGetIsX2Enabl(LwrEnbType enbType);
EXTERN U8 smGetPeerId(U32 enbId);
EXTERN Void smReturnPeerIdToFreeList(U8 peerId);
EXTERN Void smFillNghUtraFreqParam(LwrNeighUtraFreq *lwrNghEutraCell,\
                         MsmLteNeighUtraFreq *msmNghEutraCell, U32 reCfgType);
EXTERN Void smFillNghEutraFreqParam(LwrNeighEutraFreq *lwrNghEutraFreq,\
                                   MsmLteNeighEutraFreq *msmNghEutraFreq,\
                                   U32 reCfgType);
EXTERN Void smFillNghGeranFreqParam(LwrNeighGERANFreq *lwrNghGeranFreq,\
                                   MsmLteNeighGERANFreq *msmNghGeranFreq,\
                                   U32 reCfgType, U32 idx);
EXTERN Void smFillNghUtraCellParam(LwrUtraNeighCell *lwrNghUtraCell,\
                                   MsmUtraNeighCell *msmNghUtraCell,\
                                   U32 reCfgType);
EXTERN Void smFillNghEutraCellParam(LwrEUTRANeighCell *lwrNghEutraCell,\
                                   MsmEUTRANeighCell *msmNghEutraCell,\
                                   U32 reCfgType);
EXTERN Void smFillNghGeranCellParam(LwrGERANNeighCell *lwrNghGeranCell,\
                                   MsmLteGERANNeighCell *msmNghGeranCell,\
                                   U32 reCfgType, U32 idx);
EXTERN S16 smValidateFillMmeIp(LwrSmMmeCfg *mmeCfg, U8 *idx, Mmeiptype ip);

EXTERN Void smFillNghEnbParam(SmNeigEnbList *nghEnbCfg,\
                              MsmEUTRANeighCell *msmNghEutraCell);
EXTERN Void smFillNghHashKeyInit(smNghCellHasKey *hashKey, LwrPlmnId *plmnId, U16 cId);
EXTERN S16 smFillNghCdma1xBandParam(LwrNeighCdmaBandClassCfg *bandClassCfg, MsmCdmaBandClassCfg *bandClass, U32 reCfgType, U8 idx);
EXTERN S16 smFillNghCdma1xCellParam(LwrCDMA1xNeighCell *lwrNghCdma1xCell, MsmCDMA1xNeighCell *msmNghCdma1xCell, LwrNeighCdma1xFreq *lwrNghCdma1xFreq, U32 reCfgType, U16 cnt);
EXTERN Void smFillNghCdma1xFreqParam(LwrNeighCdma1xFreq *lwrNghCdma1xFreq, LwrCDMA1xNeighCell *nghCdma1xCell, U32 reCfgType, U8 idx);
EXTERN S16 smBuildWrNr1xCdmaFreqCfg(LwrMngmt *lwrMng);
EXTERN S16 smBuildWrNr1xCdmaCellCfg(LwrMngmt *lwrMng);
EXTERN S16 wrSendDynAddMmeCfg(U32 mmeIndex);
EXTERN S16 smSendDynCfgReq(MsmSubscriptionGroup msmCfgType,LwrDynCfgEvntType lwrCfgType,Void *data);
EXTERN S16 smHdlNeighUtraCellCfgAdd(LwrUtraNeighCell *updateddUtraCellCfg);
EXTERN S16 MsmSmmDynCfgReq(Void *cfg, U32 cfgType, U8 priority);
EXTERN S16 smInitMmeCfg(Void);
EXTERN S16 SmHdlNghUrtaCellUpDateMib(LwrUtraNeighCell *lwrUtranCell);
EXTERN S16 SmHdlNghEurtaCellUpDateMib(LwrEUTRANeighCell *lwrEutranCell);
EXTERN S16 smStaticCfgReq(Void);
EXTERN Void MsmStartStopStackReq(U8 isPremReq,U8 isStkSt,U8 isImd);
#endif/*end of WR_RSYS_OAM*/

EXTERN S16 smBuildGenCfg(LwrMngmt *lwrMng);

#if 0
#ifdef LTE_L2_MEAS
EXTERN Void wrSendL2MeasTrigger(Void);
EXTERN S16 PtLiLrgSchL2MeasReq(Pst * pst,LrgSchMeasReqInfo *meas);
EXTERN S16 PtMiLpjL2MeasStopReq(Pst *pst, U16 measType);
EXTERN S16 PtMiLpjL2MeasSendReq(Pst *pst, U16 measType);
EXTERN S16 wrSendL2MeasStartReq(Void);
EXTERN S16 smSndRgL2MeasStartReq(U8 measType);
EXTERN S16 smSndPjL2MeasStartReq(U16 measType);
EXTERN S16 smSndKwL2MeasStartReq(U8 measType);
EXTERN S16 smSndRgSchL2MeasStartReq(U8 measType);
EXTERN S16 smSndPjL2MeasStopReq(U16 measType);
EXTERN S16 smSndRgSchL2MeasStopReq(U8 measType);
EXTERN S16 smSndPjL2MeasSendReq(U16 measType);
EXTERN S16 smSndRgSchL2MeasSendReq(U8 measType);
EXTERN S16 wrSndL2MeasStopReq (Void);
EXTERN S16 wrSndL2MeasSendReq(Void);
EXTERN S16 smSndKwL2MeasStopReq(U8 measType);
EXTERN S16 smSndKwL2MeasSendReq(U8 measType);
#endif/*LTE_L2_MEAS*/ 
#endif
EXTERN Void smBuildShutRestart(Void);

#ifdef LTE_L2_MEAS
EXTERN Void wrSendL2MeasTrigger(Void);
EXTERN S16 PtLiLrgSchL2MeasReq(Pst * pst,LrgSchMeasReqInfo *meas);
EXTERN S16 PtMiLpjL2MeasStopReq(Pst *pst, U16 measType);
EXTERN S16 PtMiLpjL2MeasSendReq(Pst *pst, U16 measType);
EXTERN S16 wrSendL2MeasStartReq(Void);
EXTERN S16 smSndRgL2MeasStartReq(U8 measType);
EXTERN S16 smSndPjL2MeasStartReq(U16 measType);
EXTERN S16 smSndKwL2MeasStartReq(U8 measType);
EXTERN S16 smSndRgSchL2MeasStartReq(U16 measType);
EXTERN S16 smSndPjL2MeasStopReq(U16 measType);
EXTERN S16 smSndRgSchL2MeasStopReq(U16 measType);
EXTERN S16 smSndPjL2MeasSendReq(U16 measType);
EXTERN S16 smSndRgSchL2MeasSendReq(U16 measType);
EXTERN S16 wrSndL2MeasStopReq (Void);
EXTERN S16 wrSndL2MeasSendReq(Void);
EXTERN S16 smSndKwL2MeasStopReq(U8 measType);
EXTERN S16 smSndKwL2MeasSendReq(U8 measType);
#endif/*LTE_L2_MEAS*/ 


EXTERN S16 wrMemCpuL3MeasSendReq(Void);
EXTERN S16 wrMemCpuL3MeasStopReq(Void);
EXTERN S16 wrMemCpuUtilMeasStartReq(Void);
EXTERN Void wrMemCpuUtilTmrExp(Void);
EXTERN Void smSendKpiMemCpuInfo(Void);
EXTERN Void smBuildShutdown(Void);

typedef enum {
   WR_TAG_CELL_ID,
   WR_TAG_MOD_TYPE,
   WR_TAG_DUP_MODE,
   WR_TAG_MAX_UE_SUPPORT,
   WR_TAG_MAX_AVG_GBRPRB_USAGE, /* RRM_SP1_START */
   WR_TAG_MCC_0,
   WR_TAG_MCC_1,
   WR_TAG_MCC_2,
   WR_TAG_MNC_0,
   WR_TAG_MNC_1,
   WR_TAG_MNC_2,
   WR_TAG_TA_CODE,
   WR_TAG_FREQ_BAND_IND,
   WR_TAG_ENB_IP_ADDR,
   WR_TAG_ENB_IP_ADDR_IPV6,
   WR_TAG_PCI_SELECT_TYPE,
   WR_TAG_PRACH_SELECT_TYPE,
   WR_TAG_EARFCN_SELECT_TYPE,
   WR_TAG_PCI_LIST,
   WR_TAG_DL_EARFCN_LIST,
   WR_TAG_UL_EARFCN_LIST,
   WR_TAG_ROOTSEQ_IDX_LIST,
   WR_TAG_PRACHCFG_IDX_LIST,
   WR_TAG_ZERO_CORR_ZONE_CFG_LIST,
   WR_TAG_PRACH_FREQ_OFFSET_LIST,
   WR_TAG_ROOTSEQ_IDX,
   WR_TAG_PRACHCFG_IDX,
   WR_TAG_ZERO_CORR_ZONE_CFG,
   WR_TAG_PRACH_FREQ_OFFSET,
   WR_TAG_NO_OF_MME_INFO,
   WR_TAG_MME_INFO,
   WR_TAG_NO_OF_MME_INFO_IPV6,
   WR_TAG_MME_INFO_IPV6,
   WR_TAG_HI_DBG,
   WR_TAG_SB_DBG,
   WR_TAG_SZ_DBG,
   WR_TAG_EG_DBG,
   WR_TAG_WR_DBG,
   WR_TAG_NH_DBG,
   WR_TAG_KW_DBG,
   WR_TAG_RG_DBG,
   WR_TAG_YS_DBG,
   WR_TAG_SM_DBG,
   WR_TAG_INACTIVITY_TIMER_VAL,
   WR_TAG_MAX_EXPIRY,
#ifdef LTE_HO_SUPPORT
   WR_TAG_CZ_DBG,
   WR_TAG_X2_PREP_TMR,
   WR_TAG_X2_OVRALL_TMR,
   WR_TAG_ENB_NAME,
   WR_TAG_NO_OF_BRDCST_PLMN,
   WR_TAG_PLMN_ID_LST,
   WR_TAG_NO_OF_NGH_INFO,
   WR_TAG_NGH_INFO_CFG,
   WR_TAG_NO_OF_GU_GRP,
   WR_TAG_GU_GRP_ID_CFG,
   WR_TAG_S1_PREP_TMR,
   WR_TAG_S1_OVRALL_TMR,
   WR_TAG_NO_OF_NGH_CFG,
   WR_TAG_NGH_CELL_CFG,
   WR_TAG_NO_OF_NGH_CFG_IPV6,
   WR_TAG_NGH_CELL_CFG_IPV6,
   WR_TAG_MEAS_CFG_ENB,
   WR_TAG_RRM_RNTI_STRT,
   WR_TAG_MAX_RRM_RNTIS,
   WR_TAG_MAC_RNTI_STRT,
   WR_TAG_MAX_MAC_RNTIS,
   WR_TAG_RRM_NO_OF_DED_PREMBL,
   WR_TAG_RRM_DED_PREMBL_STRT,
   WR_TAG_MAC_NO_OF_PREMBL,
   WR_TAG_A1_RSRP_THRSHLD_VAL,
   WR_TAG_A2_RSRP_THRSHLD_VAL,
   WR_TAG_INTRA_A5_RSRP_THRSD1_VAL,
   WR_TAG_INTRA_A5_RSRP_THRSD2_VAL,
   WR_TAG_INTER_A5_RSRP_THRSD1_VAL,
   WR_TAG_INTER_A5_RSRP_THRSD2_VAL,
   WR_TAG_INTRA_HO_A3_OFFSET,
   WR_TAG_INTRA_ANR_A3_OFFSET,
   WR_TAG_INTER_ANR_A5_RSRP_THRSD1_VAL,
   WR_TAG_INTER_ANR_A5_RSRP_THRSD2_VAL,
   WR_TAG_ANR_REPORT_CFG_VAL,
   WR_TAG_ANR_CELL_VALID_AGE_VAL,
   WR_TAG_HO_REPORT_CFG_VAL,
   WR_TAG_RSRQ_THRSHLD_VAL,
   WR_TAG_ANR_EPOC_TMR_VAL_IN_SECS,
   WR_TAG_ANR_TRICE_INTV_COUNT,
   WR_TAG_UTRA_B2_RSRP_THRSD1_VAL,
   WR_TAG_UTRA_FDD_B2_RSCP_THRSD2_VAL,
   WR_TAG_UTRA_TDD_B2_RSCP_THRSD2_VAL,
   WR_TAG_S_MEASURE_VAL,
   WR_TAG_INTRA_TTT_VAL,
   WR_TAG_INTRA_HYTERISIS,
   WR_TAG_NO_OF_NGH_ENB_CFG,
   WR_TAG_NGH_ENB_CFG,
   WR_TAG_ENB_TYPE,
   WR_TAG_PCI_VAL,
   WR_TAG_DL_NUM_UE_PER_TTI,
   WR_TAG_UL_NUM_UE_PER_TTI,
   WR_TAG_DL_SCHD_TYPE,
   WR_TAG_UL_SCHD_TYPE,
   WR_TAG_DLFS_SCHD_TYPE,
   WR_TAG_PFS_DL_TPT_COEFFICIENT,
   WR_TAG_PFS_DL_FAIRNESS_COEFFICIENT,
   WR_TAG_PFS_UL_TPT_COEFFICIENT,
   WR_TAG_PFS_UL_FAIRNESS_COEFFICIENT,
   WR_TAG_TM_AUTO_CONFIG,
   WR_TAG_PREFERRED_TM,
   WR_TAG_MAX_X2_PEERS,
   WR_TAG_X2_TIME_TO_WAIT,
   WR_TAG_UE_HO_ATTACH_TMR,
   WR_TAG_SCTP_SRVC_TYPE,
   WR_TAG_OP_MODE,
   WR_TAG_PERIOD,
   WR_TAG_DL_EARFCN,
   WR_TAG_UL_EARFCN,
   WR_TAG_END_MARKER_TIMER_VAL,
#endif
#ifdef OAM_PH0_SUPPORT
   WR_TAG_S1_RESET_TMR,
   WR_TAG_X2_RESET_TMR,
#endif
   WR_TAG_AS_PRI_INTG_ALGO_LST,
   WR_TAG_AS_PRI_CIPH_ALGO_LST,
   WR_TAG_DIAG_TUCL,
   WR_TAG_DIAG_S1AP,
   WR_TAG_DIAG_EGTP,
   WR_TAG_DIAG_APP,
   WR_TAG_DIAG_LTERRC,
   WR_TAG_DIAG_LTERLC,
   WR_TAG_DIAG_LTEPDCP,
   WR_TAG_DIAG_LTECL,
   WR_TAG_DIAG_LTEMAC,
   WR_TAG_DIAG_X2AP,
   WR_TAG_DIAG_SCTP,
#if PHY_ERROR_LOGING
   WR_TAG_UL_MCS_VAL,
   WR_TAG_UL_RB_START_VAL,
   WR_TAG_UL_NUM_RB_VAL,
   WR_TAG_UL_ENABLE_TIME,
#endif
   WR_TAG_NO_OF_UTRA_FDD_NGH_CFG,
   WR_TAG_NGH_UTRA_FDD_CELL_CFG,
   WR_TAG_NO_OF_UTRA_TDD_NGH_CFG,
   WR_TAG_NGH_UTRA_TDD_CELL_CFG,
   WR_TAG_NUM_EUTRA_FREQ,
   WR_TAG_EUTRA_FREQ_CFG,
   WR_TAG_NUM_UTRA_FDD_FREQ,
   WR_TAG_UTRA_FDD_FREQ_CFG,
   WR_TAG_NUM_UTRA_TDD_FREQ,
   WR_TAG_UTRA_TDD_FREQ_CFG,
   WR_TAG_NUM_CDMA_1XRTT_BAND_CLS,
   WR_TAG_CDMA_1XRTT_BAND_CLS,
   WR_TAG_NUM_CDMA_1XRTT_NEIGH_FREQ,
   WR_TAG_CDMA_1XRTT_NEIGH_FREQ,
   WR_TAG_NUM_CDMA_1XRTT_NEIGH_CELL,
   WR_TAG_CDMA_1XRTT_NEIGH_CELL,
   WR_TAG_CSFB_UTRA_CFG_VAL,
   WR_TAG_CSFB_CDMA_CFG_VAL,
   WR_TAG_ECSFB_CFG_VAL,
   WR_TAG_DUAL_RX_SUPPORTED,
   WR_TAG_MAX_DL_UE_PER_TTI,
   WR_TAG_MAX_UL_UE_PER_TTI,
   WR_TAG_DEFAULT_PAGING_CYCLE,
   WR_TAG_DEFAULT_PCCH_CFG_NB,
   WR_TAG_SCTP_RTO_MIN,
   WR_TAG_SCTP_RTO_MAX,
   WR_TAG_SCTP_RTO_INITIAL,
   WR_TAG_SCTP_HRTBEAT_INTERVAL,
   WR_TAG_ABS_A3_OFFSET,
   WR_TAG_PICO_OFFSET,
   WR_TAG_ABS_PATTERN_TYPE,
   WR_TAG_ABS_PATTERN,
   WR_TAG_ABS_LOAD_PERIODICITY,
   WR_TAG_SFR_START_RB,
   WR_TAG_SFR_END_RB,
   WR_TAG_SFR_POWER_LOW,
   WR_TAG_SFR_POWER_HIGH,
   WR_TAG_DRX_QCI_SUPPORT_ENABLED,
   WR_TAG_DRX_ENABLED,                              
   WR_TAG_DRX_INACTIVITY_TMR,
   WR_TAG_DRX_RETX_TMR,
   WR_TAG_DRX_LONG_CYCLE_GBR,
   WR_TAG_DRX_LONG_CYCLE_NON_GBR,
   WR_TAG_ANR_DRX_LONG_CYCLE,
   WR_TAG_ANR_DRX_ON_DURATION_TMR,  
   WR_TAG_ANR_DRX_INACTIVITY_TMR, 
#ifdef E_TM
   WR_TAG_BOOT_MODE,
   WR_TAG_ETM_ID,
#endif 
   WR_TAG_BANDWIDTH,
   WR_TAG_NO_OF_EAID_INFO,
   WR_TAG_EAID_LIST_INFO,
   WR_TAG_PRIMETWS_NO_BROADCAST, /* ccpu00128279 : Number of broadcast for primary ETWS. */
   WR_TAG_PRIMETWS_REPETITION_PERIOD, /* ccpu00128279 : Repetition period for primary ETWS. */
   WR_TAG_PWS_ETWS_CMAS_CNTRL,
   WR_TAG_TDD_ULDL_CFG_MODE,
   WR_TAG_TDD_SPCL_SF_CONFIG,
#ifdef WR_WATCHDOG
   WR_TAG_WATCHDOG_SOFT_LIMIT,
   WR_TAG_WATCHDOG_HARD_LIMIT,
#endif
   WR_TAG_INT_FREQ_MEAS_GAP,
   WR_TAG_ANR_MEAS_GAP_CONFIG,
   WR_TAG_SRC_HO_CANCEL_TMR,
   WR_TAG_LOG_PATH,
   WR_TAG_LOG_FILE,
   WR_TAG_LOG_MAX_FILES,
   WR_TAG_LOG_FILESIZE_LIMIT,
   WR_TAG_LOG_LEVEL,
   WR_TAG_LOG_MASK,
   WR_TAG_LOG_PORT,
   WR_TAG_LOG_REMOTE_LOGGING,
   WR_TAG_LOG_CIRBUF_SIZE,
   WR_TAG_LOG_COREDUMP_FLAG,
   WR_TAG_RRM_SR_PRDCTY,
   WR_TAG_RRM_CQI_PRDCTY,
   WR_TAG_RRM_NUM_SR_PER_TTI,
   WR_TAG_RRM_NUM_CQI_PER_TTI,
   WR_TAG_RRM_N1_PUCCH,
   WR_TAG_SPS_ENABLE_FLAG,
   WR_TAG_DL_NUM_SPS_UE_PER_TTI,
   WR_TAG_UL_NUM_SPS_UE_PER_TTI,
   WR_TAG_MAX_SPS_RB,
   WR_TAG_DSCP_ENABLE,         /*TAG for enabling/disabling the DSCP marking feature*/
   WR_TAG_QCI_DSCP_MAP,        /*Mapping Table for QCI-DSCP*/
   WR_TAG_DL_QCI_SCHD_WGT,
   WR_TAG_UL_QCI_SCHD_WGT,
   WR_TAG_CSG_CSG_ID,
   WR_TAG_CSG_ACCESS_MODE,
   WR_TAG_CSG_CSG_PCI_START,
   WR_TAG_CSG_CSG_PCI_RANGE,
   WR_TAG_CSG_SMCELL_PCI_START,
   WR_TAG_CSG_SMCELL_PCI_RANGE,
   WR_TAG_CSG_HENB_NAME,
   WR_TAG_MAX_CSG_UES,
   WR_TAG_MAX_NON_CSG_UES,
   WR_TAG_MIN_DL_RSRC_FOR_NON_CSG,
   WR_TAG_MIN_UL_RSRC_FOR_NON_CSG,
   WR_TAG_RIM_CFG,
   WR_TAG_NUM_GERAN_NEIGH_FREQ,  
   WR_TAG_GERAN_NEIGH_FREQ,
   WR_TAG_GERAN_NEIGH_CELL,
   WR_TAG_GERAN_MEAS_CFG,
   WR_TAG_CELL_PMAX,
   WR_TAG_CNM_ENABLED,
   WR_TAG_CNM_BOARD_TYPE,
   WR_TAG_TA_TMR_CMN,
   WR_TAG_TA_TMR_DED,
   WR_TAG_CELL_SIZE_TYPE, /*UE History info*/
   WR_TAG_ARP_EMER_SERV, /* ARP for Emergency Bearers */
   WR_TAG_DCFI_ENABLE,
   WR_TAG_CFI,
   WR_TAG_GRP_PUCCH_PWR_CTRL_ENABLE,
   WR_TAG_GRP_PUSCH_PWR_CTRL_ENABLE,
   WR_TAG_LTE_PUCCH_PWR_FMT3A_ENABLE,
   WR_TAG_LTE_PUSCH_PWR_FMT3A_ENABLE,
   /* ccpu00138740 */
   WR_TAG_PRACH_CONFIG_IDX,
   WR_TAG_DUAL_RX_TX_SUPPORTED,/*RRC Rel 10 Upgrade*/
   WR_TAG_GERAN_B2_RED_RSRP_THRSD1_VAL,
   WR_TAG_GERAN_B2_RED_THRSD2_VAL,
   WR_TAG_GERAN_B2_HO_RSRP_THRSD1_VAL,
   WR_TAG_GERAN_B2_HO_THRSD2_VAL,
   WR_TAG_NUM_PRB_RPTS,
   WR_TAG_PRB_RPT_INTERVAL,
   WR_TAG_SRS_ENABLE,
   WR_TAG_SRS_PERIODICITY,
   WR_TAG_SRS_CELL_BANDWIDTH,
   WR_TAG_SRS_CELL_SUBFRAME_CFG,
   WR_TAG_SRS_CELL_ACKNACK_SIMTX,
   WR_TAG_SRS_CELL_MAX_UPPTS,
   WR_TAG_SRS_UE_BANDWIDTH,
   WR_TAG_SRS_UE_HOPPING_BW,
#ifdef RSYS_WIRESHARK
   WR_TAG_WIRESHARK_PORT,/* to send Logs to wireshark */
   WR_TAG_WIRESHARK_DST_IP_ADDR, /* to send Logs to wireshark */
#endif
   WR_TAG_RSYS_TPM_TGT_RSRP,
   WR_TAG_RSYS_TPM_ATTN_CONST_IDX,
   WR_TAG_RSYS_TPM_MIN_RSRP,
   WR_TAG_RSYS_TPM_MAX_RSRP,
   WR_TAG_NUM_TX_ANTENNA,
   MAX_CFG_PARAM
}wrCfgTagNum;

EXTERN S8 *wrCfgTagLst[MAX_CFG_PARAM];
EXTERN U16 wrSmmGetX2apFreePeerId(Void);

EXTERN U16 cmMaxBufLen;
EXTERN S16 wrSendDynAddMmeCfg
(
U32 mmeIndex
);
EXTERN S16 wrSmDynAddMme(void);
EXTERN Void smWrProcCliMsg(Void);
EXTERN Void wrSmmSetBandwidth(U32    freqBandwidth);

/* MACRO to identify UTRA duple mode */
#define  WR_UTRA_DUPLEX_FDD   0
#define  WR_UTRA_DUPLEX_TDD   1

#ifdef SS_DIAG
 /** @details Macro definition for WR level 0 logs
 *
*/
#define SM_DIAG_LVL0(_tknId, _splArg, _string, _arg1, _arg2, _arg3, _arg4)                      \
{                                                                                              \
   if(smCb.init.logMask & SS_DIAG_LVL0)                                                         \
   {                                                                                           \
      ssDiagFix(_tknId, 0, ENTSM, smCb.init.inst, SS_DIAG_LVL0, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                                                           \
}

/** @details Macro definition for WR level 1 logs
 *  
*/
#define SM_DIAG_LVL1(_tknId, _splArg, _string, _arg1, _arg2, _arg3, _arg4)            \
{                                                                 \
   if(smCb.init.logMask & SS_DIAG_LVL1)                             \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTSM, smCb.init.inst, SS_DIAG_LVL1, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for WR level 2 logs
 *  
*/
#define SM_DIAG_LVL2(_tknId, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       \
{                                                              \
   if(smCb.init.logMask & SS_DIAG_LVL2)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTSM, smCb.init.inst, SS_DIAG_LVL2, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for WR  level 3 logs 
 *  
*/
#define SM_DIAG_LVL3(_tknId, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
   if(smCb.init.logMask & SS_DIAG_LVL3)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTSM, smCb.init.inst, SS_DIAG_LVL3, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}

/** @details Macro definition for WR  level 4 logs
 *  
*/
#define SM_DIAG_LVL4(_tknId, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        \
{                                                              \
   if(smCb.init.logMask & SS_DIAG_LVL4)                            \
   {                                                              \
      ssDiagFix(_tknId, _splArgEnum, ENTSM, smCb.init.inst, SS_DIAG_LVL4, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                              \
}
#else

#define SM_DIAG_LVL0(_tknId, _splArg, _string, _arg1, _arg2, _arg3, _arg4)        
#define SM_DIAG_LVL1(_tknId, _splArg, _string, _arg1, _arg2, _arg3, _arg4)       
#define SM_DIAG_LVL2(_tknId, _splArg, _string, _arg1, _arg2, _arg3, _arg4)      
#define SM_DIAG_LVL3(_tknId, _splArg, _string, _arg1, _arg2, _arg3, _arg4)     
#define SM_DIAG_LVL4(_tknId, _splArg, _string, _arg1, _arg2, _arg3, _arg4)    
#endif
 

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_SM_INIT_H__ */


/********************************************************************30**

           End of file:     wr_smm_init_brcm.h@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:53:21 2014

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
SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
