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
               

     File:     wr_ifm_dam.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="IFM";
static int RLOG_MODULE_ID=4;
static int RLOG_FILE_ID=19;


#include "wr_cmn.h"
#include "wr_ifm_dam.h"
#include "wr_ifm_dam_utils.h"
#include "wr_ifm_dam_utils.x"

/** @brief This function is used to configure the Data Application Module.
 *
 * @details
 *
 *     Function: wrIfmDamCntrlReq 
 *
 *         Processing steps:
 *         - This function invokes the function wrDamCntrlReq to process the
 *           control Req.
 *
 * @param[in] cfg: Data Application Module Configuration.  
 * @return S16 
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrIfmDamCntrlReq
(
LwrMngmt    *cfg
)
{
   Pst  *pst;
   pst = &wrCb.wrDamPst;

   RETVALUE(cmPkDamCntrlReq(pst,cfg));
} /* wrIfmDamCntrlReq */

/** @brief This function is used to configure the Data Application Module.
 *
 * @details
 *
 *     Function: wrIfmDamCfgReq 
 *
 *         Processing steps:
 *         - This function is invoked by Stack manager to configre the DAM
 *           module 
 *         - This function invokes the function wrDamCfgReq to process the
 *           configuration Req.
 *
 * @param[in] cfg: Data Application Module Configuration.  
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamCfgReq
(
LwrMngmt *cfg
)
{
   Pst *pst;
   pst = &wrCb.wrDamPst;

   RETVALUE(cmPkDamCfgReq(pst,cfg));

} /* wrIfmDamCfgReq */


/** @brief This function is used to configure the Data Application Module.
 *
 * @details
 *
 *     Function: wrIfmDamSendAlarmInd 
 *
 *         Processing steps:
 *         - This function invokes the function wrSendLmAlarm to process the
 *           Alarm Indication.
 *
 * @param[in] U16     category.  
 * @param[in] U16     event.  
 * @param[in] U16     cause.  
 * @return Void
 */
PUBLIC Void  wrIfmDamSendAlarmInd
(
U16     category,
U16     event,
U16     cause
)
{
   wrSendLmAlarm(category,event,cause);

} /*wrIfmDamSendAlarmInd*/


/** @brief This function is used to configure the Data Application Module.
 *
 * @details
 *
 *     Function: wrIfmDamCntrlCfm 
 *
 *         Processing steps:
 *         - This function is invoked by Stacke maager to configre the DAM
 *           module 
 *         - This function invokes the function wrDamCfg to process the
 *           configuration Req.
 *
 * @param[in] cfg: Data Application Module Configuration.  
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamCntrlCfm
(
LwrMngmt    *cntrl,
CmStatus   *cfmStatus
)
{
   Pst pst;
   cmMemset((U8*)&pst, 0, sizeof(Pst));
#ifdef TENB_T2K3K_SPECIFIC_CHANGES
   switch (cntrl->hdr.elmId.elmnt)
   { 
      case STWRGEN: 
      {
         switch (cntrl->t.cntrl.action)
         {
            case ASHUTDOWN:
            {
               cmMemcpy((U8*)&pst, (U8*)&(wrCb.init.lmPst), sizeof(Pst));
               pst.srcEnt    = wrCb.init.lmPst.dstEnt;
               pst.srcInst   = wrCb.init.lmPst.dstInst;
               pst.srcProcId = wrCb.init.lmPst.dstProcId;
               wrShutdown();
               break;
            }
            default :
            {
               break;
            }
         }
         break;
      }
      default :
      {
         break;
      }
   }
#endif
   wrGenerateCfm(NULLP,cfmStatus,TCNTRL,cntrl);
   RETVALUE(ROK);
} /*wrIfmDamCntrlCfm*/

/** @brief This function is used to configure the Data Application Module.
 *
 * @details
 *
 *     Function: wrIfmDamCfgCfm 
 *
 *         Processing steps:
 *         - This function is invoked by Stacke maager to configre the DAM
 *           module 
 *         - This function invokes the function wrDamCfg to process the
 *           configuration Req.
 *
 * @param[in] cfg: Data Application Module Configuration.  
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamCfgCfm
(
LwrMngmt  *cfg,
CmStatus *cfmStatus
)
{
   Pst *pst = NULLP;
   wrGenerateCfm(pst,cfmStatus,TCFG,cfg);
   RETVALUE(ROK);
} /* end of wrIfmDamCfgCfm*/


/* RLC_DL_MAX_RETX fix */
/** @brief This function is used to handle the Datcfm came from PDCP.
 *
 * @details
 *
 *     Function: wrIfmDamPdcpDatCfm
 *
 *         Processing steps:
 *
 * @param[in]  pdcpId: pointer to CmLtePdcpId structure
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16  wrIfmDamPdcpDatCfm
(
CmLtePdcpId pdcpId
)
{
   
   TRC2(wrIfmDamPdcpDatCfm);

#ifdef LCDAM
#else
   if (wrUmmPdcpDatCfm(pdcpId) != ROK)
   {
      /* Log error */
      RLOG0(L_ERROR, "wrIfmDamPdcpDatCfm: wrUmmSndCtxtRelReqn Failed");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
#endif
} /*wrIfmDamPdcpDatCfm*/


/** @brief This function is used to an EGTP create tuunel.
 *
 * @details
 *
 *     Function: wrIfmDamTnlCreatReq
 *
 *         Processing steps:
 *         - This function is invoked by the UMM module to ceate a EGTP tunnel.
 *         - This function invokes the function wrDamTnlCreatReq to create
 *           tunnel.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]  tnlInfo: Tunnel Info whhic needs to be create.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamTnlCreatReq
(
U32                          transId,
WrDamTnlInfo                 *tnlInfo
)
{
   Pst *pst;
   pst = &wrCb.wrDamPst;

   RETVALUE(cmPkTnlCreatReq(pst,transId,tnlInfo));
} /* end of wrIfmDamTnlCreatReq */

/** @brief This function is used to start UE timers at DAM module
 *
 * @details
 *
 *     Function: wrIfmDamStartInactivityTimer
 *
 *         Processing steps:
 *         - This function is invoked by the UMM module to start the inactivity timer.
 *         - This function invokes the function wrDamStartUeTmr to start
 *           timer.
 *
 * @param[in]  cellID : Cell ID. 
 * @param[in]  RNTI : RNTI 
 * @param[in]  Event :Type of the timer to start. 
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamStartInactivityTimer
(
 U16                         cellId,
 U16                         crnti
 )
{
#ifdef LCDAM
#else
    Pst *pst;
    pst = &wrCb.wrDamPst;
    RETVALUE(cmPkStartInactivityTimer(pst,cellId,crnti));
#endif
} /* end of wrIfmDamStartInactivityTimer*/

/** @brief This function informs the tunnel was ceated successfully with local
 *         teid or tunnel was not created to the UMM Module. 
 *
 * @details
 *
 *     Function: wrIfmDamTnlCreatCfm
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
PUBLIC S16 wrIfmDamTnlCreatCfm
(
U32                          transId,
U8                           status,
U32                          lclTeid
)
{
#ifdef LCDAM
#else
   wrUmmTnlCreatCfm(transId, status, lclTeid);
   RETVALUE(ROK);
#endif
} /* end of wrIfmDamTnlCreatCfm */

/* ccpu00126336 */
/** @brief This function is used to modify a EGTP tunnel.
 *
 * @details
 *
 *     Function: wrIfmDamTnlModReq
 *
 *         Processing steps:
 *         - This function is invoked by the UMM module to ceate a EGTP tunnel.
 *         - This function invokes the function wrDamTnlModReq to create
 *           tunnel.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]  tnlInfo: Tunnel Info whhic needs to be create.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamTnlModReq
(
U32                          transId,
WrDamTnlModInfo              *tnlModInfo
)
{
   Pst *pst;
   pst = &wrCb.wrDamPst;

   RETVALUE( cmPkTnlModReq (pst,transId,tnlModInfo));
} /* end of wrIfmDamTnlModReq */

/* ccpu00126336 */
/** @brief This function informs the tunnel was updated successfully with local
 *         teid or tunnel was not created to the UMM Module. 
 *
 * @details
 *
 *     Function: wrIfmDamTnlModCfm
 *
 *         Processing steps:
 *         - This function is process the tunnel modification confirmation from DAM
 *           module.
 *         - This function invokes the UMM Module function wrUmmTnlCreatCfm to
 *           process the tunnel create confirmation from DAM.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]   status: status of tunnel creation.
 * @param[in]  lclTeid: Local Tunnel ID.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamTnlModCfm
(
U32                          transId,
U8                           status,
U32                          lclTeId
)
{
#ifdef LCDAM
#else
   wrUmmTnlModCfm(transId, status, lclTeId);
   RETVALUE(ROK);
#endif
} /* end of wrIfmDamTnlModReq */

/** @brief This function will be caled to send delete tunnel to the DAM.
 *
 * @details
 *
 *     Function:wrIfmDamTnlDelReq 
 *
 *         Processing steps:
 *         - Call the function wrDamTnlDelReq to delete the tunnel 
 *           in DAM Module.
 *         - This function is invoked by UMM Module to delete a particular tunel
 *           information in DAM.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]    tnlId: Tunnel ID which is going to be delete.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamTnlDelReq
(
U32                          transId,
WrDamTnlId                   *tnlId
)
{
#ifdef LCDAM
#else
   Pst *pst;
   pst = &wrCb.wrDamPst;

   RETVALUE(cmPkTnlDelReq(pst,transId, tnlId));
#endif
} /* end of wrIfmDamTnlDelReq */

/** @brief This function is used to send the DAM Configuration to Delete the UE.
 *         information in DAM.
 * @details
 *
 *     Function: wrIfmDamUeDelReq
 *
 *         Processing steps:
 *         - This function will be invoked by UMM Module.
 *         - Calls the function wrDamUeDelReq to send the UE Delete Req in DAM. 
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]   cellId: Cell ID. 
 * @param[in]    crnti: CRNTI.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamUeDelReq
(
U32                          transId,
U16                          cellId,
U16                          crnti
)
{
#ifdef LCDAM
#else
   Pst *pst;
   pst = &wrCb.wrDamPst;

   RETVALUE(cmPkUeDelReq(pst,transId, cellId, crnti));
#endif
} /* end of wrIfmDamUeDelReq */

/** @brief This function is used to informs to the UMM Module the tunnel
 * deletion was successful from Data  Application module.
 *
 * @details
 *
 *     Function: wrIfmDamUeDelCfm 
 *
 *         Processing steps:
 *         - This function is invoked by DAM module to inform the tuunel
 *           deletaion was successful in DAM to the UMM.
 *         - Calls the UMM Module function wrUmmPrcDamUeDelCfm to process the
 *           delete conformation. 
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]   cellId: Cell ID. 
 * @param[in]    crnti: CRNTI.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamUeDelCfm
(
U32                          transId,
U16                          cellId,
U16                          crnti
)
{
#ifdef LCDAM
#else
   wrUmmPrcDamUeDelCfm(transId, cellId, crnti);
   RETVALUE(ROK);
#endif
} /* end of wrIfmDamUeDelCfm */

/** @brief This function validates the UE-ReConfiguration is possible or not in
 *         the Data Application Module.
 *
 * @details
 *
 *     Function: wrIfmDamReCfgReq 
 *
 *         Processing steps:
 *         - This function invokes the function wrDamReCfgReq.
 *         - This function is invoked by UMM Module during RRC connection 
 *           Reestablishment complete  processing.
 *
 * @param[in]  transId: Transaction ID. 
 * @param[in]   cellId: Cell ID. 
 * @param[in]   crnti : CRNTI. 
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamReCfgReq
(
U32                          transId,
U16                          cellId,
U16                          crnti
)
{
#ifdef LCDAM
#else
   Pst *pst;
   pst = &wrCb.wrDamPst;

   RETVALUE(cmPkReCfgReq(pst,transId, cellId, crnti));
#endif
} /* end of wrIfmDamReCfgReq */

/** @brief This function validates the UE-Reestablishmint is possible or not in
 *         the Data Application Module.
 *
 * @details
 *
 *     Function: wrIfmDamReestabReq 
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
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamReestabReq
(
U32                          transId,
U16                          cellId,
U16                          oldcrnti,
U16                          newcrnti
)
{
#ifdef LCDAM
#else
   Pst *pst;
   pst = &wrCb.wrDamPst;

   RETVALUE(cmPkReestabReq(pst,transId,cellId,oldcrnti,newcrnti));
#endif
} /* end of wrIfmDamReestabReq */

/** @brief This function gets the UE Re-establishment Conformation
 *         from Data Application Module. 
 *
 * @details
 *
 *     Function: wrIfmDamReestabCfm 
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
PUBLIC S16 wrIfmDamReestabCfm
(
U32                          transId,
U8                           status
)
{
#ifdef LCDAM
#else
   wrUmmPrcReestabCfm(transId, status);
   RETVALUE(ROK);
#endif
} /* end of wrIfmDamReestabCfm */

/** @brief This function informs the new cell information 
 *         to the Data Application Module during cell creation. 
 *         
 * @details
 *
 *     Function: wrIfmDamCellAddReq
 *
 *         Processing steps:
 *         - Invokes the function wrDamCellAddReq to add cell information to
 *           Data Application Module.
 *         - This function invoked by EMM Module during new cell adding
 *           to enodeB.
 *
 * @param[in] transId: Transaction ID.
 * @param[in] cellInfo: Cell Information. 
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamCellAddReq
(
U32                          transId,
WrDamCellInfo                *cellInfo
)
{
#ifdef LCDAM
#else
   Pst *pst;
   pst = &wrCb.wrDamPst;

   RETVALUE(cmPkCellAddReq(pst,transId,cellInfo));
#endif
} /* end of wrIfmDamCellAddReq */

/** @brief This function forwards the state chagne request for a particular 
 *         tunnel to DAM.
 *         
 * @details
 *
 *     Function: wrIfmDamTnlStChngReq
 *
 *         Processing steps:
 *         - Invokes the function wrDamTnlStChngReq to process the request
 *
 * @param[in] tnlStInfo: State information for the tunnel.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamChngTnlState
(
WrDamTnlStInfo               *tnlStInfo
)
{
#ifdef LCDAM
#else
    Pst *pst;
    pst = &wrCb.wrDamPst;

    RETVALUE(cmPkTnlStChngReq(pst,tnlStInfo));
#endif
} /* end of wrIfmDamCellAddReq */

/* HO_REEST */
/** @brief This function deletes the FWD tunnels and set the state chagne
 * request for particular drb
 *
 * @details
 *
 *     Function: wrIfmDamHdlHoFailure
 *
 *         Processing steps:
 *         - Invokes the function wrDamHdlHoFailure to process the request
 *
 * @param[in] tnlStInfo: State information for the tunnel.
 * @return S16
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamHdlHoFailure
(
U16                          cellId,
U16                          crnti
)
{
#ifdef LCDAM
#else
    Pst *pst;
    pst = &wrCb.wrDamPst;
    RETVALUE(cmPkHdlHoFailure(pst,cellId,crnti));
   RETVALUE(ROK);
#endif
} /* end of wrIfmDamHdlHoFailure */


/** @brief This function forwards the state chagne request for a particular 
 *         tunnel to DAM.
 *         
 * @details
 *
 *     Function:wrIfmDamStopUeTmr 
 *
 *         Processing steps:
 *         - 
 *
 * @param[in] tnlStInfo: State information for the tunnel.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamStopUeTmr
(
U16                          cellId,
U16                          ueId
)
{
#ifdef LCDAM
#else
    Pst *pst;
    pst = &wrCb.wrDamPst;

    RETVALUE(cmPkStopUeTmr(pst,cellId,ueId));
#endif
} /* end of wrIfmDamStopUeTmr*/

/*ccpu00138576 - start*/
/** @brief This function forwards the state chagne request for a particular 
 *         tunnel to DAM.
 *         
 * @details
 *
 *     Function:wrIfmDamSetDataRcvdFlag
 *
 *         Processing steps:
 *         - 
 *
 * @param[in] cellId.
 * @param[in] ueId.
 * @return S16 
 *        -# Success : ROK
 */
PUBLIC S16 wrIfmDamSetDataRcvdFlag
(
U16                          cellId,
U16                          ueId
)
{
#ifdef LCDAM
#else
    Pst *pst;
    pst = &wrCb.wrDamPst;

    RETVALUE(cmPkSetDataRcvdFlag(pst,cellId,ueId));
#endif
} /* end of wrIfmDamSetDataRcvdFlag*/
/*ccpu00138576 - end*/




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
