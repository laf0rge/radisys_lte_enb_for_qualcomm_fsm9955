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
               

     File:     wr_umm.h

     Sid:      wr_umm_ecsfb_utils.h@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:53:28 2014

     Prg:      Sriky 

*********************************************************************21*/



#ifndef __WR_UMM_ECSFB_UTILS_H__
#define __WR_UMM_ECSFB_UTILS_H__

#include "wr.h"
#include "wr_cmn.h"
#include "wr_umm_meas.h"
#include "wr_umm_anrmeas.h"
#include "wr_umm_csfb.h"
#include "wr_lwr.h"
#include "wr_smm_smallcell.h"
#include "wr_utils.h"
#include "wr_ifm_s1ap.h"
#include "wr_umm_trans.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct wrUmmS1UlTnMsgParams
{
   U32                  cdmaType;
   TknStrOSXL           *mainSecId;
   Bool                 hoReqdInd;
   U8                   numOfPilotLst;
   WrCdmaRttPilotLst    **PilotList;
   TknStrOSXL           *ulCdmaInfo;
   TknStrBSXL           *meId;
}WrUmmS1UlTnlMsgParams; 

EXTERN S16 wrUmmBuildAndSndCdmaDlInfTfr(WrUmmTransCb *transCb, TknStrOSXL *cdmaPdu,U32 ratTyp);

EXTERN S16 wrUmmRrcFillCdmaDlInfTfr
(
NhuDatReqSdus                *dlInfoTfrPdu,
TknStrOSXL                   *cdmaPdu,
U32                           ratTyp,
U8                            pdcpId
);

EXTERN Bool wrUmmUtilGetUeDualRx( WrUeCb *ueCb,U32 ratType);
EXTERN Bool wrUmmUtilsIsEcsfbSupported(WrUeCb *ueCb);

EXTERN S16 wrUmmFillCdmaRedirectionInfo(NhuRedirectedCarrierInfo *redirInfoIe,WrUmmRedirectInfo *cdmaRedirectInf);  

EXTERN S16 wrUmmRrcFillHoFrmEutraPrepReqInfo
(
   NhuDatReqSdus        *nhuDatReqSdu,
   WrUeCb               *ueCb,
   U8                   cdmaType,
   Bool                 isConCurrHoHrpd,
   TknStrOSXL           *mobilityParams,
   Bool                 sndDualRxTxRedirectInd  /*RRC Rel10 Upgrade*/
);

EXTERN S16 wrUmmRrcFillMobFrmEutraCmd
(
   NhuDatReqSdus            *nhuDatReqSdu,
   TknStrOSXL               *cdmaS1Pdu,
   U32                      ratTyp
);


EXTERN WrUmmRedirectInfo* wrUmmFillCdmaRedirectInfo( WrUeCb *ueCb, wrCsfbRatType ratPriority);

EXTERN S16 wrFillSrvccInfoIe
(
WrUeCb                       *ueCb,
WrUmmS1UlTnlMsgParams        *ulTnlMsgParams,
SztCdma2000OneXSRVCCInfo     *ie,
S1apPdu                      *ulCdmaPdu
);

EXTERN S16 wrFillPilotLstIe
(
   U8                            cellId,
   SztCdma2000OneXSRVCCInfo      *ie,
   WrUmmS1UlTnlMsgParams         *ulTnlMsgParams,
   S1apPdu                       *ulCdmaPdu
);
EXTERN S16 wrUmmS1apBldUlCdmaPdu
(
WrUeCb                       *ueCb,
WrUmmS1UlTnlMsgParams        *ulTnlMsgParams,
S1apPdu                      **pdu
);

EXTERN S16  wrEmmGetHighestPrioFreq(U32 bandClass, U32 *arfcn, WrCellId cellId);
EXTERN S16  wrEmmGetHighestPrioBand(WrUeCb *ueCb, U32 *bandClass);
#if 0
EXTERN S16 wrEmmAnrNumCdmaCell(U8 cdmaType,WrCellId cellId);
#endif
EXTERN Bool wrUmmIs1xCdmaBandSupported(WrUeCb *ueCb,U32 *bandClass);
EXTERN S16 wrEmmGetRatPriority(WrPlmnId *plmn, WrRatPriorityList *ratPriority, WrCellId cellId);
EXTERN Bool wrEmmIsDualRxSupported(WrCellId cellId);

EXTERN S16 wrUmmHdlAndPrcRabMsg
(
WrUmmTransCb              *incTrans,
WrUmmMsgCause             *cause
);

EXTERN S16 wrFillEcsfbMainSecId
(
TknStrOSXL                   *ptr,
U16                          len,
Data                         *val,
CmMemListCp                  *mem
);
EXTERN  S16 wrUmmGet1xRttNeighCell
(
   WrUeCb              *ueCb,
   WrCdmaRttPilotLst   **cdma1xRttPilotLst,
   U8                  ratPriority
);
EXTERN S16 wrEmmIsCsfbOrEcsfbMeasSup
(
WrCellId                    cellId, 
Bool                        csfbOrEcsfbSup,
U8                          *csfbFlag
);

EXTERN S16 wrEmmGetCdma1xRttTgtCell(
TknStrOSXL    **sectorIdPtr,
WrCellId      cellId
);

EXTERN S16 wrUmmChkOneBandAndFreq
(
 WrUeCb           *ueCb
);
/*RRC Rel10 Upgrade - start*/
EXTERN Bool wrUmmUtilGetUeDualRxTx(WrUeCb *ueCb, U32 ratPriority);
EXTERN Bool wrUmmUtilGetUeEcsfbAndDualRxTxSuppCfgs(WrUeCb *ueCb);
EXTERN Bool wrEmmIsDualRxTxSupported(WrCellId cellId);
/*RRC Rel10 Upgrade - end*/
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_UMM_ECSFB_UTILS_H__ */



/********************************************************************30**

           End of file:     wr_umm_ecsfb_utils.h@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:53:28 2014

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
/main/1        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
