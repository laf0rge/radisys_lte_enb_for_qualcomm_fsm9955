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
  
     File:     rg_utl.c
  
     Sid:      gk_utl.c@@/main/3 - Sat Jul 30 02:21:57 2011
  
     Prg:      st
  
**********************************************************************/

/** @file rg_utl.c
@brief This file implements utility functions for LTE MAC
*/

static const char* RLOG_MODULE_NAME="MAC";
static int RLOG_MODULE_ID=4096;
static int RLOG_FILE_ID=179;

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* Common Token Defines */
#include "cm_llist.h"      /* Common Link List Defines */
#include "cm_hash.h"       /* Common Hash List Defines */
#include "cm_mblk.h"       /* common memory link list library */
#include "cm_lte.h"        /* Common LTE */

#include "rg_env.h"        /* MAC Environment Defines */
#include "crg.h"           /* CRG Interface defines */
#include "rgu.h"           /* RGU Interface defines */
#include "tfu.h"           /* TFU Interface defines */
#include "rg_sch_inf.h"           /* RGR Interface defines */
#include "lrg.h"           /* LRG Interface defines */

#include "rg.h"            /* MAC defines */
#include "rg_err.h"        /* MAC error defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* Timer */
#include "cm_tkns.x"       /* Common Token Definitions */
#include "cm_llist.x"      /* Common Link List Definitions */
#include "cm_lib.x"        /* Common Library Definitions */
#include "cm_hash.x"       /* Common Hash List Definitions */
#include "cm_mblk.x"       /* common memory link list library */
#include "cm_lte.x"        /* Common LTE */

#include "crg.x"           /* CRG Interface includes */
#include "rgu.x"           /* RGU Interface includes */
#include "tfu.x"           /* TFU Interface includes */
#include "rg_sch_inf.x"           /* RGR Interface includes */
#include "lrg.x"           /* LRG Interface includes */

#include "rg.x"            /* MAC includes */

/* local defines */
#define RG_NON_MIMO_IDX 0

/* local typedefs */
 
/* local externs */
 
/* forward references */
PRIVATE S16 rgUtlHndlCrntiChng ARGS((
RgCellCb        *cell,
CmLteRnti       rnti,
CmLteRnti       newRnti
));
PRIVATE Void rgUtlHndlCrntiRls ARGS((
RgCellCb        *cell,
CmLteRnti       rnti
));


/***********************************************************
 *
 *     Func : rgAllocShrablSBuf
 *
 *     Desc : Utility Function to Allocate static buffer which is 
 *            sharable among different layers if YYYY flag is enabled. 
 *            else it allocates from normal static region
 *            Memory allocated is assumed contiguous.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: Caller doesnt need to raise the alarm in case of memory
 *            allocation gets failed. 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgAllocShrablSBuf
(
Data    **pData,            /* Pointer of the data to be returned */
Size    size                /* size */
)
#else
PUBLIC S16 rgAllocShrablSBuf(pData, size)
Data    **pData;            /* Pointer of the data to be returned */
Size    size;               /* size */
#endif
{
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   TRC2(rgAllocShrablSBuf)

   /* Initialize the param to NULLP */
   *pData = NULLP;

   if (size == 0)
   {
      RETVALUE(RFAILED);
   }

   /* allocate buffer */
   if (SGetStaticBuffer(rgCb.rgInit.region, rgCb.rgInit.pool, pData, size, 0) != ROK)
   {
     dgn.type = LRG_USTA_DGNVAL_MEM;
     dgn.u.mem.region = rgCb.rgInit.region;
     dgn.u.mem.pool = rgCb.rgInit.pool;
     /*  Send an alarm to Layer Manager */
     rgLMMStaInd(LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      RGLOGERROR(ERRCLS_DEBUG, ERG028, 0, "Unable to Allocate Buffer");
      RETVALUE(RFAILED);
   }


   /* zero out the allocated memory */
   cmMemset((U8 *)*pData, 0x00, size);

   RETVALUE(ROK);

} /* end of rgAllocSBuf */
PRIVATE S16 rgUtlHndlCrntiChng ARGS((
RgCellCb        *cell,
CmLteRnti       rnti,
CmLteRnti       newRnti
));
PRIVATE Void rgUtlHndlCrntiRls ARGS((
RgCellCb        *cell,
CmLteRnti       rnti
));

/***********************************************************
 *
 *     Func : rgAllocSBuf
 *
 *     Desc : Utility Function to Allocate static buffer. 
 *            Memory allocated is assumed contiguous.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: Caller doesnt need to raise the alarm in case of memory
 *            allocation gets failed. 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgAllocSBuf
(
Data    **pData,            /* Pointer of the data to be returned */
Size    size                /* size */
)
#else
PUBLIC S16 rgAllocSBuf(pData, size)
Data    **pData;            /* Pointer of the data to be returned */
Size    size;               /* size */
#endif
{
   RgUstaDgn dgn;      /* Alarm diagnostics structure */

   TRC2(rgAllocSBuf)

   /* Initialize the param to NULLP */
   *pData = NULLP;

   if (size == 0)
   {
      RETVALUE(RFAILED);
   }

   /* allocate buffer */
#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
   if (SGetSBuf(rgCb.rgInit.region, rgCb.rgInit.pool, pData, size) != ROK)
   {
     dgn.type = LRG_USTA_DGNVAL_MEM;
     dgn.u.mem.region = rgCb.rgInit.region;
     dgn.u.mem.pool = rgCb.rgInit.pool;
     /*  Send an alarm to Layer Manager */
     rgLMMStaInd(LCM_CATEGORY_RESOURCE, LCM_EVENT_SMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      RGLOGERROR(ERRCLS_DEBUG, ERG028, 0, "Unable to Allocate Buffer");
      RETVALUE(RFAILED);
   }


   /* zero out the allocated memory */
   cmMemset((U8 *)*pData, 0x00, size);

   RETVALUE(ROK);

} /* end of rgAllocSBuf */

/*
*
*       Fun:   rgFreeSharableSBuf
*
*       Desc:  The argument to rgFreeSBuf() is a pointer to a block
*              previously allocated by rgAllocSBuf() and size. It 
*              deallocates the memory. 
*
*       Ret:   RETVOID
*
*       Notes: ccpu00117052 - MOD- changed the Data parameter from 
*                             pointer to address of pointer so that
*                             the freed memory could be set to NULLP
*
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC Void rgFreeSharableSBuf
(
Data **data,         /* address of pointer to data */
Size size            /* size */
)
#else
PUBLIC Void rgFreeSharableSBuf(data, size)
Data **data;         /* address of pointer to data */
Size size;          /* size */
#endif
{

   S16 ret;

   TRC2(rgFreeSharableBuf)

   if ((data == NULLP) || (*data == NULLP) || (size == 0))
   {
      RETVOID;
   }


   /* Deallocate buffer */
   ret = SPutStaticBuffer(rgCb.rgInit.region, rgCb.rgInit.pool, *data, size, SS_SHARABLE_MEMORY);

   if (ret != ROK)
   {
      RGLOGERROR(ERRCLS_DEBUG, ERG029, (ErrVal) 0, "rgFreeSBuf failed.\n");
      RETVOID;
   }

   *data = NULLP;

   RETVOID;

} /* end of rgFreeSharableBuf */



/*
*
*       Fun:   rgFreeSBuf
*
*       Desc:  The argument to rgFreeSBuf() is a pointer to a block
*              previously allocated by rgAllocSBuf() and size. It 
*              deallocates the memory. 
*
*       Ret:   RETVOID
*
*       Notes: ccpu00117052 - MOD- changed the Data parameter from 
*                             pointer to address of pointer so that
*                             the freed memory could be set to NULLP
*
*       File:  rg_utl.c
*/
#ifdef ANSI
PUBLIC Void rgFreeSBuf
(
Data **data,         /* address of pointer to data */
Size size            /* size */
)
#else
PUBLIC Void rgFreeSBuf(data, size)
Data **data;         /* address of pointer to data */
Size size;          /* size */
#endif
{

   S16 ret;

   TRC2(rgFreeSBuf)

   if ((data == NULLP) || (*data == NULLP) || (size == 0))
   {
      RETVOID;
   }


#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_CALLER();
#endif /* */
   /* Deallocate buffer */
   ret = SPutSBuf(rgCb.rgInit.region, rgCb.rgInit.pool, *data, size);

   if (ret != ROK)
   {
      RGLOGERROR(ERRCLS_DEBUG, ERG029, (ErrVal) 0, "rgFreeSBuf failed.\n");
      RETVOID;
   }

   *data = NULLP;

   RETVOID;

} /* end of rgFreeSBuf */


/***********************************************************
 *
 *     Func : rgGetMsg
 *
 *     Desc : Utility Function to Allocate message buffer. 
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: Caller doesnt need to raise the alarm in case of memory
 *            allocation gets failed. 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 rgGetMsg
(
Buffer    **mBuf            /* Message Buffer pointer be returned */
)
#else
PUBLIC S16 rgGetMsg(mBuf)
Buffer    **mBuf;           /* Message Buffer pointer be returned */
#endif
{
   S16         ret;

   TRC2(rgGetMsg)

#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
   ret = SGetMsg(RG_GET_MEM_REGION(rgCb), RG_GET_MEM_POOL(rgCb), mBuf);

   if (ROK != ret) 
   {
      /* Moving diagnostics structure to limited scope for optimization */
      RgUstaDgn   dgn;      /* Alarm diagnostics structure */

      rgFillDgnParams(&dgn, LRG_USTA_DGNVAL_MEM);

      /*  Send an alarm to Layer Manager */
      rgLMMStaInd(LCM_CATEGORY_RESOURCE, LCM_EVENT_DMEM_ALLOC_FAIL,
                                       LCM_CAUSE_MEM_ALLOC_FAIL, &dgn);
      RGLOGERROR(ERRCLS_DEBUG, ERG030, 0, "Unable to Allocate Buffer");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* end of rgGetMsg */


/***********************************************************
 *
 *     Func : rgFillDgnParams
 *
 *     Desc : Utility Function to Fill Diagonostic params. 
 *
 *     Ret  : None.
 *
 *     Notes: None.
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgFillDgnParams
(
RgUstaDgn   *dgn,
U8          dgnType
)
#else
PUBLIC Void rgFillDgnParams(dgn, dgnType)
RgUstaDgn   *dgn;
U8          dgnType;
#endif
{

   TRC2(rgFillDgnParams)

   switch(dgnType)
   {
      case LRG_USTA_DGNVAL_MEM:
         dgn->type = (U8) LRG_USTA_DGNVAL_MEM;
         dgn->u.mem.region  = rgCb.rgInit.region;
         dgn->u.mem.pool    = rgCb.rgInit.pool;
      break;

      default:
      break;
   }

   RETVOID;
} /* end of rgFillDgnParams */


/***********************************************************
 *
 *     Func : rgUpdtRguDedSts
 *
 *     Desc : Utility Function to update rgu sap statistics for dedicated
 *            DatReqs.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgUpdtRguDedSts
(
U8             stsType,   /* Statistics type to update */
RgRguDedDatReq *datReq    /* DatReq pointer */
)
#else
PUBLIC Void rgUpdtRguDedSts(stsType, datReq)
U8             stsType;   /* Statistics type to update */
RgRguDedDatReq *datReq;   /* DatReq pointer */
#endif
{
   U8 idx1,idx2;
   SuId  suId;
   U32 idx;


   TRC2(rgUpdtRguDedSts)

  suId = rgRguDlSap;

   switch(stsType)
   {
      case RG_RGU_SDU_RCVD:
      for(idx = 0; idx < datReq->nmbOfUeGrantPerTti; idx++)
      {
         RguDDatReqPerUe *datReqPerUe = &datReq->datReq[idx];
         for (idx1 = 0; idx1 < datReqPerUe->nmbOfTbs; idx1++) 
         {
            for(idx2 = 0; idx2 < datReqPerUe->datReqTb[idx1].nmbLch; idx2++) 
            {
               rgCb.rguSap[suId].sapSts.numPduRcvd +=
                  datReqPerUe->datReqTb[idx1].lchData[idx2].pdu.numPdu;
            }
         }
      }

         break;
      case RG_RGU_SDU_DROP:
      for(idx = 0; idx < datReq->nmbOfUeGrantPerTti; idx++)
      {
         RguDDatReqPerUe *datReqPerUe = &datReq->datReq[idx];
         for (idx1 = 0; idx1 < datReqPerUe->nmbOfTbs; idx1++) 
         {
            for(idx2 = 0; idx2 < datReqPerUe->datReqTb[idx1].nmbLch; idx2++) 
            {
               rgCb.rguSap[suId].sapSts.numPduRcvd +=
                  datReqPerUe->datReqTb[idx1].lchData[idx2].pdu.numPdu;
               rgCb.rguSap[suId].sapSts.numPduDrop +=
                  datReqPerUe->datReqTb[idx1].lchData[idx2].pdu.numPdu;
            }
         }
      }

         break;
   }
   
   RETVOID;
} /* rgUpdtRguDedSts */


/***********************************************************
 *
 *     Func : rgUpdtRguCmnSts
 *
 *     Desc : Utility Function to update rgu sap statistics for common
 *            DatReqs.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: 
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgUpdtRguCmnSts
(
U8             stsType   /* Statistics type to update */
)
#else
PUBLIC Void rgUpdtRguCmnSts(stsType)
U8             stsType;   /* Statistics type to update */
#endif
{
   SuId suId;
   TRC2(rgUpdtRguCmnSts)


  suId = rgRguDlSap;

   switch(stsType)
   {
      case RG_RGU_SDU_RCVD:
         rgCb.rguSap[suId].sapSts.numPduRcvd ++;
         break;
      case RG_RGU_SDU_DROP:
         rgCb.rguSap[suId].sapSts.numPduRcvd ++;
         rgCb.rguSap[suId].sapSts.numPduDrop ++;
         break;
   }
   
   RETVOID;
} /* rgUpdtRguCmnSts */


/***********************************************************
 *
 *     Func : rgUpdtCellCnt
 *
 *     Desc : Utility Function to update cell count. It gives number of active
 *     cells
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called only after cell is added/deleted 
 *     from the globlal hashlist
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgUpdtCellCnt
(
U8 updtType
)
#else
PUBLIC Void rgUpdtCellCnt(updtType)
U8 updtType;
#endif
{
   TRC2(rgUpdtCellCnt);

   switch (updtType)
   {
      case RG_CFG_ADD:
         rgCb.genSts.numCellCfg++;
         break;
      case RG_CFG_DEL:
         rgCb.genSts.numCellCfg--;
         break;
      default:
         break;
   }

   RETVOID;
} /* rgUpdtCellCnt */


/***********************************************************
 *
 *     Func : rgUpdtUeCnt
 *
 *     Desc : Utility Function to update ue count. It gives number of active
 *     Ues.
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called only after ue is added/deleted 
 *     from the globlal hashlist
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgUpdtUeCnt
(
U8 updtType
)
#else
PUBLIC Void rgUpdtUeCnt (updtType)
U8 updtType;
#endif
{
   TRC2(rgUpdtUeCnt);

   switch (updtType)
   {
      case RG_CFG_ADD:
         rgCb.genSts.numUeCfg++;
         break;
      case RG_CFG_DEL:
         rgCb.genSts.numUeCfg--;
         break;
      default:
         break;
   }
   RETVOID;
} /* rgUpdtUeCnt */

/*
*
*       Fun:   rgAllocEventMem
*
*       Desc:  This function allocates event memory 
*
*       Ret:   ROK      - on success
*              RFAILED  - on failure
*
*       Notes: None
*
*       File:  rg_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 rgAllocEventMem
(
Ptr       *memPtr,
Size      memSize
)
#else
PUBLIC S16 rgAllocEventMem(memPtr, memSize)
Ptr       *memPtr;
Size      memSize;
#endif
{
   Mem              sMem;
   VOLATILE U32     startTime=0;

   TRC2(rgAllocEventMem)

   sMem.region = rgCb.rgInit.region;
   sMem.pool = rgCb.rgInit.pool;

#if (ERRCLASS & ERRCLS_DEBUG)
   if (memSize<= 0)
   {
      RGLOGERROR(ERRCLS_INT_PAR, ERG031, memSize,
                   "rgAllocEventMem(): memSize invalid\n");
      RETVALUE (RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */


   /*starting Task*/
   SStartTask(&startTime, PID_MACUTL_CMALLCEVT);

#ifdef MS_MBUF_CORRUPTION /* Should be enabled when debugging mbuf corruption */
   MS_BUF_ADD_ALLOC_CALLER();
#endif /* */
#ifdef TFU_ALLOC_EVENT_NO_INIT
   if(ROK != cmAllocEvntNoInit(memSize, TFU_MAX_MEMBLK_SIZE, &sMem, memPtr))
#else
   if(ROK != cmAllocEvnt(memSize, TFU_MAX_MEMBLK_SIZE, &sMem, memPtr))
#endif /* */
   {
      RLOG0(L_ERROR,"cmAllocEvnt Failed"); 
      RETVALUE(RFAILED);
   }

   /*stoping Task*/
   SStopTask(startTime, PID_MACUTL_CMALLCEVT);

   RETVALUE(ROK);
} /* end of rgAllocEventMem*/

/*
*
*       Fun:   rgGetEventMem
*
*       Desc:  This function allocates event memory 
*
*       Ret:   ROK      - on success
*              RFAILED  - on failure
*
*       Notes: None
*
*       File:  rg_utl.c
*
*/
#ifdef ANSI
PUBLIC S16 rgGetEventMem
(
Ptr       *ptr,
Size      len,
Ptr       memCp
)
#else
PUBLIC S16 rgGetEventMem(ptr, len, memCp)
Ptr       *ptr;
Size      len;
Ptr       memCp;
#endif
{
   S16   ret;

   TRC2(rgGetEventMem)
#ifdef TFU_ALLOC_EVENT_NO_INIT
   ret = cmGetMemNoInit(memCp, len, (Ptr *)ptr);
#else
   ret = cmGetMem(memCp, len, (Ptr *)ptr);
#endif /* */
   RETVALUE(ret);
} /* end of rgGetEventMem*/

/***********************************************************
 *
 *     Func : rgGetPstToInst
 *
 *     Desc : Utility Function to get the pst structure to post a message to
 *     scheduler instance
 *            
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called while sending a msg from 
 *     MAC to a scheduler instance
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC Void rgGetPstToInst
(
Pst           *pst,
Inst          schInst
)
#else
PUBLIC Void rgGetPstToInst (pst, schInst)
Pst           *pst;
Inst          schInst;
#endif
{
   TRC2(rgGetPstToInst);

   pst->srcEnt = rgCb.rgInit.ent; 
   pst->srcInst = rgCb.rgInit.inst;
   pst->srcProcId = rgCb.rgInit.procId;
   pst->region = rgCb.rgInit.region;
   pst->pool = rgCb.rgInit.pool;

   pst->dstProcId = rgCb.rgInit.procId;
   pst->dstEnt = rgCb.rgInit.ent; 
   pst->dstInst = schInst;
   pst->selector = 0;
   pst->prior     = PRIOR0;
   pst->intfVer   = 0;
   pst->route   = RTESPEC;

   RETVOID; 
} /* end of rgGetPstToInst */

/***********************************************************
 *
 *     Func : RgSchMacLcgRegReq
 *
 *     Desc : Utility Function to register the set of GBR LCG.
 *        Invoked at the time of LCG configuration/Reconfiguration at Schedular.
 * 
 *     Processing Steps: 
 *           - Fetch the ueCb using the crnti given in lcInfo            
 *           - Store the if LCG is GBR or not.
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called at the time of LCG 
 *     configuration/Reconfiguration at Schedular. 
 *     
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 RgSchMacLcgRegReq
(
Pst            *pst,
RgInfLcgRegReq *lcgRegReq
)
#else
PUBLIC S16 RgSchMacLcgRegReq (pst, lcgRegReq)
Pst            *pst;
RgInfLcgRegReq *lcgRegReq;
#endif
{
   RgCellCb   *cell;
   RgUeCb     *ue;

   TRC2(RgSchMacLcgRegReq);

   /* Fetch the cell and then the UE */
   if ((cell = rgDBMGetCellCb(lcgRegReq->cellId)) == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,lcgRegReq->cellId,"Cell does not exist ");
      RETVALUE(RFAILED);
   }

   if ((ue = rgDBMGetUeCb(cell, lcgRegReq->crnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,cell->cellId,"CRNTI:%d does not exist", 
		         lcgRegReq->crnti);
      RETVALUE(RFAILED);
   }
   ue->ul.lcgArr[lcgRegReq->lcgId].isGbr = lcgRegReq->isGbr;

   RETVALUE(ROK); 
} /* end of RgSchMacLcgRegReq */

#ifdef LTEMAC_SPS

/***********************************************************
 *
 *     Func : RgSchMacUlSpsResetReq
 *
 *     Desc : Utility Function to reset SPS params for a UE
 * 
 *     Processing Steps: 
 *           - Fetch the ueCb using the crnti 
 *           - reset implRelCnt and explRelCnt
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 RgSchMacUlSpsResetReq
(
Pst            *pst,
RgInfUlSpsReset *ulSpsResetInfo
)
#else
PUBLIC S16 RgSchMacUlSpsResetReq (pst, lcInfo)
Pst            *pst;
RgInfUlSpsReset *ulSpsResetInfo;
#endif
{
   RgCellCb   *cell;
   RgUeCb     *ue;

   TRC2(RgSchMacUlSpsResetReq);

   /* Fetch the cell and then the UE */
   if ((cell = rgDBMGetCellCb(ulSpsResetInfo->cellId)) == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,ulSpsResetInfo->cellId,"Cell does not exist ");
      RETVALUE(RFAILED);
   }

   if ((ue = rgDBMGetUeCb(cell, ulSpsResetInfo->crnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,cell->cellId,"CRNTI:%d does not exist", 
		      ulSpsResetInfo->crnti);
      RETVALUE(RFAILED);
   }

   ue->ul.implRelCntr = 0;
   ue->ul.explRelCntr = 0;

   RETVALUE(ROK); 
} /* end of RgSchMacUlSpsResetReq */




/***********************************************************
 *
 *     Func : RgSchMacSpsLcRegReq
 *
 *     Desc : Utility Function to register the set of uplink SPS logical
 *        channels for a SPS UE.
 *        Invoked at the time of activation of a UE for UL-SPS.
 *        Whenever there is data on these LCs MAC shall inform scheduler
 * 
 *     Processing Steps: 
 *           - Fetch the ueCb using the crnti given in lcInfo            
 *           - Store the sps-rnti and set the bits corresponding to the
 *             logical channel ids in ueUlCb->spsLcMask.
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called at the time UL SPS is activated 
 *     for a UE at scheduler
 *     
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 RgSchMacSpsLcRegReq
(
Pst            *pst,
RgInfSpsLcInfo *lcInfo
)
#else
PUBLIC S16 RgSchMacSpsLcRegReq (pst, lcInfo)
Pst            *pst;
RgInfSpsLcInfo *lcInfo;
#endif
{
   RgCellCb   *cell;
   RgUeCb     *ue;
   U8         idx;

   TRC2(RgSchMacSpsLcRegReq);

   /* Fetch the cell and then the UE */
   if ((cell = rgDBMGetCellCb(lcInfo->cellId)) == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,lcInfo->cellId, "Cell does not exist ");
      RETVALUE(RFAILED);
   }

   if ((ue = rgDBMGetUeCb(cell, lcInfo->crnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,cell->cellId,"CRNTI:%d does not exist", 
		      lcInfo->crnti);
      RETVALUE(RFAILED);
   }

   /* Store the sps-rnti and SPS LC information in the UE */ 
   ue->spsRnti = lcInfo->spsRnti;
   for (idx=0; idx < lcInfo->spsLcCnt; idx++)
   {
      /* KWORK_FIX: Modified the index from lcId to lcId-1 for handling lcId 10 properly */
      ue->ul.spsLcId[(lcInfo->spsLcId[idx])-1] = TRUE;
   }
   ue->ul.implRelCnt = lcInfo->implRelCnt;
   ue->ul.explRelCnt = ue->ul.implRelCnt + 1; /*(lcInfo->implRelCnt * lcInfo->spsPrd);*/

   /* Insert the UE into SPS UE List */
   if (rgDBMInsSpsUeCb(cell, ue) == RFAILED)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
	            "Ue insertion into SPS list failed SPS CRNTI:%d", ue->spsRnti);
      RETVALUE(RFAILED);
   } 

   RETVALUE(ROK); 
} /* end of RgSchMacSpsLcRegReq */


/***********************************************************
 *
 *     Func : RgSchMacSpsLcDeregReq
 *
 *     Desc : Utility Function to deregister the set of uplink SPS 
 *        logical channels for a UE.
 *        Invoked at the time of release of UL-SPS for an activated UE.
 * 
 *     Processing Steps: 
 *           - Fetch the ueCb using the crnti given
 *           - Reset the bits corresponding to the logical channel ids in
 *             ueUlCb->spsLcMask.
 *
 *     Ret  : ROK
 *            RFAILED
 *
 *     Notes: This function should be called at the time UL SPS is released 
 *     for a UE at scheduler
 *     
 *
 *     File : rg_utl.c
 *
 **********************************************************/
#ifdef ANSI
PUBLIC S16 RgSchMacSpsLcDeregReq
(
Pst            *pst,
CmLteCellId    cellId,
CmLteRnti      crnti
)
#else
PUBLIC S16 RgSchMacSpsLcDeregReq (pst, cellId, crnti)
Pst            *pst;
CmLteCellId    cellId;
CmLteRnti      crnti;
#endif
{
   RgCellCb    *cell;
   RgUeCb      *ue;

   TRC2(RgSchMacSpsLcDeregReq);

   /* Fetch the cell and then the UE */
   if ((cell = rgDBMGetCellCb(cellId)) == NULLP)
   {
      RLOG_ARG0(L_ERROR,DBG_CELLID,cellId, "Cell does not exist ");
      RETVALUE(RFAILED);
   }

   if ((ue = rgDBMGetUeCb(cell, crnti)) == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,cellId,"CRNTI:%d Ue does not exist", crnti);
      RETVALUE(RFAILED);
   }

   /* No need to reset the SPS LC Ids as they will not be looked at*/

   /* Delete UE from the SPS UE List */
   rgDBMDelSpsUeCb(cell, ue);
   
   RETVALUE(ROK); 
} /* end of RgSchMacSpsLcDeregReq */

#endif /* LTEMAC_SPS */

/**
 * @brief Function for handling CRNTI change request 
 * received from scheduler to MAC.
 *
 * @details
 *
 *     Function : rgUtlHndlCrntiChng
 *     
 *        - Delete old UE from the list.
 *        - Update the new rnti and re-insert the UE in the list.
 *     
 *           
 *  @param[in] RgCellCb      *cell,
 *  @param[in] CmLteRnti     rnti,
 *  @param[in] CmLteRnti     newRnti
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED
 **/
#ifdef ANSI
PRIVATE S16 rgUtlHndlCrntiChng 
(
RgCellCb        *cell,
CmLteRnti       rnti,
CmLteRnti       newRnti
)
#else
PRIVATE S16 rgUtlHndlCrntiChng(cell, rnti, newRnti)
RgCellCb        *cell;
CmLteRnti       rnti;
CmLteRnti       newRnti;
#endif
{
   RgUeCb         *ue = NULLP;
   RgUeCb         *newUe = NULLP;

   TRC3(rgUtlHndlCrntiChng)

   ue = rgDBMGetUeCb(cell, rnti);
   newUe = rgDBMGetUeCbFromRachLst(cell, newRnti);
   if ((ue == NULLP) || (newUe == NULLP))
   {
      RLOG_ARG4(L_ERROR,DBG_CELLID,cell->cellId,
         		"RNTI:%d Failed to get UECB[%lu:%lu] or NEW RNTI:%d", 
               rnti, ((U32)ue), ((U32)newUe), newRnti);
      RETVALUE(RFAILED);
   }
   rgDBMDelUeCb(cell, ue);

   ue->ueId = newRnti;

   cmMemcpy((U8*)&(ue->contResId), (U8*)&(newUe->contResId), 
         sizeof(newUe->contResId));
   /* Fix : syed MSG4 might be RETXing need to store the
    * HARQ context. */
   rgDHMFreeUe(&ue->dl.hqEnt);
   ue->dl.hqEnt = newUe->dl.hqEnt;
       
   rgDBMInsUeCb(cell, ue);

   rgDBMDelUeCbFromRachLst(cell, newUe);
   rgFreeSBuf((Data **)&newUe, sizeof(*newUe));

   RETVALUE(ROK);
} /* end of rgUtlHndlCrntiChng */


/**
 * @brief Function for handling CRNTI Context release 
 * triggered from SCH to MAC.
 *
 * @details
 *
 *     Function : rgUtlHndlCrntiRls
 *     
 *        - Remove the UE context from MAC corresponding to rnti.
 *     
 *           
 *  @param[in] RgCellCb      *cell,
 *  @param[in] CmLteRnti     rnti
 *  @return  Void 
 **/
#ifdef ANSI
PRIVATE Void rgUtlHndlCrntiRls
(
RgCellCb        *cell,
CmLteRnti       rnti
)
#else
PRIVATE Void rgUtlHndlCrntiRls(cell, rnti)
RgCellCb        *cell;
CmLteRnti       rnti;
#endif
{
   RgUeCb         *ue = NULLP;
#ifdef LTEMAC_SPS
   RgUeCb         *spsUeCb = NULLP;
#endif

   TRC3(rgUtlHndlCrntiRls)

   if ((ue = rgDBMGetUeCb(cell, rnti)) == NULLP)
   {
      /* Check in RachLst */
      if((ue=rgDBMGetUeCbFromRachLst (cell, rnti)) != NULLP)
      {
         /* Delete Ue from the UE list */
         rgDBMDelUeCbFromRachLst(cell, ue);

         /* Free Ue */
         rgRAMFreeUeCb(ue);
      }
      else
      {
         RLOG_ARG1(L_WARNING,DBG_CELLID,cell->cellId,
                   "RNTI:%d ue not present in MAC context",rnti);
      }
   }
   else
   {
      /* Delete Ue from the UE list */
      rgDBMDelUeCb(cell, ue);
#ifdef LTEMAC_SPS
      spsUeCb = rgDBMGetSpsUeCb (cell, ue->spsRnti);
      if (spsUeCb)
      {
         rgDBMDelSpsUeCb(cell, spsUeCb);
      }
#endif

      /* Free Ue */
      rgCFGFreeUeCb(cell, ue);      
      /* MS_REMOVE : syed Check in RachLst */
      {
         if((ue=rgDBMGetUeCbFromRachLst (cell, rnti)) != NULLP)
         {
            RLOG_ARG1(L_ERROR,DBG_CELLID,cell->cellId,
	      	      	"RNTI:%d STALE UE is still present", rnti);         
         }
      }
   }

   RETVOID;
} /* end of rgUtlHndlCrntiRls */

/**
 * @brief Function for handling RaResp request received from scheduler to MAC.
 *
 * @details
 *
 *     Function : RgSchMacRlsRntiReq
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
PUBLIC S16 RgSchMacRlsRntiReq
(
Pst                 *pst,
RgInfRlsRnti        *rlsRnti
)
#else
PUBLIC S16 RgSchMacRlsRntiReq(pst, rlsRnti)
Pst                 *pst;
RgInfRlsRnti        *rlsRnti;
#endif
{
   RgCellCb       *cell;
   Pst            schPst;
   RgInfUeDelInd  ueDelInd;

   TRC3(RgSchMacRlsRntiReq)

   if(NULLP == rlsRnti)
   {
      RETVALUE(RFAILED);
   }

   if((cell = rgDBMGetCellCb(rlsRnti->cellId)) == NULLP)
   {
      RLOG_ARG1(L_ERROR,DBG_CELLID,rlsRnti->cellId,
               "No cellCb found with cellId for RNTI:%d", 
		         rlsRnti->rnti);
      RETVALUE(RFAILED);
   }
   /* Fix : syed Clearing UE context when SCH indicates to do so
    * UE DEL from CRG interface is now dummy. */
   if (rlsRnti->ueIdChng)
   {
      /* Fix : syed ueId change as part of reestablishment.
       * Now SCH to trigger this. CRG ueRecfg for ueId change 
       * is dummy */      
      if (rgUtlHndlCrntiChng(cell, rlsRnti->rnti, rlsRnti->newRnti) != ROK)      
      {
         RLOG_ARG2(L_ERROR,DBG_CELLID,rlsRnti->cellId,
	                "CRNTI change failed for RNTI:%d new RNTI:%d",
                   rlsRnti->rnti,rlsRnti->newRnti);
         RETVALUE(RFAILED);
      }
   }
   else
   {
      rgUtlHndlCrntiRls(cell, rlsRnti->rnti);
   }
   /* Fix : syed Send delete confirmation to SCH */
   ueDelInd.cellSapId  = cell->schInstMap.cellSapId;
   ueDelInd.cellId  = rlsRnti->cellId;
   ueDelInd.rnti    = rlsRnti->rnti; 
   rgGetPstToInst(&schPst, cell->schInstMap.schInst);
   RgMacSchUeDel(&schPst, &ueDelInd);

   RETVALUE(ROK);
} /* end of RgSchMacRlsRntiReq */

#ifdef L2_OPTMZ
#ifdef ANSI
PUBLIC Bool RgUtlIsTbMuxed
(
 TfuDatReqTbInfo *tb
)
#else
PUBLIC Bool RgUtlIsTbMuxed()
   TfuDatReqTbInfo *tb
#endif
{
   MsgLen len = 0;
   SFndLenMsg(tb->macHdr, &len);
   RETVALUE(len?TRUE : FALSE);
}
#endif

/**********************************************************************
 
         End of file:     gk_utl.c@@/main/3 - Sat Jul 30 02:21:57 2011
 
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
/main/1      ---     st  1. Initial Release.
/main/2      ---     st  1. LTE MAC 2.1 release
          rg003.201  ns  1. Trace added for functions.
          rg004.201  ns  1. Memory related Fix
          rg005.201  sm  1. Changes for optimization.
          rg008.201 rsharon  2. Changes for optimization.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1           
$SID$        ---    rt   1. LTE MAC 4.1 release
*********************************************************************91*/
