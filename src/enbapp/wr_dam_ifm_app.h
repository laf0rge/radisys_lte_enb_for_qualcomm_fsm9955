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
			   -> Wrapper functions to functions defined in wr_dam.c
			   -> Wrapper functions to call pack functions defined in wr_ifm_dam_utis.c
                  for functions defined in APP 

     File:     wr_dami_ifm_app.h

     Sid:      $SID$ 

     Prg:      Suhas 

*********************************************************************21*/
#ifndef __WR_DAM_IFM_APP_H__
#define __WR_DAM_IFM_APP_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */
/* Functions called from utils file - start*/
EXTERN Void wrIfmDamWrCntrlReq(LwrMngmt    *cfg);
EXTERN Void wrIfmDamWrCfgReq(LwrMngmt *cfg);
EXTERN S16 wrIfmDamWrTnlCreatReq(U32 transId, WrDamTnlInfo *tnlInfo);
EXTERN Void wrIfmDamWrTnlModReq(U32 transId, WrDamTnlModInfo *tnlModInfo);
EXTERN Void wrIfmDamWrTnlDelReq(U32 transId, WrDamTnlId *tnlId);
EXTERN Void wrIfmDamWrUeDelReq(U32 transId, U16 cellId, U16 crnti);
EXTERN S16 wrIfmDamWrReCfgReq(U32 transId,U16 cellId,U16 crnti);
EXTERN Void wrIfmDamWrReestabReq(U32 transId, U16 cellId, U16 ocrnti, U16 ncrnti);
EXTERN Void wrIfmDamWrCellAddReq(U32 transId, WrDamCellInfo *cellInfo);
EXTERN Void wrIfmDamWrChngTnlState(WrDamTnlStInfo *tnlStInfo);
EXTERN Void wrIfmDamWrSetDataRcvdFlag(U16 cellId,U16 ueId);/*ccpu00138576*/
/* Functions called from utils file - end*/

/* Functions called from dam file - start*/
EXTERN S16 wrIfmDamWrCntrlCfm(CmStatus   *status,LwrMngmt    *cntrl);
EXTERN S16 wrIfmDamWrCfgCfm(LwrMngmt    *cfg,CmStatus   *cfgCfm);
EXTERN S16 wrIfmDamWrReestabCfm(U32 transId, U8 status);
EXTERN S16 wrIfmDamWrPdcpDatCfm(CmLtePdcpId pdcpId);
EXTERN S16 wrIfmDamWrTnlCreatCfm(U32 transId, U8 status, U32 lclTeid);
EXTERN S16 wrIfmDamWrTnlModCfm(U32 transId, U8 status, U32 lclTeId);
EXTERN S16 wrIfmDamWrUeDelCfm(U32 transId, U16 cellId, U16 crnti);
EXTERN Void wrIfmDamWrSendLmAlarm(U16  category, U16 event, U16 cause);
EXTERN Void wrIfmDamWrCtxtRel(U16  crnti, U16  cellId,U8 event );
/* Functions called from dam file - end*/
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_DAM_IFM_APP_H__ */

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
$SID$        ---      Suhas         1. TotaleNodeB GA4.0
*********************************************************************91*/
