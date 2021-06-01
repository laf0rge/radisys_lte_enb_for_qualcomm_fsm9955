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
  
     File:     rg_dhm.c
  
     Sid:      gk_dhm.c@@/main/3 - Sat Jul 30 02:21:26 2011
  
     Prg:      ps
  
**********************************************************************/

/** @file rg_dhm.c
@brief APIs related to Downlink HARQ.
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=230;
/* header include files -- defines (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#ifdef L2_OPTMZ
#include "ss_strm.h"
#endif
#include "cm5.h"           /* common timers */
#include "cm_hash.h"       /* common hash list */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_llist.h"      /* common linked list library */
#include "cm_err.h"        /* common error */
#include "cm_lte.h"        /* common LTE */
#include "lrg.h"
#include "crg.h"
#include "rgu.h"
#include "tfu.h"
#include "rg_sch_inf.h"
#include "rg_env.h"
#include "rg_err.h"
#include "rg.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system service interface */
#ifdef L2_OPTMZ
#include "ss_strm.x"
#endif
#include "cm5.x"           /* common timers */
#include "cm_lib.x"        /* common library */
#include "cm_hash.x"       /* common hash list */
#include "cm_llist.x"      /* common linked list library */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"        /* common LTE */
#include "lrg.x"
#include "crg.x"
#include "rgu.x"
#include "tfu.x"
#include "rg_sch_inf.x"
#include "rg.x"

#ifdef L2_OPTMZ
#include "ss_queue.h"
#include "ss_queue.x"
#include "ss_task.x"
#include "ss_msg.x"            /* MAC includes */
/* local defines */
EXTERN  S16 ssGetDBufOfSize(Region region,Size size,Buffer **dBuf);
//void prc_trace_format_string(UINT32 group_mask, UINT16 level, const char *format, ...);
#endif

/* local typedefs */
 
/* local externs */
PRIVATE Void rgDHMBldTfuDatReq ARGS((RgCellCb *cellCb, RgDlSf *dlSf, RgDlHqProcCb *hqP,
                           RgTfuDatReqPduInfo *datReq));
#ifdef L2_OPTMZ
#define RG_MAC_HDR_SIZE 250
#define RG_MAC_CE_SIZE  250
U32 MacPtrAddress;
U32 macHeader[2];
#if 0
#else
#define RG_ADD_DBuf(_dBuf,_size,_mBuf)\
{ \
  SsMsgInfo *mInfo = NULLP; \
  mInfo = (SsMsgInfo *)_mBuf->b_rptr; \
  ssGetDBufOfSize(mInfo->region,_size, &_dBuf); \
  SUpdMsg(_mBuf, _dBuf,0);\
  _dBuf->b_wptr = _dBuf->b_rptr =  (_dBuf->b_datap->db_base + 5);\
  MacPtrAddress = (U32)_dBuf->b_wptr;\
}
#endif
  //printf("The write & read ptr is %p %p %p %p \n", _dBuf->b_wptr, _dBuf->b_rptr,_dBuf->b_datap->db_base, _dBuf->b_datap->db_lim );\
  //printf("The write & read ptr is %p %p \n", _dBuf->b_wptr, _dBuf->b_rptr);\

#endif

/* forward references */

/**
 * @brief This function initializes the DL HARQ Entity of UE
 *
 * @details
 *
 *     Function: rgDHMHqEntInit
 *     Purpose:  This function initializes the DL HARQ entity of 
 *               UE control block. This is performed at the time
 *               of creating UE control block.
 *     
 *     Invoked by: configuration module
 *     
 *  @param[in]  RgCellCb*  cell
 *  @param[in]  RgUeCb*    ue
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
#ifdef ANSI
PUBLIC S16 rgDHMHqEntInit
(
RgDlHqEnt          *hqE,
U8                 maxHqProcs
)
#else
PUBLIC S16 rgDHMHqEntInit(hqE, maxHqProcs)
RgDlHqEnt          *hqE;
U8                 maxHqProcs;
#endif
{
   U8 idx1,idx2;
#ifdef L2_OPTMZ
   Buffer  *hdrDBuf = NULLP;
   Buffer  *ceDBuf = NULLP;
#endif

   TRC2(rgDHMHqEntInit)

   hqE->numHqProcs = maxHqProcs;
   /* for each harq process */
   for (idx1 = 0; idx1 < hqE->numHqProcs; idx1++)
   {
      if (rgAllocSBuf((Data **)&(hqE->procs[idx1]),sizeof(RgDlHqProcCb)) != ROK) 
      {
         while(idx1--)
         {
            rgFreeSBuf((Data **)&(hqE->procs[idx1]), sizeof(RgDlHqProcCb));
         }
         RLOG0(L_ERROR, "Memory Alloc Failure for RgDlHqProcCb");        
         RETVALUE(RFAILED);
      }

      hqE->procs[idx1]->procId      = idx1;
      for(idx2 = 0; idx2 < RG_MAX_TB_PER_UE; idx2++)
      {
#ifndef L2_OPTMZ
         hqE->procs[idx1]->tbInfo[idx2].tb = NULLP;
#else
         Buffer *tmpMBuf;
         /* L2 optimization for mUe/Tti: Allocating buffers for macHdr, macCes
          * and macPadding. These buffers shall not be released by MAC/CL.
          * However, Only rPtr and wPtr will be reset while release of hq proc
          */
         tmpMBuf = hqE->procs[idx1]->tbInfo[idx2].tb.macHdr;
         rgGetMsg(&tmpMBuf);
         RG_ADD_DBuf(hdrDBuf, RG_MAC_HDR_SIZE, tmpMBuf);
         hqE->procs[idx1]->tbInfo[idx2].tb.macHdr = tmpMBuf;
         macHeader[idx2] = MacPtrAddress;

         tmpMBuf = hqE->procs[idx1]->tbInfo[idx2].tb.macCes;
         rgGetMsg(&tmpMBuf);
         RG_ADD_DBuf(ceDBuf, RG_MAC_CE_SIZE, tmpMBuf);
         hqE->procs[idx1]->tbInfo[idx2].tb.macCes = tmpMBuf;

         hqE->procs[idx1]->tbInfo[idx2].tb.padSize = 0;
#endif
#ifdef LTE_L2_MEAS
         hqE->procs[idx1]->tbId[idx2] = RGU_INVALID_TBID;
#endif
      }
   }


   RETVALUE(ROK);
} /* rgDHMHqEntInit */

/**
 * @brief This function releases a HARQ process
 *
 * @details
 *
 *     Function: rgDHMUeReset
 *     Purpose:  This function resets TB in each HarqProc.
 *     
 *     Invoked by: CFG UE Reset 
 *     
 *  @param[in]  RgDlHqProc    *hqP
 *  @return  Void      
 *
 **/
#ifdef ANSI
PUBLIC Void rgDHMUeReset
(
RgDlHqEnt          *hqE
)
#else
PUBLIC Void rgDHMUeReset(hqE)
RgDlHqEnt          *hqE;
#endif
{
   U8       i = 0;

   TRC2(rgDHMUeReset)

   if(hqE->procs[0])
   {
      /* Free all the TB memory associated with HARQ */
      for (i=0; i < hqE->numHqProcs; i++)
      {
         rgDHMRlsHqProcTB(hqE->procs[i], 1);
         rgDHMRlsHqProcTB(hqE->procs[i], 2);
      }
   }
   RETVOID;
} /* rgDHMUeReset*/

/**
 * @brief This function defers shared memory buffer
 *        freeing out of the critical RT path.
 *
 * @details
 *
 *     Function: rgDHMHdlBufFree
 *     Purpose: To defer shared memory freeing post 
 *              critical path. Defer as many if defer queue 
 *              is full then release instantly.
 *     
 *     Invoked by: HARQ TB Release. 
 *     
 *  @return  Void      
 *
 **/
#ifdef ANSI
PUBLIC Void rgDHMHdlBufFree
(
Buffer **mBuf
)
#else
PUBLIC Void rgDHMHdlBufFree(Buffer **mBuf)
#endif
{
   TRC2(rgDHMHdlBufFree)

   if (rgCb.bufCnt < RG_MAX_DFRD_FREE_BUFS)
   {
      if (*mBuf)
      {
         rgCb.bufToFree[rgCb.bufCnt] = *mBuf;
         rgCb.bufCnt++;
         *mBuf = NULLP;
      }
   }
   else
   {
      RG_FREE_MSG(*mBuf);
   }
   RETVOID;
}
/**
 * @brief This function is called to release the 
 *        shared memory of the HARQ TBs outside 
 *        the critical RT path.
 *
 * @details
 *
 *     Function: rgDHMFreeTbBufs
 *     Purpose: This function is called to release the 
 *        shared memory of the HARQ TBs outside 
 *        the critical RT path.
 *     
 *     1. Job of releasing TBs is shared across TTIs
 *     Invoked by: MAC every TTI 
 *     
 *  @return  Void      
 *
 **/
#ifdef ANSI
PUBLIC Void rgDHMFreeTbBufs
(
)
#else
PUBLIC Void rgDHMFreeTbBufs()
#endif
{
   U8 start = rgCb.bufCnt;
   U8 end = 0;

   TRC2(rgDHMFreeTbBufs)

   if (rgCb.bufCnt < RG_MAX_FREE_BUFS_PERTTI)
   {
      end = 0;
   }
   else
   {
      end = rgCb.bufCnt - RG_MAX_FREE_BUFS_PERTTI;
   }
   while (start != end)
   {
      start--;
      SPutMsg(rgCb.bufToFree[start]);
   }
   rgCb.bufCnt = end;
   RETVOID;
} /* rgDHMFreeTbBufs */

/**
 * @brief This function releases a HARQ process
 *
 * @details
 *
 *     Function: rgDHMRlsHqProcTB
 *     Purpose:  This function returns a HARQ process to HARQ Entity 
 *               in the DL direction.
 *     
 *               1. Add the HARQ process to the free queue.
 *     Invoked by: scheduler and HARQ processing
 *     
 *  @param[in]  RgDlHqProc    *hqP
 *  @return  Void      
 *
 **/
#ifdef ANSI
PUBLIC S16 rgDHMRlsHqProcTB
(
RgDlHqProcCb         *hqP,
U8                   tbIndex
)
#else
PUBLIC S16 rgDHMRlsHqProcTB(hqP, tbIndex)
RgDlHqProcCb         *hqP;
U8                   tbIndex;
#endif
{
#ifdef L2_OPTMZ
    RgTfuDatReqTbInfo     *tb;   /* TB to be sent to CL/PHY*/
   // U32 lchIdx, pduIdx;
#endif

   TRC2(rgDHMRlsHqProcTB)

   if((tbIndex > RG_MAX_TB_PER_UE) ||
      (tbIndex == 0))
   {
      RETVALUE(RFAILED);
   }

   hqP->tbInfo[tbIndex-1].numSchLch = 0;
#ifndef L2_OPTMZ
   rgDHMHdlBufFree(&hqP->tbInfo[tbIndex-1].tb);
#else
   /* L2 Optimization for mUe/Tti:  macHdr, macCes and macPad mBuf pointers
    * shall not be released. However, Inorder to release harq info/TB info,
    * just Resetting rPtr and wPtr of these mbufs to db_base
    */
   tb = &(hqP->tbInfo[tbIndex-1].tb);
//   printf("\nrgDHMRlsHqProcTB: RG_FREE_TB1 \n");
   RG_FREE_TB(tb);
#endif
   hqP->tbInfo[tbIndex-1].schdTa.pres = FALSE;
   /* Decrementing might lead to roundoff error in case of say UE reset
    * where all the HqProcs irrespective whether in use are called for rls.
    * Hence to avoid the same shift operator is being used. */
   hqP->numOfTBs = hqP->numOfTBs >> 1;
   if (hqP->tbInfo[tbIndex-1].sfLnk.node != NULLP)
   {
      cmLListDelFrm(&hqP->tbInfo[tbIndex-1].sf->tbs,
                    &hqP->tbInfo[tbIndex-1].sfLnk);
      hqP->tbInfo[tbIndex-1].sfLnk.node  = (PTR)NULLP;
   }
   hqP->tbInfo[tbIndex-1].sf = NULLP;
   /* Fix : syed It is better to reset these feilds
    * corruption avoidance */
   hqP->tbInfo[tbIndex-1].tbSz = 0;
   hqP->tbInfo[tbIndex-1].contResCe = NOTPRSNT;

   RETVALUE(ROK);
} /* rgDHMRlsHqProc */

/**
 * @brief This function gets HARQ process with the given Id
 *
 * @details
 *
 *     Function: rgDHMGetHqProcFrmId
 *     Purpose:  This function returns the HARQ process with the given ID.
 *     Invoked by: ROM
 *     
 *  @param[in]  RgUeCb        *ue
 *  @param[in]  U8            idx
 *  @param[in]  RgDlHqProc    **hqP
 *  @return  S16       
 *         -#   ROK     if successful
 *         -#   RFAILED otherwise
 *
 **/
#ifdef ANSI
PUBLIC S16 rgDHMGetHqProcFrmId
(
RgUeCb               *ue,
U8                   idx,
RgDlHqProcCb         **hqP
)
#else
PUBLIC S16 rgDHMGetHqProcFrmId(ue, idx, hqP)
RgUeCb               *ue;
U8                   idx;
RgDlHqProcCb         **hqP;
#endif
{
   TRC2(rgDHMGetHqProcFrmId)

   /* Pick the proc based on the index provided */
   *hqP = (ue->dl.hqEnt.procs[idx]);

   RETVALUE(ROK);
} /* rgDHMGetHqProcFrmId */

/*PRIVATE U32 dataAvl; */
/**
 * @brief Handler for sending data to PHY
 *
 * @details
 *
 *     Function : rgDHMSndDatReq
 *     
 *     This function shall send the MAC PDU built for the UE to TOM
 *     when invoked as part of TTI processing and keep track of the number of
 *     transmissions for this TB.
 *     
 *           
 *  @param[in]  RgCellCb      *cell
 *  @param[in]  RgDlHqProcCb  *hqE 
 *  @param[out] RgErrInfo     *err 
 *  @return     S16
 *      -#ROK 
 *      -#RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgDHMSndDatReq
(
RgCellCb        *cellCb,
RgDlSf          *dlSf,
RgTfuDatReqInfo *datInfo,
RgDlHqProcCb   *hqP,
RgErrInfo      *err 
)
#else
PUBLIC S16 rgDHMSndDatReq(cellCb, dlSf, datInfo, hqP, err)
RgCellCb        *cellCb;
RgDlSf          *dlSf;
RgTfuDatReqInfo *datInfo;
RgDlHqProcCb    *hqP;
RgErrInfo       *err;
#endif
{
   U8 i;
   RgTfuDatReqPduInfo   *datReq;
   RgBldPduInfo      bldPdu;
   /*Added this variable to figure out that whether this UE data
     has to be inclueded in the TFU Data request.*/
   Bool  dataAvlblUe;

   TRC2(rgDHMSndDatReq)
  
   dataAvlblUe = TRUE;
   for(i=0;i< RG_MAX_TB_PER_UE;i++)
   {
      if (hqP->tbInfo[i].sf == dlSf)
      {
         /* Check if data from RLC had been received and got muxed. */
#ifndef L2_OPTMZ
         if (hqP->tbInfo[i].tb == NULLP) 
#else
         if (!(hqP->tbInfo[i].tb.tbPres)) 
#endif
         {
            if (hqP->tbInfo[i].schdTa.pres == TRUE ||
                  hqP->tbInfo[i].contResCe == PRSNT_NODEF)
            {
               /* Data not received but ta needs to be sent. */
               /* MUX TA and send it */
               bldPdu.datReq    =  NULLP;
               bldPdu.reqType   =  EVTTFUTTIIND;
               bldPdu.schdTbSz  =  hqP->tbInfo[i].tbSz;
               bldPdu.ta        =  hqP->tbInfo[i].schdTa;
               /* changes for CR timer implementation*/
               bldPdu.contResId =  hqP->tbInfo[i].contResId;
               if (ROK != rgMUXBldPdu(&bldPdu, &(hqP->tbInfo[i].tb), err))  
               {
                  RETVALUE(RFAILED);
               }
            }
            else   
            {
#ifdef LTEMAC_RGU_PAD
               /* Data not received from RLC. Padding at MAC */
               bldPdu.datReq    =  NULLP;
               bldPdu.reqType   =  EVTTFUTTIIND;
               bldPdu.schdTbSz  =  hqP->tbInfo[i].tbSz;
               bldPdu.ta        =  hqP->tbInfo[i].schdTa;
               bldPdu.ta.val    =  0;
               bldPdu.contResId =  NULLP;

               if (ROK != rgMUXBldPdu(&bldPdu, &(hqP->tbInfo[i].tb), err))  
               {
                  RETVALUE(RFAILED);
               }
#else
               /*Padding is not done so data for this UE will not be
                 included.*/
               dataAvlblUe = FALSE;
#endif
            }
         }
         else
         {
         }
      }
   }

   /*If Data/Padding is not available for UE, then we can not include
     any Data for this UE in TFU Data Request.*/
   if(!dataAvlblUe)
   {
      /*Free up the HARQ process for this allocation.*/
      /* Release First TB, as this would be anyway there*/
      rgDHMRlsHqProcTB(hqP, 1);
      if(2 == hqP->numOfTBs)
      {
         rgDHMRlsHqProcTB(hqP, 2);
      }
      
      RETVALUE(ROK);
   }

   if (rgGetEventMem((Ptr *)&datReq, sizeof(TfuDatReqPduInfo),
            &(datInfo->memCp)) != ROK)
   {
      RETVALUE(RFAILED);
   }
   /* Fill the TFU Dat Req with information from Harq Proc */
  
   rgDHMBldTfuDatReq(cellCb, dlSf, hqP, datReq);

   /* MS_WORKAROUND for ccpu00122894 */
   for(i=0;i< RG_MAX_TB_PER_UE;i++)
   {
      if (hqP->tbInfo[i].sf == dlSf)
      {
         cmLListDelFrm(&dlSf->tbs, &hqP->tbInfo[i].sfLnk);
         hqP->tbInfo[i].sfLnk.node = NULLP;
      }
      hqP->tbInfo[i].sf = NULLP;
   }
   cmLListAdd2Tail(&datInfo->pdus, &(datReq->lnk));
   datReq->lnk.node = (PTR)datReq;

   RETVALUE(ROK);
}  /* rgDHMSndDatReq */

/**
 * @brief Function to handle RGU datReq received from ROM
 *
 * @details
 *
 *     Function : rgDHMHndlDedDatReq
 *     
 *     This function shall act on the datReq received on RGU. It shall 
 *     store the data IDs for all the logical channels and get the MAC 
 *     PDU built.
 *     
 *           
 *  @param[in]  RgDlHqProcCb   *hqProc 
 *  @param[in]  RgRguDedDatReq *datReq
 *  @param[out] RgErrInfo      *err
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgDHMHndlDedDatReq
(
RgDlHqProcCb   *hqProc,
RgRguDDatReqPerUe *datReq,
RgErrInfo      *err
)
#else
PUBLIC S16 rgDHMHndlDedDatReq(hqProc, datReq, err)
RgDlHqProcCb   *hqProc;
RgRguDDatReqPerUe *datReq;
RgErrInfo      *err;
#endif
{
   U8             i;
   U8             j;
   RgBldPduInfo   bldPdu;
   U8             tbIndex;
#ifdef L2_OPTMZ
   RgTfuDatReqTbInfo     *tb;
#endif

   TRC2(rgDHMHndlDedDatReq);

   tbIndex = (U8)(datReq->transId & 0x03);
   /* Accept all the data requests even if delayed in case nothing
    * has been sent earlier on the harq proc.
    */
   if((datReq->nmbOfTbs > RG_MAX_TB_PER_UE) ||
         (tbIndex == 0))
   {
      /* release corresponding TBs from SF tbs List */
      for(j=0;j<datReq->nmbOfTbs;j++)
      {
         if (!(tbIndex & (j+1)))
         {
            j++;
         } 
         rgDHMRlsHqProcTB(hqProc, (U8)(j+1));
      }
      RETVALUE(RFAILED);
   }

   for(i=0;i<datReq->nmbOfTbs;i++)
   {
      /* tbIndex 01 corresponds to presence of 1st TB
       * 10 corresponds 2nd TB
       * 11 corresponds two TBs of UE */
      if (!(tbIndex & (i+1)))
      {
          continue;
      }
      if (hqProc->tbInfo[i].sfLnk.node == NULLP)
      {
         /* release corresponding TBs from SF tbs List */
         for(j=0;j<datReq->nmbOfTbs;j++)
         {
            if (!(tbIndex & (j+1)))
            {
               j++;
            }
            rgDHMRlsHqProcTB(hqProc, (U8)(j+1));
         }
         RETVALUE(RFAILED);

      }
#ifndef L2_OPTMZ
      RG_FREE_MSG(hqProc->tbInfo[i].tb);
      /* L2 optimization for mUe/Tti: Pre-allocated mBuf pointers(macHdr, 
       * macCes and MacPad) of harq TB need to be reset to db_base
       */
#else
      tb = &hqProc->tbInfo[i].tb;
  //  printf("rgDHMHndlDedDatReq: RG_FREE_TB");
      RG_FREE_TB(tb);
#endif
//printf("Sending from 2 %d \n", datReq->rnti);
      bldPdu.datReq    =  datReq;
      bldPdu.reqType   =  EVTRGUDDATREQ;
      bldPdu.schdTbSz  =  hqProc->tbInfo[i].tbSz;
      bldPdu.tbIndex   =  i+1;
      bldPdu.ta        =  hqProc->tbInfo[i].schdTa;
      bldPdu.contResId =  NULLP;
#ifdef LTE_L2_MEAS
      /* Store tbId from RLC in DDatRequest */
      hqProc->tbId[i] = datReq->datReqTb[i].tbId;


      hqProc->status[i] =  FALSE;
#endif 
      if(rgMUXBldPdu(&bldPdu, &(hqProc->tbInfo[i].tb), err) != ROK)
      {
         /* release corresponding TBs from SF tbs List */
         for(j=0;j<datReq->nmbOfTbs;j++)
         {
            if (!(tbIndex & (j+1)))
            {
               j++;
            }
            rgDHMRlsHqProcTB(hqProc, (U8)(j+1));
         }
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}  /* rgDHMHndlDedDatReq */

/**
 * @brief Function to handle RGU datReq received from ROM
 *
 * @details
 *
 *     Function : rgDHMHndlCmnDatReq
 *     
 *     This function shall act on the datReq received on RGU. It shall 
 *     store the data IDs for all the logical channels and get the MAC 
 *     PDU built.
 *     
 *           
 *  @param[in]  RgDlHqProcCb   *hqProc 
 *  @param[in]  RgRguCmnDatReq *datReq
 *  @param[out] RgErrInfo      *err
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgDHMHndlCmnDatReq
(
RgDlHqProcCb   *hqProc,
RgRguCmnDatReq *datReq,
RgErrInfo      *err
)
#else
PUBLIC S16 rgDHMHndlCmnDatReq(hqProc, datReq, err)
RgDlHqProcCb   *hqProc;
RgRguCmnDatReq *datReq;
RgErrInfo      *err;
#endif
{
   RgUstaDgn      dgn;
   RgBldPduInfo   bldPdu;

   TRC2(rgDHMHndlCmnDatReq)

#ifndef L2_OPTMZ
      if (hqProc->tbInfo[0].tb != NULLP)
#else
      /* If numLch is non zero means HQ Proc is busy*/
      if (hqProc->tbInfo[0].tb.tbPres)
#endif
      {
         /* datReq discarded. Generate an alarm */
         rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_HARQ); 
         rgLMMStaInd(LCM_CATEGORY_PROTOCOL, LCM_EVENT_UI_INV_EVT,
               LRG_CAUSE_HQ_PROC_BUSY, &dgn);
         RETVALUE(RFAILED);
      }

   bldPdu.datReq    =  datReq;
   bldPdu.reqType   =  EVTRGUCDATREQ;
   bldPdu.schdTbSz  =  hqProc->tbInfo[0].tbSz;
   bldPdu.ta        =  hqProc->tbInfo[0].schdTa;
   bldPdu.contResId  =  hqProc->tbInfo[0].contResId;

   if(rgMUXBldPdu(&bldPdu, &(hqProc->tbInfo[0].tb), err) != ROK)
   {
      RG_FREE_MSG(datReq->pdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}  /* rgDHMHndlCmnDatReq */

/**
 * @brief Function to get consolidate grants and send consolidated grant to RLC
 *
 * @details
 *
 *     Function : rgDHMSndConsolidatedStaInd
 *     
 *     This function shall be invoked by Scheduler to trigger DHM to send a
 *     consolidated status indication of all UE scheduled in a TTI as well as
 *     send consolidated CStaInd for MSG4 and for all common channels(PCCH,
 *     if RGR_SI_SCH is not defined then it includes BCH and BCCH also)
 *     
 *           
 *  @param[in]  RgCellCb       *cell
 *  @param[in]  RgInfUeInfo   *ueInfo,
 *  @param[in]  CmLteTimingInfo timingInfo,
 *  @param[out] RgErrInfo      err
 *  @param[in]  RguCStaIndInfo   *cStaInd
 *  @return     S16
 *      -# ROK 
 *      -# RFAILED 
 **/
#ifdef ANSI
PUBLIC S16 rgDHMSndConsolidatedStaInd
(
RgCellCb        *cell,
RgInfUeInfo     *ueInfo,
CmLteTimingInfo timingInfo,
RgErrInfo       *err
)
#else
PUBLIC S16 rgDHMSndConsolidatedStaInd(cell, ueInfo, timingInfo, err)
RgCellCb        *cell;
RgInfUeInfo     *ueInfo;
CmLteTimingInfo timingInfo;
RgErrInfo       *err;
#endif
{
   RgRguDedStaInd  *dStaInd = NULLP;
   U8              idx;
   U8              ueIdx;
   U8              lcIdx;
   U8              tbIndex=0,idx1;
   RgDlSf          *dlSf = &cell->subFrms[timingInfo.subframe];
//   Bool            isDStaReqrd = FALSE;

   int lchBufSize =0;
   RgUeCb         *ue;
   RgDlHqProcCb   *hqP;
   RgInfUeAlloc   *allocInfo;
#ifdef L2_OPTMZ
   RgTfuDatReqTbInfo     *tb;   /* TB to be sent to CL/PHY*/
#endif

   TRC2(rgDHMSndConsolidatedStaInd)

   /* Send StaInd for the scheduled UEs */
   for(ueIdx = 0; ueIdx < ueInfo->numUes; ueIdx++)
   {
      if((ue=rgDBMGetUeCb (cell, ueInfo->allocInfo[ueIdx].rnti)) == NULLP)
      {
         /* Check in RachLst */
         if((ue=rgDBMGetUeCbFromRachLst (cell, 
                     ueInfo->allocInfo[ueIdx].rnti)) == NULLP)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,"CRNTI:%d No ueCb found", 
                      ueInfo->allocInfo[ueIdx].rnti);
            /*Fix: If one UE is not present dont return, look for the next.*/
            continue;
         }
      }

      rgDHMGetHqProcFrmId(ue,ueInfo->allocInfo[ueIdx].hqProcId,&hqP);
      allocInfo = &ueInfo->allocInfo[ueIdx];
      /* Fix : syed Avoid sending data for a RETX
       * if initial TX data processing was unsuccessful */
      if((allocInfo->tbInfo[0].isReTx == TRUE) &&
            (hqP->tbInfo[0].tbSz == 0)) 
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
               "CRNTI:%d RETX hqP(%d) tb(0) for a failed New Tx", 
                  allocInfo->rnti, hqP->procId);        
         continue;
      }
      if((allocInfo->tbInfo[1].isReTx == TRUE) &&
            (hqP->tbInfo[1].tbSz == 0)) 
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId,
                   "CRNTI:%d RETX hqP(%d) tb(1) for a failed New Tx", 
                  allocInfo->rnti, hqP->procId);        
         continue;
      }

      for(idx=allocInfo->tbStrtIdx;((idx-allocInfo->tbStrtIdx) <\
               allocInfo->nmbOfTBs); idx++)
      {
         RguCStaIndInfo  *cStaInd;
#ifdef TFU_UPGRADE
         /* LTE_ADV_FLAG_REMOVED_START */
         hqP->tbInfo[idx].isEnbSFR = allocInfo->isEnbSFR;
         /* update pA value */
         hqP->tbInfo[idx].pa = allocInfo->pa;
         /* LTE_ADV_FLAG_REMOVED_END */
#endif

         hqP->numOfTBs =  allocInfo->nmbOfTBs;
         hqP->tbInfo[idx].sfLnk.node = (PTR)hqP;
         hqP->tbInfo[idx].sf = dlSf;
         cmLListAdd2Tail(&dlSf->tbs,&hqP->tbInfo[idx].sfLnk);

         /* Changes as part of performance testing*/
         /*   hqP->numOfTBs++;*/
         hqP->tbInfo[idx].doa = allocInfo->doa;
         hqP->tbInfo[idx].txMode = allocInfo->txMode;
         hqP->tbInfo[idx].puschRptUsd = allocInfo->puschRptUsd;
         hqP->tbInfo[idx].puschPmiInfo = allocInfo->puschPmiInfo;
#ifdef LTEMAC_SPS
         hqP->tbInfo[idx].pdcch.rnti = allocInfo->pdcchRnti;
#else
         hqP->tbInfo[idx].pdcch.rnti = allocInfo->rnti;
#endif
         if(allocInfo->tbInfo[idx].isReTx == TRUE)
         {
            hqP->tbInfo[idx].pdcch.dci = allocInfo->dciInfo;
            continue;
         }

         if (allocInfo->tbInfo[idx].schdDat[0].lcId == RG_CCCH_LCID)        
         {
            hqP->tbInfo[idx].pdcch.dci = allocInfo->dciInfo;
#ifndef L2_OPTMZ
            RG_FREE_MSG(hqP->tbInfo[idx].tb);
#else
      /* L2 optimization for mUe/Tti: Pre-allocated mBuf pointers(macHdr, 
       * macCes and MacPad) of harq TB need to be reset to db_base
       */
       tb = &(hqP->tbInfo[idx].tb);
   // printf("rgDHMSndConsolidatedStaInd: sending grant to RLC for msg4. Reseting TB");
       RG_FREE_TB(tb);
#endif
            hqP->tbInfo[idx].timingInfo = timingInfo;
            hqP->tbInfo[idx].tbSz = allocInfo->tbInfo[idx].schdTbSz; 

            hqP->tbInfo[idx].schdTa.pres = allocInfo->tbInfo[idx].ta.pres;
            hqP->tbInfo[idx].schdTa.val  = allocInfo->tbInfo[idx].ta.val;
            hqP->tbInfo[0].contResCe  = allocInfo->tbInfo[0].contResCe;
            if(allocInfo->tbInfo[0].contResCe)
            {
               hqP->tbInfo[0].contResId = &ue->contResId;
            }


            if(allocInfo->tbInfo[idx].numSchLch == 0)
            {
               RLOG_ARG2(L_DEBUG,DBG_CELLID,cell->cellId,
                        "UEID:%d MSG4 with only contResId hqP(%d)",
               			allocInfo->rnti,
                     	hqP->procId);
               hqP->tbInfo[idx].numSchLch = 0;
               continue;
            }

            /* Increamenting the tbIndex instead of
               assigning it to constant */
            tbIndex++;


            hqP->tbInfo[idx].numSchLch = 1;
            hqP->tbInfo[idx].schdData[0].lcId = 
               allocInfo->tbInfo[idx].schdDat[0].lcId;
            hqP->tbInfo[idx].schdData[0].schdSz = 
               allocInfo->tbInfo[idx].schdDat[0].numBytes;

           // if(cStaInd == NULLP)
            {
               if ((rgAllocShrablSBuf((Data**)&cStaInd, sizeof(RguCStaIndInfo))) != ROK)
               {
                  err->errType  = RGERR_DHM_SND_STA_IND;
                  err->errCause = RG_DHM_MEM_ALLOC_FAIL;
                  RETVALUE(RFAILED); 
               }
            }

            idx1 = (hqP->procId << 2) | tbIndex;
            
            cStaInd->cellId    = cell->cellId;
            cStaInd->rnti      = allocInfo->rnti;
            cStaInd->lcId      = cell->dlCcchId;
            cStaInd->transId   = (timingInfo.sfn << 16) | 
                                 (timingInfo.subframe << 8) | idx1;
               /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
            dlSf->remDatReqCnt++;
#endif
            RLOG_ARG3(L_DEBUG,DBG_CELLID,cell->cellId,
		               "RNTI:%d UE:MSG4 grant for CCCH hqP(%d) LCID:%d",
                     allocInfo->rnti, 
                     hqP->procId,
		               cStaInd->lcId);       
            /* Fix : syed Avoid return param for interface prim and
             * proceed for other UEs. For the failed UE, MAC shall
             * do padding. */
            rgUIMSndCmnStaInd(cStaInd);
            break;
         }
         else
         {
            hqP->tbInfo[idx].pdcch.dci = allocInfo->dciInfo;
            hqP->tbInfo[idx].schdTa.pres = allocInfo->tbInfo[idx].ta.pres;
            hqP->tbInfo[idx].schdTa.val  = allocInfo->tbInfo[idx].ta.val;
            tbIndex+=idx+1;
#ifndef L2_OPTMZ
            RG_FREE_MSG(hqP->tbInfo[idx].tb);
#else
      /* L2 optimization for mUe/Tti: Pre-allocated mBuf pointers(macHdr, 
       * macCes and MacPad) of harq TB need to be reset to db_base
       */
       tb = &(hqP->tbInfo[idx].tb);
    //printf("rgDHMSndConsolidatedStaInd: sending grant for dedicated chanel: resting TB rebore");
       RG_FREE_TB(tb);
#endif

            hqP->tbInfo[idx].timingInfo = timingInfo;
            hqP->tbInfo[idx].tbSz = allocInfo->tbInfo[idx].schdTbSz; 
            if((NULLP == dStaInd) && (allocInfo->tbInfo[idx].numSchLch))
            {
               if ((rgAllocShrablSBuf ((Data**)&dStaInd, sizeof(RguDStaIndInfo))) != ROK)
               {
                  err->errType  = RGERR_DHM_SND_STA_IND;
                  err->errCause = RG_DHM_MEM_ALLOC_FAIL;
                  /* Need to return as memory allocation will fail for other UEs also*/
                  RETVALUE(RFAILED);
               }
               dStaInd->nmbOfUeGrantPerTti = 0;
            }

            for (lcIdx = 0; 
                  lcIdx < allocInfo->tbInfo[idx].numSchLch; lcIdx++)
            {
               hqP->tbInfo[idx].schdData[lcIdx].lcId = 
                  allocInfo->tbInfo[idx].schdDat[lcIdx].lcId;
               if (hqP->tbInfo[idx].schdData[lcIdx].lcId == 0)
               {
                  RLOG_ARG2(L_ERROR,DBG_CELLID,cell->cellId, 
                           "CCCH grant in DStaInd for LCID:%d CRNTI:%d",
                           hqP->tbInfo[idx].schdData[lcIdx].lcId,allocInfo->rnti);
               }
               hqP->tbInfo[idx].schdData[lcIdx].schdSz = 
                  allocInfo->tbInfo[idx].schdDat[lcIdx].numBytes;
               if(dStaInd)
               {
                  dStaInd->staInd[dStaInd->nmbOfUeGrantPerTti].staIndTb[idx].\
                  lchStaInd[lcIdx].lcId = allocInfo->tbInfo[idx].\
                  schdDat[lcIdx].lcId;
                  dStaInd->staInd[dStaInd->nmbOfUeGrantPerTti].staIndTb[idx].\
                  lchStaInd[lcIdx].totBufSize = allocInfo->tbInfo[idx].\
                  schdDat[lcIdx].numBytes;
               }

               lchBufSize+=allocInfo->tbInfo[idx].schdDat[lcIdx].numBytes;
            }
            hqP->tbInfo[idx].numSchLch = 
               allocInfo->tbInfo[idx].numSchLch;
            if(dStaInd)
            {
               dStaInd->staInd[dStaInd->nmbOfUeGrantPerTti].staIndTb[idx].nmbLch =
                  allocInfo->tbInfo[idx].numSchLch;
#ifdef LTE_L2_MEAS
               dStaInd->staInd[dStaInd->nmbOfUeGrantPerTti].staIndTb[idx].tbId =
                  hqP->tbId[idx]; 
#endif
            }
            lchBufSize=0;
         }
      }
      //if((dStaInd) && (tbIndex) && (isDStaReqrd == TRUE))
      if((dStaInd) && (tbIndex))
      {
         idx1 = (hqP->procId << 2) | tbIndex;
         /* Create RguDStaInd struct and send to UIM */
         dStaInd->staInd[dStaInd->nmbOfUeGrantPerTti].rnti    = allocInfo->rnti;
         /*
            dStaInd->transId = (hqP->timingInfo.sfn << 16) | 
            (hqP->timingInfo.subframe << 8) | hqP->procId;
          */
         dStaInd->staInd[dStaInd->nmbOfUeGrantPerTti].transId = (timingInfo.sfn << 16) | 
            (timingInfo.subframe << 8) | idx1;
         dStaInd->staInd[dStaInd->nmbOfUeGrantPerTti].nmbOfTbs = hqP->numOfTBs;
         /*increment num of UE as staInd is prepared for it */
         dStaInd->nmbOfUeGrantPerTti++;
         /* ADD Changes for Downlink UE Timing Optimization */
#ifdef LTEMAC_DLUE_TMGOPTMZ
         dlSf->remDatReqCnt++;
#endif
      }
      //isDStaReqrd = FALSE;
      tbIndex = 0;
   }
  // if((dStaInd) && ((dStaInd->nmbOfUeGrantPerTti) >= 1))
   if(dStaInd)
   {
      dStaInd->cellId  = cell->cellId;
      /* Fix : syed Avoid return param for interface prim and
       * proceed for other UEs. For the failed UE, MAC shall
       * do padding. */
      rgUIMSndDedStaInd(dStaInd);
   }
   RETVALUE(ROK);
}  /* rgDHMSndConsolidatedStaInd */


/**
 * @brief Function to handle building the TFU Data Request
 *
 * @details
 *
 *     Function : rgDHMBldTfuDatReq
 *     
 *     This function builds the TFU Data Request with the details 
 *     present in HARQ Process.
 *           
 *  @param[in]  RgDlHqProcCb     *hqP 
 *  @param[out] TfuDatReqPduInfo  *datReq 
 *  @return     Void
 *              None 
 **/
#ifdef ANSI
PRIVATE Void rgDHMBldTfuDatReq
(
RgCellCb           *cellCb,
RgDlSf             *dlSf,
RgDlHqProcCb       *hqP,
RgTfuDatReqPduInfo *datReq
)
#else
PRIVATE Void rgDHMBldTfuDatReq(cellCb, dlSf, hqP, datReq)
RgCellCb           *cellCb;
RgDlSf             *dlSf;
RgDlHqProcCb       *hqP;
RgTfuDatReqPduInfo *datReq;
#endif
{
   U8 i;
   //RgUeCb *ueCb = NULLP;
#ifdef L2_OPTMZ
   U32 lchIdx, pduIdx;
#endif
   TRC2(rgDHMBldTfuDatReq)
 
   datReq->nmbOfTBs = 0;
   /*MS_WORKAROUND  for ccpu00123904*/
   datReq->isTApres = FALSE;
#ifdef TFU_ALLOC_EVENT_NO_INIT
   datReq->mBuf[0] = 0;
   datReq->mBuf[1] = 0;
#endif
 
   for(i=0;i<RG_MAX_TB_PER_UE;i++)
   {
#ifndef L2_OPTMZ
      if ((hqP->tbInfo[i].sf == dlSf) && (hqP->tbInfo[i].tb != NULLP))
#else
      if ((hqP->tbInfo[i].sf == dlSf) && RgUtlIsTbMuxed(&(hqP->tbInfo[i].tb)))
#endif
      {

         datReq->rnti           =  hqP->tbInfo[i].pdcch.rnti;
         datReq->dciInfo        =  hqP->tbInfo[i].pdcch.dci;
         datReq->doa            =  hqP->tbInfo[i].doa;
         datReq->transMode      =  hqP->tbInfo[i].txMode;
         datReq->puschRptUsd    =  hqP->tbInfo[i].puschRptUsd;
         datReq->puschPmiInfo   =  hqP->tbInfo[i].puschPmiInfo;
         /*MS_WORKAROUND  for ccpu00123904*/
         if (hqP->tbInfo[i].schdTa.pres)
         {
            datReq->isTApres       =  TRUE; 
         }
#ifdef   TFU_UPGRADE
         /* update pA value */
         datReq->pa             =  hqP->tbInfo[i].pa;
#endif
         /* LTE_ADV_FLAG_REMOVED_START */
         datReq->isEnbSFR       =  hqP->tbInfo[i].isEnbSFR;
         /* LTE_ADV_FLAG_REMOVED_END */
#ifndef L2_OPTMZ
#ifndef LTE_ENB_PAL
         /* datReq->mBuf[i] = hqP->tbInfo[i].tb; */
         /* Intel Tdd- Commenting out the Optimization for direct Access of 
          * mBuf Index */
         /*Proper clean-up needed as this needs long stability tests
          * in all SoCs*/
#if defined(TENB_T2K3K_SPECIFIC_CHANGES) && defined(LTE_TDD)
         SIncMsgRef(hqP->tbInfo[i].tb, RG_GET_MEM_REGION(rgCb),
               RG_GET_MEM_POOL(rgCb), &datReq->mBuf[i]); 
#else
         datReq->mBuf[i] = hqP->tbInfo[i].tb;
#endif
#else
         SIncMsgRef(hqP->tbInfo[i].tb, RG_GET_MEM_REGION(rgCb),
               RG_GET_MEM_POOL(rgCb), &datReq->mBuf[i]);
#endif
         {
            MsgLen   dbgBufLen;
            if(SFndLenMsg(datReq->mBuf[i], &dbgBufLen))
            {
               if(dbgBufLen == 0)
               {              
                  RLOG_ARG4(L_ERROR,DBG_CELLID,cellCb->cellId,
			                  "RNTI:%d SFN:%d subframe:%d tbIdx:%d Sdu Length 0 ",
                    	      datReq->rnti,
            			      hqP->tbInfo[i].timingInfo.sfn,
			                  hqP->tbInfo[i].timingInfo.subframe,i);
                  RLOG_ARG3(L_ERROR,DBG_CELLID,cellCb->cellId,
                   	      "taPres [%d] numOfTbs [%d] format[%d]",
                   	      datReq->isTApres, 
                           hqP->numOfTBs, 
                           datReq->dciInfo.format);  
               }              
            }            
         }
#else
         /* L2 optimization for mUe/Tti: Removing SIncMsgRef to avoid additional
          * mBuf allocation. MAC header, MAC Ces, MAC PDU per LCH per TB Per UE
          * and MAC padding Mbufs are being sent to CL. Populating these Ptrs
          * From TB Info to TfuDatReq
          */
         datReq->tbInfo[i].tbPres =  TRUE;
         datReq->tbInfo[i].tbSize =  hqP->tbInfo[i].tbSz;
         datReq->tbInfo[i].macHdr =  hqP->tbInfo[i].tb.macHdr;
         datReq->tbInfo[i].macCes =  hqP->tbInfo[i].tb.macCes;
         datReq->tbInfo[i].numLch =  hqP->tbInfo[i].tb.numLch;
#if 0 
#else
//printf("\n MacHdr in TfuBldReq");
//SPrntMsg(datReq->tbInfo[i].macHdr, 0, 0);
#endif
         for(lchIdx = 0; lchIdx < hqP->tbInfo[i].tb.numLch; lchIdx++)
         {
            datReq->tbInfo[i].lchInfo[lchIdx].numPdu = hqP->tbInfo[i].tb.\
                                                       lchInfo[lchIdx].numPdu;
            for(pduIdx = 0; pduIdx < hqP->tbInfo[i].tb.lchInfo[lchIdx].numPdu;\
                  pduIdx++)
            {
               datReq->tbInfo[i].lchInfo[lchIdx].mBuf[pduIdx] =
                  hqP->tbInfo[i].tb.lchInfo[lchIdx].mBuf[pduIdx];
              // printf("\nTfuDatReq:padSize=%ld rnti=%d  numLch=%ld numPdu=%ld tbSz=%d, tbPres=%d\n",hqP->tbInfo[i].tb.padSize,datReq->rnti, datReq->tbInfo[i].numLch, datReq->tbInfo[i].lchInfo[lchIdx].numPdu, datReq->tbInfo[i].tbSize,  datReq->tbInfo[i].tbPres);
            }
         }
        // datReq->tbInfo[i].macPad  =  hqP->tbInfo[i].tb.macPad;
         datReq->tbInfo[i].padSize =  hqP->tbInfo[i].tb.padSize;
        // prc_trace_format_string(0x40,3,"TfuDatReq:RNTI=%d TbIdx=%d TbSize=%d PdSz=(%d) macHdraddr: (%p) macCEAddr: (%p) noLch=(%d)",datReq->rnti, i,
          //     hqP->tbInfo[i].tbSz, datReq->tbInfo[i].padSize, datReq->tbInfo[i].macHdr, datReq->tbInfo[i].macCes, datReq->tbInfo[i].numLch);

#endif
         datReq->nmbOfTBs++;
      }
   }
   RETVOID;
}  /* rgDHMBldTfuDatReq */


#ifdef L2_OPTMZ
/**
 * @brief This function releases a HARQ process
 *
 * @details
 *
 *     Function: rgDHMFreeHqProcTB
 *     Purpose:  This function returns a HARQ process to HARQ Entity 
 *               in the DL direction.
 *     
 *               1. Add the HARQ process to the free queue.
 *     Invoked by: scheduler and HARQ processing
 *     
 *  @param[in]  RgDlHqProc    *hqP
 *  @return  Void      
 *
 **/
#ifdef ANSI
PUBLIC S16 rgDHMFreeHqProcTB
(
RgDlHqProcCb         *hqP,
U8                   tbIndex
)
#else
PUBLIC S16 rgDHMFreeHqProcTB(hqP, tbIndex)
RgDlHqProcCb         *hqP;
U8                   tbIndex;
#endif
{
   RgTfuDatReqTbInfo     *tb;   /* TB to be sent to CL/PHY*/

   TRC2(rgDHMRlsHqProcTB)

   if((tbIndex > RG_MAX_TB_PER_UE) ||
      (tbIndex == 0))
   {
      RETVALUE(RFAILED);
   }

   tb = &(hqP->tbInfo[tbIndex-1].tb);
   RG_FREE_MSG(tb->macHdr);
   RG_FREE_MSG(tb->macCes);
   
   RETVALUE(ROK);
}
#endif


/**
 * @brief Handler for freeing up the harq related information from ueCb
 *
 * @details
 *
 *     Function : rgDHMFreeUe
 *     
 *     This function shall free up the HARQ specific information from ueCb.
 *           
 *  @param[in]  RgDlHqEnt     *hqE 
 *
 *  @return     None.
 *
 **/
#ifdef ANSI
PUBLIC Void rgDHMFreeUe
(
RgDlHqEnt          *hqE
)
#else
PUBLIC Void rgDHMFreeUe(hqE)
RgDlHqEnt          *hqE;
#endif
{
   U8             i;

   TRC2(rgDHMFreeUe)

   if(hqE->procs)
   {
      /* Free all the memory associated with HARQ */
      for (i=0; i < hqE->numHqProcs; i++)
      {
#ifndef L2_OPTMZ
         rgDHMRlsHqProcTB(hqE->procs[i], 1);
         rgDHMRlsHqProcTB(hqE->procs[i], 2);
#else
         rgDHMFreeHqProcTB(hqE->procs[i], 1);
         rgDHMFreeHqProcTB(hqE->procs[i], 2);
#endif
         rgFreeSBuf((Data **)&(hqE->procs[i]), sizeof(RgDlHqProcCb));
      }

      /*ccpu00117052 - MOD - Passing double pointer for proper NULLP
                            assignment */
   }

   RETVOID;

}  /* rgDHMFreeUe */

/**
 * @brief Function for handling RaResp request received from scheduler to MAC
 *
 * @details
 *
 *     Function : RgSchMacRlsHqReq
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
PUBLIC S16 RgSchMacRlsHqReq
(
Pst                 *pst,
RgInfRlsHqInfo      *rlshqUeInfo
)
#else
PUBLIC S16 RgSchMacRlsHqReq(pst, rlshqUeInfo)
Pst                 *pst;
RgInfRlsHqInfo      *rlshqUeInfo;
#endif
{
   RgCellCb       *cell;
   RgUeCb         *ue;
   RgDlHqProcCb   *hqP;
   U8             idx1,idx2;
#ifdef LTE_L2_MEAS
   SuId              suId;
   U8                tbId;
   RguHarqStatusInd  hqStaInd;
   Bool              isValidTbId = FALSE;
#endif
   U32        startTime=0;
   
   TRC2(RgSchMacRlsHqReq)

   /*starting Task*/
   SStartTask(&startTime, PID_MAC_AM_HARQ_RLS);

   if(NULLP == rlshqUeInfo)
   {
      RETVALUE(RFAILED);
   }

   if((cell = rgDBMGetCellCb(rlshqUeInfo->cellId)) == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,rlshqUeInfo->cellId,
                "No cellCb found with cellId");
      RETVALUE(RFAILED);
   }

   if(NULLP == rlshqUeInfo->ueHqInfo)
   {
      RETVALUE(RFAILED);
   }

   for(idx1 = 0; idx1 < rlshqUeInfo->numUes; idx1++)
   {
      if((ue=rgDBMGetUeCb (cell, rlshqUeInfo->ueHqInfo[idx1].rnti)) == NULLP)
      {
         /* Check in RachLst */
         if((ue=rgDBMGetUeCbFromRachLst (cell, 
                     rlshqUeInfo->ueHqInfo[idx1].rnti)) == NULLP)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,rlshqUeInfo->cellId, "CRNTI:%d No ueCb found",
                     rlshqUeInfo->ueHqInfo[idx1].rnti);
            continue;
         }
      }

      rgDHMGetHqProcFrmId(ue,rlshqUeInfo->ueHqInfo[idx1].hqProcId,&hqP);
      if(rlshqUeInfo->ueHqInfo[idx1].status[0] != TRUE)
      {
         rgCb.genSts.numHarqFail++;
      }
     
#ifdef LTE_L2_MEAS
      hqStaInd.cellId = cell->cellId;
      hqStaInd.ueId = rlshqUeInfo->ueHqInfo[idx1].rnti;
      hqStaInd.numTbs = rlshqUeInfo->ueHqInfo[idx1].numOfTBs;
#endif

      for(idx2=0; idx2 < rlshqUeInfo->ueHqInfo[idx1].numOfTBs; idx2++)
      {
#ifdef LTE_L2_MEAS
         /* Fill the hq sta Ind stucture. Need to send the Status Ind for only
          those TBID's reported by Scheduler*/
            tbId = rlshqUeInfo->ueHqInfo[idx1].tbId[idx2];
            if (hqP->tbId[tbId-1] != RGU_INVALID_TBID)
            {
            /* Fill the correct Sn Map corresponding to the TBID */
            hqStaInd.tbId[idx2] = hqP->tbId[tbId-1];
            hqStaInd.status[idx2] = rlshqUeInfo->ueHqInfo[idx1].status[idx2];
               isValidTbId = TRUE;
            }
#endif
         if(rgDHMRlsHqProcTB(hqP, 
               rlshqUeInfo->ueHqInfo[idx1].tbId[idx2]) != ROK)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,rlshqUeInfo->cellId,
                  "CRNTI:%d Failure in releasing hq TB",
                  rlshqUeInfo->ueHqInfo[idx1].rnti);
            continue;
         }
      }

#ifdef LTE_L2_MEAS

         suId = rgRguDlSap;
         if (isValidTbId)
         {
            RgUiRguHqStaInd(&rgCb.rguSap[suId].sapCfg.sapPst, rgCb.rguSap[suId].sapCfg.suId,
                       &hqStaInd);
         }
#endif
   } /* end of ues loop */

   /*starting Task*/
   SStopTask(startTime,PID_MAC_AM_HARQ_RLS);

   RETVALUE(ROK);
} /* end of RgSchMacRlsHqReq */
/**********************************************************************
 
         End of file:     gk_dhm.c@@/main/3 - Sat Jul 30 02:21:26 2011
 
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
/main/1      ---     ps              1. Initial Release.
/main/2      ---     ps              1. LTE MAC 2.1 release
             rg001.201 sm 1. Removed extra comments. 
             rg003.201 sm 1. changes for CR ccpu00108099. 
             rg004.201 sm 1. ABR fixed
             rg005.201 sm 1. Changes for optimization.
             rg007.201   ap  1. Added support for MIMO
             rg008.201 sd  1. Removed dependncy on MIMO compile-time flag
             rg009.201 rsharon 1.[ccpu00112372] Changes for CR timer implementation.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1         
      rg001.301   nudupi   1. Changes for the TB indexing issue.
                           2. Added the changes corresponding to the CR ccpu00119023
      rg002.301  asehgal   1. ccpu00120298 ADD sf is reset to NULLP
$SID$        ---    rt     1. LTE MAC 4.1 release
*********************************************************************91*/
