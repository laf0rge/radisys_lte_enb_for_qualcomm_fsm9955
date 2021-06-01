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
  
     Desc:     C source code SSI Interface Implementation
  
     File:     rg_sch_ex_ms.c 
  
     Sid:      gk_sch_ex_ms.c@@/main/2 - Sat Jul 30 02:21:41 2011
  
     Prg:      sm
  
**********************************************************************/

/** @file rg_ex_ms.c
@brief This file contains the implementation of callback functions 
registered with SSI during the LTE MAC Task initialization.
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
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "rgr.h"           /* layer management defines for LTE-MAC */
#include "rgm.h"           /* layer management defines for LTE-MAC */
#include "rg_env.h"        /* customisable defines and macros for LTE-MAC */
#include "rg_sch_err.h"        /* defines and macros for Scheduler */
#include "rg_sch_inf.h"        /* defines and macros for Scheduler */
#include "rg_sch.h"        /* defines and macros for Scheduler */


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
#include "tfu.x"           /* RGU types */
#include "lrg.x"           /* layer management typedefs for MAC */
#include "rgr.x"           /* layer management typedefs for MAC */
#include "rgm.x"           /* layer management typedefs for MAC */
#include "rg_sch_inf.x"        /* typedefs for Scheduler */
#include "rg_sch.x"        /* typedefs for Scheduler */




/**
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *     Function : schActvTsk
 *     
 *     Primitives invoked by MAC's users/providers through
 *     a loosely coupled interface arrive here by means of 
 *     SSI's message handling. This API is registered with
 *     SSI during the Task Registration of MAC.
 *     
 *  @param[in]  Pst     *pst, post structure of the Primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 schActvTsk
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PUBLIC S16 schActvTsk(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(schActvTsk)

   switch(pst->srcEnt)
   {
      /* The originator of this message is the stack manager,
       * unpack and go to the respective primitive processing function */
      case ENTSM:
         switch(pst->event)
         {
#ifdef LCRGMILRG
            case EVTLRGSCHCFGREQ:
               /* Process a config. request */
               cmUnpkLrgSchCfgReq(RgMiLrgSchCfgReq, pst, mBuf);
               break;
            case EVTLRGSCHCNTRLREQ:
               /* Process a control request */
               cmUnpkLrgSchCntrlReq(RgMiLrgSchCntrlReq, pst, mBuf);
               break;
            case EVTLRGSCHSTAIND:
               /* Process a control request */
               cmUnpkLrgSchStaInd(RgMiLrgSchStaInd, pst, mBuf);
               break;
#ifdef LTE_L2_MEAS
            case EVTLRGSCHL2MEASREQ:
               /* Process L2 Measurement request */
               cmUnpkLrgSchL2MeasReq(RgMiLrgSchL2MeasReq, pst, mBuf);
               break;
            case EVTLRGSCHL2MEASSTOPREQ:
               /* Process L2 Measurement Stop request */
               cmUnpkLrgSchL2MeasStopReq(RgMiLrgSchL2MeasStopReq, pst, mBuf);
               break;
            case EVTLRGSCHL2MEASSENDREQ:
               /* Process L2 Measurement Send  request */
               cmUnpkLrgSchL2MeasSendReq(RgMiLrgSchL2MeasSendReq, pst, mBuf);
               break;
#endif
#endif /* LCRGMILRG */
            default:
               RGSCH_FREE_MSG(mBuf);
               break;
         }
         break;
     case ENTNX:
         switch(pst->event)
         {
#ifdef LCRGUIRGR
            case EVTRGRBNDREQ:
               cmUnpkRgrBndReq(RgUiRgrBndReq, pst, mBuf);
               break;
            case EVTRGRUBNDREQ:
               cmUnpkRgrUbndReq(RgUiRgrUbndReq, pst, mBuf);
               break;
            case EVTRGRCFGREQ:
               cmUnpkRgrCfgReq(RgUiRgrCfgReq, pst, mBuf);
               break;
#ifdef RGR_SI_SCH
            case EVTRGRSICFGREQ:
               cmUnpkRgrSiCfgReq(RgUiRgrSiCfgReq, pst, mBuf);
               break;
            case EVTRGRWARNINGSICFGREQ:
               cmUnpkRgrWarningSiCfgReq(RgUiRgrWarningSiCfgReq, pst, mBuf);
               break;

            case EVTRGRWARNINGSISTOPREQ:
               cmUnpkRgrWarningSiStopReq(RgUiRgrWarningSiStopReq, pst, mBuf);
               break;
#endif/*RGR_SI_SCH */
               /* LTE_ADV_FLAG_REMOVED_START */
            case EVTRGRLOADINFREQ:
               cmUnpkRgrLoadInfReq(RgUiRgrLoadInfReq, pst, mBuf);
               break;
               /* LTE_ADV_FLAG_REMOVED_END */
#endif            
            default:
               RGSCH_FREE_MSG(mBuf);
               break;
         }
         break;
      case ENTTF:
         switch(pst->event)
         {
/*#ifdef LCRGLITFU L2Split */
#if (defined(LCRGLITFU) || defined(LWLCRGLITFU)) 
            case EVTTFUSCHBNDCFM:
               cmUnpkTfuBndCfm(RgLiTfuSchBndCfm, pst, mBuf);
               break;
            case EVTTFURAREQIND:
               cmUnpkTfuRaReqInd(RgLiTfuRaReqInd, pst, mBuf);
               break;
            case EVTTFUULCQIIND:
               cmUnpkTfuUlCqiInd(RgLiTfuUlCqiInd, pst, mBuf);
               break;
            case EVTTFUHQIND:
               cmUnpkTfuHqInd(RgLiTfuHqInd, pst, mBuf);
               break;
            case EVTTFUSRIND:
               cmUnpkTfuSrInd(RgLiTfuSrInd, pst, mBuf);
               break;
            case EVTTFUDLCQIIND:
               cmUnpkTfuDlCqiInd(RgLiTfuDlCqiInd, pst, mBuf);
               break;
            case EVTTFUCRCIND:
               /*cmUnpkTfuCrcIndInfo(RgLiTfuCrcInd, pst, mBuf); */
               cmUnpkTfuCrcInd(RgLiTfuCrcInd, pst, mBuf);
               break;
            case EVTTFUTIMINGADVIND:
               cmUnpkTfuTimingAdvInd(RgLiTfuTimingAdvInd, pst, mBuf);
               break;
            case EVTTFUSCHTTIIND:
               cmUnpkTfuSchTtiInd(RgLiTfuSchTtiInd, pst, mBuf);
               break;
            case EVTTFUPUCCHDELPWR:
               cmUnpkTfuPucchDeltaPwr(RgLiTfuPucchDeltaPwrInd, pst, mBuf);
               break;
            case EVTTFUDOAIND:
               cmUnpkTfuDoaInd(RgLiTfuDoaInd, pst, mBuf);
               break;
#ifdef TFU_UPGRADE
            case EVTTFURAWCQIIND:
               cmUnpkTfuRawCqiInd(RgLiTfuRawCqiInd, pst, mBuf);
               break;
            case EVTTFUSRSIND:
               cmUnpkTfuSrsInd(RgLiTfuSrsInd, pst, mBuf);
               break;
#endif 
#endif            
            default:
               RGSCH_FREE_MSG(mBuf);
               break;
         }
         break;
      case ENTRG: /* When MAC sends a msg to Scheduler instance */
         switch(pst->event)
         {
#ifdef LCSCH
            case EVTINFDEDBOUPDTREQ:
               cmUnpkMacSchDedBoUpdtReq(RgMacSchDedBoUpdtReq, pst, mBuf);
               break;
            case EVTINFCMNBOUPDTREQ:
               cmUnpkMacSchCmnBoUpdtReq(RgMacSchCmnBoUpdtReq, pst, mBuf);
               break;   
            case EVTINFSFRECPIND:
               cmUnpkMacSchSfRecpInd(RgMacSchSfRecpInd, pst, mBuf);
               break;
               /*Fix: start: Inform UE delete to scheduler*/
            case EVTINFUEDELIND:
               cmUnpkMacSchUeDelInd(RgMacSchUeDelInd, pst, mBuf);
               break;
            /*Fix: end: Inform UE delete to scheduler*/
#ifdef LTE_L2_MEAS
            case EVTINFL2MEASCFM:
               cmUnpkMacSchL2MeasCfm(RgMacSchL2MeasCfm, pst, mBuf);
               break;
            case EVTINFL2MEASSTOPCFM:
               cmUnpkMacSchL2MeasCfm(RgMacSchL2MeasStopCfm, pst, mBuf);
               break;
#endif
#endif            
            default:
               RGSCH_FREE_MSG(mBuf);
               break;
         }
         break;
      case ENTRM: /* When RRM sends msg to scheduler */
         switch(pst->event)
         {
            case EVTRGMBNDREQ:
               cmUnpkRgmBndReq(RgUiRgmBndReq, pst, mBuf); 
               break;
            case EVTRGMUBNDREQ:
               cmUnpkRgmUbndReq(RgUiRgmUbndReq, pst, mBuf); 
               break;
            case EVTRGMCFGPRBRPRT:
               cmUnpkRgmCfgPrbRprt(RgUiRgmCfgPrbRprt, pst, mBuf);
               break;
            default:
               RGSCH_FREE_MSG(mBuf);
               break;
         }
         break;
      default:
          RGSCH_FREE_MSG(mBuf);
          break;
   }
   SExitTsk();
   RETVALUE(ROK);
}/* end of schActvTsk */


/**********************************************************************
 
         End of file:     gk_sch_ex_ms.c@@/main/2 - Sat Jul 30 02:21:41 2011
 
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
/main/1      ---     sm                1. LTE MAC 2.1 release
           rg003.201 ns                1. Trace added for function.
           rg004.201 rnt               1. Modified for SI Enhancement
           rg007.201   ap  1. Added support for MIMO
           rg008.201 rsharon  1. Added support for SPS.
           rg009.201 dv 1. Added Changes of TFU_UPGRADE.
/main/2      ---        gvj  1. Updated for LTE MAC Rel 3.1
           rg002.301 av      1.ccpu00119825 changes
$SID$        ---     rt             1. LTE MAC 4.1 release
*********************************************************************91*/
