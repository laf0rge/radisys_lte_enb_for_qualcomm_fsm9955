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
               

     File:     wr_umm_rrcreestab.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_UMM_RRCREESTAB_H__
#define __WR_UMM_RRCREESTAB_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

#define WR_UMM_REESTAB_INITED          0
#define WR_UMM_REESTAB_REJECTED        1
#define WR_UMM_REESTAB_ACCEPTED        2
#define WR_UMM_REESTAB_CMPL_RCVD       3
#define WR_UMM_REESTAB_RRC_CFG_DONE    4
#define WR_UMM_REESTAB_DONE            5
#define WR_UMM_REESTAB_ERROR           6
#define WR_UMM_RRC_RESTB_CFG_FAIL      0x00
#define WR_UMM_RRC_RESTB_CFG_SUCC      0x01
#define WR_UMM_RRC_RESTB_SCH_CFG_DONE  0x02
#define WR_UMM_RRC_RESTB_L1_CFG_DONE   0x04
#define WR_UMM_RRC_RESTB_RRC_CFG_DONE  0x08
#define WR_UMM_RRC_RESTB_CFG_DONE      0x0F



#define WR_UMM_NEW_RNTI_RRC_UPDT            0x01
#define WR_UMM_NEW_RNTI_SCH_UPDT            0x02
#define WR_UMM_NEW_RNTI_PHY_UPDT            0x04
#define WR_UMM_NEW_RNTI_RRM_UPDT            0x08
#define WR_UMM_NEW_RNTI_UPDT_DONE      (WR_UMM_NEW_RNTI_RRC_UPDT | WR_UMM_NEW_RNTI_SCH_UPDT | \
                                        WR_UMM_NEW_RNTI_PHY_UPDT | WR_UMM_NEW_RNTI_RRM_UPDT)

/** 
 * @brief Structure declarations and definitions for 
 * RRC RE-ESTABLISH tranactions CB.
 *
 * @details These are the structure members
   -U8          state
   -U8          rabCfmReceived 
   -U8          rabSchCfg[WR_MAX_DRBS]
   -U16         newRnti - New CRNTI Value 
   -TknStrOSXL  *nasPdu 
   -Bool        UeschCfg - Ue Re-Configuration status
   -Bool        eRabSchCfg - eRAB Re-Configuration Status
   -Bool        reConfigState

 */
typedef struct wrRrcReEstabTransCb {
   U8            state;
   U8            rabCfmReceived;
   U8            rabSchCfg[WR_MAX_DRBS];
   U16           newRnti;
   TknStrOSXL    *nasPdu;
   Bool          UeschCfg;
   Bool          eRabSchCfg;
   Bool          reConfigState;
   U8            schdUeCfgState;
   U8            schdUeRstState;
   U8            curCfgRes;
   U8          reestabCause;
   U8          rejectCause; /* Cause for Rejecting Re-establishment */
   U8          phyCfgState;
   U8          ueRntiUpdtFlg;
#ifdef TENB_AS_SECURITY
   WrUeSecCfg    secCfg;
#endif
   WrUmmMeasGapStatus   measGapStatus;
   U8          isUeCtxtRelDefrd;
}WrRrcReEstabTransCb;

EXTERN S16 wrUmmRrcReestabSchUeReset(U32  transId,U16   cellId,
                      U16   oldRnti);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_UMM_RRCREESTAB_H__ */

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
