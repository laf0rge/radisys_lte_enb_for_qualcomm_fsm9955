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

     Name:     X2AP Layer 
  
     Type:     C source file
  
     Desc:     C Source code

     File:     cz_ex_ms.c

     Sid:      tr_ex_ms.c@@/main/2 - Tue Aug 30 18:36:11 2011

     Prg:      sy

*********************************************************************21*/

/* Header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System Services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common structs 1 */
#include "cm_mblk.h"       /* common memory */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common structs 3 */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport */
#include "cm_pasn.h"       /* Common Asn.1 library           */
#include "sct.h"           /* SCTP Layer */
#include "czt.h"           /* X2AP Upper Interface */
#include "lcz.h"           /* X2AP LM Interface */
#include "cz.h"            /* X2AP Layer */
#include "cz_err.h"        /* X2AP Error */
#include "czt_asn.h"       /* X2AP ASN */

/* Header include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System Services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
#include "cm_pasn.x"       /* Common Asn.1 library           */
#include "sct.x"           /* SCTP Layer */
#include "czt.x"           /* X2AP Upper Interface */
#include "lcz.x"           /* X2AP LM Interface */
#include "cz.x"            /* X2AP Layer */
#include "czt_asn.x"       /* X2AP ASN */

/* functions in other modules */

/* public variable declarations */

/* control variables for testing */

/* private variable declarations */

/*
 * support functions
 */

/* cz001.101 : CR fixes */

/*
 *
 *       Fun  :  activate task
 *
 *       Desc :  All loosely coupled messages that are received by RANAP
 *               arrive at this point. This includes loosely coupled layer 
 *               management requests and loosely coupled messages arriving 
 *               from the service user and service provider(s).
 *
 *       Ret  :  ROK  - ok
 *
 *       Notes:  None
 *
 *       File :  cz_ex_ms.c
 *
 */
#ifdef ANSI
PUBLIC S16 czActvTsk
(
Pst     *pst,                                   /* post structure       */
Buffer  *mBuf                                   /* message buffer       */
)
#else
PUBLIC S16 czActvTsk(pst, mBuf)
Pst     *pst;                                   /* post structure       */
Buffer  *mBuf;                                  /* message buffer       */
#endif
{
   Mem sMem;

   CZ_TRC3(czActvTsk);

#ifdef SS_MULTIPLE_PROCS
   if(((SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&czCbPtr))
                 !=ROK)||( czCbPtr == NULLP))
   {

#if (ERRCLASS & ERRCLS_INT_PAR)
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, (S8*)__FILE__, __LINE__,\
             ERRCLS_INT_PAR,1,0, (S8*)"czActvTsk() failed, cannot derive czCb");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      SPutMsg(mBuf);
      /* cz001.301: Klockwork warning fix*/
      RETVALUE(FALSE);
   }

#endif /*SS_MULTIPLE_PROCS */

   sMem.region = czCb.init.region;
   sMem.pool = czCb.init.pool;

   switch(pst->srcEnt)
   {
      /* The originator of this message is the stack manager,
       * unpack and go to the respective primitive processing function */
#ifdef LCLCZ
      case ENTSM:
      {
         switch(pst->event)
         {
            case EVTLCZCFGREQ:
               /* Process a config. request */
               cmUnpkLczCfgReq(CzMiLczCfgReq, pst, mBuf);
               break;
            case EVTLCZCNTRLREQ:
               /* Process a control request */
               cmUnpkLczCntrlReq(CzMiLczCntrlReq, pst, mBuf);
               break;
            case EVTLCZSTAREQ:
               /* Process a status request  */
               cmUnpkLczStaReq(CzMiLczStaReq, pst, mBuf);
               break;
            case EVTLCZSTSREQ:
               /* Process a statistics request */
               cmUnpkLczStsReq(CzMiLczStsReq, pst, mBuf);
               break;
            default:
               CZ_DBG_ERROR((CZ_PRNT_BUF,
                      "Invalid event from Layer Manager: czActvTsk Failed"));
               SPutMsg(mBuf);
               break;
         }
         break;
      }
#endif /* LCLCZ */

#ifdef LCSCT
      case ENTSB:
      case ENTHI:
      {
         S16 ret = RFAILED;
         switch(pst->event)
         {   
            /* Bind confirm */
            case SCT_EVTBNDCFM:
               ret = cmUnpkSctBndCfm(CzLiSctBndCfm, pst, mBuf);
               break;
            /* Assoc. Establishment indication */
            case SCT_EVTASSOCIND:
               ret = cmUnpkSctAssocInd(CzLiSctAssocInd, pst, mBuf);
               break;
            /* Assoc. Establishment confirmation */
            case SCT_EVTASSOCCFM:
               ret = cmUnpkSctAssocCfm(CzLiSctAssocCfm, pst, mBuf);
               break;
            /* Association Termination indication */
            case SCT_EVTTERMIND:
               ret = cmUnpkSctTermInd(CzLiSctTermInd, pst, mBuf);
               break;
            /* Assoc. Termination confirmation */
            case SCT_EVTTERMCFM:
               ret = cmUnpkSctTermCfm(CzLiSctTermCfm, pst, mBuf);
               break;
            /* Set Primary confirmation */
            case SCT_EVTSETPRICFM:
               ret = cmUnpkSctSetPriCfm(CzLiSctSetPriCfm, pst, mBuf);
               break;
            /*Set heartbeat confirmation */
            case SCT_EVTHBEATCFM:
               ret = cmUnpkSctHBeatCfm(CzLiSctHBeatCfm, pst, mBuf);
               break;
            /* Status confirm */
            case SCT_EVTSTACFM:
               ret = cmUnpkSctStaCfm(CzLiSctStaCfm, pst, mBuf);
               break;
            /* Data Indication */
            case SCT_EVTDATIND:
               ret = cmUnpkSctDatInd(CzLiSctDatInd, pst, mBuf);
               break;
            /* Status Indication */
            case SCT_EVTSTAIND:
               ret = cmUnpkSctStaInd(CzLiSctStaInd, pst, mBuf);
               break;
            /* Flow control indication */
            case SCT_EVTFLCIND:
               ret = cmUnpkSctFlcInd(CzLiSctFlcInd, pst, mBuf);
               break;
            /* Endpoint open confirmation */
            case SCT_EVTENDPOPENCFM:
               ret = cmUnpkSctEndpOpenCfm(CzLiSctEndpOpenCfm, pst, mBuf);
               break;
            /* Endpoint close confirmation */
            case SCT_EVTENDPCLOSECFM:
               ret = cmUnpkSctEndpCloseCfm(CzLiSctEndpCloseCfm, pst, mBuf);
               break;

            default:
            {
#if (ERRCLASS & ERRCLS_DEBUG)
               if (pst->event == ENTSB)
               {
                  CZ_DBG_ERROR((CZ_PRNT_BUF,
                    "czActvTsk: Invalid event specified for srcEnt (ENTSB)."));
               }
               if (pst->event == ENTHI)
               {
                  
                  CZ_DBG_ERROR((CZ_PRNT_BUF,
                    "czActvTsk: Invalid event specified for srcEnt (ENTHI)."));
               }

#endif /* (ERRCLASS & ERRCLS_DEBUG) */
               SPutMsg(mBuf);
               break;
            }
         }
         break;
      }
#endif /* LCSCT */

#if (defined(LCCZT) || defined(LWLCCZT))
      case ENTRS:
      {
         switch (pst->event)
         {
            case EVTCZTBNDREQ:
               cmUnpkCztBndReq(CzUiCztBndReq, pst, mBuf);
               break;
            case EVTCZTUBNDREQ:
               cmUnpkCztUbndReq(CzUiCztUbndReq, pst, mBuf);
               break;
            case EVTCZTGPREQ:
               cmUnpkCztGpReq(CzUiCztGpReq, pst, mBuf);
               break;
            case EVTCZTGPRSP:
               cmUnpkCztGpRsp(CzUiCztGpRsp, pst, mBuf);
               break;
            case EVTCZTGPRETXREQ:
               cmUnpkCztGpRetxReq(CzUiCztGpRetxReq, pst, mBuf);
               break;
            case EVTCZTBMPREQ:
               cmUnpkCztBmpReq(CzUiCztBmpReq, pst, mBuf);
               break;
            case EVTCZTBMPRSP:
               cmUnpkCztBmpRsp(CzUiCztBmpRsp, pst, mBuf);
               break;
            case EVTCZTGPERRREQ:
               cmUnpkCztGpErrReq(CzUiCztGpErrReq, pst, mBuf);
               break;
            case EVTCZTPEERASSOCRSP:
               cmUnpkCztDynamPeerAssocRsp(CzUiCztDynamPeerAssocRsp,  pst, mBuf);
               break;
            case EVTCZTLOCALRELREQ:
               cmUnpkCztRelReq(CzUiCztRelReq,  pst, mBuf);
               break;
            case EVTCZTAUDREQ:
               cmUnpkCztAudReq(CzUiCztAudReq,  pst, mBuf);
               break;
            default:
#if (ERRCLASS & ERRCLS_DEBUG)
                  CZ_DBG_ERROR((CZ_PRNT_BUF,
              "Invalid event from Upper Interface: czActvTsk Failed"));
#endif /* ERRCLASS & ERRCLS_DEBUG */
               SPutMsg(mBuf);
               break;
         }
         break;
      }
#endif 
      default:
#if (ERRCLASS & ERRCLS_DEBUG)
                  CZ_DBG_ERROR((CZ_PRNT_BUF,
              "Event Received from Invalid Entity : czActvTsk Failed"));
#endif /* ERRCLASS & ERRCLS_DEBUG */
        SPutMsg(mBuf);
        break;

   } /* end switch */

   SExitTsk();
   CZ_RETVALUE(ROK);

} /* end of czActvTsk */

/********************************************************************30**
 
         End of file:     tr_ex_ms.c@@/main/2 - Tue Aug 30 18:36:11 2011
 
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
/main/1      ---      sy             1. LTE-X2AP Initial Release.
           cz001.101  mm             1. CR Fixes, Memory Leak fixes.
/main/2      ---      pt             1. LTE-X2AP 3.1 release.
/main/3    cz001.301  akaranth       1. Klockwork Warning fix
*********************************************************************91*/
