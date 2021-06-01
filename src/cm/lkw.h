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
  
        Name:    LKW RLC Layer Manager Interface
    
        Type:    C include file
  
        Desc:    This file Contains the Hash defines for LKW Interface
 
        File:    lkw.h

        Sid:      lkw.h@@/main/4 - Fri Sep  2 18:20:55 2011
  
        Prg:     chakrapani
  
*********************************************************************21*/

#ifndef __LKW_H__
#define __LKW_H__

#ifdef __cplusplus
EXTERN "C" {
#endif  /*for extern "C"*/

#define STUDXSAP           189
/* KWU Interface Hash Defines */
#define LKW_MAX_NUM_RB     24      /*!< Maximum number of RBs. */
#define LKW_RLC_MODE_DL     1
#define LKW_RLC_MODE_UL     2 

/* Event codes at the RLC management interface */
#define LKW_EVT_CFG_REQ    0x01    /*!< General configuration request. */
#define LKW_EVT_CFG_CFM    0x02    /*!< General configuration confirm. */
#define LKW_EVT_CNTRL_REQ  0x03    /*!< Control request. */
#define LKW_EVT_CNTRL_CFM  0x04    /*!< Control confirm. */
#define LKW_EVT_STA_IND    0x05    /*!< Unsolicited status indication. */
#define LKW_EVT_STA_REQ    0x06    /*!< Solicited status request. */
#define LKW_EVT_STA_CFM    0x07    /*!< Solicited status confirm. */
#define LKW_EVT_STS_REQ    0x08    /*!< Statistics request. */
#define LKW_EVT_STS_CFM    0x09    /*!< Statistics confirm. */
#define LKW_EVT_TRC_IND    0x0A    /*!< Trace indication. */
/* lkw_h_001.main_3 added support for L2 Measurement */   
#define LKW_EVT_L2MEAS_REQ 0x0B    /*!< L2 Measurement Request. */
#define LKW_EVT_L2MEAS_CFM 0x0C    /*!< L2 Measurement Request. */
#define LKW_EVT_L2MEAS_SEND_REQ 0x0D    /*!< L2 Measurement send request*/
#define LKW_EVT_L2MEAS_STOP_REQ 0x0E   /*!< L2 Measurement stop request.*/
#define LKW_EVT_L2MEAS_STOP_CFM 0x0F   /*!< L2 Measurement stop confirm*/

#define LKW_ZEROSTS        0       /*!< Reset the statistics counters. */
#define LKW_NOZEROSTS      1       /*!< No reset of statistics counters. */

/********************************************************************KW**
 Specific event values
*********************************************************************KW*/
#define LKW_EVENT_RGU_BND_CFM        (LCM_EVENT_LYR_SPECIFIC + 1)
#define LKW_EVENT_KWU_BND_REQ        (LCM_EVENT_LYR_SPECIFIC + 2)
#define LKW_EVENT_CKW_BND_REQ        (LCM_EVENT_LYR_SPECIFIC + 3)

/********************************************************************KW**
 Specific event values
*********************************************************************KW*/
#define LKW_EVENT_UDX_BND_CFM        (LCM_EVENT_LYR_SPECIFIC + 1)
#define LKW_EVENT_UDX_BND_REQ        (LCM_EVENT_LYR_SPECIFIC + 2)


/********************************************************************KW**
 Specific cause values
*********************************************************************KW*/
#define LKW_CAUSE_UNKNOWN        (LCM_CAUSE_LYR_SPECIFIC + 1)
#define LKW_CAUSE_SAP_BNDENB     (LCM_CAUSE_LYR_SPECIFIC + 2)
#define LKW_CAUSE_SAP_BNDDIS     (LCM_CAUSE_LYR_SPECIFIC + 3)
/* lkw_h_001.main_3 corrected the macro. Added L2 Measurement support */
#define LKW_CAUSE_RSRC_UNAVAIL   (LCM_CAUSE_LYR_SPECIFIC + 4)
#ifdef LTE_L2_MEAS
#define LKW_CAUSE_RB_DELETION      (LCM_CAUSE_LYR_SPECIFIC + 5)
#define LKW_CAUSE_INVALID_MEASTYPE (LCM_CAUSE_LYR_SPECIFIC + 6)
#define LKW_CAUSE_EXCEED_NUMMEAS   (LCM_CAUSE_LYR_SPECIFIC + 7)
#define LKW_CAUSE_INVALID_NUMQCI   (LCM_CAUSE_LYR_SPECIFIC + 8)
#define LKW_CAUSE_ZERO_NUMSAM      (LCM_CAUSE_LYR_SPECIFIC + 9)
#define LKW_CAUSE_INVALID_NUMSAM   (LCM_CAUSE_LYR_SPECIFIC + 10)
#define LKW_CAUSE_INVALID_UE       (LCM_CAUSE_LYR_SPECIFIC + 11)
#define LKW_CAUSE_INVALID_QCI      (LCM_CAUSE_LYR_SPECIFIC + 12)
#define LKW_CAUSE_MEAS_ALREADY_ENA (LCM_CAUSE_LYR_SPECIFIC + 13)
#endif /* LTE_L2_MEAS */
#define LKW_CAUSE_RLSTXLST_FULL       (LCM_CAUSE_LYR_SPECIFIC + 14)
#define LKW_CAUSE_RLSRETXLST_FULL     (LCM_CAUSE_LYR_SPECIFIC + 15)
#define LKW_CAUSE_REASMBLDAMLST_FULL  (LCM_CAUSE_LYR_SPECIFIC + 16)
#define LKW_CAUSE_REASMBLDUMLST_FULL  (LCM_CAUSE_LYR_SPECIFIC + 17)
#ifdef LTE_L2_MEAS
#define LKW_CAUSE_EXCEED_NUMUES       (LCM_CAUSE_LYR_SPECIFIC + 18)
#endif /* LTE_L2_MEAS */
/************************************************************************
 *                         Events
 ***********************************************************************/
#define LKW_EVT_BND_REQ       0     /*!< Bind Request Event. */
#define LKW_EVT_BND_CFM       1     /*!< Bind Confirm Event. */
#define LKW_EVT_UBND_REQ      2     /*!< Unbind Request Event. */
#define LKW_EVT_WAIT_BNDCFM   3     /*!< Wait Bind Confirm Event. */
/* lkw_h_001.main_3 added support for L2 Measurement */   
#ifdef LTE_L2_MEAS
#define LKW_EVT_MEAS_HALT     4     /*!< Measurement is halted */
#endif /* LTE_L2_MEAS */
#define LKW_EVENT_STA_PDU     5     /*!< Status PDU from other node */
#define LKW_EVENT_AM_UL_PDU   6     /*!< UL PDU from MAC in AM */
#define LKW_EVENT_UM_UL_PDU   7     /*!< Ul PDU from MAC in UM */
/*************************************************************************
 *                            KWU SAPs
 ************************************************************************/
#define LKW_RRC_SAP   0x00         /*!< RLU SAP ID for RRC */
#define LKW_PDCP_SAP  0x01         /*!< RLU SAP ID for PDCP */

/********************************************************************KW**
 Usta defines
*********************************************************************KW*/
#define LKW_USTA_DGNVAL_NONE     1
#define LKW_USTA_DGNVAL_MEM      2
#define LKW_USTA_DGNVAL_SAPID    3
#define LKW_USTA_DGNVAL_UEID     4
#define LKW_USTA_MAX_DGNVAL      256

/* lkw_h_001.main_3 added support for L2 Measurement */   
/********************************************************************KW**
 L2 MEASUREMENT VALUES 
*********************************************************************KW*/
#define LKW_L2MEAS_NONE       0x0000
#define LKW_L2MEAS_ACT_UE     0x0001
#define LKW_L2MEAS_DL_DELAY   0x0002
#define LKW_L2MEAS_DL_DISC    0x0004
#define LKW_L2MEAS_UU_LOSS    0x0008
#define LKW_L2MEAS_DL_IP      0x0010
#define LKW_L2MEAS_UL_IP      0x0020

#define LKW_MAX_L2MEAS        10
#define LKW_MAX_QCI           10
#define LKW_MAX_UE            TENB_MAX_UE_SUPPORTED

/********************************************************************KW**
 Specific event values
*********************************************************************KW*/
#define LKW_EVENT_LI_BND_CFM        (LCM_EVENT_LYR_SPECIFIC + 1)

#define LKW_PART_NUM_STR_LEN        10        /*!< Part Number String Length. */

/* lkw_h_001.main_2 */
#define LKW_FULL_TRACE              -1        /*!< Trace macro to get full trace */

/* Added Log Error for LKW Interface */
#define LKWLOGERROR(_pst, _errCls, _errCode, _errVal, _errDesc) \
        SLogError(_pst->srcEnt, _pst->srcInst, _pst->srcProcId, \
                   __FILE__, __LINE__, _errCls, _errCode, _errVal, _errDesc)

/* Error Codes */
#define   ERRLKW         0
#define   ELKWXXX        0

#define   ELKW001      (ERRLKW +    1)    /*        lkw.c: 203 */
#define   ELKW002      (ERRLKW +    2)    /*        lkw.c: 218 */
#define   ELKW003      (ERRLKW +    3)    /*        lkw.c: 272 */
#define   ELKW004      (ERRLKW +    4)    /*        lkw.c: 322 */
#define   ELKW005      (ERRLKW +    5)    /*        lkw.c: 337 */
#define   ELKW006      (ERRLKW +    6)    /*        lkw.c: 390 */
#define   ELKW007      (ERRLKW +    7)    /*        lkw.c: 436 */
#define   ELKW008      (ERRLKW +    8)    /*        lkw.c: 451 */
#define   ELKW009      (ERRLKW +    9)    /*        lkw.c: 505 */
#define   ELKW010      (ERRLKW +   10)    /*        lkw.c: 556 */
#define   ELKW011      (ERRLKW +   11)    /*        lkw.c: 604 */
#define   ELKW012      (ERRLKW +   12)    /*        lkw.c: 619 */
#define   ELKW013      (ERRLKW +   13)    /*        lkw.c: 672 */
#define   ELKW014      (ERRLKW +   14)    /*        lkw.c: 720 */
#define   ELKW015      (ERRLKW +   15)    /*        lkw.c: 735 */
#define   ELKW016      (ERRLKW +   16)    /*        lkw.c: 789 */
#define   ELKW017      (ERRLKW +   17)    /*        lkw.c: 835 */
#define   ELKW018      (ERRLKW +   18)    /*        lkw.c: 850 */
#define   ELKW019      (ERRLKW +   19)    /*        lkw.c: 902 */
#define   ELKW020      (ERRLKW +   20)    /*        lkw.c: 956 */
#define   ELKW021      (ERRLKW +   21)    /*        lkw.c:1008 */
#define   ELKW022      (ERRLKW +   22)    /*        lkw.c:1061 */
#define   ELKW023      (ERRLKW +   23)    /*        lkw.c:1294 */
#define   ELKW024      (ERRLKW +   24)    /*        lkw.c:1309 */
#define   ELKW025      (ERRLKW +   25)    /*        lkw.c:1358 */
#define   ELKW026      (ERRLKW +   26)    /*        lkw.c:1375 */
#define   ELKW027      (ERRLKW +   27)    /*        lkw.c:1393 */
#define   ELKW028      (ERRLKW +   28)    /*        lkw.c:1445 */
#define   ELKW029      (ERRLKW +   29)    /*        lkw.c:1460 */
#define   ELKW030      (ERRLKW +   30)    /*        lkw.c:1511 */
#define   ELKW031      (ERRLKW +   31)    /*        lkw.c:1526 */


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __LKW_H__ */

  
/********************************************************************30**
         End of file:     lkw.h@@/main/4 - Fri Sep  2 18:20:55 2011
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
