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
               

     File:     wr_ifm_l1.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/


#ifndef __WR_IFM_L1_H__
#define __WR_IFM_L1_H__

#ifdef __cplusplus
extern "C" {
#endif
#define WR_MS_PHY_SHUTDOWN_CFM    0x01
#define WR_MS_TX_PWR_DOWN_CFM     0x02
#define WR_MS_TX_PWR_UP_CFM       0x04

#define WR_MS_PHY_PWR_DOWN_CFM    6
#define WR_MS_PHY_PWR_UP_CFM      7
#define IS_DUMMY_SHUTDOWN(a) (a%2)==0 ? TRUE : FALSE
#define WR_MS_CELL_STOP_CFM       8

/* UMM CTF Prototypes */
EXTERN S16 TfLiCtfBndCfm (Pst *pst, SuId suId, U8 status);
EXTERN S16 WrIfmL1BndCfm (Pst *pst, SuId suId, U8 status);
EXTERN S16 TfLiCtfCfgCfm (Pst *pst, SuId suId, CtfCfgTransId transId, 
                          U8 status);
EXTERN S16 WrIfmL1CfgCfm (Pst *pst, SuId suId, CtfCfgTransId transId, 
                          U8 status);
EXTERN S16 WrIfmL1EnbStopInd (Pst *pst, SuId suId, CtfCfgTransId transId);
EXTERN S16 TfLiCtfUeIdChgCfm (Pst *pst, SuId suId, CtfCfgTransId transId,
                              CtfUeInfo *ueInfo, U8 status);
/* CNM_DEV start */
EXTERN S16 TfLiCtfCnmInitialSyncRsp (Pst *pst, SuId suId, CtfCfgTransId *transId,
                              CtfCnmInitSyncRsp *cnmInitSyncRsp);
EXTERN S16 TfLiCtfCnmSyncInd (Pst *pst, SuId suId, CtfCfgTransId *transId,
                              CtfCnmCellSyncInd *cnmSyncInd);
EXTERN S16 TfLiCtfCnmSyncRsp (Pst *pst, SuId suId, CtfCfgTransId *transId,
                              CtfCnmCellSyncRsp *cnmSyncRsp);
/* CNM_DEV end */

EXTERN S16 WrIfmL1BndReq (Pst* pst, SuId suId, SpId spId);
EXTERN S16 WrIfmL1UbndReq (Pst* pst, SpId spId, Reason reason);
EXTERN S16 wrIfmPhyCellCfg (CtfCfgReqInfo *cellCfgReq, U32 transId);
EXTERN S16 wrIfmPhyUeCfg (CtfCfgReqInfo *ueCfg, U32 transId);
#ifdef TENB_AS_SECURITY
EXTERN S16 WrIfmL1KdfCfm (Pst *pst, SuId suId, CtfCfgTransId transId,CtfKdfCfmInfo *kdfCfmInfo,
                          U8 status);
EXTERN S16 wrIfmPhyKdfReq (CtfKdfReqInfo *info, U32 transid);
#endif

/* CNM_DEV start */
EXTERN S16 wrIfmPhyCnmInitialSyncReq (CtfCnmInitSyncReq *cnmInitSyncReq,
      U32 transId);
EXTERN S16 wrIfmPhyCnmCellSyncReq (CtfCnmCellSyncReq *cnmCellSyncReq,
      U32 transId);

EXTERN S16 WrIfmL1CnmInitialSyncRsp (Pst *pst, SuId suId, CtfCfgTransId *transId,
      CtfCnmInitSyncRsp *cnmInitSyncRsp);
EXTERN S16 WrIfmL1CnmSyncInd (Pst *pst, SuId suId, CtfCfgTransId *transId,
      CtfCnmCellSyncInd *cnmSyncInd);
EXTERN S16 WrIfmL1CnmSyncRsp (Pst *pst, SuId suId, CtfCfgTransId *transId,
      CtfCnmCellSyncRsp *cnmSyncRsp);
/* CNM_DEV end */

/* Periodic REM scan for TPM */
EXTERN S16 TfLiCtfPerCellSrchRsp (Pst *pst,SuId suId,CtfCfgTransId *transId,
CtfPeriodicRemCellSearchRsp  *perCellSrchRsp); 
/* Periodic REM scan for TPM End */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_IFM_L1_H__ */


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
