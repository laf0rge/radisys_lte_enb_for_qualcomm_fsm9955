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
               

     File:     wr_ifm_dam.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_IFM_DAM_H__
#define __WR_IFM_DAM_H__

#include "wr.h"
#include "wr_dam.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

#define EVTDAMCREATEEGTPTUNEL       1
#define EVTDAMCREATEEGTPTUNELCFM    2
#define EVDAMMODIFYEGTPTUNEL        3
#define EVTDAMDELEGTPTUNEL          4
#define EUTXXX 1

/** @def WR_DAM_CFG_OK 
 * This Macro defines the Configuration Status Success.
 */
#define WR_DAM_CFG_OK               0

/** @def WR_DAM_CFG_FAIL
 * This Macro defines the Configuration Status Fail.
 */
#define WR_DAM_CFG_FAIL             1

/* structure definitions to be used beween DAM and other modules */
   
/**
 *@brief This defines the EGTP TEID. 
 */
typedef U32                  WrEgtpTeid;

/**
 *@brief This structure contains the DAM Configuration. 
 *
 *@details These are the structure members
 * - U32      inactvTmrVal   Inactive Timer Value
 * - U32      endMrkTmrVal   End Marker Timer Value
 */
typedef struct wrDamCfg
{
   U32                       inactvTmrVal;
   U16                       expiryCnt; 
   U32                       endMrkTmrVal;
} WrDamCfg;

/**
 *@brief This structure contains the DAM Cell Info. 
 *
 *@details These are the structure members
 * - U16     cellId        Cell ID.
 * - U16     startRnti     Start RNTI.
 * - U16     maxRntis      Max RNTIs.
 */
typedef struct wrDamCellInfo
{
   U16                       cellId;
   U16                       startRnti;
   U16                       maxRntis;
} WrDamCellInfo;

/**
 *@brief This structure describes the types of the tunnel
 *
 *@details These are the values of enumerations
 * - WR_TNL_NORMAL         Normal Tunnel
 * - WR_TNL_DL_DATFWD      Downlink Data forwarding tunnel
 * - WR_TNL_UL_DATFWD      Uplink Data forwarding tunnel
 */


typedef enum wrTnlType{
      WR_TNL_NORMAL,
      WR_TNL_DL_DATFWD,
      WR_TNL_UL_DATFWD
} WrDamTnlType;

/**
 *@brief This structure contains the DAM Tunnel ID. 
 *
 *@details These are the structure members
 * - U16     cellId        Cell ID.
 * - U16     crnti         CRNTI.
 * - U8      drbId         DRB ID.
 * - WrDamTnlType tnlType  Type of the tunnel
 */
typedef struct wrDamTnlId
{
   U16                       cellId;
   U16                       crnti;
   U8                        drbId;
   WrDamTnlType              tnlType;
   U8                        tnlPriority;
} WrDamTnlId;

/**
 *@brief This structure contains the DAM Tunnel Information. 
 *
 *@details These are the structure members
 * - U32            transId      Transaction ID.
 * - WrDamTnlId     tnlId        Tunnel ID.
 * - U8             dscp         DSCP Value.
 * - WrDamTnlType   tnlType      Type of the tunnel
 * - CmTptAddr      dstAddr      Destination Addresses
 * - CmTptAddr      srcAddr      Source Address.
 * - WrEgtpTeid     remTeid      Remote TEID.
 * - Bool           isFullCfg    indicate the need for full configuration during HO
 */
typedef struct wrDamTunInfo
{
   U32                       transId;
   WrDamTnlId                tnlId;
   U8                        dscp;
   WrDamTnlType              tnlType;
   CmTptAddr                 dstAddr;
   CmTptAddr                 srcAddr;
   WrEgtpTeid                remTeid;
   Bool                      hoSrcEnodeb;
   WrHoType                  hoType;
   Bool                      isFullCfg;     /* RRC-REL10-Upgrade */
} WrDamTnlInfo;

/**
 *@brief This structure contains the DAM Tunnel Information for modifying a tunnel. 
 *
 *@details These are the structure members
 * - U32            transId      Transaction ID.
 * - WrDamTnlId     tnlId        Tunnel ID.
 * - CmTptAddr      newDstAddr      New Destination Addresses
 */
typedef struct wrDamTunModInfo
{
   U32                       transId;
   WrDamTnlId                tnlId;
   U32                       remTeid;
   CmTptAddr                 newDstAddr;
   U8                        dscp;     /*dscp value for the tunnel */
} WrDamTnlModInfo;

/**
 *@brief This structure contains the DAM Configuration. 
 *
 *@details These are the structure members
 * - U32      inactvTmrVal   Inactive Timer Value
 */
typedef struct wrDamTnlStInfo
{
   U32                 transId;
   U16                 state;
   U16                 cause;
   WrEgtpTeid          lclTeid;
} WrDamTnlStInfo;

/* Primitive definitions used between DAM and other modules               */
typedef S16 (*DamTnlCreatReq)
(
   U32                       transId,
   WrDamTnlInfo              *tnlInfo
);

typedef S16 (*DamTnlCreatCfm)
(
   U32                       transId,
   U8                        status,
   U32                       lclTeid
);

/* ccpu00126336 */
typedef S16 (*DamTnlModReq)
(
   U32                       transId,
   WrDamTnlModInfo           *tnlModInfo
);

typedef S16 (*DamTnlModCfm)
(
   U32                       transId,
   U8                        status,
   U32                       lclTeid
);

typedef S16 (*DamTnlDelReq)
(
   WrDamTnlId                locTeid
);

typedef S16 (DamUeDelReq)
(
   U32                       transId,
   U16                       cellId,
   U16                       crnti
);

/* Adding for handling the bind request */
EXTERN S16 WrIfmDatBndReq(Pst *pst, SuId suId, SpId spId);

EXTERN DamTnlCreatReq WrDamTnlCreatReq;

/* DAM initialization function which is called as part of system init     */
EXTERN S16 wrIfmDamWrCfg(WrDamCfg *cfg);
EXTERN S16 wrDamCfg(WrDamCfg *cfg);

/* DAM tunnel creation which is responsible for creating a eGTP tunnel    */
/* for a data bearer. The association between tunnel and DRB is maintained*/
/* as part of DAM.                                                        */
EXTERN S16 wrIfmDamWrTnlCreatReq(U32 transId, WrDamTnlInfo *tnlInfo);
EXTERN S16 wrIfmDamTnlCreatReq(U32 transId, WrDamTnlInfo *tnlInfo);
EXTERN Void  wrDamTnlCreatReq(U32 transId, WrDamTnlInfo *tnlInfo);
EXTERN S16 wrIfmDamTnlCreatCfm(U32 transId, U8 status, U32 lclTeid);
EXTERN S16 wrUmmTnlCreatCfm(U32 transId, U8 status, U32 lclTeid);
EXTERN Void  wrIfmDamWrCntrlReq(LwrMngmt    *cfg);
EXTERN Void wrIfmDamWrCfgReq(LwrMngmt *cfg);

EXTERN Void  wrIfmDamSendAlarmInd(U16     category,U16     event,U16     cause);
EXTERN S16 wrIfmDamCfgReq(LwrMngmt *cfg);
EXTERN S16 wrIfmDamCfgCfm(LwrMngmt  *cfg,CmStatus *cfmStatus);
EXTERN S16 wrIfmDamCntrlReq(LwrMngmt    *cfg);
EXTERN S16 wrIfmDamCntrlCfm(LwrMngmt    *cntrl,CmStatus   *cfmStatus);
EXTERN S16 wrIfmDamTnlModCfm(U32 transId,U8  status,U32  lclTeId);
EXTERN S16 wrIfmDamReestabCfm(U32  transId,U8  status);
EXTERN S16  wrIfmDamPdcpDatCfm(CmLtePdcpId pdcpId);

EXTERN Void wrDamTnlDelReq(U32  transId, WrDamTnlId  *tnlId);
EXTERN S16 wrDamStartUeTmr(U16 cellId,U16 crnti); 

/* ccpu00126336 */
/* Tunnel modification primitive to modify the tunnels                    */
EXTERN Void wrIfmDamWrTnlModReq(U32 transId, WrDamTnlModInfo *tnlModInfo);
EXTERN S16 wrIfmDamTnlModReq(U32 transId, WrDamTnlModInfo *tnlModInfo);
EXTERN S16 wrDamTnlModReq(U32 transId, WrDamTnlModInfo *tnlModInfo);
EXTERN S16 wrUmmTnlModCfm(U32 transId, U8 status, U32 lclTeId);

/* HO_REEST */
EXTERN S16 wrIfmDamWrHdlHoFailure(U16 cellId, U16 crnti);
EXTERN S16 wrIfmDamHdlHoFailure(U16 cellId, U16 crnti);
EXTERN S16 wrDamHdlHoFailure(U16 cellId, U16 crnti);

/* Tunnel deletion primitive to delete the tunnels upon bearer release    */
/* No confirmation is provided for deletion request                      */
EXTERN Void wrIfmDamWrTnlDelReq(U32 transId, WrDamTnlId *tnlId);
EXTERN S16 wrIfmDamTnlDelReq(U32 transId, WrDamTnlId *tnlId);

/* Tunnel state change primitive                                          */
EXTERN Void wrIfmDamWrChngTnlState(WrDamTnlStInfo *tnlStInfo);
EXTERN S16 wrIfmDamChngTnlState(WrDamTnlStInfo *tnlStInfo);
EXTERN S16 wrDamTnlStChngReq(WrDamTnlStInfo *tnlStInfo);

/* Primitive to start end marker timer                                    */
EXTERN S16 wrDamStartEndMrkrTmr(U16 cellId, U16 ueId);

EXTERN S16 wrUmmPrcEgtpErrInd(U32 lclTeid, U32 numDrbs, U16 crnti);

/* Cell addition request primitive for system bring up*/
EXTERN Void wrIfmDamWrCellAddReq(U32 transId, WrDamCellInfo *cellInfo);
EXTERN S16 wrIfmDamCellAddReq(U32 transId, WrDamCellInfo *cellInfo);
EXTERN S16 wrDamCellAddReq(U32 transId, WrDamCellInfo *cellInfo);

/* UE deletion request primitive when a call tear down is performed       */
EXTERN Void wrIfmDamWrUeDelReq(U32 transId, U16 cellId, U16 crnti);
EXTERN S16 wrIfmDamUeDelReq(U32 transId, U16 cellId, U16 crnti);
EXTERN Void  wrDamUeDelReq(U32 transId, U16 cellId, U16 crnti);
EXTERN S16 wrIfmDamUeDelCfm(U32 transId, U16 cellId, U16 crnti);
EXTERN S16 wrUmmPrcDamUeDelCfm(U32 transId, U16 cellId, U16 crnti);

/* UE ID change primitives to be used during reestablishment              */
EXTERN S16 wrIfmDamWrUeIdChgReq(U32 transId, U16 cellId, U16 oCrnti, U16 nCrnti);
EXTERN S16 wrDamUeIdChgReq(U32 transId, U16 cellId, U16 oCrnti, U16 nCrnti);
EXTERN S16 wrIfmDamWrUeIdChgCfm(U32 transId, U8 status);
EXTERN S16 wrUmmPrcDamUeIdChgCfm(U32 transId, U8 status);

EXTERN Void wrIfmDamWrReestabReq(U32 transId, U16 cellId, U16 ocrnti, U16 ncrnti);
EXTERN S16 wrIfmDamReestabReq(U32 transId, U16 cellId, U16 ocrnti, U16 ncrnti);
/* RLC_DL_MAX_RETX fix */
EXTERN S16 wrIfmDamWrReCfgReq(U32 transId,U16 cellId,U16 crnti);
EXTERN S16 wrIfmDamReCfgReq(U32 transId,U16 cellId,U16 crnti);
EXTERN S16 wrIfmDamStopUeTmr(U16 cellId,U16 ueId);
EXTERN S16 wrDamStopUeTmr (U16 cellId,U16 ueId);
EXTERN S16 wrIfmDamStartInactivityTimer(U16 cellId,U16 crnti);
EXTERN S16 wrIfmDamSetDataRcvdFlag(U16 cellId, U16 ueId);/*ccpu00138576*/


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_IFM_DAM_H__ */

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
