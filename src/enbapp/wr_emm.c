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
               

     File:     wr_emm.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/


/* header include files (.h) */
static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=3;

#include "wr.h"
#include "wr_cmn.h"
#include "wr_emm.h"
#include "wr_smm_smallcell.h"
#include "rgr.x"
#include "wr_emm_mme.h"
#include "wr_emm_pbm.h"
#include "wr_utils.h"
#include "wr_umm_meas_fun.h"
#include "wr_emm_s1_flex.h"
#include "wr_emm_rim.h"
#include "wr_emm_rim_app.h"
#include "wr_kpi.h"

#define WR_UMM_NUMOF_MSECS_PERDAY            86400000
#define WR_UMM_FIRST_EPOC_INTERVAL          4
#define WR_UMM_ONE_TO_FIVE_EPOC_INTERVAL    2
#define WR_UMM_ABOVE_FIVE_EPOC_INTERVAL     1
#define PRINT_STR(x) #x

/* CSG_DEV */
U16   wrPciRange[14] = {4, 8, 12, 16, 24, 32, 48, 64,84, 96, 128,
                        168, 252, 504};

EXTERN U32 numTtiTicks;
PUBLIC WrCb                  wrCb;
PUBLIC WrEmmCb               wrEmmCb;
EXTERN U32 wrUmmNumUeCnt;

EXTERN S16 wrEmmS1FlexInitS1FlexVarForEmmCb(void);
PUBLIC U32 wrEmmGetS1PrepTmrVal(void);
PUBLIC U32 wrEmmGetS1OvrAllTmrVal(void);
PUBLIC WrNrUtranFreqCb* wrEmmAnrGetFreqNode
(
U16                           arfcnDl,
WrNrUtranCb                   *utranCb
);

PUBLIC S16 wrEmmGetDatAppAddr
(
CmTptAddr                    **addr,
U8                           type
)
{
   if(wrEmmCb.datAppAddr[WR_IPV4_IDX].type == type)
   {
      *addr = &wrEmmCb.datAppAddr[WR_IPV4_IDX];
   }
   else if(wrEmmCb.datAppAddr[WR_IPV6_IDX].type == type)
   {
      *addr = &wrEmmCb.datAppAddr[WR_IPV6_IDX];
   }
   RETVALUE(ROK);
}
EXTERN S16 wrEmmGetTai
(
WrUeCb *ueCb,
WrTai  *tai
);
EXTERN S16 wrEmmGetCgiFromUeCb
(
 WrUeCb                     *ueCb,
 WrCgi                        *cgi
);
EXTERN S16 wrEmmInitUeGetCgi
(
 WrUeCb *ueCb, 
 WrCgi *cgi
);

PUBLIC S16 wrEmmGetNeighUtraSIInfo
(
WrUmmRedirectInfo       *redirectInfo
);

PUBLIC CONSTANT S8* wrEmmPrintCellState
(
 U8 state
);

/*
 *       Fun:     wrEmmCheckPRem
 *
 *       Desc:    This function will check whether any priodic REM request pending from OAM
 *
 *       Ret:     ROK  
 *
 *       Notes :  None.
 */

PUBLIC S16 wrEmmCheckPRem
(
)
{
   if(smCb.prem == TRUE)
   {
      if (wrUmmNumUeCnt == 1)
      {
         smBuildShutRestart();
      }
   }
   RETVALUE(ROK);
}
/*
 *       Fun:     wrEmmSetupS1
 *
 *       Desc:    This primitive is called by SM to request
 *                EMM to invoke S1 Setup with the configured 
 *                MMEs.
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 wrEmmSetupS1
(
WrMmeId                      mmeId
)
{
   wrEmmMmeSetup (mmeId);
   RETVALUE(ROK);
}
/*
 *       Fun:     wrEmmUpdateCellState 
 *
 *       Desc:    This primitive is called by UMM module
 *                when a cell State is to be changed at ummCb.
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 wrEmmUpdateCellState 
(
WrCellId                    cellId,
WrCellState                 state
)
{
   WrCellCb                  *cellCb;
  
   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for"
                "updating cell state");
      RETVALUE(RFAILED);
   }
   cellCb->cellState = state;
   RETVALUE(ROK);
}


/*
 *       Fun:     wrEmmResetCell
 *
 *       Desc:    This primitive is called by management module
 *                when a cell is to be Reset to the eNB.
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 wrEmmResetCell
(
WrCellId                     cellId
)
{

   WrCellCb                  *cellCb;
   /*
     1.Update the Wr state to CELL RESET 
     1.Update the Broadcast Message to Cell Barred
     2.Send S1AP Reset to Release the UE and also to update the Ue context at MME Side
     3.Delete the Cell From the lower Layers 
     */
   /* Need to send Update to Broadcast */

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for Cell"
                "reset");
      RETVALUE(RFAILED);
   }
/* Check if the State is Already RESET if So Return ROK */
   if((WR_CELL_STATE_RESET == cellCb->cellState) || (WR_CELL_STATE_INIT == cellCb->cellState))
   {
      RLOG_STR(L_INFO,"Cell State is %s",wrEmmPrintCellState(cellCb->cellState));
      RETVALUE(ROK);
   }

   cellCb->cellState = WR_CELL_STATE_RESET;
   /* Update the WrummCellCb state here */
   if(ROK != wrUmmUpdateCellState(cellId,WR_CELL_STATE_RESET))
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Updation of Cell State to WR_CELL_STATE_RESET Failed");
      RETVALUE(RFAILED);
   }
   if (ROK != wrEmmMmeEnbRst(cellCb))
   {
      RLOG0(L_ERROR,"Failed to send eNB reset to MME");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
/* FIX for ccpu00135294 */
/*
 *       Fun:     wrEmmGetCgiFromUeCb
 *
 *       Desc:    This primitive returns the CGI to be filled in INIT UE msg
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 wrEmmGetCgiFromUeCb
(
WrUeCb                     *ueCb,
WrCgi                        *cgi
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, ueCb->cellId,"Invalid CellId while getting"
                "CGI from UE CB");
      RETVALUE(RFAILED);
   }

   cgi->plmnId = ueCb->plmnId;
   cgi->cellId = cellCb->sib1.cellIdentity;
   RETVALUE(ROK);
}

PUBLIC S16 wrEmmGetCgi
(
WrCellId                     cellId,
WrCgi                        *cgi
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received while getting CGI");
      RETVALUE(RFAILED);
   }

   cgi->plmnId = cellCb->sib1.plmns[0].plmnId;
   cgi->cellId = cellCb->sib1.cellIdentity;
   RETVALUE(ROK);
}

/*
 *       Fun:     wrEmmGetTai
 *
 *       Desc:    This primitive returns the TAI of the cell
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 wrEmmGetTai
(
WrUeCb                     *ueCb,
WrTai                        *tai
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, ueCb->cellId, "Invalid CellId received while getting TAI");
      RETVALUE(RFAILED);
   }

   tai->plmnId = ueCb->plmnId;
   tai->tac    = cellCb->sib1.tac;
   RETVALUE(ROK);
}

/*UE History info - start*/
/*
 *       Fun:     wrEmmGetCellType
 *
 *       Desc:    This primitive returns the cellSizeType of the cell
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 wrEmmGetCellSizeType
(
WrCellId                     cellId,
U8                           *cellSizeType
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG1(L_ERROR,"Invalid CellId[%d] received while getting Cell Size Type", cellId);
      RETVALUE(RFAILED);
   }

   *cellSizeType = cellCb->cellSizeType;
   RETVALUE(ROK);
}
/*UE History info - end*/

/*
*       Fun:   wrEmmRrcCellCfgCfm 
*
*       Desc:  Handle Cell Cfg Cfm 
*
*       Ret:   ROK
*
*       Notes: None
*/
PUBLIC S16 wrEmmRrcCellCfgCfm
(
U16                          cellId
)
{
   /* Trigger scheduler Cell Cfg */
   if (wrEmmCellSchdCfg(cellId) != ROK)
   {
      RLOG1(L_ERROR, "Scheduler Cell configuration failed for cellid  %d", cellId);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*
*       Fun:  wrEmmRrcCellDelCfgCfm 
*
*       Desc:  Handle Cell Del Cfg Cfm 
*
*       Ret:   ROK
*
*       Notes: None
*/
PUBLIC S16 wrEmmRrcCellDelCfgCfm
(
U16                          cellId
)
{
   U16 mmeIdx = 0;
   U8  cfgLyr = FALSE;
   S16 retVal = ROK;
   U8  idx    = 0;
   WrCellCb *cellCb = NULL;

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received for RRC cell delete confirmation");
      RETVALUE(RFAILED);
   }

   /* send cell delete to RRM */
#ifndef RM_INTF
   if (ROK != wrEmmCellDelRrmCfg(cellCb))
   {
      RLOG0(L_ERROR, "Failed to send cell delete confirmation to RRM");
      RETVALUE(RFAILED);
   }
#endif /*RM_INTF*/

   /* Need to Clean up the Pbm and reconfigure Measurement and SON module
      Change the Cell state to WR_CELL_STATE_INIT
      Invoke the Cell Configuration towards Lower Layers
   */
   if(ROK != wrEmmAnrProcessIntraFreqChg(cellId))
   {
      RLOG1(L_ERROR,"RRC cell delete confirmation failed due to failure in changing intra frequency %d", cellId);
      RETVALUE(RFAILED);
   }
   if (ROK != wrEmmPbmCellDel(cellCb))
   {
      RLOG1(L_ERROR,"RRC cell delete confirmation failed due to failure received from PBM for cell deletion."
            "CellId =%d",cellId);
      RETVALUE(RFAILED);
   }
   /* Need to Clean up the Pbm and reconfigure Measurement module*/
   if (ROK != wrEmmPbmCellCfg(cellCb))
   {
      RLOG1(L_ERROR,"RRC cell delete confirmation failed due to failure received from PBM for cell deletion."
            "CellId =%d",cellId);
      RETVALUE(RFAILED);
   }

   if(ROK != wrEmmUmmMeasReCfg(cellId))
   {
      RLOG1(L_ERROR,"RRC cell delete confirmation failed due to failure received"
            "from UMM for cell reconfiguration.CellId is %d", cellId);
      RETVALUE(RFAILED);

   }

   RLOG_ARG0(L_INFO, DBG_CELLID, cellCb->cellId,
               "Cell state = INIT, Reason = Cell delete confirmation");

   cellCb->cellState = WR_CELL_STATE_INIT;
   /* Update the WrummCellCb state here */
   if(ROK != wrUmmUpdateCellState(cellId,WR_CELL_STATE_INIT))
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellCb->cellId, "Updation of cell state to WR_CELL_STATE_INIT failed");
      RETVALUE(RFAILED);
   }

   while (mmeIdx < wrEmmCb.mmeCont.numMmes)
   {
      if(wrEmmCb.mmeCont.mmes[idx])
      {
         if(wrEmmCb.mmeCont.mmes[idx]->state == WR_MME_UP)
         {
            cfgLyr = TRUE;
            break;
         }
         mmeIdx++;
      }
      idx++;
   }

   if(cfgLyr == TRUE)
   {
      retVal = wrFillNhuCellCfgReq(cellCb);
   }
   else
   {
      RLOG_ARG0(L_WARNING, DBG_CELLID, cellId, "No MME association is present");
   }

   RETVALUE(retVal);
}

/*
*       Fun:  wrEmmUmmMeasReCfg 
*
*       Desc: Send Umm Cell ReCfg  
*
*       Ret:   ROK
*
*       Notes: None
*/
PUBLIC S16 wrEmmUmmMeasReCfg 
(
WrCellId cellId
)
{
   RETVALUE(wrUmmMeasIntraCellReCfg(cellId));
}
/*
*       Fun:   wrEmmSchdCfgCfm
*
*       Desc:  Handle Cell Cfg Cfm 
*
*       Ret:   ROK
*
*       Notes: None
*/
PUBLIC S16 wrEmmSchdCellCfgCfm
(
U32                          transId,
U8                           status
)
{
   WrCellCb                  *cellCb = NULLP;


   WR_GET_CELLCB(cellCb, transId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, transId, "Failed to process cell confirmation received from scheduler."
                "Invalid cell id is received.");
      RETVALUE(RFAILED);
   }

  
   if (status != WR_CFG_OK)
   {
      /* Raise an alarm here for cell configuration failure               */
      RLOG0(L_FATAL,"ALARM!! Scheduler cell configuration failed");
      RETVALUE(RFAILED);
   }

   /* Configure the broadcast module */
   if (wrEmmPbmSibConfig((U16)transId) != ROK)
   {
      RLOG0(L_FATAL,"ALARM!! Scheduler SI configuration error");
      RETVALUE(RFAILED);
   }

   /* send cell config to RRM */
#ifndef RM_INTF
   if (ROK != wrEmmCellRrmCfg(cellCb))
   {
      RLOG0(L_ERROR,"RRM cell configuration failed ");
      RETVALUE(RFAILED);
   }
#endif /*RM_INTF*/

   /* Send the data application module cell configuration */
   if (wrEmmCellCfgDam((U16)transId) != ROK)
   {
      RLOG0(L_FATAL,"ALARM!! Data application configuration error");
      RETVALUE(RFAILED);
   }

   if ((smCb.reCfg == TRUE)&&(smCb.prem == FALSE) && (smCb.isPhyCfgd == TRUE))
   {
      RLOG0(L_EVENT,"CTF cell reconfiguration");
      if (wrFillCtfCellReCfgReq(cellCb->cellId) != ROK)
      {
         RLOG0(L_FATAL,"Cell reconfiguration request failed");
         RETVALUE(RFAILED);
      }
      else
      {
         RLOG0(L_INFO,"Cell reconfiguration request successful");
      }
      RETVALUE(ROK);
   }
   smCb.prem = FALSE;
   RLOG0(L_EVENT,"CTF cell configuration");
   /* Send the Ctf Cell Configuration */
   if (wrFillCtfCellCfgReq((U16)transId) != ROK)
   {
      RLOG0(L_FATAL,"ALARM!! PHY cell configuration error");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
/* RIM */

/**
 *  @brief This function is used to initiate the RIM Procedure towards 
 *  the UTRAN FDD/TDD cells.
 *
 *  @details
 *
 *      Function  : wrEmmTrigUtraRirProc
 *
 *          Processing Steps:
 *          - Get UTRAN FDD/TDD  Cell Cb from the cellCb 
 *          - Trigger the RIR Procedure towards the Utran Cell
 *
 *  @param[in]  lst        : NRT list for fetching the UTRA frequency and cellCb 
 *  @param[in]  reptType   : Report type to be used for RIM RIR procedure
 *  @param[out] status     : Result of RIR procedure to be stored in status 
 *  @param[out] rimReqInfo : It will store RIM src,dst and rptCellId details
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
PUBLIC S16 wrEmmTrigUtraRirProc
(
 CmLListCp                   *lst,
 U8                          reptType,
 U8                          *status,
 WrRimReqInfo                *rimReqInfo
)
{
   CmLList                   *lnk = NULLP;
   CmLListCp                 *nrLst = NULLP;
   CmLList                   *link = NULLP;
   WrNrUtranFreqCb           *utranFreqNode = NULLP;
   WrUtraNeighCellCb         *utraCellCb = NULLP;


   lnk = lst->first;

   while( NULLP != lnk)
   {
      utranFreqNode = (WrNrUtranFreqCb*)lnk->node;
      nrLst = &utranFreqNode->nrWlLst;
      link = nrLst->first;
      while( NULLP != link)
      {
         utraCellCb = (WrUtraNeighCellCb*)link->node;
         link = link->next;
         if(( TRUE == utraCellCb->isRimSupported ) &&
               ((( WR_RIM_RIR_MULTI == reptType) && 
                 ( WR_RIM_PROC_NOT_INITIATED == utraCellCb->rimState )) || 
                (( WR_RIM_RIR_STOP == reptType) && 
                 ( WR_RIM_PROC_INITIATED == utraCellCb->rimState ))))
         {
            /* Filling the rptCellId of the rimReqInfo */
            rimReqInfo->appId                      = WR_RIM_APP_UTRA_SI;
            rimReqInfo->rptCellId.ratChoice        = WR_RIM_UTRA_RAT ;
            rimReqInfo->rptCellId.t.utraCell.cId   = utraCellCb->cId;
            rimReqInfo->rptCellId.t.utraCell.rncId = utraCellCb->rncId;
            wrUtlCopyPlmnId(&(utraCellCb->plmnId[0]),
                  &(rimReqInfo->rptCellId.t.utraCell.plmnId));


            /* Filling the dstAddr of the rimReqInfo */
            rimReqInfo->dstAddr.ratChoice           = WR_RIM_UTRA_RAT;
            rimReqInfo->dstAddr.t.rncIdentity.lac   = utraCellCb->lac;
            rimReqInfo->dstAddr.t.rncIdentity.rncId = utraCellCb->rncId; 
            rimReqInfo->dstAddr.t.rncIdentity.rac   = utraCellCb->rac;
            wrUtlCopyPlmnId(&(utraCellCb->plmnId[0]),
                  &(rimReqInfo->dstAddr.t.rncIdentity.plmnId));

            /* Initiate the RIR Procedure */
            if(ROK == wrEmmRimInfoReq(rimReqInfo))
            {
               if( WR_RIM_RIR_MULTI == reptType)
               {
                  utraCellCb->rimState = WR_RIM_PROC_INITIATED;
                  *status = TRUE;
                  /* Increment the RIM Attempt counter */
                  WR_INC_KPI_FUNC(wrIncKpiFrRimAttmptd);
               }
               else if ( WR_RIM_RIR_STOP == reptType)
               {
                  utraCellCb->rimState = WR_RIM_PROC_NOT_INITIATED;
               }  
               RLOG3(L_DEBUG, "Rim Info request has been initiated for Utra Cell"
                     "with lac [%d] rncId[%ld] rac[%ld]",utraCellCb->lac,
                     utraCellCb->rncId,utraCellCb->rac);
            }
            else 
            {
               RLOG3(L_WARNING, "Rim Info request has been failed for Utra Cell"
                     "with lac [%d] rncId[%ld] rac[%ld]",utraCellCb->lac,
                     utraCellCb->rncId,utraCellCb->rac);
            }
         }
      }/* end of inner while */
      lnk = lnk->next;
   }/* end of outer while */

   RETVALUE(ROK);
}/* End of wrEmmTrigUtraRirProc */
/**
 *  @brief This function is used to initiate the RIM Procedure towards 
 *  the UTRA/GERAN cells.
 *
 *  @details
 *
 *      Function  : wrEmmTrigRirProc
 *
 *          Processing Steps:
 *          - Get Geran Cell Cb from the cellCb 
 *          - Trigger the RIR Procedure towards the Geran Cell
 *          - Get Utran frequency list from the cellCb
 *          - Get Utran Cell Cb from the Utran frequency list 
 *          - Trigger the RIR Procedure towards the Utra Cell
 *
 *  @param[in] cellCb : cellCb for fetching the UTRA frequency and cellCb 
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
PUBLIC S16 wrEmmTrigRirProc
(
 WrCellCb                    *cellCb,
 U8                          reptType
)
{
   WrRimReqInfo              rimReqInfo;
   CmLListCp                 *lst = NULLP;
   WrGeranCellCb             *geranCellCb = NULLP;
   CmLList                   *listNode = NULLP;
   U8                        status = FALSE;


   /* Filling the srcEnbId of the rimReqInfo */
   rimReqInfo.reptType                        = reptType;
   rimReqInfo.srcAddr.ratChoice               = WR_RIM_EUTRA_RAT;
   rimReqInfo.srcAddr.t.eutraIdentity.enbId   = wrEmmCb.enbId;
   rimReqInfo.srcAddr.t.eutraIdentity.enbType = wrEmmCb.enbType;
   rimReqInfo.srcAddr.t.eutraIdentity.tac     = cellCb->sib1.tac;
   wrUtlCopyPlmnId(&(cellCb->sib1.plmns[0].plmnId),
         &(rimReqInfo.srcAddr.t.eutraIdentity.plmnId));

   /* RIR procedure initiation for GERAN cells*/
   /* Fetch the GERAN cell Cb 
    * Initiate the RIR Procedure per GERAN Cell*/
   geranCellCb = (WrGeranCellCb *)
      CM_LLIST_FIRST_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,
            listNode);
   while(geranCellCb != NULLP)
   {
      if(( TRUE == geranCellCb->isRimSupported ) &&
            ((( WR_RIM_RIR_MULTI == reptType) && 
              ( WR_RIM_PROC_NOT_INITIATED == geranCellCb->rimState )) || 
             (( WR_RIM_RIR_STOP == reptType) && 
              ( WR_RIM_PROC_INITIATED == geranCellCb->rimState ))))
      {
         /* Filling the rptCellId of the rimReqInfo */
         rimReqInfo.appId                      = WR_RIM_APP_NACC;
         rimReqInfo.rptCellId.ratChoice        = WR_RIM_GERAN_RAT;
         rimReqInfo.rptCellId.t.geranCell.ci   = geranCellCb->cId;
         rimReqInfo.rptCellId.t.geranCell.lac  = geranCellCb->lac;
         rimReqInfo.rptCellId.t.geranCell.rac  = geranCellCb->rac;
         wrUtlCopyPlmnId(&(geranCellCb->plmnId),
               &(rimReqInfo.rptCellId.t.geranCell.plmnId));

         /* Filling the dstAddr of the rimReqInfo */
         rimReqInfo.dstAddr.ratChoice           = WR_RIM_GERAN_RAT;
         rimReqInfo.dstAddr.t.geranCellId.ci    = geranCellCb->cId;
         rimReqInfo.dstAddr.t.geranCellId.lac   = geranCellCb->lac; 
         rimReqInfo.dstAddr.t.geranCellId.rac   = geranCellCb->rac;
         wrUtlCopyPlmnId(&(geranCellCb->plmnId),
               &(rimReqInfo.dstAddr.t.geranCellId.plmnId));

         /* Initiate the RIR Procedure */
         if(ROK == wrEmmRimInfoReq(&rimReqInfo))
         {
            if( WR_RIM_RIR_MULTI == reptType)
            {
               geranCellCb->rimState = WR_RIM_PROC_INITIATED;
               status = TRUE;
               /* Increment the RIM Attempt counter */
               WR_INC_KPI_FUNC(wrIncKpiFrRimAttmptd);
            }
            else if( WR_RIM_RIR_STOP == reptType) 
            {
               geranCellCb->rimState = WR_RIM_PROC_NOT_INITIATED;
            }  
            RLOG3(L_DEBUG, "Rim Info request has been initiated for GERAN Cell"
                  "with ci [%d] lac[%ld] rac[%ld]",geranCellCb->cId,
                  geranCellCb->lac,geranCellCb->rac);
         }
         else 
         {
            RLOG3(L_WARNING, "Rim Info request has been failed for GERAN Cell"
                  "with ci [%d] lac[%ld] rac[%ld]",geranCellCb->cId,
                  geranCellCb->lac,geranCellCb->rac);
         }
      }
      geranCellCb = (WrGeranCellCb *)
         CM_LLIST_NEXT_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,
               listNode);
   } /* end of while */

   /* RIR procedure initiation for UTRAN FDD cells 
    * Fetch the UTRA FDD cell Cb by tranverse through the UTRA Frequency 
    * Initiate the RIR Procedure per UTRAN FDD Cell*/

   lst = &cellCb->nrCb->utranCb.utranFreqList;
   wrEmmTrigUtraRirProc(lst, reptType, &status, &rimReqInfo);

   /* RIR procedure initiation for UTRAN TDD cells 
    * Fetch the UTRA TDD cell Cb by tranverse through the UTRA Frequency 
    * Initiate the RIR Procedure per UTRAN TDD Cell*/

   lst = &cellCb->nrCb->utranTddCb.utranFreqList;
   wrEmmTrigUtraRirProc(lst, reptType, &status, &rimReqInfo);

   /*Start the overall RIM timer when atleast one RIR Procedure 
    * has been triggered */
   if( TRUE == status)
   {
      wrStartTmr((PTR)cellCb, WR_TMR_RIM_OVERALL, wrEmmCb.tmrCfg.rimOvrlTmr);
      RLOG0(L_DEBUG,"RIM overall timer has been started.");
   }

   RETVALUE(ROK);

} /* end of wrEmmTrigRirProc */

/**
 *  @brief This function is used to fetch the Utracell cb.
 *
 *  @details
 *      Function  : wrEmmGetUtraCellCb
 *          Processing Steps:
 *          - Get Utran frequency list from the cellCb
 *          - Get Utran Cell Cb from the Utran frequency list
 *          - Compare the cId and the PLMN of the received WrRimRptCell 
 *          - with the Utran Cell cb 
 *
 *  @param[in] cellCb : cellCb for fetching the UTRA frequency and cellCb 
 *  @param[in] rptCellId : Received WrRimRptCell from the RIM 
 *  @param[out] WrUtraNeighCellCb : Utran cellCb which matches the cId and
 *                                  PLMN of the received rptCellId.  
**/ 
PRIVATE WrUtraNeighCellCb* wrEmmGetUtraCellCb
(
 WrCellCb                    *cellCb,
 WrRimRptCell                *rptCellId 
)
{
   CmLListCp                 *lst = NULLP;
   CmLList                   *lnk = NULLP;
   CmLListCp                 *nrLst = NULLP;
   CmLList                   *link = NULLP;
   WrNrUtranFreqCb           *utranFreqNode = NULLP;
   WrUtraNeighCellCb         *utraCellCb = NULLP; 

   /* UTRA FDD Frequency List */
   lst = &cellCb->nrCb->utranCb.utranFreqList;
   lnk = lst->first;

   while( NULLP != lnk)
   {
      utranFreqNode = (WrNrUtranFreqCb*)lnk->node;
      nrLst = &utranFreqNode->nrWlLst;
      link = nrLst->first;
      while( NULLP != link)
      {
         utraCellCb = (WrUtraNeighCellCb*)link->node;
         if( (utraCellCb->cId   == rptCellId->t.utraCell.cId) &&
             (utraCellCb->rncId == rptCellId->t.utraCell.rncId) &&
             (wrEmmPlmnPlmnsEqual(&(utraCellCb->plmnId[0]),
                                  &(rptCellId->t.utraCell.plmnId))))
         {
            RETVALUE(utraCellCb);
         }
         link = link->next;
      }
      lnk = lnk->next;
   }

   /* UTRA TDD Frequency List */
   lst = &cellCb->nrCb->utranTddCb.utranFreqList;
   lnk = lst->first;

   while( NULLP != lnk)
   {
      utranFreqNode = (WrNrUtranFreqCb*)lnk->node;
      nrLst = &utranFreqNode->nrWlLst;
      link = nrLst->first;
      while( NULLP != link)
      {
         utraCellCb = (WrUtraNeighCellCb*)link->node;
         if( (utraCellCb->cId   == rptCellId->t.utraCell.cId) &&
             (utraCellCb->rncId == rptCellId->t.utraCell.rncId) &&
             (wrEmmPlmnPlmnsEqual(&(utraCellCb->plmnId[0]),
                                  &(rptCellId->t.utraCell.plmnId))))
         {
            RETVALUE(utraCellCb);
         }
         link = link->next;
      }
      lnk = lnk->next;
   }

   RETVALUE(NULLP);
}/* end of wrEmmGetUtraCellCb */

/**
 *  @brief This function is used to fetch the Geran cell cb.
 *
 *  @details
 *      Function  : wrEmmGetGeranCellCb
 *          Processing Steps:
 *          - Get Geran Cell Cb from the cellCb
 *          - Compare the ci,rac,lac and the PLMN of the received WrRimRptCell 
 *          - with the Geran Cell cb 
 *
 *  @param[in] cellCb : cellCb for fetching the Geran cellCb 
 *  @param[in] rptCellId : Received WrRimRptCell from the RIM 
 *  @param[out] WrGeranCellCb : Geran cellCb which matches the ci,lac,rac and
 *                                  PLMN of the received rptCellId.  
**/ 
PRIVATE WrGeranCellCb* wrEmmGetGeranCellCb
(
 WrCellCb                    *cellCb,
 WrRimRptCell                *rptCellId 
)
{
   WrGeranCellCb             *geranCellCb = NULLP;
   CmLList                   *listNode = NULLP;

   geranCellCb = (WrGeranCellCb *)
      CM_LLIST_FIRST_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,
            listNode);
   while(geranCellCb != NULLP)
   {
      if(( geranCellCb->cId == rptCellId->t.geranCell.ci)  &&
         ( geranCellCb->lac == rptCellId->t.geranCell.lac) &&
         ( geranCellCb->rac == rptCellId->t.geranCell.rac) &&
         ( wrEmmPlmnPlmnsEqual(&(geranCellCb->plmnId),
                                 &(rptCellId->t.geranCell.plmnId))))
      {
         RETVALUE(geranCellCb);
      }
      geranCellCb = (WrGeranCellCb *)
         CM_LLIST_NEXT_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,
               listNode);
   } /* end of while */ 

   RETVALUE(NULLP);
}/* end of wrEmmGetGeranCellCb */
 
/**
 *  @brief This function is used to delete the SI from Geran cellCb.
 *
 *  @details
 *      Function  : wrEmmDelGeranSi
 *          Processing Steps:
 *          -Delete the SI present in the Geran cellCb one after another
 *
 *  @param[in] geranCellCb : Geran cellCb for which SI needs to be deleted 
 *  @param[out] Void
**/ 
PUBLIC Void wrEmmDelGeranSi
(
 WrGeranCellCb             *geranCellCb
)
{
   U8                        idx = 0;
   U8                        siLen = 0;

   siLen =((WR_RIM_SI == geranCellCb->siPsiType)? 
            WR_EMM_RIM_GERAN_SI_LEN : WR_EMM_RIM_GERAN_PSI_LEN);
   for(idx = 0;idx < geranCellCb->noOfsiPsi;idx++)
   {
      if(NULLP != geranCellCb->siOrPsi[idx])
      {
         if(NULLP != geranCellCb->siOrPsi[idx]->val)
         {
            WR_FREE(geranCellCb->siOrPsi[idx]->val,
                  siLen);
         }
         WR_FREE(geranCellCb->siOrPsi[idx],
               sizeof(TknStrOSXL));
      }
   }   
   RETVOID;
}/* end of wrEmmDelGeranSi */ 

/**
 *  @brief This function is used to update the recieved info in the NRT .
 *
 *  @details
 *      Function  : wrEmmRimErrInd
 *          Processing Steps:
 *          - Get Utra cellCb or Geran cellCb based on the appId
 *          - Update the RIM counter
 *          - Delete the stored SI if available
 *
 *  @param[in] rimErrInd : Error info received from the RIM 
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
PUBLIC S16 wrEmmRimErrInd
(
 WrRimErrInd                 *rimErrInd
)
{

   WrCellCb                  *cellCb = wrEmmCb.cellCb[0];
   WrUtraNeighCellCb         *utraCellCb = NULLP;
   WrGeranCellCb             *geranCellCb = NULLP;

 switch(rimErrInd->appId)
   {
      case WR_RIM_APP_UTRA_SI:
         {
            utraCellCb = wrEmmGetUtraCellCb(cellCb,&(rimErrInd->rptCellId));
            if(NULLP == utraCellCb)
            {

               RLOG2(L_ERROR,"Unable to fetch the UtraCellCb"
                     "from the received rptCellId(cid = %d,rncId =%d)"
                     ,rimErrInd->rptCellId.t.utraCell.cId,
                     rimErrInd->rptCellId.t.utraCell.rncId);
               RETVALUE(RFAILED);
            } 
            utraCellCb->rimState = WR_RIM_PROC_NOT_INITIATED;
            if(( NULLP == utraCellCb->utraSI.val)  &&
               (( WR_EMM_RIM_ERR_TMR_EXP       ==  rimErrInd->errType)  || 
                ( WR_EMM_RIM_ERR_PROT          == rimErrInd->errType)  || 
                ( WR_EMM_RIM_ERR_RI_VAL_FAILED == rimErrInd->errType)))
            {
               /* Increment the RIM Fail counter */
                  WR_INC_KPI_FUNC(wrIncKpiFrRimFaild);
            }

            if(NULLP != utraCellCb->utraSI.val)
            { 
               WR_FREE(utraCellCb->utraSI.val,utraCellCb->utraSI.len);
            }
            break;
         }
      case WR_RIM_APP_NACC:
         {   
            geranCellCb = wrEmmGetGeranCellCb(cellCb,&(rimErrInd->rptCellId));
            if(NULLP == geranCellCb)
            {

               RLOG3(L_ERROR,"Unable to fetch the geranCellCb"
                     "from the received rptCellId(cellId =%d,lac =%d,rac = %d)",
                     rimErrInd->rptCellId.t.geranCell.ci,
                     rimErrInd->rptCellId.t.geranCell.lac,
                     rimErrInd->rptCellId.t.geranCell.rac);
               RETVALUE(RFAILED);
            } 
            geranCellCb->rimState = WR_RIM_PROC_NOT_INITIATED;
            if(( 0 == geranCellCb->noOfsiPsi)   &&
               (( WR_EMM_RIM_ERR_TMR_EXP       == rimErrInd->errType)  || 
                ( WR_EMM_RIM_ERR_PROT          == rimErrInd->errType)  || 
                ( WR_EMM_RIM_ERR_RI_VAL_FAILED == rimErrInd->errType)))
            {
               /* Increment the RIM Fail counter */
                  WR_INC_KPI_FUNC(wrIncKpiFrRimFaild);
            }
            if(geranCellCb->noOfsiPsi)
            {
                 wrEmmDelGeranSi(geranCellCb);
            }
            break;
         }
      default:
            break;
   } 
   RETVALUE(ROK);
} /* end of wrEmmRimErrInd */

/**
 *  @brief This function is used to update the recieved info in the NRT .
 *
 *  @details
 *      Function  : wrEmmRimInfoRsp
 *          Processing Steps:
 *          - Get Utra cellCb or Geran cellCb based on the appId
 *          - Update the RIM counter
 *          - Update the received SI in the Utra cellCb or Geran cellCb
 *            based on the appid
 *
 *  @param[in] riInfo : RI info received from the RIM 
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
PUBLIC S16 wrEmmRimInfoRsp
(
 WrEmmRimRspInfo             *riInfo
)
{
   WrCellCb                  *cellCb = wrEmmCb.cellCb[0];
   WrUtraNeighCellCb         *utraCellCb = NULLP;
   WrGeranCellCb             *geranCellCb = NULLP;
   U8                        siLen = 0;
   U8                        idx = 0;
   S16                       ret = ROK;

   switch(riInfo->appId)
   {
      case WR_RIM_APP_UTRA_SI:
         {
            utraCellCb = wrEmmGetUtraCellCb(cellCb,&(riInfo->rptCellId));
            if( NULLP == utraCellCb)
            {

               RLOG2(L_ERROR,"Unable to fetch the UtraCellCb"
                     "from the received rptCellId(cid =%d,rncId =%d)"
                     ,riInfo->rptCellId.t.utraCell.cId,
                     riInfo->rptCellId.t.utraCell.rncId);
               if( NULLP != riInfo->app.utraSI.val)
               {
                  WR_FREE(riInfo->app.utraSI.val,riInfo->app.utraSI.len);
               }
               RETVALUE(RFAILED);
            }
            if(( WR_RIM_RI_END  == riInfo->rptType) ||
                  ( WR_RIM_RI_STOP == riInfo->rptType) ||
                  ( PRSNT_NODEF == riInfo->appErrCause.pres))  
            {
               utraCellCb->rimState = WR_RIM_PROC_NOT_INITIATED;
               if( NULLP != utraCellCb->utraSI.val)
               {
                  WR_FREE(utraCellCb->utraSI.val,utraCellCb->utraSI.len);
               }
               if( PRSNT_NODEF == riInfo->appErrCause.pres)
               {
                  /* Increment the RIM Fail counter */
                  WR_INC_KPI_FUNC(wrIncKpiFrRimFaild);
               }
            }
            else 
            {
               if( WR_RIM_RI_INIT_MULTI == riInfo->rptType)
               {
                  /* Increment the RIM Sucess count */
                  WR_INC_KPI_FUNC(wrIncKpiFrRimSuccd);
               }
               /* Delete the old SI and update with new SI */
               if( NULLP != utraCellCb->utraSI.val)
               {
                  WR_FREE(utraCellCb->utraSI.val,utraCellCb->utraSI.len);
               }
               utraCellCb->utraSI = riInfo->app.utraSI;
            }
            break;
         }
      case WR_RIM_APP_NACC:
         {  
            geranCellCb = wrEmmGetGeranCellCb(cellCb,&(riInfo->rptCellId));
            if( NULLP == geranCellCb)
            {
               RLOG3(L_ERROR,"Unable to fetch the geranCellCb"
                     "from the received rptCellId(ci =%d,lac =%d,rac = %d)",
                     riInfo->rptCellId.t.geranCell.ci,
                     riInfo->rptCellId.t.geranCell.lac,
                     riInfo->rptCellId.t.geranCell.rac);
               if( NULLP != riInfo->app.nacc.si.val)
               {
                  siLen =( WR_RIM_SI ==riInfo->app.nacc.siType)? 
                     WR_EMM_RIM_GERAN_SI_LEN : WR_EMM_RIM_GERAN_PSI_LEN ;
                  WR_FREE(riInfo->app.nacc.si.val,
                        (riInfo->app.nacc.noOfSi * siLen));
               }
               RETVALUE(RFAILED);
            }
            if(( WR_RIM_RI_END  == riInfo->rptType) ||
                  ( WR_RIM_RI_STOP == riInfo->rptType) ||
                  ( PRSNT_NODEF == riInfo->appErrCause.pres))  
            {
               geranCellCb->rimState = WR_RIM_PROC_NOT_INITIATED;

               if(geranCellCb->noOfsiPsi)
               {
                  wrEmmDelGeranSi(geranCellCb);
               }
               if( PRSNT_NODEF == riInfo->appErrCause.pres)
               {
                  /* Increment the RIM Fail counter */
                  WR_INC_KPI_FUNC(wrIncKpiFrRimFaild);
               }
            }
            else 
            {   
               if( WR_RIM_RI_INIT_MULTI == riInfo->rptType)
               {
                  /* Increment the RIM Sucess count */
                  WR_INC_KPI_FUNC(wrIncKpiFrRimSuccd);
               }

               /* Delete the old SI if present*/
               if(geranCellCb->noOfsiPsi)
               {
                  wrEmmDelGeranSi(geranCellCb);
               }
               /* Update the SI the geranCellCb with New SI */
               geranCellCb->siPsiType = riInfo->app.nacc.siType; 
               geranCellCb->noOfsiPsi = riInfo->app.nacc.noOfSi; 
               siLen = ( WR_RIM_SI == riInfo->app.nacc.siType)? 
                  WR_EMM_RIM_GERAN_SI_LEN : WR_EMM_RIM_GERAN_PSI_LEN ;
               for(idx = 0 ;(( idx < geranCellCb->noOfsiPsi) && 
                        ( idx < WR_MAX_GERAN_SI)) ; idx++)
               {

                  WR_ALLOC(&(geranCellCb->siOrPsi[idx]),
                        sizeof(TknStrOSXL));
                  if( NULLP == geranCellCb->siOrPsi[idx])
                  {
                     RLOG0(L_FATAL, "Memory allocation failed for geranCellCb->siOrPsi.");
                     ret = RFAILED;
                     break;
                  }
                  WR_ALLOC(&(geranCellCb->siOrPsi[idx]->val),
                        siLen);
                  if( NULLP == geranCellCb->siOrPsi[idx]->val)
                  {
                     RLOG0(L_FATAL, "Memory allocation failed for geranCellCb->siOrPsi[idx]->val.");
                     ret = RFAILED;
                     break;
                  }
                  else
                  {
                     cmMemcpy((U8 *)(geranCellCb->siOrPsi[idx]->val),
                           (U8 *)&(riInfo->app.nacc.si.val[idx * siLen]),
                           siLen);
                  }
               }

               WR_FREE(riInfo->app.nacc.si.val,
                     (riInfo->app.nacc.noOfSi * siLen));
               if( RFAILED == ret)
               {
                  wrEmmDelGeranSi(geranCellCb);
                  geranCellCb->rimState = WR_RIM_PROC_NOT_INITIATED;
               }
            } 
            break;
         }
      default:
            break;
   }
   RETVALUE(ROK);
} /* end of wrEmmRimInfoRsp */


/**
 *  @brief This function is used to intiate RIR with Stop .
 *
 *  @details
 *      Function  : wrEmmRimTrigRirStop
 *          Processing Steps:
 *          - Stop the RIM overall timer
 *          - Initiate the RI procedure with Stop
 *
 *  @param[in] cellCb : WrCellCb 
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
PUBLIC S16 wrEmmRimTrigRirStop
(
 WrCellCb                    *cellCb
)
{
   /* Stop the overall RIM timer */
   wrStopTmr((PTR)cellCb, WR_TMR_RIM_OVERALL);
   RLOG0(L_DEBUG,"Stopping RIM overall timer");

   /* Sending the RIR with Stop request to all the UTRA & GERAN Cells */
   wrEmmTrigRirProc(cellCb,WR_RIM_RIR_STOP);

   RETVALUE(ROK);
}

/**
 *  @brief This function is used to change the rimState .
 *
 *  @details
 *      Function  : wrEmmRimNghStateChange
 *          Processing Steps:
 *          - Fetch the Geran cellCb and update 
 *          - the rimState to WR_RIM_PROC_NOT_INITIATED and delete SI 
 *          - if isRimSupported is TRUE and rimState is WR_RIM_PROC_INITIATED
 *          - Fetch the UTRA cellCb and update 
 *          - the rimState to WR_RIM_PROC_NOT_INITIATED and delete SI 
 *          - if isRimSupported is TRUE and rimState is WR_RIM_PROC_INITIATED
 *
 *  @param[in] cellCb : WrCellCb 
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
PUBLIC S16 wrEmmRimNghStateChange
(
 WrCellCb                    *cellCb
)
{

   CmLListCp                 *lst = NULLP;
   CmLList                   *lnk = NULLP;
   CmLListCp                 *nrLst = NULLP;
   CmLList                   *link = NULLP;
   WrNrUtranFreqCb           *utranFreqNode = NULLP;
   WrUtraNeighCellCb         *utraCellCb = NULLP;
   WrGeranCellCb             *geranCellCb = NULLP;
   CmLList                   *listNode = NULLP;

   /* Update the RIM state of the GERAN cell and 
    * delete the SI in the GERAN cell */
   geranCellCb = (WrGeranCellCb *)
      CM_LLIST_FIRST_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,
            listNode);
   while(geranCellCb != NULLP)
   {
      if(( TRUE == geranCellCb->isRimSupported ) && 
         ( WR_RIM_PROC_INITIATED == geranCellCb->rimState))
      {
         geranCellCb->rimState = WR_RIM_PROC_NOT_INITIATED;
         if(geranCellCb->noOfsiPsi)
         {
            wrEmmDelGeranSi(geranCellCb);
         }
      }
      geranCellCb = (WrGeranCellCb *)
         CM_LLIST_NEXT_NODE(&cellCb->nrCb->geranCb.geranNrCellLst,
               listNode);
   } /* end of while */

   /* Update the RIM state of the UTRA cell and 
    * delete the SI in the UTRA cell */
   lst = &cellCb->nrCb->utranCb.utranFreqList;
   lnk = lst->first;
   while( NULLP != lnk)
   {
      utranFreqNode = (WrNrUtranFreqCb*)lnk->node;
      nrLst = &utranFreqNode->nrWlLst;
      link = nrLst->first;
      while( NULLP != link)
      {
         utraCellCb = (WrUtraNeighCellCb*)link->node;
         link = link->next;
         if(( TRUE == utraCellCb->isRimSupported ) && 
            ( WR_RIM_PROC_INITIATED == utraCellCb->rimState))
         {
            utraCellCb->rimState = WR_RIM_PROC_NOT_INITIATED;
            if( NULLP != utraCellCb->utraSI.val)
            {
               WR_FREE(utraCellCb->utraSI.val,utraCellCb->utraSI.len);
            }
         }
      }/* end of inner while */
      lnk = lnk->next;
   }/* end of outer while */

	/* Update the RIM state of the UTRA TDD cell and 
    * delete the SI in the UTRA TDD cell */
   lst = &cellCb->nrCb->utranTddCb.utranFreqList;
   lnk = lst->first;
   while( NULLP != lnk)
   {
      utranFreqNode = (WrNrUtranFreqCb*)lnk->node;
      nrLst = &utranFreqNode->nrWlLst;
      link = nrLst->first;
      while( NULLP != link)
      {
         utraCellCb = (WrUtraNeighCellCb*)link->node;
         link = link->next;
         if(( TRUE == utraCellCb->isRimSupported ) && 
            ( WR_RIM_PROC_INITIATED == utraCellCb->rimState))
         {
            utraCellCb->rimState = WR_RIM_PROC_NOT_INITIATED;
            if( NULLP != utraCellCb->utraSI.val)
            {
               WR_FREE(utraCellCb->utraSI.val,utraCellCb->utraSI.len);
            }
         }
      }/* end of inner while */
      lnk = lnk->next;
   }/* end of outer while */

   RETVALUE(ROK);
}/* end of wrEmmRimNghStateChange */

/* RIM */ 


PUBLIC S16 wrEmmSchdCellReCfgCfm
(
U32                          transId,
U8                           status
)
{
   WrCellCb                  *cellCb = NULLP;

   RLOG0(L_DEBUG, 
            "Scheduler reconfiguration confirm is recived and filling SIs");
   WR_GET_CELLCB(cellCb, transId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, transId, "Invalid CellId received in cell"
                "reconfiguration confirmation.");
      RETVALUE(RFAILED);
   }
 
   if (status != WR_CFG_OK)
   {
      /* Raise an alarm here for cell Re-configuration failure               */
      RLOG0(L_FATAL,"ALARM!! Scheduler cell re-configuration failed");
      RETVALUE(ROK);
   }
   /* Re-Configure the broadcast module */
   if (wrEmmCb.cellCb[0]->pendReCfgPres)
   {
      cellCb->pendReCfgPres = FALSE;
      if( wrEmmPbmSibConfig((U16)transId) != ROK)
      {
         RLOG1(L_ERROR,"ALARM!! Scheduler SI configuration error.TransId[%ld]",transId);
         RETVALUE(ROK);
      }
   }
   RETVALUE(ROK);
}


PUBLIC S16 wrEmmSendEnbConfigTransfer
(
Void
)
{
#ifdef WR_RSYS_OAM
   WrCellCb                  *cellCb = wrEmmCb.cellCb[0];

   /* IP addresss for neighbors will not be configured from RADISYS OAM.
    * The IP address is dynamicaly derived via ENB Configiration Transfer
    * procedure.
    */
   wrEmmMmeBldEnbConfigTranForAllNegh(cellCb);
#endif
   RETVALUE(ROK);
}

PUBLIC S16 wrEmmPrcPhyCfgCfm
(
U32                          transId,
U8                           status
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, transId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, transId, "Failed to send phy confirmation.Invalid cellid received");
      RETVALUE(RFAILED);
   }

   if (status == WR_CFG_OK)
   {
      RLOG1(L_INFO, "ALARM!! Cell is UP Cell Id %ld",transId);
      if (WR_CELL_STATE_INIT == cellCb->cellState)
      {
         RLOG_ARG0(L_INFO, DBG_CELLID, cellCb->cellId,
               "Cell state = UP, Reason = PHY Configuration Confirm");

         cellCb->cellState = WR_CELL_STATE_UP; 
         /* fix for ccpu00139401 */

      }
      /* Update the WrummCellCb state here */
      if(ROK != wrUmmUpdateCellState(transId,WR_CELL_STATE_UP))
      {
         RLOG_ARG0(L_ERROR, DBG_CELLID, transId, "Updation of cell state to WR_CELL_STATE_UP failed");
         RETVALUE(RFAILED);

      }
      /* Sending the RIR with Multiple request to all the UTRA & GERAN Cells*/
      wrEmmTrigRirProc(cellCb, WR_RIM_RIR_MULTI);

#ifdef WR_RSYS_OAM
      /*from radisy oam we will not configure ip address for any of the neighbor,
        we need to get x2 ip address dynamicaly by sending enb config transfer to mme*/
      wrEmmMmeBldEnbConfigTranForAllNegh(cellCb);
#endif
   }
   else if((status != WR_CFG_OK) && (WR_CELL_STATE_UP != cellCb->cellState))
   {
      RLOG0(L_FATAL,"ALARM!! Cell UP failed at PHY");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/*
 *
 *       Fun:     wrEmmS1apProcPdu
 *
 *       Desc:    This primitive is called by IFM 
 *                to send any S1AP message for processing.
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 *
 */
PUBLIC S16 wrEmmS1apProcPdu
(
SztUDatEvnt               *uDatEvnt
)
{
   wrEmmMmePrcRcvdPdu (uDatEvnt->peerId.val, uDatEvnt->pdu);
   RETVALUE(ROK);
}

/*
 *       Fun:     wrEmmGetMmeByPlmn
 *
 *       Desc:     
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 wrEmmGetMmeByPlmn
(
WrPlmnId                *plmnId,
WrMmeId                 *mme
)
{
   RETVALUE(wrEmmPlmnGetMmeByPlmn(plmnId, mme));
}

/*
 *       Fun:     wrEmmGetMmeByMmec
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 wrEmmGetMmeByMmec
(
U8                      mmeCode,
WrMmeId                 *mmeId
)
{
   RETVALUE(wrEmmMmeGetMmeByMmec(mmeCode, mmeId));
}


PUBLIC S16 wrEmmInit
(
Void
)
{
   U32                       idx;
   S16                       ret;

   wrEmmCb.numCells = WR_EMM_MAX_CELLS;
   WR_ALLOC(&wrEmmCb.cellCb, wrEmmCb.numCells * sizeof(WrCellCb *));
   if (wrEmmCb.cellCb == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed while initialising EMM.");
      RETVALUE(RFAILED);
   }
   for(idx = 0; idx < wrEmmCb.numCells; idx++)
   {
      wrEmmCb.cellCb[idx] = NULLP;
   }
   cmInitTimers(&(wrEmmCb.enbCfgWaitTmr), 1);
   cmInitTimers(&(wrEmmCb.enbCfgRspTmr), 1);
   cmInitTimers(&(wrEmmCb.enbResetRspTmr), 1);
   cmInitTimers(&(wrEmmCb.sib8Tmr), 1);
   cmInitTimers(&(wrEmmCb.rimOvrlTmr), 1);

   /*Initlizing the Neighbouring neighbours Hash List*/
   ret = cmHashListInit(&wrEmmCb.neighEnbLstCp, 32/*nmbBins*/,
                           0 /*offset*/, FALSE, 
                           CM_HASH_KEYTYPE_U32MOD,
                           wrEmmCb.init.region, wrEmmCb.init.pool);
   if (ROK!= ret)
   {
      RLOG0(L_ERROR,
               "Initialisation of EMM failed due to failure in the hash list initilization ");
      RETVALUE(RFAILED);
   }
   wrEmmS1FlexInitS1FlexVarForEmmCb();
   wrEmmCb.numGuGrpIds = WR_VAL_ONE;
   wrEmmCb.guGrpIdLst[0].mmeGrpId  = WR_VAL_ONE;

   RETVALUE(ROK);
}

PUBLIC S16 wrEmmSchdTtiInd
(
RgrTtiIndInfo                *ttiInd
)
{
   wrPbmTTIInd(ttiInd);
   WR_FREE(ttiInd, sizeof(RgrTtiIndInfo));
   RETVALUE(ROK);
}

PUBLIC S16 wrEmmSchdSiCfgCfm
(
U32              transId,
U8              status
)
{
  U16                         Id;
  WrCellId                   cellId;
  WrCellCb                   *cellCb;

  /*Parsing the Ids from transaction ID*/  
  Id = transId & 0xffff;
  cellId = transId >> 16;
 
  if (status != RGR_CFG_CFM_OK )  
  {
     RLOG0(L_ERROR,
          "SI CFG CFM is FAILED!!!! ");
     RETVALUE(RFAILED);
  }
  switch(Id)
  {
     case WR_MIB_TRANS_ID:
        {
           RLOG0(L_INFO, 
                        "MIB CFG CFM is success");
        }
        break;
     case WR_SIB1_TRANS_ID:
        {
         /* Enable this print, when logging level are implemented , and chose the appropriate level 
           RLOG0(L_INFO, 
                        "wrEmmSchdSiCfgCfm:SIB1 CFG CFM is success");
         */
        }
        break;
    /* PH04_CMAS */
     case WR_SIB1_PRI_ETWS_TRANS_ID:
        {
           RLOG0(L_INFO, 
                        "SIB1 CFG CFM is success for primary ETWS.");
        }
        break;
     case WR_SIB1_SEC_ETWS_TRANS_ID:
        {
           RLOG0(L_INFO, 
                        "SIB1 CFG CFM is success for secondary ETWS.");
        }
        break;
     case WR_SIB1_BOTH_ETWS_TRANS_ID: 
        {
           RLOG0(L_INFO, 
              "SIB1 CFG CFM is success for both primary and secondary ETWS.");
        }
        break;
     case WR_SIB1_CMAS_TRANS_ID:
        {
           RLOG0(L_INFO, 
                        "SIB1 CFG CFM is success for CMAS.");
        }
        break;
     case WR_SIB1_KILL_TRANS_ID:
        {
           RLOG0(L_INFO, 
                        "SIB1 CFG CFM is success for KILL.");

           WR_GET_CELLCB(cellCb, cellId); 
           if (cellCb == NULLP)
           {
              RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Failed to get the cellCb.");
              RETVALUE(RFAILED);
           }

           /* Create the SIs and configure the MAC for SI transmission. */
           wrEmmPbmWrngStopSi(cellCb->cellId, WR_PBM_SIB12_SI_INDEX);
        }
        break;
     /* PH04_CMAS : end */
     default:
        {
        /* Enable this print, when logging level are implemented , and chose the appropriate level 
           RLOG0(L_INFO, 
                        "wrEmmSchdSiCfgCfm: IDX SI's CFG CFM is success");
        */
        }
        break;
  }

   RETVALUE(ROK);
}/*wrEmmSchdSiCfgCfm*/


/* PH04_CMAS : Added for CMAS feature. */
PUBLIC S16 wrEmmSchdWrngSiCfgCfm
(
U32                          transId,
U8                           status
)
{
 
  WrCellId                   cellId;

  /*Parsing the Ids from transaction ID*/

  cellId = transId >> 16;

    if(status == RGR_CFG_CFM_TX_COMPLETE)
    {
         RLOG0(L_INFO, "Warning SI's CFG CFM is success");
         wrPwsHndlSchdCfgCfm(cellId, transId);
    }
    else if(status == RGR_CFG_CFM_NOK)
    {
           RLOG0(L_ERROR,"Warning SI's CFG CFM failed.");
           wrPwsHndlSchdCfgFail(cellId, transId);
    }

   RETVALUE(ROK);
}/*wrEmmSchdWrngSiCfgCfm*/



/* PH04_CMAS : Added for CMAS feature. This function is added for pahse2.
 * It will not be called for Pahse1 features.*/
PUBLIC S16 wrEmmSchdStopWrngSiCfgCfm
(
U32                          transId,
U8                           status
)
{
  U16                        Id;
  WrCellId                   cellId;

  /*Parsing the Ids from transaction ID*/
  Id = transId & 0xffff;
  cellId = transId >> 16;

    if(status == RGR_CFG_CFM_OK)
    {
         RLOG0(L_INFO,
                    "Stop Warning SI's CFG CFM is success");
         wrPwsHndlStopWrngSiCfgCfm(cellId, transId);
    }
    else if(status == RGR_CFG_CFM_NOK)
    {
           RLOG0(L_ERROR,
                        "Stop Warning SI's CFG CFM failed.");
           /*wrPwsHndlSchdCfgFail(cellId, transId);*/
    }

   RETVALUE(ROK);
}/*wrEmmSchdStopWrngSiCfgCfm*/

/* PH04_CMAS : end */





PUBLIC S16 wrEmmPrcS1StaInd
(
SztStaInd                    *sztSta
)
{
   /* TODO: Manage MME association failure here */
   wrEmmMmePrcStaInd(sztSta);
   RETVALUE(ROK);
}

/** @brief This function is used to fetch the DL Carrier Frequency for
 *         the corresponding cellId
 *
 * @details
 *
 *     Function: wrEmmGetServCarrierFreq
 *
 *         Processing steps:
 *         - get the corresponding cellCb from the cellId
 *         - return the dlCarrierFreq
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */
PUBLIC U16 wrEmmGetServCarrierFreq
(
U16                          cellId
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cell id received while getting serving carrier frequency");
      RETVALUE(RFAILED);
   }
   
   RETVALUE(cellCb->dlCarrierFreq);
}

/** @brief This function is used to fetch the PCI for the corresponding cellId
 *
 * @details
 *
 *     Function: wrEmmGetPhyCellId
 *
 *         Processing steps:
 *         - get the corresponding cellCb from the cellId
 *         - return the physCellId
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */
PUBLIC U16 wrEmmGetPhyCellId
(
U16                          cellId
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for getting PCI");
      RETVALUE(RFAILED);
   }
   RETVALUE(cellCb->physCellId);
}

/* SPS changes starts */
/** @brief This function is used to Indicate if it is FDD or TDD
 *
 * @details
 *
 *     Function: wrEmmGetMaxUeSupport
 *
 *         Processing steps:
 *         - get the corresponding cellCb from the cellId
 *         - return the maxUeSupp
 *
 * @param[in]   cellId : cell identifier
 * @return S16
 */
PUBLIC S16 wrEmmGetMaxUeSupport
(
U16                          cellId,
U16                          *pMaxUeSupp
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received for"
                " getting max UEs supported.");
      RETVALUE(RFAILED); 
   }
   *pMaxUeSupp = (U16)cellCb->maxUeSupp;
   RETVALUE(ROK);
}

/** @brief This function is used to Indicate if it is FDD or TDD
 *
 * @details
 *
 *     Function: wrEmmGetDuplexMode
 *
 *         Processing steps:
 *         - get the corresponding cellCb from the cellId
 *         - return the duplexmode
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */
PUBLIC U16 wrEmmGetDuplexMode
(
U16                          cellId
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received while getting duplex mode.");
      RETVALUE((U16)(-1)); /* To avoid the value being taken if value of macro for FDD/TDD 
                              is changed in future, so 0xffff. Typecasted -1 so that it will 
                              look like a failure */
   }
   RETVALUE(cellCb->duplexMode);
}
/* SPS changes ends */

PUBLIC S16 wrEmmGetIntraLteHoNghInfo
(
U16                            cellId,   
WrHoNghInfo                     *hoNghInfo
)
{
   S16                           ret = ROK;
   WrCellCb                      *cellCb = NULLP;
   WrEutranNeighCellCb           *neighCellCb = NULLP;
   WrNeighEnbCb                  *neighEnbCb  = NULLP;
   WrEutranNbrCellInfo            nrInfo;
   WrUmmIntraLteHoNghInfo        *intraLteHoNghInfo = 
                                 &(hoNghInfo->t.intraLteNghInfo);

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId for getting intra Lte"
                "HO neighbour information.");
      RETVALUE(RFAILED);
   }
   nrInfo.phyCellId = intraLteHoNghInfo->pci;
   nrInfo.earfcn    = intraLteHoNghInfo->earfcn;
   neighCellCb = wrEmmAnrGetEutranNr(&cellCb->nrCb->eutranCb, &nrInfo);

   if(neighCellCb == NULLP)
   {
      RLOG0(L_WARNING, "Failed to get neighbor cell control block");
      RETVALUE(RFAILED);
   }
   neighCellCb->numTtiTicks = numTtiTicks;

   /*Finding the neighbour eNodeB control block*/
   ret = cmHashListFind(&(wrEmmCb.neighEnbLstCp),
         (U8 *)&neighCellCb->enbId,sizeof(neighCellCb->enbId),0,(PTR *)&neighEnbCb);
   if( ret != ROK)
   {
      RLOG1(L_WARNING, "Failed To get neighbor eNodeB control block [enbId : %ld]",
              neighCellCb->enbId);
      RETVALUE(RFAILED);
   }

   hoNghInfo->peerId         = neighEnbCb->peerId;
   intraLteHoNghInfo->enbType        = neighEnbCb->enbType;
 
   /* Cell identity copyig to neighbour enodeb Control block*/
   intraLteHoNghInfo->eutranCellId   = neighCellCb->eCgi.eutranCellId;
   cmMemcpy((U8 *)&hoNghInfo->plmnId,
        (U8 *)&neighCellCb->enbPlmn, sizeof(neighCellCb->enbPlmn));
   intraLteHoNghInfo->neighEnbAddr   = neighEnbCb->neighEnbAddr;

   intraLteHoNghInfo->neighEnbCb   = (PTR)neighEnbCb;
   hoNghInfo->neighCellCb  = (PTR)neighCellCb;

   intraLteHoNghInfo->isX2Connected  = neighEnbCb->x2Connection;
   intraLteHoNghInfo->tac            = neighCellCb->tac;
   /* ccpu00126532 */
   intraLteHoNghInfo->pci            = neighCellCb->pci;
   intraLteHoNghInfo->earfcn         = neighCellCb->earfcnDl;
   /* CSG_DEV Start */
   hoNghInfo->csgId                = neighCellCb->csgId;
   hoNghInfo->isCsgInfoDiscovered  = neighCellCb->isCsgIdResolved;
   hoNghInfo->csgCellAccessMode    = neighCellCb->csgAccessMode;
   /* CSG_DEV Ends */

   RETVALUE(ROK);
}

/** @brief This function is used to fetch the number of antenna's for the enodeb
 *
 * @details
 *
 *     Function: wrEmmGetNumTxAnt
 *
 *         Processing steps:
 *         - return the number of antenna
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */
PUBLIC U8 wrEmmGetNumTxAnt
(
)
{
   RLOG1(L_INFO,"Number of TX antenna = %d",wrEmmCb.numTxAntenna);
   RETVALUE(wrEmmCb.numTxAntenna);
}
/** @brief This function is used to fetch the PCI for the corresponding cellId
 *
 * @details
 *
 *     Function: wrEmmGetPhyCellId
 *
 *         Processing steps:
 *         - get the corresponding cellCb from the cellId
 *         - return the physCellId
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */
PUBLIC U32 wrEmmGetRrcUeCapTmrVal
(
U16                          cellId
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Failed to get RRC UE capability timer value.Invalid cellId received");
      RETVALUE(RFAILED);
   }
   RETVALUE(cellCb->tmrCfg.rrcUeCapabilityEnquiryTmr);
}

/** @brief This function is used to fetch the PCI for the corresponding cellId
 *
 * @details
 *
 *     Function: wrEmmGetPhyCellId
 *
 *         Processing steps:
 *         - get the corresponding cellCb from the cellId
 *         - return the physCellId
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */


PUBLIC U32 wrEmmGetRrcSecModTmrVal
(
U16                          cellId
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for getting"
                "RRC security mode timer value");
      RETVALUE(RFAILED);
   }
   RETVALUE(cellCb->tmrCfg.rrcSecurityModeCommandTmr);
}



/** @brief This function is used to fetch the PCI for the corresponding cellId
 *
 * @details
 *
 *     Function: wrEmmGetPhyCellId
 *
 *         Processing steps:
 *         - get the corresponding cellCb from the cellId
 *         - return the physCellId
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */
PUBLIC U32 wrEmmGetRrcConTmrVal
(
U16                          cellId
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG1(L_ERROR,"Invalid cell id [%d] received for getting RRC connection timer"
                    "Value.",cellId);
      RETVALUE(RFAILED);
   }
   RETVALUE(cellCb->tmrCfg.rrcConnectionSetupTmr);
}


/** @brief This function is used to fetch the PCI for the corresponding cellId
 *
 * @details
 *
 *     Function: wrEmmGetPhyCellId
 *
 *         Processing steps:
 *         - get the corresponding cellCb from the cellId
 *         - return the physCellId
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */
PUBLIC U32 wrEmmGetRrcReestabTmrVal
(
U16                          cellId
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received for getting RRC"
                "connection re-establishment timer value.");
      RETVALUE(RFAILED);
   }
   RETVALUE(cellCb->tmrCfg.rrcConnectionReEstablishTmr);
}


/** @brief This function is used to fetch the PCI for the corresponding cellId
 *
 * @details
 *
 *     Function: wrEmmGetPhyCellId
 *
 *         Processing steps:
 *         - get the corresponding cellCb from the cellId
 *         - return the physCellId
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */
PUBLIC U32 wrEmmGetRrcConRelTmrVal
(
U16                          cellId
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for"
                "getting RRC connection release timer value");
      RETVALUE(RFAILED);
   }
   RETVALUE(cellCb->tmrCfg.rrcConnectionReleaseTmr);
}

/** @brief This function is used to fetch RRC reconfiguration timer value
 *
 * @details
 *
 *     Function: wrEmmGetRrcReCfgTmrVal 
 *
 *         Processing steps:
 *         - get the RRC Reconfig timer value.
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */
PUBLIC U32 wrEmmGetRrcReCfgTmrVal
(
U16                          cellId
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for getting RRC"
                "connection reconfiguration timer value.");
      RETVALUE(RFAILED);
   }
   RETVALUE(cellCb->tmrCfg.rrcConnectionReconfigTmr);
}

/** @brief This function is used to fetch IRAT304 timer value
 *
 * @details
 *
 *     Function: wrEmmGetT304IRATTmrVal 
 *
 *         Processing steps:
 *         - get the CSFB IRAT 304 timer value.
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */
PUBLIC S16 wrEmmGetT304IRATTmrVal
(
U16                          cellId,
U32                          *val
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for getting"
                "T304 IRAT timer value");
      RETVALUE(RFAILED);
   }
   *val = cellCb->ueTmrConstants.ccoT304;
   RETVALUE(ROK);
}

/** @brief This function is used to fetch 301 timer value
 *
 * @details
 *
 *     Function: wrEmmGetT301TmrVal
 *
 *         Processing steps:
 *         - get the 301 timer value.
 *
 * @param[in]   cellId : cell identifier
 * @return U16
 */
PUBLIC S16 wrEmmGetT301TmrVal
(
 U16     cellId,
 U32     *val
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId,"Invalid cellId received for getting"
                "T301 timer value.");
      RETVALUE(RFAILED);
   }  
   /* RSYS OAM will provide enum value for T301 timer
    * Coverting into timer value */
   switch(cellCb->sib2.t301)
   {
      case   WR_SIB2_T301VAL100MS:
         *val = 100;
         break;
      case   WR_SIB2_T301VAL200MS:
         *val = 200;
         break;
      case   WR_SIB2_T301VAL300MS:
         *val = 300;
         break;
      case   WR_SIB2_T301VAL400MS:
         *val = 400;
         break;
      case   WR_SIB2_T301VAL600MS:
         *val = 600;
         break;
      case   WR_SIB2_T301VAL1000MS:
         *val = 1000;
         break;
      case   WR_SIB2_T301VAL1500MS:
         *val = 1500;
         break;
      case   WR_SIB2_T301VAL2000MS:
         *val = 2000;
         break;
   }

   RETVALUE(ROK);
} 

/** @brief This function is used to fetch timer values
 *
 * @details
 *
 *     Function: wrEmmGetTmrVal  
 *
 *         Processing steps:
 *         - get the corresponding cellCb from the cellId
 *         - return timer value 
 *
 * @param[in]   cellId  : cell identifier
 * @param[in]   tmr     : wrUmmTmrVal
 * @param[out]  *tmrval : Timer value
 * @return S16
 */
PUBLIC S16 wrEmmGetTmrVal
(
 U16                          cellId,
 U32                          tmr,
 U32                          *tmrval
 )
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for getting"
                "timer value.");
      RETVALUE(RFAILED);
   }
   *tmrval = 0;
   switch (tmr)
   {
      case WR_UMM_INT_LTE_S1_PREP_TMR: 
         {
            *tmrval = cellCb->tmrCfg.s1HandoverPrepTmr;
         }
         break;
      case WR_UMM_IRAT_S1_PREP_TMR:
         {
            *tmrval = cellCb->tmrCfg.s1IRatHandoverPrepTmr;
         }
         break;

      case WR_UMM_INT_LTE_S1_OVRL_TMR:
         {
            *tmrval = cellCb->tmrCfg.s1RelocOverallTmr;
         }
         break;

      case WR_UMM_IRAT_S1_OVRL_TMR:
         {
            *tmrval = cellCb->tmrCfg.s1IRatRelocOverallTmr;
         }
         break;
         /* TODO: Same API can be used to fetch the timer other timer
            values instead of having seperate API for each */
      /*ccpu00137962 start*/
      case WR_TMR_SRC_HO_CANCEL:        
         {        
            *tmrval = cellCb->tmrCfg.s1HandoverCancelTmr;        
         } 
         break;
      /*ccpu00137962 end*/
      default:
         {
            RLOG1(L_ERROR, "Invalid or *UNDEFINED* timer [%d]",tmr);
            RETVALUE(RFAILED);
         }
   }
   RETVALUE(ROK);
}

/** @brief This function is used to fetch the PCI for the corresponding cellId
 *
 *   Function: wrEmmGetOffsetFreqFrmArfcn
 *
 *   Processing steps:
 *     - Update the Structure
 *
 * @param[in] cellId  : cellIdentifier
 * @param[in] arfcn   : Arfcn
 * @return S16
 *     -# Success : LCM_REASON_NOT_APPL
 *     -# Failure  : LCM_REASON_INVALID_PAR_VAL
 */
PUBLIC S16 wrEmmGetOffsetFreqFrmArfcn
(
 U16                         cellId,
 U16                         arfcn,
 U8                          *offsetFreq
)
{
   WrEutranFreqCb             *neighFreqCb = NULLP;
   CmLList                    *neighFreqLnk = NULLP;
   WrCellCb                   *cellCb;

   TRC2(wrEmmGetOffsetFreqFrmArfcn)
      
   WR_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received for getting"
                " offset frequency from ArFcn.");
      RETVALUE(RFAILED);
   }

   neighFreqLnk = cellCb->eutranNbrFreqLst.first;
   while(neighFreqLnk != NULLP)
   {
      neighFreqCb = (WrEutranFreqCb*)neighFreqLnk->node;
      if(neighFreqCb->dlEarfcn == arfcn)
      {
         *offsetFreq  = neighFreqCb->offsetFreq;
         RETVALUE(ROK);
      }
      neighFreqLnk = neighFreqLnk->next;
   }
   RETVALUE(RFAILED);
}

/** @brief This function is used to fetch the UTRAN Frequency control block
 *         for the corresponding DL ARFCN
 *
 *   Function: wrEmmAnrGetFreqNode
 *
 *   Processing steps:
 *     - return the utran cb if the arfcn value matches
 *
 * @param[in] arfcnDl : ARFCN
 * @param[in] utranCb : Neighbour UTRAN CB
 * @param[out] neighCellCb : pointer to WrNrUtranFreqCb 
 */
PUBLIC WrNrUtranFreqCb* wrEmmAnrGetFreqNode
(
   U16                           arfcnDl,
   WrNrUtranCb                   *utranCb
)
{
   CmLListCp                     *lst = NULLP;
   CmLList                       *lnk = NULLP;
   WrNrUtranFreqCb               *lnkFreqNode = NULLP;

   TRC2(wrEmmAnrGetUtranFreqNode);

   lst = &utranCb->utranFreqList;

   lnk = lst->first;

   while(lnk != NULLP)
   {
      lnkFreqNode = (WrNrUtranFreqCb*)lnk->node;

      if(lnkFreqNode->arfcn == arfcnDl)
      {
         RETVALUE(lnkFreqNode);
      }
      lnk = lnk->next;
   }

   RETVALUE(NULLP);

} /* end of wrEmmAnrGetFreqNode */
/** @brief This function is used to fetch the cell node for the corresponding
 * arfcn and psc
 *  *   Function: wrEmmGetNeighUtraCell
 *   *   Processing steps: 
 *    *     - Update the utraSI for each PSC  
 *     * @param[in] arfcn : ARFCN
 *      * @param[in] psc   : PSC
 *       * @param[out] utraSi: pointer to TknStrOSXL of WrUmmUtraCellInfo  in
 *       WrUmmUtraRedirectInfo */
PUBLIC S16 wrEmmGetNeighUtraSIInfo
(
WrUmmRedirectInfo       *redirectInfo
)
{

   WrNrUtranFreqCb         *freqNode        = NULLP;
   WrUtraNeighCellCb       *nbrUtraCellNode = NULLP;
   WrUmmUtraRedirectInfo   *utraRedirectInfo = &redirectInfo->val.utraInfo;
   WrCellCb                *emmCellCb   = NULLP;
   CmLListCp               *utraCellLst = NULLP;
   CmLList                 *nbrUtraCelllnk = NULLP;
   U8                      index1           = 0;
   U8                      index2          = 0;

   TRC2(wrEmmGetNeighUtraSIInfo);
   WR_GET_CELLCB(emmCellCb, redirectInfo->cellId);
   if (NULLP == emmCellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, redirectInfo->cellId, "Invalid CellId"
                "received for getting neighbour Utra SI information");
      RETVALUE (RFAILED);
   }

   freqNode = wrEmmAnrGetFreqNode(utraRedirectInfo->arfcn, 
         &emmCellCb->nrCb->utranCb);

   if (NULLP == freqNode)
   {
      RLOG1 (L_WARNING, "Failed to get neighbour Utra SI information.ArfcnDl is invalid [%d]"
            ,utraRedirectInfo->arfcn);
      RETVALUE (RFAILED);
   }
   /*Get the UTRA cell node */
   utraCellLst = &freqNode->nrWlLst;
   if (NULLP == utraCellLst)
   {
      RLOG0 (L_WARNING, "Failed to get neighbour Utra SI information.UtraCellLst is NULL");
      RETVALUE (RFAILED);
   }

   for(index1 = 0, index2 = 0; 
         index1 < utraRedirectInfo->noOfPscs; index1++)
   {
      /*Re Initialize the utraLnk to check UTRA cells from the starting
       * so that previous PCI also will be checked with the UTRA redirection
       * List which has SI */
      nbrUtraCelllnk = utraCellLst->first;
      while (nbrUtraCelllnk != NULLP)
      { 
         nbrUtraCellNode = (WrUtraNeighCellCb*) nbrUtraCelllnk->node;
         if (nbrUtraCellNode != NULLP)
         {
            if(nbrUtraCellNode->t.utraFdd.psc == 
                  utraRedirectInfo->utraCellRedirInfo[index1].pscId)
            {
               /* Copy the utra SI information and update PSC 
                * in order of SI available which shall be used
                * when filling Redirection with SI
                * */
               if(nbrUtraCellNode->utraSI.pres == PRSNT_NODEF )
               {
                  utraRedirectInfo->utraCellRedirInfo[index2].utraSI =
                     nbrUtraCellNode->utraSI;
                  utraRedirectInfo->utraCellRedirInfo[index2].pscId = 
                     nbrUtraCellNode->t.utraFdd.psc;
                  /*increment the index for updated PSC which has SI
                   * Note: we will be overriting the PSC with previous 
                   * SI (both with and without Meas Case*/
                  index2++;
               }
               break;
            }
         }/*end of if*/
         nbrUtraCelllnk = nbrUtraCelllnk->next;
      }/*end of while*/
   }/*end of for loop*/

   /*update the noOf PSC with selcted PSC which has SI information */
   utraRedirectInfo->noOfPscs = index2;

   RETVALUE(ROK);
}


/** @brief This function is used to fetch the UTRA TDD cell node for the corresponding
 * arfcn and psc
 *  *   Function: wrEmmGetNeighUtraTddSIInfo
 *   *   Processing steps: 
 *    *     - Update the utraSI for each PSC  
 *     * @param[in] arfcn : measUTRA with UTRA PSC
 *      * @param[in] psc   : redirInfo with ARFCN and Cell Id
 *       * @param[out] WrUmmUtraTddRedirectInfo: Nos of PSCs with SI
 *       and all params of WrUmmUtraTddCellRedirInfo.
 *        */
PUBLIC Bool wrEmmGetNeighUtraTddSIInfo
(
 U16         psc,
 U16         arfcn,
 U16         cellId,
 TknStrOSXL  *utraSI
 )
{
   WrUtraNeighCellCb    *utraNeighCellCb = NULLP;

   utraNeighCellCb = (WrUtraNeighCellCb *) wrEmmGetNeighUtraCell(cellId,
         arfcn, psc, WR_UTRA_MODE_TDD);

   if(NULLP == utraNeighCellCb)
   {
      RETVALUE(FALSE);
   }

   if(utraNeighCellCb->utraSI.pres == PRSNT_NODEF)
   {
      (*utraSI) = utraNeighCellCb->utraSI;
      RETVALUE(TRUE);
   }

   RETVALUE(FALSE);
}


/** @brief This function is used to fetch the cell node for the corresponding 
 * arfcn and psc
 *
 *   Function: wrEmmGetUtranCb
 *
 *   Processing steps:
 *     - Update the Structure
 *
 * @param[in] arfcn : cellId
 * @param[in] psc   : duplexMode
 * @param[out] utranCb : pointer to WrUtranCb 
 *
 */
PUBLIC WrNrUtranCb* wrEmmGetUtranCb
(
 WrCellId			          cellId,
 U8                         duplexMode
)
{
   WrCellCb                  *cellCb  = NULLP;

   TRC2(wrEmmGetUtranCb);

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for getting"
                "Utran Cb.");
      RETVALUE(NULLP);
   }

   /* Get Frequency Node for UTRA FDD */
	if (duplexMode == WR_UTRA_MODE_FDD)
   {
	   return (&cellCb->nrCb->utranCb);
	}
   else if (duplexMode == WR_UTRA_MODE_TDD)
   {
      /* Get Frequency Node for UTRA TDD */
      return (&cellCb->nrCb->utranTddCb);
   }

   RETVALUE(NULLP);
}
/** @brief This function is used to fetch the cell node for the corresponding 
 *  arfcn and bsic
 *
 *   Function: wrEmmGetNeighGeranCell
 *
 *   Processing steps:
 *     - Update the Structure
 *
 * @param[in] arfcn : ARFCN
 * @param[in] bsic   : BSIC
 * @param[out] neighCellCb : pointer to WrNrGeranFreqCb 
 *
 */
PUBLIC WrGeranCellCb* wrEmmGetNeighGeranCell
(
 U16                          cellId,
 U16                          arfcn,
 U16                          bsic 
)
{
   WrGeranCellCb             *lnkCellNode =NULLP;
   WrCellCb                  *cellCb  = NULLP;
   CmLListCp                 *lst = NULLP;
   CmLList                   *lnk = NULLP;

   TRC2(wrEmmGetNeighGeranCell);

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG1(L_ERROR, "Invalid cellId [%d] received for getting neighbour geran cell.",cellId);
      RETVALUE(NULLP);
   }

   /* Get Frequency Node for Geran */
   lst= &cellCb->nrCb->geranCb.geranNrCellLst;
   lnk = lst->first;

   while(lnk != NULLP)
   {
      lnkCellNode = (WrGeranCellCb*)lnk->node;

      if(((lnkCellNode->freqCb != NULLP) &&
          (lnkCellNode->freqCb->arfcn == arfcn)) && (lnkCellNode->bsic == bsic))
      {
         RETVALUE(lnkCellNode);
      }
      lnk = lnk->next;
   }

   RETVALUE(NULLP);
}

/** @brief This function is used to fetch the cell node for the corresponding 
 *  arfcn and psc
 *
 *   Function: wrEmmGetNeighUtraCell
 *
 *   Processing steps:
 *     - Update the Structure
 *
 * @param[in] arfcn : ARFCN
 * @param[in] psc   : PSC
 * @param[out] neighCellCb : pointer to WrNrUtranFreqCb 
 *
 */
PUBLIC WrUtraNeighCellCb* wrEmmGetNeighUtraCell
(
 U16                           cellId,
 U16                          arfcn,
 U16                          psc, 
 U8 									duplexMode
)
{
   WrNrUtranFreqCb           *freqNode = NULLP;
   WrUtraNeighCellCb         *lnkCellNode =NULLP;
   WrCellCb                  *cellCb  = NULLP;
   CmLListCp                 *lst = NULLP;
   CmLList                   *lnk = NULLP;
   
   TRC2(wrUmmGetNeighUtraCell);

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG1(L_ERROR,"Invalid CellId [%d] received for getting neighbour Utra cell.",cellId);
      RETVALUE(NULLP);
   }

   /* Get Frequency Node for UTRA FDD */
	if (duplexMode == WR_UTRA_MODE_FDD)
	{
	   freqNode = wrEmmAnrGetFreqNode(arfcn,
	         &cellCb->nrCb->utranCb);

	   if(freqNode != NULLP)
	   {
	      /*Get the cell node */
	      lst = &freqNode->nrWlLst;
	      lnk = lst->first;
	      while(lnk != NULLP)
	      {
	         lnkCellNode = (WrUtraNeighCellCb*)lnk->node;

	         if(lnkCellNode->t.utraFdd.psc== psc)
	         {
	            RETVALUE(lnkCellNode);
	         }
	         lnk = lnk->next;
	      }
	   }
	}
   else if (duplexMode == WR_UTRA_MODE_TDD)
   {
      /* Get Frequency Node for UTRA TDD */
      freqNode = wrEmmAnrGetFreqNode(arfcn,
            &cellCb->nrCb->utranTddCb);
      if(freqNode != NULLP)
      {
         /*Get the cell node */
         lst = &freqNode->nrWlLst;
         lnk = lst->first;
         while(lnk != NULLP)
         {
            lnkCellNode = (WrUtraNeighCellCb*)lnk->node;

            if(lnkCellNode->t.utraTdd.psc== psc)
            {
               RETVALUE(lnkCellNode);
            }
            lnk = lnk->next;
         }
      }
   }

   RETVALUE(NULLP);
}

/** @brief This function is used to copy the peer IP address if the peer id
 *         matches.
 *
 *   Function: wrEmmGetNeighIpFrmPeerId
 *
 *   Processing steps:
 *     - Update the Structure if the peerId matches
 *
 * @param[in] peerId : peerId
 * @param[in] cellId : cellId
 * @param[out] peerAddr : peer IP Address
 */
PUBLIC U16 wrEmmGetNeighIpFrmPeerId
(
U32                          peerId,
U8                           cellId,
CmTptAddr                    *peerAddr
)
{
   U16                       retVal;
   PTR                       prevEntry = NULLP;
   WrNeighEnbCb              *nghEnbCb;

   while ((retVal = cmHashListGetNext(&(wrEmmCb.neighEnbLstCp), prevEntry, (PTR *) &nghEnbCb)) == ROK)
   {
      if (nghEnbCb->peerId == peerId)
      {
         wrCpyCmTptAddr(peerAddr, &(nghEnbCb->neighEnbAddr));
         RETVALUE(ROK);
      }
      prevEntry = (PTR) nghEnbCb;
   }
   RLOG1(L_DEBUG,"No matching PeerId found for %d",peerId);
   RETVALUE(RFAILED);
}

/** @brief This function is used to start the Trice timer.
 *
 *   Function: wrEmmStartTriceTmr
 *
 *   Processing steps:
 *     - based on epoch count value start trice timer.
 *
 * @param[in] cellCb: pointer to cell Cb
 */
PUBLIC S16 wrEmmStartTriceTmr
(
 WrCellCb                 *cellCb
)
{
   U64                      epochTmrVal = 0;
   U16                      epochTmrCnt = 0;
   U32                      trcCount = 0;
   U32                      triceTmrVal = 0;
   U32                      triceCntPerEpocIntv = 0;

   TRC2(wrEmmStartTriceTmr)
   epochTmrCnt = cellCb->nrCb->epochCnt;
   epochTmrVal = cellCb->nrCb->epochTmrVal;
   triceCntPerEpocIntv = cellCb->nrCb->tricePeriodIntvCnt;

   if(epochTmrCnt == 1)
   {
      trcCount = WR_UMM_FIRST_EPOC_INTERVAL;
   }
   else if((epochTmrCnt > 1) && (epochTmrCnt <= 5))
   {
      trcCount = WR_UMM_ONE_TO_FIVE_EPOC_INTERVAL;
   }
   else
   {
      trcCount = WR_UMM_ABOVE_FIVE_EPOC_INTERVAL;
   }
   triceTmrVal = (epochTmrVal/triceCntPerEpocIntv) * trcCount;

   wrStartTmr((PTR)cellCb, WR_TMR_ANR_MEAS_TRICE_TMR, triceTmrVal);
   RLOG1(L_DEBUG, 
       "Current Epoch Timer Count is = %u while starting the trice timer",cellCb->nrCb->epochCnt);
   RLOG1(L_DEBUG, 
       "Started Trice timer with value = %u",triceTmrVal);
   wrEmmCfgAnrMeasFreqInfo(cellCb);
   RETVALUE(ROK);
}

/** @brief This function checks does P-ANR Timers that is EPOCH and TRICE 
 *         are running or not. 
 *
 *   Function: wrEmmIsPrdAnrTmrsRunning
 *
 *   Processing steps:
 *     - From the Cell Id get the CellCb
 *     - Check Epoch and Trice timers are running or not.
 *
 * @param[in] cellId: Cell Id
 * @param[in] running:  If the timers are running set this flag to true.
 */
PUBLIC S16 wrEmmIsPrdAnrTmrsRunning
(
   WrCellId                 cellId,
   Bool                     *running
)
{
   WrCellCb                  *cellCb  = NULLP;

   /* initialise the output */
   *running = FALSE;

   /* Get the EMM Cell CB */
   WR_GET_CELLCB(cellCb, cellId);
   if(cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "Invalid cell id received to check timers running or not");
      RETVALUE(RFAILED);
   }


   /*Check the EPOCH timer is running or not. 
    * If the EPOCH timer is not running then start the EPOCH and Trice timers
    * set the output flag to TRUE */
   if(wrIsTmrRunning(&cellCb->nrCb->epochTmr, 
                                 WR_TMR_ANR_MEAS_EPOCH_TMR) == FALSE)

   {
      /* Start the EPOCH timer */
      wrStartTmr((PTR)cellCb, WR_TMR_ANR_MEAS_EPOCH_TMR, 
                 cellCb->nrCb->epochTmrVal);

      /* Start the Trice timer */
      wrEmmStartTriceTmr(cellCb);

      /* Set the flag to TRUE */
      *running = TRUE;
   }
   else
   {
      /* EPOCH timer is running check Trice Timer is running */
      if(wrEmmAnrIsTriceTmrRunning(cellId) == TRUE)
      {
         /* Set the flag to TRUE */
         *running = TRUE;
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is used to handle epoch timer expiry
 *
 *   Function: wrEmmPrcEpochTimerExp
 *
 *   Processing steps:
 *     - increment epoch count value and start epoch and trice timers
 *
 * @param[in] Cb: pointer to cell Cb
 * @param[in] wrUmmTmrExpEvnt: Timer Event
 */
PUBLIC S16 wrEmmPrcEpochTimerExp
(
Void                         *cb,
U8                           wrUmmTmrExpEvnt
)
{
   WrCellCb                  *cellCb;
   U32                       temp;
   U32                       epochTmrVal = 0;
   TRC2(wrEmmPrcEpochTimerExp)

   cellCb      = cb;
   cellCb->nrCb->epochCnt++;

   epochTmrVal = cellCb->nrCb->epochTmrVal;
   temp = WR_UMM_NUMOF_MSECS_PERDAY/epochTmrVal;
   if(cellCb->nrCb->epochCnt >= temp)
   {
      cellCb->nrCb->epochCnt = 1;
   }
   wrStartTmr((PTR)cellCb, WR_TMR_ANR_MEAS_EPOCH_TMR, epochTmrVal);
   wrEmmStartTriceTmr(cellCb);
   wrUmmAnrStartMeasurements(cellCb->cellId);

   RETVALUE(ROK);
}
/** @brief This function is used to handle trice timer expiry
 *
 *   Function: wrEmmPrcTriceTimerExp
 *
 *   Processing steps:
 *     - Delete Anr measuremnt configuration for all UE's
 *
 * @param[in] Cb: pointer to cell Cb
 * @param[in] wrUmmTmrExpEvnt: Timer Event
 */
PUBLIC S16 wrEmmPrcTriceTimerExp
(
Void                         *cb,
U8                           wrUmmTmrExpEvnt
)
{
   WrCellCb               *cellCb;

   TRC2(wrEmmPrcTriceTimerExp)
   cellCb      = (WrCellCb *)cb;
   wrUmmAnrDelMeasCfgForAllUe(cellCb->cellId);
   RETVALUE(ROK);
}

/** @brief This function is used to handle the cell's max age timer expiry
 *
 *   Function: wrEmmPrcCellMaxAgeTmrExp
 *
 *   Processing steps:
 *     - Check all the Neighbour cells for which the source is UE meas
 *     - If the time duration is greater than the max age configured,
 *       delete the cell from NRT
 *
 */
PUBLIC S16 wrEmmPrcCellMaxAgeTmrExp
(
Void                         *cb
)
{
   WrCellCb               *cellCb;
   WrNrEutranFreqCb       *intraFreqCb = NULLP;
   CmLListCp              *interFreqLst = NULLP;
   CmLList                *interFreqNodeLnk = NULLP;
   WrNrEutranFreqCb       *interFreqNode = NULLP;

   CmLListCp              *nbrCellLst = NULLP;
   CmLList                *nbrCellLnk = NULLP;
   WrEutranNeighCellCb    *nbrCellNode = NULLP;
   U32                     actualTmrVal;
 
   TRC2(wrEmmPrcCellMaxAgeTmrExp)

   cellCb      = (WrCellCb *)cb;

   /* Check in Inter frequency nodes */
   interFreqLst = &cellCb->nrCb->eutranCb.interFreqList; 
   interFreqNodeLnk = interFreqLst->first;

   while (interFreqNodeLnk != NULLP)
   {
      interFreqNode = (WrNrEutranFreqCb *)interFreqNodeLnk->node;

      /* White list */
      nbrCellLst = &interFreqNode->nrWlLst;
      nbrCellLnk = nbrCellLst->first;

      while(nbrCellLnk != NULLP)
      {
         nbrCellNode = (WrEutranNeighCellCb *)nbrCellLnk->node;
         nbrCellLnk = nbrCellLnk->next;
         if(nbrCellNode != NULLP)
         {
            wrEmmCheckAgeAndDeleteCell(cellCb,nbrCellNode);
         }
      }

      /* Black list */
      nbrCellLst = &interFreqNode->nrBlLst;
      nbrCellLnk = nbrCellLst->first;

      while(nbrCellLnk != NULLP)
      {
         nbrCellNode = (WrEutranNeighCellCb *)nbrCellLnk->node;
         nbrCellLnk = nbrCellLnk->next;
         if(nbrCellNode != NULLP)
         {
            wrEmmCheckAgeAndDeleteCell(cellCb,nbrCellNode);
         }
      }
      interFreqNodeLnk = interFreqNodeLnk->next;
   }

   /* Now check the intra frequency nodes */
   intraFreqCb = &cellCb->nrCb->eutranCb.intraFreqCb;

   /* White list */
   nbrCellLst = &intraFreqCb->nrWlLst;
   nbrCellLnk = nbrCellLst->first;

   while(nbrCellLnk != NULLP)
   {
      nbrCellNode = (WrEutranNeighCellCb *)nbrCellLnk->node;
      nbrCellLnk = nbrCellLnk->next;
      if(nbrCellNode != NULLP)
      {
         wrEmmCheckAgeAndDeleteCell(cellCb,nbrCellNode);
      }
   }

   /* Black list */
   nbrCellLst = &intraFreqCb->nrBlLst;
   nbrCellLnk = nbrCellLst->first;

   while(nbrCellLnk != NULLP)
   {
      nbrCellNode = (WrEutranNeighCellCb *)nbrCellLnk->node;
      nbrCellLnk = nbrCellLnk->next;
      if(nbrCellNode != NULLP)
      {
         wrEmmCheckAgeAndDeleteCell(cellCb,nbrCellNode);
      }
   }

   if (cellCb->nrCb->cellValidAgeTmrVal >= 10)
   {
      actualTmrVal = (10 * 60 * 1000);
   }
   else
   {
      actualTmrVal = (cellCb->nrCb->cellValidAgeTmrVal * 60 * 1000);
   }

   wrStartTmr((PTR)cellCb, WR_TMR_ANR_CELL_MAX_AGE_TMR,actualTmrVal);
   RLOG1(L_INFO, "Started cell valid age timer with value = %llu",
    actualTmrVal);

   RETVALUE(ROK);
}

/** @brief This function is used to verify whether trice timer is running or
 * not.
 *
 *   Function: wrEmmAnrIsTriceTmrRunning
 *
 *   Processing steps:
 *     - check the trice timer is rumnning or not 
 *     - if running return TRUE else FALSE
 *
 * @param[in] cellId: Cel identity   
 */
PUBLIC Bool wrEmmAnrIsTriceTmrRunning
(
  U16                         cellId
)
{
   Bool                      isTmrRunning = FALSE; 
   WrCellCb                  *cellCb  = NULLP;

   TRC2(wrEmmAnrIsTriceTmrRunning)

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received for checking trice timer is running or not");
      RETVALUE(RFAILED);
   }

   isTmrRunning = wrIsTmrRunning(&cellCb->nrCb->triceTmr, 
         WR_TMR_ANR_MEAS_TRICE_TMR);
   RETVALUE(isTmrRunning);
}


/*Removed DRX feature flag*/
/*
 *       Fun:     wrEmmGetAnrDrxOnDur
 *
 *       Desc:    This primitive returns the DRX On duration Timer 
 *                value for ANR.
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC U16 wrEmmGetAnrDrxOnDur
(
WrCellId                     cellId
)
{
   WrCellCb                  *cellCb;
   U16                       drxOnDur; 

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received for getting DRX ON duration timer.");
      RETVALUE(RFAILED);
   }
   
   drxOnDur = cellCb->ueAnrDrxCfg.drxOnDurTmr;
   RETVALUE(drxOnDur);
}

/*
 *       Fun:     wrEmmGetAnrDrxLongCycle
 *
 *       Desc:    This primitive returns the DRX Long Cycle
 *                value for ANR.
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC U16 wrEmmGetAnrDrxLongCycle
(
WrCellId                     cellId
)
{
   WrCellCb                  *cellCb;
   U16                       drxLongCycle; 

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for getting DRX long cycle value.");
      RETVALUE(RFAILED);
   }
   
   drxLongCycle = cellCb->ueAnrDrxCfg.longDrxCycle;
   RETVALUE(drxLongCycle);
}
/*
 *       Fun:     wrEmmGetAnrDrxInActivTmrVal
 *
 *       Desc:    This primitive returns the DRX Inactive timer Value
 *                value for ANR.
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC U8 wrEmmGetAnrDrxInActivTmrVal
(
WrCellId                     cellId
)
{
   WrCellCb                  *cellCb;

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid CellId received for getting"
                "ANR DRX inactive timer value");
      RETVALUE(RFAILED);
   }
   
   RETVALUE(cellCb->ueAnrDrxCfg.inActivTmr);
}


/*
 *       Fun:     wrChkForFrbRat
 *
 *       Desc:    This function will check for forbidden RAT.
 *                - Inputs: RAT type and HO Rstrct Info.
 *                - Output: RAT is forbidden or not.
 *                - Based on the RAT type check the 
 *                  HO Rstrct Info to see if the RAT
 *                  is forbidden or not.                         
 *
 *       Ret:     RAT is forbidden or not
 *
 *       Notes :  None.
 */
PUBLIC WrHrlDecisionRet wrChkForFrbRat
(
 WrForbRatTyp       forbRatTyp,
 WrHoRstrctInfo    *hoRstrctInfo
 )
{
   U32      forbRat;
   WrHrlDecisionRet     ret = WR_MEAS_MBLTY_APPL_TO_RAT;

   if (hoRstrctInfo->frbdnInterRATs.pres == NOTPRSNT)
   {
      ret = WR_MEAS_MBLTY_APPL_TO_RAT;
   }

   if(hoRstrctInfo->frbdnInterRATs.val == SztForbiddenInterRATsallEnum)
   {
      ret = WR_MEAS_MBLTY_NOT_APPL_TO_RAT;
   }

   forbRat = hoRstrctInfo->frbdnInterRATs.val;
   switch(forbRatTyp)
   {
      case WR_FORB_RAT_UTRA:
         {
            if((forbRat == SztForbiddenInterRATsutranEnum) 
                  || (forbRat == SztForbiddenInterRATsgeranandutranEnum)
                  || (forbRat == SztForbiddenInterRATscdma2000andutranEnum))
            {
               ret = WR_MEAS_MBLTY_NOT_APPL_TO_RAT;
            }
            else
            {
               ret = WR_MEAS_MBLTY_APPL_TO_RAT;
            }
         }
         break;
      case WR_FORB_RAT_CDMA:
         {
            if((forbRat == SztForbiddenInterRATscdma2000Enum)
                  || forbRat == SztForbiddenInterRATscdma2000andutranEnum)
            {
               ret = WR_MEAS_MBLTY_NOT_APPL_TO_RAT;
            }
            else
            {
               ret = WR_MEAS_MBLTY_APPL_TO_RAT;
            }
         }
         break;
      case WR_FORB_RAT_GERAN:
         {
            if((forbRat == SztForbiddenInterRATsgeranEnum)
                  || (forbRat == SztForbiddenInterRATsgeranandutranEnum))
            {
               ret = WR_MEAS_MBLTY_NOT_APPL_TO_RAT;
            }
            else
            {
               ret = WR_MEAS_MBLTY_APPL_TO_RAT;
            }
         }
         break;
      default:
         {
            RLOG1(L_WARNING, "Invalid RAT type received while checking forbidden RAT[%d].",forbRatTyp);
            ret = WR_MEAS_MBLTY_NOT_APPL_TO_RAT;
         }
         break;
   }
   RETVALUE (ret);
}


/*
 *       Fun:     wrChkForFrbPLMN
 *
 *       Desc:    This function will check for forbidden PLMN.
 *                - Inputs: HRL CB and HO Rstrct Info.
 *                - Output: Mobility applicable or not applicable 
 *                          towards the cells based on PLMN.
 *                - Compare the PLMN received in the HRL CB with 
 *                   the serving and equivalent PLMN of the HO Rstrct 
 *                   Info. Fill the matched PLMNs in the WrSelctdPLMN.
 *
 *       Ret:     PLMN is forbidden or not
 *
 *       Notes :  None.
 */
PRIVATE U8 wrChkForFrbPLMN
(
 WrHoRstrctInfo    *hoRstrctInfo,
 WrSelctdPLMN      *selctdPlmn,
 WrHrlCb           hrlCb
 )
{
   U8    idx = 0;
   U8    ePlmnIdx = 0;

   /*1. Check whether the plmn to be validated matches with the serving plmn */
   for(idx = 0; idx < WR_MAX_BRDCST_PLMN; idx++)
   {
      if(wrUtlPlmnsEqual(&hoRstrctInfo->srvngPlmn, &hrlCb.plmnToVldt[idx]))
      {
         selctdPlmn->selectdPLMN[0].plmnId = hoRstrctInfo->srvngPlmn;
         selctdPlmn->selectdPLMN[0].isPLMNForb = FALSE;
         selctdPlmn->nosOfPlmns++;
         RETVALUE(WR_MEAS_MBLTY_APPL_TO_CELL);
      }
   }

   /*2. Check whether the plmn to be validated matches with the equivalent plmn */
   for(ePlmnIdx = 0; ePlmnIdx < hoRstrctInfo->noOfEplmn; ePlmnIdx++)      
   {
      for(idx = 0; idx < hrlCb.nosOfPlmns; idx++)
      {
         if(wrUtlPlmnsEqual(&hoRstrctInfo->ePlmn[ePlmnIdx], &hrlCb.plmnToVldt[idx]))
         {
            selctdPlmn->selectdPLMN[selctdPlmn->nosOfPlmns].plmnId = \
                                                       hoRstrctInfo->ePlmn[ePlmnIdx];
            selctdPlmn->selectdPLMN[selctdPlmn->nosOfPlmns].isPLMNForb = FALSE;
            selctdPlmn->nosOfPlmns++;
         }
      }
   }

   if(selctdPlmn->nosOfPlmns != 0)
   {
      RETVALUE(WR_MEAS_MBLTY_APPL_TO_CELL);
   }
   RETVALUE(WR_MEAS_MBLTY_NOT_APPL_TO_CELL);
}

/*
 *       Fun:     wrChkForFrbTa
 *
 *       Desc:    This function will check for forbidden TA.
 *                - Inputs: HRL CB and HO Rstrct Info.
 *                - Output: Mobility applicable or not applicable 
 *                          towards the cells based on TA.
 *                - Compare the TA received in the HRL CB with 
 *                   the forbidden TA of the HO Rstrct Info. 
 *                   Mark the PLMNs entry in the WrSelctdPLMN as
 *                   TRUE or FALSE based on the forbidden TA of HRL.
 *
 *       Ret:     TA is forbidden or not
 *
 *       Notes :  None.
 */
PRIVATE U8 wrChkForFrbTa
(
 WrHoRstrctInfo    *hoRstrctInfo,
 WrSelctdPLMN      *selctdPlmn,
 WrHrlCb           hrlCb
 )
{
   U8    idx;
   U8    taIdx;
   U8    tacIdx;
   U8    nosOfTAsFrb = 0;

   /* Check for the forbidden TA only for each selected PLMN */
   for(idx = 0; idx < WR_MAX_BRDCST_PLMN; idx++)
   {
      for(taIdx = 0; taIdx < hoRstrctInfo->noOfTAs; taIdx++)
      {
         if(wrUtlPlmnsEqual(&selctdPlmn->selectdPLMN[idx].plmnId, \
                  &hoRstrctInfo->frbdnTAs[taIdx].plmnId))
         {
            /* Check if the Cell's TA is forbidden */
            for(tacIdx = 0; tacIdx < hoRstrctInfo->frbdnTAs[taIdx].noOfTac; tacIdx++)
            {
               if(hrlCb.tacOrLacToVldt == hoRstrctInfo->frbdnTAs[taIdx].tac[tacIdx])
               {
                  selctdPlmn->selectdPLMN[idx].isPLMNForb = TRUE;
                  nosOfTAsFrb++;
                  break;
               }
            } /* End of nos. of forbidden TAC loop  */
         }
      } /* End of forbidden TA loop*/
   } /* End of selected PLMN loop */

   /* If nosOfTAsFrb is equal to the nos. of selected PLMNs, then all selected PLMNs 
    * have forbidden TAC. */
   if(nosOfTAsFrb == selctdPlmn->nosOfPlmns)
   {
      /* All the PLMNs have forbidden TA */
      RETVALUE(WR_MEAS_MBLTY_NOT_APPL_TO_CELL);
   }
   else
   {
      /* Only some PLMNs have forbidden TA, 
       * so the PLMN which does not have forbidde TA can be used. */
      RETVALUE(WR_MEAS_MBLTY_APPL_TO_CELL);
   }
}

/*
 *       Fun:     wrChkForFrbLa
 *
 *       Desc:    This function will check for forbidden LA.
 *                - Inputs: HRL CB and HO Rstrct Info.
 *                - Output: Mobility applicable or not applicable 
 *                          towards the cells based on LA.
 *                - Compare the LA received in the HRL CB with 
 *                   the forbidden LA of the HO Rstrct Info. 
 *                   Mark the PLMNs entry in the WrSelctdPLMN as
 *                   TRUE or FALSE based on the forbidden LA of HRL.
 *
 *       Ret:     LA is forbidden or not
 *
 *       Notes :  None.
 */
PRIVATE U8 wrChkForFrbLa
(
 WrHoRstrctInfo    *hoRstrctInfo,
 WrSelctdPLMN      *selctdPlmn,
 WrHrlCb           hrlCb
 )
{
   U8    idx;
   U8    laIdx;
   U8    lacIdx;
   U8    nosOfLAsFrb = 0;

   /* Check for the forbidden LA only for each selected PLMN */
   for(idx = 0; idx < WR_MAX_BRDCST_PLMN; idx++)
   {
      for(laIdx = 0; laIdx < hoRstrctInfo->noOfLAs; laIdx++)
      {
         if(wrUtlPlmnsEqual(&selctdPlmn->selectdPLMN[idx].plmnId, \
                  &hoRstrctInfo->frbdnLAs[laIdx].plmnId))
         {
            /* Check if the Cell's LA is forbidden */
            for(lacIdx = 0; lacIdx < hoRstrctInfo->frbdnLAs[laIdx].noOfLac; lacIdx++)
            {
               if(hrlCb.tacOrLacToVldt == hoRstrctInfo->frbdnLAs[laIdx].lac[lacIdx])
               {
                  selctdPlmn->selectdPLMN[idx].isPLMNForb = TRUE;
                  nosOfLAsFrb++;
                  break;
               }
            } /* End of nos. of forbidden LAC loop */
         }
      } /* End of forbidden LA loop */
   } /* End of selected PLMN loop */

   /* If nosOfLAsFrb is equal to the nos. of selected PLMNs, then all selected PLMNs 
    * have forbidden LAC. */
   if(nosOfLAsFrb == selctdPlmn->nosOfPlmns)
   {
      /* All the PLMNs have forbidden LA */
      RETVALUE(WR_MEAS_MBLTY_NOT_APPL_TO_CELL);
   }
   else
   {
      /* Only some PLMNs have forbidden TA, 
       * so the PLMN which does not have forbidde TA can be used. */
      RETVALUE(WR_MEAS_MBLTY_APPL_TO_CELL);
   }
}

/*
 *       Fun:     wrChkForGuGrpId
 *
 *       Desc:    This function will check whether the GU Group Id of 
 *                a particular PLMN supported by the source and target
 *                eNodeB are same or not.
 *                - Inputs: Selected PLMNs and neigh eNB CB.
 *                - Output: Ho Type.
 *                - Check whethher X2 HO applicable or not. 
 *                  If X2 HO is not applicable then mark Ho Type
 *                  for S1 HO.                        
 *
 *       Ret:     None. Will set HoType as S1 HO if GU Group Id does not match
 *
 *       Notes :  None.
 */
PRIVATE Void wrChkForGuGrpId
(
 WrSelctdPLMN      *selctdPlmn,
 WrNeighEnbCb      *neighEnbCb,
 U8                *hoTyp
 )
{
   U8       idx = 0;
   U8       guIdx = 0;
   U8       guGrupIdArr[GU_GRUP_KEY_LEN] = {0};
   U8       guGrpIdExst = 0;

   for(idx = 0; idx < WR_MAX_BRDCST_PLMN; idx++)
   {
      if(selctdPlmn->selectdPLMN[idx].isPLMNForb == FALSE)
      {
         /* This PLMN can be used for the UE */
         for(guIdx = 0; guIdx < neighEnbCb->numGuGrpIds; guIdx++)
         {
            /* Check if the selected PLMN matches with the GUMMEI PLMN */
            if(wrUtlPlmnsEqual(&selctdPlmn->selectdPLMN[idx].plmnId, \
                     &neighEnbCb->guGrpIdLst[guIdx].plmnId))
            {
               /* The selected PLMN matches with the GUMMEI PLMN,
                * now check whether the GU Group Id of the target cell
                * and source cell is same or not. */
               WrGuGrupCbList *ptrGuGrupCbLst = NULLP; 
               WR_EMM_GET_GUGRUP_ARR(neighEnbCb->guGrpIdLst[guIdx], guGrupIdArr);
               if(ROK == cmHashListFind(&(wrEmmCb.guGrupIdLst),
                        (U8 *)guGrupIdArr, GU_GRUP_KEY_LEN, 0,
                        (PTR *)&ptrGuGrupCbLst))
               {
                  guGrpIdExst++;
                  break;
               }
            }
         } /* End of Gu Group Id loop */
      }      
   } /* End of selected PLMN loop */

   if(guGrpIdExst == 0)
   {
      /* There does not exist even a single PLMN supported by the Target 
       * cell which belongs to the same GU Group as the source. So proceed 
       * with S1 HO instead of X2 HO. */
      *hoTyp = WR_HO_TYPE_INTRA_LTE_S1;
   }
}

/*
 *       Fun:     wrMbltyRestrc
 *
 *       Desc:    This function will check for the mobility restriction for a UE.
 *                - Inputs: Mobility type (which includes RAT type and type of mobility
 *                          whether measurements or HO/CSFB)
 *                          neighCellCb (EUTRA / UTRA neighbor cell CB)
 *                          neighEnbCb (EUTRA neighbor eNB CB)
 *                          hoType (S1 / X2 HO)
 *                          HO Rstrct Info.
 *                - Output: Whether mobility is applicable towards the neighbor 
 *                          cell based on the PLMN, TA or LA.
 *                          Type of HO, S1 or X2 based on the neighbor eNB
 *                          GU Group Id.
 *                          The finalized selected PLMN.
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC WrHrlDecisionRet wrMbltyRestrc
(
WrMobilityType    mbltyTyp,
PTR               *neighCellCb,
PTR               *neighEnbCb,
U8                *hoTyp,
WrPlmnId          *fnlzdPLMN,
WrHoRstrctInfo    *hoRstrctInfo
 )
{
   WrSelctdPLMN      selctdPlmn;
   WrHrlCb           hrlCb;
   WrForbRatTyp      forbRatTyp;
   U8                idx;
   WrHrlDecisionRet  ret;

   WR_SET_ZERO(&hrlCb, (U16)sizeof(WrHrlCb));
   WR_SET_ZERO(&selctdPlmn, (U16)sizeof(WrSelctdPLMN));
   switch(mbltyTyp) 
   {
      case WR_INTRA_SYS_MEAS:
      case WR_INTRA_SYS_MBLTY:
         {
            WrEutranNeighCellCb  *eutraNeighCell;
            forbRatTyp = WR_RAT_EUTRA;

            eutraNeighCell = (WrEutranNeighCellCb*) neighCellCb;
            if(eutraNeighCell->numPlmn <= 1)
            {
               hrlCb.nosOfPlmns = 1;
               hrlCb.plmnToVldt[0] = eutraNeighCell->enbPlmn; 
            }
            else
            {
               hrlCb.nosOfPlmns = eutraNeighCell->numPlmn;
               for (idx = 0; idx < hrlCb.nosOfPlmns; idx++)
               {
                  hrlCb.plmnToVldt[idx] = eutraNeighCell->plmn[idx]; 
               }
            }
            hrlCb.tacOrLacToVldt = eutraNeighCell->tac;
         }
         break;
      case WR_INTER_SYS_UTRA_MBLTY:
         {
            WrUtraNeighCellCb    *utraNeighCell;
            forbRatTyp = WR_FORB_RAT_UTRA;

            utraNeighCell = (WrUtraNeighCellCb*) neighCellCb;
            hrlCb.nosOfPlmns = utraNeighCell->numPlmn;
            for (idx = 0; idx < hrlCb.nosOfPlmns; idx++)
            {
               hrlCb.plmnToVldt[idx] = utraNeighCell->plmnId[idx]; 
            }
            hrlCb.tacOrLacToVldt = utraNeighCell->lac;
         }
         break;
      case WR_INTER_SYS_GERAN_MBLTY:
         {
            WrGeranCellCb    *geranCell;
            forbRatTyp = WR_FORB_RAT_GERAN;

            geranCell = (WrGeranCellCb*) neighCellCb;
            hrlCb.nosOfPlmns = 1;
            hrlCb.plmnToVldt[0] = geranCell->plmnId; 
            hrlCb.tacOrLacToVldt = geranCell->lac;
         }
         break;
      default:
         {
            RLOG1(L_INFO,"Invalid mobilty type received [%d] to check"
                  "mobility restriction for a UE",mbltyTyp);
            RETVALUE (WR_MEAS_MBLTY_NOT_APPL_TO_CELL);
         }
   }

   /* 1. Check for the forbidden RAT */
   if(forbRatTyp != WR_RAT_EUTRA)
   {
      ret = wrChkForFrbRat(forbRatTyp, hoRstrctInfo);
      if(ret == WR_MEAS_MBLTY_NOT_APPL_TO_RAT)
      {
         RETVALUE (WR_MEAS_MBLTY_NOT_APPL_TO_RAT);
      }
   }

   /*2. Check for PLMN */
   ret = wrChkForFrbPLMN(hoRstrctInfo, &selctdPlmn, hrlCb);
   if(ret == WR_MEAS_MBLTY_NOT_APPL_TO_CELL)
   {
      RETVALUE(WR_MEAS_MBLTY_NOT_APPL_TO_CELL);
   }

   /*3. Check for TA */
   if((mbltyTyp == WR_INTRA_SYS_MBLTY) || (mbltyTyp == WR_INTRA_SYS_MEAS))
   {
      ret = wrChkForFrbTa(hoRstrctInfo, &selctdPlmn, hrlCb);
      if(ret == WR_MEAS_MBLTY_NOT_APPL_TO_CELL)
      {
         RETVALUE(WR_MEAS_MBLTY_NOT_APPL_TO_CELL);
      }
   }

   /*4. Check for LA */
   if((mbltyTyp == WR_INTER_SYS_UTRA_MBLTY) || (mbltyTyp == WR_INTER_SYS_GERAN_MBLTY))
   {
      ret = wrChkForFrbLa(hoRstrctInfo, &selctdPlmn, hrlCb);
      if(ret == WR_MEAS_MBLTY_NOT_APPL_TO_CELL)
      {
         RETVALUE(WR_MEAS_MBLTY_NOT_APPL_TO_CELL);
      }
   }

   /*5. Check for GU Group Id */   
   /* GU Group Id validation is only in case of EUTRA mobility
    * for X2 handover. */
   if((mbltyTyp == WR_INTRA_SYS_MBLTY) && (neighEnbCb != NULLP) 
         && (*hoTyp == WR_HO_TYPE_X2))
   {
      WrNeighEnbCb      *eutraNeighEnbCb = NULLP;
      eutraNeighEnbCb = (WrNeighEnbCb *)neighEnbCb;
      wrChkForGuGrpId(&selctdPlmn, eutraNeighEnbCb, hoTyp);
   }

   /*6. Fill the first non - forbidden selected PLMN
    * as the finalized PLMN */
   if(fnlzdPLMN != NULLP)
   {
      for(idx = 0; idx < WR_MAX_BRDCST_PLMN; idx++)
      {
         if(selctdPlmn.selectdPLMN[idx].isPLMNForb == FALSE)
         {
            wrUtlCopyPlmnId(&selctdPlmn.selectdPLMN[idx].plmnId, fnlzdPLMN);
            break;
         }
      }
   }

   RETVALUE(WR_MEAS_MBLTY_APPL_TO_CELL);
}


/*
 *       Fun:     wrValidateRegLaiCell
 *
 *       Desc:    This function will check for the mobility restriction for a UE.
 *                - Inputs: Mobility type (which includes RAT type and type of mobility
 *                          whether measurements or HO/CSFB)
 *                          neighCellCb (EUTRA / UTRA neighbor cell CB)
 *                          neighEnbCb (EUTRA neighbor eNB CB)
 *                          hoType (S1 / X2 HO)
 *                          HO Rstrct Info.
 *                - Output: Whether mobility is applicable towards the neighbor 
 *                          cell based on the PLMN, TA or LA.
 *                          Type of HO, S1 or X2 based on the neighbor eNB
 *                          GU Group Id.
 *                          The finalized selected PLMN.
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC Bool wrFindRegLaiPlmnCell
(
WrRegLai                 *regLai,
WrUtraNeighCellCb        *neighCellCb,
WrHoRstrctInfo           *hoRstrctInfo
)
{
   U8                idx;
	U32             	cnt;
   U32             	cnt1;

   for (idx = 0; idx < neighCellCb->numPlmn; idx++)
   {
      if (TRUE == wrEmmPlmnPlmnsEqual(&neighCellCb->plmnId[idx], &regLai->plmnId))
      {
         RLOG0(L_INFO, "Neighbour cell plmn found matching with RegLAI plmn while finding RegLaiPlmn cell");
         break;
      }
   }

   if (idx == neighCellCb->numPlmn)
   {
      RLOG0(L_WARNING, "Unable to find RegLaiPlmn cell");
      RETVALUE(FALSE);
   }
	
	/*Check if Neighbor cell LAI is present in Forbidden LAs list*/
   for(cnt1 = 0; (hoRstrctInfo != NULLP && cnt1 < hoRstrctInfo->noOfLAs); cnt1++)
   {
      if(TRUE == wrEmmPlmnPlmnsEqual(&regLai->plmnId,&hoRstrctInfo->frbdnLAs[cnt1].plmnId))
      {
         /*Ignore this Forbidden LA*/
         for(cnt = 0; cnt < hoRstrctInfo->frbdnLAs[cnt1].noOfLac; cnt++)
         {
            if(neighCellCb->lac == hoRstrctInfo->frbdnLAs[cnt1].lac[cnt])
            {
               RLOG0(L_INFO, "Neighbor cell LAI is present in Forbidden LAs");
               RETVALUE(FALSE);
            }
         }
      }
   }
   RETVALUE(TRUE);
}


/** 
 *    Fun: wrEmmGetUtraFddFreqLst
 * 
 *    Desc: This function will access the UTRA FDD
 *          Frequency List and return it.
 *   
 *    inputs: EMM Cell ID
 *
 *    ret: Returns UTRA FDD Freq
 * */
PUBLIC CmLListCp* wrEmmGetUtraFddFreqLst
(
WrCellId                  cellId
)
{
   WrCellCb                  *cellCb = NULLP;

   WR_GET_CELLCB(cellCb, cellId);
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received for getting Utra FDD frequency list");
      RETVALUE (NULLP);
   }

   RETVALUE(&cellCb->utraFddNbrFreqLst);
}

/** 
 *    Fun: wrEmmGetUtraTddFreqLst
 * 
 *    Desc: This function will access the UTRA TDD
 *          Frequency List and return it.
 *   
 *    inputs: EMM Cell ID
 *
 *    ret: Returns UTRA TDD Freq
 * */
PUBLIC CmLListCp* wrEmmGetUtraTddFreqLst
(
WrCellId                  cellId
)
{
   WrCellCb                  *cellCb = NULLP;

   WR_GET_CELLCB(cellCb, cellId);
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Invalid cellId received for getting Utra TDD frequency list");
      RETVALUE (NULLP);
   }

   RETVALUE(&cellCb->utraTddNbrFreqLst);
}
/* 
 *       Fun:     wrEmmGetPlmnRedirSupport
 *
 *       Desc:    This function will return the Redirection support for PLMN
 *       Processing steps:
 *         - Get the redirection support for PLMN ID. 
 *
 *       Ret:     ROK   - OK / RFAILED - Failure
 *
 *       Notes :  None.
 */
PUBLIC S16 wrEmmGetPlmnRedirSupport
(
WrPlmnId                     *plmn,
Bool                         *isRedirSupport,
WrCellId                     cellId
)
{
   WrPlmnId                  plmnId ={0};
   WrCellCb                  *cellCb;
   U8                        numOfPlmn =0;
   U8                        idx =0;
   S16                       ret = RFAILED;

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG1(L_ERROR, "Invalid CellId received for getting redirection support for PLMN [%d].",cellId);
      RETVALUE(RFAILED);
   }

   /* Get the num Of Plmn from Sib1 */
   numOfPlmn = wrEmmCb.plmns.numPlmns;

   for(idx = 0; idx < numOfPlmn ; idx ++)
   {
      plmnId = wrEmmCb.plmns.plmns[idx].plmnInfo.plmnId;
      if (wrEmmPlmnPlmnsEqual(&plmnId, plmn) == TRUE)
      {
         /* get csfb prior RatTyp for selected plmn */
         *isRedirSupport =  wrEmmCb.plmns.plmns[idx].plmnInfo.isRedirSupport;
         ret = ROK;
         break;
      }
   }
   RETVALUE(ret);
}

/** 
 *    Fun: wrEmmPrintCellState
 * 
 *    Desc: This function will print the cell states in string format
 *          
 *    inputs: Cell State
 *
 *    ret: Returns Cell State in Descriptive String Format
 * */
PUBLIC CONSTANT S8* wrEmmPrintCellState
(
 U8 cellState
)
{
   CONSTANT S8* stateStr;

   switch(cellState)
   {
      case WR_CELL_STATE_INIT:
      {
         stateStr = PRINT_STR(WR_CELL_STATE_INIT);
         break;
      }

      case WR_CELL_STATE_RESET:
      {
         stateStr = PRINT_STR(WR_CELL_STATE_RESET);
         break;
      }

      case WR_CELL_STATE_UP:
      {
         stateStr = PRINT_STR(WR_CELL_STATE_UP);
         break;
      }

      case WR_CELL_STATE_PWR_DOWN:
      {
         stateStr = PRINT_STR(WR_CELL_STATE_PWR_DOWN);
         break;
      }

      case WR_CELL_STATE_RECFG:
      {
         stateStr = PRINT_STR(WR_CELL_STATE_RECFG);
         break;
      }
      default:
      {
         stateStr = PRINT_STR(INVALID STATE);
         break;
      }
  }
  return stateStr;
}
/**
 *  @brief This function is used to intiate RIR with Stop .
 *
 *  @details
 *      Function  : wrEmmStopNRTAgeTimer
 *          Processing Steps:
 *          - Stop the NRT age timer
 *
 *
 *  @param[in]
 *  @return S16 
 *        -# Success : ROK 
 *        -# Failure : RFAILED
**/ 
PUBLIC S16 wrEmmStopNRTAgeTimer
(
)
{
   RLOG0(L_INFO,"Stopping NRT Age Timer");
   wrStopTmr((PTR)wrEmmCb.cellCb[0],WR_TMR_ANR_CELL_MAX_AGE_TMR);
   RETVALUE(ROK);
}


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
