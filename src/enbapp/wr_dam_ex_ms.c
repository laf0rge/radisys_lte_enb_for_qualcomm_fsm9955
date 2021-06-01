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

     Name:    EnodeB Application

     Type:    C Source file

     Desc:    This file contains the functions to handle any incoming
              message (loosely coupled), from other layers.

     File:    wr_dam_ex_ms.c

     Sid:      fc_dam_ex_ms.c@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:51:33 2014

     Prg:     sps

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="DAM";
static int RLOG_MODULE_ID=16;
static int RLOG_FILE_ID=107;

/* header include files */
#include "wr.h"
#include "wr_ifm_dam.h"
#include "wr_ifm_dam_utils.h"
#include "wr_ifm_dam_utils.x"
#include "ssi.h"
#ifdef SS_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
#endif
#include "wr_smm_init.h"

#ifdef SS_RBUF
extern SsRngBufCnt rngCb;
extern U32 gulRcvdCnt;
#endif
#ifdef SS_RBUF
#ifdef ANSI
PUBLIC void wrDamBatchProc
(
Void
)
#else
PUBLIC void wrDamBatchProc()
Void;
#endif
{
   U32 mBuf = 0;
   Pst pst = {0};
   U32 n=0;
   U32 count = 0;
   U32 tempBuff[32];
   /* Fill pst */
   pst.srcProcId = WR_SRC_PROC_ID;
   pst.dstProcId = WR_DST_PROC_ID;
   pst.dstEnt = ENTPX;
   pst.dstInst = 0;
   pst.srcEnt = ENTPJ;
   pst.srcInst = 0;
   pst.prior = PRIOR0;
   pst.route = RTESPEC;
   pst.event = EVTFPPJUDATIND;
   pst.region = BC_SHRD_REG;
   pst.pool = 1;
   pst.selector = 1; /*SM_SELECTOR_LC */
   /*Void *elem = NULLP;*/
   /* MITUL tcp data stoll n =10 */
   while((n< rngCb.ulRngBuffCnt) && (SDeqSRngBuf(SS_RNG_BUF_ICPU_TO_DAM, (void *)&mBuf) == ROK))
   {
       tempBuff[n] = mBuf;
       n++;
   }
   gulRcvdCnt = n; 
   for(count = 0; count < n; count++)
   {
       cmUnpkFpPjuDatInd(PxLiPjuDatInd, &pst, (Buffer *)tempBuff[count]);
   }
}
#endif
/*
 *
 *       Fun:    wrDamActvTsk - DAM activation
 *
 *       Desc:   Function used to handle the event generated for DAM module.
 *               Will call the unpack functions which in turn calls the
 *               handlers of it.
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   wr_dam_ex_ms.c
 *
 */

#ifdef ANSI
PUBLIC S16 wrDamActvTsk
(
Pst      *pst,
Buffer   *mBuf
)
#else
PUBLIC S16 wrDamActvTsk(pst, mBuf)
Pst      *pst;
Buffer   *mBuf;
#endif /* ANSI */
{
   TRC3(wrDamActvTsk);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (pst == (Pst *)NULLP)
   {
      RLOG0(L_ERROR, "Invalid Pst");
      if (mBuf != (Buffer *)NULLP)
      {
         (Void)SPutMsg(mBuf);
      }

      RETVALUE(RFAILED);
   }

   if (mBuf == (Buffer *)NULLP)
   {
      RLOG0(L_ERROR, "Invalid MBuf");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
   /*  Handling Events for ENTEU */
   switch (pst->srcEnt)
   {
      case ENTWR:
         switch (pst->event)
         {
              /* Tunnel Creation */
            case EVTDAMTNLCRETREQ:
               cmUnPkTnlCreatReq(pst,mBuf);
               break;
              /* Tunnel Del Req */
            case EVTDAMTNLDELTREQ:
               cmUnPkTnlDelReq(pst, mBuf);
               break;
              /* UE Del Req */
            case EVTDAMUEDELTREQ:
               cmUnPkUeDelReq(pst, mBuf);
               break;
              /* Cell Add Req */
            case EVTDAMCELLADDREQ:
              cmUnPkCellAddReq(pst, mBuf);
               break;
              /* UE Reconf Req */
            case EVTDAMRECFGREQ:
               cmUnPkReCfgReq(pst, mBuf);
               break;
             /* UE Re-Estb. */
            case EVTDAMRESTREQ:
               cmUnPkReestabReq(pst, mBuf);
               break;
             /* Tunnel State Change Req */
            case EVTDAMTNLSTCHNGREQ:
              cmUnPkTnlStChngReq(pst,mBuf);
              break;
             /* Start Inactivity Timer */
            case EVTDAMINACTIVITYTMR:
              cmUnPkStartInactivityTmr(pst,mBuf);
              break;
              /* UE Stop Timer */
            case EVTDAMUESTOPTMR:
              cmUnPkStopUeTmr(pst,mBuf);
              break;
             /* DAM Cfg Req */
            case EVTDAMCFGREQ:
              cmUnpkDamCfgReq(pst,mBuf);
              break;
             /* DAM Control Req */
            case EVTDAMCNTRLREQ:
              cmUnpkDamCntrlReq(pst,mBuf);
              break;
             /* DAM Tunnel Modification Req */
            case EVTDAMTNLMODREQ:
              cmUnpkTnlModReq(pst,mBuf);
              break;
            case EVTDAMHDLHOFAIL:
              cmUnPkHdlHoFailure(pst,mBuf); 
              break;
            /*ccpu00138576 - start*/ 
            case EVTDAMDATRCVDFLAG:
              cmUnPkSetDataRcvdFlag(pst,mBuf);
              break;
            /*ccpu00138576 - end*/ 
            default:
              RLOG0(L_ERROR, "Unknown Event from SM");
               (Void)SPutMsg(mBuf);
               break;
         }/* Event Switch */
         break;
      case ENTEG:
         switch (pst->event)
         {
            case EVTEGTBNDCFM:
               (Void)cmUnpkEgtBndCfm(EuLiEgtBndCfm, pst, mBuf);
               break;
            case EVTEGTULCLTNLMGMTCFM:
               (Void)cmUnpkEgtEguLclTnlMgmtCfm(EuLiEgtEguLclTnlMgmtCfm, pst, mBuf);
               break;
            default:
              RLOG0(L_ERROR, "Unknown Event from EGT");
               (Void)SPutMsg(mBuf);
               break;
         }
         break;
      case ENTPJ:
         switch (pst->event)
         {
            case  EVTPJUBNDCFM:
               cmUnpkPjuBndCfm(PxLiPjuBndCfm, pst, mBuf);
               break;
            case  EVTPJUDATIND:
               cmUnpkPjuDatInd(PxLiPjuDatInd, pst, mBuf);
               break;
#ifdef SS_RBUF
            case  EVTFPPJUDATIND:
               cmUnpkFpPjuDatInd(PxLiPjuDatInd, pst, mBuf);
               break;
#endif
            case EVTPJUDATFWDIND:
               cmUnpkPjuDatFwdInd(PxLiPjuDatFwdInd, pst, mBuf);
               break;
            case  EVTPJUDATCFM:
               cmUnpkPjuDatCfm(PxLiPjuDatCfm, pst, mBuf);
               break;
            default:
               RLOG0(L_ERROR, "Unknown Event from PJU");
               (Void)SPutMsg(mBuf);
               break;
         }

         break;
   default:
      RLOG0(L_ERROR, "Received Msg From Unknow Src");
         (Void)SPutMsg(mBuf);
         break;
   }/* Entity Switch */
   (Void)SExitTsk();
   RETVALUE(ROK);
} /* end of wrDamActvTsk() */

/********************************************************************30**
  
         End of file:     fc_dam_ex_ms.c@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:51:33 2014
  
*********************************************************************31*/
/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
*********************************************************************91*/
