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
               

     File:     wr_umm_x2srcho.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#include "wr.h"
#include "wr_cmn.h"
#ifndef __WR_UMM_X2SRCHO_H__
#define __WR_UMM_X2SRCHO_H__

#ifdef __cplusplus
extern "C" {
#endif

#define WR_UMM_X2_SRC_HANDLE_HO_FAIL(_transCb)\
{\
   _transCb->ueCb->hoTransId = 0;\
   _transCb->ueCb->x2ConCb.oldEnbUeX2apId = 0;\
   _transCb->ueCb->x2ConCb.newEnbUeX2apId = 0;\
   _transCb->ueCb->x2ConCb.peerId = 0;\
   _transCb->ueCb->hoType = WR_HO_TYPE_NONE;\
   _transCb->ueCb->mobCtrlState = WR_HO_NORMAL;\
}

/**
 * @brief
 *    This structure contains RAB information
 *
 * @details
 *    These are the structure members
 * -U8              erabId                  E-Rab Id
 * -U8              rbId                    DRB ID
 * -U8              rbDir                   Direction Of Bearer
 * -U8              qci                     Quality Channel Indicator
 * -U8              tnlCfgState             Tunnel Configuration State
 * -U8              expectedTnlRsp          Expected Tunnel Response
 * -U8              dlFwding                Indicating DL Data Forwarding is
 *                                          available or not
 * -U8              peerAddr                Destination Address
 * -U8              datFwdUlRemTunId        UL Remote Tunnel ID
 * -U8              datFwdDlRemTunId        DL Remote Tunnel ID
 * -U8              datFwdUlLclTunId        UL Local Tunnel Id
 * -U8              datFwdDlLclTunId        Dl Local Tunnel Id
 */
typedef struct wrUmmX2SrcHoRabInfo
{
   U8                        erabId;
   U8                        rbId;
   U8                        rbDir;
   U8                        qci;
   U8                        tnlCfgState;
   U8                        expectedTnlRsp;
   Bool                      dlFwding;
   CmTptAddr                 peerAddr;
   U32                       datFwdUlRemTunId; 
   U32                       datFwdDlRemTunId; 
   U32                       datFwdUlLclTunId; 
   U32                       datFwdDlLclTunId; 
   Bool                      isULDatFwdReq; /* Set to TRUE only if Targe trequested for UL Data Forwarding */
} WrUmmX2SrcHoRabInfo;
/**
 * @brief
 *    This structure contains Source Handover Transaction information
 *
 * @details
 *    These are the structure members
 * -U32                           state                  Transaction State
 * -U32                           crnti                  crnti of UE
 * -U32                           cellId                 cellId
 * -U32                           peerId
 * -U32                           old_eNB_UE_X2ap_Id     Old eNB UE X2ap ID
 *                                                       Generated by source eNB
 * -U32                           new_eNB_UE_X2ap_Id     New eNB UE X2ap ID
 * -WrUmmMsgCause                 cztCause               Cause Value given by HO
 *                                                       Decision module.
 * -WrEcgi                        ecgi                   Cell Global Identity 
 * -U8                            numOfRbInfo            Num of Rab
 * -WrUmmX2SrcHoRabInfo           *rbInfo                RAB Information
 * -TknStrOSXL                    srcToTgtCntr           Source To Target
 *                                                       Container
 * -CztE_RAB_Lst                  *cztE_RAB_Lst          Not Admited List.
 * -CztTgeteNBtoSrc_eNBTprntCont  *tgetToSrcTprntCont    Tgt To Src container  
 * -WrUmmIncMsg                   *msg                   Incoming Message
 *  U8                            henbGwCtxtRelRequired  Flag to indicate
 *                                                       if GW Cntxt Rel Ind
 *                                                       IE needs to be included
 *                                                       in UE CTXT rel message
 *                                  
 */
typedef struct wrUmmX2SrcHoTrans
{
   U32                           state;
   U32                           crnti;
   U32                           cellId;
   U32                           peerId;
   U32                           old_eNB_UE_X2ap_Id;
   U32                           new_eNB_UE_X2ap_Id;
   WrUmmMsgCause                 cztCause;
   WrEcgi                        ecgi;
   /* ccpu00126532 */
   U16                           targetPci;
   U32                           targetEarfcnDl;
   U8                            enbType;
   U8                            numOfRbInfo;
   WrUmmX2SrcHoRabInfo           *rbInfo;
   TknStrOSXL                    srcToTgtCntr;
   CztE_RAB_Lst                  *cztE_RAB_Lst;
   CztTgeteNBtoSrc_eNBTprntCont  *tgetToSrcTprntCont;
   WrUmmIncMsg                   *msg;
   /* X2_HO Changes for Falcon */
   Buffer                        *hoCmd;
   /* ccpu00126532 */
   /* Key derived from base key and Target PCI and Taret DL EARFCN values */
   U8                            kEnbStar[WR_KENB_KEY_LEN];
   /*ccpu00128988*/
#ifdef TENB_AS_SECURITY
   Bool                          isKenbKeyGenerated;
   Bool                          isRrcContrPrepared;
#endif
   WrUmmIncMsg                   *rrcContrMsg;
   /* CSG_DEV start */
   U32                           csgId; /* CSG ID */
   U8                            csgCellAccessMode; /* CSG Access Mode */
   /* CSG_DEV end */
   U8                            henbGwCtxtRelRequired;
} WrUmmX2SrcHoTrans;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* end of __WR_UMM_X2SRCHO_H__ */


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
