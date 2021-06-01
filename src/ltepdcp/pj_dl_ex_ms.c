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

Name:    LTE-PDCP Layer - System Services Interface Functions

Type:    C file

Desc:    C source code for the interface to System Services
of LTE-PDCP

File:    pj_ul_ex_ms.c

Sid:      eq_dl_ex_ms.c@@/main/TeNB_Main_BR/12 - Mon Aug 11 16:42:46 2014

Prg:    

*********************************************************************21*/
  
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=217;
/** @file pj_ul_ex_ms.c
  @brief PDCP System Services Interface
 */

/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "kwu.h"           /* KWU defines */
#include "cpj.h"           /* CPJ defines */
#include "ctf.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#include "lpj.h"           /* LPJ defines */
#include "pj_err.h"        /* Err defines */
#include "pj_env.h"        /* RLC environment options */
#include "pj.h"            /* RLC defines */
#include "pj_udx.h"
#include "pj_dl.h"
#include "pj_lib.h"
#include "pj_ptsec.h"

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "kwu.x"           /* KWU */
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#include "lpj.x"           /* LPJ defines */
#include "pj.x"
#include "pj_udx.x"
#include "pj_dl.x"
#include "pj_ul.x"
#include "pj_lib.x"
#ifdef SS_RBUF
#include "ss_rbuf.h"
#include "ss_rbuf.x"
PUBLIC S16 pjBatchProcTick(Void);
EXTERN S16 pjRBufFreeMsg(void);
#endif


PUBLIC U32 ysPjPrcTtiInd = 0;   /* UDAY */

/***********************************************************************
  System Service Interface Functions
 ***********************************************************************/
/**
 *
 * @brief
 *
 *    <b> Activates Initialization </b>
 *
 *    @b Description:
 *    This function is invoked by system services to initialize the LTE-PDCP
 *    layer. This is an entry point used by LTE_PDCP layer to initialize its
 *    global variables, before becoming operational.
 *
 *    Allowable values for parameters are specified in ssi.h.
 *
 *    @param[in] ent    - Specify the entity id of the LTE-RLC task.
 *    @param[in] inst   - Specify the entity id of the LTE-RLC task.
 *    @param[in] region - Specifies the memory region from which
 *                         LTE-PDCP should allocate structures and buffers.
 *    @param[in] reason - Specifies the reason for calling this
 *                         initialization function.
 *
 *  @return  S16
 *      -# ROK
 *
 */
#ifdef ANSI
PUBLIC S16 pjDlActvInit
(
 Ent    ent,                 /* entity */
 Inst   inst,                /* instance */
 Region region,              /* region */
 Reason reason               /* reason */
 )
#else
PUBLIC S16 pjDlActvInit(ent, inst, region, reason)
  Ent    ent;                 /* entity */
  Inst   inst;                /* instance */
  Region region;              /* region */
  Reason reason;              /* reason */
#endif
{
   PjCb   *tPjCb;
   TRC3(pjDlActvInit)
   if (inst >= PJ_MAX_PDCP_INSTANCES)
   {
       /* intance greater than MAX instances */ 
       RETVALUE(RFAILED); 
   }

   if (pjCb[inst] != NULLP)
   {
      RETVALUE (RFAILED);
   }

   if (SGetSBuf(region, 0, (Data **)&tPjCb,
            (Size)sizeof (PjCb)) != ROK)
   {
      RETVALUE(RFAILED);
   }
   /* Initialize kwCb */
   PJ_MEM_SET(tPjCb, 0, sizeof(PjCb));

   /* Initialize task configuration parameters */
   /* We will take only entity name */
   tPjCb->init.ent     = ent;           /* entity */
   tPjCb->init.inst    = inst;          /* instance */
   tPjCb->init.region  = region;        /* static region */
   tPjCb->init.pool    = 0;             /* static pool */
   tPjCb->init.reason  = reason;        /* reason */
   tPjCb->init.cfgDone = FALSE;         /* configuration done */
   tPjCb->init.acnt    = TRUE;          /* enable accounting */
   tPjCb->init.usta    = TRUE;          /* enable unsolicited status */
   tPjCb->init.trc     = FALSE;         /* enable trace */
#ifdef DEBUGP
   tPjCb->init.dbgMask = 0x000000;
#endif
   tPjCb->init.procId  = SFndProcId();

   pjCb[inst] = tPjCb;

  RETVALUE(ROK);
} /* pjActvInit */


/**
 *
 * @brief
 *
 *  <b> Activation Task </b>
 *
 *  @b Description:
 *  Processes events received for LTE-PDCP layer via System Services from
 *  other layers.
 *
 *  @param[in] pst   - Pst Structure
 *  @param[in] mBuf  - Message Buffer
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 pjDlActvTsk
(
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 pjDlActvTsk(pst, mBuf)
Pst *pst;              /* pst structure */
Buffer *mBuf;           /* message buffer */
#endif
{
   S16 ret = ROK;

   TRC3(pjDlActvTsk);

   switch(pst->srcEnt)
   {
      case ENTSM:
         {
            switch(pst->event)
            {
#ifdef LCLPJ
               case LPJ_EVT_CFG_REQ:
                  {
                     ret = cmUnpkLpjCfgReq(PjMiLpjCfgReq, pst, mBuf);
                     break;
                  }

               case LPJ_EVT_CNTRL_REQ:
                  {
                     ret = cmUnpkLpjCntrlReq(PjMiLpjCntrlReq, pst, mBuf);
                     break;
                  }

               case LPJ_EVT_STS_REQ:
                  {
                     ret = cmUnpkLpjStsReq(PjMiLpjStsReq, pst, mBuf);
                     break;
                  }

               case LPJ_EVT_STA_REQ:
                  {
                     ret = cmUnpkLpjStaReq(PjMiLpjStaReq, pst, mBuf);
                     break;
                  }
                  /* kw005.201 added support for L2 Measurement */

               default:
                  {
                     RLOG0(L_ERROR, "Received Invalid Event from SM");
                     ret = RFAILED;
                     break;
                  }
#endif  /* LCLPJ */
            }
            break;
         }

      case ENTPJ:
         {
            switch(pst->event)
            {
#ifdef LCUDX
            case EVTUDXBNDREQ:
                  {
                     cmUnpkUdxBndReq(PjDlUdxBndReq, pst, mBuf);
                     break;
                  }

            case EVTUDXUBNDREQ:
                  {
                     cmUnpkUdxUbndReq(PjDlUdxUbndReq, pst, mBuf);
                     break;
                  }

            case EVTUDXCFGREQ:
                  {
                     cmUnpkUdxCfgReq(PjDlUdxCfgReq, pst, mBuf);
                     break;
                  }

            case EVTUDXULSTAREP:
                  {
                     cmUnpkUdxUlStaRep(PjDlUdxUlStaRep, pst, mBuf);
                     break;
                  }

            case EVTUDXDLSTAREP:
                  {
                     cmUnpkUdxDlStaRep(PjDlUdxDlStaRep, pst, mBuf);
                     break;
                  }            

            case EVTUDXULROHCFDB:
                  {
                      cmUnpkUdxUlFdbkPkt(PjDlUdxUlFdbkPktInfo, pst, mBuf);
                      break;
                  }

            case EVTUDXDLROHCFDB:
                  {
                      cmUnpkUdxDlFdbkPkt(PjDlUdxDlFdbkPktInfo, pst, mBuf);
                      break;
                  }
            
            case EVTUDXREESTREQ:
                  {
                     cmUnpkUdxReEstReq(PjDlUdxReEstReq, pst, mBuf);
                     break;
                  }

            case EVTUDXSECCFGREQ:
                  {
                     cmUnpkUdxSecCfgReq(PjDlUdxSecCfgReq, pst, mBuf);
                     break;
                  }

            case EVTUDXUEIDCHGREQ:
                  {
                     cmUnpkUdxUeIdChgReq(PjDlUdxUeIdChgReq, pst, mBuf);
                     break;
                  }

            case EVTUDXCOUNTREQ:
                  {
                     cmUnpkUdxCountReq(PjDlUdxCountReq, pst, mBuf);
                     break;
                  }

            case EVTUDXSDUSTAREQ:
                  {
                     cmUnpkUdxSduStaReq(PjDlUdxSduStaReq, pst, mBuf);
                     break;
                  }
             case EVTUDXDATRESUME:
                  {
#ifdef FLAT_BUFFER_OPT
                       printf("Received Data Resume at PDCP \n");
                       cmUnpkUdxDatResumeReq(PjDlUdxDatResumeReqFB, pst, mBuf);
#else
                       cmUnpkUdxDatResumeReq(PjDlUdxDatResumeReq, pst, mBuf);
#endif
                       break;
                  }
             case EVTUDXDATFRWD:
                  {
#ifdef FLAT_BUFFER_OPT
                     printf("Received Event EVTUDXDATFRWD at PDCP DL \n");
                     cmUnpkUdxDatForwardReq(PjDlUdxDatForwardReqFB, pst, mBuf);
#else
                     cmUnpkUdxDatForwardReq(PjDlUdxDatForwardReq, pst, mBuf);
#endif
                     break;
                  }
#ifdef LTE_L2_MEAS
               case UDX_EVT_L2MEAS_REQ:
                  {
                     ret = cmUnpkUdxL2MeasReq(PjDlUdxL2MeasReq, pst, mBuf);
                     break;
                  }
               case UDX_EVT_L2MEAS_SEND_REQ:
                 {

                    ret = cmUnpkUdxL2MeasSendReq(PjDlUdxL2MeasSendReq, pst, mBuf); 
  
                     break;
                 }
               case UDX_EVT_L2MEAS_STOP_REQ:
                 {
                     ret = cmUnpkUdxL2MeasStopReq(PjDlUdxL2MeasStopReq, pst, mBuf);
                     break;
                 }
#endif
               default:
                  {
                     RLOG0(L_ERROR, "Received Invalid Event from PDCP UL");
                     ret = RFAILED;
                     break;
                  }
#endif  /* LCLPJ */
            }
            break;
         }


      case ENTNH:
         {
            switch(pst->event)
            {
#ifdef LCPJU
               case EVTPJUBNDREQ:
                  {
                     cmUnpkPjuBndReq(PjUiPjuBndReq, pst, mBuf);
                     break;
                  }

               case EVTPJUUBNDREQ:
                  {
                     cmUnpkPjuUbndReq(PjUiPjuUbndReq, pst, mBuf);
                     break;
                  }

               case EVTPJUDATREQ:
                  {
                     cmUnpkPjuDatReq(PjUiPjuDatReq, pst, mBuf);
                     break;
                  }

               case EVTPJUDATFWDREQ:
                  {
                     cmUnpkPjuDatFwdReq(PjUiPjuDatFwdReq, pst, mBuf);
                     break;
                  }
#endif /* LCPJU */
               default:
                  SPutMsg(mBuf);
                  RLOG0(L_ERROR, " Received Invalid Event From RRC");
                  ret = RFAILED;
                  break;

            }
            break;
         }

      case ENTPX:
         {
            switch(pst->event)
            {
#ifdef LCPJU
               case EVTPJUBNDREQ:
                  {
                     cmUnpkPjuBndReq(PjUiPjuBndReq, pst, mBuf);
                     break;
                  }

               case EVTPJUUBNDREQ:
                  {
                     cmUnpkPjuUbndReq(PjUiPjuUbndReq, pst, mBuf);
                     break;
                  }

               case EVTPJUDATREQ:
                  {
#ifdef SS_RBUF
                     cmUnpkFpPjuDatReq(PjUiPjuDatReq, pst, mBuf);
#else
                     cmUnpkPjuDatReq(PjUiPjuDatReq, pst, mBuf);
#endif
                     break;
                  }

               case EVTPJUDATFWDREQ:
                  {
                     cmUnpkPjuDatFwdReq(PjUiPjuDatFwdReq, pst, mBuf);
                     break;
                  }
#endif
               default:
                  SPutMsg(mBuf);
                  RLOG0(L_ERROR, " Received Invalid Event from Relay");
                  ret = RFAILED;
                  break;
            }
            break;
         }

      case ENTOC:
         {
            switch(pst->event)
            {
#ifdef PJ_CMP_ASYNC
               /* Compression Confirmations */
               case OBD_EVT_CMP_INIT_CFM:
                  ret = cmUnpkLibObdCmpInitCfm(PjLibObdCmpInitCfm, pst,mBuf);
                  break;

               case OBD_EVT_CMP_CFM:
                  ret = cmUnpkLibObdCmpCfm(PjLibObdCmpCfm, pst, mBuf);
                  break;

#endif /* PJ_CMP_ASYNC */
               default:
                  SPutMsg(mBuf);
                  RLOG0(L_ERROR, "Received Invalid Event from Offboard Ciphering");
                  ret = RFAILED;
                  break;
            }
            break;
         }

      case ENTOS:
         {
            switch(pst->event)
            {
#ifdef PJ_SEC_ASYNC
                  /* Integrity Confirmations */
               case OBD_EVT_INT_INIT_CFM:
                  ret = cmUnpkLibObdIntInitCfm(PjLibObdIntInitCfm, pst, mBuf);
                  break;

               case OBD_EVT_INT_PROT_CFM:
                  ret = cmUnpkLibObdIntProtCfm(PjLibObdIntProtCfm, pst, mBuf);
                  break;

                  /* Ciphering Confirmations */
               case OBD_EVT_CP_INIT_CFM:
                  ret = cmUnpkLibObdCpInitCfm(PjLibObdCpInitCfm, pst, mBuf);
                  break;

               case OBD_EVT_UP_INIT_CFM:
                  ret = cmUnpkLibObdUpInitCfm(PjLibObdUpInitCfm, pst, mBuf);
                  break;

               case OBD_EVT_CIPHER_CFM:
                  ret = cmUnpkLibObdCipherCfm(PjLibObdCipherCfm, pst, mBuf);
                  break;

#endif /* PJ_SEC_ASYNC */
               default:
                  SPutMsg(mBuf);
                  RLOG0(L_ERROR, "Received Invalid Event from Offboard Security");
                  ret = RFAILED;
                  break;
            }
            break;
         }
      case ENTKW:
         {
            switch(pst->event)
            {
#ifdef LCKWU
               case KWU_EVT_BND_CFM:     /* Bind request */
                  {
                     ret = cmUnpkKwuBndCfm(PjLiKwuBndCfm, pst, mBuf );
                     break;
                  }

                case KWU_EVT_DAT_CFM:    /* Dedicated Channel Data request */
                  {
                     ret = cmUnpkKwuDatCfm(PjLiKwuDatCfm, pst, mBuf);
                     break;
                  }

               case KWU_EVT_DISC_SDU_REQ:    /* Coomon Channel Status Response */
                  {
                     ret = cmUnpkKwuDiscSduReq(PjLiKwuDiscSduReq, pst , mBuf);
                     break;
                  }

               case KWU_EVT_STA_IND:    /* Dedicated Channel Status Response */
                  {
                     ret = cmUnpkKwuStaInd(PjLiKwuStaInd, pst, mBuf);
                     break;
                  }
               case KWU_EVT_DISC_SDU_CFM:    /* Harq status indication */
                  {
                     ret = cmUnpkKwuDiscSduCfm(PjLiKwuDiscSduCfm, pst, mBuf);
                     break;
                  }
               case KWU_EVT_FLOW_CNTRL_IND:    /* Flow control indication*/
                  {
                     ret = cmUnpkKwuFlowCntrlInd(PjLiKwuFlowCntrlInd, pst, mBuf);
                     break;
                  }
               default:
                  {
                     SPutMsg(mBuf);
                     RLOG0(L_ERROR, "Received Invalid Event from RLC");
                     ret = RFAILED;
                     break;
                   }

#endif  /* LCRGU */
            }
            break;
         }
#ifdef QCOM
#ifdef TENB_AS_SECURITY
      case ENTTF:
         {
            switch(pst->event)
            {
               case EVTPJUTTIIND:
               {
                  ysPjPrcTtiInd++; /* UDAY */
                  SPutMsg(mBuf);

                  PjUlSpaccTrigger();
                  /*PjDlSpaccTrigger();*/

                  /* Process L2 RLOG  */
                  rlProcessTicks();
                  break;
               }
               case OBD_EVT_INT_PROT_CFM:
			   {
                  ret = cmUnpkLibObdIntProtCfm(PjLibObdIntProtCfm, pst, mBuf);
                  break;
			   }
               case OBD_EVT_CIPHER_CFM:
               {
			      ret = cmUnpkLibObdCipherCfm(PjLibObdCipherCfm, pst, mBuf);
                  break;
               } 
            }
            break;
         }
#endif
#endif  /* QCOM */
#ifdef SS_RBUF
      case ENTTF:
      {
            switch(pst->event)
            {
               case EVTCTFBTCHPROCTICK:
               {
                  pjBatchProcTick();
                  break;
               }
               case EVTCTFNRTRBUFCLEAN:
               {
                  pjRBufFreeMsg();
               }
             }
         SPutMsg(mBuf);
         break;
      }
#endif
      default:
         {
            RLOG2(L_ERROR, "Recieved Invalid Src Entity [%d], Evnt[%d]",
                  pst->srcEnt,pst->event);
            QU_CRITLOG(QU_LOG_MESSAGE,"Recieved Invalid Source Entity ");
            QU_CRITLOG(QU_LOG_MESSAGE,"PDCP ActvTsk Src [%d], Evnt[%d]",pst->srcEnt,pst->event);
            SPutMsg(mBuf);
            ret = RFAILED;
            break;
         }
   }

   SExitTsk();

   RETVALUE(ret);
} /* pjActvTsk */

  
/********************************************************************30**
         End of file:     eq_dl_ex_ms.c@@/main/TeNB_Main_BR/12 - Mon Aug 11 16:42:46 2014
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
/main/1      ---      gk   1. Initial release.
/main/2      ---      gk   1. LTERLC Release 2.1
/main/3     kw005.201 ap   1. Added support for L2 Measurement.
*********************************************************************91*/
