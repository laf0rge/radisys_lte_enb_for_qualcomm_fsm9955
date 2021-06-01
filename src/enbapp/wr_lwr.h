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
               

     File:     wr_lwr.h

     Sid:      $SID$

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_LWR_H__
#define __WR_LWR_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

#include "lnl.h"
#include "lnl.x"

#define LWR_NEIGH_ADD_CFG           1      /*!< Neigh configuration addition */
#define LWR_NEIGH_MOD_CFG           2      /*!< Neigh configuration addition */
#define LWR_NEIGH_DEL               3      /*!< Neigh configuration addition */

#define MAX_NUM_NEIGH_CELLS           40
#define LWR_MAX_NUM_NEIGH_GERAN_CELLS 16 /* Max Number of Geran cells*/
#define LWR_MAX_NUM_NEIGH_GERAN_FREQS 16 /* Max Number of Geran Freqs */
#define LWR_MAX_NUM_MBLTY_LAI         32 /* Max Number of Mobility LAI */

/* bitmaps for structure elements */
#define LWR_CELL_MEAS_CFG_A1EVT_RPT               1
#define LWR_CELL_MEAS_CFG_A2EVT_RPT               2
#define LWR_CELL_MEAS_CFG_A4EVT_RPT               4
#define LWR_CELL_MEAS_CFG_EVT_A3                  8
#define LWR_CELL_MEAS_CFG_EVT_A5                  16
#define LWR_CELL_MEAS_CFG_EUTRA_PERD_RPT          32
#define LWR_CELL_MEAS_CFG_1XCDMA_EVT_B1_RPT       64
#define LWR_CELL_MEAS_CFG_CDMA_HRPD_EVT_B1_RPT    128
#define LWR_CELL_MEAS_CFG_1XCDMA_EVT_B2_RPT       256
#define LWR_CELL_MEAS_CFG_CDMA_HRPD_EVT_B2_RPT    512
#define LWR_CELL_MEAS_CFG_1XCDMA_PERIOD_RPT       1024
#define LWR_CELL_MEAS_CFG_CDMA_HRPD_PERIOD_RPT    2048
#define LWR_CELL_MEAS_CFG_SPD_SCL_FACT            4096
#define LWR_CELL_MEAS_CFG_MOB_STAT_PARAMS         8192 
#define LWR_CELL_MEAS_CFG_QTY_CFG                 16384
#define LWR_CELL_MEAS_CFG_MEAS_GAP                32768
#define LWR_CELL_MEAS_CFG_PRE_REG_HRPD            65536

/* LwrENodeBCfg bitmaps for structure elements */
#define  LWR_ENB_NUM_OF_UE                        1
#define  LWR_ENB_NUM_OF_GRPS                      2
#define  LWR_ENB_GRP_CFG_LST                      4 
#define  LWR_ENB_PROTOCFG                         8
#define  LWR_ENB_SON_ENB_CNTRL                    16
#define  LWR_ENB_ANR_ALGO_INFO                    32
#define  LWR_ENB_NUM_MME_PRI_GRPS                 64
#define  LWR_ENB_MME_LDBAL_PRI_GRPS_LST           128
#define  LWR_ENB_PWS_ENABLE_DISABLE               256

/* LwrAddModMmeCfg bitmaps for structure elements */
#define LWR_MME_INDEX                             1
#define LWR_PLMN_ID                               2
#define LWR_MME_GROUP_ID                          4
#define LWR_MME_CODES                             8
#define LWR_MME_IPADDR                            16
#define LWR_MME_SECIP_ADDR                        32
#define LWR_MME_STATE                             64
#define LWR_ADMIN_STATE                           128
#define LWR_MAX_IP_PER_MME                        5 
/*LwrAddCellCfg bitmaps for structure elements*/
#define LWR_CELL_CFG_PHY_CELL_ID                  1
#define LWR_CELL_CFG_DUP_MODE                     2
#define LWR_CELL_CFG_DL_CARR_FREQ                 4
#define LWR_CELL_CFG_GRP                          8
#define LWR_CELL_CFG_PHY_PCI_UPDATE              16
#define LWR_CELL_SON_UPDATE                      32

/* LwrCellMibCfgGrp bitmaps for structure elements */
#define LWR_CELL_MIB_CFG_DLBW                     1
#define LWR_CELL_MIB_CFG_PHICH_CFG                2



/* LwrCellSib1CfgGrp bitmaps for structure elements */
#define LWR_CELL_SIB1_CFG_CELL_ACCESS_INFO        1
#define LWR_CELL_SIB1_CFG_CELL_SELECT_INFO        2
#define LWR_CELL_SIB1_CFG_PMAX                    4 
#define LWR_CELL_SIB1_CFG_FREQ_BAND_IND           8 
#define LWR_CELL_SIB1_CFG_TDD_CFG                 16
#define LWR_CELL_SIB1_CFG_SIWIN_SIZE              32 

/* LwrCellSib2CfgGrp bitmaps for structure elements */
#define LWR_CELL_SIB2_CFG_BARRING_INFO            1 
#define LWR_CELL_SIB2_CFG_RADIO_RSRC_CMN          2
#define LWR_CELL_SIB2_CFG_UE_TMR_CONS             4
#define LWR_CELL_SIB2_CFG_UE_FREQ_INFO            8
#define LWR_CELL_SIB2_CFG_TIME_ALIGN              16
#define LWR_CELL_SIB2_CFG_BARRING_VOICE           32
#define LWR_CELL_SIB2_CFG_BARRING_VIDEO           64

/*SIB2 Radio Resource Common bitmap for structure elements*/
#define LWR_CELL_SIB2_RRSRCMN_RACH_CFG_CMN         1 
#define LWR_CELL_SIB2_RRSRCMN_BCCH_CFG_CMN         2 
#define LWR_CELL_SIB2_RRSRCMN_PCCH_CFG_CMN         4 
#define LWR_CELL_SIB2_RRSRCMN_PRACH_CFG_CMN        8 
#define LWR_CELL_SIB2_RRSRCMN_PDSCH_CFG_CMN        16 
#define LWR_CELL_SIB2_RRSRCMN_PUSCH_CFG_CMN        32 
#define LWR_CELL_SIB2_RRSRCMN_PUCCH_CFG_CMN        64 
#define LWR_CELL_SIB2_RRSRCMN_SRS_CFG_CMN          128
#define LWR_CELL_SIB2_RRSRCMN_UL_PWR_CNRL_CFG_CMN  256
#define LWR_CELL_SIB2_RRSRCMN_UL_CYCPREFIX_CFG_CMN 512
#define LWR_CELL_SIB2_RRSRCMN_DL_CYCPREFIX_CFG_CMN 1024 

/* LwrCellSib3CfgGrp bitmaps for structure elements */
#define LWR_CELL_SIB3_CFG_QHYST                   1
#define LWR_CELL_SIB3_CFG_SPD_ST_RESEL            2
#define LWR_CELL_SIB3_CFG_MOB_ST_PAR              4
#define LWR_CELL_SIB3_CFG_SF_MEDIUM               8
#define LWR_CELL_SIB3_CFG_SF_HIG                  16
#define LWR_CELL_SIB3_CFG_SERV_FREQ_INFO          32
#define LWR_CELL_SIB3_CFG_INTRA_RREQ              64

/* bitmaps for structure elements */
#define LWR_CELL_SIB3_CFG_CSG_INFO                1

/* bitmaps for structure elements */
#define LWR_CELL_ANR_CFG_CTRL_INFO                1
#define LWR_CELL_ANR_CFG_SON_ANR_SCH              2
#define LWR_CELL_ANR_CFG_BL_CELL_INFO             4
#define LWR_CELL_ANR_CFG_EUTRAN_BL_NR_INFO        8
#define LWR_CELL_ANR_CFG_CDMA_1XRTT_INFO          16
#define LWR_CELL_ANR_CFG_CDMA_HRPD_INFO           32

/* bitmaps for structure elements */
#define LWR_CELL_EAID_CFG_EAID                    1

/* bitmaps for structure elements */
#define LWR_CELL_SIBSCH_SIB2_PERIOD           1
#define LWR_CELL_SIBSCH_SIB3_PERIOD           2  
#define LWR_CELL_SIBSCH_SIB4_PERIOD           4
#define LWR_CELL_SIBSCH_SIB5_PERIOD           8
#define LWR_CELL_SIBSCH_SIB6_PERIOD          16
#define LWR_CELL_SIBSCH_SIB7_PERIOD          32
#define LWR_CELL_SIBSCH_SIB8_PERIOD          64
#define LWR_CELL_SIBSCH_SIB9_PERIOD          128
#define LWR_CELL_SIBSCH_SIB10_PERIOD         256
#define LWR_CELL_SIBSCH_SIB11_PERIOD         512
#define LWR_CELL_SIBSCH_SIB12_PERIOD         1024
#define LWR_CELL_SIBSCH_SWIN_PERIOD          2048 
#define LWR_CELL_SIBSCH_SIB10_MANUAL_PRD     4096
#define LWR_CELL_SIBSCH_SIB10_REPEAT_PRD     8192
#define LWR_CELL_SIBSCH_SIB10_NUM_BRDCST     16384
#define LWR_CELL_SIBSCH_SIB11_WARN_SEG       32768

/* LwrDrbPolicy bitmaps for structure elements */
#define LWR_CELL_DRB_QCI                          1
#define LWR_CELL_DRB_QCI_INFO                     2
#define LWR_CELL_DRB_LCH_INFO                     4
#define LWR_CELL_DRB_RLC_INFO                     8
#define LWR_CELL_DRB_PDCP_INFO                    16
#define LWR_CELL_DRB_DSCP_INFO                    32

/* LwrRabPolicyCfgGrp bitmaps for structure elements */
#define LWR_CELL_RAB_SRB_POLICY                   1
#define LWR_CELL_RAB_RAB_POLICY                   2

/* LwrSrbPolicy bitmaps for structure elements */
#define LWR_CELL_SRB_SRBID                        1
#define LWR_CELL_SRB_RLC_INFO                     2
#define LWR_CELL_SRB_LCH_INFO                     4
#define LWR_CELL_SRB_QCI_INFO                     8

/* LwrBlackListCellInfo bitmaps for structure elements */
#define  LWR_BL_CELL_INFO_INDEX                   1
#define  LWR_BL_CELL_INFO_STATUS                  2
#define  LWR_BL_CELL_INFO_PCITYPE                 4
#define  LWR_BL_CELL_INFO_STARTPCI                8
#define  LWR_BL_CELL_INFO_PCIRANGE                16
#define  LWR_BL_CELL_INFO_EARFCNDL                32
#define  LWR_BL_CELL_INFO_EARFCNUL                64


/* LwrEutranBLNrInfo bitmaps for structure elements */
#define  LWR_EUTRAN_BL_NR_INDEX                   1
#define  LWR_EUTRAN_BL_NR_STATUS                  2
#define  LWR_EUTRAN_BL_NR_ENBID                   4
#define  LWR_EUTRAN_BL_NR_TARGET_CELL_NUM         8
#define  LWR_EUTRAN_BL_NR_ENB_TYPE                16
#define  LWR_EUTRAN_BL_NR_TAC                     32
#define  LWR_EUTRAN_BL_NR_ENB_PLMN                64
#define  LWR_EUTRAN_BL_NR_PCI                     128

/* LwrEUTRANeighCell bitmaps for structure elements */

#define  LWR_EUTRAN_NGH_CELL_CELLNUM              1
#define  LWR_EUTRAN_NGH_CELL_CID                  2
#define  LWR_EUTRAN_NGH_CELL_ENBTYPE              4
#define  LWR_EUTRAN_NGH_CELL_PLMNID               8
#define  LWR_EUTRAN_NGH_CELL_PCI                  0x10
#define  LWR_EUTRAN_NGH_CELL_TAC                  0x20
#define  LWR_EUTRAN_NGH_CELL_DL_EARFCN            0x40
#define  LWR_EUTRAN_NGH_CELL_UL_EARFCN            0x80
#define  LWR_EUTRAN_NGH_CELL_DLBW                 0x100
#define  LWR_EUTRAN_NGH_CELL_ULBW                 0x200
#define  LWR_EUTRAN_NGH_CELL_CIO                  0x400
#define  LWR_EUTRAN_NGH_CELL_QOFFSET              0x800
#define  LWR_EUTRAN_NGH_CELL_CSG_ID               0x1000
#define  LWR_EUTRAN_NGH_CELL_CSG_ACC_MODE         0x2000
#define  LWR_EUTRAN_NGH_CELL_BLACK_LIST           0x4000
/* LwrCdmaHrpdBLInfo bitmaps for structure elements */

#define LWR_CDMA_HRPD_BL_NR_INDEX                 1
#define LWR_CDMA_HRPD_BL_NR_STATUS                2
#define LWR_CDMA_HRPD_BL_NR_COLOR_CODE            4
#define LWR_CDMA_HRPD_BL_NR_BSMID                 8
#define LWR_CDMA_HRPD_BL_NR_BSCID                 16
#define LWR_CDMA_HRPD_BL_NR_DPSSID                32
#define LWR_CDMA_HRPD_BL_NR_BTSID                 64
#define LWR_CDMA_HRPD_BL_NR_SECTORID              128
#define LWR_CDMA_HRPD_BL_NR_BANDCLASS             256
#define LWR_CDMA_HRPD_BL_NR_ARFCN                 512
#define LWR_CDMA_HRPD_BL_NR_PNOFFSET              1024


/* LwrCdma1xRttBLInfo bitmaps for structure elements */
#define LWR_CDMA_1XRTT_BL_NR_INDEX                1
#define LWR_CDMA_1XRTT_BL_NR_STATUS               2
#define LWR_CDMA_1XRTT_BL_NR_SID                  4
#define LWR_CDMA_1XRTT_BL_NR_NID                  8
#define LWR_CDMA_1XRTT_BL_NR_BASEID               16
#define LWR_CDMA_1XRTT_BL_NR_PNOFFSET             32
#define LWR_CDMA_1XRTT_BL_NR_MARKETID             64
#define LWR_CDMA_1XRTT_BL_NR_CELLID               128
#define LWR_CDMA_1XRTT_BL_NR_SECTOR_NUM           256
#define LWR_CDMA_1XRTT_BL_NR_BANDCLASS            512
#define LWR_CDMA_1XRTT_BL_NR_ARFCN                1024

/* LwrNeighEutraFreq bitmaps for structure elements */

#define   LWR_NBR_EUTRAN_FREQ_STATUS              1
#define   LWR_NBR_EUTRAN_FREQ_EARFCNUL            2
#define   LWR_NBR_EUTRAN_FREQ_EARFCNDL            4
#define   LWR_NBR_EUTRAN_FREQ_PRIORITY            8
#define   LWR_NBR_EUTRAN_FREQ_QRXLEVMIN           16
#define   LWR_NBR_EUTRAN_FREQ_PMAXUSAGE           32
#define   LWR_NBR_EUTRAN_FREQ_PMAX                64
#define   LWR_NBR_EUTRAN_FREQ_TRESELECTION        128
#define   LWR_NBR_EUTRAN_FREQ_SFUSAGE             256
#define   LWR_NBR_EUTRAN_FREQ_TRESELECTIONSFMED   512
#define   LWR_NBR_EUTRAN_FREQ_TRESELECTIONSFHIGH  1024
#define   LWR_NBR_EUTRAN_FREQ_SINTRASEARCH        2048
#define   LWR_NBR_EUTRAN_FREQ_SNONINTRASEARCH     4096
#define   LWR_NBR_EUTRAN_FREQ_SERVTHRESHOLDLOW    8192
#define   LWR_NBR_EUTRAN_FREQ_THRESHXHIGH         16384
#define   LWR_NBR_EUTRAN_FREQ_THRESHXLOW          32768
#define   LWR_NBR_EUTRAN_FREQ_MEASBANDWIDTH       65536
#define   LWR_NBR_EUTRAN_FREQ_PRESANTENNAPORT1    131072
#define   LWR_NBR_EUTRAN_FREQ_NEIGHCELLCFG        262144
#define   LWR_NBR_EUTRAN_FREQ_QOFFSETFREQ         524288
#define   LWR_NBR_EUTRAN_FREQ_OFFSETFREQ          1048576 
#define   LWR_NBR_EUTRAN_FREQ_SINTRASEARCHREL9    2097152
#define   LWR_NBR_EUTRAN_FREQ_SINTERSEARCHREL9    4194304
#define   LWR_NBR_EUTRAN_FREQ_QQUALMINREL9        8388608
#define   LWR_NBR_EUTRAN_FREQ_RESELECTHRESHQREL9  16777216
#define   LWR_NBR_EUTRAN_FREQ_THRESHXQREL9HIGH   33554432
#define   LWR_NBR_EUTRAN_FREQ_THRESHXQREL9LOW    67108864

/* LwrNeighUtraFreq bitmaps for structure elements */
#define   LWR_NBR_UTRAN_FREQ_FREQINDEX            1
#define   LWR_NBR_UTRAN_FREQ_ARFCNUL              2
#define   LWR_NBR_UTRAN_FREQ_ARFCNDL              4
#define   LWR_NBR_UTRAN_FREQ_PRIORITY             8
#define   LWR_NBR_UTRAN_FREQ_THRESHXHIGH          16
#define   LWR_NBR_UTRAN_FREQ_THRESHXLOW           32
#define   LWR_NBR_UTRAN_FREQ_QRXLEVELMIN          64
#define   LWR_NBR_UTRAN_FREQ_PMAXUTRA             128
#define   LWR_NBR_UTRAN_FREQ_PQUALMIN             256
#define   LWR_NBR_UTRAN_FREQ_OFFSETFREQ           512
#define   LWR_NBR_UTRAN_FREQ_DUPLEXMODE           1024
#define   LWR_NBR_UTRAN_FREQ_THRESHXHIGHQR9       2048
#define   LWR_NBR_UTRAN_FREQ_THRESHXLOWQR9        4096

/* These are CSFB GERAN related macros */
#define   LWR_NBR_GERAN_FREQ_FREQIDX              1
#define   LWR_NBR_GERAN_FREQ_BANDINDICATOR        2
#define   LWR_NBR_GERAN_FREQ_ARFCN                4
#define   LWR_NBR_GERAN_FREQ_QRXLEVMIN            8
#define   LWR_NBR_GERAN_FREQ_CELLRESEL_PRIORITY   16
#define   LWR_NBR_GERAN_FREQ_THRESHXHIGH          32
#define   LWR_NBR_GERAN_FREQ_THRESHXLOW           64
#define   LWR_NBR_GERAN_FREQ_NCC_PERMITTED        128

/* LwrUtraNeighCell bitmaps for structure elements */
#define   LWR_NBR_UTRAN_CELL_NRINDEX              1
#define   LWR_NBR_UTRAN_CELL_RNCID                2
#define   LWR_NBR_UTRAN_CELL_CID                  4
#define   LWR_NBR_UTRAN_CELL_LAC                  8
#define   LWR_NBR_UTRAN_CELL_RAC                  16
#define   LWR_NBR_UTRAN_CELL_PLMN                 32
#define   LWR_NBR_UTRAN_CELL_PSCMCODE             64
#define   LWR_NBR_UTRAN_CELL_REMOVE_ALLOWED       128
#define   LWR_NBR_UTRAN_CELL_HO_ALLOWED           256
#define   LWR_NBR_UTRAN_CELL_ARFCN_UL             512
#define   LWR_NBR_UTRAN_CELL_ARFCN_DL             1024
#define   LWR_NBR_UTRAN_CELL_VOIP_IN_CAPABLE      2048
#define   LWR_NBR_UTRAN_CELL_DUPLEXMODE           4096

/* These are CSFB GERAN related macros */
#define  LWR_NBR_GERAN_CELL_NRINDEX               1
#define  LWR_NBR_GERAN_CELL_PLMNID                2
#define  LWR_NBR_GERAN_CELL_LAC                   4
#define  LWR_NBR_GERAN_CELL_RAC                   8
#define  LWR_NBR_GERAN_CELL_BSIC                  16
#define  LWR_NBR_GERAN_CELL_CI                    32
#define  LWR_NBR_GERAN_CELL_BANDINDCATOR          64
#define  LWR_NBR_GERAN_CELL_ARFCN                 128
#define  LWR_NBR_GERAN_CELL_NCC_PERMITTED         256
#define  LWR_NBR_GERAN_CELL_DTM_CAPABLE           512

/* LwrCellSib6CfgGrp bitmaps for structure elements */
#define LWR_UTRAN_SIB6_TRESELECT                  1
#define LWR_UTRAN_SIB6_TRESELECT_SF_MEDIUM        2
#define LWR_UTRAN_SIB6_TRESELECT_SF_HIGH          4
#define LWR_UTRAN_SIB6_SFUSAGE_PRES                 8

/* These are SIB7 params GERAN related macros */
#define LWR_SIB7_TRESELCTIONGERAN                1
#define LWR_SIB7_PRES_SPEEDSTATEPARAM            2
#define LWR_SIB7_TRESELECT_SF_MEDIUM             4
#define LWR_SIB7_TRESELECT_SF_HIGH               8
#define LWR_SIB7_PMAXGERAN                       16  

/* LwrNeighCdma1xFreq bitmaps for structure elements */
#define LWR_CDMA_1XRTT_FREQ_STATUS                1
#define LWR_CDMA_1XRTT_FREQ_BANDCLASS             2
#define LWR_CDMA_1XRTT_FREQ_ARFCN                 4
#define LWR_CDMA_1XRTT_FREQ_SEARCHRATE            8
#define LWR_CDMA_1XRTT_FREQ_OFFSETFREQ            16
#define LWR_CDMA_1XRTT_FREQ_ANRUESEARCHRATE_1XRTT 32
                                                         
/* LwrNeighCdmaHRPDFreq bitmaps for structure elements */
                                                         
#define   LWR_CDMA_HRPD_FREQ_STATUS               1
#define   LWR_CDMA_HRPD_FREQ_BANDCLASS            2
#define   LWR_CDMA_HRPD_FREQ_ARFCN                4
#define   LWR_CDMA_HRPD_FREQ_SEARCHRATE           8
#define   LWR_CDMA_HRPD_FREQ_OFFSETFREQ           16
#define   LWR_CDMA_HRPD_FREQ_ANRUESEARCHRATE_HRPD 32

/* LwrNeighCdmaBandClassCfg bitmaps for structure elements */
#define LWR_NBR_CDMA_BANDCLASS_INDEX              1
#define LWR_NBR_CDMA_BANDCLASS_STATUS             2
#define LWR_NBR_CDMA_BANDCLASS_BANDCLASS          4 
#define LWR_NBR_CDMA_BANDCLASS_CELL_RESEL_PRI     8
#define LWR_NBR_CDMA_BANDCLASS_THRESHX_HIGH       16
#define LWR_NBR_CDMA_BANDCLASS_THRESHX_LOW        32

/* ccpu00135220 */
/* LwrProcTimerCfgGrp bitmaps for structure elements */
#define LWR_TMR_CFG_S1PREP_TMR_CFG                1
#define LWR_TMR_CFG_S1OVRALL_TMR_CFG              2
#define LWR_TMR_CFG_X2PREP_TMR_CFG                4
#define LWR_TMR_CFG_X2OVRALL_TMR_CFG              8
#define LWR_TMR_CFG_S1HOV_CANCELACK_TMR_CFG     16
/* ccpu00135220 */

/* bitmaps for SCHD cfg elements */
#define LWR_SCHD_CFG_DLFS_SCHD                    1

#define LWR_MAX_NUM_QCI                           9
/*****************************************************************************
 ****************************************************************************/ 

#define LWR_SRB_ID_1                              1
#define LWR_SRB_ID_2                              2
/* Element types */

#define LWR_MAX_PLMNS_PER_MME                     32 
#define LWR_MAX_GROUPS_PER_MME                    6
#define LWR_MAX_CODES_PER_MME                     6 
#define LWR_MAX_ENODEB_POOLS                      6
#define LWR_MAX_MMES                              17/* Supported MMEs are 16.
                                                       17 is to handle peer IDs as they start from 1*/
#define LWR_MAX_PLMN_IDS                          6
#define LWR_MAX_SRBS                              3
#define LWR_MAX_QCI                               256
   
#define LWR_SM_MAX_MMES                           17/* Supported MMEs are 16. 
                                                       17 is to handle peer IDs as they start from 1*/
#define LWR_SM_MAX_EAIDS                          15
#define LWR_SM_MAX_PLMN_IDS                       6

#define LWR_SM_MAX_ULSRS_PERIOD   320
#define LWR_SM_MIN_ULSRS_PERIOD  2
/*Protocol Configuration Count*/
#define LWR_MAX_PROTOCOL_CFG_COUNT  7

/* Interface Events */
#define EVTLWRCFGREQ                              1       /* Config request */
#define EVTLWRCFGCFM                              2       /* Config Confirm */
#define EVTLWRCNTRLREQ                            3       /* Control Request */
#define EVTLWRCNTRLCFM                            4       /* Control Confirm */
#define EVTLWRSTAIND                              5       /* Status Indication */
#define EVTLWRLOADIND                             6       /* Status Indication */
#define EVTLWRDYNCFGREQ                           7       /* Dynamic Config request */
#define EVTLWRDYNCFGCFM                           8       /* Dynamic Config Confirm */
#define EVTLWRNGHCFGREQ                           9       /*send neighbor enb Ip info to SM*/
#define EVTLWRNGHCFGCFM                           61       /*send neighbor enb Ip info to SM*/
#define EVTLWRADDNGHCELLCFGREQ                    62      /*send neighbor cell info to sm*/
#define EVTSMSTATICCFG                            65      /* send static cfg req */
#define EVTSMDYNCFG                               66      /* send dynamic cfg req */
#define EVTSTKSTRSTPREQ                           67      /* starting and stopping stack*/
#define EVTNGHCELLIND                             68      /*neighbour Cell indication*/

  
/*----------------- State Of SAP ------------------*/

#define LWR_SAP_UNBOUND                           1
#define LWR_SAP_BINDING                           2
#define LWR_SAP_BOUND                             3

#define EWRXXX 0


/* Measurement config Macros */
#define LWR_UMM_MEAS_EVENT_A1                     1       /* Event A1 */
#define LWR_UMM_MEAS_EVENT_A2                     2       /* Event A2 */
#define LWR_UMM_MEAS_EVENT_A4                     4       /* Event A4 */
#define LWR_UMM_MEAS_EVENT_A3                     8       /* Event A3 */
#define LWR_UMM_MEAS_EVENT_A5                     16      /* Event A5 */
#define LWR_UMM_MEAS_EUTRA_PERD                   32      /* EUTRA Periodic */
#define LWR_UMM_MEAS_1XCDMA_EVENT_B1              64      /* 1xCDMA Event B1 */
#define LWR_UMM_MEAS_CDMA_HRPD_EVENT_B1           128     /* CDMA HRPD Event B1 */
#define LWR_UMM_MEAS_1XCDMA_EVENT_B2              256     /* 1xCDMA Event B2 */
#define LWR_UMM_MEAS_CDMA_HRPD_EVENT_B2           512     /* CDMA HRPD Event B2 */
#define LWR_UMM_MEAS_1XCDMA_PERD                  1024    /* 1xCDMA Periodic */
#define LWR_UMM_MEAS_CDMA_HRPD_PERD               2048    /* CDMA HRPD Periodic */
#define LWR_UMM_MEAS_SPEED_PARAMS                 4096    /* Speed state params */
#define LWR_UMM_MEAS_MOBILITY_PARAMS              8192    /* Mobility state params */
#define LWR_UMM_MEAS_QUANTITY_PARAMS              16384   /* Quantity params*/
#define LWR_UMM_MEAS_GAP_PARAMS                   32768   /* measurement gap */
#define LWR_UMM_MEAS_PRE_REGIS_HRPD               65536   /* Pre registration Hrpd */

/**********************************************************************
 cause field values in cmAlarm
**********************************************************************/
#define LWR_CAUSE_NHU_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 1)
#define LWR_CAUSE_CTF_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 2)
#define LWR_CAUSE_RGR_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 3)
#define LWR_CAUSE_SZT_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 4)
#define LWR_CAUSE_EGT_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 5)
#define LWR_CAUSE_PJU_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 6)
#ifdef RM_INTF
#define LWR_CAUSE_RMU_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 7) /*RRM->FSM sck changes*/
#endif /*RM_INTF*/
#ifdef LTE_HO_SUPPORT
#define LWR_CAUSE_CZT_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 8)
#endif
#define LWR_CAUSE_MME_SYNC_LOST          (LCM_CAUSE_LYR_SPECIFIC + 9)
#define LWR_CAUSE_NLU_SAP_BOUND          (LCM_CAUSE_LYR_SPECIFIC + 10)


#define LWR_EVENT_CELL_CFG_SUCCESS       (LCM_CAUSE_LYR_SPECIFIC + 1)
#define LWR_EVENT_CELL_CFG_FAILED        (LCM_CAUSE_LYR_SPECIFIC + 2)
#define LWR_EVENT_S1CON_SET_SUCCESS      (LCM_CAUSE_LYR_SPECIFIC + 3)
#define LWR_EVENT_S1CON_SET_FAIL         (LCM_CAUSE_LYR_SPECIFIC + 4)
#define LWR_EVENT_ENB_CFGUPD_SUCCESS     (LCM_CAUSE_LYR_SPECIFIC + 5)
#define LWR_EVENT_ENB_CFGUPD_FAIL        (LCM_CAUSE_LYR_SPECIFIC + 6)
#define LWR_EVENT_ERRIND_FROM_PEER       (LCM_CAUSE_LYR_SPECIFIC + 7)
#define LWR_EVENT_ERR_IND_RCVD           (LCM_CAUSE_LYR_SPECIFIC + 8)
/* ccpu00122813 */
/* START*/
#define LCM_CAUSE_DUP_ECGI               (LCM_CAUSE_LYR_SPECIFIC + 9)
#define LCM_CAUSE_DUP_ENB_ID             (LCM_CAUSE_LYR_SPECIFIC + 10)
/*S1-U Interface Related Events*/
#define LWR_EVENT_S1U_PATH_UNREACHABLE   (LCM_CAUSE_LYR_SPECIFIC + 19)
#define LWR_EVENT_S1U_PATH_REACHABLE     (LCM_CAUSE_LYR_SPECIFIC + 20)
/*X2Ap Interface Related Events*/
#define LWR_EVENT_X2AP_PATH_UNREACHABLE  (LCM_CAUSE_LYR_SPECIFIC + 21)
#define LWR_EVENT_X2AP_PATH_REACHABLE    (LCM_CAUSE_LYR_SPECIFIC + 22)
#define LWR_EVENT_CELL_UNAVAILABLE       (LCM_CAUSE_LYR_SPECIFIC + 23)
#define LWR_EVENT_CELL_AVAILABLE         (LCM_CAUSE_LYR_SPECIFIC + 24)
#define LWR_EVENT_DYNAMIC_IP_CHANGE      (LCM_CAUSE_LYR_SPECIFIC + 25)
#define LWR_EVENT_SEND_ALARM             (LCM_CAUSE_LYR_SPECIFIC + 27)

/*LwrPuschCfgCommon config Macros */
#define LWR_CELL_SIB2_RRSRCMN_PUSCH_BASIC_CFG     1 
#define LWR_CELL_SIB2_RRSRCMN_PUSCH_REFSIG_CFG    2

/*IPSec reset */
#define LWR_EVENT_CELL_RECFG_SUCCESS       (LCM_CAUSE_LYR_SPECIFIC + 25)
#define LWR_EVENT_PHY_STOP_SUCCESS       (LCM_CAUSE_LYR_SPECIFIC + 26)
/* CNM_DEV */
#define LWR_EVENT_INIT_SYNC_SUCCESS      (LCM_CAUSE_LYR_SPECIFIC + 30)
#define LWR_EVENT_SYNC_NBR_CELL_UNAVAILABLE (LCM_CAUSE_LYR_SPECIFIC + 31)
#ifdef ENABLE_CNM
/* CNM Related */
#define LWR_EVENT_CNM_SYNC_NOT_POSSIBLE   (LCM_CAUSE_LYR_SPECIFIC + 28)
#endif
#define LWR_EVENT_PHY_TX_DOWN_SUCCESS       (LCM_CAUSE_LYR_SPECIFIC + 32)
#define LWR_EVENT_PHY_TX_UP_SUCCESS       (LCM_CAUSE_LYR_SPECIFIC + 33)
typedef enum wrOamNotifyType
{
    WR_OAM_NOTIFY_S1U = 1,
    WR_OAM_NOTIFY_S1MME,
    WR_OAM_NOTIFY_X2AP,
    WR_OAM_NOTIFY_MAX
}WrOamNotifyType;

 
typedef enum lwrDynCfgEvntType
{
   LWR_DYN_EVNT_ADMIN_STATE_CHANGE,
   LWR_DYN_EVNT_GENERIC_PRAMS,
   LWR_DYN_EVNT_MME_IP_PRAMS,
   LWR_DYN_EVNT_CELL_CONFIG_PRAMS,
   LWR_DYN_EVNT_ENBIP_CONFIG_PRAMS,
   LWR_DYN_EVNT_CELL_MIB_CONFIG_PRAMS,
   LWR_DYN_EVNT_PRACH_CONFIG_PRAMS,
   LWR_DYN_EVNT_RACH_CONFIG_PRAMS,
   LWR_DYN_EVNT_PDSCH_CONFIG_PRAMS,
   LWR_DYN_EVNT_SRS_CONFIG_PRAMS,
   LWR_DYN_EVNT_PUSCH_CONFIG_PRAMS,
   LWR_DYN_EVNT_ULPOWER_CONFIG_PRAMS,
   LWR_DYN_EVNT_ULFREQ_CONFIG_PRAMS,
   LWR_DYN_EVNT_PUCCH_CONFIG_PRAMS,
   LWR_DYN_EVNT_UETMRCONS_CONFIG_PRAMS,
   LWR_DYN_EVNT_CELL_SIB1_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_SIB2_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_SIB3_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_SIB4_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_SIB6_CONFIG_GROUP_PRAMS, 
   LWR_DYN_EVNT_CELL_SIB9_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_ANR_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_MEAS_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_EAID_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_SIBSCHED_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_RABPOLICY_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_TIMER_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_RRM_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_ULSCHD_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_CELL_DLSCHD_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_NEIGH_CONFIG_PRAMS,
   LWR_DYN_EVNT_NEIGH_CELL_CONFIG_PRAMS,
   LWR_DYN_EVNT_NEIGH_FREQ_CONFIG_PRAMS,
   LWR_DYN_EVNT_NEIGH_BAND_CLASS_CONFIG_PRAMS,
   LWR_DYN_EVNT_ENB_PROTO_CONFIG_PRAMS,
   LWR_DYN_EVNT_NEIGH_UTRA_FREQ_CFG,
   LWR_DYN_EVNT_NEIGH_EUTRA_FREQ_CFG,
   LWR_DYN_EVNT_NEIGH_UTRA_CELL_CFG,
   LWR_DYN_EVNT_NEIGH_EUTRA_CELL_CFG,
   LWR_DYN_EVNT_UTRANEILIST_CONFIG,
   LWR_DYN_EVNT_EUTRANEILIST_CONFIG,
   LWR_DYN_EVNT_RABSRB1_CONFIG_PARAMS,
   LWR_DYN_EVNT_RABSRB2_CONFIG_PARAMS,
   LWR_DYN_EVNT_NEIGH_ENB_CFG,
   LWR_DYN_EVNT_SM_CELL_CONFIG_PRAMS,
   LWR_DYN_EVNT_CELL_DYN_CFI_CONFIG_PRAMS,
   LWR_DYN_EVNT_CELL_SCHD_CONFIG_GROUP_PRAMS,
   LWR_DYN_EVNT_ADD_CELL_CONFIG_PRAMS,
   LWR_DYN_EVNT_FAP_ACCS_PRAMS,
   LWR_DYN_EVNT_CELL_TM_CONFIG_PRAMS,
/* *** Add any new enumaration before this Line. Should be the last enum value *** */
   LWR_DYN_EVNT_MESSENGER_CONFIGURATION_COMPLETE
} LwrDynCfgEvntType;


#define LWR_TIMER_CFG_S1SETUP             1       /* S1 setup */

   
/*----------------- State Of SAP ------------------*/

#define LWR_SAP_UNBOUND             1
#define LWR_SAP_BINDING             2
#define LWR_SAP_BOUND               3

#define ELWRXXX         0
/********************************************************
 *           Transation Id for static configuration     *
 *******************************************************/
#define LWR_TRANSID_GEN                       1000
#define LWR_TRANSID_CTF                       (LWR_TRANSID_GEN + 1)
#define LWR_TRANSID_NHU                       (LWR_TRANSID_GEN + 2)
#define LWR_TRANSID_PJU                       (LWR_TRANSID_GEN + 3)
#define LWR_TRANSID_EGT                       (LWR_TRANSID_GEN + 4)
#define LWR_TRANSID_SZT                       (LWR_TRANSID_GEN + 5)
#define LWR_TRANSID_CZT                       (LWR_TRANSID_GEN + 6)
#define LWR_TRANSID_RGR                       (LWR_TRANSID_GEN + 7)
#define LWR_TRANSID_ENBCFG                    (LWR_TRANSID_GEN + 8)
#define LWR_TRANSID_CELLCFG                   (LWR_TRANSID_GEN + 9)
#define LWR_TRANSID_MME                       (LWR_TRANSID_GEN + 10)
#define LWR_TRANSID_NHCFG                     (LWR_TRANSID_GEN + 11)
#define LWR_TRANSID_NREUTRAN                  (LWR_TRANSID_GEN + 12)
#define LWR_TRANSID_NRUTRAN                   (LWR_TRANSID_GEN + 13)
#define LWR_TRANSID_NHCELLCFG                 (LWR_TRANSID_GEN + 14)
#define LWR_TRANSID_NHUTRAN_CELLCFG           (LWR_TRANSID_GEN + 15)
#define LWR_TRANSID_INTER_NHCELLCFG           (LWR_TRANSID_GEN + 16)
#define LWR_TRANSID_CDMA1XRTT_BAND            (LWR_TRANSID_GEN + 17)
#define LWR_TRANSID_CDMA1XRTT_FREQ            (LWR_TRANSID_GEN + 18)
#define LWR_TRANSID_CDMAHRPD_CLASS            (LWR_TRANSID_GEN + 19)
#define LWR_TRANSID_CDMAHRPD_FREQ             (LWR_TRANSID_GEN + 20)
#ifdef RM_INTF
#define LWR_TRANSID_RMU                       (LWR_TRANSID_GEN + 22)
#endif /*RM_INTF*/
#define LWR_TRANSID_MME_DYN_ADDCFG            (LWR_TRANSID_GEN + 23)
#define LWR_TRANSID_MME_DYN_DELCFG            (LWR_TRANSID_GEN + 24)
#define LWR_TRANSID_ADD_NGHCELL_MEAS          (LWR_TRANSID_GEN + 25)
#define LWR_TRANSID_CDMA1XRTT_CELL            (LWR_TRANSID_GEN + 26)
#define LWR_TRANSID_GERAN_FREQ                (LWR_TRANSID_GEN + 27)
#define LWR_TRANSID_GERAN_CELL                (LWR_TRANSID_GEN + 28)
#define LWR_TRANSID_NLU                       (LWR_TRANSID_GEN + 29)
#define LWR_TRANSID_CELL_MOD                  (LWR_TRANSID_GEN + 30)
/***************************************************************
 *                       Element Macros                        *
 ***************************************************************/
#define STWRGEN                                   1
#define STWRRGRSAP                                2
#define STWRNHUSAP                                3
#define STWRCTFSAP                                4
#define STWRSZTSAP                                5
#define STWRCELLCFG                               6
#define STWRS1CONSETUP                            7
#ifdef EU_DAT_APP
#define STWREGTSAP                                8
#define STWRPJUSAP                                9
#endif 
#ifdef LTE_HO_SUPPORT
#define STWRCZTSAP                                10
#define STWRNGH                                   11
#define STWRMME                                   12
#define STWRX2CONSETUP                            0
#define MAX_NO_OF_PLMNS                           6
#endif
#define STWRSNDPARTS1RST                          15
#define STWRSNDFULLS1RST                          16
#define STWRSNDERABRELIND                         17
#define STWRTRIGX2RST                             18
#define STWRSYSTEMSTA                             19
#define STWRCELLSTA                               20
#define STWRMMESTA                                21
#define STWRSGWSTA                                22
#define STWRMMESTAPER                             23
#define STWRMMERSETCTRL                           24
#define STWRCELLSETUPSTA                          25
#define STWRUEACTSTA                              26
#define STWRANRPRINTCELL                          27
#define STWRDYNCFG                                28
#define STWRBRINGCELLDOWN                         29
#define STWRNGHCFGCFM                             30
/* CNM_DEV */
#define STWRINITALSYNC                            31
#define STWRNLUSAP                                32

#define STWRPROTOCFG                              256                /*!< Protocol configuration */
#define STWRPROTOIND                              128                /*!< Protocol configuration */
/*******************************************************
 *         Protocol macros                             *
 *******************************************************/

 
#define LWR_MAX_GU_GRP_IDS                        16
#ifdef RM_INTF
#define STWRRMUSAP         13 /*RRM->FSM sck changes*/
#endif /*RM_INTF*/


#define STWRDRBCFGDONE                            99 /* DRB configuration is completed */
 
/* operations */
#define LWR_CELLCONFIG  1

#define LWR_USTA_DGNVAL_SAP  0
#define LWR_USTA_DGNVAL_CELLUEID  1
#define LWR_USTA_DGNVAL_ALARM     2

#define LWR_CAUSE_CELL_UP  100
/* operations */
#define LWR_CELLCONFIG  1

/* lve_h_001.main_2: Added maximum file path */
#ifdef WR_SM_LOG_TO_FILE
#define LWR_MAX_FILE_PATH 256
#endif

#define MAX_GP_PREF                        16

#define ELWRXXX         0

#define ERRLWR          0

#define   ELWR001              (ERRLWR + 1)
#define   ELWR002              (ERRLWR + 2)
#define   ELWR003              (ERRLWR + 3)
#define   ELWR004              (ERRLWR + 4)
#define   ELWR005              (ERRLWR + 5)
#define   ELWR006              (ERRLWR + 6)
#define   ELWR007              (ERRLWR + 7)
#define   ELWR008              (ERRLWR + 8)
#define   ELWR009              (ERRLWR + 9)
#define   ELWR010              (ERRLWR + 10)
#define   ELWR011              (ERRLWR + 11)
#define   ELWR012              (ERRLWR + 12)
#define   ELWR013              (ERRLWR + 13)
#define   ELWR014              (ERRLWR + 14)
#define   ELWR015              (ERRLWR + 15)
#define   ELWR016              (ERRLWR + 16)
#define   ELWR017              (ERRLWR + 17)
#define   ELWR018              (ERRLWR + 18)
#define   ELWR019              (ERRLWR + 19)
#define   ELWR020              (ERRLWR + 20)
#define   ELWR021              (ERRLWR + 21)
#define   ELWR022              (ERRLWR + 22)
#define   ELWR023              (ERRLWR + 23)
#define   ELWR024              (ERRLWR + 24)
#define   ELWR025              (ERRLWR + 25)
#define   ELWR026              (ERRLWR + 26)
#define   ELWR027              (ERRLWR + 27)
#define   ELWR028              (ERRLWR + 28)
#define   ELWR029              (ERRLWR + 29)
#define   ELWR030              (ERRLWR + 30)
#define   ELWR031              (ERRLWR + 31)
#define   ELWR032              (ERRLWR + 32)
#define   ELWR033              (ERRLWR + 33)
#define   ELWR034              (ERRLWR + 34)

/* Neigh Frequency configuration related Macros */
#define LWR_MAX_BL_CELL_INFO           16
#define LWR_MAX_EUTRAN_BL_NR           16
#define LWR_MAX_CDMA_HRPD_BL_NR        16
#define LWR_MAX_CDMA_1XRTT_BL_NR       16

#define WR_CDMA_SYNC_TIME   0
#define WR_CDMA_ASYNC_TIME  1

#define MAX_NUM_FREQ_CFG 16
#define MAX_NUM_BAND_CFG 32



/*! MAX number of IP address supported by eNB.
  We can have IPv4 or IPv6, IPv4 and IPv6
 */
#define WR_MAX_IP_TYPES                2

/*! IPv4 address index. 
  IP address is stored as an array of addresses.
  IP address can be IPv4 or IPv6 or both.
  We use index 0 for storing IPv4 address in the array.
 */
#define WR_IPV4_IDX 0
/*! IPv6 address index. 
  IP address is stored as an array of addresses.
  IP address can be IPv4 or IPv6 or both.
  We use index 1 for storing IPv6 address in the array.
 */
#define WR_IPV6_IDX 1

/*! Represents invalid ip address or unused ip address. 
 */
#define WR_IP_INVALID 2

/*! IPv6 address size in bytes. 
 */
#define WR_IPV6_ADDR_LEN 16

/*! IPv4 address size in bytes. 
 */
#define WR_IPV4_ADDR_LEN 4 

#define WR_MAX_PLMN           6 
#define LWR_MAX_CONN          16


#define LWR_SEL_LC         0
#define LWR_SEL_TC         1
#define LWR_SEL_LWLC       2

//#define SM_MAX_CFG_SIZE 20000
//#define SM_MAX_CFG_SIZE 30000

#define LWR_GEN_TRANSID(_transId, _cfg, _trans)                   \
{                                                                 \
   _trans ++;                                                     \
  _transId = ((_cfg & 0x0000ffff) << 16) | (_trans & 0x0000ffff); \
}


typedef struct lwrNghENBCfg
{
  U32            peerId;          /**< Peer identifier. */
  U16            eAValue;         /**< Encryption algorithm supported by
                                          this eNodeB. */
  U16            iPAValue;        /**< Integrity algorithm supported for
                                    this eNodeB. */
  U32            priDstAddr;       /**< Primary destination address. */
  U16            dstPort;         /**< This parameter identifies the
                                    destination port number which, together 
                                    with the dstNAddr, forms
                                    the destination transport address. */
} LwrNghENBCfg;

typedef  struct lwrSmMmeCfg
{  
   U16          mmeId;
   U8           noOfIps;
   CmTptAddr    mmeAddr[LWR_MAX_IP_PER_MME];
} LwrSmMmeCfg;

typedef struct lwrEcgiCfg
{
   /* 24 bit long */
   U32 plmnId;
   /* 28 bit long */
   U32 eutranCellId;
} LwrEcgiCfg;

typedef struct lwrNghInfoCfg
{
   LwrEcgiCfg  ecgi;
   U16         physCellId;
   U32         earfcn;
}LwrNghInfoCfg;


typedef struct lwrNghUtraFddCfg
{
   U16                       rncId;
   U16                       cId;
   U16                       arfcnDl;
   U16                       arfcnUl;
   U16                       psc;        
   U16                       lac;
   U8                        rac;
   Bool                      isRimSupported; /* This variable is 
                                                 used to know whether neighbour
                                                 UTRA cell is RIM supported 
                                                 or not */                           
   S16                       pcpich;
   Bool                      voipCapable; /* This variable is used to know if the neighbour is
                                             voip capable or not? */
   Bool                      psHOCapable; /* This variable is used to know if the neighbour is
                                             PH Handover capable or not? */
}LwrNghUtraFddCfg;

typedef struct lwrNghUtraTddCfg
{
   U16                       rncId;          /* RNC Identity */
   U16                       cId;            /* Cell Identity */
   U16                       arfcn;          /* UTRA TDD ARFCN */
   U16                       lac;            /* Location Area Code */ 
   S16                       ccpchTPwr;      /* Tx power of ccpch */
   U8                        cellParamId;    /* Cell Parameter Id of TDD cell - 
                                                indicates the scrambling code */
   U8                        rac;            /* Routing Area Code */
   Bool                      isRimSupported; /* This variable is 
                                                 used to know whether neighbour
                                                 UTRA cell is RIM supported 
                                                 or not */                           
}LwrNghUtraTddCfg;

typedef struct lwrNghFreqCfg
{
   U16   arfcnDl;
   U16   arfcnUl;
}LwrNghFreqCfg;

/* Changes End*/

typedef struct lwrGuGrpIdCfg
{
   /* 24 bit long */
   U32 plmnId;
   U16 mmeGrpId;
} LwrGuGrpIdCfg;

typedef struct lwrNghCellCfg
{
   U32         enbId;       /* Enodeb ID */
   U8          enbType;     /* home / macro eNodeB */
   CmTptAddr   ipAddr;     /* IP address for Dynamic update*/
   CmTptAddr   enbAddr;     /* eNodeB IP Address */
   U16         physCellId;  /* Physical Cell ID */
   U16         tac;         /* Tracking Area Code */
   U32         cellId;      /* Cell ID */
   U32         plmnId;      /* PLMN ID - 24 bits */
   U8          nghPlmn[6];  /* Neighbor PLMN ID*/
   U8          noMnc;       /*number of MNC digits*/
   U32         peerId;      /* Peer ID */
   Bool        trgrX2;      /* flag to indicate whether 
                                          to fg eNB or not */
   U32         dlEarfcn;
   U32         ulEarfcn;
/* LTE_ADV_DSFR starts */
   RgrSfrRbRange dsfrCEFreqRange;
/* LTE_ADV_DSFR ends */
   /* CSG_DEV start */
   U8          csgAccessMode; /* CSG Access Mode */
   U32         csgId; /* CSG Identity */
   /* CSG_DEV end */
   /* CNM_DEV start */
   U8           numAntenna; /* Antenna count of the neihbour */
   U8           ulDlSubFrameCfg; /* Subfame config of the neibhour */
   U8           splSfCfg; /* Special subframe config of the neighbour */
   U8           cpDl; /* DL cyclic prefix */
   U8           cpUl; /* UL cyclic prefix */
   S16          rsrp; /* RSRP in the range of -128 to 0 */
   S16          rssi; /* RSSI in the range of 0 to 256 */
   /* CNM_DEV end */
} LwrNghCellCfg;

/** @brief the following enum defines the possible actions on the configuration
 * entities like Cell, MME, Neighbour etc.
 */
typedef enum lwrActType
{
   WR_ACT_ADD = 1,   /*!< ACTION = ADD */
   WR_ACT_MOD,       /*!< Modify */
   WR_ACT_DEL,        /*!< Delete */
   WR_ACT_IND       /* Indication from SM to layers/app */
} LwrActType;

typedef enum lwrBandwidth
{
   LWR_BANDWIDTH_6,
   LWR_BANDWIDTH_15,
   LWR_BANDWIDTH_25,
   LWR_BANDWIDTH_50,
   LWR_BANDWIDTH_75,
   LWR_BANDWIDTH_100
} LwrBandwidth;

typedef enum lwrQoffRange
{
   LWR_OFFSET_DB_MINUS_24 = 0,
   LWR_OFFSET_DB_MINUS_22,
   LWR_OFFSET_DB_MINUS_20,
   LWR_OFFSET_DB_MINUS_18,
   LWR_OFFSET_DB_MINUS_16,
   LWR_OFFSET_DB_MINUS_14,
   LWR_OFFSET_DB_MINUS_12,
   LWR_OFFSET_DB_MINUS_10,
   LWR_OFFSET_DB_MINUS_8,
   LWR_OFFSET_DB_MINUS_6,
/* LTE_ADV starts */
   LWR_OFFSET_DB_MINUS_5,
/* LTE_ADV ends */
   LWR_OFFSET_DB_MINUS_4,
/* LTE_ADV starts */
   LWR_OFFSET_DB_MINUS_3,
/* LTE_ADV ends */
   LWR_OFFSET_DB_MINUS_2,
/* LTE_ADV starts */
   LWR_OFFSET_DB_MINUS_1,
/* LTE_ADV ends */
   LWR_OFFSET_DB_ZERO_0,
   LWR_OFFSET_DB_PLUS_1,
   LWR_OFFSET_DB_PLUS_2,
   LWR_OFFSET_DB_PLUS_3,
   LWR_OFFSET_DB_PLUS_4,
   LWR_OFFSET_DB_PLUS_5,
   LWR_OFFSET_DB_PLUS_6,
   LWR_OFFSET_DB_PLUS_8,
   LWR_OFFSET_DB_PLUS_10,
   LWR_OFFSET_DB_PLUS_12,
   LWR_OFFSET_DB_PLUS_14,
   LWR_OFFSET_DB_PLUS_16,
   LWR_OFFSET_DB_PLUS_18,
   LWR_OFFSET_DB_PLUS_20,
   LWR_OFFSET_DB_PLUS_21,
   LWR_OFFSET_DB_PLUS_24
}LwrQoffRange;


/** 
 * @brief Enum defines the possible types of configurations possible for the
 * enodeB application 
 * - Cell
 * - MME
 * - Neighbour
 */
typedef enum lwrProtoCfgType
{
   WR_CFG_TYPE_CELL        = 1,   /*!< CELL Cfg */
   WR_CFG_TYPE_MME         = 2,   /*!< MME Cfg */
   WR_CFG_TYPE_NEIGH       = 3,   /*!< Neighbour Cfg */
   WR_CFG_TYPE_NEIGH_CELL  = 4,   /*!< Neighbour Cell Cfg */
   WR_CFG_TYPE_ENB         = 5,   /*!< ENodeB Cfg */
   WR_CFG_TYPE_NEIGH_FREQ  = 6,   /*!< Neighbouring frequency configuration. */
   WR_CFG_TYPE_BAND_CLASS  = 7, /*!< CDMA BandClass Info */
   WR_IND_TYPE_ENB         = 8
} LwrProtoCfgType;


/** @brief This enum defines the two states that a configured MME could be in
 *  -# ACTIVE
 *  -# IN_ACTIVE
 *  @todo add more information about the states
 */
typedef enum lwrMmeState
{
   WR_MME_STATE_ACTIVE,   /*!< MME state is Active */
   WR_MME_STATE_INACTIVE     /*!< MME state is inactive */
} LwrMmeState;

/** @brief This enum defines the administrative states that the MME could be in 
 * -# WR_MME_LOCKED - i.e. all active calls connected to the MME are dropped and
 *    new calls are not allowed. 
 * -# WR_MME_UNLOCKED - i.e. Normal state of MME, calls are allowed.
 * -# WR_MME_SHUTTING_DOWN - i.e. Keep existing calls active however new calls
 *    are not allowed to connect to the MME. 
 */
typedef enum lwrMmeAdminState
{
   WR_MME_LOCKED,                  /*!< MME is locked */
   WR_MME_UNLOCKED,                /*!< MME is unlocked */
   WR_MME_SHUTTING_DOWN            /*!< MME is shutting down */
} LwrMmeAdminState;


typedef enum lwrInetAddrType
{
   WR_INET_IPV4,
   WR_INET_IPV6
} LwrInetAddrType;


typedef struct lwrPlmnId
{
   U8             numMncDigits;   /*!< Number of digits in the MNC */
   U8             mcc[3];   /*!< Mobile country code */
   U8             mnc[3];   /*!< Mobile country code */
} LwrPlmnId;

typedef struct lwrMmePlmnCfg
{
   TknPres        pres;   /*!< Presence for fields below */
} LwrMmePlmnCfg;

typedef struct wrFreqAlarmVal
{
   U8      maxErrIndCnt;
   U8      maxResetCnt;
   U64     minErrIndTime;
   U64     minResetTime;
}WrFreqAlarmVal;


/** @brief This structure has the information necessary for the addition or
 * Reconfiguration of a MME to the EnodeB. 
 * @todo the first parameter i.e. the mmeIndex is the value that should be know
 * by both the EnodeB Application and the stack manager the reason for this
 * extra parameter is that all other identities such as mmeGi, mmeCode can
 * potentially be reconfigured hence we do need a unique identification for the
 * MME and this is the purpose served by the mmeIndex. 
 */
typedef struct lwrAddModMmeCfg
{
   U32               reCfgType;
   U8                mmeIndex;   /*!< Index of the MME being (re)configured */
   U8                peerId;   /*!< Index of the MME being (re)configured */
   U8                noOfIps; /*!< Total number of IP addresses configured */ 
   CmTptAddr         ipAddr[LWR_MAX_IP_PER_MME];     /*!< Primary IP address */
   LwrMmeState       mmeState;   /*!< State of MME */
   LwrMmeAdminState  adminState; /*!< Locked/Unlocked */
   WrFreqAlarmVal        alarmVal;
} LwrAddModMmeCfg;

/** @brief This structure defines the information needed to delete an existing
 * MME
 * @todo see if any other inputs are needed or not 
 */
typedef struct lwrDelMme
{
   U8          mmeIndex;   /*!< Index of MME to be deleted */
   U8          peerId;   /*!< Index of the MME being (re)configured */
} LwrDelMme;

typedef struct lwrMmeCfg
{
   LwrActType         action;   /*!< Config, Modify or Delete */
   union
   {
      LwrAddModMmeCfg       addMme;   /*!< Add a new MME */
      LwrAddModMmeCfg       modMme;   /*!< Modify an existing MME */
      LwrDelMme             delMme;   /*!< Delete an existing MME */
   }t;
} LwrMmeCfg;

/** @brief This Enum defines the possible values of PHICH resource i.e. Ng
 */
typedef enum lwrPhichRes
{
   WR_PHICH_RES_ONE_SIXTH,   /*!< Ng = 1/6 */
   WR_PHICH_RES_ONE_HALF,    /*!< Ng = 1/2 */
   WR_PHICH_RES_ONE,         /*!< Ng = 1 */
   WR_PHICH_RES_TWO          /*!< Ng = 2 */
} LwrPhichRes; 

/** @brief This Enum Defines the possible values of PHICH Duration.
 */
typedef enum lwrPhichDur
{
   WR_PHICH_DURATION_NORMAL,     /*!< PHICH duration is normal */
   WR_PHICH_DURATION_EXTENDED    /*!< PHICH duration is extended */
} LwrPhichDur;

/** @brief This structure defines the PHICH configuration for the cell.
 */
typedef struct lwrPhichCfg
{
   LwrPhichDur       phichDur;   /*!< PHICH Duration */
   LwrPhichRes       phichRes;   /*!< PHICH Resource */
} LwrPhichCfg;

/** @brief This structure contains the information for the MIB group of
 * information.
 * - A Bit mask conveys which parameter has changed.
 */
typedef struct lwrCellMibCfgGrp
{
   U32                  reCfgType;   /*!< Bit mask each param corresponds to a
                                       bit. 
                                       - LSB corresponds to first parameter
                                       within the group.
                                       */
   LwrBandwidth          dlBw;       /*!< Downlink Bandwidth */
   LwrPhichCfg           phichCfg;   /*!< PHICH Configuration */
} LwrCellMibCfgGrp;
/** @brief This enum describes the values for cell reservation.
 */
typedef enum lwrCellRsrvdFrOperUse
{
   CELL_RESERVED_FOR_OPER_USE,      /*!< Cell Reserved */
   CELL_NOT_RESERVED_FOR_OPER_USE   /*!< Cell not Reserved */
} LwrCellRsrvdFrOperUse;

typedef enum LwrPlmnRatPrior{
   LWR_CSFB_RAT_PRIOR_UTRA_FDD,
   LWR_CSFB_RAT_PRIOR_UTRA_TDD,
   LWR_CSFB_RAT_PRIOR_GERAN,
   LWR_CSFB_RAT_PRIOR_CDMA_CDMA2000_HRPD,
   LWR_CSFB_RAT_PRIOR_CDMA_CDMA2000_1XRTT,
   LWR_CSFB_MAX_RAT_TYPE
}LwrPlmnRatPrior;
/* @brief: This structure is used to store the PLMN RAT 
 *          priority list.
 * */
typedef struct lwrRatPriorityList
{
   U8                numberOfRats;        /*!< The number of RATs provided in the 
                                                RAT priority list. */
   LwrPlmnRatPrior   rat[LWR_CSFB_MAX_RAT_TYPE];   /*!< Store the RATs in the order of 
                                                highest priority. */
}LwrRatPriorityList;


typedef struct lwrEnbPlmnInfo
{
   LwrPlmnId             plmnId;           /*!< PLMN ID */
   LwrRatPriorityList    plmnRatPriority;  /* RAT Priority list for CSFB per PLMN */
   U8                    isRedirSupport;   /* redirection support */
} LwrEnbPlmnInfo;

/** @brief This structure PLMN Info
 */
typedef struct lwrPlmnInfo
{
   LwrPlmnId             plmnId;           /*!< PLMN ID */
   LwrCellRsrvdFrOperUse rsrvdFrOperUse;   /*!< Reserved for Operator */
} LwrPlmnInfo;


/** @brief This structure contains the cell access parameters
 */
typedef struct lwrCellAccessInfo
{
   U32             reCfgType;   /*!< Bit mask each param corresponds to a
                                           bit. 
                                           - LSB corresponds to first parameter
                                             within the group.  */
   U8               numOfPlmns;             /*!< Number of PLMNs */
   LwrPlmnInfo      plmnInfo[WR_MAX_PLMN];  /*!< List of PLMNs to be
                                                broadcast */
   U16              tac;                    /*!< Tracking area code */
   U32              cellId;                 /*!< Cell Identity 28bits LSB */
   Bool             cellBarred;             /*!< Cell is barred or not-barred */
   Bool             intraFreqReSelection;   /*!< Intra frequency
                                             reselection is allowed or
                                             not allowed. */
   Bool             csgIndication;          /*!< CSG cell or not */
   TknU32           csgIdentity;            /*!< CSG Identity 27 bits */
} LwrCellAccessInfo;

/** @brief This structure defines the parameters used for cell reselection by
 * the UE, and broadcast in SIB1
 */
typedef struct lwrCellSelectInfo
{
   S8                qRxLevMin;        /*!< Minimum RSRP level */
   TknU8             qRxLevMinOffset;  /*!< Minimum RSRP level offset */
   U8                rel9SelInfoPres;
   S8                qQualMin;        /*!< Minimum qQual */
   TknU8             qQualMinOffset;  /*!< qQual offset */
} LwrCellSelectInfo;

/** @brief This structure defines the TDD configuration.
 * - Subframe configuration {0, 1, 2, 3, 4, 5, 6}
 * - Special subframe configuration {0 , 1, 2, 3, 4, 5, 6, 7, 8}
 */
typedef struct lwrTddCfg
{
   U8       subframeCfg;          /*!< Subframe configuration {0, 1, ..} */
   U8       specialSubframeCfg;   /*!< Special subframe configuration */
} LwrTddCfg;

typedef enum lwrSiWindowSize
{
   WR_SI_WINDOWLEN_1,
   WR_SI_WINDOWLEN_2,
   WR_SI_WINDOWLEN_5,
   WR_SI_WINDOWLEN_10,
   WR_SI_WINDOWLEN_15,
   WR_SI_WINDOWLEN_20,
   WR_SI_WINDOWLEN_40
} LwrSiWindowSize;

/** @brief This structure contains the information for the SIB1 group of
 * information.
 * - A bit mask conveys which parameter is present/changed.
 */
typedef struct lwrCellSib1CfgGrp
{
   U32                   reCfgType;   /*!< Bit mask each param corresponds to a
                                       bit. 
                                       - LSB corresponds to first parameter
                                       within the group.
                                       */
   LwrCellAccessInfo     cellAccessInfo;   /*!< All information related to cell
                                            access. */
   LwrCellSelectInfo     cellSelectInfo;   /*!< Cell Selection related
                                            information */
   U8                    pMaxPres;
   S8                    pMax;             /*!< P-MAX Maximum Transmit power */
   U8                    freqBandInd;      /*!< Frequency band indicatory */
   LwrTddCfg             tddCfg;           /*!< TDD configuration */
   LwrSiWindowSize       siWindowSize;     /*!< SI window size */
} LwrCellSib1CfgGrp;

typedef struct lwrAcBarringCfg
{
   TknPres     pres;            /*!< Optional hence presence needed */
   U8          acBarringFactor; /*!< Barring factor {0, 5, 10, 15, ... 95} */
   U16         acBarringTime;   /*!< Barring Time in seconds {4, 8, ... 512} */
   Bool        acBarringAC11;   /*!< Ac Barring for Access class 11 */
   Bool        acBarringAC12;   /*!< Ac Barring for Access class 12 */
   Bool        acBarringAC13;   /*!< Ac Barring for Access class 13 */
   Bool        acBarringAC14;   /*!< Ac Barring for Access class 14 */
   Bool        acBarringAC15;   /*!< Ac Barring for Access class 15 */
} LwrAcBarringCfg;

typedef struct lwrAcBarringInfo
{
   TknPres           pres;            /*!< Optional hence presence needed */
   Bool              acBarringForEmergency;/*!< TRUE or false */
   LwrAcBarringCfg   acBarringForMOSignal; /*!< Ac Barring for MO Signalling */
   LwrAcBarringCfg   acBarringForMOData;   /*!< AC Barring for MO Data */
#ifdef DG
   Bool              barringDataUsage;   /*!< TRUE then we shall enable barring
                                           for DATA */
   Bool              barringSignalUsage; /*!< TRUE then we shall enable
                                             barring for Signalling */


#endif
} LwrAcBarringInfo;

typedef enum lwrCyclicPrefixLen
{
   WR_CYCLIC_PREFIX_NORMAL,         /*!< Normal CP len */
   WR_CYCLIC_PREFIX_EXTENDED        /*!< Extended CP len */
} LwrCyclicPrefixLen;

typedef enum lwrPwrOffstGrpB
{
   WR_POWER_OFFSET_DB_MINUS_INFNTY,
   WR_POWER_OFFSET_DB_0,
   WR_POWER_OFFSET_DB_5,
   WR_POWER_OFFSET_DB_8,
   WR_POWER_OFFSET_DB_10,
   WR_POWER_OFFSET_DB_12,
   WR_POWER_OFFSET_DB_15,
   WR_POWER_OFFSET_DB_18
} LwrPwrOffstGrpB;


typedef enum lwrCdma2kBandClassType
{
   WR_CDMA_2K_BAND_CLASS_0 = 0,
   WR_CDMA_2K_BAND_CLASS_1 = 1,
   WR_CDMA_2K_BAND_CLASS_2 = 2,
   WR_CDMA_2K_BAND_CLASS_3 = 3,
   WR_CDMA_2K_BAND_CLASS_4 = 4,
   WR_CDMA_2K_BAND_CLASS_5 = 5,
   WR_CDMA_2K_BAND_CLASS_6 = 6,
   WR_CDMA_2K_BAND_CLASS_7 = 7,
   WR_CDMA_2K_BAND_CLASS_8 = 8,
   WR_CDMA_2K_BAND_CLASS_9 = 9,
   WR_CDMA_2K_BAND_CLASS_10 = 10,
   WR_CDMA_2K_BAND_CLASS_11 = 11,
   WR_CDMA_2K_BAND_CLASS_12 = 12,
   WR_CDMA_2K_BAND_CLASS_13 = 13,
   WR_CDMA_2K_BAND_CLASS_14 = 14,
   WR_CDMA_2K_BAND_CLASS_15 = 15,
   WR_CDMA_2K_BAND_CLASS_16 = 16,
   WR_CDMA_2K_BAND_CLASS_17 = 17,
   WR_CDMA_2K_BAND_CLASS_18 = 18,
   WR_CDMA_2K_BAND_CLASS_19 = 19,
   WR_CDMA_2K_BAND_CLASS_20 = 20,
   WR_CDMA_2K_BAND_CLASS_21 = 21,
   WR_CDMA_2K_BAND_CLASS_SPARE_10 = 22,
   WR_CDMA_2K_BAND_CLASS_SPARE_9 = 23,
   WR_CDMA_2K_BAND_CLASS_SPARE_8 = 24,
   WR_CDMA_2K_BAND_CLASS_SPARE_7 = 25,
   WR_CDMA_2K_BAND_CLASS_SPARE_6 = 26,
   WR_CDMA_2K_BAND_CLASS_SPARE_5 = 27,
   WR_CDMA_2K_BAND_CLASS_SPARE_4 = 28,
   WR_CDMA_2K_BAND_CLASS_SPARE_3 = 29,
   WR_CDMA_2K_BAND_CLASS_SPARE_2 = 30,
   WR_CDMA_2K_BAND_CLASS_SPARE_1 = 31
} LwrCdma2kBandClassType;

typedef struct lwrCdma2k1xMobParams
{
   Bool sidIncl; 
   U16  sid;              /* System identifier */
   Bool nidIncl; 
   U16  nid;              /* Network identifier */
   Bool multSidsIncl;  
   Bool multSids;         /* Multiple SID storage indicator */
   Bool multNidsIncl;  
   Bool multNids;         /* Multiple NID storage indicator */
   Bool regZoneIncl;   
   U16  regZone;          /* Registration zone number */
   Bool totalZonesIncl;
   U8   totalZones;       /* Total number of registration zones */
   Bool zoneTimerIncl;  
   U8   zoneTimer;        /* Zone timer */
   Bool pktZoneIdIncl;  
   U8   pktZoneId;        /* Pakcet data services zone identifier */
   Bool pzIdHysParamIncl;
   Bool pzHystEnabled;    /* Packet zone hysteris enabled */
   Bool pzHystInfoIncl;   /* Pkt zone Hys Info Included indicator */
   Bool pzHystListLen;    /* Pkt zone Hys list length */
   U8   pzHystActTimer;   /* Pkt zone Hys Activation timer */
   U8   pzHystTimerMul;   /* Pkt zone Hys timer multiplier */
   U8   pzHystTimerExp;   /* Pkt zone hys timer exponent */
   Bool pRevIncl;    
   U8   pRev;             /* Protocol revision level */
   Bool minPRevIncl;
   U8   minPRev;          /* Min protocol revision level */
   Bool negSlotCycIndxSupIncl; 
   Bool negSlotCycIndxSup; /* Negative slot cycle index supported indicator */
   Bool encModeIncl;
   U8   encMode;          /* Message encryption mode */
   Bool encSupIncl;
   Bool encSup;           /* Encryption fields included */
   Bool sigEncSupIncl;
   U8   sigEncSup;        /* Signalling encryption supported indicator */
   Bool msgIntSupIncl;
   Bool msgIntSup;        /* Message integrity supported indicator */
   Bool sigIntSupINCLIncl; 
   Bool sigIntSupINCL;    /* Signalling message integrity info included ind */
   Bool sigIntSupIncl;
   U8   sigIntSup;        /* Signalling integrity algo supported */
   Bool authIncl;
   U8   auth;             /* Authentication mode */
   Bool maxNumAltoSoIncl;
   U8   maxNumAltoSo;     /* Max numb of alternative service options */
   Bool useSyncIdIncl;
   Bool useSyncId;        /* Sync ID supported indicator */
   Bool msInitPosLocSupIndIncl;
   /* MS initiated position location determination supported ind */
   Bool msInitPosLocSupInd; 
   Bool mobQosIncl;
   Bool mobQos;           /* QoS record */        
   Bool bandClsInfoReqIncl;
   Bool bandClsInfoReq;   /* Band class info request indicator */
   Bool bandClsIncl;
   U8   bandCls;          /* Band class */
   Bool bypassRegIndIncl;
   U8   bypassRegInd;     /* Bypass powerup reg Ind */
   Bool altBandClsIncl;
   U8   altBandCls;       /* Alternate band class */
   Bool maxAddServInstIncl; 
   U8   maxAddServInst;   /* Max num of additional serv ref ids allowed */
   Bool homeRegIncl;
   Bool homeReg;          /* Home reg indicator */
   Bool forSidRegIncl;
   Bool forSidReg;        /* SID roamer registration indicator */
   Bool forNidRegIncl;
   Bool forNidReg;        /* NID roamer registration indicator */
   Bool powerUpRegIncl;
   Bool powerUpReg;       /* Power up registration indicator */
   Bool powerDownRegIncl;
   Bool powerDownReg;     /* Power down registration indicator */
   Bool parmRegIncl;
   Bool parmReg;          /* Parameter change reg indicator */
   Bool regPrdIncl;
   U8   regPrd;           /* Registration period */
   Bool regDistIncl;
   U16  regDist;          /* Registration distance */
   Bool prefMsIdTypeIncl;
   U8   prefMsIdType;     /* Pref enhanced access channel MS ID Type */
   Bool extPrefMsIdTypeIncl;
   U8   extPrefMsIdType;  /* Ext Pref enhanced access channel MS ID Type */
   Bool meIdReqdIncl;
   Bool meIdReqd;         /* MEID Required indicator */
   Bool mccIncl;
   U16  mcc;              /* MCC */
   Bool imsi1112Incl;
   U8   imsi1112;         /* 11t and 12th digits of IMSI */
   Bool imsiTSupIncl;
   Bool imsiTSup;         /* IMSI_T support indicator */
   Bool recntMsgIndIncl;
   Bool recntMsgInd;      /* Reconnect message support indicator */
   Bool rerModeSupIncl; 
   Bool rerModeSup;       /* Radio env reporting mode support indicator */
   Bool tzkModeSupIncl;
   Bool tzkModeSup;       /* Tracking zone mode indicator */    
   Bool tzkIdIncl;
   Bool tzkId;            /* Tracking zone identifier */
   Bool pilRepIncl;
   Bool pilRep;           /* Pilot reporting indicator */
   Bool sdbSupIncl;
   Bool sdbSup;           /* Short data burst supported indicator */
   Bool autoFcsoAllIncl;
   Bool autoFcsoAll;      /* Autonomous fast call setup order allowed ind */
   Bool sdbInRcnmIncl;
   Bool sdbInRcnm;        /* Short data burst allowed in recon msg ind */
   Bool fpcFchIncl;       /* Fundamental Channel included indicator */
   U8   fpcFchInitSetptRC3; /* Initial Fundamental Channel outer loop 
                               Eb/Nt setpoint with RC3 */
   U8   fpcFchInitSetptRC4; /* Initial Fundamental Channel outer loop 
                               Eb/Nt setpoint with RC4 */
   U8   fpcFchInitSetptRC5; /* Initial Fundamental Channel outer loop 
                               Eb/Nt setpoint with RC5 */
   U8   fpcFchInitSetptRC11; /* Initial Fundamental Channel outer loop
                                Eb/Nt setpoint with RC11 */
   U8   fpcFchInitSetptRC12; /* Initial Fundamental Channel outer loop 
                                Eb/Nt setpoint with RC12 */
   Bool tAddIncl;
   U8   tAdd;               /* Pilot detection threshold */
   Bool pilotIncIncl;
   U8   PilotInc;          /* Pilot PN sequence offset index increment */
   Bool randIncl;
   U32  rand;             /* Random challenge */
   Bool lpSecIncl;
   U8   lpSec;            /* Number  of  leap  seconds */
   Bool ltmOffIncl;
   U8   ltmOff;           /* Offset of local time from system time */
   Bool dayltIncl;
   Bool daylt;            /* Daylight savings time indicator */
   Bool gcsnaL2ActTimerIncl;
   U8   gcsnaL2ActTimer;   /* GCSNAL2AckTimer */
   Bool gcsnaSeqCtxtTimerIncl;
   U8   gcsnaSeqCtxtTimer; /* GCSNASequenceContextTimer */
   U8   reserved;
}LwrCdma2k1xMobParams;

typedef struct lwrPreambleCfg
{
   U8             numOfRAPreambles;   /*!< Number of RA preambles BCAST */
   U8             groupASize;   /*!< Group A size */
   U16            msgSizeGroupA;   /*!< Message size group A */
   LwrPwrOffstGrpB pwrOffstGrpB;            /*!< Group B message power offset */
} LwrPreambleCfg;

typedef struct lwrPowerRamping
{
   U8             powerRampingstep;   /*!< Power Ramping step in db */
   S16            initRcvdTrgtPwr;    /*!< Initial preamble power */
} LwrPowerRamping;

typedef struct lwrRaSupervisionCfg
{
   U8          preambleTransMax;    /*!< Maximum preamble transmisssions */
   U8          rarWindowSize;       /*!< RA response window size */
   U8          macContResTimer;     /*!< MAC contention resolution timer */
} LwrRaSupervisionCfg;

typedef struct lwrRachCfgCommon
{
   U32                  reCfgType;   /*!< Bit mask each param corresponds to a
                                       bit. 
                                       - LSB corresponds to first parameter
                                       within the group.
                                       */
   LwrPreambleCfg        preambleCfg;      /*!< Preamble Configuration */
   LwrPowerRamping       powerRamping;     /*!< Power ramping config */
   LwrRaSupervisionCfg   raSupervisionCfg; /*!< RA Suppervision info */
   U8                    maxHARQMsg3;      /*!< Msg3 HARQ retransmissions */
} LwrRachCfgCommon;

/** @brief This structure gives the modification coeffcient
 *  defines the modification coeff actual modification period is modCoeff
 *  default paging cycle.
 */
typedef struct lwrBCCHCfgCommon
{
   U8       modPeriodCoeff;   /*!< Modification period */
} LwrBCCHCfgCommon;

/** @brief This Enum defines the values of nB as per spec 36.304 Paging section.
 */
typedef enum lwrnB
{
   WR_FOUR_T,
   WR_TWO_T,    
   WR_ONE_T,    
   WR_HALF_T,    
   WR_QUATER_T,    
   WR_EIGTH_T,    
   WR_SIXTEENTH_T,    
   WR_THIRTYTWOTH_T    
} LwrnB;

/** @brief This Enum defines the values of nB as per spec 36.304 Paging section.
 */
typedef enum lwrDefaultPagingCycle
{
   WR_RF_32,
   WR_RF_64,
   WR_RF_128,
   WR_RF_256
} LwrDefaultPagingCycle;

typedef struct lwrPCCHCfgCommon
{
   LwrDefaultPagingCycle    defaultPagingCycle;   /*!< Default paging cycle */
   LwrnB                    nB;                   /*!< nB as per spec 36.304 */
} LwrPCCHCfgCommon;

/** @brief This structure defines the PDSCH configuration that is broadcast as
 * part of SIB 2.
 * @todo this values are to be determined by SON/Application and is not expected
 * to be populated over the OAM interface. 
 */
typedef struct lwrPdschCfgCommon
{
   S8          referenceSignalPwr;   /*!< Reference signal power {-60..50} */
   U8          pB;                   /*!< p-b */
} LwrPdschCfgCommon;

typedef enum lwrPuschHoppingMode
{
   WR_INTER_SUBFRAME_HOPPING,
   WR_INTRA_SUBFRAME_HOPPING
} LwrPuschHoppingMode;

typedef struct lwrPuschBasicCfgCommon
{
   U32                  reCfgType;   /*!< Bit mask each param corresponds to a
                                       bit. 
                                       - LSB corresponds to first parameter
                                       within the group.
                                       */
   U8                   nSb;          /*!< n-SB i.e. number of Subbands 36.211[5.3.4] */
   LwrPuschHoppingMode hoppingMode;   /*!< PUSCH hopping mode */
   U8                  hoppingOffset; /*!< PUSCH hopping offset */
   Bool                enable64QAM;   /*!< Enable 64 QAM or not */
} LwrPuschBasicCfgCommon;

 typedef struct lwrPuschRefSignalCfg
{
   U32                reCfgType;   /*!< Bit mask each param corresponds to a
                                       bit. 
                                       - LSB corresponds to first parameter
                                       within the group.*/
   Bool        grpHopEnabled; /*!< Boolean if group hopping is enabled or not*/
   U8          grpAsgnPusch;  /*!< Group assignment PUSCH i.e. f(ss) in 36.211*/
   Bool        seqHopEnabled; /*!< Sequence hopping enabled or not */
   U8          cyclicShift;   /*!< Value of N1(DMRS) 36.211 */
} LwrPuschRefSignalCfg;

/** @brief This structure Defines the common PUSCH configuraiton that contains
 * the Basic configuration for PUSCH and also configuration for Reference
 * signals in the UL direction.
 */
typedef struct lwrPuschCfgCommon
{
   U32                reCfgType;   /*!< Bit mask each param corresponds to a
                                       bit. 
                                       - LSB corresponds to first parameter
                                       within the group.
                                       */
   LwrPuschBasicCfgCommon puschBasicCfg;   /*!< Basic PUSCH configuration */
   LwrPuschRefSignalCfg   puschRefSigCfg;   /*!< UL Reference signal
                                             configuration */
} LwrPuschCfgCommon;

typedef enum lwrDeltaPucchShift
{
   WR_DELTA_PUCCH_SHIFT_1,
   WR_DELTA_PUCCH_SHIFT_2,
   WR_DELTA_PUCCH_SHIFT_3
} LwrDeltaPucchShift;

/** @brief This structure Defines the pucch configuration that is common to all
 * UEs and broadcast in the SIB
 */
typedef struct lwrPucchCfgCommon
{
   LwrDeltaPucchShift    deltaShift;   /*!< @todo self PUCCH Delta shift */
   U8                    nRBCqi;       /*!< number of RBs to be used for PUCCH 2/2a..
                                       */
   U8                    nCSAN;        /*!< Cyclic shift determines the number of
                                           mixed RBs */
   U16                   n1PucchAn;    /*!< N1(PUCCH) */
} LwrPucchCfgCommon;

typedef struct lwrSRSCfgCommon
{
   U8          cSrs;             /*!< SRS bandwidth = cSrs */
   U8          subframeCfg;      /*!< SRS subframe configuration */
   Bool        simulAckNackSrs;  /*!< Simultaneous ACK/NACK SRS */
   Bool        srsMaxUpts;       /*!< TDD specific */
} LwrSRSCfgCommon;

typedef enum lwrULPwrAlpha
{
   WR_UL_PWR_ALPHA_0,
   WR_UL_PWR_ALPHA_04,
   WR_UL_PWR_ALPHA_05,
   WR_UL_PWR_ALPHA_06,
   WR_UL_PWR_ALPHA_07,
   WR_UL_PWR_ALPHA_08,
   WR_UL_PWR_ALPHA_09,
   WR_UL_PWR_ALPHA_1
} LwrULPwrAlpha;

typedef enum lwrDeltaPUCCHF
{
   WR_PUCCH_F_DELTA_min_2,
   WR_PUCCH_F_DELTA_0,
   WR_PUCCH_F_DELTA_1,
   WR_PUCCH_F_DELTA_2,
   WR_PUCCH_F_DELTA_3,
   WR_PUCCH_F_DELTA_5    
} LwrDeltaPUCCHF;

typedef struct lwrDeltaPUCCHLst
{
   LwrDeltaPUCCHF  deltaPucchFormat1;      /*!< Format 1 */
   LwrDeltaPUCCHF  deltaPucchFormat1b;     /*!< Format 1b */
   LwrDeltaPUCCHF  deltaPucchFormat2;      /*!< Format 2 */
   LwrDeltaPUCCHF  deltaPucchFormat2a;     /*!< Format 2a */
   LwrDeltaPUCCHF  deltaPucchFormat2b;     /*!< Format 2b */
} LwrDeltaPUCCHLst;

/** @brief This structure defines the UL power control common parameters that
 * are broadcast
 */
typedef struct lwrULPwrCtrlCommon
{
   S8                   pNomialPusch;   /*!< P0_Nominal_PUSCH */
   LwrULPwrAlpha        alpha;    /*!< Alpha 36.213 */
   S8                   pNomialPucch;   /*!< P0_Nominal_PUCCH */
   LwrDeltaPUCCHLst     deltaFLst;   /*!< Delta PUCCH for formats */
   S8                   deltaMsg3;   /*!< Delta Preamble Msg3 */
} LwrULPwrCtrlCommon;

typedef struct lwrPrachCfgInfo
{
   U8       prachCfgIdx;            /*!< PRACH config index determines format and RA
                                    occassions */
   Bool     highSpeedFlag;          /*!< TRUE if high speed is enabled */
   U8       zeroCorrelationZoneCfg; /*!< Zero Correlation zone */
   U8       prachFreqOffset;        /*!< PRACH frequency offset */
} LwrPrachCfgInfo;

typedef struct lwrPrachCfgCommon
{
   U16              rootSequenceIndx;   /*!< ROOT sequence for preambles */
   LwrPrachCfgInfo  prachCfgInfo;           /*!< PRACH configuration */
   U8               prachPosition;      /*!< PRACH Position */
} LwrPrachCfgCommon;

typedef struct lwrRadioResourceCommon
{
   U32                   reCfgType;   /*!< Bit mask each param corresponds to a
                                       bit. 
                                       - LSB corresponds to first parameter
                                       within the group.
                                       */
   LwrRachCfgCommon      rachCfgCommon;   /*!< RACH common configuration */
   LwrBCCHCfgCommon      bcchCfgCommon;   /*!< BCCH common configuration */
   LwrPCCHCfgCommon      pcchCfgCommon;   /*!< PCCH common configuration */
   LwrPrachCfgCommon     prachCfgCommon;  /*!< PRACH common configuration */
   LwrPdschCfgCommon     pdschCfgCommon;  /*!< @todo PDSCH common configuration */
   LwrPuschCfgCommon     puschCfgCommon;  /*!< PUSCH common configuration */
   LwrPucchCfgCommon     pucchCfgCommon;  /*!< PUCCH common configuration */
   LwrSRSCfgCommon       srsCfgCommon;    /*!< SRS common configuration */
   LwrULPwrCtrlCommon    ulPwrCtrlCommon; /*!< UL power control */
   LwrCyclicPrefixLen    ulCyclicPrefix;  /*!< UL cyclic prefix */
   LwrCyclicPrefixLen    dlCyclicPrefix;  /*!< UL cyclic prefix */
} LwrRadioResourceCommon;

typedef struct lwrUeTimerConstants
{
   U16         t300;   /*!< RRC Connection Request timer */
   U16         t301;   /*!< RRC Connection reestablishnment timer */
   U16         t302;   /*!< RRC timer */
   U16         t304;   /*!< RRC timer */
   U16         ccoT304;   /*!< RRC timer */
   U16         t310;   /*!< RRC timer */
   U16         n310;   /*!< RRC timer */
   U16         t311;   /*!< RRC timer */
   U16         n311;   /*!< RRC timer */
   U16         t320;   /*!< RRC timer */
} LwrUeTimerConstants;

typedef struct lwrULFrequencyInfo
{
   U16               eARFCN;             /*!< ARFCN for the cell */
   LwrBandwidth      ulBandwidth;        /*!< UL bandwidth */
   U8                addSpectEmission;   /*!< Additional Spectrum emission */
} LwrULFrequencyInfo;

typedef enum lwrTimeAlignment
{
   WR_TIME_ALIGNMENT_TIMER_500,
   WR_TIME_ALIGNMENT_TIMER_750,
   WR_TIME_ALIGNMENT_TIMER_1280,
   WR_TIME_ALIGNMENT_TIMER_1920,
   WR_TIME_ALIGNMENT_TIMER_2560,
   WR_TIME_ALIGNMENT_TIMER_5120,
   WR_TIME_ALIGNMENT_TIMER_10240,
   WR_TIME_ALIGNMENT_TIMER_INFINITE
} LwrTimeAlignment;

typedef struct lwrCellSib2CfgGrp
{
   U32                      reCfgType; /*!< Bit mask each param corresponds to a
                                       bit. 
                                       - LSB corresponds to first parameter
                                       within the group.
                                       */
   LwrAcBarringInfo         acBarringInfo;   /*!< Access barring information */
   LwrRadioResourceCommon   radioResourceCommon;   /*!< Radio resource common */
   LwrUeTimerConstants      ueTimerConstants;   /*!< Timer and Constants */
   LwrULFrequencyInfo       ulFreqInfo;   /*!< UL Frequency and BW */
   LwrTimeAlignment         timeAlignmentCommon;   /*!< Timming alignment common
                                                    */
   LwrAcBarringCfg          acBarringVoice;   /*!< AC Barring for voice */
   LwrAcBarringCfg          acBarringVideo;   /*!< AC Barring for video */
} LwrCellSib2CfgGrp;

/** @brief This structure used to get Measurement Mobilty State Params from EMM.
 * @details These are the structure members
  - U16           speedStaeReSelParsUsage       speed State ReSelec Params Usage
  - U16           tEvalulation                  evaluation time of Cell in
                                               basestation
  - U16           tHystNormal                   hyst normal time of Cell in 
                                               basestation.
  - U16           nCellChangeMedium             number of cell change in medium
                                               mobility state value of Cell in
                                                basestation
  - U16           nCellChangeHigh               number of cell change in high
                                               mobility state value of Cell in
                                              basestation
 * 
 * */
typedef struct lwrMeasMobilityStatParams
{
   TknPres                  speedStatPres;
   U16                      tEvalulation;
   U16                      tHystNormal;
   U16                      nCellChangeMedium;
   U16                      nCellChangeHigh;
} LwrMeasMobilityStatParams;

/** @brief This structure used to get Measurement Info parameters from EMM.
 * @details These are the structure members
   - U8        sMeasure                S-Measure
   - U8        sfUsage                 Whether to use Scaling factor
   - U8        timeToTriggerSfMedium   Medium Speed State Scale Factors
   - U8        timeToTriggerSfHigh     High Speed State Scale Factors
 *
 *  */
typedef struct lwrMeasSpdStateScaleFact
{
   U8                       sMeasure;
   TknPres                  sfUsagePres;
   U8                       timeToTriggerSfMedium;
   U8                       timeToTriggerSfHigh;
} LwrMeasSpdStateScaleFact;

/** @brief This structure used to get Quality Config Meas OAM Params.
 * @details These are the structure members
 - U8       rsrpFltrCoeff         RSRP Filter Coefficient
 - U8       rsrqFltrCoeff         RSRQ Filter Coefficient
 - U8       measQtyCDMA2k         CDMA2k Meas Quantity
 *  */
typedef struct lwrMeasQtyCfgParams
{
   U8                        rsrpFltrCoeff;
   U8                        rsrqFltrCoeff;
   U8                        measQtyCDMA2k;
   U8                          ecnoFltrCoeff;
   U8                        measQtyUtraFdd;
   U8                        measQtyUtraTdd;
   U8                        fltrCoeffGERAN; /* Filtering coefficient used for GERAN measurement */
   U8                        filterCoefficient2FDDr10;
} LwrMeasQtyCfgParams;
typedef enum lwrMeasQtyCfgGeran
{
   WR_MEAS_QTY_CFG_GERAN_RSSI
}LwrMeasQtyCfgGeran;
/** @brief This structure used to get Pre Registration Hrpd information.
 * @details These are the structure members
 - Bool         preRegAllowed               Whether to use CDMA2000 HRPD pre-
                                            registration of the cell in the eNB
 - U8           preRegZoneId                The preRegistration zone ID of the
                                            cell in the eNB.
 - U8           NumOf2ndPreRegZoneIdList    The secondary pre-registration zone
                                            ID list count of the cell in eNB.
 - U8           secondaryPreRegZoneId1      The first secondary pre-registration
                                            zone ID of the cell in the eNB.
 - U8           secondaryPreRegZoneId2      The second secondary pre-
                                            registration zone ID of the cell in
                                            the eNB.
 *  */
typedef struct lwrMeasPreRegHrpd
{
   U8                        preRegAllowed;
   U8                        preRegZoneId;
   U8                        NumOf2ndPreRegZoneIdList;
   U8                        secondaryPreRegZoneId1;
   U8                        secondaryPreRegZoneId2;
} LwrMeasPreRegHrpd;

/** @brief This structure used to get Gap Pattern information from OAM.
 * @details These are the structure members
   - U32        gapPatternForInterFA     measurement gap pattern information
                                         used in Inter-frequency measurement.
   - U32        gapPatternForInterRAT    measurement gap pattern information
                                         used in Inter-RAT measurement
   - U32        gapUseForANR             Whether to use measurement gap for SON
                                           ANR function
   - U32        gapPatternForANR         measurement gap pattern information
                                         for SON ANR.
 *  */
typedef struct lwrMeasGapConfig
{
   U32                      gapPatternForInterFA;
   U32                      gapPatternForInterRAT;
   TknPres                  gapForANRPres;
   U32                      gapPatternForANR;
} LwrMeasGapConfig;

#define LWR_MEAS_MAX_EVENT_A1_CFG         2
/** @brief This enum defines the purpose to use Event A1
 * the following have been defined
 * -# Deactivate measurement Gap for Event A1 inter frequency handover RSRP
 * -# Deactivate measurement Gap for Event A1 inter frequency handover RSRQ
 * -# Reserved1
 * -# Reserved2
 */
typedef enum lwrMeasEvntA1Type
{
   WR_MEAS_EVENTA1_HO_RSRP,
   WR_MEAS_EVENTA1_HO_RSRQ,
   WR_MEAS_EVENTA1_RESV1,
   WR_MEAS_EVENTA1_RESV2
} LwrMeasEvntA1Type;

#define LWR_MEAS_MAX_EVENT_A2_CFG         2
/** @brief This enum defines the purpose to use Event A2
 * the following have been defined
 * -# Activate measurement Gap for Event A2 inter frequency handover RSRP
 * -# Activate measurement Gap for Event A2 inter frequency handover RSRQ
 * -# Activate measurement Gap for Event A2 inter frequency ANR RSRP
 * -# Activate measurement Gap for Event A2 inter frequency ANR RSRQ
 * -# Activate measurement Gap for Event A2 non-Optimized HRPD handover RSRP
 * -# Activate measurement Gap for Event A2 non-Optimized HRPD handover RSRQ
 * -# Activate measurement Gap for Event A2 Optimized HRPD handover RSRP
 * -# Activate measurement Gap for Event A2 Optimized HRPD handover RSRQ
 * -# Activate measurement Gap for Event A2 ANR Hrpd RSRP
 * -# Activate measurement Gap for Event A2 ANR Hrpd RSRQ
 * -# Reserved1
 * -# Reserved2
 */
typedef enum lwrMeasEvntA2Type
{
   WR_MEAS_EVENTA2_HO_RSRP,
   WR_MEAS_EVENTA2_HO_RSRQ,
   WR_MEAS_EVENTA2_RESV1,
   WR_MEAS_EVENTA2_RESV2
} LwrMeasEvntA2Type;

/* LTE_ADV starts */
#define LWR_MEAS_MAX_EVENT_A3_CFG         6
/* LTE_ADV ends */
/** @brief This enum defines the purpose to use Event A3
 * the following have been defined
 * -# Intra Frequency Handover for Event A3
 * -# Inter Frequency RSRP Handover for Event A3
 * -# Inter Frequency RSRQ Handover for Event A3
 * -# Intra Frequency ANR for Event A3
 * -# Inter Frequency ANR for Event A3
 * -# Reserved1
 * -# Reserved2
 */
typedef enum lwrMeasEvntA3Type
{
   WR_MEAS_EVENTA3_INTRA_FREQ_HO,
   WR_MEAS_EVENTA3_INTER_FREQ_HO_RSRP,
   WR_MEAS_EVENTA3_INTER_FREQ_HO_RSRQ,
   WR_MEAS_EVENTA3_INTRA_FREQ_ANR,
   WR_MEAS_EVENTA3_INTER_FREQ_ANR,
/* LTE_ADV starts */
   WR_MEAS_EVENTA3_INTRA_FREQ_ABS,
/* LTE_ADV ends */
   WR_MEAS_EVENTA3_RESV1,
   WR_MEAS_EVENTA3_RESV2
} LwrMeasEvntA3Type;

#define LWR_MEAS_MAX_EVENT_A4_CFG         1
/** @brief This enum defines the purpose to use Event A4
 * the following have been defined
 * -# Anr Specific for Event A4
 * -# Reserved1
 * -# Reserved2
 */
typedef enum lwrMeasEvntA4Type
{
   WR_MEAS_EVENTA4_ANR_SPECIFIC,
   WR_MEAS_EVENTA4_RESV1,
   WR_MEAS_EVENTA4_RESV2
} LwrMeasEvntA4Type;

#define LWR_MEAS_MAX_EVENT_A5_CFG         5
/** @brief This enum defines the purpose to use Event A5
 * the following have been defined
 * -# Intra Frequency Handover for Event A5
 * -# Inter Frequency RSRP Handover for Event A5
 * -# Inter Frequency RSRQ Handover for Event A5
 * -# Intra Frequency ANR for Event A5
 * -# Inter Frequency ANR for Event A5
 * -# Reserved1
 * -# Reserved2
 */
typedef enum lwrMeasEvntA5Type
{
   WR_MEAS_EVENTA5_INTRA_FREQ_HO,
   WR_MEAS_EVENTA5_INTER_FREQ_HO_RSRP,
   WR_MEAS_EVENTA5_INTER_FREQ_HO_RSRQ,
   WR_MEAS_EVENTA5_INTRA_FREQ_ANR,
   WR_MEAS_EVENTA5_INTER_FREQ_ANR,
   WR_MEAS_EVENTA5_RESV1,
   WR_MEAS_EVENTA5_RESV2
} LwrMeasEvntA5Type;

#define LWR_MEAS_MAX_EUTRA_PERD_CFG       2
/** @brief This enum defines the purpose to use EUTRA Periodic
 * the following have been defined
 * -# Eutra periodic report strongest cells
 * -# Eutra periodic report CGI
 * -# Reserved2
 */
typedef enum lwrMeasEutraPeriodic
{
   WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS,
   WR_MEAS_EUTRA_PERIODIC_REPORT_CGI,
   WR_MEAS_EUTRA_PERIODIC_RESV1
} LwrMeasEutraPeriodic;

#define LWR_MEAS_MAX_EVENT_B1_CFG         1
/** @brief This enum defines the purpose to use Event B1
 * the following have been defined
 * -# Anr Specific for Event B1
 * -# Reserved1
 * -# Reserved2
 */
typedef enum lwrMeasEvntB1Type
{
   WR_MEAS_EVENTB1_ANR_SPECIFIC,
   WR_MEAS_EVENTB1_RESV1,
   WR_MEAS_EVENTB1_RESV2
} LwrMeasEvntB1Type;

#define LWR_MEAS_MAX_UTRA_B1_CFG          3
/** @brief This enum defines the purpose to use Event B1
 * the following have been defined for UTRA
 * -# Anr Specific for Event B1
 * -# HO Specific for Event B1
 * -# CSFB Specific for Event B1
 * -# Reserved1
 */
typedef enum lwrMeasUtraB1Type
{
   WR_MEAS_UTRA_EVENTB1_HO,
   WR_MEAS_UTRA_EVENTB1_CSFB,
   WR_MEAS_UTRA_EVENTB1_ANR,
   WR_MEAS_UTRA_EVENTB1_RESV1  
}LwrMeasUtraB1Type;

#define LWR_MEAS_MAX_UTRA_EVNT_B2_CFG     4
/** @brief This enum defines the purpose to use Event B2
 * the following have been defined for UTRA
 * -# Anr Specific for Event B2
 * -# HO Specific for Event B2
 * -# CSFB Specific for Event B2
 * -# Reserved1
 */
typedef enum lwrMeasUtraB2Type
{
   WR_MEAS_UTRA_EVENTB2_HO,
   WR_MEAS_UTRA_EVENTB2_CSFB,
   WR_MEAS_UTRA_EVENTB2_ANR,
   WR_MEAS_UTRA_EVENTB2_REDIR,
   WR_MEAS_UTRA_EVENTB2_RESV1
}LwrMeasUtraB2Type;

#define LWR_MEAS_MAX_GERAN_EVNT_B2_CFG    2
/** @brief This enum defines the purpose to use GERAN Event B2
 * the following have been defined
 * -# 1 Geran Event B2 interRAT Handover
 * -# Reserved1
 * -# Reserved2
 */
typedef enum lwrMeasGeranEvntB2Type
{
   WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR,
   WR_MEAS_GERAN_EVENTB2_INTER_RAT_HO,
   WR_MEAS_GERAN_EVENTB2_RESV2
} LwrMeasGeranEvntB2Type;

#define LWR_MEAS_MAX_1XCDMA_EVENT_B2_CFG  1
/** @brief This enum defines the purpose to use 1xRTT CDMA Event B2
 * the following have been defined
 * -# 1xCdma Event B2 interRAT Handover
 * -# Reserved1
 * -# Reserved2
 */
typedef enum lwrMeas1xCdmaEvntB2Type
{
   WR_MEAS_1XCDMA_EVENTB2_INTER_RAT_HO,
   WR_MEAS_1XCDMA_EVENTB2_RESV1,
   WR_MEAS_1XCDMA_EVENTB2_RESV2
} LwrMeas1xCdmaEvntB2Type;

#define LWR_MEAS_MAX_CDMA_HRPD_EVENT_B2_CFG 1
/** @brief This enum defines the purpose to use CDMA HRPD Event B2
 * the following have been defined
 * -# Cdma Hrpd event B2 optimised Handover
 * -# Reserved1
 * -# Reserved2
 */
typedef enum lwrMeasCdmaHrpdEvntB2Type
{
   WR_MEAS_CDMA_HRPD_EVENTB2_OPT_HRPD_HO,
   WR_MEAS_CDMA_HRPD_EVENTB2_RESV1,
   WR_MEAS_CDMA_HRPD_EVENTB2_RESV2
} LwrMeasCdmaHrpdEvntB2Type;

#define LWR_MEAS_MAX_CDMA_PERD_CFG        3
/** @brief This enum defines the purpose to use CDMA Periodic
 * the following have been defined
 * -# cdma periodic report strongest cells
 * -# cdma periodic report strongest cells ANR
 * -# cdma periodic report CGI
 */
typedef enum lwrMeasCDMAPeriodic
{
   WR_MEAS_CDMA_PERIODIC_REPORT_STRNG_CELLS,
   WR_MEAS_CDMA_PERIODIC_REPORT_STRNG_CELLS_ANR,
   WR_MEAS_CDMA_PERIODIC_REPORT_CGI
} LwrMeasCDMAPeriodic;


#define LWR_MEAS_MAX_UTRA_PERD_CFG        3
typedef enum lwrMeasUtraPeriodic
{
   WR_MEAS_UTRA_PRDRPT_STRNGCELLS,
   WR_MEAS_UTRA_PRDRPT_STRGCELLS_ANR,
   WR_MEAS_UTRA_PRDRPT_STRGCELLS_CSFB,
   WR_MEAS_UTRA_PERIODIC_REPORT_CGI
}LwrMeasUtraPeriodic;

#define LWR_MEAS_MAX_GERAN_PERD_CFG 3
/** @brief This enum defines the purpose to use GERAN Periodic
 * the following have been defined
 * -# GERAN periodic report strongest cells
 * -# GERAN periodic report strongest cells for ANR
 * -# GERAN CGI report
 */  
typedef enum lwrMeasGERANPeriodic
{
   LWR_MEAS_GERAN_REPORT_STRNG_CELLS,
   LWR_MEAS_GERAN_REPORT_STRNG_CELLS_ANR,
   LWR_MEAS_GERAN_CGI
}LwrMeasGERANPeriodic;

/** @brief This structure used to get Event B2 Measurement releated OAM
 * parameters from EMM.
 * @details These are the structure members
  - Bool        isTobeUsed        TRUE: configure, FALSE: Not configaure
  - U8          reason            reason to configure
  - U8          cellType          CDMA 1xRTT or CDMA HRPD 
  - U32         threshold1Rsrp    threshold1 RSRP used in Inter-RAT measurement
                                  report triggering condition for CDMA2000 
                                  Event B2
  - U32         threshold1Rsrq    threshold1 RSRQ used in Inter-RAT measurement
                                  report triggering condition for
                                  CDMA2000 Event B2
  - U32         threshold2        threshold2 used in Inter-RAT measurement report
                                  triggering condition for CDMA2000 Event B2
  - U32         hysteresis        hysteresis value used in entry and
                                  leave condition 
  - U32         timeToTrigger     time to trigger value
  - U8          maxRptCells       Number of maximum Cell including in
                                  measurement report
  - U8          trigQtyEutra      This is EUTRA quantity used for calculating
                                  triggering condition of CDMA2000 1xRTT Event
                                  B2 0: RSRP 1: RSRQ
  - U32         reportInterval    interval of measurement report
  - U32         reportAmount      number of measurement report 
 *  */
typedef struct lwrMeasCDMAEvntB2RptCfg
{
   TknPres                   isPres;
   U8                        reason;
   U8                        cellType;
   U32                       threshold1Rsrp;
   U32                       threshold1Rsrq;
   U32                       threshold2;
   U32                       hysteresis;
   U32                       timeToTrigger;
   U8                        maxRptCells;
   U8                        trigQtyEutra;
   U32                       reportInterval;
   U32                       reportAmount;
} LwrMeasCDMAEvntB2RptCfg;

/** @brief This structure used to get CDMA2000 Periodic Measurement releated OAM
 * parameters from EMM.
 * @details These are the structure members
  - Bool        isTobeUsed        TRUE: configure, FALSE: Not configaure
  - U8          reason            reason to configure
  - U8          cellType          CDMA 1xRTT or CDMA HRPD 
  - U8          maxRptCells       Number of maximum Cell including in
                                  measurement report
  - U32         reportInterval    interval of measurement report
  - U32         reportAmount      number of measurement report 
 *
 *  */
typedef struct lwrMeasCDMAPeridRptCfg
{
   TknPres                   isPres;
   U8                        reason;
   U8                        cellType;
   U8                        maxRptCells;
   U32                       reportInterval;
   U32                       reportAmount;
} LwrMeasCDMAPeridRptCfg;

/** @brief This structure used to get EUTRAN Periodic Measurement releated OAM
 * parameters from EMM.
 * @details These are the structure members
  - Bool        isTobeUsed        TRUE: configure, FALSE: Not configaure
  - U8          reason            reason to configure
  - U8          triggerQty        quantity used to calculate triggering
                                  condition
  - U8          reportQty         Report Quantity
  - U8          maxRptCells       Number of maximum Cell including in
                                  measurement report
  - U32         reportInterval    interval of measurement report
  - U32         reportAmount      number of measurement report 
 *
 * */
typedef struct lwrMeasEutraPeridRptCfg
{
   TknPres                   isPres;
   U8                        reason;
   U8                        triggerQty;
   U8                        reportQty;
   U8                        maxRptCells;
   U32                       reportInterval;
   U32                       reportAmount;
   U32                       sonCfgRptInterval; /*<! This meas periodic reporting 
                             interval will be calculated at SON module according 
                             to SON UE report Interval value*/
   Bool                      isPerdStrongCellCfgRptEnable;
} LwrMeasEutraPeridRptCfg;

/** @brief This structure used to get Event B1 Measurement releated OAM
 * parameters from EMM.
 * @details These are the structure members

  - Bool        isTobeUsed        TRUE: configure, FALSE: Not configaure
  - U8          reason            reason to configure
  - U8          cellType          CDMA 1xRTT or CDMA HRPD 
  - U32         threshold         threshold used in Inter-RAT measurement
                                  report triggering condition for CDMA2000
                                  Event B1
  - U32         hysteresis        hysteresis value used in entry and leave
                                  condition
  - U32         timeToTrigger     time to trigger value
  - U8          maxRptCells       Number of maximum Cell including in
                                  measurement report
  - U32         reportInterval    interval of measurement report
  - U32         reportAmount      number of measurement report 
 *
 *  */
typedef struct lwrMeasCDMAEvntB1RptCfg
{
   TknPres                   isPres;
   U8                        reason;
   U8                        cellType;
   U32                       threshold;
   U32                       hysteresis;
   U32                       timeToTrigger;
   U8                        maxRptCells;
   U32                       reportInterval;
   U32                       reportAmount;
} LwrMeasCDMAEvntB1RptCfg;

/** @brief This structure used to get Event A3 Measurement releated OAM
 * parameters from EMM.
 * @details These are the structure members
  - Bool        isTobeUsed        TRUE: configure, FALSE: Not configaure
  - U8          reason            reason to configure
  - S32         offset            threshold of RSRP used in EUTRA measurement report 
                                  triggering condition for Event A3
  - U8          reportOnLeave     threshold of RSRQ used in EUTRA measurement
                                  report triggering condition for Event A3
  - U32         hysteresis        hysteresis value used in entry and leave
                                  condition
  - U32         timeToTrigger     time to trigger value
  - U8          triggerQty        quantity used to calculate triggering
                                  condition
  - U8          reportQty         Report Quantity
  - U8          maxRptCells       Number of maximum Cell including in
                                  measurement report
  - U32         reportInterval    interval of measurement report
  - U32         reportAmount      number of measurement report 
 *
 *  */
typedef struct lwrMeasEvntA3RptCfg
{
   TknPres                   isPres;
   U8                        reason;
   S32                       offset;
   U8                        reportOnLeave;
   U32                       hysteresis;
   U32                       timeToTrigger;
   U8                        triggerQty;
   U8                        reportQty;
   U8                        maxRptCells;
   U32                       reportInterval;
   U32                       reportAmount;
} LwrMeasEvntA3RptCfg;

/** @brief This structure used to get Event A5 Measurement releated OAM
 * parameters from EMM.
 * @details These are the structure members
 -  Bool        isTobeUsed          TRUE: configure, FALSE: Not configaure
 -  U8          reason              reason to configure
 -  U32         threshold1_RSRP     threshold1 of RSRP used in EUTRA
                                    measurement report
 -  U32         threshold1_RSRQ     threshold1 of RSRQ used
                                    in EUTRA measurement report
 -  U32         threshold2_RSRP     threshold2 of RSRP used in EUTRA
                                    measurement report
 -  U32         threshold2_RSRQ     threshold2 of RSRQ used in EUTRA
                                    measurement report
 -  U32         hysteresis          hysteresis value used in entry and
                                    leave condition 
 -  U32         timeToTrigger       time to trigger value
 -  U8          triggerQty          quantity used to calculate
                                    triggering  condition
 -  U8          reportQty           Report Quantity
 -  U8          maxRptCells         Number of maximum Cell including in
                                    measurement report
 -  U32         reportInterval      interval of measurement report
 -  U32         reportAmount        number of measurement report
 *
 *  */
typedef struct lwrMeasEvntA5RptCfg
{
   TknPres                   isPres;
   U8                        reason;
   U32                       threshold1_RSRP;
   U32                       threshold1_RSRQ;
   U32                       threshold2_RSRP;
   U32                       threshold2_RSRQ;
   U32                       hysteresis;
   U32                       timeToTrigger;
   U8                        triggerQty;
   U8                        reportQty;
   U8                        maxRptCells;
   U32                       reportInterval;
   U32                       reportAmount;
} LwrMeasEvntA5RptCfg;

/** @brief This structure used to get Event A1, A2, and A4 Measurement releated
 *     OAM parameters from EMM.
 * @details These are the structure members
 -  Bool        isTobeUsed          TRUE: configure, FALSE: Not configaure
 -  U8          reason              reason to configure
 -  U32         threshold_RSRP      threshold of RSRP used in EUTRA
                                    measurement report
 -  U32         threshold_RSRQ      threshold of RSRQ used
                                    in EUTRA measurement report
 -  U32         hysteresis          hysteresis value used in entry and
                                    leave condition 
 -  U32         timeToTrigger       time to trigger value
 -  U8          triggerQty          quantity used to calculate
                                    triggering  condition
 -  U8          reportQty           Report Quantity
 -  U8          maxRptCells         Number of maximum Cell including in
                                    measurement report
 -  U32         reportInterval      interval of measurement report
 -  U32         reportAmount        number of measurement report
 *
 * */
typedef struct lwrMeasEvntRptCfg
{
   TknPres                  isPres;
   U8                       reason;
   U32                      threshold_RSRP;
   U32                      threshold_RSRQ;
   U32                      hysteresis;
   U32                      timeToTrigger;
   U8                       triggerQty;
   U8                       reportQty;
   U8                       maxRptCells;
   U32                      reportInterval;
   U32                      reportAmount;
} LwrMeasEvntRptCfg;

/** @brief This structure holds periodic report configuration information for UTRAN
  - U8    reason;              Reason for using UTRA periodic report configuration
  - Bool  activeState;         Indicates whether periodic report is used or not
  - U8    maxReportCell;       The maximum number of cells included in a measurement
                                     report for the UTRA periodic report
  - U8    reportInterval;      The reporting interval of a measurement report
                                     for the UTRA periodic report
  - U8    reportAmount;        The number of measurement reports for the
                     UTRA periodic report
*/
typedef struct lwrMeasUtraPeriodRptCfg
{
   U8                         reason;
   Bool                       activeState;
   U8                         maxReportCell;
   U8                         reportInterval;
   U8                         reportAmount;
}LwrMeasUtraPeriodRptCfg;

      
/** @brief This structure holds event-B1 information for UTRAN
  - LwrMeasUtraB1Type      reason;              Reason for using UTRA event B1
  - Bool                   activeState;         Indicates whether event B1 is used
                  or not
  - S8                     b1ThresholdUtraRSCP; UTRA RSCP threshold used in 
                  the Inter-RAT measurement report
                                           triggering condition for the UTRA 
                  event B1
  - U8                     b1ThresholdUtraEcNo; UTRA ECNO threshold used in the
                  Inter-RAT measurement report
                                                triggering condition for the UTRA 
                  event B1
  - U8                     hysteresis;          Hysteresis applied to entry and
                  leaving condition
  - U8                     timeToTrigger        timeToTrigegr value for UTRA
                    Event B1.The timeToTrigger value is
                                                the period of time that must be met
                  for the UE to trigger a measurement report
  - U8                     maxReportCell;       The maximum number of cells 
                  included in a measurement
                                                report for UTRA Event B1
  - U8                     reportInterval;      The reporting interval of a
                  measurement report for UTRA Event B1
  - U8                     reportAmount;        The number of measurement reports
                  for UTRA Event B1
*/
typedef struct lwrMeasUtraEvntB1RptCfg
{
   LwrMeasUtraB1Type           reason;                
   Bool                        activeState;          
   S8                          b1ThresholdUtraRSCP;   
   U8                          b1ThresholdUtraEcNo;   
   U8                          hysteresis;            
   U16                         timeToTrigger;         
   U8                          maxReportCell;         
   U8                          reportInterval;        
   U8                          reportAmount;          
}LwrMeasUtraEvntB1RptCfg;

/** @brief This structure holds event-B2 information for UTRAN
  - LwrMeasUtraB1Type    reason;              Reason for using UTRA event B2
  - Bool                 activeState;         Indicates whether event B2 is used
                     or not
  - U8                   b2ThresholdUtraRSRP; UTRA RSRP threshold used in the
                     Inter-RAT measurement report
                                              triggering condition for the UTRA 
                     event B2
  - U8                   b2ThresholdUtraEcNo; UTRA ECNO threshold used in the 
                     Inter-RAT measurement report
                                              triggering condition for the UTRA
                     event B2
  - S8                   b2ThresholdUtraRSCP; UTRA RSCP threshold used in the 
                     Inter-RAT measurement report
                                              triggering condition for the UTRA
                     event B2
  - U8                   b2ThresholdUtraRSRQ; UTRA RSRQ threshold used in the 
                     Inter-RAT measurement report
                                              triggering condition for the 
                     UTRA event B2
  - U8                   hysteresis;          Hysteresis applied to entry and
                     leaving condition
  - U8                   timeToTrigger        timeToTrigegr value for UTRA Event
                     B2.The timeToTrigger value is
                                              the period of time that must be
                     met for the UE to
                                              trigger a measurement report
  - U8                   maxReportCell;       The maximum number of cells 
                     included in a measurement
                                              report for UTRA Event B2
  - U8                   reportInterval;      The reporting interval of a 
                     measurement report for UTRA Event
                     B2
  - U8                   reportAmount;        The number of measurement reports
                     for UTRA Event B2
*/
typedef struct lwrMeasUtraEvntB2RptCfg
{
   LwrMeasUtraB2Type          reason;               
   Bool                       activeState;         
   U8                         b2Threshold1RSRP;
   U8                         b2Threshold1RSRQ;      
   S8                         b2Threshold2UtraRSCP;
   S8                         b2Threshold2UtraTddRSCP;
   U8                         b2Threshold2UtraEcNo;  
   U8                         hysteresis;            
   U16                        timeToTrigger;         
   U8                         maxReportCell;       
   U8                         reportInterval;        
   U8                         reportAmount;          

}LwrMeasUtraEvntB2RptCfg;
/** @brief This structure holds event-B2 information for UTRAN
  - LwrMeasGernaB2Type    reason;              Reason for using GERAN event B2
  - Bool                  activeState;         Indicates whether event B2 is used
  - U8                    b2Threshold1RSRP;    EUTRA Threshold B2 RSRP
  - U8                    b2Threshold1RSRQ;    EUTRA Threshold B2 RSCP   
  - U8                    b2ThresholdGeran;    Geran threshold used in the B2
  - U8                    hysteresis;          Hysteresis applied to entry and
                          leaving condition
  - U8                    timeToTrigger        timeToTrigegr value for GERAN Event
                                               B2.The timeToTrigger value is
                                               the period of time that must be
                                               met for the UE to
                                               trigger a measurement report
  - U8                   maxReportCell;        The maximum number of cells 
                                               included in a measurement
                                               report for GERAN Event B2
  - U8                   reportInterval;       The reporting interval of a 
                                               measurement report for GERAN Event
                                               B2
  - U8                   reportAmount;         The number of measurement reports
                                               for GERAN Event B2
*/
typedef struct lwrMeasGeranEvntB2RptCfg 
{
   LwrMeasGeranEvntB2Type     reason;
   Bool                       activeState;
   U8                         b2Threshold1RSRP;
   U8                         b2Threshold1RSRQ;      
   S8                         b2Threshold2Geran;
   U8                         hysteresis;          
   U16                        timeToTrigger;
   U8                         maxReportCell;       
   U8                         reportInterval;        
   U8                         reportAmount;          
}LwrMeasGeranEvntB2RptCfg;



/** @brief This structure holds perodic measurment config for GERAN */
typedef struct lwrMeasGERANPeridRptCfg
{
   TknPres                   isPres;
   U8                        reason;
   U8                        maxRptCells;
   U32                       reportInterval;
   U32                       reportAmount;
} LwrMeasGERANPeridRptCfg; 


/* @brief This structure used to store measurement configuration Params in cell
 *     Control block.
 * @details These are the structure members
 - U32                       reCfgType      Bit mask each param corresponds to a bit.
                                            -LSB  corresponds to first parameter
                                             within the group. 
 - LwrMeasEvntRptCfg         measA1EvtRptCfg  Event A1 configuration
 - LwrMeasEvntRptCfg         measA2EvtRptCfg  Event A2 configuration
 - LwrMeasEvntRptCfg         measA4EvtRptCfg  Event A4 configuration
 - LwrMeasEvntA3RptCfg       measEvtA3Cfg     Event A3 configuration
 - LwrMeasEvntA5RptCfg       measEvtA5Cfg     Event A5 configuration
 - LwrMeasEutraPeridRptCfg   measEutraPerdRptCfg  EUTRA Periodic configuration
 - LwrMeasCDMAEvntB1RptCfg   meas1xCDMAEventB1RptCfg  1xRTT CDMA2000 Event B1 configuration
 - LwrMeasCDMAEvntB1RptCfg   measCDMAHrpdEventB1RptCfg  CDMA2000  HRPD Event B1 configuration
 - LwrMeasCDMAEvntB2RptCfg   meas1xCDMAEventB2RptCfg    1xRTT CDMA2000 Event B2 configuration
 - LwrMeasCDMAEvntB2RptCfg   measCDMAHrpdEventB2RptCfg  CDMA2000 HRPD Event B2 configuration
 - LwrMeasCDMAPeridRptCfg    meas1xCDMAPeridRptCfg 1xRTT CDMA2000 periodic configuration
 - LwrMeasCDMAPeridRptCfg    measCDMAHrpdPeridRptCfg CDMA2000 HRPD periodic configuration
 - LwrMeasSpdStateScaleFact  measSpdSclFact Measurement Speed State scale Factor 
 - LwrMeasMobilityStatParams mobStatParams Mobility state Params
 - LwrMeasQtyCfgParams       qtyCfg        Quantity configuration 
 - LwrMeasGapConfig          measGap       MeasGap configuration
 - LwrMeasPreRegHrpd         preRegHrpd    Preregistration Hrpd.
 - U8                        searchWindowSize1xRTT search window size for 1xRTT
 - U8                        searchWindowSizeHRPD  search window size for HRPD
 - U8                        anrRptCfg Anr report Config value
 - U8                        hoRptCfg  HO report config value
*
*/
typedef struct lwrCellMeasCfgGrp
{
   U32                        reCfgType;
   LwrMeasEvntRptCfg          measA1EvtRptCfg[LWR_MEAS_MAX_EVENT_A1_CFG];
   LwrMeasEvntRptCfg          measA2EvtRptCfg[LWR_MEAS_MAX_EVENT_A2_CFG];
   LwrMeasEvntRptCfg          measA4EvtRptCfg[LWR_MEAS_MAX_EVENT_A4_CFG];
   LwrMeasEvntA3RptCfg        measEvtA3Cfg[LWR_MEAS_MAX_EVENT_A3_CFG];
   LwrMeasEvntA5RptCfg        measEvtA5Cfg[LWR_MEAS_MAX_EVENT_A5_CFG];
   LwrMeasEutraPeridRptCfg    measEutraPerdRptCfg[LWR_MEAS_MAX_EUTRA_PERD_CFG];
   LwrMeasUtraPeriodRptCfg    measUtraPerdRptCfg[LWR_MEAS_MAX_UTRA_PERD_CFG];
   LwrMeasUtraEvntB1RptCfg    measUtraEventB1RptCfg[LWR_MEAS_MAX_UTRA_B1_CFG];
   LwrMeasUtraEvntB2RptCfg    measUtraEventB2RptCfg[LWR_MEAS_MAX_UTRA_EVNT_B2_CFG];
   LwrMeasGeranEvntB2RptCfg   measGeranEventB2RptCfg[LWR_MEAS_MAX_GERAN_EVNT_B2_CFG];
   LwrMeasCDMAEvntB1RptCfg    meas1xCDMAEventB1RptCfg[LWR_MEAS_MAX_EVENT_B1_CFG];
   LwrMeasCDMAEvntB1RptCfg    measCDMAHrpdEventB1RptCfg[LWR_MEAS_MAX_EVENT_B1_CFG];
   LwrMeasCDMAEvntB2RptCfg
      meas1xCDMAEventB2RptCfg[LWR_MEAS_MAX_1XCDMA_EVENT_B2_CFG];
   LwrMeasCDMAEvntB2RptCfg
      measCDMAHrpdEventB2RptCfg[LWR_MEAS_MAX_CDMA_HRPD_EVENT_B2_CFG];
   LwrMeasCDMAPeridRptCfg     meas1xCDMAPeridRptCfg[LWR_MEAS_MAX_CDMA_PERD_CFG];
   LwrMeasCDMAPeridRptCfg     measCDMAHrpdPeridRptCfg[LWR_MEAS_MAX_CDMA_PERD_CFG];
   /* GERAN Periodic Report Configuration */
   LwrMeasGERANPeridRptCfg    measGERANPeridRptCfg[LWR_MEAS_MAX_GERAN_PERD_CFG]; 
   LwrMeasSpdStateScaleFact   measSpdSclFact;
   LwrMeasMobilityStatParams  mobStatParams;
   LwrMeasQtyCfgParams        qtyCfg;
   LwrMeasGapConfig           measGap;
   LwrMeasPreRegHrpd          preRegHrpd;
   U8                         searchWindowSize1xRTT;
   U8                         searchWindowSizeHRPD;
   U8                         anrRptCfg;
   U8                         hoRptCfg;
   /*rsrq_ho*/
   U8                         rsrqHoThrshld;
} LwrCellMeasCfgGrp;


/** @brief This enum defines the various type of groups we can have currently
 * the following have been defined 
 * -# information related to MIB
 * -# information related to SIB1
 * -# information related to SIB2
 * -# information related to SIB3
 * -# information related to SIB4
 */
typedef enum lwrCellCfgGrpType
{
   WR_CELL_CFG_GRP_MIB,
   WR_CELL_CFG_GRP_SIB1,
   WR_CELL_CFG_GRP_SIB2,
   WR_CELL_CFG_GRP_SIB3,
   WR_CELL_CFG_GRP_SIB4,
   WR_CELL_CFG_GRP_SIB6,
   WR_CELL_CFG_GRP_SIB7, 
   WR_CELL_CFG_GRP_SIB8,
   WR_CELL_CFG_GRP_SIB9,
   WR_CELL_CFG_GRP_ANR,
   WR_CELL_CFG_GRP_MEAS,
   WR_CELL_CFG_GRP_EAID,
   WR_CELL_CFG_GRP_SIB_SCHED,
   WR_CELL_CFG_GRP_RAB_PARAMS,
   WR_CELL_CFG_GRP_TIMER, /* TBD */
   WR_CELL_CFG_GRP_RRM,
   WR_CELL_CFG_GRP_ULSCHD, /*Added for UL Scheduler Cfg*/
   WR_CELL_CFG_GRP_DLSCHD, /*Added for DL Scheduler Cfg*/
   WR_CELL_CFG_GRP_ULPWRCNTRL,
   WR_CELL_CFG_GRP_RGR,   
   WR_CELL_CFG_GRP_CTF,
   WR_CELL_CFG_GRP_RLC,
   WR_CELL_CFG_GRP_RAB_POLICY,
   WR_CELL_CFG_GRP_SCHD,
   WR_CELL_CFG_GRP_CSFB,
   WR_CELL_CFG_GRP_LTEA,
   WR_CELL_CFG_MAX_GRPS
} LwrCellCfgGrpType;

#if 1
typedef struct lwrSib3IntraFreqCellResel
{
   S8                      qRxLevMin;
   U8                      pMaxPres;
   S8                      pMax;
   U8                      sIntraSearchPres;
   U8                      SIntraSearch;
   U8                      allwdMeasBandwidthPres;
   U8                      allwdMeasBandwidth;
   U8                      antennaPort1Pres;
   U8                      neighCellCfg;
   U8                      tReselEutra;
   U8                      tResetEutraSFMedium;
   U8                      tResetEutraSFHigh;
} LwrSib3IntraFreqCellResel;
typedef struct lwrSib3ServFreqInfo
{
  U8                         sNonIntraSearchPres;
  U8                         sNonIntraSearch;
  U8                         threshServingLow;
  U8                         cellReselPrio;
} LwrSib3ServFreqInfo;
typedef struct lwrSib3MobiStateParams
{
   U8                        tEvaluation;
   U8                        tHyst;
   U8                        nCellChangeMed;
   U8                        nCellChangeHigh;
} LwrSib3MobiStateParams;
#endif
/* * speedStReselPreset is 0 implies that sfMedium and sfHigh are not to  */
/*   be included in the broadcast. These define hysteris parameters for   */
/*   UEs with medium and high mobility                                    */
typedef struct lwrCellSib3CfgGrp
{
   U32                      reCfgType; /*!< Bit mask each param corresponds to a
                                       bit. 
                                       - LSB corresponds to first parameter
                                       within the group.
                                       */
   U8                        qHyst;
   U8                        speedStateReselPres;
   U8                        sfMedium;
   U8                        sfHigh;
   LwrMeasMobilityStatParams mobStateParams;
#if 1
   LwrSib3ServFreqInfo       servFreqInfo;
   LwrSib3IntraFreqCellResel intraFreqReSel;
#endif
} LwrCellSib3CfgGrp;

typedef struct lwrSib4CsgInfo
{
  U8                        csgPciType;
  U16                       csgStartPhyCellId; 
  U8                        isCsgPciCfgd;
  U8                        csgPciRange;
} LwrSib4CsgInfo;
typedef struct lwrcellSib4CfgGrp
{
   U32                       reCfgType;
   LwrSib4CsgInfo            csgInfo;
} LwrCellSib4CfgGrp;
typedef enum lwrEnbType
{
   WR_ENODEB_TYPE_MACRO,
   WR_ENODEB_TYPE_HOME
} LwrEnbType;
typedef enum lwrX2ConFlg
{
   WR_X2_CON_DIS_ALL,
   WR_X2_CON_ENBL_ALL,
   WR_X2_CON_ENBL_FEMTO,
   WR_X2_CON_ENBL_MACRO
} LwrX2ConFlg;


/* UE history info - start */
typedef enum lwrEnbCellType
{
   WR_ENB_CELL_SIZE_TYPE_VERY_SMALL,
   WR_ENB_CELL_SIZE_TYPE_SMALL,
   WR_ENB_CELL_SIZE_TYPE_MEDIUM,
   WR_ENB_CELL_SIZE_TYPE_LARGE
} LwrEnbCellType;
/* UE history info - end */

typedef struct lwrCellEAIDCfgGrp
{
   U32                       reCfgType;
   TknU32                    eaid[LWR_SM_MAX_EAIDS];
} LwrCellEAIDCfgGrp; 
/** @brief This structure contains the SIB scheduling configuration
 */
typedef struct lwrCellSibSchedCfgGrp 
{
  U32                       reCfgType;   /*!<  Bit mask for Reconfiguration */
   U8                        sib2Period; 
   U8                        sib3Period; 
   U8                        sib4Period; 
   U8                        sib5Period; 
   U8                        sib6Period; 
   U8                        sib7Period; 
   U8                        sib8Period; 
   U8                        sib9Period; 
   U8                        sib10Period; 
   U8                        sib11Period; 
   U8                        sib12Period; 
   U8                        siWindow;
   Bool                      sib10ManualPrdPres;
   U16                       sib10RepetPeriod;
   U16                       sib10NumBroadCast;
   U16                       sib11WarnSegSize;
   Bool                      sib6PeriodValid;
   Bool                      sib7PeriodValid; 
   Bool                      sib8PeriodValid;
} LwrCellSibSchedCfgGrp;

/* Added structure for CDMA target cell information ,
 * which consist of - marketId, switchNum, cellId & sectorNum 
 */
typedef struct lwrCdmaTgtcell
{
   U16          marketId; 
   U16          switchNum;
   U16          cellId;
   U16          sectorNum;
}LwrCdmaTgtCell;

/* Added structure to store Referenced Cell Id/Sector id 
 * for Cdma2000 - either in TknStrOSXL format or as 
 * combination of marketId, switchNum, cellId & sectorNum */
typedef struct lwrCdmaSectorId
{
   Bool         isOctStrPres;   /* TRUE means sector Id is present in octet form
                                 * else need to generate from LwrCdmaTgtCell */
   U32          cdmaType;       /* Cdma type - 1xRTT or HRPD */
   union
   {
      TknStrOSXL        sectorIdPtr; /* sector Id is in octet string */
      LwrCdmaTgtCell    cdmaSectorId; /* sector Id is combination of */
   }s;
}LwrCdmaSectorId;

/* Added structure to store CSFB configuration specific to 1xRtt */
typedef struct lwrCsfbCdma1xRttCfgParams
{
   LwrCdmaSectorId           secId;    /* Sector Id at eNodeB level */
   LwrCdma2k1xMobParams     mobParams; /* Mobility parameters for Cdma2000 */
}LwrCsfbCdma1xRttCfgParams;

typedef enum wrCsfbSupport
{
   LWR_CSFB_WITHOUT_MEAS,
   LWR_CSFB_WITH_MEAS
}LwrCsfbSupport;

typedef enum wrEcsfbSupport
{
   LWR_ECSFB_NOT_SUPPORTED,
   LWR_ECSFB_WITHOUT_MEAS,
   LWR_ECSFB_WITH_MEAS
}LwrEcsfbSupport;

/* Added structure to store CSFB related configuration information*/
typedef struct lwrCsfbCfgParams
{
   Bool                       dualRxSup;        /* Indicates whether Dual-Rx 
                                                   is supported or not */
   LwrCsfbSupport             csfbToUtraMeasSup;/* Indicates whether CSFB to 
                                                   UTRA is supported with or
                                                   without Meas */
   LwrCsfbSupport             csfbToCdmaMeasSup;/* Indicates whether CSFB to 
                                                   CDMA is supported with or
                                                   without Meas */
   LwrEcsfbSupport            eCsfbMeasSup;     /* Indicates whether ECSFB is 
                                                supported with or without Meas
                                                or not supported */
   Bool                       cdma1xRttSup; /*indicates whether 1xRTT is supported or not */
   LwrCsfbCdma1xRttCfgParams cdma1xrttCsfbCfg; /* CSFB configuration specific 
                                                  to CDMA 1xRtt */
   Bool                       dualRxTxSup;     /* Indicates whether Dual-Rx-Tx 
												  is supported or not*/
}LwrCsfbCfgGrp;

typedef enum lwrUpdateRetryCount
{
   ci_CountZero = 0,
   ci_CountOne = 1,
   ci_CouintTwo = 2,
   ci_CountThree = 3,
   ci_CountTen = 4,
   ci_CountInfinity = 5
} LwrUpdateRetryCount;
                  
typedef enum lwrUpdateTimeToWait
{
   ci_v1s = 0,
   ci_v2s = 1,
   ci_v5s = 2,
   ci_v10s = 3,
   ci_v20s = 4,
   ci_v60s = 5
}LwrUpdateTimeToWait;

      
typedef struct lwrProcTimerCfgGrp
{
   U32                 reCfgType;
   U8                  dbIndex;
   U32                 rrcConnectionSetupTmr;
   U32                 rrcConnectionReconfigTmr;
   U32                 rrcConnectionReEstablishTmr;
   U32                 rrcSecurityModeCommandTmr;
   U32                 rrcUeCapabilityEnquiryTmr;
   U32                 rrcConnectionReleaseTmr;
   U32                 rrcHandoverPrepTmr;
   U32                 rrcUeInformationRequestTmr;
   U16                 s1SetupTmr;
   U16                 s1UpdateTmr;
   LwrUpdateRetryCount s1RetryCount;
   LwrUpdateTimeToWait s1TimeToWait;
   U16                 s1InitialContextSetupTmr;
   U16                 s1ErabReleaseIndicationTmr;
   U16                 s1UeContextReleaseTmr;
   U16                 s1PathSwitchTmr;
   U16                 s1HandoverPrepTmr;
   U16                 s1RelocOverallTmr;
   /* S1 Handover Preparation timer for IRAT handover to UTRA */  
   U16                 s1IRatHandoverPrepTmr;  
   /* S1 Handover overall timer for IRAT handover to UTRA */ 
   U16                 s1IRatRelocOverallTmr;          
   U16                 s1HandoverCancelTmr;
   U16                 s1MmeConfigurationTransferTmr;
   U16                 s1MmeStatusTransferTmr;
   U16                 s1ResetTmr;
   U16                 S1ResetRetryCount; 
   U16                 x2SetupTmr;
   LwrUpdateTimeToWait x2SetTimeToWait;
   U16                 x2UpdateTmr;
   LwrUpdateRetryCount x2UpdRetryCount;
   LwrUpdateTimeToWait x2UpdTimeToWait;
   U16                 x2HandoverPrepTmr;
   U16                 x2RelocOverallTmr;
   U16                 x2StoreUeContextTmr;
   U16                 x2SnStatusTransferTmr;
   U16                 x2ResetTmr;
   U16                 X2ResetRetryCount;
   U16                 X2SetupRetryCount;
   U16                 internalResourceSetupTmr;
   U16                 internalResourceReleaseTmr;
   U16                 internalSecurityControlTmr;
   U16                 internalRrcDataTransferTmr;
   U16                 internalForwardingPathSetupTmr;
   U16                 internalReestablishControlTmr;
   U16                 internalBufferFlushTmr;
   U16                 internalDataTransferStartTmr;
   U16                 internalDataForwardEndIndTmr;
   U16                 internalReestablishTimeToWaitTmr;
   U16                 internalUserInactivityTmr;
   U16                 internalSignalingInactivityTmr;
   U16                 internalUnassignedTmr;
   U16                 internalNoWaitRetryTmr;
   U16                 internalRrcResetTmr;
   U16                 internalEnbResetTmr;
   U16                 internalSolicitMeasurementTmr;
   U16                 internalRtdInfoReqTmr;
   U16                 internalSubCellChangeTmr;
   U16                 internalSubCellPdcpControlTmr;
   U16                 ehrpdOptHoCompleteTmr;
   U16                 intraEnbHoCompleteTmr;
   U16                 measurementReportToWaitTmr;
   U32                 rimOvrlTmr;
   U32                 rimRirTmr;
   U32                 rimRiTmr;
   U32                 rimRiaeTmr;
   U32                 rimMaxRetryCnt;
}LwrProcTimerCfgGrp;

typedef enum lwrTimerPollRetransmit
{
   LTE_MS5,
   LTE_MS10, 
   LTE_MS15, 
   LTE_MS20, 
   LTE_MS25, 
   LTE_MS30, 
   LTE_MS35,
   LTE_MS40,  
   LTE_MS45,
   LTE_MS50, 
   LTE_MS55, 
   LTE_MS60, 
   LTE_MS65, 
   LTE_MS70,
   LTE_MS75,
   LTE_MS80, 
   LTE_MS85, 
   LTE_MS90, 
   LTE_MS95, 
   LTE_MS100, 
   LTE_MS105,
   LTE_MS110, 
   LTE_MS115, 
   LTE_MS120, 
   LTE_MS125, 
   LTE_MS130, 
   LTE_MS135,
   LTE_MS140, 
   LTE_MS145, 
   LTE_MS150, 
   LTE_MS155, 
   LTE_MS160, 
   LTE_MS165,
   LTE_MS170, 
   LTE_MS175, 
   LTE_MS180, 
   LTE_MS185, 
   LTE_MS190, 
   LTE_MS195,
   LTE_MS200, 
   LTE_MS205, 
   LTE_MS210, 
   LTE_MS215, 
   LTE_MS220, 
   LTE_MS225,
   LTE_MS230, 
   LTE_MS235, 
   LTE_MS240, 
   LTE_MS245, 
   LTE_MS250, 
   LTE_MS300,
   LTE_MS350, 
   LTE_MS400, 
   LTE_MS450, 
   LTE_MS500, 
   LTE_SPARE9, 
   LTE_SPARE8,
   LTE_SPARE7, 
   LTE_SPARE6, 
   LTE_SPARE5, 
   LTE_SPARE4, 
   LTE_SPARE3,
   LTE_SPARE2, 
   LTE_SPARE1

} LwrTimerPollRetransmit;


typedef enum lwrPollPdu
{
   LTE_P4, 
   LTE_P8, 
   LTE_P16, 
   LTE_P32, 
   LTE_P64, 
   LTE_P128, 
   LTE_P256, 
   LTE_PInfinity    
} LwrPollPdu;

typedef enum lwrPollByte
{
   LTE_KB25, 
   LTE_KB50, 
   LTE_KB75, 
   LTE_KB100, 
   LTE_KB125, 
   LTE_KB250, 
   LTE_KB375,
   LTE_KB500,
   LTE_KB750, 
   LTE_KB1000, 
   LTE_KB1250, 
   LTE_KB1500, 
   LTE_KB2000,
   LTE_KB3000, 
   LTE_KBinfinity, 
   LTE_SPARE_1
} LwrPollByte;

typedef enum lwrMaxRetransThresh
{
   LTE_T1, 
   LTE_T2, 
   LTE_T3, 
   LTE_T4, 
   LTE_T6, 
   LTE_T8, 
   LTE_T16, 
   LTE_T32
} LwrMaxRetransThresh;

typedef enum lwrTimerReordering
{
   LTE_TR_MS0, 
   LTE_TR_MS5, 
   LTE_TR_MS10, 
   LTE_TR_MS15, 
   LTE_TR_MS20, 
   LTE_TR_MS25, 
   LTE_TR_MS30, 
   LTE_TR_MS35,
   LTE_TR_MS40, 
   LTE_TR_MS45, 
   LTE_TR_MS50, 
   LTE_TR_MS55, 
   LTE_TR_MS60, 
   LTE_TR_MS65, 
   LTE_TR_MS70,
   LTE_TR_MS75, 
   LTE_TR_MS80, 
   LTE_TR_MS85, 
   LTE_TR_MS90, 
   LTE_TR_MS95, 
   LTE_TR_MS100, 
   LTE_TR_MS110,
   LTE_TR_MS120, 
   LTE_TR_MS130, 
   LTE_TR_MS140, 
   LTE_TR_MS150, 
   LTE_TR_MS160, 
   LTE_TR_MS170,
   LTE_TR_MS180, 
   LTE_TR_MS190, 
   LTE_TR_MS200, 
   LTE_TR_SPARE1
} LwrTimerReordering;

typedef enum lwrTimerStatusProhibit
{
   LTE_TSP_MS0, 
   LTE_TSP_MS5, 
   LTE_TSP_MS10, 
   LTE_TSP_MS15, 
   LTE_TSP_MS20, 
   LTE_TSP_MS25,
   LTE_TSP_MS30, 
   LTE_TSP_MS35,
   LTE_TSP_MS40, 
   LTE_TSP_MS45, 
   LTE_TSP_MS50, 
   LTE_TSP_MS55, 
   LTE_TSP_MS60, 
   LTE_TSP_MS65, 
   LTE_TSP_MS70,
   LTE_TSP_MS75, 
   LTE_TSP_MS80, 
   LTE_TSP_MS85, 
   LTE_TSP_MS90, 
   LTE_TSP_MS95, 
   LTE_TSP_MS100, 
   LTE_TSP_MS105,
   LTE_TSP_MS110, 
   LTE_TSP_MS115, 
   LTE_TSP_MS120, 
   LTE_TSP_MS125, 
   LTE_TSP_MS130, 
   LTE_TSP_MS135,
   LTE_TSP_MS140, 
   LTE_TSP_MS145, 
   LTE_TSP_MS150, 
   LTE_TSP_MS155, 
   LTE_TSP_MS160, 
   LTE_TSP_MS165,
   LTE_TSP_MS170, 
   LTE_TSP_MS175, 
   LTE_TSP_MS180, 
   LTE_TSP_MS185, 
   LTE_TSP_MS190, 
   LTE_TSP_MS195,
   LTE_TSP_MS200, 
   LTE_TSP_MS205, 
   LTE_TSP_MS210, 
   LTE_TSP_MS215, 
   LTE_TSP_MS220, 
   LTE_TSP_MS225,
   LTE_TSP_MS230, 
   LTE_TSP_MS235, 
   LTE_TSP_MS240, 
   LTE_TSP_MS245, 
   LTE_TSP_MS250, 
   LTE_TSP_MS300,
   LTE_TSP_MS350, 
   LTE_TSP_MS400, 
   LTE_TSP_MS450, 
   LTE_TSP_MS500, 
   LTE_TSP_SPARE8, 
   LTE_TSP_SPARE7,
   LTE_TSP_SPARE6, 
   LTE_TSP_SPARE5, 
   LTE_TSP_SPARE4, 
   LTE_TSP_SPARE3, 
   LTE_TSP_SPARE2,
   LTE_TSP_SPARE1

} LwrTimerStatusProhibit;

typedef enum lwrSnFieldLength
{
   SIZE5, 
   SIZE10
} LwrSnFieldLength;

typedef enum lwrPrioritisedBitRate
{
   LTE_PBR_KBPS0, 
   LTE_PBR_KBPS8, 
   LTE_PBR_KBPS16, 
   LTE_PBR_KBPS32, 
   LTE_PBR_KBPS64, 
   LTE_PBR_KBPS128,
   LTE_PBR_KBPS256, 
   INFINITY, 
   LTE_PBR_SPARE8, 
   LTE_PBR_SPARE7, 
   LTE_PBR_SPARE6,
   LTE_PBR_SPARE5, 
   LTE_PBR_SPARE4, 
   LTE_PBR_SPARE3, 
   LTE_PBR_SPARE2, 
   LTE_PBR_SPARE1

} LwrPrioritisedBitRate;

#define WR_UL_BIT_RATE_192_kBps 192000 
#define WR_UL_BIT_RATE_96_kBps  96000 
#define WR_UL_BIT_RATE_48_kBps  48000
#define WR_UL_BIT_RATE_24_kBps  24000 
#define WR_UL_BIT_RATE_12_kBps  12000 

typedef enum lwrBucketSizeDurn
{
   LTE_BSD_MS50, 
   LTE_BSD_MS100, 
   LTE_BSD_MS150, 
   LTE_BSD_MS300, 
   LTE_BSD_MS500, 
   LTE_BSD_MS1000, 
   LTE_BSD_SPARE2,
   LTE_BSD_SPARE1

} LwrBucketSizeDurn;
typedef struct lwrSrbQciInfo
{
   U8                        srbId;
   U8                        resourceType;    /*!< GBR or Non-GBR */
   U8                        priotity;   /*!< SRB priority */
   U8                        pdb;      /*!< Packet Delay Budget */
   U8                        pler;     /*!< Packet Error Loss Rate */
}LwrSrbQciInfo;

typedef struct lwrSrbLchInfo
{
   U8                       srbId;
   U8                       pbr; /*!<  Prioritzed Bit Rate */
   U8                       bucketSizeDuration;
   U8                       lchGrpId;
   U8                       priority;
}LwrSrbLchInfo;
typedef struct lwrSrbRlcInfo
{
   U8                        srbId;
   LwrTimerPollRetransmit    tpRetransmit;
   LwrPollPdu                pollPdu;
   LwrPollByte               pollByte;
   LwrMaxRetransThresh       maxRetransThresh;
   LwrTimerReordering        timerReorderingUl;
   LwrTimerReordering        timerReorderingDl;
   LwrTimerStatusProhibit    timerStatusProhibit;
}LwrSrbRlcInfo;
typedef struct lwrSrbPolicy
{
   U32                       reCfgType;
   U8                        srbId;
   LwrSrbRlcInfo             rlcInfo;
   LwrSrbLchInfo             lchInfo;
   LwrSrbQciInfo             qciInfo;
}LwrSrbPolicy;

typedef struct lwrDrbPdcpInfo
{
   S16                        qci; 
   U16                       umSnSize;   /*!< size of PDCP Sequence Number (SN) in RLC UM mode */
   U8                        discardTimer;   /*!< Discard timer value for a packet buffer 
                                                  managed per RB in RLC AM mode */
   U16                       fwdEndTimer;   /*!< PDCP of target eNB waits for end marker 
                                                 up on receiving of HO Complete Msg*/
   Bool                      statRprtReqrd;
} LwrDrbPdcpInfo;

typedef struct lwrDrbRlcInfo
{
   S16                        qci;
   U8                        rlcMode;
   LwrTimerPollRetransmit    dlTpRetransmit;
   LwrPollPdu                dlPollPdu;
   LwrPollByte               dlPollByte;
   LwrMaxRetransThresh       dlMaxRetransThresh;
   LwrTimerReordering        dlTimerReordering;
   LwrTimerStatusProhibit    dlTimerStatusProhibit;
   LwrTimerPollRetransmit    ulTpRetransmit;
   LwrPollPdu                ulPollPdu;
   LwrPollByte               ulPollByte;
   LwrMaxRetransThresh       ulMaxRetransThresh;
   LwrTimerReordering        ulTimerReordering;
   LwrTimerStatusProhibit    ulTimerStatusProhibit;
   LwrSnFieldLength          ulSnFieldLength;
   LwrSnFieldLength          dlSnFieldLength;
} LwrDrbRlcInfo;

typedef struct lwrDrbLchInfo
{
   S16                        qci; 
   LwrPrioritisedBitRate     prioritisedBitRate;
   LwrBucketSizeDurn         bcketSizeDurn;
   U16                       lchPriority;
   U16                       nonGbrPfWgt;   
} LwrDrbLchInfo;

typedef struct lwrDrbQciInfo
{
   S16                        qci; 
   Bool                      qciSupported;
   U8                        resourceType;    /*!< GBR or Non-GBR */
   U8                        priotity;   /*!< QCI priority */
   U8                        pdb;      /*!< Packet Delay Budget */
   U8                        pler;     /*!< Packet Error Loss Rate */
   U8                        bhServiceGrp;   /*!< Backhual Service Group */
   U16                       schedulingType;   /*!< Scheduler type */
}LwrDrbQciInfo;
         
typedef struct lwrDrbDscpInfo
{
   S16                     qci; 
   U16                     dscp;
   U16                     port;   /*!< Need for Backhual feature */
}LwrDrbDscpInfo;
typedef struct lwrDrbPolicy
{
   U32                       reCfgType;
   U8                        qci; 
   LwrActType                action;   /*!< Config, Modify or Delete */
   LwrDrbQciInfo             qciInfo;
   LwrDrbLchInfo             lchInfo;
   LwrDrbRlcInfo             rlcInfo;
   LwrDrbPdcpInfo            pdcpInfo; 
   LwrDrbDscpInfo            dscpInfo;
} LwrDrbPolicy;

typedef struct lwrRabPolicyCfgGrp
{
   U32            reCfgType;   /*!<  Bit mask for Reconfiguration */
   U8             numSrbPolicies;
   U8             numDrbPolicies;
   U8             arpForEmergencyServ;
   LwrSrbPolicy   srbPolicy[LWR_MAX_SRBS];
   LwrDrbPolicy   drbPolicy[LWR_MAX_QCI];
} LwrRabPolicyCfgGrp;


typedef struct lwrEnbCacInfo
{
   U8                        dbIndex;
   Bool                      callCountCacUsage;
   U16                       maxEnbCallCount; 
   U16                       callCacThreshForNormal; 
   U16                       callCacThreshForEmerHo; 
   Bool                      checkUeIdUsage;
   Bool                      plmnEnbCacUsage;
   U16                       dfltRrmRntiSize;
   U16                       dfltRrmStartPreamble;
   U16                       dfltRrmNumPreamble; 
} LwrEnbCacInfo;

typedef struct lwrRrmCfgGrp
{
   LwrEnbCacInfo   enbCacInfo;
} LwrRrmCfgGrp;

typedef struct lwrCellUlSchdCfgGrp
{
   U8    maxUePerUlSf;       /*!< Max Allocations in a given SF */
   U8    maxUlUeNewTxPerTti; /*!< Max UEs to be considered for New Tx Alloc in UL */
   U32   pfsUlTptCoefficient; /*!< Uplink Throughput co-effiecient*/
   U32   pfsUlFairCoefficient;/*!< Uplink Fairness co-effiecient*/
   U32   dlMimoMode;
   U32   alpha;
   U32   beta;
   U32   gamma;
   U32   backhaulDelay;
   U32   ulSchedulingAlg;
   U32   ulQciWgtList[LWR_MAX_NUM_QCI]; /*!< uplink QCI weights*/
}LwrCellUlSchdCfgGrp;

typedef struct lwrCellDlSchdCfgGrp
{
   U32   reCfgType;
   U8    maxUePerDlSf;     /*!< Max UE to be considered for DL
                               scheduling in a TTI */
   U8    maxDlUeNewTxPerTti; /*!< Max UEs to be considered for New Tx Alloc in DL */
   U32    pfsDlTptCoefficient; /*!< Downlink Throughput co-effiecient*/
   U32    pfsDlFairCoefficient;/*!< Downlink Fairness co-effiecient*/
   U32   dlMimoMode;
   U32   alpha;
   U32   beta;
   U32   gamma;
   U32   backhaulDelay;
   U32   dlSchedulingAlg;
   U8    dlfsSchdType;
   U8    dlfsSchd;   /*to enable/disable DLFSS(Downlink Frequency Selective Scheduling)*/
   U32   dlQciWgtList[LWR_MAX_NUM_QCI]; /*!< downlink QCI weights*/
}LwrCellDlSchdCfgGrp;

/** @brief This structure holds periodic report configuration information for UTRAN
  - U32                        reCfgType;           
  - TknPres                    sfUsagePres;        
  - U8                         tReselection;       
  - U8                         tReselectionSfMedium;    Medium Speed State Scale Factors
  - U8                         tReselectionSfHigh;      High Speed State Scale Factors
                                                        
*/
typedef struct lwrCellSib6CfgGrp
{
   U32              reCfgType; 
   TknPres          sfUsagePres;
   U16              tReselection;
   U8              tReselectionSfMedium;
   U8              tReselectionSfHigh;
    
}LwrCellSib6CfgGrp;

/** @brief This enum holds Speed State information */
typedef enum lwrSpdStSclFactor
{
   LWR_O_DOT_25,
   LWR_O_DOT_5,
   LWR_O_DOT_75,
   LWR_1_DOT_0
} LwrSpdStSclFactor;


/** @brief This structure holds GERAN cell reselection information 
*/
typedef struct lwrCellSib7CfgGrp
{
   U32                reCfgType;                   /*!<  Bit mask for Reconfiguration */
   U8                 tReselectionGERAN;           /*!<  cell reselection timer */
   TknPres            pres_SpeedStateScaleFactors;
   LwrSpdStSclFactor  tReselectionGERANSFMedium;
   LwrSpdStSclFactor  tReselectionGERANSFHigh;
}LwrCellSib7CfgGrp;

/** @brief This structure holds SIB8 Config params
  - U8                        henbName;                                                                   
*/
typedef struct lwr1xRttCsfbRegInfo
{
   Bool pres;          /* Presence of this field */
   U16  sid;           /* System identifier */ 
   U16  nid;           /* Network identifier */
   Bool mulSID;        /* Multiple SID storage indicator */
   Bool mulNID;        /* Multiple NID storage indicator */
   Bool homeReg;       /* Home reg indicator */
   Bool foreignSIDReg; /* SID roamer registration indicator */
   Bool foreignNIDReg; /* NID roamer registration indicator */
   Bool paramReg;      /* Parameter change reg indicator */
   Bool powerUpReg;    /* Power up registration indicator */
   U8   regPeriod;     /* Registration period */
   U16  regZone;       /* Registration zone number */
   U8   totalZone;     /* Total number of registration zone */
   U8   zoneTimer;     /* Zone timer */
   U8   powerDownRegPres; /* Power down registration indicator */ 
}Lwr1xRttCsfbRegInfo;

/* Added structure for Access class barring */
typedef struct Lwr1xRttACBarringCfg
{
  Bool pres;
  U8 acBarring0to9_r9;
  U8 acBarring10_r9;
  U8 acBarring11_r9;
  U8 acBarring12_r9;
  U8 acBarring13_r9;
  U8 acBarring14_r9;
  U8 acBarring15_r9;
  U8 acBarringMsg_r9;
  U8 acBarringReg_r9;
  U8 acBarringEmg_r9;
}Lwr1xRttACBarringCfg;

typedef struct lwrCdma1xRttParams
{
   Lwr1xRttCsfbRegInfo    cdma1xrttRegParams; /* CDMA 1xRTT registration params*/
   Lwr1xRttACBarringCfg   cdma1xrttACBarringCfg; /* Access class barring params */
}LwrCdma1xRttParams;

typedef struct lwrCellSib8CfgGrp
{
   Bool                   cdmaEutraTimeSync;  /* If cdmaEutraTimeSync set as 
                                                 TRUE indicates that the 
                                                 networks are synchronised
                                                 else not synchronised */
   U8                     tReselection;       /* cell reselection timer */
   Bool                   speedStatePres;     /* Indicate SpeedStateScaleFactors
                                                 is present or not */
   U8                     sfMedium;       /* Medium Speed State Scale Factors */
   U8                     sfHigh;         /* High Speed State Scale Factors */
   LwrCdma1xRttParams     cdma1xrttParams; /* CDMA 1xRTT related config params */
}LwrCellSib8CfgGrp;

/** @brief This structure holds SIB9 Config params
  - U8                        henbName;                                                                   
*/
typedef struct lwrCellSib9CfgGrp
{
   U8 henbName[48];
}LwrCellSib9CfgGrp;
/** @brief This structure ANr Specific DRX configuration 
 *   - U8                    drxOnDurTmr;
 *   - U16                   longDrxCycle;   
 *   */
typedef struct lwrAnrDrxCfg
{
   U8                    drxOnDurTmr;
   U16                   longDrxCycle;   
   U8                    inActivTmr;
}LwrAnrDrxCfg; 

/* ANR MEasureemnt */
typedef struct lwrCellAnrCfgGrp
{
   TknU8          maxRankWght;    /*!< Maximum Rank Weight Default will be 10*/
   TknU8          minRankWght;    /*!< Minimum Rank Weight Default will be 8*/
   TknU32         epocTimerVal;/*!< Value corresponds to seconds*/
   U8             tricePeriodIntv; /* Number of interval per Epoc */
   U32            csfbTransTmrVal;/*!<Timer value for CSFB Measurements*/
   LwrAnrDrxCfg   ueAnrDrxCfg;   /* DRX Configuration of ANR */
   U8             anrCellValidAge; /* EUTRAN cell valid age in minutes */
} LwrCellAnrCfgGrp;

/* LTE_ADV starts */
/** @brief This structure holds RE Config params
  - RgrFeatureStatus   status;                                                                 
*/
typedef struct lwrReCfg
{
   RgrFeatureStatus   status; /*!< Indicate feature is enabled or disabled */
   U8                         neighQoffset;    /*!< Cell reselection offset to be given in SIB4 for neighbor eNB (Q-OffsetRange ::= ENUMERATED 
                                                  {dB-24, dB-22, dB-20, dB-18, dB-16, dB-14, dB-12, dB-10, dB-8, dB-6, 
                                                   dB-5, dB-4, dB-3, dB-2, dB-1, dB0, dB1, dB2, dB3, dB4, dB5, dB6, dB8, 
                                                   dB10, dB12, dB14, dB16, dB18, dB20, dB22, dB24}*/
   U8                         neighCIO;    /*!< Cell specific offset for neighbor eNB (Q-OffsetRange ::= ENUMERATED 
                                                  {dB-24, dB-22, dB-20, dB-18, dB-16, dB-14, dB-12, dB-10, dB-8, dB-6, 
                                                   dB-5, dB-4, dB-3, dB-2, dB-1, dB0, dB1, dB2, dB3, dB4, dB5, dB6, dB8, 
                                                   dB10, dB12, dB14, dB16, dB18, dB20, dB22, dB24}*/
}LwrReCfg;

/* LTE_ADV starts */
typedef struct lwrLteAdvancedConfig
{
   S8                         a3Offset;      /*!< A3 offset for MACRO eNB (a3-Offset INTEGER (-30..30))*/
   U8                         enbType;       /*!< eNB type for lte advanced functionality
                                                  (WR_LTE_ADV_PICO_ENB or WR_LTE_ADV_MACRO_ENB) */
/* LTE_ADV_ABS starts */
   RgrAbsConfig               absCfg;        /*!< Configuration of ABS feature */
/* LTE_ADV_ABS ends */
   RgrSfrConfig               sfrCfg;        /*!< Configuration of SFR feature */
   LwrReCfg                   reCfg;         /*!< Configuration of RE feature */
   RgrDsfrConfig              dsfrCfg;       /*!< LTE_ADV_DSFR Configuration of DSFR feature */
}LwrLteAdvancedConfig;
/* LTE_ADV ends */


typedef struct lwrLteAdvancedConfigTrans
{
   U32                        pres;   /*!< To indicate presence of feature config */
   LwrLteAdvancedConfig       lteAdvancedConfig;
}LwrLteAdvancedConfigTrans;
/* LTE_ADV ends */

typedef struct lwrCellSchdPwrCfg
{
   U16  pucchPwrFmt3Rnti;
   U16  pucchPwrFmt3Siz;
   U16  pucchPwrFmt3aRnti;
   U16  pucchPwrFmt3aSiz;
   U16  puschPwrFmt3Rnti;
   U16  puschPwrFmt3Siz;
   U16  puschPwrFmt3aRnti;
   U16  puschPwrFmt3aSiz;
   U8   pucchPwrFmt3aEnable;
   U8   puschPwrFmt3aEnable;
   U8   isUlGrpPwrCntrlPucchEnable;
   U8   isUlGrpPwrCntrlPuschEnable;
}LwrCellSchdPwrCfg;

typedef struct lwrCellSchdRachCfg
{
   U8 preambleFormat;
   U8 raOccSize;
   U8 raOccSfnEnum; 
   U16 prachResource;
}LwrCellSchdRachCfg;

/* SPS changes starts */
typedef struct lwrCellSchdSpsCfg
{
   Bool                       bIsSpsEnabled;     /*!< TRUE = SPS is Enabled and FALSE = SPS is diabled */
   U16                        maxDlSpsUePerTti;  /*!< Number of DL SPS User per TTI for new transmission */
   U16                        maxUlSpsUePerTti;  /*!< Number of UL SPS User per TTI for new transmission */
   U8                         maxDlBwInRbs;  /*!< Number of DL SPS RBs */
} LwrCellSchdSpsCfg;  
/* SPS changes ends */

typedef struct lwrCellSchdGenCfg
{
   U8                  macInst;
   U8                  maxUePerUlSf;   /*!< Maximum UEs that may be scheduled
                                            per uplink subframe. Currently this is
                                           unused parameter */
   U8                  maxUePerDlSf;   /*!< Maximum UE to be considered for DL 
                                            scheduling in a TTI.Currently this is
                                            unused parameter */
   U8                  maxUlBwPerUe;   /*!< Maximum number of RBs that can be  
                                           allocated to an UE in an UL subframe
                                           Maximum value is defined by 
                                           RG_SCH_CMN_MAX_UL_BW_PER_UE in   
                                           rg_env.h. This can be modified as
                                           per need basis */
   U8                  maxDlBwPerUe;   /*!< Maximum number of RBs that can be    
                                        allocated to an UE in an DL subframe  
                                        Maximum value is defined by     
                                        RG_SCH_CMN_MAX_DL_BW_PER_UE in
                                             rg_env.h. This can be modified as 
                                         per need basis */
   U8                  maxDlRetxBw;    /*!< Maximum number of RBs that can be     
                                        allocated for retransmission in DL
                                        Maximum value is defined by 
                                        RG_SCH_CMN_MAX_DL_RETX_BW in   
                                        rg_env.h. This can be modified as   
                                        per need basis */
   U8                  maxDlUeNewTxPerTti; /*!< Maximum number of UEs that can  
                                            be scheduled for a new DL    
                                            transmission in a TTI. Value should
                                            be configured by the user as per  
                                            specific needs */ 
  U8                  maxUlUeNewTxPerTti;  /*!< Maximum number of UEs that can */
  Bool                cellModSchm;
  U8                   dlfsSchdType;
            
}LwrCellSchdGenCfg;

typedef struct lwrCellSchdCfg
{
   U8 srsBwEnum;
   U8  prdEnum;
   U8 modPrd;
   U8  siWinSize;  /*!< SI window size */
   U8  retxCnt;    /*!< Retransmission count */
   LwrCellSchdPwrCfg pwrCfg;
   Bool    preamPres;      /*!< Indicates if other configuration fields are valid */
   U8      preamStart;     /*!< Start Preamble ID for the range managed by MAC */
   U8      preamSize;      /*!< Indicates contiguous range of premables managae*/
   LwrCellSchdRachCfg rachCfg;
   LwrCellSchdGenCfg  genCfg;
   /* SPS changes starts */
   LwrCellSchdSpsCfg  spsCfg;
   /* SPS changes ends */
   Bool isDynCfiEnb;   /*!< Indicate whether Dynamic CFI feature is enabled or not */
}LwrCellSchdCfg;

typedef struct lwrRgrCfgGrp
{
  U16 dfltStartRnti;
  U16 dfltRntiSize;
  U8  dfltCfiCfg;
  U8  dfltTrgUlCqi;
  U16 dfltBcchPchRaCodeRate;
  U16 dfltPdcchCodeRate;
  U8  dfltPuschSubBndStrt;
  U8  dfltPuschNoSubbnds;
  U8  dfltULCmnCodRateCcchCqi;
  U8  dfltDLCmnCodRateCcchCqi; 
  U8  dfltIsDlFreqSel;
  U8  dfltThresholdCqi;
  U8  dfltMaxDlHqTx;
  U8  dfltMaxMsg4HqTx;
  U8  dfltPuschSubBandSz; 
  U8  dfltPuschSubBandDmrs;
  U8  dfltMaxPucchRbs;  
}LwrRgrCfgGrp;

/**
 *  * @enum ctfSCSpacing
 *   * This element enumerates the sub-carrier spacing for configuration at PHY. 
 *   
typedef enum {
   CTF_SC_SPACING_15KHZ,
   CTF_SC_SPACING_7DOT5KHZ
} CtfSCSpacing;*/

typedef struct lwrCtfCfgGrp
{
   CtfSCSpacing    dfltScSpacing;
   U32             dfltOpMode;
   U32             dfltTagPeriod;
}LwrCtfCfgGrp;

typedef struct lwrDlRlcModeInfo
{
   U8  dfltAmRlcSrbmaxRetxThresh;
   U16 dfltAmRlcSrbmaxpollByte;
   U16 dfltAmRlcSrbpollPdu;
   U16 dfltAmRlcSrbtPollRetx;
   U8  dfltAmRlcDrbmaxRetxThresh;
   U16 dfltAmRlcDrbmaxpollByte;
   U16 dfltAmRlcDrbpollPdu;
   U16 dfltAmRlcDrbtPollRetx;
   U8  dfltUmRlcsnFieldLen;
   U8  srbtReordring;
   U8  drbtReordring;
}LwrDlRlcModeInfo;

typedef struct lwrUlRlcModeInfo
{
   U8  dfltAmRlcSrbtStatusProh;
   U8  srbtReordring;
   U8  dfltAmRlcDrbtStatusProh;
   U8  drbtReordring;
   U8  dfltUmRlcsnFieldLen;
}LwrUlRlcModeInfo;

typedef struct lwrRlcCfgGrp
{
   LwrDlRlcModeInfo dlModeInfo;
   LwrUlRlcModeInfo ulModeInfo;
}LwrRlcCfgGrp;

typedef struct lwrAddPwrCntrlParams
{
   Bool  deltaMCSenabled;
   S32   p0UePUSCH;
   S32   p0UePUCCH;
   Bool  accumulationEnabled;
   U32   pSRSoffset;
   U32   filterCoefficient;
}LwrAddPwrCntrlParams;
/** @brief This structure contains the Cell Cfg Group 
 */
typedef struct lwrCellCfgGrp
{
   LwrCellCfgGrpType      grpType;   /*!< Group Type */
   union
   {
      LwrCellMibCfgGrp      mibCfgGrp;   /*!< Groups information sent in MIB */
      LwrCellSib1CfgGrp     sib1CfgGrp;  /*!< Groups information sent in SIB1 */
      LwrCellSib2CfgGrp     sib2CfgGrp;  /*!< Groups information sent in SIB2 */
      LwrCellSib3CfgGrp     sib3CfgGrp;  /*!< Groups information sent in SIB3 */
      LwrCellSib4CfgGrp     sib4CfgGrp;  /*!< Groups information sent in SIB4 */
      LwrCellSib6CfgGrp     sib6CfgGrp;  /*!< Group Information sent in SIB6*/
      LwrCellSib7CfgGrp     sib7CfgGrp;  /*!< Group Information sent in SIB7*/
      LwrCellSib8CfgGrp     sib8CfgGrp;  /*!< Group Information sent in SIB8 */
      LwrCellSib9CfgGrp     sib9CfgGrp;  /*!< Group Information sent in SIB9*/
      LwrCellAnrCfgGrp      anrCfgGrp;   /*!< ANR configaration */
      LwrCellMeasCfgGrp     measCfgGrp;  /*!< Measurement configaration */
      LwrCellEAIDCfgGrp     eaidCfgGrp;    /*!< Emergency Area IDs Config */
      LwrCellSibSchedCfgGrp sibSchedCfgGrp;   /*!< SIB Scheduling Information */
      LwrRabPolicyCfgGrp    rabPolicyCfgGrp;
      LwrProcTimerCfgGrp    procTimerCfgGrp;   /*!< TIMERs for Procedures */
      LwrRrmCfgGrp          rrmCfgGrp;   /*!< TIMERs for Procedures */
      LwrCellUlSchdCfgGrp   ulSchdCfgGrp;  /*Stores Ul Scheduler related Cfg*/
      LwrCellDlSchdCfgGrp   dlSchdCfgGrp;  /*Stores Dl Scheduler related Cfg*/
      LwrRgrCfgGrp          rgrCfgGrp;     
      LwrCtfCfgGrp          ctfCfgGrp;
      LwrRlcCfgGrp          rlcCfgGrp;
      LwrCellSchdCfg        schdCfgGrp;
      LwrAddPwrCntrlParams  uLPwrCtrl;
      LwrCsfbCfgGrp         csfbCfgGrp;   /* Stores CSFB/eCSFB related config */
      LwrLteAdvancedConfigTrans  lteAdvCfgGrp;
   }t;
} LwrCellCfgGrp;

typedef struct lwrLowPwrCfg
{
   S8    lowRefSigPwr;
   S16   lowPilotSigPwr;
   S16   lowPriSigPwr;
   S16   lowSecSigPwr;
}LwrLowPwrCfg;

typedef struct lwrHighPwrCfg
{
   S8    highRefSigPwr;
   S16   highPilotSigPwr;
   S16   highPriSigPwr;
   S16   highSecSigPwr;
}LwrHighPwrCfg;
typedef struct lwrShutdownPwrCfg
{
   LwrLowPwrCfg   lwPwrCfg;
   LwrHighPwrCfg  hiPwrCfg;
}LwrShutdownPwrCfg;

/* CSG_DEV start */
typedef enum
{
   WR_ENB_OPEN_ACCESS = 1,
   WR_ENB_CLOSED_ACCESS,
   WR_ENB_HYBRID_ACCESS
} LwrEnbAccessMode;

#define WR_MAX_CSG_PHY_CELL_ID_RANGE         13

#define WR_LWR_NON_CSG_UE_ACC_DECSN_BY_CORE  0x01

typedef struct lwrCsgPciRangeInfo
{
   Bool                      pciRangeInfoPres;
   U16                       startPci;
   U16                       pciRange;
} LwrCsgPciRangeInfo;
/* CSG_DEV end */
typedef LnlSonCfg  LwrSonCfg;
typedef LnlSonSchdCfg LwrSonSchdCfg;

typedef struct lwrHCsgParam
{
   U8          minDlResNonCsg;
   U8          minUlResNonCsg;
   U16         maxCsgUeSup;
   U16         maxNonCsgUeSup;
}LwrHCsgParam;
typedef struct lwrTxnModeInfo
{
   U8   txPrefMode;     /*!< indicates the tranmission mode */
   Bool isAutoCfgModeEnb; /*!< Indicate whether Auto config is enabled or not */
} LwrTxnModeInfo;
/** @brief This structure is to add a new configuration. It shall contain all
 * possible configuration coming from the OAM to the Node.
 * - It will be an array of cellCfg Groups each one carrying a bunch of
 * information that is logically connected. 
 * - Updates shall happen in a grouped manner i.e. an update might only change
 * RACH parameters only. 
 * - This allows for a simpler design at the receiving side
 * where we will not have to compare the whole structure with the existing
 * configuration to figure out what parameters have changed. 
 */
typedef struct lwrAddCellCfg
{
   U32            reCfgType;   /*!<  Bit mask for Reconfiguration */
   U8             cellIndex;   /*!< Cell index */
   U32            cellId;      /*!< Cell ID */
   U16            physCellId;  /*!< Physical cell id */
   U16            numOfGrps;   /*!< Number of Groups present in the array */
   U8             duplexMode;  /*!< Duplex Mode of Operation */
   U16            dlCarrierFreq;          /*!< Dl Carrier Freq */
   LwrCsfbSupport  measFlagForCsfbUtra;   /*!< Meas Flag for CSFB to UTRA */
   LwrCsfbSupport  measFlagForCsfbCdma;   /*< Meas Flag for CSFB to CDMA */ 
   LwrEcsfbSupport measFlagForEcsfb;      /*< Meas Flag for CSFB to ECSFB */
   LwrCellCfgGrp  *cellCfgGrp[WR_CELL_CFG_MAX_GRPS];   /*!< Array of pointers groups of
                                                     configuration */

   
   LwrShutdownPwrCfg  shutdownPwrCfg;
   /*DRX QCI*/
   Bool           isDRXQCIEnabled;
   /* CSG_DEV start */
   U8                 csgAccessMode; /*!< Access Mode */
   LwrCsgPciRangeInfo smallCellCsgPciRange; /*!< Smallcell PCI Info */
   LwrHCsgParam  hcsgParam; /*!< HCSG parameters */
   /* CSG_DEV end */

   /* UE history info - start */
   LwrEnbCellType  cellSizeType;   /*!< Cell Type */
   /* UE history info - end */
   U8   txPrefMode;     /*!< indicates the tranmission mode */
   Bool isAutoCfgModeEnb; /*!< Indicate whether Auto config is enabled or not */
   LwrSonCfg           sonCfg;   /*!< SON configuration like:scheduler configuration 
                                      related to PRB and PA*/
} LwrAddCellCfg;
/** @brief This structure defines the cells to be deleted from the eNodeB
 */
typedef struct lwrDelCell
{
   U8       cellIndex;   /*!< Cell index */
} LwrDelCell;

/** @brief This structure is Cell configuration
 */
typedef struct lwrCellCfg
{
   LwrActType        action;   /*!< Config, Modify or Delete */
   union
   {
      LwrAddCellCfg      addCell;   /*!< New Cell configuration */
      LwrAddCellCfg      modCell;   /*!< Modify existing cell configuration */
      LwrDelCell         delCell;   /*!< Delete the cell */
   }t;
} LwrCellCfg;

typedef enum lwrNeighEnbState
{
   WR_NEIGH_ENB_STATE_DOWN = 1,
   WR_NEIGH_ENB_STATE_UP   = 2
} LwrNeighEnbState;

typedef struct lwrGuGrpIdLst
{
   LwrPlmnId                 plmnId;
   U16                       mmeGrpId;
} LwrGuGrpIdLst;

/** @brief This structure provides the information about the EUTRA neigbour to
 * the ENodeB.
 * @todo the first parameter i.e. the enbIndex is the value that should be know
 * by both the EnodeB Application and the stack manager the reason for this
 * extra parameter is that all other identities such as mmeGi, mmeCode can
 * potentially be reconfigured hence we do need a unique identification for the
 * EnodeB and this is the purpose served by the enbIndex. 
 */
typedef struct lwrEUTRANeighCfg
{
   U8                enbIndx;    /*!< Index of the EnodeB Added/Modified */
   U32               enbId;      /*!< EnodeB Id 20/28 bits starting from LSB */
   LwrEnbType        enbType;    /*!< MACRO/Home */
   LwrPlmnId          plmn;       /*!<PLMN DI*/
   CmTptAddr         ipAddr;     /*!< Primary IP address */
   CmTptAddr         secIpAddr;  /*!< Secondary IP address */
   LwrNeighEnbState  state;      /*!< State of the neighbour cell */
   Bool              isX2Reqd;       /*!< Whether X2 connection should be established(FALSE) or Not(TRUE) */
   Bool              noHO;       /*!< Whether HO to neighbor eNB is performed(FALSE) or Not(TRUE) */
   Bool              noDirectFwd; /*!< Indicates Flag for Approve Direct Forwarding */
#if 0
   U8 noGrpIdLst;
   LwrGuGrpIdLst              guGrpIdLst[LWR_MAX_GU_GRP_IDS];
#endif
} LwrEUTRANeighCfg;

/** @brief This structure defines the CDMA 2K neighbour 
 * @todo define this structure 
 */
typedef struct lwrCDMA2KNeighCfg
{
   U8    cellIndex;   /*!< Cell index */
} LwrCDMA2KNeighCfg;

typedef enum lwrNeighType
{
   WR_NEIGH_CELL_EUTRA,
   WR_NEIGH_CELL_CDMA2K_1XRTT,
   WR_NEIGH_CELL_CDMA2K_HRPD,
   WR_NEIGH_CELL_UTRA_FDD,
   WR_NEIGH_CELL_UTRA_TDD,
   WR_NEIGH_CELL_GERAN  
} LwrNeighType;

typedef enum lwrNeighCfgType
{
   WR_NEIGH_CELL_CFG_INITIAL = 1,
   WR_NEIGH_CELL_CFG_DYNAMIC,
   WR_NEIGH_CELL_CFG_ANR
}LwrNeighCfgType;

/** @brief This structure provides information about the neighbour cells to the
 * ENodeB.
 */
typedef struct lwrNeighAddModCfg
{
   U8                enbIndx;    /*!< Index of the EnodeB Added/Modified */
   U32               peerId;     /*!< Peer Id assigned for this EnodeB */
   U32               enbId;      /*!< EnodeB Id 20/28 bits starting from LSB */
   LwrEnbType        enbType;    /*!< MACRO/Home */
   LwrPlmnId          plmn;       /*!<PLMN DI*/
   CmTptAddr         ipAddr;     /*!< Primary IP address */
   CmTptAddr         secIpAddr;  /*!< Secondary IP address */
   LwrNeighEnbState  state;      /*!< State of the neighbour cell */
   LwrX2ConFlg       isX2Reqd;       /*!< Whether X2 connection should be established */
/* LTE_ADV_DSFR starts */
   RgrSfrRbRange     dsfrCEFreqRange; /* Cell edge RB range */
/* LTE_ADV_DSFR ends */
#if 0
   U8 noGrpIdLst;
   LwrGuGrpIdLst              guGrpIdLst[LWR_MAX_GU_GRP_IDS];
#endif
   WrFreqAlarmVal        alarmVal;
} LwrNeighAddModCfg;

/** @brief This structure is to delete an existing neigbour cell */
typedef struct lwrNeighDel
{
      U8        enbIndx;    /*!< Index of the EnodeB To be Deleted */
      U32       enbId;   /*!< Index of the Cell To be Deleted */
} LwrNeighDel;

#define LWR_MAX_NBR_ENB 25
typedef struct lwrNeighCfg
{
   U16             cellId; /*!< Serving cell Id */
   LwrActType      action;   /*!< ADD/MOD/DEL */
   U8              numEnb;
   union
   {
      LwrNeighAddModCfg  neighAdd[LWR_MAX_NBR_ENB];   /*!< To ADD neighbour cell */
      LwrNeighAddModCfg  neighMod[LWR_MAX_NBR_ENB];   /*!< To Modify existing cell */
      LwrNeighDel        neighDel[LWR_MAX_NBR_ENB];   /*!< To Delete existing cell */
   }t;
} LwrNeighCfg;

typedef struct lwrMmeLdBalCfg
{
   U16            mmeGroupId;
   U16            mmeGrpPrior;
}LwrMmeLdBalCfg;

typedef struct  lwrSctpParamCfg
{
	U32 mmeFailoverTimer;
	U32 heartBeatInterval;
	U32 numPathReTx;
	U32 rtoInitial;
	U32 rtoMin;
	U32 rtoMax;
	U32 connectInterval;
   U32 maxInitRetx;
   U32 maxAssocRetx;
   U32 cookieLife;
   U32 localPort;
}LwrSctpParamCfg;


/** @brief This structure defines the ENodeB Group ID configuration 
 */
typedef struct lwrEnbGrpInfo
{
   LwrPlmnInfo    plmnId;   /*!< PLMN ID */
   U16            mmeGId;   /*!< MME Group ID */
} LwrEnbGrpInfo;

typedef struct lwrServedCellCfg
{
   /*lwr*/
   U16         pci;

} LwrServedCellCfg;

/** @brief This structure protocol specific options from the OAM/
 */
typedef struct lwrEnbProtoCfg
{
   CmTptAddr         enbIpAddr[WR_MAX_IP_TYPES]; /*!< eNB IP Address */
   U16               sctpPortNumS1;  /*!< The port number to be used in SCTP 
                                          association configuration for S1AP 
                                          with the MME*/
   U16               sctpPortNumX2;  /*!< The port number to be used in SCTP 
                                          association configuration for X2AP 
                                          with the neighbor eNB */
 
   U32               bhLinkCapacity; /*!< Total backhaul link of Mhe eNB */
   TknStrOSXL        enbName;        /*!< EnodeB name */

   LwrEnbType        enbType;        /*!< EnodeB type */
   U32               enbId;          /*!< EnodeB Id 20/28 bits starting from LSB */
   U32               numPlmns;
   LwrEnbPlmnInfo       plmns[LWR_MAX_PLMN_IDS];
   /* RAT Priority for CSFB for the PLMN */
} LwrEnbProtoCfg;


#ifdef ENB_OVRLOAD
typedef struct lwrEnbOvldParams
{
   U8   critThrldVal;            /*----!< EnodeB Overload Critical Threshold value --*/
   U8   majThrldVal;             /*----!< EnodeB Overload Major threshold value --*/
   U8   minThrldVal;             /*----!< EnodeB Overload minor threshold value --*/
   U8   critRcvryVal;            /*----!< EnodeB Overload recovery Critical threshold  value --*/
   U8   majRcvryVal;             /*----!< EnodeB Overload recovery Major threshold  value --*/
   U8   minRcvryVal;             /*----!< EnodeB Overload recovery Minor threshold  value --*/
   U8   eNBMemCritThresVal;      /*----!< EnodeB Overload Critical threshold  value --*/
   U8   eNBMemMajThresVal;       /*----!< EnodeB Overload Major threshold  value --*/
   U8   eNBMemMinThresVal;       /*----!< EnodeB Overload Minor threshold  value --*/
   U32  eNBOvldMonTimerVal;      /*----!< EnodeB Overload monitor timer value --*/
   U32  eNBOvldMonfreq;          /*----!< EnodeB Overload monitor Frequency --*/
   U8   maxCntval;               /*----!< EnodeB Overload Cnt value before alarms is raised --*/
   Bool eNbOvrLdFlag;            /*----!< EnodeB overload Feature Flag --*/
} LwrEnbOvldParams;
#endif
/** @brief This structure 
 - U16  longDrxCycle   This parameter specifies the Long DRX cycle
 - U16  drxStartOffst  This parameter specifies the DRX Start offset
 */
typedef struct lwrDrxLongCycleOffst
{
   U16      longDrxCycle;   
   U16      drxStartOffst;  
} LwrDrxLongCycleOffst;

/** @brief This structure 
 - Bool    pres        This paramter indicates the presence of short
                       DRX
 - U16     shortDrxCycle This parameter specifies the short DRX cycle
 - U8      drxShortCycleTmr This parameter specifies the short DRX
                            timer value
 */
typedef struct lwrDrxShortDrx
{
   Bool    pres;             /*!< Short cycle is configured or not */
   U16     shortDrxCycle;    /*!< DRX Short Cycle value in sub-frames*/
   U8      drxShortCycleTmr; /*!< Value in multiples of Short DRX Cycles*/
}LwrDrxShortDrx;



/** @brief This structure holds the Drx Configuration
 - Bool                   isDrxEnabled       Specifies whethere DRX is enabled or not
 - TknS32                 cqiMask            Specifies the CQI mask
 - U8                     drxOnDurTmr        Specifies the Duration of Drx On timer
 - U16                    drxInactvTmr       Specifies the Drx Inactivity timer
 - U8                     drxRetxTmr         Specifies the DRX Retransmission timer
 - WrDrxLongCycleOffst    drxLongCycleOffst  Specifies the long cycle Offset
 - WrDrxShortDrx          drxShortDrx        Specifies the short cycle information
 */
typedef struct lwrDrxCfg
{
   Bool                  isDrxEnabled;
   TknS32                cqiMask;
   U8                    drxOnDurTmr;
   U16                   drxInactvTmr; 
   U8                    drxRetxTmr; 
   LwrDrxLongCycleOffst   drxLongCycleOffst;
   LwrDrxShortDrx         drxShortDrx;
}LwrDrxCfg;

 /** @brief This structure holds the MacMainSch Configuration
 - U32 maxHARQ_Tx        Specifies Maximum number of transmissions for UL HARQ
 - U32 periodicBSR_Timer Specifies Timer for BSR reporting
 - U32 retxBSR_Timer     Specifies Timer for BSR reporting
 - U32 ttiBundling       Specifies indicates that TTI bundling enable or disable
 */ 
typedef struct lwrMacMainSchCfg
{
  U32 maxHARQ_Tx;
  U8  deltaHqOffset;
  U32 periodicBSR_Timer;
  U32 retxBSR_Timer;
  U8  ttiBundling;
}LwrMacMainSchCfg;
  /** @brief This structure holds the MacMainPhr Configuration
 -  U32 periodicPHR_Timer Specifies  Timer for BSR reporting 
 -  U32 prohibitPHR_Timer Specifies  Timer for PHR reporting
 -  U32 dl_PathlossChange Specifies  DL Pathloss Change for PHR reporting
 */
typedef struct lwrMacMainPhrCfg
{
   U32 periodicPHR_Timer;
   U32 prohibitPHR_Timer;
   U32 dl_PathlossChange;
}LwrMacMainPhrCfg;
 /** @brief This structure holds the MacMainPhr Configuration
 -  U32              reCfgType Bit mask for Reconfiguration 
 -  LwrMacMainSchCfg ulSchCfg Specifies MacMainSch Configuration 
 -  LwrDrxCfg        drxCfg Specifies the Drx Configuration  
 -  U32              timeAlignmentTimerDedicated Specifies how long 
 -                             the UE is considered uplink time aligned.
 -  LwrMacMainPhrCfg phr_Config Specifies MacMainPhr Configuration 
 -  TknU32           srProhibitTimer Specifies Timer for SR transmission on PUCCH             
 */
typedef struct lwrMacMainConifg
{
  U32    reCfgType;   /*!<  Bit mask for Reconfiguration */
  LwrMacMainSchCfg ulSchCfg; 
  U8             timeAlignmentTimerDedicated;
  LwrDrxCfg       drxCfg;
  LwrMacMainPhrCfg phr_Config;
  TknU32           srProhibitTimer;
}LwrMacMainConifg;         

/** @brief This structure defines the PDSCH configuration are used to specify the common and the UE
 * specific PDSCH configuration respectively
 *   * @todo this values are to be determined by OAM
 *   *to be populated over the OAM interface.
 **/

typedef struct lwrPdschCfgDedicated
{
   U8 pA;
}LwrPdschCfgDedicated;

 /** @brief This structure defines the
 *   *Value to be populated over the OAM interface.
 *   U8 intgAlgo Indicates the integrity protection algorithm to be used for SRBs
 *   U8 ciphAlgo Indicates the ciphering algorithm to be used for SRBs and DRBs
 *     */

typedef struct lwrSecAlgo
{
   U8 intgAlgo;
   U8 ciphAlgo;
}LwrSecAlgo;     

/*129311 : enum for CQI format type*/
typedef enum wrCqiFormatInd
{
   WR_UE_CQI_FMT_WB = 1,
   WR_UE_CQI_FMT_SB
}WrCqiFormatInd;

/** @brief This structure 
  -   U32            cqiAperiodicMode     Aperiodic CQI mode
  -   S8             nomPdschRsEpreOff    Nominal PDSCH RS EPRE offset
  -   U8             periodRptSetupPres   TRUE if preiodic report is configured as TRUE
  -   U16            cqiPucchRsrcIdx      PUCCH resource for CQI
  -   U16            cqipmiCfgIdx         CQI/PMI configuration Index 
  -   U8             subbndCqik           Value of k for subband CQI
  -   WrCqiFormatInd    fmtInd               Format type
  -   U16            riCfgIdx             RI configuration Index
  -   Bool           simulAckNackAndCQI   TRUE if support for simultaneous 
                                    HARQ feedback with CQI
  -   U32            periodicmode         Periodic CQI reporting mode
 */
typedef struct wrCqiRptCfg 
{
      Bool            aperCqiEnbld;
      U32		       cqiAperiodicMode;
      S8		          nomPdschRsEpreOff;
      U8       		 periodRptSetupPres;
      U16      		 cqiPucchRsrcIdx;
      U16      		 cqipmiCfgIdx;
      U8       		 subbndCqik;
      WrCqiFormatInd  fmtInd;
      U16      		 riCfgIdx;
      Bool     		 simulAckNackAndCQI;
      U32      		 periodicmode;
      Bool     		 riPres;

}WrCqiRptCfg;

typedef struct lwrSchReqCfg
{
      U16       srPucchRsrcIdx;
      U16       srCfgIdx;
      U32       dsrTransMax;
}LwrSchReqCfg;

typedef struct lwrUeUlPwrCfg 
{
   Bool            pres;           
   CmLteRnti       tpcRnti;         
   U8              idx;
   U8              isFmt3a;
}LwrUeUlPwrCfg;

typedef struct lwrPuschDedCfg
{
   U8                        ackIdx;/*ACK's Beta offset value*/
   U8                        riIdx;/*Rank Index Beta offset value*/
   U8                        cqiIdx;/*CQI Beta offset value*/
   LwrUeUlPwrCfg             puschPwrCfg;
}LwrPuschDedCfg;

typedef struct lwrQosCfg
{
   Bool ambrPres;
   U32  dlAmbr;
   U32  ueBr;
} LwrQosCfg; 

/* This structure holds the CSFB related info for GERAN */
typedef struct lwrCsfbGeranCfg
{
   Bool            isGERANMeasAllowedForCCO; /* Flag to set GERAN Meas for CCO
   */
   Bool            isCCOAllowed; /* Flag to set to allow CCO */
   U32             rrcDatCfmWaitTmr; /* Timer value for RRC Dat Cfm wait time */
   U32             GERANMeasWaitTmr; /* Geran Meas wait time  for report Rx */
   U32             T304expWaitTmrVal; /* T304 wait timer value for CCO */
} LwrCsfbGeranCfg;

/** @brief This structure contains the node level configuration coming from the
 * OAM interface. 
 */
typedef struct lwrENodeBCfg
{
   U32             reCfgType;   /*!< Bit mask each param corresponds to a
                                       bit. 
                                       - LSB corresponds to first parameter
                                       within the group.
                                       */
   U8              maxNumOfUe;   /*!< Maximum number of UEs to be admitted nosw*/
   U8              numTxAntenna; /* Number of Antenna ports*/ 
#if 0
   U8              numOfGrps;    /*!< Number of ENodeB groups */
   LwrEnbGrpInfo   grpCfgLst[LWR_MAX_ENODEB_POOLS];   /*!< List of Group IDs */
#endif
   LwrEnbProtoCfg  protoCfg;     /*!< Protocol configuration */

   U8              numOfMmePriGrps;
   LwrMmeLdBalCfg  mmeLdBalCfg[LWR_SM_MAX_MMES];
   U8              cpuLoad;      /*!< CPU Load, it comes in Enb modification */
   U32             enbCfgTransTmrVal;/*!<Timer value for Enb Config transfer msg*/
   LwrMacMainConifg macMainCfg;      /*Mac-MainConfig configuration*/
   LwrPdschCfgDedicated pdschCfgDedicated;  /*!< @todo PDSCH dedicated configuration */
   WrCqiRptCfg               cqiRptCfg;
   LwrDrbPdcpInfo            pdcpInfo; 
   LwrSchReqCfg              schReqCfg;
   LwrPuschDedCfg            puschDedicated;
   LwrUeUlPwrCfg             pucchPwrCfg;
   LwrQosCfg                 qosCfg; 
   LwrX2ConFlg               x2ConFlg;
   U8                        pwsEtwsCmas; /* To enable ETWS or CMAS or disable both*/
   U8                        avgFreeCpuRqd; /* Avg free cpu required */
   /* CNM_DEV */
   U8                        isCnmEnabled;
   LwrCsfbGeranCfg           csfbToGeranCfg; /* CSFB to Geran params */
} LwrENodeBCfg;

/** @brief This structure defines the configuration that is related to the
 * ENodeB and not for a particular Cell within it.
 */
typedef struct lwrENbCfg
{
   LwrActType      action;   /*!< Configure/ReConfigure/Reset */
   union
   {
      LwrENodeBCfg      eNodeBCfg;       /*!< Configuration for ENodeB */
      LwrENodeBCfg      eNodeBReCfg;     /*!< ReConfiguration for ENodeB */
      /*LwrENodeReset     eNodeBReset;*/     /*!< Reset for ENodeB */
   }t;
} LwrENbCfg;

typedef struct LwrSib2PrachCfg
{
   U16                       rootSeqIdx;
   U8                        prachCfgIdx;
   U8                        highSpeedFlag;
   U8                        zeroCorrZone;
   U8                        prachFreqOffset;
} LwrSib2PrachCfg;

typedef struct lwrEcgi
{
   LwrPlmnId plmnId; /* 24 bit long */
   U32                       eutranCellId; /* 28 bit long */
} LwrEcgi;

typedef struct lwrEUTRANeighCell
{
   U32                        reCfgType;
  
   U8                         status;  
   U8                         cellNum; /**< This is Cell number for changing */
   U8                         neighCellId;  /* For Macro Enodeb this cell Id will be used to build ECGI */
   U8                         nrIndex; /**< Index to the Neighbor Cell Info */
                              
   U32                        enbId; /**< This is eNB ID of basestation affiliated with
                                      *   E-UTRAN Neighbor Cell on around basestation
                                      *   if enbType value is Macro eNB, eNB ID is 20bit,
                                      *   if enbType value is Home eNB, eNB ID is 28bit
                                      *   Used when creating Cell Identifier
                                      */
   LwrEnbType                 enbType; /**< This is basestation type information affiliated
                                           *   with E-UTRAN Neighbor Cell on around basestation
                                           *   - ci_Macro_eNB: Macro eNB
                                           *   - ci_Home_eNB: Home eNB
                                           */
   LwrPlmnId                  enbPlmn; /**< This is the PLMN List information(MNC) of
                               *   ENB in the neighbor cell belongs.
                               */
                              
   U16                        pci; /**< This is Physical Cell ID of E-UTRAN Neighbor Cell on
                                    *   around basestation
                                    */
   U16                        tac; /**< This is Tracking Area Code of E-UTRAN Neighbor Cell
                                    *   on around basestation
                                    */
   U8                         numPlmn; /**< Number of PLMN ID present for the cell */
   LwrPlmnId                  plmnId[LWR_MAX_PLMN_IDS]; /**< This is Broadcast PLMN List information(MNC) of
                                        *   E-UTRAN Neighbor Cell on around basestation.
                                        */
   U16                        earfcnDl; /**< This is E-UTRAN Neighbor Cell.
                                         *   Downlink EARFCN(E-UTRAN Absolute Radio Frequency
                                         *   Channel Number) on  around basestation
                                         */
   U16                        earfcnUl; /**< This is E-UTRAN Neighbor Cell.
                                         *   Uplink EARFCN on around basestation
                                         */
   LwrBandwidth               dlBw; /**< This is Downlink Bandwidth information of
                                    *   E-UTRAN Neighbor Cell on around basestation.
                                    */
   LwrBandwidth               ulBw; /**< This is Uplink Bandwidth information of
                                    *   E-UTRAN Neighbor Cell on around basestation.
                                    */
   LwrQoffRange               cio;  /**<  indOffset : This is  Cell Individual Offset applyed to
                                       *   E-UTRAN Neighbor Cell on around basestation.
                                       *   that use when terminal is measuring on RRC
                                       *   Connected mode */
   LwrQoffRange               qoffset; /**<  qOffsetCell : This is Cell Quality Offset value
                                       *    applied to E-UTRAN Neighbor Cell on around base
                                       *    station. That use Whenreselecting Cell of
                                       *    terminal on RRC Idle mode
                                       */
   U8                         blackListed;/*  neighbor cell is allowed for UEs as handover target or not.
                                           If {{true}}, handover is prohibited towards this cell.
                                           If {{false}}, handover is allowed toward this cell*/
   S8                         refSigPower;/*The downlink reference-signal transmit power*/
   /* CSG_DEV start */
   U8                         isCsgIdResolved; /**< Flag to indicate if CSG ID
                                                 *  is resolved */
   U32                        csgId; /**< CSG Identity */
   U8                         isCsgAccessModeResolved; /**< Flag to indicate if
                                                         *  Access Mode is
                                                         * resolved */
   U8                         csgAccessMode; /**< CSG Access Mode */
   /* CSG_DEV end */
   U8           numAntenna; /* Antenna count of the neihbour */
   U8           ulDlSubFrameCfg; /* Subfame config of the neibhour */
   U8           splSfCfg; /* Special subframe config of the neighbour */
   U8           cpDl; /* DL cyclic prefix */
   U8           cpUl; /* UL cyclic prefix */
   S16          rsrp; /* RSRP in the range of -128 to 0 */
   S16          rssi; /* RSSI in the range of 0 to 256 */
   U16                         oldPci;
   U16                         oldEarfcnDl;
   U8                          isHeMsCell;
}LwrEUTRANeighCell;

typedef enum lwrCdmaType{
   LWR_1XRTT,
   LWR_HRPD
}LwrCdmaType;

typedef enum lwrBandType{
   LWR_NEIGH_BAND_TYPE_CDMA_1XRTT,
   LWR_NEIGH_BAND_TYPE_CDMA_HRPD
}LwrBandType;


typedef struct lwrCdma1xrttBndClsCfg
{
   LwrCdma2kBandClassType  bandClass;  /* CDMA2000 band */
   Bool                    cellReselectPriorPres; /* presence for below
                                                     CellReselectionPriority */
   U8                      cellReselectPrior; /* CellReselectionPriority */
   U8                      threshXHigh;
   U8                      threshXLow;
}LwrCdma1xrttBndClsCfg;

typedef struct lwrCdma1xrttNeighFreqCfg
{
   LwrCdma2kBandClassType     bandClass; /* band class info */
   U16         arfcn;       /* CDMA2000 frequency carrier */
   S8          freqPriority; /* frequency priority within Band */
}LwrCdma1xrttNeighFreqCfg;

typedef struct lwrCdma1xrttNeighCellCfg
{
#if 0
   U16 sId;
   U16 nId;
   U16 baseId;
   U16 marketId;
   U16 switchNum;
   U16 cellId;
   U16 sectorNum;
#endif

   U16                        cid;        /* cell Id */

   LwrCdma2kBandClassType     bandClass; /* Defines the CDMA2000 band in which
                                            the CDMA2000 frequency carrier can
                                             be found, specified in bandclass
                                             number */
   U32         arfcn;    /*Defines the CDMA2000 frequency carrier within a
                           CDMA2000 band, as specified by ARFCN-ValueCDMA2000
                           in SIB8 */
   U16         pnOffset;  /* Defines the PN offset that represent the "Physical
                             cell identity" in CDMA2000 system, as specified by
                             PhysCellIdCDMA2000 in SIB8 */
   U16         curRank;   /* To be multiplied with with a multiply factor 1000
                             to this value */
   U32         neighCellFlag; /* This parameter indicates SIB8 or MeasObject or
                                 both */
   LwrCdma2k1xMobParams mobParams; /* Mobility paramters */
}LwrCdma1xrttNeighCellCfg;


typedef struct lwrCDMAHrpdNeighCell
{
   U8  cellNum;  /**< cell number of having black list  */
   U16 nrIndex; /**< Index information of NRT Black List Cell */
   Bool status;
   U16 colorCode;
   U16 bsmId;
   U16 bscId;
   U16 dpssId;
   U16 btsId;
   U16 sectorId;
   LwrCdma2kBandClassType     bandClass; /**< Defines the CDMA2000 band in which the CDMA2000
                                          *   frequency carrier can be found, specified in
                                          *   bandclass number.
                                          */
   U32 arfcn; /**< Defines the CDMA2000 frequency carrier within a
               *   CDMA2000 band, as specified by
               *   ARFCN-ValueCDMA2000 in SIB8
               */
   U16 pnOffset; /**< Defines the PN offset that represents the
                  *   "Physical cell identity" in CDMA2000 system,
                  *   as specified by PhysCellIdCDMA2000 in SIB8 */
   U16  curRank; /* To be multiplied with with a multiply factor 1000 to 
                    this value */
   U8   loadBalanceRate; /**< This parameter indicates load balance rate when
                           eHPRD Pre-Registration.*/
   U32  neighCellFlag; /**< This parameter indicates SIB8 or MeasObject or Both
   */
   U8   cid[32];
}LwrCDMAHrpdNeighCell;



typedef struct lwrCDMA1xNeighCell
{
   U8  cellNum;  /**< cell number of having black list  */
   U8  nrIndex; /**< Index information of NRT Black List Cell */
   Bool status;
   U16                        sId;           /* system identification received in meas report */
   U16                        nId;           /* network idnetification received in meas report */
   U16                        baseId;        /* baseId received in meas report */
   U16                        cellId;        /* internal cell id */
   LwrCdma2kBandClassType     bandClass; /**< Defines the CDMA2000 band in which the CDMA2000
                                          *   frequency carrier can be found, specified in
                                          *   bandclass number.
                                          */
   U32 arfcn; /**< Defines the CDMA2000 frequency carrier within a
               *   CDMA2000 band, as specified by
               *   ARFCN-ValueCDMA2000 in SIB8
               */
   U16                     pnOffset;      /* Defines the PN offset that represent the "Physical
                                           * cell identity" in CDMA2000 system, as specified by
                                           * PhysCellIdCDMA2000 in SIB8 */
   U16                     curRank;       /* To be multiplied with with a multiply factor 1000 
                                        to this value */
   U32                     neighCellFlag; /* This parameter indicates SIB8 or MeasObject or 
                        *   Both */
   Bool blackListed;   
   LwrCdmaSectorId         secId;         /* sector id per neighbour */ 
   LwrCdma2k1xMobParams mobParams;  /* Mobility paramters */
   U8                      systemType;    /* system Type per pilot */
   U16                     channelNum;    /* channel num per pilot */
   U8                      pilotOneWay;   /* One way delay info for pilot */
}LwrCDMA1xNeighCell;


/** @brief This structure holds the neighbour cell information specific for UTRAN FDD
 *  - Bool   isHOAllowed; Whether to perform handover to UTRAN neigboring cell or not.
 *  - U16    arfcnUl;     The uplink absolute radio frequency channel
 *                           (ARFCN) of UTRAN neighboring cell to the eNB.
 *  - U16    arfcnDl;     The downlink absolute radio frequency channel number 
 *                        (ARFCN) of UTRAN neighboring cell to the eNB.
 *  - U16    psc;         Primary Scrambling Code of UTRA FDD Cell.
 *  - S16    pcpich;      pcpich Tx power of the UTRA FDD Cell.
 */
typedef struct lwrUtraFddCell{
   Bool     isHOAllowed;                             
   U16      arfcnUl;                                 
   U16      arfcnDl;                                 
   U16      psc;                                     
   S16      pcpich;
}LwrUtraFddCell;

/** @brief This structure holds the neighbour cell information specific for UTRAN TDD
 *    - U8     cellParamId;   The Cell Parameter Id of the UTRA TDD Cell.
 *    - U16    arfcn;  The absolute radio frequency channel of the UTRA TDD Cell.
 *    - S16    pccpch; pccpch Tx power of the UTRA TDD Cell.
 */
typedef struct lwrUtraTddCell{
   U8      cellParamId;
   U16     arfcn;
   S16     pccpch;   
}LwrUtraTddCell;

/** @brief This structure holds the neighbour cell information for UTRAN
  - U32    reCfgType;
  - U32    nrIndex;                  Database index of UTRAN neighboring cell
  - U16    rncId;                    RNC ID of UTRAN neighboring cell to the 
				     eNB
  - U16    cId;                      CID of UTRAN neighboring cell to the eNB
  - U16    lac;                      Location Area Code of UTRAN neighboring
				     cell to the eNB
  - U8     rac;                      Routing area code of UTRAN neighboring
				     cell to the eNB
  - U8     numPlmn;
  - LwrPlmnId    plmnId[LWR_MAX_PLMN_IDS]; This is the PLMN List information
                  (MNC) of ENB in the neighbor cell
                   belongs
  - U8     duplexMode;               The duplex type of UTRAN neighboring cell
				     to the eNB
  - Bool   isRemAllowed;             Whether to delete a certain neighboring 
                 cell or not
  - Bool   isVoipCapable;            Whether to support VoIp of UTRAN neighborin
				     cell
  - Bool   isPsHoCapable;            Whether to support PS HO of UTRAN neighborin
				     cell
  - U16    curRank;                  To be multiplied with with a multiply factor
				     1000 to the PLD value
  - U16    prevRank;                 To be multiplied with with a multiply factor
				     1000 to the PLD value
*/

typedef struct lwrUtraNeighCell{
   U32                          reCfgType;
   U8                           cellNum; /**< This is Cell number for changing */
   U8                           status;
   U32                          nrIndex;                                 
   U16                          rncId;                                   
   U16                          cId;                                     
   U16                          lac;                                     
   U8                           rac;                                     
   U8                           numPlmn;                                 
   LwrPlmnId                    plmnId[LWR_MAX_PLMN_IDS];                
   Bool                         isRimSupported; /* This variable is 
                                                 used to know whether neighbour
                                                 UTRA cell is RIM supported 
                                                 or not */                           
   Bool                         isRemAllowed;                            
   Bool                         isVoipCapable;                           
   Bool                         isPsHOCapable; /* Whether the UTRA neighbor is 
                                                      PS HO capable */
   U16                          curRank;                                 
   U16                          prevRank;                                
   U8                           duplexMode;     /* Use the WrUtraRatType ENUM values here */
   union{
      LwrUtraFddCell            utraFdd;    /* Stores cell params specific to UTRA FDD */
      LwrUtraTddCell            utraTdd;    /* Stores cell params specific to UTRA TDD */
   }t; 
}LwrUtraNeighCell;

/* This enum defines the purpose to use GERAN band indicators */
typedef enum lwrGeranBand
{
   WR_GERAN_GSM850,
   WR_GERAN_GSM900,
   WR_GERAN_DCS1800,
   WR_GERAN_PCS1900,
   WR_GERAN_MAX_BANDS
} LwrGeranBand;
#define WR_GERAN_BAND_INVLD 4 /* Default value for GERAN band */

/* This structure defines the GERAN neighbor cell params */
typedef struct lwrGERANNeighCell
{
   U32                 reCfgType; /* Re-Config type */
   U8                  status; /* Cell Status */
   U32                 nrIndex; /* Neighbor cell index */
   LwrPlmnId           plmnId; /* PLMN id */
   U16                 lac; /* Location Area Code */
   U8                  rac; /* Routing Area Code */
   U8                  bsic; /* Base Station Identification Code */
   U16                 ci; /* Cell Identifier */
   LwrGeranBand        bandIndicator; /* Geran Band */
   U16                 bcchArfcn; /* Geran Arfcn */
   U8                  nccPermittedMeas; /* Network Color Code Permitted for
   Meas */
   Bool                isDTMCapable; /* Flag to set for Cell DTM capable */
   Bool                ncoPres;
   U8                  ncoVal;
   S8                  offsetFreq;               
   Bool                rimSupportForGeranNgh;
}LwrGERANNeighCell;


typedef struct lwrNeighCellAddCfg
{
   LwrNeighType    type;   /*!< Type of neighbour i.e. EUTRA/CDMA2K */
   LwrNeighCfgType  cfgType;
   union 
   {
      LwrEUTRANeighCell           eutraCell;
      LwrCDMA1xNeighCell          cdma1xCell;
      LwrCDMAHrpdNeighCell        cdmaHrpdCell;
      LwrUtraNeighCell		       utranCell;    
      LwrGERANNeighCell           geranCell; /* Geran neighbor cell cfg */
   }t;
} LwrNeighCellAddCfg;

typedef struct lwrEutranCellDelCfg
{
   U16            nrIndex; /*!< Index of the neighbor cell */
   U16            earfcn; /*!< EARFCN of the cell */
   U16            phyCellId; /*!< PCI of the cell */
   U16           cid;  
   LwrPlmnId     plmnId;
}LwrEutranCellDelCfg;

typedef struct lwrCdmaCellDelCfg
{
   U16            nrIndex; /*!< Index of the neighbor cell */
   LwrCdma2kBandClassType     bandClass; /*!< BandClass of the cell */
   U16            arfcn; /*!< ARFCN of the cell */
   U16            pnOffset; /*!< PCI of the cell */
}LwrCdmaCellDelCfg;


/** @brief This structure holds the neighbour cell information for UTRAN
  - U16          nrIndex;         Database index of UTRAN neighboring cell
  - U16          arfcn;           ARFCN
  - U16          psc;             Primary scramble code 
*/
typedef struct lwrUtraCellDelCfg
{
   U16       nrIndex;  
   U16       arfcn;    
   U16        psc;       
   U16           cid;  
   LwrPlmnId     plmnId;
}LwrUtraCellDelCfg;

typedef struct lwrNeighCellDelCfg
{
   LwrNeighType   type;   /*!< Type of neighbour i.e. EUTRA/CDMA2K */
   union
   {
      LwrEutranCellDelCfg  eutranCell;
      LwrCdmaCellDelCfg    cdmaCell;
      LwrUtraCellDelCfg    utranCell;   
   }t;
}LwrNeighCellDelCfg;

typedef struct lwrNeighCellCfg
{
   U8                      cellId;   /*!< Cell Index */
   LwrActType              action;   /*!< Configure/ReConfigure/Reset */
   U16                     numCells; /*!< Number of Cells to be configured */                     
   union
   {
      LwrNeighCellAddCfg      neighAddCfg[MAX_NUM_NEIGH_CELLS];   /*!< Addition */
      LwrNeighCellAddCfg      neighModCfg[MAX_NUM_NEIGH_CELLS];   /*!< Modification */
      LwrNeighCellDelCfg      neighDelCfg[MAX_NUM_NEIGH_CELLS];   /*!< Deletion */
   }t;
} LwrNeighCellCfg;

typedef struct lwrNghCellInd
{
   LwrActType              action;
   union
   {
      LwrNeighCellAddCfg      neighAddCfg;
      LwrNeighCellAddCfg      neighModCfg;
      LwrNeighCellDelCfg      neighDelCfg;
   }t;
}LwrNghCellInd;

typedef enum lwrNeighCdmaFreqType
{
   LWR_NEIGH_CDMA_1x_RTT_TYPE,
   LWR_NEIGH_CDMA_HRPD_TYPE
} LwrNeighCdmaFreq;

typedef enum lwrNeighFreqType
{
   LWR_NEIGH_EUTRA_FREQ_TYPE,
   LWR_NEIGH_CDMA_FREQ_TYPE
} LwrNeighFreqType;

typedef enum lwrQOffsetRange
{
   LWR_Q_OFFSET_RANGE_MINUS_24,
   LWR_Q_OFFSET_RANGE_MINUS_22,
   LWR_Q_OFFSET_RANGE_MINUS_20,
   LWR_Q_OFFSET_RANGE_MINUS_18,
   LWR_Q_OFFSET_RANGE_MINUS_16,
   LWR_Q_OFFSET_RANGE_MINUS_14,
   LWR_Q_OFFSET_RANGE_MINUS_12,
   LWR_Q_OFFSET_RANGE_MINUS_10,
   LWR_Q_OFFSET_RANGE_MINUS_8,
   LWR_Q_OFFSET_RANGE_MINUS_6,
   LWR_Q_OFFSET_RANGE_MINUS_5,
   LWR_Q_OFFSET_RANGE_MINUS_4,
   LWR_Q_OFFSET_RANGE_MINUS_3,
   LWR_Q_OFFSET_RANGE_MINUS_2,
   LWR_Q_OFFSET_RANGE_MINUS_1,
   LWR_Q_OFFSET_RANGE_0,
   LWR_Q_OFFSET_RANGE_1,
   LWR_Q_OFFSET_RANGE_2,
   LWR_Q_OFFSET_RANGE_3,
   LWR_Q_OFFSET_RANGE_4,
   LWR_Q_OFFSET_RANGE_5,
   LWR_Q_OFFSET_RANGE_6,
   LWR_Q_OFFSET_RANGE_8,
   LWR_Q_OFFSET_RANGE_10,
   LWR_Q_OFFSET_RANGE_12,
   LWR_Q_OFFSET_RANGE_14,
   LWR_Q_OFFSET_RANGE_16,
   LWR_Q_OFFSET_RANGE_18,
   LWR_Q_OFFSET_RANGE_20,
   LWR_Q_OFFSET_RANGE_22,
   LWR_Q_OFFSET_RANGE_24
} LwrQOffsetRange;

typedef struct lwrSSearchRel9
{
   Bool        pres;
   U8          sSearchPRel9;
   U8          sSearchQRel9;
} LwrSSearchRel9;

typedef struct lwrThreshXQRel9
{
   U8       threshXHighQR9;
   U8       threshXLowQR9;
} LwrThreshXQRel9;


typedef struct lwrNeighEutraFreq
{
   U32                  reCfgType;
   U8                   freqIndex;
   U8                   status;
   U16                  earfcnUl;
   U16                  earfcnDl;
   U8                   priority;
   S8                   qRxLevMin;
   Bool                 pMaxUsage;
   S8                   pMax;
   U8                   tReselection;
   Bool                 sfUsage;
   U8                   tReselectionSFMed;
   U8                   tReselectionSFHigh;
   TknU8                sIntraSearch;        
   TknU8                sNonIntraSearch;        
   U8                   servThresholdLow;
   U8                   threshXHigh;
   U8                   threshXLow;
   TknU8                measBandwidth;          
   Bool                 presAntennaPort1;
   U8                   neighCellCfg;  /*!< Bit string (2) */
   LwrQOffsetRange      qOffsetFreq;   /*!< For SIB 5 */
   LwrQOffsetRange      offsetFreq;    /*!<  FOR MeasEUTRA */
   LwrSSearchRel9       sIntraSearchRel9;
   LwrSSearchRel9       sInterSearchRel9;
   TknS8                qQualMinRel9;    /*!< {-34,..., -3} */
   TknU8                reselecThreshQRel9;
   LwrThreshXQRel9      threshXQRel9;
} LwrNeighEutraFreq;

/** @brief This structure contains the EARFCN to be deleted
 *  */
typedef struct lwrNeighEutraDelInfo
{
   U8          freqIndex;
   U16         earfcn;
} LwrNeighEutraDelInfo;

/** @brief This structure contains the ARFCN to be deleted
 *  */
typedef struct lwrNeighCdmaDelInfo
{
   U8          freqIndex;     /*!<ID for Configured Freq>*/
   U16         arfcn;         /*!< ARFCN of NeighCell > */
   LwrCdma2kBandClassType     bandClass;
} LwrNeighCdmaDelInfo;

/** @brief This structure contains the ARFCN to be deleted
* */
typedef struct lwrNeighUtraDelInfo
{
   U8          freqIndex;    /*!<ID for Configured Freq>*/
   U16          arfcn;     /*!< ARFCN of NeighCell UTRA > */
}LwrNeighUtraDelInfo;

        
/** @brief This structure holds the freq Info of cdma1xRTT carrier
 *  */
typedef struct lwrNeighCdma1xFreq
{
   U8       reCfgType;             /*!<BitMaskDynamicUpdates >*/
   U8       freqIndex;             /*!<ID for Configured Freq>*/
   Bool     status;                /*!<configured/Not Configured>*/
   LwrCdma2kBandClassType     bandClass; /*!<BandCalss> */
   U16      arfcn;                 /*!< ARFCN of NeighCell > */
   U8       searchRate;            /*!<used for Measurements > */
   S8       offsetFreq;            /*!<Used for Measurements > */
   U8       freqPriority;          /*!<Priority of Freq in particular Band >*/
} LwrNeighCdma1xFreq;

/** @brief This structure holds the freq Info of CDMA HRPD carrier
 *  */
typedef struct lwrNeighCdmaHRPDFreq
{
   U8       reCfgType;
   U8       cellNum;
   U8       freqIndex;
   Bool     status;
   LwrCdma2kBandClassType     bandClass;
   U16      arfcn;
   U8       searchRate;
   S8       offsetFreq;
} LwrNeighCdmaHRPDFreq;

/** @brief This structure holds the neighbour frequency information for UTRAN
  - U8           pres   Presence Flag
  - U8           thresXHighQR9 Threshold High Rel9
  - U8           thresXLowQR9  Threshold Low Rel9
  */
typedef struct  lwrNrUtraFreqThrQR9{
   U8           pres;
   U8           thresXHighQR9;
   U8           thresXLowQR9;
}LwrNrUtraFreqThrQR9;

/** @brief: This structure identifies the type UTRA TDD Mode
 * */
typedef enum lwrUtraTddMode{
   LWR_UTRA_TDD128,
   LWR_UTRA_TDD384,
   LWR_UTRA_TDD768
}LwrUtraTddMode;

/** @brief This structure holds the neighbour frequency information specific for UTRAN FDD
      - U8      pQualMin;     The minimum quality level required in UTRA 
				                  FDD cells
      - U16     arfcnUl;      The uplink absolute radio frequency channel
                              (ARFCN) of UTRAN FDD neighboring cell to the eNB
      - U16     arfcnDl;      The downlink absolute radio frequency channel 
                              number (ARFCN) of UTRAN FDD neighboring 
                              cell to the eNB
 * */
typedef struct lwrUtraFddFreq{
   S8                    pQualMin;                 
   U16                   arfcnUl;                  
   U16                   arfcnDl;                  
   LwrNrUtraFreqThrQR9   thresR9;
}LwrUtraFddFreq;

/** @brief This structure holds the neighbour frequency information specific for UTRAN TDD
 *    - LwrUtraTddMode  tddMode;  Identify the UTRA TDD Mode for this frequency.
 *    - U16             arfcn;    The absolute radio frequency channel
 *                                (ARFCN) of UTRAN TDD neighboring cell to the eNB
 *    -U8               bandIndicator;    Identifies the band for this UTRA TDD
 *    frequency as some of the frequencies can belong to multiple bands. This
 *    parameter has been added. 
 * */
typedef struct lwrUtraTddFreq{
   U8                    bandIndicator;
   U16                   arfcn;                  
   LwrUtraTddMode        tddMode;
}LwrUtraTddFreq;

/* @brief: ENUM to define FDD and TDD*/
typedef enum lwrUtraDuplexMode{
   WR_LWR_UTRA_FDD_MODE = 0,
   WR_LWR_UTRA_TDD_MODE
}LwrUtraDuplexMode;

/** @brief This structure holds the neighbour frequency information for UTRAN
  - U32     reCfgType;
  - U32     freqIndex;                Database index of UTRAN neighboring cell
  - U8      priority;                 Priority information on the UTRA FA
  - U8      threshXhigh;              The threshold used in the UE to reselect
                  a frequency whose priority is higher than 
                  the current camped frequency
  - U8      threshXlow;                The threshold used in the UE to reselct a 
                  low-priority frequency from high-priority
                  frequency
  - U8      qRxLevMin;           The minimum RX level required in a cell in
                  dbM units
  - U8      pMaxUtra;                 The maximum RF output power in the UE
  - U8      offsetFreq;               The offset applied to UTRA carrier frequency
*/
typedef struct  lwrNeighUtraFreq{
   U32                   reCfgType;
   U8                    freqIndex;                
   U8                    status;
   TknU8                 priority;                 
   U8                    threshXhigh;              
   U8                    threshXlow;               
   S8                    qRxLevMin;   
   S8                    pMaxUtra;                 
   S8                    offsetFreq;               
   U8                    duplexMode; /* Use the WrUtraRatType ENUM values here */
   union{
      LwrUtraFddFreq     utraFdd;    /* Stores frequency params specific to UTRA FDD */
      LwrUtraTddFreq     utraTdd;    /* Stores frequency params specific to UTRA TDD */
   }t; 
}LwrNeighUtraFreq;


/* This structure holds the neighbour frequency information for GERAN */
typedef struct LwrNeighGERANFreq
{
   U32             reCfgType;
   U8              freqIndex;
   U8              status;
   LwrGeranBand    bandIndicator;
   U16             bcchArfcn;
   U8              qRxLevMin;
   U8              cellReselePri;
   U8              threshXHigh;
   U8              threshXLow;   
   U8              ncc_Permitted;   
   TknU8           pMaxGeran;     
} LwrNeighGERANFreq;


typedef struct lwrNeighFreqDelCfg
{
   LwrNeighType   type;   /*!< Frequency type */
   union
   {
      LwrNeighEutraDelInfo          eutranFreq;
      LwrNeighCdmaDelInfo           cdmaFreq;
      LwrNeighUtraDelInfo           utranFreq;   
   }t;
} LwrNeighFreqDelCfg;


typedef struct lwrNeighFreqAddCfg
{
   LwrNeighType   type;   /*!< Frequency type */
   union
   {
      LwrNeighEutraFreq          eutranFreq;
      LwrNeighCdma1xFreq         cdma1xFreq;
      LwrNeighCdmaHRPDFreq       cdmaHrpdFreq;
      LwrNeighUtraFreq           utranFreq;
      LwrNeighGERANFreq          geranFreq; /* Geran Freq Cfg */
   }t;
} LwrNeighFreqAddModCfg;

typedef struct lwrNeighFreqCfg
{
   U8              cellId;   /*!< Cell index */
   LwrActType      action;   /*!< ADD/MOD/DEL */
   U8              numFreq;
   union
   {
      LwrNeighFreqAddModCfg   addNeighFreq[MAX_NUM_FREQ_CFG];
      LwrNeighFreqAddModCfg   modNeighFreq[MAX_NUM_FREQ_CFG];
      LwrNeighFreqDelCfg      delNeighFreq[MAX_NUM_FREQ_CFG];
   }t;
} LwrNeighFreqCfg;

typedef struct lwrNeighCdmaBandClassCfg
{
   U8          reCfgType;        /*!<BitMaskDynamicUpdates >*/
   U8          bandClassIndex;   /*!<ID for Configured Band > */
   U8          status;           /*!<configured/Not Configured>*/
   LwrCdma2kBandClassType     bandClass; /* ! <BandClass > */
   U8          cellReselectPrior; /*!<Used on Cell Relection >*/
   U8          threshXHigh;       /*!<Upper threshold for CellRel>*/
   U8          threshXLow;        /*!<Upper threshold for CellRel>*/
} LwrNeighCdmaBandClassCfg;

typedef struct lwrNeighBandClassDel
{
   LwrCdmaType                cdmaType;
   U8                         badnClassIndex;
   LwrCdma2kBandClassType     bandClass;
} LwrNeighBandClassDel;

typedef struct lwrNeighBandClassCfg
{
   LwrBandType       bandType;
   union
   {
      LwrNeighCdmaBandClassCfg bandClass1xRtt;
      LwrNeighCdmaBandClassCfg bandClassHRPD;
   }t;
} LwrNeighBandClassCfg;

typedef struct lwrNeighBandClass
{
   U8              cellId;   /*!< Cell index */
   LwrActType      action;   /*!< ADD/MOD/DEL */
   U8              numBandClass;
   union
   {
      LwrNeighBandClassCfg      bcAddCfg[MAX_NUM_BAND_CFG];
   }t;
} LwrNeighBandClass;


typedef struct lwrProtoCfg
{
   LwrProtoCfgType  type;   /*!< Type of configuration */
   union
   {
      LwrCellCfg         cellCfg;    /*!< Cell Configuration */
      LwrMmeCfg          mmeCfg;     /*!< MME configuration */
      LwrNeighCfg        neighCfg;   /*!< Neighbour configuration */
      LwrNeighCellCfg    neighCellCfg;   /*!< Neighbour cell configuration */
      LwrENbCfg          eNbCfg;     /*!< ENodeB configuration */
      LwrNeighFreqCfg    neighFreqCfg;   /*!< Neighbouring frequency */
      LwrNeighBandClass  bandClassCfg;
   }t;
} LwrProtoCfg;

typedef struct lwrLoadIndInfo
{
  U8  cpuLoad;
}LwrLoadIndInfo;

typedef struct lwrCellFddCfg
{
   U32 ulEarfcn;
   U32 dlEarfcn;
   U8 ulTxBndWdth;
   U8 dlTxBndWdth;
} LwrCellFddCfg;

typedef struct lwrCellTddCfg
{
   U32 earfcn;
   U8 txBndWdth;
   U8 sfAssgnmnt;
   U8 splSfPatrn;
   U8 dlCycPrefix;
   U8 ulCycPrefix;
} LwrCellTddCfg;

typedef struct lwrGenUeCfg
{
   /* Security related info */
  U8 intgAlgo;
  U8 ciphAlgo;
} LwrGenUeCfg;

typedef struct lwrSmCellCfg
{
   /* Cell cfg related info */
   U16          cellId;
   /* Modulation Type */ 
   U8  modType;
   /* duplexing mode : TDD/FDD/HD_FDD */
   U16 duplexMode;
   U8  maxUeSupp; /* maximum number of UEs supported by each eNodeB */
   /* PLMN Id */
   TknU8 mcc[3];
   TknU8 mnc[3]; /* Token since size could be of 2 to 3 */
   /* Tracking Area Code */
   U16 trackAreaCode;
   /* Frequency Band Indicator */
   U8 freqBandInd;
   LwrGenUeCfg genUeCfg;
#ifdef LTE_HO_SUPPORT
   U16 physCellId;   /* Physical Cell ID */
   U16 rrmRntiStart;
   U16 maxRrmRntis;
   U16 rrmPrmblStart;
   U16 maxRrmPrmbls;
   U16 macRntiStart;
   U16 maxMacRntis;
   U8  numOfMacPrm;
   U32 rsrpTrshldRange;            /* Serving Cell threshold 
                                                   RSRP value */
   LwrCellFddCfg    fddCfg;                     /* FDD config INFO */
   LwrCellTddCfg    tddCfg;                     /* TDD config info */
   U8              noOfBrdCstPlmn;             /* numbre of broadcasr plmns */
   U8              plmnId[LWR_MAX_PLMN_IDS][3];

#endif
} LwrSmCellCfg;

typedef struct lwrGenCfg
{
   Pst      lmPst;  
#ifdef WR_RELAY
   LwrSmCellCfg   cellCfg;
#endif
   CmTptAddr dataAppAddr;
   U8          eNBType;
   U16         noOfNghInfo;
   LwrNghCellCfg   **nghInfo;
   U8          noOfguGrp; /* MAX of 16 group items are possible */
   LwrGuGrpIdCfg   **guGrpId;
   U32         ueHoAttachTimerVal;/* UE Ho Attach Timer Value */
   U32         inactvTmrVal;/* UE Inactivity timer value */
   U16         expiryCnt; /* UE Inactivity timer expiry count */
   U32         endMrkTmrVal;/* End Marker timer value */
#ifdef ENB_OVRLOAD
   LwrEnbOvldParams eNBOvldParams;
#endif
   Ticks       timeRes;       /* Time resolution */
   U32         ulSrsPeriod; /* UL SRS Periodicity in milli second */
   Bool        enblSIAndPagngLog; /* To enable SI and paging wireshark logging*/ 
} LwrGenCfg;


typedef struct lwrLSapCfg
{
   SuId        suId;      
   SpId        spId;   
   Ent         srcEnt;
   Inst        srcInst;
   ProcId      dstProcId; 
   Ent         dstEnt;    
   Inst        dstInst;   
   Priority    dstPrior;  
   Route       dstRoute;  
   Selector    dstSel;    
   Mem         mem;
   U8          maxBndRetry;
   TmrCfg      bndTmr;    
   TmrCfg      connTmr;   
} LwrLSapCfg;

typedef struct lwrCfg
{
   union
   {
      LwrGenCfg    genCfg;   /*!< General configuration */
      LwrLSapCfg   lSap;   /*!< General configuration */
   }t;
} LwrCfg;

typedef struct lwrDbgCntrl 
{
   U32       dbgMsk;                       /*!< Debug Mask parameter. 
                                            This field can be set to a 
                                            combination of the below allowed 
                                            values by using OR "|" operator.
                                            For example, if the 
                                            debug and info debugging prints  
                                            to be enabled/disabled, the dbgMask
                                            is set to "LWR_DBGMASK_INFO|
                                            LWR_DBGMASK_DBG".
                                              <b> Allowed values </b>
                                                - LWR_DBGMASK_INFO
                                                   To print specific behaviour
                                                   /logic flow.
                                                - LWR_DBGMASK_DBG
                                                   To print extensive logs.
                                                - LWR_DBGMASK_PARAM
                                                   To print function
                                                   parameters values.
                                                - LWR_DBGMASK_ERROR
                                                   To print in error legs.
                                                - LWR_DBGMASK_FATAL
                                                   To print any system effecting
                                                   errors such as resouce
                                                   unavailability.
                                            */

#ifdef WR_SM_LOG_TO_FILE
   Txt       filePath[LWR_MAX_FILE_PATH];  /*!< Path to store log files. */  
   U32       nmbDbgLines;                  /*!< Number of lines per Debug file. */
#endif /* WR_SM_LOG_TO_FILE */
} LwrDbgCntrl;

typedef struct lwrCntrl
{
   Action    action;    
   Action    subAction; 
   U8        numConn;
   U32       peerId[LWR_MAX_CONN];
   U32       idLst[LWR_MAX_CONN];
   
   union
   {
      SpId       sapId; 
#ifdef WR_RELAY
      U16        cellId;
#endif
#ifdef DEBUGP
      U32        dbgMask;
      LwrDbgCntrl dbg;/*!< The layer manager can selectively enable
                                          or disable various levels of debug 
                                          printing. */
#endif
      U8         oper;
      U32        logMask; /* logging Mask control */
   } u;

} LwrCntrl;

typedef struct lwrUstaDgn
{
   U8    type;
   union
   {
      SpId     sapId;
      struct {
         U16     udId;
         U16     cellId;
      }s;
      struct{
         U32 alarmId;
         U8  severity;     
         U8  causeType;
         U8  causeVal;
      }alarm;
   }u;
} LwrUstaDgn;

typedef struct lwrUsta
{
   CmAlarm     alarm;
   LwrUstaDgn   dgn;   /*!< Diagnostic */
} LwrUsta;

typedef struct wrHoEutran
{
    U32 wrHODesirableForRadioReasons;
    U32 wrTimeCriticalHO;
    U32 wrResourceOptimizationHO;
    U32 wrReduceLoadInServingCell;
    U32 wrRadioResourcesNotAvailable;
    U32 wrControlProcessingOverload;
    U32 wrNotEnoughUPProcesingResources;
    U32 wrOAMIntervention;
    U32 wrSum;
}WrHoEutran;

typedef struct wrHoUtran
{
    U32 wrHODesirableForRadioReasons;
    U32 wrTimeCriticalHO;
    U32 wrResourceOptimizationHO;
    U32 wrReduceLoadInServingCell;
    U32 wrRadioResourcesNotAvailable;
    U32 wrControlProcessingOverload;
    U32 wrNotEnoughUPProcesingResources;
    U32 wrOAMIntervention;
    U32 wrSum;
}WrHoUtran;


typedef struct wrIntraRatHoSts
{
   WrHoEutran wrToAttempt;
   WrHoEutran wrToSuccess;
   WrHoEutran wrToFail;
   WrHoEutran wrFrAttempt;
   WrHoEutran wrFrSuccess;
   WrHoEutran wrFrFail;
}WrIntraRatHoSts;

typedef struct wrInterRatHoSts
{
   WrHoUtran wrToAttempt;
    WrHoUtran wrToSuccess;
    WrHoUtran wrToFail;
    WrHoUtran wrFrAttempt;
    WrHoUtran wrFrSuccess;
    WrHoUtran wrFrFail;
}WrInterRatHoSts;


typedef struct wrHoSts
{
   WrIntraRatHoSts wrIntraRatHoStat;
   WrInterRatHoSts wrInterRatHoStat;
}WrHoSts;

typedef struct wrConnEstab
{
    U32 emergency;
    U32 HighPriorityAccess;
    U32 MtAccess;
    U32 MoSignalling;
    U32 MoData;
    U32 DelayTolerantAccessV1020;
    U32 Sum;
}WrConnEstab;


typedef struct wrRrcConCause
{
    U32 emergency;
    U32 highPriorityAccess;
    U32 mtAccess;
    U32 moSignalling;
    U32 moData;
    U32 delayTolerantAccessV1020;
    U32 sum;
}WrRrcConCause;


typedef struct wrRrcSts
{
    WrRrcConCause attConnEstab;
    WrRrcConCause succConnEstab;
    WrRrcConCause failConnEstab;
    WrRrcConCause failConnDegradedRadio;
}WrRrcSts;

typedef struct wrRrcReEstabCause
{
   U32 recfgFail;
   U32 hoFail;
   U32 OthrFail;
   U32 sum;
}WrRrcReEstabCause;

typedef struct wrRrcReEstabSts
{
    WrRrcReEstabCause attConnEstab;
    WrRrcReEstabCause succConnEstab;
    WrRrcReEstabCause failConnEstab;
    WrRrcReEstabCause failConnDegradedRadio;
}WrRrcReEstabSts;


typedef struct wrErabStat
{
    U32 qci1;
    U32 qci2;
    U32 qci3;
    U32 qci4;
    U32 qci5;
    U32 qci6;
    U32 qci7;
    U32 qci8;
    U32 qci9;
    U32 qciSum;
}WrErabStat;

typedef struct wrErabFailStat
{
    U32   wrRadioResourcesNotAvailable;
    U32   wrFailureInRadioInterfaceProcedure;
    U32   wrInvalidQoSCombinations;
    U32   wrMultipleERABIDInstances;
    U32   wrNotSupportedQCIValue;
    U32   wrS1IntraSystemHandoverTriggered;
    U32   wrS1InterSystemHandoverTriggered;
    U32   wrX2HandoverTriggered;
    U32   wrSum;
}WrErabFailStat;


typedef struct wrErabFailStatHenb
{
   U32 wrRadioResourcesNotAvailable;
    U32 wrReduceLoadInServingCell;
    U32 wrFailureInTheRadioInterfaceProcedure;
    U32 wrReleaseDueToEUTRANGeneratedReaso;
    U32 wrUserInactivity;
    U32 wrRadioConnectionWithUELost;
    U32 wrHardwareFailure;
    U32 wrOAMIntervention;
}WrErabFailStatHenb;



typedef struct wrErabSts
{
    WrErabStat estabInitAttNbr;                /*Statistics to track per QCI Default bearers attempted*/
    WrErabStat estabInitSuccNbr;               /*Statistics to track per QCI Default bearers Sucessfully established*/
    WrErabFailStat EstabInitFailNbr;           /*Statistics to track per QCI Default bearers failed to  established*/

    WrErabStat EstabAttNbr;                    /*Statistics to track per QCI dedicated bearers attempted*/
    WrErabStat EstabSuccNbr;                   /*Statistics to track per QCI dedicated bearers Sucessfully established*/
    WrErabFailStat EstabFailNbr;               /*Statistics to track per QCI dedicated bearers failed to  established*/

    WrErabStat RelEnbNbr;                      /*Statistics to track per QCI bearers requested to be released by Enodeb */
    WrErabFailStatHenb RelEnbNbr_Henb;         /*Statistics to track per FailureReason bearers requested to be released by Enodeb */


    WrErabStat RelEnbMmeAtt;                   /*Statistics to track per QCI bearers requested to be released by Enodeb */
    WrErabStat RelEnbMmeSucc;                   /*Statistics to track per QCI bearers requested to be released by Enodeb */
    WrErabFailStat RelEnbMmeFail;                   /*Statistics to track per QCI bearers requested to be released by Enodeb */

    WrErabStat ModAttNbr;                      /*Statistics to track per QCI Bearers Modification attempted*/
    WrErabStat ModSuccNbr;                     /*Statistics to track per QCI Bearers Modification sucessfully Done*/
    WrErabFailStat ModFailNbr;                 /*Statistics to track per QCI Bearers Modification failed*/
}WrErabSts;

typedef struct wrSigSts
{
   U32 wrSigAttempt;
   U32 wrSigSuccess;
}WrSigSts;


typedef struct wrUeCtxtRelSts
{
   U32 wrUeCtxtRelReq;
   U32 wrUeCtxtRelSuccess;
}WrUeCtxtRelSts;



typedef struct wrCallSts
{
    U32 wrActiveCalls;
    U32 wrTotalCalls;
}WrCallSts;



typedef enum wrMmeStatus
{
    WR_MME_IDLE = 0,
   WR_MME_ACTIVE
}WrMmeStatus;


typedef struct wrMmeSta
{  U32             numMme;
   U32             mmeIdx[LWR_MAX_MMES]; 
   WrMmeStatus     mmeUsageState[LWR_MAX_MMES];
}WrMmeSta;


typedef enum wrCellStatus
{
  WR_CELL_IDLE = 0,
  WR_CELL_ACTIVE,
  WR_CELL_BUSY
}WrCellStatus;


typedef struct wrCellSta
{
    U16     cellNum;
    WrCellStatus    cellUsageState[2];  /*Replace 2 by WR_MAX_CELLS*/
}WrCellSta;


typedef struct wrS1USta
{
    U32 s1UStat;
}WrS1USta;

typedef struct wrCellSetupSta
{
   U16        cellNum;
   U16        result; /*!< OK=0 FAIL=1 */
   U8         cause[40];
}WrCellSetupSta;

typedef struct wrUeActvCount
{
   U16 cellNum;
   U16 activeUeCnt;
   
} WrUeActvCount;


/**
 * @brief
   This structure holds Wr's solicited status information. */
typedef struct lwrSta
{
   DateTime dt;             /*!< Date and time */
   U8       sapInst;        /*!< SAP instance */
   union
   {
      WrMmeSta wrMmeSta;
      WrCellSta wrCellSt;
      WrS1USta  wrS1Usta;
      WrCellSetupSta wrCellSetupSta;
      WrUeActvCount   wrUeActvCount;
   }s;
}LwrSta;


/**
 * @brief
   This structure holds Wr's statistical information. */
typedef struct lwrSts
{
   DateTime dt;          /*!< Date and Time. */
   U8       sapInst;     /*!< SAP instance. */
   Action   action;      /*!< Action on Trafffic load related statistic values.
                              ARST: To reset the statistic values. */
   union
   {
      WrRrcSts  wrRrcStat;
      WrErabSts wrErabStat;
      WrCallSts wrCallStat;
      U32       wrPagStat;
      WrHoSts   wrHoStat;
      WrUeCtxtRelSts wrUeCtxtRelSts;
   }s;
}LwrSts;

typedef struct lwrNbrAddReq
{
   U32       enbId;
   CmTptAddr nbIpAddress;
} LwrNbrAddReq;   

typedef struct lwrNbrAddCfm
{
   U32       enbId;
   U32       peerId;
   CmTptAddr ipAddr;
} LwrNbrAddCfm;   

typedef struct lwrMngmt
{
   Header            hdr;    /*!< Header for struct */
   CmStatus          cfm;    /*!< Confirmation of the result of operation */
   union
   {
      LwrCfg          cfg;     /*!< Configuration for WR in terms TAPA */
      LwrCntrl        cntrl;   /*!< Control for WR in terms of TAPA */
      LwrUsta         usta;    /*!< Alarms etc */
      LwrSts          sts;     /*!< Statistics */
      LwrSta          sta;     /*!< Solicited Status */
      LwrProtoCfg     protoCfg;/*!< Protocol specific configuration */
      LwrNbrAddReq    nbrAddReq;
      LwrNbrAddCfm    nbrAddCfm;
   }t;
} LwrMngmt;

typedef struct lwrDynMngmt
{
   Header            hdr;    /*!< Header for struct */
   CmStatus          cfm;    /*!< Confirmation of the result of operation */
   LwrDynCfgEvntType cfgType;
   U8                cfgPriority;   
   Void              *dynCfg;
} LwrDynMngmt;



typedef S16 (*LwrCfgReq)   (
Pst      *pst, 
LwrMngmt *cfg
);

typedef S16 (*LwrDynCfgReq)   (
Pst    *pst, 
LwrDynMngmt *dynCfg
); 

typedef S16 (*LwrCfgCfm)   (
Pst      *pst, 
LwrMngmt *cfg
);

typedef S16 (*LwrDynCfgCfm)   (
Pst      *pst,
LwrDynMngmt *cfm
);

typedef S16 (*LwrCntrlReq) (
Pst      *pst, 
LwrMngmt *cntrl
);

typedef S16 (*LwrCntrlCfm) (
Pst      *pst, 
LwrMngmt *cntrl
);

typedef S16 (*LwrUpDateNhgCellCfg) (
Pst      *pst,
LwrMngmt *sta
);


typedef S16 (*LwrStaInd) (
Pst      *pst,
LwrMngmt *sta
);

typedef S16 (*LwrLoadInd) (
Pst *pst,
LwrLoadIndInfo *loadInd
);

typedef S16 (*LwrDynCfgReqSend) (
void *cfg,
U32  cfgType,
U8  priority
);

typedef S16 (*LwrStartStopStackReq) (
U8 isPerdREMReq,
U8 isStackStart,
U8 isImmediate
);

typedef S16 (*LwrStaticCfgReqSend) (
Void
);

typedef S16 (*LwrNghCellIndToOam)   
(
Pst    *pst, 
LwrNghCellInd *dynCfg
); 


EXTERN S16 WrMiLwrCfgReq      (Pst *pst, LwrMngmt *cfg);
EXTERN S16 cmPkLwrCfgReq      (Pst *pst, LwrMngmt *cfg);

EXTERN S16 WrMiLwrCfgCfm      (Pst *pst, LwrMngmt *cfg);
EXTERN S16 cmPkLwrCfgCfm      (Pst *pst, LwrMngmt *cfg);

EXTERN S16 WrMiLwrCntrlReq    (Pst *pst, LwrMngmt *cntrl);
EXTERN S16 cmPkLwrCntrlReq    (Pst *pst, LwrMngmt *cntrl);

EXTERN S16 WrMiLwrCntrlCfm    (Pst *pst, LwrMngmt *cntrl);
EXTERN S16 cmPkLwrCntrlCfm    (Pst *pst, LwrMngmt *cntrl);

EXTERN S16 WrMiLwrStaInd      (Pst *pst, LwrMngmt *sta);
EXTERN S16 cmPkLwrStaInd      (Pst *pst, LwrMngmt *sta);
EXTERN S16 WrMiLwrLoadInd     (Pst *pst, LwrLoadIndInfo *loadInd);
EXTERN S16 WrMiLwrUpDateNhgCellCfg (Pst *pst, LwrMngmt *nghCellCfg);

EXTERN S16 WrMiLwrDynCfgReq   (Pst *pst, LwrDynMngmt *dynCfg);
EXTERN S16 cmPkLwrDynCfgReq   (Pst *pst, LwrDynMngmt *dynCfg);

EXTERN S16 cmPkLwrUpDateNhgCellCfgReq (Pst *pst,LwrMngmt *cfg);
EXTERN S16 cmUnpkLwrUpDateNhgCellCfgReq(LwrUpDateNhgCellCfg func,Pst *pst,Buffer *mBuf);
EXTERN S16 cmUnpkLwrNghCellInd(LwrNghCellIndToOam func,Pst *pst,Buffer *mBuf);

/*---- Layer manager interface primitives ----*/
#ifdef EU_DAT_APP
EXTERN S16 WrMiLwrCfgReq      (Pst *pst, LwrMngmt *cfg);
EXTERN S16 WrMiLwrCfgCfm      (Pst *pst, LwrMngmt *cfg);
EXTERN S16 WrMiLwrCntrlReq    (Pst *pst, LwrMngmt *cntrl);
EXTERN S16 WrMiLwrCntrlCfm    (Pst *pst, LwrMngmt *cntrl);
EXTERN S16 WrMiLwrStaInd      (Pst *pst, LwrMngmt *sta);
EXTERN S16 WrMiLwrLoadInd     (Pst *pst, LwrLoadIndInfo *loadInd);
#endif

EXTERN S16 smDynCfgReq(Void *cfg, U32 cfgType, U8 priority);


EXTERN S16 cmPkLwrLoadInd( Pst *pst, LwrLoadIndInfo *loadIndInfo); 
EXTERN S16 cmPkLwrAddNeighCfm( Pst *pst, LwrMngmt *cfg);
EXTERN S16 cmPkLwrCfgCfm      (Pst *pst, LwrMngmt *cfg);
EXTERN S16 cmPkLwrCntrlCfm    (Pst *pst, LwrMngmt *cntrl);
EXTERN S16 cmPkLwrAddNeighCfgReq (Pst *pst,LwrMngmt *cfm);
EXTERN S16 cmPkLwrDynCfgCfm   (Pst *pst, LwrDynMngmt *cfgCfm);

EXTERN S16 cmUnpkLwrCfgCfm    (LwrCfgCfm func, Pst *pst, Buffer *mBuf);
EXTERN S16 cmUnpkLwrCntrlCfm  (LwrCntrlCfm func, Pst *pst, Buffer *mBuf);
EXTERN S16 cmUnpkLwrStaInd    (LwrStaInd func, Pst *pst, Buffer *mBuf);
EXTERN S16 cmUnpkLwrDynCfgCfm (LwrDynCfgCfm func, Pst *pst, Buffer *mBuf);

#ifdef SM
EXTERN S16 SmMiLwrCfgReq      (Pst *pst, LwrMngmt *cfg);
EXTERN S16 SmMiLwrCfgCfm      (Pst *pst, LwrMngmt *cfg);
EXTERN S16 SmMiLwrCntrlReq    (Pst *pst, LwrMngmt *cntrl);
EXTERN S16 SmMiLwrCntrlCfm    (Pst *pst, LwrMngmt *cntrl);
EXTERN S16 SmMiLwrStaInd      (Pst *pst, LwrMngmt *sta);
EXTERN S16 SmMiLwrAddNeighCfgReq (Pst *pst,LwrMngmt *cntrl);
EXTERN S16 SmMiLwrLoadInd (Pst *pst, LwrLoadIndInfo *loadInd);
EXTERN S16 WrMiLwrAddNeighCfm (Pst *pst,LwrMngmt *cfg);

EXTERN S16 SmMiLwrDynCfgCfm   (Pst *pst, LwrDynMngmt *dynCfgCfm);
EXTERN S16 SmMiLwrDynCfgReq   (Pst *spst, LwrDynMngmt *cfg);
EXTERN S16 cmPkLwrNghCellInd(Pst   *pst,LwrNghCellInd *nghCellInd);
EXTERN S16 SmMiLwrNghInd(Pst    *spst,LwrNghCellInd *cfg);
#endif /* SM */


#ifdef SM
/*-- stack manager initialization function ---*/
EXTERN S16 smWrActvInit   (Ent ent, Inst inst,
                                Region region, Reason reason);
/*---- stack manager activation function -----*/
EXTERN S16 smWrActvTsk    (Pst *pst, Buffer *mBuf);
#endif

#ifdef WR
/*-- stack manager initialization function ---*/
EXTERN S16 wrActvInit   (Ent ent, Inst inst,
                                Region region, Reason reason);
/*---- stack manager activation function -----*/
EXTERN S16 wrActvTsk    (Pst *pst, Buffer *mBuf);

/* Adding this function as part of the dm Active init function*/
EXTERN S16 wrDamActvTsk    (Pst *pst, Buffer *mBuf);

#endif

#ifdef EU_DAT_APP
/*-- stack manager initialization function ---*/
EXTERN S16 wrDamActvInit   (Ent ent, Inst inst,
                                Region region, Reason reason);
/*---- stack manager activation function -----*/
EXTERN S16 wrDamActvTsk    (Pst *pst, Buffer *mBuf);
EXTERN S16 cmUnpkWrNghCfg(LwrNeighCellCfg *param,Buffer *mBuf,Pst *pst);
EXTERN S16 cmPkWrMmeCfg(LwrMmeCfg  *param,Buffer       *mBuf);
EXTERN S16 cmUnpkLwrCfgReq(LwrCfgReq func,Pst *psti,Buffer *mBuf);
EXTERN S16 cmUnpkLwrDynCfgReq(LwrDynCfgReq func,Pst    *pst,Buffer *mBuf);
EXTERN S16 cmUnpkLwrCntrlReq(LwrCntrlReq func,Pst *pst,Buffer *mBuf);
EXTERN S16 cmPkWrGenCfg(LwrGenCfg *param,Buffer *mBuf);
EXTERN S16 cmUnpkWrNghCfg(LwrNeighCellCfg *param,Buffer *mBuf,Pst *pst);
EXTERN S16 cmPkWrMmeCfg(LwrMmeCfg  *param,Buffer    *mBuf);
EXTERN S16 cmUnpkWrMmeCfg(LwrMmeCfg  *param,Buffer   *mBuf,Pst  *pst);
EXTERN S16 cmPkWrUstaDgn(LwrUstaDgn *param,Buffer *mBuf);
EXTERN S16 cmPkWrUsta(LwrUsta *param,Buffer *mBuf);
EXTERN S16 cmPkWrCntrl(LwrCntrl *param,U16 elmnt,Buffer *mBuf);
EXTERN S16 cmPkWrCntrl(LwrCntrl *param,U16 elmnt,Buffer *mBuf);
EXTERN S16 cmPkWrLSapCfg(LwrLSapCfg *param,Buffer *mBuf);
EXTERN S16 cmPkWrCfg(LwrCfg *param,U16 elmnt,Buffer *mBuf);
EXTERN S16 cmPkWrMngmt(LwrMngmt *param,U8 evnt,Buffer *mBuf);
EXTERN S16 cmUnpkWrUstaDgn(LwrUstaDgn *param,Buffer *mBuf);
EXTERN S16 cmUnpkWrUsta(LwrUsta *param,Buffer *mBuf);
EXTERN S16 cmUnpkWrCntrl(LwrCntrl *param,U16 elmnt,Buffer *mBuf);
EXTERN S16 cmUnpkWrLSapCfg(LwrLSapCfg *param,Buffer *mBuf);
EXTERN S16 cmUnpkWrCfg(LwrCfg *param,U16 elmnt,Buffer *mBuf,Pst *pst);
EXTERN S16 cmUnpkWrMngmt(LwrMngmt *param,U8 evnt,Ptr ptr, Buffer *mBuf, Pst *pst);
EXTERN S16 cmUnpkLwrLoadInd(LwrLoadInd func, Pst *pst, Buffer *mBuf);
EXTERN S16 cmUnpkWrGenCfg(LwrGenCfg *param, Buffer *mBuf,Pst *pst);
EXTERN S16 cmPkWrDbgCntrl(LwrDbgCntrl *param, Buffer *mBuf);
EXTERN S16 cmUnpkWrDbgCntrl(LwrDbgCntrl *param, Buffer *mBuf);
EXTERN S16 cmUnpkLwrNghCfgInd (LwrCfgReq func, Pst *pst, Buffer *mBuf);

#endif

EXTERN S16 wrEmmSndSchdReCfgReq(U32  cellId,U32  minDlResNonCsg,U32  minUlResNonCsg);
EXTERN S16 cmUnPackSendStaticCfg(LwrStaticCfgReqSend func, Pst *pst, Buffer *mBuf);
EXTERN S16 cmUnPackSendDynCfg(LwrDynCfgReqSend func, Pst *pst, Buffer *mBuf);
EXTERN S16 smUnPackStartStopStackReq(LwrStartStopStackReq func, Pst *pst, Buffer *mBuf);
EXTERN S16 SmMiLwrHdlNghCellInd(Pst *pst,LwrNghCellInd *nghCellInfo);
EXTERN S16 smBldAndSendNghCellInd(LwrNghCellInd *nghCellInd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LWR_H__ */ 

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
