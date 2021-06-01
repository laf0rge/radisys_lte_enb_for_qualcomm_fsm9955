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
               

     File:     wr_umm.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=46;


static int RLOG_MODULE_ID=32;

#include "wr.h"            /* Common headers and Data Structures */
#include "wr_umm.h"        /* UMM Specific files */
#include "wr_umm_trans.h"
#include "wr_ifm_dam.h"
#include "wr_ifm_rrm.h"
#include "wr_umm_ho.h"
#include "wr_smm_smallcell.h"
#include "wr_umm_msg.h"
#include "wr_emm.h"
#include "wr_kpi.h"
#include "wr_kpiIds.h"
#include "wr_umm_meas_fun.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_son.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_ecsfb_utils.h"
#include "wr_umm_initcntsetup.h"
/* To avoid CPUH crashed at wrUmmMeasGapReqdInterfreq  while attaching Single UE*/
#define WR_UMM_GET_MME(_mmeId) &(wrUeDb.mmes[_mmeId])
PRIVATE U16 dlArfcnEUTRA[43]={599,1199,1949,2399,2649,2749,3449,3799,4149,4749,4949,5179,5279,5379,
                0,0,5849,5999,6149,6449,6599,7399,7699,8039,8689,9039,9209,9659,9769,9869,9919,0,
                36199,36349,36949,37549,37749,38249,38649,39649,41589,43589,45589};

PRIVATE U16 dlArfcnUTRA[26][2]={
                                 {10562,10838},
                                 {9662,9938},
                                 {1162,1513},
                                 {1537,1738},
                                 {4357,4458},
                                 {4387,4413},
                                 {2237,2563},
                                 {2937,3088},
                                 {9237,9387},
                                 {3112,3388},
                                 {3712,3787},
                                 {3842,3903},
                                 {4017,4043},
                                 {4117,4143},
                                 {0,0},
                                 {0,0},
                                 {0,0},
                                 {0,0},
                                 {712,763},
                                 {4512,4638},
                                 {862,912},
                                 {4662,5038},
                                 {0,0},
                                 {0,0},
                                 {5112,5413},
                                 {5762,5913}
                                };
/* Store the allocated ue idx in this array associated with the SRS periodicity.
 * Index of the UE location identifies the SRS Config index associated with
 * the UE. For example, SRS Periodicity with 80 milli second configuration, 
 * isSrsCfgIdxUsed[12] = 71; here 12 is the offset for UEID 71 to be
 * applied to determine the SRS Configuration Index. SRS Config Index is calculated 
 * as SRSCFG_INDX_START_80MS + 12 => 77+12=89.*/
PRIVATE U8 isSrsCfgIdxUsed[WR_MAX_UES_PER_CELL] = {0};

PUBLIC WrUeDb  wrUeDb;
EXTERN U32 numTtiTicks;
PRIVATE WrUmmTransProcessor wrUmmTransPrcs[WR_UMM_MAX_TRANS_TYPE];
PRIVATE WrUmmTransMsgPrcr wrUmmTransMsgPrcs[WR_UMM_MAX_TRANS_TYPE];
PRIVATE WrUmmStopTransPrcr wrUmmStopTransPrcs[WR_UMM_MAX_TRANS_TYPE]; 
PRIVATE WrUmmTransAllowMsg  wrUmmTransAllowMsg[WR_UMM_MAX_TRANS_TYPE];
PRIVATE WrUmmTransRelease   wrUmmTransRelease[WR_UMM_MAX_TRANS_TYPE];
PRIVATE S16 wrUmmInitalCtxRspTimerExp (WrUeCb *ueCb);
PRIVATE S16 wrUmmUeCtxRelTimerExp (WrUeCb *ueCb);
EXTERN S16 wrUmmRrcReEstabTransProcessor (WrUmmTransCb  *transCb);
EXTERN S16 wrUmmCtxtModProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmMeasProcessor(WrUmmTransCb *transCb);
EXTERN S16 wrUmmMeasTransRel(WrUmmTransCb *transCb);
EXTERN S16 wrEmmErabParseStat(WrEmmErabStat updType, 
         WrEmmErabSubType subType, WrUmmRabSetupTrans * rabSetup, WrCellId cellId );
EXTERN Void wrUmmGetSrsCfgIdx (WrUeCb *ueCb, U8 ueIdx);
EXTERN Void wrUmmDelSrsCfgIdx (WrUeCb *ueCb, U8 idx);
EXTERN S16 wrUmmTgtHoInitUeCb(WrUeCb *ueCb);
PRIVATE Void wrUmmTmrExpBldInternalMsg (U8 wrUmmTmrExpEvnt, WrUmmIncMsg *msg);

PRIVATE U32 wrUmmNumTransCnt = 0;
PUBLIC U32 wrUmmNumUeCnt = 0;
EXTERN S16 wrUmmPrcPdcpDatResumeCfm(NhuPdcpDatResumeCfm *pdcpDatResumeCfm);

EXTERN  WrNrUtranFreqCb* wrEmmAnrGetFreqNode
(
U16                           arfcnDl,
WrNrUtranCb                   *utranCb
);
EXTERN CmLListCp* wrEmmGetUtraTddFreqLst
(
WrCellId                  cellId
);

PRIVATE Void wrEmmUpdateEstabInitAttNbr
(
  WrCellCb *cellCb,
   WrUmmRabSetupTrans * rabSetup
);

PRIVATE Void wrEmmUpdateEstabInitSuccNbr
(
  WrCellCb *cellCb,
   WrUmmRabSetupTrans * rabSetup
);

PRIVATE Void wrEmmUpdateEstabAttNbr
(
  WrCellCb *cellCb,
   WrUmmRabSetupTrans * rabSetup
);

PRIVATE Void wrEmmUpdateEstabSuccNbr
(
  WrCellCb *cellCb,
   WrUmmRabSetupTrans * rabSetup
);

EXTERN S16 wrUmmMeasCellReCfgParams
(
 LwrCellMeasCfgGrp            *cellCfgParams, 
 LwrCellMeasCfgGrp            *measReCfg
);

PUBLIC Bool wrUmmChkCdmaFgi
(
 U16                         cellId,
 U16                         crnti
);


EXTERN S16 wrUmmCfgRabParams
(
 U16                   cellId,
 LwrRabPolicyCfgGrp    *rabPolicyCfgGrp
);
EXTERN S16 wrUmmInitRabParams(WrCellCb  *cellCb); 
EXTERN S16 wrUmmAddKPILaiList(WrCellId celId, WrPlmnId plmnId, U16 lac); 

PRIVATE  WrUmmTransProcessor  wrUmmTransPrcs[WR_UMM_MAX_TRANS_TYPE] = 
{
   wrUmmRrcSetupProcessor,
   wrUmmInitCtxtProcessor,
   wrUmmRabSetupProcessor,
   wrUmmUlNasProcessor,
   wrUmmDlNasProcessor,
   wrUmmLcRabRelProcessor,
   wrUmmRrcReEstabTransProcessor,
   wrUmmCtxtRelProcessor,
   wrUmmRabRelProcessor,
   wrUmmRabModifyProcessor,
   wrUmmCtxtModProcessor,
   wrUmmMeasProcessor,
   wrUmmS1SrcHoProcessor,
   wrUmmS1TgtHoProcessor,
   wrUmmX2SrcHoProcessor,
   wrUmmX2TgtHoProcessor,
   wrUmmCsfbProcessor,
   wrUmmEcsfbProcessor,
   wrUmmDlCdmaProcessor,
   wrUmmRadResConfModProcessor,
   wrUmmLocReptProcessor,
   wrUmmAnrMeasProcessor,
   wrUmmEventAnrProcessor,
   wrUmmRrcCsfbProcessor,
   wrUmmLteAdvProcessor,
   wrUmmCsfbGeranCcoProcessor,
   wrUmmTxModeChngProcessor,
   wrUmmPerdStrongCellMeasProcessor,
   wrUmmDefIncTrans
};

PRIVATE WrUmmTransRelease   wrUmmTransRelease[WR_UMM_MAX_TRANS_TYPE] = 
{
   wrUmmRrcConTransRel,
   wrUmmInitCtxtTransRel,
   wrUmmRabSetupTransRel,
   wrUmmUlNasTransRel,
   wrUmmDlNasTransRel,
   wrUmmLcRabRelTransRel,
   wrUmmRrcReestabTransRel,
   wrUmmCtxtRelTransRel,
   wrUmmRabRelTransRel,
   wrUmmRabModifyTransRel,
   wrUmmCtxtModTransRel,
   wrUmmMeasTransRel,
   wrUmmS1SrcHoTransRel,
   wrUmmS1TgtHoTransRel,
   wrUmmX2SrcHoTransRel,
   wrUmmX2TgtHoTransRel,
   wrUmmCsfbTransRel,
   wrUmmEcsfbTransRel,
   wrUmmDlCdmaTransRel,
   wrUmmRadResConfModTransRel,
   wrUmmLocReptTransRel,
   wrUmmAnrMeasTransRel,
   wrUmmEventAnrTransRel,
   wrUmmRrcCsfbTransRel,
   wrUmmLteAdvTransRel,
   wrUmmCsfbGeranCcoTransRel,
   wrUmmTxModeChngTransRel,
   wrUmmPerdStrongCellMeasTransRel,
   wrUmmDefIncTransRel
};
PRIVATE WrUmmNewTransPrcr wrUmmNewTransPrcrs[WR_UMM_MAX_TRANS_TYPE] = 
{
   wrUmmRrcConnNewTransProcessor, 
   wrUmmInitCtxtNewTransProcessor,
   wrUmmRabSetupNewTransProcessor,
   wrUmmIncNewTransProcessor,
   wrUmmDlNasNewTransProcessor,
   wrUmmIncNewTransProcessor,
   wrUmmReestabNewTransProcessor,
   wrUmmCtxtRelNewTransProcessor,
   wrUmmRabRelNewTransProcessor,
   wrUmmRabModNewTransProcessor,
   wrUmmIncNewTransProcessor,
   wrUmmMeasNewTransProcessor,
/* HO_REEST */
   wrUmmS1SrcHoNewTransProcessor,
   wrUmmS1TgtHoNewTransProcessor,
   wrUmmX2SrcHoNewTransProcessor,
   wrUmmX2TgtHoNewTransProcessor,
   wrUmmCsfbNewTransProcessor,
   wrUmmEcsfbNewTransProcessor,
   wrUmmDlCdmaNewTransProcessor,
   wrUmmRadResModNewTransProcessor,
   wrUmmIncNewTransProcessor,
   wrUmmAnrMeasNewTransProcessor,
   wrUmmEventAnrNewTransProcessor,
   wrUmmRrcCsfbNewTransProcessor,
   wrUmmIncNewTransProcessor,
   wrUmmCsfbGeranCcoNewTransProcessor,
   wrUmmIncTxModeChngNewTransProcessor,
   wrUmmPerdStrongCellMeasNewTransProcessor,
   wrUmmIncNewTransProcessor
};
PRIVATE  WrUmmTransMsgPrcr  wrUmmTransMsgPrcs[WR_UMM_MAX_TRANS_TYPE] = 
{
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmCsfbPrcDeferTrans,
   wrUmmEcsfbPrcDeferTrans,
   wrUmmDlCdmaPrcDeferTrans,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmRrcCsfbPrcDeferTrans,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg,
   wrUmmDefIncTransMsg
};

PRIVATE WrUmmStopTransPrcr wrUmmStopTransPrcs[WR_UMM_MAX_TRANS_TYPE] = 
{
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmStopReEstabTransPrcs,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans,
   wrUmmDefIncStopTrans
};

/** @brief This function is the default processing of incoming transaction
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Return OK so that transaction processing continues
 *
 *
 * @param WrUmmTransCb         *trans
 * @param WrUmmTransCb         *incTrans
 * @return WR_UMM_TRANS_OK
 */ 
PUBLIC S16 wrUmmDefIncTrans
(
WrUmmTransCb                 *transCb
)
{
   RETVALUE(WR_UMM_TRANS_OK);
}


/** @brief This function is the default processing of incoming New transaction
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Return OK so that transaction processing continues
 *
 *
 * @param WrUmmTransCb         *trans
 * @param WrUmmTransCb         *incTrans
 * @return WR_UMM_TRANS_OK
 */ 
PUBLIC S16 wrUmmIncNewTransProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *defTransCb
)
{
   RETVALUE(WR_UMM_TRANS_OK);
}



/** @brief This function is the default processing of Trans Msg transaction
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Return OK so that transaction processing continues
 *
 *
 * @param WrUmmTransCb         *trans
 * @param WrUmmTransCb         *incTrans
 * @return WR_UMM_TRANS_OK
 */ 
PUBLIC S16 wrUmmDefIncTransMsg
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *defTransCb
)
{
   RETVALUE(WR_UMM_TRANS_OK);
}



/** @brief This function is the default processing of incoming 
           transaction release message
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Return OK so that transaction processing continues
 *
 *
 * @param WrUmmTransCb         *trans
 * @param WrUmmTransCb         *incTrans
 * @return WR_UMM_TRANS_OK
 */ 
PUBLIC S16 wrUmmDefIncTransRel
(
WrUmmTransCb                 *transCb
)
{
   RETVALUE(WR_UMM_TRANS_OK);
}

/** @brief This function is the default processing of incoming 
           stop transaction msg
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Return OK so that transaction processing continues
 *
 *
 * @param WrUmmTransCb         *trans
 * @param WrUmmTransCb         *incTrans
 * @return WR_UMM_TRANS_OK
 */ 
PUBLIC S16 wrUmmDefIncStopTrans
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *defTransCb
)
{
   RETVALUE(WR_UMM_TRANS_OK);
}

/** @brief This function adds the UE in the MME Control block
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the MME control Block using mmeId
 *         - Add the UE control block in the linked list
 *
 *
 * @param [in] ueCb
 * @param [in] mmeId
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmAddUeToMme
(
WrUeCb                       *ueCb,
WrMmeId                      mmeId
)
{
   WrUmmMmeCb                *mmeCb;

   mmeCb = WR_UMM_GET_MME(mmeId);
   cmLListAdd2Tail(&mmeCb->ueLst, &ueCb->mmeLnk);
   ueCb->mmeLnk.node = (PTR)ueCb;
   ueCb->mmeId = mmeId;
   RETVALUE(ROK);
}

/** @brief This function returns the number of UEs which are pending to
 * for release
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the Cell control Block using cellId
 *         - Return the number of UEs to be released
 *
 *
 * @param [in] cellId 
 * @return S16
    -# Number of UEs to be released
 */ 
PUBLIC S16 wrUmmGetUeCntxToRel  
(
WrCellId                    cellId
)
{
   WrUmmCellCb               *cellCb;

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
       /*127802*/
       RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL ");
       RETVALUE(RFAILED);
   }
   if ( 0 == cellCb->noUeCntxToRel )
   {
     /* Stop the timer is if cellCb->noUeCntxToRel is zero */
      wrStopTmr((PTR)cellCb, WR_TMR_GUARD_UE_CTX_REL);
   }
   RETVALUE(cellCb->noUeCntxToRel);
}

/** @brief This function returns the RRC DAT CFM timer value
 *
 * @details
 *
 *     Function:  wrUmmGetRrcDatCfmTmrVal 
 *
 *         Processing steps:
 *         - Get the Cell control Block using cellId
 *         - Return the RRC DAT CFM timer value
 *
 * @param [in] cellId 
 * @return U32
    -# RRC DAT CFM Timer value
 */ 
PUBLIC U32 wrUmmGetRrcDatCfmTmrVal
(
 U32     cellId
)
{
   WrUmmCellCb      *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb,cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL ");
      RETVALUE(RFAILED);
   }
   RETVALUE(cellCb->csfbToGeranCfg.rrcDatCfmWaitTmr);
}

/** @brief This function Updates the Cell State in the Cell Control block
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the Cell control Block using cellId
 *         - Update the Cell State 
 *
 *
 * @param [in] cellId 
 * @param [in] state
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmUpdateCellState  
(
WrCellId                    cellId,
WrCellState                 state
)
{
   WrUmmCellCb               *cellCb;

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(NULL == cellCb)
   {
      /*127802*/
       RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL ");
       RETVALUE(RFAILED);
   }
   cellCb->cellState = state;
   RETVALUE(ROK);
}


/** @brief This function deletes the UE in the MME Control block
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the MME control Block using mmeId
 *         - Delete the UE control block from the linked list
 *
 *
 * @param [in] ueCb
 * @param [in] mmeId
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PRIVATE S16 wrUmmDelUeFrmMme
(
WrUeCb                       *ueCb,
WrMmeId                      mmeId
)
{
   WrUmmMmeCb                *mmeCb;

   mmeCb = WR_UMM_GET_MME(mmeId);
   cmLListDelFrm(&mmeCb->ueLst, &ueCb->mmeLnk);
   /* Fix for ccpu00136912 */
   ueCb->mmeLnk.node = NULLP;
   RETVALUE(ROK);
}

/** @brief This function returns the pointer to UE control block based on cell
 *         identity and old CRNTI duirng re-establishment. 
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the Cell control Block using cellId
 *         - In the list of UE Cbs in the cell control block get the UE
 *           with the corresponding CRNTI
 *
 *
 * @param [in] ueCb
 * @param [in] cellId 
 * @param [in] oldCrnti
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmGetOldUe
(
WrUeCb                       **ueCb,
WrCellId                     cellId,
U16                          oldCrnti
)
{
   WrUmmCellCb               *cellCb;
   U32                       idx;

   *ueCb = NULLP;
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < cellCb->maxUesPerCell; idx++)
   {
      if (cellCb->ueCbs[idx] != NULLP)
      {
         if (cellCb->ueCbs[idx]->oldCrnti == oldCrnti)
         {
            *ueCb = cellCb->ueCbs[idx];
            RETVALUE(ROK);
         }
      }
   }
   RETVALUE(RFAILED);
}


/** @brief This function returns the pointer to UE control block based on cell
 *         identity and CRNTI. 
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the Cell control Block using cellId
 *         - In the list of UE Cbs in the cell control block get the UE
 *           with the corresponding CRNTI
 *
 *
 * @param [in] ueCb
 * @param [in] cellId 
 * @param [in] crnti
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmGetUe
(
WrUeCb                       **ueCb,
WrCellId                     cellId,
U16                          crnti
)
{
   WrUmmCellCb               *cellCb;
   U32                       idx;

   *ueCb = NULLP;
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < cellCb->maxUesPerCell; idx++)
   {
      if (cellCb->ueCbs[idx] != NULLP)
      {
         if (cellCb->ueCbs[idx]->crnti == crnti)
         {
            *ueCb = cellCb->ueCbs[idx];
            RETVALUE(ROK);
         }
      }
   }
   RETVALUE(RFAILED);
}
/** @brief This function returns the pointer to UE control block based on cell
 *         identity and CRNTI/Previous CRNTI in case of RRC Reestablishment only. 
 * @details
 *
 *     Function: wrUmmGetReestUe 
 *
 *         Processing steps:
 *         - Get the Cell control Block using cellId
 *         - In the list of UE Cbs in the cell control block get the UE
 *           with the corresponding CRNTI/Previous CRNTI used
 *           in case of RRC Reestablishment only
 *
 *
 * @param [in] ueCb
 * @param [in] cellId 
 * @param [in] crnti
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmGetReestUe
(
WrUeCb                       **ueCb,
WrCellId                     cellId,
U16                          crnti
)
{
   WrUmmCellCb               *cellCb;
   U32                       idx;

   *ueCb = NULLP;
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < cellCb->maxUesPerCell; idx++)
   {
      if (cellCb->ueCbs[idx] != NULLP)
      {
         if ((cellCb->ueCbs[idx]->crnti == crnti) || (cellCb->ueCbs[idx]->oldCrnti == crnti))
         {
            *ueCb = cellCb->ueCbs[idx];
            RETVALUE(ROK);
         }
      }
   }
   RETVALUE(RFAILED);
}



/** @brief This function returns the pointer to UE control block based on cell
 *         identity and CRNTI. 
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the Cell control Block using cellId
 *         - In the list of UE Cbs in the cell control block get the UE
 *           with the corresponding CRNTI
 *
 *
 * @param [in] ueCb
 * @param [in] cellId 
 * @param [in] crnti
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmGetUeCbforRabRel
(
WrUeCb                       **ueCb,
WrCellId                     cellId
)
{
   WrUmmCellCb               *cellCb;
   U32                       idx;

   *ueCb = NULLP;
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < cellCb->maxUesPerCell; idx++)
   {
      if (cellCb->ueCbs[idx] != NULLP)
      { 
         if ((cellCb->ueCbs[idx]->crnti != 0) && (cellCb->ueCbs[idx]->rbInfo.numOfRbCfgd >= 2) )
         {
            *ueCb = cellCb->ueCbs[idx];
            RETVALUE(ROK);
         }
      }
   }
   RETVALUE(RFAILED);
}


/** @brief This function returs the pointer to UE control block based on
 *         cell id and UE Index.
 *
 * @details
 *
 *     Function: wrUmmGetUeFrmUeIdx
 *
 *         Processing steps:
 *         - Get the Cell control Block using cellId
 *         - If the location of ue index in the Array of UeCbs id not NULL,
 *           - validate that the ue index is for this ue
 *         - Return the address of the UE Control Block
 *
 * @param WrUeCb     ueCb 
 * @param WrCellId     cellId 
 * @param U8         idx
 * @return ROK 
 */ 
PUBLIC S16 wrUmmGetUeFrmUeIdx
(
WrUeCb                       **ueCb,
WrCellId                     cellId,
U8                           idx
)
{
   WrUmmCellCb               *cellCb;

   *ueCb = NULLP;
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   if ((idx < cellCb->maxUesPerCell) && (cellCb->ueCbs[idx]) != NULLP)
   {
      if (cellCb->ueCbs[idx]->ueIdx == idx)
      {
         *ueCb = cellCb->ueCbs[idx];
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
}

/** @brief This function allocates SRS Configuration Index based 
 *         on SRS Periodicity.
 *
 * @details
 *
 *     Function: wrUmmGetSrsCfgIdx
 *
 * @param WrUeCb     ueCb 
 * @param U8         idx
 * @return none 
 */ 
PUBLIC Void wrUmmGetSrsCfgIdx 
(
WrUeCb                       *ueCb,
U8                           ueIdx
)
{
   U8 idx;

   for(idx=0; idx<WR_MAX_UES_PER_CELL; idx++)
   {
      if(isSrsCfgIdxUsed[idx] == FALSE)
      {
          break;
      }  
   }

   if(idx == WR_MAX_UES_PER_CELL)
   {
      /* handle error condition */
      WR_DBGP(DBGMASK_SM, (WR_PRNT_BUF,"UL SRS Cfg Idx UE failed: %d \n",ueIdx));
      RETVOID;
   }
#if 0
#else
   U16 div,srsCfgIdxInit,ueIdx1;

   /*scheduling SRS only in Special subframes 1 and 6 */
   /* Currently provided flexibility to configure only, 20, 40, 80 and 160 */
   /* 20ms */
   if(wrEmmCb.ulSrsPeriod == SRS_PERIODICITY_20MS)
   {
      div=4; /* there would be 4 SS, assuming 2 SS per RF */
      srsCfgIdxInit = 26;
   }
   
   /* 40ms */
   else if( wrEmmCb.ulSrsPeriod == SRS_PERIODICITY_40MS)
   {
      div=8; /* there would be 8 SS, assuming 2 SS per RF */
      srsCfgIdxInit = 46;
   }

   /* 80ms */
   else if(wrEmmCb.ulSrsPeriod == SRS_PERIODICITY_80MS)
   {
      div=16; /* there would be 16 SS, assuming 2 SS per RF */
      srsCfgIdxInit = 86;
   }
   /* 160 ms */
   else if(wrEmmCb.ulSrsPeriod == SRS_PERIODICITY_160MS)
   {
      div=32; /* there would be 32 SS, assuming 2 SS per RF */
      srsCfgIdxInit = 166;
   }
   else /* By default configuring for 20ms */
   {
      div=4; /* there would be 4 SS, assuming 2 SS per RF */
      srsCfgIdxInit = 26;
      printf("\n\n[SRS] in 20Ms configuration, start is 26 \n");
   }
   if((wrSmDfltSubframeAssignment == 3) || (wrSmDfltSubframeAssignment == 4)
         || (wrSmDfltSubframeAssignment== 5))
   {
      /* case where we have only 1 SS in a
       * Frame i.e SF=1 */
      div = div/2; /* So there would be half num of SFs */
      ueCb->srsUlDedCfg.cyclicshift = (ueIdx /div); /* Calculate CyclicShift, Residx is cyclic Shift here*/
      ueIdx1=ueIdx %div;
      ueCb->srsUlDedCfg.srsCfgIdx = (srsCfgIdxInit + 5*ueIdx1);
      printf("\n\n [SRS] invalid SUB FRAME CONFIG [%d]\n", wrSmDfltSubframeAssignment);
   }
   else
   {
      ueCb->srsUlDedCfg.cyclicshift = (ueIdx /div); /* Calculate CyclicShift, Residx is cyclic Shift here*/
      ueIdx1=ueIdx %div;
      ueCb->srsUlDedCfg.srsCfgIdx = (srsCfgIdxInit + 5*ueIdx1);
   }
   
   ueCb->idxSrsCfgIdx  = idx;
   isSrsCfgIdxUsed[idx] = TRUE;
#endif
   WR_DBGP(DBGMASK_SM, (WR_PRNT_BUF,"UL SRS Cfg Idx UE: %d Allocted: %d CfgIdx: %d CS: %d \n", ueIdx, idx, ueCb->srsUlDedCfg.srsCfgIdx, (S16)ueCb->srsUlDedCfg.cyclicshift));
   RETVOID;
}


/** @brief This function deallocates the SRS Configuration Index 
 *
 * @details
 *
 *     Function: wrUmmDelSrsCfgIdx
 *
 * @param WrUeCb     ueCb 
 * @param U8         idx
 * @return none 
 */ 
PUBLIC Void wrUmmDelSrsCfgIdx 
(
WrUeCb                       *ueCb,
U8                           idx
)
{
   WR_DBGP(DBGMASK_SM, (WR_PRNT_BUF,"UL SRS Cfg Idx UE: %d Freed : %d CfgIdx: %d \n",idx, ueCb->idxSrsCfgIdx, ueCb->srsUlDedCfg.srsCfgIdx));
   /* Free the index allocated for other UE */
   isSrsCfgIdxUsed[ueCb->idxSrsCfgIdx] = FALSE;
   RETVOID;
}

/** @brief This function adds a UE to the UE database as part of creation of
 *         connection
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the Cell control Block using cellId
 *         - In the free location of the Array of UeCbs add the UE 
 *           control block of the given crnti by allocating memory
 *         - Return the address of the UE Control Block 
 *
 * @param [in] cellId 
 * @param [in] crnti
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 


PRIVATE WrUeCb *wrUmmAddUe
(
WrCellId                     cellId,
U16                          crnti
)
{
   U8                        idx = 0;
   WrUmmCellCb               *cellCb;
   WrUeCb                    *ueCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE (NULLP);
   }

   for (idx = 0; idx < cellCb->maxUesPerCell; idx++)
   {
      /* ccpu00145343: Dont allocate the last deleted UeIdx */
      if ( idx == cellCb->lastDelUeIdx )
      {
         if ( wrUmmNumUeCnt >= (WR_MAX_UES_PER_CELL - 1))
         {
            /* Last available Ue Indx, allocate it */ 
            WR_DBGP(WR_ERROR,(WR_PRNT_BUF,"wrUmmAddUe: Allocating last deleted UeIdx\n"));
         }
         else
         {
            /* Go for next available UeIdx */
            continue; 
         }
      }
      if (cellCb->ueCbs[idx] == NULLP)
      {
         WR_ALLOC(&ueCb, sizeof(*ueCb));
         if(ueCb == NULLP)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(NULLP);
         }

         cellCb->ueCbs[idx] = ueCb;
         cellCb->ueCbs[idx]->crnti = crnti;
         cellCb->ueCbs[idx]->ueIdx = idx;
         cellCb->ueCbs[idx]->cellId = cellId;
         cellCb->ueCbs[idx]->anrMeasTransId = 0;
         cellCb->ueCbs[idx]->eventAnrTransId = 0;
         cellCb->ueCbs[idx]->rrcCsfbTransId = 0;
         cellCb->ueCbs[idx]->redirectUe = FALSE;
         cmLListInit(&ueCb->transLst);
         cmLListInit(&ueCb->tmrLst);
         wrUmmNumUeCnt ++;
         /* Initialize all the UE timers */
         cmInitTimers(&ueCb->radResModTmr.tmr, 1);
         cmInitTimers(&ueCb->intialCntRspTmr.tmr, 1);
         cmInitTimers(&ueCb->ueCtxRelTmr.tmr, 1);
         /* cmInitTimers(&ueCb->s1RelocTmr.tmr,1); */
         cmInitTimers(&ueCb->s1OvrallTmr.tmr,1);
         cmInitTimers(&ueCb->s1TgtEnbRelocTmr.tmr,1);
#ifdef WR_DETECT_RLF         
         cmInitTimers(&ueCb->cqiRprtTmr.tmr, 1);
#endif
         RETVALUE(ueCb);
      }
   }

   RETVALUE(NULLP);
}

/** @brief This function deAllocates the UeCapabilities from Ue control block
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Deallocates Ue capabilities
 *
 * @param [in] ueCap : pointer to Ue capabilities
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC Void wrUmmDeAllocUeCap
(
 WrUeCapInfo                 *ueCap
)
{
	/* Free the allocated event for ueCap */
	if( NULLP != ueCap)
	{   
		WR_FREEEVNT(ueCap);
	}
}
/** @brief This function stop all the started UMM UE timers 
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Check tmrEvnt, stop timer if it is started
 *
 * @param [in] ueCb
 * @return void
 */ 
PUBLIC void wrUmmStopUeTimers
(
WrUeCb                       *ueCb
)
{
   CmLList                   *lnk;
   WrUmmTimer                *wrtmr;

   lnk = ueCb->tmrLst.first;
   while(lnk != NULLP)
   {
     wrtmr = (WrUmmTimer *)(lnk->node);
     lnk = lnk->next;
     /*127802*/
     RLOG_ARG1(L_INFO,DBG_CRNTI,ueCb->crnti, "Stopping UE Timer Event[%d]",
        wrtmr->tmr.tmrEvnt);
     wrStopUeTmr(wrtmr->cb,wrtmr->tmr.tmrEvnt);
   }
   RETVOID;
}
/** @brief This function stop all the started UMM Transacation timers 
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Check tmrEvnt, stop timer if it is started
 *
 * @param [in] transCb
 * @return void
 */ 
PUBLIC void wrUmmStopTransTimers
(
WrUmmTransCb              *transCb
)
{
   CmLList                   *lnk;
   WrUmmTimer                *wrtmr;

   lnk = transCb->tmrLst.first;
   while(lnk != NULLP)
   {
      wrtmr = (WrUmmTimer *)(lnk->node);
      lnk = lnk->next;
      /*127802*/
     RLOG1(L_DEBUG, "Stopping Transaction Timer Event[%d]",
        wrtmr->tmr.tmrEvnt);
      wrStopTransTmr(wrtmr->cb,wrtmr->tmr.tmrEvnt);
   }

   RETVOID;
}


PRIVATE S16 wrProcUeX2HoReq( WrUeCb    *ueCb )
{
   S16 retval = RFAILED;   
   CztRlsReq *x2RelReq;

   if( ueCb == NULLP)
   {
      RETVALUE(retval);
   }    

    WR_ALLOC(&x2RelReq, sizeof(CztRlsReq)); /* ccpu00126491 */
    if(x2RelReq == NULLP)
    {
       RLOG_ARG0(L_FATAL, DBG_CRNTI, ueCb->crnti, "Memory allocation Failed");
       RETVALUE(retval);  
    }
    else
    {
       x2RelReq->peerId = ueCb->x2ConCb.peerId;
       x2RelReq->u.oldUEX2APId = ueCb->x2ConCb.oldEnbUeX2apId;
       x2RelReq->rlsType = CZT_RELTYPE_UE;
         if(ueCb->hovDir == OUT_HOV)
         {
          x2RelReq->idType = CZT_IDTYPE_LOCAL;         
         }
         else
         {
            x2RelReq->idType = CZT_IDTYPE_REMOTE;
         }
       retval = WrIfmX2apRelReq(x2RelReq);
    }
    RETVALUE(retval);
}   

/** @brief This function deletes UE from UE database which includes both the
 *         cell and the MME
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the Cell control Block using cellId
 *         - Delete UE from MME 
 *         - Delete the UE from the Cell Control Block
 *
 * @param [in] ueCb
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PRIVATE S16 wrUmmDelUe
(
WrUeCb                       *ueCb
)
{
   U8                        idx = ueCb->ueIdx;
   WrUmmCellCb               *cellCb;
   WrCellCb                  *emmCellCb = NULLP;
   U16                       minVal;
   U16                       maxVal;
   /* UE history info - start*/
   U8                        cellIdx = 0;
   TknStrOSXL                *utraCellInfo = NULLP;
   /* UE history info - end */
    /* CSG_DEV start */
   WrCsgMbrEntry             *prevCsgMbr = NULLP;
   WrCsgMbrEntry             *csgMbr     = NULLP;
   WrCsgNbrBlLst             *nbrBlNode  = NULLP;
   CmLList                   *tmpLlist   = NULLP;
   WrUmmMeasGeranMeasObjNode *geranMeasObj = NULLP;
    /* CSG_DEV end */

   /* Get EMM CellCb*/
   WR_GET_CELLCB(emmCellCb, ueCb->cellId);

   if (emmCellCb == NULLP)
   {
      RLOG0(L_ERROR, "Cell Cb not found ");
      RETVALUE(RFAILED);
   }

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(NULL == cellCb)
   {
      /*127802*/
      RLOG_ARG0(L_ERROR, DBG_CELLID, ueCb->cellId, "Cell Cb is NULL ");
      RETVALUE(RFAILED);
   }
   wrUmmDelSrsCfgIdx(ueCb, ueCb->ueIdx); 
   
   if (ueCb->mmeLnk.node != NULLP)
   {
      wrUmmDelUeFrmMme(ueCb, ueCb->mmeId);
   }
   wrUmmStopUeTimers(ueCb);

   wrUmmDeAllocUeCap(ueCb->ueCap);
   wrUmmMeasUeCbDeInit(ueCb);
   if(ueCb->secCfg != NULLP)
   {
      WR_FREE(ueCb->secCfg,sizeof(WrUeSecCfg));
   }
#ifdef TENB_AS_SECURITY
   if(ueCb->hzSecCfg != NULLP)
   {
      WR_FREE(ueCb->hzSecCfg,sizeof(WrUeSecCfg));
   }
#endif
   if(ueCb->s1ConCb != NULLP)
   {
      WR_FREE(ueCb->s1ConCb,sizeof(WrS1ConCb));
   }

/* UE history info - start */
   for(cellIdx = 0; cellIdx < ueCb->ueHstryInfo.numOfCells; cellIdx++)
   {
      /* EUTRAN: memory is not being allocated */

      /* Deallocating memory for UTRAN */
      if ( ueCb->ueHstryInfo.cellInfo[cellIdx].ratType == LASTVISITEDCELL_ITEM_UTRAN_CELL)
      {
         utraCellInfo = &(ueCb->ueHstryInfo.cellInfo[cellIdx].ratInfo.utranInfo);
         if(utraCellInfo->val != NULLP)
         {
            WR_FREE(utraCellInfo->val,utraCellInfo->len);
         }
      }
      
      /*GERAN: Need to update GERAN as necessary later*/
   }
/* UE history info - end */

   if(ueCb->hoType == WR_HO_TYPE_X2)
   {
     /* ccpu00148952 */
     RLOG_ARG0(L_ALWAYS, DBG_CRNTI, ueCb->crnti, "Sending Release for X2 "); 
     /* ccpu00148952 */
       wrProcUeX2HoReq(ueCb);
   }
   /* For GERAN Case Measurement object is maintained in hrlMeasCfg
    * eventhough there is no HRL for the UE  for rest of the case
    * like UTRA, CDMA, etc geranMeasObjLst is freed inside wrFreeUeHrlMeasCfg
    * */
   cmLListFirst(&ueCb->hrlMeasCfg.geranMeasObjLst);   
   while(cmLListCrnt(&ueCb->hrlMeasCfg.geranMeasObjLst))
   {
      geranMeasObj = (WrUmmMeasGeranMeasObjNode *) cmLListNode \
                    (cmLListCrnt(&ueCb->hrlMeasCfg.geranMeasObjLst));
      cmLListDelFrm(&ueCb->hrlMeasCfg.geranMeasObjLst, &geranMeasObj->lnk);
      cmLListNext(&ueCb->hrlMeasCfg.geranMeasObjLst);
      WR_FREE(geranMeasObj, sizeof(WrUmmMeasGeranMeasObjNode));
   }
   
   if(ueCb->hoRstrctInfo != NULLP)
   {
      wrFreeUeHrlMeasCfg(ueCb); 
      WR_FREE_EVNT(ueCb->hoRstrctInfo);
   }
   else if(ueCb->regLai.isLaiPres == TRUE)
   {
      wrFreeUeHrlMeasCfg(ueCb); 
   }

   if(ROK == wrEmmCellDedCrntiRange(ueCb->cellId,&minVal,&maxVal))
   {
         if(ueCb->crnti >= minVal && ueCb->crnti <= maxVal)
         {
            wrUtlFreeDedCrnti(ueCb->crnti, ueCb->cellId);
         }
   }
   /* SPS changes starts */
   wrUtlFreeSpsRnti(ueCb->spsCfg.spsRnti, ueCb->cellId);
   /* SPS changes ends */

/* LTE_ADV_ABS starts */
    WrEmmAbsEvent  event;
    event.eventId = WR_EMM_ABS_UE_REL_IND_EVENT;
    event.u.absUeEvent.ueId = ueCb->crnti;
    wrEmmAbsProcessEvent(&event); 
/* LTE_ADV_ABS ends */

   /* CSG_DEV Start */
   /* Remove entire CSG ID list for the UE and free the memory for each member */
   while(cmHashListGetNext(&(ueCb->csgMbrLst),(PTR)prevCsgMbr,(PTR*)&csgMbr) == ROK)
   { 
      cmHashListDelete(&(ueCb->csgMbrLst),(PTR)csgMbr);
      WR_FREE(csgMbr,sizeof(WrCsgMbrEntry));
   }
   cmHashListDeinit(&(ueCb->csgMbrLst));

   nbrBlNode = (WrCsgNbrBlLst*)CM_LLIST_FIRST_NODE(&(ueCb->csgNbrBlLst),
         tmpLlist);
   while(NULLP != nbrBlNode)
   {
      cmLListDelFrm(&(ueCb->csgNbrBlLst), tmpLlist);

      WR_FREE(nbrBlNode, sizeof(WrCsgNbrBlLst));

      nbrBlNode = (WrCsgNbrBlLst*)CM_LLIST_NEXT_NODE(&(ueCb->csgNbrBlLst),
            tmpLlist);
   }
   /* CSG_DEV Ends */

    /*
     * Check for the Cell Staste if it is RESET 
     * Trigger Cell Reset Once all the UE Delete
     * Has been received 
     */
      if(WR_CELL_STATE_RESET == cellCb->cellState)
      {
         /*127802*/
         RLOG1(L_INFO, "=============INFO::: noUeCntxToRel [%d]===============",
            cellCb->noUeCntxToRel);
         cellCb->noUeCntxToRel--;
         if(0 == cellCb->noUeCntxToRel)
         {
            wrCb.resetCmfState |= WR_UE_RELEASE_DONE;    
            wrCb.parResetCmfState |= WR_UE_RELEASE_DONE;    
            /* Stop the timer if running */
            wrStopTmr((PTR)cellCb, WR_TMR_GUARD_UE_CTX_REL);
         }
         if(WR_ALL_RESET_DONE == wrCb.resetCmfState)
         {
            wrCb.resetCmfState = WR_INIT_STATE_NONE;
            if(ROK != wrFillCtfCellDelCfgReq(ueCb->cellId))
            {
               RLOG_ARG0(L_ERROR, DBG_CELLID, ueCb->cellId,
                  "wrFillCtfCellDelCfgReq failed");
               RETVALUE(RFAILED); 
            }
         }
         if(WR_PAR_RESET_DONE == wrCb.parResetCmfState)
         {
            wrCb.parResetCmfState = WR_INIT_STATE_NONE;
            cellCb->cellState = WR_CELL_STATE_UP;
            RLOG_ARG0(L_INFO, DBG_CELLID, cellCb->cellId, "Cell State = UP, "
               "Cause = RESET Complete");
            /* Update the EMM Cell Cb State to Cell Up */
            if(ROK != wrEmmUpdateCellState(cellCb->cellId,WR_CELL_STATE_UP))
            {
               RLOG_ARG0(L_ERROR, DBG_CELLID, cellCb->cellId, "Cell Cb Not FOund");
               RETVALUE(RFAILED);
            }
         }
      }
      else if(WR_CELL_STATE_PWR_DOWN == cellCb->cellState)
      {
         /* Fix for CR ccpu00143897 */ 
         if ( cellCb->noUeCntxToRel>0)
         {
            if ( ueCb->tobeReleased == TRUE )
            {
               cellCb->noUeCntxToRel--;
            }
            RLOG1(L_DEBUG, "Releasing noUeCntxToRel:%d",cellCb->noUeCntxToRel);
            if(0 == cellCb->noUeCntxToRel)
            {
               ALARM("Stopping WR_TMR_GUARD_UE_CTX_REL timer\n");
               wrStopTmr((PTR)cellCb, WR_TMR_GUARD_UE_CTX_REL);
               /* Check if the Cell is in power down state, bring down the power of the 
               * cell. This function call is added here to make sure that the all UE
               * releated resources are released from by eNB before bringing down
               * the power of cell. */
               RLOG0(L_DEBUG,"Bringing the CELL POWER DOWN inside wrUmmDelUe");
               wrEmmMmeBrngCellPwrDown();
               /* ccpu00142992:[BCM][GA4.0][TDD][S1AP]TTIs are not coming after
               * s1-close triggered from MME when UE is in attached state
               */
               wrEmmCallMmeSetup();
            }
         }
         else
         {
            wrEmmCallMmeSetup();
         }
      }
      /*127802*/
     RLOG_ARG1(L_INFO, DBG_CRNTI, ueCb->crnti, "UeCb getting deleted Index %d",
        idx); 
      wrUmmNumUeCnt --;
      WR_FREE(ueCb, sizeof(*ueCb));
      cellCb->ueCbs[idx] = NULLP;
      /* ccpu00145343: Store last deleted UeIdx,
       * this idx would not be allocated to next UE, if no other UeIdx
       * is available then allocate it */
      cellCb->lastDelUeIdx = idx;

   RETVALUE(ROK);
}

#define WR_UMM_AVAILABLE 0
#define WR_UMM_INUSE     1
/** @brief This function locates a LCH id that is available and reserves it for
 *         the caller
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - In the Ue Control block iterate for the list of Logical
 *           channels and get the index of the free logical channel
 *
 * @param [in] ueCb
 * @param [in] lch
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmGetLch
(
WrUeCb                       *ueCb,
U8                           *lch
)
{
   U32                       idx;

   for(idx = 3; idx <  WR_UMM_MAX_LCHS; idx++)
   {
      if (ueCb->lchs[idx] == WR_UMM_AVAILABLE)
      {
         *lch = idx;
         ueCb->lchs[idx] = WR_UMM_INUSE;
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
}

/* ccpu00128207 */
/** @brief This function locates a LCH id and mark it as reserved
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the index of the LCH ID in the UE Control Block
 *
 * @param [in] ueCb
 * @param [in] lch
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmSetLch
(
WrUeCb                       *ueCb,
U8                           lch
)
{
   if (ueCb->lchs[lch] != WR_UMM_INUSE)
   {
      ueCb->lchs[lch] = WR_UMM_INUSE;
      RETVALUE(ROK);
   }

   RETVALUE(RFAILED);
}

/** @brief This function releases the passed LCH id and marks it available  
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Sets the correponding index of the logical Channel in UE
 *           as available
 *
 * @param [in] ueCb
 * @param [in] lch
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmRelLch
(
WrUeCb                       *ueCb,
U8                           lch
)
{
   if (ueCb->lchs[lch] == WR_UMM_AVAILABLE)
   {
      /*127802*/
      RLOG1(L_WARNING, "Releasing %lu LCH that is not in use",(U32)lch);
   }
   ueCb->lchs[lch] = WR_UMM_AVAILABLE;
   RETVALUE(ROK);
}

/** @brief This function locates a DRB id that is available and reserves it for
 *         the caller
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Gets the index of the DRB in the UE Control Block
 *
 * @param [in] ueCb
 * @param [in] drb
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmGetDrb
(
WrUeCb                       *ueCb,
U8                           *drb
)
{
   U32                       idx;

   /* drb id can range between 1..32 */
   for(idx = 1; idx < WR_UMM_MAX_DRBS; idx++)
   {
      if (ueCb->drbs[idx] == WR_UMM_AVAILABLE)
      {
         *drb = idx;
         ueCb->drbs[idx] = WR_UMM_INUSE;
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
}

/* ccpu00128207 */
/** @brief This function locates the DRB id and mark it as reserved
 * 
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Gets the index of the DRB in the UE Control Block
 *
 * @param [in] ueCb
 * @param [in] drb
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmSetDrb
(
WrUeCb                       *ueCb,
U8                           drb
)
{
   if (ueCb->drbs[drb] != WR_UMM_INUSE)
   {
      ueCb->drbs[drb] = WR_UMM_INUSE;
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}

/** @brief This function releases the passed DRB id and marks it available
 *
 * @details
 *
 *     Function: wrUmmRelDrb
 *
 *         Processing steps:
 *         - Gets the index of the DRB in the UE Control Block
 *
 * @param [in] ueCb
 * @param [in] drb
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmRelDrb
(
WrUeCb                       *ueCb,
U8                           drb
)
{
   if (ueCb->drbs[drb] == WR_UMM_AVAILABLE)
   {
      /*127802*/
      RLOG_ARG1(L_WARNING,DBG_CRNTI,ueCb->crnti,"Releasing %lu DRB that is not in use",(U32)drb);
   }
   ueCb->drbs[drb] = WR_UMM_AVAILABLE;
   RETVALUE(ROK);
}

/** @brief This function is used to get the DRB ID from the given ERAB ID.
 *
 * @details
 *
 *     Function: wrUmmGetDrbIdFromErabId
 *
 *         Processing steps:
 *         - validate if the ERAB ID is already configured
 *         - set the the DRB ID to drbId
 *
 * @param [in]     ueCb   - UE Control Block
 * @param [in]     erabId - ERAB Identity
 * @param [in/out] drbId  - DRB Identity
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmGetDrbIdFromErabId
(
WrUeCb                       *ueCb,
U8                           erabId,
U8                           *drbId
)
{
   U8                       idx = erabId - WR_UMM_ERAB_OFFSET;

   if(idx >= WR_MAX_DRBS)
   {
      RETVALUE(RFAILED);
   }

   if(ueCb->rbInfo.rab[idx].eRABId == erabId)
   {
      *drbId = ueCb->rbInfo.rab[idx].rbId;
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}

/** @brief This is a dummy function that gets called in case the actual
 *         implementation is not provided by the appropriate transaction 
 * @details
 *
 *     Function: wrUmmDummyTransProcessor
 *
 *         Processing steps:
 *         - Prints that no processor is defined for this transaction type
 *
 * @param [in] transCb
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 


/** @brief This is a dummy function that gets called in case the actual
 *         implementation is not provided by the appropriate transaction 
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Prints that no message validation is provided
 *
 * @param [in] transCb
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PRIVATE S16 wrUmmDummyTransAllowMsg
(
WrUmmTransCb                 *transCb,
WrUmmIncMsg                  *msg
)
{
   /*127802*/
   RLOG1(L_WARNING, "No message validation  defined for transaction = %u",transCb->transTyp);
   RETVALUE(RFAILED);
}

/** @brief This is a dummy function that gets called in case the actual
 *         implementation is not provided by the appropriate transaction 
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Prints that no transaction release is provided
 *
 * @param [in] transCb
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 

#define wrUmmIncMsgAddRef(_msg) _msg->refCnt++;

/** @brief This function releases the received message. All messages are
 *         released upon processing. If a transaction intends to keep a copy,
 *         refCnt field of the message may be bumped up. It would be the
 *         responsibility of the transaction to release such messages.
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Frees the incoming message based on the reference count
 *
 * @param [in] msg
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 


PUBLIC void wrUmmRlsIncMsg
(
 WrUmmIncMsg                  **incMsg
 )
{
   WrUmmIncMsg                  *msg = *incMsg;

   if(msg == NULLP)
   {
      RETVOID;
   }

   if (msg->refCnt > 1)
   {
      msg->refCnt--;
      RETVOID;
   }

   switch (msg->msgTyp)
   {
      case WR_UMM_S1_CON_PDU:
      {
         WR_FREE_EVNT(msg->u.s1Pdu->pdu);
         WR_FREE(msg->u.s1Pdu, sizeof(*(msg->u.s1Pdu)));
         break;
      }
      case WR_UMM_S1_DAT_IND:
      {
         WR_FREE_EVNT(msg->u.s1Pdu->pdu);
         WR_FREE(msg->u.s1Pdu, sizeof(*(msg->u.s1Pdu)));
         break;
      }
      case WR_UMM_S1_REL_IND:
      {
         /* MEM LEAK FIX: */
         WR_FREE_EVNT(msg->u.relInd->pdu);
         WR_FREE(msg->u.relInd, sizeof(SztRelInd));
         break;
      }
      case WR_UMM_S1_CON_CFM:
      {
         WR_FREE_EVNT(msg->u.cfmPdu->pdu);
         WR_FREE(msg->u.cfmPdu, sizeof(*(msg->u.cfmPdu)));
         break;
      }
      case WR_UMM_RRC_ENC_CFM:
      {
         WR_FREEMBUF(msg->u.encCfm->sdu.mBuf); 
         WR_FREE(msg->u.encCfm, sizeof(*(msg->u.encCfm)));
         break;
      }
      case WR_UMM_RRC_DEC_CFM:
      {
         WR_FREE_EVNT(msg->u.decCfm);
         break;
      }
 
      case WR_UMM_X2_BMP_PDU:
      {
         WR_FREE_EVNT(msg->u.x2Pdu);
         break;
      }
      case WR_UMM_UU_IND_PDU:
      {
         WR_FREE_EVNT(msg->u.uuIndPdu);
         break;
      }
      case WR_UMM_UU_CFM_PDU:
      {
         WR_FREE_EVNT(msg->u.uuCfmPdu);
         break;
      }
      case WR_UMM_RRC_CFG:
      {
         WR_FREE(msg->u.rrcCfm, sizeof(*(msg->u.rrcCfm)));
         break;
      }
      case WR_UMM_SCHD_CFG:
      {
         break;
      }
      case WR_UMM_PHY_CFG:
      {
         break;
      }
      case WR_UMM_RRC_ERR_IND:
      {
         WR_FREE(msg->u.rrcErrInd, sizeof(*(msg->u.rrcErrInd)));
         break;
      }
      case WR_UMM_INTERNAL_MSG:
      {
         if((msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_ANR_MEAS_TRANS) || 
            (msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_EVNT_ANR_MEAS_TRANS))
         {
            WR_FREE(msg->u.internalMsg->u.anrMeasMsg, sizeof(WrUmmIntAnrMeasMsg));
         }
/* Handover */
         else if(msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_CR_HO_TRANS) 
         {
            WR_FREE(msg->u.internalMsg->u.hoTrans, sizeof(WrUmmIntCrHoTran));
         }
/* LTE_ADV starts */
         else if(msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_LTE_ADV_TRANS)
         {
            WR_FREE(msg->u.internalMsg->u.lteAdvMsg, sizeof(WrUmmIntLteAdvMsg));
         }
/* LTE_ADV ends */
         else if((msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_CSFB_ECSFB_TRANS) ||
		    (msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_CSFB_REDIR_TRANS ) ||
		    (msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_CSFB_CCO_TRANS) ||
          (msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_ECSFB_TRANS))
         {
            WR_FREE(msg->u.internalMsg->u.csfbIndMsg, sizeof(WrUmmIntCsfbTransMsg));
         } 
         else if((msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_MEAS_TRANS))
         {
            WrUmmIntMeasTrans         *nrUpdtNode;
            nrUpdtNode = msg->u.internalMsg->u.measTrans;

            WR_FREE_EVNT(nrUpdtNode);
         }
         else if((msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_NEW_MEAS_TRANS))
         {
            WrMeasCellReCfgInfo       *reCfgInfo;

            reCfgInfo = msg->u.internalMsg->u.measNewTrans;

            WR_FREE(reCfgInfo, sizeof(WrMeasCellReCfgInfo));
         }
         else if((msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_REDIR_TRANS))
         {
            WrUmmRedirectInfo  *redirInfo = NULLP;
            redirInfo = msg->u.internalMsg->u.redirInfo;
            if(redirInfo != NULLP)
            {
               WR_FREE(redirInfo, sizeof(WrUmmRedirectInfo));
            }
         } 
         WR_FREE(msg->u.internalMsg, sizeof(WrUmmInternalMsg));
         break;
      }
      /* Fix : Memory Leak */
      case WR_UMM_RRC_DAT_CFM:
      {
         WR_FREE(msg->u.rrcDatCfm, sizeof(*(msg->u.rrcDatCfm)));
         break;
      }
      case WR_UMM_PDCP_SDU_STATUS_CFM:
      {
         WR_FREE_EVNT(msg->u.pdcpSduCfm);
         break;
      }
      case WR_UMM_PDCP_DATA_RESUME_CFM:
      {
         WR_FREE_EVNT(msg->u.pdcpDatResumeCfm);
         break;
      }
      case WR_UMM_DAM_CFM:
/* ccpu00126336 */
      case WR_UMM_DAM_MOD_CFM:
      case WR_UMM_DAM_UE_DEL_CFM:
      case WR_UMM_LCL_CTXT_REL:
      case WR_UMM_TMR_EXPIRY:
      {
         break;
      }
      /* Fix ccpu00126717 */
      case WR_UMM_RAB_RLS_EVNT:
      {
         WR_FREE((msg->u.lcrabrls),sizeof(*(msg->u.lcrabrls)));
         break;
      }
      case WR_UMM_RRM_UEREL_RESP:
      {
         /* Free the UE Relese Response memory */
         WR_FREE((msg->u.rrmUeRelResp), sizeof(RmuCommonMsg));
         break;
      }
      case WR_UMM_RRM_UEADMIT_RESP:
      {
         WR_FREE((msg->u.rrmUeAdmitResp), sizeof(RmuCommonMsg));
      }
      break;
      case WR_UMM_RRM_UEHO_RESP:
      {
         WR_FREE((msg->u.rrmUeHoResp), sizeof(RmuCommonMsg));
      }
      break;
      case WR_UMM_RRM_UERECFG_RESP:
      {
         WR_FREE((msg->u.rrmUeRecfgResp), sizeof(RmuCommonMsg));
      }
      break;
      case WR_UMM_RRM_UE_ERAB_REL_IND:
      {
         WR_FREE((msg->u.rmuUeErabRelInd), sizeof(RmuCommonMsg));
      }
      break;
     /* Fix for - ccpu00138051  */
      case WR_UMM_X2AP_ERR_IND:
      {
         WR_FREE((msg->u.cztLclErrInd),sizeof(CztLclErrInd));
      }
      break;
      case WR_UMM_RRM_UERECFG_IND:
      {
         WR_FREE((msg->u.rrmUeRecfgInd), sizeof(RmuCommonMsg));
      }
      default:
      {
         RLOG1(L_DEBUG, "Message release is not implemented for msg Type = %d",
            msg->msgTyp);
      }
   }
   WR_FREE(*incMsg, sizeof(*msg));
   RETVOID;
}

#define WR_TRANS_MASK 0xfffffff0

/** @brief This function locates a transction control block by the transaction
 *        ID passed. Each transaction has first 4 bits as the cell ID, next 16 
 *        bits as CRNTI, next 8 bits as the transaction which is unique with in
 *        the UE context. The last 4 bits are provided as subtransactions and 
 *        are ignored when matching transaction identities.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the Transaction Control Block based on the transaction Identity
 *
 * @param [in] ueCb
 * @return WrUmmTransCb  *transCb
 */ 


PRIVATE WrUmmTransCb *wrUmmGetTransCbById
(
WrUeCb                       *ueCb,
U32                          transId
)
{
   CmLList                   *lnk;
   WrUmmTransCb              *transCb;

   lnk = ueCb->transLst.first;

   while(lnk != NULLP)
   {
      transCb = (WrUmmTransCb *)(lnk->node);
      lnk     = lnk->next;
      /* Out of 32 bits in transaction id, first 28 identify the major    */
      /* transaction. The last four bits are available for subtransactions*/
      if ((transCb != NULLP) && 
            ((transCb->transId & WR_TRANS_MASK) == (transId & WR_TRANS_MASK)))
      {
         RETVALUE(transCb);
      }
   }

   RETVALUE(NULLP);
}

/** @brief This function marks a transaction as complete. When an incoming
 *        message is processed, if the transaction is marked as complete, that
 *        transaction is released by the framework.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the Transaction Control Block based on the transaction Identity
 *
 * @param [in] transCb
 */ 
PUBLIC Void wrUmmTransComplete
(
WrUmmTransCb                 *transCb
)
{
   transCb->state = WR_UMM_TRANS_DONE;
}

/** @brief This function returns the transaction identifier received from lower
 *        interface. Some messages do not contain and require special
 *        processing to identify the transaction. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Get the Transaction Control Block based on the transaction Identity
 *
 * @param [in] transCb
 * @return  Returns WR_CREATE_NEW_TRANS if this message initiates a transaction. 
 */ 
PRIVATE S16 wrUmmGetTransIdFromMsg
(
WrUeCb                       *ueCb,
WrUmmIncMsg                  *msg,
U32                          *transId
)
{
   switch(msg->msgTyp)
   {
      case WR_UMM_S1_DAT_IND:
      {
         RETVALUE(wrUmmS1GetTransIdFromMsg(ueCb, msg, transId));
      }
      case WR_UMM_S1_CON_PDU:
      {
         *transId = WR_CREATE_NEW_TRANS;
         RETVALUE(ROK);
      }
      case WR_UMM_S1_CON_CFM:
      {
         if(ueCb->hoTransId)
         {
            *transId = ueCb->hoTransId;
         }
         else
         {
            *transId = WR_CREATE_NEW_TRANS;
         }
         RETVALUE(ROK);
      }
      case WR_UMM_X2_BMP_PDU:
      {
         RETVALUE(wrUmmX2GetTransIdFromMsg(ueCb, msg, transId));
      }
      /*ccpu00130220*/
      case WR_UMM_X2_STA_IND:
      {
         if(ueCb->hoTransId)
         {
            *transId = ueCb->hoTransId;
            RETVALUE(ROK);
         }
         RETVALUE(RFAILED);
      }
      case WR_UMM_SND_HANDOVER_CANCEL: 
      {
         if(ueCb->hoTransId)
         {
            *transId = ueCb->hoTransId;
            RETVALUE(ROK);
         }
         RETVALUE(RFAILED);
      }
      /*ccpu00130220*/
      case WR_UMM_UU_IND_PDU:
      case WR_UMM_UU_CFM_PDU:
      {
         RETVALUE(wrUmmUuGetTransIdFromMsg(ueCb, msg, transId));
      }
      case WR_UMM_SCHD_CFG:
      {
         *transId = msg->u.schdCfm.transId;
         RETVALUE(ROK);
      }
      case WR_UMM_PHY_CFG:
      {
         *transId = msg->u.phyCfm.transId;
         RETVALUE(ROK);
      }
#ifdef TENB_AS_SECURITY
      case WR_UMM_PHY_KDF:
      {
         *transId = msg->u.kdfCfm.transId;
         RETVALUE(ROK);
      }
#endif
      case WR_UMM_RRC_CFG:
      {
         *transId = msg->u.rrcCfm->hdr.transId;
         RETVALUE(ROK);
      } 
      case WR_UMM_RRC_ENC_CFM:
      {
         *transId = msg->u.encCfm->hdr.transId;
         RETVALUE(ROK);
      }
      case WR_UMM_RRC_DEC_CFM:
      {
         *transId = msg->u.decCfm->hdr.transId;
         RETVALUE(ROK);
      }
      case WR_UMM_INTERNAL_MSG:
      {
         if(msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_ANR_MEAS_TRANS) 
         {
            if(ueCb->anrMeasTransId)
            {
               *transId = ueCb->anrMeasTransId;
               RETVALUE(ROK);
            }
         }
         else if(msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_EVNT_ANR_MEAS_TRANS)
         {
            if(ueCb->eventAnrTransId > 0 )
             {
               *transId = ueCb->eventAnrTransId;
            }
            else if(msg->u.internalMsg->u.anrMeasMsg->choice == WR_UMM_ANR_MEAS_CREATE)
            {
               *transId = WR_CREATE_NEW_TRANS;
            }
         }
         else if(msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_CSFB_REDIR_TRANS)
         {
            *transId = WR_CREATE_NEW_TRANS;
            if(ueCb->csfbRedirTransId)
            {
               *transId = ueCb->csfbRedirTransId;
            }
         }
         else
         {
            *transId = WR_CREATE_NEW_TRANS;
         }
         RETVALUE(ROK);
      }
      case WR_UMM_LCL_CTXT_REL:
      {
         *transId = WR_CREATE_NEW_TRANS;
         RETVALUE(ROK);
      }
      case WR_UMM_DAM_UE_DEL_CFM:
      {
         *transId = msg->u.damUeDelCfm.transId;
         RETVALUE(ROK);
      }
      case WR_UMM_DAM_CFM:
/* ccpu00126336 */
      case WR_UMM_DAM_MOD_CFM:
      {
         *transId = msg->u.damCfm.transId;
         RETVALUE(ROK);
      }
      case WR_UMM_RAB_RLS_EVNT:
      { 
         *transId= WR_CREATE_NEW_TRANS;
         RETVALUE(ROK);
      }
      case WR_UMM_TMR_EXPIRY:
      {
         *transId = msg->u.tmrExpiry.transId;
         RETVALUE(ROK);
      }
      case WR_UMM_S1_REL_IND:
      {
         *transId = WR_CREATE_NEW_TRANS;
         RETVALUE(ROK);
      }
      case WR_UMM_RRC_DAT_CFM:
      {
         *transId = msg->u.rrcDatCfm->hdr.transId;
         RETVALUE(ROK);
      }
      case WR_UMM_TMREXP_INACTIVITY:
      {
         *transId = WR_CREATE_NEW_TRANS;
         RETVALUE(ROK);
      }
      case WR_UMM_RRC_ERR_IND:
      {
         *transId = msg->u.rrcErrInd->hdr.transId;
         RETVALUE(ROK);
      }
      case WR_UMM_PDCP_SDU_STATUS_CFM:
      {
         *transId = msg->u.pdcpSduCfm->hdr.transId;
         RETVALUE(ROK);
      }
      case WR_UMM_PDCP_DATA_RESUME_CFM:
      {
         *transId = msg->u.pdcpDatResumeCfm->hdr.transId;
         RETVALUE(ROK);
      }
      case WR_UMM_S1AP_ENC_DEC_CFM:
      {
         *transId = msg->u.sztEncCfm->transId;
         RETVALUE(ROK);
      }
      case WR_UMM_S1AP_ENC_DEC_FAIL:
      {
         *transId = msg->u.sztEncCfm->transId;
         RETVALUE(ROK);
      }
      case WR_UMM_SRCTOTGT_CONT_DEC_CFM:
      case WR_UMM_TGTTOSRC_CONT_DEC_CFM:
      {
         *transId = msg->u.sztDecCfm->transId;
         RETVALUE(ROK);
      }
      case WR_UMM_X2AP_ERR_IND:
      {
         if(ueCb->hoTransId)
            *transId = ueCb->hoTransId;
         RETVALUE(ROK);
      }
      case WR_UMM_RRM_UEADMIT_RESP:
      {
         *transId = msg->u.rrmUeAdmitResp->uiTransId;
         RETVALUE(ROK);
      }
      case WR_UMM_RRM_UEHO_RESP:
      {
         *transId = msg->u.rrmUeHoResp->uiTransId;
         RETVALUE(ROK);
      }
      case WR_UMM_RRM_UEREL_RESP:
      {
         *transId = msg->u.rrmUeRelResp->uiTransId;
         RETVALUE(ROK);
      }
      case WR_UMM_RRM_UERECFG_RESP:
      {
         *transId = msg->u.rrmUeRecfgResp->uiTransId;
         RETVALUE(ROK);
      }
      case WR_UMM_RRM_UERECFG_IND:
      {
         *transId = WR_CREATE_NEW_TRANS;
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
}


/** @brief This function is responsible for identifying transaction type based
 * on the received message. All transactions start with initiating messages
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - assign Measurement transaction type
 *
 * @param [in] msg
 * @param [in] transTyp
 * @return  Returns the transaction type based on the message
 */ 
PUBLIC S16 wrUmmGetIntMsgTransType
(
WrUmmIncMsg                  *msg,
U32                          *transTyp
)
{
   U8                        msgType = msg->u.internalMsg->intMsgType;
   U8                        hoType;

   switch(msgType)
   {
      case WR_UMM_INTMSG_CR_HO_TRANS:
         {
            hoType = msg->u.internalMsg->u.hoTrans->hoType;
            if(( WR_HO_TYPE_INTRA_LTE_S1 == hoType) || 
               ( WR_HO_TYPE_IRAT_UTRA == hoType) ||
               ( WR_HO_TYPE_IRAT_GERAN == hoType))
            {
               *transTyp = WR_UMM_S1_SRC_HO_TRANS;
            }
            else
            {
               *transTyp = WR_UMM_X2_SRC_HO_TRANS;
            }
         }
         break;
      case WR_UMM_INTMSG_MEAS_TRANS:
      case WR_UMM_INTMSG_NEW_MEAS_TRANS:
         {
            *transTyp = WR_UMM_MEAS_TRANS;
            RETVALUE(ROK);
         }
      case WR_UMM_INTMSG_CSFB_REDIR_TRANS:
      case WR_UMM_INTMSG_CSFB_ECSFB_TRANS:
         {
            *transTyp = WR_UMM_CSFB_REDIR_TRANS;
         }
         break;
         /* eCSFB - Added for ecsfb */
      case WR_UMM_INTMSG_ECSFB_TRANS:
         {
            *transTyp = WR_UMM_ECSFB_TRANS;
         }
         break;
      case WR_UMM_INTMSG_CSFB_CCO_TRANS:
         {
            *transTyp = WR_UMM_CSFB_CCO_TRANS;
         }
         break;
      case WR_UMM_INTMSG_RAD_RES_MOD_MSG: 
      case WR_UMM_INTMSG_MOD_SR_CQI_RES_MSG: 
         /* SPS changes starts */
      case WR_UMM_INTMSG_UNSET_SR_MASK:
         /* SPS changes ends */
         { 
            *transTyp = WR_UMM_RAD_RES_MOD_TRANS;
         }
         break;
      case WR_UMM_INTMSG_EVNT_ANR_MEAS_TRANS:
         {
            *transTyp = WR_UMM_EVENT_ANR_TRANS;
         }
         break;
      case WR_UMM_INTMSG_ANR_MEAS_TRANS: 
         { 
            *transTyp = WR_UMM_ANR_MEAS_TRANS;
         }
         break;
         /* LTE_ADV starts */
      case WR_UMM_INTMSG_LTE_ADV_TRANS:
         {
            *transTyp = WR_UMM_LTE_ADV_TRANS;
         }
         break;
      case WR_UMM_INTMSG_REDIR_TRANS:
         {
            *transTyp = WR_UMM_CTXT_REL_TRANS;
         }
         break;
      default:
         {
            RLOG0(L_ERROR, "Invalid MsgType"); 
            RETVALUE(RFAILED);
         }

         /* LTE_ADV ends */
   } /* end of switch */
   RETVALUE(ROK);
}

/** @brief This function returns the transaction type based on the
 *         incoming message
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Based on the message type get the Transaction type
 *
 * @param [in] ueCb
 * @param [in] msg
 * @param [in] transTyp
 * @return  Returns the transaction type based on the message
 */ 
PRIVATE S16 wrUmmGetTransTypFromMsg
(
 WrUeCb                       *ueCb,
 WrUmmIncMsg                  *msg,
 U32                          *transTyp
 )
{
   switch(msg->msgTyp)
   {
      case WR_UMM_S1_DAT_IND:
         {
            RETVALUE(wrUmmGetS1TransType(msg->u.s1Pdu->pdu, transTyp));
         }
      case WR_UMM_S1_CON_PDU:
         {
            RETVALUE(wrUmmGetS1TransType(msg->u.s1Pdu->pdu, transTyp));
         }
      case WR_UMM_S1_CON_CFM:
         {
            RETVALUE(wrUmmGetS1TransType(msg->u.cfmPdu->pdu, transTyp));
         }
      case WR_UMM_RAB_RLS_EVNT:
         { 
            *transTyp = WR_UMM_LC_RAB_RLS_TRANS;
            RETVALUE(ROK);
         }
      case WR_UMM_X2_BMP_PDU:
         {
            RETVALUE(wrUmmGetX2TransType(msg, transTyp));
         }
      case WR_UMM_UU_IND_PDU:
         {
            RETVALUE(wrUmmGetUuTransType(ueCb, msg, transTyp));
         }
      case WR_UMM_LCL_CTXT_REL:
      case WR_UMM_S1_REL_IND:
         {
            *transTyp = WR_UMM_CTXT_REL_TRANS;
            RETVALUE(ROK);
         }
      case WR_UMM_INTERNAL_MSG:
         {
            RETVALUE(wrUmmGetIntMsgTransType(msg, transTyp));
         }
      case WR_UMM_RRM_UERECFG_IND:
         { 
            *transTyp = WR_UMM_TX_MODE_CHNG_TRANS;
            RETVALUE(ROK);
         }
   }
   RETVALUE(RFAILED);
}



/** @brief This function releases the transaction from the 
 *         UE Control Block
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Delete the Transaction Control Block from UE
 *
 *
 * @param [in] transCb
 * @return Void
 */ 

PRIVATE Void wrUmmTransRel
(
 WrUmmTransCb                 *transCb
 )
{
   WrUeCb                    *ueCb = transCb->ueCb;
   CmLList                   *lnk;
   U8                        transId = WR_GET_TRANS_FRM_TRANSID(transCb->transId);
   U8                        cnt = 0;
   WrUmmTransCb              *defTransCb;
   WrUmmCellCb               *cellCb;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, ueCb->cellId, "wrUmmRrcReestabTransRel:Cell Cb is NULL ");
      RETVOID;
   }

   transCb->ueCb->transIdInUse[transId] = FALSE;
   /* Stopping all timers not stopped with transcation
    * ideally by this time all timers should be stopped */
   wrUmmStopTransTimers(transCb);
   /* Fix */
   cmLListDelFrm(&ueCb->transLst, &transCb->lnk);
   transCb->lnk.node = NULLP;
   RLOG0(L_DEBUG, " Calling RRC TransId Free(wrUmmFreeRrcTransId) for the Transaction "
        "Message Type passing as zero "); 
   wrUtlPrntIncTansType(transCb->transTyp, ueCb->crnti, 0, transCb->transId);
   wrUmmFreeRrcTransId(ueCb,transId);
   wrUmmTransRelease[transCb->transTyp](transCb);
   wrUmmNumTransCnt --;
   /* Move all deferred transactions of this transaction to 
    * ueCb main transLst 
    */
   lnk = transCb->deferredTrans.first;
   while(lnk != NULLP)
   {
      defTransCb = (WrUmmTransCb *)(lnk->node);
      wrUmmAddDefTransToUeTransLst(transCb, defTransCb);
      RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,
            "Starting deferred transaction[%ld] ", defTransCb->transId);
      lnk = lnk->next;
   }
   /*
    * Calling Processing message which are in moved from Defered list to UE
    * main list - UE Defr List has the stored message which are received for
    * Defered Transaction  those are being processed here
    * */
   for(cnt = 0; cnt < (transCb->numDefMsgCount); cnt++)
   {
      /* process the deferred messages only if its not initiating
       * UE Context Release */
      if(transCb->state != WR_UMM_TRANS_RELEASE_CALL)
      {
         wrUmmPrcIncMsg(cellCb, ueCb, transCb->defList[cnt]);
      } /* not RELEASE_CALL */
      wrUmmRlsIncMsg(&(transCb->defList[cnt]));
   }
   WR_FREE(transCb, sizeof(*transCb));
}

/** @brief This function is used to add deffered transaction to
 *     ue Transaction list.
 *
 * @details
 *
 *     Function: wrUmmAddDefTransToUeTransLst
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  defTrans   : Deffered transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmAddDefTransToUeTransLst
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *defTransCb
 )
{
   WrUeCb                    *ueCb;

   TRC2(wrUmmAddDefTransToUeTransLst)

      ueCb = transCb->ueCb;
   cmLListDelFrm (&transCb->deferredTrans, &defTransCb->lnk);
   cmLListAdd2Tail(&ueCb->transLst, &defTransCb->lnk);

   RETVALUE(ROK);
}
/** @brief This function is used to send received Uu message to defer
 *     transaction List.
 *
 * @details
 *
 *     Function: wrUmmSndIncMsgToDefTrans
 *     
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction control block
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmSndIncMsgToDefTrans
(
 WrUmmTransCb                *transCb
 )
{
   U8                         ret = 0;
   CmLList                   *lnk;
   WrUmmTransCb              *defTransCb;

   TRC2(wrUmmSndIncMsgToDefTrans)

      lnk = transCb->deferredTrans.first;
   while(lnk != NULLP)
   {
      defTransCb = (WrUmmTransCb *)(lnk->node);
      lnk = lnk->next;
      ret =  wrUmmTransMsgPrcs[defTransCb->transTyp](transCb, defTransCb);
      /*check the ret value if ret value is "DEQUE_CSFB" release reestablishment
        transaction and add defered transaction into ue trans list and intiated
        it*/
      if(ret == WR_UMM_STOP_OLD_TRANSACTION)
      {
         wrUmmAddDefTransToUeTransLst(transCb, defTransCb);
         wrUmmStopTransPrcs[transCb->transTyp](transCb, defTransCb);
      }
      if(WR_UMM_RESUME_DEF_TRANSACTION == ret)
      {
         wrUmmAddDefTransToUeTransLst(transCb, defTransCb);
      }
   }
   RETVALUE(ROK);
}


/** @brief This function Process the transaction
 *
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Based on the transaction type in Transaction
 *           control block it invokes the function pointer
 *
 *
 * @param [in] transCb
 *
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
PRIVATE S16 wrUmmPrcTrans
(
 WrUmmTransCb                 *transCb
 )
{
   WrUeCb *ueCb      = NULL;
   U8     errorCause = 0;   
   wrUtlPrntIncTansType(transCb->transTyp, transCb->ueCb->crnti,
         transCb->msg->msgTyp, transCb->transId);
   wrUmmSndIncMsgToDefTrans(transCb);
   /* Fix for CR ccpu00135466 
    * If more than 1 Msg is processing a transaction
    * then the transCb/ueCb will not be removed until 
    * all the Msgs have finished their processing. */
   transCb->transRefCount++;
   wrUmmTransPrcs[transCb->transTyp](transCb);
   transCb->transRefCount--;
   errorCause = transCb->errorCause;   

   if(transCb->transRefCount > 0)
   {
      RETVALUE(ROK);
   }

   /* Depending on the state of the transaction either delete the UE or
      release the Call that is remove the UE from the lower layers or 
      release the transaction */
   if (transCb->state == WR_UMM_TRANS_DELETE_UE)
   {
      /* Release the UE CB */
      wrUmmRelUeCb(transCb->ueCb);
   }
   else if (transCb->state == WR_UMM_TRANS_RELEASE_CALL)
   {
      /* Release the ongoing transaction. 
         And release the UE context from all the layers. */
      ueCb = transCb->ueCb;
      wrUmmTransRel(transCb);
      /*Fix for ccpu00132236*/
      if((NULLP != ueCb->s1ConCb) && 
            (ueCb->s1ConCb->s1apConnState == WR_S1AP_CONNECTED))

      {
         /* If UE has a connection with MME then
            initiate UE Context release */
         wrUmmSndCtxtRelReq(ueCb->crnti, ueCb->cellId,errorCause);
      }
      else
      {
         /* If UE has not a connection with MME then 
            initiate local Context UE release */
         if(ueCb->causeTauRequiredSet)
         {
            wrUmmPrcLclCtxtRel(ueCb,NhuRlsCauseloadBalancingTAUrequiredEnum);
         }
         else
         {
            wrUmmPrcLclCtxtRel(ueCb,NhuRlsCauseotherEnum);
         }
      }
   }
   else if (transCb->state == WR_UMM_TRANS_DONE)
   {
      /* Release the transaction */
      wrUmmTransRel(transCb);
   }
   RETVALUE(ROK);
}

/** @brief This function is used to add Existing transaction to
 *     deffered transaction of incoming transaction.
 *
 * @details
 *
 *     Function: wrUmmAddExistTransToDefTransLst
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */

PRIVATE S16 wrUmmAddExistTransToDefTransLst
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *incTransCb
 )
{
   WrUeCb                    *ueCb;

   TRC2(wrUmmAddExistTransToDefTransLst)

      ueCb = transCb->ueCb;
   cmLListDelFrm (&ueCb->transLst, &transCb->lnk);
   cmLListAdd2Tail(&incTransCb->deferredTrans, &transCb->lnk);

   RETVALUE(ROK);
}

/* Add the incoming transaction to the deferred transaction list */
PRIVATE S16 wrUmmAddNewTransToDefTransLst
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *incTransCb
 )
{
   TRC2(wrUmmAddNewTransToDefTransLst);

   incTransCb->state = WR_UMM_TRANS_DEFFERED;
   cmLListAdd2Tail(&transCb->deferredTrans, &incTransCb->lnk);

   RETVALUE(WR_UMM_TRANS_OK);
}


/** @brief This function is used to insert incoming transaction into 
 * Ue transaction List.
 *
 * @details
 *
 *     Function: wrUmmInsTrans
 *
 *         Processing steps:
 *
 * @param[in]  ueCb    : pointer to Ue control block
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInsTrans
(
 WrUeCb                       *ueCb,
 WrUmmTransCb                 *incTrans
 )
{
   WrUmmTransCb              *trans;
   CmLList                   *node;
   S16                       ret = WR_UMM_TRANS_OK;
   U8                        transId; 
   U8                        cnt;

   node = ueCb->transLst.first;
   incTrans->lnk.node = (PTR)incTrans;
   while (node != NULLP)
   {
      trans = (WrUmmTransCb *)(node->node);
      node  = node->next;
      if((trans != NULL) && (trans->transTyp < WR_UMM_MAX_TRANS_TYPE)) 
      {
         /*Call DefTransList if any new message received for Ongoing 
          * Transaction needs imidiate attention and process new 
          * message after close the ongoing message once completed  */
         ret = wrUmmNewTransPrcrs[trans->transTyp](trans, incTrans);
         /* Fix for CR ccpu00135466 */
         if ((trans->state == WR_UMM_TRANS_DONE) && (trans->transRefCount == 0))
         {
            /*
             * moving Defered messages in trans to the high priorty incoming transaction
             * */
            for(cnt = 0; cnt < (trans->numDefMsgCount); cnt++)
            {
               if(trans->numDefMsgCount < WR_UMM_DEF_LIST)
               {
                  WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(incTrans, trans->defList[cnt]);       
               }
               wrUmmRlsIncMsg(&(trans->defList[cnt]));
            }
            trans->numDefMsgCount = 0;
            wrUmmTransRel(trans);
         }

         if(ret == WR_UMM_TRANS_DEFER_ONGOING)
         {
            /*Fix for ccpu00123852 */
            ret = wrUmmAddExistTransToDefTransLst(trans, incTrans);
         }
         else if(ret == WR_UMM_TRANS_DEFER_INCOMING)
         {
            ret = wrUmmAddNewTransToDefTransLst(trans, incTrans);
            RETVALUE(ret);
         }
         else 
         {
            if (ret == WR_UMM_TRANS_FAIL)
            {
               transId = ((incTrans->transId & 0x00000ff0) >> 4);
               incTrans->ueCb->transIdInUse[transId] = FALSE;
               /* We should instead generate a failure event towards the     */
               /* transaction. Failure event should respond appropriately to */
               /* the other nodes involved in the transaction   TODO         */
               WR_FREE(incTrans, sizeof(*incTrans));
               /* The return value could indicate that the transaction is    */
               /* deferred by another transaction. Other transaction will    */
               /* release and activate the new transaction at an appropriately*/
               RETVALUE(ret);
            }
         }
      }
      else
      {

         /* Entering here suggests corruption. There is no certain remedy to    */
         /* fix this scenario. The best we can do is to log this situation for  */
         /* future reference and continue further by adding the incoming        */
         /* transaction to the list                                             */
         RLOG0(L_ERROR, "!!!TransCb NULL or number of transType is exceeding");
      }
   }

   cmLListAdd2Tail(&ueCb->transLst, &incTrans->lnk);

   RETVALUE(ret);
}


/** @brief This function Returns the Free TransId
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - return the FreeTransID
 * @param [in] ueCb
 * @return  Returns the transaction ID
 */ 

PRIVATE U8 wrUmmGetFreeTransId
(
 WrUeCb                        *ueCb
 )
{
   U8                          idx = 0;

   idx = ueCb->avlTransId;
   do{
      if (ueCb->transIdInUse[idx] == FALSE)
      {
         ueCb->transIdInUse[idx] = TRUE;
         ueCb->avlTransId          = (idx + 1) % WR_UMM_MAX_TRANSID;

         RETVALUE(idx);
      }
      idx = (idx + 1) % WR_UMM_MAX_TRANSID;
   }while(idx != ueCb->avlTransId);

   RLOG_ARG0(L_ERROR, DBG_CRNTI, ueCb->crnti, 
         "wrUmmGetFreeTransId ReachedMax(255) TransactionIds");
   RETVALUE(RFAILED);

}


/** @brief This function adds the new transaction
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Allocate the transaction
 *         - Add the trasaction in the linked list
 *           of the UE control block
 * @param [in] cellCb
 * @param [in] ueCb
 * @param [in] msg
 * @return  Returns the transaction control block
 */ 

PRIVATE WrUmmTransCb *wrUmmAddNewTrans
(
 WrUmmCellCb                  *cellCb,
 WrUeCb                       *ueCb,
 WrUmmIncMsg                  *msg
 )
{
   U32                       transTyp;
   WrUmmTransCb              *transCb;

   if (wrUmmGetTransTypFromMsg(ueCb, msg, &transTyp) != ROK)
   {
      /*127802*/
      RLOG0(L_ERROR, "Failed to identify transaction type");
      RETVALUE(NULLP);
   }

   WR_ALLOC(&transCb, sizeof(*transCb));
   if (transCb == NULLP)
   {
      /*127802*/
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(NULLP);
   }
   wrUmmNumTransCnt ++;

   /* The last 4 bits are set to zero to allow subtransactions            */
   transCb->ueCb      = ueCb;
   transCb->transTyp  = transTyp;
   transCb->msg       = msg;
   transCb->state     = WR_UMM_TRANS_INUSE;
   ueCb->lastTransId  = wrUmmGetFreeTransId(ueCb);
   WR_SET_TRANS_ID((transCb->transId), (cellCb->cellId), 
         (ueCb->ueIdx),(ueCb->lastTransId)); 
   cmLListInit(&transCb->tmrLst);
   cmInitTimers(&(transCb->timer.tmr), WR_MAX_TMR_PER_UE);   
   cmInitTimers(&(transCb->timerGrd.tmr), WR_MAX_TMR_PER_UE);   

   RLOG_ARG2(L_INFO, DBG_CRNTI, ueCb->crnti, 
         "Adding new Transaction transId: %d TransType: %d",
         transCb->transId, transCb->transTyp);

   /* This function is used to insert incoming transaction into */
   /*   Ue transaction List */
   if (wrUmmInsTrans(ueCb, transCb) != WR_UMM_TRANS_OK)
   {
      RETVALUE(NULLP);
   }

   RETVALUE(transCb);
}

/** @brief This function locates the transaction control block in use for the
 *         associated transaction.
 *
 * @details
 *
 *     Function: wrUmmCheckIncMsgForDef 
 *
 *         Processing steps:
 *         - If the message is a transaction intiating message, a new transaction
 *            is created. If not, existing transaction is fetched. 
 *
 *
 * @param [in] ueCb
 * @param [in] transCb
 *
 * @return FAIL if transaction could not be located
 */ 
PRIVATE U16 wrUmmCheckIncMsgForDef
(
 WrUeCb                       *ueCb,
 U32                          transId,
 CmLListCp                    *mainTransLst,
 WrUmmTransCb                 **transCb
 )
{
   CmLList                   *mainLnk;   
   CmLList                   *defLnk;
   WrUmmTransCb              *mainTransCb;
   WrUmmTransCb              *defTransCb;

   mainLnk = mainTransLst->first;

   while(mainLnk != NULLP)
   {
      mainTransCb = (WrUmmTransCb *)(mainLnk->node);

      defLnk = mainTransCb->deferredTrans.first;
      while(defLnk != NULLP)
      {
         defTransCb = (WrUmmTransCb *)(defLnk->node);
         /* Out of 32 bits in transaction id, first 28 identify the major    */
         /* transaction. The last four bits are available for subtransactions*/
         if ((defTransCb != NULLP) && 
               ((defTransCb->transId & WR_TRANS_MASK) == (transId & WR_TRANS_MASK)))
         {
            *transCb = mainTransCb;
            RETVALUE(ROK);
         }
         defLnk = defLnk->next;
      } /* End of one transCb's deferred lst */
      mainLnk = mainLnk->next;
   } /* End of UeCb main transLst */

   RETVALUE(RFAILED);
}

/** @brief This function locates the transaction control block in use for the
 *         associated transaction.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - If the message is a transaction intiating message, a new transaction
 *            is created. If not, existing transaction is fetched. 
 *
 *
 * @param [in] cellCb
 * @param [in] ueCb
 * @param [in] msg
 *
 * @return NULLP if transaction could not be located
 */ 
PRIVATE WrUmmTransCb *wrUmmGetTransCbByMsg
(
 WrUmmCellCb                  *cellCb,
 WrUeCb                       *ueCb,
 WrUmmIncMsg                  *msg
 )
{
   U32                       transId = 0;
   WrUmmTransCb              *transCb;
   WrUmmTransCb              *mainTransCb = NULLP;

   if (wrUmmGetTransIdFromMsg(ueCb, msg, &transId) != ROK)
   {
      RLOG1(L_ERROR, "Transaction Id could not be retrieved from msgType [%d]",
            msg->msgTyp);
      RETVALUE(NULLP);
   }
   if (transId == WR_CREATE_NEW_TRANS)
   {
      transCb = wrUmmAddNewTrans(cellCb, ueCb, msg);
      RETVALUE(transCb);
   }
   if ((transCb = wrUmmGetTransCbById(ueCb, transId)) == NULLP)
   {
      /*Naveen_def
       * Search if this message is for any deferred transaction.
       * If yes then increase the reference count and store the message in 
       * ueCb->defQ[] and increase the ueCb->NumDefMsgCount */
      if(ROK == wrUmmCheckIncMsgForDef(ueCb, transId, &ueCb->transLst, &mainTransCb))
      {
         WR_UMM_CPY_DEFR_MSG_TO_TRANSCB (mainTransCb, msg);       
      }
      else
      {
         RLOG1(L_ERROR, "Transaction Cb could not be retrieved from transID [%d]",
               transId);
         RETVALUE(NULLP);
      }
   }
   RETVALUE(transCb);
}
/** @brief This function is responsible for locating the transaction contol block
 *         for the received message from the UE control block.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Process the incoming message by getting the corresponding
 *           transaction
 *
 *
 * @param [in] cellCb
 * @param [in] ueCb
 * @param [in] msg
 *
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcIncMsg
(
 WrUmmCellCb                  *cellCb,
 WrUeCb                       *ueCb,
 WrUmmIncMsg                  *msg
 )
{
   WrUmmTransCb              *transCb;

   if ((transCb = wrUmmGetTransCbByMsg(cellCb, ueCb, msg)) == NULLP)
   {
      /* ccpu00129424 Start */
      if(msg->msgTyp == WR_UMM_INTERNAL_MSG)
      {
         RLOG_ARG1(L_WARNING, DBG_CRNTI, ueCb->crnti,
               "Internal Transaction CB could not be created."
               " Internal MsgType is [%d]", msg->u.internalMsg->intMsgType);
      }
      else
      {
         RLOG_ARG1(L_WARNING, DBG_CRNTI, ueCb->crnti,
               "Transaction CB could not be retrieved. msgType [%d]",
               msg->msgTyp);
      }
      /* ccpu00129424 End */
      RETVALUE(RFAILED);
   }

   transCb->msg  = msg;
   if(transCb->state != WR_UMM_TRANS_DEFFERED)
   {
      wrUmmPrcTrans(transCb);
   }
   RETVALUE(ROK);
}
/* HO_REEST */
/** @brief This function stores the UE information in handover UE list.
 * @details
 *
 *     Function: wrUmmGetHoUeCtxt 
 *
 *         Processing steps:
 *         - Get the Cell control Block using cellId
 *         - Run throuhg all the ueCB to find out valid hoReestInfo 
 *
 *
 * @param [in] ueCb
 * @param [in] pci
 * @param [in] crnti
 * @param [in] cellId
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
/** @brief This function returns the pointer to UE control block based on cell
 *         source phy cell identity and CRNTI.
 -# Success : ROK
 -# Failure : RFAILED
 */
PRIVATE S16 wrUmmGetHoUeCtxt
(
 WrUeCb     **ueCb,
 WrCrnti     crnti,
 U16           pci,
 WrCellId    cellId
 )
{
   WrUmmCellCb               *cellCb;
   U32                       idx;

   *ueCb = NULLP;
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < cellCb->maxUesPerCell; idx++)
   {
      if (cellCb->ueCbs[idx] != NULLP)
      {
         if ((cellCb->ueCbs[idx]->hoReestInfo.srcUeCrnti == crnti) &&
               (cellCb->ueCbs[idx]->hoReestInfo.pci == pci))
         {
            *ueCb = cellCb->ueCbs[idx];
            (*ueCb)->hoReestInfo.isTooLateHO = TRUE;
            RETVALUE(ROK);
         }
      }
   }
   RETVALUE(RFAILED);

}
/** @brief  This function is called by IFM module to pass the received UU PDU 
 *         
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         It receives procedure initiaing messages from UE. It     
 *           creates an incoming message that is common for all the different   
 *           interfaces and invokes wrUmmPrcIncMsg() for further processing.   
 *
 *
 * @param  NhuDatIndSdus    *uuDatInd
 *
 * @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcUuDatInd
(
 NhuDatIndSdus                *uuDatInd
 )
{
   WrCellId                  cellId;
   WrCrnti                   crnti;
   WrUmmIncMsg               *msg = NULLP;
   WrUeCb                    *ueCb = NULLP;
   NhuUlCcchMsg              *ulCcchPdu = NULLP;
   U8                        messageType;
   WrUmmCellCb               *cellCb;
   U32                       phyCellId;
   U32                       reestabCause; 

   if(uuDatInd == NULL)
   {
      RETVALUE(ROK);
   }

   cellId = uuDatInd->hdr.cellId;
   crnti  = uuDatInd->hdr.ueId;

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(NULL == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL ");
      RETVALUE(ROK);
   }

   if(WR_CELL_STATE_RESET == cellCb->cellState)
   {
      RLOG_ARG1(L_WARNING, DBG_CRNTI, crnti,"Cell In Reset State Dropping UuDatInd"
            "Cell ID %d",cellCb->cellId);
      RETVALUE(ROK);
   }

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(uuDatInd);
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_UU_IND_PDU;
   msg->u.uuIndPdu = uuDatInd;

   if(uuDatInd->sdu.msgCategory == NHU_MSG_ULCCCH)
   {   
      /* Get the  then allocate the UE and add it into CellCb*/
      ulCcchPdu = &uuDatInd->sdu.m.ulCcchMsg;

      messageType = ulCcchPdu->ccchMsg.message.val.c1.choice.val; 
      if(messageType == C1_RRCCONRQST)
      {
         crnti  = ulCcchPdu->tmpCrnti;
   

         /*check if UeCb  Exists that means this is a duplicate Msg3 Drop it */
         wrUmmGetUe(&ueCb, cellId, crnti);
         if(NULL != ueCb)
         {
            /*127802*/
            RLOG_ARG1(L_WARNING, DBG_CRNTI, crnti, "Duplicate RRCConnectionRequest"
                  "Message received cellId = %lu dropping it",(U32)cellCb->cellId);
            wrUmmRlsIncMsg(&msg);
            RETVALUE(ROK);
         }
         RLOG_ARG1(L_EVENT, DBG_CRNTI, crnti, "[OTA] Received RRCConnectionRequest "
               "message with establishmentCause [%ld]", ulCcchPdu->ccchMsg.message.
               val.c1.val.rrcConRqst.criticalExtns.val.rrcConRqst_r8.establishmentCause.val);
         /*Insert into the CellCb List*/
         ueCb = wrUmmAddUe(cellId, crnti);
         if(NULLP != ueCb)
         {
            wrUmmMeasUeCbInit(ueCb);
            ueCb->numTtiTicks = numTtiTicks;
         }
      }
      else if(messageType == C1_RRCCONREESTBRQST)
      {
         /*Get the UeCb as it must already exist
           1) if Re-establsih at Same Source enodeB - It must be existing.
           2) if Re-establsih at Target eNodeB - Must have been allocated during 
           Prep for handover.
           */
         phyCellId = (ulCcchPdu->ccchMsg.message.val.c1.val.rrcConReestbRqst.
               criticalExtns.val.rrcConReestbRqst_r8.
               ue_Identity.physCellId.val);

         if(ulCcchPdu->ccchMsg.message.val.c1.val.rrcConReestbRqst.
            criticalExtns.val.rrcConReestbRqst_r8.ue_Identity.c_RNTI.pres != PRSNT_NODEF)
         {
            RLOG0(L_ERROR, "UE Identity is NULL in ReEstablishment message");
            wrUmmRlsIncMsg(&msg);
            RETVALUE(ROK);
         }
         crnti = ((ulCcchPdu->ccchMsg.message.val.c1.val.rrcConReestbRqst.
                  criticalExtns.val.rrcConReestbRqst_r8.
                  ue_Identity.c_RNTI.val[0] << 8)    |
               (ulCcchPdu->ccchMsg.message.val.c1.val.rrcConReestbRqst.
                criticalExtns.val.rrcConReestbRqst_r8.
                ue_Identity.c_RNTI.val[1]));

         RLOG_ARG3(L_EVENT, DBG_CELLID, phyCellId, "[OTA] Received RRCConnection"
               "ReestablishmentRequest message newCrnti[%u], oldCrnti[%u]"
               " with reestablishmentCause [%ld]", ulCcchPdu->tmpCrnti, crnti,
               ulCcchPdu->ccchMsg.message.val.c1.val.rrcConReestbRqst.
               criticalExtns.val.rrcConReestbRqst_r8.reestablishmentCause.val);

         if ( phyCellId == wrEmmGetPhyCellId(cellId))
         {
            wrUmmGetReestUe(&ueCb, cellId, crnti);
         }
         /* check if this UE is being handed over to this cell
          * Too late HO case, UE sends RRC Reestablishment to Tgt with CRNTI & PCI of serving cell
          * if HO Preparation Info received at this cell then this may have the context
          * in this case accept the RRC Connection Reestablishment 
          */ 
         else
         {
            /* crnti of the ue which is with serving cell,PCI of serving cell */
            wrUmmGetHoUeCtxt(&ueCb,crnti,phyCellId,cellId);
            /* ueCb->crnti points to the crnti of this cell,so
             * further processing happens with this crnti
             */
         }  

         if(ueCb == NULLP)
         {
            NhuDatRspSdus             *reestabPdu;


            reestabCause = 
               ulCcchPdu->ccchMsg.message.val.c1.val.rrcConReestbRqst.
               criticalExtns.val.rrcConReestbRqst_r8.reestablishmentCause.val; 

            /*Increment KPI for Attempted RRC connection re-establishments*/
            WR_INC_KPI_FUNC1(wrIncKpiFrReestbAtmptdPrEtbCaus,reestabCause); 



            /* Send RRC REESTABLISHMENT REJECT and release the UE CB by invoking   */
            /* local context release                                               */
            WR_ALLOCEVNT(&reestabPdu, sizeof(NhuDatRspSdus));
            if (reestabPdu == NULLP)
            {
               wrUmmRlsIncMsg(&msg);
               RETVALUE(ROK);
            }
            reestabPdu->sdu.m.dlCcchMsg.rbId = WR_CCCH_DL_ID;
            reestabPdu->sdu.m.dlCcchMsg.tmpCrnti = ulCcchPdu->tmpCrnti;
            wrUmmFillRrcEvntHdr(&(reestabPdu->hdr), cellId, 
                  0, 
                  WR_UMM_RRC_REESTAB_TRANS);

            wrUmmRrcBldReestabReject(reestabPdu);
            RLOG_ARG0(L_EVENT, DBG_CRNTI, ulCcchPdu->tmpCrnti, "[OTA] Sending "
                  "RRCConnectionReestablishmentReject"); 
            RLOG_ARG0(L_DEBUG, DBG_CRNTI, ulCcchPdu->tmpCrnti, "No Ue Context in Cell");
            if (wrIfmRrcSndCcchRspToUe(reestabPdu) != ROK)
            {
               RLOG0(L_ERROR, "wrIfmRrcSndCcchReqToUe failed");
               WR_FREEEVNT(reestabPdu);
               wrUmmRlsIncMsg(&msg);
               RETVALUE(ROK);
            }

            /*Increment KPI for Failed RRC connection re-establishments*/
            WR_INC_KPI_FUNC1(wrIncKpiFrReestbFaildPrEtabCaus,reestabCause); 
            wrUmmRlsIncMsg(&msg);
            RETVALUE(ROK);
         }
      }
   }
   else
   {   
      wrUmmGetUe(&ueCb, cellId, crnti);
   }   

   if (ueCb == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CRNTI, (U32)crnti, "Error in getting UECB cellId = %lu",
         (U32)cellCb->cellId);
      wrUmmRlsIncMsg(&msg);
      /* Statistics print */
      wrUmmEnbStats();
      RETVALUE(ROK);
   }
   
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief  This function is called by IFM module to pass the received S1 PDU 
 *         
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         It receives procedure initiaing messages from UE. It     
 *           creates an incoming message that is common for all the different   
 *           interfaces and invokes wrUmmPrcIncMsg() for further processing.   
 *
 *
 * @param  SztDatEvntInd  *s1DatInd
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcS1DatInd
(
SztDatEvntInd                *s1DatInd
)
{
   WrCellId                  cellId;
   U8                        ueIdx = 0;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;

   WR_GET_CELLID_FRM_SUCONID(s1DatInd->suConnId, cellId, ueIdx);

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(s1DatInd->pdu);
      RETVALUE(ROK);
   }
   msg->refCnt = 1;

   msg->msgTyp = WR_UMM_S1_DAT_IND;
   /* s1Pdu are being copied the adress of
    * stack variables. Hence would be invalid during releasing.*/
   WR_ALLOC(&msg->u.s1Pdu, sizeof(*(msg->u.s1Pdu)));
   if (msg->u.s1Pdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(s1DatInd->pdu);
     WR_FREE(msg, sizeof(*msg));
      RETVALUE(RFAILED);
   }
   msg->u.s1Pdu->suConnId = s1DatInd->suConnId;
   msg->u.s1Pdu->pdu = s1DatInd->pdu;

   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIdx);
   if (ueCb == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "ueIdx = %u", ueIdx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELLCB ");
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }

   /*Reset INACTIVITY TIMER by setting dataRcvd flag to TRUE*/
   wrIfmDamSetDataRcvdFlag(ueCb->cellId, ueCb->crnti);


   wrUmmPrcIncMsg(cellCb, ueCb, msg);

   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* This function is called by IFM module to pass the received S1 PDU      */
/*                                                                        */
/* This function is receives procedure initiaing messages from UE. It     */
/*    creates an incoming message that is common for all the different    */
/*    interfaces and invokes wrUmmPrcIncMsg() for further processing.     */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmPrcS1ConInd
(
SztConReq                   *conReq,
U32                         *cause
)
{
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb = NULLP;
   WrCrnti                   crnti;
   WrUmmCellCb               *cellCb;
   /* U32                       cellId; */
   WrMmeCb                   *mmeCb;
   S1apPdu                   *pdu;

   mmeCb = wrEmmMmeGetMmeByPeerId(conReq->peerId.val);
   if(mmeCb == NULLP)
   {
      /* discard the connection indication */
      RLOG1(L_ERROR, "S1 ConInd from Invalid PeerId [%ld]", conReq->peerId.val);
      *cause = SztCauseRadioNwho_failure_in_target_EPC_eNB_or_target_systemEnum;     
      RETVALUE(RFAILED);
   }
   cellCb = &wrUeDb.cells[0];
   pdu = conReq->pdu;
   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      *cause = SztCauseRadioNwho_failure_in_target_EPC_eNB_or_target_systemEnum;
      RETVALUE(RFAILED);
   }
   msg->refCnt  = 1;
   msg->msgTyp  = WR_UMM_S1_CON_PDU;
   WR_ALLOC(&msg->u.s1Pdu,sizeof(SztDatEvntInd));
   if (msg->u.s1Pdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE(msg, sizeof(*msg));
      *cause = SztCauseRadioNwho_failure_in_target_EPC_eNB_or_target_systemEnum;
      RETVALUE(RFAILED);
   }
   msg->u.s1Pdu->pdu = pdu;
   /* If cannot find RNTI please reject */
   if(ROK == wrUtlGetDedCrnti(&crnti, cellCb->cellId))
   {
     ueCb = wrUmmAddUe(cellCb->cellId, crnti);
     if (ueCb != NULLP)
     {
       wrUmmMeasUeCbInit(ueCb);
     }
   }
   /* Assign spConnId into UeCb */
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU
       * */
      RLOG_ARG1(L_ERROR, DBG_CRNTI, crnti, "Error in getting UECB cellId [%lu]",
         (U32)cellCb->cellId);
      WR_FREE(msg->u.s1Pdu,sizeof(SztDatEvntInd));
      WR_FREE(msg,sizeof(*msg));
      *cause = SztCauseRadioNwho_failure_in_target_EPC_eNB_or_target_systemEnum;              
      RETVALUE(RFAILED);
   }

   /* sri - HOFIX - add ue to MME */
   ueCb->mmeId = mmeCb->mmeId;
   wrUmmAddUeToMme(ueCb, ueCb->mmeId);

   /* Allocate s1ap Context.*/
   WR_ALLOC(&ueCb->s1ConCb, sizeof(WrS1ConCb));
   if (NULLP == ueCb->s1ConCb) 
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE(msg->u.s1Pdu,sizeof(SztDatEvntInd));
      WR_FREE(msg,sizeof(*msg));
      *cause = SztCauseRadioNwho_failure_in_target_EPC_eNB_or_target_systemEnum;              
      RETVALUE(RFAILED);
   }/* End of the if condition */
   /* Store spConnId which is given by s1ap layer.*/
   ueCb->s1ConCb->spConnId = conReq->u.spConnId;
   /* update enb_ue_s1ap_id - in case of conInd both values are same */
   ueCb->s1ConCb->enb_ue_s1ap_id = conReq->u.spConnId;
   /*ccpu00129650 Initialize the HO UeCb */
   wrUmmTgtHoInitUeCb(ueCb);
   /* WR_UMM_GET_CELLCB(cellCb, cellId); */
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/*FIX for ccpu00127073*/

/** @brief  This function is called to retrieve New_Ue_X2AP_Id from 
 *                                                 Old_Ue_X2AP_Id 
 *         
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *             The function wrUtlX2GetCztIE() is called to retrieve the IE from        
 *          which oldUeX2apId is obtained. Using the Old Id the Lst is traversed
 *          to retrieve the NewUeX2apId, using which UeCb can be accessed.
 *
 *
 * @param[in] cztInd - X2AP PDU
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrUmmGetNewX2IdFromOldX2Id
(
CztX2AP_PDU                  *pdu,
U32                          *ueX2apId
)
{
     TknU8                     *ie;
     U8                        i;
     U32                       oldUeX2apId =0;
     WrCellId                  cellId;
     WrUmmCellCb               *cellCb;

   wrUtlX2GetCztIE(pdu, Cztid_Old_eNB_UE_X2AP_ID, (void **)&ie);
   if(ie == NULLP)
   {
      RLOG0(L_ERROR, "Could not retreive OLD UE X2AP ID not found");
      RETVALUE(RFAILED);
   }
   oldUeX2apId = ((CztUE_X2AP_ID*)ie)->val;

   /* TODO: need logic to select cellId */
   cellId = 1;

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   for(i = 0; i < cellCb->maxUesPerCell; i++)     
   {
      if(cellCb->ueCbs[i] != NULLP)
      {
         if(cellCb->ueCbs[i]->x2ConCb.oldEnbUeX2apId == oldUeX2apId)
         {
            (*ueX2apId) = cellCb->ueCbs[i]->x2ConCb.newEnbUeX2apId;
            break;
         }
      }
   }
   RETVALUE(ROK);
}
/* Fix for ccpu00138192 */
/** @brief  This function is used to get transType 
 *                   
 *         
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         Fetch the transType.
 *
 *
 * @param[in] ueCb - UeCB
 * @param[out] trasnType - Transaction Type
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrUmmGetHoTransType 
(
WrUeCb                    *ueCb,
U8                        *transType
)
{
   WrUmmTransCb             *hoTransCb = NULLP;
   if ( !ueCb->hoTransId )
   {
      RLOG0(L_ERROR, "ueCb->hoTransId = 0!!!");
      RETVALUE(RFAILED);
   }
   hoTransCb = wrUmmGetHoTransCb(ueCb);
   if ( hoTransCb == NULLP )
   {
      RLOG0(L_ERROR, "hoTransCb = NULLP!!!");
      RETVALUE(RFAILED);
   }
   *transType = hoTransCb->transTyp;
    RETVALUE(ROK);
}

/* Fix for ccpu00138192 */
/** @brief  This function is called to retrieve X2AP id from error indication 
 *                   
 *         
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         Fetch Old X2AP ID if error Indication message message 
 *         received at Source eNB or fetch New X2AP id, if received at
 *         target eNB. 
 *
 *
 * @param[in] cztInd - X2AP PDU
 * @param[out] ueX2apId - X2AP ID
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrUmmGetX2IdFromErrInd 
(
CztX2AP_PDU                  *pdu,
U32                           peerId,
U32                          *ueX2apId
)
{
   TknU8                     *oldX2apIdIe;
   TknU8                     *newX2apIdIe;
   U8                        idx;
   WrCellId                  cellId = 1; /* Hardcoded - TODO - add a logic */
   U32                       oldUeX2apId = 0xFFFF; /* Invalid X2AP id */
   U32                       newUeX2apId = 0xFFFF; 
   WrUmmCellCb              *cellCb;
   S16                       ret = RFAILED;
   U8                        transType = 0;


   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(ret);
   }

   /* Fetch new x2ap Id IE */ 
   wrUtlX2GetCztIE(pdu, Cztid_New_eNB_UE_X2AP_ID, (Void **)&newX2apIdIe);
   if ( newX2apIdIe != NULLP )
   {
      newUeX2apId = ((CztUE_X2AP_ID*)newX2apIdIe)->val;
   }
   if ( newUeX2apId != 0xFFFF )
   {
      for(idx = 0; idx < cellCb->maxUesPerCell; idx++)
      {
         if((cellCb->ueCbs[idx] != NULLP) &&
               (cellCb->ueCbs[idx]->x2ConCb.newEnbUeX2apId == newUeX2apId) &&
               (cellCb->ueCbs[idx]->x2ConCb.peerId == peerId))
         {
            if ( ROK == wrUmmGetHoTransType(cellCb->ueCbs[idx], &transType))
            {
               if ( WR_UMM_X2_SRC_HO_TRANS == transType )
               {
                  (*ueX2apId) = cellCb->ueCbs[idx]->x2ConCb.oldEnbUeX2apId;
                  ret = ROK;
               }
               else if (  WR_UMM_X2_TGT_HO_TRANS == transType ) 
               {
                  (*ueX2apId) = cellCb->ueCbs[idx]->x2ConCb.newEnbUeX2apId;
                  ret = ROK;
               }
            }
            else
            {
               ret = RFAILED;
            }
            RETVALUE(ret);
         }
      }
   }

   /* Fetch old x2ap Id IE */ 
   wrUtlX2GetCztIE(pdu, Cztid_Old_eNB_UE_X2AP_ID, (Void **)&oldX2apIdIe);
   if ( oldX2apIdIe != NULLP )
   {
      oldUeX2apId = ((CztUE_X2AP_ID*)oldX2apIdIe)->val;
   }
   if ( oldUeX2apId != 0xFFFF )
   {
      for(idx = 0; idx < cellCb->maxUesPerCell; idx++)
      {
         if((cellCb->ueCbs[idx] != NULLP) &&
               (cellCb->ueCbs[idx]->x2ConCb.oldEnbUeX2apId == oldUeX2apId) &&
               (cellCb->ueCbs[idx]->x2ConCb.peerId == peerId))
         {
            if ( ROK == wrUmmGetHoTransType(cellCb->ueCbs[idx], &transType))
            {
               if ( WR_UMM_X2_SRC_HO_TRANS == transType )
               {
                  (*ueX2apId) = cellCb->ueCbs[idx]->x2ConCb.oldEnbUeX2apId;
                  ret = ROK;
               }
               else if (  WR_UMM_X2_TGT_HO_TRANS == transType ) 
               {
                  (*ueX2apId) = cellCb->ueCbs[idx]->x2ConCb.newEnbUeX2apId;
                  ret = ROK;
               }
            }
            else
            {
               ret = RFAILED;
            }
            RETVALUE(ret);
         }
      }
   }
   RETVALUE(ret);
}

/*FIX for ccpu00127073*/

/* ccpu00130220 */
/** @brief  This function is called by IFM module to pass the X2AP
 *          status indication to corresponding transaction.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - derive the UE IDX from OldUeX2apID
 *         - fetch UE CB and CELL CB
 *         - invoke PrcIncMsg
 *
 * @param[in] cztInd - X2AP PDU
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmX2TmrPrcStaInd
(
CztStaInd                    *cztStaInd
)
{
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;
   WrCellId                  cellId;
   U8                        ueIdx;
   U16                       UeX2apId = cztStaInd->oldUEX2APId;

   cellId = (UeX2apId & 0x0000ff00) >> 8;
   ueIdx  = (UeX2apId & 0x000000ff);

   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIdx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CRNTI,(U32)ueIdx, "Error in getting UECB from ueIdx, cellId "
         "[%lu]", (U32)cellId);
      RETVALUE(ROK);
   }

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }

   msg->refCnt  = 1;
   msg->msgTyp  = WR_UMM_X2_STA_IND;
   msg->u.cztStaInd = cztStaInd;

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);
   RETVALUE(ROK);
}
/* ccpu00130220 */

/** @brief  This function is called by IFM module to pass the received X2 PDU 
 *         
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         It receives procedure initiaing messages from UE. It     
 *           creates an incoming message that is common for all the different   
 *           interfaces and invokes wrUmmPrcIncMsg() for further processing.   
 *
 *
 * @param[in] cztInd - X2AP PDU
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcX2BmpInd
(
CztEvnt                      *cztInd
)
{
   WrCellId                  cellId = 0;
   WrCrnti                   crnti = 0;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;
   S16                       msgIdx;
   TknU8                     *ie;
   CztECGI                   *ecgi;
   U32                       ueX2apId = 0;
   U8                        ueIdx;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(cztInd);
      RETVALUE(ROK);
   }
   msg->refCnt  = 1;
   msg->msgTyp  = WR_UMM_X2_BMP_PDU;
   msg->u.x2Pdu = cztInd;

   msgIdx = wrUtlX2GetMsgIdx (&cztInd->pdu);
   if(CZT_MSG_HO_REQ == msgIdx)
   {
      wrUtlX2GetCztIE(&cztInd->pdu, Cztid_TgetCell_ID, (void **)&ie);
      if(ie == NULLP)
      {
         RLOG0(L_ERROR, "Target CellId not found");
         RETVALUE(RFAILED);
      }
      ecgi = (CztECGI*)ie;
      wrUtlGetU32FrmBStr32((U32*) &cellId, &ecgi->eUTRANcellIdentifier);
      /* sps - HOFIX - for testing */
      cellId = 1;
      WR_UMM_GET_CELLCB(cellCb, cellId); 
      if (cellCb == NULLP) 
      { 
         /* Log the error through an error debug and release the PDU          */ 
         RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB"); 
         wrUmmRlsIncMsg(&msg); 
         RETVALUE(RFAILED); 
      } 

      wrUtlGetDedCrnti(&crnti, cellId);
      ueCb = wrUmmAddUe(cellId, crnti);
      if (ueCb != NULLP)
      {
         wrUmmMeasUeCbInit(ueCb);
         /*ccpu00129650: Init UE Control block*/
         wrUmmX2TgtHoInitUeCb(ueCb);
      }
   }
   else
   {
      if ( msgIdx != CZT_MSG_ERR_IND )
      {
         wrUtlX2GetX2UeId(msgIdx, &cztInd->pdu, &ueX2apId);

         /*FIX for ccpu00127073*/
         if((ueX2apId == 0) && (msgIdx != CZT_MSG_HO_CANCEL))
         {
            RLOG0(L_ERROR, "UE ID not found");
            wrUmmRlsIncMsg(&msg);
            RETVALUE(ROK);
         }
         if((ueX2apId == 0) && (msgIdx == CZT_MSG_HO_CANCEL))
         {
            wrUmmGetNewX2IdFromOldX2Id(&cztInd->pdu, &ueX2apId);
         }
      }
      /* Fix for ccpu00138192 */
      else 
      {
         if ( ROK != wrUmmGetX2IdFromErrInd(&cztInd->pdu, cztInd->peerId, &ueX2apId))
         {
            RLOG0(L_ERROR, "UE ID not found");
            wrUmmRlsIncMsg(&msg);
            RETVALUE(RFAILED);
         }
      }
      /*FIX for ccpu00127073*/
      cellId = (ueX2apId & 0x0000ff00) >> 8;
      ueIdx  = (ueX2apId & 0x000000ff);
      WR_UMM_GET_CELLCB(cellCb, cellId); 
      if (cellCb == NULLP) 
      { 
         /* Log the error through an error debug and release the PDU          */ 
         RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB"); 
         wrUmmRlsIncMsg(&msg); 
         RETVALUE(RFAILED); 
      } 
      if (ueIdx >= cellCb->maxUesPerCell)
      {
         /* Log the error through an error debug and release the PDU          */
         RLOG1(L_ERROR, "ueIdx exceeds #UEs[%d]", ueIdx);
         wrUmmRlsIncMsg(&msg);
         RETVALUE(RFAILED);
      }
      wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIdx);
   }

   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CRNTI,crnti, "Error in getting UECB cellId [%lu]",
         (U32)cellId);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);

   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* This function is called by IFM module to pass the received X2 PDU      */
/*                                                                        */
/* This function is receives procedure initiaing messages from UE. It     */
/*    creates an incoming message that is common for all the different    */
/*    interfaces and invokes wrUmmPrcIncMsg() for further processing.     */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmPrcX2BmpCfm
(
CztEvnt                      *cztCfm
)
{
   WrCellId                  cellId;
   U8                        ueIdx;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;
   TknU8                     *ie;
   U32                       ueX2apId;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(cztCfm);
      RETVALUE(ROK);
   }
   msg->refCnt  = 1;
   msg->msgTyp  = WR_UMM_X2_BMP_PDU;
   msg->u.x2Pdu = cztCfm;

   wrUtlX2GetCztIE(&cztCfm->pdu, Cztid_Old_eNB_UE_X2AP_ID, (void **)&ie);
   if(ie == NULLP)
   {
      wrUmmRlsIncMsg(&msg);
      RLOG0(L_ERROR, "Old UE X2ID not found");
      RETVALUE(RFAILED);
   }
   ueX2apId = ((CztUE_X2AP_ID*)ie)->val;
   cellId = (ueX2apId & 0x0000ff00) >> 8;
   ueIdx  = (ueX2apId & 0x000000ff);
   WR_UMM_GET_CELLCB(cellCb, cellId); 
   if (cellCb == NULLP) 
   { 
      /* Log the error through an error debug and release the PDU          */ 
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB"); 
      wrUmmRlsIncMsg(&msg); 
      RETVALUE(RFAILED); 
   } 

   if (ueIdx >= cellCb->maxUesPerCell)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG1(L_ERROR, "ueIdx exceeds #UEs [%d]", ueIdx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }
   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIdx);

   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "idx [%u]", ueIdx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);

   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief  This function is called by IFM module to pass the received RRC
 *           data confirm
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         It receives the RRC data confirm from RRC. It     
 *         creates an incoming message that is common for all the different   
 *         interfaces and invokes wrUmmPrcIncMsg() for further processing.   
 *
 *
 * @param  NhuDatCfmSdus       *rrcDatCfm
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcRrcDatCfm
(
NhuDatCfmSdus                *rrcDatCfm
)
{
   WrCellId                  cellId;
   //WrCrnti                   crnti;
   U8                        ueIndx;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;

   //cellId = rrcDatCfm->hdr.cellId;
   //crnti  = rrcDatCfm->hdr.ueId;
   cellId = (rrcDatCfm->hdr.transId >> 20 ) & 0x0000000f;
   ueIndx = (rrcDatCfm->hdr.transId >> 12 ) & 0x000000ff;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE(rrcDatCfm, sizeof(*rrcDatCfm));
      RETVALUE(ROK);
   }
   msg->refCnt = 1;

   msg->msgTyp = WR_UMM_RRC_DAT_CFM;
   msg->u.rrcDatCfm = rrcDatCfm;

   //wrUmmGetUe(&ueCb, cellId, crnti); 
   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIndx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UE INDEX [%u]", ueIndx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }

   wrUmmPrcIncMsg(&wrUeDb.cells[0], ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* This function is called by IFM module to pass the received UU PDU      */
/*                                                                        */
/* This function is receives procedure initiaing messages from UE. It     */
/*    creates an incoming message that is common for all the different    */
/*    interfaces and invokes wrUmmPrcIncMsg() for further processing.     */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmPrcPdcpSduStaCfm
(
NhuPdcpSduStaCfm             *pdcpSduStaCfm
)
{
   WrCellId                  cellId;
   //WrCrnti                   crnti;
   U8                        ueIndx;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;

   //cellId = pdcpSduStaCfm->hdr.cellId;
   //crnti  = pdcpSduStaCfm->hdr.ueId;
   cellId = (pdcpSduStaCfm->hdr.transId >> 20 ) & 0x0000000f;
   ueIndx = (pdcpSduStaCfm->hdr.transId >> 12 ) & 0x000000ff;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(pdcpSduStaCfm);
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_PDCP_SDU_STATUS_CFM;
   msg->u.pdcpSduCfm = pdcpSduStaCfm;

   //wrUmmGetUe(&ueCb, cellId, crnti);
   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIndx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UE INDEX [%u]", ueIndx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }
   
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* This function is called by IFM module to pass the received UU PDU      */
/*                                                                        */
/* This function is receives procedure initiaing messages from UE. It     */
/*    creates an incoming message that is common for all the different    */
/*    interfaces and invokes wrUmmPrcIncMsg() for further processing.     */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmPrcPdcpDatResumeCfm
(
NhuPdcpDatResumeCfm             *pdcpDatResumeCfm
)
{
   WrCellId                  cellId;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;
   U8                        ueIndx;

   cellId = (pdcpDatResumeCfm->hdr.transId >> 20 ) & 0x0000000f;
   ueIndx = (pdcpDatResumeCfm->hdr.transId >> 12 ) & 0x000000ff;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(pdcpDatResumeCfm);
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_PDCP_DATA_RESUME_CFM;
   msg->u.pdcpDatResumeCfm = pdcpDatResumeCfm;

   /*wrUmmGetUe(&ueCb, cellId, crnti);*/
   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIndx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
       RLOG_ARG1(L_ERROR,DBG_CELLID,(U32)cellId,"Error in getting UECB for"
          "UE Index:[%u]",ueIndx);  
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }
   
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR,DBG_CELLID,(U32)cellId,"Error in getting CELL CB");
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}
/** @brief  This function is called by IFM module to pass the received RRC
 *           Error Indication
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         It receives the RRC Error Indication from RRC. It releases
 *         the incoming message.
 *
 *
 * @param  NhuErrIndSdus       *errInd
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcRrcErrInd
(
NhuErrIndSdus                *errInd
)
{
   WrCellId                  cellId;
   //WrCrnti                   crnti;
   U8                        ueIndx;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;

   //cellId = errInd->hdr.cellId;
   //crnti  = errInd->hdr.ueId;
   cellId = (errInd->hdr.transId >> 20 ) & 0x0000000f;
   ueIndx = (errInd->hdr.transId >> 12 ) & 0x000000ff;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG0(L_FATAL, "Memory allocation failed.");
      /* Fix : Memory Leak */
      WR_FREE(errInd, sizeof(*errInd));
      RETVALUE(ROK);
   }
   msg->refCnt = 1;

   msg->msgTyp      = WR_UMM_RRC_ERR_IND;
   msg->u.rrcErrInd = errInd;

   // wrUmmGetUe(&ueCb, cellId, crnti); 
   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIndx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UE INDEX [%u]", ueIndx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }

   wrUmmPrcIncMsg(&wrUeDb.cells[0], ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* This function is called by IFM module to pass the received S1ap Encoded*/
/* buffer.                                                                */
/*                                                                        */
/* This function receives the S1ap decoded buffer from RRC. It creates an */
/* incoming message that is common for all the different interfaces and   */
/* invokes wrUmmPrcIncMsg() for further processing.                       */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmPrcSztEncDecCfm
(
SztEncDecEvnt                *sztEncDecCfmSdus,
SztReqStatus                 *status
)
{
   WrCellId                  cellId;
   //WrCrnti                   crnti;
   U8                        ueIndx;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   
   cellId = (sztEncDecCfmSdus->transId >> 20 ) & 0x0000000f;
   ueIndx = (sztEncDecCfmSdus->transId >> 12 ) & 0x000000ff;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   if(status->status != 0)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Container encoding ERROR for "
         "UEINDEX[%u]", ueIndx); /* Prachi */
      msg->msgTyp = WR_UMM_S1AP_ENC_DEC_FAIL;
      msg->u.sztDecCfm = sztEncDecCfmSdus;
   }
   else
   {
   if ((sztEncDecCfmSdus->type.val == SZT_REQ_TYPE_DECODE_IE) &&
            ((sztEncDecCfmSdus->ieType.val ==  SZT_REQ_TYPE_ENCDEC_IE_TGETSRC) ||
             (sztEncDecCfmSdus->ieType.val == SZT_REQ_TYPE_ENCDEC_IE_TGETRNCSRCRNC)))
   {
      msg->msgTyp = WR_UMM_TGTTOSRC_CONT_DEC_CFM;
      msg->u.sztDecCfm = sztEncDecCfmSdus;
   }
   else if ((sztEncDecCfmSdus->type.val == SZT_REQ_TYPE_DECODE_IE) &&
         (sztEncDecCfmSdus->ieType.val ==  SZT_REQ_TYPE_ENCDEC_IE_SRCTGET))
   {
      msg->msgTyp = WR_UMM_SRCTOTGT_CONT_DEC_CFM;
      msg->u.sztDecCfm = sztEncDecCfmSdus;
   }
   else if ((sztEncDecCfmSdus->type.val == SZT_REQ_TYPE_ENCODE_IE) &&
            ((sztEncDecCfmSdus->ieType.val == SZT_REQ_TYPE_ENCDEC_IE_SRCTGET) ||
             (sztEncDecCfmSdus->ieType.val == SZT_REQ_TYPE_ENCDEC_IE_SRCRNCTGETRNC)))
   {
      msg->msgTyp = WR_UMM_S1AP_ENC_DEC_CFM;
      msg->u.sztEncCfm = sztEncDecCfmSdus;
   }
   else if ((sztEncDecCfmSdus->type.val == SZT_REQ_TYPE_ENCODE_IE) &&
         (sztEncDecCfmSdus->ieType.val ==  SZT_REQ_TYPE_ENCDEC_IE_TGETSRC))
   {
      msg->msgTyp = WR_UMM_S1AP_ENC_DEC_CFM;
      msg->u.sztEncCfm = sztEncDecCfmSdus;
   }
   else
   {
      /* Log the error through a error debug and release the PDU  */
      RLOG2(L_WARNING,"Container encoding CFM having a wrong TYPE[%u]/IE TYPE[%d]",
         sztEncDecCfmSdus->type.val, sztEncDecCfmSdus->ieType.val);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }
   }

   wrUmmGetUeFrmUeIdx(&ueCb, cellId,ueIndx);
   //wrUmmGetUe(&ueCb, cellId, ueIndx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UEINDEX[%u]", ueIndx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }
   wrUmmPrcIncMsg(&wrUeDb.cells[0], ueCb, msg);
   wrUmmRlsIncMsg(&msg);
   RETVALUE(ROK);
}

/** @brief  This function is called by IFM module to pass scheduler
 *           configuration status
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *          This function is receives configuration confrim from RGR/Scheduler     
 *             interface, creates a message and invokes transaction processor 
 *
 *
 * @param [in] transId
 * @param [in] status
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcSchdCfgCfm
(
U32                           transId,
U8                            status
)
{
   WrCellId                  cellId;
   //WrCrnti                   crnti;
   U8                        ueIndx;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;

   /*Fix for ccpu00125493 */
   cellId = (transId & 0x00f00000) >> 20;
   ueIndx = (transId & 0x000ff000) >> 12;
   

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_SCHD_CFG;
   msg->u.schdCfm.transId = transId;
   msg->u.schdCfm.status  = status;

   wrUmmGetUeFrmUeIdx(&ueCb, cellId,ueIndx);
      if (ueCb == NULLP)
      {
         /* Log the error through a error debug and release the PDU          */
         RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
               "UEINDEX[%u]", ueIndx);
         wrUmmRlsIncMsg(&msg);
         RETVALUE(ROK);
      }
      /*WR_SET_TRANS_ID(msg->u.schdCfm.transId,cellId, 
             ueCb->ueIdx,  0);*/

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

#ifdef TENB_AS_SECURITY
/** @brief  This function is called by IFM module to pass PHY 
 *           configuration status
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *          This function is receives configuration confrim from CTF/PHY
 *             interface, creates a message and invokes transaction processor 
 *
 *
 * @param [in] transId
 * @param [in] status
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcKdfCfm
(
U32                          transId,
U8                           status,
CtfKdfCfmInfo                *kdfCfmInf
)
{
   WrCellId                  cellId;
   U8                        ueIndx;
   //WrCrnti                   crnti;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;

   
   cellId = (transId & 0x00f00000) >> 20;
   ueIndx = (transId & 0x000ff000) >> 12;

   //cellId = (transId & 0xf0000000) >> 28;
   //crnti  = (transId & 0x0ffff000) >> 12;
  /*127802*/
   RLOG_ARG1(L_INFO, DBG_CELLID,(U32)cellId, "PHY cfg Cfm for UEINDEX[%u]",
      ueIndx);
      
   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      /*127802*/
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_PHY_KDF;
   msg->u.kdfCfm.status = status;
   msg->u.kdfCfm.transId = transId;
   msg->u.kdfCfm.ctfKdfCfm = kdfCfmInf;

   wrUmmGetUeFrmUeIdx(&ueCb, cellId,ueIndx);
      if (ueCb == NULLP)
      {
         /* Log the error through a error debug and release the PDU          */
         RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
            "UE INDEX [%u]", ueIndx);
         wrUmmRlsIncMsg(&msg);
         RETVALUE(ROK);
      }
   wrUmmPrcIncMsg(&wrUeDb.cells[0], ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}
#endif

/** @brief  This function is called by IFM module to pass PHY 
 *           configuration status
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *          This function is receives configuration confrim from CTF/PHY
 *             interface, creates a message and invokes transaction processor 
 *
 *
 * @param [in] transId
 * @param [in] status
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcPhyCfgCfm
(
U32                          transId,
U8                           status
)
{
   WrCellId                  cellId;
   //WrCrnti                   crnti;
   U8                        ueIndx;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   
   
   cellId = (transId & 0x00f00000) >> 20;
   ueIndx = (transId & 0x000ff000) >> 12;
   //cellId = (transId & 0xf0000000) >> 28;
   //crnti  = (transId & 0x0ffff000) >> 12;

  /*127802*/
   RLOG_ARG1(L_INFO, DBG_CELLID,(U32)cellId, "PHY cfg Cfm for UEINDEX[%u]",
      ueIndx);
      
   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_PHY_CFG;
   msg->u.phyCfm.transId = transId;
   msg->u.phyCfm.status  = status;

   wrUmmGetUeFrmUeIdx(&ueCb, cellId,ueIndx);
      if (ueCb == NULLP)
      {
         /* Log the error through a error debug and release the PDU          */
         RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
            "UEINDEX[%u]", ueIndx);
         wrUmmRlsIncMsg(&msg);
         RETVALUE(ROK);
      }
   wrUmmPrcIncMsg(&wrUeDb.cells[0], ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief  This function is called by IFM module to pass RRC
 *           configuration status
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *          This function is receives configuration confrim from NHU/RRC
 *             interface, creates a message and invokes transaction processor 
 *
 *
 * @param  NhuCfgCfmSdus                 *cfgCfm
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcRrcCfgCfm
(
NhuCfgCfmSdus                 *cfgCfm
)
{
   WrCellId                  cellId;
   //WrCrnti                   crnti;
   U8                        ueIndx;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;

   U32 transId = cfgCfm->hdr.transId;

   cellId = (transId & 0x00f00000) >> 20;
   ueIndx = (transId & 0x000ff000) >> 12;
//   cellId = (transId & 0xf0000000) >> 28;
//   crnti  = (transId & 0x0ffff000) >> 12;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE(cfgCfm, sizeof(*cfgCfm));
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_RRC_CFG;
   msg->u.rrcCfm = cfgCfm;

   wrUmmGetUeFrmUeIdx(&ueCb, cellId,ueIndx);
      if (ueCb == NULLP)
      {
         /* Log the error through a error debug and release the PDU          */
         RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
            "UEINDEX[%u]", ueIndx);
         wrUmmRlsIncMsg(&msg);
         RETVALUE(ROK);
      }
   wrUmmPrcIncMsg(&wrUeDb.cells[0], ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief  This function is called by IFM module to pass RRC encoded Buffer
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *          This function receives the RRC encoded buffer from RRC. It creates an 
 *          incoming message that is common for all the different interfaces anda
 *           invokes wrUmmPrcIncMsg() for further processing.
 *
 *
 * @param  NhuEncCfmSdus                 *rrcEncCfmSdus
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcRrcEncodeCfm
(
NhuEncCfmSdus          *rrcEncCfmSdus
)
{
   WrCellId                  cellId;
   U8                        ueIndx;
   //WrCrnti                   crnti;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;

   //cellId = rrcEncCfmSdus->hdr.cellId;
   //crnti  = rrcEncCfmSdus->hdr.ueId;
   cellId = (rrcEncCfmSdus->hdr.transId >> 20 ) & 0x0000000f;
   ueIndx = (rrcEncCfmSdus->hdr.transId >> 12 ) & 0x000000ff;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREEMBUF(rrcEncCfmSdus->sdu.mBuf);
      WR_FREE(rrcEncCfmSdus, sizeof(*rrcEncCfmSdus));
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_RRC_ENC_CFM;
   msg->u.encCfm = rrcEncCfmSdus;

   //wrUmmGetUe(&ueCb, cellId, crnti);
   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIndx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UEINDEX[%u]", ueIndx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }

   wrUmmPrcIncMsg(&wrUeDb.cells[0], ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief  This function is called by IFM module to pass RRC decoded Buffer
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *          This function receives the RRC decoded buffer from RRC. It creates an 
 *          incoming message that is common for all the different interfaces and
 *           invokes wrUmmPrcIncMsg() for further processing.
 *
 *
 * @param  NhuDecodeSduCfm    *rrcDecCfmSdus
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcRrcDecodeCfm
(
NhuDecodeSduCfm         *rrcDecCfmSdus
)
{
   WrCellId                  cellId;
   U8                        ueIndx;
   //WrCrnti                   crnti;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;

   //cellId = rrcDecCfmSdus->hdr.cellId;
   //crnti  = rrcDecCfmSdus->hdr.ueId;
   cellId = (rrcDecCfmSdus->hdr.transId >> 20 ) & 0x0000000f;
   ueIndx = (rrcDecCfmSdus->hdr.transId >> 12 ) & 0x000000ff;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(rrcDecCfmSdus);
      RETVALUE(ROK);
   }
   msg->refCnt = 1;

   msg->msgTyp = WR_UMM_RRC_DEC_CFM;
   msg->u.decCfm = rrcDecCfmSdus;

   //wrUmmGetUe(&ueCb, cellId, crnti); 
   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIndx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UEINDEX[%u]", ueIndx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }

   wrUmmPrcIncMsg(&wrUeDb.cells[0], ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief  This function is called by IFM module to pass the received S1AP
 *          PDU
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *           This function is receives procedure initiaing messages from UE. It
 *           creates an incoming message that is common for all the different
 *           interfaces and invokes wrUmmPrcIncMsg() for further processing.
 *
 *
 * @param  SztConCfm        *conCfm
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcS1apConCfm
(
SztConCfm                    *conCfm
)
{
   WrCellId                  cellId;
   U8                        ueIdx = 0;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;

   cellId = conCfm->suConnId >> 16;
   ueIdx  = conCfm->suConnId & 0xffff;

   WR_GET_CELLID_FRM_SUCONID(conCfm->suConnId, cellId, ueIdx);

   /*127802*/
   RLOG_ARG1(L_DEBUG, DBG_CELLID,(U32)cellId, "Inside wrUmmPrcS1apConCfm : UEINDEX[%u]", ueIdx);

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(conCfm->pdu);
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_S1_CON_CFM;
   /* cfmPdu is being copied the adress of
    * stack variables. Hence would be invalid during releasing.*/
   WR_ALLOC(&msg->u.cfmPdu, sizeof(*(msg->u.cfmPdu)));
   if (msg->u.cfmPdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(conCfm->pdu);
     WR_FREE(msg, sizeof(*msg));
      RETVALUE(RFAILED);
   }
   msg->u.cfmPdu->pdu = conCfm->pdu;
   msg->u.cfmPdu->suConnId = conCfm->suConnId;
   msg->u.cfmPdu->spConnId = conCfm->spConnId;

   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIdx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UEINDEX[%u]", ueIdx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief  This function is called whenever a local relesase is to be
 *          invoked
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *           This function is creates a message for LCL release and initiates a 
 *           transaction for releasing the context locally. It does not send
 *           release information over S1 to the connected MME.
 *
 *
 * @param [in] ueCb
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcLclCtxtRel
(
WrUeCb                       *ueCb,
U32                          relCause
)
{
   WrCellId                  cellId = ueCb->cellId;
   /*WrCrnti                   crnti  = ueCb->crnti;*/
   WrUmmIncMsg               *msg;
   WrUmmCellCb               *cellCb;

   /*127802*/
   RLOG_ARG1(L_DEBUG, DBG_CRNTI,ueCb->crnti, "Inside wrUmmPrcLclCtxtRel : cellId[%d]", cellId);

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_LCL_CTXT_REL;
   msg->u.rrcConRelCause = relCause;
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      WR_FREE(msg, sizeof(*msg));
      RETVALUE(RFAILED);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   wrEmmUpdateStat(WR_EMM_CALL_STATS,ueCb->cellId,WR_EMM_CALL_ACT,-1,WR_EMM_RRC_MAX);
   RETVALUE(ROK);
}

/** @brief  This function is called whenever a local relesase is to be
 *          invoked
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *           This function is creates a message for LCL release and initiates a 
 *           transaction for releasing the context locally. It does not send
 *           release information over S1 to the connected MME.
 *
 *
 * @param [in] ueCb
 *
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PUBLIC S16 wrUmmPrcDamUeDelCfm
(
U32                          transId,
U16                          cellId,
U16                          crnti
)
{
   WrUmmIncMsg               *msg;
   WrUmmCellCb               *cellCb = NULLP;
   WrUeCb                    *ueCb;


   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_DAM_UE_DEL_CFM;
   msg->u.damUeDelCfm.cellId  = cellId;
   msg->u.damUeDelCfm.crnti   = crnti;
   msg->u.damUeDelCfm.transId = transId;
   wrUmmGetUe(&ueCb, cellId, crnti);
   if (ueCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG2(L_ERROR, "Error in getting UECB for crnti[%u] & cellId[%ld]", crnti, (U32)cellId);
      WR_FREE(msg, sizeof(*msg));
      RETVALUE(RFAILED);
   }

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      WR_FREE(msg, sizeof(*msg));
      RETVALUE(RFAILED);
   }

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief This function is called upon creation of EGTP Tunnel
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Allocates a new WrUmmIncMsg 
 *         - Assigns msgType to WR_UMM_DAM_CFM
 *         - Call wrUmmPrcIncMsg with this newly created message, this should
 *         take the processing further
 *         - Free up the received message.
 *
 * @param [in] transId     Transaction ID
 * @param [in] status      Status i.e. OK/NOK
 * @param [in] lclTeid     Local tunnel end point ID
 */
PUBLIC S16 wrUmmTnlCreatCfm
(
U32                          transId,
U8                           status,
U32                          lclTeid
)
{
   WrCellId                  cellId;
   //WrCrnti                   crnti;
   U8                        ueIndx;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;

   /*Fix for ccpu00125493 */
   cellId = (transId & 0x00f00000) >> 20;
   ueIndx = (transId & 0x000ff000) >> 12;
   //cellId = (transId & 0xf0000000) >> 28;
   //crnti  = (transId & 0x0ffff000) >> 12;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }

   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_DAM_CFM;
   msg->u.damCfm.lclTeId = lclTeid;
   msg->u.damCfm.transId = transId;
   msg->u.damCfm.status  = status;

   wrUmmGetUeFrmUeIdx(&ueCb, cellId,ueIndx);
   //wrUmmGetUe(&ueCb, cellId, ueIndx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UEINDEX[%u]", ueIndx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }
   RLOG2(L_INFO, "Creating Tunnel create Cfm UE[%d] LclTeid[%ld] ",ueIndx,lclTeid);
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/* ccpu00126336 */
/** @brief This function is called upon modification of EGTP Tunnel
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Allocates a new WrUmmIncMsg 
 *         - Assigns msgType to WR_UMM_DAM_MOD_CFM
 *         - Call wrUmmPrcIncMsg with this newly created message, this should
 *         take the processing further
 *         - Free up the received message.
 *
 * @param [in] transId     Transaction ID
 * @param [in] status      Status i.e. OK/NOK
 * @param [in] lclTeid     Local tunnel end point ID
 */
PUBLIC S16 wrUmmTnlModCfm
(
U32                          transId,
U8                           status,
U32                          lclTeid
)
{
   WrCellId                  cellId;
   //WrCrnti                   crnti;
   U8                        ueIndx;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;

   /*Fix for ccpu00125493 */
   cellId = (transId & 0x00f00000) >> 20;
   ueIndx = (transId & 0x000ff000) >> 12;
   //cellId = (transId & 0xf0000000) >> 28;
   //crnti  = (transId & 0x0ffff000) >> 12;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }

   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_DAM_MOD_CFM;
   msg->u.damCfm.lclTeId = lclTeid;
   msg->u.damCfm.transId = transId;
   msg->u.damCfm.status  = status;

   wrUmmGetUeFrmUeIdx(&ueCb, cellId,ueIndx);
   //wrUmmGetUe(&ueCb, cellId, ueIndx); 
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UEINDEX[%u]", ueIndx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief This function processes Reestablishment confirm
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Currently we dont have handling for negative case i.e. failure of
 *         Re-Establishment hence the function returns ROK always
 *
 *
 * @param [in] transId : Transaction ID
 * @param [in] status  : Status of ReEstablishment 
 * @return S16
 *  -# ROK 
 *  -# RFAILED
 */
PUBLIC S16 wrUmmPrcReestabCfm
(
U32                          transId,
U8                           status
)
{
   /* TODO: As part of reestablishment, currently, we dont verify if this */
   /* is successful. This has almost no chance of failure. The negative   */
   /* path shall be added subsequently                                    */
   RETVALUE(ROK);
}

PUBLIC S16 wrUmmPrcErabRelIndctrl 
(
void
)
{   
   WrCellId                  cellId = 0x01;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;
   WrUmmRabRls               *lcrabrls;
   U8                        rabIdx;


   WR_ALLOC(&lcrabrls, sizeof(*lcrabrls));
   if (lcrabrls == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   lcrabrls->numrabs    = 0x01;
   lcrabrls->cause[0] = SztCauseRadioNwradio_resources_not_availableEnum;/*RRM_ARP*/
   lcrabrls->causeTyp = WR_CAUSE_RADIONW;
   
   RLOG0(L_INFO, "Sending E-RAB RELEASE INDICATION");
   
   for (rabIdx=0;rabIdx < lcrabrls->numrabs;rabIdx++)
   {
      lcrabrls->erabId[rabIdx] = (WR_UMM_ERAB_OFFSET + rabIdx + 1);
   }   

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_RAB_RLS_EVNT;
   msg->u.lcrabrls = lcrabrls;

   wrUmmGetUeCbforRabRel(&ueCb, cellId);

   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB");
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }
   
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }
   
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);



   RETVALUE(ROK);

}


/** @brief This function processes S1 Release indication
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Allocates a new WrUmmIncMsg 
 *         - Assigns msgType to WR_UMM_S1_REL_IND
 *         - Call wrUmmPrcIncMsg with this newly created message, this should
 *         take the processing further
 *         - Free up the received message.
 *
 * @param  SztRelInd                    *relInd
 * @return S16
 *       -# ROK     In case of success
 *       -# RFAILED In case of failure
 */
PUBLIC S16 wrUmmPrcS1RelInd
(
SztRelInd                    *relInd
)
{
   WrCellId                  cellId;
   U8                        ueIdx = 0;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;

   WR_GET_CELLID_FRM_SUCONID(relInd->suConnId.val, cellId, ueIdx);

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(relInd->pdu);
      RETVALUE(ROK);
   }
   msg->refCnt = 1;

   msg->msgTyp = WR_UMM_S1_REL_IND;
   /* MEMORY LEAK FIX: */
   WR_ALLOC(&msg->u.relInd, sizeof(SztRelInd));
   if (msg->u.relInd == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(relInd->pdu);
     WR_FREE(msg, sizeof(*msg));
      RETVALUE(RFAILED);
   }

   msg->u.relInd->pdu = relInd->pdu;
   /* Fix for ccpu00127369 */
   msg->u.relInd->spConnId = relInd->spConnId;
   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIdx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UE INDEX [%u]", ueIdx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(ROK);
   }

   wrUmmPrcIncMsg(&wrUeDb.cells[0], ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief This function processes S1 Error indication
 *
 * @details
 *
 *     Function: wrUmmPrcS1ErrInd 
 *
 *         Processing steps:
 *         - Allocates a new WrUmmIncMsg 
 *         - Assigns msgType to WR_UMM_S1AP_ERR_IND
 *         - Call wrUmmPrcIncMsg with this newly created message, this should
 *         take the processing further
 *         - Free up the received message.
 *
 * @param  SztLclErrInd                    *errInd
 * @return S16
 *       -# ROK     In case of success
 *       -# RFAILED In case of failure
 */
PUBLIC S16 wrUmmPrcS1ErrInd
(
SztLclErrInd                  *errInd
)
{
   WrCellId                  cellId;
   U8                        ueIdx = 0;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;

   WR_GET_CELLID_FRM_SUCONID(errInd->transId.val, cellId, ueIdx);

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   msg->refCnt = 1;

   msg->msgTyp = WR_UMM_S1AP_ERR_IND;
   /* ccpu00149336 */
   msg->u.sztErrInd = errInd;
   /* ccpu00149336 */
   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIdx);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UEINDEX[%u]", ueIdx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }

   wrUmmPrcIncMsg(&wrUeDb.cells[0], ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}
/** @brief This function handles timer expiry for the timers started by UMM
 * modules/transactions
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Based on the Expiry event type cast the "cb" to the transCb and
 *         fill in the value of the WrUmmIncMsg->msgType 
 *         - Call wrUmmPrcIncMsg
 *
 *
 * @param  void   *cb               Control block pointer
 * @param  U8     wrUmmTmrExpEvnt   Expiry Event
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmPrcTimerExp
(
WrUmmTimer                      *cb,
U8                           wrUmmTmrExpEvnt
)
{
   WrCellId                  cellId;
   WrCrnti                   crnti;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;
   WrUmmTransCb              *transCb;
   Bool                      bldIntMsgFlag = FALSE;
   switch(wrUmmTmrExpEvnt)
   {
      case WR_TMR_RRC_RRCCON:
      case WR_TMR_RRC_CONREL:
      case WR_TMR_RRC_CAPENQ:
      case WR_TMR_RRC_SECUCMD:
      case WR_TMR_REESTABLISH:
      case WR_TMR_RECONFIGURE:
      case WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG:
      case WR_TMR_TGT_HOPREP:
      case WR_TMR_RRC_DAT_CFM:
      case WR_TMR_TGT_PATH_SWITCH:
      case WR_TMR_RRC_REL_GUARD:
      case WR_TMR_S1_RELOC_TMR:
      case WR_TMR_SRC_HO_CANCEL:
      case WR_TMR_UL_HO_PREP_WAIT_TMR:
      case WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR:
      case WR_TMR_ECSFB_HO_EXEC_TMR:
      case WR_TMR_ANR_STRONGCELL_REPORT_R1:
      {
         transCb     = (WrUmmTransCb *)cb->cb;
         ueCb        = transCb->ueCb;
         cellId      = transCb->ueCb->cellId;
         crnti       = transCb->ueCb->crnti;
          cmLListDelFrm(&(transCb->tmrLst), &(cb->tmrLnk));
         break;
      }
      case WR_TMR_CSFB_CFG:
      case WR_TMR_ECSFB_MEAS_RPT:
      {
         transCb = (WrUmmTransCb *)cb->cb;
         ueCb    = transCb->ueCb;
         cellId  = transCb->ueCb->cellId;
         crnti   = transCb->ueCb->crnti;
          cmLListDelFrm(&(transCb->tmrLst), &(cb->tmrLnk));
         break;
      }
      case WR_TMR_CSFB_GSM_T304_EXP_WAIT:
      case WR_TMR_CSFB_GSM_MEAS_WAIT:
      {
         transCb = (WrUmmTransCb *)cb->cb;
         ueCb    = transCb->ueCb;
         cellId  = ueCb->cellId;
         crnti   = ueCb->crnti;
    	   cmLListDelFrm(&(transCb->tmrLst), &(cb->tmrLnk));
         break;
      }
      case WR_TMR_RAD_RES_MOD_TMR:
      {
         ueCb        = (WrUeCb *)cb->cb; 
         cellId      = ueCb->cellId;
         crnti       = ueCb->crnti;
         bldIntMsgFlag = TRUE;
          cmLListDelFrm(&(ueCb->tmrLst), &(cb->tmrLnk));
         break;
      }
      case WR_TMR_INITIAL_CTX_REQ:
      {
         ueCb        = (WrUeCb *)cb->cb; 
          cmLListDelFrm(&(ueCb->tmrLst), &(cb->tmrLnk));
         wrUmmInitalCtxRspTimerExp(ueCb);
         RETVALUE(ROK);
      }
      case WR_TMR_UE_CTX_REL_REQ:
      {
         ueCb        = (WrUeCb *)cb->cb; 
          cmLListDelFrm(&(ueCb->tmrLst), &(cb->tmrLnk));
         wrUmmUeCtxRelTimerExp(ueCb);
         RETVALUE(ROK);
      }
#ifdef WR_DETECT_RLF
      case WR_TMR_DETECT_RLF:
      {
         ueCb        = (WrUeCb*)cb->cb; 
         cellId      = ueCb->cellId;
         crnti       = ueCb->crnti;
         RLOG_ARG0(L_WARNING, DBG_CRNTI,ueCb->crnti, "CQI Rept Timer Expired "
            "RLF Detected");
          cmLListDelFrm(&(ueCb->tmrLst), &(cb->tmrLnk));
         wrUmmCqiReptTmrExp(ueCb);
         RETVALUE(ROK);
      }
     break;
#endif
      default:
      {
         RETVALUE(ROK);
      }
   }

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }

   msg->refCnt = 1;

   if (bldIntMsgFlag == FALSE)
   {    
      msg->u.tmrExpiry.transId = ((WrUmmTransCb*)cb->cb)->transId;
      msg->msgTyp = WR_UMM_TMR_EXPIRY;
      msg->u.tmrExpiry.timerEvnt = wrUmmTmrExpEvnt;
   }
   else
   {
      wrUmmTmrExpBldInternalMsg(wrUmmTmrExpEvnt, msg);
   }

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug and release the PDU          */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief This function responsible for building internal transaction
 *
 * @details
 *
 *     Function: wrUmmBldInternalMsg
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] intMsg : Pointer to the WrUmmInternalMsg structure
 * @param [in] intMsgType: message type
 * @param [in] msg : Pointer to the message
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PRIVATE Void wrUmmTmrExpBldInternalMsg
(
 U8                           wrUmmTmrExpEvnt,
 WrUmmIncMsg                  *msg
)
{
   U8 intMsgType = 0;
   switch (wrUmmTmrExpEvnt)
   {
      case WR_TMR_RAD_RES_MOD_TMR:
      {         
         intMsgType = WR_UMM_INTMSG_RAD_RES_MOD_MSG;
         break;
      }
      default:
         RLOG1(L_ERROR, "Unknown Timer Expiry Event[%d] Received ",
            wrUmmTmrExpEvnt);
         RETVOID;
   }   
   msg->msgTyp = WR_UMM_INTERNAL_MSG;
   wrUmmBldInternalMsg(&msg->u.internalMsg, intMsgType, msg);

}
/** @brief This function processes S1 Release confirm for a UE
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Currently we dont have handling for negative case i.e. failure of
 *         S1 Release hence the function returns ROK always
 *
 *
 * @param [in] transId : Transaction ID
 * @param [in] status  : Status of ReEstablishment 
 * @return S16
 *  -# ROK 
 *  -# RFAILED
 */
PUBLIC S16 wrUmmPrcS1RelCfm
(
SztRelCfm                    *relCfm
)
{
   /* Need to process like all other messages received                    */
   RETVALUE(ROK);
}

/** @brief This function gets a free Transaction ID for RRC Transaction
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Check if Message type > WR_NUM_DLDCCH_MSGS
 *         - Loop over list of Transaction IDs to find a free entity
 *         - When found return the allocated Transaction ID
 *
 *
 * @param [in] ueCb,
 * @param [in] dlDcchMsg,
 * @param [in] transId,
 * @param [in] rrcTransId
 * @return S16
 *    -# ROK
 *    -# RFAILED
 */
PUBLIC S16 wrUmmGetRrcTransId
(
WrUeCb                       *ueCb,
U8                           dlDcchMsg,
U8                           transId,
U8                           *rrcTransId
)
{
   U32                       idx;

   if (dlDcchMsg >= WR_NUM_DLDCCH_MSGS)
   {
      RETVALUE(RFAILED);
   }
   for(idx = 0; idx < WR_NUM_RRC_TRANS; idx++)
   {
      if (ueCb->rrcTrans[dlDcchMsg][idx] >= WR_INVALID_TRANS_ID)
      {
         *rrcTransId = idx;
         ueCb->rrcTrans[dlDcchMsg][idx] = transId;
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
}
/** @brief This function frees RRC Transaction ids for a Transaction
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loop over list of Transaction IDs to find TransId passed for which 
 *         - RRC Transaction Allocated
 *
 *
 * @param [in] ueCb,
 * @param [in] transId,
 * @return S16
 *    -# ROK
 *    -# RFAILED
 */
PUBLIC S16 wrUmmFreeRrcTransId
(
WrUeCb                       *ueCb,
U8                           transId
)
{
   U8  idx1,idx2;

   for(idx1 = 0; idx1 < WR_NUM_DLDCCH_MSGS; idx1++)
   {
      for(idx2 = 0; idx2 < WR_NUM_RRC_TRANS; idx2++) /* maximum 4 possible rrc transactions */
      {
         if (ueCb->rrcTrans[idx1][idx2] == transId)
         {
            RLOG_ARG1(L_INFO,DBG_CRNTI,ueCb->crnti,"Freeing RRC TransId for transId :%d ", 
                               transId);
            ueCb->rrcTrans[idx1][idx2] = WR_INVALID_TRANS_ID;
         }
      }
   }
   RETVALUE(ROK);
}

/** @brief This function gets a transaction ID.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Returns value of the TransID based on the dlDcchMsg and rrcTransId
 *
 *
 * @param [in] ueCb,
 * @param [in] dlDcchMsg
 * @param [in] rrcTransId
 * @param [in] transId
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmGetTransId
(
WrUeCb                       *ueCb,
U8                           dlDcchMsg,
U8                           rrcTransId,
U8                           *transId
)
{
   if (dlDcchMsg >= WR_NUM_DLDCCH_MSGS)
   {
      RETVALUE(RFAILED);
   }
   if (rrcTransId >= WR_NUM_RRC_TRANS)
   {
      RETVALUE(RFAILED);
   }
   if (ueCb->rrcTrans[dlDcchMsg][rrcTransId] < WR_INVALID_TRANS_ID)
   {
      *transId = (U8)ueCb->rrcTrans[dlDcchMsg][rrcTransId];
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}

/** @brief This function releases RRC Transaction to the Free List
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Releases RRC transaction back to the free list
 *
 *
 * @param [in] ueCb
 * @param [in] dlDcchMsg
 * @param [in] rrcTransId
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmRelRrcTransId
(
WrUeCb                       *ueCb,
U8                           dlDcchMsg,
U8                           rrcTransId
)
{
   if (dlDcchMsg >= WR_NUM_DLDCCH_MSGS)
   {
      RETVALUE(RFAILED);
   }
   if (rrcTransId >= WR_NUM_RRC_TRANS)
   {
      RETVALUE(RFAILED);
   }
   if (ueCb->rrcTrans[dlDcchMsg][rrcTransId] < WR_INVALID_TRANS_ID)
   {
      ueCb->rrcTrans[dlDcchMsg][rrcTransId] = WR_INVALID_TRANS_ID;
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}

/** @brief This function initializes the RRC transaction ID free List
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loops over the maximum number of TransIDs and DCCH Message types
 *         and intializes the values to WR_INVALID_TRANS_ID.
 *
 *
 * @param [in] ueCb: UE control block
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC Void wrUmmInitRrcTrans
(
WrUeCb                       *ueCb
)
{
   U32                       msgIdx;
   U32                       txIdx;

   for(msgIdx = 0; msgIdx < WR_NUM_DLDCCH_MSGS; msgIdx++)
   {
      for(txIdx = 0; txIdx < WR_NUM_RRC_TRANS; txIdx++)
      {
         ueCb->rrcTrans[msgIdx][txIdx] = WR_INVALID_TRANS_ID;
      }
   }
}
#ifdef WR_UMM_WITH_RRM
PUBLIC S16 wrRrmCellInit
(
WrCellId                     cellId
)
{
}

PUBLIC S16 wrRrmCallAdmCntrl
(
WrUeCb                       *ueCb
)
{
}
#endif

/** @brief This function releases UE control block
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Loops through the list of Transaction active for the UEs
 *           - Releases the transactions
 *         - Finally release the UE Cb itself
 *
 *
 * @param [in] ueCb: UE control block
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmRelUeCb
(
WrUeCb                       *ueCb
)
{
   WrUmmTransCb              *transCb;

   /* ccpu00148952 */
   RLOG_ARG2(L_DEBUG, DBG_CRNTI, ueCb->crnti, "mobCtrlState[%d] hoType[%d]"
         , (U32)ueCb->mobCtrlState, (U32)ueCb->hoType);

   if (ueCb->hoType == WR_HO_TYPE_X2)
   {
     RLOG_ARG0(L_DEBUG, DBG_CRNTI, ueCb->crnti, "Sending Release for X2 "); 
     wrProcUeX2HoReq(ueCb);
   }
   /* ccpu00148952 */

   while(ueCb->transLst.first != NULLP)
   {
      transCb = (WrUmmTransCb *)(ueCb->transLst.first->node);
      wrUmmTransRel(transCb);
   }
   wrUmmDelUe(ueCb);

   RETVALUE(ROK);
}

/** @brief This function is called by EMM to create an MME context in UMM module
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Initializes ueLst within MME control block
 *         - Sets the flag mmeCfgd equal to TRUE
 *
 *
 * @param mmeId: MME Index
 * @return S16
 *          -# ROK
 *          -# RFAILED
 */
PUBLIC S16 wrUmmAddMme  
(
WrMmeId                      mmeId
)
{
   if (mmeId >= WR_MAX_MMES)
   {
      RETVALUE(RFAILED);
   }
   
   cmLListInit(&wrUeDb.mmes[mmeId].ueLst);
   wrUeDb.mmes[mmeId].mmeCfgd = TRUE;
   RETVALUE(ROK);
}

/** @brief This function Deletes the MME control block in UMM, called by EMM
 * module when an MME gets deleted.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - If any UE context is present for this MME return Failed.
 *         - Else set the flag mmeCfgd equals FALSE
 *
 *
 * @param   mmeId: MME index
 * @return S16
 *          -# ROK
 *          -# RFAILED
 */
PUBLIC S16 wrUmmDelMme  
(
WrMmeId                      mmeId
)
{
   if (mmeId >= WR_MAX_MMES)
   {
      RETVALUE(RFAILED);
   }
   
   if (wrUeDb.mmes[mmeId].ueLst.first != NULLP)
   {
      RETVALUE(RFAILED);
   }

   wrUeDb.mmes[mmeId].mmeCfgd = FALSE;
   RETVALUE(ROK);
}

PRIVATE Void wrUmmAddDrbParam
(
 WrUmmCellCb               *ummCellCb,
 LwrDrbPolicy              *drb
)
{
   WrRabCfgParams           *rabCfg = NULLP;
   rabCfg = &ummCellCb->wrRabCfgParams[drb->qci];

   /* Fill parameters for QCI matched DRB                             */
   rabCfg->qciSupported                    = drb->qciInfo.qciSupported;
   rabCfg->rlcMode                         = drb->rlcInfo.rlcMode;

   if (drb->rlcInfo.rlcMode == WR_RLC_UM_MODE)
   {
      /* Filling RLC UM related Parameters */
      rabCfg->rlcParams.u.um.ul.snFldLen      = drb->rlcInfo.ulSnFieldLength;
      rabCfg->rlcParams.u.um.dl.snFldLen      = drb->rlcInfo.dlSnFieldLength;
      rabCfg->rlcParams.u.um.ul.tReordering   = drb->rlcInfo.ulTimerReordering;
      rabCfg->rlcParams.u.um.dl.tReordering   = drb->rlcInfo.dlTimerReordering;
   }
   else 
   {
      /* Filling RLC AM related Parameters */
      rabCfg->rlcParams.u.am.ul.statusProhTmr = drb->rlcInfo.ulTimerStatusProhibit; 
      rabCfg->rlcParams.u.am.ul.reorderingTmr = drb->rlcInfo.ulTimerReordering;
      rabCfg->rlcParams.u.am.ul.pollByte = drb->rlcInfo.ulPollByte;
      rabCfg->rlcParams.u.am.ul.pollPdu = drb->rlcInfo.ulPollPdu; 
      rabCfg->rlcParams.u.am.ul.pollRetxTmr = drb->rlcInfo.ulTpRetransmit;
      rabCfg->rlcParams.u.am.ul.maxRetxThrsh = drb->rlcInfo.ulMaxRetransThresh;
      rabCfg->rlcParams.u.am.ul.pbr = drb->lchInfo.prioritisedBitRate;
      rabCfg->rlcParams.u.am.ul.priority = drb->lchInfo.lchPriority;
      rabCfg->rlcParams.u.am.ul.bucketSizeDur = drb->lchInfo.bcketSizeDurn;
      /*rabCfg->rlcParams.u.am.ul.lcg = drb->lchInfo.lchGrpId; */

      rabCfg->rlcParams.u.am.dl.statusProhTmr = drb->rlcInfo.dlTimerStatusProhibit;
      rabCfg->rlcParams.u.am.dl.reorderingTmr = drb->rlcInfo.dlTimerReordering; 
      rabCfg->rlcParams.u.am.dl.pollByte = drb->rlcInfo.dlPollByte; 
      rabCfg->rlcParams.u.am.dl.pollPdu = drb->rlcInfo.dlPollPdu; 
      rabCfg->rlcParams.u.am.dl.pollRetxTmr = drb->rlcInfo.dlTpRetransmit;
      rabCfg->rlcParams.u.am.dl.maxRetxThrsh = drb->rlcInfo.dlMaxRetransThresh; 
   }
   /* Filling PDCP Parameters */
   rabCfg->pdcpParams.pdcpSnSize           = drb->pdcpInfo.umSnSize;
   rabCfg->pdcpParams.discardTimer         = drb->pdcpInfo.discardTimer;
   rabCfg->pdcpParams.statusRprtReqd       = drb->pdcpInfo.statRprtReqrd;
   rabCfg->pdcpParams.fwdEndTimer          = drb->pdcpInfo.fwdEndTimer;
   rabCfg->pdcpParams.hdrCompress          = 0;
   rabCfg->pdcpParams.cfmReqrd             = FALSE;

   /* Filling DSCP related Parameters */
   rabCfg->dscp                            = drb->dscpInfo.dscp;
   rabCfg->port                            = drb->dscpInfo.port;

   /* Filling LCH related Parameters */
   rabCfg->lchParams.priority              = drb->lchInfo.lchPriority;
   rabCfg->lchParams.bitRate               = drb->lchInfo.prioritisedBitRate;
   rabCfg->lchParams.bucketSizeDur         = drb->lchInfo.bcketSizeDurn;
   rabCfg->lchParams.lchGrp                = wrSmDfltPrioInfoTbl.drb[drb->qci].lchGrp;

}

PRIVATE Void wrUmmUpdateDrbParams
(
 WrUmmCellCb               *ummCellCb,
 LwrRabPolicyCfgGrp        *rabPolicyCfgGrp
)
{
   LwrDrbPolicy             *drb = NULLP;
   U16                      idx = 0;

   for(idx = 0; idx < rabPolicyCfgGrp->numDrbPolicies; idx++)
   {
      drb = &rabPolicyCfgGrp->drbPolicy[idx]; 

      if(drb->qci > WR_MAX_QCI)
      {
         RLOG1(L_WARNING, "Unknown DRB QCI:[%d]", (drb->qci));
         RETVOID;
      }
      wrUmmAddDrbParam(ummCellCb, drb);
   } /* end of for to update all DRBs */
} /* end of wrUmmUpdateDrbParams */

PRIVATE Void wrUmmModDrbParam
(
 WrUmmCellCb               *ummCellCb,
 LwrDrbPolicy              *drb
)
{
   WrRabCfgParams           *rabCfg = NULLP;
   U32                      rCfgType = 0;
   U32                      mask = 1;

   rabCfg = &ummCellCb->wrRabCfgParams[drb->qci];
   rCfgType =  drb->reCfgType;

   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_CELL_DRB_LCH_INFO:
               {
                  rCfgType =  rCfgType ^ mask; 

                  /* Filling LCH related Parameters */
                  rabCfg->lchParams.priority              = drb->lchInfo.lchPriority;
                  rabCfg->lchParams.bitRate               = drb->lchInfo.prioritisedBitRate;
                  rabCfg->lchParams.bucketSizeDur         = drb->lchInfo.bcketSizeDurn;
                  rabCfg->lchParams.lchGrp                = wrSmDfltPrioInfoTbl.drb[drb->qci].lchGrp;
                  RLOG3(L_INFO, "DRBLCH: Priority[%d] DRBLCH: bitRate[%d]"
                     "DRBLCH: BucketSizeDur[%d]", rabCfg->lchParams.priority,
                     rabCfg->lchParams.bitRate,rabCfg->lchParams.bucketSizeDur);
                  break;
               }
            case LWR_CELL_DRB_RLC_INFO:
               {
                  rCfgType =  rCfgType ^ mask; 

                  rabCfg->rlcMode                         = drb->rlcInfo.rlcMode;
                  if (drb->rlcInfo.rlcMode == WR_RLC_UM_MODE)
                  {
                     /* Filling RLC UM related Parameters */
                     rabCfg->rlcParams.u.um.ul.snFldLen      = drb->rlcInfo.ulSnFieldLength;
                     rabCfg->rlcParams.u.um.dl.snFldLen      = drb->rlcInfo.dlSnFieldLength;
                     rabCfg->rlcParams.u.um.ul.tReordering   = drb->rlcInfo.ulTimerReordering;
                     rabCfg->rlcParams.u.um.dl.tReordering   = drb->rlcInfo.dlTimerReordering;
                  }
                  else 
                  {
                     /* Filling RLC AM related Parameters */
                     rabCfg->rlcParams.u.am.ul.statusProhTmr =
                                 drb->rlcInfo.ulTimerStatusProhibit;
                     rabCfg->rlcParams.u.am.ul.reorderingTmr =
                                     drb->rlcInfo.ulTimerReordering;
                     rabCfg->rlcParams.u.am.ul.pollByte =
                                            drb->rlcInfo.ulPollByte;
                     rabCfg->rlcParams.u.am.ul.pollPdu =
                                             drb->rlcInfo.ulPollPdu;
                     rabCfg->rlcParams.u.am.ul.pollRetxTmr =
                                        drb->rlcInfo.ulTpRetransmit;
                     rabCfg->rlcParams.u.am.ul.maxRetxThrsh =
                                    drb->rlcInfo.ulMaxRetransThresh;
                     rabCfg->rlcParams.u.am.ul.pbr =
                                    drb->lchInfo.prioritisedBitRate;
                     rabCfg->rlcParams.u.am.ul.priority =
                                           drb->lchInfo.lchPriority;
                     rabCfg->rlcParams.u.am.ul.bucketSizeDur =
                                         drb->lchInfo.bcketSizeDurn;
                     /*rabCfg->rlcParams.u.am.ul.lcg = 
                      *          drb->lchInfo.lchGrpId; */
                     rabCfg->rlcParams.u.am.dl.statusProhTmr =
                                 drb->rlcInfo.dlTimerStatusProhibit;
                     rabCfg->rlcParams.u.am.dl.reorderingTmr =
                                     drb->rlcInfo.dlTimerReordering;
                     rabCfg->rlcParams.u.am.dl.pollByte =
                                            drb->rlcInfo.dlPollByte;
                     rabCfg->rlcParams.u.am.dl.pollPdu =
                                             drb->rlcInfo.dlPollPdu;
                     rabCfg->rlcParams.u.am.dl.pollRetxTmr =
                                        drb->rlcInfo.dlTpRetransmit;
                     rabCfg->rlcParams.u.am.dl.maxRetxThrsh =
                                    drb->rlcInfo.dlMaxRetransThresh;
                  }
                  break;
               }
            case LWR_CELL_DRB_PDCP_INFO:
               {
                  rCfgType =  rCfgType ^ mask; 
                  /* Filling PDCP Parameters */
                  rabCfg->pdcpParams.pdcpSnSize           = drb->pdcpInfo.umSnSize;
                  rabCfg->pdcpParams.discardTimer         = drb->pdcpInfo.discardTimer;
                  rabCfg->pdcpParams.statusRprtReqd       = drb->pdcpInfo.statRprtReqrd;
                  rabCfg->pdcpParams.fwdEndTimer          = drb->pdcpInfo.fwdEndTimer;
                  rabCfg->pdcpParams.hdrCompress          = 0;
                  rabCfg->pdcpParams.cfmReqrd             = FALSE;
                  break;
               }
            case LWR_CELL_DRB_DSCP_INFO:
               {
                  rCfgType =  rCfgType ^ mask; 
                  /* Filling DSCP related Parameters */
                  rabCfg->dscp                            = drb->dscpInfo.dscp;
                  rabCfg->port                            = drb->dscpInfo.port;
                  break;
               }
            case LWR_CELL_DRB_QCI_INFO:
               {
                  /* Fill parameters for QCI matched DRB                             */
                  rabCfg->qciSupported                    = drb->qciInfo.qciSupported;
                  rCfgType =  rCfgType ^ mask; 
                  break;
               }
            case LWR_CELL_DRB_QCI:
               {
                  rCfgType =  rCfgType ^ mask; 
                  break;
               }
            default:
               {   
                  rCfgType =  rCfgType ^ mask; 
                  RLOG1(L_WARNING, "DRB Policy Re-config received unknown "
                     "reCfgType [%ld]", (drb->reCfgType));
                  break;
               }
         }
      } /* end of if */
      mask = mask << 1;
   } /* end of while */
} /* end of wrUmmModDrbParam */

PRIVATE Void wrUmmDelDrbParam
(
 WrUmmCellCb               *ummCellCb,
 LwrDrbPolicy              *drb
)
{
   WrRabCfgParams           *rabCfg = NULLP;

   rabCfg = &ummCellCb->wrRabCfgParams[drb->qci];
   rabCfg->qciSupported = FALSE;
}

PRIVATE Void wrUmmReCfgDrbParams
(
 WrUmmCellCb               *ummCellCb,
 LwrRabPolicyCfgGrp        *rabPolicyCfgGrp
)
{
   U16                      idx = 0;

   for(idx = 0; idx < rabPolicyCfgGrp->numDrbPolicies; idx++)
   {
      LwrDrbPolicy          *drb = NULLP;
      U8                    action;

      drb = &rabPolicyCfgGrp->drbPolicy[idx]; 

      if(drb->qci > WR_MAX_QCI)
      {
         RLOG1(L_WARNING, "Unknown DRB QCI:[%d]", (drb->qci));
         RETVOID;
      }
      action = rabPolicyCfgGrp->drbPolicy[idx].action;

      if(action == WR_ACT_ADD)
      {
         wrUmmAddDrbParam(ummCellCb, drb);
      }
      else if(action == WR_ACT_MOD)
      {
         wrUmmModDrbParam(ummCellCb, drb);
      }
      else if(action == WR_ACT_DEL)
      {
         wrUmmDelDrbParam(ummCellCb, drb);
      }
      else
      {
         RLOG1(L_ERROR, "wrUmmReCfgDrbParams : Invalid Action type[%d] received ",action);
      }
   } /* end of for to update all DRBs */
} /* end of wrUmmReCfgDrbParams */

PRIVATE Void wrUmmReCfgSrbParams
(
 WrUmmCellCb               *ummCellCb,
 LwrRabPolicyCfgGrp        *rabPolicyCfgGrp
)
{
   WrRabCfgParams           *rabCfg = NULLP;
   LwrSrbPolicy             *srb = NULLP;
   U16                      idx = 0;
   U32                      rCfgType = 0;
   U32                      mask = 1;

   for(idx = 0; idx < rabPolicyCfgGrp->numSrbPolicies; idx++)
   {
      srb = &rabPolicyCfgGrp->srbPolicy[idx]; 

      switch(srb->srbId)
      {
         case LWR_SRB_ID_1:
            {
               /* Fill parameters for SRB1                                            */
               rabCfg = &ummCellCb->wrRabCfgParams[WR_SRB1_QCI];
               break;
            }
         case LWR_SRB_ID_2:
            {
               /* Fill parameters for SRB2                                            */
               rabCfg = &ummCellCb->wrRabCfgParams[WR_SRB2_QCI];
               break;
            }
         default:
            {
               RLOG1(L_ERROR, "Unknown SRB ID:[%d]", (srb->srbId));
               RETVOID;
               break;
            }
      } /* end of switch */

      /* Update the common SRB Parameters which are not receiving from OAM */
      rabCfg->qciSupported           = TRUE;
      rabCfg->rlcMode                = WR_RLC_AM_MODE;
      rabCfg->rlcDir                 = WR_DIR_DL_UL;
      rabCfg->pdcpParams.cfmReqrd    = TRUE;

      rCfgType =  srb->reCfgType;

      while (rCfgType)
      {
         if(rCfgType & mask)
         {
            switch(mask)
            {
               case LWR_CELL_SRB_SRBID:
                  {
                     rCfgType =  rCfgType ^ mask; 
                     break;
                  }
               case LWR_CELL_SRB_RLC_INFO:
                  {
                     rCfgType =  rCfgType ^ mask; 
                     rabCfg->rlcParams.u.am.ul.statusProhTmr = srb->rlcInfo.timerStatusProhibit;
                     rabCfg->rlcParams.u.am.ul.reorderingTmr = srb->rlcInfo.timerReorderingUl;
                     rabCfg->rlcParams.u.am.ul.pollByte = srb->rlcInfo.pollByte;
                     rabCfg->rlcParams.u.am.ul.pollPdu = srb->rlcInfo.pollPdu; 
                     rabCfg->rlcParams.u.am.ul.pollRetxTmr = srb->rlcInfo.tpRetransmit; 
                     rabCfg->rlcParams.u.am.ul.maxRetxThrsh = srb->rlcInfo.maxRetransThresh;

                     rabCfg->rlcParams.u.am.dl.statusProhTmr = srb->rlcInfo.timerStatusProhibit;
                     rabCfg->rlcParams.u.am.dl.reorderingTmr = srb->rlcInfo.timerReorderingDl;
                     rabCfg->rlcParams.u.am.dl.pollByte = srb->rlcInfo.pollByte;
                     rabCfg->rlcParams.u.am.dl.pollPdu = srb->rlcInfo.pollPdu;
                     rabCfg->rlcParams.u.am.dl.pollRetxTmr = srb->rlcInfo.tpRetransmit;
                     rabCfg->rlcParams.u.am.dl.maxRetxThrsh = srb->rlcInfo.maxRetransThresh;

                     rabCfg->rlcParams.u.am.ul.pbr = srb->lchInfo.pbr;
                     rabCfg->rlcParams.u.am.ul.bucketSizeDur = srb->lchInfo.bucketSizeDuration;
                     rabCfg->rlcParams.u.am.ul.lcg = srb->lchInfo.lchGrpId;
                     RLOG3(L_DEBUG, "StatusProhTmr[%ld] TimerReordering[%ld] "
                        "PollByte[%ld]", rabCfg->rlcParams.u.am.ul.statusProhTmr,
                        rabCfg->rlcParams.u.am.ul.reorderingTmr,
                        rabCfg->rlcParams.u.am.ul.pollByte);
                     RLOG3(L_DEBUG, "PollPdu[%ld] TpRetransmit[%ld] "
                        "MaxRetransThresh[%ld] ",
                        rabCfg->rlcParams.u.am.ul.pollPdu,
                        rabCfg->rlcParams.u.am.ul.pollRetxTmr,
                        rabCfg->rlcParams.u.am.ul.maxRetxThrsh);
                     RLOG3(L_DEBUG, "StatusProhTmr[%ld] reorderingTmr[%ld] "
                        "pollByte[%ld]",rabCfg->rlcParams.u.am.dl.statusProhTmr,
                        rabCfg->rlcParams.u.am.dl.reorderingTmr,
                        rabCfg->rlcParams.u.am.dl.pollByte);
                     RLOG3(L_DEBUG, " pollPdu[%ld] pollRetxTmr[%ld] "
                        "maxRetxThrsh[%ld]", rabCfg->rlcParams.u.am.dl.pollPdu,
                        rabCfg->rlcParams.u.am.dl.pollRetxTmr,
                        rabCfg->rlcParams.u.am.dl.maxRetxThrsh);
                     break;
                  }
               case LWR_CELL_SRB_LCH_INFO:
                  {
                     rCfgType =  rCfgType ^ mask; 
                     rabCfg->lchParams.bitRate = srb->lchInfo.pbr;
                     rabCfg->lchParams.bucketSizeDur = srb->lchInfo.bucketSizeDuration;
                     rabCfg->lchParams.lchGrp = srb->lchInfo.lchGrpId;

                     rabCfg->rlcParams.u.am.ul.priority = srb->lchInfo.priority;
                     rabCfg->lchParams.priority = srb->lchInfo.priority;
                     break;
                  }
               case LWR_CELL_SRB_QCI_INFO:
                  {
                     rCfgType =  rCfgType ^ mask; 
                     /* TODO: QCI Info not used for SRB's currently */
                     /*   srb->qciInfo.resourceType;
                          srb->qciInfo.priotity;
                          srb->qciInfo.pdb;
                          srb->qciInfo.pler; */

                     break;
                  }
               default:
                  {   
                     rCfgType =  rCfgType ^ mask; 
                     RLOG1(L_ERROR, "SRB Poloicy Re-config received unknown "
                        "reCfgType [%ld]", (srb->reCfgType));

                     break;
                  }
            }
         } /* end of if */
         mask = mask << 1;
      } /* end of while */
   } /* end of for to process all Srbs */
} /* end of wrUmmReCfgSrbParams */

PRIVATE Void wrUmmUpdateSrbParams
(
 WrUmmCellCb               *ummCellCb,
 LwrRabPolicyCfgGrp        *rabPolicyCfgGrp
)
{
   WrRabCfgParams           *rabCfg = NULLP;
   LwrSrbPolicy             *srb = NULLP;
   U16                      idx = 0;

   for(idx = 0; idx < rabPolicyCfgGrp->numSrbPolicies; idx++)
   {
      srb = &rabPolicyCfgGrp->srbPolicy[idx]; 

      switch(srb->srbId)
      {
         case LWR_SRB_ID_1:
            {
               /* Fill parameters for SRB1                                            */
               rabCfg = &ummCellCb->wrRabCfgParams[WR_SRB1_QCI];
               break;
            }
         case LWR_SRB_ID_2:
            {
               /* Fill parameters for SRB2                                            */
               rabCfg = &ummCellCb->wrRabCfgParams[WR_SRB2_QCI];
               break;
            }
         default:
            {
               RLOG1(L_ERROR, "Unknown SRB ID:[%d]", (srb->srbId));
               RETVOID;
               break;
            }
      } /* end of switch */

      /* Update the common SRB Parameters which are not receiving from OAM */
      rabCfg->qciSupported           = TRUE;
      rabCfg->rlcMode                = WR_RLC_AM_MODE;
      rabCfg->rlcDir                 = WR_DIR_DL_UL;
      rabCfg->pdcpParams.cfmReqrd    = TRUE;

      rabCfg->rlcParams.u.am.ul.statusProhTmr = srb->rlcInfo.timerStatusProhibit;
      rabCfg->rlcParams.u.am.ul.reorderingTmr = srb->rlcInfo.timerReorderingUl;
      rabCfg->rlcParams.u.am.ul.pollByte = srb->rlcInfo.pollByte;
      rabCfg->rlcParams.u.am.ul.pollPdu = srb->rlcInfo.pollPdu; 
      rabCfg->rlcParams.u.am.ul.pollRetxTmr = srb->rlcInfo.tpRetransmit; 
      rabCfg->rlcParams.u.am.ul.maxRetxThrsh = srb->rlcInfo.maxRetransThresh;

      rabCfg->rlcParams.u.am.dl.statusProhTmr = srb->rlcInfo.timerStatusProhibit;
      rabCfg->rlcParams.u.am.dl.reorderingTmr = srb->rlcInfo.timerReorderingDl;
      rabCfg->rlcParams.u.am.dl.pollByte = srb->rlcInfo.pollByte;
      rabCfg->rlcParams.u.am.dl.pollPdu = srb->rlcInfo.pollPdu;
      rabCfg->rlcParams.u.am.dl.pollRetxTmr = srb->rlcInfo.tpRetransmit;
      rabCfg->rlcParams.u.am.dl.maxRetxThrsh = srb->rlcInfo.maxRetransThresh;

      rabCfg->rlcParams.u.am.ul.pbr = srb->lchInfo.pbr;
      rabCfg->rlcParams.u.am.ul.bucketSizeDur = srb->lchInfo.bucketSizeDuration;
      rabCfg->rlcParams.u.am.ul.lcg = srb->lchInfo.lchGrpId;
      rabCfg->lchParams.bitRate = srb->lchInfo.pbr;
      rabCfg->lchParams.bucketSizeDur = srb->lchInfo.bucketSizeDuration;
      rabCfg->lchParams.lchGrp = srb->lchInfo.lchGrpId;

      rabCfg->rlcParams.u.am.ul.priority = srb->lchInfo.priority;
      rabCfg->lchParams.priority = srb->lchInfo.priority;
      /* TODO: QCI Info not used for SRB's currently */
      /*   srb->qciInfo.resourceType;
           srb->qciInfo.priotity;
           srb->qciInfo.pdb;
           srb->qciInfo.pler; */

   } /* end of for to process all Srbs */
} /* end of wrUmmReCfgSrbParams */

PUBLIC S16 wrUmmReCfgRabParams
(
 U16                   cellId,
 LwrRabPolicyCfgGrp    *rabPolicyReCfgGrp
)
{
   WrUmmCellCb               *ummCellCb = NULLP;
   U32                       rCfgType = 0;
   U32                       mask = 1;
   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if (ummCellCb != NULLP)
   { 
      rCfgType =  rabPolicyReCfgGrp->reCfgType;

      while (rCfgType)
      {
         if(rCfgType & mask)
         {
            switch(mask)
            {
               case LWR_CELL_RAB_SRB_POLICY:
                  {
                     rCfgType =  rCfgType ^ mask; 
                     wrUmmReCfgSrbParams(ummCellCb, rabPolicyReCfgGrp);
                     break;
                  }
               case LWR_CELL_RAB_RAB_POLICY:
                  {
                     rCfgType =  rCfgType ^ mask; 
                     wrUmmReCfgDrbParams(ummCellCb, rabPolicyReCfgGrp);
                     break;
                  }
               default:
                  {   
                     rCfgType =  rCfgType ^ mask; 
                     RLOG1(L_ERROR, "Rab Policy Re-config received unknown "
                        "reCfgType [%ld]", (rabPolicyReCfgGrp->reCfgType));

                     break;
                  }
            }
         } /* end of if */
         mask = mask << 1;
      } /* end of while */
   }
   RETVALUE(ROK);
} /* end of wrUmmReCfgRabParams */



PUBLIC S16 wrUmmCfgRabParams
(
 U16                   cellId,
 LwrRabPolicyCfgGrp    *rabPolicyCfgGrp
)
{
   WrUmmCellCb               *ummCellCb = NULLP;
   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if (ummCellCb != NULLP)
   {
      wrUmmUpdateSrbParams(ummCellCb, rabPolicyCfgGrp);
      wrUmmUpdateDrbParams(ummCellCb, rabPolicyCfgGrp);
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}

PUBLIC S16 wrUmmInitRabParams
(
  WrCellCb                      *cellCb
)
{
   WrRabCfgParams            *params = NULLP;
   WrUmmCellCb                  *ummCellCb = NULLP; 

   WR_UMM_GET_CELLCB(ummCellCb, cellCb->cellId);
   if (ummCellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellCb->cellId,
         "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   /* Fill parameters for SRB1                                            */
   params = &ummCellCb->wrRabCfgParams[WR_SRB1_QCI];
   params->qciSupported = TRUE;
   params->rlcMode = WR_RLC_AM_MODE;
   params->rlcDir                        = WR_DIR_DL_UL;
   params->rlcParams.u.am.ul.statusProhTmr = NhuT_StatusProhibitms35Enum;
#if 0
#else
   params->rlcParams.u.am.ul.reorderingTmr = cellCb->rlcCfg.ulModeInfo.srbtReordring;
#endif
   params->rlcParams.u.am.ul.pollByte = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.ul.pollPdu = NhuPollPDUpInfinityEnum;
   params->rlcParams.u.am.ul.pollRetxTmr = cellCb->srbPolicy[0].rlcInfo.tpRetransmit;
   params->rlcParams.u.am.ul.maxRetxThrsh = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->rlcParams.u.am.ul.pbr = cellCb->srbPolicy[0].lchInfo.pbr;
   params->rlcParams.u.am.ul.priority = cellCb->srbPolicy[0].lchInfo.priority;
   params->rlcParams.u.am.ul.bucketSizeDur = cellCb->srbPolicy[0].lchInfo.bucketSizeDuration;
   params->rlcParams.u.am.ul.lcg = cellCb->srbPolicy[0].lchInfo.lchGrpId;
   params->rlcParams.u.am.dl.statusProhTmr = NhuT_StatusProhibitms35Enum;
#if 0
#else
   params->rlcParams.u.am.dl.reorderingTmr = cellCb->rlcCfg.dlModeInfo.srbtReordring;
#endif
   params->rlcParams.u.am.dl.pollByte = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.dl.pollPdu = NhuPollPDUpInfinityEnum;
   params->rlcParams.u.am.dl.pollRetxTmr = cellCb->srbPolicy[0].rlcInfo.tpRetransmit;
   params->rlcParams.u.am.dl.maxRetxThrsh = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->pdcpParams.cfmReqrd             = TRUE;
   params->lchParams.priority      = wrSmDfltPrioInfoTbl.srb[0].lchPriority;
   params->lchParams.bitRate       = wrSmDfltPrioInfoTbl.srb[0].pbr;
   params->lchParams.bucketSizeDur = wrSmDfltPrioInfoTbl.srb[0].bucketSizeDur;
   params->lchParams.lchGrp        = wrSmDfltPrioInfoTbl.srb[0].lchGrp;

   /* Fill parameters for SRB2                                            */
   params = &ummCellCb->wrRabCfgParams[WR_SRB2_QCI];
   params->qciSupported = TRUE;
   params->rlcMode = WR_RLC_AM_MODE;
   params->rlcDir                        = WR_DIR_DL_UL;
   params->rlcParams.u.am.ul.statusProhTmr = NhuT_StatusProhibitms10Enum;
#if 0
#else
   params->rlcParams.u.am.ul.reorderingTmr = cellCb->rlcCfg.ulModeInfo.srbtReordring;
#endif
   params->rlcParams.u.am.ul.pollByte = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.ul.pollPdu = NhuPollPDUpInfinityEnum;
   params->rlcParams.u.am.ul.pollRetxTmr = cellCb->srbPolicy[1].rlcInfo.tpRetransmit;
   params->rlcParams.u.am.ul.maxRetxThrsh = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->rlcParams.u.am.ul.pbr = cellCb->srbPolicy[0].lchInfo.pbr;
   params->rlcParams.u.am.ul.priority = cellCb->srbPolicy[0].lchInfo.priority;
   params->rlcParams.u.am.ul.bucketSizeDur = cellCb->srbPolicy[0].lchInfo.bucketSizeDuration;
   params->rlcParams.u.am.ul.lcg = cellCb->srbPolicy[0].lchInfo.lchGrpId;
   params->rlcParams.u.am.dl.statusProhTmr = NhuT_StatusProhibitms35Enum;
#if 0
#else
   params->rlcParams.u.am.dl.reorderingTmr = cellCb->rlcCfg.dlModeInfo.srbtReordring;
#endif
   params->rlcParams.u.am.dl.pollByte = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.dl.pollPdu = NhuPollPDUpInfinityEnum;
   params->rlcParams.u.am.dl.pollRetxTmr = cellCb->srbPolicy[1].rlcInfo.tpRetransmit;
   params->rlcParams.u.am.dl.maxRetxThrsh = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->pdcpParams.cfmReqrd             = TRUE;
   params->lchParams.priority      = wrSmDfltPrioInfoTbl.srb[1].lchPriority;
   params->lchParams.bitRate       = wrSmDfltPrioInfoTbl.srb[1].pbr;
   params->lchParams.bucketSizeDur = wrSmDfltPrioInfoTbl.srb[1].bucketSizeDur;
   params->lchParams.lchGrp        = wrSmDfltPrioInfoTbl.srb[1].lchGrp;

   /* Fill parameters for QCI value of 1                                  */
   params = &ummCellCb->wrRabCfgParams[1];
   params->qciSupported                    = TRUE;
   params->rlcMode                         = WR_RLC_UM_MODE;
   params->rlcDir                          = WR_DIR_DL_UL;
   params->rlcParams.u.um.ul.snFldLen      = NhuSN_FieldLengthsize5Enum;
   params->rlcParams.u.um.dl.snFldLen      = NhuSN_FieldLengthsize10Enum;
   params->rlcParams.u.um.ul.tReordering   = cellCb->rlcCfg.ulModeInfo.drbtReordring;
   params->rlcParams.u.um.dl.tReordering   = cellCb->rlcCfg.dlModeInfo.drbtReordring;
   params->pdcpParams.pdcpSnSize           = NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum;
   params->pdcpParams.discardTimer         = NhuPDCP_ConfigdiscardTimerms100Enum;
   params->pdcpParams.statusRprtReqd       = TRUE;
   params->pdcpParams.hdrCompress          = 0;
   params->pdcpParams.cfmReqrd             = FALSE;
   params->dscp                            = 0;
   params->lchParams.priority              = wrSmDfltPrioInfoTbl.drb[1].lchPriority;
   params->lchParams.bitRate               = wrSmDfltPrioInfoTbl.drb[1].pbr;
   params->lchParams.bucketSizeDur         = wrSmDfltPrioInfoTbl.drb[1].bucketSizeDur;
   params->lchParams.lchGrp                = wrSmDfltPrioInfoTbl.drb[1].lchGrp;


   /* Fill parameters for QCI value of 2                                  */
   /* This is a GBR bearer and VoIP kind of services use this QCI.        */
   /* TODO: RLC UM Mode is more suitable to this QCI */
   params = &ummCellCb->wrRabCfgParams[2];
   params->qciSupported                    = TRUE;
   params->rlcMode                         = WR_RLC_UM_MODE;
   params->rlcDir                          = WR_DIR_DL_UL;
   params->rlcParams.u.um.ul.snFldLen      = NhuSN_FieldLengthsize5Enum;
   params->rlcParams.u.um.dl.snFldLen      = NhuSN_FieldLengthsize10Enum;
   params->rlcParams.u.um.ul.tReordering   = cellCb->rlcCfg.ulModeInfo.drbtReordring;
   params->rlcParams.u.um.dl.tReordering   = cellCb->rlcCfg.dlModeInfo.drbtReordring;
   params->pdcpParams.pdcpSnSize           = NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum;
   params->pdcpParams.discardTimer         = NhuPDCP_ConfigdiscardTimerms150Enum;
   params->pdcpParams.statusRprtReqd       = TRUE;
   params->pdcpParams.hdrCompress          = 0;
   params->pdcpParams.cfmReqrd             = FALSE;
   params->dscp                            = 0; 
   params->lchParams.priority              = wrSmDfltPrioInfoTbl.drb[2].lchPriority;
   params->lchParams.bitRate               = wrSmDfltPrioInfoTbl.drb[2].pbr;
   params->lchParams.bucketSizeDur         = wrSmDfltPrioInfoTbl.drb[2].bucketSizeDur;
   params->lchParams.lchGrp                = wrSmDfltPrioInfoTbl.drb[2].lchGrp;


   /* Fill parameters for QCI value of 3                                  */
   /* TODO: This is a GBR bearer and is not supported just yet            */
   params = &ummCellCb->wrRabCfgParams[3];
   /* ccpu00125880 */
   params->qciSupported = TRUE;
   params->rlcMode                         = WR_RLC_UM_MODE;
   params->rlcDir                          = WR_DIR_DL_UL;
   params->rlcParams.u.um.ul.snFldLen      = NhuSN_FieldLengthsize5Enum;
   params->rlcParams.u.um.dl.snFldLen      = NhuSN_FieldLengthsize10Enum;
   params->rlcParams.u.um.ul.tReordering   = cellCb->rlcCfg.ulModeInfo.drbtReordring;
   params->rlcParams.u.um.dl.tReordering   = cellCb->rlcCfg.dlModeInfo.drbtReordring;
   params->pdcpParams.pdcpSnSize           = NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum;
   params->pdcpParams.discardTimer         = NhuPDCP_ConfigdiscardTimerms50Enum;
   params->pdcpParams.statusRprtReqd       = TRUE;
   params->pdcpParams.hdrCompress          = 0;
   params->pdcpParams.cfmReqrd             = FALSE;
   params->dscp                            = 0; 
   params->lchParams.priority              = wrSmDfltPrioInfoTbl.drb[3].lchPriority;
   params->lchParams.bitRate               = wrSmDfltPrioInfoTbl.drb[3].pbr;
   params->lchParams.bucketSizeDur         = wrSmDfltPrioInfoTbl.drb[3].bucketSizeDur;
   params->lchParams.lchGrp                = wrSmDfltPrioInfoTbl.drb[3].lchGrp;
   /* ccpu00125880 */

   /* Fill parameters for QCI value of 4                                  */
   /* TODO: This is a GBR bearer and is not supported just yet            */
   params = &ummCellCb->wrRabCfgParams[4];
   /* ccpu00125880 */
   params->qciSupported = TRUE;
   params->rlcMode                         = WR_RLC_UM_MODE;
   params->rlcDir                          = WR_DIR_DL_UL;
   params->rlcParams.u.um.ul.snFldLen      = NhuSN_FieldLengthsize5Enum;
   params->rlcParams.u.um.dl.snFldLen      = NhuSN_FieldLengthsize10Enum;
   params->rlcParams.u.um.ul.tReordering   = cellCb->rlcCfg.ulModeInfo.drbtReordring;
   params->rlcParams.u.um.dl.tReordering   = cellCb->rlcCfg.dlModeInfo.drbtReordring;
   params->pdcpParams.pdcpSnSize           = NhuPDCP_Configrlc_UMpdcp_SN_Sizelen7bitsEnum;
   params->pdcpParams.discardTimer         = NhuPDCP_ConfigdiscardTimerms300Enum;
   params->pdcpParams.statusRprtReqd       = TRUE;
   params->pdcpParams.hdrCompress          = 0;
   params->pdcpParams.cfmReqrd             = FALSE;
   params->dscp                            = 0;
   params->lchParams.priority              = wrSmDfltPrioInfoTbl.drb[4].lchPriority;
   params->lchParams.bitRate               = wrSmDfltPrioInfoTbl.drb[4].pbr;
   params->lchParams.bucketSizeDur         = wrSmDfltPrioInfoTbl.drb[4].bucketSizeDur;
   params->lchParams.lchGrp                = wrSmDfltPrioInfoTbl.drb[4].lchGrp;
   /* ccpu00125880 */

   /* Fill parameters for QCI value of 5                                  */
   params = &ummCellCb->wrRabCfgParams[5];
   params->qciSupported                  = TRUE;
   params->rlcMode                       = WR_RLC_AM_MODE;
   params->rlcDir                        = WR_DIR_DL_UL;
   params->rlcParams.u.am.ul.statusProhTmr = NhuT_StatusProhibitms10Enum;
   params->rlcParams.u.am.ul.reorderingTmr = cellCb->rlcCfg.ulModeInfo.drbtReordring;
   params->rlcParams.u.am.ul.pollByte      = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.ul.pollPdu       = NhuPollPDUp16Enum;
   params->rlcParams.u.am.ul.pollRetxTmr   = NhuT_PollRetransmitms400Enum;
   params->rlcParams.u.am.ul.maxRetxThrsh  = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->rlcParams.u.am.ul.pbr           = cellCb->srbPolicy[0].lchInfo.pbr;
   params->rlcParams.u.am.ul.priority      = cellCb->srbPolicy[0].lchInfo.priority;
   params->rlcParams.u.am.ul.bucketSizeDur = cellCb->srbPolicy[0].lchInfo.bucketSizeDuration;
   params->rlcParams.u.am.ul.lcg           = cellCb->srbPolicy[0].lchInfo.lchGrpId;
   params->rlcParams.u.am.dl.statusProhTmr = NhuT_StatusProhibitms35Enum;
#if 0
#else
   params->rlcParams.u.am.dl.reorderingTmr = cellCb->rlcCfg.dlModeInfo.drbtReordring;
#endif
   params->rlcParams.u.am.dl.pollByte      = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.dl.pollPdu       = NhuPollPDUp8Enum; /* Was NhuPollPDUp16Enum Changed for low UDP throughput */
   params->rlcParams.u.am.dl.pollRetxTmr   = NhuT_PollRetransmitms400Enum;
   params->rlcParams.u.am.dl.maxRetxThrsh  = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->pdcpParams.discardTimer         = NhuPDCP_ConfigdiscardTimerinfinityEnum;
   params->pdcpParams.statusRprtReqd       = TRUE;
   params->pdcpParams.hdrCompress          = 0;
   params->pdcpParams.cfmReqrd             = FALSE;
   params->dscp                            = 0;
   params->lchParams.priority              = wrSmDfltPrioInfoTbl.drb[5].lchPriority;
   params->lchParams.bitRate               = wrSmDfltPrioInfoTbl.drb[5].pbr;
   params->lchParams.bucketSizeDur         = wrSmDfltPrioInfoTbl.drb[5].bucketSizeDur;
   params->lchParams.lchGrp                = wrSmDfltPrioInfoTbl.drb[5].lchGrp;

   /* Fill parameters for QCI value of 6                                  */
   params = &ummCellCb->wrRabCfgParams[6];
   params->qciSupported                  = TRUE;
   params->rlcMode                       = WR_RLC_AM_MODE;
   params->rlcDir                        = WR_DIR_DL_UL;
   params->rlcParams.u.am.ul.statusProhTmr = NhuT_StatusProhibitms10Enum;
   params->rlcParams.u.am.ul.reorderingTmr = NhuT_Reorderingms30Enum;
   params->rlcParams.u.am.ul.pollByte      = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.ul.pollPdu       = NhuPollPDUp16Enum;
   params->rlcParams.u.am.ul.pollRetxTmr   = NhuT_PollRetransmitms400Enum;
   params->rlcParams.u.am.ul.maxRetxThrsh  = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->rlcParams.u.am.ul.pbr           = cellCb->srbPolicy[0].lchInfo.pbr;
   params->rlcParams.u.am.ul.priority      = cellCb->srbPolicy[0].lchInfo.priority;
   params->rlcParams.u.am.ul.bucketSizeDur = cellCb->srbPolicy[0].lchInfo.bucketSizeDuration;
   params->rlcParams.u.am.ul.lcg           = cellCb->srbPolicy[0].lchInfo.lchGrpId;
   params->rlcParams.u.am.dl.statusProhTmr = NhuT_StatusProhibitms40Enum;
#if 0
#else
   params->rlcParams.u.am.dl.reorderingTmr = cellCb->rlcCfg.dlModeInfo.drbtReordring;
#endif
   params->rlcParams.u.am.dl.pollByte      = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.dl.pollPdu       = NhuPollPDUp16Enum;
   params->rlcParams.u.am.dl.pollRetxTmr   = NhuT_PollRetransmitms400Enum;
   params->rlcParams.u.am.dl.maxRetxThrsh  = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->pdcpParams.discardTimer         = NhuPDCP_ConfigdiscardTimerinfinityEnum;
   params->pdcpParams.statusRprtReqd       = TRUE;
   params->pdcpParams.hdrCompress          = 0;
   params->pdcpParams.cfmReqrd             = FALSE;
   params->dscp                            = 0;
   params->lchParams.priority              = wrSmDfltPrioInfoTbl.drb[6].lchPriority;
   params->lchParams.bitRate               = wrSmDfltPrioInfoTbl.drb[6].pbr;
   params->lchParams.bucketSizeDur         = wrSmDfltPrioInfoTbl.drb[6].bucketSizeDur;
   params->lchParams.lchGrp                = wrSmDfltPrioInfoTbl.drb[6].lchGrp;

   /* Fill parameters for QCI value of 7                                  */
   params = &ummCellCb->wrRabCfgParams[7];
   params->qciSupported                  = TRUE;
   params->rlcMode                       = WR_RLC_AM_MODE;
   params->rlcDir                        = WR_DIR_DL_UL;
   params->rlcParams.u.am.ul.statusProhTmr = NhuT_StatusProhibitms35Enum;
#if 0
#else
   params->rlcParams.u.am.ul.reorderingTmr = cellCb->rlcCfg.ulModeInfo.drbtReordring;
#endif
   params->rlcParams.u.am.ul.pollByte      = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.ul.pollPdu       = NhuPollPDUp16Enum;
   params->rlcParams.u.am.ul.pollRetxTmr   = NhuT_PollRetransmitms400Enum;
   params->rlcParams.u.am.ul.maxRetxThrsh  = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->rlcParams.u.am.ul.pbr           = cellCb->srbPolicy[0].lchInfo.pbr;
   params->rlcParams.u.am.ul.priority      = cellCb->srbPolicy[0].lchInfo.priority;
   params->rlcParams.u.am.ul.bucketSizeDur = cellCb->srbPolicy[0].lchInfo.bucketSizeDuration;
   params->rlcParams.u.am.ul.lcg           = cellCb->srbPolicy[0].lchInfo.lchGrpId;
#if 0
#else
   params->rlcParams.u.am.dl.statusProhTmr = NhuT_StatusProhibitms5Enum;
#if 0
#else
   params->rlcParams.u.am.dl.reorderingTmr = cellCb->rlcCfg.dlModeInfo.drbtReordring;
#endif
#endif
   params->rlcParams.u.am.dl.pollByte      = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.dl.pollPdu       = NhuPollPDUp16Enum;
   params->rlcParams.u.am.dl.pollRetxTmr   = NhuT_PollRetransmitms400Enum;
   params->rlcParams.u.am.dl.maxRetxThrsh  = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->pdcpParams.discardTimer         = NhuPDCP_ConfigdiscardTimerinfinityEnum;
   params->pdcpParams.statusRprtReqd       = TRUE;
   params->pdcpParams.hdrCompress          = 0;
   params->pdcpParams.cfmReqrd             = FALSE;
   params->dscp                            = 0;
   params->lchParams.priority              = wrSmDfltPrioInfoTbl.drb[7].lchPriority;
   params->lchParams.bitRate               = wrSmDfltPrioInfoTbl.drb[7].pbr;
   params->lchParams.bucketSizeDur         = wrSmDfltPrioInfoTbl.drb[7].bucketSizeDur;
   params->lchParams.lchGrp                = wrSmDfltPrioInfoTbl.drb[7].lchGrp;

   /* Fill parameters for QCI value of 8                                  */
   params = &ummCellCb->wrRabCfgParams[8];
   params->qciSupported                  = TRUE;
   params->rlcMode                       = WR_RLC_AM_MODE;
   params->rlcDir                        = WR_DIR_DL_UL;
   params->rlcParams.u.am.ul.statusProhTmr = NhuT_StatusProhibitms35Enum;
#if 0
#else
   params->rlcParams.u.am.ul.reorderingTmr = cellCb->rlcCfg.ulModeInfo.drbtReordring;
#endif
   params->rlcParams.u.am.ul.pollByte      = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.ul.pollPdu       = NhuPollPDUp16Enum;
   params->rlcParams.u.am.ul.pollRetxTmr   = NhuT_PollRetransmitms400Enum;
   params->rlcParams.u.am.ul.maxRetxThrsh  = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->rlcParams.u.am.ul.pbr           = cellCb->srbPolicy[0].lchInfo.pbr;
   params->rlcParams.u.am.ul.priority      = cellCb->srbPolicy[0].lchInfo.priority;
   params->rlcParams.u.am.ul.bucketSizeDur = cellCb->srbPolicy[0].lchInfo.bucketSizeDuration;
   params->rlcParams.u.am.ul.lcg           = cellCb->srbPolicy[0].lchInfo.lchGrpId;
   params->rlcParams.u.am.dl.statusProhTmr = NhuT_StatusProhibitms35Enum;
#if 0
#else
   params->rlcParams.u.am.dl.reorderingTmr = cellCb->rlcCfg.dlModeInfo.drbtReordring;
#endif
   params->rlcParams.u.am.dl.pollByte      = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.dl.pollPdu       = NhuPollPDUp16Enum;
   params->rlcParams.u.am.dl.pollRetxTmr   = NhuT_PollRetransmitms400Enum;
   params->rlcParams.u.am.dl.maxRetxThrsh  = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->pdcpParams.discardTimer         = NhuPDCP_ConfigdiscardTimerinfinityEnum;
   params->pdcpParams.statusRprtReqd       = TRUE;
   params->pdcpParams.hdrCompress          = 0;
   params->pdcpParams.cfmReqrd             = FALSE;
   params->dscp                            = 0;
   params->lchParams.priority              = wrSmDfltPrioInfoTbl.drb[8].lchPriority;
   params->lchParams.bitRate               = wrSmDfltPrioInfoTbl.drb[8].pbr;
   params->lchParams.bucketSizeDur         = wrSmDfltPrioInfoTbl.drb[8].bucketSizeDur;
   params->lchParams.lchGrp                = wrSmDfltPrioInfoTbl.drb[8].lchGrp;

   /* Fill parameters for QCI value of 9                                  */
   params = &ummCellCb->wrRabCfgParams[9];
   params->qciSupported                  = TRUE;
   params->rlcMode                       = WR_RLC_AM_MODE;
   params->rlcDir                        = WR_DIR_DL_UL;
   params->rlcParams.u.am.ul.statusProhTmr = NhuT_StatusProhibitms35Enum;
#if 0
#else
   params->rlcParams.u.am.ul.reorderingTmr = cellCb->rlcCfg.ulModeInfo.drbtReordring;
#endif
   params->rlcParams.u.am.ul.pollByte      = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.ul.pollPdu       = NhuPollPDUp16Enum;
   params->rlcParams.u.am.ul.pollRetxTmr   = NhuT_PollRetransmitms400Enum;
   params->rlcParams.u.am.ul.maxRetxThrsh  = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->rlcParams.u.am.ul.pbr           = cellCb->srbPolicy[0].lchInfo.pbr;
   params->rlcParams.u.am.ul.priority      = cellCb->srbPolicy[0].lchInfo.priority;
   params->rlcParams.u.am.ul.bucketSizeDur = cellCb->srbPolicy[0].lchInfo.bucketSizeDuration;
   params->rlcParams.u.am.ul.lcg           = cellCb->srbPolicy[0].lchInfo.lchGrpId;
   params->rlcParams.u.am.dl.statusProhTmr = NhuT_StatusProhibitms35Enum;
#if 0
#else
   params->rlcParams.u.am.dl.reorderingTmr = cellCb->rlcCfg.dlModeInfo.drbtReordring;
#endif
   params->rlcParams.u.am.dl.pollByte      = NhuPollBytekBinfinityEnum;
   params->rlcParams.u.am.dl.pollPdu       = NhuPollPDUp16Enum;
   params->rlcParams.u.am.dl.pollRetxTmr   = NhuT_PollRetransmitms400Enum;
   params->rlcParams.u.am.dl.maxRetxThrsh  = NhuUL_AM_RLCmaxRetxThresholdt8Enum;
   params->pdcpParams.discardTimer         = NhuPDCP_ConfigdiscardTimerinfinityEnum;
   params->pdcpParams.statusRprtReqd       = TRUE;
   params->pdcpParams.cfmReqrd             = FALSE;
   params->pdcpParams.hdrCompress          = 0;
   params->dscp                            = 0;
   params->lchParams.priority              = wrSmDfltPrioInfoTbl.drb[9].lchPriority;
   params->lchParams.bitRate               = wrSmDfltPrioInfoTbl.drb[9].pbr;
   params->lchParams.bucketSizeDur         = wrSmDfltPrioInfoTbl.drb[9].bucketSizeDur;
   params->lchParams.lchGrp                = wrSmDfltPrioInfoTbl.drb[9].lchGrp;
   RETVALUE(ROK);
} /* end of wrUmmInitRabParams */



/** @brief This function is for allocatting the available Cell Control Block
 *
 * @details
 *
 *     Function:wrUmmGetNewCellCb 
 *
 *         Processing steps:
 *         - Interate the Umm Cell Cb.
 *         - if any cell cb is free allocate the cell cb
 *
 *
 * @param   cellCb: Cell Control Block
 * @return S16
 *          -# ROK
 *          -# RFAILED
 */

PRIVATE S16 wrUmmGetNewCellCb
(
WrUmmCellCb                  **cellCb
)
{
   U8                        cellCount = 0;
   S16                       ret = RFAILED;

   for (cellCount = 0; cellCount < WR_MAX_CELLS; cellCount++)
   {
      if (wrUeDb.cells[cellCount].isFree == FALSE)
      {
         wrUeDb.cells[cellCount].isFree = TRUE;
         *cellCb = &(wrUeDb.cells[cellCount]);
         ret = ROK;
         break;
      }
   }
   RETVALUE(ret); 
}

/* SPS changes starts */
/** @brief This function is called by EMM to create a SPS RNTI DB when a cell is
 * configured. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Find the minimum and create DB
 *
 * @param cellId:  Cell Index
 * @param cfgAdd:  Cell Config received
 * @param grpIndx: Schedular group index
 *
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmCreateSpsRntiDb
(
WrCellId                     cellId,
LwrAddCellCfg                *cfgAdd,
U32                          grpIndx
)
{
   U16                       maxSpsUeSupport = 0;
   S16                       ret = ROK;
   WrRntinode                *rntiNode = NULLP;
   WrUmmCellCb               *cellCb = NULLP;
   U32                       rnti = 0;
   U32                       idx = 0;

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "Cell Cb is NULL ");
      RETVALUE(RFAILED);
   }

   if (grpIndx < WR_CELL_CFG_MAX_GRPS)
   {
      cellCb->spsRntiDb.rntiStart = (U16)(cellCb->rntiDb.rntiStart + cellCb->rntiDb.maxRrmRntis + WR_RNTI_GAP);
      
      ret = wrEmmGetMaxUeSupport(cellId, &maxSpsUeSupport);
      if ((ret != ROK) || (maxSpsUeSupport == 0))
      {
         maxSpsUeSupport = (U16)(cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.spsCfg.maxDlSpsUePerTti * RGR_SPS_PRD_20SF); /* Select UL or DL */
      }

      if (cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.spsCfg.maxDlSpsUePerTti < cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.spsCfg.maxUlSpsUePerTti)
      {
         if (maxSpsUeSupport > (cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.spsCfg.maxDlSpsUePerTti * RGR_SPS_PRD_20SF))
         {
            maxSpsUeSupport = (U16)(cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.spsCfg.maxDlSpsUePerTti * RGR_SPS_PRD_20SF);
         }
      }
      else
      {
         if (maxSpsUeSupport > (cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.spsCfg.maxUlSpsUePerTti * RGR_SPS_PRD_20SF))
         {
            maxSpsUeSupport = (U16)(cfgAdd->cellCfgGrp[grpIndx]->t.schdCfgGrp.spsCfg.maxUlSpsUePerTti * RGR_SPS_PRD_20SF);
         }
      }
      cellCb->spsRntiDb.maxRrmRntis = (U16)(maxSpsUeSupport + WR_DELTA_SPS_RNTI);

      cmLListInit(&(cellCb->spsRntiDb.freeRntiLst));
      rnti = cellCb->spsRntiDb.rntiStart;
      for (idx = 0; idx < cellCb->spsRntiDb.maxRrmRntis; idx++)
      {
         WR_ALLOC(&rntiNode, sizeof(WrRntinode));
         if(NULLP == rntiNode)
         {
            while (NULLP != cellCb->spsRntiDb.freeRntiLst.first)
            {
               rntiNode = (WrRntinode *)cmLListDelFrm(&cellCb->spsRntiDb.freeRntiLst, cellCb->spsRntiDb.freeRntiLst.first);
               WR_FREE(rntiNode, sizeof(WrRntinode));
            }
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         rntiNode->rnti = (U16)(rnti++);
         rntiNode->lnk.node = (PTR)rntiNode;
         cmLListAdd2Tail(&cellCb->spsRntiDb.freeRntiLst, &rntiNode->lnk);
      }/* End of for() */
   }
   RETVALUE(ROK);
}
/* SPS changes ends */

/* ********************************************************************** */
/** @brief This function is called by EMM to create a cell when a cell is
 * configured. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - If cellId Exceeds WR_MAX_CELLS Return RFAILED
 *         - Loop for the cellCb->maxUesPerCell and intialize the array of UE
 *         control blocks within this cellCb.
 *
 * @param cellId: Cell Index
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmAddCell
(
WrCellId                     cellId,
LwrAddCellCfg                *cfgAdd
)
{
   U32                       idx;
   WrUmmCellCb               *cellCb = NULLP;
   WrRntinode                *rntiNode;
   WrPrmblnode               *prmblNode;
   U32                       rnti;
   U32                       prmbl;
   /* ccpu00132701 */
   U8                        grpIndxCrnti = 0;
   U8                        grpIndxPrmbl = 0;
   U8                        grpIndxRab = 0;
   /* CSG_DEV start */
   S16                       grpIndx = 0;
   /* CSG_DEV end */

   if( wrUmmGetNewCellCb (&cellCb) != ROK)
   {
      RLOG0(L_ERROR, "Cell Cb is not available ");
      RETVALUE(RFAILED);
   }
   cellCb->cellId = cellId;
   if(cellCb->measCellCfg == NULLP)
   {
      WR_ALLOC(&cellCb->measCellCfg, sizeof(LwrCellMeasCfgGrp));
      if(NULLP == cellCb->measCellCfg)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
   }
   if(cellCb->measCellCb == NULLP)
   {
      WR_ALLOC(&cellCb->measCellCb, sizeof(WrUmmMeasCellCb));
      if(NULLP == cellCb->measCellCb)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
   }

   cellCb->cellState = WR_CELL_STATE_INIT; 
   /* Initialize the RNTI DB */

   /* Traversing the List of CellCfgGrp to fetch Cell Grp RGR which contains Dedicated rnti */
   /* ccpu00132701 */
   while(cfgAdd->cellCfgGrp[grpIndxCrnti]->grpType != WR_CELL_CFG_GRP_RGR)
   {
      grpIndxCrnti++;
   }
   cellCb->rntiDb.rntiStart =
      (cfgAdd->cellCfgGrp[grpIndxCrnti]->t.rgrCfgGrp.dfltStartRnti +
          cfgAdd->cellCfgGrp[grpIndxCrnti]->t.rgrCfgGrp.dfltRntiSize);


   /* Initialize the RNTI DB                                              */

   /* Traversing the List of CellCfgGrp to fetch Cell Grp RRM which contains Dedicated prmbl */
   /* ccpu00132701 */
   while(cfgAdd->cellCfgGrp[grpIndxPrmbl]->grpType != WR_CELL_CFG_GRP_RRM)
   {
      grpIndxPrmbl++;
   }
   cellCb->maxUesPerCell = cfgAdd->cellCfgGrp[grpIndxPrmbl]->t.rrmCfgGrp.enbCacInfo.maxEnbCallCount + WR_MAX_SR_CQI_NOT_CONFIGURED_UES; 
   for (idx = 0; idx < cellCb->maxUesPerCell; idx++) 
   { 
      cellCb->ueCbs[idx] = NULLP; 
   } 

   /* ccpu00145343: Initialize last deleted UeIdx to 0xFF */ 
   cellCb->lastDelUeIdx = 0xFF;
   /* Initialize the Guard timer */
   cmInitTimers(&(cellCb->ueCtxRelGuardTmr), 1);

   cellCb->prmblDb.prmblStart =
      cfgAdd->cellCfgGrp[grpIndxPrmbl]->t.rrmCfgGrp.enbCacInfo.dfltRrmStartPreamble;
   cellCb->prmblDb.maxPrmbls =
      cfgAdd->cellCfgGrp[grpIndxPrmbl]->t.rrmCfgGrp.enbCacInfo.dfltRrmNumPreamble;
   cmLListInit(&(cellCb->prmblDb.freePrmblLst));
   cmLListInit(&(cellCb->prmblDb.inUsePrmblLst));
   prmbl = cellCb->prmblDb.prmblStart;

   /* Filling only free preamble list as the inuse preamble list will
      get filled as and when it's get assigned */
   for (idx = 0; idx < cellCb->prmblDb.maxPrmbls; idx++)
   {
      WR_ALLOC(&prmblNode, sizeof(WrPrmblnode));
      if (NULLP == prmblNode)
      {
         RLOG0(L_FATAL, "Memory allocation failed");
         RETVALUE(RFAILED);
      }
      prmblNode->prmblId = prmbl++;
      prmblNode->lnk.node = (PTR)prmblNode;
      cmLListAdd2Tail(&cellCb->prmblDb.freePrmblLst, &prmblNode->lnk);
   }/* End of for() */



   /* ccpu00133711: Moved the dedicated RNTI list building down as the maxRrmRnti is 
    * configured with groupType GRP_RRM 
    */
   cellCb->rntiDb.maxRrmRntis =
      cfgAdd->cellCfgGrp[grpIndxPrmbl]->t.rrmCfgGrp.enbCacInfo.dfltRrmRntiSize; 
   cmLListInit(&(cellCb->rntiDb.freeRntiLst));
   cmLListInit(&(cellCb->rntiDb.inUseRntiLst));
   rnti = cellCb->rntiDb.rntiStart;

   /* Filling only free RNTI list as the inuse RNTI list will
      get filled as and when it's get assigned */
   for (idx = 0; idx < cellCb->rntiDb.maxRrmRntis; idx++)
   {
      WR_ALLOC(&rntiNode, sizeof(WrRntinode));
      if(NULLP == rntiNode)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      rntiNode->rnti = rnti++;
      rntiNode->lnk.node = (PTR)rntiNode;
      cmLListAdd2Tail(&cellCb->rntiDb.freeRntiLst, &rntiNode->lnk);
   }/* End of for() */

   /* CSG_DEV start */
   for (grpIndx = 0; grpIndx < cfgAdd->numOfGrps; grpIndx++)
   {
      if (WR_CELL_CFG_GRP_SIB1 == cfgAdd->cellCfgGrp[grpIndx]->grpType)
      {
         cellCb->csgId  = cfgAdd->cellCfgGrp[grpIndx]->t.sib1CfgGrp.
            cellAccessInfo.csgIdentity.val;
         break;
      }
   }
   cellCb->csgAccessMode = cfgAdd->csgAccessMode;
   /* CSG_DEV end */
   while(cfgAdd->cellCfgGrp[grpIndxRab]->grpType != WR_CELL_CFG_GRP_RAB_PARAMS)
   {
      grpIndxRab++;
   }
   cellCb->rabArpForEmerServ = cfgAdd->cellCfgGrp[grpIndxRab]->t.rabPolicyCfgGrp.arpForEmergencyServ;
   
   RETVALUE(ROK);
}

/** @brief This function is called by EMM to create a list of LAI's 
 *         when a cell is configured.This list will be used for KPIs
 *
 * @details
 *
 *     Function: wrUmmAddKPILaiList 
 *
 *
 * @param cellCb: Cell control block 
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmAddKPILaiList
(
WrCellId      cellId,
WrPlmnId      plmnId, 
U16           lac
)
{
   WrUmmCellCb          *ummCellCb = NULLP;
   U32                  idx = 0;

   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if(ummCellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "Cell Cb is NULL ");
      RETVALUE(RFAILED);
   }
   for(idx = 0; idx < LWR_MAX_NUM_MBLTY_LAI; idx++)
   {
      /* no matching LAI found, so copy it in first available slot */ 
      if(ummCellCb->lstLai[idx].pres != PRSNT_NODEF)
      {
         ummCellCb->lstLai[idx].pres = PRSNT_NODEF;
         ummCellCb->lstLai[idx].lac = lac;
         cmMemcpy((U8 *)&ummCellCb->lstLai[idx].plmnId, (U8 *)&plmnId, sizeof(WrPlmnId));
         break;
      }
      /* already there is a same LAI, so break it */
      else if(((TRUE == wrUtlPlmnsEqual((WrPlmnId *)&plmnId, 
                     (WrPlmnId *)&ummCellCb->lstLai[idx].plmnId))
               && (lac == ummCellCb->lstLai[idx].lac)))
      {
         break;
      }
   }
   RETVALUE(ROK);
}

/** @brief This function intializes the UMM module 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - For all all Transaction type initalize the function pointer to a
 *         common dummy function. 
 *
 * @return Void
 */
PUBLIC Void wrUmmInit
(
Void
)
{
   U32                       idx;
   for(idx = 0; idx < WR_UMM_MAX_TRANS_TYPE; idx++)
   {
      wrUmmTransAllowMsg[idx] = wrUmmDummyTransAllowMsg;
   }
}
/** @brief This function expiry of UE context release guard timer 
 *
 * @details 
 *    If the noUeCntxToRel is non zero, then delete the UE CB and
 *    proceed with power down
 *
 *     Function: 
 *
 *         Processing steps:
 *         -If the noUeCntxToRel is non zero, then delete the UE CB and
 *
 *  @param[in] cellCb    : ummCellCb
 *  @return  Void
 */
PUBLIC Void wrUmmUeCtxRelGrdTmrExp(WrUmmCellCb *cellCb)
{
   U32 idx = 0;

   ALARM("wrUmmUeCtxRelGrdTmrExp: GUARD timer expired\n");
   if((WR_CELL_STATE_PWR_DOWN == cellCb->cellState) &&
      (cellCb->noUeCntxToRel > 0 ))
   {
       for (idx = 0; idx < cellCb->maxUesPerCell; idx++)
       {
          if (cellCb->ueCbs[idx] != NULLP)
          {
             /* Release the UE CB */
             wrUmmRelUeCb(cellCb->ueCbs[idx]);
          }
       }
       /* Triigered PWR down if still noUeCntxToRel is non ZERO,
        * This to take care if any failure while 
        * releasing the UeCB  */
       if ( cellCb->noUeCntxToRel > 0 )
       {

         ALARM("wrUmmUeCtxRelGrdTmrExp: noUeCntxtToRel is non ZERO(%d)\n", 
               cellCb->noUeCntxToRel);
         cellCb->noUeCntxToRel = 0;
         RLOG0(L_DEBUG,"Bringing the CELL POWER DOWN inside wrUmmDelUe");
         wrEmmMmeBrngCellPwrDown();
         /* ccpu00142992:[BCM][GA4.0][TDD][S1AP]TTIs are not coming after
          *                * s1-close triggered from MME when UE is in attached state
          *                               */
         wrEmmCallMmeSetup();
      }
   }
}/* wrUmmUeCtxRelGrdTmrExp */

/** @brief This function Handles the Reset request recieved from MME
 *
 * @details 
 *    Handles Full reset i.e. all the UEs connected to this MME must be
 *    released.
 *
 *     Function: 
 *
 *         Processing steps:
 *         - If MME context is not found return FAILED
 *         - Loop for all the UE's having context with this MME and initiate
 *         local release procedure for each UE.
 *
 *  @param[in] mmeId    : MME Id from which reset recvd
 *  @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrUmmMmeFullRst
(
WrMmeId                      mmeId
)
{
   WrUmmMmeCb                *ummMme;
   WrUeCb                    *ueCb = NULLP;
   CmLList                   *node = NULLP;
   U8                        noUeCntxToRel = 0;
   ummMme = WR_UMM_GET_MME(mmeId);
   if(ummMme == NULLP)
   {
      RETVALUE(RFAILED);
   }

   node = ummMme->ueLst.first;
   while(node != NULLP)
   {
      noUeCntxToRel +=1;
      ueCb = (WrUeCb *)(node->node);
      /* This is requred to keep count the
       * total number of UE release which are present 
       * before reset triggered */ 
      ueCb->tobeReleased = TRUE;
      node = node->next;
      if(ueCb->s1ConCb != NULLP)
      {
         WR_FREE(ueCb->s1ConCb, sizeof(WrS1ConCb));
         ueCb->s1ConCb = NULLP;
      }
      wrUmmPrcLclCtxtRel(ueCb,NhuRlsCauseotherEnum);
   }
   /* Start Ctxt release Guard timer */
   if ( noUeCntxToRel != 0 )
   {
      wrStartTmr((PTR)&(wrUeDb.cells[0]), WR_TMR_GUARD_UE_CTX_REL, 
            WR_TMR_GUARD_UE_CTX_REL_TMR_VAL);
   }
  
   /* Update the number of Ue Context to be 
    * Release in the WrUmmCellCb so that 
    * after all the UE Context are release 
    * Cell Reset can be triggered 
    * Currently only one cell is considered 
    * so index is 0
    */
   wrUeDb.cells[0].noUeCntxToRel = noUeCntxToRel;  
   if((WR_CELL_STATE_RESET == wrUeDb.cells[0].cellState) &&
      (noUeCntxToRel == 0))
   {
      /* Set the state to WR_UE_RELEASE_DONE so the Cell Reset can be triggered */ 
      wrCb.resetCmfState |= WR_UE_RELEASE_DONE;
      wrCb.parResetCmfState |= WR_UE_RELEASE_DONE;
   }
   RETVALUE(ROK);
} /* wrUmmMmeFullRst */



/** @brief This function Handles the Partial Reset request just before
 * triggering the RESET request to MME.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Check if MME context is present if NOT then return RFAILED
 *         - Loop through all the UEs connected to the MME
 *           - Loop through the UE-IDs in the MME Reset message 
 *           - If Match == TRUE 
 *           -  Issue Local context release 
 *           -  else
 *           -  Continue
 *
 *  @param[in] mmeId    : MME Id from which reset recvd
 *  @param[in] conId    : List of ConId's
 *  @param[in] nmbCons  : numbers of UE's to be reset
 *  @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */

PUBLIC S16 wrUmmMmePartRst
(
WrMmeId                      mmeId,
WrEmmS1ConId                *conId,
U32                          nmbCons 
)
{
   WrUmmMmeCb                *ummMme;
   WrUeCb                    *ueCb = NULLP;
   CmLList                   *tmpLlist = NULLP;
   U32                       ueIdx;
   U32                       mmeUeId;
   U32                       enbUeId;

   ummMme = WR_UMM_GET_MME(mmeId);
   if (ummMme == NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Here go through all the UE list which are associated with MME and 
    * release the connection for those UE whose connection IDs are present.
    * As a part of S1-Reset, we may receive either eNB UE S1AP ID or 
    * MME UE S1AP ID. Hence, it is requied to validate the ID present and
    * release the connection based on what is received */
   for(ueCb = (WrUeCb *)CM_LLIST_FIRST_NODE(&(ummMme->ueLst), tmpLlist);
       (ueCb != NULLP);
       ueCb = (WrUeCb *)CM_LLIST_NEXT_NODE(&(ummMme->ueLst), tmpLlist))
   {
      /* fix for ccpu00149035: If s1ConCb is not allocated/NULL, then 
       * continue */
      if ( NULLP == ueCb->s1ConCb )
      {
         RLOG_ARG0(L_DEBUG, DBG_CRNTI, ueCb->crnti, 
               "ueCb->s1ConCb is NULL");
         continue;
      }
      mmeUeId = ueCb->s1ConCb->mme_ue_s1ap_id;
      enbUeId = ueCb->s1ConCb->enb_ue_s1ap_id;

      for(ueIdx = 0; ueIdx < nmbCons; ueIdx++)
      {
        if ( conId[ueIdx].type == WR_S1AP_ENB_ID )
        {
          if(conId[ueIdx].conId == enbUeId)
          {
             wrUmmPrcLclCtxtRel(ueCb,NhuRlsCauseotherEnum);
          }
        }
        else
        {
          if(conId[ueIdx].conId == mmeUeId)
          {
               wrUmmPrcLclCtxtRel(ueCb,NhuRlsCauseotherEnum);
          }
         } /* End of UE id present */
      } /* End of for loop for nmbCons */
   } /* End of UE list for loop */

   RETVALUE(ROK);

} /* wrUmmMmePartRst */

/** @brief This function Handles the Partial Reset request recieved from MME
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Check if MME context is present if NOT then return RFAILED
 *         - Loop through all the UEs connected to the MME
 *           - Loop through the UE-IDs in the MME Reset message 
 *           - If Match == TRUE 
 *           -  Issue Local context release 
 *           -  else
 *           -  Continue
 *
 *  @param[in] mmeId    : MME Id from which reset recvd
 *  @param[in] tknIE     : Pointer to IE which contains UE
 *  @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrUmmMmePartInRst
(
WrMmeId                      mmeId,
SztResetTyp                  *tknIE
)
{
   WrUmmMmeCb                *ummMme;
   WrUeCb                    *ueCb = NULLP;
   CmLList                   *tmpLlist = NULLP;
   U32                       ueIdx;
   U32                       nmbCons;
   U32                       mmeUeId;
   U32                       enbUeId;
   U32                       rcvdMmeUeId;
   U32                       rcvdEnbUeId;

   ummMme = WR_UMM_GET_MME(mmeId);
   if (ummMme == NULLP)
   {
      RETVALUE(RFAILED);
   }

   /* Here go through all the UE list which are associated with MME and 
    * release the connection for those UE whose connection IDs are present.
    * As a part of S1-Reset, we may receive either eNB UE S1AP ID or 
    * MME UE S1AP ID. Hence, it is requied to validate the ID present and
    * release the connection based on what is received */
   for(ueCb = (WrUeCb *)CM_LLIST_FIRST_NODE(&(ummMme->ueLst), tmpLlist);
       (ueCb != NULLP);
       ueCb = (WrUeCb *)CM_LLIST_NEXT_NODE(&(ummMme->ueLst), tmpLlist))
   {
      /* fix for ccpu00149035: If s1ConCb is not allocated/NULL, then 
       * continue */
      if ( NULLP == ueCb->s1ConCb )
      {
         RLOG_ARG0(L_DEBUG, DBG_CRNTI, ueCb->crnti, 
               "ueCb->s1ConCb is NULL");
         continue;
      }

      mmeUeId = ueCb->s1ConCb->mme_ue_s1ap_id;
      enbUeId = ueCb->s1ConCb->enb_ue_s1ap_id;

      nmbCons = ((SztResetTyp *)tknIE)->val.partOfS1_Intf.noComp.val;

      for(ueIdx = 0; ueIdx < nmbCons; ueIdx++)
      {
         if(((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[ueIdx].value.\
              u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID.pres != NOTPRSNT)
         {
            rcvdEnbUeId = ((SztResetTyp *)tknIE)->val.partOfS1_Intf.\
                           member[ueIdx].value.u.sztUE_assocLogS1_ConItem.\
                           eNB_UE_S1AP_ID.val;
            if(rcvdEnbUeId == enbUeId)
            {
               wrUmmPrcLclCtxtRel(ueCb,NhuRlsCauseotherEnum);
            }
         }
         else if(((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[ueIdx].\
               value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres != NOTPRSNT)
         {

            rcvdMmeUeId = ((SztResetTyp *)tknIE)->val.partOfS1_Intf.\
                          member[ueIdx].value.u.sztUE_assocLogS1_ConItem.\
                          mME_UE_S1AP_ID.val;
            if(rcvdMmeUeId == mmeUeId)
            {
               wrUmmPrcLclCtxtRel(ueCb,NhuRlsCauseotherEnum);
            } /* End of UE id matching */
         } /* End of UE id present */
      } /* End of for loop for nmbCons */
   } /* End of UE list for loop */

   RETVALUE(ROK);

} /* wrUmmMmePartInRst */
/* RLC_DL_MAX_RETX fix */
/** @brief This function is used to handle the Datcfm came from PDCP.
 *
 * @details
 *
 *     Function: wrUmmPdcpDatCfm
 *
 *         Processing steps:
 *
 * @param[in]  pdcpId: pointer to CmLtePdcpId structure
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16  wrUmmPdcpDatCfm
(
CmLtePdcpId pdcpId
)
{
   
   TRC2(wrUmmPdcpDatCfm);
 
   /* As of now PdcpDatCfm in DRB comes only if there is a RLF failure */
   /* Fix for the CR ccpu00125242 */
   if (wrUmmSndCtxtRelReq(pdcpId.ueId, pdcpId.cellId, \
        WR_UMM_CTXT_RLF_REL) != ROK)
   {
      /* Log error */
      RLOG0(L_ERROR, "Sending wrUmmSndCtxtRelReq Failed");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of  wrUmmPdcpDatCfm*/


/** @brief This function is responsible for sending context release request 
 *
 * @details
 *
 *     Function: wrUmmSndCtxtRelReq
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] crnti : CRNTI for the UE
 * @param [in] cellId: Cell Identity
 * @param [in] event : Event values could be due to Inactivity
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmSndCtxtRelReq
(
U16                          crnti,
U16                          cellId, 
U8                           event
)
{
   WrUeCb                    *ueCb;
   S1apPdu                   *ctxtRelReqPdu;
   WrUmmMsgCause             cause;
   U8                        cntr1;

   wrUmmGetUe(&ueCb, cellId, crnti);
   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CRNTI, (U32)crnti, "Error in getting UECB for "
         "cellId [%ld]", (U32)cellId);
      RETVALUE(RFAILED);
   }
   switch(event)
   {
      case WR_UMM_UE_LOST:
      {
         cause.causeTyp = WR_CAUSE_RADIONW;
         cause.causeVal = SztCauseRadioNwradio_connection_with_ue_lostEnum; 
      }
      break;
      case WR_ENB_INIT_UE_REL:
         {
            cause.causeTyp = WR_CAUSE_RADIONW;
            /*ccpu00130220*/
            if(ueCb->hoType == WR_HO_TYPE_X2)
            {
               cause.causeVal = SztCauseRadioNwtx2relocoverall_expiryEnum;
            }
            else
            {
               cause.causeVal = SztCauseRadioNwtS1relocoverall_expiryEnum;
            }
            /*ccpu00130220*/
            break;
         }
      case WR_UMM_CTXT_REL_INACTIVITY:
         {
            cause.causeTyp = WR_CAUSE_RADIONW;
            cause.causeVal = SztCauseRadioNwuser_inactivityEnum;
            break;
         }
      case WR_UMM_CTXT_CSFB_REL:
         {
            cause.causeTyp = WR_CAUSE_RADIONW;
            if((ueCb->csfbInfo.ratType == CDMA2000_1XRTT) && 
               ((ueCb->csfbInfo.redirType == CDMA2000_CSFB_WITHOUT_PS_HO) ||
                (ueCb->csfbInfo.redirType == CDMA2000_CSFB_WITH_REDIRECTION) ||
                (ueCb->csfbInfo.redirType == CDMA2000_CSFB_WITHOUT_REDIRECTION)))
            {
               cause.causeVal = SztCauseRadioNwredirection_towards_1xRTTEnum;
            }
            else if((GERAN == ueCb->csfbInfo.ratType) && 
                  ((FALSE == ueCb->csfbInfo.ueDTMCap) ||
                  (FALSE == ueCb->csfbInfo.nwDTMCap)) &&
                  (GERAN_CSFB_WITHOUT_REDIRECTION != ueCb->csfbInfo.redirType))
            {
               /* If either Target GERAN neighbour or UE is DTM incapable, 
                * this cause is sent to inform MME to store the UEs bearer context. */
               cause.causeVal = SztCauseRadioNwue_not_available_for_ps_serviceEnum;
            }
            else
            {
               cause.causeVal = SztCauseRadioNwcs_fallback_triggeredEnum;
            }
            break;
         }

      case WR_UMM_HIGH_PRIOR_UE_ADM:
         {
            cause.causeTyp = WR_CAUSE_RADIONW;
            cause.causeVal = SztCauseRadioNwrelease_due_to_eutran_generated_reasonEnum;
         }
         break;

      case WR_UMM_CTXT_RLF_REL:
         {
            /* This branch is meant for RLF Error */
            cause.causeTyp = WR_CAUSE_RADIONW;
            cause.causeVal = SztCauseRadioNwfailure_in_radio_interface_procedureEnum;
            break;
         }

      case WR_UMM_REL_CAUSE_SUCES_HO:
         {
            cause.causeTyp = WR_CAUSE_RADIONW;
            cause.causeVal = SztCauseRadioNwsuccessful_handoverEnum;
            break;
         }

      /* ccpu00126817 */
      case WR_UMM_REL_CAUSE_HO_SN_NOTREVD:
         {
            cause.causeTyp = WR_CAUSE_MISC;
            cause.causeVal = SztCauseMiscunspecifiedEnum;
            break;
         }

/* ccpu00126336 */
      case WR_UMM_REL_CAUSE_UL_TNL_MOD_FAIL:
         {
            cause.causeTyp = WR_CAUSE_TRANSPORT;
            cause.causeVal = SztCauseTportunspecifiedEnum;
            break;
         }
      /* CSG_DEV start */
      case WR_UMM_CTXT_REL_CSG_SUBSCRIPTION_EXP:
         {
            cause.causeTyp = WR_CAUSE_NAS;
            cause.causeVal = SztCauseNascsg_subscription_expiryEnum;
            break;
         }/* WR_UMM_CTXT_REL_CSG_SUBSCRIPTION_EXP */
      /* CSG_DEV end */
      case WR_UMM_CTXT_REL_S1_U_ERROR:
         {
            cause.causeTyp = WR_CAUSE_MISC;
            cause.causeVal = SztCauseMiscunspecifiedEnum;
            break;
         }
      case WR_UMM_SEMANTIC_ERROR:
         {
            cause.causeTyp = WR_CAUSE_PROTOCOL;
            cause.causeVal = SztCauseProtsemantic_errorEnum;
            break;
         }
      case WR_UMM_IRAT_REDIR_REL_CAUSE:
         {
            cause.causeTyp = WR_CAUSE_RADIONW;
            cause.causeVal = SztCauseRadioNwinterrat_redirectionEnum;
            break;
         }
      default:
         {
            /* This branch is meant for WR_UMM_CTXT_REL_SWERROR */
            cause.causeTyp = WR_CAUSE_RADIONW;
            cause.causeVal = SztCauseRadioNwfailure_in_radio_interface_procedureEnum;
            break;
         }
   }

   /*Check for s1ConCb before sending Connection release to MME */
   if(NULLP != ueCb->s1ConCb) 
   {

      RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending UE CONTEXT RELEASE "
         "REQUEST, MME-UE-S1AP-ID[%lu] ENB-UE-S1AP-ID[%lu] ",
         ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id);

      wrPrintS1apCauseInfo(cause.causeTyp,cause.causeVal);
      wrUmmS1apFillCtxtRelReq(ueCb, &ctxtRelReqPdu, &cause);

      wrIfmS1apSndUeMsg(ueCb->s1ConCb->spConnId, ctxtRelReqPdu);
      wrStartUeTmr(ueCb,WR_TMR_UE_CTX_REL_REQ, WR_TMR_UE_CTX_REL_VAL);
      wrEmmUpdateStat(WR_EMM_UE_CTXT_REL_STAT, ueCb->cellId,
            WR_EMM_UE_CTXT_REL_REQ,WR_EMM_DFLT_POS_STEP,
            WR_EMM_UE_CTXT_REL_SUBTYP_MAX);
      if((SztCauseRadioNwredirection_towards_1xRTTEnum == cause.causeVal) ||
             (SztCauseRadioNwcs_fallback_triggeredEnum == cause.causeVal) ||
           (SztCauseRadioNwue_not_available_for_ps_serviceEnum == cause.causeVal))
      {
         /* Increment the Success Counter for CSFB/eCSFB */
         wrIncKpiCsfbSucc(ueCb->csfbInfo.ratType, ueCb->csfbInfo.redirType);
      }
      /*Increment KPI forUE CONTEXT Release: Number of UE CONTEXT Release 
       * Request initiated by eNodeB*/
      WR_INC_KPI_FUNC(wrIncKpiUeCtxRelReqAttmptd);
      for(cntr1 =0;cntr1 < ueCb->rbInfo.numOfRbCfgd;cntr1++)
      {
       /*Increment Erab req to release pr Qci*/
         WR_INC_KPI_FUNC1(wrIncKpiFrERABReqToRelPrQci,
               ueCb->rbInfo.rab[cntr1].qci);
         /*Increment KPI for Number of E-RABs requested to release initiated
          * by HeNB per QCI. This is increment-by-one counter*/
         WR_INC_KPI_FUNC2(wrIncKpiFrERABReqToRelPrCause,&cause,
               INC_KPI_VALUE_BY_ONE);
      }
   }
   else
   {

      RLOG_ARG1(L_ERROR, DBG_CRNTI,crnti, "Error in getting S1ConCb for "
         "cellId[%ld]", (U32)cellId);
   }
   RETVALUE(ROK);
}
/** @brief This function responsible for modification measurement neighbor cell
 * configuration
 *
 * @details
 *
 *     Function: wrUmmModNrFreq
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] cellId : cell Id
 * @param [in] numFreq : number of frequency
 * @param [in] measCfgGrp: pointer to the measurement config group
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmModNrFreq
(
 U16                         cellId,
 U8                          numFreq,
 LwrNeighFreqAddModCfg       *nrFreqCfg
)
{
   WrUmmCellCb               *cellCb;
   TRC2(wrUmmModNrFreq)

   WR_UMM_GET_CELLCB(cellCb, cellId)
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   if(RFAILED == wrUmmMeasNrFreqReCfgMod(cellCb, numFreq, nrFreqCfg))
   {
      RLOG0(L_ERROR, "Measurement cell reConfig failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
/** @brief This function responsible for deletion measurement neighbor cell
 * configuration
 *
 * @details
 *
 *     Function: wrUmmDelNrFreq
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] cellId : cell Id
 * @param [in] numFreq : Number of frequency
 * @param [in] nrFreqCfg: pointer to the LwrNeighFreqCfg
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmDelNrFreq
(
 U16                         cellId,
 U16                         numFreq,
 LwrNeighFreqDelCfg          *nrFreqCfg
)
{
   WrUmmCellCb               *cellCb;
   TRC2(wrUmmDelNrFreq)

   WR_UMM_GET_CELLCB(cellCb, cellId)
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   
   if(RFAILED == wrUmmMeasNrFreqReCfgDel(cellCb, numFreq, nrFreqCfg))
   {
      RLOG0(L_ERROR, "Measurement delete cell reConfig failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function responsible for configuring CQIRpt 
 * configuration
 *
 * @details
 *
 *     Function: wrUmmCqiRptCfg
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] cqiRpt: pointer to the WrCqiRptCfg config group
 * @return S16
 *       -# Void
 */
PUBLIC Void wrUmmCqiRptCfg
(
   WrCqiRptCfg *cqiRpt
)
{
   TRC2(wrUmmCqiRptCfg)

   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.aperCqiEnbld = cqiRpt->aperCqiEnbld;
   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.cqiAperiodicMode = cqiRpt->cqiAperiodicMode;
   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.nomPdschRsEpreOff = cqiRpt->nomPdschRsEpreOff;
   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.periodRptSetupPres = cqiRpt->periodRptSetupPres;
   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.cqiPucchRsrcIdx = cqiRpt->cqiPucchRsrcIdx;
   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.cqipmiCfgIdx = cqiRpt->cqipmiCfgIdx;
   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.subbndCqik = cqiRpt->subbndCqik;
   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.fmtInd = cqiRpt->fmtInd;
   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.riCfgIdx = cqiRpt->riCfgIdx;
   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.simulAckNackAndCQI = cqiRpt->simulAckNackAndCQI;
   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.periodicmode = cqiRpt->periodicmode;
   wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.riPres = cqiRpt->riPres;

   RETVOID;
}
/** @brief This function responsible for configuring MAC 
 * configuration
 *
 * @details
 *
 *     Function: wrUmmMacMainCfg
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] macMainCfg: pointer to the LwrMacMainConifg config group
 * @return S16
 *       -# Void
 */
PUBLIC Void wrUmmMacMainCfg
(
   LwrMacMainConifg     *macMainCfg
)
{
   TRC2(wrUmmMacMainCfg)

   wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.timeAlignmentTimerDedicated =
                                 macMainCfg->timeAlignmentTimerDedicated;
   wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.maxHARQ_Tx =
                                 macMainCfg->ulSchCfg.maxHARQ_Tx;
   wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.periodicBSR_Timer = 
                                 macMainCfg->ulSchCfg.periodicBSR_Timer;
   wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.retxBSR_Timer = 
                                 macMainCfg->ulSchCfg.retxBSR_Timer;
   wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.ttiBundling = 
                                 macMainCfg->ulSchCfg.ttiBundling;
   wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.deltaHqOffset = 
                                 macMainCfg->ulSchCfg.deltaHqOffset;
   wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.phr_Config.periodicPHR_Timer =
                                macMainCfg->phr_Config.periodicPHR_Timer;
   wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.phr_Config.prohibitPHR_Timer = 
                                 macMainCfg->phr_Config.prohibitPHR_Timer;
   wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.phr_Config.dl_PathlossChange =
                                 macMainCfg->phr_Config.dl_PathlossChange; 
   RETVOID;
}

/** @brief This function responsible for configuring PDCP 
 * configuration
 *
 * @details
 *
 *     Function: wrUmmPdcpCfg
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] pdcpInfo: pointer to the LwrDrbPdcpInfo group
 * @return S16
 *       -# Void
 */
PUBLIC Void wrUmmPdcpCfg
(
   LwrDrbPdcpInfo     *pdcpInfo
)
{
   TRC2(wrUmmPdcpCfg)

   wrUeDb.cells[WR_DFLT_CELL_IDX].pdcpInfo.discardTimer =
                                pdcpInfo->discardTimer;
   wrUeDb.cells[WR_DFLT_CELL_IDX].pdcpInfo.statRprtReqrd = 
                                 pdcpInfo->statRprtReqrd;
   wrUeDb.cells[WR_DFLT_CELL_IDX].pdcpInfo.umSnSize = 
                                 pdcpInfo->umSnSize;
   RETVOID;
}

/** @brief This function responsible for configuring SCHD 
 * configuration
 *
 * @details
 *
 *     Function: wrUmmSchReqCfg
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] schReqCfg: pointer to the WrSchReqCfg group
 * @return S16
 *       -# Void
 */
PUBLIC Void wrUmmSchReqCfg
(
   LwrSchReqCfg       *schReqCfg
)
{
   TRC2(wrUmmSchReqCfg)

   wrUeDb.cells[WR_DFLT_CELL_IDX].schReqCfg.srPucchRsrcIdx =
                                schReqCfg->srPucchRsrcIdx;
   wrUeDb.cells[WR_DFLT_CELL_IDX].schReqCfg.dsrTransMax = 
                                 schReqCfg->dsrTransMax;
   RETVOID;
}

/** @brief This function responsible for configuring PUSCH 
 * dedicated configuration
 *
 * @details
 *
 *     Function: wrUmmPuschDedCfg
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] puschDedicated: pointer to the WrPuschDedCfg group
 * @return S16
 *       -# Void
 */
PUBLIC Void wrUmmPuschDedCfg
(
   LwrPuschDedCfg       *puschDedicated
)
{
   TRC2(wrUmmPuschDedCfg)

   wrUeDb.cells[WR_DFLT_CELL_IDX].puschDedicated.ackIdx =
                                puschDedicated->ackIdx;
   wrUeDb.cells[WR_DFLT_CELL_IDX].puschDedicated.riIdx = 
                                puschDedicated->riIdx;
   wrUeDb.cells[WR_DFLT_CELL_IDX].puschDedicated.cqiIdx = 
                                puschDedicated->cqiIdx;
   wrUeDb.cells[WR_DFLT_CELL_IDX].puschDedicated.puschPwrCfg.pres = 
                     puschDedicated->puschPwrCfg.pres;
   wrUeDb.cells[WR_DFLT_CELL_IDX].puschDedicated.puschPwrCfg.isFmt3a = 
                     puschDedicated->puschPwrCfg.isFmt3a;

   RETVOID;
}

/** @brief This function responsible for configuring PUCCH 
 * dedicated configuration
 *
 * @details
 *
 *     Function: wrUmmPucchPwrCfg
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] pucchPwrCfg: pointer to the LwrUeUlPwrCfg group
 * @return S16
 *       -# Void
 */
PUBLIC Void wrUmmPucchPwrCfg
(
   LwrUeUlPwrCfg       *pucchPwrCfg
)
{
   TRC2(wrUmmPucchPwrCfg)

   wrUeDb.cells[WR_DFLT_CELL_IDX].uePucchPwr.pres =
                                pucchPwrCfg->pres;
   wrUeDb.cells[WR_DFLT_CELL_IDX].uePucchPwr.isFmt3a = 
                                pucchPwrCfg->isFmt3a;
                                   
   RETVOID;
}

/** @brief This function responsible for configuring PDSCH 
 * dedicated configuration
 *
 * @details
 *
 *     Function: wrUmmPdschPwrCfg
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] puschDedicated: pointer to the WrPuschDedCfg group
 * @return S16
 *       -# Void
 */
PUBLIC Void wrUmmPdschPwrCfg
(
   LwrPdschCfgDedicated  *pdschCfgDedicated
)
{
   TRC2(wrUmmPdschPwrCfg)

   wrUeDb.cells[WR_DFLT_CELL_IDX].pdschCfgDedicated.pA =
                                pdschCfgDedicated->pA;
                                   
   RETVOID;
}

/** @brief This function responsible for configuring QOS 
 * configuration
 *
 * @details
 *
 *     Function: wrUmmQosCfg
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] puschDedicated: pointer to the WrPuschDedCfg group
 * @return S16
 *       -# Void
 */
PUBLIC Void wrUmmQosCfg
(
    LwrQosCfg                *qosCfg
)
{
   TRC2(wrUmmQosCfg)

   wrUeDb.cells[WR_DFLT_CELL_IDX].qosCfg.ueBr =
                                qosCfg->ueBr;
                                   
   RETVOID;
}

/** @brief This function responsible for configuring QOS 
 *          configuration
 *
 * @details
 *
 *     Function: wrUmmCsfbGeranCfg
 *
 *         Processing steps:
 *
 * @param [in] csfbToGeranCfg: pointer to the csfbToGeranCfg
 * @return S16
 *       -# Void
 */
PUBLIC Void wrUmmCsfbGeranCfg
(
 LwrCsfbGeranCfg   *csfbToGeranCfg
)
{
   TRC2(wrUmmCsfbGeranCfg)

   wrUeDb.cells[WR_DFLT_CELL_IDX].csfbToGeranCfg.isGERANMeasAllowedForCCO =
                                csfbToGeranCfg->isGERANMeasAllowedForCCO;
   wrUeDb.cells[WR_DFLT_CELL_IDX].csfbToGeranCfg.isCCOAllowed =
                                csfbToGeranCfg->isCCOAllowed;
   wrUeDb.cells[WR_DFLT_CELL_IDX].csfbToGeranCfg.rrcDatCfmWaitTmr =
                                csfbToGeranCfg->rrcDatCfmWaitTmr;
   wrUeDb.cells[WR_DFLT_CELL_IDX].csfbToGeranCfg.GERANMeasWaitTmr =
                                csfbToGeranCfg->GERANMeasWaitTmr;
   wrUeDb.cells[WR_DFLT_CELL_IDX].csfbToGeranCfg.T304expWaitTmrVal =
                                csfbToGeranCfg->T304expWaitTmrVal;
                                   
   RETVOID;
}
/** @brief This function responsible for configuring GERAN measurement object in
 * measuremnt cell Cb
 * configuration
 *
 * @details
 *
 *     Function: wrUmmAddNrGeranObj
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] cellId : cell Id
 * @param [in] numFreq : number of frequency
 * @param [in] measCfgGrp: pointer to the measurement config group
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmAddNrGeranObj
(
 U16                         cellId,
 U8                          numFreq,
 WrUmmMeasGeranMeasInfo      *nrFreqCfg
)
{
   WrUmmCellCb               *cellCb;
   TRC2(wrUmmAddNrGeranObj)

   WR_UMM_GET_CELLCB(cellCb, cellId)
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   
   if(RFAILED == wrUmmMeasAllocNrGeranObj(cellCb, numFreq, nrFreqCfg))
   {
      RLOG0(L_ERROR, "GERAN Measurement neighbor alloc failed ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
/** @brief This function responsible for configuring measurement neighbor cell
 * configuration
 *
 * @details
 *
 *     Function: wrUmmAddNrFreq
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] cellId : cell Id
 * @param [in] numFreq : number of frequency
 * @param [in] measCfgGrp: pointer to the measurement config group
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmAddNrFreq
(
 U16                         cellId,
 U8                          numFreq,
 LwrNeighFreqAddModCfg       *nrFreqCfg
)
{
   WrUmmCellCb               *cellCb;
   TRC2(wrUmmAddNrFreq)

   WR_UMM_GET_CELLCB(cellCb, cellId)
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   
   if(RFAILED == wrUmmMeasAllocNrFreqCfg(cellCb, numFreq, nrFreqCfg))
   {
      RLOG0(L_ERROR, "Measurement neighbor alloc failed ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
/** @brief This function responsible for re-configuring measurement during 
 * cell reconfiguration
 *
 * @details
 *
 *     Function: wrUmmReCfgCellMeas
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] cellCb : pointer to the cell Cb
 * @param [in] meaResCfgGrp: pointer to the measurement config group
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmReCfgCellMeas 
(
 U16                         cellId,
 LwrCellMeasCfgGrp           *measReCfgGrp
)
{
   WrUmmCellCb               *ummCellCb = NULLP;

   TRC2(wrUmmReCfgCellMeas)

   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if (ummCellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   
   if(NULLP == ummCellCb->measCellCfg)
   {
      RLOG0(L_ERROR, "Measurement configuration is not done");
      RETVALUE(RFAILED);
   }
   wrUmmMeasCellReCfgParams(ummCellCb->measCellCfg, measReCfgGrp);

   RETVALUE(ROK);
}
/** @brief This function responsible for configuring measurement cell
 * configuration
 *
 * @details
 *
 *     Function: wrUmmCfgCellMeas
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] cellCb : pointer to the cell Cb
 * @param [in] measCfgGrp: pointer to the measurement config group
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmCfgCellMeas 
(
 U16                         cellId,
 LwrCellMeasCfgGrp           *measCfgGrp
)
{
   WrUmmCellCb               *ummCellCb = NULLP;

   TRC2(wrUmmCfgCellMeas)

   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if (ummCellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   
   if(RFAILED == wrUmmMeasAllocCellCfg(ummCellCb, measCfgGrp))
   {
      RLOG0(L_ERROR, "Measurement Config alloc failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
/** @brief This function responsible for building internal transaction
 *
 * @details
 *
 *     Function: wrUmmBldInternalMsg
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] intMsg : Pointer to the WrUmmInternalMsg structure
 * @param [in] intMsgType: message type
 * @param [in] msg : Pointer to the message
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmBldInternalMsg
(
 WrUmmInternalMsg             **intMsg,
 U8                           intMsgType,
 Void                         *msg
 )
{
   WR_ALLOC(intMsg, sizeof(WrUmmInternalMsg));
   if (NULLP == *intMsg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   (*intMsg)->intMsgType  = intMsgType;
   switch(intMsgType)
   {
      case WR_UMM_INTMSG_CR_HO_TRANS:
         {
            (*intMsg)->u.hoTrans = (WrUmmIntCrHoTran *)msg;
            break;
         }
      case WR_UMM_INTMSG_MEAS_TRANS:
         {
            (*intMsg)->u.measTrans = (WrUmmIntMeasTrans *)msg;
            break;
         }
      case WR_UMM_INTMSG_NEW_MEAS_TRANS:
         {
            (*intMsg)->u.measNewTrans = (WrMeasCellReCfgInfo *)msg;
            break;
         }
      case WR_UMM_INTMSG_CSFB_REDIR_TRANS:
      case WR_UMM_INTMSG_ECSFB_TRANS:
      case WR_UMM_INTMSG_CSFB_CCO_TRANS:
      case WR_UMM_INTMSG_CSFB_ECSFB_TRANS:
         {
            (*intMsg)->u.csfbIndMsg = (WrUmmIntCsfbTransMsg*)msg;
            break;
         }
      case WR_UMM_INTMSG_EVNT_ANR_MEAS_TRANS:
      case WR_UMM_INTMSG_ANR_MEAS_TRANS:
         {
            (*intMsg)->u.anrMeasMsg = (WrUmmIntAnrMeasMsg*)msg;
            break;
         }
      case WR_UMM_INTMSG_RAD_RES_MOD_MSG: 
         {
            (*intMsg)->u.radResMod.bitMsk = WR_UMM_RAD_RES_MOD_ULPOWER;
            break;          
         }
/* LTE_ADV starts */
      case WR_UMM_INTMSG_LTE_ADV_TRANS:
         {
            (*intMsg)->u.lteAdvMsg = (WrUmmIntLteAdvMsg*)msg;
            break;
         }
/* LTE_ADV ends */
      case WR_UMM_INTMSG_MOD_SR_CQI_RES_MSG:
         {
            (*intMsg)->u.radResMod.bitMsk = WR_UMM_RAD_RES_MOD_CQI_SR;
            break;          
         }
      /* SPS changes starts */
      case WR_UMM_INTMSG_UNSET_SR_MASK:
         {
            (*intMsg)->u.radResMod.bitMsk = WR_UMM_RAD_RES_UNSET_SR_MASK;
            break;          
         }
      /* SPS changes ends */
      case WR_UMM_INTMSG_REDIR_TRANS:
         {
            (*intMsg)->u.redirInfo = (WrUmmRedirectInfo *)msg;
            break;
         }
      default:
         {
            break;
         }
   } /* end of switch */

   RETVALUE(ROK);

} /* end of wrUmmBldInternalMsg */

/** @brief This function gives the ue s1ap id's for the first UE. 
 *
 * @details 
 *     First UE's S1AP Id's will be returned. 
 *
 *     Function: wrUmmMmeGetFirstUeS1apId 
 U
 *         Processing steps:
 *         - Fetch the first UE and update the UE S1AP id's
 *         - if UE list is empty return RFAILED
 *         
 *
 *  @param[in]  mmeId          : MME Id from which reset recvd
 *  @param[out] ue_enb_s1apId  :  
 *  @param[out] ue_mme_s1apId  : 
 *  @return  S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrUmmMmeGetFirstUeS1apId
(
WrMmeId                      mmeId,
U32                         *ue_enb_s1apId,
U32                         *ue_mme_s1apId
)
{
   WrUmmMmeCb                *ummMme;
   WrUeCb                    *ueCb = NULLP;
   CmLList                   *node = NULLP;
   ummMme = WR_UMM_GET_MME(mmeId);
   if(ummMme == NULLP)
   {
      RETVALUE(RFAILED);
   }

   node = ummMme->ueLst.first;
   if(node != NULLP)
   {
      ueCb = (WrUeCb *)(node->node);
     *ue_enb_s1apId = ueCb->s1ConCb->enb_ue_s1ap_id;
     *ue_mme_s1apId = ueCb->s1ConCb->mme_ue_s1ap_id; 

     RETVALUE(ROK);
     
   }
   else
   {
      RETVALUE(RFAILED);
   }
  
} /* wrUmmMmeFullRst */

/** @brief This function updates the counters for the S1AP cause values received. 
 *
 * @details 
 *     Counters are incremented
 *
 *     Function: wrEmmUpdateKpiCounter 
 *
 *         Processing steps:
 *         - build message
 *         
 *
 *  @param[in]  s1cause          : S1AP cause value received
 *  @return  S16
 *        -# Success : ROK
 *        -# Failure : 
 */
PUBLIC S16 wrEmmUpdateKpiCounter
(
 SztCause *s1cause
)
{
        if(s1cause != NULLP)
        {
             switch(s1cause->choice.val)
             {
                 case WR_CAUSE_RADIONW:
            WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, s1cause->choice.val,
                                s1cause->val.radioNw.val, INC_KPI_VALUE_BY_ONE);
                      break;
                 case WR_CAUSE_TRANSPORT:
            WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, s1cause->choice.val,
                               s1cause->val.transport.val, INC_KPI_VALUE_BY_ONE);
                      break;
                 case WR_CAUSE_NAS:
            WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, s1cause->choice.val,
                                     s1cause->val.nas.val, INC_KPI_VALUE_BY_ONE);
                      break;
                 case WR_CAUSE_PROTOCOL:
            WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, s1cause->choice.val,
                               s1cause->val.protocol.val, INC_KPI_VALUE_BY_ONE);
                      break;
                 case WR_CAUSE_MISC:
            WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, s1cause->choice.val,
                                   s1cause->val.misc.val, INC_KPI_VALUE_BY_ONE);
                      break;
                 default:
                      RLOG1(L_ERROR, "Invalid event [%d] received", s1cause->choice.val);
                      break;
             }
        }

        RETVALUE(ROK);
}


PUBLIC S16 wrEmmErabParseStat
(
 WrEmmErabStat updType,
 WrEmmErabSubType subType,
 WrUmmRabSetupTrans * rabSetup,
 WrCellId cellId
)
{

   WrCellCb *cellCb;
   if(NULL == rabSetup)
   {
      /*127802*/
      RLOG0(L_ERROR, "NULL pointer Recived from calling function");
      RETVALUE(ROK);
   }

   WR_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      /*127802*/
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "No Cell Exist");
      RETVALUE(RFAILED);
   }

   switch(updType)
   {
      case WR_EMM_ERAB_DEFAULT:
         switch(subType)
         {
            case WR_EMM_ERAB_ATTEMPT:
                   wrEmmUpdateEstabInitAttNbr(cellCb, rabSetup);
               break;

            case WR_EMM_ERAB_SUCCESS:
                   wrEmmUpdateEstabInitSuccNbr(cellCb, rabSetup);
               break;

            default:
               break;
         }
         break;

      case WR_EMM_ERAB_DEDICATED:
         switch(subType)
         {
            case WR_EMM_ERAB_ATTEMPT:
                    wrEmmUpdateEstabAttNbr(cellCb, rabSetup);
               break;

            case WR_EMM_ERAB_SUCCESS:
                    wrEmmUpdateEstabSuccNbr(cellCb, rabSetup);

               break;
            default:
               break;
         }
         break;
      case WR_EMM_ERAB_MODIFY:
         break;
      case WR_EMM_ERAB_RELEASE_ENB:
         break;
      case WR_EMM_ERAB_RELEASE_MME:
         break;
      default:
         break;
   }

   RETVALUE(ROK);
}

PUBLIC S16 wrEmmUpdateStat
(
 U8 moduleType,
 WrCellId wrCellId,
 Action updType,
 S8 incStep,
 U32 subType
)
{
#ifndef WR_RSYS_KPI
   WrCellCb *cellCb;

   WR_GET_CELLCB(cellCb, wrCellId);
   if(NULLP == cellCb)
   {
      /*127802*/
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)wrCellId, "No Cell Exist");
      RETVALUE(RFAILED);
   }

   switch(moduleType)
   {
      case WR_EMM_RRC_STATS:
         {
            switch (updType)
            {
               case WR_EMM_RRC_CON_ATTEMPT:
                  cellCb->wrCellStats.wrRrcStat.attConnEstab.sum += incStep;
                  switch(subType)
                  {
                     case WR_EMM_RRC_EMERGENCY:
                        cellCb->wrCellStats.wrRrcStat.attConnEstab.emergency += incStep;
                        break;

                     case WR_EMM_RRC_HIGH_PRIORITY_ACCESS:
                        cellCb->wrCellStats.wrRrcStat.attConnEstab.highPriorityAccess += incStep;
                        break;

                     case WR_EMM_RRC_MT_ACCESS:
                        cellCb->wrCellStats.wrRrcStat.attConnEstab.mtAccess += incStep;
                        break;

                     case WR_EMM_RRC_MO_SIGNALLING:
                        cellCb->wrCellStats.wrRrcStat.attConnEstab.moSignalling += incStep;
                        break;

                     case WR_EMM_RRC_MO_DATA:
                        cellCb->wrCellStats.wrRrcStat.attConnEstab.moData += incStep;
                        break;

                     case WR_EMM_RRC_DELAY_TOLERANT_ACCESS_V1020:
                        cellCb->wrCellStats.wrRrcStat.attConnEstab.delayTolerantAccessV1020 += incStep;
                        break;

                     default:
                        /*127802*/
                        RLOG2(L_ERROR, "RRC Statistic of Type=[%d] doesn't "
                              "support SubType=[%ld]", updType, subType);
                        break;
                  }
                  break;

               case WR_EMM_RRC_CON_SUCCESS:
                  cellCb->wrCellStats.wrRrcStat.succConnEstab.sum += incStep;
                  switch(subType)
                  {
                     case WR_EMM_RRC_EMERGENCY:
                        cellCb->wrCellStats.wrRrcStat.succConnEstab.emergency += incStep;
                        break;

                     case WR_EMM_RRC_HIGH_PRIORITY_ACCESS:
                        cellCb->wrCellStats.wrRrcStat.succConnEstab.highPriorityAccess += incStep;
                        break;

                     case WR_EMM_RRC_MT_ACCESS:
                        cellCb->wrCellStats.wrRrcStat.succConnEstab.mtAccess += incStep;
                        break;

                     case WR_EMM_RRC_MO_SIGNALLING:
                        cellCb->wrCellStats.wrRrcStat.succConnEstab.moSignalling += incStep;
                        break;

                     case WR_EMM_RRC_MO_DATA:
                        cellCb->wrCellStats.wrRrcStat.succConnEstab.moData += incStep;
                        break;

                     case WR_EMM_RRC_DELAY_TOLERANT_ACCESS_V1020:
                        cellCb->wrCellStats.wrRrcStat.succConnEstab.delayTolerantAccessV1020 += incStep;
                        break;

                     default:
                        /*127802*/
                        RLOG2(L_ERROR, "RRC Statistic of Type=[%d] doesn't "
                              "support SubType=[%ld]", updType, subType);
                        break;
                  }
                  break;
               case WR_EMM_RRC_CON_FAIL:
                  cellCb->wrCellStats.wrRrcStat.failConnEstab.sum += incStep;
                  switch(subType)
                  {
                     case WR_EMM_RRC_EMERGENCY:
                        cellCb->wrCellStats.wrRrcStat.failConnEstab.emergency += incStep;
                        break;

                     case WR_EMM_RRC_HIGH_PRIORITY_ACCESS:
                        cellCb->wrCellStats.wrRrcStat.failConnEstab.highPriorityAccess += incStep;
                        break;

                     case WR_EMM_RRC_MT_ACCESS:
                        cellCb->wrCellStats.wrRrcStat.failConnEstab.mtAccess += incStep;
                        break;

                     case WR_EMM_RRC_MO_SIGNALLING:
                        cellCb->wrCellStats.wrRrcStat.failConnEstab.moSignalling += incStep;
                        break;

                     case WR_EMM_RRC_MO_DATA:
                        cellCb->wrCellStats.wrRrcStat.failConnEstab.moData += incStep;
                        break;

                     case WR_EMM_RRC_DELAY_TOLERANT_ACCESS_V1020:
                        cellCb->wrCellStats.wrRrcStat.failConnEstab.delayTolerantAccessV1020 += incStep;
                        break;

                     default:
                        /*127802*/
                        RLOG2(L_ERROR, "RRC Statistic of Type=[%d] doesn't "
                              "support SubType=[%ld]", updType, subType);
                        break;
                  }

                  break;
               case WR_EMM_RRC_CON_FAIL_DEGRAD:
                  {
                     switch(subType)
                     {
                        case WR_EMM_RRC_EMERGENCY:
                           cellCb->wrCellStats.wrRrcStat.failConnDegradedRadio.emergency += incStep;
                           break;
                        case WR_EMM_RRC_HIGH_PRIORITY_ACCESS:
                           cellCb->wrCellStats.wrRrcStat.failConnDegradedRadio.highPriorityAccess += incStep;
                           break;
                        case WR_EMM_RRC_MT_ACCESS:
                           cellCb->wrCellStats.wrRrcStat.failConnDegradedRadio.mtAccess += incStep;
                           break;
                        case WR_EMM_RRC_MO_SIGNALLING:
                           cellCb->wrCellStats.wrRrcStat.failConnDegradedRadio.moSignalling += incStep;
                           break;
                        case WR_EMM_RRC_MO_DATA:
                           cellCb->wrCellStats.wrRrcStat.failConnDegradedRadio.moData += incStep;
                           break;
                        case WR_EMM_RRC_DELAY_TOLERANT_ACCESS_V1020:
                           cellCb->wrCellStats.wrRrcStat.failConnDegradedRadio.delayTolerantAccessV1020 += incStep;
                           break;
                        default:
                           /*127802*/
                           RLOG2(L_ERROR, "RRC Statistic of Type=[%d] doesn't "
                                 "support SubType=[%ld]", updType, subType);
                           break;
                     }
                  }
                  cellCb->wrCellStats.wrRrcStat.failConnDegradedRadio.sum += incStep;
                  break;

               default:
                  /*127802*/
                  RLOG1(L_ERROR, "RRC Statistic of Type=[%d] is not Supported",
                        updType);
                  break;
            }
         }
         break;

      case WR_EMM_ERAB_STATS:
         {
            switch(updType)
            {

               case WR_EMM_ERAB_DEFAULT:
                  {
                     switch(subType)
                     {
                        case WR_EMM_ERAB_ATTEMPT:
                           cellCb->wrCellStats.wrErabStat.estabInitAttNbr.qciSum += incStep;
                           break;

                        case WR_EMM_ERAB_SUCCESS:
                           cellCb->wrCellStats.wrErabStat.estabInitSuccNbr.qciSum += incStep;
                           break;

                        case WR_EMM_ERAB_QCI_1:
                           break;

                        case WR_EMM_ERAB_QCI_2:
                           break;

                        case WR_EMM_ERAB_QCI_3:
                           break;

                        case WR_EMM_ERAB_QCI_4:
                           break;

                        case WR_EMM_ERAB_QCI_5:
                           break;

                        case WR_EMM_ERAB_QCI_6:
                           break;

                        case WR_EMM_ERAB_QCI_7:
                           break;

                        case WR_EMM_ERAB_QCI_8:
                           break;

                        case WR_EMM_ERAB_QCI_9:
                           break;

                        case WR_EMM_ERAB_FAIL_RADIORESOURCESNOTAVAILABLE:
                           break;

                        case WR_EMM_ERAB_FAIL_REDUCELOADINSERVINGCELL:
                           break;

                        case WR_EMM_ERAB_FAIL_FAILUREINTHERADIOINTERFACEPROCEDURE:
                           break;

                        case WR_EMM_ERAB_FAIL_RELEASEDUETOEUTRANGENERATEDREASONS:
                           cellCb->wrCellStats.wrErabStat.EstabInitFailNbr.wrInvalidQoSCombinations += incStep;
                           break;

                        case WR_EMM_ERAB_FAIL_USERINACTIVITY:
                           break;

                        case WR_EMM_ERAB_FAIL_RADIOCONNECTIONWITHUELOST:
                           break;

                        case WR_EMM_ERAB_FAIL_OAMINTERVENTION:
                           break;
                        default:
                           break;
                     }
                  }
                  break;

               case WR_EMM_ERAB_DEDICATED:
                  {
                     switch(subType)
                     {
                        case WR_EMM_ERAB_ATTEMPT:
                           cellCb->wrCellStats.wrErabStat.EstabAttNbr.qciSum += incStep;
                           break;

                        case WR_EMM_ERAB_SUCCESS:
                           cellCb->wrCellStats.wrErabStat.EstabSuccNbr.qciSum += incStep;
                           break;

                        case WR_EMM_ERAB_QCI_1:
                           break;

                        case WR_EMM_ERAB_QCI_2:
                           break;

                        case WR_EMM_ERAB_QCI_3:
                           break;

                        case WR_EMM_ERAB_QCI_4:
                           break;

                        case WR_EMM_ERAB_QCI_5:
                           break;

                        case WR_EMM_ERAB_QCI_6:
                           break;

                        case WR_EMM_ERAB_QCI_7:
                           break;

                        case WR_EMM_ERAB_QCI_8:
                           break;

                        case WR_EMM_ERAB_QCI_9:
                           break;

                        case WR_EMM_ERAB_FAIL_RADIORESOURCESNOTAVAILABLE:
                           cellCb->wrCellStats.wrErabStat.EstabInitFailNbr.wrRadioResourcesNotAvailable += incStep;
                           break;

                        case WR_EMM_ERAB_FAIL_REDUCELOADINSERVINGCELL:
                           break;

                        case WR_EMM_ERAB_FAIL_FAILUREINTHERADIOINTERFACEPROCEDURE:
                           break;

                        case WR_EMM_ERAB_FAIL_RELEASEDUETOEUTRANGENERATEDREASONS:
                           break;

                        case WR_EMM_ERAB_FAIL_USERINACTIVITY:
                           break;

                        case WR_EMM_ERAB_FAIL_RADIOCONNECTIONWITHUELOST:
                           cellCb->wrCellStats.wrErabStat.EstabInitFailNbr.wrFailureInRadioInterfaceProcedure += incStep;
                           break;

                        case WR_EMM_ERAB_FAIL_OAMINTERVENTION:
                           break;

                        default:
                           break;
                     }
                  }
                  break;

               case WR_EMM_ERAB_MODIFY:
                  {
                     switch(subType)
                     {
                        case WR_EMM_ERAB_ATTEMPT:
                           cellCb->wrCellStats.wrErabStat.ModAttNbr.qciSum += incStep;
                           break;

                        case WR_EMM_ERAB_SUCCESS:
                           cellCb->wrCellStats.wrErabStat.ModSuccNbr.qciSum += incStep;
                           break;

                        case WR_EMM_ERAB_QCI_1:
                           break;

                        case WR_EMM_ERAB_QCI_2:
                           break;

                        case WR_EMM_ERAB_QCI_3:
                           break;

                        case WR_EMM_ERAB_QCI_4:
                           break;

                        case WR_EMM_ERAB_QCI_5:
                           break;

                        case WR_EMM_ERAB_QCI_6:
                           break;

                        case WR_EMM_ERAB_QCI_7:
                           break;

                        case WR_EMM_ERAB_QCI_8:
                           break;

                        case WR_EMM_ERAB_QCI_9:
                           break;

                        case WR_EMM_ERAB_FAIL_RADIORESOURCESNOTAVAILABLE:
                           break;

                        case WR_EMM_ERAB_FAIL_REDUCELOADINSERVINGCELL:
                           break;

                        case WR_EMM_ERAB_FAIL_FAILUREINTHERADIOINTERFACEPROCEDURE:
                           cellCb->wrCellStats.wrErabStat.ModFailNbr.wrFailureInRadioInterfaceProcedure += incStep;
                           break;

                        case WR_EMM_ERAB_FAIL_RELEASEDUETOEUTRANGENERATEDREASONS:
                           break;

                        case WR_EMM_ERAB_FAIL_USERINACTIVITY:
                           break;

                        case WR_EMM_ERAB_FAIL_RADIOCONNECTIONWITHUELOST:
                           cellCb->wrCellStats.wrErabStat.ModFailNbr.wrFailureInRadioInterfaceProcedure += incStep;
                           break;

                        case WR_EMM_ERAB_FAIL_OAMINTERVENTION:
                           break;
                        default:
                           break;
                     }
                  }

                  break;

               case WR_EMM_ERAB_RELEASE_ENB:
                  {
                     switch(subType)
                     {
                        case WR_EMM_ERAB_ATTEMPT:
                           break;

                        case WR_EMM_ERAB_SUCCESS:
                           cellCb->wrCellStats.wrErabStat.RelEnbNbr.qciSum += incStep;
                           break;

                        case WR_EMM_ERAB_QCI_1:
                           break;

                        case WR_EMM_ERAB_QCI_2:
                           break;

                        case WR_EMM_ERAB_QCI_3:
                           break;

                        case WR_EMM_ERAB_QCI_4:
                           break;

                        case WR_EMM_ERAB_QCI_5:
                           break;

                        case WR_EMM_ERAB_QCI_6:
                           break;

                        case WR_EMM_ERAB_QCI_7:
                           break;

                        case WR_EMM_ERAB_QCI_8:
                           break;

                        case WR_EMM_ERAB_QCI_9:
                           break;

                        case WR_EMM_ERAB_FAIL_RADIORESOURCESNOTAVAILABLE:
                           cellCb->wrCellStats.wrErabStat.RelEnbNbr_Henb.wrRadioResourcesNotAvailable += incStep;
                           break;

                        case WR_EMM_ERAB_FAIL_REDUCELOADINSERVINGCELL:
                           break;

                        case WR_EMM_ERAB_FAIL_FAILUREINTHERADIOINTERFACEPROCEDURE:
                           break;

                        case WR_EMM_ERAB_FAIL_RELEASEDUETOEUTRANGENERATEDREASONS:
                           break;

                        case WR_EMM_ERAB_FAIL_USERINACTIVITY:
                           break;

                        case WR_EMM_ERAB_FAIL_RADIOCONNECTIONWITHUELOST:
                           break;

                        case WR_EMM_ERAB_FAIL_OAMINTERVENTION:
                           break;
                        default:
                           break;
                     }
                  }

                  break;

               case WR_EMM_ERAB_RELEASE_MME:
                  {
                     switch(subType)
                     {
                        case WR_EMM_ERAB_ATTEMPT:
                           cellCb->wrCellStats.wrErabStat.RelEnbMmeAtt.qciSum += incStep;
                           break;

                        case WR_EMM_ERAB_SUCCESS:
                           cellCb->wrCellStats.wrErabStat.RelEnbMmeSucc.qciSum += incStep;
                           break;

                        case WR_EMM_ERAB_QCI_1:
                           break;

                        case WR_EMM_ERAB_QCI_2:
                           break;

                        case WR_EMM_ERAB_QCI_3:
                           break;

                        case WR_EMM_ERAB_QCI_4:
                           break;

                        case WR_EMM_ERAB_QCI_5:
                           break;

                        case WR_EMM_ERAB_QCI_6:
                           break;

                        case WR_EMM_ERAB_QCI_7:
                           break;

                        case WR_EMM_ERAB_QCI_8:
                           break;

                        case WR_EMM_ERAB_QCI_9:
                           break;

                        case WR_EMM_ERAB_FAIL_RADIORESOURCESNOTAVAILABLE:
                           break;

                        case WR_EMM_ERAB_FAIL_REDUCELOADINSERVINGCELL:
                           break;

                        case WR_EMM_ERAB_FAIL_FAILUREINTHERADIOINTERFACEPROCEDURE:
                           break;

                        case WR_EMM_ERAB_FAIL_RELEASEDUETOEUTRANGENERATEDREASONS:
                           break;

                        case WR_EMM_ERAB_FAIL_USERINACTIVITY:
                           break;

                        case WR_EMM_ERAB_FAIL_RADIOCONNECTIONWITHUELOST:
                           break;

                        case WR_EMM_ERAB_FAIL_OAMINTERVENTION:
                           break;
                        default:
                           break;
                     }
                  }

                  break;

               default:
                  break;
            }
         }
         break;
      case WR_EMM_PAG_STATS:
         {
            switch(updType)
            {
               case WR_EMM_PAG_TOT:
                  cellCb->wrCellStats.wrPagStat += incStep;
                  break;

               default:
                  /*127802*/
                  RLOG1(L_ERROR, "RRC Statistic of Type=[%d] is not Supported",
                        updType);
                  break;

            }
         }

      case WR_EMM_CALL_STATS:
         {
            switch(updType)
            {
               case WR_EMM_CALL_TOT:
                  cellCb->wrCellStats.wrCallStat.wrTotalCalls += incStep;
                  break;
               case WR_EMM_CALL_ACT:
                  if(incStep < 0 && cellCb->wrCellStats.wrCallStat.wrActiveCalls <= 0)
                     break;

                  cellCb->wrCellStats.wrCallStat.wrActiveCalls += incStep;
                  break;
               default:
                  /*127802*/
                  RLOG1(L_ERROR, "RRC Statistic of Type=[%d] is not Supported",
                        updType);
                  break;
            }
         }
         break;

      case WR_EMM_RRC_REEST_STATS:
         {
            switch (updType)
            {
               case WR_EMM_RRC_REESTAB_CON_ATTEMPT:
                  cellCb->wrCellStats.wrRrcReEstabSts.attConnEstab.sum += incStep;
                  switch(subType)
                  {
                     case WR_EMM_RRC_REESTAB_RECFG:
                        cellCb->wrCellStats.wrRrcReEstabSts.attConnEstab.recfgFail += incStep;
                        break;
                     case WR_EMM_RRC_REESTAB_HO:
                        cellCb->wrCellStats.wrRrcReEstabSts.attConnEstab.hoFail += incStep;
                        break;
                     case WR_EMM_RRC_REESTAB_OTHR:
                        cellCb->wrCellStats.wrRrcReEstabSts.attConnEstab.OthrFail += incStep;
                        break;

                     default:
                        /*127802*/
                        RLOG2(L_ERROR, "RRC- Re Establishment Statistic of "
                              "Type=[%d] doesn't support SubType=[%ld]",
                              updType, subType);
                        break;
                  }
                  break;

               case WR_EMM_RRC_REESTAB_CON_SUCCESS:
                  cellCb->wrCellStats.wrRrcReEstabSts.succConnEstab.sum += incStep;
                  switch(subType)
                  {
                     case WR_EMM_RRC_REESTAB_RECFG:
                        cellCb->wrCellStats.wrRrcReEstabSts.succConnEstab.recfgFail += incStep;
                        break;
                     case WR_EMM_RRC_REESTAB_HO:
                        cellCb->wrCellStats.wrRrcReEstabSts.succConnEstab.hoFail += incStep;
                        break;
                     case WR_EMM_RRC_REESTAB_OTHR:
                        cellCb->wrCellStats.wrRrcReEstabSts.succConnEstab.OthrFail += incStep;
                        break;

                     default:
                        /*127802*/
                        RLOG2(L_ERROR, "RRC- Re Establishment Statistic of "
                              "Type=[%d] doesn't support SubType=[%ld]",
                              updType, subType);
                        break;
                  }
                  break;
               case WR_EMM_RRC_REESTAB_CON_FAIL:
                  cellCb->wrCellStats.wrRrcReEstabSts.failConnEstab.sum += incStep;
                  switch(subType)
                  {
                     case WR_EMM_RRC_REESTAB_RECFG:
                        cellCb->wrCellStats.wrRrcReEstabSts.succConnEstab.recfgFail += incStep;
                        break;
                     case WR_EMM_RRC_REESTAB_HO:
                        cellCb->wrCellStats.wrRrcReEstabSts.succConnEstab.hoFail += incStep;
                        break;
                     case WR_EMM_RRC_REESTAB_OTHR:
                        cellCb->wrCellStats.wrRrcReEstabSts.succConnEstab.OthrFail += incStep;
                        break;

                     default:
                        /*127802*/
                        RLOG2(L_ERROR, "RRC- Re Establishment Statistic of "
                              "Type=[%d] doesn't support SubType=[%ld]",
                              updType, subType);
                        break;
                  }

                  break;
               case WR_EMM_RRC_REESTAB_CON_FAIL_DEGRAD:
                  {
                     cellCb->wrCellStats.wrRrcReEstabSts.failConnDegradedRadio.sum += incStep;
                     switch(subType)
                     {
                        case WR_EMM_RRC_REESTAB_RECFG:
                           cellCb->wrCellStats.wrRrcReEstabSts.failConnDegradedRadio.recfgFail += incStep;
                           break;
                        case WR_EMM_RRC_REESTAB_HO:
                           cellCb->wrCellStats.wrRrcReEstabSts.failConnDegradedRadio.hoFail += incStep;
                           break;
                        case WR_EMM_RRC_REESTAB_OTHR:
                           cellCb->wrCellStats.wrRrcReEstabSts.failConnDegradedRadio.OthrFail += incStep;
                           break;

                        default:
                           /*127802*/
                           RLOG2(L_ERROR, "RRC- Re Establishment Statistic of "
                                 "Type=[%d] doesn't support SubType=[%ld]",
                                 updType, subType);
                           break;
                     }
                  }
                  cellCb->wrCellStats.wrRrcReEstabSts.failConnDegradedRadio.sum += incStep;
                  break;

               default:
                  /*127802*/
                  RLOG1(L_ERROR, "RRC Statistic of Type=[%d] is not Supported",
                        updType);
                  break;
            }
         }
         break;

      case WR_EMM_SIG_STATS:
         {
            switch (updType)
            {
               case  WR_EMM_SIG_ATTEMPT:
                  cellCb->wrCellStats.wrSigSts.wrSigAttempt +=incStep;
                  break;

               case WR_EMM_SIG_SUCCESS:
                  cellCb->wrCellStats.wrSigSts.wrSigSuccess +=incStep;
                  break;

               default:
                  break;
            }
         }
         break;

      case WR_EMM_UE_CTXT_REL_STAT:
         {
            switch(updType)
            {
               case WR_EMM_UE_CTXT_REL_REQ:
                  cellCb->wrCellStats.wrUeCtxtRelSts.wrUeCtxtRelReq +=incStep;
                  break;
               case WR_EMM_UE_CTXT_REL_SUCCESS:
                  cellCb->wrCellStats.wrUeCtxtRelSts.wrUeCtxtRelSuccess +=incStep;
                  break;
               default:
                  /*127802*/
                  RLOG1(L_ERROR, "UE Ctnxt Rel Statistic of Type=[%d] is not "
                        "Supported",updType);
                  break;
            }
         }
         break;

      default:
         /*127802*/
         RLOG1(L_ERROR, "Statics are Not for Module = [%d]",moduleType);
         break;
   }
#endif/*WR_RSYS_KPI*/
   RETVALUE(ROK);
}
/** @brief This function is responsible for releasing UE if the MME is not 
 *                               sending UE Release for Initial context failure 
 *                               cause Request Message. 
 *
 * @details
 *
 *     Function: wrUmmUeCtxRelTimerExp
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] ueCb : UE Control block
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */

PUBLIC S16 wrUmmUeCtxRelTimerExp
(
WrUeCb                    *ueCb
)
{
  wrUmmPrcLclCtxtRel(ueCb,NhuRlsCauseotherEnum);
  RETVALUE(ROK);
}


/** @brief This function is responsible for releasing UE if the MME is not 
 *                               sending Initial context Setup Request Message. 
 *
 * @details
 *
 *     Function: wrUmmInitalCtxRspTimerExp
 *
 *         Processing steps:
 *         - Process message
 *
 * @param [in] ueCb : UE Control block
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmInitalCtxRspTimerExp
(
WrUeCb                    *ueCb
)
{
   S16 ret;
   if ((ueCb->s1ConCb) && (ueCb->s1ConCb->s1apConnState == WR_S1AP_CONNECTED))
   {

      ret = wrUmmSndCtxtRelReq( ueCb->crnti, ueCb->cellId,
                                WR_UMM_REL_CAUSE_UL_TNL_MOD_FAIL);

      if ( ret != ROK)
      {
         /* Log error */
         RLOG0(L_ERROR, "wrUmmSndCtxtRelReq Failed");
      }
   }
   else 
   {
      wrUmmPrcLclCtxtRel(ueCb,NhuRlsCauseotherEnum);
   }
  
  RETVALUE(ROK);
}


/** @brief This function is responsible for finding out if measurement gap
 *         is required for a band class(inter RAT)  
 * @details
 *
 *     Function: wrUmmMeasGapReqdInterRat
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] cellId : cellID 
 * @param [in] crnti : UE crnti 
 * @param [in] servingBand: Band at which UE currently camped on
 * @param [in] targetBand : Band for which measurement gap is needed or not
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmMeasGapReqdInterRat
(
 WrUeCb                       *ueCb,
 U8                           servingBand,
 U8                           targetBand,
 U8                           ratType,
 LwrUtraTddMode               tddMode,
 Bool                         *result
 )
{
   U8          idx            = 0;
   U8          targetBandIdx  = 0;
   U16         numElmnts      = 0;
   U16         iRatBandIdx    = 0;
   Bool        isBandPrsnt    = FALSE;
   U8          utraFDDVal     = 0;
   U8          utraTDD128Val  = 0; 
   U8          utraTDD384Val  = 0; 
   U8          utraTDD768Val  = 0; 
   U8          geranVal       = 0;
   U8          cdma2kHRPDVal  = 0; 
   U8          cdma2k1xRTTVal = 0;     

   /*find out index of serving band and target band in the ue capability list*/
   if(ueCb->ueCap->eUTRA_Cap.rf_Params.pres.pres != NOTPRSNT)
   {
      numElmnts = ueCb->ueCap->eUTRA_Cap.rf_Params.supportedBandLstEUTRA.noComp.val;
      for(idx=0;idx<numElmnts;idx++)
      {
         if(ueCb->ueCap->eUTRA_Cap.rf_Params.supportedBandLstEUTRA.member[idx].
               bandEUTRA.val == servingBand)
         {
            break;
         }
      }
   }
   else
   {
      RLOG0(L_ERROR, "Supported band list not present");
      RETVALUE(RFAILED);
   }
   /* UE didnt include eNB operating band in its supported LTE band list
    * this can not happen but still having check */
   if(idx >= numElmnts)
   {
      RLOG0(L_ERROR, "UE didnt include eNB serving band in its Supported band list");
      RETVALUE(RFAILED);
   }

   if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.pres.pres == NOTPRSNT)
   {
      RLOG0(L_ERROR, "UE is not supporting any Inter RAT, Inter RAT params IE is not present");
      RETVALUE(RFAILED);
   }

   if(ueCb->ueCap->eUTRA_Cap.measParams.bandLstEUTRA.member[idx].
      interRAT_BandLst.noComp.pres == NOTPRSNT)
   {
      RLOG0(L_INFO, "MeasParameters.BandListEUTRA.Inter RAT params is not present, assuming measurement gap is not required");
      /* UE has not included the IE (optional), assuming UE doesnt need meas gap */
      *result = FALSE;
      RETVALUE(ROK);
   }

   if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraFDD.pres.pres != NOTPRSNT )
   {
      utraFDDVal = ueCb->ueCap->eUTRA_Cap.interRAT_Params.
         utraFDD.supportedBandLstUTRA_FDD.noComp.val;
   }
   if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD128.pres.pres != NOTPRSNT )
   {
      utraTDD128Val = ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD128.
         supportedBandLstUTRA_TDD128.noComp.val;
   }
   if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD384.pres.pres != NOTPRSNT)
   {
      utraTDD384Val = ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD384.
         supportedBandLstUTRA_TDD384.noComp.val;
   }
   if (ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD768.pres.pres != NOTPRSNT)
   {
      utraTDD768Val =  ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD768.
         supportedBandLstUTRA_TDD768.noComp.val;
   }
   if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.geran.pres.pres  != NOTPRSNT )
   {
      geranVal = ueCb->ueCap->eUTRA_Cap.interRAT_Params.geran.
         supportedBandLstGERAN.noComp.val;
   }
   if (ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_HRPD.pres.pres != NOTPRSNT)
   {
      cdma2kHRPDVal = ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_HRPD.
         supportedBandLstHRPD.noComp.val;
   }
   if (ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_1xRTT.pres.pres != NOTPRSNT)
   {
      cdma2k1xRTTVal = ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_1xRTT.
         supportedBandLst1XRTT.noComp.val;
   }
   /*Find the target Band Idx matching with UE supported Band */
   switch(ratType)
   {
      case UTRA_FDD:
         {
            if(NOTPRSNT != 
                  ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraFDD.pres.pres)
            {
               numElmnts = ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraFDD.
                  supportedBandLstUTRA_FDD.noComp.val;
               for(targetBandIdx=0;targetBandIdx<numElmnts;targetBandIdx++)
               {
                  if((ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraFDD.
                           supportedBandLstUTRA_FDD.member[targetBandIdx].
                           val + 1) == targetBand)
                  {
                     iRatBandIdx = targetBandIdx;
                     isBandPrsnt = TRUE;
                     break;
                  }
               }/*end of for*/
               if(isBandPrsnt == FALSE)
               {
                  RLOG0(L_ERROR, "target Band not present in UTRA FDD List");
                  RETVALUE(RFAILED);
               }
            }
            else
            {
               RLOG0(L_ERROR, "UNTRA FDD list Not present");
               RETVALUE(RFAILED);
            }
            break;
         }
      case UTRA_TDD:
         {
            switch(tddMode)
            {
               case LWR_UTRA_TDD128:
                  {
                     if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD128.pres.pres != NOTPRSNT)
                     {
                        numElmnts = ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD128.
                           supportedBandLstUTRA_TDD128.noComp.val;
                        for(targetBandIdx=0;targetBandIdx<numElmnts;targetBandIdx++)
                        {
                           if((ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD128.
                                    supportedBandLstUTRA_TDD128.member[targetBandIdx].
                                    val+1) == targetBand)
                           {
                              iRatBandIdx = targetBandIdx + utraFDDVal;
                              isBandPrsnt = TRUE;   
                              break;
                           }
                        }/*end of for loop*/
                        if(isBandPrsnt == FALSE)
                        {
                           RLOG0(L_ERROR, "target Band not present in UTRA TDD128 List");
                           RETVALUE(RFAILED);
                        }
                     }/*end of if*/
                     else
                     {
                        RLOG0(L_ERROR, "UNTRA TDD128 list Not present");
                        RETVALUE(RFAILED);
                     }
                     break;
                  }

               case LWR_UTRA_TDD384:
                  {
                     if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD384.pres.pres != NOTPRSNT)
                     {
                        numElmnts = ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD384.
                           supportedBandLstUTRA_TDD384.noComp.val;
                        for(targetBandIdx=0;targetBandIdx<numElmnts;targetBandIdx++)
                        {
                           if((ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD384.
                                    supportedBandLstUTRA_TDD384.member[targetBandIdx].
                                    val+1) == targetBand)
                           {
                              iRatBandIdx = targetBandIdx + utraFDDVal + utraTDD128Val;
                              isBandPrsnt = TRUE;
                              break;
                           }
                        }/*end of for loop*/
                        if(isBandPrsnt == FALSE)
                        {
                           RLOG0(L_ERROR, "target Band not present in UTRA TDD384 List");
                           RETVALUE(RFAILED);
                        }
                     }
                     else
                     {
                        RLOG0(L_ERROR, "UNTRA TDD384 list Not present");
                        RETVALUE(RFAILED);
                     }
                     break;
                  }
               case LWR_UTRA_TDD768:
                  {
                     if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD768.pres.pres != NOTPRSNT)
                     {
                        numElmnts = ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD768.
                           supportedBandLstUTRA_TDD768.noComp.val;
                        for(targetBandIdx=0; targetBandIdx<numElmnts; targetBandIdx++)
                        {
                           if((ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD768.
                                    supportedBandLstUTRA_TDD768.member[targetBandIdx].
                                    val + 1) == targetBand)
                           {
                              iRatBandIdx = targetBandIdx + utraFDDVal + utraTDD128Val + 
                                 utraTDD384Val;
                              isBandPrsnt = TRUE;
                              break;
                           }
                        }/*end of for loop*/
                        if(isBandPrsnt == FALSE)
                        {
                           RLOG0(L_ERROR, "target Band not present in UTRA TDD768 List");
                           RETVALUE(RFAILED);
                        }
                     }
                     else
                     {
                        RLOG0(L_ERROR, "UNTRA TDD768 list Not present");
                        RETVALUE(RFAILED);
                     }
                     break;
                  }
            }
         }
         break;
      case GERAN:
         {
            if(NOTPRSNT != 
                  ueCb->ueCap->eUTRA_Cap.interRAT_Params.geran.pres.pres)
            {
               numElmnts = ueCb->ueCap->eUTRA_Cap.interRAT_Params.geran.
                  supportedBandLstGERAN.noComp.val;
               for(targetBandIdx=0; targetBandIdx < numElmnts; targetBandIdx++)
               {
                  /* + 1 is removed as CDMA bandclass starts with ZERO ENUM (BC0)*/
                  if((ueCb->ueCap->eUTRA_Cap.interRAT_Params.geran.
                           supportedBandLstGERAN.member[targetBandIdx].
                           val) == targetBand)
                  {
                     iRatBandIdx = targetBandIdx + utraFDDVal + utraTDD128Val + 
                        utraTDD384Val + utraTDD768Val;
                     isBandPrsnt = TRUE;
                     break;
                  }
               }/*end of for*/
               if(isBandPrsnt == FALSE)
               {
                  RLOG0(L_ERROR, "target Band not present in GERAN List");
                  RETVALUE(RFAILED);
               }
            }
            else
            {
               RLOG0(L_ERROR, "GERAN list Not present");
            }
            break;
         }
      case CDMA2000_HRPD:
         {
            if(NOTPRSNT != 
                  ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_HRPD.pres.pres)
            {
               numElmnts = ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_HRPD.
                  supportedBandLstHRPD.noComp.val;
               for(targetBandIdx=0; targetBandIdx < numElmnts; targetBandIdx++)
               {
                  /* + 1 is removed as CDMA bandclass starts with ZERO ENUM (BC0)*/
                  if((ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_HRPD.
                           supportedBandLstHRPD.member[targetBandIdx].
                           val) == targetBand)
                  {
                     iRatBandIdx = targetBandIdx + utraFDDVal + utraTDD128Val + 
                        utraTDD384Val + utraTDD768Val + geranVal;
                     isBandPrsnt = TRUE;
                     break;
                  }
               }/*end of for*/
               if(isBandPrsnt == FALSE)
               {
                  RLOG0(L_ERROR, "target Band not present in CDMA 1xRTT List");
                  RETVALUE(RFAILED);
               }
            }
            else
            {
               RLOG0(L_ERROR, "CDMA list Not present");
            }
            break;
         }
      case CDMA2000_1XRTT:
         {
            if(NOTPRSNT != 
                  ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_1xRTT.pres.pres)
            {
               numElmnts = ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_1xRTT.
                  supportedBandLst1XRTT.noComp.val;
               for(targetBandIdx=0; targetBandIdx < numElmnts; targetBandIdx++)
               {
                  /* + 1 is removed as CDMA bandclass starts with ZERO ENUM (BC0)*/
                  if((ueCb->ueCap->eUTRA_Cap.interRAT_Params.cdma2000_1xRTT.
                           supportedBandLst1XRTT.member[targetBandIdx].
                           val) == targetBand)
                  {
                     iRatBandIdx = targetBandIdx + utraFDDVal + utraTDD128Val + 
                        utraTDD384Val + utraTDD768Val + geranVal  + 
                        cdma2kHRPDVal;
                     isBandPrsnt = TRUE;
                     break;
                  }
               }/*end of for*/
               if(isBandPrsnt == FALSE)
               {
                  RLOG0(L_ERROR, "target Band not present in CDMA 1xRTT List");
                  RETVALUE(RFAILED);
               }
            }
            else
            {
               RLOG0(L_ERROR, "UNTRA FDD list Not present");
               RETVALUE(RFAILED);
            }
            break;
         }
      default:
         RLOG0(L_ERROR, "Target Band not present in InterRat List");
         RETVALUE(RFAILED);
         break;
   }
   *result = ueCb->ueCap->eUTRA_Cap.measParams.bandLstEUTRA.member[idx].
      interRAT_BandLst.member[iRatBandIdx].interRAT_NeedForGaps.val;

   RETVALUE(ROK);
}/*wrUmmMeasGapReqdInterRat*/

/** @brief This function is responsible for finding out if measurement gap
 *         is required for a band class(interfreq)  
 * @details
 *
 *     Function: wrUmmMeasGapReqdInterfreq
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] cellId : cellID 
 * @param [in] crnti : UE crnti 
 * @param [in] servingBand: Band at which UE currently camped on
 * @param [in] targetBand : Band for which measurement gap is needed or not
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmMeasGapReqdInterfreq
(
 WrUeCb                       *ueCb,
 U8                           servingBand,
 U8                           targetBand,
 Bool                         *result
 )
{
   U8          idx            = 0;
   U8          targetBandIdx  = 0;
   U8          numElmnts      = 0;
   Bool        servBandPres   = FALSE;
   Bool        tgtBandPres    = FALSE;
   

   /*find out index of serving band and target band in the ue capability list*/
   if(ueCb->ueCap->eUTRA_Cap.rf_Params.supportedBandLstEUTRA.noComp.pres 
         != NOTPRSNT)
   {
      numElmnts = ueCb->ueCap->eUTRA_Cap.rf_Params.supportedBandLstEUTRA
                  .noComp.val;
      for(idx=0;idx<numElmnts;idx++)
      {
         if(ueCb->ueCap->eUTRA_Cap.rf_Params.supportedBandLstEUTRA.member[idx].
               bandEUTRA.val == servingBand)
         {
           /*ccpu00134566 -Modify- void CPUH crashed at wrUmmMeasGapReqdInterfreq  while 
            attaching Single UE*/
            servBandPres = TRUE;
            break;
         }
      }
   }
   else
   {
      RLOG0(L_ERROR, "Supported band list not present");
      RETVALUE(RFAILED);
   }
   for(targetBandIdx=0;targetBandIdx < numElmnts;targetBandIdx++)
   {
      if(ueCb->ueCap->eUTRA_Cap.rf_Params.supportedBandLstEUTRA.member[targetBandIdx]
           .bandEUTRA.val == targetBand)
      {
         /*ccpu00134566 -Modify- void CPUH crashed at wrUmmMeasGapReqdInterfreq  while 
         attaching Single UE*/
         tgtBandPres = TRUE;
         break;
      }
   }
  /*ccpu00134566 -Modify- void CPUH crashed at wrUmmMeasGapReqdInterfreq  while attaching
    Single UE*/
   if(( TRUE == servBandPres) && ( TRUE == tgtBandPres))
   {
      if(numElmnts == ueCb->ueCap->eUTRA_Cap.measParams.bandLstEUTRA.noComp.val)
      { 
         *result = ueCb->ueCap->eUTRA_Cap.measParams.bandLstEUTRA.member[idx].
 	             interFreqBandLst.member[targetBandIdx].interFreqNeedForGaps.val;
      }
      else
      {
         *result = TRUE;
      }
   }
   else
   {
     *result = FALSE;
   }
   RETVALUE(ROK);
}/*wrUmmMeasGapReqdInterfreq*/

/** @brief This function is responsible for finding out BAND class
 *           for a given DL ARFCN for EUTRA. 
 * @details
 *
 *     Function: wrUmmFndEUTRABand
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] arfcn : 
 * @param [in] *result:
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmFndEUTRABand
(
 U32        arfcn,
 U8         *result
)
{
   U8    cnt = 0;

   if(arfcn < 0 && arfcn > dlArfcnEUTRA[42] )
   {
      RETVALUE(RFAILED);
   }
   for(cnt =0;cnt < 43;cnt++)
   {
      if(arfcn < dlArfcnEUTRA[cnt])
      {
         break;
      }
   }
   *result = cnt+1;

   RETVALUE(ROK);
}/*wrUmmFndEUTRABand*/

/** @brief This function is responsible for finding out BAND class
 *           for a given DL ARFCN for UTRA FDD. 
 * @details
 *
 *     Function: wrUmmFndEUTRABandFDD
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] arfcn : 
 * @param [in] *result:
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC S16 wrUmmFndUTRABandFDD
(
 U32        arfcn,
 U8         *result
)
{
   U8    idx = 0;

   for(idx =0;idx < 26;idx++)
   {
      if(arfcn >= dlArfcnUTRA[idx][0] && arfcn <= dlArfcnUTRA[idx][1])
      {
         *result = idx+1;
         break;
      }
   }
   if(idx > 26)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}/*wrUmmFndUTRABandFDD*/

/** @brief This function is responsible for finding out if a UTRA TDD band is 
 *         supported by UE    
 * @details
 *
 *     Function: wrUmmUtraTddBandSupported
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] ueCb:
 * @param [in] band :  
 * @param [in] *result:
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC Bool wrUmmUtraTddBandSupported
(
 WrUeCb     *ueCb,
 U8          band,
 LwrUtraTddMode    tddMode
)
{
   Bool     result = FALSE;

   switch(tddMode)
   {
      case LWR_UTRA_TDD128:
         result = wrUmmUTRATDD128BandSupported(ueCb, band);
         break;
      case LWR_UTRA_TDD384:
         result = wrUmmUTRATDD384BandSupported(ueCb, band);
         break;
      case LWR_UTRA_TDD768:
         result = wrUmmUTRATDD768BandSupported(ueCb, band);
         break;
      default:
         break;
   }
   RETVALUE(result);
}

/** @brief This function is responsible for finding out if a UTRA FDD band is 
 *         supported by UE    
 * @details
 *
 *     Function: wrUmmUTRAFDDBandSupported
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] ueCb:
 * @param [in] band :  
 * @param [in] *result:
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC Bool wrUmmUTRAFDDBandSupported
(
 WrUeCb     *ueCb,
 U8            band
)
{
   U8       idx        = 0;
   U8       numElmnts;
   Bool     result    = FALSE;
   
   if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraFDD.pres.pres != NOTPRSNT)
   {
      numElmnts = ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraFDD.
                  supportedBandLstUTRA_FDD.noComp.val;
      for(idx =0;idx < numElmnts;idx++)
      {
         if(band == (ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraFDD.
               supportedBandLstUTRA_FDD.member[idx].val + 1))
         {
            result = TRUE;
            break;
         }
      }/*end of for loop*/
   }
   RETVALUE(result);
}/*wrUmmUTRAFDDBandSupported*/


/** @brief This function is responsible for finding out if a Cdma 1XRTT band is
 *         supported by UE    
 * @details
 *
 *     Function: wrUmmChk1xRttCdmaBandSupported
 *
 *         Processing steps:
 *         - Check the bandClass present in Cdma 1XRTT Band List
 *
 * @param [in] ueCb: pointer to ue Control block
 * @param [in] band : band Class 
 * @return S16
 *       -# SUCCESS :   TRUE
 *       -# FAILURE :   FALSE
 */
PUBLIC Bool wrUmmChk1xRttCdmaBandSupported
(
 WrUeCb                      *ueCb,
 U8                           band
)
{
   U8                              idx        = 0;
   U8                              numElmnts;
   Bool                            result    = FALSE;
   NhuUE_EUTRA_CapinterRAT_Params  *interRatPar;

   interRatPar = &ueCb->ueCap->eUTRA_Cap.interRAT_Params;
   
   if(interRatPar->cdma2000_1xRTT.pres.pres != NOTPRSNT)
   {
      numElmnts = interRatPar->cdma2000_1xRTT.supportedBandLst1XRTT.noComp.val;
      for(idx =0;idx < numElmnts;idx++)
      {
         if(interRatPar->cdma2000_1xRTT.supportedBandLst1XRTT.member[idx].pres
                                         != NOTPRSNT)
         {
            if(band == interRatPar->cdma2000_1xRTT.supportedBandLst1XRTT.
                                      member[idx].val)
            {
               result = TRUE;
               break;
            }
         }
      }/*end of for loop*/
   }
   RETVALUE(result);
}/*wrUmmChk1xRttCdmaBandSupported*/

/** @brief This function is responsible for finding out if a Cdma Hrpd band is 
 *         supported by UE    
 * @details
 *
 *     Function: wrUmmChkCdmaHrpdBandSupported
 *
 *         Processing steps:
 *         - Check the bandClass present in Cdma Hrpd Band List
 *
 * @param [in] ueCb: pointer to ue Control block
 * @param [in] band : band Class 
 * @return S16
 *       -# SUCCESS :   TRUE
 *       -# FAILURE :   FALSE
 */
PUBLIC Bool wrUmmChkCdmaHrpdBandSupported
(
 WrUeCb                      *ueCb,
 U8                           band
)
{
   U8                              idx        = 0;
   U8                              numElmnts;
   Bool                            result    = FALSE;
   NhuUE_EUTRA_CapinterRAT_Params  *interRatPar;

   interRatPar = &ueCb->ueCap->eUTRA_Cap.interRAT_Params;
   
   if(interRatPar->cdma2000_HRPD.pres.pres != NOTPRSNT)
   {
      numElmnts = interRatPar->cdma2000_HRPD.supportedBandLstHRPD.noComp.val;
      for(idx =0;idx < numElmnts;idx++)
      {
         if(interRatPar->cdma2000_HRPD.supportedBandLstHRPD.member[idx].pres
                                         != NOTPRSNT)
         {
            if(band == interRatPar->cdma2000_HRPD.supportedBandLstHRPD.
                                      member[idx].val)
            {
               result = TRUE;
               break;
            }
         }
      }/*end of for loop*/
   }
   RETVALUE(result);
}/*wrUmmChkCdmaHrpdBandSupported*/

/** @brief This function is responsible for finding out if a EUTRA band is 
 *         supported by UE    
 * @details
 *
 *     Function: wrUmmEUTRABandSupported
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] ueCb:
 * @param [in] band :  
 * @param [in] *result:
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC Bool wrUmmEUTRABandSupported
(
 WrUeCb                      *ueCb,
 U8                          band,
 U8                          duplexTyp
)
{
   U8                        idx = 0;
   U8                        numElmnts;
   Bool                      result    = FALSE;
   
   if(ueCb->ueCap->eUTRA_Cap.rf_Params.pres.pres != NOTPRSNT)
   {
      numElmnts = ueCb->ueCap->eUTRA_Cap.rf_Params.supportedBandLstEUTRA.noComp.val;
      for(idx =0;idx < numElmnts;idx++)
      {
         if(ueCb->ueCap->eUTRA_Cap.rf_Params.supportedBandLstEUTRA.
              member[idx].pres.pres != NOTPRSNT)
         {
            if(band == ueCb->ueCap->eUTRA_Cap.rf_Params.supportedBandLstEUTRA.
                                      member[idx].bandEUTRA.val)
            {
               result = TRUE;
               break;
            }
         }
      }/*end of for loop*/
   }
   RETVALUE(result);
}/*wrUmmUTRAFDDBandSupported*/

/** @brief This function is responsible for finding out if a UTRA TDD 128 band
 *         supported by UE    
 * @details
 *
 *     Function: wrUmmUTRATDD128BandSupported
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] ueCb:
 * @param [in] band :  
 * @param [in] *result:
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC Bool wrUmmUTRATDD128BandSupported
(
 WrUeCb     *ueCb,
 U8            band
)
{
   Bool     result = FALSE;
   U8       idx     = 0;
   U8       numElmnts;

   if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD128.pres.pres != NOTPRSNT)
   {
      numElmnts = ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD128.
                     supportedBandLstUTRA_TDD128.noComp.val;
      for(idx =0;idx < numElmnts;idx++)
      {
         if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD128.
             supportedBandLstUTRA_TDD128.member[idx].pres != NOTPRSNT)
         {
            if(band == ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD128.
                        supportedBandLstUTRA_TDD128.member[idx].val + 1)
            {
               result = TRUE;
               break;
            }
         }
      }
   }
   RETVALUE(result);
}/*wrUmmUTRATDD128BandSupported*/

/** @brief This function is responsible for finding out if a UTRA TDD 384 band
 *         supported by UE    
 * @details
 *
 *     Function: wrUmmUTRATDD384BandSupported
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] ueCb:
 * @param [in] band :  
 * @param [in] *result:
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC Bool wrUmmUTRATDD384BandSupported
(
 WrUeCb     *ueCb,
 U8            band
)
{
   Bool     result = FALSE;
   U8       idx     = 0;
   U8       numElmnts;

   if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD384.pres.pres != NOTPRSNT)
   {
      numElmnts = ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD384.
         supportedBandLstUTRA_TDD384.noComp.val;
      for(idx =0;idx < numElmnts;idx++)
      {
         if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD384.
             supportedBandLstUTRA_TDD384.member[idx].pres != NOTPRSNT)
         {
            if(band == ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD384.
                  supportedBandLstUTRA_TDD384.member[idx].val + 1)
            {
               result = TRUE;
               break;
            }
         }
      }/*end of for*/
   }
   RETVALUE(result);
}/*wrUmmUTRATDD384BandSupported*/

/** @brief This function is responsible for finding out if a UTRA TDD 768 band
 *         supported by UE    
 * @details
 *
 *     Function: wrUmmUTRATDD768BandSupported
 *
 *         Processing steps:
 *         - build message
 *
 * @param [in] ueCb:
 * @param [in] band :  
 * @param [in] *result:
 * @return S16
 *       -# ROK
 *       -# RFAILED
 */
PUBLIC Bool wrUmmUTRATDD768BandSupported
(
 WrUeCb     *ueCb,
 U8            band
)
{
   Bool     result = FALSE;
   U8       idx     = 0;
   U8       numElmnts;

   if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD768.pres.pres != NOTPRSNT)
   {
      numElmnts = ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD768.
                     supportedBandLstUTRA_TDD768.noComp.val;
      for(idx =0;idx < numElmnts;idx++)
      {
         if(ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD768.
              supportedBandLstUTRA_TDD768.member[idx].pres != NOTPRSNT)
         {
            if(band == ueCb->ueCap->eUTRA_Cap.interRAT_Params.utraTDD768.
                  supportedBandLstUTRA_TDD768.member[idx].val + 1)
            {
               result = TRUE;
               break;
            }
         }
      }/*end of for loop*/
   }
   RETVALUE(result);
}/*wrUmmUTRATDD768BandSupported*/
/** @brief This function is used to check if UE supports 
 *         e_RedirectionGERAN
 *
 * @details
 *
 *     Function: wrUmmCsfbGeranChkERedirGeran
 *     
 *     Processing steps:
 *      - Check the presence of e_RedirectionGERAN_r9 along with access stratum
 *        release
 *
 * @param[in]  ueCb    : UE control block
 * @return Bool
 *    -#Success : TRUE
 *    -#Failure : FALSE
 */
PRIVATE Bool wrUmmCsfbGeranChkERedirGeran
(
 WrUeCb      *ueCb
)
{
   NhuIRAT_ParamsGERAN_v920        *geranV920 = NULLP;
   geranV920 = &ueCb->ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsGERAN_v920;
   if((NhuAccessStratumRlsrel9Enum <=ueCb->ueCap->eUTRA_Cap.accessStratumRls.val) 
         &&  (PRSNT_NODEF == geranV920->pres.pres) 
         &&  (PRSNT_NODEF == geranV920->e_RedirectionGERAN_r9.pres))
   {
      RETVALUE(TRUE);
   }
   RETVALUE(FALSE);
}
/** @brief This function is used to send received Uu message to defer
 *     transaction List.
 *
 * @details
 *
 *     Function: wrUmmStopTransaction
 *     
 *     Processing steps:
 *      - If any Defered transaction present delete from List and copy to UE
 *         transaction list
 *      - release transaction
 *
 * @param[in]  transCb    : transaction control block
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmStopTransaction
(
 WrUmmTransCb                *transCb
)
{
   TRC2(wrUmmStopTransaction)

   if(transCb->transTyp == WR_UMM_RRC_REESTAB_TRANS)
   {
      wrUmmTransComplete(transCb);
   }
   RETVALUE(ROK);
}

/** @brief This function is used to create CSFB transaction.
 *
 * @details
 *
 *     Function: wrUmmCreatCsfbTrans
 *     
 *         Processing steps:
 *     1) create CSFB transaction internal message
 *     2) Update CSFB indicator and LAI into message
 *     3) proces message
 *     
 * @param[in]  transCb    : pointer to CSFb transaction control block
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmCreatCsfbTrans
(
 WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb;
	U16								cellId;
   WrUmmCellCb               *cellCb;
   WrUmmIntCsfbTransMsg      *csfbtransMsg = NULLP;
   WrUmmInternalMsg          *internalMsg;
   WrUmmIncMsg               *msg;
   U8                        intMsgType = 0;
   Void                      *transMsg = NULLP;
   wrCsfbRatType             ratPriority = CDMA2000_1XRTT;
   U8                        redirType = CDMA2000_CSFB;

   TRC2(wrUmmCreatCsfbTrans)

   RLOG0(L_DEBUG, "wrUmmCreatCsfbTrans Called");

   ueCb = transCb->ueCb;
	cellId = ueCb->cellId;
	WR_UMM_GET_CELLCB(cellCb, cellId);
	if (NULLP == cellCb)
	{
		RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL ");
		RETVALUE(RFAILED);
	}
	
	if (RFAILED == wrRatSelectionAlgorithm (transCb,&ratPriority))
   {
      RLOG0(L_ERROR, " Failure in finding PLMN-RAT priority from  wrRatSelectionAlgorithm");
      ueCb->csfbInfo.ratType = MAX_RAT_TYPE;
      redirType = UNKNOWN_CSFB_REDIRECTION_TYPE;
      wrIncKpiCsfbAtt(redirType);
      RETVALUE(RFAILED);	
   }
   /*Set RAT type in UeCb*/
   ueCb->csfbInfo.ratType = ratPriority;

   RLOG1(L_INFO, "Selected CSFB RAT Prtiority = %d", ratPriority);

   if(ueCb->mobCtrlState == WR_HO_NORMAL)
   { 
      if((CDMA2000_1XRTT ==  ratPriority) || (CDMA2000_HRPD == ratPriority))
      {
         if((NhuAccessStratumRlsrel9Enum <=
                  ueCb->ueCap->eUTRA_Cap.accessStratumRls.val) && 
               (ueCb->ueCap->ueCdmaOctStr.pres == TRUE))
         {
            if(TRUE == wrUmmUtilsIsEcsfbSupported(ueCb))
            {
               /* TODO */
               /* Need to Add Special Case for eCSFB Scenarios */
               intMsgType = WR_UMM_INTMSG_ECSFB_TRANS;
               redirType = CDMA2000_CSFB_WITHOUT_PS_HO;
            }
            else
            {
               /*Set CSFB UE state to RRC_REDIR*/
               intMsgType = WR_UMM_INTMSG_CSFB_REDIR_TRANS;
               redirType = CDMA2000_CSFB;
            }      
         }
         /* For Release 8 trigger normal CSFB */
         else
         {
            /*Set CSFB UE state to RRC_REDIR*/
            intMsgType = WR_UMM_INTMSG_CSFB_REDIR_TRANS;
            redirType = CDMA2000_CSFB;
         }
      }
      /*When PLMN-RAT priority is WCDMA/UTRA then call normal CSFB.
       * where RRC release can be sent with re-direction and system Information
       * or without system Information */
      else if((UTRA_FDD ==  ratPriority) || (UTRA_TDD == ratPriority))
      {
         /*Set CSFB UE state to RRC_REDIR*/
         ueCb->csfbInfo.csfbState = WR_CSFB_GSM_UE_STATE_RRC_REDIR;
         intMsgType = WR_UMM_INTMSG_CSFB_REDIR_TRANS;
			ueCb->csfbInfo.ratType = ratPriority;
			ueCb->csfbInfo.redirType = redirType = UTRA_CSFB;       
      } 
      else if(GERAN == ratPriority)
      {
         /* Set CSFB GERAN UE state to CSFB_READY */
         ueCb->csfbInfo.csfbState = WR_CSFB_GSM_UE_STATE_CSFB_READY;
         
         /*INITIAL CONTEXT SETUP will trigger redirection always. This is
          * the current desing choice. Logic in else part can also be applied
          * here*/
         if(WR_UMM_INIT_CTXT_TRANS == transCb->transTyp)
         {
            intMsgType = WR_UMM_INTMSG_CSFB_REDIR_TRANS;
         }
         else
         { /*a. If (ue as-rel is 9 & above and e-RedirectionGERAN-r9  IE is 
             present in UE GERAN CAPABILITY)  then Choose RRC Connection 
             Release with redirection procedure 
             b. Else if (FGI Bit 10 is set to 1(NACC) ) and isCCOAllowed to TRUE   
             then choose Cell Change Order procedure 
             c. Else Choose RRC Connection Release with redirection procedure */ 
            if(TRUE == wrUmmCsfbGeranChkERedirGeran(ueCb))
            {
               intMsgType = WR_UMM_INTMSG_CSFB_REDIR_TRANS;
            }
            else if((TRUE == cellCb->csfbToGeranCfg.isCCOAllowed) &&
                  (TRUE == wrUmmUtilFeatureGrpInd(ueCb->cellId,ueCb->crnti,WR_UMM_CCO_SUPPORTED)))
            {
               intMsgType = WR_UMM_INTMSG_CSFB_CCO_TRANS;
            }
            else
            {
               intMsgType = WR_UMM_INTMSG_CSFB_REDIR_TRANS;
            }
         }
         redirType = GERAN_CSFB;
      }
      else
      {
         RLOG0(L_ERROR,"Invalid ratPriority");
         RETVALUE(RFAILED);
      } 
   }
   else /*For WR_HO_INITIATED*/
   {
      intMsgType = WR_UMM_INTMSG_CSFB_REDIR_TRANS;
   }
   /* When it is triggered for CSFB then only allocate the memory, for ECSFB we
    * are not passing any information so no need to update/allocate memory here
    */
      WR_ALLOC(&csfbtransMsg, sizeof(WrUmmIntCsfbTransMsg));
      if (NULLP == csfbtransMsg)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      /*Store the RatPriority in the Internal message */
      csfbtransMsg->ratPriority = ratPriority;
      if(transCb->transTyp == WR_UMM_UE_CTXT_MOD_TRANS)
      {
         WrUmmCtxtModTrans         *ctxtMod;
         ctxtMod = &(transCb->u.ctxtModTrans);
         wrFillTknU32(&(csfbtransMsg->csFallBackInd), ctxtMod->csFallBackInd.val);
         RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti, "CSFB:: UE Context modification received\
               with CSFallback Indicator for UE"); 
      }
      else
      {
         WrInitCntxtSetupTransCb   *initCtxt;
         initCtxt = &(transCb->u.initCtxtCb);
         wrFillTknU32(&(csfbtransMsg->csFallBackInd), initCtxt->csFallBackInd.val);
         RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti, "CSFB:: Initial Context received \
               with CSFallback Indicator for UE");
      }

      transMsg = (Void *)csfbtransMsg;
      if (ROK != wrUmmBldInternalMsg(&internalMsg, intMsgType,
               transMsg))
      {
         RLOG0(L_ERROR, "Creation of internal message Failed ");
         if(NULLP != csfbtransMsg)
         {
            WR_FREE(csfbtransMsg, sizeof(WrUmmIntCsfbTransMsg));
         }
         RETVALUE(RFAILED);
      }

      WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
      if (NULLP == msg)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         if(NULLP != csfbtransMsg)
         {
            WR_FREE(csfbtransMsg, sizeof(WrUmmIntCsfbTransMsg));
         }
         RETVALUE(RFAILED);
      }
      msg->msgTyp = WR_UMM_INTERNAL_MSG;
      msg->u.internalMsg = internalMsg;
      msg->refCnt = 1;

      wrIncKpiCsfbAtt(redirType);
      wrUmmPrcIncMsg(cellCb, ueCb, msg);
      wrUmmRlsIncMsg(&msg);

      RETVALUE(ROK);
}

PUBLIC S16 wrUmmPrcTgtEnbRelocTmrExpiry
(
 WrUeCb                       *ueCb
 )
{
   WrUmmIncMsg               *msg = NULLP;
   U16                       cellId;
   WrUmmCellCb               *cellCb;

   cellId = ueCb->cellId;
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      /**
       * Log the error through a error debug and release the PDU
       */
      /*127802*/
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   msg->refCnt  = 1;
   msg->msgTyp  = WR_UMM_LCL_CTXT_REL;

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, WR_CAUSE_RADIONW,
         SztCauseRadioNwtx2relocoverall_expiryEnum, INC_KPI_VALUE_BY_ONE);
   RETVALUE(ROK);
}

/** @brief This function is used to find UE capabilities based on feature 
 *  group indicator.
 *
 * @details
 *
 *     Function: wrUmmUtlFeatureGrpInd
 *     
 *         Processing steps:
 *     1) check the bit if bit is '1' return 'TRUE' else 'FALSE'
 *     
 * @param[in]  cellId    : cell identifier
 * @param[in]  crnti     : Crnti
 * @return Bool
 *    -#Success : TRUE
 */
PUBLIC Bool wrUmmUtilFeatureGrpInd
(
 U16                         cellId,
 U16                         crnti,
 WrUmmFgiType                isSupported
)
{
   TknStrBSXL                 *featureGrpInd;
   WrUeCb                     *ueCb = NULLP;
   Bool                       ret = FALSE;
   U32                        featureGrpIndVar;
   
   TRC2(wrUmmUtilFeatureGrpInd)
      
   wrUmmGetUe(&ueCb, cellId, crnti);
   if (ueCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(FALSE);
   }
   featureGrpInd = &ueCb->ueCap->eUTRA_Cap.featureGroupIndicators;
   
   if(featureGrpInd->pres == FALSE)
   {
      RETVALUE(TRUE);
   }
   WR_GET_U32_FRM_OSXL(featureGrpIndVar, ueCb->ueCap->eUTRA_Cap.featureGroupIndicators);
 
   switch(isSupported)
   {
      case WR_UMM_UTRA_PS_SUPPORTED:
      {
         if((featureGrpIndVar & WR_UMM_UTRA_PS_CHK_BIT))
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_INTER_FREQ_HO_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_INTER_HO_CHK_BIT) 
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_1xRTT_SUPPORTED:
      {
         if((featureGrpIndVar & WR_UMM_1XRTT_CHK_BIT))
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_HRPD_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_HRPD_CHK_BIT) 
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_A4_A5_MEAS_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_A4_A5_MEAS_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_NON_ANR_PERD_MEAS_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_NON_ANR_PERD_MEAS_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_ANR_PERD_INTRA_MEAS_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_ANR_INTRA_PERD_MEAS_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_ANR_PERD_INTER_MEAS_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_ANR_INTER_PERD_MEAS_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_ANR_PERD_INTERAT_MEAS_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_ANR_INTERRAT_PERD_MEAS_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_UTRA_MEAS_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_UTRA_MEAS_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_1xRTT_MEAS_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_1XRTT_MEAS_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_HRPD_MEAS_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_HRPD_MEAS_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_INTER_FREQ_MEAS_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_INTER_FREQ_MEAS_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_UTRA_CS_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_UTRA_CS_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
/* Removed DRX flag*/
      case WR_UMM_LONG_DRX_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_LONG_DRX_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_SHORT_DRX_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_SHORT_DRX_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      case WR_UMM_CCO_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_CCO_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      } 
      case WR_UMM_GERAN_MEAS_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_GERAN_MEAS_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      } 
		
		// FOR TDD SPECIFIC MEASUREMENTS:	
		case WR_UMM_UTRA_TDD_PERD_MEAS_SUPPORTED: 
		case WR_UMM_UTRA_TDD_EVNT_MEAS_SUPPORTED: 	
		{
			if (TRUE == wrUmmUtilUtraTddMeasChk(ueCb, isSupported)) 
			{
				ret = TRUE;
			}
			else
			{
				ret = FALSE;
			}
		}
      break;
      case WR_UMM_GERAN_SRVCC_HO_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_GERAN_SRVCC_HO_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      break;
      case WR_UMM_DCI_FORMAT_3A_SUPPORTED:
      {
         if(featureGrpIndVar & WR_UMM_DCI_FORMAT_3A_CHK_BIT)
         {
            ret = TRUE;
         }
         break;
      }
      break;
      default:
        ret = FALSE;
   }
   RETVALUE(ret);
}

/** @brief This function is used to find UE capabilities based on feature 
 *  group indicator v1020.
 *
 * @details
 *
 *     Function: wrUmmUtlFeatureGrpIndV1020
 *     
 *         Processing steps:
 *     1) check the bit if bit is '1' return 'TRUE' else 'FALSE'
 *     
 * @param[in]  cellId    : cell identifier
 * @param[in]  crnti     : Crnti
 * @return Bool
 *    -#Success : TRUE
 */
PUBLIC Bool wrUmmUtilFeatureGrpIndV1020
(
 WrUeCb                      *ueCb,
 WrUmmFgiType                isSupported
)
{
   Bool                       ret = FALSE;
   U32                        featureGrpIndVar;
   NhuUE_EUTRA_Cap            *eUTRACap;
   
   TRC2(wrUmmUtlFeatureGrpIndV1020)
      
   eUTRACap = &ueCb->ueCap->eUTRA_Cap;
   
   if((NOTPRSNT == eUTRACap->nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.featureGroupIndRel10_r10.pres) &&
     (eUTRACap->accessStratumRls.val > NhuAccessStratumRlsrel9Enum)) 
   {
      RETVALUE(TRUE);
   }
   else if(eUTRACap->accessStratumRls.val < NhuAccessStratumRlsrel10Enum)
   {
      RLOG0(L_WARNING, "UE's AS release is less then Rel10");
      RETVALUE(FALSE);
   }
   WR_GET_U32_FRM_OSXL(featureGrpIndVar, 
         ueCb->ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.featureGroupIndRel10_r10);
 
   switch(isSupported)
   {
      case WR_UMM_UTRA_MEAS_RPRT_ECN0_SUPPORTED:
      {
         if((featureGrpIndVar & WR_UMM_UTRA_MEAS_RPRT_BOTH_CHK_BIT))
         {
            ret = TRUE;
         }
         break;
      }

      default:
        ret = FALSE;
   }
   RETVALUE(ret);
} 


/** @brief This function is used to find UE capabilities based on 
 *          Release 9 feature group indicator.
 */
PUBLIC Bool wrUmmUtilFeatureGrpIndRel9
(
 U16                         cellId,
 U16                         crnti,
 WrUmmFgiType                isSupported
 )
{
   TknStrBSXL                 *featureGrpInd;
   WrUeCb                     *ueCb = NULLP;
   Bool                       ret = FALSE;
   U32                        featureGrpIndVar;

   TRC2(wrUmmUtilFeatureGrpIndRel9)

   wrUmmGetUe(&ueCb, cellId, crnti);
   if (ueCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(FALSE);
   }
	
   featureGrpInd = &ueCb->ueCap->eUTRA_Cap_v9a0.featureGroupIndRel9Add_r9;

   if(featureGrpInd->pres == FALSE)
   {
      RETVALUE(FALSE);
   }
	
   WR_GET_U32_FRM_OSXL(featureGrpIndVar, ueCb->ueCap->eUTRA_Cap_v9a0.featureGroupIndRel9Add_r9);

   switch(isSupported)
   {
      case WR_UMM_UTRA_TDD_EVNT_MEAS_SUPPORTED:
         {
            if(featureGrpIndVar & WR_UMM_UTRA_TDD_MEAS_CHK_BIT)
            {
               ret = TRUE;
            }
         }
         break;
      default:
         ret = FALSE;
         break;
   }
   RETVALUE(ret);
}

/** @brief This function is used to find MEAS SUPPORTED based on feature 
 *  group indicator per RAT TYPE.
 *
 * @details
 *
 *     Function: wrUmmChkCdmaFgi
 *     
 *         Processing steps:
 *     1) check the BIT 16 and BIT 24 for 1xRTT return 'TRUE' else 'FALSE'
 *     
 * @param[in]  cellId    : cell identifier
 * @param[in]  crnti     : Crnti
 * @return Bool
 *    -#Success : TRUE
 */
PUBLIC Bool wrUmmChkCdmaFgi
(
 U16                         cellId,
 U16                         crnti
)
{
   Bool                    fgiBit24Chk;
   Bool                    fgiBit16Chk;

   TRC2(wrUmmChkCdmaFgi)
   
      /* Check for CDMA 1xRTT FGI BIT 16 along with BIT 16 */
   fgiBit16Chk = wrUmmUtilFeatureGrpInd(cellId, crnti, 
                              WR_UMM_NON_ANR_PERD_MEAS_SUPPORTED);
   fgiBit24Chk = wrUmmUtilFeatureGrpInd(cellId, crnti, 
                                     WR_UMM_1xRTT_MEAS_SUPPORTED);
   RETVALUE (fgiBit16Chk && fgiBit24Chk);
  /* TODO */
  /* Add for HRPD FGI handling part */
}

PUBLIC S16 wrUmmProcHoAdmitRsp
(
RmuUeHoRsp* hoResp
)
{
   WrCellId                  cellId;
   WrCrnti                   crnti;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;

   cellId = hoResp->bCellId;
   crnti  = hoResp->usCrnti;

   WR_ALLOC(&(msg), sizeof(WrUmmIncMsg));
   if (msg == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   msg->refCnt           = 1;
   msg->msgTyp           = WR_UMM_RRM_UEHO_RESP;
   msg->u.rrmUeHoResp = hoResp;

   wrUmmGetUe(&ueCb, cellId, crnti);
   if (NULLP == ueCb)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CRNTI, crnti, "Error in getting UECB cellId[%u]",
         cellId);

      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(cellCb==NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
     RETVALUE(RFAILED);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

PUBLIC S16 wrUmmPrcRrmUeAdmitResp
(
RmuUeAdmitRsp         *ueAdmitResp
)
{
   WrCellId                  cellId;
   WrCrnti                   crnti;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;

   cellId = ueAdmitResp->bCellId;
   crnti  = ueAdmitResp->usCrnti;

   WR_ALLOC(&(msg), sizeof(WrUmmIncMsg));
   if (msg == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   msg->refCnt           = 1;
   msg->msgTyp           = WR_UMM_RRM_UEADMIT_RESP;
   msg->u.rrmUeAdmitResp = ueAdmitResp;

   wrUmmGetUe(&ueCb, cellId, crnti);
   if (NULLP == ueCb)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CRNTI, crnti, "Error in getting UECB for cellId[%u",
         cellId);

      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(cellCb==NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   wrUmmCommitUeRadioResDedInfo(ueCb, &ueAdmitResp->stUeRadioResCfg);

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

PUBLIC S16 wrUmmPrcRrmUeRelResp
(
RmuUeRelRsp         *ueRelResp
)
{
   WrCellId                  cellId;
   WrCrnti                   crnti;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;

   cellId = ueRelResp->bCellId;
   crnti  = ueRelResp->usCrnti;

   WR_ALLOC(&(msg), sizeof(WrUmmIncMsg));
   if (msg == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   msg->refCnt         = 1;
   msg->msgTyp         = WR_UMM_RRM_UEREL_RESP;
   msg->u.rrmUeRelResp = ueRelResp;

   wrUmmGetUe(&ueCb, cellId, crnti);
   if (NULLP == ueCb)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CRNTI, crnti, "Error in getting UECB for cellId[%u",
         cellId);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(cellCb==NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}
/**
 *  @brief This function calls the transaction processor to process the ERAB
 *         release indication 
 *
 *  @details
 *
 *      Function: wrUmmProcRrmUeRabRelInd 
 *
 *          Processing steps:
 *          - Allocate memory for transaction message
 *          - Update the message type
 *          - call function to process incoming message
 *
 *
 *  @param [in] relIndinfo: pointer to the Ue ERAB release indication  Info
 *  @return S16
 *  -# Success : ROK
 *  -# Failure : RFAILED
 */
PUBLIC S16 wrUmmProcRrmUeRabRelInd
(
RmuUeERabRelInd *relIndinfo
)
{
   WrCellId                  cellId;
   WrCrnti                   crnti;
   WrUmmIncMsg               *msg = NULLP;
   WrUeCb                    *ueCb = NULLP;
   WrUmmCellCb               *cellCb = NULLP;
   WrUmmRabRls               *lcrabrls = NULLP;
   U8                        idx = 0;

   cellId = relIndinfo->bCellId;
   crnti  = relIndinfo->usCrnti;

   WR_ALLOC(&lcrabrls, sizeof(*lcrabrls));
   if (lcrabrls == NULLP)
   {
      RLOG0(L_FATAL, "Failed to allocate memory.");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&(msg), sizeof(WrUmmIncMsg));
   if (msg == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG0(L_FATAL, "Failed to allocate memory.");
      RETVALUE(RFAILED);
   }

   msg->refCnt            = 1;
   msg->msgTyp            = WR_UMM_RAB_RLS_EVNT;
   
   /*Filling wrUmmRabRls from RmuUeERabRelInd*/
   lcrabrls->numrabs   = relIndinfo->stErabRelInfo.bNoErabsToRel;
   lcrabrls->causeTyp  = WR_CAUSE_RADIONW; 
   for(idx = 0; (idx < lcrabrls->numrabs) && (idx < WR_MAX_DRBS); idx++)
   {
      lcrabrls->cause[idx]  = relIndinfo->stErabRelInfo.stErabRelLst[idx].enErabRelCause;
      lcrabrls->erabId[idx] = relIndinfo->stErabRelInfo.stErabRelLst[idx].bErabId;
   }

   msg->u.lcrabrls        = lcrabrls;

   /*Free  relIndinfo*/
   WR_FREE(relIndinfo,sizeof(RmuCommonMsg));

   wrUmmGetUe(&ueCb, cellId, crnti);
   if (NULLP == ueCb)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CRNTI, crnti,"Error in getting UECB for cellId[%u]",
         cellId);

      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(cellCb==NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
} /* end of wrUmmProcRrmUeCfgRsp */ 



PUBLIC S16 wrUmmPrcRrmUeRelInd
(
RmuUeRelInd            *ueRelInd
)
{
   WrUeCb                     *ueCb;
   S16                        ret = RFAILED;

   wrUmmGetUe(&ueCb, ueRelInd->bCellId, ueRelInd->usCrnti);
   if (NULLP == ueCb)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CRNTI, ueRelInd->usCrnti, "Error in getting UECB "
         "for cellId[%u]", ueRelInd->bCellId);

      RETVALUE(RFAILED);
   }

   ueCb->redirectUe = TRUE;
   if ((ueCb->s1ConCb) && (ueCb->s1ConCb->s1apConnState == WR_S1AP_CONNECTED))
   {
      ret = wrUmmSndCtxtRelReq(ueRelInd->usCrnti, ueRelInd->bCellId, WR_UMM_HIGH_PRIOR_UE_ADM);

      if ( ret != ROK)
      {
         /* Log error */
         RLOG0(L_ERROR, "Sending wrUmmSndCtxtRelReq Failed");
      }
   }
   else 
   {
      wrUmmPrcLclCtxtRel(ueCb,NhuRlsCauseotherEnum);
   }

   RETVALUE(ret);
} /* end of wrUmmPrcRrmUeRelInd */

/**
 *  @brief This function calls the transaction processor to process the RRM UE
 *         Recfg Response
 *
 *  @details
 *
 *      Function: wrUmmProcRrmUeCfgRsp
 *
 *          Processing steps:
 *          - Allocate memory for transaction message
 *          - Update the message type
 *          - call function to process incoming message

 *
 *  @param [in] ueRecfgRespPtr : pointer to the Ue Reconfig Response Info
 *  @return S16
 *  -# Success : ROK
 *  -# Failure : RFAILED
 */

PUBLIC S16 wrUmmProcRrmUeCfgRsp
(
RmuUeRecfgRsp       *ueRecfgRespPtr
)
{
   WrCellId                  cellId;
   WrCrnti                   crnti;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;

   cellId = ueRecfgRespPtr->bCellId;
   crnti  = ueRecfgRespPtr->usCrnti;

   WR_ALLOC(&(msg), sizeof(WrUmmIncMsg));
   if (msg == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   msg->refCnt           = 1;
   msg->msgTyp           = WR_UMM_RRM_UERECFG_RESP;
   msg->u.rrmUeRecfgResp = ueRecfgRespPtr;

   wrUmmGetUe(&ueCb, cellId, crnti);
   if (NULLP == ueCb)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CRNTI, crnti,"Error in getting UECB for cellId[%u]",
         cellId);

      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(cellCb==NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   RLOG1(L_INFO, "*****RRM CFGD cqiPmiCfgIdx=%d*****",
      ueRecfgRespPtr->stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.usCqiConfigIndex);
   RLOG2(L_INFO, "**UeIdx=%ld*****SRCfgIdx=%d*****", ueCb->ueIdx,
            ueCb->srCfg.srCfgIdx);
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
} /* end of wrUmmProcRrmUeCfgRsp */ 

/**
 *  @brief This function process the Ue Recfg response received from RRM
 *
 *  @details This function update the CQI Report Config information received 
 *           in the UE recfg response into the UE context. 
 *
 *      Function: wrUmmCommitUeCqiCfgInfo
 *
 *          Processing steps:
 *          - update the CQI Report Cfg info into UE context
 *
 *  @param [in] ueCqiRptCfgPtr  : pointer to the UE CB's CQI Report config Info 
 *  @param [in] rrmCqiRepCfgPtr : pointer to CQI Report config Info in RRM Ue Recfg
 *                                Resp
 *  @return Void
 */

PRIVATE Void wrUmmCommitUeCqiCfgInfo
(
   WrCqiRptCfg                *ueCqiRptCfgPtr,
   RmuUeCqiReportCfg         *rrmCqiRepCfgPtr
)
{
   ueCqiRptCfgPtr->aperCqiEnbld = rrmCqiRepCfgPtr->stAPrdCqiRepCfg.enbld;
   ueCqiRptCfgPtr->cqiAperiodicMode = \
      (RgrAprdCqiMode)rrmCqiRepCfgPtr->stAPrdCqiRepCfg.enPuschReportMode;
   ueCqiRptCfgPtr->nomPdschRsEpreOff = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.nomPdschRsEpreOff;
   
   /* Commented the check while merging the CQI RI changes from T3K */
   //if(rrmCqiRepCfgPtr->prdCqiRepCfg.pres == TRUE)
      ueCqiRptCfgPtr->periodRptSetupPres = TRUE;
      
      ueCqiRptCfgPtr->cqiPucchRsrcIdx = 
         rrmCqiRepCfgPtr->stPrdCqiRepCfg.usCqiResourceIndex;
      
      ueCqiRptCfgPtr->cqipmiCfgIdx = 
         rrmCqiRepCfgPtr->stPrdCqiRepCfg.usCqiConfigIndex;
      /*129311 : Cqi format check*/  
      if(RMU_PCQI_FMT_WIDEBAND == rrmCqiRepCfgPtr->stPrdCqiRepCfg.enCqiFormatInd)
      {
          ueCqiRptCfgPtr->fmtInd = WR_UE_CQI_FMT_WB;
      }
      else
      {
          ueCqiRptCfgPtr->fmtInd = WR_UE_CQI_FMT_SB;
      }
      
//      if(rrmCqiRepCfgPtr->prdCqiRepCfg.factorK.pres == TRUE)
//      {
         ueCqiRptCfgPtr->subbndCqik = rrmCqiRepCfgPtr->stPrdCqiRepCfg.bFactorK;
//      }
      if(rrmCqiRepCfgPtr->stPrdCqiRepCfg.riConfigIndex.bPres == TRUE)
      {
         ueCqiRptCfgPtr->riPres = (Bool)TRUE;
         ueCqiRptCfgPtr->riCfgIdx =
            rrmCqiRepCfgPtr->stPrdCqiRepCfg.riConfigIndex.usRiConfigIndex;
      }
      else
      {
         ueCqiRptCfgPtr->riPres = (Bool)FALSE;
      }   
     
      RLOG0(L_INFO,"HO_TP: ueCqiRptCfgPtr->riPres made false");
      ueCqiRptCfgPtr->simulAckNackAndCQI = 
         rrmCqiRepCfgPtr->stPrdCqiRepCfg.bSimulAckNackCqi;
      
      ueCqiRptCfgPtr->periodicmode = \
         (RgrPrdCqiMode)rrmCqiRepCfgPtr->stPrdCqiRepCfg.enPucchReportMode;
//   }
} /* end of wrUmmCommitUeCqiCfgInfo */

/**
 *  @brief This function updates the UE CB with PHY config dedicated info
 *
 *      Function: wrUmmCommitUePhyCfgDedResInfo
 *
 *          Processing steps:
 *          - For all the resource types
 *          - check the resource type is reconfigured
 *          - update the ueCb with resource info
 *
 *
 *  @param [in] ueCb             : pointer to UE control block
 *  @param [in] phyCfgDedResInfo : pointer to phy config dedicated info
 *                                 configured by RRM
 *  @return Void
 */
PUBLIC Void wrUmmCommitUePhyCfgDedResInfo
(
   WrUeCb                        *ueCb,
   RmuUeDedPhyCfg         *stPhyCfgDed
)
{
   if(stPhyCfgDed->bPhyCfgDedResBitMap & RM_RCM_PHY_DED_RES_CFG_CQI)
   {
      wrUmmCommitUeCqiCfgInfo(&(ueCb->cqiRptCfg), &(stPhyCfgDed->stCqiRepCfg));

      /* Set the Radio resource configuraiotn bitmap for CQI */
      ueCb->bResCfgBitMap |= WR_UE_RADIO_RES_CFG_CQI;
   
   }
   else
   {
      RLOG0(L_WARNING,"CQI Resource Not Configured From RRM");
   }

   if((stPhyCfgDed->bPhyCfgDedResBitMap) & (U8)RM_RCM_PHY_DED_RES_CFG_SR)
   {
      ueCb->srCfg.srCfgIdx = stPhyCfgDed->stSrCfg.usSrIdx;
      ueCb->srCfg.srPUCCHResIdx = stPhyCfgDed->stSrCfg.usSrResIdx;
      ueCb->srCfg.enDsrTransMax = stPhyCfgDed->stSrCfg.enDsrTransMax;
      
      /* Set the Radio resource configuraiotn bitmap for CQI */
      ueCb->bResCfgBitMap |= WR_UE_RADIO_RES_CFG_SR;
   }
   else
   {
      RLOG0(L_WARNING, "SR Resource Not Configured From RRM");
   }
   if((stPhyCfgDed->bPhyCfgDedResBitMap) & (U8)RM_RCM_PHY_DED_RES_CFG_ANT)
   {
      ueCb->antennaInfo.txmode = stPhyCfgDed->stAntInfo.eTxnMode;
   }
   else
   {
      RLOG0(L_INFO, "TM mode not Configured From RRM");
   }
} /* end of wrUmmCommitUePhyCfgDedResInfo */ 

/**
 *  @brief This function updates the UE CB with PHY config dedicated info
 *
 *      Function: wrUmmCommitUeRadioResDedInfo
 *
 *          Processing steps:
 *          - For all the resource types
 *          - check the resource type is reconfigured
 *          - update the ueCb with resource info
 *
 *
 *  @param [in] ueCb             : pointer to UE control block
 *  @param [in] phyCfgDedResInfo : pointer to phy config dedicated info
 *                                 configured by RRM
 *  @return Void
 */
PUBLIC Void wrUmmCommitUeRadioResDedInfo 
(
   WrUeCb                        *ueCb,
   RmuUeRadioResCfg           *stUeRadioResCfg
)
{

/* Removed DRX flag*/
   if((stUeRadioResCfg->bRadioResDedBitMap & 
          RM_RCM_MAC_MAIN_CFG) &&
      (stUeRadioResCfg->stMacMainCfg.bMacMainCfgBitMap & 
          RM_RCM_MAC_MAIN_CFG_DRX))
   {   
      ueCb->drxCfg = *((WrDrxCfg *)&(stUeRadioResCfg->stMacMainCfg.stRmDrxCfg));
      
      ueCb->bResCfgBitMap |= WR_UE_RADIO_RES_CFG_DRX;
     /*127802*/
      RLOG_ARG0(L_DEBUG,DBG_CRNTI,ueCb->crnti,"Commited DRX config in UE Control BLOCK");
   }

   if(stUeRadioResCfg->bRadioResDedBitMap & RM_RCM_PHY_DED_RES_CFG)
   {   
      wrUmmCommitUePhyCfgDedResInfo(ueCb, &stUeRadioResCfg->stPhyCfgDed);
   }
} /* end of wrUmmCommitUePhyCfgDedResInfo */  

/**
 *  @brief This function updates the UE CB with PHY config dedicated info
 *
 *      Function: wrUmmCommitUePhyCfgDedHoResInfo
 *
 *  @param [in] phyCfgDedHoResInfo : pointer to phy config dedicated info
 *                                 configured by RRM
 *  @return Void
 */
PUBLIC Void wrUmmCommitUePhyCfgDedHoResInfo
(
   WrUeCb                        *ueCb,
   RmuUeHoRsp                    *rrmUeHoResp
)
{
   if(rrmUeHoResp->stUeRadioResCfg.bRadioResDedBitMap &
         RM_RCM_PHY_DED_RES_CFG_CQI)
   {
      wrUmmCommitUeCqiCfgInfo(&(ueCb->cqiRptCfg),
            &(rrmUeHoResp->stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg));
   }
} /* end of wrUmmCommitUePhyCfgDedHoResInfo */ 

/** @brief This function responsible to print UMM statistics
 *
 * @details
 *
 *     Function: wrUmmEnbStats
 *
 *         Processing steps:
 *
 */
PUBLIC Void wrUmmEnbStats
(
)
{
   WrUmmCellCb *cellCb = &wrUeDb.cells[0];
   WrUeCb      *ueCb;
   U8          idx;
   U8          *mcc;
   U8          *mnc;
   U32         mme_ue_s1ap_id;
   U32         enb_ue_s1ap_id;

   /*127802*/
   RLOG0(L_DEBUG, "*****************Statistics****************************");
   RLOG1(L_DEBUG,"Number of Active Transaction [%ld]", wrUmmNumTransCnt);
   RLOG1(L_DEBUG,"Number of active UEs         [%ld]", wrUmmNumUeCnt);
   RLOG0(L_DEBUG,"**************************");
   RLOG0(L_DEBUG,"UE Informations:");
   RLOG0(L_DEBUG,"**************************");
    /*127802*/

   for(idx=0; idx < cellCb->maxUesPerCell; idx++)
   {
      if((ueCb = cellCb->ueCbs[idx]) != NULLP)
      {
         mcc = (U8 *)&ueCb->plmnId.mcc;
         mnc = (U8 *)&ueCb->plmnId.mnc;
         if(ueCb->s1ConCb != NULLP)
         {
            mme_ue_s1ap_id = ueCb->s1ConCb->mme_ue_s1ap_id;
            enb_ue_s1ap_id = ueCb->s1ConCb->enb_ue_s1ap_id;
         }
         else
         {
            enb_ue_s1ap_id = mme_ue_s1ap_id = 0;
         }
         /*127802*/
         RLOG0(L_DEBUG,"**************************");
         RLOG_ARG4(L_DEBUG, DBG_UEIDX, ueCb->ueIdx, "mmeId[%lu] RrcConSta[%d] "
            "tmme_s1apId[%lu] tenb_s1apId[%lu]",ueCb->mmeId, ueCb->rrcConnState,
            mme_ue_s1ap_id, enb_ue_s1ap_id);
         RLOG_ARG4(L_DEBUG, DBG_CRNTI, ueCb->crnti, "cellId[%d] PLMN:"
            "MCC[%d:%d:%d]", ueCb->cellId, mcc[0], mcc[1], mcc[2]);
         RLOG3(L_DEBUG, "MNC[%d:%d:%d]", mnc[0], mnc[1], mnc[2]);
         RLOG0(L_DEBUG,"**************************");
      }
   }
  /*127802*/
   RLOG0(L_DEBUG,"RrcStates: 0-IDLE 1-CONNECTING 2-CONNECTED 3-CON_RELEASED");
   RLOG0(L_DEBUG,"**********************************************************");
    /*127802*/
  RETVOID;
} /* end of wrUmmEnbStats */

#ifdef RGR_CQI_REPT
PUBLIC S16 wrUmmSchdStaInd
(
RgrStaIndInfo                *staInd
)
{
   WrUeCb                     *ueCb = NULLP;
   
   TRC2(wrUmmSchdStaInd)
   wrUmmGetUe(&ueCb, staInd->cellId, staInd->crnti);
#ifdef WR_DETECT_RLF
   if(NULLP != ueCb)
   {
      wrStopUeTmr(ueCb, WR_TMR_DETECT_RLF);
      wrStartUeTmr(ueCb, WR_TMR_DETECT_RLF, WR_TMR_DETECT_RLF_VAL);
   }
   else
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, staInd->crnti,"Schd Stats Indication"
         " (CQI Indication) received, UeCb Not Found");
   }
#endif

   /* Inform the CQI report to BRDCM-SON */
   wrIfmSonUeCqiReportInd(staInd, ueCb);

   WR_FREE(staInd, sizeof(RgrStaIndInfo));
   RETVALUE(ROK);
}
#endif

/** @brief This function verifies that UE needs meas Gaps or not.
 *
 * @details
 *
 *     Function: wrUmmChkUeNeedMeasGapForInterFreq
 *
 *         Processing steps:
 *     1) 
 *
 * @param  ueCb : pointer to ueCB.  
 * @param  targetArfcn: target ARFCN value.  
 * @param  output : Carries the ouptut of this function. 
 * @return  ROK 
 */
PUBLIC S16 wrUmmChkUeNeedMeasGapForInterFreq
(
   WrUeCb                      *ueCb, 
   U32                         targetArfcn, 
   Bool                        *output
) 
{
   U32                          servFreq;
   U8                           targetBand;
   U8                           servingBand;

   /*Get the serving cell Serving Band */
   servFreq = wrEmmGetServCarrierFreq(ueCb->cellId);
   wrUmmFndEUTRABand(servFreq,&servingBand);

   /*Get the target Band based on the given arfcn */
   wrUmmFndEUTRABand(targetArfcn,&targetBand);

   /*From the UE Capability find does UE require Meas Gaps */
   wrUmmMeasGapReqdInterfreq(ueCb,servingBand,targetBand, output);

   RETVALUE(ROK);
}   

/** @brief This function configures the Periodic ANR 
 *
 * @details
 *
 *     Function: wrUmmStartAnr
 *
 *         Processing steps:
 *     1) Check the configured ANR algorithm.
 *     2) If P-ANR is configured start P-ANR transaction.
 *     3) If E-ANR is configured start E-ANR transaction.
 *
 * @param[in]  ueCb       : pointer to UE control block
 * @param[in]  reCfgReq   : requests to send RRC Reconfiguration message explicitly.
 * @return
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmStartAnr
(
   WrUeCb               *ueCb, 
   Bool                 recfgReq
)
{
   WrUmmCellCb               *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)ueCb->cellId, "Cell Cb is NULL ");
      RETVALUE(RFAILED);
   }

   /* Based on the ANR Report Configured start either P-ANR or 
    E-ANR */
   if(WR_UMM_STRONG_CELL_CFG == cellCb->measCellCb->anrRptCfg) 
   {
      /* Starting the P-ANR transaction */
      wrUmmStartPeriodicAnrTrans(ueCb);
   }
   else if((WR_UMM_EVENT_A3_CFG == cellCb->measCellCb->anrRptCfg) || 
           (WR_UMM_EVENT_A5_INTER_CFG == cellCb->measCellCb->anrRptCfg))
   {
      /* Starting Event ANR transaction */
      wrUmmStartEventAnrTrans(recfgReq, ueCb);
   }
   RETVALUE(ROK);
}

/** @brief This function configures the Periodic ANR 
 *
 * @details
 *
 *     Function: wrUmmStopAnr
 *
 *         Processing steps:
 *     1) Check the configured ANR algorithm.
 *     2) If P-ANR is configured stop P-ANR transaction.
 *     3) If E-ANR is configured stop E-ANR transaction.
 *
 * @param[in]  ueCb       : pointer to UE control block
 * @param[in]  reCfgReq   : requests to send RRC Reconfiguration message explicitly.
 * @return
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmStopAnr
(
   WrUeCb               *ueCb
)
{
   WrUmmCellCb               *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)ueCb->cellId, "Cell Cb is NULL ");
      RETVALUE(RFAILED);
   }

   /* Based on the ANR Report Configured stop either P-ANR or 
    E-ANR */
   if(WR_UMM_STRONG_CELL_CFG == cellCb->measCellCb->anrRptCfg) 
   {
      /* Stoping the P-ANR transaction */
      wrUmmStopPeriodicAnr(ueCb);
   }
   else if((WR_UMM_EVENT_A3_CFG == cellCb->measCellCb->anrRptCfg) || 
           (WR_UMM_EVENT_A5_INTER_CFG == cellCb->measCellCb->anrRptCfg))
   {
      /* Stoping Event ANR transaction */
      wrUmmStopEventAnrTrans(ueCb);
   }

   RETVALUE(ROK);
}

/** @brief This function is used to configure the Timers for Measurements
 *
 * @details
 *
 *     Function: wrUmmCfgCellMeasTmrs
 *
 *         Processing steps:
 *     1) Update the timer values
 *
 * @param[in]  cellId : cell Identifier.
 * @param[in]  anrMeasParams : pointer to Anr Measurements.
 * @return
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmCfgCellMeasTmrs
(
 U16                         cellId,
 U32                         csfbTransTmrVal
)
{
   WrUmmCellCb               *cellCb = NULLP;
   TRC2(wrUmmCfgCellMeasTmrs)

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   
   cellCb->measCellCb->csfbTmrVal = csfbTransTmrVal;

   RETVALUE(ROK);
}

PUBLIC S16 wrUmmPrcX2ErrInd
(
CztLclErrInd   *cztLclErrInd
)
{
   WrCellId                  cellId;
   WrUmmIncMsg               *msg;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;
   U32                       ueIdx;

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   msg->refCnt  = 1;
   msg->msgTyp  = WR_UMM_X2AP_ERR_IND;
   msg->u.cztLclErrInd = cztLclErrInd;

   cellId = (cztLclErrInd->u.ueInfo.oldUEX2APId & 0x0000ff00) >> 8;
   ueIdx  = (cztLclErrInd->u.ueInfo.oldUEX2APId & 0x000000ff);
   wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIdx);

   if (ueCb == NULLP)
   {
      /* Log the error through a error debug and release the PDU          */
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
         "UEINDEX[%u]", ueIdx);
      wrUmmRlsIncMsg(&msg);
      RETVALUE(RFAILED);
   }
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   
   wrUmmPrcIncMsg(cellCb, ueCb, msg);

   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);

}
PUBLIC S16 wrUmmSchdUeStaInd
(
RgrUeStaIndInfo            *ueStaInd
)
{
   WrUeCb                  *ueCb = NULLP;
   WrUmmCellCb             *cellCb;
   WrUmmIncMsg             *msg = NULLP;
   U8                      intMsgType;

   TRC2(wrUmmSchdUeStaInd);

   switch(ueStaInd->status)
   {
      case RGR_UESTA_MAC_CRNTI_CE_RECVD:
      {
         intMsgType = WR_UMM_INTMSG_MOD_SR_CQI_RES_MSG;
         break;
      }
      /* SPS changes starts */
      case RGR_UESTA_MAC_CRNTI_CE_RECVD_IN_SPS_ACTIVE:
      {
         intMsgType = WR_UMM_INTMSG_UNSET_SR_MASK;
         break;
      }
      /* SPS changes ends */
      default:
      {
         RLOG0(L_ERROR, "Wrong Status is Recieved");
         WR_FREE(ueStaInd, sizeof(RgrUeStaIndInfo));
         RETVALUE(RFAILED);
      }
   }

   WR_UMM_GET_CELLCB(cellCb, ueStaInd->cellId);   

   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "Failed to get cellCB");
      /*Fix for ccpu00126823: ueStaIndInfo coming from SCHD is freed */
      WR_FREE(ueStaInd, sizeof(RgrUeStaIndInfo));
      RETVALUE(RFAILED);     
   }


   wrUmmGetUe(&ueCb, ueStaInd->cellId, ueStaInd->crnti);
   if(ueCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, ueStaInd->crnti,"UE Not Found");
      /*Fix for ccpu00126823: ueStaIndInfo coming from SCHD is freed */
      WR_FREE(ueStaInd, sizeof(RgrUeStaIndInfo));
      RETVALUE(RFAILED);     
   }
    
   if(!((ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_CQI) ||
            (ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_SR)))
   {
      WR_DBGP(WR_ERROR,(WR_PRNT_BUF,
               "UE[%d] Radio Res Not Configured, So Dropping RRC ReConfig \n",
               ueStaInd->crnti));
      WR_FREE(ueStaInd, sizeof(RgrUeStaIndInfo));
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE(ueStaInd, sizeof(RgrUeStaIndInfo));
      RETVALUE(RFAILED);
   }

  /* SPS changes starts */
   if (ueStaInd->status == RGR_UESTA_MAC_CRNTI_CE_RECVD_IN_SPS_ACTIVE)
   {
      ueCb->isSrMaskEnabled = FALSE;
   }
   /* SPS changes ends */

   msg->msgTyp = WR_UMM_INTERNAL_MSG;
   msg->refCnt = 1;


   wrUmmBldInternalMsg(&msg->u.internalMsg, intMsgType, msg);

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);
/* ccpu00133532*/
   WR_FREE(ueStaInd, sizeof(RgrUeStaIndInfo));
   RETVALUE(ROK);
}

/** @brief This function is used to handle the Tx Mode change indication
 * recieved from RRM
 *
 * @details
 *
 *     Function: wrUmmProcRmUeRecfgInd
 *
 * @return S16
 *    -# ROK
 *    -# RFAILED
 */
PUBLIC S16 wrUmmProcRmUeRecfgInd
(
RmuUeRecfgInd         *ueRecfgIndPtr
)
{
   WrUeCb                  *ueCb = NULLP;
   WrUmmCellCb             *cellCb;
   WrUmmIncMsg             *msg = NULLP;

   TRC2(wrUmmProcRmUeRecfgInd);

   WR_UMM_GET_CELLCB(cellCb, ueRecfgIndPtr->bCellId);   

   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "Failed to get cellCB");
      RETVALUE(RFAILED);     
   }

   wrUmmGetUe(&ueCb, ueRecfgIndPtr->bCellId, ueRecfgIndPtr->usCrnti);
   if(ueCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, ueRecfgIndPtr->usCrnti,"UE Not Found");
      RETVALUE(RFAILED);     
   }

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Failed to allocate memory.");
      RETVALUE(RFAILED);
   }
   
   msg->msgTyp = WR_UMM_RRM_UERECFG_IND;
   msg->refCnt = 1;
   msg->u.rrmUeRecfgInd = ueRecfgIndPtr;

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);
   RETVALUE(ROK);
}

/** @brief This function gets a free Transaction ID for RRC Transaction
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Check if Message type > WR_NUM_DLDCCH_MSGS
 *         - Loop over list of Transaction IDs to find a free entity
 *         - When found return the allocated Transaction ID
 *
 *
 * @param [in] ueCb,
 * @param [in] dlDcchMsg,
 * @param [in] transId,
 * @param [in] rrcTransId
 * @return S16
 *    -# ROK
 *    -# RFAILED
 */
PUBLIC S16 wrUmmGetOnGoingRrcTransId
(
WrUeCb                       *ueCb,
U8                           dlDcchMsg,
U8                           transId,
U32                          *rrcTransId
)
{
   U32                       idx;

   if (dlDcchMsg >= WR_NUM_DLDCCH_MSGS)
   {
      RETVALUE(RFAILED);
   }
   for(idx = 0; idx < WR_NUM_RRC_TRANS; idx++)
   {
      if (ueCb->rrcTrans[dlDcchMsg][idx] == transId)
      {
         *rrcTransId = idx;
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
}


/** @brief This function is used to Fill EUTRA redirection information for RRC
 * connection release
 *
 * @details
 *
 *     Function: wrUmmFillEutraRedirectInfo
 *
 *         Processing steps:
 *     1) check the Ue capabilities
 *     2) if Ue cap is not present in ue cb select intra frequency
 *     3) if Ue cap is present in ue cb select inter frequency based on Ue
 *     capabilities
 *
 * @param[in]  ueCb: Ue control block
 * @return
 *    -#Success : WrUmmRedirectInfo*
 *    -#Failure : RFAILED
 */
PUBLIC WrUmmRedirectInfo* wrUmmFillEutraRedirectInfo
(
   WrUeCb                    *ueCb
)
{
   WrUmmCellCb               *cellCb;
   U16                       servFreq;
   U8                        targetBand;
   U8                        isUeSupported;

   WrUmmMeasEutraMeasObjNode *eutraCellObj;
   CmLList                   *lnk;
   WrUmmRedirectInfo         *redirInfo;

   TRC2(wrUmmFillEutraRedirectInfo)

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "Failed to get cellCB");
      RETVALUE(NULLP);     
   }

   if(ueCb->ueCap == NULLP)
   {
      RLOG_ARG0(L_DEBUG, DBG_CRNTI, ueCb->crnti, "UE Capability Not Present. "
            "Redirecting to Serving Frequency");
      servFreq = wrEmmGetServCarrierFreq(ueCb->cellId);
      if(RFAILED == servFreq)
      {
         RLOG0(L_ERROR, "Not able to find serving cell frequency");
         RETVALUE(NULLP);     
      }
      WR_ALLOC(&redirInfo, sizeof(WrUmmRedirectInfo));
      if (NULLP == redirInfo)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(NULLP);
      }
      redirInfo->cellId = ueCb->cellId;
            redirInfo->ratTyp = WR_UMM_MEAS_INTRA_FREQ;
      redirInfo->val.eutraInfo.arfcn = servFreq;
      RETVALUE(redirInfo);
   }

   RLOG_ARG0(L_DEBUG, DBG_CRNTI, ueCb->crnti, "Checking for Inter Frequency");
   lnk = cellCb->measCellCb->interMeasObjLst.first;
   while (NULLP != lnk)
   {
      /*Meas Objects for inter frequencies*/
      eutraCellObj = (WrUmmMeasEutraMeasObjNode*)lnk->node;

      /*Get bandclass for corresponding ARFCN*/
      wrUmmFndEUTRABand(eutraCellObj->carrFreq,&targetBand);

      /*Check whether the bandclass is UE supported or not*/
      isUeSupported = wrUmmEUTRABandSupported(ueCb,targetBand, 0);

      if(isUeSupported == TRUE)
      {
         RLOG_ARG1(L_DEBUG, DBG_CRNTI, ueCb->crnti, "Redirecting to EUTRAN Inter "
               "Frequency %ld", eutraCellObj->carrFreq);
         WR_ALLOC(&redirInfo, sizeof(WrUmmRedirectInfo));
         if (NULLP == redirInfo)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(NULLP);
         }
         redirInfo->ratTyp = WR_UMM_MEAS_INTER_FREQ;
         redirInfo->val.eutraInfo.arfcn = eutraCellObj->carrFreq;
         RETVALUE(redirInfo);
      }
      lnk = lnk->next;
   }
   RETVALUE(NULLP);
} /* End of wrUmmFillEutraRedirectInfo */

/** @brief This function is used to Fill redirection information for RRC
 * connection release
 *
 * @details
 *
 *     Function: wrUmmFillRedirectInfo
 *
 *         Processing steps:
 *         1) Select inter-frequency redirection information
 *         2) If inter-frequency redirection is not available, 
 *            select inter-RAT redirection and fill the information,
 *            based on RAT priority configured and UE's capability,
 *         2) If inter-frequency nor inter-RAT is available, fill redirection
 *            for serving frequency.
 *
 * @param[in]  transCb: Transaction control block
 * @return
 *    -#Success : WrUmmRedirectInfo*
 *    -#Failure : RFAILED
 */
PUBLIC WrUmmRedirectInfo* wrUmmFillRedirectInfo
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRedirectInfo         *redirInfo = NULLP;
   WrUeCb                    *ueCb = transCb->ueCb;
   wrCsfbRatType             ratPriority = MAX_RAT_TYPE;
   U32                       state = transCb->state;
   U32                       errCause = transCb->errorCause;

   TRC2(wrUmmFillRedirectInfo)

   redirInfo = wrUmmFillEutraRedirectInfo(ueCb);
   if(NULLP == redirInfo)
   {
      /* Reusing existing API to select RAT based on UE's capability and 
       * PLMN RAT priority */
      if(RFAILED == wrRatSelectionAlgorithm(transCb, &ratPriority))
      {
         /* Reverting transCb states. In this case default case is hit */
         transCb->state = state;
         transCb->errorCause = errCause;
      }

      switch(ratPriority)
      {
         case UTRA_FDD:
         {
            RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti, 
                  "Filling UTRA FDD Information");
            redirInfo = wrUmmFillUtraFddRedirectInfo(ueCb, FALSE);
         }
         break;
         case UTRA_TDD:
         {
            RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti, 
                  "Filling UTRA TDD Information");
            redirInfo = wrUmmFillDfltUtraTddRedirectInfo(ueCb, FALSE);
         }
         break;
         case GERAN:
         {
            RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti, 
                  "Filling GERAN Redirect Information");
            wrGetCsdomainPLMNNeighFreqs(ueCb, ueCb->regLai.plmnId, 
                  &redirInfo);
         }
         break;
         case CDMA2000_HRPD:
         case CDMA2000_1XRTT:
         {
            RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti, 
                  "Filling CDMA Redirect Information");
            redirInfo = wrUmmFillCdmaRedirectInfo(ueCb, ratPriority);
         }
         break;
         default:
         {
            /* If UE doesn't support any other RATs nor INTER FREQ,
             * serving frequency is being filled here */
            RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti, 
                  "Filling serving frequency");
            WR_ALLOC(&redirInfo, sizeof(WrUmmRedirectInfo));
            if (NULLP == redirInfo)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(NULLP);
            }
            redirInfo->cellId = ueCb->cellId;
            redirInfo->ratTyp = WR_UMM_MEAS_INTRA_FREQ;
            redirInfo->val.eutraInfo.arfcn =
               wrEmmGetServCarrierFreq(ueCb->cellId);
         }
         break;
      }
   }

   RETVALUE(redirInfo);
}

/** @brief: This function will access the UE Cap info.
 *          to check if it supports eRedirUtrafor both Rel9 and Rel10 UEs.
 *          And fill the same in RedirInfo.
 * */
PUBLIC Void wrUmmFilleRedirUtraSup
(
WrUeCapInfo         *ueCap,
WrUmmRedirectInfo   *redirInfo 
 )
{
   redirInfo->eRedirUtraR9Sup = FALSE; 
   redirInfo->val.utraTddInfo.eRedirUtraR10Sup = FALSE; 

   if(NhuAccessStratumRlsrel9Enum <= ueCap->eUTRA_Cap.accessStratumRls.val)
   {
      /* Check for th eRedirection R9 IE  */
      if(( ueCap->eUTRA_Cap.nonCriticalExtn.pres.pres == PRSNT_NODEF) &&
            ( ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsUTRA_v920.pres.pres == PRSNT_NODEF))
      {
         if( ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsUTRA_v920.e_RedirectionUTRA_r9.val == 
               NhuIRAT_ParamsUTRA_v920e_RedirectionUTRA_r9supportedEnum)
         {
            redirInfo->eRedirUtraR9Sup = TRUE; 
         }
      }
   }

   if((redirInfo->ratTyp == WR_UMM_MEAS_UTRA_TDD_FREQ) && 
         (NhuAccessStratumRlsrel10Enum == ueCap->eUTRA_Cap.accessStratumRls.val))
   {
      /* Check for th eRedirection R10 IE  */
      if((ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.pres.pres == PRSNT_NODEF) && 
            (ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.pres.pres == PRSNT_NODEF))
      {
         NhuUE_EUTRA_Cap_v1020_IEs  *rel10UtraTddCap = 
            &ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn;

         if((rel10UtraTddCap->interRAT_ParamsUTRA_TDD_v1020.pres.pres == PRSNT_NODEF) && 
               (rel10UtraTddCap->interRAT_ParamsUTRA_TDD_v1020.e_RedirectionUTRA_TDD_r10.val ==
                NhuIRAT_ParamsUTRA_TDD_v1020e_RedirectionUTRA_TDD_r10supportedEnum))
         {
            redirInfo->val.utraTddInfo.eRedirUtraR10Sup = TRUE; 
         }
      }
   }
}

/** @brief This function is used to update the UE service type
 *
 * @details
 *
 *     Function: wrUmmUpdtUeServType
 *
 *         Processing steps:
 *     1) Access the E-RAB info in the ueCb
 *     2) If there are no emergency bearers then set the service type
 *        to NORMAL else set it to EMERGENCY
 *
 * @param[in]  ueCb: Ue control block
 * @return
 *    -#Success : ROK
 */
PUBLIC Void wrUmmUpdtUeServType
(
 WrUeCb                       *ueCb
 )
{
   U8       idx = 0;
   Bool     isEmerBearExist = FALSE;

   /* Traverse the UE's RAB info and update the UE service type */
   for(idx = 0; idx < (ueCb->rbInfo.numOfRbCfgd + WR_UMM_ERAB_OFFSET); idx++)
   {
      if((ueCb->rbInfo.rab[idx].inUse == TRUE) &&
            (ueCb->rbInfo.rab[idx].isEmergencyBearer == TRUE))
      {
         isEmerBearExist = TRUE;
         break;
      }
   }

   if(isEmerBearExist == TRUE)
   {
      ueCb->ueServTyp = WR_UMM_EMERGENCY_SERV;
   }
   else
   {
      ueCb->ueServTyp = WR_UMM_NORMAL_SERV;
   }
}

/** @brief This function is used to initialise UeCb
 *
 * @details
 *
 *     Function: wrUmmInitUeCb
 *
 *         Processing steps:
 *     1) initializes ueCb
 *
 * @param[in]  ueCb: Ue control block
 * @return
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmInitUeCb 
(
WrUeCb                       *ueCb
)
{
   WrCellCb             *cellCb;
   U16                  cellId = ueCb->cellId;
   U8                   idx = 0;

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
 /* Reset the Radio resource configuraiotn bitmap */
   ueCb->bResCfgBitMap                     = 0;
   
   /* GRP_PWR TODO: Decision needs to be taken on whether  DCI format 3/3a will be used for the UE. 
         Currently, assuming format 3 for all UEs */
   ueCb->uePuschPwr.pres                   = 
   wrUeDb.cells[WR_DFLT_CELL_IDX].puschDedicated.puschPwrCfg.pres;
   if (ueCb->uePuschPwr.pres == TRUE)
   {
      ueCb->uePuschPwr.isFmt3a                = FALSE;
      WR_GET_PUSCH_FMT3_TPC_RNTI_AND_IDX(ueCb->uePuschPwr.tpcRnti, ueCb->uePuschPwr.idx, ueCb, cellCb);      
   }
   ueCb->uePucchPwr.pres                   = wrUeDb.cells[WR_DFLT_CELL_IDX].uePucchPwr.pres;
   if (ueCb->uePucchPwr.pres == TRUE)
   {
      ueCb->uePucchPwr.isFmt3a                = FALSE;
      WR_GET_PUCCH_FMT3_TPC_RNTI_AND_IDX(ueCb->uePucchPwr.tpcRnti, ueCb->uePucchPwr.idx, ueCb, cellCb);  
   } 

   /*PDSCH */
   ueCb->pdschDedCfg.p_a                   = wrUeDb.cells[WR_DFLT_CELL_IDX].pdschCfgDedicated.pA;

   /*PUCCH*/
   ueCb->pucchDedCfg.pres                  = TRUE;
   ueCb->pucchDedCfg.repeatFactor          = 0;
   ueCb->pucchDedCfg.n1pucchAn             = 0;
   ueCb->pucchDedCfg.tddAckNackFdbkMode    = 0; /*MIMO*/

   /* PUSCH */
   ueCb->puschDedCfg.ackIdx                = wrUeDb.cells[WR_DFLT_CELL_IDX].puschDedicated.ackIdx;
   ueCb->puschDedCfg.riIdx                 = wrUeDb.cells[WR_DFLT_CELL_IDX].puschDedicated.riIdx;
   ueCb->puschDedCfg.cqiIdx                = wrUeDb.cells[WR_DFLT_CELL_IDX].puschDedicated.cqiIdx;

   /*Power Control*/
   ueCb->ulPwrDedCtrl.p0UePusch            = cellCb->ulPwrCtrl.p0UePUSCH; 
   ueCb->ulPwrDedCtrl.deltaMcsEnabled      = cellCb->ulPwrCtrl.deltaMCSenabled;
   ueCb->ulPwrDedCtrl.accumlationEnabled   = cellCb->ulPwrCtrl.accumulationEnabled;
   ueCb->ulPwrDedCtrl.p0UePucch            = cellCb->ulPwrCtrl.p0UePUCCH;
   ueCb->ulPwrDedCtrl.pSrsOff              = cellCb->ulPwrCtrl.pSRSoffset;
   ueCb->ulPwrDedCtrl.filterCoeff          = cellCb->ulPwrCtrl.filterCoefficient;
   ueCb->ulPwrDedCtrl.trgCqi               = cellCb->rgrCfg.dfltTrgUlCqi;

   /* Reset the Radio resource configuraiotn bitmap */
   ueCb->bResCfgBitMap                     = 0;
   if(TRUE == wrSmEnableSrs)
   {
      wrUmmGetSrsCfgIdx(ueCb, ueCb->ueIdx);
      ueCb->srsUlDedCfg.pres = wrSmEnableSrs;    
      ueCb->srsUlDedCfg.srsBw = wrSmDfltSrsUeBandwidthCfg;
      ueCb->srsUlDedCfg.srsHoppingBw = wrSmDfltSrsUEHoppingBandwidth;
      /* SUBBU :- Harcoding for now, need to
       * revisit for multiple UE support
       *           * and dynamic derivation of
       *           these values */
      ueCb->srsUlDedCfg.freqDomPos = 0; 
      ueCb->srsUlDedCfg.duration = TRUE;
      ueCb->srsUlDedCfg.transComb = 0;
      ueCb->srsUlDedCfg.cyclicshift = 0;
      RLOG2(L_INFO, "[SRS] wrUmmInitUeCb, srsUlDedCfg.pres[%d], wrSmEnableSrs[%d]", 
            ueCb->srsUlDedCfg.pres, wrSmEnableSrs);
   }
   else
   {
      ueCb->srsUlDedCfg.pres                  = FALSE;
   }

   ueCb->cqiRptCfg.aperCqiEnbld            = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.aperCqiEnbld;
   ueCb->cqiRptCfg.cqiAperiodicMode        = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.cqiAperiodicMode;
   ueCb->cqiRptCfg.nomPdschRsEpreOff       = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.nomPdschRsEpreOff; 
   ueCb->cqiRptCfg.periodRptSetupPres      = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.periodRptSetupPres;
   ueCb->cqiRptCfg.cqiPucchRsrcIdx         = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.cqiPucchRsrcIdx;

   /* syed same cqiIdx for all UEs could pose issues when multiple UEs attach at around same time.
   *  UEIdx 0, 1, 2, 3 would get 78, 80, 82, 84. */
   ueCb->cqiRptCfg.cqipmiCfgIdx = ((ueCb->ueIdx * 2 + 1) % WR_UMM_DLCQI_PERIODICITY + WR_UMM_CQI_PMI_CONFIG_IDX);
   ueCb->cqiRptCfg.subbndCqik              = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.subbndCqik;
   ueCb->cqiRptCfg.fmtInd                  = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.fmtInd;
   ueCb->cqiRptCfg.riCfgIdx                = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.riCfgIdx;
   ueCb->cqiRptCfg.simulAckNackAndCQI      = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.simulAckNackAndCQI;
   ueCb->cqiRptCfg.periodicmode            = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.periodicmode;
   ueCb->cqiRptCfg.riPres                  = wrUeDb.cells[WR_DFLT_CELL_IDX].cqiRptCfg.riPres;

   ueCb->pdcpCfg.discardTmr                = wrUeDb.cells[WR_DFLT_CELL_IDX].pdcpInfo.discardTimer;
   ueCb->pdcpCfg.statusRptReq              = wrUeDb.cells[WR_DFLT_CELL_IDX].pdcpInfo.statRprtReqrd;
   ueCb->pdcpCfg.snSize                    = wrUeDb.cells[WR_DFLT_CELL_IDX].pdcpInfo.umSnSize;

   ueCb->harqinfo.ulMaxHarqTx              = wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.maxHARQ_Tx;
   ueCb->harqinfo.deltaHarqOffset          = wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.deltaHqOffset;

   WR_GET_SR_CFGINDX(ueCb->srCfg.srCfgIdx, ueCb);
   ueCb->srCfg.srPUCCHResIdx               = wrUeDb.cells[WR_DFLT_CELL_IDX].schReqCfg.srPucchRsrcIdx; 
   ueCb->srCfg.enDsrTransMax               = wrUeDb.cells[WR_DFLT_CELL_IDX].schReqCfg.dsrTransMax;

   ueCb->qosCfg.ambrPres                   = FALSE;
   ueCb->qosCfg.dlAmbr                     = 0;
   ueCb->qosCfg.ueBr                       = wrUeDb.cells[WR_DFLT_CELL_IDX].qosCfg.ueBr;
   if(wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.timeAlignmentTimerDedicated
         == WR_TIME_ALIGNMENT_TIMER_INFINITE)
   {
      ueCb->taTmrCfg.pres = FALSE; 
   }
   else
   {
      ueCb->taTmrCfg.pres                     = TRUE;
      if(wrUtlGetDedTaTmrVal(wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.timeAlignmentTimerDedicated,
               &(ueCb->taTmrCfg.taTmr))==RFAILED)
         {
            ueCb->taTmrCfg.pres = FALSE; 
            RLOG0(L_ERROR, "Dedicated ta timer value is not in the defined range");
         }  
   }
   /* TODO - The values 2 and 10 are not needed. They are a stop */
   /* gap solution till MAC issue is resolved.                   */
   ueCb->ueAckNackRepCfg.isAckNackEnabled  = FALSE;
   ueCb->ueAckNackRepCfg.pucchAckNackRep   = 10;
   ueCb->ueAckNackRepCfg.ackNackRepFactor  = 2;

   ueCb->ueMeasGapCfg.isMesGapEnabled      = FALSE;

   ueCb->ueCapCfg.pwrClass                 = 0;
   ueCb->ueCapCfg.intraSfFeqHop            = FALSE;
   ueCb->ueCapCfg.resAloocType1            = FALSE;
   ueCb->ueCapCfg.simCqiAckNack            = FALSE;
   ueCb->ueCapCfg.txAntSel                 = FALSE;
   ueCb->antennaInfo.ueTxAntSel            = 0;

   ueCb->ueUlHqCfg.maxUlHqTx               = wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.maxHARQ_Tx;
   ueCb->ueUlHqCfg.deltaHqOffst            = wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.deltaHqOffset;
   
   /* RBInfo filling*/
   ueCb->rbInfo.lchChnlCfg.priority        = cellCb->srbPolicy[0].lchInfo.priority;
   ueCb->rbInfo.lchChnlCfg.pbr             = cellCb->srbPolicy[0].lchInfo.pbr;
   ueCb->rbInfo.lchChnlCfg.lcg             = cellCb->srbPolicy[0].lchInfo.lchGrpId;
   ueCb->rbInfo.lchChnlCfg.bucketSzDur     = cellCb->srbPolicy[0].lchInfo.bucketSizeDuration;

   ueCb->rbInfo.srbRlcCfg.tPollRetransmit  = cellCb->srbPolicy[0].rlcInfo.tpRetransmit;
   ueCb->rbInfo.srbRlcCfg.pollPdu          = cellCb->srbPolicy[0].rlcInfo.pollPdu;
   ueCb->rbInfo.srbRlcCfg.pollByte         = cellCb->srbPolicy[0].rlcInfo.pollByte;
   ueCb->rbInfo.srbRlcCfg.maxRetxThreshold = cellCb->srbPolicy[0].rlcInfo.maxRetransThresh;
   ueCb->rbInfo.srbRlcCfg.tReorderingDl      = cellCb->srbPolicy[0].rlcInfo.timerReorderingDl;
   ueCb->rbInfo.srbRlcCfg.tReorderingUl      = cellCb->srbPolicy[0].rlcInfo.timerReorderingUl;
   ueCb->rbInfo.srbRlcCfg.tStatusProhibit  = cellCb->srbPolicy[0].rlcInfo.timerStatusProhibit;

   /* SPS changes starts */
   ueCb->spsCfg.spsRnti = 0;
   ueCb->spsCfg.spsCfgDl.bIsSpsEnabled = FALSE;
   ueCb->spsCfg.spsCfgUl.bIsSpsEnabled = FALSE;
   ueCb->isSrMaskEnabled = FALSE; /* SR Mask feature disabled */
   for(idx = 0; idx < WR_UMM_MAX_LCG_GRP_ID; idx++)
   {
      ueCb->mappedLcgInfo[idx].effUlGbr = 0;
      ueCb->mappedLcgInfo[idx].effUlMbr = 0;
   }
   /* SPS changes ends */
   /*CSG_DEV Start */
  /* Initialize UE membership list */
  if(RFAILED == cmHashListInit(&ueCb->csgMbrLst,
    WR_UMM_MAX_CSG_MBR_ENTRY, WR_UMM_MAX_CSG_MBR_LST_OFFSET,
    FALSE, CM_HASH_KEYTYPE_U32MOD,
     WR_MEM_REGION, WR_MEM_POOL))
  {
    RLOG0(L_ERROR, "Failure in the Hash List Initilization");
    RETVALUE(RFAILED);
  }
  cmLListInit(&ueCb->csgNbrBlLst);
  /* CSG_DEV end */

   RETVALUE(ROK);
}/*wrUmmInitUeCb*/


/** @brief This function is used to initiation of updation of attributes using
 * QCI values
 *
 * @details
 *
 *     Function: wrEmmUpdateEstabInitAttNbr
 *
 *         Processing steps:
 *     1) initiation of updation of attributes using QCI values
 *
 * @param[in]  cellCb: cell Control Block
 *             rabSetup: rabsetup info
 * @return: void
 */
PRIVATE Void wrEmmUpdateEstabInitAttNbr
(
 WrCellCb *cellCb,
 WrUmmRabSetupTrans * rabSetup
 )
{
   U32 i;
   for(i = 0; i < rabSetup->numRabs; i++)
   {
      switch(rabSetup->rabs->qci + QCI_OFFSET)
      {
         case WR_EMM_ERAB_QCI_1:
            cellCb->wrCellStats.wrErabStat.estabInitAttNbr.qci1 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_2:
            cellCb->wrCellStats.wrErabStat.estabInitAttNbr.qci2 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_3:
            cellCb->wrCellStats.wrErabStat.estabInitAttNbr.qci3 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_4:
            cellCb->wrCellStats.wrErabStat.estabInitAttNbr.qci4 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_5:
            cellCb->wrCellStats.wrErabStat.estabInitAttNbr.qci5 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_6:
            cellCb->wrCellStats.wrErabStat.estabInitAttNbr.qci6 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_7:
            cellCb->wrCellStats.wrErabStat.estabInitAttNbr.qci7 += WR_EMM_DFLT_POS_STEP;
            break;
        case WR_EMM_ERAB_QCI_8:
            cellCb->wrCellStats.wrErabStat.estabInitAttNbr.qci8 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_9:
            cellCb->wrCellStats.wrErabStat.estabInitAttNbr.qci9 += WR_EMM_DFLT_POS_STEP;
            break;

        default:
            /*127802*/
            RLOG1(L_ERROR, "Unknow QCI Value [%d] Received ",rabSetup->rabs->qci);
            break;
      }/*rabSetup->rabs->qci + QCI_OFFSET) */
   }/*for(i = 0; i < rabSetup->numRabs; i++)*/
}

/** @brief This function is used to initiation of updation of ERAB attributes using
 * QCI values(success number)
 *
 * @details
 *
 *     Function: wrEmmUpdateEstabInitSuccNbr
 *
 *         Processing steps:
 *     1) initiation of updation of ERAB attributes using QCI values(success number)
 *
 * @param[in]  cellCb: cell Control Block
 *             rabSetup: rabsetup info
 * @return: void
 */
PRIVATE Void wrEmmUpdateEstabInitSuccNbr
(
 WrCellCb *cellCb,
 WrUmmRabSetupTrans * rabSetup
 )
{
   U32 i;
   for(i = 0; i < rabSetup->numRabs; i++)
   {
      switch(rabSetup->rabs->qci + QCI_OFFSET)
      {
         case WR_EMM_ERAB_QCI_1:
            cellCb->wrCellStats.wrErabStat.estabInitSuccNbr.qci1 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_2:
            cellCb->wrCellStats.wrErabStat.estabInitSuccNbr.qci2 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_3:
            cellCb->wrCellStats.wrErabStat.estabInitSuccNbr.qci3 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_4:
            cellCb->wrCellStats.wrErabStat.estabInitSuccNbr.qci4 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_5:
            cellCb->wrCellStats.wrErabStat.estabInitSuccNbr.qci5 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_6:
            cellCb->wrCellStats.wrErabStat.estabInitSuccNbr.qci6 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_7:
            cellCb->wrCellStats.wrErabStat.estabInitSuccNbr.qci7 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_8:
            cellCb->wrCellStats.wrErabStat.estabInitSuccNbr.qci8 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_9:
            cellCb->wrCellStats.wrErabStat.estabInitSuccNbr.qci9 += WR_EMM_DFLT_POS_STEP;
            break;

        default:
            /*127802*/
            RLOG1(L_ERROR, "Unknow QCI Value [%d] Received ",rabSetup->rabs->qci);
            break;
      }/*switch(rabSetup->rabs->qci + QCI_OFFSET) */
   }/*for(i = 0; i < rabSetup->numRabs; i++)*/

}

/** @brief This function is used to updation of ERAB attributes using
 * QCI values
 *
 * @details
 *
 *     Function: wrEmmUpdateEstabAttNbr
 *
 *         Processing steps:
 *     1) initiation of updation of ERAB attributes using QCI values
 *
 * @param[in]  cellCb: cell Control Block
 *             rabSetup: rabsetup info
 * @return: void
 */
PRIVATE Void wrEmmUpdateEstabAttNbr
(
 WrCellCb *cellCb,
 WrUmmRabSetupTrans * rabSetup
 )
{
   U32 i;
   for(i = 0; i < rabSetup->numRabs; i++)
   {
      switch(rabSetup->rabs->qci + QCI_OFFSET)
      {
         case WR_EMM_ERAB_QCI_1:
            cellCb->wrCellStats.wrErabStat.EstabAttNbr.qci1 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_2:
            cellCb->wrCellStats.wrErabStat.EstabAttNbr.qci2 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_3:
            cellCb->wrCellStats.wrErabStat.EstabAttNbr.qci3 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_4:
            cellCb->wrCellStats.wrErabStat.EstabAttNbr.qci4 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_5:
            cellCb->wrCellStats.wrErabStat.EstabAttNbr.qci5 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_6:
            cellCb->wrCellStats.wrErabStat.EstabAttNbr.qci6 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_7:
            cellCb->wrCellStats.wrErabStat.EstabAttNbr.qci7 += WR_EMM_DFLT_POS_STEP;
            break;
        case WR_EMM_ERAB_QCI_8:
            cellCb->wrCellStats.wrErabStat.EstabAttNbr.qci8 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_9:
            cellCb->wrCellStats.wrErabStat.EstabAttNbr.qci9 += WR_EMM_DFLT_POS_STEP;
            break;

        default:
            /*127802*/
            RLOG1(L_ERROR, "Unknow QCI Value [%d] Received ",rabSetup->rabs->qci);
            break;
      }
   }
}

/** @brief This function is used to updation of ERAB attributes using
 * QCI values(success number)
 *
 * @details
 *
 *     Function: wrEmmUpdateEstabSuccNbr
 *
 *         Processing steps:
 *     1) initiation of updation of ERAB attributes using QCI values(success Number)
 *
 * @param[in]  cellCb: cell Control Block
 *             rabSetup: rabsetup info
 * @return: void
 */
PRIVATE Void wrEmmUpdateEstabSuccNbr
(
 WrCellCb *cellCb,
 WrUmmRabSetupTrans * rabSetup
 )
{
   U32 i;
   for(i = 0; i < rabSetup->numRabs; i++)
   {
      switch(rabSetup->rabs->qci + QCI_OFFSET)
      {
         case WR_EMM_ERAB_QCI_1:
            cellCb->wrCellStats.wrErabStat.EstabSuccNbr.qci1 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_2:
            cellCb->wrCellStats.wrErabStat.EstabSuccNbr.qci2 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_3:
            cellCb->wrCellStats.wrErabStat.EstabSuccNbr.qci3 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_4:
            cellCb->wrCellStats.wrErabStat.EstabSuccNbr.qci4 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_5:
            cellCb->wrCellStats.wrErabStat.EstabSuccNbr.qci5 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_6:
            cellCb->wrCellStats.wrErabStat.EstabSuccNbr.qci6 += WR_EMM_DFLT_POS_STEP;
            break;

        case WR_EMM_ERAB_QCI_7:
            cellCb->wrCellStats.wrErabStat.EstabSuccNbr.qci7 += WR_EMM_DFLT_POS_STEP;
            break;
        case WR_EMM_ERAB_QCI_9:
            cellCb->wrCellStats.wrErabStat.EstabSuccNbr.qci9 += WR_EMM_DFLT_POS_STEP;
            break;

        default:
            /*127802*/
            RLOG1(L_ERROR, "Unknow QCI Value [%d] Received ",rabSetup->rabs->qci);
            break;
      }
   }
}

/* HO_REEST */
PUBLIC S16 wrUmmFwdUuIndPdu
(
  WrUmmTransCb *transCb
)
{ 
   WrUmmTransCb *hoTransCb = NULLP;
   WrUmmIncMsg               *msg = NULLP;
   WrUeCb                    *ueCb;
   
   msg = transCb->msg;
   msg->refCnt++;
   ueCb = transCb->ueCb;

   if ((hoTransCb = wrUmmGetHoTransCb(ueCb)) == NULLP)
   {
      RLOG_ARG2(L_ERROR, DBG_CRNTI, ueCb->crnti,"Transaction CB could not be "
         "retrieved from transId [0x%x] msgType [%d]", ueCb->hoTransId,
          msg->msgTyp);
      RETVALUE(RFAILED);
   }

   /* ccpu00149864: to prevent access of invalid transCb and to detect
    * invalid ho transid */
   if(hoTransCb->transTyp != WR_UMM_S1_SRC_HO_TRANS &&
         hoTransCb->transTyp != WR_UMM_S1_TGT_HO_TRANS &&
         hoTransCb->transTyp != WR_UMM_X2_SRC_HO_TRANS &&
         hoTransCb->transTyp != WR_UMM_X2_TGT_HO_TRANS)
   {
      ueCb->hoTransId = 0;
      ALARM("\nERROR:CRNTI:%d:Invalid Handover transaction ID "
            "[%ld] msgType [%d] transType [%d]\n",
            ueCb->crnti, ueCb->hoTransId,
            msg->msgTyp, hoTransCb->transTyp);
      RLOG_ARG3(L_ERROR, DBG_CRNTI, ueCb->crnti,
            "Invalid Handover transaction ID "
            "[0x%x] msgType [%d] transType [%d]", ueCb->hoTransId,
            msg->msgTyp, hoTransCb->transTyp);
      RETVALUE(RFAILED);
   }

   hoTransCb->msg  = msg;
   wrUmmPrcTrans(hoTransCb);
   wrUmmRlsIncMsg(&msg);
   RETVALUE(ROK);
} /* wrUmmFwdUuIndPdu */

PUBLIC WrUmmTransCb* wrUmmGetHoTransCb (WrUeCb *ueCb)
{ 
   if(ueCb->hoTransId)
   {
     RETVALUE((wrUmmGetTransCbById(ueCb, ueCb->hoTransId)));
   }
   RETVALUE(NULLP);
}

/**
 *  @brief This function used to find the CSFB transaction Id from
 measurement ID
 *
 *  @details Search in transaction List and Deffered transaction List for CSFB 
 *  Transaction Id using measurement ID
 *
 *      Function: wrUmmGetExistingTransId
 *
 *          Processing steps:
 *          - Search in transaction List and Deffered transaction List for CSFB
 *              Transaction Id using measurement ID
 *
 *  @param [in]  transTyp    : transaction type
 *  @param [in]  transLst    : pointer to the transaction list
 *  @param [out] transId     : pointer to transaction Id
 *  @return S16
 *   -# Success : ROK
 *   -# Failure : RFAILED
 */
PUBLIC S16 wrUmmGetExistingTransId
(
 U8                        transTyp,
 CmLListCp                 *transLst, 
 U32                       *transId
 )
{
   CmLList                   *lnk;
   WrUmmTransCb              *transCb;

   TRC2(wrUmmGetExistingTransId)

   lnk = transLst->first;
   while(lnk != NULLP)
   {
      transCb = (WrUmmTransCb *)(lnk->node);
      lnk = lnk->next;
      if(transCb->transTyp == transTyp)
      {
         *transId = transCb->transId;
         RETVALUE(ROK);
      }
      else
      {
         if(ROK == wrUmmGetExistingTransId(transTyp, &transCb->
                  deferredTrans,transId))
         {
            RETVALUE(ROK);
         }
      }
   }
   RETVALUE(RFAILED);
}

/**
 *  @brief This function used to find the LCG ID 
 *
 *      Function: wrUmmGetLcgIdForQci
 *
 *  @param [in]  cellId    : transaction type
 *  @param [out] qci       : qci value
 *  @return S16
 *   -# Success : ROK
 *   -# Failure : RFAILED
 */
PUBLIC U8 wrUmmGetLcgIdForQci
(
 U32   cellId, 
 U8    qci
)
{
   WrRabCfgParams                       *params;
   WrLchCfgParams                       *lch;
   WrUmmCellCb                          *cellCb = NULLP;
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(cellCb == NULLP)
   {
      RLOG0(L_ERROR, " CellCb is NULL at wrUmmGetLcgIdForQci ");
      RETVALUE(RFAILED);
   } 
   params = &cellCb->wrRabCfgParams[qci];
   lch    = &(params->lchParams);
   
   RETVALUE(lch->lchGrp);
}
/** @brief This function is called to release the reestablishment transaction 
 *         immediately.
 *
 * @details
 *
 *     Function: wrUmmStopReEstabTransPrcs
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  defTransCb : Deferred transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmStopReEstabTransPrcs
(
 WrUmmTransCb                 *transCb,
 WrUmmTransCb                 *defTrans
)
{
   TRC2(wrUmmStopReEstabTransPrcs)
   if(defTrans->transTyp == WR_UMM_CSFB_REDIR_TRANS)
   {
      wrUmmTransComplete(transCb);
   }
   /* This scenario will hit when number of re-transmission reaches maximum.
    * This may happen because of mutiple re-establishment which may cause 
    * delay/missing of DAT_CFM for either HoFromEutraPrepReq or DL CDMA
    * Information transfer. Release the ongoing eCSFB transaction
    */
   if(WR_UMM_ECSFB_TRANS == defTrans->transTyp)
   {
      wrUmmTransRel(defTrans);
   }
   if(WR_UMM_DL_CDMA_TRANS == defTrans->transTyp)
   {
      wrUmmTransRel(defTrans);
   }
   RETVALUE(ROK);
}
/** @brief This function Process mobility parameter to eCSFB module.
 *
 * @details
 *
 *     Function: wrUmmCsfbCdma2k1xMobParams
 *
 *         Processing steps:
 *
 * @param[in]  cellId     : cell identity 
 * @param[in]  mobParams  : Pointer to mobility parameters
 * @return S16
 *    -#Success : ROK
 */
 
PUBLIC S16 wrUmmCsfbCdma2k1xMobParams
(
  U16                        cellId,
  LwrCdma2k1xMobParams       *mobParams
)
{
   WrUmmCellCb             *cellCb;

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      /* Log the error through an error debug */
      RLOG1(L_ERROR, " Error in getting CELL CB for cellId = %u ",cellId);
      RETVALUE(RFAILED);
   }

   if(ROK != wrUmmEncCdmaPdu(cellCb, mobParams))
   {
      RLOG0(L_ERROR, "Failed at wrUmmEncCdmaPdu ");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function used to create an internal Csfb transaction
 *   so that eNB shall send RRC connection release towards UE as part 
 *   of csfb transaction.
 *
 * @details
 *
 *     Function: wrUmmCreateIntCsfb
 *
 *         Processing steps:
 *         - Build the Internal msg
 *         - Process the message as part of csfb transaction.
 *
 * @param[in]  cellCb     : Cell Cb inforamation
 * @param[in]  ueCb       : Ue Cb inforamation
 * @param[in]  intMsgType : Internal msg type
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmCreateIntCsfb
 (
   WrUmmCellCb               *cellCb,
   WrUeCb                    *ueCb,
   U8                        intMsgType
 )
{
   WrUmmInternalMsg          *internalMsg =NULLP;
   WrUmmIncMsg               *msg;
   Void                      *transMsg = NULLP;
   WrUmmIntCsfbTransMsg      *csfbtransMsg = NULLP;

   WR_ALLOC(&csfbtransMsg, sizeof(WrUmmIntCsfbTransMsg));
   if (NULLP == csfbtransMsg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   csfbtransMsg->ratPriority = CDMA2000_1XRTT;
   transMsg = (Void *)csfbtransMsg;
   /* send the RRC connection release based on the UE capability. */
   if (ROK != wrUmmBldInternalMsg(&internalMsg, intMsgType,
            transMsg))
   {
      RLOG0(L_ERROR, "wrUmmCreateIntCsfb: creation of internal message Failed \n");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
   if (NULLP == msg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   msg->msgTyp = WR_UMM_INTERNAL_MSG;
   msg->u.internalMsg = internalMsg;
   msg->refCnt = 1;

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);
   RETVALUE(ROK);
} /* End of wrUmmCreateIntCsfb */



/** @brief This function Process RAB Release based on number of DRB's.
 *
 * @details
 *
 *     Function: wrUmmPrcEgtpRabRelS1Err
 *
 *         Processing steps:
 *
 * @param[in]  ueCb       : UE control block 
 * @param[in]  numDrbs    : number of DRB's
 * @param[in]  lclTeid    : local tunnel ID
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmPrcEgtpRabRelS1Err
(
WrUeCb   *ueCb, 
U32      numDrbs, 
U32      lclTeid
)
{
   CmLteCellId          cellId;
   CmLteRbId            rbId;
   WrUmmIncMsg          *msg;
   WrUmmCellCb          *cellCb;
   WrUmmRabRls          *lcrabrls;
   U8                   eRABId = 0;
   U8                   idx = 0;

   if(WR_UMM_SINGLE_RAB_CFG == numDrbs)
   {
      /* if only one rab exsits release the UE */
      wrUmmSndCtxtRelReq(ueCb->crnti, ueCb->cellId,WR_UMM_CTXT_REL_S1_U_ERROR);
   }
   else
   {
      cellId = (lclTeid & 0xff000000) >> 24;
      rbId   = (lclTeid & 0x000000f8) >> 3;
      WR_UMM_GET_CELLCB(cellCb, cellId);
      if (NULLP == cellCb)
      {
         RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "Cell Cb is NULL ");
         RETVALUE(RFAILED);
      }
      for(idx = 0;idx < WR_MAX_DRBS; idx++)
      {
         if((ueCb->rbInfo.rab[idx].rbId == rbId) && 
               (TRUE == ueCb->rbInfo.rab[idx].inUse))

         {
            eRABId = ueCb->rbInfo.rab[idx].eRABId;
            break;
         }
      }
      if((WR_MAX_DRBS == idx) || 
            ((eRABId < WR_UMM_ERAB_OFFSET) || (eRABId > WR_UMM_MAX_ERABID)))
      {
         RLOG1(L_DEBUG, "Not able to Fetch ERAB ID from DRBID : %d \n",
               rbId);
         RETVALUE(RFAILED);
      }
      WR_ALLOC(&msg, sizeof(*msg));
      if (msg == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed. \n");
         RETVALUE(ROK);
      }
      /* release RAB only, as mutiple RAB's are present */
      msg->refCnt = 1;
      msg->msgTyp = WR_UMM_RAB_RLS_EVNT;
      WR_ALLOC(&lcrabrls, sizeof(*lcrabrls));
      if (lcrabrls == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed. \n");
         WR_FREE(msg, sizeof(*msg));
         RETVALUE(ROK);
      }
      lcrabrls->numrabs = 1;
      lcrabrls->erabId[0] = eRABId;
      /* ccpu00149547 */
      lcrabrls->cause[0] = SztCauseTportunspecifiedEnum;
      lcrabrls->causeTyp = WR_CAUSE_TRANSPORT;

      msg->u.lcrabrls = lcrabrls;

      wrUmmPrcIncMsg(cellCb, ueCb, msg);
      wrUmmRlsIncMsg(&msg);
   }
   RETVALUE(ROK);
}

/** @brief This function Process EGTP-U error indication for a UE on 
 *             perticular tunnel.
 *
 * @details
 *
 *     Function: wrUmmPrcEgtpErrInd
 *
 *         Processing steps:
 *
 * @param[in]  lclTeid    : local tunnel ID
 * @param[in]  numDrbs    : number of DRB's
 * @param[in]  ueId       : crnti
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmPrcEgtpErrInd 
(
U32                          lclTeid,
U32                          numDrbs,
U16                          crnti 
)
{   
   U8                   tnlType;
   CmLteCellId          cellId;
   WrUmmIncMsg          *msg;
   WrUeCb               *ueCb;
   WrUmmCellCb          *cellCb;

   TRC2(wrUmmPrcEgtpErrInd);

   cellId = (lclTeid & 0xff000000) >> 24;
   tnlType= (lclTeid & 0x00000007);

   wrUmmGetUe(&ueCb, cellId, crnti);
   if (NULLP == ueCb)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
            "ueId = %u", crnti);
      RETVALUE(RFAILED);
   }

   

   if(tnlType == WR_TNL_NORMAL)
   {
      /* If HO is not in progress */
      if (ueCb->mobCtrlState == WR_HO_NORMAL)
      {
         wrUmmPrcEgtpRabRelS1Err(ueCb, numDrbs, lclTeid);

      } 
      /* If HO is in progress */
      else 
      {
         WR_ALLOC(&msg, sizeof(*msg));
         if (msg == NULLP)
         {
            RLOG0(L_FATAL, "Memory allocation failed. \n");
            RETVALUE(ROK);
         }
         WR_UMM_GET_CELLCB(cellCb, cellId);
         if (cellCb == NULLP)
         {
            RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId, "Cell Cb is NULL ");
            RETVALUE(RFAILED);
         }

         msg->refCnt  = 1;
         msg->msgTyp  = WR_UMM_SND_HANDOVER_CANCEL;

         wrUmmPrcIncMsg(cellCb, ueCb, msg);
         wrUmmRlsIncMsg(&msg);
         wrUmmPrcEgtpRabRelS1Err(ueCb, numDrbs, lclTeid);
      }
   }
   else 
   {
      RLOG0(L_ERROR, "S1-U Error received on Forward Tunnel\n");
   }

   RETVALUE(ROK);
}
/** @brief This function is used to copy 
 *  handover restriction list to UE control Block
 *
 * @details
 *
 *     Function: wrUmmCpyHoRestrnLst
 *
 *         Processing steps:
 *
 * @param[in]    ueCb   : Ue control Block to copy Restriction info
 * @param[in]    hoRestrnLst : HO Restriction list received from Peer
 * @return S16 
 *    -# Success : ROK
 *    -# Failure   : RFAILED
 */
 
PUBLIC S16 wrUmmCpyHoRestrnLst
(
WrUeCb                       *ueCb,
SztHovrRestrnLst             *hoRestrnLst   
)
{
   U8                        idx = 0;
   U8                        idx1 = 0;
   WrHoRstrctInfo            *hoRstrctInfo;

   if(ueCb->hoRstrctInfo == NULLP)
   {
      if (ROK != cmAllocEvnt(sizeof(WrHoRstrctInfo), WR_SZ_MEM_SDU_SIZE, &wrCb.mem,
               (Ptr *)&ueCb->hoRstrctInfo))
      {    
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }/* End of if condition.*/ 
   }
   else
   {  
      /*Overwrite the existing HoRstrnList for this UE*/
      WR_FREE_EVNT(ueCb->hoRstrctInfo);

      if (ROK != cmAllocEvnt(sizeof(WrHoRstrctInfo), WR_SZ_MEM_SDU_SIZE, &wrCb.mem,
               (Ptr *)&ueCb->hoRstrctInfo))
      {    
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }/* End of if condition.*/  
   }
   hoRstrctInfo = ueCb->hoRstrctInfo;

   wrUtlGetPlmnId(&hoRstrctInfo->srvngPlmn, &hoRestrnLst->servingPLMN);
   hoRstrctInfo->noOfEplmn = (U8)hoRestrnLst->equivalentPLMNs.noComp.val;
   WR_GET_MEM(&hoRstrctInfo->memCp,
         (sizeof(WrPlmnId)*hoRstrctInfo->noOfEplmn),
         (Ptr*)&hoRstrctInfo->ePlmn); 
   for(idx = 0;idx < hoRstrctInfo->noOfEplmn;idx++)
   {
      wrUtlGetPlmnId(&hoRstrctInfo->ePlmn[idx], &hoRestrnLst->equivalentPLMNs.member[idx]);
   }
   /*Filling Forbidden TAs*/
   hoRstrctInfo->noOfTAs = (U8)hoRestrnLst->forbiddenTAs.noComp.val;
   WR_GET_MEM(&hoRstrctInfo->memCp,
         (sizeof(WrForbiddenTAs) * hoRstrctInfo->noOfTAs),
        (Ptr*)&hoRstrctInfo->frbdnTAs); 
   for(idx = 0;idx < hoRstrctInfo->noOfTAs;idx++)
   {
      SztForbiddenTAs_Item* forbiddenTA =  
         &hoRestrnLst->forbiddenTAs.member[idx];
      WrForbiddenTAs * wrForbiddenTa = &hoRstrctInfo->frbdnTAs[idx];
      wrUtlGetPlmnId(&wrForbiddenTa->plmnId, 
            &forbiddenTA->pLMN_Identity);
      /*Filling Forbidden TACs*/
      wrForbiddenTa->noOfTac = forbiddenTA->forbiddenTACs.noComp.val;
      WR_GET_MEM(&hoRstrctInfo->memCp,
            (sizeof(WrTac) * wrForbiddenTa->noOfTac),
            (Ptr*)&wrForbiddenTa->tac);  
      for(idx1 = 0; idx1 < wrForbiddenTa->noOfTac; idx1++)
      {
         SztTAC *szTac = &forbiddenTA->forbiddenTACs.member[idx1];
         WrTac  *tac  = &wrForbiddenTa->tac[idx1];
         wrGetU16FrmTknStr4(tac,szTac);

      }
   }

   /*Filling Forbidden LAs*/
   hoRstrctInfo->noOfLAs = (U8)hoRestrnLst->forbiddenLAs.noComp.val;
   WR_GET_MEM(&hoRstrctInfo->memCp,
         (sizeof(WrForbiddenLAs) * hoRstrctInfo->noOfLAs),
         (Ptr*)&hoRstrctInfo->frbdnLAs); 
   for(idx = 0;idx < hoRstrctInfo->noOfLAs;idx++)
   {
      SztForbiddenLAs_Item* forbiddenLA =  
         &hoRestrnLst->forbiddenLAs.member[idx];
      WrForbiddenLAs * wrForbiddenLa = &hoRstrctInfo->frbdnLAs[idx];
      wrUtlGetPlmnId(&wrForbiddenLa->plmnId, 
            &forbiddenLA->pLMN_Identity);
      wrForbiddenLa->noOfLac = forbiddenLA->forbiddenLACs.noComp.val;
      WR_GET_MEM(&hoRstrctInfo->memCp,
            (sizeof(WrLac) * wrForbiddenLa->noOfLac),
            (Ptr*)&wrForbiddenLa->lac);  
      /*Filling Forbidden LACs*/
      for(idx1 = 0; idx1 < wrForbiddenLa->noOfLac; idx1++)
      {
         SztLAC *szLac = &forbiddenLA->forbiddenLACs.member[idx1];
         WrLac  *lac  = &wrForbiddenLa->lac[idx1];
         wrGetU16FrmTknStr4(lac,szLac);

      }
   }
   if(hoRestrnLst->forbiddenInterRATs.pres)
   {
      hoRstrctInfo->frbdnInterRATs.pres = PRSNT_NODEF;
      hoRstrctInfo->frbdnInterRATs.val =  hoRestrnLst->forbiddenInterRATs.val;
   }
   RETVALUE(ROK); 
}
 
/** @brief This function is responsible for finding wheter B2 UTRA
 * configuration can be configured to UE or not from UE capability
 *
 * @details
 *
 *     Function: wrUmmChkUeB2UtraCap
 *
 *
 * @param [in] U16    cellId
 * @param [in] U16    crnti
 * @return S16
 *      -# Success  : ROK
 *      -# Failure  : RFAILED
 */
PUBLIC S16 wrUmmChkUeB2UtraCap
(
U16                          cellId,   
U16                          crnti
)
{

   WrUeCb                     *ueCb = NULLP;
   WrHrlDecisionRet            forbRat = WR_MEAS_MBLTY_APPL_TO_RAT;

   TRC2(wrUmmChkUeB2UtraCap)

   wrUmmGetUe(&ueCb, cellId, crnti);
   if ( ueCb == NULLP )
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
            "CRNTI [%u]", crnti);
      RETVALUE(RFAILED);
   }

	/* To do if SRVCC is support is there we need to CHK 27th bit as well*/
   if( TRUE == wrUmmUtilFeatureGrpInd(cellId, ueCb->crnti,
            WR_UMM_UTRA_PS_SUPPORTED) )
   {
      /* Check whether UTRA RAT is forbidden or not */
      if((ueCb->ueServTyp == WR_UMM_NORMAL_SERV) && (ueCb->hoRstrctInfo != NULLP))
      {
         forbRat = wrChkForFrbRat(WR_FORB_RAT_UTRA, ueCb->hoRstrctInfo);
      }
      if(forbRat == WR_MEAS_MBLTY_NOT_APPL_TO_RAT)
         {                                                                            
            RLOG0(L_ERROR, "wrUmmChkUeB2UtraCap: Handover restriction list "
                  "as UTRA in frdbInterRAT list");
            RETVALUE(RFAILED);
         }
         else
         {
            RLOG0(L_INFO, "wrUmmChkUeB2UtraCap: Handover restriction list "
                  "doesnot have UTRA in frdbInterRAT list hence continue "
                  "with B2 UTRA configuration");
            RETVALUE(ROK);
         }
   }

   RETVALUE(RFAILED);
}


/** @brief This function is responsible for finding wheter B2 UTRA
 * configuration can be configured to UE or not from UE capability
 *
 * @details
 *
 *     Function: wrUmmChkUeB2UtraCap
 *
 *
 * @param [in] U16    cellId
 * @param [in] U16    crnti
 * @return S16
 *      -# Success  : ROK
 *      -# Failure  : RFAILED
 */
PUBLIC S16 wrUmmChkUeB2UtraTddCap
(
U16                          cellId,   
U16                          crnti
)
{

   WrUeCb                     *ueCb = NULLP;
   WrHrlDecisionRet            forbRat = WR_MEAS_MBLTY_APPL_TO_RAT;

   TRC2(wrUmmChkUeB2UtraCap)

   wrUmmGetUe(&ueCb, cellId, crnti);
	
   if ( ueCb == NULLP )
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
            "CRNTI [%u]", crnti);
      RETVALUE(RFAILED);
   }

	/*check if UE supports some TDD bands */
	if (FALSE == wrChkUtranTddSupp(ueCb->ueCap))
	{
      RLOG_ARG1(L_INFO, DBG_CELLID,(U32)cellId, "TDD not supported for CRNTI [%u]", crnti);
      RETVALUE(RFAILED);
   }

	/* To check if FGI bits 22/39 are supported for B2 reports*/
   if( TRUE == wrUmmUtilFeatureGrpInd(cellId, ueCb->crnti,
            WR_UMM_UTRA_TDD_EVNT_MEAS_SUPPORTED) )
   {
      /* Check whether UTRA RAT is forbidden or not */
      if((ueCb->ueServTyp == WR_UMM_NORMAL_SERV) && (ueCb->hoRstrctInfo != NULLP))
      {
         forbRat = wrChkForFrbRat(WR_FORB_RAT_UTRA, ueCb->hoRstrctInfo);
      }
		
      if(forbRat == WR_MEAS_MBLTY_NOT_APPL_TO_RAT)
      {                                                                            
         RLOG0(L_ERROR, "wrUmmChkUeB2UtraCap: Handover restriction list\
               as UTRA in frdbInterRAT list");
         RETVALUE(RFAILED);
      }
      else
      {
         RLOG0(L_INFO, "wrUmmChkUeB2UtraCap: Handover restriction list \
               doesnot have UTRA in frdbInterRAT list hence continue \
               with B2 UTRA configuration");
         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
}


/*DRX QCI*/

/** @brief This function is responsible checking whether GBR bearer exxists or not
 * 
 *
 * @details
 *
 *     Function: wrUmmCheckGBRBearer
 *
 * @param [in] WrUmmIncMsg    *msg
 * @return S16
 *      -# Success  : ROK
 *      -# Failure    : RFAILED
 */
PUBLIC S16 wrUmmCheckGBRBearer
(
WrUmmIncMsg                  *msg
)
{
   
   S1apPdu                   *pdu = msg->u.s1Pdu->pdu;
   SztInitiatingMsg          *initMsg = &pdu->pdu.val.initiatingMsg;
   SztE_RABSetupRqst         *rabSetupIE = &initMsg->value.u.sztE_RABSetupRqst;
   U16                       ieIdx,rabCount;
   SztProtIE_Field_E_RABSetupRqstIEs *ie;
   U16                       numRabs;
   SztE_RABToBeSetupItemBrSUReq *rabInfo = NULLP;
   SztE_RABToBeSetupLstBrSUReq  *rabLst;
            
   
   for(ieIdx = 0;ieIdx < rabSetupIE->protocolIEs.noComp.val; ieIdx++)
   {
      ie = &rabSetupIE->protocolIEs.member[ieIdx];
      switch(ie->id.val)
      {
         case Sztid_E_RABToBeSetupLstBrSUReq:
         {
            rabLst = &(ie->value.u.sztE_RABToBeSetupLstBrSUReq);
            numRabs = rabLst->noComp.val;
            
            for(rabCount = 0;rabCount < numRabs; rabCount++)
            {
               rabInfo = 
                &rabLst->member[rabCount].value.u.sztE_RABToBeSetupItemBrSUReq;
               if(rabInfo->e_RABlevelQoSParams.qCI.val <= (WR_EMM_ERAB_QCI_4 -QCI_OFFSET))
               {
                  RETVALUE(ROK);
               }
            }
            break;
         }
      }
   }
   RETVALUE(RFAILED);
}

/** @brief This function is use to update emergency bearer status
 *  druing attach,erab setup,erab release and UE context modification
 * 
 * @details
 *
 *     Function: wrUmmUpdateUeEmerState
 *
 * @param [in] WrUmmTransCb    Transaction Control Block
 * @param [in] WrUmmCellCb     Cell Control Block
 * @return Void
 *
 */
PUBLIC Void wrUmmUpdateUeEmerState
(
WrUmmTransCb              *transCb,
WrUmmCellCb               *cellCb
)
{
   WrErabInfo             *initCtxtRabInfo;
   U8                     idx;
   
   if(transCb->transTyp == WR_UMM_INIT_CTXT_TRANS)
   {
      for(idx = 0; idx <transCb->u.initCtxtCb.numErabs; idx++)
      {
         initCtxtRabInfo = &(transCb->u.initCtxtCb.erabList[idx]);
         if((initCtxtRabInfo->rabAllowed == TRUE) && 
               (initCtxtRabInfo->tnlSetup == WR_INIT_CTXT_TNL_SETUP_SUCC)&&
               (initCtxtRabInfo->qos.priority == cellCb->rabArpForEmerServ))
         {
            RLOG_ARG0(L_INFO,DBG_CRNTI,transCb->ueCb->crnti,
                  "Emergency bearer is received in initial contxt setup request"
                  "message");
            transCb->ueCb->ueServTyp = WR_UMM_EMERGENCY_SERV;
            break;
         }
      }
   }
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
