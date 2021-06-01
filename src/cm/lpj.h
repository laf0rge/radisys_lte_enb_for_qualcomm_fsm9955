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
  
        Name:    LPJ PDCP Layer Manager Interface
    
        Type:    C include file
  
        Desc:    This file Contains the Hash defines for LPJ Interface
 
        File:    lpj.h

        Sid:      lpj.h@@/main/3 - Fri Sep  2 19:18:15 2011
  
        Prg:     chakrapani
  
*********************************************************************21*/

#ifndef __LPJ_H__
#define __LPJ_H__

#ifdef __cplusplus
EXTERN "C" {
#endif  /*for extern "C"*/

#define STUDXSAP           189

#define LPJ_SEL_LC         0
#ifdef TENB_SPLIT_ARCH
#define LPJ_MODE_PDCP_UL   0x1
#define LPJ_MODE_PDCP_DL   0x2
#endif

/* KWU Interface Hash Defines */
#define LPJ_MAX_NUM_RB     24      /*!< Maximum number of RBs. */

/* Event codes at the PDCP management interface */
#define LPJ_EVT_CFG_REQ    0x50    /*!< General configuration request. */
#define LPJ_EVT_CFG_CFM    0x51    /*!< General configuration confirm. */
#define LPJ_EVT_CNTRL_REQ  0x52    /*!< Control request. */
#define LPJ_EVT_CNTRL_CFM  0x53    /*!< Control confirm. */
#define LPJ_EVT_STA_IND    0x54    /*!< Unsolicited status indication. */
#define LPJ_EVT_STA_REQ    0x55    /*!< Solicited status request. */
#define LPJ_EVT_STA_CFM    0x56    /*!< Solicited status confirm. */
#define LPJ_EVT_STS_REQ    0x57    /*!< Statistics request. */
#define LPJ_EVT_STS_CFM    0x58    /*!< Statistics confirm. */
#define LPJ_EVT_TRC_IND    0x59    /*!< Trace indication. */
/* lpj_h_001.main_2 added support for L2 Measurement */
#define LPJ_EVT_L2MEAS_REQ 0x60    /*!< L2 Measurement Request. */
#define LPJ_EVT_L2MEAS_CFM 0x61    /*!< L2 Measurement Confirm. */
#ifdef LTE_L2_MEAS
#define LPJ_EVT_MEAS_HALT  0x62    /*!< Measurement is halted due to RB deletion */
#define LPJ_EVT_L2MEAS_SEND_REQ  0x63   /*!< L2 Measurement Send Request*/
#define LPJ_EVT_L2MEAS_STOP_REQ  0x64   /*!< L2 Measurement Stop Request*/
#define LPJ_EVT_L2MEAS_STOP_CFM  0x65   /*!< L2 Measurement Stop Confirm*/    
#endif
#define LPJ_EVT_L2MEAS_SEND_REQ 0x63    /*!< L2 Measurement send request*/
#define LPJ_EVT_L2MEAS_STOP_REQ 0x64   /*!< L2 Measurement stop request.*/
#define LPJ_EVT_L2MEAS_STOP_CFM 0x65   /*!< L2 Measurement stop confirm*/

#define LPJ_ZEROSTS        0       /*!< Reset the statistics counters. */
#define LPJ_NOZEROSTS      1       /*!< No reset of statistics counters. */

/********************************************************************KW**
 Specific event values
*********************************************************************KW*/
#define LPJ_EVENT_RGU_BND_CFM        (LCM_EVENT_LYR_SPECIFIC + 1)
#define LPJ_EVENT_KWU_BND_REQ        (LCM_EVENT_LYR_SPECIFIC + 2)
#define LPJ_EVENT_CKW_BND_REQ        (LCM_EVENT_LYR_SPECIFIC + 3)
#define LPJ_EVENT_CPJ_BND_REQ        (LCM_EVENT_LYR_SPECIFIC + 4)
#define LPJ_EVENT_PJU_BND_REQ        (LCM_EVENT_LYR_SPECIFIC + 5)

/********************************************************************KW**
 Specific cause values
*********************************************************************KW*/
#define LPJ_CAUSE_UNKNOWN        (LCM_CAUSE_LYR_SPECIFIC + 1)
#define LPJ_CAUSE_SAP_BNDENB     (LCM_CAUSE_LYR_SPECIFIC + 2)
#define LPJ_CAUSE_SAP_BNDDIS     (LCM_CAUSE_LYR_SPECIFIC + 3)
#define LMG_CAUSE_RSRC_UNAVAIL   (LCM_CAUSE_LYR_SPECIFIC + 4)
/* lpj_h_001.main_2 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
#define LPJ_CAUSE_MEM_FAILED         (LCM_CAUSE_LYR_SPECIFIC + 5)
#define LPJ_CAUSE_INVALID_MEASTYPE   (LCM_CAUSE_LYR_SPECIFIC  + 6)
#define LPJ_CAUSE_EXCEED_MEASREQ     (LCM_CAUSE_LYR_SPECIFIC  + 7)
#define LPJ_CAUSE_EXCEED_NUMQCI      (LCM_CAUSE_LYR_SPECIFIC  + 8)
#define LPJ_CAUSE_MEAS_ALREADY_ENA   (LCM_CAUSE_LYR_SPECIFIC  + 9)
#define LPJ_CAUSE_INVALID_QCI        (LCM_CAUSE_LYR_SPECIFIC + 10)
#define LPJ_CAUSE_ZERO_MEASPERIOD    (LCM_CAUSE_LYR_SPECIFIC + 11)
#define LPJ_CAUSE_RB_DELETION        (LCM_CAUSE_LYR_SPECIFIC + 12)
#endif
/************************************************************************
 *                         Events
 ***********************************************************************/
#define LPJ_EVT_BND_REQ       0     /*!< Bind Request Event. */
#define LPJ_EVT_BND_CFM       1     /*!< Bind Confirm Event. */
#define LPJ_EVT_UBND_REQ      2     /*!< Unbind Request Event. */
#define LPJ_EVT_WAIT_BNDCFM   3     /*!< Wait Bind Confirm Event. */

/*************************************************************************
 *                            KWU SAPs
 ************************************************************************/
#define LPJ_RRC_SAP   0x00         /*!< RLU SAP ID for RRC */
#define LPJ_PDCP_SAP  0x01         /*!< RLU SAP ID for PDCP */

/********************************************************************KW**
 Usta defines
*********************************************************************KW*/
#define LPJ_USTA_DGNVAL_NONE     1
#define LPJ_USTA_DGNVAL_MEM      2
#define LPJ_USTA_DGNVAL_SAPID    3
#define LPJ_USTA_DGNVAL_UEID     4
#define LPJ_USTA_MAX_DGNVAL      256
/********************************************************************KW**
 Specific event values
*********************************************************************KW*/
#define LPJ_EVENT_LI_BND_CFM        (LCM_EVENT_LYR_SPECIFIC + 1)

#define LPJ_PART_NUM_STR_LEN        10        /*!< Part Number String Length. */

/* lpj_h_001.main_2 added support for L2 Measurement */
/********************************************************************KW**
 L2 MEASUREMENT VALUES 
*********************************************************************KW*/
#ifdef LTE_L2_MEAS
#define LPJ_L2MEAS_NONE       0x0000
#define LPJ_L2MEAS_DL_DELAY   0x0001
#define LPJ_L2MEAS_DL_DISC    0x0002
#define LPJ_L2MEAS_UL_LOSS    0x0004
#define LPJ_L2MEAS_DLDP_BIT_RATE 0x0008 /*Added for L2 Meas*/
#define LPJ_L2MEAS_ULDP_BIT_RATE 0x0010
#define LPJ_L2MEAS_DLCP_BIT_RATE 0x0020 
#define LPJ_L2MEAS_ULCP_BIT_RATE 0x0040
#define LPJ_L2CPU_PERCORE_STATS  0x0080
#define LPJ_L2MEM_PERPOOL_STATS  0x0100

#define LPJ_MAX_L2MEAS        5 
#define LPJ_MAX_QCI             10
#endif
/* lpj_h_001.main_1 */
#define LPJ_FULL_TRACE              -1        /*!< Trace macro to get full trace */   
/* Added Log Error for LPJ Interface */
#define LPJLOGERROR(_pst, _errCls, _errCode, _errVal, _errDesc) \
        SLogError(_pst->srcEnt, _pst->srcInst, _pst->srcProcId, \
                   __FILE__, __LINE__, _errCls, _errCode, _errVal, _errDesc)

/* Error Codes */
#define   ERRLPJ         0

#define ELPJ001         (ERRLPJ + 1)
#define ELPJ002         (ERRLPJ + 2)
#define ELPJ003         (ERRLPJ + 3)
#define ELPJ004         (ERRLPJ + 4)
#define ELPJ005         (ERRLPJ + 5)
#define ELPJ006         (ERRLPJ + 6)
#define ELPJ007         (ERRLPJ + 7)
#define ELPJ008         (ERRLPJ + 8)
#define ELPJ009         (ERRLPJ + 9)
#define ELPJ010         (ERRLPJ + 10)
#define ELPJ011         (ERRLPJ + 11)
#define ELPJ012         (ERRLPJ + 12)
#define ELPJ013         (ERRLPJ + 13)
#define ELPJ014         (ERRLPJ + 14)
#define ELPJ015         (ERRLPJ + 15)
#define ELPJ016         (ERRLPJ + 16)
#define ELPJ017         (ERRLPJ + 17)
#define ELPJ018         (ERRLPJ + 18)
#define ELPJ019         (ERRLPJ + 19)
#define ELPJ020         (ERRLPJ + 20)
#define ELPJ021         (ERRLPJ + 21)
#define ELPJ022         (ERRLPJ + 22)
#define ELPJ023         (ERRLPJ + 23)
#define ELPJ024         (ERRLPJ + 24)
#define ELPJ025         (ERRLPJ + 25)
#define ELPJ026         (ERRLPJ + 26)
#define ELPJ027         (ERRLPJ + 27)
#define ELPJ028         (ERRLPJ + 28)
#define ELPJ029         (ERRLPJ + 29)
#define ELPJ030         (ERRLPJ + 30)
#define ELPJ031         (ERRLPJ + 31)
#define ELPJ032         (ERRLPJ + 32)
#define ELPJ033         (ERRLPJ + 33)
#define ELPJ034         (ERRLPJ + 34)
#define ELPJ035         (ERRLPJ + 35)
#define ELPJ036         (ERRLPJ + 36)
#define ELPJ037         (ERRLPJ + 37)
#define ELPJ038         (ERRLPJ + 38)
#define ELPJ039         (ERRLPJ + 39)
#define ELPJ040         (ERRLPJ + 40)
#define ELPJ041         (ERRLPJ + 41)
#define ELPJ042         (ERRLPJ + 42)
#define ELPJ043         (ERRLPJ + 43)
#define ELPJ044         (ERRLPJ + 44)
#define ELPJ045         (ERRLPJ + 45)
#define ELPJ046         (ERRLPJ + 46)
#define ELPJ047         (ERRLPJ + 47)
#define ELPJ048         (ERRLPJ + 48)
#define ELPJ049         (ERRLPJ + 49)
#define ELPJ050         (ERRLPJ + 50)
#define ELPJ051         (ERRLPJ + 51)
#define ELPJ052         (ERRLPJ + 52)
#define ELPJ053         (ERRLPJ + 53)
#define ELPJ054         (ERRLPJ + 54)
#define ELPJ055         (ERRLPJ + 55)
#define ELPJ056         (ERRLPJ + 56)
#define ELPJ057         (ERRLPJ + 57)
#define ELPJ058         (ERRLPJ + 58)
#define ELPJ059         (ERRLPJ + 59)
#define ELPJ060         (ERRLPJ + 60)
#define ELPJ061         (ERRLPJ + 61)
#define ELPJ062         (ERRLPJ + 62)
#define ELPJ063         (ERRLPJ + 63)
#define ELPJ064         (ERRLPJ + 64)
#define ELPJ065         (ERRLPJ + 65)
#define ELPJ066         (ERRLPJ + 66)
#define ELPJ067         (ERRLPJ + 67)

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __LPJ_H__ */

  
/********************************************************************30**
         End of file:     lpj.h@@/main/3 - Fri Sep  2 19:18:15 2011
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
/main/1      ---      avl         1. LTE RLC GA 3.1 Initial release.
*********************************************************************91*/
