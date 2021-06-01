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
               

     File:     wr_umm_s1srcho.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_UMM_S1SRCHO_H__
#define __WR_UMM_S1SRCHO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "wr.h"
#include "wr_cmn.h"
#include "wr_umm_trans.h"
/*UE History info - start*/
#include "cm_pasn.h"
#include "cm_pasn.x"
/*UE History info - end*/

#define WR_UMM_S1_SRC_HANDLE_HO_FAIL(_transCb)\
{\
   _transCb->ueCb->hoTransId = 0;\
   _transCb->ueCb->hoType = WR_HO_TYPE_NONE;\
   _transCb->ueCb->mobCtrlState = WR_HO_NORMAL;\
}
/*SRVCC to GERAN changes start*/
#define WR_UMM_TLV_PK_TAGIE(_val1, _val2)\
{\
   _val2 = _val1;\
}

#define WR_UMM_TLV_PK_LI(_val1, _val2 )\
{\
   _val2 = _val1;\
}
#define WR_UMM_TLV_PK_RXLEV(_val1, _val2 )\
{\
   _val2 = _val1;\
}
#define WR_UMM_OLD_BSS_TO_NEW_BSS_IE 0x3A
#define WR_UMM_IRAT_HO_TGT_RAD_INFO 0x03
#define WR_UMM_LEN_IRAT 3
#define WR_UMM_LEN_IRAT_HO_TGT_RAD_INFO 1
/*SRVCC to GERAN changes ends*/
   
/* Function Prototypes */

EXTERN S16 wrUmmS1HoFillCause(SztCause *transCause, SztCause *cause);
EXTERN S16 wrUmmS1SrcHoOvrallTmrExp(WrUeCb  *ueCb);
EXTERN S16 wrUmmS1SrcHoFillPLMNId(S1apPdu   *pdu,WrPlmnId  *plmnId, SztPLMNidentity   *plmnIe);
EXTERN S16 wrEmmMmeSztFillLAC(U16 lac, SztLAC *lAC);


typedef struct wrUmmS1SrcHoRabInfo
{
   U8                        erabId;
   U8                        rbId;
   U8                        rbDir;
   U8                        qci;
   U8                        tnlCfgState;
   U8                        expectedTnlRsp;
   /* Indicates that Data forwarding is available or not */
   Bool                      dlFwding;
   /* Data Forwarding tunnel info */
   CmTptAddr                 peerAddr;
   U32                       locTunId;
   U32                       datFwdUlRemTunId; /* UL Local tunnel id */
   U32                       datFwdDlRemTunId; /* DL Local tunnel id */
   U32                       datFwdUlLclTunId; /* UL Local tunnel id */
   U32                       datFwdDlLclTunId; /* DL Local tunnel id */
   Bool                     isULDatFwdReq; /* Set to TRUE only if UL_GTP Tnl Info is rcvd in HovrCmd */
} WrUmmS1SrcHoRabInfo;
/* IRAT_HO */
typedef struct wrUmmIntraLteHoInfo
{
   U16                      eNBType;        /* Fill at the time of transaction creation.*/
   WrEcgi                   ecgi;
   U16                      tac;            /* Fill at the time of transaction creation.*/
   Bool                     isDirectPathAvl; /* Whether direct Path with the Tgt exists or not */
} WrUmmIntraLteHoInfo;

typedef struct wrUmmIRatGeranHoInfo
{
  Bool                          psSrvcNotAvlbl;  
  U8                            bsic;
  U16                           cId;         
  U16                           lac;         
  U8                            rac;         
  U8                            rxLev;
  U8                            bandIndicator;/* BandInc IE for MobilityFromUtraCmd */
} WrUmmIRatGeranInfo;

typedef struct wrUmmIRatUtraHoInfo
{
  U16                           psc;
  U16                           rncId;         
  U16                           cId;         
  U16                           lac;         
  U8                            rac;         
  TknStrOSXL                    utraUeCapContainer; 
} WrUmmIRatUtraInfo;

typedef struct wrUmmS1SrcHoTrans
{
  U8                  X2Presence; 
 U8                       state;
   U32                      crnti;          /* Filled at the time of transaction creation*/
   U32                      cellId;         /* Filled at the time of transaction creation.*/
#if 0
   /* Removing as these params are not used */ 
   U32                      mme_Ue_S1ap_Id;
   U32                      enb_Ue_S1ap_Id;
#endif
   U8                       numOfRbInfo;
   SztCause                 *sztCause;      /* Fill at the time of transaction creation.*/
   WrUmmMsgCause            *Cause;
   SztHovrTyp               *sztHovrTyp;    /* Fill at the time of transaction creation.*/
   WrPlmnId                 plmnId;         /* Fill at the time of transaction creation.*/
   WrUmmS1SrcHoRabInfo      *rbInfo;
   SztE_RABLst              sztE_RABLst;    /* Release List from MME.*/
   TknStrOSXL               srcToTgtCntr;
   TknStrOSXL               rrcCont;
   TknStrOSXL               *tgtToSrcCont;  /* Received From target eNB.*/
   WrUmmTimer               s1RelocTmr;/* Timer used at Source to reset L2 */
   WrUmmTimer               s1CancelTmr;
   Buffer                   *hoCmd;
   /* CSG_DEV start */
   U32                      csgId; /* CSG ID */
   U8                       csgCellAccessMode; /* CSG Access Mode */
   U16                      tgtPci;
   TknStrOSXL               nasSecParamFrmEutran;
   TknU32                   srvccHoInd; /* SRVCC handover presence flag for interrat and geran */
   /* CSG_DEV end */
   union {
     WrUmmIntraLteHoInfo intraLteInfo; /* Contains parameters specific to intra LTE HO */ 
     WrUmmIRatUtraInfo   interRatInfo; /* Contains parameters specific to inter RAT HO to UTRA */
     WrUmmIRatGeranInfo  interRatGeranInfo; /* Contains parameters specific to inter RAT HO to GERAN */
   }t;
} WrUmmS1SrcHoTrans;

EXTERN S16 wrUmmS1SrcHoOvrallTmrExp(WrUeCb  *ueCb);
EXTERN S16 wrUmmS1SrcHoRelocTmrExp(WrUeCb  *ueCb);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* end of __WR_UMM_S1SRCHO_H__ */



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
