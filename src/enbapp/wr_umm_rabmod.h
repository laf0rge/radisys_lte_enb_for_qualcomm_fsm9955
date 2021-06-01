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
               

     File:     wr_umm_rabmod.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_UMM_RABMOD_H__
#define __WR_UMM_RABMOD_H__

#include "wr.h"
#include "wr_cmn.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

/* The below two definitions are for field schdCfgState in structure      */
/*   WrUmmRabModifyRabInfo                                                 */
#define WR_RAB_MODIFY_SCHD_CFG_IDLE 0
#define WR_RAB_MODIFY_SCHD_CFG_SENT 1
#define WR_RAB_MODIFY_SCHD_CFM_FAIL 2
#define WR_RAB_MODIFY_SCHD_CFM_SUCC 3
#define WR_RAB_MODIFY_SCHD_CFM_FAIL_RVRT 4


#define WR_UMM_RAB_MODIFY_UE_CFG_SENT   0x01
#define WR_UMM_RAB_MODIFY_LCG_CFG_SENT  0x02
#define WR_UMM_RAB_MODIFY_LCH_CFG_SENT  0x04
#define WR_UMM_RAB_MODIFY_UE_CFG_RECVD  0x08
#define WR_UMM_RAB_MODIFY_LCG_CFG_RECVD 0x10
#define WR_UMM_RAB_MODIFY_LCH_CFG_RECVD 0x20
#define WR_UMM_RAB_MODIFY_CFG_RECVD (WR_UMM_RAB_MODIFY_UE_CFG_RECVD | WR_UMM_RAB_MODIFY_LCG_CFG_RECVD | WR_UMM_RAB_MODIFY_LCH_CFG_RECVD)

/** @brief This structure contains the ARP info of the Radio Access Bearer
 *
 - U16           priority  - This idicates the prioriy of the bearer
 - Bool          preemptCap   - This indicates the preemption capability of the bearer
 - Bool          preemptVulnerability  - This indicates the preemption vulnerability
*/
typedef struct wrUmmRabArpInfo
{
   U32                        priorityLvl;
   U32                        preemptCapability;
   U32                        preemptVulnerability;
} WrUmmRabArpInfo;

/** 
  * @brief This is a structure for Rab Modify 
  * transaction each RAB information. 
  * @details The structure members 
  * - U8                        qci          QCI Value
  * - U8                        erabId       E-RAB ID 
  * - U8                        rabAllowed   Rab Allowed flag
  * - Bool                      rabfailed    Flag to say rab failed to modify             
  * - U8                        drbId        Data Radio Bearer Id
  * - U8                        lchId        Logical channel Id
  * - U8                        schdCfgState SCHD Cfg state
  * - U32                       ulGbr        UL Gauranteed Bit Rate 
  * - U32                       dlGbr        DL Gauranteed Bit Rate  
  * - U32                       ulMbr        UL Maximum Bit Rate 
  * - U32                       dlMbr        DL Maximum Bit Rate 
  * - TknStrOSXL                *nasPdu      NAS PDU for each RAB
  */
typedef struct wrUmmRabModifyRabInfo
{
   U8                        qci;
   U8                        erabId;
   U8                        drbId;
   Bool                      rabAllowed;     /* Removed rabFailed as per the comments*/
   U8                        rlcMode;
   U8                        lchId;
   U8                        schdCfgState;
   U8                        schdRvrtCfgCfmCnt;
   U32                       ulGbr;
   U32                       dlGbr;
   U32                       ulMbr;
   U32                       dlMbr;
   TknStrOSXL                *nasPdu;
   WrUmmRabArpInfo           arpInfo;
   WrUmmMsgCause             cause;           /* Added cause value for E-Rab Modify procedures */
   /* SPS changes starts */
   Bool                      isDlSpsEnabled;
   Bool                      isUlSpsEnabled;
   /* SPS changes ends */
   Bool                      isQciChanged;
} WrUmmRabModifyRabInfo;

/* The following definitions are for the field status in the structure    */
/* below.                                                                 */
#define WR_UMM_RAB_MODIFY_INITIATED      0
#define WR_UMM_RAB_MODIFY_SCHD_CFG_DONE  1
#define WR_UMM_RAB_MODIFY_STACK_CFG_DONE 2
#define WR_UMM_RAB_MODIFY_SCHD_DRX_CFG_DONE  3
#define WR_UMM_RAB_MODIFY_SCHD_CFG_RVRT_DONE 4
/** 
  * @brief This is a structure for RAB modify 
  * transaction. 
  * @details The structure members 
  * - U8                        transId          Transaction ID
  * - U32                       dlAmbr           Downlink AMBR value
  * - U32                       ulAmbr           Uplink AMBR  value
  * - U32                       state            State within transaction
  * - U32                       numRabs          Num. of RABs to be modify 
  * - WrUmmRabRelRabInfo        *rabs            substructure for each RAB Info
  * - Bool                      isDrbCfgDone     Flag to determine CFG state of DRB 
  * - Void                      *msg             Copy of Incoming message
  * - U8                        isUeAggrMaxBitRateRcvd
  *                             Flag to indication if AMBR is recvd in message
  * - U32                       cfgStateMsk      Bitmask to maintain Schedular msg state
 */
typedef struct wrUmmRabModifyTransCb
{
   U8                        transId;
   U32                       dlAmbr;
   U32                       ulAmbr;
   U32                       state;
   U32                       numRabs;
   WrUmmRabModifyRabInfo     *rabs;
   Void                      *msg;
   Bool                      isDrbCfgDone;
   Bool                      schdUndoCfgState;
   Bool                      rrmConfigRvrt;
   U8                        numRabsFound;
   WrUmmIncMsg               *rrmUeRecfgRspMsg;
   /* SPS changes starts */
   Bool                      isDlSpsEnabled;
   Bool                      isUlSpsEnabled;
   /* SPS changes ends */
   U8                        isUeAggrMaxBitRateRcvd;
   U32                       cfgStateMsk;
   U8                        lcgCfgStatus[WR_UMM_MAX_LCG_GRP_ID];
   U8                        expectedLcgCfgStatus;
   Bool                      allRabsFailed;
} WrUmmRabModifyTransCb;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_UMM_RABMOD_H__ */

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
