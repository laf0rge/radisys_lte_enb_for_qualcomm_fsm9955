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

Sid:      eq_ul_ex_ms.c@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:42:58 2014

Prg:    

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="PDCP";
static int RLOG_MODULE_ID=1024;
static int RLOG_FILE_ID=242;
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
#include "pju.h"           /* PJU defines */
#include "lpj.h"           /* LPJ defines */
#include "pj_err.h"        /* Err defines */
#include "pj_env.h"        /* RLC environment options */
#include "pj.h"            /* RLC defines */
#include "pj_udx.h"
#include "pj_dl.h"        
#include "pj_lib.h"

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
#include "pj_lib.x"
#ifdef TENB_STATS
#include "pj_tenb_stats.x"
#endif

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
PUBLIC S16 pjUlActvInit
(
 Ent    ent,                 /* entity */
 Inst   inst,                /* instance */
 Region region,              /* region */
 Reason reason               /* reason */
 )
#else
PUBLIC S16 pjUlActvInit(ent, inst, region, reason)
  Ent    ent;                 /* entity */
  Inst   inst;                /* instance */
  Region region;              /* region */
  Reason reason;              /* reason */
#endif
{
   PjCb   *tPjCb;
   TRC3(pjUlActvInit)
         
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
   tPjCb->init.dbgMask = 0x00000;
#endif
   tPjCb->init.procId  = SFndProcId();

   pjCb[inst] = tPjCb;
#if (defined(TENB_STATS) && !(defined(L2_L3_SPLIT)))
   TSL2AllocStatsMem(tPjCb->init.region, tPjCb->init.pool); 
#endif

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
PUBLIC S16 pjUlActvTsk
(
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 pjUlActvTsk(pst, mBuf)
Pst *pst;              /* pst structure */
Buffer *mBuf;           /* message buffer */
#endif
{
   S16 ret = ROK;

   TRC3(pjUlActvTsk);

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
#ifdef LTE_L2_MEAS
               case LPJ_EVT_L2MEAS_REQ:
                  {
                     ret = cmUnpkLpjL2MeasReq(PjMiLpjL2MeasReq, pst, mBuf);
                     break;
                  }
               case LPJ_EVT_L2MEAS_SEND_REQ:
                 {

                    ret = cmUnpkLpjL2MeasSendReq(PjMiLpjL2MeasSendReq, pst, mBuf); 
  
                     break;
                 }
               case LPJ_EVT_L2MEAS_STOP_REQ:
                 {
                     ret = cmUnpkLpjL2MeasStopReq(PjMiLpjL2MeasStopReq, pst, mBuf);
                     break;
                 }
#endif

               default:
                  {
                     RLOG1(L_ERROR, "Received Invalid Event[%d] from SM", pst->event);
                     ret = RFAILED;
                     break;
                  }
#endif  /* LCLPJ */
            }
            break;
         }

      case ENTWR:
         {
            switch(pst->event)
            {

               case EVTCPJKDFREQ:
               {
                   cmUnpkCpjKdfReq(PjUiCpjKdfReq, pst, mBuf);
                  break;
               }

            default:
                  SPutMsg(mBuf);
                   RLOG1(L_ERROR, "Received Invalid Event[%d] from APP", pst->event);
                  ret = RFAILED;
                  break;
            }
            break;
         }

      case ENTPJ:
         {
            switch(pst->event)
            {
#ifdef LCUDX
            case EVTUDXBNDCFM:
                  {
                     cmUnpkUdxBndCfm(PjUlUdxBndCfm, pst, mBuf);
                     break;
                  }

            case EVTUDXCFGCFM:
                  {
                     cmUnpkUdxCfgCfm(PjUlUdxCfgCfm, pst, mBuf);
                     break;
                  }

            case EVTUDXREESTCFM:
                  {
                     cmUnpkUdxReEstCfm(PjUlUdxReEstCfm, pst, mBuf);
                     break;
                  }

            case EVTUDXSECCFGCFM:
                  {
                     cmUnpkUdxSecCfgCfm(PjUlUdxSecCfgCfm, pst, mBuf);
                     break;
                  }

            case EVTUDXUEIDCHGCFM:
                  {
                     cmUnpkUdxUeIdChgCfm(PjUlUdxUeIdChgCfm, pst, mBuf);
                     break;
                  }

            case EVTUDXCOUNTCFM:
                  {
                     cmUnpkUdxCountCfm(PjUlUdxCountCfm, pst, mBuf);
                     break;
                  }

            case EVTUDXSDUSTACFM:
                  {
                     cmUnpkUdxSduStaCfm(PjUlUdxSduStaCfm, pst, mBuf);
                     break;
                  }
            case EVTUDXDATRESUMECFM:
                  {
                     cmUnpkUdxDatResumeCfm(PjUlUdxDatResumeCfm, pst, mBuf);
                     break;
                  }
            case EVTUDXDATFRWDCFM:
                  {
                     cmUnpkUdxDatForwardCfm(PjUlUdxDatForwardCfm, pst, mBuf);
                     break; 
                  }                  
#endif /* LCCPJ */
            case EVTCPJSTARTDATAFWDREQ: /* Start Data Forwarding Request from RRC user for Source eNB during HO */
                  {
                      cmUnpkCpjStartDataFwdReq(PjUiCpjDatForwardReq, pst, mBuf);
                      break;
                  }
              default:
                  SPutMsg(mBuf);
                  RLOG1(L_ERROR, "Received Invalid Event[%d] from PDCP DL", pst->event);
                  ret = RFAILED;
                  break;

            }
            break;
         }


      case ENTNH:
         {
            switch(pst->event)
            {
#ifdef LCCPJ
            case EVTCPJBNDREQ:
                  {
                     cmUnpkCpjBndReq(PjUiCpjBndReq, pst, mBuf);
                     break;
                  }

            case EVTCPJUBNDREQ:
                  {
                     cmUnpkCpjUbndReq(PjUiCpjUbndReq, pst, mBuf);
                     break;
                  }

            case EVTCPJCFGREQ:
                  {
                     cmUnpkCpjCfgReq(PjUiCpjCfgReq, pst, mBuf);
                     break;
                  }

            case EVTCPJREESTREQ:
                  {
                     cmUnpkCpjReEstReq(PjUiCpjReEstReq, pst, mBuf);
                     break;
                  }

            case EVTCPJSECCFGREQ:
                  {
                     cmUnpkCpjSecCfgReq(PjUiCpjSecCfgReq, pst, mBuf);
                     break;
                  }

            case EVTCPJUEIDCHGREQ:
                  {
                     cmUnpkCpjUeIdChgReq(PjUiCpjUeIdChgReq, pst, mBuf);
                     break;
                  }

            case EVTCPJCOUNTREQ:
                  {
                     cmUnpkCpjCountReq(PjUiCpjCountReq, pst, mBuf);
                     break;
                  }

            case EVTCPJSDUSTAREQ:
                  {
                     cmUnpkCpjSduStaReq(PjUiCpjSduStaReq, pst, mBuf);
                     break;
                  }
              case EVTCPJSTARTDATAFWDREQ: /* Start Data Forwarding Request from RRC user for Source eNB during HO */
                  {
                      cmUnpkCpjStartDataFwdReq(PjUiCpjDatForwardReq, pst, mBuf);
                      break;
                  }
              case EVTCPJDATRESUMEREQ: /* Data Resume Request from RRC user during Re-establishment */
                  {
                      cmUnpkCpjDatResumeReq(PjUiCpjDatResumeReq, pst, mBuf);
                      break;
                  }
#endif /* LCCPJ */
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

              case EVTPJUDATFWDREQ:
                  {
                     cmUnpkPjuDatFwdReq(PjUiPjuDatFwdReq, pst, mBuf);
                     break;
                  }
#endif
              default:
                  SPutMsg(mBuf);
                  RLOG1(L_ERROR, "Received Invalid Event[%d] from RRC",pst->event);
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
                     cmUnpkPjuDatReq(PjUiPjuDatReq, pst, mBuf);
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
                  RLOG1(L_ERROR, "Received Invalid Event[%d] from Relay", pst->event);
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
                  ret = cmUnpkLibObdCmpInitCfm(PjLibObdCmpInitCfm, pst, mBuf);
                  break;

               case OBD_EVT_DECMP_CFM:
                  ret = cmUnpkLibObdDecmpCfm(PjLibObdDecmpCfm, pst, mBuf);
                  break;
#endif /* PJ_CMP_ASYNC */
               default:
                  SPutMsg(mBuf);
                  RLOG1(L_ERROR, "Received Invalid Event[%d] from Offboard Compression",pst->event);
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

               case OBD_EVT_INT_VER_CFM:
                  ret = cmUnpkLibObdIntVerCfm(PjLibObdIntVerCfm, pst, mBuf);
                  break;

                  /* Ciphering Confirmations */
               case OBD_EVT_CP_INIT_CFM:
                  ret = cmUnpkLibObdCpInitCfm(PjLibObdCpInitCfm, pst, mBuf); /* TODO - Only Dl path? */
                  break;

               case OBD_EVT_UP_INIT_CFM:
                  ret = cmUnpkLibObdUpInitCfm(PjLibObdUpInitCfm, pst, mBuf); /* TODO - Only Dl path? */
                  break;

               case OBD_EVT_DECIPHER_CFM:
                  ret = cmUnpkLibObdDecipherCfm(PjLibObdDecipherCfm, pst, mBuf);
                  break;
#endif /* PJ_SEC_ASYNC */
               default:
                  SPutMsg(mBuf);
                  RLOG1(L_ERROR, "Received Invalid Event[%d] from Offboard Security",pst->event);
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

               case KWU_EVT_DAT_IND:    /* Coomon Channel Data request */
                  {
                     ret = cmUnpkKwuDatInd(PjLiKwuDatInd, pst, mBuf);
                     break;
                  }

               case KWU_EVT_REEST_CMP_IND:    /* Dedicated Channel Status Response */
                  {
                     ret = cmUnpkKwuReEstCmpInd(PjLiKwuReEstCmpInd, pst, mBuf);
                     break;
                  }
#ifdef LTE_L2_MEAS
               case KWU_EVT_DAT_ACK_IND:    /* Harq status indication */
                  {
                     ret = cmUnpkKwuDatAckInd(PjLiKwuDatAckInd, pst, mBuf);
                     break;
                  }
#endif
               default:
                  {
                     SPutMsg(mBuf);
                     RLOG1(L_ERROR, "Received Invalid Event[%d] from RLC", pst->event);
                     ret = RFAILED;
                     break;
                   }

#endif  /* LCRGU */
            }
            break;
         }
      case ENTTF:
         {
            switch(pst->event)
            {
               case OBD_EVT_INT_VER_CFM:
                  ret = cmUnpkLibObdIntVerCfm(PjLibObdIntVerCfm, pst, mBuf);
                  break;
               case OBD_EVT_DECIPHER_CFM:
                  ret = cmUnpkLibObdDecipherCfm(PjLibObdDecipherCfm, pst, mBuf);
                  break;
#ifdef TENB_STATS
               case TENBSTATSINIT:
               {
                  PjCb *tPjCb;
                  tPjCb = PJ_GET_PJCB(pst->dstInst);
                  TSL2SendStatsToApp(&(tPjCb->pjGenCfg.lmPst), 0);
                  SPutMsg(mBuf);
                  break;
               }
#endif
               default:
               {
                  printf("\n ERROR Invalid Event[%d] from CL to PDCPUL\n", 
                         pst->event);
                  SPutMsg(mBuf);
                  ret = RFAILED;
                  break;
               }
            }
            break;
         }
      default:
         {
            RLOG1(L_ERROR, "Received Invalid Src Entity[%d]", pst->event);
            SPutMsg(mBuf);
            ret = RFAILED;
            break;
         }
   }

   SExitTsk();

   RETVALUE(ret);
} /* pjActvTsk */

  
/********************************************************************30**
         End of file:     eq_ul_ex_ms.c@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:42:58 2014
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
