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
               

     File:     wr_umm_s1tgtho.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_UMM_S1TGTHO_H__
#define __WR_UMM_S1TGTHO_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wrUmmS1TgtHoRabInfo
{
   U8                        rabAllowed;
   U8                        rlcMode;
   U8                        lchId;
   U8                        schdCfgState;
   U8                        tnlCfgState;
   U8                        expectedTnlRsp;
   /* Indicates that Data forwarding is available or not */
   Bool                      dlFwding;
   WrUeRAB                   drbInfo;
   /* Data Forwarding tunnel info */
   U32                       datFwdUlLocEgtpTunId; /* UL Local tunnel id */
   U32                       datFwdDlLocEgtpTunId; /* DL Local tunnel id */
   /* HO_RLC_PDCP_CFG */
   /* PDCP RLC Configuration used at Source */
   WrRabCfgParams            srcPdcpRlcCfgParams;
   /* SPS changes starts */
   Bool                      isDlSpsEnabled;
   Bool                      isUlSpsEnabled;
   /* SPS changes ends */
} WrUmmS1TgtHoRabInfo;

/* IRAT_HO */
typedef struct wrUmmIRatHoTgtInfo
{
  TknStrOSXL  nasSecParamToEutran;
} WrUmmIRatHoTgtInfo;

typedef struct wrUmmS1TgtHoTrans
{
  U32                        state;
  U8                         hovrPrepFailState;
  U8                         hovrFailFlag;
  U16                        crnti;
  U8                         phyCfg;
  U8                         schdUeCfgCfmRcvd;
  U8                         schdSrb1LchCfgCmpl;
  U8                         schdSrb2LchCfgCmpl;
  U8                         schdLchCfgCmpl;
  /* ccpu00128203 */
  U8                         lcgCfgCmpl;
  U8                         expctLcgCfgCfm;
  U8                         numlcgCfgCfm;
  U8                         noOfAdmitdBearers;
  SztCause                   *sztCause;
  U32                        cellId;
  WrPlmnId                   plmnId; /* Need to be changed WrPlmn Structure(3 Octet String)*/
  SztGUMMEI                  *sztGUMMEI;
  U32                        mme_ue_s1ap_id;/* MME UE S1AP ID */
  U32                        enb_ue_s1ap_id;/* Generated By Target eNB.*/
  U32                        sztHovrTyp;
  U8                         numOfRbCfgd;
  WrBitRate                  ambr;
  WrUeSecCfg                 *secCfg;
/* ccpu00126532 */
#ifdef TENB_AS_SECURITY
  WrUeSecCfg                 *hzSecCfg;
  Bool                       asKeyGenerated; /* to check if AS keys are
                                                generated */
  Bool                       isNewKenbGenerated; /* to check if kEnb generated
                                                    from the key from MME */
  Bool                       asSecModeInit; /*TX of Security Mode command initiated*/
#endif
  WrUmmS1TgtHoRabInfo        *rbInfo; /* Radio Bearer Information.*/
  TknBStr32                  *kenb_Star;
  U32                        nextHopChainingCount;
  TknStrOSXL                 nextHop_NH;
  U32                        new_nextHopChainingCount;/* MME Shall give the details.*/
  SztUE_HistoryInform        *ueHistoryInfo;
  SztHovrRestrnLst           *hovrRestrnLst;
  TknStrOSXL                 *srcToTgtCont; /* Received From Source ENB.*/
#if 0
  WrTraceCb                  *traceCb;
#endif
  RmuUeRadioResCfg           ueRadioResCfg;
  WrUmmSnStsInfo             *snStsInfo;
  WrUmmMeasTransInfo         measTransCb;
  U16                        l1CfmRcvd;
  U16                        damCfmRcvd;
  U16                        schdCfmRcvd;
  WrUeCapInfo                *ueCap; /* Ue Capability Info */
  U8                         snStatusInfoFlag;
  Bool                       isUeAttached;
  Bool                       handInUeCdma;
  U8                         locReptReqTyp;
  WrGummei                   gummei;
  TknU32                     mmeUeS1apId2;
  U8                         numBufferedMsg;
  /* ccpu00131426 - Fix */
  WrUmmIncMsg               *rrmUeRecfgRspMsg;
  /* CSG_DEV start */
  TknU32                    csgId; /* CSG ID */
  U8                        ueMbrShpSts; /* UE Membership Status */
  U8                        isEmrgncyCall;
  /* CSG_DEV end */
  /*ccpu00130428*/
  WrUmmMsgCause             hoPrepFailCause;
  /* SPS changes starts */
  WrUmmHoSpsInfo            srcToTgtSpsInfo;
  Bool                      isRrcReEstRcvd;/*ccpu00141451: HO successful
                                            via RRC Re-establishment to Target*/ 
  /* SPS changes ends */
  /*RRM_ARP*/
  WrUmmRabRls               erabsToRejectLst;
/* IRAT_HO */
  union 
  {
     WrUmmIRatHoTgtInfo     iRatHoInfo; /*Parameters specific to IRAT HO*/
     WrUmmEutraHoTgtInfo    eutraTgtInfo;   /* RRC-REL10-Upgrade */
  }u;
} WrUmmS1TgtHoTrans;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* end of __WR_UMM_S1TGTHO_H__ */



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
