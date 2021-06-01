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
               

     File:     wr_umm_initcntsetup.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_UMM_INITCNTSETUP_H__
#define __WR_UMM_INITCNTSETUP_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef enum 
{
  WR_INIT_CTXT_TNL_SETUP_NOT_SENT = 0, 
  WR_INIT_CTXT_TNL_SETUP_SENT,    
  WR_INIT_CTXT_TNL_SETUP_SUCC 
}WR_INIT_CTXT_TNL_SETUP;  


typedef enum 
{
  WR_INIT_CTXT_SCHD_CFG_NOT_SENT = 0,          
  WR_INIT_CTXT_SCHD_CFG_SENT,
  WR_INIT_CTXT_SCHD_CFM_FAIL,
  WR_INIT_CTXT_SCHD_CFM_SUCC
}WR_INIT_CTXT_SCHD;

/** 
 * @brief This structure contains the ERAB QoS information.
 *
 * @details These are the structure members
 * - U32  priority    Priority of ERAB
 * - U32  preempCap   Pre-emption capability of ERAB
 * - U32  preempVul   Pre-emption vulnerability of ERAB
 * - U32  dlMbr       DL MBR
 * - U32  ulMbr       UL MBR
 * - U32  dlGbr       DL Guarenteed Bit Rate
 * - U32  ulGbr       UL Guarenteed Bit Rate
 */
typedef struct wrErabQos
{
   Bool                      isGbrQosPres;
   U32                       priority;
   U32                       preempCap;
   U32                       preempVul;
   U32                       dlMbr;
   U32                       ulMbr;
   U32                       dlGbr;
   U32                       ulGbr;
} WrErabQos;

/** 
 * @brief This structure contains the information regarding the ERAB 
 *        that is to be setup
 *
 * @details These are the structure members
 * - U8          erabId         ERAB ID
 * - Bool        erabInit       Flag to indicate if ERAB is initialized
 * - Bool        rabAllowed     Flag to indicate if ERAB is admitted
 * - U8          reason         Reasion for non-admission
 * - U8          drbId          Data Radio Bearer ID
 * - U8          lchId          Logical Channel ID
 * - U32         qci            QCI
 * - U8          schdCfgState   Scheduler configuration state
 * - U32         remTeId        Remote Tunnel ID
 * - U32         lclTeId        Local Tunnel ID
 * - CmTptAddr   sgwAddr        Serving Gateway Tunnel ID
 * - TknStrOSXL  *nasPdu        Corresponding NAS PDU
 * - WrErabQos   qos            QoS Information
 * - Bool        tnlSetup       Flag to indicate if tunnel setup is done
 */
typedef struct wrErabInfo 
{
   U8                        erabId;
   Bool                      erabInit;
   Bool                      rabAllowed;
   U8                        reason;
   U8                        drbId;
   U8                        lchId;
   U32                       qci;
   U8                        schdCfgState;
   U32                       remTeId;
   U32                       lclTeId;
   CmTptAddr                 sgwAddr;
   TknStrOSXL                *nasPdu;
   WrErabQos                 qos;
   Bool                      tnlSetup;
   WrUmmMsgCause             cause; /* Added cause value for E-Rab procedures */
#ifdef WR_RSYS_KPI
   EpcTime    earbSetupReqTm; /*timestamp(ms) when Enb recevies earb setup req or initial context req from MME*/
   EpcTime    erabEstabTm;  /*timestamp(ms) when Enb sedn erab setup rsp*/
   /* SPS changes starts */
   Bool                      isDlSpsEnabled;
   Bool                      isUlSpsEnabled;
   /* SPS changes ends */
#endif/*WR_RSYS_OAM*/
} WrErabInfo;

/* The following macros are used for the state field in the transaction  */
/* control block.                                                        */
#define WR_UMM_INIT_CTXT_INITED        0
#define WR_UMM_INIT_CTXT_ERROR         1
#define WR_UMM_INIT_CTXT_UECAP_DONE    2
#define WR_UMM_INIT_CTXT_SEC_DONE      3
#define WR_UMM_INIT_CTXT_SCH_CFG_DONE  4
#define WR_UMM_INIT_CTXT_DAM_CFG_DONE  5
#define WR_UMM_INIT_CTXT_RRC_CFG_DONE  6
#define WR_UMM_INIT_CTXT_FAIL_SEND     7
#define WR_UMM_INIT_CTXT_SCH_DRX_CFG_DONE  8

/* SPS changes starts */
#define WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT 0x0f
/* SPS changes ends */

/** 
 * @brief This structure contains the Initial Context Setup Transaction
 *        information
 *
 * @details These are the structure members
 * - U8                 numErabSucc         Number of ERABs that are 
 *                                          successfully established
 * - U8                 numErabs            Number of ERABs to be setup
 * - U8                 numErabsAllowed     Number of ERABs that are admitted
 * - U32                dlAmbr              DL Agreegated Max Bit Rate
 * - U32                ulAmbr              UL Agreegated Max Bit Rate
 * - WrRegLai           regLai              registered LAI
 * - TknU32             csFallBackInd       CS Fallback indicator
 * - WrErabInfo         *erabList           ERAB to be setup info list
 * - WrUeSecCfg         *secCfg             UE Security related info
 * - WrUeCapInfo        *ueCap              UE Radio Capability Info
 * - CmTptAddr          *lclAddr            Source IP Address
 * - WrUmmIncMsg        *msg                Initial Context Setup PDU
 * - U8                 state               State of initial context setup
 *                                          transaction
 * - Bool               schdUeCfgCfmRcvd    Flag to indicate if confirm for UE 
 *                                          configuration of Scheduler is 
 *                                          received
 * - Bool               schdLchCfgCmpl      Flag to indicate if logical channel
 *                                          configuration of Scheduler for DRBs
 *                                          is complete
 * - Bool               schdSrb2LchCfgCmpl  Flag to indicate if logical 
 *                                          channel configuration of Scheduler
 *                                          for SRB2 is complete
 * - SztUERadioCapblty  *ueRadCapIe         UE Radio Capability Information
 *   U8                  failCauseType      Failure cause type which will be used
 *                                          while building the context setup fail
 */
typedef struct wrInitCntxtSetupTransCb
{
   U8                        numErabSucc;
   U8                        numErabFail;
   U8                        numErabs;
   U8                        numErabsAllowed;
   U32                       dlAmbr;
   U32                       ulAmbr;
   Bool                      updLai; /*Set to TRUE if regLai is present*/
   WrRegLai                  regLai;
   TknU32                    csFallBackInd;
   WrErabInfo                *erabList;
   WrUeSecCfg                *secCfg; /* Security related info */
#ifdef TENB_AS_SECURITY
   /* Fix for ccpu00126700 */
   WrUeSecCfg                *hzSecCfg; /* Security related info */
   Bool                      asKeyGenerated; /*AS key generated*/
   Bool                      asSecModeInit; /*TX of Security Mode command initiated*/
#endif
   WrUeCapInfo               *ueCap; /* Ue Capability Info */
   CmTptAddr                 *lclAddr;
   WrUmmIncMsg               *msg;
   U8                        state;
   Bool                      schdUeCfgCfmRcvd;
   Bool                      schdLchCfgCmpl;
   Bool                      schdSrb2LchCfgCmpl;
   Bool                      schdLcgCfgCmpl;
   /* ccpu00128203 */
   U8                        expctLcgCfgCfm;
   U8                        numlcgCfgCfm;
   SztUERadioCapblty         *ueRadCapIe;
   WrUmmMeasTransInfo        measTransCb;
   WrUmmIncMsg               *rrmUeRecfgRspMsg;
   WrGummei                  gummei;
   TknU32                    mmeUeS1apId2;
   U8                        failCause; /*ccpu00126907: Initial Context failure cause*/
   U8                        failCauseType;
   /* CSG_DEV */
   U8                        ueMbrShip;
} WrInitCntxtSetupTransCb;

typedef struct wrUmmTxModeChngTransCb
{
   WrUmmIncMsg         *msg;    
}WrUmmTxModeChngTransCb;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_UMM_INITCNTSETUP_H__ */



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
