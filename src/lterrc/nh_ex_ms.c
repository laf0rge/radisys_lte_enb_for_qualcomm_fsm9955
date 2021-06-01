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
 
     Name:     LTE RRC layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions.
  
     File:     nh_ex_ms.c
  
     Sid:      ni_ex_ms.c@@/main/3 - Fri Jul  1 01:13:18 2011
  
     Prg:      vk
  
**********************************************************************/
/* Header include files -- defines (.h) */
static const char* RLOG_MODULE_NAME="RRC";
static int RLOG_MODULE_ID=512;
static int RLOG_FILE_ID=88;
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */
  
#include "gen.h"           /* General */
#include "ssi.h"           /* System services */

#include "cm_tkns.h"       /* Common Token defines */
#include "cm_mblk.h"       /* Common Mem Allocation defines */
#include "cm_llist.h"      /* Common Link List defines */
#include "cm_hash.h"       /* Common Hashlist defines */
#include "cm_lte.h"        /* Common Lte defines */

#include "nhu.h"           /* NHU Interface defines */
#include "lnh.h"           /* LNH Interface defines */
#include "kwu.h"           /* KWU Interface defines */
#include "ckw.h"           /* CKW Interface defines */
#include "crg.h"           /* CRG Interface defines */
#include "cpj.h"           /* CPJ Interface defines */
#include "pju.h"           /* PJU Interface defines */
#ifdef RRC_PHY_CFG_SUPP
#include "ctf.h"           /* PHY Interface defines */
#endif /* RRC_PHY_CFG_SUPP */
#include "nh.h"            /* RRC defines */

/* Header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */
#include "cm5.x"
#include "cm_tkns.x"       /* Common Token definitions */
#include "cm_mblk.x"       /* Common Memory Allocation */
#include "cm_llist.x"      /* Common Link List definitions */
#include "cm_hash.x"       /* Common Hashlist definitions */
#include "cm_lte.x"        /* Common Lte definitions */

#include "nhu.x"           /* NHU Interface includes */
#include "lnh.x"           /* LNH Interface includes */
#include "kwu.x"           /* KWU Interface defines */
#include "ckw.x"           /* CKW Interface defines */
#include "crg.x"           /* CRG Interface defines */
#include "pju.x"           /* PJU Interface defines */
#include "cpj.x"           /* CPJ Interface defines */
#ifdef RRC_PHY_CFG_SUPP
#include "ctf.x"           /* CTF Interface defines */
#endif /* RRC_PHY_CFG_SUPP */
#include "nh.x"            /* RRC includes */


/* local defines */

/* local typedefs */
 
/* local externs */
 
/* forward references */


/************************************************************************
 *
 *      Fun:   nhActvTsk
 *
 *      Desc:  This is the entry point for all loosely coupled messages
 *             that have the RRC as the destination. This includes layer
 *             manager messages, upper and Lower layer(s) messages.
 *
 *      Ret:   ROK - ok
 *
 *      Notes: None
 *
 *      File:  nh_ex_ms.c
 *
 ************************************************************************/
#ifdef ANSI
PUBLIC S16 nhActvTsk
(
Pst     *post,                       /* Post structure */
Buffer  *mBuf                        /* message buffer */
)
#else /* ANSI */
PUBLIC S16 nhActvTsk(post, mBuf)
Pst     *post;                       /* Post structure */
Buffer  *mBuf;                       /* message buffer */
#endif /* ANSI */
{

   Mem      sMem;
   Size     maxBlkSize;
   Pst      *pst;
   S16      ret; 
   TRC3(nhActvTsk);
   
   sMem.region = nhCb.init.region;
   sMem.pool = nhCb.init.pool;

   /* ICNC: define NH_MEM_PDU_SIZE */
   maxBlkSize = NH_MEM_PDU_SIZE;

   pst = post;

   switch(pst->srcEnt)
   {
      
#ifdef LCNHMILNH
      case ENTSM: /* Layer Manager events */
      {
         switch(pst->event)
         {
            case EVTLNHCFGREQ:
               /* Process the Config request from Layer Manager */
               cmUnpkLnhCfgReq(NhMiLnhCfgReq, pst, mBuf);
               break;
            case EVTLNHCNTRLREQ:
               /* Process the Control request from Layer Manager */
               cmUnpkLnhCntrlReq(NhMiLnhCntrlReq, pst, mBuf);
               break;
            case EVTLNHSTSREQ:
               /* Process the statistics request from Layer Manager */
               cmUnpkLnhStsReq(NhMiLnhStsReq, pst, mBuf);
               break;
            case EVTLNHSTAREQ:
               /* Process the status request from Layer Manager */
               cmUnpkLnhStaReq(NhMiLnhStaReq, pst, mBuf);
               break;
            default:
               /* Invalid event */ 
               NH_FREEMBUF(mBuf);
               RLOG1(L_ERROR, "Invalid event [%d] from SM", pst->event);
               break;
         } /* end of switch on pst->event */
         break;
      } /* end of case ENTSM */
#endif /* LCNHMILNH */

#ifdef LCNHUINHU
      case ENTNX :
      {
         switch(pst->event) /* RRC User events */ 
         {
            case EVTNHUBNDREQ : 
               cmUnpkNhuBndReq(NhUiNhuBndReq, pst, mBuf);
               break;
            case EVTNHUUBNDREQ : 
               cmUnpkNhuUbndReq(NhUiNhuUbndReq, pst, mBuf); 
               break;
            case EVTNHUDATREQ:
               cmUnpkNhuDatReq(NhUiNhuDatReq, pst, mBuf, &sMem, maxBlkSize);
               break;
            case  EVTNHUDATRSP:
               cmUnpkNhuDatRsp(NhUiNhuDatRsp, pst, mBuf, &sMem, maxBlkSize);
               break;
            case  EVTNHUCFGREQ:
               cmUnpkNhuCfgReq(NhUiNhuCfgReq, pst, mBuf, &sMem, maxBlkSize);
               break;
            case  EVTNHUCNCLUEREQ:
               cmUnpkNhuCnclUeReq(NhUiNhuCnclUeReq, pst, mBuf);
               break;
            case  EVTNHUCELLCFGREQ:
               cmUnpkNhuCellCfgReq(NhUiNhuCellCfgReq, pst, mBuf, &sMem, 
                                    maxBlkSize);
               break;
            case  EVTNHUDELETETRANSREQ:
               cmUnpkNhuDeleteTransReq(NhUiNhuDeleteTransReq, pst, mBuf);
               break;
            case  EVTNHUENCODEREQ:
               cmUnpkNhuEncodeReq(NhUiNhuEncodeReq, pst, mBuf, &sMem, 
                                    maxBlkSize);
               break;
            case  EVTNHUPDCPSDUSTAREQ:
               cmUnpkNhuPdcpSduStaReq(NhUiNhuPdcpSduStaReq, pst, mBuf);
               break;
            case  EVTNHUSTARTPDCPDATAFWDREQ:
               cmUnpkNhuStartPdcpDataFwdReq(NhUiNhuStartPdcpDataFwdReq, pst, mBuf);
               break;
 
            case  EVTNHUPDCPDATRESUMEREQ:
               cmUnpkNhuPdcpDatResumeReq(NhUiNhuPdcpDatResumeReq, pst, mBuf);
               break;
            case  EVTNHUDCODEREQ:
               cmUnpkNhuDecodeReq(NhUiNhuDecodeReq, pst, mBuf, &sMem, 
                                    maxBlkSize);
               break;
            case  EVTNHUCOUNTREQ:
               cmUnpkNhuCountReq(NhUiNhuCountReq, pst, mBuf, &sMem, maxBlkSize);
               break;
            default:
               /* Invalid event */ 
               NH_FREEMBUF(mBuf);
               RLOG1(L_ERROR, "Invalid event [%d] from RRC user", pst->event);
               break;
         }/* end of switch on pst->event */
         break;
      }/*end of case ENTNX */
#endif /* LCNHUINHU */

      case ENTPJ : /* PDCP Layer events */ 
      {
         switch(pst->event)
         {
#ifdef LCNHLIPJU
            case EVTPJUBNDCFM :
               cmUnpkPjuBndCfm(NhLiPjuBndCfm, pst, mBuf);
               break;
            case EVTPJUDATCFM :
               ret = cmUnpkPjuDatCfm(NhLiPjuDatCfm, pst, mBuf);
               if (ret == RIGNORE)
               {
                  RLOG1(L_ERROR, "numSdus in PDCP datCfm exceeded [%d], "
                     "ignoring this message\n",PJU_MAX_SDU_CFM);
               }
               break;
            case EVTPJUDATIND :
               cmUnpkPjuDatInd(NhLiPjuDatInd, pst, mBuf);
               break;
#ifdef SS_RBUF                 
            case EVTFPPJUDATIND :
               cmUnpkFpPjuDatInd(NhLiPjuDatInd, pst, mBuf);
               break;
#endif
            case EVTPJUSTAIND:
               cmUnpkPjuStaInd(NhLiPjuStaInd, pst, mBuf);
               break;
#endif /* LCNHLIPJU */
#ifdef LCNHLICPJ
            case EVTCPJBNDCFM :
               cmUnpkCpjBndCfm(NhLiCpjBndCfm, pst, mBuf);
               break;
            case EVTCPJCFGCFM :
               cmUnpkCpjCfgCfm(NhLiCpjCfgCfm, pst, mBuf);
               break;
            case EVTCPJUEIDCHGCFM:
               cmUnpkCpjUeIdChgCfm(NhLiCpjUeIdChgCfm, pst, mBuf);
               break;
            case EVTCPJSECCFGCFM:
               cmUnpkCpjSecCfgCfm(NhLiCpjSecCfgCfm, pst, mBuf);
               break;
            case EVTCPJREESTCFM:
               cmUnpkCpjReEstCfm(NhLiCpjReEstCfm, pst, mBuf);
               break;
            case EVTCPJCOUNTCFM:
               cmUnpkCpjCountCfm(NhLiCpjCountCfm, pst, mBuf);
               break;
            case EVTCPJSDUSTACFM:
               cmUnpkCpjSduStaCfm(NhLiCpjSduStaCfm, pst, mBuf);
               break;
            case EVTCPJDATRESUMECFM:
               cmUnpkCpjDatResumeCfm(NhLiCpjDatResumeCfm, pst, mBuf);
               break;
#endif /* LCNHLICPJ */
            default:
               /* Invalid event */ 
               NH_FREEMBUF(mBuf);
               RLOG1(L_ERROR, "Invalid event [%d] from PDCP", pst->event);
               break;
         }/* end of switch on pst->event */
         break;
      }/*end of case ENTPJ */   

      case ENTKW : /* RLC Layer events */ 
      {
         switch(pst->event)
         {
#ifdef LCNHLIKWU
            case KWU_EVT_BND_CFM:
               cmUnpkKwuBndCfm(NhLiKwuBndCfm, pst, mBuf);
               break;
            case KWU_EVT_DAT_IND: 
               cmUnpkKwuDatInd(NhLiKwuDatInd, pst, mBuf);
               break;
#endif /* LCNHLINHU */       
#ifdef LCNHLICKW
            case CKW_EVT_BND_CFM:
               cmUnpkCkwBndCfm(NhLiCkwBndCfm, pst, mBuf);
               break;
            case CKW_EVT_CFG_CFM:
               cmUnpkCkwCfgCfm(NhLiCkwCfgCfm, pst, mBuf);
               break;
            case CKW_EVT_UEIDCHG_CFM: 
               cmUnpkCkwUeIdChgCfm(NhLiCkwUeIdChgCfm, pst, mBuf);
               break;
#endif /* LCNHLICKW */
            default:
               /* Invalid event */ 
               NH_FREEMBUF(mBuf);
               RLOG1(L_ERROR, "Invalid event [%d] from RLC", pst->event);
               break;
         }/* end of switch on pst->event */
         break;
      }/*end of case ENTKW */   

#ifdef LCNHLICRG
      case ENTRG : /* MAC Layer events */ 
      {
         switch(pst->event)
         {
            case EVTCRGBNDCFM :
               cmUnpkCrgBndCfm(NhLiCrgBndCfm, pst, mBuf);
               break;
            case EVTCRGCFGCFM :
               cmUnpkCrgCfgCfm(NhLiCrgCfgCfm, pst, mBuf);
               break;
            default:
               /* Invalid event */ 
               NH_FREEMBUF(mBuf);
               RLOG1(L_ERROR, "Invalid event [%d] from MAC", pst->event);
               break;
         }/* end of switch on pst->event */
         break;
      }/*end of case ENTRG */   
#endif /* LCNHLICRG */

#ifdef RRC_PHY_CFG_SUPP

#ifdef LCNHLICTF
      case ENTTF : /* PHY Layer events */ 
      {
         switch(pst->event)
         {
            case EVTCTFBNDCFM :
               cmUnpkCtfBndCfm(NhLiCtfBndCfm, pst, mBuf);
               break;
            case EVTCTFCFGCFM :
               cmUnpkCtfCfgCfm(NhLiCtfCfgCfm, pst, mBuf);
               break;
            case EVTCTFUEIDCHGCFM:
               cmUnpkCtfUeIdChgCfm(NhLiCtfUeIdChgCfm, pst, mBuf);
               break;
            default:
               /* Invalid event */ 
               NH_FREEMBUF(mBuf);
               RLOG1(L_ERROR, "Invalid event [%d] from PHY", pst->event);
               break;
         }/* end of switch on pst->event */
         break;
      }/*end of case ENTTF */   
#endif /* LCNHLICTF */
#endif /* RRC_PHY_CFG_SUPP */


      default:
         NH_FREEMBUF(mBuf);
         RLOG1(L_ERROR, "Event from Unknown entity[%d]", pst->srcEnt);
         break;
   } /* end of switch */

   SExitTsk();

   RETVALUE(ROK);
} /* end of nhActvTsk */


/**********************************************************************
 
         End of file:     ni_ex_ms.c@@/main/3 - Fri Jul  1 01:13:18 2011
 
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
/main/1      ---      vk               1. LTE RRC Initial Release.
/main/2      ---      ch               1. RRC Release 2.1. 
/main/3      ---      sbalakrishna     1. Updated for Release of 3.1.
*********************************************************************91*/
