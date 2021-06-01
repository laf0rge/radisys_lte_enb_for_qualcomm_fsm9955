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
               

     File:     wr_umm_anrmeas.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_UMM_ANRMEAS_H__
#define __WR_UMM_ANRMEAS_H__

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

#define WR_UMM_ANR_MEAS_TRANS_INPROG   1
#define WR_UMM_ANR_MEAS_TRANS_DONE     2

/** @brief This Enum used for Anr Measurement Type.
 */
typedef enum wrUmmAnrMeasCfgInfo
{
  WR_UMM_NOT_CONFIGED = 1,
  WR_UMM_CONFIGED
}WrUmmAnrMeasCfgInfo;

/** @brief This structure used to store ANR measurement Transaction 
 * configuration in Transaction control block.

 -  U8          transId                 Transaction identifier
 -  U8                     transState Transaction state
 -  U32                    anrPrdRptCnt ANR Periodic Report count
 -  Bool                   isDrxReqForRptCgi Drx flag for Report CGI
 -  Bool                   isDrxCfged Is DRX configred
 -  WrUmmMeasRmvInfo       rmvMeasInfo remove List information
 -  WrUmmMeasAddInfo       addMeasInfo Add list information
 -  WrUmmMeasGapInfo       gapMeasInfo Gap related information 
 -  WrDrxCfg               ueDrxCfg Ue DRX Config 
 -  WrDrxCfg               *drxCfgPtr Pointer to DrxConfig  
 -  NhuDatReqSdus          rrcReCfgMsg  RRC reconfig Message Pointer
*/ 
typedef struct wrUmmAnrMeasTranInfo
{
    WrUmmMeasTransInfo       measTransCfg;
    U32                      anrPrdRptCnt; 
    Bool                     isDrxReqForRptCgi;
    Bool                     isDrxCfged;
    Bool                     isReCfgSent;
    Bool                     closeTrns;
    WrDrxCfg                 ueDrxCfg; 
    WrDrxCfg                 *drxCfgPtr; 
} WrUmmAnrMeasTransInfo;

#if 0
/*Removed can be deleted */
/*DRX QCI*/
/** @brief This Enum used for setting initial state of ANR 
       when ANR module receive the DRX QCI internal message
       from UMM module.
 */
typedef enum wrUmmEvntAnrInitialDrxQciState
{
  EVNT_INITIAL_NONE,
  NEW_TRANS_RLS_EVNT_ANR_MEAS_CFG,
  EVNT_WAIT_DRX_SCH_CFG_CFM,
  EVNT_WAIT_FOR_CGI_DRX_CFG_RRC_CMPL,
  EVNT_WAIT_FOR_CGI_RPT_FRM_UE,
  EVNT_WAIT_FOR_DRX_RLS_RRC_CMPL_ANR_SCH_CFGM
  
}WrUmmEvntAnrInitialDrxQciState;

/** @brief This Enum used for setting current state of ANR 
       during the ANR and DRX release procedure.
 */
typedef enum wrUmmEvntAnrDrxQciState
{
   EVNT_STATE_NONE,
   EVNT_MEAS_RLS_RRC_CFG_SENT_TO_UE,
   EVNT_DRX_RLS_SENT_TO_SCH,
   EVNT_MEAS_DRX_RLS_CFG_SENT_TO_UE,
   EVNT_DRX_RLS_RRC_CFG_SENT_TO_UE
   
}WrUmmEvntAnrDrxQciState;
#endif
/** @brief This structure used to store Event ANR measurement Transaction 
 *  configuration in Transaction control block.
 *
 * -  U8                     transId    Transaction identifier
 * -  U8                     transState Transaction state
 * -  Bool                   isDrxReqForRptCgi Drx flag for Report CGI
 * -  Bool                   isDrxCfged is DRX configured 
 * -  WrUmmMeasRmvInfo       rmvMeasInfo remove List information
 * -  WrUmmMeasAddInfo       addMeasInfo Add list information
 * -  WrUmmMeasGapInfo       gapMeasInfo Gap related information 
 * -  NhuDatReqSdus          rrcReCfgMsg  RRC reconfig Message Pointer
 * */ 
typedef struct wrUmmEventAnrTransInfo
{
   WrUmmMeasTransInfo       measTransCfg;
   Bool                     isDrxReqForRptCgi;
   Bool                     isDrxCfged;
   Bool                     isReCfgSent;
   Bool                     closeTrns;
   WrDrxCfg                 ueDrxCfg; 
   WrDrxCfg                 *drxCfgPtr; 
} WrUmmEventAnrTransInfo;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_UMM_ANRMEAS_H__ */

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
