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




/************************************************************************
 
     Name:     LTE-MAC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     rg_tom.c 
  
     Sid:      gk_tom.c@@/main/3 - Sat Jul 30 02:21:55 2011
  
     Prg:      sd 
  
**********************************************************************/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_FILE_ID=1;
static int RLOG_MODULE_ID=4096;
/** @file rg_tom.c 
@brief This module does processing related to handling of lower interface APIs 
invoked by PHY towards MAC
*/
/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"       /* common tokens */
#include "rgu.h"           /* RGU defines */
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "crg.h"           /* layer management defines for LTE-MAC */
#include "rg_sch_inf.h"           /* layer management defines for LTE-MAC */
#include "rg.h"            /* defines and macros for MAC */
#include "rg_env.h"            /* defines and macros for MAC */
#include "rg_err.h"            /* defines and macros for MAC */


/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"       /* common tokens */
#include "rgu.x"           /* RGU types */
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "crg.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rgm.h"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"           /* layer management typedefs for MAC */
#include "rg.x"            /* typedefs for MAC */

#ifdef MAC_RLC_UL_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif

/* ADD Changes for Downlink UE Timing Optimization */
#ifndef LTEMAC_DLUE_TMGOPTMZ 
PRIVATE S16 rgTOMUtlProcDlSf ARGS(( RgDlSf *dlSf, RgCellCb   *cellCb,
                                    RgErrInfo  *err));
#else
PUBLIC S16 rgTOMUtlProcDlSf ARGS((RgDlSf *dlSf, RgCellCb *cellCb,
                                  RgErrInfo  *err));
#endif
PRIVATE S16 rgTOMProcCrntiCEInDatInd ARGS((
RgMacPdu          *pdu,
RgUeCb            *prevUeCb,
RgCellCb          *cellCb,
TfuDatInfo        *datInfo,
RgInfCeInfo       *ceInfo,
U8                subframe
));

PRIVATE S16 rgTOMProcCCCHSduInDatInd ARGS((
RgMacPdu          *pdu,
RgUeCb            *prevUeCb,
RgCellCb          *cellCb,
TfuDatInfo        *datInfo,
RgInfCeInfo       *ceInfo,
U8                subframe
));

PUBLIC S16 rgHndlFlowCntrl
(
RgCellCb       *cell,
RgInfSfAlloc        *sfInfo
);

EXTERN S16 RgUiRguFlowCntrlInd(Pst* pst, SuId suId, RguFlowCntrlInd *flowCntrlInd);
#ifdef LTE_L2_MEAS
PRIVATE Void rgTOML2MCompileActiveLCs ARGS
((
 RgCellCb      *cellCb, 
 RgUeCb        *ueCb,
 RgMacPdu      *pdu,
 RgInfCeInfo   *ceInfo 
 ));
PRIVATE S16 rgTOMUtlL2MStoreBufSz ARGS
((
 RgUeCb      *ueCb,
 RgInfCeInfo *ceInfo
 ));

PRIVATE S16 rgTomUtlPrepareL2MUlThrpInfo ARGS
((
   RgCellCb *cellCb,
   RgUeCb  *ueCb,
   RgRguDedDatInd  *dDatInd
));


/* The below table takes lower values of BSR Range for a BSR value
     This is to ensure that outstanding can be decrease to zero upon reception of
     TB, which is not guaranteed if higher Range values are used */
   /* Note: taking value 10 for BSR index 1 */
PRIVATE U32 rgLwrBsrTbl[64] = {
   0, 10, 10, 12, 14, 17, 19, 22, 26,
   31, 36, 42, 49, 57, 67, 78, 91,
   107, 125, 146, 171, 200, 234, 274, 321,
   376, 440, 515, 603, 706, 826, 967, 1132,
   1326, 1552, 1817, 2127, 2490, 2915, 3413, 3995,
   4677, 5476, 6411, 7505, 8787, 10287, 12043, 14099,
   16507, 19325, 22624, 26487, 31009, 36304, 42502, 49759,
   58255, 68201, 79846, 93479, 109439, 128125, 150000
};

#endif

/* local defines */
#define RG_TOM_INF_ALLOC(_pdu, _size, _dataPtr, _ret) {\
   _ret = cmGetMem((Ptr)&_pdu->memCp, _size, (Ptr *)&_dataPtr); \
}

/* global variables */
PUBLIC U32 rgUlrate_tfu;
/* local typedefs */

/* local externs */


/** @brief This function fills the PDSCH data of a downlink subframe 
 *
 * @details
 *
 *     Function: rgTOMUtlFillDatReqPdus 
 *
 *         Processing steps:
 *         - Fill BCCH on DLSCH data using datInfo
 *         - Fill PCCH on DLSCH data using datInfo
 *         - Fill Dedicated data on DLSCH data using datInfo
 *         - Fill RA RSP data using datInfo
 *
 * @param  [out] TfuDatReqInfo *datInfo 
 * @param  [in]  RgDlSf     *dlSf
 * @param  [in]  RgCellCb   *cellCb
 * @param  [out] RgErrInfo *err
 * @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgTOMUtlFillDatReqPdus 
(
 TfuDatReqInfo *datInfo,
 RgDlSf        *dlSf,
 RgCellCb      *cellCb,
 RgErrInfo     *err
 )
#else
PRIVATE S16 rgTOMUtlFillDatReqPdus(datInfo, dlSf, cellCb, err)
 TfuDatReqInfo *datInfo;
 RgDlSf        *dlSf;
 RgCellCb      *cellCb;
 RgErrInfo     *err;
#endif
{
   S16              ret;
   TfuDatReqPduInfo *datReq=NULLP;
 /* Moving node declaration to limited scope for optimization */
   RgDlHqProcCb     *hqCb;
   U8               idx;

   TRC2(rgTOMUtlFillDatReqPdus)

      /* first lets send the BCCH data down to PHY */
   if (dlSf->bcch.tb != NULLP)
   {
      if ((ret = rgGetEventMem((Ptr *)&datReq, sizeof(TfuDatReqPduInfo),
                                &(datInfo->memCp))) != ROK)
      {
         err->errCause = RGERR_TOM_MEM_EXHAUST;
         RLOG_ARG0(L_ERROR,DBG_CELLID,cellCb->cellId,"Memory Exhaustion ");
         RETVALUE(ret);
      }
#ifdef TFU_ALLOC_EVENT_NO_INIT
      datReq->mBuf[1] = 0;
#endif
      datReq->rnti                   =  RG_SI_RNTI;
      datReq->dciInfo                =  dlSf->bcch.pdcch.dci;
      /* Note: SCpyMsgMsg is not done since free of unsent buffer 
       * has been taken care through cell delete by invoking rgTomRlsSf
       * during shutdown */
      datReq->nmbOfTBs               =  1;
#ifndef L2_OPTMZ
      datReq->mBuf[0]                =  dlSf->bcch.tb;
#else
      SFndLenMsg((Buffer *)dlSf->bcch.tb, &(datReq->tbInfo[0].tbSize));
      datReq->tbInfo[0].tbPres             = TRUE;
      datReq->tbInfo[0].lchInfo[0].mBuf[0] = dlSf->bcch.tb;
      datReq->tbInfo[0].numLch             = 1;
      datReq->tbInfo[0].lchInfo[0].numPdu  = 1;
#endif
      cmLListAdd2Tail(&datInfo->pdus, &(datReq->lnk));
      datReq->lnk.node = (PTR)datReq;
#ifdef TFU_UPGRADE      
      /* ccpu00132314-ADD- Adding txPower offset for the PDSCH transmission */
      datReq->txPwrOffset            = dlSf->bcch.txPwrOffset;
#endif      
      /* Setting the pointer to NULL post transmission */
      dlSf->bcch.tb = NULLP;
   }
   /* Fill PCCH data */
   if (dlSf->pcch.tb != NULLP)
   {
      if ((ret = rgGetEventMem((Ptr *)&datReq, sizeof(TfuDatReqPduInfo),
                                &(datInfo->memCp))) != ROK)
      {
         err->errCause = RGERR_TOM_MEM_EXHAUST;
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,"Memory Exhaustion CRNTI:%d",datReq->rnti);
         RETVALUE(ret);
      }
#ifdef TFU_ALLOC_EVENT_NO_INIT
      datReq->mBuf[1] = 0;
#endif     
      datReq->rnti                   =  RG_P_RNTI;
      datReq->dciInfo                = dlSf->pcch.pdcch.dci;
      datReq->nmbOfTBs               =  1;
#ifndef L2_OPTMZ
      datReq->mBuf[0]                =  dlSf->pcch.tb;
#else
      SFndLenMsg((Buffer *)dlSf->pcch.tb, &datReq->tbInfo[0].tbSize);
      datReq->tbInfo[0].tbPres             = TRUE;
      datReq->tbInfo[0].lchInfo[0].mBuf[0] =  dlSf->pcch.tb;
      datReq->tbInfo[0].numLch             = 1;
      datReq->tbInfo[0].lchInfo[0].numPdu  = 1;
#endif
      cmLListAdd2Tail(&datInfo->pdus, &(datReq->lnk));
      datReq->lnk.node = (PTR)datReq;
#ifdef TFU_UPGRADE      
      /* ccpu00132314-ADD- Adding txPower offset for the PDSCH transmission */
      datReq->txPwrOffset            = dlSf->pcch.txPwrOffset;
#endif      
      dlSf->pcch.tb = NULLP;
   }

   for(idx=0; idx < dlSf->numRaRsp; idx++)
   {
      if ((ret = rgGetEventMem((Ptr *)&datReq, sizeof(TfuDatReqPduInfo),
                                &(datInfo->memCp))) != ROK)
      {
         err->errCause = RGERR_TOM_MEM_EXHAUST;
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,"Memory Exhaustion CRNTI:%d",
			 datReq->rnti);
         RETVALUE(ret);
      }
#ifdef TFU_ALLOC_EVENT_NO_INIT
      datReq->mBuf[1] = 0;
#endif    
      datReq->rnti                   =  dlSf->raRsp[idx].pdcch.rnti;
      datReq->dciInfo                = dlSf->raRsp[idx].pdcch.dci;
      datReq->nmbOfTBs               =  1;
#ifndef L2_OPTMZ
      datReq->mBuf[0]                =  dlSf->raRsp[idx].rar;
#else
      SFndLenMsg((Buffer *)dlSf->raRsp[idx].rar, &datReq->tbInfo[0].tbSize);
      datReq->tbInfo[0].tbPres             = TRUE;
      datReq->tbInfo[0].lchInfo[0].mBuf[0] =  dlSf->raRsp[idx].rar;
      datReq->tbInfo[0].numLch             = 1;
      datReq->tbInfo[0].lchInfo[0].numPdu  = 1;
   //   prc_trace_format_string(0x40,3,"UE Id=(%d) tbSz=(%d)",datReq->rnti, datReq->tbInfo[0].tbSize);
#endif
      cmLListAdd2Tail(&datInfo->pdus, &(datReq->lnk));
      datReq->lnk.node = (PTR)datReq;
#ifdef TFU_UPGRADE      
      /* ccpu00132314-ADD- Adding txPower offset for the PDSCH transmission */
      datReq->txPwrOffset            = dlSf->raRsp[idx].txPwrOffset;
#endif
      dlSf->raRsp[idx].rar = NULLP;
   }

   /* Fill Dedicated UE data */
   if (dlSf->tbs.count != 0) 
   {
      CmLList          *node;
      while (dlSf->tbs.first)
      {
         node = dlSf->tbs.first;
         hqCb = (RgDlHqProcCb*)node->node;
         if ((ret = rgDHMSndDatReq (cellCb, dlSf, datInfo, hqCb, err)) != ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cellCb->cellId,
			    "DHM unable to fill DATA request");
            err->errType = RGERR_TOM_TTIIND;
            continue;
         }
      } /* end of while */
   } 

   RETVALUE(ROK);
} /* end of rgTOMUtlFillDatReqPdus*/ 

/** @brief This function does all the processing related to a single downlink
 * subframe.
 *
 * @details 
 *
 *     Function: rgTOMUtlProcDlSf
 *
 *         Processing steps:
 *         - collate control data for all UEs and send to PHY 
 *         - collate data buffers for all UEs and send to PHY 
 *
 * @param  [in] RgDlSf     *dlSf
 * @param  [in] RgCellCb   *cellCb
 * @param  [out] RgErrInfo *err
 * @return S16
 */
/* ADD Changes for Downlink UE Timing Optimization */
#ifndef LTEMAC_DLUE_TMGOPTMZ 
#ifdef ANSI
PRIVATE S16 rgTOMUtlProcDlSf
(
 RgDlSf     *dlSf,
 RgCellCb   *cellCb,
 RgErrInfo  *err
 )
#else
PRIVATE S16 rgTOMUtlProcDlSf (dlSf, cellCb, err)
 RgDlSf     *dlSf;
 RgCellCb   *cellCb;
 RgErrInfo  *err;
#endif
#else
#ifdef ANSI
PUBLIC S16 rgTOMUtlProcDlSf
(
 RgDlSf     *dlSf,
 RgCellCb   *cellCb,
 RgErrInfo  *err
 )
#else
PUBLIC S16 rgTOMUtlProcDlSf (dlSf, cellCb, err)
 RgDlSf     *dlSf;
 RgCellCb   *cellCb;
 RgErrInfo  *err;
#endif
#endif
{
   S16               ret;
   TfuDatReqInfo     *datInfo;

   TRC2(rgTOMUtlProcDlSf);

   /* Fill Data Request Info from scheduler to PHY */   
   if ((ret = rgAllocEventMem((Ptr *)&datInfo, 
                            sizeof(TfuDatReqInfo))) != ROK)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellCb->cellId,"Unable to Allocate TfuDatReqInfo");
      RETVALUE(ret);
   }
   else
   {
      cmLListInit(&datInfo->pdus);
#ifdef LTE_TDD
      RGADDTOCRNTTIME(dlSf->schdTime, datInfo->timingInfo, TFU_DELTA);
#else
      RGADDTOCRNTTIME(dlSf->schdTime, datInfo->timingInfo, TFU_DLDATA_DLDELTA);
#endif
      datInfo->cellId = cellCb->cellId;
#ifdef TFU_ALLOC_EVENT_NO_INIT
      datInfo->bchDat.pres = 0;
#endif

      /* Fill BCH data */
      if (dlSf->bch.tb != NULLP)
      {
         datInfo->bchDat.pres = PRSNT_NODEF;
         datInfo->bchDat.val  = dlSf->bch.tb;
         dlSf->bch.tb = NULLP;
      }
      /* Fill the DLSCH PDUs of BCCH, PCCH and Dedicated Channels */
      if ((ret = rgTOMUtlFillDatReqPdus(datInfo, dlSf, cellCb, err)) != ROK)
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cellCb->cellId,"Unable to send data for cell");
         RG_FREE_MEM(datInfo);
         RETVALUE(ret);
      }
      if((datInfo->pdus.count) || (datInfo->bchDat.pres == TRUE))
      {
         /* sending the data to Phy */
         if (rgLIMTfuDatReq(datInfo) != ROK)
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cellCb->cellId,"Unable to send data info for cell");               
         }
      }
      else
      {
         /* Nothing to send: free the allocated datInfo */
         RG_FREE_MEM(datInfo);
      }
   }
   RETVALUE(ROK);
} /* end of */

U32  rgMacGT;

/**
 * @brief Handler for processing TTI indication recieved from 
 * PHY for a cell.
 *
 * @details
 *
 *     Function: rgTOMTtiInd
 *
 *     Handler for processing TTI indication recieved from PHY
 *     for a cell.
 *
 *     Invoked by: RgLiTfuTtiInd
 *
 *     Processing Steps:
 *     - Get cell and update the cell's current time with the timing value given
 *     by PHY
 *     - Invoke the cmPrcTmr to process the timing queue.
 *     - Append the PHICH information to the downlink subframe that needs to go
 *       out to PHY in this subframe.
 *     - Invoke DHM to release the downlink subframe that occured earlier
 *     rgDHMRlsDlsfHqProc.
 *     - Invoke the TTI handler of scheduler. 
 *     - Invoke the TTI handler of RAM module. 
 *     - Get the downlink subframe that has to go out to PHY in this subframe
 *     rgSCHSubFrmGet.
 *     - Invoke rgTOMUtlProcTA to perform and timing advance processing. 
 *     - Invoke rgTOMUtlProcDlSf to do further processing on the downlink
 *     subframe.
 *     - Get the downlink subframe that would occur after RG_DL_DELTA and 
 *     invoke rgTOMUtlProcDlSfStaInd to send status indications to the higher
 *     layer.
 *     - Invoke GOM's TTI handler rgGOMTtiHndlr
 *     - Invoke COM's TTI handler rgCOMTtiHndlr
 *           
 *  @param[in] TfuTtiIndInfo *ttiInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgTOMTtiInd
(
 TfuTtiIndInfo *ttiInd
 )
#else
PUBLIC S16 rgTOMTtiInd(ttiInd)
   TfuTtiIndInfo *ttiInd;
#endif
{
   RgCellCb             *cellCb;
   RgErrInfo            err;
   RgDlSf               *dlSf;
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   RgDlSf               *prevDlSf;
   CmLteTimingInfo       prevTmInfo;  
#endif

   TRC2(rgTOMTtiInd);

   cellCb = rgDBMGetCellCb (ttiInd->cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,ttiInd->cellId, 
		"Unable to get the cellCb for cell");
      err.errType = RGERR_TOM_TTIIND;
      err.errCause = RGERR_TOM_INV_CELL_ID;
      RETVALUE(RFAILED);
   }
   RGCPYTIMEINFO(ttiInd->timingInfo, cellCb->crntTime);
   rgMacGT = ttiInd->timingInfo.sfn * 10 + ttiInd->timingInfo.subframe;
#ifdef LTE_L2_MEAS
   rgL2Meas(cellCb);
   /*Included to track the number of 10240 cycles completed */

  if((cellCb->crntTime.sfn == 0) && (cellCb->crntTime.subframe==0))
  {
     cellCb->ttiCycle += 1;
  }

#endif

   /*Check if we have transmitted the previous DL SF, it could be the
     case that we haven't received all the DATA from RLC as yet
     and thus we would not have transmitted previous DL SF yet.*/
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   RGSUBFRMCRNTTIME(ttiInd->timingInfo, prevTmInfo, 1);
   prevDlSf = &cellCb->subFrms[prevTmInfo.subframe];
   if(FALSE == prevDlSf->txDone)
   {
      if (ROK != rgTOMUtlProcDlSf (prevDlSf, cellCb, &err))
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cellCb->cellId,"Unable to process "
		"previous downlink subframe for cell");
         err.errType = RGERR_TOM_TTIIND;
      }

      /* Mark this frame as sent */
      prevDlSf->txDone = TRUE;

      if(prevDlSf->remDatReqCnt)
      {
         /*We have not received 1 or more data requests from RLC, this is
           error scenario. MAC & SCH need to discard the allocations for
           which data request hasn't been received as yet. And MAC
           needs to inform SCH about the list of UEs for which 
           allocation need to be discarded. */
         prevDlSf->remDatReqCnt = 0;
      }
   }
#endif
   dlSf = &cellCb->subFrms[ttiInd->timingInfo.subframe];

   if((dlSf->txDone == TRUE) ||
      (!RG_TIMEINFO_SAME(ttiInd->timingInfo,dlSf->schdTime)))
   {
   /* MS_WORKAROUND */
#ifndef LTEMAC_DLUE_TMGOPTMZ
      TfuDatReqInfo     *datInfo;
     CmLteTimingInfo   timingInfo;
#ifdef TFU_DL_DELTA_CHANGE
         RGADDTOCRNTTIME(cellCb->crntTime, timingInfo, TFU_DLDATA_DLDELTA);
#else
         RGADDTOCRNTTIME(cellCb->crntTime, timingInfo, TFU_DELTA);
#endif
     /* Fill Data Request from MAC for BCH  */
     if ((timingInfo.sfn % 4 == 0) && (timingInfo.subframe == 0))
     {
         if (ROK != rgAllocEventMem((Ptr *)&datInfo, 
                            sizeof(TfuDatReqInfo)))
         {
            RLOG_ARG0(L_ERROR,DBG_CELLID,cellCb->cellId,
		"rgTOMUtlProcDlSf() Unable to Allocate TfuDatReqInfo for cell");
            RETVALUE(RFAILED);
         }
         else
         {
            cmLListInit(&datInfo->pdus);
            datInfo->cellId = cellCb->cellId;
            datInfo->bchDat.pres = NOTPRSNT;
            datInfo->timingInfo = timingInfo;
          /* sending the dummy data req to Phy */
            if (rgLIMTfuDatReq(datInfo) != ROK)
            {
               RLOG_ARG0(L_ERROR,DBG_CELLID,cellCb->cellId,
			 "rgTOMUtlProcDlSf() Unable to send data info for cell");
            }
         
         }
     }      
#endif
      /* Freeing as the part of CL Non RT Indication */
      /* TDOD : Generalize for all SOCs */
#if !(defined(TENB_T2K3K_SPECIFIC_CHANGES) && defined(LTE_TDD))
      rgDHMFreeTbBufs();
#endif
      RETVALUE(ROK);
   }

   /*Return if there is still some data to be received
     from RLC for this DL SF. */
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   if(0 != dlSf->remDatReqCnt) 
   {
      /* Freeing as the part of CL Non RT Indication */
      /* TODO : Generalize for all SOCs and remove this flag */
#if !(defined(TENB_T2K3K_SPECIFIC_CHANGES) && defined(LTE_TDD))
      rgDHMFreeTbBufs(); 
#endif
      RETVALUE(ROK);
   }
#endif

   if (ROK != rgTOMUtlProcDlSf (dlSf, cellCb, &err))
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,ttiInd->cellId,
		      "Unable to process downlink subframe for cell");
      err.errType = RGERR_TOM_TTIIND;
   }

   /* Mark this frame as sent */
   dlSf->txDone = TRUE;

   /* Freeing as the part of CL Non RT Indication */
   /* TODO : Generalize for all SOCs and remove this flag */
#if !(defined(TENB_T2K3K_SPECIFIC_CHANGES) && defined(LTE_TDD))
   rgDHMFreeTbBufs();
#endif 
   RETVALUE(ROK);
}  /* rgTOMTtiInd */

/** @brief This function allocates the RgMacPdu that will be populated by DEMUX
 * with the SubHeaders list and the values of the Control elements.
 *
 * @details
 *
 *     Function: rgTOMUtlAllocPduEvnt
 *
 *         Processing steps:
 * @param  [out] RgMacPdu   **pdu
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgTOMUtlAllocPduEvnt 
(
 RgMacPdu         **pdu
 )
#else
PRIVATE S16 rgTOMUtlAllocPduEvnt (pdu)
   RgMacPdu         **pdu;
#endif
{

   Mem               evntMem;
   RgUstaDgn         dgn;      /* Alarm diagnostics structure */
   VOLATILE U32      startTime=0;

   TRC2(rgTOMUtlAllocPduEvnt);

   evntMem.region = rgCb.rgInit.region;
   evntMem.pool   = rgCb.rgInit.pool;

   /*starting Task*/
   SStartTask(&startTime, PID_TOMUTL_CMALLCEVT);

#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */

   if (cmAllocEvnt (sizeof (RgMacPdu), RG_BLKSZ, &evntMem, (Ptr*)pdu) != ROK)
   {
      rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM);
      rgLMMStaInd(LCM_CATEGORY_RESOURCE, LCM_EVENT_DMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      RLOG0(L_ERROR,"Allocation of DUX event failed");
      RETVALUE(RFAILED);
   }

   /*stoping Task*/
   SStopTask(startTime, PID_TOMUTL_CMALLCEVT);

   RETVALUE(ROK);
} /* end of */ 

/** @brief This function frees up the RgMacPdu structure that has been
 * populated by demux.
 *
 * @details
 *
 *     Function: rgTOMUtlFreePduEvnt
 *       - Function frees up the RgMacPdu structure, in case of error it shall
 *       free up the buffer's present in the different sdu.
 *
 *         Processing steps:
 * @param  [in] RgMacPdu   *pdu
 * @param  [in] Bool       *error
 * @return 
 */
#ifdef ANSI
PRIVATE Void rgTOMUtlFreePduEvnt
(
 RgMacPdu *pdu,
 Bool      error
 )
#else
PRIVATE Void rgTOMUtlFreePduEvnt (pdu, error)
   RgMacPdu *pdu;
   Bool      error;
#endif
{

   RgMacSdu       *sdu;
   CmLList        *node;

   TRC2(rgTOMUtlFreePduEvnt);
   /* Steps of freeing up the PDU.
    * 1. loop through the subHdrLst and free up all the buffers.
    * 2. free up the whole event
    */
   if ((error == TRUE) && (pdu->sduLst.count > 0))
   {
      node =  pdu->sduLst.first;
      while (node)
      {
         sdu = (RgMacSdu*)node->node;
         RG_FREE_MSG(sdu->mBuf);
         node = node->next;
      }
   }
   RG_FREE_MEM(pdu);
   RETVOID;
} /* end of rgTOMUtlFreePduEvnt */ 

/** @brief This function allocates the RgMacPdu that will be populated by DEMUX
 * with the SubHeaders list and the values of the Control elements.
 *
 * @details
 *
 *     Function: rgTOMInfAllocPduEvnt
 *
 *         Processing steps:
 * @param  [out] RgMacPdu   **pdu
 *
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE S16 rgTOMInfAllocPduEvnt 
(
RgInfSfDatInd **sfInfo
 )
#else
PRIVATE S16 rgTOMInfAllocPduEvnt (sfInfo)
RgInfSfDatInd **sfInfo;
#endif
{

   Mem               evntMem;
   RgUstaDgn         dgn;      /* Alarm diagnostics structure */
   VOLATILE U32      startTime=0;

   TRC2(rgTOMInfAllocPduEvnt);

   evntMem.region = rgCb.rgInit.region;
   evntMem.pool   = rgCb.rgInit.pool;

   /*starting Task*/
   SStartTask(&startTime, PID_TOMINF_CMALLCEVT);

#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
   if (cmAllocEvnt (sizeof (RgInfSfDatInd), RG_BLKSZ, &evntMem, (Ptr*)sfInfo) != ROK)
   {
      rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM);
      rgLMMStaInd(LCM_CATEGORY_RESOURCE, LCM_EVENT_DMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      RLOG0(L_ERROR,"Allocation failed");
      RETVALUE(RFAILED);
   }

   /*stoping Task*/
   SStopTask(startTime, PID_TOMINF_CMALLCEVT);

   RETVALUE(ROK);
} /* end of */ 

/** @brief This function frees up the RgMacPdu structure that has been
 * populated by demux.
 *
 * @details
 *
 *     Function: rgTOMInfFreePduEvnt
 *       - Function frees up the RgMacPdu structure, in case of error it shall
 *       free up the buffer's present in the different sdu.
 *
 *         Processing steps:
 * @param  [in] RgMacPdu   *pdu
 * @param  [in] Bool       *error
 * @return 
 */
#ifdef ANSI
PRIVATE Void rgTOMInfFreePduEvnt
(
RgInfSfDatInd *sfInfo
 )
#else
PRIVATE Void rgTOMInfFreePduEvnt (sfInfo)
RgInfSfDatInd *sfInfo;
#endif
{
   TRC2(rgTOMInfFreePduEvnt);

   RG_FREE_MEM(sfInfo);
   RETVOID;
} /* end of rgTOMUtlFreePduEvnt */

#ifdef LTE_L2_MEAS

/** @brief This function performs the preparation of information needed to set
 * L2M Scheduled UL Throughput Information for a particular UE.
 *
 * @details
 *
 *     Function: rgTomUtlPrepareL2MUlThrpInfo
 *      This function performs the preparation of information needed to set
 *       L2M Scheduled UL Throughput Information for a particular UE.
 *
 *
 *         Processing steps:
 * @param  [in] RgCellCb   *cellCb
 * @param  [in] RgUeCb       *ueCb
 * @param  [out] RgRguDedDatInd *dDatInd
 * @return 
 */
#ifdef ANSI
PRIVATE S16 rgTomUtlPrepareL2MUlThrpInfo
(
   RgCellCb *cellCb,
   RgUeCb  *ueCb,
   RgRguDedDatInd  *dDatInd
 )
#else
PRIVATE S16 rgTomUtlPrepareL2MUlThrpInfo(cellCb,ueCb,dDatInd)
   RgCellCb *cellCb;
   RgUeCb *ueCb;
   RgRguDedDatInd    *dDatInd;
#endif
{
   U8 lcId;
   U8 lcgId;
   U8 loop;
   TRC2(rgTomUtlPrepareL2MUlThrpInfo);

   dDatInd->burstInd = RGU_L2M_UL_BURST_END;
   for(loop=0;loop<dDatInd->numLch;loop++)
   {
      lcId=dDatInd->lchData[loop].lcId;
      if (lcId)
      {
         lcgId = ueCb->ul.lcCb[lcId - 1].lcgId;
         if(ueCb->ul.lcgArr[lcgId].lcgBsInfo.outStndngBs > 0)
         {
            dDatInd->burstInd = RGU_L2M_UL_BURST_START;
            break;
                  }
               }
   }

   RETVALUE(ROK);
}  /* rgTOMTtiInd */

#endif

/** @brief This function is called by rgTOMDatInd. This function invokes the
 * scheduler with the information of the received Data and any Control Elements
 * if present. Also it generates Data indications towards the higher layers.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - Retrieves the RaCb with the rnti provided, if it doesnt exist
 *         return failure. 
 *         - If UE exists then update the Schduler with any MAC CEs if present. 
 *         - Invoke RAM module to do Msg3 related processing rgRAMProcMsg3
 *         - Loop through the SDU subheaders and invoke either a common data
 *         indication (rgUIMSndCmnDatInd) or dedicated data indication
 *         (rgUIMSndDedDatInd) towards the higher layers. 
 *
 * @param  [in] RgCellCb   *cellCb
 * @param  [in] RgUeCb     *ueCb
 * @param  [in] CmLteRnti  rnti
 * @param  [in] RgMacPdu   *pdu
 * @param  [out] U32       *lcgBytes
 *  
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
 RgUeCb  *glblueCb4;
 RgUeCb  *glblueCb5;

#ifdef LTEMAC_SPS
#ifdef ANSI
PRIVATE S16 rgTOMUtlProcMsg
(
 RgCellCb      *cellCb, 
 RgUeCb        *ueCb,
 RgMacPdu      *pdu,
 Bool          isSpsRnti,
 Bool          *spsToBeActvtd,
 U16           *sduSize,
 U8            subframe,
 U32           *lcgBytes
 )
#else
PRIVATE S16 rgTOMUtlProcMsg(cellCb, ueCb, pdu, isSpsRnti,spsToBeActvtd,sduSize, subframe, lcgBytes)
   RgCellCb      *cellCb; 
   RgUeCb        *ueCb;
   RgMacPdu      *pdu;
   Bool          isSpsRnti;
   Bool          *spsToBeActvtd;
   U16           *sduSize;
   U8            subframe;
   U32           *lcgBytes;
#endif   
#else /* LTEMAC_SPS */
#ifdef ANSI
PRIVATE S16 rgTOMUtlProcMsg
(
 RgCellCb      *cellCb, 
 RgUeCb        *ueCb,
 RgMacPdu      *pdu,
 U8            subframe,
 U32           *lcgBytes
 )
#else
PRIVATE S16 rgTOMUtlProcMsg(cellCb, ueCb, pdu, subframe, lcgBytes)
   RgCellCb      *cellCb; 
   RgUeCb        *ueCb;
   RgMacPdu      *pdu;
   U8            subframe;
   U32           *lcgByes;
#endif  
#endif
{
   S16               ret;
   RgRguCmnDatInd    *cDatInd;
   RgRguDedDatInd    *dDatInd;
   CmLList           *node;
   RgMacSdu          *sdu;
   MsgLen            ccchSz;
   MsgLen            cpySz;
#ifdef LTEMAC_SPS
   Pst               schPst1;  
   RgInfSpsRelInfo   relInfo;
#endif

#ifdef LTE_L2_MEAS
   U8                idx1;
   U8                idx2;
   RgUlSf            *ulSf;
   U16               totalBytesRcvd = 0;
   U16               sduLen[RGU_MAX_LC] = {0};
   U8                qciVal[RGU_MAX_LC] = {0};
   U8                numPrb = 0;

#endif
   U8                lcgId;
   MsgLen            bufSz;

  /* Moved outside of LTE_L2_MEAS
    *          scope as this pointer will now be used to 
    *          check for valid Logical Channel ID
   */
   RgUlLcCb          *ulLcCb;
   
   cDatInd  = NULLP;
   dDatInd  = NULLP;
#ifdef LTE_L2_MEAS
   ulSf   = NULLP;
   idx1   = 0;
   idx2   = 0;
#endif
#ifdef SS_RBUF 
   Void *elem = NULLP;
#endif
  
   ulLcCb = NULLP;
   
   TRC2(rgTOMUtlProcMsg)

#ifndef LTE_L2_MEAS      
      UNUSED(subframe);
#endif

   if(pdu->sduLst.first)
   {
      sdu = (RgMacSdu*)(pdu->sduLst.first->node);
			glblueCb4 = ueCb;
      if ((sdu->lcId == RG_CCCH_LCID))
      {
         /* code for common channel dat indications */
         if ((ret = rgAllocShrablSBuf ((Data**)&cDatInd, sizeof(RgRguCmnDatInd))) != ROK)
         {
            RETVALUE(RFAILED);
         }
         cDatInd->cellId   = cellCb->cellId;
         cDatInd->rnti     = ueCb->ueId;
         /* rg001.101: Corrected lcId value for common data indication */
         cDatInd->lcId     = cellCb->ulCcchId;
         cDatInd->pdu      = sdu->mBuf;
         SFndLenMsg (sdu->mBuf, &ccchSz);
         /* Fix : syed Contention resolution ID copy should consider only
          * 6 bytes of information from sdu->mBuf. Incase of CCCH sdu for reest
          * message/psuedo reest message, ccchSz can go beyond 6 and can corrupt 
          * other fields of ueCb. */
         if (ccchSz >= RG_CRES_LEN)
         {
            SCpyMsgFix (sdu->mBuf, (MsgLen)0, RG_CRES_LEN, ueCb->contResId.resId,
                        &cpySz);
         }
         sdu->mBuf = NULLP;
         rgUIMSndCmnDatInd(cDatInd);
         RETVALUE(ROK);
      } /* end of common channel processing */
#ifndef SS_RBUF 
      if ((ret = rgAllocShrablSBuf ((Data**)&dDatInd, sizeof(RgRguDedDatInd))) != ROK)
      {
         RETVALUE(RFAILED);
      }
#else
			glblueCb5 = ueCb;
      elem = SRngGetWIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
      if (NULLP == elem)
      { 
         RETVALUE(RFAILED);
      }
      dDatInd = (RgRguDedDatInd *)elem;
#endif
      dDatInd->cellId   = cellCb->cellId;
      dDatInd->rnti     = ueCb->ueId;
      dDatInd->numLch   = 0;
   }
#ifdef LTE_L2_MEAS
   ulSf = &cellCb->ulSf[subframe];
   if(ulSf->ueUlAllocInfo != NULLP)
   {
     for(idx1 = 0; idx1 < ulSf->numUe; idx1++)
     {
        if(ulSf->ueUlAllocInfo[idx1].rnti == ueCb->ueId)
        {
           numPrb = ulSf->ueUlAllocInfo[idx1].numPrb;
           break;
        }
     }
   }
#endif
   node =  pdu->sduLst.first;
   while (node)
   {
      sdu = (RgMacSdu*)node->node;
      
      ulLcCb = rgDBMGetUlDedLcCb (ueCb, sdu->lcId);
      
      if(ulLcCb == NULLP)
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cellCb->cellId,"Unconfigured LCID:%d CRNTI:%d"
			 ,sdu->lcId,ueCb->ueId);
         /* ccpu00128443: Fix for memory leak */
         /* Fix : syed Neccessary to set sdu->mBuf = NULLP */
         RG_FREE_MSG(sdu->mBuf);         
         node = node->next;
         continue;
      }

      /* ccpu00116477- Fixed the rgUIMSndDedDatInd condition when we receive 11 sdus in the 
       * list we are losing 11th sdu and sending the first 10 sdus again which
       * is causing the duplicate packets and eNB crashing due to access
       * of the freed memory */
      if (dDatInd->numLch >= RGU_MAX_LC)
      {
         if ((ret = rgUIMSndDedDatInd(dDatInd)) != ROK)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
		"Failed to send datIndication to RGU CRNTI:%d",ueCb->ueId);
            RETVALUE(ret);
         }
#ifndef SS_RBUF
         if ((ret = rgAllocShrablSBuf ((Data**)&dDatInd, sizeof(RgRguDedDatInd))) != ROK)
         {
            RETVALUE(RFAILED);
         }
#else
      elem = SRngGetWIndx(SS_RNG_BUF_ULMAC_TO_ULRLC);
      if (NULLP == elem)
      { 
         RETVALUE(RFAILED);
      }
      dDatInd = (RgRguDedDatInd *)elem;
#endif
         dDatInd->cellId   = cellCb->cellId;
         dDatInd->rnti     = ueCb->ueId;
         dDatInd->numLch   = 0;
      }
      dDatInd->lchData[dDatInd->numLch].lcId = sdu->lcId;
      dDatInd->lchData[dDatInd->numLch].pdu.mBuf[0] = sdu->mBuf;
      dDatInd->lchData[dDatInd->numLch].pdu.numPdu = 1;
      lcgId = ulLcCb->lcgId;
      SFndLenMsg(sdu->mBuf, &bufSz);
#ifdef LTE_L2_MEAS
      if(ulLcCb->measOn)
      {
         ueCb->ul.lcgArr[lcgId].lcgBsInfo.outStndngBs -= bufSz; 
      }
#endif
      //if ((lcgBytes != NULLP) && (ueCb->ul.lcgArr[lcgId].isGbr == TRUE))
      if (lcgBytes != NULLP)
      {
         lcgBytes[lcgId] += bufSz;
      }
      sdu->mBuf = NULLP;
      dDatInd->numLch++;
#ifdef LTEMAC_SPS
      /* Check if data has come on SPS LC */
      /* KWORK_FIX: Modified the index from lcId to lcId-1 for handling lcId 10 properly */
      if (ueCb->ul.spsLcId[sdu->lcId-1] == TRUE)
      {
         ueCb->ul.spsDatRcvd++;
      }
            
      if(isSpsRnti)
      {
         /* Data rcvd on CRNTI*/
         /* Retrieve the LCG ID of the LCID*/
         /* SPS LCG has data whose size > SID Size */
         /* Activate SPS if data recvd on SPS LCID and size > SID Packet Size */
         if((ueCb->ul.spsLcId[sdu->lcId-1] == TRUE) &&
            (sdu->len > RG_SPS_SID_PACKET_SIZE))
         {
             *spsToBeActvtd = TRUE;
             *sduSize = sdu->len;
         }
      }
      
#endif  

#ifdef LTE_L2_MEAS
      if(cellCb->qciArray[ulLcCb->qci].mask == TRUE)
      {
           sduLen[ulLcCb->qci] = sdu->len;
           totalBytesRcvd += sdu->len;
           qciVal[ulLcCb->qci] = ulLcCb->qci;
      }
#endif
      node = node->next;
   } /* end of while for SubHeaders */
#ifdef LTE_L2_MEAS
   for(idx2 = 0; idx2 < RGU_MAX_LC; idx2++)
   {
      if((cellCb->qciArray[qciVal[idx2]].mask == TRUE) &&
          totalBytesRcvd > 0)
      {
         cellCb->qciArray[qciVal[idx2]].prbCount += 
         ((numPrb * sduLen[idx2]) / totalBytesRcvd);
      }
      
      /* RRM_RBC_X */
      if(totalBytesRcvd > 0 && qciVal[idx2] > 0)
      {
         RG_UPD_GBR_PRB(cellCb, qciVal[idx2], ((numPrb * sduLen[idx2])/totalBytesRcvd));
      }
      /* RRM_RBC_Y */
   }
#endif
/*Added for explicit release - start*/
#ifdef LTEMAC_SPS

   if(isSpsRnti && dDatInd && dDatInd->numLch)
   {
      if(ueCb->ul.spsDatRcvd != 0)
      {
         ueCb->ul.explRelCntr = 0;
         ueCb->ul.spsDatRcvd = 0;
      }
      else
      {
         ueCb->ul.explRelCntr++;
         if (ueCb->ul.explRelCntr == ueCb->ul.explRelCnt)
         {
            ueCb->ul.explRelCntr = 0;
            /* Indicate scheduler for explicit release */
            cmMemset((U8*)&schPst1, (U8)0, sizeof(Pst));
            rgGetPstToInst(&schPst1, cellCb->schInstMap.schInst);
            relInfo.cellSapId = cellCb->schInstMap.cellSapId;
            relInfo.cRnti = ueCb->ueId;
            relInfo.isExplRel = TRUE;
            /* Release indicator is called now through the matrix in the function below */
            RgMacSchSpsRel( &schPst1, &relInfo );
            ueCb->ul.implRelCntr = 0;
         }
      }
   } 
   else
   {
      /* SPS_FIX */
      if(ueCb->ul.spsDatRcvd != 0)
      {
         //ueCb->ul.implRelCntr = 0;
         ueCb->ul.explRelCntr = 0;
         ueCb->ul.spsDatRcvd = 0;
      }
   }
#endif
   /*Added for explicit release - end */

   if((dDatInd) && (dDatInd->numLch))
   {
#ifdef LTE_L2_MEAS
      rgTomUtlPrepareL2MUlThrpInfo(cellCb, ueCb,dDatInd);

      RG_CALC_TTI_CNT(cellCb, dDatInd->ttiCnt); 
#endif
      if ((ret = rgUIMSndDedDatInd(dDatInd)) != ROK)
      {
         RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
		"Failed to send datIndication to RGU CRNTI:%d",ueCb->ueId);
         RETVALUE(ret);
      }
   }
#ifndef SS_RBUF
   else if((dDatInd) && (0 == dDatInd->numLch))
   {
      /* Free the memory allocated for dDatInd if we 
       * have no valid LCH PDU to send to RLC.*/
      rgFreeSharableSBuf((Data **)&dDatInd,sizeof(RgRguDedDatInd)); 
   }
#endif
   RETVALUE(ROK);
} /* end of */ 

/** @brief This function frees up the RgMacPdu structure that has been
 * populated by demux.
 *
 * @details
 *
 *     Function: rgTOMUtlInsSchInfo
 *       - Function frees up the RgMacPdu structure, in case of error it shall
 *       free up the buffer's present in the different sdu.
 *
 *         Processing steps:
 * @param  [in] RgMacPdu   *pdu
 * @param  [in] Bool       *error
 * @return 
 */
#ifdef LTEMAC_SPS
#ifdef ANSI
PRIVATE S16 rgTOMUtlInsSchInfo
(
RgMacPdu *pdu,
RgInfSfDatInd *sfInfo,
RgInfCeInfo *ceInfo,
CmLteRnti   rnti,
Bool        spsToBeActvtd,
U16         sduSize,
U32         *lcgBytes
)
#else
PRIVATE S16 rgTOMUtlInsSchInfo (pdu, sfInfo, ceInfo, rnti,spsToBeActvtd,sduSize, lcgBytes)
RgMacPdu *pdu;
RgInfSfDatInd *sfInfo;
RgInfCeInfo *ceInfo;
CmLteRnti   rnti;
Bool        spsToBeActvtd;
U16         sduSize;
U32         *lcgBytes;
#endif

#else
#ifdef ANSI
PRIVATE S16 rgTOMUtlInsSchInfo
(
RgMacPdu *pdu,
RgInfSfDatInd *sfInfo,
RgInfCeInfo *ceInfo,
CmLteRnti   rnti,
U32         *lcgBytes
)
#else
PRIVATE S16 rgTOMUtlInsSchInfo (pdu, sfInfo, ceInfo, rnti, lcgBytes)
RgMacPdu *pdu;
RgInfSfDatInd *sfInfo;
RgInfCeInfo *ceInfo;
CmLteRnti   rnti;
U32         *lcgBytes;
#endif
#endif
{
   S16            ret;
   RgInfUeDatInd *ueInfo;
   U32           lcgId = 0;
   U32           idx = 0;

   TRC2(rgTOMUtlInsSchInfo);

   RG_TOM_INF_ALLOC(sfInfo, sizeof(RgInfUeDatInd), ueInfo, ret);

   if(ROK != ret)
   {
      RETVALUE(RFAILED);
   }

   ueInfo->rnti = rnti; 
      
#ifdef LTEMAC_SPS
   if(spsToBeActvtd)
   {
      if ((ceInfo->bitMask & RG_SHORT_BSR_CE_PRSNT) ||
          (ceInfo->bitMask & RG_LONG_BSR_CE_PRSNT))
      {
         ceInfo->bitMask |= RG_UL_SPS_ACT_PRSNT;
         ceInfo->spsSduSize = sduSize;
      }
   }
#endif
      
   ueInfo->ceInfo = *ceInfo;
   ueInfo->ueLstEnt.node = (PTR)ueInfo;
   for (lcgId = 1, idx = 0; lcgId < RGINF_MAX_LCG_PER_UE; lcgId++)
   {
      if (lcgBytes[lcgId] != 0)
      {
         /* Only GBR bytes */
         ueInfo->lcgInfo[idx].lcgId     = lcgId;
         ueInfo->lcgInfo[idx++].bytesRcvd = lcgBytes[lcgId];
         lcgBytes[lcgId] = 0;
      }
   }
   cmLListAdd2Tail(&sfInfo->ueLst, &ueInfo->ueLstEnt);
   RETVALUE(ROK);
} /* end of rgTOMUtlInsSchInfo */

/**
 * @brief Handler for processing data indication recieved from PHY for UEs.
 *
 * @details
 *
 *     Function: rgTOMDatInd
 *
 *     Handler for processing data indication recieved from PHY for UEs.
 *
 *     Invoked by: RgLiTfuDatInd of LIM 
 *
 *     Processing Steps: 
 *     For each DataInfo recieved
 *      - Validate the information received and retrieve cellCb 
 *        Validate cellId, rnti 
 *      - Call De-Mux module to decode the data rgDUXDemuxData
 *      - If  received a CRNTI control element 
 *          - Check if a CCCH SDU is present, if it is return failure
 *          - Check for the existence of UE, if its isnt present return failure.
 *          - Delegate the remaining processing to rgTOMUtlProcMsg3 which
 *          primarily informs the scheduler about the data received and
 *          generates Data indications towards the higher layer.
 *      - If only CCCH SDU is present
 *        - Invoke rgTOMUtlProcMsg3 for further processing.
 *      - If its a non-Msg3 PDU i.e. received outside of a RA procedure
 *        - Retrieve the UeCB 
 *        - Validate that the received PDU contains only configured Logical
 *        Channels.
 *        - Invoke rgTOMUtlProcDatPdu for further processing. It informs the
 *        scheduler with the information of the received Data and generates
 *        DatIndications towards the higher layers. 
 *           
 *  @param[in]  TfuDatIndInfo *datInd
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgTOMDatInd
(
 TfuDatIndInfo *datInd
 )
#else
PUBLIC S16 rgTOMDatInd(datInd)
 TfuDatIndInfo *datInd;
#endif
{
   S16               ret = ROK;
   RgErrInfo         err;
   RgUeCb            *ueCb;
   RgUeCb            *prevUeCb = NULLP;
   RgCellCb          *cellCb;
   RgMacPdu          *pdu;
   RgInfSfDatInd     *sfInfo;
   RgInfCeInfo       ceInfo; 
   Pst               schPst;
   CmLList           *node;
   TfuDatInfo        *datInfo;
   RgLowSapCb        *tfuSap;
   U8                subframe;
#ifdef LTEMAC_SPS
   Bool              isSpsRnti=FALSE;
   Pst               schPst1;  
   RgInfSpsRelInfo   relInfo;
   Bool              spsToBeActvtd = FALSE;
   U16               sduSize = 0;
#endif
   U32               lcgBytes[RGINF_MAX_LCG_PER_UE];


   TRC2(rgTOMDatInd);

   cmMemset((U8 *)&lcgBytes, 0, sizeof(lcgBytes));

   tfuSap = &(rgCb.tfuSap);
   ueCb = NULLP;
   cellCb = rgDBMGetCellCb (datInd->cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,datInd->cellId,"Unable to get the cellCb for cell");
      RETVALUE(RFAILED);
   }
   /* Avoiding memset as all the fields are getting initialized further */

   if (rgTOMInfAllocPduEvnt (&sfInfo) != ROK)
   {
      err.errType = RGERR_TOM_DATIND;
      RLOG_ARG0(L_ERROR,DBG_CELLID,datInd->cellId,"Unable to Allocate PDU for DUX cell");
      node =  datInd->datIndLst.first;
      RETVALUE(RFAILED);
   }
   cmLListInit(&sfInfo->ueLst);
   sfInfo->cellId = datInd->cellId;
   sfInfo->timingInfo = datInd->timingInfo;
   subframe = datInd->timingInfo.subframe;

   node =  datInd->datIndLst.first;
   for (;node; node=node->next)
   {
      datInfo = (TfuDatInfo*)node->node;
      {
         //extern U32 ulrate_tfu;
         MsgLen len;
         SFndLenMsg(datInfo->mBuf, &len);
         rgUlrate_tfu += len;
      }
      /* We shall call De-Mux to process the received buffer. We shall try and find
       * out the RaCb based on the following - 
       * 1. If the incoming PDU contained a CCCH SDU i.e. this is message 3.
       * 2. If the incoming PDU contained a CRNTI control element, i.e. we should
       * have a ueCb also for this 
       */
      /* Lets allocate the event that needs to be passed to DUX */
      if (rgTOMUtlAllocPduEvnt (&pdu) != ROK)
      {
         err.errType = RGERR_TOM_DATIND;
         RLOG_ARG0(L_ERROR,DBG_CELLID,datInd->cellId,"Unable to Allocate PDU for DUX cell");
         rgTOMInfFreePduEvnt (sfInfo);
         RETVALUE(RFAILED);
      }

      if ((ret = rgDUXDemuxData (pdu, &ceInfo, 
                           &datInfo->mBuf, &err)) != ROK)
      {
         /* Fix: sriky memory corruption precautions */
         rgTOMUtlFreePduEvnt (pdu, TRUE);
         err.errType = RGERR_TOM_DATIND;
         RLOG_ARG0(L_ERROR,DBG_CELLID,datInd->cellId,"DUX processing failed");
         tfuSap->sapSts.numPduDrop++;
         continue; 
      }
      /* It could be that a non-msg3 pdu contains a CRNTI Control element. We
       * should check for CRNTI CE and if it exists the UECb must exist, also an
       * if the CRNTI in the CE and the one with which the message came in are
       * different we shall look for an raCb as well. 
       */

      if (ceInfo.bitMask & RG_CCCH_SDU_PRSNT)
      {
        ret = rgTOMProcCCCHSduInDatInd(pdu, prevUeCb, \
                cellCb, datInfo, &ceInfo, subframe);
        if (ret == RFAILED)
        {
            rgTOMUtlFreePduEvnt (pdu, TRUE);
            err.errType = RGERR_TOM_DATIND;
            tfuSap->sapSts.numPduDrop++;
            continue; 
        }
      } /* end of Msg3 processing */

      else if (ceInfo.bitMask & RG_CRNTI_CE_PRSNT)
      {
        ret = rgTOMProcCrntiCEInDatInd(pdu, prevUeCb, \
                cellCb, datInfo, &ceInfo, subframe);
        if (ret == RFAILED)
        {
            rgTOMUtlFreePduEvnt (pdu, TRUE);
            err.errType = RGERR_TOM_DATIND;
            tfuSap->sapSts.numPduDrop++;
            continue; 
        }

      } /* end of CRNTI based message */
      else
      {
         ueCb = rgDBMGetUeCb (cellCb, datInfo->rnti);
         if (ueCb == NULLP)
         {
#ifdef LTEMAC_SPS
            /* Try getting the UE using SPS-RNTI. */
            ueCb = rgDBMGetSpsUeCb (cellCb, datInfo->rnti);
            if (ueCb != NULLP)
            {
               isSpsRnti = TRUE;
               /* Increment implrelCntr for an empty transmission */
               if (pdu->sduLst.count == 0)
               {
                  ueCb->ul.implRelCntr++;
                  if (ueCb->ul.implRelCntr == ueCb->ul.implRelCnt)
                  {
                     /* Indicate scheduler for implicit release */
                     cmMemset((U8*)&schPst1, (U8)0, sizeof(Pst));
                     rgGetPstToInst(&schPst1, cellCb->schInstMap.schInst);

                     ueCb->ul.implRelCntr = 0;
                     ueCb->ul.explRelCntr = 0;
                     relInfo.cellSapId = cellCb->schInstMap.cellSapId;
                     relInfo.cRnti = ueCb->ueId;
                     relInfo.isExplRel = FALSE;
                     RgMacSchSpsRel(&schPst1, &relInfo);
                  }
               }
               else
               {
                  /* Reset the implrelCntr */
                  ueCb->ul.implRelCntr = 0;
               }
            }
            else
#endif 
            {
               /* Perform failure if ueCb is still NULLP */
               rgTOMUtlFreePduEvnt (pdu, TRUE);
               err.errType = RGERR_TOM_DATIND;
               RLOG_ARG1(L_ERROR,DBG_CELLID,datInd->cellId,"RNTI:%d Unable to get the UE CB", 
                  datInfo->rnti);
               tfuSap->sapSts.numPduDrop++;
               continue;
            }
         }
#ifdef LTE_L2_MEAS         
     rgTOMUtlL2MStoreBufSz(ueCb, &ceInfo);
     rgTOML2MCompileActiveLCs (cellCb, ueCb, pdu, &ceInfo);
#endif
#ifdef LTEMAC_SPS
         if ((ret = rgTOMUtlProcMsg(cellCb, ueCb, pdu, isSpsRnti,&spsToBeActvtd,&sduSize, subframe, (U32 *)&lcgBytes)) != ROK)
#else
         if ((ret = rgTOMUtlProcMsg (cellCb, ueCb, pdu, subframe, (U32 *)&lcgBytes)) != ROK)
#endif /* LTEMAC_SPS */
         {
            rgTOMUtlFreePduEvnt (pdu, TRUE);
            err.errType = RGERR_TOM_DATIND;
            RLOG_ARG1(L_ERROR,DBG_CELLID,datInd->cellId,
			    "Unable to handle Data Indication CRNTI:%d",ueCb->ueId);
            tfuSap->sapSts.numPduDrop++;
            continue;
         }
      }

      
#ifdef LTEMAC_SPS
      if(rgTOMUtlInsSchInfo(pdu, sfInfo, &ceInfo, datInfo->rnti,spsToBeActvtd,sduSize, (U32 *)&lcgBytes) != ROK)
#else
      if(rgTOMUtlInsSchInfo(pdu, sfInfo, &ceInfo, datInfo->rnti, (U32 *)&lcgBytes) != ROK)
#endif
      
      {
         rgTOMInfFreePduEvnt (sfInfo);
         rgTOMUtlFreePduEvnt (pdu, FALSE);
         RETVALUE(RFAILED);
      }
      /* free up the PDU memory */
      rgTOMUtlFreePduEvnt (pdu, FALSE);
   }
   /* Free the allocated memory for ueUlAllocInfo here */
#ifdef LTE_L2_MEAS
   if(cellCb->ulSf[subframe].ueUlAllocInfo != NULLP)
   {
      /*ccpu00117052 - MOD - Passing double for proper NULLP
                             assignment */
      rgFreeSBuf((Data **)&(cellCb->ulSf[subframe].ueUlAllocInfo), 
      ((cellCb->ulSf[subframe].numUe) * sizeof(RgUeUlAlloc)));
   }
#endif
   /* RRM_RBC_X */
   /* Update PRB used for all GBR QCIs to scheduler */
    cmMemcpy((U8*) &sfInfo->qcisUlPrbCnt[0],
             (U8*) &cellCb->qcisUlPrbCnt[0],
             (RGM_MAX_QCI_REPORTS * sizeof(U32)));
    /* clear the cellCb ul prb value */
    cmMemset((U8*)&cellCb->qcisUlPrbCnt[0], 0, 
             (RGM_MAX_QCI_REPORTS * sizeof(U32)));

   /* RRM_RBC_Y */

   rgGetPstToInst(&schPst, cellCb->schInstMap.schInst);
   sfInfo->cellSapId = cellCb->schInstMap.cellSapId;
   RgMacSchSfRecp(&schPst, sfInfo);
   RETVALUE(ROK);
}  /* rgTOMDatInd */

/**
 * @brief Function handles allocation for common channels i.e. BCCH-BCH,
 * BCCH-DLSCH, PCCH-DLSCH.
 *
 * @details
 *
 *     Function : rgHndlCmnChnl
 *     
 *     This function is invoked from RgSchMacSfAllocReq. This function handles
 *     allocations made for common channels like BCCH and PCCH. 
 *
 *     Processing steps:
 *     1. If BCCH on BCH has been scheduled, send StatusIndication on RGU.
 *     2. If PCCH is scheduled, send StatusIndication on RGU.
 *     3. If BCCH on DLSCH has been scheduled and sndStatInd is TRUE, send
 *     StatusIndication on RGU, else copy the bcch buffer onto the downlink
 *     subframe. 
 *     
 *           
 *  @param[in] RgCellCb          *cell,
 *  @param[in] CmLteTimingInfo   timingInfo,
 *  @param[in] RgInfCmnLcInfo    *cmnLcInfo,
 *  @param[in/out] RgErrInfo     *err,
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgHndlCmnChnl
(
RgCellCb            *cell,
CmLteTimingInfo     timingInfo,
RgInfCmnLcInfo      *cmnLcInfo,
RgErrInfo           *err
)
#else
PRIVATE S16 rgHndlCmnChnl(cell, timingInfo, cmnLcInfo, err)
RgCellCb            *cell;
CmLteTimingInfo     timingInfo;
RgInfCmnLcInfo      *cmnLcInfo;
RgErrInfo           *err;
#endif
{
   #if (ERRCLASS & ERRCLS_DEBUG)
   RgPcchLcCb           *pcch;
   #endif
#ifndef RGR_SI_SCH
   RgBcchDlschLcCb      *bcch;
#if (ERRCLASS & ERRCLS_DEBUG)
   RgBcchBchLcCb        *bch;
#endif
#endif/*RGR_SI_SCH*/
   RguCStaIndInfo       *staInd;
   RgDlSf               *dlSf;

   TRC2(rgHndlCmnChnl)

   dlSf = &cell->subFrms[timingInfo.subframe];

   if(cmnLcInfo->bitMask & RGINF_BCH_INFO)
   {
#ifndef RGR_SI_SCH
      #if (ERRCLASS & ERRCLS_DEBUG) 
      if(NULLP == (bch = rgDBMGetBcchOnBch(cell)))
      {
         RETVALUE(RFAILED);
      }
      if(cmnLcInfo->bchInfo.lcId != bch->lcId)
      {
         RETVALUE(RFAILED);
      }
      #endif
      if (rgAllocShrablSBuf ((Data**)&staInd, sizeof(RguCStaIndInfo)) != ROK)
      {
         err->errCause = RGERR_TOM_MEM_EXHAUST;
         RETVALUE(RFAILED);
      }
      staInd->cellId = cell->cellId;
      staInd->rnti   = RG_INVALID_RNTI;
      staInd->lcId   = cmnLcInfo->bchInfo.lcId;
      staInd->transId = (timingInfo.sfn << 8) | (timingInfo.subframe);
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
      dlSf->remDatReqCnt++;
#endif
      if (rgUIMSndCmnStaInd (staInd) != ROK)
      {
         RETVALUE(RFAILED);
      }
#else
      /*Store the received BCH Data in the scheduled subframe*/
      dlSf->bch.tb = cmnLcInfo->bchInfo.pdu;
#endif/*RGR_SI_SCH*/
   }

   if(cmnLcInfo->bitMask & RGINF_PCCH_INFO)
   {
      #if (ERRCLASS & ERRCLS_DEBUG) 
      if(NULLP == (pcch = rgDBMGetPcch(cell)))
      {
         RETVALUE(RFAILED);
      }
      if(cmnLcInfo->pcchInfo.lcId != pcch->lcId)
      {
         RETVALUE(RFAILED);
      }
      #endif

      dlSf->pcch.pdcch.rnti = 
               cmnLcInfo->pcchInfo.rnti;
      dlSf->pcch.pdcch.dci = 
               cmnLcInfo->pcchInfo.dciInfo;
#ifdef TFU_UPGRADE               
      /* ccpu00132314-ADD-Fill the tx Pwr offset from scheduler */         
      dlSf->pcch.txPwrOffset = cmnLcInfo->pcchInfo.txPwrOffset;         
#endif
      if (rgAllocShrablSBuf ((Data**)&staInd, sizeof(RguCStaIndInfo)) != ROK)
      {
         err->errCause = RGERR_TOM_MEM_EXHAUST;
         RETVALUE(RFAILED);
      }
      staInd->cellId = cell->cellId;
      staInd->rnti   = RG_INVALID_RNTI;
      staInd->lcId   = cmnLcInfo->pcchInfo.lcId;
      staInd->transId = (timingInfo.sfn << 8) | (timingInfo.subframe);
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
      dlSf->remDatReqCnt++;
#endif
      /* for consolidated CmnStaInd calling below function from function 
       * rgHndlSchedUe once CmnStaInd prepared for all UEs
       */
      if (rgUIMSndCmnStaInd (staInd) != ROK)
      {
         RETVALUE(RFAILED);
      }
   }

   if(cmnLcInfo->bitMask & RGINF_BCCH_INFO)
   {
      dlSf->bcch.pdcch.rnti = 
               cmnLcInfo->bcchInfo.rnti;
      dlSf->bcch.pdcch.dci = 
               cmnLcInfo->bcchInfo.dciInfo;
#ifdef TFU_UPGRADE               
      /* ccpu00132314-ADD-Fill the tx Pwr offset from scheduler */         
      dlSf->bcch.txPwrOffset = cmnLcInfo->bcchInfo.txPwrOffset;         
#endif      
#ifndef RGR_SI_SCH
      if(NULLP == 
         (bcch=rgDBMGetBcchOnDlsch(cell,cmnLcInfo->bcchInfo.lcId)))
      {
         RETVALUE(RFAILED);
      }
      if(TRUE == cmnLcInfo->bcchInfo.sndStatInd)
      {
         RG_FREE_MSG(bcch->tb);
         if (rgAllocShrablSBuf ((Data**)&staInd, 
                  sizeof(RguCStaIndInfo)) != ROK)
         {
            err->errCause = RGERR_TOM_MEM_EXHAUST;
            RETVALUE(RFAILED);
         }
         staInd->cellId = cell->cellId;
         staInd->rnti   = RG_INVALID_RNTI;
         staInd->lcId   = cmnLcInfo->bcchInfo.lcId;
         staInd->transId = (timingInfo.sfn << 8) | (timingInfo.subframe);
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
         dlSf->remDatReqCnt++;
#endif
         if (rgUIMSndCmnStaInd (staInd) != ROK)
         {
            RETVALUE(RFAILED);
         }
      }
      else
      {
         SCpyMsgMsg(bcch->tb, RG_GET_MEM_REGION(rgCb),
                  RG_GET_MEM_POOL(rgCb), &dlSf->bcch.tb);
      }
#else
      /*Store the received BCCH Data in the scheduled subframe*/
      dlSf->bcch.tb = cmnLcInfo->bcchInfo.pdu;
#endif/*RGR_SI_SCH*/
   }

   RETVALUE(ROK);
} /* end of rgHndlCmnChnl */

/**
 * @brief Function for handling allocations for dedicated channels for a
 * subframe.
 *
 * @details
 *
 *     Function : rgHndlSchdUe
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     allocations of dedicated channels for a subframe. Invoked by the function
 *     RgSchMacSfAllocReq.
 *
 *     Processing steps :
 *     1. Loops through the list of UE's scheduled looking for the corresponding
 *     ueCb/raCb. 
 *     2. Finds the corresponding HARQ process.
 *     3. Invokes the DHM function to issue StatusIndications on RGU.
 *
 *           
 *  @param[in] RgCellCb            *cell,
 *  @param[in] CmLteTimingInfo     timingInfo,
 *  @param[in] RgInfUeInfo         *ueInfo
 *  @param[in/out] RgErrInfo       *err
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgHndlSchdUe
(
RgCellCb            *cell,
CmLteTimingInfo     timingInfo,
RgInfUeInfo         *ueInfo,
RgErrInfo           *err
)
#else
PRIVATE S16 rgHndlSchdUe(cell, timingInfo, ueInfo, err)
RgCellCb            *cell;
CmLteTimingInfo     timingInfo;
RgInfUeInfo         *ueInfo;
RgErrInfo           *err;
#endif
{

   TRC2(rgHndlSchdUe);

   if(NULLP == ueInfo->allocInfo)
   {
      RETVALUE(RFAILED);
   }

   rgDHMSndConsolidatedStaInd(cell, ueInfo, timingInfo, err);
   RETVALUE(ROK);
} /* end of rgHndlSchdUe */

#ifdef LTE_L2_MEAS
/**
 * @brief Function for handling Uplink allocations for Ue for a
 * subframe.
 *
 * @details
 *
 *     Function : rgHndlUlUeInfo
 *     
 *  @param[in] RgCellCb            *cell,
 *  @param[in] CmLteTimingInfo     timingInfo,
 *  @param[in] RgInfUlUeInfo       *ueInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgHndlUlUeInfo
(
RgCellCb            *cell,
CmLteTimingInfo     timingInfo,
RgInfUlUeInfo       *ueInfo
)
#else
PRIVATE S16 rgHndlUlUeInfo(cell, timingInfo, ueInfo)
RgCellCb            *cell;
CmLteTimingInfo     timingInfo;
RgInfUlUeInfo       *ueInfo;
#endif
{
   U8             idx;
   RgUlSf         *ulSf;
   S16            ret;

   TRC2(rgHndlUlUeInfo)
   
   ulSf = &cell->ulSf[timingInfo.subframe];

   /* rg003.301-MOD- Corrected the purifier memory leak */
   if (ulSf->numUe != ueInfo->numUes)
   {
      if (ulSf->ueUlAllocInfo)
      {
         rgFreeSBuf((Data **)&(ulSf->ueUlAllocInfo),
               ulSf->numUe * sizeof(RgUeUlAlloc));
      }
   }
   ulSf->numUe         = ueInfo->numUes;
   if((ulSf->ueUlAllocInfo == NULLP) && (ueInfo->numUes > 0))
   {
       /* Allocate memory for ulAllocInfo */
       if((ret = rgAllocSBuf((Data**)&(cell->ulSf[timingInfo.subframe].
                 ueUlAllocInfo), ueInfo->numUes *  sizeof(RgUeUlAlloc))) != ROK)
       {
          RETVALUE(ret);
       }
   }
   if (ulSf->ueUlAllocInfo != NULLP)
   {
      for(idx = 0; idx < ueInfo->numUes; idx++)
      {
         ulSf->ueUlAllocInfo[idx].rnti   = ueInfo->ulAllocInfo[idx].rnti;
         ulSf->ueUlAllocInfo[idx].numPrb = ueInfo->ulAllocInfo[idx].numPrb;
      }
   }
   RGCPYTIMEINFO(timingInfo, ulSf->schdTime);
   RETVALUE(ROK);
} /* end of rgHndlUlUeInfo */
#endif
/**
 * @brief Function for handling RaResp request received from scheduler to MAC
 *
 * @details
 *
 *     Function : rgTOMRlsSf
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     allocations of random access responses for a subframe.
 *     This shall invoke RAM to create ueCbs for all the rapIds allocated and 
 *     shall invoke MUX to create RAR PDUs for raRntis allocated.
 *     
 *           
 *  @param[in] CmLteCellId         cellId,
 *  @param[in] CmLteTimingInfo     timingInfo,
 *  @param[in] RaRespInfo          *rarInfo
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC Void rgTOMRlsSf
(
RgDlSf              *dlSf
)
#else
PUBLIC Void rgTOMRlsSf(dlSf)
RgDlSf              *dlSf;
#endif
{
   U8               idx;

   TRC2(rgTOMRlsSf)

   if(dlSf->txDone == FALSE)
   {
      RLOG0(L_ERROR, "SUBFRAME Not pushed to the PHY");

      if (dlSf->bch.tb != NULLP)
      {
         RG_FREE_MSG(dlSf->bch.tb);
      }
      if (dlSf->bcch.tb != NULLP)
      {
         RG_FREE_MSG(dlSf->bcch.tb);
      }
      if (dlSf->pcch.tb != NULLP)
      {
         RG_FREE_MSG(dlSf->pcch.tb);
      }

      for(idx=0; idx < dlSf->numRaRsp; idx++)
      {
         RG_FREE_MSG(dlSf->raRsp[idx].rar);
      }
   }
/* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
   dlSf->remDatReqCnt = 0;
   /* Fix[ccpu00126310]: Tracks Data Requests from RLC for both loosely and tight coupled 
      RLC-MAC */
   dlSf->statIndDone = FALSE;
#endif
	 if (dlSf->tbs.count)
	 {
      U8           i;
      CmLList      *node;
      RgDlHqProcCb *hqP;
      node = dlSf->tbs.first;
		  while(node)
		  {
         hqP = (RgDlHqProcCb*)node->node;
         node = node->next;
		     for(i=0;i< RG_MAX_TB_PER_UE;i++)
		     {
				    if (hqP->tbInfo[i].sf == dlSf)
				    {
					     cmLListDelFrm(&dlSf->tbs, &hqP->tbInfo[i].sfLnk);
					     hqP->tbInfo[i].sfLnk.node = NULLP;
				    }
				    hqP->tbInfo[i].sf = NULLP;
			   }
			}
	 }
   cmLListInit(&dlSf->tbs);
   dlSf->txDone = FALSE;
   RETVOID;
}

/**
 * @brief Function is called by the scheduler once it has completed the
 * allocation for the subframe. 
 *
 * @details
 *
 *     Function : rgHndlFlowCntrl
 *     This function should fill and send Flow control 
 *     indication to RLC
 *
 *           
 *  @param[in] Pst                 *cell       
 *  @param[in] RgInfSfAlloc        *sfInfo   Carries all the allocation
 *  information.
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 rgHndlFlowCntrl
(
RgCellCb       *cell,
RgInfSfAlloc        *sfInfo
)
#else
PUBLIC S16 rgHndlFlowCntrl(cell, sfInfo)
RgCellCb            *cell;
RgInfSfAlloc        *sfInfo;
#endif
{
   RguFlowCntrlInd  *flowCntrlInd;
   Pst              *pst;
   U32              ueIdx;
   U32              lcIdx;
   TRC3(rgHndlFlowCntrl);

   pst = &rgCb.rguSap[rgRguDlSap].sapCfg.sapPst;
   /* flowCntrlInd is alloced in cell init time and will be re-used throughout */
   flowCntrlInd = cell->flowCntrlInd;
   flowCntrlInd->cellId = sfInfo->cellId;
   flowCntrlInd->numUes = sfInfo->flowCntrlInfo.numUes; 
  
   for (ueIdx = 0; ueIdx < sfInfo->flowCntrlInfo.numUes; ueIdx++)
   {
      flowCntrlInd->ueFlowCntrlInfo[ueIdx].ueId = sfInfo->flowCntrlInfo.ueFlowCntrlInfo[ueIdx].ueId;
      flowCntrlInd->ueFlowCntrlInfo[ueIdx].numLcs = sfInfo->flowCntrlInfo.ueFlowCntrlInfo[ueIdx].numLcs;
      
      for (lcIdx = 0; lcIdx < RGINF_MAX_NUM_DED_LC; lcIdx++)
      {
         flowCntrlInd->ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].lcId =
         sfInfo->flowCntrlInfo.ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].lcId;
         flowCntrlInd->ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].pktAdmitCnt = 
         sfInfo->flowCntrlInfo.ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].pktAdmitCnt;
        
         flowCntrlInd->ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].maxBo4FlowCtrl = 
         sfInfo->flowCntrlInfo.ueFlowCntrlInfo[ueIdx].lcInfo[lcIdx].maxBo4FlowCtrl;
      }
   }
   RgUiRguFlowCntrlInd(pst,rgRguDlSap,flowCntrlInd); 
   RETVALUE(ROK);
}
/**
 * @brief Function is called by the scheduler once it has completed the
 * allocation for the subframe. 
 *
 * @details
 *
 *     Function : RgSchMacSfAllocReq
 *     
 *     This function shall be invoked whenever scheduler is done with the
 *     allocations of for a subframe. The sfInfo carries all the allocation
 *     details for the common channels, RA responses and dedicated channel
 *     allocations. 
 *
 *     Processing steps :
 *     1. Reset the information present in the downlink subframe that is
 *     scheduled.
 *     2. Handle common channel allocations
 *     3. Handle RA Response allocations
 *     4. Handle dedicated channel allocations.
 *           
 *  @param[in] Pst                 *pst       
 *  @param[in] RgInfSfAlloc        *sfInfo   Carries all the allocation
 *  information.
 *  @return  S16
 *      -# ROK 
 **/
#ifdef ANSI
PUBLIC S16 RgSchMacSfAllocReq
(
Pst                 *pst,
RgInfSfAlloc        *sfInfo
)
#else
PUBLIC S16 RgSchMacSfAllocReq(pst, sfInfo)
Pst                 *pst;
RgInfSfAlloc        *sfInfo;
#endif
{
   RgCellCb       *cell;
   RgDlSf         *dlSf;
   RgErrInfo      err;
   VOLATILE U32   startTime=0;

   TRC2(RgSchMacSfAllocReq)

   /*starting Task*/
   SStartTask(&startTime, PID_MAC_SF_ALLOC_REQ);

   if(NULLP == sfInfo)
   {
      RETVALUE(RFAILED);
   }

   if((cell = rgDBMGetCellCb(sfInfo->cellId)) == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,sfInfo->cellId, "No cellCb found with cell");
      RETVALUE(RFAILED);
   }

   dlSf = &cell->subFrms[sfInfo->timingInfo.subframe];

   rgTOMRlsSf(dlSf);
   dlSf->schdTime = sfInfo->timingInfo;

   /* Fix : syed Ignore Failure Returns and continue processing.
    * Incomplete processing results in state sync loss between MAC-SCH. */
   rgHndlCmnChnl(cell, sfInfo->timingInfo, &sfInfo->cmnLcInfo, &err);

   rgHndlRaResp(cell, sfInfo->timingInfo, &sfInfo->rarInfo, &err);

   rgHndlSchdUe(cell, sfInfo->timingInfo, &sfInfo->ueInfo, &err);

#ifdef LTE_L2_MEAS
   if(rgHndlUlUeInfo(cell, sfInfo->ulUeInfo.timingInfo, 
                     &sfInfo->ulUeInfo) != ROK)
   {
      RETVALUE(RFAILED);
   }
#endif
   /* Fix[ccpu00126310]: Tracks Data Requests from RLC for both loosely and tight coupled 
      RLC-MAC */


   /* Added the handling for pushing down
    * TFU Data request in the retransmission only scenario.*/ 
#ifdef LTEMAC_DLUE_TMGOPTMZ
   dlSf->statIndDone = TRUE;
   /* Fix [ccpu00126310]: Tracks Data Requests from RLC for both loosely and tight coupled 
      RLC-MAC */
   if(!(dlSf->txDone) && (0 == dlSf->remDatReqCnt) && (dlSf->statIndDone) && 
        (RG_TIMEINFO_SAME(cell->crntTime, dlSf->schdTime)))
   {
      /*This is the case of rettransmission, so no need
       * to wait for TTI Ind to push TFU Data Request. Send
       * it right away.*/
      if (ROK != rgTOMUtlProcDlSf (dlSf, cell, &err))
      {
         RLOG_ARG0(L_ERROR,DBG_CELLID,cell->cellId,"Unable to process downlink subframe for cell");
         err.errType = RGERR_ROM_DEDDATREQ;
      }
      /* Mark this frame as sent */
      dlSf->txDone = TRUE;
   }
#endif
   if (sfInfo->flowCntrlInfo.numUes > 0)
   {
     rgHndlFlowCntrl(cell,sfInfo);
   }
   /*stoping Task*/
   SStopTask(startTime, PID_MAC_SF_ALLOC_REQ);
   RETVALUE(ROK);
} /* end of RgSchMacSfAllocReq */
/**
 * @brief Handler for processing data indication recieved from PHY for UEs.
 *
 * @details
 *
 *     Function: rgTOMProcCrntiCEInDatInd
 *
 *     Handler for processing data indication recieved from PHY for UEs.
 *
 *     Invoked by: RgLiTfuDatInd of LIM 
 *
 *     Processing Steps: 
 *     For each DataInfo recieved
 *      - If  received a CRNTI control element 
 *          - Check if a CCCH SDU is present, if it is return failure
 *          - Check for the existence of UE, if its isnt present return failure.
 *          - Delegate the remaining processing to rgTOMUtlProcMsg3 which
 *          primarily informs the scheduler about the data received and
 *          generates Data indications towards the higher layer.
 *           
 *  @param      RgMacPdu          *pdu,
 *  @param      RgUeCb            *prevUeCb,
 *  @param      RgCellCb          *cellCb,
 *  @param      TfuDatInfo        *datInfo,
 *  @param      RgInfCeInfo       *ceInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgTOMProcCrntiCEInDatInd
(
RgMacPdu          *pdu,
RgUeCb            *prevUeCb,
RgCellCb          *cellCb,
TfuDatInfo        *datInfo,
RgInfCeInfo       *ceInfo,
U8                subframe
)
#else
PRIVATE S16 rgTOMProcCrntiCEInDatInd( pdu, prevUeCb, cellCb, datInfo, ceInfo, subframe)
RgMacPdu          *pdu;
RgUeCb            *prevUeCb;
RgCellCb          *cellCb;
TfuDatInfo        *datInfo;
RgInfCeInfo       *ceInfo;
U8                subframe;
#endif
{
   RgUeCb *ueCb = NULLP;


#ifdef LTEMAC_SPS
   Bool spsToBeActvtd;
   U16  sduSize;
#endif

   TRC2(rgTOMProcCrntiCEInDatInd)

#ifndef LTE_L2_MEAS      
      UNUSED(subframe);
#endif

   ueCb = rgDBMGetUeCbFromRachLst (cellCb, datInfo->rnti);

   if (ueCb == NULLP)
   {
       RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
		"RNTI:%d Received MSG3 with CRNTI,unable to find ueCb", ceInfo->ces.cRnti);
       RETVALUE(RFAILED);
   }

   prevUeCb = rgDBMGetUeCb (cellCb, ceInfo->ces.cRnti);
   if (prevUeCb == NULLP)
   {
       RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
		 "RNTI:%d Received MSG3 with CRNTI,unable to find ueCb", ceInfo->ces.cRnti);
       RETVALUE(RFAILED);
   }
   RLOG_ARG2(L_DEBUG,DBG_CELLID,cellCb->cellId,
            "CRNTI CE(%d) received through tmpCrnti(%d)",
            ceInfo->ces.cRnti, datInfo->rnti);
   rgDBMDelUeCbFromRachLst(cellCb, ueCb);
   rgRAMFreeUeCb(ueCb);
   ueCb = prevUeCb;
#ifdef LTEMAC_SPS
   if ((rgTOMUtlProcMsg(cellCb, ueCb, pdu, FALSE,&spsToBeActvtd,&sduSize, subframe, NULLP)) != ROK)
#else
   if ((rgTOMUtlProcMsg (cellCb, ueCb, pdu, subframe, NULLP)) != ROK)
#endif /* LTEMAC_SPS */
   {
       RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
		       "RNTI:%d Processing for MSG3 failed",datInfo->rnti);
       RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
/**
 * @brief Handler for processing data indication recieved from PHY for UEs.
 *
 * @details
 *
 *     Function: rgTOMProcCCCHSduInDatInd
 *
 *     Handler for processing data indication recieved from PHY for UEs.
 *
 *     Invoked by: RgLiTfuDatInd of LIM 
 *
 *     Processing Steps: 
 *     For each DataInfo recieved
 *      - If only CCCH SDU is present
 *        - Invoke rgTOMUtlProcMsg3 for further processing.
 *      - If its a non-Msg3 PDU i.e. received outside of a RA procedure
 *        - Retrieve the UeCB 
 *        - Validate that the received PDU contains only configured Logical
 *        Channels.
 *        - Invoke rgTOMUtlProcDatPdu for further processing. It informs the
 *        scheduler with the information of the received Data and generates
 *        DatIndications towards the higher layers. 
 *           
 *  @param  TfuDatIndInfo *datInd
 *  @param  RgMacPdu          *pdu,
 *  @param  RgUeCb            *prevUeCb,
 *  @param  RgCellCb          *cellCb,
 *  @param  TfuDatInfo        *datInfo,
 *  @param  RgInfCeInfo       *ceInfo
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PRIVATE S16 rgTOMProcCCCHSduInDatInd
(
RgMacPdu          *pdu,
RgUeCb            *prevUeCb,
RgCellCb          *cellCb,
TfuDatInfo        *datInfo,
RgInfCeInfo       *ceInfo,
U8                subframe 
)
#else
PRIVATE S16 rgTOMProcCCCHSduInDatInd( pdu, prevUeCb, cellCb, datInfo, ceInfo, subframe)
RgMacPdu          *pdu;
RgUeCb            *prevUeCb;
RgCellCb          *cellCb;
TfuDatInfo        *datInfo;
RgInfCeInfo       *ceInfo;
U8                subframe;
#endif
{
   RgUeCb *ueCb = NULLP;

#ifdef LTEMAC_SPS
   Bool spsToBeActvtd;
   U16  sduSize;
#endif


   TRC2(rgTOMProcCCCHSduInDatInd)

#ifndef LTE_L2_MEAS      
      UNUSED(subframe);
#endif

   if (ceInfo->bitMask & RG_CRNTI_CE_PRSNT)
   {
       RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
		       "CRNTI:%d Received MSG3 with CCCH",ceInfo->ces.cRnti);
       RETVALUE(RFAILED);
   }
   
   ueCb = rgDBMGetUeCbFromRachLst (cellCb, datInfo->rnti);
   
   if (ueCb == NULLP)
   {
       RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,
		       "RNTI:%d Processing for MSG3 failed", datInfo->rnti);
       RETVALUE(RFAILED);
   }
   /* Fix: syed Drop any duplicate Msg3(CCCH Sdu) */
   if (ueCb->dl.hqEnt.numHqProcs)
   {
      /* HqE is already initialized by a previuos Msg3 */ 
       RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,"RNTI:%d Processing for MSG3 failed. Duplicate "
          "MSG3 received. Dropping", datInfo->rnti);
       RETVALUE(RFAILED);
   }
   
   if(rgDHMHqEntInit(&ueCb->dl.hqEnt,
                     cellCb->maxDlHqProcPerUe) != ROK)
   {
       RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,"RNTI:%d Harq Initialization failed ", 
          datInfo->rnti);
       RETVALUE(RFAILED);
   }
    RLOG_ARG1(L_DEBUG,DBG_CELLID,cellCb->cellId,
             "CCCH SDU received through tmpCrnti(%d)",datInfo->rnti);
#ifdef LTEMAC_SPS
   if ((rgTOMUtlProcMsg(cellCb, ueCb, pdu, FALSE,&spsToBeActvtd,&sduSize, subframe, NULLP)) != ROK)
#else
   if ((rgTOMUtlProcMsg (cellCb, ueCb, pdu, subframe, NULLP)) != ROK)
#endif /* LTEMAC_SPS */
   {
       RLOG_ARG1(L_ERROR,DBG_CELLID,cellCb->cellId,"RNTI:%d Processing for MSG3 failed", 
          datInfo->rnti);
       RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

#ifdef LTE_L2_MEAS

/** @brief This function captures the BSR value from Control Element
 *  Info structure and updates the effective Buffer size into the 
 *  corresponding LCG ID. 
 *
 * @details 
 *
 *     Function: rgTOMUtlL2MStoreBufSz
 *
 *         Processing steps:
 *         - update/append the Data structure based on BSR type 
 *
 * @param  [in] RgUeCb     *ueCb
 * @param  [in] RgInfCeInfo *ceInfo
 * @return S16
 */

#ifdef ANSI
PRIVATE S16 rgTOMUtlL2MStoreBufSz
(
 RgUeCb      *ueCb,
 RgInfCeInfo *ceInfo
 )
#else
PRIVATE S16 rgTOMUtlL2MStoreBufSz (ueCb, ceInfo)
 RgUeCb      *ueCb;
 RgInfCeInfo *ceInfo;
#endif
{
   U8 lcgId;
   U8 bsr;
   TRC2(rgTOMUtlL2MStoreBufSz);

   if(ceInfo->bitMask & RG_TRUNC_BSR_CE_PRSNT)
   {
      lcgId = ((ceInfo->ces.bsr.truncBsr >> 6) & 0x03);
      bsr = ceInfo->ces.bsr.truncBsr & 0x3F;
      ueCb->ul.lcgArr[lcgId].lcgBsInfo.outStndngBs = rgLwrBsrTbl[bsr];
   }
   else if(ceInfo->bitMask & RG_SHORT_BSR_CE_PRSNT)
   {
      lcgId = ((ceInfo->ces.bsr.shortBsr >> 6) & 0x03);
      bsr = ceInfo->ces.bsr.shortBsr & 0x3F;
      ueCb->ul.lcgArr[lcgId].lcgBsInfo.outStndngBs = rgLwrBsrTbl[bsr];

   }
   else if(ceInfo->bitMask & RG_LONG_BSR_CE_PRSNT)
   {
      ueCb->ul.lcgArr[0].lcgBsInfo.outStndngBs = rgLwrBsrTbl[ceInfo->ces.bsr.longBsr.bs1];
      ueCb->ul.lcgArr[1].lcgBsInfo.outStndngBs = rgLwrBsrTbl[ceInfo->ces.bsr.longBsr.bs2];
      ueCb->ul.lcgArr[2].lcgBsInfo.outStndngBs = rgLwrBsrTbl[ceInfo->ces.bsr.longBsr.bs3];
      ueCb->ul.lcgArr[3].lcgBsInfo.outStndngBs = rgLwrBsrTbl[ceInfo->ces.bsr.longBsr.bs4];
   }
   RETVALUE(ROK);
} /* end of rgTOMUtlL2MStoreBufSz*/

/** @brief : Compiles list of LCs received in UL data for DTCH RBs
 *
 * @details
 *
 * @param  [in] RgCellCb   *cellCb
 * @param  [in] RgUeCb     *ueCb
 * @param  [in] CmLteRnti  rnti
 * @param  [in] RgMacPdu   *pdu
 * @param 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PRIVATE Void rgTOML2MCompileActiveLCs
(
 RgCellCb      *cellCb, 
 RgUeCb        *ueCb,
 RgMacPdu      *pdu,
 RgInfCeInfo   *ceInfo 
 )
#else
PRIVATE Void rgTOML2MCompileActiveLCs(cellCb, ueCb, pdu, ceInfo)
   RgCellCb      *cellCb; 
   RgUeCb        *ueCb;
   RgMacPdu      *pdu;
   RgInfCeInfo   *ceInfo; 
#endif 
{
   CmLList           *node;
   RgMacSdu          *sdu;
   RgUlLcCb          *ulLcCb;

   TRC2(rgTOML2MCompileActiveLCs)

   node =  pdu->sduLst.first;
   while (node)
   {
      sdu = (RgMacSdu*)node->node;
      
      if ((ulLcCb = rgDBMGetUlDedLcCb(ueCb, sdu->lcId)), ulLcCb != NULLP)
      {
         if (ulLcCb->lcgId != 0)
         {
            ceInfo->bitMask |= RG_ACTIVE_LC_PRSNT;
            ceInfo->ulActLCs[ulLcCb->lcId - 1] = TRUE;
         }
      }
      node = node->next;
   }

} /* end of */ 



#endif
/**********************************************************************
 
         End of file:     gk_tom.c@@/main/3 - Sat Jul 30 02:21:55 2011
 
**********************************************************************/
 
/**********************************************************************
 
        Notes:
 
**********************************************************************/
 
/**********************************************************************
 
        Revision history:
 
**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---       sd   1. Initial Release.
/main/2      ---       sd   1. LTE MAC 2.1 release
           rg001.201   sm          1. Moved Processing before calling 
                                   interface function.
/main/2+   rg002.201   sd   1. ccpu00108810: Setting the pointer to NULL 
                               post transmission.
                            2. Changed the fucntions description.
           rg003.201   ns   1. Trace added to functions.
                       sm   2. Fixed Contention Resolution Id.
           rg004.201   rnt  1. Modified for SI Enhancement
           rg005.201   sm   1. Removed memset for optimization
           rg006.201   sd   1. ccpu00110797: Removed check for logical 
                               channel not configured at MAC in function 
                               'rgTOMUtlProcMsg'
           rg007.201   ap  1. Added support for MIMO
           rg008.201   sd  1. Removed dependency on MIMO compile-time flag
              rsharon 2. Added for SPS support.
           rg009.201 rsharon  1.[ccpu00112372]: CR timer changes.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
           rg002.301 asehgal 1. ccpu00120383: added checks for validity
                                of logical channel ID.
                             2. ccpu00118351: seperate deltas for UL & DL.
$SID$        ---      rt     1. LTE MAC 4.1 release
*********************************************************************91*/
