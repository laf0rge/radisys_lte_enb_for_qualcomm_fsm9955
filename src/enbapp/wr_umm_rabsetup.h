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
               

     File:     wr_umm_rabsetup.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_UMM_RABSETUP_H__
#define __WR_UMM_RABSETUP_H__

#include "wr.h"
#include "wr_cmn.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

/* The below two definitions are for field schdCfgState in structure      */
/*   WrUmmRabSetupRabInfo                                                 */
#define WR_UMM_RAB_SETUP_CFG_IDLE 0
#define WR_UMM_RAB_SETUP_CFG_SENT 1
#define WR_UMM_RAB_SETUP_CFG_FAIL 2
#define WR_UMM_RAB_SETUP_CFG_SUCC 3
#define WR_UMM_RAB_SETUP_CFG_ERRO 4


#define WR_UMM_RAB_SETUP_UE_CFG_SENT    0x01
#define WR_UMM_RAB_SETUP_LCG_CFG_SENT   0x02
#define WR_UMM_RAB_SETUP_LCH_CFG_SENT   0x04
#define WR_UMM_RAB_SETUP_UE_CFG_RECVD   0x08
#define WR_UMM_RAB_SETUP_LCG_CFG_RECVD  0x10
#define WR_UMM_RAB_SETUP_LCH_CFG_RECVD  0x20
#define WR_UMM_RAB_SETUP_CFG_RECVD      (WR_UMM_RAB_SETUP_UE_CFG_RECVD | WR_UMM_RAB_SETUP_LCG_CFG_RECVD | WR_UMM_RAB_SETUP_LCH_CFG_RECVD)

/* The below macros are used to mark if a E-RAB is to be established      */
/* rabAllowed field will be set with one of the values below              */
/* WR_UMM_RAB_SETUP_ALLOWED   - E-RAB is allowed for setup                */
/* WR_UMM_RAB_SETUP_QCI_UNSUP - E-RAB is disallowed due to QCI value      */
/* WR_UMM_RAB_SETUP_RRM_DISAL - E-RAB is disallowed by RRM                */
/* WR_UMM_RAB_SETUP_OUT_RES   - E-RAB cannot be established as eNB is out */
/*                               of resources to establish the E-RAB      */
#define WR_UMM_RAB_SETUP_ALLOWED      0
#define WR_UMM_RAB_SETUP_QCI_UNSUP    1
#define WR_UMM_RAB_SETUP_RRM_DISAL    2
#define WR_UMM_RAB_SETUP_OUT_RES      3
/*Fix For ccpu00123304*/
#define WR_UMM_RAB_SETUP_RABID_UNSUP  4

/** @brief This structure contains the ARP info of the Radio Access Bearer
 *
 - U16           priority  - This idicates the prioriy of the bearer
 - Bool          preemptCap   - This indicates the preemption capability of the bearer
 - Bool          preemptVulnerability  - This indicates the preemption vulnerability
*/
typedef struct wrUmmRabSetupArpInfo
{
   U32                        priorityLvl;
   U32                        preemptCapability;
   U32                        preemptVulnerability;
} WrUmmRabSetupArpInfo;

/** 
 * @brief This structure contains the information regarding the ERAB to be
 *        setup.
 *
 * @details These are the structure members
 * - U8          qci            QCI
 * - U8          erabId         ERAB ID
 * - U8          drbId          Data Radio Bearer ID
 * - U8          rabAllowed     Flag to indicate if the ERAB is admitted
 * - U8          rlcMode        RLC Mode
 * - U8          lchId          Logical Channel ID
 * - U8          schdCfgState   Flag to indicate the scheduler configuration 
 *                              state
 * - U8          tnlCfgState    Flag to indicate the creation of tunnels
 * - U32         ulGbr          Uplink Guarenteed Bit Rate
 * - U32         dlGbr          Downlink Guarenteed Bit Rate
 * - U32         ulMbr          Uplink MBR
 * - U32         dlMbr          Downlink MBR
 * - TknStrOSXL  *nasPdu        NAS PDU to be sent to UE
 * - CmTptAddr   sgwAddr        Serving Gateway IP Address
 * - U32         remTeid        Remote (SGW) Tunnel ID
 * - U32         lclTeid        Local Tunnel ID
 */
typedef struct wrUmmRabSetupRabInfo
{
   U8                        qci;
   U8                        erabId;
   U8                        drbId;
   U8                        rabAllowed;
   U8                        rlcMode;
   U8                        lchId;
   U8                        schdCfgState;
   U8                        tnlCfgState;
   U32                       ulGbr;
   U32                       dlGbr;
   U32                       ulMbr;
   U32                       dlMbr;
   TknStrOSXL                *nasPdu;
   CmTptAddr                 sgwAddr;
   U32                       remTeid;
   U32                       lclTeid;
   WrUmmRabSetupArpInfo      arpInfo;
   WrUmmMsgCause             cause;     /* Added cause value for E-Rab Setup procedures */
#ifdef WR_RSYS_KPI
   EpcTime    earbSetupReqTm; /*timestamp(ms) when Enb recevies earb setup req or initial context req from MME*/
   EpcTime    erabEstabTm;  /*timestamp(ms) when Enb sedn erab setup rsp*/
#endif /*WR_RSYS_OAM*/
   /* SPS changes starts */
   Bool                      isDlSpsEnabled;
   Bool                      isUlSpsEnabled;
   /* SPS changes ends */
} WrUmmRabSetupRabInfo;

/* The following definitions are for the field status in the structure    */
/* below.                                                                 */
#define WR_UMM_RAB_SETUP_INITIATED       0
#define WR_UMM_RAB_SETUP_SCHD_CFG_DONE   1
#define WR_UMM_RAB_SETUP_DAM_CFG_DONE    2
#define WR_UMM_RAB_SETUP_STACK_CFG_DONE  3
#define WR_UMM_RAB_SETUP_SCH_CFG_DRX     4

/** 
 * @brief This structure contains the parameters to assist the processing
 *        of this transaction.
 *
 * @details These are the structure members
 * - U32                   dlAmbr           Downlink Aggregated Max Bit Rate
 * - U32                   ulAmbr           Uplink Aggregated Max Bit Rate
 * - U8                    schdUeCfgState   Flag to indicate UE Configuration
 *                                          state
 * - U8                    transId          This field can take values 0 to 3 
 *                                          as per RRC specification
 * - U8                    state            The stage of the procedure 
 *                                          whether configuring scheduler, RRC
 *                                          etc.
 * - U8                    curCfgRes
 * - U32                   numRabs          Number of RABs requested to be
 *                                          established
 * - U32                   numRabsAllowed   Number of RABs allowed
 * - WrUmmRabSetupRabInfo  *rabs            Information about RABs to be 
 *                                          established
 * - Void                  *msg             RAB Setup Request message stored
 *                                          to access NAS PDU information
 * - U8                    schdLchCfgCmpl   Flag to indicate Logical Channel 
 *                                          Configuration status
 * - Bool                  tnlSetupCmpl     Flag to indicate DAM configuration
 *                                          status
 * - Bool                  isDrbCfgDone     Flag to indicate if lower layer
 *                                          configuration for this RAB is done
 * - U8                    isUeAggrMaxBitRateRcvd Flag to indicate if the UE
 *                                                aggr bit rate IE is rcvd
 * - U32                   cfgStateMsk      Bitmask to maintain Schedular msg state
 */
typedef struct wrUmmRabSetupTrans
{
   U32                       dlAmbr;
   U32                       ulAmbr;
   U8                        schdUeCfgState;
   U8                        transId;
   U8                        state;
   U8                        curCfgRes;
   U32                       numRabs;
   U32                       numRabsAllowed;
   WrUmmRabSetupRabInfo      *rabs;
   Void                      *msg;
   U8                        schdLchCfgCmpl;
   Bool                      tnlSetupCmpl;
   Bool                      isDrbCfgDone;
   Bool                      rrcUndostate;
   Bool                      startAnr;
   U8                        schdUndoCfgCnt;
   WrUmmIncMsg               *rrmUeRecfgRspMsg;
   U8                        isUeAggrMaxBitRateRcvd;
   U32                       cfgStateMsk;
   U8                        lcgCfgStatus[WR_UMM_MAX_LCG_GRP_ID];
   U8                        expectedLcgCfgStatus;
} WrUmmRabSetupTrans;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_UMM_RABSETUP_H__ */

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
