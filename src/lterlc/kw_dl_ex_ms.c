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
  
        Name:    LTE-RLC Layer - System Services Interface Functions
    
        Type:    C file
  
        Desc:    C source code for the interface to System Services
                  of LTE-RLC
 
        File:    kw_ex_ms.c

        Sid:      gp_dl_ex_ms.c@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:43:15 2014
  
        Prg:     gk
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="RLC_DL";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=195;

/** @file gp_ex_ms.c
@brief RLC System Services Interface
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
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#include "kw_udx.h"
#include "kw_dl.h"


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
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#include "kw.x"
#include "kw_udx.x"
#include "kw_dl.x"

#include "ctf.h"
PUBLIC S16 kwUtlDlBatchProcPkts(Void);
PUBLIC S16 kwDlBatchProc(Void);
#if defined(MAC_RLC_HARQ_STA_RBUF)
U32 isDatReqProcessed;
PUBLIC void kwUtlDlBatchProcHqStaInd ARGS ((Void));
#endif

#if (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF))
EXTERN S16 kwDlBatchProcSplit  ARGS((Void));
#endif
//UDAY
#ifdef L2_OPTMZ
U32 kwAmmStaPduList[128] = {0};
EXTERN S16 ssGetDBufOfSize ARGS((Region region, Size size, Buffer **dBuf));
#endif
PUBLIC S16 kwDlInitExt ARGS (( Void ));

/**
 *
 * @brief
 *
 *  <b> Initialize External </b>
 *
 *  @b Description:
 *  Initializes variables used to interface with Upper/Lower Layer  
 *
 *  @return  S16
 *      -# ROK 
 *
*/
  
#ifdef ANSI
PUBLIC S16 kwDlInitExt 
(
)
#else
PUBLIC S16 kwDlInitExt()
#endif
{
   TRC2(kwDlInitExt);

   RETVALUE(ROK);
} /* kwInitExt */



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
 *    This function is invoked by system services to initialize the LTE-RLC
 *    layer. This is an entry point used by LTE_RLC layer to initialize its
 *    global variables, before becoming operational.
 *
 *    Allowable values for parameters are specified in ssi.h.
 *
 *    @param[in] ent    - Specify the entity id of the LTE-RLC task.
 *    @param[in] inst   - Specify the entity id of the LTE-RLC task.
 *    @param[in] region - Specifies the memory region from which
 *                         LTE-RLC should allocate structures and buffers.
 *    @param[in] reason - Specifies the reason for calling this
 *                         initialization function.
 *
 *  @return  S16
 *      -# ROK 
 *
 */
#ifdef ANSI
PUBLIC S16 kwDlActvInit
(
Ent    ent,                 /* entity */
Inst   inst,                /* instance */
Region region,              /* region */
Reason reason               /* reason */
)
#else
PUBLIC S16 kwDlActvInit(ent, inst, region, reason)
Ent    ent;                 /* entity */
Inst   inst;                /* instance */
Region region;              /* region */
Reason reason;              /* reason */
#endif
{
   KwCb    *tKwCb;
   TRC3(kwDlActvInit)

   if (inst >= KW_MAX_RLC_INSTANCES)
   {
       /* intance greater than MAX instances */ 
       RETVALUE(RFAILED); 
   }

   if (kwCb[inst] != NULLP)
   {
       RETVALUE (RFAILED);
   }
  
   if (SGetSBuf(region, 0, (Data **)&tKwCb,
                (Size)sizeof (KwCb)) != ROK)    
   {                     
      RETVALUE(RFAILED);
   }
   /* Initialize kwCb */
   KW_MEM_SET(tKwCb, 0, sizeof(KwCb));

   /* Initialize task configuration parameters */
   tKwCb->init.ent     = ent;           /* entity */
   tKwCb->init.inst    = inst;          /* instance */
   tKwCb->init.region  = region;        /* static region */
   tKwCb->init.pool    = 0;             /* static pool */
   tKwCb->init.reason  = reason;        /* reason */
   tKwCb->init.cfgDone = FALSE;         /* configuration done */
   tKwCb->init.acnt    = TRUE;          /* enable accounting */
   tKwCb->init.usta    = TRUE;          /* enable unsolicited status */
   tKwCb->init.trc     = FALSE;         /* enable trace */
   tKwCb->init.procId  = SFndProcId();

   kwCb[inst] = tKwCb;

//UDAY
#ifdef L2_OPTMZ
   for(int i = 0; i < 128; i++)
   {
      Buffer *mBuf = NULL;
      Buffer *bufPtr = NULL;
      SGetMsg(1, 0 , &mBuf);
      ssGetDBufOfSize(1 , 1800, &bufPtr);
      SUpdMsg(mBuf, bufPtr, 0);
      kwAmmStaPduList[i] = (U32)mBuf; 
   }
#endif
   /* call external function for intialization */
   /*
   kwInitExt();
   */

   

   RETVALUE(ROK);
} /* kwActvInit */


/**
 *
 * @brief
 *
 *  <b> Activation Task </b>
 *
 *  @b Description:
 *  Processes events received for MLTE-RLC layer via System Services from
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
PUBLIC S16 kwDlActvTsk
(
Pst *pst,              /* pst structure */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 kwDlActvTsk(pst, mBuf)
Pst *pst;              /* pst structure */
Buffer *mBuf;           /* message buffer */
#endif
{
   S16 ret = ROK;

   TRC3(kwDlActvTsk);

   switch(pst->srcEnt)
   {
      case ENTSM:
         {
            switch(pst->event)
            {
#ifdef LCLKW
               case LKW_EVT_CFG_REQ:
                  {
                     ret = cmUnpkLkwCfgReq(KwMiLkwCfgReq, pst, mBuf);
                     break;
                  }

               case LKW_EVT_CNTRL_REQ:
                  {
                     ret = cmUnpkLkwCntrlReq(KwMiLkwCntrlReq, pst, mBuf);
                     break;
                  }

               case LKW_EVT_STS_REQ:
                  {
                     ret = cmUnpkLkwStsReq(KwMiLkwStsReq, pst, mBuf);
                     break;
                  }

               case LKW_EVT_STA_REQ:
                  {
                     ret = cmUnpkLkwStaReq(KwMiLkwStaReq, pst, mBuf);
                     break;
                  }
                  /* kw005.201 added support for L2 Measurement */
#endif  /* LCLKW */
               default:
                  SPutMsg(mBuf);
                  if (pst->dstInst < KW_MAX_RLC_INSTANCES)
                  {
                      RLOG1(L_ERROR,"Received Invalid Event[%d] from SM",
                            pst->event);
                  }
                  ret = RFAILED;
                  break;

            }
            break;
         }

      case ENTKW:
         {

            switch(pst->event)
            {
#ifdef LCUDX
               case UDX_EVT_BND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkUdxBndReq(KwDlUdxBndReq, pst, mBuf );
                     break;
                  }

               case UDX_EVT_UBND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkUdxUbndReq(KwDlUdxUbndReq, pst, mBuf );
                     break;
                  }
               case UDX_EVT_CFG_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkUdxCfgReq(KwDlUdxCfgReq, pst, mBuf );
                     break;
                  }

               case UDX_EVT_UEIDCHG_REQ:              /* Configuration request */
                  {
                     ret = cmUnpkUdxUeIdChgReq(KwDlUdxUeIdChgReq, pst, mBuf);
                     break;
                  }

               case UDX_EVT_STA_UPD_REQ:              /* Configuration request */
                  {
                     ret = cmUnpkUdxStaUpdReq(KwDlUdxStaUpdReq, pst, mBuf);
                     break;
                  }

               case UDX_EVT_STA_PDU_REQ:              /* Configuration request */
                  {
                     ret = cmUnpkUdxStaPduReq(KwDlUdxStaPduReq, pst, mBuf);
                     break;
                  }

#ifdef LTE_L2_MEAS
               case UDX_EVT_L2MEAS_REQ:
                  {
                     ret = cmUnpkUdxL2MeasReq(KwDlUdxL2MeasReq, pst, mBuf);
                     break;
                  }
               case UDX_EVT_L2MEAS_SEND_REQ:
                 {

                    ret = cmUnpkUdxL2MeasSendReq(KwDlUdxL2MeasSendReq, pst, mBuf); 
  
                     break;
                 }
               case UDX_EVT_L2MEAS_STOP_REQ:
                 {
                     ret = cmUnpkUdxL2MeasStopReq(KwDlUdxL2MeasStopReq, pst, mBuf);
                     break;
                 }
#endif

#endif  /* LCCKW */
               case UDX_EVT_DL_CLEANUP_MEM:
                  {
                     kwUtlFreeDlMemory(KW_GET_KWCB(pst->dstInst));
                     break;
                  }
               
               default:
                  SPutMsg(mBuf);
                  if (pst->dstInst < KW_MAX_RLC_INSTANCES)
                  {
                      RLOG1(L_ERROR,"Received Invalid Event[%d] from RLC UL",
                            pst->event);
                  }
                  ret = RFAILED;
                  break;

            }
            break;
         }

      case ENTNH:
         {
            switch(pst->event)
            {
#ifdef LCKWU
               case KWU_EVT_BND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkKwuBndReq(KwUiKwuBndReq, pst, mBuf );
                     break;
                  }

               case KWU_EVT_UBND_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkKwuUbndReq(KwUiKwuUbndReq, pst, mBuf );
                     break;
                  }
#ifdef L2_L3_SPLIT
               case KWU_EVT_CPLANE_DAT_REQ:       /* C-Plane Data request */
                  {
                     ret = cmUnpkKwuDatReq(KwUiKwuDatReq, pst, mBuf);
                     break;
                  }
#else
               case KWU_EVT_DAT_REQ:              /* Data request */
                  {
                     ret = cmUnpkKwuDatReq(KwUiKwuDatReq, pst, mBuf);
                     break;
                  }
#endif
               case KWU_EVT_DISC_SDU_REQ:         /* Discard SDU request */
                  {
                     ret = cmUnpkKwuDiscSduReq(KwUiKwuDiscSduReq, pst, mBuf);
                     break;
                  }

#endif  /* LCKWU */
               default:
                  SPutMsg(mBuf);
                  if (pst->dstInst < KW_MAX_RLC_INSTANCES)
                  {
                      RLOG1(L_ERROR,"Received Invalid Event[%d] from RRC",
                            pst->event);
                  }
                  ret = RFAILED;
                  break;

            }
            break;
         }

      case ENTPJ:
         {
            switch(pst->event)
            {
#ifdef LCKWU
               case KWU_EVT_BND_REQ:              /* Bind request */
                  {
                     ret = cmUnpkKwuBndReq(KwUiKwuBndReq, pst, mBuf );
                     break;
                  }

               case KWU_EVT_UBND_REQ:             /* Unbind request */
                  {
                     ret = cmUnpkKwuUbndReq(KwUiKwuUbndReq, pst, mBuf );
                     break;
                  }
#ifdef L2_L3_SPLIT
               case KWU_EVT_CPLANE_DAT_REQ:       /* C-Plane Data request */
               case KWU_EVT_UPLANE_DAT_REQ:       /* U-Plane Data request */
                  {
                     ret = cmUnpkKwuDatReq(KwUiKwuDatReq, pst, mBuf);
                     break;
                  }
#else
               case KWU_EVT_DAT_REQ:              /* Data request */
                  {
                     ret = cmUnpkKwuDatReq(KwUiKwuDatReq, pst, mBuf);
                     break;
                  }
#endif
               case KWU_EVT_DISC_SDU_REQ:         /* Discard SDU request */
                  {
                     ret = cmUnpkKwuDiscSduReq(KwUiKwuDiscSduReq, pst, mBuf);
                     break;
                  }

               default:
                  SPutMsg(mBuf);
                  if (pst->dstInst < KW_MAX_RLC_INSTANCES)
                  {
                      RLOG1(L_ERROR,"Received Invalid Event[%d] from PDCP",
                            pst->event);
                  }
                  ret = RFAILED;
                  break;
#endif  /* LCKWU */
            }
            break;
         }

      case ENTRG:
         {
            switch(pst->event)
            {
#ifdef LCRGU
               case EVTRGUBNDCFM:     /* Bind request */
                  {
                     ret = cmUnpkRguBndCfm(KwLiRguBndCfm, pst, mBuf );
                     break;
                  }

               case EVTRGUCSTAIND:    /* Coomon Channel Status Response */
                  {
                     ret = cmUnpkRguCStaInd(KwLiRguCStaInd, pst, mBuf);
                     break;
                  }

               case EVTRGUDSTAIND:    /* Dedicated Channel Status Response */
                  {
                     ret = cmUnpkRguDStaInd(KwLiRguDStaInd, pst, mBuf);
                     break;
                  }
                  /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
               case EVTRGUHQSTAIND:    /* Harq status indication */
                  {
                     ret = cmUnpkRguHqStaInd(KwLiRguHqStaInd, pst, mBuf);
                     break;
                  }
#endif
               case EVTRGUFLOWCNTRLIND:
                  {
                     ret = cmUnpkRguFlowCntrlInd(KwLiRguFlowCntrlInd,pst,mBuf);
                     break;
                  }   
#endif  /* LCRGU */

               default:
                  SPutMsg(mBuf);
                  if (pst->dstInst < KW_MAX_RLC_INSTANCES)
                  {
                      RLOG1(L_ERROR,"Received Invalid Event[%d] from MAC",
                            pst->event);
                  }
                  ret = RFAILED;
                  break;
            }
            break;
         }
#ifdef SS_RBUF 
      case ENTTF:
      {
            switch(pst->event)
            {
               case EVTCTFBTCHPROCTICK:
               {
                  kwUtlDlBatchProcPkts();
                  break;
               }
             }
         SPutMsg(mBuf);
         break;
      }
#endif
      case ENTYS:
         {
            switch(pst->event)
            {
               case KWU_EVT_TTI_IND:
                  {
#if (defined(L2_L3_SPLIT) && defined(ICC_RECV_TSK_RBUF))
      kwDlBatchProcSplit();
#else 
#if defined(PDCP_RLC_DL_RBUF)
      kwDlBatchProc();
#endif
#endif

#if defined(SPLIT_RLC_DL_TASK) && defined(MAC_RLC_HARQ_STA_RBUF)
      //KwDlHarqStaBatchProc();
      kwUtlDlBatchProcHqStaInd();
#ifndef KWSELFPSTDLCLEAN
      kwUtlFreeDlMem();
#endif
#endif 
        
                     SPutMsg(mBuf);
                     break;
                  }
            }
            break;
         }


      default:
         {
            if (pst->dstInst < KW_MAX_RLC_INSTANCES)
            {
               /*KwCb *tKwCb = KW_GET_KWCB(pst->dstInst);*/
               RLOG1(L_ERROR, "Received Invalid Source Entity[%d]",
                     pst->event);
            }
            SPutMsg(mBuf);
            ret = RFAILED;
            break;
         }
    }
   SExitTsk();

   RETVALUE(ret);
} /* kwActvTsk */


  
/********************************************************************30**
         End of file:     gp_dl_ex_ms.c@@/main/TeNB_Main_BR/6 - Mon Aug 11 16:43:15 2014
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
