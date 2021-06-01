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
               

     File:     wr_umm_x2tgtho.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_UMM_X2TGTHO_H__
#define __WR_UMM_X2TGTHO_H__


#ifdef __cplusplus
extern "C" {
#endif


#include "wr.h"
#include "wr_cmn.h"
/* SPS changes starts */
#include "wr_umm_ho.h"
/* SPS changes ends */

#define INITIATING_MSG                           0
#define SUCCESSFUL_OUTCOME                       1
#define UN_SUCCESSFUL_OUTCOME                    2


#define WR_ALIGN_RRM_CAUSE_TO_X2AP(_rrmCause) {                          \
   switch(_rrmCause) {                                                            \
      case RMU_CAUSE_RADIO_RESOURCE_NOT_AVAIL:                                    \
      {                                                                           \
         _rrmCause =                                                              \
            CztCauseRadioNwno_radio_resources_available_in_target_cellEnum;       \
         break;                                                                   \
      }                                                                           \
      default:                                                                    \
         break;                                                                   \
   }                                                                              \
}

/*ccpu00129650*/   
EXTERN U16 wrUmmX2TgtHoInitUeCb(WrUeCb *ueCb);   
/**
 * @brief
 *    This structure contains Sn Status information
 *
 * @details
 *    These are the structure members
 *
 * -U8                 rbId                Rab Id
 * -NhuHoPdcpSduInfo   nhuHoPdcpSduInfo    Pdcp SDU Information
 */
typedef struct _wrUmmSnSts
{
  U8               rbId;
  NhuHoPdcpSduInfo nhuHoPdcpSduInfo;
} WrUmmSnSts;
/**
 * @brief
 *    This structure contains List of Sn Status information
 *
 * @details
 *    These are the structure members
 *
 * -U8                 numOfRb             no of Rab
 * -WrUmmSnSts         wrSnSts[8]          List of Sn Status Info
 */
typedef struct _wrUmmSnStsInfo
{
  U8                  numOfRb;
  WrUmmSnSts          wrSnSts[8];
} WrUmmSnStsInfo;

/**
 * @brief
 *    This structure contains handover rab information
 *
 * @details
 *    These are the structure members
 *
 * -U8                 rabAllowed             Indicates whether rab is allowed
 *                                            or not
 * -U8                 rlcMode                Mode of RLC
 * -U8                 lchId                  logical channel ID
 * -U8                 schdCfgState           Scheduler configuration State 
 * -U8                 tnlCfgState            Tunnel configuration State
 * -U8                 expectedTnlRsp         Expected Tunnel Response
 * -Bool               dlFwding               Indicates that DL Forwarding is
 *                                            available or not
 * -WrUeRAB            drbInfo                DRB information
 * -U32                datFwdUlLocEgtpTunId   UL Local Tunnel ID
 * -U32                datFwdDlLocEgtpTunId   Dl Local Tunnel ID
 */

typedef struct wrUmmX2TgtHoRabInfo
{
   U8                        rabAllowed;
   U8                        rlcMode;
   U8                        lchId;
   U8                        schdCfgState;
   U8                        tnlCfgState;
   U8                        expectedTnlRsp;
   Bool                      dlFwding;
   WrUeRAB                   drbInfo;
   U32                       datFwdUlLocEgtpTunId; 
   U32                       datFwdDlLocEgtpTunId; 
   CmTptAddr                 newSgwAddr; 
   U32                       newRemTnlId;
   /* HO_RLC_PDCP_CFG */
   /* PDCP RLC Configuration used at source */
   WrRabCfgParams            srcPdcpRlcCfgParams;
   /* SPS changes starts */
   Bool                     isDlSpsEnabled;
   Bool                     isUlSpsEnabled;
   /* SPS changes ends */
} WrUmmX2TgtHoRabInfo;
/**
 * @brief
 *    This structure contains handover rab information
 *
 * @details
 *    These are the structure members
 * -U32                       state                      Transaction State
 * -U32                       peerId                     Peer Id
 * -U8                        phyCfg                     Physical Configuration
 * -U8                        schdUeCfgCfmRcvd           Indicates that
 *                                                       scheduler UE config confirm is received or not
 * -U8                        schdSrb1LchCfgCmpl         Indicates that
 *                                                       scheduler SRB1 config confirm is received or not
 * -U8                        schdSrb2LchCfgCmpl         Indicates that
 *                                                       scheduler SRB2 config confirm is received or not
 * -U8                        schdLchCfgCmpl             Indicates that
 *                                                       scheduler LCH config confirm is received or not
 *  U8                        lcgCfgCmpl                 Indicates that
 *                                                       scheduler LCG config confirm is received or not
 *  U16                       crnti                      crnti of UE
 *  U32                       old_eNB_UE_X2ap_Id         Old eNB UE X2ap ID
 *  U32                       new_eNB_UE_X2ap_Id         New eNB UE X2ap ID
 *  U8                        noOfAdmitdBearers          no of Admitted bearers
 *  CztCause                  *cztCause                  cause value coming from
 *                                                       HO Decision Module
 *  U16                       pci                        PCI
 *  U32                       cellId                     CellId
 *  WrPlmnId                  plmnId                     PLMN ID
 *  WrMmeId                   mmeId                      mmeID
 *  U16                       tac                        Tracking Area Code
 *  WrGummei                  gummei                     GUMMEi
 *  U32                       mme_ue_s1ap_id             MME UE S1ap ID
 *  WrUeSecCfg                *secCfg                    Security Configuration
 *  U8                        numOfRbCfgd                no of RAB configured
 *  WrUmmX2TgtHoRabInfo       *rbInfo                    Radio Bearer Information
 *  WrBitRate                 ambr                       AMBR Value
 *  TknStrOSXL                kenb_Star                  KeNB Star Value
 *  U32                       nextHopChainingCount       Next HOP chaining Count
 *  U32                       new_nextHopChainingCount   After Receiving Path Switch Request Ack
 *  CztHovrRestrnLst          *hovrRestrnLst             Handover Resriction
 *                                                       Information
 *  WrUmmSnStsInfo            *snStsInfo                 Sn Status Information
 *  WrUmmIncMsg               *bufferedMsg1              Incoming Message
 *  WrUmmMeasTransInfo           measTransCb                Measurement Control
 *                                                       block
 *  CmTptAddr                 peerAddr                   peer address
 */
typedef struct wrUmmX2TgtHoTrans
{
   U32                       state;
   U32                       peerId;
   U8                        phyCfg;
   U8                        schdUeCfgCfmRcvd;
   U8                        schdSrb1LchCfgCmpl;
   U8                        schdSrb2LchCfgCmpl;
   U8                        schdLchCfgCmpl;
   U8                        lcgCfgCmpl;
   /* ccpu00128203 */
   U8                        numlcgCfgCfm;
   U8                        expctLcgCfgCfm;
   U16                       crnti;
   U32                       old_eNB_UE_X2ap_Id;
   U32                       new_eNB_UE_X2ap_Id;
   U8                        noOfAdmitdBearers;
   CztCause                  *cztCause;
   U16                       pci;
   U32                       cellId;
   WrPlmnId                  plmnId;
   WrMmeId                   mmeId;
   U16                       tac;
   WrGummei                  gummei;
   U32                       mme_ue_s1ap_id;/* MME UE S1AP ID */
   WrUeSecCfg                *secCfg;
/* ccpu00126532 */
#ifdef TENB_AS_SECURITY
   WrUeSecCfg                *hzSecCfg;
   Bool                      asKeyGenerated; /*AS key generated*/
   Bool                      asSecModeInit; /*TX of Security Mode command initiated*/
#endif
   U8                        numOfRbCfgd;
   WrUmmX2TgtHoRabInfo       *rbInfo; /* Radio Bearer Information.*/
   WrBitRate                 ambr;
   TknStrOSXL                kenb_Star;
   U32                       nextHopChainingCount;
   U32                       new_nextHopChainingCount;/* After Receiving Path Switch Request Ack*/
   CztHovrRestrnLst          *hovrRestrnLst;
#if 0
   WrTraceCb                 *traceCb;
#endif
   RmuUeRadioResCfg          ueRadioResCfg;
   WrUmmSnStsInfo            *snStsInfo;
   U8                        numBufferedMsg;
   WrUmmMeasTransInfo        measTransCb;
   CmTptAddr                 peerAddr; 
   WrUeCapInfo               *ueCap; /* Ue Capability Info */
   TknU32                    mmeUeS1apId2;
   U8                        snStatusInfoFlag;
   Bool                      isUeAttached;
   Bool                      handInUeCdma;
   WrUmmIncMsg               *rrmUeRecfgRspMsg;
   /* CSG_DEV start */
   U8                        ueMbrShpSts; /* UE Membership Status */
   U8                        isEmrgncyCall;
   /* CSG_DEV end */
   U8                        errCause;
   WrUmmMsgCause             hoPrepFailCause;
   /* SPS changes starts */
   WrUmmHoSpsInfo            srcToTgtSpsInfo;
   /* SPS changes ends */
   /*RRM_ARP*/
   WrUmmRabRls               erabsToRejectLst;
   WrUmmEutraHoTgtInfo       eutraTgtInfo;   /* RRC-REL10-Upgrade */
   Bool                      isRrcReEstRcvd;/*ccpu00141451: HO successful
                                            via RRC Re-establishment to Target*/
} WrUmmX2TgtHoTrans;



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* end of __WR_UMM_X2TGTHO_H__ */


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
