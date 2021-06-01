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

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_dam_ifm_app.c

     Sid:      fc_dam_ifm_app.c@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:51:33 2014

     Prg:      Sriky 

*********************************************************************21*/

#include "wr_umm.h"
#include "wr_ifm_dam.h"
#include "wr_dam.h"
#include "wr_ifm_dam_utils.h"
#include "wr_dam_ifm_app.h"

/** @brief This function is used to configure the Data Application Module.
 *
 * @details
 *
 *     Function: wrIfmDamWrCntrlReq 
 *
 *         Processing steps:
 *         - This function is invoked by Stack manager to configre the DAM
 *           module 
 *         - This function invokes the function wrDamCntrlReq to process the
 *           control req.
 *
 * @param[in] cfg: Data Application Module Configuration.  
 * @return  Void 
 */
PUBLIC Void  wrIfmDamWrCntrlReq
(
LwrMngmt    *cfg
)
{
   TRC2(wrIfmDamWrCntrlReq);
   wrDamCntrlReq(cfg);
   RETVOID;
} /*wrIfmDamWrWrCntrlReq*/

/** @brief This function is used to configure the Data Application Module.
 *
 * @details
 *
 *     Function: wrIfmDamWrCfgReq 
 *
 *         Processing steps:
 *         - This function is invoked by Stack manager to configure the DAM
 *           module 
 *         - This function invokes the function wrIfmDamWrCfgReq to process the
 *           configuration Req.
 *
 * @param[in] cfg: Data Application Module Configuration.  
 * @return Void  
 */
PUBLIC Void wrIfmDamWrCfgReq
(
LwrMngmt *cfg
)
{
   TRC2(wrIfmDamWrCfgReq);
   wrDamCfgReq(cfg);
   RETVOID;
} /*wrIfmDamWrWrCfgReq*/

/** @brief This function is used to send the configure confirmation 
 *   the enbapp .
 *
 * @details
 *
 *     Function: wrIfmDamWrCfgCfm 
 *
 *         Processing steps:
 *         - This function is invoked the DAM  module 
 *         - This function invokes the pack function to send the 
 *           configuration Confirm.
 *
 * @param[in] cfg    : Data Application Module Configuration.  
 * @param[in] cfgCfm : Status of the configuration process.  
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamWrCfgCfm
(
LwrMngmt    *cfg,
CmStatus   *cfgCfm
)
{
   Pst  *pst;
   TRC2(wrIfmDamWrCfgCfm);
   pst = &wrDamCb.wrAppPst;

   RETVALUE(cmPkDamCfgCfm(pst,cfg,cfgCfm));
} /*wrIfmDamWrCfgCfm*/

/** @brief This function is used to send the control confirm back to the Application.
 *
 * @details
 *
 *     Function: wrIfmDamWrCntrlCfm 
 *
 *         Processing steps:
 *         - This function is invoked by the DAM module to send the confirm for the same. 
 *         - This function invokes the pack function for sending the confirm.
 *
 * @param[in] cfg: Data Application Module Configuration.  
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamWrCntrlCfm
(
CmStatus   *status,
LwrMngmt    *cntrl
)
{
   Pst  *pst;

   TRC2(wrIfmDamWrCntrlCfm);

   pst = &wrDamCb.wrAppPst;
   RETVALUE(cmPkDamCntrlCfm(pst,status,cntrl));

} /*wrIfmDamWrCntrlCfm*/

/** @brief This function is used to send UE Context release to Application. 
 *
 * @details
 *
 *     Function:wrIfmDamWrCtxtRel
 *
 *         Processing steps:
 *         - This function is invoked by the DAM module to send Context release to app. 
 *         - This function invokes the pack function for sending the conetxt release.
 *
 * @param[in] crnti: UE Identifier.  
 * @param[in] cellId: Cell ID.  
 * @param[in] event: Event causing context release.  
 * @return  Void 
 */
PUBLIC Void wrIfmDamWrCtxtRel
(
U16                    crnti,
U16                    cellId,
U8                     event 
)
{
   Pst  *pst;

   TRC2(wrIfmDamWrCtxtRel);
   pst = &wrDamCb.wrAppPst;

   cmPkDamSendCtxtRel(pst,crnti,cellId,event);
   RETVOID;
}

/*
 *       Fun:     wrSendLmAlarm
 *
 *       Desc:    Function trigger alarm
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  None
 */
PUBLIC Void wrIfmDamWrSendLmAlarm
(
U16                          category,
U16                          event,
U16                          cause
)
{
   Pst  *pst;

   TRC2(wrIfmDamWrSendLmAlarm);

   pst = &wrDamCb.wrAppPst;
   cmPkDamSendAlarmInd(pst,category,event,cause);
   RETVOID;

}/* wrIfmDamWrSendLmAlarm */


/** @brief This function is used to handle the Datacfm came from PDCP.
 *
 * @details
 *
 *     Function:wrIfmDamWrPdcpDatCfm 
 *
 *         Processing steps:
 *         - Function invoke by DAM Module to send the data confirm message.
 *
 * @param[in]  pdcpId: pointer to CmLtePdcpId structure
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16  wrIfmDamWrPdcpDatCfm
(
CmLtePdcpId pdcpId
)
{
   Pst *pst;
   TRC2(wrIfmDamWrPdcpDatCfm);
   pst = &wrDamCb.wrAppPst;
   RETVALUE(cmPkDamPdcpDatCfm (pst,pdcpId));

} /* end of  wrIfmDamWrPdcpDatCfm*/


/** @brief This function is used to an EGTP create tuunel.
 *
 * @details
 *
 *     Function: wrIfmDamWrTnlCreatReq
 *
 *         Processing steps:
 *         - This function is invoked by the UMM module to ceate a EGTP tunnel.
 *         - This function invokes the function wrDamTnlCreatReq to create
 *           tunnel.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]  tnlInfo: Tunnel Info which needs to be create.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamWrTnlCreatReq
(
U32                          transId,
WrDamTnlInfo                 *tnlInfo
)
{
#ifdef LCDAM
#else
   TRC2(wrIfmDamWrTnlCreatReq);
   wrDamTnlCreatReq(transId, tnlInfo);
   RETVALUE(ROK);
#endif
} /*wrIfmDamWrTnlCreatReq */

/** @brief This function informs the tunnel was ceated successfully with local
 *         teid or tunnel was not created to the UMM Module. 
 *
 * @details
 *
 *     Function: wrIfmDamWrTnlCreatCfm
 *
 *         Processing steps:
 *         - This function is process the tunnel creation conformation from DAM
 *           module.
 *         - This function invokes the UMM Module function wrUmmTnlCreatCfm to
 *           process the tunnel create conformation from DAM.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]   status: status of tunnel creation.
 * @param[in]  lclTeid: Local Tunnel ID.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamWrTnlCreatCfm
(
U32                          transId,
U8                           status,
U32                          lclTeid
)
{
   Pst *pst;
   TRC2(wrIfmDamWrTnlCreatCfm);

   pst = &wrDamCb.wrAppPst;
   RETVALUE(cmPkTnlCreatCfm (pst,transId, status, lclTeid));
} /*wrIfmDamWrTnlCreatCfm*/

/* ccpu00126336 */
/** @brief This function is used to modify a EGTP tunnel.
 *
 * @details
 *
 *     Function: wrIfmDamWrTnlModReq
 *
 *         Processing steps:
 *         - This function is invoked by the UMM module to ceate a EGTP tunnel.
 *         - This function invokes the function wrDamTnlModReq to create
 *           tunnel.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]  tnlInfo: Tunnel Info which needs to be created.
 * @return Void 
 */
PUBLIC Void wrIfmDamWrTnlModReq
(
U32                          transId,
WrDamTnlModInfo              *tnlModInfo
)
{
#ifdef LCDAM
#else
   TRC2(wrIfmDamWrTnlModReq);
   wrDamTnlModReq(transId, tnlModInfo);
   WR_FREE_DATA_APP_SHAREBLE_BUF(tnlModInfo, sizeof(WrDamTnlModInfo));
#endif
  RETVOID;
} /*wrIfmDamWrTnlModReq */

/* ccpu00126336 */
/** @brief This function informs the tunnel was updated successfully with local
 *         teid or tunnel was not created to the UMM Module. 
 *
 * @details
 *
 *     Function: wrIfmDamTnlModCfm
 *
 *         Processing steps:
 *         - This function processes the tunnel modification confirmation from DAM
 *           module.
 *         - This function invokes the UMM Module function wrUmmTnlCreatCfm to
 *           process the tunnel create confirmation from DAM.
 *
 * @param[in]  transId : Transaction ID. 
 * @param[in]  status  : status of tunnel creation.
 * @param[in]  lclTeid : Local Tunnel ID.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamWrTnlModCfm
(
U32                          transId,
U8                           status,
U32                          lclTeId
)
{
   Pst *pst;
   TRC2(wrIfmDamWrTnlModCfm);

   pst = &wrDamCb.wrAppPst;
   RETVALUE(cmPkTnlModCfm (pst,transId, status, lclTeId));

} /*wrIfmDamWrTnlModCfm*/

/** @brief This function will be called to send delete tunnel to the DAM.
 *
 * @details
 *
 *     Function:wrIfmDamWrTnlDelReq 
 *
 *         Processing steps:
 *         - Call the function wrDamTnlDelReq to delete the tunnel 
 *           in DAM Module.
 *         - This function is invoked by UMM Module to delete a particular tunnel
 *           information in DAM.
 *
 * @param[in]  transId : Transaction ID. 
 * @param[in]  tnlId   : Tunnel ID which is going to be delete.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC Void wrIfmDamWrTnlDelReq
(
U32                          transId,
WrDamTnlId                   *tnlId
)
{
#ifdef LCDAM
#else
   TRC2(wrIfmDamWrTnlDelReq);
   wrDamTnlDelReq(transId, tnlId);
   WR_FREE_DATA_APP_SHAREBLE_BUF(tnlId, sizeof(WrDamTnlId));
#endif
   RETVOID;
} /*wrIfmDamWrTnlDelReq*/

/** @brief This function is used to send the DAM Configuration to Delete the UE.
 *         
 * @details
 *
 *     Function: wrIfmDamWrUeDelReq
 *
 *         Processing steps:
 *         - This function will be invoked by unpack function.
 *         - Calls the function wrDamUeDelReq to send the UE Delete Req in DAM. 
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]  cellId : Cell ID required to retrieve the UE. 
 * @param[in]  crnti  : CRNTI Cell Specific UE identifier.
 * @return Void  
 */
PUBLIC Void wrIfmDamWrUeDelReq
(
U32                          transId,
U16                          cellId,
U16                          crnti
)
{
#ifdef LCDAM
#else
   TRC2(wrIfmDamWrUeDelReq);
   wrDamUeDelReq(transId, cellId, crnti);
#endif
   RETVOID;
} /*wrIfmDamWrUeDelReq*/

/** @brief This function is used to informs to the UMM Module the UE 
 * deletion was successful from Data  Application module.
 *
 * @details
 *
 *     Function: wrIfmDamWrUeDelCfm 
 *
 *         Processing steps:
 *         - This function is invoked by DAM module to inform the UE 
 *           deletaion was successful in DAM to the UMM.
 *         - Calls the UMM Module function wrUmmPrcDamUeDelCfm to process the
 *           delete conformation. 
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]  cellId: Cell ID required to retrieve the UE. 
 * @param[in]  crnti: CRNTI Cell Specific UE identifier.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamWrUeDelCfm
(
U32                          transId,
U16                          cellId,
U16                          crnti
)
{
#ifdef LCDAM
#else
   Pst *pst;
   TRC2(wrIfmDamWrUeDelCfm);
   pst = &wrDamCb.wrAppPst;
   RETVALUE(cmPkUeDelCfm(pst,transId, cellId,crnti));
#endif
  RETVALUE(ROK);
} /*wrIfmDamWrUeDelCfm*/

/** @brief This function validates the UE-ReConfiguration is possible or not in
 *         the Data Application Module.
 *
 * @details
 *
 *     Function: wrIfmDamWrReCfgReq 
 *
 *         Processing steps:
 *         - This function invokes the function wrDamReCfgReq.
 *         - This function is invoked by UMM Module during RRC connection 
 *           Reestablishment complete  processing.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]   cellId: Cell ID. 
 * @param[in]   crnti : CRNTI. 
 * @return  Void 
 */
PUBLIC S16 wrIfmDamWrReCfgReq
(
U32                          transId,
U16                          cellId,
U16                          crnti
)
{
#ifdef LCDAM
#else
   TRC2(wrIfmDamWrReCfgReq);
   wrDamReCfgReq(transId, cellId, crnti);
#endif
   RETVALUE(ROK);
} /*wrIfmDamWrReCfgReq*/

/** @brief This function validates the UE-Reestablishment is possible or not in
 *         the Data Application Module.
 *
 * @details
 *
 *     Function:wrIfmDamWrReestabReq
 *
 *         Processing steps:
 *         - This function invokes the function wrDamReestabReq for validating
 *           the new and old CRNTIs for this UE by the Data Application Module. 
 *         - This function is invoked by UMM Module during RRC connection 
 *           Reestablishment Req processing.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]   cellId: Cell ID. 
 * @param[in] oldcrnti: Old CRNTI.
 * @param[in] newcrnti: New CRNTI. 
 * @return Void  
 */
PUBLIC Void wrIfmDamWrReestabReq
(
U32                          transId,
U16                          cellId,
U16                          oldcrnti,
U16                          newcrnti
)
{
#ifdef LCDAM
#else
   TRC2(wrIfmDamWrReestabReq);
   wrDamReestabReq(transId, cellId, oldcrnti, newcrnti);
#endif
   RETVOID;
} /*wrIfmDamWrReestabReq */

/** @brief This function gets the UE Re-establishment Confirmation
 *         from Data Application Module. 
 *
 * @details
 *
 *     Function: wrIfmDamWrReestabCfm 
 *
 *         Processing steps:
 *         - This function invokes the function wrUmmPrcReestabCfm to validate
 *           the UE Re-establishment to handle -ve scenarios.
 *
 *
 * @param[in] transId: Transaction ID.  
 * @param[in]  status: Status of  
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamWrReestabCfm
(
U32                          transId,
U8                           status
)
{
#ifdef LCDAM
#else
   Pst *pst;
   TRC2(wrIfmDamWrReestabCfm);
   pst = &wrDamCb.wrAppPst;
   RETVALUE(cmPkReestabCfm(pst,transId, status));
#endif
   RETVALUE(ROK);
} /*wrIfmDamWrReestabCfm */

/** @brief This function informs the new cell information 
 *         to the Data Application Module during cell creation. 
 *         
 * @details
 *
 *     Function: wrIfmDamWrCellAddReq
 *
 *         Processing steps:
 *         - Invokes the function wrDamCellAddReq to add cell information to
 *           Data Application Module.
 *         - This function invoked by EMM Module during new cell adding
 *           to enodeB.
 *
 * @param[in] transId: Transaction ID.
 * @param[in] cellInfo: Cell Information. 
 * @return Void  
 */
PUBLIC Void wrIfmDamWrCellAddReq
(
U32                          transId,
WrDamCellInfo                *cellInfo
)
{
#ifdef LCDAM
#else
   TRC2(wrIfmDamWrCellAddReq);
   wrDamCellAddReq(transId, cellInfo);
   WR_FREE_DATA_APP_SHAREBLE_BUF(cellInfo, sizeof(WrDamCellInfo));
#endif
   RETVOID;
} /*wrIfmDamWrCellAddReq */

/** @brief This function forwards the state change request for a particular 
 *         tunnel to DAM.
 *         
 * @details
 *
 *     Function: wrIfmDamWrChngTnlState
 *
 *         Processing steps:
 *         - Invokes the function wrDamTnlStChngReq to process the request
 *
 * @param[in] tnlStInfo: State information for the tunnel.
 * @return  Void 
 */
PUBLIC Void wrIfmDamWrChngTnlState
(
WrDamTnlStInfo               *tnlStInfo
)
{
#ifdef LCDAM
#else
   TRC2(wrIfmDamWrChngTnlState);
   wrDamTnlStChngReq(tnlStInfo);
   WR_FREE_DATA_APP_SHAREBLE_BUF(tnlStInfo, sizeof(WrDamTnlStInfo));
#endif
   RETVOID;
} /*wrIfmDamWrChngTnlState*/


/** @brief This function forwards the S1-U error indication for a particular 
 *         tunnel to FSM.
 *         
 * @details
 *
 *     Function:wrIfmDamEgtpErrInd
 *
 *         Processing steps:
 *         - 
 *
 * @param[in] lclTeid: Local Teid.
 * @param[in] numDrbs: Drb count.
 * @param[in] ueId: crnti.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamEgtpErrInd
(
U32                          lclTeid,
U32                          numDrbs,
U16                          crnti 
)
{
#ifdef LCDAM
#else
    Pst        *pst;

    pst = &wrDamCb.wrAppPst;

    RETVALUE(cmPkEgtpErrInd(pst,lclTeid,numDrbs,crnti));
#endif
} /* end of wrIfmDamEgtpErrInd*/

/*ccpu00138576 - start*/
/** @brief This function is used to set the data rcvd flag in DAM.
 *         
 * @details
 *
 *     Function: wrIfmDamWrSetDataRcvdFlag
 *
 *         Processing steps:
 *         - This function will be invoked by unpack function.
 *         - Calls the function wrDamSetDataRcvdFlag in wr_dam.c. 
 *
 * @param[in]  cellId : Cell ID required to retrieve the UE. 
 * @param[in]  ueId   : UE identifier.
 * @return Void  
 */
PUBLIC Void wrIfmDamWrSetDataRcvdFlag
(
U16                          cellId,
U16                          ueId 
)
{
#ifdef LCDAM
#else
   TRC2(wrIfmDamWrSetDataRcvdFlag);
   wrDamSetDataRcvdFlag(cellId, ueId);
#endif
   RETVOID;
} /*end of wrIfmDamWrSetDataRcvdFlag*/
/*ccpu00138576 - end*/

/********************************************************************30**

           End of file:     fc_dam_ifm_app.c@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:51:33 2014

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
