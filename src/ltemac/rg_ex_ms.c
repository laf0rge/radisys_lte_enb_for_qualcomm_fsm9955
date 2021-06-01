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
  
     File:     rg_ex_ms.c 
  
     Sid:      gk_ex_ms.c@@/main/3 - Sat Jul 30 02:21:28 2011
  
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
#include "rgu.h"           /* RGU defines */
#include "tfu.h"           /* RGU defines */
#include "lrg.h"           /* layer management defines for LTE-MAC */
#include "crg.h"           /* layer management defines for LTE-MAC */
#include "rg_sch_inf.h"           /* layer management defines for LTE-MAC */
#include "rg_env.h"        /* customisable defines and macros for LTE-MAC */
#include "rg.h"            /* defines and macros for MAC */


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
#include "rg_sch_inf.x"           /* layer management typedefs for MAC */
#include "rg.x"            /* typedefs for MAC */

/**
 * @brief Task Activation callback function Entity SM. 
 *
 * @details
 *
 *     Function : rgHdlSMEvents
 *     
 *     Process Messages received from Entity SM
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
#ifdef ANSI
PRIVATE INLINE void rgHdlSMEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlSMEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgHdlSMEvents)

   switch(pst->event)
   {
#ifdef LCRGMILRG
      case EVTLRGCFGREQ:
         /* Process a config. request */
         cmUnpkLrgCfgReq(RgMiLrgCfgReq, pst, mBuf);
         break;
      case EVTLRGCNTRLREQ:
         /* Process a control request */
         cmUnpkLrgCntrlReq(RgMiLrgCntrlReq, pst, mBuf);
         break;
      case EVTLRGSSTAREQ:
         /* Process a status request  */
         cmUnpkLrgStaReq(RgMiLrgStaReq, pst, mBuf);
         break;
      case EVTLRGSTSREQ:
         /* Process a statistics request */
         cmUnpkLrgStsReq(RgMiLrgStsReq, pst, mBuf);
         break;
#endif /* LCRGMILRG */
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}


/**
 * @brief Task Activation callback function Entity NH. 
 *
 * @details
 *
 *     Function : rgHdlNHEvents
 *     
 *     Process Messages received from Entity NH
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
#ifdef ANSI
PRIVATE INLINE void rgHdlNHEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlNHEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgHdlNHEvents)

   switch(pst->event)
   {
#ifdef LCRGUICRG
      case EVTCRGBNDREQ:
         cmUnpkCrgBndReq(RgUiCrgBndReq, pst, mBuf);
         break;
      case EVTCRGUBNDREQ:
         cmUnpkCrgUbndReq(RgUiCrgUbndReq, pst, mBuf);
         break;
      case EVTCRGCFGREQ:
         cmUnpkCrgCfgReq(RgUiCrgCfgReq, pst, mBuf);
         break;
#endif            
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}

/**
 * @brief Task Activation callback function Entity KW. 
 *
 * @details
 *
 *     Function : rgHdlKWEvents
 *     
 *     Process Messages received from Entity KW
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
#ifdef ANSI
PRIVATE INLINE void rgHdlKWEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlKWEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgHdlKWEvents)

   switch(pst->event)
   {
#ifdef LCRGUIRGU
      case EVTRGUBNDREQ:
         cmUnpkRguBndReq(RgUiRguBndReq, pst, mBuf);
         break;
      case EVTRGUUBNDREQ:
         cmUnpkRguUbndReq(RgUiRguUbndReq, pst, mBuf);
         break;
      case EVTRGUCDATREQ:
         cmUnpkRguCDatReq(RgUiRguCDatReq, pst, mBuf);
         break;
      case EVTRGUDDATREQ:
         cmUnpkRguDDatReq(RgUiRguDDatReq, pst, mBuf);
         break;
      case EVTRGUCSTARSP:
         cmUnpkRguCStaRsp(RgUiRguCStaRsp, pst, mBuf);
         break;
      case EVTRGUDSTARSP:
         cmUnpkRguDStaRsp(RgUiRguDStaRsp, pst, mBuf);
         break;
#ifdef LTE_L2_MEAS

      case EVTRGUL2MULTHRPMEASREQ:
         cmUnpkRguL2MUlThrpMeasReq(RgUiRguL2MUlThrpMeasReq, pst,mBuf);
         break;

#endif
#endif            
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}

/**
 * @brief Task Activation callback function Entity TF. 
 *
 * @details
 *
 *     Function : rgHdlTFEvents
 *     
 *     Process Messages received from Entity TF
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
#ifdef ANSI
PRIVATE INLINE void rgHdlTFEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlTFEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgHdlTFEvents)

   switch(pst->event)
   {
#if  (defined(LCRGLITFU) || defined(LWLCRGLITFU))
      case EVTTFUBNDCFM:
         cmUnpkTfuBndCfm(RgLiTfuBndCfm, pst, mBuf);
         break;
      case EVTTFUDATIND:
         cmUnpkTfuDatInd(RgLiTfuDatInd, pst, mBuf);
         break;
      case EVTTFUTTIIND:
         cmUnpkTfuTtiInd(RgLiTfuTtiInd, pst, mBuf);
         break;
#if defined(TENB_T2K3K_SPECIFIC_CHANGES) && defined(LTE_TDD)
      case EVTTFUNONRTIND:
         cmUnpkTfuNonRtInd(RgLiTfuNonRtInd, pst, mBuf);
         break;
#endif
#endif            
      default:
         RG_FREE_MSG(mBuf);
         break;
   }
}


/**
 * @brief Task Activation callback function Entity RG SCH. 
 *
 * @details
 *
 *     Function : rgHdlRGEvents
 *     
 *     Process Messages received from Entity RG SCH
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  void
 **/
#ifdef ANSI
PRIVATE INLINE void rgHdlRGEvents
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PRIVATE INLINE void rgHdlRGEvents(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgHdlRGEvents)

   switch(pst->event)
   {
#ifdef LCRG
      case EVTINFCELLREGREQ:
         cmUnpkSchMacCellRegReq(RgSchMacCellRegReq, pst, mBuf);
         break;
      case EVTINFSFALLOCREQ:
         cmUnpkSchMacSfAllocReq(RgSchMacSfAllocReq, pst, mBuf);
         break;
      case EVTINFRLSHQREQ:
         cmUnpkSchMacRlsHqReq(RgSchMacRlsHqReq, pst, mBuf);
         break;
      case EVTINFRLSRNTIREQ:
         cmUnpkSchMacRlsRntiReq(RgSchMacRlsRntiReq, pst, mBuf);
         break;
#ifdef LTEMAC_SPS
      case EVTINFSPSLCREG:
         cmUnpkSchMacSpsLcRegReq(RgSchMacSpsLcRegReq, pst, mBuf);
         break;
      case EVTINFSPSLCDEREG:
         cmUnpkSchMacSpsLcDeregReq(RgSchMacSpsLcDeregReq, pst, mBuf);
         break;
      case EVTINFSPSRESET:
         cmUnpkSchMacUlSpsResetReq(RgSchMacUlSpsResetReq, pst, mBuf);
         break;
#endif /* LTEMAC_SPS */
#ifdef LTE_L2_MEAS
      case EVTINFL2MEASREQ:
         cmUnpkSchMacL2MeasReq(RgSchMacL2MeasReq, pst, mBuf);
         break;
      case EVTINFL2MEASSENDREQ :
         cmUnpkSchMacL2MeasSendReq(RgSchMacL2MeasSendReq, pst , mBuf);
         break;
      case EVTINFL2MEASSTOPREQ:
         cmUnpkSchMacL2MeasStopReq(RgSchMacL2MeasStopReq, pst , mBuf);
         break;
#endif/* LTE_L2_MEAS */
      case EVTINFLCGREG:
         cmUnpkSchMacLcgRegReq(RgSchMacLcgRegReq, pst, mBuf);
#endif            
      default:
         RG_FREE_MSG(mBuf);
         break;

   }
}



/**
 * @brief Task Activation callback function. 
 *
 * @details
 *
 *     Function : rgActvTsk
 *     
 *     Primitives invoked by MAC's users/providers through
 *     a loosely coupled interface arrive here by means of 
 *     SSI's message handling. This API is registered with
 *     SSI during the Task Registration of MAC.
 *     
 *  @param[in]  Pst     *pst, Post structure of the primitive.     
 *  @param[in]  Buffer *mBuf, Packed primitive parameters in the buffer.
 *  @param[in]  Reason reason.
 *  @return  S16
 *      -# ROK
 **/
#ifdef ANSI
PUBLIC S16 rgActvTsk
(
Pst     *pst,                       /* post structure       */
Buffer  *mBuf                       /* message buffer       */
)
#else
PUBLIC S16 rgActvTsk(pst, mBuf)
Pst     *pst;                       /* post structure       */
Buffer  *mBuf;                      /* message buffer       */
#endif
{
   TRC2(rgActvTsk)

   switch(pst->srcEnt)
   {
      /* The originator of this message is the stack manager,
       * unpack and go to the respective primitive processing function */
      case ENTSM:
          rgHdlSMEvents(pst, mBuf);
           break;
      case ENTNH:
          rgHdlNHEvents(pst, mBuf);
          break;
      case ENTKW:
          rgHdlKWEvents(pst, mBuf);
          break;
      case ENTTF:
          rgHdlTFEvents(pst, mBuf);
          break;
      case ENTRG: /* When scheduler instance sends msg to MAC */
          rgHdlRGEvents(pst, mBuf);
          break;
       default:
          RG_FREE_MSG(mBuf);
          break;
   }
   SExitTsk();
   RETVALUE(ROK);
}/* end of rgActvTsk */


/**********************************************************************
 
         End of file:     gk_ex_ms.c@@/main/3 - Sat Jul 30 02:21:28 2011
 
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
/main/1      ---     sm                1. Initial Release.
/main/2      ---     sm                1. LTE MAC 2.1 release
         rg003.201   ns                1. Trace added for functions.
         rg008.201   rsharon           1. Added support for SPS.
/main/3      ---        gvj  1. Updated for LTE MAC Rel 3.1         
$SID$        ---     rt      1. LTE MAC 4.1 release
*********************************************************************91*/
