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
               

     File:     wr_umm_rrc_msg.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_UMM_RRC_MSG_H__
#define __WR_UMM_RRC_MSG_H__

#include "wr.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

EXTERN S16 wrUmmRrcFillUeSrbInfo (WrUeRBInfo *srbCfg,U8  rabId);
EXTERN S16 wrUmmRrcFillConSetup (NhuDatRspSdus *nhuDatRspSdu,  WrUeCb *ueCb);
EXTERN S16 wrFillRrcSrbAddModLst (NhuRadioResourceConfigDedicated *radioResCfg,
                                           WrUeCb *ueCb, Ptr evnt, U8 srbId);
EXTERN S16 wrFillNhuUeCfgInfo (Ptr evnt, NhuUeCfgInfo *ueCfg, WrUeCb *ueCb,
                                                                 U8 msgType);
EXTERN S16 wrFillPdcpCfg (NhuPdcpCfg *pdcpCfg, U8 rbId, U8 cfmReqd,
                                                               WrUeCb *ueCb);
EXTERN S16 wrFillTrChCfg (NhuTrChCfg *trChCfg,WrUeCb  *ueCb);
EXTERN S16 wrUmmRrcFillSecCfg(NhuSecurityCfg *rrcSecCfg, WrUeSecCfg *secCfg);
EXTERN S16 wrUmmRrcFillSecModeCmd (U32 transId, WrUeSecCfg *secCfg, 
                                               NhuDatReqSdus *rrcDatReqSdu, U32 ueRlsVer);
EXTERN S16 wrFillRadResPhyCfgDed(NhuRadioResourceConfigDedicated *radioResCfg, 
                                            Ptr evnt, U8 rbId, WrUeCb *ueCb);
EXTERN S16 wrFillUeAddRbCfg (NhuRbCfgList **rbCfgList,WrUeCb *ueCb,U8 msgType);
EXTERN S16 wrUmmRrcCreateRrcReconfigMsg(NhuDatReqSdus *recfgMsg,
                                                             U8 recfgTransId);
EXTERN S16 wrUmmRrcPrepRadioResourceDed(
                                   NhuRadioResourceConfigDedicated *radioRes);
/* SPS changes starts */
EXTERN S16 wrUmmRrcReconfigFillDrb(WrUeCb *ueCb, Bool bIsUeSpsEnabled, NhuDRB_ToAddMod *drbIe, U8 erabId, 
                 U8 drbId, U8 lchId, U8 qci, Bool isDlSpsEnabled, Bool isUlSpsEnabled);

EXTERN Void wrUmmRrcReconfigFillulPbr(NhuDRB_ToAddMod *drbIe,U32 ulGbr,U32 qci,Bool isUlSpsEnabled);

/* SPS changes ends */
EXTERN S16 wrUmmRrcFillSrbIE(U16 cellId, NhuSRB_ToAddMod *srbIe, U8 isSrb1);
/* SPS changes starts */
EXTERN S16 wrUmmRrcAddDrbCfg(U16 cellId, NhuRbCfgInfo *drbCfg, U8 drbId, U8 lcId, U8 qci, Bool isUlSpsEnabled);
/* SPS changes ends */
EXTERN S16 wrUmmRrcFillUeCapEnq (NhuDatReqSdus *nhuDatReqSdu, 
                                  WrUmmRatLst *ratLst, U32 transId);
EXTERN S16 wrUmmRrcFillUeRadAccCapInfo (TknStrOSXL  *encodedInfo, 
                                       NhuEncReqSdus *encReq, WrUeCb *ueCb);
EXTERN S16 wrUmmRrcFillDlInfTfr(NhuDatReqSdus *dlInfoTfrPdu, 
                                             TknStrOSXL *nasPdu, U8 pdcpId);
EXTERN S16 wrUmmFillRrcEvntHdr(NhuHdr *nhuHdr, U16 cellId, U16 crnti, 
                                                               U32 transId);
EXTERN S16 wrUmmRrcBldConRls(WrUeCb *ueCb, U8 accessStratumRls, WrUmmRedirectInfo
      *redirectInfo, NhuDatReqSdus *sdu, U32 relCause, U32 t320InMin);

EXTERN S16 wrUmmRrcAddSrbCfg(U16 cellId, NhuRbCfgInfo *srbCfg, U8 isSrb1);
EXTERN S16 wrUmmRrcAddSrb1Cfg(Ptr evnt, NhuUeCfgInfo *ueCfg, WrUeCb *ueCb);
EXTERN S16 wrUmmFillRrcReEstabSetup(NhuDatRspSdus *nhuDatRspSdu, WrUeCb *ueCb);
EXTERN S16 wrUmmRrcDedNASInfo(NhuDatReqSdus *evnt, 
                     NhuRRCConRecfgn_r8_IEs *reCfgIes, TknStrOSXL *nasPdu);
EXTERN S16 wrUmmRrcBldReestabReject(NhuDatRspSdus *reestabRejSdu);
EXTERN S16 wrUmmRrcBldReestabMsg(NhuDatRspSdus *reestabSdu, WrUeCb *ueCb);
EXTERN S16 wrUmmRrcFillTrchCfg (NhuTrChCfg *trChCfg, WrUeCb *ueCb);
EXTERN S16 wrUmmRrcFillMacCfgIe ( NhuMAC_MainConfig  *macCfg, WrUeCb *ueCb);
   /* SPS changes starts */
EXTERN S16 wrUmmRrcFillDedRadioCfg(NhuRadioResourceConfigDedicated *ded,
                                          WrUeCb *ueCB,CmMemListCp *memCp, Bool isUeUlSpsEnabled);
   /* SPS changes ends */
EXTERN S16 wrUmmRrcFillSrcToTgetIe(WrUeCb *ueCb, NhuEncReqSdus *hoPrepMsgEnc);
EXTERN S16 wrUmmUtraRrcFillSrcToTgetIe(WrUeCb *ueCb, NhuEncReqSdus *hoPrepMsgEnc,
                                                  TknStrOSXL *utraUeCapContainer);
EXTERN Void wrUmmRrcFillUePhyCfgDedInfo(
                        NhuRadioResourceConfigDedicated  *rrcCfgDed,
                        RmuUeRecfgRsp                    *rrmUeRecfgResp);
EXTERN Void wrUmmRrcFillUeRadioResDedInfo(
                        /* SPS changes starts */
                        CmMemListCp                    *memCp,
                        WrUeCb                           *ueCb,
                        WrSpsCfg                       *pSpsCfg,
                        /* SPS changes ends */
                        NhuRadioResourceConfigDedicated  *rrcCfgDed,
                        RmuUeRecfgRsp *rrmUeRecfgResp);
EXTERN Void wrUmmRrcFillIntgAlgo(NhuSecAlgorithmConfig *secAlgCfg, WrUeCb *ueCb);
EXTERN S16 wrUmmRrcModDrbCfg(U16  cellId,NhuRbCfgInfo *drbCfg,U8  drbId,
                               U8  lcId,U8  qci, Bool isUlSpsEnabled);
EXTERN S16 wrUmmFillNhuUeCfgInfo(NhuUeCfgInfo  *ueCfg,WrUeCb *ueCb,U16   newCrnti);
EXTERN S16 wrUmmRrcFillConRej(NhuDatRspSdus *nhuDatRspEvntSdu,U32  waitTime);
EXTERN S16 wrUmmRrcFillSrcMeasCfgInfo(NhuMeasConfig *measCfg,WrUeCb  *ueCb,CmMemListCp *memCp);
EXTERN Void wrUmmRrcGetNhuMacMainCfgFrmReCfg( NhuDatReqSdus *rrcReCfgMsg,
                                    NhuMAC_MainConfig   **macCfg);
//EXTERN S16 wrUmmRrcFillRatContnr(CmMemListCp  *memCp,NhuUE_CapRAT_Cont  *rrcIe,
//                     TknStrOSXL *ueRatInfo,U32   ratType);
#ifdef TENB_AS_SECURITY
EXTERN S16 wrUmmFillNhuUeCfgInfo1 (NhuUeCfgInfo *ueCfg, WrUeCb *ueCb, U16 newCrnti);
#endif //TENB_AS_SECURITY

/* SPS changes starts */
PUBLIC S16 wrUmmRrcFillSpsCfgIe(CmMemListCp *memCp, U8 bSpsBitMap, U16 spsRnti, \
      RmuSpsConfigDl *pDlSpsCfg, RmuSpsConfigUl *pUlSpsCfg, NhuSPS_Config *spsCfgIe, U16 duplexMode);
/* SPS changes ends */
PUBLIC S16 wrUmmRrcReconfigFillModDrb(WrUeCb *ueCb, Bool isUeUlSpsEnabled, \
       NhuDRB_ToAddMod *drbIe, U8 erabId, U8 drbId, U8 lchId, U8 qci);
EXTERN S16 wrUmmFillNhuGeranRedirectInfo
(
 WrUmmRedirectInfo          *redirectInfo,
 NhuRRCConRls_r8_IEs         *conRlsR8,
 NhuDatReqSdus                *nhuDatReqSdu,
 U8                           accessStratumRls
);
EXTERN U32 wrGetRptInvlValFromEnum(U32 enumVal);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_UMM_RRC_MSG_H__ */

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
SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
