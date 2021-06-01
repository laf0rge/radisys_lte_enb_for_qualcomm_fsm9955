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

     Name:    Egtp

     Type:    C source file

     Desc:    Functions required for scheduling and initialization.

     File:    eg_ex_ms.c

     Sid:      eg_ex_ms.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/10 - Mon Feb 15 12:52:58 2016

     Prg:     pc

*********************************************************************21*/



/************************************************************************
 * H E A D E R     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_dns.h"        /* common transport */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "hit.h"           /* HIT interface defines           */
/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_psf.h"
#include "sht.h"           /* SHT include file                */
#endif
#include "egt.h"           /* EGT interface defines           */
#include "eg_util.h"       /* Egtp Utility library            */
#include "leg.h"           /* Layer management, Egtp          */
#include "eg.h"            /* Egtp layer defines              */
#include "eg_err.h"        /* Error codes                     */
#include "eg_edm.h"        /* EDM Module structures            */
#include "ctf.h"

/************************************************************************
 * E X T E R N     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_dns.x"        /* common transport */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "hit.x"           /* HIT interface defines           */
/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"           /* SHT include file                */
#endif
#include "egt.x"           /* SOT interface defines           */
#include "eg_util.x"       /* Egtp Utility library */
#include "leg.x"           /* Layer management Egtp            */
#include "eg_edm.x"        /* EDM Module structures            */
#include "eg.x"            /* Egtp layer structures            */
#include "eg_tpt.x"            /* EGTP TPT module defines */

/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "lhw.x"
#include "hw.x"          /* EGTPC UA                       */
#endif

#ifdef REVIEW
#include "eg_socklayer.h"
#include "eg_socklayer.x"
#endif

#if (defined(SS_RBUF) || defined(L2_L3_SPLIT)|| defined(LTE_QCOM))
#include "rl_rlog.h"
#endif
#ifdef SS_RBUF
PUBLIC void wrDamBatchProc(Void);
/* WORKAROUND: Disabling Logger thread */
#if 0               
static U8 ttiCntToReadLog = 1;
#endif
#endif
PUBLIC U32 ysEgTxPrcCnt = 0;
PUBLIC U32 ysEgRxPrcCnt = 0;
/*
*
*       Fun:    activate task
*
*       Desc:   Processes received events from the Egtp interfaces.
*               Primitives are unpacked and the correct functions are called.
*
*       Ret:    ROK  -    ok
*               RFAILED - failed
*
*       Notes:  This is only used for dispatching to the unpacking routines.
*               If tightly coupled then we should never be in this file.
*
*       File:   eg_ex_ms.c
*
*/
extern U32 tmrFlag;
#ifdef ANSI
PUBLIC S16 egActvTsk
(
Pst    *pst,                /* post message                       */
Buffer *mBuf                /* message buffer                     */
)
#else
PUBLIC S16 egActvTsk(pst, mBuf)
Pst    *pst;                /* post                               */
Buffer *mBuf;               /* message buffer                     */
#endif
{
   /* local variables */
   S16         retVal = ROK;/* return code                        */
   Mem         sMem;        /* mem block passed to unpacking code */
#ifdef EGTP_U
#endif /* EGTP_U */

   EG_TRC2(egActvTsk);

#ifdef SS_MULTIPLE_PROCS
   if((SGetXxCb(pst->dstProcId, pst->dstEnt,\
               egCb.init.inst,(Void **)&egCbPtr))!=ROK)
   {
      EGLOGERROR(ERRCLS_DEBUG, EEG094, 0,\
            "egActvTsk() failed, cannot derive egCb");
      RETVALUE(RFAILED);
   }

   EG_DBG_INFO(0,0, (egp,\
            "-----EGTP-----(proc(%d), entt(%d), inst(%d))------\n"\
            ,pst->dstProcId, pst->dstEnt, egCb.init.inst));

#endif/* SS_MULTIPLE_PROCS */

   sMem.region = egCb.init.region;
   sMem.pool   = egCb.init.pool;

   /********************************************
    *  Check if the received mBUF if null?      *
    ********************************************/
   if(mBuf == NULLP)
   {
      EGLOGDBGERR( EEG095, pst->event,
            "egActvTsk: received mBuf is NULLP");
      EG_RETVALUE(RFAILED);
   }

   switch(pst->srcEnt)
   {
#ifdef LCEGLIHIT
      /********************************************
       *    loosely coupled lower interface        *
       ********************************************/
      case ENTHI:
         {
            switch(pst->event)
            {
               /********************************************
                *             BIND   CONFIRM                *
                ********************************************/
               case EVTHITBNDCFM:
                  {
                     retVal = cmUnpkHitBndCfm(EgLiHitBndCfm, pst, mBuf);
                     break;
                  }

                  /********************************************
                   *             CONNECTION   CONFIRM                *
                   ********************************************/
               case EVTHITCONCFM:
                  {
                     retVal = cmUnpkHitConCfm(EgLiHitConCfm, pst, mBuf);
                     break;
                  }
#ifdef EG_PHASE2
                  /********************************************
                   *             FLOW   INDICATION             *
                   ********************************************/
               case EVTHITFLCIND:
                  {
                     retVal = cmUnpkHitFlcInd(EgLiHitFlcInd, pst, mBuf);
                     break;
                  }
#endif /* EG_PHASE2 */
                  /********************************************
                   *         UDP DATA INDICATION               *
                   ********************************************/
               case EVTHITUDATIND:
                  {
                     retVal = cmUnpkHitUDatInd(EgLiHitUDatInd, pst, mBuf);
                     break;
                  }

                  /********************************************
                   *             DISC   INDICATION             *
                   ********************************************/
               case EVTHITDISCIND:
                  {
                     retVal = cmUnpkHitDiscInd(EgLiHitDiscInd, pst, mBuf);
                     break;
                  }

                  /********************************************
                   *             DISC   CONFIRM                *
                   ********************************************/
               case EVTHITDISCCFM:
                  {
                     retVal = cmUnpkHitDiscCfm(EgLiHitDiscCfm, pst, mBuf);
                     break;
                  }

#ifdef ERRCHK
                  /********************************************
                   *      Should hopefully never get here      *
                   ********************************************/
               default:
                  {
                     EG_FREEMBUF(mBuf);
                     EGLOGDBGERR( EEG096, pst->event,
                           "egActvTsk: Invalid event");
                     retVal = RFAILED;
                     break;
                  }
#endif
            }
            break;
         }
#endif /* LCEGLIHIT */

#if (defined(LCEGUIEGT) || defined(LWLCEGUIEGT))
         /********************************************
          *    Loosely Coupled UPPER INTERFACE        *
          ********************************************/
      case ENTEU:
      case ENTVE:
#ifdef REVIEW
      case ENTYS:
#endif
         {
            switch(pst->event)
            {
               /********************************************
                *               BIND  REQUEST               *
                ********************************************/
               case EVTEGTBNDREQ:
                  {
                     retVal = cmUnpkEgtBndReq(EgUiEgtBndReq, pst, mBuf);
                     break;
                  }

                  /********************************************
                   *             UNBIND  REQUEST               *
                   ********************************************/
               case EVTEGTUBNDREQ:
                  {
                     retVal = cmUnpkEgtUbndReq(EgUiEgtUbndReq, pst, mBuf);
                     break;
                  }

#ifdef EGTP_U
                  /********************************************
                   *           UNIT DATA REQUEST                *
                   ********************************************/
               case EVTEGTUDATREQ:
                  {
                     /* eg001.201  Updating the Procedure arguments */
                     retVal = cmUnpkEgtEguDatReq(EgUiEgtEguDatReq, pst, mBuf, &sMem);
                     break;
                  }

                  /********************************************
                   *    TUNNEL MANAGEMENT  REQUEST             *
                   ********************************************/
               case EVTEGTULCLTNLMGMTREQ:
                  {
                     /* eg001.201  Updating the Procedure arguments */
                     retVal = cmUnpkEgtEguLclTnlMgmtReq(EgUiEgtEguLclTnlMgmtReq, pst, mBuf, &sMem);
                     break;
                  }

                  /********************************************
                   *                STATUS REQUEST             *
                   ********************************************/
               case EVTEGTUSTAREQ:
                  {
                     /* eg001.201  Updating the Procedure arguments */
                     retVal = cmUnpkEgtEguStaReq(EgUiEgtEguStaReq, pst, mBuf, &sMem);
                     break;
                  }
#ifdef REVIEW
               case EVTEGTTXDATA:
                  {
                     SPutMsg(mBuf);
                     ysEgTxPrcCnt++;

#if (defined MSPD) || (defined LTE_QCOM)
                     EgTLSendMsg(FALSE); /* Arg FALSE is req if ENB_CPU_OVERLOAD_CONTROL is enabled,
                                            just to control the num of pkts to be sent over the socket*/
#else
                     EgTLSendMsg();
#endif
                     break;
                  }

               case EVTEGTRXDATA:
                  {
                     SPutMsg(mBuf);
                     ysEgRxPrcCnt++;

#if (defined MSPD) || (defined LTE_QCOM) 
                     EgTLSendMsg(FALSE); /* Arg FALSE is req if ENB_CPU_OVERLOAD_CONTROL is enabled,
                                            just to control the num of pkts to be sent over the socket*/
                     rlProcessTicks();
#else
                     EgTLSendMsg();
#endif

#ifdef FLAT_BUFFER_OPT
                     EgTLReadMsgFB(0,10);
#else
                     EgTLReadMsg(0,10);
#endif
                     break;
                  }
#endif

#endif /* EGTP_U */

#ifdef ERRCHK
                  /********************************************
                   *      Should hopefully never get here      *
                   ********************************************/
               default:
                  {
                     EG_FREEMBUF(mBuf);
                     EGLOGDBGERR( EEG097, pst->event,
                           "egActvTsk: Invalid event");
                     retVal = RFAILED;
                     break;
                  }
#endif
            }

            break;
         }
#endif /* LCEGUIEGU or LWLCEGUIEGU */

#ifdef LCSMEGMILEG
         /********************************************
          *  Loosely coupled LAYER MANAGER INTFERFACE *
          ********************************************/
      case ENTSM:
         {
            switch(pst->event)
            {
               /********************************************
                *             CONFIG  REQUEST               *
                ********************************************/
               case EVTLEGCFGREQ:
                  {
                     retVal = cmUnpkLegCfgReq(EgMiLegCfgReq, pst, mBuf);
                     break;
                  }

                  /********************************************
                   *            CONTROL  REQUEST               *
                   ********************************************/
               case EVTLEGCNTRLREQ:
                  {
                     retVal = cmUnpkLegCntrlReq(EgMiLegCntrlReq, pst, mBuf);
                     break;
                  }

                  /********************************************
                   *               STATUS  REQUEST             *
                   ********************************************/
               case EVTLEGSTAREQ:
                  {
                     retVal = cmUnpkLegStaReq(EgMiLegStaReq, pst, mBuf);
                     break;
                  }

                  /********************************************
                   *           STATISTICS  REQUEST             *
                   ********************************************/
               case EVTLEGSTSREQ:
                  {
                     retVal = cmUnpkLegStsReq(EgMiLegStsReq, pst, mBuf);
                     break;
                  }

                  /********************************************
                   *      Should hopefully never get here      *
                   ********************************************/
               default:
                  /*eg004.201 Added for eGTP-C PSF Upgrade:PSF Active task handler to handle PSF event*/
#ifdef HW
                  hwActvTsk(pst, mBuf);
#else
                  {
                     EG_FREEMBUF(mBuf);
                     EGLOGDBGERR( EEG098, pst->event, "egActvTsk: Invalid event");
                     retVal = RFAILED;
                     break;
                  }
#endif
            }

            break;
         }
#endif /* LCSMEGMILEG */
         /*eg004.201 Added for eGTP-C PSF Upgrade:PSF Event PACK/UNPACK followed by invoking respective
          *event hadlers
          */
#ifdef TDS_CORE2
      case ENTSH:
         switch (pst->event)
         {
#ifdef EG_FTHA
            case EVTSHTCNTRLREQ:            /* system agent control request */
               cmUnpkMiShtCntrlReq(EgMiShtCntrlReq, pst, mBuf);
               break;
#endif
            default:
#ifdef HW
               hwActvTsk(pst, mBuf);
#else
               EG_FREEMBUF(mBuf);
               EGLOGDBGERR( EEG101, pst->event,
                     "egActvTsk: Invalid event");
#endif
               break;
         }
         break;
#endif
#ifdef SS_RBUF
      case ENTEG:
         switch (pst->event)
         {
            case EVTCTFBTCHPROCTICK:            /* TTI Indication */

               bcCpuhPktFlowCtrl();
               
               EgTLReadMsg(0,egTLCb.maxMsgToread);
               
               wrDamBatchProc();
               
               EgTLSendMsg();
               
               bcIcpuBatchMgrProcess();
               /* WORKAROUND: Disabling Logger thread */
#if 0               
               if(ttiCntToReadLog == PROCESS_L2LOG_TTI)
               { 
                  readL2LogBuff();
                  ttiCntToReadLog = 1;
               }
               else
               {
                  ttiCntToReadLog++;
               }
#endif
               /* End of WORKAROUND */

               SPutMsg(mBuf);
               
               tmrFlag = 0;
               
               break;
            default:
               EG_FREEMBUF(mBuf);
               EGLOGDBGERR( EEG101, pst->event,
                     "egActvTsk: Invalid event");
               break;
         }
         break;
#endif
#ifdef HW
      case ENTEG:
         hwActvTsk(pst, mBuf);
         break;
#endif
         /********************************************
          *      Should hopefully never get here      *
          ********************************************/
      default:
         {
            EGLOGDBGERR( EEG101, pst->event,
                  "egActvTsk: Invalid event");
            EG_FREEMBUF(mBuf);
            retVal = RFAILED;
            break;
         }
   }

   SExitTsk();
   EG_RETVALUE(retVal);
}



/********************************************************************30**

        End of file:     eg_ex_ms.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/10 - Mon Feb 15 12:52:58 2016

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
/main/1      ---         ad              1. Created for Initial Release 1.1
/main/2      ---         pmacharla       1. Initial for eGTP 1.2 Release
/main/2      eg001.102    snatarajan        1. EGTP_C Path management functionality is introduced.
                          rss               2. Multithreaded performance changes gaurded under SS_MT_CB flag.
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3     eg001.201   asaurabh        1. Updating the procedure arguments
/main/3     eg004.201   magnihotri        1. Header files added for eGTP-C PSF
                                          2. Added for eGTP-C PSF Upgrade:PSF Active task
                                             handler to handle PSF event
                                          3. Added for eGTP-C PSF Upgrade:PSF Event PACK/UNPACK followed
                                             by invoking respective
                                             event hadlers
*********************************************************************91*/
