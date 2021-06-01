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
               

     File:     wr_dam.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="DAM";


static int RLOG_MODULE_ID=16;

static int RLOG_FILE_ID=1;
#include "wr.h"
#include "wr_lwr.h"
#include "wr_umm.h"
#include "wr_dam.h"
#include "wr_ifm_egtp.h"
#include "wr_ifm_pdcp.h"
#include "wr_ifm_dam.h"
#include "wr_umm.h"
#include "wr_kpi.h"
#ifdef SS_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif
#include "wr_smm_init.h"
#include "wr_dam_ifm_app.h"/*ccpu00138576*/
 
void DumpRLCDlDebugInformation(void);
void DumpRLCUlDebugInformation(void);
void DumpPDCPDlDebugInformation(void);
void DumpPDCPUlDebugInformation(void);
PRIVATE S16 wrDamLSapCfg(LwrMngmt *cfg, CmStatus *status);
PRIVATE S16 wrDamLSapCntrl(LwrCntrl *sapCntrl,CmStatus *status,Elmnt elmnt);
PRIVATE S16 wrDamBndLSap (WrLiSapCb *sapCb,CmStatus  *status,Elmnt elmnt);
PRIVATE S16 wrDamUbndLSap (WrLiSapCb  *sapCb);
PUBLIC S16 egFpTunProcEguDatReq(U32 lclTeid, U32 remTeid, U8 tos, U32 tPduSize, Buffer* mBuf);
PRIVATE Void  wrDamDelTunnel(WrCellId cellId,U16 rnti,U8 rbId,WrDamTnlType tnlType);
/* Some basic default values used for E-GTP Tunnel Management*/
/* Default Interface type while adding the tunnel at E-GTP*/
#define EGU_DFLT_INTFTYPE    0
/* Default expected sequence number for E-GTP tunnel used for initilization*/
#define EGU_DFLT_EXPSEQNMB   1000
/* Default Maximum Transmit Pdu Size*/
#define EGU_DFLT_MAXTPDUSZ   5000
/* Default EGTP Message size */
#define WR_DAM_EGTP_MSG_SZ   1024
/* Default Interface type while adding the tunnel at GTP*/
#define WR_DAM_MAX_DRB_TNLS  3

#define WR_DAM_VALID_RNTI(_cell, _rnti) \
 ((_rnti >= _cell->startRnti) && (_rnti < _cell->numRntis + _cell->startRnti))


PUBLIC WrDamCb   wrDamCb;
U32 droppedIcpuPkts = 0;
U32 totalIcpuPkts   = 0;
void DumpRLCDlDebugInformation(void)
{
  RETVOID;
}
void DumpRLCUlDebugInformation(void)
{

  RETVOID;
}
void DumpPDCPDlDebugInformation(void)
{
  RETVOID;
}
void DumpPDCPUlDebugInformation(void)
{
  RETVOID;
}

/** @brief This function is responsible for initializing the free and in use
 *         list for the UE Idexes
 *
 * @details
 *
 *     Function: wrDamInitUeIdxs
 *
 *         Processing steps:
 *         - initialize free list
 *         - initialize in-use list
 *         - add 'numIdx' indexes to the free list
 *
 * @param[in] cellCb  : Cell Control Block
 * @param[in] numIdxs : Number of Indexes
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrDamInitUeIdxs
(
WrDamCellCb                  *cellCb,
U16                          numIdxs
)
{
   U32                       idx;
   WrDamUeIdxCp              *idxCp;

   idxCp = &(cellCb->ueIdxCp);
   idxCp->numIdxs = numIdxs;
   cmLListInit(&idxCp->freeLst);
   cmLListInit(&idxCp->inuseLst);
   WR_ALLOC_DATA_APP(&(idxCp->idxs), sizeof(WrDamUeIdx) * numIdxs);
   if (idxCp->idxs == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   for (idx = 0; idx < numIdxs; idx++)
   {
     idxCp->idxs[idx].ueCb = NULLP;
     idxCp->idxs[idx].idx = idx;
     idxCp->idxs[idx].lnk.node = (PTR)(&(idxCp->idxs[idx]));
     cmLListAdd2Tail(&idxCp->freeLst, &idxCp->idxs[idx].lnk);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for getting a free UE Index
 *
 * @details
 *
 *     Function: wrDamGetUeIdx
 *
 *         Processing steps:
 *         - remove a node from the free list
 *         - add a node to in-use list
 *         - set the idx to the free index value
 *
 * @param[in] cellCb : cell control block
 * @param[in] idx    : UE Index
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrDamGetUeIdx
(
WrDamCellCb                  *cellCb,
U16                          *idx
)
{
   WrDamUeIdx                *ueIdx;
   WrDamUeIdxCp              *idxCp = &cellCb->ueIdxCp;
   
   if (idxCp->freeLst.first == NULLP)
   {
      RETVALUE(RFAILED);
   }

   ueIdx = (WrDamUeIdx *)(idxCp->freeLst.first->node);
   cmLListDelFrm(&(idxCp->freeLst), &(ueIdx->lnk));
   cmLListAdd2Tail(&(idxCp->inuseLst), &(ueIdx->lnk));
   *idx = ueIdx->idx;
   RETVALUE(ROK);
}

/** @brief This function is responsible for freeing a UE Index
 *
 * @details
 *
 *     Function: wrDamPutUeIdx
 *
 *         Processing steps:
 *         - remove a node from the in-use list
 *         - add a node to free list
 *
 * @param[in] cellCb : cell control block
 * @param[in] idx    : UE Index
 * @return  S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrDamPutUeIdx
(
WrDamCellCb                  *cellCb,
U16                          idx
)
{
   WrDamUeIdx                *ueIdx;
   WrDamUeIdxCp              *idxCp = &cellCb->ueIdxCp;
   
   if (idx >= idxCp->numIdxs)
   {
      RETVALUE(RFAILED);
   }
   ueIdx = &idxCp->idxs[idx];
   cmLListDelFrm(&(idxCp->inuseLst), &(ueIdx->lnk));
   cmLListAdd2Tail(&(idxCp->freeLst), &(ueIdx->lnk));
   RETVALUE(ROK);
}

/** @brief This function is responsible for starting the inactivity timer.
 *
 * @details This function starts the inactivity timer with the provided ueCb.
 *
 *     Function: wrDamStartUeInactvTmr
 *
 *         Processing steps:
 *         - use the ueCb and call the wrStartTmr primitive to start the timer.
 *
 *
 * @param[in]  ueCb : UE CB for which the timer is started.
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamStartUeInactvTmr
(
WrDamUeCb                    *ueCb
)
{
   RLOG2(L_INFO, "Inactivity timer has been started inactvTmrVal: %d expiry count:%d",wrDamCb.inactvTmrVal,wrDamCb.expiryCnt);
   wrDamStartTmr((PTR)ueCb, WR_DAM_TMR_INACTIVITY, wrDamCb.inactvTmrVal);
   RETVALUE(ROK);
}

/** @brief This function is responsible for filling and requesting the EGTP
 *         to create a new tunnel with the provided id.
 *
 * @details
 *
 *     Function: wrDamAddTunnelAtGtp
 *
 *         Processing steps:
 *         - allocate memory for the event data structure (EgtUEvnt)
 *         - fill the EgtUEvnt structure for creating a new tunnel using 
 *           the values from tnl parameter passed to this function
 *
 *
 * @param[in]  tnl: tunnel info for creating new tunnel
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrDamAddTunnelAtGtp
(
WrDamTnlCb                   *tnlCb
)
{
   EgtUEvnt                  *eguEvtMsg;

   /*****************************************************************
    * allocate memory for the e-gtpu event message of size EgtUEvnt *
    *****************************************************************/
   WR_ALLOC_DATA_APP(&eguEvtMsg, sizeof(EgtUEvnt));
   if (eguEvtMsg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   eguEvtMsg->u.tnlMgmt.action = EGT_TMGMT_REQTYPE_ADD;

   /* Currently default values are filled */
   eguEvtMsg->u.tnlMgmt.intfType = EGU_DFLT_INTFTYPE;
   eguEvtMsg->u.tnlMgmt.reOrderPres = FALSE;
   eguEvtMsg->u.tnlMgmt.expSeqNmb = EGU_DFLT_EXPSEQNMB;
   eguEvtMsg->u.tnlMgmt.maxTPduSize = EGU_DFLT_MAXTPDUSZ;


   eguEvtMsg->u.tnlMgmt.teId = tnlCb->locTeId;
   eguEvtMsg->u.tnlMgmt.reordAlgo = EG_REORDER_TYPE_1;
   eguEvtMsg->u.tnlMgmt.tunnelType = (tnlCb->locTeId & 0x03)== WR_TNL_NORMAL ? EG_NORMAL_DATA_TUNN  : EG_FWD_DATA_TUNN;
   eguEvtMsg->u.tnlMgmt.dscp = tnlCb->dscp;
   eguEvtMsg->u.tnlMgmt.remTeId = tnlCb->remTeid;
   wrCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.srcAddr), &(tnlCb->lclAddr));
   wrCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.dstIpAddr), &(tnlCb->dstAddr));

   eguEvtMsg->event = EVTEGTULCLTNLMGMTREQ;
   /* ccpu00125004: Trigger primitive */
   if(ROK != WrIfmEgtpEguLclTnlMgmtReq(eguEvtMsg))
   {
	   WR_FREE_DATA_APP(eguEvtMsg, sizeof(EgtUEvnt));
   }

   RETVALUE(ROK);
}

/* ccpu00126336 */
/** @brief This function is responsible for filling and requesting the EGTP
 *         to modify an existing tunnel.
 *
 * @details
 *
 *     Function: wrDamModTunnelAtGtp
 *
 *         Processing steps:
 *         - allocate memory for the event data structure (EgtUEvnt)
 *         - fill the EgtUEvnt structure for modifying an existing tunnel using 
 *           the values from tnl parameter passed to this function
 *
 *
 * @param[in]  tnlCb     : tunnel info for creating modify a tunnel
 * @param[in]  frmDstAddr: previously configured destination address
 *
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrDamModTunnelAtGtp
(
WrDamTnlCb                   *tnlCb,
CmTptAddr                    *frmDstAddr
)
{
   EgtUEvnt    *eguEvtMsg;

   /*****************************************************************
    * allocate memory for the e-gtpu event message of size EgtUEvnt *
    *****************************************************************/
   WR_ALLOC_DATA_APP(&eguEvtMsg, sizeof(EgtUEvnt));
   if (eguEvtMsg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   eguEvtMsg->u.tnlMgmt.action = EGT_TMGMT_REQTYPE_MOD;

   /* Currently Default values are filled for intfType and maxTPduSize */
   eguEvtMsg->u.tnlMgmt.intfType = EGU_DFLT_INTFTYPE;
   eguEvtMsg->u.tnlMgmt.maxTPduSize = EGU_DFLT_MAXTPDUSZ;
   eguEvtMsg->u.tnlMgmt.teId = tnlCb->locTeId;
   eguEvtMsg->u.tnlMgmt.remTeId = tnlCb->remTeid;
   eguEvtMsg->u.tnlMgmt.dscp = tnlCb->dscp;
   wrCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.srcAddr), &(tnlCb->lclAddr));
   wrCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.dstIpAddr), &(tnlCb->dstAddr));
   wrCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.frmDstIpAddr), frmDstAddr);

   eguEvtMsg->event = EVTEGTULCLTNLMGMTREQ;
   /* Trigger primitive */
   if(ROK != WrIfmEgtpEguLclTnlMgmtReq(eguEvtMsg))
   {
      WR_FREE_DATA_APP(eguEvtMsg, sizeof(EgtUEvnt));
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for filling and requesting the EGTP
 *         to delete an existing tunnel with the provided id.
 *
 * @details 
 *
 *     Function: wrDamDelTunnelAtGtp
 *
 *         Processing steps:
 *         - allocate memory for the event data structure (EgtUEvnt)
 *         - fill the EgtUEvnt structure for deleting an existing tunnel using 
 *           the values from tnl parameter passed to this function
 *
 *
 * @param[in]  tnl: tunnel info for deleting a tunnel
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrDamDelTunnelAtGtp
(
WrDamTnlCb                   *tnlCb
)
{
   EgtUEvnt    *eguEvtMsg = NULLP;

   /*****************************************************************
    * allocate memory for the e-gtpu event message of size EgtUEvnt *
    *****************************************************************/
   WR_ALLOC_DATA_APP(&eguEvtMsg, sizeof(EgtUEvnt));
   if (eguEvtMsg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   eguEvtMsg->u.tnlMgmt.action = EGT_TMGMT_REQTYPE_DEL;
   eguEvtMsg->u.tnlMgmt.teId   = tnlCb->locTeId;
 
   wrCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.srcAddr), &(tnlCb->lclAddr));
   wrCpyCmTptAddr(&(eguEvtMsg->u.tnlMgmt.dstIpAddr), &(tnlCb->dstAddr));
   /* ccpu00125004 Trigger primitive */
   if(ROK!=WrIfmEgtpEguLclTnlMgmtReq(eguEvtMsg))
   {
      WR_FREE_DATA_APP(eguEvtMsg, sizeof(EgtUEvnt));
   }


   RETVALUE(ROK);
}

/** @brief This function is responsible for retrieving a WrDamCellCb,
 *         corresponding to the cellId.
 *
 * @details 
 *
 *     Function: wrDamGetCell
 *
 *         Processing steps:
 *         - traverses the cell data base in wrDamCb and return the 
 *           address of the cellCb corresponding to the provided cellId.
 *
 *
 * @param[in]  cellId: unique cellId
 * @return WrDamCellCb*
 *     -#Success: WrDamCellCb*
 *     -#Failure: NULLP
 */
PUBLIC WrDamCellCb *wrDamGetCell
(
U16                          cellId
)
{
   U32                       idx;

   for (idx = 0; idx < WR_DAM_MAX_CELLS; idx++)
   {
      if (wrDamCb.cells[idx].cellId == cellId)
      {
         RETVALUE(&wrDamCb.cells[idx]);
      }
   }
   RETVALUE(NULLP);
}

/** @brief This function is responsible for fetching a ueCb from a rnti
 *
 * @details
 *
 *     Function: wrDamGetUe
 *
 *         Processing steps:
 *         - fetch the cellCb corresponding to the cellId
 *         - validate the rnti
 *         - return the ueCb at [rnti - configured start rnti]
 *
 * @param[in]  cellId: unique cellId
 * @param[in]  rnti  : UE ID
 * @return  WrDamUeCb*
 *    -#Success : WrDamUeCb*
 *    -#Failure : NULLP
 */
PRIVATE WrDamUeCb *wrDamGetUe
(
U16                          cellId,
U16                          rnti
)
{
   WrDamCellCb               *cellCb;

   if ((cellCb = wrDamGetCell(cellId)) == NULLP)
   {
      RETVALUE(NULLP);
   }
   if (!(WR_DAM_VALID_RNTI(cellCb, rnti)))
   {
      RETVALUE(NULLP);
   }
   RETVALUE(cellCb->ueCbs[rnti - cellCb->startRnti]);
}

/** @brief This function is responsible for fetching a UE CB from a ueIdx
 *
 * @details
 *
 *     Function: wrDamGetUeByUeIdx
 *
 *         Processing steps:
 *         - fetch the cellCb corresponding to the cellId
 *         - validate the ueIdx
 *         - return the ueCb at [ueIdx]
 *
 * @param[in]  cellId: unique cellId
 * @param[in]  ueIdx : UE Index
 * @return  WrDamUeCb*
 *    -#Success : WrDamUeCb*
 *    -#Failure : NULLP
 */
PRIVATE WrDamUeCb *wrDamGetUeByUeIdx
(
U16                          cellId,
U16                          ueIdx
)
{
   WrDamCellCb               *cellCb;

   if ((cellCb = wrDamGetCell(cellId)) == NULLP)
   {
      RETVALUE(NULLP);
   }
   if (ueIdx >= cellCb->ueIdxCp.numIdxs)
   {
      RETVALUE(NULLP);
   }
   RETVALUE(cellCb->ueIdxCp.idxs[ueIdx].ueCb);
}

/** @brief This function is responsible for starting the inactivity timer.
 *
 * @details This function starts the inactivity timer with the provided ueCb.
 *
 *     Function: wrDamStartUeTmr 
 *         Processing steps:
 *         - use the ueCb and call the wrStartTmr primitive to start the timer.
 *
 *
 * @param[in]  ueCb : UE CB for which the timer is started.
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamStartUeTmr
(
 U16                         cellId,
 U16                         crnti
 )
{
   WrDamUeCb                 *ueCb;
   ueCb = wrDamGetUe(cellId,crnti);
   if (ueCb == NULLP)
   {
     RLOG0(L_ERROR, "UeCb is not configured at dam");
     RETVALUE(RFAILED);
   }
         /*Start the Inactivity timer*/
   if(ROK !=  wrDamStartUeInactvTmr(ueCb))
   {
      RLOG0(L_ERROR, "Failed to start Inactivity timer");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for getting the tunnel
 *         from DRB control block
 *
 * @details
 *
 *     Function: wrDamGetTnlCbFrmDrb
 *
 *         Processing steps:
 *         - Based on the tunnel type get the Tunnel control
 *           block from the DRB
 *
 *
 * @param[in]  drbCb  : DRB Control Block
 * @param[in]  tnlType: Type of the Tunnel
 * @return WrDamTnlCb address
 */
PRIVATE WrDamTnlCb * wrDamGetTnlCbFrmDrb
(
WrDamDrbCb                   *drbCb,
WrDamTnlType                 tnlType
)
{
   WrDamTnlCb                *tnlCb = NULLP;

   switch(tnlType)
   {
      case WR_TNL_NORMAL:
         tnlCb = drbCb->normalTnl;
         break;
      case WR_TNL_DL_DATFWD:
         tnlCb = drbCb->dlFwdTnl;
         break;
      case WR_TNL_UL_DATFWD:
         tnlCb = drbCb->ulFwdTnl;
         break;
      default:
         break;
   } /* end of switch statement */
   
   RETVALUE(tnlCb);
} /* end of wrDamGetTnlCbFrmDrb */

/** @brief This function is responsible for fetching a tunnel cb based on rnti
 *         and rbId
 *
 * @details
 *
 *     Function: wrDamGetTunnel
 *
 *         Processing steps:
 *         - fetch the ueCb corresponding to the rnti
 *         - return tunnelCb at [rbId]
 *
 * @param[in]  cellId : unique cell Id
 * @param[in]  rnti   : unique ue id
 * @param[in]  rbId   : radio bearer id
 * @return WrDamTunnelCb*
 *    -#Success : WrDamTunnelCb*
 *    -#Failure : NULLP
 */
PRIVATE WrDamTnlCb *wrDamGetTnlCb
(
U16                          cellId,
U16                          rnti,
U8                           rbId,
WrDamTnlType                 tnlType
)
{
   WrDamTnlCb                *tnlCb;
   WrDamUeCb                 *ueCb;
   WrDamDrbCb                *drbCb;

   ueCb = wrDamGetUe(cellId, rnti);
   if(ueCb)
   {
      drbCb = ueCb->drbs[rbId];
      if(drbCb)
      {
         tnlCb = wrDamGetTnlCbFrmDrb(drbCb, tnlType);
         RETVALUE(tnlCb);
      }
   }
   RETVALUE(NULLP);
}



/** @brief This function is responsible for forwarding the tunnel management
 *         confirms (add/delete/modify) to the application.
 *
 * @details
 *
 *     Function: wrDamTnlMgmtCfm
 *
 *         Processing steps:
 *         - derive the cellId, rnti and rbId from the teId sent as part of the
 *           eguMsg
 *         - fetch the tunnelCb corresponding to these values
 *         - invoke the IFM primitive to send the confirm to application
 *         - free the eguMsg
 *
 * @param[in]  eguMsg : management info regarding the tunnel
 * @return void 
 */
PUBLIC Void  wrDamTnlMgmtCfm
(
EgtUEvnt                     *eguMsg
)
{
   WrDamTnlCb                *tnlCb;
   U16                       cellId;
   U16                       ueIdx;
   U8                        rbId;
   WrDamUeCb                 *ueCb;
   WrDamTnlType              tnlType;
   

   cellId = (eguMsg->u.tnlMgmt.teId & 0xff000000) >> 24;
   ueIdx  = (eguMsg->u.tnlMgmt.teId & 0x00ffff00) >> 8;
   rbId   = (eguMsg->u.tnlMgmt.teId & 0x000000f8) >> 3;
   tnlType= (eguMsg->u.tnlMgmt.teId & 0x00000007);

   ueCb   = wrDamGetUeByUeIdx(cellId, ueIdx);

   if (ueCb == NULLP)
   {
       RLOG_ARG1(L_ERROR, DBG_UEIDX, ueIdx, "Invalid UE IDX [tnlMgmtCfm : %d]",
                 EGT_TMGMT_REQTYPE_MOD);
       RETVOID;
   }

   tnlCb  = wrDamGetTnlCb(cellId, ueCb->ueId, rbId, tnlType);

   if (tnlCb == NULLP)
   {
       RLOG_ARG3(L_ERROR, DBG_UEID, ueCb->ueId,
                 "Invalid Tunnel Info [tnlMgmtCfm : %d]"
                 "[rbId : %d] [tnlType : %d]",
                  EGT_TMGMT_REQTYPE_MOD, rbId, tnlType);
       RETVOID;
   }

   switch(eguMsg->u.tnlMgmt.action)
   {
      case EGT_TMGMT_REQTYPE_ADD:
      {
         if (eguMsg->u.tnlMgmt.cfmStatus.status == LCM_PRIM_OK)
         {
            /* Confirm creation of tunnel to the UMM module               */
            wrIfmDamWrTnlCreatCfm(tnlCb->transId, WR_DAM_CFG_OK, tnlCb->locTeId);
         }
         else
         {

             wrIfmDamWrTnlCreatCfm(tnlCb->transId, WR_DAM_CFG_FAIL, tnlCb->locTeId);
            RLOG_ARG1(L_DEBUG, DBG_UEIDX, ueIdx, "wrdamtnlmgmtcfm(): for LclTnlID[%ld]",
                          eguMsg->u.tnlMgmt.teId);
            RLOG_STR(L_DEBUG, "Tnltype:%s",(tnlType?"Forwarding":"Normal"));
         }
         break;
      }
      /* ccpu00126336 */
      case EGT_TMGMT_REQTYPE_MOD:
      {
         if (eguMsg->u.tnlMgmt.cfmStatus.status == LCM_PRIM_OK)
         {
            wrIfmDamWrTnlModCfm(tnlCb->transId, WR_DAM_CFG_OK, tnlCb->locTeId);
         }
         else
         {
            wrIfmDamWrTnlModCfm(tnlCb->transId, WR_DAM_CFG_FAIL, tnlCb->locTeId);
         }
         break;
      }
      case EGT_TMGMT_REQTYPE_DEL:
      {
         break;
      }
      default:
      {
         RLOG1(L_ERROR, "Tnl Mgmt Unhandled Action :%x", eguMsg->u.tnlMgmt.action);
         break;
      }
   }

}/* wrDamTnlMgmtCfm */

/** @brief This function is responsible for handling the error indications 
 *         sent in different scenarios.
 *
 * @details
 *
 *     Function: wrDamErrInd
 *
 *         Processing steps:
 *         - THIS FEATURE IS NOT SUPPORTED IN THIS RELEASE.
 *
 *
 * @param[in] eguMsg : Error Info 
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamErrInd
(
EgtUEvnt                     *eguMsg
)
{
   /* TODO: Address the cause indicated in error indication               */
   WR_FREE_DATA_APP(eguMsg, sizeof(*eguMsg));
   RETVALUE(ROK);
}

/** @brief This function is responsible for adding and initializing 
 *         a new cell to the wrDamCb
 *
 * @details
 *
 *     Function: wrDamAddCell
 *
 *         Processing steps:
 *         - Add the cell for this cellID
 *         - initialize the particular cellCb with the input values
 *         - allocate ue data base for maxRntis size
 *
 *
 * @param[in]  cellId    : unique cellID
 * @param[in]  startRnti : starting range of UE IDs
 * @param[in]  numRntis  : number of UE IDs that can be used
 * @return S16
 *    -#Success : ROK
 *    -#Faileure: RFAILED
 */
PRIVATE S16 wrDamAddCell
(
U16                          cellId,
U16                          startRnti,
U16                          numRntis
)
{
   WrDamCellCb               *cellCb = NULLP;
   U32                       idx;

   for (idx = 0; idx < WR_DAM_MAX_CELLS; idx++)
   {
      if (wrDamCb.cells[idx].cellInUse == FALSE)
      {
         cellCb = &wrDamCb.cells[idx];
         break;
      }
   }
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }
   cellCb->cellId    = cellId;
   cellCb->startRnti = startRnti;
   cellCb->numRntis  = numRntis;

   WR_ALLOC_DATA_APP(&cellCb->ueCbs, numRntis * sizeof(WrDamUeCb *));

   if (cellCb->ueCbs == NULLP)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      for (idx = 0; idx < numRntis; idx++)
      {
         cellCb->ueCbs[idx] = NULLP;
      }
   }
   if (wrDamInitUeIdxs(cellCb, numRntis) != ROK)
   {
      WR_FREE_DATA_APP(cellCb->ueCbs, numRntis * sizeof(WrDamUeCb *));
      RETVALUE(RFAILED);
   }
    
   RETVALUE(ROK);
}

/** @brief This function handles the incoming cell addition request.
 *
 * @details
 *
 *     Function: wrDamCellAddReq
 *
 *         Processing steps:
 *         - invoke wrDamAddCell primitive to create new cell with the 
 *           provided inputs.
 *
 *
 * @param[in]  transId  : unique transaction id
 * @param[in]  cellInfo : init values for a new cell
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamCellAddReq
(
U32                          transId,
WrDamCellInfo                *cellInfo
)
{
   if (wrDamAddCell(cellInfo->cellId, cellInfo->startRnti, 
                                      cellInfo->maxRntis) != ROK)
   {
      /*127802*/
      RLOG0(L_ERROR, "ALARM!! Error adding cell to DAM module");
      RETVALUE(ROK);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for deleting a UE
 *
 * @details
 *
 *     Function: wrDamDelUe
 *
 *         Processing steps:
 *         - get the cellCb corresponding to cellCb
 *         - validate the rnti
 *         - free the ue index
 *         - free the ueCb
 *
 * @param[in]  cellId    : unique cellID
 * @param[in]  rnti      : UE ID
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrDamDelUe
(
U16                          cellId,
U16                          rnti
)
{
   WrDamCellCb               *cellCb;
   WrDamUeCb                 *ueCb;
   U16 ueIdxToBeDeleted;
 
   if ((cellCb = wrDamGetCell(cellId)) == NULLP)
   {
      RETVALUE(RFAILED);
   }
   if (!(WR_DAM_VALID_RNTI(cellCb, rnti)))
   {
      RETVALUE(RFAILED);
   }
   ueCb = cellCb->ueCbs[rnti - cellCb->startRnti];
   RLOG_ARG1(L_ALWAYS, DBG_CRNTI, ueCb->ueId, "DAM UeCb getting deleted Index %d\n",
               ueCb->ueIdx);
   /* Calling Stop Timer for the UE */
   wrDamStopTmr((PTR)ueCb,WR_DAM_TMR_INACTIVITY);
   wrDamStopTmr((PTR)ueCb,WR_DAM_TMR_END_MARKER_TMR);

   ueIdxToBeDeleted = ueCb->ueIdx;
   wrDamPutUeIdx(cellCb, ueCb->ueIdx);

   WR_FREE_DATA_APP((ueCb), sizeof(WrDamUeCb));
   cellCb->ueCbs[rnti - cellCb->startRnti] = NULLP;
   cellCb->ueIdxCp.idxs[ueIdxToBeDeleted].ueCb = NULLP;

   RETVALUE(ROK);
}

/** @brief This function is responsible for adding a new UE CB
 *
 * @details
 *
 *     Function: wrDamAddUe
 *
 *         Processing steps:
 *         - get the cell corresponding to the cellId
 *         - validate the rnti
 *         - allocate a new ueCb
 *         - initilize the ueCb with default values
 *         - initilize inactivity timer
 *         - start inactivity timer
 *
 * @param[in] cellId  : unique cell id
 * @param[in] rnti    : UE ID
 * @param[in] ueCb    : UE Control Block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrDamAddUe
(
U16                          cellId,
U16                          rnti,
WrDamUeCb                    **ueCb,
Bool                         isFullCfg  /* RRC-REL10-Upgrade */
)
{
   WrDamCellCb               *cellCb;
   U16                       ueIdx;

   if ( wrDamGetUe(cellId,rnti) == NULLP)
   {
       if ((cellCb = wrDamGetCell(cellId)) == NULLP)
       {
           RETVALUE(RFAILED);
       }
       if (!(WR_DAM_VALID_RNTI(cellCb, rnti)))
       {
           RETVALUE(RFAILED);
       }
       if (wrDamGetUeIdx(cellCb, &ueIdx) != ROK)
       {
           RETVALUE(RFAILED);
       }
       WR_ALLOC_DATA_APP(ueCb, sizeof(**ueCb));
       if ((*ueCb) == NULLP)
       {
          RLOG0(L_FATAL, "Memory allocation failed.");
          RETVALUE(RFAILED);
       }
       (*ueCb)->ueId       = rnti;
       (*ueCb)->ueIdx      = ueIdx;
       (*ueCb)->cellId     = cellId;
       (*ueCb)->dataRcvd   = FALSE;
       (*ueCb)->numTunnels = 0;
       (*ueCb)->isFullCfg  = isFullCfg;  /* RRC-REL10-Upgrade */
       /* Start - Fix for ccpu00123981 */
       (*ueCb)->expiryCnt = 0;
       /* End - Fix for ccpu00123981 */
       cellCb->ueCbs[rnti - cellCb->startRnti] = (*ueCb);
       cellCb->ueIdxCp.idxs[ueIdx].ueCb = (*ueCb);
       cmInitTimers(&(*ueCb)->inactivityTmr, 1);
       cmInitTimers(&(*ueCb)->endMrkTmr, 1);
       wrDamStartUeInactvTmr(*ueCb);
       RLOG_ARG1(L_ALWAYS, DBG_CRNTI, rnti, "DAM UeCb getting added Index %d\n",
               ueIdx);
   }
   RETVALUE(ROK);
}

/** @brief This function is reponsible to add a new tunnel to DAM for a 
 *         particular UE.
 *
 * @details
 *
 *     Function: wrDamAddTunnel
 *
 *         Processing steps:
 *         - fetch the cellCb corresponding to the cellId
 *         - validate that the rnti is within the configured range
 *         - validate that the rbId is within the range
 *         - if a ueCb does not exist (i.e, this is the first tunnel getting
 *           created for a UE),
 *           - then derive the ueIdx, create and initialize a new ueCb
 *           - start the ue inactivity timer
 *         - add the tunnel info in the ueCb and initialize the tunnel info
 *         - invoke wrDamAddTunnelAtGtp to send request to egtp to create a 
 *           new tunnel
 *
 *
 * @param[in]  transId : unique transaction id
 * @param[in]  tnlInfo : new tunnel info
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrDamAddTunnel
(
U32                          transId,
WrDamTnlInfo                 *tnlInfo
)
{
   WrDamUeCb       *ueCb;
   WrDamTnlCb      *tnlCb  = NULLP;
   U16             cellId  = tnlInfo->tnlId.cellId;
   U16             rnti    = tnlInfo->tnlId.crnti;
   U8              rbId    = tnlInfo->tnlId.drbId;
   WrDamTnlType    tnlType = tnlInfo->tnlType;

   if (rbId >= WR_DAM_MAX_DRBS)
   {
      RETVALUE(RFAILED);
   }


   ueCb = wrDamGetUe(cellId, rnti);

   /* ccpu00135777 : SrcEnb trying to process Forwarded packets as TgtEnb &&
            Endmarker started at Tgt expires in Src Enb. In case of 
            HI and immediate HO. */
   if (NULLP != ueCb)
   {
      /* If this was previously a HI ue, check if EndMarker timer
       * is still running. If yes call expiry to set the states
       * right and also stop this timer */
      if (ueCb->endMrkTmr.tmrEvnt != TMR_NONE)
      {
         wrDamStopTmr((PTR)ueCb, WR_DAM_TMR_END_MARKER_TMR);
         wrDamHdlTmrExp(WR_DAM_TMR_END_MARKER_TMR, ueCb);
      }
   }
   if (tnlType == WR_TNL_NORMAL)
   {
       if(ueCb == NULLP)
       {
          if (wrDamAddUe(cellId, rnti, &ueCb, tnlInfo->isFullCfg) != ROK) /* RRC-REL10-Upgrade */
          {
             RETVALUE(RFAILED);
          }
       }
       if (((ueCb)->drbs[rbId]) == NULLP )
       {
           /* Create the DrbCb */
           WR_ALLOC_DATA_APP(&(ueCb)->drbs[rbId],sizeof(WrDamDrbCb));
           if((ueCb)->drbs[rbId])
           {
               (ueCb)->numDrbs++;
               (ueCb)->drbs[rbId]->state = WR_DAM_DRB_STATE_NORMAL;
               (ueCb)->reestabInProgress = FALSE;
               /* Create the tunnel */
               WR_ALLOC_DATA_APP(&(tnlCb), sizeof(WrDamTnlCb));
               (((ueCb)->drbs[rbId])->normalTnl) = tnlCb;
           }
       }
       else
       {
           RETVALUE(RFAILED);
       }
   }/* WR_TNL_NORMAL */
   else if (tnlType == WR_TNL_DL_DATFWD)
   {
       if(ueCb == NULLP)
       {
          RLOG0(L_ERROR, "ueCb is NULLP.");
          RETVALUE(RFAILED);
       }
       if ((ueCb)->drbs[rbId] != NULLP)
       {

          if ( (ueCb)->drbs[rbId]->dlFwdTnl != NULLP )
          {    
             RLOG0(L_ERROR, "dlFwdTnl already exists.. deleting fwd tunnel");
             wrDamDelTunnel(ueCb->cellId, ueCb->ueId, rbId,
                   WR_TNL_DL_DATFWD);
          }    

          WR_ALLOC_DATA_APP(&(tnlCb), sizeof(WrDamTnlCb));
          (((ueCb)->drbs[rbId])->dlFwdTnl) = tnlCb;


           /* set the normal tunnel state to buffering */
           (ueCb)->drbs[rbId]->state = WR_DAM_DRB_STATE_BUFFERING;

         if(tnlInfo->hoSrcEnodeb == TRUE)     
         {       
            (ueCb)->drbs[rbId]->normalTnl->fwdState =     
                          WR_DAM_TNL_STATE_SOURCE_FWD_OLD_PKTS;
         }
         else
         {
            (ueCb)->drbs[rbId]->normalTnl->fwdState =
                                 WR_DAM_TNL_STATE_FWD_OLD_PKTS;
         }
      }
     
       else
       {
           RETVALUE(RFAILED);
       }
   }/* WR_TNL_DL_DATFWD */
   else if (tnlType == WR_TNL_UL_DATFWD)
   {
       if(ueCb == NULLP)
       {
          RLOG0(L_ERROR, "ueCb is NULLP.")
          RETVALUE(RFAILED);
       }
       if ((ueCb)->drbs[rbId] != NULLP)
       {

          if ( (ueCb)->drbs[rbId]->ulFwdTnl != NULLP )
          {
             RLOG0(L_ERROR, "ulFwdTnl already exists.. deleting fwd tunnel");
             wrDamDelTunnel(ueCb->cellId, ueCb->ueId, rbId,
                   WR_TNL_UL_DATFWD);
          }
          WR_ALLOC_DATA_APP(&(tnlCb), sizeof(WrDamTnlCb));
          (((ueCb)->drbs[rbId])->ulFwdTnl) = tnlCb;

           /* set the normal tunnel state to buffering */
           (ueCb)->drbs[rbId]->state = WR_DAM_DRB_STATE_BUFFERING;

         if(tnlInfo->hoSrcEnodeb == TRUE)       
         {       
            (ueCb)->drbs[rbId]->normalTnl->fwdState =     
                           WR_DAM_TNL_STATE_SOURCE_FWD_OLD_PKTS;      
         }       
         else       
         {
            (ueCb)->drbs[rbId]->normalTnl->fwdState =
                                  WR_DAM_TNL_STATE_FWD_OLD_PKTS;
           }
       }
       else
       {
           RETVALUE(RFAILED);
       }
   } /* WR_TNL_UL_DATFWD */
   else
   {
       RLOG0(L_ERROR, "Invalid Tunnel type received.");
       RETVALUE(RFAILED);
   }
   if(NULLP == tnlCb)
   {
       RLOG0(L_FATAL, "Memory allocation failed.");
       WR_FREE_DATA_APP((ueCb)->drbs[rbId], sizeof(*((ueCb)->drbs[rbId])))
       RETVALUE(RFAILED);
   }

   ueCb->numTunnels++;
   /* Cache Opt Changes */
   (ueCb)->drbs[rbId]->tnlPriority = tnlInfo->tnlId.tnlPriority;
   ueCb->hoType = tnlInfo->hoType;
   tnlCb->dscp = tnlInfo->dscp;
   tnlCb->locTeId  = (cellId & 0xff) << 24;
   tnlCb->locTeId |= (ueCb->ueIdx & 0xffff) << 8;
   tnlCb->locTeId |= (rbId & 0xff) << 3;
   tnlCb->locTeId |= (tnlType & 0xff);
   tnlCb->sduIdCntr = 0;
   tnlCb->transId = transId;
   tnlCb->remTeid = tnlInfo->remTeid;
   wrCpyCmTptAddr(&(tnlCb->dstAddr), &tnlInfo->dstAddr);
   wrCpyCmTptAddr(&(tnlCb->lclAddr), &tnlInfo->srcAddr);
   cmLListInit(&tnlCb->datBuffLst);
   tnlCb->fwdState = WR_DAM_TNL_STATE_FWD_INITIAL;

   /* Invoke GTP to add the tunnel here */
   wrDamAddTunnelAtGtp(tnlCb);
   RETVALUE(ROK);
}

/** @brief This function handles the incoming tunnel create request.
 *
 * @details
 *
 *     Function: wrDamTnlCreatReq
 *
 *         Processing steps:
 *         - invoke wrDamAddTunel to create a new tunnel
 *         - send the tunnel create confirm to application
 *
 *
 * @param[in]  transId : unique transaction id
 * @param[in]  tnlInfo : new tunnel info
 * @return S16
 */
PUBLIC Void  wrDamTnlCreatReq
(
U32                          transId,
WrDamTnlInfo                 *tnlInfo
)
{
  
   if (wrDamAddTunnel(transId, tnlInfo) != ROK)
   {
      /* Send failure back to the calling module. The last parameter      */
      /* as 0 should be ignored by the receiving module as status is      */
      /* marked as FAILURE                                                */
      wrIfmDamWrTnlCreatCfm(transId, WR_DAM_CFG_FAIL, 0);
   }
   WR_FREE_DATA_APP_SHAREBLE_BUF(tnlInfo, sizeof(WrDamTnlInfo));
}

/* ccpu00126336 */
/** @brief This function handles the incoming tunnel modification request.
 *
 * @details
 *
 *     Function: wrDamTnlModReq
 *
 *         Processing steps:
 *         - invoke wrDamModTunel to modify a tunnel
 *         - send the tunnel modify confirm to application
 *
 * @param[in]  transId : unique transaction id
 * @param[in]  tnlInfo  : tunnel modification info
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamTnlModReq
(
U32                          transId,
WrDamTnlModInfo              *tnlModInfo
)
{
   WrDamTnlCb                *tnl;
   CmTptAddr                 frmDstAddr;

   tnl = wrDamGetTnlCb(tnlModInfo->tnlId.cellId,
         tnlModInfo->tnlId.crnti, tnlModInfo->tnlId.drbId,
         tnlModInfo->tnlId.tnlType);

   if(tnl == NULLP)
   {
      RLOG_ARG3(L_ERROR, DBG_CRNTI, tnlModInfo->tnlId.crnti, "Tunnel CB not found. "
               "CellId (%d) DRB (%d) tnlType (%d)",
               tnlModInfo->tnlId.cellId,
               tnlModInfo->tnlId.drbId,
               tnlModInfo->tnlId.tnlType);
      RETVALUE(RFAILED);
   }

   tnl->remTeid = tnlModInfo->remTeid;
   tnl->dscp = tnlModInfo->dscp;
   wrCpyCmTptAddr(&frmDstAddr, &(tnl->dstAddr));
   wrCpyCmTptAddr(&(tnl->dstAddr), &tnlModInfo->newDstAddr);

   wrDamModTunnelAtGtp(tnl, &frmDstAddr);

   RETVALUE(ROK);
}

/* ccpu00126796 */
/** @brief This function is responsible for freeing any buffered packets in 
 *         then tunnel CB
 *
 * @details
 *
 *     Function: wrDamFreeBuffPkts
 *
 *         Processing steps:
 *         - For each node in the linked list
 *           - Free the node from the linked list
 *           - Free the data packet stored in the node
 *           - Free the node
 *
 *
 * @param[in]  tnlCb   : Tunnel Control Block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrDamFreeBuffPkts
(
WrDamTnlCb                   *tnlCb
)
{
   CmLList                   *tmpNode = NULLP;
   WrDamEgtpDataBuf          *egtpBufInfo;

   CM_LLIST_FIRST_NODE(&tnlCb->datBuffLst, tmpNode);
   while(NULLP != tmpNode)
   {
      cmLListDelFrm(&tnlCb->datBuffLst, tmpNode);

      egtpBufInfo = (WrDamEgtpDataBuf*)tmpNode->node;
      SPutMsg(egtpBufInfo->data);
      WR_FREE_DATA_APP(egtpBufInfo, sizeof(WrDamEgtpDataBuf));

      CM_LLIST_NEXT_NODE(&tnlCb->datBuffLst, tmpNode);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for deleting the DRB of ueCb
 *
 * @details
 *
 *     Function: wrDamDelDrbCb
 *
 *         Processing steps:
 *         - Get Tunnel Cb from DrbCb
 *         - Delete the tunnel at GTP
 *
 *
 * @param[in]  ueCb   : Ue Control Block
 * @param[in]  drbCb  : DRB Control Block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrDamDelDrbCb
(
WrDamUeCb                   *ueCb,
WrDamDrbCb                  *drbCb
)
{
   U8                       tnlType;
   WrDamTnlCb               *tnlCb = NULLP;

   for(tnlType = 0; tnlType < WR_DAM_MAX_DRB_TNLS; tnlType++)
   {
      if((tnlCb = wrDamGetTnlCbFrmDrb(drbCb, tnlType)))
      {
         wrDamDelTunnelAtGtp(tnlCb);
         /* ccpu00126796 */
         wrDamFreeBuffPkts(tnlCb);
         WR_FREE_DATA_APP(tnlCb, sizeof(WrDamTnlCb));
      }
   } /* end of for loop */

   /* Decrement the number of Drbs */
   ueCb->numDrbs--;

   RETVALUE(ROK);
}

/** @brief This function is responsible for validating an incoming tunnel
 *         delete request and deleting the tunnel.
 *
 * @details
 *
 *     Function: wrDamDelTunnel
 *
 *         Processing steps:
 *         - validate that the rnti is within the configured range
 *         - fetch the ueCb
 *         - invoke wrDamDelTunFromDrbCb to delete the tunnel
 *
 *
 * @param[in]  cellId : unique cell Id
 * @param[in]  rnti   : unique ue id
 * @param[in]  rbId   : radio bearer id
 * @param[in]  tnlType : Type of the tunnel
 */
PRIVATE Void  wrDamDelTunnel
(
WrCellId                     cellId,
U16                          rnti,
U8                           rbId,
WrDamTnlType                 tnlType
)
{
   WrDamUeCb                 *ueCb;
   WrDamDrbCb                *drbCb = NULLP; 

   /* Fix of CR ccpu00123253 */
   ueCb = wrDamGetUe(cellId,rnti); 

   if (ueCb == NULLP)
   {
      RLOG0(L_WARNING, "[ALARM] Sending INV_EVT to SM");
      wrIfmDamWrSendLmAlarm(LCM_CATEGORY_PROTOCOL,LCM_EVENT_INV_EVT,
            LCM_CAUSE_UNKNOWN);
      RETVOID;
   }

   drbCb = ueCb->drbs[rbId];

   if(drbCb != NULLP)
   {
      if(tnlType == WR_TNL_NORMAL)
      {
        /* If the tunnel type to be deleted is Normal tunnel
         * the forwarding tunnels shall also be deleted implicitly 
         * and the DRB is deleted. */
         wrDamDelDrbCb(ueCb, drbCb);
         WR_FREE_DATA_APP(drbCb, sizeof(WrDamDrbCb));
         /* Fix of CR ccpu00123253 */
         ueCb->drbs[rbId] = NULLP;
         /* If the number of DRBs become zero remove the UeCb */
         if (ueCb->numDrbs == 0)
         {
            wrDamDelUe(cellId, rnti);
         }
      }
      else
      {
         /* If the tunnel is FWD tunnel just delete this tunnel */
         if(tnlType == WR_TNL_DL_DATFWD)
         {
            RLOG_ARG1(L_INFO, DBG_UEID, ueCb->ueId, "Deleting DL Forwarding Tunnel "
                     " DRB (%d)",rbId);
            /* ccpu00135777 : SrcEnb trying to process Forwarded packets as TgtEnb &&
                     Endmarker started at Tgt expires in Src Enb. In case of 
                     HI and immediate HO. */
            if (NULLP == drbCb->dlFwdTnl)
            {
               RETVOID;
            }
            wrDamDelTunnelAtGtp(drbCb->dlFwdTnl);
            /* ccpu00126796 */
            wrDamFreeBuffPkts(drbCb->dlFwdTnl);
            WR_FREE_DATA_APP(drbCb->dlFwdTnl, sizeof(WrDamTnlCb));
         }
         else if(tnlType == WR_TNL_UL_DATFWD)
         {
            RLOG_ARG1(L_INFO, DBG_UEID, ueCb->ueId, "Deleting UL Forwarding Tunnel "
                     "DRB (%d)",rbId);
            /* ccpu00135777 : SrcEnb trying to process Forwarded packets as TgtEnb &&
                     Endmarker started at Tgt expires in Src Enb. In case of 
                     HI and immediate HO. */
            if (NULLP == drbCb->ulFwdTnl)
            {
               RETVOID;
            }
            /* ccpu00126796 */
            wrDamFreeBuffPkts(drbCb->ulFwdTnl);
            wrDamDelTunnelAtGtp(drbCb->ulFwdTnl);
            WR_FREE_DATA_APP(drbCb->ulFwdTnl, sizeof(WrDamTnlCb));
         }
         else
         {
            RLOG_ARG1(L_ERROR, DBG_UEID,  ueCb->ueId, "Invalid tunnel type to delete for "
                     " DRB (%d)",rbId);
         }
      }
   }
}

/** @brief This function handles the incoming tunnel delete request.
 *
 * @details
 *
 *     Function: wrDamTnlDelReq
 *
 *         Processing steps:
 *         - invoke wrDamDelTunnel to delete the tunnel corresponding to the 
 *           input.
 *
 *
 * @param[in]  transId : unique transaction id
 * @param[in]  tnlId : tunnel info
 */
PUBLIC Void  wrDamTnlDelReq
(
U32                          transId,
WrDamTnlId                   *tnlId
)
{
   wrDamDelTunnel(tnlId->cellId, tnlId->crnti, tnlId->drbId, tnlId->tnlType);
}


/** @brief This function is responsible for filling the egtp event structure
 *         for sending data packet.
 *
 * @details
 *
 *     Function: wrFillEgtpDatMsgFB
 *
 *         Processing steps:
 *         - allocate a new EgtUEvnt
 *         - fill the src and dst addresses
 *         - fill the message header with default values and with the 
 *           tnl info
 *
 *
 * @param[in]   tnl       : tunnel info
 * @param[out]  eguEvtMsg : outgoing egtp message
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */


   /* Populating Header Info */


/** @brief This function is responsible for filling the egtp event structure
 *         for sending data packet.
 *
 * @details
 *
 *     Function: wrFillEgtpDatMsg
 *
 *         Processing steps:
 *         - allocate a new EgtUEvnt
 *         - fill the src and dst addresses
 *         - fill the message header with default values and with the 
 *           tnl info
 *
 *
 * @param[in]   tnl       : tunnel info
 * @param[out]  eguEvtMsg : outgoing egtp message
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16  wrFillEgtpDatMsg
(
WrDamTnlCb                   *tnl,
EgtUEvnt                     **eguEvtMsg,
U8                           msgType
)
{
   EgUMsgHdr                 *eguHdr = NULLP;

   WR_ALLOC_DATA_APP(eguEvtMsg, sizeof(EgtUEvnt));
   if (*eguEvtMsg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   WR_ALLOCEVNT_DATA_APP(&((*eguEvtMsg)->u.egMsg), sizeof(EgUMsg));
   if ((*eguEvtMsg)->u.egMsg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Src & Dst Address */
   wrCpyCmTptAddr(&((*eguEvtMsg)->u.egMsg->srcAddr), &(tnl->lclAddr));
   wrCpyCmTptAddr(&((*eguEvtMsg)->u.egMsg->remAddr), &(tnl->dstAddr));
   (*eguEvtMsg)->u.egMsg->lclTeid = tnl->locTeId;

   /* Populating Header Info */
   eguHdr = &(*eguEvtMsg)->u.egMsg->msgHdr;
   eguHdr->msgType = msgType;
   eguHdr->nPduNmb.pres = FALSE;
   eguHdr->seqNumber.pres = FALSE;
   eguHdr->extHdr.udpPort.pres = FALSE;
   eguHdr->extHdr.pdcpNmb.pres = FALSE;
   eguHdr->teId = tnl->remTeid;

   RETVALUE(ROK);
}
/* RLC_DL_MAX_RETX fix */
/** @brief This function handles PDCP datCfm.
 *
 * @details
 *
 *     Function: wrDamPdcpDatCfm
 *
 *         Processing steps:
 *
 * @param[in]  pdcpId: CmLtePdcpId structure
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16  wrDamPdcpDatCfm
(
CmLtePdcpId                  pdcpId
)
{
   WrDamUeCb                 *ueCb;
      
   TRC2(wrDamPdcpDatCfm);

   /* As of now it is assumed that DatCfm for DRBs comes only
    * for RLC DL Max Retx exceeding case and handled in the same 
    * way of Inactivity Timer Expiry case*/
   ueCb = wrDamGetUe(pdcpId.cellId, pdcpId.ueId);
   if (NULLP != ueCb)
   {
     WR_INC_KPI_FUNC(wrIncKpiFrRlcFailure);
      /* Stop the Inactivity Timer */
      wrDamStopTmr((PTR)ueCb, WR_DAM_TMR_INACTIVITY);

      if (wrIfmDamWrPdcpDatCfm(pdcpId) != ROK)
      {
         RETVALUE(RFAILED);
      }
   }
   else
   {
      /* Log error */
      RLOG0(L_ERROR, ":UeCb not Found");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function handles the incoming Uplink PDCP data indication.
 *
 * @details
 *
 *     Function: wrDamPdcpDatInd
 *
 *         Processing steps:
 *         - fetch the tunnel Cb
 *         - fetch the ueCb
 *         - set the dataRcvd flag to TRUE
 *         - derive the tunnel id from the cellId, ueId and the rbId
 *         - invoke wrFillEgtpDatMsg to fill the egtp event structure
 *         - invoke WrIfmEgtpEguDatReq to send the data packet to egtp
 *
 *
 * @param[in]  pdcpId : pdcp info having cell and ue info
 * @param[in]  mBuf   : uplink data packet
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrDamPdcpDatInd
(
CmLtePdcpId                  *pdcpId,
Buffer                       *mBuf
)
{
   WrDamTnlCb                *tnl;
   WrDamUeCb                 *ueCb;
#if !(defined(TENB_FP) || defined(TENB_UL_FP))
   EgtUEvnt                  *eguEvtMsg;
   EgUMsg                    *egMsg;
   WrDamTnlType              tnlType = WR_TNL_NORMAL;
#endif
   MsgLen                       ulTpt = 0;
#if (defined(TENB_FP) || defined(TENB_UL_FP))
   WrDamCellCb               *cellCb = NULLP;
   U32                       lclTeId;
   U32                       idx;
   WrDamDrbCb                *drbCb = NULLP;


   for (idx = 0; idx < WR_MAX_CELLS; idx++)
   {
      if (wrDamCb.cells[idx].cellId == pdcpId->cellId)
      {
         cellCb = &wrDamCb.cells[idx];

         if ((WR_DAM_VALID_RNTI(cellCb, pdcpId->ueId)))
         {
            ueCb = cellCb->ueCbs[pdcpId->ueId - cellCb->startRnti];

            if(ueCb)
            {
               drbCb = ueCb->drbs[pdcpId->rbId];
               if(drbCb)
               {
                 tnl = drbCb->normalTnl;

                 if(tnl)
                 {
                    ueCb->dataRcvd = TRUE;

                    /* Find out the UL througput */
                    SFndLenMsg(mBuf, &ulTpt);
                    ueCb->ulTpt += ulTpt;

                    /* Get the local TeId */
                    lclTeId  = (pdcpId->cellId & 0xff) << 24;
                    lclTeId |= (ueCb->ueIdx & 0xffff) << 8;
                    lclTeId |= (pdcpId->rbId & 0xff) << 3;
                    lclTeId |= (WR_TNL_NORMAL & 0xff);

                    egFpTunProcEguDatReq(lclTeId, tnl->remTeid, tnl->dscp, ulTpt, mBuf);

                 }  /* tnlcb */
               } /* drbcb */
            } /* UeCb */
         } /* RNTI */
         break;
      } /* Cell */
   } /* For */

#ifdef TENB_FP
   SPutMsg(mBuf);
#endif 

#else

   /* Fill EGTP Data Message */
   tnl = wrDamGetTnlCb(pdcpId->cellId, pdcpId->ueId, pdcpId->rbId, tnlType );
   ueCb = wrDamGetUe(pdcpId->cellId, pdcpId->ueId);
   if(ueCb == NULLP || tnl == NULLP)
   {
      RETVALUE(RFAILED);
   }
   ueCb->dataRcvd = TRUE;
   /* Find out the UL througput */
   SFndLenMsg(mBuf, &ulTpt);
   ueCb->ulTpt += ulTpt;

   if(ROK != wrFillEgtpDatMsg(tnl, &eguEvtMsg, EGT_GTPU_MSG_GPDU))
   {
      WR_FREEMBUF(mBuf);
      RETVALUE(ROK);
   }   

   egMsg = eguEvtMsg->u.egMsg;
   egMsg->u.mBuf = mBuf;

   /* Trigger EGTP Data Req */
   WrIfmEgtpEguDatReq(eguEvtMsg);
#endif
   RETVALUE(ROK);
}

/** @brief This function handles the forwarded packets from source eNB and
 *         forwards them to PDCP.
 *
 * @details
 *
 *     Function: wrDamSndFwdPktToPdcp
 *
 *         Processing steps:
 *         - invoke WrIfmPdcpDatFwdReq to send the packet to PDCP.
 *
 *
 * @param[in]  eguMsg : incoming egtp message
 * @param[in]  pdcpId : pdcp information
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrDamSndFwdPktToPdcp
(
CmLtePdcpId                  *pdcpId,
U32                          sduId,
U8                           dir,
EgUMsgHdr                    *msgHdr,
Buffer                       *mBuf
)
{
   PjuDatFwdReqInfo          *datFwdReq;

   if(mBuf == NULLP)
   {
      RLOG0(L_ERROR,"Mbuf is NULL in data forwarding");
      RETVALUE(RFAILED);
   }
   WR_ALLOC_DATA_APP(&datFwdReq, sizeof(PjuDatFwdReqInfo));
   if(datFwdReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   datFwdReq->dir = dir;
   /* as and when a forwarded packet with PDCP Number arrives, we send the 
    * packet to PDCP. So the numSdu is always 1 */
   datFwdReq->numSdus = 1;
   WR_ALLOC_DATA_APP(&datFwdReq->datFwdInfo, sizeof(PjuDatFwdInfo));
   if(datFwdReq->datFwdInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_DATA_APP(datFwdReq, sizeof(PjuDatFwdReqInfo));
      RETVALUE(RFAILED);
   }

   datFwdReq->datFwdInfo->sduId = sduId;
   datFwdReq->datFwdInfo->sn    = msgHdr->extHdr.pdcpNmb.val;
   datFwdReq->datFwdInfo->sdu   = mBuf;

   WrIfmPdcpDatFwdReq(pdcpId, datFwdReq);
   RETVALUE(ROK);
}


/** @brief This function forwards data packets to EGTP. The packets can be
 * buffered depending on "pktDiscard" flag.
 *
 * @details
 *
 *     Function: wrDamHndlFwdPkts
 *
 *         Processing steps:
 *
 *
 * @param[in]  eguMsg : incoming egtp message
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PUBLIC S16 wrDamHndlFwdPkts
(
Buffer            *mBuf,
WrDamEgtpDataBuf  *egtpBufInfo,
WrDamDrbCb        *drbCb,
EgtUEvnt          *eguEvtMsg,
Bool               pktDiscard,
U8                 msgType
)
{
   EgUMsg   *egMsg;
   Buffer   *tempMBuf;

/*ccpu00135468 start*/
   if(NULLP == drbCb->dlFwdTnl)
   {
        RLOG0(L_ERROR, "Fwd tunnels do NOT exist");
        RETVALUE(RFAILED);
   }
/*ccpu00135468 end*/

   if(ROK == wrFillEgtpDatMsg(drbCb->dlFwdTnl, &eguEvtMsg, msgType))
   {
      if(msgType == EGT_GTPU_MSG_GPDU)
      {
         if(eguEvtMsg == NULLP)
         {
             /* Log error */
             RLOG0(L_ERROR, "eguEvtMsg is NULL");
             RETVALUE(RFAILED);
         }
         egMsg = eguEvtMsg->u.egMsg;
         if(pktDiscard == FALSE)
         {

            if(egtpBufInfo == NULLP)
            {
              /* Log error */
              RLOG0(L_ERROR, "egtpBufInfo is NULL");
              RETVALUE(RFAILED);
            }

            /* Create a copy of the data buffer and forward the packet */
            SCpyMsgMsg(egtpBufInfo->data, WR_MEM_REGION, 0, &tempMBuf);
            egMsg->u.mBuf = tempMBuf;
         }
         else
         {
           egMsg->u.mBuf = mBuf;
         /*  eguMsg->u.egMsg->u.mBuf = NULLP;*/
         }
      }
      else if(EGT_GTPU_MSG_END_MARKER == msgType)
      {
          RLOG0(L_INFO,"********SENDING END MARKER TO TARGET ENB********");
      }
      /* Trigger EGTP Data Req */
      WrIfmEgtpEguDatReq(eguEvtMsg);
   }
   RETVALUE(ROK);
}

/* ccpu00135472 start */
/** @brief This function stops the endmarter timer for the bearer which have
 *   received the End Market packet.
 *
 * @details
 *
 *     Function: wrDamChkEndMrkTmr
 *
 *         Processing steps:
 *
 *
 * @param[in]  ueCb : pointer to DAM UeCb Context
 * @return Void
 */
PRIVATE Void wrDamChkEndMrkTmr(WrDamUeCb *ueCb)
{
	U16                       rbIdx = 0;
	WrDamDrbCb                *rbInfo;
	U8                        rbId;
	U8                        drbCnt=0;

	for(rbIdx = 0; rbIdx < WR_DAM_MAX_DRBS; rbIdx++)
	{
		rbInfo = ueCb->drbs[rbIdx];
		if(NULLP != rbInfo)
		{
			drbCnt++;
			if(WR_DAM_DRB_STATE_BUFFERING == rbInfo->state)
			{
				WR_GET_RBID_FROM_LOC_TEID(rbInfo->normalTnl->locTeId, rbId);

				RLOG2(L_DEBUG, "Pending EndMark Reception for UE (%d) DRB (%d)",
						ueCb->ueId, rbId);
				/* If any of them has not received EndMarker yet then
				 * skip stopping EndMarker timer */
				RETVOID;
			}
			if (drbCnt == ueCb->numDrbs)
			{
				break;
			}
		}
	}
	RLOG1(L_DEBUG, "Stop EndMark Timer for UE (%d)\n",
			ueCb->ueId);

   ueCb->isFullCfg = FALSE; /* RRC-REL10-Upgrade */
	wrDamStopTmr((PTR)ueCb, WR_DAM_TMR_END_MARKER_TMR);
	RETVOID;
}
/* ccpu00135472 end */
 
PUBLIC S16 wrDamHdlDlPkts
(
 EgUMsgHdr                 *msgHdr,
 WrDamDrbCb                *drbCb,
 WrDamTnlCb                *tnlCb,
 WrDamUeCb                 *ueCb,
 Buffer                    *mBuf
)
{
   WrDamTnlCb                *nrmlTnlCb;
   U32                       lclTeid = msgHdr->teId;
   WrDamTnlType              tnlType = (lclTeid & 0x00000007);
   CmLteCellId               cellId = (lclTeid & 0xff000000) >> 24;
   CmLteRbId                 rbId   = (lclTeid & 0x000000f8) >> 3;
   U8                        dir;
   Bool                      fwdedOldPkt = FALSE;
   Bool                      delFwdTnls = FALSE;
   WrDamTnlStInfo            *tnlInfo;
   CmLtePdcpId               pdcpId;
   WrDamEgtpDataBuf          *egtpBufInfo = NULLP;
   U8                        msgType = EGT_GTPU_MSG_GPDU;
   EgtUEvnt                  *eguEvtMsg = NULLP;
      nrmlTnlCb = wrDamGetTnlCbFrmDrb(drbCb, WR_TNL_NORMAL);
   if(msgHdr->msgType == EGT_GTPU_MSG_GPDU)
      {
         if(WR_TNL_NORMAL != tnlType)
         {
         if(TRUE == msgHdr->extHdr.pdcpNmb.pres)
            {
               if(ueCb->isFullCfg == TRUE)
               {
               WR_FREEMBUF(mBuf); 
                  RETVALUE(ROK);
               }
               fwdedOldPkt = TRUE;
            }
         }
         if(fwdedOldPkt == TRUE)
         {
            pdcpId.ueId    = ueCb->ueId;
            pdcpId.cellId = cellId;
            pdcpId.rbId   = rbId;
            pdcpId.rbType = CM_LTE_DRB;
            if(tnlType == WR_TNL_DL_DATFWD)
            {
               dir = CM_LTE_DIR_DL;
            }
            else
            {
               dir = CM_LTE_DIR_UL;
            }
            nrmlTnlCb->sduIdCntr++;
#if 1
         wrDamSndFwdPktToPdcp(&pdcpId, nrmlTnlCb->sduIdCntr, dir,
                                                   msgHdr, mBuf);
#else
            WR_FREEMBUF(eguMsg->u.egMsg->u.mBuf);
#endif
         }
         if((tnlType == WR_TNL_NORMAL) &&
               (tnlCb->fwdState != WR_DAM_TNL_STATE_FWD_NEW_PKTS) &&
               (tnlCb->fwdState != WR_DAM_TNL_STATE_SOURCE_FWD_OLD_PKTS) )
         {
            if(nrmlTnlCb->sduIdCntr > 0)
            {
               if(!wrIsTmrRunning(&(ueCb->endMrkTmr), WR_TMR_END_MARKER_TMR))
               {
                  RLOG0(L_DEBUG, "\nEndMarker Timer Started\n");
                  wrDamStartEndMrkrTmr(cellId,ueCb->ueId);
               }
            }
            else
            {
               WR_ALLOC(&(tnlInfo), sizeof(WrDamTnlStInfo));
               if(tnlInfo == NULLP)
               {
                  RLOG0(L_FATAL, " Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               tnlInfo->transId = 0;
               tnlInfo->state = WR_DAM_DRB_STATE_NORMAL;
               tnlInfo->cause = WR_DAM_HO_END_MARKER_RCVD;
               tnlInfo->lclTeid = nrmlTnlCb->locTeId;
               wrDamTnlStChngReq(tnlInfo);
               WR_FREE_DATA_APP((tnlInfo),
                     sizeof(WrDamTnlStInfo));
               delFwdTnls = TRUE;
               pdcpId.ueId    = ueCb->ueId;
               pdcpId.cellId = cellId;
               pdcpId.rbId   = rbId;
               pdcpId.rbType = CM_LTE_DRB;
               tnlCb->sduIdCntr++;
            WrIfmPdcpDatReq(&pdcpId, tnlCb->sduIdCntr, drbCb->tnlPriority, mBuf);
            }
         }
         if((fwdedOldPkt == FALSE) && (delFwdTnls != TRUE))
         {
            if(WR_DAM_BUFFERING_PKT_LIMIT == (tnlCb->datBuffLst.count + 1))
            {
               if(WR_DAM_TNL_STATE_FWD_NEW_PKTS == tnlCb->fwdState)
               {
               wrDamHndlFwdPkts(mBuf, egtpBufInfo, drbCb,
                                                  eguEvtMsg, TRUE, msgType); 
               }
               RETVALUE(ROK);
            }
            WR_ALLOC_DATA_APP(&egtpBufInfo, sizeof(WrDamEgtpDataBuf));
            if(egtpBufInfo == NULLP)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
         egtpBufInfo->data = mBuf;
            cmLListAdd2Tail(&(tnlCb->datBuffLst), &(egtpBufInfo->lnk));
            egtpBufInfo->lnk.node = (PTR)egtpBufInfo;
            RLOG2(L_DEBUG, "buffering packet... DatFwd State [%d] TnlId [%lu]",
                  tnlCb->fwdState, tnlCb->locTeId);
         }
      }
      else
      {
         msgType = EGT_GTPU_MSG_END_MARKER;
         RLOG_ARG1(L_INFO, DBG_UEID,ueCb->ueId, "End Marker recieved for "
               "DRB (%d)",rbId);
         if((tnlCb->fwdState != WR_DAM_TNL_STATE_FWD_NEW_PKTS) &&
               (nrmlTnlCb != tnlCb) &&
               (tnlCb->fwdState != WR_DAM_TNL_STATE_SOURCE_FWD_OLD_PKTS))
         {
            RLOG_ARG1(L_INFO, DBG_UEID,ueCb->ueId, "Changing tunnel state for"
                  "DRB (%d)",rbId);
            WR_ALLOC_DATA_APP(&(tnlInfo), sizeof(WrDamTnlStInfo));
            if(tnlInfo == NULLP)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            tnlInfo->transId = 0;
            tnlInfo->state = WR_DAM_DRB_STATE_NORMAL;
            tnlInfo->cause = WR_DAM_HO_END_MARKER_RCVD;
            tnlInfo->lclTeid = nrmlTnlCb->locTeId;
            wrDamTnlStChngReq(tnlInfo);
            WR_FREE_DATA_APP((tnlInfo), sizeof(WrDamTnlStInfo));
            wrDamChkEndMrkTmr(ueCb);/* ccpu00135472 fix */
         }
         delFwdTnls = TRUE;
      }
      if ( tnlCb->fwdState == WR_DAM_TNL_STATE_FWD_NEW_PKTS )
      {
      if(wrDamHndlFwdPkts(mBuf, egtpBufInfo, drbCb, 
                                     eguEvtMsg, FALSE,msgType) != ROK)
         {   
            CmLList          *delNode;
            WrDamEgtpDataBuf *egtpBufInfo1;
            delNode = cmLListLast(&tnlCb->datBuffLst);
            egtpBufInfo1 = (WrDamEgtpDataBuf*)delNode->node;
            cmLListDelFrm(&(tnlCb->datBuffLst), delNode);
            SPutSBuf(0, 0, (Data *)egtpBufInfo1, sizeof(WrDamEgtpDataBuf));
            RETVALUE(RFAILED);
         }
      }
      if(delFwdTnls == TRUE)
      {
         if(drbCb->dlFwdTnl != NULLP)
         {
            wrDamDelTunnel(ueCb->cellId, ueCb->ueId, rbId,
                  WR_TNL_DL_DATFWD);
         }
         if(drbCb->ulFwdTnl != NULLP)
         {
            wrDamDelTunnel(ueCb->cellId, ueCb->ueId, rbId,
                  WR_TNL_UL_DATFWD);
         }
      }
   RETVALUE(ROK);
}

/** @brief This function handles the incoming EGTP data packets and forwards
 *         them to PDCP.
 *
 * @details
 *
 *     Function: wrDamEgtpDatInd
 *
 *         Processing steps:
 *         - derive the cellId, ueId, rbId from the teId of the incoming 
 *           message and fill the CmLtePdcpId structure
 *         - get the tunnelCb
 *         - get the ueCb
 *         - set the dataRcvd flag to TRUE
 *         - invoke WrIfmPdcpDatReq to send the packet to PDCP.
 *
 *
 * @param[in]  eguMsg : incoming egtp message
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PUBLIC S16 wrDamEgtpDatInd
(
Pst                          *pst,
EgtUEvnt                     *eguMsg
)
{
   U32                       lclTeid = eguMsg->u.egMsg->msgHdr.teId;
   CmLtePdcpId               pdcpId;
   WrDamTnlCb                *tnlCb = NULLP;
   WrDamUeCb                 *ueCb = NULLP;
   WrDamDrbCb                *drbCb = NULLP;
   U16                       ueIdx;
   WrDamTnlType              tnlType;
   CmLteCellId               cellId;      /*!< Cell ID */
   CmLteRbId                 rbId;        /*!< PDCP Instance ID */
   MsgLen                    dlTpt = 0;

   cellId = (lclTeid & 0xff000000) >> 24;
   ueIdx  = (lclTeid & 0x00ffff00) >> 8;
   rbId   = (lclTeid & 0x000000f8) >> 3;
   tnlType= (lclTeid & 0x00000007);

   ueCb  = wrDamGetUeByUeIdx(cellId, ueIdx);
   if (ueCb == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting ueCb for "
            "ueIdx = %u", ueIdx);
      RETVALUE(RFAILED);
   }
   if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_SUPP_EXT_HDR_NTF)
   {
      RLOG0(L_DEBUG, "Received EGTP HDR NOTIFY");
      RETVALUE(RFAILED);
   }
   if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_ERROR_IND)
   {
      RLOG0(L_ERROR, "Received EGTP ERROR IND");

      if(TRUE == ueCb->reestabInProgress)
      {
         RLOG0(L_ERROR, "wrDamEgtpDatInd: Received EGTP ERROR IND \
               during re-eastablishment in Progress");
         RETVALUE(RFAILED);
      }

      RLOG2(L_ERROR, "EGTP ERROR IND: sending to APP lclTeid(%ld)" 
            ",numDrbs(%ld)",lclTeid,ueCb->numDrbs);

      wrIfmDamEgtpErrInd(lclTeid, ueCb->numDrbs, ueCb->ueId);
      RETVALUE(RFAILED);
   }

   if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_ECHO_RSP)
   {
      RLOG0(L_DEBUG, "Received EGTP ECHO RSP ");
      RETVALUE(RFAILED);
   }

   drbCb = ueCb->drbs[rbId];
   if(drbCb)
   {
      tnlCb = wrDamGetTnlCbFrmDrb(drbCb, tnlType);
   }
   if( tnlCb == NULLP )
   {
      RLOG0(L_ERROR, "tnlCb not found");
      RETVALUE(RFAILED);
   }
   ueCb->dataRcvd = TRUE;
   if(drbCb->state == WR_DAM_DRB_STATE_NORMAL)
   {
      if(eguMsg->u.egMsg->msgHdr.msgType == EGT_GTPU_MSG_END_MARKER)
      {
         RETVALUE(RFAILED);
      }

      /* Find out the DL througput */
      SFndLenMsg(eguMsg->u.egMsg->u.mBuf, &dlTpt);
      ueCb->dlTpt += dlTpt;
      pdcpId.ueId    = ueCb->ueId;
      pdcpId.cellId = cellId;
      pdcpId.rbId   = rbId;
      pdcpId.rbType = CM_LTE_DRB;
      /* Updating the sduCntr and passing to pdcp                            */
      tnlCb->sduIdCntr++;
      WrIfmPdcpDatReq(&pdcpId, tnlCb->sduIdCntr, drbCb->tnlPriority, eguMsg->u.egMsg->u.mBuf);
   }
   else
   {
      /* if sequence number field is not present, then buffer the packet, 
         else send to PDCP */
               /* RRC-REL10-Upgrade Start */
               /* if full configuration enabled then eNodeB will drop the SN assigned packets */
                  /* Drop the packet and free its memory and return ROK*/
                  /* NULL ckeck not requried as  WR_FREEMBUF macro take care */
               /* RRC-REL10-Upgrade end */

               /* packet that was transmitted in serving cell and for which ACK
                * was not received before handover */
         /* send the packet to pdcp, need not buffer it in DAM */
            /* Updating the sduCntr and passing to pdcp                            */
            /* - HOFIX - temporary. need to be removed as part of 
             * data forwarding */
         /* Check if there are any forwarded packet for this bearer.
          * If yes, then start the END Marker timer,
          * Else, change the tunnel state and start sending the packets
          * to PDCP
          */
         /* THIS NEEDS TO BE DONE ONLY AT TARGET.
          * WR_DAM_TNL_STATE_FWD_NEW_PKTS will be set only at source */
               /* Since there are no packets so far on forwarding tunnels,
                * change the state of the normal tunnel */


               /* Send the packet to PDCP */
     wrDamHdlDlPkts(&(eguMsg->u.egMsg->msgHdr), drbCb, tnlCb, ueCb, eguMsg->u.egMsg->u.mBuf);
            }

         /* store the pkt in tht buff list */
            /* check if pkt storage limit is execeeded */
               /* discard the pdu if the pktDiscard flag is set to true */

            /* add to dataBuff */
               /* Log error */
            /*127802*/
         /* not changing tunnel at source.
          * only at source, the state will be
          * WR_DAM_TNL_STATE_FWD_NEW_PKTS */
            /* Change tunnel state */


            /* On failure the mBuf is deleted by the caller. As we have seen earlier, wrDamHndlFwdPkts() 
               can result in failure due to race condition of tnlCb->fwdState being set to FWD_NEW_PKTS and 
               actual dlFwdingTnl creation. We can also avoid this race by alwasys requesting DLFwding 
               tunnel creation prior to UlFwing tnl creation. Now when mBufs are cleared as part of ueCtxRel 
               at src side, we endup in a double free of mBuf. */
            /* Do not delete the mBuf */
         /* delete the forwarding tunnels */
   cmFreeMem(eguMsg->u.egMsg); 
   WR_FREE_DATA_APP(eguMsg, sizeof(EgtUEvnt));
   RETVALUE(ROK);
}
#ifdef FLAT_BUFFER_OPT
/** @brief This function handles the incoming EGTP data packets and forwards
 *         them to PDCP.
 *
 * @details
 *
 *     Function: wrDamEgtpDatIndFB
 *
 *         Processing steps:
 *         - derive the cellId, ueId, rbId from the teId of the incoming 
 *           message and fill the CmLtePdcpId structure
 *         - get the tunnelCb
 *         - get the ueCb
 *         - set the dataRcvd flag to TRUE
 *         - invoke WrIfmPdcpDatReqFB to send the packet to PDCP.
 *
 *
 * @param[in]  eguMsg : incoming egtp message (Flat Buffer)
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */

PUBLIC S16 wrDamEgtpDatIndFB
(
Pst                          *pst,
FlatBuffer                     *eguMsg,
U32 tnlId
)
{
   U32                       lclTeid = tnlId;
   CmLtePdcpId               pdcpId;
   WrDamTnlCb                *tnlCb = NULLP;
  /* WrDamTnlCb                *nrmlTnlCb;*/
   WrDamUeCb                 *ueCb = NULLP;
   WrDamCellCb               *cellCb = NULLP;
   WrDamDrbCb                *drbCb = NULLP;
   U16                       ueIdx;
   WrDamTnlType              tnlType;
 /*Bool                      pktDiscard = FALSE;
   EgtUEvnt                  *eguEvtMsg;
   EgUMsg                    *egMsg;
   U8                        dir;
   Buffer                    *tempMBuf;
   WrDamTnlStInfo            *tnlInfo;*/
   MsgLen                    dlTpt = 0;

   pdcpId.cellId  = (lclTeid & 0xff000000) >> 24;
   ueIdx  = (lclTeid & 0x00ffff00) >> 8;
   pdcpId.rbId    = (lclTeid & 0x000000f8) >> 3;
   tnlType= (lclTeid & 0x00000007);

   if ((cellCb = wrDamGetCell(pdcpId.cellId)) == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Allow the packet only if UE is not in Re-Establsihment State         */
   if((ueCb  = cellCb->ueIdxCp.idxs[ueIdx].ueCb) == NULLP)
/*   if (ueCb->reestabInProgress == FALSE) */
   {
      RETVALUE(RFAILED);
      }

   drbCb = ueCb->drbs[pdcpId.rbId];
   if ((drbCb == NULLP) || (tnlType != WR_TNL_NORMAL) || 
                             (drbCb->normalTnl == NULLP))
      {
      RLOG0(L_ERROR, "ueCb or DRBcb or TunnelCb is NULL");
         RETVALUE(RFAILED);
      }
   {
      tnlCb = drbCb->normalTnl;
      ueCb->dataRcvd = TRUE;
      if(drbCb->state == WR_DAM_DRB_STATE_NORMAL)
      {
         /* Find out the DL througput */
         dlTpt = eguMsg->len;
         ueCb->dlTpt += dlTpt;
         /* For now, excluding HO, all we need to do is construct the PDCP id   */
         /* from the TEID in the message received                               */
         pdcpId.ueId    = ueCb->ueId;
         pdcpId.rbType = CM_LTE_DRB;
         /* Updating the sduCntr and passing to pdcp                            */
         tnlCb->sduIdCntr++;
         WrIfmPdcpDatReqFB(&pdcpId, tnlCb->sduIdCntr, drbCb->tnlPriority, eguMsg);
      }
      else
      {
         Buffer *mBuf = NULLP;
         EgUMsgHdr eguHdr;
         S16 ret;

         /*tnlCb = wrDamGetTnlCbFrmDrb(drbCb, WR_TNL_NORMAL);*/


         /* For the case of Data Forwarding or any other case where Tunnel is not in the normal state,
            Follow the usual path and not the Fast path of Flat Buffer */
#ifdef T2K_MEM_LEAK_DBG 
         if((SAttachPtrToBufNew(wrDamCb.init.region, wrDamCb.init.pool,
                     eguMsg->ptr, eguMsg->len, &mBuf,__FILE__,__LINE__)) != ROK)
#else
         if((SAttachPtrToBuf(wrDamCb.init.region, wrDamCb.init.pool,
                     eguMsg->ptr, eguMsg->len, &mBuf)) != ROK)
#endif      
            {
               RLOG0(L_ERROR, "SAttachPtrToBuf failed");
               RETVALUE(RFAILED);
            }
   
         /* Create and fill EgtUEvnt message */
        
         eguHdr.msgType = EGT_GTPU_MSG_GPDU;
         eguHdr.teId = lclTeid;
         eguHdr.nPduNmb.pres = FALSE;
         eguHdr.extHdr.pdcpNmb.pres = FALSE;         

         /* Send Message to normal path */
         ret = wrDamHdlDlPkts(&eguHdr, drbCb, tnlCb, ueCb, mBuf);
         if(ret != ROK)
         {
            if(mBuf != NULLP)
            {
               WR_FREEMBUF(mBuf);
            }
         }
      }
   }

   RETVALUE(ROK);
}
#endif

/** @brief This function is resposible for deleting all the ue info for the 
 *         provided crnti.
 *
 * @details
 *
 *     Function: wrDamUeDelReq
 *
 *         Processing steps:
 *         - fetch the ueCb corresponding to the crnti
 *         - for all the established rbs in the ueCb send a tunnel delete
 *           request to egtp
 *         - send ue delete confirm to application
 *
 *
 * @param[in]  transId : unique transaction Id
 * @param[in]  cellId  : unique cell Id
 * @param[in]  crnti   : unique ue Id
 * @return S16
 *    -#Success : ROK
 */
PUBLIC Void wrDamUeDelReq
(
U32                          transId,
U16                          cellId,
U16                          crnti
)
{
   WrDamUeCb                 *ueCb;
   WrDamDrbCb                *drbCb;
   U32                       drbIdx;

   /* Delete all the tunnels that have been established for this UE       */
   ueCb = wrDamGetUe(cellId, crnti);
   if (ueCb != NULLP)
   {
      /* Invoke delete towards EGTP for all the tunnels but do not wait for  */
      /* response. If any response is failure, raise a DEBUG ERROR           */
      for(drbIdx = 0; drbIdx < WR_DAM_MAX_DRBS; drbIdx++)
      {
         /* If the number of DRBs become zero remove the UeCb */
         if (ueCb->numDrbs == 0)
         {
            wrDamDelUe(cellId, crnti);
            break;
         }
         drbCb = ueCb->drbs[drbIdx];
         if(drbCb)
         {
            wrDamDelDrbCb(ueCb, drbCb);
            WR_FREE_DATA_APP(drbCb, sizeof(WrDamDrbCb));
         }
      }
   }
   wrIfmDamWrUeDelCfm(transId, cellId, crnti);
}

/** @brief This function is responsible for changing the ueId and the
 *         corresponding indexes.
 *
 * @details
 *
 *     Function: wrDamChgUeId
 *
 *         Processing steps:
 *         - fetch the cellCb corresponding to the cellId
 *         - validate the oldCrnti
 *         - validate the newCrnti
 *         - fetch the ueCb at [oldCrnti - configured start rnti]
 *         - set index [oldCrnti - configured start rnti] to NULL
 *         - set index [newCrnti - configured start rnti] to ueCb
 *
 * @param[in]  cellId     : unique cell Id
 * @param[in]  oldCrnti   : old crnti
 * @param[in]  newCrnti   : new crnti
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrDamChgUeId
(
WrDamCellCb               *cellCb,
U16                       oldCrnti,
U16                       newCrnti
)
{
   WrDamUeCb    *ueCb;

   if (!(WR_DAM_VALID_RNTI(cellCb,oldCrnti))&& !(WR_DAM_VALID_RNTI(cellCb,newCrnti)))
   {
      RETVALUE(RFAILED);
   }
   ueCb = cellCb->ueCbs[oldCrnti - cellCb->startRnti];
   cellCb->ueCbs[oldCrnti - cellCb->startRnti] = NULLP;
   cellCb->ueCbs[newCrnti - cellCb->startRnti] = ueCb;
   ueCb->ueId = newCrnti; /* fix for updated NEW CRNTI REEST */
   RETVALUE(ROK);
}

/** @brief This function is responsible for handling the DAM reconfiguration
 *         request.
 *
 * @details
 *
 *     Function: wrDamReCfgReq
 *
 *         Processing steps:
 *         - Set  the reestabInProgress to FALSE
 *         - send the reestablishment confirm to application
 *
 * @param[in]  transId    : transaction id for the reestablishment
 * @param[in]  cellId     : unique cell Id
 * @param[in]  Crnti      : crnti
 * @return Void
 */
PUBLIC Void  wrDamReCfgReq
(
U32                          transId,
U16                          cellId,
U16                          crnti
)
{
   WrDamUeCb                 *ueCb;

   ueCb = wrDamGetUe(cellId, crnti);
   if(ueCb != NULLP)
   {
      ueCb->reestabInProgress = FALSE;
      wrIfmDamWrReestabCfm(transId, WR_DAM_CFG_OK);
   }
   else
   {
      wrIfmDamWrReestabCfm(transId, WR_DAM_CFG_FAIL);
   }
}

/** @brief This function is responsible for handling the reesetablishment
 *         request.
 *
 * @details
 *
 *     Function: wrDamReestabReq
 *
 *         Processing steps:
 *         - change the oldCrnti to newCrnti
 *         - send the reestablishment confirm to application
 *
 * @param[in]  transId    : transaction id for the reestablishment
 * @param[in]  cellId     : unique cell Id
 * @param[in]  oldCrnti   : old crnti
 * @param[in]  newCrnti   : new crnti
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamReestabReq
(
U32                          transId,
U16                          cellId,
U16                          oldCrnti,
U16                          newCrnti
)
{
   WrDamUeCb                 *ueCb;
   WrDamCellCb               *cellCb;

   if ((cellCb = wrDamGetCell(cellId)) == NULLP)
   {
      RETVALUE(RFAILED);
   }
   if (!(WR_DAM_VALID_RNTI(cellCb, oldCrnti)))
   {
      RETVALUE(RFAILED);
   }
   if (!(WR_DAM_VALID_RNTI(cellCb,newCrnti)))
   {
      RETVALUE(RFAILED);
   }
   ueCb = cellCb->ueCbs[oldCrnti - cellCb->startRnti];

   if(ueCb == NULLP)
   {
      RETVALUE(ROK);
   }

   if (wrDamChgUeId(cellCb,oldCrnti,newCrnti) != ROK)
   {

      wrIfmDamWrReestabCfm(transId, WR_DAM_CFG_FAIL);
   }
   else
   {
      ueCb = wrDamGetUe(cellId, newCrnti);
      if(ueCb == NULLP)
      {
         /* Log error */
         RLOG0(L_ERROR, "ueCb is NULL");
         RETVALUE(RFAILED);
      }
      ueCb->reestabInProgress = TRUE; /* Fix for Reest */
      wrIfmDamWrReestabCfm(transId, WR_DAM_CFG_OK);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for initializing the DAM.
 *
 * @details
 *
 *     Function: wrDamInit
 *
 *         Processing steps:
 *         - all the cells in wrDamCb are initialised to default value.
 *
 *
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamInit
(
Void
)
{
   U32                       idx;

   for (idx = 0; idx < WR_DAM_MAX_CELLS; idx++)
   {
      wrDamCb.cells[0].cellInUse = FALSE;
   }
   RETVALUE(ROK);
}

/*
 *  Fun:  wrDamActvInit 
 *
 *  Desc:    
 *
 *  Ret:  ROK   - ok
 *
 *  Notes :The task initializes its global variables.
 */
PUBLIC S16 wrDamActvInit
(
Ent                          entity,
Inst                         inst,
Region                       region,
Reason                       reason
)
{
   wrDamCb.init.procId  = SFndProcId();
   wrDamCb.init.ent     = entity;
   wrDamCb.init.inst    = inst;
   wrDamCb.init.region  = region;
   wrDamCb.init.reason  = reason;
   wrDamCb.init.cfgDone = FALSE;
   wrDamCb.init.pool    = 1;
   wrDamCb.init.acnt    = FALSE;
   wrDamCb.init.trc     = FALSE; 
   wrDamCb.init.usta    = TRUE;
   wrDamInit();
   wrDamCb.wrAppPst.srcProcId = SFndProcId();
   wrDamCb.wrAppPst.srcEnt = ENTEU;
   wrDamCb.wrAppPst.srcInst = 0;
   wrDamCb.wrAppPst.dstProcId = SFndProcId();
   wrDamCb.wrAppPst.dstEnt = ENTWR;
   wrDamCb.wrAppPst.dstInst = 0;
   wrDamCb.wrAppPst.prior = 0;
   wrDamCb.wrAppPst.route = 0;
   wrDamCb.wrAppPst.selector = 0;
   wrDamCb.wrAppPst.region = region;
   
   /* RRM_SP3 */
   wrDamCb.wrDamToRrmPst.srcEnt = ENTEU;
   wrDamCb.wrDamToRrmPst.srcProcId = SFndProcId();
   wrDamCb.wrDamToRrmPst.srcInst = 0;
   wrDamCb.wrDamToRrmPst.dstEnt = ENTRM;
   wrDamCb.wrDamToRrmPst.dstProcId = SFndProcId();
   wrDamCb.wrDamToRrmPst.dstInst = 0;
   wrDamCb.wrDamToRrmPst.selector = RMU_LWLC;
   wrDamCb.wrDamToRrmPst.region = region;
   wrDamCb.wrDamToRrmPst.pool = 0;


 RETVALUE(ROK);
} /*wrDamActvInit*/



/** @brief This function is responsible for configuring the wrDamCb with the 
 *         incoming configuration values.
 *
 * @details
 *
 *     Function: wrDamCfgReq
 *
 *         Processing steps:
 *         - update the existing values with the new values.
 *
 *
 * @param[in]  cfg : configuration values
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamCfgReq
(
LwrMngmt      *cfg
)
{
   S16 retVal;
   CmStatus cfgCfm;


   cfgCfm.status = LCM_PRIM_OK;
   cfgCfm.reason = LCM_REASON_NOT_APPL;
   retVal = ROK;

   switch (cfg->hdr.elmId.elmnt)
   {
      case STWRGEN:
      {
          LwrGenCfg *genCfg = &cfg->t.cfg.t.genCfg;
          wrDamCb.inactvTmrVal = genCfg->inactvTmrVal;
          wrDamCb.expiryCnt    = genCfg->expiryCnt;
          wrDamCb.endMrkTmrVal = genCfg->endMrkTmrVal;
          wrDamCb.timeRes = genCfg->timeRes;
          /* Allocate memory */
          WR_ALLOC_DATA_APP(&wrDamCb.egtSap, (sizeof(WrLiSapCb *) * WR_NUM_EGT_SAPS));
          if(NULLP == wrDamCb.egtSap)
          {
              RLOG0(L_FATAL, "Memory allocation failed.");
              retVal = RFAILED;
              break;
          }

          WR_ALLOC_DATA_APP(&wrDamCb.pjuSap, (sizeof(WrLiSapCb *) * WR_NUM_PJU_SAPS));
          if(NULLP == wrDamCb.pjuSap)
          {
             RLOG0(L_FATAL, "Memory allocation failed.");
              retVal = RFAILED;
              break;
           }
           wrDamRegInitTmr();
           break;
      }
      case STWREGTSAP:
      case STWRPJUSAP:
      {
         retVal = wrDamLSapCfg(cfg, &cfgCfm);
         break;
      }
      default:
      {
         cfgCfm.status = LCM_PRIM_NOK;
         cfgCfm.reason = LCM_REASON_INVALID_ELMNT;
         retVal = RFAILED;
         break;
      }
   }

   wrIfmDamWrCfgCfm(cfg, &cfgCfm);

   RETVALUE(retVal);

}/* wrDamCfgReq */

/*
 *       Fun:     wrDamLSapCfg
 * 
 *       Desc:    This function is used to configure the Lower SAP.
 * 
 *       Ret:     ROK - ok / RFAILED - Failure 
 *
 *       Notes :  None.
 */
PRIVATE S16 wrDamLSapCfg
(     
LwrMngmt                      *cfg,
CmStatus                     *status
)
{
   WrLiSapCb *sap = NULLP;
   LwrLSapCfg *lSap = &cfg->t.cfg.t.lSap;

   TRC2(wrLSapCfg);

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (NULLP == lSap)
   {
      RLOG0(L_ERROR, "Invalid Sap Cfg");
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_INVALID_PAR_VAL;
      RETVALUE(RFAILED);
   }

   switch(cfg->hdr.elmId.elmnt)
   {
#ifdef EU_DAT_APP
      case STWREGTSAP:
      {
         WR_ALLOC_DATA_APP(&wrDamCb.egtSap[0], sizeof(WrLiSapCb));
         if(NULLP == wrDamCb.egtSap[0])
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_PAR_VAL;
            RETVALUE(RFAILED);
         }
         sap = wrDamCb.egtSap[0];
         break;
      }
      case STWRPJUSAP:
      {
         WR_ALLOC_DATA_APP(&wrDamCb.pjuSap[lSap->suId], sizeof(WrLiSapCb));
         if(NULLP == wrDamCb.pjuSap[lSap->suId])
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_PAR_VAL;
            RETVALUE(RFAILED);
         }
         sap = wrDamCb.pjuSap[lSap->suId];
         break;
      }
#endif 
      default:
      {
         RLOG0(L_ERROR, "Invalid Sap ElemenT");
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_PAR_VAL;
         RETVALUE(RFAILED);
         break;
      }
   }

   sap->suId = lSap->suId;
   sap->spId = lSap->spId;
   sap->pst.srcProcId = SFndProcId();
   sap->pst.srcEnt = lSap->srcEnt;
   sap->pst.srcInst = lSap->srcInst;
   sap->pst.dstProcId = lSap->dstProcId;
   sap->pst.dstEnt = lSap->dstEnt;
   sap->pst.dstInst = lSap->dstInst;
   sap->pst.prior = lSap->dstPrior;
   sap->pst.route = lSap->dstRoute;
   sap->pst.selector = lSap->dstSel;
#ifdef SS_LOCKLESS_MEMORY
   sap->mem.region = wrDamCb.init.region;
   sap->pst.region = wrDamCb.init.region;
#else
   sap->mem.region = lSap->mem.region;
#endif
   sap->mem.pool = lSap->mem.pool;
   sap->maxBndRetry = lSap->maxBndRetry;
   sap->bndTmr.val = lSap->bndTmr.val;
   sap->bndTmr.enb = lSap->bndTmr.enb;
   sap->sapState = LWR_SAP_UNBOUND;
   cmInitTimers(&sap->timer , 1);

   RETVALUE(ROK);
} /* wrLSapCfg */

/*
 *       Fun:     wrDamCntrlReq
 *
 *       Desc:    This primitive is called by LM to issue control 
 *                requests to the DAM.
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 wrDamCntrlReq
(
LwrMngmt                      *cntrl
)
{
   S16                        retVal = ROK;
   CmStatus                   cfmStatus;    

   TRC3(wrDamCntrlReq);
   /* Initialize the status memebrs to Success */
   cfmStatus.status = LCM_PRIM_OK;
   cfmStatus.reason = LCM_REASON_NOT_APPL;
   /* Identify the element for control */
   switch (cntrl->hdr.elmId.elmnt)
   { 
      case STWRGEN: 
         {
            switch (cntrl->t.cntrl.action)
            {
               case ASHUTDOWN:
                  {
                     wrDamDeInit();
                     break;
                  }
               case AENA:
                  {
                     if(cntrl->t.cntrl.subAction == SAUSTA)
                     {
                        /* Sub Action Unsolicited Status Alarm */
                        wrDamCb.init.usta = TRUE;
                     }
                     else if (cntrl->t.cntrl.subAction == SADBG)
                     {
                     }
                     break;
                  }
               case ADISIMM:        /* Action DISABLE */
                  {
                     if(cntrl->t.cntrl.subAction == SAUSTA)
                     {
                        wrDamCb.init.usta = FALSE;
                     }
                     else if (cntrl->t.cntrl.subAction == SADBG)
                     {
			
                     }
                     break;
                  }
            }

            break;
         } /* end of  case STWRGEN */
      case STWREGTSAP:
      case STWRPJUSAP:
      {
         retVal = wrDamLSapCntrl(&cntrl->t.cntrl, &cfmStatus,
                                                cntrl->hdr.elmId.elmnt);
         break;
      }

      default:
      {
         retVal = RFAILED;
         break;
      }
   }
   if(retVal == RFAILED)
   {
      cfmStatus.status = LCM_PRIM_NOK;
      cfmStatus.reason = LCM_REASON_INVALID_ELMNT;
   }

   wrIfmDamWrCntrlCfm(&cfmStatus, cntrl);
   
   RETVALUE(retVal);

} /*wrDamCntrlReq*/

/*
 *       Fun:     wrDamLSapCntrl
 *
 *       Desc:    This function processes control request for the
 *                Lower SAP.
 *
 *       Ret:     ROK - Ok/RFAILED - Failure 
 *
 *       Notes :  This function implements the State transition
 *                for Lower SAP control requests. The function
 *                is the entry point for all control request
 *                events defined.
 *
 */
PRIVATE S16 wrDamLSapCntrl
(
LwrCntrl                      *sapCntrl,
CmStatus                     *status,
Elmnt                        elmnt
)
{
   WrLiSapCb                 *lSapCb = NULLP;
   SuId                      lSapId;
   S16                       retVal;

   retVal = ROK;
   /* Validate the parameters */
   lSapId = sapCntrl->u.sapId;

   switch (elmnt) 
   {
      case STWREGTSAP:
      {
         lSapCb = wrDamCb.egtSap[0];
         break;
      }
      case STWRPJUSAP:
      {
         lSapCb = wrDamCb.pjuSap[lSapId];
         break;
      }
      default:
      {
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_PAR_VAL;
         RETVALUE(RFAILED);
      }
   }

   /* Initialize the status members to success */
   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   switch (sapCntrl->action)
   {
      case ABND_ENA:
      {
         /* Bind Lower Sap  */
         wrDamBndLSap(lSapCb, status, elmnt);
         retVal = ROK;
         break;
      }
      case AUBND_DIS:
      {
         /* Disconnect PVC and DONT send Unbind Request */
         retVal = wrDamUbndLSap(lSapCb);
         if (ROK != retVal)
         {
            RLOG0(L_ERROR, "Invalid Receiver State in call to nuUbndLSap");
            status->status = LCM_PRIM_NOK;
            status->reason = retVal;
         }
         break;
      }
      default:
      {
         RLOG0(L_ERROR, "Invalid Action Value");
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_ACTION;
         retVal = RFAILED;
         break;
      }
   }

   RETVALUE(retVal);
} /* wrLSapCntrl */

/********************************************************************IB**
 Layer Management Incoming Primitives
*********************************************************************IB*/

/*
 *       Fun:     wrDamUbndLSap
 *
 *       Desc:    This function is invoked to unbind & disable a SAP 
 *                A disconnect is sent and the SAP state is changed
 *                to WR_SAP_CFG without sending a unbind request.
 *
 *       Ret:     ROK or LCM_REASON_INVALID_STATE
 *
 *       Notes :  This function provides run time support for the
 *                stack reinitialization procedure.
 */
PRIVATE S16 wrDamUbndLSap
(
WrLiSapCb                    *sapCb
)
{
   S16                       retVal;

   TRC2(wrDamUbndLSap);

   switch(sapCb->sapState)
   {
      case LWR_SAP_BINDING:
      {
         sapCb->sapState = LWR_SAP_UNBOUND;
         retVal = ROK;
         break;
      }
      case LWR_SAP_BOUND:
      {
         sapCb->sapState = LWR_SAP_UNBOUND;
         retVal = ROK;
         break;
      }
      default:
      {
         retVal = ROK /* LCM_REASON_INVALID_STATE */;
         break;
      }
   }

   RETVALUE(retVal);
} /* wrDamUbndLSap */

/*
 *       Fun:     wrBndLSap
 *
 *       Desc:    This function binds the lower sap.
 *
 *       Ret:     ROK   - ok
 *
 *       Notes :  This function is a state transition action handler
 *                for lower SAP ABND_ENA control request. 
 */
PRIVATE S16 wrDamBndLSap
(
WrLiSapCb                    *sapCb,
CmStatus                     *status,
Elmnt                        elmnt
)
{
   if (LWR_SAP_UNBOUND == sapCb->sapState)
   {
      status->status = LCM_PRIM_OK_NDONE;
      sapCb->sapState = LWR_SAP_BINDING;
      sapCb->bndRetryCnt = 0;

      switch(elmnt)
      {
#ifdef EU_DAT_APP
         case STWREGTSAP:
         {
            wrDamStartTmr ((PTR)sapCb, WR_TMR_EGT_SAP_BND, sapCb->bndTmr.val);
            WrIfmEgtpBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         }
         case STWRPJUSAP:
         {
            wrDamStartTmr ((PTR)sapCb, WR_TMR_PJU_SAP_BND, sapCb->bndTmr.val);
            WrIfmPdcpBndReq(&sapCb->pst, sapCb->suId, sapCb->spId);
            break;
         }
#endif 
         default:
            break;
      }
   }
   else
   {
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_INVALID_STATE;
   }


   RETVALUE (ROK);
} /* wrBndLSap */




/** @brief This function is responsible for deinitialization of Dam Function.
 *
 * @details
 *
 *     Function:wrDamDeInit 
 *
 *         Processing steps:
 *         - Function de-registers the intiated timer.
 *
 *
 * @param[in]  void
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrDamDeInit
(
)
{
   WrDamCellCb    *damcellCb = NULLP; 
   U32            sapCount = 0;
   U32            cellCount = 0;

   for(cellCount = 0; cellCount < WR_MAX_CELLS; cellCount++)
   {
      /*
      if(wrDamCb.cells[cellCount].cellInUse == TRUE)
      {
         */
         damcellCb = &(wrDamCb.cells[cellCount]);
         if(damcellCb != NULLP)
         {
            if(damcellCb->ueIdxCp.idxs != NULLP)
            {
               WR_FREE(damcellCb->ueIdxCp.idxs, sizeof(WrDamUeIdx) * damcellCb->ueIdxCp.numIdxs);
            }
            if(damcellCb->ueCbs != NULLP)
            {
               WR_FREE(damcellCb->ueCbs, damcellCb->numRntis * sizeof(WrDamUeCb *)); 
            }
         cmLListInit(&damcellCb->ueIdxCp.freeLst);
         cmLListInit(&damcellCb->ueIdxCp.inuseLst);

         }
         /*
         cmLListInit(&damcellCb->ueIdxCp.freeLst);
         cmLListInit(&damcellCb->ueIdxCp.inuseLst);
      }
      */
   }

   for(sapCount = 0; sapCount < WR_NUM_EGT_SAPS; sapCount++)
   {
      if(wrDamCb.egtSap[sapCount] != NULLP)
      {
         WR_FREE_DATA_APP(wrDamCb.egtSap[sapCount], sizeof(WrLiSapCb));
      }
   }
   WR_FREE_DATA_APP(wrDamCb.egtSap, (sizeof(WrLiSapCb *) * WR_NUM_EGT_SAPS));

   for(sapCount = 0; sapCount < WR_NUM_PJU_SAPS; sapCount++) 
   {
      if(wrDamCb.pjuSap[sapCount] != NULLP)
      {
         WR_FREE_DATA_APP(wrDamCb.pjuSap[sapCount], sizeof(WrLiSapCb));
      }
   }
   WR_FREE_DATA_APP(wrDamCb.pjuSap, (sizeof(WrLiSapCb *) * WR_NUM_PJU_SAPS));

   RETVALUE(wrDamDeRegTmr());
}

/** @brief This function is responsible for changing the state of the tunnels.
 *
 * @details
 *
 *     Function: wrDamChngTnlState
 *
 *         Processing steps:
 *         - update tunnel state
 *
 *
 * @param[in]  tnlStInfo: state to which tunnel needs to move
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrDamTnlStChngReq
(
WrDamTnlStInfo               *tnlStInfo
)
{
   U16                       cellId;
   U16                       ueIdx;
   U8                        rbId;
   WrDamTnlType              tnlType = WR_TNL_NORMAL;
   WrDamTnlCb                *tnlCb = NULLP;
   WrDamUeCb                 *ueCb = NULLP;
   WrDamDrbCb                *drbCb = NULLP;
   CmLList                   *bufNode;
   WrDamEgtpDataBuf          *egtpBufInfo;
   CmLtePdcpId               pdcpId;

   cellId  = (tnlStInfo->lclTeid & 0xff000000) >> 24;
   ueIdx   = (tnlStInfo->lclTeid & 0x00ffff00) >> 8;
   rbId    = (tnlStInfo->lclTeid & 0x000000f8) >> 3;
   tnlType = (tnlStInfo->lclTeid & 0x00000007);

   ueCb   = wrDamGetUeByUeIdx(cellId, ueIdx);
   if( ueCb == NULLP )
   {
      RETVALUE(RFAILED);
   }
   drbCb = ueCb->drbs[rbId];
   if(drbCb)
   {
      tnlCb = wrDamGetTnlCbFrmDrb(drbCb, tnlType);
   }
   if( tnlCb == NULLP)
   {
      RETVALUE(RFAILED);
   }
   switch(tnlStInfo->state)
   {
      case WR_DAM_DRB_STATE_BUFFERING:
         {
            if (drbCb->state == WR_DAM_DRB_STATE_NORMAL)
            {
               RLOG_ARG2(L_DEBUG, DBG_UEID, ueCb->ueId, "Tunnel state changed from (%d) to (%d)",
                        drbCb->state, tnlStInfo->state);
               drbCb->state = tnlStInfo->state;
            }
            if(tnlStInfo->cause == WR_DAM_HO_UE_UM_DAT_FWD)
            {
               RLOG_ARG2(L_DEBUG, DBG_UEID, ueCb->ueId,
                        "Tunnel state changed from (%d) to (%d)",
                        drbCb->normalTnl->fwdState, WR_DAM_TNL_STATE_FWD_NEW_PKTS);
               drbCb->normalTnl->fwdState = WR_DAM_TNL_STATE_FWD_NEW_PKTS;
            }
            break;
         }
      case WR_DAM_DRB_STATE_NORMAL:
         {
            /* Send the buffered packets to pdcp */
            if(drbCb->dlFwdTnl)
            {
               CM_LLIST_FIRST_NODE(&drbCb->dlFwdTnl->datBuffLst, bufNode);

               while((NULLP != bufNode) &&
                     (drbCb->dlFwdTnl->datBuffLst.count != 0))
               {
                  egtpBufInfo = (WrDamEgtpDataBuf*)bufNode->node;

                  /* remove the node from the candLst */
                  cmLListDelFrm(&drbCb->dlFwdTnl->datBuffLst, bufNode);
                  pdcpId.cellId = cellId;
                  pdcpId.rbId   = rbId;
                  pdcpId.rbType = CM_LTE_DRB;
                  pdcpId.ueId    = ueCb->ueId;

                  /* Updating the sduCntr and passing to pdcp             */
                  drbCb->normalTnl->sduIdCntr++;
                  WrIfmPdcpDatReq(&pdcpId, drbCb->normalTnl->sduIdCntr,
                        drbCb->tnlPriority, egtpBufInfo->data);

                  SPutSBuf(0, 0, (Data *)egtpBufInfo, sizeof(WrDamEgtpDataBuf));
                  CM_LLIST_NEXT_NODE(&drbCb->dlFwdTnl->datBuffLst, bufNode);
               }

            }
            /* HO_REEST */
            if((tnlStInfo->cause == WR_DAM_HO_END_MARKER_RCVD) ||
               (tnlStInfo->cause ==WR_DAM_SRC_HO_FAILURE))
            {
               CM_LLIST_FIRST_NODE(&drbCb->normalTnl->datBuffLst, bufNode);

               while((NULLP != bufNode) &&
                     (drbCb->normalTnl->datBuffLst.count != 0))
               {
                  egtpBufInfo = (WrDamEgtpDataBuf*)bufNode->node;

                  /* remove the node from the candLst */
                  cmLListDelFrm(&drbCb->normalTnl->datBuffLst, bufNode);

                  pdcpId.cellId = cellId;
                  pdcpId.rbId   = rbId;
                  pdcpId.rbType = CM_LTE_DRB;
                  pdcpId.ueId    = ueCb->ueId;

                  /* Updating the sduCntr and passing to pdcp             */
                  drbCb->normalTnl->sduIdCntr++;
                  WrIfmPdcpDatReq(&pdcpId, drbCb->normalTnl->sduIdCntr,
                        drbCb->tnlPriority, egtpBufInfo->data);

                  SPutSBuf(0, 0, (Data *)egtpBufInfo, sizeof(WrDamEgtpDataBuf));
                  CM_LLIST_NEXT_NODE(&drbCb->normalTnl->datBuffLst, bufNode);
               }
               drbCb->normalTnl->fwdState = WR_DAM_TNL_STATE_FWD_INITIAL;
               RLOG_ARG2(L_INFO, DBG_UEID, ueCb->ueId, 
                        "Tunnel state changed from (%d) to (%d)",
                        drbCb->state, tnlStInfo->state);
               drbCb->state = tnlStInfo->state;
            }
            break;
         }
      case WR_DAM_TNL_DL_FWDSTATE_NEW:
         {
            if(drbCb->normalTnl->fwdState != WR_DAM_TNL_STATE_FWD_NEW_PKTS)
            {
               RLOG_ARG2(L_DEBUG, DBG_UEID, ueCb->ueId,
                        "Tunnel state changed from (%d) to (%d)",
                        drbCb->normalTnl->fwdState, WR_DAM_TNL_STATE_FWD_NEW_PKTS);
               drbCb->normalTnl->fwdState = WR_DAM_TNL_STATE_FWD_NEW_PKTS;
            }
            break;
         }
      default:
         {
            RLOG_ARG2(L_ERROR, DBG_UEID, ueCb->ueId,
                     "Invalid state changed request (%d) to (%d)",
                     drbCb->normalTnl->fwdState, tnlStInfo->state);
            break;
         }
   }
   RETVALUE(ROK);
}

/** @brief This function handles the incoming Uplink PDCP data indication.
 *
 * @details
 *
 *     Function: wrDamPdcpDatFwdInd
 *
 *         Processing steps:
 *         - fetch the tunnel Cb
 *         - fetch the ueCb
 *         - set the dataRcvd flag to TRUE
 *         - derive the tunnel id from the cellId, ueId and the rbId
 *         - invoke wrFillEgtpDatMsg to fill the egtp event structure
 *         - invoke WrIfmEgtpEguDatReq to send the data packet to egtp
 *
 *
 * @param[in]  pdcpId : pdcp info having cell and ue info
 * @param[in]  mBuf   : uplink data packet
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrDamPdcpDatFwdInd
(
Pst                          *pst,
CmLtePdcpId                  *pdcpId,
PjuDatFwdIndInfo             *datFwdInd
)
{
   EgtUEvnt                  *eguEvtMsg;
   WrDamTnlCb                *tnlCb = NULLP;
   WrDamUeCb                 *ueCb = NULLP;
   EgUMsg                    *egMsg;
   WrDamTnlType              tnlType;
   WrDamDrbCb                *drbCb = NULLP;
   PjuDatFwdInfo             *datFwdInfo;
   EgUMsgHdr                 *eguHdr;
   CmLList                   *bufNode;
   U16                       idx;
   WrDamEgtpDataBuf          *egtpBufInfo;
   Buffer                    *tempMBuf;
   S16                       ret;

   RLOG0(L_DEBUG, "Received Data Forward indication for UEID");
   /* Fill EGTP Data Message */
   ueCb = wrDamGetUe(pdcpId->cellId, pdcpId->ueId);
   if( ueCb == NULLP )
   {
      if(datFwdInd->numSdus)
      {
         for ( idx = 0; idx < datFwdInd->numSdus; idx++ )
         {
           SPutMsg(datFwdInd->datFwdInfo[idx].sdu);
         }
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd->datFwdInfo,\
               (sizeof(PjuDatFwdInfo)* datFwdInd->numSdus));
      }
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd,\
            sizeof(PjuDatFwdIndInfo));
      RETVALUE(RFAILED);
   }

   drbCb = ueCb->drbs[pdcpId->rbId];
   if(drbCb)
   {
      if(datFwdInd->dir == CM_LTE_DIR_DL)
      {
         tnlType = WR_TNL_DL_DATFWD;
      }
      else
      {
         tnlType = WR_TNL_UL_DATFWD;
      }
      tnlCb = wrDamGetTnlCbFrmDrb(drbCb, tnlType);
   }
   if( tnlCb == NULLP )
   {
      if(datFwdInd->numSdus)
      {
         for ( idx = 0; idx < datFwdInd->numSdus; idx++ )
         {
           SPutMsg(datFwdInd->datFwdInfo[idx].sdu);
         }
         SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd->datFwdInfo,\
               (sizeof(PjuDatFwdInfo)* datFwdInd->numSdus));
      }
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd,\
            sizeof(PjuDatFwdIndInfo));
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < datFwdInd->numSdus; idx++)
   {
      eguEvtMsg = NULLP;
      datFwdInfo = &datFwdInd->datFwdInfo[idx];
      /* fill the egtp packet */
      ret = wrFillEgtpDatMsg(tnlCb, &eguEvtMsg, EGT_GTPU_MSG_GPDU);
      if(ret != ROK)
      {
         /* Log error */
         RLOG0(L_ERROR, "wrFillEgtpDatMsg Failed for eguEvtMsg");
         RETVALUE(RFAILED);
      }
      egMsg = eguEvtMsg->u.egMsg;
      eguHdr = &egMsg->msgHdr;
      /* Need to verify HO type, if the HO type
         is HO to UTRAN/GERAN then no need to include
         the sn info */
      if(( ueCb->hoType != WR_HO_TYPE_IRAT_UTRA ) &&
         ( ueCb->hoType != WR_HO_TYPE_IRAT_GERAN))
      {
         eguHdr->extHdr.pdcpNmb.pres = TRUE;
         eguHdr->extHdr.pdcpNmb.val  = datFwdInfo->sn;
      }
      if(datFwdInfo->sduId)
      {
         eguHdr->seqNumber.pres = TRUE;
         eguHdr->seqNumber.val = datFwdInfo->sduId;
      }
      /*  Data Buffering Memory Exhaustion work around:
      *   Don't maintain a copy, just forward the packets
      */
      egMsg->u.mBuf = datFwdInfo->sdu;

      /* Trigger EGTP Data Req */
      WrIfmEgtpEguDatReq(eguEvtMsg);
   }

   /* free the data structures */
   if(datFwdInd->numSdus)
   {
      SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd->datFwdInfo,\
            (sizeof(PjuDatFwdInfo)* datFwdInd->numSdus));
   }

   /* set the normal tunnel state to WR_DAM_TNL_FWD_STATE_NEW_PKTS so that the 
    * fresh packets from SGW is forwarded as when received */
   if((datFwdInd->isLastDatFwdInd == TRUE) && (datFwdInd->dir == CM_LTE_DIR_DL))
   {
      drbCb->normalTnl->fwdState = WR_DAM_TNL_STATE_FWD_NEW_PKTS;
      /* forward fresh packets received from SGW (stored in normal tunnel) */
      CM_LLIST_FIRST_NODE(&drbCb->normalTnl->datBuffLst, bufNode);
      while(bufNode)
      {
         tempMBuf = NULLP;
         eguEvtMsg = NULLP;
         egtpBufInfo = (WrDamEgtpDataBuf*)bufNode->node;
         /* fill the egtp packet */
         wrFillEgtpDatMsg(tnlCb, &eguEvtMsg, EGT_GTPU_MSG_GPDU);

         if(eguEvtMsg == NULLP)
         {
            RLOG0(L_ERROR, "Failed to get eguEvtMsg.");
            RETVALUE(RFAILED);
         }

         egMsg = eguEvtMsg->u.egMsg;
         /* Create a copy of the data buffer and forward the packet */
         SCpyMsgMsg(egtpBufInfo->data, 0, 0, &tempMBuf);
         egMsg->u.mBuf = tempMBuf;
         /* Trigger EGTP Data Req */
         WrIfmEgtpEguDatReq(eguEvtMsg);

         CM_LLIST_NEXT_NODE(&drbCb->normalTnl->datBuffLst, bufNode);
      }
   }
   SPutSBuf(pst->region, pst->pool, (Data *)datFwdInd,\
         sizeof(PjuDatFwdIndInfo));

   RETVALUE(ROK);
}

/** @brief This function handles the various timer expiry events for DAM
 *         module.
 *
 * @details
 *
 *     Function: wrDamHdlTmrExp
 *
 *         Processing steps:
 *         - handle the timer expiry for the corresponding event
 *
 * @param[in]  event : Timer Event
 * @param[in]  ueCb  : DAM UE Control Block
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamHdlTmrExp
(
S16                          event,
WrDamUeCb                    *ueCb
)
{
   WrDamTnlStInfo            *tnlInfo;
   U16                       rbIdx = 0;
   U8                        rbId;
   WrDamDrbCb                *rbInfo;

   RLOG_ARG0(L_DEBUG, DBG_UEID, ueCb->ueId,
            "End Marker Timer expired UEID ");

   switch(event)
   {
      case WR_DAM_TMR_END_MARKER_TMR:
         if (ueCb->numDrbs == 0)
         {
            RETVALUE(ROK);
         }
         for(rbIdx = 0; rbIdx < WR_DAM_MAX_DRBS; rbIdx++)
         {
            rbInfo = ueCb->drbs[rbIdx];
            if(rbInfo)
            {
               /* 0514_END_MRKR_CHANGE */
               if(rbInfo->state != WR_DAM_DRB_STATE_BUFFERING)
               {
                  continue;
               }
               rbId = (rbInfo->normalTnl->locTeId & 0x000000f8) >> 3;

               RLOG_ARG1(L_INFO, DBG_UEID, ueCb->ueId,
                        "Changing tunnel state of DRB (%d)",
                        rbId);

               /* Change tunnel state */
               WR_ALLOC_DATA_APP(&(tnlInfo), sizeof(WrDamTnlStInfo));
               if (tnlInfo == NULLP)
               {
                  /* Log the error through an error debug */
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               tnlInfo->transId = 0;
               tnlInfo->state = WR_DAM_DRB_STATE_NORMAL;
               tnlInfo->cause = WR_DAM_HO_END_MARKER_RCVD;
               tnlInfo->lclTeid = rbInfo->normalTnl->locTeId;
               wrDamTnlStChngReq(tnlInfo);
               WR_FREE_DATA_APP((tnlInfo), sizeof(WrDamTnlStInfo));

               /* delete the forwarding tunnels corresponding to this RB */
               if(NULLP != rbInfo->dlFwdTnl) /* ccpu00133863 */
               {
                  wrDamDelTunnel(ueCb->cellId, ueCb->ueId, rbId,
                        WR_TNL_DL_DATFWD);
               }
               if(NULLP != rbInfo->ulFwdTnl) /* ccpu00133863 */
               {
                  wrDamDelTunnel(ueCb->cellId, ueCb->ueId, rbId,
                        WR_TNL_UL_DATFWD);
               }
            }
         }
         ueCb->isFullCfg = FALSE; /* RRC-REL10-Upgrade */
         break;
      default:
         RLOG0(L_ERROR, "wrDamHdlTmrExp: Invalid Timer Event");
         RETVALUE(RFAILED);
         break;
   }

   RETVALUE(ROK);
}
/* HO_REEST */
/** @brief This function is called when RRC Re-establishment received from UE at
 * source(both S1 and X2) during HO. It will delete the UL/DL Fowarding tunnels
 * and set the state of DrbCn to NORMAL
 * @details
 *
 *     Function: wrDamHdlHoFailure
 *
 *         Processing steps:
 *         - handle the start timer indication
 *
 * @param[in]  ueCb  : DAM UE Control Block
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamHdlHoFailure
(
U16                          cellId,
U16                          rnti
)
{
   WrDamUeCb                 *damUeCb = NULLP;
   WrDamDrbCb                *drbCb=NULLP;
   U8                        rbId=0;
   U8                        drbIdx=0;
   WrDamTnlStInfo            *tnlInfo;

   damUeCb = wrDamGetUe(cellId, rnti);
   if(NULLP != damUeCb)
   {
      /*Change the Tunnel State to NORMAL for all normal tunnels
       * Delete the fowarding tunnels if any */
      for(drbIdx=0; drbIdx<WR_DAM_MAX_DRBS; drbIdx++)
      {
         drbCb = damUeCb->drbs[drbIdx];
         if(NULLP != drbCb)
         {
            rbId = (drbCb->normalTnl->locTeId & 0x000000f8) >> 3;
            /* delete the forwarding tunnels */
            if(NULLP != drbCb->dlFwdTnl)
            {
               ALARM("In wrDamHdlHoFailure:Deleting WR_TNL_DL_DATFWD tunnel for rbId=%d\n", rbId);
               wrDamDelTunnel(damUeCb->cellId, damUeCb->ueId, rbId,
                     WR_TNL_DL_DATFWD);
            }
            if(NULLP != drbCb->ulFwdTnl)
            {
               ALARM("\nIn wrDamHdlHoFailure: Deleting WR_TNL_UL_DATFWD tunnel for rbId=%d\n",
                     rbId);
               wrDamDelTunnel(damUeCb->cellId, damUeCb->ueId, rbId,
                     WR_TNL_UL_DATFWD);
            }
            ALARM("\nIn wrUmmDefS1SrcHoIncTrans: drbCb->state =%d\n",drbCb->state);
            /* Change the tunnel states: which intern takes care of
               forwarding buffered DL packets to L2  */
            WR_ALLOC_DATA_APP(&(tnlInfo), sizeof(WrDamTnlStInfo));
            if(tnlInfo == NULLP)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            tnlInfo->transId = 0;
            tnlInfo->state = WR_DAM_DRB_STATE_NORMAL;
            tnlInfo->cause = WR_DAM_SRC_HO_FAILURE;
            tnlInfo->lclTeid = drbCb->normalTnl->locTeId;
            wrDamTnlStChngReq(tnlInfo);
            WR_FREE_DATA_APP((tnlInfo), sizeof(WrDamTnlStInfo));
            ALARM("\nIn wrUmmDefS1SrcHoIncTrans State set to Normal: drbCb->state =%d\n",drbCb->state);
         }
      }
   }
   else
   {
      ALARM("\n****ALARM*****No Dam Ue Cb found for cellId=%d and Crnti=%d\n",cellId,rnti);
   }
   RETVALUE(ROK);
}/* End wrDamHdlHoFailure */

/* sri - HOFIX */
/** @brief This function starts the end marker timer for the corresponding UE.
 *
 * @details
 *
 *     Function: wrDamStartEndMrkrTmr
 *
 *         Processing steps:
 *         - handle the start timer indication
 *
 * @param[in]  ueCb  : DAM UE Control Block
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamStartEndMrkrTmr
(
U16                          cellId,
U16                          ueId
)
{
   WrDamUeCb                 *ueCb = NULLP;

   ueCb = wrDamGetUe(cellId, ueId);
   if(ueCb == NULLP)
   {
      RLOG0(L_ERROR, "UE CB not found UE ID.");
      RETVALUE(RFAILED);
   }

   RLOG_ARG0(L_DEBUG, DBG_UEID, ueCb->ueId,
            "Starting End Marker timer for UE");
   wrDamStartTmr((PTR)ueCb, WR_DAM_TMR_END_MARKER_TMR, wrDamCb.endMrkTmrVal);

   RETVALUE(ROK);
}


/** @brief This function starts the end marker timer for the corresponding UE.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - handle the start timer indication
 *
 * @param[in]  ueCb  : DAM UE Control Block
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrDamStopUeTmr 
(
U16                          cellId,
U16                          ueId
)
{
   WrDamUeCb                 *ueCb = NULLP;

   ueCb = wrDamGetUe(cellId, ueId);
   if(ueCb == NULLP)
   {
      RLOG0(L_ERROR, "UE CB not found.");
      RETVALUE(RFAILED);
   }

   RLOG_ARG0(L_DEBUG,DBG_UEID,(U32)ueCb->ueId,
          "Stopping the  timer for UE due to UE Inactivity");

   wrDamStopTmr((PTR)ueCb,WR_DAM_TMR_INACTIVITY);

   RETVALUE(ROK);
}

/** @brief This function is used to print UL/DL throughput information
 *
 * @details
 *
 *     Function: wrDamPrintThroughput
 *
 *         Processing steps:
 *         - 
 *
 *
 * @return S16
 *    -#Success : ROK
 */
PUBLIC Void wrDamPrintThroughput (Void)
{
   WrDamUeCb                 *ueCb;
   WrDamUeIdx                *ueIdx;
   WrDamUeIdxCp              *idxCp = &wrDamCb.cells[0].ueIdxCp;
   CmLList                   *ueLst;
   double                    ulTtlTpt = 0;
   double                    dlTtlTpt = 0;
   char                      logBufTemp[5000];
   int                       len = 0;
   ueLst = idxCp->inuseLst.first;
   if(ueLst == NULLP)
   {
      RETVOID;
   }
   strcpy(logBufTemp, "ueIdx\tueId\tulTpt\tdlTpt\n");
   len = strlen(logBufTemp);
   while(ueLst)
   {
      ueIdx = (WrDamUeIdx *)(ueLst->node);
      ueCb = ueIdx->ueCb;
      if (ueCb == NULLP)
      continue; 
      len += sprintf(logBufTemp+len, "%d\t%d\t%.2f(Mbps)\t%.2f(Mbps)\n", 
            ueCb->ueIdx, ueCb->ueId, (float)(ueCb->ulTpt*8)/(1000*1000*30), 
            (float)(ueCb->dlTpt*8)/(1000*1000*30));
      ulTtlTpt += ueCb->ulTpt;
      dlTtlTpt += ueCb->dlTpt;
      ueCb->ulTpt = 0;
      ueCb->dlTpt = 0;
      ueLst = ueLst->next;
   }

#ifndef SS_RBUF
   len += sprintf( logBufTemp+len, "ulTtlTpt=%.2f Mbps, dlTtlTpt=%.2f Mbps, numActUe=%lu\n", 
         (ulTtlTpt*8)/(1000*1000*30) , (dlTtlTpt*8)/(1000*1000*30), idxCp->inuseLst.count);


   len += sprintf(logBufTemp+len, "%lu : %lu",totalIcpuPkts, droppedIcpuPkts);
   //RLOG_STR(L_ALWAYS, "THROUGHPUT DATA: %s", logBufTemp);
   printf("THROUGHPUT DATA: %s", logBufTemp);
   /* reset the counters */
   totalIcpuPkts = droppedIcpuPkts = 0;
#else   
      extern U32 pktCntegtp;
      extern U32 pktDropategtp;
      extern U32 bc_alloc, bc_free;
      extern U32 packtsToSgw;
      extern U32 failToSendPkts;
      len+=sprintf(logBufTemp+len,"\nIcpuRate=[%ld] IcpuDropRate=[%ld] PktCount@EGTP:[%lu] PktsDroped@EGTP:[%lu]\n", 
		      ((SsRngInfoTbl[0].pktRate)/30),((SsRngInfoTbl[0].nWriteFail)/30), pktCntegtp,pktDropategtp);
      printf("\nIcpuRate=[%ld] IcpuDropRate=[%ld] PktCount@EGTP:[%lu] PktsDroped@EGTP:[%lu]\n", 
		      ((SsRngInfoTbl[0].pktRate)/30),((SsRngInfoTbl[0].nWriteFail)/30), pktCntegtp,pktDropategtp);
      len+=sprintf(logBufTemp+len,"\nbc_alloc:[%lu] bc_free:[%ld] diff:[%ld]\n", bc_alloc, bc_free,bc_alloc-bc_free);
      printf("bc_alloc:[%lu] bc_free:[%ld] diff:[%ld]\n", bc_alloc, bc_free,bc_alloc-bc_free);
      printf("packtsToSgw [%lu] failToSendPkts [%lu]\n", packtsToSgw, failToSendPkts);
      SsRngInfoTbl[0].pktRate = 0;//DL icpu Packet Rate
      SsRngInfoTbl[0].nWriteFail = 0;// Packet Drop Count
      pktCntegtp = 0;
      pktDropategtp = 0;
      packtsToSgw   = 0;
      failToSendPkts = 0;
      len+=sprintf(logBufTemp+len,"\nL_TPT=[%.2fMbps], DL_TPT=[%.2fMbps], Active UEs=[%lu]\n", 
		      (ulTtlTpt*8)/(1000*1000*30) , (dlTtlTpt*8)/(1000*1000*30), idxCp->inuseLst.count);
      printf("\nUL_TPT=[%.2fMbps], DL_TPT=[%.2fMbps], Active UEs=[%lu]\n", 
         (ulTtlTpt*8)/(1000*1000*30) , (dlTtlTpt*8)/(1000*1000*30), idxCp->inuseLst.count);

      //RLOG_STR(L_ALWAYS, "THROUGHPUT DATA: \n%s", logBufTemp);
#endif
      RLOG3(L_ALWAYS,"THROUGHPUT DATA: ulTtlTpt=%ld Mbps, dlTtlTpt=%ld Mbps, numActUe=%ld", 
		      (ulTtlTpt*8)/(1000*1000*30) , (dlTtlTpt*8)/(1000*1000*30), idxCp->inuseLst.count);
      RETVOID;
}
PUBLIC U32 wrDamGetActiveUeCnt(U32 cellId)
{
  if(cellId > 0)
  {
    /*127802*/
     RLOG0(L_INFO, "Only Single cell with CellId Zero[0] is supported ");
    RETVALUE(ROK); /*send Zero as the active UE count*/
  }

  RETVALUE(wrDamCb.cells[cellId].ueIdxCp.inuseLst.count);
}


/** @brief This function handles the incoming EGTP data packets and forwards
 *         them to PDCP.
 *
 * @details
 *
 *     Function: wrDamEgtpDatInd
 *
 *         Processing steps:
 *         - derive the cellId, ueId, rbId from the teId of the incoming
 *           message and fill the CmLtePdcpId structure
 *         - get the tunnelCb
 *         - get the ueCb
 *         - set the dataRcvd flag to TRUE
 *         - invoke WrIfmPdcpDatReq to send the packet to PDCP.
 *
 *
 * @param[in]  eguMsg : incoming egtp message
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
#ifdef TENB_FP
PUBLIC S16 wrDamEgtpFpDatInd
(
U32                    lclTeid,
Buffer                 *mBuf
)
{
   CmLtePdcpId               pdcpId;
   WrDamTnlCb                *tnlCb = NULLP;
   WrDamTnlCb                *nrmlTnlCb;
   WrDamUeCb                 *ueCb = NULLP;
   WrDamDrbCb                *drbCb = NULLP;
   U16                       ueIdx;
   WrDamTnlType              tnlType;
   Bool                      pktDiscard = FALSE;
   Bool                      fwdedOldPkt = FALSE;
   WrDamEgtpDataBuf          *egtpBufInfo = NULLP;
   EgtUEvnt                  *eguEvtMsg;
   EgUMsg                    *egMsg;
   U8                        msgType = EGT_GTPU_MSG_GPDU;
   Buffer                    *tempMBuf;
   CmLteCellId               cellId;      /*!< Cell ID */
   CmLteRbId                 rbId;        /*!< PDCP Instance ID */
   MsgLen                    dlTpt = 0;

   cellId = (lclTeid & 0xff000000) >> 24;
   ueIdx  = (lclTeid & 0x00ffff00) >> 8;
   rbId   = (lclTeid & 0x000000f8) >> 3;
   tnlType= (lclTeid & 0x00000007);

   ueCb  = wrDamGetUeByUeIdx(cellId, ueIdx);
   if (ueCb == NULLP)
   {
      RLOG0(L_ERROR, "ueCb not found");
      RETVALUE(RFAILED);
   }
   /* Allow the packet only if UE is not in Re-Establsihment State         */
   /*   Otherwise drop the GTP-U PDU                                         */
   if (ueCb->reestabInProgress == FALSE)
   {
      drbCb = ueCb->drbs[rbId];
      if(drbCb)
      {
         tnlCb = wrDamGetTnlCbFrmDrb(drbCb, tnlType);
      }
      if( tnlCb == NULLP )
      {
         RLOG0(L_ERROR, "tnlCb not found");
         RETVALUE(RFAILED);
      }
      ueCb->dataRcvd = TRUE;
      if(drbCb->state == WR_DAM_DRB_STATE_NORMAL)
      {
         /* Find out the DL througput */
         SFndLenMsg(mBuf, &dlTpt);
         ueCb->dlTpt += dlTpt;
         /* For now, excluding HO, all we need to do is construct the PDCP id   */
         /* from the TEID in the message received                               */
         pdcpId.ueId    = ueCb->ueId;
         pdcpId.cellId = cellId;
         pdcpId.rbId   = rbId;
         pdcpId.rbType = CM_LTE_DRB;
         /* Updating the sduCntr and passing to pdcp                            */
         tnlCb->sduIdCntr++;
         WrIfmPdcpDatReq(&pdcpId, tnlCb->sduIdCntr, drbCb->tnlPriority, mBuf);
      }
      else
      {
         nrmlTnlCb = wrDamGetTnlCbFrmDrb(drbCb, WR_TNL_NORMAL);
         /* if sequence number field is not present, then buffer the packet,
            else send to PDCP */
         if(msgType == EGT_GTPU_MSG_GPDU)
         {
            /* check if pkt storage limit is execeeded */
            if(WR_DAM_BUFFERING_PKT_LIMIT == (tnlCb->datBuffLst.count + 1))
            {
               pktDiscard = TRUE;
            }

            fwdedOldPkt = FALSE;

            /* store the pkt in tht buff list */
            if(pktDiscard == FALSE && fwdedOldPkt == FALSE)
            {
               /* add to dataBuff */
               WR_ALLOC_DATA_APP(&egtpBufInfo, sizeof(WrDamEgtpDataBuf));
               if(egtpBufInfo == NULLP)
               {
                  /* Log error */
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               egtpBufInfo->data = mBuf;
               cmLListAdd2Tail(&(tnlCb->datBuffLst), &(egtpBufInfo->lnk));
               egtpBufInfo->lnk.node = (PTR)egtpBufInfo;
#ifndef BRDCM_BUGFIX
               /*127802*/
               RLOG2(L_DEBUG, "buffering packet... DatFwd State [%d] TnlId [%lu]",
                     tnlCb->fwdState, tnlCb->locTeId);
#endif
            }
         }
         else
         {
         }
         if(tnlCb->fwdState == WR_DAM_TNL_STATE_FWD_NEW_PKTS)
         {
           if(ROK == wrFillEgtpDatMsg(drbCb->dlFwdTnl, &eguEvtMsg, msgType))
           {
             if(msgType == EGT_GTPU_MSG_GPDU)
             {
               egMsg = eguEvtMsg->u.egMsg;
               if(pktDiscard == FALSE)
               {

                  if(egtpBufInfo == NULLP)
                  {
                      /* Log error */
                      RLOG0(L_ERROR, "egtpBufInfo is NULL");
                      RETVALUE(RFAILED);
                  }

                  /* Create a copy of the data buffer and forward the packet */
                  SCpyMsgMsg(egtpBufInfo->data, 0, 0, &tempMBuf);
                  egMsg->u.mBuf = tempMBuf;
               }
               else
               {
                  egMsg->u.mBuf = mBuf;
               }
             }
             /* Trigger EGTP Data Req */
             WrIfmEgtpEguDatReq(eguEvtMsg);
           }
         }
         /* discard the pdu if the pktDiscard flag is set to true */
         if(pktDiscard == TRUE)
         {
           SPutMsg(mBuf);
         }
      }
   }
   else
   {
     static U32 cnt = 0;
     /* Currently Drop the PDU is UE is in Re-Establishment state  */
     /* Freeing EgtUEvnt structure                                 */
     cnt++;
     if ((cnt % 1024) == 0)
     {
         RLOG1(L_ERROR, "wrFillEgtpDatInd: Dropping the PDU "
                  "as UE in Re-establishment state: %ld", cnt);
     }

     SPutMsg(mBuf);

   }

   RETVALUE(ROK);
}
#endif

/** @brief This function is used to set dataRcvd flag to TRUE for some siganlling scenarios
 *
 * @details
 *
 *     Function: wrDamSetDataRcvdFlag
 *
 *         Processing steps:
 *         -
 *
 *
 * @return S16
 *    -#Success : ROK
 */
PUBLIC Void wrDamSetDataRcvdFlag (U16 cellId, U16 ueId)
{
   WrDamUeCb  *ueCb=NULLP;

   ueCb = wrDamGetUe(cellId, ueId);
   if(ueCb)
   {
      ueCb->dataRcvd = TRUE;
   }
   else
   {
      RLOG1(L_DEBUG, "wrDamSetDataRcvdFlag ueCb is NULL for UeId (%d)\n",
            ueId);
      RETVOID;
   }
}
#ifdef DAM_PDCP_OPT
PUBLIC S16 wrDamFbDatInd
(
U8            *tmpBuf,
U8            *buf,
U32            bufLen       /* bufLen */
)
{
   FlatBuffer     fbuf;
   U16 CellId = tmpBuf[0]; 
   U16 ueIdx = tmpBuf[1] << 8 | tmpBuf[2];
   U16 rbId = (tmpBuf[3] & 0x000000f8) >> 3;
   WrDamCellCb    *cellCb =NULLP;
   WrDamUeCb      *ueCb = NULLP;

   if (CellId == 0 ||(cellCb = wrDamGetCell(CellId)) == NULLP)
   {
      RETVALUE(RFAILED);
   }  
   ueCb= cellCb->ueIdxCp.idxs[ueIdx].ueCb;

   if((ueCb) && (ueCb->drbs[rbId]) && ((tmpBuf[3] & 0x7) == WR_TNL_NORMAL) && 
      (ueCb->drbs[rbId]->normalTnl) && (ueCb->drbs[rbId]->state == WR_DAM_DRB_STATE_NORMAL))
   {
      ueCb->dlTpt += bufLen - 8;
      ueCb->dataRcvd = TRUE;
   }
   else 
   {
      RETVALUE(RFAILED);
   }
   tmpBuf += 4;
   fbuf.startAddr = buf;
   fbuf.len       = bufLen - 8;
   fbuf.ptr       = tmpBuf;

   PjUiPjuDatReqFBPdcpOpt(CellId,ueCb->ueId,rbId,ueCb->drbs[rbId]->normalTnl->sduIdCntr++, &fbuf);

   RETVALUE(ROK);     
}
#endif 

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
